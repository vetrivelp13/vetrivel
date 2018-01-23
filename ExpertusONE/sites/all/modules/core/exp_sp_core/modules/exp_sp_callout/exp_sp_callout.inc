<?php
/**
*  saveTaxCalculationTables($result,$order,$orderRequest); This function is used to store the tax values.This Funtion should be used when the tax calculated successfully.(this is the helper function present in exp_sp_callout.inc).
*  clearTaxCalculationTables($sltOrderId,$order_id); This function is used to restore the tax values to zero.This Funtion should be used when the tax calculated fail.(this is the helper function present in exp_sp_callout.inc).
*  logTaxRequestResponse($SltOrderId, http_build_query($orderRequest), $result); This function is used to store the log of tax response.This Funtion should be used when the tax calculated successfully or else failed.
*
*

* @param object $order Containig current order related variables like uc_order_id, billing details, item details, user details asn so on.
* @param array $orderRequest Containig segregated line of items like ILT and NONILT.
* @param array $taxSettings This array contain the our site setting tax details.
* @param array $result This array contain the calculated tax details in repective format.
*
*
* */
function updateTaxInformation($orderRequest,$order,$taxSettings,$result)
{
	expDebug::dPrint('Sales Tax response--> ' . print_r($result, true), 4);
	foreach ($result as $k => $v){
		$SltOrderId = $v['SltOrderId'];
		break;
	}
	expDebug::dPrint('$SltOrderId--> ' . print_r($SltOrderId, true), 4);
	//log calculated tax response in  slt_payment_log
	logTaxRequestResponse($SltOrderId, http_build_query($orderRequest), http_build_query($result));

	if (count($result)<1) {
		// clear item level calculated tax
		clearTaxCalculationTables($orderRequest['SltOrderId'],$order->order_id);
		return 0;
	}
	expDebug::dPrint('After TaxCalculationTables:' . print_r($orderRequest, 1), 4);
	// Save item level calculated tax
	saveTaxCalculationTables($result,$order,$orderRequest);
}

/**
 * Perform tax amount clear on required tables
 * @param array $replayList
 */
