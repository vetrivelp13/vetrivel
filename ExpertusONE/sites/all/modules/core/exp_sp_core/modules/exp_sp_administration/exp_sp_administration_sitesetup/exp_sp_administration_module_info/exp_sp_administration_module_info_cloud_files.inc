<?php 
function cloud_files_AddEdit($js = NULL){
	try {
		expDebug::dPrint(' CDN $js = ' . print_r($form, true) , 5);
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
    
		$cdn_style = array(
				'ctools-admin-cdn-style' => array(
						'modalSize' => array(
								'width' => ($theme_key == "expertusoneV2") ? $defaultWidth : 615,
								'minHeight' => 20,
						),
				)
		);

		drupal_add_js($cdn_style, 'setting');

		if ($js) {
			// Prepare the arguments list to be passed to $formState['build_info']['args'] of each form
			$argsList = array();
				
			// Prepare list of forms to be displayed in the modal
			$formIdList = array(
					array('id' => 'exp_sp_administration_module_info_cloud_files_form',
							'title' => t('CDN – Cloud files')),
			);

			exp_ctools_modal_multi_ajaxonly_forms_display($formIdList, $argsList);
		}
		else {
			throw new Exception('CloudfilesAddEdit');
		}
	}catch (Exception $ex) {
		watchdog_exception('CloudfilesAddEdit', $ex);
		expertusErrorThrow($ex);
	}
}

function get_auth_url_form_id($auth_url){
	try{
		require_once 'sites/all/libraries/php-opencloud/vendor/autoload.php';		
		expDebug::dPrint('$auth_url = '.$auth_url, 5);
		expDebug::dPrint('$auth_url_other = '.$auth_url_other, 5);
		if(module_exists('libraries')){
			$info = libraries_load('php-opencloud');			
			$authurl=array();
			if($info['loaded']){
				$authurl = array(
						'us' => array(
								'name' => t('United States'),
								'url' => OpenCloud\Rackspace::US_IDENTITY_ENDPOINT,),
						'gb' => array(
								'name' => t('United Kingdom'),
								'url' => OpenCloud\Rackspace::UK_IDENTITY_ENDPOINT,),
				);
			}			
		}
		expDebug::dPrint(' CDN get_auth_url_form_id info - $authurl = ' . print_r($authurl, true) , 5);
			
		$tmp_auth_url_us=$authurl['us']['url'];
		$tmp_auth_url_uk=$authurl['gb']['url'];
		
		if($auth_url==$tmp_auth_url_us){
			$ret_short_form_id="US";
		}else if($auth_url==$tmp_auth_url_uk){
			$ret_short_form_id="UK";
		}else{
			$ret_short_form_id="other";
		} 
		expDebug::dPrint('$ret_short_form_id = '.$ret_short_form_id, 5);

		return $ret_short_form_id;


	}catch (Exception $ex) {
		watchdog_exception('get_auth_url_form_id', $ex);
		expDebug::dPrint('$exception = '.print_r($ex->getMessage(),true), 5);
		expertusErrorThrow($ex);
	}
}

function get_auth_url($auth_url,$auth_url_other){
	try{
		require_once 'sites/all/libraries/php-opencloud/vendor/autoload.php';		
		expDebug::dPrint('$auth_url = '.$auth_url, 5);
		expDebug::dPrint('$auth_url_other = '.$auth_url_other, 5);
		if(module_exists('libraries')){
			$info = libraries_load('php-opencloud');
			$authurl=array();
			if($info['loaded']){
				$authurl = array(
						'us' => array(
								'name' => t('United States'),
								'url' => OpenCloud\Rackspace::US_IDENTITY_ENDPOINT,),
						'gb' => array(
								'name' => t('United Kingdom'),
								'url' => OpenCloud\Rackspace::UK_IDENTITY_ENDPOINT,),
				);
			}
		}
		expDebug::dPrint(' CDN get_auth_url info - $authurl = ' . print_r($authurl, true) , 5);
		 
		if($auth_url=="US"){
			$tmp_auth_url=$authurl['us']['url'];
		}elseif($auth_url=="UK"){
			$tmp_auth_url=$authurl['gb']['url'];
		}else{
			$tmp_auth_url = $auth_url_other;
		}
		
		expDebug::dPrint('$tmp_auth_url = '.$tmp_auth_url, 5);
		
		return $tmp_auth_url;
		
		
	}catch (Exception $ex) {
		watchdog_exception('get_auth_url', $ex);
		expDebug::dPrint('$exception = '.print_r($ex->getMessage(),true), 5);
		expertusErrorThrow($ex);
	}
}


