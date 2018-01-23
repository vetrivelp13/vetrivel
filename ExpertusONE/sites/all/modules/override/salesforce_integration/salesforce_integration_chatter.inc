<?php

/*
 * do_curl_get() - GET Method : Get salesforce details by sending oauth token details using curl  
*/

function do_curl_get($oAuth_token,$url,$params){	
	
	try{	
		
		if(empty($oAuth_token) || empty($url)){
			return NULL;
		}		
		$qString='';
		if(isset($params)){
			$qString=http_build_query($params);
		}
		
		$curl_req_qry_str=$oAuth_token['instance_url'].$url.'?'.$qString;
		
		expDebug::dPrint('$curl_req_qry_str = ' . print_r($curl_req_qry_str, true),4);
		
		$curl = curl_init($curl_req_qry_str);
		
		//$curl = curl_init($oAuth_token['instance_url'].$url.'?'.$qString);
		
		curl_setopt($curl, CURLOPT_HEADER, false);
		curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
		curl_setopt($curl, CURLOPT_HTTPHEADER,array("Authorization: ".$oAuth_token['token_type']." ".$oAuth_token['access_token']));
		curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, false);
		$curl_response = curl_exec($curl);
		
		expDebug::dPrint('$curl_response = ' . print_r($curl_response, true),4);
		
		$status = curl_getinfo($curl, CURLINFO_HTTP_CODE);
		
		expDebug::dPrint('$curl status = ' . print_r($status, true),4); 
		 
	    expDebug::dPrint("curl status desc : call to token URL $url failed with status $status, response $curl_response, curl_error " . curl_error($curl) . ", curl_errno " . curl_errno($curl),5);
		
		curl_close($curl);
		if(empty($curl_response)) return NULL;
		return json_decode($curl_response,true);
			
	}catch (Exception $ex) {
		watchdog_exception('do_curl_get', $ex);
		expertusErrorThrow($ex);
	} 
	
}

/*
 * do_curl_post() - POST Method : Get salesforce details by sending oauth token details using curl
*/

function do_curl_post($oAuth_token,$url,$post_params){
	
	try{
		
		if(empty($oAuth_token) || empty($url)){
		
			return NULL;
		}
		$post_field_string='';
		if(isset($post_params)){
			$post_field_string=http_build_query($post_params);
		}
		
		$curl_post_url=$oAuth_token['instance_url'].$url;
		
		expDebug::dPrint('$curl_post_url = ' . print_r($curl_post_url, true),4);
		expDebug::dPrint('$post_field_string = ' . print_r($post_field_string, true),4);
		
		$curl = curl_init($curl_post_url);
		
		//$curl = curl_init($oAuth_token['instance_url'].$url);
		
		curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
		curl_setopt($curl, CURLOPT_CONNECTTIMEOUT, 10);
		curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_setopt($curl, CURLOPT_POSTFIELDS, $post_field_string);
		curl_setopt($curl, CURLOPT_POST, true);
		curl_setopt($curl, CURLOPT_HEADER, false);
		curl_setopt($curl, CURLOPT_HTTPHEADER,array("Authorization: ".$oAuth_token['token_type']." ".$oAuth_token['access_token']));
		$curl_response = curl_exec($curl);
		
		expDebug::dPrint('$curl_response = ' . print_r($curl_response, true),4);
		
		$status = curl_getinfo($curl, CURLINFO_HTTP_CODE);
		
		expDebug::dPrint('$curl status = ' . print_r($status, true),4);
		
		expDebug::dPrint("curl status desc : call to token URL $url failed with status $status, response $curl_response, curl_error " . curl_error($curl) . ", curl_errno " . curl_errno($curl),5);
		
		curl_close($curl);
		if(empty($curl_response)) return NULL;
		return json_decode($curl_response,true);
			
	}catch (Exception $ex) {
		watchdog_exception('do_curl_post', $ex);
		expertusErrorThrow($ex);
	} 

}

/*
 * get_sf_oauth_token_response() - Get oauth token response by sending default salesforce credential, which supports the chatter feed posting
 * $is_default_sf_org="1", it takes credential from default in DB, 
 * $is_default_sf_org="2" then details takes from session, which is used for chatter option 
 * $sf_org_det_arr , which contains the different org's sf credential to get oauth for enrollment syncup
*/

