<?php 
/**
 * Data Load API authentication by OAuth2 
 */
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/GlobalUtil.php";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/Trace.php";

$util=new GlobalUtil();
$config=$util->getConfig();
$peerVerify = $config['peer_verify'] == 0 ? FALSE : TRUE;

try{
	$context = stream_context_create(array(
		'http' => array(
				'method' => 'POST',
				'header' => 'Content-Type: application/x-www-form-urlencoded; charset=utf-8',
				'content' => 'access_token='.$_POST['access_token']
				),
			'ssl' => array('verify_peer'=>$peerVerify, 
           'verify_peer_name'=>$peerVerify)
	));
	
	$signed = file_get_contents($_POST['baseurl'].'/dataload/request_validation.php', false, $context);
	if(stripos($signed,'Invalid token or scope')!==false || stripos($signed,'Fatal error')!==false){
		throw new Exception("Invalid token or permission denied.");
	}
}catch(Exception $e){
	$ErrMsg = "Authentication failed. ".$e->getMessage();
	$response['Status']	= 'Failure';
	$response['ErrorMessage'] = $ErrMsg;
	expDebug::dPrint( ' Error message '.print_r($response,true),1);
	$returnType = $_POST['Return_Type'];
	if($returnType == 'xml'){
		$xml = new SimpleXMLElement('<root/>');
		array_walk_recursive(array_flip($response), array ($xml, 'addChild'));
		print $xml->asXML();
	}else{
		echo json_encode($response);
	}
	exit;
}

define('DRUPAL_ROOT', $_SERVER['DOCUMENT_ROOT']);
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
require_once DRUPAL_ROOT . '/includes/entity.inc';
require_once DRUPAL_ROOT . '/modules/user/user.module';
include_once $_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_core/modules/ip_ranges/ip_ranges.module';
//include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/GlobalUtil.php";
drupal_bootstrap(DRUPAL_BOOTSTRAP_SESSION);

$list = system_list('module_enabled'); // load enabled modules from cache
if(isset($list['ip_ranges']))
{
	ip_ranges_boot();
}

// API Process started upon successfull authentication 

include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/exp_sp_core.inc";
require_once $_SERVER["DOCUMENT_ROOT"].'/dataload/exp_sp_data_load.php';
require_once $_SERVER["DOCUMENT_ROOT"].'/dataload/Database.php';

expDebug::dPrint('$_POST '. print_r($_POST,true),4);

//$ErrMsg = '' ;
$reqType = $_POST['Request_Type'];
$baseurl = $_POST['baseurl'];
$returnType = $_POST['Return_Type'];


if(empty($reqType)) {
	$ErrMsg = 'Request Type is missing in Post request.';
	$response['Status']	= 'Failure';
	$response['ErrorMessage'] = $ErrMsg;
	expDebug::dPrint( ' Error message '.print_r($response,true),5);
	//echo $response;
	if($returnType == 'xml'){
		$xml = new SimpleXMLElement('<root/>');
		array_walk_recursive(array_flip($response), array ($xml, 'addChild'));
		print $xml->asXML();
	}else{
		echo json_encode($response);
	}
	exit;
}

