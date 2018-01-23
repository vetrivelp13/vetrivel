<?php


function display_drop_policy_form($js = NULL,$entityId) {
try {	
 if ($js) {
    ctools_include('modal');
    ctools_include('ajax');
  
    $form_state = array();
    $form_state['title'] = t('CANCELLATION POLICY');
    $form_state['ajax'] = TRUE;
  
    $output = ctools_modal_form_wrapper('exp_sp_commerce_admin_addedit_droppolicy_form', $form_state);
  
    if (!empty($form_state['executed'])) {
      ctools_drop_policy_form_submit_success();
    }
    print ajax_render($output);
    exit;
  }
  else {
    return drupal_get_form('exp_sp_commerce_admin_addedit_droppolicy_form', 'nojs', $entityId);
  }
  }catch (Exception $ex) {
  	watchdog_exception('display_drop_policy_form', $ex);
  	expertusErrorThrow($ex);
  }
}


function ctools_drop_policy_form_submit_success() {
	try {
  expDebug::dPrint('Success msg from ectools_drop_policy_form_submit_success()');
  }catch (Exception $ex) {
  	watchdog_exception('ctools_drop_policy_form_submit_success', $ex);
  	expertusErrorThrow($ex);
  }
}


/**
 * exp_sp_commerce_admin_addedit_droppolicy_form() - Build the addedit form.
 */
