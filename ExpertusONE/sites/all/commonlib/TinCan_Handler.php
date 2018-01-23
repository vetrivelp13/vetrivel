<?php
/*
 *  Each time the content application sends a request message to Smart Portal LMS, the LMS server returns an
 *  appropriate response message. In response to putParam, putInteractions, and exitAU requests,
 *  LMS returns a simple acknowledgment in the form of an error code.
 *  In response to a getParam request, LMS returns a text-based output stream of data, including
 *  information about the learner and lesson data collected and stored from previous learning
 *  sessions, if any.
 *  Error Number	Error Text
 *  0				Successful
 *  1				Invalid Command
 * -1				Failed Database save
 *
 */
require_once "../services/GlobalUtil.php";
require_once "../services/Trace.php";
include_once "../services/FPM_Includes.php";
require_once "../dao/AbstractDAO.php";
require_once "../services/Encryption.php";
require_once "../libraries/tincan_php_library/tincan_client.php";


header("Content-Type:text/html");
header("Cache-Control: no-store, no-cache, must-revalidate, post-check=0, pre-check=0");
header("Pragma: no-cache");
header("ETag: PUB" . time());
$isError=0;
$score = '';
$reqHeaders = getallheaders();
$putdata=file_get_contents('php://input');
$parsedata = array();
$version = 0;
if(isset($reqHeaders['Authorization']) && !empty($reqHeaders['Authorization'])){
	$parsedata = $putdata;
	$version = 1;
}else{
parse_str(urldecode($putdata), $parsedata);
}
expDebug::dPrint("Tincan handler :: request header >>>>>".print_r($reqHeaders,true),4);
$session_id = isset($reqHeaders['Authorization']) && !empty($reqHeaders['Authorization']) ? $reqHeaders['Authorization'] : $parsedata['Authorization'];
expDebug::dPrint("Tincan handler :: Parser Data".print_r($parsedata,true),4);
expDebug::dPrint("_POST :: _POST Data".print_r($_POST,true),4);
if(isset($_POST['launchflag']) &&  $_POST['launchflag']==1){
	$session_id=$_POST['Authorization'];
}

expDebug::dPrint("Session id >>>> ".$session_id);
foreach ($_POST as $key => $value)
{
	expDebug::dPrint("AICC POST KEY - ".$key."  - Value  - ".$value,4);
	$tempkey = strtolower($key);
	$_POST[$tempkey] = $value;
}

expDebug::dPrint("Tincan handler :: command".print_r($_POST["command"],true),4);
expDebug::dPrint("Tincan handler :: content_token".print_r($_POST["content_token"],true),4);


$cmd = isset($_POST["command"])?$_POST["command"]:'';
$cmd = strtoupper($cmd);
$aicc_id_mob = isset($_POST["content_token"])?$_POST["content_token"]:'';

expDebug::dPrint("Tincan handler :: command1".print_r($cmd,true),4);
expDebug::dPrint("Tincan handler :: content_token1".print_r($aicc_id_mob,true),4);


