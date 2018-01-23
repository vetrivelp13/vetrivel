<?php
include_once $_SERVER["DOCUMENT_ROOT"] . "/sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc";
function getClassDetailsInfo($classId, $sessionId,$vLMSUserId,$callAPI='',$callfrom='') {
  try{
  expDebug::dPrint(" classIdAndSessionId = ". $classId . "-" . $sessionId , 4);
  $is_shown = db_select('slt_course_class','class');
  $is_shown->addField('class','addn_catalog_show','addn_catalog_show');
  $is_shown->condition('class.id', $classId);
  $resultisshown = $is_shown->execute()->fetchfield();
 //$vLMSUserId = getSltpersonUserId();
  $sessionCnt = _getClassSessionCount($classId);
 // $isHasAccessRow = getAccessCount($classId);
  $UserDefaultCurrencyArr = getUserDefaultCurrency();
  $UserPrefCurrCode = $UserDefaultCurrencyArr['currency_code'];
  $UserPrefCurrSym = $UserDefaultCurrencyArr['currency_sym'];
  //expDebug::dPrint('is has access' . print_r($isHasAccessRow, 1), 4);
    //if (!empty($sessionId)) {
  if($sessionCnt) {
    $classDetailSel = db_select('slt_course_class','class');
    $classDetailSel->leftJoin('slt_node_learning_activity','nla','class.id = nla.entity_id AND nla.entity_type = \'cre_sys_obt_cls\'');
    $classDetailSel->leftJoin('slt_course_class_session','ses','class.id = ses.class_id'); 
    $classDetailSel->leftJoin('slt_location','loc','ses.location_id = loc.id');
   // $classDetailSel->leftJoin('slt_state','sname','loc.state = sname.state_code');
    $classDetailSel->leftJoin('slt_country','con','loc.country = con.country_code');
    //$classDetailSel->leftJoin('slt_mro_mapping', 'mro', 'mro.entity_id=class.id AND mro.job_role_id = (SELECT job_role FROM slt_person WHERE id=\''.$vLMSUserId.'\') AND mro.entity_type = \'cre_sys_obt_cls\'');
   // $classDetailSel->leftJoin('slt_catalog_access', 'catacs', 'catacs.entity_id=class.id AND catacs.entity_type = \'cre_sys_obt_cls\'');   
   /* if ($isHasAccessRow) {
     $classDetailSel->leftJoin('slt_group_mapping','group_map','group_map.entity_id=class.id AND group_map.entity_type = \'cre_sys_obt_cls\'');
    } else {
     $classDetailSel->leftJoin('slt_group_mapping','group_map','group_map.entity_id=class.course_id AND group_map.entity_type = \'cre_sys_obt_crs\'');
    }*/
    // $classDetailSel->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');
    
    if($callfrom == 'lrnplan_course_class_list'){
    	$classDetailSel->leftJoin('slt_enrollment','enr','enr.class_id=class.id');
    	$classDetailSel->innerJoin('slt_master_enrollment','menr','enr.master_enrollment_id=menr.id');
    	$classDetailSel->innerJoin('slt_program','prg','prg.id=menr.program_id');
    	$classDetailSel->addField('prg', 'id', 'program_id');
    	$classDetailSel->addField('enr', 'recertify_path', 'recertify_path');
    }
    
    $classDetailSel->addField('class','title','Title');
    $classDetailSel->addField('class','id','classid');
    $classDetailSel->addField('class','course_id','crs_id');
    $classDetailSel->addField('class','code','Code');
    $classDetailSel->addField('class','short_description','Description');
	if($callAPI)
	{
		    $classDetailSel->addExpression('if(class.addn_catalog_show = 1,class.additional_info,null)','additional_info');
	}
	else
	        $classDetailSel->addField('class','additional_info','additional_info');
	$classDetailSel->addField('class','addn_catalog_show','addn_catalog_show');
	$classDetailSel->addField('class','addn_notification_show','addn_notification_show');
	$classDetailSel->addField('class','scheduled_duration','Duration');
    $classDetailSel->addField('class','course_id','courseid');
    $classDetailSel->addField('class','custom4','export_compliance');
    $classDetailSel->addField('class','lang_code','ClsLang'); 
    $classDetailSel->addField('class','delivery_type','delivery_type_code');
    $classDetailSel->addField('class','delivery_type','ClsDeliveryType');  
    $classDetailSel->addField('class','is_compliance','is_compliance'); 
    $classDetailSel->addField('ses','start_date','startdatetime'); 
    $classDetailSel->addField('ses','end_date','enddatetime');
    $classDetailSel->addField('ses','title','sessionTitle');
    $classDetailSel->addField('ses','start_time','sessionStartTime');
    $classDetailSel->addField('ses','end_time','sessionEndTime'); 
    $classDetailSel->addExpression('if(usrmap.mro is not null, usrmap.mro, group_map.mro )', 'mro_id');
    
    $classDetailSel->addField('loc','id','LocationId');
    $classDetailSel->addField('loc','name','LocationName');
    $classDetailSel->addField('loc','addr1','LocationAddr1');
    $classDetailSel->addField('loc','addr2','LocationAddr2');
    $classDetailSel->addField('loc','city','LocationCity');
    $classDetailSel->addExpression('(SELECT  IF(COUNT(state_name)>0,state_name,loc.state)  stname FROM slt_state WHERE state_code= loc.state limit 1)','LocationState');
    $classDetailSel->addField('loc','country','LocationCountry');
    $classDetailSel->addField('con','country_name','LocationCountryName');
    $classDetailSel->addField('loc','zipcode','LocationZip');
    $classDetailSel->addField('loc','phone','LocationPhone');
    $classDetailSel->addField('loc','latitude','Latitude');
    $classDetailSel->addField('loc','longitude','Longitude');
    
    $classDetailSel->addField('nla','node_id','node_id');
    
    $classDetailSel->addField('ses','timezone','TimeZone');
    
    $classDetailSel->addField('class','price','price');
    $classDetailSel->addField('class','currency_type','currency_type');
    $classDetailSel->addField('class','custom4','CurrencyPrefixSymbol'); 
    $classDetailSel->addField('class','custom4','CurrencyPostfixSymbol');
    $classDetailSel->addField('class','max_seats','MaxCapacity');
    $classDetailSel->addField('class','waitlist_count','AvailablibleSeatCount');
    $classDetailSel->addField('class','valid_to','EnrollmentValidity');
    
    $classDetailSel->addField('class','registration_end_on', 'registration_end_on');
    $classDetailSel->addField('class','dedicated_class_flag', 'dedicated_class_flag');
	$classDetailSel->addExpression('slf_convert_price(class.price,class.currency_type,\''.$UserPrefCurrCode.'\')','price');
	$classDetailSel->addExpression('\''.$UserPrefCurrSym.'\'','currency_symbol');
    //filterByUserAccessPrivileges($classDetailSel, 'catacs',$vLMSUserId,'group_map');  
    filterByUserAccessPrivileges($classDetailSel, 'class',$vLMSUserId,'','cre_sys_obt_cls');
    
    // below line commented for Mantis - SQL Injection #31297
    //$classDetailSel->condition('class.id',$classId);
    if($callfrom == 'lrnplan_course_class_list')
    	$classDetailSel->where('class.id=:classid',array(':classid'=>$classId));
	else 
		$classDetailSel->where('class.id=:classid AND (class.dedicated_class_flag = \'N\' or class.dedicated_class_flag is null)',array(':classid'=>$classId));
  }
  else {
    $classDetailSel = db_select('slt_course_class','class');
    $classDetailSel->join('slt_node_learning_activity','nla','class.id = nla.entity_id AND nla.entity_type = \'cre_sys_obt_cls\'');
    $classDetailSel->leftJoin('slt_common_mapping','pre','class.id = pre.id2');
    //$classDetailSel->leftJoin('slt_mro_mapping', 'mro', 'mro.entity_id=class.id AND mro.job_role_id = (SELECT job_role FROM slt_person WHERE id=\''.$vLMSUserId.'\') AND mro.entity_type = \'cre_sys_obt_cls\'');
    //$classDetailSel->leftJoin('slt_catalog_access', 'catacs', 'catacs.entity_id=class.id AND catacs.entity_type = \'cre_sys_obt_cls\'');       
  /*  if ($isHasAccessRow) {
     $classDetailSel->leftJoin('slt_group_mapping','group_map','group_map.entity_id=class.id AND group_map.entity_type = \'cre_sys_obt_cls\'');
    } else {
     $classDetailSel->leftJoin('slt_group_mapping','group_map','group_map.entity_id=class.course_id AND group_map.entity_type = \'cre_sys_obt_crs\'');
    }*/
    //$classDetailSel->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');
    
    if($callfrom == 'lrnplan_course_class_list'){
    	$classDetailSel->leftJoin('slt_enrollment','enr','enr.class_id=class.id');
    	$classDetailSel->leftJoin('slt_master_enrollment','menr','enr.master_enrollment_id=menr.id');
    	$classDetailSel->leftJoin('slt_program','prg','prg.id=menr.program_id');
    	$classDetailSel->addField('prg', 'id', 'program_id');
    	$classDetailSel->addField('enr', 'recertify_path', 'recertify_path');
    	$classDetailSel->addField('enr', 'comp_status', 'comp_status');
    }
    
    $classDetailSel->addField('class','title','Title');
    $classDetailSel->addField('class','code','Code');
    $classDetailSel->addField('class','id','classid');
    $classDetailSel->addField('class','course_id','crs_id');
    $classDetailSel->addField('class','short_description','Description');
    if($callAPI)
	{
    $classDetailSel->addExpression('if(class.addn_catalog_show = 1,class.additional_info,null)','additional_info');
    }
	else
	        $classDetailSel->addField('class','additional_info','additional_info');
    $classDetailSel->addField('class','course_id','courseid');
    $classDetailSel->addField('class','custom4','export_compliance');
    $classDetailSel->addField('class','scheduled_duration','Duration');
    $classDetailSel->addField('class','lang_code','ClsLang'); 
    $classDetailSel->addField('class','delivery_type','delivery_type_code');
    $classDetailSel->addField('class','delivery_type','ClsDeliveryType');
    $classDetailSel->addField('class','is_compliance','is_compliance'); 
    $classDetailSel->addField('nla','node_id','node_id');
    $classDetailSel->addField('class','price','price');
    $classDetailSel->addField('class','currency_type','currency_type');
	$classDetailSel->addField('class','addn_catalog_show','addn_catalog_show');
	$classDetailSel->addField('class','addn_notification_show','addn_notification_show');
	$classDetailSel->addField('class','custom4','CurrencyPrefixSymbol'); 
    $classDetailSel->addField('class','custom4','CurrencyPostfixSymbol');
    $classDetailSel->addField('class','max_seats','MaxCapacity');
    $classDetailSel->addField('class','waitlist_count','AvailablibleSeatCount');
    $classDetailSel->addField('class','valid_to','EnrollmentValidity');
  	$classDetailSel->addField('class','registration_end_on', 'registration_end_on');
  	$classDetailSel->addField('class','dedicated_class_flag', 'dedicated_class_flag');
    $classDetailSel->addExpression('if(usrmap.mro is not null, usrmap.mro, group_map.mro )', 'mro_id');
	$classDetailSel->addExpression('slf_convert_price(class.price,class.currency_type,\''.$UserPrefCurrCode.'\')','price');
	$classDetailSel->addExpression('\''.$UserPrefCurrSym.'\'','currency_symbol');
    $classDetailSel->addExpression("if(if(usrmap.mro is not null, usrmap.mro, group_map.mro ) = 'cre_sys_inv_opt',3,if(if(usrmap.mro is not null, usrmap.mro, group_map.mro ) = 'cre_sys_inv_man',1,2))", "ascen");
	//filterByUserAccessPrivileges($classDetailSel, 'catacs',$vLMSUserId,'group_map');
	filterByUserAccessPrivileges($classDetailSel, 'class',$vLMSUserId,'','cre_sys_obt_cls');
    // below line commented for Mantis - SQL Injection #31297	
    //$classDetailSel->condition('class.id',$classId);
	if($callfrom == 'lrnplan_course_class_list')
    	$classDetailSel->where('class.id=:classid',array(':classid'=>$classId));
	else 
		$classDetailSel->where('class.id=:classid AND (class.dedicated_class_flag = \'N\' or class.dedicated_class_flag is null)',array(':classid'=>$classId));
    $classDetailSel->orderBy("ascen");
  }
  $classDetailSel->condition('class.status','lrn_cls_sts_atv','=');
  if($_REQUEST['apiname']=='GetClassDetailsAPI')
  {
    $classDetailSel->leftJoin('slt_tag_entity','te','((class.id=te.entity_id and te.entity_type=\'Class\') OR (class.course_id=te.entity_id and te.entity_type=\'Course\'))');
    $classDetailSel->leftJoin('slt_tagdefn','td','te.tagid= td.id');
    $classDetailSel->addExpression("group_concat(td.tagname)","tagname");
      $tag=trim($_REQUEST['tagname']);
      if(isset($tag) && !empty($tag)){
        $classDetailSel->condition('td.tagname', db_like($tag),'=');
      }
  }

  expDebug::dPrintDBAPI('getClassDetailsInfo-->', $classDetailSel);
  $results = $classDetailSel->execute()->fetchAll();
  if (count($results) > 0) {
    // Call the new function
    $prereqequv = getPrerequisiteclassDetails($classId);
    $prereqArr = Array();
    $equvArr = Array();
    foreach ($prereqequv as $row){
    	if($row->cmp_type == "5"){
    		$prereqArr[] = $row;
    	}else{
    		$equvArr[] = $row;
    	}
    }
    $results[0]->prerequisites = $prereqArr;
    $results[0]->equivalence = $equvArr;
    
  }
  
  return $results;
 
 
//  Return the result
  expDebug::dPrint('getClassDetailsInfo : = ' . print_r($results, true) , 3);
  expDebug::dPrint(' getClassDetailsInfo() SQL:  = '. print_r($results->__toString(), true) , 3);
  expDebug::dPrint('result_response after json encoding = '.print_r(drupal_json_encode($results), true) , 3);
  }catch (Exception $ex) {
    watchdog_exception('getClassDetailsInfo', $ex);
    expertusErrorThrow($ex);
  } 
}

