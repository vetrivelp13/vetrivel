<?php 

/**
 ** Implementaion of moveEnrollmentsToSFtable().
 ** - Get enrollment details from slt_enrollment and slt_master_enrollment in Report DB Server and store into slt_sf_enrollment table in Master DB Server to update in salesforce custom objects.
 **/
function moveEnrollmentsToSFtable(){

	$txn = db_transaction();

	try{  
		
		include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/GlobalUtil.php";
	    $util    = new GlobalUtil();
		$conf    = $util->getConfig();
		$db_url  = $conf["report_db_url"];
		$tmp1    = explode("@",$db_url);
		$report_server_db_name  = '`'.substr($tmp1[1],stripos($tmp1[1],"/")+1,strlen($tmp1[1])).'`';
		expDebug::dPrint('$report_server_db_name = ' .print_r($report_server_db_name,true) , 4);	
		
		$ins_str="insert into slt_sf_enrollment(enrollment_id,master_enrollment_id,tp_code,tp_title,tp_type,tp_overall_status,course_code,
		course_title,class_code,class_title,delivery_type,email,
		enrollment_status,
		reg_date,
		comp_date,
		cancel_date,
		created_by,created_on,updated_by,updated_on)
		select enr.id as enr_id,mas_enr.id as mas_enr_id,prg.code as tp_code,prg.title as tp_title,spli_tp.name as tp_type,spli_os.name as tp_status,crs.code as course_code,
		crs.title as course_title,cls.code as class_code,cls.title as class_title,spli.name as del_type,per.email as email,
		if(enr.reg_status='lrn_crs_reg_cnf',spli_es.name,spli_rs.name) as enrollment_status,
		enr.reg_date as reg_date, 
		if(enr.reg_status='lrn_crs_reg_cnf' and enr.comp_status='lrn_crs_cmp_cmp',enr.comp_date,null) as comp_date,
		if(enr.reg_status='lrn_crs_reg_can',enr.reg_status_date,null) as cancel_date,
		1 as created_by,now() as created_on,1 as updated_by,now() as updated_on
		from $report_server_db_name.slt_enrollment enr 
		left join $report_server_db_name.slt_sf_enrollment sf_enr on sf_enr.enrollment_id=enr.id
		inner join $report_server_db_name.slt_course_template crs on crs.id=enr.course_id
		inner join $report_server_db_name.slt_course_class cls on cls.id=enr.class_id
		inner join $report_server_db_name.slt_person per on per.id=enr.user_id
		inner join $report_server_db_name.slt_organization org on org.id=per.org_id
	    inner join $report_server_db_name.slt_custom_fields scf on scf.entity_id=org.id and scf.entity_type='organization' and scf.label='Salesforce Org ID' and scf.value!=''
	    inner join $report_server_db_name.slt_cstm_salesforce_settings scss on scss.exp_org_id=org.id and scss.enr_syncup=1
		left join $report_server_db_name.slt_master_enrollment mas_enr on mas_enr.id=enr.master_enrollment_id
		left join $report_server_db_name.slt_program prg on prg.id=mas_enr.program_id
		left join $report_server_db_name.slt_profile_list_items spli_tp on spli_tp.code=prg.object_type		
		left join $report_server_db_name.slt_profile_list_items spli on spli.code=cls.delivery_type
		left join $report_server_db_name.slt_profile_list_items spli_os on spli_os.code=mas_enr.overall_status
		left join $report_server_db_name.slt_profile_list_items spli_es on spli_es.code=enr.comp_status
		left join $report_server_db_name.slt_profile_list_items spli_rs on spli_rs.code=enr.reg_status		
		where sf_enr.enrollment_id is null"; 
		
		expDebug::dPrint('Enrollment Insert Query String $ins_str = ' . $ins_str , 4); 
			
		$ins_qry_response = db_query($ins_str);
		
		expDebug::dPrint('Bulk Enrollment Insert Response = ' . print_r($ins_qry_response, true) , 4);

	}catch (Exception $ex) {
		$txn->rollback();
		unset($txn);
		watchdog_exception('moveEnrollmentsToSFtable', $ex);
		expDebug::dPrint('moveEnrollmentsToSFtable $Exception Error = '.$ex  , 4);
		//expertusErrorThrow($ex);
	}
	unset($txn);
} 
		

/**
 ** Implementaion of syncEnrollmentUpdationDetailstoSalesforce().
 ** - Get enrollment status of Class and TP Trainings from Report (slave) Server and update those details to Salesforce
 **/