if(!empty($session_id)){
	$req_param = explode('-E1-',$session_id);
	$req_id='';
	if(sizeof($req_param)>2){
		$req_id = $req_param[sizeof($req_param)-1];
		unset($req_param[sizeof($req_param)-1]);
		$session_id = implode('-E1-',$req_param);
	}else{
		$session_id = $req_param[0];
		$req_id = $req_param[1];
	}
	$enc2 = new Encrypt();
	$req_id = $enc2->decrypt($req_id);
	expDebug::dPrint(" Current learner's session_id -- ".$session_id);
	expDebug::dPrint(" Tincan Interaction id -- ".$req_id);

	$pdao=new AbstractDAO();
	$pdao->connect();
	$qry = "SELECT aicc_id,session_id,param FROM slt_aicc_interaction WHERE id='".$req_id."'";
	expDebug::dPrint("Tincan Handler::Get session details :".$qry,2);
	$res = $pdao->query($qry);
	$sesdt=$pdao->fetchAllResults();
	$pdao->closeconnect();
	if(sizeof($sesdt)<=0){ //|| $session_id!=$sesdt[0]->session_id
		$isError=1;		
	}else if(substr_count($sesdt[0]->aicc_id, '-')!=6){
		$aicc_sid =$aicc_id_mob;
		$sid = explode("-",$aicc_sid);
	}
	else{
		$aicc_sid  = $sesdt[0]->aicc_id;
		$sid = explode("-",$aicc_sid);
	}
	expDebug::dPrint("Tincan aicc id :".$aicc_sid,4);
}else if ($cmd == "SETSESSIONMOBILE"){
	expDebug::dPrint("Inside SETSESSIONMOBILE");
 	$session_id = setSessionAiccMobile("set",$aicc_id_mob );
 	echo $session_id;
 	exit();
}else{	
	$isError=1;
	expDebug::dPrint("Tincan Handler::Launch Error :".$resp,1);
}
if($isError == 1){
	$resp = "Error=1\r\n";
	$resp.= "Error_Text=Invalid Access\r\n";
	$resp.= "version=3.0\r\n";
	echo $resp;
}
else if($isError==0){
	$enr_id = explode('-',$aicc_sid);
	$dao=new AbstractDAO();
	$dao->connect();
	$dao->query("SELECT reg_status FROM slt_enrollment WHERE id =". trim($enr_id[5]).";");
	$result_reg_status = $dao->fetchResult();
	$reg_status = $result_reg_status->reg_status;
	$dao->closeconnect();
	if($aicc_sid){
		$pdao=new AbstractDAO();
		$pdao->connect();
		$qry = "SELECT id,full_name FROM slt_person WHERE id='".$sid[0]."'";
		expDebug::dPrint("Tincan Handler::Get User details :".$qry,2);
		$res = $pdao->query($qry);
		$usrdt=$pdao->fetchAllResults();
		$pdao->closeconnect();
	}
	$vUserId 			= $usrdt[0]->id;
	$vUserName 			= $usrdt[0]->full_name;
	expDebug::dPrint("User id :".$vUserId,4);
	expDebug::dPrint("User Name :".$vUserName,4);
	expDebug::dPrint( date("m/d/y H:i:s",$currenttime)." ". " ===> aicc_data:" .$aicc_data . "\n",4);
	
	if($version == 1){ // for 0.9 version content
		expDebug::dPrint("get calue ".print_r($_GET,true),5);
		expDebug::dPrint("parsedat ".print_r($parsedata,true),5);
		if(isset($_GET['stateId']) && empty($parsedata)){
			$dao2=new AbstractDAO();
			$dao2->connect();
			$qry2 = "SELECT param FROM slt_aicc_interaction WHERE id='".$req_id."' and session_id='".$session_id."'";
			expDebug::dPrint("Tincan Handler::GETPARAM - slt_aicc_interaction Query :".$qry2,2);
			$res2 = $dao2->query($qry2);
			$result2=$dao2->fetchAllResults();
			$dao2->closeconnect();
			
			if(empty($result2[0]->param)){
				$dao=new AbstractDAO();
				$dao->connect();
				$qry = "SELECT aicc_data FROM slt_attendance_summary WHERE enrollment_id='".trim($enr_id[5])."' and user_id='".$vUserId."'";
				expDebug::dPrint("Tincan Handler::GETPARAM - slt_aicc_interaction Query :".$qry,2);
				$res = $dao->query($qry);
				$result=$dao->fetchAllResults();
				$dao->closeconnect();
			}
			expDebug::dPrint("restilasd test".print_R($result,true),5);
			if(empty($result[0]) || empty($result[0]->aicc_data)){
				$aiccdata = '';
			}
			else{
				$data = json_decode($result[0]->aicc_data);						
				$aiccdata = isset($data->$_GET['stateId']) ? $data->$_GET['stateId'] : '';				
				//captureDataTinCan($result[0]->aicc_data,$session_id,$sid[0]);
			}			
			echo $aiccdata;
		}else if(isset($_GET['stateId']) && !empty($parsedata)){		
			$dao2=new AbstractDAO();
			$dao2->connect();
			$qry2 = "SELECT param FROM slt_aicc_interaction WHERE id='".$req_id."' and session_id='".$session_id."'";
			expDebug::dPrint("Tincan Handler::GETPARAM - slt_aicc_interaction Query :".$qry2,2);
			$res2 = $dao2->query($qry2);
			$result2=$dao2->fetchAllResults();
			$dao2->closeconnect();
			
			$data = new stdClass();			
			if(!empty($result2[0]->param)){
				$data = json_decode($result2[0]->param);
			}			
			$data->$_GET['stateId'] = $parsedata;
			$aicc_data = json_encode($data);			
			captureDataTinCan($aicc_data,$session_id,$sid[0],$version);			
			
		}
		
		
	}else{
		if($parsedata['stateId'] == 'resume' && empty($parsedata['content'])){
			$dao=new AbstractDAO();
			$dao->connect();
			$qry = "SELECT aicc_data FROM slt_attendance_summary WHERE enrollment_id='".trim($enr_id[5])."' and user_id='".$vUserId."'";
			expDebug::dPrint("Tincan Handler::GETPARAM - slt_aicc_interaction Query :".$qry,2);
			$res = $dao->query($qry);
			$result=$dao->fetchAllResults();
			$dao->closeconnect();
			if(empty($result[0]) || empty($result[0]->aicc_data)){
				$aiccdata = '';
			}
			else{
				$aiccdata = $result[0]->aicc_data;
			}
			echo $aiccdata;
		}else if($parsedata['stateId'] == 'resume' && !empty($parsedata['content'])){			
			$aicc_data = $parsedata['content'];
			captureDataTinCan($aicc_data,$session_id,$sid[0]);
		}
	}
	expDebug::dPrint(" Attributs >>> ".$vUserId."--".$vUserName."--".$sid[6],4);
	if($sid[1] == 0){ // preview content 
		updateAiccPreview($session_id);
	}else{ // launch content
		expDebug::dPrint("json Data ::sadas ".print_r($parsedata,true),5);
		if(isset($reqHeaders['Authorization']) && !empty($reqHeaders['Authorization'])){
			$jsonData = json_decode($parsedata);	
		}else{
		$jsonData = json_decode($parsedata['content']);
		}	
		expDebug::dPrint("json Data ::jsonDatajsonData ".print_r($jsonData,true),5);		
		//storeTincanDataIntoLRS($aicc_sid,$session_id,$vUserId,$jsonData);
		expDebug::dPrint("json Data :: FULL ".print_r($jsonData,true),5);
		if($parsedata['command'] == "UPDATELMSDATA")
			$sts['status'] = "attempted";
		else
			$sts = getCompletionStatus($jsonData,$version);
		if(!empty($sts)){
			//if($reg_status == 'lrn_crs_reg_cnf') {
			  updateLMSDataTincan($aicc_sid,$session_id,$vUserId,$sts,$version);
			//}
			}
		}
		storeTincanDataIntoLRS($aicc_sid,$session_id,$vUserId,$jsonData, $sts);
	}

function setSessionAiccMobile($type,$aicc_sid = 0){
	if($type=="set"){
		//$current_session = session_id();
		expDebug::dPrint("current_sessioncurrent_sessioncurrent_sessioncurrent_session".$current_session,2);
		$sid = explode("-",$aicc_sid);
		$current_session = 'T0tcGFYI_x8DrEVwvbRiV1hos5AJpcYT5eRk6E_2b'.$sid[0].'_'.$sid[1].'_'.$sid[2].'_'.$sid[3].'_'.$sid[4].'_'.$sid[5];
		expDebug::dPrintDBAPI('$aicc_sidSession id store while launch AICC -->',$aicc_sid);
		$pdao=new AbstractDAO();
	    $pdao->connect();
	    $qry = "SELECT param FROM slt_aicc_interaction WHERE aicc_id='".$aicc_sid."'";
	    expDebug::dPrint("AICC Handler::Get session details :".$qry,2);
		$res = $pdao->query($qry);
		$param=$pdao->fetchResult();
		$pdao->closeconnect();
		expDebug::dPrint("Tincan handler :: setSessionAiccMobile".$param->param);
		$param =$param->param;
		expDebug::dPrint("Tincan handler :: setSessionAiccMobile".$param);
				
		// Delete any previous entry if any
		
		$pdao=new AbstractDAO();
	    $pdao->connect();
	    $qry = "DELETE FROM slt_aicc_interaction WHERE aicc_id='".$aicc_sid."' AND session_id='".$current_session."' ";
	    expDebug::dPrint("AICC Handler::Get session details :".$qry,2);
		$res = $pdao->execute($qry);
		$pdao->closeconnect();
		expDebug::dPrint("Tincan handler :: Delete any previous entry if any");
		
		// Insert session session details
		$pdao=new AbstractDAO();
	    $pdao->connect();
	    $qry = "INSERT INTO slt_aicc_interaction (aicc_id,session_id,param,connection_status, created_by, created_on) VALUES  ('".$aicc_sid."',". 
			"'".$current_session."',".
			"'".$param."',". 
			"'N'".",".
			"'".$sid[0]."',". 
			"now());"; 
			 
	    
	    expDebug::dPrint("AICC Handler::Get session details :".$qry,2);
		$res = $pdao->execute($qry);
		$res = $pdao->getLastRecordNo();
		$pdao->closeconnect();
		expDebug::dPrint("Tincan handler :: Insert session session details".$res);
		$enc2 = new Encrypt();
		$aicc_sid = $enc2->encrypt($res);
		return $current_session.'-E1-'.$aicc_sid;
		//return (array('session_id'=>$current_session.'-E1-'.$aicc_sid)); 
	}else{
		return (array('session_id'=>session_id()));
	}
}




