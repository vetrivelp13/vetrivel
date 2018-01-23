<?php
/**
 * Commerce Settings Payment Method Module Credit Card Settings Admin inc file
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 04-Jul-2013
 */

/**
 * Render credit card settings screen for displaying in expertus qtip popup
 */
function exp_sp_administration_commerce_cc_settings_display_form() {
  try {
    $form_state = array(
      'no_redirect'  => TRUE,
      'selected_tab' => 'basic'  // by default basic settings tab is selected
    );
    
    $leftTabsFormId = 'exp_sp_administration_commerce_cc_settings_tabs_form';
    $builtLeftTabsForm = drupal_build_form($leftTabsFormId, $form_state);
    $renderedTabs = drupal_render($builtLeftTabsForm);

    $form_state = array(
      'no_redirect'  => TRUE
    );
    $defaultSettingsFormId = 'exp_sp_administration_commerce_cc_basic_settings_form';
    $builtSettingsForm = drupal_build_form($defaultSettingsFormId, $form_state);
    $renderedSettingsForm = drupal_render($builtSettingsForm);
    
    $renderedCcSettingsScreen = theme('commerce-payment-cc-settings-screen', array('rendered_tabs' => $renderedTabs,
                                                                       'rendered_settings_form' => $renderedSettingsForm));

    $drupalSettings = getDrupalSettings();
  
    $response = array(
      'drupal_settings' => $drupalSettings,
      'render_content_main' => $renderedCcSettingsScreen,
    ); 
   
    echo drupal_json_output($response);
    exit;
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_commerce_cc_settings_display_form', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Build the form for left tabs
 */
function exp_sp_administration_commerce_cc_settings_tabs_form($form, $form_state) {
	try {
	  $form = array();
	  $form['tabs-list-start'] = array(
	    '#markup' => '<ul>'
	  );

	  // Add Basic Settings tab button
    $isSelected = ($form_state['selected_tab'] == 'basic' || empty($form_state['selected_tab']))? 'selected' : '';
    expDebug::dPrint('basic $isSelected = ' . $isSelected ,4);
    $form['cc-settings-basic'] = array(
      '#type' => 'submit',      
      '#value' => t('LBL1075'), //Basic Settings
      '#name'  => 'basic',
/* Sample code to show tab label help text    
      '#prefix' => '<li>' .
                     '<span class="' . $isSelected . ' commerce-cc-settings-other-wrapper">' .
                       '<div class="commerce-cc-settings-other-button">',
      '#suffix' =>     '</div>'.
                       '<div class="commerce-cc-settings-other-hint">' .
                         t('Key setting') . ', ' . //Key settings
                         t('CVV') . ', ' . //CVV
                         t('Card type') . //Card type
                       '</div>' .
                     '</span>' .
                   '</li>', */
      '#prefix' => '<li>' .
                     '<span class="' . $isSelected . '">',
      '#suffix' =>   '</span>' .
                   '</li>',
      '#attributes' => array(
        'class' => array('form-submit', 'commerce-cc-settings-basic', 'addedit-form-expertusone-throbber'),
        'data-wrapperid' => 'commerce-cc-settings-screen-wrapper'
      ),
      '#ajax' => array(  
        'callback' => 'commerceCcSettingsRepaintScreen',
        'wrapper'  => 'commerce-cc-settings-screen-wrapper',
        'event'    => 'click'
      ), 
    );

    $gatewaysData = _uc_payment_gateway_list('credit'); //taken from uc_credit.admin.inc:uc_credit_settings_form()
    $gatewaysList = array_keys($gatewaysData);
    $paymentGateway = variable_get('uc_payment_credit_gateway', "0");
    
   // Add Cybersource settings tab button
    if (in_array('exp_cybersource', $gatewaysList) && $paymentGateway == 'exp_cybersource') {
		  $isSelected = ($form_state['selected_tab'] == 'cybersource')? 'selected' : '';
		  expDebug::dPrint('cybersource $isSelected = ' . $isSelected ,4);
		  $form['cc-settings-cybersource'] = array(
		    '#type' => 'submit',      
		    '#value' => t('LBL1076'), //Cybersource
		    '#name'  => 'cybersource',
		    '#prefix' => '<li>' .
		                   '<span class="' . $isSelected . ' notfirst">',   
		    '#suffix' =>   '</span>' .
		                 '</li>',
		    '#attributes' => array(
		      'class' => array('form-submit', 'commerce-cc-settings-cybersource', 'addedit-form-expertusone-throbber'),
		      'data-wrapperid' => 'commerce-cc-settings-screen-wrapper'
		    ),
		    '#ajax' => array(  
		      'callback' => 'commerceCcSettingsRepaintScreen',
		      'wrapper'  => 'commerce-cc-settings-screen-wrapper',
		      'event'    => 'click'
		    ), 
		  );
    }

    // Add Paymetric settings tab button // 0046427 - Paymetric gateway disabled in 4.5 Release. Now it has been enabled.
    if (in_array('exp_paymetric', $gatewaysList)  && $paymentGateway == 'exp_paymetric') {
		  $isSelected = ($form_state['selected_tab'] == 'paymetric')? 'selected' : '';
		  expDebug::dPrint('paymetric $isSelected = ' . $isSelected, 4);
		  $form['cc-settings-paymetric'] = array(
		    '#type' => 'submit',      
		    '#value' => t('LBL1077'), //Paymetric
		    '#name'  => 'paymetric',
		    '#prefix' => '<li>' .
		                   '<span class="' . $isSelected . ' notfirst">',
		    '#suffix' =>   '</span>' .
		                 '</li>',
		    '#attributes' => array(
		      'class' => array('form-submit', 'commerce-cc-settings-paymetric', 'addedit-form-expertusone-throbber'),
		      'data-wrapperid' => 'commerce-cc-settings-screen-wrapper'
		    ),
		    '#ajax' => array(  
		      'callback' => 'commerceCcSettingsRepaintScreen',
		      'wrapper'  => 'commerce-cc-settings-screen-wrapper',
		      'event'    => 'click'
		    ),
		  );
    }
    
     // Add Paypal settings tab button 
    if (in_array('exp_paypal', $gatewaysList)  && $paymentGateway == 'exp_paypal') {
		  $isSelected = ($form_state['selected_tab'] == 'paypal')? 'selected' : '';
		  expDebug::dPrint('paypal $isSelected = ' . $isSelected, 4);
		  $form['cc-settings-paypal'] = array(
		    '#type' => 'submit',      
		    '#value' => t('LBL3008'), //Paypal
		    '#name'  => 'paypal',
		    '#prefix' => '<li>' .
		                   '<span class="' . $isSelected . ' notfirst">',
		    '#suffix' =>   '</span>' .
		                 '</li>',
		    '#attributes' => array(
		      'class' => array('form-submit', 'commerce-cc-settings-paypal', 'addedit-form-expertusone-throbber'),
		      'data-wrapperid' => 'commerce-cc-settings-screen-wrapper'
		    ),
		    '#ajax' => array(  
		      'callback' => 'commerceCcSettingsRepaintScreen',
		      'wrapper'  => 'commerce-cc-settings-screen-wrapper',
		      'event'    => 'click'
		    ),
		  );
    }
    
	   
	  return $form;
	}
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_commerce_cc_settings_tabs_form', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Submit handler for the tabs form
 */
function exp_sp_administration_commerce_cc_settings_tabs_form_submit($form, &$form_state) {
	try {
	  switch ($form_state['triggering_element']['#name']) {
      case 'basic':
        $form_state['selected_tab'] = 'basic';
        break;
	    case 'cybersource':
	      $form_state['selected_tab'] = 'cybersource';
	      break;
	    case 'paymetric':
	    	$form_state['selected_tab'] = 'paymetric';
	    	break;
	    case 'paypal':
	    	$form_state['selected_tab'] = 'paypal';
	    	break;
	 
	  }
	  $form_state['rebuild'] = TRUE;
	}
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_commerce_cc_settings_tabs_form_submit', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Ajax callback function for left tab form buttons
 */
function commerceCcSettingsRepaintScreen($leftTabsForm, $left_tabs_form_state) {
	try {
	  $renderedTabs = drupal_render($leftTabsForm);
	  
	  switch ($left_tabs_form_state['selected_tab']) {
      case 'basic':
        $settingsFormId = 'exp_sp_administration_commerce_cc_basic_settings_form';
        break;
	    case 'cybersource':
	      $settingsFormId = 'exp_sp_administration_commerce_cc_cybersource_settings_form';
	      break;
	    case 'paymetric':
	      $settingsFormId = 'exp_sp_administration_commerce_cc_paymetric_settings_form';
	      break;
	    case 'paypal':
	      $settingsFormId = 'exp_sp_administration_commerce_cc_paypal_settings_form';
	      break;

	  }
	
	  $form_state = array(
	    'no_redirect'  => TRUE
	  );
	  
	  $builtSettingsForm = drupal_build_form($settingsFormId, $form_state);
	  $renderedSettingsForm = drupal_render($builtSettingsForm);
	  $renderedCcSettingsScreen = theme('commerce-payment-cc-settings-screen',
	                                      array('rendered_tabs' => $renderedTabs, 'rendered_settings_form' => $renderedSettingsForm));
	  $commands = array();
	  $commands[] = ajax_command_replace(NULL, $renderedCcSettingsScreen);
	  
	  expDebug::dPrint(' $commands = ' . print_r($commands, true) , 4);
	  return array('#type' => 'ajax', '#commands' => $commands);
	}
  catch (Exception $ex) {
    watchdog_exception('commerceCcSettingsRepaintScreen', $ex);
    expertusErrorThrow($ex);
  }
}


/**
 * Adds close and save buttons to the credit card settings form and closes the form
 * (Used by basic settings, cybersource and paymetric forms)
 */
function expAdminCreditCardSettingsFormClose($formType, &$form, $wrapperId, $tabIndex) {
  try {
    global $theme_key;
    if($theme_key == 'expertusoneV2'){
    	$prfixPgLeft   = '<div class="white-btn-bg-left"></div>';
    	$suffixPgRight = '<div class="white-btn-bg-right"></div>';
    	$cancelAttributes['class'][] = 'white-btn-bg-middle';
    	$alignBottom      = '<div class="cancel-align-bottom">';
    	$alignBottomCLose = '</div>';
    } 
    // Button ids
    $saveButtonId = $formType . '-save-button';
    $cancelButtonId = $formType . '-cancel-button';
    
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
    
    $cancelAttributes['onclick'][] = "closeQtip('','$cancelButtonId');  return false;";
    $cancelAttributes['tabindex'][] = $tabIndex + 1;
      
    $form[$cancelButtonId] = array(
      '#type' => 'button',
      '#prefix' => '<div style="clear:both;"></div>' .
                   '<div class="addedit-form-cancel-and-save-actions-row">' .
                     '<div class="addedit-form-cancel-container-actions">'.$alignBottom.$prfixPgLeft,
      '#suffix' =>  $suffixPgRight.$alignBottomCLose,
      // Suffix to close the two divs is added in #suffix property of Save button
      '#value' => t('LBL123'), //Close
      '#name' => $cancelButtonId,
      '#attributes' => $cancelAttributes
    );
      
    // Add the Save button
    $ajaxCallbackSave = 'expAdminCreditCardSettingsRepaintForm';
      
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
      '#name' => $saveButtonId,
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
    watchdog_exception('expAdminCreditCardSettingsFormClose', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Ajax callback function for Save button in the credit card settings form
 */
function expAdminCreditCardSettingsRepaintForm($settings_form, $settings_form_state) {
  try {
  	$commands = array();
  	
  	$renderedSettingsForm = drupal_render($settings_form);
  	if (isset($settings_form_state['rebuild_payment_screen'])) { //set only by submit handler for basic settings screen
  		// Build and re-render the whole payment method screen
      $tabs_form_state = array(
        'no_redirect'  => TRUE,
        'selected_tab' => 'basic'  // by default basic settings tab is selected
      );
    
      $leftTabsFormId = 'exp_sp_administration_commerce_cc_settings_tabs_form';
      $builtLeftTabsForm = drupal_build_form($leftTabsFormId, $tabs_form_state);
      $renderedTabs = drupal_render($builtLeftTabsForm);

      $renderedCcSettingsScreen = theme('commerce-payment-cc-settings-screen',
                                        array('rendered_tabs' => $renderedTabs, 'rendered_settings_form' => $renderedSettingsForm));

      $commands[] = ajax_command_replace('#commerce-cc-settings-screen-wrapper', $renderedCcSettingsScreen);
  	}
  	else {
	    // Re-render only the form
	    $commands[] = ajax_command_replace(NULL, $renderedSettingsForm);
  	}
      
    // Shown any status messages
    $statusMsgHTML = theme('status_messages');
    if (!empty($statusMsgHTML)) {
      $commands[] = ajax_command_html('#bubble-face-table #show_expertus_message', $statusMsgHTML);
    }
    
    expDebug::dPrint('$commands = ' . print_r($commands, true), 5);
    return array('#type' => 'ajax', '#commands' => $commands);
  }
  catch (Exception $ex) {
    watchdog_exception('expAdminCreditCardSettingsRepaintForm', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Build the basic settings form
 */
function exp_sp_administration_commerce_cc_basic_settings_form() {
  try {
    $gateways = _uc_payment_gateway_list('credit');

    // Form wrapper id
    $formWrapperId = 'commerce-cc-settings-basic-form';

    // New form
    $form = newAddEditFormWithCancelAndSave($formWrapperId);
    
    addTwoColumnRowToAddEditForm($form, 'basic-twocolrow-1');
    
    // Add Payment Gateway drop-down
    $paymentGatewayOptions = array();
    foreach ($gateways as $id => $gateway) {
      if ($id == 'exp_cybersource') {
        $paymentGatewayOptions[$id] = t('LBL1076'); //Cybersource
      }
      else if ($id == 'exp_paymetric') {
        $paymentGatewayOptions[$id] = t('LBL1077'); //Paymetric
      }
      else if ($id == 'exp_paypal') {
       $paymentGatewayOptions[$id] = "Paypal"; //Paypal 
      }
    }
    $defaultValue = variable_get('uc_payment_credit_gateway', "0");
    addDropDownToAddEditForm($form, 'uc_payment_credit_gateway', t('LBL1093'), $paymentGatewayOptions, $defaultValue, 1, true); //Payment Gateway

    // Add Card Types multi-select drop-down
    $cardTypeOptions = array('visa' => t('LBL1094'), //Visa
                             'mastercard' => t('LBL1095'), //Mastercard
                             'discover' => t('LBL1096'), //Discover
                             'amex' => t('LBL1097')); //American Express
    $defaultValue = getSelectedCardTypesForBasicSettingsForm();
    // Add multi-select field (when saving this form googleanalytics_visibility_roles will always be set to 0)
    addMultiselectDropDownToAddEditForm($form, 'exp-card-types', t('LBL1098'), $cardTypeOptions, //Roles
                                               $defaultValue, 2, true, false, array('size' => 1)); //size attribute added to minimize any flicker    
    endAddEditTwoColumnRow($form);

    addTwoColumnRowToAddEditForm($form, 'basic-twocolrow-2');

    // Add Card number encryption key filepath text field
    $defaultValue = variable_get('uc_credit_encryption_path', t('Not configured, see below.')); //uc string - for internal use
    addEncryptionKeyTextFieldToBasicSettingsForm($form, 'uc_credit_encryption_path', t('LBL1099'), $defaultValue, 3, true); //Encryption Key
    
    // Add Request CVV checkbox
    $defaultValue = variable_get('uc_credit_cvv_enabled', TRUE);
    // #0057641 For Paymetric the cvv is must. henceforth add this condition to disable the cvv and pass the value as 1 in 'uc_credit_cvv_enabled' variable
    $checkgateway = variable_get('uc_payment_credit_gateway', "0");
    if($checkgateway == 'exp_paymetric'){
    	$checked = 1;// hard coded since for paymetric it is must required for cvv
        addCheckBoxToAddEditForm($form, 'uc_credit_cvv_enabled', t('LBL1100'), true, $checked, 4); //Request CVV
        $form['uc_credit_cvv_enabled']['#disabled'] = TRUE;
    }else{
    	addCheckBoxToAddEditForm($form, 'uc_credit_cvv_enabled', t('LBL1100'), true, $defaultValue, 4); //Request CVV
    }
    
    endAddEditTwoColumnRow($form);
    
    // Add action buttons and close the form
    expAdminCreditCardSettingsFormClose('basic', $form, $formWrapperId, 5);
        
    expDebug::dPrint('$form = ' . print_r($form, true), 5);
    return $form;
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_commerce_cc_basic_settings_form', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Add encryption key text field to the basic settings form
 */
function addEncryptionKeyTextFieldToBasicSettingsForm(&$form, $id, $title = '', $defaultValue = '', $tabIndex = null, $mandatory = false) {
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
    $attributes['data-default-text'] = array(t('MSG655')); //Enter file path
    $attributes['data-uc-default-text'] = array(t('Not configured, see below.')); //uc string - for internal use
  
    $mandatorySymbol = "<span class='addedit-mandatory'>*</span>";
    $separatorSymbol = ':';
    
    $prefix = '<div class= "addedit-twocol-firstcol">' .
                '<div class="addedit-new-field-title-wrapper">' .
                  '<div class="addedit-new-field-title">' .
                    $title . $separatorSymbol . ($mandatory? $mandatorySymbol : '') .
                  '</div>' .
                  '<div class="addedit-new-field-title-hint">' .
                    '(' . t('MSG653') . ')' . //for storing card details
                  '</div>' .
                '</div>' .
                '<div class="addedit-firstcol-field-value">';
    $suffix =   '</div>' .
                '<div class="clearBoth"></div>' .
              '</div>';
    
    // increment current column
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
    watchdog_exception('addEncryptionKeyTextFieldToBasicSettingsForm', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Prepare a list of selected types to specify the default value for card types field in the basic settings form
 */
function getSelectedCardTypesForBasicSettingsForm() {
  try {
    $selectedCcTypes = array();
    $ccTypes = array('visa', 'mastercard', 'discover', 'amex');
    foreach ($ccTypes as $type) {
      if (variable_get('uc_credit_' . $type, TRUE)) {
        $selectedCcTypes[$type] = $type;
      }
    }
    
    expDebug::dPrint('$selectedCcTypes = ' . print_r($selectedCcTypes, true), 5);
    return $selectedCcTypes;
  }
  catch (Exception $ex) {
    watchdog_exception('getSelectedCardTypesForBasicSettingsForm', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Validate handler for the basic settings form
 */
function exp_sp_administration_commerce_cc_basic_settings_form_validate($form, &$form_state) {
  expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 5);
  try {
    //Cleanup
    unset($form_state['rebuild_payment_screen']); // this was set in submit handler and needs to be removed asap.

    // Trim text fields
    $form_state['values']['uc_credit_encryption_path'] = trim($form_state['values']['uc_credit_encryption_path']);
    if ($form_state['values']['uc_credit_encryption_path'] == t('MSG655')) { //Enter file path
      $form_state['values']['uc_credit_encryption_path'] = '';
    }
    $form_state['values']['uc_credit_encryption_path'] = rtrim($form_state['values']['uc_credit_encryption_path'], '/\\');
    
    // Check all mandatory fields in order
    addEditCheckMandatoryFields($form, $form_state, array('uc_payment_credit_gateway'));
    $fieldName = 'exp-card-types';
    $fieldValue = $form_state['values'][$fieldName];
    if (empty($fieldValue)) {
      form_set_error($fieldName, $form[$fieldName]['#title'] . t('ERR101')); // is required.
      $form_state['esign_show_popup']= false;
    }
    addEditCheckMandatoryFields($form, $form_state, array('uc_credit_encryption_path'));
    
    // Check uc_credit_encryption path
    validateEncryptionKeyPathForBasicSettingsForm($form_state['values']['uc_credit_encryption_path']);
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_commerce_cc_basic_settings_form_validate', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Validate encryption key directory path is a valid directory path.
 * Code from uc_credit.admin.inc:uc_credit_settings_form_validate() used here with minor modifications.
 */
function validateEncryptionKeyPathForBasicSettingsForm($newDir) {
  // Get currently configured value
  try{
  $currentDir = variable_get('uc_credit_encryption_path', t('Not configured, see below.')); //uc string - for internal use
  if ($currentDir == t('Not configured, see below.')) { //uc string - for internal use
    $currentDir = '';
  }

  // If the value is being changed by admin
  if (!empty($newDir) && $newDir !== $currentDir) {
    $_SESSION['update_cc_encrypt_dir'] = TRUE;
    if (!is_dir($newDir)) {
      form_set_error('uc_credit_encryption_path', t('ERR209')); //You have specified a non-existent directory.
      unset($_SESSION['update_cc_encrypt_dir']);
    }
    else {
      $file = @fopen($newDir . '/encrypt.test', 'w');
      if ($file === FALSE) {
        form_set_error('uc_credit_encryption_path', t('ERR210')); //Cannot write to directory, please verify the directory permissions.
        unset($_SESSION['update_cc_encrypt_dir']);
      }
      else {
        if (@fwrite($file, '0123456789') === FALSE) {
          form_set_error('uc_credit_encryption_path', t('ERR210')); //Cannot write to directory, please verify the directory permissions.
          unset($_SESSION['update_cc_encrypt_dir']);
          fclose($file);
        }
        else {
          fclose($file);
          $file = @fopen($newDir . '/encrypt.test', 'r');
          if ($file === FALSE) {
            form_set_error('uc_credit_encryption_path', t('ERR211')); //Cannot read from directory, please verify the directory permissions.
            unset($_SESSION['update_cc_encrypt_dir']);
          }
          fclose($file);
        }
        unlink($newDir . '/encrypt.test');
      }
    }
  }
  }catch (Exception $ex) {
  	watchdog_exception('validateEncryptionKeyPathForBasicSettingsForm', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Submit handler for the basic settings form
 */
function exp_sp_administration_commerce_cc_basic_settings_form_submit($form, &$form_state) {
  try {
    
    // Credit card types
    $ccTypes = array('visa', 'mastercard', 'discover', 'amex');
    
    // Initialize data structures for audit trail
    $auditFieldsList = array('uc_payment_credit_gateway', 'uc_credit_encryption_path', 'uc_credit_cvv_enabled');
    $prevValues = array(new stdClass());
    $newValues = array();
    
    // From exp-card-types multiselect field value, generate the new uc_credit_<type> variable values
    foreach ($ccTypes as $type) {
      $ucCreditVar = 'uc_credit_' . $type;
      $auditFieldsList[] = $ucCreditVar; // for audit trail
      $form_state['values'][$ucCreditVar] = FALSE; //initialize
      
      if (!empty($form_state['values']['exp-card-types'][$type]) && $form_state['values']['exp-card-types'][$type] == $type) {
        $form_state['values'][$ucCreditVar] = TRUE;
      }
    } //end foreach
    unset($form_state['values']['exp-card-types']);
    
    // Below code taken from uc_credit.admin.inc:uc_credit_settings_form_submit() and slightly modified
    if (isset($_SESSION['update_cc_encrypt_dir']) && $_SESSION['update_cc_encrypt_dir'] === TRUE) {
      $dir = rtrim($form_state['values']['uc_credit_encryption_path'], '/\\');
      if (!file_exists($dir . '/uc_credit.key')) {
        if (!$file = fopen($dir . '/uc_credit.key', 'wb')) {
          expertus_set_message(t('ERR212'), 'error'); //Credit card encryption key file creation failed. Check your filepath settings and directory permissions.
          watchdog('exp_sp_administration_commerce_setting', 'Credit card encryption key file creation failed. Check your filepath settings and directory permissions.', array(), WATCHDOG_ERROR);
          $form_state['values'] = array(); // no further use
          $form_state['input'] = array(); // on form rebuild show form default values to user, for showing data actually saved in db (e.g. trimmed values)
          $form_state['rebuild'] = true; // rebuild form
          return;
        }
        else {
          // Replacement key generation suggested by Barry Jaspan for increased security.
          fwrite($file, md5(drupal_get_token(serialize($_REQUEST) . serialize($_SERVER) . REQUEST_TIME)));
          fclose($file);
          watchdog('exp_sp_administration_commerce_setting', 'Credit card encryption key file generated. Card data will now be encrypted.');
        }
      }
      unset($_SESSION['update_cc_encrypt_dir']); //No more required
    }
    
    // Below code taken from system.module:system_settings_form_submit() and modified
    form_state_values_clean($form_state);
    foreach ($form_state['values'] as $key => $value) {
      expDebug::dPrint('Saving ' . $key  . ' => ' . print_r($value, true), 4);
      $prevValues[0]->$key = variable_get($key); // for audit trail
      // Added the below conditional statement to enable the gateway for use
      if($key == 'uc_payment_credit_gateway' && $value == 'exp_cybersource'){
      	variable_set('uc_pg_exp_cybersource_enabled', 1);
      	variable_set('uc_pg_exp_paymetric_enabled', 0);
      }
      if($key == 'uc_payment_credit_gateway' && $value == 'exp_paymetric'){
      	variable_set('uc_pg_exp_paymetric_enabled', 1);
      	variable_set('uc_pg_exp_cybersource_enabled', 0);
      	variable_set('uc_credit_cvv_enabled', 1);
      }
      variable_set($key, $value);
      $newValues[$key] = $value; // for audit trail
    }

    // Make audit trail entries
    make_audit_trail_entries($newValues, getIdOfLoggedInUser(), $prevValues, 0, 'Updated', 'Commerce Credit Card Basic Settings Admin',
                                  'exp_sp_administration_commerce_setting','exp_sp_administration_commerce_cc_basic_settings_form_submit',
                                       $auditFieldsList);

    expertus_set_message(t('LBL1075').' '.t('MSG600').'.'); //Basic settings saved successfully.
    
    $form_state['values'] = array(); // no further use
    $form_state['input'] = array(); // on form rebuild show form default values to user, for showing data actually saved in db (e.g. trimmed values)
    $form_state['rebuild'] = true; // rebuild form
    if ($prevValues[0]->uc_payment_credit_gateway != $newValues['uc_payment_credit_gateway']) {
      $form_state['rebuild_payment_screen'] = true;
    }
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_commerce_cc_basic_settings_form_submit', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Build the cybersource settings form
 */
function exp_sp_administration_commerce_cc_cybersource_settings_form() {
  try {
    // Form wrapper id
    $formWrapperId = 'commerce-cc-settings-cybersource-form';

    // New form
    $form = newAddEditFormWithCancelAndSave($formWrapperId);
    
    // Get decrypted Cybersource soap service login credentials
    $login = _uc_cybersource_soap_login_data();
    
    addTwoColumnRowToAddEditForm($form, 'cybersource-twocolrow-1');
    
    // Add Payment Server drop-down
    $serverOptions = array(
        'production' => t('LBL1078'), //Production
        'test' => t('LBL1079'), //Test
    );
    $defaultServer = variable_get('uc_cybersource_server', 'test');
    addDropDownToAddEditForm($form, 'uc_cybersource_server', t('LBL1080'), $serverOptions, $defaultServer, 1, true); //Payment Server

    // Add Merchant ID text field
    addTextFieldToAddEditForm($form, 'uc_cybersource_soap_merchant_id', t('LBL1081'), $login['merchant_id'], 2, true); //Merchant ID
    
    endAddEditTwoColumnRow($form);
    
    addTwoColumnRowToAddEditForm($form, 'cybersource-twocolrow-2');
    
    // Add Transaction Key textarea field
    addTextFieldToAddEditForm($form, 'uc_cybersource_soap_transaction_key', t('LBL1082'), $login['transaction_key'], 3, true); //Transaction Key
    
    // Add Calculate Tax checkbox
    $calcTaxDefaultValue = variable_get('exp_cybersource_soap_tax_calculate', FALSE);
    addCheckBoxToAddEditForm($form, 'exp_cybersource_soap_tax_calculate', t('LBL1083'), true, $calcTaxDefaultValue, 4); //Calculate Tax

    endAddEditTwoColumnRow($form);
    
    // Add action buttons and close the form
    expAdminCreditCardSettingsFormClose('cybersource', $form, $formWrapperId, 5);
        
    expDebug::dPrint('$form = ' . print_r($form, true), 5);
    return $form;
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_commerce_cc_cybersource_settings_form', $ex);
    expertusErrorThrow($ex);
  }
	
}

/**
 * Validate handler for cybersource settings form
 */
function exp_sp_administration_commerce_cc_cybersource_settings_form_validate($form, &$form_state) {
  try {
    // Trim text fields
    $form_state['values']['uc_cybersource_soap_merchant_id'] = trim($form_state['values']['uc_cybersource_soap_merchant_id']);
    $form_state['values']['uc_cybersource_soap_transaction_key'] = trim($form_state['values']['uc_cybersource_soap_transaction_key']);
    
    // Check all mandatory fields
    $mandatoryFields = array('uc_cybersource_server', 'uc_cybersource_soap_merchant_id', 'uc_cybersource_soap_transaction_key');
    addEditCheckMandatoryFields($form, $form_state, $mandatoryFields);
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_commerce_cc_cybersource_settings_form_validate', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Submit handler for cybersource settings form
 */
function exp_sp_administration_commerce_cc_cybersource_settings_form_submit($form, &$form_state) {
  try {
    // Initialize data structures for audit trail
    $auditFieldsList = array('uc_cybersource_server', 'uc_cybersource_soap_merchant_id', 'uc_cybersource_soap_transaction_key',
                                 'exp_cybersource_soap_tax_calculate', 'uc_cybersource_method');
    $prevValues = array(new stdClass());
    $newValues = array();
    
    // Always set uc_cybersource_method to soap
    $form_state['values']['uc_cybersource_method'] = 'soap';
    
    //Below code taken from uc_cybersource.module:uc_cybersource_payment_gateway_settings_submit()
    // The original function was not added as a form submit handler to have CC encryption in audit_trail as well.
    // If CC encryption has been configured properly.
    if ($key = uc_credit_encryption_key()) {
      // Setup our encryption object.
      $crypt = new uc_encryption_class;

      // Encrypt the Merchant ID and Transaction key.
      if (!empty($form_state['values']['uc_cybersource_soap_merchant_id'])) {
        $form_state['values']['uc_cybersource_soap_merchant_id'] =
                                            $crypt->encrypt($key, $form_state['values']['uc_cybersource_soap_merchant_id']);
      }
      if (!empty($form_state['values']['uc_cybersource_soap_transaction_key'])) {
        $form_state['values']['uc_cybersource_soap_transaction_key'] =
                                           $crypt->encrypt($key, $form_state['values']['uc_cybersource_soap_transaction_key']);
      }

      // Store any errors.
      uc_store_encryption_errors($crypt, 'exp_sp_administration_commerce_setting');
    }
    
  	
    // Below code taken from system.module:system_settings_form_submit()
    // Exclude unnecessary elements.
    form_state_values_clean($form_state);    
    foreach ($form_state['values'] as $key => $value) {
    	expDebug::dPrint('Saving ' . $key  . ' => ' . print_r($value, true), 4);
    	$prevValues[0]->$key = variable_get($key); // for audit trail
      variable_set($key, $value);
      $newValues[$key] = $value; // for audit trail
    }

    // Make audit trail entries
    make_audit_trail_entries($newValues, getIdOfLoggedInUser(), $prevValues, 0, 'Updated', 'Commerce Credit Card Cybersource Settings Admin',
                                  'exp_sp_administration_commerce_setting','exp_sp_administration_commerce_cc_cybersource_settings_form_submit', $auditFieldsList);

    expertus_set_message(t('LBL563').' '.t('MSG600').'.'); //Cybersource settings saved successfully.
    $form_state['values'] = array(); // no further use
    $form_state['input'] = array(); // on form rebuild show form default values to user, for showing data actually saved in db (e.g. trimmed values)
    $form_state['rebuild'] = true; // rebuild form  
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_commerce_cc_cybersource_settings_form_submit', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Build the paymetric settings form
 */
function exp_sp_administration_commerce_cc_paymetric_settings_form() {
  try {
    // Form wrapper id
    $formWrapperId = 'commerce-cc-settings-paymetric-form';
    
    // New form
    $form = newAddEditFormWithCancelAndSave($formWrapperId);
    
    addTwoColumnRowToAddEditForm($form, 'paymetric-twocolrow-1');
     
    // Add Payment Server drop-down
    $serverOptions = array(
        'production' => t('LBL1078'), //Production
        'test' => t('LBL1079'), //Test
    );
    $defaultValue = variable_get('paymetric_server', 'test');
    addDropDownToAddEditForm($form, 'paymetric_server', t('LBL1080'), $serverOptions, $defaultValue, 1, true); //Payment Server

    // Add URL text field (Paymetric SOAP settings)
    $defaultValue = variable_get('paymetric_url', '');
    addTextFieldToAddEditForm($form, 'paymetric_url', t('LBL611'), $defaultValue, 6, true); //URL
    
    endAddEditTwoColumnRow($form);
    
    addTwoColumnRowToAddEditForm($form, 'paymetric-twocolrow-2');

    // Add DI Component URL text field (Data Intercept component settings)
    $defaultValue = variable_get('paymetric_di_url', '');
    addTextFieldToAddEditForm($form, 'paymetric_di_url', t('LBL1084'), $defaultValue, 2, true); //DI Component URL

    // Add WSDL text field (Paymetric SOAP settings)
    $defaultValue = variable_get('paymetric_wsdl', '');
    addTextFieldToAddEditForm($form, 'paymetric_wsdl', t('LBL1085'), $defaultValue, 7, true); //WSDL

    endAddEditTwoColumnRow($form);
    
    addTwoColumnRowToAddEditForm($form, 'paymetric-twocolrow-3');
    
    // Add Security Option drop-down (Data Intercept component settings)
    $secureMethodOptions = array(
      'tokenization' => t('LBL1086'), //Tokenization
      'Secure3D' => t('LBL1087'), //3D Secure
      'ach' => t('LBL1088'), //ACH
    );
    $defaultValue = variable_get('paymetric_secure_method', 'tokenization');
    addDropDownToAddEditForm($form, 'paymetric_secure_method', t('LBL1089'), $secureMethodOptions, $defaultValue, 3, true); //Security Option
    
    // Add Merchant ID text field (Paymetric SOAP settings)
    $defaultValue = variable_get('paymetric_merchant_id', '');
    addTextFieldToAddEditForm($form, 'paymetric_merchant_id', t('LBL1081'), $defaultValue, 8, true); //Merchant ID

    endAddEditTwoColumnRow($form);
    
    addTwoColumnRowToAddEditForm($form, 'paymetric-twocolrow-4');
    
    // Add Merchant GUID text field (Data Intercept component settings)
    $defaultValue = variable_get('paymetric_di_guid', '');
    addTextFieldToAddEditForm($form, 'paymetric_di_guid', t('LBL1091'), $defaultValue, 4, true); //Merchant GUID

    // Add Username text field (Paymetric SOAP settings)
    $defaultValue = variable_get('paymetric_user', '');
    addTextFieldToAddEditForm($form, 'paymetric_user', t('LBL054'), $defaultValue, 9, true); //Username

    endAddEditTwoColumnRow($form);

    addTwoColumnRowToAddEditForm($form, 'paymetric-twocolrow-5');

    // Add Shared Key text field (Data Intercept component settings)
    $defaultValue = variable_get('paymetric_di_sharedkey', '');
    addTextFieldToAddEditForm($form, 'paymetric_di_sharedkey', t('LBL1092'), $defaultValue, 5, true); //Shared Key

    // Add Password text field (Paymetric SOAP settings)
    $defaultValue = variable_get('paymetric_password', '');
    addTextFieldToAddEditForm($form, 'paymetric_password', t('LBL060'), $defaultValue, 10, true); //Password

    endAddEditTwoColumnRow($form);

    // Add action buttons and close the form
    expAdminCreditCardSettingsFormClose('paymetric', $form, $formWrapperId, 11);
    
    expDebug::dPrint('$form = ' . print_r($form, true), 5);
    return $form;
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_commerce_cc_paymetric_settings_form', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Validate handler for paymetric settings form
 */
function exp_sp_administration_commerce_cc_paymetric_settings_form_validate($form, &$form_state) {
  try {
    // Trim text fields
    $textFields = array('paymetric_di_url', 'paymetric_di_guid', 'paymetric_di_sharedkey',
                                  'paymetric_url', 'paymetric_wsdl', 'paymetric_merchant_id', 'paymetric_user', 'paymetric_password');
    foreach ($textFields as $field) {
      $form_state['values'][$field] = trim($form_state['values'][$field]);
    }
  	    
    // Check all mandatory fields   
    $mandatoryFields = array('paymetric_server', 'paymetric_di_url', 'paymetric_secure_method', 'paymetric_di_guid', 'paymetric_di_sharedkey',
                                 'paymetric_url', 'paymetric_wsdl', 'paymetric_merchant_id', 'paymetric_user', 'paymetric_password');
    addEditCheckMandatoryFields($form, $form_state, $mandatoryFields);
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_commerce_cc_paymetric_settings_form_validate', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Submit handler for paymetric settings form
 */
function exp_sp_administration_commerce_cc_paymetric_settings_form_submit($form, &$form_state) {
  try {
    // Initialize data structures for audit trail
    $auditFieldsList = array('paymetric_server', 'paymetric_di_url', 'paymetric_secure_method', 'paymetric_di_guid', 'paymetric_di_sharedkey',
                                 'paymetric_url', 'paymetric_wsdl', 'paymetric_merchant_id', 'paymetric_user', 'paymetric_password');
    $prevValues = array(new stdClass());
    $newValues = array();
    
    // Below code taken from system.module:system_settings_form_submit()
    // Exclude unnecessary elements.
    form_state_values_clean($form_state);    
    foreach ($form_state['values'] as $key => $value) {
      expDebug::dPrint('Saving ' . $key  . ' => ' . print_r($value, true), 4);
      $prevValues[0]->$key = variable_get($key); // for audit trail
      variable_set($key, $value);
      $newValues[$key] = $value; // for audit trail
    }

    // Make audit trail entries
    make_audit_trail_entries($newValues, getIdOfLoggedInUser(), $prevValues, 0, 'Updated', 'Commerce Credit Card Paymetric Settings Admin',
                               'exp_sp_administration_commerce_setting', 'exp_sp_administration_commerce_cc_paymetric_settings_form_submit',
                                 $auditFieldsList);

    expertus_set_message(t('LBL563').' '.t('MSG600').'.'); //Paymetric settings saved successfully.
    $form_state['values'] = array(); // no further use
    $form_state['input'] = array(); // on form rebuild show form default values to user, for showing data actually saved in db (e.g. trimmed values)
    $form_state['rebuild'] = true; // rebuild form  
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_commerce_cc_paymetric_settings_form_submit', $ex);
    expertusErrorThrow($ex);
  }
}


/**
 * Build the paypal settings form
 */
function exp_sp_administration_commerce_cc_paypal_settings_form() {
  try {
    // Form wrapper id
    $formWrapperId = 'commerce-cc-settings-paypal-form';
    
    // New form
    $form = newAddEditFormWithCancelAndSave($formWrapperId);
    
    addTwoColumnRowToAddEditForm($form, 'paypal-twocolrow-1');
     
    // Add Payment Server drop-down
    $serverOptions = array(
        'production' => t('LBL1078'), //Production
        'test' => t('LBL1079'), //Test
    );
    $defaultValue = variable_get('paypal_server', 'test');
    addDropDownToAddEditForm($form, 'paypal_server', t('LBL1080'), $serverOptions, $defaultValue, 1, true); //Payment Server

    // Add URL text field (Paymetric SOAP settings)
    /*$defaultValue = variable_get('paypal_url', '');
    addTextFieldToAddEditForm($form, 'paypal_url', t('LBL611'), $defaultValue, 6, true); //URL
    */
    
    endAddEditTwoColumnRow($form);
    
    addTwoColumnRowToAddEditForm($form, 'paypal-twocolrow-2');

    // Add Paypal partner
    $defaultValue = variable_get('paypal_partner', '');
    addTextFieldToAddEditForm($form, 'paypal_partner', t('Partner'), $defaultValue, 2, true); //DI Component URL

    // Add Paypal merchant id
    $defaultValue = variable_get('paypal_merchant_id', '');
    addTextFieldToAddEditForm($form, 'paypal_merchant_id', t('LBL1081'), $defaultValue, 7, true); //WSDL

    endAddEditTwoColumnRow($form);
    
    addTwoColumnRowToAddEditForm($form, 'paymetric-twocolrow-3');
    
    // Add Paypal Username
    $defaultValue = variable_get('paypal_user', '');
    addTextFieldToAddEditForm($form, 'paypal_user', t('LBL054'), $defaultValue, 4, true); //Username

    // Add Password text field
    $defaultValue = variable_get('paypal_password', '');
    addTextFieldToAddEditForm($form, 'paypal_password', t('LBL060'), $defaultValue, 9, true); //Password

    endAddEditTwoColumnRow($form);


    // Add action buttons and close the form
    expAdminCreditCardSettingsFormClose('paypal', $form, $formWrapperId, 11);
    
    expDebug::dPrint('$form = ' . print_r($form, true), 5);
    return $form;
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_commerce_cc_paymetric_settings_form', $ex);
    expertusErrorThrow($ex);
  }
}


/**
 * Validate handler for paymetric settings form
 */
function exp_sp_administration_commerce_cc_paypal_settings_form_validate($form, &$form_state) {
  try {
    // Trim text fields
    $textFields = array( 'paypal_user', 'paypal_password',
                                  'paypal_partner', 'paypal_merchant_id');
    foreach ($textFields as $field) {
      $form_state['values'][$field] = trim($form_state['values'][$field]);
    }
  	    
    // Check all mandatory fields   
    $mandatoryFields = array('paypal_server',  'paypal_user', 'paypal_password', 'paypal_partner',
                                 'paypal_merchant_id');
    addEditCheckMandatoryFields($form, $form_state, $mandatoryFields);
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_commerce_cc_paypal_settings_form_validate', $ex);
    expertusErrorThrow($ex);
  }
}



/**
 * Submit handler for paymetric settings form
 */
function exp_sp_administration_commerce_cc_paypal_settings_form_submit($form, &$form_state) {
  try {
    // Initialize data structures for audit trail
    $auditFieldsList =  array('paypal_server',  'paypal_user', 'paypal_password', 'paypal_partner',
                                 'paypal_merchant_id');
    $prevValues = array(new stdClass());
    $newValues = array();
    
    // Below code taken from system.module:system_settings_form_submit()
    // Exclude unnecessary elements.
    form_state_values_clean($form_state);    
    foreach ($form_state['values'] as $key => $value) {
      expDebug::dPrint('Saving ' . $key  . ' => ' . print_r($value, true), 4);
      $prevValues[0]->$key = variable_get($key); // for audit trail
      variable_set($key, $value);
      $newValues[$key] = $value; // for audit trail
    }

    // Make audit trail entries
    make_audit_trail_entries($newValues, getIdOfLoggedInUser(), $prevValues, 0, 'Updated', 'Commerce Credit Card Paypal Settings Admin',
                               'exp_sp_administration_commerce_setting', 'exp_sp_administration_commerce_cc_paypal_settings_form_submit',
                                 $auditFieldsList);

    expertus_set_message(t('LBL563').' '.t('MSG600').'.'); //Paypal settings saved successfully.
    $form_state['values'] = array(); // no further use
    $form_state['input'] = array(); // on form rebuild show form default values to user, for showing data actually saved in db (e.g. trimmed values)
    $form_state['rebuild'] = true; // rebuild form  
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_commerce_cc_paypal_settings_form_submit', $ex);
    expertusErrorThrow($ex);
  }
}


?>