function get_sf_oauth_token_response($is_default_sf_org="1",$sf_org_det_arr=array()){

	try{

		/*$fields=array('client_id'=>variable_get('salesforce_consumer_key'),
		 'client_secret'=>variable_get('salesforce_consumer_secret_key'),
				'grant_type' => 'password',
				'username'=>variable_get('salesforce_system_user_name'),
				'password'=>variable_get('salesforce_system_user_password'));*/

		/*$fields=array('client_id'=>'3MVG9xOCXq4ID1uFugIaEG0cnLZsZOYcoonG5Sf_b_.ac8DHWDJheCQ7_Vauq3vmPd4aUdjepw7W2HYN9raZc',
		 'client_secret'=>'4277228558462529578',
				'grant_type' => 'password',
				'username'=>'vincents@expertus.com',
				'password'=>'expertus1@');*/
		
		expDebug::dPrint('$is_default_sf_org = ' . print_r($is_default_sf_org, true),4);
		expDebug::dPrint('$sf_org_det_arr = ' . print_r($sf_org_det_arr, true),4);
		

		module_load_include('module','salesforce_integration','salesforce_integration');
		
		if($is_default_sf_org=="1"){
			$fields_res=getSalesforceDefaultSecretKey();
			expDebug::dPrint('Default Org Sf details - $fields_res = ' . print_r($fields_res, true),4);
		}else if($is_default_sf_org=="2"){ // Get sf ouath details from session for chatter post to different org
			
			expDebug::dPrint('Taking oauth details from session,which is set from signed request of logged in sf user.',4);
			
			$oAuth_token=array();
			$oAuth_token['instance_url']=trim($_SESSION['sf']['instance_url']);
			$oAuth_token['access_token']=trim($_SESSION['sf']['oauth_token']);
			$oAuth_token['token_type']=trim($_SESSION['sf']['token_type']);
			
			expDebug::dPrint('$oAuth_token in Session = ' . print_r($oAuth_token, true),4);
			
			return $oAuth_token;
			
		}else{
			$fields_res[0]->consumer_key=trim($sf_org_det_arr['consumer_key']);
			$fields_res[0]->consumer_secret_key=trim($sf_org_det_arr['consumer_secret_key']);
			$fields_res[0]->system_user=trim($sf_org_det_arr['system_user']);
			$fields_res[0]->system_user_pwd=trim($sf_org_det_arr['system_user_pwd']);
			expDebug::dPrint('Org Sf details - $fields_res = ' . print_r($fields_res, true),4);
		}  
		
		//expDebug::dPrint('$fields_res = ' . print_r($fields_res, true),4);

		$fields=array('client_id'=>trim($fields_res[0]->consumer_key),
				'client_secret'=>trim($fields_res[0]->consumer_secret_key),
				'grant_type' => 'password',
				'username'=>trim($fields_res[0]->system_user),
				'password'=>trim($fields_res[0]->system_user_pwd));


		expDebug::dPrint('$fields = ' . print_r($fields, true),4);

		$url='https://login.salesforce.com/services/oauth2/token';
		
		/* Added by ganeshbabuv on April 27th 2017 #0074905: Requesting to change the salesforce rest api URL according to production and testing server. */
		$salesforce_api_url=trim(variable_get('salesforce_api_url','')); 
		expDebug::dPrint('$salesforce_api_url : '.$salesforce_api_url, 5); 
		if($salesforce_api_url!=''){
			$url=$salesforce_api_url;
		} 
				
		$post_field_string = http_build_query($fields, '', '&');
		$ch = curl_init();
		curl_setopt($ch, CURLOPT_URL, $url);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
		curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 20);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
		curl_setopt($ch, CURLOPT_POSTFIELDS, $post_field_string);
		curl_setopt($ch, CURLOPT_POST, true);
		//curl_setopt($ch, CURLOPT_TIMEOUT, 20);
		$response = curl_exec($ch);

		expDebug::dPrint('$response = ' . print_r($response, true),4);

		$status = curl_getinfo($ch, CURLINFO_HTTP_CODE);
		if ( $status != 200 ) {
			 expDebug::dPrint("Error: call to token URL $url failed with status $status, response $response, curl_error " . curl_error($ch) . ", curl_errno " . curl_errno($ch),5);
			//die("Error: call to token URL $token_url failed with status $status, response $json_response, curl_error " . curl_error($curl) . ", curl_errno " . curl_errno($curl));
			return NULL;
		}
		curl_close ($ch);
		if(empty($response)) return NULL;
		return json_decode($response,true);

	}catch (Exception $ex) {
		watchdog_exception('get_sf_oauth_token_response', $ex);
		expertusErrorThrow($ex);
	}

} 


/*
 * getUserRecord() - Get salesforce user details by sending oauth details and user name
*/

function getUserRecord($oAuth_token,$user_name){

	try{
		
		expDebug::dPrint('$oAuth_token = ' . print_r($oAuth_token, true),4);
		expDebug::dPrint('$user_name = '.$user_name,5);
		
		if(empty($oAuth_token) || empty($user_name)){
		
			return NULL;
		} 
		
		$user_search="select Id,FirstName,LastName,Email from User where Email = '$user_name' limit 1"; 
		
		$user_response=do_curl_get($oAuth_token,'/services/data/v29.0/query',array(q=>$user_search));
		
		expDebug::dPrint('$user_response = ' . print_r($user_response, true),4);
		
		if(empty($user_response)) return NULL; 
	 
		$user_record=$user_response['records'][0];
		
		expDebug::dPrint('$user_record = ' . print_r($user_record, true),4);
		
		return $user_record;  
	
	}catch (Exception $ex) {
		watchdog_exception('getUserRecord', $ex);
		expertusErrorThrow($ex);
	}
		 
}

/*
 * postFeedItem() - Post feed in salesforce chatter to salesforce user by invoking the salesforce API through curl
*/

function postFeedItem($oAuth_token,$userId,$feedText){
	
	try{
					
		expDebug::dPrint('$oAuth_token = ' . print_r($oAuth_token, true),4);
		expDebug::dPrint('$userId = ' . print_r($userId, true),4);
		expDebug::dPrint('$feedText = ' . print_r($feedText, true),4);
		
		if(empty($oAuth_token) || empty($userId) || empty($feedText)){
		
			return NULL;
		}
		
		$res_feed=do_curl_post($oAuth_token,'/services/data/v31.0/chatter/feed-elements',array('feedElementType'=>'FeedItem','subjectId'=>$userId,'text'=>$feedText));
		
		expDebug::dPrint('$postFeedItem Response = ' . print_r($res_feed, true),4);
	
	}catch (Exception $ex) {
		watchdog_exception('postFeedItem', $ex);
		expertusErrorThrow($ex);
	} 
}

/* Start LMS Releated Feed Function 
 * getClassRemindSessionDate()- Get the ILT/VCL class session start date for remain the user
*/

