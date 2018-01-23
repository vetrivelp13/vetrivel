<?php

/*
 * join_meeting_attendee() - Returns the attendee URL to Join the Meeting.
 */
function join_meeting_attendee($class_id){
  $nodeSelect = db_select('slt_node_learning_activity', 'entity');
  $nodeSelect->innerjoin('node', 'n', 'entity.node_id=n.nid');
  $nodeSelect->addField('n','nid','nid');
  $nodeSelect->addField('n','vid','vid');
  $nodeSelect->condition('entity.entity_id', $class_id);
  $nodeSelect->condition('entity.entity_type', 'meeting');
  $nodeResult =  $nodeSelect->execute()->fetchAll();
  expDebug::dPrintDBAPI("Node list QUERY ::",$nodeSelect);
  $node = node_load($nodeResult[0]->nid,$nodeResult[0]->vid);
  $logoutURL = variable_get('bbb_content_type_logoutURL_'   . $node->type, '');
  if(stripos($logoutURL,'http')=== false){
  	$siteURL = getConfigValue('meeting_url');
  	variable_set('bbb_content_type_logoutURL_'   . $node->type, $siteURL.$logoutURL);
  }
  bbb_meeting_attend($node);
}


/*
 * join_meeting_presenter() - Returns the Presenter/Moderate URL to Join the Meeting.
 */
function join_meeting_presenter($class_id){
  $nodeSelect = db_select('slt_node_learning_activity', 'entity');
  $nodeSelect->innerjoin('node', 'n', 'entity.node_id=n.nid');
  $nodeSelect->addField('n','nid','nid');
  $nodeSelect->addField('n','vid','vid');
  $nodeSelect->condition('entity.entity_id', $class_id);
  $nodeSelect->condition('entity.entity_type', 'meeting');
  $nodeResult =  $nodeSelect->execute()->fetchAll();
  expDebug::dPrintDBAPI("Node QUERY ::",$nodeSelect);
  $node = node_load($nodeResult[0]->nid,$nodeResult[0]->vid);
  $logoutURL = variable_get('bbb_content_type_logoutURL_'   . $node->type, '');
  if(stripos($logoutURL,'http')=== false){
  	$siteURL = getConfigValue('meeting_url');
  	variable_set('bbb_content_type_logoutURL_'   . $node->type, $siteURL.$logoutURL);
  }
  bbb_meeting_moderate($node);
}


/*
 * join_meeting() - To Check the valid session to launch.
 */
function join_meeting($urlType,$classId,$sessionId=''){
	expDebug::dPrint('METTING SESSION ID --->'.print_r($sessionId,1),4);
    if($urlType == 'attendee') {
      join_meeting_attendee($classId);
    }else{
      //Multi instructor feature related changes
      if($sessionId!=''){
	      $loggedUserId = getSltpersonUserId();
	      $Select = db_select('slt_session_instructor_details', 'ses_ins');
	      $Select->addField('ses_ins','is_presenter','is_presenter');
	      $Select->condition('ses_ins.session_id', $sessionId,'=');
	      $Select->condition('ses_ins.class_id', $classId,'=');
	      $Select->condition('ses_ins.instructor_id', $loggedUserId,'=');
	      $Select =  $Select->execute()->fetchAll();
	      if($Select[0]->is_presenter =='Y')
	      	join_meeting_presenter($classId);
	      else
	      	join_meeting_attendee($classId);
      }else
      	join_meeting_presenter($classId);

      expDebug::dPrint('METTING SESSION ID RESULT--->'.print_r($Select,1),4);
    }
}


/*
 * join_webex_meeting() - To Check the valid session to launch.
 */