function getConsolidatedScore($Jsonresult){
	if(!empty($Jsonresult->score)){
		if(array_key_exists("scaled",$Jsonresult->score)){
			 return ($Jsonresult->score->scaled * 100);
		}else if(array_key_exists("raw",$Jsonresult->score) && array_key_exists("max",$Jsonresult->score)) {
			if($Jsonresult->score->max != 0)
				return ($Jsonresult->score->raw/$Jsonresult->score->max)*100;
			else
				return ($Jsonresult->score->raw);
		}
	}
}
function getCompletionStatus($data,$version){
	expDebug::dPrint("version check".$version);
	$action = '';
	$return = array();
	if($version == 1){
		if(!empty(end($data)->verb->id)){
			$action_ps = explode("/verbs/",end($data)->verb->id);
			$action = $action_ps[1];
		}else if(!empty(end($data)->verb)){
			$action = end($data)->verb;
		}
		if(end($data)->result->completion || $action == "passed" || $action == "failed"){
			if(end($data)->result->completion === "false"){
				return '';
			}
			$score = getConsolidatedScore(end($data)->result);
			$return['status'] = $action;
			$return['score'] = $score;
			return $return;
		}else{
			expDebug::dPrint("data get action asde");
			return '';
		}	
	}else{
	if(!empty($data->verb->id)){
			$action_ps = explode("/verbs/",$data->verb->id);
			$action = $action_ps[1];
	}else if(!empty($data->verb)){
		$action = $data->verb;
	}
	if($data->result->completion || $action == "passed" || $action == "failed"){
		if($data->result->completion === "false"){
				return '';
		}
		$score = getConsolidatedScore($data->result);
		$return['status'] = $action;
		$return['score'] = $score;
		return $return;
	}else{
		 return '';
	}
}
}
function captureDataTinCan($aicc_data,$session_id,$vUserId){
	$qry1='';
	$qry2='';
	$rst1;
	if(!empty($session_id)){
		try{			
				$qry2 = "UPDATE slt_aicc_interaction SET param='".addslashes($aicc_data)."', connection_status='Y', updated_by='".$vUserId."', updated_on = now() WHERE session_id='".$session_id."'";
			if(!empty($qry2)){
				$dao=new AbstractDAO();
				$dao->connect();
				expDebug::dPrint("Tin Can Handler::captureData - Insert/Update Query :".$qry2,2);
				$res = $dao->execute($qry2);
				$dao->closeconnect();
			}
		}catch(Exception $e){
			$dao->closeconnect();
			expDebug::dPrint("Tin Can Handler::captureData Error :".$e->getMessage());
		}
	}
}

