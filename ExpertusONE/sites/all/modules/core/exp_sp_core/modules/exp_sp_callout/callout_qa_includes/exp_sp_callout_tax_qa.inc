<?php
require_once DRUPAL_ROOT. '/sites/all/modules/core/exp_sp_core/modules/exp_sp_callout/exp_sp_callout.inc';

/**
* processCalloutTaxRequest
* @param array $orderRequest Containig segregated line of items like ILT and NONILT
* @param object $order Containig current order related variables like uc_order_id, billing details, item details, user details and so on.
* @param array $taxSettings This array contain the our site setting tax details.
* @return $result array.
* Used to initiate the tax request
* 

*  The tax calculation return format should be bellow.
*  		$result = Array
*  		(
*				[ILT11] => Array
*				(
*						[groupTotalTaxAmount] => 22.19
*						[SltOrderId] => 2491
*						[Items] => Array
*						(
*								[0] => Array
*								(
*										[totalTaxAmount] => 13.31
*										[NEW YORK] => 6 
*										[NEW YORK CITY] => 6.75
*										[METROPOLITAN COMMUTER TRANSPORTATION DISTRICT] => 0.56
*								)
*
*								[1] => Array
*								(
*										[totalTaxAmount] => 8.88
*										[NEW YORK] => 4
*										[NEW YORK CITY] => 4.5
*										[METROPOLITAN COMMUTER TRANSPORTATION DISTRICT] => 0.38
*								)
*
*						)
*
*				)
*
*				[NONILT] => Array
*
*				(
*								[groupTotalTaxAmount] => 9.77
*								[SltOrderId] => 2491
*								[Items] => Array
*								(
*												[0] => Array
*												(
*																[totalTaxAmount] => 4.44
*																[NEW YORK] => 2
*																[NEW YORK CITY] => 2.25
*																[METROPOLITAN COMMUTER TRANSPORTATION DISTRICT] => 0.19
*														)
*
*												[1] => Array
*												(
*																[totalTaxAmount] => 5.33
*																[NEW YORK] => 2.4
*																[NEW YORK CITY] => 2.7
*																[METROPOLITAN COMMUTER TRANSPORTATION DISTRICT] => 0.23
*														)
*
*										)
*
*					)
*
*				[wholeTaxAmount] => 31.96
*		)
*
* The above array fomat item tax arry like 
* 							[0] => Array
*								(
*										[totalTaxAmount] => 13.31
*										[NEW YORK] => 6 
*										[NEW YORK CITY] => 6.75
*										[METROPOLITAN COMMUTER TRANSPORTATION DISTRICT] => 0.56
*								)
*
* This array should be have the totalTaxAmount value other than are not necessary
*
*
* 
**/

function processCalloutTaxRequest($orderRequest,$order,$taxSettings) {
	try {
		$form_single_arr = array();
		foreach ($orderRequest as $key=>$values){
			$values['Address']['Street'] = $order->billing_street1;
			$form_single_arr[$key] = $values;
		}
		expDebug::dPrint('$form_single_arr--> ' . print_r($form_single_arr, true), 4);
		expDebug::dPrint('order_arr--> ' . print_r($order, true), 4);
		//expDebug::dPrint('before request response--> ' . print_r(http_build_query($orderRequest), true), 4);
		// create a new cURL resource
		$ch = curl_init();
		
		// set URL and other appropriate options
		curl_setopt($ch, CURLOPT_URL, "http://115.111.237.97/tax/avatax/Samples/GetTaxTest.php");
		curl_setopt($ch, CURLOPT_HEADER, false);
		curl_setopt($ch, CURLOPT_NOBODY, false); // remove body
		curl_setopt($ch, CURLOPT_POST, 1);
		curl_setopt($ch, CURLOPT_POSTFIELDS,http_build_query($form_single_arr));
		// receive server response ...
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
		
		$server_output = curl_exec ($ch);
		// close cURL resource, and free up system resources
		curl_close($ch);
		$result = json_decode($server_output,true);
		return $result;
	} catch(Exception $e) {
		watchdog_exception('processCalloutTaxRequest', $e);
		expertusErrorThrow($e);
	}
}
