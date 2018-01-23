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
require_once "../dao/AbstractDAO.php";
require_once "../services/Encryption.php";
require_once "../libraries/tincan_php_library/tincan_client.php";
require_once "../modules/core/exp_sp_core/exp_sp_core.module";

header("Content-Type:text/html");
header("Cache-Control: no-store, no-cache, must-revalidate, post-check=0, pre-check=0");
header("Pragma: no-cache");
header("ETag: PUB" . time());
$exitflag = false;
$currenttime = time ();
expDebug::dPrint(date("m/d/y H:i:s",$currenttime)." ". "Log Started\n",4);
$isError=0;

foreach ($_POST as $key => $value)
{
	expDebug::dPrint("AICC POST KEY - ".$key."  - Value  - ".$value,4);
	$tempkey = strtolower($key);
	$_POST[$tempkey] = $value;
}

$cmd = isset($_POST["command"])?$_POST["command"]:'';
$cmd = strtoupper($cmd);

$aicc_id_mob=isset($_GET["aicc_id"])?$_GET["aicc_id"]:'';
expDebug::dPrint("aicc_id_mob :".$aicc_id_mob);
$aicc_data = !empty($_POST["aicc_data"])?urldecode($_POST["aicc_data"]):'';
$session_id = isset($_POST["session_id"])?$_POST["session_id"]:'';
$logedId = '';
$aicc_sid = '';
$sid = array();
$total_time_spend = '';

expDebug::dPrint("Received Aicc session id :".$session_id,4);
expDebug::dPrint("command   :".$cmd,4);

if ($cmd == "SETSESSIONMOBILE"){
	$aicc_sid_mob=isset($_POST["aicc_id"])?$_POST["aicc_id"]:'';

	expDebug::dPrint("Inside_SETSESSIONMOBILE_AICC4587");
	$session_id = setSessionAiccMobile("set",$aicc_sid_mob );

	expDebug::dPrint("command_seession_id   :".$session_id,4);
	echo $session_id;
	exit();
}

/**
 * Validate AICC request from logged user - Start
 */
if(!empty($session_id) && !empty($cmd)){
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
	expDebug::dPrint(" AICC Interaction id -- ".$req_id);

	$pdao=new AbstractDAO();
	$pdao->connect();
	$qry = "SELECT aicc_id,session_id FROM slt_aicc_interaction WHERE id='".$req_id."'";
	expDebug::dPrint("AICC Handler::Get session details :".$qry,2);
	$res = $pdao->query($qry);
	$sesdt=$pdao->fetchAllResults();
	$pdao->closeconnect();
	if(sizeof($sesdt)<=0 || $session_id!=$sesdt[0]->session_id){
		$isError=1;
		if(stripos($cmd,'UPDATE')===false){
			$resp = "Error=1\r\n";
			$resp.= "Error_Text=Invalid Access\r\n";
			$resp.= "version=3.0\r\n";
			echo $resp;
			expDebug::dPrint("AICC Handler::Launch Error :".$resp,1);
		}
	}else if(substr_count($sesdt[0]->aicc_id, '-')!=6){

		$aicc_sid =$aicc_id_mob;
		$sid = explode("-",$aicc_sid);

	}
	else{
		$aicc_sid  = $sesdt[0]->aicc_id;
		$sid = explode("-",$aicc_sid);
	}
	expDebug::dPrint("Aicc aicc id :".$aicc_sid,4);
}else{
	$resp = "Error=1\r\n";
	$resp.= "Error_Text=Invalid Access\r\n";
	$resp.= "version=3.0\r\n";
	echo $resp;
	$isError=1;
	expDebug::dPrint("AICC Handler::Launch Error :".$resp,1);
}

/**
 * Validate AICC request from logged user - END
 */