/**
 * Implements to get the prerequisite course information
 * SELECT 
 * 	crs.title, 
 	* crs.code
 * FROM 
 * 	slt_course_class cls
 * 	RIGHT JOIN slt_common_mapping cmp ON cls.course_id = cmp.id1
 * 	LEFT JOIN slt_course_template crs ON cmp.id2 = crs.id
 * WHERE 
 * 	cls.id = 40
 *
 */
function getPrerequisiteclassDetails($classId) {
  try{
  // Select table
  $selectStmt = db_select('slt_course_class', 'cls');

  $selectStmt->rightJoin('slt_common_mapping','cmp','cls.course_id = cmp.id1');
  $selectStmt->leftJoin('slt_course_template','crs','cmp.id2 = crs.id');
  

  // Set conditions
  $selectStmt->condition('cls.id', $classId);
  $selectStmt->condition('cmp.object_type', 'cre_sys_obt_crs');
  
  // Select fields
  $selectStmt->distinct();
  $selectStmt->addField('crs', 'id', 'course_id'); // This is course id.
  $selectStmt->addField('cmp', 'type', 'cmp_type');
  $selectStmt->addField('crs', 'title', 'crs_title');
  $selectStmt->addField('crs', 'code', 'crs_code');   // Note that this is the class title and not course title.

  // Execute query
  $result = $selectStmt->execute()->fetchAll();

  return $result;
  
//  Return the result
  expDebug::dPrint(' getPrerequisiteclassDetails : = ' . print_r($result, true) , 3);
  expDebug::dPrint(' getPrerequisiteclassDetails() SQL:  = '. print_r($result->__toString(), true) , 3);
  expDebug::dPrint('result_response after json encoding = '.print_r(drupal_json_encode($result), true) , 3);
  }catch (Exception $ex) {
    watchdog_exception('getPrerequisiteclassDetails', $ex);
    expertusErrorThrow($ex);
  } 
}

