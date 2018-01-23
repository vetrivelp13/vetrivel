<?php
define('DRUPAL_ROOT', $_SERVER['DOCUMENT_ROOT']);
require_once "sites/all/services/GlobalUtil.php";
require_once "sites/all/services/Trace.php";
require_once "sites/all/dao/AbstractDAO.php";
require_once "sites/all/services/Encryption.php";
require_once "sites/all/modules/core/exp_sp_core/exp_sp_core.module";
require_once $_SERVER["DOCUMENT_ROOT"].'/apis/oauth2/Server.php';
require_once $_SERVER["DOCUMENT_ROOT"].'/apis/oauth2/Token.php';
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
require_once DRUPAL_ROOT . '/includes/entity.inc';
require_once DRUPAL_ROOT . '/modules/user/user.module';
require_once "twilio-php/Services/Twilio.php";
drupal_bootstrap(DRUPAL_BOOTSTRAP_SESSION);
include_once DRUPAL_ROOT .'/apis/expertusone_oauth/config.php';
$currenttime = time ();
expDebug::dPrint(date("m/d/y H:i:s",$currenttime)." ". "Log Started\n",4);
foreach ($_POST as $key => $value)
{
	expDebug::dPrint("AICC POST KEY - ".$key."  - Value  - ".$value,4);
	$tempkey = strtolower($key);
	$_POST[$tempkey] = $value;
}


$msgtext = trim($_POST['Body']," ");
$from    = trim($_POST['From']," ");
$error ='';

if(strpos($msgtext,'Sent from your Twilio trial account -') !== false){
$msgtext = substr($msgtext,38);
expDebug::dPrint($msgtext,5);
}

$key ='';
$entity = '';
$userid = '';
$msgends = checkSmstemplate($msgtext);
expDebug::dPrint('msgends' . $msgends, 4); 
if($msgends){
    $msg = getFullMsg();
    expDebug::dPrint('Full message is' .$msg); 
    $keyword = explode(" ",$msg);
    $key = trim(strtolower($keyword[0])," ");
	$entity = trim(strtolower($keyword[1])," ");
    $msgtext = formatMsg($msg,$entity);
    $userid = validateUser();
	if(!$userid){
		$msg = "User Authentication Failed";
 		sendReply($msg);
 		echo $msg;
 		return;
	}
}else{
	saveMsg();
	return;
}

//for testing
//$userid =1 ;

if($key === "create" ){
	switch ($entity) {
    	case "crs":
    		 createCourse($msgtext);
       		 break;
    	case "wbtcls":
        	createClass('wbt',$msgtext);
        	break;
        case "vodcls":
        	createClass('vod',$msgtext);
        	break;
        case "iltcls":
        	 createClass('ilt',$msgtext);
        	break;
        case "vccls":
        	 createClass('vc',$msgtext);
        	break;
    	case "usr":
        	createUser($msgtext);
        	break;
    	case "org":
        	createOrganization($msgtext);
        	break;
    	default:
       		  expDebug::dPrint('Invalid Entity Type' , 4); 
       		  sendReply("Invalid Entity Type");
			  echo "Invalid Entity Type";
			  return;

	}
}

function createCourse($params)
{
	expDebug::dPrint('Inside Create Course' , 4); 
//	$params = $GLOBALS['hints_array'];
	expDebug::dPrint('hints_array' . print_r($params, true) , 4); 
    $mandatoryFieldMissing = isMandatoryFieldsExists('crs',$params);
    if($mandatoryFieldMissing)
    {
		$error = "Mandatory fields Missing - ".$mandatoryFieldMissing;
		sendReply($error);
		return ;
    }

	$crs_title = $params['title'];
	$crs_code  = $params['code'];
	$crs_desc  = $params['description'];
	$crs_language = $params['language'];
	$status = $params['status'];
	$author = $params['author'];
	$compliance = strtolower(trim($params['compliance']," "));
	$completeby = $params['completeby'];
	$validity = $params['validity'];
	$currency_type = trim($params['currencytype']," ");
	$price = trim($params['price']," ");
	
	$complete_days = '';
	$complete_date = '';
	$validity_days = '';
	$validity_date = '';
	
	if($compliance === true || $compliance === "true" || $compliance === "yes"){
	$compliance = 1;
	}else{
	$compliance = 0;
	}
	
	if(!empty($currency_type)){
	
		$pdao=new AbstractDAO();
	    $pdao->connect();
	    $qry = "select code from slt_profile_list_items where attr1='".$currency_type."' and is_active = 'Y'";
	    expDebug::dPrint("currency code query".$qry,2);
		$res = $pdao->query($qry);
		$param=$pdao->fetchResult();
		$pdao->closeconnect();
		expDebug::dPrint("currency code".$param->code);
		$currency_type = $param->code;
	
	
	if(empty($currency_type)){
		$error = "Invalid Currency Type"  ;
		sendReply($error);
		return ;
	}
	}
	

    if(!empty($completeby))
    {
    if (preg_match("/^(0[1-9]|[1-2][0-9]|3[0-1])-(0[1-9]|1[0-2])-[0-9]{4}$/",trim($completeby," ")))
    {
    	$completeby = trim($completeby," ");
    	$time = strtotime($completeby);
        $cdate = date('m-d-Y',$time);
		$currentdate = date("m-d-Y");
		
		if($currentdate <= $cdate){
		$complete_date = $cdate;
		}else{
			$msg = "completeby date is invalid. Date format is dd-mm-yyyy";
			sendReply($msg);
			return;
		}
    }else if(strpos($completeby,'-') === false) {
    	$complete_days = trim($completeby," ");
    }else{
    	$msg = "completeby date is invalid. Date format is dd-mm-yyyy";
		sendReply($msg);
		return;
    }
    }
    
    if(!empty($validity))
    {
    if (preg_match("/^(0[1-9]|[1-2][0-9]|3[0-1])-(0[1-9]|1[0-2])-[0-9]{4}$/",trim($validity," ")))
    {	
    
    	$validity = trim($validity," ");
    	$time = strtotime($validity);
        $vdate = date('m-d-Y',$time);
		$currentdate = date("m-d-Y");
		
		if($currentdate <= $vdate){
		$validity_date = $vdate;
		}else{
			$msg = "validity date date is invalid. Date format is dd-mm-yyyy";
			sendReply($msg);
			return;
		}

    }else if(strpos($validity,'-') === false) {
    	$validity_days = trim($validity," ");
    }else{
		$msg = "validity date date is invalid. Date format is dd-mm-yyyy";
		sendReply($msg);
		return;
    }
    }

	switch(trim(strtolower($crs_language)," ")){
	    case "english":
			$crs_language = "cre_sys_lng_eng"; 
			break;
    	case "chinese":
			$crs_language = "cre_sys_lng_gzh"; 
			break;
        case "japanese":
			$crs_language = "cre_sys_lng_jap"; 
			break;
        case "spanish":
			$crs_language = "cre_sys_lng_esp"; 
			break;
        case "german":
			$crs_language = "cre_sys_lng_den"; 
			break;
    	case "french":
			$crs_language = "cre_sys_lng_frh"; 
			break;
    	case "italian":
			$crs_language = "cre_sys_lng_itn"; 
			break;
        case "korean":
			$crs_language = "cre_sys_lng_kon"; 
			break;
        case "russian":
			$crs_language = "cre_sys_lng_rus"; 
			break;
        case "portuguese":
			$crs_language = "cre_sys_lng_pts"; 
			break;
    	default:
       		 // print_r("Invalid Language");
       		  $crs_language = "cre_sys_lng_eng"; 
	}
	expDebug::dPrint('crs_language' . print_r($crs_language, true) , 4); 
	
	
	if(strtolower(trim($status," ")) === "active")
	{
		$status = 'lrn_crs_sts_atv';
	}else{
		$status = 'lrn_crs_sts_itv';
	}


	$requestdata = array("crs_title"=> $crs_title,
			  'crs_code'=> $crs_code,
			  'value'=> $crs_desc,
			  'crs_status'=>$status,
			  'crs_language'=>$crs_language,
			  'crs_author_vendor'=>$author,
			  'course_compliance'=>$compliance,
			  'completed_days'=>$complete_days,
			  'completed_date'=>$complete_date,
			  'validity_days'=>$validity_days,
			  'validity_date'=>$validity_date,
			  'currency_type'=>$currency_type,
			  'price'=>$price,
			  'apiname'=>'CreateCourseAPI',
			  'userid'=>$GLOBALS['userid']
							);
	$response = sendApirequest($requestdata);
	$id = $response['results']['jsonResponse'][0]['id'];
	expDebug::dPrint("responseeeee".$id);
	$msg ='';
	if(is_numeric($id))
	{
	$msg = 'Course Created ';
	expDebug::dPrint("is number");
	}else{
	$msg = 'Course creation has failed.'.$id;
	}

	//print_r("responseeeee".$response['results']['jsonResponse'][0]['id']);
	sendReply($msg);
}


