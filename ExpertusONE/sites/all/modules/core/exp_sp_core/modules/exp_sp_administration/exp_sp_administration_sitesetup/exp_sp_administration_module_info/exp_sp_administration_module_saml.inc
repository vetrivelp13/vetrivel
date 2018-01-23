<?php 
function SamlAddEdit($js = NULL){
	try {
    global $theme_key, $user;
	$js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
	drupal_add_js(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_addedit_behaviours.js', $js_module_optional);
	
	if ($user->language == 'ru') {
	  	$defaultWidth = 750;
	} else if ($user->language == 'de'){
	  	$defaultWidth = 650;
	} else{
		  $defaultWidth = 620;
	}

	$saml_style = array(
	    'ctools-admin-saml-style' => array(
	      'modalSize' => array(
	         'width' => ($theme_key == "expertusoneV2") ? $defaultWidth : 615,
	         'minHeight' => 20,
	      ),
	    )
	  );

	drupal_add_js($saml_style, 'setting');

	if ($js) {
		// Prepare the arguments list to be passed to $formState['build_info']['args'] of each form
		$argsList = array();
		 
		// Prepare list of forms to be displayed in the modal
		$formIdList = array(
			array('id' => 'exp_sp_administration_module_saml_form',
				  'title' => t('LBL1185')),
		);

		exp_ctools_modal_multi_ajaxonly_forms_display($formIdList, $argsList);
	}
	else {
		throw new Exception('SamlAddEdit');
	}
	}catch (Exception $ex) {
		watchdog_exception('SamlAddEdit', $ex);
		expertusErrorThrow($ex);
	}
}
function exp_sp_administration_module_saml_form($form, &$form_state) {
  try {
  	
  	expDebug::dPrint(' $form = ' . print_r($form, true) , 5);
  	
	$formWrapperId = 'saml-basic-addedit-form';

    $form['saml-basic-edit-form-start'] = array(
      '#type'   => 'markup',
      '#markup' => '<div id="saml-basic-addedit-form" class="saml-edit-rows">',
    );
	//$form = newAddEditFormWithCancelAndSave('samlserver-basic-addedit-form');
	//expDebug::dPrint('values to be printed'.variable_get('saml_idp_sso_login_url',''));
	addTextFieldToAddEditForm($form, 'saml_idp_sso_login_url', t('LBL1187'),  variable_get('saml_idp_sso_login_url', ''),1, true);
	
	addTextFieldToAddEditForm($form, 'saml_idp_issuer_id', t('LBL1166'),variable_get('saml_idp_issuer_id', ''), 2, true);
	
	addTextFieldToAddEditForm($form, 'saml_sp_entity_id', t('LBL1167'), variable_get('saml_sp_entity_id', 'expertusonesso'),3, true);
	
	addTextFieldToAddEditForm($form, 'saml_e1_acs_url', t('LBL1168'),getAcsURL(), 4, false,true);
	
	// Below two fields are currently not supported by our product
	// This might be needed in future.
	//addTextFieldToAddEditForm($form, 'saml_private_certificate_path', t('LBL1169'),variable_get('saml_private_certificate_path', ''), 5, false);
	//addTextFieldToAddEditForm($form, 'saml_private_certificate_password', t('LBL1170'),variable_get('saml_private_certificate_password', ''), 6, false);
	$selectIgnore='saml';
	$saml_nameId=variable_get('saml_nameid_format');
	$saml_nameId= empty($saml_nameId)? 'persistent' : $saml_nameId;
	// $samlFormatOptions = array('attribute' => t('LBL1179'),'email'=> t('LBL1180'),'persistent'=>t('LBL1181'));
	$samlFormatOptions = array('email'=> t('LBL1180'),'persistent'=>t('LBL1181'));
	addDropDownToAddEditForm($form, 'saml_nameid_format', t('LBL1171'),$samlFormatOptions,$saml_nameId, 7, false,false,array(),$selectIgnore);
	
	// Below field are currently not supported by our product
	// This might be needed in future.
	
	/*$saml_attrib_name=variable_get('saml_nameid_attribute_name');
	$saml_attrib_name = empty($saml_attrib_name)?'':$saml_attrib_name;
	addTextFieldToAddEditForm($form, 'saml_nameid_attribute_name', t('LBL1172'),$saml_attrib_name, 8, false);*/
	
	$authenticationModeOptions = array('yes' => t('Yes'),'no' => t('No'),);
	addDropDownToAddEditForm($form, 'saml_multimode_auth', t('LBL1173'), $authenticationModeOptions, variable_get('saml_multimode_auth','yes'), 9, false, false, array(), $selectIgnore);
	
	//'Auto Create/Update' option to create/update user profile during SSO.
	$autoCreationModeOptions = array('yes' => t('Yes'),'no' => t('No'),);
	addDropDownToAddEditForm($form, 'saml_Auto_create', t('LBL3010'), $autoCreationModeOptions, variable_get('saml_Auto_create','no'), 10, false, false, array(), $selectIgnore);
	
	addTextFieldToAddEditForm($form, 'saml_debug_textbox', t('LBL061'), variable_get('saml_debug_textbox', ''), 4, false);
	
	$samlAlogrithmOptions = array('sha256' => t('sha256'),'sha1' => t('sha1'),);
	addDropDownToAddEditForm($form, 'saml_certificate_algo', t('SAML Alogrithm'), $samlAlogrithmOptions, variable_get('saml_certificate_algo','sha256'), 9, false, false, array(), $selectIgnore);
	addTextAreaFieldToAddEditForm($form, 'saml_service_provider_private_key', t('SAML Service Provider Private key'), variable_get('saml_service_provider_private_key', ''),3, false);
	addTextAreaFieldToAddEditForm($form, 'saml_idp_public_key', t('SAML IdP Public key'), variable_get('saml_idp_public_key', ''),3, false);
		
	addEditFormWithSamlCancelAndSaveClose($form, $formWrapperId, 10);
	expDebug::dPrint('$form formvalue= ' . print_r($form, true), 5);
	
	$form['saml-basic-edit-form-end'] = array(
			'#type'   => 'markup',
			'#markup' => '</div>',
	);
	return $form;
  }
    catch (Exception $ex) {
   	watchdog_exception('exp_sp_administration_module_saml_form', $ex);
   	expertusErrorThrow($ex);
   }
}
function addEditFormWithSamlCancelAndSaveClose(&$form, $wrapperId, $tabIndex) {
	try {
		// Button ids
		$saveButtonId = 'samlserver-save-button';
		$cancelButtonId = 'samlserver-cancel-button';
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
		watchdog_exception('addEditFormWithSamlCancelAndSaveClose', $ex);
		expertusErrorThrow($ex);
	}
}
function exp_sp_administration_module_saml_form_submit($form, &$form_state) {
	expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 5);
	expDebug::dPrint('$form_state[triggering_element]  = ' . print_r($form_state['triggering_element'], true), 5);
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
		if($eSignValidated){	
			foreach($form_state['values'] as $key=>$samlval){
			  variable_set($key, $samlval);
				
			  expDebug::dPrint('print the Key = ' . $key);
			  expDebug::dPrint('print the Value = ' . $samlval);
			 }
			 $newSAMLVal = array(
			 		'key' => $key,
			 		'samlval' => $samlval,
			 	);
      make_audit_trail_entries($newSAMLVal, getIdOfLoggedInUser(), $oldSAMLvalue, 0, $auditTrailAction, 'SAML Admin',
                                         'exp_sp_administration_module_saml','exp_sp_administration_module_saml_form_submit', $auditFieldsList,getIdOfLoggedInUser());
			expertus_set_message(t('MSG689'));
			// clear drupal performance cache
			drupal_flush_all_caches();
		}
	}
	catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_module_saml_form_submit', $ex);
		expertusErrorThrow($ex);
	}
}
function exp_sp_administration_module_saml_form_validate($form, &$form_state){
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

			addEditCheckMandatoryFields($form, $form_state, array('saml_idp_sso_login_url', 'saml_idp_issuer_id', 'saml_sp_entity_id','saml_e1_acs_url'));

			if (!empty($form_state['values']['saml_idp_sso_login_url'])) {
				if (strpos($form_state['values']['saml_idp_sso_login_url'], ' ') !== FALSE) {
					form_set_error('loginURL', t('ERR219'));
					$form_state['esign_show_popup'] = false;
				}
			}
			if (parse_url($form_state['values']['saml_idp_sso_login_url'], PHP_URL_SCHEME) != 'https') {
				form_error($form['saml_idp_sso_login_url'], t('ERR220'));
			}
		   
		   if (!empty($form_state['values']['saml_idp_issuer_id'])) {
				if (strpos($form_state['values']['saml_idp_issuer_id'], ' ') !== FALSE) {
					form_set_error('issuer_id', t('ERR221'));
					$form_state['esign_show_popup'] = false;
				}
			}

			if (!empty($form_state['values']['saml_sp_entity_id'])) {
				if (strpos($form_state['values']['saml_sp_entity_id'], ' ') !== FALSE) {
					form_set_error('entity_id', t('ERR222'));
					$form_state['esign_show_popup'] = false;
				}
			} 
			

			if (!empty($form_state['values']['saml_debug_textbox']) && !validateEmail($form_state['values']['saml_debug_textbox'])) {
					form_set_error('email_id', t('ERR002'));
					$form_state['esign_show_popup']= false;
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
		watchdog_exception('exp_sp_administration_module_saml_form_validate', $ex);
		expertusErrorThrow($ex);
	}

}
?>