function join_webex_meeting($urlType,$classId,$sessionId=''){
  global $base_url;
  $loggedUserId = getSltpersonUserId();
  $field = array('email','full_name');
  $details  = getPersonDetails($loggedUserId,$field);
  $fullName = $details['full_name'];
  $email = $details['email'];
  //header('Location: https://indiafreetrial.webex.com/indiafreetrial/m.php?AT=JM&MK='.$meetingId.'&AN='.$fullName.'&AE='.$email.'&PW=test123');
  //$url = getConfigValue('webex_url').'/w.php?AT=GS&MK='.$meetingId;

  $cnfUrl = variable_get('webex_url');
  $expUrl = explode('/', $cnfUrl);
  $sliceUrl = array_slice($expUrl,0,-1);
  $impUrl = implode('/',$sliceUrl);

  if((variable_get('webex_url') == '') && $urlType == 'presenter'){
  	expertus_deliver_html_page('Expertus-Message',t('MSG763'));
  	return;
  }
  expDebug::dPrint('webex url' .$cnfUrl,5);
  $expUrl = explode('/', $cnfUrl);
  $sliceUrl = array_slice($expUrl,0,-1);
  $impUrl = implode('/',$sliceUrl);
  $url = $impUrl.'/p.php';

  	$result = getmeetingdetails($classId,$sessionId);
	expDebug::dPrint("Meeting id for JM " .print_r($result,true),5);

  	$meeting=$result[0]->meeting_id; // existing meeting id

  	$presenter=session_presenter($classId,$sessionId,$loggedUserId);
  	expDebug::dPrint("Presenter details " .print_r($presenter,true),5);

  	if($presenter[0]->is_presenter == 'Y' && $urlType == 'presenter' && ($meeting == null || $meeting == '')){
  		try{
  	if(variable_get('webex_User')==1){
  	$result = getwebexcredentials($loggedUserId);
	$username = $result[name];
  	$password = $result[pass];

  	if($username == null || $username == '' || $password == null || $password == ''){
  		$userid=
  		meeting_audit_entry($loggedUserId,'login failed',$sessionId,'webex credentials not configured');
		expertus_deliver_html_page('Expertus-Message',t('MSG760'));
  		return;
  	}
  	}else{
  		$username = variable_get('webex_name');
  		$password = variable_get('webex_pass');
  	}

  	$data = array('AT' => 'LI', // Login
  								'WID' => $username,
  								'PW' =>  $password,
  								'MU' => $base_url.'/?q=learning/webex/meeting/login/'.$classId.'/'.$sessionId,
  					  			'BU' => $base_url.'/?q=learning/webex/meeting/fail/'.$sessionId,
  					);
  	presenter_login($url,$data);
  	}catch(Exception $e){
	  	expDebug::dPrint("WebEx meeting Status check Error ".print_r($e,true),4);
	  }
  	}else{
  try{
  	$url = $impUrl.'/w.php'; // check meeting status
  	$fields_string = 'AT=GS&MK='.$meeting;
	$util=new GlobalUtil();
	$config=$util->getConfig();
	$peerVerify = $config['peer_verify'] == 0 ? FALSE : TRUE;
	$context = stream_context_create(array(
		'http' => array(
				'method' => 'POST',
				'header' => 'Content-Type: application/x-www-form-urlencoded',
				'content' => $fields_string,
				),
		'ssl' => array('verify_peer'=>$peerVerify,
           'verify_peer_name'=>$peerVerify)
	));
  	expDebug::dPrint("WebEx meeting Status check URL ".print_r($url,true),4);
  	expDebug::dPrint("WebEx meeting Status check fields_string ".print_r($fields_string,true),4);

  	$getresult = file_get_contents($url,false,$context);
  	expDebug::dPrint("WebEx meeting Status check response ".print_r($getresult,true),4);
	}catch(Exception $e){
  		expDebug::dPrint("WebEx meeting Status check Error ".print_r($e,true),4);
  	}

  	$token =  stristr($getresult,"&SS=");
  	$splitToken = explode('&',$token);
  	$status = '';
  	foreach($splitToken as $values){
  		if(stristr($values,"SS=")){
  			$statusToken = explode('SS=',$values);
  			$status = $statusToken[1];
  		}
  	}
  	$pass=$result[0]->webex_meeting_password;
  	expDebug::dPrint("Meeting status" .$status,5);
  	if($status == 'INPROGRESS'){
  		$data = array('AT' => 'JM', // Join meeting
  				'MK' => $meeting, // meeting id
  				'AN' => $fullName,
  				'AE' => $email,
  				'PW' => $pass,
  				'BU' => $base_url
  		);
  		$webUrl = $impUrl.'/m.php';
  		if($urlType == 'attendee') {
  			$data['BU'] .= '/?q=webex/meeting/end/attendee/'.$classId.'/'.$sessionId;
  			connect_webex_meeting($webUrl,$data);
  		}else{
  			$data['BU'] .= '/?q=webex/meeting/end/presenter/'.$classId.'/'.$sessionId;
  			connect_webex_meeting($webUrl,$data);

  }}else if($presenter[0]->is_presenter != 'Y'){
    //print t('MSG525').'.';
    expertus_deliver_html_page('Expertus-Message',t('MSG525'));
  }
  else {
	  	$webUrl = $impUrl.'/p.php';

  		if(variable_get('webex_User')==1){
  		$result = getwebexcredentials($loggedUserId);
  		$username = $result[name];
  		$password = $result[pass];
		
		if($username == null || $username == '' || $password == null || $password == ''){
		meeting_audit_entry($loggedUserId,'login failed',$sessionId,'webex credentials not configured');
  		expertus_deliver_html_page('Expertus-Message',t('MSG760'));
  		return;
  	}

  	}else{
  		$username = variable_get('webex_name');
  		$password = variable_get('webex_pass');
  	}

  	$data = array('AT' => 'LI',
  								'WID' => $username,
  								'PW' => $password,
  								'MU' => $base_url.'/?q=learning/webex/meeting/login/'.$classId.'/'.$sessionId,
  					  			'BU' => $base_url.'/?q=learning/webex/meeting/fail/'.$sessionId,
  					);
  	presenter_login($webUrl,$data);

  }
 }
}