function createClass($cls_deliverytype,$params){

	expDebug::dPrint('Inside Create class' , 4); 
//	$params = $GLOBALS['hints_array'];
	expDebug::dPrint('hints_array' . print_r($params, true) , 4); 
	
    $mandatoryFieldMissing = isMandatoryFieldsExists($cls_deliverytype,$params);
    if($mandatoryFieldMissing)
    {
		$error = "Mandatory fields Missing - ".$mandatoryFieldMissing;
		sendReply($error);
		return ;
    }

	//$crs_id = $params[1];
	$crs_code = trim(strtolower($params['coursecode'])," ");
	$cls_title = $params['title'];
	$cls_code  = $params['classcode'];
	$cls_desc  = $params['description'];
	$cls_language = $params['language'];
	$currency_type = trim($params['currencytype']," ");
	$price = trim($params['price']," ");
	$max_seats = '';
	$cls_location = '';
	$cntid = '';
	$waitlist_count = ''; 
	
	if(!empty($currency_type)){
	
		$pdao=new AbstractDAO();
	    $pdao->connect();
	    $qry = "select code from slt_profile_list_items where attr1='".$currency_type."' and is_active = 'Y'";
	    expDebug::dPrint("currency code query".$qry,2);
		$res = $pdao->query($qry);
		$param=$pdao->fetchResult();
		$pdao->closeconnect();
		expDebug::dPrint("currency code".$param->code);
		$currency_type = $param->code;
	
	
	if(empty($currency_type)){
		$error = "Invalid Currency Type"  ;
		sendReply($error);
		return ;
	}
	}
	
	
		$pdao=new AbstractDAO();
	    $pdao->connect();
	    $qry = "SELECT id FROM slt_course_template WHERE code='".$crs_code."'";
	    expDebug::dPrint("course code query".$qry,2);
		$res = $pdao->query($qry);
		$param=$pdao->fetchResult();
		$pdao->closeconnect();
		expDebug::dPrint("course id".$param->id);
		$crs_id =$param->id;
		expDebug::dPrint("course id is".$crs_id);
		
	if(empty($crs_id)){
		$error = "Course code doesn't exists"  ;
		sendReply($error);
		return ;
	}	
		

	switch(trim(strtolower($cls_language)," ")){
	    case "english":
			$cls_language = "cre_sys_lng_eng"; 
			break;
    	case "chinese":
			$cls_language = "cre_sys_lng_gzh"; 
			break;
        case "japanese":
			$cls_language = "cre_sys_lng_jap"; 
			break;
        case "spanish":
			$cls_language = "cre_sys_lng_esp"; 
			break;
        case "german":
			$cls_language = "cre_sys_lng_den"; 
			break;
    	case "french":
			$cls_language = "cre_sys_lng_frh"; 
			break;
    	case "italian":
			$cls_language = "cre_sys_lng_itn"; 
			break;
        case "korean":
			$cls_language = "cre_sys_lng_kon"; 
			break;
        case "russian":
			$cls_language = "cre_sys_lng_rus"; 
			break;
        case "portuguese":
			$cls_language = "cre_sys_lng_pts"; 
			break;
    	default:
       		 // print_r("Invalid Language");
       		  $cls_language = "cre_sys_lng_eng"; 

       		  
	}
	
	expDebug::dPrint('cls_language' . print_r($cls_language, true) , 4); 

	
	switch(trim(strtolower($cls_deliverytype))){
	    case "wbt":
    		 $cls_deliverytype = 'lrn_cls_dty_wbt';
       		 break;
    	case "vod":
        	$cls_deliverytype = 'lrn_cls_dty_vod';
        	break;
    	case "ilt":
        	$cls_deliverytype = 'lrn_cls_dty_ilt';
        	$max_seats = trim($params['maxseats']," ")  ; 
        	$cls_location = getlocationid(trim($params['location']," ")); 
        	$waitlist_count = trim($params['waitlistcount']," ");  	
        	break;
    	case "vc":
			$cls_deliverytype = 'lrn_cls_dty_vcl';
			$max_seats = trim($params['maxseats']," ")  ; 
			$waitlist_count = trim($params['waitlistcount']," ");    	
        	break;
    	default:
       		  print_r("Invalid Delivery Type");
       		  expDebug::dPrint('Invalid Delivery Type' , 4); 
       		  sendReply("Invalid Delivery Type");
			  return ;
	}


	if($cls_deliverytype =='lrn_cls_dty_wbt' || $cls_deliverytype =='lrn_cls_dty_vod')
	{
		expDebug::dPrint("Inside content check");
		$cnt_name = trim(strtolower($params['contentname'])," ");
		$attempts = trim($params['attempts']," ");
		$validity = trim($params['validity']," ");
		expDebug::dPrint("content code query".$cnt_name,2);
		
		$pdao=new AbstractDAO();
	    $pdao->connect();
	    $qry = "SELECT id FROM slt_content_master WHERE code='".$cnt_name."'";
	    expDebug::dPrint("content code query".$qry,2);
		$res = $pdao->query($qry);
		$param=$pdao->fetchResult();
		$pdao->closeconnect();
		expDebug::dPrint("content id".$param->id);
		$cntid =$param->id;
		expDebug::dPrint("content id is ".$cntid);
		
		if(!is_numeric($cntid))
		{
			$msg = 'Invalid content name';
			sendReply($msg);
			return ;
		}		

	}

     
	$requestdata = array("course_id"=> $crs_id,
			  'title'=> $cls_title,
			  'code'=> $cls_code,
			  'value'=>$cls_desc,
			  'status'=>'lrn_cls_sts_itv',
			  'lang_code'=>$cls_language,
			  'delivery_type'=>$cls_deliverytype,
			  'currency_type'=>$currency_type,
			  'price'=>$price,
			  'max_seats'=>$max_seats,
			  'class_location'=>$cls_location,
			  'waitlist_count'=>$waitlist_count,
			  'apiname'=>'CreateClassAPI',
			  'userid'=>$GLOBALS['userid']
							);
							
							
	$response = sendApirequest($requestdata);
	$id = $response['results']['jsonResponse'][0]['id'];
	expDebug::dPrint("responseeeee".$id);
	if(!is_numeric($id))
	{
		$msg = $id;
		sendReply($msg);
		return;
	}
	
	expDebug::dPrint("responseeeee tttt".$cls_deliverytype);
	
	if($cls_deliverytype =='lrn_cls_dty_wbt' || $cls_deliverytype =='lrn_cls_dty_vod'){
		$cnt_id_st = addContentToVodWbt($crs_id,$id,$cntid,$attempts,$validity);
		//print_r('content status'.$cnt_id_st);
	}else if($cls_deliverytype =='lrn_cls_dty_ilt'){
		$session_name = trim($params['sessionname']," ");
		$instructor_name = trim($params['instructorname']," ");
		$start_date = trim($params['startdate']," ");
		$start_hours = trim($params['starttime']," ");
		$end_hours = trim($params['endtime']," ");
	 	$session_id = createILTSession($crs_id,$id,$instructor_name,$session_name,$start_date,$start_hours,$end_hours);
	 	//print_r('session_id'.$session_id);
	 		if(!is_numeric($session_id))
			{
				$msg = $session_id;
				sendReply($msg);
				return;
			}
	}else if($cls_deliverytype =='lrn_cls_dty_vcl'){
		$session_name = trim($params['sessionname']," ");
		$instructor_name = trim($params['instructorname']," ");
		$start_date = trim($params['startdate']," ");
		$start_hours = trim($params['starttime']," ");
		$end_hours = trim($params['endtime']," ");
		$timezone = trim($params['timezone']," ");
		$meeting_type = trim($params['meetingtype']," ");
		$meeting_id='';
		$attendee_pwd='';
		$presenter_pwd='';
		$attendee_url='';
		$presenter_url='';
		$presenter_name = trim($params['presentername']," ");;
		if(strtolower($meeting_type) == 'expertusmeeting'){
			$meeting_type = 'lrn_cls_vct_exp';
		}else if(strtolower($meeting_type) == 'livemeeting'){
			$meeting_type = 'lrn_cls_vct_web';
			$meeting_id=trim($params['meetingid']," ");
			$attendee_pwd=trim($params['attendeepassword']," ");
			$presenter_pwd=trim($params['presenterpassword']," ");
		}else if(strtolower($meeting_type) == 'othermeeting'){
			$meeting_type = 'lrn_cls_vct_oth';
			$attendee_url=trim($params['attendeeurl']," ");;
			$presenter_url=trim($params['presenterurl']," ");;
		}else{
			$msg = "Invalid Meeting Type";
			sendReply($msg);
			return;
		}
	 	$session_id = createVCSession($crs_id,$id,$instructor_name,$session_name,$start_date,$start_hours,$end_hours,$meeting_type,$timezone,$meeting_id,$attendee_pwd,$presenter_pwd,$attendee_url,$presenter_url,$presenter_name);
	 	print_r('session_id'.$session_id);
	 		if(!is_numeric($session_id))
			{
				$msg = $session_id;
				sendReply($msg);
				return;
			}
	}
	//publishCourse($crs_id);

	if(strtolower(trim($params['status']," ")) === "active")
	{
	//print_r("success");
	publishCourse($crs_id);
	publishClass($id);
	}
		
	expDebug::dPrint(is_numeric($id));
	$msg ='';
	if(is_numeric($id))
	{
	$msg = 'Class Created ';
	expDebug::dPrint("is number");
	}else{
	$msg = 'Class creation has failed. '.$id;
	}

	//print_r("responseeeee".$response['results']['jsonResponse'][0]['id']);
	sendReply($msg);


}
function publishCourse($id){

		$pdao=new AbstractDAO();
	    $pdao->connect();
	    $qry = "SELECT * FROM slt_course_template WHERE id='".$id."'";
	    expDebug::dPrint("course details query".$qry,2);
		$res = $pdao->query($qry);
		$param=$pdao->fetchResult();
		$pdao->closeconnect();
		
		
		
			$requestdata = array('crs_id'=>$param->id,
			  "crs_title"=> $param->title,
			  'crs_code'=> $param->code,
			  'value'=> $param->short_description,
			  'crs_status'=>'lrn_crs_sts_atv',
			  'crs_language'=>$param->lang_code,
			  'crs_author_vendor'=>$param->author_vendor,
			  'course_compliance'=>$param->is_compliance,
			  'completed_days'=>$param->complete_days,
			  'completed_date'=>$param->complete_date,
			  'validity_days'=>$param->validity_days,
			  'validity_date'=>$param->validity_date,
			  'apiname'=>'UpdateCourseAPI',
			  'userid'=>$GLOBALS['userid']
							);

       $response = sendApirequest($requestdata);

		
}


