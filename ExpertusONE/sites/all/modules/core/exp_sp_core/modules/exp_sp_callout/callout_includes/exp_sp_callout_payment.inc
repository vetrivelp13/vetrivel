<?php 
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
	//completeCalloutPaymentResponse($result);
}