function clearTaxCalculationTables($sltOrderId,$order_id) {
	try {
		expDebug::dPrint('clearTaxCalculationTables:' . $sltOrderId, 4);
		expDebug::dPrint('clearTaxCalculationTables:$order_id' . $order_id, 4);
		if ($sltOrderId == '') {
			return false;
		}
		// update slt_order and slt_order_items table
		$updateStmt = db_update('slt_order');
		$updateStmt->fields(array('order_tax_amt' => 0,
				'updated_by' => getSltpersonUserId(),
				'updated_on' => now()
		)
		);
		$updateStmt->condition('id', $sltOrderId, '=');
		$updateStmt->condition('user_id', getSltpersonUserId(), '=');
		//expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
		$updateStmt->execute();

		$updateStmt = db_update('slt_order_items');
		$updateStmt->fields(array('tax_amount' => 0,
				'updated_by' => getSltpersonUserId(),
				'updated_on' => now()
		)
		);
		$updateStmt->condition('order_id', $sltOrderId, '=');
		$updateStmt->condition('user_id', getSltpersonUserId(), '=');
		//expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
		$updateStmt->execute();


		// remove line item in uber cart table
		// Add the tax line item to the uber cart order
		db_query('DELETE FROM {uc_order_line_items} WHERE order_id = :oid AND type = :typ',
			                array(':oid' => $order_id, ':typ' => 'cybersource_tax'));
	} catch(Exception $ex) {
		watchdog_exception('clearTaxCalculationTables', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Perform tax amount updates on required tables
 * @param array $replayList
 */
function saveTaxCalculationTables($replayList,$order,$orderRequest) {
	try {
		expDebug::dPrint('saveTaxCalculationTables:' . print_r($replayList, 1), 4);
		require_once DRUPAL_ROOT . '/sites/all/modules/core/exp_sp_commerce/modules/ubercart/uc_cart/uc_cart.pages.inc';

		$totalTaxAmount = $replayList['wholeTaxAmount'];
		// update slt_order and slt_order_items table
		$updateStmt = db_update('slt_order');
		$updateStmt->fields(array('order_tax_amt' => $totalTaxAmount,
				'updated_by' => getSltpersonUserId(),
				'updated_on' => now()
		)
		);
		$updateStmt->condition('uc_order_id', $order->order_id, '=');
		$updateStmt->execute();
		$weight = 0;
		foreach ($replayList as $replayKey => $replayValue) {
			$sltOrderId = $replayValue['SltOrderId'];
			$tempCount = count($replayValue['Items']);
			if ($tempCount>0) {
				for($i = 0; $i < $tempCount; $i++) {
					saveTaxForItem($sltOrderId, $orderRequest[$replayKey]['Items'][$i]['EntityId'], $orderRequest[$replayKey]['Items'][$i]['EntityType'],
							$orderRequest[$replayKey]['Items'][$i]['AdditionalInfo'], $replayValue['Items'][$i]['totalTaxAmount']);
				}
				$weight = $weight + $i;
			}
			else {
				saveTaxForItem($sltOrderId,$orderRequest[$replayKey]['Items']['EntityId'],$orderRequest[$replayKey]['Items']['EntityType'],
							$orderRequest[$replayKey]['Items']['AdditionalInfo'], $replayValue['Items'][$i]['totalTaxAmount']);
			}
		}
		// Add the tax line item to the uber cart order
		uc_order_line_item_add (
				$order->order_id, // uc order ID.
				'cybersource_tax', // line item ID
				t('Tax'), // line item name (or line item text)
				$totalTaxAmount, // line item value
				$weight, // line item weight
				$replayList
		);
	} catch(Exception $ex) {
		watchdog_exception('saveTaxCalculationTables', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getISOCodeForPayment() - Fetches from db or adjusts  as approriate country or state ISO codes
*/
function getISOCodeForPayment($id, $idType, $arg1 = NULL) {
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

function getPersonCompanyfromOrderId($orderId = '') {
	try {
		if ($orderId == '') {
			return '';
		}
		$slt_user_id  = db_query("SELECT user_id FROM slt_order WHERE uc_order_id = '".$orderId."' limit 1")->fetchField();
		$org_company = db_query("SELECT org.name FROM slt_person usr INNER JOIN slt_organization org  ON (usr.org_id=org.id) WHERE usr.id=:user LIMIT 1", array(':user' => $slt_user_id))->fetchField();
		return $org_company;
	} catch(Exception $ex) {
		watchdog_exception('getSltOrderId', $ex);
		expertusErrorThrow($ex);
	}
}

function getSltOrderIdfromUberCartId($ubercartId) {
	try {
		if ($ubercartId == '') {
			return '';
		}
		$slt_order_id  = db_query("SELECT id FROM slt_order WHERE uc_order_id = '".$ubercartId."' limit 1")->fetchField();
		return $slt_order_id;
	} catch(Exception $ex) {
		watchdog_exception('getSltOrderId', $ex);
		expertusErrorThrow($ex);
	}
}
/**
 * Used to save the transaction log based on the payment method.
 *
 * @param array $result contain the result values
 * @param string $paymethod payment method used in the transcation
 *
 * @return null
 */
function processPaymentsTransactions($result = '', $paymethod) {
	try {
		expDebug::dPrint('inside processSuccessPayments' . print_r($result, 1), 4);
		// Collect the variables needed
		$order_id = $result['orderId'];
		$order = uc_order_load($order_id);

		if ($result['success'] === TRUE) {
			switch($paymethod) {
				case 'credit':
					// Add credit card based transaction logs
					// uc payment recipts
					uc_payment_enter($order_id, $paymethod, $result['transaction']->amount, empty($result['uid']) ? 0 : $result['uid'], empty($result['additionalInfo']) ? '' : $result['additionalInfo'], empty($result['comment']) ? '' : $result['comment']);
					break;
				case 'po':
					db_merge('uc_payment_po')
					->key(array('order_id' => $order_id))
					->updateFields(array(
					'po_number' => empty($result['po_number']) ? '' : $result['po_number'],
					))
					->insertFields(array(
					'order_id' => $order->order_id,
					'po_number' => empty($result['po_number']) ? '' : $result['po_number'],
					))
					->execute();
					if (!empty($order->payment_details)) {
						_uc_po_save_po_data_to_order($order->payment_details, $order->order_id);
					}
					break;
			}
		}


	} catch(Exception $ex) {
		watchdog_exception('processSuccessPayments', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Capture transaction log for the refund
 *
 * @param 	object 	$replyDet	object contains essential information to save log
 * @return int 		$orderid 	returns orderid on success or null on failure.
 */
function processRefundTransactionLog($replyDet){
	try{
		$vLMSUserId     = getSltpersonUserId();
		$orderId        = $replyDet->orderId;
		$vreqxml        = urlencode($replyDet->vreqxml);
		$type			= "CC_REFUND";
		$vrespxml       = urlencode($replyDet->vrespxml);//urlencode($respxml);

		try
		{
			$dao = new AbstractDAO();
			$dao->connect();
			$dao->beginTrans();
			$vFetchSQL ="call slp_createpayment_log('$orderId','$type','$vreqxml','$vrespxml','$vLMSUserId')";
			expDebug::dPrint("Cart payment log  DAO  ==>".$vFetchSQL, 4);

			$dao->execute($vFetchSQL);
			$dao->commitTrans();
			$dao->closeconnect();
			return $orderId;
		}catch (Exception $e)
		{
			expDebug::dPrint($e,1);
		}
	}catch(Exception $ex){
		expDebug::dPrint('Error in refund transaction log entry'.$ex,1);
	}
}
/**
 * short desc
 *
 * Function used to process the payment success/failure transactions in the LMS based on the result value from third party.
 * Based on the success/failure redirect user to the respective pages for API payment method.
 *
 * @param mixed[] $result variable contain the success result and logs
 *
 * 		$result['success']				- Set result value based on the decision code value (boolean)
 *		$result['orderId']				- lms order id value (int)
 *		$result['paymethod']			- payment method used in the third party(string) (eg: credit, po)
 *		$result['uid']					- ordered by the user in lms (int)
 *		$result['additionalInfo'] 		- if order has contain any data info (array)
 *		$result['comment']				- additional comments (string)
 *
 *		// Online transaction
 *		$transaction->requestID 		- request number used in the payment gateway(string)
 *		$transaction->currency			- currency used for this transaction(string)
 *		$transaction->amount	 		- total order amount (float);
 *		$transaction->requestQuery		- request variable/object
 *		$transaction->responseResult	- response variable/object;
 *
 *		//Optional params
 *		$transaction->taxAmount	 		- total tax amount (float);
 *		$transaction->shipAmount		- total shipment amount (float);
 *		$transaction->reasonCode 		- any reason/success code from payment gateway (int);
 *		$transaction->transactionType	- transcation type used for this transction (string);
 *		$transaction->decision			- transaction result value (string)
 *		$transaction->avsCode 			- if transaction contain any avscode (string)
 *		$transaction->cvCode 			- if transaction contain any cvcode (string)
 *
 *		// Purchase order transaction
 *		$transaction->poNumber	 		- if transaction has purchase order. metion po number (int)
 *
 * It returns the below variable, if the payment method native used in the callout feature.
 * @return array $resultFinal
 */
function completeCalloutPaymentResponse($result) {
	try {
		global $user;

		if (empty($result)) { expDebug::dPrint('Complete callout result is empty'); }

		expDebug::dPrint('received Final result variable: ' . print_r($result, 1), 4);
		$order_id = $result['orderId'];
		$transaction = $result['transaction'];
		$order = uc_order_load($order_id);


		/**
		 * Payment log table.
		 */

		$slt_order_id = getSltOrderIdfromUberCartId($order_id);
		$redirectPath = ($order->data['order_create_from'] == 'admin_checkout') ? 'administration/order/finish' : 'cart/checkout/complete';
		$lmsUserId = getDrupalIdOfUser($order->uid);
		$cartPaymentlogUpdate ="call slp_createpayment_log('$slt_order_id','CC', 'not available','$transaction->responseResult','$lmsUserId')";
		db_query($cartPaymentlogUpdate);

		/**
		 * Create the order and payment ledger comments.
		 */
		$o_comment = t('<b>@type:</b> @amount<br /><b>Decision: @decision</b><br /><b>Reason:</b> !reason',
				array('@type' => $transaction->transactionType, '@amount' => uc_currency_format($transaction->amount), '@decision' => $transaction->decision, '!reason' => _uc_cybersource_parse_reason_code($transaction->reasonCode)));
		$p_comment = t('<b>@type:</b><br />@id<br />@decision, Reason: !reason',
				array('@type' => $transaction->transactionType, '@id' => $transaction->requestID, '@decision' => $transaction->decision, '!reason' => $transaction->reasonCode));

		if (!empty($transaction->avsCode)) {
			$o_comment .= '<br />' . t('<b>AVS:</b> @avs', array('@avs' => _uc_cybersource_parse_avs_code($transaction->avsCode)));
			$p_comment .= t(', AVS: @avs', array('@avs' => $transaction->avsCode));
		}
		if (!empty($transaction->cvCode)) {
			$o_comment .= '<br />' . t('<b>CVV:</b> @cvv', array('@cvv' => _uc_cybersoure_parse_cvv_code($transaction->cvCode)));
			$p_comment .= t(', CVV: @cvv', array('@cvv' => $transaction->cvCode));
		}
		uc_order_comment_save($order_id, $user->uid, $o_comment, 'admin');

		/**
		 * prepare return response array
		 */
		if (isset($result)) {
			if ($transaction->decision == 'ACCEPT') {
				$resultFinal = array(
						'success' => TRUE,
						'comment' => $p_comment,
						'message' => $o_comment,
						'reasonCode'  =>$transaction->reasonCode,
						'uid' => $user->uid,
				);
			} else {
				// consider as failure
				$resultFinal = array(
						'success' => FALSE,
						'comment' => $p_comment,
						'message' => _uc_cybersource_parse_reason_code($transaction->reasonCode)	,
						'reasonCode'  =>$transaction->reasonCode,
						'uid' => $user->uid,
				);
			}
			$result['comment'] = $p_comment;
		} else {
			$resultFinal = array(
					'success' => FALSE,
					'message' => t(variable_get('uc_credit_fail_message',
							'We were unable to process your credit card payment. Please verify your card details and try again.  If the problem persists, contact us to complete your order.'))
			);
		}


		// Prepare result array based on the payment method used on the external site
		if ($resultFinal['success'] === TRUE) {

			if (!empty($result['paymethod'])) {
				$result = processPaymentsTransactions($result, $result['paymethod']);
			} else {
				watchdog('uc_payment', 'Payment failed for order @order_id: @message', array('@order_id' => $order_id, '@message' => $result['message']), WATCHDOG_WARNING, l(t('view order'), 'admin/store/orders/' . $order_id));
			}

			$payCallout = getCalloutEntityInfo('cme_pmt');

			if ($payCallout['method_type'] == 'native') {
				return $resultFinal;
			} else {
				$_SESSION['callout_response'] = true;
				/*--- Add discount details in internal tables ---*/
				addProductDiscounts($order);
				if ($order->data['order_create_from'] == 'admin_checkout') {
					$_SESSION['cart_admin_order'] = $order_id;
					$_SESSION['slt_admin_orderid'] = $slt_order_id;
					$_SESSION['do_admin_complete'] = TRUE;
					drupal_goto($redirectPath);
				} else {
					// Include the needed files to generate complete page
					require_once DRUPAL_ROOT."/sites/all/modules/core/exp_sp_commerce/modules/ubercart/uc_cart/uc_cart.pages.inc";
					$_SESSION['cart_order'] = $order_id;
					$_SESSION['slt_order_id'] = $slt_order_id;
					$_SESSION['do_complete'] = TRUE;
					drupal_goto($redirectPath);
				}
			}

		} else {
			$redirectPath = ($order->data['order_create_from'] == 'admin_checkout') ? 'administration/order/pay' : 'cart/checkout/review';
			drupal_set_message($resultFinal['message']);
			drupal_goto($redirectPath);
		}
	} catch(Exception $ex) {
		watchdog_exception('completeCalloutPaymentResponse', $ex);
		expertusErrorThrow($ex);
	}
}

function organizeOrderDetails($order){
	//expDebug::dPrint("organize the order detyails".print_r($order,true),4);

	$modOrder = new stdClass();
	$orderDet = array();

	//order details array
	$order_details = array();
	$order_details['order_id'] = $order->order_id;
	$order_details['order_status'] = $order->order_status;
	$order_details['order_currency'] = $order->currency_type;  // default currency used in the site
	$order_details['order_total'] = convertCurrencyFormat($order->order_total);
	$order_details['created_time'] = $order->created;
	$order_details['modified_time'] = $order->modified;
	$order_details['order_ipaddress'] = $order->host;

	// Process the order items
	$order_details['order_items'] = $order->products;
	foreach($order_details['order_items'] as $element) {
		unset($element->manufacturer, $element->model, $element->weight, $element->data);
		$element->cost = convertCurrencyFormat($element->cost);
		$element->price = convertCurrencyFormat($element->price);
		$element->tax = getProductTaxAmount($element->order_id, $element->classId);
	}

	// Process the line items
	$order_details['line_items'] = $order->line_items;
	foreach($order_details['line_items'] as $key => $element) {
		unset($order_details['line_items'][$key]['weight'], $order_details['line_items'][$key]['data']);
		$order_details['line_items'][$key]['amount'] = convertCurrencyFormat($element['amount']);
	}
	$orderDet['order_details'] = $order_details;

	// User Details array
	$userDetails = array();
	$userDetails['user_id'] = $order->uid;
	$userDetails['user_email'] = $order->primary_email;
	$orderDet['user_details'] = $userDetails;

	// Billing Details array
	$billingDetails = array();
	$billingDetails['billing_first_name'] = $order->billing_first_name;
	$billingDetails['billing_last_name'] = $order->billing_last_name;
	$billingDetails['billing_phone'] = $order->billing_phone;
	$billingDetails['billing_company'] = $order->billing_company;
	$billingDetails['billing_street1'] = $order->billing_street1;
	$billingDetails['billing_street2'] = $order->billing_street2;
	$billingDetails['billing_city'] = $order->billing_city;
	$billingDetails['billing_zone'] = $order->billing_zone;
	$billingDetails['billing_postal_code'] = $order->billing_postal_code;
	$billingDetails['billing_country'] = $order->billing_country;
	$orderDet['billing_details'] = $billingDetails;

	if(!empty($order->payment_method)){
		// Payment details array
		$paymentDetails = array();
		$paymentDetails['payment_method'] = $order->payment_method;
		$paymentDetails['card_details'] = $order->payment_details;
		$orderDet['payment_details'] = $paymentDetails;
	}

	// Additional details array
	$additionalDet = array();
	$additionalDet['data'] = $order->data;
	$additionalDet['discounts_codes_used'] = $order->uc_discounts_codes;
	$additionalDet['rdf_mapping'] = $order->rdf_mapping;
	$orderDet['additional_details'] = $additionalDet;
	//expDebug::dPrint("After modified the order variable".print_r($orderDet,true),4);
	$modOrder = (object) $orderDet;
	expDebug::dPrint("After modified the order as stdobject".print_r($modOrder,true),4);

	return $modOrder;
}

function convertCurrencyFormat($amount = '', $decimalPoints = 2){
	try {
		$dblamount = (double) $amount;
		//44546: In Callout payment calculation is "Active" if its order completed incorrectly showing status in order screen "Payment Received"
		// Check the negative value
// 		if ($dblamount < 0) {
// 			return number_format(abs($dblamount), $decimalPoints);
// 		} else {
// 			return number_format($dblamount, $decimalPoints);
// 		}
		return number_format($dblamount, $decimalPoints);
	} catch (Exception $ex) {
		expDebug::dPrint("Error in convertCurrencyFormat".$e->getMessage());
	}
}

function getProductTaxAmount($order_id, $class_id) {
	try {
		$taxAmount = db_query("SELECT sl_item.tax_amount FROM slt_order_items sl_item INNER JOIN slt_order sl_order ON sl_order.id = sl_item.order_id
				WHERE sl_order.uc_order_id = :order_id AND sl_item.class_id = :class_id", array(":order_id" => $order_id, ":class_id" => $class_id))->fetchField();
		return convertCurrencyFormat($taxAmount);
	} Catch(Exception $e) {
		expDebug::dPrint("Error in getProductTaxAmount".$e->getMessage());
	}
}

function addProductDiscounts($order) {
	try {
		global $user;
		if($order->data['order_create_from'] == 'admin_checkout'){
			if(is_array($order->products)) {
				foreach($order->products as $key => $product) {
					if(isset($_SESSION['admin_discount'][$product->nid])) {
						db_query('DELETE FROM {slt_class_discount} WHERE user_id = :uid AND nid = :nid AND order_id = :oid',array(':uid' => $order->uid,':nid' => $product->nid,':oid' => $order->order_id));
						db_query("INSERT INTO {slt_class_discount} (user_id, nid, order_id, discount) VALUES(:uid, :nid, :oid, :discount)",
								array(':uid'=>$order->uid, ':nid'=>$product->nid, ':oid'=>$order->order_id, ':discount'=>$_SESSION['admin_discount'][$product->nid])
						);
					}
				}
			}
			if(isset($_SESSION['admin_discount'])){
				unset($_SESSION['admin_discount']);
			}
		}
		else{
			if(is_array($order->products)) {
				foreach($order->products as $key => $product) {
					if(isset($_SESSION['discount'][$product->nid])) {
						db_query('DELETE FROM {slt_class_discount} WHERE user_id = :uid AND nid = :nid AND order_id = :oid',array(':uid' => $user->uid,':nid' => $product->nid,':oid' => $order->order_id));
						db_query("INSERT INTO {slt_class_discount} (user_id, nid, order_id, discount) VALUES(:uid, :nid, :oid, :discount)",
								array(':uid'=>$user->uid, ':nid'=>$product->nid, ':oid'=>$order->order_id, ':discount'=>$_SESSION['discount'][$product->nid])
						);
					}
				}
			}
			if(isset($_SESSION['discount'])){
				unset($_SESSION['discount']);
			}
		}
	} Catch(Exception $ex) {
		expDebug::dPrint("Error in addProductDiscounts".$ex->getMessage());
	}
}
/* Currency Conversion rate update - Callout */
function updateCurrencyConversionRate($response,$apifrom){
	$loggedUserId = getSltpersonUserId();
	$result = json_decode($response);
	expDebug::dPrint('updateCurrencyConversionRate:' . $apifrom .'---'. print_r($result, 1), 3);
	$from = $result->from;
	$to_currency_list = $result->to;
	if(count($to_currency_list) > 0){
	foreach($to_currency_list as $toval) {
		$to_currency=$toval->quotecurrency;
		expDebug::dPrint('updateCurrencyConversionRate comb :' . $from .'---'. $to_currency, 3);
		$codeexist=checkCurrencyExist($from,$to_currency);
		$rate=$toval->mid;
		if($codeexist > 0){
			$select = db_update('slt_currency_conversion_details');
			$select->fields(array('rate'=>roundCurrency($rate),'updated_by'=>$loggedUserId,'updated_on'=>date('Y-m-d H:i:s')));
			$select->condition('base_type', $from, '=');
			$select->condition('convert_type', $to_currency, '=');
			expDebug::dPrintDBAPI('updateCurrencyConversionRate Code', $select);
			$select->execute();
			$objectId = get_slt_profile_entity_id($from);
			$msgsave=$from.'-Update Currency conversion rate from live url -'. $apifrom;
			add_audit_trail_entry($loggedUserId, $objectId, 'cre_sys_crn', 'exp_sp_callout', 'updateCurrencyConversionRate',$msgsave,$loggedUserId,'Y','Y');
		}else{
			$insertstmt = db_insert('slt_currency_conversion_details');
			$fields = array(
					'base_type'         => $from,
					'convert_type'      => $to_currency,
					'rate'              => roundCurrency($rate),
					'created_by'        => $loggedUserId,
					'created_on'        => date('Y-m-d H:i:s')
			);
			$insertstmt->fields($fields);
			expDebug::dPrintDBAPI('list of slt_currency_conversion_details record ', $insertstmt);
			$insertstmt->execute();
			/* Update slt_profile_list_items table attr3 value */
			$select = db_update('slt_profile_list_items');
			$select->fields(array('attr3'=>'Y'));
			$select->condition('attr1', $from, '=');
			expDebug::dPrintDBAPI('call out list of updateCurrencyList Code', $select);
			$select->execute();
			$objectId = get_slt_profile_entity_id($from);
			$msgsave=$from.'-Insert Currency conversion rate from live url -'. $apifrom;
			add_audit_trail_entry($loggedUserId, $objectId, 'cre_sys_crn', 'exp_sp_callout', 'updateCurrencyConversionRate',$msgsave,$loggedUserId,'N','Y');
			/* Update slt_profile_list_items table attr3 value */
		}
	}
}

}
function roundCurrency($value, $precision = 11) {
	return round($value, $precision);
}
function checkCurrencyExist($basetype,$converttype){
	try{
		$qry="SELECT rate FROM slt_currency_conversion_details where base_type='$basetype' AND convert_type='$converttype'";
		expDebug::dPrintDBAPI('checkCurrencyExist qry', $qry);
		$codematchexist = db_query($qry);
		expDebug::dprint('CALL OUT checkCurrencyExist return '.$codematchexist->rowCount());
		return $codematchexist->rowCount();
	}catch (Exception $e) {
		watchdog_exception('checkCurrencyExist', $e);
		expertusLogError($e);
	}
}
/* Currency Conversion rate update - Callout */
?>