function connect_webex_meeting($url,$post_data){
	expDebug::dPrint("WebEx Meeting connect to URL ".print_r($url,true),4);
	expDebug::dPrint("WebEx Meeting connect to Post Data  ".print_r($post_data,true),4);
	?>
		<form action="<?php echo $url; ?>" Method="POST" style="height:1px; overflow:hidden;">
		<input type="text" name="AT" value="<?php echo $post_data['AT']; ?>"/>
		<input type="text" name="AN" value="<?php echo $post_data['AN']; ?>"/>
		<input type="text" name="AE" value="<?php echo $post_data['AE']; ?>"/>
		<input type="text" name="PW" value="<?php echo $post_data['PW']; ?>"/>
		<input type="text" name="MK" value="<?php echo $post_data['MK']; ?>"/>
		<input type="text" name="BU" value="<?php echo $post_data['BU']; ?>"/>
		<input type="submit" name="join" id="WebExSubmit">
		</form>
		<script> document.getElementById("WebExSubmit").click(); </script>
	<?php
}

/*
 * join_live_meeting() - To Check the valid session to launch.
 */
function join_live_meeting($urlType,$classId,$meetingId,$pw,$sessionId=''){
  //Presenter: https://www.livemeeting.com/cc/microsoft/join?id=6HSKMD&role=present&pw=Q%7D%2C3%7Cdm%25p
  //Attendee:  https://www.livemeeting.com/cc/microsoft/join?id=6HSKMD&role=attend&pw=whRKh%3B94X
  if($urlType == 'attendee') {
    header('Location: '.getConfigValue('live_meeting_url').'?id='.$meetingId.'&role=attend&pw='.$pw);
  }else{
  	//Multi instructor feature related changes
  	if($sessionId!=''){
  		$loggedUserId = getSltpersonUserId();
  		$Select = db_select('slt_session_instructor_details', 'ses_ins');
  		$Select->addField('ses_ins','is_presenter','is_presenter');
  		$Select->condition('ses_ins.session_id', $sessionId,'=');
  		$Select->condition('ses_ins.class_id', $classId,'=');
  		$Select->condition('ses_ins.instructor_id', $loggedUserId,'=');
  		$Select =  $Select->execute()->fetchAll();
  		if($Select[0]->is_presenter =='Y')
  			header('Location: '.getConfigValue('live_meeting_url').'?id='.$meetingId.'&role=present&pw='.$pw);
  		else
  			header('Location: '.getConfigValue('live_meeting_url').'?id='.$meetingId.'&role=attend&pw='.$pw);
  	}else
  		header('Location: '.getConfigValue('live_meeting_url').'?id='.$meetingId.'&role=present&pw='.$pw);
  }
}


