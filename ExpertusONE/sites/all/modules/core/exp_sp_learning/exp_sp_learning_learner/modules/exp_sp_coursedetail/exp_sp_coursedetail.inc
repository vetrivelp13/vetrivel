<?php
/**
 * Get course details and increment view count
 * Implements stored procedure slp_course_widget_sel(xcourse_id INT)
 *    Stored procedure: slp_course_widget_sel() :
 *    	SELECT
 *    		id ID,
 *    		title Title,
 *    		description Description,
 *    		short_description ShortDescription,
 *    		code 'Code'
 * 	 	FROM
 *    		slt_course_template
 *    	WHERE
 *   		id = xcourse_id
 * 			status = 'lrn_crs_sts_atv';
 *
 * 	  	IF xcourse_id IS NOT NULL  THEN
 *		UPDATE
 *			slt_course_template
 *		SET
 *			custom0 = IFNULL(custom0, 0) + 1
 *		WHERE
 *			id=xcourse_id;
 *	  END IF;
 */
function getCourseDetails($courseId,$callFromApi = '',$callfrom='') {
  try{
    // Select table
    $selectStmt = db_select('slt_course_template', 'crs');
    if($callfrom=='course_page_view'){
    	$selectStmt->leftJoin('slt_course_class','clsRollup','clsRollup.course_id=crs.id AND (clsRollup.status IN  (\'lrn_cls_sts_atv\'))');
    	$selectStmt->leftJoin('slt_group_mapping','group_map','(group_map.parent_id=concat(crs.id, \'-\', IFNULL(clsRollup.id,0)) AND group_map.parent_type = \'learning\') and group_map.group_type = 0 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
    	$selectStmt->addField('clsRollup',	'price','price');
    	$selectStmt->addField('clsRollup',	'currency_type','currency_type');
    	$selectStmt->addField('clsRollup',	'registration_end_on', 	'registration_end_on');
    	$selectStmt->addField('clsRollup',	'custom4', 	'export_compliance');
    	$selectStmt->addField('crs',	'created_on',					'created_on');
    	$selectStmt->addField('crs',	'published_on',				'published_on');
    	$selectStmt->addExpression('if(usrmap.mro is not null, usrmap.mro, group_map.mro )', 'mro_id');
    }
    // Set conditions
    $selectStmt->condition('crs.id',  $courseId);

    // Select fields
    $selectStmt->addField('crs',	'id',					'id');
    $selectStmt->addField('crs',	'title',				'title');
    $selectStmt->addField('crs',	'short_description', 	'description');
    $selectStmt->addField('crs',	'short_description',	'short_description');
    $selectStmt->addField('crs',	'code',					'code');
    $selectStmt->addField('crs',  'custom0',              'view_count');
    $selectStmt->addField('crs',	'is_compliance', 	'is_compliance');
    $selectStmt->condition('crs.status',	'lrn_crs_sts_atv', 	'=');
    if($callFromApi == ''){
    //  filterByUserAccessPrivileges($selectStmt, 'crs','','','cre_sys_obt_crs'); // Ram Issue is Fixed for the Course Detail page
    	$userId  = getIdOfLoggedInUser();
	    $userAccess  = getUserAccessPriviledges($userId);
			$enabledValues = getEnabledAttrForVisibility();
			$closeBracket='';
			$lastid = 1;
			$enabledCount = count($enabledValues);
			
    if($enabledCount > 0){
				$classQuery = ' AND ';
				$classQuery .= '(if(FIND_IN_SET('.$userId.',catacs.added_users)>0 OR usrmap.user_id = '.$userId.',1=1,';
				foreach($enabledValues as $key => $value){
					if($value == 'state')
						$userValue = $userAccess->country.'-'.$userAccess->$value;
					else
						$userValue = $userAccess->$value;
	
					if($value != 'role' && $value != 'hire_date'){
	
						if($value == 'job_role'){
							$resultjobrole = explode(',',$userValue);
							$classQuery .= ' (if(catacs.'.$value.' = \'All\',\''.$userValue .'\'!=\'\', (catacs.'.$value.' is null or catacs.'.$value.'=\'\'';
							
							foreach ($resultjobrole as $job_role){
								expDebug::dPrint('$JOB_ROLE------>'.print_r($job_role,1),4);
								$classQuery .= 'OR (FIND_IN_SET(\''.$job_role.'\',catacs.'.$value.'))';
							}
							$classQuery .= '))) ';
							if($lastid != $enabledCount)
								$classQuery .= ' AND ';
						}else{
	
							$classQuery .= ' (if(catacs.'.$value.' = \'All\',\''.$userValue .'\'!=\'\',(catacs.'.$value.' is null or catacs.'.$value.'=\'\' or (FIND_IN_SET(\''.$userValue.'\',catacs.'.$value.'))))) ';
							
							if($lastid != $enabledCount)
								$classQuery .= ' AND ';
						}
					}
	
	
					if($value == 'role'){
						$isManager = empty($userAccess->is_manager) ? 'N' : $userAccess->is_manager;
						$classQuery .= '(CASE WHEN (catacs.is_manager=\'Y\' AND catacs.is_instructor=\'Y\') THEN (\''.$userAccess->is_manager.'\' = catacs.is_manager OR \''.$userAccess->is_instructor.'\' = catacs.is_instructor) 
						WHEN (catacs.is_manager=\'Y\' AND catacs.is_instructor=\'N\') THEN (\''.$userAccess->is_manager.'\' = catacs.is_manager) 
					  WHEN (catacs.is_manager=\'N\' AND catacs.is_instructor=\'Y\') THEN (\''.$userAccess->is_instructor.'\' = catacs.is_instructor) ELSE 1=1 END) ';
						if($lastid != $enabledCount)
							$classQuery .= ' AND ';
					}
				if($value == 'hire_date'){
					$perHireDate = $userAccess->hire_date;
					$classQuery .= '(IF((select att.id from slt_group_attributes att where att.group_id = catacs.id)>0,
					  (select att.id from slt_group_attributes att where att.group_id = catacs.id and (
					  (DATE_FORMAT(\'' .$perHireDate. '\',\'%Y-%m-%d\') >= DATE_FORMAT(att.on_or_after_start_date,\'%Y-%m-%d\') ) or
					  (DATE_FORMAT(\'' .$perHireDate. '\',\'%Y-%m-%d\') <= DATE_FORMAT(att.on_or_before_start_date,\'%Y-%m-%d\')) or
					  (DATE_FORMAT(\'' .$perHireDate. '\',\'%Y-%m-%d\') between DATE_FORMAT(att.between_start_date,\'%Y-%m-%d\') AND DATE_FORMAT(att.between_end_date,\'%Y-%m-%d\'))
					  )) >0
					  ,1=1)) ';
					if($lastid != $enabledCount)
						$classQuery .= ' AND ';
				}
					
				    $lastid++;
				}
				$classQuery .= ' and ((catacs.removed_users IS NULL) OR (FIND_IN_SET('.$userId.',catacs.removed_users) <= 0))';
				$classQuery .= 'AND( (group_map.group_type = 0) 
												OR (group_map.group_id IS NULL) 
												AND (SELECT count(1) FROM slt_user_access_mapping usrmap WHERE (usrmap.parent_id=concat(crs.id, \'-\', IFNULL(clsRollup.id,0)) AND usrmap.parent_type = \'learning\') and usrmap.access_type = 0 )=0 )
												AND (IF((catacs.is_admin = 1 AND catacs.userslist IS NOT NULL AND catacs.userslist != \'\'),FIND_IN_SET('.$userId.',catacs.userslist)>0,1=1))))';
			}
			
    	$selectStmt->leftJoin('slt_course_class', 'clsRollup', '(clsRollup.course_id=crs.id AND (clsRollup.status IN  (\'lrn_cls_sts_atv\',\'lrn_cls_sts_dld\')) AND (clsRollup.dedicated_class_flag = \'N\' or clsRollup.dedicated_class_flag is null))');
			$selectStmt->leftJoin('slt_group_mapping','group_map','((group_map.entity_id=crs.id AND group_map.entity_type = \'cre_sys_obt_crs\') OR (group_map.entity_id=clsRollup.id AND group_map.entity_type = \'cre_sys_obt_cls\')) and group_map.group_type = 0 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
			$selectStmt->leftJoin('slt_user_access_mapping','usrmap','(usrmap.parent_id=concat(crs.id, \'-\', IFNULL(clsRollup.id,0)) AND usrmap.parent_type = \'learning\') and usrmap.access_type = 0 and usrmap.user_id = '.$userId.' ');
			$selectStmt->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) '.$classQuery );
		
		
			$selectStmt->where('if(group_map.entity_type = \'cre_sys_obt_crs\',
       catacs.id is not null OR (select count(1) from slt_course_class cs
                                 left join slt_group_mapping gm on cs.id=gm.entity_id and gm.entity_type = \'cre_sys_obt_cls\'  
                                 where cs.course_id=group_map.entity_id and gm.id is null
                                 )> 0,  
       if(group_map.entity_type = \'cre_sys_obt_cls\',
         catacs.id is not null,
         1=1)
                         )');
    }
    if($callFromApi){
        $selectStmt->leftJoin('slt_tag_entity','te','(crs.id=te.entity_id and te.entity_type=\'Course\')');
	$selectStmt->leftJoin('slt_tagdefn','td','te.tagid= td.id');
        $selectStmt->addExpression("group_concat(td.tagname)","tagname");
    }
    // Conditon "OR (select count(1) from slt_group_mapping where entity_id=clsRollup.course_id and entity_type= \'cre_sys_obt_crs\') = 0"
    // removed in the "if(group_map.entity_type = \'cre_sys_obt_cls\" by Vincent on 06 Apr, 2017 - 
    // for in course level registration if the course doesnot have a access and it is a mandatory course
    // in caralog it shows mandatory but while click register the class not shown (in case of multiple classes under the course)
    
    expDebug::dPrintDBAPI(' $selectStmt OBJ = ' , $selectStmt);

    // Execute query
    $result = $selectStmt->execute();
    if($callFromApi){
      return $result->fetchObject();
    }
    expDebug::dPrint(' rowCount = ' . $result->rowCount() , 3);
    if ($result->rowCount() <= 0) {
       $courseDetails->code = '-';
       $courseDetails->description = '-';
       $courseDetails->id = '-';
       $courseDetails->view_count = 0;
    }else{
      $courseDetails = $result->fetchObject();
    }
    $courseDetails->description = trim($courseDetails->description);
    expDebug::dPrint(' $courseDetails = ' . print_r($courseDetails, true) , 4);
    $txn = db_transaction();
    try{
      // Select table
      $updateStmt = db_update('slt_course_template');

      // Set conditions
      $updateStmt->condition('id', $courseId);

      // Select fields
      $updateStmt->fields(array('custom0' => ($courseDetails->view_count + 1)));

      expDebug::dPrintDBAPI(' $updateStmt = ' , $updateStmt);

      // Execute query
      $updateStmt->execute();
    }catch (Exception $ex) {
      $txn->rollback();
      unset($txn);
      watchdog_exception('getCourseDetails', $ex);
      expertusErrorThrow($ex);
    }
    // Commit the transaction
    unset($txn);
    expDebug::dPrint("Return getCourseDetails ". print_r($courseDetails,true) , 4);
    // Return course details
    return $courseDetails;
  }catch (Exception $ex) {
    watchdog_exception('getCourseDetails', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Implements stored procedure slp_get_entity_proflie_sel(IN xentityid VARCHAR(500), IN xentitytype VARCHAR(255))
 * DECLARE xqry TEXT DEFAULT '';
 *
 *  SET xqry = CONCAT('select plst.id ID,plst.code Code,plst.name Name from slt_profile_list_items plst ');
 *  SET xqry = CONCAT(xqry,'where FIND_IN_SET(id,(select CONCAT_WS('','',IFNULL(col1,''''),IFNULL(col2,''''),IFNULL(col3,''''),IFNULL(col4,''''),IFNULL(col5,''''),IFNULL(col6,''''),IFNULL(col7,''''),IFNULL(col8,''''),IFNULL(col9,''''),IFNULL(col10,''''),IFNULL(col11,''''),IFNULL(col12,''''),IFNULL(col13,''''),IFNULL(col14,''''),IFNULL(col15,''''),IFNULL(col16,''''),IFNULL(col17,''''),IFNULL(col18,''''),IFNULL(col19,''''),IFNULL(col20,''''),IFNULL(col21,''''),IFNULL(col22,''''),IFNULL(col23,''''),IFNULL(col24,''''),IFNULL(col25,'''')) cols from slt_entity_profile_mapping  ');
 *  SET xqry = CONCAT(xqry,'where entity_id=','''',xentityid,'''',' and entity_type = ','''',xentitytype,'''',' ))');
 *
 *  SET @xsql = xqry;
 *
 *  PREPARE stmt FROM @xsql;
 *
 *  EXECUTE stmt;
 *  DEALLOCATE PREPARE stmt;
 */
function getEntityProfile($xentityId, $xentityType) {
  /*$dao = new AbstractDAO();
   $dao->connect();
   $profileSql        = "CALL slp_get_entity_proflie_sel(" . $xentityId . ", '" . $xentityType . "')";
   //$profileSql        = "CALL slp_get_entity_proflie_sel(24755, '21')";
   expDebug::dPrint("Course Entity profile  : ". $profileSql);
   $qryEntityProf = $dao->query($profileSql);
   $entityProfResOld = $dao->fetchAllResults();
   $dao->closeconnect();
   expDebug::dPrint('exp_sp_coursedetail.inc : getEntityProfileSel() : $entityProfResOld = ' . print_r($entityProfResOld, true)); */

  try{

  // Select table
  $selectStmt1 = db_select('slt_entity_profile_mapping', 'pmap');

  // Set conditions
  $selectStmt1->condition('pmap.entity_id', $xentityId);
  $selectStmt1->condition('pmap.entity_type', $xentityType);
  //$selectStmt1->condition('pmap.entity_id', 24755);
  //$selectStmt1->condition('pmap.entity_type', '21');

  // Select fields
  for ($i=1; $i <= 25; $i++) {
    $selectStmt1->addField('pmap', 'col' . $i, 'col' . $i);
  }

  expDebug::dPrintDBAPI(' $selectStmt1 = ' , $selectStmt1);

  // Execute query
  $result1 = $selectStmt1->execute();
  expDebug::dPrint(' $result1 = ' . print_r($result1, true) , 3);

  $entityProfRes = array();
  if ($result1->rowCount() <= 0) {
    expDebug::dPrint(' $result1 is empty : $entityProfRes = ' . print_r($entityProfRes, true) , 4);
    return $entityProfRes; // Return empty array
  }

  $entityProfRes = array();
  foreach ($result1 as $pmap_row) {

    //Prepare an array of column values
    $rowIdList = array();
    for ($i=1; $i <= 25; $i++) {
      $colName = 'col' . $i;
      if ($pmap_row->$colName != null) {
        $colValues = explode(',' , $pmap_row->$colName);
        foreach ($colValues as $value) {
          $rowIdList[] = $value;
        }
      }
    }

    // Select table
    $selectStmt2 = db_select('slt_profile_list_items', 'plst');

    // Set conditions
    $selectStmt2->condition('plst.id', $rowIdList, 'IN');

    // Select fields
    $selectStmt2->addField('plst', 'id', 'id');
    $selectStmt2->addField('plst', 'code', 'code');
    $selectStmt2->addField('plst', 'name', 'name');

    expDebug::dPrintDBAPI(' $selectStmt2 = ' , $selectStmt2);

    $result2 = $selectStmt2->execute();

    expDebug::dPrint(' $result2 = ' . print_r($result2, true) , 3);

    foreach ($result2 as $plstrow) {
      $entityProfRes[] = $plstrow;
    }
  }

  expDebug::dPrint(' $entityProfRes = ' . print_r($entityProfRes, true) , 3);
  return $entityProfRes;

  }catch (Exception $ex) {
    watchdog_exception('getEntityProfile', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * getCourseProfileAsText() returns comma separated list as string of the profile names (categories) of
 * the profiles applied to the course.
 */
function getCourseProfileAsText($courseId) {
  try{
  $entityProfRes = getEntityProfile($courseId, '02');
  $profCategory = array();
  if(count($entityProfRes) > 0) {
    foreach ($entityProfRes as $id => $row) {
      $profCategory[] = $row->name;
    }
    $profCategoryTxt = implode("," , $profCategory);
  }
  else {
    $profCategoryTxt = t('None');
  }

  expDebug::dPrint(' $profCategoryTxt = ' . print_r($profCategoryTxt, true) , 4);
  return $profCategoryTxt;
  }catch (Exception $ex) {
    watchdog_exception('getCourseProfileAsText', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Implements to get the prerequisite course information
 * SELECT
 * 		crs.id
 * 		crs.title
 * 		crs.code
 * FROM
 * 		slt_common_mapping cmp
 *		LEFT JOIN slt_course_template crs ON crs.id=cmp.id2
 * WHERE
 * 		cmp.id1=34
 *
 */
function getPrerequisiteDetails($courseId,$type) {
  try{
  // Select table
  $selectStmt = db_select('slt_common_mapping', 'map');

  $selectStmt->leftJoin('slt_course_template', 'crs', 'map.id2 = crs.id');

  // Set conditions
  $selectStmt->condition('map.id1', $courseId);
  $selectStmt->condition('map.type', $type);
  //  $selectStmt->condition('course_cls.status', 'lrn_cls_sts_atv');

  // Select fields
  //$selectStmt->distinct();
  $selectStmt->addField('crs', 'id', 'course_id'); // This is course id.
  $selectStmt->addField('crs', 'title', 'name');
  $selectStmt->addField('crs', 'code', 'code');   // Note that this is the class title and not course title.
	$selectStmt->condition('map.object_type', 'cre_sys_obt_crs');
  // Execute query
  $result = $selectStmt->execute()->fetchAll();

  return $result;
//  Return the result
  expDebug::dPrint(' getPrerequisiteDetails : = ' . print_r($result, true) , 3);
  expDebug::dPrint(' getPrerequisiteDetails() SQL:  = '. print_r($result->__toString(), true) , 3);
  expDebug::dPrint('result_response after json encoding = '.print_r(drupal_json_encode($result), true) , 3);
  }catch (Exception $ex) {
    watchdog_exception('getPrerequisiteDetails', $ex);
    expertusErrorThrow($ex);
  }
}


/**
 * Fetch course detail info to show on course detail page.
 */
function getCourse_DetailsInfo($nodeId) {
  try{
  $ret_array = array();
  if(!empty($nodeId)) {

    // Get course id from node id
    $courseId = getEntityIdFromNodeId($nodeId);
    // Get course details
    $ret_array['catalog'] = getCourseDetails($courseId);


    // Get course profile
    $ret_array['entityProfRes'] = getCourseProfileAsText($courseId);

    // Get pre-requisites
    $ret_array['preRequiste'] = getPrerequisiteDetails($courseId,5);
    expDebug::dPrint('preRequiste ' . print_r($ret_array['preRequiste'], true) , 4);

    //For getting Equivalence Coursees
    $ret_array['Equivalence'] = getPrerequisiteDetails($courseId,6);
    expDebug::dPrint('Equivalence ' . print_r($ret_array['Equivalence'], true) , 4);

  }
  expDebug::dPrint(' $ret_array = ' . print_r($ret_array, true) , 4);
  return $ret_array;
  }catch (Exception $ex) {
    watchdog_exception('getCourse_DetailsInfo', $ex);
    expertusErrorThrow($ex);
  }
}

function getDiscussionForum() {
	try{
  return "Under Construction";
  }catch (Exception $ex) {
  	watchdog_exception('getDiscussionForum', $ex);
  	expertusErrorThrow($ex);
  }
}

function getBlogsDetail() {
	try{
  return "Under Construction";
  }catch (Exception $ex) {
  	watchdog_exception('getBlogsDetail', $ex);
  	expertusErrorThrow($ex);
  }
}

function getRelatedCourse() {
	try{
  return "Under Construction";
  }catch (Exception $ex) {
  	watchdog_exception('getRelatedCourse', $ex);
  	expertusErrorThrow($ex);
  }
}

function getRatingReview() {
	try{
  return "Under Construction";
  }catch (Exception $ex) {
  	watchdog_exception('getRatingReview', $ex);
  	expertusErrorThrow($ex);
  }
}

function getCategoryRelated() {
	try{
  return "Under Construction";
  }catch (Exception $ex) {
  	watchdog_exception('getCategoryRelated', $ex);
  	expertusErrorThrow($ex);
  }
}

function fetch_classes_list_view($courseId,$callFrom,$isFromTP=FALSE) {
	try{
		$timeZone = date_default_timezone(false);
		$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
		expDebug::dPrint('values for the access privileges fff fetch_classes_list_view'.$callFrom);
		global $user;
		$vLMSUserId = ($user->uid > 0)?getSltpersonUserId():'';
		//$page = $_GET['page']; // get the requested page
		//$limit = $_GET['rows']; // get how many rows we want to have into the grid
		$sidx = addslashes($_GET['sidx']); // get index row - i.e. user click to sort
		$sord = $_GET['sord']; // get the direction
		if(!$sidx) $sidx ='name';
		// $classsrQuery = 'if((select count(1) from slt_group_mapping grp_map where grp_map.entity_id = class.id AND group_map.entity_type = \'cre_sys_obt_cls\')>0,(group_map.entity_id=class.id
		// AND group_map.entity_type = \'cre_sys_obt_cls\'),(group_map.entity_id=class.course_id AND group_map.entity_type = \'cre_sys_obt_crs\'))';
		// Select the tables to fetch data from
		$UserDefaultCurrencyArr = getUserDefaultCurrency();
		$UserPrefCurrCode = $UserDefaultCurrencyArr['currency_code'];
		$UserPrefCurrSym = $UserDefaultCurrencyArr['currency_sym'];

		$classesListSql = db_select('slt_course_class','class');
		$classesListSql->leftJoin('slt_course_template','crs','class.course_id = crs.id');
		//$classesListSql->leftjoin('slt_entity_profile_mapping', 'sepm', 'sepm.entity_id=class.id AND sepm.entity_type=27');
		//$classesListSql->leftJoin('slt_mro_mapping', 'mro', 'mro.entity_id=class.id AND mro.job_role_id = (SELECT job_role FROM slt_person WHERE id=\''.$vLMSUserId.'\') AND mro.entity_type = \'cre_sys_obt_cls\'');
		//$classesListSql->leftjoin('slt_catalog_access', 'catacs', 'catacs.entity_id=class.id AND catacs.entity_type = \'cre_sys_obt_cls\' ');
		// $classesListSql->leftJoin('slt_group_mapping', 'group_map', '('.$classsrQuery.') AND  group_map.group_type = 0');
		// $classesListSql->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');
		//$classesListSql->leftJoin('slt_course_class_session','ses','ses.class_id=class.id AND ses.course_id=class.course_id');
		$classesListSql->leftjoin('slt_course_class_session', 'ses', 'class.id=ses.class_id and ses.id=(SELECT id FROM  slt_course_class_session WHERE class_id=ses.class_id ORDER BY start_date asc, start_time asc LIMIT 1)');
		$classesListSql->leftjoin('slt_facility', 'splt_fac', 'splt_fac.id=ses.facility_id');
		$classesListSql->leftJoin('slt_location','loc','class.location_id = loc.id');
		$classesListSql->leftJoin('slt_profile_list_items','pfl','ses.timezone = pfl.code');
		$classesListSql->leftJoin('slt_node_learning_activity','nla','nla.entity_id = class.id');
		//$classesListSql->leftjoin('slt_course_content_mapper', 'content', ' class.id = content.class_id AND crs.id = content.course_id');
		$classesListSql->leftjoin('slt_course_content_mapper', 'content', '  content.id=(SELECT id FROM slt_course_content_mapper cont WHERE class.id = cont.class_id AND crs.id = cont.course_id LIMIT 1 )');

		// Select the fields to fetch
		$classesListSql->addField('class','id','id');
		$classesListSql->addField('crs','id','crs_id');
		$classesListSql->addField('nla','node_id','node_id');
		$classesListSql->addField('class','id','cls_id');
		$classesListSql->addField('class','code','code');
		$classesListSql->addField('class','lang_code','lang');
		$classesListSql->addField('class','title','name');
		$classesListSql->addField('class','short_description','description');
		$classesListSql->addField('class','delivery_type','delivery_type_code');
		$classesListSql->addField('class','delivery_type','deliverytype');
		$classesListSql->addField('loc','name','locationname');
		$classesListSql->addExpression('slf_convert_price(class.price,class.currency_type,\''.$UserPrefCurrCode.'\')','price');
		$classesListSql->addExpression('\''.$UserPrefCurrSym.'\'','currency_symbol');
		//$classesListSql->addField('class','price','price');
		$classesListSql->addField('class','custom4','export_compliance');
		$classesListSql->addField('class','currency_type','currency_type');
		$classesListSql->addField('class','custom4','currencyprefixsymbol');
		$classesListSql->addField('class','custom4','currencypostfixsymbol');
		$classesListSql->addField('class','scheduled_duration','duration');
		$classesListSql->addField('class','max_seats','maxcapacity');
		$classesListSql->addField('class','waitlist_count','avaliableseatcount');
		$classesListSql->addField('class','valid_to','enrollmentvalidity');
		$classesListSql->addField('ses','id','sessionid');
		$classesListSql->addField('ses','class_id','sessionclassid');
		$classesListSql->addField('ses','start_date','startdate');
		$classesListSql->addField('ses','start_time','session_start_time');
		$classesListSql->addField('ses','end_time','session_end_time');
		$classesListSql->addField('ses','title','session_title');
		$classesListSql->addField('loc','name', 'name');
		$classesListSql->addField('loc','addr1', 'contact_addr1');
		$classesListSql->addField('loc','addr2', 'contact_addr2');
		//$classesListSql->addField('loc','country', 'contact_country');
		//$classesListSql->addField('loc','state', 'contact_state');
		$classesListSql->addField('loc','city', 'contact_city');
		$classesListSql->addField('loc','zipcode', 'contact_zipcode');
		$classesListSql->addExpression('(select country_name from slt_country where country_code = loc.country)', 'contact_country');
		$classesListSql->addExpression('(select state_name from slt_state where state_code = loc.state and country_code =  loc.country)', 'contact_state');

		$classesListSql->addField('ses','end_date','enddate');
		$classesListSql->addField('ses','timezone','timezone');
		$classesListSql->addField('pfl','attr2','session_timezone');

		$classesListSql->addField('class','registration_end_on', 'registration_end_on');
		$classesListSql->addExpression("CONCAT( DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.end_time,':00')", 'session_end_datetime');
		
		if($callFrom != 'admin') {
		    $classesListSql->addExpression('if(usrmap.mro is not null, usrmap.mro, group_map.mro )', 'mro_id');
		    $classesListSql->addExpression("if(if(usrmap.mro is not null, usrmap.mro, group_map.mro ) = 'cre_sys_inv_opt',3,if(if(usrmap.mro is not null, usrmap.mro, group_map.mro ) = 'cre_sys_inv_man',1,2))", "ascen");
		    $classesListSql->orderBy("ascen");
		}

		// Set the conditions
		// below line commented for Mantis - SQL Injection - #31297
		//$classesListSql->condition('class.course_id',$courseId);
		$classesListSql->condition('class.status','lrn_cls_sts_del','!=');
		//$classesListSql->condition('crs.status','lrn_crs_sts_atv');
		$classesListSql->condition('nla.entity_type','cre_sys_obt_cls');
		if($isFromTP==FALSE) // fix for issue #0025076
			$classesListSql->where('(class.dedicated_class_flag = \'N\' or class.dedicated_class_flag is null) AND class.course_id=:courseid',array(':courseid'=>$courseId));
		else
			$classesListSql->where('class.course_id=:courseid',array(':courseid'=>$courseId));

		/*  $classesListSql->condition(db_and()->
		 condition(db_or()->
		 		condition(db_and()->
		 				condition('class.delivery_type', 'lrn_cls_dty_ilt', '=')->
		 				condition('ses.start_date', now(), '>='))->
		 		condition(db_and()->
		 				condition('class.delivery_type', 'lrn_cls_dty_vcl', '=')->
		 				condition('ses.start_date', now(), '>='))->

		 		condition(db_and()->
		 				condition('class.delivery_type', 'lrn_cls_dty_wbt', '=')->
		 				where('content.content_id IS NOT NULL'))->
		 		//condition('class.delivery_type', 'lrn_cls_dty_wbt', '=')->
		 		condition('class.delivery_type', 'lrn_cls_dty_vod', '=')
		 ));*/
		
		$classesListSql->groupBy("class.id");
		expDebug::dPrintDBAPI('values for the access privileges fetch_classes_list_view',$classesListSql);
		// Further filter by role
		//addFilterForLoggedInUserRole($classesListSql, 'sepm');
		//filterByUserAccessPrivileges($classesListSql, 'catacs','','group_map');
        if($callFrom == 'admin')
            filterByUserAccessPrivileges($classesListSql, 'class','',1,'cre_sys_obt_cls','Class','class');
        else
            filterByUserAccessPrivileges($classesListSql, 'class','','','cre_sys_obt_cls','Class','class');

		// Group By
		//$classesListSql->groupBy('class.id');

		// Set order of fetched records
		$classesListSql->orderBy($sidx,$sord);

		expDebug::dPrintDBAPI("fetch_classes_list_view() : ", $classesListSql);

		$classesListRes = $classesListSql->execute()->fetchAll();
		expDebug::dPrint(print_r($classesListRes, true) , 3);

		$result_response->page    = $page;
		$result_response->total   = $total_pages;
		$result_response->records = count($classesListRes);

		module_load_include('inc', 'exp_sp_lnrsearch', 'exp_sp_lnrsearch');

		$classArr       = array();
		$storeArr       = true;
		$classSession   = array();
		$sessionDetails = array();

		$i=0;
		$userId = $vLMSUserId;
		foreach ($classesListRes as $record) {

			if(!in_array($record->cls_id,$classArr)) {
				$classArr[] = $record->cls_id;
				$classSession[$record->cls_id] = $i;
				$storeArr = true;
			} else {
				$storeArr = false;
			}

			// $sessionclassid = sessionclassid;

			$result_response->rows[$i]['id'] = $record->id.'-'.$record->sessionid;
			if (empty($record->startdate)) {
				$dateCell       = '-';
				$startTimeCell  = '-';
				$endTimeCell    = '-';
			} else {
				if($record->delivery_type_code == "lrn_cls_dty_vcl" || ($record->delivery_type_code == "lrn_cls_dty_ilt" && $record->session_timezone)){
					$ses_start_date     = explode(" ",$record->startdate);
					$ses_stDtTm = $ses_start_date[0]." ".$record->session_start_time;
					$ses_edDtTm = $ses_start_date[0]." ".$record->session_end_time;
					$vTimeZone	= $record->session_timezone;
					if($userId){
						$UserPHPTimezoneName		= ($record->delivery_type_code == "lrn_cls_dty_vcl")?getUserPHPTimezoneName():date_default_timezone(FALSE);
						$vUserPHPTimezonename		= $UserPHPTimezoneName[0]->zone_name;
						if(!$UserPHPTimezoneName[0]->zone_name){
							$vUserPHPTimezonename = date_default_timezone(FALSE);
						}
					}else{
						$vUserPHPTimezonename = date_default_timezone(FALSE);
					}
					$oStartDateTime  = timeZoneConvert($ses_stDtTm , $vTimeZone,  $vUserPHPTimezonename);
					$oEndDateTime    = timeZoneConvert($ses_edDtTm, $vTimeZone,  $vUserPHPTimezonename);
					$startDate = date_format($oStartDateTime,'Y-m-d')." 00:00:00";
					$endDate  = $record->startdate;
					$startTime = date_format($oStartDateTime,'H:i');
					$endTime = date_format($oEndDateTime,'H:i');
					// $ilt_startDate =  $ses_stDtTm;
				}else{
					$startDate = $record->startdate;
					$endDate  = $record->startdate;
					$startTime = $record->session_start_time;
					$endTime = $record->session_end_time;
				}
				$sptStartDateTime     = explode(" ", strtoupper(dateTimeStrToUnivShort($startDate, TRUE, TRUE)));
				$dateCell             = $sptStartDateTime[0] . ' ' . $sptStartDateTime[1] . ' ' . $sptStartDateTime[2];
				$startTimeCell        = $sptStartDateTime[3] . ' ' . $sptStartDateTime[4];
				$sptEndDateTime       = explode(" ", strtoupper(dateTimeStrToUnivShort($endDate, TRUE, TRUE)));
				$endTimeCell          = $sptEndDateTime[3] . ' ' . $sptEndDateTime[4];
				$dateCell             = date_format(date_create($startDate),'M d, Y');
				$startTimeCell        = date_format(date_create($startTime),'g:i A');
				$endTimeCell          = date_format(date_create($endTime),'g:i A');
				//Added by Vincent on Oct 29, 2013 for #0028593
				$ilt_dateCell             = date_format(date_create($ses_stDtTm),'M d, Y');
				$ilt_startTimeCell        = date_format(date_create($record->session_start_time),'g:i A');
				$ilt_endTimeCell          = date_format(date_create($record->session_end_time),'g:i A');
			}

			if ($record->deliverytype == null || $record->deliverytype == '') {
				$deliveryType = '-';
			} else {
				$deliveryType = getProfileListItemAttr($record->deliverytype);
			}

			if ($record->locationname == null || $record->locationname == '') {
				$locationName = '-';
			} else {
				$locationName = $record->locationname;
			}

			/*if($record->currency_type == "cre_sys_crn_usd" || $record->currency_type == '' || $record->currency_type == null) {
			 $record->currency_type = '$';
			} else {
			$record->currency_type = '$';
			}*/
			if($record->currency_symbol == '' || $record->currency_symbol == null) {
				$record->currency_type = '';  // 0056924 - default $ removal
			}else{
				$record->currency_type = $record->currency_symbol;
			}

			if($record->export_compliance == null){
				$record->export_compliance = 'N';
			}

			if(empty($record->price)){
				$record->price = '0.00';
			}

			$record->multi_register    = is_multiregistration_allowed($record->id);
			$record->available_seats   = getAvailableSeats($record->id);
			$record->waitlist_seats    = getWaitlistCatalogInfo($record->id);
			$record->widgetId = 'gbox_paint-classes-list';
			$record->view_from = 'details';
			
			$result_response->rows[$i]['cell'] = array(
					'CourseId' => $record->crs_id,
					'ClassId' => $record->id,
					'ClassNodeId' => $record->node_id,
					'SessonId' => $record->sessionid,
					'NofAvailableSeats' => $record->available_seats,
					'ExportCompliance' => $record->export_compliance,
					'DeliveryTypeCode' => $record->delivery_type_code,
					'Price' => $record->price,
					'CurrencyTypeCode' => $record->currency_type,
					'name' => sanitize_data($record->name),
					'mro_id' => $record->mro_id,
					'date' => $dateCell,
					'start' => $startTimeCell,
					'end' => $endTimeCell,
					//Added by Vincent on Oct 29, 2013 for #0028593
					'ilt_date' => $ilt_dateCell,
					'ilt_start' => $ilt_startTimeCell,
					'ilt_end' => $ilt_endTimeCell,

					'type' => $deliveryType,
					'location' => $locationName,
					'action' => commonRegisterHtml($record,true),
					'session_start'       => $startDate,
					'session_start_format' => date_format(date_create($startDate),'M d, Y'),
					'session_start_time'         => $startTime,
					'session_end_time'           => $endTime,
					'session_name' => $record->name,
					'session_address1' => $record->contact_addr1,
					'session_address2' => $record->contact_addr2,
					'session_country' => $record->contact_country,
					'session_state' => $record->contact_state,
					'session_city' => $record->contact_city,
					'session_zipcode' => $record->contact_zipcode,
			);
			$i++;

		}
		echo drupal_json_encode($result_response);

		expDebug::dPrint('fetch_classes_list_view = '.print_r(drupal_json_encode($result_response), true) , 4);
	}catch (Exception $ex) {
		watchdog_exception('fetch_classes_list_view', $ex);
		expertusErrorThrow($ex);
	}
}
function fetch_classes_list($courseId,$callFrom,$isFromTP=FALSE) {
  try{
  	$timeZone = date_default_timezone(false);
  	$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
  	expDebug::dPrint('values for the access privileges fff'.$callFrom);
  	global $user;
  $vLMSUserId = ($user->uid > 0)?getSltpersonUserId():'';
  //$page = $_GET['page']; // get the requested page
  //$limit = $_GET['rows']; // get how many rows we want to have into the grid
  $sidx = addslashes($_GET['sidx']); // get index row - i.e. user click to sort
  $sord = $_GET['sord']; // get the direction
  if(!$sidx) $sidx ='name';
 // $classsrQuery = 'if((select count(1) from slt_group_mapping grp_map where grp_map.entity_id = class.id AND group_map.entity_type = \'cre_sys_obt_cls\')>0,(group_map.entity_id=class.id
// AND group_map.entity_type = \'cre_sys_obt_cls\'),(group_map.entity_id=class.course_id AND group_map.entity_type = \'cre_sys_obt_crs\'))';
  // Select the tables to fetch data from
  $UserDefaultCurrencyArr = getUserDefaultCurrency();
  $UserPrefCurrCode = $UserDefaultCurrencyArr['currency_code'];
  $UserPrefCurrSym = $UserDefaultCurrencyArr['currency_sym'];

  $classesListSql = db_select('slt_course_class','class');
  $classesListSql->leftJoin('slt_course_template','crs','class.course_id = crs.id');
  //$classesListSql->leftjoin('slt_entity_profile_mapping', 'sepm', 'sepm.entity_id=class.id AND sepm.entity_type=27');
  //$classesListSql->leftJoin('slt_mro_mapping', 'mro', 'mro.entity_id=class.id AND mro.job_role_id = (SELECT job_role FROM slt_person WHERE id=\''.$vLMSUserId.'\') AND mro.entity_type = \'cre_sys_obt_cls\'');
  //$classesListSql->leftjoin('slt_catalog_access', 'catacs', 'catacs.entity_id=class.id AND catacs.entity_type = \'cre_sys_obt_cls\' ');
  // $classesListSql->leftJoin('slt_group_mapping', 'group_map', '('.$classsrQuery.') AND  group_map.group_type = 0');
  // $classesListSql->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');
  //$classesListSql->leftJoin('slt_course_class_session','ses','ses.class_id=class.id AND ses.course_id=class.course_id');
  $classesListSql->leftjoin('slt_course_class_session', 'ses', 'class.id=ses.class_id and ses.id=(SELECT id FROM  slt_course_class_session WHERE class_id=ses.class_id ORDER BY start_date asc, start_time asc LIMIT 1)');
  $classesListSql->leftjoin('slt_course_class_session', 'ses1', 'class.id=ses1.class_id and ses1.id=(SELECT id FROM  slt_course_class_session WHERE class_id=ses1.class_id ORDER BY start_date desc, start_time desc LIMIT 1)');
  $classesListSql->leftjoin('slt_facility', 'splt_fac', 'splt_fac.id=ses.facility_id');
  $classesListSql->leftJoin('slt_location','loc','class.location_id = loc.id');
  $classesListSql->leftJoin('slt_profile_list_items','pfl','ses.timezone = pfl.code');
  $classesListSql->leftJoin('slt_node_learning_activity','nla','nla.entity_id = class.id');
  //$classesListSql->leftjoin('slt_course_content_mapper', 'content', ' class.id = content.class_id AND crs.id = content.course_id');
  $classesListSql->leftjoin('slt_course_content_mapper', 'content', '  content.id=(SELECT id FROM slt_course_content_mapper cont WHERE class.id = cont.class_id AND crs.id = cont.course_id LIMIT 1 )');

  // Select the fields to fetch
  $classesListSql->addField('class','id','id');
  $classesListSql->addField('crs','id','crs_id');
  $classesListSql->addField('nla','node_id','node_id');
  $classesListSql->addField('class','id','cls_id');
  $classesListSql->addField('class','code','code');
  $classesListSql->addField('class','lang_code','lang');
  $classesListSql->addField('class','title','name');
  $classesListSql->addField('class','short_description','description');
  $classesListSql->addField('class','delivery_type','delivery_type_code');
  $classesListSql->addField('class','delivery_type','deliverytype');
  $classesListSql->addField('loc','name','locationname');
  $classesListSql->addExpression('slf_convert_price(class.price,class.currency_type,\''.$UserPrefCurrCode.'\')','price');
  $classesListSql->addExpression('\''.$UserPrefCurrSym.'\'','currency_symbol');
  //$classesListSql->addField('class','price','price');
  $classesListSql->addField('class','custom4','export_compliance');
  $classesListSql->addField('class','currency_type','currency_type');
  $classesListSql->addField('class','custom4','currencyprefixsymbol');
  $classesListSql->addField('class','custom4','currencypostfixsymbol');
  $classesListSql->addField('class','scheduled_duration','duration');
  $classesListSql->addField('class','max_seats','maxcapacity');
  $classesListSql->addField('class','waitlist_count','avaliableseatcount');
  $classesListSql->addField('class','valid_to','enrollmentvalidity');
  $classesListSql->addField('ses','id','sessionid');
  $classesListSql->addField('ses','class_id','sessionclassid');
  $classesListSql->addField('ses','start_date','startdate');
  $classesListSql->addField('ses','start_time','session_start_time');
  $classesListSql->addField('ses1','end_time','session_end_time');
  $classesListSql->addField('ses','title','session_title');
  
  if($callFrom == 'admin') {
  	$classesListSql->addField('group_map','mro', 'mro_id');
  }
  else {
  	if($isFromTP != '1') {
  	  $classesListSql->addExpression('if(usrmap.mro is not null, usrmap.mro, group_map.mro )', 'mro_id');
  	}
  }
  $classesListSql->addField('loc','name', 'name');
  $classesListSql->addField('loc','addr1', 'contact_addr1');
  $classesListSql->addField('loc','addr2', 'contact_addr2');
  //$classesListSql->addField('loc','country', 'contact_country');
  //$classesListSql->addField('loc','state', 'contact_state');
  $classesListSql->addField('loc','city', 'contact_city');
  $classesListSql->addField('loc','zipcode', 'contact_zipcode');
  $classesListSql->addExpression('(select country_name from slt_country where country_code = loc.country)', 'contact_country');
  $classesListSql->addExpression('(select state_name from slt_state where state_code = loc.state and country_code =  loc.country)', 'contact_state');

  $classesListSql->addField('ses','end_date','enddate');
  $classesListSql->addField('ses','timezone','timezone');
  $classesListSql->addField('pfl','attr2','session_timezone');
  $classesListSql->addField('pfl','attr1','attr1');
  $classesListSql->addField('pfl','attr4','attr4');

  $classesListSql->addField('class','registration_end_on', 'registration_end_on');
  $classesListSql->addExpression("CONCAT( DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.end_time,':00')", 'session_end_datetime'); 

  // Set the conditions
  // below line commented for Mantis - SQL Injection - #31297
  //$classesListSql->condition('class.course_id',$courseId);
  $classesListSql->condition('class.status','lrn_cls_sts_atv');
  $classesListSql->condition('crs.status','lrn_crs_sts_atv');
  $classesListSql->condition('nla.entity_type','cre_sys_obt_cls');
  if($isFromTP==FALSE) // fix for issue #0025076
  	$classesListSql->where('(class.dedicated_class_flag = \'N\' or class.dedicated_class_flag is null) AND class.course_id=:courseid',array(':courseid'=>$courseId));
  else
  	$classesListSql->where('class.course_id=:courseid',array(':courseid'=>$courseId));

/*  $classesListSql->condition(db_and()->
      condition(db_or()->
        condition(db_and()->
          condition('class.delivery_type', 'lrn_cls_dty_ilt', '=')->
          condition('ses.start_date', now(), '>='))->
        condition(db_and()->
          condition('class.delivery_type', 'lrn_cls_dty_vcl', '=')->
          condition('ses.start_date', now(), '>='))->

        condition(db_and()->
          condition('class.delivery_type', 'lrn_cls_dty_wbt', '=')->
          where('content.content_id IS NOT NULL'))->
        //condition('class.delivery_type', 'lrn_cls_dty_wbt', '=')->
        condition('class.delivery_type', 'lrn_cls_dty_vod', '=')
      ));*/
  
  $classesListSql->groupBy("class.id having session_end_datetime > IFNULL(CONVERT_TZ(now(), '".$defaultTimezone."' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone)),NOW()) or session_end_datetime is null");// Added by sabeena for #0029130
  expDebug::dPrintDBAPI('values for the access privileges',$classesListSql);
  // Further filter by role
  //addFilterForLoggedInUserRole($classesListSql, 'sepm');
  //filterByUserAccessPrivileges($classesListSql, 'catacs','','group_map');
  
  if($callFrom == 'admin') {
  	filterByUserAccessPrivileges($classesListSql, 'class','',1,'cre_sys_obt_cls','Class','class');
  }
  else {
  	//if($isFromTP != '1') {  Commented by Rajeshwar on Dec 23, 2016 for #0070652 
  	  filterByUserAccessPrivileges($classesListSql, 'class','','','cre_sys_obt_cls','Class','class');
  	//}
  	}
  // Group By
  //$classesListSql->groupBy('class.id');

  // Set order of fetched records
  $classesListSql->orderBy($sidx,$sord);

  expDebug::dPrintDBAPI("fetch_classes_list() : ", $classesListSql);

  $classesListRes = $classesListSql->execute()->fetchAll();
  expDebug::dPrint(print_r($classesListRes, true) , 3);

  $result_response->page    = $page;
  $result_response->total   = $total_pages;
  $result_response->records = count($classesListRes);

  module_load_include('inc', 'exp_sp_lnrsearch', 'exp_sp_lnrsearch');

  $classArr       = array();
  $storeArr       = true;
  $classSession   = array();
  $sessionDetails = array();

  $i=0;
  $userId = $vLMSUserId;
  foreach ($classesListRes as $record) {

  if(!in_array($record->cls_id,$classArr)) {
   	  $classArr[] = $record->cls_id;
   	  $classSession[$record->cls_id] = $i;
      $storeArr = true;
   	} else {
   	  $storeArr = false;
   	}

    // $sessionclassid = sessionclassid;

        $result_response->rows[$i]['id'] = $record->id.'-'.$record->sessionid;
        if (empty($record->startdate)) {
            $dateCell       = '-';
            $startTimeCell  = '-';
            $endTimeCell    = '-';
        } else {
            if($record->delivery_type_code == "lrn_cls_dty_vcl" || ($record->delivery_type_code == "lrn_cls_dty_ilt" && $record->session_timezone)){
            	expDebug::dPrint("fetch_deltyppe");
              $ses_start_date     = explode(" ",$record->startdate);
              $ses_stDtTm = $ses_start_date[0]." ".$record->session_start_time;
              $ses_edDtTm = $ses_start_date[0]." ".$record->session_end_time;
              $vTimeZone	= $record->session_timezone;
        	  if($userId){
            		$UserPHPTimezoneName		= getUserPHPTimezoneName($userId);
            		$vUserPHPTimezonename		= $UserPHPTimezoneName[0]->zone_name;
            		if(!$UserPHPTimezoneName[0]->zone_name){
            		  $vUserPHPTimezonename = date_default_timezone(FALSE);
            		}
    		    }else{
    		        $vUserPHPTimezonename = date_default_timezone(FALSE);
    		    }
        	  $oStartDateTime  = timeZoneConvert($ses_stDtTm , $vTimeZone,  $vUserPHPTimezonename);
        	  $oEndDateTime    = timeZoneConvert($ses_edDtTm, $vTimeZone,  $vUserPHPTimezonename);
              $startDate = date_format($oStartDateTime,'Y-m-d')." 00:00:00";
              $endDate  = $record->startdate;
              $startTime = date_format($oStartDateTime,'H:i');
              $endTime = date_format($oEndDateTime,'H:i');
             // $ilt_startDate =  $ses_stDtTm;
            }else{
              $startDate = $record->startdate;
              $endDate  = $record->startdate;
              $startTime = $record->session_start_time;
              $endTime = $record->session_end_time;
            }
    		$sptStartDateTime     = explode(" ", strtoupper(dateTimeStrToUnivShort($startDate, TRUE, TRUE)));
    		$dateCell             = $sptStartDateTime[0] . ' ' . $sptStartDateTime[1] . ' ' . $sptStartDateTime[2];
    		$startTimeCell        = $sptStartDateTime[3] . ' ' . $sptStartDateTime[4];
    		$sptEndDateTime       = explode(" ", strtoupper(dateTimeStrToUnivShort($endDate, TRUE, TRUE)));
    		$endTimeCell          = $sptEndDateTime[3] . ' ' . $sptEndDateTime[4];
    		$dateCell             = date_format(date_create($startDate),'M d, Y');
            $startTimeCell        = date_format(date_create($startTime),'g:i A');
            $endTimeCell          = date_format(date_create($endTime),'g:i A');
            	//Added by Vincent on Oct 29, 2013 for #0028593
            $ilt_dateCell             = date_format(date_create($ses_stDtTm),'M d, Y');
            $ilt_startTimeCell        = date_format(date_create($record->session_start_time),'g:i A');
            $ilt_endTimeCell          = date_format(date_create($record->session_end_time),'g:i A');
       }

       if ($record->deliverytype == null || $record->deliverytype == '') {
      	$deliveryType = '-';
       } else {
      	$deliveryType = getProfileListItemAttr($record->deliverytype);
       }

       if ($record->locationname == null || $record->locationname == '') {
      	$locationName = '-';
       } else {
      	$locationName = $record->locationname;
       }

       /*if($record->currency_type == "cre_sys_crn_usd" || $record->currency_type == '' || $record->currency_type == null) {
      	$record->currency_type = '$';
       } else {
       	$record->currency_type = '$';
       }*/
       if($record->currency_symbol == '' || $record->currency_symbol == null) {
	     $record->currency_type = '';  // 0056924 - default $ removal
	   }else{
	     $record->currency_type = $record->currency_symbol;
	   }

       if($record->export_compliance == null){
        	$record->export_compliance = 'N';
       }

       if(empty($record->price)){
        	$record->price = '0.00';
       }

       $record->multi_register    = is_multiregistration_allowed($record->id);
       $record->available_seats   = getAvailableSeats($record->id);
       $record->waitlist_seats    = getWaitlistCatalogInfo($record->id);
       $record->widgetId = 'gbox_paint-classes-list';
	   /*if(!empty($userId))
       $exmpId = getRegisteredEnrollmentExmeptedorNot('class',$userId,$record->crs_id,$record->id);*/
       expDebug::dPrint("fetch_classes_list() 1111: ".print_r($record->crs_id,1),5); 
       expDebug::dPrint("fetch_classes_list()11111 : ".print_r($record->id,1),5);
       $record->view_from = 'details';

       $result_response->rows[$i]['cell'] = array(
        'CourseId' => $record->crs_id,
        'ClassId' => $record->id,
        'ClassNodeId' => $record->node_id,
        'SessonId' => $record->sessionid,
        'NofAvailableSeats' => $record->available_seats,
        'ExportCompliance' => $record->export_compliance,
        'DeliveryTypeCode' => $record->delivery_type_code,
        'Price' => $record->price,
        'CurrencyTypeCode' => $record->currency_type,
        'name' => sanitize_data($record->name),
        'mro_id' => $record->mro_id,
        'date' => $dateCell,
        'start' => $startTimeCell,
        'end' => $endTimeCell,
       	//Added by Vincent on Oct 29, 2013 for #0028593
        'ilt_date' => $ilt_dateCell,
        'ilt_start' => $ilt_startTimeCell,
        'ilt_end' => $ilt_endTimeCell,

        'type' => $deliveryType,
        'location' => $locationName,
        'action' => commonRegisterHtml($record,true),
        'session_start'       => $startDate,
        'session_start_format' => date_format(date_create($startDate),'M d, Y'),
        'session_start_time'         => $startTime,
        'session_end_time'           => $endTime,
        'session_name' => $record->name,
        'session_address1' => $record->contact_addr1,
        'session_address2' => $record->contact_addr2,
        'session_country' => $record->contact_country,
        'session_state' => $record->contact_state,
        'session_city' => $record->contact_city,
        'session_zipcode' => $record->contact_zipcode,
       	'session_timezone' => $record->session_timezone,
       		'user_timezone' => $vUserPHPTimezonename,
       		'session_code'  => $record->attr1,
       		'short_code'  => $record->attr4,
       		//	'exemptedid'=> ''
       );
       $i++;

 }
 /*if($classListOnly == true){
 	return $result_response;
	
 }else {*/
 echo drupal_json_encode($result_response);
 // }

 expDebug::dPrint('fetch_classes_list = '.print_r(drupal_json_encode($result_response), true) , 4);
 }catch (Exception $ex) {
    watchdog_exception('fetch_classes_list', $ex);
    expertusErrorThrow($ex);
  }
}

function fetch_class_details($classIdAndSessionId) {
  try{
  expDebug::dPrint(" classIdAndSessionId = ". $classIdAndSessionId , 4);

  $idArr = explode("-", $classIdAndSessionId);
  expDebug::dPrint(" idArr = ". print_r($idArr, true) , 4);

  $classId = $idArr[0];
  $sessionId = $idArr[1];

  // Fetch class attachments

  $classAttachmentSql = db_select('slt_entity_reading_mapper','serm');
  $classAttachmentSql->addField('serm','id','Id');
  $classAttachmentSql->addField('serm','reading_type','ReadingTitle');
  $classAttachmentSql->addField('serm','reading_content','ReadingContent');

  $classAttachmentSql->condition('serm.entity_id',$classId);
  $classAttachmentSql->condition('serm.entity_type','cre_sys_obt_cls');
  $classAttachmentSql->condition('serm.showlnr','Y');

  expDebug::dPrintDBAPI(" classAttachmentSql = ", $classAttachmentSql);


  $classAttachmentsRes = $classAttachmentSql->execute();

  expDebug::dPrint(" classAttachmentSql rowCount= ". $classAttachmentsRes->rowCount() , 3);
  $classAttachmentsCnt = $classAttachmentsRes->rowCount();
  $classAttachmentsResult = $classAttachmentsRes->fetchAll(); //$classAttachmentsRes->fetchAllAssoc('id',PDO::FETCH_ASSOC);
  expDebug::dPrint(" classAttachmentsResult= ". print_r($classAttachmentsResult,true), 3);
  //$classAttachmentsArray = array("NumAttachments" => count($classAttachmentsRes));
  $classAttachmentsArray = array("NumAttachments" => $classAttachmentsCnt);

  $classLocationDetailsSql = '';

  $UserDefaultCurrencyArr = getUserDefaultCurrency();
  $UserPrefCurrCode = $UserDefaultCurrencyArr['currency_code'];
  $UserPrefCurrSym = $UserDefaultCurrencyArr['currency_sym'];

  if (!empty($sessionId)){ //($sessionId != null && $sessionId != '') {
    $classDetailsSql = db_select('slt_course_class','class');
    $classDetailsSql->leftJoin('slt_course_class_session','ses','class.id=ses.class_id');
    $classDetailsSql->leftJoin('slt_location','loc','class.location_id = loc.id');
    $classDetailsSql->leftJoin('slt_state','sname','loc.state = sname.state_code');
    $classDetailsSql->leftJoin('slt_country','con','loc.country=con.country_code');

    $classDetailsSql->addField('class','title','title');
    $classDetailsSql->addField('class','id','classid');
    $classDetailsSql->addField('class','course_id','courseid');
    $classDetailsSql->addField('class','code','code');
    $classDetailsSql->addField('class','short_description','description');
    $classDetailsSql->addField('class','scheduled_duration','duration');
    $classDetailsSql->addField('class','lang_code','lang'); //Lang
    $classDetailsSql->addField('class','delivery_type','deliverytypecode');
    $classDetailsSql->addField('class','delivery_type','deliverytype');
    $classDetailsSql->addField('ses','start_date','startdatetime');
    $classDetailsSql->addField('ses','end_date','enddatetime');
    $classDetailsSql->addField('loc','id','locationid');
    $classDetailsSql->addField('loc','name','locationname');
    $classDetailsSql->addField('loc','addr1','locationaddr1');
    $classDetailsSql->addField('loc','addr2','locationaddr2');
    $classDetailsSql->addField('loc','city','locationcity');
    //$classDetailsSql->addField('sname','state_name','locationstate');
    //$classDetailsSql->addField('loc','country','locationcountry');
    $classDetailsSql->addExpression('(select country_name from slt_country where country_code = loc.country LIMIT 1)', 'locationcountry');
    $classDetailsSql->addExpression('(select state_name from slt_state where state_code = loc.state and country_code =  loc.country LIMIT 1)', 'locationstate');
    $classDetailsSql->addField('con','country_name','locationcountryname');
    $classDetailsSql->addField('loc','zipcode','locationzip');
    $classDetailsSql->addField('loc','phone','locationphone');
    $classDetailsSql->addField('loc','latitude','latitude');
    $classDetailsSql->addField('loc','longitude','longitude');
    $classDetailsSql->addExpression('slf_convert_price(class.price,class.currency_type,\''.$UserPrefCurrCode.'\')','price');
    //$classDetailsSql->addField('class','price','price');
    $classDetailsSql->addField('class','currency_type','currencytypecode');
    $classDetailsSql->addField('class','custom4','currencyprefixsymbol');
    $classDetailsSql->addField('class','custom4','currencypostfixsymbol');
    $classDetailsSql->addField('class','max_seats','maxcapacity');
    $classDetailsSql->addField('class','waitlist_count','availableseatcount');
    $classDetailsSql->addField('class','valid_to','enrollmentvalidity');
    $classDetailsSql->addField('ses','timezone','timezone');
	$classDetailsSql->addField('class','additional_info','additional_info');
	$classDetailsSql->addField('class','addn_catalog_show','addn_catalog_show');
	$classDetailsSql->addField('class','addn_notification_show','addn_notification_show');
    // below line commented for Mantis - SQL Injection #31297
    //$classDetailsSql->condition('class.id',$classId);
    $classDetailsSql->where('class.id=:classid',array(':classid'=>$classId));
	
  }
  else {
    $classDetailsSql = db_select('slt_course_class','class');
    $classDetailsSql->addField('class','title','title');
    $classDetailsSql->addField('class','id','classid');
    $classDetailsSql->addField('class','course_id','courseid');
    $classDetailsSql->addField('class','code','code');
    $classDetailsSql->addField('class','short_description','description');
    $classDetailsSql->addField('class','scheduled_duration','duration');
    $classDetailsSql->addField('class','lang_code','lang');
    $classDetailsSql->addField('class','delivery_type','deliverytypecode');
    $classDetailsSql->addField('class','delivery_type','deliverytype');
    $classDetailsSql->addExpression('slf_convert_price(class.price,class.currency_type,\''.$UserPrefCurrCode.'\')','price');
    //$classDetailsSql->addField('class','price','price');
    $classDetailsSql->addField('class','currency_type','currencytypecode');
    $classDetailsSql->addField('class','custom4','currencyprefixsymbol');
    $classDetailsSql->addField('class','custom4','currencypostfixsymbol');
    $classDetailsSql->addField('class','max_seats','maxcapacity');
    $classDetailsSql->addField('class','waitlist_count','avaliableseatcount');
    $classDetailsSql->addField('class','valid_to','enrollmentvalidity');
	$classDetailsSql->addField('class','additional_info','additional_info');
	$classDetailsSql->addField('class','addn_catalog_show','addn_catalog_show');
	$classDetailsSql->addField('class','addn_notification_show','addn_notification_show');
    // below line commented for Mantis - SQL Injection #31297
    //$classDetailsSql->condition('class.id',$classId);
    $classDetailsSql->where('class.id=:classid',array(':classid'=>$classId));
  }

   expDebug::dPrintDBAPI(" classDetailsSql = ", $classDetailsSql);

   $classDetailsRes = $classDetailsSql->execute();
   expDebug::dPrint('classDetailsRes = ' . print_r($classDetailsRes, true) , 3);

   if ($classLocationDetailsSql != '') {
     $classLocationDetailsRes = db_query($classLocationDetailsSql);
     expDebug::dPrintDBAPI('$classLocationDetailsRes = ', $classLocationDetailsRes);
   }
   $record = $classDetailsRes->fetchAssoc();
   expDebug::dPrint('$record = ' . print_r($record, true) , 3);

   module_load_include('inc', 'exp_sp_lnrsearch', 'exp_sp_lnrsearch');
   $result_response = array(
        'ClassIdAndSessionId' => $classIdAndSessionId,
     	'Title' => $record['title'],
     	'Code' => $record['code'],
     	'Description' => $record['description'],
     	'Duration' => formatDuration($record['duration']),
     	'Lang' => $record['lang'],
        'Language' => getProfileListItemName($record['lang']),
        'Additional notes' => $record['additional_info'],
        'addn_catalog_show' => $record['addn_catalog_show'],
        'addn_notification_show' => $record['addn_notification_show'],
        'DeliveryTypeCode' => $record['deliverytypecode'],
        'DeliveryType' => $record['deliverytype'],
        'StartDateTime' => $record['startdatetime'],
        'EndDateTime' => $record['enddatetime'],
        'AvailableSeats' => getAvailableSeats($classId),
        'LocationId' => $record['locationid'],
		'LocationName' => $record['locationname'],
        'LocationAddr1' => $record['locationaddr1'],
		'LocationAddr2' => $record['locationaddr2'],
		'LocationCity' => $record['locationcity'],
		'LocationState' => $record['locationstate'],
		'LocationCountry' => $record['locationcountry'],
		'LocationCountryName' => $record['locationcountryname'],
		'LocationZip' => $record['locationzip'],
		'LocationPhone' => $record['locationphone'],
		'Latitude' => $record['latitude'],
		'Longitude' => $record['longitude'],
        'Price' => $record['price'],
        'CurrencyTypeCode' => $record['currencytypecode'],
        'CurrencyPrefixSymbol' => $record['currencyprefixsymbol'],
        'CurrencyPostfixSymbol' => $record['curencypostfixsymbol'],
        'SessionDetails' => getSessionDetails($record['courseid'],$record['classid'],$record['deliverytypecode']),
        'Attachments' => $classAttachmentsArray,
   		'AttachmentsResult' => $classAttachmentsResult
  );

   /* $exmpId = getRegisteredEnrollmentExmeptedorNot('tp',$userId,$results->cls_id);
   if(empty($exmpId)) */
 expDebug::dPrint('result_response = '.print_r($result_response, true) , 4);
 expDebug::dPrint('result_response after json encoding = '.print_r(drupal_json_encode($result_response), true) , 4);
 //echo drupal_json_encode($result_response);
 return $result_response;
 }catch (Exception $ex) {
    watchdog_exception('fetch_class_details', $ex);
    expertusErrorThrow($ex);
  }
}

function getCourseDetailsWrapperApi($courseId,$courseCode){
  try{
  $gCourseId = '';
  if(!empty($courseCode)){
    $select = db_select('slt_course_template', 'course');
    $select -> addField('course','id');
    $select -> condition('course.code',$courseCode);
    $gCourseId = $select -> execute() -> fetchField();
  }
  if($courseId && empty($courseCode)){
    $gCourseId = $courseId;
  }
  if(!(empty($courseId)) && (!empty($courseCode)) && ($courseId != $gCourseId)){
    $gCourseId = '';
  }
  if($gCourseId=="")
  {
  	//return array((object)array('id'=>'CourseID is Invalid.'));
  	$errobj=new stdClass();
  	$errobj->isValidateError = 1;
  	$errobj->errcode = 'L_012';
  	$errobj->errormsg = "CourseID is Invalid.";
  	return $errobj;
  	//throw new Exception("CourseID is Invalid.");
  }
  $ret_array = array();
  if(!empty($gCourseId))
  {
   $ret_array[0] = getCourseDetails($gCourseId,1);
  }
 return $ret_array;
 }catch (Exception $ex) {
    watchdog_exception('getCourseDetailsWrapperApi', $ex);
    expertusErrorThrow($ex);
  }
}


function fetch_detailed_class_list($courseId, $callFrom, $isFromTP=FALSE) {
	try {
		global $user;
		$vLMSUserId = ($user->uid > 0)?getSltpersonUserId():'';
		$page = $_GET['page']; // get the requested page
        $limit = $_GET['rows'];
		$total_pages = 0;
		
		$classesListSql = db_select('slt_course_class','class');
		$classesListSql->leftJoin('slt_course_template','crs','class.course_id = crs.id');
		$classesListSql->addField('class','id','classid');
		if($callFrom == 'admin') {
			$classesListSql->addField('group_map','mro', 'mro_id');
		}
		else {
			if($isFromTP != '1') {
				$classesListSql->addExpression('if(usrmap.mro is not null, usrmap.mro, group_map.mro )', 'mro_id');
			}
		}
		$classesListSql->condition('class.status','lrn_cls_sts_atv');

		if($isFromTP==FALSE) // fix for issue #0025076
			$classesListSql->where('(class.dedicated_class_flag = \'N\' or class.dedicated_class_flag is null) AND class.course_id=:courseid',array(':courseid'=>$courseId));
		else
			$classesListSql->where('class.course_id=:courseid',array(':courseid'=>$courseId));
		
		if($callFrom == 'admin') {
			filterByUserAccessPrivileges($classesListSql, 'class','',1,'cre_sys_obt_cls','Class','class');
		}
		else {
			filterByUserAccessPrivileges($classesListSql, 'class','','','cre_sys_obt_cls','Class','');
		}
		
		// Set order of fetched records
		// $classesListSql->orderBy($sidx,$sord);
		$classesListSql->groupBy('classid');
		expDebug::dPrintDBAPI("fetch_detailed_classes_list count query : ", $classesListSql);
		$classesListCount 	= $classesListSql->execute()->fetchCol();
		$totalRecords 		= count($classesListCount);
		if ( $totalRecords > 0 ) {
			$total_pages = ceil($totalRecords/$limit);
		} 
		$start = calculate_offset($page,$limit);
		
		$classesListSql->range($start, $limit);
		expDebug::dPrintDBAPI("fetch_detailed_classes_list select query : ", $classesListSql);
		
		$classesListRes = $classesListSql->execute()->fetchAll();
		expDebug::dPrint(print_r($classesListRes, true) , 3);
		
		$result_response->page    = $page;
		$result_response->total   = $total_pages;
		$result_response->records = count($classesListCount);
		$result_response->course_id    = $courseId;
		
		module_load_include('inc', 'exp_sp_classdetail', 'exp_sp_classdetail');
		
		$class_details_arr       = array();
		
		$i=0;
		$userId = $vLMSUserId;
		$pagefrom ='course_class_list';
		if($isFromTP==true)
			$pagefrom ='lrnplan_course_class_list';
		
		foreach ($classesListRes as $record) {
			if($isFromTP==1)
				$classDetails =  getFullClassDetails($record->classid, $pagefrom);
			else 
				$classDetails =  getFullClassDetails($record->classid, $pagefrom);
			$result_response->rows[$i]['id'] = $record->classid;
			$result_response->rows[$i]['class'] = $classDetails;
			$result_response->rows[$i]['cell'] = theme('content_class_list', array(
													"result" => $classDetails,
													"context" => ['callfrom' => $pagefrom],
												));
			
			$i++;
		
		}
		
		echo drupal_json_encode($result_response);
		
		expDebug::dPrint('fetch_classes_list = '.print_r(drupal_json_encode($result_response), true) , 4);
	} catch(Exception $e) {
		
	}
}
?>