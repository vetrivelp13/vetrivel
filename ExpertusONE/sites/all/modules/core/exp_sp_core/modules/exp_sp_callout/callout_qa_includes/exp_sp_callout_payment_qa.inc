<?php
require_once DRUPAL_ROOT. '/sites/all/modules/core/exp_sp_core/modules/exp_sp_callout/exp_sp_callout.inc';
/**
 * processCalloutPaymentRequest
 *
 * Used to intiate the payment request to the external party.
 *
 * @param mixed $orderRequest contains full order details of the order.
 * $orderRequest->order_details contains the order basic information
 * $orderRequest->user_details contains the user information used in the order
 * $orderRequest->billing_details contains the billing information captured in the user screen
 * $orderRequest->payment_details contains the payment information captured in the user screen
 * $orderRequest->additional_details contains misc information on this order. it's used for internal system use.
 *
 *  The Payment order request format as like below sample.
 *  $orderRequest = stdClass Object
 *	(
 *	    [order_details] => Array
 *	        (
 *	            [order_id] => 4
 *	            [order_status] => in_checkout
 *	            [order_currency] => usd
 *	            [order_total] => 342.56
 *	            [created_time] => 1419933823
 *	            [modified_time] => 1419933873
 *	            [order_ipaddress] => 192.168.2.147
 *	            [order_items] => Array
 *	                (
 *	                    [0] => stdClass Object
 *	                        (
 *	                            [order_product_id] => 8
 *	                            [order_id] => 4
 *	                            [nid] => 1262
 *	                            [title] => Priced course VOD 1
 *	                            [qty] => 1
 *	                            [cost] => 150.00
 *	                            [price] => 150.00
 *	                            [order_uid] => 1
 *	                            [courseId] => 257
 *	                            [classId] => 391
 *	                            [delivery_type] => Video
 *	                            [delivery_type_code] => lrn_cls_dty_vod
 *	                            [location_id] =>
 *	                            [location] => N/A
 *	                            [tax] => 11.81
 *	                        )
 *
 *	                )
 *
 *	            [line_items] => Array
 *	                (
 *	                    [0] => Array
 *	                        (
 *	                            [line_item_id] => subtotal
 *	                            [type] => subtotal
 *	                            [title] => Subtotal:
 *	                            [amount] => 350.00
 *	                        )
 *
 *	                    [1] => Array
 *	                        (
 *	                            [line_item_id] => 6
 *	                            [type] => uc_discounts
 *	                            [title] =>
 *	                            [amount] => 35.00
 *	                        )
 *
 *	                    [2] => Array
 *	                        (
 *	                            [line_item_id] => 5
 *	                            [type] => cybersource_tax
 *	                            [title] => Tax
 *	                            [amount] => 27.56
 *	                        )
 *
 *	                )
 *
 *	        )
 *
 *	    [user_details] => Array
 *	        (
 *	            [user_id] => 1
 *	            [user_email] => expertusonefulldev@expertusone.com
 *	        )
 *
 *	    [billing_details] => Array
 *	        (
 *	            [billing_first_name] => person firstname
 *	            [billing_last_name] => person secondname
 *	            [billing_phone] => 9324235234
 *	            [billing_company] =>
 *	            [billing_street1] => test street
 *	            [billing_street2] =>
 *	            [billing_city] => Mountain view
 *	            [billing_zone] => 12
 *	            [billing_postal_code] => 12345
 *	            [billing_country] => 840
 *	        )
 *
 *	    [payment_details] => Array
 *	        (
 *	            [payment_method] => credit
 *	            [card_details] => Array
 *	                (
 *	                    [cc_number] => 4111111111111111
 *	                    [cc_exp_month] => 1
 *	                    [cc_exp_year] => 2015
 *	                    [cc_cvv] => 1111
 *	                )
 *
 *	        )
 *
 *	    [additional_details] => Array
 *	        (
 *	            [data] => Array
 *	                (
 *	                    [cc_data] => )YMa%I=fR9(?ay.e@*i/"+.t17%:)*M+-J+{20i3zkeRu@cO{p cGKO?j/ah1r5yuWG<K/([m~#NFO6QsUarwRs?-nN
 *	                )
 *
 *	            [discounts_codes_used] => Array
 *	                (
 *	                    [0] => NEWYEAR
 *	                )
 *
 *	            [rdf_mapping] => Array
 *	                (
 *	                )
 *
 *	        )
 *
 *	)
 *
 * @return null
 */
