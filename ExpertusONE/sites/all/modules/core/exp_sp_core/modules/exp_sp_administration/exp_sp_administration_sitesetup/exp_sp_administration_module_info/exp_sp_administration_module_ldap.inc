<?php 

function ldapAddEdit($js = NULL){
 try {
 	global $user;
 	
  $js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
  drupal_add_js(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_addedit_behaviours.js', $js_module_optional);
  
  if ($user->language == 'ru') {
  	$defaultWidth = 1000;
  } else {
  	$defaultWidth = 735;
  }
  
  $catalog_style = array(
    'ctools-admin-ldap-setting-addedit-scroll-wrapper' => array(
      'modalSize' => array(
        'width' => $defaultWidth,
        'minHeight' => 380,
       ),
      'modalTheme' => 'ExpertusCToolsAdminCourseModalTheme',
    )
  );

  drupal_add_js($catalog_style, 'setting');
  
  if ($js) {   
    // Prepare the arguments list to be passed to $formState['build_info']['args'] of each form
    $argsList = array();
           
   // Prepare list of forms to be displayed in the modal
    $formIdList = array(
                        array('id' => 'exp_sp_administration_module_ldap_form', 'title' => t('LBL1024'))
                        );
                        
    exp_ctools_modal_multi_ajaxonly_forms_display($formIdList, $argsList);
  }
  else {
  	throw new Exception('ldapAddEdit');
  }
  } catch (Exception $ex) {
  	watchdog_exception('ldapAddEdit', $ex);
  	expertusErrorThrow($ex);
  }
}// ldapAddEdit end.

function exp_sp_administration_module_ldap_form($form, &$form_state) {
	try {
  expDebug::dPrint(' $form = ' . print_r($form, true) , 5);
  expDebug::dPrint(' $form_state = ' . print_r($form_state, true) , 5);
  expDebug::dPrint(' $form_state[values] = ' . print_r($form_state['values'], true) , 5);
  expDebug::dPrint(' $form_state[build_info] = ' . print_r($form_state['build_info'], true) , 5);

  // Create a new add/edit form
  $form = newAddEditFormWithCancelAndSave('ldap-basic-addedit-form');
  
  $form['ldap-basic-addedit-form-username'] = array(
    '#prefix' => '<div class="addedit-new-field" id="username-mismatch"><div class="addedit-new-field-title">&nbsp;</div><div class="addedit-new-field-value messerror ">'.t('ERR016').'</div></div>',
  );
  
  $defaultValue =  getLDAPServerDefaultValue('sid');
  addTextFieldToAddEditForm($form, 'sid', t('LBL1006'), $defaultValue, 1, true);
  $defaultValue =  getLDAPServerDefaultValue('name');
  addTextFieldToAddEditForm($form, 'name', t('LBL107'), $defaultValue, 2, true);
  
  addTwoColumnRowToAddEditForm($form, 'ldaptype_address');
  $defaultValue =  getLDAPServerDefaultValue('ldap_type');
  addDropDownToAddEditForm($form, 'ldap_type', t('LBL1007'), ldap_servers_ldaps_option_array(), $defaultValue, 3, true);
  $defaultValue =  getLDAPServerDefaultValue('address');
  addTextFieldToAddEditForm($form, 'address', t('LBL1008'), $defaultValue, 4, true);
  endAddEditTwoColumnRow($form);

  addTwoColumnRowToAddEditForm($form, 'port_userattr');
  $defaultValue =  getLDAPServerDefaultValue('port');
  addTextFieldToAddEditForm($form, 'port', t('LBL1009'), $defaultValue, 5, true);
  
  $defaultValue =  getLDAPServerDefaultValue('user_attr');
  addTextFieldToAddEditForm($form, 'user_attr', t('LBL054'), $defaultValue, 6, true);
  endAddEditTwoColumnRow($form);
  
  addTwoColumnRowToAddEditForm($form, 'basedn_mailattr');
  $defaultValue =  unserialize(getLDAPServerDefaultValue('basedn'));
  addTextFieldToAddEditForm($form, 'basedn', t('LBL1012'), $defaultValue, 7, true);
  $defaultValue =  getLDAPServerDefaultValue('mail_attr');
  addTextFieldToAddEditForm($form, 'mail_attr', t('LBL061'), $defaultValue, 8, true);
  endAddEditTwoColumnRow($form);

  addTwoColumnRowToAddEditForm($form, 'uniquepersistentattr_bindpw');
  $defaultValue =  getLDAPServerDefaultValue('unique_persistent_attr');
  addTextFieldToAddEditForm($form, 'unique_persistent_attr', t('LBL1011'), $defaultValue, 9, true);
  $defaultValue =  ldap_servers_decrypt(getLDAPServerDefaultValue('bindpw'));
  addHiddenValueToAddEditForm($form, 'defaultbindpw', $defaultValue);
  addPasswordFieldToAddEditForm($form, 'bindpw', t('LBL060'), '', 10);
  endAddEditTwoColumnRow($form);
  
  $defaultValue =  getLDAPServerDefaultValue('binddn');
  addTextFieldToAddEditForm($form, 'binddn', t('LBL1010'), $defaultValue, 11, true);
  
  $form['separator1'] = array(
    '#prefix' => '<div class="separator"></div>',
  );
  
  addTwoColumnRowToAddEditForm($form, 'authenticationMode_cookieExpire');
  $defaultValue =  getAuthDefaultValue('authenticationMode');
  $authenticationModeOptions = array(LDAP_AUTHENTICATION_MIXED => t('LBL1029'),LDAP_AUTHENTICATION_EXCLUSIVE => t('LBL1030'),);
  addDropDownToAddEditForm($form, 'authenticationMode', t('LBL1013'), $authenticationModeOptions, $defaultValue, 12, true);
  $defaultValue =  getAuthDefaultValue('cookieExpire');
  $cookieExpirePeriod = array(1 => t('LBL1031')) + drupal_map_assoc(array(3600, 86400, 604800, 2592000, 31536000, 315360000), 'format_interval') + array(-1 => t('LBL1032'));
  addDropDownToAddEditForm($form, 'cookieExpire', t('LBL1014'), $cookieExpirePeriod, $defaultValue, 13);
  endAddEditTwoColumnRow($form);

  addTwoColumnRowToAddEditForm($form, 'sso_ssotimeout');
  
  $defaultValue = array();
  $ssoEnabled = getAuthDefaultValue('ssoEnabled');
  $seamlessLogin = getAuthDefaultValue('seamlessLogin');
  if(isset($ssoEnabled) && $ssoEnabled == '1'){
    $defaultValue[] = 'ssoEnabled';
  }
  if(isset($seamlessLogin) && $seamlessLogin == '1'){
    $defaultValue[] = 'seamlessLogin';
  }
  
  $sso = array('ssoEnabled' => t('LBL1033'),'seamlessLogin' => t('LBL1034'));
  addMultiselectDropDownToAddEditForm($form, 'sso', t('LBL1015'), $sso, $defaultValue, 14);
  
  $defaultValue =  getAuthDefaultValue('sso_timeout');
  addTextFieldToAddEditForm($form, 'sso_timeout', t('LBL1016'), $defaultValue, 15);
  endAddEditTwoColumnRow($form);

  addTwoColumnRowToAddEditForm($form, 'ldapImplementation_sharedsecret');
  $defaultValue =  getAuthDefaultValue('ldapImplementation');
  addDropDownToAddEditForm($form, 'ldapImplementation', t('LBL1017'), array('mod_auth_sspi' => t('mod_auth_sspi')), $defaultValue, 16, true);
  $defaultValue =  getAuthDefaultValue('shared_secret');
  addTextFieldToAddEditForm($form, 'shared_secret', t('LBL1018'), $defaultValue, 17);
  endAddEditTwoColumnRow($form);

  $defaultValue =  getAuthDefaultValue('sso_login_url');
  addTextFieldToAddEditForm($form, 'sso_login_url', t('LBL1019'), $defaultValue, 18);
  
  $form['separator2'] = array(
    '#prefix' => '<div class="separator"></div>',
  );

  addTwoColumnRowToAddEditForm($form, 'FirstName_LastName');
  addTextFieldToAddEditForm($form, 'FirstName', t('LBL056'), getLDAPFieldMappingValue('FirstName'), 19, true);
  addTextFieldToAddEditForm($form, 'LastName', t('LBL058'), getLDAPFieldMappingValue('LastName'), 20, true);
  endAddEditTwoColumnRow($form);
  
  addTwoColumnRowToAddEditForm($form, 'Email1_JobTitle');
  addTextFieldToAddEditForm($form, 'Email1', t('LBL061'), getLDAPFieldMappingValue('Email'), 21, true);
  addTextFieldToAddEditForm($form, 'JobTitle', t('LBL073'), getLDAPFieldMappingValue('JobTitle'), 22);
  endAddEditTwoColumnRow($form);
  
  addTwoColumnRowToAddEditForm($form, 'TimeZone_PhoneNo');
  addTextFieldToAddEditForm($form, 'TimeZone', t('LBL297'), getLDAPFieldMappingValue('TimeZone'), 23);
  addTextFieldToAddEditForm($form, 'PhoneNo', t('LBL070'), getLDAPFieldMappingValue('PhoneNo'), 24);
  endAddEditTwoColumnRow($form);

  $ldapMachineName = getLDAPServerDefaultValue('sid');
  expDebug::dPrint(' $form $ldapMachineName = ' . print_r($ldapMachineName, true) , 25);
  ldapServerAddEditFormWithCancelAndSaveClose($form, 'ldap-basic-addedit-form', 'ldap-basic-save', 'ldap-basic-cancel-button', 26);
  
  return $form;
  } catch (Exception $ex) {
  	watchdog_exception('exp_sp_administration_module_ldap_form', $ex);
  	expertusErrorThrow($ex);
  }
}

function ldapServerAddEditFormWithCancelAndSaveClose(&$form, $wrapperId, $saveButtonId, $cancelButtonId, $tabIndex) {
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
    watchdog_exception('ldapServerAddEditFormWithCancelAndSaveClose', $ex);
    expertusErrorThrow($ex);
  }
}

