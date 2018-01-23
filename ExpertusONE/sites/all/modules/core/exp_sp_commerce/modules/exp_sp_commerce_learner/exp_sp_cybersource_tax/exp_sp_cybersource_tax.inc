<?php
/*
 * calculateTaxForOrder() - Calculate total tax for an order
 */

function calculateTaxForOrder($order) {
  try {
    // Segregate products in order based on delivery_type, find their total after subtracting any discount and get address for tax calculation purpose
    $orderItemsGroupsList = segregateOrderItemsForTax($order);
    expDebug::dPrint('$orderItemsGroupsList = ' . $orderItemsGroupsList, 4);
    // Chekc the callout featute enabled or not
    if (module_exists('exp_sp_callout') && calloutIsActive() &&  calloutEntityIsActive('cme_tax')) {
    	$callOutInfo = getCalloutEntityInfo('cme_tax');
    	expDebug::dPrint('CAllOUT TAX = ' . $callOutInfo, 4);
    	// include the external source file
    	require_once DRUPAL_ROOT. $callOutInfo['file'];
    	$taxSettings = getTaxSetting();
    	 
    	//  here we send the  $orderItemsGroupsList -> this contain the Items based ilt and nonilt.
    	//  $order -> this object contain the billing details, order details, item details.
    	//	$taxSettings --> this array contain the site setting tax details.
    	//  this function only return the total tax amount of ordered items.
    	$result = processCalloutTaxRequest($orderItemsGroupsList,$order,$taxSettings);
    	require_once DRUPAL_ROOT. '/sites/all/modules/core/exp_sp_core/modules/exp_sp_callout/exp_sp_callout.inc';
    	updateTaxInformation($orderItemsGroupsList,$order,$taxSettings,$result);
    	$totalTaxAmount = $result['wholeTaxAmount'];
    	expDebug::dPrint('Total CAllOUT TAX amount = ' . $totalTaxAmount, 4);
    }
    else {
    	$taxSettings = getTaxSettingsForCountry($orderItemsGroup['Address']['Country']);
    	// tax calculation exsting work flow code
    // Determine the URL for the CyberSource SOAP Service.
    if (variable_get('uc_cybersource_server', 'test') == 'test') {
      $csSOAPServiceURL = 'https://ics2wstest.ic3.com/commerce/1.x/transactionProcessor/CyberSourceTransaction_1.38.wsdl';
    }
    else {
      $csSOAPServiceURL = 'https://ics2ws.ic3.com/commerce/1.x/transactionProcessor/CyberSourceTransaction_1.38.wsdl';
    }
    expDebug::dPrint('$csSOAPServiceURL = ' . $csSOAPServiceURL, 5);
    
    //Get cybersource SOAP service login credentials
    $soapLoginData = _uc_cybersource_soap_login_data();
    
    // Create the SoapClient
    require_once DRUPAL_ROOT."/sites/all/commonlib/CyberSourceClientLib.php";
    
    define('MERCHANT_ID',           $soapLoginData['merchant_id']); // used by SoapClient::_doRequest() override function in ExtendedClient class
    define('TRANSACTION_KEY',       $soapLoginData['transaction_key']); // used by SoapClient::_doRequest() override function in ExtendedClient class
      
    $soapClient = new ExtendedClient($csSOAPServiceURL, array()/*array('exceptions'=>FALSE, 'trace'=>true)*/);
    
    // Calculate tax
    $totalTaxAmount = 0;
    foreach ($orderItemsGroupsList as $orderItemsGroupKey => $orderItemsGroup) {
      $error = false;
      $tax = calculateTaxForOrderItemsGroup($soapClient, $soapLoginData['merchant_id'], $order, $orderItemsGroupKey, $orderItemsGroup, $error);
      // Break on error
      if ($error) {
      //  Lenthy Message is Commented For this Ticket #0039049	
       // $config = getConfig("exp_sp");
        $defaultErrorMsg = t('MSG593'); // . '. ' .//Unable to calculate tax, reason code
                          /*  t('MSG595') . ' ' . //Please contact your local training office or the
                           variable_get('site_name', 'ExpertusONE') . ' ' . t('MSG596') . ' ' . $config["reply_to"] . '.'; //team at */
        switch ($tax->reasonCode) {
          case 102: // One or more fields in the request contains invalid data. See the reply fields invalidField_0...N for which fields are invalid.
          	if($order->data['order_create_from'] == 'admin_checkout'){
            	$_SESSION['admin_cybersourceTaxError'] = $defaultErrorMsg;
          	}
          	else{
          		$_SESSION['cybersourceTaxError'] = $defaultErrorMsg;
          	}
            if ($orderItemsGroupKey == 'NONILT') {
              if (!empty($tax->invalidField)) {
                if (strpos($tax->invalidField, 'billTo') !== false) {
                	if($order->data['order_create_from'] == 'admin_checkout'){
	                  $_SESSION['admin_cybersourceTaxError'] = t('MSG593'); // . '. ' .//Unable to calculate tax, reason code
	                                                   /* t('MSG597') . ', ' . //Please check your billing information
	                                                   t('MSG598') . ' ' . //else contact your local training office or the
	                                                   variable_get('site_name', 'ExpertusONE') . ' ' . t('MSG596') . ' ' . $config["reply_to"] . '.'; //team at */
	                	}
                	else{
                		$_SESSION['cybersourceTaxError'] = t('MSG593'); // .  '. ' . //Unable to calculate tax, reason code
                				/* t('MSG597') . ', ' . //Please check your billing information
                				t('MSG598') . ' ' . //else contact your local training office or the
                				variable_get('site_name', 'ExpertusONE') . ' ' . t('MSG596') . ' ' . $config["reply_to"] . '.'; //team at */
                		}
                }
              }
            }
            break;
            
          case 101: // The request is missing one or more required fields. See the reply fields missingField_0...N for which fields are missing.
          case 150: // Error: General system failure.
          case 151: // Error: The request was received but there was a server timeout.
                    //        This error does not include timeouts between the client and the server.
          case 152: // Error: The request was received but there was a service timeout.
          case 234: // There is a problem with your CyberSource merchant configuration.
          case 600: // Address verification failed. Typically occurs if one part of the address is not consistent with another part.
                    // For example, occurs if the postal code is not consistent with the rest of the address.       
          default:
          	if($order->data['order_create_from'] == 'admin_checkout'){
            	$_SESSION['admin_cybersourceTaxError'] = $defaultErrorMsg;
          	}
          	else{
          		$_SESSION['cybersourceTaxError'] = $defaultErrorMsg;
          	}
            break;
        } // end switch
        expDebug::dPrint('$_SESSION[cybersourceTaxError] = ' . print_r($_SESSION['cybersourceTaxError'], true), 5);
        expDebug::dPrint('$_SESSION[admin_cybersourceTaxError] = ' . print_r($_SESSION['admin_cybersourceTaxError'], true), 5);
        return false;
      }
      
      // Add tax to the total tax amount
      $totalTaxAmount += $tax;
    }
    }
    
    // Return total tax
    return $totalTaxAmount;
  }
  catch (Exception $ex) {
      watchdog_exception('calculateTaxForOrder', $ex);
      expertusErrorThrow($ex);
  }
}

