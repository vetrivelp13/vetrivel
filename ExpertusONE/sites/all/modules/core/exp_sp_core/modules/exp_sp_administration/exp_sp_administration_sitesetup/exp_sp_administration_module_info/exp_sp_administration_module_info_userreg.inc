<?php
function exp_sp_administration_userreg_display_addedit_form($js = NULL) {
	try {
		    global $theme_key;
			$js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
			drupal_add_js(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_addedit_behaviours.js', $js_module_optional);

			drupal_add_js(drupal_get_path('module', 'exp_sp_userregister') .'/exp_sp_userregister.js', $js_module_optional);

			$css_theme_optional = array('type' => 'file', 'group' => CSS_THEME, 'weight'=>999);

			//NEWUI THEME STYLE SHEET
				drupal_add_css(drupal_get_path('module', 'exp_sp_administration_config') .'/exp_sp_administration_config_v2.css', $css_theme_optional);


		// Default modalTheme is ExpertusCToolsModalTheme (see exp_sp_core_extend_ctools_ajax.js:Drupal.CTools.Modal.show())
		$userreg_style = array(
				'ctools-admin-userreg-style' => array(
						'modalSize' => array(
									  'width' => 450,
									  'minHeight' => 360,
								     ),
						     			)
		);
		drupal_add_js($userreg_style, 'setting');

		if ($js) {
			$argsList = array();

			// Prepare list of forms to be displayed in the modal
			$formIdList = array(
					array('id' => 'exp_sp_administration_userreg_addedit_form',
							  'title' => t('LBL1296')) //Group
			);
			exp_ctools_modal_multi_ajaxonly_forms_display($formIdList, $argsList);
		}
		else {
			throw new Exception('exp_sp_administration_userreg_display_addedit_form');
		}
	} catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_userreg_display_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}

function exp_sp_administration_userreg_addedit_form($form, &$form_state){
	try{

		global $theme_key;
		$wrapperId = 'userreg-detail-wrapper';

		$form = newAddEditFormUseregWithCancelAndSave($wrapperId);

		$usrattr = array('Address 1','Address 2','City','Country','Currency','Department','Employee ID','Employment Type','Job Role','Job Title','Language','Mobile','Organization','Phone','State','User Type','Zip Code');

		expDebug::dPrint('usr attr = ' . print_r($usrattr, true), 5);

		$form[$formName]['userreg_table_start'] = array(
				'#type' => 'markup',
		);

		$i=0;
		foreach($usrattr as &$value) {

			$uniqueid = 'registration_'.$value;
			expDebug::dPrint('usr attr id = ' . $uniqueid , 4);
			expDebug::dPrint('usr attr value = ' . variable_get($uniqueid,0) , 4);
			$checkedStatus = (variable_get($uniqueid,0)==1) ? 'checked' : '';
			$checkedClass =  (variable_get($uniqueid,0)==1) ? 'checkbox-selected' : 'checkbox-unselected';

			if($checkedStatus=='checked') {
				$i++;
			}
		}
		$usrattr_cnt = count($usrattr);
		expDebug::dPrint('$usrattr = ' . $usrattr_cnt , 4);
		expDebug::dPrint('$usrattr = ' . $i , 4);
		$checkedallStatus = ($i==$usrattr_cnt) ? 'checked' : '';
		$checkedall = ($i==$usrattr_cnt) ? 'checkbox-selected' : 'checkbox-unselected';


				$form[$formName]['userreg_fieldset'] = array(
				'#type' => 'markup',
				/*'#prefix' => '<table width="94%" class="userreg-table-heading" cellpadding="4" cellspacing="0" border="0">',
				'#suffix' => '',*/
				'#prefix' => '<table width="94%" class="group-table-heading" cellpadding="4" cellspacing="0" border="0">
	<th width="90%" style="'.$style.'" class="group-title-heading group-header heading-background-class-title">'.t("LBL1298").'</th>

	<th width="10%" style="padding-left: 0px; text-align:center;" class="group-title-heading group-header heading-background-class"><div class="usr-muliselect '.$checkedall.'" style="margin-left: 4px;">
    					<input type="checkbox" id="usr_select" class="multiselect-selectall" onclick="userregistercheckedall(this);" value="" '.$checkedallStatus.' /></div></th>',
		);

		$i=0;
		$j=0;
		foreach($usrattr as &$value){

			$uniqueid = 'registration_'.$value;
			expDebug::dPrint('usr attr id = ' . $uniqueid , 5);
			expDebug::dPrint('usr attr value = ' . variable_get($uniqueid,0) , 5);
			$checkedStatus = (variable_get($uniqueid,0)==1) ? 'checked' : '';
			$checkedClass =  (variable_get($uniqueid,0)==1) ? 'checkbox-selected' : 'checkbox-unselected';

			$class='even-list-class';
			if($j==1){
				$class='odd-list-class';
				$j=0;
			}else{
				$j=1;
			}
			$messageDisable = ($value == 'State') ? 'checkCountryDisableuserreg(); stateDisabledMessageuserreg();': '';
			$countryDisable = ($value == 'Country') ?  'checkCountryDisableuserreg();' : '';

			//Address 2','City','Country','Department','Employee ID','Employment Type','Job Role','Job Title','Language','Mobile',
			//'Organization','Phone','State','User Type','Zip Code'

			switch ($value) {
				case "Address 1":
					$title = 'LBL064';
				break;

				case "Address 2":
					$title = 'LBL065';
				break;

				case "City":
					$title = 'LBL066';
				break;

				case "Country":
					$title = 'LBL039';
				break;

				case "Department":
					$title = 'LBL179';
				break;

				case "Employee ID":
					$title = 'LBL294';
				break;

				case "Employment Type":
					$title = 'LBL174';
				break;

				case "Job Role":
					$title = 'LBL133';
				break;

				case "Job Title":
					$title = 'LBL073';
				break;

				case "Language":
					$title = 'LBL038';
				break;

				case "Mobile":
					$title = 'Mobile';
				break;

				case "Organization":
					$title = 'Organization';
				break;

				case "Phone":
					$title = 'LBL070';
				break;

				case "State":
					$title = 'LBL152';
				break;

				case "User Type":
					$title = 'LBL173';
				break;

				case "Zip Code":
					$title = 'LBL562';
				break;

				case "Currency":
					$title = 'LBL101';
					break;

			}


			  $form[$formName]['userreg_table_row'.$i] = array(
					'#type' => 'markup',
					'#prefix' => '<tr class="'.$class.'"><td class="check-attributes"><span class="vtip" title="'.t($title).'"> '.t($title).'</span></td>
     							  <td class="Checked-column"><div class="multichk '.$checkedClass.'" >
     							  <input type="checkbox" id = "userreg_'.$value.'" class = "attach-group-list"  name="attach_attributes[]" onclick="checkboxSelectedUnselectedCommon(this); '.$countryDisable.' '.$messageDisable.'" value="'.$value.'" '.$checkedStatus.'/>',
					'#suffix' => '</div></td></tr>',
			);


			$i++;
		}



		$form[$formName]['userreg_table_end'] = array(
				'#type' => 'markup',
				'#suffix' => '</table>',
		);

		addEditFormWithuserregCancelAndSaveClose($form, $wrapperId, $tabIndex);
		return $form;
	}
	catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_userreg_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Default validate handler for exp_sp_administration_userreg_addedit_form_validate
 */
function exp_sp_administration_userreg_addedit_form_validate($form, &$form_state){
	try {
		expDebug::dPrint('Validate $form_state[values] = ' . print_r($form_state['values'], true), 5);
		expDebug::dPrint('$form_state[triggering_element] = ' . print_r($form_state['input'], true), 5);

		$form_state['hidden-Save-submit'] = false;
		$form_state['esign_show_popup']= false;
		$form_state['storage']= true;

		if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
			$validationTrigger  = 'save';
		}
		else{
			$validationTrigger  = 'hidden-Save';
		}

		if ($form_state['triggering_element']['#name'] == 'hidden-Save' || $form_state['triggering_element']['#ajax']['triggerEvent'] == 'hidden-Save') {
			$form_state['hidden-Save-submit'] = true;
		}

		$form_state['esign_validation_status'] = false; // checked to be false in exp_sp_administration.inc:addEditRepaintForm() as a precondition to
		// showing popup.
		if (!($form_state['hidden-Save-submit'])) {
			unset($_SESSION['esign_validate']); // for every button other than hidden-Save, unset $_SESSION['esign_validate'].
		}
	} catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_userreg_addedit_form_validate', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Submit handler for exp_sp_administration_userreg_addedit_form_submit
 */
function exp_sp_administration_userreg_addedit_form_submit($form, &$form_state){
	try{

		expDebug::dPrint('$form_state[input] = ' . print_r($form_state['input'], true), 5);

		$newval=implode(',',$form_state['input']['attach_attributes']);

		$usrattr = array('Address 1','Address 2','City','Country','Currency','Department','Employee ID','Employment Type','Job Role','Job Title','Language','Mobile','Organization','Phone','State','User Type','Zip Code');
		$getcheckedattr  = $form_state['input']['attach_attributes'];

		$oldvalue = array();
		foreach($usrattr as &$value){
			$uniqueid = 'registration_'.$value;
			if((variable_get($uniqueid,0)==1)){
				expDebug::dPrint('Checked values:' .$uniqueid,5);
				$oldvalue[] = $value;
			}
		}
		$oldval = implode(",", $oldvalue);

		//reset existing values
		foreach ($usrattr as &$attr1) {
			variable_set('registration_'.$attr1,'0');
		}

		foreach ($getcheckedattr as &$attr) {
			expDebug::dPrint('usr attr $attr = ' . $attr , 5);
			variable_set('registration_'.$attr,'1');
		}

		//expertus_set_message(t('The Selected fields will be available for User Registration page'));
		expertus_set_message(t('MSG689'));
		$form_state['rebuild'] = TRUE;

		$entity=entityid();

		add_audit_trail_entry(1, $entity, 'cre_ste_mod_reg', 'exp_sp_administration_module_info', 'exp_sp_administration_userreg_addedit_form_submit', 'Updated User Reg Form', null, $oldval, $newval);

    	} catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_userreg_addedit_form_submit', $ex);
		expertusErrorThrow($ex);
	}
}

function entityid()
{
	try{
		$select = db_select('slt_profile_list_items','spfa');
		$select->addField('spfa','id','id');
		$select->condition('spfa.code','cre_ste_mod_reg');
		return $select->execute()->fetchField();
	}catch (Exception $ex) {
		watchdog_exception('entityid', $ex);
		expertusErrorThrow($ex);
	}
}




function newAddEditFormUseregWithCancelAndSave($wrapperId = 'addedit-form-wrapper', $miscAction = array()) {
	try{
		$form = array();
		//  $form_close_tag = (( stripos($_SERVER['HTTP_USER_AGENT'], 'Chrome')  || stripos($_SERVER['HTTP_USER_AGENT'], 'Firefox'))? '</form>' : '');
		$form_close_tag =  '</form>';
		$form['#prefix'] = $form_close_tag.'<div id="' . $wrapperId . '" class="addedit-form-wrapper">';
		$form['#suffix'] = '</div>';
		// Add a class to the form so that we can apply custom style (margin, padding, etc).
		$form['#attributes'] = array('class' => array('addedit-form'));
		$form['#attributes']['autocomplete'] = 'off';
		$form['#tree'] = TRUE; // Show full path of values (hierarchical values)
		$form['addedit-right-col-start'] = array(
				'#type' => 'markup',
				'#markup' => '<div class="addedit-right-col">',
		);

		/* TO DO - Misc buttons need to be redone. There is no nojs fallback anymore for forms, so there would be no Back To Search button
		 * $form['action_buttons'] = array(
		 		'#type' => 'markup',
		 		'#markup' => empty($miscAction)? '' : theme('addedit-misc-actions', $miscAction),
		 );*/

		$form['addedit-right-col-end'] = array(
				'#type' => 'markup',
				'#markup' => '</div>',
		);

		$form['addedit-left-col-start'] = array(
				'#type' => 'markup',
				'#markup' => '<div class="addedit-left-col">',
		);

		// These hidden form fields are needed to be able to communicate to ctools modal to refresh grid on close after a save op
		//addHiddenValueToAddEditForm($form, 'js_object_init_id', 'root-admin');
		//addHiddenValueToAddEditForm($form, 'js_object', 'narrowsearch');

		return $form;
	} catch (Exception $ex) {
		watchdog_exception('newAddEditFormUseregWithCancelAndSave', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Adds cancel and save button to exp_sp_administration_userreg_addedit_form
 */
function addEditFormWithuserregCancelAndSaveClose(&$form, $wrapperId, $tabIndex ,&$form_state) {
	try {
		// Button ids

		$saveButtonId = 'userreg-save-button';
		$cancelButtonId = 'userreg-cancel-button';
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
		watchdog_exception('addEditFormWithuserregCancelAndSaveClose', $ex);
		expertusErrorThrow($ex);
	}
}
?>
