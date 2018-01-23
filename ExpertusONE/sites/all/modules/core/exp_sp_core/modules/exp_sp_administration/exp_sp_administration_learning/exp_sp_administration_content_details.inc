<?php

function displayContentDetails(&$form, &$form_state, $courseId, $entityId, $entityType, $uniqueId, $cls_delivery_type){
	try {
  global $secure_entities;
  $customHiddenButtonId = 'content-esign-button';
  $contentResult = fetchContentList($courseId,$entityId, $entityType);
  if (in_array($entityType, $secure_entities)) {
  	$entityId_encrypted = core_encrypt($entityId); // URL encryption
  } else {
  	$entityId_encrypted = $entityId;
  }
  $uniqueId = "content-".$entityId_encrypted."-$entityType";
  $count = count($contentResult);
  
  $msg = t('MSG303'); 
  
  $wrapperId = 'content-grid-wrapper-'.$uniqueId;
  $formName = 'content_form_'.$uniqueId;
  $form_state['form_unique_name'] = $formName;
  
  $form['attr_action'] = array(
    '#type' => 'hidden',
    '#name' => 'attr_action',
    '#value' => 'content',
  );  
  
  $setminHight = ($entityType=='cre_sys_obt_cls') ? 'admin-course-class-tab-datagrid-wrapper' : '';
  
  $form[$formName] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="admin-data-grid"><div id="'.$wrapperId.'" class="admin-content-data-grid-rows admin-datagrid-pagination content-grid-wrapper '.$setminHight.'">',
    //'#suffix' => '</div></div></div>',
    '#suffix' => '</div></div>',
  );  

  $form[$formName]['hidden_idlist'] = array(
    '#type' => 'hidden',
    '#name' => 'hidden_idlist_'.$uniqueId,
    //'#id' => 'datagrid-idlist-'.$uniqueId,
	'#attributes' => array('id' => 'hidden_idlist_'.$uniqueId)
  );  
  
  $displayMode = $form_state['storage']['mode'];
  
  if($count == 0 && empty($displayMode)){    
    
    $addAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
    $addAttributes['data-wrapperid'] = array($wrapperId);
    
    if(isset($form_state['storage']['display_mode']) && $form_state['storage']['display_mode'] == 'view'){
      $form[$formName]['add_prereqiuisite_text'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.t('There are no contents available for this class.').'</div>',
      );
    } else {        
    
      $contentDetails = fetchContentList($courseId,$entityId, $entityType);
  	  $courseIdArr = array();
      $excludedCourseId = '""';
      if(count($contentDetails) > 0){
        foreach($contentDetails as $details){
          $contentIdArr[] = $details->content_id;
        }
      }
      $excludedContentId = implode(',', $contentIdArr);
      $excludedContentId = empty($excludedContentId) ? 0 : $excludedContentId;
      
      $listquery = loadCtoolDataGridForContent('', $cls_delivery_type, '', $excludedContentId);
     // $priv_visible = adminVisibilityForAssign('cre_sys_obt_cnt');  
      if(count($listquery) == 0 ){ //|| empty($priv_visible
      
        $msg = t('MSG344');
        
        $form[$formName]['add_content'] = array(
          '#type' => 'markup',
          '#prefix' => '<div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-greyout-bg"></div>',
          '#suffix' => '<div class="admin-save-button-middle-greyout-bg">'.t('LBL363').'</div><div class="admin-save-button-right-greyout-bg"></div></span></div>',
        );
  
        $form[$formName]['add_content_text'] = array(
          '#type' => 'markup',
          //'#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.$msg.'</div></div>',
          '#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.$msg.'</div></div>',
        );
        
      } else {
      
        $form[$formName]['add_content'] = array(
          '#type' => 'submit',
          '#prefix' => '<div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-bg"></div>',
          '#suffix' => '<div class="admin-save-button-right-bg"></div></span></div>',
          '#title' => t('LBL363'),
          '#name' => 'Add content',
          '#value' => t('LBL363'),
          '#submit' => array('add_content'),
          '#attributes' => $addAttributes,
          '#ajax' => array(
            'callback' => 'contentGrid',
            'wrapper' => $wrapperId,
          ),  
        );
  
        $form[$formName]['add_content_text'] = array(
          '#type' => 'markup',
          //'#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.$msg.'</div></div>',
          '#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.$msg.'</div></div>',
        );
      
      }
      
    }
       $form[$formName]['add_content_wrapper_end'] = array(
          '#type' => 'markup',
          '#markup' => '</div>',
        );      
    
  } else {

    $contentDetails = fetchContentList($courseId,$entityId, $entityType);
	$courseIdArr = array();
    $excludedCourseId = '""';
    if(count($contentDetails) > 0){
      foreach($contentDetails as $details){
        $contentIdArr[] = $details->content_id;
      }
    }
    $excludedContentId = implode(',', $contentIdArr);
    $excludedContentId = empty($excludedContentId) ? 0 : $excludedContentId;

    if($displayMode == 'edit'){
      
      $form[$formName][] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_search_container">',
      );
      $paintMultiAction  = "<div id='search-list-title-keyword' class='search-list-keyword' style='display:block;'>";
      $paintMultiAction .= "<span id='search-dropdwn-list'>";
      $paintMultiAction .= "<input type='hidden' id='search_all_content_type-hidden' value='name' />";
      $paintMultiAction .= "<input type='hidden' id='content-autocomplete_hidden' value='".t('LBL756')."' />";
      $paintMultiAction .= "<span id='select-list-dropdown' class='select-list-dropdownContent'>".t('LBL107')."</span>";
      $paintMultiAction .= "<a  id='admin-dropdown-arrow' class='select-list-dropdown-link dropdown-link-font ' onclick='$(\"body\").data(\"mulitselectdatagrid\").moreEnrollSearchHideShow();'>&nbsp;</a>";
      $paintMultiAction .= "<ul id='select-list-dropdown-list' class='select-Content-dropdown-list'>";
      $paintMultiAction .= "<li id='name-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreContentSearchTypeText('name','Content');\">".t('LBL107')."</li>";
      $paintMultiAction .= "<li id='tags-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreContentSearchTypeText('tags','Content');\">".t('LBL191')."</li>";
      $paintMultiAction .= "</ul>";
      $paintMultiAction .= "</span></div>";
      
      
      $form[$formName]['more_add_enrolluser_search'] = array(
      		'#type' => 'markup',
      		'#markup' => $paintMultiAction,
      );
      
      $searchAjaxEvent = 'click';
      $searchAttributes['onClick'][] = '$("body").data("mulitselectdatagrid").searchDataGrid("'.$displayMode.'", "content", $(\'#content-autocomplete\').val(), "'.$entityId_encrypted.'", "'.$entityType.'", "'.$excludedContentId.'"); return false;';
    
      $acMenuPath = 'administration/learning/wbt-content/autocomplete';
      
      $form[$formName]['add_content_search'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_auto_search">',
      );
      
      $autoCompleteAttributes['class'][] = 'admin_ac_input_mainform';
      $autoCompleteAttributes['onkeydown'][]  = '$("body").data("mulitselectdatagrid").multiSelectSearchBoxKeyDown(event)';
      addACFieldForMultiSelect($form[$formName], 'content-autocomplete', $acMenuPath, '', '', '', array(), array('excluded_id' => "'".$excludedContentId."'", 'id_required' => 0, 'deliverytyp' =>"'".$cls_delivery_type."'",'search_type' => "$('#search_all_content_type-hidden').val()"), false, false, t('LBL756'), $autoCompleteAttributes);

      $form[$formName]['add_content_search1'] = array(
      '#type' => 'markup',
      '#markup' => '</div>',
      );
      
      $searchAttributes['Class'][] = 'admin-pagination-search-go';
       $form[$formName]['search_content'] = array(
        '#type' => 'submit',// '#src' => base_path().'sites/all/themes/core/expertusone/expertusone-internals/images/close.png',
        '#title' => t('LBL304'),
        '#name' => 'Search content',
        '#submit' => array('search_content'),
        '#attributes' => $searchAttributes,
        '#ajax' => array(
          'event' => $searchAjaxEvent,
        ),  
      );
      $form[$formName]['search-content-search-end'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="filter-search-start-date-right-bg"></div><div class="clearBoth"></div></div>',
      );

        
    } else {
      
      $displayMode = 'view';
      
    } 
    
    $form[$formName]['content_view_grid_markup'] = array(
      '#type' => 'markup',
      '#markup' => '<div id="datagrid-div-'.$uniqueId.'"><table id="datagrid-container-'.$uniqueId.'"></table><div id="pager-datagrid-'.$uniqueId.'" class="pager-datagrid-common"></div></div>',
    );
    
    $displayModeDataGrid = $form_state['storage']['display_mode'] == 'view' ? 'view_only' : $displayMode;
    $form[$formName]['content_view_grid_load'] = array( 
      '#type' => 'markup',
      '#prefix' => '<div style="display:none"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-'.$uniqueId.'\'); $(\'body\').data(\'mulitselectdatagrid\').loadDataGrid(\''.$displayModeDataGrid.'\', \'content\', \'\', \''.$entityId_encrypted.'\', \''.$entityType.'\', \''.$excludedContentId.'\');" width="100" height="132" /></div>',
    );           
    
    if($displayMode == 'view'){
      
        $addAnotherAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
        $addAnotherAttributes['data-wrapperid'] = array($wrapperId);
        
        $listquery = loadCtoolDataGridForContent('', $cls_delivery_type, '', $excludedContentId);
        if(count($listquery) != 0 && empty($contentDetails[0]->enroll_id)){        
          $form[$formName]['add_content_view_mode'] = array(
            '#type' => 'submit',
            '#title' => t('LBL363'),
      	    // '#prefix' => '<div id= "datagrid-add-search-button-'.$uniqueId.'"  style="display:block;"><span class="admin-empty-text-msg">* To edit Maximum Attempts and Validity Days click on the corresponding values</span><div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
      	    '#prefix' => '<div id= "datagrid-add-search-button-'.$uniqueId.'"  style="display:block;"><div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
            '#suffix' => '<div class="admin-save-button-right-bg"></div></div></div>',
  	        '#value' => t('LBL363'),
            '#name' => 'Add content',
            '#submit' => array('add_content'),
            '#attributes' => $addAnotherAttributes,
            '#ajax' => array(
              'callback' => 'contentGrid',
              'wrapper' => $wrapperId,
            ),  
          );
        }
        $form[$formName]['content_hints_markup'] = array( 
      	    '#type' => 'markup',
            '#markup' => '<div id="content_hints_markup"><div class="content-view-mode">'.t('LBL879').'</div></div>',
        ); 
          $form[$formName]['add_content_view_mode_btn'] = array(
            '#type' => 'submit',
            '#title' => t('LBL363'),
      	  '#prefix' => '<div style="display:none;"  id="datagrid-noresult-msg-'.$uniqueId.'"><div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-bg"></div>',
  	      '#suffix' => '<div class="admin-save-button-right-bg"></div></span></div>',
  	      '#value' => t('LBL363'),
            '#name' => 'Add content',
            '#submit' => array('add_content'),
            '#attributes' => $addAnotherAttributes,
            '#ajax' => array(
              'callback' => 'contentGrid',
              'wrapper' => $wrapperId,
            ),  
          );
          
          $form[$formName]['content_view_grid_no_msg'] = array(
            '#type' => 'markup',
            '#markup' => '<div class="admin-empty-text-msg"> '.$msg.'</div></div></div></div>',
          );
        
    } else if($displayMode == 'edit'){

        $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-action-button-middle-bg white-btn-bg-middle';
        $cancelAttributes['data-wrapperid'] = array($wrapperId);
      
        $form[$formName]['cancel_content_edit_mode'] = array(
          '#type' => 'submit',
          '#value' => t('LBL109'),
          '#title' => t('LBL109'),
          '#prefix' => '<div class="admin-datagrid-save-cancel-button-align-bottom"><div class="addedit-form-cancel-container-actions admin-save-button-container resize-save_btn2"><div class="white-btn-bg-left"></div>',
          '#suffix' => '<div class="white-btn-bg-right"></div>',
          '#name' => 'Cancel content',
          '#submit' => array('cancelContentDetails'),
          '#attributes' => $cancelAttributes,
          '#ajax' => array(
            'callback' => 'contentGrid',
            'wrapper' => $wrapperId,
          ),  
        );

        $listquery = loadCtoolDataGridForContent('', $cls_delivery_type, '', $excludedContentId);
        
        if(count($listquery) != 0){
          /*$setAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
          $setAttributes['data-wrapperid'] = array($wrapperId);
        
          $form[$formName]['set_content_edit_mode'] = array(
            '#type' => 'submit',
            '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
  	        '#suffix' => '<div class="admin-save-button-right-bg"></div></div></div>',
            '#title' => t('LBL305'),
            '#value' => t('LBL363'),
            '#name' => 'Set content',
            '#submit' => array('addNewContentDetails'),
            '#attributes' => $setAttributes,
            '#ajax' => array(
              'callback' => 'contentGrid',
              'wrapper' => $wrapperId,
              'entity_id' => $entityId, 
              'entity_type' => $entityType,
              'course_id' => $courseId,
            ),  
          );*/
            if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
              $buttonName  = 'esign-content'; 
            }else{
              $buttonName  = 'set-content';
            }
            
            $setAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
            $setAttributes['data-wrapperid'] = array($wrapperId);
            $setAttributes['onclick'][] = 'remove_messages("content");';
            $form[$formName]['set_content_edit_mode'] = array(
              '#type' => 'submit',
              '#prefix' => '<div style="display: none;"><div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
    	      '#suffix' => '<div class="admin-save-button-right-bg"></div></div></div>',
              '#title' => t('LBL305'),
              '#value' => t('LBL363'),
              '#name' => $buttonName,
              '#attributes' => $setAttributes,
              '#id' => 'submit-content-'.$uniqueId,
              '#ajax' => array(
        		    'wrapper' => $wrapperId,
        		    'callback' => 'addEditRepaintContentForm',
                    'entity_id' => $entityId, 
                    'entity_type' => $entityType,
            		'course_id' => $courseId,
                    'entity_plugin'=>'content',
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
            $setAttributes = array();
            $setAttributes['class'][] = 'addedit-form-validate-attempts admin-save-button-middle-bg';
            $setAttributes['data-wrapperid'] = array($wrapperId);
            $setAttributes['onclick'][] = 'return checkIfContentAttemptsSet(\''.$uniqueId.'\');';
            $form[$formName]['set_content_edit_mode_validate_attempts'] = array(
				'#type' => 'button',
				'#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
				'#suffix' => '<div class="admin-save-button-right-bg"></div></div></div></div>',
				'#title' => t ( 'LBL305' ),
				'#value' => t ( 'LBL363' ),
				'#name' => 'test',
				'#attributes' => $setAttributes,
            );
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
                'callback' => 'addEditRepaintContentForm',
                'entity_id' => $entityId, 
                'entity_type' => $entityType,
                'course_id' => $courseId,
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
  	watchdog_exception('clearClassRosterEnrollUserAutocomplete', $ex);
  	expertusErrorThrow($ex);
  }
}

function loadCtoolDataGridForContent($searchKeyword, $deliveryType, $queryRequired = '', $excludedContentId = ''){
 try {
 	$searchType = $_GET['searhType'];
 	expDebug::dPrint("TESTING:searchType:".$searchType);
  $userId = getSltpersonUserId();
  $h5p_exists = 0;
  if (module_exists('exp_sp_administration_contentauthor')) {
  	$h5p_exists = 1;
  }
 
  $select = db_select('slt_content_master', 'content');  
  $select->leftJoin('slt_content_version', 'conver', 'content.id = conver.content_master_id');
  $select->leftjoin('slt_profile_list_items', 'items', 'content.type = items.code');
  if($searchType == 'tags'){
  	$select->leftJoin('slt_tag_entity', 'en', 'en.entity_id = content.id');
  	$select->leftJoin('slt_tagdefn', 'def', 'def.id = en.tagid');
  }
  $select->addField('content', 'id', 'id');
  $select->addField('content', 'code', 'code');
  $select->addField('content', 'type', 'type');
  $select->addField('items', 'name', 'type_name');
  $select->addField('conver', 'total_lesson', 'total_lesson');
  $select->condition('content.status', 'lrn_cnt_sts_atv');
  $select->condition('content.subtype', array('h5p-video-mp4','h5p-video-webm','h5p-Youtube','h5p-Vimeo','h5p-presentatn'),'NOT IN');
  
  //Added condition to refine query results while joining with multiple versions for a content
  $select->condition('conver.status', 'lrn_cnt_sts_atv');
  filterByUserAccessPrivileges($select,'content','',1,'cre_sys_obt_cnt','attach','content');
  if($searchType == 'name'){
  if(!empty($searchKeyword) && $searchKeyword!='' && $searchKeyword!= t('LBL756')){
    $select->condition('content.code', '%' . db_like($searchKeyword) . '%', 'LIKE');
  }
  }else  if($searchType == 'tags'){
  if(!empty($searchKeyword) && $searchKeyword!='' && $searchKeyword!= t('LBL193')){
    $select->condition('def.tagname', '%' . db_like($searchKeyword) . '%', 'LIKE');
    $select->condition('en.entity_type', 'Content','=');
  }
  }
  
  if (!empty($excludedContentId)) {
    $excludedContentId = urldecode($excludedContentId);
    $excludedContentIdArr = explode(',', $excludedContentId);
  	$select->condition('content.id', $excludedContentIdArr, 'NOT IN');
  }
  
  if($deliveryType == 'lrn_cls_dty_vod'){
    $select->condition('content.type', 'lrn_cnt_typ_vod');
  } else {
    $select->condition('content.type', 'lrn_cnt_typ_vod', '!=');
  }
  if ($h5p_exists == 1) {
  	
  	$union  =  db_select('slt_content_master', 'content');
  	$union->leftJoin('slt_content_version', 'conver', 'content.id = conver.content_master_id'); 
  	$union->leftjoin('slt_profile_list_items', 'items', 'content.type = items.code');
  	$union->leftjoin("slt_content_master_h5p_rel","rel","rel.content_master_id=content.id");
 // 	$union->join("slt_h5p_content_share","share1","share1.h5p_content_id=rel.h5p_id and share1.object_id=".$userId." and share1.status='Shared'");
  	 
  	if($searchType == 'tags'){
  		$union->leftJoin('slt_tag_entity', 'en', 'en.entity_id = content.id');
  		$union->leftJoin('slt_tagdefn', 'def', 'def.id = en.tagid');		 
  	}
  	 
  	$union->addField('content', 'id', 'id');
  	$union->addField('content', 'code', 'code');
  	$union->addField('content', 'type', 'type');
  	$union->addField('items', 'name', 'type_name');
  	$union->addField('conver', 'total_lesson', 'total_lesson');
  	$union->condition('content.status', 'lrn_cnt_sts_atv');
  	$union->condition(db_or()
  			->condition('content.subtype','h5p-video-mp4')
  			->condition('content.subtype','h5p-video-webm')
  			->condition('content.subtype','h5p-Youtube')
  	         ->condition('content.subtype','h5p-Vimeo')
        	->condition('content.subtype','h5p-presentatn'));
  	//Added condition to refine query results while joining with multiple versions for a content
  	$union->condition('conver.status', 'lrn_cnt_sts_atv');
  	 
  //	filterByUserAccessPrivileges($union,'content','',1,'cre_sys_obt_cnt','attach','content');
  	
  	if($deliveryType == 'lrn_cls_dty_vod')
  	{
  		filterByUserAccessPrivileges($union,'content','',1,'cre_sys_obt_cnt_aut','attach','content');
  	}
  	else{
  		filterByUserAccessPrivileges($union,'content','',1,'cre_sys_obt_cnt_aut_pre','attach','content');
  	}
  	
  	
  	if($searchType == 'name'){
  		if(!empty($searchKeyword) && $searchKeyword!='' && $searchKeyword!= t('LBL756')){
  			$union->condition('content.code', '%' . db_like($searchKeyword) . '%', 'LIKE');
  		}
  	}else  if($searchType == 'tags'){
  		if(!empty($searchKeyword) && $searchKeyword!='' && $searchKeyword!= t('LBL193')){
  			$union->condition('def.tagname', '%' . db_like($searchKeyword) . '%', 'LIKE');
  			$union->condition('en.entity_type', 'Content','=');
  			 
  		}
  	}
  	 
  	if (!empty($excludedContentId)) {
  		$excludedContentId = urldecode($excludedContentId);
  		$excludedContentIdArr = explode(',', $excludedContentId);
  		$union->condition('content.id', $excludedContentIdArr, 'NOT IN');		 
  	}
  	 
  	if($deliveryType == 'lrn_cls_dty_vod'){
  		$union->condition('content.type', 'lrn_cnt_typ_vod');
  	} else {
  		$union->condition('content.type', 'lrn_cnt_typ_vod', '!=');
  	}
  }
  

//   $config=getConfig("exp_sp");
//   $mylearn_version 					= $config['mylearn_version'];
//   $content_conversion		= $config['content_conversion'];
//   if($deliveryType == 'lrn_cls_dty_wbt' && $mylearn_version && $content_conversion){
//   	$select->condition('content.conversion_status', '', 'IS NULL');
//   }
  expDebug::dPrint('delivery Type ---->'.print_r($deliveryType,1),4);
  $select->groupBy('content.id');

  $select->orderBy('code');  //h5pcustomize - below order by brought it from  loadCtoolDataGridType exp_sp_administration_learning.inc  
  if ($h5p_exists == 1) {
 // $select->orderBy('code'); 
  $union->union($select,"UNION ALL");
  expDebug::dPrintDBAPI("Db_PrintAPI For Cintent suresh:", $union);
  }
  //$select->union($union,'UNION ALL');
  expDebug::dPrintDBAPI("Db_PrintAPI For Cintent suresh123:", $select);
  if(empty($queryRequired)){
   if ($h5p_exists == 1)
   	return $union->execute()->fetchAll();
   else
  	return $select->execute()->fetchAll();
  } else {
  	if ($h5p_exists == 1)
  	 return $union;
  	else
  	  return $select;
  }
  } catch (Exception $ex) {
  	watchdog_exception('clearClassRosterEnrollUserAutocomplete', $ex);
  	expertusErrorThrow($ex);
  }
}

function fetchContentList($courseId,$entityId, $entityType, $queryRequired = ''){
 try {
  $select = db_select('slt_course_content_mapper', 'content');
  $select->leftjoin('slt_content_master', 'master', 'content.content_id = master.id');
  $select->leftjoin('slt_content_version', 'conver', 'conver.content_master_id = master.id');
  $select->leftjoin('slt_profile_list_items', 'items', 'master.type = items.code');
  $select->leftjoin('slt_enrollment', 'enroll', 'enroll.class_id = content.class_id and enroll.course_id = content.course_id and enroll.reg_status IN(\'lrn_crs_reg_wtl\',\'lrn_crs_reg_ppm\',\'lrn_crs_reg_cnf\',\'lrn_crs_reg_ppv\')');
  if($_REQUEST['apiname']=='ListAssociatedContentwithWBTClassAPI') {
   $select->leftjoin('slt_tag_entity', 'te', "master.id=te.entity_id and te.entity_type='Content'");
   $select->leftjoin('slt_tagdefn', 'td', 'td.id=te.tagid');
    $select->addExpression("(select GROUP_CONCAT(distinct def.tagname) FROM slt_tag_entity ent "
            . "LEFT OUTER JOIN slt_tagdefn def on def.id = ent.tagid "
            . "where ent.entity_id = content.content_id and ent.entity_type='Content')","tagname");
   $tag=trim($_REQUEST['tagname']);
   if(isset($tag) && !empty($tag))
        $select->condition('td.tagname', $tag,'=');
   }
  $select->addField('content','id', 'id');
  $select->addField('content','content_id','content_id');
  $select->addField('enroll','id','enroll_id');
  $select->addField('master','code','code');
  $select->addField('items', 'name', 'type_name');
  $select->addField('content','mastery_score','mastery_score');
  $select->addField('content','max_attempts','max_attempts');
  $select->addField('content','validity_days','validity_days');
  $select->addField('conver', 'total_lesson', 'total_lesson');
  $select->condition('content.class_id',$entityId,'=');
  $select->condition('content.course_id',$courseId,'=');
  $select->groupby('content.id');
  $select->orderBy('content.sequence_number');
  expDebug::dPrintDBAPI("Db_PrintAPI VELU", $select);
  if(empty($queryRequired)){
    return $select->execute()->fetchAll();
  } else {
    return $select;
  }
  } catch (Exception $ex) {
  	watchdog_exception('clearClassRosterEnrollUserAutocomplete', $ex);
  	expertusErrorThrow($ex);
  }
}

function cancelContentDetails($form, &$form_state){
	try {
  $form_state['storage']['mode'] = '';
  $form_state['rebuild'] = TRUE;
  } catch (Exception $ex) {
  	watchdog_exception('clearClassRosterEnrollUserAutocomplete', $ex);
  	expertusErrorThrow($ex);
  }
}

function contentGrid($form, &$form_state) {
	try {
  $formName = $form_state['form_unique_name'];
  return $form[$formName];
  } catch (Exception $ex) {
  	watchdog_exception('clearClassRosterEnrollUserAutocomplete', $ex);
  	expertusErrorThrow($ex);
  }
}

function add_content($form, &$form_state) {
	try {
  $form_state['storage']['mode'] = 'edit';
  $form_state['rebuild'] = TRUE;
  } catch (Exception $ex) {
  	watchdog_exception('clearClassRosterEnrollUserAutocomplete', $ex);
  	expertusErrorThrow($ex);
  }
}


function deleteContentDetails($entityId, $entityType, $contentIdList, $callFromAPI = 0) {
	try {
	        $loggedInUserId  = getIdOfLoggedInUser();
	        
	  $select = db_select('slt_course_content_mapper', 'con'); 
	        $select->addField('con','id','id');
	        $select->addField('con','content_id','p3');
	        $select->addField('con','class_id','p2');
	        $select->addField('con','course_id','p1');
	        $select->condition('id', $contentIdList, '='); 
	        $select1=$select->execute()->fetchAll();
	        expDebug::dPrint('$select1 for content'.print_r($select1,true),4);
	        
	  $contentIdArr = explode('|', $contentIdList);
	  expDebug::dPrint('content called.'.$entityId,4);
	  $txn = db_transaction();
	  try {
	    if(count($contentIdArr) > 0){

	        $deleteStmt = db_delete('slt_course_content_mapper');
	        $deleteStmt->condition('id', $contentIdArr,'IN');
	        $numUpdated = $deleteStmt->execute();
	        
	        foreach($select1 as $val){
	            $fields= array(
	                    'table_name' => 'slt_course_content_mapper',
	                    'entity_id' => $val->id,
	                    'entity_type' => 'content_mapping',
	                    'parent1_entity_id' => $val->p1,
	                    'parent1_entity_type' => 'course_id',
	                    'parent2_entity_id' => $val->p2,
	                    'parent2_entity_type' => 'class_id',
	                    'parent3_entity_id' => $val->p3,
	                    'parent3_entity_type' => 'content_id',
	                    'module_name' => 'exp_sp_administration_content_details',
	                    'functionality_name' => 'deleteContentDetails',
	                    'api_name' => $_REQUEST['apiname'],
	                    'deleted_on' => now(),
	                    'deleted_by' => $loggedInUserId,
	            
	            );
	            deleted_log_entry($fields);
	        }
	       
	        updateContentClassDurationTime($entityId);
	        if($callFromAPI && $numUpdated){
	          return array((object)array('status'=>'Success'));
	        }
	        // taking Count of this Contents attached To this Class -- fix for #0035988
	        $select = db_select('slt_course_content_mapper', 'con');
	        $select->addExpression('count(con.id)', 'contentcount');
	        $select->condition('con.class_id', $entityId, '=');
	        expDebug::dPrintDBAPI('$select', $select);
	        $countId = $select->execute()->fetchField();
	        expDebug::dPrint('$countId : '.$countId, 4);
	        // if there is No content means inactive the class -- fix for #0035988
	        if($countId == 0 ){
		        	$updateStmt = db_update('slt_course_class');
		        	$updateStmt->condition('id', $entityId);
		        	$fields = array(
		        			'status' => 'lrn_cls_sts_itv',
		        	);
		        	$updateStmt->fields($fields);
		        	$updateStmt->execute();
	        }
	    }
	  }
	  catch (Exception $ex) {
	    $txn->rollback();
	    watchdog_exception('deletecontent', $ex);
	    if($callFromAPI){
	      //return array((object)array('status'=>'Failiure'));
	    	$errobj=new stdClass();
	    	$errobj->isValidateError = 1;
	    	$errobj->errcode = 'L_012';
	    	$errobj->errormsg = "Content Id is not valid";
	    	return $errobj;
	    }
	    throw $ex;
	  }
	
	  unset($txn);
	  return $delId;
  } catch (Exception $ex) {
  	watchdog_exception('clearClassRosterEnrollUserAutocomplete', $ex);
  	expertusErrorThrow($ex);
  }
}

function addNewContentDetails($form, &$form_state, $callFromApi = 0) { 
	global $secure_entities;
  $txn = db_transaction();
  try {
      if($callFromApi){
        $userId = $form_state['createuserid'];
      }
      else{
         $userId = getSltpersonUserId();
      }
      $entityId = $form_state['triggering_element']['#ajax']['entity_id'];
      $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
      $courseId = $form_state['triggering_element']['#ajax']['course_id'];
      
      
      if (in_array($entityType, $secure_entities)) {
      	$uniqueId = "content-".core_encrypt($entityId)."-$entityType";  // URL Encryption to reretive data
      } else {
      	$uniqueId = "content-".$entityId."-$entityType";  // URL Encryption to reretive data
      }
      
      if(!empty($form_state['input']['hidden_idlist_'.$uniqueId]) || !empty($form_state['hidden_idlist_'.$uniqueId]) || $callFromApi){
        $formName = $form_state['form_unique_name'];
        if(!empty($form_state['input']['hidden_idlist_'.$uniqueId]))
          $idListArray = explode(',', $form_state['input']['hidden_idlist_'.$uniqueId]);
        else
          $idListArray = explode(',', $form_state['hidden_idlist_'.$uniqueId]);
        
       if($callFromApi){
         $idListArray = array('one' => 'contentid');
       }
        $idArray = array();
        $viewOption = '';        
        $selectMaxSequence = db_select('slt_course_content_mapper', 'conmap');
        $selectMaxSequence->addExpression('MAX(conmap.sequence_number)', 'maxseq');              
        $selectMaxSequence->condition('conmap.class_id', $entityId); 
        $maxSequence = $selectMaxSequence->execute()->fetchField();
        $maxSequence = empty($maxSequence) ? 0: $maxSequence; 
        foreach($idListArray as $contentId){
            if($callFromApi){
              $contentId = $form_state['ContentId'];
            }
            $maxSequence++;
            $select = db_select('slt_content_master', 'conmaster');
            $select->addField('conmaster', 'code');              
            $select->condition('conmaster.id', $contentId); 
            $contentCode = $select->execute()->fetchField();        
            
            if(!in_array($contentId, $idArray)){
            
              $insertStmt = db_insert('slt_course_content_mapper');
              $custom  = NULL;
              	
              $attempt = isset($form_state['input']['content-maxattempt-'.$contentId]) ? $form_state['input']['content-maxattempt-'.$contentId] : (isset($form_state['input']['content-maxattempt-'.$contentId.'-hidden']) ? $form_state['input']['content-maxattempt-'.$contentId.'-hidden'] : '');
              $validityDays = $form_state['input']['content-validitydays-'.$contentId];
              if($callFromApi){
              $attempt = $form_state['MaxAttempts'];
              $validityDays = $form_state['ValidityDays'];
              }
              $attempt = empty($attempt) ? $custom : $attempt;
              $validityDays = empty($validityDays) ? $custom : $validityDays;
              
              $fields = array(
                          'course_id'           => $courseId,
                          'class_id'            => $entityId, //class_id
                          'content_id'          => $contentId,
                          'title'               => $contentCode,
                          'content_code'        => $contentCode,
                          'validity_days'       => $validityDays,
                          'max_attempts'        => $attempt,
                          'launch_url'		  => $custom,
                          'sequence_number'	  => $maxSequence,
                          'created_by'          => $userId,
                          'created_on'          => now(),
                          'updated_by'          => $userId,
                          'updated_on'          => now(),
                		    'custom0'             => $custom,
                          'custom1'             => $custom,
                          'custom2'             => $custom,
                          'custom3'             => $custom,
                          'custom4'             => $custom   
                      );
              $insertStmt->fields($fields);			            
               
              $wbtId = $insertStmt->execute();
              
              $updateStmt = db_update('slt_course_class');
              $updateStmt->condition('id', $entityId, '=');
              $updateStmt->fields(array('updated_on'    => now()));
              expDebug::dPrintDBAPI('QUERY DEFAULT123 sannn', $updateStmt);
              $updateStmt->execute();
              
              syncSolrData('Class');   
              
              make_audit_trail_entries($fields, $userId, null, $entityId ,'Inserted','cre_sys_obt_cls','exp_sp_administration_content_details','addNewContentDetails',
                                          array('course_id', 'class_id', 'content_id', 'title', 'content_code', 'validity_days',
                                           'max_attempts', 'launch_url', 'sequence_number'));
			if(empty($attempt)||empty($validityDays)){
				$fields1 = array(
						  'validity_days'       => 'Not Set',
                          'max_attempts'        => 'Not Set',
                          );
						  $insertStmt->fields($fields1);
						  $wbtId = $insertStmt->execute();
			make_audit_trail_entries($fields1, $userId, null, $entityId ,'Inserted','cre_sys_obt_cls','exp_sp_administration_content_details','addNewContentDetails',
						  array('validity_days','max_attempts'));
			}
              if($callFromApi){
                 return array((object)array('id'=>$wbtId));
              }                                     
                  
            }   
          
        }
        
        $form_state['storage']['mode'] = 'view';
        $form_state['rebuild'] = TRUE;
        expertus_set_message(t('MSG601').'.');
      }/*else {
        
        drupal_set_message('Please select one or more Content.','error');
        $form_state['storage']['mode'] = 'edit';
        $form_state['rebuild'] = true;
        //$form_state['storage']['refresh'] = 'norefresh';

      }*/
  }   catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('addcontent', $ex);
    if($callFromAPI){
      //return array((object)array('id'=>'Failiure'));
      $errobj=new stdClass();
      $errobj->isValidateError = 1;
      $errobj->errcode = 'L_012';
      $errobj->errormsg = "Content Id is not valid";
      return $errobj;
    }
    throw $ex;    
  }
  
  unset($txn);

}
function updateNewContentDetails($form, &$form_state, $callFromApi = 0) {
	expDebug::dPrint(' $form_state = ' . print_r($form_state, true) , 5);
	$entityId = $form_state['ContentversionId']; 
	$lessonId = $form_state['ContentlessonId']; 
	$ContentLaunchUrl = $form_state['ContentLaunchUrl'];


	$txn = db_transaction();
	try {
		if($callFromApi){
			$userId = $form_state['createuserid'];
		}
		else{
			$userId = getSltpersonUserId();
		}
		
		//validation Started
		$ch=$ContentLaunchUrl;
		$curl = curl_init($ch);
		curl_setopt_array($curl, array(
    	CURLOPT_RETURNTRANSFER => 1,
    	CURLOPT_USERAGENT => 'Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:28.0) Gecko/20100101 Firefox/28.0'
		));
		$result1 = curl_exec($curl);
		expDebug::dPrint('$result : ' . print_r($result1, true),5);
		$statusCode = curl_getinfo($curl, CURLINFO_HTTP_CODE);
		expDebug::dPrint('$statusCode : ' . print_r($statusCode, true),5);
		$exists = false;
		if ($result1 !== false && $statusCode != 404)
		{
    		 $exists = true;
			 expDebug::dPrint('URL Exists',5);
			 expDebug::dPrint('$exists'.$exists,5);
  		}else{
    		 
      		$exists = false;
			expDebug::dPrint('URL Not Exists',5);
			expDebug::dPrint('$exists'.$exists,5);		
  		}
		curl_close($curl);
		if($exists == 1 && $statusCode != 404) 
		{
			
			expDebug::dPrint(' $result[0]->id = ' . $entityId,4);
			$default1= db_select('slt_content_lesson', 'content1');
			$default1->addField('content1', 'launchurl', 'launchurl');
			$default1->condition('content1.content_version_id', $entityId);
			$default1->condition('content1.id', $lessonId);
			$default = $default1->execute()->fetchAll();
			
			expDebug::dPrintDBAPI('select query content lesson', $default1);
			
			expDebug::dPrint('$default : ' . print_r($default, true),5);
			expDebug::dPrint('$ContentLaunchUrl : ' . print_r($ContentLaunchUrl, true),5);
			
			
		//if($result[0]->hosted_type==2){
		$updateStmt = db_update('slt_content_lesson');
		$updateStmt->condition('content_version_id', $entityId);
		$updateStmt->condition('id', $lessonId);
		$fields = array(
				        'content_version_id'   => $entityId,
						'launchurl'		  => $ContentLaunchUrl,
						'updated_by'          => $userId,
						'updated_on'          => now()
				);
		$updateStmt->fields($fields);
		expDebug::dPrintDBAPI(' update statement = ' , $updateStmt,5);
		
		
		$durationUpdated1 = $updateStmt->execute();
		make_audit_trail_entries($fields, $userId, $default, $entityId ,'Updated','cre_sys_obt_cnt','exp_sp_administration_content_details','updateNewContentByApi',
                                          array('launchurl'));
		
		return array((object)array('Id'=>$entityId, 'LaunchUrl'=> $ContentLaunchUrl));
	}
//}
		//validation ended
	else {
		$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "The selected content url is not valid";
			return $errobj;
		}
	}
	catch (Exception $ex) {
		watchdog_exception('Update third party content URL', $ex);
		expertusErrorThrow($ex);
	}

	unset($txn);

}
function contentSaveEditOption($details, $value){
  try {
  $userId = getSltpersonUserId();
  
  list($id, $type, $entityId, $entityType, $index) = explode('-', $details);
  $index = strtolower($index);
  /*-- #37424: Unable to remove Content validity and attempts in class content page. --*/
  $value = (empty($value) || $value == '-1' || $value == 'NULL')? null : $value;
  $default= db_query("SELECT max_attempts,validity_days,course_id,class_id from slt_course_content_mapper where id=:id",array(":id"=>$id))->fetchAll();
 
  $updateQuery = db_update('slt_course_content_mapper');
  
  $contentDetails = fetchContentList($default[0]->course_id,$default[0]->class_id);
      if(count($contentDetails) > 0){
        foreach($contentDetails as $qdetails){
          $content_id[] = $qdetails->id;
          $total_lesson[] = $qdetails->total_lesson;
        }
      }
      
  $lessionCntError = 0;
  $key = array_search($id, $content_id);
  if($index == 'max_attempts'){
  	$attempt = arg(4);
  }
  
  if ($attempt == 'NULL') {
  	$lessionCntError = 0;
  } else {
  	if($attempt < 1){
  		$lessionCntError = 1;
  	}
  	if(!empty($attempt)) {
  		if($total_lesson[$key] > $attempt){
  			$lessionCntError = 1;
  		}
  	}
  }
    
  
  $fieldsArray = array();
  $result = new stdClass();
  $result->max_attempts = $default[0]->max_attempts;
  // #50062 Modified by joolavasavi
  $result->validity_days = (empty($value) || is_numeric($value)) ? $value : $default[0]->validity_days;
  if($index == 'max_attempts' && $lessionCntError == 0){
    $fieldsArray['max_attempts'] = $value;
    $result->max_attempts = $value;
  }
  if($index == 'validity_days' && (empty($value) || is_numeric($value)) ){
    $fieldsArray['validity_days'] = $value;
  }
  $fieldsArray['updated_by'] = $userId;
  $fieldsArray['updated_on'] = now();
  $updateQuery->fields($fieldsArray);
  $updateQuery->condition('id', $id);
  expDebug::dPrintDBAPI('update query ',$updateQuery);
  	$updateQuery->execute();
    make_audit_trail_entries($fieldsArray, getIdOfLoggedInUser() , $default, $id ,'Updated','cre_sys_obt_cls','exp_sp_administration_content_details','contentSaveEditOption',
                                          array( 'validity_days','max_attempts'));   
                                                                                 
    return $result;      
    } catch (Exception $ex) {
    	watchdog_exception('clearClassRosterEnrollUserAutocomplete', $ex);
    	expertusErrorThrow($ex);
    }                               
}

/*
 * getContentAutoComplete() 
 */
function getContentAutoComplete() {
try {
  $nameSubstr = $_GET['z'];
  $excludedId = $_GET['excluded_id'];
  $deliverytyp = $_GET['deliverytyp'];
  $searchType = $_GET['search_type'];
  $userId = getSltpersonUserId();
  $h5p_exists = 0;
  if (module_exists('exp_sp_administration_contentauthor')) {
  	expDebug::dPrint("CA module exists now for autocomplete:");
  	$h5p_exists = 1;
  }
  // Select the table
  expDebug::dPrint('$searchType :'.$searchType,5);
  if($searchType == 'name'){
  $select = db_select('slt_content_master', 'content');

  // Add fields to fetch
  $select->addField('content', 'id', 'id');
  $select->addField('content', 'code', 'code');
  
  // Add condition
  if($deliverytyp == 'lrn_cls_dty_vod'){
    $select->condition('content.type', 'lrn_cnt_typ_vod');
    } else {
    $select->condition('content.type', 'lrn_cnt_typ_vod', '!=');
    }

  $select->condition('content.status', 'lrn_cnt_sts_atv');
  $select->condition('content.subtype', array('h5p-video-mp4','h5p-video-webm','h5p-Youtube','h5p-Vimeo','h5p-presentatn'),'NOT IN');
  $select->condition('content.code', '%' . db_like($nameSubstr) . '%', 'LIKE');
  
  if (!empty($excludedId)) {
    $excludedId = urldecode($excludedId);
    $excludedIdArr = explode(',', $excludedId);
  	$select->condition('content.id', $excludedIdArr, 'NOT IN');
  }
  filterByUserAccessPrivileges($select,'content','',1,'cre_sys_obt_cnt','attach','content');  
  
   if ($h5p_exists == 1){
  	$union = db_select('slt_content_master', 'content');
  	$union->leftjoin("slt_content_master_h5p_rel","rel","rel.content_master_id=content.id");
 // 	$union->join("slt_h5p_content_share","share1","share1.h5p_content_id=rel.h5p_id and share1.object_id=".$userId." and share1.status='Shared'");
  	
  	// Add fields to fetch
  	$union->addField('content', 'id', 'id');
   	$union->addField('content', 'code', 'code');
	
  	// Add condition
  	if($deliverytyp == 'lrn_cls_dty_vod'){
  		$union->condition('content.type', 'lrn_cnt_typ_vod');
  		expDebug::dPrint('for video testing123 :'.$searchType,5);
  	} else {
  		$union->condition('content.type', 'lrn_cnt_typ_vod', '!=');
  		expDebug::dPrint('for presentation testing123 :'.$searchType,5);
  	}
  	
  	$union->condition('content.status', 'lrn_cnt_sts_atv');
  	$union->condition(db_or()
  			->condition('content.subtype','h5p-video-mp4')
  			->condition('content.subtype','h5p-video-webm')
  			->condition('content.subtype','h5p-Youtube')
  	        ->condition('content.subtype','h5p-Vimeo')
  	        ->condition('content.subtype','h5p-presentatn'));
  	$union->condition('content.code', '%' . db_like($nameSubstr) . '%', 'LIKE');
  	
  	if (!empty($excludedId)) {
  		$excludedId = urldecode($excludedId);
  		$excludedIdArr = explode(',', $excludedId);
  		$union->condition('content.id', $excludedIdArr, 'NOT IN');
  	}
  	
  //	filterByUserAccessPrivileges($union,'content','',1,'cre_sys_obt_cnt','attach','content');

  	 
   	if($deliverytyp == 'lrn_cls_dty_vod')
   	{

     	filterByUserAccessPrivileges($union,'content','',1,'cre_sys_obt_cnt_aut','attach','content');
   	}
  	else
  		filterByUserAccessPrivileges($union,'content','',1,'cre_sys_obt_cnt_aut_pre','attach','content');
  	
  /* 	function filterByUserAccessPrivileges(&$selectStmt, $alisename='', $userId = '',$fromAdmin='',$entity_type='',$page='',$createByAdmin='',$join = '',$accessType ='',$createByAdminCondition = '',$enrollmentalias='') {
  		try{ */
  			 
  }
  
  
 // $select->orderBy('content.code');
 // $union->orderBy('content.code');
  if ($h5p_exists == 1){
  $select->orderBy('code');  //h5pcustomize - below order by brought it from  loadCtoolDataGridType exp_sp_administration_learning.inc
  $union->union($select,"UNION ALL");
  expDebug::dPrintDBAPI('test111111', $union);
  $matchingContent = $union->execute()->fetchAll();
  }
  else
  {
  $select->orderBy('content.code');

  $matchingContent = $select->execute()->fetchAll();
  expDebug::dPrintDBAPI('test111111222', $select);
  }  
  }else if($searchType == 'tags'){
  	$tag = $_GET['z'];
  	$select = db_select('slt_tagdefn', 'sl');
  	$select->leftJoin('slt_tag_entity', 'en', 'en.tagid = sl.id');
  	$select->leftJoin('slt_content_master', 'content', 'en.entity_id = content.id');
  	$select->addField('sl', 'id', 'id');
  	$select->addField('sl', 'tagname', 'code');
  	$select->condition('sl.tagname', '%' . db_like($nameSubstr) . '%', 'LIKE');
  	$select->condition('en.entity_type', 'Content');
  	 if($deliverytyp == 'lrn_cls_dty_vod'){
  		$select->condition('content.type', 'lrn_cnt_typ_vod');
  	} else {
  		$select->condition('content.type', 'lrn_cnt_typ_vod', '!=');
  	} 
  	$select->groupBy('sl.id');
  	filterByUserAccessPrivileges($select, 'content','',1,'cre_sys_obt_cnt','','content');
  	expDebug::dPrintDBAPI('tags', $select);
  	$matchingContent = $select->execute()->fetchAll();
  }
  foreach ($matchingContent as $content) {
    print $content->code. "\n";
  }
  print '';
  } catch (Exception $ex) {
  	watchdog_exception('clearClassRosterEnrollUserAutocomplete', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addEditRepaintContentForm() - callback function for save button to invoke the esign popup if esign module is enabled and validation is passed through
 *                     
 */
function addEditRepaintContentForm($form, &$form_state) {
	try {
  global $secure_entities;			
  expDebug::dPrint(' addEditRepaintPreReqForm() called.');
  expDebug::dPrint(' ranjani ranjani addEditRepaintPreReqForm() $form_state[esign_show_popup].'.$form_state['esign_show_popup'],5);
  $form_state['storage']['mode'] = 'edit';
  
  $formName = $form_state['form_unique_name'];
  $entityId = $form_state['triggering_element']['#ajax']['entity_id'];
  $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
  
  
  if (in_array($entityType, $secure_entities)) {
  	$uniqueId = "content-".core_encrypt($entityId)."-$entityType"; 
  } else {
  	$uniqueId = "content-".$entityId."-$entityType"; 
  }
  
  $commands = array();
  if($form_state['hidden-save-submit'] && (!empty($form_state['input']['hidden_idlist_'.$uniqueId]) || !empty($form_state['hidden_idlist_'.$uniqueId]))){
    $commands[] = ajax_command_replace(NULL, drupal_render($form[$formName]));
  }
  $statusMsgHTML = theme('status_messages');
  //$commands[] = ajax_command_replace(NULL, drupal_render($form[$formName]));
  //$commands[] = ajax_command_prepend(NULL, theme('status_messages'));
  $commands[] = ajax_command_html('#bubble-face-table #show_expertus_message', $statusMsgHTML);
  
  $commands[] = exp_ctools_modal_adjust();
  
  if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){  
    $custButtonId  = $form_state['triggering_element']['#ajax']['customButtonId'];
    $esignObj  = '{"popupDiv":"ctools-face-table","esignFor":"AddAdmin","esignButtId":"'.$custButtonId.'"}';
    if($form_state['esign_show_popup']==true)
      $commands[] = ajax_command_invoke(NULL, 'getNewEsignPopup',array($esignObj));
  }
  // to refresh the parent search page
  $commands[] = ajax_command_refresh_narrow_search_results('onclose');

  return array('#type' => 'ajax', '#commands' => $commands);
  } catch (Exception $ex) {
  	watchdog_exception('clearClassRosterEnrollUserAutocomplete', $ex);
  	expertusErrorThrow($ex);
  }
}

function addNewContentByApi( &$form_state,$createuserid=""){
	try {
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$form_state = trimInputFields($form_state); // #42124 - Trim input values
		$form=array();
		$errobj=new stdClass();
		$form["form_id"]="";
		$form_state['triggering_element']['#ajax']['entity_id']   = $form_state['ClassId'];
		$form_state['triggering_element']['#ajax']['entity_type'] = 'cre_sys_obt_cls';
		$form_state['triggering_element']['#ajax']['course_id']   = $form_state['CourseId'];
		$form_state['createuserid'] = $createuserid;


		$selectenr = db_select('slt_enrollment', 'enr');
		$selectenr->addExpression('count(enr.id)', 'enrollcount');
		$selectenr->condition('enr.course_id', $form_state['CourseId']);
		$selectenr->condition('enr.class_id', $form_state['ClassId']);
		//  $selectenr->condition('enr.user_id', $createuserid);
		$selectenr->condition('enr.reg_status', array('lrn_crs_reg_cnf','lrn_crs_reg_ppm','lrn_crs_reg_wtl','lrn_crs_reg_ppv'),'IN');
		$enrollMapCount = $selectenr->execute()->fetchField();
		if($enrollMapCount > 0){
			$error -> errormsg = 'Cannot associate content since active enrollment avilable for the class';
			$error -> isValidateError = true;
			return $error;
		}


		$selectStmt = db_select('slt_course_class', 'cls');
		$selectStmt->addField('cls', 'delivery_type');
		$selectStmt->addField('cls', 'course_id');
		$selectStmt->condition('cls.id', $form_state['ClassId']);
		$classdetails = $selectStmt->execute()->fetchAll();
		expDebug::dPrint('$classSeliveryType : '.print_r($classSeliveryType, true), 4);
		$classSeliveryType  = $classdetails[0]->delivery_type;
		$classCourseId		= $classdetails[0]->course_id;
		if($classCourseId != $form_state['CourseId']){ // Course id validation
			$error -> errormsg = 'Class is not attached in this Course.';
			$error -> isValidateError = true;
			return $error;
		}
		if(!$classSeliveryType || ($classSeliveryType != 'lrn_cls_dty_wbt' && $classSeliveryType != 'lrn_cls_dty_vod')){
			//return array((object)array('id'=>'Invalid class.'));
			
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Invalid class";
			return $errobj;
		}

		$selectmap = db_select('slt_course_content_mapper', 'map');
		$selectmap->addExpression('count(map.id)', 'contentcount');
		$selectmap->condition('map.course_id', $form_state['CourseId']);
		$selectmap->condition('map.class_id', $form_state['ClassId']);
		$selectmap->condition('map.content_id', $form_state['ContentId']);
		$contentMapCount = $selectmap->execute()->fetchField();

		if($contentMapCount > 0){
			$error -> errormsg = 'Already added this content';
			$error -> isValidateError = true;
			return $error;
		}


		$select = db_select('slt_content_master', 'cm');
		$select->addField('cm', 'type');
		$select->condition('cm.id', $form_state['ContentId']);
		$select->condition('cm.status', 'lrn_cnt_sts_atv');
		$contentType = $select->execute()->fetchField();
		if(!$contentType){
			//return array((object)array('id'=>'Invalid content.'));
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Invalid content";
			return $errobj;
		}
		if((!empty($form_state['MaxAttempts']) && $form_state['MaxAttempts'] < 0 ) 
				|| (!is_numeric($form_state['MaxAttempts']) && is_null($form_state['MaxAttempts'])) ){ // MaxAttempts Validation
			$error -> errormsg = 'MaxAttempts should be greater than 0 or Numeric';
			$error -> isValidateError = true;
			return $error;
		}
		if((!empty($form_state['ValidityDays']) && $form_state['ValidityDays'] < 0 )
				|| (!is_numeric($form_state['ValidityDays']) && is_null($form_state['ValidityDays'])) ){ // ValidityDays Validation
			$error -> errormsg = 'ValidityDays should be greater than 0 or Numeric';
			$error -> isValidateError = true;
			return $error;
		}
		if($classSeliveryType == 'lrn_cls_dty_wbt' && $contentType == 'lrn_cnt_typ_vod'){
			//return array((object)array('id'=>'The selected class is a wbt class and cannot attach a video content.'));
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "The selected class is a wbt class and cannot attach a video content";
			return $errobj;
		}
		if($classSeliveryType == 'lrn_cls_dty_vod' && $contentType != 'lrn_cnt_typ_vod'){
			//return array((object)array('id'=>'The selected class is a vod class and cannot attach a web based content.'));
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "The selected class is a vod class and cannot attach a web based content";
			return $errobj;
		}
		$contentId =  addNewContentDetails($form,$form_state,1);
		expDebug::dPrint(' addNewContentByApi created content id:' . print_r($contentId,true), 5 );
		return $contentId;
	} catch (Exception $ex) {
		watchdog_exception('clearClassRosterEnrollUserAutocomplete', $ex);
		expertusErrorThrow($ex);
	}
}

function updateNewContentByApi(&$form_state,$createuserid=""){
	try {
		//drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$form_state = trimInputFields($form_state); // #42124 - Trim input values
		$form=array();
		$errobj=new stdClass();
		$form["form_id"]="";
		$form_state['triggering_element']['#ajax']['entity_id']   = $form_state['ContentversionId'];
		$form_state['triggering_element']['#ajax']['master_Id']   = $form_state['ContentmasterId'];
		$form_state['triggering_element']['#ajax']['Launchurl']   = $form_state['ContentLaunchUrl'];
		$form_state['triggering_element']['#ajax']['lesson_id']   = $form_state['ContentlessonId'];
		$form_state['createuserid'] = $createuserid;
		
		expDebug::dPrint(' updateNewContentByApi created content type:' .$contentType);
		$select1 = db_select('slt_content_version', 'conver');
		$select1->addField('conver', 'hosted_type');
	    $select1->condition('conver.id', $form_state['ContentversionId']);
	    $select1->condition('conver.content_master_id', $form_state['ContentmasterId']);
		expDebug::dPrintDBAPI(' updateNewContentByApi created select query:' ,$select1);
		$contentType1 = $select1->execute()->rowCount();
		expDebug::dPrint(' updateNewContentByApi created content type1:' .$contentType1);
		
		$default1= db_select('slt_content_lesson', 'content1');
			$default1->addField('content1', 'launchurl', 'launchurl');
			$default1->condition('content1.content_version_id', $form_state['ContentversionId']);
			$default1->condition('content1.id', $form_state['ContentlessonId']);
			$contentType2 = $default1->execute()->rowCount();
		
		if(!$contentType1 || !$contentType2){
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Invalid content";
			return $errobj;
		}
		/*if($contentType1!=2){
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Not a third party content";
			return $errobj;
		}*/
		$result = updateNewContentDetails($form,$form_state,1);
		return $result;
		
	}
	catch (Exception $ex) {
		watchdog_exception('clearClassRosterEnrollUserAutocomplete', $ex);
		expertusErrorThrow($ex);
	}
}
function removeContentByApi( &$form_state,$createuserid="") 
{ 
	try{
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$form_state = trimInputFields($form_state); // #42124 - Trim input values
		$form=array();
		$form["form_id"]="";
	  	$form_state['createuserid'] = $createuserid;
	  	$form_state = stripslashes_deep($form_state);
	  	expDebug::dPrint('Form state : '.print_r($form_state , true), 4);
	  	include_once(drupal_get_path('module', 'exp_sp_administration_catalog') .'/exp_sp_administration_catalog_class.inc');
	  	include_once(drupal_get_path('module', 'exp_sp_administration_content') .'/exp_sp_administration_content.inc');
	  	
	  	$contentstaus = getContentStatus($form_state['ContentId']);
	  	$classStatus = fetchClassDetails($form_state['ClassId']);
	  	$totalEnroll = getTotalClassEnroll($form_state['ClassId']);
	  	
	  	expDebug::dPrint('$totalEnroll: '.print_r($totalEnroll , true), 4);
	  	expDebug::dPrint('$contentstaus : '.print_r($contentstaus , true), 4);
	  	expDebug::dPrint('$classStatus : '.print_r($classStatus , true), 4);
	  	
	  	if(empty($contentstaus)){ // Content id Valdiation
	  		$error -> errormsg = 'Content Id invalid';
	  		$error -> isValidateError = true;
	  		return $error;
	  	}
	  	if(empty($classStatus)){ // Content id Valdiation
	  		$error -> errormsg = 'Class Id invalid';
	  		$error -> isValidateError = true;
	  		return $error;
	  	}
	  	if($totalEnroll > 0){ // Total Enrollment validation
	  		$error -> errormsg = 'Active Enrollments available';
	  		$error -> isValidateError = true;
	  		return $error;
	  	}
	  	$contentMapperValues = getContentTitleForView($form_state['ClassId'],$form_state['ContentId'] , 1);
	  	if(empty($contentMapperValues[0]->id)){ // Content id Valdiation
	  		$error -> errormsg = 'Content is not attached to this class';
	  		$error -> isValidateError = true;
	  		return $error;
	  	}
	  	expDebug::dPrint('$contentMapperValues : '.print_r($contentMapperValues[0]->id , true), 4);
	  	 $status =  deleteContentDetails('','',$contentMapperValues[0]->id,1);
		return $status;  
	} catch (Exception $ex) {
		watchdog_exception('clearClassRosterEnrollUserAutocomplete', $ex);
		expertusErrorThrow($ex);
	}
}
function listContentByApi( &$form_state,$createuserid="") 
{ 
	try {
			drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
			$form=array();
			$form["form_id"]="";
	    $form_state['createuserid'] = $createuserid;    
			$status =  fetchContentList($form_state['CourseId'],$form_state['ClassId'],'cre_sys_obt_cls');
			$lessonId = '';
		if($status[0]->content_id){
	  	  $select = db_select('slt_content_version', 'ver'); 
	      $select->addField('ver','id','content_id');  
	      $select->condition('ver.content_master_id',$status[0]->content_id, '=');  
	      $contentId = $select->execute()->fetchField();
	      
	      $selectLes = db_select('slt_content_lesson', 'les'); 
	      $selectLes->addField('les','id','lession_id');  
	      $selectLes->condition('les.content_version_id',$contentId, '=');  
	      $selectLesson = $selectLes->execute()->fetchAllKeyed();
	      $selectLesson = array_keys($selectLesson);
	      $lessonId  = implode(',',$selectLesson);
		}
		if($lessonId != '') {
		$status[0]->lession_id = $lessonId;
		}
		//51737: List of API's total records count shows "NULL"
		$selectContent = fetchContentList($form_state['CourseId'],$form_state['ClassId'],'cre_sys_obt_cls', 1);
		$totalRecords = $selectContent->countQuery()->execute()->fetchField();
		$status['totalrow'] = $totalRecords;
		return $status; 
	} catch (Exception $ex) {
		watchdog_exception('clearClassRosterEnrollUserAutocomplete', $ex);
		expertusErrorThrow($ex);
	} 
}

function updateContentClassDurationTime ($classId) {
  try {
   $updateStmt = db_update('slt_course_class');    
   $updateStmt->condition('id', $classId);
   $fields = array(
                  'scheduled_duration' => Null,
                  );     
   $updateStmt->fields($fields);
   $durationUpdated = $updateStmt->execute();
   } catch (Exception $ex) {
   	watchdog_exception('clearClassRosterEnrollUserAutocomplete', $ex);
   	expertusErrorThrow($ex);
   }
}

/*
 * sequenceAttachContent - Re-sequence the attached courses, returns list of used module ids
 */ 
function sequenceAttachContent($sequenceOrder) {
	$txn = db_transaction();
	try {
		$sequenceOrderArray = explode(',',$sequenceOrder);
		$sequence = 1;
		foreach($sequenceOrderArray as $mappingId){
			if(!empty($mappingId)){
				$tableName = 'slt_course_content_mapper';
				updateContentAttachSequence($tableName, $mappingId, $sequence);
				$sequence++;
			}
		}
	}
	catch (Exception $ex) {
		$txn->rollback();
		watchdog_exception('sequenceAttachContent', $ex);
		throw $ex;
	}
	unset($txn); // Commit txn
}

/*
 * updateContentAttachSequence - Sequence for Attach Question
 */ 
function updateContentAttachSequence($tableName, $mappingId, $sequence){
	try {
  $fields['sequence_number'] = $sequence;  
  $update = db_update($tableName);
  $update->fields($fields);
  $update->condition('id', $mappingId);
  $update->execute();
  } catch (Exception $ex) {
  	watchdog_exception('clearClassRosterEnrollUserAutocomplete', $ex);
  	expertusErrorThrow($ex);
  }
}

?>
