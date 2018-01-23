<?php
function displayAttachCourseDetails(&$form,&$form_state,$entityId, $entityType, $mode=""){
	try {
  // $form = array();
   $formName = 'attach-course';
   $uniqueId = 'TPAttachCourse-'.$entityId.'-'.$entityType;
   $wrapperId = 'tp-grid-wrapper-'.$uniqueId;
   $customHiddenButtonId = 'equv-save-esign';
   $modId = $form_state['module_id'];
   $crsform_build_id = $form_state['crsfrom_build_id'];
   expDebug::dPrint("crsfrom_build_id ".$crsform_build_id,4);
   expDebug::dPrint("crsfrom_build_id ".print_r($form_state,true),4);
   $setminHight = ($entityType=='cre_sys_obt_trp') ? 'admin-attach-course-tab-datagrid-wrapper' : '';

   expDebug::dPrint('displayAttachCourseDetails Form = ' . print_r($form, true) , 4);
    if(empty($form)){
	    $form = array(
	      '#type' => 'markup',
	   		// '#prefix' => (( stripos($_SERVER['HTTP_USER_AGENT'], 'Chrome')  || stripos($_SERVER['HTTP_USER_AGENT'], 'Firefox'))? '</form>' : '' ),
	      '#prefix' =>  '</form>',
	    );
   	}
  $form[$formName] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="admin-add-course-training-plan"><div id="'.$wrapperId.'" class="admin-datagrid-pagination '.$setminHight.'">',
    '#suffix' => '</div></div></div>',
  );

     $form[$formName][] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_search_container">',
      );

    $form[$formName]['hidden_idlist'] = array(
      '#type' => 'hidden',
      '#name' => 'hidden_idlist_'.$uniqueId,
      '#id' => 'datagrid-idlist-'.$uniqueId,
    );
   //  $excludedCourseId = '';
   $getTpArray = getTrainingProgramDetails($entityId,array('object_type'));
   $getObjectType = $getTpArray['object_type'];
   //if($getObjectType != "cre_sys_obt_crt") {
    $excludedCourseIdArray = array();
    $tpCoursesList =  getAttachedTpCourses($entityId,'','','','',$modId);
    expDebug::dPrint("current available crs". print_r($tpCoursesList,true),5);
    $excludedCourseId =0;
     if(count($tpCoursesList) > 0) {
        for($i=0;$i<count($tpCoursesList);$i++) {
        	if(!empty($tpCoursesList[$i]->course_id))
        $excludedCourseIdArray[]  =    $tpCoursesList[$i]->course_id;
        }
        //expDebug::dPrint("current available excluded ". p,5);
        $excludedCourseId = (count($excludedCourseIdArray) > 0 ) ? implode(',',$excludedCourseIdArray) : 0;
     }
    
     expDebug::dPrint("current available excluded ". $excludedCourseId,5);
   /*}
     else {
    	$excludedCourseId =0;
   }*/
   // $excludedCourseId =0;
      $searchAttributes['onClick'][] = '$("body").data("mulitselectdatagrid").searchDataGrid("edit", "TPAttachCourse", $(\'#tpattachcourse-autocomplete\').val(), "'.$entityId.'-'.$modId.'", "'.$entityType.'", "'.$excludedCourseId.'"); return false;';

      $acMenuPath = 'administration/learning/courses/allcourses-autocomplete';

      $form[$formName]['add_attachcourse_search'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_auto_search">',
      );
      $default_text_title = t('LBL088').' '.t('LBL083').' '.t('LBL644').' '.t('LBL096');
      $autoCompleteAttributes['class'][] = 'admin_ac_input_mainform';
      $autoCompleteAttributes['onkeydown'][]  = '$("body").data("mulitselectdatagrid").multiSelectSearchBoxKeyDown(event)';
      addACFieldForMultiSelect($form[$formName], 'tpattachcourse-autocomplete', $acMenuPath, '', '', '', array(), array('excluded_course_id' => "'".$excludedCourseId."'", 'id_required' => 0), false, false, $default_text_title,$autoCompleteAttributes, 100);

      $form[$formName]['add_attachcourse_search1'] = array(
      '#type' => 'markup',
      '#markup' => '</div>',
      );

      $searchAttributes['Class'][] = 'admin-pagination-search-go';
       $form[$formName]['search_attachcourse'] = array(
        '#type' => 'submit',
        '#title' => t('LBL304'), //Search
        '#name' => 'Search attachcourse',
        '#submit' => array('search_attachcourse'), // No such function search_attachcourse(), apparently property is ignored.
        '#attributes' => $searchAttributes,
      );
      $form[$formName][] = array(
      '#type' => 'markup',
      '#markup' => '<div class="filter-search-start-date-right-bg"></div><div class="clearBoth"></div></div>',
      );

    $form[$formName]['attachcourse_view_grid_markup'] = array(
      '#type' => 'markup',
      '#markup' => '<div id="datagrid-div-'.$uniqueId.'"><table id="datagrid-container-'.$uniqueId.'" class="datagrid-container-common"></table><div id="pager-datagrid-'.$uniqueId.'" class="pager-datagrid-common"></div></div>',
    );
    //$excludedCourseId =0;
    $form[$formName]['attachcourse_view_grid_load'] = array(
      '#type' => 'markup',
      '#prefix' => '<div style="display:none"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-'.$uniqueId.'\'); $(\'body\').data(\'mulitselectdatagrid\').loadDataGrid(\'edit\', \'TPAttachCourse\', \'\', \''.$entityId.'\', \''.$entityType.'\', \''.$excludedCourseId.'\');" width="100" height="132" /></div>',
    );

        $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-action-button-middle-bg white-btn-bg-middle';
        $cancelAttributes['onclick'][]         = "closeQtip('','$cancelButtonId');  return false;";
        $cancelAttributes['data-wrapperid'] = array($wrapperId);

        $form[$formName]['cancel_attachcourse_edit_mode'] = array(
          '#type' => 'submit',
          '#value' => t('LBL123'),
          '#title' => t('LBL123'),
          '#prefix' => '<div class="admin-datagrid-save-cancel-button-align-bottom"><div class="addedit-form-cancel-container-actions admin-save-button-container resize-save_btn"><div class="white-btn-bg-left"></div>',
          '#suffix' => '<div class="white-btn-bg-right"></div>',
          '#name' => 'Cancel attachcourse',
          '#attributes' => $cancelAttributes,
        );

        $listquery = loadAttachCourseForTP('', '', $excludedCourseId);

        if(count($listquery) != 0){

            $setAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg attach-list-view-attach-btn';
            $setAttributes['data-wrapperid'] = 'attach_course_dt';
            $form[$formName]['set_attachcourse_edit_mode'] = array(
              '#type' => 'submit',
              '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
    	      '#suffix' => '<div class="admin-save-button-right-bg"></div></div></div>',
              '#title' => t('LBL700'), //Attach Course
              '#value' => t('LBL701'), //Attach
              '#name' => $buttonName,
              '#submit' => array('attachCourseDetailsList'),
              '#attributes' => $setAttributes,
              '#ajax' => array(
        		    'wrapper' => 'attach_course_dt',
        		    'callback' => 'attachCourseCallBack',
                    'program_id' =>$entityId,
            		'module_id' => $modId,
                    'entity_type' =>$entityType,
            				'crsform_build_id' => $crsform_build_id,
              	      'method' => 'replace',
            	      'effect' => 'none', // 'fade',
            	      'event'=>'click',
            	      'keypress' => true,
            	      'progress' => array(
            	        'type' => 'throbber',
            	        'message' => ''
            	      ),
               ),
            );
        }

      //$ren = drupal_render($form);
  return $form;
  } catch (Exception $ex) {
  	watchdog_exception('displayAttachCourseDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function attachCourseCallBack($form, &$form_state){
 try {
  $programId = $form_state['triggering_element']['#ajax']['program_id'];
  $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
  $moduleId = $form_state['triggering_element']['#ajax']['module_id'];
  $crsform_build_id = $form_state['triggering_element']['#ajax']['crsform_build_id'];
  expDebug::dPrint("attach course call back>> ".$moduleId,4 );
  $min_form_state = array(
   'ajax' => TRUE,
   're_render' => TRUE,
   'no_redirect' => TRUE,
   'program_id' => $programId,
   //'build_info' => array('args' => array('ajax'))
   'call_type' => 'render_course_list',
  );
	$min_form_state['values']['mapped_module_id'] = $moduleId;
	$min_form_state['build_info'] = $form_state['build_info'];
	//$form['#form_id'] = 'exp_sp_administration_program_attaching_courses_form';
	
	 $statusMsgHTML = theme('status_messages');
		if(!empty($statusMsgHTML)) {
			$commands[] = ajax_command_html('#bubble-face-table #show_expertus_message', $statusMsgHTML);
		}else{
			$min_form_state['triggering_element'] = $form_state['triggering_element'];
			$min_form_state['triggering_element']['#name'] = "attach_course";
			$form_state = array();
			$_POST['form_build_id'] = $crsform_build_id;
			list($form, $form_state) = ajax_get_form();
			//expDebug::dPrint("Pagination builtForm form - ".print_r($form,true),4);
		  $form_state = array_merge($form_state,$min_form_state);
			//$form['#build_id'] = $crsform_build_id;
			$form_state['input']['form_build_id'] = $crsform_build_id;
		 	$form_state['rebuild_info']['copy']['#build_id'] = TRUE;
	 		$form_state['rebuild_info']['copy']['#action'] = TRUE;
	 		$form_state['rebuild'] = true;
	     
	     //$form_state['build_info']['args'] = array('ajax');
	     //$form_state['build_info']['question_pgn_page'] = $question_pgn_page;
			 //expDebug::dPrint("form prg 22 ".print_r($form_state,true),3);
	
	    $form_state['storage']['page'] = '1';
	
	
	
	     $formId = 'exp_sp_administration_program_attaching_courses_form';
	      //$formId = 'exp_sp_administration_program_rendercourse_html';
	     $builtForm = drupal_rebuild_form($formId, $form_state, $form);
				expDebug::dPrint("Pagination builtForm result - ".print_r($builtForm,true),4);
				//expDebug::dPrint("Pagination form state result - ".print_r($form_state,true),4);
				$form = array_merge($form,$builtForm);
				//$builtForm1 = form_builder($formId, $form, $form_state);
				form_set_cache($form['#build_id'], $form, $form_state);
				
	      $commands = array();
	      $commands[] = ajax_command_replace(NULL, drupal_render($builtForm));
	      expDebug::dPrint("Pagination builtForm commands - ".print_r($commands,true),4);
	      //$commands[] = ajax_command_replace('#module-program-'.$moduleId, drupal_render($builtForm));
	      //expDebug::dPrint('module test ccccccccc '. $moduleId);
	      $settings = get_drupal_ajax_settings($builtForm1);
			  //expDebug::dPrint('module test ccccccccc '. $moduleId);
			  $dom = new DOMDocument();
				$dom->loadHTML('<?xml encoding="utf-8" ?>' . $commands[0]['data']);
				$node = $dom->getElementById('module-program-'.$moduleId);
				//$node = $dom->getElementById('exp-sp-administration-program-rendercourse-html');
				$tabContant ='';
				if($node)
					$tabContant = getInnerHtml($node);
				$commands = array();
				$data = '<div id = "module-program-'.$moduleId.'">' . $tabContant . '</div>' ;
				expDebug::dPrint("Pagination final data - ".print_r($data,true),4);
				$commands[] = ajax_command_replace('#module-program-'.$moduleId, mb_convert_encoding($data, "HTML-ENTITIES", "UTF-8"));
			  $commands[] = ajax_command_invoke(NULL, "postPagination", array($settings,'show_more'));
			  $commands[] = ajax_command_invoke(NULL, "attachClose", array($programId.'-'.$moduleId, $entityType,'pagination'));
				expDebug::dPrint("Pagination final call result - ".print_r($commands,true),4);
				//'dragndrop-'.$moduleId
		}
			expDebug::dPrint("Pagination final call result - ".print_r($commands,true),4);
      return array(
            '#type' => 'ajax',
            '#commands' => $commands
      );
	
      
  } catch (Exception $ex) {
  	watchdog_exception('attachCourseCallBack', $ex);
  	expertusErrorThrow($ex);
  }
}

function loadAttachCourseForTP($searchKeyword, $queryRequired = '',$excludedCourseId = '', $moduleId = '',$entityId =''){
	try {
	  $select = db_select('slt_course_template', 'course');
	  $select->distinct();
	  $select->addField('course', 'id', 'id');
	  $select->addField('course', 'title', 'title');
	  $select->addField('course', 'code', 'code');
	  $default_text_title = t('LBL088').' '.t('LBL083').' '.t('LBL644').' '.t('LBL096');
	  if(!empty($searchKeyword) && $searchKeyword!='' && $searchKeyword!=$default_text_title){
	    //$select->condition('course.title', '%' . db_like($searchKeyword) . '%', 'LIKE');
	    $or_query1 = db_or();
		$or_query1->condition('course.title', '%' . db_like($searchKeyword) . '%', 'LIKE');
		$or_query1->condition('course.code', '%' . db_like($searchKeyword) . '%', 'LIKE');
		$select->condition($or_query1);
	  }
	  if(!empty($moduleId)){
	  	$select->leftjoin('slt_module_crs_mapping', 'map','course.id = map.course_id AND map.program_id = '.$entityId.' AND map.module_id = '.$moduleId.'');
	    $or_query = db_or();
		$or_query->condition('map.id', '','IS NULL');
		$or_query->where('map.course_id != course.id');
		$select->condition($or_query);
	  	//$select->condition('course.id', explode(',', $excludedId) , 'NOT IN');
	  }else if(!empty($excludedCourseId)){
	  	$select->condition('course.id', explode(',', $excludedCourseId) , 'NOT IN');
	  }
	  $select->condition('course.status', 'lrn_crs_sts_atv','=');
	  filterByUserAccessPrivileges($select,'course','',1,'cre_sys_obt_crs','attach','course');	   
	  		//$select->orderBy('title');
	  expDebug::dPrintDBAPI("Attach Course in TP " , $select);
	  if(empty($queryRequired)){
	    return $select->execute()->fetchAll();
	  } else {
	    return $select;
	  }
  } catch (Exception $ex) {
  	watchdog_exception('loadAttachCourseForTP', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getTPAttachedCoursesCount() - Returns the number of courses (direct + recertify) currently attached to a training plan
 */
function getTPAttachedCoursesCount($tpId) {
	try {
	    // Find attached courses from slt_module_crs_mapping
      $selectCoursMapping = db_select('slt_module_crs_mapping', 'modcrsmap');
      $selectCoursMapping->addExpression('COUNT(modcrsmap.course_id)', 'count');
      $selectCoursMapping->condition('modcrsmap.program_id', $tpId, '=');
      expDebug::dPrintDBAPI(' $selectCoursMapping SQL = ' , $selectCoursMapping);
      $modCrsMapCount = $selectCoursMapping->execute()->fetchField();
      expDebug::dPrint(' $modCrsMapCount = ' . $modCrsMapCount, 3);

      /*// Find attached courses from slt_re_certify
      $selectRecertifyCrsMapping = db_select('slt_re_certify', 'recertify');
      $selectRecertifyCrsMapping->addExpression('COUNT(recertify.course_id)', 'count');
      $selectRecertifyCrsMapping->condition('recertify.program_id', $tpId, '=');
      expDebug::dPrintDBAPI(' $selectRecertifyCrsMapping SQL = ' , $selectRecertifyCrsMapping);
      $recertifyCrsMapCount = $selectRecertifyCrsMapping->execute()->fetchField();
      expDebug::dPrint(' $recertifyCrsMapCount = ' . $recertifyCrsMapCount, 3);*/

      return ($modCrsMapCount);
      } catch (Exception $ex) {
      	watchdog_exception('getTPAttachedCoursesCount', $ex);
      	expertusErrorThrow($ex);
      }
}

/*
 * attachCourseDetailsList() -
 */
function attachCourseDetailsList($form, &$form_state,$entityId,$entityType) {

  $txn = db_transaction();

  try {
  	expDebug::dPrint("form value -->> " .print_r($form,true),5);
  	expDebug::dPrint("form state  -->> " .print_r($form_state,true),5);
    $entityId = $form_state['entity_id'];
    $moduleId = $form_state['module_id'];
    $entityType = $form_state['entity_type'];
    $jobStatus = checkBackgroundJobs($entityId);
    if($jobStatus == '1') {
    	return '';
    }
    $uniqueId  = "TPAttachCourse-$entityId-$entityType";
    //$grpList = explode(',',listOfGrpsProgram($moduleId));
    expDebug::dPrint("grp list last element >>> ". end($grpList) ,4);
    
    //Last Inserted groups
     $select = db_select("slt_module_groups", "grps");
	 $select->addField('grps', 'id', 'group_id');
	 $select->condition('grps.module_id', $moduleId, '=');
	 $select->condition('grps.program_id', $entityId, '=');
	 //$select->condition('grps.sequence', 9999, '=');
	 $select->orderBy('grps.sequence','DESC');
	 $select->orderBy('grps.id','DESC');
	 $select->range(0,1);
	 expDebug::dPrintDBAPI(' SQL = ' , $select);
	 $grpId = $select->execute()->fetchField();
     expDebug::dPrint("grp id last element >>> ". $grpId ,4);
    
    //$newGroup = end($grpList);
	$seq = getGroupCodeFromGrpTitle($moduleId,$grpId);
	
	expDebug::dPrint("debugging result >> ". print_r($grp_det,true),5);
    //$auto_code = !empty($grp_code) ? $grp_code : "grpco_".rand(1,99999);
    $lastSeqNo = !empty($seq) ? $seq+1 : 1;
    $grpId = !empty($grpId) ? $grpId : 0;
  
    if(!empty($form_state['input']['hidden_idlist_'.$uniqueId]) || !empty($form_state['hidden_idlist_'.$uniqueId])){
      $userId = getSltpersonUserId();
      if (!empty($form_state['input']['hidden_idlist_'.$uniqueId])) {
          $associateIdListArray = explode(',', $form_state['input']['hidden_idlist_'.$uniqueId]);
      }else {
          $associateIdListArray = explode(',', $form_state['hidden_idlist_'.$uniqueId]);
      }
      $objectType   = getObjectypeFromProgramID($entityId);
      $currentUserId = getIdOfLoggedInUser();
      foreach ($associateIdListArray as $associatedEntityId) {
        $checkTp = checkingTpAssingnedCourses($associatedEntityId, $entityId,$moduleId);
        if($checkTp == 0) {
          $insertCoursMapping = db_insert('slt_module_crs_mapping');
          $fields = array(
                            'program_id'  => $entityId,
                            'module_id'   => $moduleId,
                            'course_id'   => orgValidate($associatedEntityId),  // Start #0050212 - Added by ganesh babu v for avoid SQL Query PDO exception error during security fix on June 12 2015 6:38 PM
                            'object_type' => $objectType,
          					//'group_title' => $newGroup,
          					//'group_code'  => !empty($newGroup) ? $auto_code : '',
          					'group_id' => $grpId,
             	            'sequence'    => $lastSeqNo,
                            'created_by'  => $currentUserId,
                            'created_on'  => now(),
          					'updated_by'  => $currentUserId,
          					'updated_on'  => now(),
                            'is_required' => 'Y',
                     );
          $insertCoursMapping->fields($fields);
          expDebug::dPrintDBAPI('insert course mapping >>> ',$insertCoursMapping);
          $insertCoursMapping->execute();
          make_audit_trail_entries($fields, getIdOfLoggedInUser() , null ,$entityId, 'Inserted', $entityType, 'exp_sp_administration_program',
            	                                 'attachCourseDetailsList', array('program_id', 'module_id', 'course_id'));
		        	$lastSeqNo++;
        }
      } // end foreach

      	// get last sequence number
		$select = db_select('slt_module_groups', 'sltmodule');
	 	$select->addExpression('COUNT(sltmodule.id)', 'countnewval');
	 	$select->condition('sltmodule.program_id', $entityId, '=');
	 	$select->condition('sltmodule.module_id', $moduleId, '=');
	 	$select->condition('sltmodule.sequence', 9999, '!=');
	 	expDebug::dPrintDBAPI(' fetch group list all  = ', $select);
	 	$tpmoduleCount = $select->execute()->fetchField();
		expDebug::dPrint(' tp module count = ' . $tpmoduleCount , 3);

		$update = db_update("slt_module_groups");
		$update->fields(array('sequence'=>$tpmoduleCount+1));
		$update->condition('id', $grpId);
		$update->condition('sequence', 9999 , '=');
		expDebug::dPrintDBAPI('update module grp list ' , $update);
		$update->execute();
		

      //updateTPGroupsSeqFromAttachedCoursesSeq($entityId);

      $form_state['rebuild'] = TRUE;
    } // end if
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('attachCourseDetailsList', $ex);
    throw $ex;
  }

  unset($txn); // Commit txn

}

/*
 * tpAttachedCoursesSeqOrderRecCompare() - Function called by usort in getTPAttachedCoursesSequenceOrder() for sorting records by sequence number
 */
function tpAttachedCoursesSeqOrderRecCompare($seqOrderRecA, $seqOrderRecB) {
	try {
  return ($seqOrderRecA->sequence - $seqOrderRecB->sequence);
  } catch (Exception $ex) {
  	watchdog_exception('tpAttachedCoursesSeqOrderRecCompare', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getTPAttachedCoursesSequenceOrder() - Prepare sequence order as is passed as argument to sequenceAttachCourse()
 */
function getTPAttachedCoursesSequenceOrder($programId,$moduleId,$groupId) {

  try {

    $selectQry1 = db_select('slt_module_crs_mapping', 'modcrsmap');
    $selectQry1->addField('modcrsmap', 'sequence', 'sequence');
    $selectQry1->addField('modcrsmap', 'id', 'id');
    $selectQry1->condition('modcrsmap.program_id', $programId);
    $selectQry1->condition('modcrsmap.module_id', $moduleId);
    $selectQry1->condition('modcrsmap.group_id', $groupId);

	  //expDebug::dPrintDBAPI(' SQL = ' , $selectQry1);
    $seqOrderRecList = $selectQry1->execute()->fetchAll();
    expDebug::dPrintDBAPI('get sequence of attached courses to TP' , $selectQry1);
    expDebug::dPrint(' $seqOrderRecList = ' .print_r($seqOrderRecList, true) , 3);
    usort($seqOrderRecList, "tpAttachedCoursesSeqOrderRecCompare");
    expDebug::dPrint(' after usort, $seqOrderRecList = ' .print_r($seqOrderRecList, true) , 3);
    $sequenceOrder = '';
    $first = true;
    if(count($seqOrderRecList) > 0){
    foreach ($seqOrderRecList as $seqOrderRec) {
      if ($first) {
	       $sequenceOrder .= $seqOrderRec->id;
       $first = false;
      }
      else {
	      	$sequenceOrder .= ',' . $seqOrderRec->id;
      }
    } // end foreach
    }else{
    	$update = db_update("slt_module_groups");
		$update->fields(array('sequence'=>9999));
		$update->condition('id', $groupId);
		expDebug::dPrintDBAPI('update module grp list ' , $update);
		$update->execute();
    }
    
    expDebug::dPrint(' $sequenceOrder = ' . print_r($sequenceOrder, true) , 4);

    return $sequenceOrder;
  }
  catch (Exception $ex) {
    watchdog_exception('getTPAttachedCoursesSequenceOrder', $ex);
    throw $ex;
  }
}

/*
 * sequenceAttachCourse - Re-sequence the attached courses, returns list of used module ids
 */
function sequenceAttachCourse($sequenceOrder,$program_id,$module_id) {
	$txn = db_transaction();
  try {
  	expDebug::dPrint("sequence order Json " . print_r($sequenceOrder,true),5);
  	$seqArry = json_decode(stripslashes($sequenceOrder));
  	expDebug::dPrint("sequence order Array " . print_r($seqArry,true),5);
    $groups = array();
    $grpseq = 1;
    foreach($seqArry as $seq){
    	//$groups[] = $seq->GroupName;
    $sequence = 1;
	    if(count($seq->CrsId) > 0) {
	    	//$new_grp = empty($seq->GroupCode) ? "grpco_".rand(1,99999) : $seq->GroupCode;
	       foreach($seq->CrsId as $crs){
       		 $fields = array();
	         $tableName ='slt_module_crs_mapping';
	         $fields['sequence'] = $sequence;
		  	 //$fields['group_title'] = $seq->GroupName;
		     $fields['group_id'] =  $seq->GroupId;
	         updateCourseAttachSequence($tableName,$fields, $program_id, $module_id,$crs);
        $sequence++;
      }
       		$update = db_update("slt_module_groups");
			$update->fields(array('sequence'=>$grpseq));
			$update->condition('id', $seq->GroupId);
			expDebug::dPrintDBAPI('update module grps sequence order ' , $update);
			$update->execute();
			$grpseq++;
      	}else{
      		$update = db_update("slt_module_groups");
			$update->fields(array('sequence'=>9999));
			$update->condition('id', $seq->GroupId);
			expDebug::dPrintDBAPI('update module grps sequence ' , $update);
			$update->execute();
    }
    }
	$grp_list = implode(',',$groups);
	expDebug::dPrint("final group list >>> " . print_r($grp_list,true),5);

	// Update group list in slt_module table
	/*$update = db_update("slt_module");
	$update->fields(array('group_list'=>$grp_list));
	$update->condition('id', $module_id);
	expDebug::dPrintDBAPI('update module grp list ' , $update);
	$update->execute();*/
    
	return;
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('sequenceAttachCourse', $ex);
    throw $ex;
  }

  unset($txn); // Commit txn
}

/*
 * updateCourseAttachSequence - Sequence for Attach Course
 */
function updateCourseAttachSequence($tableName,$fields, $program_id, $module_id,$crs) {
  try {
	  $userId = getSltpersonUserId();

	  $update = db_update($tableName);
	  $update->fields($fields);
	  $update->condition('program_id', $program_id);
	  $update->condition('module_id', $module_id);
	  $update->condition('id', $crs);
	  expDebug::dPrintDBAPI('update sequence details ' , $update);
	  $update->execute();

	  // Update group sequence number
	 /* $moduleId = fetchModuleIdFromCourseAttachTable($tableName, $mappingId);
	  if (!in_array($moduleId, $moduleIdOrderedList)) {
	  	$moduleIdOrderedList[] = $moduleId;
	  	$modSeq = sizeof($moduleIdOrderedList);
	  	setTPGroupSequenceNumber($moduleId, $modSeq);
	  }*/
	  make_audit_trail_entries($fields, $userId,$module_id,$program_id, 'Sequencing', 'Sequence' , 'exp_sp_administration_attach_course', 'updateCourseAttachSequence',array('sequence'));
	  }
  catch (Exception $ex) {
    watchdog_exception('updateCourseAttachSequence', $ex);
    throw $ex;
  }

}

/*
 * fetchModuleIdFromCourseAttachTable - Fetch the module if from crs mapping table
 */
function fetchModuleIdFromCourseAttachTable($tableName, $mappingId){
	try {
	  
	  $select = db_select($tableName, 'attach');
	  $select->addField('attach', 'module_id', 'module_id');
	  $select->condition('id', $mappingId);
	  expDebug::dPrintDBAPI(' SQL = ' , $select);
	  $moduleId = $select->execute()->fetchField();
	  expDebug::dPrint(' $moduleId = ' . print_r($moduleId, true) , 3);
	  return $moduleId;
	}
  catch (Exception $ex) {
    watchdog_exception('fetchModuleIdFromCourseAttachTable', $ex);
    throw $ex;
  }
}

/*
 * checkingTPModuleGroupName - Checking the group name count
 */
function checkingTPModuleGroupName($modulename, $program_id,$groupName,$module_id = '') {
 try {
  $select = db_select('slt_module', 'groupingname');
  // Set the conditions
  $select->condition('groupingname.title', $modulename, '=');
  $select->condition('groupingname.program_id', $program_id, '=');
  if(!empty($module_id))
  	$select->condition('groupingname.id', $module_id, '!=');
  	
  $select->addField('groupingname', 'id', 'id');
  expDebug::dPrintDBAPI('gayu TP $select obj = ' , $select);

  // Execute the query
  $getgroupid = $select->execute()->fetchField();
  expDebug::dPrint(' $getmoduleid = ' . print_r($getgroupid, true) , 3);

  return $getgroupid ;
  } catch (Exception $ex) {
  	watchdog_exception('checkingTPModuleName', $ex);
  	expertusErrorThrow($ex);
  }
}
function checkGrpNameExistsForModule($moduleid,$program_id,$groupName){
	try{	
		 $select = db_select("slt_module_groups", "grps");
		 $select->addField('grps', 'id', 'group_id');
		 $select->condition('grps.module_id', $moduleid, '=');
		 $select->condition('grps.program_id', $program_id, '=');
		 $select->condition('grps.title', $groupName, '=');
		 expDebug::dPrintDBAPI(' SQL checkGrpNameExistsForModule = ' , $select);
		 $grpId = $select->execute()->fetchField();
	 	 return $grpId;
	} catch (Exception $ex) {
		watchdog_exception('getModulegrpList', $ex);
  		expertusErrorThrow($ex);
  	}
}


/*
 * * getModulegrpList - Fetch the module group list
 */
function getModulegrpList($module_id) {
	try{
		$select = db_select('slt_module', 'module');
		$select->addField('module','group_list', 'group_list');
		$select->condition('module.id', $module_id, '=');
		expDebug::dPrintDBAPI('getRemoveUserListField $select obj = ' , $select);
		$ModulegrpList = $select->execute()->fetchField();
		expDebug::dPrint('$ModulegrpList = ' . $ModulegrpList,4);
		return $ModulegrpList;
	} catch (Exception $ex) {
		watchdog_exception('getModulegrpList', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * checkingTpAssingnedRecertify - Checking assigned recertify count for the training program
 */
function checkingTpAssingnedRecertify($courseid, $programid){
 try {
  $select = db_select('slt_re_certify', 'recertify');
  $select->addExpression('COUNT(recertify.id)', 'count');
  $select->condition('recertify.course_id', $courseid, '=');
  $select->condition('recertify.program_id', $programid, '=');
  // Execute the query
  $uniquecount = $select->execute()->fetchField();
  expDebug::dPrint(' $uniquecount = ' . print_r($uniquecount, true) , 3);

  return $uniquecount; // a valid new name
  } catch (Exception $ex) {
  	watchdog_exception('checkingTpAssingnedRecertify', $ex);
  	expertusErrorThrow($ex);
  }
}

function attachedcourse_group_update($entityId,$entityType,$newGroup,$courseId,$recertifyFlag,$callfrom=''){
	$txn = db_transaction();

	try {
      $groupNameTruncated = $newGroup;
	  	$random_num=rand(1,99999);
      $auto_code="grpco_".$random_num;
      $currentUserId = getIdOfLoggedInUser();
		if ($newGroup != '' && !empty($newGroup)) {
           
		if($entityType == 'module'){
			$groupNameTruncated = $newGroup;
			$checkGroupId  =   checkingTPModuleGroupName($newGroup, $entityId,'',$courseId);
           if(empty($checkGroupId)){
           		$updStmt = db_update('slt_module');
                $fields = array(
                            'title'              => stripslashes($newGroup),
    	                    'updated_by'         => getIdOfLoggedInUser(),
                						'updated_on'         => now(),
                          );
    	        $updStmt->fields($fields);
    	        $updStmt->condition('program_id',$entityId,'=');
    	        $updStmt->condition('id',$courseId,'=');
				expDebug::dPrintDBAPI(' TP module update = ' , $updStmt);
    	        $attDefIdUpd = $updStmt->execute();
    	        if($attDefIdUpd == 1){
    	        	$return_obj =1;
    	        }
		            make_audit_trail_entries($fields, getIdOfLoggedInUser() , null ,$entityId, 'Added ','Module','exp_sp_administration_attach_course','new module', array('title', 'code','sequence'));
           }else{
               $groupNameTruncated = "module name exist";
           }
           }else{

           }

		}
		expDebug::dPrint("group truncated name >> ". $groupNameTruncated);
		print $groupNameTruncated;
		if($callfrom =='api'){
			return $return_obj;
	    }
	}
	catch (Exception $ex) {
	    $txn->rollback();
	    watchdog_exception('attachedcourse_group_update', $ex);
	    throw $ex;
  	}

  unset($txn);

}

/*
 * updateTPGroupsSeqFromAttachedCoursesSeq() - refresh sequence numbers of TP groups based on TP attached courses' (normal + re-certify) sequence number
 */
function updateTPGroupsSeqFromAttachedCoursesSeq($entityId) {
	$txn = db_transaction();
	try {
    // Fetch unique module ids of the attached courses in sequence number of attached courses.
    $selectQry2 = db_select('slt_re_certify', 'recertify');

    // Add fields to fetch
    $selectQry2->addField('recertify', 'sequence', 'sequence');
    $selectQry2->addField('recertify', 'module_id', 'module_id');

    // Add condition
    $selectQry2->condition('recertify.program_id', $entityId, '=');
    // Fetch unique module ids of the attached courses in sequence number of attached courses.
    $selectQry1 = db_select('slt_module_crs_mapping', 'modcrsmap');

    // Add fields to fetch
    $selectQry1->addField('modcrsmap', 'sequence', 'sequence');
    $selectQry1->addField('modcrsmap', 'module_id', 'module_id');

    // Add condition
    $selectQry1->condition('modcrsmap.program_id', $entityId, '=');
    $selectQry1->union($selectQry2, 'UNION');

    expDebug::dPrintDBAPI(' $selectQry1 SQL = ' , $selectQry1);
    // Execute the query
    $modCrsSeqMap = $selectQry1->execute()->fetchAllKeyed();
    expDebug::dPrintDBAPI('$modCrsSeqMap',$selectQry1);
    ksort($modCrsSeqMap);
    expDebug::dPrint(' after ksort, $modCrsSeqMap = ' .print_r($modCrsSeqMap, true) , 3);
    $modCrsSeqMap = array_unique($modCrsSeqMap);
    expDebug::dPrint(' after array_unique, $modCrsSeqMap = ' .print_r($modCrsSeqMap, true) , 3);
	  // Freshly assign sequence number to module ids
	  $usedModuleIds = array();
    $sequence = 1;
    foreach($modCrsSeqMap as $key => $moduleId) {
      setTPGroupSequenceNumber($moduleId, $sequence);
      $usedModuleIds[] =  $moduleId;
      $sequence++;
    } // End foreach

    // Delete any group that is now unused
   // deleteUnusedTPGroups($entityId, $usedModuleIds);
	}
	catch (Exception $ex) {
		$txn->rollback();
    watchdog_exception('updateTPGroupsSeqFromAttachedCoursesSeq', $ex);
    throw $ex;
  }

  unset($txn);
}

/*
 * setTPGroupSequenceNumber() - Set the sequence number of a TP group
 */
function setTPGroupSequenceNumber($moduleId, $sequence) {
  try {
    $fields['sequence'] = $sequence;
	
    $updateStmt = db_update('slt_module');
    $updateStmt->fields($fields);
    $updateStmt->condition('id', $moduleId);
    expDebug::dPrintDBAPI(' $updateStmt SQL = ' , $updateStmt);
    // Execute the update statement
    $updateStmt->execute();
  }
  catch (Exception $ex) {
    watchdog_exception('setTPGroupSequenceNumber', $ex);
    throw $ex;
  }
}

/*
 * deleteUnusedTPGroups() - Deletes any unused tp groups, all used group module ids are in the argument $usedModuleIds
 */
function deleteUnusedTPGroups($programId, $usedModuleIds) {
	try {
	    expDebug::dPrint('hello deleteUnusedTPGroups');
	    if($form_state['values']['create_user_id'])
	        $loggedInUserId = $form_state['values']['create_user_id'];
	    else
	        $loggedInUserId  = getIdOfLoggedInUser();
	    
	    $delete_id = db_select('slt_module', 'm');
	    $delete_id->addField('m', 'id', 'id');
	    if (!empty($usedModuleIds)) {
	        $delete_id->condition('id', $usedModuleIds, 'NOT IN');
	    }
	    $delete_id->condition('program_id', $programId, '=');
	    $del=$delete_id->execute()->fetchAll();
	    expDebug::dPrintDBAPI('$delete_id', $del);
	    
	    
	  $deleteStmt = db_delete('slt_module');
	  if (!empty($usedModuleIds)) {
      $deleteStmt->condition('id', $usedModuleIds, 'NOT IN');
	  }
    $deleteStmt->condition('program_id', $programId, '=');
    expDebug::dPrintDBAPI(' $deleteStmt SQL = ' , $deleteStmt);
    $numUpdated = $deleteStmt->execute();
    
    //Make delete log entries
    foreach ($del as $val){
        $fields = array(
                'table_name' => 'slt_module',
                'entity_id' => $val->id,
                'entity_type' => 'module',
                'module_name' => 'exp_sp_administration_attach_course',
                'functionality_name' => 'deleteUnusedTPGroups',
                'api_name' => null,
                'deleted_on' => now(),
                'deleted_by' => $loggedInUserId,
                'parent1_entity_id' => $programId,
                'parent1_entity_type' => 'program_id',
        );
        deleted_log_entry($fields);
    }expDebug::dPrint('slt_module table log');
	}
  catch (Exception $ex) {
    watchdog_exception('deleteUnusedTPGroups', $ex);
    throw $ex;
  }
}

function attachedcourse_mandatory_update($listValues){
 try {
  $listVal         = explode('-',$listValues);
  $entityId        = $listVal[0];
  $entityType      = $listVal[1];
  $courseId        = $listVal[2];
  $recertifyvalue  = $listVal[3];
  $mandatoryOption = $listVal[4];

      $default = db_query("SELECT is_required FROM slt_module_crs_mapping WHERE program_id = :pid and module_id = :mid and course_id = :cid", array(':pid' =>$entityId,':mid' =>$recertifyvalue, ':cid' => $courseId))->fetchAll();
                $updStmt = db_update('slt_module_crs_mapping');
    	        $custom  = NULL;
    	        $fields = array(
    	                    'is_required'     => $mandatoryOption,
    	                    'updated_by'         => 1,
    	                    'updated_on'         => now()
    	                  );
    	        $updStmt->fields($fields);
    	        $updStmt->condition('program_id',$entityId,'=');
      $updStmt->condition('module_id',$recertifyvalue,'=');
    	        $updStmt->condition('course_id',$courseId,'=');

    	        $attDefIdUpd = $updStmt->execute();
    	        make_audit_trail_entries($fields, getIdOfLoggedInUser() , $default, $entityId ,'Updated',$entityType,'exp_sp_administration_attach_course','attachedcourse_mandatory_update',
                                          array('is_required'));
  } catch (Exception $ex) {
  	watchdog_exception('attachedcourse_mandatory_update', $ex);
  	expertusErrorThrow($ex);
  }
}

/*function attachedcourse_recertify_update($entityId, $entityType, $recertifyvalue, $courseId) {
	try {
	  $loggedInUserId = getSltpersonUserId();
	  if ($recertifyvalue == 0) {
	    if (!empty($entityId) && !empty($courseId) ) {
	      $txn = db_transaction();
	      try {
	        $deleteStmt = db_delete('slt_re_certify');
	        $deleteStmt->condition('program_id', $entityId,'=');
	        $deleteStmt->condition('course_id', $courseId, '=');
	        $numUpdated = $deleteStmt->execute();
	        add_audit_trail_entry($loggedInUserId, $entityId, $entityType, 'exp_sp_administration_attach_course', 'attachedcourse_recertify_update',
	                                                                                'Recertity removed');

	        // Resequence attached courses and their groups
	        expDebug::dPrint(' Re-sequencing attached courses after delete',5);
	        $sequenceOrder = getTPAttachedCoursesSequenceOrder($entityId);
	        $usedModuleIds = sequenceAttachCourse($sequenceOrder);

	        // Delete any group that is now unused
          //deleteUnusedTPGroups($entityId, $usedModuleIds);
	      }
	      catch (Exception $ex) {
	        $txn->rollback();
	        watchdog_exception('attachedcourse_recertify_update (delete op)', $ex);
	        expertusErrorThrow($ex);
	      }

	      unset($txn); // Commit the db transaction
	    }
	  }
	  else { // $recertifyvalue = 1;

	    $checkRecertify = checkingTpAssingnedRecertify($courseId, $entityId);
	    if ($checkRecertify == 0) { // Record does not already exist
	    	$lastSeqNo = getTPAttachedCoursesCount($entityId);
	    	$moduleId = getModuleIdOfTPAttachedCourse($entityId, $courseId, 'slt_module_crs_mapping');
	    	$txn = db_transaction();
	      try {
	        $insertRecertify = db_insert('slt_re_certify');
	        $fields = array(
	                  'program_id'  => $entityId,
	                  'course_id'   => $courseId,
	      			  		'module_id'   => $moduleId,
	                  'sequence'    => ++$lastSeqNo,
	                  'created_by'  => $loggedInUserId,
	                  'created_on'  => now(),
	        					'updated_by'  => $loggedInUserId,
	        					'updated_on'  => now(),
	                  'is_required' => 'Y',
	                );
	        $insertRecertify->fields($fields);
	        $insertRecertify->execute();
	        make_audit_trail_entries($fields, $loggedInUserId, null, $entityId, 'Inserted', 'cre_sys_obt_trp', 'exp_sp_administration_attach_course',
	                                   'attachedcourse_recertify_update', array('program_id', 'course_id', 'module_id','is_required'));
	      }
	      catch (Exception $ex) {
	        $txn->rollback();
	        watchdog_exception('attachedcourse_recertify_update (add new op)', $ex);
	        expertusErrorThrow($ex);
	      }
	      unset($txn); // Commit the db transaction
	    }
	  }
  }
  catch (Exception $ex) {
    watchdog_exception('attachedcourse_recertify_update', $ex);
    expertusErrorThrow($ex);
  }
}
*/
/*
 * getModuleIdOfTPAttachedCourse() - Fetches the module id of a tp attached course id
 */
function getModuleIdOfTPAttachedCourse($programId, $courseId, $tableName) {
  try {
    $select = db_select($tableName, 'attach');
    $select->addField('attach', 'module_id', 'module_id');
    $select->condition('attach.course_id', $courseId, '=');
    if ($tableName == 'slt_module_crs_mapping') {
    	 $select->condition('attach.program_id', $programId, '=');
    }
    else {
    	$select->condition('attach.program_id', $programId, '=');
    }

    expDebug::dPrintDBAPI(' SQL = ' , $select);
    $moduleId = $select->execute()->fetchField();
    expDebug::dPrint(' $moduleId = ' . $moduleId, 5);
    return $moduleId;
  }
  catch (Exception $ex) {
    watchdog_exception('getModuleIdOfTPAttachedCourse', $ex);
    throw $ex;
  }
}

function updateGroupForNewAttachCourse($newGroup,$entityId){
  $txn = db_transaction();
  try {
    $module_id   = '';
    $random_num  = rand(1,99999);
    $auto_code   = "grpco_".$random_num;
    $currentUserId = getIdOfLoggedInUser();
    if($newGroup != '' && !empty($newGroup)){
      $checkGroupId  =   checkingTPModuleGroupName($newGroup,$entityId);
      if(empty($checkGroupId)){
    	//$object_type=getObjectypeFromProgramID($entityId);
        $select = db_select('slt_module', 'sltmodule');
        $select->addExpression('COUNT(sltmodule.program_id)', 'countnewval');
        $select->condition('sltmodule.program_id', $entityId, '=');
        $tpcourseCount = $select->execute()->fetchField();

        $insertStmt = db_insert('slt_module');
        $fields = array(
                    'program_id'  		   => $entityId,
                    'title'              => $newGroup,
                    'code'               => $auto_code,
                    'created_by'         => $currentUserId,
                    'sequence'			     => $tpcourseCount+1,
                    'created_on'         => now(),
        						'updated_by'         => $currentUserId,
        						'updated_on'         => now(),
                    'status'             => 'spp_tpm_mdl_atv',
                    'is_required'        => '1',
                    'parent_id'			     => '0',

                  );

        // Select the fields and the values to be inserted
        $insertStmt->fields($fields);
        $module_id = $insertStmt->execute();
        //make_audit_trail_entries($fields, getIdOfLoggedInUser() , null ,$module_id, 'Added ','Module','exp_sp_administration_program','new module', array('program_id', 'title', 'code','sequence','is_required'));
      }else{
          $module_id=$checkGroupId;
      }
    }
    return $module_id;
  }catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('updateGroupForNewAttachCourse', $ex);
    throw $ex;
  }
  unset($txn);
}
?>