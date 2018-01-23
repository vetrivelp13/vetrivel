<?php

function displayPrerequisiteDetails(&$form, &$form_state, $entityId, $entityType, $uniqueId, $mode = ''){
 try {
 	global $secure_entities;
 	include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_learning.inc');
 	include_once(drupal_get_path('module', 'exp_sp_administration_catalog') .'/exp_sp_administration_catalog_course.inc');
  $prerequisiteResult = fetchPrerequisiteList($entityId, $entityType);
  if (in_array($entityType, $secure_entities)) {
  	$entityId_encrypted = core_encrypt($entityId); // URL encryption
  } else {
  	$entityId_encrypted = $entityId;
  }
  $uniqueId = "prerequisite-$entityId_encrypted-$entityType";
  $count = count($prerequisiteResult);
  
  $customHiddenButtonId = 'esign-prerequisite-button';
 
  
  $wrapperId = 'prerequisite-grid-wrapper-'.$uniqueId;
  $formName = 'prerequisite_form_'.$uniqueId;
  $form_state['form_unique_name'] = $formName;
  
 $form['attr_action'] = array(
    '#type' => 'hidden',
    '#name' => 'attr_action',
    '#value' => 'prerequisite',
  ); 
   
  
  if($entityType=='cre_sys_obt_trp') {
     $setminHight = 'admin-training-program-tab-datagrid-wrapper';
     $msg = t('MSG504');
   }else{
    $setminHight = 'admin-course-tab-datagrid-wrapper';
     $msg = t('MSG306');
  }
  //$setminHight = ($entityType=='cre_sys_obt_trp') ? 'admin-training-program-tab-datagrid-wrapper' : 'admin-course-tab-datagrid-wrapper';
  
  $form[$formName] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="admin-data-grid"><div id="'.$wrapperId.'" class="admin-datagrid-pagination prereq-grid-wrapper '.$setminHight.'">',
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
    $addAttributes['onclick'][]       = '$(document).ready(function(){ editAttributesDetailsView(); });';
    
    if((isset($form_state['storage']['display_mode']) && $form_state['storage']['display_mode'] == 'view')|| (isset($form_state['storage']['step']) && ($form_state['storage']['step'] == 'view')) ){
      $form[$formName]['add_prereqiuisite_text'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="no-records-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.t('MSG342').'</div>',
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
      
      $prerequisiteAvailableDetails = loadCtoolDataGridForPrerequisite('', '', $excludedCourseId , $entityType);
      //$priv_visible = adminVisibilityForAssign('cre_sys_obt_crs');     
      if(count($prerequisiteAvailableDetails) == 0  ){ //  || empty($priv_visible)
           
        $msg = t('MSG342');

        $form[$formName]['add_prerequisite'] = array(
          '#type' => 'markup',
          '#prefix' => '<div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-greyout-bg"></div>',
          '#suffix' => '<div class="admin-save-button-middle-greyout-bg">'.t('LBL365').'</div><div class="admin-save-button-right-greyout-bg"></div></span></div>',
        );
                
        $form[$formName]['add_prereqiuisite_text'] = array(
          '#type' => 'markup',
          '#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.$msg.'</div></div>',
        );
        
      } else {
          
        $form[$formName]['add_prerequisite'] = array(
          '#type' => 'submit',
          '#prefix' => '<div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-bg"></div>',
          '#suffix' => '<div class="admin-save-button-right-bg"></div></span></div>',
          '#title' => t('LBL365'),
          '#name' => 'Add prerequisite',
          '#value' => t('LBL365'),
          '#submit' => array('add_prerequisite'),
          '#attributes' => $addAttributes,
          '#ajax' => array(
            'callback' => 'prerequisiteGrid',
            'wrapper' => $wrapperId,
            'event'	   => 'click' 
          ),  
        );
    
        $form[$formName]['add_prereqiuisite_text'] = array(
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
      $searchAttributes['onClick'][] = '$("body").data("mulitselectdatagrid").searchDataGrid("'.$displayMode.'", "prerequisite", $(\'#prerequisite-autocomplete\').val(), "'.$entityId_encrypted.'", "'.$entityType.'", "'.$excludedCourseId.'"); return false;';
    
      if($entityType=='cre_sys_obt_trp'){  
        $acMenuPath = 'administration/learning/courses_tp/allcourses-tp-autocomplete';
      }else{
        $acMenuPath = 'administration/learning/courses/allcourses-autocomplete';
      }
      $paintMultiAction  = "<div id='search-list-title-keyword' class='search-list-keyword' style='display:block;'>";
      $paintMultiAction .= "<span id='search-dropdwn-list'>";
      $paintMultiAction .= "<input type='hidden' id='search_all_prerequisite_type-hidden' value='name' />";
      $paintMultiAction .= "<input type='hidden' id='prerequisite-autocomplete_hidden' value='".t('LBL702')."' />";
      $paintMultiAction .= "<span id='select-list-dropdown' class='select-list-dropdownContent'>".t('LBL107')."</span>";
      $paintMultiAction .= "<a id='admin-dropdown-arrow' class='select-list-dropdown-link dropdown-link-font ' onclick='$(\"#select-list-dropdown-list\").slideToggle();$(\"#select-list-dropdown-list li:last\").css(\"border-bottom\",\"0px none\");'>&nbsp;</a>";
      $paintMultiAction .= "<ul id='select-list-dropdown-list'>";
      if($entityType=='cre_sys_obt_trp'){
      	$paintMultiAction .= "<li id='name-enr-search' onclick=\"$('body').data('mulitselectdatagrid').morePreCourseSearchTypeText('name','Tp');\">".t('LBL107')."</li>";
      	$paintMultiAction .= "<li id='tags-enr-search' onclick=\"$('body').data('mulitselectdatagrid').morePreCourseSearchTypeText('tags','Tp');\">".t('LBL191')."</li>";
      }else{
      	$paintMultiAction .= "<li id='name-enr-search' onclick=\"$('body').data('mulitselectdatagrid').morePreCourseSearchTypeText('name','Course');\">".t('LBL107')."</li>";
      	$paintMultiAction .= "<li id='tags-enr-search' onclick=\"$('body').data('mulitselectdatagrid').morePreCourseSearchTypeText('tags','Course');\">".t('LBL191')."</li>";
      }
      
      $paintMultiAction .= "</ul>";
      $paintMultiAction .= "</span></div>";
       
      $form[$formName]['more_add_moveusers_search'] = array(
      		'#type' => 'markup',
      		'#markup' => $paintMultiAction,
      );
      
      $form[$formName]['add_prerequisite_search'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_auto_search">',
      );
      
      $autoCompleteAttributes['class'][] = 'admin_ac_input_mainform';
      $autoCompleteAttributes['onkeydown'][]  = '$("body").data("mulitselectdatagrid").multiSelectSearchBoxKeyDown(event)'; 
      addACFieldForMultiSelect($form[$formName], 'prerequisite-autocomplete', $acMenuPath, '', '', '', array(), array('excluded_course_id' => "'".$excludedCourseId."'", 'id_required' => 0,'search_type' => "$('#search_all_prerequisite_type-hidden').val()"), false, false, t('LBL702'),$autoCompleteAttributes);

      $form[$formName]['add_prerequisite_search1'] = array(
      '#type' => 'markup',
      '#markup' => '</div>',
      );
      
      $searchAttributes['Class'][] = 'admin-pagination-search-go';
       $form[$formName]['search_prerequisite'] = array(
        '#type' => 'submit',// '#src' => base_path().'sites/all/themes/core/expertusone/expertusone-internals/images/close.png',
        '#title' => t('LBL304'),
        '#name' => 'Search prerequisite',
        '#submit' => array('search_prerequisite'),
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
    
    $form[$formName]['prerequisite_view_grid_markup'] = array(
      '#type' => 'markup',
      '#markup' => '<div id="datagrid-div-'.$uniqueId.'"><table id="datagrid-container-'.$uniqueId.'"></table><div id="pager-datagrid-'.$uniqueId.'" class="pager-datagrid-common"></div></div>',
    );
    
    $displayModeDataGrid = $form_state['storage']['display_mode'] == 'view' || ($form_state['storage']['step'] == 'view') ? 'view_only' : $displayMode;
    $form[$formName]['prerequisite_view_grid_load'] = array( 
      '#type' => 'markup',
      '#prefix' => '<div style="display:none"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-'.$uniqueId.'\'); $(\'body\').data(\'mulitselectdatagrid\').loadDataGrid(\''.$displayModeDataGrid.'\', \'prerequisite\', \'\', \''.$entityId_encrypted.'\', \''.$entityType.'\', \''.$excludedCourseId.'\');" width="100" height="132" /></div>',
    );           
    
    if($displayMode == 'view'){
      
      if($mode != 'complete_view'){
        $addAnotherAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
        $addAnotherAttributes['data-wrapperid'] = array($wrapperId);
        $addAnotherAttributes['onclick'][]       = '$(document).ready(function(){ editAttributesDetailsView(); });';
        
        $listquery = loadCtoolDataGridForEquivalence('', '', $excludedCourseId);
        
        if($form_state['storage']['display_mode'] != 'view' && count($listquery) != 0){
          
          $form[$formName]['add_prerequisite_view_mode'] = array(
            '#type' => 'submit',
            '#title' => t('LBL365'),
      	  '#prefix' => '<div id= "datagrid-add-search-button-'.$uniqueId.'"  style="display:block;" class="admin-save-button-container admin-save-button-container-view-mode"><div class="admin-save-button-left-bg"></div>',
  	      '#suffix' => '<div class="admin-save-button-right-bg"></div></div>',
  	      '#value' => t('LBL365'),
            '#name' => 'Add prerequisite',
            '#submit' => array('add_prerequisite'),
            '#attributes' => $addAnotherAttributes,
            '#ajax' => array(
              'callback' => 'prerequisiteGrid',
              'wrapper' => $wrapperId,
              'event'	   => 'click'
            ),  
          );
          $form[$formName]['add_prerequisite_view_mode_btn'] = array(
            '#type' => 'submit',
            '#title' => t('LBL365'),
      	  '#prefix' => '<div style="display:none;"  id="datagrid-noresult-msg-'.$uniqueId.'"><div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-bg"></div>',
  	      '#suffix' => '<div class="admin-save-button-right-bg"></div></span></div>',
  	      '#value' => t('LBL365'),
            '#name' => 'Add prerequisite',
            '#submit' => array('add_prerequisite'),
            '#attributes' => $addAnotherAttributes,
            '#ajax' => array(
              'callback' => 'prerequisiteGrid',
              'wrapper' => $wrapperId,
              'event'	   => 'click'
            ),  
          );
          $form[$formName]['prerequisite_view_grid_no_msg'] = array(
            '#type' => 'markup',
            '#markup' => '<div class="admin-empty-text-msg"> '.$msg.'</div></div></div>',
          );
          
        } else {
         $form[$formName]['add_prerequisite_view_mode_btn'] = array(
            '#type' => 'submit',
            '#title' => t('LBL365'),
      	  '#prefix' => '<div style="display:none;"  id="datagrid-noresult-msg-'.$uniqueId.'"><div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-bg"></div>',
  	      '#suffix' => '<div class="admin-save-button-right-bg"></div></span></div>',
  	      '#value' => t('LBL365'),
            '#name' => 'Add prerequisite',
            '#submit' => array('add_prerequisite'),
            '#attributes' => $addAnotherAttributes,
            '#ajax' => array(
              'callback' => 'prerequisiteGrid',
              'wrapper' => $wrapperId,
              'event'	   => 'click'
            ),  
          );
          $form[$formName]['prerequisite_view_grid_no_msg'] = array(
            '#type' => 'markup',
            '#markup' => '<div class="admin-empty-text-msg"> '.$msg.'</div></div></div>',
          );          
        }
        
      }
    } else if($displayMode == 'edit'){
        $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-action-button-middle-bg white-btn-bg-middle';
        $cancelAttributes['data-wrapperid'] = array($wrapperId);
        $form[$formName]['cancel_prerequisite_edit_mode'] = array(
          '#type' => 'submit',
          '#value' => t('LBL109'),
          '#title' => t('LBL109'),
          '#prefix' => '<div class="admin-datagrid-save-cancel-button-align-bottom"><div class="addedit-form-cancel-container-actions admin-save-button-container resize-save_btn3"><div class="white-btn-bg-left"></div>',
          '#suffix' => '<div class="white-btn-bg-right"></div>',
          '#name' => 'Cancel prerequisite',
          '#submit' => array('cancelPrerequisiteDetails'),
          '#attributes' => $cancelAttributes,
          '#ajax' => array(
            'callback' => 'prerequisiteGrid',
            'wrapper' => $wrapperId,
          ),  
        );
        $listquery = loadCtoolDataGridForEquivalence('', '', $excludedCourseId);
        if(count($listquery) != 0){
            /*$setAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
            $setAttributes['data-wrapperid'] = array($wrapperId);
          
            $form[$formName]['set_prerequisite_edit_mode'] = array(
              '#type' => 'submit',
              '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
    	      '#suffix' => '<div class="admin-save-button-right-bg"></div></div></div>',
              '#title' => t('LBL305'),
              '#value' => t('LBL366'),
              '#name' => 'Set prerequisite',
              '#submit' => array('addNewPrerequisiteDetails'),
              '#attributes' => $setAttributes,
              '#ajax' => array(
                'callback' => 'prerequisiteGrid',
                'wrapper' => $wrapperId,
                'entity_id' => $entityId, 
                'entity_type' => $entityType,
              ),  
            );*/
            if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
              $buttonName  = 'esign-prerequisite'; 
            }else{
              $buttonName  = 'set-prerequisite';
            }
            
            $setAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
            $setAttributes['data-wrapperid'] = array($wrapperId);
            $form[$formName]['set_prerequisite_edit_mode'] = array(
              '#type' => 'submit',
              '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
    	      '#suffix' => '<div class="admin-save-button-right-bg"></div></div></div></div>',
              '#title' => t('LBL305'),
              '#value' => t('LBL366'),
              '#name' => $buttonName,
              //'#submit' => array('addNewPrerequisiteDetails'),
              '#attributes' => $setAttributes,
              '#ajax' => array(
        		    'wrapper' => $wrapperId,
        		    'callback' => 'addEditRepaintPreReqForm',
                    'entity_id' => $entityId, 
                    'entity_type' => $entityType,
                    'entity_plugin'=>'prerequisite',
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
    }//end edit
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
      'callback' => 'addEditRepaintPreReqForm',
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
  	watchdog_exception('displayPrerequisiteDetails', $ex);
  	expertusErrorThrow($ex);
  }
}//End displayPrerequisiteDetails()

function loadCtoolDataGridForPrerequisite($searchKeyword, $queryRequired = '', $excludedId = '', $entityType,$searchType = ''){
 try {	
 	expDebug::dPrint('$searchType'.$searchType,5);
	$sidx      = $_GET['sidx'];
	$sord      = $_GET['sord'];
	$xorderBy = ($sidx != '') ? $sidx : 'title';
  $select = db_select('slt_course_template', 'course');
  $select->addField('course', 'id', 'id');
  $select->addField('course', 'title', 'title');
  $select->addField('course', 'code', 'code');
  $select->addField('course', 'course_type', 'type');
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
  $select->condition('course.status', 'lrn_crs_sts_atv','=');
  if(!empty($excludedId)){
    $select->condition('course.id', explode(',', $excludedId) , 'NOT IN');
  }
  filterByUserAccessPrivileges($select,'course','',1,'cre_sys_obt_crs','attach','course');
  
  if($entityType=='cre_sys_obt_trp'){
    $select1 = db_select('slt_program', 'program');
    $select1->addField('program', 'id', 'id');
    $select1->addField('program', 'title', 'title');
    $select1->addField('program', 'code', 'code');
    $select1->addField('program', 'object_type', 'type');
    $select1->condition('program.status', 'lrn_lpn_sts_atv','=');
    expDebug::dPrint('$searchType '.$searchType,4);
    if($searchType != 'tags'){
    if(!empty($searchKeyword) && $searchKeyword!='' && $searchKeyword!=t('LBL702')){
      $select1->condition('program.title', '%' . db_like($searchKeyword) . '%', 'LIKE');
    }
    }else if($searchType == 'tags'){
    	if(!empty($searchKeyword) && $searchKeyword!='' && $searchKeyword!= t('LBL193')){
    		$select1->leftJoin('slt_tag_entity', 'en', 'en.entity_id = program.id');
    		$select1->leftJoin('slt_tagdefn', 'def', 'def.id = en.tagid');
    		$select1->condition('def.tagname', '%' . db_like($searchKeyword) . '%', 'LIKE');
    		$select1->condition('en.entity_type', 'Class','!=');
    		$select1->condition('en.entity_type', 'Content','!=');
    	}
    }
    if(!empty($excludedId)){
      $select1->condition('program.id', explode(',', $excludedId) , 'NOT IN');
    }
    filterByUserAccessPrivileges($select1,'program','',1,'cre_sys_obt_trp','attach','program');
  }
  if(empty($queryRequired)){
    if($entityType=='cre_sys_obt_trp'){
      $select1->union($select,'UNION ALL');
      $result = $select1->execute()->fetchAll();
      expDebug::dPrintDBAPI('$result',$select1);
      return $result;
    }else{
      $select->orderby('course.title');
      $result = $select->execute()->fetchAll();
      expDebug::dPrintDBAPI('$result course',$select);
      return $result;
    }
  } else {
     if($entityType=='cre_sys_obt_trp'){
     		$select->groupBy('course.id');
     		$select1->orderBy($xorderBy,$sord);
     		$select1->groupBy('program.id');
        $select->union($select1,'UNION ALL');
        //expDebug::dPrintDBAPI('$select',$select);
        return $select;
     }else{
     	// $select->orderby('course.title');
     	$select->groupBy('course.id');
     	$select->orderBy($xorderBy,$sord); // Order by as Given for this ticket #0046110
     	expDebug::dPrintDBAPI('$select',$select);
     	return $select;
     }
  }
  } catch (Exception $ex) {
  	watchdog_exception('loadCtoolDataGridForPrerequisite', $ex);
  	expertusErrorThrow($ex);
  }
}

function fetchPrerequisiteList($entityId, $entityType, $queryRequired = ''){
 try {	
	$sidx      = $_GET['sidx'];
	$sord      = $_GET['sord'];
	$xorderBy = ($sidx != '') ? $sidx : 'title';
  $select = db_select('slt_common_mapping','map');
  if($entityType=='cre_sys_obt_trp'){
   $select->leftJoin('slt_program', 'crs_prg', 'crs_prg.id=map.id2');
   $select->leftJoin('slt_course_template', 'crs', 'crs.id=map.id2');
   $select->addField('crs_prg','title', 'title');
   $select->addField('crs_prg','code', 'code');
   $select->addField('map','prereq_type', 'prereq_type'); 
   $select->addField('crs','title', 'crstitle');
   $select->addField('crs','code', 'crscode');
   $select->addField('map','prereq_type', 'prereq_type');   
  }
  else { 
  $select->Join('slt_course_template', 'crs', 'crs.id=map.id2');
  $select->addField('crs','title', 'title');
  $select->addField('crs','code', 'code');  
  
  }
  $select->addField('map','prereq_type', 'type');
  $select->addField('map','id', 'id');
  $select->addField('map','id1', 'object_id');
  $select->addField('map','id2', 'associated_object_id');
 
  $select->condition('map.id1', $entityId,'=');
  $select->condition('map.object_type', $entityType,'=');
  $select->condition('map.type', 5,'=');
  $select->orderBy($xorderBy,$sord);
  expDebug::dPrintDBAPI('$select 2',$select);
  if(empty($queryRequired)){
    return $select->execute()->fetchAll();
  } else {
    return $select;
  }
  } catch (Exception $ex) {
  	watchdog_exception('fetchPrerequisiteList', $ex);
  	expertusErrorThrow($ex);
  }
}

function fetchPrerequisiteListTp($entityId, $entityType, $queryRequired = ''){
	try{
   $prereq_type=getAllTpAttachedPrerequisite($entityId, $entityType);
   $select = db_select('slt_common_mapping','map');
   $select->Join('slt_program', 'crs_prg', 'crs_prg.id=map.id1'); 
   $select->addField('crs_prg','title', 'title');
   $select->addField('crs_prg','code', 'code');
   $select->addField('map','prereq_type', 'type'); 
   $select->addField('map','id', 'id');
  $select->addField('map','id1', 'object_id');
  $select->addField('map','id2', 'associated_object_id');
 
  $select->condition('map.id1', $entityId,'=');
  $select->condition('map.object_type', $entityType,'=');
  $select->condition('map.type', 5,'=');
 
 
  $select1 = db_select('slt_common_mapping','map');
  $select1->Join('slt_course_template', 'crs', 'crs.id=map.id1');
  $select1->addField('crs','title', 'title');
  $select1->addField('crs','code', 'code');
  $select1->addField('map','id', 'id');
  $select1->addField('map','id1', 'object_id');
  $select1->addField('map','id2', 'associated_object_id');
 
  $select->condition('map.id1', $entityId,'=');
  $select->condition('map.object_type', $entityType,'=');
  $select->condition('map.type', 5,'=');
  if(empty($queryRequired)){
      $select->union($select1,'UNION ALL');
    $result = $select->execute()->fetchAll();
    expDebug::dPrintDBAPI('$result',$select);
    return $result;
  } else {
    return $select;
  }
  } catch (Exception $ex) {
  	watchdog_exception('fetchPrerequisiteListTp', $ex);
  	expertusErrorThrow($ex);
  }
} 

function cancelPrerequisiteDetails($form, &$form_state){
	try {
  $form_state['storage']['mode'] = '';
  $form_state['rebuild'] = TRUE;
  } catch (Exception $ex) {
  	watchdog_exception('cancelPrerequisiteDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function prerequisiteGrid($form, &$form_state) {
	try {
   $formName = $form_state['form_unique_name'];
   return $form[$formName];
   } catch (Exception $ex) {
   	watchdog_exception('prerequisiteGrid', $ex);
   	expertusErrorThrow($ex);
   }
}

function add_prerequisite($form, &$form_state) {
	try {
  $form_state['storage']['mode'] = 'edit';
  $form_state['rebuild'] = TRUE;
  } catch (Exception $ex) {
  	watchdog_exception('add_prerequisite', $ex);
  	expertusErrorThrow($ex);
  }
}


function deletePrerequisiteDetails($entityId, $entityType, $prerequisiteIdList,$callFromAPI = 0) {
	try {

  $loggedInUserId  = getIdOfLoggedInUser();
  
  $select=db_select('slt_common_mapping','common');
  $select->addField('common','id','id');
  $select->addField('common','id1','p2');
  $select->addField('common','id2','p1');
  $select->addField('common','object_type','p2type');
  $select->addField('common','prereq_type','p1type');
  $select->condition('id', $prerequisiteIdList, '='); 
  $select1=$select->execute()->fetchAll();
	    
  $prerequisiteIdArr = explode('|', $prerequisiteIdList);
  
  $txn = db_transaction();
  try {
   
    if(count($prerequisiteIdArr) > 0){
      
        $deleteStmt = db_delete('slt_common_mapping');
        $deleteStmt->condition('id', $prerequisiteIdArr,'IN');
        $numDeleted = $deleteStmt->execute();

        foreach($select1 as $val){
            $fields= array(
                    'table_name' => 'slt_common_mapping',
                    'entity_id' => $val->id,
                    'entity_type' => 'prerequisite_mapping',
                    'parent1_entity_id' => $val->p1,
                    'parent1_entity_type' => $val->p1type,
                    'parent2_entity_id' => $val->p2,
                    'parent2_entity_type' => $val->p2type,
                    'module_name' => 'exp_sp_administration_prerequisite',
                    'functionality_name' => 'deletePrerequisiteDetails',
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
    watchdog_exception('deleteprerequisite', $ex);
    if($callFromAPI){
      //return array((object)array('status'=>'Failiure'));
      $errobj=new stdClass();
      $errobj->isValidateError = 1;
      $errobj->errcode = 'L_012';
      $errobj->errormsg = "Pre-requisite Id is not valid";
      return $errobj;
    }
    throw $ex;
  }

  unset($txn);
  return $delId;
  } catch (Exception $ex) {
  	watchdog_exception('deletePrerequisiteDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function addNewPrerequisiteDetails($form, &$form_state,$entityId,$entityType,$callFromApi =0) { 
	global $secure_entities;
  $txn = db_transaction();
  try {
    expDebug::dPrint(' addEditRepaintPreReqForm() $form_state[input][hidden_idlist].'.print_r($form_state['input']['hidden_idlist_'.$uniqueId],true) , 5);
    expDebug::dPrint(' addEditRepaintPreReqForm() $form_state[hidden_idlist].'.print_r($form_state['hidden_idlist_'.$uniqueId],true) , 5);
    //$entityId = $form_state['triggering_element']['#ajax']['entity_id'];
    //$entityType = $form_state['triggering_element']['#ajax']['entity_type'];
    //$uniqueId = "prerequisite-$entityId-$entityType";
    if (in_array($entityType, $secure_entities)) {
    	$uniqueId = "prerequisite-".core_encrypt($entityId)."-$entityType"; // URL encryption data to retrive
    } else {
    	$uniqueId = "prerequisite-".$entityId."-$entityType"; // URL encryption data to retrive
    }
    
    /*if(!empty($form_state['input']['hidden_idlist_'.$uniqueId])){
      $userId = getSltpersonUserId();
      $associateIdListArray = explode(',', $form_state['input']['hidden_idlist_'.$uniqueId]);*/
    if(!empty($form_state['input']['hidden_idlist_'.$uniqueId]) || !empty($form_state['hidden_idlist_'.$uniqueId])  || $callFromApi){
      $userId = getSltpersonUserId();
      if(!empty($form_state['input']['hidden_idlist_'.$uniqueId]))
        $associateIdListArray = explode(',', $form_state['input']['hidden_idlist_'.$uniqueId]);
      else
        $associateIdListArray = explode(',', $form_state['hidden_idlist_'.$uniqueId]);
      $idArray = array();
      if($callFromApi){
         $idListArray = array('one' => 'prereqid');
         $userId = $form_state['createuserid'];
       }
      foreach($associateIdListArray as $associatedEntityDetails){
        list($associatedEntityId, $prerequisiteType) = explode('-', $associatedEntityDetails);
        if($callFromApi){
          $associatedEntityId = $form_state['PrequisitesId'];
          $prerequisiteType = $form_state['PrequisitesType'];
        }
        if(!in_array($associatedEntityId, $idArray)){
          $aExitCnt = isAssociatedCourseExits($entityId,$associatedEntityId,$entityType,'5');
          if($aExitCnt == 0) {
            $insertStmt = db_insert('slt_common_mapping');
            $custom  = NULL;
          	
            $fields = array(
                      'id1'                => $entityId,
                      'id2'                => $associatedEntityId,//1000,
            		  		'type'               => 5,
                      'object_type'    	   => $entityType,
            		 			'prereq_type'        => $prerequisiteType,
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
            $prerequisiteId = $insertStmt->execute();
            add_audit_trail_entry($userId, $entityId, $entityType , 'exp_sp_administration_prerequisite', 'Prerequisite', 'Added new prerequiste', null, null , $associatedEntityId);
            if($callFromApi){
                 return array((object)array('id'=>$prerequisiteId));
            } 
          }
        }
      }
      $form_state['storage']['mode'] = 'view';
      $form_state['rebuild'] = TRUE;
      expertus_set_message(t('MSG601').'.'); 
    }/*else {
        
        drupal_set_message('Please select one or more items.','error');
        $form_state['storage']['mode'] = 'edit';
        $form_state['rebuild'] = true;
        //$form_state['storage']['refresh'] = 'norefresh';

      }*/
  }catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('addNewPrerequisiteDetails', $ex);
    if($callFromApi){
      //return array((object)array('id'=>'Failiure'));
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Pre-requisite Id is not valid";
    	return $errobj;
    }
    throw $ex;    
  }
  unset($txn);
}

function getAllTpAttachedPrerequisite($entityId, $entityType){
	try {
  $select = db_select('slt_common_mapping', 'commmap');
  $select->addField('commmap', 'prereq_type', 'prereq_type');  
  // Set the conditions
  $select->condition('commmap.id1', $entityId, '=');
   $prereq_type= $select->execute()->fetchField();
   return $prereq_type;
   } catch (Exception $ex) {
   	watchdog_exception('getAllTpAttachedPrerequisite', $ex);
   	expertusErrorThrow($ex);
   }
}
/*
 * addEditRepaintPreReqForm() - After the form has been validated and saved, this function returns the form
 *                            to the client for re-rendering.
 */
function addEditRepaintPreReqForm($form, &$form_state) {
	try {
  global $secure_entities;
  expDebug::dPrint(' addEditRepaintPreReqForm() called.');
  expDebug::dPrint(' addEditRepaintPreReqForm() $form_state[esign_show_popup].'.$form_state['esign_show_popup'] , 5);
  $form_state['storage']['mode'] = 'edit';
  
  $formName = $form_state['form_unique_name'];
  $entityId = $form_state['triggering_element']['#ajax']['entity_id'];
  $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
  
  if (in_array($entityType, $secure_entities)) {
  	$uniqueId = "prerequisite-".core_encrypt($entityId)."-$entityType"; // URL encryption to retrive data
  } else {
  	$uniqueId = "prerequisite-".$entityId."-$entityType"; // URL encryption to retrive data
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
    //$commands[] = ajax_command_prepend(NULL, $statusMsgHTML);
    $commands[] = ajax_command_html('#show_expertus_message', $statusMsgHTML);
  }
    
  if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){  
    $custButtonId  = $form_state['triggering_element']['#ajax']['customButtonId'];
    $esignObj  = '{"popupDiv":"ctools-face-table","esignFor":"AddAdmin","esignButtId":"'.$custButtonId.'"}';
    if($form_state['esign_show_popup']==true){
      $commands[] = ajax_command_invoke(NULL, 'getNewEsignPopup',array($esignObj));
      $form_state['esign_show_popup']=false;
    }
  }
  return array('#type' => 'ajax', '#commands' => $commands);
  } catch (Exception $ex) {
  	watchdog_exception('addEditRepaintPreReqForm', $ex);
  	expertusErrorThrow($ex);
  }
}

function addPreRequisiteAPI( &$form_state,$createuserid="") 
{ 
	try {
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	$form_state = trimInputFields($form_state); // #42124 - Trim input values
	$form=array();
	$errobj=new stdClass();
	$form["form_id"]="";
	$entityId   = $form_state['EntityId'];
    $enitityType   = $form_state['EnitityType'];
    if($enitityType != 'cre_sys_obt_crs' && $enitityType != 'cre_sys_obt_trp'){
    	//return array((object)array('id'=>'Invalid Entity Type'));
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Invalid Entity Type";
    	return $errobj;
    }else if($form_state['PrequisitesType'] != 'cre_sys_pre_crs' && $form_state['PrequisitesType'] != 'cre_sys_pre_trp'){
    	//return array((object)array('id'=>'Invalid Pre-requisite Type'));
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Invalid Pre-requisite Type";
    	return $errobj;
    }
    if(!empty($entityId)){
    	$qry = db_select('slt_common_mapping','cnmp');
    	$qry->addExpression('Count(1)','cnt');
    	$qry->condition('cnmp.id1',$entityId,'=');
    	$qry->condition('cnmp.id2',$form_state['PrequisitesId'],'=');
    	$qry->condition('cnmp.object_type',$enitityType,'=');
    	$qry->condition('cnmp.prereq_type',$form_state['PrequisitesType'],'=');
    	$res = $qry->execute()->fetchField();
    	if($res){
    		//return array((object)array('id'=>'Same Course already exist.'));
    		$errobj->isValidateError = 1;
    		$errobj->errcode = 'L_012';
    		$errobj->errormsg = "Same Course already exist";
    		return $errobj;
    	}
    }
    $form_state['createuserid'] = $createuserid;
    $prerequisiteId =  addNewPrerequisiteDetails($form,$form_state,$entityId,$enitityType,1);
	expDebug::dPrint(' addNewPrerequisiteByApi created prerequisite id:' . print_r($prerequisiteId,true) , 5);
	return $prerequisiteId;  
	} catch (Exception $ex) {
		watchdog_exception('addPreRequisiteAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function removePreRequisiteByApi( &$form_state,$createuserid="") 
{ 
	try {
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	$form_state = trimInputFields($form_state); // #42124 - Trim input values
	$form=array();
	$form["form_id"]="";
    $form_state['createuserid'] = $createuserid;  
    unset($_REQUEST['entity_id']);
    $status =  deletePrerequisiteDetails('','',$form_state['id'],1);
	return $status;  
	} catch (Exception $ex) {
		watchdog_exception('removePreRequisiteByApi', $ex);
		expertusErrorThrow($ex);
	}
}

?>