function publishClass($id){

		$pdao=new AbstractDAO();
	    $pdao->connect();
	    $qry = "SELECT * FROM slt_course_class WHERE id='".$id."'";
	    expDebug::dPrint("class details query".$qry,2);
		$res = $pdao->query($qry);
		$param=$pdao->fetchResult();
		$pdao->closeconnect();
		
		$location = '';
		if($param->location_id){
			$pdao=new AbstractDAO();
	    	$pdao->connect();
	    	$qry = "SELECT name FROM slt_location WHERE id='".$param->location_id."'";
	    	expDebug::dPrint("location id  query".$qry,2);
			$res = $pdao->query($qry);
			$location=$pdao->fetchResult();
			$pdao->closeconnect();
		}
		$currency_type ='';
		if(!empty($param->currency_type)){
		
			$pdao=new AbstractDAO();
			$pdao->connect();
			$qry = "select code from slt_profile_list_items where attr1='".$param->currency_type."' and is_active = 'Y'";
			expDebug::dPrint("currency code query".$qry,2);
			$res = $pdao->query($qry);
			$cur_type=$pdao->fetchResult();
			$pdao->closeconnect();
			expDebug::dPrint("currency code".$cur_type->code);
			$currency_type = $cur_type->code;
		
		}
		
		
		
	$requestdata = array('id'=>$param->id,
				"course_id"=> $param->course_id,
			  'title'=> $param->title,
			  'code'=>$param->code,
			  'value'=>$param->short_description,
			  'status'=>'lrn_cls_sts_atv',
			  'lang_code'=>$param->lang_code,
			  'delivery_type'=>$param->delivery_type,
			  'currency_type'=>$currency_type,
			  'price'=>$param->price,
			  'max_seats'=>$param->max_seats,
			  'class_location'=>$param->location_id,
			  'waitlist_count'=>$param->waitlist_count,
			  'apiname'=>'UpdateClassAPI',
			  'userid'=>$GLOBALS['userid']
							);
							
       	$response = sendApirequest($requestdata);
}


