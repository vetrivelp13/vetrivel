<?php

function displayAssessmentDetails(&$form, &$form_state, $entityId, $entityType, $uniqueId, $mode='',$module_id = ''){
	try{
  global $secure_entities,$user;	;
  $customHiddenButtonId ='esign-assessment-button';
  $assessmentResult = fetchAssessmentList($entityId, $entityType);
  if (in_array($entityType, $secure_entities)) {
  	$entityId_encrypted = core_encrypt($entityId); // URL encryption
  } else {
  	$entityId_encrypted = $entityId;
  }
  $uniqueId = "assessment-".$entityId_encrypted."-{$entityType}";
  //$uniqueId = "assessment-{$entityId}-{$entityType}";

  $count = count($assessmentResult);

  $msg = t('MSG382');

  $wrapperId = 'assessment-grid-wrapper-'.$uniqueId;
  $formName = 'assessment_form_'.$uniqueId;
  $form_state['form_unique_name'] = $formName;

  $form['attr_action'] = array(
    '#type' => 'hidden',
    '#name' => 'attr_action',
    '#value' => 'assessment',
  );

  $setminHight = ($entityType == 'cre_sys_obt_crt' || $entityType == 'cre_sys_obt_trn' || $entityType == 'cre_sys_obt_cur') ? 'admin-training-program-tab-datagrid-wrapper' : 'admin-course-class-tab-datagrid-wrapper';

  $form[$formName] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="admin-data-grid"><div id="'.$wrapperId.'" class="admin-datagrid-pagination assess-attach-grid-wrapper '.$setminHight.'">',
    '#suffix' => '</div></div>',
  );

  $form[$formName]['hidden_idlist'] = array(
    '#type' => 'hidden',
    '#name' => 'hidden_idlist_'.$uniqueId,
    '#id' => 'datagrid-idlist-'.$uniqueId,
  );

  $form[$formName]['hidden_certificate_path'] = array(
				'#type' => 'hidden',
				'#name' => 'hidden_certificate_path_'.$uniqueId,
				'#id' => 'hidden-certificate_path-'.$uniqueId,
		);
  
  
  $displayMode = $form_state['storage']['mode'];

  if($count == 0 && empty($displayMode)){

    $addAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
    $addAttributes['data-wrapperid'] = array($wrapperId);

    if(isset($form_state['storage']['display_mode']) && $form_state['storage']['display_mode'] == 'view' || (isset($form_state['storage']['step']) && ($form_state['storage']['step'] == 'view') && ($entityType != 'cre_sys_obt_cls'))){

      $form[$formName]['add_prereqiuisite_text'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="no-records-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.t('MSG718').'</div></div>',
      );

    } else {

      $assessmentDetails = fetchAssessmentList($entityId, $entityType);
  	  $courseIdArr = array();
      $excludedCourseId = '""';
      if(count($assessmentDetails) > 0){
        foreach($assessmentDetails as $details){
          $assessmentIdArr[] = $details->sur_id;
        }
      }
      $excludedAssessmentId = implode(',', $assessmentIdArr);
      $excludedAssessmentId = empty($excludedAssessmentId) ? 0 : $excludedAssessmentId;

      $listquery = loadCtoolDataGridForAssessment('', '', $excludedAssessmentId,$entityId, $entityType);
      //$priv_visible = adminVisibilityForAssign('sry_det_typ_ass');     
      if(count($listquery) == 0  ){ // || empty($priv_visible)

        $msg = t('MSG340');

        $form[$formName]['add_assessment'] = array(
          '#type' => 'markup',
          '#prefix' => '<div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-greyout-bg"></div>',
          '#suffix' => '<div class="admin-save-button-middle-greyout-bg">'.t('LBL359').'</div><div class="admin-save-button-right-greyout-bg"></div></span></div>',
        );

        $form[$formName]['add_assessment_text'] = array(
          '#type' => 'markup',
          '#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.$msg.'</div></div></div>',
        );

      } else {

        $form[$formName]['add_assessment'] = array(
          '#type' => 'submit',
          '#prefix' => '<div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-bg"></div>',
          '#suffix' => '<div class="admin-save-button-right-bg"></div></span></div>',
          '#title' => t('LBL359'),
          '#name' => 'Add assessment',
          '#value' => t('LBL359'),
          '#submit' => array('add_assessment'),
          '#attributes' => $addAttributes,
          '#ajax' => array(
            'callback' => 'assessmentGrid',
            'wrapper' => $wrapperId,
          ),
        );

        $form[$formName]['add_assessment_text'] = array(
          '#type' => 'markup',
          '#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.$msg.'</div></div></div>',
        );

      }

    }

  } else {

    $assessmentDetails = fetchAssessmentList($entityId, $entityType,'',$module_id);
    $module_cnt = getModuleIdFromObjectId($entityId);
	$courseIdArr = array();
    $excludedCourseId = '""';
    if(count($assessmentDetails) > 0){
      foreach($assessmentDetails as $details){
        $assessmentIdArr[] = $details->sur_id;
      }
    }
    $excludedAssessmentId = implode(',', $assessmentIdArr);
    $excludedAssessmentId = empty($excludedAssessmentId) ? 0 : $excludedAssessmentId;

    if($displayMode == 'edit'){

      $form[$formName][] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_search_container">',
      );

      $searchAjaxEvent = 'click';
      $searchAttributes['onClick'][] = '$("body").data("mulitselectdatagrid").searchDataGrid("'.$displayMode.'", "assessment", $(\'#assessment-autocomplete\').val(), "'.$entityId_encrypted.'", "'.$entityType.'", "'.$excludedAssessmentId.'"); return false;';

      $acMenuPath = 'administration/learning/assessment/autocomplete';

      $form[$formName]['add_assessment_search'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_auto_search">',
      );

      $autoCompleteAttributes['class'][] = 'admin_ac_input_mainform';
      $autoCompleteAttributes['onkeydown'][]  = '$("body").data("mulitselectdatagrid").multiSelectSearchBoxKeyDown(event)';
      addACFieldForMultiSelect($form[$formName], 'assessment-autocomplete', $acMenuPath, '', '', '', array(), array('excluded_assessment_id' => $excludedAssessmentId, 'type' => '"sry_det_typ_ass"', 'id_required' => 0), false, false, t('LBL758'), $autoCompleteAttributes);

      $form[$formName]['add_assessment_search1'] = array(
      '#type' => 'markup',
      '#markup' => '</div>',
      );

      $searchAttributes['Class'][] = 'admin-pagination-search-go';
       $form[$formName]['search_assessment'] = array(
        '#type' => 'submit',
        '#title' => t('LBL304'),
        '#name' => 'Search assessment',
        '#submit' => array('search_assessment'),
        '#attributes' => $searchAttributes,
        '#ajax' => array(
          'event' => $searchAjaxEvent,
        ),
      );
      $form[$formName][] = array(
      '#type' => 'markup',
      '#markup' => '<div class="filter-search-start-date-right-bg"></div><div class="clearBoth"></div></div>',
      );


    } else {

      $displayMode = 'view';

    }

    $form[$formName]['assessment_view_grid_markup'] = array(
      '#type' => 'markup',
      '#markup' => '<div id="datagrid-div-'.$uniqueId.'"><table id="datagrid-container-'.$uniqueId.'"></table><div id="pager-datagrid-'.$uniqueId.'" class="pager-datagrid-common"></div></div>',
    );

    $displayModeDataGrid = $form_state['storage']['display_mode'] == 'view' || ($form_state['storage']['step'] == 'view') ? 'view_only' : $displayMode;
    $form[$formName]['assessment_view_grid_load'] = array(
      '#type' => 'markup',
      '#prefix' => '<div style="display:none"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-'.$uniqueId.'\'); $(\'body\').data(\'mulitselectdatagrid\').loadDataGrid(\''.$displayModeDataGrid.'\', \'assessment\', \'\', \''.$entityId_encrypted.'\', \''.$entityType.'\', \''.$excludedAssessmentId.'\');" width="100" height="132" /></div>',
    );
    if($displayMode == 'view'){
      if($mode != 'complete_view'){
        $addAnotherAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
        $addAnotherAttributes['data-wrapperid'] = array($wrapperId);

        $listquery = loadCtoolDataGridForAssessment('', '', $excludedAssessmentId,$entityId, $entityType);
       if($form_state['storage']['display_mode'] != 'view' && count($listquery) != 0){
        // Only one assessment can be attached
		$cond = ($entityType == 'cre_sys_obt_cls') ? 2 : (count($module_cnt)*2);
        if(count($assessmentDetails) < ($cond)){
        $form[$formName]['add_assessment_view_mode'] = array(
	          '#type'       => 'submit',
	          '#title'      => t('LBL359'),//Add assessment
	    	  	'#prefix'     => '<div id= "datagrid-add-search-button-'.$uniqueId.'"  style="display:block;" class="admin-save-button-container admin-save-button-container-view-mode"><div class="admin-save-button-left-bg"></div>',
		      	'#suffix'     => '<div class="admin-save-button-right-bg"></div></div>',
		      	'#value'      => t('LBL359'),
	          '#name'       => 'Add assessment',
	          '#submit'     => array('add_assessment'),
	          '#attributes' => $addAnotherAttributes,
	          '#ajax'       => array(
	                  'callback' => 'assessmentGrid',
	                   'wrapper' => $wrapperId,
          ),
        );
        }
        $form[$formName]['assess_hints_markup'] = array(
        		'#type' => 'markup',
        		'#markup' => '<div id="assess-hints-markup"><div class="assess-view-mode">'.t('LBL1250').'</div></div>',
        );


          $form[$formName]['add_assessment_view_mode_btn'] = array(
            '#type' => 'submit',
            '#title' => t('LBL359'),//Add assessment
      	  '#prefix' => '<div style="display:none;"  id="datagrid-noresult-msg-'.$uniqueId.'"><div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-bg"></div>',
  	      '#suffix' => '<div class="admin-save-button-right-bg"></div></span></div>',
  	      '#value' => t('LBL359'),//Add assessment
            '#name' => 'Add assessment',
            '#submit' => array('add_assessment'),
            '#attributes' => $addAnotherAttributes,
            '#ajax' => array(
              'callback' => 'assessmentGrid',
              'wrapper' => $wrapperId,
            ),
          );
          $form[$formName]['assessment_view_grid_no_msg'] = array(
            '#type' => 'markup',
            '#markup' => '<div class="admin-empty-text-msg"> '.$msg.'</div></div></div></div>',
          );
         }else{
          $form[$formName]['add_assessment_view_mode_btn'] = array(
            '#type' => 'submit',
            '#title' => t('LBL359'),//Add assessment
      	  '#prefix' => '<div style="display:none;"  id="datagrid-noresult-msg-'.$uniqueId.'"><div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-bg"></div>',
  	      '#suffix' => '<div class="admin-save-button-right-bg"></div></span></div>',
  	      '#value' => t('LBL359'),//Add assessment
            '#name' => 'Add assessment',
            '#submit' => array('add_assessment'),
            '#attributes' => $addAnotherAttributes,
            '#ajax' => array(
              'callback' => 'assessmentGrid',
              'wrapper' => $wrapperId,
            ),
          );
          $form[$formName]['assessment_view_grid_no_msg'] = array(
            '#type' => 'markup',
            '#markup' => '<div class="admin-empty-text-msg"> '.$msg.'</div></div></div></div>',
          );
        }
        }

    } else if($displayMode == 'edit'){

        $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-action-button-middle-bg white-btn-bg-middle';
        $cancelAttributes['data-wrapperid'] = array($wrapperId);

        $form[$formName]['cancel_assessment_edit_mode'] = array(
          '#type' => 'submit',
          '#value' => t('LBL109'),
          '#title' => t('LBL109'),
          '#prefix' => '<div class="admin-datagrid-save-cancel-button-align-bottom"><div class="addedit-form-cancel-container-actions admin-save-button-container resize-save_btn3"><div class="white-btn-bg-left"></div>',
          '#suffix' => '<div class="white-btn-bg-right"></div>',
          '#name' => 'Cancel assessment',
          '#submit' => array('cancelAssessmentDetails'),
          '#attributes' => $cancelAttributes,
          '#ajax' => array(
            'callback' => 'assessmentGrid',
            'wrapper' => $wrapperId,
          ),
        );

        $listquery = loadCtoolDataGridForAssessment('', '', $excludedAssessmentId,$entityId, $entityType);

        if(count($listquery) != 0){
           /* $setAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
            $setAttributes['data-wrapperid'] = array($wrapperId);

            $form[$formName]['set_assessment_edit_mode'] = array(
              '#type' => 'submit',
              '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
    	      '#suffix' => '<div class="admin-save-button-right-bg"></div></div></div>',
              '#title' => t('LBL305'),
              '#value' => t('LBL360'),
              '#name' => 'Set assessment',
              '#submit' => array('addNewAssessmentDetails'),
              '#attributes' => $setAttributes,
              '#ajax' => array(
                'callback' => 'assessmentGrid',
                'wrapper' => $wrapperId,
                'entity_id' => $entityId,
                'entity_type' => $entityType,
              ),
            );*/
            if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
              $buttonName  = 'esign-assessment';
            }else{
              $buttonName  = 'set-assessment';
            }

            $setAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
            $setAttributes['data-wrapperid'] = array($wrapperId);
            if($entityType == 'cre_sys_obt_crt'){
            	$qtipIdInit        	 = $entityId.'_'.$entityType;
            	if($user->language =='es' || $user->language =='ru' || $user->language =='fr' || $user->language =='it')
            		$popupWidth = '320';
            	else if($user->language =='zh-hans')
            		$popupWidth = '280';
            	else if($user->language =='ko' || $user->language =='ja')
            		$popupWidth = '250';
            	else if($user->language =='en-us')
            		$popupWidth = '260';
            	else
            		$popupWidth = '300';
            	
				$qtipOptSelectObj      = "{'entityId':'".$entityId."','entityType':'".$entityType."',
																	'url':'administration/selectpath/assessment/".$entityId."/".$entityType."','popupDispId':'qtip_visible_disp_".$qtipIdInit."',
																	'wid':'".$popupWidth."','heg':'120','postype':'topright',posrwid :'180',
																	'catalogVisibleId':'qtipSelectModulepathqtip_visible_disp_".$qtipIdInit."',
																	'popupDispId':'qtip_visible_disp_select_path_".$qtipIdInit."',
																	'linkid':'visible-assessment-path-".$entityId."'}";
				
				$paintMultiAction .='<div id="qtip_visible_disp_select_path_'.$qtipIdInit.'" class="admin-save-button-container set-survey-container"><div class="admin-save-button-left-bg"></div>';
				$paintMultiAction .= "<a id='visible-assessment-path-".$entityId."' class = 'admin-save-button-middle-bg survey-tp-path-qtip' onclick = \"callVisibility(".$qtipOptSelectObj.");\">".t('LBL360')."</a><span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span>".
						'<div class="admin-save-button-right-bg"></div></div><div class="clearBoth"></div><div class="clearBoth"></div></div>';
				 
				$form[$formName]['set_assessment_edit_mode'] = array(
						'#type' => 'markup',
						'#markup' =>$paintMultiAction,
				);
				
	            $form[$formName]['hidden_set_survey'] = array(
	              '#type' => 'submit',
            	  '#id' => 'add_new_'.$uniqueId,
            	  '#prefix' => '<div style="display:none">',
            	  '#suffix' => '</div>',
	              '#value' => '',
            	  '#name' => $buttonName,
            	  '#attributes' => $setAttributes,
	              '#ajax' => array(
	        		    'wrapper' => $wrapperId,
	        		    'callback' => 'addEditAssessmentRepaintForm',
	                    'entity_id' => $entityId,
	                    'entity_type' => $entityType,
	            		'module_id' => $module_id,
	                    'entity_plugin'=>'assessment',
	                    'customButtonId'=>$customHiddenButtonId,
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
            }else{
            $form[$formName]['set_assessment_edit_mode'] = array(
              '#type' => 'submit',
              '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
    	      '#suffix' => '<div class="admin-save-button-right-bg"></div></div></div></div>',
              '#title' => t('LBL305'),
              '#value' => t('LBL360'),
              '#name' => $buttonName,
              '#attributes' => $setAttributes,
              '#ajax' => array(
        		    'wrapper' => $wrapperId,
        		    'callback' => 'addEditAssessmentRepaintForm',
                    'entity_id' => $entityId,
                    'entity_type' => $entityType,
                    'entity_plugin'=>'assessment',
                    'customButtonId'=>$customHiddenButtonId,
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
            $saveAttributes= array();
            $form[$formName][$customHiddenButtonId] = array(
              '#type' => 'submit',
              '#value' => t('hidden-save'),
              '#name'	=> 'hidden-save',
              '#prefix' => '<span id = "esign-admin-div" style="display:none;">',
              '#suffix' => '</span>',
              '#attributes' => $saveAttributes,
              '#id'=>$customHiddenButtonId,
              '#ajax' => array(
              'wrapper' => $wrapperId,
                'callback' => 'addEditAssessmentRepaintForm',
                'entity_id' => $entityId,
                'entity_type' => $entityType,
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
    }
    $form_state['storage']['mode'] = '';
  }
  } catch (Exception $ex) {
  	watchdog_exception('displayAssessmentDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function loadCtoolDataGridForAssessment($searchKeyword, $queryRequired = '', $excludedAssessmentId = '',$entityId, $entityType){
 try {
  // Select the table
  $select = db_select('slt_survey', 'survey');

  // Add fields to fetch
  $select->distinct();
  $select->addField('survey', 'id', 'id');
  $select->addField('survey', 'code', 'code');
  $select->addField('survey', 'title', 'title');

  // Add condition
  $select->condition('survey.status', 'sry_det_sry_atv');
  if(!empty($searchKeyword) && $searchKeyword!='' && $searchKeyword!=t('LBL758')){
    $select->condition('survey.title', '%' . db_like($searchKeyword) . '%', 'LIKE');
  }
  $select->condition('survey.type', 'sry_det_typ_ass', '=');
  if($entityType == 'cre_sys_obt_cls'){
  if (!empty($excludedAssessmentId)) {
    $excludedAssessmentId = urldecode($excludedAssessmentId);
    $excludedAssessmentIdArr = explode(',', $excludedAssessmentId);
  	$select->condition('survey.id', $excludedAssessmentIdArr, 'NOT IN');
  }
  }else{
  		$selectQuery = db_select('slt_survey_mapping', 'sur_map');
	  	$selectQuery->leftjoin('slt_survey', 'sur', 'sur_map.survey_id = sur.id');
	  	$selectQuery->leftjoin('slt_module', 'mdl', 'mdl.id = sur_map.module_id');
		$selectQuery->addfield('sur_map', 'survey_id', 'sur_id');
		$selectQuery->addExpression('count(mdl.id)','module_list');
		$selectQuery->condition('sur.type', 'sry_det_typ_ass');
		$selectQuery->condition('sur_map.object_id', $entityId);
		$selectQuery->condition('sur_map.object_type', $entityType);
		$selectQuery->condition('sur.status', 'sry_det_sry_atv');
		$selectQuery->groupBy("sur_map.object_id,sur_map.object_type,sur_map.survey_id HAVING module_list = (SELECT COUNT(1) FROM slt_module module WHERE module.program_id = ".$entityId.")");
	  	expDebug::dPrintDBAPI("loadCtoolDataGridForSurvey select query mode ---> ",$selectQuery);
		$result = $selectQuery->execute()->fetchAll();
  	
	  	if(count($result) > 0){
	      foreach($result as $res){
	        $surveyIdArr[] = $res->sur_id;
	      }
	      $select->condition('survey.id', $surveyIdArr, 'NOT IN');
	    }
  }
  filterByUserAccessPrivileges($select,'survey','',1,'sry_det_typ_ass','attach','survey');  
  //$select->orderBy('survey.title');

  if(empty($queryRequired)){
    return $select->execute()->fetchAll();
  } else {
    return $select;
  }
  } catch (Exception $ex) {
  	watchdog_exception('loadCtoolDataGridForAssessment', $ex);
  	expertusErrorThrow($ex);
  }
}

function fetchAssessmentList($entityId, $entityType, $queryRequired = '',$modId = ''){
 try {
  $selectQuery = db_select('slt_survey_mapping', 'sur_map');
  $selectQuery->leftjoin('slt_survey', 'sur', 'sur_map.survey_id = sur.id');
  $selectQuery->leftjoin('slt_module', 'mdl', 'mdl.id = sur_map.module_id');
  $selectQuery->addfield('sur_map', 'id', 'id');
  $selectQuery->addfield('sur', 'id' , 'sur_id');
  $selectQuery->addfield('sur', 'title');
  $selectQuery->addfield('sur', 'code');
  $selectQuery->addfield('sur_map', 'no_of_attempts');
  $selectQuery->addfield('sur_map', 'pre_status');
  $selectQuery->addfield('sur_map', 'view_option');
  $selectQuery->addfield('sur_map', 'preview_option');
  $selectQuery->addfield('mdl', 'title','path');
  $selectQuery->addfield('mdl', 'id','module_id');
  $selectQuery->condition('sur.type', 'sry_det_typ_ass'); // sry_det_typ_sry, sry_det_typ_ass
  $selectQuery->condition('sur_map.object_id', $entityId);
  $selectQuery->condition('sur_map.object_type', $entityType);
  if(!empty($modId))
  	$selectQuery->condition('sur_map.module_id', $modId);
  $selectQuery->condition('sur.status', 'sry_det_sry_atv');
  //$selectQuery->orderBy('sur_map.created_on', 'asc');
  expDebug::dPrintDBAPI('assessment list ',$selectQuery);
  if(empty($queryRequired)){
    return $selectQuery->execute()->fetchAll();
  } else {
    return $selectQuery;
  }
  } catch (Exception $ex) {
  	watchdog_exception('fetchAssessmentList', $ex);
  	expertusErrorThrow($ex);
  }
}

function cancelAssessmentDetails($form, &$form_state){
	try {
  $form_state['storage']['mode'] = '';
  $form_state['rebuild'] = TRUE;
  } catch (Exception $ex) {
  	watchdog_exception('cancelAssessmentDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function assessmentGrid($form, &$form_state) {
	try {
  $formName = $form_state['form_unique_name'];
  return $form[$formName];
  } catch (Exception $ex) {
  	watchdog_exception('assessmentGrid', $ex);
  	expertusErrorThrow($ex);
  }
}

function add_assessment($form, &$form_state) {
	try {
  $form_state['storage']['mode'] = 'edit';
  $form_state['rebuild'] = TRUE;
  } catch (Exception $ex) {
  	watchdog_exception('add_assessment', $ex);
  	expertusErrorThrow($ex);
  }
}

function deleteAssessmentDetails($entityId, $entityType, $assessmentIdList) {
	try {
		
		if($entityType == 'cre_sys_obt_cls')
			$upd = db_update('slt_course_class');
		else
			$upd = db_update('slt_program');
		$fields = array('updated_on'=> now());
		$upd->fields($fields);
		$upd->condition('id',$entityId,'=');
		expDebug::dPrintDBAPI("Update entity sql >>>>>>>>>." , $upd);
		$upd->execute();
  $select = db_select('slt_survey_mapping', 'sur');
  $select->addField('sur','id','id');
  $select->addField('sur','survey_id','survey_id');
  $select->addField('sur','object_id','object_id');
  $select->addField('sur','object_type','object_type');
  $select->addField('sur','module_id','module_id');
  $select->condition('sur.id', $assessmentIdList, '=');
  $delId = $select->execute()->fetchAll();
  $assessmentIdArr = explode('|', $assessmentIdList);
  
  $loggeduserId = getSltpersonUserId();
  
  $txn = db_transaction();
  try {

    if(count($assessmentIdArr) > 0){
        $deleteStmt = db_delete('slt_survey_mapping');
        $deleteStmt->condition('id', $assessmentIdArr,'IN');
        $numDeleted = $deleteStmt->execute();
        
        foreach ($delId as $val){
            if($val->module_id == null){
                $mod_id = 0;
                $type = 'class';
            }
            else{
                $mod_id = $val->module_id;
                $type = 'module_id';
            }
            // Make deleted log entry
            $fields = array(
                    'table_name' => 'slt_survey_mapping',
                    'entity_id' => $val->id,
                    'entity_type' => 'assessment_mapping',
                    'parent1_entity_id' => $val->survey_id,
                    'parent1_entity_type' => 'survey_id',
                    'parent2_entity_id' => $val->object_id,
                    'parent2_entity_type' => $val->object_type,
                    'parent3_entity_id' => $mod_id,
                    'parent3_entity_type' => $type,
                    'module_name' => 'exp_sp_administration_assessment',
                    'functionality_name' => 'deleteAssessmentDetails',
                    'api_name' => $_REQUEST['apiname'],
                    'deleted_on' => now(),
                    'deleted_by' => $loggeduserId
            );
            deleted_log_entry($fields);
        }
        
    }
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('deleteassessment', $ex);
    throw $ex;
  }

  unset($txn);
  return $delId->survey_id;
  } catch (Exception $ex) {
  	watchdog_exception('deleteAssessmentDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function addNewAssessmentDetails($form, &$form_state,$entityId,$entityType) {
	global $secure_entities;
  $txn = db_transaction();
  try {
      //$entityId = $form_state['triggering_element']['#ajax']['entity_id'];
      //$entityType = $form_state['triggering_element']['#ajax']['entity_type'];
		$entityId = empty($entityId) ? $form_state['entity_id'] : $entityId;
		$entityType = empty($entityType) ? $form_state['entity_type'] : $entityType ;
		//$module_id = empty($module_id) ? $form_state['module_id'] : $module_id;
		 $uniqueId = "assessment-".$entityId."-$entityType";
		if($entityType != "cre_sys_obt_cls"){
			 if($entityType == "cre_sys_obt_crt")
				$module_id = $form_state['input']['hidden_certificate_path_'.$uniqueId];
			 else{
			 	$module = getModuleIdFromObjectId($entityId,1);
			 	$module_id = $module[0]->module_id;
			 }
		 }		
		 
		 if($entityType == 'cre_sys_obt_cls')
		 	$upd = db_update('slt_course_class');
		 else
		 	$upd = db_update('slt_program');
		 $fields = array('updated_on'=> now() , 'updated_by'=> $userid);
		 $upd->fields($fields);
		 $upd->condition('id',$entityId,'=');
		 expDebug::dPrintDBAPI("Update entity sql >>>>>>>>>." , $upd);
		 $upd->execute();
		 
      if (in_array($entityType, $secure_entities)) {
      	$uniqueId = "assessment-".core_encrypt($entityId)."-$entityType";  // URL Encryption to reretive data
      } else {
      	$uniqueId = "assessment-".$entityId."-$entityType";  // URL Encryption to reretive data
      }
      if(!empty($form_state['input']['hidden_idlist_'.$uniqueId]) || !empty($form_state['hidden_idlist_'.$uniqueId])){
        $formName = $form_state['form_unique_name'];
        $userId = getSltpersonUserId();
        if(!empty($form_state['input']['hidden_idlist_'.$uniqueId]))
          $idListArray = explode(',', $form_state['input']['hidden_idlist_'.$uniqueId]);
        else
          $idListArray = explode(',', $form_state['hidden_idlist_'.$uniqueId]);
        $idListArray = array_unique($idListArray);
        $viewOption = '';
        $viewOptionList = array();

        foreach($idListArray as $assessmentIdList){
          $viewOptionList = explode("-",$assessmentIdList);
          $insertStmt = db_insert('slt_survey_mapping');
          $custom = 'null';
          $assessmentId = $viewOptionList[0];

          $select = db_select('slt_survey_mapping', 'sur');
          $select->addExpression('COUNT(sur.id)','map_count');
          $select->addField('sur','pre_status','pre_status');
          $select->addField('sur','preview_option','preview_option');
          $select->condition('sur.object_id', $entityId, '=');
          $select->condition('sur.object_type', $entityType, '=');
          if(!empty($module_id))
          	$select->condition('sur.module_id', $module_id, '=');
          $select->condition('sur.no_of_attempts','0', '>');
          //$select->condition('sur.survey_id', $assessmentId, '=');
          $getCount = $select->execute()->fetchAssoc();
          if($getCount['map_count'] < 1){
          	$preStatus = 0;
          }elseif($getCount['map_count'] >=1 && $getCount['pre_status'] == 1){
          	$preStatus = 0;
          }elseif($getCount['map_count'] >=1 && $getCount['pre_status'] == 0){
          	$preStatus = 1;
          }
          //by default preview option disabled.
          $previewOption = 0;
          $attempt = $form_state['input']['assessment-maxattempt-'.$assessmentId];
          $attempt = empty($attempt) ? 1 : $attempt;
          $viewOption = empty($viewOptionList[1]) ? 'V' : $viewOptionList[1];
         

          $fields = array(
                    'object_id'          => $entityId,
                    'object_type'        => $entityType,
          			'module_id'			=> $module_id,
                    'survey_id'     	   => $assessmentId,
          			'no_of_attempts'     => $attempt,
                    'view_option'        => $viewOption,
          			'pre_status'        => $preStatus,
          			'preview_option'	  => $previewOption,
          			'after_completion'   => $custom,
                    'created_by'         => $userId,
                    'created_on'         => now(),
                    'updated_by'         => $userId,
                    'updated_on'         => now(),
          		  	'custom0'            => $custom,
                    'custom1'            => $custom,
                    'custom2'            => $custom,
                    'custom3'            => $custom,
                    'custom4'            => $custom
                  );
          $insertStmt->fields($fields);
          $attFormState = array();
          $attFormState['survey_id'] = $assessmentId;
          $attFormState['view_option'] =  $viewOption;
          $attFormState['after_completion'] = $custom;

          $assessmentId = $insertStmt->execute();
          make_audit_trail_entries($attFormState, getIdOfLoggedInUser() , null ,$entityId, 'Attached',$entityType,'exp_sp_administration_assessment','new assessment',
                                          array('survey_id', 'view_option', 'after_completion'));
          syncSolrData('Program');
          syncSolrData('Class');

        }

        $form_state['storage']['mode'] = 'view';
        $form_state['rebuild'] = TRUE;
        expertus_set_message(t('MSG601').'.');
      } /*else {
        drupal_set_message('Please select one or more items.','error');
        $form_state['storage']['mode'] = 'edit';
        $form_state['rebuild'] = true;
        //$form_state['storage']['refresh'] = 'norefresh';
      }*/
  }   catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('addNewAssessmentDetails', $ex);
    throw $ex;
  }

  unset($txn);

}


/*
 * getAssessmentAutoComplete()
 */
function getAssessmentAutoComplete() {
	try {
  $nameSubstr = $_GET['z'];
  $excludedAssessmentId = $_GET['excluded_assessment_id'];

  // Select the table
  $select = db_select('slt_survey', 'survey');

  // Add fields to fetch
  $select->distinct();
  $select->addField('survey', 'id', 'id');
  $select->addField('survey', 'code', 'code');
  $select->addField('survey', 'title', 'title');

  // Add condition
  $select->condition('survey.status', 'sry_det_sry_atv');
  $select->condition('survey.type', 'sry_det_typ_ass', '=');
  $select->condition(db_or()
  	                     ->condition('survey.title', '%' . db_like($nameSubstr) . '%', 'LIKE')
  	                     ->condition('survey.code', '%' . db_like($nameSubstr) . '%', 'LIKE')

  	                  );

  if (!empty($excludedAssessmentId)) {
    $excludedAssessmentId = urldecode($excludedAssessmentId);
    $excludedAssessmentIdArr = explode(',', $excludedAssessmentId);
  	$select->condition('survey.id', $excludedAssessmentIdArr, 'NOT IN');
  }
  filterByUserAccessPrivileges($select,'survey','',1,'sry_det_typ_ass','attach','survey');  
  $select->orderBy('survey.title');
  $matchingAssessments = $select->execute()->fetchAll();

  foreach ($matchingAssessments as $assessment) {
    print $assessment->title. "\n";
  }
  print '';
  } catch (Exception $ex) {
  	watchdog_exception('getAssessmentAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}
/*
 * addEditAssessmentRepaintForm() - After the form has been validated and saved, this function returns the form
 *                            to the client for re-rendering.
 */
function addEditAssessmentRepaintForm($form, &$form_state) {
	try {
		global $secure_entities;
  $formName = $form_state['form_unique_name'];
  $entityId = !empty($form_state['triggering_element']['#ajax']['entity_id']) ? 
  			$form_state['triggering_element']['#ajax']['entity_id'] : $form_state['entity_id'] ;
  $entityType = !empty($form_state['triggering_element']['#ajax']['entity_type']) ? 
  				$form_state['triggering_element']['#ajax']['entity_type'] : $form_state['entity_type'] ;

  if (in_array($entityType, $secure_entities)) {
  	$uniqueId = "assessment-".core_encrypt($entityId)."-$entityType"; // URL encryption
  } else {
  	$uniqueId = "assessment-".$entityId."-$entityType"; // URL encryption
  }
  $commands = array();
  if($form_state['hidden-save-submit'] && (!empty($form_state['input']['hidden_idlist_'.$uniqueId]) || !empty($form_state['hidden_idlist_'.$uniqueId]))){
    $commands[] = ajax_command_replace(NULL, drupal_render($form[$formName]));
  }
  $commands[] = exp_ctools_modal_adjust();

  $statusMsgHTML = theme('status_messages');
  expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true) , 4);
  if (!empty($statusMsgHTML)) {
    // Show the messages
  if($form_state['input']['mapped_program_id']){
  	  $commands[] = ajax_command_html('#show_expertus_message', $statusMsgHTML);
  	}
  	else{
    	$commands[] = ajax_command_html('#bubble-face-table #show_expertus_message', $statusMsgHTML);
  	}
  }

  if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
    $custButtonId  = $form_state['triggering_element']['#ajax']['customButtonId'];
    $esignObj  = '{"popupDiv":"ctools-face-table","esignFor":"AddAdmin","esignButtId":"'.$custButtonId.'"}';
    if($form_state['esign_show_popup']==true){
      $commands[] = ajax_command_invoke(NULL, 'getNewEsignPopup',array($esignObj));
      $form_state['esign_show_popup']=false;
    }
  }
  $form_state['storage']['mode'] = 'edit';
  return array('#type' => 'ajax', '#commands' => $commands);
  } catch (Exception $ex) {
  	watchdog_exception('addEditAssessmentRepaintForm', $ex);
  	expertusErrorThrow($ex);
  }
}
/*
 * function for saving attempts in inline edit
 */
function assessmentSaveEditOption($details, $value){
try{
	list($id, $type, $entityId, $entityType, $index) = explode('-', $details);
	expDebug::dPrint('Type: ' . $type);
	expDebug::dPrint('Index: ' . $index);
	$default= db_query("SELECT no_of_attempts,pre_status,preview_option,view_option from slt_survey_mapping where id=:uid", array (":uid"=>$id))->fetchAll();
	$fieldsArray = array();	
	$result = new stdClass();
	$fieldsArray['updated_by'] = getSltpersonUserId();
	$fieldsArray['updated_on'] = now();
	if($type == 'survey'){
		if($index == Horizontal){
			$fieldsArray['view_option'] = 'H';
			$result->view_option = 'H';
		}else{
			$fieldsArray['view_option'] = 'V';
			$result->view_option = 'V';
		}
		$updateQuery = db_update('slt_survey_mapping');
		$updateQuery->fields($fieldsArray);
		$updateQuery->condition('id', $id);
		expDebug::dPrintDBAPI('update query view option ', $updateQuery);
		$updateQuery->execute();
		make_audit_trail_entries($fieldsArray,getIdOfLoggedInUser(),$default,core_decrypt($entityId),'Updated',$entityType,'exp_sp_administration_assessment','view_option', array('view_option'));
	}
	if($index == 'no_of_attempts'){
				if($value < 1 || $value == ''){
					$fieldsArray['no_of_attempts'] = 1;
					$result->no_of_attempts = 1;
				}else{
					$fieldsArray['no_of_attempts'] = $value;
					$result->no_of_attempts = $value;
				}
				$updateQuery = db_update('slt_survey_mapping');
				$updateQuery->fields($fieldsArray);
				$updateQuery->condition('id', $id);
				$updateQuery->execute();
				make_audit_trail_entries($fieldsArray,getIdOfLoggedInUser(),$default,core_decrypt($entityId),'Updated',$entityType,'exp_sp_administration_assessment','Attempts', array('no_of_attempts'));	
	}elseif($index == 'pre_status'){
				//get module id 
				if($entityType != 'cre_sys_obt_cls'){
					$select = db_select('slt_survey_mapping', 'map');
					$select->addField('map', 'module_id', 'module_id');
					$select->condition('map.id', $id,'=');
					$moduleId = $select->execute()->fetchField();
				}
				
				$select = db_select('slt_survey_mapping', 'map');
				//$select->addExpression('GROUP_CONCAT(map.id)', 'map_id');
				$select->addField('map', 'id', 'id');
				$select->condition('map.id', $id,'!=');
				//54506: System allowing to set more than one pre-assessment for single class.
				$select->condition('map.object_id', core_decrypt($entityId),'=');
				$select->condition('map.object_type', $entityType,'=');
				if($entityType != 'cre_sys_obt_cls')
					$select->condition('map.module_id', $moduleId,'=');
				
				$select->condition('map.no_of_attempts','0','>');
				//expDebug::dPrintDBAPI('$select pre_status ', $select);
				$postAssessmentIds = $select->execute()->fetchField();
				//expDebug::dPrint('PRE ASSS $postAssessmentIds--->'.print_r($postAssessmentIds,1),4);
				if ($postAssessmentIds == ''){
				 // single assessment
				 $array = array($id=>$value);
				} else {
				 $array = ($value == 1) ? array($postAssessmentIds=>0,$id=>$value) : array($postAssessmentIds=>1,$id=>0);
				}
				//expDebug::dPrint('PRE ASSS--->'.print_r($array,1),4);
				foreach ($array as $key => $val){
						$updateQuery = db_update('slt_survey_mapping');
						$fieldsArray['pre_status'] = $val;
						$result->pre_status = $val;
						$updateQuery->fields($fieldsArray);
						$updateQuery->condition('id', $key, '=');
						$updateQuery->execute();
						make_audit_trail_entries($fieldsArray,getIdOfLoggedInUser(),$default,core_decrypt($entityId),'Updated',$entityType,'exp_sp_administration_assessment','pre_assessment', array('pre_status'));				
					}
	}elseif($index == 'preview_option'){
				$updateQuery = db_update('slt_survey_mapping');
				$fieldsArray['preview_option'] = $value;
				$result->preview_option = $value;
				$updateQuery->fields($fieldsArray);
				$updateQuery->condition('id', $id, '=');
				$updateQuery->execute();
				make_audit_trail_entries($fieldsArray,getIdOfLoggedInUser(),$default,core_decrypt($entityId),'Updated',$entityType,'exp_sp_administration_assessment','preview_option', array('preview_option'));
				expDebug::dPrintDBAPI('update query preview_option ', $updateQuery);
	}elseif ($type == 'assessment'){
			if($index == 'Horizontal' || $index == 'Vertical'){
			if($index == Horizontal){
				$fieldsArray['view_option'] = 'H';
				$result->view_option = 'H';
			}else{
				$fieldsArray['view_option'] = 'V';
				$result->view_option = 'V';
			}
			}
			$updateQuery = db_update('slt_survey_mapping');
			$updateQuery->fields($fieldsArray);
			$updateQuery->condition('id', $id);
			$updateQuery->execute();
			make_audit_trail_entries($fieldsArray,getIdOfLoggedInUser(),$default,core_decrypt($entityId),'Updated',$entityType,'exp_sp_administration_assessment','view_option', array('view_option'));
	} 

	return $result;
}
catch (Exception $ex) {
 watchdog_exception('assessmentSaveEditOption', $ex);
 expertusErrorThrow($ex);
 }
}


?>