/*
 * calculateTaxForOrderItemsGroup() - Calculate tax for an order items group
 */
function calculateTaxForOrderItemsGroup($soapClient, $merchantId, $order, $orderItemsGroupKey, $orderItemsGroup, &$error) {
  try {

    // Determine if the country is a cybersource supported country. If unsupported return 0 tax
    // 137 countries are supported
    $supportedCountryCodes = array('AX', 'AL', 'DZ', 'AD', 'AI', 'AG', 'AR', 'AW', 'AU', 'AT', 'AZ', 'BS', 'BH', 'BY', 'BE', 'BM', 'BO', 'BR', 'BN',
                                   'BG', 'CA', 'TD', 'CL', 'CN', 'CO', 'KM', 'CR', 'CI', 'HR', 'CY', 'CZ', 'DK', 'DO', 'EC', 'EG', 'SV', 'EE', 'FI',
                                   'FR', 'GF', 'PF', 'GE', 'DE', 'GH', 'GI', 'GR', 'GD', 'GU', 'GT', 'GG', 'GW', 'GY', 'HN', 'HK', 'HU', 'IS', 'ID',
                                   'IR', 'IQ', 'IE', 'IL', 'IT', 'JM', 'JP', 'JE', 'JO', 'KZ', 'KR', 'KW', 'KG', 'LV', 'LB', 'LI', 'LT', 'LU', 'MO',
                                   'MK', 'MY', 'MV', 'MT', 'MH', 'MX', 'MD', 'ME', 'MA', 'NR', 'NL', 'NZ', 'NI', 'NG', 'MP', 'NO', 'OM', 'PK', 'PA',
                                   'PE', 'PH', 'PL', 'PT', 'QA', 'RO', 'RU', 'RW', 'KN', 'LC', 'SA', 'SN', 'RS', 'SG', 'SK', 'SI', 'ZA', 'ES', 'LK',
                                   'SE', 'CH', 'SY', 'TW', 'TH', 'TT', 'TN', 'TR', 'TM', 'TC', 'UA', 'AE', 'GB', 'US', 'UY', 'UZ', 'VE', 'VN', 'VG',
                                   'WF', 'YE', 'ZM', 'ZW');
    if (!in_array($orderItemsGroup['Address']['Country'], $supportedCountryCodes)) {
      expDebug::dPrint('Unsupported country ' . $orderItemsGroup['Address']['Country'], 5);
      clearItemLevelTaxes($orderItemsGroup, $order->data);
      return 0.00;
    }
  
    // Fetch tax settings for country. If tax setting is missing or any mandatory tax setting is missing for country, return 0 tax
    $taxSettings = getTaxSettingsForCountry($orderItemsGroup['Address']['Country']);
    if ($taxSettings === FALSE) {
      expDebug::dPrint('No tax settings for country ' . $orderItemsGroup['Address']['Country'], 5);
      clearItemLevelTaxes($orderItemsGroup, $order->data);
      return 0.00;
    }

    // Build request to send to cybersource
    $request = prepareTaxRequest($merchantId, $orderItemsGroupKey, $orderItemsGroup, $taxSettings);
  
    // Send Request  
    $reply = $soapClient->runTransaction($request);
    expDebug::dPrint('$reply = ' . print_r($reply, true), 5);
  
    // Log the tax request and response
    logTaxRequestResponse($orderItemsGroup['SltOrderId'], $soapClient->mRequestXML, $soapClient->mResponseXML);
    
    if ($reply->reasonCode != 100) {
      clearItemLevelTaxes(false, $order->data);
      $error = true;
      return $reply;
    }
    
    // Save item level calculated tax in slt_order_items
    saveItemLevelTaxes($orderItemsGroup, $reply->taxReply->item);
  
    // Return calculated tax amount
    return $reply->taxReply->totalTaxAmount;
  }
  catch (Exception $ex) {
      watchdog_exception('calculateTaxForOrderItemsGroup', $ex);
      expertusLogError($ex);
  }
}