function createUser($params)
{
	expDebug::dPrint('Inside Create User' , 4);
// 	$params = $GLOBALS['hints_array'];
	expDebug::dPrint('hints_array' . print_r($params, true) , 4); 


    $mandatoryFieldMissing = isMandatoryFieldsExists('usr',$params);
    if($mandatoryFieldMissing)
    {
		$error = "Mandatory fields Missing - ".$mandatoryFieldMissing;
		sendReply($error);
		return ;
    }

	$fname = trim($params['firstname']," ");
	$lname = trim($params['lastname']," ");
	$uname = trim($params['username']," ");
	$pass  = trim($params['password']," ");
	$email = trim($params['email']," ");
	$language = trim($params['language']," ");
	$organization = trim($params['organization']," ");
	$manager = trim($params['manager']," ");
	$roles = trim($params['roles']," ");
	$emp_type = trim($params['employeetype']," ");
	$emp_id = trim($params['employeeid']," ");
	$dept = trim($params['department']," ");
	$User_Type = trim($params['userttype']," ");
	$job_role = trim($params['jobrole']," ");
	$job_title = trim($params['jobtitle']," ");
	
	$emp_type = getValueFromProfileList($emp_type);
	$dept = getValueFromProfileList($dept);
	$job_title = getValueFromProfileList($job_title);
	$User_Type = getValueFromProfileList($User_Type);


     $job_role_array = explode(",",$job_role);
     $job_role_values = '';
     if(!empty($job_role)){
     for($i=0;$i<count($job_role_array);$i++){
     	$job_role_values .= getValueFromProfileList(trim($job_role_array[$i]," ")).',';
     }
     }
     
     $manager = getuserid($manager);
//      $organization = getorgid($organization);

     if(!empty($organization)){
		$organization = getorgid($organization);
		if(!$organization){
		sendReply('Invalid Organization');
		return;
         }
	}

     
     
     
	expDebug::dPrint("job_role_values".$job_role_values);
	
		switch(trim(strtolower($language)," ")){
	    case "english":
			$language = "cre_sys_lng_eng"; 
			break;
    	case "chinese":
			$language = "cre_sys_lng_gzh"; 
			break;
        case "japanese":
			$language = "cre_sys_lng_jap"; 
			break;
        case "spanish":
			$language = "cre_sys_lng_esp"; 
			break;
        case "german":
			$language = "cre_sys_lng_den"; 
			break;
    	case "french":
			$language = "cre_sys_lng_frh"; 
			break;
    	case "italian":
			$language = "cre_sys_lng_itn"; 
			break;
        case "korean":
			$language = "cre_sys_lng_kon"; 
			break;
        case "russian":
			$language = "cre_sys_lng_rus"; 
			break;
        case "portuguese":
			$language = "cre_sys_lng_pts"; 
			break;
    	default:
       		 // print_r("Invalid Language");
       		  $language = "cre_sys_lng_eng"; 

       		  
	}

	
	
	$requestdata = array("first_name"=> $fname,
			  'last_name'=> $lname,
			  'user_name'=> $uname,
			  'password'=>$pass,
			  'email'=>$email,
			  'status'=>'cre_usr_sts_atv',
			  'preferred_language'=>$language,
			  'org_id'=>$organization,
			  'manager_id'=>$manager,
			  'roles'=>strtolower($roles),
			  'employee_no'=>$emp_id,
			  'empltype'=>$emp_type,
			  'deptcode'=>$dept,
			  'jobrole'=>$job_role_values,
			  'jobtitle'=>$job_title,
			  'usertype'=>$User_Type,
			  'apiname'=>'UserCreationAPI',
			  'userid'=>$GLOBALS['userid']
						);
						
	$response = sendApirequest($requestdata);
	$id = $response['results']['jsonResponse'][0]['Id'];
	expDebug::dPrint("responseeeee".$id);
	expDebug::dPrint(is_numeric($id));
	$msg ='';
	if(is_numeric($id))
	{
	$msg = 'User Created ';
	expDebug::dPrint("is number");
	}else{
	$msg = 'User creation has failed .'.$id;
	}
	sendReply($msg);
}

