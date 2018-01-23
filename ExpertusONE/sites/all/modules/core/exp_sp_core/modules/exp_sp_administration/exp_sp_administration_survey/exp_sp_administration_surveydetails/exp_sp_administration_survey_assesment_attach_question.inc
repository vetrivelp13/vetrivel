<?php 
function displayAttachQuestionsDetails(&$form,&$form_state,$entityId, $entityType, $mode=""){
	try{
	 global $theme_key;
   $form = array();
   $formName = 'attach-question';
   $uniqueId = 'SurAssAttachQuestion-'.$entityId.'-'.$entityType;
   $wrapperId = 'surass-grid-wrapper-'.$uniqueId;
   $customHiddenButtonId = 'equv-save-esign';
   expDebug::dPrint('displayAttachCourseDetails Form = ' . print_r($form, true) , 4);
   if(empty($form)){
    	$form = array(
   			'#type' => 'markup',
   			/* '#prefix' => (( stripos($_SERVER['HTTP_USER_AGENT'], 'Chrome')  || stripos($_SERVER['HTTP_USER_AGENT'], 'Firefox'))? '</form>' : '' ), */
    		'#prefix' =>  '</form>',
   	); 
   }

   $setminHight = ($entityType=='sry_det_typ_sry' || $entityType=='sry_det_typ_ass') ? 'admin-attach-survey-assessment-question-tab-datagrid-wrapper' : '';
   
   expDebug::dPrint('$setminHight $setminHight $setminHight : '.print_r($entityType,true) , 4);  
  /* Ticket NO: 0046992 */
   $paintMultiAction  = "<div id='search-list-surass-title-keyword' class='search-list-keyword' style='display:block;'>";
   $paintMultiAction .= "<span id='search-dropdwn-list-surass'>";
   $paintMultiAction .= "<input type='hidden' id='tagsearch_all_surass_type-hidden' value='surassqus' />";
   $paintMultiAction .= "<input type='hidden' id='surassattachquestion-autocomplete-tag_hidden' value=' ' />";
   $paintMultiAction .= "<span id='select-list-surass-dropdown-tag' class='select-list-dropdown-surass'>".t('LBL325')."</span>";
   $paintMultiAction .= "<a id='admin-dropdown-arrow' class='select-list-surass-dropdown-link dropdown-link-font ' onclick='$(\"#root-admin\").data(\"narrowsearch\").moreSurAssSearchHideShowTag();'> </a>";
   $paintMultiAction .= "<ul id='select-list-surass-dropdown-list-tag'>";
   $paintMultiAction .= "<li onclick=\"$('#root-admin').data('narrowsearch').moreSurAssSearchTypeTextTag('".t('LBL325')."','surassqus');\">".t('LBL325')."</li>";
   $paintMultiAction .= "<li onclick=\"$('#root-admin').data('narrowsearch').moreSurAssSearchTypeTextTag('".t('LBL191')."','surasstagname');\">".t('LBL191')."</li>";
   $paintMultiAction .= "</ul>";
   $paintMultiAction .= "</span></div>";
  $form[$formName] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="admin-add-questions-survey-assessment"><div id="'.$wrapperId.'" class="admin-datagrid-pagination '.$setminHight.'">',
    '#suffix' => '</div></div>',
  );
  
  $form[$formName][] = array(
  		'#type' => 'markup',
  		'#markup' => '<div class="admin_add_multi_search_container">',
  );
     
     $form[$formName][] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_search_container_pop">',
      );
     
     $form[$formName]['add_type_search_start'] = array(
     		'#type' => 'markup',
     		'#markup' => $paintMultiAction. '</div>',
     );    /* Ticket NO: 0046992 */
        
    $form[$formName]['hidden_idlist'] = array(
      '#type' => 'hidden',
      '#name' => 'hidden_idlist_'.$uniqueId,
      '#id' => 'datagrid-idlist-'.$uniqueId,
    );
     
      
      $attchQuestionDetails= attachedQuestionsId($entityId);
      $questionIdArr = array();
      $excludedSurveyId = 0;
      if(count($attchQuestionDetails) > 0){
        foreach($attchQuestionDetails as $details){
          $questionIdArr[] = $details->question_id;
        }
      }
      //$excludedSurveyId = '';
      if(count($questionIdArr)>0) {
      $excludedSurveyId = implode(',', $questionIdArr);

      $excludedSurveyId = $excludedSurveyId == "" ? 0 : $excludedSurveyId;
      }
    
      $searchAjaxEvent = 'click';
  /* Ticket NO: 0046992 */
      $searchAttributes['onClick'][] = '$("body").data("mulitselectdatagrid").searchDataGrid("edit", "SurAssAttachQuestion", $(\'#surassattachquestion-autocomplete-tag\').val(), "'.$entityId.'", "'.$entityType.'", "'.$excludedSurveyId.'"); return false;';
      if($entityType == 'sry_det_typ_ass'){
        $acMenuPath = 'administration/learning/assessment/survey-assesment-autocomplete';
      }
      else{
        $acMenuPath = 'administration/learning/survey/survey-assesment-autocomplete';
      }
      
      $form[$formName]['add_attachquestion_search_start'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_auto_search">',
      );
      
      $autoCompleteAttributes['class'][] = 'admin_ac_input_mainform';
      $autoCompleteAttributes['onkeydown'][]  = '$("body").data("mulitselectdatagrid").multiSelectSearchBoxKeyDown(event)';
      addACFieldForMultiSelect($form[$formName], 'surassattachquestion-autocomplete-tag', $acMenuPath, '', '', '', array(), array('excluded_question_id' => "'".$excludedSurveyId."'", 'id_required' => 0,'entity_type' => "'".$entityType."'",'search_type' => "$('#tagsearch_all_surass_type-hidden').val()"), false, false, t('LBL703') ,$autoCompleteAttributes);
  /* Ticket NO: 0046992 */
      $form[$formName]['add_attachquestion_search_end'] = array(
      '#type' => 'markup',
      '#markup' => '</div>',
      );
      
      $searchAttributes['Class'][] = 'admin-pagination-search-go';
       $form[$formName]['search_attachquestion'] = array(
        '#type' => 'submit',
        '#title' => t('LBL304'),
        '#name' => 'Search attachquestion',
        '#submit' => array('search_attachquestion'),
        '#attributes' => $searchAttributes,
        '#ajax' => array(
          'event' => $searchAjaxEvent,
        ),  
      );
			$filterSuffix = '';
      if($theme_key == 'expertusoneV2') {
      	$filterSuffix = '<div class="filter-search-start-date-right-bg"></div>';
      }
      $form[$formName][] = array(
      '#type' => 'markup',
      '#markup' => $filterSuffix . '</div>',
      );
      
    $form[$formName]['attachquestion_view_grid_markup'] = array(
      '#type' => 'markup',
      '#markup' => '<div id="datagrid-div-'.$uniqueId.'"><table id="datagrid-container-'.$uniqueId.'" class="datagrid-container-common"></table><div id="pager-datagrid-'.$uniqueId.'" class="pager-datagrid-common"></div></div>',
    );

    
    $form[$formName]['attachquestion_view_grid_load'] = array( 
      '#type' => 'markup',
      '#prefix' => '<div style="display:none"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-'.$uniqueId.'\'); $(\'body\').data(\'mulitselectdatagrid\').loadDataGrid(\'edit\', \'SurAssAttachQuestion\', \'\', \''.$entityId.'\', \''.$entityType.'\', \''.$excludedSurveyId.'\');" width="100" height="132" /></div>',
    );

        $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-action-button-middle-bg';
        $cancelAttributes['onclick']         = "closeQtip('','$cancelButtonId');  return false;";
        $cancelAttributes['data-wrapperid'] = array($wrapperId);
        $prfixPgLeft = '';
			  $suffixPgRight = '';
			  if($theme_key == 'expertusoneV2'){
			  	$prfixPgLeft   = '<div class="white-btn-bg-left"></div>';
			  	$suffixPgRight = '<div class="white-btn-bg-right"></div>';
			  	$cancelAttributes['class'][] = 'white-btn-bg-middle';
			  } 
    
        $form[$formName]['cancel_attachquestion_edit_mode'] = array(
        		'#type' => 'submit',
        		'#value' => t('LBL123'),
        		'#title' => t('LBL123'),
        		'#prefix' => '<div class="admin-datagrid-save-cancel-button-align-bottom"><div class="addedit-form-cancel-container-actions admin-save-button-container">' . $prfixPgLeft,
        		'#suffix' => $suffixPgRight,
        		'#name' => 'Cancel attachquestion',
        		'#attributes' => $cancelAttributes,
        );

        $listquery = loadAttachQuestionForSurvAss('', '', $excludedSurveyId,$entityType);
        
        if(count($listquery) != 0){

            $setAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg attach-list-view-attach-btn';
            $setAttributes['data-wrapperid'] = 'survey_attach_questions_dt';
            
            $form[$formName]['set_attachquestion_edit_mode'] = array(
              '#type' => 'submit',
              '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
    	      '#suffix' => '<div class="admin-save-button-right-bg"></div></div></div>',
              '#title' => t('Attach Question'),
              '#value' => t('LBL701'),
              '#name' => $buttonName,
              '#submit' => array('attachQuestionDetailsList'),
              '#attributes' => $setAttributes,
              '#ajax' => array(
        		    'wrapper' => 'survey_attach_questions_dt',
        		    'callback' => 'attachQuestionCallBack',
                    'survey_id' =>$entityId,
                    'entity_type' =>$entityType,            
              	      'method' => 'replace',
            	      'effect' => 'none',
            	      'event'=>'click',
            	      'keypress' => true,
            	      'progress' => array(
            	        'type' => 'throbber',
            	        'message' => ''
            	      ),
               ),  
            );
        }
        
     $form[$formName][] = array(
      '#type' => 'markup',
      '#markup' => '</div>',
      );        

  return $form;
  } catch (Exception $ex) {
  	watchdog_exception('displayAttachQuestionsDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function attachQuestionCallBack($form, &$form_state){
  try{
  	include_once(drupal_get_path('module', 'exp_sp_administration_surveydetails') .'/exp_sp_administration_survey_assesment_question.inc');
  $entityId = $form_state['triggering_element']['#ajax']['survey_id'];
  $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
   
  $min_form_state = array(
   'ajax' => TRUE, 
   're_render' => TRUE, 
   'no_redirect' => TRUE, 
   'survey_id' => $entityId,
   'build_info' => array('args' => array('ajax')) 
  );
  
  $form_state = $min_form_state;

  expDebug::dPrint('attachQuestionCallBack formstateee : '.print_r($form_state,true) , 5);
  $statusMsgHTML = theme('status_messages');
  expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true) , 4);
  if (!empty($statusMsgHTML)) {
    $commands[] = ajax_command_html('#bubble-face-table #show_expertus_message', $statusMsgHTML);
  }
  else{
	  $formId = 'exp_sp_administration_survey_assesment_question_form';
	  $builtForm = drupal_build_form($formId, $form_state);
	
	  $commands = array();  
	  $commands[] = ajax_command_replace(NULL, drupal_render($builtForm));
	  $commands[] = ajax_command_invoke(NULL, "attachClose", array($entityId, $entityType));
	  $commands[] = ajax_command_refresh_narrow_search_results('onclose');
  }
  return array(
        '#type' => 'ajax',
        '#commands' => $commands
  );
  } catch (Exception $ex) {
  	watchdog_exception('attachQuestionCallBack', $ex);
  	expertusErrorThrow($ex);
  }
}
function loadAttachQuestionForSurvAss($searchKeyword, $queryRequired = '', $excludedId = '',$entityType,$type=''){
	try {
  $select  = db_select('slt_survey_questions', 'surveyQstion');
  $select->distinct();
  $select->addField('surveyQstion', 'id', 'id');
  $select->addField('surveyQstion', 'code', 'question_code');
  $select->addField('surveyQstion', 'question_txt', 'question_txt');
  if ($entityType == 'sry_det_typ_ass') {
  	$entityCode = 'sry_det_typ_ass_qus';
  } else {
  	$entityCode = 'sry_det_typ_sry_qus';
  }
  expDebug::dPrintDBAPI('loadAttachQuestionForSurvAss $type = ' , $type);
  /* Ticket NO: 0046992 */
  if($type=='surasstagname'){
  	expDebug::dPrintDBAPI('LBL193 $type = ' , t('LBL193'));
  	if($searchKeyword != t('LBL193')){
	  	$select->innerJoin('slt_tag_entity', 'ste', 'ste.entity_id = surveyQstion.id');
	  	$select->innerJoin('slt_tagdefn', 'stf', 'stf.id = ste.tagid'); 
	  	$select->condition('stf.tagname', '%' . db_like($searchKeyword) . '%', 'LIKE');
	  	//$select->condition('ste.entity_type', $entityCode, '=');
  	}  
  	/* Ticket NO: 0046992 */
  }else{
  	expDebug::dPrintDBAPI('loadAttachQuestionForSurvAss $type = ' , t('LBL703')."--".t('LBL324'));
  	if($searchKeyword != t('LBL703')){
  		if($searchKeyword != t('LBL324')) {
  	
	  $select->condition('surveyQstion.question_txt', '%' . db_like($searchKeyword) . '%', 'LIKE');
  		}
  }
  }
  $select->condition('surveyQstion.status', 'sry_qtn_sts_atv', '=');
  $select->condition('surveyQstion.survey_type', $entityType);
  
 
 
  //$select->condition('surveyQstion.question_type', array('sry_qtn_typ_dpn','sry_qtn_typ_mch','sry_qtn_typ_trf','sry_qtn_typ_yno','sry_qtn_typ_cmt','sry_qtn_typ_rtg') , 'IN');
  
  if(!empty($excludedId)){
   $select->condition('surveyQstion.id', explode(",",$excludedId) , 'NOT IN');  
  }
  expDebug::dPrintDBAPI('loadAttachQuestionForSurvAss $type TAG123 = ' , $select);
  filterByUserAccessPrivileges($select,'surveyQstion','',1,$entityCode,'attach','surveyQstion'); // #46933 - group access visiblity check
  //$select->orderBy('question_txt');
  if(empty($queryRequired)){
    return $select->execute()->fetchAll();
  } else {
    return $select;
  }
  } catch (Exception $ex) {
  	watchdog_exception('loadAttachQuestionForSurvAss', $ex);
  	expertusErrorThrow($ex);
  }
}