function syncEnrollmentUpdationDetailstoSalesforce(){

	$txn = db_transaction();

	try{

		include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/GlobalUtil.php";
		$util    = new GlobalUtil();
		$conf    = $util->getConfig();
		$db_url  = $conf["report_db_url"];
		$tmp1    = explode("@",$db_url);
		$report_server_db_name  = '`'.substr($tmp1[1],stripos($tmp1[1],"/")+1,strlen($tmp1[1])).'`';
		expDebug::dPrint('$report_server_db_name = ' .print_r($report_server_db_name,true) , 4);
		
		$sf_post_data_limit=trim($config["sf_post_data_limit"]);
			
		expDebug::dPrint('$sf_post_data_limit= '.print_r($sf_post_data_limit,true), 5);
		
		if($sf_post_data_limit!=''){
			$enr_limit_to_post_in_sf=$sf_post_data_limit;
		}else{
			$enr_limit_to_post_in_sf=200;
		}
		
		expDebug::dPrint('$enr_limit_to_post_in_sf= '.print_r($enr_limit_to_post_in_sf,true), 5);
		

		$enr_get_upd_str="select sf_enr.id as sf_id,sf_enrollment_id as sf_enrollment_res_obj_id,
		if(enr.reg_status='lrn_crs_reg_cnf',spli_es.name,spli_rs.name) as enrollment_status,   
		spli_os.name as tp_overall_status , 
		if(enr.reg_status='lrn_crs_reg_cnf' and enr.comp_status='lrn_crs_cmp_cmp',enr.comp_date,null) as comp_date, 
		if(enr.reg_status='lrn_crs_reg_can',enr.reg_status_date,null) as cancel_date ,
		org.id as org_id,org.name as org_name,scss.consumer_key as scss_consumer_key,
		scss.consumer_secret_key as scss_consumer_secret_key,scss.system_user as scss_system_user,scss.system_user_pwd as scss_system_user_pwd,sf_enr.email as email
		from $report_server_db_name.slt_sf_enrollment sf_enr
		inner join $report_server_db_name.slt_enrollment enr on enr.id=sf_enr.enrollment_id
 		inner join $report_server_db_name.slt_person per on per.email=sf_enr.email
		inner join $report_server_db_name.slt_organization org on org.id=per.org_id
		inner join $report_server_db_name.slt_custom_fields scf on scf.entity_id=org.id and scf.entity_type='organization' and scf.label='Salesforce Org ID' and scf.value!='' 
		inner join $report_server_db_name.slt_cstm_salesforce_settings scss on scss.exp_org_id=org.id
		left join $report_server_db_name.slt_master_enrollment mas_enr on mas_enr.id=sf_enr.master_enrollment_id
		left join $report_server_db_name.slt_profile_list_items spli_os on spli_os.code=mas_enr.overall_status
		left join $report_server_db_name.slt_profile_list_items spli_es on spli_es.code=enr.comp_status
		left join $report_server_db_name.slt_profile_list_items spli_rs on spli_rs.code=enr.reg_status		
		where sf_enr.sf_enrollment_id!=''
		and sf_enr.enrollment_id=enr.id
		and (sf_enr.tp_overall_status!=spli_os.name
		or sf_enr.enrollment_status!=if(enr.reg_status='lrn_crs_reg_cnf',spli_es.name,spli_rs.name) 
		or if(sf_enr.comp_date!='',sf_enr.comp_date,'0000-00-00 00:00:00')!=if(enr.reg_status='lrn_crs_reg_cnf' and enr.comp_status='lrn_crs_cmp_cmp',enr.comp_date,'0000-00-00 00:00:00')
		or if(sf_enr.cancel_date!='',sf_enr.cancel_date,'0000-00-00 00:00:00')!=if(enr.reg_status='lrn_crs_reg_can',enr.reg_status_date,'0000-00-00 00:00:00')
		) LIMIT $enr_limit_to_post_in_sf";

		expDebug::dPrint('Get Latest Enrollment Updation - Query String $enr_get_upd_str = ' . $enr_get_upd_str , 4);
			
		$enr_get_upd_response_obj = db_query($enr_get_upd_str);
		
		expDebug::dPrint('Get Latest Enrollment Updation - Query Response Object $enr_get_upd_response_obj = ' . print_r($enr_get_upd_response_obj, true) , 4);
		
		$enr_get_upd_results = $enr_get_upd_response_obj->fetchAll();
		
		expDebug::dPrint('Get Latest Enrollment Updation - Query Results $enr_get_upd_results = ' . print_r($enr_get_upd_results, true) , 4);
		
		$no_of_enr=$enr_get_upd_response_obj->rowCount();  
		
		expDebug::dPrint('$no_of_enr = ' . $no_of_enr , 4);
		 
		unset($txn);
		
		if($no_of_enr > 0) {   
			//Send updated enrollments to salesforce
			moveUpdatedEnrollmentsToSalesforce($enr_get_upd_results);		 	 
		} else {
			expDebug::dPrint('No enrollments available in LMS to syncup with Salesforce', 4);
		} 
		

	}catch (Exception $ex) {
		$txn->rollback();
		unset($txn);
		watchdog_exception('syncEnrollmentUpdationDetailstoSalesforce', $ex);
		expDebug::dPrint('syncEnrollmentUpdationDetailstoSalesforce $Exception Error = '.$ex  , 4);
		//expertusErrorThrow($ex);
	}
	unset($txn);
} 
 

/**
 ** Implementaion of moveUpdatedEnrollmentsToSalesforce().
 ** Update Enrollment Details from LMS to Salesforce Custom Object for Reporting Feature
 **/