switch($reqType){
	case 'createReq':
		$ErrMsg = createJobValidate();
		if(!empty($ErrMsg)){
			$response['Status']	= 'Failure';
			$response['ErrorMessage'] = $ErrMsg;
			expDebug::dPrint(' $response '. print_r($response,true),5);
		}else{
			// TODO: File format check and virus scan need to be implement
			$filename = isarray($_FILES['File_Upload']['name']);
			$filedata = isarray($_FILES['File_Upload']['tmp_name']);
			$filesize = isarray($_FILES['File_Upload']['size']);
			$fileext = pathinfo($filename, PATHINFO_EXTENSION);
			
			// File format validation
			if(!in_array($fileext,array('csv','txt'))){
				$response['Status']	= 'Failure';
				$response['ErrorMessage'] = "Invalid file format. Only csv or txt allowed";
				if($returnType == 'xml'){
					$xml = new SimpleXMLElement('<root/>');
					array_walk_recursive(array_flip($response), array ($xml, 'addChild'));
					print $xml->asXML();
				}else{
					echo json_encode($response);
				}
				exit;
			}
			$file_name = substr_replace($filename, '_'.microtime(false), strpos($filename,'.'));
		 	$file_path = str_replace(' ', '_', $config['dataload_file_path'] . "/" . $file_name.'.'.$fileext);
			move_uploaded_file($_FILES['File_Upload']['tmp_name'],$file_path);
			//$_POST['file_path'] = $file_path;
			
			$startDate = new DateTime($_POST['Starts_when']);
			$jobs = array(
				'Entity_Name' => $_POST['Entity_Name'],
				'file_path' => $file_name.'.'.$fileext,
			  'file_size' => $filesize,
				'Process_Type' => $_POST['Process_Type'],
				//'Upload_Type' => $_POST['Upload_Type'],
				'User_Id' => $_POST['User_Id'],
				'Mail_To' => $_POST['Mail_To'],
				'Starts_when' => $startDate->format('Y-m-d H:i'),
		  );
			try{
				$obj = new dataLoadBase;
				$response = $obj->createNewJob($jobs);
			}catch(Exception $exp){
				//echo $exp->getMessage();
				$response['Status']	= 'Failure';
				$response['ErrorMessage'] = $exp->getMessage();
			} 
		} 
		break;
	case 'stsReq':
		$ErrMsg = mandatoryValidate();
		if(!empty($ErrMsg)){
			$response['Status']	= 'Failure';
			$response['ErrorMessage'] = $ErrMsg;
			expDebug::dPrint(' $response '. print_r($response,true),5);
		}else{
			$jobSt['Job_Id'] = $_POST['Job_Id'];
			$jobSt['Status_Type'] = $_POST['Status_Type'];
			//$jobSt['Message'] = $_POST['Message'];
			try{
				$obj = new dataLoadBase;
				$response = $obj->getJobStatus($jobSt);
				expDebug::dPrint('get job details '.print_r($response,true),5);
			}catch(Exception $exp){
				//echo $exp->getMessage();
				$response['Status']	= 'Failure';
				$response['ErrorMessage'] = $exp->getMessage();
			}
		}
		break;
	case 'pauseReq':
	case 'endReq':
		$ErrMsg = mandatoryValidate();
		if(!empty($ErrMsg)){
			$response['Status']	= 'Failure';
			$response['ErrorMessage'] = $ErrMsg;
			expDebug::dPrint(' $response '. print_r($response,true),5);
		}else{
			try{
				$obj = new JobTermination;
				$actType = (!empty($_POST['Action_Type'])) ? $_POST['Action_Type'] : '';
				$response = $obj->setJobProcessingStatus($_POST['Job_Id'],$actType,$returnType);
				expDebug::dPrint('Response '.print_r($response,true),5);
			}catch(Exception $exp){
				//echo $exp->getMessage();
				$response['Status']	= 'Failure';
				$response['ErrorMessage'] = $exp->getMessage();
			}
		}
		break;
	
} 
function createJobValidate(){
	$processType = $_POST['Process_Type'];
	$entityType = $_POST['Entity_Name'];
	$UserId =  $_POST['User_Id'];
	$email = $_POST['Mail_To'];
	$startDt = $_POST['Starts_when'];
	//$filename = $_POST['filename'];
	$filename = isarray($_FILES['File_Upload']['name']);
	//$UserId ='';
	$fields = '';
	$ErrMsg = '';
	$date = '';
	$mailtosend = '';
	if(empty($processType)){
		$fields = 'ProcessType';
	}
	/* if(empty($_POST['Upload_Type'])){
		$fields = 'UploadType';
	} */
	if(empty($entityType)){
		$fields .= ($fields == '') ? 'Entity Type' : ',Entity Type';
	}
	if(empty($UserId)){
		$fields .= ($fields == '') ? 'User Id' : ',User Id';
	}
	if(empty($filename)){
		$fields .= ($fields == '') ? 'File name' : ',File name';
	}

	$ErrMsg = ($fields == '')? '' : 'Folowing Parameters are needed '.$fields.'.';
	
	//UserId validation
	$ValidUser = validUserorNot($UserId);
	//expDebug::dPrint('valid check '.print_r($ValidUser,true),5);
	$ErrMsg .= !empty($ValidUser[0]->cnt) ? '' : 'Invalid User Id';
	
	//Email validation
	if(!empty($email)){
		$emailList = explode(',',$email);
		$UserMail = $ValidUser[0]->email;
		foreach( $emailList as $key=>$mail){
			//expDebug::dPrint(' $$key '. $key. '$mail '. $mail. 'usermail '.$UserMail,5);
			if(!validateEmail($mail)){
				unset($emailList[$key]);
			}elseif($mail == $UserMail){
				unset($emailList[$key]);
			}
		}
		//expDebug::dPrint(' $Emails '. print_r($emailList,true),5);
		$mailtosend = implode(',',$emailList);
	}
	//expDebug::dPrint(' $mailtosend '. $mailtosend,5);
	$_POST['Mail_To'] = $mailtosend;
	
	//Date Validation
	if(!empty($startDt)){
		expDebug::dPrint('start date'.$startDt,5);
		if(!dateValidation($startDt)){
			$ErrMsg .= 'Date and Time is not in correct format.';
		}
	} 

	return $ErrMsg; 
} 
function mandatoryValidate(){
	$reqType = $_POST['Request_Type'];
	$UserId =  $_POST['User_Id'];
	$fields = '';
	$ErrMsg = '';
	$JobId = $_POST['Job_Id'];
	if(empty($JobId)){
		$fields = 'Job Id';
	}
	if($reqType == 'stsReq'){
		$StsType = $_POST['Status_Type'];
		
		if(empty($StsType)){
			$fields .= ($fields == '') ? 'Status Type' : ',Status Type';
		}
	}elseif($reqType == 'pauseReq'){
		$ActType = $_POST['Action_Type'];
		if(empty($ActType)){
			$fields .= ($fields == '') ? 'Action Type' : ',Action Type';
		}
	}else{
		//Terminate process
	}
	if(empty($UserId)){
		$fields .= ($fields == '') ? 'User Id' : ',User Id';
	}else{
		$ValidUser = validUserorNot($UserId);
		$ErrMsg .= !empty($ValidUser[0]->cnt) ? '' : 'Invalid User Id';
	}
	
	$ErrMsg .= (empty($fields)) ? '' : 'Folowing Parameters are needed '.$fields.'.';
	return $ErrMsg;
}