function attachQuestionDetailsList($form, &$form_state) {  

  $txn = db_transaction();
  
  try {
    $entityId = $form_state['entity_id'];
    $entityType = $form_state['entity_type'];
    $uniqueId = "SurAssAttachQuestion-$entityId-$entityType";
    $groupId  = updateGroupForNewAttachQuestion('group1',$entityId);
    $currentUserId = getIdOfLoggedInUser();
    if($entityType == 'sry_det_typ_ass'){
      $mandatoryOption = 'Y';
    }else{
      $mandatoryOption = 'N';
    }    
    
    if(!empty($form_state['input']['hidden_idlist_'.$uniqueId]) || !empty($form_state['hidden_idlist_'.$uniqueId])){
      $userId = getSltpersonUserId();

        if(!empty($form_state['input']['hidden_idlist_'.$uniqueId]))
          $associateIdListArray = explode(',', $form_state['input']['hidden_idlist_'.$uniqueId]);
        else
          $associateIdListArray = explode(',', $form_state['hidden_idlist_'.$uniqueId]);
    	$idArray = array();    
    	$sequenceNo   =    checkMaxSequenceNumber($entityId,$groupId); 
        foreach($associateIdListArray as $associatedEntityId){
            
         $checkTp      =    checkingSurAssAssingnedQuestions($associatedEntityId, $entityId);
         $sequenceNo++;
         if($entityType=='sry_det_typ_ass')
            $score=0;
           else 
             $score=NULL;
            if($checkTp == 0) {
                $insertQuestinoMapping = db_insert('slt_survey_groups_questions');
             	$fields = array(
                            'survey_id'       => $entityId,
                            'survey_group_id' => $groupId,
                            'question_id'     => $associatedEntityId,  
             	            	'mandatoryOption' => $mandatoryOption,
			             					'score'  	        => $score,
             	              'sequence'  	    => $sequenceNo,
                            'created_by'      => $currentUserId,
                            'created_on'      => now(),
             								'updated_by'      => $currentUserId,
             								'updated_on'      => now(),
                          );
                          
            	$insertQuestinoMapping->fields($fields); 
            	$insertQuestinoMapping->execute();
            	make_audit_trail_entries($fields, getIdOfLoggedInUser() , null, $associatedEntityId ,'Inserted',$entityType,'exp_sp_administration_survey','attachQuestionDetailsList',
                                          array('survey_id', 'survey_group_id','mandatoryOption','question_id', 'score'));
    
            }
          
        }
          $form_state['rebuild'] = TRUE;
      
      } 
  }   catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('attachQuestionDetailsList', $ex);
    throw $ex;    
  }
  
  unset($txn);
 
}

