<?php 
function salesforce_integration_AddEdit($js = NULL){
	try {
		expDebug::dPrint(' SalesForce $js = ' . print_r($form, true) , 5);
		global $theme_key, $user;
		$js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
		drupal_add_js(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_addedit_behaviours.js', $js_module_optional);

		if ($user->language == 'ru') {
			$defaultWidth = 894;
		} else if ($user->language == 'de'){
			$defaultWidth = 835;
		} else if ($user->language == 'es'){
			$defaultWidth = 935;
		} else if ($user->language == 'fr'){
			$defaultWidth = 815;
		} else if ($user->language == 'it'){
			$defaultWidth = 775;
		} else if ($user->language == 'pt-pt'){
			$defaultWidth = 850;
		} else{
			//$defaultWidth = 620;
			$defaultWidth = 740;
		}
    
		$sf_style = array(
				'ctools-admin-sf-style' => array(
						'modalSize' => array(
								'width' => ($theme_key == "expertusoneV2") ? $defaultWidth : 615,
								'minHeight' => 20,
						),
				)
		);

		drupal_add_js($sf_style, 'setting');

		if ($js) {
			// Prepare the arguments list to be passed to $formState['build_info']['args'] of each form
			$argsList = array();
				
			// Prepare list of forms to be displayed in the modal
			$formIdList = array(
					array('id' => 'exp_sp_administration_module_info_salesforce_integration_form',
							'title' => t('SalesForce Settings')),
			);

			exp_ctools_modal_multi_ajaxonly_forms_display($formIdList, $argsList);
		}
		else {
			throw new Exception('salesforce_integration_AddEdit');
		}
	}catch (Exception $ex) {
		watchdog_exception('salesforce_integration_AddEdit', $ex);
		expertusErrorThrow($ex);
	}
}

function getdefaultvariableval(){
	try{
		$sf_feed = variable_get('salesforce_feed_trigger_option',0);
		$sf_enr_comp = variable_get('salesforce_comp_enr_option',0);
		$sf_namespace = variable_get('salesforce_namespace_prefix','');
		$sf_debug = variable_get('salesforce_debug','');
		$e1_sf_id = variable_get('expertusone_sf_id','');
		$sf_consumer_keys = variable_get('sf_consumer_keys','');
        $sf_consumer_secret_keys = variable_get('sf_consumer_secret_keys','');
		$sf_api_url = variable_get('salesforce_api_url','');
		
		$defaultval[0] = new stdClass();
		$defaultval[0]->salesforce_namespace_prefix = $sf_namespace;
		$defaultval[0]->salesforce_feed_trigger_option   = $sf_feed;
		$defaultval[0]->salesforce_comp_enr_option   = $sf_enr_comp;
		$defaultval[0]->salesforce_debug   = $sf_debug;
		$defaultval[0]->expertusone_sf_id   = $e1_sf_id;
		$defaultval[0]->sf_consumer_keys   = $sf_consumer_keys;
		$defaultval[0]->sf_consumer_secret_keys   = $sf_consumer_secret_keys;
		$defaultval[0]->sf_api_url   = $sf_api_url;
		
		return $defaultval;
		
		}catch(Exception $ex){
		watchdog_exception('getdefaultvariableval', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * addEditRepaintFormreuse() - After the form has been validated and saved, this function returns the form
 *                            to the client for re-rendering.
 */
function addEditRepaintFormreuse($form, &$form_state) {
	try{
  expDebug::dPrint(' addEditRepaintFormreuse() called.', 5);
  expDebug::dPrint(' $form = ' . print_r($form, true) , 5);
  expDebug::dPrint('  $form_state = ' . print_r($form_state, true) , 5);
  expDebug::dPrint(' $form_state[storage] = ' . print_r($form_state['storage'], true) , 5);
  expDebug::dPrint(' $form_state[build_info] = ' . print_r($form_state['build_info'], true) , 5);
  $ErrorCode = $form_state['esign_show_popup'];
  expDebug::dPrint(' storageCode 111 = ' . print_r($ErrorCode, true) , 5);

  // If a new entity was added, build additional forms
  if (!empty($form_state['storage']['new_entity']) && $form_state['storage']['new_entity'] == TRUE) {
    // Create the additional now editable forms if any present
    $formInfoList = $form_state['storage']['form_info_list'];
    $argsList = $form_state['build_info']['args'];
    array_shift($argsList); // The first arg is 'ajax', which we need to drop from $argsList

    $renderedAddlForms = exp_ctools_modal_multi_ajaxonly_forms_render($formInfoList, $argsList, 'ADD-TO-EDIT');
  }

  $commands = array();

  // Build HTML for the custom fields block if triggering element is Add custom or Delete custom button
  // We paint only the custom fields block in this case, and not the full form.
  if (!empty($form_state['triggering_element']['#name']) &&
               ($form_state['triggering_element']['#name'] == 'Add custom' || substr($form_state['triggering_element']['#name'], 0, 13) == 'Delete custom')) {
    $customFieldsWrapperId = $form_state['triggering_element']['#ajax']['custom_fields_wrapper_id'];
    expDebug::dPrint(' $customFieldsWrapperId = ' . print_r($customFieldsWrapperId, true) , 5);

    $customFieldsFormElements = array();
    //$customFieldsFormElementsKeysForDebugging = array();
    $startCapture = false;
    foreach ($form as $elemKey => $elemAttrib) {
      expDebug::dPrint(' $elemKey = ' . print_r($elemKey, true) , 4);
      expDebug::dPrint(' $elemAttrib = ' . print_r($elemAttrib, true), 4);

      if (!$startCapture && $elemKey != 'customfields_wrapper_start') {
        continue;
      }

      $startCapture = true;
      $customFieldsFormElements[] = $form[$elemKey];
      //$customFieldsFormElementsKeysForDebugging[] = $elemKey;

      if ($elemKey == 'customfields_wrapper_end') {
        break; // break out of form; we have got all the fields that we want to repaint
      }
    }
    expDebug::dPrint(' $customFieldsFormElementsKeysForDebugging = ' . print_r($customFieldsFormElementsKeysForDebugging, true) , 4);

    $renderHTML = drupal_render($customFieldsFormElements);
    expDebug::dPrint(' $renderHTML = ' . print_r($renderHTML, true)  , 4);
    $commands[] = ajax_command_replace('#' . $customFieldsWrapperId, $renderHTML);
  }
  else { // repaint the full form
  if(empty($ErrorCode) && $ErrorCode == '' ) {
    $commands[] = ajax_command_invoke(NULL, 'dynamicstylechangeAdd');
  } else {
   $commands[] = ajax_command_replace(NULL, drupal_render($form));
  }
  }

  // Render additional forms, if moving from add to edit
  if (!empty($renderedAddlForms)) {
    $commands[] = ajax_command_after(NULL, $renderedAddlForms);
  }

  // Resize the modal backdrop, adjust the model top (and left) to optimal
  // Show the status messages, if any.
  // Ensure the status messages are auto removed after a brief while
  $statusMsgHTML = theme('status_messages');
  expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true) , 4);
  if (!empty($statusMsgHTML)) {
  // Show the messages
  //$commands[] = ajax_command_prepend(NULL, $statusMsgHTML);
  $commands[] = ajax_command_html('#show_expertus_message', $statusMsgHTML);
 }
 
  // If a new entity was added or an existing entity was edited, inform ctools to refresh narrow search results no modal close.
  if ($form_state['storage']['new_entity'] == TRUE || $form_state['storage']['edited_entity'] == TRUE) {
    $commands[] = ajax_command_refresh_narrow_search_results('onclose');
  }

  if($form_state['values']['attr_action'] == 'certificate' && $form_state['triggering_element']['#name']=='save'){
     $defaulttCertificate=$form_state['default_certificate_id'];
     $commands[] = ajax_command_invoke(NULL, "validateCertificate",array($defaulttCertificate));
  }
  if (isset($form_state['cutom_fields_wrapper_id'])){
    $customWrapperId = $form_state['cutom_fields_wrapper_id'];
    $customErrorFlag = $form_state['custom_fields_error'];
    $commands[] = ajax_command_invoke(NULL, "validateCustomFields", array($customWrapperId, $customErrorFlag));
  }

  if ($form_state['triggering_element']['#ajax']['add_custom_gray_field'] == true){
    expDebug::dPrint(' $form_state[triggering_element][#ajax][add_custom_gray_field] = ' . $form_state['triggering_element']['#ajax']['add_custom_gray_field'] , 5);
    $commands[] = ajax_command_invoke(NULL, "addCustomGrayConversion", array($form_state['custom_fields']['count']));
  }

  /* For create Location in Class Page*/
  if($form_state['triggering_element']['#ajax']['from_tooltip_location'] == 'Yes'){
    $commands[] = ajax_command_invoke(NULL, "classCreateLocation", array($form_state['storage']['locationId'],$form_state['storage']['locationName'],$form_state['triggering_element']['#ajax']['from_tooltip_location_classId']));
  }
  /*---*/
  //calling scroll function in security role
  $var =$form_state['triggering_element']['#ajax']['wrapper'];
  if(strpos($var,'permissions-addedit-form')!== false){
  	$commands[] = ajax_command_invoke(NULL, "scrollPerm", array($var));
  }
  /*==*/
  //calling scroll function in tags
  $var =$form_state['triggering_element']['#ajax']['wrapper'];
  if(strpos($var,'tag-fieldset-wrapper')!== false){
  	$commands[] = ajax_command_invoke(NULL, "scrollTag", array($var));
  }
  if (module_exists('exp_sp_esignature') && isActiveEsignRegion() && (!$form_state['esign_validation_status'])){
    $custButtonId  = $form_state['triggering_element']['#ajax']['customButtonId'];
    $drupalUserId  = $form_state['triggering_element']['#ajax']['drupalUserId'];
    $drupalUserId  = $form_state['values'][$drupalUserId];
    $esignObj  = '{"popupDiv":"modalContent","esignFor":"AddAdmin","drupalUserId":"'.$drupalUserId.'","esignButtId":"'.$custButtonId.'"}';
    if($form_state['esign_show_popup'] == true){
      $commands[] = ajax_command_invoke(NULL, 'getNewEsignPopup',array($esignObj));
      $form_state['esign_show_popup']=false; // this is ignored, as form state is not saved after the ajax callback is executed
    }
  }
  //0053694: SF-Survey | Survey code fadeout
  $commands[] = ajax_command_invoke(NULL, "callVtip", array(1));
  $commands[0]['data'] = mb_convert_encoding($commands[0]['data'], "HTML-ENTITIES", "UTF-8");
  expDebug::dPrint(' $commands = ' . print_r($commands, true) , 4);

  return array('#type' => 'ajax', '#commands' => $commands);
  } catch (Exception $ex) {
  	watchdog_exception('addEditRepaintFormreuse', $ex);
  	expertusErrorThrow($ex);
  }
}

function remove_org_from_sfconfig() {
		
	try{ 
		$query_org_del=db_select('slt_cstm_salesforce_settings','scss');
		$query_org_del->innerJoin('slt_organization','org','org.id=scss.exp_org_id');
		$query_org_del->leftJoin('slt_custom_fields','scf','scf.entity_type=\'organization\' and scf.label=\'Salesforce Org ID\' and scf.entity_id=org.id and scf.value!=\'\'');
		$query_org_del->where('scf.value is null or org.status=\'cre_org_sts_del\'');
		 
		$query_org_del->addField('org', 'id', 'org_id');
		
		expDebug::dPrintDBAPI(' $query_org_del = ' , $query_org_del,5);
		
		$result_org_del=$query_org_del->execute();
		 
		$result_del=$result_org_del->fetchAll();
				
		expDebug::dPrint('$result_del = ' . print_r($result_del, true),4);
		if(count($result_del)>0){
			$del_org_arr=array();
			foreach ($result_del as $row => $row_value)	{ 
				$del_org_arr[]=$row_value->org_id;
			}
			expDebug::dPrint('$del_org_arr = ' . print_r($del_org_arr, true),4);
			
			$deleteStmt = db_delete('slt_cstm_salesforce_settings');
			$deleteStmt->condition('exp_org_id', $del_org_arr,'IN');
			
			expDebug::dPrintDBAPI('$deleteStmt = ' , $deleteStmt,5);
			
			$deleteStmt->execute();
			
		}else{
			expDebug::dPrint('There is no deleted Organization found to delete from salesforce settings.',4);
		} 
		
	}catch (Exception $ex) {
		  	watchdog_exception('remove_org_from_sfconfig', $ex);
		    expertusErrorThrow($ex);
	} 
}

function exp_sp_administration_module_info_salesforce_integration_form($form, &$form_state){
	try {
		global $theme_key, $user;
		expDebug::dPrint(' SalesForce $form = ' . print_r($form, true) , 5);
		expDebug::dPrint(' SalesForce $form state = ' . print_r($form_state, true) , 5);
		$path = base_path() . drupal_get_path('module', 'uc_credit');
		$form = newAddEditFormWithCancelAndSave('sfserver-basic-addedit-form');
		$form['sf-basic-edit-form-start'] = array(
				'#type'   => 'markup',
				'#markup' => '<div id="sf-basic-addedit-form" class="sf-edit-rows">'
		);  
		
		// Removing organization details(which was removed from LMS) from Salesforce settings before list form
	  	remove_org_from_sfconfig();
		
		$results=array();
		
		 $query_sf=db_select('slt_organization','org');
				   $query_sf->innerJoin('slt_custom_fields','scf','scf.entity_type=\'organization\' and scf.label=\'Salesforce Org ID\' and scf.entity_id=org.id and scf.value!=\'\'');
				   $query_sf->leftJoin('slt_cstm_salesforce_settings','scss','scss.exp_org_id=org.id');
				   $query_sf->condition('org.status','cre_org_sts_del','!=');
				   $query_sf->fields('org',array('id'));
				   $query_sf->fields('org',array('name'));   
				   $query_sf->fields('scss',array('consumer_secret_key'));
				   $query_sf->fields('scss',array('consumer_key'));
				   $query_sf->fields('scss',array('system_user'));
				   $query_sf->fields('scss',array('system_user_pwd'));
				   $query_sf->fields('scss',array('enr_syncup'));
				   
		expDebug::dPrintDBAPI(' $query_sf selectStmt SQL = ' , $query_sf,5);
		
		$result_sf = $query_sf->execute()->fetchAll();
		$count_details = count($result_sf);
		
		expDebug::dPrint('$query_sf selectStmt SQL = ' .print_r($result_sf,true),5);
		
		$consumer_secret_key = $result_sf[0]->consumer_secret_key;
		$consumer_key = $result_sf[0]->consumer_key;
		$system_user = $result_sf[0]->system_user;
		$system_user_password = $result_sf[0]->system_user_pwd;
		
		expDebug::dPrint('consumer_secret_key = ' .$consumer_secret_key,5);
		expDebug::dPrint('consumer_key = ' .$consumer_key,5);
		expDebug::dPrint('system_user = ' .$system_user,5);
		expDebug::dPrint('system_user_password = ' .$system_user_password,5);  
				
		// Getting the consumer secret keys & consumer keys alone from variable table since its common and if there are no salesforce organization created in LMS then the datas will be null in the slt_cstm_salesforce_setting table.
		addTextFieldToAddEditForm($form, 'consumer_secret_key', t('LBL3100'), $consumer_secret_key, 1, true); 
				
		addTextFieldToAddEditForm($form, 'consumer_key', t('LBL3101'), $consumer_key, 2, true);

        addTextFieldToAddEditForm($form, 'expertusone_sf_id', t('LBL3106'), variable_get('expertusone_sf_id', ''), 3, true);
		
		addTextFieldToAddEditForm($form, 'salesforce_namespace_prefix', t('LBL3102'), variable_get('salesforce_namespace_prefix', ''), 4, false);
		
		//Added by ganeshbabuv on April 27th 2017 #0074905: Requesting to change the salesforce rest api URL according to production and testing server.
		addTextFieldToAddEditForm($form, 'salesforce_api_url', t('LBL3111'), variable_get('salesforce_api_url', 'https://login.salesforce.com/services/oauth2/token'), 4, false); 

		addTextFieldToAddEditForm($form, 'salesforce_debug', t('LBL3105'), variable_get('salesforce_debug',''), 5, false);
		
		addCheckBoxToAddEditForm($form,'salesforce_feed_trigger_option', t('LBL3103'), true, variable_get('salesforce_feed_trigger_option',0), 6, false);
		
		expDebug::dPrint('Form value = ' .$form['salesforce_feed_trigger_option']['#prefix'], 5);
		$feedprefix = str_replace('<div class="addedit-new-field-title">'.t('LBL3103').':</div>', '<div class="addedit-new-field-title">'.t('LBL3103').':<span>&nbsp;<img src="'.$path.'/images/info.png" class="vtip info-enr-upload" title="'.t('MSG750').'" onload="vtip();"></span></div>', $form['salesforce_feed_trigger_option']['#prefix']);
		$form['salesforce_feed_trigger_option']['#prefix'] = $feedprefix;
		
       if($theme_key == 'expertusoneV2'){
	 		$style_label = 'padding: 0 4px;';
	 		$style_th="padding: 0 0 0 3px";
	 		$prefixBgLeft   = '<div class="white-btn-bg-left"></div>';
	 		$suffixBgRight = '<div class="white-btn-bg-right"></div>';
	 	}else{
	 		$style = 'padding-left: 0';
	 	}
		
		if ($user->language == 'ru') {
			$defaultWidth = 820;
		} else if ($user->language == 'de'){
			$defaultWidth = 773;
		} else if ($user->language == 'es'){
			$defaultWidth = 865;
		} else if ($user->language == 'ja'){
			$defaultWidth = 680;
		} else if ($user->language == 'ko'){
			$defaultWidth = 675;
		} else if ($user->language == 'pt-pt'){
			$defaultWidth = 790;
		} else if ($user->language == 'zh-hans'){
			$defaultWidth = 675;
		} else if ($user->language == 'it'){
			$defaultWidth = 710;
		}  else if ($user->language == 'fr'){
			$defaultWidth = 750;
		} else{
			$defaultWidth = 676;
		}
		 
        $vtipheader = '<span>&nbsp;<img src="'.$path.'/images/info.png" class="vtip info-enr-upload" title="'.t('MSG751').'" onload="$(\'#multi_sf_org_setup_div\').jScrollPane({});"></span>';
		if($count_details>0){
			$tablelist='<div id="tableholder"><div id="multi_sf_org_setup_div" style="max-height: 100px; overflow: hidden;"><table width="'.$defaultWidth.'" class="sf-table-heading" cellpadding="4" cellspacing="0" border="0">
			<th style="width:16%;line-height: 25px; padding: 0px 10px; font-family: proximanovabold,arial; font-size:12px; font-weight: normal;'.$style_label.'" class="sf-title-heading sf-header heading-background-class-title">'.t('LBL137').'</th>
			<th style="width:20%;line-height: 25px; padding: 0px 10px; font-family: proximanovabold,arial; font-size:12px; font-weight: normal;'.$style_label.'" class="sf-title-heading sf-header heading-background-class-title">'.t('LBL054').'</th>
			<th style="width:20%;line-height: 25px; padding: 0px 10px; font-family: proximanovabold,arial; font-size:12px; font-weight: normal;'.$style_label.'" class="sf-title-heading sf-header heading-background-class-title">'.t('LBL060').'</th>
			<th style="width:22%;line-height: 25px; padding: 0px 10px; font-family: proximanovabold,arial; font-size:12px; font-weight: normal;'.$style_label.'" class="sf-title-heading sf-header heading-background-class-title">'.t('LBL3104').' '.$vtipheader.'</th>';
		
		
		$form['sf_fieldset'] = array(
        	'#type' => 'markup',
        	'#prefix' => $tablelist,
        	'#suffix' => '</div></div>',
        );
        
      	$j=0;
		$tablebodyrow='';
		$rows = array();           
		foreach($result_sf as $row => $row_value){ 
					   $check_enr_arr=array();
					   if($row_value->enr_syncup=='1'){
					        $check_enr_arr="value =" .$row_value->enr_syncup. " checked";
					   }else{
					   	  	$check_enr_arr= ($row_value->enr_syncup=='') ? 0 : "value =" .$row_value->enr_syncup;
					       }
					   $class='odd-list-class';
		    		   if($j==1){
		    				$class='even-list-class';
		    				$j=0;
		    		   }else{
		    				$j=1;
		    		   }
		    		   
				$tablebodyrow.='<tr class="rowtbody row-'.$row_value->name.' '.$class.'"><td class="check-attributes"><input type="hidden" name="org_id_'.$row.'" value="'.$row_value->id.'" ><span class="vtip" title="'.sanitize_data(t($row_value->name)).'"> '.$row_value->name.'</span></td>';
				$tablebodyrow.='<td class="check-attributes"><input id="system_user_'.$row.'" type="text" name= "system_user_'.$row.'" value="'.$row_value->system_user.'"></td><td class="check-attributes"><input id="system_user_pwd_'.$row.'" type="password" name= "system_user_pwd_'.$row.'" value="'.$row_value->system_user_pwd.'"></td>';
				$tablebodyrow.='<td class="check-attributes" style="text-align: center;"><div class="publish_checkbox"><input type="checkbox" name="enr_syncup_'.$row.'" '.$check_enr_arr.' ></div></td>';
				
		
		    	$tablebodyrow.='</tr>';
				
				$form['sf_table_row'] = array(
		    		'#type' => 'markup',
		    		'#prefix' => $tablebodyrow,
		    	);
		}

		if($count_details<=0){ //if there is no organization result set then row value is 0 else increase it.
		         $row=0;
		      }else{
		         $row++;
		      }
		    	
		 
		
    	$form['sf_table_end'] = array(
       		 '#type' => 'markup',
       		 '#suffix' => '</table></div>',
		);
		}else{
	 		$tablebodyrow = "<div id=\"tableholder\"><div id=\"multi_sf_setup_div\"><div width=\"130px\" height=\"100px\" class=\"sf-table-heading admin-empty-text-msg\" style=\"border:0px;\">".t('MSG752')."</div>";
			$form['sf_table_row'] = array(
    		'#type' => 'markup',
    		'#prefix' => $tablebodyrow,
    		'#suffix' => '</div></div>',
    	);
			
	 	}
	 	
		$form['key']=array('#type' => 'hidden','#value' => $row);
		
		$form['sf-basic-edit-form-end'] = array(
				'#type'   => 'markup',
				'#markup' => '</div>',
		);
		
		sfAddEditFormWithCancelAndSaveClose($form, 'sfserver-basic-addedit-form', 'sf-save-button', 'sf-cancel-button', 8);
		expDebug::dPrint('$form formvalue= ' . print_r($form, true), 5);
		
		return $form;
	}catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_module_info_salesforce_integration_form', $ex);
		expertusErrorThrow($ex);
	}
}

function sfAddEditFormWithCancelAndSaveClose(&$form, $wrapperId, $saveButtonId, $cancelButtonId, $tabIndex) {
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
		$ajaxCallbackSave = 'addEditRepaintFormreuse';

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
		watchdog_exception('cdnAddEditFormWithCancelAndSaveClose', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Default validate handler for exp_sp_administration_group_addedit_form_validate
 */

function exp_sp_administration_module_info_salesforce_integration_form_validate($form, &$form_state){
	expDebug::dPrint(' Validate $form = ' . print_r($form, true) , 5);
	expDebug::dPrint(' Validate $form_state = ' . print_r($form_state['values'], true) , 5);
	expDebug::dPrint(' Validate $form_input' . print_r($form_state['input'], true) , 5);
	try{
		$key=$form_state['values']['key'];
		$validate_error=0;
		
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
			
			if (!empty($form_state['values']['consumer_secret_key'])) {
				if (strpos($form_state['values']['consumer_secret_key'], ' ') !== FALSE) {
					form_set_error('consumer_secret_key', t('ERR071').' '.t('LBL3100'));
					$form_state['esign_show_popup'] = false;
			      }
			}else{
				form_set_error('consumer_secret_key', t('LBL3100').t('ERR101'));
				$form_state['esign_show_popup'] = false;
			}
			
			if (!empty($form_state['values']['consumer_key'])) {
				if (strpos($form_state['values']['consumer_key'], ' ') !== FALSE) {
					form_set_error('consumer_key', t('ERR071').' '.t('LBL3101'));
					$form_state['esign_show_popup'] = false;
				}
			}else{
				form_set_error('consumer_key', t('LBL3101').t('ERR101'));
				$form_state['esign_show_popup'] = false;
			}
			
			/*if (!empty($form_state['values']['system_user'])) {
				if (strpos($form_state['values']['system_user'], ' ') !== FALSE) {
					form_set_error('system_user', t('ERR071').' '.t('LBL054'));
					$form_state['esign_show_popup'] = false;
				}
			}else{
				form_set_error('system_user', t('LBL054').t('ERR101'));
				$form_state['esign_show_popup'] = false;
			}
			
			if (!empty($form_state['values']['system_user_password'])) {
				if (strpos($form_state['values']['system_user_password'], ' ') !== FALSE) {
					form_set_error('system_user_password', t('ERR071').' '.t('LBL060'));
					$form_state['esign_show_popup'] = false;
				}
			}else{
				form_set_error('system_user_password', t('LBL060').t('ERR101'));
				$form_state['esign_show_popup'] = false;
			}*/
			
			//if (!empty($form_state['values']['expertusone_sf_id']) || $form_state['values']['expertusone_sf_id']==0) {
			if(trim($form_state['values']['expertusone_sf_id'])!="") {
				if (strpos($form_state['values']['expertusone_sf_id'], ' ') !== FALSE) {
					form_set_error('expertusone_sf_id', t('ERR071').' '.t('LBL3106'));
					$form_state['esign_show_popup'] = false;
				}
				if(!is_numeric($form_state['values']['expertusone_sf_id'])){
				//if (!preg_match('/^[1-9][0-9]*$/',$form_state['values']['expertusone_sf_id'])) {
					form_set_error('expertusone_sf_id', t('ERR097').' '.t('LBL3106'));
					$form_state['esign_show_popup'] = false;
				}
			}else{
				form_set_error('expertusone_sf_id', t('LBL3106').t('ERR101'));
				$form_state['esign_show_popup'] = false;
			}
			 
			
			for($i=0;$i<$key;$i++){	        	
		        	$tmp_org_name=trim($form_state['input']['org_id_'.$i]);
		        	$tmp_system_user=trim($form_state['input']['system_user_'.$i]);
		        	$tmp_system_user_pwd=trim($form_state['input']['system_user_pwd_'.$i]);   
					
					expDebug::dPrint('Temp System Username ' .$tmp_system_user, 4);
					expDebug::dPrint('Temp System Password' .$tmp_system_user_pwd, 4); 
		        	
		        	if($tmp_system_user==""){
		        	  form_set_error('system_user', t('LBL054').t('ERR101'));
				       $form_state['esign_show_popup'] = false;
		        	}
					if($tmp_system_user_pwd==""){
					   form_set_error('system_user_pwd', t('LBL060').t('ERR101'));
				       $form_state['esign_show_popup'] = false;
					}
		          	
			}
			
			if(!empty($form_state['values']['salesforce_debug'])) {
			if (strpos($form_state['values']['salesforce_debug'], ' ') !== FALSE) {
					form_set_error('salesforce_debug', t('ERR071').' '.t('LBL3105'));
					$form_state['esign_show_popup'] = false;
				}elseif (!validateEmail($form_state['values']['salesforce_debug'])) {
                                       form_set_error('salesforce_debug', t('ERR002'));
                                       $form_state['esign_show_popup']= false;
                }
		    }
						
			if ((!empty($form_state['values']['salesforce_namespace_prefix'])) && (strpos($form_state['values']['salesforce_namespace_prefix'], ' ') !== FALSE)) {
					form_set_error('salesforce_namespace_prefix', t('ERR071').' '.t('LBL3102'));
					$form_state['esign_show_popup'] = false;
				}
		
		}
		/* End validation */
		
		if ($form_state['triggering_element']['#name'] == 'hidden-save' || $form_state['triggering_element']['#ajax']['triggerEvent'] == 'hidden-save') {
			$form_state['hidden-save-submit'] = true;
		}

		$form_state['esign_validation_status'] = false; // checked to be false in exp_sp_administration.inc:addEditRepaintFormreuse() as a precondition to
		// showing popup.
		if (!($form_state['hidden-save-submit'])) {
			unset($_SESSION['esign_validate']); // for every button other than hidden-save, unset $_SESSION['esign_validate'].
		}

	}catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_module_info_salesforce_integration_form_validate', $ex);
		expertusErrorThrow($ex);
	}

}

/**
 * Submit handler for exp_sp_administration_group_addedit_form_submit
 */
function exp_sp_administration_module_info_salesforce_integration_form_submit($form, &$form_state){
	try{
		expDebug::dPrint('Submit $form_state[values] = ' . print_r($form_state['values'], true), 5);
		expDebug::dPrint('$form_state[input] = ' . print_r($form_state['input'], true), 5);
		
			$key=$form_state['values']['key'];
			expDebug::dPrint('$key$key$key$key$key = ' . print_r($key, true), 5);
			
		
		    $getpreval = getdefaultvariableval(); 
		    		    
		    $feed_trigger_option=$form_state['values']['salesforce_feed_trigger_option'];
		    variable_set('salesforce_feed_trigger_option',$feed_trigger_option);   
 
		    //$update_data_in_sf_option=$form_state['values']['salesforce_comp_enr_option'];
		    //variable_set('salesforce_comp_enr_option',$update_data_in_sf_option); 
		    
		    $sf_namespace_prefix=$form_state['values']['salesforce_namespace_prefix'];
		    variable_set('salesforce_namespace_prefix',$sf_namespace_prefix);
			
			$salesforce_api_url=$form_state['values']['salesforce_api_url'];
		    variable_set('salesforce_api_url',$salesforce_api_url); 			
		    
		    $salesforce_debug=$form_state['values']['salesforce_debug'];
		    variable_set('salesforce_debug',$salesforce_debug);
		    
		    $e1_salesforce_id=$form_state['values']['expertusone_sf_id'];
		    variable_set('expertusone_sf_id',$e1_salesforce_id);
		    
		    $consumer_secret_key = trim($form_state['values']['consumer_secret_key']);
			 variable_set('sf_consumer_secret_keys',$consumer_secret_key);
		    $consumer_key = trim($form_state['values']['consumer_key']);
			 variable_set('sf_consumer_keys',$consumer_key);
			
			// Adding Audit trail Entry
		     $salesforce_feed = variable_get('salesforce_feed_trigger_option',0);
		    $salesforce_namespace = variable_get('salesforce_namespace_prefix', '');
		    $salesforce_debug_notify = variable_get('salesforce_debug', '');
		    $expertus1_salesforce_id = variable_get('expertusone_sf_id', '');
			$sf_consumer_keys = variable_get('sf_consumer_keys', '');
			$sf_consumer_secret_keys = variable_get('sf_consumer_secret_keys', '');
			$sf_api_url = variable_get('salesforce_api_url', '');
			
			
		    
		    $userId = getIdOfLoggedInUser();
		    expDebug::dPrint('Logged in User Id = ' .$userId, 4);
		    	
		    $entityId = db_select('slt_profile_list_items','spli');
		    $entityId->addField('spli','id');
		    $entityId->condition('spli.code','cre_ste_mod_sf','=');
		    $entityId = $entityId->execute()->fetchField();
		    expDebug::dPrint('Module entityId = ' .print_r($entityId,true), 4);
		    
		    $variablefields = array('salesforce_namespace_prefix' => $salesforce_namespace, 'salesforce_feed_trigger_option' => $salesforce_feed, 'salesforce_debug' => $salesforce_debug_notify, 'expertusone_sf_id' => $expertus1_salesforce_id, 'sf_consumer_keys' => $sf_consumer_keys, 'sf_consumer_secret_keys' => $sf_consumer_secret_keys,'sf_api_url' => $sf_api_url);
		     expDebug::dPrint('Old values in Variable table = ' . print_r($getpreval,true), 4);
		    expDebug::dPrint('New value set in variable = ' . print_r($variablefields,true), 4);
		    
		    make_audit_trail_entries($variablefields, $userId, $getpreval ,$entityId, 'Settings Updated','cre_ste_mod_sf','exp_sp_administration_sitesetup','Salesforce Configuration',
		    		array('salesforce_namespace_prefix', 'salesforce_feed_trigger_option', 'salesforce_debug', 'expertusone_sf_id', 'sf_consumer_keys', 'sf_consumer_secret_keys','sf_api_url'));
		    		    
		    $sqlstmt= "SELECT id, consumer_secret_key, consumer_key, system_user, system_user_pwd, enr_syncup FROM slt_cstm_salesforce_settings";
		    $default = db_query( $sqlstmt )->fetchAll();
		    expDebug::dPrint('SF Configuration old Value= ' . print_r($default,true), 4);
			
				    
			     //$query_delete= db_delete('slt_cstm_salesforce_settings')->execute(); 
				 
				 for($i=0;$i< $key;$i++)
			     {	        	
			        	$org_id=$form_state['input']['org_id_'.$i];
			        	$system_user=trim($form_state['input']['system_user_'.$i]);
			        	$system_user_pwd=trim($form_state['input']['system_user_pwd_'.$i]);
			        	$enr_syncup=$form_state['input']['enr_syncup_'.$i];
			        	if(empty($enr_syncup) && $enr_syncup == ''){ //handled when new org detail is added, its value for $enr_syncup will be null, thus while inserting null into tinyint type it throws error.
			        		$enr_syncup = 0;
			        	}else{
			        		$enr_syncup = 1;
			        	}
						expDebug::dPrint('SF Org ID= ' . print_r($org_id,true), 4);
						//insert for only new org entry
						$sql = "SELECT exp_org_id FROM slt_cstm_salesforce_settings where exp_org_id in (".$org_id.")";
						$check = db_query( $sql )->fetchAll();
						expDebug::dPrintDBAPI('SF Org SQL '.$sql,5);
						expDebug::dPrint('SF Org Value= ' . print_r($check,true), 4);

							if(empty($check)){
								expDebug::dPrint('Inserting new org entry to salesforce settings.',4);
									$insquery = db_insert('slt_cstm_salesforce_settings');

									$fields = array(
									'exp_org_id' => $org_id,
									'consumer_secret_key' => $consumer_secret_key,
									'consumer_key' => $consumer_key,
									'system_user'=>$system_user,
									'system_user_pwd'=>$system_user_pwd,
									'enr_syncup'=>$enr_syncup,
									'created_on'=>date("Y-m-d H:i:s")
									);

									$fields = trimInputFields($fields);
									$insquery->fields($fields);
									$insquery = $insquery->execute();
							
							} else{
								expDebug::dPrint('Updating new org entry to salesforce settings.',4);
									
									$updquery = db_update('slt_cstm_salesforce_settings');  
          		
          	    					$updquery->condition('exp_org_id', $org_id,'=');
          	    					$updquery->fields(array(
          	                        'exp_org_id'=>$org_id,
          	                        'consumer_secret_key'=> $consumer_secret_key,
          	                        'consumer_key'=> $consumer_key,
                                    'system_user'=>$system_user,  
                                    'system_user_pwd'=> $system_user_pwd,  
                                    'enr_syncup'=>$enr_syncup,  
                                    'created_on'=>date("Y-m-d H:i:s")
          	                        ));	
          	    					$updquery->execute(); 
							}
						//Audit trail added for new entries added
						add_audit_trail_entry($userId, $entityId, 'cre_ste_mod_sf', 'exp_sp_administration_sitesetup', 'Salesforce Configuration', 'System User Added/Updated',
		                                     null, null, $system_user);
						add_audit_trail_entry($userId, $entityId, 'cre_ste_mod_sf', 'exp_sp_administration_sitesetup', 'Salesforce Configuration', 'System User Password Added/Updated',
		                                     null, null, $system_user_pwd);
						add_audit_trail_entry($userId, $entityId, 'cre_ste_mod_sf', 'exp_sp_administration_sitesetup', 'Salesforce Configuration', 'Enrollment User Sync-up',
		                                     null, null, $enr_syncup);
						   
				  }
				
			expertus_set_message(t('MSG689'));
			drupal_flush_all_caches();
			$form_state['rebuild'] = TRUE;
		
	} catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_module_info_salesforce_integration_form_submit', $ex);
		expertusErrorThrow($ex);
	}
}
?>