function _getClassSessionCount($classId) {
  try{
  // Select table
  $selectStmt = db_select('slt_course_class_session', 'sess');
  // Set conditions
  $selectStmt->condition('sess.class_id', $classId);  
  // Select fields
  $selectStmt->distinct();
  $selectStmt->addField('sess', 'id', 'session_id'); // This is course id.
  // Execute query
  $result = $selectStmt->execute()->fetchAll();
  $countSession = ((count($result)>0) ? count($result) : 0);
  return $countSession; 
  }catch (Exception $ex) {
    watchdog_exception('_getClassSessionCount', $ex);
    expertusErrorThrow($ex);
  }  
}

function getClassDetails($classId, $sessionId, $callAPI=""){
  try{
  $vLMSUserId = getSltpersonUserId();
  $timezone_details = getPersonDetails($vLMSUserId);
  $records = getClassDetailsInfo($classId, $sessionId,$vLMSUserId,$callAPI);
  expDebug::dPrint('class Details data info--->'.print_r($records,1) , 4);
  $record = $records[0];
  $record->description = trim($record->description);
  $record->additional_info = trim($record->additional_info);
  if($record->currency_symbol == '' || $record->currency_symbol == null) {
    $record->currency_type = ''; // 0056924 - default $ removal
  }else{
    $record->currency_type = $record->currency_symbol;
  }
  if(is_null($record->export_compliance)){
    $record->export_compliance = 'N';
  }
  if(is_null($record->price)){
    $record->price = '0.00';
  }
  
  if ($record->delivery_type_code == 'lrn_cls_dty_ilt' || $record->delivery_type_code == 'lrn_cls_dty_vcl') {
    $sessionArr = array();
    $inc=0;
    
    $session_records = getSessionDetails($record->crs_id,$record->classid,$record->delivery_type_code);
    $record->session_count = count($session_records);
    foreach($session_records as $sessDet) {
    
    	
      $sessionArr[$inc]['title']                =   $sessDet['session_title'];
      $sessionArr[$inc]['session_id']           =   $sessDet['session_id'];
      $sessionArr[$inc]['facilityid']          =   $sessDet['facility_id'];
      $sessionArr[$inc]['start_date']           =   $sessDet['session_start_format'];
      $sessionArr[$inc]['start_time']           =   $sessDet['session_start_time_format'];
      $sessionArr[$inc]['start_time_format']    =   $sessDet['session_start_time_form'];
      $sessionArr[$inc]['end_time']             =   $sessDet['session_start_end_format'];
      $sessionArr[$inc]['end_time_format']      =   $sessDet['session_end_time_form'];
      $sessionArr[$inc]['sess_timezone']     =   $sessDet['sess_timezone'];
      $sessionArr[$inc]['session_end_date_format'] =   $sessDet['session_end_date_format'];
      $sessionArr[$inc]['session_name']         =   $sessDet['session_name'];
      $sessionArr[$inc]['session_address1']     =   $sessDet['session_address1'];
      $sessionArr[$inc]['session_address2']     =   $sessDet['session_address2'];
      $sessionArr[$inc]['session_country']      =   $sessDet['session_country'];
      $sessionArr[$inc]['session_state']        =   $sessDet['session_state'];
      $sessionArr[$inc]['session_city']         =   $sessDet['session_city'];
      $sessionArr[$inc]['session_zipcode']      =   $sessDet['session_zipcode'];
      $sessionArr[$inc]['session_code']         =   $sessDet['session_code'];
      $sessionArr[$inc]['tz_code']              =   $sessDet['tz_code'];
      //Added by Vincent on Oct 28, 2013 for #0028593
      $sessionArr[$inc]['ilt_start_date']       =   $sessDet['ilt_session_start_format'];
      $sessionArr[$inc]['ilt_start_time']       =   $sessDet['ilt_session_start_time_format'];
      $sessionArr[$inc]['ilt_start_time_format']=   $sessDet['ilt_session_start_time_form'];
      $sessionArr[$inc]['ilt_end_time']         =   $sessDet['ilt_session_start_end_format'];
      $sessionArr[$inc]['ilt_end_time_format']  =   $sessDet['ilt_session_end_time_form'];
      $sessionArr[$inc]['ilt_session_end_date_format'] =   $sessDet['ilt_session_end_date_format'];
      $inc++;
    } 
    expDebug::dPrint('$sessionArr : '.print_r($sessionArr, true) , 4);
    if (empty($record->startdatetime)||empty($record->enddatetime)) {

      $record->sessionDetails = '-';
    }
    else {
      $record->sessionDetails = dateTimeStrToUnivShort($record->startdatetime, 'Y','TRUE',false,false)." - ". dateTimeStrToUnivShort($record->enddatetime, 'Y','TRUE',false,false);				
    }
    if (empty($record->locationid)) {
      $record->locationaddr1 = ''; 
      $record->locationaddr2 = '';
      $record->citystate = '';
      $record->locationzip = '';
      $record->locationcountryname = '';
    }
    else {
      if (empty($record->locationname)) {        
        $record->locationname = '-';
      }
      $locCityState = array();
      if(!empty($record->locationcity)) {
        $locCityState[] = $record->locationcity;
      }
      if(!empty($record->locationstate)) {
        $locCityState[] = $record->locationstate;
      }
      $record->citystate = (count($locCityState)>1) ?  implode(", ", $locCityState) : $locCityState[0];      
    }
    if(!isset($record->startdatetime))
    	$record->startdatetime="";
    if(!isset($record->enddatetime))
    	$record->enddatetime="";
    
    $record->sessionDetails    = dateTimeStrToUnivShort($record->startdatetime,'Y','TRUE',false,false)." - ". dateTimeStrToUnivShort($record->enddatetime,'Y','TRUE',false,false);
    $record->available_seats   = getAvailableSeats($classId);
    $record->waitlist_seats    = getWaitlistCatalogInfo($classId);
    $record->sessionDetailInfo = $sessionArr;
   
  }
  else {
    //$location = '-';
    $record->sessionDetails   = '-';
    $record->sessionDetailInfo = '-';
    $record->available_seats  = '-';
    $record->waitlist_seats   = '-';
  }
  $record->multi_register = is_multiregistration_allowed($classId);
 /* if(!empty($vLMSUserId) && (!empty($record->crs_id)))
  	$exmpId = getRegisteredEnrollmentExmeptedorNot('class',$vLMSUserId,$record->crs_id,$record->classid);
  if(!empty($exmpId))
  	$record->exemptedid = 1; */
  
  expDebug::dPrint('class Details data --->'.print_r($record,1) , 4);
  return $record;
  }catch (Exception $ex) {
    watchdog_exception('getClassDetails', $ex);
    expertusErrorThrow($ex);
  }  
}

 function getClassDiscussionForum() {
 		try{
    return "Under Construction";
    }catch (Exception $ex) {
    	watchdog_exception('getClassDiscussionForum', $ex);
    	expertusErrorThrow($ex);
    }
 }
 
 function getClassBlogsDetail() {
 	try{
    return "Under Construction";
    }catch (Exception $ex) {
    	watchdog_exception('getClassBlogsDetail', $ex);
    	expertusErrorThrow($ex);
    }
 }
 
 function getRelatedClass() {
 	try{
    return "Under Construction";
    }catch (Exception $ex) {
    	watchdog_exception('getRelatedClass', $ex);
    	expertusErrorThrow($ex);
    }
 }
 
 function getClassRatingReview() {
 	try{
    return "Under Construction";
    }catch (Exception $ex) {
    	watchdog_exception('getClassRatingReview', $ex);
    	expertusErrorThrow($ex);
    }
 }
 
 function getClassCategoryRelated() {
 	try{
    return "Under Construction";
    }catch (Exception $ex) {
    	watchdog_exception('getClassCategoryRelated', $ex);
    	expertusErrorThrow($ex);
    }
 }
 