function updateLMSDataTincan($aicc_sid,$session_id,$vUserId,$result,$version=0){
	$aicc_sid = trim($aicc_sid);
	try{
		define('DRUPAL_ROOT', $_SERVER['DOCUMENT_ROOT']);
		include_once $_SERVER['DOCUMENT_ROOT'].'/includes/bootstrap.inc';
		require_once "../modules/core/exp_sp_core/exp_sp_core.inc";
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		//expDebug::dPrint('Update user data '.print_r(func_get_args(),true),5);
		$conObj = new GlobalUtil();
		$conf = $conObj->getConfig();
		$tincan = $conf['tincanapi'];
		expDebug::dPrint("getConfigValue ".print_r($tincan,true),5);
		$total_time_spend_update ='';
		$dao=new AbstractDAO();
		$dao->connect();
		$qry1 = "SELECT id,aicc_id,param,interaction,connection_status FROM slt_aicc_interaction WHERE session_id='".str_replace("=","", $session_id)."'";
		expDebug::dPrint("Tincan Handler::updateLMSData - Query :".$qry1,2);
		$res = $dao->query($qry1);
		$rst1=$dao->fetchAllResults();
		$dao->closeconnect();
		//expDebug::dPrint('connection status ' . $rst1[0]->connection_status);
		//if($rst1[0]->connection_status == 'Y'){
			$calculateTime = true;
			if(!empty($rst1[0]->param)){
				$aicc_data = $rst1[0]->param;
				$aicc_int = $rst1[0]->interaction;
			}else{
				$sid = explode('-',$aicc_sid);
				$dao=new AbstractDAO();
				$dao->connect();
				$qry1 = "SELECT aicc_data FROM slt_attendance_summary WHERE user_id=trim($sid[0]) AND course_id=trim($sid[1]) AND class_id=trim($sid[2])";
				expDebug::dPrint("Tincan Handler::updateLMSData from previous value - Query :".$qry1,2);
				$res = $dao->query($qry1);
				$rst1=$dao->fetchAllResults();
				$dao->closeconnect();
				if(!empty($rst1[0])){
					$data = explode('[*Interaction Starts*]',$rst1[0]->aicc_data);
					$aicc_data = trim($data[0]);
					$aicc_int = trim($data[1]);
					$calculateTime = false;
				}else{
					$aicc_data = '';
					$aicc_int = '';
				}
			}			
			$aicc_data_dump = addslashes($aicc_data);
			$aicc_tmp= explode("\n",$aicc_data) ;
			expDebug::dPrint("Tincan Handler::updateLMSData from previous value - aicc_tmp :".$aicc_tmp,2);
			$status="lrn_crs_cmp_enr";
			//$score= $score.",";
			$score = $result['score'];
			$location;
			$timespend;
			$cstatus = $result['status'];
			$progress = null;
			if($result['status'] == "passed" || $result['status'] == "completed"){
				$status = "lrn_crs_cmp_cmp";
				$cstatus = "completed" ;
				$progress = 100;
			}
			elseif($result['status'] == "failed"){
				$status = "lrn_crs_cmp_cmp";
				$cstatus = "failed";
				$progress = 100;
			}elseif($result['status'] == "attempted"){
				$status = "lrn_crs_cmp_inp";
				$cstatus = "attempted";
			}else{
				$status = "lrn_crs_cmp_enr";
				$cstatus = "not attempted";
			}			
			$tsid = explode('-',$aicc_sid);

			$dao->connect();
			$dao->query("SELECT comp_status,master_enrollment_id FROM slt_enrollment WHERE id =". trim($tsid[5]).";");
			$resultStatus = $dao->fetchResult();
			$clsPrevStatus  = $resultStatus->comp_status;
			$master_enr_id = $resultStatus->master_enrollment_id;
			expDebug::dPrint("cls Prev Status ". $clsPrevStatus ,5);
			$dao->closeconnect();

			//Getting LRS enabled settings
			$lrs = $conf['lrs_enable'];
			$lrsEnabled = ($tincan == 1 && $lrs == 1) ? 'Y' : 'N';
			expDebug::dPrint('LRS enabled value'.$lrsEnabled,4);
			$splitTime = explode('.',$timespend);
			$tmeSpt = ($splitTime[0]*60 + $splitTime[1]);
			$totTime = gmdate("H:i:s",$tmeSpt);
			expDebug::dPrint("time spent values".$timespend."Modified time".$totTime,4);

			/*$qry3 = "call slp_attendance_details_ins(";
			$qry3 .= "'".trim($tsid[5])."',";
			$qry3 .= "'".trim($tsid[0])."',";
			$qry3 .= "'".trim($tsid[1])."',";
			$qry3 .= "'".trim($tsid[2])."',";
			$qry3 .= "'".trim($tsid[3])."',";
			$qry3 .= "'".trim($tsid[4])."',";
			$qry3 .= "'".trim($status)."',";
			$qry3 .= "'',";// added by yogaraja launch content count issue 35754.
			//$qry3 .= "'".trim($timespend)."',";
			$qry3 .= "'".trim($score)."',";;
			//$qry3 .= "null,";
			$qry3 .= "'".trim($location)."',";
			$qry3 .= "'".trim($cstatus)."',";
			$qry3 .= "'',";	//added for 49519: SCORM 1.2 and 2004 - Need to change content completion and attempts reduce
			$qry3 .= "'".trim($vUserId)."',";
			$qry3 .= "null,null,null,";
			$qry3 .= "0,"; //launchflag
			$qry3 .= "'".trim($aicc_data_dump)."',";
			$qry3 .= "'TinCan',";
			$qry3 .= "null,";// added by yogaraja launch content count issue 35754.
			$qry3 .= "null,";// added by yogaraja launch content count issue 35754.
			$qry3 .= "'".trim($lrsEnabled)."',";// added by Shobana LRS Content save.
			$qry3 .= "'".trim($totTime)."',";// added by Shobana LRS Content save.
			$qry3 .= "'lrn_cnt_typ_srm_tnc',";// added by yogaraja  Content code.
			$qry3 .= "'".$progress."',";// added by yogaraja  Content progress.			
			$qry3 .= "@insval,";
			$qry3 .= "@compstatus)";
			expDebug::dPrint("Tincan Handler::updateLMSData - Attendance Update :".$qry3,2);
			$dao->connect();
			$dao->beginTrans();
			$res1 = $dao->execute($qry3);
			$dao->commitTrans();
			$dao->query("select @insval Id,@compstatus CompStatus");
			$result = $dao->fetchResult();
			$dao->closeconnect();*/
			$statements = array('statement' => true);
			// Passing additional parameter $statements for final executions
			$result = lp_attendance_details_ins($tsid[5],$tsid[0],$tsid[1],$tsid[2],$tsid[3],$tsid[4],$status,'',$score,$location,$cstatus,'',$vUserId,'','','',$_POST['launchflag'],$aicc_data_dump,'TinCan','','',$lrsEnabled,$totTime,'lrn_cnt_typ_srm_tnc',$progress,'',$statements);
			dbStatementsExecution($statements);
			expDebug::dPrint("crs comp_status".$result->CompStatus,5);
			if($result->CompStatus != "Not Changed"){
			if($result->CompStatus == 'Completed' && $clsPrevStatus != 'lrn_crs_cmp_cmp') {
				$tincan_enr_id = empty($master_enr_id) ? $tsid[5] : $master_enr_id;
				$tincan_entity_type = empty($master_enr_id) ? 'class' : 'tp';
				insertUserPointsForEachActionsPerformedTincan($vUserId,'complete_class_training',$tincan_enr_id,'',$tincan_entity_type);				
				$dao->connect();
				$courseCompletedNotify = "CALL slp_lnr_notification_ins(" .
						"'course_completed', " .
						$tsid[0] . ", " .
						((!empty($tsid[2]))? $tsid[2] : 'null') . ", " .
						"'', " .
						"'', " .
						$tsid[0] . ")";
				expDebug::dPrintDBAPI("Tincan Handler::updateLMSData Completion Notification : SQL query = ", $courseCompletedNotify,4);
				$dao->query($courseCompletedNotify);
				$dao->closeconnect();
			}
			audit_trail_insert($result->CompStatus,$status,$aicc_sid,$vUserId);
		
			expDebug::dPrint("aicc tsid : ".print_r($tsid,true),4);		
		}
		//73059: Content completion % is not getting updated unless we close the window and reopen it or move to the next content
		if(isset($_POST['launchflag']) && $_POST['launchflag']==0){	
			$dao->connect();
			$queryDel = "DELETE FROM slt_aicc_interaction WHERE session_id='".str_replace("=","", $session_id)."'";
			expDebug::dPrint("Tincan Handler::updateLMSData DELETE query = " . $queryDel,2);
			$dao->query($queryDel);
			$dao->closeconnect();
		}elseif(isset($_POST['launchflag']) && $_POST['launchflag']==1){
			//73059: Content completion % is not getting updated unless we close the window and reopen it or move to the next content
			$dao->closeconnect();
			$lunObj = new stdClass;
			$lunObj->userId = $vUserId;
			$lunObj->RegId = $tsid[5];
			$lunObj->LearnerId = $tsid[0];
			$lunObj->CourseId = $tsid[1];
			$lunObj->ClassId = $tsid[2];
			$lunObj->LessonId = $tsid[3];
			$lunObj->VersionId = $tsid[4];
			$lunObj->Status = $status;
			$lunObj->SessionTime = $timespend;
			$lunObj->Score = $score;
			$lunObj->Location = $location;
			$lunObj->ContentStatus = $cstatus;
			$lunObj->prevStatus = $clsPrevStatus;
			$lunObj->contentCode = 'lrn_cnt_typ_aic';
			
			
			// get previous progress from summary table.
			$sid = explode('-',$aicc_sid);
			$dao=new AbstractDAO();
			$dao->connect();
			$qry1 = "SELECT progress FROM slt_attendance_summary WHERE enrollment_id  = $lunObj->RegId and  user_id=$lunObj->userId AND course_id=trim($lunObj->CourseId) AND class_id=trim($lunObj->ClassId) AND content_version_id = $lunObj->VersionId AND lesson_id = $lunObj->LessonId";
			expDebug::dPrint("Tincan Handler::updateLMSData from previous value - Query :".$qry1,2);
			$res = $dao->query($qry1);
			$rst1=$dao->fetchAllResults();
			$dao->closeconnect();
			$prev_progress = $rst1[0]->progress;
			if($progress>$prev_progress)
				$lunObj->progress = convertProgressValue($progress);
			else 
				$lunObj->progress = convertProgressValue($prev_progress);
			$lunObj->content_id = getContentMasterId($lunObj->VersionId);
			$lunObj->tot_progress = convertProgressValue(getMultiLessonDetails($lunObj->VersionId,$lunObj));
			expDebug::dPrint("AICC lunObj values :".print_r($lunObj,true),4);
			echo drupal_json_output($lunObj);
			exit();
		}
		// To sync solr data
		syncSolrData('Enrollment');
		syncSolrData('MasterEnrollment');
		syncSolrData('User');
		return $result;
	}catch(Exception $e){
		$dao->rollbackTrans();
		$dao->closeconnect();
		expDebug::dPrint("Tincan Handler::updateLMSData Error :".$e->getMessage(),1);
	}
}