function moveUpdatedEnrollmentsToSalesforce($result){

	$txn1 = db_transaction();

	try{

		//Connect Salesforce and get oAuth Details and Session and Location Variable of Salesforce

		require_once $_SERVER["DOCUMENT_ROOT"].'/sites/all/modules/override/salesforce_integration/salesforce_integration_chatter.inc';  

		$email_str="";

		$customObjectPrefix=variable_get('salesforce_namespace_prefix','');
		 
		if($customObjectPrefix!=''){			
			$sf_cus_field_tp_oveall_status=$customObjectPrefix.'__'.'TP_Overall_Status__c';			
			$sf_cus_field_comp_date=$customObjectPrefix.'__'.'Completion_Date__c';
			$sf_cus_field_cancel_date=$customObjectPrefix.'__'.'Cancellation_Date__c';
			$sf_cus_field_enrollment_status=$customObjectPrefix.'__'.'Enrollment_Status__c'; 
		}else{			
			$sf_cus_field_tp_oveall_status='TP_Overall_Status__c';
			$sf_cus_field_comp_date='Completion_Date__c';
			$sf_cus_field_cancel_date='Cancellation_Date__c';
			$sf_cus_field_enrollment_status='Enrollment_Status__c';  
		}

		
		//$customObjectAPIName='Training_Record__c';
		
		$customObjectAPIName=trim(variable_get('salesforce_object_api_name','')); 
		expDebug::dPrint("Salesforce Custom Object API Name : ".$customObjectAPIName, 5);  
		
		if(trim($customObjectAPIName)=='' || empty($customObjectAPIName)){
			$customObjectAPIName='Training_Record__c';
			expDebug::dPrint('No saleforce custom object name configured in LMS to update enrollments in salesforce. So taking the default salesforce object name'.$customObjectAPIName.' to update', 1);
		} 
		
		if($customObjectPrefix!=''){
			$customObjectAPIName=$customObjectPrefix.'__'.$customObjectAPIName;
		}
		
		$sfObjResponse=array();
		
		$sf_oauth_org_arr=array();
			
		for($i=0;$i<count($result);$i++){

			$fields = array (
					$sf_cus_field_tp_oveall_status => htmlspecialchars($result[$i]->tp_overall_status),
					$sf_cus_field_enrollment_status=> htmlspecialchars($result[$i]->enrollment_status),	 
					$sf_cus_field_comp_date =>  ($result[$i]->comp_date!='' && $result[$i]->comp_date!='0000-00-00 00:00:00' ? date('m/d/Y',strtotime($result[$i]->comp_date)) : ' '),
					$sf_cus_field_cancel_date =>  ($result[$i]->cancel_date!='' && $result[$i]->cancel_date!='0000-00-00 00:00:00' ? date('m/d/Y',strtotime($result[$i]->cancel_date)) : ' ')
					
			);
			
			
			
			$org_id=$result[$i]->org_id;
			$org_name=$result[$i]->org_name;
			$user_email=$result[$i]->email;
				
			$sf_org_det_arr=array();
			 
			$sf_org_det_arr['consumer_key']=trim($result[$i]->scss_consumer_key);
			$sf_org_det_arr['consumer_secret_key']=trim($result[$i]->scss_consumer_secret_key);
			$sf_org_det_arr['system_user']=trim($result[$i]->scss_system_user);
			$sf_org_det_arr['system_user_pwd']=trim($result[$i]->scss_system_user_pwd);
			
			 
			if (array_key_exists($org_id, $sf_oauth_org_arr)) {
				expDebug::dPrint("Already generated oauth details for this org - $org_name. Using that oauth details",5);
				$oAuth_token=$sf_oauth_org_arr[$org_id];
			
			}else{
			
				expDebug::dPrint("Generate oauth details for - $org_name ",5);
				 
				$oAuth_token = get_sf_oauth_token_response("",$sf_org_det_arr);
			
				if($oAuth_token!='' && isset($oAuth_token) && !empty($oAuth_token) &&  $oAuth_token['instance_url']!="" && $oAuth_token['access_token']!=""){
					$sf_oauth_org_arr[$org_id]=$oAuth_token;
				}
			}
			 
			expDebug::dPrint('$oAuth_token_response = ' . print_r($oAuth_token, true),5);
				
			
			if(!isset($oAuth_token) || $oAuth_token['instance_url']=="" || $oAuth_token['access_token']=="" ){
				expDebug::dPrint("Not getting oAuth_token_response for $org_name - $user_email - Sf org credentials ".print_r($sf_org_det_arr,true) ,5);
				$sfObjResponse[$i]->id='';
			}else{
				
				$sf_enr_res_id=$result[$i]->sf_enrollment_res_obj_id;
					
				expDebug::dPrint('$sf_enr_res_id = ' . $sf_enr_res_id , 5);
					
				$sync_custom_obj_feed_res=do_curl_post_for_custom_object($oAuth_token,"/services/data/v20.0/sobjects/$customObjectAPIName/$sf_enr_res_id/",$fields,"1");
				
				expDebug::dPrint('$sync_custom_obj_feed_res = ' . print_r($sync_custom_obj_feed_res, true),4);
				
				if($sync_custom_obj_feed_res=="1"){
					$sfObjResponse[$i]->id=$sf_enr_res_id;
				}else{
					$sfObjResponse[$i]->id="";
				}
			} 
			
			
		} 

		expDebug::dPrint('OAuth details for all org - $sf_oauth_org_arr = ' . print_r($sf_oauth_org_arr, true),4);
		
		expDebug::dPrint('$sfObjResponse= ' . print_r($sfObjResponse, true) , 4);


		//Update Salesforce Response Id in slt_sf_enrollment table (i.e we are considering as salesforce enrollment id)

		if(count($sfObjResponse)>0){  
			
			$up_str="";
				
			foreach ($sfObjResponse as $i => $sfObjResponseVal) { 
				
				$sf_response_id=$sfObjResponseVal->id;
				$sf_id=$result[$i]->sf_id;
				 
				if($sf_response_id!=''){ 					
					$tmp_comp_date = !empty($result[$i]->comp_date) ? "'".$result[$i]->comp_date."'" : "NULL";
					$tmp_cancel_date = !empty($result[$i]->cancel_date) ? "'".$result[$i]->cancel_date."'" : "NULL";  
					$up_str.="UPDATE slt_sf_enrollment SET tp_overall_status='".$result[$i]->tp_overall_status."',enrollment_status='".$result[$i]->enrollment_status."',comp_date=$tmp_comp_date,cancel_date=$tmp_cancel_date, sf_enrollment_id='".$sf_response_id."',updated_on=now() WHERE id='".$sf_id."';";
				}else{
					expDebug::dPrint('There is no salesforce response id for sf_id = '.$sf_id , 4);
				} 
			} 
				
			expDebug::dPrint('Update Process - Bulk Update $up_str= ' . print_r($up_str, true) , 4);
				
			if($up_str!='') {
				$update_qry_response = db_query($up_str);
				expDebug::dPrint('$update_qry_response= ' . print_r($update_qry_response, true) , 4);
			}else{
				expDebug::dPrint('No update id available from salesforce custom object to LMS for updation', 1);
			}
		}
		
		unset($txn1);
			

	}catch (Exception $ex) {
		$txn1->rollback();
		unset($txn1);
		watchdog_exception('moveUpdatedEnrollmentsToSalesforce', $ex);
		expDebug::dPrint('moveUpdatedEnrollmentsToSalesforce $Exception Error = '.$ex  , 4);
		//expertusErrorThrow($ex);
	}

	unset($txn1);
}

/*
 * do_curl_post() - POST Method : Get salesforce details by sending oauth token details using curl
*/

function do_curl_post_for_custom_object($oAuth_token,$url,$post_params,$sync_option=""){

	try{

		if(empty($oAuth_token) || empty($url)){
			
			expDebug::dPrint("No oAuth_token / url available. Please check. oAuth_token : $oAuth_token, url : $url",4);
			return NULL;
		}

		$post_field_string='';
		
		if(isset($post_params)){ 
			$post_field_string = json_encode($post_params); 
		}

		expDebug::dPrint('$post_field_string after encode  = ' . print_r($post_field_string, true),4);
		
		$curl_post_url=$oAuth_token['instance_url'].$url;

		expDebug::dPrint('$curl_post_url = ' . print_r($curl_post_url, true),4);
		
		expDebug::dPrint('$post_field_string = ' . print_r($post_field_string, true),4);

		$curl = curl_init($curl_post_url); 
		
		curl_setopt($curl, CURLOPT_CONNECTTIMEOUT, 10);
		curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, false);   
		curl_setopt($curl, CURLOPT_POSTFIELDS, $post_field_string); 
		curl_setopt($curl, CURLOPT_HEADER, false);  
		curl_setopt($curl, CURLOPT_HTTPHEADER,array("Authorization: ".$oAuth_token['token_type']." ".$oAuth_token['access_token'],"Content-type: application/json"));
		 
		if($sync_option=="1"){ 
			curl_setopt($curl, CURLOPT_CUSTOMREQUEST, "PATCH");
		}else{
			curl_setopt($curl, CURLOPT_RETURNTRANSFER, true); 
			curl_setopt($curl, CURLOPT_POST, true);
		}  
		
		$curl_response = curl_exec($curl);

		expDebug::dPrint('$curl_response = ' . print_r($curl_response, true),4);

		$status = curl_getinfo($curl, CURLINFO_HTTP_CODE);
		curl_close($curl);
		if(empty($curl_response)) return NULL;
		return json_decode($curl_response,true);
			
	}catch (Exception $ex) {
		watchdog_exception('do_curl_post_for_custom_object', $ex);
		expertusErrorThrow($ex);
	}

}