function getClassRemindSessionDate($del_type,$slt_order_id,$user_id,$class_id){
	
	try
	{ 
		$timeZone = date_default_timezone(false);
		$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
		 
		$reminderType = ($del_type == 'lrn_cls_dty_vcl') ? 'virtual_class_reminder' : 'class_remind_session';
		$selectStmt = db_select('slt_enrollment', 'a');
		$selectStmt->leftJoin('slt_course_class_session', 'b', 'b.course_id=a.course_id AND b.class_id=a.class_id');
		$selectStmt->leftJoin('slt_course_class', 'c', 'b.class_id=c.id');
		$selectStmt->leftJoin('slt_notification_info', 'e', 'e.lang_code = \'cre_sys_lng_eng\' AND e.notification_code=\''.$reminderType.'\'');
		$selectStmt->addField('a', 'user_id');
		$selectStmt->addField('a', 'order_id');
		$selectStmt->addField('a', 'class_id');
		$selectStmt->addField('b', 'title');
		$selectStmt->addField('b', 'start_date');
		$selectStmt->addField('b', 'start_time');
		$selectStmt->addField('b', 'end_time'); 
		
		$selectStmt->condition('a.reg_status', 'lrn_crs_reg_cnf');
		$selectStmt->condition('a.comp_status', 'lrn_crs_cmp_enr');
		$selectStmt->condition('c.status', 'lrn_cls_sts_atv');
		$selectStmt->condition('a.order_id', $slt_order_id);
		$selectStmt->condition('a.user_id', $user_id);
		$selectStmt->condition('a.class_id', $class_id); 
		$selectStmt->condition('c.delivery_type', $del_type);
		$selectStmt->where('e.frequency_data_value = DATEDIFF(DATE_FORMAT(b.start_date,\'%Y-%m-%d\'),
				IFNULL(CONVERT_TZ(now(), \'".$defaultTimezone."\' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= b.timezone)),NOW()))');
		$selectStmt->groupBy('a.id');
		expDebug::dPrintDBAPI('getClassRemindSessionDate ' , $selectStmt);
		$result =  $selectStmt->execute()->fetchAll();
		expDebug::dPrint('getClassRemindSessionDate - $result ' . print_r($result,true),4);
		
		return $result;		
		
	}catch(Exception $ex) {
		watchdog_exception('getClassRemindSessionDate', $ex);
		expertusErrorThrow($ex);  
	}
	
}

/*
 * getSalesforceCertCurrLPExpiryDetailsForNotifications() - get all Certification/Curriculum/Learning plan expiry enrollments for saleforce feed
*/

function getSalesforceCertCurrLPExpiryDetailsForNotifications($user_id,$tp_id='') {
	
	try{
		
		$config           = getConfig("exp_sp");
		$cert_days_expire = isset($config["cert_expire_reminder_days"]) ? $config["cert_expire_reminder_days"] : 10;
		$select = db_select('slt_program','prgm');
		$select->innerJoin('slt_master_enrollment','mse','mse.program_id = prgm.id');
		$select->innerJoin('slt_person','prs','prs.id = mse.user_id');
		$select->innerJoin('slt_profile_list_items','prof','prof.code = prgm.object_type');
		$select->addField('prs','id','personId');
		$select->addField('prs','first_name');
		$select->addField('prgm','id','tp_id');
		$select->addField('prgm','title','tp_title');
		$select->addField('prgm','code','tp_code');
		$select->addField('prgm','title');
		$select->addField('prgm','lang_code');
		$select->addField('prof','name','delivery_type');
		$select->addField('mse','comp_date','comp_date');
		$select->addExpression("DATE_FORMAT(
				IF (prgm.expires_in_unit = 'days', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value DAY),
				IF (prgm.expires_in_unit = 'months', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value MONTH),
				DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value YEAR))),
				'%b %e, %Y')", "tp_end_date");
		$select->addExpression("DATEDIFF(
				IF (prgm.expires_in_unit = 'days', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value DAY),
				IF (prgm.expires_in_unit = 'months', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value MONTH),
				DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value YEAR))),
				NOW())", "expire_days");
		if($user_id>0) {
			$select->condition('prs.id',$user_id,'=');
		}
		
		if($tp_id>0 && $tp_id!='') {
			$select->condition('prgm.id',$tp_id,'=');
		}
		
		$select->condition('prgm.object_type', 'cre_sys_obt_crt', '=');
		$select->condition('mse.overall_status', 'lrn_tpm_ovr_cmp', '=');
		$select->isNotNull('mse.comp_date');
		$select->isNotNull('prgm.expires_in_value');
		$select->isNotNull('prgm.expires_in_unit');
		$select->condition('prgm.expires_in_value', 0, '>');
		$select->where("DATEDIFF(
				IF (prgm.expires_in_unit = 'days', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value DAY),
				IF (prgm.expires_in_unit = 'months', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value MONTH),
				DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value YEAR))),
				NOW()) = $cert_days_expire");

		expDebug::dPrintDBAPI('$select', $select);
		$result = $select->execute();
		if($user_id>0) {
			$results = $result->fetchAssoc();
		} else {
			$results = $result->fetchAll();
		}
		expDebug::dPrint('$results = ' . print_r($results, true));
		
		return $results;
		
	}catch (Exception $ex) {
		watchdog_exception('getSalesforceCertCurrLPExpiryDetailsForNotifications', $ex);
		if (isset($_GET['cron_key'])) {
				throw new Exception($ex);
		}
		expertusErrorThrow($ex);
	}
	
}

/*
 * getSalesforceLPTerminateDetailsForReminderNotifications() - get all Learning plan which ends before given sequence days, which is for salesforce 
*/