function getLDAPServerDefaultValue($ldapField){
  try{
  $select = db_select('ldap_servers', 'ldap');
  $select->addField('ldap',$ldapField);
  $select->range(0,1);
  expDebug::dPrintDBAPI(' getLDAPServerDefaultValue Velu = ' , $select);
  $result = $select->execute()->fetchField();
  return $result;
  }catch (Exception $ex) {
    watchdog_exception('getLDAPServerDefaultValue', $ex);
    expertusErrorThrow($ex);
  }  
}

function getAuthDefaultValue($authVal){
  try{
  $select = db_select('variable', 'va');
  $select->addField('va','value');
  $select->condition('name', 'ldap_authentication_conf');
  expDebug::dPrintDBAPI(' ldap_authentication_conf Qry = ' , $select);
  $result = $select->execute()->fetchField();
  $authvalue = unserialize($result);
  expDebug::dPrint(' ldap_authentication_conf Value = ' . print_r(unserialize($result), true) , 5);
  return $authvalue[$authVal];
  }catch (Exception $ex) {
    watchdog_exception('getAuthDefaultValue', $ex);
    expertusErrorThrow($ex);
  }  
}

function getLDAPFieldMappingValue($lms_field_name){
  try{
  $select = db_select('slt_ldap_field_mapping', 'slfm');
  $select->addField('slfm','ldap_field_name');
  $select->condition('lms_field_name', $lms_field_name);
  expDebug::dPrintDBAPI(' getLDAPFieldMappingValue Velu = ' , $select);
  $result = $select->execute()->fetchField();
  return $result;
  }catch (Exception $ex) {
    watchdog_exception('getLDAPFieldMappingValue', $ex);
    expertusErrorThrow($ex);
  }  
}