/**
 ** Implementaion of insertLmsEnrollmentsToSalesforce(). 
 ** Insert Enrollment Details from LMS to Salesforce Custom Object for Reporting Feature
 **/

function insertLmsEnrollmentsToSalesforce(){

	$txn = db_transaction();

	try{

		
		$config=getConfig("exp_sp");
		
		$sf_post_data_limit=trim($config["sf_post_data_limit"]);
			
		expDebug::dPrint('$sf_post_data_limit= '.print_r($sf_post_data_limit,true), 5);
		
		if($sf_post_data_limit!=''){
			$enr_limit_to_post_in_sf=$sf_post_data_limit;
		}else{
			$enr_limit_to_post_in_sf=200;
		}
		
		expDebug::dPrint('$enr_limit_to_post_in_sf= '.print_r($enr_limit_to_post_in_sf,true), 5);
		
		
		//Connect Salesforce and get oAuth Details and Session and Location Variable of Salesforce

		require_once $_SERVER["DOCUMENT_ROOT"].'/sites/all/modules/override/salesforce_integration/salesforce_integration_chatter.inc'; 

		//Get completion details from Enrollment table

		$select = db_select('slt_sf_enrollment','sf_enr');
		
		$select->innerJoin('slt_person', 'per', 'per.email=sf_enr.email');
		$select->innerJoin('slt_organization', 'org', 'org.id=per.org_id');
		$select->innerJoin('slt_custom_fields', 'scf', 'scf.entity_id=org.id and scf.entity_type=\'organization\' and scf.label=\'Salesforce Org ID\' and scf.value!=\'\'');
		$select->innerJoin('slt_cstm_salesforce_settings', 'scss', 'scss.exp_org_id=org.id');
		
		
		$select->addField('sf_enr','id', 'sf_id');

		$select->addField('sf_enr','tp_code', 'tp_code');
		$select->addField('sf_enr','tp_title', 'tp_title');
		$select->addField('sf_enr','tp_type', 'tp_type');
		$select->addField('sf_enr','tp_overall_status', 'tp_overall_status');
		$select->addField('sf_enr','course_code', 'course_code');
		$select->addField('sf_enr','course_title', 'course_title');

		$select->addField('sf_enr','class_code', 'class_code');
		$select->addField('sf_enr','class_title', 'class_title');
		$select->addField('sf_enr','delivery_type', 'delivery_type');
		$select->addField('sf_enr','email', 'email');
		$select->addField('sf_enr','enrollment_status', 'enrollment_status');
		$select->addField('sf_enr','reg_date', 'reg_date');
		$select->addField('sf_enr','comp_date', 'comp_date');
		$select->addField('sf_enr','cancel_date', 'cancel_date');
		
		$select->addField('org','id', 'org_id'); 
		$select->addField('org','name', 'org_name');
		$select->addField('scss','consumer_key', 'scss_consumer_key');
		$select->addField('scss','consumer_secret_key', 'scss_consumer_secret_key');
		$select->addField('scss','system_user', 'scss_system_user');
		$select->addField('scss','system_user_pwd', 'scss_system_user_pwd');
		

		$select->where("(sf_enr.sf_enrollment_id IS NULL) OR (sf_enr.sf_enrollment_id='') OR (sf_enr.sf_enrollment_id=' ')");

		//$select->range(0, 200);
		$select->range(0, $enr_limit_to_post_in_sf);

		expDebug::dPrintDBAPI('$select obj = ' , $select);

		$result =  $select->execute()->fetchAll();

		expDebug::dPrint('$result= ' . print_r($result, true) , 4);

		if(count($result)<=0){
			expDebug::dPrint('No enrollments found in LMS to post in salesforce custom object', 1);
			return false;
		} 

		$email_str="";

		$customObjectPrefix=variable_get('salesforce_namespace_prefix','');
		 
		if($customObjectPrefix!=''){
			$sf_cus_field_tp_code=$customObjectPrefix.'__'.'TP_Code__c';
			$sf_cus_field_tp_title=$customObjectPrefix.'__'.'TP_Title__c';
			$sf_cus_field_tp_delivery_type=$customObjectPrefix.'__'.'TP_Delivery_Type__c';
			$sf_cus_field_tp_oveall_status=$customObjectPrefix.'__'.'TP_Overall_Status__c';
			$sf_cus_field_course_code=$customObjectPrefix.'__'.'Course_Code__c';
			$sf_cus_field_course_title=$customObjectPrefix.'__'.'Course_Title__c';
			$sf_cus_field_class_code=$customObjectPrefix.'__'.'Class_Code__c';
			$sf_cus_field_class_title=$customObjectPrefix.'__'.'Class_Title__c';
			$sf_cus_field_delivery_type=$customObjectPrefix.'__'.'Delivery_Type__c';
			$sf_cus_field_email=$customObjectPrefix.'__'.'Email__c';
			$sf_cus_field_enrollment_status=$customObjectPrefix.'__'.'Enrollment_Status__c';
			$sf_cus_field_reg_date=$customObjectPrefix.'__'.'Registration_Date__c';
			$sf_cus_field_comp_date=$customObjectPrefix.'__'.'Completion_Date__c';
			$sf_cus_field_cancel_date=$customObjectPrefix.'__'.'Cancellation_Date__c';
				
		}else{
			$sf_cus_field_tp_code='TP_Code__c';
			$sf_cus_field_tp_title='TP_Title__c';
			$sf_cus_field_tp_delivery_type='TP_Delivery_Type__c';
			$sf_cus_field_tp_oveall_status='TP_Overall_Status__c';
			$sf_cus_field_course_code='Course_Code__c';
			$sf_cus_field_course_title='Course_Title__c';
			$sf_cus_field_class_code='Class_Code__c';
			$sf_cus_field_class_title='Class_Title__c';
			$sf_cus_field_delivery_type='Delivery_Type__c';
			$sf_cus_field_email='Email__c';
			$sf_cus_field_enrollment_status='Enrollment_Status__c';
			$sf_cus_field_reg_date='Registration_Date__c';
			$sf_cus_field_comp_date='Completion_Date__c';
			$sf_cus_field_cancel_date='Cancellation_Date__c'; 
		}
		
		
		//$customObjectAPIName='Training_Record__c';
		
		$customObjectAPIName=trim(variable_get('salesforce_object_api_name','')); 
		expDebug::dPrint("Salesforce Custom Object API Name : ".$customObjectAPIName, 5);  
		
		if(trim($customObjectAPIName)=='' || empty($customObjectAPIName)){
			$customObjectAPIName='Training_Record__c';
			expDebug::dPrint('No saleforce custom object name configured in LMS to insert enrollments in salesforce.So taking the default salesforce object name '.$customObjectAPIName.' to update', 1);
		}  
		
		if($customObjectPrefix!=''){
			$customObjectAPIName=$customObjectPrefix.'__'.$customObjectAPIName;
		}		


		
		
		$sf_oauth_org_arr=array();
		
		
		for($i=0;$i<count($result);$i++){

			$fields = array (
					$sf_cus_field_tp_code => htmlspecialchars($result[$i]->tp_code),
					$sf_cus_field_tp_title => htmlspecialchars($result[$i]->tp_title),
					$sf_cus_field_tp_delivery_type => htmlspecialchars($result[$i]->tp_type),
					$sf_cus_field_tp_oveall_status => htmlspecialchars($result[$i]->tp_overall_status),
					$sf_cus_field_course_code =>  htmlspecialchars($result[$i]->course_code),
					$sf_cus_field_course_title =>  htmlspecialchars($result[$i]->course_title),
					$sf_cus_field_class_code => htmlspecialchars($result[$i]->class_code),
					$sf_cus_field_class_title =>  htmlspecialchars($result[$i]->class_title),
					$sf_cus_field_delivery_type =>  htmlspecialchars($result[$i]->delivery_type),
					$sf_cus_field_email =>  htmlspecialchars($result[$i]->email),
					$sf_cus_field_enrollment_status=> htmlspecialchars($result[$i]->enrollment_status),
					$sf_cus_field_reg_date => ($result[$i]->reg_date!='' && $result[$i]->reg_date!='0000-00-00 00:00:00' ? date('m/d/Y',strtotime($result[$i]->reg_date)) : ''),
					$sf_cus_field_comp_date =>  ($result[$i]->comp_date!='' && $result[$i]->comp_date!='0000-00-00 00:00:00' ? date('m/d/Y',strtotime($result[$i]->comp_date)) : ''),
					$sf_cus_field_cancel_date =>  ($result[$i]->cancel_date!='' && $result[$i]->cancel_date!='0000-00-00 00:00:00' ? date('m/d/Y',strtotime($result[$i]->cancel_date)) : '')
			);   
		   
			$org_id=$result[$i]->org_id;
			$org_name=$result[$i]->org_name;
			$user_email=$result[$i]->email;
			
		   $sf_org_det_arr=array();  
		   
		   $sf_org_det_arr['consumer_key']=trim($result[$i]->scss_consumer_key);
		   $sf_org_det_arr['consumer_secret_key']=trim($result[$i]->scss_consumer_secret_key);
		   $sf_org_det_arr['system_user']=trim($result[$i]->scss_system_user);
		   $sf_org_det_arr['system_user_pwd']=trim($result[$i]->scss_system_user_pwd); 
		    
		   
		   if (array_key_exists($org_id, $sf_oauth_org_arr)) {
		    	expDebug::dPrint("Already generated oauth details for this org - $org_name. Using that oauth details",5);
		    	$oAuth_token=$sf_oauth_org_arr[$org_id]; 
		   	
		   }else{
		   	
		      	expDebug::dPrint("Generate oauth details for - $org_name ",5);
		      			   	
			   	$oAuth_token = get_sf_oauth_token_response("",$sf_org_det_arr);  
			   		
			   	if($oAuth_token!='' && isset($oAuth_token) && !empty($oAuth_token) &&  $oAuth_token['instance_url']!="" && $oAuth_token['access_token']!=""){
			   		$sf_oauth_org_arr[$org_id]=$oAuth_token;
			   	} 
		   } 
		   
		   expDebug::dPrint('$oAuth_token_response = ' . print_r($oAuth_token, true),5);  
 			 
				
			if(!isset($oAuth_token) || $oAuth_token['instance_url']=="" || $oAuth_token['access_token']=="" ){
				expDebug::dPrint("Not getting oAuth_token_response for $org_name - $user_email - Sf org credentials ".print_r($sf_org_det_arr,true) ,5); 
				$sfObjResponse[$i]->id='';				
			}else{ //Post data
				$custom_obj_feed_res=do_curl_post_for_custom_object($oAuth_token,"/services/data/v20.0/sobjects/$customObjectAPIName/",$fields);					
				expDebug::dPrint('$custom_obj_feed_res = ' . print_r($custom_obj_feed_res, true),4);					
				$tmp_sf_response_id=trim($custom_obj_feed_res["id"]);					
				$sfObjResponse[$i]->id=$tmp_sf_response_id;				
			}  
			
		} 
		
		
		expDebug::dPrint('OAuth details for all org - $sf_oauth_org_arr = ' . print_r($sf_oauth_org_arr, true),4);

		expDebug::dPrint('$sfObjResponse = ' . print_r($sfObjResponse, true) , 4);


		//Update Salesforce Response Id in slt_sf_enrollment table (i.e we are considering as salesforce enrollment id)

		if(count($sfObjResponse)>0){
				
			$up_str="";
				
			foreach ($sfObjResponse as $i => $sfObjResponseVal) {
				$sf_response_id=$sfObjResponseVal->id;
				$sf_id=$result[$i]->sf_id;
				if($sf_response_id!=''){
					$up_str.="UPDATE slt_sf_enrollment SET sf_enrollment_id='".$sf_response_id."',updated_on=now() WHERE id='".$sf_id."';";
				}
			}
				
			expDebug::dPrint('Bulk Update $up_str= ' . print_r($up_str, true) , 4);
				
			if($up_str!='') {
				$update_qry_response = db_query($up_str);
				expDebug::dPrint('$update_qry_response= ' . print_r($update_qry_response, true) , 4);
			}else{
				expDebug::dPrint('No insert id from salesforce custom object to LMS for updation', 1);
			}
		}
			

	}catch (Exception $ex) {
		watchdog_exception('insertLmsEnrollmentsToSalesforce', $ex);
		expDebug::dPrint('insertLmsEnrollmentsToSalesforce $Exception Error = '.$ex  , 4);
		//expertusErrorThrow($ex);
	}

	unset($txn);
}

 
/**
 ** Implementaion of checkCustomObjectInSalesforce().
 ** Check whether custom object available or not in salesforce. If custom object exists, return true else return false
 **/

