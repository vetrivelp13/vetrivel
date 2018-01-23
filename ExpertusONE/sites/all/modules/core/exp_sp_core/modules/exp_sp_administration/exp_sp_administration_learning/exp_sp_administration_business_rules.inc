<?php

/*
 * _getBusinessRulesList() - Get all Business rules except Manager Mark Complete and Instructor Mark Complete 
 */
function _getBusinessRulesList($entityId, $entityType, $deliveryType){
	try {
  $query = db_select('slt_profile_list_items', 'items');
  $query->addField('items', 'code');
  $query->addField('items', 'name');
  
  $query->condition('items.code', array('cre_sys_brl_imc', 'cre_sys_brl_mmc'), 'NOT IN');
  $query->condition('items.code', 'cre_sys_brl_%', 'like');
  $query->condition('items.is_active', 'Y', '=');
  
  if($deliveryType != 'lrn_cls_dty_vod' && $deliveryType != 'lrn_cls_dty_wbt'){
    $query->condition('items.code', array('cre_sys_brl_rra'), 'NOT IN');
  }
  
  $result = $query->execute()->fetchAll();
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('_getBusinessRulesList', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * _getBusinessRulesMapping() - Get Mapped Business Rules for the Entity Id and Entity Type  
 */
function _getBusinessRulesMapping($entityId, $entityType){
  try {
  $query = db_select('slt_business_rule_mapping', 'rules');
  $query->addField('rules', 'business_rule_code');
  $query->condition('rules.entity_id', $entityId);
  $query->condition('rules.entity_type', $entityType);
  $result = $query->execute()->fetchAll();
  $ruleCodeArray = array();
  foreach($result as $ruleCode){
    $ruleCodeArray[] = $ruleCode->business_rule_code; 
  }
  return $ruleCodeArray;
  } catch (Exception $ex) {
  	watchdog_exception('_getBusinessRulesMapping', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * _updateBusinessRules() - Updated the Business Rules for the Entity Id and Entity Type  
 */
function updateBusinessRules($entityId, $entityType, $uniqueid, &$form_state){
  
  $txn = db_transaction();
  try{
    
    $updateStatus = 0;
    expDebug::dPrint('exp_sp_administration_business_rules.inc - business rules list - All123 : ' .print_r($entityId, true),5);
    expDebug::dPrint('exp_sp_administration_business_rules.inc - business rules list - Already Mapped123 : ' .print_r($entityType, true),4);
    if($entityType == 'cre_sys_obt_cls'){
    	$upd = db_update('slt_course_class');
    	$fields = array('updated_on'=> now() , 'updated_by'=> $userid);
    	$upd->fields($fields);
    	$upd->condition('id',$entityId,'=');
    	expDebug::dPrintDBAPI("Update entity sql >>>>>>>>>." , $upd);
    	$upd->execute();
    }
    // Business Rules form values  
    $businessRulesAll = $form_state['input']['business_rules_fieldset']['business_rules_hidden'];
    $businessRulesSelected = $form_state['input']['attach_business_rules'.$uniqueid];
    $businessRulesSelected = empty($businessRulesSelected) ? array() : $businessRulesSelected;
    
    // Get Mapped Business Rules for the Entity Id and Entity Type  
    $businessRulesMapping = _getBusinessRulesMapping($entityId, $entityType);
    
    expDebug::dPrint('exp_sp_administration_business_rules.inc - business rules list - All : ' .print_r($businessRulesAll, true),5);
    expDebug::dPrint('exp_sp_administration_business_rules.inc - business rules list - Selected : ' .print_r($businessRulesSelected, true), 5);
    expDebug::dPrint('exp_sp_administration_business_rules.inc - business rules list - Already Mapped : ' .print_r($businessRulesMapping, true),4);
    
    // Get the Business Rules needs to be deleted
    $deleteRulesList = array_diff($businessRulesMapping, $businessRulesSelected);
    
    expDebug::dPrint('exp_sp_administration_business_rules.inc - business rules list - Deleted rules : ' .print_r($deleteRulesList, true),5);
    
    // Delete the Business Rules for the Entity Id and Entity Type 
    if(count($deleteRulesList)) {
      _deleteBusinessRules($entityId, $entityType, $deleteRulesList);
      $updateStatus = 1;
    }
    
    // Add all the newly selected Business Rules 
    foreach($businessRulesSelected as $businessRuleCode){
      if(!in_array($businessRuleCode, $businessRulesMapping)){
        _addBusinessRules($entityId, $entityType, $businessRuleCode);
        $updateStatus = 1;
      }
    }
    
    /*if($updateStatus){
      drupal_set_message('Business Rules updated sucessfully.');
    }*/
    
  } catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('updateBusinessRules', $ex);
    throw $ex;
  }
  
  // Commit the transaction
  unset($txn);
  
}

/*
 * _addBusinessRules() - Add the Business Rules for the Entity Id and Entity Type  
 */
function _addBusinessRules($entityId, $entityType, $businessRuleCode){
  try {
  // User ID
  $userId = getIdOfLoggedInUser();
  
  // Fields to be inserted
  $fields['entity_id'] = $entityId;
  $fields['entity_type'] = $entityType;
  $fields['business_rule_code'] = $businessRuleCode;
  $fields['created_by'] = $userId;
  $fields['created_on'] = now();
  $fields['updated_by'] = $userId;
  $fields['updated_on'] = now();
  
  // Insert Query
  $query = db_insert('slt_business_rule_mapping');
  $query->fields($fields);
  $query->execute();
  
  // Audit log entry
  if($businessRuleCode == "cre_sys_brl_dtc"){
  		  $updateQuery = db_update('slt_course_class');
  		  $updateQuery->fields(array(
                        'dedicated_class_flag'      => 'Y',                                                   
                        'updated_on'    => now(),
                        'updated_by'    => $userId
                        ));
		  $updateQuery->condition('id', $entityId);
		  $updateQuery->execute();
  }
  make_audit_trail_entries($fields, getIdOfLoggedInUser() , null, $entityId , 'Inserted', $entityType, 'exp_sp_administration_business_rules', '_addBusinessRules',
                                          array('business_rule_code'));
  } catch (Exception $ex) {
  	watchdog_exception('_addBusinessRules', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * _deleteBusinessRules() - Delete the Business Rules for the Entity Id and Entity Type  
 */
function _deleteBusinessRules($entityId, $entityType, $deleteRulesList){
	try {
  $userId = getIdOfLoggedInUser();
  
  $select=db_select('slt_business_rule_mapping','bus');
  $select->addField('bus','id','id');
  $select->addField('bus','business_rule_code','p2');
  $select->addField('bus','entity_id','p1');
  $select->addField('bus','entity_type','entity_type');
  $select->condition('business_rule_code', $deleteRulesList, 'IN');
  $select1=$select->execute()->fetchAll();

  expDebug::dPrint('$select business6'.print_r($select,true),3);
  
  $deleteQuery = db_delete('slt_business_rule_mapping');
  $deleteQuery->condition('entity_id', $entityId);
  $deleteQuery->condition('entity_type', $entityType);
  $deleteQuery->condition('business_rule_code', $deleteRulesList, 'IN');
  $deleteQuery->execute();
  
  foreach($select1 as $val)
  {
      $fields= array(
              'table_name' => 'slt_business_rule_mapping',
              'entity_id' => $val->id,
              'entity_type' => 'business_rule_mapping',
              'parent1_entity_id' => $val->p1,
              'parent1_entity_type' => $val->entity_type,
              'parent2_entity_type' => $val->p2,
              'module_name' => 'exp_sp_administration_business_rules',
              'functionality_name' => 'deleteBusinessRules',
              'api_name' => $_REQUEST['apiname'],
              'deleted_on' => now(),
              'deleted_by' => $userId,        	
      );
      deleted_log_entry($fields);
  }
  // Audit log entry
  foreach($deleteRulesList as $rule){
  	if($rule == "cre_sys_brl_dtc"){
  		  $updateQuery = db_update('slt_course_class');
  		  $updateQuery->fields(array(
                        'dedicated_class_flag'      => 'N',                                                   
                        'updated_on'    => now(),
                        'updated_by'    => $userId
                        ));
		  $updateQuery->condition('id', $entityId);
		  $updateQuery->execute();
  	}
  	$fields['business_rule_code'] = $rule;
  	make_audit_trail_entries($fields, getIdOfLoggedInUser() , null, $entityId ,'Deleted', $entityType, 'exp_sp_administration_business_rules', '_deleteBusinessRules',
	                                          array('entity_id', 'entity_type', 'business_rule_code'));
  }
  } catch (Exception $ex) {
  	watchdog_exception('_deleteBusinessRules', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * Display the Business Rules for the Entity Id and Entity Type
 */
function displayBusinessRulesToAddEditForm(&$form, $form_state, $entityId, $entityType, $uniqueid, $deliveryType, $readonly = FALSE){
  try {
  // All available Business rules except Manager Mark Complete and Instructor Mark Complete 
  $businessRulesList  = _getBusinessRulesList($entityId, $entityType, $deliveryType);
  
  // Get Mapped Business Rules for the Entity Id and Entity Type
  $businessRulesMapping = _getBusinessRulesMapping($entityId, $entityType);

  
  
  $form['attr_action'] = array(
      '#type' => 'hidden',
      '#name' => 'attr_action',
      '#value' => 'business_rules',
  );  
  
  $readOnlyType = $readonly == true ? 'disabled="disabled"' : '';
  
  if(!empty($businessRulesList)){

  	$form['business_rules_table_start'] = array(
        '#type' => 'markup',
     );  
     
     $form['business_rules_fieldset'] = array(
        '#type' => 'markup',
        '#prefix' => '<div id="business-rules-fieldset-wrapper" class="admin-tab-list-row">',
        '#suffix' => '</div><table width="98%" cellpadding="4" cellspacing="0" border="0"><tr><td class="businessrules-table-title">Business Rules</td><td class="businessrules-table-title businessrules-add-border">Select</td></tr>',
     );  

    $i=0;
    foreach($businessRulesList as $key => $val){
      
      // Already mapped rules should be checked
      $checkedStatus = in_array($val->code, $businessRulesMapping) ? 'checked' : '';
      
      addTwoColumnRowToAddEditForm($form, 'business_rules_frequencylabel_'.$val->code);
      
      $form['business_rules_table_row'.$i] = array(
        '#type' => 'markup',
        '#prefix' => '<tr><td class="businessrules-table-value">'.$val->name.'</td><td class="businessrules-table-value businessrules-add-border"><input type="checkbox" '.$readOnlyType.' name="attach_business_rules'.$uniqueid.'[]" value="'.$val->code.'" '.$checkedStatus.'>',
        '#suffix' => '</td></tr>',
      ); 

      $form['business_rules_fieldset']['business_rules_hidden'][$val->code] = array('#type' => 'hidden', '#value' => $val->code);
      
      endAddEditTwoColumnRow($form);

      $i++;
      
    }
    
    $form['business_rules_table_end'] = array(
        '#type' => 'markup',
        '#suffix' => '</table></td></tr></table>',
     );  
    
  } else {
    
      $form['no-records-msg'] = array(
      	'#type' => 'markup',
      	'#markup' => '<div class="no-records-msg">'.t('There are no business rules available for this class.').'</div></div>'
      );
  }
  } catch (Exception $ex) {
  	watchdog_exception('displayBusinessRulesToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
} 

// New Business Rule Start From  

function exp_sp_administration_class_bussiness_rule_form($entityId, $entityType, $deliveryType, $emptyId){
 try {
  $form_state = array(
   'no_redirect' => TRUE, // To confirm whether necessary
   'build_info' => array('args' => array('ajax')), // The form builder function expects the first argument to be ajax or nojs.
   'entity_id' => $entityId,
   'entity_type' => $entityType,
   'delivery_type' => $deliveryType,
   'empty_id' => $emptyId   
  );      
  
  $formId = 'exp_sp_administration_class_bussiness_rule_form_html';
  $builtForm = drupal_build_form($formId, $form_state);
  $renderedForm = drupal_render($builtForm);

  $drupalSettings = getDrupalSettings();

  $businessRulesResponse = array(
    'drupal_settings' => $drupalSettings,
    'render_content_main' => $renderedForm,
  ); 
 
  echo drupal_json_output($businessRulesResponse);
  exit;
  } catch (Exception $ex) {
  	watchdog_exception('exp_sp_administration_class_bussiness_rule_form', $ex);
  	expertusErrorThrow($ex);
  }
}

function exp_sp_administration_class_bussiness_rule_form_html($form, &$form_state, $renderMode) {
  try {
  include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_business_rules.inc');  
  $css_theme_optional = array('type' => 'file', 'group' => CSS_THEME);
  global $theme_key;
  	if($theme_key == 'expertusoneV2') {
  		//NEWUI THEME STYLE SHEET
  		drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_v2.css', $css_theme_optional);
  	} else {
		drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration.css', $css_theme_optional);  		
  	}
  //drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration.css', $css_theme_optional);  
  expDebug::dPrint('Testing arguament valiue'.print_r($form_state,true),5);

  $entityId      = $form_state['entity_id'];
  $entityType    = $form_state['entity_type'];
  $deliveryType  = $form_state['delivery_type'];
  $uniqueid      = $entityId.'-'.$entityType; 
   
  $dynamicwrapperid="businessrule-addedit-form".$form_state['entity_id'].$form_state['entity_type'];
  $popuprenderBusinessId = "popup_container_qtip_buisness_disp_".$form_state['entity_id'].'_'.$form_state['entity_type'];
  $popupDisplayId				 = "qtipBusinessqtip_visible_disp_".$form_state['entity_id'].'_'.$form_state['entity_type'];
  $form = businessRuleNewAddEditFormWithCancelAndSave($dynamicwrapperid);
  
  addHiddenValueToAddEditForm($form, 'entity_id', $form_state['entity_id']);
  addHiddenValueToAddEditForm($form, 'entity_type', $form_state['entity_type']);
  addHiddenValueToAddEditForm($form, 'empty_id', $form_state['empty_id']);
  addHiddenValueToAddEditForm($form, 'delivery_type', $form_state['delivery_type']);
  
 // All available Business rules except Manager Mark Complete and Instructor Mark Complete 
  $businessRulesList  = _getBusinessRulesList($entityId, $entityType, $deliveryType);
  
  // Get Mapped Business Rules for the Entity Id and Entity Type
  $businessRulesMapping = _getBusinessRulesMapping($entityId, $entityType);
  
  $form['attr_action'] = array(
      '#type' => 'hidden',
      '#name' => 'attr_action',
      '#value' => 'business_rules',
  );  
  
  $readOnlyType = $readonly == true ? 'disabled="disabled"' : '';
  
  if(!empty($businessRulesList)){

  	$form['business_rules_table_start'] = array(
        '#type' => 'markup',
     );  
     
     $form['business_rules_fieldset'] = array(
        '#type' => 'markup',
        '#prefix' => '<table width="98%" id="business-rules-table" cellpadding="4" cellspacing="0" border="0"><tr><td colspan="2" class="info-text">'.t(MSG510).'</td></tr><div id="business-rules-fieldset-wrapper" class="admin-tab-list-row">',
        '#suffix' => '</div>',
     );  

    $i=0;
    foreach($businessRulesList as $key => $val){
      
      // Already mapped rules should be checked
      $checkedStatus = in_array($val->code, $businessRulesMapping) ? 'checked' : '';
      //Class Name Checked
      /*--#38292 - issue fix--*/
      $CheckedClassName = in_array($val->code, $businessRulesMapping) ? 'checkbox-selected' : 'checkbox-unselected';
      //addTwoColumnRowToAddEditForm($form, 'business_rules_frequencylabel_'.$val->code);
      
      $form['business_rules_table_row'.$i] = array(
        '#type' => 'markup',
        '#prefix' => '<tr><td class="brl-column-left"><span class="vtip" title="'.sanitize_data(t($val->name)).'"> '.titleController('ADDEDIT-CLASS-BUSINESS-RULES-NAME',t($val->name),45).'</span></td><td class="brl-column-right"><div class="'.$CheckedClassName.'"><input class="brl-checkbox-input" id="business-rule-'.$val->code.'" type="checkbox" '.$readOnlyType.' name="attach_business_rules'.$uniqueid.'[]" onclick="checkboxSelectedUnselectedCommon(this);" value="'.$val->code.'" '.$checkedStatus.'></div>',
        '#suffix' => '</td></tr>',
      ); 

      $form['business_rules_fieldset']['business_rules_hidden'][$val->code] = array('#type' => 'hidden', '#value' => $val->code);
      
      //endAddEditTwoColumnRow($form);

      $i++;
      
    }
    
    $form['business_rules_table_end'] = array(
        '#type' => 'markup',
        '#suffix' => '</table>',
     );  
    
  } else {
    
      $form['no-records-msg'] = array(
      	'#type' => 'markup',
      	'#markup' => '<div class="no-records-msg">'.t('MSG608').'</div></div>'
      );
  }
  
  
 businessRuleAddEditFormWithCancelAndSaveClose($form,$dynamicwrapperid,'newtheme-save-button','newtheme-cancel-link',$popuprenderBusinessId,$popupDisplayId);
 
  return $form;
  } catch (Exception $ex) {
  	watchdog_exception('exp_sp_administration_class_bussiness_rule_form_html', $ex);
  	expertusErrorThrow($ex);
  }
  
}
        
function exp_sp_administration_class_bussiness_rule_form_html_submit ($form, &$form_state){
  try {
   expDebug::dPrint(":Calling business form : ".print_r($form_state,true),5);
   $entityId      = $form_state['entity_id'];
   $entityType    = $form_state['entity_type'];
   $deliveryType  = $form_state['delivery_type'];
   $uniqueid      = $entityId.'-'.$entityType;
   updateBusinessRules($entityId, $entityType, $uniqueid, $form_state);
   syncSolrData('Class');
   $form_state['rebuild'] = TRUE; 
   } catch (Exception $ex) {
   	watchdog_exception('exp_sp_administration_class_bussiness_rule_form_html_submit', $ex);
   	expertusErrorThrow($ex);
   }
  
}

/*
 * certificatenewAddEditFormWithCancelAndSave() - Use this function to create your addedit form having Cancel and Save buttons at the bottom 
 */
function businessRuleNewAddEditFormWithCancelAndSave($wrapperId = 'addedit-form-wrapper', $miscAction = array()) {
  try {
  $form = array();
  //Added by priya for nested form closing #0036495
  $form['#prefix'] = /* (( stripos($_SERVER['HTTP_USER_AGENT'], 'Chrome') || stripos($_SERVER['HTTP_USER_AGENT'], 'Firefox') )? '</form>' : '' ) */ '</form>'.'<div id="' . $wrapperId . '" class="addedit-form-wrapper">';
  //$form['#prefix'] = '<div id="' . $wrapperId . '" class="addedit-form-wrapper">';
  $form['#suffix'] = '</div>';  
  
  // Add a class to the form so that we can apply custom style (margin, padding, etc).  
  $form['#attributes'] = array('class' => array('addedit-form'));
  $form['#attributes']['autocomplete'] = 'off';
  $form['#tree'] = TRUE; // Show full path of values (hierarchical values)
  
  $form['addedit-left-col-start'] = array(
    '#type' => 'markup',
    '#markup' => '<div>',
  );

      
  return $form;
  } catch (Exception $ex) {
  	watchdog_exception('businessRuleNewAddEditFormWithCancelAndSave', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * certificateAddEditFormWithCancelAndSaveClose() - Closure for newAddEditFormWithCancelAndSave()
 */
function businessRuleAddEditFormWithCancelAndSaveClose(&$form, $wrapperId = '', $saveButtonId = 'save-button', $cancelButtonId = 'cancel-button', $popuprenderCertsId = "",$popupDisplayId='', $expandPopup = FALSE, $saveTabIndex = '', $esignature=false) {
	try {
  $customHiddenButtonId = 'esign-button-'.$wrapperId;

  // Remove the temporary storage for API's to exchange information during form creation
  unset($form['addedit_tmp_storage']);
  
  if (isset($saveTabIndex) && $saveTabIndex > 0) {
    $saveAttributes['tabindex'] = $saveTabIndex;
    $cancelAttributes['tabindex'] = $saveTabIndex+1;
  }
  
  $ajaxCallbackCancel = 'addEditCancelRepaintForm';
  $cancelAttributes['class'] = array('addedit-edit-' . $cancelButtonId); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
  
  
  // Using this class to applying bg color to the save button
  $cancelAttributes['class'][] = 'admin-action-button-middle-bg white-btn-bg-middle';

  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-bubble-close';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  // Also used by multiselect to show input as selected for only the current form
  $cancelAttributes['data-wrapperid'] = array($wrapperId);
  
  // A common class for all addedit form cancel button, for styling to appear as a link.
  $cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';
  $cancelAttributes['class'][] = 'tags-close-button';
  $cancelAttributes['onclick'] = "closeQtip('','$cancelButtonId');  return false;";
  
  
  $form[$cancelButtonId] = array(
    '#type' => 'submit',
    '#prefix' => '<div class="addedit-form-cancel-and-certificate-done-actions-row certificate-btn-container">' .
     			'<div class="addedit-form-cancel-container-actions resize-save_btn"><div class="white-btn-bg-left"></div>',
    '#suffix' =>   '<div class="white-btn-bg-right"></div>',
    '#value' => t('LBL123'),
    '#attributes' => $cancelAttributes,
  );
  
  // Add the Save button
  $ajaxCallbackSave = 'addEditSaveAndCloseForm';
  
  $saveAttributes['class'] = array('addedit-edit-' . $saveButtonId); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  $saveAttributes['class'][] = 'admin-save-button-middle-bg';

  //Increase the width of the popup while clicking on save button
  if ($wrapperId != 'resource-addedit-form' && $wrapperId != 'organization-addedit-form' && $wrapperId != 'user-org-addedit-form'  && $wrapperId != 'user-addr-addedit-form' && $expandPopup) {
  $saveAttributes['onclick'][] = 'increasePopWidth();';
  }
  
  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
  $saveAttributes['data-wrapperid'] = array($wrapperId);
  
  $form[$saveButtonId] = array(
    '#type' => 'submit',
    '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
    '#suffix' => 
               '<div class="admin-save-button-right-bg"></div></div>' .
               '<div class="clearBoth"></div></div>' .
             '<div class="clearBoth"></div></div>',
    '#value' =>  t('LBL569'),
    	'#name' => 'save',
    '#attributes' => $saveAttributes,
      '#ajax' => array(
	      'wrapper' => $wrapperId,
	      'callback' => $ajaxCallbackSave,
        'triggerEvent' => 'hidden-save',
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

  $form['addedit-left-col-end'] = array(
    '#type' => 'markup',
    '#markup' => '</div><div class="clearBoth"></div>',
  );
  } catch (Exception $ex) {
  	watchdog_exception('businessRuleAddEditFormWithCancelAndSaveClose', $ex);
  	expertusErrorThrow($ex);
  }
}

?>