function exp_sp_commerce_admin_addedit_droppolicy_form($form, &$form_state, $renderMode, $arg) {
	try{
  //include_once('exp_sp_commerce_admin.inc');
  
  $header_optional = array('type' => 'file', 'scope' => 'header', 'group' => JS_LIBRARY);
  $css_module_optional = array('type' => 'file', 'group' => CSS_DEFAULT);
  $css_theme_optional = array('type' => 'file', 'group' => CSS_THEME);
  $js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
    global $theme_key;
  	if($theme_key == 'expertusoneV2') {
  		//NEWUI THEME STYLE SHEET
  		drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_v2.css', $css_theme_optional);
  	} else {
		drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration.css', $css_theme_optional);  		
  	}
  //drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration.css', $css_theme_optional);
  
  $dropPolicyDetails = array();

  //if (empty($form_state['values'])) { // An edit operation. Is not form rebuild after save, in which case we can fetch values from $form_state.
    // Fetch Drop Policy details to be displayed in the form
    $dropPolicyDetails = getDropPolicyDetails($arg, array('id', 'entity_id', 'entity_type', 'delivery_type','days', 'refund_percentage'));
    expDebug::dPrint(' $dropPolicyDetails = ' . print_r($dropPolicyDetails, true),4);
 // }
      
  $miscActions = array();
  
   // Create a new add/edit form
  $form = newAddEditSaveBtnForm($miscActions);
  
  $cnt = 5;
  addHiddenValueToAddEditForm($form, "hiddenid", count($dropPolicyDetails));  
  addHiddenValueToAddEditForm($form, "nofpolicy", $cnt);  
  
  addFieldSetToAddEditForm($form, 'dp_cls_fieldset', t('Cancellation Policy for ILT and VC Classes'));
 	
  for ($i = 0; $i < $cnt; $i++) {
 	addHiddenValueToAddEditForm($form, "policyid_$i" , $dropPolicyDetails[$i]->id);
  
  	$form['dp_cls_fieldset']["refund_percentage_$i"] = array(
	    '#type' => 'textfield',
	    '#prefix' => '<div><table class="dptxtfield" cellpadding="2" cellspacing="2" border="0"><tr><td>'.($i+1).'.</td><td>'.t('Deduct').'</td><td>',
	    '#suffix' => '</td><td> %&nbsp;&nbsp;'.t('if cancelled').'</td>',
	    '#title' => t('Class Percentage'),
	    '#title_display' => 'invisible',
	    '#default_value' => $dropPolicyDetails[$i]->refund_percentage,
	    '#size' => 5,
	    '#maxlength' => 5
  	);
  	
 	$form['dp_cls_fieldset']["days_$i"] = array(
	    '#type' => 'textfield',
	    '#prefix' => '<td>',
	    '#suffix' => '</td><td>'.t('day(s) before training start date.').'</td></tr></table></div>',
	    '#title' => t('Days'),
	    '#title_display' => 'invisible',
	    '#default_value' => $dropPolicyDetails[$i]->days,
	    '#size' => 5,
	    '#maxlength' => 5
  	); 
  	
  }  
  
  addFieldSetToAddEditForm($form, 'dp_wbt_fieldset', t('Cancellation Policy for WBT Classes'));  

  $form['dp_wbt_fieldset']["wbt_refund_percentage"] = array(
	    '#type' => 'textfield',
	    '#prefix' => '<div><table class="dptxtfield" cellpadding="2" cellspacing="2" border="0"><tr><td>'.t('Enter Percentage to be deducted').'</td><td>',
	    '#suffix' => '</td></tr></table></div>',
	    '#title' => t('WBT Percentage'),
	    '#title_display' => 'invisible',
	    '#default_value' => $dropPolicyDetails[$cnt]->refund_percentage,
	    '#size' => 5,
	    '#maxlength' => 5
  	); 
   
	addFieldSetToAddEditForm($form, 'dp_tp_fieldset', t('Cancellation Policy for Training Plan'));

	$form['dp_tp_fieldset']["tp_refund_percentage"] = array(
	    '#type' => 'textfield',
	    '#prefix' => '<div><table class="dptxtfield" cellpadding="2" cellspacing="2" border="0"><tr><td>'.t('Enter Percentage to be deducted').'</td><td>',
	    '#suffix' => '</td></tr></table></div>',
	    '#title' => t('TP Percentage'),
	    '#title_display' => 'invisible',
	    '#default_value' => $dropPolicyDetails[$cnt+1]->refund_percentage,
	    '#size' => 5,
	    '#maxlength' => 5
  	);
  	
  addFieldSetToAddEditForm($form, 'dp_vod_fieldset', t('Cancellation Policy for Video Classes'));  

  $form['dp_vod_fieldset']["vod_refund_percentage"] = array(
      '#type' => 'textfield',
      '#prefix' => '<div><table class="dptxtfield" cellpadding="2" cellspacing="2" border="0"><tr><td>'.t('Enter Percentage to be deducted').'</td><td>',
      '#suffix' => '</td></tr></table></div>',
      '#title' => t('Video Percentage'),
      '#title_display' => 'invisible',
      '#default_value' => $dropPolicyDetails[$cnt+2]->refund_percentage,
      '#size' => 5,
      '#maxlength' => 5
    );
    
  	$form['addedit-right-col-start'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="addedit-right-col-dropPolicy">',
  );

  // Add Save button
  $wrapper = 'addedit-form-wrapper';
  $ajaxCallback = 'addEditRepaintForm';
  
  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-save'); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $attributes['class'][] = 'addedit-form-expertusone-throbber';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  $attributes['data-wrapperid'] = array($wrapper);
  
  
  
  
  $form['save'] = array(
    '#type' => 'submit',
    '#prefix' => addEditGetPrefix('ACTION'),
    '#suffix' => addEditGetSuffix('ACTION'),
    '#value' => t('LBL141'),
    '#attributes' => $attributes,
    '#ajax' => array(
      'wrapper' => $wrapper,
      'callback' => $ajaxCallback,
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
  // Remove the error messages or success messages on clicking 
  $cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss();$(".active-qtip-div").remove();return false;';
  $cancelAttributes['class'][]   = 'admin-action-button-middle-bg';
    $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('LBL109'),
    '#title' => t('LBL109'),
    '#prefix' => '<div class="addedit-form-cancel-container-actions">',
    '#suffix' => '</div>',
    '#attributes' => $cancelAttributes,
    '#ajax' => array(
      'event'=>'click',
    )
  ); 
  $form['action_buttons'] = array(
    '#type' => 'markup',
    '#markup' => empty($miscAction)? '' : theme('addedit-misc-actions', $miscAction),
  );

  $form['addedit-right-col-end'] = array(
    '#type' => 'markup',
    '#markup' => '</div>',
  );
  	
  	
  addEditFormClose($form); 

 expDebug::dPrint(' $form = ' . print_r($form, true),5);
  return $form;
  }catch (Exception $ex) {
  	watchdog_exception('exp_sp_commerce_admin_addedit_droppolicy_form', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * exp_sp_commerce_admin_addedit_droppolicy_form_submit() - Save the data entered by the user in the exp_sp_commerce_admin_addedit_droppolicy_form form
 */
function exp_sp_commerce_admin_addedit_droppolicy_form_submit($form, &$form_state) {
	try {
  //include_once('exp_sp_commerce_admin.inc');
  
  // Check whether add or edit 
  if ($form_state['values']["hiddenid"] == 0) { // Add
    addNewDropPolicy($form, $form_state);
    drupal_set_message('Cancellation Policy details saved successfully');
  } else {
    updateDropPolicyDetails($form, $form_state);
    drupal_set_message('Cancellation Policy details updated successfully');
  }
  
  // Setting $form_state['rebuild'] = TRUE causes the form to be rebuilt again.
  $form_state['rebuild'] = TRUE;    
  
  return $form;
  }catch (Exception $ex) {
  	watchdog_exception('exp_sp_commerce_admin_addedit_droppolicy_form_submit', $ex);
  	expertusErrorThrow($ex);
  }
}

function exp_sp_commerce_admin_addedit_droppolicy_form_validate($form, &$form_state) {
  try { 
  $cnt = $form_state['values']['nofpolicy'];
   for ($i = 0; $i < $cnt; $i++) {
	  if (!empty($form_state['values']['dp_cls_fieldset']['days_'.$i]) && !is_numeric($form_state['values']['dp_cls_fieldset']['days_'.$i]) ) {
	    form_set_error('name', t('All Fields must be Numeric'));
	  }
	  
  	 if (!empty($form_state['values']['dp_cls_fieldset']['refund_percentage_'.$i]) && !is_numeric($form_state['values']['dp_cls_fieldset']['refund_percentage_'.$i]) ) {
	     form_set_error('name', t('All Fields must be Numeric'));
	   }
   }

	if (!empty($form_state['values']['dp_wbt_fieldset']['wbt_refund_percentage']) && !is_numeric($form_state['values']['dp_wbt_fieldset']['wbt_refund_percentage']) ) {
	    form_set_error('name', t('All Fields must be Numeric'));
	 }
	 
	if (!empty($form_state['values']['dp_tp_fieldset']['tp_refund_percentage']) && !is_numeric($form_state['values']['dp_tp_fieldset']['tp_refund_percentage']) ) {
	    form_set_error('name', t('All Fields must be Numeric'));
	 }
  if (!empty($form_state['values']['dp_vod_fieldset']['vod_refund_percentage']) && !is_numeric($form_state['values']['dp_vod_fieldset']['vod_refund_percentage']) ) {
      form_set_error('name', t('All Fields must be Numeric'));
   }
  
  return;
  }catch (Exception $ex) {
  	watchdog_exception('exp_sp_commerce_admin_addedit_droppolicy_form_validate', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * addNewDropPolicy() - Save a new organization details in the database
 */
function addNewDropPolicy($form, &$form_state) {
   expDebug::dPrint(' $form = ' . print_r($form, true),5);
   expDebug::dPrint(' $form_state = ' . print_r($form_state['values'], true),5);
  
  global $user;
      
  $txn = db_transaction();


  try {
  	
  	  $cnt = $form_state['values']['nofpolicy'];
  	  //$cnt =5;
  	  $insertStmt = db_insert('slt_drop_policy');
  	  
  	  $insertStmt->fields(array('delivery_type','days','refund_percentage','created_by','created_on'));
  	  
	  	  for ($i = 0; $i < $cnt; $i++) {
	
				expDebug::dPrint(' $form_state =>>>>> ' .$cnt,5);
				expDebug::dPrint('$form_state_day'.$form_state_day.' : $form_state_refund_percentage'.$form_state_refund_percentage.' : $form_state =>>>>> ' .$form_state['values']['refund_percentage_'.$i], 5);
			 		//if(!empty($form_state['values']['days_'.$i])) { 
					    $values = array(
					    			'delivery_type' => 'lrn_cls_dty_ilt',
					                'days' => (!empty($form_state['values']['dp_cls_fieldset']['days_'.$i]) ?  $form_state['values']['dp_cls_fieldset']['days_'.$i] : 0),
					                'refund_percentage' => (!empty($form_state['values']['dp_cls_fieldset']['refund_percentage_'.$i]) ? $form_state['values']['dp_cls_fieldset']['refund_percentage_'.$i] : 0),
					                'created_by' => $user->uid,
					                'created_on' => now(),
					    						'updated_by' => $user->uid,
					    						'updated_on' => now()
					              );
			            $insertStmt->values($values);
			 		//}
	  	  }
  	        
	  	  				$values = array(
	  	  							'delivery_type' => 'lrn_cls_dty_wbt',
					                'days' => 0,
					                'refund_percentage' => (!empty($form_state['values']['dp_wbt_fieldset']['wbt_refund_percentage']) ? $form_state['values']['dp_wbt_fieldset']['wbt_refund_percentage'] : 0),
					                'created_by' => $user->uid,
					                'created_on' => now(),
	  	  									'updated_by' => $user->uid,
	  	  									'updated_on' => now()
					              );
					              
						$insertStmt->values($values);
			
  	  					$values = array(
  	  								'delivery_type' => 'TP',
					                'days' => 0,
					                'refund_percentage' => (!empty($form_state['values']['dp_tp_fieldset']['tp_refund_percentage']) ? $form_state['values']['dp_tp_fieldset']['tp_refund_percentage'] : 0),
					                'created_by' => $user->uid,
					                'created_on' => now(),
  	  										'updated_by' => $user->uid,
  	  										'updated_on' => now()
					              );
					              
						$insertStmt->values($values);

                $values = array(
                      'delivery_type' => 'lrn_cls_dty_vod',
                          'days' => 0,
                          'refund_percentage' => (!empty($form_state['values']['dp_vod_fieldset']['vod_refund_percentage']) ? $form_state['values']['dp_vod_fieldset']['vod_refund_percentage'] : 0),
                          'created_by' => $user->uid,
                          'created_on' => now(),
                					'updated_by' => $user->uid,
                					'updated_on' => now()
                        );
                        
            $insertStmt->values($values);
            
  	        expDebug::dPrintDBAPI(' $insertStmt obj = ' , $insertStmt);  
		    
  	  		// Execute the insert statement. Fetch the new org id. 
		    $dPId = $insertStmt->execute();
		    
		    
		    expDebug::dPrint("Test >>>>>>>".$dPId,3);
		    //$form_state['values']['id'] = $dPId;
 			expDebug::dPrint(' $orgId = ' . print_r($dPId, true),3);  
    
  }
  catch (Exception $ex) {
  	expDebug::dPrint("Error Msg".print_r($ex, true));
    $txn->rollback();
    watchdog_exception('droppolicy', $ex);
  }
}


/*
 * updateOrganizationDetails - Updates an existing organization details when the user clicks save
 */
function updateDropPolicyDetails($form, &$form_state) {
   expDebug::dPrint(' $form = ' . print_r($form_state['values'], true),5);
   expDebug::dPrint(' $form_state = ' . print_r($form_state, true),5);
  
  global $user;
      
  $txn = db_transaction();

  try {

  	  $cnt = $form_state['values']['nofpolicy'];
  	
  	  for ($i = 0; $i < $cnt; $i++) {		    

		    $updateStmt = db_update('slt_drop_policy');
		    $fields = array(
		      'days' => (!empty($form_state['values']['dp_cls_fieldset']['days_'.$i]) ?  $form_state['values']['dp_cls_fieldset']['days_'.$i] : 0),
		      'refund_percentage' => (!empty($form_state['values']['dp_cls_fieldset']['refund_percentage_'.$i]) ? $form_state['values']['dp_cls_fieldset']['refund_percentage_'.$i] : 0),
		      'updated_by' => $user->uid,
		      'updated_on' => now()
		    );		    
		    $updateStmt->fields($fields);
		    $updateStmt->condition('id', $form_state['values']['policyid_'.$i]);
  			expDebug::dPrintDBAPI(' $$updateStmt obj = ' , $updateStmt);  
		    		    
  	   		$numUpdated = $updateStmt->execute();

		 }    
		 
		  $updateStmt = db_update('slt_drop_policy');
		    $fields = array(
		      'days' => 0,
		      'refund_percentage' => (!empty($form_state['values']['dp_wbt_fieldset']['wbt_refund_percentage']) ? $form_state['values']['dp_wbt_fieldset']['wbt_refund_percentage'] : 0),
		      'updated_by' => $user->uid,
		      'updated_on' => now()
		    );		    
		    $updateStmt->fields($fields);
		    $updateStmt->condition('delivery_type', 'lrn_cls_dty_wbt');
		    $numUpdated = $updateStmt->execute();
		    
		    $updateStmt = db_update('slt_drop_policy');
		    $fields = array(
		      'days' => 0,
		      'refund_percentage' => (!empty($form_state['values']['dp_tp_fieldset']['tp_refund_percentage']) ? $form_state['values']['dp_tp_fieldset']['tp_refund_percentage'] : 0),
		      'updated_by' => $user->uid,
		      'updated_on' => now()
		    );		    
		    $updateStmt->fields($fields);
		    $updateStmt->condition('delivery_type', 'TP');
		    $numUpdated = $updateStmt->execute();

      $updateStmt = db_update('slt_drop_policy');
        $fields = array(
          'days' => 0,
          'refund_percentage' => (!empty($form_state['values']['dp_vod_fieldset']['vod_refund_percentage']) ? $form_state['values']['dp_vod_fieldset']['vod_refund_percentage'] : 0),
          'updated_by' => $user->uid,
          'updated_on' => now()
        );        
        $updateStmt->fields($fields);
        $updateStmt->condition('delivery_type', 'lrn_cls_dty_vod');
        $numUpdated = $updateStmt->execute();
  }
  catch (Exception $ex) {
  	expDebug::dPrint("Error Msg".print_r($ex, true));
    $txn->rollback();
    watchdog_exception('droppolicy', $ex);
  }
}

function newAddEditSaveBtnForm($miscAction = array()) {
  try {
  $form = array();
  $form['#prefix'] = '<div id="addedit-form-wrapper" class="addedit-form-wrapper">';
  $form['#suffix'] = '</div>';  
  
  // Add a class to the form so that we can apply custom style (margin, padding, etc).  
  $form['#attributes'] = array('class' => array('addedit-form-wrapper'));
  $form['#tree'] = TRUE; // Show full path of values (hierarchical values)
  
  
  $form['addedit-left-col-start'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="addedit-left-col-dropPolicy">',
  );
  
  // These hidden form fields are needed to be able to communicate to ctools modal to refresh grid on close after a save op
  //addHiddenValueToAddEditForm($form, 'js_object_init_id', 'root-admin');
  //addHiddenValueToAddEditForm($form, 'js_object', 'narrowsearch');
      
  return $form;
  }catch (Exception $ex) {
  	watchdog_exception('newAddEditSaveBtnForm', $ex);
  	expertusErrorThrow($ex);
  }
}


?>