function check_valid_cdn_username_and_api($user_name,$api_key,$auth_url){

	try{
		
		require_once 'sites/all/libraries/php-opencloud/vendor/autoload.php';
			
		$client = new OpenCloud\Rackspace($auth_url, array(
				'username' => $user_name,
				'apiKey' => $api_key,
		));
		
		$res_cli_aut=$client->authenticate();  
		expDebug::dPrint('$res_cli_aut = '.print_r($res_cli_aut,true), 5); 
		return "1"; 
		
	}catch(Exception $ex){ 
		watchdog_exception('check_valid_cdn_username_and_api', $ex); 
		expDebug::dPrint('$exception = '.print_r($ex->getMessage(),true), 5);
		expDebug::dPrint('Given username and api key is invalid.', 5);
		return "";
	}
}


function get_container_domain($user_name,$api_key,$auth_url,$region,$container){
	try{
	    require_once 'sites/all/libraries/php-opencloud/vendor/autoload.php';
	    if(module_exists('libraries')){
		    $info = libraries_load('php-opencloud');
		    if ($info['loaded']){
		    $client = new OpenCloud\Rackspace($auth_url, array(
					'username' => $user_name,
					'apiKey' => $api_key,
			));
		        $client->authenticate();
				$object_store_service = $client->objectStoreService('cloudFiles', $region);
	        	$container = $object_store_service->getContainer($container);
	        	$domain = str_replace(array('http://', 'https://'), '', $container->getCdn()->getCdnUri());
			}
	    }
	    return $domain;
	}catch(Exception $ex){
		watchdog_exception('get_container_domain', $ex);
	}
}


function check_container_in_region($user_name,$api_key,$container_name,$region,$auth_url){  	
		try {
			
			expDebug::dPrint(' $user_name = ' . $user_name , 5);
			expDebug::dPrint(' $api_key = ' . $api_key , 5);
			expDebug::dPrint(' $container_name = ' . $container_name , 5);
			expDebug::dPrint(' $region = ' . $region , 5);
			expDebug::dPrint(' $auth_url = ' . $auth_url , 5);
			
			
			require_once 'sites/all/libraries/php-opencloud/vendor/autoload.php';
			
			$container_options="";
			$cdn_container = $container_name;
			$cdn_container = empty($cdn_container)? 'persistent' : $cdn_container;
			$client = new OpenCloud\Rackspace($auth_url, array(
				'username' => $user_name,
				'apiKey' => $api_key,
			));
			$client->authenticate();
			$object_store_service = $client->objectStoreService('cloudFiles', $region);
			$container_list = $object_store_service->listContainers();
			$container_options = array();
			while ($container = $container_list->Next()) {
				$container_options[$container->name] = $container->name;
			}  
			
			expDebug::dPrint(' $container_options = ' . print_r($container_options, true) , 5);
			
			return $container_options;
		}
		catch (Exception $ex) {
			watchdog_exception('check_container_in_region', $ex);
			expDebug::dPrint(' No container available for specificed region', 5);
			return "";
			//expertusErrorThrow($ex);
		}  
}

