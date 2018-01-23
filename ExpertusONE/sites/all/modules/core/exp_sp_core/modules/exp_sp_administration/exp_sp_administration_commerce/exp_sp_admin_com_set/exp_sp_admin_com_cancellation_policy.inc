<?php
/**
 * Commerce Settings Cancellation Policy Admin inc file
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 18-Jul-2013
 */

/**
 * Renders cancellation policy admin screen in ctools modal popup.
 */
function render_cpolicy_admin_screen_in_ctools_modal($js) {
  try {
	  if ($js) {
	    ctools_include('modal');
      ctools_include('ajax');
      
      // Default modalTheme is ExpertusCToolsModalTheme (see exp_sp_core_extend_ctools_ajax.js:Drupal.CTools.Modal.show())
      $cancellationpolicy_style = array(
        'ctools-admin-cancellation-policy-addedit-scroll-wrapper' => array(
          'modalSize' => array(
            'width' => 650,
            'minHeight' => 160,
           ),
        )
      );
  
      drupal_add_js($cancellationpolicy_style, 'setting');
      
      $js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
      drupal_add_js(drupal_get_path('module', 'exp_sp_admin_com_set') .'/exp_sp_admin_com_cancellation_policy.js', $js_module_optional);

      $title = t('LBL1022'); //Cancellation Policy
      $output = theme('cancellation-policy-admin-screen');

      $commands[] = ctools_modal_command_display($title, $output);
      expDebug::dPrint(' $commands = ' . print_r($commands, true) , 4);
 
      print ajax_render($commands);
      exit;    
	  }
	  else {
	    throw new Exception('exp_sp_admin_com_cancellation_policy.inc: render_cpolicy_admin_screen_in_ctools_modal() : nojs');
	  }
  }
  catch (Exception $ex) {
    watchdog_exception('render_cpolicy_admin_screen_in_ctools_modal', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Fetches the list with details of cancellation policies from slt_drop_policy when $policyId is empty.
 * Otherwise, fetches the specific policy details.
 */
function getCancellationPolicyDetails($policyId = null) {
  try {  
	  // Select table(s)
	  $selectStmt = db_select('slt_drop_policy', 'sdp');
	  $selectStmt->leftJoin('slt_profile_list_items', 'spli', 'sdp.delivery_type = spli.code');
	  
	  // Select fields and/or add expressions
	  $fields = array('id', 'delivery_type', 'days', 'refund_percentage');
	  foreach($fields as $fieldName) {
	    $selectStmt->addField('sdp', $fieldName, $fieldName);
	  }
	  $selectStmt->addField('spli', 'name', 'delivery_type_name');

	  if (empty($policyId)) {
		  $selectStmt->orderBy('delivery_type_name', 'ASC');
		  $selectStmt->orderBy('sdp.days', 'DESC');
		  expDebug::dPrintDBAPI('fetch all $selectStmt', $selectStmt);
		  
		  $result = $selectStmt->execute()->fetchAll();
		  foreach ($result as $idx => $cancelPolicy) {
		  	$result[$idx]->delivery_type_name = t($result[$idx]->delivery_type_name); //translate name
		  }
		  expDebug::dPrint('fetch all $result = ' . print_r($result, true));
	  }
	  else {
	  	$selectStmt->condition('sdp.id', $policyId, '=');
	  	expDebug::dPrintDBAPI('fetch one $selectStmt', $selectStmt);
	  	$result = $selectStmt->execute()->fetch();
	    $result->delivery_type_name = t($result->delivery_type_name); //translate name
	  	expDebug::dPrint('fetch one $result = ' . print_r($result, true));
	  }
	  
	  return $result;
  }
  catch (Exception $ex) {
    watchdog_exception('getCancellationPolicyDetails', $ex);
    expertusErrorThrow($ex);
  }  
}

/**
 * Fetches list of cancellation policy entity types ordered by display name
 */
function getCancellationPolicyEntityTypes($indexByType = false) {
	try {
	  // Tables
    $select = db_select('slt_profile_list_items','spli');
  
    // Fields
    $select->addField('spli', 'code', 'code');
    $select->addField('spli', 'name', 'name');
    $select->addField('spli', 'attr1', 'attr1');
	
    // Conditions, etc.
    $select->condition('spli.is_active', 'Y', '=');
    $select->condition(db_or()->condition('spli.code', db_like('lrn_cls_dty_') . '%', 'LIKE')
                              ->condition('spli.code', 'cre_sys_obt_trp', '='));
    $select->orderBy('spli.name', 'ASC');
    expDebug::dPrintDBAPI('$select', $select);
    
    $result = $select->execute()->fetchAll();
    expDebug::dPrint('$result = ' . print_r($result, true));
    
    if ($indexByType) {
      $resultIndexByType = array();
    }
    foreach ($result as $idx => $cancelPolicyType) {
    	if ($indexByType) {
    		$cancelPolicyType->name = t($cancelPolicyType->name); //translate name
    		$resultIndexByType[$cancelPolicyType->code] = $cancelPolicyType;
    	}
    	else {
    		$result[$idx]->name = t($result[$idx]->name); //translate name
    	}
    }  //end foreach
    
    if ($indexByType) {
      $result = $resultIndexByType;
    }
    
    expDebug::dPrint('returning $result = ' . print_r($result, true), 5);
    return $result;
  }
  catch (Exception $ex) {
    watchdog_exception('getCancellationPolicyEntityTypes', $ex);
    expertusErrorThrow($ex);
  }  
}

/**
 * Drupal.ajax command to show cancellation policy admin form in qtip-like popup
 */
function render_admin_cpolicy_form_in_qtip($html) {
	try{
  return array(
    'command' => 'expAdminCPolicyFormInQtip',
    'html' => $html
  );
  }catch (Exception $ex) {
  	watchdog_exception('render_admin_cpolicy_form_in_qtip', $ex);
  	expertusErrorThrow($ex);
  }
}
  
/**
 * Renders cancellation policy addedit form in qtip popup
 */
function render_cpolicy_addedit_form_in_qtip_popup($entityType, $policyId) {
	expDebug::dPrint('$entityType = ' . print_r($entityType, true), 4);
	expDebug::dPrint('$policyId = ' . print_r($policyId, true), 4);
  try {
    $form_state = array(
      'no_redirect'  => TRUE,
      'build_info' => array('args' => array($entityType, $policyId))
    );
    $cpolicyAddEditFormId = 'exp_sp_administration_cpolicy_addedit_form';
    $builtCpolicyAddEditForm = drupal_build_form($cpolicyAddEditFormId, $form_state);
    $renderedCpolicyAddEditForm = drupal_render($builtCpolicyAddEditForm);
    
    // Render the qtip box html
    // Create popup id (this is same as specified in $qtipObj in exp_sp_administraton_commerce_cancellation_policy.tpl.php)
    if (empty($policyId)) {
    	$popupId = 'add-cancel-policy-qtip';
    }
    else {
    	$popupId = 'edit-cancel-policy-' . $policyId . '-qtip';
    }
    if (in_array($entityType, array('lrn_cls_dty_vcl', 'lrn_cls_dty_ilt'))) {
      $qtipWidth = '294px';
    }
    else {
      $qtipWidth = '228px';
    }
    $output = theme('drupalajax_qtip', array('popup_id' => $popupId, // The id to be given to the popup
                                             'popup_width' => $qtipWidth, // Popup width with unit
                                             'class' => 'admin-cancellation-policy-popup',  // Class(es) to be added to the popup for styling etc.
                                             'display_html' => $renderedCpolicyAddEditForm // The html to display inside the popup
                           ));
  
    $commands = array();
    $commands[] = render_admin_cpolicy_form_in_qtip($output);
    expDebug::dPrint('$commands = ' . print_r($commands, true), 4);
    print ajax_render($commands);
    exit;
  }
  catch (Exception $ex) {
    watchdog_exception('render_cpolicy_addedit_form_in_qtip_popup', $ex);
    expertusErrorThrow($ex);
  }
}


/**
 * Renders cancellation policy addedit form in qtip popup
 */
function exp_sp_administration_cpolicy_addedit_form($form, &$form_state, $entityType, $policyId) {
	try {
		expDebug::dPrint('$entityType = ' . print_r($entityType, true), 4);
    expDebug::dPrint('$policyId = ' . print_r($policyId, true), 4);
	
		if (!empty($policyId)) {
		  $policyDetails = getCancellationPolicyDetails($policyId);
		}
		
    // Form wrapper id
    $formWrapperId = 'cancel-policy-addedit-form';

    // New form
    $form = array();
    $form['#prefix'] = '<div id="' . $formWrapperId . '" class="' . $formWrapperId . '-wrapper">';
    $form['#suffix'] = '</div>';
    $form['#attributes']['autocomplete'] = 'off';
    $form['#tree'] = TRUE; // Show full path of values (hierarchical values)

    // Add percentage to deduct field
    $defaultValue = empty($policyDetails)? '' : $policyDetails->refund_percentage;
    $form['cpolicy-deduct-percentage'] = array(
      '#id' => 'cpolicy-deduct-percentage',
      '#type' => 'textfield',
      '#title' => t('LBL1124') . '&nbsp;' . '%', //Deduct
      '#title_display' => 'invisible',
      '#default_value' => $defaultValue,
      '#attributes' => array('autocomplete' => 'off', 'tabindex' => 1, 'class' => array('cancel-policy-addedit-textfield')), 
      '#size' => 6,
      '#maxlength' => 6,
    );
    
	  if ($entityType == 'lrn_cls_dty_ilt' || $entityType == 'lrn_cls_dty_vcl') {
      $defaultValue = '';
      $defaultValue = empty($policyDetails)? '' : $policyDetails->days;
      $form['cpolicy-days-before'] = array(
        '#id' => 'cpolicy-days-before',
        '#type' => 'textfield',
        '#title' => ucfirst(t('LBL605')), //Days
        '#title_display' => 'invisible',
        '#default_value' => $defaultValue,
        '#attributes' => array('autocomplete' => 'off', 'tabindex' => 2, 'class' => array('cancel-policy-addedit-textfield')), 
        '#size' => 6,
        '#maxlength' => 6,
      );
    }
    
    $form['cpolicy-delivery-type'] = array(
      '#type' => 'value',
      '#value' => $entityType
    );
    
    $form['cpolicy-id'] = array(
      '#type' => 'value',
      '#value' => $policyId
    );

    // Add action buttons and close the form
    expAdminCancelPolicyAddEditFormClose($form, $formWrapperId, 3);
        
    expDebug::dPrint('$form = ' . print_r($form, true), 5);
    return $form;
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_cpolicy_addedit_form', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Adds close and save buttons to the cancellation policy addedit form and closes the form
 */
function expAdminCancelPolicyAddEditFormClose(&$form, $wrapperId, $tabIndex) {
  try {
    // Button ids
    global $theme_key;
    $saveButtonId = 'addedit-cpolicy-save-button';
    $cancelButtonId = 'addedit-cpolicy-cancel-button';
    
    // Add the Cancel button
    // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
    // each time the form is re-built.
    $cancelAttributes['class'] = array('addedit-edit-' . $cancelButtonId);
      
    // Using this class to applying bg color to the save button
    $cancelAttributes['class'][] = 'admin-action-button-middle-bg';
      
    // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
    $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber';
    // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
    $cancelAttributes['data-wrapperid'] = array($wrapperId);
      
    // A common class for all addedit form cancel button, for styling to appear as a link.
    $cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';
    
    $cancelAttributes['onclick'][] = '$("#root-admin").data("narrowsearch").removeActiveQtip(); return false;';
    $cancelAttributes['tabindex'][] = $tabIndex + 1;
  	$prfixPgLeft = '';
	  $suffixPgRight = '';
	  if($theme_key == 'expertusoneV2'){
	  	$prfixPgLeft   = '<div class="white-btn-bg-left"></div>';
	  	$suffixPgRight = '<div class="white-btn-bg-right"></div>';
	  	$cancelAttributes['class'][] = 'white-btn-bg-middle';
	  }  
    $form[$cancelButtonId] = array(
      '#type' => 'button',
      '#prefix' => '<div style="clear:both;"></div>' .
                   '<div class="addedit-form-cancel-and-save-actions-row">' .
                     '<div class="addedit-form-cancel-container-actions">'. $prfixPgLeft,
      '#suffix' => $suffixPgRight,
      '#value' => t('LBL123'), //Close
      '#name' => $cancelButtonId,
      '#attributes' => $cancelAttributes
    );
      
    // Add the Save button
    $ajaxCallbackSave = 'expAdminCancelPolicyRepaintAddEditForm';
      
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

    $form[$saveButtonId] = array(
      '#type' => 'submit',
      '#prefix' => '<div class="admin-save-button-container">' .
                     '<div class="admin-save-button-left-bg"></div>',
      '#suffix' =>   '<div class="admin-save-button-right-bg"></div>' .
                   '</div>' .
                   '<div class="clearBoth"></div>' .
                 '</div>' . // see #prefix of close button above
                 '<div class="clearBoth"></div>' .
               '</div>', // see #prefix of close button above
      '#value' => t('LBL141'), // Save
      '#name' => $saveButtonId,
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
  }
  catch (Exception $ex) {
    watchdog_exception('expAdminCancelPolicyAddEditFormClose', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Themes cancellation policy add/edit form
 */
function theme_exp_sp_administration_cpolicy_addedit_form($variables) {
	try {
		expDebug::dPrint('$variables = ' . print_r($variables, true), 5);
	
	  $form = $variables['form'];
	  
		$labelClass = 'narrow';
		if ($form['cpolicy-delivery-type']['#value'] == 'lrn_cls_dty_ilt' || $form['cpolicy-delivery-type']['#value'] == 'lrn_cls_dty_vcl') {
			$labelClass = 'wide';
		}
		
		$output  = '<table class="cancel-policy-addedit-form-table">';
		$output .=   '<tr>';
		$output .=     '<td class="' . $labelClass . '">';
		$output .=       t('LBL1124') . ':' . '<span class="addedit-mandatory">*</span>'; //Deduct
		$output .=     '</td>';
		$output .=     '<td>';
		$output .=       drupal_render($form['cpolicy-deduct-percentage']);
		$output .=     '</td>';
		
		if ($form['cpolicy-delivery-type']['#value'] == 'lrn_cls_dty_ilt' || $form['cpolicy-delivery-type']['#value'] == 'lrn_cls_dty_vcl') {
		  $output .=     '<td>';
		  $output .=       '%';
		  $output .=     '</td>';
		}
		else {
	    $output .=     '<td>';
	    $output .=       '%' . '&nbsp;' . t('LBL1126'); //always
	    $output .=     '</td>'; 
		}
		$output .=   '</tr>';
		
	  
		if ($form['cpolicy-delivery-type']['#value'] == 'lrn_cls_dty_ilt' || $form['cpolicy-delivery-type']['#value'] == 'lrn_cls_dty_vcl') {
	    $output .=   '<tr>';
	    $output .=     '<td class="' . $labelClass . '">';
			$output .=       t('LBL1127') . ':'  . '<span class="addedit-mandatory">*</span>'; //When Training Starts In
	    $output .=     '</td>';
	    $output .=     '<td>';
			$output .=       drupal_render($form['cpolicy-days-before']);
	    $output .=     '</td>';
	    $output .=     '<td>';
	    $output .=       t('LBL605'); //days
	    $output .=     '</td>'; 
	    $output .=   '</tr>';
		}
		$output  .= '</table>';
		
	  $output .= drupal_render_children($form);
	
	  expDebug::dPrint('$output = ' . print_r($output, true), 5);
	  return $output;
	}
  catch (Exception $ex) {
    watchdog_exception('theme_exp_sp_administration_cpolicy_addedit_form', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Validates cancellation policy add/edit form
 */
function exp_sp_administration_cpolicy_addedit_form_validate($form, &$form_state) {
	try {
		expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 5);

		$formFieldId = 'cpolicy-deduct-percentage';
    $form_state['values'][$formFieldId] = trim($form_state['values'][$formFieldId]);
		cancelPolicyAddEditFormValidateField($formFieldId, $form_state['values'][$formFieldId], $form[$formFieldId]['#title']);

		if ($form_state['values']['cpolicy-delivery-type'] == 'lrn_cls_dty_ilt' || $form_state['values']['cpolicy-delivery-type'] == 'lrn_cls_dty_vcl') {
      $formFieldId = 'cpolicy-days-before';
      $form_state['values'][$formFieldId] = trim($form_state['values'][$formFieldId]);
      cancelPolicyAddEditFormValidateField($formFieldId, $form_state['values'][$formFieldId], $form[$formFieldId]['#title']);
	  }

	  validateDistinctCancellationPolicy($form, $form_state);	 
	  checkCancellationPolicyifexists($form, $form_state); 	 	  
	}
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_cpolicy_addedit_form_validate', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Validates cancellation policy form field. Assumes the field is numeric and is mandatory.
 */
function cancelPolicyAddEditFormValidateField($formFieldId, $formFieldValue, $formFieldTitle) {
	try {
		expDebug::dPrint('$formFieldValue = ' . print_r($formFieldValue, true));
	  if ($formFieldValue == '') {
	  	expDebug::dPrint('field is missing ' . $formFieldId, 5);
	    form_set_error($formFieldId, $formFieldTitle . t('ERR101')); // is required.
	  }
	  else if (!is_numeric($formFieldValue)) {
	    expDebug::dPrint('field is non-numeric ' . $formFieldId, 5);
	    form_set_error($formFieldId, $formFieldTitle . ' ' . t('ERR200')); //is not a number.
	  }
	  else if ($formFieldValue < 0) {
	    expDebug::dPrint('field is less than 0 ' . $formFieldId, 5);
	    form_set_error($formFieldId, $formFieldTitle . ' ' . t('ERR213')); //cannot be negative.
	  }
	  else if ($formFieldId == 'cpolicy-deduct-percentage' && $formFieldValue > 100) {
	    expDebug::dPrint('field is not within 0 - 100 ' . $formFieldId, 5);
	    form_set_error($formFieldId, $formFieldTitle . ' ' . t('ERR214')); //not within 0 - 100.   
	  }
	  else if ($formFieldId == 'cpolicy-days-before' && $formFieldValue != floor($formFieldValue)) {
	    expDebug::dPrint('field is not an integer ' . $formFieldId, 5);
	    form_set_error($formFieldId, $formFieldTitle . ' ' . t('ERR200')); //is not a number.
	  }
    /* else if ($formFieldId == 'cpolicy-days-before'  && $formFieldValue == '0' ) {
      expDebug::dPrint('field is 0 ' . $formFieldId, 5);
      form_set_error($formFieldId, $formFieldTitle . ' ' . t('ERR216')); //cannot be zero.
    } */
	}
  catch (Exception $ex) {
    watchdog_exception('cancelPolicyAddEditFormValidateField', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Checks to confirm cancellation policy does not already exist.
 */
function validateDistinctCancellationPolicy($form, $form_state) {
	try {
		if ($form_state['values']['cpolicy-deduct-percentage'] != '' &&
		      (($form_state['values']['cpolicy-delivery-type'] != 'lrn_cls_dty_ilt' &&
		           $form_state['values']['cpolicy-delivery-type'] != 'lrn_cls_dty_vcl') ||
		        $form_state['values']['cpolicy-days-before'] != '')) {
			$selectStmt = db_select('slt_drop_policy', 'sdp');
			$selectStmt->addExpression('count(1)', 'count');
			$selectStmt->condition('sdp.delivery_type', $form_state['values']['cpolicy-delivery-type'], '=');
			$selectStmt->condition('sdp.refund_percentage', $form_state['values']['cpolicy-deduct-percentage'], '=');
	    if ($form_state['values']['cpolicy-delivery-type'] == 'lrn_cls_dty_ilt' || $form_state['values']['cpolicy-delivery-type'] == 'lrn_cls_dty_vcl') {
	    	$selectStmt->condition('sdp.days', $form_state['values']['cpolicy-days-before'], '=');
	    }
	    
	    if (!empty($form_state['values']['cpolicy-id'])) {
	    	$selectStmt->condition('sdp.id', $form_state['values']['cpolicy-id'], '<>');
	    }
	    expDebug::dPrintDBAPI('$selectStmt', $selectStmt);
	    $numPolicies = $selectStmt->execute()->fetchField();
	    expDebug::dPrint('$numPolicies = ' . $numPolicies);
	    
	    if ($numPolicies > 0) {
	      expDebug::dPrint('Cancellation policy already exists.', 5);
	      form_set_error('cpolicy-deduct-percentage', t('ERR217')); //Cancellation policy already exists.
	      if ($form_state['values']['cpolicy-delivery-type'] == 'lrn_cls_dty_ilt' ||
	              $form_state['values']['cpolicy-delivery-type'] == 'lrn_cls_dty_vcl') {
	        form_set_error('cpolicy-days-before', ' ');
	      }
	    }
		}
	}
  catch (Exception $ex) {
    watchdog_exception('validateDistinctCancellationPolicy', $ex);
    expertusErrorThrow($ex);
  }	
}

/**
 * Saves new or edited cancellation policy to db
 */
function exp_sp_administration_cpolicy_addedit_form_submit($form, &$form_state) {
  try {
  	expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 5);
  	
  	$loggedInUserId = getIdOfLoggedInUser();
  	if (empty($form_state['values']['cpolicy-id'])) {
  		addNewCancellationPolicy($form, $form_state, $loggedInUserId);
  	}
  	else {
  		updateCancellationPolicy($form, $form_state, $loggedInUserId);
  	}

  	$form_state['dbupdated'] = TRUE; // Refresh password policy list
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_cpolicy_addedit_form_submit', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Adds a new cancellation policy record to the DB
 */
function addNewCancellationPolicy($form, &$form_state, $loggedInUserId) {
  $txn = db_transaction();
  try {
  	$auditFieldsList = array('delivery_type', 'refund_percentage', 'days');
  	
    $insertFields = array(
      'delivery_type' => $form_state['values']['cpolicy-delivery-type'],
      'refund_percentage' => $form_state['values']['cpolicy-deduct-percentage'],
      'created_by' => $loggedInUserId,
      'created_on' => now(),
    	'updated_by' => $loggedInUserId,
    	'updated_on' => now()
    );
    if ($form_state['values']['cpolicy-delivery-type'] == 'lrn_cls_dty_ilt' || $form_state['values']['cpolicy-delivery-type'] == 'lrn_cls_dty_vcl') {
    	$insertFields['days'] = $form_state['values']['cpolicy-days-before'];
    }
    else {
    	$insertFields['days'] = 0;
    }
    $insertQuery = db_insert('slt_drop_policy');
    $insertQuery->fields($insertFields);
    expDebug::dPrintDBAPI('$insertQuery', $insertQuery);
    $cancelPolicyId = $insertQuery->execute();
    expDebug::dPrint('$cancelPolicyId = ' . $cancelPolicyId);

    make_audit_trail_entries($insertFields, $loggedInUserId, null, $cancelPolicyId, 'Inserted', 'cancellation_policy',
                                  'exp_sp_administration_commerce_setting', 'addNewCancellationPolicy', $auditFieldsList);
    expertus_set_message(t('MSG600').'.'); //Cancellation policy added.
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('addNewCancellationPolicy', $ex);
    expertusErrorThrow($ex);
  }
}


function checkCancellationPolicyifexists($form, $form_state) {
	try {
		if ($form_state['values']['cpolicy-deduct-percentage'] != '' &&
		      (($form_state['values']['cpolicy-delivery-type'] != 'lrn_cls_dty_ilt' &&
		           $form_state['values']['cpolicy-delivery-type'] != 'lrn_cls_dty_vcl') ||
		        $form_state['values']['cpolicy-days-before'] != '')) {
			$selectStmt = db_select('slt_drop_policy', 'sdp');
			$selectStmt->addExpression('count(1)', 'count');
			$selectStmt->condition('sdp.delivery_type', $form_state['values']['cpolicy-delivery-type'], '=');	
		
	    if ($form_state['values']['cpolicy-delivery-type'] == 'lrn_cls_dty_ilt' || $form_state['values']['cpolicy-delivery-type'] == 'lrn_cls_dty_vcl') {
	    	$queryCondition = db_or();	    	
	    	$queryCondition->condition('sdp.days', $form_state['values']['cpolicy-days-before'], '=');
	    	$queryCondition->condition('sdp.refund_percentage', $form_state['values']['cpolicy-deduct-percentage'], '=');	    	
	    	$selectStmt->condition($queryCondition);
	    }
	    
		 if (!empty($form_state['values']['cpolicy-id'])) {
	    	$selectStmt->condition('sdp.id', $form_state['values']['cpolicy-id'], '<>');
	    }
	    
	    expDebug::dPrintDBAPI('$selectStmt', $selectStmt);
	    $numifexist = $selectStmt->execute()->fetchField();
	    expDebug::dPrint('$numifexist = ' . $numifexist);
	    
	    if ($numifexist > 0) {
	      expDebug::dPrint('There is already a cancellation policy set for the number of day entered..', 5);
	      form_set_error('cpolicy-days-before', t('MSG767')); //There is already a cancellation policy set for the number of day entered.	     
	       }
		}
	}
  catch (Exception $ex) {
    watchdog_exception('checkCancellationPolicyifexits', $ex);
    expertusErrorThrow($ex);
  }	
	}

/**
 * Updates a cancellation policy record in db
 */
function updateCancellationPolicy($form, &$form_state, $loggedInUserId){
  $txn = db_transaction();
  try {
    $auditFieldsList = array('refund_percentage');
    $prevValues = getCancellationPolicyDetails($form_state['values']['cpolicy-id']);
    $prevValuesForAuditTrail[] = $prevValues;
    
    $updateFields = array(
      'refund_percentage' => $form_state['values']['cpolicy-deduct-percentage'],
      'updated_by' => $loggedInUserId,
      'updated_on' => now()
    );

    if ($form_state['values']['cpolicy-delivery-type'] == 'lrn_cls_dty_ilt' || $form_state['values']['cpolicy-delivery-type'] == 'lrn_cls_dty_vcl') {
      $updateFields['days'] = $form_state['values']['cpolicy-days-before'];
      $auditFieldsList[] = 'days';
    }
    
    $updateQuery = db_update('slt_drop_policy')
      ->fields($updateFields)
      ->condition('id', $form_state['values']['cpolicy-id']);
    expDebug::dPrintDBAPI('$updateQuery', $updateQuery);
    $updateQuery->execute();

    expDebug::dPrint('$auditFieldsList = ' . print_r($auditFieldsList, true), 5);
    expDebug::dPrint('$prevValuesForAuditTrail = ' . print_r($prevValuesForAuditTrail, true), 5);
    make_audit_trail_entries($updateFields, $loggedInUserId, $prevValuesForAuditTrail, $form_state['values']['cpolicy-id'], 'Updated',
                                      'cancellation_policy', 'exp_sp_administration_commerce_setting', 'updateCancellationPolicy', $auditFieldsList);
    expertus_set_message(t('MSG601').'.'); //Cancellation policy updated.
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('updateCancellationPolicy', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Ajax callback for cancellation policy add/edit form submit button
 */
function expAdminCancelPolicyRepaintAddEditForm($form, $form_state) {
  try {
  	expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 5);
  	
  	$commands = array();
  	if (isset($form_state['dbupdated'])) { //submit was successful - refresh password policy list
      $renderedCancelPolicyAdminScreen = theme('cancellation-policy-admin-screen');
      $commands[] = ajax_command_replace('#commerce-cancellation-policy-screen-wrapper', $renderedCancelPolicyAdminScreen);
      $commands[] = ajax_command_html('#commerce-cancellation-policy-screen-wrapper #show_expertus_message', theme('status_messages'));
      $commands[] = exp_ctools_modal_adjust();
  	}
  	else { //validation failed - repaint form in qtip showing the validation messages
	    $commands[] = ajax_command_replace(NULL, drupal_render($form));	
	    $statusMsgHTML = theme('status_messages');
	    if (!empty($statusMsgHTML)) {
	      // Show the messages
	      $commands[] = ajax_command_html('.cancel-policy-popup-wrapper #show_expertus_message', $statusMsgHTML);
	    }
  	}
    
    expDebug::dPrint('$commands = ' . print_r($commands, true), 5);
    return array('#type' => 'ajax', '#commands' => $commands);
  }
  catch (Exception $ex) {
    watchdog_exception('expAdminCancelPolicyRepaintAddEditForm', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Generates commands to refresh cancellation policies list when a policy is deleted by admin
 */
function exp_sp_administration_delete_cancellation_policy($cancelPolicyId) {
  try {
  	$loggedInUserId = getIdOfLoggedInUser();
  	deleteCancellationPolicy($cancelPolicyId, $loggedInUserId);
  	
    $commands = array();
    $renderedCancelPolicyAdminScreen = theme('cancellation-policy-admin-screen');
    $commands[] = ajax_command_replace('#commerce-cancellation-policy-screen-wrapper', $renderedCancelPolicyAdminScreen);
    $commands[] = ajax_command_html('#commerce-cancellation-policy-screen-wrapper #show_expertus_message', theme('status_messages'));
    $commands[] = exp_ctools_modal_adjust();
    expDebug::dPrint('$commands = ' . print_r($commands, true), 5);
    
    print ajax_render($commands);
    exit;
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_delete_cancellation_policy', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Deletes a cancellation policy
 */
function deleteCancellationPolicy($cancelPolicyId, $loggedInUserId) {
	$txn = db_transaction();
  try {
      
      expDebug::dPrint('inside camncellatrion');
      
    $auditFieldsList = array('delivery_type', 'refund_percentage');
    $prevValues = getCancellationPolicyDetails($cancelPolicyId);
    $prevValuesForAuditTrail[] = $prevValues;

    if ($prevValues->delivery_type == 'lrn_cls_dty_ilt' || $prevValues->delivery_type == 'lrn_cls_dty_vcl') {
      $auditFieldsList[] = 'days';
    }

    
    $deleteQuery = db_delete('slt_drop_policy')
      ->condition('id', $cancelPolicyId);
    expDebug::dPrintDBAPI('$deleteQuery', $deleteQuery);
    $deleteQuery->execute();

	//Make delete log entries
    $fields = array(
            'table_name' => 'slt_drop_policy',
            'entity_id' => $cancelPolicyId,
            'entity_type' => 'cancellation_policy',
            'module_name' => 'exp_sp_admin_com_cancellation_policy',
            'functionality_name' => 'deleteCancellationPolicy',
            'api_name' => null,
            'deleted_on' => now(),
            'deleted_by' => $loggedInUserId,
    );
    deleted_log_entry($fields);
	expDebug::dPrint('slt_drop_policy table log');
	
    // Make audit trail entries
    expDebug::dPrint('$auditFieldsList = ' . print_r($auditFieldsList, true), 5);
    expDebug::dPrint('$prevValuesForAuditTrail = ' . print_r($prevValuesForAuditTrail, true), 5);
    make_audit_trail_entries(null, $loggedInUserId, $prevValuesForAuditTrail, $cancelPolicyId, 'Deleted',
                               'cancellation_policy', 'exp_sp_administration_commerce_setting', 'deleteCancellationPolicy',
                                 $auditFieldsList);
    expertus_set_message(t('LBL1022').' '.strtolower(t('LBL1225')).'.'); //Cancellation policy deleted.
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('deleteCancellationPolicy', $ex);
    expertusErrorThrow($ex);
  }
}

?>