function checkCustomObjectInSalesforce($session_id,$serverURL,$customObjectAPIName,$customObjectPrefix=""){
	try{
		
		expDebug::dPrint('$session_id = ' . $session_id,4);
		expDebug::dPrint('$serverURL = ' . $serverURL,4);
		expDebug::dPrint('$customObjectAPIName = ' . $customObjectAPIName,4);
		expDebug::dPrint('$customObjectPrefix = ' . $customObjectPrefix,4);
		
		$mySforceConnectionforCO = getSalesforceConnection($session_id,$serverURL); 
		expDebug::dPrint('Saleforce Connection -  $mySforceConnectionforCO= ' . print_r($mySforceConnectionforCO, true) , 4);
		
		if($customObjectPrefix!=''){
			$customFieldEmail=$customObjectPrefix.'__'.'Email__c';
		}else{
			$customFieldEmail='Email__c';
		}  
		
		$sfco_qry = "SELECT $customFieldEmail FROM $customObjectAPIName limit 1";
		
		expDebug::dPrint('Training Object Query $sfco_qry = ' . $sfco_qry,4);
		
		$sfco_qry_res=$mySforceConnectionforCO->query($sfco_qry,false);
		
		expDebug::dPrint('Training Object Result $sfco_qry_res = ' . print_r($sfco_qry_res, true),4); 
				
		
		$email_existing=html_entity_decode(strip_tags($sfco_qry_res->records[0]->any));
		
		expDebug::dPrint('Email of Training $email_existing = ' . print_r($email_existing, true),4);
		
		if($email_existing!='' || $email_existing=="" || is_null($email_existing)){
			return true;
		}else{
			return false;
		}
		
	}catch (Exception $ex) {
		expDebug::dPrint("No custom object '$customObjectAPIName' available in Salesforce",1);
		expDebug::dPrint('$Exception Error = '.$ex  , 4);
	 	watchdog_exception('checkCustomObjectInSalesforce', $ex);
		//expertusErrorThrow($ex);
		return false;
	}
}

