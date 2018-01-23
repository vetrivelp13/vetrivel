<?php

function displaySurveyDetails(&$form, &$form_state, $entityId, $entityType, $uniqueId, $mode = '',$module_id){
	try {
  global $secure_entities, $user;	
  $customHiddenButtonId ='esign-survey-button';
  $surveyResult = fetchSurveyList($entityId, $entityType,'',$module_id);
  if (in_array($entityType, $secure_entities)) {
  	$entityId_encrypted = core_encrypt($entityId); // URL encryption
  } else {
  	$entityId_encrypted = $entityId;
  }
  
  $uniqueId = "survey-".$entityId_encrypted."-$entityType";
  $count = count($surveyResult);
  $msg = t('MSG307');
  $wrapperId = 'survey-grid-wrapper-'.$uniqueId;
  $formName = 'survey_form_'.$uniqueId;
  $form_state['form_unique_name'] = $formName;
  $select = db_select('slt_module', 'sltmodule');
  $select->addExpression('sltmodule.id', 'id');
  $select->condition('sltmodule.program_id', $entityId, '=');
  $select->range(0,1);
  $module_id = $select->execute()->fetchField();
  $form['attr_action'] = array(
    '#type' => 'hidden',
    '#name' => 'attr_action',
    '#value' => 'survey',
  );  
  
  $setminHight = ($entityType == 'cre_sys_obt_crt' || $entityType == 'cre_sys_obt_trn' || $entityType == 'cre_sys_obt_cur') ? 'admin-training-program-tab-datagrid-wrapper' : 'admin-course-class-tab-datagrid-wrapper';

  $form[$formName] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="admin-data-grid"><div id="'.$wrapperId.'" class="admin-datagrid-pagination survey-attach-grid-wrapper '.$setminHight.'">',
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
  				'#value' => $module_id,
		);
  
  
  $displayMode = $form_state['storage']['mode'];
  if($count == 0 && empty($displayMode)){    
    
    $addAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
    $addAttributes['data-wrapperid'] = array($wrapperId);
    if(isset($form_state['storage']['display_mode']) && $form_state['storage']['display_mode'] == 'view'|| (isset($form_state['storage']['step']) && ($form_state['storage']['step'] == 'view') && ($entityType != 'cre_sys_obt_cls'))){
      $form[$formName]['add_prereqiuisite_text'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="no-records-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.t('MSG717').'</div></div>',
      );      
    } else {      
      $surveyDetails = fetchSurveyList($entityId, $entityType);
  	  $courseIdArr = array();
      $excludedCourseId = '""';
      if(count($surveyDetails) > 0){
        foreach($surveyDetails as $details){
          $surveyIdArr[] = $details->sur_id;
        }
      }
      $excludedSurveyId = implode(',', $surveyIdArr);
      $excludedSurveyId = empty($excludedSurveyId) ? 0 : $excludedSurveyId;
      
      $listquery = loadCtoolDataGridForSurvey('', '', $excludedSurveyId,$entityId, $entityType);
      //$priv_visible = adminVisibilityForAssign('sry_det_typ_sry');   
      if(count($listquery) == 0  ){ //|| empty($priv_visible)
      
        $msg = t('MSG341');
        
        $form[$formName]['add_survey'] = array(
          '#type' => 'markup',
          '#prefix' => '<div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-greyout-bg"></div>',
          '#suffix' => '<div class="admin-save-button-middle-greyout-bg">'.t('LBL321').'</div><div class="admin-save-button-right-greyout-bg"></div></span></div>',
        );
    
        $form[$formName]['add_survey_text'] = array(
          '#type' => 'markup',
          '#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.$msg.'</div></div></div>',
        );
        
      } else {
        
        $form[$formName]['add_survey'] = array(
          '#type' => 'submit',
          '#prefix' => '<div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-bg"></div>',
          '#suffix' => '<div class="admin-save-button-right-bg"></div></span></div>',
          '#title' => t('LBL321'),
          '#name' => 'Add survey',
          '#value' => t('LBL321'),
          '#submit' => array('add_survey'),
          '#attributes' => $addAttributes,
          '#ajax' => array(
            'callback' => 'surveyGrid',
            'wrapper' => $wrapperId,
          ),  
        );
    
        $form[$formName]['add_survey_text'] = array(
          '#type' => 'markup',
          '#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.$msg.'</div></div></div>',
        );
      
      }
      
    }
  
  } else {

    $surveyDetails = fetchSurveyList($entityId, $entityType);
	$courseIdArr = array();
    $excludedCourseId = '""';
    if(count($surveyDetails) > 0){
      foreach($surveyDetails as $details){
        $surveyIdArr[] = $details->sur_id;
      }
    }
    $excludedSurveyId = implode(',', $surveyIdArr);
    $excludedSurveyId = empty($excludedSurveyId) ? 0 : $excludedSurveyId;

    if($displayMode == 'edit'){
      
      $form[$formName]['start'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_search_container">',
      );
      
      $searchAjaxEvent = 'click';
      $searchAttributes['onClick'][] = '$("body").data("mulitselectdatagrid").searchDataGrid("'.$displayMode.'", "survey", $(\'#survey-autocomplete\').val(), "'.$entityId_encrypted.'", "'.$entityType.'", "'.$excludedSurveyId.'"); return false;';
    
      $acMenuPath = 'administration/learning/survey/autocomplete';
      
      $form[$formName]['add_survey_search'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_auto_search">',
      );
      
      $autoCompleteAttributes['class'][] = 'admin_ac_input_mainform';
      $autoCompleteAttributes['onkeydown'][]  = '$("body").data("mulitselectdatagrid").multiSelectSearchBoxKeyDown(event)'; 
      addACFieldForMultiSelect($form[$formName], 'survey-autocomplete', $acMenuPath, '', '', '', array(), array('excluded_survey_id' => "'".$excludedSurveyId."'", 'type' => '"sry_det_typ_sry"', 'id_required' => 0), false, false, t('LBL664'), $autoCompleteAttributes);

      $form[$formName]['add_survey_search1'] = array(
      '#type' => 'markup',
      '#markup' => '</div>',
      );
      
      $searchAttributes['Class'][] = 'admin-pagination-search-go';
       $form[$formName]['search_survey'] = array(
        '#type' => 'submit',
        '#title' => t('LBL304'),
        '#name' => 'Search survey',
        '#submit' => array('search_survey'),
        '#attributes' => $searchAttributes,
        '#ajax' => array(
          'event' => $searchAjaxEvent,
        ),  
      );
      $form[$formName]['end'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="filter-search-start-date-right-bg"></div><div class="clearBoth"></div></div>',
      );

        
    } else {
      
      $displayMode = 'view';
      
    } 
    
    $form[$formName]['survey_view_grid_markup'] = array(
      '#type' => 'markup',
      '#markup' => '<div id="datagrid-div-'.$uniqueId.'"><table id="datagrid-container-'.$uniqueId.'"></table><div id="pager-datagrid-'.$uniqueId.'" class="pager-datagrid-common"></div></div>',
    );
    
    $displayModeDataGrid = $form_state['storage']['display_mode'] == 'view' || ($form_state['storage']['step'] == 'view') ? 'view_only' : $displayMode;
    $form[$formName]['survey_view_grid_load'] = array( 
      '#type' => 'markup',
      '#prefix' => '<div style="display:none"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-'.$uniqueId.'\'); $(\'body\').data(\'mulitselectdatagrid\').loadDataGrid(\''.$displayModeDataGrid.'\', \'survey\', \'\', \''.$entityId_encrypted.'\', \''.$entityType.'\', \''.$excludedSurveyId.'\');" width="100" height="132" /></div>',
    );       
    
    if($displayMode == 'view'){
      if($mode != 'complete_view'){
        $addAnotherAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
        $addAnotherAttributes['data-wrapperid'] = array($wrapperId);
        
        $listquery = loadCtoolDataGridForSurvey('', '', $excludedSurveyId,$entityId, $entityType);
        
		$sharelinkall = t('MSG848');
		$id='null';
		
		if ($user->language != '' && $user->language == 'ru') {
			$defaultWidth = 347;
		} else if ($user->language != '' && $user->language == 'de'){
			$defaultWidth = 312;
		} else if ($user->language != '' && $user->language == 'es'){
			$defaultWidth = 315;
		} else if ($user->language != '' && $user->language == 'fr'){
			$defaultWidth = 312;
		} else if ($user->language != '' && $user->language == 'it'){
			$defaultWidth = 292;
		} else if ($user->language != '' && $user->language == 'pt-pt'){
			$defaultWidth = 312;
		} else if ($user->language != '' && $user->language == 'ja'){
			$defaultWidth = 321;
		} else if ($user->language != '' && $user->language == 'ko'){
			$defaultWidth = 280;
		} else if ($user->language != '' && $user->language == 'zh-hans'){
			$defaultWidth = 281;
		} else{
			$defaultWidth = 288;
		}
                
        $fnobj = "{'entityId':'".core_encrypt($entityId)."','entityType':'".$entityType."','url':'administration/survey/sharelink/multiple/".core_encrypt($entityId)."/".$entityType."/".$id."','popupDispId':'qtip_disp_sharelink_all_".core_encrypt($entityId)."','catalogVisibleId':'qtipAccessqtip_visible_disp_sharelink_all_".core_encrypt($entityId)."','wid':'".$defaultWidth."','heg':'75','postype':'middle','poslwid':'','qdis':'','linkid':'visible-sharelink-".core_encrypt($entityId)."'}";
        
        $form[$formName]['survey_share_link_all'] = array(
            '#type' => 'markup',
            '#markup' => '<div style="position: absolute; display: block;" class="survey-tab-titles-container" id="sharelink_disp_div"><span class="sharelink-tab-icon"></span><div id="qtip_disp_sharelink_all_'.core_encrypt($entityId).'" class="survey-sharelink-main-wrapper"> '. 
            '<span class="vtip" title="'.Drupal.t('Share').'"><a id="visible-sharelink-'.core_encrypt($entityId).'" onClick="callVisibility('.$fnobj.'); return false;">'.$sharelinkall.'</a></span>'.
            '<span id="visible-popup-'.core_encrypt($entityId).'" class="qtip-popup-visible"></span></div></div>',
        );

        if($form_state['storage']['display_mode'] != 'view' && count($listquery) != 0){
          $form[$formName]['add_survey_view_mode'] = array(
            '#type' => 'submit',
            '#title' => t('LBL321'),
      	  '#prefix' => '<div id= "datagrid-add-search-button-'.$uniqueId.'"  style="display:block;padding-right:10px;" class="admin-save-button-container admin-class-survey-add"><div class="admin-save-button-left-bg"></div>',
  	      '#suffix' => '<div class="admin-save-button-right-bg"></div></div>',
  	      '#value' => t('LBL321'),
            '#name' => 'Add survey',
            '#submit' => array('add_survey'),
            '#attributes' => $addAnotherAttributes,
            '#ajax' => array(
              'callback' => 'surveyGrid',
              'wrapper' => $wrapperId,
            ),  
          );
          $form[$formName]['add_survey_view_mode_btn'] = array(
            '#type' => 'submit',
            '#title' => t('LBL321'),
      	  '#prefix' => '<div style="display:none;"  id="datagrid-noresult-msg-'.$uniqueId.'"><div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-bg"></div>',
  	      '#suffix' => '<div class="admin-save-button-right-bg"></div></span></div>',
  	      '#value' => t('LBL321'),
            '#name' => 'Add survey',
            '#submit' => array('add_survey'),
            '#attributes' => $addAnotherAttributes,
            '#ajax' => array(
              'callback' => 'surveyGrid',
              'wrapper' => $wrapperId,
            ),  
          );
          
          $form[$formName]['survey_view_grid_no_msg'] = array(
            '#type' => 'markup',
            '#markup' => '<div class="admin-empty-text-msg"> '.$msg.'</div></div></div></div>',
          );
        } else {
          $form[$formName]['add_survey_view_mode_btn'] = array(
            '#type' => 'submit',
            '#title' => t('LBL321'),
      	  '#prefix' => '<div style="display:none;"  id="datagrid-noresult-msg-'.$uniqueId.'"><div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-bg"></div>',
  	      '#suffix' => '<div class="admin-save-button-right-bg"></div></span></div>',
  	      '#value' => t('LBL321'),
            '#name' => 'Add survey',
            '#submit' => array('add_survey'),
            '#attributes' => $addAnotherAttributes,
            '#ajax' => array(
              'callback' => 'surveyGrid',
              'wrapper' => $wrapperId,
            ),  
          );
          
          $form[$formName]['survey_view_grid_no_msg'] = array(
            '#type' => 'markup',
            '#markup' => '<div class="admin-empty-text-msg"> '.$msg.'</div></div></div></div>',
          );          
        }
      }
    } else if($displayMode == 'edit'){

        $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-action-button-middle-bg white-btn-bg-middle';
        $cancelAttributes['data-wrapperid'] = array($wrapperId);
      
        $form[$formName]['cancel_survey_edit_mode'] = array(
          '#type' => 'submit',
          '#value' => t('LBL109'),
          '#title' => t('LBL109'),
          '#prefix' => '<div class="admin-datagrid-save-cancel-button-align-bottom"><div class="addedit-form-cancel-container-actions admin-save-button-container resize-save_btn2"><div class="white-btn-bg-left"></div>',
          '#suffix' => '<div class="white-btn-bg-right"></div>',
          '#name' => 'Cancel survey',
          '#submit' => array('cancelSurveyDetails'),
          '#attributes' => $cancelAttributes,
          '#ajax' => array(
            'callback' => 'surveyGrid',
            'wrapper' => $wrapperId,
          ),  
        );

        $listquery = loadCtoolDataGridForSurvey('', '', $excludedSurveyId,$entityId, $entityType);
        
        if(count($listquery) != 0){
          
          /*$setAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
          $setAttributes['data-wrapperid'] = array($wrapperId);
        
          $form[$formName]['set_survey_edit_mode'] = array(
            '#type' => 'submit',
            '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
  	      '#suffix' => '<div class="admin-save-button-right-bg"></div></div></div>',
            '#title' => t('LBL305'),
            '#value' => t('LBL322'),
            '#name' => 'Set survey',
            '#submit' => array('addNewSurveyDetails'),
            '#attributes' => $setAttributes,
            '#ajax' => array(
              'callback' => 'surveyGrid',
              'wrapper' => $wrapperId,
              'entity_id' => $entityId, 
              'entity_type' => $entityType,
            ),  
          );*/
            if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
              $buttonName  = 'esign-survey'; 
            }else{
              $buttonName  = 'set-survey';
            }
            
            $setAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
            $setAttributes['data-wrapperid'] = array($wrapperId);
            if(($entityType == 'cre_sys_obt_crt')){
            	$qtipIdInit        	 = $entityId.'_'.$entityType;
            
            	if($user->language =='es' || $user->language =='ru' || $user->language =='fr'|| $user->language =='it')
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
																	'url':'administration/selectpath/survey/".$entityId."/".$entityType."','popupDispId':'qtip_visible_disp_".$qtipIdInit."',
																	'wid':'".$popupWidth."','heg':'120','postype':'topright',posrwid :'100',
																	'catalogVisibleId':'qtipSelectModulepathqtip_visible_disp_".$qtipIdInit."',
																	'popupDispId':'qtip_visible_disp_select_path_".$qtipIdInit."',
																	'linkid':'visible-survey-path-".$entityId."'}";
				
				$paintMultiAction .='<div id="qtip_visible_disp_select_path_'.$qtipIdInit.'" class="admin-save-button-container set-survey-container"><div class="admin-save-button-left-bg"></div>';
				$paintMultiAction .= "<div id='visible-survey-path-".$entityId."' class = 'admin-save-button-middle-bg survey-tp-path-qtip' onclick = \"callVisibility(".$qtipOptSelectObj.");\" >".t('LBL322')."</div><span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span>".
						'<div class="admin-save-button-right-bg"></div></div><div class="clearBoth"></div><div class="clearBoth"></div></div>';
				
				$form[$formName]['set_survey_edit_mode'] = array(
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
				        		    'callback' => 'addEditSurveyRepaintForm',
				                    'entity_id' => $entityId,
				                    'entity_type' => $entityType,
				            		'module_id' => $module_id,
				                    'entity_plugin'=>'survey',
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
            $form[$formName]['set_survey_edit_mode'] = array(
              '#type' => 'submit',
              '#prefix' => '<div class="admin-save-button-container set-survey-btn"><div class="admin-save-button-left-bg"></div>',
    	      '#suffix' => '<div class="admin-save-button-right-bg"></div></div></div></div>',
              '#title' => t('LBL305'),
              '#value' => t('LBL322'),
              '#name' => $buttonName,
              '#attributes' => $setAttributes,
              '#ajax' => array(
        		    'wrapper' => $wrapperId,
        		    'callback' => 'addEditSurveyRepaintForm',
                    'entity_id' => $entityId, 
                    'entity_type' => $entityType,
                    'entity_plugin'=>'survey',
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
      'callback' => 'addEditSurveyRepaintForm',
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
  	watchdog_exception('displaySurveyDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function loadCtoolDataGridForSurvey($searchKeyword, $queryRequired = '', $excludedSurveyId = '',$entityId = '',$entityType = ''){
 try {
  // Select the table
  $select = db_select('slt_survey', 'survey');
  //$select->leftjoin('slt_survey_mapping', 'map', 'map.survey_id = survey.id');
  //$select->leftjoin('slt_module', 'mdl', 'mdl.id = map.module_id');
  // Add fields to fetch
  $select->distinct();
  $select->addField('survey', 'id', 'id'); 
  $select->addField('survey', 'code', 'code');
  $select->addField('survey', 'title', 'title');

  // Add condition
  $select->condition('survey.status', 'sry_det_sry_atv');
  if(!empty($searchKeyword) && $searchKeyword!='' && $searchKeyword!=t('LBL664')){
    $select->condition('survey.title', '%' . db_like($searchKeyword) . '%', 'LIKE');
  }
  
  $select->condition('survey.type', 'sry_det_typ_sry', '=');
  
  	if($entityType == 'cre_sys_obt_cls'){
  if (!empty($excludedSurveyId)) {
    $excludedSurveyId = urldecode($excludedSurveyId);
    $excludedSurveyIdArr = explode(',', $excludedSurveyId);
  	$select->condition('survey.id', $excludedSurveyIdArr, 'NOT IN');
  }
  	}else{
  	    $selectQuery = db_select('slt_survey_mapping', 'sur_map');
	  	$selectQuery->leftjoin('slt_survey', 'sur', 'sur_map.survey_id = sur.id');
	  	$selectQuery->leftjoin('slt_module', 'mdl', 'mdl.id = sur_map.module_id');
		$selectQuery->addfield('sur_map', 'survey_id', 'sur_id');
		$selectQuery->addExpression('count(mdl.id)','module_list');
		$selectQuery->condition('sur.type', 'sry_det_typ_sry'); // sry_det_typ_sry, sry_det_typ_ass
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
  
  filterByUserAccessPrivileges($select,'survey','',1,'sry_det_typ_sry','attach','survey'); //this is admin segmentation code.Wrongly moved to here so commented this line
  $select->orderBy('survey.title');
  expDebug::dPrintDBAPI("loadCtoolDataGridForSurvey Query---> ",$select);

  if(empty($queryRequired)){
    return $select->execute()->fetchAll();
  } else {
    return $select;
  }
  } catch (Exception $ex) {
  	watchdog_exception('loadCtoolDataGridForSurvey', $ex);
  	expertusErrorThrow($ex);
  }
}

function fetchSurveyList($entityId, $entityType, $queryRequired = '',$module_id=''){
 try {
  $selectQuery = db_select('slt_survey_mapping', 'sur_map');
  $selectQuery->leftjoin('slt_survey', 'sur', 'sur_map.survey_id = sur.id');
  if($entityType != 'cre_sys_obt_cls')
  	$selectQuery->leftjoin('slt_module', 'mdl', 'mdl.id = sur_map.module_id');
  $selectQuery->addfield('sur_map', 'id', 'id');
  $selectQuery->addfield('sur', 'id' , 'sur_id');
  $selectQuery->addfield('sur', 'title');
  $selectQuery->addfield('sur', 'code');
  $selectQuery->addfield('sur_map', 'view_option');
  if($entityType != 'cre_sys_obt_cls')
  	$selectQuery->addfield('mdl', 'title','path');
  
  $selectQuery->condition('sur.type', 'sry_det_typ_sry'); // sry_det_typ_sry, sry_det_typ_ass
  $selectQuery->condition('sur_map.object_id', $entityId);
  $selectQuery->condition('sur_map.object_type', $entityType);
  $selectQuery->condition('sur.status', 'sry_det_sry_atv');
  //$selectQuery->orderBy('sur_map.created_on', 'asc');  
  expDebug::dPrintDBAPI('survey list ',$selectQuery);
  if(empty($queryRequired)){
    return $selectQuery->execute()->fetchAll();
  } else {
    return $selectQuery;
  }
  } catch (Exception $ex) {
  	watchdog_exception('fetchSurveyList', $ex);
  	expertusErrorThrow($ex);
  }
}

function cancelSurveyDetails($form, &$form_state){
	try {
  $form_state['storage']['mode'] = '';
  $form_state['rebuild'] = TRUE;
  } catch (Exception $ex) {
  	watchdog_exception('cancelSurveyDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function surveyGrid($form, &$form_state) {
	try {
  $formName = $form_state['form_unique_name'];
  return $form[$formName];
  } catch (Exception $ex) {
  	watchdog_exception('surveyGrid', $ex);
  	expertusErrorThrow($ex);
  }
}

function add_survey($form, &$form_state) {
	try {
  $form_state['storage']['mode'] = 'edit';
  $form_state['rebuild'] = TRUE;
  } catch (Exception $ex) {
  	watchdog_exception('add_survey', $ex);
  	expertusErrorThrow($ex);
  }
}


function deleteSurveyDetails($entityId, $entityType, $surveyIdList) {
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
  $select->addField('sur','survey_id','survey_id');
  $select->addField('sur','object_id','object_id');
  $select->addField('sur','object_type','object_type');
  $select->addField('sur','module_id','module_id');
  $select->condition('sur.id', $surveyIdList, '=');  
  $delId = $select->execute()->fetchAll(); 
  $surveyIdArr = explode('|', $surveyIdList);
  
  $loggeduserId = getSltpersonUserId();
  
  $txn = db_transaction();
  try {
   
    if(count($surveyIdArr) > 0){
        $deleteStmt = db_delete('slt_survey_mapping');
        $deleteStmt->condition('id', $surveyIdArr,'IN');
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
                    'entity_type' => 'survey_mapping',
                    'parent1_entity_id' => $val->survey_id,
                    'parent1_entity_type' => 'survey_id',
                    'parent2_entity_id' => $val->object_id,
                    'parent2_entity_type' => $val->object_type,
                    'parent3_entity_id' => $mod_id,
                    'parent3_entity_type' => $type,
                    'module_name' => 'exp_sp_administration_survey',
                    'functionality_name' => 'deleteSurveyDetails',
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
    watchdog_exception('deletesurvey', $ex);
    throw $ex;
  }

  unset($txn);
  return $delId->survey_id;
  } catch (Exception $ex) {
  	watchdog_exception('deleteSurveyDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function addNewSurveyDetails($form, &$form_state,$entityId,$entityType) { 
	global $secure_entities;
  $txn = db_transaction();
  
  try {
  	  $uniqueId = "survey-".$entityId."-$entityType";
      expDebug::dprint("test inside add new details". $form_state['input']['hidden_certificate_path_'.$uniqueId]);
      //$entityId = $form_state['triggering_element']['#ajax']['entity_id'];
      //$entityType = $form_state['triggering_element']['#ajax']['entity_type'];
      $entityId = empty($entityId) ? $form_state['entity_id'] : $entityId;
	  $entityType = empty($entityType) ? $form_state['entity_type'] : $entityType ;
	  //$module_id = empty($module_id) ? $form_state['module_id'] : $module_id;
	  $module_id = $form_state['input']['hidden_certificate_path_'.$uniqueId];
      if (in_array($entityType, $secure_entities)) {
      	$uniqueId = "survey-".core_encrypt($entityId)."-$entityType";  // URL Encryption to reretive data
      } else {
      	$uniqueId = "survey-".$entityId."-$entityType";  // URL Encryption to reretive data
      }
      expDebug::dPrint('uniqueId=>'.$uniqueId , 4);
      if(!empty($form_state['input']['hidden_idlist_'.$uniqueId]) || !empty($form_state['hidden_idlist_'.$uniqueId])){
        $formName = $form_state['form_unique_name'];
        $userId = getSltpersonUserId();
        //$idListArray = explode(',', $form_state['input']['hidden_idlist_'.$uniqueId]);
        if(!empty($form_state['input']['hidden_idlist_'.$uniqueId]))
          $idListArray = explode(',', $form_state['input']['hidden_idlist_'.$uniqueId]);
        else
          $idListArray = explode(',', $form_state['hidden_idlist_'.$uniqueId]);
                  
        $idListArray = array_unique($idListArray);
        expDebug::dPrint('$idListArray=>'.print_r($idListArray,true) , 5);
        $viewOption = '';
        $viewOptionList = array();
        
        foreach($idListArray as $surveyIdList){
        	$viewOptionList = explode("-",$surveyIdList);
        	$insertStmt = db_insert('slt_survey_mapping');
        	$custom = 'null';
        	$surveyId = $viewOptionList[0];
        	$viewOption = empty($viewOptionList[1]) ? 'V' : $viewOptionList[1];
        
        	
          $fields = array(
                    'object_id'          => $entityId,                  
                    'object_type'        => $entityType,
          			'module_id'			=> $module_id,
                    'survey_id'     	 => $surveyId,
                    'view_option'        => $viewOption,
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
          $attFormState['survey_id'] = $surveyId;
          $attFormState['view_option'] =  $viewOption;
          $attFormState['after_completion'] = $custom;
        			            
          $surveyId = $insertStmt->execute(); 
          
          
       if($entityType == 'cre_sys_obt_cls')
      	$upd = db_update('slt_course_class');
      else 
      	$upd = db_update('slt_program');
      $fields = array('updated_on'=> now());
      $upd->fields($fields);
      $upd->condition('id',$entityId,'=');
      expDebug::dPrintDBAPI("Update entity sql >>>>>>>>>." , $upd);
      $upd->execute();
          
          make_audit_trail_entries($attFormState, getIdOfLoggedInUser() , null ,$entityId, 'Attached',$entityType,'exp_sp_administration_survey','new survey',
                                          array('survey_id', 'view_option', 'after_completion'));   
          syncSolrData('Program');
          syncSolrData('Class');
          
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
  }catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('addNewSurveyDetails', $ex);
    throw $ex;    
  }
  
  unset($txn);

}


/*
 * getSurveyAutoComplete()
 */
function getSurveyAutoComplete() {
	try {
  $nameSubstr = $_GET['z'];
  $excludedSurveyId = $_GET['excluded_survey_id'];
  
  // Select the table
  $select = db_select('slt_survey', 'survey');

  // Add fields to fetch
  $select->distinct();
  $select->addField('survey', 'id', 'id'); 
  $select->addField('survey', 'code', 'code');
  $select->addField('survey', 'title', 'title');

  // Add condition
  $select->condition('survey.status', 'sry_det_sry_atv');
  $select->condition('survey.type', 'sry_det_typ_sry', '=');
  $select->condition(db_or()
  	                     ->condition('survey.title', '%' . db_like($nameSubstr) . '%', 'LIKE')
  	                     ->condition('survey.code', '%' . db_like($nameSubstr) . '%', 'LIKE')
  	            
  	                  );
  
  if (!empty($excludedSurveyId)) {
    $excludedSurveyId = urldecode($excludedSurveyId);
    $excludedSurveyIdArr = explode(',', $excludedSurveyId);
  	$select->condition('survey.id', $excludedSurveyIdArr, 'NOT IN');
  }
  
  $select->orderBy('survey.title');
 
  filterByUserAccessPrivileges($select,'survey','',1,'sry_det_typ_sry','attach','survey');
  expDebug::dPrintDBAPI("getSurveyAutoComplete ",$select);
  $matchingSurveys = $select->execute()->fetchAll();

  foreach ($matchingSurveys as $survey) {
    print $survey->title. "\n"; 
  }
  print '';
  } catch (Exception $ex) {
  	watchdog_exception('getSurveyAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addEditSurveyRepaintForm() - After the form has been validated and saved, this function returns the form
 *                            to the client for re-rendering.
 */
function addEditSurveyRepaintForm($form, &$form_state) {
	try {
  global $secure_entities;
  $formName = $form_state['form_unique_name'];
  $entityId = $form_state['triggering_element']['#ajax']['entity_id'];
  $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
  if (in_array($entityType, $secure_entities)) {
  	$uniqueId = "survey-".core_encrypt($entityId)."-$entityType"; // URL encryption to retrive data
  } else {
  	$uniqueId = "survey-".$entityId."-$entityType"; // URL encryption to retrive data
  }
  
  $commands = array();
  if($form_state['hidden-save-submit'] && (!empty($form_state['input']['hidden_idlist_'.$uniqueId]) || !empty($form_state['hidden_idlist_'.$uniqueId]))){
    $commands[] = ajax_command_replace(NULL, drupal_render($form[$formName]));
  }
  $commands[] = exp_ctools_modal_adjust();
  
  $statusMsgHTML = theme('status_messages');
  expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true), 4);
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
  	watchdog_exception('addEditSurveyRepaintForm', $ex);
  	expertusErrorThrow($ex);
  }
}

function getSurveyShareLink($linktype, $entityId, $entityType, $surveyId) {
    try {
		$form_state = array(
				'no_redirect' => TRUE, // To confirm whether necessary
				'build_info' => array('args' => array('ajax')), // The form builder function expects the first argument to be ajax or nojs.
				'sharelink_type' => $linktype,
				'entity_id' => $entityId,
				'entity_type' => $entityType,
				'surveymap_id' => $surveyId
				//'empty_id' => $emptyId,
		);
		
		$wrapperId = 'survey-sharelink';
		$formId = 'getSurveyShareLink_view_html';
		$builtForm = drupal_build_form($formId, $form_state);
		$renderedForm = drupal_render($builtForm);

		$renderedTabContentHTML = $renderedForm;//$builtForm;

		$drupalSettings = getDrupalSettings();

		$surveyResponse = array(
				'drupal_settings' => $drupalSettings,
				'render_content_main' => $renderedTabContentHTML,

		);

		return drupal_json_output($surveyResponse);
		exit;

	}catch (Exception $ex) {
		watchdog_exception('getSurveyShareLink', $ex);
		expertusErrorThrow($ex);
	}
}

function getSurveyShareLink_view_html($form, &$form_state){
    try{
    	$enc1 = new Encrypt();
        $enc2 = new Encrypt();
        $enc3 = new Encrypt();
		$survey_mapid = ($form_state['surveymap_id'] !='null') ? $enc1->encrypt($form_state['surveymap_id']) : '';
		if(isset($form_state['entity_type'])) {
			switch($form_state['entity_type']) {
				case 'cre_sys_obt_cls':
					$entity_type = 'Class';
					break;
				case 'cre_sys_obt_cur':
					$entity_type = 'Curricula';
					break;
				case 'cre_sys_obt_crt':
					$entity_type = 'Certification';
					break;
				case 'cre_sys_obt_trn':
					$entity_type = 'LearningPlan';
					break;
			}
		}
		$survey_sharelink = $GLOBALS['base_url'].'/?q=share/surveylink/'.$form_state['sharelink_type'].'/'.$enc2->encrypt($form_state['entity_id']).'/'.$enc3->encrypt($entity_type).'/'.$survey_mapid;

		$shareLinkAttributes['onClick'][] = 'copytoclipboard("edit-sharelink","survey-sharelink-button","'.$form_state['entity_type'].'"); return false;';
        
        $form['surveysharelink_fieldset']['sharelink'] = array(
			'#type' => 'textfield',     
          	'#prefix' => '<div class="sharelink-details-row admin-text-field-column">',
          	'#default_value' => $survey_sharelink,  
          	'#size' => 22,
          	'#attributes' => array('readonly' => 'readonly'),
        );
        $form['surveysharelink_fieldset']['sharelink_button'] = array(
        	'#type' => 'button',
        	'#title' => t('LBL3085'),
        	'#prefix' => '<span id= "survey-sharelink-button" class="admin-save-button-container admin-class-survey-sharelink-button">',
        	'#suffix' => '</span></div>',
        	'#value' => t('LBL3085'),
        	'#name' => 'copy link',
        	'#submit' => array('survey_link'),
        	'#attributes' => $shareLinkAttributes,
        	'#ajax' => array(
        		'#event' => 'click',
        	),
        );
        return $form;
    } catch (Exception $ex) {
        watchdog_exception('getSurveyShareLink_view_html', $ex);
        expertusErrorThrow($ex);
    }
}

?>