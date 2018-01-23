<?php 
function getcheckSettingDisplay($optionValue,$entityId){  
  try {

      $form_state = array(
   		'no_redirect'  => TRUE,
    	'build_info'   => array('args' => array('ajax')),
   		'entity_id'    => $entityId,
        'option_value'    => $optionValue,
      );
            
      $formId                 = 'exp_sp_my_check_setting_add_form';
      $builtForm              = drupal_build_form($formId, $form_state);
      $renderedTabContentHTML = drupal_render($builtForm);
      $drupalSettings         = getDrupalSettings();
    
      $checkSettingResponse = array(
        'drupal_settings'     => $drupalSettings,
        'render_content_main' => $renderedTabContentHTML,
      ); 
     
      echo drupal_json_output($checkSettingResponse);
      exit;
       
  } catch (Exception $ex) {
    watchdog_exception('checksetting', $ex);
    expertusErrorThrow($ex);
  }
  
}

function exp_sp_my_check_setting_add_form ($form, &$form_state){
try{
  global $theme_key;
  $prfixPgLeft='';$suffixPgRight='';$alignBottom='';$alignBottomCLose='';
  if($theme_key == 'expertusoneV2'){
  	$prfixPgLeft   = '<div class="white-btn-bg-left"></div>';
  	$suffixPgRight = '<div class="white-btn-bg-right"></div>';
  	$cancelAttributes['class'][] = 'white-btn-bg-middle';
  	$alignBottom      = '<div class="cancel-align-bottom">';
  	$alignBottomCLose = '</div>';
  } 
  $form['check_heading_row_start'] = array(
      '#type' => 'markup',     
      '#markup' => '<div id="payment-check-details"><table cellpadding="0" cellspacing="0" class="admin-check-details-wrapper" border="0">',
	);
	
  $titleAttributes['class'] = array();
  $titleAttributes['class'][] = 'input-field-set ';
  $titleAttributes['class'][] = 'uc-check-mailing-name ';
  
  $form['uc_check_mailing_name'] = array(
        '#type' => 'textfield',
        '#title' => t('LBL153').':<span class="addedit-mandatory">*</span><span class="greyouttext">('.t('LBL1073').')</span>',
        '#prefix' => '<tr> <td colspan="2">',
        '#suffix' => '</td></tr>',  	
        '#default_value' => variable_get('uc_check_mailing_name'), 
        '#size' => 15,
        '#maxlength' => 50,
        '#attributes' => $titleAttributes,
    );
  $titleAttributes['class'][] = 'uc-check-mailing-street1';
  $form['uc_check_mailing_street1'] = array(
        '#type' => 'textfield',
        '#title' => t('LBL064').':<span class="addedit-mandatory">*</span>',
        '#prefix' => '<tr> <td colspan="2">',
        '#suffix' => '</td></tr>',  	
        '#default_value' => variable_get('uc_check_mailing_street1'), 
        '#size' => 15,
        '#maxlength' => 50,
        '#attributes' => $titleAttributes,
    );
  $titleAttributes['class'][] = 'uc-check-mailing-street2';   
  $form['uc_check_mailing_street2'] = array(
        '#type' => 'textfield',
        '#title' => t('LBL065').':',
        '#prefix' => '<tr> <td colspan="2">',
        '#suffix' => '</td></tr>',  	
        '#default_value' => variable_get('uc_check_mailing_street2'), 
        '#size' => 15,
        '#maxlength' => 50,
        '#attributes' => $titleAttributes,
    );        

  $form['check_details1']['ck_second_row_start'] = array(
      '#type' => 'markup',     
      '#markup' => '<tr>',	
  );
  $defaultAffectorValue = empty($form_state['values']['country']) ? variable_get('uc_check_mailing_country',True):$form_state['values']['country'];
  $defaultAffectedValue = empty($form_state['values']['state']) ? variable_get('uc_check_mailing_zone',True) : $form_state['values']['state'];// getDefaultAddEditFormFieldValue('state', $form_state, $userDetails);
  if ($form_state['triggering_element']['#name'] == 'country') {
    $defaultAffectedValue = '';
    
  }
  
 $affector = array(
    'id' => 'country',
    'title' => t('LBL039'),
    'fetch_options' => getUcCountries,
    'default_value' => $defaultAffectorValue,
    'tab_index' => 10,
    'mandatory' => true,
  );

  $affected = array(
    'id' => 'state',
    'wrapper_id' => 'adddedit-state-wrapper',
    'title' => t('LBL152'),
    'fetch_options' => getUCStatesForCountry,
    'default_value' => $defaultAffectedValue,
    'tab_index' => 11,
    'mandatory' => true,
  );  
  addAjaxDependantDropDownPairToUCFormCountry($form, $affector, $affected,'payment-check-details');
  addAjaxDependantDropDownPairToUCFormState($form, $affector, $affected);
    
  $form['check_details2']['ck_second_row_end'] = array(
        '#type' => 'markup',     
        '#markup' => '</tr>',	
    );
  unset($titleAttributes);
  $titleAttributes['class'][] = 'input-field-set ';
  $titleAttributes['class'][] = 'uc-check-mailing-city'; 
  $form['check_details3']['ck_third_row_start'] = array(
        '#type' => 'markup',     
        '#markup' => '<tr>',	
    );
    
  $form['uc_check_mailing_city'] = array(
        '#type' => 'textfield',
        '#title' => t('LBL066').':<span class="addedit-mandatory">*</span>',
        '#prefix' => '<td class="admin-addedit-class-check-name">',
        '#suffix' => '</td>',  	
        '#default_value' =>variable_get('uc_check_mailing_city'),  
        '#size' => 15,
        '#maxlength' => 50,
        '#attributes' => $titleAttributes,
    );
  $titleAttributes['class'][] = 'uc-check-mailing-postal-code';   
  $form['uc_check_mailing_postal_code'] = array(
        '#type' => 'textfield',
        '#title' => t('LBL562').':<span class="addedit-mandatory">*</span>',
        '#prefix' => '<td class="admin-addedit-class-check-name postal-code-col">',
        '#suffix' => '</td>',  	
        '#default_value' =>variable_get('uc_check_mailing_postal_code'),  
        '#size' => 15,
        '#maxlength' => 50,
        '#attributes' => $titleAttributes,
    );
    
  $form['check_details3']['ck_third_row_end'] = array(
        '#type' => 'markup',     
        '#markup' => '</tr>',	
    );
  $form[$formName]['check_heading_row_end'] = array(
      '#type' => 'markup',     
      '#markup' => '</table>',  	
  );
  

  $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-action-button-middle-bg white-btn-bg-middle';
  $cancelAttributes['data-wrapperid'] = array($wrapperId);
  $cancelAttributes['onclick'][]         = "closeQtip('','$wrapperId');  return false;";
  $form['cancel_payment_setting'] = array(
      '#type' => 'submit',
      '#value' => t('LBL123'),
      '#title' => t('LBL123'),
      '#prefix' =>  '<div class="addedit-form-cancel-container-actions admin-session-save-button admin-save-button-container">'.$alignBottom.$prfixPgLeft,
      '#suffix' => $suffixPgRight.$alignBottomCLose,
      '#name' => 'Cancel Payment Check',
      '#attributes' => $cancelAttributes,
  );
    
  $checkSaveAttributes['class'][]= 'admin-save-button-middle-bg';
  $checkSaveAttributes['class'][] = 'addedit-form-expertusone-throbber';
  $checkSaveAttributes['data-wrapperid'] = array('payment-check-details');
  $form['save_payment_check_setting'] = array(
    '#type' => 'submit',
    '#value' => t('LBL141'),
    '#name' => 'payment-check-details',
    '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
    '#suffix' => '<div class="admin-save-button-right-bg"></div></div></div></div>',
    '#title' => t('save'),
    '#attributes' => $checkSaveAttributes,
    '#ajax' => array(
                'wrapper' => 'payment-check-details',
                'callback' => 'payment_set_check_callback',
                'method' => 'replace',
                'effect' => 'none',
                'event'=>'click',
                'keypress' => true,
                'progress' => array(
                  'type' => 'throbber',
                  'message' => ''
                ),
        )   
      );      
 return $form;
  }catch (Exception $ex) {
    watchdog_exception('exp_sp_my_check_setting_add_form', $ex);
    expertusErrorThrow($ex);
  }
  
  
}

