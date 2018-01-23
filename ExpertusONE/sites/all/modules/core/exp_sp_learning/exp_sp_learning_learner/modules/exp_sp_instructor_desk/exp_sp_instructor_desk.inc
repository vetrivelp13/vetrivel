<?php


/**
 * To get the list of classes which the instructor handles
 */
function getInstructorClasses(){
  try{
  	
  	if(isset($_REQUEST['mobile_request']) && isset($_REQUEST['action'])){
  		if($_REQUEST['action'] == 'surveyreport' || $_REQUEST['action'] == 'assessmentreport' || $_REQUEST['action'] == 'LearningActivity'){
  			$result = getInstructorActivityResultSet();
  			return $result;
  		} 	
  	}
  	
  	
  	
  global $regstatus;
  if(isset($_REQUEST['regstatuschk'])){
    $regstatus  = stripRegStatus($_REQUEST['regstatuschk']);
  } else {
    $regstatus = 'scheduled';
  }
  $result_response= new stdClass();
  $solrSource = getConfigValue('solr_search');
  if($solrSource == 0) {
  $getlpQryRes = getInstructorEnrollmentResultSet();
  $lpQryRes = $getlpQryRes['result'];
 
  $result_response->page    = $getlpQryRes['page'];
  $result_response->total   = $getlpQryRes['total_pages'];
  $result_response->records = $getlpQryRes['count'];
  }
  else {
  	if(isset($_REQUEST['apiname']) && $_REQUEST['apiname'] == 'ListInstructorClassesAPI') {
  		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  	}
  	require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_mylearning_myclasses.php';
  	try{
  		$rtnType = $_REQUEST['returntype'];
  		$_REQUEST['returntype'] = 'json';
  		$solrObj = new MyLearningMyClassesSolrSearch();
  		$getlpQryRes = $solrObj->myClassesSolrSearch();
  		$lpQryRes = $getlpQryRes['result'];
  		expDebug::dPrint("My program solr search results ".print_r($lpQryRes,true));
  		$result_response->records = $getlpQryRes['count'];	
  		$_REQUEST['returntype'] = $rtnType;
  		unset($_REQUEST['SolrImpl']);
  	}catch(Exception $e){
  		expDebug::dPrint("Error in my program Solr Search -- ".print_r($e->getMessage(),true),1);
  	}
  }
  $classArr = array();
  $storeArr = true;
  $classSession = array();
  $sessionDetails = array();
  $i=0;
  $currentDateTime = now();
 
  foreach ($lpQryRes as $record) {
      $sessionDetails = ($record->basetype=='ILT' || $record->basetype=='VC')?getSessionDetails($record->courseid, $record->cls_id, $record->delivery_type_code, "enroll",'','instructorDesk'):array();
      expDebug::dPrint('Session complete Details : '.print_r($sessionDetails, true) , 4);
      //Append the Insturctor name in session details
      $j=0;
      foreach ($sessionDetails as $instructorName) {
          $sessionDetails[$j]['instructorName'] = '';
          if($instructorName['session_instructor_id'] !='' || $instructorName['session_instructor_id'] !=null){
          	  $instructor_ids = explode(',',$instructorName['session_instructor_id']);
          	  $ins_name = '';
          	  foreach($instructor_ids as $ins_id){
              	$instructorName = getPersonDetails($ins_id,array('full_name'));
              	if($ins_name=='')
              	 $ins_name = $instructorName['full_name'];
              	else
              		$ins_name .= ', '.$instructorName['full_name'];
          	  }
          	  $sessionDetails[$j]['instructorName']=$ins_name;
          }
          $j++;
      }
      expDebug::dPrint('Session complete after get the instructor Details : '.print_r($sessionDetails, true) , 4);
      list($mro_name, $mro_code) = explode('##',$record->mro);



 	  if(isset($_REQUEST['apiname']) && $_REQUEST['apiname'] == 'ListInstructorClassesAPI'){


 	  $result_response->rows[$i] = formInstructorResult($record, $mro_name, $mro_code, $sessionDetails);
 	  if(!isset($_REQUEST['action'])){	 
 	  $query  = db_select('slt_survey', 'sur');
      $query->join('slt_survey_mapping', 'map','sur.id=map.survey_id');
      $query->addField('sur', 'id','surveyId');
      $query->condition('map.object_type', 'cre_sys_obt_cls', '=');
      $query->condition('sur.status', 'sry_det_sry_atv', '=');
      $query->condition('sur.type', 'sry_det_typ_ass', '!=');
      $query->condition('map.object_id', $record->cls_id, '=');
      $surqryRes = $query->execute()->fetchAll();

      $query  = db_select('slt_survey', 'sur');
      $query->join('slt_survey_mapping', 'map','sur.id=map.survey_id');
      $query->addField('sur', 'id','surveyId');
      $query->condition('map.object_type', 'cre_sys_obt_cls', '=');
      $query->condition('sur.status', 'sry_det_sry_atv', '=');
      $query->condition('sur.type', 'sry_det_typ_ass', '=');
      $query->condition('map.object_id', $record->cls_id, '=');
      $AssqryRes = $query->execute()->fetchAll();

      $result_response->rows[$i]['id']   = $record->id;
      $result_response->rows[$i]['survey_status'] = $surqryRes;
      $result_response->rows[$i]['assessment_status'] = $AssqryRes;
      $result_response->rows[$i]['survey_completed_count'] = getSurveyCompletedUsersCount($record->cls_id,$surqryRes[0]->surveyid);
      $result_response->rows[$i]['assessment_completed_count'] = getSurveyCompletedUsersCount($record->cls_id,$AssqryRes[0]->surveyid);
 	  }

      expDebug::dPrint('ResultResultResultResultResult'.print_r($result_response->rows[$i], true) , 4);
 	  }
 	  else{
 	    $result_response->rows[$i]['cell'] = formInstructorResult($record, $mro_name, $mro_code, $sessionDetails);
 	  }

      $i++;
  }

   if(isset($_REQUEST['apiname']) && $_REQUEST['apiname'] == 'ListInstructorClassesAPI'){
   	//$result_response->rows['totalrow'] = $i-1; // Added by Vincent to show rec count in API
   	$result_response->rows['totalrow'] = $getlpQryRes['count'];
    return $result_response->rows;
  }

  // Identify last rec in the list sent back to jqgrid
  if ($i > 0) {
    $result_response->rows[$i-1]['cell']['is_last_rec'] = 'last';
  }

  expDebug::dPrint('Instructor Response: '.print_r($result_response, true) , 4);
  echo drupal_json_encode($result_response);

  }catch (Exception $ex) {
    watchdog_exception('getInstructorClasses', $ex);
    expertusErrorThrow($ex);
  }
}

function getLaunchtype($sessionId){
	$userId=getSltpersonUserId();
	$select=db_select('slt_session_instructor_details','ins');
	$select->addfield('ins','launch_flag','launch_flag');
	$select->condition('session_id',$sessionId);
	$select->condition('instructor_id',$userId);
	expDebug::dPrintDBAPI('launch detail qry  -->', $select);
	return $select->execute()->fetchAll();
}

function getClassCurrentSessionPresenterDetail($sessionId){
	$userId = getIdOfLoggedInUser();
	$presenterQry = "select is_presenter from slt_session_instructor_details	where session_id=:sessionId	AND instructor_id=:usrid";	
	$presenterQry  = db_query($presenterQry, array(':sessionId' => $sessionId,'usrid' => $userId));
	expDebug::dPrintDBAPI('Presenter detail qry  -->', $presenterQry,1);
	$vlcLaunchDetRes  = array($presenterQry->fetchAssoc());
	expDebug::dPrint("Get VC Launch details classid : ".$classId,2);
	return $vlcLaunchDetRes;
}
function formInstructorResult($record, $mro_name, $mro_code, $sessionDetails){
  try{
  	global $theme_key;
  	$userId = getSltpersonUserId();
   	$timezone_details = getPersonDetails($userId);
  	$markcomplete = getInstructorMarkComplete();
  	$regStatusesList = array('lrn_crs_reg_cnf');
  	$compStatusesList = array('lrn_crs_cmp_enr','lrn_crs_cmp_inp','lrn_crs_cmp_cmp', 'lrn_crs_cmp_nsw','lrn_crs_cmp_att');
  	$enrolledSeats = getInstructorEnrollCount($record->cls_id, $regStatusesList,$compStatusesList);
  	$launchDt = ($record->basetype=='VC')?getVirutalClassLauncDetail($record->cls_id):'';  
  	expDebug::dPrint('Virtual Class Launch Details--->'.print_r($launchDt,1),4);
  	
  	$presenter_details = getClassCurrentSessionPresenterDetail($launchDt['sessionId']); 
  	$launch_details = getLaunchtype($launchDt['sessionId']);    
	
  	expDebug::dPrint('VC PRESENTER Details--->'.print_r($presenter_details,1),4);
  	expDebug::dPrint('VC Launch Details--->'.var_export($launch_details,1),4);
  	if($launchDt['SessionType'] == 'lrn_cls_vct_exp' || $launchDt['SessionType'] == 'lrn_cls_vct_web' || $launchDt['SessionType'] == 'lrn_cls_vct_oth') {
  		if($presenter_details[0]['is_presenter'] == 'Y' && $launch_details[0]->launch_flag == 0){
  			$launchLabel = t('LBL251'); // Start
  		}else if($presenter_details[0]['is_presenter'] == 'Y' && $launch_details[0]->launch_flag == 1){
  			$launchLabel = t('LBL1318'); //Restart
  		}else if($presenter_details[0]['is_presenter'] == 'N' && $launch_details[0]->launch_flag == 0){ 
  			$launchLabel = t('LBL880'); // Join
  	}else if($presenter_details[0]['is_presenter'] == 'N' && $launch_details[0]->launch_flag == 1){
  		$launchLabel = t('LBL1319'); //Rejoin
  	}
  	}else{
  		$launchLabel = t('LBL199'); // Launch
  	}
  $result =  array(
    'id'						 => $record->cls_id,
    'course_title'               => $record->title,
    'cls_title'                  => sanitize_data($record->cls_title),
    'cls_code'                   => sanitize_data($record->code),
    'description'     		     => titleController('ANNOUNCEMENT_SHORTTITLE', strip_tags(str_replace(array("\n","\r"), array("<br/>","&nbsp;"), trim($record->description))),($theme_key == "expertusoneV2" ? 120 : '')),
    'descriptionfull'			 => /* strip_tags( */str_replace(array("\n","\r","<p>&nbsp;</p>"), array("","&nbsp;",""), trim($record->description))/* ) */,
    'delivery_type'              => $record->delivery_type,
    'delivery_type_code'         => $record->delivery_type_code,
    'basetype'                   => $record->basetype,
    'courseid'                   => $record->courseid,
    'classprice'                 => $record->classprice,
    'session_id'                 => $record->session_id,
    'session_start'              => $record->session_start,
    'session_start_day'		     => ($record->session_start != null ) ? date_format(date_create($record->session_start),'D') : '',
    'session_end'                => '',
    'session_start_format'       => date_format(date_create($record->session_start),'M d, Y'),
    'sessionDetails'			 => $sessionDetails,
    'session_max_capacity'		 => $record->session_max_capacity,
    'enrolled_count'		     => $enrolledSeats,
    'location_id'                => $record->locationid,
    'location_name'              => $record->locationname,
    'location_addr1'             => $record->locationaddr1,
    'location_addr2'             => $record->locationaddr2,
    'location_city'              => $record->locationcity,
    'location_state'             => $record->locationstate,
    'location_zip'               => $record->locationzip,
    'location_phone'             => $record->locationphone,
    'location_country'           => $record->locationcountry,
    'language'             	     => $record->language,
    'node_id'             	     => $record->nodeid,
    'mandatory'			         => $record->mandatory,
    'full_name'                  => ($record->created_by)?(($full_name=getPersonDetails($record->created_by, array('full_name')))?$full_name['full_name']:'') :'',
    'managerid'                  => $record->created_by,
    'updated_by'                 => $record->updated_by,
    'updated_by_name'            => ($record->updated_by)?(($updated_by=getPersonDetails($record->updated_by, array('full_name')))?$updated_by['full_name']:'') :'',
    'launch'					 => $launchDt,
    'show_lnr_attach'			 => '',
    'show_events'			     => '', //getAlternativeEventStatus($record->class_id,$record->course_id)
    'mro'						 => $mro_name,
    'mro_id'					 => $mro_code,
    'assigned_by'				 => $record->assigned_by,
    'MarkComplete'               => $markcomplete,
    'classStatus'				 => $record->classstatus,
    'labelmsg'                   => array(
							    		'enrolled_on'      => t('LBL025'),
							    		'seats'            => t('LBL106'),
							    		'name'             => t('LBL107'),
							    		'status'           => t('LBL102'),
							    		'action'           => t('LBL108'),
							    		'cancel'           => t('LBL109'),
							    		'mark_completion'  => t('LBL110'),
							    		'roster'           => t('LBL275'),
							    		'err_no_learner'   => t('ERR074'),
							    		'total_seats'  	   => t('LBL128'),
							    		'enrolled_seats'   => t('LBL129'),
							    		'pending'          => t('Pending'),
							    		'launch'		   => $launchLabel
  									),
      'learning_type_image'		 => getTypeImageClass($record->delivery_type_code),
      'learning_type'		 	 => $record->delivery_type,
                          );
  return $result;
  }catch (Exception $ex) {
    watchdog_exception('formInstructorResult', $ex);
    expertusErrorThrow($ex);
  }
}