function exp_sp_administration_module_info_cloud_files_form($form, &$form_state){
	try {
		include_once $_SERVER["DOCUMENT_ROOT"]. "/sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc"; 
				
		expDebug::dPrint(' CDN $form = ' . print_r($form, true) , 5);
		//$formWrapperId = 'cdn-basic-addedit-form';
		$form = newAddEditFormWithCancelAndSave('cdnserver-basic-addedit-form');
		$form['cdn-basic-edit-form-start'] = array(
				'#type'   => 'markup',
				'#markup' => '<div id="cdn-basic-addedit-form" class="cdn-edit-rows">',
		);  
		
		$rackspace_cloud_username = variable_get('rackspace_cloud_username');
		$rackspace_cloud_api_key = variable_get('rackspace_cloud_api_key');
		$rackspace_cloud_container = variable_get('rackspace_cloud_container');
		$rackspace_cloud_region = variable_get('rackspace_cloud_region');
		$rackspace_cloud_region_other = variable_get('rackspace_cloud_region_other');
		$rackspace_cloud_auth_url = variable_get('rackspace_cloud_auth_url');
		$rackspace_cloud_auth_url_other = variable_get('rackspace_cloud_auth_url_other'); 
		
		expDebug::dPrint('Existing $rackspace_cloud_username = '.$rackspace_cloud_username, 5);
		expDebug::dPrint('Existing $rackspace_cloud_api_key = '.$rackspace_cloud_api_key, 5);
		expDebug::dPrint('Existing $rackspace_cloud_container = '.$rackspace_cloud_container, 5);
		expDebug::dPrint('Existing $rackspace_cloud_region = '.$rackspace_cloud_region, 5);
		expDebug::dPrint('Existing $rackspace_cloud_region_other = '.$rackspace_cloud_region_other, 5);
		expDebug::dPrint('Existing $rackspace_cloud_auth_url = '.$rackspace_cloud_auth_url, 5);
		expDebug::dPrint('Existing $rackspace_cloud_auth_url_other = '.$rackspace_cloud_auth_url_other, 5); 

		if(($rackspace_cloud_region != "DFW") && ($rackspace_cloud_region != "ORD") && ($rackspace_cloud_region != "LON")){
			$rackspace_cloud_region = "other";
		}else {
			$rackspace_cloud_region = variable_get('rackspace_cloud_region');
		}
		
		$default_form_id_auth_url = get_auth_url_form_id($rackspace_cloud_auth_url);
		
		expDebug::dPrint(' $default_form_id_auth_url = ' . print_r($default_form_id_auth_url, true) , 5);
		
		addTwoColumnRowToAddEditForm($form, 'url_region'); 
		addRadioSetToAddEditForm($form, 'rackspace_cloud_auth_url', t('LBL3000'), array('US'=>'United States','UK'=>'United Kingdom','other'=>'Other') ,$default_form_id_auth_url,1,true); // Authentication URL
		addRadioSetToAddEditForm($form, 'rackspace_cloud_region', t('LBL3001'),array('DFW' => 'Dallas','ORD' => 'Chicago','LON' => 'London','other' => 'Other'), $rackspace_cloud_region, 2, true); // Region
		endAddEditTwoColumnRow($form); 
		
		$rackspace_cloud_region = variable_get('rackspace_cloud_region');
		
		addTwoColumnRowToAddEditForm($form, 'other_url_region');
		addTextFieldToAddEditForm($form, 'rackspace_cloud_auth_url_other','', $rackspace_cloud_auth_url_other, 3, false); 
		addTextFieldToAddEditForm($form, 'rackspace_cloud_region_other', '', $rackspace_cloud_region_other, 3, false);
		endAddEditTwoColumnRow($form); 
		 
		addTwoColumnRowToAddEditForm($form, 'username_api_key');
		
		addTextFieldToAddEditForm($form, 'rackspace_cloud_username', t('LBL054'), variable_get('rackspace_cloud_username', ''), 3, true); // username
				
		addPasswordFieldToAddEditForm($form, 'rackspace_cloud_api_key', t('LBL3002'), variable_get('rackspace_cloud_api_key', ''), 4, true); // api key
		
		endAddEditTwoColumnRow($form);
		addTwoColumnRowToAddEditForm($form, 'container_domain');
		
		
		if($rackspace_cloud_auth_url_other!=''){
			$cdn_auth_url=$rackspace_cloud_auth_url_other;
		}else{
			$cdn_auth_url=$rackspace_cloud_auth_url;
		}
		
		if($rackspace_cloud_region_other!=''){
			$cdn_region=$rackspace_cloud_region_other;
		}else{
			$cdn_region=$rackspace_cloud_region;
		}
		
		$container_option_arr=check_container_in_region($rackspace_cloud_username,$rackspace_cloud_api_key,$rackspace_cloud_container,$cdn_region,$cdn_auth_url);
		
		expDebug::dPrint(' $container_option_arr = ' . print_r($container_option_arr, true) , 5);
		// For Container field
		if(isset($container_option_arr) && !empty($container_option_arr) && $container_option_arr!=""){
			addDropDownToAddEditForm($form, 'rackspace_cloud_container', t('LBL3003'),$container_option_arr,variable_get('rackspace_cloud_container', ''), 5, true);
		}else{
			addTextFieldToAddEditForm($form, 'rackspace_cloud_container', t('LBL3003'), variable_get('rackspace_cloud_container', ''),5, true);
		}
				
		addTextFieldToAddEditForm($form, 'rackspace_cloud_cdn_domain', t('LBL3004'), variable_get('rackspace_cloud_cdn_domain'), 6, true);
		endAddEditTwoColumnRow($form);
		
		cdnAddEditFormWithCancelAndSaveClose($form, 'cdnserver-basic-addedit-form', 'cdn-save-button', 'cdn-cancel-button', 7);
		expDebug::dPrint('$form formvalue= ' . print_r($form, true), 5);
    
		$form['cdn-basic-edit-form-end'] = array(
				'#type'   => 'markup',
				'#markup' => '</div>',
		);
		
		return $form;
	}catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_module_info_cloud_files_form', $ex);
		expertusErrorThrow($ex);
	}
}