function tincanCreateStatement_Tincan($lunObj){
	try{
		if(isset($lunObj->launchflag) && $lunObj->launchflag == 1){
			return;
		}else{
		expDebug::dPrint("contente id :--> ".$content_id);
		$url = $_SERVER['HTTP_HOST'];
		$calltype = $lunObj->calltype;
		$attemptCount = getAttemptCountTincan($lunObj->VersionId,$lunObj->ClassId,$lunObj->userId);
		$content_info = getContentInfoTincan($lunObj->VersionId);
		$content_id = $content_info->content_id;
		$content_code = $content_info->content_code;
		expDebug::dPrint("contente id :--> ".$content_id);
		$api_client = TinCanClient::Instance();
		$result = getCourseClassInfoTincan($lunObj->ClassId, $lunObj->userId, $lunObj->LessonId, $lunObj->VersionId, $lunObj->RegId, $content_id);
		expDebug::dPrint("fetch results ".print_r($dao,true),5);
		expDebug::dPrint("fetch results ".print_r($result,true),5);
		$courseName = $result->course_name;
		$classname = $result->class_name;
		//$summaryCount = $result->summaryCount;
		$max_attempts = $result->max_attempts;

		$getAssessmentCount = getAssessmentCount_tinCan($lunObj->ClassId,'cre_sys_obt_cls');
		expDebug::dPrint("Total getAssessmentCount ".print_r($getAssessmentCount->assessment_count,true),5);
		$assessment_count = $getAssessmentCount->assessment_count;

		$total_attempts = $attemptCount;
		expDebug::dPrint("Total Attempts ".$total_attempts);
		if($total_attempts == 1){
			$totalAttempts = $total_attempts."st";
		}elseif($total_attempts == 2){
			$totalAttempts = $total_attempts."nd";
		}elseif($total_attempts == 3){
			$totalAttempts = $total_attempts."rd";
		}else{
			$totalAttempts = $total_attempts."th";
		}

		if($lunObj->assessCheck == '0'){
			$max_attempts = $result->no_of_attempts;
		}else{
			$max_attempts = $result->max_attempts;
		}
		$validity_days = $result->validity_days;
		if($validity_days == 1){
			$days = 'day';
		}else{
			$days = 'days';
		}
		if($max_attempts > 0){
			$attempt_left = $max_attempts - $total_attempts;
		}else{
			$attempt_left = 0;
		}
		//$last_lesson_location = $result->last_lesson_location;
		$aicc_dataArray =  $result->aicc_data;

		//$exploe = explode("|",$aicc_dataArray);
		//$string = explode("=",$exploe[1]);
		
		$last_lesson_location = !empty($aicc_dataArray) ? true : false;

		$last_attempt_date = $result->last_attempt_date;
		//$total_time_spend = $result->total_time_spend;
		//$total_time_spend = $lunObj->SessionTime;
		$SessionTime = $lunObj->SessionTime;
		$parts = explode('.', $SessionTime);
		$total_time_spend = $parts[0]*60 + $parts[1];

		$explode = explode(" ", $last_attempt_date);
		$toDate = $explode[3]." ".$explode[4];
		expDebug::dPrint("from total_time_spend".print_r($total_time_spend,true),5);
		$newtimestamp = strtotime("$last_attempt_date - $total_time_spend second");
		$FromTime =  date('jS M Y h:i A', $newtimestamp);

		expDebug::dPrint("from time".print_r($FromTime,true),5);
		expDebug::dPrint("to time".print_r($toDate,true),5);
		$statusName = $result->statusName;


		if($assessment_count > 0 && $calltype == "'assessment'"){
			$score = $result->Score;
		}else if($assessment_count > 0 && ($calltype == null || $calltype == 'null')){
			$score = '';
		}else if($assessment_count == 0 && ($calltype == null || $calltype == 'null')){
			$score = $result->Score;
		}
		expDebug::dPrint("score calltype".print_r($calltype,true),5);
		expDebug::dPrint("score update".print_r($score,true),5);

		$fullName = $result->fullname;
		$email = $result->email;
		$base_url = "http://".$url;

		if($attempt_left == 1 || $max_attempts == 1){
			$attempt = 'attempt';
		}else{
			$attempt = 'attempts';
		}
		$versionNo = $result->version_no;

		//$actor will be commnon for all the statements
		$actor = array('name' => "$fullName",
				'objectType' => 'Agent',
				'mbox' => "mailto:" . $email
		);
	
		$statuses = array('completed', 'passed', 'failed');
		$content_status = strtolower(trim($lunObj->ContentStatus));
		$completion_status = strtolower(trim($lunObj->CompletionStatus));
		$sidval = "-".$lunObj->LessonId."-".$lunObj->VersionId."-".$lunObj->RegId."-".$lunObj->Location;
		expDebug::dPrint('aicc id'. $sidval, 5);
		$enc1 = new Encrypt();
		$aicc_id = $enc1->encrypt($sidval);
		expDebug::dPrint('aicc id ency'. $sidval);
		$content_result = array(
				'duration' => $lunObj->time_spent,
				'success' => in_array($content_status, $statuses),
				'completion' => in_array($completion_status, $statuses),
				'score' => array(
						'raw' => $lunObj->Score,
				),
		);
		$context = array(
				'course_name' => $classname,
				'course_id' => $lunObj->ClassId,
				'content_id' => $content_id,
				'content_name' => $content_code,
				'content_completed' => in_array($content_status, $statuses),
				'course_completed' => in_array($result[$key]->comp_status, array('lrn_tpm_ovr_cmp', 'lrn_crs_cmp_cmp')),
				'registration' => $aicc_id,
		);
		$statusDetails = array('actor' => $actor,
				'result' => $content_result,
				'context' => $context,
				'verb' => array('id' => $base_url,'display' => array("en-US"=> "$statusName"),),
				'object' => array('type' => 'course','definition' => array('name'=>array('en-US'=>"$classname")),)
				);
		$scoreDetails = array('actor' => $actor,
				'result' => $content_result,
				'context' => $context,
				'verb' => array('id' => $base_url,'display' => array("en-US"=> "scored"),),
				'object' => array('type' => 'course','definition' => array('name'=>array('en-US'=>"$score for $classname")),)
				);
		$validityDetails = array('actor' => $actor,
				'result' => $content_result,
				'context' => $context,
				'verb' => array('id' => $base_url,'display' => array("en-US"=> "attempted"),),
				'object' => array('type' => 'course','definition' => array('name'=>array('en-US'=>"$classname and their Validity is $validity_days $days")),)
		);
		$bookDetails = array('actor' => $actor,
				'result' => $content_result,
				'context' => $context,
				'verb' => array('id' => $base_url,'display' => array("en-US"=> "bookmarked"),),
				'object' => array('type' => 'course','definition' => array('name'=>array('en-US'=>"$classname")),)
				
		);
		$launchDateTime = array('actor' => $actor,
				'result' => $content_result,
				'context' => $context,
				'verb' => array('id' => $base_url,'display' => array("en-US"=> "attempted"),),
				'object' => array('type' => 'course', 'definition' => array('name'=>array('en-US'=>"$classname and their launch period is from $FromTime to $toDate")),)
		);
		$track = array('actor' => $actor,
				'result' => $content_result,
				'context' => $context,
				'verb' => array('id' => $base_url,	'display' => array("en-US"=> "attempted"),),
				'object' => array('type' => 'course',	'definition' => array('name'=>array('en-US'=>"$classname the $totalAttempts time out of their $max_attempts $attempt")),)
		);
		$launchDetails = array('actor' => $actor,
				'result' => $content_result,
				'context' => $context,
				'verb' => array('id' => $base_url,'display' => array("en-US"=> "attempted"),),
				'object' => array('type' => 'course','definition' => array('name'=>array('en-US'=>"$classname and has $attempt_left $attempt left")),)
		);
		$versionDetails = array('actor' => $actor,
				'result' => $content_result,
				'context' => $context,
				'verb' => array('id' => $base_url,'display' => array("en-US"=> "attempted"),),
				'object' => array('type' => 'course','definition' => array('name'=>array('en-US'=>"$classname and version number of the content is $versionNo")),)
		);

		
		$statusDetails = json_encode($statusDetails);
		$scoreDetails = json_encode($scoreDetails);
		$validityDetails = json_encode($validityDetails);
		$bookDetails = json_encode($bookDetails);
		$launchDateTime = json_encode($launchDateTime);
		$track = json_encode($track);
		$launchDetails = json_encode($launchDetails);
		$versionDetails = json_encode($versionDetails);

		expDebug::dPrint("launchDetails : ".print_r($launchDetails,true),5);

		/* if(empty($last_lesson_location) || $last_lesson_location == ''){
			$last_lesson_location = "null";
		} */
		/*expDebug::dPrint('$$statusDetails '.print_r($statusDetails, 1));
		 expDebug::dPrint('$$scoreDetails '.print_r($scoreDetails, 1));
		 expDebug::dPrint('$$validityDetails '.print_r($validityDetails, 1));
		 expDebug::dPrint('$$bookDetails '.print_r($bookDetails, 1));
		 expDebug::dPrint('$$launchDateTime '.print_r($launchDateTime, 1));
		 expDebug::dPrint('$$track '.print_r($track, 1));
		 expDebug::dPrint('$$launchDetails '.print_r($launchDetails, 1));
		 expDebug::dPrint('$$versionDetails '.print_r($versionDetails, 1));*/
		if(!empty($statusDetails) && $statusName == 'Completed' && ($lunObj->prevStatus != 'lrn_crs_cmp_cmp')){
			$api_client->create_statements(array('statements' => $statusDetails));
		}sleep(1);
		if((!empty($scoreDetails)) && ($statusName != 'Completed' || $score != 0) && ($result->comp_status == 'lrn_crs_cmp_cmp') && !empty($score) && ($lunObj->prevStatus != 'lrn_crs_cmp_cmp')){
			$api_client->create_statements(array('statements' => $scoreDetails));
		}sleep(1);if(!empty($validityDetails) && $validity_days > 0 && ($lunObj->prevStatus != 'lrn_crs_cmp_cmp' && $lunObj->prevStatus != 'lrn_tpm_ovr_cmp') && ($total_attempts == 1 || empty($total_attempts))){
			$api_client->create_statements(array('statements' => $validityDetails));
		}sleep(1);if((!empty($bookDetails)) && ($last_lesson_location)){
			$api_client->create_statements(array('statements' => $bookDetails));
		}sleep(1);if(!empty($launchDateTime) && $attempt_left >= 0){
			$api_client->create_statements(array('statements' => $launchDateTime));
		}sleep(1);if(!empty($track) && $attempt_left >= 0){
			$api_client->create_statements(array('statements' => $track));
		}sleep(1);if(!empty($launchDetails) && $attempt_left >= 0){
			$api_client->create_statements(array('statements' => $launchDetails));
		}sleep(1);if(!empty($versionDetails) && $versionNo >= 0){
			$api_client->create_statements(array('statements' => $versionDetails));
		}

		$dao->closeconnect();
		//$dao1->closeconnect();
	  }
	}catch(Exception $e){
		$dao->rollbackTrans();
		$dao->closeconnect();
		expDebug::dPrint("tincanCreateStatement_Tincan".$e->getMessage(),1);
	}
}