function exp_sp_administration_module_ldap_form_validate($form, &$form_state){
  expDebug::dPrint(' Validate $form = ' . print_r($form, true) , 5);
  expDebug::dPrint(' Validate $form_state = ' . print_r($form_state['values'], true) , 5);
  try{
    $form_state['hidden-save-submit'] = false;
    $form_state['esign_show_popup'] = false;
    
    // Determine which button triggers validation
    if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
      $validationTrigger  = 'save';
    }
    else{
      $validationTrigger  = 'hidden-save';
    }
    
    if ($form_state['triggering_element']['#name'] == $validationTrigger) {
    $form_state['esign_show_popup']= true;
    
    addEditCheckMandatoryFields($form, $form_state, array('sid', 'name', 'ldap_type', 'port', 'address', 'user_attr', 'binddn', 
    'mail_attr', 'unique_persistent_attr', 'basedn', 'authenticationMode','ldapImplementation', 'FirstName', 'LastName', 'Email1')); 
    
    if (!empty($form_state['values']['sid'])) {
      if (strpos($form_state['values']['sid'], ' ') !== FALSE) {
          form_set_error('user_name', t('ERR015'));
          $form_state['esign_show_popup'] = false;
      } 
    }
    
    if(!is_numeric($form_state['values']['port']) && !empty($form_state['values']['port'])){
      	form_set_error('port', t('ERR097'));
      	$form_state['esign_show_popup'] = false;
    }
    
    if (!empty($form_state['values']['user_attr'])) {
      if (strpos($form_state['values']['user_attr'], ' ') !== FALSE) {
          form_set_error('user_name', t('ERR015'));
          $form_state['esign_show_popup'] = false;
      } 
    }
    
    if(!is_numeric($form_state['values']['cookieExpire']) && !empty($form_state['values']['cookieExpire'])){
      	form_set_error('cookieExpire', t('ERR097'));
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

  }catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_module_ldap_form_validate', $ex);
    expertusErrorThrow($ex);
  }
  
}