function getSalesforceLPTerminateDetailsForReminderNotifications($user_id,$tp_id='') {
	
	try{
		
		$select = db_select('slt_program','prgm');
		$select->innerJoin('slt_master_enrollment','mse','mse.program_id = prgm.id');
		$select->innerJoin('slt_person','prs','prs.id = mse.user_id');
		$select->leftJoin('slt_notification_info','info','info.notification_code = \'lp_termination_reminder\' AND  info.lang_code = \'cre_sys_lng_eng\'');
		$select->leftJoin('slt_entity_notification_mapping','map','map.notify_id = info.id AND map.entity_id=prgm.id');


		$select->addField('prs','id','personId');
		$select->addField('prs','first_name');
		$select->addField('prgm','id','tp_id');
		$select->addField('prgm','title','tp_title');
		$select->addField('prgm','code','tp_code');
		$select->addField('prgm','lang_code');
		$select->addField('prgm','end_date','tp_end_date');
		$select->addExpression("DATEDIFF(prgm.end_date,NOW())","expire_days");

		if($user_id>0) {
			$select->condition('prs.id',$user_id,'=');
		}
		
		if($tp_id>0 && $tp_id!='') {
			$select->condition('prgm.id',$tp_id,'=');
		}
		
		$select->condition('prgm.object_type','cre_sys_obt_trn','=');
		$select->condition('info.status', 'cre_ntn_sts_atv');
		$select->where("IFNULL(map.status,'Y') = 'Y' AND DATEDIFF(prgm.end_date,NOW())  = IFNULL(map.notify_frequency,info.frequency_data_value)");
		expDebug::dPrintDBAPI(' getLPTerminateDetailsForReminderNotifications SQL:  = ', $select);
		$result = $select->execute();

		if($user_id>0) {
			$results = $result->fetchAssoc();
		} else {
			$results = $result->fetchAll();
		}
		
		expDebug::dPrint('$results = ' . print_r($results, true),4);
		
		return $results;
		
	}catch (Exception $ex) {
	 watchdog_exception('getSalesforceLPTerminateDetailsForReminderNotifications', $ex);
		if (isset($_GET['cron_key'])) {
				throw new Exception($ex);
		}
	 expertusErrorThrow($ex);
	}
	
}


/*
 * getPersonDetailsForFeed() - Gets person information for salesforce feed
*/
function getPersonDetailsForFeed($user_id) {
	
	try{
		// Select table
		$selectStmt = db_select('slt_person', 'per');
		$selectStmt->leftjoin('slt_person', 'mgr', 'mgr.id = per.manager_id'); 
		// Set conditions
		$selectStmt->condition('per.id', $user_id);

		// Select fields and/or add expressions
		$selectStmt->addField('per', 'id', 'id');
		$selectStmt->addField('per', 'first_name', 'first_name');
		$selectStmt->addField('per', 'last_name', 'last_name');
		$selectStmt->addField('per', 'full_name', 'user_full_name');
		$selectStmt->addField('per', 'phone_no', 'phone_no');
		$selectStmt->addField('per', 'email', 'email');
		$selectStmt->addField('per', 'user_name', 'user_name');
		$selectStmt->addField('per', 'manager_id', 'manager_id');
		$selectStmt->addField('per', 'org_id', 'org_id');
		//$selectStmt->addField('per', 'full_name', 'full_name');
		$selectStmt->addField('mgr', 'full_name', 'manager_name');
		$selectStmt->addField('mgr', 'user_name', 'manager_user_name');  

		expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);

		// Execute query
		$result = $selectStmt->execute()->fetchAssoc();
		expDebug::dPrint(' $result = ', 4);
		expDebug::dPrint($result, 4);

		if (count($result) <= 0) {
			return null;
		}

		return $result;
		
	}catch (Exception $ex) {
		watchdog_exception('getPersonDetailsForFeed', $ex);
		expertusErrorThrow($ex);
	}
	
}

/*
 * getTpEnrollmentDeatils() - Gets person information for salesforce feed
*/

function getTpEnrollmentDeatils($enr_arr) {
	
	try{

		expDebug::dPrint('$enr_arr = ' . print_r($enr_arr, true),4);
		
		$user_id=trim($enr_arr["user_id"]);
		$tp_id=trim($enr_arr["tp_id"]);
		$mas_enr_id=trim($enr_arr["mas_enr_id"]);  
		
		// Select table
		$selectStmt = db_select('slt_master_enrollment', 'mas_enr');
		$selectStmt->innerjoin('slt_program', 'prg', 'prg.id=mas_enr.program_id');
		$selectStmt->innerjoin('slt_person', 'per', 'per.id=mas_enr.user_id');
		$selectStmt->leftjoin('slt_person', 'mgr', 'mgr.id=per.manager_id');
		$selectStmt->leftjoin('slt_profile_list_items', 'spli', 'spli.code=prg.object_type');
		
		// Set conditions
		if($user_id!=""){
		   $selectStmt->condition('mas_enr.user_id', $user_id);
		}
		
		if($tp_id!=""){
			$selectStmt->condition('mas_enr.program_id', $tp_id);
		}
		
		if($mas_enr_id!=""){
			$selectStmt->condition('mas_enr.id', $mas_enr_id);
		}

		// Select fields and/or add expressions
		$selectStmt->addField('prg', 'id', 'tp_id');
		$selectStmt->addField('prg', 'title', 'tp_title');
		$selectStmt->addField('prg', 'code', 'tp_code');
		$selectStmt->addField('prg', 'object_type', 'tp_del_type_code');		
		$selectStmt->addField('spli', 'name', 'tp_del_type'); 
		$selectStmt->addField('mas_enr', 'reg_date', 'tp_reg_date');
		
		$selectStmt->addField('per', 'user_name', 'user_name');
		$selectStmt->addField('per', 'full_name', 'user_full_name');
		$selectStmt->addField('per', 'manager_id', 'manager_id');  
		$selectStmt->addField('mgr', 'user_name', 'manager_user_name');
		$selectStmt->addField('mgr', 'full_name', 'manager_name');
		

		expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);

		// Execute query
		$result = $selectStmt->execute()->fetchAssoc();
		expDebug::dPrint('$result = ' . print_r($result, true),4);

		if (count($result) <= 0) {
			return null;
		} 

		return $result;

	}catch (Exception $ex) {
		watchdog_exception('getTpEnrollmentDeatils', $ex);
		expertusErrorThrow($ex);
	}
	
}