function checkingSurAssAssingnedQuestions($questionid, $surveyid){
  try{
  $select = db_select('slt_survey_groups_questions', 'mapping');   
  $select->addExpression('COUNT(mapping.id)', 'count');
  // Set the conditions
  $select->condition('mapping.question_id', $questionid, '=');
  $select->condition('mapping.survey_id', $surveyid, '=');
  // Execute the query
  $questionCount = $select->execute()->fetchField();
  return $questionCount; // a valid new name
  } catch (Exception $ex) {
  	watchdog_exception('checkingSurAssAssingnedQuestions', $ex);
  	expertusErrorThrow($ex);
  }
}

function attachedQuestionsId($entity_id){
	try{
  $select = db_select("slt_survey_groups_questions","sgq");
  $select->addField('sgq','question_id');
  $select->condition('sgq.survey_id',$entity_id);
  return $select->execute()->fetchAll();
  } catch (Exception $ex) {
  	watchdog_exception('attachedQuestionsId', $ex);
  	expertusErrorThrow($ex);
  }
}

function SurvAssAttachedQuestions($survey_id,$group_question_id,$start = 0,$end = 0, $surAssQuestionName, $questionSearchType) {
 try{ 
  $select = db_select("slt_survey_groups_questions","sgq");
  $select->innerJoin('slt_survey', 'sy', 'sy.id = sgq.survey_id');
  $select->innerJoin('slt_survey_questions', 'qst', 'qst.id = sgq.question_id');
  $select->leftjoin('slt_survey_groups', 'sg', 'sg.id = sgq.survey_group_id');
  $select->innerJoin('slt_profile_list_items', 'prof', 'prof.code = qst.status');
  $select->innerJoin('slt_profile_list_items', 'prof1', 'prof1.code = qst.question_type');
  $select->addField('qst','question_txt','question_txt');
  $select->addField('qst','id','survey_question_id'); 
  $select->addField('qst','code','question_code');
  $select->addField('sgq','id','survey_group_question_id');
  $select->addField('sgq','sequence','survey_group_sequence');
  $select->addField('sgq','mandatoryOption','mandatory_option');
  $select->addField('sgq','score','survey_score');
  $select->addField('sgq','id','mapping_id');
  $select->addField('sg','id','survey_group_id');
  $select->addField('sg','title','survey_group_title');     
  $select->addField('prof','name','question_status'); 
  $select->addField('prof1','name','question_type');
  $select->addField('sy','random','random_type');
/*  if($group_question_id>0) {
    $select->condition('sgq.id',$group_question_id,'=');
  }   */
  if(!empty($surAssQuestionName)&& ($surAssQuestionName!=t('LBL324') && $questionSearchType=='surassqus' ) ){
     $select->condition('qst.question_txt', '%' . db_like($surAssQuestionName) . '%', 'LIKE');
   }
  elseif(!empty($surAssQuestionName) && $surAssQuestionName!= t('LBL036'). ' '.t('Group') &&  $questionSearchType=='surassgrpname'){
       $select->condition('sg.title', '%' . db_like($surAssQuestionName) . '%', 'LIKE');
   }
  $select->condition('sgq.survey_id',$survey_id,'=');
  $select->condition('qst.status','sry_qtn_sts_atv','=');
  if(($start > 0) || ($end > 0)) {
    $select->range($start,$end);
  }
  
  $select->orderBy('sgq.sequence');
  $select->orderBy('qst.id');

  $surveyGroupList = $select->execute()->fetchAll();
   expDebug::dPrintDBAPI(' $select object = ' , $select);
   //expDebug::dPrintDBAPI($survey_id.' == group_question_id :'.$group_question_id.' || surveyGorupList 123434444 :'.$select);
  expDebug::dPrint($survey_id.' == group_question_id :'.$group_question_id.' || surveyGorupList 123434444 :'.print_r($surveyGroupList,true) , 3);
  
  return $surveyGroupList;
  } catch (Exception $ex) {
  	watchdog_exception('SurvAssAttachedQuestions', $ex);
  	expertusErrorThrow($ex);
  }
}

