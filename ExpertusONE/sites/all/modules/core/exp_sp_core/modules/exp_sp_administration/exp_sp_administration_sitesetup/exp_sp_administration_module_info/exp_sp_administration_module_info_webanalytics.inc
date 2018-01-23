<?php
/**
 * Web Analytics Module Admin inc file
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 13-Jun-2013
 */

define('GOOGLEANALYTICS_PAGES', "admin\nadmin/*\nbatch\nnode/add*\nnode/*/*\nuser/*/*");

/**
 * Show exp_sp_administration_webanalytics_form in ctools popup
 */
function exp_sp_administration_webanalytics_display_form($js = NULL){
  try { 
	  $js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
	  drupal_add_js(drupal_get_path('module', 'exp_sp_administration_module_info') .'/exp_sp_administration_module_info_webanalytics.js', $js_module_optional);
	  
	  // Default modalTheme is ExpertusCToolsModalTheme (see exp_sp_core_extend_ctools_ajax.js:Drupal.CTools.Modal.show())
	  $webanalytics_style = array(
	    'ctools-admin-webanalytics-style' => array(
	      'modalSize' => array(
	        'width' => 428,
	        'minHeight' => 280,
	       ),
	    )
	  );
	
	  drupal_add_js($webanalytics_style, 'setting');
	  
	  if ($js) {
	    $formIdList = array(array('id'    => 'exp_sp_administration_webanalytics_form',
	                              'title' => (t('LBL990')))); //Web Analytics
	    exp_ctools_modal_multi_ajaxonly_forms_display($formIdList, array());
	    
	  } else {
	    throw new Exception('exp_sp_administration_module_info_webanalytics.inc: exp_sp_administration_webanalytics_display_form() : nojs');
	  }
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_webanalytics_display_form', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Build the form for web analytics admin
 */
function exp_sp_administration_webanalytics_form($form, &$form_state, $renderMode) {
  try {
    $form = newAddEditFormWithCancelAndSave('webanalytics-admin-form');

    // Add text field for entering Web Property ID
    $form['googleanalytics_account_default_text'] = array('#markup' =>
      '<span id= "googleanalytics_account_default_text" class="display:none;" data-default-text="' . t('MSG619') . '"></span>'); //Enter Web Property ID
    addTextFieldToAddEditForm($form, 'googleanalytics_account', t('LBL991'),//Web Property ID
                                       variable_get('googleanalytics_account', 'UA-'), 1, true, false);
  
    // Add Roles to Track multi-select dropdown
    // Get all role options
    // The first two role options are 'anonymous user' and 'authenticated user' followed by a sorted list of other available roles.                                       
    $roleQry1 = db_select('slt_groups', 'sec_role'); // Modified for - #0046888
    $roleQry1->innerJoin('role', 'role','sec_role.name = role.name');
    $roleQry1->fields('role', array('rid', 'name'));
    $roleQry1->orderBy('name');
    expDebug::dPrintDBAPI('$roleQry1', $roleQry1);   
    $orderedRoleQry1 = db_select($roleQry1, 'tmp'); // Workaround for a bug in Drupal7 DBAPI union().
    $orderedRoleQry1->fields('tmp', array('rid', 'name'));
    expDebug::dPrintDBAPI('$orderedRoleQry1', $orderedRoleQry1);

    $roleQry2 = db_select('role', 'role');
    $roleQry2->fields('role', array('rid', 'name'));
    $roleQry2->condition('role.name', array('anonymous user', 'authenticated user'), 'IN');
    expDebug::dPrintDBAPI('$roleQry2', $roleQry2);
     
    $roleQry2->union($orderedRoleQry1);
    $roleOptions = $roleQry2->execute()->fetchAllKeyed();
    expDebug::dPrintDBAPI('Webanalytics form role', $roleQry2);
    expDebug::dPrint('role options = ' . print_r($roleOptions, true), 5);

    // Get variable googleanalytics_visibility_roles
    $visibilityRoles = variable_get('googleanalytics_visibility_roles', 0);
    expDebug::dPrint('$visibilityRoles = ' . print_r($visibilityRoles, true), 5);
    // Get variable googleanalytics_roles for selected roles
    $checkedRolesDefault = variable_get('googleanalytics_roles', array());
    expDebug::dPrint('$checkedRolesDefault = ' . print_r($checkedRolesDefault, true), 5);
    // Based on whether googleanalytics_visibility_roles is 0 (selected roles) or 1 (excluded roles), build default checked roles list
    if ($visibilityRoles == 1) {
      foreach ($checkedRolesDefault as $key => $value) {
        if ($value == 0) {
          $checkedRolesDefault[$key] = $key;
        }
        else {
          $checkedRolesDefault[$key] = 0;
        }
      }
      expDebug::dPrint('$checkedRolesDefault after adjustment = ' . print_r($checkedRolesDefault, true), 5);
    }
    // Add multi-select field (when saving this form googleanalytics_visibility_roles will always be set to 0)
    addMultiselectDropDownToAddEditForm($form, 'googleanalytics_roles', t('LBL1001'), $roleOptions, //Roles to Track
                                                $checkedRolesDefault, 2, true, false, array('size' => 1)); //size attribute added to minimize any flicker
  
    // Add 'domains to track' select dropdown and the textarea field to enter domain names

    global $cookie_domain;
    // Per RFC 2109, cookie domains must contain at least one dot other than the
    // first. For hosts such as 'localhost' or IP Addresses we don't set a cookie domain.
    if (count(explode('.', $cookie_domain)) > 2 && !is_numeric(str_replace('.', '', $cookie_domain))) {
      $domainWithSubdomains = '*' . $cookie_domain;
    }
    else {
      $domainWithSubdomains = t('ERR195'); //option unavailable
    }
    expDebug::dPrint('$domainWithSubdomains = ' . print_r($domainWithSubdomains, true), 5);
                                               
    $domainTrackingOptions = array(
        1 => t('LBL992'), //A single domain
        2 => t('LBL993'), //One domain with multiple subdomains
        3 => t('LBL994'), //Multiple top-level domains
      );
    $defaultDomainMode = variable_get('googleanalytics_domain_mode', 0) + 1;
    addDropDownToAddEditForm($form, 'googleanalytics_domain_mode', t('LBL995'), $domainTrackingOptions, //Domain Tracking
                                        $defaultDomainMode, 3, true, false, array('data-single-domain-name' => $_SERVER['HTTP_HOST'],
                                                                                  'data-domain-with-subdomains' => $domainWithSubdomains,
                                                                                  'data-default-text' => t('MSG620'))); //Enter the domain names one per line
    $subDomainNamesDefaultValue = variable_get('googleanalytics_cross_domains', '');
    if ($defaultDomainMode == 1) {
      $subDomainNamesDefaultValue = $_SERVER['HTTP_HOST'];
    }
    else if ($defaultDomainMode == 2) {
      $subDomainNamesDefaultValue = $domainWithSubdomains;
    }
    addTextAreaFieldToAddEditForm($form, 'googleanalytics_cross_domains', t('LBL996'), $subDomainNamesDefaultValue, 4, true); //Domain Names
    
    // Page specific visibility configurations.
    $visibility = variable_get('googleanalytics_visibility_pages', 0);
    $pages = variable_get('googleanalytics_pages', GOOGLEANALYTICS_PAGES);
    
    if ($visibility > 1) { // to take care of case $visibility == 2, in case
      expertusErrorThrow(new Exception('Unsupported visibility ' . $visibility));
    }
  
    $pagesToTrackOptions = array(
      1 => t('LBL997'), //Every page except the listed pages
      2 => t('LBL998'), //The listed pages only
    );
  
    $pagesToTrackDefaultValue = $visibility + 1;
    addDropDownToAddEditForm($form, 'googleanalytics_visibility_pages', t('LBL999'), $pagesToTrackOptions, //Pages To Track
                                        $pagesToTrackDefaultValue, 5, true, false, array('data-default-text' => t('MSG621'))); //Enter the pages one per line
  
    addTextAreaFieldToAddEditForm($form, 'googleanalytics_pages', t('LBL1000'), $pages, 6); //Pages
    
    webanalyticsAddEditFormWithCancelAndSaveClose($form, 'webanalytics-admin-form', 'webanalytics-save-button', 'webanalytics-cancel-button', 7);
  
    return $form;
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_webanalytics_form', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * Customized closure for newAddEditFormWithCancelAndSave(). Adds the save and cancel button.
 */
function webanalyticsAddEditFormWithCancelAndSaveClose(&$form, $wrapperId, $saveButtonId, $cancelButtonId, $tabIndex) {
  try {
    $esignHiddenButt = 'esign-'.$wrapperId;
    global $theme_key;
    $prefixBgLeft='';$suffixBgRight='';
    if($theme_key == 'expertusoneV2'){
    	$prefixBgLeft   = '<div class="white-btn-bg-left"></div>';
    	$suffixBgRight = '<div class="white-btn-bg-right"></div>';
    } 
    // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
    // each time the form is re-built.
    $cancelAttributes['class'] = array('addedit-edit-' . $cancelButtonId);
  
    $cancelAttributes['class'][] = 'admin-action-button-middle-bg white-btn-bg-middle';
    $cancelAttributes['tabindex'][] = $tabIndex + 1;
  
    // A common class for all addedit form cancel button, for styling to appear as a link.
    $cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';
    $cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss(); return false;';
    
    $form[$cancelButtonId] = array(
      '#type' => 'submit',
      '#prefix' => '<div class="addedit-form-cancel-and-save-actions-row">' .
                     '<div class="addedit-form-cancel-container-actions">'.$prefixBgLeft,
      '#suffix' => $suffixBgRight,
       // Suffix to close the two divs is added in #suffix property of Save button
      '#value' => t('LBL123'), //Close
      '#attributes' => $cancelAttributes,
    );

    // Add the Save button
    $ajaxCallbackSave = 'addEditRepaintForm';
    
    // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
    // each time the form is re-built.
    $saveAttributes['class'] = array('addedit-edit-' . $saveButtonId);
    
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
      $saveAttributes['onclick'][] = '$("#'.$esignHiddenButt.'").click(); return false;';
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
      '#markup' => '</div><div class="clearBoth"></div>',
    );
  }
  catch (Exception $ex) {
    watchdog_exception('webanalyticsAddEditFormWithCancelAndSaveClose', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Implements _form_validate().
 */
function exp_sp_administration_webanalytics_form_validate($form, &$form_state) {
  try {
    expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 5);
    expDebug::dPrint('$form_state[triggering_element] = ' . print_r($form_state['triggering_element'], true), 5);
    
    $form_state['hidden-save-submit'] = false;
    $form_state['esign_show_popup']= false;
    
    // Trim some text values.
    $form_state['values']['googleanalytics_account'] = trim($form_state['values']['googleanalytics_account']);
    if ($form_state['values']['googleanalytics_account'] == t('MSG619')) { //Enter Web Property ID
      $form_state['values']['googleanalytics_account'] = '';
    }
    $form_state['values']['googleanalytics_pages'] = trim($form_state['values']['googleanalytics_pages']);
    if ($form_state['values']['googleanalytics_pages'] == t('MSG621')) { //Enter the pages one per line
      $form_state['values']['googleanalytics_pages'] = '';
    }
    $form_state['values']['googleanalytics_cross_domains'] = trim($form_state['values']['googleanalytics_cross_domains']);
    if ($form_state['values']['googleanalytics_cross_domains'] == t('MSG620')) { //Enter the domain names one per line
      $form_state['values']['googleanalytics_cross_domains'] = '';
    }
    
    // Correct some values so that the correct value is saved
    $form_state['values']['googleanalytics_domain_mode'] = $form_state['values']['googleanalytics_domain_mode'] - 1;
    $form_state['values']['googleanalytics_visibility_pages'] = $form_state['values']['googleanalytics_visibility_pages'] - 1;      
    if ($form_state['values']['googleanalytics_domain_mode'] != 2) {
      $form_state['values']['googleanalytics_cross_domains'] = '';
    }
    
    // Trim each item of multiline values
    webAnalyticsAdminTrimMultilineValue($form_state, 'googleanalytics_cross_domains');
    webAnalyticsAdminTrimMultilineValue($form_state, 'googleanalytics_pages');

    // Determine which button triggers validation
    if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
      $validationTrigger  = 'save';
    }
    else{
      $validationTrigger  = 'hidden-save';
    }
  
    if ($form_state['triggering_element']['#name'] == $validationTrigger) {
      $form_state['esign_show_popup']= true;
      
      // Check the mandatory fields
      webAnalyticsAdminCheckMandatoryFields($form, $form_state);
      
      // Check Web Property Id field
      if (!preg_match('/^UA-\d{4,}-\d+$/', $form_state['values']['googleanalytics_account'])) {
        form_set_error('googleanalytics_account', t('ERR196')); //A valid Google Analytics Web Property ID is case sensitive and formatted like UA-xxxxxxx-yy.
        $form_state['esign_show_popup'] = false;
      }
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
    watchdog_exception('exp_sp_administration_webanalytics_form_validate', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Checks the mandatory fields
 */
function webAnalyticsAdminCheckMandatoryFields($form, &$form_state) {
  try {
	  addEditCheckMandatoryFields($form, $form_state, array('googleanalytics_account'));
	
	  $fieldName = 'googleanalytics_roles';
	  $fieldValue = $form_state['values'][$fieldName];
	  if (empty($fieldValue)) {
	    form_set_error($fieldName, $form[$fieldName]['#title'] . t('ERR101')); // is required.
	    $form_state['esign_show_popup']= false;
	  }
	  
	  $fieldName = 'googleanalytics_domain_mode';
	  $fieldValue = $form_state['values'][$fieldName];
	  if ($fieldValue < 0) {
	    form_set_error($fieldName, $form[$fieldName]['#title'] . t('ERR101')); // is required.
	    $form_state['esign_show_popup']= false;
	  }
	  
	  if ($form_state['values']['googleanalytics_domain_mode'] == 2 || $form_state['values']['googleanalytics_domain_mode'] < 0) {
	    addEditCheckMandatoryFields($form, $form_state, array('googleanalytics_cross_domains'));
	  }
	  
	  $fieldName = 'googleanalytics_visibility_pages';
	  $fieldValue = $form_state['values'][$fieldName];
	  if ($fieldValue < 0) {
	    form_set_error($fieldName, $form[$fieldName]['#title'] . t('ERR101')); // is required.
	    $form_state['esign_show_popup']= false;
	  }
  }
  catch (Exception $ex) {
    watchdog_exception('webAnalyticsAdminCheckMandatoryFields', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Implements _form_submit().
 */
function exp_sp_administration_webanalytics_form_submit($form, &$form_state) {
  try {
    expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 5);
    expDebug::dPrint('$form_state[triggering_element] = ' . print_r($form_state['triggering_element'], true), 5);
    expDebug::dPrint('$form_state[storage] = ' . print_r($form_state['storage'], true), 5);
  	
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
    	// Initialize data structures for audit trail
    	$auditFieldsList = array('googleanalytics_account', 'googleanalytics_visibility_roles', 'googleanalytics_roles',
                                 'googleanalytics_domain_mode', 'googleanalytics_cross_domains',
                                   'googleanalytics_visibility_pages', 'googleanalytics_pages');
      $prevValues = array(new stdClass());
      $newValues = array();

      // Code in system.module:system_settings_form_submit() used below to save the variables
      // Exclude unnecessary elements.
      form_state_values_clean($form_state);
      foreach ($form_state['values'] as $key => $value) {
        expDebug::dPrint('Processing ' . $key  . ' => ' . print_r($value, true), 4);
        if (in_array($key, array('googleanalytics_account', 'googleanalytics_roles', 'googleanalytics_domain_mode',
                                     'googleanalytics_cross_domains', 'googleanalytics_visibility_pages', 'googleanalytics_pages'))) {
          if (is_array($value) && isset($form_state['values']['array_filter'])) {
            $value = array_keys(array_filter($value));
          }
          expDebug::dPrint('Saving ' . $key  . ' => ' . print_r($value, true), 4);
          $prevValues[0]->$key = variable_get($key); // for audit trail
          if (is_array($prevValues[0]->$key)) {
            $prevValues[0]->$key = implode(',', $prevValues[0]->$key);
          }
          variable_set($key, $value);
          $newValues[$key] = $value; // for audit trail
          if (is_array($newValues[$key])) {
            $newValues[$key] = implode(',', $newValues[$key]);
          }
        }
      } // end foreach
      
      // Always set googleanalytics_visibility_roles to 0
      expDebug::dPrint('Saving googleanalytics_visibility_roles => 0', 4);
      $prevValues[0]->googleanalytics_visibility_roles = variable_get('googleanalytics_visibility_roles'); // for audit trail
      variable_set('googleanalytics_visibility_roles', 0);
      $newValues['googleanalytics_visibility_roles'] = 0; // for audit trail
      
      // Header section must be forced for multiple top-level domains. (see googleanalytics.admin.inc: googleanalytics_admin_settings_form_validate())
      // googleanalytics_js_scope default is header as well. Only ensure for multiple top-level domains here.
      if ($form_state['values']['googleanalytics_domain_mode'] == 2) {
	      expDebug::dPrint('Saving googleanalytics_js_scope => header as googleanalytics_domain_mode is 2', 4);
	      $prevValues[0]->googleanalytics_js_scope = variable_get('googleanalytics_js_scope'); // for audit trail
	      variable_set('googleanalytics_js_scope', 'header');
	      $newValues['googleanalytics_js_scope'] = 'header'; // for audit trail
	      
	      $auditFieldsList[] = 'googleanalytics_js_scope'; // for audit trail
      }
      
      // Make audit trail entries
      make_audit_trail_entries($newValues, getIdOfLoggedInUser(), $prevValues, 0, 'Updated', 'Web Analytics Admin',
                                  'exp_sp_administration_module_info_webanalytics','exp_sp_administration_webanalytics_form_submit', $auditFieldsList);
      
      expertus_set_message(t('LBL563').' '.t('MSG600').'.'); //Web analytics settings saved successfully.
      $form_state['values'] = array(); // no further use
      $form_state['input'] = array(); // on form rebuild show form default values to user, for showing data actually saved in db (e.g. trimmed values)
      $form_state['rebuild'] = true; // rebuild form
    }
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_webanalytics_form_submit', $ex);
    expertusErrorThrow($ex);
  }
}  

/**
 * Trims multiline form field value items. Removes any blank items
 */
function webAnalyticsAdminTrimMultilineValue(&$form_state, $field) {
	try {
    // Trim each item, remove blank items
    if (!empty($form_state['values'][$field])) {
      $itemsList = preg_split('/(\r\n?|\n)/', $form_state['values'][$field]);
      expDebug::dPrint('$itemsList = ' . print_r($itemsList, true), 5);
      $cleanedItemsList = array();
      foreach ($itemsList as $domain) {
        $cleanedDomain = trim($domain);
        if (!empty($cleanedDomain)) {
          $cleanedItemsList[] = $cleanedDomain;
        }
      } //end foreach
      expDebug::dPrint('$cleanedItemsList = ' . print_r($cleanedItemsList, true), 5);
      if (empty($cleanedItemsList)) {
        $form_state['values'][$field] = '';
      }
      else {
        $form_state['values'][$field] = implode("\n", $cleanedItemsList);
        $itemsList = preg_split('/(\r\n?|\n)/', $form_state['values'][$field]); // for testing only
        expDebug::dPrint('$itemsList from cleaned field value = ' . print_r($itemsList, true), 5); // for testing only
      }
    }
    expDebug::dPrint("form_state[values][$field] after cleanup = " . print_r($form_state['values'][$field], true), 5);
  }
  catch (Exception $ex) {
    watchdog_exception('webAnalyticsAdminTrimMultilineValue', $ex);
    expertusErrorThrow($ex);
  }
}

?>