function validUserorNot($userId){
	$db = new DLDatabase();
	$field = array(
		'count(1) as cnt',
		'per.email'
	);
	$con = array(
		'id = :uid'
	);
	$arg = array(
		':uid'=>$userId
	);
	$query = $db->prepareQuerySelect('slt_person per',$field,$con);
	$db->callQuery($query,$arg);
	$Userexists = $db->fetchAllResults();
	return $Userexists;
}

expDebug::dPrint(' $response '. print_r($response,true),5);

if(!empty($response)){
	if($returnType == 'xml'){
		$xml = new SimpleXMLElement('<root/>');
		array_walk_recursive(array_flip($response), array ($xml, 'addChild'));
		print $xml->asXML();
	}else{
		echo json_encode($response);
	}
}

// Check if the date is "DD/MM/YYYY HH:MI" format
// Also check if the give date is grater than now
function dateValidation($date){
	try{
		if(substr_count($date,'/') != 2 || substr_count($date,":") != 1 || substr_count($date," ") != 1)
			return false;
		
		list($date, $time) = explode(' ',$date);
		
		if(empty($time))
			$time = "00:00";
			
		// Date format check
		if(empty($date) || empty($time) || 
		    strlen($date) < 6 || strlen($time) < 3)
			return false;
			
		list( $m, $d, $y ) = preg_split( '/[-\.\/ ]/', $date );
		
		if(preg_match('/[^0-9]/', $m) || (int) $m > 12 || (int) $m <= 0
		 	 || preg_match('/[^0-9]/', $d) || (int) $d > 31 || (int) $d <= 0
		 	 || preg_match('/[^0-9]/', $y) || strlen($y)!=4){
		 	 	return false;
		 	 }
		
		// Time format check
		list($h,$n) = explode(":",$time);
		
		if(preg_match('/[^0-9]/', $h) || (int) $h > 24
		  || preg_match('/[^0-9]/', $n) || (int) $n > 60){
		  	return false;
		  }
		
		// Past date check

		$gDate = new DateTime($date." ".$time);
		$gDate = $gDate->format('Y-m-d H:i');

		$cDate = new DateTime("now");
		$cDate = $cDate->format('Y-m-d H:i');

		//expDebug::dPrint('$$gDate '.print_r($gDate,true),5);
		//expDebug::dPrint('$cDate '.print_r($cDate,true),5);

		if($gDate < $cDate)
			return false;
			
		return true;
	}catch(Exception $e){
		// throw new Exception($e->getMessage());
		return false;
	}
}

function isarray($arrVal=''){
	return is_array($arrVal) ? $arrVal[0] : $arrVal;
}
?>