/*
 * clearItemLevelTaxes() - Resets tax for individual line items of an order to 0 in slt_order_items table, unless the $orderItemsGroup is specified.
 *                         If $orderItemsGroup is specified, resets tax to 0 for only the line items in the order items group.
 */
function clearItemLevelTaxes($orderItemsGroup = false, $orderData = NULL) {
  try {
    expDebug::dPrint('$orderItemsGroup = ' . print_r($orderItemsGroup, true), 5);
    if($orderData['order_create_from'] == 'admin_checkout'){
    	$sltOrderID = $_SESSION['slt_admin_orderid'];
    }
    else{
    	$sltOrderID = $_SESSION['slt_order_id'];
    }
    if ($orderItemsGroup === false && !empty($sltOrderID)) {
      // Clear item level tax from slt_order_items table
      $updateStmt = db_update('slt_order_items');
      $updateStmt->fields(array('tax_amount' => 0,
                                'updated_by' => getSltpersonUserId(),
                                'updated_on' => now()
                               )
                         );
      $updateStmt->condition('order_id', $sltOrderID, '=');
      //$updateStmt->condition('user_id', getSltpersonUserId(), '=');
      expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
      $updateStmt->execute();
    }
    else {
      for($i = 0; $i < count($orderItemsGroup[Items]); $i++) {
        saveTaxForItem($orderItemsGroup['SltOrderId'], $orderItemsGroup[Items][$i]['EntityId'], $orderItemsGroup[Items][$i]['EntityType'],
                             $orderItemsGroup[Items][$i]['AdditionalInfo'], 0);
      }
    }
  }
  catch (Exception $ex) {
    watchdog_exception('clearItemLevelTaxes', $ex);
    expertusErrorThrow($ex);    
  } 
}

/*
 * saveItemLevelTaxes() - Saves calculated tax for individual lines items in a tax calc request into slt_order_items table
 */