function exp_sp_administration_module_ldap_form_submit($form, &$form_state){

  expDebug::dPrint(' Submit $form = ' . print_r($form, true) , 5);
  expDebug::dPrint(' Submit $form_state = ' . print_r($form_state, true) , 5);
  
    foreach ($form_state['values'] as $key => $value) {
      expDebug::dPrint('Processing ' . $key  . ' => ' . print_r($value, true), 4);
    }
    try{
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
   
   if($eSignValidated){
   // Audit Trail Entry
   $preLDAPVal = db_query("SELECT sid,name,ldap_type,address,port,user_attr,bindpw,binddn,mail_attr,unique_persistent_attr FROM ldap_servers")->fetchAll();
   expDebug::dPrint(' Audit Trail Entry $preLDAPVal = ' . print_r($preLDAPVal, true) , 4);
     
    //LDAP Add Server
    if(empty($preLDAPVal)){
      $insertUpdateStmt = db_insert('ldap_servers');
      $auditTrailAction = 'Add LDAP Server'; //Audit Trail Entry
      expertus_set_message(t('LBL272').' '.t('MSG600').'.');
    }else{
      $insertUpdateStmt = db_update('ldap_servers');
      $insertUpdateStmt->condition('sid', $form_state['values']['sid']);
      $auditTrailAction = 'Update LDAP Server'; //Audit Trail Entry
      expertus_set_message(t('LBL272').' '.t('MSG601').'.');
    }
    $basedn = array('0' => $form_state['values']['basedn']);
    if(empty($form_state['values']['bindpw']) && empty($form_state['values']['defaultbindpw'])){
      $password = '';
    }else if(!empty($form_state['values']['bindpw']) && !empty($form_state['values']['defaultbindpw'])){
      $password = ldap_servers_encrypt($form_state['values']['bindpw']);
    }else if(empty($form_state['values']['bindpw']) && !empty($form_state['values']['defaultbindpw'])){
      $password = ldap_servers_encrypt($form_state['values']['defaultbindpw']);
    }else if(!empty($form_state['values']['bindpw']) && empty($form_state['values']['defaultbindpw'])){
      $password = ldap_servers_encrypt($form_state['values']['bindpw']);
    }
    $insertUpdateStmt->fields(array(
    	'sid' => $form_state['values']['sid'],
    	'name' => $form_state['values']['name'],
        'status' => 1,
    	'ldap_type' => $form_state['values']['ldap_type'],
    	'address' => $form_state['values']['address'],
    	'port' => $form_state['values']['port'],
        'bind_method' => 1,
    	'user_attr' => $form_state['values']['user_attr'],
    	'binddn' => $form_state['values']['binddn'],
    	'mail_attr' => $form_state['values']['mail_attr'],
    	'unique_persistent_attr' => $form_state['values']['unique_persistent_attr'],
    	'bindpw' => $password,
    	'basedn' => serialize($basedn)
    ));
    expDebug::dPrintDBAPI(' LDAP Add Server InsertUpdate Qry = ' , $insertUpdateStmt);
    $insertUpdateStmt->execute();
    
    //LDAP Profile Mapping fields
    $ldapFormFields = array(
		'FirstName' => $form_state['values']['FirstName'],
		'LastName' => $form_state['values']['LastName'],
		'Email' => $form_state['values']['Email1'],
		'JobTitle' => $form_state['values']['JobTitle'],
		'TimeZone' => $form_state['values']['TimeZone'],
		'PhoneNo' => $form_state['values']['PhoneNo']
    );
    expDebug::dPrint(' LDAP Profile Mapping form fields = ' . print_r($ldapFormFields, true) , 4);
    
    $results=db_query("select lms_field_name from slt_ldap_field_mapping");
    foreach($results as $row){
     	$lms_field_name=$row->lms_field_name;
     	$update = db_update('slt_ldap_field_mapping');
        $update->fields(array(
          'ldap_field_name' => $ldapFormFields[$lms_field_name],
          'updated_on' => now()
        ));
        $update->condition('lms_field_name', $lms_field_name);
        $update->execute();
     	expDebug::dPrintDBAPI(' LDAP Profile Mapping fields Update Qry = ' , $update);
     }
     
     // Audit Trail Entry
     $newLDAPVal = array(
    	'sid' => $form_state['values']['sid'],
    	'name' => $form_state['values']['name'],
    	'ldap_type' => $form_state['values']['ldap_type'],
    	'address' => $form_state['values']['address'],
    	'port' => $form_state['values']['port'],
    	'user_attr' => $form_state['values']['user_attr'],
    	'binddn' => $form_state['values']['binddn'],
    	'mail_attr' => $form_state['values']['mail_attr'],
    	'unique_persistent_attr' => $form_state['values']['unique_persistent_attr']
     );
     
     $auditFieldsList = array_keys($newLDAPVal);
     make_audit_trail_entries($newLDAPVal, getIdOfLoggedInUser(), $preLDAPVal, 0, $auditTrailAction, 'LDAP Admin',
                                         'exp_sp_administration_module_ldap','exp_sp_administration_module_ldap_form_submit', $auditFieldsList,getIdOfLoggedInUser());
     
    $newAuthVal = array(
    'sids' => array($form_state['values']['sid'] => $form_state['values']['sid']),
    'authenticationMode' => $form_state['values']['authenticationMode'],
    'ssoEnabled' => isset($form_state['values']['sso']['ssoEnabled']) ? '1' : '',
    'seamlessLogin' => isset($form_state['values']['sso']['seamlessLogin']) ? '1' : '',
    'ldapImplementation' => $form_state['values']['ldapImplementation'],
    'cookieExpire' => $form_state['values']['cookieExpire'],
    'shared_secret' => $form_state['values']['shared_secret'],
  	'sso_timeout' => $form_state['values']['sso_timeout'],
  	'sso_login_url' => $form_state['values']['sso_login_url']
    );
    expDebug::dPrint(' Submit New Auth Values = ' . print_r($newAuthVal, true) , 5);
    
    $select = db_select('variable', 'va');
    $select->addField('va','value');
    $select->condition('name', 'ldap_authentication_conf');
    expDebug::dPrintDBAPI(' ldap_authentication_conf Qry = ' , $select);
    $result = $select->execute()->fetchField();
    $defaultAuthVal = unserialize($result);
    expDebug::dPrint(' Default Auth Values = ' . print_r($defaultAuthVal, true) , 5);

    $updateAuthVal = array_merge($defaultAuthVal, $newAuthVal);
    expDebug::dPrint(' Updated Auth Values = ' . print_r($updateAuthVal, true) , 5);
    
    $updateAuth = db_update('variable');
    $updateAuth->fields(array(
       'value' => serialize($updateAuthVal),
    ));
    $updateAuth->condition('name', 'ldap_authentication_conf');
    $updateAuth->execute();
    expDebug::dPrintDBAPI(' Authentication fields Update Qry = ' , $updateAuth);
  
    $form_state['rebuild'] = true;
   }
      
    }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_module_ldap_form_submit', $ex);
    expertusErrorThrow($ex);
  }
    
}


?>