/*
 * end_meeting()
 */
function expertus_end_meeting($mode,$classId){
    $upDet = updateMeetingDetails($classId);
	// to close the expertus meeting window after meeting has completed.
	$closeWindow = "<script language='JavaScript'>window.close();</script>";
	print $closeWindow;
}

/*
 * webex_end_meeting()
 */
function webex_end_meeting($mode,$classId,$sessionId){
    if($mode == 'attendee'){
      $upDet = updateMeetingDetails($classId);
    }else{
    	$upd = updateSessionDetails($classId,$sessionId);
    }
	$closeWindow = "<script language='JavaScript'>window.close();</script>";
	print $closeWindow;
}

/*
 * updateMeetingDetails()
 */
function updateMeetingDetails($classId){
  $loggedUserId = getSltpersonUserId();
  $updateEnroll = db_update('slt_enrollment');
  $updateEnrollFld = array(
                    'comp_status' => 'lrn_crs_cmp_att',
                    'updated_by' => $loggedUserId,
                    'updated_on' => now()
  );
  $updateEnroll->fields($updateEnrollFld);
  $updateEnroll->condition('class_id', $classId);
  $updateEnroll->condition('user_id', $loggedUserId);
  $updateEnroll->condition('reg_status', 'lrn_crs_reg_cnf');

  expDebug::dPrintDBAPI("updateMeetingDetails ----- : ",$updateEnroll);
  $updateEnroll->execute();
  
  syncSolrData('Enrollment');
}


  /*

  $insertAttDet = db_insert('slt_attendance_details');
  $insertAttDetFld = array(
                    'enrollment_id' => 1,
                    'user_id' => $loggedUserId,
                    'course_id' => 1,
                    'class_id' => $classId,
                    'lesson_id' => 1,
                    'status' => 'node',
                    'attempt_number' => 'node',
                    'attempt_date' => 'node',
                    'time_spend' => 'node',
                    'score' => 'node',
                    'grade' => 'node',
                    'launch_data' => 'node',
                    'suspend_data' => 'node',
                    'cmiexit' => 'node',
                    'aicc_data' => 'node'
                    );
  $insertAttDet->fields($insertAttDetFld);
  $insertAttDet->execute();

  $select = db_select('slt_attendance_summary', 'sas');
  $select->addField('sas','id','sid');
  $select->condition('sas.id',$summaryId,'=');
  $sumId = $select->execute()->fetchField();

  if( 1 == 1 ) {
    $insertAttSum = db_insert('slt_attendance_summary');
    $insertAttSumFld = array(
                      'enrollment_id' => 1,
                      'user_id' => 1,
                      'course_id' => 1,
                      'class_id' => 1,
                      'lesson_id' => 1,
                      'status' => 'node',
                      'total_attempts' => 'node',
                      'last_attempt_date' => 'node',
                      'total_time_spend' => 'node',
     				  'last_lesson_location' => 'node',
                      'score' => 'node',
                      'grade' => 'node',
                      'launch_data' => 'node',
                      'suspend_data' => 'node',
                      'cmiexit' => 'node',
                      'aicc_data' => 'node'
                      );
    $insertAttSum->fields($insertAttSumFld);
    $insertAttSum->execute();
  } else {
    $updateAttSum = db_update('slt_attendance_summary');
    $updateAttSumFld = array(
                      'enrollment_id' => 1,
                      'user_id' => 1,
                      'course_id' => 1,
                      'class_id' => 1,
                      'lesson_id' => 1,
                      'status' => 'node',
                      'total_attempts' => 'node',
                      'last_attempt_date' => 'node',
                      'total_time_spend' => 'node',
     				  'last_lesson_location' => 'node',
                      'score' => 'node',
                      'grade' => 'node',
                      'launch_data' => 'node',
                      'suspend_data' => 'node',
                      'cmiexit' => 'node',
                      'aicc_data' => 'node'
    );
    $updateAttSum->fields($updateAttSumFld);
    $updateAttSum->condition('id', $classId);
    $updateAttSum->execute();
  }
*/





