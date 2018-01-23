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
	} catch(Exception $e) {
		watchdog_exception('processCalloutTaxRequest', $e);
		expertusErrorThrow($e);
	}
}