function SurvAssAttachedQuestions_OLD($survey_id,$group_question_id) {
 try{ 
  $select = db_select("slt_survey_groups_questions","sgq");
  $select->innerJoin('slt_survey', 'sy', 'sy.id = sgq.survey_id');
  $select->innerJoin('slt_survey_questions', 'qst', 'qst.id = sgq.question_id');
  $select->leftjoin('slt_survey_groups', 'sg', 'sg.id = sgq.survey_group_id');
  $select->innerJoin('slt_profile_list_items', 'prof', 'prof.code = qst.status');
  $select->innerJoin('slt_profile_list_items', 'prof1', 'prof1.code = qst.question_type');
  $select->addField('qst','question_txt','question_txt');
  $select->addField('qst','id','survey_question_id'); 
  $select->addField('qst','code','question_code');
  $select->addField('sgq','id','survey_group_question_id');
  $select->addField('sgq','sequence','survey_group_sequence');
  $select->addField('sgq','mandatoryOption','mandatory_option');
  $select->addField('sgq','score','survey_score');
  $select->addField('sgq','id','mapping_id');
  $select->addField('sg','id','survey_group_id');
  $select->addField('sg','title','survey_group_title');     
  $select->addField('prof','name','question_status'); 
  $select->addField('prof1','name','question_type');
/*  if($group_question_id>0) {
    $select->condition('sgq.id',$group_question_id,'=');
  }   */
  $select->condition('sgq.survey_id',$survey_id,'=');
  $select->condition('qst.status','sry_qtn_sts_atv','=');

  $select->orderBy('sgq.sequence');
  
  $surveyGroupList = $select->execute()->fetchAll();
  expDebug::dPrintDBAPI($survey_id.' == group_question_id :'.$group_question_id.' || surveyGorupList 123434444 :'.$select);
  
  expDebug::dPrint($survey_id.' == group_question_id :'.$group_question_id.' || surveyGorupList 123434444 :'.print_r($surveyGroupList,true) , 3);
  
  return $surveyGroupList;
  } catch (Exception $ex) {
  	watchdog_exception('SurvAssAttachedQuestions_OLD', $ex);
  	expertusErrorThrow($ex);
  }
}