/*
  $query = "call slp_attendance_details_ins($lunObj->RegId,$lunObj->LearnerId,$lunObj->CourseId,$lunObj->ClassId,
  $lunObj->LessonId,$lunObj->Status,$lunObj->SessionTime,$lunObj->Score,$lunObj->Grade,$lunObj->Location,
  $lunObj->ContentStatus,$lunObj->userId,$lunObj->launch_data,$lunObj->suspend_data,$lunObj->exit,null,
  $lunObj->calltype,@insval,@compstatus)";
  $dao = new AbstractDAO();
  $dao->connect();
  $dao->beginTrans();
  expDebug::dPrint(' SQL query = ' . $query , 4);
  $dao->execute($query);
  $dao->commitTrans();
  $dao->query("select @insval Id,@compstatus CompStatus");
  $result = $dao->fetchResult();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 4);
  $dao->closeconnect();
  return $result;
*/

function login_validate($classId,$sessionId=''){
	global $base_url;
	$userid=getSltpersonUserId();
	
	$ref_url = $_SERVER['REQUEST_URI'];
	$token =  stristr($ref_url,"&CSRF=");
	$CSRFToken = explode('&',$token);
	$status = '';
	$meetingid = '';
	foreach($CSRFToken as $values){
		if(stristr($values,"CSRF=")){
			$statusToken = explode('CSRF=',$values);
			$_SESSION['CSRF'] = $statusToken[1];
			$CSRF=$_SESSION['CSRF'];
		}
	}
	meeting_audit_entry($userid,'login',$sessionId,'successfull login to webex');
	$result = getmeetingdetails($classId,$sessionId);
	// when user has joined/started the meeting and closed the session. Join/Start the same meeting.
	if(($result[0]->meeting_id != '' || $result[0]->meeting_id != null) && ($result[0]->webex_meeting_password != '' || $result[0]->webex_meeting_password != null)){
		get_meetingkey($classId,$sessionId,$result[0]->meeting_id);
	}
	else{
	$cnfUrl = variable_get('webex_url');
	$expUrl = explode('/', $cnfUrl);
	$sliceUrl = array_slice($expUrl,0,-1);
	$impUrl = implode('/',$sliceUrl);
	$url = $impUrl.'/m.php';

	//$url = "https://indiafreetrial.webex.com/indiafreetrial/m.php";
	$backurl = $base_url.'/?q=learning/webex/schedule/'.$classId.'/'.$sessionId;
	expDebug::dPrint("url schedule" .$url,5);

	$sessdetails = db_select('slt_course_class_session','ses');
	$sessdetails->addfield('ses','title','title');
	$sessdetails->addfield('ses','start_date','start_date');
	$sessdetails->addfield('ses','start_time','start_time');
	$sessdetails->addfield('ses','end_time','end_time');
	$sessdetails->innerjoin('slt_profile_list_items', 'spf', 'ses.timezone=spf.code');
	$sessdetails->addfield('spf','attr2','phptimezone');
	$sessdetails->addfield('spf','attr3','timezone');
	$sessdetails->condition('ses.id', $sessionId);
	$sessdetails->condition('ses.class_id', $classId);

	$sessresult = $sessdetails->execute()->fetchAll();
  	expDebug::dPrint('result' .print_r($sessresult,true),5);

  	$MN=$sessresult[0]->title;


  	$date = $sessresult[0]->start_date;

  	$timestamp = strtotime($date);
  	$N_date = date('Y-m-d', $timestamp);
  	//$N_time = date('H:i:s', $timestamp);
  	$dt = explode('-',$N_date);

  	$DA=$dt[2];
  	$MO=$dt[1];
  	$YE=$dt[0];

  	$phptimezone = $sessresult[0]->phptimezone;
  	date_default_timezone_set($phptimezone);
  	$current_time=date("H:i:s");
  	$current_time_plus=strtotime("$current_time + 3min");
  	$starttime = $sessresult[0]->start_time;
  	if(strtotime($starttime) > $current_time_plus){
  		$time=$starttime;
  	}else{
  		$time_new=$current_time_plus;
  		$time = date('H:i:s', $time_new);
  		expDebug::dPrint("time" .$time,5);
  	}

  	$time_ses = strtotime($time);
  	$N_time = date('H:i:s', $time_ses);
  	$start = explode(':',$N_time);
  	$HO=$start[0];
  	$MI=$start[1];

  	$timezone=$sessresult[0]->timezone;

  	$TZ=ltrim($timezone,"webex_");

  	$PW=time();

  	$pass_update=db_update('slt_course_class_session');
  	$pass_update->fields(array('webex_meeting_password'=>$PW));
  	$pass_update->condition('id', $sessionId);
  	$pass_update->condition('class_id', $classId);
  	$pass_update->execute();

  	expDebug::dPrint('timezone' .$TZ,5);
  	expDebug::dPrint('Date' .$DA,5);
  	expDebug::dPrint('Month' .$MO,5);
  	expDebug::dPrint('Year' .$YE,5);
  	expDebug::dPrint('HO' .$HO,5);
  	expDebug::dPrint('MI' .$MI,5);

  	$session_start = strtotime($starttime);
  	$N_start_time = date('H:i:s', $session_start);

  	$start_date_concat = $N_date." ".$N_start_time;
  	expDebug::dPrint('$$start_date_concat' .$start_date_concat,5);
  	expDebug::dPrint('$endtime' .$sessresult[0]->end_time);
  	$end_date_concat = $N_date." ".$sessresult[0]->end_time.":00";
  	expDebug::dPrint('$endtime' .$end_date_concat,5);

  	$new_start_date = new DateTime($start_date_concat);
  	$time_diff = $new_start_date->diff(new DateTime($end_date_concat));

  	$minutes += $time_diff->h * 60;
  	$minutes += $time_diff->i;
  	$DU +=  $minutes;

  	expDebug::dPrint('session duration' .$DU,5);
  	if(strpos($url, 'freetrial') !== false) // if the account is freetrial teleconference value should be 1 for paid account value should be 3
  		$tc = 1;
  	else 
  		$tc = 3;
  	expDebug::dPrint('Teleconference type ' .$tc,5);
  	?>
<div style="visibility:hidden;">
<form action="<?php echo $url?>" Method="POST">
<input type="text" name="AT" value="SM"/>
<input type="text" name="PW" value="<?php echo $PW?>"/>
<input type="text" name="MN" value="<?php echo $MN?>"/>
<input type="text" name="DA" value="<?php echo $DA?>"/>
<input type="text" name="MO" value="<?php echo $MO?>"/>
<input type="text" name="YE" value="<?php echo $YE?>"/>
<input type="text" name="HO" value="<?php echo $HO?>"/>
<input type="text" name="MI" value="<?php echo $MI?>"/>
<input type="text" name="TZ" value="<?php echo $TZ?>"/>
<input type="text" name="DU" value="<?php echo $DU?>"/>
<input type="text" name="CSRF" value="<?php echo $CSRF?>"/>
<input type="text" name="TC" value="<?php echo $tc?>"/>
<input type="text" name="BU" value="<?php echo $backurl?>"/>
<input type="submit" name="Schedule" id="Schedule">
</form>
</div>
  			<script> document.getElementById("Schedule").click(); </script>
  		<?php
}
}
function get_meetingkey($classId,$sessionId,$meeting_id = '')
{
	global $base_url;
	$userid=getSltpersonUserId();
	$cnfUrl = variable_get('webex_url');
	expDebug::dPrint('webex url' .$cnfUrl,5);
	$expUrl = explode('/', $cnfUrl);
	$sliceUrl = array_slice($expUrl,0,-1);
	$impUrl = implode('/',$sliceUrl);
	$url = $impUrl.'/m.php';

	$backurl = $base_url.'/?q=webex/meeting/end/presenter/'.$classId.'/'.$sessionId;

	if($meeting_id != null || $meeting_id != ''){
		//$meetingid = $meeting_id;
		$data = array('meetingid' => $meeting_id,
				'BU' => $backurl,
		);
		host_meeting($url,$data);
	}else {
	$ref_url = $_SERVER['REQUEST_URI'];
    $token =  stristr($ref_url,"&ST=");
    $splitToken = explode('&',$token);
    expDebug::dPrint("split token" .print_r($splitToken,true),5);
    $status = '';
    $meetingid = '';
    foreach($splitToken as $values){
    	if(stristr($values,"ST=")){
    		$statusToken = explode('ST=',$values);
    		$status = $statusToken[1];
    		expDebug::dPrint("Status return" .$status,5);
    	}if(stristr($values,"MK=")){
    		$meetingidreturn = explode('MK=',$values);
    		$meetingid = $meetingidreturn[1];
    		expDebug::dPrint("Meeting id return" .$meetingid,5);
    	}
    }
    	if($status=="SUCCESS"){
    	meeting_audit_entry($userid,'schedule',$sessionId,'meeting scheduled successfully ' .$meetingid);
    	$updatesess = db_update('slt_course_class_session');
    	$updatefields = array('meeting_id' => $meetingid);
    	$updatesess->fields($updatefields);
    	$updatesess->condition('class_id',$classId);
    	$updatesess->condition('id',$sessionId);
    	expDebug::dPrintDBAPI("Update meeting id", $updatesess,4);
    	$updatesess->execute();

    	$cnfUrl = variable_get('webex_url');
    	$expUrl = explode('/', $cnfUrl);
    	$sliceUrl = array_slice($expUrl,0,-1);
    	$impUrl = implode('/',$sliceUrl);
    	$url = $impUrl.'/m.php';

    	$data = array('meetingid' => $meetingid,
    			'BU' => $backurl,
    	);
    	host_meeting($url,$data);
}else{
	meeting_audit_entry($userid,'schedule fail',$sessionId,'meeting not scheduled');
	expertus_deliver_html_page('Expertus-Message',t('ERR057'));
}
}
}

