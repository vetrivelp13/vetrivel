<?php 
function exp_sp_administration_webex_display_addedit_form($js = NULL) {
	try {
		  global $theme_key;
			$js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
			drupal_add_js(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_addedit_behaviours.js', $js_module_optional);
			//drupal_add_js(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration.js', $js_module_optional);

		// Default modalTheme is ExpertusCToolsModalTheme (see exp_sp_core_extend_ctools_ajax.js:Drupal.CTools.Modal.show())
		$webex_style = array(
				'ctools-admin-webex-style' => array(
						'modalSize' => array(
																'width' => 445,
	       											  'minHeight' => auto,
															),
														)
		);
		drupal_add_js($webex_style, 'setting');
		
		if ($js) {
			$argsList = array();
			 
			// Prepare list of forms to be displayed in the modal
			$formIdList = array(
					array('id' => 'exp_sp_administration_webex_addedit_form', 
							  'title' => t('WebEx Meeting')) //Group
			);
			exp_ctools_modal_multi_ajaxonly_forms_display($formIdList, $argsList);
		}
		else {
			throw new Exception('exp_sp_administration_webex_display_addedit_form');
		}
	} catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_webex_display_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}

function exp_sp_administration_webex_addedit_form($form, &$form_state){
	try{
		
		global $base_url;
		global $theme_key;
		$form_state['default_values'] = array();

		$wrapperId = 'webex-detail-wrapper';
		//$path = base_path() . drupal_get_path('module', 'exp_sp_administration_module_info');
		$path= $base_url.'/sites/all/themes/core/expertusoneV2/expertusone-internals/images';
		$form = newAddEditFormWithCancelAndSave($wrapperId);

$defvalue = (variable_get('webex_User') == 1)? 'webex_User':'webex_Organization';

$form['hidden'] = array(
		'#markup' => '<img style="display:none;" src="'.$base_url.'/sites/all/themes/core/expertusoneV2/logo.png" id="Img" onload="webex_onload()"/>',
		 
);

$form['webex_radio'] = array(
		'#type' => 'radios',
		//'#title' => '',
		'#prefix' => '<div class="addedit-new-field-title">'.t('WebEx').t(' ').t('Account').':&nbsp;<span>&nbsp;<img src="'.$path.'/help.png" class="vtip info-enr-upload" title="'.t('MSG758').'" onload="vtip();"></span></div>',
		'#options' => array(
				'webex_Organization' => t('Organization'),
				'webex_User' => t('User')),
		'#default_value' => $defvalue,
		'#attributes' => array('onclick' => 'webex_config(this.value);')
		);

		$form_state['default_values']['webex_radio'] = $defvalue;
		
	  $defaultvalueurl=variable_get('webex_url');
      addTextFieldToAddEditForm($form, 'webex_url', t('LBL611'), $defaultvalueurl, 1, True);
      $form_state['default_values']['webex_url'] = $defaultvalueurl;
	  
	  $feedprefix = str_replace('<div class="addedit-new-field-title">'.t('LBL611').':<span class=\'addedit-mandatory\'>*</span>', '<div class="addedit-new-field-title">'.t('LBL611').':<span class=\'addedit-mandatory\'>*</span>'.'&nbsp;<span>&nbsp;<img src="'.$path.'/help.png" class="vtip info-enr-upload" title="'.t('MSG759').'" onload="vtip();"></span>', $form['webex_url']['#prefix']);
	  $form['webex_url']['#prefix'] = $feedprefix;

	  $form['markup_start'] = array(
	  		'#type' => 'markup',
	  		'#prefix' => '<div id="webex_org_wrapper" style="display:none;">'
	  );
	  $defaultvaluename=variable_get('webex_name');
	  addTextFieldToAddEditForm($form, 'webex_name', t('LBL054'), $defaultvaluename, 2, True,False,50);
	  
	  $form_state['default_values']['webex_name'] = $defaultvaluename;
		  
	  $defaultvaluepass=variable_get('webex_pass');
	  addPasswordFieldToAddEditForm($form, 'webex_pass', t('LBL060'), $defaultvaluepass, 3, true);
	  
	  $form['hid'] = array(
	  		'#type' => 'hidden',
	  		'#default_value' => $defaultvaluepass	  		
	  );
	  
	  $form_state['default_values']['webex_pass'] = $defaultvaluepass;
	  
	  $form['markup_end'] = array(
	  		'#type' => 'markup',
	  		'#suffix' => '</div>'
	  );
 
      addEditFormWithGroupCancelAndSaveClose($form,$wrapperId,'');
     
        
      return $form;
	}
	catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_webex_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Default validate handler for exp_sp_administration_group_addedit_form_validate
 */
function exp_sp_administration_webex_addedit_form_validate($form, &$form_state){
	try {
		expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 5);
		expDebug::dPrint('$form_state[triggering_element] = ' . print_r($form_state['input'], true), 5);
		expDebug::dPrint("$form_state vali : ".print_r($form_state['default_values']['webex_pass'],true));
		expDebug::dPrint("$form_state pass indhu: ".print_r($form_state['webex_pass']['#default_value'],true));
		expDebug::dPrint("$form_state : ".print_r($form_state['default_values'],true));

		$form_state['hidden-Save-submit'] = false;
		$form_state['esign_show_popup']= false;
		$form_state['storage']= true;
		
	$getcheckedattr  = $form_state['values']['webex_radio'];
	
		if($getcheckedattr != 'webex_User'){
			
			if(empty($form_state['values']['webex_name'])){
			form_set_error('webex_name', t('LBL054').t(' ').t('ERR101'));
			$form_state['esign_show_popup']= false;
		}
		
		if(empty($form_state['values']['webex_pass']) && empty($form['webex_pass']['#default_value'])){
			form_set_error('webex_pass', t('LBL060').t(' ').t('ERR101'));
			$form_state['esign_show_popup']= false;
		}
		if(!empty($form_state['values']['webex_name']) && ($form_state['values']['webex_name'] <> $form['webex_name']['#default_value']) && empty($form_state['values']['webex_pass'])){
			form_set_error('webex_pass', t('LBL060').t(' ').t('ERR101'));
			$form_state['esign_show_popup']= false;
		}
		
	    if(!empty($form_state['values']['webex_name']) && empty($form_state['values']['webex_pass']) && empty($form['webex_pass']['#default_value'])){
    	form_set_error('webex_pass', t('LBL060').t(' ').t('ERR101'));
    	$form_state['esign_show_popup']= false;
    }
    
    if(empty($form_state['values']['webex_name']) && (!empty($form_state['values']['webex_pass']) || !empty($form['webex_pass']['#default_value']))){
    	form_set_error('webex_name', t('LBL054').t(' ').t('ERR101'));
    	$form_state['esign_show_popup']= false;
    }
		}
    
    if(empty($form_state['values']['webex_url'])){
    	form_set_error('webex_url', t('LBL611').t(' ').t('ERR101'));
    	$form_state['esign_show_popup']= false;
    }
	if (!empty($form_state['values']['webex_url'])) {
				if (!valid_url($form_state['values']['webex_url'],TRUE)) {
					form_set_error('webex_url', t('ERR208'));
					$form_state["storage"]["form_validate"] = false;
					$form_state['esign_show_popup']= false;
				}
			}
		 
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
		watchdog_exception('exp_sp_administration_webex_addedit_form_validate', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Submit handler for exp_sp_administration_webex_addedit_form_submit
 */
function exp_sp_administration_webex_addedit_form_submit($form, &$form_state){
	try{
		expDebug::dPrint('$form_state[values] = ' . print_r($form_state['input'], true), 5);
		expDebug::dPrint("$form_state : ".print_r($form_state['default_values']['webex_pass'],true));
					
		$getcheckedattr  = $form_state['values']['webex_radio'];
		
		expDebug::dPrint("Radio button checked " .$getcheckedattr,5);
		
		if($getcheckedattr == 'webex_User'){
		variable_set($getcheckedattr,1);
		variable_set('webex_Organization',0);
		}else{
			variable_set($getcheckedattr,1);
			variable_set('webex_User',0);
		}
		
		variable_set('webex_url',$form_state['values']['webex_url']);
		
		if($getcheckedattr == 'webex_Organization'){
		//expDebug::dPrint('inside webex org save');
		variable_set('webex_name',trim($form_state['values']['webex_name']));
		if(empty($form_state['values']['webex_pass'])){
		variable_set('webex_pass',$form_state['hid']['#default_value']);
		}else{
		variable_set('webex_pass',$form_state['values']['webex_pass']);
		}
		}
		
		
		$userid = getIdOfLoggedInUser();
		$entity=entityid();
		
		if($form_state['values']['webex_radio'] == 'webex_User'){
			$fields = array(
					'webex_radio' => $form_state['values']['webex_radio'],
					'webex_url' => $form_state['values']['webex_url']);
			$fields1 = 	array('webex_radio','webex_url');
				}else{
		$fields = array(
				'webex_radio' => $form_state['values']['webex_radio'],
				'webex_url' => $form_state['values']['webex_url'],
				'webex_name' =>  $form_state['values']['webex_name']);
		$fields1 = 	array('webex_radio','webex_url','webex_name');
				}
				
		make_webexsetting_audit_trail_entries($fields, $userid, $entity, $form_state['default_values'],$fields1);
		
		if($form_state['values']['webex_radio'] != 'webex_User' && !empty($form_state['values']['webex_pass'])){
			$fields = array('webex_pass' => $form_state['values']['webex_pass']);
			$fields1 = 	array('webex_pass');
			make_webexsetting_audit_trail_entries($fields, $userid, $entity, $form_state['default_values'],$fields1);
		}
		
		expertus_set_message(t('MSG689'));
	  $form_state['rebuild'] = TRUE;

	} catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_webex_addedit_form_submit', $ex);
		expertusErrorThrow($ex);
	}
}

function entityid()
{
	try{
		$select = db_select('slt_profile_list_items','spfa');
		$select->addField('spfa','id','id');
		$select->condition('spfa.code','cre_ste_mod_wex');
		return $select->execute()->fetchField();
	}catch (Exception $ex) {
		watchdog_exception('entityid', $ex);
		expertusErrorThrow($ex);
	}
}


/**
 * Adds cancel and save button to exp_sp_administration_pwdpolicy_form with password strength qtip popup links
 */
function addEditFormWithGroupCancelAndSaveClose(&$form, $wrapperId, $tabIndex) {
	try {
		
		expDebug::dPrint('$form_state[values] submit= ' . print_r($form_state['values'], true), 5);
		expDebug::dPrint('$form_state[triggering_element] submit= ' . print_r($form_state['input'], true), 5);
		
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

function make_webexsetting_audit_trail_entries($resultRecord, $userId, $entityid, $defaultValues, $formFieldsList = array()) {
	try{
		$activity_id=null;
		foreach ($formFieldsList as $formField) {
			expDebug::dPrint(' $formField = ' . print_r($formField, true) , 4);
			expDebug::dPrint(' newVal = ' . print_r(TRIM($resultRecord[$formField]), true) , 4);
			expDebug::dPrint(' oldVal = ' . print_r($defaultValues[$formField], true) , 4);
			if (TRIM($resultRecord[$formField]) != $defaultValues[$formField]) {
		  $insertId=add_audit_trail_entry($userId, $entityid, 'cre_ste_mod_wex', 'exp_sp_administration_webex_addedit_form_sumbit', 'webexsetting', 'Updated ' . $formField,
		  		$userId, $defaultValues[$formField], TRIM($resultRecord[$formField]),null,null,null,null,$activity_id);
	  }
	  if($activity_id==null){
		 	$activity_id = $insertId;
		 }
		}
	}catch (Exception $ex) {
		watchdog_exception('make_mymeeting_audit_trail_entries', $ex);
		expertusErrorThrow($ex);
	}
}
?>