function payment_set_check_callback(&$form, &$form_state){ 
	try{
  $min_form_state = array(
   'ajax' => TRUE, 
   're_render' => TRUE, 
   'no_redirect' => TRUE, 
   'build_info' => array('args' => array('ajax')) 
  );
  
  $form_state = $min_form_state;
  
  $formId = 'exp_sp_my_check_setting_add_form';
  
  $commands = array();
  $commands[] = ajax_command_replace('#payment-check-details', drupal_render($form));
  
  $statusMsgHTML = theme('status_messages');

  expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true) , 4);
  
  if (!empty($statusMsgHTML)) {
  	// Show the messages
    $commands[] = ajax_command_html('#bubble-face-table #show_expertus_message', $statusMsgHTML);
  }
  
 return array(
        '#type' => 'ajax',
        '#commands' => $commands
  );
 }catch (Exception $ex) {
 	watchdog_exception('payment_set_check_callback', $ex);
 	expertusErrorThrow($ex);
 }
}

function exp_sp_my_check_setting_add_form_validate($form, &$form_state) {
	try{
  if ($form_state['triggering_element']['#name'] == 'country') {
      // Since country has changed, we need to unset any selection for state made by the user
      unset($form_state['values']['state']);
      unset($form_state['input']['state']);
      $form_state['storage']['affected_dropdown'] = 'state';
      return;
  }
  if(empty($form_state['values']['uc_check_mailing_name'])){
     form_set_error('uc_check_mailing_name',t('LBL153').t('ERR101')); 
  }
 if(empty($form_state['values']['uc_check_mailing_street1'])){
     form_set_error('uc_check_mailing_street1',t('LBL064').t('ERR101')); 
  }
 if(empty($form_state['values']['country'])){
     form_set_error('country',t('LBL039').t('ERR101')); 
  }
 if(empty($form_state['values']['state'])){
     form_set_error('state',t('LBL152').t('ERR101')); 
  }
 if(empty($form_state['values']['uc_check_mailing_city'])){
     form_set_error('uc_check_mailing_city',t('LBL066').t('ERR101')); 
  }
 if(empty($form_state['values']['uc_check_mailing_postal_code'])){
     form_set_error('uc_check_mailing_postal_code',t('LBL562').t('ERR101')); 
  }
  }catch (Exception $ex) {
  	watchdog_exception('exp_sp_my_check_setting_add_form_validate', $ex);
  	expertusErrorThrow($ex);
  }
}  
function exp_sp_my_check_setting_add_form_submit($form, &$form_state) {
	try{
  expDebug::dPrint('exp_sp_my_check_setting_add_form_submit : '.print_r($form_state['values'],true) , 5);
  
  $country = '';
  $state   = '';
  if(!empty($form_state['values']['country']) && (!empty($form_state['values']['state']))){
      $country = $form_state['values']['country'];
      $state   = $form_state['values']['state'];
      
  }
   $getValues =  array(
				'uc_check_mailing_name'           => empty($form_state['values']['uc_check_mailing_name'])? '':$form_state['values']['uc_check_mailing_name'],
				'uc_check_mailing_country'        => $country,
                'uc_check_mailing_zone'           => $state, 
                'uc_check_mailing_street1'        => empty($form_state['values']['uc_check_mailing_street1']) ? '':$form_state['values']['uc_check_mailing_street1'],
				'uc_check_mailing_street2'        => empty($form_state['values']['uc_check_mailing_street2']) ? '':$form_state['values']['uc_check_mailing_street2'],
                'uc_check_mailing_city'           => empty($form_state['values']['uc_check_mailing_city']) ? '':$form_state['values']['uc_check_mailing_city'],
				'uc_check_mailing_postal_code'    => empty($form_state['values']['uc_check_mailing_postal_code'])? '':$form_state['values']['uc_check_mailing_postal_code'],
		    
		);
    foreach ($getValues as $key => $value) {
       // Updating the Field value in the variable set
         variable_set($key,$value);
    }
  expertus_set_message(t('MSG650')); //Check settings saved successfully.
  $form_state['rebuild']  = TRUE;
  }catch (Exception $ex) {
  	watchdog_exception('exp_sp_my_check_setting_add_form_submit', $ex);
  	expertusErrorThrow($ex);
  }
}