function webex_fail($url,$sessionid=''){
	if($url=='fail'){
	$userId=getSltpersonUserId();
	meeting_audit_entry($userId,'login fail',$sessionid,'invalid webex credentials');
	expertus_deliver_html_page('Expertus-Message',t('MSG762'));
}else{
	expertus_deliver_html_page('Expertus-Message',t('ERR057'));
}
}
function session_presenter($classId,$sessionId,$userId)
{
	$select=db_select('slt_session_instructor_details','ins');
	$select->addfield('ins','is_presenter','is_presenter');
	$select->condition('ins.session_id',$sessionId);
	$select->condition('ins.class_id',$classId);
	$select->condition('instructor_id',$userId);
	$result=$select->execute()->fetchAll();
	return $result;
	expDebug::dPrint("Presenter result" .print_r($result,true),5);
}

function presenter_login($url,$data){
	expDebug::dPrint('login presenter data' .print_r($data,true),5);
	?>          <div style="visibility:hidden;">
	  			<form action="<?php echo $url; ?>" Method="POST" style="height:1px; overflow:hidden;">
	  			<input type="text" name="AT" value="<?php echo $data['AT']; ?>"/>
	  			<input type="text" name="WID" value="<?php echo $data['WID']; ?>"/>
	  			<input type="text" name="PW" value="<?php echo $data['PW']; ?>"/>
	  			<input type="text" name="MU" value="<?php echo $data['MU']; ?>"/>
	  			<input type="text" name="BU" value="<?php echo $data['BU']; ?>"/>
	  			<input type="submit" name="login" id="login">
	  			</form>
	  			</div>
	  			<script> document.getElementById("login").click(); </script>
	  		<?php
}