function createOrganization($params)
{
	expDebug::dPrint('Inside Create Org' , 4);
	//$params = $GLOBALS['hints_array'];
	expDebug::dPrint('hints_array' . print_r($params, true) , 4); 
    
    $mandatoryFieldMissing = isMandatoryFieldsExists('org',$params);
    if($mandatoryFieldMissing)
    {
		$error = "Mandatory fields Missing - ".$mandatoryFieldMissing;
		sendReply($error);
		return ;
    }

	$org_name = trim($params['name']," ");
	$org_desc  = trim($params['description']," ");
	$org_type  = trim($params['type']," ");
	$costcenter  = trim($params['costcenter']," ");
	$parentorg  = trim($params['parent']," ");
	$contact  = trim($params['contact']," ");
//	$parentorg = getorgid($parentorg);
	
	if(!empty($parentorg)){
		$parentorg = getorgid($parentorg);
		if(!$parentorg){
		sendReply('Invalid Parent Organization');
		return;
         }
	
	}
	
	if(strtolower(trim($org_type," ")) == 'external'){
		$org_type  = 'cre_org_typ_ext';
	}else if(strtolower(trim($org_type," ")) == 'internal'){
		$org_type  = 'cre_org_typ_int';
	}else{
		sendReply('Invalid Organization Type');
		return;
	}

	$requestdata = array("name"=> $org_name,
			  'description'=> $org_desc,
			  'type'=> $org_type,
			  'apiname'=>'AddOrganizationAPI',
			  'cost_center'=>$costcenter ,
			  'parent'=>$parentorg,
			  'contact'=>$contact,
			  'status'=>'cre_org_sts_act',
			  'userid'=>$GLOBALS['userid']
							);
	$response = sendApirequest($requestdata);
	$id = $response['results']['jsonResponse'][0]['id'];
	expDebug::dPrint("responseeeee".$id);
	expDebug::dPrint(is_numeric($id));
	$msg ='';
	if(is_numeric($id))
	{
	$msg = 'Organization Created ';
	expDebug::dPrint("is number");
	}else{
	$msg = 'Organization creation has failed .'.$id;
	}

	sendReply($msg);
}

function addContentToVodWbt($crs_id,$cls_id,$cnt_id,$attempts,$validity)
{
	//print_r("Inside addContentToVodWbt");

	$requestdata = array("CourseId"=> $crs_id,
			  'ClassId'=> $cls_id,
			  'ContentId'=> $cnt_id,
			  'MaxAttempts'=> $attempts,
			  'ValidityDays'=> $validity,
			  'apiname'=>'AssociateContentwithWBTClassAPI',
			  'userid'=>$GLOBALS['userid']
							);
	$response = sendApirequest($requestdata);
	$id = $response['results']['jsonResponse'][0]['id'];
	return $id;
}


function createILTSession($crs_id,$cls_id,$instructor_name,$session_name,$start_date,$start_hours,$end_hours)
{
		//print_r("Inside add ilt session");
// 		$pdao=new AbstractDAO();
// 	    $pdao->connect();
// 	    $qry = "select id from slt_person where user_name = '".$instructor_name."'";
// 	    expDebug::dPrint("instructor id query".$qry,2);
// 		$res = $pdao->query($qry);
// 		$param=$pdao->fetchResult();
// 		$pdao->closeconnect();

		$instructor_name_array = explode(",",$instructor_name);
		$instructor_id_value = '';
		if(!empty($instructor_name)){
			for($i=0;$i<count($instructor_name_array);$i++){
				$instructor_id_value .= getinstructorid(trim($instructor_name_array[$i]," ")).',';
			}
		}
		
		$instructor_id = $instructor_id_value;
		
		
		if (preg_match("/^(0[1-9]|[1-2][0-9]|3[0-1])-(0[1-9]|1[0-2])-[0-9]{4}$/",trim($start_date," "))){
		    $time = strtotime($start_date);
        	$start_date = date('Y-m-d',$time);
        	//print_r("time test".$start_date);
			$currentdate = date("Y-m-d");
			//print_r("time test".$currentdate);
// 			if($currentdate <= $start_date){
				$start_date = $start_date;
// 			}else{
// 				$msg = "Start date is invalid. Date format is dd-mm-yyyy";
// 				return $msg ;
// 			}
		}else {
				$msg = "Start date is invalid. Date format is dd-mm-yyyy";
				return $msg ;
		}
		//print_r("time test final".$start_date);
	$requestdata = array("course_id"=> $crs_id,
			  'class_id'=> $cls_id,
			  'instructor_id'=>$instructor_id,
			  'session_name'=> $session_name,
			  'start_date'=> $start_date,
			  'start_hours'=> $start_hours,
			  'end_hours'=> $end_hours,
			  'apiname'=>'SessionCreationILTAPI',
			  'userid'=>$GLOBALS['userid']
							);
	$response = sendApirequest($requestdata);
	$id = $response['results']['jsonResponse'][0]['id'];
	$iserror = $response['results']['jsonResponse'][0]['iserror'];
	if($iserror == 'true' || $iserror == true){
	return $response['results']['jsonResponse'][0]['shortmessage'];
	}
	return $id;
}