function attachedquestion_group_update($entityId,$groupName,$questionId) { 
 
  $txn = db_transaction();

  try {
    include_once(drupal_get_path('module', 'exp_sp_administration_surveydetails') .'/exp_sp_administration_survey_assesment_question.inc');
    $random_num=rand(1,99999);
    $auto_code="grpco_".$random_num;
    
    //$truncatedGroupName     =  titleController('EXP-SP-ADMIN-SURVEY-ASSESSMENT-ATTACH-QSTN-GROUPNAME', $groupName,15);
    $truncatedGroupName     =  $groupName;
    $checkGroupNameExist = checkGroupNameExist($groupName);
              
    if(empty($checkGroupNameExist["id"])) {             
   
          $insertStmt  	 = db_insert('slt_survey_groups');
          $loggedInUserId  = getIdOfLoggedInUser();
          $field_change1   = 'created_by';
          $field_change2   = 'created_on';   
          $custom = NULL;    
          $fields          = array(                
              			  		'survey_id'          => $entityId,                  
                          'title'              => $groupName,
                          'code'		           => $auto_code,
                          'status'             => 'sry_det_grp_atv',                
                          $field_change1       => $loggedInUserId,
                          $field_change2       => now(),
          								'updated_by'         => $loggedInUserId,
          								'updated_on'         => now(),
                          'custom0'            => $custom,
                          'custom1'            => $custom,
                          'custom2'            => $custom,
                          'custom3'            => $custom,
                          'custom4'            => $custom,             
                    );
          
          $insertStmt->fields($fields);
           
          $surveyGroupId	= $insertStmt->execute();
          make_audit_trail_entries($fields, getIdOfLoggedInUser() , null, $entityId ,'Inserted','sry_det_grp','exp_sp_administration_survey','attachedquestion_group_update',
                                          array('survey_id', 'title','code','status'));
    
    }else{
          $surveyGroupId   = $checkGroupNameExist["id"];
    }
    
    if(!empty($surveyGroupId)) {  
    	    $updStmt = db_update('slt_survey_groups_questions');
            $custom  = NULL;
            	
            $fields = array(
                        'survey_group_id'     => $surveyGroupId,
                        'updated_by'         => 1,
                        'updated_on'         => now()
                      );
            $updStmt->fields($fields);	
            // Added orgValidate by Vincent for #0050212 on June 9, 2015
            $updStmt->condition('survey_id',orgValidate($entityId),'=');
            $updStmt->condition('question_id',orgValidate($questionId),'=');
            expDebug::dPrintDBAPI("Update survey qroup qry -- > ",$updStmt) ;           
            $attDefIdUpd = $updStmt->execute(); 
			
    }
    print $truncatedGroupName . '|' . $groupName;
  } catch (Exception $ex) {
    
    $txn->rollback();
    watchdog_exception('attachedquestion_group_update', $ex);
    throw $ex;
  }
  
  // Commit the transaction
  unset($txn);
}

