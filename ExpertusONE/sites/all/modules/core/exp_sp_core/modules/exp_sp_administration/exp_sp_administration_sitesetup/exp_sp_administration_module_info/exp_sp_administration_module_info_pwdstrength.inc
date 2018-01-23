<?php
/**
 * Password Policy Module Password Strength Admin inc file
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 18-Jun-2013
 */

/**
 * Render and return exp_sp_administration_pwdstrength_form form for displaying in expertus qtip popup
 */
function exp_sp_administration_pwdstrength_display_form($pwdPolicyId) {
	try {
	  $form_state = array(
	    'no_redirect'  => TRUE,
	    'build_info'   => array('args' => array($pwdPolicyId)),
	  );
	  
	  $formId = 'exp_sp_administration_pwdstrength_form';
	  $builtForm = drupal_build_form($formId, $form_state);
	  $renderedForm = drupal_render($builtForm);
	  $drupalSettings = getDrupalSettings();
	
	  $response = array(
	    'drupal_settings' => $drupalSettings,
	    'render_content_main' => $renderedForm,
	  ); 
	 
	  echo drupal_json_output($response);
	  exit;
	}
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_pwdstrength_display_form', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Build the form for password policy strength
 */
function exp_sp_administration_pwdstrength_form($form, $form_state, $pwdPolicyId) {
	try {
    // Form wrapper id
    $formWrapperId = 'pwdstrength-admin-form';
    
    // Get current values from db
    $policyConstraints = get_expertus_pwdpolicy_constraints($pwdPolicyId);
    
		// New form
    $form = newAddEditFormWithCancelAndSave($formWrapperId);
    
    // Add password policy id field (hidden)
    addHiddenValueToAddEditForm($form, 'pwdstrength-pid', $pwdPolicyId);
    
    // Add all form fields excluding the action buttons inside a div wrapper to have them scrollbable
    $form['fields-wrapper-start'] = array(
      '#markup' => '<div id="pwdstrength-admin-form-fields" class="pwdstrength-admin-form-fields">'
    );
    
    // Add fields
    $defaultValue = $policyConstraints['alphanumeric']? $policyConstraints['alphanumeric'] : '';
    addPwdStrengthTextFieldToAddEditForm($form, 'pwdstrength-alphanumeric', t('LBL1045'), $defaultValue, 31, //Minimum number of alphanumeric characters
                                                t('MSG638')); //alphanumeric characters are alphabets or numbers

    $defaultValue = $policyConstraints['complexity']? $policyConstraints['complexity'] : '';
    addPwdStrengthTextFieldToAddEditForm($form, 'pwdstrength-complexity', t('LBL1046'), $defaultValue, 32, //Minimum number of character types
                                                t('MSG639')); //character types are lowercase, uppercase, digit or punctuation
    
    $defaultValue = $policyConstraints['delay']? $policyConstraints['delay'] : '';
    addPwdStrengthTextFieldToAddEditForm($form, 'pwdstrength-delay', t('LBL1047'), //Minimum number of hours between password changes
                                                 $defaultValue, 33);

    $defaultValue = $policyConstraints['digit']? $policyConstraints['digit'] : '';
    addPwdStrengthTextFieldToAddEditForm($form, 'pwdstrength-digit', t('LBL1048'), $defaultValue, 34); //Minimum number of digits

    $defaultValue = $policyConstraints['digit_placement']? $policyConstraints['digit_placement'] : '';
    addPwdStrengthTextFieldToAddEditForm($form, 'pwdstrength-digit_placement', t('LBL1049'), //Minimum number of digits in the first or last position
                                                $defaultValue, 35, t('MSG640')); //e.g. 2qpcxrm3 and 99qpcxrm are allowed passwords when 2 is set here

    $defaultValue = $policyConstraints['history']? $policyConstraints['history'] : '';
    addPwdStrengthTextFieldToAddEditForm($form, 'pwdstrength-history', t('LBL1050'), $defaultValue, 36); //Allow repeat of passwords after

    $defaultValue = $policyConstraints['length']? $policyConstraints['length'] : '';
    addPwdStrengthTextFieldToAddEditForm($form, 'pwdstrength-length', t('LBL1051'), $defaultValue, 37); //Minimum password length

    $defaultValue = $policyConstraints['letter']? $policyConstraints['letter'] : '';
    addPwdStrengthTextFieldToAddEditForm($form, 'pwdstrength-letter', t('LBL1052'), $defaultValue, 38); //Minimum number of alphabets

    $defaultValue = $policyConstraints['lowercase']? $policyConstraints['lowercase'] : '';
    addPwdStrengthTextFieldToAddEditForm($form, 'pwdstrength-lowercase', t('LBL1053'), $defaultValue, 39); //Minimum number of lowercase alphabets

    $defaultValue = $policyConstraints['punctuation']? $policyConstraints['punctuation'] : '';
    addPwdStrengthTextFieldToAddEditForm($form, 'pwdstrength-punctuation', t('LBL1054'), $defaultValue, 40, //Minimum number of punctuation characters
                                              t('MSG641')); //not white space or alphanumeric

    $defaultValue = $policyConstraints['uppercase']? $policyConstraints['uppercase'] : '';
    addPwdStrengthTextFieldToAddEditForm($form, 'pwdstrength-uppercase', t('LBL1055'), $defaultValue, 41); //Minimum number of uppercase alphabets

    $defaultValue = $policyConstraints['username'] && $policyConstraints['username'] > 0? 1 : 0;
    addPwdStrengthCheckBoxToAddEditForm($form, 'pwdstrength-username', t('LBL1056'), $defaultValue, 42); //Enforce password to differ from the user name

    // Close the wrapper div for all form fields excluding the action buttons
    $form['fields-wrapper-end'] = array(
      '#markup' => '</div>'
    );
    
    // Add action buttons and close the form
    addEditFormWithPwdstrengthCancelAndSaveClose($form, $formWrapperId, 43);
    
    expDebug::dPrint('$form = ' . print_r($form, true), 5);
		return $form;
	}
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_pwdstrength_form', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Fetches the policy contraints of a password policy
 */
function get_expertus_pwdpolicy_constraints($policyId, $forAuditTrail = false) {
  try {
  	expDebug::dPrint('$policyId = ' . $policyId, 4);
    $select = db_select('password_policy', 'p');
    $select->addField('p', 'policy', 'policy');
    $select->condition('p.pid', $policyId, '=');
    expDebug::dPrintDBAPI('$select', $select);

    $policyConstraints = $select->execute()->fetchField();
    expDebug::dPrint('$policyConstraints = ' . $policyConstraints);
    
    if ($forAuditTrail) {
    	$recObj = new stdClass();
    	$recObj->policy = $policyConstraints;
    	return array($recObj);
    }

    if (!empty($policyConstraints)) {
      $policyConstraints = unserialize($policyConstraints);
      expDebug::dPrint('$policyConstraints = ' . print_r($policyConstraints, true), 5);
      return $policyConstraints;
    }

    return array();
  }
  catch (Exception $ex) {
    watchdog_exception('get_expertus_pwdpolicy_constraints', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Adds a text field to the password strength form
 */
function addPwdStrengthTextFieldToAddEditForm(&$form, $id, $title, $defaultValue, $tabIndex = null, $titleHelp = false, $mandatory = false) {
	try {
  $maxChars = 99999;
  $viewWidthInChars = 60;
  
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
    
  // If the view width is larger than max allowed characters, make view width equal to the max allowed characters 
  if ($viewWidthInChars > $maxChars) {
    $viewWidthInChars = $maxChars;
  }

  $prefix = pwdStrengthGetFieldAffix('prefix', $title, $titleHelp, $mandatory);
  $suffix = pwdStrengthGetFieldAffix('suffix');
  
  // Create the form element
  $form[$id] = array(
    '#type' => 'textfield',
    '#prefix' => $prefix,
    '#suffix' => $suffix,
    '#title' => $title,
    '#title_display' => 'invisible',
    '#default_value' => $defaultValue,
    '#attributes' => $attributes,
    '#size' => $viewWidthInChars,
    '#maxlength' => $maxChars,
    '#description' => false
  );
	}
  catch (Exception $ex) {
    watchdog_exception('addPwdStrengthTextFieldToAddEditForm', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Adds a checkbox to the password strength form
 */
function addPwdStrengthCheckBoxToAddEditForm(&$form, $id, $title, $defaultValue, $tabIndex = null, $titleHelp = false, $mandatory = false) {
	try {
	  global $theme_key;
	  $attributes = array();
	  
	  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
	  // each time the form is re-built.
	  $attributes['class'] = array('addedit-edit-' . $id);
	  if($theme_key == 'expertusoneV2') {
	    $attributes['onclick'][] = 'checkboxSelectedUnselectedCommon(this);';
	  }
	  // Add class to apply common style to all checkbox fields
	  $attributes['class'][] = 'addedit-edit-checkbox';
	    
	  if (isset($tabIndex) && $tabIndex > 0) {
	    $attributes['tabindex'] = $tabIndex;
	  }
	
	    $prefix = pwdStrengthGetFieldAffix('prefix', $title, $titleHelp, $mandatory);
	    $suffix = pwdStrengthGetFieldAffix('suffix');
	  
	  $form[$id] = array (
	    '#type' => 'checkbox',
	    '#prefix' => $prefix,
	    '#suffix' => $suffix,
	    '#field_prefix' => ($theme_key == 'expertusoneV2') ? '<div class="'.($defaultValue == 1 ? 'checkbox-selected' : 'checkbox-unselected').'">' : '',
	    '#field_suffix' => ($theme_key == 'expertusoneV2') ? '</div>' : '',
	    '#title' => $title,
	    '#title_display' => 'invisible',
	    '#default_value' =>  $defaultValue,
	    '#disabled' => false,
	    '#attributes' => $attributes
	  );
	}
  catch (Exception $ex) {
    watchdog_exception('addPwdStrengthCheckBoxToAddEditForm', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Provides HTML for #prefix and #suffix for the form fields of password strength form
 */
function pwdStrengthGetFieldAffix($affixType, $title = '', $titleHelp = false, $mandatory = false) {
	try {
		if ($affixType == 'prefix') {
		  $mandatorySymbol = "<span class='addedit-mandatory'>*</span>";  
		  $affix = '<div class="addedit-new-field">' .
		              '<div class="addedit-new-field-title-wrapper">' .
		                '<div class="addedit-new-field-title">' .
		                  $title . ':' . ($mandatory? $mandatorySymbol : '') .
		                '</div>' .
		                ($titleHelp? '<div class="addedit-new-field-title-hint">' . '(' . $titleHelp . ')' . '</div>' : '') .
		              '</div>' .
		              '<div class="addedit-new-field-value">';
		}
		else {
			$affix = '</div>' .
                '<div class="clearBoth"></div>' .
              '</div>';
		}

		return $affix;
	}
  catch (Exception $ex) {
    watchdog_exception('pwdStrengthGetFieldAffix', $ex);
    expertusErrorThrow($ex);
  }
}


/**
 * Adds cancel and save button to exp_sp_administration_pwdstrength_form
 */
function addEditFormWithPwdstrengthCancelAndSaveClose(&$form, $wrapperId, $tabIndex) {
  try {
    // Button ids
    $saveButtonId = 'pwdstrength-save-button';
    $cancelButtonId = 'pwdstrength-cancel-button';
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
    
    $cancelAttributes['onclick'][] = "closeQtip('','$cancelButtonId');  return false; ";
    $cancelAttributes['tabindex'][] = $tabIndex + 1;
      
    $form[$cancelButtonId] = array(
      '#type' => 'button',
      '#prefix' => '<div style="clear:both;"></div>' .
                   '<div class="addedit-form-cancel-and-save-actions-row">' .
                     '<div class="addedit-form-cancel-container-actions">'.$prefixBgLeft,
     '#suffix' => $suffixBgRight,
      // Suffix to close the two divs is added in #suffix property of Save button
      '#value' => t('LBL123'), //Close
      '#name' => 'cancel',
      '#attributes' => $cancelAttributes
    );
      
    // Add the Save button
    $ajaxCallbackSave = 'pwdStrengthAdminRepaintForm';
      
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

    $form[$saveButtonId] = array(
      '#type' => 'submit',
      '#prefix' => '<div class="admin-save-button-container">' .
                     '<div class="admin-save-button-left-bg"></div>',
      '#suffix' =>   '<div class="admin-save-button-right-bg"></div>' .
                   '</div>' .
                   '<div class="clearBoth"></div>' .
                 '</div>' . // see #prefix of close button above
                 '<div class="clearBoth"></div>' .
               '</div>', // see #prefix of close button above
      '#value' => t('LBL141'), // Save
      '#name' => 'pwdstrength-save',
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
    watchdog_exception('addEditFormWithPwdstrengthCancelAndSaveClose', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * #ajax callback for form submit button
 */
function pwdStrengthAdminRepaintForm($form, &$form_state) {
  try {
      $commands = array();
      $commands[] = ajax_command_replace(NULL, drupal_render($form));
      
      $statusMsgHTML = theme('status_messages');
      if (!empty($statusMsgHTML)) {
        // Show the messages
        $commands[] = ajax_command_html('.pwdstrength-admin-popup #show_expertus_message', $statusMsgHTML);
      }
      return array('#type' => 'ajax', '#commands' => $commands);
  }
  catch (Exception $ex) {
    watchdog_exception('pwdStrengthAdminRepaintForm', $ex);
    expertusErrorThrow($ex);
  }      
}

/**
 * Validation handler of form exp_sp_administration_pwdstrength_form
 */
function exp_sp_administration_pwdstrength_form_validate($form, &$form_state) {
	try {
		expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 5);
		$fieldsList = array('alphanumeric', 'complexity', 'delay', 'digit', 'digit_placement', 'history', 'length', 'letter', 'lowercase', 'punctuation',
                      'uppercase');
		
		foreach ($fieldsList as $field) {
			$formFieldId = 'pwdstrength-' . $field;
			$form_state['values'][$formFieldId] = trim($form_state['values'][$formFieldId]);
			if (!empty($form_state['values'][$formFieldId]) && !is_numeric($form_state['values'][$formFieldId])) {
				expDebug::dPrint('field is non-numeric ' . $formFieldId, 5);
				form_set_error($formFieldId, $form[$formFieldId]['#title'] . ' ' . t('ERR200')); //is not a number.
				$form_state['esign_show_popup']= false;
			} //end if
		} // end foreach
	}
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_pwdstrength_form_validate', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Submit handler of form exp_sp_administration_pwdstrength_form
 */
function exp_sp_administration_pwdstrength_form_submit($form, &$form_state) {
  try {
    $loggedInUserId = getIdOfLoggedInUser();
    updatePasswordStrength($form, $form_state, $loggedInUserId);
    watchdog('exp_admin_password_policy', 'Password strenth for policy %id updated.', array('%id' => $form_state['values']['pwdstrength-pid'])); 
    expertus_set_message(t('MSG600').'.'); //Password policy strength saved successfully.
    $form_state['values'] = array(); // no further use
    $form_state['input'] = array(); // on form rebuild show form default values to user to show data actually saved in db (e.g. trimmed values)
    $form_state['rebuild'] = true; // rebuild form    
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_pwdstrength_form_submit', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Updates password strength of a policy in DB
 */
function updatePasswordStrength($form, &$form_state, $loggedInUserId) {
  $txn = db_transaction();
  try {
  	expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 5);
    $fieldsList = array('alphanumeric', 'complexity', 'delay', 'digit', 'digit_placement', 'history', 'length', 'letter', 'lowercase', 'punctuation',
                      'uppercase', 'username');
    $prevValues = get_expertus_pwdpolicy_constraints($form_state['values']['pwdstrength-pid'], true);
    expDebug::dPrint('$prevValues = ' . print_r($prevValues, true), 5);
    
    $pwdStrengthFieldsList = array();
    foreach ($fieldsList as $field) {
      $formFieldId = 'pwdstrength-' . $field;
      if (!empty($form_state['values'][$formFieldId])) {
      	$pwdStrengthFieldsList[$field] = strval($form_state['values'][$formFieldId]);
      }
    }
    expDebug::dPrint('$pwdStrengthFieldsList = ' . print_r($pwdStrengthFieldsList, true), 5);
    
    $updateFields = array(
      'policy' => serialize($pwdStrengthFieldsList),
    );
      
    $updateQuery = db_update('password_policy')
      ->fields($updateFields)
      ->condition('pid', $form_state['values']['pwdstrength-pid']);
    expDebug::dPrintDBAPI('$updateQuery', $updateQuery);
    $updateQuery->execute();

    make_audit_trail_entries($updateFields, $loggedInUserId, $prevValues, $form_state['values']['pwdstrength-pid'], 'Updated',
                                      'password_policy', 'exp_sp_administration_module_info_pwdpolicy', 'updatePasswordStrength', array('policy'));
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('updatePasswordStrength', $ex);
    expertusErrorThrow($ex);
  }
}

?>