function getCourseInfo($courseId) {
  try{
  // Select table
  $selectStmt = db_select('slt_course_template', 'crs');
  // Set conditions
  $selectStmt->condition('crs.id',        $courseId);

  // Select fields
  $selectStmt->addField('crs',	'id',					'id');
  $selectStmt->addField('crs',	'title',				'title');
  $selectStmt->addField('crs',	'code',					'code');
  $selectStmt->addField('crs',	'complete_days',					'complete_days');
  $selectStmt->addField('crs',	'complete_date',					'complete_date');
  $selectStmt->addField('crs',	'compliance_completed',					'compliance_completed');
  $selectStmt->addField('crs',	'validity_days',					'validity_days');
  $selectStmt->addField('crs',	'validity_date',					'validity_date');

  // Execute query
  $result = $selectStmt->execute();
  expDebug::dPrint(' rowCount = ' . $result->rowCount() , 3);
  if ($result->rowCount() <= 0) {
     $courseDetails->code  = '-';
     $courseDetails->title = '-';
  }else{
    $courseDetails = $result->fetchObject();
  }
  return $courseDetails;
  }catch (Exception $ex) {
    watchdog_exception('getCourseInfo', $ex);
    expertusErrorThrow($ex);
  } 
}

/*
 *  getCountOfLesson(): Function to return the count of lessons attached to the class
 * 
 */