function getUcCountries() {
  try{
  $select = db_select('uc_countries', 'countries');
  $select->addField('countries','country_id');
  $select->addField('countries','country_name');
  $select->orderBy('countries.country_name');
  return $select->execute()->fetchAllKeyed();
  }catch (Exception $ex) {
    watchdog_exception('getUcCountries', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * getUCStatesForCountry($zoneId) - Fetch states list of a country
 */
function getUCStatesForCountry($countryCode){
  try{	
  $select = db_select('uc_zones', 'zone');
  $select->addField('zone', 'zone_id');
  $select->addField('zone', 'zone_name');
  $select->condition('zone.zone_country_id', $countryCode, '=');
  $select->orderBy('zone.zone_name');
  $select->groupBy('zone.zone_name');
  
  expDebug::dPrintDBAPI(' $select OBJ = ' ,$select);
  
  return $select->execute()->fetchAllKeyed();
  }catch (Exception $ex) {
    watchdog_exception('getUCStatesForCountry', $ex);
    expertusErrorThrow($ex);
  }
}


function addAjaxDependantDropDownPairToUCFormCountry(&$form, $affector = array(), $affected = array(), $wrapperId = '') {
	try{
  global $theme_key;
  $affectorAttributes = array();
  $affectorAttributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  $affectorAttributes['class'][] = 'addedit-edit-dropdown';
  $affectorAttributes['class'][] = 'addedit-dependant-dropdown-affector';
  $affectorAttributes['onchange'][] = 'textfieldTitleChange(this)';
  
  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $affectorAttributes['class'][] = 'addedit-form-expertusone-throbber';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  
  $wrapperId = empty($wrapperId) ? 'user-basic-addedit-form' : $wrapperId;
  $affectorAttributes['data-wrapperid'] = array($wrapperId);
  

  if (isset($affector['tab_index']) && $affector['tab_index'] > 0) {
    $affectorAttributes['tabindex'] = $affector['tab_index'];
  }

  $affectorOptions = $affector['fetch_options']();
  if ($affector['mandatory']) {
    $nullOption[''] = t('LBL674');
  } else {
    $nullOption[''] = t('LBL674');
  }
  
  $finalAffectorOptions = $nullOption + $affectorOptions;
  expDebug::dPrint(' $$affectorOptions = ' . print_r($affectorOptions, true) , 4);
  expDebug::dPrint(' $$finalAffectorOptions = ' . print_r($finalAffectorOptions, true) , 4);

  if (empty($form['addedit_tmp_storage']['two_column'])) {
    $prefix = addEditGetPrefix('FIELD', $affector['title'], $affector['mandatory']);
    $suffix = addEditGetSuffix('FIELD');
  }
  else {
    if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
        $prefix = addEditGetPrefix('TWOCOL_FIRSTCOL_FIELD', $affector['title'], $affector['mandatory']);
        $suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
    } else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
        $prefix = addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $affector['title'], $affector['mandatory']);
        $suffix = addEditGetSuffix('TWOCOL_SECONDCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
    } else {
      throw new Exception('exp_sp_admin_com_payment_check_setting.inc : addTextFieldToAddEditForm() : Attempt to add a third field to a two column row.');
    }
  }  
  if($theme_key == 'expertusoneV2') {
    $fieldPrefix = '<div class="expertus-dropdown-bg dropdown-width"><div class="expertus-dropdown-icon ">';
    $fieldSuffix = '</div></div>';
  }else {
    $fieldPrefix = '';
    $fieldSuffix = '';
  } 
  $form[$affector['id']] = array(
    '#type' => 'select',
    '#prefix' => "<td>".$prefix . $fieldPrefix,
    '#suffix' => $fieldSuffix . $suffix."</td>",
    '#title' => $affector['title'],
    '#title_display' => 'invisible',
    //'#field_prefix'  => '<div class="expertus-dropdown-bg"><div class="expertus-dropdown-icon">',
    //'#field_suffix'  => '</div></div>',
    '#options'=> $finalAffectorOptions,
    '#default_value' => $affector['default_value'],
    '#attributes' => $affectorAttributes,
    '#ajax' => array(
      'wrapper' => $affected['wrapper_id'],
      'callback' => addAjaxDependantDropDownPairToAddEditForm_callback,
      'method' => 'replace',
      'effect' => 'none', // 'fade',
      //'event'=>'click',
      'keypress' => true,
      'progress' => array(
        'type' => 'throbber',
        'message' => ''
      ),
    )
  );
  $affectId = $affector['id'];
  $replacedId = str_replace('_', '-', $affectId);
	$form['load_select_drop_down_' . $affectId] = array (
		'#prefix' => '<img src=\'themes/seven/images/buttons.png\' width="0" height="0" onload="textfieldTitleChangeField(\'edit-'.$replacedId.'\');" />',
	);
	}catch (Exception $ex) {
		watchdog_exception('addAjaxDependantDropDownPairToUCFormCountry', $ex);
		expertusErrorThrow($ex);
	}
}

function addAjaxDependantDropDownPairToUCFormState(&$form, $affector = array(), $affected = array()) {
	try{
  global $theme_key;
  $affectedAttributes = array();
  $affectedAttributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  $affectedAttributes['class'] = array('addedit-edit-' . $affected['id'],'uc-mailing-country'); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
  // Add classes for adding style
  $affectedAttributes['class'][] = 'addedit-edit-dropdown';
  $affectedAttributes['class'][] = 'addedit-dependant-dropdown-affected';
  $affectedAttributes['onchange'][] = 'textfieldTitleChange(this)';
  
  if (isset($affected['tab_index']) && $affected['tab_index'] > 0) {
    $affectedAttributes['tabindex'] = $affected['tab_index'];
  }
  
  $affectedOptions = $affected['fetch_options']($affector['default_value']);
  
  if (empty($affector['default_value'])) {
    //$nullOption[''] = t('Select' . ' ' . $affector['title']);
    $nullOption[''] = t('LBL674');
  }
  else if ($affected['mandatory']) {
    $nullOption[''] = t('LBL674');
  }
  else if (empty($affectedOptions)) {
    $nullOption[''] = t('No' . ' ' . $affected['title']);
  }
  else {
    $nullOption[''] = t('LBL674');
  }
  
  //$finalAffectedOptions = array_merge($nullOption, $affectedOptions);
  $finalAffectedOptions = $nullOption + $affectedOptions;
  expDebug::dPrint(' $finalAffectedOptions = ' . print_r($finalAffectedOptions, true) , 4);

  if (empty($form['addedit_tmp_storage']['two_column'])) {
    $prefix = addEditGetPrefix('FIELD', $affected['title'], $affected['mandatory'], ':', $affected['wrapper_id']);
    $suffix = addEditGetSuffix('FIELD');
  }
  else {
    if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
        $prefix = addEditGetPrefix('TWOCOL_FIRSTCOL_FIELD', $affected['title'], $affected['mandatory'], ':', $affected['wrapper_id']);
        $suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
    } else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
        $prefix = addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $affected['title'], $affected['mandatory'], ':', $affected['wrapper_id']);
        $suffix = addEditGetSuffix('TWOCOL_SECONDCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
    } else {
      throw new Exception('exp_sp_administration.inc : addTextFieldToAddEditForm() : Attempt to add a third field to a two column row.');
    }
  }  
	if($theme_key == 'expertusoneV2') {
    $fieldPrefix = '<div class="expertus-dropdown-bg dropdown-width"><div class="expertus-dropdown-icon ">';
    $fieldSuffix = '</div></div>';
  }else {
    $fieldPrefix = '';
    $fieldSuffix = '';
  }
  $form[$affected['id']] = array(
    '#type' => 'select',
    '#prefix' => "<td>".$prefix . $fieldPrefix,
    '#suffix' => $fieldSuffix. $suffix."</td>",
    //'#field_prefix'  => '<div class="expertus-dropdown-bg"><div class="expertus-dropdown-icon">',
    //'#field_suffix'  => '</div></div>',
    '#title' => $affected['title'],
    '#title_display' => 'invisible',
    '#options' => $finalAffectedOptions,
    '#default_value' => $affected['default_value'],
    '#attributes' => $affectedAttributes,
  );
  $affectId = $affected['id'];
  $replacedId = str_replace('_', '-', $affectId);
	$form['load_select_drop_down_' . $affectId] = array (
		'#prefix' => '<img src=\'themes/seven/images/buttons.png\' width="0" height="0" onload="textfieldTitleChangeField(\'edit-'.$replacedId.'\');" />',
	);
	}catch (Exception $ex) {
		watchdog_exception('addAjaxDependantDropDownPairToUCFormState', $ex);
		expertusErrorThrow($ex);
	}
}
 /*print variable_get('uc_payment_method_check_label')."<br>";
 print variable_get('uc_check_mailing_name')."<br>";
 print variable_get('uc_check_mailing_country')."<br>";
 print variable_get('uc_check_mailing_zone')."<br>";
 print variable_get('uc_check_mailing_street1')."<br>";
 print variable_get('uc_check_mailing_street2')."<br>";
 print variable_get('uc_check_mailing_city')."<br>";
 print variable_get('uc_check_mailing_postal_code');*/
?>