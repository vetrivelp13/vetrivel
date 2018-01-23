<?php

function displayEquivalenceDetails(&$form, &$form_state, $entityId, $entityType, $uniqueId, $mode=""){
	try {
	global $secure_entities;		
  $equivalenceResult = fetchEquivalenceList($entityId, $entityType);
  if (in_array($entityType, $secure_entities)) {
  	$entityId_encrypted = core_encrypt($entityId); // URL encryption
  } else {
  	$entityId_encrypted = $entityId;
  }
  $uniqueId = "equivalence-$entityId_encrypted-$entityType";
  $count = count($equivalenceResult);
  
  $customHiddenButtonId = 'equv-save-esign';
  $wrapperId = 'equivalence-grid-wrapper-'.$uniqueId;
  $formName = 'equivalence_form_'.$uniqueId;
  $form_state['form_unique_name'] = $formName;
  
  $msg = t('MSG304');
  
  $form['attr_action'] = array(
    '#type' => 'hidden',
    '#name' => 'attr_action',
    '#value' => 'equivalence',
  );  
  
  $setminHight = ($entityType=='cre_sys_obt_trp') ? 'admin-training-program-tab-datagrid-wrapper' : 'admin-course-tab-datagrid-wrapper';
  
  $form[$formName] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="admin-data-grid"><div id="'.$wrapperId.'" class="admin-datagrid-pagination equv-grid-wrapper '.$setminHight.'">',
    '#suffix' => '</div></div></div>',
  );  

  $form[$formName]['hidden_idlist'] = array(
    '#type' => 'hidden',
    '#name' => 'hidden_idlist_'.$uniqueId,
    '#id' => 'datagrid-idlist-'.$uniqueId,
  );  
  
  $displayMode = $form_state['storage']['mode'];
  
  if($count == 0 && empty($displayMode)){
    
    $addAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
    $addAttributes['data-wrapperid'] = array($wrapperId);
    if((isset($form_state['storage']['display_mode']) && $form_state['storage']['display_mode'] == 'view') || (isset($form_state['storage']['step']) && $form_state['storage']['step'] == 'view')){
      $form[$formName]['add_prereqiuisite_text'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="no-records-msg admin-datagrid-add-btn-wrapper-container" id="expertus-no-online-users-msg-'.$uniqueId.'">'.t('MSG343').'</div>',
      );
    } else {

      $equivalenceDetails = fetchEquivalenceList($entityId, $entityType);
  	  $courseIdArr = array();
      $excludedCourseId = '""';
      if(count($equivalenceDetails) > 0){
        foreach($equivalenceDetails as $details){
          $courseIdArr[] = $details->associated_object_id;
        }
      }
      $prerequisiteDetails = fetchPrerequisiteList($entityId, $entityType);
      $excludedCourseId = '""';
      if(count($prerequisiteDetails) > 0){
        foreach($prerequisiteDetails as $details){
          $courseIdArr[] = $details->associated_object_id;
        }
      }
      
      $excludedCourseId = implode(',', $courseIdArr);
      $excludedCourseId = $excludedCourseId==""?$entityId:$excludedCourseId.",".$entityId;
      
      $equivalenceAvailableDetails = loadCtoolDataGridForEquivalence('', '', $excludedCourseId, $entityType);
      //$priv_visible = adminVisibilityForAssign('cre_sys_obt_crs');     
      if(count($equivalenceAvailableDetails) == 0   ){  // || empty($priv_visible)     
        $form[$formName]['add_equivalance'] = array(
          '#type' => 'markup',
          '#prefix' => '<div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-greyout-bg"></div>',
          '#suffix' => '<div class="admin-save-button-middle-greyout-bg">'.t('LBL303').'</div><div class="admin-save-button-right-greyout-bg"></div></span></div>',
        );
    
        $form[$formName]['add_equivalance_text'] = array(
          '#type' => 'markup',
          '#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.$msg.'</div></div>',
        );
        
      } else {
      
        $form[$formName]['add_equivalance'] = array(
          '#type' => 'submit',
          '#prefix' => '<div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-bg"></div>',
          '#suffix' => '<div class="admin-save-button-right-bg"></div></span></div>',
          '#title' => t('LBL303'),
          '#name' => 'Add equivalence',
          '#value' => t('LBL303'),
          '#submit' => array('add_equivalence'),
          '#attributes' => $addAttributes,
          '#ajax' => array(
            'callback' => 'equivalenceGrid',
            'wrapper' => $wrapperId,
          ),  
        );
    
        $form[$formName]['add_equivalance_text'] = array(
          '#type' => 'markup',
          '#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.$msg.'</div></div>',
        );
      
      }
      
    }
      
  } else {
    
    $equivalenceDetails = fetchEquivalenceList($entityId, $entityType);
	$courseIdArr = array();
    $excludedCourseId = '""';
    if(count($equivalenceDetails) > 0){
      foreach($equivalenceDetails as $details){
        $courseIdArr[] = $details->associated_object_id;
      }
    }
    $prerequisiteDetails = fetchPrerequisiteList($entityId, $entityType);
    $excludedCourseId = '""';
    if(count($prerequisiteDetails) > 0){
      foreach($prerequisiteDetails as $details){
        $courseIdArr[] = $details->associated_object_id;
      }
    }
    $excludedCourseId = implode(',', $courseIdArr);
    $excludedCourseId = $excludedCourseId==""?$entityId:$excludedCourseId.",".$entityId;

    if($displayMode == 'edit'){
      
      $form[$formName][] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_search_container">',
      );
      
      $searchAjaxEvent = 'click';
      $searchAttributes['onClick'][] = '$("body").data("mulitselectdatagrid").searchDataGrid("'.$displayMode.'", "equivalence", $(\'#equivalence-autocomplete\').val(), "'.$entityId_encrypted.'", "'.$entityType.'", "'.$excludedCourseId.'"); return false;';
    
      $acMenuPath = 'administration/learning/courses/allcourses-autocomplete';
      $paintMultiAction  = "<div id='search-list-title-keyword' class='search-list-keyword' style='display:block;'>";
      $paintMultiAction .= "<span id='search-dropdwn-list'>";
      $paintMultiAction .= "<input type='hidden' id='search_all_equivalence_type-hidden' value='name' />";
      $paintMultiAction .= "<input type='hidden' id='equivalence-autocomplete_hidden' value='".t('LBL702')."' />";
      $paintMultiAction .= "<span id='select-list-dropdown' class='select-list-dropdownContent'>".t('LBL107')."</span>";
      $paintMultiAction .= "<a id='admin-dropdown-arrow' class='select-list-dropdown-link dropdown-link-font ' onclick='$(\"#select-list-dropdown-list\").slideToggle();$(\"#select-list-dropdown-list li:last\").css(\"border-bottom\",\"0px none\");'>&nbsp;</a>";
      $paintMultiAction .= "<ul id='select-list-dropdown-list'>";
      $paintMultiAction .= "<li id='name-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEqlCourseSearchTypeText('name','Course');\">".t('LBL107')."</li>";
      $paintMultiAction .= "<li id='tags-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEqlCourseSearchTypeText('tags','Course');\">".t('LBL191')."</li>";
      $paintMultiAction .= "</ul>";
      $paintMultiAction .= "</span></div>";
       
      $form[$formName]['more_add_moveusers_search'] = array(
      		'#type' => 'markup',
      		'#markup' => $paintMultiAction,
      );
      
      $form[$formName]['add_equivalance_search'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_auto_search">',
      );
      
      $autoCompleteAttributes['class'][] = 'admin_ac_input_mainform';
      $autoCompleteAttributes['onkeydown'][]  = '$("body").data("mulitselectdatagrid").multiSelectSearchBoxKeyDown(event)';
      addACFieldForMultiSelect($form[$formName], 'equivalence-autocomplete', $acMenuPath, '', '', '', array(), array('excluded_course_id' => "'".$excludedCourseId."'", 'id_required' => 0,'search_type' => "$('#search_all_equivalence_type-hidden').val()"), false, false, t('LBL702'),$autoCompleteAttributes);

      $form[$formName]['add_equivalance_search1'] = array(
      '#type' => 'markup',
      '#markup' => '</div>',
      );
      
      $searchAttributes['Class'][] = 'admin-pagination-search-go';
       $form[$formName]['search_equivalance'] = array(
        '#type' => 'submit',
        '#title' => t('LBL304'),
        '#name' => 'Search equivalence',
        '#submit' => array('search_equivalence'),
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
    
    $form[$formName]['equivalance_view_grid_markup'] = array(
      '#type' => 'markup',
      '#markup' => '<div id="datagrid-div-'.$uniqueId.'"><table id="datagrid-container-'.$uniqueId.'" class="datagrid-container-common"></table><div id="pager-datagrid-'.$uniqueId.'" class="pager-datagrid-common"></div></div>',
    );
    $displayModeDataGrid = $form_state['storage']['display_mode'] == 'view' || $form_state['storage']['step'] == 'view' ? 'view_only' : $displayMode;
    $form[$formName]['equivalance_view_grid_load'] = array( 
      '#type' => 'markup',
      '#prefix' => '<div style="display:none"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-'.$uniqueId.'\'); $(\'body\').data(\'mulitselectdatagrid\').loadDataGrid(\''.$displayModeDataGrid.'\', \'equivalence\', \'\', \''.$entityId_encrypted.'\', \''.$entityType.'\', \''.$excludedCourseId.'\');" width="100" height="132" /></div>',
    );       
    
    if($displayMode == 'view'){
      
      if($mode != 'complete_view'){
        $addAnotherAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
        $addAnotherAttributes['data-wrapperid'] = array($wrapperId);
        
        $listquery = loadCtoolDataGridForEquivalence('', '', $excludedCourseId);
        
        if($form_state['storage']['display_mode'] != 'view' && count($listquery) != 0){
          $form[$formName]['add_equivalance_view_mode'] = array(
            '#type' => 'submit',
            '#title' => t('LBL303'),
      	  '#prefix' => '<div id= "datagrid-add-search-button-'.$uniqueId.'"  style="display:block;" class="admin-save-button-container admin-save-button-container-view-mode"><div class="admin-save-button-left-bg"></div>',
  	      '#suffix' => '<div class="admin-save-button-right-bg"></div></div>',
  	      '#value' => t('LBL303'),
            '#name' => 'Add equivalence',
            '#submit' => array('add_equivalence'),
            '#attributes' => $addAnotherAttributes,
            '#ajax' => array(
              'callback' => 'equivalenceGrid',
              'wrapper' => $wrapperId,
            ),  
          );
          $form[$formName]['add_equivalance_view_mode1'] = array(
            '#type' => 'submit',
            '#title' => t('LBL303'),
      	  '#prefix' => '<div style="display:none;"  id="datagrid-noresult-msg-'.$uniqueId.'"><div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-bg"></div>',
  	      '#suffix' => '<div class="admin-save-button-right-bg"></div></span></div>',
  	      '#value' => t('LBL303'),
            '#name' => 'Add equivalence',
            '#submit' => array('add_equivalence'),
            '#attributes' => $addAnotherAttributes,
            '#ajax' => array(
              'callback' => 'equivalenceGrid',
              'wrapper' => $wrapperId,
            ),  
          );        
          
          $form[$formName]['equivalance_view_grid_no_msg'] = array(
            '#type' => 'markup',
            '#markup' => '<div class="admin-empty-text-msg"> '.$msg.'</div></div></div>',
          );
        } else {
          $form[$formName]['add_equivalance_view_mode1'] = array(
            '#type' => 'submit',
            '#title' => t('LBL303'),
      	  '#prefix' => '<div style="display:none;"  id="datagrid-noresult-msg-'.$uniqueId.'"><div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-bg"></div>',
  	      '#suffix' => '<div class="admin-save-button-right-bg"></div></span></div>',
  	      '#value' => t('LBL303'),
            '#name' => 'Add equivalence',
            '#submit' => array('add_equivalence'),
            '#attributes' => $addAnotherAttributes,
            '#ajax' => array(
              'callback' => 'equivalenceGrid',
              'wrapper' => $wrapperId,
            ),  
          );        
          
          $form[$formName]['equivalance_view_grid_no_msg'] = array(
            '#type' => 'markup',
            '#markup' => '<div class="admin-empty-text-msg"> '.$msg.'</div></div></div>',
          );          
        }
      }  
    } else if($displayMode == 'edit'){

        $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-action-button-middle-bg white-btn-bg-middle';
        $cancelAttributes['data-wrapperid'] = array($wrapperId);
      
        $form[$formName]['cancel_equivalance_edit_mode'] = array(
          '#type' => 'submit',
          '#value' => t('LBL109'),
          '#title' => t('LBL109'),
          '#prefix' => '<div class="admin-datagrid-save-cancel-button-align-bottom"><div class="addedit-form-cancel-container-actions admin-save-button-container resize-save_btn3"><div class="white-btn-bg-left"></div>',
          '#suffix' => '<div class="white-btn-bg-right"></div>',
          '#name' => 'Cancel equivalence',
          '#submit' => array('cancelEquivalenceDetails'),
          '#attributes' => $cancelAttributes,
          '#ajax' => array(
            'callback' => 'equivalenceGrid',
            'wrapper' => $wrapperId,
          ),  
        );

        $listquery = loadCtoolDataGridForEquivalence('', '', $excludedCourseId);
        
        if(count($listquery) != 0){
            /*$setAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
            $setAttributes['data-wrapperid'] = array($wrapperId);
    
            $form[$formName]['set_equivalance_edit_mode'] = array(
              '#type' => 'submit',
              '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
    	      '#suffix' => '<div class="admin-save-button-right-bg"></div></div></div>',
              '#title' => t('LBL305'),
              '#value' => t('LBL306'),
              '#name' => 'Set equivalence',
              '#submit' => array('addNewEquivalenceDetails'),
              '#attributes' => $setAttributes,
              '#ajax' => array(
                'callback' => 'equivalenceGrid',
                'wrapper' => $wrapperId,
                'entity_id' => $entityId, 
                'entity_type' => $entityType,
              ),  
            );*/
            if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
              $buttonName  = 'esign-equivalence'; 
            }else{
              $buttonName  = 'set-equivalence';
            }
            
            $setAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
            $setAttributes['data-wrapperid'] = array($wrapperId);
            $form[$formName]['set_equivalence_edit_mode'] = array(
              '#type' => 'submit',
              '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
    	      '#suffix' => '<div class="admin-save-button-right-bg"></div></div></div></div>',
              '#title' => t('LBL305'),
              '#value' => t('LBL306'),
              '#name' => $buttonName,
              '#attributes' => $setAttributes,
              '#ajax' => array(
        		    'wrapper' => $wrapperId,
        		    'callback' => 'addEditRepaintEquvForm',
                    'entity_id' => $entityId, 
                    'entity_type' => $entityType,
                    'entity_plugin'=>'equivalence',
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
    }
    $form_state['storage']['mode'] = '';
  }
  $saveAttributes= array();
  $form[$customHiddenButtonId] = array(
    '#type' => 'submit',
    '#value' => t('hidden-save'),
    '#name'	=> 'hidden-save',
    '#prefix' => '<span id = "esign-admin-div" style="display:none;">',
    '#suffix' => '</span>',
    '#attributes' => $saveAttributes,
    '#id'=>$customHiddenButtonId,
    '#ajax' => array(
    'wrapper' => $wrapperId,
      'callback' => 'addEditRepaintEquvForm',
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
  } catch (Exception $ex) {
  	watchdog_exception('displayEquivalenceDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function loadCtoolDataGridForEquivalence($searchKeyword, $queryRequired = '', $excludedId = '',$searchType=''){
	try {
	expDebug::dPrint('$searchType :'.$searchType,5);
  $select = db_select('slt_course_template', 'course');
  $select->addField('course', 'id', 'id');
  $select->addField('course', 'title', 'title');
  $select->addField('course', 'code', 'code');
  if($searchType != 'tags'){
	  if(!empty($searchKeyword) && $searchKeyword!='' && $searchKeyword!=t('LBL702')){
	    $select->condition('course.title', '%' . db_like($searchKeyword) . '%', 'LIKE');
	  }
  }else if($searchType == 'tags'){
  	if(!empty($searchKeyword) && $searchKeyword!='' && $searchKeyword!= t('LBL193')){
  		$select->leftJoin('slt_tag_entity', 'en', 'en.entity_id = course.id');
  		$select->leftJoin('slt_tagdefn', 'def', 'def.id = en.tagid');
  		$select->condition('def.tagname', '%' . db_like($searchKeyword) . '%', 'LIKE');
  		$select->condition('en.entity_type', 'Course','=');
  	}
  }
  if(!empty($excludedId)){
    $select->condition('course.id', explode(',', $excludedId) , 'NOT IN');
  }
  $select->condition('course.status', 'lrn_crs_sts_atv','=');
  filterByUserAccessPrivileges($select,'course','',1,'cre_sys_obt_crs','attach','course');  
  $select->groupBy('course.id');
  //$select->orderBy('title');
  if(empty($queryRequired)){
    return $select->execute()->fetchAll();
  } else {
    return $select;
  }
  } catch (Exception $ex) {
  	watchdog_exception('loadCtoolDataGridForEquivalence', $ex);
  	expertusErrorThrow($ex);
  }
}

function fetchEquivalenceList($entityId, $entityType, $queryRequired = ''){
	try {
  $select = db_select('slt_common_mapping','map');
  $select->Join('slt_course_template', 'crs', 'crs.id=map.id2');
  $select->addField('map','id', 'id');
  $select->addField('map','id1', 'object_id');
  $select->addField('map','id2', 'associated_object_id');
  $select->addField('crs','title', 'title');
  $select->addField('crs','code', 'code');
  $select->condition('map.id1', $entityId,'=');
  $select->condition('map.object_type', $entityType,'=');
  $select->condition('map.type', 6,'=');
  if(empty($queryRequired)){
    return $select->execute()->fetchAll();
  } else {
    return $select;
  }
  } catch (Exception $ex) {
  	watchdog_exception('fetchEquivalenceList', $ex);
  	expertusErrorThrow($ex);
  }
}

function cancelEquivalenceDetails($form, &$form_state){
	try {
  $form_state['storage']['mode'] = '';
  $form_state['rebuild'] = TRUE;
  } catch (Exception $ex) {
  	watchdog_exception('cancelEquivalenceDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function equivalenceGrid($form, &$form_state) {
	try {
  $formName = $form_state['form_unique_name'];
  return $form[$formName];
  } catch (Exception $ex) {
  	watchdog_exception('equivalenceGrid', $ex);
  	expertusErrorThrow($ex);
  }
}

function add_equivalence($form, &$form_state) {
	try {
  $form_state['storage']['mode'] = 'edit';
  $form_state['rebuild'] = TRUE;
  } catch (Exception $ex) {
  	watchdog_exception('add_equivalence', $ex);
  	expertusErrorThrow($ex);
  }
}


function deleteEquivalenceDetails($entityId, $entityType, $equivalenceIdList,$callFromAPI = 0) {
	try {
	    
  $loggedInUserId  = getIdOfLoggedInUser();
  
  $select=db_select('slt_common_mapping','common');
  $select->addField('common','id','id');
  $select->addField('common','id1','p2');
  $select->addField('common','id2','p1');
  $select->addField('common','object_type','p2type');
  $select->addField('common','prereq_type','p1type');
  $equivalenceIdArr = explode('|', $equivalenceIdList);
  expDebug::dPrint('$equivalenceIdArr-->'.print_r($equivalenceIdArr,1),4);
  //$select->condition('id', $equivalenceIdArr,'IN');
  $select->condition('id', $equivalenceIdList, '=');
  expDebug::dPrintDBAPI('$select-------',$select);
  $select1 = $select->execute()->fetchAll();
  expDebug::dPrint('$select1-->'.print_r($select1,1),4);
  
  $txn = db_transaction();
  try {
   
    if(count($equivalenceIdArr) > 0){
     
        $deleteStmt = db_delete('slt_common_mapping');
        $deleteStmt->condition('id', $equivalenceIdArr,'IN');
        $numDeleted = $deleteStmt->execute();
        
        expDebug::dPrint("numDeleted-->".print_r($numDeleted,1),4);
        expDebug::dPrint("result of ids-->".print_r($equivalenceIdArr,1),4);

        foreach($select1 as $val){
            
            $fields= array(
                    'table_name' => 'slt_common_mapping',
                    'entity_id' => $val->id,
                    'entity_type' => 'equivalence_mapping',
                    'parent1_entity_id' => $val->p1,
                    'parent1_entity_type' => $val->p1type,
                    'parent2_entity_id' => $val->p2,
                    'parent2_entity_type' => $val->p2type,
                    'module_name' => 'exp_sp_administration_equivalence',
                    'functionality_name' => 'deleteEquivalenceDetails',
                    'api_name' => $_REQUEST['apiname'],
                    'deleted_on' => now(),
                    'deleted_by' => $loggedInUserId,
                    
            );
            deleted_log_entry($fields);
        }
	        
        if($callFromAPI && $numDeleted){
          return array((object)array('status'=>'Success'));
        }
    }
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('deleteequivalence', $ex);
    if($callFromAPI){
      //return array((object)array('status'=>'Failiure'));
      $errobj=new stdClass();
      $errobj->isValidateError = 1;
      $errobj->errcode = 'L_012';
      $errobj->errormsg = "Equivalence Id is not valid";
      return $errobj;
    }
    throw $ex;
  }

  unset($txn);
  return $delId;
  } catch (Exception $ex) {
  	watchdog_exception('deleteEquivalenceDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function addNewEquivalenceDetails($form, &$form_state,$entityId,$entityType,$callFromApi = 0 ) {  
	global $secure_entities;
  $txn = db_transaction();
  
  try {
    //$entityId = $form_state['triggering_element']['#ajax']['entity_id'];
    //$entityType = $form_state['triggering_element']['#ajax']['entity_type'];
    if (in_array($entityType, $secure_entities)) {
    	$uniqueId = "equivalence-".core_encrypt($entityId)."-$entityType"; // URL encryption to retrive the data
    } else {
    	$uniqueId = "equivalence-$entityId-$entityType";
    }
    
    //if(!empty($form_state['input']['hidden_idlist_'.$uniqueId])){
    if(!empty($form_state['input']['hidden_idlist_'.$uniqueId]) || !empty($form_state['hidden_idlist_'.$uniqueId])  || $callFromApi){
      $userId = getSltpersonUserId();
        //$associateIdListArray = explode(',', $form_state['input']['hidden_idlist_'.$uniqueId]);
        if(!empty($form_state['input']['hidden_idlist_'.$uniqueId]))
          $associateIdListArray = explode(',', $form_state['input']['hidden_idlist_'.$uniqueId]);
        else
          $associateIdListArray = explode(',', $form_state['hidden_idlist_'.$uniqueId]);
    	$idArray = array();   
        if($callFromApi){
          $associateIdList = $form_state['EquivalenceId'];
          $associateIdListArray = explode(',', $associateIdList);
          $userId = $form_state['createuserid'];
        } 
        foreach($associateIdListArray as $associatedEntityId){
          $aExitCnt = isAssociatedCourseExits($entityId,$associatedEntityId,$entityType,'6');
          
          if($aExitCnt == 0) {
            $insertStmt = db_insert('slt_common_mapping');
            $custom  = NULL;
          	
            $fields = array(
                      'id1'                => $entityId,
                      'id2'                => $associatedEntityId,//1000,
            		  'type'               => 6,
                      'object_type'    	   => $entityType,
            		  'prereq_type'        => 'cre_sys_pre_crs',
                      'value_string'       => 'Pre-req',
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
            
            $equivalenceId = $insertStmt->execute();
            add_audit_trail_entry($userId, $entityId, $entityType , 'exp_sp_administration_equivalence', 'Equivalence', 'Added new equivalent', null, null , $associatedEntityId);
          if($callFromApi){
                 return array((object)array('id'=>$equivalenceId));
            }
            
          }
          
        }
        
          $form_state['storage']['mode'] = 'view';
          $form_state['rebuild'] = TRUE;
           expertus_set_message(t('MSG601').'.'); 
      
      } /*else {
        
        drupal_set_message(t('ERR106'),'error');
        $form_state['storage']['mode'] = 'edit';
        $form_state['rebuild'] = true;
        //$form_state['storage']['refresh'] = 'norefresh';

      }*/
  }   catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('addNewEquivalenceDetails', $ex);
    if($callFromApi){
      //return array((object)array('id'=>'Failiure'));
      $errobj=new stdClass();
      $errobj->isValidateError = 1;
      $errobj->errcode = 'L_012';
      $errobj->errormsg = "Equivalence Id is not valid";
      return $errobj;
    }
    throw $ex;    
  }
  
  unset($txn);

}

/*
 * addEditRepaintEquvForm() - callback function for save button to invoke the esign popup if esign module is enabled and validation is passed through
 *                     
 */
function addEditRepaintEquvForm($form, &$form_state) {
	try {
		global $secure_entities;		
  expDebug::dPrint(' addEditRepaintPreReqForm() called.');
  expDebug::dPrint(' ranjani ranjani addEditRepaintPreReqForm() $form_state[esign_show_popup].'.$form_state['esign_show_popup'],5);
  $form_state['storage']['mode'] = 'edit';
  
  $formName = $form_state['form_unique_name'];
  $entityId = $form_state['triggering_element']['#ajax']['entity_id'];
  $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
  
  if (in_array($entityType, $secure_entities)) {
  	$uniqueId = "equivalence-".core_encrypt($entityId)."-$entityType"; // URL encryption to retrive data
  } else {
  	$uniqueId = "equivalence-".$entityId."-$entityType"; // URL encryption to retrive data
  }
  $commands = array();
  if($form_state['hidden-save-submit'] && (!empty($form_state['input']['hidden_idlist_'.$uniqueId]) || !empty($form_state['hidden_idlist_'.$uniqueId]))){
    $commands[] = ajax_command_replace(NULL, drupal_render($form[$formName]));
  }
  
  //$commands[] = ajax_command_replace(NULL, drupal_render($form[$formName]));
  //$commands[] = ajax_command_prepend(NULL, theme('status_messages'));
  $commands[] = ajax_command_html('#show_expertus_message', theme('status_messages'));
  $commands[] = exp_ctools_modal_adjust();
  if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){  
    $custButtonId  = $form_state['triggering_element']['#ajax']['customButtonId'];
    $esignObj  = '{"popupDiv":"ctools-face-table","esignFor":"AddAdmin","esignButtId":"'.$custButtonId.'"}';
    if($form_state['esign_show_popup']==true)
      $commands[] = ajax_command_invoke(NULL, 'getNewEsignPopup',array($esignObj));
  }
  return array('#type' => 'ajax', '#commands' => $commands);
  } catch (Exception $ex) {
  	watchdog_exception('addEditRepaintEquvForm', $ex);
  	expertusErrorThrow($ex);
  }
}

function addEquivalenceByAPI( &$form_state,$createuserid="") 
{ 
	try {
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$form_state = trimInputFields($form_state); // #42124 - Trim input values
		$form=array();
		$form["form_id"]="";
		$entityId   = $form_state['EntityId'];
    	$enitityType   = $form_state['EnitityType'];
    	$form_state['createuserid'] = $createuserid;
    	if(!empty($entityId)){
    		$qry = db_select('slt_common_mapping','cnmp');
    		$qry->addExpression('Count(1)','cnt');
    		$qry->condition('cnmp.id1',$entityId,'=');
    		$qry->condition('cnmp.id2',$form_state['EquivalenceId'],'=');
    		$qry->condition('cnmp.object_type',$enitityType,'=');
    		$res = $qry->execute()->fetchField();
    		if($res){
    			//return array((object)array('id'=>'Same Course already exist.'));
    			$errobj->isValidateError = 1;
    			$errobj->errcode = 'L_012';
    			$errobj->errormsg = "Same Course has already been added as equivalence";
    			return $errobj;
    		}
    	}
    	$prerequisiteId =  addNewEquivalenceDetails($form,$form_state,$entityId,$enitityType,1);
		expDebug::dPrint(' addNewPrerequisiteByApi created prerequisite id:' . print_r($prerequisiteId,true), 5);
		return $prerequisiteId;  
	} catch (Exception $ex) {
		watchdog_exception('addEquivalenceByAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function removeEquivalenceByApi( &$form_state,$createuserid="") 
{ 
	try {
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	$form_state = trimInputFields($form_state); // #42124 - Trim input values
	$form=array();
	$form["form_id"]="";
    $form_state['createuserid'] = $createuserid;
    $status =  deleteEquivalenceDetails('','',$form_state['id'],1);
	return $status;  
	} catch (Exception $ex) {
		watchdog_exception('removeEquivalenceByApi', $ex);
		expertusErrorThrow($ex);
	}
}


/**
 * 0024925: Enforce equivalence in training plans
 * for this ticket we created equivalence option for training plan
 * don't delete it. Its may need for feature enhancement 
 * Here included overall change previous part. When this option needed we compare with previous part and modify it. 
 * 
 */ 

/**
*

function displayEquivalenceDetails(&$form, &$form_state, $entityId, $entityType, $uniqueId, $mode="", $from=""){
	try {

	  expDebug::dPrint('value of $from'.$from , 5);
	  if($from == 'trainingPlan')
        $equivalenceResult = fetchEquivalenceListTP($entityId, $entityType);
	  else 
	    $equivalenceResult = fetchEquivalenceList($entityId, $entityType);
	  
	  expDebug::dPrint(' count of $listquery1:' . print_r(count($equivalenceResult),true), 5);
  $uniqueId = "equivalence-$entityId-$entityType";
  $count = count($equivalenceResult);

  $customHiddenButtonId = 'equv-save-esign';
  $wrapperId = 'equivalence-grid-wrapper-'.$uniqueId;
  $formName = 'equivalence_form_'.$uniqueId;
  $form_state['form_unique_name'] = $formName;
  
  if($from == 'trainingPlan')
    $msg = t('MSG305TP');
  else 
    $msg = t('MSG304');
  
  $form['attr_action'] = array(
    '#type' => 'hidden',
    '#name' => 'attr_action',
    '#value' => 'equivalence',
  );  
  
  $setminHight = (($entityType=='cre_sys_obt_trp')||($entityType=='cre_sys_obt_cur')) ? 'admin-training-program-tab-datagrid-wrapper' :  'admin-course-tab-datagrid-wrapper';
  
  $form[$formName] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="admin-data-grid"><div id="'.$wrapperId.'" class="admin-datagrid-pagination equv-grid-wrapper '.$setminHight.'">',
    '#suffix' => '</div></div></div>',
  );  

  $form[$formName]['hidden_idlist'] = array(
    '#type' => 'hidden',
    '#name' => 'hidden_idlist_'.$uniqueId,
    '#id' => 'datagrid-idlist-'.$uniqueId,
  );  
  
  $displayMode = $form_state['storage']['mode'];
  
  if($count == 0 && empty($displayMode)){
    
    $addAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
    $addAttributes['data-wrapperid'] = array($wrapperId);
    if((isset($form_state['storage']['display_mode']) && $form_state['storage']['display_mode'] == 'view') || (isset($form_state['storage']['step']) && $form_state['storage']['step'] == 'view')){
      if($from == 'trainingPlan'){
        $form[$formName]['add_prereqiuisite_text'] = array(
            '#type' => 'markup',
            '#markup' => '<div class="no-records-msg admin-datagrid-add-btn-wrapper-container" id="expertus-no-online-users-msg-'.$uniqueId.'">'.t('Training Plan is in-active').'</div>',
        );     
      }
      else{
        $form[$formName]['add_prereqiuisite_text'] = array(
            '#type' => 'markup',
            '#markup' => '<div class="no-records-msg admin-datagrid-add-btn-wrapper-container" id="expertus-no-online-users-msg-'.$uniqueId.'">'.t('MSG343').'</div>',
        );
      }
      
    } else {    
      if($from == 'trainingPlan')
        $equivalenceDetails = fetchEquivalenceListTP($entityId, $entityType);
      else 
        $equivalenceDetails = fetchEquivalenceList($entityId, $entityType);
      
      expDebug::dPrint(' count of $listquery2:' . print_r(count($equivalenceDetails),true), 5);
      //$equivalenceDetails = fetchEquivalenceList($entityId, $entityType);
  	  $courseIdArr = array();
      $excludedCourseId = '""';
      if(count($equivalenceDetails) > 0){
        foreach($equivalenceDetails as $details){
          $courseIdArr[] = $details->associated_object_id;
        }
      }
      $prerequisiteDetails = fetchPrerequisiteList($entityId, $entityType);
      $excludedCourseId = '""';
      if(count($prerequisiteDetails) > 0){
        foreach($prerequisiteDetails as $details){
          $courseIdArr[] = $details->associated_object_id;
        }
      }
      
      $excludedCourseId = implode(',', $courseIdArr);
      $excludedCourseId = $excludedCourseId==""?$entityId:$excludedCourseId.",".$entityId;
      
     if($from == 'trainingPlan')
       $equivalenceAvailableDetails = loadCtoolDataGridForEquivalenceTp('', '', $excludedCourseId, $entityType);
     else
       $equivalenceAvailableDetails = loadCtoolDataGridForEquivalence('', '', $excludedCourseId, $entityType);
      
     expDebug::dPrint(' count of $listquery5:' . print_r(count($equivalenceAvailableDetails),true), 5);
      //$equivalenceAvailableDetails = loadCtoolDataGridForEquivalence('', '', $excludedCourseId, $entityType);
      
      if(count($equivalenceAvailableDetails) == 0){
        
        $form[$formName]['add_equivalance'] = array(
          '#type' => 'markup',
          '#prefix' => '<div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-greyout-bg"></div>',
          '#suffix' => '<div class="admin-save-button-middle-greyout-bg">'.t('LBL303').'</div><div class="admin-save-button-right-greyout-bg"></div></span></div>',
        );
    
        $form[$formName]['add_equivalance_text'] = array(
          '#type' => 'markup',
          '#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.$msg.'</div></div>',
        );
        
      } else {
      //add equivalence button . 
        $form[$formName]['add_equivalance'] = array(
          '#type' => 'submit',
          '#prefix' => '<div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-bg"></div>',
          '#suffix' => '<div class="admin-save-button-right-bg"></div></span></div>',
          '#title' => t('LBL303'),
          '#name' => 'Add equivalence',
          '#value' => t('LBL303'),
          '#submit' => array('add_equivalence'),
          '#attributes' => $addAttributes,
          '#ajax' => array(
            'callback' => 'equivalenceGrid',
            'wrapper' => $wrapperId,
          ),  
        );
    
        $form[$formName]['add_equivalance_text'] = array(
          '#type' => 'markup',
          '#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.$msg.'</div></div>',
        );
      
      }
      
    }
      
  } else {
    
    if($from == 'trainingPlan')
      $equivalenceDetails = fetchEquivalenceListTP($entityId, $entityType);
    else
      $equivalenceDetails = fetchEquivalenceList($entityId, $entityType);
       
	$courseIdArr = array();
    $excludedCourseId = '""';
    if(count($equivalenceDetails) > 0){
      foreach($equivalenceDetails as $details){
        $courseIdArr[] = $details->associated_object_id;
      }
    }
    $prerequisiteDetails = fetchPrerequisiteList($entityId, $entityType);
    $excludedCourseId = '""';
    if(count($prerequisiteDetails) > 0){
      foreach($prerequisiteDetails as $details){
        $courseIdArr[] = $details->associated_object_id;
      }
    }
    $excludedCourseId = implode(',', $courseIdArr);
    $excludedCourseId = $excludedCourseId==""?$entityId:$excludedCourseId.",".$entityId;

    if($displayMode == 'edit'){
      
      $form[$formName][] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_search_container">',
      );
      
      $searchAjaxEvent = 'click';
      $searchAttributes['onClick'][] = '$("body").data("mulitselectdatagrid").searchDataGrid("'.$displayMode.'", "equivalence", $(\'#equivalence-autocomplete\').val(), "'.$entityId.'", "'.$entityType.'", "'.$excludedCourseId.'"); return false;';
      
      if($from == 'trainingPlan')
        $acMenuPath = 'administration/learning/courses/alltp-autocomplete/trainingPlan';
      else
        $acMenuPath = 'administration/learning/courses/allcourses-autocomplete';
      $paintMultiAction  = "<div id='search-list-title-keyword' class='search-list-keyword' style='display:block;'>";
      $paintMultiAction .= "<span id='search-dropdwn-list'>";
      $paintMultiAction .= "<input type='hidden' id='search_all_equivalence_type-hidden' value='name' />";
      $paintMultiAction .= "<input type='hidden' id='equivalence-autocomplete_hidden' value='".t('LBL702')."' />";
      $paintMultiAction .= "<span id='select-list-dropdown' class='select-list-dropdownContent'>".t('LBL107')."</span>";
      $paintMultiAction .= "<a id='admin-dropdown-arrow' class='select-list-dropdown-link dropdown-link-font ' onclick='$(\"#select-list-dropdown-list\").slideToggle();$(\"#select-list-dropdown-list li:last\").css(\"border-bottom\",\"0px none\");'>&nbsp;</a>";
      $paintMultiAction .= "<ul id='select-list-dropdown-list'>";
      $paintMultiAction .= "<li id='name-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEqlCourseSearchTypeText('name','Course');\">".t('LBL107')."</li>";
      $paintMultiAction .= "<li id='tags-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEqlCourseSearchTypeText('tags','Course');\">".t('LBL191')."</li>";
      $paintMultiAction .= "</ul>";
      $paintMultiAction .= "</span></div>";
       
      $form[$formName]['more_add_moveusers_search'] = array(
      		'#type' => 'markup',
      		'#markup' => $paintMultiAction,
      );
      
      $form[$formName]['add_equivalance_search'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_auto_search">',
      );
      
      $autoCompleteAttributes['class'][] = 'admin_ac_input_mainform';
      $autoCompleteAttributes['onkeydown'][]  = '$("body").data("mulitselectdatagrid").multiSelectSearchBoxKeyDown(event)';
      if($from == 'trainingPlan')
        addACFieldForMultiSelect($form[$formName], 'equivalence-autocomplete', $acMenuPath, '', '', '', array(), array('excluded_course_id' => "'".$excludedCourseId."'", 'id_required' => 0,'search_type' => "$('#search_all_equivalence_type-hidden').val()"), false, false, t('LBL702TP'),$autoCompleteAttributes);
      else
        addACFieldForMultiSelect($form[$formName], 'equivalence-autocomplete', $acMenuPath, '', '', '', array(), array('excluded_course_id' => "'".$excludedCourseId."'", 'id_required' => 0,'search_type' => "$('#search_all_equivalence_type-hidden').val()"), false, false, t('LBL702'),$autoCompleteAttributes);

      $form[$formName]['add_equivalance_search1'] = array(
      '#type' => 'markup',
      '#markup' => '</div>',
      );
      
      $searchAttributes['Class'][] = 'admin-pagination-search-go';
       $form[$formName]['search_equivalance'] = array(
        '#type' => 'submit',
        '#title' => t('LBL304'),
        '#name' => 'Search equivalence',
        '#submit' => array('search_equivalence'),
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
    
    $form[$formName]['equivalance_view_grid_markup'] = array(
      '#type' => 'markup',
      '#markup' => '<div id="datagrid-div-'.$uniqueId.'"><table id="datagrid-container-'.$uniqueId.'" class="datagrid-container-common"></table><div id="pager-datagrid-'.$uniqueId.'" class="pager-datagrid-common"></div></div>',
    );
    $displayModeDataGrid = $form_state['storage']['display_mode'] == 'view' || $form_state['storage']['step'] == 'view' ? 'view_only' : $displayMode;
    $form[$formName]['equivalance_view_grid_load'] = array( 
      '#type' => 'markup',
      '#prefix' => '<div style="display:none"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-'.$uniqueId.'\'); $(\'body\').data(\'mulitselectdatagrid\').loadDataGrid(\''.$displayModeDataGrid.'\', \'equivalence\', \'\', \''.$entityId.'\', \''.$entityType.'\', \''.$excludedCourseId.'\');" width="100" height="132" /></div>',
    );       
    
    if($displayMode == 'view'){
      
      if($mode != 'complete_view'){
        $addAnotherAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
        $addAnotherAttributes['data-wrapperid'] = array($wrapperId);
        
      
        if($from == 'trainingPlan') {
          $listquery = loadCtoolDataGridForEquivalenceTp('', '', $excludedCourseId);
        } else {
          $listquery = loadCtoolDataGridForEquivalence('', '', $excludedCourseId);
        }
          
        
        if($form_state['storage']['display_mode'] != 'view' && count($listquery) != 0){
          $form[$formName]['add_equivalance_view_mode'] = array(
            '#type' => 'submit',
            '#title' => t('LBL303'),
      	  '#prefix' => '<div id= "datagrid-add-search-button-'.$uniqueId.'"  style="display:block;" class="admin-save-button-container admin-save-button-container-view-mode"><div class="admin-save-button-left-bg"></div>',
  	      '#suffix' => '<div class="admin-save-button-right-bg"></div></div>',
  	      '#value' => t('LBL303'),
            '#name' => 'Add equivalence',
            '#submit' => array('add_equivalence'),
            '#attributes' => $addAnotherAttributes,
            '#ajax' => array(
              'callback' => 'equivalenceGrid',
              'wrapper' => $wrapperId,
            ),  
          );
          $form[$formName]['add_equivalance_view_mode1'] = array(
            '#type' => 'submit',
            '#title' => t('LBL303'),
      	  '#prefix' => '<div style="display:none;"  id="datagrid-noresult-msg-'.$uniqueId.'"><div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-bg"></div>',
  	      '#suffix' => '<div class="admin-save-button-right-bg"></div></span></div>',
  	      '#value' => t('LBL303'),
            '#name' => 'Add equivalence',
            '#submit' => array('add_equivalence'),
            '#attributes' => $addAnotherAttributes,
            '#ajax' => array(
              'callback' => 'equivalenceGrid',
              'wrapper' => $wrapperId,
            ),  
          );        
          
          $form[$formName]['equivalance_view_grid_no_msg'] = array(
            '#type' => 'markup',
            '#markup' => '<div class="admin-empty-text-msg"> '.$msg.'</div></div></div>',
          );
        } else {
          $form[$formName]['add_equivalance_view_mode1'] = array(
            '#type' => 'submit',
            '#title' => t('LBL303'),
      	  '#prefix' => '<div style="display:none;"  id="datagrid-noresult-msg-'.$uniqueId.'"><div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-bg"></div>',
  	      '#suffix' => '<div class="admin-save-button-right-bg"></div></span></div>',
  	      '#value' => t('LBL303'),
            '#name' => 'Add equivalence',
            '#submit' => array('add_equivalence'),
            '#attributes' => $addAnotherAttributes,
            '#ajax' => array(
              'callback' => 'equivalenceGrid',
              'wrapper' => $wrapperId,
            ),  
          );        
          
          $form[$formName]['equivalance_view_grid_no_msg'] = array(
            '#type' => 'markup',
            '#markup' => '<div class="admin-empty-text-msg"> '.$msg.'</div></div></div>',
          );          
        }
      }  
    } else if($displayMode == 'edit'){

        $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-action-button-middle-bg white-btn-bg-middle';
        $cancelAttributes['data-wrapperid'] = array($wrapperId);
      
        $form[$formName]['cancel_equivalance_edit_mode'] = array(
          '#type' => 'submit',
          '#value' => t('LBL109'),
          '#title' => t('LBL109'),
          '#prefix' => '<div class="admin-datagrid-save-cancel-button-align-bottom"><div class="addedit-form-cancel-container-actions admin-save-button-container resize-save_btn3"><div class="white-btn-bg-left"></div>',
          '#suffix' => '<div class="white-btn-bg-right"></div>',
          '#name' => 'Cancel equivalence',
          '#submit' => array('cancelEquivalenceDetails'),
          '#attributes' => $cancelAttributes,
          '#ajax' => array(
            'callback' => 'equivalenceGrid',
            'wrapper' => $wrapperId,
          ),  
        );
        
        
        if($from == 'trainingPlan')
          $listquery = loadCtoolDataGridForEquivalenceTp('', '', $excludedCourseId);
        else 
         $listquery = loadCtoolDataGridForEquivalence('', '', $excludedCourseId);

        $listquery = loadCtoolDataGridForEquivalence('', '', $excludedCourseId);
        
        if(count($listquery) != 0){
            /*$setAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
            $setAttributes['data-wrapperid'] = array($wrapperId);
    
            $form[$formName]['set_equivalance_edit_mode'] = array(
              '#type' => 'submit',
              '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
    	      '#suffix' => '<div class="admin-save-button-right-bg"></div></div></div>',
              '#title' => t('LBL305'),
              '#value' => t('LBL306'),
              '#name' => 'Set equivalence',
              '#submit' => array('addNewEquivalenceDetails'),
              '#attributes' => $setAttributes,
              '#ajax' => array(
                'callback' => 'equivalenceGrid',
                'wrapper' => $wrapperId,
                'entity_id' => $entityId, 
                'entity_type' => $entityType,
              ),  
            );*/
       /*     if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
              $buttonName  = 'esign-equivalence'; 
            }else{
              $buttonName  = 'set-equivalence';
            }
            
            $setAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
            $setAttributes['data-wrapperid'] = array($wrapperId);
            $form[$formName]['set_equivalence_edit_mode'] = array(
              '#type' => 'submit',
              '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
    	      '#suffix' => '<div class="admin-save-button-right-bg"></div></div></div></div>',
              '#title' => t('LBL305'),
              '#value' => t('LBL306'),
              '#name' => $buttonName,
              '#attributes' => $setAttributes,
              '#ajax' => array(
        		    'wrapper' => $wrapperId,
        		    'callback' => 'addEditRepaintEquvForm',
                    'entity_id' => $entityId, 
                    'entity_type' => $entityType,
                    'entity_plugin'=>'equivalence',
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
    }
    $form_state['storage']['mode'] = '';
  }
  $saveAttributes= array();
  $form[$customHiddenButtonId] = array(
    '#type' => 'submit',
    '#value' => t('hidden-save'),
    '#name'	=> 'hidden-save',
    '#prefix' => '<span id = "esign-admin-div" style="display:none;">',
    '#suffix' => '</span>',
    '#attributes' => $saveAttributes,
    '#id'=>$customHiddenButtonId,
    '#ajax' => array(
    'wrapper' => $wrapperId,
      'callback' => 'addEditRepaintEquvForm',
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
  } catch (Exception $ex) {
  	watchdog_exception('displayEquivalenceDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function loadCtoolDataGridForEquivalence($searchKeyword, $queryRequired = '', $excludedId = '',$searchType=''){
	try {
	expDebug::dPrint('$searchType :'.$searchType,5);
  $select = db_select('slt_course_template', 'course');
  $select->addField('course', 'id', 'id');
  $select->addField('course', 'title', 'title');
  $select->addField('course', 'code', 'code');
  if($searchType != 'tags'){
	  if(!empty($searchKeyword) && $searchKeyword!='' && $searchKeyword!=t('LBL702')){
	    $select->condition('course.title', '%' . db_like($searchKeyword) . '%', 'LIKE');
	  }
  }else if($searchType == 'tags'){
  	if(!empty($searchKeyword) && $searchKeyword!='' && $searchKeyword!= t('LBL193')){
  		$select->leftJoin('slt_tag_entity', 'en', 'en.entity_id = course.id');
  		$select->leftJoin('slt_tagdefn', 'def', 'def.id = en.tagid');
  		$select->condition('def.tagname', '%' . db_like($searchKeyword) . '%', 'LIKE');
  		$select->condition('en.entity_type', 'Course','=');
  	}
  }
  if(!empty($excludedId)){
    $select->condition('course.id', explode(',', $excludedId) , 'NOT IN');
  }
  $select->condition('course.status', 'lrn_crs_sts_atv','=');
  $select->groupBy('course.id');
  //$select->orderBy('title');
  if(empty($queryRequired)){
    return $select->execute()->fetchAll();
  } else {
    return $select;
  }
  } catch (Exception $ex) {
  	watchdog_exception('loadCtoolDataGridForEquivalence', $ex);
  	expertusErrorThrow($ex);
  }
}

function fetchEquivalenceList($entityId, $entityType, $queryRequired = ''){
	try {
  $select = db_select('slt_common_mapping','map');
  $select->Join('slt_course_template', 'crs', 'crs.id=map.id2');
  $select->addField('map','id', 'id');
  $select->addField('map','id1', 'object_id');
  $select->addField('map','id2', 'associated_object_id');
  $select->addField('crs','title', 'title');
  $select->addField('crs','code', 'code');
  $select->condition('map.id1', $entityId,'=');
  $select->condition('map.object_type', $entityType,'=');
  $select->condition('map.type', 6,'=');
  if(empty($queryRequired)){
    return $select->execute()->fetchAll();
  } else {
    return $select;
  }
  } catch (Exception $ex) {
  	watchdog_exception('fetchEquivalenceList', $ex);
  	expertusErrorThrow($ex);
  }
}


function loadCtoolDataGridForEquivalenceTp($searchKeyword, $queryRequired = '', $excludedId = '',$searchType=''){
  try {
    expDebug::dPrint('$searchType :'.$searchType,5);
    expDebug::dPrint('$searchType from :'.$from,5);
    $select = db_select('slt_program', 'program');
    $select->addField('program', 'id', 'id');
    $select->addField('program', 'title', 'title');
    $select->addField('program', 'code', 'code');
    if($searchType != 'tags'){
      if(!empty($searchKeyword) && $searchKeyword!='' && $searchKeyword!=t('LBL702')){
        $select->condition('program.title', '%' . db_like($searchKeyword) . '%', 'LIKE');
      }
    }else if($searchType == 'tags'){
      if(!empty($searchKeyword) && $searchKeyword!='' && $searchKeyword!= t('LBL193')){
        $select->leftJoin('slt_tag_entity', 'en', 'en.entity_id = course.id');
        $select->leftJoin('slt_tagdefn', 'def', 'def.id = en.tagid');
        $select->condition('def.tagname', '%' . db_like($searchKeyword) . '%', 'LIKE');
        //$select->condition('en.entity_type', 'Course','=');
      }
    }
    if(!empty($excludedId)){
      $select->condition('program.id', explode(',', $excludedId) , 'NOT IN');
    }
    $select->condition('program.status', 'lrn_lpn_sts_atv','=');
    $select->groupBy('program.id');
    //$select->orderBy('title');
    if(empty($queryRequired)){
      return $select->execute()->fetchAll();
    } else {
      return $select;
    }
  } catch (Exception $ex) {
    watchdog_exception('loadCtoolDataGridForEquivalenceTp', $ex);
    expertusErrorThrow($ex);
  }
}


function fetchEquivalenceListTP($entityId, $entityType, $queryRequired = ''){
  try {
    $select = db_select('slt_common_mapping','map');
    $select->Join('slt_program', 'program', 'program.id=map.id2');
    $select->addField('map','id', 'id');
    $select->addField('map','id1', 'object_id');
    $select->addField('map','id2', 'associated_object_id');
    $select->addField('program','title', 'title');
    $select->addField('program','code', 'code');
    $select->condition('map.id1', $entityId,'=');
    $select->condition('map.object_type', $entityType,'=');
    $select->condition('map.type', 6,'=');
    if(empty($queryRequired)){
      return $select->execute()->fetchAll();
    } else {
      return $select;
    }
  } catch (Exception $ex) {
    watchdog_exception('fetchEquivalenceListTP', $ex);
    expertusErrorThrow($ex);
  }
}

function cancelEquivalenceDetails($form, &$form_state){
	try {
  $form_state['storage']['mode'] = '';
  $form_state['rebuild'] = TRUE;
  } catch (Exception $ex) {
  	watchdog_exception('cancelEquivalenceDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function equivalenceGrid($form, &$form_state) {
	try {
  $formName = $form_state['form_unique_name'];
  return $form[$formName];
  } catch (Exception $ex) {
  	watchdog_exception('equivalenceGrid', $ex);
  	expertusErrorThrow($ex);
  }
}

function add_equivalence($form, &$form_state) {
	try {
  $form_state['storage']['mode'] = 'edit';
  $form_state['rebuild'] = TRUE;
  } catch (Exception $ex) {
  	watchdog_exception('add_equivalence', $ex);
  	expertusErrorThrow($ex);
  }
}


function deleteEquivalenceDetails($entityId, $entityType, $equivalenceIdList,$callFromAPI = 0) {
	try {
  $select = db_select('slt_common_mapping', 'com'); 
  $select->addField('com','id2','id2');  
  $select->condition('com.id', $equivalenceIdList, '=');  
  $delId = $select->execute()->fetchField(); 
  $equivalenceIdArr = explode('|', $equivalenceIdList);
  
  $txn = db_transaction();
  try {
   
    if(count($equivalenceIdArr) > 0){
        $deleteStmt = db_delete('slt_common_mapping');
        $deleteStmt->condition('id', $equivalenceIdArr,'IN');
        $numDeleted = $deleteStmt->execute();
        if($callFromAPI && $numDeleted){
          return array((object)array('status'=>'Success'));
        }
    }
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('deleteequivalence', $ex);
    if($callFromAPI){
      return array((object)array('status'=>'Failiure'));
    }
    throw $ex;
  }

  unset($txn);
  return $delId;
  } catch (Exception $ex) {
  	watchdog_exception('deleteEquivalenceDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function addNewEquivalenceDetails($form, &$form_state,$entityId,$entityType,$callFromApi = 0 ) {  

  $txn = db_transaction();
  
  try {
    //$entityId = $form_state['triggering_element']['#ajax']['entity_id'];
    //$entityType = $form_state['triggering_element']['#ajax']['entity_type'];
    $uniqueId = "equivalence-$entityId-$entityType";

    
    //if(!empty($form_state['input']['hidden_idlist_'.$uniqueId])){
    if(!empty($form_state['input']['hidden_idlist_'.$uniqueId]) || !empty($form_state['hidden_idlist_'.$uniqueId])  || $callFromApi){
      $userId = getSltpersonUserId();
        //$associateIdListArray = explode(',', $form_state['input']['hidden_idlist_'.$uniqueId]);
        if(!empty($form_state['input']['hidden_idlist_'.$uniqueId]))
          $associateIdListArray = explode(',', $form_state['input']['hidden_idlist_'.$uniqueId]);
        else
          $associateIdListArray = explode(',', $form_state['hidden_idlist_'.$uniqueId]);
    	$idArray = array();   
        if($callFromApi){
          $associateIdList = $form_state['EquivalenceId'];
          $associateIdListArray = explode(',', $associateIdList);
          $userId = $form_state['createuserid'];
        } 
        foreach($associateIdListArray as $associatedEntityId){
          $aExitCnt = isAssociatedCourseExits($entityId,$associatedEntityId,$entityType,'6');
          
          if($aExitCnt == 0) {
            $insertStmt = db_insert('slt_common_mapping');
            $custom  = NULL;
          	
            $fields = array(
                      'id1'                => $entityId,
                      'id2'                => $associatedEntityId,//1000,
            		  		'type'               => 6,
                      'object_type'    	   => $entityType,
            		  		'prereq_type'        => 'cre_sys_pre_crs',
                      'value_string'       => 'Pre-req',
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
            
            $equivalenceId = $insertStmt->execute();
            add_audit_trail_entry($userId, $entityId, $entityType , 'exp_sp_administration_equivalence', 'Equivalence', 'Added new equivalent', null, null , $associatedEntityId);
          if($callFromApi){
                 return array((object)array('id'=>$equivalenceId));
            }
            
          }
          
        }
        
          $form_state['storage']['mode'] = 'view';
          $form_state['rebuild'] = TRUE;
           expertus_set_message(t('MSG601').'.'); 
      
      } /*else {
        
        drupal_set_message(t('ERR106'),'error');
        $form_state['storage']['mode'] = 'edit';
        $form_state['rebuild'] = true;
        //$form_state['storage']['refresh'] = 'norefresh';

      }*/
/*  }   catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('addNewEquivalenceDetails', $ex);
    if($callFromAPI){
      return array((object)array('id'=>'Failiure'));
    }
    throw $ex;    
  }
  
  unset($txn);

}

/*
 * addEditRepaintEquvForm() - callback function for save button to invoke the esign popup if esign module is enabled and validation is passed through
 *                     
 
function addEditRepaintEquvForm($form, &$form_state) {
	try {
  expDebug::dPrint(' addEditRepaintPreReqForm() called.');
  expDebug::dPrint(' ranjani ranjani addEditRepaintPreReqForm() $form_state[esign_show_popup].'.$form_state['esign_show_popup'],5);
  $form_state['storage']['mode'] = 'edit';
  
  $formName = $form_state['form_unique_name'];
  $entityId = $form_state['triggering_element']['#ajax']['entity_id'];
  $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
  $uniqueId = "equivalence-$entityId-$entityType";
  
  $commands = array();
  if($form_state['hidden-save-submit'] && (!empty($form_state['input']['hidden_idlist_'.$uniqueId]) || !empty($form_state['hidden_idlist_'.$uniqueId]))){
    $commands[] = ajax_command_replace(NULL, drupal_render($form[$formName]));
  }
  
  //$commands[] = ajax_command_replace(NULL, drupal_render($form[$formName]));
  //$commands[] = ajax_command_prepend(NULL, theme('status_messages'));
  $commands[] = ajax_command_html('#show_expertus_message', theme('status_messages'));
  $commands[] = exp_ctools_modal_adjust();
  if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){  
    $custButtonId  = $form_state['triggering_element']['#ajax']['customButtonId'];
    $esignObj  = '{"popupDiv":"ctools-face-table","esignFor":"AddAdmin","esignButtId":"'.$custButtonId.'"}';
    if($form_state['esign_show_popup']==true)
      $commands[] = ajax_command_invoke(NULL, 'getNewEsignPopup',array($esignObj));
  }
  return array('#type' => 'ajax', '#commands' => $commands);
  } catch (Exception $ex) {
  	watchdog_exception('addEditRepaintEquvForm', $ex);
  	expertusErrorThrow($ex);
  }
}

function addEquivalenceByAPI( &$form_state,$createuserid="") 
{ 
	try {
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$form=array();
		$form["form_id"]="";
		$entityId   = $form_state['EntityId'];
    $enitityType   = $form_state['EnitityType'];
    $form_state['createuserid'] = $createuserid;
		$prerequisiteId =  addNewEquivalenceDetails($form,$form_state,$entityId,$enitityType,1);
		expDebug::dPrint(' addNewPrerequisiteByApi created prerequisite id:' . print_r($prerequisiteId,true), 5);
		return $prerequisiteId;  
	} catch (Exception $ex) {
		watchdog_exception('addEquivalenceByAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function removeEquivalenceByApi( &$form_state,$createuserid="") 
{ 
	try {
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	$form=array();
	$form["form_id"]="";
    $form_state['createuserid'] = $createuserid;    
	$status =  deleteEquivalenceDetails('','',$form_state['id'],1);
	return $status;  
	} catch (Exception $ex) {
		watchdog_exception('removeEquivalenceByApi', $ex);
		expertusErrorThrow($ex);
	}
}
?>
*/