function getCountOfLesson($classid) {
	$txn = db_transaction();
	try{
		$selectStmt = db_select('slt_content_version', 'cont_vers');
		$selectStmt->addExpression('SUM(cont_vers.total_lesson)', 'lesson_cnt');
		$selectStmt->leftJoin('slt_content_master', 'cont_mast', 'cont_vers.content_master_id=cont_mast.id');
		$selectStmt->leftJoin('slt_course_content_mapper', 'cc_map', 'cont_mast.id=cc_map.content_id');
		$selectStmt->leftJoin('slt_profile_list_items', 'slpi', 'cont_mast.type=slpi.code and cont_mast.status=slpi.code');
		
		$selectStmt->condition('cont_mast.status', 'lrn_cnt_sts_del', '!=');
		$selectStmt->condition('cont_vers.content_sub_type',array('lrn_cnt_typ_vod'), 'NOT IN');
		// below line commented for Mantis - SQL Injection #31297
		// $selectStmt->condition('cc_map.class_id', $classid, '=');
		$selectStmt->where('cc_map.class_id=:classid', array(':classid'=>$classid));
		
		
		expDebug::dPrintDBAPI('count of lessons', $selectStmt);
		$result = $selectStmt->execute()->fetchField();
		expDebug::dPrint('result of the lesson count'.$result, 3);
		return $result;
	}catch (Exception $ex) {
		$txn->rollback();
		unset($txn);
		watchdog_exception('getCountOfLesson', $ex);
		expertusErrorThrow($ex);
  } 
	unset($txn);
}

