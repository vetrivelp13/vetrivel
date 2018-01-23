<?php 
function exp_sp_administration_group_display_addedit_form($js = NULL) {
	try {
		  global $theme_key;
			$js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
			drupal_add_js(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_addedit_behaviours.js', $js_module_optional);

		// Default modalTheme is ExpertusCToolsModalTheme (see exp_sp_core_extend_ctools_ajax.js:Drupal.CTools.Modal.show())
		$group_style = array(
				'ctools-admin-group-style' => array(
						'modalSize' => array(
																'width' => 450,
	       											  'minHeight' => 360,
															),
														)
		);
		drupal_add_js($group_style, 'setting');
		
		if ($js) {
			$argsList = array();
			 
			// Prepare list of forms to be displayed in the modal
			$formIdList = array(
					array('id' => 'exp_sp_administration_group_addedit_form', 
							  'title' => t('Group')) //Group
			);
			exp_ctools_modal_multi_ajaxonly_forms_display($formIdList, $argsList);
		}
		else {
			throw new Exception('exp_sp_administration_group_display_addedit_form');
		}
	} catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_group_display_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}

function exp_sp_administration_group_addedit_form($form, &$form_state){
	try{
		expDebug::dPrint('$form2222 = ' . print_r($form, true), 5);
		expDebug::dPrint('$$form_state11111 = ' . print_r($form_state['values'], true), 5);
		global $theme_key;
		$wrapperId = 'group-detail-wrapper';
		
		$form = newAddEditFormWithCancelAndSave($wrapperId);
	
		$groupList = getGroupAttributes();
	 	$form[$formName]['group_table_start'] = array(
        '#type' => 'markup',
     );  
	 	if($theme_key == 'expertusoneV2'){
	 		$style = 'padding-left: 15px';
	 	}else{
	 		$style = 'padding-left: 0';
	 	}
     $form[$formName]['group_fieldset'] = array(
        '#type' => 'markup',
        '#prefix' => '<table width="94%" class="group-table-heading" cellpadding="4" cellspacing="0" border="0">
     									<th width="90%" style="'.$style.'" class="group-title-heading group-header heading-background-class-title">'.t('LBL1266').'</th>
											<th width="10%" style="padding-left: 0px; text-align:center;" class="group-title-heading group-header heading-background-class"></th>',
        '#suffix' => '</div>',
     );  
     
    $i=0;
    $j=0;
    foreach($groupList as $key => $val){
    	$uniqueid = $val->id;
      $checkedStatus = ($val->enabled==1) ? 'checked' : '';
      $checkedClass =  ($val->enabled==1) ? 'checkbox-selected' : 'checkbox-unselected';

      $class='even-list-class';
      if($j==1){
      	$class='odd-list-class';
      	$j=0;
      }else{
      	$j=1;
      }
      $messageDisable = ($val->name == 'State') ? 'checkCountryDisable(); stateDisabledMessage();': '';
      $countryDisable = ($val->name == 'Country') ?  'checkCountryDisable();' : '';
      // Multilanguage Support
      if($val->name == 'Country'){
      	$val->name = t('LBL039');
      	$val->id_name = 'Country';
      }else if($val->name == 'Department'){
      	$val->name = t('LBL179');
      	$val->id_name = 'Department';
      }else if($val->name == 'Employment Type'){
      	$val->name = t('LBL174');
      	$val->id_name = 'Employment Type';
      }else if($val->name == 'Role'){
      	$val->name = t('Role');
      	$val->id_name = 'Role';
      }else if($val->name == 'Job Role'){
      	$val->name = t('LBL133');
      	$val->id_name = 'Job Role';
      }else if($val->name == 'Language'){
      	$val->name = t('LBL038');
      	$val->id_name = 'Language';
      }else if($val->name == 'State'){
      	$val->name = t('LBL152');
      	$val->id_name = 'State';
      }else if($val->name == 'User Type'){
      	$val->name = t('LBL173');
      	$val->id_name = 'User Type';
      }
      $form[$formName]['permissions_table_row'.$i] = array(
        '#type' => 'markup',
        '#prefix' => '<tr class="'.$class.'"><td class="check-attributes"><span class="vtip" title="'.sanitize_data(t($val->name)).'"> '.titleController('ADMIN-CONFIG-GROUP-CHECK-ATTRIBUTES-TITLE',t($val->name),45).'</span></td><td class="Checked-column"><div class='.$checkedClass.'><input type="checkbox" id = "add_grp_'.$val->id_name.'" class = "attach-group-list" onclick="checkboxSelectedUnselectedCommon(this); '.$countryDisable.' '.$messageDisable.'" name="attach_attributes'.$uniqueId.'[]" value="'.$val->code.'" '.$checkedStatus.'/>',
        '#suffix' => '</div></td></tr>',
      ); 
      //$form_state['input']['group_fieldset']['group_hidden'][$val->code] = array('#type' => 'hidden', '#value' => $val->enabled);
      $i++;
      
    }
    
    $form[$formName]['group_table_end'] = array(
        '#type' => 'markup',
        '#suffix' => '</table>',
     );  
		
		addEditFormWithGroupCancelAndSaveClose($form, $wrapperId, $tabIndex);
		return $form;
	}
	catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_group_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Default validate handler for exp_sp_administration_group_addedit_form_validate
 */
function exp_sp_administration_group_addedit_form_validate($form, &$form_state){
	try {
		expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 5);
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
		watchdog_exception('exp_sp_administration_group_addedit_form_validate', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Submit handler for exp_sp_administration_group_addedit_form_submit
 */
function exp_sp_administration_group_addedit_form_submit($form, &$form_state){
	try{
    expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 5);
		expDebug::dPrint('$form_state[input] = ' . print_r($form_state['input'], true), 5);
	  updateGroupAttributes($form_state);
	  expertus_set_message(t('MSG689'));
	  $form_state['rebuild'] = TRUE;

	} catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_group_addedit_form_submit', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Fetch group attriutes from criteria table
 */
function getGroupAttributes(){
	try{
		$select = db_select('slt_site_configuration','site');
		$select->addField('site','id','id');
		$select->addField('site','name','name');
		$select->addField('site','code','code');
		$select->addField('site','enabled','enabled');
		expDebug::dPrintDBAPI('$select->',$select);
		$result = $select->execute()->fetchAll();
		return $result;
	}
	catch (Exception $ex) {
		watchdog_exception('getGroupAttributes', $ex);
		expertusErrorThrow($ex);
	}
}
/**
 * update the attribute value
 */
function updateGroupAttributes($form_state){
	try{
		$getcheckedattr  = $form_state['input']['attach_attributes'];
		$attrcode = implode(',',$form_state['input']['attach_attributes']);
		
		expDebug::dPrint('$attrcode--> ' . $attrcode, 5);
		$update = db_update('slt_site_configuration');
		$update->fields(array('enabled'=>0));
		$update->execute();
		
		if(!empty($getcheckedattr)){
			$update = db_update('slt_site_configuration');
			$update->fields(array('enabled'=>1));
			$update->condition('code', $getcheckedattr, 'in');
			$update->execute();
		}
	}
	catch (Exception $ex) {
		watchdog_exception('updateGroupAttributes', $ex);
		expertusErrorThrow($ex);
	}
}
/**
 * Adds cancel and save button to exp_sp_administration_pwdpolicy_form with password strength qtip popup links
 */
function addEditFormWithGroupCancelAndSaveClose(&$form, $wrapperId, $tabIndex) {
	try {
		// Button ids
		$saveButtonId = 'group-save-button';
		$cancelButtonId = 'group-cancel-button';
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
		watchdog_exception('addEditFormWithGroupCancelAndSaveClose', $ex);
		expertusErrorThrow($ex);
	}
}

?>