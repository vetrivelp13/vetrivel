<?php
/**
 * Fetch Learning plan detail info to show on course detail page.
 */

function getlearning_DetailsInfo($programId) {
  try{
  $sel_array = array();
  if(!empty($programId)) {

   // $courseId = getEntityIdFromNodeId($programId);
    // Get learning details
       $sel_array['learning'] = getLearningDetails($programId);

  }

  return $sel_array;
  }catch (Exception $ex) {
    watchdog_exception('getlearning_DetailsInfo', $ex);
    expertusErrorThrow($ex);
  }
}



/**
 * Get learning plan details and increment view count
 * SQL query statement
 * SELECT
 * lp.id AS id,
 * lp.title AS title,
 * lp.description AS description,
 * lp.short_desc AS short_desc,
 * lp.code AS CODE,
 * lp.end_date AS end_date,
 * lp.expires_in_value AS expires_in_value,
 * lp.expires_in_unit AS expires_in_unit,
 * lp.custom0 AS view_count,
 * map.course_id AS course_id,
 * crs.title AS crs_title,
 * crs.code AS crs_code,
 * crs.description AS crs_description,
 * pro.code AS pro_code,
 * pro.name AS pro_type
 * FROM
 * slt_program lp
 * LEFT OUTER JOIN slt_module_crs_mapping map ON lp.id = map.program_id
 * LEFT OUTER JOIN slt_course_template crs ON map.course_id = crs.id
 * LEFT OUTER JOIN slt_profile_list_items pro ON lp.object_type = pro.code
 * WHERE  lp.id = 1
 *
 */