function slt_mobile_exchange_contacts()
{

	try{
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	$action = "";
	expDebug::dPrint('API NAME :'.$_REQUEST["apiname"],4);
	if($_REQUEST["apiname"] == "RequestParticipantContactByAPI")
	  $action = "insert";
	else if($_REQUEST["apiname"] == "ListRequestedContactsByAPI")
		$action = "list";
	else if($_REQUEST["apiname"] == "ContactExchangeApprovalAPI")
	    $action = "update";
	else if ($_REQUEST["apiname"] == "GetApprovedContactsAPI")
		$action = "getapprovedcontacts";
	else if ($_REQUEST["apiname"] == "GetStroedContactsAPI")
		$action = "getstoredcontacts";
		
	expDebug::dPrint('$action :'.$action,4);
	if($action == "insert")
	{
		$userid = $_REQUEST["userid"];
		$participant_id = $_REQUEST["learnerid"];
		$contact_needed_of = $_REQUEST["contact_needed_of"];
		$session_attended_id = $_REQUEST["session_attended_id"];
		$status = "CONTACT_REQUESTED";
		$user_fullname =  $_REQUEST["user_fullname"];
		$qry = "select id,status from slt_mobile_exchange_contacts where participant_id = :pid and contact_needed_of = :cn ";
		$qry_res  = db_query($qry, array(':pid' => $participant_id,':cn' => $contact_needed_of));
		$result = $qry_res->fetchAll();
		$resultCount = count($result);
		$isExist = false;
	/*	for($i = 0; $i < $resultCount; $i++){
			if($result[$i]->status == "CONTACT_APPROVED")
			{
				$isExist = true;
			}
		}
		*/
		if(!$isExist)
		{
			drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
			$txn = db_transaction();
			try 
			{
				$drupalUid = getDrupalIdOfUser($participant_id);
				$account = user_load($drupalUid);
				if(!empty($account->picture->uri)){
					$filepath  = $account->picture->uri;
					$profileImage = file_create_url($filepath);
				}
				expDebug::dPrint('$filepath :'.$filepath,5);
				expDebug::dPrint('$profileImage :'.$profileImage,5);
				$insLog  = db_insert('slt_mobile_exchange_contacts');
				$insLog->fields(array(
						'session_attended_id' => $session_attended_id,
						'contact_needed_of' => $contact_needed_of,
						'participant_fullname' => $user_fullname,
						'participant_id' => $participant_id,
						'photo_path' => $profileImage,
						'status' => $status,
						'created_by' => $participant_id,
						'created_on' => now(),
						'updated_by' => $participant_id,
						'updated_on' => now()
				));
				$logId = $insLog->execute();
				return array((object)array('id'=>$logId));
			}
			catch (Exception $ex) {
				expDebug::dPrint("Insert mobile_exchange contacts error:".$ex,4);
				$txn->rollback();
				unset($txn);
				watchdog_exception('orgadmin', $ex);
				//return array((object)array('id'=>"failure"));
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "error in inserting mobile exchange contacts";
				return $errobj;
			}
			unset($txn);
		}
			
	}
	else if($action == "list")
	{
		$user_id = $_REQUEST["userid"];
		$learner_id = $_REQUEST["learnerid"];
		$session_id = $_REQUEST['sessionid'];  //CE Enhancement
		expDebug::dPrint("learner_id :".print_r($learner_id,true),4);
		expDebug::dPrint("session_id :".print_r($session_id,true),4);
		
		$qry = " select mobexc.id,mobexc.participant_id contact_requester_id,mobexc.participant_fullname contact_requester_fullname,mobexc.status,profile.name job_title,mobexc.photo_path
		from slt_mobile_exchange_contacts mobexc 
		LEFT JOIN slt_person per ON per.id=mobexc.participant_id
		LEFT JOIN slt_profile_list_items profile ON profile.code=per.job_title 
		where mobexc.contact_needed_of = :cn and mobexc.session_attended_id = :sid and mobexc.status = 'CONTACT_REQUESTED'";
		$qry_res  = db_query($qry, array(':cn' => $learner_id,':sid'=> $session_id ));  //CE Enhancement
		expDebug::dPrintDBAPI('hello', $qry_res);
		$result = $qry_res->fetchAll();
		expDebug::dPrint("result :".print_r($result,true),4);
		return $result;
	}
	else if($action == "update")
	{
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$contact_request_id = $_REQUEST["contact_request_id"];
		$contact_needed_of = $_REQUEST["contact_needed_of"];
		$userid = $_REQUEST["userid"];
		$participant_id = $_REQUEST["learnerid"];
		$status = $_REQUEST["status"];
		$contact_needed_firstname = "";
		$contact_needed_lastname = "";
		$contact_needed_email = "";
		$contact_needed_phoneno = "";
		
		if($status == "CONTACT_APPROVED")
		{
			$qry = "select first_name,last_name,phone_no,email from slt_person where id = :uid";
			$qry_res  = db_query($qry, array(':uid' => $participant_id));
			$result = $qry_res->fetchAll();
			$contact_needed_firstname = $result[0]->first_name;
			$contact_needed_lastname = $result[0]->last_name;
			$contact_needed_email = $result[0]->email;
			$contact_needed_phoneno = $result[0]->phone_no;
		
		$txn = db_transaction();
		try 
		{
			$updLog  = db_update('slt_mobile_exchange_contacts');
			$updLog->fields(array(
							'status' => $status,
							'contact_needed_firstname' => $contact_needed_firstname,
							'contact_needed_lastname' => $contact_needed_lastname,
							'contact_needed_email' => $contact_needed_email,
							'contact_needed_phoneno' => $contact_needed_phoneno,
							'updated_by' => $participant_id,
							'updated_on' => now()
			));
			$updLog->condition('id', $contact_request_id);
			$updLog->condition('contact_needed_of', $participant_id);				
			
			expDebug::dPrintDBAPI('update slt_mobile_exchange_contacts-->', $updLog);
				
			$logId = $updLog->execute();
			return array((object)array('id'=>$logId));
		}
		catch (Exception $ex) {
			$txn->rollback();
			unset($txn);
			watchdog_exception('orgadmin', $ex);
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Error in updating a contact";
			return $errobj;
		}
		unset($txn);
		} 	
		else if($status == "CONTACT_REMOVED")
		{	
		   $txn = db_transaction();
			try
			{
	        	$updLog  = db_update('slt_mobile_exchange_contacts');
		        $updLog->fields(array(
							'status' => $status,
							'updated_by' => $participant_id,
							'updated_on' => now()
		                   ));
	           	$updLog->condition('contact_needed_of', $contact_request_id);
	           	$updLog->condition('participant_id', $participant_id);
		       $logId = $updLog->execute();
		       return array((object)array('id'=>$logId));
	        }
			catch (Exception $ex) {
				$txn->rollback();
				unset($txn);
				watchdog_exception('orgadmin', $ex);
				//return array((object)array('id'=>"failure"));
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Error in contact removal";
				return $errobj;
			}
			unset($txn);
		}	
		else if($status == "CONTACT_REJECTED")
		{	
			try
			{
	        	$updLog  = db_update('slt_mobile_exchange_contacts');
		        $updLog->fields(array(
							'status' => $status,
							'updated_by' => $participant_id,
							'updated_on' => now()
		                   ));
	           	$updLog->condition('contact_needed_of', $participant_id);
	            $updLog->condition('participant_id', $contact_request_id);
		       $logId = $updLog->execute();
		       return array((object)array('id'=>$logId));
	        }
			catch (Exception $ex) {
				watchdog_exception('orgadmin', $ex);
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Error in rejecting a contact";
				return $errobj;
			}
		}	
		else if($status == "CONTACT_STORED")
		{
			$txn = db_transaction();
			try
			{
				$updLog  = db_update('slt_mobile_exchange_contacts');
				$updLog->fields(array(
						'status' => $status,
						'updated_by' => $participant_id,
						'updated_on' => now()
				));
				$updLog->condition('id', $contact_request_id);
				$updLog->condition('participant_id', $participant_id);
				$logId = $updLog->execute();
				return array((object)array('id'=>$logId));
			}
			catch (Exception $ex) {
				$txn->rollback();
				unset($txn);
				watchdog_exception('orgadmin', $ex);
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Error in updating a contact";
				return $errobj;
			}
			unset($txn);
				
		}
	}
	else if($action == "getapprovedcontacts")
	{
		$learner_id = $_REQUEST["userid"];
		$session_id = $_REQUEST["sessionid"];
		
		$qry = "select  mobexc.id,mobexc.participant_id contact_requester_id,mobexc.participant_fullname contact_requester_fullname,mobexc.status,mobexc.photo_path, profile.name 'job_title'
				from slt_mobile_exchange_contacts mobexc
				LEFT JOIN slt_person per ON per.id=mobexc.participant_id
				LEFT JOIN slt_profile_list_items profile ON profile.code=per.job_title
				where mobexc.session_attended_id = ".$session_id." and mobexc.status = 'CONTACT_REQUESTED' and mobexc.contact_needed_of = ".$learner_id;
		$qry_res  = db_query($qry);
		$receivedrequests = $qry_res->fetchAll();
		
  		expDebug::dPrint(" receivedrequests = ".print_r($receivedrequests,true) , 4);
		
		
		$qry = "select  mobexc.id,mobexc.contact_needed_of
				from slt_mobile_exchange_contacts mobexc
				LEFT JOIN slt_person per ON per.id=mobexc.participant_id
				LEFT JOIN slt_profile_list_items profile ON profile.code=per.job_title
				where mobexc.session_attended_id = ".$session_id." and mobexc.status = 'CONTACT_APPROVED' and mobexc.participant_id = ".$learner_id;
		
		expDebug::dPrint("acceptedrequests qry ".$qry);
		$qry_res  = db_query($qry);
		$acceptedrequests = $qry_res->fetchAll();
		expDebug::dPrint(" acceptedrequests = ".print_r($acceptedrequests,true) , 4);
		
		
		$result[0]->acceptedrequests = $acceptedrequests;
		$result[0]->receivedrequests = $receivedrequests;
		expDebug::dPrint(" resultresult = ".print_r($result,true) , 4);
		
		return array((object)array('acceptedrequests'=>$acceptedrequests ,'receivedrequests'=>$receivedrequests));
// 		return $result;
		
	}
	else if($action == "getstoredcontacts")
	{

		$learner_id = $_REQUEST["userid"];
		$qry = " select id,contact_needed_of,contact_needed_firstname, contact_needed_lastname,contact_needed_email, contact_needed_phoneno from slt_mobile_exchange_contacts where participant_id = :pid and status = 'CONTACT_STORED'";
		$qry_res  = db_query($qry, array(':pid' => $learner_id));
		$result = $qry_res->fetchAll();
		return $result;
	}
	}catch (Exception $ex) {
		watchdog_exception('slt_mobile_exchange_contacts', $ex);
		expertusErrorThrow($ex);
	}
	
	
}

function getClassDetailsRestAPI($classId, $sessionId){
	try{
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		if($classId > 0 && is_numeric($classId)) {
			$select = db_select("slt_course_class", "scs");
			$select->addField("scs", "id", "id");
			$select->condition("scs.id", $classId, "=");
			$select->condition("scs.status", 'lrn_cls_sts_atv', "=");
			$result_set = $select->execute();
			expdebug::dPrintDBAPI('$select joo:', $select);
			if($result_set->rowCount() > 0) {
				$results = getClassDetails($classId, $sessionId, 1);
			} else {
				$errobj = new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errormsg = 'Inactive/Invalid Class ID';
				return $errobj;
			}
		} else {
			expDebug::dPrint('inside final else');
			$errobj = new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errormsg = 'Invalid Class ID';
			return $errobj;
		}
		$result_new[0] = $results;
		$total_records = count($result_new);
		$result_new['totalrow'] = $total_records;
		return $result_new;
	}catch (Exception $ex) {
		watchdog_exception('getClassDetailsRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}
?>