/*
 * getEnrollmentDeatils() - To get the enrollment details as well as class details and person details from LMS
*/

function getEnrollmentDeatils($enr_arr){ 

	try{
	
		expDebug::dPrint('$enr_arr = ' . print_r($enr_arr, true),4);
		
		$class_id=trim($enr_arr["class_id"]);
		$user_id=trim($enr_arr["user_id"]);
		$enr_id=trim($enr_arr["enr_id"]);
		$enr_comp_status=trim($enr_arr["enr_comp_status"]);
		$enr_order_id=trim($enr_arr["order_id"]);
		$enr_course_id=trim($enr_arr["course_id"]);
		
		$query="select per.full_name as name,
		per.user_name as uname,
		per.manager_id as manager,
		mgr.full_name as mgr_name,
		mgr.user_name as mgr_user_name,
		cls.title as title,
		cls.code as code,
		cls.delivery_type as type_code,
		slf_get_profile_list_name(cls.delivery_type,NULL) as type,
		date_format(enr.reg_date,'%d-%b-%Y') as reg_date,
		enr.comp_status as comp_status,
		date_format(enr.comp_date,'%d-%b-%Y') as comp_date
		from slt_enrollment enr
		inner join slt_course_class cls on cls.id=enr.class_id
		inner join slt_person per on per.id=enr.user_id
		left join slt_person mgr on mgr.id=per.manager_id
		where enr.user_id='$user_id'";
		
		
		if($class_id!="" && $class_id>0){
			$query.=" and enr.class_id='$class_id'";
		}
		
		if($enr_id!="" && $enr_id>0){
			$query.=" and enr.id='$enr_id'";
		}
		
		if($enr_comp_status!=""){
			$query.=" and enr.comp_status='$enr_comp_status'";
		}
		
		if($enr_order_id!=""){
			$query.=" and enr.order_id='$enr_order_id'";
		}
		
		if($enr_course_id!=""){
			$query.=" and enr.course_id='$enr_course_id'";
		}
		
		
		$query.=" order by enr.id desc limit 1";
		
		
		expDebug::dPrint('$query = ' . print_r($query, true),4);
		
		$enrollmentDeatils=array();
		
		//$result=db_query($query,array(':enr_id'=>$enrollmentId));
		$result=db_query($query);
		
		expDebug::dPrint('$result = ' . print_r($result, true),4);
		
		if(count($result)>0){
		
			foreach($result as $record){
				$enrollmentDeatils['user_full_name']= $record->name;
				$enrollmentDeatils['user_name']= $record->uname;
				$enrollmentDeatils['manager_id']= $record->manager;
				$enrollmentDeatils['class_title']= $record->title;
				$enrollmentDeatils['class_code']= $record->code;
				$enrollmentDeatils['class_type']= $record->type;
				$enrollmentDeatils['class_type_code']= $record->type_code;
				$enrollmentDeatils['class_reg_date']= $record->reg_date;
				$enrollmentDeatils['class_comp_date']= $record->comp_date;
				$enrollmentDeatils['manager_user_name']= $record->mgr_user_name;
				break;
			}
		}
		
		expDebug::dPrint('$enrollmentDeatils = ' . print_r($enrollmentDeatils, true),4);
		
		return $enrollmentDeatils;
	
	}catch (Exception $ex) {
		watchdog_exception('getEnrollmentDeatils', $ex);
		expertusErrorThrow($ex);
	} 
}

/*
 * triggerFeed() - Trigger feed after make connection with salesforce and confirm the security key and consumer key details
*/

function triggerFeed($user_name,$user_feed_text,$manager_user_name="",$manager_feed_text=""){ 

	try{
			
		expDebug::dPrint('$user_name = ' . print_r($user_name, true),4);
		expDebug::dPrint('$user_feed_text = ' . print_r($user_feed_text, true),4);
		expDebug::dPrint('$manager_user_name = ' . print_r($manager_user_name, true),4);
		expDebug::dPrint('$manager_feed_text = ' . print_r($manager_feed_text, true),4);
		
		 
		$oAuth_token_response = get_sf_oauth_token_response("2"); //Get oAuth details from Session
		expDebug::dPrint('$oAuth_token_response = ' . print_r($oAuth_token_response, true),4);
		 
		//Trigger feed to User
		if($user_name!=""){
			if(isset($oAuth_token_response)){
				
				expDebug::dPrint('SF user id in session  = ' . print_r($_SESSION['sf']['user_id'], true),4);
				
				if(trim($_SESSION['sf']['user_id'])!=''){
					$user_record['Id']=trim($_SESSION['sf']['user_id']);
				}else{
					$user_record=getUserRecord($oAuth_token_response,$user_name); 
				}  
				if(empty($user_record)) return;
				postFeedItem($oAuth_token_response,$user_record['Id'],$user_feed_text);
		
			}
		}
		
		//Trigger feed to Manager
		if($manager_user_name!=""){
			if(isset($oAuth_token_response)){
				// echo "posting to ".$enrollmentDeatils['mgr_uname'];
				$user_record=getUserRecord($oAuth_token_response,$manager_user_name);
				if(empty($user_record)) return;
				postFeedItem($oAuth_token_response,$user_record['Id'],$manager_feed_text);
		
			}
		}
		
	
	}catch (Exception $ex) {
		watchdog_exception('triggerFeed', $ex);
		expertusErrorThrow($ex);
	}
	  
}

/*
 * getFeedText() - Get Feed Text for proper notification and user type 
*/

