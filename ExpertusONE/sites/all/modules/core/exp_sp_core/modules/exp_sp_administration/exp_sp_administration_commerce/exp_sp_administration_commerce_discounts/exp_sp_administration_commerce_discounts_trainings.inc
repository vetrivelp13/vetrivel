<?php 

function displayTrainingsDiscountDetails(&$form, &$form_state, $entityId, $entityType, $uniqueId, $mode = ''){
 try{	
	expDebug::dPrint("touching training screen".$form_state['storage']['mode']);
  $availableTrng = fetchAlreadyAddedTrainings($entityId, $entityType);
  $isToBEAddedAvailable = fetchToBeAddedTrainings($entityId, $entityType);
  expDebug::dPrint('$availableTrng$availableTrng$availableTrng---->'.print_r($availableTrng,true));
  $uniqueId = "trainings-$entityId-$entityType";
  $wrapperId = 'trainings-grid-wrapper-'.$uniqueId;
  $formName = 'trainings_form_'.$uniqueId;
  $form_state['form_unique_name'] = $formName;
  global $theme_key;
  $filterSearchRgt = '';
  $prefixBgLeft = '';
  $filtersearchRight ='';
  $filterSearchRgt= '';
  if($theme_key == 'expertusoneV2'){
  	$prefixBgLeft   = '<div class="white-btn-bg-left"></div>';
  	$suffixBgRight = '<div class="white-btn-bg-right"></div>';
  	$cancelAttributes['class'][] = 'white-btn-bg-middle';
  	$filtersearchRight   = '<div class="filter-search-start-date-right-bg"></div>';  	
  }   
  $form['attr_action'] = array(
    '#type' => 'hidden',
    '#name' => 'attr_action',
    '#value' => 'trainings',
  ); 
  
  $setminHight = 'admin-course-tab-datagrid-wrapper';
  $msg = t('MSG660');
  
  $form[$formName] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="admin-data-grid"><div id="'.$wrapperId.'" class="admin-datagrid-pagination discount-grid-wrapper '.$setminHight.' admin-discounts-trainings-wrapper">',
    '#suffix' => '</div></div></div>',
  );  

  $form[$formName]['hidden_idlist'] = array(
    '#type' => 'hidden',
    '#name' => 'hidden_idlist_'.$uniqueId,
    '#id' => 'datagrid-idlist-'.$uniqueId,
  );  
  
  $displayMode = $form_state['storage']['mode'];
  expDebug::dPrint(' do you want to know what the display mode is.....???????????????'.$displayMode);
  //$priv_visible = adminVisibilityForAssign('discount'); 
	if(count($availableTrng) == 0 && empty($displayMode)){        
	    $addAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
	    $addAttributes['data-wrapperid'] = array($wrapperId);
	    $addAttributes['onclick'][]       = '$(document).ready(function(){ editAttributesDetailsView(); });';
 
	      if(count($isToBEAddedAvailable) == 0 ){   // || empty($priv_visible)    
		        $msg = t('MSG659');
		        $form[$formName]['add_trainings'] = array(
		          '#type' => 'markup',
		          '#prefix' => '<div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-greyout-bg"></div>',
		          '#suffix' => '<div class="admin-save-button-middle-greyout-bg">'.t('LBL1121').'</div><div class="admin-save-button-right-greyout-bg"></div></span></div>',
		        );                
		        $form[$formName]['add_trainings_text'] = array(
		          '#type' => 'markup',
		          '#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.$msg.'</div></div>',
		        );        
	      } else {          
		        $form[$formName]['add_trainings'] = array(
		          '#type' => 'submit',
		          '#prefix' => '<div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-bg"></div>',
		          '#suffix' => '<div class="admin-save-button-right-bg"></div></span></div>',
		          '#title' => t('LBL1121'),
		          '#name' => 'Add trainings',
		          '#value' => t('LBL1121'),
		          '#submit' => array('addtrainings'),
		          '#attributes' => $addAttributes,
		          '#ajax' => array(
		            'callback' => 'trainingsGrid',
		            'wrapper' => $wrapperId,
		            'event'	   => 'click' 
		          ),  
		        );    
		        $form[$formName]['add_trainings_text'] = array(
		          '#type' => 'markup',
		          '#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.$msg.'</div></div>',
		        );        
	      }      
	   // }  
  	}
  	/* else if training portion */
 	else {
	      $excludedCourseId = 0;
	      $displayMode = ($displayMode == "edit") ? $displayMode : "view";
      	  $form[$formName][] = array(
      		'#type' => 'markup',
      		'#markup' => '<div class="admin_add_multi_search_container">',
      	  );
      
      	  /*****************************************************************/
	      $form[$formName]['search_all_trainings_type-hidden'] = array(
		      '#type' => 'hidden',
		      '#name' => 'search_all_trainings_type-hidden',
		      '#attributes' => array('id' => 'search_all_trainings_type-hidden'),
	      	  '#value' => 'any',
	      );
	      /* select drop down for search by username and class title */
	      $paintMultiAction  = "<div id='search-list-title-keyword' class='search-list-keyword' style='display:block;'>";
	      $paintMultiAction .= "<span id='search-dropdwn-list'>";
	      //$paintMultiAction .= "<input type='hidden' id='search_all_moveuser_type-hidden' value='user' />";
	      $paintMultiAction .= "<input type='hidden' id='trainings-autocomplete_hidden' value='".t('LBL545')."' />";
	      $paintMultiAction .= "<span id='select-trainings-dropdown' class='select-trainings-dropdown'>".t('LBL428')."</span>";
	      $paintMultiAction .= "<a id='admin-dropdown-arrow' class='select-trainings-dropdown-link dropdown-link-font ' onclick='$(\"#select-trainings-dropdown-list\").slideToggle();$(\"#select-trainings-dropdown-list li:last\").css(\"border-bottom\",\"0px none\");'>&nbsp;</a>";
	              							
	      $paintMultiAction .= "<ul id='select-trainings-dropdown-list'>";
	      $searchTypeArr = array('any'=>t('LBL428'),'lrn_cls_dty_ilt'=>t('Classroom'),'lrn_cls_dty_wbt'=>t('Web-based'),'lrn_cls_dty_vod'=>t('Video'),'lrn_cls_dty_vcl'=>t('Virtual Class'),'cre_sys_obt_crt'=>t('Certification'),'cre_sys_obt_cur'=>t('Curricula'),'cre_sys_obt_trn'=>t('Learning Plan'));
	      foreach($searchTypeArr as $ele => $label){
	      	$delType = str_replace('\'','\u0027',t($label));
	      	$paintMultiAction .= "<li onclick=\"$('body').data('mulitselectdatagrid').trainingsSearchTypeText('".$delType."','".$ele."');\">".$label."</li>";
	      }
	       
	      $paintMultiAction .= "</ul>";
	      $paintMultiAction .= "</span></div>"; 
	     
	  	  $form[$formName]['more_add_trainings_search'] = array(
		      '#type' => 'markup',
		      '#markup' => $paintMultiAction,
	      );
	      /*****************************************************************/
	      $searchAjaxEvent = 'click';
	      $searchAttributes['onClick'][] = '$("body").data("mulitselectdatagrid").searchDataGrid("'.$displayMode.'", "trainings", $(\'#trainings-autocomplete\').val(), "'.$entityId.'", "'.$entityType.'", "'.$excludedCourseId.'"); return false;';
	    
	      
	      $acMenuPath = 'administration/commerce/discounts/'.$displayMode.'/get-trainings-autocomplete';
	      
	      $form[$formName]['add_trainings_search'] = array(
		      '#type' => 'markup',
		      '#markup' => '<div class="admin_add_multi_auto_search">',
	      );
      
	      $autoCompleteAttributes['class'][] = 'admin_ac_input_mainform';
	      $autoCompleteAttributes['onkeydown'][]  = '$("body").data("mulitselectdatagrid").multiSelectSearchBoxKeyDown(event)'; 
	      addACFieldForMultiSelect($form[$formName], 'trainings-autocomplete', $acMenuPath, '', '', '', array(), array('entity_id'  =>  $entityId,'search_type' => "$('#search_all_trainings_type-hidden').val()",'excluded_course_id' => "'".$excludedCourseId."'", 'id_required' => 0), false, false, t('LBL545') ,$autoCompleteAttributes);
	
	      $form[$formName]['add_trainings_search1'] = array(
		      '#type' => 'markup',
		      '#markup' => '</div>',
	      );
	      
	      $searchAttributes['Class'][] = 'admin-pagination-search-go';
	       $form[$formName]['search_trainings'] = array(
		        '#type' => 'submit',// '#src' => base_path().'sites/all/themes/core/expertusone/expertusone-internals/images/close.png',
		        '#title' => t('LBL304'),
		        '#name' => 'Search trainings',
		        '#submit' => array('search_trainings'),
		        '#attributes' => $searchAttributes,
		        '#ajax' => array(
		          'event' => $searchAjaxEvent,
		        ),  
	      );
	      $form[$formName][] = array(
		      '#type' => 'markup',
		      '#markup' => $filtersearchRight . '<div class="clearBoth"></div></div>',
	      );
    
	    $form[$formName]['trainings_view_grid_markup'] = array(
	      '#type' => 'markup',
	      '#markup' => '<div id="datagrid-div-'.$uniqueId.'" class="trainings-view-grid"><table id="datagrid-container-'.$uniqueId.'"></table><div id="pager-datagrid-'.$uniqueId.'" class="pager-datagrid-common"></div></div>',
	    );
	    
	    $form[$formName]['trainings_view_grid_load'] = array( 
	      '#type' => 'markup',
	      '#prefix' => '<div style="display:none"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-'.$uniqueId.'\'); $(\'body\').data(\'mulitselectdatagrid\').loadDataGrid(\''.$displayMode.'\', \'trainings\', \'\', \''.$entityId.'\', \''.$entityType.'\', \''.$excludedCourseId.'\');" width="100" height="132" /></div>',
	    );           
	    
	    if($displayMode == 'view'){
	      if($mode != 'complete_view'){
	        $addAnotherAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
	        $addAnotherAttributes['data-wrapperid'] = array($wrapperId);
	        //$addAnotherAttributes['onclick'][]       = '$(document).ready(function(){ editAttributesDetailsView(); });';
	        
	        //$listquery = fetchAlreadyAddedTrainings($entityId, $entityType);	        
	        if(count($isToBEAddedAvailable) != 0){	          
	          $form[$formName]['add_trainings_view_mode'] = array(
		          '#type' => 'submit',
		          '#title' => t('LBL1121'),
		      	  '#prefix' => '<div id= "datagrid-add-search-button-'.$uniqueId.'"  style="display:block;" class="admin-save-button-container admin-save-button-container-view-mode"><div class="admin-save-button-left-bg"></div>',
		  	      '#suffix' => '<div class="admin-save-button-right-bg"></div></div>',
		  	      '#value' => t('LBL1121'),
	              '#name' => 'Add trainings',
	              '#submit' => array('addtrainings'),
	              '#attributes' => $addAnotherAttributes,
	              '#ajax' => array(
		              'callback' => 'trainingsGrid',
		              'wrapper' => $wrapperId,
		              'event'	   => 'click'
	               ),  
	          );
	         /*$form[$formName]['add_trainings_view_mode_btn'] = array(
	            '#type' => 'submit',
	            '#title' => t('LBL365'),
	      	  '#prefix' => '<div style="display:none;"  id="datagrid-noresult-msg-'.$uniqueId.'"><div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-bg"></div>',
	  	      '#suffix' => '<div class="admin-save-button-right-bg"></div></span></div>',
	  	      '#value' => t('LBL3652222'),
	            '#name' => 'Add trainings',
	            '#submit' => array('addtrainings'),
	            '#attributes' => $addAnotherAttributes,
	            '#ajax' => array(
	              'callback' => 'trainingsGrid',
	              'wrapper' => $wrapperId,
	              'event'	   => 'click'
	            ),  
	          );*/
	        /*  $form[$formName]['trainings_view_grid_no_msg'] = array(
	            '#type' => 'markup',
	            '#markup' => '<div class="admin-empty-text-msg"> '.$msg.'</div></div></div>',
	          );*/
	          
	          
	        } else {
			 $form[$formName]['add_trainings_view_mode_btn'] = array(
	            '#type' => 'submit',
	            '#title' => t('LBL1121'),
	      	    '#prefix' => '<div style="display:none;"  id="add-training-hidden-btn-'.$uniqueId.'" class="admin-save-button-container admin-save-button-container-view-mode"><div class="admin-save-button-left-bg"></div>',
	  	        '#suffix' => '<div class="admin-save-button-right-bg"></div></div>',
	  	        '#value' => t('LBL1121'),
	            '#name' => 'Add trainings',
	            '#submit' => array('addtrainings'),
	            '#attributes' => $addAnotherAttributes,
	            '#ajax' => array(
		              'callback' => 'trainingsGrid',
		              'wrapper' => $wrapperId,
		              'event'	   => 'click'
	            ),  
	          );
	        }
	        
	      }
    } else if($displayMode == 'edit'){
        $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-action-button-middle-bg';
        $cancelAttributes['data-wrapperid'] = array($wrapperId);
        $form[$formName]['cancel_trainings_edit_mode'] = array(
	          '#type' => 'submit',
	          '#value' => t('LBL109'),
	          '#title' => t('LBL109'),
	          '#prefix' => '<div class="admin-datagrid-save-cancel-button-align-bottom"><div class="addedit-form-cancel-container-actions admin-save-button-container">'.$prefixBgLeft,
	          '#suffix' => $suffixBgRight,
	          '#name' => 'Cancel trainings',
	          '#submit' => array('cancelAddTrainings'),
	          '#attributes' => $cancelAttributes,
	          '#ajax' => array(
	            'callback' => 'trainingsGrid',
	            'wrapper' => $wrapperId,
	          ),  
        );
        
        if(count($isToBEAddedAvailable) != 0){
            $ajaxCallbackSave = 'addEditTrainingsRepaintForm';
            $setAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
            $setAttributes['data-wrapperid'] = array($wrapperId);
            $setAttributes['onclick'] = array();
	      	$setAttributes['onclick'][] = 'clearMessages();'; 
	      	$setAttributes['onclick'][] = 'increasePopWidth();';
	      	$form[$formName]['set_trainings_edit_mode'] = array(
		      	'#type' => 'submit',
		      	'#prefix' => '<div class="admin-save-pub-unpub-button-container"><div class="admin-save-button-left-bg"></div>',
		      	'#suffix' => '<span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span></div></div>'.$completionContainer.'<ul class="catalog-pub-add-list">',
		      	'#title' => t('LBL141'),
              	'#value' => t('LBL141'),
		      	'#name' => 'commonsave',
		      	'#attributes' => $setAttributes,
		          '#submit' => array('addDiscountsForTrainings'),
		          '#ajax' => array(
		  	      'wrapper' => $wrapperId,
		  	      'callback' => $ajaxCallbackSave,
		          'discount_id' => $entityId, 
		          'training_type' => 'save-selected',		          
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
		      $setAttributes = array();
 				$setAttributes['data-wrapperid'] = array($wrapperId);
 				 $setAttributes['onclick'] = array();
	      	$setAttributes['onclick'][] = 'clearMessages();'; 
	      	$setAttributes['onclick'][] = 'increasePopWidth();';
		       $form[$formName]['set_ilt_trainings'] = array(
			      '#type' => 'submit',
			      '#prefix' => '<li class="save-pub-unpub-sub-menu cancelledandsave">',
      			  '#suffix' => '</li>',
			      '#value' => t('LBL1114'),
			      '#name' => 'set all ilt',
			      '#attributes' => $setAttributes,
			      '#submit' => array('addDiscountsForTrainings'),
			      '#ajax' => array(
			          'wrapper' => $wrapperId,
			          'callback' => $ajaxCallbackSave,
			          'discount_id' => $entityId, 
		          	  'training_type' => 'lrn_cls_dty_ilt',
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
      
      $form[$formName]['set_vcl_trainings'] = array(
      '#type' => 'submit',
      '#prefix' => '<li class="save-pub-unpub-sub-menu cancelledandsave">',
      '#suffix' => '</li>',
      '#value' => t('LBL1115'),
      '#name' => 'set all vcl',
      '#attributes' => $setAttributes,
      '#submit' => array('addDiscountsForTrainings'),
      '#ajax' => array(
          'wrapper' => $wrapperId,
          'callback' => $ajaxCallbackSave,
          'discount_id' => $entityId, 
		  'training_type' => 'lrn_cls_dty_vcl',
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
      	 
      $form[$formName]['set_wbt_trainings'] = array(
      '#type' => 'submit',
      '#prefix' => '<li class="save-pub-unpub-sub-menu enrolledandsave">',
      '#suffix' => '</li>',
      '#value' => t('LBL1116'),
      '#name' => 'set all wbt',
      '#attributes' => $setAttributes,
      '#submit' => array('addDiscountsForTrainings'),
      '#ajax' => array(
          'wrapper' => $wrapperId,
          'callback' => $ajaxCallbackSave,
          'discount_id' => $entityId, 
		  'training_type' => 'lrn_cls_dty_wbt',
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
  
        
     	 
      $form[$formName]['set_vod_trainings'] = array(
      '#type' => 'submit',
      '#prefix' => '<li class="save-pub-unpub-sub-menu cancelledandsave">',
      '#suffix' => '</li>',
      '#value' => t('LBL1117'),
      '#name' => 'set all vod',
      '#attributes' => $setAttributes,
      '#submit' => array('addDiscountsForTrainings'),
      '#ajax' => array(
          'wrapper' => $wrapperId,
          'callback' => $ajaxCallbackSave,
          'discount_id' => $entityId, 
		  'training_type' => 'lrn_cls_dty_vod',
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
      
       
      
      $form[$formName]['set_crt_trainings'] = array(
      '#type' => 'submit',
      '#prefix' => '<li class="save-pub-unpub-sub-menu cancelledandsave">',
      '#suffix' => '</li>',
      '#value' => t('LBL1118'),
      '#name' => 'set all crt',
      '#attributes' => $setAttributes,
      '#submit' => array('addDiscountsForTrainings'),
      '#ajax' => array(
          'wrapper' => $wrapperId,
          'callback' => $ajaxCallbackSave,
          'discount_id' => $entityId, 
		  'training_type' => 'cre_sys_obt_crt',
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
      
      $form[$formName]['set_cur_trainings'] = array(
      '#type' => 'submit',
      '#prefix' => '<li class="save-pub-unpub-sub-menu cancelledandsave">',
      '#suffix' => '</li>',
      '#value' => t('LBL1119'),
      '#name' => 'set all cur',
      '#attributes' => $setAttributes,
      '#submit' => array('addDiscountsForTrainings'),
      '#ajax' => array(
          'wrapper' => $wrapperId,
          'callback' => $ajaxCallbackSave,
          'discount_id' => $entityId, 
		  'training_type' => 'cre_sys_obt_cur',
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
      
      $form[$formName]['set_trn_trainings'] = array(
      '#type' => 'submit',
      '#prefix' => '<li class="save-pub-unpub-sub-menu cancelledandsave">',
      '#suffix' => '</li>',
      '#value' => t('LBL1120'),
      '#name' => 'set all trn',
      '#attributes' => $setAttributes,
      '#submit' => array('addDiscountsForTrainings'),
      '#ajax' => array(
          'wrapper' => $wrapperId,
          'callback' => $ajaxCallbackSave,
          'discount_id' => $entityId, 
		  'training_type' => 'cre_sys_obt_trn',
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
      
  
        
  	$form[$formName]['trainings-btn-ul-end'] = array(
        '#type' => 'markup',
        '#markup' => '</ul>',
      );
        }
    }//end edit
    $form_state['storage']['mode'] = '';
 	}
 	}catch (Exception $ex) {
 		watchdog_exception('displayTrainingsDiscountDetails', $ex);
 		expertusErrorThrow($ex);
 	}
}

function addEditTrainingsRepaintForm($form, &$form_state) {
	try{
  expDebug::dPrint(' addEditTrainingsRepaintForm() called.');
  $form_state['storage']['mode'] = 'edit';
  
  $formName = $form_state['form_unique_name'];
  $entityId = $form_state['triggering_element']['#ajax']['entity_id'];
  $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
  $uniqueId = "trainings-$entityId-$entityType";
  
  $commands = array();
  //if($form_state['hidden-save-submit'] && (!empty($form_state['input']['hidden_idlist_'.$uniqueId]) || !empty($form_state['hidden_idlist_'.$uniqueId]))){
    $commands[] = ajax_command_replace(NULL, drupal_render($form[$formName]));
  //}
  $commands[] = exp_ctools_modal_adjust();
  
  $statusMsgHTML = theme('status_messages');
  expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true) , 4);
  if (!empty($statusMsgHTML)) {
    // Show the messages
    $commands[] = ajax_command_html('#show_expertus_message', $statusMsgHTML);
  }
  return array('#type' => 'ajax', '#commands' => $commands);
  }catch (Exception $ex) {
  	watchdog_exception('addEditTrainingsRepaintForm', $ex);
  	expertusErrorThrow($ex);
  }
}


function fetchToBeAddedTrainings($entityId, $entityType,$searchType ='',$searchKeyword = '', $queryRequired = '',$start, $limit){
	try{
  $alreadyAdded = array();
  $alreadyAdded = fetchAlreadyAddedTrainings($entityId,$entityType,'','','','edit');
  expDebug::dPrint('$alreadyAdded$alreadyAdded$alreadyAdded$alreadyAdded$alreadyAdded --->'.$searchType);
  $classQuery = db_select('taxonomy_term_data','td');
  $classQuery->join('taxonomy_index','tin','tin.tid = td.tid');
  $classQuery->join('slt_node_learning_activity','nodesl','tin.nid = nodesl.node_id and nodesl.entity_type =\'cre_sys_obt_cls\'');
  $classQuery->join('slt_course_class','cls','cls.id = nodesl.entity_id and cls.price > 0  and cls.id = td.description');
  $classQuery->leftJoin('slt_tag_entity','te','((cls.id=te.entity_id and te.entity_type=\'Class\') OR (cls.course_id=te.entity_id and te.entity_type=\'Course\'))');
  $classQuery->leftJoin('slt_tagdefn','tag','te.tagid= tag.id');
  $classQuery->distinct();
  $classQuery->addField('cls','id','entity_id');
  $classQuery->addField('cls','code','entity_code');
  $classQuery->addField('cls','title','entity_title');
  $classQuery->addField('td','tid','term_id');
  $classQuery->addField('nodesl','node_id','node_id');
  $classQuery->addField('cls','delivery_type','objtype');
 // $classQuery->addField('tag','tagname', 'tagname');
  $classQuery->condition('cls.status', 'lrn_cls_sts_atv');
  $classQuery->condition('td.vid', 3);
  $classQuery->condition('td.tid',explode(',',$alreadyAdded[0]->term_id),'not in');
  //$classQuery->groupBy('entity_id');
  filterByUserAccessPrivileges($classQuery, 'cls','',1,'cre_sys_obt_cls','attach','cls');  

  
  $tpQuery = db_select('taxonomy_term_data','td');
  $tpQuery->join('taxonomy_index','tin','tin.tid = td.tid');
  $tpQuery->join('slt_node_learning_activity','nodesl','tin.nid = nodesl.node_id and nodesl.entity_type in (\'cre_sys_obt_cur\',\'cre_sys_obt_trn\',\'cre_sys_obt_crt\')');
  $tpQuery->join('slt_program','prg','prg.id = nodesl.entity_id and prg.price > 0  and prg.id = td.description'); 
  $tpQuery->leftJoin('slt_tag_entity','te','((prg.id= te.entity_id AND (te.entity_type=\'Certification\' OR te.entity_type=\'Curricula\' OR te.entity_type=\'Learning Plan\') ))');
  $tpQuery->leftJoin('slt_tagdefn','tag','te.tagid= tag.id');
  $tpQuery->distinct();
  $tpQuery->addField('prg','id','entity_id');
  $tpQuery->addField('prg','code','entity_code');
  $tpQuery->addField('prg','title','entity_title');
  $tpQuery->addField('td','tid','term_id');
  $tpQuery->addField('nodesl','node_id','node_id');
  $tpQuery->addField('prg','object_type','objtype');
 // $tpQuery->addField('tag','tagname', 'tagname');
  $tpQuery->condition('prg.status', 'lrn_lpn_sts_atv');
  $tpQuery->condition('td.vid', 5);
  $tpQuery->condition('td.tid',explode(',',$alreadyAdded[1]->term_id) ,'not in');
  //$tpQuery->groupBy('entity_id');
  filterByUserAccessPrivileges($tpQuery, 'prg','',1,'','attach','prg');  
  if($searchKeyword != t('LBL545')){
	 	$classQuery->condition(db_or()
  			->condition('cls.title', '%' . db_like($searchKeyword) . '%', 'LIKE')
  			->condition('cls.code', '%' . db_like($searchKeyword) . '%', 'LIKE')
			->condition('tag.tagname', '%' . db_like($searchKeyword) . '%', 'LIKE')
 		);
   		$tpQuery->condition(db_or()
  			->condition('prg.title', '%' . db_like($searchKeyword) . '%', 'LIKE')
  			->condition('prg.code', '%' . db_like($searchKeyword) . '%', 'LIKE')
			->condition('tag.tagname', '%' . db_like($searchKeyword) . '%', 'LIKE')
  		);
  }
  
  if(in_array($searchType,array('lrn_cls_dty_ilt','lrn_cls_dty_wbt','lrn_cls_dty_vcl','lrn_cls_dty_vod'))){
  	if(($start > 0) || ($limit > 0)) {
          $classQuery->range($start,$limit);
    }
  	$finalQuery = $classQuery->condition('cls.delivery_type', $searchType); 
  	
  }
  if(in_array($searchType,array('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn'))){
  	if(($start > 0) || ($limit > 0)) {
          $tpQuery->range($start,$limit);
    }
  	$finalQuery = $tpQuery->condition('prg.object_type', $searchType); 
  }
  if($searchType == "any" || empty($searchType)){
  	if(($start > 0) || ($limit > 0)) {
          $tpQuery->range($start,$limit);
    }
  	$finalQuery = $classQuery->union($tpQuery,'UNION ALL'); 
  }
  
  if(empty($queryRequired)){
    $result = $finalQuery->execute()->fetchAll();
  	expDebug::dPrintDBAPI('To be added trainings',$finalQuery);
    return $result;
  } else {
    return $finalQuery;
  }
  }catch (Exception $ex) {
  	watchdog_exception('fetchToBeAddedTrainings', $ex);
  	expertusErrorThrow($ex);
  }
}

function fetchAlreadyAddedTrainings($entityId, $entityType='',$searchType = '',$searchKeyword = '', $queryRequired = '',$mode = '',$start='', $limit=''){
	try{
	$classQuery = db_select('uc_discounts_terms','dis_terms');  
	$classQuery->join('taxonomy_index','tax_index','tax_index.tid = dis_terms.term_id');
	$classQuery->join('taxonomy_term_data','term_data','term_data.vid = 3');
	$classQuery->join('slt_node_learning_activity','node_learning','tax_index.nid = node_learning.node_id and node_learning.entity_type =\'cre_sys_obt_cls\'');
	$classQuery->join('slt_course_class','cls','cls.id = node_learning.entity_id and cls.price > 0 and cls.status = \'lrn_cls_sts_atv\' and cls.id = term_data.description');
	$classQuery->leftJoin('slt_tag_entity','te','((cls.id=te.entity_id and te.entity_type=\'Class\') OR (cls.course_id=te.entity_id and te.entity_type=\'Course\'))');
	$classQuery->leftJoin('slt_tagdefn','tag','te.tagid= tag.id');
	if($mode == "edit"){
		$classQuery->addExpression('GROUP_CONCAT(term_data.tid)', 'term_id');
	}else{
		$classQuery->distinct();
		$classQuery->addField('cls','id','entity_id');
		$classQuery->addField('cls','code','entity_code');
		$classQuery->addField('cls','title','entity_title');
		$classQuery->addField('term_data','tid','term_id');
		$classQuery->addField('node_learning','node_id','node_id');
		$classQuery->addField('cls','delivery_type','objtype');
		//$classQuery->addField('tag','tagname', 'tagname');
		
	}
	$classQuery->condition('dis_terms.discount_id',$entityId,'=');
	
 
	$tpQuery = db_select('uc_discounts_terms','dis_terms');  
	$tpQuery->join('taxonomy_index','tax_index','tax_index.tid = dis_terms.term_id');	
	$tpQuery->join('taxonomy_term_data','term_data','term_data.vid = 5');
	$tpQuery->join('slt_node_learning_activity','node_learning','tax_index.nid = node_learning.node_id and node_learning.entity_type in (\'cre_sys_obt_cur\',\'cre_sys_obt_trn\',\'cre_sys_obt_crt\')');
	$tpQuery->join('slt_program','prg','prg.id = node_learning.entity_id and prg.price > 0 and prg.status = \'lrn_lpn_sts_atv\' and prg.id = term_data.description');
	$tpQuery->leftJoin('slt_tag_entity','te','((prg.id= te.entity_id AND (te.entity_type=\'Certification\' OR te.entity_type=\'Curricula\' OR te.entity_type=\'Learning Plan\') ))');
    $tpQuery->leftJoin('slt_tagdefn','tag','te.tagid= tag.id');
	if($mode == "edit"){
		$tpQuery->addExpression('GROUP_CONCAT(term_data.tid)', 'term_id');
	}else{	
		$tpQuery->distinct();
		$tpQuery->addField('prg','id','entity_id');
		$tpQuery->addField('prg','code','entity_code');
		$tpQuery->addField('prg','title','entity_title');
		$tpQuery->addField('term_data','tid','term_id');
		$tpQuery->addField('node_learning','node_id','node_id');
		$tpQuery->addField('prg','object_type','objtype');
		//$tpQuery->addField('tag','tagname', 'tagname');
		
	}
	$tpQuery->condition('dis_terms.discount_id',$entityId,'=');
	if($searchKeyword != t('LBL545')){
		 $classQuery->condition(db_or()
  			->condition('cls.title', '%' . db_like($searchKeyword) . '%', 'LIKE')
  			->condition('cls.code', '%' . db_like($searchKeyword) . '%', 'LIKE')
			->condition('tag.tagname', '%' . db_like($searchKeyword) . '%', 'LIKE')
  		);
    	$tpQuery->condition(db_or()
  			->condition('prg.title', '%' . db_like($searchKeyword) . '%', 'LIKE')
  			->condition('prg.code', '%' . db_like($searchKeyword) . '%', 'LIKE')
			->condition('tag.tagname', '%' . db_like($searchKeyword) . '%', 'LIKE')
  		);
	}
	expDebug::dPrint('$searchType$searchType$searchType$searchType$searchType$searchType --> '.$searchType);
    if(in_array($searchType,array('lrn_cls_dty_ilt','lrn_cls_dty_wbt','lrn_cls_dty_vcl','lrn_cls_dty_vod'))){
      if(($start > 0) || ($limit > 0)) {
          $classQuery->range($start,$limit);
      }
  	  $finalQuery = $classQuery->condition('cls.delivery_type', $searchType); 
    }
    if(in_array($searchType,array('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn'))){
      if(($start > 0) || ($limit > 0)) {
          $tpQuery->range($start,$limit);
      }
  	  $finalQuery = $tpQuery->condition('prg.object_type', $searchType); 
    }
    if($searchType == "any" || empty($searchType)){
      if(($start > 0) || ($limit > 0)) {
          $tpQuery->range($start,$limit);
      }
  	  $finalQuery = $classQuery->union($tpQuery,'UNION ALL'); 
    } 
   // expDebug::dPrintDBAPI(' WE HAVE TRAINING ADDED are into viw mode --> ',$finalQuery);
    if(empty($queryRequired)){
    	expDebug::dPrint(' indide execuuuttteee');
    	return $finalQuery->execute()->fetchAll();
    } else {
    	return $finalQuery;
    }
    }catch (Exception $ex) {
    	watchdog_exception('fetchAlreadyAddedTrainings', $ex);
    	expertusErrorThrow($ex);
    }
}


function getTrainingsAutocomplete($mode){	
	try{
		expDebug::dPrint('getTrainingsAutocomplet getTrainingsAutocomplete'.$mode);
		$matchingQuery = array();
		$entity_id   = $_GET['entity_id'];    
		$nameSubstr = $_GET['z'];
		$searchType = $_GET['search_type'];
		if($mode == "view"){
			$matchingQuery = fetchAlreadyAddedTrainings($entity_id, '',$searchType,$nameSubstr, '');
		}else{
			$matchingQuery = fetchToBeAddedTrainings($entity_id, '',$searchType,$nameSubstr, '');			
		}
		expDebug::dPrint('getTrainingsAutocomplete $matchingQuery$matchingQuery$matchingQuery ' .print_r($matchingQuery,true));
	    foreach ($matchingQuery as $result) {
	    	 print $result->entity_title . "\n";
	    }
	}catch (Exception $ex) {
	    watchdog_exception('getTrainingsAutocomplete', $ex);
	    expertusErrorThrow($ex);
  	}

}

function deleteDiscountTrainings($entityId, $entityType, $associateId){
	$txn = db_transaction();
  	try {
		$takeTermIdAll = db_select('taxonomy_term_data','term_data');  
		$takeTermIdAll->addField('term_data', 'tid');
		$takeTermIdAll->condition('term_data.description', $entityType);
		$taxTidAll = $takeTermIdAll->execute()->fetchAll();
		
		$checkIfAllInDiscounts = db_select('uc_discounts_terms','dis_terms');  
		$checkIfAllInDiscounts->addField('dis_terms', 'term_id');
		$checkIfAllInDiscounts->condition('dis_terms.term_id', $taxTidAll[0]->tid);
		$checkIfAllInDiscounts->condition('dis_terms.discount_id', $entityId);
		$ifTypeAll = $checkIfAllInDiscounts->execute()->fetchAll();
		
		if(count($ifTypeAll) > 0){
		    $deleteTrain = db_delete('uc_discounts_terms');  
		    $deleteTrain->condition('discount_id', $entityId);
		    $deleteTrain->condition('term_id', $taxTidAll[0]->tid);
		    $deleteId = $deleteTrain->execute();
		    
		    $deleteFormState = array();
		    $deleteFormState['id'] = $deleteId;
		    make_audit_trail_entries($deleteFormState, getIdOfLoggedInUser() , null ,$entityId, 'Deleted','Commerce',
		  											'exp_sp_administration_commerce_discounts','Deleting entry for all '.$entityType,array('id'));  
			
			if(in_array($entityType,array('lrn_cls_dty_ilt','lrn_cls_dty_wbt','lrn_cls_dty_vcl','lrn_cls_dty_vod'))){
		  	  
			  $insertTerms = db_select('taxonomy_term_data','td');
			  $insertTerms->join('taxonomy_index','tin','tin.tid = td.tid');
			  $insertTerms->join('slt_node_learning_activity','nodesl','tin.nid = nodesl.node_id and nodesl.entity_type =\'cre_sys_obt_cls\'');
			  $insertTerms->join('slt_course_class','cls','cls.id = nodesl.entity_id and cls.price > 0  and cls.id = td.description');
			  $insertTerms->addField('td','tid','term_id_insert');
			  $insertTerms->condition('cls.status', 'lrn_cls_sts_atv');
			  $insertTerms->condition('td.vid', 3);
			  $insertTerms->condition('td.tid',$associateId,'<>');
			  $insertTerms->condition('cls.delivery_type', $entityType);
			}
			if(in_array($entityType,array('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn'))){  
			  $insertTerms = db_select('taxonomy_term_data','td');
			  $insertTerms->join('taxonomy_index','tin','tin.tid = td.tid');
			  $insertTerms->join('slt_node_learning_activity','nodesl','tin.nid = nodesl.node_id and nodesl.entity_type in (\'cre_sys_obt_cur\',\'cre_sys_obt_trn\',\'cre_sys_obt_crt\')');
			  $insertTerms->join('slt_program','prg','prg.id = nodesl.entity_id and prg.price > 0  and prg.id = td.description'); 
			  $insertTerms->addField('td','tid','term_id_insert');
			  $insertTerms->condition('prg.status', 'lrn_lpn_sts_atv');
			  $insertTerms->condition('td.vid', 5);
			  $insertTerms->condition('td.tid',$associateId,'<>');
			  $insertTerms->condition('prg.object_type', $entityType); 
			}	
			$insIds = $insertTerms->execute()->fetchAll();	
			foreach($insIds as $insTerms){
			   $insertDiscountTerm  	= db_insert('uc_discounts_terms');   
			   $fields = array(               
			            'discount_id'  => $entityId,
			   			'term_id'	   => $insTerms->term_id_insert,
			           
			          );
			   $insertDiscountTerm->fields($fields);
			   $DisTermId 	= $insertDiscountTerm->execute();
			   add_audit_trail_entry(getIdOfLoggedInUser(), $entityId, 'Commerce' , 'exp_sp_administration_commerce_discounts', 'Discounts', 'Added training', null, null , $insTerms->term_id_insert);
			}
			
		}else{
			$deleteTrain = db_delete('uc_discounts_terms');  
			$deleteTrain->condition('discount_id', $entityId);
			$deleteTrain->condition('term_id', $associateId);
			$deleteId = $deleteTrain->execute();
			$deleteFormState = array();
		    $deleteFormState['id'] = $deleteId;
		    make_audit_trail_entries($deleteFormState, getIdOfLoggedInUser() , null ,$entityId, 'Deleted','Commerce',
		  											'exp_sp_administration_commerce_discounts','Traning for Discount',array('id'));  
		}
  	}catch (Exception $ex) {
	    $txn->rollback();
	    watchdog_exception('deleteDiscountTrainings', $ex);
	    throw $ex;
  	}

  	unset($txn);
	
	
}

?>