/**
 ** Implementaion of getSalesforceMetaDataConnection().
 ** Get salesforce meta connection for handling insert/update data in salesforce object 
 **/ 

function getSalesforceMetaDataConnection(){
	try{
		//Connect Salesforce and get oAuth Details and Session and Location Variable of Salesforce

		require_once $_SERVER["DOCUMENT_ROOT"].'/sites/all/modules/override/salesforce_integration/salesforce_integration_chatter.inc';

		$oAuth_token = get_sf_oauth_token_response("1");
		expDebug::dPrint('$oAuth_token_response = ' . print_r($oAuth_token, true),4);
			
		if(!isset($oAuth_token) || $oAuth_token['instance_url']=="" || $oAuth_token['access_token']=="" ){
			expDebug::dPrint('Not getting oAuth_token_response',4);
			return false;
		}
			
		require_once('soapclient/SforcePartnerClient.php');
		require_once ('soapclient/SforceMetadataClient.php');
		require_once ('soapclient/SforceMetaObject.php');

		$serverURL = $oAuth_token["instance_url"]."/services/Soap/u/25.0/";
		$session_id = $oAuth_token["access_token"];
		$serverURLMeta = $oAuth_token["instance_url"]."/services/Soap/m/25.0/";

		$protocol="http://";
			
		// IIS scenario
		if(array_key_exists('HTTPS', $_SERVER) && $_SERVER['HTTPS'] === 'off'){
			$protocol="http://";
		}else if(array_key_exists('HTTPS', $_SERVER) && $_SERVER['HTTPS'] !== ''){
			$protocol="https://";
		}
		
		//$protocol="https://"; //for tmp
			
		$mySforceConnection = new SforcePartnerClient();

		$mySoapClient = $mySforceConnection->createConnection($protocol.$_SERVER["SERVER_NAME"]."/sites/all/modules/override/salesforce_integration/soapclient/partner.wsdl.xml");

		expDebug::dPrint('$mySoapClient= ' . print_r($mySoapClient, true) , 4);

		$loginResult=new stdclass();
			
		$loginResult->sessionId=$session_id;
		$loginResult->serverUrl=$serverURL;
		$loginResult->metadataServerUrl=$serverURLMeta;

		expDebug::dPrint('$loginResult= ' . print_r($loginResult, true) , 4);

		$myMetadataConnection = new SforceMetadataClient($protocol.$_SERVER["SERVER_NAME"]."/sites/all/modules/override/salesforce_integration/soapclient/metadata.wsdl.xml", $loginResult, $mySforceConnection);

		expDebug::dPrint('Custom Object Meta Connection - $myMetadataConnection  = ' . print_r($myMetadataConnection, true) , 4);

		$met_ret_arr= array();
		$met_ret_arr["connection"]=(object)$myMetadataConnection;
		$met_ret_arr["sessionId"]=$session_id;
		$met_ret_arr["serverUrl"]=$serverURL;
		
		expDebug::dPrint('Custom Object Meta Connection Details - $met_ret_arr  = ' . print_r($met_ret_arr, true) , 4);
		
		return $met_ret_arr;


	}catch (Exception $ex) {
		watchdog_exception('getSalesforceMetaDataConnection', $ex);
		expDebug::dPrint('$Exception Error = '.$ex  , 4);
		//expertusErrorThrow($ex);
	}
}

function getCustomObjectAPIName($tmp_cus_obj_name=''){
	try{
		
		expDebug::dPrint("tmp_cus_obj_name : ".$tmp_cus_obj_name, 5);
		
		if($tmp_cus_obj_name!=''){
			$salesforce_object_name=$tmp_cus_obj_name; 
		}else{
			$salesforce_object_name=variable_get('salesforce_object_name','');
		} 
		expDebug::dPrint("Salesforce Custom Object Name : ".$salesforce_object_name, 5);
		
		$customObjectName=trim($salesforce_object_name);
		$sf_cus_obj_api_name='';
		if($customObjectName!=''){
			//Get API Name 
			$sf_cus_obj_name_arr=explode(' ',$salesforce_object_name); 
			$sf_cus_obj_name_arr=array_map('trim',$sf_cus_obj_name_arr);
			expDebug::dPrint("sf_cus_obj_name_arr : ".print_r($sf_cus_obj_name_arr,true), 5); 
			$sf_cus_obj_api_name=implode('_',$sf_cus_obj_name_arr); 
			$sf_cus_obj_api_name.='__c';
			
		}
		 
		expDebug::dPrint("sf_cus_obj_api_name : ".print_r($sf_cus_obj_api_name,true), 5);
		
		return $sf_cus_obj_api_name;
		
	}catch (Exception $ex) {
		watchdog_exception('getCustomObjectAPIName', $ex);
		expDebug::dPrint('$Exception Error = '.$ex  , 4);
		//expertusErrorThrow($ex);
	}
}