function getFeedText($notification_type,$notification_det,$user_type=''){

	try{
		
		$str="";
		
		switch ($notification_type) {
		
			case 'Register':
					
				if($user_type=="user"){
					$str=t("SFMSG001")." ".$notification_det['class_title']." (".$notification_det['class_code'].") ".t("SFMSG002")." ".$notification_det['class_reg_date'].".\n";
					$str.=t("SFMSG003");
				}
		
				if($user_type=="manager"){
					$str=t("SFMSG004")." ".$notification_det['user_full_name']." ".t("SFMSG005")." ".$notification_det['class_title']." (".$notification_det['class_code'].") ".t("SFMSG002")." ";
					$str.=$notification_det['class_reg_date'].".\n ";
					$str.=t("SFMSG006");
				}
		
				break;
		
			case 'Drop':
			case 'WBTDropped':
			case 'VCDropped':
					
				if($user_type=="user"){
					$str=t("SFMSG007")." ".$notification_det['class_title']." (".$notification_det['class_code'].") ".t("SFMSG002")." ".$notification_det['class_reg_date'].".\n";
				}
		
				if($user_type=="manager"){
					$str=t("SFMSG004")." ".$notification_det['user_full_name']." ".t("SFMSG008")." ".$notification_det['class_title']." (".$notification_det['class_code'].") ".t("SFMSG002")." ";
					$str.=$notification_det['class_reg_date'].".\n ";
				}
		
				break;
		
		
			case 'course_completed':
			case 'Completed':
					
				if($user_type=="user"){
					$str=t("SFMSG009")." ".$notification_det['class_title']." (".$notification_det['class_code'].") ".t("SFMSG002")." ".$notification_det['class_comp_date'].".\n";
				}
		
				if($user_type=="manager"){
					$str=t("SFMSG004")." ".$notification_det['user_full_name']." ".t("SFMSG010")." ".$notification_det['class_title']." (".$notification_det['class_code'].") ".t("SFMSG002")." ";
					$str.=$notification_det['class_comp_date'].".\n ";
				}
		
				break;
		
			case 'Waitlist':
		
				if($user_type=="user"){
					$str=t("SFMSG001")." ".$notification_det['class_title']." (".$notification_det['class_code'].") ".t("SFMSG002")." ".$notification_det['class_reg_date'].". ".t("SFMSG011").".\n";
					$str.=t("SFMSG003");
				}
		
				if($user_type=="manager"){
					$str=t("SFMSG004")." ".$notification_det['user_full_name']." ".t("SFMSG005")." ".$notification_det['class_title']." (".$notification_det['class_code'].") ".t("SFMSG002")." ";
					$str.=$notification_det['class_reg_date'].". ".t("SFMSG011").".\n ";
					$str.=t("SFMSG006");
				}
		
				break;
		
			case 'class_remind_session':
			  
				$ses_start_str="";
		
				if($notification_det['ses_start_date']!=""){
					$ses_start_str=date("d-M-Y", strtotime($notification_det['ses_start_date']));
				}
					
				if($user_type=="user"){
					$str=t("SFMSG012")." ".t("SFMSG013")." ".t("Classroom")." ".$notification_det['class_title']." (".$notification_det['class_code'].") ".t("SFMSG014")." ".$ses_start_str.".\n";
					$str.=t("SFMSG015");
				}
					
				if($user_type=="manager"){
					$str=t("SFMSG012")." ".t("SFMSG016")." ".t("SFMSG017")." ".$notification_det['user_full_name']." ".t("SFMSG018")." ".t("Classroom")." ".$notification_det['class_title']." (".$notification_det['class_code'].") ".t("SFMSG014")." ";
					$str.=$ses_start_str.".\n";
					$str.=t("SFMSG019");
				}
					
				break;
					
			case 'virtual_class_reminder':
		
				$ses_start_str="";
					
				if($notification_det['ses_start_date']!=""){
					$ses_start_str=date("d-M-Y", strtotime($notification_det['ses_start_date']));
				}
					
				if($user_type=="user"){
					$str=t("SFMSG012")." ".t("SFMSG013")." ".t("Virtual Class")." ".$notification_det['class_title']." (".$notification_det['class_code'].") ".t("SFMSG014")." ".$ses_start_str.".\n";
					$str.=t("SFMSG015");
				}
					
				if($user_type=="manager"){
					$str=t("SFMSG012")." ".t("SFMSG016")." ".t("SFMSG017")." ".$notification_det['user_full_name']." ".t("SFMSG018")." ".t("Virtual Class")." ".$notification_det['class_title']." (".$notification_det['class_code'].") ".t("SFMSG014")." ";
					$str.=$ses_start_str.".\n";
					$str.=t("SFMSG019");
				}
					
				break;
					
			case 'LPTerminationReminder':
		
				$tp_end_date_str="";
					
				if($notification_det['tp_end_date']!=""){
					$tp_end_date_str=date("d-M-Y", strtotime($notification_det['tp_end_date']));
				}
					
				if($user_type=="user"){
					$str=t("SFMSG012")." ".t("SFMSG013")." ".t("Learning Plan")." ".$notification_det['tp_title']." (".$notification_det['tp_code'].") ".t("SFMSG020")." ".$tp_end_date_str.".\n";
					$str.=t("SFMSG021");
				}
		
				if($user_type=="manager"){
					$str=t("SFMSG012")." ".t("SFMSG016")." ".t("SFMSG017")." ".$notification_det['user_full_name']." ".t("SFMSG018")." ".t("Learning Plan")." ".$notification_det['tp_title']." (".$notification_det['tp_code'].") ".t("SFMSG020")." ";
					$str.=$tp_end_date_str.".\n";
					$str.=t("SFMSG022");
				}
		
				break;
		
			case 'RegisterWBTCompliance':
			case 'RegisterVCCompliance':
			case 'RegisterMandatoryCompliance':
		
				if($user_type=="user"){
					$str=t("SFMSG023")." ".$notification_det['class_title']." (".$notification_det['class_code'].") ".t("LBL945")." ".$notification_det['class_reg_date'].".\n";
					$str.=t("SFMSG003");
				}
		
				if($user_type=="manager"){
					$str=t("SFMSG004")." ".$notification_det['user_full_name']." ".t("SFMSG025")." ".$notification_det['class_title']." (".$notification_det['class_code'].") ".t("LBL945")." ";
					$str.=$notification_det['class_reg_date'].".\n ";
					$str.=t("SFMSG006");
				}
		
				break;
		
			case 'certCurrLPRegister':
				$tp_reg_date_str="";
		
				if($notification_det['tp_reg_date']!=""){
					$tp_reg_date_str=date("d-M-Y", strtotime($notification_det['tp_reg_date']));
				}
		
				$notification_det['tp_reg_date']=$tp_reg_date_str;
		
				if($user_type=="user"){
					$str=t("SFMSG001")." ".$notification_det['tp_title']." (".$notification_det['tp_code'].") ".t("SFMSG002")." ".$notification_det['tp_reg_date'].".\n";
					$str.=t("SFMSG003");
				}
		
				if($user_type=="manager"){
					$str=t("SFMSG004")." ".$notification_det['user_full_name']." ".t("SFMSG005")." ".$notification_det['tp_title']." (".$notification_det['tp_code'].") ".t("SFMSG002")." ";
					$str.=$notification_det['tp_reg_date'].".\n ";
					$str.=t("SFMSG006");
				}
					
				break;
		
			case 'content_move_users':
				$today_dt_str=date("d-M-Y");
					
				if($user_type=="user"){
					$str=t("SFMSG027")." ".$notification_det['class_title']." (".$notification_det['class_code'].") ".t("SFMSG028")." ".$today_dt_str." ".t("SFMSG029");
		
				}
				break;
					
			case 'class_switch':
				$today_dt_str=date("d-M-Y");
					
				if($user_type=="user"){
					$str=t("SFMSG030")." ".$notification_det['change_cls_details']['old_class_title']." (".$notification_det['change_cls_details']['old_class_code'].") ".t("SFMSG026")." ".$notification_det['change_cls_details']['new_class_title'] ." (".$notification_det['change_cls_details']['new_class_code'].") ".t("LBL945")." ".$today_dt_str.".\n";
					$str.=t("SFMSG003");
				}
				if($user_type=="manager"){
					$str=t("SFMSG004")." ".$notification_det['user_full_name']." ".t("SFMSG031")." ".$notification_det['change_cls_details']['old_class_title']." (".$notification_det['change_cls_details']['old_class_code'].") ".t("SFMSG026")." ".$notification_det['change_cls_details']['new_class_title']." (".$notification_det['change_cls_details']['new_class_code'].") ".t("LBL945")." ".$today_dt_str.".\n";
					$str.=t("SFMSG006");
				}
					
				break; 
		
		}
		
		expDebug::dPrint("Feed Text Str for $user_type = " . print_r($str, true),4);
		
		return $str;	
			
	
	}catch (Exception $ex) {
		watchdog_exception('getFeedText', $ex);
		expertusErrorThrow($ex);
	}  
}