function processCalloutPaymentRequest($orderRequest) {
	try {
		expDebug::dPrint('calout ORDER REQUEST-->' . print_r($orderRequest,1), 4);
		// Include the needed files for the thrid party lib
		require_once DRUPAL_ROOT."/sites/all/modules/core/exp_sp_core/modules/exp_sp_callout/exp_sp_callout.inc";
		$payCallout = getCalloutEntityInfo('cme_pmt');
		expDebug::dPrint('calout resonpsd' . print_r($payCallout,1), 4);


		$gateway_id = uc_credit_default_gateway();
		$data = array(
				'txn_type' => variable_get('uc_pg_' . $gateway_id . '_cc_txn_type', UC_CREDIT_AUTH_CAPTURE),
		);

		if ($payCallout['method_type'] == 'api') {
			/**
			 * API Based method call
			 */
			$actionURL = $base_url."/callout/external_payment.php";
			require_once DRUPAL_ROOT."/sites/all/modules/core/exp_sp_core/modules/exp_sp_callout/callout_includes/cybersourceClient.php";
			$request = prepareCyberSourceOrderRequest($orderRequest,$data);
			$output  = '<form action="'.$actionURL.'" method="post" name="paymentCallForm" id="paymentCallForm">';
			$output .= '<input type="hidden" value=\''. serialize($request) .'\' name="order_info"/>';
			$output .= '<input type="hidden" value=\''. $base_url.'/'.$payCallout['response_url'] .'\' name="response_url"/>';
			$output .= '</form>';
			$output .= '<script type="text/javascript">document.paymentCallForm.submit();</script>';
			// print form submit and exit from the function call
			print $output;
			exit;


		} else {
			/**
			 * Native call method
			 */
			require_once DRUPAL_ROOT."/sites/all/modules/core/exp_sp_core/modules/exp_sp_callout/callout_includes/cybersourceClient.php";

			// Prepare the request object from the order object
			$request = prepareCyberSourceOrderRequest($orderRequest,$data);

			// call server function with soap
			$soapClient = new ExtendedClient(WSDL_URL, array());
			$reply = $soapClient->runTransaction($request);
			expDebug::dPrint("Response reply " . print_r($reply,1), 4);



			$result							= array();

			$result['success']				= ($reply->decision == 'ACCEPT') ? true : false; // Set result value based on the decision code value
			$result['orderId']				= $orderRequest->order_details['order_id'];
			$result['paymethod']			= 'credit';
			$result['uid']						= $orderRequest->user_details['user_id'];
			$result['additionalInfo'] 		= $data;
			$result['comment']				= 'addtional comments';

			// Online transaction
			$transaction 					= new stdClass();
			$transaction->requestID 		= $reply->requestID;
			$transaction->currency			= $reply->purchaseTotals->currency;
			$transaction->amount	 		= $reply->ccCaptureReply->amount;
			$transaction->requestQuery		= 'not avilable';
			$transaction->responseResult	= urlencode($soapClient->mResponseXML);;

			//Optional params
			$transaction->taxAmount	 		= '';
			$transaction->shipAmount		= '';
			$transaction->reasonCode 		= $reply->ccCaptureReply->reasonCode;
			$transaction->transactionType	= $data['txn_type'];
			$transaction->decision			= $reply->decision;
			$transaction->avsCode 			= $reply->ccAuthReply->avsCode;
			$transaction->cvCode 			= $reply->ccAuthReply->cvCode;

			// Purchase order transaction
			$transaction->poNumber	 	= '';

			// Add transaction object to result
			$result['transaction']		= $transaction;

			return $result;
		}
	} catch (Exception $ex) {
		watchdog_exception('processPaymentRequest', $ex);
		expertusErrorThrow($ex);
	}
}