function saveItemLevelTaxes($orderItemsGroup, $taxReplyItems) {
  try {
    expDebug::dPrint('$orderItemsGroup = ' . print_r($orderItemsGroup, true), 5);
    expDebug::dPrint('$taxReplyItems = ' . print_r($taxReplyItems, true), 5);
    
    if (is_array($taxReplyItems)) {
      for($i = 0; $i < count($taxReplyItems); $i++) {
        saveTaxForItem($orderItemsGroup['SltOrderId'], $orderItemsGroup[Items][$i]['EntityId'], $orderItemsGroup[Items][$i]['EntityType'],
                             $orderItemsGroup[Items][$i]['AdditionalInfo'], $taxReplyItems[$i]->totalTaxAmount);
      }
    }
    else {
        saveTaxForItem($orderItemsGroup['SltOrderId'], $orderItemsGroup[Items][0]['EntityId'], $orderItemsGroup[Items][0]['EntityType'],
                             $orderItemsGroup[Items][0]['AdditionalInfo'], $taxReplyItems->totalTaxAmount);
    }
  }
  catch (Exception $ex) {
    watchdog_exception('saveItemLevelTaxes', $ex);
    expertusErrorThrow($ex);    
  }
}

/*
 * saveTaxForItem() - Updates slt_order_items table to add tax for an entity
 * //UPDATE slt_order_items SET tax_amount=xtaxamount WHERE order_id= xorderid AND user_id=xuserid AND course_id=xcourseid AND class_id=xclassid;
 */
function saveTaxForItem($sltOrderId, $entityId, $entityType, $additionalInfo, $taxAmount) {
  expDebug::dPrint('$sltOrderId = ' . $sltOrderId, 5);
  expDebug::dPrint('$entityId = ' . $entityId, 5);
  expDebug::dPrint('$entityType = ' . $entityType, 5);
  expDebug::dPrint('$additionalInfo = ' . $additionalInfo, 5);
  expDebug::dPrint('$taxAmount = ' . $taxAmount, 5);
  try {
    $updateStmt = db_update('slt_order_items');
    $fields = array(
      'tax_amount'    => $taxAmount,
      'updated_by' => getSltpersonUserId(),
      'updated_on' => now()
    );
    $updateStmt->fields($fields);
    $updateStmt->condition('order_id', $sltOrderId, '=');
    //$updateStmt->condition('user_id', getSltpersonUserId(), '=');
    
    if (in_array($entityType, array('lrn_cls_dty_wbt', 'lrn_cls_dty_ilt', 'lrn_cls_dty_vcl', 'lrn_cls_dty_vod'))) {
      $updateStmt->condition('class_id', $entityId, '=');
      $updateStmt->condition('course_id', $additionalInfo, '=');
    }
    else {
      $updateStmt->condition('program_id', $entityId, '=');
    }
    expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
    $numUpdated = $updateStmt->execute();
    expDebug::dPrint(' $numUpdated = ' . $numUpdated);
  }
  catch (Exception $ex) {
    watchdog_exception('saveTaxForItem', $ex);
    expertusErrorThrow($ex);    
  }
}

/*
 * logTaxRequestResponse - Logs the tax calc request and response in slt_payment_log
 */
