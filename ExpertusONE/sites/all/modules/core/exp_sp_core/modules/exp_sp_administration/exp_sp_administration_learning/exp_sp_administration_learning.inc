<?php

function getTagsAutoComplete() {
	try {
		$searchText =  $_GET['z'];
		if (strpos($_GET['exclude_tag_ids'], VAR_START_CHAR) !== false)
			  $excludedTagsId = core_decrypt($_GET['exclude_tag_ids']);
	  else
		   $excludedTagsId = $_GET['exclude_tag_ids'];
		$entityType = escape_string($_GET['entityType']);
		// Select the table
                $select = db_select('slt_tagdefn', 'tag');

		// Add fields to fetchgetTpCourseAutoComplete
		$select->addField('tag', 'id', 'id');
		$select->addField('tag', 'tagname', 'tag_name');
		// have added the distinct for avoid dublicate tag listing
		$select->distinct('tag', 'tagname');
		// Add condition
                if (!empty($excludedTagsId)) 
                    $select->where('tag.id not in ('.$excludedTagsId.')');    
		$select->condition('tag.tagname', '%' . db_like($searchText) . '%', 'LIKE');
		//$select->condition('tag.activity_count', 0, '<>');
		// Changes Made For this Ticket #0034575
		$select->leftjoin('slt_tag_entity', 'ent','ent.tagid=tag.id ');
		expDebug::dPrint('$entityType : '.$entityType, 4);
		/* Ticket: 46992 */
		if($entityType == 'Course'){
	  		$select->condition('ent.entity_type','Course','=');
		  $select->leftjoin('slt_course_template', 'crs','crs.id = ent.entity_id and ent.entity_type = \'Course\' ');
		  filterByUserAccessPrivileges($select, 'crs','',1,'cre_sys_obt_crs','','crs');
		} else if ($entityType == 'Class'){
	  		$select->condition('ent.entity_type','Class','=');
		  $select->leftjoin('slt_course_class', 'cls','cls.id = ent.entity_id and ent.entity_type = \'Class\' ');
		  filterByUserAccessPrivileges($select, 'cls','',1,'cre_sys_obt_cls','','cls');
		} else if ($entityType == 'Content'){
	 		$select->condition('ent.entity_type','Content','=');
		  $select->leftjoin('slt_content_master', 'cnt','cnt.id = ent.entity_id and ent.entity_type = \'Content\' ');
		  filterByUserAccessPrivileges($select, 'cnt','',1,'cre_sys_obt_cnt','','cnt');
		} else if ($entityType == 'sry_det_typ_sry_qus'){
	  	$select->condition('ent.entity_type','sry_det_typ_sry_qus','=');
	  	$select->leftjoin('slt_survey_questions', 'qus','qus.id = ent.entity_id and ent.entity_type = \'sry_det_typ_sry_qus\' ');
	  	filterByUserAccessPrivileges($select, 'qus','',1,'sry_det_typ_sry_qus','','qus');
		}else if ($entityType == 'sry_det_typ_ass_qus'){
	  	$select->condition('ent.entity_type','sry_det_typ_ass_qus','=');
	  	$select->leftjoin('slt_survey_questions', 'qus','qus.id = ent.entity_id and ent.entity_type = \'sry_det_typ_ass_qus\' ');
	  	filterByUserAccessPrivileges($select, 'qus','',1,'sry_det_typ_ass_qus','','qus');
		} else {
	 		$select->condition('ent.entity_type',array('Certification','Curricula', 'Learning Plan'));
	  	$select->leftjoin('slt_program', 'prg','prg.id = ent.entity_id and ent.entity_type IN (\'Certification\',\'Curricula\', \'Learning Plan\') ');
	  	filterByUserAccessPrivileges($select, 'prg','',1,'cre_sys_obt_trp','','prg');
		}
		/* Ticket: 46992 */
		/* if (!empty($excludedTagsId)) {
		 $excludedTagsId = urldecode($excludedTagsId);
		 $excludedTagsIdArr = explode(',', $excludedTagsId);
		 $select->condition('tag.id', $excludedTagsIdArr, 'NOT IN');
		 } */
		// Order in dictionary sort order
		$select->orderBy('tag_name');
		expDebug::dPrintDBAPI(' $select obj = ' , $select);
		$matchingTagNames = $select->execute()->fetchAll();
		expDebug::dPrint(' $$matchingTagNames = '. print_r($matchingTagNames, true) , 3);

		foreach ($matchingTagNames as $tagName) {
			print $tagName->id.EXP_AC_SEPARATOR.$tagName->tag_name . "\n";
		}
	} catch (Exception $ex) {
		watchdog_exception('getTagsAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getInstructorAutoComplete() - Returns list of instructors for instructor autcomplete field
 */
function getInstructorAutoComplete() {
	try {
		$searchText =  $_GET['z'];
		$excludedInstructorId = $_GET['excluded_instructor_id'];
		$classId = $_GET['class_id'];

		// Select the table
		$select = db_select('slt_person', 'per');
		// Add fields to fetchgetTpCourseAutoComplete
		$select->addField('per', 'id', 'id');
		$select->addField('per', 'full_name', 'full_name');
		$select->addField('per', 'user_name', 'user_name');

		// Add condition
		$select->condition(db_or()
		->condition('per.full_name', '%' . db_like($searchText) . '%', 'LIKE')
		->condition('per.user_name', '%' . db_like($searchText) . '%', 'LIKE'));

		$select->condition('per.is_instructor','Y');
		$select->condition('per.status','cre_usr_sts_atv');

		 
		if (!empty($excludedInstructorId)) {
			$excludedInstructorId = urldecode($excludedInstructorId);
			$excludedInstructorIdArr = explode(',', $excludedInstructorId);
			$select->condition('per.id', $excludedInstructorIdArr, 'NOT IN');
		}

		if (!empty($classId)) {
			// Exclude the enrolled users of this class from the returned list of instructors. Do not exclude those who have completed the class.
			$selectEnrolledUsersIds = db_select('slt_enrollment', 'enr');
			$selectEnrolledUsersIds->addField('enr', 'user_id', 'enr_user_id');
			$selectEnrolledUsersIds->condition('enr.class_id', $classId, '=');
			$selectEnrolledUsersIds->condition('enr.reg_status', array('lrn_crs_reg_can', 'lrn_crs_reg_rsc'), 'NOT IN');
			$selectEnrolledUsersIds->condition(db_or()
			->condition('enr.reg_status', 'lrn_crs_reg_cnf', '!=')
			->condition('enr.comp_status', 'lrn_crs_cmp_cmp', '!='));

			$select->condition('per.id', $selectEnrolledUsersIds, 'NOT IN');
		}

		filterByUserAccessPrivileges($select,'per','',1,'cre_usr','attach','per');
		// Order in dictionary sort order
		$select->orderBy('full_name');
		expDebug::dPrintDBAPI('$select', $select);

		$matchingInstructorNames = $select->execute()->fetchAll();
		expDebug::dPrint('$matchingInstructorNames = '. print_r($matchingInstructorNames, true) , 3);

		foreach ($matchingInstructorNames as $instructorName) {
			print $instructorName->id .EXP_AC_SEPARATOR. $instructorName->full_name . " " . '(' . $instructorName->user_name . ')' . "\n";
		}
	} catch (Exception $ex) {
		watchdog_exception('getInstructorAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}


/*
 * getInstructorAutoCompleteWithoutId() - Returns the matched Full Name from slt_person table.
 */
function getInstructorAutoCompleteWithoutId() { // Needed for search by manager username.
	try {
		$searchText =  $_GET['z'];

		$select = db_select('slt_person', 'per');

		$select->addField('per', 'id', 'id');
		$select->addField('per', 'full_name', 'full_name');

		// Add condition
		$select->condition('per.full_name', '%' . db_like($searchText) . '%', 'LIKE');

		$select->condition('per.is_instructor','Y');
		filterByUserAccessPrivileges($select, 'per','',1,'cre_usr','','per');
		expDebug::dPrintDBAPI(' $select obj = ' , $select);

		$matchingUserNames = $select->execute()->fetchAll();
		expDebug::dPrint(' $matchingUserNames = '. print_r($matchingUserNames, true) , 3);

		foreach ($matchingUserNames as $userName) {
			print $userName->full_name . "\n";
		}
	} catch (Exception $ex) {
		watchdog_exception('getInstructorAutoCompleteWithoutId', $ex);
		expertusErrorThrow($ex);
	}
}

// Function not in use
/*function multiselectEntityAssociate($type, $entityId, $entityType, $associateId){
 switch($type){
 case 'equivalence':
 addNewEquivalenceDetails($entityId, $entityType, $associateId);
 break;
 }
 }*/

function multiselectEntityDeleteAssociate($type, $entityId, $entityType, $associateId){
	try {
		switch($type){
			case 'equivalence':
				include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_equivalence.inc');
				$deleteId = deleteEquivalenceDetails($entityId, $entityType, $associateId);
				$deletedEntity = "Old Equivalence";
				break;
			case 'survey':
				include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_survey.inc');
				$deleteId = deleteSurveyDetails($entityId, $entityType, $associateId);
				$deletedEntity = "Old Survey";
				break;
			case 'assessment':
				include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_assessment.inc');
				$deleteId = deleteAssessmentDetails($entityId, $entityType, $associateId);
				$deletedEntity = "Old Assessment";
				break;
			case 'content':
				include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_content_details.inc');
				$deleteId = deleteContentDetails($entityId, $entityType, $associateId);
				$deletedEntity = "Old Content";
				break;
			case 'prerequisite':
				include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_prerequisite.inc');
				$deleteId = deletePrerequisiteDetails($entityId, $entityType, $associateId);
				$deletedEntity = "Old Prerequisite";
				break;
			case 'attachQuestion':
				include_once(drupal_get_path('module', 'exp_sp_administration_surveydetails') .'/exp_sp_administration_survey_assesment_question.inc');
				deleteSurvAssesQuestionDetails($entityId, $entityType, $associateId);
				$deletedEntity = "Old Survey Question";
				break;
			case 'session':
				include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_session_details.inc');
				$deleteId = deleteSessionDetails($entityId, $entityType, $associateId);
				$deletedEntity = "Old Session";
				break;
			case 'RoleDisplayUsers':
				include_once(drupal_get_path('module', 'exp_sp_administration_groups') .'/exp_sp_administration_groups.inc');
				$deleteId = updateRemoveUser($entityId,$associateId);
				$deletedEntity = "Old User";
				break;
				/*case 'RoleDisplayOwners':
					include_once(drupal_get_path('module', 'exp_sp_administration_groups') .'/exp_sp_administration_groups.inc');
					$deleteId = updateRemoveOwner($entityId,$associateId);
					$deletedEntity = "Old User";
					break;*/
			case 'trainings':
				include_once(drupal_get_path('module', 'exp_sp_administration_commerce_discounts') .'/exp_sp_administration_commerce_discounts_trainings.inc');
				$deleteId = deleteDiscountTrainings($entityId, $entityType, $associateId);
				$deletedEntity = "Old Discount";
				break;
				
            case 'shareuser':
				include_once(drupal_get_path('module', 'exp_sp_administration_contentauthor') .'/exp_sp_administration_contentauthor.inc');
				$deleteId = updateSharedUser($entityId,$associateId);
				$deletedEntity = "Old User";
				break;
				
				
		}
		$deleteFormState = array();
		$deleteFormState['id'] = $deleteId;
		syncSolrData('User');
		syncSolrData('Program');
		syncSolrData('Class');
		syncSolrData('Course');
		make_audit_trail_entries($deleteFormState, getIdOfLoggedInUser() , null ,$entityId, 'Deleted',$entityType,
  											'exp_sp_administration_learning.inc',$deletedEntity,array('id'));    	
	} catch (Exception $ex) {
		watchdog_exception('multiselectEntityDeleteAssociate', $ex);
		expertusErrorThrow($ex);
	}
}

function gridEditOptionSave($details, $value){
	try {
		include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_assessment.inc');
		list($id, $type, $entityId, $entityType, $index) = explode('-', $details);
		switch($type){
			case 'content':
				$result = contentSaveEditOption($details, $value);
				print drupal_json_output($result);
				break;
			case 'assessment':
			case 'survey':
			case 'mandatory_option':
				$result = assessmentSaveEditOption($details, $value);
				print drupal_json_output($result);
				break;
		}
	} catch (Exception $ex) {
		watchdog_exception('gridEditOptionSave', $ex);
		expertusErrorThrow($ex);
	}
}

function loadCtoolDataGridType($mode, $type, $searchKeyword, $entityId, $entityType, $excludedId){
	try {
	    $srcSource = getConfigValue('solr_search');
		global $theme_key, $secure_entities;
		$searchKeyword = stripslashes($searchKeyword);
		$page      = (int)$_GET['page'];
		//$skipAccess=true;
		$skipAccess=$_GET['skipAccess'];
		expDebug::dPrint('testing testing'.print_r($_GET,true),5);
		$limit     = $_GET['rows'];
		$sidxVal      = stripApostrophe(stripRegStatus($_GET['sidx']));
		if((!empty($limit) && !is_numeric($limit)) || !empty($page) && !is_numeric($page)){
			throw new Exception('Non Numeric Values');
		}

		if(!empty($sidxVal)){
			$sidx = escape_string($sidxVal);
			$_GET['sidx'] = $sidx;
		}

		$sord      = $_GET['sord'];
		if(!empty($sord)){
			if((stripos('desc',$sord) !== false)){
				$sord = 'desc';
			}else{
				$sord = 'asc';
			}
			$_GET['sord'] = $sord;
		}
		$tab       = $_GET['tab'];
		
		if($type == 'TPAttachCourse' || (($type == 'assessment' || $type == 'survey') && in_array($entityType, array('cre_sys_obt_cur','cre_sys_obt_crt','cre_sys_obt_trn','cre_sys_obt_trp')))){
			$moddet= explode('-',$entityId);
			$entityId = $moddet[0];
			$modId = $moddet[1];
		}
		expDebug::dPrint("module det " .$modId . "<<< ". $entityId. ">>>". $entityType , 5);
		/*--- URL encryption/decryption applied only the enabled modules in security module ---*/
		if (in_array($entityType, $secure_entities)) {
			$uniqueId  =  "$type-".core_encrypt($entityId)."-$entityType";
		} else {
			$uniqueId  =  "$type-".$entityId."-$entityType";
		}	
		$searchType = !empty($_GET['search_type']) ? $_GET['search_type'] : $_GET['searhType'] ;
		expDebug::dPrint(' SurAssAttachQuestion $$searchType = '.print_r($searchType,true),5);
		$queryRequired = 1;
		$fetchFields = array();

		$mode = empty($mode) ? 'view' : $mode;
		// Added by Vincent on 02, Jan 2014 for #0029687: Refresh on Admin Enrollments pages
		// Below code is handeld in exp_sp_administration_order.inc (in loadOrderAddProduct())
		// Update both file if any changes required.
		$crPage = explode('~',$_COOKIE['current_page']);
		if($mode=='view' && !empty($page) && $page==1 && $type != "trainings"){
			$isUpdate = explode('#',$crPage[2]);
			if($isUpdate[1]==1){
				$page= ($isUpdate[0])!=0?$isUpdate[0]:1;
			}
		}
		$countAvailable = 0;
		$count = 0;
		if($type == 'SurAssAttachQuestion') {
			$searchKeyword = arg(4, request_path());
		}
		expDebug::dPrint(' $searchKeyword = '.$searchKeyword);
		$searchKeyword= str_replace(EXP_AC_SEPARATOR,'/',$searchKeyword);


		switch($type){

			case 'equivalence':
				$xorderBy = 'title';
				include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_equivalence.inc');
				/*
				 * 0024925: Enforce equivalence in training plans
				 * for this ticket we created equivalence option for training plan
				 * don't delete it. Its may need for feature enhancement
				 * here we change this switch case only. don't delete commanded line
				 */
				/*$select = db_select('slt_program', 'program');
				 $select->addField('program', 'object_type', 'object_type');
				 $select->distinct();
				 $objectTypes = $select->execute()->fetchCol();
				 expDebug::dPrint('object types = '.print_r($objectTypes,true),5);
				 */
				if($mode == 'view'){
					//if(!in_array($entityType,$objectTypes))
					$listquery = fetchEquivalenceList($entityId, $entityType, $queryRequired);
					// else
					//   $listquery = fetchEquivalenceListTP($entityId, $entityType, $queryRequired);
				} else {
					// if(!in_array($entityType,$objectTypes))
					$listquery = loadCtoolDataGridForEquivalence($searchKeyword, $queryRequired, $excludedId,$searchType);
					// else
					//   $listquery = loadCtoolDataGridForEquivalenceTp($searchKeyword, $queryRequired, $excludedId,$searchType);
				}
				break;


			case 'TPAttachCourse':
				if($srcSource == 0){
					$xorderBy = 'title';
					include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_attach_course.inc');
					$listquery = loadAttachCourseForTP($searchKeyword, $queryRequired,'' ,$modId,$entityId);
				}else {
			        require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_catalog.php';
			        try{
			            $solrObj = new CatalogSolrSearch();
						$search_results = $solrObj->getSolrSearchResultTPAttachCourse($excludedId);
						$listquery = $search_results['records'];
			            $count = $search_results['recCount'];
			            $totalPages = ceil($search_results['recCount']/$_GET['rows']);
			        }catch(Exception $e){
			            expDebug::dPrint("Error in TP Attach course -- ".print_r($e->getMessage(),true),1);
			        }
			    }
				break;

			case 'CountrySetting':
				$xorderBy = 'name';
				$listquery = loadCountrySetting($searchKeyword, $queryRequired, $excludedId);
				break;

			case 'PaymentMethod':

				$listquery = loadPaymentMethod();

				break;

			case 'SurAssAttachQuestion':
				include_once(drupal_get_path('module', 'exp_sp_administration_surveydetails') .'/exp_sp_administration_survey_assesment_attach_question.inc');
				$listquery = loadAttachQuestionForSurvAss($searchKeyword, $queryRequired, $excludedId,$entityType,$searchType);
				$xorderBy = 'question_txt';
				$select1  = db_select('slt_survey_questions', 'surveyQstion');
				$select1->addExpression('count(surveyQstion.id)', 'surveycount');
				$select1->condition('surveyQstion.question_txt', '%' . db_like($searchKeyword) . '%', 'LIKE');
				$select1->condition('surveyQstion.status', 'sry_qtn_sts_atv', '=');
				$select1->condition('surveyQstion.survey_type', $entityType);
				$select1->condition('surveyQstion.id', explode(",",$excludedId) , 'IN');
				expDebug::dPrintDBAPI(' SurAssAttachQuestion Qry1 Velu = ', $select1);
				$surveycount = $select1->execute()->fetchField();
				expDebug::dPrint(' SurAssAttachQuestion Count1 Velu = ' . $surveycount , 4);

				$select2  = db_select('slt_survey_questions', 'surveyQstion');
				$select2->addExpression('count(surveyQstion.id)', 'surveycount1');
				$select2->condition('surveyQstion.question_txt', $searchKeyword , '=');
				$select2->condition('surveyQstion.status', 'sry_qtn_sts_atv', '=');
				$select2->condition('surveyQstion.survey_type', $entityType);
				$select2->condition('surveyQstion.id', explode(",",$excludedId) , 'IN');
				expDebug::dPrintDBAPI(' SurAssAttachQuestion Qry2 Velu = ', $select2);
				$surveycount1 = $select2->execute()->fetchField();
				expDebug::dPrint(' SurAssAttachQuestion Count2 Velu = ' . $surveycount1, 4);

				break;

			case 'survey':
				include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_survey.inc');
				$xorderBy = 'title';
				if($mode == 'view'){
					$listquery = fetchSurveyList($entityId, $entityType, $queryRequired,$modId);
				} else {
					$listquery = loadCtoolDataGridForSurvey($searchKeyword, $queryRequired, $excludedId,$entityId, $entityType);
				}
				break;
				case 'shareuser':
					include_once(drupal_get_path('module', 'exp_sp_administration_contentauthor') .'/exp_sp_administration_contentauthor.inc');
					include_once(drupal_get_path('module', 'exp_sp_administration_catalog') .'/exp_sp_administration_catalog.inc');
					include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_class_register.inc');
					if($mode == 'view'){
						$xorderBy = '';
						$countAvailable = 1;
						$sidx = "";
						/* -- #46023: UI -Pagination Issue --*/
						$listquery = fetchShareUserList($courseId,$entityId, $entityType, $queryRequired, $searchKeyword,'','','','view');
						$count = $listquery->countQuery()->execute()->fetchField();
						//$enrollStatistics = fetchEnrollUserListStatusCount($courseId, $entityId, $entityType, $queryRequired, $searchKeyword);
					}
					else {
						$xorderBy = 'full_name';
						$listquery = loadCtoolDataGridForEnrollUser1($searchKeyword, $queryRequired, $entityId,'','','');
						$count = loadCtoolDataGridForEnrollUser1($searchKeyword, $queryRequired, $entityId, 1,'','');
						if(!empty($queryRequired)){
							$count = $count->execute()->fetchField();
						}
						/*$select = db_select('slt_enrollment','enr');
						 $select->join('slt_person', 'per', 'per.id = enr.user_id');
						$select->addExpression('count(enr.id)', 'enrcount');
						$select->condition('per.user_name', '%' . db_like($searchKeyword) . '%', 'LIKE');
						$select->condition('enr.reg_status',array('lrn_crs_reg_cnf','lrn_crs_reg_ppm', 'lrn_crs_reg_wtl'));
						$select->condition('enr.class_id',$entityId,'=');
						expDebug::dPrintDBAPI('$select encount Velu 1 = ', $select);
						$encount = $select->execute()->fetchField();

						$select1 = db_select('slt_enrollment','enr');
						$select1->join('slt_person', 'per', 'per.id = enr.user_id');
						$select1->addExpression('count(enr.id)', 'enrcount');
						$select1->condition('per.user_name',$searchKeyword, '=');
						$select1->condition('enr.reg_status',array('lrn_crs_reg_cnf','lrn_crs_reg_ppm', 'lrn_crs_reg_wtl'));
						$select1->condition('enr.class_id',$entityId,'=');
						expDebug::dPrintDBAPI('$select encount Velu 2 = ', $select1);
						$encount1 = $select1->execute()->fetchField();*/
				
						$countAvailable = 1;
					}
					break;
				
				
				
			case 'assessment':
				include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_assessment.inc');
				$xorderBy = 'title';
				if($mode == 'view'){
					$listquery = fetchAssessmentList($entityId, $entityType, $queryRequired);
				} else {
					$listquery = loadCtoolDataGridForAssessment($searchKeyword, $queryRequired, $excludedId,$entityId, $entityType);
				}
				break;

			case 'enrolluser':
				include_once(drupal_get_path('module', 'exp_sp_administration_catalog') .'/exp_sp_administration_catalog.inc');
				include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_class_register.inc');
				$courseDet=db_query("select course_id,is_compliance,delivery_type from slt_course_class WHERE id = '".$entityId."'");
				$courseDet = $courseDet->fetchAll();
				$courseId = $courseDet[0]->course_id;

				$is_compliance = ($courseDet[0]->is_compliance == 1) ? $courseDet[0]->is_compliance : 0;
				if($mode == 'view'){
				  	$xorderBy = 'Date';
					if($sidx == 'Status'){$sidx = 'status';}/*Viswanathan added for #68932*/
					$countAvailable = 1;
					/* -- #46023: UI -Pagination Issue --*/
					if($srcSource == 0){
						$listquery = fetchEnrollUserList($courseId,$entityId, $entityType, $queryRequired, $searchKeyword,'','','','view');
						$select1 = db_select($listquery);
						$result[0] = new stdclass();
						$result[0]->enrollment_count= $select1->countQuery()->execute()->fetchField();
						$count = 	$result[0]->enrollment_count;
					} else {
						require_once $_SERVER['DOCUMENT_ROOT'] . '/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user_enrollments.php';
						try {
							$solrObj = new UserEnrollments();
							$listqueryResult = $solrObj->classEnrollments($entityId,$entityType,$searchKeyword,'enrollments',$enrType='class');
							$listquery = $listqueryResult['result'];
							expDebug::dPrint('Result set from solr'.print_r($listqueryResult,true),5);
							$count = $listqueryResult['count'];
						} catch (Exception $e) {
							expDebug::dPrint( "Error in Solr Search for Users - " . print_r($e->getMessage(), true), 1);
						}
					}
				
					//$enrollStatistics = fetchEnrollUserListStatusCount($courseId, $entityId, $entityType, $queryRequired, $searchKeyword);
				}
				else {
					$xorderBy = 'full_name';
					$srcSource = getConfigValue('solr_search');
					if($srcSource == 0){
						$xorderBy = 'full_name';
						$listquery = loadCtoolDataGridForEnrollUser($searchKeyword, $queryRequired, $entityId,'',$is_compliance,$courseDet[0]->delivery_type,$skipAccess);
						$count = loadCtoolDataGridForEnrollUser($searchKeyword, $queryRequired, $entityId, 1,$is_compliance,$courseDet[0]->delivery_type,$skipAccess);
						if(!empty($queryRequired)){
							$count = $count->execute()->fetchField();
						}
						$select = db_select('slt_enrollment','enr');
						$select->join('slt_person', 'per', 'per.id = enr.user_id');
						$select->addExpression('count(enr.id)', 'enrcount');
						$select->condition('per.user_name', '%' . db_like($searchKeyword) . '%', 'LIKE');
						$select->condition('enr.reg_status',array('lrn_crs_reg_cnf','lrn_crs_reg_ppm', 'lrn_crs_reg_wtl'));
						$select->condition('enr.class_id',$entityId,'=');
						expDebug::dPrintDBAPI('$select encount Velu 1 = ', $select);
						$encount = $select->execute()->fetchField();
	
						$select1 = db_select('slt_enrollment','enr');
						$select1->join('slt_person', 'per', 'per.id = enr.user_id');
						$select1->addExpression('count(enr.id)', 'enrcount');
						$select1->condition('per.user_name',$searchKeyword, '=');
						$select1->condition('enr.reg_status',array('lrn_crs_reg_cnf','lrn_crs_reg_ppm', 'lrn_crs_reg_wtl'));
						$select1->condition('enr.class_id',$entityId,'=');
						expDebug::dPrintDBAPI('$select encount Velu 2 = ', $select1);
						$encount1 = $select1->execute()->fetchField();
	
						$countAvailable = 1;
				}
					else {
					    try{
					    	require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user.php';
					        $solrObj = new UserSolrSearch();
					        $search_results = $solrObj->getSolrSearchResultEnrollUser($searchKeyword,$searchType,$entityId,$enrPage='class');
					        expDebug::dPrint('Result set from solr main result'.print_r($search_results,true),5);
					        $listquery = $search_results['records'];
					        $count = $search_results['recCount'];
					    }catch(Exception $e){
					        expDebug::dPrint("Error in Solr Search -- ".print_r($e->getMessage(),true),1);
					    }
					}
					
				}
				break;

			case 'enrolltpuser':
				include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_tp_register.inc');
				if($mode == 'view'){
					if($srcSource == 0){
						$xorderBy = 'Date';
						if($sidx == 'Status'){$sidx = 'overall_status_name';}
						$countAvailable = 1;
						$enrolluserDetails = fetchEnrollTPUserList($entityId, $entityType,'',$searchKeyword,'','','','',1);
						$count = $enrolluserDetails[0]->cnt;
						$listquery = fetchEnrollTPUserList($entityId, $entityType, $queryRequired, $searchKeyword,'','','','view');
					}
					else {
						require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user_enrollments.php';
						try{
							$solrObj = new UserEnrollments();
							$getlpQryRes = $solrObj->TPEnrollments($entityId,$entityType,$searchKeyword,'enrollments',$enrType='tp');
							$listquery = $getlpQryRes['result'];
							$count = $getlpQryRes['count'];
						}catch(Exception $e){
							expDebug::dPrint("Error in my program Solr Search -- ".print_r($e->getMessage(),true),1);
						}
					}
				}
				else {
					$xorderBy = 'full_name';
					$srcSource = getConfigValue('solr_search');
					// Solr implementation
					if($srcSource == 0){
						$xorderBy = 'full_name';
						$listquery = (strpos($excludedId,'R') > 0) ? loadCtoolDataGridForEnrollTPUser($searchKeyword, $queryRequired, $entityId,'',$excludedId,$skipAccess) : loadCtoolDataGridForEnrollTPUser($searchKeyword, $queryRequired, $entityId,'','',$skipAccess);
						$count = (strpos($excludedId,'R') > 0) ? loadCtoolDataGridForEnrollTPUser($searchKeyword, $queryRequired, $entityId,1,$excludedId,$skipAccess) : loadCtoolDataGridForEnrollTPUser($searchKeyword, $queryRequired, $entityId, 1,'',$skipAccess);
						if(!empty($queryRequired)){
							$count = $count->execute()->fetchField();
						}
						$countAvailable = 1;
						$select = db_select('slt_master_enrollment','menr');
						$select->join('slt_person', 'per', 'per.id = menr.user_id');
						$select->addExpression('count(menr.id)', 'menrcount');
						$select->condition('per.user_name', '%' . db_like($searchKeyword) . '%', 'LIKE');
						if(strpos($excludedId,'R') > 0){
							$select->condition('menr.overall_status','lrn_tpm_ovr_exp','=');
						}else{
							$select->condition('menr.overall_status',array('lrn_tpm_ovr_cnf', 'lrn_tpm_ovr_ppm', 'lrn_tpm_ovr_wtl','lrn_tpm_ovr_cmp','lrn_tpm_ovr_enr'));
						}
						$select->condition('menr.program_id',$entityId,'=');
						expDebug::dPrintDBAPI('$select mencount', $select);
						$encount = $select->execute()->fetchField();
	
						$select1 = db_select('slt_master_enrollment','menr');
						$select1->join('slt_person', 'per', 'per.id = menr.user_id');
						$select1->addExpression('count(menr.id)', 'menrcount');
						$select1->condition('per.user_name',$searchKeyword,'=');
						if(strpos($excludedId,'R') > 0){
							$select1->condition('menr.overall_status','lrn_tpm_ovr_exp','=');
						}else{
							$select1->condition('menr.overall_status',array('lrn_tpm_ovr_cnf', 'lrn_tpm_ovr_ppm', 'lrn_tpm_ovr_wtl','lrn_tpm_ovr_cmp','lrn_tpm_ovr_enr'));
						}
						$select1->condition('menr.program_id',$entityId,'=');
						expDebug::dPrintDBAPI('$select mencount', $select1);
						$encount1 = $select1->execute()->fetchField();
					}
					else {
					     try{
						   require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user.php';
				           $solrObj = new UserSolrSearch();
					       $search_results = $solrObj->getSolrSearchResultEnrollUser($searchKeyword,$searchType,$entityId,$enrPage='tp');
			               $listquery = $search_results['records'];
			               $count = $search_results['recCount'];					
					       }catch(Exception $e){
					       expDebug::dPrint("Error in class enrollments solr Search -- ".print_r($e->getMessage(),true),1);
					   }
					  }
				}
				break;
			case 'grpAddUsers':
		       $xorderBy = 'full_name';
				$type='grpAddUsers';
				expDebug::dPrint('grpAddUsers ---> '.$entityId ." search keyword".$searchKeyword,4);
				if($srcSource == 0){
					$listquery = fetchGrpUserList($entityId,$searchKeyword,$type);
					$countquery = fetchGrpUserList($entityId,$searchKeyword,$type,1);
					$count = $countquery->execute()->fetchField();
				}
				else{
                    require_once $_SERVER['DOCUMENT_ROOT'] . '/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user.php';
                    try {
                        $solrObj = new UserSolrSearch();
                        $solrList = $solrObj->getSolrSearchResultGroupAddUsers($entityId,$searchType);
                        $listquery = $solrList['records'];
                        expDebug::dPrint('Result set from solr'.print_r($listquery,true),5);
                        $count = $solrList['recCount'];
                       // $start = getOffsetOfFirstRecordInPage($page,$total_pages, $limit, $countquery);
                    } catch (Exception $e) {
                        expDebug::dPrint( "Error in Solr Search for Users - " . print_r($e->getMessage(), true), 1);
                               
                    }
				}
				break;
				/*case 'grpAddOwners':
				 expDebug::dPrint("dfsdfds");
				 $xorderBy = 'full_name';
				 $type='grpAddOwners';
				 expDebug::dPrint('grpAddOwners ---> '.$entityId ." search keyword".$searchKeyword);
				 $listquery = fetchGrpOwnerUserList($entityId,$searchKeyword,$type);
				 $countquery = fetchGrpOwnerUserList($entityId,$searchKeyword,$type,1);
				 	
				 $count = $countquery->execute()->fetchField();
				 break;*/
			case 'Permissions':
				expDebug::dPrint('fetchPermissionsList --->'.$entityId ." ------ entity_id".$entityType."---entity_type",4);
				$listquery = fetchPermissionsList($entityId);
				break;

			case 'addkeywords':
				include_once(drupal_get_path('module', 'exp_sp_administration_notification_template') .'/exp_sp_administration_notification_template.inc');
				$xorderBy = 'keywords';
				$listquery = fetchKeywordsList($searchKeyword);
				$countAvailable = 1;

				break;

			case 'content':
				include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_content_details.inc');
				$courseDet=db_query("select course_id, delivery_type from slt_course_class WHERE id = '".$entityId."'");
				$courseDet = $courseDet->fetchAssoc();
				$courseId = $courseDet['course_id'];
				$deliveryType = $courseDet['delivery_type'];
				$xorderBy = '';//h5pcustomize since union added orderby provided in loadCtoolDataGridForContent method
				if($mode == 'view'){
				$xorderBy = 'code';
					$listquery = fetchContentList($courseId,$entityId, $entityType, $queryRequired);
				} else {
					$xorderBy = '';
					$listquery = loadCtoolDataGridForContent($searchKeyword, $deliveryType, $queryRequired, $excludedId);
				}
				break;

			case 'prerequisite':
				include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_prerequisite.inc');
				if($mode == 'view'){
					$listquery = fetchPrerequisiteList($entityId, $entityType, $queryRequired);
				} else {
					$listquery = loadCtoolDataGridForPrerequisite($searchKeyword, $queryRequired, $excludedId, $entityType,$searchType);
				}
				break;

			case 'attachQuestion':
				include_once(drupal_get_path('module', 'exp_sp_administration_surveydetails') .'/exp_sp_administration_survey_assesment_question.inc');
				$xorderBy = 'question_txt';
				if($mode == 'view'){
					$listquery     = getAttachQuestion($entityId,0,$queryRequired);
					//$listquery = loadCtoolDataGridForQuestionList($entityId,0,$queryRequired);
				} else {
					//$listquery = loadCtoolDataGridForQuestionList($entityId,0,$queryRequired,$excludedId);
					$listquery = loadCtoolDataGridForQuestionList($searchKeyword, $queryRequired, $excludedId, $entityType);//($searchKeyword, $queryRequired, $excludedId, $entityType);
				}
				 
				break;

			case 'session':
				include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_session_details.inc');
				$xorderBy = 'start_date';
				$listquery = _getSessionDetails($entityId, $queryRequired);
				break;

			case 'trainings':
				include_once(drupal_get_path('module', 'exp_sp_administration_commerce_discounts') .'/exp_sp_administration_commerce_discounts_trainings.inc');
				if($mode == 'view'){
					$listquery = fetchAlreadyAddedTrainings($entityId, $entityType,$searchType,$searchKeyword, 'dfgdf');
				} else {
					$listquery = fetchToBeAddedTrainings($entityId,$entityType,$searchType,$searchKeyword, 'dfgdfg');
				}
				break;
				
			case 'enrolltprecertify':
				include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_tp_register.inc');
				//$xorderBy = 'path';
				$xorderBy = 'id';
				if($sidx == 'Status'){
					$xorderBy = 'overall_status';
				}elseif($sidx == 'CompletionDate'){
					$xorderBy = 'comp_date';
				}elseif($sidx == 'EnrollmentDate'){
					$xorderBy = 'reg_date';
				}
				$sidx = '';
				$listquery = fetchEnrollmentHistory($entityId, $entityType,$excludedId);
				break;	
		}
		 

		if($type != "Permissions" && $type != 'prerequisite' && $type !='PaymentMethod' && $type != 'trainings') {
		    //expDebug::dPrint('type not pe,pr,pa,tr');
			if($srcSource == 0) {  // Need to remove if condition. Once solr implemented fully
				if($srcSource == 1 && ($type == 'TPAttachCourse' || $type =='grpAddUsers' || $type == 'enrolluser' || $type == 'enrolltpuser' )) {
					// to do 
				} else {
					if($sidx !=''){
						$xorderBy = checksortby($type,$sidx);
					}
					if($xorderBy != ""){
						$listquery->orderBy($xorderBy,$sord);
						expDebug::dPrintDBAPI('query for $listquery', $listquery);
					}
				}
			} 
		}
		if($countAvailable == 0 && $type != 'grpAddUsers' && $type != 'prerequisite'){
			if($type =='PaymentMethod'){
				$count = count($listquery);
			}else{
				if($srcSource == 1 && ($type == 'TPAttachCourse' || $type =='grpAddUsers' || $type == 'enrolluser' || $type == 'enrolltpuser' )) {
			    	$count = $count;
				} else {
				$count = count($listquery->execute()->fetchAll());
			}
		}
		}
		/* } */
		 

		/* Paging Related */
		$start = $limit * $page - $limit;
		if($mode == 'view' && $type == 'trainings'){
			$listquery = fetchAlreadyAddedTrainings($entityId, $entityType,$searchType,$searchKeyword, 'dfgdf','',$start, $limit);
		} else if($mode == 'edit' && $type == 'trainings') {
			$listquery = fetchToBeAddedTrainings($entityId,$entityType,$searchType,$searchKeyword, 'dfgdfg',$start, $limit);
		}
		if($type !='PaymentMethod' && $type != 'prerequisite' ){
				if (module_exists('exp_sp_administration_contentauthor') && $type !='trainings'){// h5pcustomize - check with thamil
					if($srcSource == 1 && ($type == 'TPAttachCourse' || $type =='grpAddUsers' || $type == 'enrolluser' || $type == 'enrolltpuser')) {
						$queryResult = $listquery;
					} else {
						$args = $listquery->getArguments();	
						$listquery .= ' LIMIT ' . $limit .' ' . 'OFFSET ' . $start ;
						$queryResult = db_query($listquery, $args);
						expDebug::dPrintDBAPI('content Author list query print', $queryResult);
					}
			 }
			 else if($type !='trainings'){
			 	if($srcSource == 1 && ($type == 'TPAttachCourse' || $type =='grpAddUsers' || $type == 'enrolluser' || $type == 'enrolltpuser')) {
			 		$queryResult = $listquery;
			 	}  else {
				// Execute query
				 $listquery->range($start, $limit);
				 $queryResult = $listquery->execute();
				 expDebug::dPrintDBAPI('List query Printed',$listquery);
			 	}
			 }else{
			 		$queryResult = $listquery->execute();
			 }
			}else if($type == 'prerequisite' ) {
			$args = $listquery->getArguments();
			$query = (string) $listquery;
// 			foreach($args as $argAlias=>$argVal){
// 				$query = preg_replace("/".$argAlias."\b/","'$argVal'",$query);
// 				$query = preg_replace("/[{}]/","",$query);
// 			}
			$resQry  = 'SELECT SQL_CALC_FOUND_ROWS * From ('. $query;
			$resQry .= ") prerequestset";
			$resQry .= " LIMIT  $start, $limit";
			$queryResult = db_query($resQry, $args);
			$totRecQry  = db_query('select found_rows() as tot_records');
			$count  = $totRecQry->fetchField();
			expDebug::dPrintDBAPI('after execution prerequest list query print', $listquery);
			
		}else{
				$queryResult = $listquery;
			}
		if( $count >0 ){
			$total_pages = ceil($count/$limit);
		}else{
			$total_pages = 0;
		}
		if($page > $total_pages)
		$page = $total_pages;
		if($start < 0) {
			$start = 0;
		}
		$srch_result_response_roster->total   = $total_pages;
		$srch_result_response_roster->records = $count;
		$srch_result_response_roster->page    = $page;
		$srch_result_response_roster->unique_id    = $uniqueId;
		$srch_result_response_roster->msg    = '';

		$_SESSION['jq_count'] = array();
		$i=0;


		switch($type){

			case 'equivalence':

				foreach ($queryResult as $rec_search_results)
				{
					$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->id;
					$srch_result_response_roster->rows[$i]['cell'] = array(
        	 'id' 	              => $rec_search_results->id,
					/*'CourseTitle' 	      => $rec_search_results->title,
					 'CourseCode' 	      => $rec_search_results->code,*/
             		'title' 	      => '<span class="vtip" title="'.sanitize_data($rec_search_results->title).'">'.titleController('MULTI-GRID-EQUIVALENCE-TITLE', sanitize_data($rec_search_results->title),70).'</span>',
             		'code' 	      => '<span class="vtip" title="'.sanitize_data($rec_search_results->code).'">'.titleController('MULTI-GRID-EQUIVALENCE-CODE', sanitize_data($rec_search_results->code),10).'</span>',
             'MultiselectCheck'   => $rec_search_results->id,
					);;
					$i++;
				}
				break;

			case 'TPAttachCourse':

				foreach ($queryResult as $rec_search_results)
				{
					$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->id;
					$srch_result_response_roster->rows[$i]['cell'] = array(
        	 'id' 	              => $rec_search_results->id,
						'title' 	      	=> '<span class="vtip" title="'.sanitize_data($rec_search_results->title).'">'.titleController('MULTI-GRID-TPATTACHCOURSE-TITLE', sanitize_data($rec_search_results->title),30).'</span>',
						'code' 	      		=> '<span class="vtip" title="'.sanitize_data($rec_search_results->code).'">'.titleController('MULTI-GRID-TPATTACHCOURSE-CODE', sanitize_data($rec_search_results->code),10).'</span>',
             'MultiselectCheck'   => $rec_search_results->id,
					);;
					$i++;
				}
				break;

			case 'CountrySetting':

				foreach ($queryResult as $rec_search_results)
				{
					($rec_search_results->version > 0) ? $action=t('LBL920'): $action=t('LBL919');
					$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->id;
					$srch_result_response_roster->rows[$i]['cell'] = array(
							'name' 	      => '<span class="vtip" title="'.sanitize_data($rec_search_results->name).'">'.titleController('MULTI-GRID-COUNTRYSETTING-TITLE', sanitize_data($rec_search_results->name),30).'</span>',
							'code' 	      => '<span class="vtip" title="'.sanitize_data($rec_search_results->code).'">'.titleController('MULTI-GRID-COUNTRYSETTING-CODE', sanitize_data($rec_search_results->code),10).'</span>',
             'ActionList' 		  => '<span class="countryaction"><a href="javascript:void(0);" id ='.$rec_search_results->code.'-'.$rec_search_results->id.' onclick="$(\'body\').data(\'mulitselectdatagrid\').countryAction(\''.$rec_search_results->code.'\',\''.$rec_search_results->id.'\'); return false">'.$action.'</a></span>',	
					 
					);
					$i++;
				}
				break;

			case 'PaymentMethod':
				$j=1;
				foreach ($queryResult as $rec_search_results)
				{
					$rec_search_results = array_to_object($rec_search_results);
					// print_r($rec_search_results);
					$enablevalue = variable_get('uc_payment_method_' . $rec_search_results->id . '_checkout', $rec_search_results->checkout);
					($enablevalue == 1) ? $action='Disable': $action='Enable';
					$rec_search_results->object_type = 'paymentmethod';
					$rec_search_results->status_code = $enablevalue;
					$srch_result_response_roster->rows[$i]['id']   =  $j; /*$rec_search_results->name;*/
					$srch_result_response_roster->rows[$i]['cell'] = array( 'CourseTitle' => theme('narrow-search-results-item-image', array('results' => $rec_search_results)),
                                         'CourseCode' => getPaymentMethodHTML($rec_search_results),
                                         'ActionList' => getPaymentActionsHTML($rec_search_results)
					);
					$i++;
					$j++;
				}
				break;
			case 'grpAddUsers':

				if($mode == 'view'){
					// $userStatistics = fetchUserDetailList($listquery);
					expDebug::dPrint('grid data grpAddUsers  ---> '.print_r($listquery,true),4);

					foreach ($queryResult as $rec_search_results)
					{
						$srch_result_response_roster->rows[$i]['id']   = $i;
						$srch_result_response_roster->rows[$i]['cell'] = array(
								'id' 	      => $rec_search_results -> user_id,
								'user_name'   => $rec_search_results-> uname,
								'full_name'   => $rec_search_results-> fname,
								'Status'      => $rec_search_results-> status,
								'Remove'      => $rec_search_results-> user_id,
						);
						$i++;
					}
				}else {
					expDebug::dPrint('printing result ---> '.print_r($queryResult,true),4);

					foreach ($queryResult as $rec_search_results){
						$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->user_id;
						$srch_result_response_roster->rows[$i]['cell'] = array(
								'id' 	           => $rec_search_results->user_id,
								'user_name' 	   => $rec_search_results->uname,
								'full_name' 	   => $rec_search_results->fname,
								'MultiselectCheck' => $rec_search_results->user_id,
						);
						$i++;
					}
				}


				break;

			/*case 'grpAddOwners':
				
					if($mode == 'view'){
						// $userStatistics = fetchUserDetailList($listquery);
						expDebug::dPrint('grid data grpAddOwners  ---> '.print_r($listquery,true),4);
				
						foreach ($queryResult as $rec_search_results)
						{
							$srch_result_response_roster->rows[$i]['id']   = $i;
							$srch_result_response_roster->rows[$i]['cell'] = array(
									'id' 	              => $rec_search_results -> user_id,
									'user_name' 	        => $rec_search_results-> uname,
									'full_name' 	        => $rec_search_results-> fname,									
									'name'                =>$rec_search_results->gname,
									'Status'             => $rec_search_results-> status,
									'Remove'             => $rec_search_results-> user_id,
							);
							$i++;
						}
					}else {
					expDebug::dPrint('printing result ---> '.print_r($queryResult,true));


					foreach ($queryResult as $rec_search_results){
						$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->user_id;
						$srch_result_response_roster->rows[$i]['cell'] = array(
          	   'id' 	            => $rec_search_results->user_id,
               'user_name' 	      => $rec_search_results->uname,
               'full_name' 	      => $rec_search_results->fname,
									'name' 	          => $rec_search_results->gname,
									//'name' =>titleController(' ', $rec_search_results->gname,20),
               'MultiselectCheck' => $rec_search_results->user_id,
						);
						$i++;
					}
						expDebug::dPrint("resut ".print_r($srch_result_response_roster,true),5);
				}


				break; */

			case 'Permissions':
				 
				expDebug::dPrint('grid data permissions ---> '.print_r($queryResult,true),4);
				foreach ($queryResult as $rec_search_results){
					$srch_result_response_roster->rows[$i]['id']   = $i;
					$srch_result_response_roster->rows[$i]['cell'] = array(
							'id'   			 	=>$rec_search_results->perm_id,
							'name'  			=>$rec_search_results->pname,
							'MultiselectCheck' 	=> $rec_search_results->perm_id,
					);
					$i++;
				}

				break;
			case 'addkeywords':
				/* send data to add keywords qtip grid */
				expDebug::dPrint('grid data Addkeywords'.print_r($queryResult,true),4);
				foreach ($queryResult as $rec_search_results){
					$srch_result_response_roster->rows[$i]['id']   = $i;
					$srch_result_response_roster->rows[$i]['cell'] = array(
							//'id'                 => $rec_search_results->kid,
							'keywords' 	      	  => $rec_search_results->keywords,
        			'keyworddescription' 	  => '<span class="vtip" title="'.sanitize_data($rec_search_results->keyworddescription).'">'.titleController('MULTI-GRID-ADDKEYWORDS-TITLE', $rec_search_results->keyworddescription,25).'</span>',
							'AddToTemplate' 	  => '<span>insert</span>',
					);
					$i++;
				}
				break;
				 
			case 'SurAssAttachQuestion':

				foreach ($queryResult as $rec_search_results)
				{
					$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->id;
					$srch_result_response_roster->rows[$i]['cell'] = array(
							'id' 	             => $rec_search_results->id,
		             'question_txt' 	  => '<span class="vtip" title="'.filterSpecialCharacters($rec_search_results->question_txt).'">'.titleController('MULTI-GRID-SURASS-ATTACH-QUESTION-TITLE', $rec_search_results->question_txt,30).'</span>',
		             'QuestionCode' 	  => '<span class="vtip" title="'.filterSpecialCharacters($rec_search_results->question_code).'">'.titleController('MULTI-GRID-SURASS-ATTACH-QUESTION-CODE', $rec_search_results->question_code,10).'</span>',
							'MultiselectCheck'   => $rec_search_results->id,
					);;
					$i++;
				}
				$entityType = ($entityType == 'sry_det_typ_sry') ? 'Survey' : 'Assessment';
				if($surveycount>=1 && $surveycount1>=1){
					$srch_result_response_roster->msg  = t('This question is already attached to this ').$entityType;
				}else{
					$srch_result_response_roster->msg  = t('MSG403');
				}

				break;
			case 'survey':
				foreach ($queryResult as $rec_search_results)
				{
					$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->id;
					if($mode == 'view'){
						if($entityType != 'cre_sys_obt_cls'){
						$srch_result_response_roster->rows[$i]['cell'] = array(
								'id' 	            => $rec_search_results->id,
								//'SurveyName' 	    => $rec_search_results->title,
    					'title' 	      => '<span class="vtip" title="'.htmlentities($rec_search_results->title,ENT_QUOTES,"UTF-8").'">'.titleController('MULTI-GRID-SURVEY-VIEW-TITLE', $rec_search_results->title,45).'</span>',
								//'SurveyCode' 	    => $rec_search_results->code,
    					'code' 	      => '<span class="vtip" title="'.htmlentities($rec_search_results->code,ENT_QUOTES,"UTF-8").'">'.titleController('MULTI-GRID-SURVEY-VIEW-CODE', $rec_search_results->code,10).'</span>',
								'view_option' 	    => $rec_search_results->view_option == 'V' ? t('LBL663') : t('LBL662'),
									'path' 	      => '<span class="vtip" title="'.htmlentities($rec_search_results->path,ENT_QUOTES,"UTF-8").'">'.titleController('MULTI-GRID-SURVEY-VIEW-PATH', $rec_search_results->path,45).'</span>',
								'MultiselectCheck'  => $rec_search_results->id,
						);
					} else {
						$srch_result_response_roster->rows[$i]['cell'] = array(
								'id' 	             => $rec_search_results->id,
								//'SurveyName' 	     => $rec_search_results->title,
    					'title' 	      => '<span class="vtip" title="'.htmlentities($rec_search_results->title,ENT_QUOTES,"UTF-8").'">'.titleController('MULTI-GRID-SURVEY-VIEW-TITLE', $rec_search_results->title,45).'</span>',
								//'SurveyCode' 	    => $rec_search_results->code,
    					'code' 	      => '<span class="vtip" title="'.htmlentities($rec_search_results->code,ENT_QUOTES,"UTF-8").'">'.titleController('MULTI-GRID-SURVEY-VIEW-CODE', $rec_search_results->code,10).'</span>',
								'view_option' 	    => $rec_search_results->view_option == 'V' ? t('LBL663') : t('LBL662'),
									'path' 	      => '',
								'MultiselectCheck'  => $rec_search_results->id,
						);
						}
						
					} else {
						$srch_result_response_roster->rows[$i]['cell'] = array(
								'id' 	             => $rec_search_results->id,
								//'SurveyName' 	     => $rec_search_results->title,
    					'title' 	      => '<span class="vtip" title="'.htmlentities($rec_search_results->title,ENT_QUOTES,"UTF-8").'">'.titleController('MULTI-GRID-SURVEY-EDIT-TITLE', $rec_search_results->title,45).'</span>',
								//'SurveyCode' 	     => $rec_search_results->code,
    					'code' 	      => '<span class="vtip" title="'.htmlentities($rec_search_results->code,ENT_QUOTES,"UTF-8").'">'.titleController('MULTI-GRID-SURVEY-EDIT-CODE', $rec_search_results->code,10).'</span>',
								'MultiselectCheck'   => $rec_search_results->id,
						);
					}
					$i++;
				}
				 
				break;
			case 'assessment':
				foreach ($queryResult as $rec_search_results)
				{
					$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->id;
					if($mode == 'view'){
						$srch_result_response_roster->rows[$i]['cell'] = array(
								'id' 	           => $rec_search_results->id,
								//'SurveyName' 	   => $rec_search_results->title,
    					'title' 	      => '<span class="vtip" title="'.htmlentities($rec_search_results->title,ENT_QUOTES,"UTF-8").'">'.titleController('MULTI-GRID-ASSESSMENT-VIEW-TITLE', $rec_search_results->title,20).'</span>',
								//'SurveyCode' 	   => $rec_search_results->code,
    					'code' 	      => '<span class="vtip" title="'.htmlentities($rec_search_results->code,ENT_QUOTES,"UTF-8").'">'.titleController('MULTI-GRID-ASSESSMENT-VIEW-CODE', $rec_search_results->code,13).'</span>',
								'no_of_attempts'   => $rec_search_results->no_of_attempts,
								'pre_status' 	   => $rec_search_results->pre_status,
								'preview_option' 	   => $rec_search_results->preview_option,
								'view_option' 	   => $rec_search_results->view_option == 'V' ? t('LBL663') : t('LBL662'),
								'path' 	      => '<span class="vtip" title="'.htmlentities($rec_search_results->path,ENT_QUOTES,"UTF-8").'">'.titleController('MULTI-GRID-ASSESSMENT-VIEW-PATH', $rec_search_results->path,45).'</span>',
								'module' 	   => $rec_search_results->module_id,
								'MultiselectCheck' => $rec_search_results->id,
						);
					} else {
						$srch_result_response_roster->rows[$i]['cell'] = array(
								'id' 	           => $rec_search_results->id,
								//'SurveyName' 	   => $rec_search_results->title,
    					'title' 	      => '<span class="vtip" title="'.htmlentities($rec_search_results->title,ENT_QUOTES,"UTF-8").'">'.titleController('MULTI-GRID-ASSESSMENT-EDIT-TITLE', $rec_search_results->title,20).'</span>',	//for 44260: German - UI - Edit/Create Course Page - Old Theme (assessment title wrapped to two lines)
								//'SurveyCode' 	   => $rec_search_results->code,
    					'code' 	      => '<span class="vtip" title="'.htmlentities($rec_search_results->code,ENT_QUOTES,"UTF-8").'">'.titleController('MULTI-GRID-ASSESSMENT-EDIT-CODE', $rec_search_results->code,15).'</span>',
								'no_of_attempts'   => $rec_search_results->no_of_attempts,
								'MultiselectCheck' => $rec_search_results->id,
						);
					}
					$i++;
				}

				break;

				foreach ($queryResult as $rec_search_results)
				{
					$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->id;
					expDebug::dPrint('$rec_search_results'. print_r($rec_search_results->no_of_attempts,true),3);
					if($mode == 'view'){
						$srch_result_response_roster->rows[$i]['cell'] = array(
								'id' 	           => $rec_search_results->id,
								//'SurveyName' 	   => $rec_search_results->title,
								'title' 	      	  => '<span class="vtip" title="'.htmlentities($rec_search_results->title,ENT_QUOTES,"UTF-8").'">'.titleController('ADMIN-LEARNING-SURVEYTITLE', $rec_search_results->title,20).'</span>',
								//'SurveyCode' 	   => $rec_search_results->code,
								'code' 	     		  => '<span class="vtip" title="'.htmlentities($rec_search_results->code,ENT_QUOTES,"UTF-8").'">'.titleController('ADMIN-LEARNING-SURVEYCODE', $rec_search_results->code,15).'</span>',
								'no_of_attempts'   => $rec_search_results->no_of_attempts,
								'view_option' 	   => $rec_search_results->view_option == 'V' ? t('LBL663') : t('LBL662'),
								'MultiselectCheck' => $rec_search_results->id,
						);
					} else {
						$srch_result_response_roster->rows[$i]['cell'] = array(
								'id' 	           => $rec_search_results->id,
								//'SurveyName' 	   => $rec_search_results->title,
								'title' 	      	  => '<span class="vtip" title="'.htmlentities($rec_search_results->title,ENT_QUOTES,"UTF-8").'">'.titleController('ADMIN-LEARNING-ELSEPART-SURVEYTITLE', $rec_search_results->title,40).'</span>',
								//'SurveyCode' 	   => $rec_search_results->code,
								'code' 	     		  => '<span class="vtip" title="'.htmlentities($rec_search_results->code,ENT_QUOTES,"UTF-8").'">'.titleController('ADMIN-LEARNING-ELSEPART-SURVEYCODE', $rec_search_results->code,15).'</span>',
								'no_of_attempts'   => $rec_search_results->no_of_attempts,
								'MultiselectCheck' => $rec_search_results->id,
						);
					}
					$i++;
				}

				break;

			case 'enrolluser':
				$selectMax = db_select("slt_course_class","cls");
				$selectMax->addField('cls','max_seats','max_seats');
				$selectMax->condition('cls.id',$entityId);
				$classMaxSeats = $selectMax->execute()->fetchField();
				if($mode == 'view'){
					$enrollStatistics = fetchEnrollUserListStatusCount($listquery,$classMaxSeats,$courseId,$entityId,$mode);
					foreach ($queryResult as $key => $rec_search_results){
						$regStatus = '';
						if($rec_search_results->reg_status_name == 'Confirmed'){
							$regStatus = $rec_search_results->comp_status_name;
						} else {
							$regStatus = $rec_search_results->reg_status_name;
						}

						/*if($regStatus == "Incomplete"){
						 $regStatus = "In Complete";
						 }*/

						if($regStatus == 'Waitlisted' && $rec_search_results->waitlist_priority != '') {
							$waitlisted = ' - '.$rec_search_results->waitlist_priority;
						}

						$createdOn = $rec_search_results->created_on;
						$updatedOn = $rec_search_results->updated_on;

						$regStatusDate = empty($rec_search_results->reg_status_date) ? $createdOn : $rec_search_results->reg_status_date;
						$compDate = $rec_search_results->comp_date;

						$createdOn = date('m/d/Y', strtotime($createdOn));
						$updatedOn = date('m/d/Y', strtotime($updatedOn));
						$enrollDate = ($regStatus == 'Completed' || $regStatus == 'No Show') ? strtotime($compDate) : strtotime($regStatusDate);

						$regStatusDate = date('m/d/Y', strtotime($regStatusDate));
						$compDate = date('m/d/Y', strtotime($compDate));
						$calDate = date('m/d/Y', strtotime($rec_search_results->date));
						$srch_result_response_roster->rows[$i]['statistic_count']   = $enrollStatistics;
						$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->id;
						$regFrom = 'Class';
						expDebug::dPrint("enrollllment exempted tst".print_r($rec_search_results ,1),5);
						if($rec_search_results->master_enr_id){
							$regFrom = 'TP';
							if($srcSource == 0) {
								$exemptedCount = getEnrollmentExmeptedorNot('tp',$rec_search_results->id);
							} else {
								$eSts = $rec_search_results->exempted_sts;
								$exmSts = explode('|',$eSts);
								$exemptedCount = $exmSts[0];
							}
						}
						else if($rec_search_results->uc_order_id){
							$regFrom = 'Cart';
						}else {
							if($srcSource == 0) {
								$exemptedCount = getEnrollmentExmeptedorNot('class',$rec_search_results->id);
							}else {
								$eSts = $rec_search_results->exempted_sts;
								$exmSts = explode('|',$eSts);
								$exemptedCount = $exmSts[0];
							}
							
						}
						
						if($exemptedCount > 0) {
							$exempted_value = '1';
						} else {
							$exempted_value = '0';
						}
						$exmpSts = explode('|',$rec_search_results->exempted_sts);
						$srch_result_response_roster->rows[$i]['cell'] = array(
								'id' 	           => $rec_search_results->id,
								'DeliveryType'     => $rec_search_results->delivery_type,
								'user_name' 	   => $rec_search_results->user_name,
								'full_name' 	   => $rec_search_results->full_name,
								'RegFrom' 	       => t($regFrom),
								'Status' 	       => $rec_search_results->status,
								'Date' 	           => $calDate,
								'SessionStarted'   => isSessionStart($entityId),
								'Score'            => ($rec_search_results->comp_status == 'lrn_crs_cmp_cmp' || $rec_search_results->comp_status == 'lrn_crs_cmp_inc' || $rec_search_results->comp_status == 'lrn_crs_cmp_nsw') ? $rec_search_results->score : '',
								'MultiselectCheck' => $rec_search_results->id,
								'exempted_sts' => $exmpSts[0],
								'exempted_id' => $exmpSts[1],
								'is_mand' => (($rec_search_results->mandatory == 'Y') ? 1 : 0),
								'is_compliance' => $rec_search_results->is_compliance,
								'row_number'	=> $key,
								'having_waived' => $exempted_value,
						);
						$i++;
					}
				} else {
					//commented as enrolluser edit mode need not have enrollments statistics
					//need to check the impact of this - Ayyappan 
 					//$enrollStatistics = fetchEnrollUserListStatusCount('',$classMaxSeats,$courseId,$entityId,$mode);
					foreach ($queryResult as $rec_search_results){
						$regStatus = '';
						$srch_result_response_roster->rows[$i]['statistic_count']   = $enrollStatistics;
						$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->id;
						$srch_result_response_roster->rows[$i]['cell'] = array(
								'id' 	              => $rec_search_results->id,
								'user_name' 	      => $rec_search_results->user_name,
								'full_name' 	      => $rec_search_results->full_name,
								'manager_name' 	      => (!empty($rec_search_results->manager_name) ? $rec_search_results->manager_name : ''),
								'organization_name'   => (!empty($rec_search_results->organization_name) ? $rec_search_results->organization_name : ''),
								'MultiselectCheck'    => $rec_search_results->id,
							//	'exempted_sts' => $rec_search_results->exempted_sts
						);
						$i++;
					}
					if($encount>=1 && $encount1>=1){
						$srch_result_response_roster->msg    = t('MSG612');
					}else{
						$srch_result_response_roster->msg    = t('MSG403');
					}
				}
				break;
			case 'shareuser':
				if($mode == 'view'){
					foreach ($queryResult as $key => $rec_search_results){
						$regStatus = '';
						if($rec_search_results->reg_status_name == 'Confirmed'){
							$regStatus = $rec_search_results->comp_status_name;
						} else {
							$regStatus = $rec_search_results->reg_status_name;
						}
						$createdOn = $rec_search_results->created_on;
						$updatedOn = $rec_search_results->updated_on;


						$srch_result_response_roster->rows[$i]['statistic_count']   = "";
						$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->id;
					
						$srch_result_response_roster->rows[$i]['cell'] = array(
								'id' 	           => $rec_search_results->id,
								'h5p_content_id' 	           => $rec_search_results->h5p_content_id,
								'registered_user_id'     => $rec_search_results->registered_user_id,
								'user_name' 	   => $rec_search_results->user_name,
								'full_name' 	   => $rec_search_results->full_name,
								'organization_name'=>(!empty($rec_search_results->organization_name) ? $rec_search_results->organization_name : ''),
								'MultiselectCheck' => $rec_search_results->id,
								'row_number'	=> $key,
								'name' => (!empty($rec_search_results->name) ? $rec_search_results->name : ''),
						        //'name' => $rec_search_results->name,
						
						);
						$i++;
					}
				} else {
					$enrollStatistics = "";//fetchEnrollUserListStatusCount('',$classMaxSeats,$courseId,$entityId,$mode);
					foreach ($queryResult as $rec_search_results){
						$regStatus = '';
						$srch_result_response_roster->rows[$i]['statistic_count']   = $enrollStatistics;
						$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->id;
						$srch_result_response_roster->rows[$i]['cell'] = array(
								'id' 	           => $rec_search_results->id,
								'h5p_content_id' 	           => $rec_search_results->h5p_content_id,
								'registered_user_id'     => $rec_search_results->registered_user_id,
								'user_name' 	   => $rec_search_results->user_name,
								'full_name' 	   => $rec_search_results->full_name,
								'organization_name'=>(!empty($rec_search_results->organization_name) ? $rec_search_results->organization_name : ''),
								'MultiselectCheck' => $rec_search_results->id,
								//'name' => $rec_search_results->name,
								'name' => (!empty($rec_search_results->name) ? $rec_search_results->name : ''),
							//	'exempted_sts' => $rec_search_results->exempted_sts
						);
						$i++;
					}
					if($encount>=1 && $encount1>=1){
						$srch_result_response_roster->msg    = t('MSG612');
					}else{
						$srch_result_response_roster->msg    = t('MSG403');
					//	unset($srch_result_response_roster->rows);
						
					}
				}
				break;
			case 'enrolltpuser':
				include_once(drupal_get_path('module', 'exp_sp_administration_program') .'/exp_sp_administration_program.inc');
				if($mode == 'view'){
					$enrollStatistics = fetchEnrollTPUserListStatusCount($entityId);
					foreach ($queryResult as $key => $rec_search_results)
					{
						$regDate = $rec_search_results->reg_date;
						$cancelDate = $rec_search_results->cancel_date;
						$compDate = $rec_search_results->comp_date;
						$updatedOn = $rec_search_results->updated_on;

						$regDate = date('m/d/Y', strtotime($regDate));
						$cancelDate = date('m/d/Y', strtotime($cancelDate));
						$compDate = date('m/d/Y', strtotime($compDate));
						$updatedOn = date('m/d/Y', strtotime($updatedOn));

						if($rec_search_results->overall_status == 'lrn_tpm_ovr_enr'){
							$statusDate = $regDate;
						} elseif ($rec_search_results->overall_status == 'lrn_tpm_ovr_cln'){
							$statusDate = $cancelDate;
						} elseif ($rec_search_results->overall_status == 'lrn_tpm_ovr_cmp'){
							$statusDate = $compDate;
						} else {
							$statusDate = $updatedOn;
						}
						
						$exemptedCount = getEnrollmentExmeptedorNot('tp',$rec_search_results->id);
						if($exemptedCount > 0) {
							$exempted_value = '1';
						} else {
							$exempted_value = '0';
						}
						
						$sessionvalue = isSessionStartForTPinClassLevel($rec_search_results->id);
						expDebug::dPrint('The value of $sessionvalue is here vetrivel '.print_r($sessionvalue,1),3);
						$session = ($sessionvalue == 1) ? true : false;
						
						/*if($rec_search_results->overall_status_name == "Incomplete"){
						 $rec_search_results->overall_status_name = "In Complete";
						 }*/
						$exmpSts = explode('|',$rec_search_results->exempted_sts);
						$mod_details = getDefaultModuleId($rec_search_results->program_id,$rec_search_results->certifypath);
						
						$srch_result_response_roster->rows[$i]['statistic_count']   = $enrollStatistics;
						$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->id;
						$srch_result_response_roster->rows[$i]['cell'] = array(
			          	   'id' 	          => $rec_search_results->id,
			               'user_name' 	      => $rec_search_results->user_name,
			               'full_name' 	      => $rec_search_results->full_name,
			               //'Status' 	      => t($rec_search_results->overall_status_name),
						   'Status' 	      => $rec_search_results->overall_status_name,
			               'SessionStarted'   => $session, // isSessionStartForTP($entityId), 
						   'path'			  => '<span class="vtip" title="'.htmlentities($mod_details[0]->module_title,ENT_QUOTES,"UTF-8").'">'.titleController('MULTI-GRID-ENRTP-VIEW-PATH', $mod_details[0]->module_title,45).'</span>',
			          	   'Date' 	          => $statusDate,
			               'MultiselectCheck' => $rec_search_results->id,
			          	   'RegFrom' 	      => isRegFormCart($rec_search_results->id),
								'exempted_sts' => $exmpSts[0],
								'exempted_id' => $exmpSts[1],
								'is_mand' => $rec_search_results->mandatory,
								'row_number'	=> $key,
								'having_waived' => $exempted_value,
								'certifypath' => $rec_search_results->certifypath,
						);
						$i++;
					}
				} else {
					foreach ($queryResult as $rec_search_results){

						$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->id;
						$srch_result_response_roster->rows[$i]['cell'] = array(
			          	    'id' 	             => $rec_search_results->id,
			                'user_name' 	     => $rec_search_results->user_name,
			               	'full_name' 	     => $rec_search_results->full_name,
			          		'manager_name' 	     => (!empty($rec_search_results->manager_name) ? $rec_search_results->manager_name : ''),
			          		'organization_name'  => (!empty($rec_search_results->organization_name) ? $rec_search_results->organization_name : ''),
			               	'MultiselectCheck'   => $rec_search_results->id,
						);
						$i++;
					}
					if($encount>=1 && $encount1>=1){
						$srch_result_response_roster->msg    = t('MSG612');
					}else{
						$srch_result_response_roster->msg    = t('MSG403');
					}

				}
				break;
				 
			case 'content':
				foreach ($queryResult as $rec_search_results)
				{
					$contentType  = explode(' ',$rec_search_results->type_name);
					$contentType = ($rec_search_results->type_name == 'Tin Can') ? $rec_search_results->type_name : $contentType[0];
					$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->id;
					$srch_result_response_roster->rows[$i]['cell'] = array(
			     	   'id' 	          => $rec_search_results->id,
			    		//'ContentCode'   => $rec_search_results->code,
							'code' 	      		  => '<span class="vtip" title="'.sanitize_data($rec_search_results->code).'">'.titleController('MULTI-GRID-CONTENT-CODE', $rec_search_results->code,20).'</span>',
					   //'ContentType' 	  => $contentType,
							'type_name' 	      => '<span class="vtip" title="'.sanitize_data($contentType).'">'.titleController('MULTI-GRID-CONTENT-TYPE', $contentType,20).'</span>',
			           'total_lesson'     => $rec_search_results->total_lesson == null ? '-' : $rec_search_results->total_lesson,
				       'max_attempts'     => $rec_search_results->max_attempts == null ? '-' : $rec_search_results->max_attempts,
			           'validity_days'    => $rec_search_results->validity_days == null ? '-' : $rec_search_results->validity_days,
			           'MultiselectCheck' => $rec_search_results->id,
			           'ShowDelete'		  => (isset($rec_search_results->enroll_id) ? (($rec_search_results->enroll_id != null) ?$rec_search_results->enroll_id : 'hide') : 'hide'),
			           'countquery'       => $count,
			           'SequenceDrag'     => $rec_search_results->id,
					);
					$i++;
				}
				break;


			case 'prerequisite':
				foreach ($queryResult as $rec_search_results)
				{
					if($rec_search_results->type=='cre_sys_obt_cur' || $rec_search_results->type=='cre_sys_obt_crt' || $rec_search_results->type=='cre_sys_obt_trn' ||$rec_search_results->type=='cre_sys_pre_trp'){

						$type='Training Plan';
						$typecode='cre_sys_pre_trp';
					}
					else{
						$type='Course';
						$typecode='cre_sys_pre_crs';
					}
					if($rec_search_results->prereq_type =='cre_sys_pre_crs'){
						$commontitle=$rec_search_results->crstitle;
						$commoncode=$rec_search_results->crscode;
					}
					else{
						$commontitle=$rec_search_results->title;
						$commoncode=$rec_search_results->code;
					}
					//$charCount = ($type=='Training Program') ? 'MULTI_GRID_PERREQUISITE_TITLE' : 'MULTI_GRID_TITLE';
					$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->id.'-'. $typecode;
					$srch_result_response_roster->rows[$i]['cell'] = array(
		        	 'id' 	            => $rec_search_results->id,
					 //'CourseTitle' 	=> $commontitle,
					 //'CourseCode' 	=> $commoncode,
							'title' 	     	 => '<span class="vtip" title="'.sanitize_data($commontitle).'">'.titleController('MULTI-GRID-PERREQUISITE-TITLE', $commontitle,40).'</span>',
							'code' 	      		 => '<span class="vtip" title="'.sanitize_data($commoncode).'">'.titleController('MULTI-GRID-PERREQUISITE-CODE', $commoncode,10).'</span>',
		             'Type' 		    => t($type), 
		             'MultiselectCheck' => $rec_search_results->id.'-'. $typecode,
					);
					$i++;
				}
				break;
			case 'attachQuestion':
				$validate_flag = false;
				//'add_error' group_error
				if(isset($_GET["error_str"])) {
					$error_str                = $_GET["error_str"];
					list($sequErr,$scoreErr,$groupErr)  = explode("~",$error_str);
					$seqStr                   = explode("-",$sequErr);
					$scoreStr                 = explode("-",$scoreErr);
					$groupStr                 = explode("-",$groupErr);
					$validate_flag            = true;
					$seqStack                 = array();
					$scoreStack               = array();
					$groupStack               = array();
					$idStack                  = array();

					foreach($seqStr as $seqStrv) {
						$seqStrExp              = explode(":",$seqStrv);
						$seqId                  = $seqStrExp[0];
						$seqVal                 = $seqStrExp[1];
						$seqStack[$seqId]       = $seqVal;
						$idStack[]              = $seqId;
					}

					foreach($scoreStr as $scoreStrV) {
						$scoreStrExp            = explode(":",$scoreStrV);
						$scoreId                = $scoreStrExp[0];
						$scoreseqVal            = $scoreStrExp[1];
						$scoreStack[$scoreId]   = $scoreseqVal;
					}

					foreach($groupStr as $groupStrV) {
						$groupStrExp            = explode(":",$groupStrV);
						$groupId                = $groupStrExp[0];
						$groupVal               = $groupStrExp[1];
						$groupStack[$groupId]   = $groupVal;
					}
				}
				 
				$srch_result_response_roster->selectedIds    = implode(",",$idStack);
				 
				$srch_result_response_roster->pg_mode   = $mode;
				if($mode == 'addmore'){

					$srch_result_response_roster->total   = 1;
					$srch_result_response_roster->records = 1;
					$srch_result_response_roster->page    = 1;

					foreach ($queryResult as $rec_search_results)   {
						$validateFlag     = in_array($rec_search_results->id,$idStack) ? $validate_flag : FALSE;
						$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->id;
						$srch_result_response_roster->rows[$i]['cell'] = array(
			          	   'id'	               => $rec_search_results->id,
			               'question' 	       => $rec_search_results->question_txt,
			               'code' 	           => $rec_search_results->question_code,
			               'groupName' 	       => $validateFlag ? $groupStack[$rec_search_results->id] : (!empty($rec_search_results->survey_group_title) ? $rec_search_results->survey_group_title : ''),
			               'sequenceNo' 	   => $validateFlag ? $seqStack[$rec_search_results->id] : (!empty($rec_search_results->survey_group_sequence) ? $rec_search_results->survey_group_sequence : ''),
			               'score' 	           => $validateFlag ? $scoreStack[$rec_search_results->id] : (!empty($rec_search_results->survey_score) ? $rec_search_results->survey_score : ''),
						   //'mandatoryOption' => $validateFlag ? $mandatoryStack[$rec_search_results->id] : (!empty($rec_search_results->mandatoryOptional) ? $rec_search_results->mandatoryOptional : ''),
			               'mandatoryOptional' => (!empty($rec_search_results->mandatory_option) ? $rec_search_results->mandatory_option : ''),
			               'validate_flag'	   => $validateFlag,
			          	   'MultiselectCheck'  => $rec_search_results->id,
						);
						break;
						$i++;
					}
					 
				} else {
						
					foreach ($queryResult as $rec_search_results)   {
						$validateFlag     = in_array($rec_search_results->id,$idStack) ? $validate_flag : FALSE;
						$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->id;
						$srch_result_response_roster->rows[$i]['cell'] = array(
			          	   'id' 	           => $rec_search_results->id,
			               'question' 	       => $rec_search_results->question_txt,
			               'code' 	           => $rec_search_results->question_code,
			               'groupName' 	       => $validateFlag ? $groupStack[$rec_search_results->id] : (!empty($rec_search_results->survey_group_title) ? $rec_search_results->survey_group_title : ''),
			               'sequenceNo' 	   => $validateFlag ? $seqStack[$rec_search_results->id] : (!empty($rec_search_results->survey_group_sequence) ? $rec_search_results->survey_group_sequence : ''),
			               'score' 	           => $validateFlag ? $scoreStack[$rec_search_results->id] : (!empty($rec_search_results->survey_score) ? $rec_search_results->survey_score : ''),
			          	   'mandatoryOptional' => (!empty($rec_search_results->mandatory_option) ? $rec_search_results->mandatory_option : ''),
			               'validate_flag'	   => $validateFlag,          	   
			               'MultiselectCheck'  => $rec_search_results->id,
						);
						$i++;
					}
					break;
				}
				 
			case 'session':
				include_once(drupal_get_path('module', 'exp_sp_administration_catalog') .'/exp_sp_administration_catalog_class.inc');
				$isEnrolled = getTotalClassEnroll($entityId);
				$i = 0;
				foreach ($queryResult as $rec_search_results)
				{
					$deliveryType = $rec_search_results->delivery_type;
					$popupentityId      = $rec_search_results->session_id;
					$popupAddSessionentityType    = 'session';
					$popupAddSessionIdInit        	 = $popupentityId.'_'.$popupAddSessionentityType;
					$popupAddSessionvisibPopupId  = 'qtip_visible_disp_addsession_'.$popupAddSessionIdInit;
					$anchorId = 'frm-'.$popupAddSessionvisibPopupId;
					$meetType = (!empty($rec_search_results->meeting_type)) ? $rec_search_results->meeting_type : "Ilt";
					$titleLenght = 20;
					$otherSessionWidth=500;
					global $user;
					$lang=$user->language;
					expDebug::dPrint('debug language '.$lang);
					if($meetType=='lrn_cls_vct_web' || $meetType=='lrn_cls_vct_exp'){
					if($lang == 'ru')
					$otherSessionWidth=$otherSessionWidth;
					if($lang == 'fr')
					$otherSessionWidth=$otherSessionWidth;
					if($lang == 'es')
					$otherSessionWidth=$otherSessionWidth+35;
					}else{
						if($lang == 'ru')
						$otherSessionWidth=$otherSessionWidth;
						if($lang == 'fr')
						$otherSessionWidth=$otherSessionWidth;
						if($lang == 'de')
						$otherSessionWidth=$otherSessionWidth;
						if($lang == 'it')
						$otherSessionWidth=$otherSessionWidth+12;
						if($lang == 'es')
						$otherSessionWidth=$otherSessionWidth+30;
						
					}
					expDebug::dPrint('width and lang ' .$otherSessionWidth." ".$lang." ".$meetType);
					
					if($meetType=='lrn_cls_vct_web'){
						$popupAddSession    = "{'rowVal':".$i.",'entityId':".$popupentityId.",'entityType':'".$popupAddSessionentityType."','url':'ajax/class-add-session/".core_encrypt($rec_search_results->course_id)."/".core_encrypt($rec_search_results->class_id)."/".$meetType."/Edit/".core_encrypt($rec_search_results->session_id)."/".core_encrypt($excludedId)."','popupDispId':'".$popupAddSessionvisibPopupId."','catalogVisibleId':'qtipEditSessionIdqtip_visible_disp_".$popupAddSessionIdInit."','wBubble':$otherSessionWidth,'hBubble':'auto','tipPosition':'tipTopRight','qtipClass':'admin-qtip-access-parent','sessionPopupId':'catalog-class-basic-addedit-form-container'}";
						$titleLenght = 20;
					}
					else{
						$popupAddSession    = "{'rowVal':".$i.",'entityId':".$popupentityId.",'entityType':'".$popupAddSessionentityType."','url':'ajax/class-add-session/".core_encrypt($rec_search_results->course_id)."/".core_encrypt($rec_search_results->class_id)."/".$meetType."/Edit/".core_encrypt($rec_search_results->session_id)."/".core_encrypt($excludedId)."','popupDispId':'".$popupAddSessionvisibPopupId."','catalogVisibleId':'qtipEditSessionIdqtip_visible_disp_".$popupAddSessionIdInit."','wBubble':$otherSessionWidth,'hBubble':'auto','tipPosition':'tipTopRight','qtipClass':'admin-qtip-access-parent','sessionPopupId':'catalog-class-basic-addedit-form-container'}";
						$titleLenght = 20;
					}
					$meetingdetailId = $rec_search_results->session_id;
					$meetingdetailType    = 'session';
					$meetingdetailIdInit    = $meetingdetailId.'_'.$meetingdetailType;
					$meetingdetailsvisibPopupId  = 'qtip_visible_meeting_detail_'.$meetingdetailIdInit;
					$qtipMeetingDetailOptObj    = "{'rowVal':".$i.",'entityId':".$meetingdetailId.",'entityType':'session','url':'administration/meeting-details/".$entityId."/".$rec_search_results->session_id."','popupDispId':'".$meetingdetailsvisibPopupId."','assignHtml':'meeting-container','catalogVisibleId':'sessionMoreEditSessionIdqtip".$meetingdetailIdInit."','wBubble':460,'hBubble':'auto','tipPosition':'tipTopRight','qtipClass':'admin-qtip-access-parent'}";
					 
					 
					$stDate = explode(" ",$rec_search_results->start_date);
					$pipLine = ($isEnrolled > 0 && $theme_key == "expertusoneV2") ? '' : '<span class="narrow-search-results-item-detail-pipe-line session-edit-delete-icons">|</span>';
					//$in_style = ($isEnrolled > 0 && $theme_key == "expertusoneV2") ? 'margin-top:-11px' : '';   // for ticket no : 63575
					$in_style = '';
					$editText = ($theme_key == "expertusoneV2") ? ' ' : t('LBL063');
					$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->session_id;
					$srch_result_response_roster->rows[$i]['cell'] = array(
		        	 'id'				 => $rec_search_results->session_id,
		        	 'meeting_type_name' => !empty($rec_search_results->meeting_type_name) ? t($rec_search_results->meeting_type_name) : '',
							'title' 			=> '<span class="vtip" title="'.sanitize_data($rec_search_results->title).'">'.titleController('ADMIN-LEARNING-METTING-TITLE',sanitize_data($rec_search_results->title),$titleLenght,25).'</span>' ,
		        	 'start_date' 		 => date('m-d-Y', strtotime($stDate[0])),
		             'start_time' 	  	 => $rec_search_results->start_time,
		             'end_time' 	  	 => $rec_search_results->end_time,
							'full_name' 		=> $rec_search_results->full_name ? ('<span class="vtip" title="'.sanitize_data($rec_search_results->full_name).'">'.titleController('ADMIN-LEARNING-METTING-FULLNAME',sanitize_data($rec_search_results->full_name,15)).'</span>') : '',
					  //'More'			 => "<div class='admin-session-meeting-vc-class-container' style='position:relative;'><span class=\"session-meeting-details\">".t('LBL713')."</span><a class=\"session-dd-arrow-link\"  onclick = \"$('#root-admin').data('narrowsearch').getQtipDiv(".$qtipMeetingDetailOptObj.",".$i.");$('#meeting-container').css('display','block');\">&nbsp;</a><div id='".$meetingdetailsvisibPopupId."'></div></div>",
		        	 'More'				 => "<div class='admin-session-meeting-vc-class-container' id='more-wrapper-div'> <a class=\"session-meeting-details\"onclick = \"$('#root-admin').data('narrowsearch').getQtipDiv(".$qtipMeetingDetailOptObj.",".$i.");$('#meeting-container').css('display','block');\">".t('LBL713')."&nbsp;</a><div id='".$meetingdetailsvisibPopupId."'></div></div>",
		             'Edit'   			 => '<div style="position:relative;'.$in_style.'">'.(($deliveryType == 'lrn_cls_dty_vcl')? '<span class="narrow-search-results-item-detail-pipe-line session-edit-delete-icons">|</span>':'').'<a class="session-added-edit-link-text" onclick="$(\'#root-admin\').data(\'narrowsearch\').getQtipDiv('.$popupAddSession.'); return false;">'.$editText.'</a>'.$pipLine.'<div class="add_session_popup" id="'.$popupAddSessionvisibPopupId.'" ></div></div>',
					 //'Remove'   		 => $rec_search_results->session_id,
		      		 'ShowDelete'		 => ($isEnrolled > 0) ? $rec_search_results->session_id : 'hide',
					);
					$i++;
				}
				break;

			case 'trainings':
				foreach ($queryResult as $rec_search_results)
				{
					$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->term_id;
					$srch_result_response_roster->rows[$i]['cell'] = array(
		        	 'id' 	              => $rec_search_results->term_id,
					 /*'CourseTitle' 	  => $rec_search_results->title,
					 'CourseCode' 	      => $rec_search_results->code,*/
							'TrainingTitle' 	  => '<span class="vtip" title="'.sanitize_data($rec_search_results->entity_title).'">'.titleController('ADMIN-LEARNING-TRANINGTITLE', sanitize_data($rec_search_results->entity_title,70)).'</span>',
							'TrainingCode' 	  	  => '<span class="vtip" title="'.sanitize_data($rec_search_results->entity_code).'">'.titleController('ADMIN-LEARNING-TRANINGCODE', sanitize_data($rec_search_results->entity_code,10)).'</span>',
		             'MultiselectCheck'   => ($rec_search_results->term_id) ? $rec_search_results->term_id : $rec_search_results->entity_id ,
		        	 'Remove'		      => ($rec_search_results->term_id) ? $rec_search_results->term_id : $rec_search_results->entity_id ,
		        	 'DelType' 			  =>  $rec_search_results->objtype,
					);;
					$i++;
				}
				break;
				
			case 'enrolltprecertify':
				
				foreach ($queryResult as $rec_search_results)
				{
					expDebug::dPrint("completion date ". $rec_search_results->comp_date);
					$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->id;
					$srch_result_response_roster->rows[$i]['cell'] = array(
		        	 'id' 	              => $rec_search_results->id,
					 'Path' 	  => '<span class="vtip" title="'.sanitize_data($rec_search_results->path).'">'.titleController('ADMIN-LEARNING-TRANINGTITLE', $rec_search_results->path,70).'</span>',
					 'EnrollmentDate' 	  	  => date('m/d/Y', strtotime($rec_search_results->reg_date)),
		             'CompletionDate'   => !empty($rec_search_results->comp_date) ? date('m/d/Y', strtotime($rec_search_results->comp_date)) : '',
		        	 'Status'		      => $rec_search_results->overall_status ,
					);;
					$i++;
		}
				break;
		}
		echo drupal_json_encode($srch_result_response_roster);
	} catch (Exception $ex) {
		watchdog_exception('loadCtoolDataGridType', $ex);
		expertusErrorThrow($ex);
	}
}

function sequenceDragDrop($type, $sequenceOrder){
	try {
		switch($type){
			case 'attach_course':
				include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_attach_course.inc');
				//sequenceAttachCourse($sequenceOrder);
				break;
				 
			case 'attach_question':
				include_once(drupal_get_path('module', 'exp_sp_administration_surveydetails') .'/exp_sp_administration_survey_assesment_attach_question.inc');
				sequenceAttachQuestion($sequenceOrder);
				break;
			case 'attach_class_content':
				include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_content_details.inc');
				sequenceAttachContent($sequenceOrder);
				break;
		}
	} catch (Exception $ex) {
		watchdog_exception('sequenceDragDrop', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getClassTpEnrollmentsResultForGrid
 * Get class and tp enrollments for an user give the list to datagrid
 */
function getClassTpEnrollmentsResultForGrid($userid,$keyword=''){
	try{
		$page      = $_GET['page'];
		$limit     = $_GET['rows'];
		$sidxVal      = $_GET['sidx'];
		$sord      = $_GET['sord'];

		if((!empty($limit) && !is_numeric($limit)) || !empty($page) && !is_numeric($page)){
			throw new Exception('Non Numeric Values');
		}
		 
		if(!empty($sidxVal)){
			$sidx = escape_string($sidxVal);
			$_GET['sidx'] = $sidx;
		}

		$sord      = $_GET['sord'];
		if(!empty($sord)){
			if((stripos('desc',$sord) !== false)){
				$sord = 'desc';
			}else{
				$sord = 'asc';
			}
			$_GET['sord'] = $sord;
		}
		$srcSource = getConfigValue('solr_search');
		if($srcSource == 0) {
		$count      = getClassTpEnrollmentResult($userid,$keyword,false,true);

		/* Paging Related */
		$start = $limit * $page - $limit;
		$listquery  = getClassTpEnrollmentResult($userid,$keyword,true,false,$start,$limit);
		//$listquery->range($start, $limit);
		// Execute query
		$queryResult = $listquery->execute();
		expDebug::dPrint('User Class/TP Enrollments $start->'. $start,4);
		expDebug::dPrint('User Class/TP Enrollments $limit->'. $limit,4);
		expDebug::dPrint('User Class/TP Enrollments'. print_r($queryResult,true),3);
		expDebug::dPrintDBAPI('User Class/TP Enrollments', $listquery);

		if( $count >0 ){
			$total_pages = ceil($count/$limit);
		}else{
			$total_pages = 0;
		}
		if($page > $total_pages)
		$page = $total_pages;
		if($start < 0) {
			$start = 0;
		}
		} 
		else {
			require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user_enrollments.php';
			try{
				expDebug::dPrint ( "Solr user enrollments Search Testing", 1 );
				$solrObj = new UserEnrollments();
				$qryRes = $solrObj->UserEnrollmentSolrSearch($userid,$keyword);
				$count = $qryRes['count'];
				$queryResult = $qryRes['result'];
				if( $count >0 ){
					$total_pages = ceil($count/$limit);
				}else{
					$total_pages = 0;
				}
			}catch(Exception $e){
				expDebug::dPrint("Error in my program Solr Search -- ".print_r($e->getMessage(),true),1);
			}

		}
		$srch_result_response_roster->total   = $total_pages;
		$srch_result_response_roster->records = $count;
		$srch_result_response_roster->page    = $page;
		$srch_result_response_roster->unique_id    = core_encrypt($userid).'-enrollments';
		$_SESSION['jq_count'] = array();
		$i=0;
		foreach ($queryResult as $rec_search_results){
			if($srcSource == 1) {
				$rec_search_results->reg_date = date('m/d/Y', strtotime($rec_search_results->reg_date));
				$rec_search_results->comp_date = date('m/d/Y', strtotime($rec_search_results->comp_date));
			}
			$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->id;

			$srch_result_response_roster->rows[$i]['cell'] = array(
					'id' => $rec_search_results->id,
					'code' => '<span class="vtip" title="' . sanitize_data($rec_search_results->code) . '">' . titleController('ADMIN-LEARNING-ENROLLMENTS-RESULT-CODE', sanitize_data($rec_search_results->code), 10) . '</span>',
					'title' => '<span class="vtip" title="' . sanitize_data($rec_search_results->title) . '">' . titleController('ADMIN-LEARNING-ENROLLMENTS-RESULT-TITLE', sanitize_data($rec_search_results->title), 16) . '</span>',
					'delivery_type' => t($rec_search_results->delivery_type),
					'Date' => ($rec_search_results->status == 'Completed' || $rec_search_results->status == 'No Show') ? $rec_search_results->comp_date : $rec_search_results->reg_date,
					'Status' => ($rec_search_results->exempted_sts == 1) ? t('Waived') : t($rec_search_results->status),
					'exempted_sts' => $rec_search_results->exempted_sts
			);

			$i++;
		}
		echo drupal_json_encode($srch_result_response_roster);
	}catch (Exception $ex) {
		watchdog_exception('getClassTpEnrollmentsResultForGrid', $ex);
		expertusErrorThrow($ex);
	}
}//End getClassTpEnrollmentsResultForGrid()

/*
 * getClassTpEnrollmentResult
 * Based on the arugment, $queryRequired and $countRequired, it will return the
 * user enrollment query or total count
 */
function getClassTpEnrollmentResult($userid,$keyword='',$queryRequired=false,$countRequired=false,$start=null,$limit=null){
	try{
		$sidx      = $_GET['sidx'];
		$sord      = $_GET['sord'];
		//select query
		$selectEnrollCls = db_select('slt_enrollment', 'myenroll');
		$selectEnrollCls->leftjoin('slt_course_class', 'cat','cat.id=myenroll.class_id AND cat.course_id=myenroll.course_id');
		//select fields
		if($countRequired){
			$selectEnrollCls->addExpression('count(myenroll.id)', 'count');
		} else {
			//$selectEnrollCls->leftjoin('slt_course_class', 'cat','cat.id=myenroll.class_id AND cat.course_id=myenroll.course_id');

			//Removed In-progress condition For #0042257
			//$selectEnrollCls->leftjoin('slt_profile_list_items', 'pli_reg_status','pli_reg_status.code = if((myenroll.reg_status=\'lrn_crs_reg_cnf\'),if((myenroll.comp_status=\'lrn_crs_cmp_inp\'),\'lrn_crs_cmp_enr\',myenroll.comp_status),myenroll.reg_status)');
			$selectEnrollCls->leftjoin('slt_profile_list_items', 'pli_reg_status','pli_reg_status.code = if((myenroll.reg_status=\'lrn_crs_reg_cnf\'),myenroll.comp_status,myenroll.reg_status)');
			$selectEnrollCls->leftjoin('slt_profile_list_items', 'pli_del_type','pli_del_type.code=cat.delivery_type');

			$selectEnrollCls->addField('myenroll', 'id', 'id');
			$selectEnrollCls->addField('cat', 'title', 'title');
			$selectEnrollCls->addField('cat', 'code', 'code');
			$selectEnrollCls->addField('pli_reg_status', 'name', 'Status');
			$selectEnrollCls->addField('pli_del_type', 'name', 'delivery_type');
			$selectEnrollCls->addExpression('DATE_FORMAT(`myenroll`.`reg_date`,_utf8\'%m/%d/%Y\')', 'reg_date');
			$selectEnrollCls->addExpression('DATE_FORMAT(`myenroll`.`comp_date`,_utf8\'%m/%d/%Y\')', 'comp_date');
			$selectEnrollCls->addExpression('(SELECT exmp.exempted_status FROM slt_enrollment_exempted exmp WHERE exmp.enrollment_id = myenroll.id AND exmp.enroll_type = \'class\' ORDER BY id DESC LIMIT 1)', 'exempted_sts');
		}
		//query condition
		$selectEnrollCls->condition('myenroll.user_id', $userid,'=');
		$selectEnrollCls->condition('myenroll.master_enrollment_id', '','IS NULL');
		$selectEnrollCls->condition('myenroll.reg_status', array('lrn_crs_reg_rsc','lrn_crs_reg_rjt','lrn_crs_reg_rsv'),'NOT IN');


		//if keyword available then check with class tile
		if($keyword!=''){
			$keyword = str_replace("|","/",$keyword);
			$selectEnrollCls->condition(db_or()
			->condition('cat.title', '%' . db_like($keyword) . '%', 'LIKE')
			->condition('cat.code', '%' . db_like($keyword) . '%', 'LIKE'));
		}

		//select query
		$selectEnrollTp = db_select('slt_master_enrollment', 'masterenroll');
		$selectEnrollTp->leftjoin('slt_program', 'prg', 'prg.id = masterenroll.program_id');
		//select fields
		if($countRequired){
			$selectEnrollTp->addExpression('count(prg.id)', 'count');
		} else {
			$selectEnrollTp->leftjoin('slt_profile_list_items', 'pli_tp_reg_status','pli_tp_reg_status.code = if((masterenroll.overall_status=\'lrn_tpm_ovr_inp\'),\'lrn_tpm_ovr_enr\',masterenroll.overall_status)');
			$selectEnrollTp->leftjoin('slt_profile_list_items', 'pli_tp_del_type','pli_tp_del_type.code = prg.object_type');

			$selectEnrollTp->addField('prg', 'id', 'id');
			$selectEnrollTp->addField('prg', 'title', 'title');
			$selectEnrollTp->addField('prg', 'code', 'code');
			$selectEnrollTp->addField('pli_tp_reg_status', 'name', 'Status');
			$selectEnrollTp->addField('pli_tp_del_type', 'name', 'delivery_type');
			$selectEnrollTp->addExpression('DATE_FORMAT(`masterenroll`.`reg_date`,_utf8\'%m/%d/%Y\')', 'reg_date');
			$selectEnrollTp->addExpression('DATE_FORMAT(`masterenroll`.`comp_date`,_utf8\'%m/%d/%Y\')', 'comp_date');
			$selectEnrollTp->addExpression('(SELECT exmp.exempted_status FROM slt_enrollment_exempted exmp WHERE exmp.enrollment_id = masterenroll.id AND exmp.enroll_type = \'tp\' ORDER BY id DESC LIMIT 1)', 'exempted_sts');
		}
		//query condition
		$selectEnrollTp->condition('masterenroll.user_id', $userid,'=');
		$selectEnrollTp->condition('masterenroll.overall_status', array('lrn_tpm_ovr_enr','lrn_tpm_ovr_inp','lrn_tpm_ovr_cmp','lrn_tpm_ovr_inc','lrn_tpm_ovr_cln','lrn_tpm_ovr_ppv','lrn_tpm_ovr_ppm','lrn_tpm_ovr_wtl'),'IN');

		//if keyword available then check with program title
		if($keyword!=''){
			$selectEnrollTp->condition(db_or()
			->condition('prg.title', '%' . db_like($keyword) . '%', 'LIKE')
			->condition('prg.code', '%' . db_like($keyword) . '%', 'LIKE'));
		}


		if($countRequired){
			$enrollResult = $selectEnrollCls->execute()->fetchField();
			$masterenrollResult = $selectEnrollTp->execute()->fetchField();
			$totalenrollResult = $enrollResult + $masterenrollResult;
			expDebug::dPrintDBAPI('User Class/TP Enrollments', $selectEnrollCls);
			expDebug::dPrint('User Class/TP Enrollments count query result->'. $totalenrollResult , 3);
			return $totalenrollResult;
		}else if($queryRequired){
			if($sidx != '')
			$xorderBy = ($sidx == 'Date') ? "if(comp_date IS NULL,UNIX_TIMESTAMP(STR_TO_DATE(reg_date,'%m/%d/%Y')),UNIX_TIMESTAMP(STR_TO_DATE(comp_date,'%m/%d/%Y')))" : $sidx;
			else
			$xorderBy = "title";

			$xorderBy = checksortby('getClassTpEnrollmentResult',$xorderBy);
			$selectEnrollTp->orderBy($xorderBy, $sord);
			$selectEnrollTp->range($start, $limit);
	  //union class and tp queries
	  $selectEnrollCls->union($selectEnrollTp);
	  return $selectEnrollCls;
		}else{
			//union class and tp queries
			$selectEnrollCls->union($selectEnrollTp);
			$enrollResult = $selectEnrollCls->execute();
			return $enrollResult;
			expDebug::dPrintDBAPI('User Class/TP Enrollments', $selectEnrollCls);
			expDebug::dPrint('User Class/TP Enrollments query result->'. $enrollResult , 3);
		}
	}catch (Exception $ex) {
		watchdog_exception('getClassTpEnrollmentResult', $ex);
		expertusErrorThrow($ex);
	}
}//End getClassTpEnrollmentResult

/*
 * Fetch all class/tp enrollments for auto complete
 */
function getClassTpEnrollmentResultForAutoComplete($userId){
	try{
		$srcSource = getConfigValue('solr_search');
		if($srcSource == 0) {
		$matchValue = $_GET['z'];
		$qry_autocomplete = getClassTpEnrollmentResult($userId,$matchValue);
		} else {

			require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user_enrollments.php';
			try {
				$solrObj = new UserEnrollments();
				$solrList = $solrObj->UserEnrollmentSolrSearchAutoComplete($userId);
				$qry_autocomplete = $solrList ['records'];
				expDebug::dPrint ( ' $prgList123 = ' . print_r ( $names, true ), 4 );
				$numCatalog = $solrList ['recCount'];
				expDebug::dPrint ( ' Total matiching records ' . print_r ( $numCatalog, true ), 4 );
			} catch ( Exception $e ) {
				expDebug::dPrint ( "Error in Solr Search for Text search autocomplete - " . print_r ( $e->getMessage (), true ), 1 );
			}
			
			
		}
		foreach ($qry_autocomplete as $rec_autocomplete) {
			print $rec_autocomplete->title."\n";
		}
	}catch (Exception $ex) {
		watchdog_exception('getClassTpEnrollmentResultForAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}//End getClassTpEnrollmentResultForAutoComplete()

/*
 * Fetch enrollment status count for particular user
 */
function fetchUserEnrollmentStatusCount($userId){
	try{
		$enrolled=$inprogress=$completed=$cancelled=$incomplete=$waitlist=$pending=0;
		$srcSource = getConfigValue('solr_search');
		if($srcSource == 0) {
		//select query for class
		$selectEnrollCls = db_select('slt_enrollment', 'myenroll');
		$selectEnrollCls->leftjoin('slt_course_class', 'cat','cat.id=myenroll.class_id AND cat.course_id=myenroll.course_id');
		//select fields for class query
		$selectEnrollCls->addField('myenroll', 'reg_status', 'reg_status');
		$selectEnrollCls->addField('myenroll', 'comp_status', 'comp_status');
		$selectEnrollCls->addExpression('count(myenroll.reg_status)', 'reg_status_count');
		$selectEnrollCls->addExpression('count(myenroll.comp_status)', 'comp_status_count');
		$selectEnrollCls->groupby('myenroll.reg_status');
		$selectEnrollCls->groupby('myenroll.comp_status');
		//query condition for class query
		$selectEnrollCls->condition('myenroll.user_id', $userId,'=');
		$selectEnrollCls->condition('myenroll.master_enrollment_id', '','IS NULL');
		$selectEnrollCls->condition('myenroll.reg_status', array('lrn_crs_reg_rsc','lrn_crs_reg_rjt','lrn_crs_reg_rsv'),'NOT IN');
		$selectEnrollClsRes  = $selectEnrollCls->execute()->fetchAll();
		expDebug::dPrintDBAPI('User Class Enrollments Count Query ->', $selectEnrollCls);
		expDebug::dPrint('User Class Enrollments Count Query Result->'. $selectEnrollClsRes,3);

		//For class enrollments count
		foreach($selectEnrollClsRes as $recClsResults){
			expDebug::dPrint("Class Enrollment reg status count :".$recClsResults->reg_status_count , 4);
			expDebug::dPrint("Class Enrollment comp status count :".$recClsResults->comp_status_count , 4);
			$regStatusCount = empty($recClsResults->reg_status_count) || $recClsResults->reg_status_count=='' ? 0 : $recClsResults->reg_status_count;
			$compStatusCount = empty($recClsResults->comp_status_count) || $recClsResults->comp_status_count=='' ? 0 : $recClsResults->comp_status_count;
			if($recClsResults->comp_status == 'lrn_crs_cmp_enr'){
				$enrolled = $compStatusCount;
			}
			if($recClsResults->comp_status == 'lrn_crs_cmp_inp'){	//43240: Need to display inprogress count in user enrollments pop up
				$inprogress += $compStatusCount;
			}
			if($recClsResults->comp_status == 'lrn_crs_cmp_cmp'){
				$completed = $compStatusCount;
			}
			if($recClsResults->reg_status == 'lrn_crs_reg_can'){
				$cancelled = $regStatusCount;
			}
			if($recClsResults->comp_status == 'lrn_crs_cmp_inc'){
				$incomplete = $compStatusCount;
			}
			if($recClsResults->reg_status == 'lrn_crs_reg_ppm' || $recClsResults->reg_status == 'lrn_crs_reg_wtl'){
				$pending += $regStatusCount;
			}
		}

		//select query for TP
		$selectEnrollTp = db_select('slt_master_enrollment', 'masterenroll');
		$selectEnrollTp->leftjoin('slt_program', 'prg', 'prg.id = masterenroll.program_id');
		//select fields for TP query
		$selectEnrollTp->addField('masterenroll', 'overall_status', 'overall_status');
		$selectEnrollTp->addExpression('count(masterenroll.overall_status)', 'overall_status_count');
		$selectEnrollTp->groupby('masterenroll.overall_status');
		//query condition for TP query
		$selectEnrollTp->condition('masterenroll.user_id', $userId,'=');
		$selectEnrollTp->condition('masterenroll.overall_status', array('lrn_tpm_ovr_enr','lrn_tpm_ovr_inp','lrn_tpm_ovr_cmp','lrn_tpm_ovr_inc','lrn_tpm_ovr_cln','lrn_tpm_ovr_ppv','lrn_tpm_ovr_ppm','lrn_tpm_ovr_wtl'),'IN');
		$selectEnrollTpRes  = $selectEnrollTp->execute()->fetchAll();

		expDebug::dPrintDBAPI('User TP Enrollments Count Query->', $selectEnrollTp);
		expDebug::dPrint('User TP Enrollments Count Query Result->'. $selectEnrollTpRes,3);

		//For tp enrollments count
		foreach($selectEnrollTpRes as $recTpresults){
			$ovrStatusCount = empty($recTpresults->overall_status_count) ||  $recTpresults->overall_status_count=='' ? 0 : $recTpresults->overall_status_count;
			if($recTpresults->overall_status == 'lrn_tpm_ovr_enr' || $recTpresults->overall_status == 'lrn_tpm_ovr_inp'){
				$enrolled += $ovrStatusCount;
			}
			if($recTpresults->overall_status == 'lrn_tpm_ovr_cmp'){
				$completed += $ovrStatusCount;
			}
			if($recTpresults->overall_status == 'lrn_tpm_ovr_cln'){
				$cancelled += $ovrStatusCount;
			}
			if($recTpresults->overall_status == 'lrn_tpm_ovr_inc'){
				$incomplete += $ovrStatusCount;
			}
			if($recTpresults->overall_status == 'lrn_tpm_ovr_ppm'){
				$pending += $ovrStatusCount;
			}
		}
		} else {
			require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user_enrollments.php';
			try{
				$solrObj = new UserEnrollments();
				$qryRes = $solrObj->UserEnrollmentSolrSearch($userId);
				$statusCount = $qryRes['statusCount'];
				$enrolled = $statusCount['Enrolled'];
				$inprogress = $statusCount['In progress'];
				$completed = $statusCount['Completed'];
				$incomplete = $statusCount['Incomplete'];
				$pending = $statusCount['Pending'];
				$cancelled = $statusCount['Canceled'];
			}catch(Exception $e){
				expDebug::dPrint("Error in my program Solr Search -- ".print_r($e->getMessage(),true),1);
			}			
		}
		$statisticsArray = array();
		$statisticsArray[] = t('Enrolled').': '.$enrolled;
		$statisticsArray[] = t('In progress').': '.$inprogress;
		$statisticsArray[] = t('Completed').': '.$completed;
		$statisticsArray[] = t('Incomplete').': '.$incomplete;
		$statisticsArray[] = t('Pending').': '.$pending;
		$statisticsArray[] = t('Canceled').': '.$cancelled;

		$countResult = implode('<span class="user-enrollment-pipe-cls">|</span> ',$statisticsArray);
		echo drupal_json_encode(array("resultCount"=>$countResult));

	}catch (Exception $ex) {
		watchdog_exception('fetchUserEnrollmentStatusCount', $ex);
		expertusErrorThrow($ex);
	}
}//end fetchUserEnrollmentStatusCount
/*
function  fetchGrpOwnerUserList($grpId,$searchKeyword,$type,$count = '',$stype = '',$autosearch = ''){
	try {
		expDebug::dPrint("type---->".$type);
		if($stype == ''){
		   $searchType = $_GET['searhType'];
		}else{
			$searchType = $stype;
		}
// 		$qry1 = db_select('slt_group_privilege','priv');
// 		$qry1->addExpression('group_concat(priv.page_code)','pagecode');
// 		$qry1->condition('priv.group_id',$grpId,'=');		
// 		$qry1Result = $qry1->execute()->fetchfield();

// 	 	expDebug::dPrint("search pagecode[]---->".print_r($qry1Result,true),5);
// 	  $qry2 = db_select('slt_group_privilege','priv');
// 	  $qry2->addField('priv','group_id','group_id');
// 	  $qry2->addField('grp','userslist','userslist');
// 	  $qry2->addExpression('group_concat(priv.page_code)','pagecode');
// 	  $qry2->leftjoin('slt_groups','grp','grp.id = priv.group_id');
// 	  $qry2->condition('priv.page_code',NULL,'is not');
// 	  $qry2->condition('priv.page_code','','!=');
// 	  $qry2->groupBy('priv.group_id');	
// 	  $qry2Result = $qry2->execute()->fetchAll();

// 	  foreach ($qry2Result as $key=>$searchResult){
// 	  	$a1 = explode(",",$searchResult->pagecode);
// 	  	$a2 = explode(",",$qry1Result);
// 	  	$result=array_intersect($a1,$a2);
// 	  	$tt = (is_array($result) && is_array($a2) && array_diff($result, $a2) === array_diff($a2, $result));	  	
// 	  	if($tt == 1){
// 	  	   expDebug::dPrint("search type---->".print_r($searchResult->userslist,true),5);
// 	  	   $userslist[] = $searchResult->userslist;

// 	  	}
// 	  }
	
// 		expDebug::dPrint("search type---->".print_r($userslist,true),5);
		$userId = array(1,2);
		$select = db_select('slt_person','per');		
		$select->leftjoin('slt_groups','grp','FIND_IN_SET(per.id, grp.userslist) > 0 AND (grp.status = \'cre_sec_sts_atv\') ');	
		$select->leftjoin('slt_owner_details','owner','per.id = owner.owner_id AND owner.group_id = '.$grpId.'');
		$select->distinct();
		if(empty($count)){
			$select->addField('per','id','user_id');
			$select->addField('per','user_name','uname');
			$select->addField('per','full_name','fname');
			if($autosearch == 'autosearch'  && $searchType == 'grptitle'){
			   $select->addField('grp','name','gname');
			}else {
			   $select->addExpression('group_concat(grp.name)','gname');
			}
		}else{			
			$select->addExpression('count(DISTINCT(per.id))', 'cnt');			
		}	
		   filterByUserAccessPrivileges($select,'per','',1,'cre_usr','attach','per');
		$select->condition('per.status','cre_usr_sts_atv','=');
// 		if(empty($userslist)){
// 			//$select->condition('per.id',array('""'),'IN');
// 		}else{
// 		 $select->condition('per.id',$userslist,'IN');
// 		}
		if($type == 'RoleDisplayOwners'){
			$select->where('owner.owner_id is not null');
		}else{
		  $select->where('owner.owner_id is null');
		}

	  //$select->where("FIND_IN_SET(per.id,(select grp1.userslist from slt_groups grp1 where grp1.id=$grpId)) <=0");
		$select->condition('per.id',$userId,'not in');
		//$select->condition('grp.status','cre_sec_sts_atv','=');
		if(empty($count)){
			if($autosearch == 'autosearch' && $searchType == 'grptitle'){
				$select->groupBy('grp.name');
			}else{
			   $select->groupBy('per.user_name');
			}
		}
		if($searchType == 'grptitle'){
			if((!empty($searchKeyword)) && ($searchKeyword != 'undefined') && $searchKeyword !=t('LBL1270') && $searchKeyword !=t('Type a group name')){
				$select->condition(db_or()->condition('grp.name', '%' . db_like($searchKeyword) . '%' ,'like')
						->condition('grp.name', '%' . db_like($searchKeyword) . '%' ,'like'));
			}
		}else{
			if((!empty($searchKeyword)) && ($searchKeyword != 'undefined') && $searchKeyword !=t('LBL181') && $searchKeyword !=t('Type s User name')){
				$select->condition(db_or()->condition('per.user_name', '%' . db_like($searchKeyword) . '%' ,'like')
						->condition('per.user_name', '%' . db_like($searchKeyword) . '%' ,'like'));
			}
		}	
		expDebug::dPrintDBAPI('list of users in grousdasdsap' ,   $select);
		return $select;
	}catch (Exception $ex) {
		watchdog_exception('fetchGrpOwnerUserList', $ex);
		expertusErrorThrow($ex);
	}
}*/
/**
 * returns list of users who are not already a member of the given $grpId
 * @param unknown $grpId
 * @param unknown $searchKeyword
 * @param unknown $type
 * @param string $count
 * @return SelectQuery
 */
function fetchGrpUserList($grpId,$searchKeyword,$type,$count = ''){
	try {
		$searchType = $_GET['searhType'];
		include_once(drupal_get_path('module', 'exp_sp_administration_groups') .'/exp_sp_administration_groups.inc');
		expDebug::dPrint('typeee'.$type);
		expDebug::dPrint('search'.$searchKeyword);
		//$grpCode = getGrpCode($grpId);
		$grpDetails = getGrpDetails($grpId,
		array('code',
		 				'is_admin',
		 				'org_id',
		 				'user_type',
		 				'employment_type',
		 				'country',
		 				'state',
		 				'department',
		 				'job_role',
		 				'language',
		 				'is_manager',
		 				'is_instructor'
		));
			
		$grpCode=$grpDetails['code'];
		$isadm = $grpDetails['is_admin'];
		
		// Get hire date details
		$hireDetails = getHireDetails($grpId);
		
		$select1 = db_select('slt_person','per');
		$select1->innerjoin('users','usr','usr.name = per.user_name');
		$select1->condition('per.status','cre_usr_sts_atv','=');
		if(!empty($count)){
			$select1->addExpression('count(distinct per.id)', 'cnt');
		}else{
			$select1->addField('per','id','user_id');
			$select1->addField('usr','uid','uid');
			$select1->addField('usr','status','status');
			if(empty($fetchId)){
				$select1->addField('per','user_name','uname');
				$select1->addField('per','full_name','fname');
			}
		}
		
		
		
		$userId = array(1,2);
		$select = db_select('slt_person','persn');
		$select->innerjoin('slt_group_user_mapping','gmap','gmap.user_id = persn.id');
		$select->innerjoin('users','usr','usr.name = persn.user_name');
		$select->condition('persn.id',$userId,'not in');
		$select->condition('persn.status','cre_usr_sts_atv','=');
		$select->condition('gmap.group_id',$grpId,'=');
		$select->addField('persn','id','user_id');
		$select->condition('gmap.user_type','R','!=');
		$select->where('persn.id = per.id');
		$select1->notExists($select);
		
		filterByUserAccessPrivileges($select1,'per','',1,'cre_usr','attach','per');
		/* $valCnt = (empty($grpDetails['org_id']) && empty($grpDetails['user_type']) && empty($grpDetails['employment_type']) && empty($grpDetails['country'])
		&& empty($grpDetails['state']) && empty($grpDetails['department']) && empty($grpDetails['job_role']) && empty($grpDetails['language'])
		&& $grpDetails['is_manager']== 'N'
		&& $grpDetails['is_instructor'] == 'N' && (count($hireDetails) == 0)) ? 1 : 0;
			
		expDebug::dPrint("testing by priya ");
		
		$userId = array(1,2);
		$select1 = db_select('slt_person','per');
		$select1->join('slt_groups','grp');
		$select1->leftjoin('slt_group_attributes','grpatt' ,'grpatt.group_id = grp.id');
		
		$select1->distinct();
		if(empty($count)){
			$select1->addField('per','id','user_id');
			$select1->addField('per','user_name','uname');
			$select1->addField('per','full_name','fname');
			if($searchType == 'grptitle'){
				$select1->addField('grp','name','gname');
			}
		}else{
			$select1->addExpression('count(DISTINCT(per.id))', 'cnt');
		}
		$select1->condition('per.id',$userId,'not in');
		$select1->condition('grp.id',$grpId,'=');
		$select1->condition('per.status','cre_usr_sts_atv','=');
		filterByUserAccessPrivileges($select1,'per','',1,'cre_usr','attach','per'); // #46933 - group access visiblity check
		if($grpCode != 'grp_ins' && $grpCode != 'grp_mgr' && $grpCode != 'grp_adm' && $grpCode != 'grp_sup'){
			if($isadm){
				$flg=0;
				$select=array();
				if(!empty($grpDetails['org_id'])){
					$select[] = ("if(grp.org_id='All',per.org_id is null,FIND_IN_SET(ifnull(per.org_id,''),grp.org_id)<=0)");
					$flg=1;
				}
				if(!empty($grpDetails['user_type'])){
					$select[] = ("if(grp.user_type='All',per.user_type is null,FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)<=0 )");
					$flg=1;
				}
				if(!empty($grpDetails['employment_type'])){
					$select[] = ("if(grp.employment_type='All',per.employment_type is null,FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)<=0)");
					$flg=1;
				}
				if(!empty($grpDetails['country'])){
					$select[] = ("if(grp.country='All',per.country is null,FIND_IN_SET(ifnull(per.country,''),grp.country)<=0)");
					$flg=1;
				}
				if(!empty($grpDetails['state'])){
					$select[] = ("if(grp.state='All',per.state is null,FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)<=0)");
					$flg=1;
				}
				if(!empty($grpDetails['department'])){
					$select[] = ("if(grp.department='All',per.dept_code is null,FIND_IN_SET(ifnull(per.dept_code,''),grp.department)<=0)");
					$flg=1;
				}
				if(!empty($grpDetails['job_role'])){
					//$select1->leftjoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id != per.id and lpjm.job_role not in(grp.job_role)');
					$select[] = ("if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id=per.id)<=0,(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id=per.id and FIND_IN_SET(jobmap.job_role,ifnull(grp.job_role,''))>0)<=0)");
					//$select[] = ("if(grp.job_role='All',per.job_role is null,FIND_IN_SET(ifnull(per.job_role,''),grp.job_role)<=0)");
					$flg=1;
				}
				if(!empty($grpDetails['language'])){
					$select[] = ("if(grp.language='All',per.preferred_language is null,FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)<=0)");
					$flg=1;
				}
				if($grpDetails['is_manager']=='Y' && $grpDetails['is_instructor']=='Y'){
				  $select[] = ("ifnull(per.is_manager,'N') != grp.is_manager AND ifnull(per.is_instructor,'N') != grp.is_instructor");
				  $flg=1;
				}
				if($grpDetails['is_instructor']=='Y' && $grpDetails['is_manager']=='N'){
				  $select[] = ("ifnull(per.is_instructor,'N') != grp.is_instructor");
				  $flg=1;
				}
				if($grpDetails['is_manager']=='Y' && $grpDetails['is_instructor']=='N'){
				  $select[] = ("ifnull(per.is_manager,'N') != grp.is_manager");
				  $flg=1;
				}
				if(count($hireDetails) > 0){
	   				$ordbVal = '';
	   				if(!empty($hireDetails[0]->between_start_date) && !empty($hireDetails[0]->between_end_date)) {
	   					 $ordbVal .= empty($ordbVal) ? ("(per.hire_date is NULL OR DATE_FORMAT(per.hire_date,'%Y-%m-%d') < (DATE_FORMAT('".$hireDetails[0]->between_start_date."','%Y-%m-%d')) OR DATE_FORMAT(per.hire_date,'%Y-%m-%d') > (DATE_FORMAT('".$hireDetails[0]->between_end_date."','%Y-%m-%d')))") : " AND ". ("(per.hire_date is NULL OR DATE_FORMAT(per.hire_date,'%Y-%m-%d') < (DATE_FORMAT('".$hireDetails[0]->between_start_date."','%Y-%m-%d')) OR DATE_FORMAT(per.hire_date,'%Y-%m-%d') > (DATE_FORMAT('".$hireDetails[0]->between_end_date."','%Y-%m-%d')))");
				}
	   				if(!empty($hireDetails[0]->on_or_after_start_date)){
	   					 $ordbVal .= empty($ordbVal) ? ("(per.hire_date is NULL OR DATE_FORMAT(per.hire_date,'%Y-%m-%d') <= (DATE_FORMAT('".$hireDetails[0]->on_or_after_start_date."','%Y-%m-%d')))") : " AND ".("(per.hire_date is NULL OR DATE_FORMAT(per.hire_date,'%Y-%m-%d') <= (DATE_FORMAT('".$hireDetails[0]->on_or_after_start_date."','%Y-%m-%d')))");
	   				}
	   				if(!empty($hireDetails[0]->on_or_before_start_date)){
	   					$ordbVal .= empty($ordbVal) ? ("(per.hire_date is NULL OR DATE_FORMAT(per.hire_date,'%Y-%m-%d') >= (DATE_FORMAT('".$hireDetails[0]->on_or_before_start_date."','%Y-%m-%d')))") : " AND ".("(per.hire_date is NULL OR DATE_FORMAT(per.hire_date,'%Y-%m-%d') >= (DATE_FORMAT('".$hireDetails[0]->on_or_before_start_date."','%Y-%m-%d')))");
	   				}
	   				$select[] = '('.$ordbVal.')';
	   				$flg=1;
	   			}
	   			expDebug::dPrintDBAPI('select list >> ',$select1, 5);
				if($flg==1){
					$cond = implode(' OR ',$select);
					$select1->where("((".$cond." OR ((FIND_IN_SET(per.id,grp.removed_users)> 0))) AND (FIND_IN_SET(per.id,grp.added_users)<=0 or grp.added_users is null))");
				}else{
					$select1->where("(if(grp.added_users is not null,(FIND_IN_SET(per.id,grp.added_users)<=0),1=1))");
				}
			}else{
				if($valCnt){
					$select1->where("(((((
							if(grp.org_id='All',per.org_id is null,(case when (grp.org_id is not null) then FIND_IN_SET(ifnull(per.org_id,'T'),ifnull(grp.org_id,'T'))<=0 else 1=2 end)))
							AND (if(grp.user_type='All',per.user_type is null,(case when (grp.user_type is not null) then FIND_IN_SET(ifnull(per.user_type,'T'),ifnull(grp.user_type,'T'))<=0 else 1=2 end)))
							AND (if(grp.employment_type='All',per.employment_type is null,(case when (grp.employment_type is not null) then FIND_IN_SET(ifnull(per.employment_type,'T'),ifnull(grp.employment_type,'T'))<=0 else 1=2 end)))
							AND (if(grp.country='All',per.country is null,(case when (grp.country is not null) then FIND_IN_SET(ifnull(per.country,'T'),ifnull(grp.country,'T'))<=0 else 1=2 end)))
							AND (if(grp.state='All',per.state is null,(case when (grp.state is not null) then FIND_IN_SET(ifnull(concat(per.country,'-',per.state),'T'),ifnull(grp.state,'T'))<=0 else 1=2 end)))
							AND (if(grp.department='All',per.dept_code is null,(case when (grp.department is not null) then FIND_IN_SET(ifnull(per.dept_code,'T'),ifnull(grp.department,'T'))<=0 else 1=2 end)))
							OR (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where per.id= jobmap.user_id)<=0,(case when (grp.job_role is not null) then ((select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id = per.id and FIND_IN_SET(ifnull(jobmap.job_role,'T'),ifnull(grp.job_role,'T'))>0)<=0) else 1=2 end)))
							AND (if(grp.language='All',per.preferred_language is null,(case when (grp.language is not null) then FIND_IN_SET(ifnull(per.preferred_language,'T'),ifnull(grp.language,'T'))<=0 else 1=2 end)))
					        AND(CASE WHEN (grp.is_manager='Y' AND grp.is_instructor='Y') 
                                                    THEN
                                                    (ifnull(per.is_manager,'N') != grp.is_manager and ifnull(per.is_instructor,'N') != grp.is_instructor)
                                                    WHEN (grp.is_manager='Y' AND grp.is_instructor='N') 
                                                    THEN
                                                    (ifnull(per.is_manager,'N') != grp.is_manager)
                                                    WHEN (grp.is_manager='N' AND grp.is_instructor='Y') 
                                                    THEN 
                                                    (ifnull(per.is_instructor,'N') != grp.is_instructor)
                                                    ELSE
                                                    1=1
                                                    END)
							OR (if(grpatt.id is not null,if(per.hire_date is not null,(if( grpatt.on_or_after_start_date is not null,DATE_FORMAT(per.hire_date,'%Y-%m-%d') <= DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d'),1 = 0)
            					OR if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') <= DATE_FORMAT(per.hire_date,'%Y-%m-%d'),1 = 0) OR if(grpatt.between_start_date is not null,
            					DATE_FORMAT(per.hire_date,'%Y-%m-%d') < DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') OR DATE_FORMAT(per.hire_date,'%Y-%m-%d') > DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'),1 = 0)),1=1),1 = 2))
			   				OR (FIND_IN_SET(per.id,grp.removed_users)> 0)) AND (FIND_IN_SET(per.id,grp.added_users)<=0 or grp.added_users is null ))))");
				}else{
					
					
					if(count($hireDetails) > 0){
		   				$ordbVal = '';
		   				if(!empty($hireDetails[0]->between_start_date) && !empty($hireDetails[0]->between_end_date)) {
		   					 $ordbVal .= empty($ordbVal) ? ("(UNIX_TIMESTAMP(per.hire_date) < UNIX_TIMESTAMP(DATE_FORMAT('".$hireDetails[0]->between_start_date."','%Y-%m-%d'))) AND UNIX_TIMESTAMP(per.hire_date) > UNIX_TIMESTAMP(DATE_FORMAT('".$hireDetails[0]->between_end_date."','%Y-%m-%d')))") : " OR ". ("(UNIX_TIMESTAMP(per.hire_date) between UNIX_TIMESTAMP(DATE_FORMAT('".$hireDetails[0]->between_start_date."','%Y-%m-%d')) AND UNIX_TIMESTAMP(DATE_FORMAT('".$hireDetails[0]->between_end_date."','%Y-%m-%d')))");
		   				} 
		   				if(!empty($hireDetails[0]->on_or_after_start_date)){
		   					 $ordbVal .= empty($ordbVal) ? ("UNIX_TIMESTAMP(per.hire_date) < UNIX_TIMESTAMP(DATE_FORMAT('".$hireDetails[0]->on_or_after_start_date."','%Y-%m-%d')))"): " OR ".("UNIX_TIMESTAMP(per.hire_date) < UNIX_TIMESTAMP(DATE_FORMAT('".$hireDetails[0]->on_or_after_start_date."','%Y-%m-%d')))");
		   				} 
		   				if(!empty($hireDetails[0]->on_or_before_start_date)){
		   					$ordbVal .= empty($ordbVal) ? ("UNIX_TIMESTAMP(per.hire_date) > UNIX_TIMESTAMP(DATE_FORMAT('".$hireDetails[0]->on_or_before_start_date."','%Y-%m-%d')))"): " OR ".("UNIX_TIMESTAMP(per.hire_date) > UNIX_TIMESTAMP(DATE_FORMAT('".$hireDetails[0]->on_or_before_start_date."','%Y-%m-%d')))");
		   				}
		   				$or_cond = '('.$ordbVal.')';
		   			}else{
		   				$or_cond = "(1=1)";
		   			}
					
				$select1->where("((((((if(grp.org_id='All',per.org_id is null,(case when (grp.org_id is not null) then FIND_IN_SET(ifnull(per.org_id,'T'),ifnull(grp.org_id,'T'))<=0 else 1=2 end)))
							OR (if(grp.user_type='All',per.user_type is null,(case when (grp.user_type is not null) then FIND_IN_SET(ifnull(per.user_type,'T'),ifnull(grp.user_type,'T'))<=0 else 1=2 end)))
							OR (if(grp.employment_type='All',per.employment_type is null,(case when(grp.employment_type is not null) then FIND_IN_SET(ifnull(per.employment_type,'T'),ifnull(grp.employment_type,'T'))<=0 else 1=2 end)))
							OR (if(grp.country='All',per.country is null,(case when (grp.country is not null) then FIND_IN_SET(ifnull(per.country,'T'),ifnull(grp.country,'T'))<=0 else 1=2 end)))
							OR (if(grp.state='All',per.state is null,(case when (grp.state is not null) then FIND_IN_SET(ifnull(concat(per.country,'-',per.state),'T'),ifnull(grp.state,'T'))<=0 else 1=2 end)))
							OR (if(grp.department='All',per.dept_code is null,(case when (grp.department is not null) then FIND_IN_SET(ifnull(per.dept_code,'T'),ifnull(grp.department,'T'))<=0 else 1=2 end)))
							OR (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where per.id= jobmap.user_id)<=0,(case when (grp.job_role is not null) then ((select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id = per.id and FIND_IN_SET(ifnull(jobmap.job_role,'T'),ifnull(grp.job_role,'T'))>0)<=0) else 1=2 end)))
							OR (if(grp.language='All',per.preferred_language is null,(case when (grp.language is not null) then FIND_IN_SET(ifnull(per.preferred_language,'T'),ifnull(grp.language,'T'))<=0 else 1=2 end)))
							
				            OR (CASE WHEN (grp.is_manager='Y' AND grp.is_instructor='Y') 
                                                    THEN
                                                    (ifnull(per.is_manager,'N') != grp.is_manager AND ifnull(per.is_instructor,'N') != grp.is_instructor)
                                                    WHEN (grp.is_manager='Y' AND grp.is_instructor='N') 
                                                    THEN
                                                    (ifnull(per.is_manager,'N') != grp.is_manager)
                                                    WHEN (grp.is_manager='N' AND grp.is_instructor='Y') 
                                                    THEN 
                                                    (ifnull(per.is_instructor,'N') != grp.is_instructor)
                                                    ELSE
                                                    1=2
                                                    END)
				    		OR (if(grpatt.id is not null,if(per.hire_date is not null,(if( grpatt.on_or_after_start_date is not null,DATE_FORMAT(per.hire_date,'%Y-%m-%d') <= DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d'),1 = 0)
            					OR if(grpatt.on_or_before_start_date is not null,DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') <= DATE_FORMAT(per.hire_date,'%Y-%m-%d'),1 = 0) OR if(grpatt.between_start_date is not null,
            					DATE_FORMAT(per.hire_date,'%Y-%m-%d') < DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') OR DATE_FORMAT(per.hire_date,'%Y-%m-%d') > DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'),1 = 0)),1=1),1 = 2))
							OR (FIND_IN_SET(per.id,grp.removed_users)> 0))
							AND (FIND_IN_SET(per.id,grp.added_users)<=0 or grp.added_users is null )))))"); 
				}
			}
		}else{
			if($grpCode == 'grp_ins'){
				$select1->where("(FIND_IN_SET(per.id,grp.removed_users)>0) or (per.is_instructor='N')");
				//$select1->condition('per.is_instructor','N','=');
			}
			else if($grpCode == 'grp_mgr'){
				$select1->where("(FIND_IN_SET(per.id,grp.removed_users)>0) or (per.is_manager ='N')");
				//$select1->condition('per.is_manager',0);
			}
			else{
				$select1->where("((FIND_IN_SET(per.id,grp.removed_users) >0 or grp.removed_users is null) AND (FIND_IN_SET(per.id,grp.added_users)<=0 or grp.added_users is null))");
			}
		} */
		
		if((!empty($searchKeyword)) && ($searchKeyword != 'undefined') && $searchKeyword !=t('LBL181') && $searchKeyword !=t('Type Full Name')){
			if($searchType=='usrtit' || $searchType=='usrtitle' ){
				$select1->condition('per.user_name', '%' . db_like($searchKeyword) . '%' ,'like');
			}else{
				$select1->condition('per.full_name', '%' . db_like($searchKeyword) . '%' ,'like');
				
			}
		}
		
		expDebug::dPrintDBAPI('no users under the role priya ' ,   $select1);
		return $select1;
		/*
		 $roleusers = $select1->execute()->fetchAll();
		 expDebug::dPrintDBAPI('$roleusers=getting query' ,   $select1);
		 expDebug::dPrint('rolefrompersonresultss'.print_r($roleusers,true),4);
		 return $roleusers;*/
	} catch (Exception $ex) {
		watchdog_exception('fetchGrpUserList', $ex);
		expertusErrorThrow($ex);
	}
}

function isRegFormCart($masEnrId){
	try {
		$selectEnrolledOrderId = db_select('slt_enrollment', 'enr');
		$selectEnrolledOrderId->leftjoin('slt_order','sor','enr.order_id = sor.id');
		$selectEnrolledOrderId->addField('sor','uc_order_id');
		$selectEnrolledOrderId->condition('enr.master_enrollment_id', $masEnrId, '=');
		$selectEnrolledOrderId->range(0,1);
		$ucOrderId  = $selectEnrolledOrderId->execute()->fetchField();
		return ($ucOrderId) ? 'Cart' : '';
	} catch (Exception $ex) {
		watchdog_exception('isRegFormCart', $ex);
		expertusErrorThrow($ex);
	}
}

/* Not used anywhere - #0046888
 * function fetchUserDetailList($roleId){
	try {
		$userId = array(1,2);
		$select = db_select('slt_person','pre');
		$select->addField('pre','id','user_id');
		$select->addField('pre','user_name','uname');
		$select->addField('pre','full_name','fname');
		$select->addField('pli','name','status');
		$select->join('slt_profile_list_items','pli','pre.status=pli.code');
		$select->condition('pre.id',$userId,'not in');
		$select->where("FIND_IN_SET(pre.id,(select ifnull(r.userlist,0) from slt_security_role r where id=$roleId))>0");
		expDebug::dPrintDBAPI('list of users in role' ,   $select);

		return $select;
	} catch (Exception $ex) {
		watchdog_exception('fetchUserDetailList', $ex);
		expertusErrorThrow($ex);
	}
}*/
function getEnrollmentsResultForViewScreen($classId,$keyword=''){
	try{
		$page      = $_GET['page'];
		$limit     = $_GET['rows'];
		$sidx      = $_GET['sidx'];
		$sord      = $_GET['sord'];
		if((!empty($limit) && !is_numeric($limit)) || !empty($page) && !is_numeric($page)){
			throw new Exception('Non Numeric Values');
		}
		
		if(!empty($sidxVal)){
			$sidx = escape_string($sidxVal);
			$_GET['sidx'] = $sidx;
		}

		$sord      = $_GET['sord'];
		if(!empty($sord)){
			if((stripos('desc',$sord) !== false)){
				$sord = 'desc';
			}else{
				$sord = 'asc';
			}
			$_GET['sord'] = $sord;
		}
		
		$count      = getClassEnrollmentResults($classId, true);

		$xorderBy = 'full_name';
		if($sidx !=''){
			$xorderBy = $sidx;
		}

		if( $count >0 ){
			$total_pages = ceil($count/$limit);
		}else{
			$total_pages = 0;
		}
		if($page > $total_pages)
		$page = $total_pages;

		$start = $limit * $page - $limit;
		if($start < 0) {
			$start = 0;
		}
		$listquery  = getClassEnrollmentResults($classId);
		$listquery->range($start, $limit);
		$listquery->orderBy($xorderBy,$sord);
		expDebug::dPrintDBAPI(' getOrgUsersListForGrid $listquery = ', $listquery);
		$queryResult = $listquery->execute();

		$srch_result_response_roster->total   = $total_pages;
		$srch_result_response_roster->records = $count;
		$srch_result_response_roster->page    = $page;
		$srch_result_response_roster->unique_id    = $classId.'-viewenroll';
		$srch_result_response_roster->clsid    = $classId;
		$srch_result_response_roster->type    = 'viewenroll';
		$_SESSION['jq_count'] = array();
		$i=0;
		foreach ($queryResult as $rec_search_results){
			expDebug::dPrint('$rec_search_results'.print_r($rec_search_results,true),5);
			$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->id;
			expDebug::dPrint('$rec_search_results->status : '.print_r($rec_search_results->status , true), 4);
			// Multilangauage support for this ticket #0048749
			$rec_search_results->status = ($rec_search_results->status == 'Reserved') ? t('LBL942') : (($rec_search_results->status == 'ReservedCancelled') ? t('LBL942').' '.t('Canceled') : $rec_search_results->status);
			$srch_result_response_roster->rows[$i]['cell'] = array(
					'id' 	      => $rec_search_results->id,
					'full_name'   => '<span class="vtip" title="'.sanitize_data($rec_search_results->full_name).'">'.titleController('ADMIN-LEARNING-ENROLLMENTS-RESULT-FOR-VIEW-FULLNAME', $rec_search_results->full_name,13).'</span>',
					'user_name'   => '<span class="vtip" title="'.sanitize_data($rec_search_results->user_name).'">'.titleController('ADMIN-LEARNING-ENROLLMENTS-RESULT-FOR-VIEW-USERNAME', $rec_search_results->user_name,13).'</span>',
					'status' 	  => '<span class="vtip" title="'.sanitize_data(t($rec_search_results->status)).'">'.titleController('ADMIN-LEARNING-ENROLLMENTS-RESULT-FOR-VIEW-STATUS',t($rec_search_results->status),10).'</span>',
					'score' 	  => '<span class="vtip" title="'.sanitize_data($rec_search_results->score).'">'.titleController('ADMIN-LEARNING-ENROLLMENTS-RESULT-FOR-VIEW-SCORE', $rec_search_results->score,8).'</span>',
			);
			$i++;
		}
		if(empty($count)){
			$srch_result_response_roster->msg  = t('MSG403');//There are no records found
		}
		expDebug::dPrint('$count$count$count : '.$count , 4);
		expDebug::dPrint('$srch_result_response_roster->'.print_r($srch_result_response_roster,true),5);
		echo drupal_json_encode($srch_result_response_roster);
	}catch (Exception $ex) {
		watchdog_exception('getEnrollmentsResultForViewScreen', $ex);
		expertusErrorThrow($ex);
	}
}
function getClassEnrollmentResults($classId, $getCount=false){
	try{
		$select = db_select('slt_enrollment','enroll');
		$select->join('slt_person','per','per.id = enroll.user_id');
		$select->leftjoin('slt_profile_list_items','items_reg','items_reg.code = enroll.reg_status');
		$select->leftjoin('slt_profile_list_items','items_comp','items_comp.code = enroll.comp_status');
		$select->leftjoin('slt_profile_list_items','items_ovr','items_ovr.code = if((enroll.reg_status=\'lrn_crs_reg_cnf\'),if((enroll.comp_status=\'lrn_crs_cmp_inp\' OR enroll.comp_status=\'lrn_crs_cmp_att\'),\'lrn_crs_cmp_enr\',enroll.comp_status),enroll.reg_status)');
		if($getCount){
			$select->addExpression('count(enroll.id)', 'count');
			expDebug::dPrintDBAPI('count 2222',$select);
		} else {
			$select->addField('enroll','id','id');
			$select->addField('enroll','master_enrollment_id','master_enr_id');
			$select->addField('per','user_name','user_name');
			$select->addField('per','full_name','full_name');
			$select->addField('enroll','comp_status','comp_status');
			$select->addField('enroll','score','score');
			$select->addField('items_reg','name','reg_status_name');
			$select->addField('items_comp','name','comp_status_name');
			$select->addField('enroll','created_on','created_on');
			$select->addField('enroll','updated_on','updated_on');
			$select->addField('items_ovr','name','overall_status');
			$select->addExpression('IF(enroll.comp_status NOT IN(\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\'),items_ovr.name,ifnull((SELECT if((exmp.exempted_status = 1  and enroll.comp_status!=\'\') ,\'Waived\',NULL) FROM slt_enrollment_exempted exmp WHERE exmp.enrollment_id = enroll.id AND exmp.enroll_type = \'class\' ORDER BY id DESC LIMIT 1),items_ovr.name))', 'status');
		}
		$select->condition('enroll.class_id',$classId,'=');
		$select->condition('enroll.reg_status','lrn_crs_reg_rsc','!=');
		$select->groupBy('enroll.user_id');
		$select->groupBy('enroll.comp_status');
		expDebug::dPrintDBAPI('$select query for view',$select);
		if($getCount){
			$count = $select->countQuery()->execute()->fetchField();
			$result= $count;
		}else {
			$result=  $select;
				
		}
		expDebug::dPrint('$result$result1111'.print_r($result,true),4);
		return $result;

	}catch (Exception $ex){
		watchdog_exception('getClassEnrollmentResults', $ex);
		expertusErrorThrow($ex);
	}
}

function getMasterEnrollmentsResultForViewScreen($prgId,$keyword=''){
	try{
		expDebug::dPrint('$prgId-->'.$prgId);
		$page      = $_GET['page'];
		$limit     = $_GET['rows'];
		$sidx      = $_GET['sidx'];
		$sord      = $_GET['sord'];
		if((!empty($limit) && !is_numeric($limit)) || !empty($page) && !is_numeric($page)){
			throw new Exception('Non Numeric Values');
		}
		
		if(!empty($sidxVal)){
			$sidx = escape_string($sidxVal);
			$_GET['sidx'] = $sidx;
		}

		$sord      = $_GET['sord'];
		if(!empty($sord)){
			if((stripos('desc',$sord) !== false)){
				$sord = 'desc';
			}else{
				$sord = 'asc';
			}
			$_GET['sord'] = $sord;
		}
		
		$count      = getTpEnrollResultForViewScreen($prgId, true);

		$xorderBy = 'full_name';
		if($sidx !=''){
			$xorderBy = $sidx;
		}
		if( $count >0 ){
			$total_pages = ceil($count/$limit);
		}else{
			$total_pages = 0;
		}
		if($page > $total_pages)
		$page = $total_pages;

		$start = $limit * $page - $limit;
		if($start < 0) {
			$start = 0;
		}
		$listquery  = getTpEnrollResultForViewScreen($prgId);
		$listquery->range($start, $limit);
		$listquery->orderBy($xorderBy,$sord);
		expDebug::dPrintDBAPI(' getOrgUsersListForGrid $listquery = ', $listquery);
		$queryResult = $listquery->execute();

		$srch_result_response_roster->total   = $total_pages;
		$srch_result_response_roster->records = $count;
		$srch_result_response_roster->page    = $page;
		$srch_result_response_roster->unique_id    = core_encrypt($prgId).'-viewenroll';
		$srch_result_response_roster->clsid    = core_encrypt($prgId);
		$srch_result_response_roster->type    = 'viewenroll';
		$_SESSION['jq_count'] = array();
		$i=0;
		foreach ($queryResult as $rec_search_results){
			expDebug::dPrint('$rec_search_results'.print_r($rec_search_results,true),5);
			$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->id;
			$srch_result_response_roster->rows[$i]['cell'] = array(
					'id' 	      => $rec_search_results->id,
					'full_name'   => '<span class="vtip" title="'.sanitize_data($rec_search_results->full_name).'">'.titleController('ADMIN-LEARNING-MASTERENROLLMENTS-RESULT-FOR-VIEW-FULLNAME', $rec_search_results->full_name,13).'</span>',
					'user_name'   => '<span class="vtip" title="'.sanitize_data($rec_search_results->user_name).'">'.titleController('ADMIN-LEARNING-MASTERENROLLMENTS-RESULT-FOR-VIEW-USERNAME', $rec_search_results->user_name,13).'</span>',
					'path'   => '<span class="vtip" title="'.sanitize_data($rec_search_results->path).'">'.titleController('ADMIN-LEARNING-MASTERENROLLMENTS-RESULT-FOR-VIEW-PATH', $rec_search_results->path,13).'</span>',
					'status' 	  => '<span class="vtip" title="'.sanitize_data(t($rec_search_results->status)).'">'.titleController('ADMIN-LEARNING-MASTERENROLLMENTS-RESULT-FOR-VIEW-STATUS',t($rec_search_results->status),10).'</span>',
					'score' 	  => '<span class="vtip" title="'.sanitize_data($rec_search_results->score).'">'.titleController('ADMIN-LEARNING-MASTERENROLLMENTS-RESULT-FOR-VIEW-SCORE', $rec_search_results->score,8).'</span>',

			);
			$i++;
		}
		if(empty($count)){
			$srch_result_response_roster->msg  = t('MSG403');//There are no records found
		}
		expDebug::dPrint('$count$count$count'.$count);
		expDebug::dPrint('$srch_result_response_roster->'.print_r($srch_result_response_roster,true),5);
		echo drupal_json_encode($srch_result_response_roster);
	}catch (Exception $ex) {
		watchdog_exception('getMasterEnrollmentsResultForViewScreen', $ex);
		expertusErrorThrow($ex);
	}
}
function getTpEnrollResultForViewScreen($prgId, $getCount=false){
	try{
		expDebug::dPrint('$result$$getCount'.$getCount,4);
		$select = db_select('slt_master_enrollment','masterenroll');
		$select->join('slt_person','per','per.id = masterenroll.user_id');
		$select->leftjoin('slt_program', 'prg', 'prg.id = masterenroll.program_id');
		$select->leftjoin('slt_profile_list_items', 'spli_status','spli_status.code = masterenroll.overall_status');
		$select->leftjoin('slt_module','module','module.program_id = prg.id and module.sequence = masterenroll.recertify_path');
		if($getCount){
			$select->addExpression('count(masterenroll.id)', 'count');
			expDebug::dPrintDBAPI('count 2222',$select);
		} else {
			$select->addField('masterenroll','id','id');
			$select->addField('per','user_name','user_name');
			$select->addField('per','full_name','full_name');
			$select->addField('module','title','path');
			$select->addField('masterenroll','overall_status','overall_status');
			$select->addExpression('if(masterenroll.score = 0,null,masterenroll.score)', 'score');
			$select->addField('masterenroll','created_on','created_on');
			$select->addField('masterenroll','updated_on','updated_on');
			$select->addExpression('IF(masterenroll.overall_status NOT IN(\'lrn_tpm_ovr_enr\',\'lrn_tpm_ovr_inp\'),spli_status.name,ifnull((SELECT if((exmp.exempted_status = 1) ,\'Waived\',NULL) FROM slt_enrollment_exempted exmp WHERE exmp.enrollment_id = masterenroll.id AND exmp.enroll_type = \'tp\' ORDER BY id DESC LIMIT 1),spli_status.name))', 'status');
		}
		$select->condition('masterenroll.program_id',$prgId,'=');
		$select->condition('masterenroll.overall_status','lrn_tpm_ovr_rsc','!=');
		expDebug::dPrintDBAPI('$select query for view111',$select);
		if($getCount){
			$result=  $select->execute()->fetchField();
		}else {
			$result=  $select;

		}
		expDebug::dPrint('$result$result'.print_r($result,true),4);
		return $result;
	}catch (Exception $ex){
		watchdog_exception('getTpEnrollResultForViewScreen', $ex);
		expertusErrorThrow($ex);
	}
}

function enrollExemptedUserGridCallback($form, &$form_state) {
	try {
		$commands = array();
		$statusMsgHTML = theme('status_messages');
		expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true),4);
		if (!empty($statusMsgHTML)) {
			if($form_state['exempted_type'] == 'all')
				$commands[] = ajax_command_html('#setwaived-visible-container #bubble-face-table #show_expertus_message', $statusMsgHTML);
			else {
				$editId = ($form_state['exempted_id']) ? ' #list-values-exempted-detail-wrapper-name ' : '';
				$commands[] = ajax_command_html('.enrolled-exempted-container '. $editId .' #bubble-face-table #show_expertus_message', $statusMsgHTML);
			}
			
		} else{
			if(!empty($form_state['exempted_id'])){
				$form_state = array(
						'no_redirect'  => TRUE,
						'build_info'   => array('args' => array('ajax')),
						'enroll_id'    => $form_state['enroll_id'],
						'enroll_type'  => $form_state['enroll_type'],
						'class_id'  => $form_state['class_id'],
				);
					
				$formId                 = 'exemptedDetailListForm';
				$builtForm              = drupal_build_form($formId, $form_state);
				$renderedTabContentHTML = drupal_render($builtForm);
				$commands[] = ajax_command_replace(NULL, $renderedTabContentHTML);
			}else{
				if($form_state['exempted_type'] == 'all'){
					$setWaivedUniqueid = ($form_state['enroll_type'] == 'class') ? $form_state['class_id'].'cre_sys_obt_cls': $form_state['class_id'].'tp';
					$commands[] = ajax_command_invoke(NULL, "exemptedQtipClose", array('enrolled-exempted-'.$form_state['class_id'],$form_state['class_id'],$form_state['exempted_type'],$setWaivedUniqueid));
				}else{
					$commands[] = ajax_command_invoke(NULL, "exemptedQtipClose", array('enrolled-exempted-'.$form_state['enroll_id'],$form_state['enroll_id']));
				}
			}
			 
		}

		return array('#type' => 'ajax', '#commands' => $commands);
	} catch (Exception $ex) {
		watchdog_exception('enrollTPUserGridCallback', $ex);
		expertusErrorThrow($ex);
	}
}

function roster_enrollment_exempted($exmType,$entId,$enrId,$type,$exmId){
	try{
		$form_state = array(
				'no_redirect'  => TRUE,
				'build_info'   => array('args' => array('ajax')),
				'exempted_type'=> $exmType,
				'class_id' => $entId,
				'enroll_id'    => $enrId,
				'enroll_type'  => $type,
				'exempted_id' => $exmId
		);
		
		$formId                 = 'roster_enrollment_exempted_form_html';
		$builtForm              = drupal_build_form($formId, $form_state);
		$renderedTabContentHTML = drupal_render($builtForm);
		$drupalSettings         = getDrupalSettings();
		
		$response       = array(
				'drupal_settings'     => $drupalSettings,
				'render_content_main' => $renderedTabContentHTML,
		);
		
		echo drupal_json_output($response);
		exit;
	} catch (Exception $ex) {
    watchdog_exception('roster_enrollment_exempted', $ex);
    expertusErrorThrow($ex);
  }
}

function roster_enrollment_exempted_details($classId,$enrId,$type,$rowNumber){
	try{
		$form_state = array(
				'no_redirect'  => TRUE,
				'build_info'   => array('args' => array('ajax')),
				'enroll_id'    => $enrId,
				'enroll_type'  => $type,
				'class_id'     => $classId,
				'row_number'   => $rowNumber,
		);
		
		$formId                 = 'exemptedDetailListForm';
		$builtForm              = drupal_build_form($formId, $form_state);
		$renderedTabContentHTML = drupal_render($builtForm);
		$drupalSettings         = getDrupalSettings();
		
		$response       = array(
				'drupal_settings'     => $drupalSettings,
				'render_content_main' => $renderedTabContentHTML,
		);
		
		echo drupal_json_output($response);
		exit;
	} catch (Exception $ex) {
		watchdog_exception('roster_enrollment_exempted', $ex);
		expertusErrorThrow($ex);
	}
}

function roster_enrollment_exempted_form_html($form, &$form_state) {
	try {

		$exemptedType = $form_state['exempted_type'];
		$enrollId = $form_state['enroll_id'];
		$enrollType = $form_state['enroll_type'];
		$exmptedId = $form_state['exempted_id'];
		$classId = $form_state['class_id'];
		expDebug::dPrint("check the status of the form state variable".print_r($form_state,1));
		$uniqueId = $enrollId.'-'.$exmptedId.'-'.$enrollType;
		$wrapperId = 'roster-enrollment-exempted-'.$enrollType.'-'.$exmptedId;
		$form_state['esign_from_plugin']= true;
		if($enrollType == 'class')
			$class = 'classenrollexempteddiv';
		else 
			$class = 'tpenrollexempteddiv';
		
		$reasonVal = '';
		
		if($exemptedType == 'single' && $exmptedId > 0){
		$exmpDets = enrollmentExemptedDetails($enrollId,$enrollType,$exmptedId);
		expDebug::dPrint("Values from the exemption function".print_r($exmpDets,true),4);
			$reasonVal = $exmpDets[0]->exempted_reason;
		}
		$form = array();
		$form['#prefix'] = '</form><div id="roster-enrollment-exempted-wrapper">';
		$form['#suffix'] = '</div>';
		$form['addedit-enroll_exempt_start'] = array(
				'#type' => 'markup',
				'#markup' => '<div id="roster-enrollment-exempted-container" class="enrollment-exempted-addedit-form-container '.$class.'">',
		);
		
		$attributes = array();
		$attributes['autocomplete'] = 'off'; // Prevent browser autocomplete
		
		$attributes['class'] = array('addedit-edit-exempted_reason'); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
		
		$attributes['class'][] = 'addedit-edit-textarea';
		$attributes['tabindex'] = 1;
		
		$form['exempted_reason'] = array(
				'#type' => 'textarea',
				'#prefix' => addEditGetPrefix('FIELD', t('Reason'), true),
				'#suffix' => '<div class="roster-exempted-message">'.t('Status will be reverted if the reason is not provided').'</div>'.addEditGetSuffix('FIELD'),
				'#title' => t('Reason'),
				'#title_display' => 'invisible',
				'#default_value' => empty($reasonVal) ? $_SESSION['exempted-'.$enrollType.'-'.$classId][$enrollId] : $reasonVal,
				'#disabled' => $readOnly,
				'#attributes' => $attributes,
				'#required' => TRUE,
				'#rows'=>'3',
				'#cols'=>'40',
		);
		
		$saveAttributes['onclick'] = array('return false;');
		$saveAttributes['class'] = array('admin-save-button-middle-bg');
		$saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
		$form['exempted-save-'.$uniqueId] = array(
				'#type' => 'submit',
				'#prefix' => '<div class="exempted-button-container"><div class="admin-exempted-save-button">',
      	'#suffix' => '</div>',
				'#value' => t('LBL569'),
				'#name' => 'exemptedandsave_'.$uniqueId,
				'#id' => 'exemptedandsave_'.$uniqueId,
				'#attributes' => $saveAttributes,
				'#ajax' => array(
						'wrapper' => 'list-values-exempted-detail-wrapper-name',
						'callback' => 'enrollExemptedUserGridCallback',
						'entity_id' => $exmptedId,
						'entity_type' => $enrollType,
						'method' => 'replace',
						'effect' => 'none', // 'fade',
						'event'=>'click',
						'keypress' => true,
						'progress' => array(
								'type' => 'throbber',
								'message' => ''
						),
				)
		);
		
		$cancelAttributes['class'] = array('addedit-edit-exempted-cancel'); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
		$cancelAttributes['class'][] = 'admin-action-button-middle-bg white-btn-bg-middle';
		$cancelAttributes['class'][] = 'addedit-form-expertusone-throbber';
		$cancelAttributes['data-wrapperid'] = array($wrapperId);
		$cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';
		if($exemptedType == 'single')
			$cancelAttributes['onclick'] = "closeQtip('enrolled-exempted-$enrollId','$enrollId');  return false; ";
		else
			$cancelAttributes['onclick'] = "closeQtip('enrolled-all-exempted-$classId','$classId');  return false; ";
		
		$form['exempted-cancel-'.$uniqueId] = array(
				'#type' => 'submit',
				'#prefix' => '<div class="admin-exempted-cancel-button">' .
				'<div class="addedit-form-cancel-container-actions enrollexempted-save-cancel-btn-container resize-save_btn4"><div class="white-btn-bg-left"></div>',
				'#suffix' =>   '<div class="white-btn-bg-right"></div></div></div></div>',
				'#value' => t('LBL123'),
				'#name' => 'cancel',
				'#attributes' => $cancelAttributes,
		);
		
		/* $setWaivedUniqueid = ($enrollType == 'class') ? $classId.'cre_sys_obt_cls': $classId;
		if($exemptedType == 'all')
			$saveAttributes['onclick'] = array("$('#waivedandsave_$setWaivedUniqueid').click(); return false;");
		else */
		
		
		$form['addedit-enroll_exempt_end'] = array(
				'#type' => 'markup',
				'#markup' => '</div>',
		);
		return $form;
	} catch (Exception $ex) {
		watchdog_exception('training_plan_enroll_form_html', $ex);
		expertusErrorThrow($ex);
	}
}

function roster_enrollment_exempted_form_html_validate($form, &$form_state){
	try{
		expDebug::dPrint("With in form validate");
		$form_state["storage"]["form_validate"] = true;
		
 		$reason = $form_state['values']['exempted_reason'];
		
 		if(strlen($reason) > 150)
 		{
 			form_set_error('attachment_url', t("!name cannot be longer than %max characters but is currently %length characters long.",array("!name"=>"Reason", "%max"=>"150", "%length"=>strlen($reason))));
 			return ;
 		}
 		$exmptedType = $form_state['exempted_type'];
		$enrollId = $form_state['enroll_id'];
		$enrollType = $form_state['enroll_type'];
		$exmptedId = $form_state['exempted_id'];
		
		/*if($exmptedType == 'single'){
			if($enrollType ==  'class') {
				$select = db_select('slt_enrollment');
				$select->addExpression('count(1)','cnt');
				$select->condition('id', $enrollId, '=');
				$select->condition('reg_status', array('lrn_crs_reg_can','lrn_crs_reg_rsc'),'NOT IN');
				$select->condition('comp_status', array('lrn_crs_cmp_cmp'),'NOT IN');
				$resultCount =  $select->execute()->fetchField();
				if($resultCount < 1) {
					expDebug::dPrint("exempted  submit enrollId entered into validation part:".print_r($enrollId,true),5);
						
					form_set_error('exempted_reason', t('Class already in completed or canceled state'));
					$form_state['esign_show_popup']= false;
					$form_state["storage"]["form_validate"] = false;
					return ;
						
				}
			}else if($enrollType ==  'tp') {
				$select = db_select('slt_master_enrollment');
				$select->addExpression('count(1)','cnt');
				$select->condition('id', $enrollId, '=');
				$select->condition('overall_status', array('lrn_tpm_ovr_cmp','lrn_tpm_ovr_rsc'),'NOT IN');
				$resultCount =  $select->execute()->fetchField();
				if($resultCount < 1) {
					expDebug::dPrint("exempted  submit enrollId entered into validation part:".print_r($enrollId,true),5);
						
					form_set_error('exempted_reason', t('Class already in completed or canceled state'));
					$form_state['esign_show_popup']= false;
					$form_state["storage"]["form_validate"] = false;
					return ;
						
				}
			}
		}*/
		
	} catch (Exception $ex) {
  	watchdog_exception('roster_enrollment_exempted_form_html_validate', $ex);
  	expertusErrorThrow($ex);
  }
}

function roster_enrollment_exempted_form_html_submit($form, &$form_state){
	try{
		
		$reason = $form_state['input']['exempted_reason'];
		
		$enrollId = $form_state['enroll_id'];
		$enrollType = $form_state['enroll_type'];
		$exmptedId = $form_state['exempted_id'];
		$classId = $form_state['class_id'];
		expDebug::dPrint("With in form submit enrollid".$enrollId);
		expDebug::dPrint("exempted  submit enrollId reason:".print_r($form_state,true),5);
		expDebug::dPrint("exempted  submit enrollType :".print_r($enrollType,true),5);

		$status = '0';
		$updateUserId = getIdOfLoggedInUser();
		
		if(!empty($reason)) {
		
			if($exmptedId == 0){
				
				expDebug::dPrint("exempted enrollment submit request array".print_r($reason,true),5);
				/* expDebug::dPrint("exempted enrollment submit request array".print_r($data,true),5);
				$insertStmt1 = db_insert('slt_enrollment_exempted');
				$insertStmt1->fields(array(
						'enrollment_id' => $enrollId,
						'enroll_type' => $enrollType,
						'exempted_status' => $status,
						'exempted_reason' => $reason,
						'is_mandatory' => $madatory,
						'is_compliance' => $compliance,
						'created_by' => $updateUserId,
						'created_on' => now(),
						'updated_by' => $updateUserId,
						'updated_on' => now()
				));
				expDebug::dPrintDBAPI('insert statement = ' , $insertStmt1);
				$userNotificationId = $insertStmt1->execute(); */
				$_SESSION['exempted-'.$enrollType.'-'.$classId][$enrollId] = $reason;
			}else{
				expDebug::dPrint("exempted enrollment submit request array else".print_r($reason,true),5);
				$updateStmt = db_update('slt_enrollment_exempted');
			 	$updateStmt->condition('enrollment_id',$enrollId,'=');
			 	$updateStmt->condition('created_by',$updateUserId,'=');
			 	$updateStmt->condition('id',$exmptedId, '=');
			 	$updateStmt->fields(array(
			 			'exempted_reason' => $reason,
			 			'updated_by' => $updateUserId,
			 			'updated_on' => now()
			 	));
			 	expDebug::dPrintDBAPI("AFsgtewtwetqe",$updateStmt);
			 	$updateStmt->execute();
			} 

		}
		
		syncSolrData('Enrollment');
	} catch (Exception $ex) {
		watchdog_exception('roster_enrollment_exempted_form_html_submit', $ex);
		expertusErrorThrow($ex);
	}
}

function enrollmentExemptedDetails($enrId,$enrType,$exmptedId=0,$start = 0,$limit = 0,$searchText='',$type = 'class'){
	try{
		expDebug::dPrint("Enrolment id".$enrId."enrollment type".$enrType."exempted id".$exmptedId,4);
		$field = array('id','exempted_status','exempted_reason','enroll_type','created_by','created_on',
				'updated_by','updated_on','is_mandatory','is_compliance');
		$qry = db_select('slt_enrollment_exempted','exmp');
		
		$qry->condition('exmp.enrollment_id',$enrId,'=');
		$qry->condition('exmp.enroll_type',$enrType,'=');
		//$qry->condition('exmp.in_progress','0','=');
		if($exmptedId > 0)
			$qry->condition('exmp.id',$exmptedId,'=');
		expDebug::dPrintDBAPI("Query for exempted enrollment values",$qry);

		$totalRecords = $qry->countQuery()->execute()->fetchField();
		if($type == 'COUNT'){
			$qry->addExpression('COUNT(id)','id');
			$exmDet = $qry->execute()->fetchAll();
		}else {
			$qry->fields('exmp',$field);
		}
		if(($start > 0) || ($limit > 0)) {
			$qry->range($start,$limit);
		}
		$exmDet = $qry->execute()->fetchAll();
		/* if($type != 'COUNT'){
			$exmDet['totalrow'] = $totalRecords;
		} */
		expDebug::dPrint("Values for exempted enrollments".print_r($exmDet,true),3);
		syncSolrData('Enrollment');
		return $exmDet;
	} catch (Exception $ex) {
		watchdog_exception('enrollmentExemptedDetails', $ex);
		expertusErrorThrow($ex);
	}
}

function enrollmentExemptedDetailsUpdate($enrollmentId,$entityType,$classId,$submitType,$courseId='',$fromApi = false, $apiReason= false,$userid=''){
	try{
		$user_id = getIdOfLoggedInUser();
		
		$complianceDetail = checkComplianceAndMandatory ($enrollmentId,$entityType);
		$multiregister = is_multiregistration_allowed($classId);	
		if($entityType == 'class' ) {
			if($multiregister == 1){
				$updateStmt = db_update('slt_enrollment');
				$updateStmt->condition('class_id',$classId,'=');
				$updateStmt->condition('user_id',$userid,'=');
				$updateStmt->condition('reg_status', array('lrn_crs_reg_can','lrn_crs_reg_rsc'),'NOT IN');
				$updateStmt->condition('comp_status', array('lrn_crs_cmp_cmp'),'NOT IN');
				// Set new values of fields
				$updateStmt->fields(array(
						'mandatory' 	=> NULL,
						'is_compliance' => NULL,
						'updated_on'    => now(),
						'updated_by'    => $user_id
				));
				expDebug::dPrintDBAPI(' slt enrollment $multiregister updateStmt = ', $updateStmt);
				$updateStmt->execute();
			}else{
			$updateStmt = db_update('slt_enrollment');
			// Set conditions
			$updateStmt->condition('id',$enrollmentId,'=');
			$updateStmt->condition('reg_status', array('lrn_crs_reg_can','lrn_crs_reg_rsc'),'NOT IN');
			$updateStmt->condition('comp_status', array('lrn_crs_cmp_cmp'),'NOT IN');
			// Set new values of fields
			$updateStmt->fields(array(
					'mandatory' 	=> NULL,
					'is_compliance' => NULL,
					'updated_on'    => now(),
					'updated_by'    => $user_id
			));
			expDebug::dPrintDBAPI(' slt enrollment updateStmt = ', $updateStmt);
			$updateStmt->execute();
			}
			
		}
		if($entityType == 'tp') {
			$updateStmt = db_update('slt_master_enrollment');
			// Set conditions
			$updateStmt->condition('id',$enrollmentId,'=');
			$updateStmt->condition('overall_status', array('lrn_tpm_ovr_cmp','lrn_tpm_ovr_cln','lrn_tpm_ovr_rsv'),'NOT IN');
			// Set new values of fields
			$updateStmt->fields(array(
					'mandatory' 	=> NULL,
					'updated_on'    => now(),
					'updated_by'    => $user_id
			));
			expDebug::dPrintDBAPI(' slt master enrollment updateStmt = ', $updateStmt);
			$updateStmt->execute();
			
		}
		if($submitType == 'fromApi') {
			$reasonVal = $apiReason;
			$updateUserId = $userid;
		} else {
			$reasonVal = ($submitType == 'waivedandsave') ? $_SESSION['exempted-'.$entityType.'-'.$classId][0] : $_SESSION['exempted-'.$entityType.'-'.$classId][$enrollmentId];
			$updateUserId = getIdOfLoggedInUser();
		}
		
		expDebug::dPrint("exempted enrollment submit".print_r($reasonVal,true),5);
			
		$compliance = !empty($complianceDetail[0]->is_compliance)? $complianceDetail[0]->is_compliance: '0' ;
		$madatory = !empty($complianceDetail[0]->mandatory)? $complianceDetail[0]->mandatory: '0' ;
		$_SESSION['waived_enroll_id_'.$enrollmentId] = $enrollmentId;
		expDebug::dPrint("exempted enrollment submitSSS---->".$entityType.'---'.$classId,5);
				if($entityType == 'class'){
				if(!empty($classId)){
					$insertStmt1 = db_insert('slt_enrollment_exempted');
					$insertStmt1->fields(array(
							'enrollment_id' => $enrollmentId,
							'course_id'  => $courseId,
							'class_id'   => $classId,
							'user_id'   => $userid,
							'enroll_type' => $entityType,
							'exempted_status' => 1,
							'exempted_reason' => $reasonVal,
							'is_mandatory' => $madatory,
							'is_compliance' => $compliance,
							'created_by' => $updateUserId,
							'created_on' => now(),
							'updated_by' => $updateUserId,
							'updated_on' => now()
					));
					expDebug::dPrintDBAPI('insert statement = ' , $insertStmt1);
					$retrunId = $insertStmt1->execute();
					expDebug::dPrint('insert retrunId = ' . $retrunId);
			}
		}else if($entityType == 'tp'){
			expDebug::dPrint("exempted enrollment submit 111".print_r($reasonVal,true),5);
			if(!empty($classId)){
				$insertStmt1 = db_insert('slt_enrollment_exempted');
				$insertStmt1->fields(array(
						'enrollment_id' => $enrollmentId,
						'course_id'  => $classId,
						'enroll_type' => $entityType,
						'exempted_status' => 1,
						'exempted_reason' => $reasonVal,
						'is_mandatory' => $madatory,
						'is_compliance' => $compliance,
						'created_by' => $updateUserId,
						'created_on' => now(),
						'updated_by' => $updateUserId,
						'updated_on' => now()
				));
				expDebug::dPrintDBAPI('insert statement = ' , $insertStmt1);
				$retrunId = $insertStmt1->execute();
			}
			
		}
	    
		$notifyUserId = getUserIdFromEnrollId($enrollmentId,$entityType);
		
		$insertaudit = array(
				'enrollment_id' => $enrollmentId,
				'enroll_type' => $entityType,
				'exempted_reason' => $reasonVal,
				'exempted_status' => 1,
		);
		
		make_audit_trail_entries($insertaudit,$notifyUserId , null ,$classId, 'Inserted','cre_sys_obt_cls','exp_sp_learning','waivedtoEnrolled',
				array('enrollment_id','enroll_type','exempted_status','exempted_reason'),$user_id);
		
		insertNotification('enrollment_waived', $enrollmentId, $entityType, $notifyUserId, $updateUserId);
		
		if($retrunId && $fromApi == true) {
			return $retrunId;
		}
		
		syncSolrData('Enrollment');
		syncSolrData('MasterEnrollment');

	} catch (Exception $ex) {
		watchdog_exception('enrollmentExemptedDetails', $ex);
		expertusErrorThrow($ex);
	}
}
	


function exemptedDetailListForm ($form, &$form_state) {
	try {
		global $theme_key;
		$enrollId = $form_state['enroll_id'];
		$enrollType = $form_state['enroll_type'];
		$classId = $form_state['class_id'];
		$rowNumber = $form_state['row_number'];
		$exmptedId = (isset($form_state['exempted_id'])) ? $form_state['exempted_id'] : '0';
		
		$uid = getIdOfLoggedInUser();
		
		
		$list_count = enrollmentExemptedDetails($enrollId,$enrollType,0,0,0,$searchText='','COUNT');
		
		expDebug::dPrint("Values for exempted enrollments list count".print_r($list_count,true),3);
		
		//pagination start here
			
		$pg_page     = 1;
		$start       = 0;
		$limit       = 3;
		$addListValuesCount = $list_count[0]->id;
		
		expDebug::dPrint("Values for exempted enrollments list count addListValuesCount".print_r($addListValuesCount,true),3);
			
		if( $addListValuesCount > 0 ){
			$total_pages = ceil($addListValuesCount/$limit);
		}else{
			$total_pages = 0;
		}
			
		if($pg_page > $total_pages) {
			$pg_page = $total_pages;
		}
		if(!empty($form_state['storage']['page'])) {
			$pg_pagenew  = $form_state['storage']['page'];
			if($pg_pagenew > $total_pages){
				$pg_page  = $form_state['storage']['page']- 1;
				$form_state['storage']['page'] = $pg_page;
				// $start    = ($limit * $pg_page) - $limit ;
			}else{
				$pg_page  = $form_state['storage']['page'];//$form_state['question_pgn_page'];
				//$start    = ($limit * $pg_page) - $limit ;
			}
		}
		// Added by Vincent on 02, Jan 2014 for #0029687: Refresh on Admin Enrollments page
		if($total_pages > 0){
			$start    = getOffsetOfFirstRecordInPage($pg_page, $total_pages, $limit, $addListValuesCount,false);
		}
		if($start < 0) {
			$start = 0;
		}
			
			
		if($addListValuesCount > 3 ){
			$displayPageStyle = "display:block;";
		}else{
			$displayPageStyle = "display:none;";
		}
		//$setupList = enrollmentExemptedDetails($listCode,'LIST', $start, $limit,$searchText);
		$details = enrollmentExemptedDetails($enrollId,$enrollType,$exmptedId,$start,$limit,$searchText='','LIST');
		expDebug::dPrint("Details from the exempted list".print_r($details,1));
		//api related work
		//53653: Empty Row found in Department/Employment type/User Type in People -> setting.
		/* unset($setupList['totalrow']); */
		$uniqueId = $enrollId.'-'.$exmptedId.'-'.$enrollType;
		$form = array();
		$formName = "enrollment-exempted-form-".$enrollId.'-'.$enrollType;
		$form['#prefix'] = '</form><div id="list-values-exempted-detail-wrapper-name">';
		$form['#suffix'] = '</div>';
		
		$wrapperId = 'list-values-exempted-detail-wrapper-name';

		$form_state['esign_from_plugin']= true;
		
		if($enrollType == 'class')
			$class = 'classenrollexempteddiv';
		else
			$class = 'tpenrollexempteddiv';


		$form[$formName]['addedit-enroll_exempt_start'] = array(

				'#type' => 'markup',
				'#markup' => '<div id="roster-enrollment-exempted-container" class="enrollment-exempted-addedit-form-container '.$class.'">',
		);
		
		$form[$formName]['exempted_reason-start-foreach'] = array(
				'#type' => 'item',
				'#prefix' =>  "<table width='100%'>",
				'#markup' => '',
		);
		$cls = 0;
		
		//$classId = getClassIdFromEnrollmentId($enrollId);

		foreach ($details as $key => $values) {
			expdebug::dPrint("key value--->>>".$key);
			$cls++;
				
			$attributesEdit = array();
			$attributesEdit['class'][] = 'addedit-form-expertusone-throbber';
			$attributesEdit['onclick'][] = "enrollmentExemptedEdit(".$qtipOptClassObj."); return false;";
			if($theme_key == 'expertusoneV2'){
				$attributesEdit['class'][] = 'enable-edit-icon';
			}else{
				$attributesEdit['class'][] = 'admin-action-button-edit';
			}
				
			$update_person_details = getPersonDetails($values->updated_by);
			$timezone_details = getPersonDetails($uid);
			expDebug::dPrint('odee even class update_person_details'.print_r($update_person_details, true), 5);
			$chngByLbl = ($values->exempted_status == 1) ? t("Changed to Waived by")." " : t("MSG749")." ";
			
			$fromTz = date_default_timezone(FALSE);

			$dt = new DateTime($values->updated_on, new DateTimeZone($fromTz));
			$dt->setTimezone(new DateTimeZone($timezone_details['attr2']));
 			$convert_datetime = $dt->format('M j, Y h:i A');  
			$user_details = $chngByLbl.$update_person_details['full_name']." On ". $convert_datetime;
			
			$classOddEven = ($key%2) ? 'exempted-even-list-class' : 'exempted-odd-list-class';
			
			$lastRowclass = ($total_pages > $pg_page) ? $limit : $addListValuesCount - (($pg_page-1)*$limit);
			$btmLineCls = ($cls == $lastRowclass) ? 'exempted-last-list' : 'exempted-bottom-line';
				
			$form[$formName]['exempted_reason-'.$key] = array(
					'#type' => 'item',
					'#prefix' =>  "<tr class ='".$classOddEven.' '.$btmLineCls."'><td class='exempted-reason-width'><span class='exempt-list-detail' >",
					'#markup' => "<span class='exempted_user_details'>".$user_details."</span><br /><span class='exempted_reason_details'>".descController('EXEMPTED_REASON', $values->exempted_reason,'cre_sys_obt_cls')."</span></td>",
			);
			if($uid == $values->created_by || $uid == $values->updated_by) {
				$qtipIdInit        	 = $values->id.'_cre_sys_obt_cls';
				if($rowNumber >5) {
					$callVisibeId = 'qtipAttachIdqtip_listvaledit_visible_disp_upper';
				$qtipOptClassObj     = "{'entityId':".$values->id.",'entityType':'cre_sys_obt_cls',
				'url':'administration/enrollment/exempted/single/".$classId."/".$enrollId."/".$enrollType."/".$values->id."',
				'popupDispId':'qtip_exempted_visible_disp_".$qtipIdInit."',
					'catalogVisibleId':'qtipAttachIdqtip_listvaledit_visible_disp',
					'wid':400,'heg':'150','postype':'middle','poslwid':'70','qdis':'ctool',
					'linkid':'edit-exempted-values-".$values->id."','dispDown':''}";
					
				}else {
					$callVisibeId = 'qtipAttachIdqtip_listvaledit_visible_disp_lower';
					$qtipOptClassObj     = "{'entityId':".$values->id.",'entityType':'cre_sys_obt_cls',
					'url':'administration/enrollment/exempted/single/".$classId."/".$enrollId."/".$enrollType."/".$values->id."',
					'popupDispId':'qtip_exempted_visible_disp_".$qtipIdInit."',
				'catalogVisibleId':'qtipAttachIdqtip_listvaledit_visible_disp_".$qtipIdInit."',
					'wid':400,'heg':'200','postype':'middle','poslwid':'70','qdis':'ctool',
				'linkid':'edit-exempted-values-".$values->id."','dispDown':'Y'}";
					
				}			
				$attributesEdit = array();
				$attributesEdit['class'][] = 'addedit-form-expertusone-throbber';
				$attributesEdit['onclick'][] = "exemptedEditVisibility(".$qtipOptClassObj."); return false;";
				$attributesEdit['class'][] = 'enable-edit-icon';
				$form[$formName]['edit-group-list-values'.$values->id] = array(
						'#type' => 'button',
						'#name' => 'edit-exempted-values-'.$values->id,
						'#id' => 'edit-exempted-values-'.$values->id,
						'#prefix' => "<td class ='exempted-eidt-icon-width'><div id='qtip_exempted_visible_disp_".$qtipIdInit."' class='attached-exempted-list'><div class='edit-exempted-list-values'>
						<span class='vtip ".$callVisibeId."'  title='".t('LBL063')."' >",
						'#suffix' => "</span><span id='visible-popup-".$values->id."' class='qtip-popup-exempted-edit' style='display:none; position:absolute; left:0px; top:0px;'></span></div>",
						'#value' => '', 
						'#name' => 'edit-exempted-list-values',
						'#attributes' => $attributesEdit,
						'#ajax' => array(
								'wrapper' => $wrapperId,
								'event'=>'click',
								'progress' => array(
										'type' => 'throbber',
										'message' => ''
								),
						)
				);
				
				
				$form[$formName]['exempted_reason-close'.$key] = array(
						'#type' => 'item',
						'#markup' => '</span></td>',
				);
			}
			
		}
		
		$form[$formName]['exempted_reason-end-foreach'] = array(
				'#type' => 'markup',
				'#markup' => '</tr></table>',
		);
		
		
		/* ------------------------------------Start Pagination----------------------------*/
		$attributesPrev = array();
		$attributesPrev['class'][] = 'addedit-form-expertusone-throbber';
		$attributesPrev['class'][] = 'exempted-list-first-btn';
		$attributesPrev['data-wrapperid'] = $wrapperId;
		 
		if($pg_page == 1) {
			$form[$formName]['exemptedlist-pagination-first'] = array(
					'#type' => 'markup',
					'#markup' => '<div class="exempted-list-pagination-container" style="'.$displayPageStyle.'"><span class="exempted-list-first-btn"></span>', //start
			);
		}else{
			$form[$formName]['exemptedlist-pagination-first'] = array(
					'#type' => 'button',
					'#id' => 'exempted_lst_pg_first',
					'#prefix' => '<div class="exempted-list-pagination-container" style="'.$displayPageStyle.'">',//(PR) Page :'.$pg_page." || start : ".$start
					'#suffix' => ' ',
					'#name' => 'exempted_list_pagination_first',
					'#attributes' => $attributesPrev,
					'#ajax' => array(
							'wrapper' => $wrapperId,
							'callback' => 'exempted_list_pagination_callback',
							'method' => 'replace',
							'effect' => 'none',
							'event'=>'click',
							'question_pgn_page' => 1,
							'keypress' => true,
							'progress' => array(
									'type' => 'throbber',
									'message' => ''
							),
					)
			);
		}
		
		$attributesPrev = array();
		$attributesPrev['class'][] = 'addedit-form-expertusone-throbber';
		$attributesPrev['class'][] = 'exempted-list-previous-btn';
		$attributesPrev['data-wrapperid'] = $wrapperId;
		 
		if($pg_page == 1) {
			$form[$formName]['exemptedlist-pagination-previous'] = array(
					'#type' => 'markup',
					'#markup' => '<span class="exempted-list-previous-btn"></span>', //start
			);
		}else{
			$form[$formName]['exemptedlist-pagination-previous'] = array(
					'#type' => 'button',
					'#id' => 'exempted_lst_pg_previous',
					'#prefix' => ' ',//(PR) Page :'.$pg_page." || start : ".$start
					'#suffix' => ' ',
					'#name' => 'exempted_list_pagination_previous',
					'#attributes' => $attributesPrev,
					'#ajax' => array(
							'wrapper' => $wrapperId,
							'callback' => 'exempted_list_pagination_callback',
							'method' => 'replace',
							'effect' => 'none',
							'event'=>'click',
							'question_pgn_page' => ($pg_page-1),
							'keypress' => true,
							'progress' => array(
									'type' => 'throbber',
									'message' => ''
							),
					)
			);
		}
		 
		
		$attributesCenter = array();
		$attributesCenter['class'][] = 'addedit-form-expertusone-throbber';
		$attributesCenter['class'][] = 'addedit-page-text';
		$attributesCenter['data-wrapperid'] = $wrapperId;
		$prefixPage = ($theme_key == 'expertusoneV2')? '<span style="margin-right: 3px;">'.t('LBL989').'</span>' : '';
		
		$form[$formName]['exemptedlist-pagination-total-txt-1'] = array(
				'#type' => 'textfield',
				'#id' => 'exempted_lst_pg_txtfld',
				'#name' => 'exempted_lst_pg_txtfld',
				'#prefix' => '<div class="tot-txt-cls">'.$prefixPage,
				'#suffix' =>  '<span class= "pagination-total-pages">'.t('LBL981').' '.$total_pages.'</span></div>',
				'#value' => $pg_page,
				'#attributes' => $attributesCenter,
				'#size' => 1,
				'#ajax' => array(
						'callback' => 'exempted_list_pagination_callback',
						'wrapper' => $wrapperId,
						'question_pgn_page' => $pg_page,
						'event' => 'change',
				),
		
		);
		 
		 
		$attributesNxt = array();
		$attributesNxt['class'][] = 'addedit-form-expertusone-throbber';
		$attributesNxt['class'][] = 'exempted-list-next-btn';
		$attributesNxt['data-wrapperid'] = $wrapperId;
		
		if($pg_page == $total_pages) {
			$form[$formName]['exemptedlist-pagination-next'] = array(
					'#type' => 'markup',
					'#markup' => '<span class="exempted-list-next-btn"></span>',
			);
		}else{
			$form[$formName]['exemptedlist-pagination-next'] = array(
					'#type' => 'button',
					'#id' => 'exempted_lst_pg_next',
					'#prefix' => ' ',//(NX) Page :'.$pg_page." || start : ".$start
					//'#suffix' => '</div>',
					'#suffix' => '',
					'#value' => ' ', //add_usr_pg_next
					'#name' => 'exempted_list_pagination_next',
					'#attributes' => $attributesNxt,
					'#ajax' => array(
							'wrapper' => $wrapperId,
							'callback' => 'exempted_list_pagination_callback',
							'method' => 'replace',
							'effect' => 'none',
							'event'=>'click',
							'question_pgn_page' => ($pg_page+1),
							'keypress' => true,
							'progress' => array(
									'type' => 'throbber',
									'message' => ''
							),
					)
			);
		}
		
		$attributesNxt = array();
		$attributesNxt['class'][] = 'addedit-form-expertusone-throbber';
		$attributesNxt['class'][] = 'exempted-list-last-btn';
		$attributesNxt['data-wrapperid'] = $wrapperId;
		
		if($pg_page == $total_pages) {
			$form[$formName]['exemptedlist-pagination-last'] = array(
					'#type' => 'markup',
					'#markup' => '<span class="exempted-list-last-btn"></span></div>',
			);
		}else{
			$form[$formName]['exemptedlist-pagination-last'] = array(
					'#type' => 'button',
					'#id' => 'exempted_lst_pg_last',
					'#prefix' => ' ',//(NX) Page :'.$pg_page." || start : ".$start
					'#suffix' => '</div>',
					'#value' => ' ', //add_usr_pg_next
					'#name' => 'exempted_list_pagination_last',
					'#attributes' => $attributesNxt,
					'#ajax' => array(
							'wrapper' => $wrapperId,
							'callback' => 'exempted_list_pagination_callback',
							'method' => 'replace',
							'effect' => 'none',
							'event'=>'click',
							'question_pgn_page' => $total_pages,
							'keypress' => true,
							'progress' => array(
									'type' => 'throbber',
									'message' => ''
							),
					)
			);
		} 
		/* ------------------------------------End Pagination----------------------------*/
			
		$form[$formName]['addedit-enroll_exempt_end'] = array(
				'#type' => 'markup',
				'#markup' => '</div>',
		);
			
		return $form;
		
}catch (Exception $ex) {
    watchdog_exception('addEditFormWithListValuesCancelAndSaveClose', $ex);
    expertusErrorThrow($ex);
  }
}

function exemptedDetailListForm_validate($form, &$form_state){
	try {
		if ($form_state['triggering_element']['#id'] == 'exempted_lst_pg_txtfld') {
			expDebug::dPrint("Validate herrrrr : 3");
			$form_state['display_class']   = TRUE;
			$form_state['rebuild']         = TRUE;
			$form_state['storage']['page'] = $form_state['input']['exempted_lst_pg_txtfld'];//$form_state['triggering_element']['#ajax']['question_pgn_page'];//
			return;
		}

		if ($form_state['triggering_element']['#id'] == 'exempted_lst_pg_next') {
			expDebug::dPrint("Validate herrrrr : 1");
			$form_state['display_class']   = TRUE;
			$form_state['input']           = array();
			$form_state['values']          = array();
			$form_state['rebuild']         = TRUE;
			$form_state['storage']['page'] = $form_state['triggering_element']['#ajax']['question_pgn_page'];
			return;
		}

		if ($form_state['triggering_element']['#id'] == 'exempted_lst_pg_previous') {
			expDebug::dPrint("Validate herrrrr : 2");
			$form_state['display_class']   = TRUE;
			$form_state['input']           = array();
			$form_state['values']          = array();
			$form_state['rebuild']         = TRUE;
			$form_state['storage']['page'] = $form_state['triggering_element']['#ajax']['question_pgn_page'];
			return;
		}
		if ($form_state['triggering_element']['#id'] == 'exempted_lst_pg_first') {
			expDebug::dPrint("Validate herrrrr : 1");
			$form_state['display_class']   = TRUE;
			$form_state['input']           = array();
			$form_state['values']          = array();
			$form_state['rebuild']         = TRUE;
			$form_state['storage']['page'] = $form_state['triggering_element']['#ajax']['question_pgn_page'];
			return;
		}
	
		if ($form_state['triggering_element']['#id'] == 'exempted_lst_pg_last') {
			expDebug::dPrint("Validate herrrrr : 2");
			$form_state['display_class']   = TRUE;
			$form_state['input']           = array();
			$form_state['values']          = array();
			$form_state['rebuild']         = TRUE;
			$form_state['storage']['page'] = $form_state['triggering_element']['#ajax']['question_pgn_page'];
			return;
		}
		} catch (Exception $ex) {
			watchdog_exception('exp_sp_administration_setup_addedit_form_validate', $ex);
			expertusErrorThrow($ex);
		}
}

/* function enrollmentexmpeteddetailslistview($form, &$form_state){
 expDebug::dPrint("enrollmentexmpeteddetailslistview",4);
$form = array();
$form['#prefix'] = '</form><div id="list-values-detail-wrapper-name">';
$form['#suffix'] = '</div>';

$doneBtnAttributes                      = array();
$doneBtnAttributes['class'][]           = 'addedit-form-expertusone-throbber';
$doneBtnAttributes['data-wrapperid']    = array('list-values-detail-wrapper-name');
$doneBtnAttributes['onclick'][]         = 'return false;';
$form["enrollment_exempted_btn_view"] = array(
		'#type'       => 'submit',
		'#id'         => 'enrollment_exempted_btn_view',
		'#prefix'     => '<div style="display:block;">',
		'#suffix'     => '</div>',
		'#value'      => 'uploaduser',
		'#name'       => 'enrollment_exempted_btn_view_name',
		'#attributes' => $doneBtnAttributes,
		'#submit' => array('exempted_list_pagination_callback'),
		'#ajax' => array(
				'wrapper'   => 'list-values-detail-wrapper-name',
				'callback'    => 'roster_enrollment_exempted_save',
				//'entity_id' => $enrollId,
				//'entity_type' => $enrollType,
				'method'    => 'replace',
				'effect'    => 'none', // 'fade',
				'event'     =>'click',
				'keypress'  => true,
				'progress' => array(
						'type'    => 'throbber',
						'message' => ''
				),
		),
);

return $form;
} */


function exempted_list_pagination_callback($form, &$form_state){
	try {
		expDebug::dPrint('page call exempted_list_pagination_callback'.print_r($form_state['triggering_element']['#ajax'],true));
		$question_pgn_page = $form_state['triggering_element']['#ajax']['question_pgn_page'];
		$enroll_id = $form_state['enroll_id'];
		$enroll_type = $form_state['enroll_type'];

		$form_state = array(
				'no_redirect' => TRUE,
				'enroll_id' => $enroll_id,
				'enroll_type' => $enroll_type,
				'class_id'  	 => $form_state['class_id'],
				'question_pgn_page' => $question_pgn_page,
				'build_info' => array('args' => array('ajax')),
		);

		$formId = 'exemptedDetailListForm';
		$builtForm = drupal_build_form($formId, $form_state);
		$commands = array();
		$commands[] = ajax_command_replace(NULL, drupal_render($builtForm));
		return array(
				'#type' => 'ajax',
				'#commands' => $commands
		);


	} catch (Exception $ex) {
		watchdog_exception('exemptedDetailListForm', $ex);
		expertusErrorThrow($ex);
	}

}

 function checkWaivedReasonEmpty($enrId) {
	$user_id = getIdOfLoggedInUser();
	expDebug::dPrint("Enrolment id".$enrId."enrollment type".$enrType."exempted id".$enrId,4);
	$field = array('exempted_reason');
	$qry = db_select('slt_enrollment_exempted','exmp');
	$qry->condition('exmp.enrollment_id',$enrId,'=');
	//$qry->condition('exmp.created_by',$user_id,'=');
	$qry->fields('exmp',$field);
	$qry->range(0,1);
	$qry->orderBy('id','desc');
	expDebug::dPrintDBAPI("Query for exempted enrollment values",$qry);
	$reason = $qry->execute()->fetchField();
	expDebug::dPrint("checkWaivedReasonEmpty reason".print_r($reason,1),5);
	if(empty($reason))
		return false;
	else
		return true;

} 

function revertExemptedStatus($previous_exempted_status, $enrollId, $type,$classId,$submitType,$courseId='',$fromApi = false,$apiReason = false,$userid='') {

	$enrId = $enrollId;
	$multiregister = is_multiregistration_allowed($classId);
	if($type == 'class') {
		/* enrollment table update */
		if($multiregister == 1){
			$updateStmt = db_update('slt_enrollment');
			$updateStmt->condition('class_id',$classId,'=');
			$updateStmt->condition('user_id',$userid,'=');
			$updateStmt->condition('reg_status', array('lrn_crs_reg_can','lrn_crs_reg_rsc'),'NOT IN');
			$updateStmt->condition('comp_status', array('lrn_crs_cmp_cmp'),'NOT IN');
			$updateStmt->fields(array(
					'mandatory' 	=> ($previous_exempted_status[0]->is_mandatory) ? $previous_exempted_status[0]->is_mandatory  : NULL,
					'is_compliance' => ($previous_exempted_status[0]->is_compliance) ? $previous_exempted_status[0]->is_compliance : NULL ,
					'updated_on' => now()
			));
			expDebug::dPrintDBAPI(' slt exempted table multieregsiter updateStmt = ', $updateStmt);
			$result = $updateStmt->execute();
		}else{
		$updateStmt = db_update('slt_enrollment');
		$updateStmt->condition('id',$enrId,'=');
		$updateStmt->condition('reg_status', array('lrn_crs_reg_can','lrn_crs_reg_rsc'),'NOT IN');
		$updateStmt->condition('comp_status', array('lrn_crs_cmp_cmp'),'NOT IN');
		$updateStmt->fields(array(
				'mandatory' 	=> ($previous_exempted_status[0]->is_mandatory) ? $previous_exempted_status[0]->is_mandatory  : NULL,
				'is_compliance' => ($previous_exempted_status[0]->is_compliance) ? $previous_exempted_status[0]->is_compliance : NULL ,
				'updated_on' => now()
		));
		expDebug::dPrintDBAPI(' slt exempted table updateStmt = ', $updateStmt);
		$result = $updateStmt->execute();
		}
	} else if($type = 'tp') {
		/* enrollment table update */
		$updateStmt = db_update('slt_master_enrollment');
		$updateStmt->condition('id',$enrId,'=');
		$updateStmt->condition('overall_status', array('lrn_tpm_ovr_cmp','lrn_tpm_ovr_cln','lrn_tpm_ovr_rsv'),'NOT IN');
		$updateStmt->fields(array(
				'mandatory' 	=> ($previous_exempted_status[0]->is_mandatory) ? $previous_exempted_status[0]->is_mandatory : NULL,
				'updated_on' => now()
		));
		expDebug::dPrintDBAPI(' slt exempted table updateStmt = ', $updateStmt);
		$result = $updateStmt->execute();

	}
	$user_id = getIdOfLoggedInUser();
    if($fromApi == true) {
    	$reasonVal = $apiReason;
		$user_id = $userid;
    } else {
    	$reasonVal = ($submitType == 'waivedandsave') ? $_SESSION['exempted-'.$type.'-'.$classId][0] : $_SESSION['exempted-'.$type.'-'.$classId][$enrId];
    }
	$_SESSION['waived_enroll_id_'.$enrId] = $enrId;
	
	if($type == 'class'){
				if(!empty($classId) && !empty($result) && is_numeric($result)){
					$insertStmt1 = db_insert('slt_enrollment_exempted');
					$insertStmt1->fields(array(
							'enrollment_id' => $enrId,
							'course_id'  => $courseId,
							'class_id'   => $classId,
							'user_id'   => $userid,
							'enroll_type' => $type,
							'exempted_status' => 0,
							'exempted_reason' => $reasonVal,
							'is_mandatory' => $previous_exempted_status[0]->is_mandatory,
							'is_compliance' => $previous_exempted_status[0]->is_compliance,
							'created_by' => $user_id,
							'created_on' => now(),
							'updated_by' => $user_id,
							'updated_on' => now()
					));
					expDebug::dPrintDBAPI('insert statement = ' , $insertStmt1);
					$retrunId = $insertStmt1->execute();
			}
		}else if($type == 'tp'){
			expDebug::dPrint("exempted enrollment submit 111".print_r($reasonVal,true),5);
			if(!empty($classId)&& !empty($result) && is_numeric($result)){
				$insertStmt1 = db_insert('slt_enrollment_exempted');
				$insertStmt1->fields(array(
						'enrollment_id' => $enrId,
						'course_id'  => $classId,
						'enroll_type' => $type,
						'exempted_status' => 0,
						'exempted_reason' => $reasonVal,
						'is_mandatory' => $previous_exempted_status[0]->is_mandatory,
						'is_compliance' => $previous_exempted_status[0]->is_compliance,
						'created_by' => $user_id,
						'created_on' => now(),
						'updated_by' => $user_id,
						'updated_on' => now()
				));
				expDebug::dPrintDBAPI('insert statement = ' , $insertStmt1);
				$retrunId = $insertStmt1->execute();
			}
			
		}
	$notifyUserId = getUserIdFromEnrollId($enrId,$type);
	
	$insertaudit = array(
			'enrollment_id' => $enrId,
			'enroll_type' => $type,
			'exempted_reason' => $reasonVal,
			'exempted_status' => 0,
	);
	
	make_audit_trail_entries($insertaudit,$notifyUserId , null ,$classId, 'Inserted','cre_sys_obt_cls','exp_sp_learning','waivedtoEnrolled',
			array('enrollment_id','enroll_type','exempted_status','exempted_reason'),$user_id);
	
	insertNotification('reverted_waived', $enrId, $type, $notifyUserId, $user_id);
	
	if($fromApi == true) {
		return $retrunId;
	}
	syncSolrData('Enrollment');
}

function getUserIdFromEnrollId($enrId,$type){
	try{
		expDebug::dPrint("type value".$type);
		$tblNme = ($type == 'class') ? 'slt_enrollment' : 'slt_master_enrollment';
		$selQry = db_select($tblNme,'enr');
		$selQry->addField('enr','user_id');
		$selQry->condition('enr.id',$enrId,'=');
		expDebug::dPrintDBAPI("Qurey for getUserIdFromEnrollId",$selQry);
		$res = $selQry->execute()->fetchField();
		return $res;
	}catch(Exception $e){
		watchdog_exception('getUserIdFromEnrollId', $e);
		expertusErrorThrow($e);
	}
}

function UpdateRegistrationStatusToWaivedByAPI(&$form_state, $createuserid="") {
	try{
	$error = new stdClass();
	$errobj = new stdClass();
	include_once(drupal_get_path('module', 'exp_sp_learning') .'/exp_sp_learning.inc');
	expDebug::dPrint('insert statement = '.print_r($form_state['values'], true),1);
	
	$form=array();
	$error_messages = array();
	$enrollId 	= trim($form_state['values']['enrollId']);
	$reason 	= trim($form_state['values']['reason']);
	$type 	    = trim(stripslashes($form_state['values']['type']));
	$status 	= trim(stripslashes($form_state['values']['status']));
	$userid		= trim(stripslashes($form_state['values']['user_id']));
	expDebug::dPrint('api user id value from rest api= '.print_r($userid, true),1);
	if (empty($userid)) {
			$error -> isValidateError = true;
			$errobj->errorfield = "enrollId";
			$error -> errormsg ='User id should not be empty';
			return $error;
			$enrollmentCheckFlag = false;
			break;
	}
	
	
	$enrollIdArray = explode(',',$enrollId);
	
	expDebug::dPrint('enrollIdArray = '.print_r($enrollIdArray, true),1);
	
	$enrollmentCheckFlag = true;
	foreach($enrollIdArray as $enrollIdValue ) {
		if(!empty($enrollIdValue ) && !getUserIdFromEnrollId($enrollIdValue,$type)) {
			$error -> isValidateError = true;
			$errobj->errorfield = "enrollId";
			$error -> errormsg =$enrollIdValue.' - Enrollment ID is not available';
			return $error;
			$enrollmentCheckFlag = false;
			break;
		}
	}
	
	if(strlen($reason) > 150 && !empty($reason))
	{
		$error -> isValidateError = true;
		$errobj->errorfield = "reason";
		$error -> errormsg ='Reason cannot be empty and not more than 150 characters .';
		return $error;
	}
	$return_arry  = array();
	if($enrollmentCheckFlag) {
		if($type == 'class' && $status == 'yes') {
			foreach($enrollIdArray as $enrollIdValue ) {
				$courseId = getEnrollmentDetails($enrollIdValue, array('course_id','class_id'));
					$entity_typeNew = 'Class';
					$entity_id = $courseId->class_id;
					$entity_type_parameter = $entity_typeNew.'$$$'.$entity_id.'$~$User'.'$$$'.$userid; // Dynamic enity type param based on enrollID Course-Id/user-Id
					$entity_type_with_id = array_filter(explode('$~$',$entity_type_parameter));
					// Verify api access with Class and user level.
					foreach ($entity_type_with_id as $key => $value ) {
					$value_array = array_filter(explode('$$$', $value));
					$entity_type = getEntityTypeByItName(trim($value_array[0]));
					$entity_id   = array($value_array[1]);
					$check = objectLevelAccessCheckInGroupMap($entity_type,$entity_id);
					if(!empty($check)) {
						$privResult = apiUseraccesscheck($createuserid,$entity_type,$value_array[1]);
						if(empty($privResult)) {
							$error -> isValidateError = true;
							$errobj->errorfield = "enrollId";
							$error -> errormsg ='user dont have this privilege';
							return $error;
							$enrollmentCheckFlag = false;
							break;
						}
					}
				}
				$previous_exempted_status = getComplianceMandatoryStatusFromExempted($enrollIdValue,$type,$courseId->class_id);
				if($previous_exempted_status[0]->exempted_status == '1') {
					$error -> isValidateError = true;
					$errobj->errorfield = "enrollId";
					$error -> errormsg =$enrollIdValue .' - Given enrollment id already in Waived status.';
					return $error;
					$enrollmentCheckFlag = false;
					break;
					
				}
				$complianceDetail = checkComplianceAndMandatory ($enrollIdValue,$type);
				if($complianceDetail[0]->mandatory != 'Y' && $complianceDetail[0]->is_compliance != '1') {
					$error -> isValidateError = true;
					$errobj->errorfield = "enrollId";
					$error -> errormsg ='Enrollment id must have compliance or mandatory';
					return $error;
					break;
					
				}
				$status = enrollmentExemptedDetailsUpdate($enrollIdValue,$type,$courseId->class_id,'fromApi',$courseId->course_id,true, $reason,$userid);
				
				expDebug::dPrint('getEnrollmentDetails return = '.print_r($status, true),1);
				if($status) {
					$return_arry[] = $enrollIdValue;
				}
			}
		}
		if($type == 'tp' && $status == 'yes') {
			foreach($enrollIdArray as $enrollIdValue ) {
				$prgId = getMasterEnrollmentDetails($enrollIdValue, array('program_id'));
				$entity_typeNew = 'Training Plan';
				$entity_id = $prgId->program_id;
				$entity_type_parameter = $entity_typeNew.'$$$'.$entity_id.'$~$User'.'$$$'.$userid;// Dynamic enity type param based on enrollID Tp-Id/user-Id
				$entity_type_with_id = array_filter(explode('$~$',$entity_type_parameter));
				// Verify api access with tp and user level.
				foreach ($entity_type_with_id as $key => $value ) {
					$value_array = array_filter(explode('$$$', $value));
					$entity_type = getEntityTypeByItName(trim($value_array[0]));
					$entity_id   = array($value_array[1]);
					$check = objectLevelAccessCheckInGroupMap($entity_type,$entity_id);
					if(!empty($check)){
						$privResult = apiUseraccesscheck($createuserid,$entity_type,$value_array[1],'tp');
						if(empty($privResult)) {
							$error -> isValidateError = true;
							$errobj->errorfield = "enrollId";
							$error -> errormsg ='user dont have this privilege';
							return $error;
							$enrollmentCheckFlag = false;
							break;
						}
					}
				}
				$previous_exempted_status = getComplianceMandatoryStatusFromExempted($enrollIdValue,$type,$prgId->program_id);
				if($previous_exempted_status[0]->exempted_status == '1') {
					$error -> isValidateError = true;
					$errobj->errorfield = "enrollId";
					$error -> errormsg =$enrollIdValue .' - Given enrollment already in Waived status.';
					return $error;
					$enrollmentCheckFlag = false;
					break;
						
				}
				expDebug::dPrint('get previous status from tp  = '.print_r($previous_exempted_status, true),1);
				
				$complianceDetail = checkComplianceAndMandatory ($enrollIdValue,$type);	
				if($complianceDetail[0]->mandatory != '1' ) {
					$error -> isValidateError = true;
					$errobj->errorfield = "enrollId";
					$error -> errormsg ='Enrollment Id must have compliance or mandatory';
					return $error;
					break;
						
				}
				
				$status = enrollmentExemptedDetailsUpdate($enrollIdValue,$type,$prgId->program_id,'fromApi','',true, $reason,$userid);
				if($status) {
					$return_arry[] = $enrollIdValue;
				}

			}
		}
		if($type == 'class' && $status == 'revert') {
			foreach($enrollIdArray as $enrollIdValue ) {
				$courseId = getEnrollmentDetails($enrollIdValue, array('course_id','class_id'));
				$entity_typeNew = 'Class';
				$entity_id = $courseId->class_id;
				$apiPriv = 'Course/Class';
				$entity_type_parameter = $entity_typeNew.'$$$'.$entity_id.'$~$User'.'$$$'.$userid;
				$entity_type_with_id = array_filter(explode('$~$',$entity_type_parameter));
				foreach ($entity_type_with_id as $key => $value ) {
					$value_array = array_filter(explode('$$$', $value));
					$entity_type = getEntityTypeByItName(trim($value_array[0]));
					$entity_id   = array($value_array[1]);
					$check = objectLevelAccessCheckInGroupMap($entity_type,$entity_id);
					if(!empty($check)){
						$privResult = apiUseraccesscheck($createuserid,$entity_type,$value_array[1]);
						if(empty($privResult)) {
							$error -> isValidateError = true;
							$errobj->errorfield = "enrollId";
							$error -> errormsg ='user dont have this privilege';
							return $error;
							$enrollmentCheckFlag = false;
							break;
						}
					}
				}
				$previous_exempted_status = getComplianceMandatoryStatusFromExempted($enrollIdValue,$type,$courseId->class_id);
				if($previous_exempted_status[0]->exempted_status == '0') {
					$error -> isValidateError = true;
					$errobj->errorfield = "enrollId";
					$error -> errormsg =$enrollIdValue .' - Given enrollment id already in reverted status.';
					return $error;
					$enrollmentCheckFlag = false;
					break;
						
				}else if(empty($previous_exempted_status[0]->exempted_status)){
					$error -> isValidateError = true;
					$errobj->errorfield = "enrollId";
					$error -> errormsg =$enrollIdValue .' - Given enrollment is not in waived status';
					return $error;
					$enrollmentCheckFlag = false;
					break;
				}
				
					$status = revertExemptedStatus($previous_exempted_status,$enrollIdValue,'class',$courseId->class_id,'fromApi',$courseId->course_id,true,$reason,$userid);
					if($status) {
						$return_arry[] = $enrollIdValue;
					}
			}
		}
		
		if($type == 'tp' && $status == 'revert') {
			foreach($enrollIdArray as $enrollIdValue ) {
				$courseId = getMasterEnrollmentDetails($enrollIdValue, array('program_id'));
				$entity_typeNew = 'Training Plan';
				$entity_id = $courseId->program_id;
				$entity_type_parameter = $entity_typeNew.'$$$'.$entity_id.'$~$User'.'$$$'.$userid;
				$entity_type_with_id = array_filter(explode('$~$',$entity_type_parameter));
				foreach ($entity_type_with_id as $key => $value ) {
					$value_array = array_filter(explode('$$$', $value));
					$entity_type = getEntityTypeByItName(trim($value_array[0]));
					$entity_id   = array($value_array[1]);
					$check = objectLevelAccessCheckInGroupMap($entity_type,$entity_id);
					if(!empty($check)){
						$privResult = apiUseraccesscheck($createuserid,$entity_type,$value_array[1],"tp");
						if(empty($privResult)) {
							$error -> isValidateError = true;
							$errobj->errorfield = "enrollId";
							$error -> errormsg ='user dont have this privilege';
							return $error;
							$enrollmentCheckFlag = false;
							break;
						}
					}
				}
				$previous_exempted_status = getComplianceMandatoryStatusFromExempted($enrollIdValue,$type,$courseId->program_id);
				if($previous_exempted_status[0]->exempted_status == '0') {
					$error -> isValidateError = true;
					$errobj->errorfield = "enrollId";
					$error -> errormsg =$enrollIdValue .' - Given enrollment id already in reverted status.';
					return $error;
					$enrollmentCheckFlag = false;
					break;
				}else if(empty($previous_exempted_status[0]->exempted_status)){
					$error -> isValidateError = true;
					$errobj->errorfield = "enrollId";
					$error -> errormsg =$enrollIdValue .' - Given enrollment is not in waived status';
					return $error;
					$enrollmentCheckFlag = false;
					break;
				}
				
				$status = revertExemptedStatus($previous_exempted_status,$enrollIdValue,'tp',$courseId->program_id,'fromApi','',true,$reason,$userid);
				if($status) {
					$return_arry[] = $enrollIdValue;
				}
			}
		}
		
		
		
	}
	
	// To sync solr data
	syncSolrData('MasterEnrollment');
	
	$return_status = implode ($return_arry,',') . ' - updated Successfully ';
	return array((object)array('Id'=>$return_status));

	} catch(Exception $ex) {
		watchdog_exception('updateCustomFieldByRestAPI', $ex);
		expertusErrorThrow($ex);
	}	
	
}
?>