if($isError==0){
	$enr_id = explode('-',$aicc_sid);
	$dao=new AbstractDAO();
	$dao->connect();
	$dao->query("SELECT reg_status FROM slt_enrollment WHERE id =". trim($enr_id[5]).";");
	$result_reg_status = $dao->fetchResult();
	$reg_status = $result_reg_status->reg_status;
	$dao->closeconnect();
	if ($cmd == "GETPARAM"){
		if($aicc_sid){
			$pdao=new AbstractDAO();
			$pdao->connect();
			$qry = "SELECT id,full_name FROM slt_person WHERE id='".$sid[0]."'";
			expDebug::dPrint("AICC Handler::Get User details :".$qry,2);
			$res = $pdao->query($qry);
			$usrdt=$pdao->fetchAllResults();
			$pdao->closeconnect();
		}
		$vUserId 			= $usrdt[0]->id;
		$vUserName 			= $usrdt[0]->full_name;
		expDebug::dPrint("User id :".$vUserId,4);
		expDebug::dPrint("User Name :".$vUserName,4);
		expDebug::dPrint( date("m/d/y H:i:s",$currenttime)." ". " ===> cmd:" .$cmd . "\n",4);
		expDebug::dPrint( date("m/d/y H:i:s",$currenttime)." ". " ===> aicc_data:" .$aicc_data . "\n",4);

		$dao=new AbstractDAO();
		$dao->connect();
		$qry = "SELECT param FROM slt_aicc_interaction WHERE session_id='".$session_id."'";
		expDebug::dPrint("AICC Handler::GETPARAM - slt_aicc_interaction Query :".$qry,2);
		$res = $dao->query($qry);
		$result=$dao->fetchAllResults();
		$dao->closeconnect();
			
		if(empty($result[0]) || empty($result[0]->param)){
			$dao0=new AbstractDAO();
			$dao0->connect();
			$qry0 = "SELECT aicc_data, total_time_spend FROM slt_attendance_summary WHERE user_id=trim($sid[0]) AND course_id=trim($sid[1]) AND class_id=trim($sid[2])";
			$qry0 .= " AND lesson_id=trim($sid[3]) and content_version_id = trim($sid[4]) AND enrollment_id = trim($sid[5])";
			expDebug::dPrint("AICC Handler::GETPARAM - Query :".$qry0,2);
			$res0 = $dao0->query($qry0);
			$rst0=$dao0->fetchAllResults();
			$dao0->closeconnect();
			$aiccdata = !empty($rst0[0])?$rst0[0]->aicc_data:'';
			$total_time_spend = !empty($rst0[0])?$rst0[0]->total_time_spend:'';

			expDebug::dPrint("AICC Handler::GETPARAM aicc data  : ".$rst0[0]->aicc_data,2);

			if(!empty($total_time_spend)) {
				$aiccdata = checkAiccTotalTimeSpend($aiccdata, $total_time_spend);
			}
		}
		else{
			$aiccdata = $result[0]->param;
		}
		$aicc_tmp= explode("\n",$aiccdata) ;
		$aicc_score = '';
		$stpo = stripos(strtolower($aiccdata),'[core]')+7;
		$enpo = stripos($aiccdata,"[*Interaction Starts*]");
		$aiccdata = substr($aiccdata,$stpo,$enpo-$stpo);

		expDebug::dPrint("GETPARAM Attributs >>> ".$vUserId."--".$vUserName."--".$sid[6],4);
		$resp='';
		$resp.= "Error=0\r\n";
		$resp.= "Error_Text=Successful\r\n";
		$resp.= "version=3.0\r\n";
		$resp.= "aicc_data=\r\n";
		$resp.= "[core]\r\n";
		$resp.= 'Student_ID='.$vUserId."\r\n";
		$resp.= 'Student_Name='.$vUserName."\r\n";
		if($aiccdata==''){
			$other_info = array();
			$dao0=new AbstractDAO();
			$dao0->connect();
			$qry0 = "SELECT masteryscore, datafromlms FROM slt_content_lesson WHERE id=trim($sid[3]) ";
			expDebug::dPrint("AICC Handler::GETPARAM - Lesson other details Query --  :".$qry0, 2);
			$res0 = $dao0->query($qry0);
			$lmsdt=$dao0->fetchAllResults();
			$dao0->closeconnect();
			if(!empty($lmsdt[0]->datafromlms))
			$other_info = json_decode($lmsdt[0]->datafromlms);
			expDebug::dPrint("Other informantion -->".print_r($other_info,true),3);
			$resp.= 'Score='.$aicc_score."\r\n";
			$resp.= 'Time='.'0000:00:00.00'."\r\n";
			$resp.= 'Credit='.'C'."\r\n";
			$resp.= 'Lesson_Location='.$sid[6]."\r\n";
			$resp.= "Lesson_Status=\r\n";
			$resp.= "OUTPUT_FILE=\r\n";
			$resp.= "[Core_Lesson]\r\n";
			$resp.= "[Student_Data]\r\n";
			$resp.= "mastery_Score=".$lmsdt[0]->masteryscore."\r\n";
			$resp.= "max_time_allowed=".$other_info->Max_Time_Allowed."\r\n";
			$resp.= "time_limit_action=".$other_info->Time_Limit_Action."\r\n";
			$resp.= "[Evaluation]\r\n";
			$resp.= "Course_ID=\r\n";
		}else{
			$resp.= trim($aiccdata)."\r\n";
		}

		echo $resp;
		expDebug::dPrint( "getParam Completed Response >>> ".$resp,4);
		expDebug::dPrint( date("m/d/y H:i:s",$currenttime)." ". " ===> getParam Completed\n",4);
	}
	else if ($cmd == "PUTPARAM"){
		expDebug::dPrint( date("m/d/y H:i:s",$currenttime)." ". " ===> cmd:" .$cmd . "\n",4);
		expDebug::dPrint( date("m/d/y H:i:s",$currenttime)." ". " ===> aicc_data:" .$aicc_data . "\n",4);
		echo "Error=0\r\n";
		echo "Error_Text=Successful\r\n";
		expDebug::dPrint( date("m/d/y H:i:s",$currenttime)." ". " ===> putParam Completed\n",4);
	}
	else if ($cmd == "PUTINTERACTIONS"){
		expDebug::dPrint( date("m/d/y H:i:s",$currenttime)." ". " ===> cmd:" .$cmd . "\n",4);
		expDebug::dPrint( date("m/d/y H:i:s",$currenttime)." ". " ===> aicc_data:" .$aicc_data . "\n",4);
		echo "Error=0\r\n";
		echo "Error_Text=Successful\r\n";
		expDebug::dPrint( date("m/d/y H:i:s",$currenttime)." ". " ===> putInteractions Completed\n",4);
	}
	else if ($cmd == "EXITAU"){
		expDebug::dPrint( date("m/d/y H:i:s",$currenttime)." ". " ===> cmd:" .$cmd . "\n",4);
		expDebug::dPrint( date("m/d/y H:i:s",$currenttime)." ". " ===> aicc_data:" .$aicc_data . "\n",4);
		// Removed the updateLMSData call from here, since it will update total attempts as 2, 
		// if the user launch a content once. Modified by Shobana for #0065895.
		if(($sid[1] == 0) && ($sid[2] == 0)){
			updateAiccPreview($session_id);
		}
		echo "Error=0\r\n";
		echo "Error_Text=Successful\r\n";
		expDebug::dPrint( date("m/d/y H:i:s",$currenttime)." ". " ===> exitAU Completed\n",4);
		$exitflag = true;
	}
	else if ($cmd == "UPDATELMSDATA"){
		//if($reg_status == 'lrn_crs_reg_cnf') {
		  updateLMSData($aicc_sid,$session_id,$sid[0]);
		//}
	}else if ($cmd == "UPDATEAICCPREVIEW"){
		updateAiccPreview($session_id);
	}else{
		expDebug::dPrint( date("m/d/y H:i:s",$currenttime)." ". " ===> cmd:" .$cmd . "\n",4);
		expDebug::dPrint( date("m/d/y H:i:s",$currenttime)." ". " ===> aicc_data:" .$aicc_data . "\n",4);
		echo "Error=1\r\n";
		echo "Error_Text=InvalidCommand\r\n";
		expDebug::dPrint( date("m/d/y H:i:s",$currenttime)." ". " ===> inValid Completed\n",4);
	}
	captureData($cmd,$aicc_data,$session_id,$sid[0]);
}
function writeToLog($msgstr)
{
	expDebug::dPrint( date("m/d/y H:i:s",$currenttime)." ". $msgstr,4);
}

