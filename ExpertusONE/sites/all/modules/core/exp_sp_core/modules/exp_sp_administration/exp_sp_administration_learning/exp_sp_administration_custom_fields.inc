<?php

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the names in it.
 */
function custom_add_more_btn_callback($form, $form_state) {
	try {
  return $form['custom_fieldset'];
  } catch (Exception $ex) {
  	watchdog_exception('custom_add_more_btn_callback', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function custom_add_row($form, &$form_state) {
	try {
  if($form_state['num_custom']==0){ 
    $form_state['num_custom']=1;
  }
  $form_state['num_custom']++;
  $form_state['display_class'] = TRUE;
  $form_state['rebuild'] = TRUE;
  } catch (Exception $ex) {
  	watchdog_exception('custom_add_row', $ex);
  	expertusErrorThrow($ex);
  }
}

function customfieldsOperations($form, $form_state,$entityId,$entityType){
  try {
  /*deleteCustomFieldsDetails($form, $form_state,$entityId,$entityType);
  $cnt = $form_state['num_custom']==0?1:$form_state['num_custom']; //Add by Vincent
  $isAdded = 0;
  $isUpdated = 0;
  for ($i = 0; $i < $cnt; $i++) {
    $customName = $form_state['values']['custom_fieldset']['custom']['custom_name_'.$i];
    $customURL = $form_state['values']['custom_fieldset']['custom']['custom_value_'.$i];
    
    if((!empty($customName) && $customName != 'Enter a Label') && (empty($customURL) || $customURL == 'Enter a Value')) {
      
      drupal_set_message(t('ERR096'), 'error');
      $isAdded = 0;
      $isUpdated = 0;
      
    }else if((!empty($customURL) && $customURL != 'Enter a Value') && (empty($customName) || $customName == 'Enter a Label')){
      
      drupal_set_message(t('ERR096'), 'error');
      $isAdded = 0;
      $isUpdated = 0;
      
    }else{
    
        if(!empty($customName) && !empty($customURL) && $customName != 'Enter a Label' && $customURL !='Enter a Value') {
          if(empty($form_state['values']['custom_id_'.$i])) {
          	addNewCustomFieldsDetails($form, $form_state,$entityId,$entityType,$i);
          	$isAdded = 1;
          }else{
          	updateCustomFieldsDetails($form, $form_state,$entityId,$entityType,$i);
          	$isUpdated = 1;
          }
        }
        
    }
  }
  
  if($isAdded){
    drupal_set_message(t('MSG275'));
  }
  elseif($isUpdated){
    drupal_set_message(t('MSG276'));
  }*/
  deleteCustomFieldsDetails($form, $form_state,$entityId,$entityType);
  $cnt = $form_state['num_custom']==0?1:$form_state['num_custom']; //Add by Vincent
  $isAdded = 0;
  $isUpdated = 0;
  for ($i = 0; $i < $cnt; $i++) {
    $customName = $form_state['values']['custom_fieldset']['custom']['custom_name_'.$i];
    $customURL = $form_state['values']['custom_fieldset']['custom']['custom_value_'.$i];
    if(!empty($customName) && !empty($customURL) && $customName != 'Enter a Label' && $customURL !='Enter a Value') {
      if(empty($form_state['values']['custom_id_'.$i])) {
      	addNewCustomFieldsDetails($form, $form_state,$entityId,$entityType,$i);
      	$isAdded = 1;
      }else{
      	updateCustomFieldsDetails($form, $form_state,$entityId,$entityType,$i);
      	$isUpdated = 1;
      }
    }
  }
  
  if($isAdded){
    drupal_set_message(t('Custom attributes added successfully.'));
  }
  elseif($isUpdated){
    drupal_set_message(t('Custom updated successfully.'));
  }  
  } catch (Exception $ex) {
  	watchdog_exception('customfieldsOperations', $ex);
  	expertusErrorThrow($ex);
  }
}


function getCustomFieldsDetails($entityId,$entityType) {
  try {
  $select = db_select("slt_custom_fields","cst");
  $select->addField('cst','id','custom_id');
  $select->addField('cst','entity_id','object_id');
  $select->addField('cst','entity_type','object_type');
  $select->addField('cst','label','label_txt');
  $select->addField('cst','value','value_txt');
  $select->condition('cst.entity_id',$entityId,'=');
  $select->condition('cst.entity_type',$entityType,'=');	
  expDebug::dPrintDBAPI('$select', $select);
  $customFieldsList = $select->execute()->fetchAll();
  
  return $customFieldsList;
  } catch (Exception $ex) {
  	watchdog_exception('getCustomFieldsDetails', $ex);
  	expertusErrorThrow($ex);
  }
}


function addNewCustomFieldsDetails($form, &$form_state,$entityId,$entityType,$i) { 
	
  $txn = db_transaction();

  try {
    	
      $insertStmt = db_insert('slt_custom_fields');
      $custom  = NULL;
      $currentUserId = getIdOfLoggedInUser();
      $fields = array(
                'entity_id'          => $entityId,                  
                'entity_type'        => $entityType,
                'label'              => $form_state['values']['custom_fieldset']['custom']['custom_name_'.$i],
                'value'              => $form_state['values']['custom_fieldset']['custom']['custom_value_'.$i],
                'created_by'         => $currentUserId,
                'created_on'         => now(),
      					'updated_by'         => $currentUserId,
      					'updated_on'         => now(),
                'custom0'            => $custom,
                'custom1'            => $custom,
                'custom2'            => $custom,
                'custom3'            => $custom,
                'custom4'            => $custom   
              );
      $insertStmt->fields($fields);			            
       
      $customId = $insertStmt->execute();
      $insertcustomform = $form_state['values']; 
      $insertcustomform[custom_label] = $form_state['values']['custom_fieldset']['custom']['custom_name_'.$i];
      $insertcustomform[custom_value] = $form_state['values']['custom_fieldset']['custom']['custom_value_'.$i];
      
      make_audit_trail_entries($insertcustomform, getIdOfLoggedInUser() , null, $customId ,'Inserted','cre_sys_obt_cls','exp_sp_administration_resources','addNewCustomFieldsDetails',
                                          array('entity_id', 'custom_label', 'custom_value'));
    	   
      expDebug::dPrint(' $$customId = ' . print_r($customId, true),3);  
     
      return array((object)array('id'=>$customId));
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('addNewCustomFieldsDetails', $ex);
    throw $ex;    
  }
}


function updateCustomFieldsDetails($form, &$form_state,$entityId,$entityType,$i) {
  
  $txn = db_transaction();

  try {

    $updateStmt = db_update('slt_custom_fields');
    $default= db_query("SELECT id,label as custom_label,value as custom_value from slt_custom_fields where id=:uid",array(":uid"=>$form_state['values']['custom_id_'.$i]))->fetchAll();
    $custom  = NULL;  

    $fields = array(
                  'label'              => trim($form_state['values']['custom_fieldset']['custom']['custom_name_'.$i]),
                  'value'              => trim($form_state['values']['custom_fieldset']['custom']['custom_value_'.$i]),
                  'updated_by'         => 1,
                  'updated_on'         => now(),
                  'custom0'            => $custom,
                  'custom1'            => $custom,
                  'custom2'            => $custom,
                  'custom3'            => $custom,
                  'custom4'            => $custom   
                );
    
    $updateStmt->fields($fields);    
    $updateStmt->condition('id', $form_state['values']['custom_id_'.$i]);
    $numUpdated = $updateStmt->execute();
     $updatecustomform = $form_state['values']; 
     $updatecustomform[custom_label] = $form_state['values']['custom_fieldset']['custom']['custom_name_'.$i];
     $updatecustomform[custom_value] = $form_state['values']['custom_fieldset']['custom']['custom_value_'.$i];
      
      make_audit_trail_entries($updatecustomform, getIdOfLoggedInUser() , $default, $entityId ,'updated','cre_sys_obt_cus','exp_sp_administration_resources','updateCustomFieldsDetails',
                                          array('entity_id', 'custom_label', 'custom_value'));
      	
    expDebug::dPrint(' $numUpdated = ' . print_r($numUpdated, true),3);
      
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('updateCustomFieldsDetails', $ex);
    throw $ex;
  }

  unset($txn);
  
}

/**
 * Submit handler for the "Delete" button.
 *
 * Decrements the Deleted value and causes a form rebuild.
 */
function delete_custom_fields($form, &$form_state) { 
	try {
  $customId = $form_state['triggering_element']['#ajax']['param'];
  $indexId = $form_state['triggering_element']['#ajax']['loop_id'];
  $form_state['delete_custom_fieldset'][] = $customId;    
  $form_state['delete_custom_index'][] = $indexId;
  $form_state['display_class'] = TRUE;
	if($form_state['num_custom']==count($form_state['delete_custom_index']) || $form_state['num_custom']==0){
    custom_add_row($form, $form_state);
  }    
  $form_state['rebuild'] = TRUE;
  } catch (Exception $ex) {
  	watchdog_exception('delete_custom_fields', $ex);
  	expertusErrorThrow($ex);
  }
}


function deleteCustomFieldsDetails($form, &$form_state,$entityId,$entityType) {
  
  $customId = $form_state['values']['custom_fieldset']['custom_hidden_delete'];
  
  $customIdArr = explode(',', $customId);
 
  $txn = db_transaction();
  
  $loggeduserId = getSltpersonUserId();
  
  try {
    if(!empty($customId)){
        //Get the fields before deleting
        $select = db_select('slt_custom_fields','custf');
        $select->addField('custf','id','id');
        $select->addField('custf','entity_id','entity_id');
        $select->addField('custf','entity_type','entity_type');
        $select->condition('id',$customIdArr,'IN');
        $custField=$select->execute()->fetchAll();
        
        $deleteStmt = db_delete('slt_custom_fields');
        $deleteStmt->condition('id', $customIdArr,'IN');
        $numUpdated = $deleteStmt->execute();
        
        // Make deleted log entry
        foreach ($custField as $val) {
            $fields = array(
                    'table_name' => 'slt_custom_fields',
                    'entity_id' => $val->id,
                    'entity_type' => 'custom_field',
                    'parent1_entity_id' => $val->entity_id,
                    'parent1_entity_type' => $val->entity_type,
                    'module_name' => 'exp_sp_administration_custom_fields',
                    'functionality_name' => 'deleteCustomFieldsDetails',
                    'api_name' => $_REQUEST['apiname'],
                    'deleted_on' => now(),
                    'deleted_by' => $loggeduserId
            );
            deleted_log_entry($fields);
        }
        
        expDebug::dPrint(' $numUpdated = ' . print_r($form_state['values']['custom_id_'.$i], true),5);
    }
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('deleteCustomFieldsDetails', $ex);
    throw $ex;
  }

  unset($txn);
  
}

function displayCustomFieldsDetails(&$form, &$form_state, $entityId, $entityType, $uniqueId, $isDropdown=FALSE){
	try {
  $customDetails = array();
  if (empty($form_state['values']) && !empty($entityId)) { 
    $customDetails = getCustomFieldsDetails($entityId, $entityType);
    $form_state['num_custom'] = count($customDetails);   
    $collapsedStatus = count($customDetails) > 0 ? TRUE : FALSE; 
  }

  //added for for dropdown design
  if($isDropdown == 	TRUE) {
    //$collapsedStatus = FALSE;
    $dropDownClass1  = 'admin-custom-required-col1-dropdown';
    $dropDownClass2  = 'admin-custom-required-col2-dropdown';
    $deleteImgClass  = 'admin-add-button-container-dropdown';
  } else{
    $dropDownClass1  = 'admin-custom-required-col1';
    $dropDownClass2  = 'admin-custom-required-col2';
    $deleteImgClass  = '';
  }
  //End dropdown changes
  
  if($form_state['display_class'] == TRUE){    
    if ((!empty($form_state['executed']) && $form_state['triggering_element']['#value'] == 'Save') || $form_state['storage']['step'] == 'edit') {
    	$customDetails = getCustomFieldsDetails($entityId, $entityType);
        $hideShowClass = (count($customDetails) > 0 || $form_state['num_custom'] > 0) ? '' : 'disable-class';
    } else{
       $hideShowClass = '';
    }
  }else{
     $hideShowClass = count($customDetails) > 0 ? '' : 'disable-class';
  }  
  
  $form['attr_action'] = array(
    '#type' => 'hidden',
    '#name' => 'attr_action',
    '#value' => 'custom',
  );
  
  //Changes done here for dropdown design
  if($isDropdown == 	TRUE) {    
    $form['custom_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('LBL319'),
      '#prefix' => '<div id="custom-fieldset-wrapper-'.$uniqueId.'" class="admin-tab-list-row custom-fieldset-wrapper '.$hideShowClass.'">',
      '#suffix' => '</div>',
      '#collapsible' => TRUE,
      '#collapsed' => $collapsedStatus
    );
  }else{
    $form['custom_fieldset'] = array(
      '#type' => 'markup',
      '#prefix' => '<div id="custom-fieldset-wrapper-'.$uniqueId.'" class="admin-tab-list-row custom-fieldset-wrapper">',
      '#suffix' => '</div>',
    );
  }
  //End  dropdown design changes
  
  if (!empty($entityId)) { 
  		$customDetails = getCustomFieldsDetails($entityId, $entityType);
  }
  
  if(count($customDetails) > 0){
    unset($form['list_type_'.$uniqueId]['#options']['custom_fieldset']);
    if(count($form['list_type_'.$uniqueId]['#options']) == 1){
      unset($form['list_type_'.$uniqueId]);
    }
  }

  $customCount = $form_state['num_custom'];
  if (empty($form_state['num_custom']) || $form_state['num_custom'] == 0) {
    //$form_state['num_custom'] = 1;
    $customCount=1;
  }  
  
  expDebug::dPrint('$form_state[num_custom] : '.print_r($form_state['num_custom'],true),5);
  expDebug::dPrint('$form_state[delete_custom_fieldset] : '.print_r($form_state['delete_custom_fieldset'],true),5);
  expDebug::dPrint('$customCount '.print_r($customCount,true),5);
  expDebug::dPrint('$customDetails : '.print_r($customDetails,true),4);

  $form['custom_fieldset']['custom']['custom_name_'] = array(
         '#type' => 'markup',    
         '#prefix' => '<div class="custom-details-row admin-text-field-column" ><table cellpadding="2" cellspacing="2" border="0"><tr><td class="addedit-new-field-title-custom">'.t('LBL397').':'.'<span class="addedit-mandatory">*</span>',
         '#suffix' => '</td>',          
         '#size' => 20,
         '#maxlength' => 50,
       );
       
         
       $form['custom_fieldset']['custom']['custom_value_'] = array(
         '#type' => 'markup',
         '#prefix' => '<td class="addedit-new-field-title-custom">'.t('LBL398').':'.'<span class="addedit-mandatory">*</span>',
         '#suffix' => '</td><td></td></tr></table></div>',
         '#size' => 38,
         '#maxlength' => 100,

       );
  
  for ($i = 0,$j=0; $i < $customCount; $i++) {
    
      if(in_array( $customDetails[$j]->custom_id, $form_state['delete_custom_fieldset'])){
      	$j++;
      }
      if(!in_array( $i, $form_state['delete_custom_index'])){
        
        addHiddenValueToAddEditForm($form, 'custom_id_'.$i, $customDetails[$j]->custom_id);
        
        $labelTxt = $customDetails[$j]->label_txt;
        $valueTxt = $customDetails[$j]->value_txt;
       
       if(!empty($customDetails[$j]->label_txt)) {
          $customLabelAttributes = array();
          //$customLabelAttributes['readonly'] = 'readonly';
          //$customLabelAttributes['class'][] = 'addedit-readonly-textfield custom-label-field'; // Add another class
          $customLabelAttributes['class'][] = 'input-field-set input-attachment-title addedit-edit-ac-textfield custom-label-field'; // Add another class
        } else {          
          $customLabelClass = '';
          $customLabelEmptyValue = 'Enter a Label';
          $formCustomLabel = $form_state['values']['custom_fieldset']['custom']['custom_name_'.$j];
          //$customLabelClass = 'input-field-grey';
          $customLabelClass = ($formCustomLabel == $customLabelEmptyValue || empty($formCustomLabel) )? 'input-field-grey' : '';
          $labelTxt = $customLabelEmptyValue;
          
          $customLabelAttributes = array();
          $customLabelAttributes['class'][] = 'custom-label-field input-field-set input-attachment-title addedit-edit-ac-textfield '.$customLabelClass; // Add another class
          $customLabelAttributes['onblur'][] = 'textfieldTitleBlur(this, "'.$customLabelEmptyValue.'");';
          $customLabelAttributes['onfocus'][] = 'textfieldTitleClick(this, "'.$customLabelEmptyValue.'");';
        } 
        
        
        if(!empty($customDetails[$j]->value_txt)) {
          $customValueAttributes = array();
         // $customValueAttributes['readonly'] = 'readonly';
          //$customValueAttributes['class'][]  = 'addedit-readonly-textfield'; // Add another class
          $customValueAttributes['class'][]  = 'input-field-set input-attachment-url addedit-edit-ac-textfield'; // Add another class
        } else {          
          $customValueClass = '';
          $customValueEmptyValue = 'Enter a Value';
          $formCustomValue = $form_state['values']['custom_fieldset']['custom']['custom_value_'.$j];
          //$customValueClass = 'input-field-grey';
          $customValueClass = ($formCustomValue == $customValueEmptyValue || empty($formCustomValue) )? 'input-field-grey' : '';
          $valueTxt = $customValueEmptyValue;
          
          $customValueAttributes = array();
          $customValueAttributes['class'][]  = 'input-field-set input-attachment-url addedit-edit-ac-textfield '.$customValueClass; // Add another class
          $customValueAttributes['onblur'][] = 'textfieldTitleBlur(this, "'.$customValueEmptyValue.'");';
          $customValueAttributes['onfocus'][] = 'textfieldTitleClick(this, "'.$customValueEmptyValue.'");';
        }
        
        
        $form['custom_fieldset']['custom']['custom_name_'.$i] = array(
          '#type' => 'textfield',     
          '#prefix' => '<div class="custom-details-row admin-text-field-column" ><table cellpadding="2" cellspacing="2" border="0"><tr><td class="'.$dropDownClass1.'">',
          '#suffix' => '</td>',  	
          '#default_value' => $labelTxt,  
          '#size' => 20,
          '#maxlength' => 50,
          '#attributes' => $customLabelAttributes,
        );
        
          
        $form['custom_fieldset']['custom']['custom_value_'.$i] = array(
          '#type' => 'textfield',
          '#prefix' => '<td class="custom-details-input-value-field '.$dropDownClass2.'">',
          '#suffix' => '</td>',
          '#default_value' => $valueTxt,
          '#size' => 38,
          '#maxlength' => 100,
          '#attributes' => $customValueAttributes,
        );

        // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
        $delCustomAttributes['class'][] = 'addedit-form-expertusone-throbber';
        $delCustomAttributes['class'][] = 'admin-save-button-middle-bg';
        $delCustomAttributes['class'][] = 'admin-delete-img';
        $delCustomAttributes['class'][] = 'admin-custom-delete-img';
        // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
        $delCustomAttributes['data-wrapperid'] = array('custom-fieldset-wrapper-'.$uniqueId);
        global $theme_key;
        if($theme_key=='expertusoneV2'){
            $image_path = '/sites/all/themes/core/expertusoneV2/expertusone-internals/images/close.png';
        }else{
            $image_path = 'sites/all/themes/core/expertusone/expertusone-internals/images/close.png';
        }
        
        
   //url("/sites/all/themes/core/expertusoneV2/expertusone-internals/images/close.png") no-repeat scroll 0 0 transparent
        $form['custom_fieldset']['custom']['custom_delete_'.$i] = array(
          //'#type' => 'submit',    
          '#type' => 'image_button', '#src' => base_path().$image_path,//'sites/all/themes/core/expertusone/expertusone-internals/images/close.png',
          //'#value' => 'Delete',
          '#title' => t('LBL286'),
          '#name' => 'Delete Custom'.$i,
          '#prefix' => '<td><div class="admin-add-button-container '.$deleteImgClass.'">',
          '#suffix' => '</div></td></tr></table></div>',    
          '#submit' => array('delete_custom_fields'),
          '#attributes' => $delCustomAttributes,
          '#ajax' => array(
            'callback' => 'custom_add_more_btn_callback',
            'wrapper' => 'custom-fieldset-wrapper-'.$uniqueId,
            'param' =>$customDetails[$j]->custom_id,
            'loop_id' =>$i
          ),  
        );
        $j++;
      }
  }
  
  $form['custom_fieldset']['custom_hidden_delete'] = array(
    '#type' => 'value',
    '#value' => implode(",",$form_state['delete_custom_fieldset'])
  );
  
  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $addcustomAttributes['class'][] = 'addedit-form-expertusone-throbber';
  $addcustomAttributes['class'][] = 'admin-save-button-middle-bg';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  $addcustomAttributes['data-wrapperid'] = array('custom-fieldset-wrapper-'.$uniqueId);  
  
  $form['custom_fieldset']['add_custom'] = array(
    '#type' => 'submit',
    //'#type' => 'image_button', '#src' => base_path().'sites/all/themes/core/AdministrationTheme/images/exp_sp_icon_16x16_MoveDown.gif',
    '#value' => t('LBL386'),
    '#name' => 'Add Custom',
    '#prefix' => '<div class="add-more-custom admin-add-more-field-column" id="add-more-custom"><table cellpadding="2" cellspacing="2" border="0"><tr><td><div class="admin-save-button-left-bg"></div>',
    '#suffix' => '<div class="admin-save-button-right-bg"></div></td></tr></table></div>',
    '#submit' => array('custom_add_row'),
  	'#attributes' => $addcustomAttributes,
    '#ajax' => array(
      'callback' => 'custom_add_more_btn_callback',
      'wrapper' => 'custom-fieldset-wrapper-'.$uniqueId,
    ),
  );
  } catch (Exception $ex) {
  	watchdog_exception('displayCustomFieldsDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function customFieldsDetailsView(&$form, &$form_state, $entityId, $entityType, $uniqueId){
 try {
  $customDetails = getCustomFieldsDetails($entityId, $entityType);
  
  if(count($customDetails) > 0) {
  
      /*$form['customfield_fieldset'] = array(
        '#type' => 'fieldset',
        '#title' => t('Custom Fields'),
        '#prefix' => '<div id="custom-fieldset-wrapper-'.$uniqueId.'" class="admin-tab-list-row custom-fieldset-wrapper '.$hideShowClass.'">',
        '#suffix' => '</div>',
        '#collapsible' => TRUE,
        '#collapsed' =>TRUE
      );*/
      $form['customfield_fieldset'] = array(
        '#type' => 'markup',
        '#prefix' => '<div id="custom-fieldset-wrapper-'.$uniqueId.'" class="admin-tab-list-row custom-fieldset-wrapper">',
        '#suffix' => '</div>',
      );
     $form['customfield_fieldset']['table_start'] = array(
      '#type'   => 'markup',
      '#markup' => '<table cellpadding="3" cellspacing="3" width="80%" border="0">',
     );

     for($i=0;$i<count($customDetails);$i++) {
       
       $readingType     = $customDetails[$i]->label_txt;
       $readingContent  = $customDetails[$i]->value_txt;
       
       $form['customfield_fieldset']['custom_label_'.$i] = array(
          '#type'     => 'item',
          '#markup'   => '<tr><td><div class="addedit-new-field-value addedit-view-field-value input-field-set input-custom-textfield addedit-edit-ac-textfield">'.$readingType.'</div></td>',
        );
       
       $form['customfield_fieldset']['custom_value_'.$i] = array(
          '#type'     => 'item',
          '#markup'   => '<td><div class="addedit-new-field-value addedit-view-field-value input-field-set input-custom-textfield addedit-edit-ac-textfield">'.$readingContent.'</div></td></tr>',
        );
     }
      
     $form['customfield_fieldset']['table_end'] = array(
      '#type'   => 'markup',
      '#markup' => '</table>',
     );
  }
  else{
    $form['no-records-msg'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="no-records-msg">'.t('MSG278').'</div>'
    );  
  }
  } catch (Exception $ex) {
  	watchdog_exception('customFieldsDetailsView', $ex);
  	expertusErrorThrow($ex);
  }
}
/*
 * function to add/update cutom fields on dropdown desing based modules
 */
function customfieldsOperationsForDropdown(&$form, &$form_state,$entityId,$entityType){
	try {
  deleteCustomFieldsDetails($form, $form_state,$entityId,$entityType);
  $cnt = $form_state['num_custom']==0?1:$form_state['num_custom']; //Add by Vincent
  for ($i = 0; $i < $cnt; $i++) {
    $customName = $form_state['values']['custom_fieldset']['custom']['custom_name_'.$i];
    $customURL = $form_state['values']['custom_fieldset']['custom']['custom_value_'.$i];
    if(!empty($customName) && !empty($customURL) && $customName != 'Enter a Label' && $customURL !='Enter a Value') {
      if(empty($form_state['values']['custom_id_'.$i])) {
      	addNewCustomFieldsDetails($form, $form_state,$entityId,$entityType,$i);
      }else{
      	updateCustomFieldsDetails($form, $form_state,$entityId,$entityType,$i);
      }
    }
  }//End for
  
  /*$form_state['custom_fields_error']= false;
  deleteCustomFieldsDetails($form, $form_state,$entityId,$entityType);
  $cnt = $form_state['num_custom']==0?1:$form_state['num_custom']; //Add by Vincent
  $result=true;
  for ($i = 0; $i < $cnt; $i++) {
    $customName = $form_state['values']['custom_fieldset']['custom']['custom_name_'.$i];
    $customURL = $form_state['values']['custom_fieldset']['custom']['custom_value_'.$i];
    if((!empty($customName) && $customName != 'Enter a Label') && (empty($customURL) || $customURL == 'Enter a Value')) {
      $form_state['custom_fields_error']= true;
      drupal_set_message(t('ERR096'), 'error');
      $result = false;      
    }else if((!empty($customURL) && $customURL != 'Enter a Value') && (empty($customName) || $customName == 'Enter a Label')){
      $form_state['custom_fields_error']= true;
      drupal_set_message(t('ERR096'), 'error');
      $result = false;
    }
  }
  if($result==true){
    for ($i = 0; $i < $cnt; $i++) {
      $customName = $form_state['values']['custom_fieldset']['custom']['custom_name_'.$i];
      $customURL = $form_state['values']['custom_fieldset']['custom']['custom_value_'.$i];
      if(!empty($customName) && !empty($customURL) && $customName != 'Enter a Label' && $customURL !='Enter a Value') {
        if(empty($form_state['values']['custom_id_'.$i])) {
        	addNewCustomFieldsDetails($form, $form_state,$entityId,$entityType,$i);
        }else{
        	updateCustomFieldsDetails($form, $form_state,$entityId,$entityType,$i);
        }
      }
    }
  }  
  return $result;*/
  } catch (Exception $ex) {
  	watchdog_exception('customfieldsOperationsForDropdown', $ex);
  	expertusErrorThrow($ex);
  }
}//End customfieldsOperationsForDropdown()

/*
 * function validate cutom fields on dropdown design based modules
 */
function customfieldsValidation(&$form, &$form_state){
	try {
  expDebug::dPrint(' $form_state[custom_fields] = ' . $form_state['custom_fields'],5);
  expDebug::dPrint(' $form_state[values] = ' . print_r($form_state['values'], true),5);
/*  $cnt = $form_state['custom_fields']['count'] == 0? 1 : $form_state['custom_fields']['count']; //Add by Vincent
  $form_state['custom_fields_error'] = false;
  for ($i = 0; $i < $cnt; $i++) {
    $customName = $form_state['values']['custom_name_'.$i];
    $customURL = $form_state['values']['custom_value_'.$i];
    expDebug::dPrint('Custom $customName= '. $customName);
    expDebug::dPrint('Custom $customURL= '. $customURL);
    if((!empty($customName) && $customName == 'Enter a Label' ) || (!empty($customURL) && $customURL == 'Enter a Value')) {
      $form_state['custom_fields']['count']--;
      expDebug::dPrint('Custom field validation count= '.$form_state['custom_fields']['count']);
    }        
  }
  return $form; */
  } catch (Exception $ex) {
  	watchdog_exception('customfieldsValidation', $ex);
  	expertusErrorThrow($ex);
  }
}//End customfieldsValidation



/*
 * getDefaultDetailsForCustomFields() -
 */
function getDefaultDetailsForCustomFields(&$form_state, $customDetails) {
	try {
		expDebug::dPrint(' $form_state[values] = ' .print_r($form_state['values'], true),5);
		expDebug::dPrint(' $form_state[custom_fields] = ' .print_r($form_state['custom_fields'], true),5);
		expDebug::dPrint(' $customDetails = ' .print_r($customDetails, true),4);
		expDebug::dPrint('$form_state[custom_fields][values] = ' .print_r($form_state['custom_fields']['values'], true),5);
		 
		$formsvalues = isset($form_state['values'])?$form_state['values']:array() ;
		$formStateValues = isset($form_state['custom_fields']['values'])? $form_state['custom_fields']['values'] : $formsvalues;
		$defaultCustomDetailsList = array();
		for ($i = 0; $i < $form_state['custom_fields']['count']; $i++) {
			$defaultCustomDetails = array();
			$defaultCustomDetails['id'] = isset($formStateValues['custom_id_' . $i])? $formStateValues['custom_id_' . $i] :
			(isset($customDetails[$i]->custom_id)? $customDetails[$i]->custom_id : '');
			;
	  $defaultCustomDetails['label'] = isset($formStateValues['custom_label_' . $i])? $formStateValues['custom_label_' . $i] :
                                                   (isset($customDetails[$i]->label_txt)? $customDetails[$i]->label_txt : ''); // #53342 - custom lable title fadeout effect removed.
	   
	  $defaultCustomDetails['value'] = isset($formStateValues['custom_value_' . $i])? $formStateValues['custom_value_' . $i] :
	  (isset($customDetails[$i]->value_txt)? $customDetails[$i]->value_txt : '');

	  // Exclude deleted field info from $defaultCustomDetailsList, which is used to rebuild the custom fields in the form
	  if (!empty($defaultCustomDetails['id']) && !empty($form_state['custom_fields']['delete'])
	  		&& in_array($defaultCustomDetails['id'], $form_state['custom_fields']['delete'])) {
	  	expDebug::dPrint('  Hiding id = ' .print_r($defaultCustomDetails['id'], true),4);
	  	continue;
	  }

	  $defaultCustomDetailsList[] = $defaultCustomDetails;

		} // end for

		// Discard $form_state['custom_fields']['values'] as no more needed
		unset($form_state['custom_fields']['values']);

		expDebug::dPrint(' $defaultCustomDetailsList = ' .print_r($defaultCustomDetailsList, true),4);
		return $defaultCustomDetailsList;
	} catch (Exception $ex) {
		watchdog_exception('getDefaultDetailsForCustomFields', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * preserveFormStateCustomFieldValues() - Save custom field values from $form state[values] in $form_state[custom_fields][values]
 *                                        and unset these in $form state[values] and $form state[input]
 *                                        On form rebuild the fields take these values from these saved values.
 */
function preserveFormStateCustomFieldValues(&$form_state) {
	try {
  $form_state['custom_fields']['values'] = array();
  for ($i = 0; $i < $form_state['custom_fields']['count']; $i++) {
    if (isset($form_state['values']['custom_id_' . $i])) {
      $form_state['custom_fields']['values']['custom_id_' . $i] = $form_state['values']['custom_id_' . $i];
      unset($form_state['values']['custom_id_' . $i]);
      unset($form_state['input']['custom_id_' . $i]);
    }
    
    if (isset($form_state['values']['custom_label_' . $i])) {
      $form_state['custom_fields']['values']['custom_label_' . $i] = $form_state['values']['custom_label_' . $i];
      unset($form_state['values']['custom_label_' . $i]);
      unset($form_state['input']['custom_label_' . $i]);
    }
    
    if (isset($form_state['values']['custom_value_' . $i])) {
      $form_state['custom_fields']['values']['custom_value_' . $i] = $form_state['values']['custom_value_' . $i];
      unset($form_state['values']['custom_value_' . $i]);
      unset($form_state['input']['custom_value_' . $i]);
    }
  }
  } catch (Exception $ex) {
  	watchdog_exception('preserveFormStateCustomFieldValues', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * markCustomFieldForDeletionAndRemoveFromAddEditForm() - 
 */
function markCustomFieldForDeletionAndRemoveFromAddEditForm($form, &$form_state) {
	try {
  expDebug::dPrint(' $form_state = ' .print_r($form_state, true),5);
  $displaySeqNo = $form_state['triggering_element']['#ajax']['display_seq_no'];
  $customFieldId = $form_state['triggering_element']['#ajax']['custom_field_id'];
  expDebug::dPrint(' $displaySeqNo = ' .print_r($displaySeqNo, true),5);
  expDebug::dPrint(' $customFieldId = ' .print_r($customFieldId, true),5);
                                                              
  $form_state['custom_fields']['delete'][] = $customFieldId;
  
  // Save custom field values from $form_state[values] in $form_state[custom_fields][values]
  // and unset these in $form state[values] and $form state[input]
  // On form rebuild the fields take these values from these saved values.
  // This is necessary, as on delete of a custom field, form_order_id is disturbed.
  preserveFormStateCustomFieldValues($form_state);
  //drupal_set_message(t('MSG347'));
  $form_state['rebuild'] = TRUE;
  } catch (Exception $ex) {
  	watchdog_exception('markCustomFieldForDeletionAndRemoveFromAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addNewCustomFieldOnAddEditFormRebuild() - 
 */
function addNewCustomFieldOnAddEditFormRebuild($form, &$form_state) {
	try {
  expDebug::dPrint(' $form_state[custom_fields][count] = ' .print_r($form_state['custom_fields']['count'], true),5);
  $incrementFlag  = true;
  if($form_state['custom_fields']['count'] > 0) {
    for ($i = 0; $i < $form_state['custom_fields']['count']; $i++) {
      if($form_state['values']['custom_value_' . $i]=='' || $form_state['values']['custom_value_' . $i]=='Enter a Value' || $form_state['values']['custom_label_' . $i]=='' || $form_state['values']['custom_label_' . $i]=='Enter a Label')
        $incrementFlag=false;
    }
  }
  
  if($incrementFlag==true){
	  // Save custom field values from $form state[values] in $form_state[custom_fields][values]
	  // and unset these in $form state[values] and $form state[input]
	  // On form rebuild the fields take these values from these saved values.
    preserveFormStateCustomFieldValues($form_state);

	  // Save the new custom field in $form_state['custom_fields']
    $form_state['custom_fields']['values']['custom_id_' . $i] = '';
    $form_state['custom_fields']['values']['custom_label_' . $i] = '';
    $form_state['custom_fields']['values']['custom_value_' . $i] = '';
    
    $form_state['custom_fields']['count']++; // Increment the custom field count

    //drupal_set_message(t('MSG346'));
  }
  $form_state['rebuild'] = TRUE; // so that form_submit is not called
  } catch (Exception $ex) {
  	watchdog_exception('addNewCustomFieldOnAddEditFormRebuild', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addCustomFieldsBlockToAddEditForm() - 
 */
function addCustomFieldsBlockToAddEditForm(&$form, &$form_state, $entityId, $entityType, $uniqueId, $formId, $tabIdx) {
	try {
  global $theme_key;
  $loaderWrapper = $uniqueId;
	// Get any pre-saved custom fields.
  $customDetails = array();
  expDebug::dPrint(' $form_state[custom_fields] = ' .print_r($form_state['custom_fields'], true),5);
  expDebug::dPrint(' $entityId = ' .print_r($entityId, true),4);
                                                                            
  if (empty($form_state['custom_fields']['values']) && !empty($entityId)) {
  	$customDetails = getCustomFieldsDetails($entityId, $entityType);
    expDebug::dPrint(' fetched $customDetails = ' .print_r($customDetails, true),4);
  	if (empty($form_state['custom_fields'])) {
      expDebug::dPrint(' setting custom fields count to records in db');
  		$form_state['custom_fields'] = array();
  		$form_state['custom_fields']['count'] = count($customDetails);
  	}
  }
  
  $defaultCustomDetails = getDefaultDetailsForCustomFields($form_state, $customDetails);
  // Update count of fields
  $form_state['custom_fields']['count'] = count($defaultCustomDetails);
    expDebug::dPrint(' after getting default details $form_state[custom_fields] = ' .print_r($form_state['custom_fields'], true),5);
  $form['customfields_wrapper_start'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="' . $uniqueId . '" class="addedit-customfields-wrapper">'
  );
  $countDefaultCustomDetails = count($defaultCustomDetails);
  for ($i = 0; $i < $countDefaultCustomDetails; $i++) {
  	
    if (!empty($defaultCustomDetails[$i]['id'])) {
      expDebug::dPrint(' Adding a custom field readonly label to form');
      addHiddenValueToAddEditForm($form, 'custom_id_' . $i, $defaultCustomDetails[$i]['id']);
      addHiddenValueToAddEditForm($form, 'custom_label_' . $i, $defaultCustomDetails[$i]['label']);
      $form['cfield_delbtn_wrapper_' . $i . '_start'] = array(
        '#type' => 'markup',
        '#markup' => '<table id="addedit-cfield-delbtn-wrapper-' . $i . '" class="addedit-cfield-delbtn-wrapper"><tr>'
      );
      $form['cfield_wrapper_' . $i . '_start'] = array(
        '#type' => 'markup',
        '#markup' => '<td id="addedit-cfield-wrapper-' . $i . '" class="addedit-cfield-wrapper">'
      );

     $valueFieldAttributes = array();
     $valueFieldAttributes['class'][] = 'addedit-edit-custom-value-field';
     $valueFieldAttributes['data-default-text'] = t('LBL405');
      
      $labelDetails = $defaultCustomDetails[$i]['id'].'-'. $entityId .'-'. $entityType;
      addTextAreaFieldToAddEditForm($form, 'custom_value_' . $i, $defaultCustomDetails[$i]['label'], $defaultCustomDetails[$i]['value'], $tabIdx, false, false);
      /*addTextFieldToAddEditFormEditable($form, 'custom_value_' . $i, $defaultCustomDetails[$i]['label'], $defaultCustomDetails[$i]['value'],
                                             $tabIdx, false, false, 99999, 60, false, true, $labelDetails, $loaderWrapper, $valueFieldAttributes);
                                             */
      //addTextFieldToAddEditForm($form, 'custom_value_' . $i, $defaultCustomDetails[$i]['label'], $defaultCustomDetails[$i]['value'], $tabIdx);
      ++$tabIdx;
      $form['cfield_wrapper_' . $i . '_end'] = array(
        '#type' => 'markup',
        '#markup' => '</td>'
      );

      $form['delbtn_wrapper_' . $i . '_start'] = array(
        '#type' => 'markup',
        '#markup' => '<td id="addedit-delbtn_wrapper-' . $i . '" class="delbtn-wrapper" valign="middle">'
      );
  
      // Add the delete button
      $delCustomAttributes['class'][] = 'admin-save-button-middle-bg';
      $delCustomAttributes['class'][] = 'admin-delete-img';
      $delCustomAttributes['class'][] = 'admin-custom-delete-img';
      // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
      $delCustomAttributes['class'][] = 'addedit-form-expertusone-throbber';
      // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
      $delCustomAttributes['data-wrapperid'] = array($uniqueId);
      
      $form['custom_delete_' . $i] = array (   
        '#type' => 'image_button',
        '#name' => 'Delete custom '.$i, // Form API requires this to be unique
        '#src' => base_path().($theme_key == 'expertusoneV2' ? 'sites/all/themes/core/expertusoneV2/expertusone-internals/images/close.png' : 'sites/all/themes/core/expertusone/expertusone-internals/images/close.png'),
        '#title' => t('LBL286'), //Delete
        '#title_display' => 'invisible',
        '#validate' => array('markCustomFieldForDeletionAndRemoveFromAddEditForm'), // will mark the custom field for deletion in form_state
                                                                                    // and rebuilt the form without it
        '#attributes' => $delCustomAttributes,
        '#ajax' => array (
          'callback' => 'addEditRepaintForm',
          'wrapper' => $formId,
          'event'=>'click',
          'keypress' => true,
          'custom_field_id' => $defaultCustomDetails[$i]['id'],
          'display_seq_no' => $i,
          'custom_fields_wrapper_id' => $uniqueId,
        ),  
      );
    
      $form['delbtn_wrapper_' . $i . '_end'] = array(
        '#type' => 'markup',
        '#markup' => '</td>'
      );

      $form['cfield_delbtn_wrapper_' . $i . '_end'] = array(
        '#type' => 'markup',
        '#markup' => '</tr></table>'
      );

    }
    else {
      expDebug::dPrint(' Adding a custom field editable label to form');
      addHiddenValueToAddEditForm($form, 'custom_id_' . $i, ''); // This is needed now, as getDefaultDetailsForCustomFields() gets non-null $customDetails even when values are to be read from $form_state
                                                                 // So, this variable needs to be maintained in form_state[values] for new records
    	addCustomFieldToAddEditForm($form, $i, $defaultCustomDetails[$i], $tabIdx);
    	$tabIdx += 2;
    }
  } // end for
  
  // Add the Add Custom button
  //$addcustomAttributes['class'][] = 'admin-save-button-middle-bg';
  //$addcustomAttributes['class'][] = 'admin-custom-field-addbtn';
  //$addcustomAttributes['class'][] = 'admin-custom-field-addbtn-middle-bg narrow-search-actionbar-orange-btnBG';
  
  $addcustomAttributes['class'][] = 'admin-action-button-middle-bg'; 
  
  //Adding plus symbol in the add another class
  $addcustomAttributes['class'][] = 'add-custombtn-symbol';
  
  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $addcustomAttributes['class'][] = 'addedit-form-expertusone-throbber';  
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  $addcustomAttributes['data-wrapperid'] = array($uniqueId);

  //$addcustomAttributes['onclick'][]  = 'addCustomGrayToBlack("'.$form_state['custom_fields']['count'].'");'; 
  
  $addBtnPrefix = '<div class="addedit-new-field addedit-custom-field" id="custom-add-btn-' . $uniqueId . '">' .
                     '<div class="addedit-new-field-title">&nbsp;</div>' .
                     '<div class="addedit-new-field-value"><div class="addedit-form-cancel-container-actions">';
  $addBtnSuffix =    '</div></div>' .
                     '<div class="clearBoth"></div>' .
                  '</div>';
  
  if($countDefaultCustomDetails < 10){
    $form['add_custom'.$uniqueId] = array(
      '#type' => 'submit',
      '#value' => t('LBL568'),
      '#name' => 'Add custom',
      '#prefix' => $addBtnPrefix,
      '#suffix' => $addBtnSuffix,
      '#validate' => array('addNewCustomFieldOnAddEditFormRebuild'),
      '#attributes' => $addcustomAttributes,
      '#ajax' => array(
        'callback' => 'addEditRepaintForm',
        'wrapper' => $formId,
        'event'=>'click',
        'keypress' => true,
        'add_custom_gray_field' => false,
        'custom_fields_wrapper_id' => $uniqueId,
      ),
    );
  }  
  $form['customfields_wrapper_end'] = array(
    '#type' => 'markup',
    '#markup' => '</div>'
  ); 
  
  expDebug::dPrint(' $form after build = ' . print_r($form, true),5);
  } catch (Exception $ex) {
  	watchdog_exception('addCustomFieldsBlockToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addCustomFieldToAddEditForm() - Adds a pair of text fields to the $form to allow user to add/edit custom field label and value
 */
function addCustomFieldToAddEditForm(&$form, $uniqueId, $defaultDetails, $tabIndex = null) {
	try {
  expDebug::dPrint(' $uniqueId = ' . $uniqueId,4);
  expDebug::dPrint(' $defaultDetails = ' . print_r($defaultDetails, true),4);
  // Create the label entry field
	$labelViewWidthInChars = 16;
	$labelMaxChars = 256;
	
  // Prepare the label textfield attributes 
  $labelFieldAttributes = array();
  $labelFieldAttributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $labelFieldAttributes['class'] = array('addedit-edit-custom-label-field-' . $uniqueId); // This class must be prefixed with 'addedit-edit-'
                                                                                          // followed by the form field id.
  
  // Add class to apply common style to all text fields.
  $labelFieldAttributes['class'][] = 'addedit-edit-custom-label-field';
    
  if (isset($tabIndex) && $tabIndex > 0) {
    $labelFieldAttributes['tabindex'] = $tabIndex;
    $tabIndex++; // Increment to set correct tab index for the value field
  }

  $labelPrefix = '<div class="addedit-new-field addedit-custom-field" id="custom-field-row-' . $uniqueId . '">' .
                   '<div class="addedit-new-field-title">';  
  $labelSuffix =   '</div>';  
  $valuePrefix =   '<div class="addedit-new-field-value">';
  $valueSuffix =   '</div>' .
                   '<div class="clearBoth"></div>' .
                 '</div>';

  $labelFieldAttributes['data-default-text'] = t('LBL404');

  $form['custom_label_' . $uniqueId] = array(
    '#type' => 'textfield',
    '#prefix' => $labelPrefix,
    '#suffix' => $labelSuffix,
    '#title_display' => 'invisible',
    '#default_value' => $defaultDetails['label'],
    '#attributes' => $labelFieldAttributes,
    '#size' => $labelViewWidthInChars,
    '#maxlength' => $labelMaxChars,
  );
	
  // Prepare the value text area attributes
  $valueFieldAttributes = array();
  $valueFieldAttributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $valueFieldAttributes['class'] = array('addedit-edit-textarea-custom_value_' . $uniqueId); // This class must be prefixed with 'addedit-edit-'
                                                                          // followed by the form field id.

  // Add class to apply common style to all text area fields.
  $valueFieldAttributes['class'][] = 'addedit-edit-textarea';

  // Add class to apply common style to all text fields.
  $valueFieldAttributes['class'][] = 'addedit-edit-custom-value-field';
  
  if (isset($tabIndex) && $tabIndex > 0) {
    $valueFieldAttributes['tabindex'] = $tabIndex;
  }
  
  $valueFieldAttributes['data-default-text'] = t('LBL405');
  
  $form['custom_value_' . $uniqueId] = array(
    '#type' => 'textarea',
    '#prefix' => $valuePrefix,
    '#suffix' => $valueSuffix,
    '#title_display' => 'invisible',
    '#default_value' => $defaultDetails['value'],
    '#attributes' => $valueFieldAttributes,
    '#rows'=>'1',
    '#cols'=>'40' ,
  );
  } catch (Exception $ex) {
  	watchdog_exception('addCustomFieldToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * validateCustomFieldsInAddEditForm() - 
 */
function validateCustomFieldsInAddEditForm($form, &$form_state) {
	try{
	$labelMissingMsgGenerated = false;
	$labelDuplicateMsgGenerated = false;
	//$labelInvalidMsgGenerated = false;
	$valueMissingMsgGenerated = false;
	$uniqueLabels = array();
  for ($i = 0; $i < $form_state['custom_fields']['count']; $i++) {
    // Custom field label cannot be null for the new field
    if (empty($form_state['values']['custom_id_' . $i])) {
    	// Fixed For this issue #0038739 White space are Eliminated => $custom_label
    	$custom_label =trim($form_state['values']['custom_label_' . $i]);
      if (empty($custom_label) || $form_state['values']['custom_label_' . $i] == t('LBL404')) {
        expDebug::dPrint(' Label for custom field custom_label_' . $i . ' is missing.',4);
        if (!$labelMissingMsgGenerated) {
          form_set_error('custom_label_' . $i, t('ERR136') . t('ERR101'));
          $form_state["storage"]["form_validate"] = false; // not used
          $form_state['esign_show_popup']= false;
          $labelMissingMsgGenerated = true;
        }
        else {
         form_set_error('custom_label_' . $i, ' '); // Only highlight the box, as error message is already generated.
        }
      }
      /*if (!empty($custom_label) || $form_state['values']['custom_label_' . $i] != t('LBL404')) { 
      	if (!preg_match('/^([a-z0-9\p{L}\p{N}\p{Pd}_-\s\']+)$/u', $custom_label)) {
      		if (!$labelInvalidMsgGenerated) {
      			form_set_error('custom_label_' . $i, t('ERR171') .' '. t('ERR136'));
      			$form_state["storage"]["form_validate"] = false; // not used
      			$form_state['esign_show_popup']= false;
      			$labelInvalidMsgGenerated = true;
      		}
      		else {
      			form_set_error('custom_label_' . $i, ' '); // Only highlight the box, as error message is already generated.
      		}
      	}
      }*/
      }
    // Fixed For this issue #0038739 White space are Eliminated => $custom_value
    $custom_value =trim($form_state['values']['custom_value_' . $i]);
    if (empty($custom_value) || $form_state['values']['custom_value_' . $i] == t('LBL405')) {
      expDebug::dPrint(' Value for custom field custom_value_' . $i . ' is missing.',4);
      if (!$valueMissingMsgGenerated) {
        form_set_error('custom_value_' . $i, t('ERR142') . t('ERR101'));
        $form_state["storage"]["form_validate"] = false; // not used
        $form_state['esign_show_popup']= false;
        $valueMissingMsgGenerated = true;
      }
      else {
        form_set_error('custom_value_' . $i, ' '); // Only highlight the box, as error message is already generated.
      }
    }
	$custom_label = trim(strtolower($form_state['values']['custom_label_' . $i])); // make lower case string to check case insenstive search
    // Check Duplicate label value
	if ($custom_label != t('LBL404') && isset($uniqueLabels[$custom_label])) {
    	if (!$labelDuplicateMsgGenerated) {
			form_set_error('custom_label_' . $i, t('LBL397').' '. t('ERR187'));
       		$form_state["storage"]["form_validate"] = false; // not used
       		$form_state['esign_show_popup']= false;
       		$labelDuplicateMsgGenerated = true;
       	}
       	else {
       		form_set_error('custom_label_' . $i, ' '); // Only highlight the box, as error message is already generated.
       	}
    }
    $uniqueLabels[$custom_label] = $i;
  } // end for loop
  } catch (Exception $ex) {
  	watchdog_exception('validateCustomFieldsInAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * fetchAddEditCustomFieldRecord() - get current values of a custom field for audit trail entries
 */
function fetchAddEditCustomFieldRecord($customFieldId, $fieldsList) {
	try {
  $select = db_select('slt_custom_fields', 'cfields');
  $select->condition('cfields.id', $customFieldId, '=');
  foreach ($fieldsList as $field) {
    $select->addField('cfields', $field, $field);
  }
  expDebug::dPrintDBAPI(' $select obj = ' ,$select);  
  $result = $select->execute()->fetchAll(); //make_audit_trail_entries() expects an array of objects for prev values
  expDebug::dPrint(' $result = ' . print_r($result, true),3);
  
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('fetchAddEditCustomFieldRecord', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * deleteAddEditCustomFieldInDB() - 
 */
function deleteAddEditCustomFieldInDB($customFieldId, $entityId, $entityType, $userId, $callFromAPI = 0) {
	$fieldsList = array('label', 'value');
  $prevValues = fetchAddEditCustomFieldRecord($customFieldId, $fieldsList);

  $txn = db_transaction(); 
  try {
      $loggeduserId = getSltpersonUserId();
      //Get the fields before deleting
      $select = db_select('slt_custom_fields','custf');
      $select->addField('custf','id','id');
      $select->addField('custf','entity_id','entity_id');
      $select->addField('custf','entity_type','entity_type');
      $select->condition('id',$customFieldId,'=');
      $custField=$select->execute()->fetchObject();
      
      $deleteStmt = db_delete('slt_custom_fields');
      $deleteStmt->condition('id', $customFieldId, '=');
      $numDeleted = $deleteStmt->execute();
      
      // Make deleted log entry
      $fields = array(
              'table_name' => 'slt_custom_fields',
              'entity_id' => $customFieldId,
              'entity_type' => 'custom_field',
              'parent1_entity_id' => $custField->entity_id,
              'parent1_entity_type' => $custField->entity_type,
              'module_name' => 'exp_sp_administration_custom_fields',
              'functionality_name' => 'deleteAddEditCustomFieldInDB',
              'api_name' => $_REQUEST['apiname'],
              'deleted_on' => now(),
              'deleted_by' => $loggeduserId
      );
      deleted_log_entry($fields);
              
      expDebug::dPrint(' $numDeleted = ' . $numDeleted,3);
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('deleteAddEditCustomFieldInDB', $ex);
    if($callFromAPI){
    	//return array((object)array('status'=>'Failiure'));
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Custom Field Id is not valid";
    	return $errobj;
    }
    throw $ex;
  }
  
  unset($txn); // Commit the transaction
  
  // Make audit trail entry
  make_audit_trail_entries(null, $userId, $prevValues, $customFieldId, 'Deleted', 'slt_custom_fields', $entityType . ':' . $entityId, 'deleteAddEditCustomFieldInDB',
                                          $fieldsList);
  if($callFromAPI && $numDeleted){
  	return array((object)array('status'=>'Success'));
  }
}

/*
 * addNewAddEditCustomFieldInDB()
 */
function addNewAddEditCustomFieldInDB($newFieldLabel, $newFieldValue, $entityId, $entityType, $userId, $callFromAPI =0) {

	$txn = db_transaction();
  try {
      
      $insertStmt = db_insert('slt_custom_fields');   
      $fields = array(
                'entity_id'          => $entityId,                  
                'entity_type'        => $entityType,
                'label'              => trim($newFieldLabel),
                'value'              => trim($newFieldValue),
                'created_by'         => $userId,
                'created_on'         => now(),
      					'updated_by'         => $userId,
      					'updated_on'         => now(),
              );

      $insertStmt->fields($fields);                        
      $customFieldId = $insertStmt->execute();
      expDebug::dPrint(' $customFieldId = ' . print_r($customFieldId, true),3);  
     
      
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('addNewAddEditCustomFieldInDB', $ex);
    if($callFromAPI){
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Unable to create custom field id.";
    	return $errobj;
    }
    else {
    	return array((object)array('id'=>'Failiure'));
    }
    throw $ex;
  }

  unset($txn); //Commit transaction

  $fieldsList = array('label', 'value');
  $insertedFields = array();
  $insertedFields['label'] = trim($newFieldLabel);
  $insertedFields['value'] = trim($newFieldValue);
  
  make_audit_trail_entries($insertedFields,  $userId, null, $customFieldId, 'Inserted', 'slt_custom_fields', $entityType . ':' . $entityId, 'addNewAddEditCustomFieldInDB',
                                          $fieldsList);
                                          
  return array((object) array('id' => $customFieldId));
}

/*
 * updateAddEditCustomFieldInDB() - 
 */
function updateAddEditCustomFieldInDB($customFieldId, $customFieldValue, $entityId, $entityType, $userId, $callFromAPI =0) {
	
  $fieldsList = array('value');
  $prevValues = fetchAddEditCustomFieldRecord($customFieldId, $fieldsList);
	
  
  $txn = db_transaction();

  try {
    $updateStmt = db_update('slt_custom_fields');
    $fields = array(
                  'value'              => trim($customFieldValue),
                  'updated_by'         => $userId,
                  'updated_on'         => now(),
                );
    
    $updateStmt->fields($fields);    
    $updateStmt->condition('id', $customFieldId);
    $numUpdated = $updateStmt->execute();
    expDebug::dPrint(' $numUpdated = ' . print_r($numUpdated, true),3);      
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('updateAddEditCustomFieldInDB', $ex);
    if($callFromAPI){
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Unable to update a custom field.";
    	return $errobj;
    }
    else {
    	return array((object)array('id'=>'Failiure'));
    }
    throw $ex;
  }

  unset($txn); // Commit the transaction

  $updatedFields = array();
  $updatedFields['value'] = trim($customFieldValue);
          
  make_audit_trail_entries($updatedFields,  $userId, $prevValues, $customFieldId, 'Updated', 'slt_custom_fields', $entityType . ':' . $entityId, 'updateAddEditCustomFieldInDB',
                                          $fieldsList);
  if($callFromAPI){
  	return array((object)array('id'=>$customFieldId));
  }
}

/*
 * saveCustomFieldsInAddEditForm() -
 */
function saveCustomFieldsInAddEditForm($form, &$form_state, $entityId, $entityType, $userId) {
 try {
	// Delete custom field records marked for deletion.
	// @TODO: Delete all records with one sql statement.
	if (isset($form_state['custom_fields']['delete'])) {
		foreach ($form_state['custom_fields']['delete'] as $customFieldId) {
	      expDebug::dPrint(' Deleting custom field db record id = ' . $customFieldId,5);
	      deleteAddEditCustomFieldInDB($customFieldId, $entityId, $entityType, $userId);
	  }
	  unset($form_state['custom_fields']['delete']);
	}
	
	$fetchValuesFromDBOnFormRebuild = false;
  for ($i = 0; $i < $form_state['custom_fields']['count']; $i++) {
    if (empty($form_state['values']['custom_id_' . $i])) {
    	// Create new custom field for $entityId and $entityType
    	$newCustomFieldLabel = $form_state['values']['custom_label_' . $i];
    	$newCustomFieldValue = $form_state['values']['custom_value_' . $i];
    	
    	// Save a new field only when it has both label and value (and these are not the default text)
    	if((!empty($newCustomFieldLabel) && $newCustomFieldLabel != t('LBL404')) && (!empty($newCustomFieldValue) && $newCustomFieldValue != t('LBL405'))) {
        	$return = addNewAddEditCustomFieldInDB($newCustomFieldLabel, $newCustomFieldValue, $entityId, $entityType, $userId);
        	$newCustomFieldId = $return[0]->id;
        	expDebug::dPrint(' Created new custom field db record id = ' . $newCustomFieldId,4);
        	$form_state['values']['custom_id_' . $i] = $newCustomFieldId;
    	}
    	else {
    		$fetchValuesFromDBOnFormRebuild = true;
      }        
    }
    else {
      // Update value for custom field
      $customFieldId = $form_state['values']['custom_id_' . $i];
      $customFieldValue = $form_state['values']['custom_value_' . $i];
      expDebug::dPrint(' update : $customFieldValue = ' . $customFieldValue,5);
      if (!empty($customFieldValue) && $customFieldValue != t('LBL405')) {
        expDebug::dPrint(' Updating custom field db record id = ' . $customFieldId . ' to value = ' . $customFieldValue,5);
        updateAddEditCustomFieldInDB($customFieldId, $customFieldValue, $entityId, $entityType, $userId);
      }
      else { // No value, delete the custom field from db.
      	$fetchValuesFromDBOnFormRebuild = true;
        expDebug::dPrint(' Deleting custom field db record id = ' . $customFieldId . ' as value is removed',5);
        deleteAddEditCustomFieldInDB($customFieldId, $entityId, $entityType, $userId);
      }
    }
  } // end for loop

  if ($fetchValuesFromDBOnFormRebuild) {	
  	for ($i = 0; $i < $form_state['custom_fields']['count']; $i++) {
	    unset($form_state['values']['custom_id_' . $i]);
      unset($form_state['values']['custom_label_' . $i]);
      unset($form_state['values']['custom_value_' . $i]);
	    unset($form_state['input']['custom_id_' . $i]);
	    unset($form_state['input']['custom_label_' . $i]);
	    unset($form_state['input']['custom_value_' . $i]);
  	}
  	unset($form_state['custom_fields']);
  }
  } catch (Exception $ex) {
  	watchdog_exception('saveCustomFieldsInAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * updateCustomFieldsById() -
 */
function updateCustomFieldsById($newValue, $customId){
	try {
  $fields['label'] = trim($newValue);
  
  $update = db_update('slt_custom_fields');
  $update->fields($fields);
  $update->condition('id', $customId);
  $update->execute();
  
  $field = $newValue; //titleController(' ', trim($newValue),10); #53342 - custom lable title fadeout effect removed
  print $field.':';
  } catch (Exception $ex) {
  	watchdog_exception('updateCustomFieldsById', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * List custom fields api
 */
function listCustomFieldApi($start='', $limit='', $userid='') {
	try {
		$form=array();
		$form["form_id"]="";
		$default_limit = 10;
		$entity_type = chagneEntityCode(trim($_REQUEST['entity_type']));
		$entity_id 	= trim($_REQUEST['entity_id']); // list of class api support.
		$start = trim($_REQUEST['start']);
		$limit = trim($_REQUEST['limit']);
		$start = (empty($start) || ($start < 0)) ?  0 : $start;
		$limit = (empty($limit) || ($limit < 0)) ?  $default_limit : $limit;
		if (!checkCustomFieldEntityTypes($entity_type)) { // check valid custom fields avaiable entities
			$error -> isValidateError = true;
			$errobj->errorfield = "entity_type";
			$error -> errormsg ='Invalid EntityType';
			return $error;
		}
		if (!checkCustomFieldEntityTypes($entity_type, $entity_id) || !is_numeric($entity_id)) { // check valid entity id against entity
			$error -> isValidateError = true;
			$errobj->errorfield = "entity_type";
			$error -> errormsg ='Invalid EntityId or Not Found';
			return $error;
		}
		if(!is_numeric($start) || !is_numeric($limit)) {
			$error -> isValidateError = true;
			$error -> errormsg =t('start and limit must be in Numeric values only');
			return $error;
		}
		if (!checkCustomFieldEntityTypes($entity_type)) {
			$error -> isValidateError = true;
			$error -> errormsg =t('Custom Field is not supported to this entity type');
			return $error;
		}
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$result = fetchCustomFieldsAll('',$start,$limit,$entity_type, $entity_id);
		$count = fetchCustomFieldsAll('COUNT', '', '',$entity_type, $entity_id);
		$result['totalrow'] = $count ;
		return $result;
	} catch (Exception $ex) {
		watchdog_exception('listCustomFieldApi', $ex);
		expertusErrorThrow($ex);
	}
}
/**
 * 	Retrive all custom fields based on entity_type and entity_id
 */
function fetchCustomFieldsAll($op, $start, $limit, $entity_type, $entity_id) {
	try {
		$select = db_select('slt_custom_fields', 'customfields');
		if($op=='COUNT'){
			$select->addExpression('count(1)','cnt');
		}else{
		$select->addField('customfields', 'id', 'id');
		$select->addField('customfields', 'label', 'field_label');
		$select->addField('customfields', 'value', 'field_value');
		$select->addField('customfields', 'entity_type', 'Entity Type');
		}
		if ($entity_type)
			$select->condition('entity_type', $entity_type, '=');
		if ($entity_id)
			$select->condition('entity_id', $entity_id, '=');
		// Limit to $limit records
		if(is_numeric($start) && is_numeric($limit))
			$select->range($start, $limit);
		expDebug::dPrintDBAPI('$select with range', $select);
		// Execure the query and fetch all the records
		if($op=='COUNT'){
		$result = $select->execute()->fetchField();
		} else {
		$result = $select->execute()->fetchAll();
		}
		expDebug::dPrint('LIST $result = ' . print_r($result, true) , 3);
			
		return $result;
	} catch(Exception $ex) {
		watchdog_exception('fetchCustomFieldsAll', $ex);
		expertusErrorThrow($ex);
	}
	}

/**
 *  Create Custom Fields to the Entities.
 */
function createCustomFieldByRestAPI(&$form_state, $createuserid="") {
	try {
		$form=array();
		$error_messages = array();
		$form["form_id"]="";
		$entity_type 	= chagneEntityCode(trim($form_state['values']['entity_type']));
		$entity_id 		= trim($form_state['values']['entity_id']);
		$custom_label 	= trim($form_state['values']['custom_label']);
		$custom_value 	= trim($form_state['values']['custom_value']);
		if (!checkCustomFieldEntityTypes($entity_type)) { // check valid custom fields avaiable entities
			$error -> isValidateError = true;
			$errobj->errorfield = "entity_type";
			$error -> errormsg ='Invalid EntityType';
			return $error;
		}
		if (!checkCustomFieldEntityTypes($entity_type, $entity_id) || !is_numeric($entity_id)) { // check valid entity id against entity
			$error -> isValidateError = true;
			$errobj->errorfield = "entity_type";
			$error -> errormsg ='Invalid EntityId or Not Found';
			return $error;
		}
		if (getCustomFieldCount($entity_type, $entity_id)) { // check valid custom fields avaiable entities
			$error -> isValidateError = true;
			$errobj->errorfield = "entity_type";
			$error -> errormsg = 'Maximum 10 Custom Fields Allowed to an Entity';
			return $error;
		}
		/*if (!preg_match('/^([a-z0-9_-\s]+)$/i', $custom_label)) {  // code commented due to support of utf-8 char's
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errorfield = "custom_label";
			$errobj->errormsg = 'Invalid Label for custom attribute';
			$error_messages[] = $errobj;
		}*/
		if (strlen($custom_label) > 256) {
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errorfield = "custom_label";
			$errobj->errormsg = 'Only 256 characters allowed to Custom Label';
			$error_messages[] = $errobj;
		}
		if(checkLabelDuplicate($entity_type, $entity_id, $custom_label)) { // Check label duplicate
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errorfield = "custom_label";
			$errobj->errormsg = 'Label already exist. Enter a new label';
			$error_messages[] = $errobj;
		}
		if(count($error_messages) > 0) {
			$errors = new stdClass();
			$errors->isValidateError = 1;
			$errors->errors = $error_messages;
			return $errors;
		}
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$customFieldId = addNewAddEditCustomFieldInDB($custom_label, $custom_value, $entity_id, $entity_type, $createuserid, 1);
		expDebug::dPrint(' createCustomFieldByRestAPI created:' . print_r($customFieldId,true), 4);
		if($customFieldId[0]->id  && $customFieldId[0]->id != 'Failiure')
			return $customFieldId;
	} catch(Exception $ex) {
		watchdog_exception('createCustomFieldByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 *  Update Custom Field to the Entities.
 */
function updateCustomFieldByRestAPI(&$form_state, $createuserid="") {
	try {
		$form=array();
		$error_messages = array();
		$form["form_id"]="";
		$entity_type 	= chagneEntityCode(trim($form_state['values']['entity_type']));
		$entity_id 		= trim($form_state['values']['entity_id']);
		$custom_label 	= trim(stripslashes($form_state['values']['custom_label']));
		$custom_value 	= trim(stripslashes($form_state['values']['custom_value']));
		$customFieldId	= getCustomFieldIdByLabel($entity_type, $entity_id, $custom_label);
		if (!checkCustomFieldEntityTypes($entity_type)) { // check valid custom fields avaiable entities
			$error -> isValidateError = true;
			$errobj->errorfield = "entity_type";
			$error -> errormsg ='Invalid EntityType';
			return $error;
		}
		if (!checkCustomFieldEntityTypes($entity_type, $entity_id) || !is_numeric($entity_id)) { // check valid entity id against entity
			$error -> isValidateError = true;
			$errobj->errorfield = "entity_type";
			$error -> errormsg ='Invalid EntityId or Not Found';
			return $error;
		}
		if ($customFieldId == '') { // Check the field exist before proceed
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errorfield = "custom_label";
			$errobj->errormsg = 'Invalid Custom Label to this Entity or Not Found';
			$error_messages[] = $errobj;
		}
		if(count($error_messages) > 0) {
			$errors = new stdClass();
			$errors->isValidateError = 1;
			$errors->errors = $error_messages;
			return $errors;
		}
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$updatedCustomFieldId = updateAddEditCustomFieldInDB($customFieldId, $custom_value, $entity_id, $entity_type, $createuserid, 1);
		expDebug::dPrint(' createCustomFieldByRestAPI created:' . print_r($updatedCustomFieldId,true), 4);
		return $updatedCustomFieldId;
	} catch(Exception $ex) {
		watchdog_exception('updateCustomFieldByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 *  Delete Custom Field in the attached Entities.
 */
function deleteCustomFieldByRestAPI(&$form_state, $createuserid="") {
	try {
		$form=array();
		$error_messages = array();
		$form["form_id"]="";
		$entity_type 	= chagneEntityCode(trim($form_state['values']['entity_type']));
		$entity_id 		= trim($form_state['values']['entity_id']);
		$custom_label 	= trim(stripslashes($form_state['values']['custom_label']));
		$customFieldId	= getCustomFieldIdByLabel($entity_type, $entity_id, $custom_label);
		if (!checkCustomFieldEntityTypes($entity_type)) { // check valid custom fields avaiable entities
			$error -> isValidateError = true;
			$errobj->errorfield = "entity_type";
			$error -> errormsg ='Invalid EntityType';
			return $error;
		}
		if (!checkCustomFieldEntityTypes($entity_type, $entity_id) || !is_numeric($entity_id)) { // ccheck valid entity id against entity
			$error -> isValidateError = true;
			$errobj->errorfield = "entity_type";
			$error -> errormsg ='Invalid EntityId or Not Found';
			return $error;
		}
		if ($customFieldId == '') { // Check the field exist before proceed
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errorfield = "custom_label";
			$errobj->errormsg = 'Invalid Custom Label to this Entity or Not Found';
			$error_messages[] = $errobj;
		}
		if(count($error_messages) > 0) {
			$errors = new stdClass();
			$errors->isValidateError = 1;
			$errors->errors = $error_messages;
			return $errors;
		}
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$status = deleteAddEditCustomFieldInDB($customFieldId, $entity_id, $entity_type, $createuserid, 1);
		expDebug::dPrint(' deleteCustomFieldByRestAPI delete course:' . print_r($status,true), 4);
		return $status;
	} catch(Exception $ex) {
		watchdog_exception('deleteCustomFieldByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Helper function to validate support entity types in custom fields
 */
function checkCustomFieldEntityTypes($entity_type = '', $entity_id = 0) {
	$support_entities = array(
			'course' => 'slt_course_template',
			'class' => 'slt_course_class',
			'cre_sys_obt_trp' => 'slt_program',
			'user' => 'slt_person',
			'organization' => 'slt_organization',
			'sry_det_typ_sry' => 'slt_survey',
			'sry_det_typ_ass' => 'slt_survey',
			'location' => 'slt_location');
	//expDebug::dPrint('table name:' . $support_entities[$entity_type]);
	if ($entity_id) {
		if (array_key_exists($entity_type, $support_entities)) {
			$select = db_select($support_entities[$entity_type], 'entity');
			$select->addField('entity', 'id');
			$select->condition('id', $entity_id);
			if ($entity_type == 'sry_det_typ_sry' || $entity_type == 'sry_det_typ_ass' ) { // cross verify type - survey and assessment use single table.
				$select->condition('type', $entity_type);
			}
			//expDebug::dPrintDBAPI('$select in checkCustomFieldEntityTypes', $select);
			$result = $select->execute()->fetchField();
			if ($result > 0 && $result != '')
				return true;
		}
		return false;
	} else {
		return array_key_exists($entity_type, $support_entities);
	}
}

/**
 * Helper function to get primary id of the custom field
 */
function getCustomFieldIdByLabel($entityType = '', $entityId = '', $labelTxt = '') {
	$result = db_query('select id from slt_custom_fields where entity_id = :entityId and entity_type = :entityType and label = :labelTxt', array(':entityId' => $entityId, ':entityType' => $entityType, ':labelTxt' => $labelTxt))->fetchField();
	//expDebug::dPrint('getCustomFieldIdByLabel $result = ' . print_r($result, true) , 3);
	return $result;
}

/**
 * Helper function to validate support entity types in custom fields
 */
function checkLabelDuplicate($entity_type = '', $entity_id = '', $label_text = '') {
	$select = db_select('slt_custom_fields', 'custom_fields');
	$select->addField('custom_fields', 'id', 'id');
	$select->condition('entity_type', $entity_type);
	$select->condition('entity_id', $entity_id);
	$select->where('LOWER(label) = :label', array(':label' => strtolower($label_text)));
	$result = $select->execute()->rowCount();
	//expDebug::dPrint('LIST $result = ' . print_r($result, true) , 3);
	if ($result) {
		return true;
	} else {
		return false;
	}
}

/**
 * Helper function to validate support entity types in custom fields
 */
function getCustomFieldCount($entity_type = '', $entity_id = '') {
	$select = db_select('slt_custom_fields', 'custom_fields');
	$select->addField('custom_fields', 'id', 'id');
	$select->condition('entity_type', $entity_type);
	$select->condition('entity_id', $entity_id);
	$count = $select->execute()->rowCount();
	//expDebug::dPrint('getCustomFieldCount LIST $result = ' . print_r($count, true) , 3);
	if ($count >= 10) {
		return true;
	} else {
		return false;
	}
}

/**
 * Helper function to change common name to machine name
 */
function chagneEntityCode($entityType = '') {
	try {
		$support_entities = array(	// Custom fields supporting entities
			'course' 		=> 'course', 
			'class' 		=> 'class', 
			'trainingplan'	=> 'cre_sys_obt_trp', 
			'user' 			=> 'user', 
			'organization' 	=> 'organization', 
			'survey' 		=> 'sry_det_typ_sry', 
			'assessment' 	=> 'sry_det_typ_ass', 
			'location' 		=> 'location');
		expDebug::dPrint('changed entity code:' . $support_entities[$entityType]);
		if (isset($support_entities[$entityType])) {
			return $support_entities[$entityType];
		}
		return false;
	} catch(Exception $ex) {
		watchdog_exception('chagneEntityCode', $ex);
		expertusErrorThrow($ex);
	}
}

?>