/* postSalesforceFeed() - Post the feed to users and their manager according to notification code
 * Parametres : $notification_type - Notification Title, $notification_code - Notification Code, 
 * $object_id1 - Course Id/Order ID /Maseter Enrollment ID/ Content id
 * $object_id2 - Class id,  Enrollment ID
 * $user_id - Enrolled User Id
 */  

function postSalesforceFeed($notification_type,$notification_code,$object_id1, $object_id2, $user_id){
	
	try{
	
	  expDebug::dPrint('postSalesforceFeed Called with ' .
				'$notification Type = ' . $notification_type . ', ' .
				'$notification Code = ' . $notification_code . ', ' .
				'$objectId1 = ' . $object_id1 . ', ' .
				'$objectId2 = ' . $object_id2 . ', ' .				
				'$userId = ' . userId , 4);
		
		$enr_arr=array("");
		
		//Check that whether feed option is enabled or not
		
		$feed_trigger_option=variable_get('salesforce_feed_trigger_option');
		
		expDebug::dPrint('$feed_trigger_option = ' . $feed_trigger_option,4);
		
		if($feed_trigger_option!="1"){
			expDebug::dPrint('Chatter feed option has in disabled state.',1);
			return false;
		}
		
		switch ($notification_type) {
			 
			case 'Register':
				$enr_arr["class_id"]=$object_id2;
				$enr_arr["user_id"]=$user_id;
				$enr_details=getEnrollmentDeatils($enr_arr);
				break;
			case 'Drop':
			case 'WBTDropped':
			case 'VCDropped':
				$enr_arr["class_id"]=$object_id2;
				$enr_arr["enr_id"]=$enr_id;
				$enr_arr["user_id"]=$user_id;
				$enr_details=getEnrollmentDeatils($enr_arr);
				break;
				 
			case 'course_completed': //WBT and Video Completion
				 
				$enr_arr["class_id"]=$object_id2;
				$enr_arr["user_id"]=$user_id;
				$enr_arr["enr_comp_status"]='lrn_crs_cmp_cmp';
				$enr_details=getEnrollmentDeatils($enr_arr);
				break;
		
			case 'Completed': //ILT and VC Completion
				 
				$enr_arr["class_id"]=$object_id2;
				$enr_arr["user_id"]=$user_id;
				$enr_arr["enr_id"]=$object_id1;
				$enr_arr["enr_comp_status"]='lrn_crs_cmp_cmp';
				$enr_details=getEnrollmentDeatils($enr_arr);
				break;
				 
			case 'Waitlist':    // Wait list position for ILT and VCL
		
				$enr_arr["class_id"]=$object_id2;
				$enr_arr["user_id"]=$user_id;
				$enr_details=getEnrollmentDeatils($enr_arr);
				break;
		
			case 'class_remind_session':    // Class Start Session Remainder for ILT  before x days, which is configured in notification template
				 
				$enr_arr["class_id"]=$object_id2;
				$enr_arr["user_id"]=$user_id;
				$enr_arr["order_id"]=$object_id1;
		
				$enr_details=array("");
				$enr_details=getEnrollmentDeatils($enr_arr);
				$ses_details=getClassRemindSessionDate('lrn_cls_dty_ilt',$object_id1,$user_id,$object_id2);
				$enr_details["ses_start_date"]=$ses_details[0]->start_date;
				$enr_details["ses_start_time"]=$ses_details[0]->start_time;
				$enr_details["ses_end_time"]=$ses_details[0]->end_time;
				$enr_details["ses_title"]=$ses_details[0]->title;
				break;
		
			case 'virtual_class_reminder':    // Class Start Session Remainder for VCL  before x days, which is configured in notification template
		
				$enr_arr["class_id"]=$object_id2;
				$enr_arr["user_id"]=$user_id;
				$enr_arr["order_id"]=$object_id1;
		
				$enr_details=array("");
				$enr_details=getEnrollmentDeatils($enr_arr);
				$ses_details=getClassRemindSessionDate('lrn_cls_dty_vcl',$object_id1,$user_id,$object_id2);
				$enr_details["ses_start_date"]=$ses_details[0]->start_date;
				$enr_details["ses_start_time"]=$ses_details[0]->start_time;
				$enr_details["ses_end_time"]=$ses_details[0]->end_time;
				$enr_details["ses_title"]=$ses_details[0]->title;
				break;
		
			case 'LPTerminationReminder':    // Learning Plan Termination Reminder before x days, which is configured in notification template
					
				$tp_id=$object_id1;
		
				$tp_reminder_res=getSalesforceLPTerminateDetailsForReminderNotifications($user_id,$tp_id);
		
				$user_details=getPersonDetailsForFeed($user_id);
		
				$tp_reminder_res['user_full_name']=$user_details['user_full_name'];
				$tp_reminder_res['user_name']=$user_details['user_name'];
				$tp_reminder_res['manager_user_name']=$user_details['manager_user_name'];
				$enr_details=$tp_reminder_res;
					
				break;
					
			case 'certCurrLPExpireRemainder':    // Certificate Expiry Reminder before x days, which is configured in exp_sp.ini file
					
				$tp_id=$object_id1;
					
				$tp_reminder_res=getSalesforceCertCurrLPExpiryDetailsForNotifications($user_id,$tp_id);
					
				$user_details=getPersonDetailsForFeed($user_id);
					
				$tp_reminder_res['user_full_name']=$user_details['user_full_name'];
				$tp_reminder_res['user_name']=$user_details['user_name'];
				$tp_reminder_res['manager_user_name']=$user_details['manager_user_name'];
				$enr_details=$tp_reminder_res;
				break;
		
			case 'RegisterWBTCompliance':
			case 'RegisterVCCompliance':
			case 'RegisterMandatoryCompliance':   //Compilance course auto registeration for ILT/VC/WBT/Video
				$enr_arr["class_id"]=$object_id2;
				$enr_arr["user_id"]=$user_id;
				$enr_arr["course_id"]=$object_id1;
				$enr_details=getEnrollmentDeatils($enr_arr);
				break;
		
			case 'certCurrLPRegister':   // TP Registration and auto registration for mandatory TP.
				$enr_arr["tp_id"]=$object_id2;
				$enr_arr["user_id"]=$user_id;
				$enr_arr["mas_enr_id"]=$object_id1;
				$enr_details=getTpEnrollmentDeatils($enr_arr);
				break;
					
			case 'content_move_users':   // Move content from old version to new version
				$enr_arr["enr_id"]=$object_id2;
				$enr_arr["user_id"]=$user_id;
				$enr_arr["content_id"]=$object_id1;
				$enr_details=getEnrollmentDeatils($enr_arr);
				$enr_details['manager_user_name']="";  //set as empty for manager name to avoid the manager feed
				break;
					
			case 'class_switch':   // Change Class from My Enrollment
				$enr_details=array("");
				$class_id=$object_id2;
				$change_class_details_obj=$object_id1;
		
				$user_details=getPersonDetailsForFeed($user_id);
				$enr_details['user_full_name']=$user_details['user_full_name'];
				$enr_details['user_name']=$user_details['user_name'];
				$enr_details['manager_user_name']=$user_details['manager_user_name'];
				$enr_details['change_cls_details']=$change_class_details_obj;
				break;
		
		}
			
		expDebug::dPrint('$enr_details = ' . print_r($enr_details, true),4);
		
		//Trigger Feed after get the feed text
		$user_feed_text=getFeedText($notification_type,$enr_details,"user");
		
		$mgr_feed_text="";
		if($enr_details['manager_user_name']!=""){  //if manager is not there
			$mgr_feed_text=getFeedText($notification_type,$enr_details,"manager");
		}
		if($enr_details['user_name']!=""){ //If user is there then trigger the feed
			triggerFeed($enr_details['user_name'],$user_feed_text,$enr_details['manager_user_name'],$mgr_feed_text);
		}			
	
	}catch (Exception $ex) {
		watchdog_exception('postSalesforceFeed', $ex);
		if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
		expertusErrorThrow($ex);
	}  
	
}  
 
/* End LMS Releated Feed Function */

?>