function getInstructorEnrollCount($classId, $regStatusList,$compStatusList) {
  try{
  $selectStmt = db_select('slt_enrollment', 'enr');
  $selectStmt->leftjoin('slt_enrollment', 'e1', 'e1.id = enr.id');
  $selectStmt->condition('enr.class_id', $classId);
  if(count($regStatusList) > 0){
    $selectStmt->condition('enr.reg_status', $regStatusList, 'IN');
  }
  if(count($compStatusList) > 0){
    $selectStmt->condition('enr.comp_status', $compStatusList, 'IN');
  }
  $selectStmt->where('enr.id = (select max(id) from slt_enrollment where class_id = '.$classId.'  and user_id = enr.user_id)');
  $selectStmt->addField('enr', 'id');
  $selectStmt->groupBy('enr.user_id');
  $selectStmt->groupBy('enr.reg_status');
  $selectStmt->groupBy('enr.comp_status');
  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
  $statusCount = $result->rowCount();
  expDebug::dPrint(' $result StatusCount = ' .$statusCount , 4);
  return $statusCount;
  }catch (Exception $ex) {
    watchdog_exception('getInstructorEnrollCount', $ex);
    expertusErrorThrow($ex);
  }
}

function getSurveyCompletedUsersCount($classId,$surveyId) {
  try{

  $query  = db_select('slt_survey_results', 'surRes');
  $query->addField('surRes',id, 'surveyCount');
  $query->leftjoin('slt_enrollment', 'enroll', 'surRes.entity_id = enroll.class_id and  surRes.user_id = enroll.user_id');
  $query->condition('surRes.entity_type', 'cre_sys_obt_cls', '=');
  $query->condition('surRes.entity_id', $classId, '=');
  $query->condition('surRes.survey_id', $surveyId, '=');
  $query->condition('enroll.reg_status', 'lrn_crs_reg_cnf', '=');
  $query->groupBy("surRes.user_id");

  expDebug::dPrintDBAPI('  $qryRes $qryRes $qryRes' ,  $query);
  // Execute query
  $result = $query->execute();
  $statusCount = $result->rowCount();
  expDebug::dPrint(' $result StatusCount = ' .$statusCount , 4);
  return $statusCount;
  }catch (Exception $ex) {
    watchdog_exception('getInstructorEnrollCount', $ex);
    expertusErrorThrow($ex);
  }
}

function getInstructorEnrollmentResultSet(){
  try{
  		if(isset($_REQUEST['mobile_version']) || isset($_REQUEST['mobile_request']) || isset($_REQUEST['apiname'])) {
			$enrollCountQuery = getInstructorEnrollmentQuery(true);
			
			expDebug::dPrintDBAPI('instructor classes count query -> ', $enrollCountQuery);
			$count = $enrollCountQuery->execute()->fetchField();
			expDebug::dPrint("Instructor classes total count " . $count, 4);
		}
		/* Paging Related */
// 		$count = 0;
		
		$page = $_REQUEST['page'];
		$limit = $_REQUEST['rows'];
		if(isset($_REQUEST['mobile_version']) || isset($_REQUEST['mobile_request']) || isset($_REQUEST['apiname'])) {
			if($count > 0) {
				$total_pages = ceil($count / $limit);
			} else {
				$total_pages = 0;
			}
			if($page > $total_pages) {
				$page = $total_pages;
			}
			$start = $limit * $page - $limit;
			if($start < 0) {
				$start = 0;
			}
		} else {
			$start = calculate_offset($page, $limit);
			$count = $limit;
		}
		$enrollQuery = getInstructorEnrollmentQuery();
		$enrollQuery->range($start, $limit);
		expDebug::dPrintDBAPI("instructor classes select results query -> ", $enrollQuery);
		
		
		// Execute query
		$lpQryRes = $enrollQuery->execute();
		$pagination['result'] = $lpQryRes;
		$pagination['page'] = $page;
		$pagination['count'] = $count;
		$pagination['total_pages'] = $total_pages;
		return $pagination;
	} catch(Exception $ex) {
		watchdog_exception('getInstructorEnrollmentResultSet', $ex);
		expertusErrorThrow($ex);
	}
}