function updateAiccPreview($session_id){
	$aicc_sid = trim($aicc_sid);
	try{
		//73059: Content completion % is not getting updated unless we close the window and reopen it or move to the next content
		if(isset($_POST['launchflag']) && $_POST['launchflag']==0){
			$dao=new AbstractDAO();
			$dao->connect();
			$queryDel = "DELETE FROM slt_aicc_interaction WHERE session_id='".$session_id."'";
			expDebug::dPrint("Tincan Handler::updateAiccPreview DELETE query = " . $queryDel,2);
			$dao->query($queryDel);
			$dao->closeconnect();
		}
	}catch(Exception $e){
		$dao->rollbackTrans();
		$dao->closeconnect();
		expDebug::dPrint("Tincan Handler::updateAiccPreview Error :".$e->getMessage(),1);
	}
}

function getAttemptCountTincan($versionId,$classId,$userId){
	expDebug::dPrint("Get Count".$dao);
	$dao=new AbstractDAO();
	$dao->connect();
	$dao->query("select sum(total_attempts) as attemptCount from slt_attendance_summary where content_version_id=$versionId and user_id=$userId and class_id=$classId");

	$result = $dao->fetchResult();
	expDebug::dPrint("Result Fetch ".print_r($result,true),5);
	$attemptCount = $result->attemptCount;
	expDebug::dPrint("Attempt Count ".$attemptCount);
	return $attemptCount;
}
function getContentInfoTincan($versionId){
	try {
		expDebug::dPrint("Get Count".$dao);
		$dao=new AbstractDAO();
		$dao->connect();
		$dao->query("select content_master_id as content_id, mst.code as content_code
						from slt_content_version ver
						join slt_content_master mst on mst.id = ver.content_master_id
						where ver.id = $versionId");
		$contentInfo = $dao->fetchResult();
		expDebug::dPrint("Result Fetch ".print_r($contentInfo,true),5);
// 		$contentId = $contentInfo->content_id;
		expDebug::dPrint("getContentInfoTincan results".print_r($contentInfo, 1));
		return $contentInfo;
	} catch (Exception $ex) {
		watchdog_exception('getContentInfoTincan', $ex);
		expertusErrorThrow($ex);
	}
}
function getCourseClassInfoTincan($classId, $userId, $lessonId, $versionId, $regId, $contentId){
	try {
		$dao=new AbstractDAO();
		$dao->connect();
		$selectClass = "SELECT cls.title AS class_name, enr.score AS Score, per.email AS email,per.full_name as fullname,crs.title AS course_name, map.max_attempts AS max_attempts,map.validity_days AS validity_days,
				summ.last_lesson_location AS last_lesson_location,summ.aicc_data AS aicc_data, pro.name AS statusName, enr.comp_status AS comp_status,
				DATE_FORMAT((summ.last_attempt_date),'%d %b %Y %h:%i %p') AS last_attempt_date, vers.version AS version_no
				FROM
				slt_course_class cls
				LEFT OUTER JOIN slt_course_template crs ON crs.id=cls.course_id
				LEFT OUTER JOIN slt_enrollment enr ON enr.class_id=cls.id and enr.course_id=cls.course_id and enr.user_id=$userId
				LEFT OUTER JOIN slt_person per ON per.id = enr.user_id
				LEFT OUTER JOIN slt_course_content_mapper map ON map.class_id=cls.id and map.course_id=crs.id and map.content_id = $contentId
				LEFT OUTER JOIN slt_content_version vers ON vers.content_master_id = map.content_id and vers.id = $versionId
				LEFT OUTER JOIN slt_attendance_summary summ ON summ.class_id=cls.id and summ.course_id=crs.id and summ.content_version_id = $versionId AND (summ.user_id = $userId) AND (summ.lesson_id = $lessonId)
				AND (summ.content_version_id = $versionId) AND  (summ.enrollment_id = $regId)
				LEFT OUTER JOIN slt_profile_list_items pro ON pro.code=enr.comp_status
				WHERE (cls.id = $classId)
				AND enr.reg_status = 'lrn_crs_reg_cnf' GROUP BY cls.id";
		expDebug::dPrint("getContentInfoTincan query -> $selectClass", 5);
		$dao->query($selectClass);
		$classInfo = $dao->fetchResult();
		return $classInfo;
	} catch (Exception $ex) {
		watchdog_exception('getContentInfoTincan', $ex);
		expertusErrorThrow($ex);
	}
}
function getAssessmentCount_tinCan($classID,$type){
	try {
		$dao=new AbstractDAO();
		$dao->connect();
		$dao->query("SELECT COUNT(1) AS assessment_count FROM slt_survey_mapping surmap WHERE  (surmap.object_id = $classID) AND (surmap.object_type = '$type') AND (surmap.pre_status = 0)");
		$result = $dao->fetchResult();
		return $result;
	} catch (Exception $ex) {
		watchdog_exception('getAssessmentCount_tinCan', $ex);
		expertusErrorThrow($ex);
	}
}

/* 0049061: Some of the content statements are not getting captured in the LRS and need to be captured 
 * store what are all information comes from tincan api is stored into LRS .
 */
function storeTincanDataIntoLRS($aicc_sid,$session_id,$vUserId,$jsonData, $status){
	try {
		expDebug::dprint("storeTincanDataIntoLRS function : jsonData".print_r($jsonData, true), 4);
		expDebug::dprint("storeTincanDataIntoLRS function : aicc_sid".print_r($aicc_sid, true), 4);
		expDebug::dprint("storeTincanDataIntoLRS function : session_id".print_r($session_id, true), 4);
		expDebug::dprint("storeTincanDataIntoLRS function : vUserId".print_r($vUserId, true), 4);

		$lrs_data_array = array();
		if(!empty($jsonData)) {
				$dao=new AbstractDAO();
				$dao->connect();
				$dao->query("SELECT full_name as name, email as email FROM slt_person WHERE id=trim($vUserId)");
				$result = $dao->fetchResult();
				expDebug::dprint("storeTincanDataIntoLRS function : person details".print_r($result, true), 4);
				$static_array_value = array(name => $result->name, mbox => "mailto:" . $result->email, objectType =>"Agent");
				$jsonData->context->registration = $session_id;
				expDebug::dprint("storeTincanDataIntoLRS function aicc_id".print_r($aicc_sid, true), 4);
				$context_info = prepareContextFromAiccId($aicc_sid, $status);
				expDebug::dPrint('type of $jsonData'.gettype($jsonData), 5);
				if(is_array($jsonData)) {
					foreach ($jsonData as &$statment) {
						$statment->context->course_name = $context_info['course_name'];
						$statment->context->course_id = $context_info['course_id'];
						$statment->context->content_id = $context_info['content_id'];
						$statment->context->content_name = $context_info['content_name'];
						$statment->context->course_completed = $context_info['course_completed'];
						$statment->context->content_completed = $context_info['content_completed'];
						$statment->actor = $static_array_value;
					}
					
				} else {
					$jsonData->context->course_name = $context_info['course_name'];
					$jsonData->context->course_id = $context_info['course_id'];
					$jsonData->context->content_id = $context_info['content_id'];
					$jsonData->context->content_name = $context_info['content_name'];
					$jsonData->context->course_completed = $context_info['course_completed'];
					$jsonData->context->content_completed = $context_info['content_completed'];
					$jsonData->actor = $static_array_value;
				}
				expDebug::dprint("storeTincanDataIntoLRS function : jsonData details".print_r($jsonData, true), 4);
				$api_client = TinCanClient::Instance();
				$json_Details = json_encode($jsonData);
				if(!empty($json_Details)){
					$api_client->create_statements(array('statements' => $json_Details));
				}sleep(1);
				$status_array = explode('verbs/',$jsonData->verb->id);
				audit_trail_insert($status_array[1],$status_array[1],$aicc_sid,$vUserId);
			}
	} catch (Exception $ex) {
		watchdog_exception('storeTincanDataIntoLRS', $ex);
		expertusErrorThrow($ex);
	}
}

function audit_trail_insert($status,$xstaus_old,$aicc_sid,$vUserId){
	$xstaus_new='';
	if($status=="Completed"){
		$xstaus_new='lrn_crs_cmp_cmp';
		$funcname='classcompleted';
	}else if($status=="Incomplete"){
		$xstaus_new='lrn_crs_cmp_inc';
		$funcname='classincomplete';
	}else if($status =='attempted') {
		$xstaus_old = 'Launched';
		$xstaus_new='Launched';
		$funcname='classincomplete';
	}else if($status =='answered') {
		$xstaus_new='Answered';
		$xstaus_old = 'Answered';
		$funcname='classincomplete';
	}else if($status =='passed') {
		$xstaus_new='Passed';
		$xstaus_old='Passed';
		$funcname='classincomplete';
	}
	if($xstaus_new!=''){
		$tsid = explode('-',$aicc_sid);
		$dao1=new AbstractDAO();
		$dao1->connect();
		$dao1->query("select time_zone as xtimezone from slt_person where id=". trim($vUserId).";");
		$result = $dao1->fetchResult();
		$dao1->closeconnect();
		$xtimezone= $result->xtimezone;
		$qry4 = "INSERT INTO slt_audit_trail (
		logged_user_id,
		entity_id,
		entity_type,
		module_name,
		functionality_name,
		logged_user_action,
		mod_user_id,
		old_value,
		new_value,
		esign_date_time,
		timezone,
		created_on,
		custom0,
		custom1,
		custom2,
		custom3,
		custom4)
		VALUES
		('".trim($vUserId)."',".
		"'".trim($tsid[2])."',".
		"'cre_sys_obt_cls',".
		"'exp_sp_launch'".",".
		"'".$funcname."',".
		"'updated enrollment status'".",".
		"'".trim($tsid[0])."',".
		"'".$xstaus_old."',".
		"'".$xstaus_new."',".
		"now(),".
		"'".$xtimezone."',".
		"now(),".
		"null,null,null,null,null);";
		$dao=new AbstractDAO();
		$dao->connect();
		$dao->beginTrans();
		$res1 = $dao->execute($qry4);
		expDebug::dPrint("AICC Handler::Audit trail Update :".$qry4,2);
		$dao->commitTrans();
		$dao->closeconnect();
	}
}
function insertUserPointsForEachActionsPerformedTincan($userId,$code,$entityId = 0,$operationCode = '',$entityType = 'others') {
	if($operationCode == '' || $operationCode == 'insert') {
		$dao=new AbstractDAO();
		$dao->connect();
		$dao->query("select points as upoints from slt_master_points where code like '%".$code."%'");
		$result = $dao->fetchResult();
		$dao->closeconnect();
		$userPoints= $result->upoints;
		$insertSql = "INSERT INTO slt_user_points (
		user_id,
		entity_id,
		entity_type,
		action_code,
		operation_flag,
		earned_points,
		total_points,
		created_by,
		created_on,
		updated_by,
		updated_on
		)
		VALUES
		('".trim($userId)."',".
		"'".trim($entityId)."',".
		"'".$entityType."',".
		"'".$code."',".			
		"'insert'".",".
		"'".$userPoints."',".
		"'".$userPoints."',".
		"'".$userId."',".
		"now(),".
		"'".$userId."',".
		"now());";
		$dao=new AbstractDAO();
		$dao->connect();
		$dao->beginTrans();
		$res1 = $dao->execute($insertSql);
		expDebug::dPrint("Tincan Handler::Audit trail Update :".$insertSql,2);
		$dao->commitTrans();
		$dao->closeconnect();
	}
	else if($operationCode == 'delete'){
		$dao=new AbstractDAO();
		$dao->connect();
		$qry = "select id from slt_user_points where action_code = '$code' AND entity_id=$entityId AND user_id=$userId AND operation_flag='insert'";
		//$dao->query("select id from slt_user_points where action_code = '$code' AND entity_id='$entityId' AND user_id='$userId' AND operation_flag='insert'");
		if($entityType != 'others') {

			$qry .= "AND entity_type='$entityType'";
		}
		$dao->query($qry);
		$result = $dao->fetchResult();
		$dao->closeconnect();
		$userPoints= $result->id;
		$update = "UPDATE slt_user_points SET operation_flag='delete',total_points=0,updated_by=$userId,updated_on=now() WHERE action_code='$code' AND entity_id=$entityId AND user_id=$userId AND operation_flag='insert'";
		if($entityType != 'others') {
			$update .= "AND entity_type='$entityType' ";
		}
		$dao=new AbstractDAO();
		$dao->connect();
		expDebug::dPrint("Tincan Handler::captureData - Insert/Update Query :".$update,2);
		$res = $dao->execute($update);
		$dao->closeconnect();
	}
}

function prepareContextFromAiccId($aicc_id, $compStatus) {
	try {
		//data.learnerId+"-"+data.courseid+"-"+data.classid+"-"+data.lessonid+"-"+data.versionid+"-"+data.enrollid+"-"+data.location;
		expDebug::dPrint('$aicc_id_split '.print_r($aicc_id, 1));
		$aicc_id_split = explode('-', $aicc_id);
		expDebug::dPrint('$aicc_id_split '.print_r($aicc_id_split, 1));
		$content_info = getContentInfoTincan($aicc_id_split[4]);
		$content_id = $content_info->content_id;
		$content_code = $content_info->content_code;
		$classInfo = getCourseClassInfoTincan($aicc_id_split[2], $aicc_id_split[0], $aicc_id_split[3], $aicc_id_split[4], $aicc_id_split[5], $content_id);
		expDebug::dPrint("prepareContextFromAiccId classInfo :".print_r($classInfo, 1), 5);
		$context = array(
				'course_name' => $classInfo->class_name,			//mandatory param, refers to Class Name/Programe Name in ExpertusONE
				'course_id' => $aicc_id_split[2],			//mandatory param, refers to Class Id in ExpertusONE
				'content_id' => $content_id,			//mandatory param, refers to Content Id in ExpertusONE
				'content_name' => $content_code,
				'content_completed' => in_array($compStatus['status'], array('completed', 'passed')),	//optional , shoule be sent on completion
				'course_completed' => in_array($classInfo->comp_status, array('lrn_tpm_ovr_cmp', 'lrn_crs_cmp_cmp')),
				'registration' => $aicc_id,	//aicc id
		);
		expDebug::dPrint("prepareContextFromAiccId context :".var_export($context, 1), 5);
		return $context;
	}catch(Exception $e){
		$dao->closeconnect();
		expDebug::dPrint("prepareContextFromAiccId :".$e->getMessage());
	}
}
?>