function captureData($cmd,$aicc_data,$session_id,$vUserId){
	$qry1='';
	$qry2='';
	$rst1;
	if((!empty($aicc_data) && $cmd!="EXITAU") || $cmd == "GETPARAM" ){
		try{
			if($cmd == "PUTPARAM" || $cmd == "GETPARAM")
			$qry2 = "UPDATE slt_aicc_interaction SET param='".$aicc_data."', connection_status='Y', updated_by='".$vUserId."', updated_on = now() WHERE session_id='".$session_id."'";
			else if($cmd=="PUTINTERACTIONS")
			$qry2 = "UPDATE slt_aicc_interaction SET interaction= concat(ifnull(interaction,''), '".$aicc_data."'), connection_status='Y', updated_by='".$vUserId."', updated_on = now() WHERE session_id='".$session_id."'";
			if(!empty($qry2)){
				$dao=new AbstractDAO();
				$dao->connect();
				expDebug::dPrint("AICC Handler::captureData - Insert/Update Query :".$qry2,2);
				$res = $dao->execute($qry2);
				$dao->closeconnect();
			}
		}catch(Exception $e){
			$dao->closeconnect();
			expDebug::dPrint("AICC Handler::captureData Error :".$e->getMessage());
		}
	}
}

function updateLMSData($aicc_sid,$session_id,$vUserId){
	$aicc_sid = trim($aicc_sid);
	try{
		define('DRUPAL_ROOT', $_SERVER['DOCUMENT_ROOT']);
		include_once $_SERVER['DOCUMENT_ROOT'].'/includes/bootstrap.inc';
		require_once "../modules/core/exp_sp_core/exp_sp_core.inc";
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$conObj = new GlobalUtil();
		$conf = $conObj->getConfig();
		$tincan = $conf['tincanapi'];		
		expDebug::dPrint("sadas dsadsa".print_r($tincan,true),5);
		$total_time_spend_update ='';
		$dao=new AbstractDAO();
		$dao->connect();
		$qry1 = "SELECT id,aicc_id,param,interaction,connection_status FROM slt_aicc_interaction WHERE session_id='".$session_id."'";
		expDebug::dPrint("AICC Handler::updateLMSData - Query :".$qry1,2);
		$res = $dao->query($qry1);
		$rst1=$dao->fetchAllResults();
		$dao->closeconnect();
		if($rst1[0]->connection_status == 'Y'){
			$calculateTime = true;
			if(!empty($rst1[0]->param)){
				$aicc_data = $rst1[0]->param;
				$aicc_int = $rst1[0]->interaction;
			}else{
				$sid = explode('-',$aicc_sid);
				$dao=new AbstractDAO();
				$dao->connect();
				$qry1 = "SELECT aicc_data FROM slt_attendance_summary WHERE user_id=trim($sid[0]) AND course_id=trim($sid[1]) AND class_id=trim($sid[2])";
				expDebug::dPrint("AICC Handler::updateLMSData from previous value - Query :".$qry1,2);
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
			$aicc_data_dump = $aicc_data." [*Interaction Starts*] ".$aicc_int;
			$aicc_tmp= explode("\n",$aicc_data) ;
			$status="lrn_crs_cmp_enr";
			$score="0,";
			$location;
			$timespend;
			$cstatus="not attempted";
			$grade='';
			$progress = null;
			for($i=0;$i<sizeOf($aicc_tmp);$i++){
				$tmp_untrimmed = explode('=',$aicc_tmp[$i]);
				$tmp = array();
				$tmp[0] = trim($tmp_untrimmed[0]);
				$tmp[1] = isset($tmp_untrimmed[1]) ? trim($tmp_untrimmed[1]) : '';
				if(strtolower($tmp[0])=="lesson_status"){
					$stat = trim(strtolower($tmp[1]));
					switch($stat){
						case 'incomplete':
						case 'i':
							$status="lrn_crs_cmp_inp"; // In-Progress/
							$cstatus="incomplete";
							break;
						case 'failed':
						case 'f':
							$status="lrn_crs_cmp_cmp"; // Completed/
							$cstatus="failed";
							$progress=100;
							break;
						case 'completed':
						case 'c':
						case 'passed':
						case 'p':
							$status="lrn_crs_cmp_cmp"; // Completed
							$cstatus="completed";
							$progress=100;
							break;
						case 'not attempted':
						case 'n':
						case 'na':
						default :
							$status="lrn_crs_cmp_enr"; // Enrolled
							$cstatus="not attempted";
							break;
					}
				}else if(strtolower($tmp[0])=="score"){
					$score=trim($tmp[1]);
					if(!empty($score) && $score!=''){
						$score = is_numeric($score)?$score:addslashes(trim($score));
						if(stripos($score,',')>0){
							$tmpscore = explode(',',$score);
							$score = $tmpscore[0];
						}
					//	$score = "'".$score."',";
					}else{
						$score = "";
					}
				}else if(strtolower($tmp[0])=="time"){

					$int_timespend = "00.00";
					$timespend_split = explode('.',$int_timespend);
					$tmp1 = explode(":",$tmp[1]);
					if($tmp1[0]>0) {
						$timespend_split[0] = intval($timespend_split[0])+(intval($tmp1[0])*60);
					}
					if($tmp1[1]>0) {
						$timespend_split[0] = intval($timespend_split[0])+intval($tmp1[1]);
					}
					if($tmp1[2]>0) {
						$timespend_split[1] = sprintf("%02s",($timespend_split[1] + $tmp1[2]));
					}
					$timespend =implode(".",$timespend_split);
					$timespend = ($calculateTime===true)?$timespend:0;
				}else if(strtolower($tmp[0])=="lesson_location"){
					$location=$tmp[1];
					expDebug::dPrint($location,4);
				}
			}
			$tsid = explode('-',$aicc_sid);
			$dao->connect();
			$dao->query("SELECT comp_status,master_enrollment_id FROM slt_enrollment WHERE id =". trim($tsid[5]).";");
			$resultStatus = $dao->fetchResult();
			$clsPrevStatus  = $resultStatus->comp_status;
			$enr_master_id = $resultStatus->master_enrollment_id;
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
			expDebug::dPrint("score >>> ". $score, 5);
		/*	$qry3 = "call slp_attendance_details_ins(";
			$qry3 .= "'".trim($tsid[5])."',";
			$qry3 .= "'".trim($tsid[0])."',";
			$qry3 .= "'".trim($tsid[1])."',";
			$qry3 .= "'".trim($tsid[2])."',";
			$qry3 .= "'".trim($tsid[3])."',";
			$qry3 .= "'".trim($tsid[4])."',";
			$qry3 .= "'".trim($status)."',";
			$qry3 .= "'',";// added by yogaraja launch content count issue 35754.
			//$qry3 .= "'".trim($timespend)."',";
			$qry3 .= $score;
			//$qry3 .= "null,";
			$qry3 .= "'".trim($location)."',";
			$qry3 .= "'".trim($cstatus)."',";
			$qry3 .= "'',";
			$qry3 .= "'".trim($vUserId)."',";
			$qry3 .= "null,null,null,";
			$qry3 .= "0,"; // launchflag
			$qry3 .= "'".trim($aicc_data_dump)."',";
			$qry3 .= "'AICC',";
			$qry3 .= "null,";// added by yogaraja launch content count issue 35754.
			$qry3 .= "null,";// added by yogaraja launch content count issue 35754.
			$qry3 .= "'".trim($lrsEnabled)."',";// added by Shobana LRS Content save.
			$qry3 .= "'".trim($totTime)."',";// added by Shobana LRS Content save.
			$qry3 .= "'lrn_cnt_typ_aic',";// added by yogaraja Content code.
			$qry3 .= "'".$progress."',";// added by yogaraja Content progress.
			$qry3 .= "@insval,";
			$qry3 .= "@compstatus)";
			expDebug::dPrint("AICC Handler::updateLMSData - Attendance Update :".$qry3,2);
			$dao->connect();
			$dao->beginTrans();
			$res1 = $dao->execute($qry3);
			$dao->commitTrans();
			$dao->query("select @insval Id,@compstatus CompStatus");
			$result = $dao->fetchResult();
			$dao->closeconnect();*/
			$statements = array('statement' => true);
			$result =lp_attendance_details_ins($tsid[5],$tsid[0],$tsid[1],$tsid[2],$tsid[3],$tsid[4],$status,'',$score,$location,$cstatus,'',$vUserId,'','','',$_POST['launchflag'],$aicc_data_dump,'AICC','','',$lrsEnabled,$totTime,'lrn_cnt_typ_aic',$progress,'',$statements);
			expDebug::dPrint(' Final result attendance details $statments= ' . print_r($statements, true) , 4);
			dbStatementsExecution($statements);
			expDebug::dPrint("crs comp_status from procedure ".$result->CompStatus,5);
			expDebug::dPrint("crs enroll status ".$status, 5);
			expDebug::dPrint("crs content status ".$cstatus,5);
			expDebug::dPrint("crs actual content status ".$stat,5);
			if($result->CompStatus != "Not Changed"){
				if($result->CompStatus == 'Completed' && $clsPrevStatus != 'lrn_crs_cmp_cmp') {
					$aicc_entity_id = empty($enr_master_id)?$tsid[5]:$enr_master_id;
					$aicc_entity_type = empty($enr_master_id) ? 'class' : 'tp';
					insertUserPointsForEachActionsPerformedAICC($vUserId,'complete_class_training',$aicc_entity_id,'',$aicc_entity_type);
					$dao->connect();
					$courseCompletedNotify = "CALL slp_lnr_notification_ins(" .
		  						   		"'course_completed', " .
					$tsid[0] . ", " .
					((!empty($tsid[2]))? $tsid[2] : 'null') . ", " .
		                                "'', " .
		                                "'', " .
					$tsid[0] . ")";
					expDebug::dPrintDBAPI("AICC Handler::updateLMSData Completion Notification : SQL query = ", $courseCompletedNotify,4);
					$dao->query($courseCompletedNotify);				
					$dao->closeconnect();				
				}
				audit_trail_insert($result->CompStatus,$status,$aicc_sid,$vUserId);
	
				expDebug::dPrint("aicc tsid : ".print_r($tsid,true),4);
				if($tincan == 1)
				{
					$lunObj = new stdClass;
					$lunObj->userId = $vUserId;
					$lunObj->calltype = null;
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
					$lunObj->launch_data = 150;
					$lunObj->suspend_data = 150;
					$lunObj->exit = null;
					$lunObj->orgScore = '';
					$lunObj->assessCheck = null;
					$lunObj->survey_id = null;
					$lunObj->prevStatus = $clsPrevStatus;
					$lunObj->contentCode = 'lrn_cnt_typ_aic';
					expDebug::dPrint("AICC lunObj values :".print_r($lunObj,true),5);
					tincanCreateStatement_aicc($lunObj);
				}
		}
		expDebug::dPrint("aicc request--------> : ".print_r($_REQUEST,true),4);
		//73059: Content completion % is not getting updated unless we close the window and reopen it or move to the next content
		if($_POST['launchflag']==0){
			$dao->connect();
			$queryDel = "DELETE FROM slt_aicc_interaction WHERE session_id='".$session_id."'";
			expDebug::dPrint("AICC Handler::updateLMSData DELETE query = " . $queryDel,2);
			$dao->query($queryDel);
			$dao->closeconnect();
		}elseif($_POST['launchflag']==1){
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
			$attsummaryDetails = get_attendance_summary($lunObj->RegId, $lunObj->CourseId, $lunObj->ClassId, $lunObj->LessonId, $lunObj->VersionId, NULL);
			if($attsummaryDetails->progress>0)
				$Current_progress = $attsummaryDetails->progress;
			$lunObj->progress = convertProgressValue($Current_progress);
			$lunObj->content_id = getContentMasterId($lunObj->VersionId);
			$lunObj->tot_progress = convertProgressValue(getMultiLessonDetails($lunObj->VersionId,$lunObj));
			expDebug::dPrint("AICC lunObj values :".print_r($lunObj,true),4);
			echo drupal_json_output($lunObj);
			exit();
		}
		}else{
			expDebug::dPrint("Content window closed before connect get established with LMS",1);
			$result ='';
		}
		syncSolrData('Enrollment');
		syncSolrData('MasterEnrollment');
		syncSolrData('User');
		return $result;
	}catch(Exception $e){
		$dao->rollbackTrans();
		$dao->closeconnect();
		expDebug::dPrint("AICC Handler::updateLMSData Error :".$e->getMessage(),1);
	}
}
function getUserDetails_aicc($userId){
	$dao=new AbstractDAO();
	$dao->connect();
	$dao->query("SELECT per.email AS email, CONCAT(per.first_name,\' \',per.last_name) AS user_firstlast_name	FROM	slt_person per	WHERE  (per.id = $userId)");
	$result = $dao->fetchResult();
	//	$dao->closeconnect();
	return $result;
}

function getAssessmentCount_aicc($classID,$type){
	try {
		$dao=new AbstractDAO();
		$dao->connect();
		$dao->query("SELECT COUNT(1) AS assessment_count FROM slt_survey_mapping surmap WHERE  (surmap.object_id = $classID) AND (surmap.object_type = '$type') AND (surmap.pre_status = 0)");
		$result = $dao->fetchResult();
		return $result;
	} catch (Exception $ex) {
		watchdog_exception('getAssessmentCount_aicc', $ex);
		expertusErrorThrow($ex);
	}
}
function getAttemptCount($versionId,$classId,$userId){
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
function getContentInfo($versionId){
	expDebug::dPrint("Get Count".$dao);
	$dao=new AbstractDAO();
	$dao->connect();
	$selectContent = "select content_master_id as content_id, mst.code as content_code
	from slt_content_version ver
	join slt_content_master mst on mst.id = ver.content_master_id
	where ver.id = $versionId";
	$dao->query($selectContent);
	$contentInfo = $dao->fetchResult();
	expDebug::dPrint("getContentInfo query -> $selectContent", 5);
// 	$contentId = $contentInfo->content_id;
// 	expDebug::dPrint("Content Id ".print_r($contentInfo, 1));
	return $contentInfo;
}
function tincanCreateStatement_aicc($lunObj){
	try{
		if(isset($lunObj->launchflag) && $lunObj->launchflag == 1){
			return;
		}else{
		$url = $_SERVER['HTTP_HOST'];
		$calltype = $lunObj->calltype;
		$attemptCount = getAttemptCount($lunObj->VersionId,$lunObj->ClassId,$lunObj->userId);
		$content_info = getContentInfo($lunObj->VersionId);
		$content_id = $content_info->content_id;
		$content_code = $content_info->content_code;
		expDebug::dPrint("contente id :--> ".$content_id);
		$api_client = TinCanClient::Instance();
		$dao=new AbstractDAO();
		$dao->connect();
		$selectClassInfo = "SELECT  cls.title AS class_name, enr.score AS Score, per.email AS email,per.full_name as fullname,crs.title AS course_name, map.max_attempts AS max_attempts,map.validity_days AS validity_days,
		summ.last_lesson_location AS last_lesson_location,summ.aicc_data AS aicc_data, pro.name AS statusName, enr.comp_status AS comp_status,
		DATE_FORMAT((summ.last_attempt_date),'%d %b %Y %h:%i %p') AS last_attempt_date
		FROM
		slt_course_class cls
		LEFT OUTER JOIN slt_course_template crs ON crs.id=cls.course_id
		LEFT OUTER JOIN slt_enrollment enr ON enr.class_id=cls.id and enr.course_id=cls.course_id and enr.user_id=$lunObj->userId
		LEFT OUTER JOIN slt_person per ON per.id = enr.user_id
		LEFT OUTER JOIN slt_course_content_mapper map ON map.class_id=cls.id and map.course_id=crs.id and map.content_id = $content_id
		LEFT OUTER JOIN slt_content_version vers ON vers.content_master_id = map.content_id and vers.id = $lunObj->VersionId
		LEFT OUTER JOIN slt_attendance_summary summ ON summ.class_id=cls.id and summ.course_id=crs.id and summ.content_version_id = $lunObj->VersionId
		LEFT OUTER JOIN slt_profile_list_items pro ON pro.code=enr.comp_status
		WHERE  (cls.id = $lunObj->ClassId) AND (summ.user_id = $lunObj->userId) AND (summ.lesson_id = $lunObj->LessonId) AND  (summ.content_version_id = $lunObj->VersionId) AND enr.reg_status = 'lrn_crs_reg_cnf' GROUP BY cls.id";
		expDebug::dPrint("tincanCreateStatement_aicc selectClassInfo query $selectClassInfo", 5);
		$dao->query($selectClassInfo);
		$result = $dao->fetchResult();
		expDebug::dPrint("fetch results ".print_r($result, true),5);
		$courseName = $result->course_name;
		$classname = $result->class_name;
		//$summaryCount = $result->summaryCount;
		$max_attempts = $result->max_attempts;

		$getAssessmentCount = getAssessmentCount_aicc($lunObj->ClassId,'cre_sys_obt_cls');
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

		$exploe = explode("|",$aicc_dataArray);
		$string = explode("=",$exploe[1]);
		$last_lesson_location = $string[1];

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
		$dateNow = new DateTime();
		$dateNow->format('u');
		$stored = $dateNow->format('Y-m-d\TH:i:s.u\Z');
		//$actor will be commnon for all the statements
		$actor = array('name' => "$fullName",
				'objectType' => 'Agent',
				'mbox' => "mailto:" . $email,
		);
		$statuses = array('completed', 'passed'); //'failed'
		$content_status = strtolower(trim($lunObj->ContentStatus));
		$completion_status = strtolower(trim($lunObj->CompletionStatus));
		$sidval = "-".$lunObj->LessonId."-".$lunObj->VersionId."-".$lunObj->RegId."-".$lunObj->Location;
		expDebug::dPrint('aicc id'. $sidval, 5);
		$enc1 = new Encrypt();
		$aicc_id = $enc1->encrypt($sidval);
		expDebug::dPrint('aicc id ency'. $aicc_id);
		$completeStatus = getContentCompletionStatus($lunObj, $completion_status);
		$content_result = array(
				'duration' => $lunObj->SessionTime,	//this need to be changed after checking with LRS
				'success' => $completeStatus['contentStatus'],	//true/false
				'completion' => $completeStatus['completionStatus'], //in_array($content_status, $statuses),//true
				'score' => array('raw' => $lunObj->Score,),
		);
		expDebug::dPrint('test here $$content_result'. print_r($content_result, 1));
		$context = array(
				'course_name' => $classname,
				'course_id' => $lunObj->ClassId,
				'content_id' => $content_id,
				'content_name' => $content_code,
				'content_completed' => $completeStatus['contentStatus'], //in_array($content_status, $statuses),
				'course_completed' => in_array($result->comp_status, array('lrn_tpm_ovr_cmp', 'lrn_crs_cmp_cmp')),
				'registration' => $aicc_id,
		);
		expDebug::dPrint('test here $context'. print_r($context, 1));
		$statusDetails = array('actor' => $actor,
								//'result' => $content_result,
								'context' => $context,
								'verb' => array('id' => 'http://adlnet.gov/expapi/verbs/'.$statusName,'display' => array("en-US"=> "$statusName"),),
								'object' => array('type' => 'course','definition' => array('name'=>array('en-US'=>"$classname")),),
								'timestamp'=>$stored,
								'stored'=>$stored,
								);
		$scoreDetails = array('actor' => $actor,
								//'result' => $content_result,
								'context' => $context,
								'verb' => array('id' => 'http://adlnet.gov/expapi/verbs/scored','display' => array("en-US"=> "scored"),	),
								'object' => array('type' => 'course','definition' => array('name'=>array('en-US'=>"$score for $classname")),),
								'timestamp'=>$stored,
								'stored'=>$stored,
								);
		$validityDetails = array('actor' => $actor,
								//'result' => $content_result,
								'context' => $context,
								'verb' => array('id' => 'http://adlnet.gov/expapi/verbs/attempted','display' => array("en-US"=> "attempted"),),
								'object' => array('type' => 'course','definition' => array('name'=>array('en-US'=>"$classname and their Validity is $validity_days $days")),),
								'timestamp'=>$stored,
								'stored'=>$stored,
								);
		$bookDetails = array('actor' => $actor,
							//'result' => $content_result,
							'context' => $context,
							'verb' => array('id' => 'http://adlnet.gov/expapi/verbs/bookmarked','display' => array("en-US"=> "bookmarked"),	),
							'object' => array('type' => 'course','definition' => array('name'=>array('en-US'=>"$classname. The book marked page is $last_lesson_location")),),
							'timestamp'=>$stored,
							'stored'=>$stored,
							);
		$launchDateTime = array('actor' => $actor,
								//'result' => $content_result,
								'context' => $context,
								'verb' => array('id' => 'http://adlnet.gov/expapi/verbs/attempted','display' => array("en-US"=> "attempted"),	),
								'object' => array('type' => 'course', 'definition' => array('name'=>array('en-US'=>"$classname and their launch period is from $FromTime to $toDate")),),
								'timestamp'=>$stored,
								'stored'=>$stored,
								);
		$track = array('actor' => $actor,
						//'result' => $content_result,
						'context' => $context,
						'verb' => array('id' => 'http://adlnet.gov/expapi/verbs/attempted','display' => array("en-US"=> "attempted"),),
						'object' => array('type' => 'course',	'definition' => array('name'=>array('en-US'=>"$classname the $totalAttempts time out of their $max_attempts $attempt")),),
						'timestamp'=>$stored,
						'stored'=>$stored,
						);
		$launchDetails = array('actor' => $actor,
							'result' => $content_result,
							'context' => $context,
							'verb' => array('id' => 'http://adlnet.gov/expapi/verbs/attempted','display' => array("en-US"=> "attempted"),),
							'object' => array('type' => 'course','definition' => array('name'=>array('en-US'=>"$classname and has $attempt_left $attempt left")),),
							'timestamp'=>$stored,
							'stored'=>$stored,
						);
		expDebug::dPrint("launchDetails : ".print_r($launchDetails,true),5);
		$statusDetails = json_encode($statusDetails);
		$scoreDetails = json_encode($scoreDetails);
		$validityDetails = json_encode($validityDetails);
		$bookDetails = json_encode($bookDetails);
		$launchDateTime = json_encode($launchDateTime);
		$track = json_encode($track);
		$launchDetails = json_encode($launchDetails);

		

		if(empty($last_lesson_location) || $last_lesson_location == ''){
			$last_lesson_location = "null";
		}
/*
		expDebug::dPrint('$statusDetails'.var_export($statusDetails, 1));
		expDebug::dPrint('$$scoreDetails'.var_export($scoreDetails, 1));
		expDebug::dPrint('$$validityDetails'.var_export($validityDetails, 1));
		expDebug::dPrint('$$bookDetails'.var_export($bookDetails, 1));
		expDebug::dPrint('$$launchDateTime'.var_export($launchDateTime, 1));
		expDebug::dPrint('$$track'.var_export($track, 1));
		expDebug::dPrint('$$launchDetails'.var_export($launchDetails, 1));
		*/
		if(!empty($statusDetails) && $statusName == 'Completed' && ($lunObj->prevStatus != 'lrn_crs_cmp_cmp')){
			$api_client->create_statements(array('statements' => $statusDetails));
		}sleep(1);
		if((!empty($scoreDetails)) && ($statusName != 'Completed' || $score != 0) && ($result->comp_status == 'lrn_crs_cmp_cmp') && !empty($score) && ($lunObj->prevStatus != 'lrn_crs_cmp_cmp')){
			$api_client->create_statements(array('statements' => $scoreDetails));
		}sleep(1);if(!empty($validityDetails) && $validity_days > 0 && ($lunObj->prevStatus != 'lrn_crs_cmp_cmp' && $lunObj->prevStatus != 'lrn_tpm_ovr_cmp') && ($total_attempts == 1 || empty($total_attempts))){
			$api_client->create_statements(array('statements' => $validityDetails));
		}sleep(1);if((!empty($bookDetails)) && ($last_lesson_location != 'null')){
			$api_client->create_statements(array('statements' => $bookDetails));
		}sleep(1);if(!empty($launchDateTime) && $attempt_left >= 0){
			$api_client->create_statements(array('statements' => $launchDateTime));
		}sleep(1);if(!empty($track) && $attempt_left >= 0){
			$api_client->create_statements(array('statements' => $track));
		}sleep(1);if(!empty($launchDetails) && $attempt_left >= 0){
			$api_client->create_statements(array('statements' => $launchDetails));
		}

		$dao->closeconnect();
		//$dao1->closeconnect();
	  }
	}catch(Exception $e){
		$dao->rollbackTrans();
		$dao->closeconnect();
		expDebug::dPrint("tincanCreateStatement_aicc".$e->getMessage(),1);
	}
}
function updateAiccPreview($session_id){
	$aicc_sid = trim($aicc_sid);
	try{
		$dao=new AbstractDAO();
		$dao->connect();
		$queryDel = "DELETE FROM slt_aicc_interaction WHERE session_id='".$session_id."'";
		expDebug::dPrint("AICC Handler::updateAiccPreview DELETE query = " . $queryDel,2);
		$dao->query($queryDel);
		$dao->closeconnect();
	}catch(Exception $e){
		$dao->rollbackTrans();
		$dao->closeconnect();
		expDebug::dPrint("AICC Handler::updateAiccPreview Error :".$e->getMessage(),1);
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

/*
 * This function update aiic_data with total time spend value.
 * @param are '$aiic_data' and '$total_time_spend'
 */
function checkAiccTotalTimeSpend($aicc_data, $total_time_spend) {
	// Internally store an arguments
	$internal_aicc_data = $aicc_data;
	$data = explode('[Core_Lesson]',$internal_aicc_data);
	$trim_data = explode("\n",$data[0]);
	for($i=0;$i<sizeOf($trim_data);$i++) {
		$tmp = explode('=',$trim_data[$i]);
		if (strtolower($tmp[0]) == "time") {
			if($tmp[1] != $total_time_spend) {
				$internal_aicc_data = str_replace($tmp[1],$total_time_spend, $internal_aicc_data);
			}
		}
	}
	return $internal_aicc_data;
}

function insertUserPointsForEachActionsPerformedAICC ($userId,$code,$entityId = 0,$operationCode = '',$entityType = 'others') {
	if($operationCode == '' || $operationCode == 'insert') {
		expDebug::dPrint("soun test inside");
		$dao=new AbstractDAO();
		$dao->connect();
		$dao->query("select points as upoints from slt_master_points where code like '%".$code."%'");

		$result = $dao->fetchResult();
		$dao->closeconnect();
		$userPoints= $result->upoints;
		expDebug::dPrint("soun test".$points,5);
			
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
		expDebug::dPrint("AICC Handler::Audit trail Update :".$insertSql,2);
		$dao->commitTrans();
		$dao->closeconnect();
	}
	else if($operationCode == 'delete'){
			
		$dao=new AbstractDAO();
		$dao->connect();
		$qry = "select id from slt_user_points where action_code = '$code' AND entity_id=$entityId AND user_id=$userId AND operation_flag='insert'";
		//$dao->query("select id from slt_user_points where action_code = '$code' AND entity_id='$entityId' AND user_id='$userId' AND operation_flag='insert'");
		if($entityType != 'others') {

			$qry .= " AND entity_type='$entityType'";
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
		expDebug::dPrint("AICC Handler::captureData - Insert/Update Query :".$update,2);
		$res = $dao->execute($update);
		$dao->closeconnect();
	}
}


function setSessionAiccMobile($type,$aicc_sid = 0){

	expDebug::dPrint('callfunction2321woi');

	if($type=="set"){
		//$current_session = session_id();


		expDebug::dPrint("aiccsid_aiccsid_withoutdecrypt".$aicc_sid,4);
		//$enc1 = new Encrypt();
		//$aicc_sid = $enc1->decrypt($aicc_sid);
		$sid = explode("-",$aicc_sid);
		$current_session = 'T0tcGFYI_x8DrEVwvbRiV1hos5AJpcYT5eRk6E_2b'.$sid[0].'_'.$sid[1].'_'.$sid[2].'_'.$sid[3].'_'.$sid[4].'_'.$sid[5];
		expDebug::dPrint("current_sessioncurrent_sessioncurrent_sessioncurrent_session".$current_session,4);
		expDebug::dPrint("aiccsid_aiccsid".$aicc_sid,4);
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

?>