function getInstructorEnrollmentQuery($count = false) {
	try {
		$user_id = stripQuotes($_REQUEST['UserID']);
		
		if(isset($_REQUEST['apiname']) && $_REQUEST['apiname'] == 'ListInstructorClassesAPI') {
			$regstatus = stripRegStatus(str_replace(',', '_', $_REQUEST['regstatuschk']));
			drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		} else {
			global $regstatus;
		}
		
		$userTimezone = getUserTimezone();
		$instructorSearchText = $_REQUEST['searchText'];
		$deliveryType = $_REQUEST['del_type'];
		$location = $_REQUEST['location'];
		$scheduledDays = $_REQUEST['scheduled'];
		$timeZone = date_default_timezone(false);
		$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
		$enrollQuery = db_select('slt_course_class', 'cat');
		
		//following fields and expressions are common for both count query and result query
		$enrollQuery->addField('cat', 'id', 'cls_id');
		$enrollQuery->addField('ses', 'timezone', 'ses_timezone');
		$enrollQuery->addExpression("CONCAT( DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.end_time,':00')", 'session_end_datetime');
		if(!$count) {
			// $enrollQuery->distinct('cat.id');
			$enrollQuery->addField('crs', 'title', 'title');
			$enrollQuery->addField('cat', 'code', 'crs_code');
			$enrollQuery->addField('cat', 'code', 'CODE');
			$enrollQuery->addField('cat', 'title', 'cls_title');
			$enrollQuery->addField('cat', 'short_description', 'description');
			$enrollQuery->addField('cat', 'delivery_type', 'delivery_type_code');
			$enrollQuery->addField('cat', 'course_id', 'courseid');
			$enrollQuery->addField('cat', 'price', 'classprice');
			$enrollQuery->addField('cat', 'lang_code', 'LangCode');
			$enrollQuery->addField('loc', 'id', 'LocationId');
			$enrollQuery->addField('loc', 'name', 'LocationName');
			$enrollQuery->addField('loc', 'addr1', 'LocationAddr1');
			$enrollQuery->addField('loc', 'addr2', 'LocationAddr2');
			$enrollQuery->addField('loc', 'city', 'LocationCity');
			$enrollQuery->addField('loc', 'zipcode', 'LocationZip');
			$enrollQuery->addField('loc', 'phone', 'LocationPhone');
			$enrollQuery->addField('loc', 'timezone', 'loc_timezone');
			// $enrollQuery->addField('p','user_name', 'user_name');
			// $enrollQuery->addField('p','time_zone', 'user_timezone');
			$enrollQuery->addField('ses', 'id', 'session_id');
			$enrollQuery->addField('ses', 'start_date', 'session_start_date');
			$enrollQuery->addField('ses', 'start_time', 'session_start_time');
			$enrollQuery->addField('ses', 'end_time', 'session_end_time');
			$enrollQuery->addField('ses', 'title', 'session_title');
			$enrollQuery->addField('ses', 'capacity_max', 'session_max_capacity');
			$enrollQuery->addField('ses', 'type', 'sess_type');
			
			// $enrollQuery->addField('ses_ins','is_presenter', 'is_presenter');
			$enrollQuery->addExpression("DATE_FORMAT(`ses`.`start_date`,_utf8'%d-%b-%Y')", 'session_start');
			
			$enrollQuery->addExpression("(SELECT name from slt_profile_list_items where code = cat.delivery_type)", 'delivery_type');
			$enrollQuery->addExpression("(SELECT attr1 from slt_profile_list_items where code =cat.delivery_type)", 'BaseType');
			$enrollQuery->addExpression("(SELECT  IF(COUNT(state_name)>0,state_name,loc.state) stname FROM slt_state WHERE state_code= loc.state limit 1)", 'LocationState');
			$enrollQuery->addExpression("(SELECT `country_name` FROM `slt_country` WHERE `country_code`=`loc`.`country` LIMIT 1)", 'LocationCountry');
			$enrollQuery->addExpression("(SELECT name from slt_profile_list_items where code= cat.lang_code)", 'Language');
			
			// $enrollQuery->addExpression("(SELECT is_presenter from slt_session_instructor_details where session_id= ses.id and instructor_id =".$user_id.")", 'is_presenter');
			// Code commented bcz job_role changed as well as slt_mro_mapping not in use
			// $enrollQuery->addExpression("(SELECT CONCAT(splt_mro.name,'##',splt_mro.code) FROM slt_mro_mapping mro LEFT OUTER JOIN slt_profile_list_items splt_mro ON mro.mro_flag = splt_mro.code WHERE mro.entity_id = cat.id AND mro.entity_type = 'cre_sys_obt_cls' AND mro.job_role_id = (SELECT CODE FROM slt_profile_list_items WHERE CODE = (SELECT job_role FROM slt_person WHERE id IN (ses.instructor_id)) AND is_active='Y') AND mro.mro_flag IN (SELECT CODE FROM slt_profile_list_items WHERE code like '%cre_sys_inv_%' and is_active = 'Y'))", 'mro');
		}
		
		$enrollQuery->leftjoin('slt_course_template', 'crs', 'crs.id = cat.course_id');
		$enrollQuery->leftjoin('slt_course_class_session', 'ses', "ses.class_id = cat.id ");
		$enrollQuery->leftjoin('slt_session_instructor_details', 'ses_ins', "ses_ins.class_id = cat.id ");
		$enrollQuery->leftjoin('slt_location', 'loc', 'ses.location_id = loc.id');
		$enrollQuery->leftjoin('slt_profile_list_items', 'spl', 'ses.timezone = spl.code');
		$enrollQuery->leftjoin('slt_state', 'state', 'loc.state = state.state_code and loc.country=state.country_code');

		/* Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404 */		
		if(isset($_REQUEST['apiname']) && $_REQUEST['apiname'] == 'ListInstructorClassesAPI'){
		    $enrollQuery->leftJoin('slt_country','loc_ctry','loc_ctry.country_code=loc.country');
		 }
  
		// $enrollQuery->leftjoin('slt_person', 'p', 'p.id');
		$enrollQuery->where("ses.id = if((select count(1) from slt_session_instructor_details as ses_ins1 where ses_ins1.class_id=cat.id and ses_ins1.instructor_id =" . $user_id . ")>0,
        (select id from slt_course_class_session where class_id=cat.id order by start_date desc, end_time desc limit 1), 0)");
 /*  $enrollQuery->condition(db_or()->condition('cat.status', 'lrn_cls_sts_can', '!=')
                                 ->condition('cat.status', 'lrn_cls_sts_del','!=')); */
		// Issue is fixed for this Ticket #0035162 Dont display cancel and Deleted classes
		$enrollQuery->condition('cat.status', array('lrn_cls_sts_can', 'lrn_cls_sts_del'), 'NOT IN');
		// Filter - Search text bar
		if(! empty($instructorSearchText)) {
		$enrollQuery->condition(db_and()->
				condition(db_or()->
						condition('cat.title', '%' . db_like($instructorSearchText) . '%', 'LIKE')->
						condition('cat.code', '%' . db_like($instructorSearchText) . '%', 'LIKE')->
						condition('cat.short_description', '%' . db_like($instructorSearchText) . '%', 'LIKE')
				));
		}
		// Filter - Delivery type
		if(! empty($deliveryType)) {
			$deliveryType = explode("|", $deliveryType);
			$enrollQuery->condition('cat.delivery_type', $deliveryType, 'IN');
		}
		// Filter - Location
		if(trim($_REQUEST['apiname'])!= 'ListInstructorClassesAPI'){	 //Added by ganeshbabuv on March 6th 2016 5:15PM for #0072992: "List of an instructor class" Rest API - Search using location filter is not working
			if(! empty($location)) {
				// $enrollQuery->condition('loc.name', '%' . db_like($location) . '%', 'LIKE');
				if($_REQUEST['selectedLocID']) {
					$enrollQuery->condition('loc.id', $_REQUEST['selectedLocID'], '=');
				} else {
					$enrollQuery->where("CONCAT(loc.name,IF(loc.addr1 IS NOT NULL ,CONCAT(',',loc.addr1),''),IF(loc.addr2 IS NOT NULL ,CONCAT(',',loc.addr2),''),loc.city,loc.zipcode,state.state_name) LIKE '%" . addslashes($location) . "%'");
				}
			}
		}
		
		// Filter - scheduled days
		if(! empty($scheduledDays)) {
			$enrollQuery->where("DATE_FORMAT(CONVERT_TZ(CONCAT( DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.start_time,':00'),spl.attr2,'$userTimezone'),'%Y-%m-%d %H:%i:%s') 
		BETWEEN CONVERT_TZ(NOW(),spl.attr2,'$userTimezone') AND DATE_ADD(convert_tz(NOW(),spl.attr2,'$userTimezone'),interval $scheduledDays day)");
		}
		$afterCompleteAllowTime = getConfigValue('allow_meeting_launch_complete');
		$addSessionTime = ($afterCompleteAllowTime) ? $afterCompleteAllowTime : 30;
		
		expDebug::dPrint("regstatus  -- >>" . $regstatus);
		
		// For API
		if($_REQUEST['apiname'] == "ListInstructorClassesAPI" && ($regstatus == "scheduled_completed" || $regstatus == "completed_scheduled")) {
			$enrollQuery->addExpression("if(CONCAT( DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.end_time,':00') >
  															date_sub(IFNULL(CONVERT_TZ(now(), '" . $defaultTimezone . "' ,
  															(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone)),NOW()) ,
  															 INTERVAL '" . $addSessionTime . "' MINUTE),'scheduled','completed')", 'classstatus'); // Added by Vincent for #0033458
			$enrollQuery->groupBy("ses.class_id");
		} else {
			if($regstatus == 'scheduled') {
				$enrollQuery->addExpression('\'scheduled\'', 'classstatus'); // Added by Vincent for #0033458
				$enrollQuery->groupBy("ses.class_id having session_end_datetime > date_sub(IFNULL(CONVERT_TZ(now(), '" . $defaultTimezone . "' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone)),NOW()) , INTERVAL '" . $addSessionTime . "' MINUTE)");
			} else if($regstatus == 'delivered') {
				$enrollQuery->addExpression('\'completed\'', 'classstatus'); // Added by Vincent for #0033458
				$enrollQuery->groupBy("ses.class_id having session_end_datetime <= date_sub(IFNULL(CONVERT_TZ(now(), '" . $defaultTimezone . "' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone)),NOW()) , INTERVAL '" . $addSessionTime . "' MINUTE)");
			} else {
				$enrollQuery->addExpression("if(CONCAT( DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.end_time,':00') >
  															date_sub(IFNULL(CONVERT_TZ(now(), '" . $defaultTimezone . "' ,
  															(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone)),NOW()) ,
  															 INTERVAL '" . $addSessionTime . "' MINUTE),'scheduled','completed')", 'classstatus');
				$enrollQuery->groupBy("ses.class_id");
			}
		}
		
		if($_REQUEST['apiname'] == "ListInstructorClassesAPI") {
			if(! empty($_REQUEST['delivery_type'])) {
				$delivery_type = explode("|", $_REQUEST['delivery_type']);
				$enrollQuery->condition('cat.delivery_type', $delivery_type, 'IN');
			}
			if(! empty($_REQUEST['searchtext'])) {
    	  $enrollQuery->condition(db_or()->condition('cat.title', '%' . db_like($_REQUEST['searchtext']) . '%' ,'LIKE')
                               			->condition('cat.code', '%' . db_like($_REQUEST['searchtext']) . '%' ,'LIKE'));
			}
			
			if($_REQUEST['sortBy'] == 'Time') {
				$enrollQuery->orderBy('ses.start_date', 'asc');
				$enrollQuery->orderBy('ses.end_time', 'asc');
			}
			
			if(! empty($_REQUEST['location'])) {
				if($_REQUEST['apiname'] == "ListInstructorClassesAPI" ) {
		  		   /* Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404 */		
		  			$loc_or_condition_obj=getLocationFilterQuery($_REQUEST['location'],'1','','state');   
			 		expDebug::dPrint('$loc_or_condition_obj ='.print_r($loc_or_condition_obj,true),5);   
		  			$enrollQuery->condition($loc_or_condition_obj);  
				} else {
					$location = explode("|", $_REQUEST['location']);
					$enrollQuery->condition('loc.id', $location, 'IN');
				}
			}
			
			if(! empty($_REQUEST['startdate']) && ! empty($_REQUEST['enddate'])) {
				$enrollQuery->condition('ses.start_date', $_REQUEST['startdate'], '>=');
				$enrollQuery->condition('ses.start_date', $_REQUEST['enddate'], '<=');
			}
		}
		
		expDebug::dPrint('sortbyvalue' . print_r($_REQUEST["sortBy"], true), 5);
		if(isset($_REQUEST["sortBy"])) {
			$sortBy = $_REQUEST["sortBy"];
			if($sortBy == 'AZ') {
				$enrollQuery->orderBy('cat.title', 'asc');
			}
			if($sortBy == 'ZA') {
				$enrollQuery->orderBy('cat.title', 'desc');
			}
			if($sortBy == 'dateOld') {
				$enrollQuery->orderBy('cat.id', 'desc');
			}
			if($sortBy == 'dateNew') {
				$enrollQuery->orderBy('cat.id', 'asc');
			}
			if($sortBy == 'type') {
				$enrollQuery->orderBy('cat.delivery_type', 'asc');
			}
			if($sortBy == 'orderbystatus') {
				$enrollQuery->orderBy('classstatus', 'desc');
			}
		} else {
			$enrollQuery->orderBy('cat.title', 'asc');
		}

		if($count) {
			$countQuery = db_select($enrollQuery, 'countquery');
			$countQuery->addExpression('COUNT(countquery.cls_id)', 'clscount');
			return $countQuery;
  }
		return $enrollQuery;
	} catch(Exception $e) {
		watchdog_exception('getInstructorEnrollmentQuery', $ex);
    expertusErrorThrow($ex);
  }
}
function getInstructorMarkComplete(){
  try{
  $result = getProfileListItemByCode('cre_sys_brl_imc');
	return $result[0]->status;
  }catch (Exception $ex) {
    watchdog_exception('getInstructorMarkComplete', $ex);
    expertusErrorThrow($ex);
  }
}
function getclassstatusforIns($classId){
	$timeZone = date_default_timezone(false);
	$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
	$afterCompleteAllowTime = getConfigValue('allow_meeting_launch_complete');
	$addSessionTime = ($afterCompleteAllowTime) ? $afterCompleteAllowTime : 30;
	$select = db_select('slt_course_class', 'cls');
	$select->leftjoin('slt_course_class_session', 'ses', "ses.class_id = cls.id ");
	$select->condition('cls.id', $classId);
	$select->addExpression("if(CONCAT( DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.end_time,':00') >
  															date_sub(IFNULL(CONVERT_TZ(now(), '".$defaultTimezone."' ,
  															(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone)),NOW()) ,
  															 INTERVAL '".$addSessionTime."' MINUTE),'scheduled','completed')",'classStatus');
	$select->where("ses.id = (select id from slt_course_class_session where class_id=cls.id order by start_date desc, end_time desc limit 1)"); // 78679 - get last session date in the class
	$clsStatus  = $select->execute()->fetchField();
	return $clsStatus;
}

function getClassLearnerResults($classId, $fromAPI = 0){
  try{
  $page      = $_REQUEST['page'];
  $limit     = ($fromAPI) ? $_REQUEST['limit'] : $_REQUEST['rows'];
  $sidx      = $_REQUEST['sidx'];
  $sord      = $_REQUEST['sord'];
  $tab       = $_REQUEST['tab'];
  $date_from = trim($_REQUEST['date_from']);
  $date_to   = trim($_REQUEST['date_to']);
  $pagination = array();
  expDebug::dPrint("Instructor getClassLearnerQuery Fetch DATA :-");

  $timeZone = date_default_timezone(false);
  $defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
  $afterCompleteAllowTime = getConfigValue('allow_meeting_launch_complete');
  $addSessionTime = ($afterCompleteAllowTime) ? $afterCompleteAllowTime : 30;

  $select = db_select('slt_enrollment', 'enroll');
  $select->addField('per','id', 'LearnerId');
  $select->addField('enroll','id', 'EnrollId');
  $select->addField('enroll','master_enrollment_id', 'MasterEnrollId');
  $select->addField('enroll','reg_status', 'Status');
  $select->addField('enroll','comp_status', 'CompStatus');
  $select->addField('enroll','class_id', 'ClassId');
  $select->addField('enroll','pre_status', 'PreStatus');
  $select->addField('enroll','pre_score', 'PreScore');
  $select->addField('enroll','mandatory', 'Mandatory');
  $select->addField('cls','title', 'ClassTitle');
  $select->addField('per','user_name', 'UserName');
  $select->addField('per','full_name','FullName');
  $select->addField('pro','name', 'JobTitle');
  $select->addField('per','email','email');
  $select->addField('per','phone_no','phone_no');
  $select->addField('per','mobile_no','mobile_no');
  $select->addField('mobattend','status','Attendance');
  $select->addField('mobattend','reached','reached');
  $select->addExpression("CONCAT(per.last_name,', ',per.first_name)", 'Name');
  $select->addExpression("DATE_FORMAT(enroll.reg_date, '%b %d, %Y')", 'EnrolledDate');
  $select->addExpression("(SELECT attr1 from slt_profile_list_items where code = cls.delivery_type)", 'BaseType');
  $select->addExpression("(SELECT name from slt_profile_list_items where code = enroll.reg_status)", 'StatusText');
  $select->addExpression("(SELECT name from slt_profile_list_items where code = enroll.comp_status)", 'CompStatusText');
  // currently we are not using slt_mro_mapping
  //$select->addExpression("(SELECT CONCAT(splt_mro.name) FROM slt_mro_mapping mro LEFT OUTER JOIN slt_profile_list_items splt_mro ON mro.mro_flag = splt_mro.code  WHERE mro.entity_id = ClassId AND mro.entity_type = 'cre_sys_obt_cls' AND mro.job_role_id = (SELECT CODE FROM slt_profile_list_items WHERE CODE = (SELECT job_role FROM slt_person WHERE id = LearnerId) AND is_active='Y') AND mro.mro_flag IN (SELECT CODE FROM slt_profile_list_items WHERE code like '%cre_sys_inv_%' and is_active = 'Y'))",'Mro');
  $select->addExpression("(SELECT COUNT(*) from slt_enrollment where class_id = enroll.class_id and reg_status NOT IN ('lrn_crs_reg_can','lrn_crs_reg_wtl'))", 'EnrolledCount');
  $select->addExpression("(SELECT COUNT(*) from slt_enrollment where class_id = enroll.class_id and comp_status IN('lrn_crs_cmp_enr'))", 'CompStatusCount');
  $select->addExpression("(SELECT COUNT(*) FROM slt_course_class_session session WHERE CONCAT(DATE(session.start_date),' ',CONCAT(session.start_time,':00')) <= (CONVERT_TZ(now(), '".$defaultTimezone."' ,(select profile.attr2 from slt_profile_list_items profile where profile.code= session.timezone))) and session.class_id = $classId)", 'IsSessionStart');
  $select->addExpression("if(CONCAT( DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.end_time,':00') >
  															date_sub(IFNULL(CONVERT_TZ(now(), '".$defaultTimezone."' ,
  															(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone)),NOW()) ,
  															 INTERVAL '".$addSessionTime."' MINUTE),'scheduled','completed')",'classStatus');
  $select->addExpression("DATE_FORMAT(`enroll`.`comp_date`,_utf8'%d-%b-%Y')", 'comp_date');

  															 
  $select->join('slt_person', 'per', 'per.id = enroll.user_id');
  $select->join('slt_course_class', 'cls', 'cls.id = enroll.class_id');
  $select->leftjoin('slt_mobile_ilt_attendance', 'mobattend', 'mobattend.regid = enroll.id');
  $select->leftJoin('slt_profile_list_items', 'pro', 'per.job_title = pro.code');
  $select->leftjoin('slt_course_class_session', 'ses', "ses.class_id = cls.id ");
  $select->condition('enroll.class_id', $classId);
  //$select->where("enroll.reg_status NOT IN ('lrn_crs_reg_rsc','lrn_crs_reg_rsv') and ( enroll.comp_status NOT IN ('lrn_crs_cmp_inc') OR enroll.comp_status IS NULL )");
  // below line commented for Mantis - SQL Injection #31297
  //$select->where("enroll.reg_status IN ('lrn_crs_reg_cnf','lrn_crs_reg_wtl') ");
  $select->where("enroll.class_id=:classid AND enroll.reg_status IN ('lrn_crs_reg_cnf','lrn_crs_reg_can')",array(':classid'=>$classId));
  //,'lrn_crs_reg_wtl'
  $select->where("ses.id = (select id from slt_course_class_session where class_id=enroll.class_id order by start_date desc, end_time desc limit 1)"); // 78679 - get last session date in the class
  $select->groupBy('enroll.user_id');
  $select->groupBy('enroll.course_id');
  $select->groupBy('enroll.class_id');
  //Added for #0066430
  $select->groupBy('enroll.reg_status');
  
  if($_REQUEST['apiname'] == 'ListEnrollmentsByInstructor'){
  	if(!empty($date_from)) {
  		$date_from = $date_from;
  		$from_timestamp = strtotime($date_from);
  		$from_format = date("Y-m-d H:i:s", $from_timestamp);
  	}
  	if(!empty($date_to)) {
  		$date_to = $date_to;
  		$to_timestamp = strtotime($date_to);
  		$to_format = date("Y-m-d 23:59:59", $to_timestamp);
  	}
  	if (($date_from == "") && (!empty($date_to))) {
  		$select->condition('enroll.reg_date',$to_format,'<=');
  	}
  	else if(($date_to==  "") && (!empty($date_from))) {
  		$select->condition('enroll.reg_date',$from_format,'>=' );
  	}
  	else if(!empty($date_from) && !empty($date_to)) {
  		if($from_timestamp <= $to_timestamp) {
  			$select->condition('enroll.reg_date',array($from_format,$to_format),'BETWEEN' );
  		}
  	}
  }
  //$select->groupBy('enroll.user_id');
  //$select->groupBy('enroll.reg_status');
  // Sorting related
  $sord = $sord==''?'asc' : $sord;
  if($sidx == 'status' && $tab == 'scheduled'){
    $select->orderBy('enroll.reg_status', $sord);
  } else if($sidx == 'status'){
    $select->orderBy('enroll.comp_status', $sord);
  } else if($sidx == 'enrolled_on'){
    $select->orderBy('enroll.reg_date', $sord);
  } else if($sidx == 'name'){
    $select->orderBy('per.last_name', $sord);
    $select->orderBy('per.first_name', $sord);
  }else if($sidx == 'username'){
  	$select->orderBy('per.user_name', $sord);
  }else if($sidx == 'reached'){
  	$select->orderBy('mobattend.reached', $sord);
  }

  $count = 0;
  expDebug::dPrintDBAPI("Instructor getClassLearner Query - ",$select);
  $count = count($select->execute()->fetchAll());
  expDebug::dPrint("Instructor getClassLearnerQuery total count: ". $count , 3);

  /* Paging Related */
  if( $count >0 ){
  	$total_pages = ceil($count/$limit);
  }else{
  	$total_pages = 0;
  }
  if($page > $total_pages){
  	$page = $total_pages;
  }
  $start = $limit * $page - $limit;
  if($start < 0) {
  	$start = 0;
  }
  $totalRecords = $select->countQuery()->execute()->fetchField();
  $select->range($start, $limit);
  expDebug::dPrintDBAPI("Instructor getClassLearnerQuery Query: ", $select);

  // Implemented for assign learning filter in mobile
  if(isset($_REQUEST['apiname']) && $_REQUEST['apiname'] == 'ListEnrollmentsByInstructor' && $_REQUEST['mobile_version']){
  
  	$fields_attendance = $_REQUEST ['attendance'];
  	$fields_Completeion = $_REQUEST ['Compl_status'];
  	expDebug::dPrint ( "PrintTheValue fields_attendance " . $fields_attendance, 4 );
  	expDebug::dPrint ( "PrintTheValue fields_Completeion" . $fields_Completeion, 4 );
  	 
  	if (! empty ( $_REQUEST ['searchtext'] )) {
  		$select->condition ( db_or ()->condition ( 'per.user_name', '%' . db_like ( $_REQUEST ['searchtext'] ) . '%', 'LIKE' )->condition ( 'per.full_name', '%' . db_like ( $_REQUEST ['searchtext'] ) . '%', 'LIKE' ) );
  	}
  	// for attended -> Status will be attneded
  	// for NOT attended -> Status will be NULL
  	if (! empty ( $_REQUEST ['Compl_status'] ) || ! empty ( $_REQUEST ['attendance'] )) {
  		 
  		if (strpos ( $fields_attendance, 'Not Attended' ) == false) {
  			 
  			if (! empty ( $_REQUEST ['Compl_status'] )) {
  				$Select_query = "((SELECT name from slt_profile_list_items where code = enroll.comp_status) IN ('" . $fields_Completeion . "'))";
  			}
  			if (! empty ( $_REQUEST ['attendance'] )) {
  				 
  				if (! empty ( $_REQUEST ['Compl_status'] )) {
  					$Select_query .= " OR ";
  				}
  				if (strpos ( $fields_attendance, 'Attended' ) !== false) {
  					$Select_query .= "(mobattend.status IN ('Attended'))";
  				}
  				if (strpos ( $fields_attendance, 'NULL' ) !== false) {
  					$Select_query .= "(mobattend.status IS NULL)";
  					if (strpos ( $fields_Completeion, 'Completed' ) == false) {
  						$Select_query .= "AND ((SELECT name from slt_profile_list_items where code = enroll.comp_status)  != 'Completed')";
  					}
  				}
  			}
  			$select->where ( "(" . $Select_query . ")" );
  		}
  	}
  	expDebug::dPrintDBAPI ( "Instructor getClassLearnerQuery with filter changes Query1: ", $select );
  
  }
  else if(isset($_REQUEST['apiname']) && $_REQUEST['apiname'] == 'ListEnrollmentsByInstructor'){
  	if (!empty($_REQUEST['searchtext'])){
  
  		$select->condition(db_or()->condition('per.user_name', '%' . db_like($_REQUEST['searchtext']) . '%' ,'LIKE')
  				->condition('per.full_name', '%' . db_like($_REQUEST['searchtext']) . '%' ,'LIKE'));
  	}
  	if (!empty($_REQUEST['attendance']) && $_REQUEST['attendance'] == 'attended'){
  		$select->condition('mobattend.status',$_REQUEST['attendance']);
  	}else if(!empty($_REQUEST['attendance']) && $_REQUEST['attendance'] == 'notattended'){
  		$select->where("mobattend.status IS NULL");
  	}
  }
  
  
  // Execute query
  $lpQryRes = $select->execute();

  $selectMax = db_select("slt_course_class","cls");
  $selectMax->addField('cls','max_seats','max_seats');
  // below line commented for Mantis - SQL Injection - #31297
  //$selectMax->condition('cls.id',$classId);
  $selectMax->where('cls.id=:classid',array(':classid'=>$classId));
  $classMaxSeats = $selectMax->execute()->fetchField();
  $maxCapacity = $classMaxSeats;

  $regStatusEnrolled  = array('lrn_crs_reg_cnf');
  $compStatusEnrolled = array('lrn_crs_cmp_enr','lrn_crs_cmp_inp','lrn_crs_cmp_att');
  $enrolled = getInstructorEnrollCount($classId, $regStatusEnrolled,$compStatusEnrolled);

  $regStatusPending  = array('lrn_crs_reg_ppm', 'lrn_crs_reg_ppv');
  $compStatusPending = array();
  $pending = getInstructorEnrollCount($classId, $regStatusPending,$compStatusPending);

  $regStatusCompleted  = array('lrn_crs_reg_cnf');
  $compStatusCompleted = array('lrn_crs_cmp_cmp');
  $completed = getInstructorEnrollCount($classId, $regStatusCompleted,$compStatusCompleted);

  $regStatusNoshow  = array('lrn_crs_reg_cnf');
  $compStatusNoshow = array('lrn_crs_cmp_nsw');
  $noshow = getInstructorEnrollCount($classId, $regStatusNoshow,$compStatusNoshow);

  $regStatusWaitlisted  = array('lrn_crs_reg_wtl');
  $compStatusWaitlisted = array();
  $waitlist = getInstructorEnrollCount($classId, $regStatusWaitlisted,$compStatusWaitlisted);

  $regStatusCancelled = array('lrn_crs_reg_can');
  $compStatusCancelled = array();
  $cancelled = getInstructorEnrollCount($classId, $regStatusCancelled,$compStatusCancelled);

  $regStatusIncomplete = array('lrn_crs_reg_cnf');
  $compStatusIncomplete = array('lrn_crs_cmp_inc');
  $incomplete = getInstructorEnrollCount($classId, $regStatusIncomplete,$compStatusIncomplete);

  $notEnrolled = $maxCapacity - ($enrolled + $pending + $completed + $noshow + $incomplete);
  $separator = '&nbsp;&nbsp;|&nbsp;&nbsp;';
  $pagination['seat_details'] =  t('LBL128') . ' : ' . $maxCapacity . $separator;
  if($enrolled)  $pagination['seat_details'] .= t('Enrolled') . ' : ' . $enrolled . $separator;
  if($completed) $pagination['seat_details'] .= t('Completed') . ' : ' . $completed . $separator;
  if($noshow)    $pagination['seat_details'] .= t('No Show') . ' : ' . $noshow . $separator;
  if($pending)   $pagination['seat_details'] .= t('Pending') . ' : ' . $pending . $separator;
  if($waitlist)  $pagination['seat_details'] .= t('Waitlist') . ' : ' . $waitlist . $separator;
  if($cancelled) $pagination['seat_details'] .= t('Canceled') . ' : ' . $cancelled . $separator;
  $pagination['seat_details'] .= t('LBL645') . ' : ' . $notEnrolled;

  $pagination['maxCapacity'] = $maxCapacity;
  $pagination['enrolled_count'] =  $enrolled + $completed + $noshow;
  $pagination['class_status'] = getclassstatusforIns($classId);
  $pagination['result'] = $lpQryRes;
  $pagination['totalrows'] = $totalRecords;
  $pagination['page'] = $page;
  $pagination['count'] = $count;
  $pagination['total_pages'] = $total_pages;
  $pagination['enrolled'] = $enrolled;
  return $pagination;
  }catch (Exception $ex) {
    watchdog_exception('getClassLearnerResults', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * Function to cancel the user registration
 */
function cancelUserRegistrationFromClass($LnrType,$enrolledId,$classId,$userId,$refundflag,$isCommerceEnabled) {
  try{
  expDebug::dPrint('cancelUserRegistrationFromClass called.');
  $updatedBy = getSltpersonUserId();
  require_once(drupal_get_path('module', 'exp_sp_lnrenrollment').'/exp_sp_lnrenrollment.inc');

  getDropEnrollment($userId,$LnrType,$enrolledId,$refundflag,$isCommerceEnabled,'', 0 ,1);
  expDebug::dPrint("user:".$updatedBy ."enroll id:".$enrolledId." refund flag::".$refundflag." isCommerce.".$isCommerceEnabled." assMand=".$assMand." frommytem=".$frommyteam." fromInstructor=1" , 4);

  //get the cancel date for classes
  $select = db_select('slt_enrollment', 'enr');
  $select->addField('enr','updated_on');
  // below line commented for Mantis - SQL Injection - #31297
  //$select->condition('enr.id',$enrolledId,'=');
  $select->where('enr.id=:enrollid',array(':enrollid'=>$enrolledId));
  $cancel_date  = $select->execute()->fetchField();
  $cancel_date = date('M d, Y',strtotime($cancel_date));
  expDebug::dPrint('cancelUserRegistrationFromClass ends.');

  expDebug::dPrint("auditlog: user".$userId.'enroll'.$enrolledId.'classid'.$classid , 4);
  add_audit_trail_entry($updatedBy, $classId, 'cre_sys_obt_cls', 'exp_sp_instructor_desk', 'Instructor', 'Instructor cancelled registration for learner',
                                         $userId, 'lrn_crs_reg_cnf', 'lrn_crs_reg_can');

  }catch (Exception $ex) {
    watchdog_exception('cancelUserRegistrationFromClass', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * Function to mark complete user registration from course-class(Manager View)
 */
function markCompleteUserFromCourseClass($LnrType,$enrolledId,$classId,$userId,$completionDate,$score, $attendanceType) {
  try{
  $updatedBy = getSltpersonUserId();
  $valcompDate = date('Y-m-d H:i',strtotime(str_replace('-', '/', $completionDate)));
  $printcompDate = date('M d, Y',strtotime(str_replace('-', '/', $completionDate)));

  expDebug::dPrint("markCompleteUserFromCourseClass : $LnrType,$enrolledId,$classId,$userId,$completionDate,$valcompDate,$printcompDate,$score,$attendanceType" , 3);
  
  $valScore = '';
  $compStatus = 'lrn_crs_cmp_nsw';
  $compStatusIns = 'lrn_crs_cmp_nsw_ins';
  if($attendanceType == 'attended'){
    $valScore = $score;
    $compStatus = 'lrn_crs_cmp_cmp';
    $compStatusIns = 'lrn_crs_cmp_cmp_ins';
  }
  $select = db_select('slt_course_class', 'coucla');
  $select->addField('coucla','course_id');
  // below line commented for Mantis - SQL Injection - #31297
  //$select->condition('coucla.id',$classId,'=');
  $select->where('coucla.id=:classid',array(':classid'=>$classId));
  $courseId  = $select->execute()->fetchField();
  //$txn = db_transaction(); //commented for dbStatementsExecution();
  try{
  	expDebug::dPrint("Mark completed enrollID".$enrolledId, 3);
    $userAndEnrollIds = explode(",", $enrolledId);

    expDebug::dPrint('"Mark completed userAndEnrollIds : '.print_r($userAndEnrollIds, true) , 4);

    for($i = 0; $i < count($userAndEnrollIds); $i++){
      $userid_enrollid = explode("_", $userAndEnrollIds[$i]);
      /*-- #38295 - Issue fix --*/
      $userId = $userid_enrollid[0];
      $enrolledId = $userid_enrollid[1];
    expDebug::dPrint("Mark completed".$courseId." ".$classId, 3);
    $username = getPersonDetails($userId);
    // Check the Class Registered in the  TP
    $enrollResult = CheckCourseRegisteredTP($classId,$courseId,$userId);
    $masterEnrollmentId = $enrollResult[0]->masterenrollmentid;
    $classenrollmentId = $enrollResult[0]->enrollmentid;
     // expDebug::dPrint(' $updateStmt for Master enrollment obj = '. $classenrollmentId.'master_enrollnent'.$masterEnrollmentId,5);
    $statements = array('statement' => true);
    if($masterEnrollmentId && $classenrollmentId) {
      // Update the slt enrollment comp status and update Master enrollment status

    	
    	 

    	$markcompleteUpdateTPClass = lp_enrollment_upd('','',$compStatusIns,$valcompDate,'','','','',$valScore,$classenrollmentId,$updatedBy,$userId,'',$classId,$courseId,$statements);
     //	$check = db_query($markcompleteUpdateTPClass)->fetchAll();
    	$check = $markcompleteUpdateTPClass;
    	if($check[0]->xdid_update > 0){
    		addICalToNotification();
    	}

  /*   	$currentDateTime = now();
    	//Update the Master Enrollment Status
    	$updateStmt = db_update('slt_master_enrollment');
    	$fields = array('overall_status' => 'lrn_tpm_ovr_enr',
    			'updated_by' => $updatedBy,
    			'updated_on' => $currentDateTime
    	);
    	$updateStmt->condition('id', $masterEnrollmentId);
    	$updateStmt->condition('user_id', $userId);
    	$updateStmt->condition('overall_status', 'lrn_tpm_ovr_enr');
    	$updateStmt->fields($fields);
    	$updateStmt->execute();
    	expDebug::dPrintDBAPI(' $updateStmt for Master enrollment obj = ' , $updateStmt); */

    }
    $markcompleteUpdate = lp_enrollment_upd('','',$compStatusIns,$valcompDate,'','','','',$valScore,$enrolledId,$updatedBy,$userId,'',$classId,$courseId,$statements);
    //$markcompleteUpdate = "call slp_enrollment_upd(NULL,NULL,'lrn_crs_cmp_cmp','".$valcompDate."',NULL,NULL,NULL,NULL,'".$valScore."',$enrolledId,$updatedBy,$userId,NULL,$classId,$courseId)";
    expDebug::dPrint("Mark completed 1", 4);
    expDebug::dPrint("Mark completed by instructor : ".$markcompleteUpdate , 4);
   // $check1 = db_query($markcompleteUpdate)->fetchAll();
    $check1 = $markcompleteUpdate;
    expDebug::dPrint("Mark completed ins ".$check1[0]->xdid_update, 4);
  //  if($check1[0]->xdid_update > 0){
  if($check1 > 0){
	    addICalToNotification();
	    
	    if($compStatus == 'lrn_crs_cmp_cmp') {
	      insertUserPointsForEachActionsPerformed($userId,'complete_class_training',0,'','others',$statements);
	      
	      $selectStmt = db_select('slt_course_content_mapper', 'content');
	      // Set conditions
	      $selectStmt->condition('content.course_id', $courseId);
	      $selectStmt->condition('content.class_id', $classId);
	      // Select fields and/or add expressions
	      $selectStmt->addField('content', 'id', 'id');
	      expDebug::dPrintDBAPI(' $selectStmt slt_course_content_mapper = ' , $selectStmt);
	      // Execute query
	      $result = $selectStmt->execute();
	      // expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
	    //  if ($result->rowCount() <= 0) {
	      	$updateFields['progress']  = 100;
	      /* }else{
	      	$enrollQuery = db_select('slt_enrollment', 'enroll');
	      	$enrollQuery->addField('enroll', 'progress' ,'progress');
	      	$enrollQuery->condition('enroll.order_id', $enrolledId);
	      	$enrollQuery->condition('enroll.class_id', $classId);
	      	$enrollQuery->condition('enroll.course_id', $courseId);
	      	$enrollResult = $enrollQuery->execute()->fetchAll();
	        $dbProgress = $enrollResult[0]->progress;
	      	$delivery_type = $enrollResult[0]->delivery_type;
	      	$updatePrgVal = ($dbProgress > 0 ) ? $dbProgress : 0;
	      	$updateFields['progress']  = $updatePrgVal;
	    } */
	      $updateFields['updated_by']  = $updatedBy;
	      $updateFields['updated_on']  = now();
	      $updateQuery = db_update('slt_enrollment');
	      $updateQuery->fields($updateFields);
	      $updateQuery->condition('id', $enrolledId);
	      $updateQuery->condition('comp_status',array('lrn_crs_cmp_cmp'),'IN');
	      $updateQuery->condition('reg_status','lrn_crs_reg_cnf');
	      expDebug::dPrintDBAPI(' $UpdateQuery = ' , $updateQuery,4);
	      //$rosUpdated = $updateQuery->execute();
	      if(!empty($statements)) {
	      	$statements[] = $updateQuery;
	      } else {
	      	$rosUpdated = $updateQuery->execute();
	      }
	    }
	    //Added by Joola for #090353
	    dbStatementsExecution($statements);
	    
	    if(isset($_REQUEST['apiname']) && $_REQUEST['apiname'] != 'InstructorMarkCompletionAPI'){
	    add_audit_trail_entry($updatedBy, $classId, 'cre_sys_obt_cls', 'exp_sp_instructor_desk', 'Instructor', 'Instructor marked class complete for learner',
	                                        $userId, 'lrn_crs_cmp_enr', $compStatus);
	
	    }
    }
    }
  }catch (Exception $ex) {
   // $txn->rollback();
    //unset($txn);
    watchdog_exception('markCompleteUserFromCourseClass', $ex);
    expertusErrorThrow($ex);
  }
  // Commit the transaction
 // unset($txn);

   if(isset($_REQUEST['apiname']) && $_REQUEST['apiname'] == 'InstructorMarkCompletionAPI'){
     $obj = new stdClass();
     $obj->compdate = $printcompDate;
     $obj->status = ($check1[0]->xdid_update > 0)?"success":'nochange';
     $obj->id = $enrolledId;
     return $obj;
   }
   else{
   	if($check1[0]->xdid_update > 0){
    	print $printcompDate.'|success'.'|'.$username['user_name'];
   	}else{
   		print $printcompDate.'|nochange'.'|'.$username['user_name'];
   	}
   }
  }catch (Exception $ex) {
    watchdog_exception('markCompleteUserFromCourseClass', $ex);
    expertusErrorThrow($ex);
  }
}

function markCompleteUserFromCourseClassByAPI()
{
	try{
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  $regids = explode(",",$_REQUEST["regids"]);
  $arr = array();
  $session_detail = getClassSessionsDateTime($_REQUEST["classid"]);
  $sccs_date_and_time = implode(' ', array($session_detail[0][date],$session_detail[0][end_time]));
  $curr_date_and_time =  date('Y-m-d H:i',strtotime("-30 minutes"));
  if (strtotime($curr_date_and_time) >= strtotime($sccs_date_and_time)) {
    list($mm,$dd,$yyyy) = explode('-',$_REQUEST["completiondate"]);
    if (!checkdate($mm,$dd,$yyyy)) {
       //return array((object)array('id'=>'Invalid date format.It should be mm-dd-yyyy.'));
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Invalid date format.It should be mm-dd-yyyy.";
    	return $errobj;
    }else {
      for($i=0;$i<count($regids);$i++){
      $select = db_select('slt_enrollment', 'enr');
  	  $select->addField('enr','user_id');
      $select->condition('enr.id',$regids[$i],'=');
      $select->condition('enr.class_id', $_REQUEST["classid"], '=');
      $regUserId  = $select->execute()->fetchField();
      $regId = $regUserId. '_' .$regids[$i];
      $res = markCompleteUserFromCourseClass("",$regId,$_REQUEST["classid"],$_REQUEST["userid"],$_REQUEST["completiondate"],$_REQUEST["score"], $_REQUEST["attendancetype"]);
      if(isset($_REQUEST['apiname']) && $_REQUEST['apicaller'] == 'mobile'){

        /*slt_mobile_ilt_attendance
        */
          try{
            $txn = db_transaction();
            $updLog  = db_update('slt_mobile_ilt_attendance');
            $updLog->fields(array(
                        'status' => "Completed",
                        'score' => $_REQUEST["score"],
                        'updated_by' => $_REQUEST["userid"],
                        'updated_on' =>now()

            ));
            $updLog->condition('regid', $regids[$i]);
            $logId = $updLog->execute();

          }
          catch (Exception $ex) {
                  $txn->rollback();
                  unset($txn);
                  watchdog_exception('orgadmin', $ex);
                  //return array((object)array('id'=>"failure"));
                  $errobj->isValidateError = 1;
                  $errobj->errcode = 'L_012';
                  $errobj->errormsg = "Error in updating mark completion.";
                  return $errobj;
          }
          unset($txn);
        }
        $arr[]=$res;
       }
      return $arr;
      }
    }
    else{
      //return array((object)array('id'=>"failure",'status'=> "Session Not finished"));
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Session Not finished.";
    	return $errobj;
    }
  }catch (Exception $ex) {
  	watchdog_exception('markCompleteUserFromCourseClassByAPI', $ex);
  	expertusErrorThrow($ex);
  }
}
/*
 * implementation of VC launch by instructor
 *
 */

function getVCLaunchDetailFroInstructor($classid) {
   try{
   	$userId = getIdOfLoggedInUser();
   	$timeZone = date_default_timezone(false);
   	$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
   	$afterCompleteAllowTime = getConfigValue('allow_meeting_launch_complete');
  	$addSessionTime = ($afterCompleteAllowTime) ? $afterCompleteAllowTime : 30;
  	$vlcLaunchQry = 'SELECT
		distinct ses.id AS Id,
		cls.title AS Title,
		ses.attendee_url AS launchurl,
		ses.presenter_url AS presenterlaunchurl,
		(SELECT attr1 FROM slt_profile_list_items lst WHERE lst.code=cls.delivery_type) AS BaseType,
		(SELECT `name` FROM slt_profile_list_items lst WHERE lst.code=cls.delivery_type) AS DeliveryType,
		cls.delivery_type AS DeliveryCode,
		ses.start_date AS SessionStarts,
		ses.end_date AS SessionEnds,
		ses.timezone AS SessionTimeZone,
		ses.type AS sessiontype
	FROM
		slt_course_class cls
		left join slt_course_class_session ses on ses.class_id=cls.id
		left join slt_session_instructor_details ses_ins on ses_ins.class_id=cls.id
		WHERE
		 cls.id=:clsid 
		 AND		
		ses.id = (select sess2.id from slt_course_class_session as sess2 
					left join slt_session_instructor_details ses_ins2 on ses_ins2.session_id=sess2.id 
					where sess2.class_id=:clsid AND ses_ins2.instructor_id=:usrid 
					and (CONCAT(DATE(sess2.start_date),\' \',CONCAT(sess2.end_time,\':00\'))) >  date_sub(IFNULL(CONVERT_TZ(now(), \''.$defaultTimezone.'\' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= sess2.timezone)),NOW()), INTERVAL \''.$addSessionTime.'\' MINUTE) 
					order by (CONVERT_TZ(CONCAT(DATE(sess2.start_date),\' \',CONCAT(sess2.start_time,\':00\')) ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= sess2.timezone), \''.$defaultTimezone.'\')) asc limit 1) 
  	    group by cls.id';

  	expDebug::dPrintDBAPI('$$vlcLaunchDetQry  -->', $vlcLaunchDetQry,1);
  	$vlcLaunchDetQry  = db_query($vlcLaunchQry, array(':clsid' => $classid,'usrid' => $userId));
  	expDebug::dPrintDBAPI('$$vlcLaunchDetQry  -->', $vlcLaunchDetQry,1);
    $vlcLaunchDetRes  = array($vlcLaunchDetQry->fetchAssoc());
    expDebug::dPrint("Get VC Launch details : ".$vlcLaunchQry,2);
    expDebug::dPrint("Get VC Launch details classid : ".$classid,2);
	
    return $vlcLaunchDetRes;
  }catch (Exception $ex) {
    watchdog_exception('getVCLaunchDetail', $ex);
    expertusErrorThrow($ex);
  }
}

function getVirutalClassLauncDetail($classId){
	try {
	$launchArr                       = array();
	$result = getVCLaunchDetailFroInstructor($classId); 

	$launchArr["LaunchType"]         = $result[0]["basetype"];//basetype launchtype
    $launchArr["ContentType"]        = $result[0]["contenttype"];
    $launchArr["LearnerLaunchURL"]   = $result[0]["launchurl"];
 	$launchArr["PresenterLaunchURL"] = $result[0]["presenterlaunchurl"];
	$launchArr["sessionId"] 				 = $result[0]["id"];
	$launchArr["SessionType"] 			 = $result[0]["sessiontype"];
 	return $launchArr;
 	}catch (Exception $ex) {
 		watchdog_exception('getVirutalClassLauncDetail', $ex);
 		expertusErrorThrow($ex);
 	}
}

function getClassLearnerResultsByAPI($classId)
{
	try {
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	if($_REQUEST['action'] == 'surveychart' || $_REQUEST['action'] == 'assessmentchart'){
		$arr =  getClassLearnerSurveyAssessmentResults($classId);
		$result =  $arr["result"]->fetchAll();
	    $resultCount = count($result);
	    $config           = getConfig("exp_sp");
	    $configSiteUrl    = $config["admin_site_url"];
	    for($i = 0; $i < $resultCount; $i++){
	          $drupalUid = getDrupalIdOfUser($result[$i]->learnerid);
	          $account = user_load($drupalUid);
	          $filepath = $account->picture->filename;
	          $path= '';
	          if($filepath){
	            $path = $configSiteUrl . '/sites/default/files/pictures/'. $filepath;
	          }
	        $result[$i]->profileimage_path = $path;
		}
		
		$result['totalrow']= $arr['totalrows'];
		expDebug::dPrint('Survey count  '.print_r($result, true) , 4);
		return $result;
	}
	
	$date_from = trim($_REQUEST['date_from']);
	$date_to = trim($_REQUEST['date_to']);
	if(!empty($date_from)){
  	if(!validateDateFormat($date_from))
  	{
  		$errobj=new stdClass();
  		$errobj->isValidateError = 1;
  		$errobj->field = "date_from";
  		$errobj->errormsg = 'From date should be in yyyy-mm-dd format';
  		return $errobj;
  	}
  }
  if(!empty($date_to)){
  	if(!validateDateFormat($date_to))
  	{
  		$errobj=new stdClass();
  		$errobj->isValidateError = 1;
  		$errobj->field = "date_to";
  		$errobj->errormsg = 'To date should be in yyyy-mm-dd format';
  		return $errobj;
  	}
  }
  if(!empty($date_from) && !empty($date_to)) {
  	if(strtotime($date_from) > strtotime($date_to)) {
  		$errobj=new stdClass();
  		$errobj->isValidateError = 1;
  		$errobj->field = "date_to";
  		$errobj->errormsg = 'To date should be greater than or equal to from date';
  		return $errobj;
  	}
  }
  $arr =  getClassLearnerResults($classId,1);
  $result =  $arr["result"]->fetchAll();
  //return $result;


  $resultCount = count($result);
  $config           = getConfig("exp_sp");
  $configSiteUrl    = $config["admin_site_url"];
  for($i = 0; $i < $resultCount; $i++){
        $drupalUid = getDrupalIdOfUser($result[$i]->learnerid);
        $account = user_load($drupalUid);
        $filepath = $account->picture->filename;
        $path= '';
        if($filepath){
          $path = $configSiteUrl . '/sites/default/files/pictures/'. $filepath;
        }
      $result[$i]->profileimage_path = $path;

	  if(!isset($_REQUEST['action'])){
      $query  = db_select('slt_survey_results', 'surRes');
      $query->leftjoin('slt_survey', 'ss', 'ss.id = surRes.survey_id');
      $query->addExpression('COUNT(surRes.Id)', 'surveyCount');
      $query->condition('surRes.entity_type','cre_sys_obt_cls', '=');
      $query->condition('ss.type','sry_det_typ_ass', '!=');
      $query->condition('surRes.entity_id', $classId, '=');
      $query->condition('surRes.user_id', $result[$i]->learnerid, '=');
      $surqryRes = $query->execute()->fetchAll();

      $query  = db_select('slt_survey_results', 'surRes');
      $query->leftjoin('slt_survey', 'ss', 'ss.id = surRes.survey_id');
      $query->addExpression('COUNT(surRes.Id)', 'surveyCount');
      $query->addField('ss','max_mark','max_score');
      $query->condition('surRes.entity_type','cre_sys_obt_cls', '=');
      $query->condition('ss.type','sry_det_typ_ass', '=');
      $query->condition('surRes.entity_id', $classId, '=');
      $query->condition('surRes.user_id', $result[$i]->learnerid, '=');
      $AssqryRes = $query->execute()->fetchAll();

      $query  = db_select('slt_enrollment', 'enr');
      $query->condition('enr.user_id',$result[$i]->learnerid, '=');
      $query->condition('enr.class_id', $classId, '=');
  	  $query->condition(db_or()->condition('enr.comp_status', 'lrn_crs_cmp_enr', '=')
                               ->condition('enr.comp_status', 'lrn_crs_cmp_cmp','='));
      $query->addField('enr','score','score');
      $score = $query->execute()->fetchAll();


      $result[$i]->surveyStatus = $surqryRes[0]->surveycount;
      $result[$i]->assessmentStatus = $AssqryRes[0]->surveycount;
      $result[$i]->score = $score[0]->score;
      $result[$i]->max_score = $AssqryRes[0]->max_score;
	  }

      expDebug::dPrint('Survey count  '.print_r($result[$i], true) , 4);


    }
    $result['totalrow']= $arr['totalrows'];
    expDebug::dPrint('Survey count  '.print_r($result, true) , 4);
  return $result;
  }catch (Exception $ex) {
  	watchdog_exception('getClassLearnerResultsByAPI', $ex);
  	expertusErrorThrow($ex);
  }
}

function updateAttendanceStatus($ClassId,$LearnerId,$EnrollId){
  try{
  $sessionId = db_query("SELECT id As sessionid  FROM slt_course_class_session  where class_id =:clsId limit 1", array(':clsId' => $ClassId))->fetchField();
  expDebug::dPrint('Course Class Session id : '.$sessionId , 5);

  $learnername = db_query("SELECT CONCAT(first_name,' ',last_name) As learnername  FROM slt_person  where id =:lnrId limit 1", array(':lnrId' => $LearnerId))->fetchField();
  expDebug::dPrint('Learner name learnername : '.$learnername , 5);

  $select = db_select('slt_mobile_ilt_attendance','milt');
  $select->addField('milt','regid','regid');
  $select->condition('milt.regid',$EnrollId,'=');
  $logId = $select->execute()->fetchField();
  expDebug::dPrintDBAPI("Mobile attendance qry :",$select);
  expDebug::dPrint('Mobile attendance id : '.$logId , 5);

  	if(!empty($logId)){
    	$updLog  = db_update('slt_mobile_ilt_attendance');
        $updLog->fields(array(
                      'status' => 'Attended',
                      'updated_by' => getSltpersonUserId(),
                      'updated_on' =>now()
        ));
        $updLog->condition('regid', $EnrollId);
        $updLog->execute();
        expDebug::dPrintDBAPI(' updateAttendanceStatus Velu = ' , $updLog);
  	}else{
  		$user_id=getSltpersonUserId();
    	$insLog  = db_insert('slt_mobile_ilt_attendance');
        $insLog->fields(array(
                      'learnerid' => $LearnerId,
                      'learnername' => $learnername,
                      'sessionid' => $sessionId,
                      'instructor_id' => $user_id,
        			  			'instructor_feedback' => NULL,
                      'status' => 'Attended',
                      'regid' => $EnrollId,
                      'created_by' => $user_id,
                      'created_on' => now(),
        							'updated_by' => $user_id,
        							'updated_on' =>now()
        ));
        $insLog->execute();
        expDebug::dPrintDBAPI(' insertAttendanceStatus Velu = ' , $insLog);
  	}
  	}catch (Exception $ex) {
  		watchdog_exception('updateAttendanceStatus', $ex);
  		expertusErrorThrow($ex);
  	}
}

function CheckCourseRegisteredTP($classId,$courseId,$userId) {
  try{
		$select = db_select('slt_enrollment', 'enroll');
		//$select->addExpression('COUNT(1)','comp_count');
		$select->addfield('enroll','master_enrollment_id','masterenrollmentid');
		$select->addfield('enroll','id','enrollmentid');
		$select->condition('enroll.comp_status', array('lrn_crs_cmp_nsw','lrn_crs_cmp_enr'),'IN');
		$select->condition('enroll.master_enrollment_id', '', 'IS NOT NULL');
		$select->condition('enroll.user_id', $userId);
		$select->condition('enroll.class_id', $classId);
		$select->condition('enroll.course_id',$courseId);
		expDebug::dPrintDBAPI('CheckCourseRegisteredTP = ', $select);
		$result = $select->execute()->fetchAll();
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('CheckCourseRegisteredTP', $ex);
		expertusErrorThrow($ex);
	}

}
function getInscountForWidget(){
	try{
		$user_id   	= stripQuotes($_GET['UserID']);
		$afterCompleteAllowTime = getConfigValue('allow_meeting_launch_complete');
		$addSessionTime = ($afterCompleteAllowTime) ? $afterCompleteAllowTime : 30;
		$enrollQuery = db_select('slt_course_class', 'cat');
		$enrollQuery->addExpression("count(1)",'cnt');
		$enrollQuery->addField("ses",'timezone');
		$enrollQuery->addExpression("CONCAT( DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.end_time,':00')", 'session_end_datetime');
		$enrollQuery->leftjoin('slt_course_template', 'crs', 'crs.id = cat.course_id');
		$enrollQuery->leftjoin('slt_course_class_session', 'ses', "ses.class_id = cat.id ");
		$enrollQuery->leftjoin('slt_location', 'loc', 'ses.location_id = loc.id');
		$enrollQuery->where("ses.id = if((select count(1) from slt_course_class_session where class_id=cat.id and (FIND_IN_SET(".$user_id.", instructor_id)))>0,
				(select id from slt_course_class_session where class_id=cat.id order by start_date desc, end_time desc limit 1), 0)");
		$enrollQuery->condition('cat.status', array('lrn_cls_sts_can','lrn_cls_sts_del'),'NOT IN');
		$enrollQuery->groupBy("ses.class_id having (session_end_datetime > date_sub(IFNULL(CONVERT_TZ(now(), '".$defaultTimezone."' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone)),NOW()) , INTERVAL '".$addSessionTime."' MINUTE))
				OR (session_end_datetime <= date_sub(IFNULL(CONVERT_TZ(now(), '".$defaultTimezone."' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone)),NOW()) , INTERVAL '".$addSessionTime."' MINUTE))");
		expDebug::dPrintDBAPI('$enrollQuery--->',$enrollQuery);
		$count = $enrollQuery->execute()->fetchAll();
		expDebug::dPrint('$count--->>'.print_r($count,true),4);
		return $count;
	}catch (Exception $ex) {
		watchdog_exception('getInscountForWidget', $ex);
		expertusErrorThrow($ex);
	}
}


function getInstructorActivityResultSet(){
	try{
		$user_id   	= stripQuotes($_REQUEST['UserID']);
		$pagination = array();
		expDebug::dPrint("getInstructorActivityResultSet111",4);
		if(isset($_REQUEST['apiname']) && $_REQUEST['apiname'] == 'ListInstructorClassesAPI'){
			drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		}
		$timeZone = date_default_timezone(false);
		expDebug::dPrint("getInstructorActivityResultSet222qq",4);
		
		$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
		expDebug::dPrint("getInstructorActivityResultSet222",4);
		
		$enrollQuery = db_select('slt_course_class', 'cat');
		//$enrollQuery->distinct('cat.id');
		$enrollQuery->addField('cat','id', 'id');
		$enrollQuery->addField('crs','title', 'course_title');
		$enrollQuery->addField('cat','code', 'cls_code');
		$enrollQuery->addField('cat','code', 'CODE');
		$enrollQuery->addField('cat','title', 'cls_title');
		$enrollQuery->addField('cat','short_description', 'description');
		$enrollQuery->addField('cat','delivery_type', 'delivery_type_code');
		$enrollQuery->addField('cat','course_id', 'courseid');
		$enrollQuery->addExpression("(SELECT name from slt_profile_list_items where code = cat.delivery_type)", 'delivery_type');
		$enrollQuery->addExpression("(SELECT attr1 from slt_profile_list_items where code =cat.delivery_type)", 'BaseType');
		$enrollQuery->leftjoin('slt_course_template', 'crs', 'crs.id = cat.course_id');
		$enrollQuery->leftjoin('slt_course_class_session', 'ses', "ses.class_id = cat.id ");
		$enrollQuery->leftjoin('slt_session_instructor_details', 'ses_ins', "ses_ins.class_id = cat.id ");
		/* Added/changed by Ashutosh Mishra */
		$enrollQuery->leftjoin('slt_location', 'loc', 'ses.location_id = loc.id');
		/* Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404 */
		if(isset($_REQUEST['apiname']) && $_REQUEST['apiname'] == 'ListInstructorClassesAPI'){
		    $enrollQuery->leftJoin('slt_country','loc_ctry','loc_ctry.country_code=loc.country');
		    $enrollQuery->leftJoin('slt_state','loc_stats','loc_stats.state_code = loc.state and loc_stats.country_code=loc.country');
		}

		// $enrollQuery->leftjoin('slt_person', 'p', 'p.id');
		$enrollQuery->where("ses.id = if((select count(1) from slt_session_instructor_details as ses_ins1 where ses_ins1.class_id=cat.id and ses_ins1.instructor_id =".$user_id.")>0,
        (select id from slt_course_class_session where class_id=cat.id order by start_date desc, end_time desc limit 1), 0)");
		/*  $enrollQuery->condition(db_or()->condition('cat.status', 'lrn_cls_sts_can', '!=')
		 ->condition('cat.status', 'lrn_cls_sts_del','!=')); */
		// Issue is fixed for this Ticket #0035162 Dont display cancel and Deleted classes
		$enrollQuery->condition('cat.status', array('lrn_cls_sts_can','lrn_cls_sts_del'),'NOT IN');

		$enrollQuery->addExpression("if(CONCAT( DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.end_time,':00') >
  															date_sub(IFNULL(CONVERT_TZ(now(), '".$defaultTimezone."' ,
  															(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone)),NOW()) ,
  															 INTERVAL '".$addSessionTime."' MINUTE),'scheduled','completed')",'classStatus'); // Added by Vincent for #0033458
		$enrollQuery->groupBy("ses.class_id");
		
		
	  	if($_REQUEST['action'] == 'surveyreport'){
	  		$enrollQuery->leftjoin('slt_survey_mapping', 'map','map.object_id = cat.id');
	  		$enrollQuery->leftjoin('slt_survey', 'sur','sur.id=map.survey_id');
	  		$enrollQuery->addField('sur', 'id','survey_status');
	  		$enrollQuery->condition('map.object_type', 'cre_sys_obt_cls', '=');
	  		$enrollQuery->condition('sur.status', 'sry_det_sry_atv', '=');
	  		$enrollQuery->condition('sur.type', 'sry_det_typ_ass', '!=');
	  	}else if($_REQUEST['action'] == 'assessmentreport'){
	  		$enrollQuery->leftjoin('slt_survey_mapping', 'map','map.object_id = cat.id');
	  		$enrollQuery->leftjoin('slt_survey', 'sur','sur.id=map.survey_id');
	  		$enrollQuery->addField('sur', 'id','assessment_status');
	  		$enrollQuery->condition('map.object_type', 'cre_sys_obt_cls', '=');
	  		$enrollQuery->condition('sur.status', 'sry_det_sry_atv', '=');
	  		$enrollQuery->condition('sur.type', 'sry_det_typ_ass', '=');  			
  		}		 	
		
		
		
		if ($_REQUEST['apiname']  == "ListInstructorClassesAPI")
		{
			if (!empty($_REQUEST['startdate']) && !empty($_REQUEST['enddate']) ){
				$enrollQuery->condition('ses.start_date', $_REQUEST['startdate'], '>=');
				$enrollQuery->condition('ses.start_date', $_REQUEST['enddate'], '<=');
			}
		}


		if(isset($_REQUEST["sortBy"])) {
			$sortBy 	 = $_REQUEST["sortBy"];
			if($sortBy == 'AZ') {
				$enrollQuery->orderBy('cat.title', 'asc');
			}
			if($sortBy == 'ZA') {
				$enrollQuery->orderBy('cat.title', 'desc');
			}
			if($sortBy == 'dateOld') {
				$enrollQuery->orderBy('cat.id', 'desc');
			}
			if($sortBy == 'dateNew') {
				$enrollQuery->orderBy('cat.id', 'asc');
			}
			if($sortBy == 'type') {
				$enrollQuery->orderBy('cat.delivery_type', 'asc');
			}
		} else {
			$enrollQuery->orderBy('cat.title', 'asc');
		}
		expDebug::dPrintDBAPI("Instructor getClassesQuery Query: ",$enrollQuery);

		// Execute query
		$lpQryRes = $enrollQuery->execute()->fetchAll();
		expDebug::dPrint('hfsdhfdshjfsdh'.print_r($lpQryRes, true) , 4);
		$regStatusesList = array('lrn_crs_reg_cnf');
		$compStatusesList = array('lrn_crs_cmp_inc','lrn_crs_cmp_enr','lrn_crs_cmp_inp','lrn_crs_cmp_cmp', 'lrn_crs_cmp_nsw','lrn_crs_cmp_att');
		
		if($_REQUEST['action'] == 'surveyreport'){				
			for($i=0;$i<count($lpQryRes);$i++){
			$lpQryRes[$i]->survey_completed_count = getSurveyCompletedUsersCount($lpQryRes[$i]->id,$lpQryRes[$i]->survey_status);				
			$lpQryRes[$i]->enrolled_count = getInstructorEnrollCount($lpQryRes[$i]->id, $regStatusesList,$compStatusesList);
				
			}
		}else if($_REQUEST['action'] == 'assessmentreport'){
			for($i=0;$i<count($lpQryRes);$i++){
			$lpQryRes[$i]->assessment_completed_count= getSurveyCompletedUsersCount($lpQryRes[$i]->id,$lpQryRes[$i]->assessment_status);				
			$lpQryRes[$i]->enrolled_count = getInstructorEnrollCount($lpQryRes[$i]->id, $regStatusesList,$compStatusesList);
			}
		}else{
		for($i=0;$i<count($lpQryRes);$i++){
			$sessionDetails = ($lpQryRes[$i]->basetype=='ILT' || $lpQryRes[$i]->basetype=='VC')?getSessionDetails($lpQryRes[$i]->courseid, $lpQryRes[$i]->id, $lpQryRes[$i]->delivery_type_code, "enroll",'','instructorDesk'):array();				
			$lpQryRes[$i]->sessionDetails = $sessionDetails;
			$lpQryRes[$i]->classStatus = $lpQryRes[$i]->classstatus;
		}
		}
		return $lpQryRes;
	}catch (Exception $ex) {
		watchdog_exception('getInstructorActivityResultSet', $ex);
		expertusErrorThrow($ex);
	}
}

function getClassLearnerSurveyAssessmentResults($classId){
  try{
  $page      = $_REQUEST['page'];
  $limit     = $_REQUEST['rows'];
  $sidx      = $_REQUEST['sidx'];
  $sord      = $_REQUEST['sord'];
  $tab       = $_REQUEST['tab'];
  $pagination = array();
  expDebug::dPrint("Instructor getClassLearnerQuery Fetch DATA :-");
  $timeZone = date_default_timezone(false);
  $defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
  $select = db_select('slt_enrollment', 'enroll');
  $select->addField('per','id', 'LearnerId');
  $select->addField('enroll','id', 'EnrollId');
  $select->addField('enroll','master_enrollment_id', 'MasterEnrollId');
  $select->addField('enroll','reg_status', 'Status');
  $select->addField('enroll','comp_status', 'CompStatus');
  $select->addField('enroll','score', 'score');
  
  $select->addField('per','user_name', 'UserName');
  $select->addField('per','full_name','FullName');
  $select->addField('pro','name', 'JobTitle');
  $select->addField('per','email','email');
  $select->addField('per','phone_no','phone_no');
  $select->addField('per','mobile_no','mobile_no');
  $select->addExpression("CONCAT(per.last_name,', ',per.first_name)", 'Name');
  $select->addExpression("(SELECT attr1 from slt_profile_list_items where code = cls.delivery_type)", 'BaseType');


  $select->join('slt_person', 'per', 'per.id = enroll.user_id');
  $select->join('slt_course_class', 'cls', 'cls.id = enroll.class_id');
  $select->leftJoin('slt_profile_list_items', 'pro', 'per.job_title = pro.code');
  
  
  $select->leftjoin('slt_survey_results', 'surRes', "surRes.entity_id = enroll.class_id and surRes.entity_type = 'cre_sys_obt_cls'");
  $select->leftjoin('slt_survey', 'ss', 'ss.id = surRes.survey_id');
  
  
  $not = "";
  $max_mark_not_null = "";
  
  if($_REQUEST['status'] == 'lrn_crs_cmp_cmp')
  {
    $not = "in";
    $max_mark_not_null = "and ss.max_mark is not null";
  }else {
      
    $not = "not in";
    $max_mark_not_null = "";
  }




  if($_REQUEST['action'] == 'surveychart'){
      $select->where("enroll.reg_status = 'lrn_crs_reg_cnf' and enroll.class_id = ".$classId." and enroll.user_id  ".$not." (SELECT `user_id` FROM `slt_survey_results` ssr left join slt_survey ss on ss.id = ssr.survey_id where ssr.entity_id = ".$classId." and ss.type!='sry_det_typ_ass'  )");

  	//$select->addExpression('COUNT(surRes.Id)', 'surveystatus');
  	 
  }else if ($_REQUEST['action'] == 'assessmentchart'){
      $select->where("enroll.reg_status = 'lrn_crs_reg_cnf' and enroll.class_id = ".$classId." and enroll.user_id  ".$not." (SELECT `user_id` FROM `slt_survey_results` ssr left join slt_survey ss on ss.id = ssr.survey_id where ssr.entity_id = ".$classId." and ss.type ='sry_det_typ_ass'  ) ".$max_mark_not_null);

  	//$select->addExpression('COUNT(surRes.Id)', 'assessmentstatus');
  	$select->addField('ss','max_mark','max_score');
  }
  
  
  $select->groupBy('enroll.user_id');
  
  
  

  // Sorting related
  $sord = $sord==''?'asc' : $sord;
  if($sidx == 'status' && $tab == 'scheduled'){
    $select->orderBy('enroll.reg_status', $sord);
  } else if($sidx == 'status'){
    $select->orderBy('enroll.comp_status', $sord);
  } else if($sidx == 'enrolled_on'){
    $select->orderBy('enroll.reg_date', $sord);
  } else if($sidx == 'name'){
    $select->orderBy('per.last_name', $sord);
    $select->orderBy('per.first_name', $sord);
  }else if($sidx == 'username'){
  	$select->orderBy('per.user_name', $sord);
  }

  $count = 0;
  expDebug::dPrintDBAPI("Instructor getClassLearner Query - ",$select);
  $count = count($select->execute()->fetchAll());
  expDebug::dPrint("Instructor getClassLearnerQuery total count: ". $count , 3);

  /* Paging Related */
  if( $count >0 ){
  	$total_pages = ceil($count/$limit);
  }else{
  	$total_pages = 0;
  }
  if($page > $total_pages){
  	$page = $total_pages;
  }
  $start = $limit * $page - $limit;
  if($start < 0) {
  	$start = 0;
  }
  $totalRecords = $select->execute()->rowCount();
  $select->range($start, $limit);
  expDebug::dPrintDBAPI("Instructor getClassLearnerQuery Query: ", $select);



  // Execute query
  $lpQryRes = $select->execute();


  $pagination['result'] = $lpQryRes;
  $pagination['totalrows'] = $totalRecords;
  $pagination['page'] = $page;
  $pagination['count'] = $count;
  $pagination['total_pages'] = $total_pages;
  return $pagination;
  }catch (Exception $ex) {
    watchdog_exception('getClassLearnerResults', $ex);
    expertusErrorThrow($ex);
  }
}

function getAutocompleteForInst(){
	try {

		$searchText =  $_GET['z'];
		$loggedUserId = getSltpersonUserId();
		$delivery_type = array("lrn_cls_dty_ilt","lrn_cls_dty_vcl");
		$solrSource = getConfigValue ( 'solr_search' );
		if ($solrSource == 0) {	
		$select = db_select('slt_course_class', 'cls');
		$select->innerJoin('slt_course_template','crs','crs.id=cls.course_id');
		$select->leftJoin('slt_course_class_session','ses','ses.class_id=cls.id');
		$select->leftJoin('slt_session_instructor_details','ins','ins.class_id=cls.id and ins.session_id=ses.id');
		$select->leftJoin('slt_tag_entity','tagentity','((tagentity.entity_id=cls.id and tagentity.entity_type="Class")
or (tagentity.entity_id=cls.course_id and tagentity.entity_type="Course"))');
		$select->leftJoin('slt_tagdefn', 'tagdef', 'tagentity.tagid=tagdef.id');
		$select->distinct();
		$select->addField('cls','id','clsid');
		$select->addField('crs','title','coursetitle');
		$select->addField('crs','code','coursecode');
		$select->addField('cls','title','classtitle');
		$select->addField('cls', 'code','classcode');
		$select->condition(db_or()
				->condition('crs.title', '%' . db_like($searchText) . '%', 'LIKE')
				->condition('crs.code', '%' . db_like($searchText) . '%', 'LIKE')
				->condition('crs.description', '%' . db_like($searchText) . '%', 'LIKE')
				->condition('cls.title', '%' . db_like($searchText) . '%', 'LIKE')
				->condition('cls.code', '%' . db_like($searchText) . '%', 'LIKE')
				->condition('cls.description', '%' . db_like($searchText) . '%', 'LIKE')
				->condition('tagdef.tagname', '%' . db_like($searchText) . '%', 'LIKE')
		);
		$select->condition('ins.instructor_id',$loggedUserId,'=');
		//$select->groupBy ( 'cls.id');
		//$elect->condition('cls.delivery_type',$delivery_type,'IN');
		//filterByUserAccessPrivileges($select,'','',1,'','Search');
		expDebug::dPrintDBAPI(' $select query getAutocompleteForInst= ' , $select);
		$names = $select->execute()->fetchAll();
		}else {
			require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_mylearning_myclasses.php';
				try {
					$solrObj = new MyLearningMyClassesSolrSearch();
					$solrList = $solrObj->myClassesSolrSearchAutoComplete();
					$names = $solrList ['records'];
					expDebug::dPrint ( ' $prgList123 = ' . print_r ( $names, true ), 4 );
					$numCatalog = $solrList ['recCount'];
					expDebug::dPrint ( ' Total matiching records ' . print_r ( $numCatalog, true ), 4 );
				} catch ( Exception $e ) {
					expDebug::dPrint ( "Error in Solr Search for Text search autocomplete - " . print_r ( $e->getMessage (), true ), 1 );
				}

		}
		expDebug::dPrint(' Names = '. print_r($names, true) , 3);
	  
	    foreach($names as $result){
	    	if ($solrSource == 0) {
	    		print $result->classtitle . "\n";
	    	} else {
	    		print $result->cls_title . "\n";
	    	}
	    }
		

		//return $names;

	} catch (Exception $ex) {
		watchdog_exception('getAutocompleteSearch', $ex);
		expertusErrorThrow($ex);
	}
}

function getAutocompleteForLoc(){
	try {
		 $loggedInUserId = getSltpersonUserId();
		$searchCurText =  $_GET['z'];
      	$select = db_select('slt_location', 'location');
        $select->leftjoin('slt_state','state','state.state_code=location.state and state.country_code=location.country');
        $select->leftJoin('slt_country','country','location.country=country.country_code');
        $select->addField('location','name');
        $select->addField('location','id');
        $select->addField('location','addr1');
        $select->addField('location','addr2');
        $select->condition(db_or()
      		->condition('location.name', '%' . db_like($searchCurText) . '%', 'LIKE')
	      	->condition('location.city', '%' . db_like($searchCurText) . '%', 'LIKE')
	      	->condition('location.addr1', '%' . db_like($searchCurText) . '%', 'LIKE')
	      	->condition('location.addr2', '%' . db_like($searchCurText) . '%', 'LIKE')
	      	->condition('location.zipcode', '%' . db_like($searchCurText) . '%', 'LIKE')
      		->condition('state.state_name', '%' . db_like($searchCurText) . '%', 'LIKE')
     		->condition('country.country_name', '%' . db_like($searchCurText) . '%', 'LIKE')
     	);
		expDebug::dPrintDBAPI("Class Autocomplete --->> " , $select);
		$result = $select->execute()->fetchAll();
		foreach ($result as $loc) {
			$result = $loc->name;
			if(!empty($loc->addr1))
				$result .= ','.$loc->addr1;
			if(!empty($loc->addr2))	
				$result .= ','.$loc->addr2;
			print $result."\n";
		} 
	} catch (Exception $ex) {
		watchdog_exception('getEnrollClassLocation', $ex);
		expertusErrorThrow($ex);
	}
	
}

?>