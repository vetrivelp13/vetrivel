<?php 
function exp_sp_administration_payment_method_display_form($js = NULL){
  try {
  	
  	// Default modalTheme is ExpertusCToolsModalTheme (see exp_sp_core_extend_ctools_ajax.js:Drupal.CTools.Modal.show())
	  $paymentmethod_style = array(
	    'ctools-admin-payment-method-addedit-scroll-wrapper' => array(
	      'modalSize' => array(
	        'width' => 750,
	        'minHeight' => 280,
	       ),
	      'modalOptions' => array(
	        'top' => '1',
	      ),
	    )
	  );
	
	  drupal_add_js($paymentmethod_style, 'setting');
	  
	  if ($js) {
	    $formIdList = array(array('id'    => 'exp_sp_administration_payment_method_form',
	                              'title' => t('LBL548'))); //Payment Method
	    exp_ctools_modal_multi_ajaxonly_forms_display($formIdList, array());
	    
	  } else {
	    throw new Exception('exp_sp_administration_payment_method_form() : nojs');
	  }
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_payment_method_display_form', $ex);
    expertusErrorThrow($ex);
  }
}
function exp_sp_administration_payment_method_form($form, &$form_state, $renderMode, $arg) {
  try{
  $entityId   = 1;
  $entityType = 'payment';
  $formName = 'payment-method';
  $uniqueId = 'PaymentMethod-'.$entityId.'-'.$entityType;
  $wrapperId = 'cs-grid-wrapper-'.$uniqueId;
  $customHiddenButtonId = 'equv-save-esign';
 
  $form[$formName] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="admin-commerce-payment-method-plan"><div id="'.$wrapperId.'" class="admin-datagrid-pagination admin-commerce-payment-method-datagrid-wrapper">',
    '#suffix' => '</div></div>',
  );
  
  $form[$formName]['hidden_idlist'] = array(
      '#type' => 'hidden',
      '#name' => 'hidden_idlist_'.$uniqueId,
      '#id' => 'datagrid-idlist-'.$uniqueId,
    );

  $excludedCourseId =0;
  $form[$formName]['paymentmethod_view_grid_load'] = array( 
    '#type' => 'markup',
    '#prefix' => '<div style="display:none"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-'.$uniqueId.'\'); $(\'body\').data(\'mulitselectdatagrid\').loadDataGrid(\'edit\', \'PaymentMethod\', \'\', \''.$entityId.'\', \''.$entityType.'\', \''.$excludedCourseId.'\');" width="100" height="132" /></div>',
  ); 
  $form[$formName]['paymentmethod_view_grid_markup'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="datagrid-div-'.$uniqueId.'" class="datagrid-payment-method-commerce"><table id="datagrid-container-'.$uniqueId.'" class="datagrid-container-common"></table><div id="pager-datagrid-'.$uniqueId.'" class="pager-datagrid-common"></div></div>',
  );
  $form[$formName][] = array(
    '#type' => 'markup',
    '#markup' => '<div class="clearBoth"></div><div></div></div>',
  );  
  $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-action-button-middle-bg';
  $cancelAttributes['onclick'][]         = 'Drupal.CTools.Modal.dismiss(); return false;';
  $cancelAttributes['data-wrapperid'] = array($wrapperId);
  
  /*$form[$formName]['cancel_paymentmethod_edit_mode'] = array(
    '#type' => 'submit',
    '#value' => t('LBL123'), //Close
    '#title' => t('LBL123'),
    '#prefix' => '<div></div><div class="admin-commerce-setting"><div class="admin-datagrid-save-cancel-button-align-bottom"><div class="addedit-form-cancel-container-actions admin-save-button-container">',
    '#suffix' => '</div></div></div>',
    '#name' => 'Cancel attachcourse',
    '#attributes' => $cancelAttributes,
  );*/
         
  return $form;
  }catch (Exception $ex) {
  	watchdog_exception('exp_sp_administration_payment_method_form', $ex);
  	expertusErrorThrow($ex);
  }
}


function loadPaymentMethod(){
	try{
  $paymentList = _uc_payment_method_list();
  $order = array('credit', 'check','po','other');// rule indicating new key order
  $paymetTypeList = array();
  foreach($order as $index) {
      $paymetTypeList[$index] = $paymentList[$index];
  }
  return $paymetTypeList;
  }catch (Exception $ex) {
  	watchdog_exception('loadPaymentMethod', $ex);
  	expertusErrorThrow($ex);
  }
}
function getPaymentMethodHTML($payment) { 
  try {
  $detailsList = array();
  $actionList = array(); 
  $enableSetting = 2;
  if( $payment->id =='credit'){
      $labelName  = t('Credit card');
  }elseif($payment->id == 'check'){
      $labelName  = t('Check');
  }elseif($payment->id == 'po'){
      $labelName  = t('Purchase Order');
  }else{
     $labelName  = t('Contract or Agreement Number/Invoice');
  } 
  if (!empty($payment->name)) {
    $detailsList[] = array('detail' => t($payment->name), 'tooltip' => t('LBL548').': '.$payment->name);
  }	
  if (!empty($payment->id)) {
    $detailsList[] = array('detail' => $payment->id, 'tooltip' => $payment->id);
  }	
  if($payment->id =='check' || $payment->id =='credit'){
    $enableSetting = 1;
  }
  $actionList[] = array('action_link_text' => t('LBL1021'),
					  'action_page_path' => 'administration/commerce/tax/nojs/addedit', 
					  'action_button_params' => $payment->id,
                      'action_payment_status' => $payment->status_code,
                      'action_payment_setting' => $enableSetting,
  					  'ctools_style' => 'empty',
  					  'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                       ),
                      'tooltip' => t('Setting')
					  );
 			   		  					  
  return theme('narrow-search-results-item-details',
                 array('title' => $labelName,                        
                       'details_list' => $detailsList,
                       'delete_info' => t('MSG357').' '.t('LBL623'),
                       'action_list'=> $actionList,
                       'description'=> '',
                      )
                );
  }catch (Exception $ex) {
  	watchdog_exception('getPaymentMethodHTML', $ex);
  	expertusErrorThrow($ex);
  }         
}
function getPaymentActionsHTML($payment) {
	try {
  return theme('narrow-search-results-item-actions',
                   array('catalog_details' => $payment)
              );    
  }catch (Exception $ex) {
  	watchdog_exception('getPaymentActionsHTML', $ex);
  	expertusErrorThrow($ex);
  }       
}

function enableOrDisablePayment($paymentId, $paymentName ){
  $txn = db_transaction();
  try {
    $getPaymentValue = variable_get('uc_payment_method_' . $paymentId . '_checkout', $paymentName);
    $getPaymentName  = 'uc_payment_method_' . $paymentId . '_checkout';
    if($getPaymentValue == 1) {
      $updatedstatus = 0;
    }else{
      $updatedstatus = 1;
    }
    //Update the module status
    variable_set($getPaymentName, $updatedstatus);
	//$returnStatus  = ($updatedstatus > 0)?t('Disable'):t('Enable');
    //LBL920 => Disable
    //LBL919 => Enable
    //0041112: Enable and disable not translated in commerce settings - payment method , for multi language support
    $returnStatus  = ($updatedstatus > 0)?t('LBL920'):t('LBL919');
	print $updatedstatus.'|'.$returnStatus;	                                     
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('enableOrDisablePayment', $ex);
    throw $ex;
  }
  unset($txn);
}

?>