function logTaxRequestResponse($sltOrderId, $requestXML, $responseXML) {
  try {
    $insertStmt = db_insert('slt_payment_log');
    $fields = array(
                'order_id'    => $sltOrderId,
                'type'        => 'Tax',
                'paytime'     => now(),
                'requestxml'  => $requestXML,
                'responsexml' => $responseXML,
                'created_by'  => getSltpersonUserId(),
                'created_on'  => now(),
    						'updated_by'  => getSltpersonUserId(),
    						'updated_on'  => now()
              );
    $insertStmt->fields($fields);
    expDebug::dPrintDBAPI('$insertStmt obj = ' , $insertStmt);
      
    // Execute the insert statement. Fetch the new org id.
    $logId = $insertStmt->execute();
    expDebug::dPrint('$logId = ' . print_r($logId, true));
  }
  catch (Exception $ex) {
    watchdog_exception('logTaxRequestResponse', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * prepareTaxRequest() - Prepares the tax calculation request for sending to cybersource SOAP server
 */
function prepareTaxRequest($merchantId, $orderItemsGroupKey, $orderItemsGroup, $taxSettings) {
  try {
    expDebug::dPrint('$merchantId = ' . print_r($merchantId, true), 5);
    expDebug::dPrint('$orderItemsGroupKey = ' . $orderItemsGroupKey, 5);
    expDebug::dPrint('$orderItemsGroup = ' . print_r($orderItemsGroup, true), 5);
    expDebug::dPrint('$taxSettings = ' . print_r($taxSettings, true), 5);
    
    $request = new stdClass();
    $request->merchantID = $merchantId;
    $request->merchantReferenceCode = $orderItemsGroup['SltOrderId'] . $orderItemsGroupKey; // max length 50 chars restriction
    
    // Retaining the below three request parameters as was present in the earlier tax code
    $request->clientLibrary = "PHP";
    $request->clientLibraryVersion = phpversion();
    $request->clientEnvironment = php_uname();
  
    // Add tax service request parameters
    $request->taxService = new stdClass();
    $request->taxService->run = 'true'; // This is always true to indicate requesting tax service
    
    if ($orderItemsGroup['Address']['Country'] == 'US' || $orderItemsGroup['Address']['Country'] == 'CA') {
      addNexusStatesToTaxRequest($taxSettings->states, $taxSettings->nexus, $request->taxService);
    }
    else { // non-US and non-CA, add vat number(s) to service request
      addVATNumbersToTaxRequest($taxSettings->vat_numbers, $request->taxService);
    }
    
    // Add Billing address to request
    $request->billTo = new stdClass();
    addBillingAddressToTaxRequest($orderItemsGroup['Address'], $request->billTo);
  
    // Add the purchased items details to the request
    addLineItemsToTaxRequest($orderItemsGroup['Items'], $taxSettings->productCodes, $request);
  
    expDebug::dPrint('$request = ' . print_r($request, true), 5);
    return $request;
  }
  catch (Exception $ex) {
    watchdog_exception('prepareTaxRequest', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * addLineItemsToTaxRequest - Adds the purchased items details to the request
 */
function addLineItemsToTaxRequest($orderItemsList, $productCodes, &$request) {
  try {
    foreach ($orderItemsList as $key => $orderItem) {
      $requestItem = new stdClass();
      $requestItem->id = $orderItem['Index']; // The id field is undocumented but seems is required, otherwise for multiple items in request
                                              // SoapClient runTransaction() throws java.lang.NullPointerException.
      $requestItem->productSKU = generateSKUForTaxRequestLineItem($orderItem);
      $requestItem->productName = $orderItem['EntityTitle'];
      $requestItem->unitPrice = $orderItem['Price'];
      $requestItem->quantity = 1;
  
      addProductCodeToTaxRequestLineItem($orderItem, $productCodes, $requestItem);
      
      $request->item[] = $requestItem;
    }
  }
  catch (Exception $ex) {
    watchdog_exception('addLineItemsToTaxRequest', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * generateSKUForTaxRequestLineItem() - Generates a distinct sku for tax request line item using delivery type and entity id
 */
function generateSKUForTaxRequestLineItem($orderItem) {
  try {
    $sku = $orderItem['EntityId'];
    if (in_array($orderItem['EntityType'], array('cre_sys_obt_crt', 'cre_sys_obt_cur', 'cre_sys_obt_trn'))) {
      $sku .= 'trp';
    }
    else {
      $sku .= 'cls' . $orderItem['AdditionalInfo']; //The additional info is the course id
    }
  
    return $sku;
  }
  catch (Exception $ex) {
    watchdog_exception('generateSKUForTaxRequestLineItem', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * addProductCodeToTaxRequestLineItem($item) - When setup, adds product code to item details in request
 */
function addProductCodeToTaxRequestLineItem($orderItem, $productCodes, &$requestItem) {
  try {
    if (empty($productCodes)) {
      return;
    }
  
    if (in_array($orderItem['EntityType'], array('cre_sys_obt_crt', 'cre_sys_obt_cur', 'cre_sys_obt_trn'))) {
      $productCode = empty($productCodes['cre_sys_obt_trp'])? '' : $productCodes['cre_sys_obt_trp'];
    }
    else {
      $productCode = empty($productCodes[$orderItem['EntityType']])? '' : $productCodes[$orderItem['EntityType']];
    }
    
    if (empty($productCode)) {
      return;
    }
    
    $requestItem->productCode = $productCode;
  }
  catch (Exception $ex) {
    watchdog_exception('addProductCodeToTaxRequestLineItem', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * addBillingAddressToTaxRequest() - Adds billing address fields to the bill to component of request
 */
function addBillingAddressToTaxRequest($address, &$billTo) {
  try {
    $billTo->country = $address['Country'];
    
    if ($address['Country'] == 'US' || $address['Country'] == 'CA') {
      $billTo->state = $address['State'];
      $billTo->city = $address['City'];
      $billTo->postalCode = $address['Zipcode'];
    }
  }
  catch (Exception $ex) {
    watchdog_exception('addBillingAddressToTaxRequest', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * addVATNumbersToTaxServiceComponent() -  Adds VAT number(s) to tax service component of request.
 *                                         For single VAT number, uses 'sellerRegistration' param.
 *                                         For multiple VAT numbers, uses 'sellerRegistration0...N' param.
 */
function addVATNumbersToTaxRequest($vatNumbers, &$taxService) {
  try {
    $vatNumbersList = explode(',', $vatNumbers);
    if (count($vatNumbersList) == 1) {
      $taxService->sellerRegistration = $vatNumbersList[0];
    }
    else {
      foreach ($vatNumbersList as $key => $vatNumber) {
        $param = 'sellerRegistration' . $key;
        $taxService->$param = $vatNumber;
      }
    }
  }
  catch (Exception $ex) {
    watchdog_exception('addVATNumbersToTaxRequest', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * addNexusStatesToTaxServiceComponent() - Adds nexus/noNexus states to tax service component of request
 */
function addNexusStatesToTaxRequest($states, $nexusFlag, &$taxService) {
  try {
    $statesList = explode(',', $states);
    if (count($statesList) > 0) { // add nexus/noNexus states to tax service request if present in tax settings
      if ($nexusFlag == 'yes') {
        $param = 'nexus';
      }
      else {
        $param = 'noNexus';
      }
      $taxService->$param = '';
      foreach ($statesList as $state) {
        $taxService->$param .= $state . ' '; // a single space separates each state in the list
      }
      $taxService->$param = trim($taxService->$param); // remove the extra space at the end
    }
  }
  catch (Exception $ex) {
    watchdog_exception('addNexusStatesToTaxRequest', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getTaxSettingsForCountry() - Fetches the country specific tax settings
 */
function getTaxSettingsForCountry($countryCode) {
  try {
    // Fetch core tax settings
    $select = db_select('slt_tax_settings', 'sts');
    $select->addField('sts', 'id', 'id');
    $select->addField('sts', 'states', 'states');
    $select->addField('sts', 'nexus', 'nexus');
    $select->addField('sts', 'status', 'status');
    $select->addField('sts', 'vat_numbers', 'vat_numbers');
    $select->condition('sts.country', $countryCode, '=');
    expDebug::dPrintDBAPI('$select', $select);
    $taxSettings = $select->execute()->fetch();
    expDebug::dPrint('$taxSettings = ' . print_r($taxSettings, true));
    
    if (empty($taxSettings)) {
      expDebug::dPrint('No tax settings for country ' . $countryCode, 5);
      return FALSE; // no tax settings for country, assuming no tax applies
    }
    
    if ($taxSettings->status != 'cme_tax_sts_atv') {
      expDebug::dPrint('The tax settings for the country are inactive '  . $countryCode, 5);
      return FALSE; // tax settings for country set to inactive, assuming no tax applies
    }
    
    if (($countryCode != 'US' && $countryCode != 'CA') && empty($taxSettings->vat_numbers)) {
      expDebug::dPrint('VAT number for non-US non-CA country not specified in tax settings ' . $countryCode, 5);
      return FALSE; // Though an ERROR condition, this would be handled and prevented in tax settings admin screen.
    }
    
    // Fetch tax product codes
    $select = db_select('slt_tax_product_code_mapping', 'stpcm');
    $select->addField('stpcm', 'delivery_type', 'delivery_type');
    $select->addField('stpcm', 'tax_product_code', 'tax_product_code');
    $select->condition('stpcm.tax_settings_id', $taxSettings->id, '=');
    expDebug::dPrintDBAPI('$select', $select);
    $productCodes = $select->execute()->fetchAllKeyed();
    expDebug::dPrint('$productCodes = ' . print_r($productCodes, true));
    
    $taxSettings->productCodes = $productCodes;
    
    expDebug::dPrint('$taxSettings with product codes = ' . print_r($taxSettings, true), 4);
    return $taxSettings;
  }
  catch (Exception $ex) {
    watchdog_exception('getTaxSettingsForCountry', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * segregateOrderItemsForTax() - Segregate order items for tax calculation
 */
function segregateOrderItemsForTax($order) {
  try {
   
    $orderItemsGroupsList = array();
    foreach ($order->products as $productIndex => $product) {
      $nodeId = $product->nid;
  
      // Fetch discount for product and calculate final price after discount
      $price = $product->price;
      if($order->data['order_create_from'] == 'admin_checkout'){
      	$discount = getAdminClassDiscountForTax($nodeId, $order->order_id);
      }else{
      	$discount = getClassDiscountForTax($nodeId, $order->uid);
      }
      expDebug::dPrint('$discount = ' . print_r($discount, true), 5);
      $finalPrice = $price - $discount;
      if ($finalPrice < 0) {
        $finalPrice = 0;
      }
  
      // Fetch product details from DB - delivery type, 
      $productDetails = getCatalogInfofromNodeId($nodeId);
      expDebug::dPrint('$productCatalogInfo = ' . print_r($productCatalogInfo, true), 5);
      
      $deliveryType = $productDetails->DeliveryTypeId;
      
      $sltOrderId = $_SESSION['slt_order_id'];
      if($order->data['order_create_from'] == 'admin_checkout'){
      	$sltOrderId = $_SESSION['slt_admin_orderid'];
      }
      
      //Seggregate based on delivery type and ILT location
      if ($deliveryType == 'lrn_cls_dty_ilt') {
        if (!isset($orderItemsGroupsList['ILT' . $productDetails->LocId])) {
          $orderItemsGroupsList['ILT' . $productDetails->LocId] = array();
        }
        $orderItemsGroup = &$orderItemsGroupsList['ILT' . $productDetails->LocId];
        
        $orderItemsGroup['SltOrderId'] = $sltOrderId;
        
        $orderItemsGroup['Items'][] = array('Index' => $productIndex,
                                            'EntityType' => $deliveryType,
                                            'EntityId' => $productDetails->ClassId,
                                            'EntityTitle' => $productDetails->ClassTitle,
                                            'AdditionalInfo' => $productDetails->CourseId,
                                            'Price' => $finalPrice);
  
        if (!isset($orderItemsGroup['TotalAmount'])) {
          $orderItemsGroup['TotalAmount'] = $finalPrice;
        }
        else {
          $orderItemsGroup['TotalAmount'] += $finalPrice;
        }
       
        if (!isset($orderItemsGroup['Address'])) {
          $orderItemsGroup['Address'] = array('Country' => $productDetails->LocCountry,
                                              'State' => getISOCodeForTax($productDetails->LocState, 'slt_state_code', $productDetails->LocCountry), 
                                              'City' => $productDetails->LocCity,
                                              'Zipcode' => $productDetails->LocZip);
        }
      }
      else { // wbt, vc, 3 tp types
        if (!isset($orderItemsGroupsList['NONILT'])) {
          $orderItemsGroupsList['NONILT'] = array();
        }
        $orderItemsGroup = &$orderItemsGroupsList['NONILT'];
        
        $orderItemsGroup['SltOrderId'] = $sltOrderId;
        
        $orderItemsGroup['Items'][] = array('Index' => $productIndex,
                                            'EntityType' => $deliveryType,
                                            'EntityId' => $productDetails->ClassId,
                                            'EntityTitle' => $productDetails->ClassTitle,
                                            'AdditionalInfo' => $productDetails->CourseId,
                                            'Price' => $finalPrice);
        if (!isset($orderItemsGroup['TotalAmount'])) {
          $orderItemsGroup['TotalAmount'] = $finalPrice;
        }
        else {
          $orderItemsGroup['TotalAmount'] += $finalPrice;
        }
       
        if (!isset($orderItemsGroup['Address'])) {
          // Fetch required address fields from uc_orders table
          $orderItemsGroup['Address'] = array('Country' => getISOCodeForTax($order->billing_country, 'uc_country_id'),
                                              'State' => getISOCodeForTax($order->billing_zone, 'uc_state_id', $order->billing_country),
                                              'City' => $order->billing_city,
                                              'Zipcode' => $order->billing_postal_code);
        }
      }
    } // end foreach
    
    expDebug::dPrint('$orderItemsGroupsList = ' . print_r($orderItemsGroupsList, true), 5);
    return $orderItemsGroupsList;
  }
  catch (Exception $ex) {
    watchdog_exception('segregateOrderItemsForTax', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getISOCodeForTax() - Fetches from db or adjusts  as approriate country or state ISO codes
 */
function getISOCodeForTax($id, $idType, $arg1 = NULL) {
  try {
    switch ($idType) {
      case 'uc_country_id':
        // Fetch two-character iso code from uc_countries table
        $select = db_select('uc_countries', 'ucc');
        $select->addField('ucc', 'country_iso_code_2', 'iso_code');
        $select->condition('ucc.country_id', $id, '=');
        expDebug::dPrintDBAPI('$select', $select);
        $isoCode = $select->execute()->fetchField();
        expDebug::dPrint('$isoCode = ' . $isoCode);
        break;
        
      case 'uc_state_id':
        // State field is required and used only for US and CA
        // $arg1 is uc country id
        if ($arg1 == 840 || $arg1 == 124) { // 840 = US, 124 = CA   
          // Fetch the two-characer iso code from uc_zones table for US and CA
          $select = db_select('uc_zones', 'ucz');
          $select->addField('ucz', 'zone_code', 'iso_code');
          $select->condition('ucz.zone_id', $id, '=');
          $select->condition('ucz.zone_country_id', $arg1, '=');
          expDebug::dPrintDBAPI('$select', $select);
          $isoCode = $select->execute()->fetchField();
          expDebug::dPrint('$isoCode = ' . $isoCode);
          break;
        }
      case 'slt_state_code':
        // slt_states table lists 'new foundland' (NL) and 'new foundland and labrador' (05) separately. However, in uc_zones table only
        // new foundland and labrador is present with code NL. Assuming NL as code for both 'new foundland' and 'new foundland and labrador'
        // for tax calc.
        // $arg1 is 2 letter country code
        if ($arg1 == 'CA' && $id == '05') { // 'CA' = Canada, '05' = New Foundland and Labrador
          $isoCode = 'NL';
        }
        else if ($arg1 == 'CA' || $arg1 == 'US') {
          // Apart from two alphabets code, slt_state also has numeric state codes. This code assumes slt_location table will always have
          // two alphabet code for US ans CA states, except for 'new foundland and labrador', which is taken care of in the above if clause.
          $isoCode = $id;
        }
        
    } // end switch
    
    if (empty($isoCode)) {
      $isoCode = '';
    }
    
    expDebug::dPrint('returning $isoCode = ' . $isoCode, 4);
    return $isoCode;
  }
  catch (Exception $ex) {
    watchdog_exception('getISOCodeForTax', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getClassDiscountForTax() - Fetch class discount for tax
 */
function getClassDiscountForTax($nodeId, $drupalUid) {
  try {
    $select = db_select('uc_cart_products', 'uccp');
    $select->addField('uccp', 'discount', 'discount');
    $select->condition('uccp.nid', $nodeId, '=');
    $select->condition('uccp.cart_id', $drupalUid, '=');
    expDebug::dPrintDBAPI('$select', $select);
    
    $discount = $select->execute()->fetchField();
    expDebug::dPrint('$discount = ' . $discount);
    
    return $discount;
  }
  catch (Exception $ex) {
    watchdog_exception('getClassDiscountForTax', $ex);
    expertusErrorThrow($ex);
  }
}


function getAdminClassDiscountForTax($nid, $order_id){
	$data = db_query("SELECT data FROM {uc_order_products} WHERE order_id = :id AND nid = :nid", array(':id' => $order_id, ':nid' => $nid))->fetchField();
	$data = unserialize($data);
	expDebug::dPrint('checkign the discount data'.print_r($data,true));
	return $data['discount_data'];
}
function getTaxSetting(){
	// Fetch core tax settings
	$select = db_select('slt_tax_settings', 'sts');
	$select->addField('sts', 'id', 'id');
	$select->addField('sts', 'country', 'country');
	$select->addField('sts', 'states', 'states');
	$select->addField('sts', 'nexus', 'nexus');
	$select->addField('sts', 'status', 'status');
	$select->addField('sts', 'vat_numbers', 'vat_numbers');
	$select->condition('sts.status', 'cme_tax_sts_atv', '=');
	expDebug::dPrintDBAPI('$select', $select);
	$taxSettings = $select->execute()->fetch();

	// Fetch tax product codes
	$select = db_select('slt_tax_product_code_mapping', 'stpcm');
	$select->addField('stpcm', 'delivery_type', 'delivery_type');
	$select->addField('stpcm', 'tax_product_code', 'tax_product_code');
	$select->condition('stpcm.tax_settings_id', $taxSettings->id, '=');
	expDebug::dPrintDBAPI('$select', $select);
	$productCodes = $select->execute()->fetchAllKeyed();
	expDebug::dPrint('$productCodes = ' . print_r($productCodes, true));

	$taxSettings->productCodes = $productCodes;

	expDebug::dPrint('$taxSettings = ' . print_r($taxSettings, true),4);

	return $taxSettings;
}
?>