function attachedquestion_score_update($entityId,$questionId,$scoreValue){
	try{
  $default = db_query("SELECT score FROM slt_survey_groups_questions WHERE survey_id = :sid and question_id = :qid", array(':sid' =>$entityId, ':qid' => $questionId))->fetchAll();
  $updStmt = db_update('slt_survey_groups_questions');
  $custom  = NULL;
  	
  $fields = array(
              'score'     => $scoreValue,
              'updated_by'         => 1,
              'updated_on'         => now()
            );
  $updStmt->fields($fields);	
  $updStmt->condition('survey_id',$entityId,'=');
  $updStmt->condition('question_id',$questionId,'=');
  		            
  $attDefIdUpd = $updStmt->execute(); 
  make_audit_trail_entries($fields, getIdOfLoggedInUser() , $default, $entityId ,'Updated','sry_det_typ_ass','exp_sp_administration_survey_assessment_attach_question','attachedquestion_score_update',
                                          array('score'));
  } catch (Exception $ex) {
  	watchdog_exception('attachedquestion_score_update', $ex);
  	expertusErrorThrow($ex);
  } 
}

function attachedquestion_mandatory_update($listValues){
  
  $listVal         = explode('-',$listValues);
  $entityId        = $listVal[0];
  $entityType      = $listVal[1];
  $questionId      = $listVal[2];  
  $mandatoryOption = $listVal[3];
  
  $txn = db_transaction();
  $default = db_query("SELECT mandatoryoption FROM slt_survey_groups_questions WHERE survey_id = :sid and question_id = :qid", array(':sid' =>$entityId, ':qid' => $questionId))->fetchAll();
  
  try {
  
  $updStmt = db_update('slt_survey_groups_questions');
  $custom  = NULL;
  	
  $fields = array(
              'mandatoryoption'     => $mandatoryOption,
              'updated_by'         => 1,
              'updated_on'         => now()
            );
            
  $updStmt->fields($fields);	
  $updStmt->condition('survey_id',$entityId,'=');
  $updStmt->condition('question_id',$questionId,'=');
  		            
  $attDefIdUpd = $updStmt->execute(); 
  make_audit_trail_entries($fields, getIdOfLoggedInUser() , $default, $entityId ,'Updated',$entityType,'exp_sp_administration_survey_assessment_attach_question','attachedquestion_mandatory_update',
                                          array('mandatoryoption'));
    
  } catch (Exception $ex) {
    
    $txn->rollback();
    watchdog_exception('attachedquestion_mandatory_update', $ex);
    throw $ex;
  }
  
  // Commit the transaction
  unset($txn);
            
}

