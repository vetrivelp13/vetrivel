<?php

/**
 * Password Policy Module Admin inc file
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 18-Jun-2013
 */

/**
 * Show exp_sp_pwdpolicy_addedit_form in ctools popup
 */
function exp_sp_administration_pwdpolicy_display_form($js = NULL) {
	try {
    $js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
    drupal_add_js(drupal_get_path('module', 'exp_sp_administration_module_info') . '/exp_sp_administration_module_info_pwdpolicy.js',
                                                 $js_module_optional);
	
    // Default modalTheme is ExpertusCToolsModalTheme (see exp_sp_core_extend_ctools_ajax.js:Drupal.CTools.Modal.show())
	  $pwdpolicy_style = array( 
	    'ctools-admin-pwdpolicy-style' => array(
	      'modalSize' => array(
	        'width' => 607,
	        'minHeight' => 290,
	      ),
	    )
	  );
	
	  drupal_add_js($pwdpolicy_style, 'setting');
	
	  if ($js) {
	    // Prepare list of forms to be displayed in the modal
	    $formIdList = array(
	      array('id' => 'exp_sp_administration_pwdpolicy_form',
	            'title' => (t('LBL1040'))), //Password Policy
	    );	
	    exp_ctools_modal_multi_ajaxonly_forms_display($formIdList, array());
	  }
	  else {
	    throw new Exception('exp_sp_administration_module_info_pwdpolicy.inc: exp_sp_administration_pwdpolicy_display_form() : nojs');
	  }
	}
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_pwdpolicy_display_form', $ex);
    expertusErrorThrow($ex);
  }
}


/**
 * Build the form for password policy admin
 */