function cdnAddEditFormWithCancelAndSaveClose(&$form, $wrapperId, $saveButtonId, $cancelButtonId, $tabIndex) {
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
		watchdog_exception('cdnAddEditFormWithCancelAndSaveClose', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Default validate handler for exp_sp_administration_group_addedit_form_validate
 */

function exp_sp_administration_module_info_cloud_files_form_validate($form, &$form_state){
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
			
		    if(empty($form_state['values']['rackspace_cloud_auth_url'])){
			     form_set_error('rackspace_cloud_auth_url', t('ERR300'));
			     $form_state['esign_show_popup'] = false;
			} 
			
			if($form_state['values']['rackspace_cloud_auth_url']=="other" && (empty($form_state['values']['rackspace_cloud_auth_url_other']) || trim($form_state['values']['rackspace_cloud_auth_url_other'])=="")){
				form_set_error('rackspace_cloud_auth_url_other', t('ERR302'));
				$form_state['esign_show_popup'] = false;
			}
			
			/*if($form_state['values']['rackspace_cloud_auth_url']!="other" && trim($form_state['values']['rackspace_cloud_auth_url_other'])!=""){
				
				form_set_error('rackspace_cloud_auth_url_other', t('ERR301'));
				$form_state['esign_show_popup'] = false;
			}*/
			
			if(empty($form_state['values']['rackspace_cloud_region'])){
				form_set_error('rackspace_cloud_region', t('ERR303'));
				$form_state['esign_show_popup'] = false;
			}
				
			if($form_state['values']['rackspace_cloud_region']=="other" && (empty($form_state['values']['rackspace_cloud_region_other']) || trim($form_state['values']['rackspace_cloud_region_other'])=="")){
				form_set_error('rackspace_cloud_region_other', t('ERR305'));
				$form_state['esign_show_popup'] = false;
			}
			
			/*if($form_state['values']['rackspace_cloud_region']!="other" && trim($form_state['values']['rackspace_cloud_region_other'])!=""){
				form_set_error('rackspace_cloud_region_other', t('ERR304'));
				$form_state['esign_show_popup'] = false;
			}*/
			
			//addEditCheckMandatoryFields($form, $form_state, array('rackspace_cloud_username','rackspace_cloud_api_key','rackspace_cloud_container','rackspace_cloud_cdn_domain'));
			if (!empty($form_state['values']['rackspace_cloud_username'])) {
				if (strpos($form_state['values']['rackspace_cloud_username'], ' ') !== FALSE) {
					form_set_error('username', t('ERR015'));
					$form_state['esign_show_popup'] = false;
				}else if(preg_match("/^[a-zA-Z]+$/",$form_state['values']['rackspace_cloud_username']) == FALSE) {
					form_set_error('username', t('ERR068'));
					$form_state['esign_show_popup'] = false;
				}
			}else{
				form_set_error('rackspace_cloud_username', t('Username is required'));
				$form_state['esign_show_popup'] = false;
			}
			
			$rackspace_cloud_api_key_val = variable_get('rackspace_cloud_api_key');
			expDebug::dPrint('Value for existing API Key = ' . print_r($rackspace_cloud_api_key_val, true), 5);
			if (!empty($form_state['values']['rackspace_cloud_api_key'])) {
				if (strpos($form_state['values']['rackspace_cloud_api_key'], ' ') !== FALSE) {
					form_set_error('api_key', t('ERR309'));
					$form_state['esign_show_popup'] = false;
				}
			}elseif(empty($form_state['values']['rackspace_cloud_api_key']) && empty($rackspace_cloud_api_key_val)){
				form_set_error('rackspace_cloud_api_key', t('Api Key is required'));
				$form_state['esign_show_popup'] = false;
			}else{
				$form_state['values']['rackspace_cloud_api_key'] = $rackspace_cloud_api_key_val;
				expDebug::dPrint('Value for API Key = ' . print_r($form_state['values']['rackspace_cloud_api_key'], true), 5);
			}

			if (!empty($form_state['values']['rackspace_cloud_container'])) {
				if (strpos($form_state['values']['rackspace_cloud_container'], ' ') !== FALSE) {
					form_set_error('container', t('ERR310'));
					$form_state['esign_show_popup'] = false;
				}
			}else{
				form_set_error('rackspace_cloud_container', t('Container is required'));
				$form_state['esign_show_popup'] = false;
			}
				
			if (!empty($form_state['values']['rackspace_cloud_cdn_domain'])) {
				if (strpos($form_state['values']['rackspace_cloud_cdn_domain'], ' ') !== FALSE) {
					form_set_error('domain', t('ERR308'));
					$form_state['esign_show_popup'] = false;
				}
			}else{
				form_set_error('rackspace_cloud_cdn_domain', t('Domain is required'));
				$form_state['esign_show_popup'] = false;
			}
			
			$check_container_option_arr="";
			//Validate container according to selected region
			if(trim($form_state['values']['rackspace_cloud_username'])!="" && trim($form_state['values']['rackspace_cloud_api_key'])!="" && trim($form_state['values']['rackspace_cloud_container'])!="" && trim($form_state['values']['rackspace_cloud_region'])!=""){
			  
			// Check user name and password whether valid or not
			    $auth_url_api = get_auth_url($form_state['values']['rackspace_cloud_auth_url'],$form_state['values']['rackspace_cloud_auth_url_other']);
				$ret_auth_val = check_valid_cdn_username_and_api($form_state['values']['rackspace_cloud_username'],$form_state['values']['rackspace_cloud_api_key'],$auth_url_api);
				expDebug::dPrint('$ret_auth_val = ' . print_r($ret_auth_val, true), 5);
				
				if($ret_auth_val=="1"){
				   
					// Get auth url
					$auth_url_con = get_auth_url($form_state['values']['rackspace_cloud_auth_url'],$form_state['values']['rackspace_cloud_auth_url_other']);
					
					expDebug::dPrint('$auth_url_con = ' . print_r($auth_url_con, true), 5);
					
					//Get region
					if(empty($form_state['values']['rackspace_cloud_region']) && $form_state['values']['rackspace_cloud_region_other']!=''){
						$cdn_region=$form_state['values']['rackspace_cloud_region_other'];
					}else if($form_state['values']['rackspace_cloud_region'] == 'other'){
						$cdn_region=$form_state['values']['rackspace_cloud_region_other'];
						expDebug::dPrint('$cdn_region else if= ' . print_r($cdn_region, true), 5);
					}else {
						$cdn_region=$form_state['values']['rackspace_cloud_region'];
					}
					
					expDebug::dPrint('$cdn_region = ' . print_r($cdn_region, true), 5);
					
					// check container
					$check_container_option_arr = check_container_in_region($form_state['values']['rackspace_cloud_username'],$form_state['values']['rackspace_cloud_api_key'],$form_state['values']['rackspace_cloud_container'],$cdn_region,$auth_url_con);
					$i=1;
					foreach($check_container_option_arr as $key=>$containername){
						if($i == 1){
							$container = (string) $check_container_option_arr[$containername];
						}
					}
					expDebug::dPrint(' Before $form_state = ' . print_r($form_state['values']['rackspace_cloud_container'], true), 5);
					expDebug::dPrint(' Before $container = ' . print_r($container, true), 5);
					expDebug::dPrint(' Before $check_container_option_arr = ' . print_r($check_container_option_arr, true), 5);
					if(isset($check_container_option_arr) && !empty($check_container_option_arr) && $check_container_option_arr!=""){
						expDebug::dPrint(' After $check_container_option_arr = ' . print_r($check_container_option_arr, true), 5);
						if(in_array($form_state['values']['rackspace_cloud_container'],$check_container_option_arr)){
							//Setting the container value here itself. Since in changing the region will not able to select the respective container in the form
							variable_set('rackspace_cloud_container', $form_state['values']['rackspace_cloud_container']);
							variable_set('rackspace_cloud_cdn_domain', $form_state['values']['rackspace_cloud_cdn_domain']);
						}else{
							$domainvalue = get_container_domain($form_state['values']['rackspace_cloud_username'],$form_state['values']['rackspace_cloud_api_key'],$auth_url_api,$cdn_region,$container);
							
							variable_set('rackspace_cloud_container', $container);
							variable_set('rackspace_cloud_cdn_domain', $domainvalue);
						}
					}else{
						form_set_error('rackspace_cloud_container', t('ERR307'));
						$form_state['esign_show_popup'] = false;
					}
					
				}else{
					form_set_error('rackspace_cloud_username', t('ERR306'));
					$form_state['esign_show_popup'] = false;
				}
			  
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
		watchdog_exception('exp_sp_administration_module_info_cloud_files_form_validate', $ex);
		expertusErrorThrow($ex);
	}

}

/**
 * Submit handler for exp_sp_administration_group_addedit_form_submit
 */
function exp_sp_administration_module_info_cloud_files_form_submit($form, &$form_state){
	try{
		expDebug::dPrint('Submit $form_state[values] = ' . print_r($form_state['values'], true), 5);
		expDebug::dPrint('$form_state[input] = ' . print_r($form_state['input'], true), 5);
		//Get Auth URL
			$rackspace_cloud_auth_url = get_auth_url($form_state['values']['rackspace_cloud_auth_url'],$form_state['values']['rackspace_cloud_auth_url_other']);
			expDebug::dPrint(' $rackspace_cloud_auth_url = ' . print_r($rackspace_cloud_auth_url, true) , 5);
			if($form_state['values']['rackspace_cloud_auth_url']=="other"){
				variable_set('rackspace_cloud_auth_url', $rackspace_cloud_auth_url);
				variable_set('rackspace_cloud_auth_url_other', $form_state['values']['rackspace_cloud_auth_url_other']);
			}else {
				variable_set('rackspace_cloud_auth_url', $rackspace_cloud_auth_url);
				variable_set('rackspace_cloud_auth_url_other', '');
			}	
			
		
		if($form_state['values']['rackspace_cloud_region']=="other"){
			variable_set('rackspace_cloud_region', $form_state['values']['rackspace_cloud_region_other']);
			variable_set('rackspace_cloud_region_other', $form_state['values']['rackspace_cloud_region_other']);
		}else{
			variable_set('rackspace_cloud_region', $form_state['values']['rackspace_cloud_region']);
			variable_set('rackspace_cloud_region_other', '');
		}  
		
		variable_set('rackspace_cloud_username', $form_state['values']['rackspace_cloud_username']);
		variable_set('rackspace_cloud_api_key', $form_state['values']['rackspace_cloud_api_key']);
		//variable_set('rackspace_cloud_container', $form_state['values']['rackspace_cloud_container']);
		//variable_set('rackspace_cloud_cdn_domain', $form_state['values']['rackspace_cloud_cdn_domain']);
			
		expertus_set_message(t('MSG689'));
		drupal_flush_all_caches();
		$form_state['rebuild'] = TRUE;

	} catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_module_info_cloud_files_form_submit', $ex);
		expertusErrorThrow($ex);
	}
}
?>