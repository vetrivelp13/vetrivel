<?php
/**
 * Show exp_sp_administration_cancellation_form in ctools popup
 */
function exp_sp_administration_cancellation_policy_display_form($js = NULL){
  try { 
	  $js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
	  //drupal_add_js(drupal_get_path('module', 'exp_sp_administration_commerce_setting') .'/exp_sp_administration_module_info_cancellationpolicy.js', $js_module_optional);
	  
	  // Default modalTheme is ExpertusCToolsModalTheme (see exp_sp_core_extend_ctools_ajax.js:Drupal.CTools.Modal.show())
	  $cancellationpolicy_style = array(
	    'ctools-admin-cancellation-policy-addedit-scroll-wrapper' => array(
	      'modalSize' => array(
	        'width' => 505,
	        'minHeight' => 280,
	       ),
	    )
	  );
	
	  drupal_add_js($cancellationpolicy_style, 'setting');
	  
	  if ($js) {
	    $formIdList = array(array('id'    => 'exp_sp_administration_cancellation_policy_form',
	                              'title' => (t('LBL1022')))); //Cancellation Policy
	    exp_ctools_modal_multi_ajaxonly_forms_display($formIdList, array());
	    
	  } else {
	    throw new Exception('exp_sp_administration_cancellation_policy_display_form() : nojs');
	  }
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_cancellation_policy_display_form', $ex);
    expertusErrorThrow($ex);
  }
}

function exp_sp_administration_cancellation_policy_form($form, &$form_state, $renderMode, $arg) {
  try{
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
  $form = $form = newAddEditFormWithCancelAndSave('cancellation-policy-admin-form');
  $cnt = 5;

  addHiddenValueToAddEditForm($form, "hiddenid", count($dropPolicyDetails));  
  addHiddenValueToAddEditForm($form, "nofpolicy", $cnt);  
  $form['dp_wbt_fieldset']["wbt_refund_percentage"] = array(
	    '#type' => 'textfield',
	    '#prefix' => '<div><table class="dptxtfield" cellpadding="2" cellspacing="2" border="0"><tr><td width="270px">'.t('Deduct Percentage for WBT').'</td><td>',
	    '#suffix' => '</td></tr></table></div>',
	    '#title' => t('WBT Percentage'),
	    '#title_display' => 'invisible',
	    '#default_value' => $dropPolicyDetails[$cnt]->refund_percentage,
	    '#size' => 5,
	    '#maxlength' => 5
  	); 
   
  $form['dp_tp_fieldset']["tp_refund_percentage"] = array(
	    '#type' => 'textfield',
	    '#prefix' => '<div><table class="dptxtfield" cellpadding="2" cellspacing="2" border="0"><tr><td width="270px">'.t('Deduct Percentage for Training Plan').'</td><td>',
	    '#suffix' => '</td></tr></table></div>',
	    '#title' => t('TP Percentage'),
	    '#title_display' => 'invisible',
	    '#default_value' => $dropPolicyDetails[$cnt+1]->refund_percentage,
	    '#size' => 5,
	    '#maxlength' => 5
  	);

  $form['dp_vod_fieldset']["vod_refund_percentage"] = array(
        '#type' => 'textfield',
        '#prefix' => '<div><table class="dptxtfield" cellpadding="2" cellspacing="2" border="0"><tr><td width="270px">'.t('Deduct Percentage for Video Class').'</td><td>',
        '#suffix' => '</td></tr></table></div>',
        '#title' => t('Video Percentage'),
        '#title_display' => 'invisible',
        '#default_value' => $dropPolicyDetails[$cnt+2]->refund_percentage,
        '#size' => 5,
        '#maxlength' => 5
    );
  
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
  
  cancellationpolicyAddEditFormWithCancelAndSaveClose($form, 'cancellation-policy-admin-form', 'cancellation-policy-save-button', 'cancellation-policy-cancel-button', 7);

  expDebug::dPrint(' $form = ' . print_r($form, true),5);
  return $form;
  }catch (Exception $ex) {
  	watchdog_exception('exp_sp_administration_cancellation_policy_form', $ex);
  	expertusErrorThrow($ex);
  }
}
/*
 * cancellationpolicyAddEditFormWithCancelAndSaveClose() - Customized closure for newAddEditFormWithCancelAndSave(). Adds the save and cancel button.
 */