function host_meeting($url,$data){
	$CSRF=$_SESSION['CSRF'];
	expDebug::dPrint('login presenter url' .$url,5);
	expDebug::dPrint('login presenter data' .print_r($data,true),5);
	?>
				    	<div style="visibility:hidden;">
				    	<form action="<?php echo $url?>" Method="POST">
				    	<input type="text" name="AT" value="HM"/>
				    	<input type="text" name="MK" value="<?php echo $data['meetingid'];?>"/>
				    	<input type="text" name="CSRF" value="<?php echo $CSRF;?>"/>
				    	<input type="text" name="BU" value="<?php echo $data['BU'];?>"/>
				    	<input type="submit" name="Host" id="Host">
				    	</form>
				    	</div>
				    	<script> document.getElementById("Host").click(); </script>
				   	  		<?php
}

function updateSessionDetails($classId,$sessionId){
	$userId=getSltpersonUserId();
	$update = db_update('slt_session_instructor_details');
	$update->fields(array('launch_flag'=>'1'));
	$update->condition('session_id', $sessionId);
	$update->condition('class_id', $classId);
	$update->condition('instructor_id', $userId);
	expDebug::dPrintDBAPI('meeting launch update' ,$update);
	$update->execute();
}

function getmeetingdetails($classId,$sessionId){
	$meetingid=db_select('slt_course_class_session','se');
	$meetingid->addfield('se','meeting_id','meeting_id');
	$meetingid->addfield('se','webex_meeting_password','webex_meeting_password');
	$meetingid->condition('se.class_id',$classId);
	$meetingid->condition('se.id',$sessionId);
	$result=$meetingid->execute()->fetchAll();
	return $result;
}

function getwebexcredentials($userID){
	$fetchins=db_select('slt_person','pe');
  	$fetchins->addfield('pe','webex_name','name');
  	$fetchins->addfield('pe','webex_pass','pass');
  	$fetchins->condition('pe.id',$userID);
  	$result=$fetchins->execute()->fetchAll();
	$username = $result[0]->name;
	$enc1 = new Encrypt();
	$password = $enc1->decrypt($result[0]->pass);
	$result=array('name'=>$username,'pass'=>$password);
	return $result;
}
/*
 * meeting_audit_entry() - This function checks for changed values and makes audit trail entries
* Added by Indhuja Feb 01,2017
*/
function meeting_audit_entry($userId, $action, $fields, $status) {
	try{
	add_audit_trail_entry($userId, $fields, 'lrn_cls_vct_web', 'exp_sp_meeting', 'webex meeting', $action,
		  		$userId, null, $status,null,null,null,null,$activity_id);
	}catch (Exception $ex) {
		watchdog_exception('meeting_audit_entry', $ex);
		expertusErrorThrow($ex);
	}
}

?>