function createVCSession($crs_id,$cls_id,$instructor_name,$session_name,$start_date,$start_hours,$end_hours,$meeting_type,$timezone,$meeting_id='',$attendee_pwd='',$presenter_pwd='',$attendee_url='',$presenter_url='',$presenter_name)
{
		// print_r("Inside add vc session");
		// print_r($meeting_type);
		// print_r($timezone);
		
	// $pdao=new AbstractDAO();
		// $pdao->connect();
		// $qry = "select id from slt_person where user_name = '".$instructor_name."'";
		// expDebug::dPrint("instructor id query".$qry,2);
		// $res = $pdao->query($qry);
		// $param=$pdao->fetchResult();
		// $pdao->closeconnect();
	$instructor_name_array = explode ( ",", $instructor_name);
	$instructor_id_value = '';
	if (! empty ( $instructor_name )) {
		for($i = 0; $i < count ( $instructor_name_array ); $i ++) {
			$instructor_id_value .= getinstructorid ( trim ( $instructor_name_array [$i], " " ) ) . ',';
		}
	}
	$instructor_id = $instructor_id_value;
	$presenter_id = '';
	
	if(! empty ( $instructor_name )){
	if (in_array($presenter_name, $instructor_name_array)) {
		$presenter_id = getinstructorid ( trim ($presenter_name, " " ) );
	}else{
		$msg = "Invalid Presenter ";
		return $msg ;
	}
	}
	
		if (preg_match("/^(0[1-9]|[1-2][0-9]|3[0-1])-(0[1-9]|1[0-2])-[0-9]{4}$/",trim($start_date," "))){
		    $time = strtotime($start_date);
        	$start_date = date('Y-m-d',$time);
       // 	print_r("time test".$start_date);
			$currentdate = date("Y-m-d");
		//	print_r("time test".$currentdate);
// 			if($currentdate <= $start_date){
				$start_date = $start_date;
// 			}else{
// 				$msg = "Start date is invalid. Date format is dd-mm-yyyy";
// 				return $msg ;
// 			}
		}else {
				$msg = "Start date is invalid. Date format is dd-mm-yyyy";
				return $msg ;
		}
		
		
		$pdao=new AbstractDAO();
	    $pdao->connect();
	    $qry = "select code from slt_profile_list_items where name like '%".$timezone."%'";
	    expDebug::dPrint("instructor id query".$qry,2);
		$res = $pdao->query($qry);
		$param=$pdao->fetchResult();
		$pdao->closeconnect();
		$timezone = $param->code;
		
		if(empty($timezone)){
			$msg = "Invalid timezone";
			return $msg ;
		}    

		
		
	$requestdata = array("course_id"=> $crs_id,
			  'class_id'=> $cls_id,
			  'instructor_id'=>$instructor_id,
			  'presenter_id'=>$presenter_id,
			  'session_name'=> $session_name,
			  'start_date'=> $start_date,
			  'start_hours'=> $start_hours,
			  'end_hours'=> $end_hours,
			  'meeting_type'=>$meeting_type,
			  'time_zone'=>$timezone,
			  'session_meeting_id'=>$meeting_id,
			  'session_presenter_url'=>$presenter_url,
			  'session_attendee_url'=>$attendee_url,
			  'session_attende_password'=>$attendee_pwd,
			  'session_presenter_password'=>$presenter_pwd,
			  'apiname'=>'SessionCreationVCAPI',
			  'userid'=>$GLOBALS['userid']
							);
	$response = sendApirequest($requestdata);
	$id = $response['results']['jsonResponse'][0]['id'];
	$iserror = $response['results']['jsonResponse'][0]['iserror'];
	if($iserror == 'true' || $iserror == true){
	return $response['results']['jsonResponse'][0]['shortmessage'];
	}
	return $id;
}






function sendApiRequest($requestdata){

	$requestdata['returntype'] = 'json';
	/* Request for access token */
	$peerVerify = getConfigValue('peer_verify') == 0 ? FALSE : TRUE;
	$base_url = (!empty($_SERVER['HTTPS'])) ? "https://".$_SERVER['SERVER_NAME'] : "http://".$_SERVER['SERVER_NAME'];;
	$client_id=CLIENT_ID;
	$client_secret=CLIENT_SECRET;
	
	$client_details = base64_encode("$client_id:$client_secret");
	
	$context = stream_context_create(array(
		'http' => array(
		'method' => 'POST',
		'header' => "Content-Type: application/x-www-form-urlencoded\r\n" .
					"Authorization : Basic $client_details\r\n",
		'content' => "grant_type=client_credentials"
		),
		'ssl' => array(
 				'verify_peer'=> $peerVerify, 
 	      'verify_peer_name'=> $peerVerify
 			)
	));
	 
	$response = file_get_contents($base_url.'/apis/oauth2/Token.php', false, $context);
	$res =  json_decode($response);
	$token = $res->access_token;
	
	expDebug::dPrint('Request data' . print_r($requestdata, true) , 4);
	

	// API params
		
	$fields_string = http_build_query($requestdata); 
	
	$context = stream_context_create(array(
		'http' => array(
		'method' => 'POST',
		'header' => "Content-Type: application/x-www-form-urlencoded\r\n" .
					"Authorization : Bearer $token\r\n",
		'content' => $fields_string
		),
		'ssl' => array(
 				'verify_peer'=> $peerVerify, 
 	      'verify_peer_name'=> $peerVerify
 			)
	)); 
	
	$response = file_get_contents($base_url.'/apis/ext/ExpertusOneAPI.php', false, $context);
	$res =  json_decode($response,true);
	expDebug::dPrint('API response' . print_r($res, true) , 4); 

	$iserror = $res['results']['jsonResponse'][0]['iserror'];
	if(	$iserror ){
	
	    if(getConfigValue('api_error_response_format') == '0'){
		$res['results']['jsonResponse'][0]['Id'] = $res['results']['jsonResponse'][0]['shortmessage'];
		$res['results']['jsonResponse'][0]['id'] = $res['results']['jsonResponse'][0]['shortmessage'];
	    return $res;
	    }else  if(getConfigValue('api_error_response_format') == '1'){
	    $res['results']['jsonResponse'][0]['Id'] = $res['results']['jsonResponse'][0]['errors'][0]['message'];
		$res['results']['jsonResponse'][0]['id'] = $res['results']['jsonResponse'][0]['errors'][0]['message'];
		return $res;
	    }
	}
	
	if(strpos($response,'Mandatory Fields are missing in the feed') !== false){
		$res['results']['jsonResponse'][0]['Id'] = 'Mandatory Fields are missing';
		$res['results']['jsonResponse'][0]['id'] = 'Mandatory Fields are missing';
	}

	
	return $res;
}