function cancellationpolicyAddEditFormWithCancelAndSaveClose(&$form, $wrapperId, $saveButtonId, $cancelButtonId, $tabIndex) {
  try {
    $customHiddenButtonId = 'last-button-account';
    $esignHiddenButt = 'esign-'.$wrapperId;
    $esignHiddenButt1= 'status-upd-esign-'.$wrapperId;
   
    // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
    // each time the form is re-built.
    $cancelAttributes['class'] = array('addedit-edit-' . $cancelButtonId);
  
    $cancelAttributes['class'][] = 'admin-action-button-middle-bg';
    $cancelAttributes['tabindex'][] = $tabIndex + 1;
  
    // A common class for all addedit form cancel button, for styling to appear as a link.
    $cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';
    $cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss(); return false;';
    
    $form[$cancelButtonId] = array(
      '#type' => 'submit',
      '#prefix' => '<div class="addedit-form-cancel-and-save-actions-row">' .
                     '<div class="addedit-form-cancel-container-actions">',
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
    watchdog_exception('cancellationpolicyAddEditFormWithCancelAndSaveClose', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * exp_sp_administration_cancellation_policy_form_submit() - Save the data entered by the user in the exp_sp_administration_cancellation_policy_form_submit form
 */
function exp_sp_administration_cancellation_policy_form_submit($form, &$form_state) {
  try{
  // Check whether add or edit 
  
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
      if($eSignValidated) {
          if ($form_state['values']["hiddenid"] == 0) { // Add
            addNewCancellationPolicy($form, $form_state);
            drupal_set_message(t('LBL272').' '.t('MSG600').'.');// Cancellation saved
          } else {
            updateCacellationPolicyDetails($form, $form_state);
            drupal_set_message(t('LBL272').' '.t('MSG601').'.'); //Cancellation updated
          }
          
          // Setting $form_state['rebuild'] = TRUE causes the form to be rebuilt again.
          $form_state['rebuild'] = TRUE;    
          
          return $form;
    }
    }catch (Exception $ex) {
    	watchdog_exception('exp_sp_administration_cancellation_policy_form_submit', $ex);
    	expertusErrorThrow($ex);
    }
}

function exp_sp_administration_cancellation_policy_form_validate($form, &$form_state) {
  try{
  $cnt = $form_state['values']['nofpolicy'];
  $form_state['hidden-save-submit'] = false;
  $form_state['esign_show_popup']= false;
  
   // Determine which button triggers validation
    if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
      $validationTrigger  = 'save';
    }
    else{
      $validationTrigger  = 'hidden-save';
    }
   if ($form_state['triggering_element']['#name'] == $validationTrigger) {
      $form_state['esign_show_popup']= true;
         for ($i = 0; $i < $cnt; $i++) {
        	  if (!empty($form_state['values']['dp_cls_fieldset']['days_'.$i]) && !is_numeric($form_state['values']['dp_cls_fieldset']['days_'.$i]) ) {
        	    form_set_error('name', t('ERR197'));
        	  }
      	  
        	 if (!empty($form_state['values']['dp_cls_fieldset']['refund_percentage_'.$i]) && !is_numeric($form_state['values']['dp_cls_fieldset']['refund_percentage_'.$i]) ) {
      	     form_set_error('name', t('ERR197'));
      	   }
         }

      	if (!empty($form_state['values']['dp_wbt_fieldset']['wbt_refund_percentage']) && !is_numeric($form_state['values']['dp_wbt_fieldset']['wbt_refund_percentage']) ) {
      	    form_set_error('name', t('ERR197'));
      	 }
      	 
      	if (!empty($form_state['values']['dp_tp_fieldset']['tp_refund_percentage']) && !is_numeric($form_state['values']['dp_tp_fieldset']['tp_refund_percentage']) ) {
      	    form_set_error('name', t('ERR197'));
      	 }
         if (!empty($form_state['values']['dp_vod_fieldset']['vod_refund_percentage']) && !is_numeric($form_state['values']['dp_vod_fieldset']['vod_refund_percentage']) ) {
            form_set_error('name', t('ERR197'));
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
  return;
  }catch (Exception $ex) {
  	watchdog_exception('exp_sp_administration_cancellation_policy_form_validate', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * addNewCancellationPolicy() - Save a new cancellation details in the database
 */
function addNewCancellationPolicy($form, &$form_state) {
   expDebug::dPrint(' $form = ' . print_r($form, true),5);
   expDebug::dPrint(' $form_state = ' . print_r($form_state['values'], true),5);
  
  global $user;
      
  $txn = db_transaction();
  try {
  	
  	  $cnt = $form_state['values']['nofpolicy'];
  	  //$cnt =5;
  	  $insertStmt = db_insert('slt_drop_policy');
  	  
  	  $insertStmt->fields(array('delivery_type','days','refund_percentage','created_by','created_on','updated_by','updated_on'));
  	  
	  	  for ($i = 0; $i < $cnt; $i++) {
	            $k=$i+1;
				expDebug::dPrint(' $form_state =>>>>> ' .$cnt,5);
				expDebug::dPrint('$form_state_day'.$form_state_day.' : $form_state_refund_percentage'.$form_state_refund_percentage.' : $form_state =>>>>> ' .$form_state['values']['refund_percentage_'.$i], 5);
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
			            make_audit_trail_entries($values, getIdOfLoggedInUser() , null ,$k, 'Inserted','drop_policy','exp_sp_administration_commerce_setting','Insert the Cancellation Policy',
                                         array('days', 'refund_percentage','delivery_type'));
			 	
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
						 make_audit_trail_entries($values, getIdOfLoggedInUser() , null ,6, 'Inserted','drop_policy','exp_sp_administration_commerce_setting','Insert the Cancellation Policy',
                                         array('days', 'refund_percentage','delivery_type'));
			
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
						make_audit_trail_entries($values, getIdOfLoggedInUser() , null ,7, 'Inserted','drop_policy','exp_sp_administration_commerce_setting','Insert the Cancellation Policy',
                                         array('days', 'refund_percentage','delivery_type'));
			

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
		     make_audit_trail_entries($values, getIdOfLoggedInUser() , null ,8, 'Inserted','drop_policy','exp_sp_administration_commerce_setting','Insert the Cancellation Policy',
                                         array('days', 'refund_percentage','delivery_type'));
		    
		    expDebug::dPrint("Test >>>>>>>".$dPId,3);
		    //$form_state['values']['id'] = $dPId;
 			expDebug::dPrint(' $orgId = ' . print_r($dPId, true),3);  
    
  }
  catch (Exception $ex) {
  	expDebug::dPrint("Error Msg".print_r($ex, true));
    $txn->rollback();
    watchdog_exception('addNewCancellationPolicy', $ex);
  }
}


/*
 * updateCacellationPolicyDetails - Updates an existing cancellation details when the user clicks save
 */
function updateCacellationPolicyDetails($form, &$form_state) {
   expDebug::dPrint(' $form = ' . print_r($form_state['values'], true),5);
   expDebug::dPrint(' $form_state = ' . print_r($form_state, true),5);
  
  global $user;
      
  $txn = db_transaction();

  try {

  	  $cnt = $form_state['values']['nofpolicy'];
  	
  	  for ($i = 0; $i < $cnt; $i++) {		    
            $select = db_select("slt_drop_policy", "drp");
            $select->addField('drp', 'delivery_type', 'delivery_type');
            $select->addField('drp', 'days', 'days');
            $select->addField('drp', 'refund_percentage', 'refund_percentage');
            $select->addField('drp', 'id', 'id');
            $select->condition('drp.id', $form_state['values']['policyid_'.$i], '=');
            $select->orderBy('delivery_type');
            $result = $select->execute()->fetchAll();
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
  	   		
  	   	    // Audit Trail Entries 
		    make_audit_trail_entries($fields, getIdOfLoggedInUser() , $result ,$result[0]->id, 'Updated','drop_policy','exp_sp_administration_commerce_setting','Updated the Cancellation Policy',
                                         array('days', 'refund_percentage'));

		 }
		 // Query used getting old values for audit trail purpose 
      	  $conditon =array('lrn_cls_dty_wbt','TP','lrn_cls_dty_vod');	
          $select = db_select("slt_drop_policy", "drp");
          $select->addField('drp', 'delivery_type', 'delivery_type');
          $select->addField('drp', 'days', 'days');
          $select->addField('drp', 'refund_percentage', 'refund_percentage');
          $select->addField('drp', 'id', 'id');
          $select->condition('drp.delivery_type', $conditon, 'IN');
          $select->orderBy('delivery_type');
          expDebug::dPrintDBAPI(' $selectStmt objdrop policy = ' , $select); 
          $result = $select->execute()->fetchAll();
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
		    // Audit Trail Entries 
		    make_audit_trail_entries($fields, getIdOfLoggedInUser() , $result ,$result[1]->id, 'Updated','drop_policy','exp_sp_administration_commerce_setting','Updated the Cancellation Policy',
                                         array('days', 'refund_percentage'));
    
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
		     // Audit Trail Entries 
		    make_audit_trail_entries($fields, getIdOfLoggedInUser() , $result ,$result[2]->id, 'Updated','drop_policy','exp_sp_administration_commerce_setting','Updated the Cancellation Policy',
                                         array('days', 'refund_percentage'));

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
         // Audit Trail Entries 
		make_audit_trail_entries($fields, getIdOfLoggedInUser() , $result ,$result[0]->id, 'Updated','drop_policy','exp_sp_administration_commerce_setting','Updated the Cancellation Policy',
                                         array('days', 'refund_percentage'));
  }
  catch (Exception $ex) {
  	expDebug::dPrint("Error Msg".print_r($ex, true));
    $txn->rollback();
    watchdog_exception('updateCacellationPolicyDetails', $ex);
  }
}


function exp_sp_administration_catalog_course_addedit_new_form($form, &$form_state, $renderMode, $courseId, $classId){
  try {
        $form 		       = newAddEditFormWithCancelAndSave('catalog-course-basic-addedit-form');
 
        $form['mapped_course_id'] = array(
          '#type' => 'hidden',
          '#name' => 'mapped_course_id',
          '#value' => $entityId
        );
        addHiddenFieldToAddEditForm($form, 'empty_id', $emptyId);
        
        $form['course_detail_disp'] = array(
      	  '#type'       => 'markup',
          '#prefix'     => '<div id="catalog-course-basic-addedit-form-disp">',      
        );
    
       cancel_list_forms($form, $form_state, $defaultValue);
       $form['course_detail_wrapper'] = array(
    	  '#type'       => 'markup',
        //'#prefix'     => '<div class="catalog-course-basic-addedit-form-container" id="catalog-course-basic-addedit-form-container">',
        '#prefix'     => '<div class="addedit-form-main-container">
        					<div class="round-corner-left"><div class="round-corner-right"><div class="round-corner-middle"></div></div></div>
        					<div class="catalog-course-basic-addedit-form-container" id="catalog-course-basic-addedit-form-container">',    
      );
    
     addHiddenValueToAddEditForm($form, 'crs_id', $defaultValue['crs_id']);  
     $form['addedit-remove-space-title-start'] = array(
      '#type'   => 'markup',
      '#markup' => '<div style="width:100%;">',
    );
    addTextFieldToAddEditForm($form, 'crs_title', t('LBL083'), $defaultValue['crs_title'], 1, true);
    $form['addedit-remove-space-title-end'] = array(
      '#type'   => 'markup',
      '#markup' => '</div>',
    );
    
    
    
    $form_state['esign_from_plugin']= false;
    
     $subStage = $form_state['storage']['substep'];
     //addEditFormWithCancellationPolicyAndSaveClose($form, 'catalog-course-basic-addedit-form', 'catalog-course-basic-save', 'catalog-course-basic-cancel', $entityId, $subStage, $emptyId);
     cancellationpolicyAddEditFormWithCancelAndSaveClose($form, 'cancellation-policy-admin-form', 'cancellation-policy-save-button', 'cancellation-policy-cancel-button', 7);
	 $form['course_detail_disp_end'] = array(
  	  '#type'       => 'markup',
       //'#prefix'     => '<div id="catalog-course-basic-addedit-form-disp">',
       //'#suffix'     => '<div class="clearBoth"></div></div>',      
      '#suffix'     => '<div class="clearBoth"></div>
      					<div class="round-corner-footer-left"><div class="round-corner-footer-right">
      					<div class="round-corner-footer-middle"></div></div></div></div></div>',      
      //'#attributes' => $editAttributes,      
    );
    
    if($showCancelSave && $form_state['storage']['display_mode'] != 'view'){
      $form['course_detail_wrapper_end'] = array(
        '#type'       => 'markup',      
        '#suffix'     => '</div>',     
      );	 
    } else {
      $form['course_detail_wrapper_end'] = array(
        '#type'       => 'markup',      
        '#suffix'     => '<div class="clearBoth"></div><div class="admin-class-course-separater"></div></div>',     
      );	 
    }    
    
    return $form;
  
  } catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_catalog_course_addedit_new_form', $ex);
    expertusErrorThrow($ex);
  }
}

/*
* addEditFormWithCourseCancelAndSaveClose() 
 */
function addEditFormWithCancellationPolicyAndSaveClose(&$form, $wrapperId = 'addedit-form-wrapper', $saveButtonId = 'save-button', $cancelButtonId = 'cancel-button', $courseId, $subStage = '', $emptyId = '') {
  
  try {
      expDebug::dPrint(' addEditFormWithCancelAndSaveClose() called.');
      
      $customHiddenButtonId = 'last-button-account';
      
      $esignHiddenButt = 'esign-'.$wrapperId;
      
      // Remove the temporary storage for API's to exchange information during form creation
      unset($form['addedit_tmp_storage']);
      
      // Add the Cancel button
      $ajaxCallbackCancel = 'addEditCancelRepaintForm';
      
      // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
      // each time the form is re-built.
      $cancelAttributes['class'] = array('addedit-edit-' . $cancelButtonId); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
      
      // For common styling of all addedit cancel buttons
      //$cancelAttributes['class'][] = 'addedit-edit-cancel';
      
      // Using this class to applying bg color to the save button
      $cancelAttributes['class'][] = 'admin-action-button-middle-bg';
    
      // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
      $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber';
      // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
      $cancelAttributes['data-wrapperid'] = array($wrapperId);
      
      // A common class for all addedit form cancel button, for styling to appear as a link.
      $cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';
      
      if(empty($courseId)){
        $cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss(); return false;';
        $cancelLabel  = t('LBL123'); // Close
      }else{
        $cancelLabel  = t('LBL109'); // Cancel
      }
      
      $cancelAttributes['tabindex'][] = '8';
      
      $form[$cancelButtonId] = array(
        '#type' => 'submit',
        '#prefix' => '<div class="addedit-form-cancel-and-save-actions-row">' .
                       '<div class="addedit-form-cancel-container-actions">',
        //'#suffix' =>   '<div class="admin-save-button-right-bg"></div>',
        '#value' => $cancelLabel,
        '#name' => 'cancelcourse',
        '#attributes' => $cancelAttributes,
      );
      
     if (!empty($courseId)) { // Cancel button instead of Close is shown only when $courseId is not empty (see above). Cancel button is ajax enabled.
     	$form[$cancelButtonId]['#ajax'] = array(
          'wrapper' => $wrapperId,
          'callback' => $ajaxCallbackCancel,
          'method' => 'replace',
          'effect' => 'none', // 'fade',
          'event'=>'click',
          'keypress' => true,
          'progress' => array(
            'type' => 'throbber',
            'message' => ''
          ),
        );
     }
      
      
      // Add the Save button
      $ajaxCallbackSave = 'addEditRepaintForm';
      
      // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
      // each time the form is re-built.
      $saveAttributes['class'] = array('addedit-edit-' . $saveButtonId); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
    
      // For common styling of all addedit save buttons
      //$saveAttributes['class'][] = 'addedit-edit-save';
      
      // Using this class to applying bg color to the save button
      $saveAttributes['class'][] = 'admin-save-button-middle-bg';
      //$saveAttributes['class'][] = 'edit-catalog-course-save-publish';
    
    
      //Increase the width of the popup while clicking on save button
      $saveAttributes['onclick'][] = 'increasePopWidth();';
    
      // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
      $saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
      // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
      $saveAttributes['data-wrapperid'] = array($wrapperId);
      $saveAttributes['tabindex'][] = '7';
      $eSignatureAvailable = ($subStage == 'Attachment') ? false : true;
      
    
      if($subStage == 'details' || empty($subStage)) {
        $courseStatus     = '';    
        if (!empty($courseId)) { 
          $CourseDetails   = getCourseDetail($courseId, array('id,status,title'));
          $courseStatus    =  $CourseDetails[0]->crs_status;
        }    
        if($courseId){
       	  $ifCourseValid = getIfCourseValid($courseId);   	  
    	}
       $isSaveParent = 0;
       if(empty($courseId) || $courseStatus != 'lrn_crs_sts_atv'){
       	  
          $isSaveParent = 1;
          $saveAttributes['onclick'] = array();
          //$saveAttributes['onclick'][] = 'hideMessageInfo();';
          $saveAttributes['onclick'][] = 'increasePopWidth();';
	      $saveAttributes['class'][] = (($courseId) ? 'admin-save-button-dropdown-middle-bg-wrapper' : '');
         // $saveAttributes['onclick'][] = '$("#'.$customHiddenButtonId.'").click();';	 
          $form[$saveButtonId] = array(
    	    '#type' => 'submit',
    	    '#prefix' => '<div class="admin-save-pub-unpub-button-container"><div class="admin-save-button-left-bg"></div>',
    	    '#suffix' => (($courseId) ? '<span id="pub-unpub-action-btn" onclick="displayPubActionList(\'crs-pub-save-btn\')" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span>' : '<div class="admin-save-button-right-bg"></div>').'</div>',
    	    '#value' => t('LBL141'),
          	'#name' => 'save',
    	    '#attributes' => $saveAttributes,
            '#ajax' => array(
      	      'wrapper' => $wrapperId,
      	      'callback' => $ajaxCallbackSave,
              'customButtonId'=>$customHiddenButtonId,
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
       
       
      	if($courseId){
          $resultCnt = delete_object_record($courseId,'Course');   
          $ifCourseUnpublish = getIfCourseBeUnpublished($courseId);	  
    	}
        if($courseId){
    	    if($isSaveParent){
    	      $form['addedit-button-group-ul-start'] = array(
    	        '#type' => 'markup',
    	        '#markup' => '<ul class="catalog-pub-add-list crs-pub-save-btn">',
    	      ); //  catalog-save-btn
    	      $savePublishPrefix = '<li class="save-pub-unpub-sub-menu">';
    	      $savePublishSuffix = '</li>';
    	    }else{
 	      	     $savePublishAttributes['class'][] = ((($ifCourseUnpublish == 0) && ($courseStatus == 'lrn_crs_sts_atv')) ? ('admin-save-button-dropdown-middle-bg-wrapper') : '' );
	      		$savePublishPrefix = '<div class="admin-save-pub-unpub-button-container"><div class="admin-save-button-left-bg"></div>';
	      		$savePublishSuffix = ((($ifCourseUnpublish == 0) && ($courseStatus == 'lrn_crs_sts_atv')) ? ('<span id="pub-unpub-action-btn" onclick="displayPubActionList(\'crs-pub-save-btn\')" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span>') : '<div class="admin-save-button-right-bg"></div>' ).'</div>';      

    	    }
    	    
    	    if($ifCourseValid[0]->sesavailable != 0){
    		  $saveButtonId = 'catalog-course-save-publish';
    		  $savePublishAttributes['onclick'] = array();
    		  $savePublishAttributes['tabindex'][] = '7';
    		  
    	     // $savePublishAttributes['onclick'][] = 'Drupal.ajax.prototype.commands.addEditHideMessages();';
    	      $savePublishAttributes['onclick'][] = 'increasePopWidth();';
    	      $savePublishAttributes['class'][] = 'addedit-form-expertusone-throbber';
            $savePublishAttributes['data-wrapperid'] = array($wrapperId);
    	     // $savePublishAttributes['onclick'][] = '$("#'.$customHiddenButtonId.'").click();';	 
    	     $buttonLabel = ($courseStatus == 'lrn_crs_sts_atv') ? t('LBL141') : t('LBL614');
    	     //$savePublishAttributes['class'][] = (($resultCnt==0) && ($courseStatus == 'lrn_crs_sts_atv')) ? 'admin-save-button-middle-bg' : 'pub-unpub-only-save-btn';
    	     $savePublishAttributes['class'][] = 'admin-save-button-middle-bg';
    	      $form[$saveButtonId] = array(
    		    '#type' => 'submit',
    		    '#prefix' => $savePublishPrefix,
    		    '#suffix' => $savePublishSuffix,
    		    '#value' => $buttonLabel,
    	      	'#name' => 'saveandpublish',
    		    '#attributes' => $savePublishAttributes,
    	        '#ajax' => array(
    	  	      'wrapper' => $wrapperId,
    	  	      'callback' => $ajaxCallbackSave,
    	          'customButtonId'=>$customHiddenButtonId,
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
        }
    	 
       if(!$isSaveParent){
          $form['addedit-button-group-ul-start'] = array(
            '#type' => 'markup',
            '#markup' => '<ul class="catalog-pub-add-list crs-pub-save-btn">',
          );
        }
        
    
    	  
    	if(($courseId) && $courseStatus == 'lrn_crs_sts_atv'){
    	  if($ifCourseUnpublish == 0){
      	    $saveButtonId = 'catalog-course-save-unpublish';
      	    $saveUnPublishAttributes['onclick'] = array();
            //$saveAttributes['onclick'][] = 'hideMessageInfo();';
            $saveUnPublishAttributes['onclick'][] = 'increasePopWidth();';
           // $savePublishAttributes['onclick'][] = '$("#'.$customHiddenButtonId.'").click();';	 
            $form[$saveButtonId] = array(
      	    '#type' => 'submit',
      	    '#prefix' => '<li class="save-pub-unpub-sub-menu">',
      	    '#suffix' => '</li>',
      	    '#value' => t('LBL571'),
            '#name' => 'unpublish',
      	    '#attributes' => $saveUnPublishAttributes,
            '#ajax' => array(
        	      'wrapper' => $wrapperId,
        	      'callback' => $ajaxCallbackSave,
                  'customButtonId'=>$customHiddenButtonId,
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
    	}
    	
        
        if(($courseId)){
          if($resultCnt == 0){ 
            $displayTitle = titleController('ADMIN-COMMERCE-SETTING-CANCELLATIONPOL-DISPLAY-TITLE', $CourseDetails[0]->crs_title,34);
      	   $saveButtonId = 'catalog-course-delete';
      	   
      	   $deleteAttributes['onclick'] = array();
            $deleteAttributes['onclick'][] = '$("#root-admin").data("narrowsearch").displayDeleteWizard("'.t('MSG357').' '.strtolower(t('Course')).' '.escape_string($CourseDetails[0]->crs_title) . '","'.$courseId.'","Course"); return false;';
            $deleteAttributes['class'][] = 'admin-delete-button-bg';
             $form[$saveButtonId] = array(
        	    '#type' => 'button',
        	    '#prefix' => '<li class="save-pub-unpub-sub-menu">',
        	    '#suffix' => '</li>',
        	    '#value' => t('LBL286'),
                '#name' => 'delete',
        	    '#attributes' => $deleteAttributes,
        	  );
          } 
        }
      
    	$form['addedit-button-group-ul-end'] = array(
          '#type' => 'markup',
          '#markup' => '</ul><div class="clearBoth"></div></div>' .
    	             '<div class="clearBoth"></div></div>',
        ); 
      }
    else{
       $saveAttributes['onclick'] = array();
      //$saveAttributes['onclick'][] = 'hideMessageInfo();';
      $saveAttributes['onclick'][] = 'Drupal.ajax.prototype.commands.addEditHideMessages();';
      $saveAttributes['onclick'][] = 'increasePopWidth();';	 
      $form[$saveButtonId] = array(
        '#type' => 'submit',
        '#value' => t('LBL141'),
        '#name' => 'save',
     	'#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
        '#suffix' => 
                 '<div class="admin-save-button-right-bg"></div></div>' .
                 '<div class="clearBoth"></div></div>' .
               '<div class="clearBoth"></div></div>',
        '#id' =>$saveButtonId,
    	'#attributes' => $saveAttributes,
        '#ajax' => array(
          'wrapper' => $wrapperId,
          'callback' => $ajaxCallbackSave,
          'method' => 'replace',
          'effect' => 'none', // 'fade',
      	  'event'=>'click',
              'customButtonId'=>$customHiddenButtonId,
          'keypress' => true,
          'progress' => array(
            'type' => 'throbber',
            'message' => ''
          ),
      	  )
      );
        
    }
      unset($saveAttributes['tabindex']);
      $saveAttributes['onclick'] = array();
      //$saveAttributes['onclick'][] = 'hideMessageInfo();';
      $saveAttributes['onclick'][] = 'Drupal.ajax.prototype.commands.addEditHideMessages();';	 
      $saveAttributes['onclick'][] = 'increasePopWidth();';
      $form[$customHiddenButtonId] = array(
    	      '#type' => 'submit',
    	      '#value' => t('hidden-save'),
    	      '#name'	=> 'hidden-save',
          	  '#prefix' => '<span id = "esign-admin-div" style="display:none;">',
    	      '#suffix' => '</span>',
    	      '#id' =>$customHiddenButtonId,
      		  '#attributes' => $saveAttributes,
    	      '#ajax' => array(
    		      'wrapper' => $wrapperId,
    		      'callback' => 'addEditRepaintForm',
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
    	      
      $form['addedit-left-col-end'] = array(
        '#type' => 'markup',
        '#markup' => '</div><div class="clearBoth"></div>',
      	
      );
  } catch (Exception $ex) {
    watchdog_exception('addEditFormWithCancellationPolicyAndSaveClose', $ex);
    expertusErrorThrow($ex);
  }       
}

function cancel_list_forms(&$form, &$form_state, $defaultValue){
      try {
      $form['course_actions_disp'] = array(
      	  '#type'       => 'markup',
          '#prefix'     => '<div class="catalog-course-basic-addedit-action-disp" id="catalog-course-basic-addedit-action-disp">',
          //'#suffix'     => '</div>',      
          //'#attributes' => $editAttributes,      
      );
    
      if(!isset($form_state['storage']['substep'])){
				$form_state['storage']['substep'] = '';
			}
      $isSelected = ($form_state['storage']['substep'] == 'details' || empty($form_state['storage']['substep'])) ? 'selected' : ''; 
      $form['edit_btn_details'] = array(
      	  '#type'       => 'submit',      
    	  '#value'      => t('Class Room'),
          '#name'       => 'details',
          //'#prefix'     => '<div id="wizard-course-form-wrapper"><div class="admin-course-edit-button-container"><div class="admin-save-button-left-bg"></div>',
          //'#suffix'     => '<div class="admin-save-button-right-bg"></div></div>',
          '#prefix'     => '<ul><li>',
          '#suffix'     => '</li>',
          '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'catalog-course-basic-addedit-form'),
            '#ajax' => array(  
      		'callback' => 'addEditRepaintForm',
            'wrapper'  => 'catalog-course-basic-addedit-form',
            'event'	   => 'click'
          ), 
        );
         $entityId       = $form['mapped_course_id']['#value'];    
         //if($entityId && ($form_state['storage']['step'] == 'edit' || empty($form_state['storage']['step']))){
    	 if($entityId){
    	   $isSelected = ($form_state['storage']['substep'] == 'Attachment') ? 'highlighted' : ''; 
    	   
          $isSelected = ($form_state['storage']['substep'] == 'prerequisite') ? 'highlighted' : '';      
          $form['edit_btn_prerequest'] = array(
            '#type'       => 'submit',      
            '#value'      => t('Other training'),
            '#name'       => 'prerequisite',
            '#prefix'     => '<li>',
            '#suffix'     => '</li>',
            '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'catalog-course-basic-addedit-form'),
            '#ajax' => array(  
          	  'callback' => 'addEditRepaintForm',
              'wrapper'  => 'catalog-course-basic-addedit-form',
              'event'	   => 'click'
            ), 
          );
          
        
       }
       else{
          //$isSelected = ($form_state['storage']['substep'] == 'prerequisite') ? 'highlighted' : '';      
          $form['edit_btn_prerequest_disabled'] = array(
            '#type'       => 'item',      
            '#title'      => t('Other training'),
            '#prefix'     => '<li><span class="deactive-links">',
            '#suffix'     => '</span></li>'
          );
          
         
         $form['course_actions_ul_container_disp_end'] = array(
           '#type'       => 'markup',
           '#suffix'     => '</ul>',  
         );
       }
       $form['course_actions_disp_end'] = array(
         '#type'       => 'markup',
         '#suffix'     => '</div>',  
       );    
   } catch (Exception $ex) {
    watchdog_exception('cancel_list_forms', $ex);
    expertusErrorThrow($ex);
  }       
}
?>