/**
 ** Implementaion of createSFCusomObject().
 ** Create the saleforce custom object (table) for holding enrollments in salesforce
 ** Argument $check_cus_obj_exists - if values has 1 , it will check whether custom object already exist or not in salesforce and 
 ** return 1 if custom object exists, or else return 0 if not exists
 **/

function createSFCusomObject($check_cus_obj_exists=""){
	try{
		 
		// Get Salesforce Meta Custom Object Connection
		
		$met_ret_arr=getSalesforceMetaDataConnection();
		
		$myMetadataConnection=$met_ret_arr["connection"];
		$session_id=trim($met_ret_arr["sessionId"]);
		$serverURL=trim($met_ret_arr["serverUrl"]);
		
		if(empty($myMetadataConnection) || is_null($myMetadataConnection) || $myMetadataConnection==""){ 
			expDebug::dPrint("Not getting meta data connection from salesforce", 1);
			return "-1"; //Error
			exit();
		} 
		
		//Object Definition  
		
		$customObjectName=trim(variable_get('salesforce_object_name','')); 		
		$customObjectAPIName=trim(variable_get('salesforce_object_api_name','')); 
		expDebug::dPrint("customObjectName : ".print_r($customObjectName,true), 5);
		expDebug::dPrint("customObjectAPIName : ".print_r($customObjectAPIName,true), 5);
		 
		 /*$customObjectName='Training Record Sep';
		$customObjectAPIName='Training_Record_Sep__c';*/
		
		$customObjectPrefix=variable_get('salesforce_namespace_prefix','');
		$customCurrentUserFormula="Email__c";
		
		if($customObjectPrefix!=''){
			$customObjectAPIName=$customObjectPrefix.'__'.$customObjectAPIName;
			$customCurrentUserFormula=$customObjectPrefix.'__'.'Email__c';
		}
		
		$customCurrentUserFormulaStr='IF('.$customCurrentUserFormula.' = $User.Email, 1 , 0)';	
		expDebug::dPrint("customCurrentUserFormulaStr : ".$customCurrentUserFormulaStr, 5);
		 
		 
		/*$customObjectLableName='Completed Training';
		$customObejctDesc='Object to store completed trainings from ExpertusONE';*/

		//$customObjectLableName='Training Record';
		
		$customObjectLableName=$customObjectName;
		$customObejctDesc='Object to store trainings from ExpertusONE'; 
		
		/*$customObjectLableName='Training Record Sep';
		$customObejctDesc='Sep Object to store trainings from ExpertusONE';*/
		
		//Check object already exists or not 

		$ret_val=checkCustomObjectInSalesforce($session_id,$serverURL,$customObjectAPIName,$customObjectPrefix);  
		if($ret_val){
			expDebug::dPrint("Custom Object $customObjectName already exists in Salesforce", 1);
			//return "1";
		}else{
			expDebug::dPrint("Custom Object $customObjectName doesnot exists in Salesforce", 1);
		} 
		
		if($check_cus_obj_exists!="" && $check_cus_obj_exists=="1"){ //Validation purpose, which has added whether custom object exists or not in salesforce
		
			expDebug::dPrint("Sending to form ", 1);
			//return "0";
			return $ret_val;
		} 

		
		//Create object and setup properties for custom object
		
		$customObject = new SforceCustomObject(); 

		$customObject->fullName = $customObjectAPIName;
		$customObject->deploymentStatus = DEPLOYMENT_STATUS_DEPLOYED;
		$customObject->setDescription($customObejctDesc);
		$customObject->setEnableActivities(true);
		$customObject->setEnableDivisions(false);
		$customObject->setEnableHistory(false);
		$customObject->setEnableReports(true);
		$customObject->setHousehold(false);
		$customObject->setLabel($customObjectLableName);
		$customObject->pluralLabel = $customObjectLableName.'s';
		$customObject->sharingModel = SHARING_MODEL_READWRITE;

		//Set Auto Increment Field for Standard Field 'ID'

		$customSdField= new SforceCustomField();

		$customSdField->setFullName($customObjectName.'.ID');
		$customSdField->setDescription('');
		$customSdField->setLabel('ID');
		$customSdField->setType('AutoNumber');
		$customSdField->setDisplayFormat('E1-{000000}');
		$customObject->nameField = $customSdField;

		$sfCusObjCreationResponse=$myMetadataConnection->create($customObject);

		expDebug::dPrint('$sfCusObjCreationResponse = ' . print_r($sfCusObjCreationResponse, true) , 4);

		// Create Custom Fields Under Custom Object
				
		$met_ret_arr=getSalesforceMetaDataConnection(); 
		expDebug::dPrint('Custom Field Connection - $met_ret_arr  = ' . print_r($met_ret_arr, true) , 4);
		$myMetadataConnection=$met_ret_arr["connection"];
		
		// TP Code
		$customField = new SforceCustomField();
		$customField->setFullName($customObjectAPIName.'.TP_Code__c');
		$customField->setDescription('Code of Training Plan');
		$customField->setLabel('Training Plan Code');
		$customField->setType('TextArea');
		$customFieldAddResult=$myMetadataConnection->create($customField);
		expDebug::dPrint('TP Code - $customFieldAddResult= ' . print_r($customFieldAddResult, true) , 4);
		
		
		// TP Title
		$customField = new SforceCustomField();
		$customField->setFullName($customObjectAPIName.'.TP_Title__c');
		$customField->setDescription('Title of Training Plan');
		$customField->setLabel('Training Plan Title');
		$customField->setType('LongTextArea');
		$customField->setVisibleLines('3');
		$customField->setLength(32768);
		$customFieldAddResult=$myMetadataConnection->create($customField);
		expDebug::dPrint('TP Title - $customFieldAddResult= ' . print_r($customFieldAddResult, true) , 4);
		
		
		// TP Delivery Type
		$customField = new SforceCustomField();
		$customField->setFullName($customObjectAPIName.'.TP_Delivery_Type__c');
		$customField->setDescription('Delivery type of Training Plan');
		$customField->setLabel('Training Plan Type');
		$customField->setType('TextArea');
		$customFieldAddResult=$myMetadataConnection->create($customField);
		expDebug::dPrint('TP Delivery Type - $customFieldAddResult= ' . print_r($customFieldAddResult, true) , 4);
		
		// TP Overall Status
		$customField = new SforceCustomField();
		$customField->setFullName($customObjectAPIName.'.TP_Overall_Status__c');
		$customField->setDescription('Overall status of Training Plan');
		$customField->setLabel('Training Plan Status');
		$customField->setType('TextArea');
		$customFieldAddResult=$myMetadataConnection->create($customField);
		expDebug::dPrint('TP Overall Status - $customFieldAddResult= ' . print_r($customFieldAddResult, true) , 4);
		
		// Course Code
		$customField = new SforceCustomField();
		$customField->setFullName($customObjectAPIName.'.Course_Code__c');
		$customField->setDescription('Course Code of Training');
		$customField->setLabel('Course Code');
		$customField->setType('TextArea');
		$customFieldAddResult=$myMetadataConnection->create($customField);
		expDebug::dPrint('Course Code - $customFieldAddResult= ' . print_r($customFieldAddResult, true) , 4);
		
		// Course Title
		$customField = new SforceCustomField();
		$customField->setFullName($customObjectAPIName.'.Course_Title__c');
		$customField->setDescription('Course Title of Training');
		$customField->setLabel('Course Title');
		$customField->setType('LongTextArea');
		$customField->setVisibleLines('3');
		$customField->setLength(32768);
		$customFieldAddResult=$myMetadataConnection->create($customField);
		expDebug::dPrint('Course Title - $customFieldAddResult= ' . print_r($customFieldAddResult, true) , 4);  
		
		
		// Class Code
		$customField = new SforceCustomField();
		$customField->setFullName($customObjectAPIName.'.Class_Code__c');
		$customField->setDescription('Class Code of Training');
		$customField->setLabel('Class Code');
		$customField->setType('TextArea');
		$customFieldAddResult=$myMetadataConnection->create($customField);
		expDebug::dPrint('Class Code - $customFieldAddResult= ' . print_r($customFieldAddResult, true) , 4);

		// Class Title
		$customField = new SforceCustomField();
		$customField->setFullName($customObjectAPIName.'.Class_Title__c');
		$customField->setDescription('Class Title of Training');
		$customField->setLabel('Class Title');
		$customField->setType('LongTextArea');
		$customField->setVisibleLines('3');
		$customField->setLength(32768);
		$customFieldAddResult=$myMetadataConnection->create($customField);
		expDebug::dPrint('Class Title - $customFieldAddResult= ' . print_r($customFieldAddResult, true) , 4);

		// Delivery type
		$customField = new SforceCustomField();
		$customField->setFullName($customObjectAPIName.'.Delivery_Type__c');
		$customField->setDescription('Delivery type of Training');
		$customField->setLabel('Delivery Type');
		$customField->setType('TextArea');
		$customFieldAddResult=$myMetadataConnection->create($customField);
		expDebug::dPrint('Delivery Type - $customFieldAddResult= ' . print_r($customFieldAddResult, true) , 4);
		
				//Email
		$customField = new SforceCustomField();
		$customField->setFullName($customObjectAPIName.'.Email__c');
		$customField->setDescription('Email');
		$customField->setLabel('Email');
		$customField->setType('Text');
		$customField->setLength(255);
		$customFieldAddResult=$myMetadataConnection->create($customField);
		expDebug::dPrint('Email - $customFieldAddResult= ' . print_r($customFieldAddResult, true) , 4);
		
		
		// Enrollment Status
		$customField = new SforceCustomField();
		$customField->setFullName($customObjectAPIName.'.Enrollment_Status__c');
		$customField->setDescription('Enrollment status of Training');
		$customField->setLabel('Training Status');
		$customField->setType('TextArea');
		$customFieldAddResult=$myMetadataConnection->create($customField);
		expDebug::dPrint('Enrollment Status - $customFieldAddResult= ' . print_r($customFieldAddResult, true) , 4);
		
		
		//Registration Date
		$customField = new SforceCustomField();
		$customField->setFullName($customObjectAPIName.'.Registration_Date__c');
		$customField->setDescription('Registration date of Training');
		$customField->setLabel('Registration Date');
		$customField->setType('Text');
		$customField->setLength(255);
		$customFieldAddResult=$myMetadataConnection->create($customField);
		expDebug::dPrint('Registration Date - $customFieldAddResult= ' . print_r($customFieldAddResult, true) , 4);
		
		//Completion Date
		$customField = new SforceCustomField();
		$customField->setFullName($customObjectAPIName.'.Completion_Date__c');
		$customField->setDescription('Completion date of Training');
		$customField->setLabel('Completion Date');
		$customField->setType('Text');
		$customField->setLength(255);
		$customFieldAddResult=$myMetadataConnection->create($customField);
		expDebug::dPrint('Completion Date - $customFieldAddResult= ' . print_r($customFieldAddResult, true) , 4); 
		
		
		//Cancellation Date
		$customField = new SforceCustomField();
		$customField->setFullName($customObjectAPIName.'.Cancellation_Date__c');
		$customField->setDescription('Cancellation date of Training');
		$customField->setLabel('Cancellation Date');
		$customField->setType('Text');
		$customField->setLength(255);
		$customFieldAddResult=$myMetadataConnection->create($customField);
		expDebug::dPrint('Cancellation Date - $customFieldAddResult= ' . print_r($customFieldAddResult, true) , 4);
		
		
		//Current User
		$customField = new SforceCustomField();
		$customField->setFullName($customObjectAPIName.'.Current_User__c');
		$customField->setDescription('Current User');
		$customField->setLabel('Current User');
		$customField->setType('Number');
		$customField->setFormula($customCurrentUserFormulaStr);
		$customField->setScale(0);
		$customField->setPrecision(0);
		//$customField->setLength(255);
		$customFieldAddResult=$myMetadataConnection->create($customField);
		expDebug::dPrint('Current User - $customFieldAddResult= ' . print_r($customFieldAddResult, true) , 4);
		
		
		//Full Name
		/*$customField = new SforceCustomField();
		$customField->setFullName($customObjectAPIName.'.Full_Name__c');
		$customField->setDescription('Full Name');
		$customField->setLabel('Full Name');
		$customField->setType('TextArea');
		$customFieldAddResult=$myMetadataConnection->create($customField);
		expDebug::dPrint('Full Name - $customFieldAddResult= ' . print_r($customFieldAddResult, true) , 4);*/ 
		
			
		expDebug::dPrint("Created new custom object $customObjectAPIName in salesforce", 1);
          
		return "2";

	}catch (Exception $ex) {
		watchdog_exception('createSFCusomObject', $ex);
		expDebug::dPrint('$Exception Error = '.$ex  , 4);
		//expertusErrorThrow($ex);
	}

} 

?>