function prepareCyberSourceOrderRequest($orderRequest, $data) {
	try {

		// collect informaion for making order request
		//$currency = variable_get('uc_cybersource_currency', 'usd');
		$currency = $orderRequest->order_details['order_currency']; //multi currency related work
		$countryIsoCode = getISOCodeForPayment($orderRequest->billing_details['billing_country'], 'uc_country_id');
		$stateIsoCode = getISOCodeForPayment($orderRequest->billing_details['billing_zone'], 'uc_state_id', $orderRequest->billing_details['billing_country']);
		$org_company = getPersonCompanyfromOrderId($orderRequest->order_details['order_id']);

		// prepare the requset object for the cyber source call
		$request = new stdClass();
		$request->merchantID = MERCHANT_ID;
		$request->merchantReferenceCode = $orderRequest->order_details['order_id'];
		$request->clientLibrary = "PHP";
		$request->clientLibraryVersion = phpversion();
		$request->clientEnvironment = php_uname();

		// Add the credit card authorization service.
		if (in_array($data['txn_type'], array(UC_CREDIT_AUTH_ONLY, UC_CREDIT_AUTH_CAPTURE, UC_CREDIT_REFERENCE_TXN))) {
			$ccAuthService = new stdClass();
			$ccAuthService->run = 'true';
			$request->ccAuthService = $ccAuthService;
		}
		expDebug::dPrint("Checking the ourder details from array".print_r($orderRequest,true));
		// Add the credit card capture service.
		if (in_array($data['txn_type'], array(UC_CREDIT_PRIOR_AUTH_CAPTURE, UC_CREDIT_AUTH_CAPTURE, UC_CREDIT_REFERENCE_TXN))) {
			$ccCaptureService = new stdClass();
			$ccCaptureService->run = 'true';
			$request->ccCaptureService = $ccCaptureService;
		}
		$ccAuthService = new stdClass();
		$ccAuthService->run = "true";
		$request->ccAuthService = $ccAuthService;

		// Order details and information
		$orderDet = new stdClass();
		$orderDet->order_id = $orderRequest->order_details['order_id'];
		$orderDet->user_id = $orderRequest->user_details['user_id'];
		$orderDet->data = $data;
		$request->orderDet = $orderDet;

		// Billing information
		$billTo = new stdClass();
		$billTo->firstName = $orderRequest->billing_details['billing_first_name'];
		$billTo->lastName = $orderRequest->billing_details['billing_last_name'];
		$billTo->street1 = $orderRequest->billing_details['billing_street1'];
		if ($order->billing_street2) {
			$billTo->street2 = $orderRequest->billing_details['billing_street2'];
		}
		$billTo->city = $orderRequest->billing_details['billing_city'];
		$billTo->state = $stateIsoCode;
		$billTo->postalCode = $orderRequest->billing_details['billing_postal_code'];
		$billTo->country = $countryIsoCode;
		if ($orderRequest->billing_details['billing_phone']) {
			$billTo->phoneNumber = $orderRequest->billing_details['billing_phone'];
		}
		if (!empty($org_company)) {
			$billTo->company = $org_company;
		}
		$billTo->email = $orderRequest->user_details['user_email'];
		$billTo->ipAddress = $orderRequest->order_details['order_ipaddress'];
		$billTo->customerID = $orderRequest->user_details['user_id'];
		$request->billTo = $billTo;

		// Credit card information
		$card = new stdClass();
		$card->accountNumber = $orderRequest->payment_details['card_details']['cc_number'];
		$card->expirationMonth = $orderRequest->payment_details['card_details']['cc_exp_month'];
		$card->expirationYear = $orderRequest->payment_details['card_details']['cc_exp_year'];
		if (variable_get('uc_credit_cvv_enabled', TRUE)) {
			$card->cvNumber = $orderRequest->payment_details['card_details']['cc_cvv'];
		}
		$request->card = $card;

		// Payment information (Total amount or product items including line items(tax))
		// Check if there is any discount line item because CyberSource API does not support negative amounts
		$discount_line_items = false;
		for ($i = 0; ($i < count($orderRequest->order_details['line_items'])) && !$discount_line_items; $i++) {
			$line_item = $orderRequest->order_details['line_items'][$i];
			if ($line_item['amount'] < 0)
				$discount_line_items = true;
		}
		$purchaseTotals = new stdClass();
		$purchaseTotals->currency = $currency;
		// Specify the total to charge if it's less than the order total.
		if ($discount_line_items) {
			$purchaseTotals->grandTotalAmount = $orderRequest->order_details['order_total'];
		} else {
			$counter = 0;
			$request->item = array();
			foreach($orderRequest->order_details['order_items'] as $product) {
				$request->item[] = $obj = new stdClass();

				$obj->productName = $product->title;
				$obj->unitPrice = $product->price;
				$obj->quantity = $product->qty;
				$obj->productSKU = $product->model;
				$obj->productCode = 'default';
				$obj->id = $counter;

				$counter++;
			}

			// Add the line items to the item array.
			foreach ((array) $orderRequest->order_details['line_items'] as $line_item) {
				// Skip subtotal line items.
				if (strpos($line_item['type'], 'subtotal') === FALSE) {
					$obj = $request->item[] = new stdClass();

					$obj->productName = $line_item['title'];
					$obj->unitPrice = $line_item['amount'];
					$obj->quantity = 1;
					$obj->productSKU = $line_item['type'] . '_' . $line_item['line_item_id'];
					$obj->id = $counter;

					$counter++;
				}
			}
		}
		$request->purchaseTotals = $purchaseTotals;

		// Add business rules.
		$business = new stdClass();
		$business->ignoreAVSResult = variable_get('uc_cybersource_avs', 'true') == 'true' ? 'false' : 'true';
		$request->businessRules = $business;
		expDebug::dPrint('order request prepared final. ' . print_r($request, 1), 4);

		return $request;

	} catch(Exception $ex) {
		watchdog_exception('prepareExternalOrderRequest', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 *	processCalloutPaymentResponse
 *
 *  Used to prepare the result array and execute the complete callback (completeCalloutPaymentResponse function) in the LMS system.
 *
 *  completeCalloutPaymentResponse($result); - This function is used to process the payment success/failure transactions in the LMS based on the result values from third party.
 *  Based on the success/failure redirect user to the respective pages for API payment method. (this is the helper function present in exp_sp_callout.inc).
 *
 *  The tax calculation result format should be like bellow.
 *
 *	$result = Array
 *	(
 *	    [success] => true
 *	    [orderId] => 2
 *	    [paymethod] => credit
 *	    [uid] => 1
 *	    [additionalInfo] => credit
 *	    [comment] => addtional comments
 *	    [transaction] => stdClass Object
 *	        (
 *	            [requestId] => JH83972834234DG
 *	            [currency] => USD
 *	            [amount] => 2.00
 *	            [requestQuery] => not avilable
 *	            [responseResult] =>
 *	            [taxAmount] =>
 *	            [shipAmount] =>
 *	            [reasonCode] => 100
 *	            [transactionType] => type
 *	            [decision] => decision
 *	            [avsCode] => JH83972834234DG
 *	            [cvCode] => JH83972834234DG
 *	            [poNumber] => 213123123
 *	        )
 *
 *	)
 *
 *  @return null
  */
function processCalloutPaymentResponse() {
	try {

		$reply = unserialize($_POST['response_info']);
		$order_id = $reply->orderDet->order_id;
		// prepare response array from the arguments array
		if ($order_id == '') {
			expDebug::dPrint('Response orderId is empty', 4);
			return false;
		} else {
			// get the order object from ubercart.
			$order = uc_order_load($order_id);
		}

		$result							= array();

		$result['success']				= ($reply->decision == 'ACCEPT') ? true : false; // Set result value based on the decision code value
		$result['orderId']				= $order_id;
		$result['paymethod']			= 'credit';
		$result['uid']					= $order->uid;
		$result['additionalInfo'] 		= $reply->orderDet->data;
		$result['comment']				= 'addtional comments';

		// Online transaction
		$transaction 					= new stdClass();
		$transaction->requestID 		= $reply->requestID;
		$transaction->currency			= $reply->purchaseTotals->currency;
		$transaction->amount	 		= $reply->ccCaptureReply->amount;
		$transaction->requestQuery		= 'not avilable';
		$transaction->responseResult	= $reply->orderDet->responsexml;

		//Optional params
		$transaction->taxAmount	 		= '';
		$transaction->shipAmount		= '';
		$transaction->reasonCode 		= $reply->ccCaptureReply->reasonCode;
		$transaction->transactionType	= $reply->orderDet->data['txn_type'];
		$transaction->decision			= $reply->decision;
		$transaction->avsCode 			= $reply->ccAuthReply->avsCode;
		$transaction->cvCode 			= $reply->ccAuthReply->cvCode;

		// Purchase order transaction
		$transaction->poNumber	 	= '';

		// Add transaction object to result
		$result['transaction']		= $transaction;

		completeCalloutPaymentResponse($result);
		return $result;
	} catch (Exception $ex) {
		watchdog_exception('calloutProcessResponse', $ex);
		expertusErrorThrow($ex);
	}

}