function getLearningDetails($programId){
  try{
  $vLMSUserId = getSltpersonUserId();
  $module_seq = getModuleId($vLMSUserId,$programId);
  $recertifyValue = ($module_seq[0]->recertify_path).'-R';
  $NextmoduleAvailable =getCourseMappingCount($programId,$recertifyValue);
  expDebug::dPrint("module seq ".print_r($module_seq,1),5);
  $module_path = 1;
  $allow_recertify = 0;
  expDebug::dPrint("count >> ". count($module_seq). "object type >> ".$module_seq[0]->object_type , 5);
  if(count($module_seq)> 0 && ($module_seq[0]->object_type == 'cre_sys_obt_crt')) {
  	if(($module_seq[0]->overall_status == 'lrn_tpm_ovr_cmp' && $NextmoduleAvailable >0 ) || ($module_seq[0]->overall_status == 'lrn_tpm_ovr_inc') )
  		$allow_recertify = getRecertifyBeforeCertifyExpired($module_seq[0]->comp_date, $module_seq[0]->expires_in_value ,$module_seq[0]->expires_in_unit); 
  	
  	if(($module_seq[0]->overall_status =='lrn_tpm_ovr_exp' && $allow_recertify == 1) || $allow_recertify == 1 ) 
  		$module_path = $module_seq[0]->recertify_path+1;
  		else
  	  	$module_path = $module_seq[0]->recertify_path;   
  	
  }
  //$module_path = !empty($module_seq[0]->recertify_path) ? $module_seq[0]->recertify_path : 1;
  expDebug::dPrint("module path ".$module_path,5);
  
  $module_id = getModuleIdFromObjectId($programId,$module_path);
  
  if(count($module_id)>0){ 
  	$modId = $module_id[0]->module_id; 
  }else{
 	$module_id = getModuleIdFromObjectId($programId,($module_path-1));
 	$modId = $module_id[0]->module_id;
  }
 
  
  // Select table
  $selStmt = db_select('slt_program', 'lp');
  $selStmt->distinct();
  $selStmt->leftJoin('slt_module_crs_mapping', 'map', 'lp.id = map.program_id');
//  $selStmt->leftJoin('slt_course_template', 'crs', 'map.course_id = crs.id');
  $selStmt->leftJoin('slt_profile_list_items', 'pro', 'lp.object_type = pro.code');
//  $selStmt->leftJoin('slt_mro_mapping', 'mro', 'mro.entity_id=lp.id AND mro.job_role_id = (SELECT job_role FROM slt_person WHERE id=\''.$vLMSUserId.'\') AND mro.entity_type IN (\'cre_sys_obt_trn\', \'cre_sys_obt_crt\', \'cre_sys_obt_cur\')');
  $selStmt->leftJoin('slt_node_learning_activity','nl','lp.id = nl.entity_id and nl.entity_type IN (\'cre_sys_obt_trn\', \'cre_sys_obt_crt\', \'cre_sys_obt_cur\')');
  //$selStmt->leftJoin('slt_catalog_access', 'catacs', 'catacs.entity_id=lp.id AND catacs.entity_type IN (\'cre_sys_obt_trn\', \'cre_sys_obt_crt\', \'cre_sys_obt_cur\')');


  filterByUserAccessPrivileges($selStmt, 'lp',$vLMSUserId,'','','Program');

  // Set conditions
  $selStmt->condition('lp.id',                      $programId);
  $selStmt->condition('lp.status', 'lrn_lpn_sts_atv','=');
  $selStmt->condition('map.module_id',$modId,'=');

  // Select fields
  $selStmt->addField('lp',	'id',					'id');
  $selStmt->addField('lp',	'title',				'title');
  $selStmt->addField('lp',	'short_desc', 			'description');
  $selStmt->addField('lp',	'short_desc',			'short_desc');
  $selStmt->addField('lp',	'code',					'code');
  $selStmt->addField('lp',	'end_date',				'end_date');
  $selStmt->addField('lp',	'expires_in_value',		'expires_in_value');
  $selStmt->addField('lp',  'expires_in_unit',    'expires_in_unit');
  $selStmt->addField('lp',  'custom0',            	'view_count');
  $selStmt->addField('lp',  'object_type',          'object_type');
  $selStmt->addField('lp', 'additional_info', 			'additional_info');
  $selStmt->addField('lp', 'addn_catalog_show', 			'addn_catalog_show');
  $selStmt->addField('lp', 'addn_notification_show', 			'addn_notification_show');
  $selStmt->addField('lp',	'lang_code',				'lang_code'); // added for new detail pages

  $selStmt->addField('lp', 'additional_info', 			'additional_info');
  $selStmt->addField('lp', 'addn_catalog_show', 			'addn_catalog_show');
  $selStmt->addField('lp', 'addn_notification_show', 			'addn_notification_show');

/*  $selStmt->addField('map', 'course_id', 			'course_id');
  $selStmt->addField('map', 'is_required', 			'is_required');
  $selStmt->addField('crs', 'title', 				'crs_title');
  $selStmt->addField('crs', 'code', 				'crs_code');
  $selStmt->addField('crs', 'description', 			'crs_description'); */
  $selStmt->addField('pro',	'code',					'pro_code');
  $selStmt->addField('pro',	'name',					'pro_type');
  $selStmt->addExpression('if(usrmap.mro is not null, usrmap.mro, group_map.mro )', 'mro_id');
  $selStmt->addField('nl','node_id', 'node_id');
  $selStmt->addExpression("if(if(usrmap.mro is not null, usrmap.mro, group_map.mro ) = 'cre_sys_inv_opt',3,if(if(usrmap.mro is not null, usrmap.mro, group_map.mro ) = 'cre_sys_inv_man',1,2))", "ascen");
  $selStmt->orderBy("ascen");
  expDebug::dPrintDBAPI('lp details $selStmt', $selStmt);

  // Execute query
  $learningDetails = $selStmt->execute()->fetchAll();
  expDebug::dPrint('$learningDetails = ' . print_r($learningDetails, true));

  if (count($learningDetails) > 0) {
    // Call the new function
    $learningDetails[0]->courselist = fetch_learning_courses_list($programId,$modId);
    $learningDetails[0]->description = trim($learningDetails[0]->description);

    // Prepare the expires in value string that is to be displayed
    if ($learningDetails[0]->object_type == 'cre_sys_obt_crt') {
      $learningDetails[0]->expires_in_value_str =
        prepareCrtExpiresInDisplayString($learningDetails[0]->expires_in_value, $learningDetails[0]->expires_in_unit);
    }
  }


  $prereqequv = getPrerequisiteTPDetails($programId);
  $prereqArr = Array();
  foreach ($prereqequv as $row){
    $prereqArr[] = $row;
  }
 /* if(!empty($vLMSUserId))
	$exmpId = getRegisteredEnrollmentExmeptedorNot('tp',$vLMSUserId,$programId);
  if(!empty($exmpId))
  	$learningDetails[0]->exemptedid = 1; */
  
  $learningDetails[0]->prerequisites = $prereqArr;

  expDebug::dPrint('$learningDetails = ' . print_r($learningDetails, true), 4);
  return $learningDetails;

  }
  catch (Exception $ex) {
    watchdog_exception('getLearningDetails', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Get the list of courses available from the learning plan details
 * SQL Query SELECT
 * lp.code,
 * lp.title,
 * lp.description,
 * map.course_id,
 * crs.title
 * FROM
 * slt_program lp,
 * slt_module_crs_mapping map,
 * slt_course_template crs
 * WHERE
 * lp.id=map.program_id AND
 * map.course_id=crs.id AND
 * lp.id=1
*/

function fetch_learning_courses_list($programId,$module_id, $plainQuery = 0){
  try{
  	expDebug::dPrint("fetch crs list ".$module_id,5);
  // Select slt_program
  $select = db_select('slt_program', 'lp');
  $select->innerJoin('slt_module_crs_mapping', 'map', 'lp.id = map.program_id');
  $select->innerJoin('slt_course_template', 'crs', 'map.course_id = crs.id');
  $select->innerJoin('slt_module', 'module', 'map.module_id = module.id');
  $select->innerJoin('slt_node_learning_activity', 'nod', 'nod.entity_id= crs.id AND nod.entity_type = \'cre_sys_obt_crs\'');
  $select->leftJoin('slt_module_groups', 'grps', 'grps.module_id = module.id AND map.group_id = grps.id');

  //Select fields/expression values to be fetched from the database.
  $select->addField('lp', 'id', 		'program_id');
  $select->addField('map', 'course_id', 		'course_id');
  $select->addField('nod', 'node_id', 		'node_id');
   $select->addField('map', 'sequence', 		'sequence_course');
  $select->addField('map', 'is_required', 		'is_required');
  $select->addField('crs', 'title', 			'crs_title');
  $select->addField('crs', 'code', 				'crs_code');
  $select->addField('crs', 'short_description', 		'crs_description');
  $select->addField('module', 'id', 			'module_id');
  $select->addField('module', 'sequence', 			'sequence_module');
  $select->addField('module', 'title', 			'module_title');
  $select->addField('module', 'code', 			'modle_code');
  //$select->addField('module', 'short_desc', 	'module_description');
  $select->addField('grps', 'id', 			'group_id');
  $select->addField('grps', 'title', 			'group_title');
  $select->addField('grps', 'sequence', 			'group_seq');
  $select->condition('lp.id', $programId, '=');
  if(!empty($module_id))
  	$select->condition('module.id', $module_id, '=');
  $select->orderBy('sequence_module');
  $select->orderBy('group_seq');
  $select->orderBy('module_id');
  $select->orderBy('sequence_course');
  
  if ($plainQuery == 1) {
  	return $select;
  }
  //$select->orderBy('course_id');
  $objecttype = getTrainingProgramDetails($programId, array('object_type'));
  expDebug::DPrint("object type >>>> ". print_r($objecttype['object_type'],true));

  /*
  if($objecttype['object_type'] == 'cre_sys_obt_crt'){
  	  $select1 = db_select('slt_program', 'lp');
	  $select1->innerJoin('slt_re_certify', 'map', 'lp.id = map.program_id');
	  $select1->innerJoin('slt_course_template', 'crs', 'map.course_id = crs.id');
	  $select1->innerJoin('slt_module', 'module', 'map.module_id = module.id');
	
	  //Select fields/expression values to be fetched from the database.
	  $select1->addField('map', 'course_id', 		'course_id');
	   $select1->addField('map', 'sequence', 		'sequence_course');
	  $select1->addField('map', 'is_required', 		'is_required');
	  $select1->addField('crs', 'title', 			'crs_title');
	  $select1->addField('crs', 'code', 				'crs_code');
	  $select1->addField('crs', 'short_description', 		'crs_description');
	  $select1->addField('module', 'id', 			'module_id');
	  $select1->addField('module', 'sequence', 			'sequence_module');
	  $select1->addField('module', 'title', 			'module_title');
	  $select1->addField('module', 'code', 			'modle_code');
	  //$select->addField('module', 'short_desc', 	'module_description');
	   $select1->addField('map', 'group_title','group_title');
  	  $select1->addField('map', 'sequence','group_seq');
	  $select1->condition('lp.id', $programId, '=');
	  $select1->orderBy('sequence_module');
	  $select1->orderBy('module_id');
	  $select1->orderBy('sequence_course');
	  $select1->orderBy('course_id');
	  expDebug::dPrintDBAPI(' SQL fetch_learning_courses_list >>> 222 = ', $select1);
	  $resultQuery=$select->union($select1,'UNION ALL');
	  
  }else{ */
  	  $resultQuery=$select;
  /* } */

  expDebug::dPrintDBAPI(' SQL fetch_learning_courses_list = ', $resultQuery);

  //$resultQuery->orderBy('module_id');
  //$resultQuery->orderBy('sequence_course');
  //$resultQuery->orderBy('course_id');

  $retResult = $resultQuery->execute()->fetchAll();
  expDebug::dPrint(' $retResult = ' . print_r($retResult, true) , 3);

  $resCrsDetials =array();
  $v = 0;
  foreach($retResult as $resCrsDetials) {  //Added by BalaG(19/12/2011) -- For Attachments Display

	    $courseId   = $resCrsDetials->course_id;
	    $resCrsDetials->attachments = getAttachmentsInfo($courseId,'Course');
	    $resCrsDetials->crs_description = trim($resCrsDetials->crs_description);
	    $resCrsDetials1[$v]   = $resCrsDetials;
	    $v++;

  }

  // Return the result
  expDebug::dPrint(' $resCrsDetials1 = ' . print_r($resCrsDetials1, true) , 4);
  return $resCrsDetials1;
  }catch (Exception $ex) {
    watchdog_exception('fetch_learning_courses_list', $ex);
    expertusErrorThrow($ex);
  }
}



/**
 * Get learning plan detail action items
 * SQL query statement
 * SELECT
 * lp.id AS id,
 * lp.price AS price,
 * lp.object_type AS objecttype,
 * lp.currency_type AS currencytype,
 * FROM
 * slt_program lp
 * WHERE  lp.id = 1
 *
 */

function fetchLearningActionItem($programId){
  try{
  // Select table
  $UserDefaultCurrencyArr = getUserDefaultCurrency();
  $UserPrefCurrCode = $UserDefaultCurrencyArr['currency_code'];
  $UserPrefCurrSym = $UserDefaultCurrencyArr['currency_sym'];

  $selStmt = db_select('slt_program', 'lp');
  $selStmt->leftJoin('slt_node_learning_activity', 'nl',	'nl.entity_id = lp.id and nl.entity_type in (\'cre_sys_obt_trn\',\'cre_sys_obt_cur\',\'cre_sys_obt_crt\')');
  $selStmt->leftJoin('slt_module_crs_mapping', 'map',	'map.program_id=lp.id');
  $selStmt->condition('lp.id',        $programId);

  // Select fields
  $selStmt->addField('lp',	'id',					'id');
  $selStmt->addExpression('slf_convert_price(lp.price,lp.currency_type,\''.$UserPrefCurrCode.'\')','prm_price');
  //$selStmt->addField('lp',	'price', 				'prm_price');
  $selStmt->addField('lp',	'object_type',			'object_type');
  $selStmt->addExpression('\''.$UserPrefCurrSym.'\'',		'prm_currency_type');
  //$selStmt->addField('spl',	'attr2',		'prm_currency_type');
  $selStmt->addField('nl','node_id','node_id');

  // Execute query
  $result = $selStmt->execute();
  $actionDetails = $result->fetchObject();
  $actionDetails->cls_id = $actionDetails->id;
  $actionDetails->widgetId = 'learning-plan-details-data';

  if($actionDetails->prm_price == null || $actionDetails->prm_price == '' || $actionDetails->prm_price == 0) {
    $actionDetails->prm_price = '0.00';
  }
  expDebug::dPrintDBAPI(' fetch_learning_courses_list() SQL:  = ', $selStmt);
  return $actionDetails;

//  Return the result
    expDebug::dPrint(' $actionDetails : = ' . print_r($actionDetails, true) , 3);
    expDebug::dPrintDBAPI(' fetch_learning_courses_list() SQL:  = ', $selStmt);
  expDebug::dPrint('result_response after json encoding = '.print_r(drupal_json_encode($actionDetails), true) , 3);
  }catch (Exception $ex) {
    watchdog_exception('fetchLearningActionItem', $ex);
    expertusErrorThrow($ex);
  }
}


function getPrerequisiteTPDetails($tpId) {
  try{
  // Select table
  $selectStmt = db_select('slt_common_mapping', 'cmp');
  $selectStmt->leftJoin('slt_program','prm','cmp.id2 = prm.id');
  $selectStmt->leftJoin('slt_course_template','crs','cmp.id2 = crs.id');
  $selectStmt->leftJoin('slt_profile_list_items','sp','sp.code = prm.object_type');

  // Set conditions
  $selectStmt->condition('cmp.id1', $tpId);
  $selectStmt->condition('cmp.object_type', 'cre_sys_obt_trp');

  $selectStmt->addExpression('if(cmp.prereq_type = \'cre_sys_pre_crs\', crs.id, prm.id)', 'pe_id');
  $selectStmt->addExpression('if(cmp.prereq_type = \'cre_sys_pre_crs\', crs.title, prm.title)', 'pe_title');
  $selectStmt->addExpression('if(cmp.prereq_type = \'cre_sys_pre_crs\', crs.code, prm.code)', 'pe_code');
  $selectStmt->addExpression('if(cmp.prereq_type = \'cre_sys_pre_crs\', \'Course\', sp.name)', 'pe_object_type');

  // Execute query
  $result = $selectStmt->execute()->fetchAll();

   //expDebug::dPrint(' BALAJIIIIIIIIIIII getPrerequisiteTPDetails : = ' . print_r($selectStmt, true) , 4);

  expDebug::dPrintDBAPI("BALAJIIIIIIIIIIII getPrerequisiteTPDetails-----  : ",$selectStmt);

  return $result;

//  Return the result


  }catch (Exception $ex) {
    watchdog_exception('getPrerequisiteTPDetails', $ex);
    expertusErrorThrow($ex);
  }
}

function fetch_detailed_courses_list($programId = '', $fromTp = false) {
	expDebug::dPrint('program id received: ' . $programId);
	$vLMSUserId = getSltpersonUserId();
	$page = $_GET['page']; // get the requested page
    $limit = $_GET['rows'];
	$total_pages = 0;
	$module_seq = getModuleId($vLMSUserId,$programId);
	$recertifyValue = ($module_seq[0]->recertify_path).'-R';
	$NextmoduleAvailable =getCourseMappingCount($programId,$recertifyValue);
	expDebug::dPrint("module seq ".print_r($module_seq,1),5);
	$module_path = 1;
	$allow_recertify = 0;
	expDebug::dPrint("count >> ". count($module_seq). "object type >> ".$module_seq[0]->object_type , 5);
	
	if(count($module_seq)> 0 && ($module_seq[0]->object_type == 'cre_sys_obt_crt')) {
		if(($module_seq[0]->overall_status == 'lrn_tpm_ovr_cmp'  && $NextmoduleAvailable) || $module_seq[0]->overall_status == 'lrn_tpm_ovr_inc' )
			$allow_recertify = getRecertifyBeforeCertifyExpired($module_seq[0]->comp_date, $module_seq[0]->expires_in_value ,$module_seq[0]->expires_in_unit);
		if(($module_seq[0]->overall_status =='lrn_tpm_ovr_exp'  && $NextmoduleAvailable) || $allow_recertify == 1 )
			$module_path = $module_seq[0]->recertify_path+1;
		else
			$module_path = $module_seq[0]->recertify_path;
	}
	
	//$module_path = !empty($module_seq[0]->recertify_path) ? $module_seq[0]->recertify_path : 1;
	expDebug::dPrint("module path ".$module_path,5);
	
	$module_id = getModuleIdFromObjectId($programId,$module_path);
	
	if(count($module_id)>0){
		$modId = $module_id[0]->module_id;
	}else{
		$module_id = getModuleIdFromObjectId($programId,($module_path-1));
		$modId = $module_id[0]->module_id;
	}
	
	$select = fetch_learning_courses_list($programId, $modId, 1);
	
	// Set order of fetched records
	//$select->orderBy($sidx,$sord);
	
	expDebug::dPrintDBAPI("fetch_detailed_courses_list sql-----  : ",$select);
	$coursesListCount = $select->execute()->fetchCol();
	
	$totalRecords 		= count($coursesListCount);
	if ( $totalRecords > 0 ) {
		$total_pages = ceil($totalRecords/$limit);
	}
	$start = calculate_offset($page,$limit);
	
	$select->range($start, $limit);
	expDebug::dPrintDBAPI("fetch_detailed_courses_list select query : ", $select);
	
	$coursesListRes = $select->execute()->fetchAll();
	expDebug::dPrint(print_r($coursesListRes, true) , 3);
	
	$result_response->page    = $page;
	$result_response->total   = $total_pages;
	$result_response->records = count($coursesListCount);
	
	$courselist 	= array();
	
	$i=0;
	$userId = $vLMSUserId;
	$grpExists = array();
	foreach ($coursesListRes as $record) {
		
		$crsDetail 								= new StdClass();
		$crsDetail->course_id 					= $record->course_id;
		$crsDetail->program_id 					= $record->program_id;
		$crsDetail->entity_id					= $record->course_id;
		$childNodeIds 							= getNodeActivityMapping($crsDetail->course_id);
		$crsDetail->node_id						= implode(",",$childNodeIds);
		//$crsDetail->node_id 					= $record->node_id;
		$crsDetail->delivery_type_code			= 'cre_sys_obt_crs';
			
		$crsDetail->data_icon 					= 'crs-icon';
		$crsDetail->icon_class					=  getTypeImageClass($crsDetail->delivery_type_code);
		$crsDetail->icon_title					=  t('Course');
		$crsDetail->data_title 					= sanitize_data($record->crs_title);
		$crsDetail->data_code 					= $record->crs_code;
		$crsDetail->cls_class_code 				= 'COURSE-DETAIL-COURSE-CODE';
		$crsDetail->data_desc					= $record->crs_description;
		$crsDetail->data_trunk8					= 'lmt-lrp-crs';
		$crsDetail->training_type				= ($record->is_required == 'Y') ? t('Mandatory') : t('Optional');
			
		$crsDetail->start_rate_setting			= array(
													'content_type' => 'Class',
													'content_id' => $childNodeIds ,
													'stars' => 5,
													'autosubmit' => TRUE,
													'allow_clear' => FALSE,
													'required' => FALSE,
													'tag' => 'vote',
													'style' => 'average',
													'text' => 'average',
													'startype' => 'lrn_cls_dty_ilt',
													'enable_rate' => 'disable',
													'catalog_crs' => 'Course'
												);
// 		$crsDetail->qtipIdInit        			= $crsDetail->course_id.'_'.$crsDetail->delivery_type_code;
// 		$crsDetail->qtipOptAccessObj      		= "{'entityId':".$crsDetail->course_id.",
// 													'entityType':'cre_sys_obt_cls',
// 													'popupDispId':'widget_share_qtip_visible_disp_$crsDetail->qtipIdInit',
// 													'wid':440,
// 													'heg':'350',
// 													'postype':'middle',
// 													'linkid':'widget-share-$crsDetail->course_id',
// 													'callFrom' : 'detail_page'}";
		// set group title informations
		if(!in_array($record->group_id, $grpExists) && !empty($record->group_title)){
			array_push($grpExists,$record->group_id);
			$crsDetail->group_title  	= $record->group_title;
			$crsDetail->group_id	  	= 'lp_grp_title_' . $record->group_id;
			$crsDetail->show_title  	= true;
		} else {
			$crsDetail->group_title 	= $record->group_title;
			$crsDetail->group_id	  	= 'lp_grp_title_' . $record->group_id;
			$crsDetail->show_title  	= false;
		}	
		$result_response->rows[$i]['id'] = $record->course_id;
		$result_response->rows[$i]['cell'] = theme('content_course_list', array(
				"result" => $crsDetail,
				"context" => ['callfrom' => 'lrnplan_course_list'],
		));
		$result_response->rows[$i]['course'] = $record;
		$i++;
	
	}
	$result_response->group_ids = $grpExists;
	echo drupal_json_encode($result_response);
	
	expDebug::dPrint('fetch_detailed_courses_list = '.print_r(drupal_json_encode($result_response), true) , 4);
}

?>