function exp_sp_administration_pwdpolicy_form($form, &$form_state) {
  try {
  	// Form wrapper id
  	$formWrapperId = 'pwdpolicy-admin-form';
  	
  	// Get current values from db
    $policy = get_expertus_pwdpolicy();
    
    // New form
	  $form = newAddEditFormWithCancelAndSave($formWrapperId);
	  
	  // Add password policy id field (hidden)
	  $defaultValue = $policy? $policy['pid'] : '';
    addHiddenFieldToAddEditForm($form, 'pwdpolicy-pid', $defaultValue); // This hidden field will also be read by javascript behaviour
	  
	  // Add name field
	  $defaultValue = $policy? $policy['name'] : '';
	  addTextFieldToAddEditForm($form, 'pwdpolicy-name', t('LBL107'), $defaultValue, 1, true); //Name
	  
	  // Add description field
	  $defaultValue = $policy? $policy['description'] : '';
	  addTextFormatFieldToAddEditForm($form, 'pwdpolicy-description', t('LBL229'), $defaultValue, 'full_html', 2, true, false, 5000); //Description
	  
	  // Add a two column row
	  addTwoColumnRowToAddEditForm($form, 'pwdpolicy-expiration-warning');

    //Add Expires After
    $defaultValue =  $policy? $policy['expiration'] : '';
    addExpiresAfterTextFieldToPwdPolicyForm($form, 'pwdpolicy-expiration', t('LBL1043'), $defaultValue, 3); //Expires After
    
    $defaultValue =  $policy? $policy['warning'] : '';
    addWarningTextFieldToPwdPolicyForm($form, 'pwdpolicy-warning', t('LBL1044'), $defaultValue, 4); //Warning Before

    endAddEditTwoColumnRow($form);

    // Save translated password strength popup label in hidden field. This field will be read by javascript behaviour.
    addHiddenFieldToAddEditForm($form, 'pwdpolicy-pwdstrength-label', t('LBL1042')); //Password Strength
    
    // Add action buttons and close the form
    addEditFormWithPwdpolicyCancelAndSaveClose($form, $formWrapperId, 5);
	  expDebug::dPrint('$form = ' . print_r($form, true), 5);

	  return $form;
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_pwdpolicy_form', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Use this function to add password policy expires after text field to exp_sp_administration_pwdpolicy_form
 */
function addExpiresAfterTextFieldToPwdPolicyForm(&$form, $id, $title = '', $defaultValue = '', $tabIndex = null) {
	try {
	  // Prepare the additional HTML attributes for the textfield 
	  $attributes = array();
	  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete
	
	  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
	  // each time the form is re-built.
	  $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
	  // Add class to apply common style to all text fields.
	  $attributes['class'][] = 'addedit-edit-textfield';
	  if (isset($tabIndex) && $tabIndex > 0) {
	    $attributes['tabindex'] = $tabIndex;
	  }
	  
	  // Default field text when empty
	  $attributes['data-default-text'] = array(t('MSG674')); //Enter number

    $prefix = addEditGetPrefix('TWOCOL_FIRSTCOL_FIELD', $title);
    $suffix =      //'<div class="pwdpolicy-help-text">' . t('MSG634') . '</div>' . //Enter days.
                 '</div>' .
                 '<div class="clearBoth"></div>' .
               '</div>';
    
    // Increment current column
    $form['addedit_tmp_storage']['two_column']['current_column'] = 1;
	
	  // Create the form element
	  $form[$id] = array(
	    '#type' => 'textfield',
	    '#prefix' => $prefix,
	    '#suffix' => $suffix,
	    '#title' => $title,
	    '#title_display' => 'invisible',
	    '#default_value' => $defaultValue,
	    '#attributes' => $attributes,
	    '#size' => 64,
	    '#maxlength' => 64,
	    '#description' => false
	  );
	}
  catch (Exception $ex) {
    watchdog_exception('addExpiresAfterTextFieldToPwdPolicyForm', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * Use this function to add password policy warning text field to exp_sp_administration_pwdpolicy_form
 */
function addWarningTextFieldToPwdPolicyForm(&$form, $id, $title = '', $defaultValue = '', $tabIndex = null) {
	try {
	  // Prepare the additional HTML attributes for the textfield 
	  $attributes = array();
	  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete
	
	  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
	  // each time the form is re-built.
	  $attributes['class'] = array('addedit-edit-' . $id);
	  // Add class to apply common style to all text fields.
	  $attributes['class'][] = 'addedit-edit-textfield';
	  if (isset($tabIndex) && $tabIndex > 0) {
	    $attributes['tabindex'] = $tabIndex;
	  }
	  
	  // Default field text when empty
	  $attributes['data-default-text'] = array(t('MSG674')); //Enter number(s)
	
    $prefix = addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $title);
    $suffix =      //'<div class="pwdpolicy-help-text">' . t('MSG636') . '</div>' . //Enter comma separated days.
                 '</div>' .
                 '<div class="clearBoth"></div>' .
               '</div>';
    
    // increment current column
    $form['addedit_tmp_storage']['two_column']['current_column'] = 2;
	
	  // Create the form element
	  $form[$id] = array(
	    '#type' => 'textfield',
	    '#prefix' => $prefix,
	    '#suffix' => $suffix,
	    '#title' => $title,
	    '#title_display' => 'invisible',
	    '#default_value' => $defaultValue,
	    '#attributes' => $attributes,
	    '#size' => 64,
	    '#maxlength' => 64,
	    '#description' => false
	  );
  }
  catch (Exception $ex) {
    watchdog_exception('addWarningTextFieldToPwdPolicyForm', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Adds cancel and save button to exp_sp_administration_pwdpolicy_form with password strength qtip popup links
 */
function addEditFormWithPwdpolicyCancelAndSaveClose(&$form, $wrapperId, $tabIndex) {
  try {
  	// Button ids
  	$saveButtonId = 'pwdpolicy-save-button';
    $cancelButtonId = 'pwdpolicy-cancel-button';
    $esignHiddenButt = 'esign-' . $wrapperId;
    global $theme_key;
    $prefixBgLeft='';$suffixBgRight='';
    if($theme_key == 'expertusoneV2'){
    	$prefixBgLeft   = '<div class="white-btn-bg-left"></div>';
    	$suffixBgRight = '<div class="white-btn-bg-right"></div>';
    } 
    // Add the Cancel button
    // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
    // each time the form is re-built. 
    $cancelAttributes['class'] = array('addedit-edit-' . $cancelButtonId);
      
    // Using this class to applying bg color to the save button
    $cancelAttributes['class'][] = 'admin-action-button-middle-bg white-btn-bg-middle';
      
    // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
    $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber';
    // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
    $cancelAttributes['data-wrapperid'] = array($wrapperId);
      
    // A common class for all addedit form cancel button, for styling to appear as a link.
    $cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';
    
    $cancelAttributes['onclick'][] = 'Drupal.CTools.Modal.dismiss(); return false;';
    $cancelAttributes['tabindex'][] = $tabIndex + 1;
      
    $form[$cancelButtonId] = array(
      '#type' => 'button',
      '#prefix' => '<div style="clear:both;"></div>' .
                   '<div class="addedit-form-cancel-and-save-actions-row">' .
                     '<div class="addedit-form-cancel-container-actions">'.$prefixBgLeft,
      // Suffix to close the two divs is added in #suffix property of Save button
      '#suffix' => $suffixBgRight,
      '#value' => t('LBL123'), //Close
      '#name' => 'cancel',
      '#attributes' => $cancelAttributes
    );
      
    // Add the Save button
    $ajaxCallbackSave = 'addEditRepaintForm';
      
    // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
    // each time the form is re-built.
    $saveAttributes['class'] = array('addedit-edit-' . $saveButtonId);
      
    // Using this class to applying bg color to the save button
    $saveAttributes['class'][] = 'admin-save-button-middle-bg';
    $saveAttributes['tabindex'][] = $tabIndex;
    // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
    $saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
    // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
    $saveAttributes['data-wrapperid'] = array($wrapperId);

    $eSignatureAvailable = true;
  
    if(module_exists('exp_sp_esignature') && isActiveEsignRegion() && $eSignatureAvailable){ 
      $form[$saveButtonId] = array(
        '#type' => 'submit',
        '#value' => t('LBL141'), // Save
        '#name' => 'save',
        '#prefix' => '<div class="admin-save-button-container">' .
                       '<div class="admin-save-button-left-bg"></div>',
        '#suffix' =>   '<div class="admin-save-button-right-bg"></div>' .
                     '</div>' .
                     '<div class="clearBoth"></div>' .
                   '</div>' . // see #prefix of close button above
                   '<div class="clearBoth"></div>' .
                 '</div>', // see #prefix of close button above
        '#id' => $saveButtonId,
        '#attributes' => $saveAttributes,
        '#ajax' => array(
          'wrapper' => $wrapperId,
          'callback' => $ajaxCallbackSave,
          'method' => 'replace',
          'effect' => 'none', // 'fade',
          'event'=> 'click',
          'drupalUserId' => 'drupal_uid',
          'customButtonId'=> $esignHiddenButt,
          'keypress' => true,
          'progress' => array(
            'type' => 'throbber',
            'message' => ''
          ),
        )
      );
    }
    else {
      $saveAttributes['onclick'] = array();
      $saveAttributes['onclick'][] = '$("#' . $esignHiddenButt . '").click(); return false;';
      $form[$saveButtonId] = array(
        '#type' => 'button',
        '#prefix' => '<div class="admin-save-button-container">' .
                       '<div class="admin-save-button-left-bg"></div>',
        '#suffix' =>   '<div class="admin-save-button-right-bg"></div>' .
                     '</div>' .
                     '<div class="clearBoth"></div>' .
                   '</div>' . // see #prefix of close button above
                   '<div class="clearBoth"></div>' .
                 '</div>', // see #prefix of close button above
        '#value' => t('LBL141'), // Save
        '#name' => 'save',
        '#attributes' => $saveAttributes,
      );
    }
    
    unset($saveAttributes['tabindex']); 
    $saveAttributes['onclick'] = array();
    $form[$esignHiddenButt] = array(
      '#type' => 'submit',
      '#value' => t('hidden-save'),
      '#name' => 'hidden-save',
      '#prefix' => '<span id = "esign-admin-div" style="display:none">',
      '#suffix' => '</span>',
      '#id' =>$esignHiddenButt,
      '#attributes' => $saveAttributes,
      '#ajax' => array(
        'wrapper' => $wrapperId,
        'callback' => $ajaxCallbackSave,
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
    
    // Close form div
    $form['addedit-left-col-end'] = array(
      '#type' => 'markup',
      '#markup' =>   '</div>' .
                   '<div class="clearBoth"></div>',
    );
  }
  catch (Exception $ex) {
    watchdog_exception('addEditFormWithPwdpolicyCancelAndSaveClose', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Fetches the first policy array from the database. It is assumed that there will be only one password policy in ExpertusONE.
 *
 * @return
 *   A populated policy array or NULL if not found
 *   (as done by password_policy.module:pp_policy_load() called automatically by Drupal for wildcard %pp_policy in menu path)
 */
function get_expertus_pwdpolicy() {
  try {
		// Code to fetch password policy is derived from password_policy.module:_password_policy_load_policy_by_pid()
	  $select = db_select('password_policy', 'p', array('fetch' => PDO::FETCH_ASSOC, 'target' => 'slave'));
	  $select->fields('p', array('pid', 'name', 'description', 'expiration', 'warning'));
	  $select->range(0, 1);
	  expDebug::dPrintDBAPI('$select password_policy', $select);
	    
	  $policy = $select->execute()->fetch();
	  expDebug::dPrint('$policy = ' . print_r($policy, true));

	  if ($policy) {
      unset($policy['policy']);
      expDebug::dPrint('returning $policy = ' . print_r($policy, true));
      return $policy;
	  }
	  
	  expDebug::dPrint('returning $policy = FALSE', 5);
	  return FALSE;
  }
  catch (Exception $ex) {
    watchdog_exception('get_expertus_pwdpolicy', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Validate handler for form exp_sp_administration_pwdpolicy_form
 */
function exp_sp_administration_pwdpolicy_form_validate($form, &$form_state) {
	try {
		expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 5);
	  expDebug::dPrint('$form_state[triggering_element] = ' . print_r($form_state['triggering_element'], true), 5);
	
	  $form_state['hidden-save-submit'] = false;
	  $form_state['esign_show_popup']= false;
	  
	  // Trim/cleanup data for validation and for saving to db
	  pwdPolicyAdminCleanupValues($form_state);
    expDebug::dPrint('$form_state[values] after cleanup = ' . print_r($form_state['values'], true), 5);
    
    // Determine which button triggers validation
    if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
      $validationTrigger  = 'save';
    }
    else{
      $validationTrigger  = 'hidden-save';
    }
    
    if ($form_state['triggering_element']['#name'] == $validationTrigger) {
      $form_state['esign_show_popup']= true;
      
      // Perform validation checks
      pwdPolicyAdminCheckMandatoryFields($form, $form_state); 
      pwdPolicyAdminCheckExpirationField($form, $form_state);      
      pwdPolicyAdminCheckWarningField($form, $form_state);
    }
    
    if ($form_state['triggering_element']['#name'] == 'hidden-save' || $form_state['triggering_element']['#ajax']['triggerEvent'] == 'hidden-save') {
      $form_state['hidden-save-submit'] = true;
    }
    
    $form_state['esign_validation_status'] = false; // checked to be false in exp_sp_administration.inc:addEditRepaintForm() as a precondition to
                                                    // showing popup.
    if (!($form_state['hidden-save-submit'])) {
      unset($_SESSION['esign_validate']); // for every button other than hidden-save, unset $_SESSION['esign_validate'].
    }
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_pwdpolicy_form_validate', $ex);
    expertusErrorThrow($ex);
  } 
}

/**
 * Checks the mandatory fields of form exp_sp_administration_pwdpolicy_form
 */
function pwdPolicyAdminCleanupValues(&$form_state) {
	try {
    $form_state['values']['pwdpolicy-name'] = trim($form_state['values']['pwdpolicy-name']);
    
    $form_state['values']['pwdpolicy-description']['value'] = trim($form_state['values']['pwdpolicy-description']['value']);
    
    $form_state['values']['pwdpolicy-expiration'] = trim($form_state['values']['pwdpolicy-expiration']);
    if ($form_state['values']['pwdpolicy-expiration'] == t('MSG674')) { //Enter number
      $form_state['values']['pwdpolicy-expiration'] = '';
    }
    
    $form_state['values']['pwdpolicy-warning'] = trim($form_state['values']['pwdpolicy-warning']);
    if ($form_state['values']['pwdpolicy-warning'] == t('MSG674')) { //Enter number(s)
      $form_state['values']['pwdpolicy-warning'] = '';
    }
    $form_state['values']['pwdpolicy-warning'] = trim($form_state['values']['pwdpolicy-warning'], ','); // also remove any preceeding/succeeding commas
    if (!empty($form_state['values']['pwdpolicy-warning'])) {
      $form_state['values']['pwdpolicy-warning'] = explode(',', $form_state['values']['pwdpolicy-warning']);
      foreach($form_state['values']['pwdpolicy-warning'] as $key => $value) {
        $form_state['values']['pwdpolicy-warning'][$key] = trim($value);
      }
    }
	}
  catch (Exception $ex) {
    watchdog_exception('pwdPolicyAdminCleanupValues', $ex);
    expertusErrorThrow($ex);
  } 
}

/**
 * Checks the mandatory fields of form exp_sp_administration_pwdpolicy_form
 */
function pwdPolicyAdminCheckMandatoryFields($form, &$form_state) {
  try {
    addEditCheckMandatoryFields($form, $form_state, array('pwdpolicy-name')); //, 'pwdpolicy-description'

    $fieldName = 'pwdpolicy-description';
    $fieldValue = $form_state['values'][$fieldName]['value'];
    if (empty($fieldValue)) {
      form_set_error($fieldName, $form[$fieldName]['#title'] . t('ERR101')); // is required.
      $form_state['esign_show_popup']= false;
    }
  }
  catch (Exception $ex) {
    watchdog_exception('pwdPolicyAdminCheckMandatoryFields', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * 
 * Checks the expiration field of form exp_sp_administration_pwdpolicy_form
 */
function pwdPolicyAdminCheckExpirationField($form, &$form_state) {
  try {
    if (!empty($form_state['values']['pwdpolicy-expiration']) && !is_numeric($form_state['values']['pwdpolicy-expiration'])) {
      form_set_error('pwdpolicy-expiration', $form['pwdpolicy-expiration']['#title'] . ' ' . t('ERR200')); //is not a number.
      $form_state['esign_show_popup']= false;
    }
  }
  catch (Exception $ex) {
    watchdog_exception('pwdPolicyAdminCheckExpirationField', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * 
 * Checks the warning field of form exp_sp_administration_pwdpolicy_form
 */
function pwdPolicyAdminCheckWarningField($form, &$form_state) {
	try {
	  if (!empty($form_state['values']['pwdpolicy-warning'])) {
      foreach($form_state['values']['pwdpolicy-warning'] as $key => $value) {
        if (empty($value) || !is_numeric($value)) {
          form_set_error('pwdpolicy-warning', $form['pwdpolicy-warning']['#title'] . ' ' . t('ERR201')); //has a non-numeric value.
          $form_state['esign_show_popup']= false;
          break;
        }
      }
    }
	}
  catch (Exception $ex) {
    watchdog_exception('pwdPolicyAdminCheckWarningField', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Submit handler for form exp_sp_administration_pwdpolicy_form
 */
function exp_sp_administration_pwdpolicy_form_submit($form, &$form_state) {
  expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 5);
  expDebug::dPrint('$form_state[triggering_element] = ' . print_r($form_state['triggering_element'], true), 5);
  expDebug::dPrint('$form_state[storage] = ' . print_r($form_state['storage'], true), 5);
  
  try {
    $eSignValidated = true; 
    if (module_exists('exp_sp_esignature') && isActiveEsignRegion()) {
      if($form_state['hidden-save-submit'] == true && $_SESSION['esign_validate'] ==  1) {
        $form_state['storage']['submit_triggering_element'] = $form_state['storage']['triggering_button_element'];
      }
      else {
        $eSignValidated = false;
        $form_state['storage']['triggering_button_element'] = $form_state['triggering_element']['#name'];
      }
    }
    else {
      $form_state['storage']['submit_triggering_element'] = $form_state['triggering_element']['#name'];
    }
   
    if ($eSignValidated) {
      $form_state['esign_validation_status'] = true;
      $loggedInUserId = getIdOfLoggedInUser();
      if(empty($form_state['values']['pwdpolicy-pid'])) {
        addNewPasswordPolicy($form, $form_state, $loggedInUserId);
        watchdog('exp_admin_password_policy', 'New policy %name created.', array('%name' => $form_state['values']['pwdpolicy-name']));
      }
      else {
        updatePasswordPolicy($form, $form_state, $loggedInUserId);
        watchdog('exp_admin_password_policy', 'Policy %name updated.', array('%name' => $form_state['values']['pwdpolicy-name'])); 
      }
      
      expertus_set_message(t('LBL563').' '.t('MSG600').'.'); //Password policy settings saved successfully.
      $form_state['values'] = array(); // no further use
      $form_state['input'] = array(); // on form rebuild show form default values to user to show data actually saved in db (e.g. trimmed values)
      $form_state['rebuild'] = true; // rebuild form
    }
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_pwdpolicy_form_submit', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * Adds a new password policy record to the DB
 */
function addNewPasswordPolicy($form, &$form_state, $loggedInUserId) {
  $txn = db_transaction();
  try {
  	$insertFields = array(
      'name' => $form_state['values']['pwdpolicy-name'],
      'description' => $form_state['values']['pwdpolicy-description']['value'],
      'policy' => serialize(array()),
      'expiration' => !empty($form_state['values']['pwdpolicy-expiration'])? $form_state['values']['pwdpolicy-expiration']: 0,
      'warning' => (is_array($form_state['values']['pwdpolicy-warning'])?
  	                           implode(',', $form_state['values']['pwdpolicy-warning']) : $form_state['values']['pwdpolicy-warning']),
  	  'enabled' => 1,
  	  'created' => time() //see exp_sp_formatter_password:format_password_policy_admin_list_submit()
    );
    $insertQuery = db_insert('password_policy');
    $insertQuery->fields($insertFields);
    expDebug::dPrintDBAPI('$insertQuery', $insertQuery);
    $pid = $insertQuery->execute();
      
    $insertQuery = db_insert('password_policy_role');
    $insertQuery->fields(array('pid', 'rid'));
    foreach (array(DRUPAL_ANONYMOUS_RID, DRUPAL_AUTHENTICATED_RID) as $rid) {
      $insertQuery->values(array(
        'pid' => $pid,
        'rid' => $rid,
      ));
    }
    expDebug::dPrintDBAPI('$insertQuery', $insertQuery);
    $insertQuery->execute();
  
    $insertFields['roles'] = serialize(array(DRUPAL_ANONYMOUS_RID => DRUPAL_ANONYMOUS_RID, DRUPAL_AUTHENTICATED_RID => DRUPAL_AUTHENTICATED_RID));
    expDebug::dPrint('$insertFields[roles] = ' . $insertFields['roles'], 4);

    $auditFieldsList = array('name', 'description', 'policy', 'expiration', 'warning', 'roles', 'enabled', 'created');
    make_audit_trail_entries($insertFields, $loggedInUserId, null, $pid, 'Inserted',
                                      'password_policy', 'exp_sp_administration_module_info_pwdpolicy', 'addNewPasswordPolicy', $auditFieldsList);
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('addNewPasswordPolicy', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * Updates password policy record in DB
 */
function updatePasswordPolicy($form, &$form_state, $loggedInUserId){
  $txn = db_transaction();
  try {
  	$auditFieldsList = array('name', 'description', 'expiration', 'warning');
  	$prevValues = get_expertus_pwdpolicy_for_audit_trail($form_state['values']['pwdpolicy-pid']);
    $updateFields = array(
      'name' => $form_state['values']['pwdpolicy-name'],
      'description' => $form_state['values']['pwdpolicy-description']['value'],
      'expiration' => !empty($form_state['values']['pwdpolicy-expiration'])?$form_state['values']['pwdpolicy-expiration']:0,
      'warning' => (is_array($form_state['values']['pwdpolicy-warning'])?
                               implode(',', $form_state['values']['pwdpolicy-warning']) : $form_state['values']['pwdpolicy-warning']),
    );
    if ($prevValues[0]->enabled == 0) {
    	$updateFields['enabled'] = 1;
    	$updateFields['created'] = time(); //see exp_sp_formatter_password:format_password_policy_admin_list_submit()
    	
    	$auditFieldsList[] = 'enabled';
    	$auditFieldsList[] = 'created';
    }
    
    $updateQuery = db_update('password_policy')
      ->fields($updateFields)
      ->condition('pid', $form_state['values']['pwdpolicy-pid']);
    expDebug::dPrintDBAPI('$updateQuery', $updateQuery);
    $updateQuery->execute();

    expDebug::dPrint('$auditFieldsList = ' . print_r($auditFieldsList, true), 5);
    make_audit_trail_entries($updateFields, $loggedInUserId, $prevValues, $form_state['values']['pwdpolicy-pid'], 'Updated',
                                      'password_policy', 'exp_sp_administration_module_info_pwdpolicy', 'updatePasswordPolicy', $auditFieldsList);
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('updatePasswordPolicy', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Fetches the policy details of a password policy from the database for audit trail.
 */
function get_expertus_pwdpolicy_for_audit_trail($pid) {
  try {
    $select = db_select('password_policy', 'p', array('target' => 'slave'));
    $select->fields('p'); // fetch all fields
    $select->condition('p.pid', $pid, '=');
    $select->forUpdate();
    expDebug::dPrintDBAPI('$select password_policy', $select);
    $policyList = $select->execute()->fetchAll(); // there would be only one record in the $policyList array
    expDebug::dPrint('$policyList = ' . print_r($policyList, true));

    // policy contraints are saved through a separate form
    unset($policyList[0]->policy);
    
    expDebug::dPrint('$policyList = ' . print_r($policyList, true), 5);
    return $policyList;
  }
  catch (Exception $ex) {
    watchdog_exception('get_expertus_pwdpolicy_for_audit_trail', $ex);
    expertusErrorThrow($ex);
  }
}

?>