function sendReply($data){
	expDebug::dPrint('Msg ' . print_r($data, true) , 4); 
	expDebug::dPrint('From ' . trim($_POST['From']," ") , 4); 
    expDebug::dPrint(getConfigValue('twilio_number'));
    expDebug::dPrint(getConfigValue('twilio_sid'));
    expDebug::dPrint(getConfigValue('twilio_token'));


	echo $data;
	$account_sid = getConfigValue('twilio_sid'); 
	$auth_token = getConfigValue('twilio_token'); 

	$client = new Services_Twilio($account_sid, $auth_token); 
 
	$client->account->messages->create(array(  
		'From' => getConfigValue('twilio_number'),    
		'Body' => $data,
		'To' => trim($_POST['From']," ")
	));

}


function validateUser(){

		$fr= substr(trim($_POST['From']," "),-8);
		$username = $GLOBALS['uname'];
		$pdao=new AbstractDAO();
	    $pdao->connect();
	    $qry = "select id  from slt_person where (mobile_no like '%".$fr."' or phone_no like '%".$fr."') and is_mobileadmin = 1";
	    expDebug::dPrint("user id query".$qry,2);
		$res = $pdao->query($qry);
		$param=$pdao->fetchResult();
		$pdao->closeconnect();
		$id = $param->id;
		expDebug::dPrint('user id' . print_r($id, true) , 4); 
		if(empty($id)){
		return false; 
		}
		
		$access_page = '';
		$privilege = '';
        if($GLOBALS['entity'] == 'vodcls' || $GLOBALS['entity'] == 'wbtcls' || $GLOBALS['entity'] == 'iltcls' || $GLOBALS['entity'] == 'vccls'  || $GLOBALS['entity'] == 'crs' ){
       		 $access_page = 'cre_sec_pmn_adm_001_001';
       		 $privilege = 'sgp.priv_add = 1 AND sgp.priv_edit = 1';
        }else if($GLOBALS['entity'] == 'usr'  ){
        	$access_page = 'cre_sec_pmn_adm_002_001';
        	$privilege = 'sgp.priv_add = 1';
        }else if($GLOBALS['entity'] == 'org' ){
        	$access_page = 'cre_sec_pmn_adm_002_002';
        	$privilege = 'sgp.priv_add = 1';
        }
        expDebug::dPrint('access page' . print_r($access_page, true) , 4); 
		
		$pdao=new AbstractDAO();
	    $pdao->connect();
	    $qry = "select * from slt_groups as sg left join slt_group_privilege as sgp on sgp.group_id = sg.id where FIND_IN_SET( '".$id."', sg.added_users )  and (sgp.access_page = '".$access_page."' and ".$privilege.")";
	    expDebug::dPrint("group id query".$qry,2);
		$res = $pdao->query($qry);
		$param=$pdao->fetchResult();
		$pdao->closeconnect();
		$grpid = $param->id;
		expDebug::dPrint('group id' . print_r($grpid, true) , 4); 

		if(empty($grpid)){
			return false; 
		}else{
			return $id;
		}        


}

function getValueFromProfileList($name){
		$pdao=new AbstractDAO();
	    $pdao->connect();
	    $qry = "select code  from slt_profile_list_items where name = '".$name."' ";
	    expDebug::dPrint("slt_profile_list_items code query".$qry,2);
		$res = $pdao->query($qry);
		$param=$pdao->fetchResult();
		$pdao->closeconnect();
		$code = $param->code;
		expDebug::dPrint("slt_profile_list_items".$code,2);
		return $code;
}

function getuserid($username){
		$pdao=new AbstractDAO();
	    $pdao->connect();
	    $qry = "select id  from slt_person where user_name = '".$username."' ";
	    expDebug::dPrint("slt_person id query".$qry,2);
		$res = $pdao->query($qry);
		$param=$pdao->fetchResult();
		$pdao->closeconnect();
		$id = $param->id;
		expDebug::dPrint("slt_person id".$id,2);
		return $id;
}

function getorgid($orgname){
		$pdao=new AbstractDAO();
	    $pdao->connect();
	    $qry = "select id  from slt_organization where name = '".$orgname."' ";
	    expDebug::dPrint("slt_organization id query".$qry,2);
		$res = $pdao->query($qry);
		$param=$pdao->fetchResult();
		$pdao->closeconnect();
		$id = $param->id;
		expDebug::dPrint("slt_organization id".$id,2);
		return $id;
}


function getlocationid($locname){
	$pdao=new AbstractDAO();
	$pdao->connect();
	$qry = "select id  from slt_location where name = '".$locname."' ";
	expDebug::dPrint("slt_location id query".$qry,2);
	$res = $pdao->query($qry);
	$param=$pdao->fetchResult();
	$pdao->closeconnect();
	$id = $param->id;
	expDebug::dPrint("slt_location id".$id,2);
	return $id;
}

function getinstructorid($instructorname){
	
	$pdao=new AbstractDAO();
	$pdao->connect();
	$qry = "select id from slt_person where user_name = '".$instructorname."'";
	expDebug::dPrint("instructor id query".$qry,2);
	$res = $pdao->query($qry);
	$param=$pdao->fetchResult();
	$pdao->closeconnect();
	$id = $param->id;
	expDebug::dPrint("instructor id id".$id,2);
	return $id;
	
}




function checkSmstemplate($msg){
	if(strpos($msg,'#') !== false){
	return true;
	}else{
	return false;
	}
}

function saveMsg(){
	  $msg = trim($_POST['Body']," ");
	  $qry ='';
      $previousrecord = isPrevMsgExists();
      if($previousrecord)
      {
      $msg = $previousrecord->body." ".trim($_POST['Body']," ");
      $qry =  "UPDATE slt_sms_msg SET `body`='".$msg."',`updated_on` = now() WHERE id='".$previousrecord->id."';";
      }else{
      $qry = "INSERT INTO slt_sms_msg (`from`,`body`,`updated_on`) VALUES  ('".trim($_POST['From']," ")."','".trim($_POST['Body']," ")."',now())";
      }
        expDebug::dPrint('isPrevMsgExists' . print_r($msg, true) , 4); 
		$pdao=new AbstractDAO();
	    $pdao->connect();
	    expDebug::dPrint("AICC Handler::Get session details :".$qry,2);
		$res = $pdao->execute($qry);
		$res = $pdao->getLastRecordNo();
		$pdao->closeconnect();
		
	
}


