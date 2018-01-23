<?php 
function AdfsAddEdit($js = NULL){
	try{
	global $theme_key;
	$js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
	drupal_add_js(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_addedit_behaviours.js', $js_module_optional);

	expDebug::dPrint('AdfsAddEdit sabee');

	$adfs_style = array(
			'ctools-admin-adfs-style' => array(
					'modalSize' => array(
							'width' => ($theme_key == "expertusoneV2") ? 575 : 600,
							'minHeight' => 200,
					),
			)
	);

	drupal_add_js($adfs_style, 'setting');

	if ($js) {
		// Prepare the arguments list to be passed to $formState['build_info']['args'] of each form
		$argsList = array();
			
		// Prepare list of forms to be displayed in the modal
		$formIdList = array(
				array('id' => 'exp_sp_administration_module_adfs_form',
				  'title' =>t('LBL1186')),
		);

		exp_ctools_modal_multi_ajaxonly_forms_display($formIdList, $argsList);
	}
	else {
		throw new Exception('AdfsAddEdit');
	}
	}catch(Exception $ex) {
		watchdog_exception('AdfsAddEdit', $ex);
		expertusErrorThrow($ex);
	}
}
function exp_sp_administration_module_adfs_form($form, &$form_state) {
	try {
		$formWrapperId = 'adfsserver-basic-addedit-form';
		
		$form['adfs-basic-edit-form-start'] = array(
				'#type'   => 'markup',
				'#markup' => '<div id="adfs-basic-addedit-form" class="adfs-edit-rows">',
		);
		addTextFieldToAddEditForm($form, 'adfs_idp_sso_target_url', t('LBL1174'),  variable_get('adfs_idp_sso_target_url', ''),1, true);

		addTextFieldToAddEditForm($form, 'adfs_sp_identity', t('LBL1175'),variable_get('adfs_sp_identity', ''), 2, true);

		addTextFieldToAddEditForm($form, 'adfs_e1_prp_url', t('LBL1176'),getPrpURL(),3, false,true);
		
		$adfs_nameid = variable_get('adfs_nameid_format');
		$adfs_nameid = empty($adfs_nameid)? 'persistent' : $adfs_nameid;
		$federationOptions =  array(option1 => t('LBL1182'),option2=> t('LBL1183'),option3=>t('LBL1184'));
		addDropDownToAddEditForm($form, 'adfs_nameid_format', t('LBL1177'),$federationOptions,$adfs_nameid, 4, false);
		
		$adfs_attrib_name = variable_get('adfs_nameid_attribute_name');
		$adfs_attrib_name = empty($adfs_attrib_name)?'':$adfs_attrib_name;
		addTextFieldToAddEditForm($form, 'adfs_nameid_attribute_name', t('LBL1178'),$adfs_attrib_name, 5, false);
		
		addEditFormWithAdfsCancelAndSaveClose($form, $formWrapperId, 6);
		expDebug::dPrint('$form formvalue= ' . print_r($form, true), 5);
    
		$form['adfs-basic-edit-form-end'] = array(
				'#type'   => 'markup',
				'#markup' => '</div>',
		);
		
		return $form;
	}
	catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_module_adfs_form', $ex);
		expertusErrorThrow($ex);
	}
}
function addEditFormWithAdfsCancelAndSaveClose(&$form, $wrapperId, $tabIndex) {
	try {
		// Button ids
		$saveButtonId = 'adfsserver-save-button';
		$cancelButtonId = 'adfsserver-cancel-button';
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
					'#value' => t('LBL141'), // Savesaml
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
		watchdog_exception('addEditFormWithAdfsCancelAndSaveClose', $ex);
		expertusErrorThrow($ex);
	}
}
function exp_sp_administration_module_adfs_form_submit($form, &$form_state) {
	expDebug::dPrint('$form_state[values] sabee = ' . print_r($form_state['values'], true), 5);
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
			foreach($form_state['values'] as $key=>$adfsval){
				variable_set($key, $adfsval);
			
				expDebug::dPrint('print the Key = ' . $key);
				expDebug::dPrint('print the Value = ' . $adfsval);
			}
			$newADFSVal = array(
					'key' => $key,
					'samlval' => $adfsval,
			);
			
			$auditTrailAction = 'Add ADFS Server';
			$auditFieldsList = array_keys($newADFSVal);
			
			make_audit_trail_entries($newADFSVal, getIdOfLoggedInUser(), $newADFSVal, 0, $auditTrailAction, 'ADFS Admin',
					'exp_sp_administration_module_adfs','exp_sp_administration_module_adfs_form_submit', $auditFieldsList,getIdOfLoggedInUser());
			expertus_set_message(t('MSG689'));
		}
	}
	catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_module_adfs_form_submit', $ex);
		expertusErrorThrow($ex);
	}
}
function exp_sp_administration_module_adfs_form_validate($form, &$form_state){
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

			addEditCheckMandatoryFields($form, $form_state, array('adfs_idp_sso_target_url', 'adfs_sp_identity', 'adfs_e1_prp_url'));

			if (!empty($form_state['values']['adfs_idp_sso_target_url'])) {
				if (strpos($form_state['values']['adfs_idp_sso_target_url'], ' ') !== FALSE) {
					form_set_error('ADFSURL', t('ERR223'));
					$form_state['esign_show_popup'] = false;
				}
			}
			if (parse_url($form_state['values']['adfs_idp_sso_target_url'], PHP_URL_SCHEME) != 'https') {
				form_error($form['adfs_idp_sso_target_url'], t('ERR220'));
			}
			 
			if (!empty($form_state['values']['adfs_sp_identity'])) {
				if (strpos($form_state['values']['adfs_sp_identity'], ' ') !== FALSE) {
					form_set_error('identity', t('ERR224'));
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
		watchdog_exception('exp_sp_administration_module_adfs_form_validate', $ex);
		expertusErrorThrow($ex);
	}

}
?>