<?php
function user_addrValidation_AddEdit($js = NULL){
	try {
		expDebug::dPrint(' USPSAPI $js = ' . print_r($form, true) , 5);
		global $theme_key, $user;
		$js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
		drupal_add_js(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_addedit_behaviours.js', $js_module_optional);

                
			$defaultWidth = 310;
		
		$uspsapi_style = array(
				'ctools-admin-uspsapi-style' => array(
						'modalSize' => array(
								'width' => ($theme_key == "expertusoneV2") ? $defaultWidth : 615,
								'minHeight' => 20,
						),
				)
		);

		drupal_add_js($uspsapi_style, 'setting');

		if ($js) {
			// Prepare the arguments list to be passed to $formState['build_info']['args'] of each form
			$argsList = array();

			// Prepare list of forms to be displayed in the modal
			$formIdList = array(
					array('id' => 'exp_sp_administration_module_info_uspsapi_form',
							'title' => t('User Address Validation')),
			);

			exp_ctools_modal_multi_ajaxonly_forms_display($formIdList, $argsList);
		}
		else {
			throw new Exception('user_addrValidation_AddEdit');
		}
	}catch (Exception $ex) {
		watchdog_exception('user_addrValidation_AddEdit', $ex);
		expertusErrorThrow($ex);
	}
}

function exp_sp_administration_module_info_uspsapi_form($form, &$form_state){
	try {
				expDebug::dPrint(' USPSAPI $form = ' . print_r($form, true) , 5);
				//$formWrapperId = 'cdn-basic-addedit-form';
				$form = newAddEditFormWithCancelAndSave('uspsapi-basic-addedit-form');
				$form['uspsapi-basic-edit-form-start'] = array(
						'#type'   => 'markup',
						'#markup' => '<div id="uspsapi-basic-addedit-form" class="uspsapi-edit-rows">',
				);
                                
//                                $form['uspsapi-master-text'] = array(
//                                        '#type'    => 'markup',
//                                        '#markup'  => '<div class="uspsapi-master-text">'.t('Enter your USPS API username').': '.'</div>'  
//                                );

				$uspsApiID = variable_get('exp_sp_usps_integration_username','');
				addTextFieldToAddEditForm($form, 'uspsapi_id', t('API Username'), $uspsApiID, 1, true, false, 12);

				uspsapiAddEditFormWithCancelAndSaveClose($form, 'uspsapi-basic-addedit-form', 'uspsapi-save-button', 'uspsapi-cancel-button', 7);
				expDebug::dPrint('$form formvalue= ' . print_r($form, true), 5);

				$form['uspsapi-basic-edit-form-end'] = array(
						'#type'   => 'markup',
						'#markup' => '</div>',
				);

			return $form;
	}catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_module_info_uspsapi_form', $ex);
		expertusErrorThrow($ex);
	}
}

function uspsapiAddEditFormWithCancelAndSaveClose(&$form, $wrapperId, $saveButtonId, $cancelButtonId, $tabIndex) {
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
		watchdog_exception('uspsapiAddEditFormWithCancelAndSaveClose', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Default validate handler for exp_sp_administration_group_addedit_form_validate
 */

function exp_sp_administration_module_info_uspsapi_form_validate($form, &$form_state){
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

		/* start validation */
		if ($form_state['triggering_element']['#name'] == $validationTrigger) {
			$form_state['esign_show_popup']= true;

			if(empty($form_state['values']['uspsapi_id'])){
			     form_set_error('uspsapi-id', t('ERR171').' '.t('LBL1325'));
			     $form_state['esign_show_popup'] = false;
			}

			require_once 'sites/all/libraries/usps/src/USPSBase.php';
                        require_once 'sites/all/libraries/usps/src/Address.php';
                        require_once 'sites/all/libraries/usps/src/XMLParser.php';
                        require_once 'sites/all/libraries/usps/src/XML2Array.php';
                        require_once 'sites/all/libraries/usps/src/AddressVerify.php';
			$UserIdkey = $form_state['values']['uspsapi_id'];
			expDebug::dPrint("Testtttt-->>> ".$UserIdkey);
			$verify = new \USPS\AddressVerify($UserIdkey);
			$checkResponse = $verify->verify();
			$checkResponse = $verify->getArrayResponse();
			$checkResponse = $checkResponse['Error']['Description'];
			$response = strripos($checkResponse,'Authorization failure');
			expDebug::dPrint("Key Verification Status4 ---->>>> ".print_r($verify->verify(),true),5);
			expDebug::dPrint("Key Verification Status3 ---->>>> ".print_r($checkResponse,true),5);
			expDebug::dPrint("Key Verification Status31 ---->>>> ".print_r($response,true),5);
			if($response === 0){
				//form_set_error('uspsapi-id',  t('ERR171').' '.t('LBL1325'));
                                form_set_error('uspsapi-id',  t('ERR171').' '.t('UserID'));
			  $form_state['esign_show_popup'] = false;
			}
		}
		/* End validation */

		if ($form_state['triggering_element']['#name'] == 'hidden-save' || $form_state['triggering_element']['#ajax']['triggerEvent'] == 'hidden-save') {
			$form_state['hidden-save-submit'] = true;
		}

		$form_state['esign_validation_status'] = false; // checked to be false in exp_sp_administration.inc:addEditRepaintForm() as a precondition to
		// showing popup.
		if (!($form_state['hidden-save-submit'])) {
			unset($_SESSION['esign_validate']); // for every button other than hidden-save, unset $_SESSION['esign_validate'].
		}

	}catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_module_info_uspsapi_form_validate', $ex);
		expertusErrorThrow($ex);
	}

}

/**
 * Submit handler for exp_sp_administration_group_addedit_form_submit
 */
function exp_sp_administration_module_info_uspsapi_form_submit($form, &$form_state){
	try{
		expDebug::dPrint('Submit $form_state[values] = ' . print_r($form_state['values'], true), 5);
		expDebug::dPrint('$form_state[input] = ' . print_r($form_state['input'], true), 5);

		$key = $form_state['values']['uspsapi_id'];
                $defKey = variable_get('exp_sp_usps_integration_username','');

		variable_set('exp_sp_usps_integration_username',$key);
                $userId = getIdOfLoggedInUser();
                $entityId = db_select('slt_profile_list_items','spli');
                $entityId->addField('spli','id');
                $entityId->condition('spli.code','cre_ste_mod_USP','=');
                $entityId = $entityId->execute()->fetchField();
                expDebug::dPrint('Module entityId = ' .print_r($entityId,true), 4);
                // audit trail entry
                add_audit_trail_entry($userId, $entityId, 'cre_ste_mod_usp', 'exp_sp_administration_sitesetup', 'User Address Validation','API Username Added/Updated',
		                                     null, $defKey, $key);

		expertus_set_message(t('MSG689'));
		//drupal_flush_all_caches(); Use only if it is necessary. Hence this will take time to save
		$form_state['rebuild'] = TRUE;

	} catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_module_info_uspsapi_form_submit', $ex);
		expertusErrorThrow($ex);
	}
}
?>