/*
 * sequenceAttachQuestion - Sequence for Attach Question
 */ 
function sequenceAttachQuestion($sequenceOrder){
	try {
  $sequenceOrderArray = explode(',',$sequenceOrder);
  $sequence = 1;
  foreach($sequenceOrderArray as $sequenceOrder){
    if(!empty($sequenceOrder)){
      list($mappingId, $recertify) = explode('-', $sequenceOrder);
      $tableName = 'slt_survey_groups_questions';
      updateAttachQuestionSequence($tableName, $mappingId, $sequence);
      $sequence++;
    }
  }
  } catch (Exception $ex) {
  	watchdog_exception('sequenceAttachQuestion', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * updateAttachQuestionSequence - Sequence for Attach Question
 */ 
function updateAttachQuestionSequence($tableName, $mappingId, $sequence){
	try{
  $fields['sequence'] = $sequence;
  $fields['updated_on'] = now();
  $userId = getSltpersonUserId();
  
  $update = db_update($tableName);
  $update->fields($fields);
  $update->condition('id', $mappingId);
  $update->execute();
  make_audit_trail_entries($fields, $userId,$moduleId,$mappingId, 'Sequencing', 'Sequence' , 'exp_sp_administration_survey_assessment_attach_questions', 'updateAttachQuestionSequence',array('sequence'));
  } catch (Exception $ex) {
  	watchdog_exception('updateAttachQuestionSequence', $ex);
  	expertusErrorThrow($ex);
  }
}



function updateGroupForNewAttachQuestion($groupName,$entityId) { 
 
  $txn = db_transaction();

  try {
    
    $random_num=rand(1,99999);
    $auto_code="grpco_".$random_num;
    
    $checkGroupNameExist = checkGroupNameExist($groupName);
              
    if(empty($checkGroupNameExist["id"])) {             
   
          $insertStmt  	 = db_insert('slt_survey_groups');
          $loggedInUserId  = getIdOfLoggedInUser();
          $field_change1   = 'created_by';
          $field_change2   = 'created_on';   
          $custom = NULL;    
          $fields          = array(                
              			      'survey_id'          => $entityId,                  
                          'title'              => $groupName,
                          'code'		           => $auto_code,
                          'status'             => 'sry_det_grp_atv',                
                          $field_change1       => $loggedInUserId,
                          $field_change2       => now(),
          								'updated_by'         => $loggedInUserId,
          								'updated_on'         => now(),
                          'custom0'            => $custom,
                          'custom1'            => $custom,
                          'custom2'            => $custom,
                          'custom3'            => $custom,
                          'custom4'            => $custom,             
                    );
          
          $insertStmt->fields($fields);
           
          $surveyGroupId	= $insertStmt->execute();
    }else{
          $surveyGroupId   = $checkGroupNameExist["id"];
    }
    
    return $surveyGroupId;
    
  } catch (Exception $ex) {
    
    $txn->rollback();
    watchdog_exception('updateGroupForNewAttachQuestion', $ex);
    throw $ex;
  }
  
  // Commit the transaction
  unset($txn);
}

function checkMaxSequenceNumber($surveyId, $groupId){
	try {
  $select = db_select("slt_survey_groups_questions","sgq");
  $select->addExpression('Max(sgq.sequence)', 'maxsequence');
  $select->condition('sgq.survey_id', $surveyId, '=');
  $select->condition('sgq.survey_group_id', $groupId, '=');
  $maxSequence = $select->execute()->fetchField();
  expDebug::dPrintDBAPI('Max Sequence No= ' , $select);
  if($maxSequence > 0)
  $maxSequence;
  else
  $maxSequence=0;
  return $maxSequence; // a valid new name
  } catch (Exception $ex) {
  	watchdog_exception('checkMaxSequenceNumber', $ex);
  	expertusErrorThrow($ex);
  }
}
?>