function isPrevMsgExists(){
		$pdao=new AbstractDAO();
	    $pdao->connect();
	    $qry = "select * from slt_sms_msg where `from` = '".trim($_POST['From']," ")."' ";
	    expDebug::dPrint("slt_organization id query".$qry,2);
		$res = $pdao->query($qry);
		$param=$pdao->fetchResult();
		$pdao->closeconnect();
		expDebug::dPrint("previous msg body".$param->body,2);
		return $param;

}


function getFullMsg(){

		$msg = trim($_POST['Body']," ");
		$previousrecord = isPrevMsgExists();
		if($previousrecord){
		$msg = $previousrecord->body.trim($_POST['Body']," ");
		}
		$pdao=new AbstractDAO();
	    $pdao->connect();
	    $qry = "DELETE FROM slt_sms_msg  WHERE id = '".$previousrecord->id."';";
	    expDebug::dPrint("slt_organization id query".$qry,2);
		$res = $pdao->execute($qry);
		$pdao->closeconnect();
		expDebug::dPrint("getFullMsg getFullMsg".$msg,2);
		
		return $msg;


}

function formatMsg($msg,$entity){
	expDebug::dPrint('inside format msg', 4); 
 	expDebug::dPrint('Entity Type'.$entity , 4); 
	$entityarray = '';

	switch ($entity) {
    	case "crs":
			$entityarray = array("title","code","description","language","status","author","compliance","completeby","validity","currencytype","price");
       		 break;
    	case "wbtcls":
			$entityarray =array("coursecode","title","classcode","description","language","currencytype","price","contentname","attempts","validity","status");
        	break;
        case "vodcls":
			$entityarray =array("coursecode","title","classcode","description","language","currencytype","price","contentname","attempts","validity","status");
        	break;
        case "iltcls":
			$entityarray =array("coursecode","title","classcode","description","language","currencytype","price","maxseats","location","waitlistcount","sessionname","instructorname","startdate","starttime","endtime","status");
        	break;
        case "vccls":
			$entityarray = array("coursecode","title","classcode","description","language","currencytype","price","maxseats","waitlistcount","sessionname","instructorname","startdate","starttime","endtime","timezone","meetingtype","presentername","meetingid","attendeepassword","presenterpassword","attendeeurl","presenterurl","status");
        	break;
    	case "usr":
			$entityarray =array("firstname","lastname","username","password","email","language","organization","manageruser","roles","employeetype","employeeid","department","userttype","jobrole","jobtitle");
        	break;
    	case "org":
			$entityarray = array("name","description","type","costcenter","parent","contact"); 
        	break;
    	default:
       		  expDebug::dPrint('Invalid Entity Type' , 4); 
       		  print_r("Invalid Entity Type");
			  return "";
	}
	$msg = trim(substr($msg,strpos(strtolower($msg),$entity)+strlen($entity))," ");
	
	expDebug::dPrint('Message with key value pair'.$msg, 4); 
	$arr =array();

   for($i=0;$i<count($entityarray);$i++){
       $pos = strpos(strtolower($msg),$entityarray[$i]);
       $length = strlen($entityarray[$i]);
      	if($pos){  
       		if($pos == 0){
    		 	$msg = putinplace($msg, "", $pos);
       		}else{ 
       			$msg = putinplace($msg, "&&", $pos);
       		}	
    	}
	}
     
     $hashpos = strpos($msg,'#');
     $msg =  substr($msg,0, $hashpos);
     $msg = putinplace($msg, "", $hashpos);
     expDebug::dPrint('Final '.$msg , 4); 
     
     
     $keyvalue = explode("&&",$msg);
     for($i=0;$i<count($keyvalue);$i++){
     $keyvaluearray = explode("=",trim($keyvalue[$i]," "));
      $arr[trim(strtolower($keyvaluearray[0])," ")] = trim($keyvaluearray[1]);
     }
     expDebug::dPrint('Final Array' . print_r($arr, true) , 4); 
     return $arr;

}


function putinplace($string=NULL, $put=NULL, $position=false)
{
    $d1=$d2=$i=false;
    $d=array(strlen($string), strlen($put));
    if($position > $d[0]) $position=$d[0];
    for($i=$d[0]; $i >= $position; $i--) $string[$i+$d[1]]=$string[$i];
    for($i=0; $i<$d[1]; $i++) $string[$position+$i]=$put[$i];
    return $string;
}


function isMandatoryFieldsExists($entity,$params)
{
    $mandatoryArray = '';
    $missing_fields = '';
    if($entity == 'org'){
    	$mandatoryArray = array("name","description","type"); 
    }else if($entity == 'usr'){
    	$mandatoryArray = array("firstname","lastname","username","password","email"); 
    }else if($entity == 'crs'){
    	$mandatoryArray = array("title","code","description","language","status"); 
    }else if($entity == 'wbt' || $entity == 'vod'){
    	$mandatoryArray = array("coursecode","title","classcode","description","language","contentname","status"); 
    }else if($entity == 'ilt'){
    	$mandatoryArray = array("coursecode","title","classcode","description","language","maxseats","location","sessionname","startdate","starttime","endtime","status");
    }else if($entity == 'vc'){
        if($params['meetingtype']){
        	if(strtolower($params['meetingtype']) == 'expertusmeeting'){
				$mandatoryArray = array("coursecode","title","classcode","description","language","maxseats","sessionname","startdate","starttime","endtime","timezone","meetingtype","status");
		    }else if(strtolower($params['meetingtype']) == 'livemeeting'){
		    	$mandatoryArray = array("coursecode","title","classcode","description","language","maxseats","sessionname","startdate","starttime","endtime","timezone","meetingtype","meetingid","attendeepassword","presenterpassword","status");
		    }else if(strtolower($params['meetingtype']) == 'othermeeting'){
		    	$mandatoryArray = array("coursecode","title","classcode","description","language","maxseats","sessionname","startdate","starttime","endtime","timezone","meetingtype","attendeeurl","presenterurl","status");    
		    }
    	}else{
    		$mandatoryArray = array("coursecode","title","classcode","description","language","maxseats","sessionname","startdate","starttime","endtime","timezone","meetingtype","status");
    	}
    }
    	
    for($i=0;$i<count($mandatoryArray);$i++){
    	if(!$params[$mandatoryArray[$i]]){
    	    if(!empty($missing_fields)){
    	 	$missing_fields .= ','.$mandatoryArray[$i];
    	 	}else{
    	 	$missing_fields = $mandatoryArray[$i];
    	 	}
    	}
    }
    return $missing_fields;
	
}


?>