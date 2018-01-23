<?php
require_once DRUPAL_ROOT. '/sites/all/modules/core/exp_sp_core/modules/exp_sp_callout/exp_sp_callout.inc';

/**
* processCalloutRefundRequest
* @param object $order Containing current order related variables like uc_order_id, billing details, item details, user details and so on.
* 
* $order = stdClass Object
*	(
*    [order_details] => Array
*        (
*           [order_id] => 1
*           [order_status] => completed
*           [order_currency] => usd
*           [order_total] => 217.50
*           [created_time] => 1420456069
*           [modified_time] => 1420456129
*           [order_ipaddress] => 192.168.2.74
*           [order_items] => Array
*           (
*                   [0] => stdClass Object
*                       (
*                           [order_product_id] => 1
*                           [order_id] => 1
*                           [nid] => 17
*                           [title] => Aggregate Supply and Demand - class 2
*                           [qty] => 1
*                           [cost] => 200.00
*                           [price] => 200.00
*                           [order_uid] => 2
*                           [tax] => 0.00
*                       )
*               )
*           [line_items] => Array
*               (
*                   [0] => Array
*                       (
*                           [line_item_id] => subtotal
*                           [type] => subtotal
*                           [title] => Subtotal:
*                           [amount] => 200.00
*                       )
*
*                   [1] => Array
*                       (
*                           [line_item_id] => 1
*                           [type] => cybersource_tax
*                           [title] => Tax
*                           [amount] => 17.50
*                       )
*
*               )
*
*        )
*   [user_details] => Array
*       (
*           [user_id] => 2
*           [user_email] => shobanan@expertus.com
*       )
*
*   [billing_details] => Array
*       (
*           [billing_first_name] => shobana
*           [billing_last_name] => n
*           [billing_phone] => 
*           [billing_company] => 
*           [billing_street1] => 2091 landing drive
*           [billing_street2] => 
*           [billing_city] => mountain view
*           [billing_zone] => 12
*           [billing_postal_code] => 12345
*           [billing_country] => 840
*       )
*
*   [payment_details] => Array
*       (
*           [payment_method] => credit
*           [card_details] => Array
*               (
*                   [cc_number] => 1111
*                   [cc_exp_month] => 1
*                   [cc_exp_year] => 2016
*               )
*
*       )
*
*   [additional_details] => Array
*       (
*           [data] => Array
*               (
*                   [order_create_from] => admin_checkout
*                   [cc_data] => 6v|*WD]pVcL"I%Rsp6%r"()KZHqS$5#RS;,]>=uSw#vXE=/AMaWH4.*$)/%NqWX7R8m<6mp$Iwj~Y>COM,kaS!$R>[3
*               )
*
*           [discounts_codes_used] => Array
*               (
*               )
*
*           [rdf_mapping] => Array
*               (
*               )
*
*       )
*
* )
* @param array $ordDet This Object array contain the following details,
*  - slt_order_id (Order id from slt_order table).
*  - amount (Amount to be refund after applied cancellation charges).
*  - url (URL that should be called for refund calculation).
* The Refund calculation return format should be as below,
*  		$replyDet = stdClass Object
* (
*    [orderId] => 1
*    [vreqxml] => %3C%3Fxml+version%3D%221.0%22+encoding%3D%22UTF-8%22%3F%3E%0A%3CSOAP-ENV%3AEnvelope+xmlns%3ASOAP-ENV%3D%22http%3A%2F%2Fschemas.xmlsoap.org%2Fsoap%2Fenvelope%2F%22+xmlns%3Ans1%3D%22urn%3Aschemas-cybersource-com%3Atransaction-data-1.38%22%3E%3CSOAP-ENV%3AHeader+xmlns%3Awsse%3D%22http%3A%2F%2Fdocs.oasis-open.org%2Fwss%2F2004%2F01%2Foasis-200401-wss-wssecurity-secext-1.0.xsd%22%3E%3Cwsse%3ASecurity+SOAP-ENV%3AmustUnderstand%3D%221%22%3E%3Cwsse%3AUsernameToken%3E%3Cwsse%3AUsername%3Edevexpertusone%3C%2Fwsse%3AUsername%3E%3Cwsse%3APassword+Type%3D%22http%3A%2F%2Fdocs.oasis-open.org%2Fwss%2F2004%2F01%2Foasis-200401-wss-username-token-profile-1.0%23PasswordText%22%3ETGTlCNuDmiH7kGEZnMm9hOx7IIC8EAKEzEij9h8qiA6pIRSI8guJf57U9cIALbjBJCRk3X9CzWSdtWQOUQvTv%2BlQdoAKBuew1JahDYJGJ9dYnPv7uoVypCDxC3kef3LW7mEa8iMCFF5KlL2pUSjEX%2BX3mjKOYs9CgJ8KrK%2B6ht15DNbTULzdJQtTA%2FuqKCvGfIi2cfxISQjykkZDTKi4pKytx%2BJvnIoiqZx5WE1DToQemNojOhxeXinFQAo8hGPVJdnelr3c2pA5%2Be74mp34jybuZFyxwaedlqmh6yqAd37VuADm%2FXpEQ3RFSDI9SJpWs1Ot%2BQuCIBvmb77OdrY%2Bag%3D%3D%3C%2Fwsse%3APassword%3E%3C%2Fwsse%3AUsernameToken%3E%3C%2Fwsse%3ASecurity%3E%3C%2FSOAP-ENV%3AHeader%3E%3CSOAP-ENV%3ABody%3E%3Cns1%3ArequestMessage%3E%3Cns1%3AmerchantID%3Edevexpertusone%3C%2Fns1%3AmerchantID%3E%3Cns1%3AmerchantReferenceCode%3E1%3C%2Fns1%3AmerchantReferenceCode%3E%3Cns1%3AclientLibrary%3EPHP%3C%2Fns1%3AclientLibrary%3E%3Cns1%3AclientLibraryVersion%3E5.3.26%3C%2Fns1%3AclientLibraryVersion%3E%3Cns1%3AclientEnvironment%3ELinux+sho-n-ltpss-ubun.expertus.com+3.8.0-29-generic+%2342%7Eprecise1-Ubuntu+SMP+Wed+Aug+14+16%3A19%3A23+UTC+2013+x86_64%3C%2Fns1%3AclientEnvironment%3E%3Cns1%3ApurchaseTotals%3E%3Cns1%3Acurrency%3Eusd%3C%2Fns1%3Acurrency%3E%3Cns1%3AgrandTotalAmount%3E17.50%3C%2Fns1%3AgrandTotalAmount%3E%3C%2Fns1%3ApurchaseTotals%3E%3Cns1%3AccCreditService+run%3D%22true%22%3E%3Cns1%3AcaptureRequestID%3E4204561292890176195843%3C%2Fns1%3AcaptureRequestID%3E%3Cns1%3AcaptureRequestToken%3EAhj%2F%2FwSRx9wE1lOILxoGIkGTFkxYuWTme0iWYMmIwT%2BKc38ioBT%2BKc38irSB9T2TjYZNJMt0gN85RDAnI4%2B4CaynEF40DAAA%2FAKc%3C%2Fns1%3AcaptureRequestToken%3E%3C%2Fns1%3AccCreditService%3E%3C%2Fns1%3ArequestMessage%3E%3C%2FSOAP-ENV%3ABody%3E%3C%2FSOAP-ENV%3AEnvelope%3E%0A
*    [vrespxml] => %3C%3Fxml+version%3D%221.0%22+encoding%3D%22utf-8%22%3F%3E%3Csoap%3AEnvelope+xmlns%3Asoap%3D%22http%3A%2F%2Fschemas.xmlsoap.org%2Fsoap%2Fenvelope%2F%22%3E%0A%3Csoap%3AHeader%3E%0A%3Cwsse%3ASecurity+xmlns%3Awsse%3D%22http%3A%2F%2Fdocs.oasis-open.org%2Fwss%2F2004%2F01%2Foasis-200401-wss-wssecurity-secext-1.0.xsd%22%3E%3Cwsu%3ATimestamp+xmlns%3Awsu%3D%22http%3A%2F%2Fdocs.oasis-open.org%2Fwss%2F2004%2F01%2Foasis-200401-wss-wssecurity-utility-1.0.xsd%22+wsu%3AId%3D%22Timestamp-671275459%22%3E%3Cwsu%3ACreated%3E2015-01-05T11%3A17%3A31.229Z%3C%2Fwsu%3ACreated%3E%3C%2Fwsu%3ATimestamp%3E%3C%2Fwsse%3ASecurity%3E%3C%2Fsoap%3AHeader%3E%3Csoap%3ABody%3E%3Cc%3AreplyMessage+xmlns%3Ac%3D%22urn%3Aschemas-cybersource-com%3Atransaction-data-1.38%22%3E%3Cc%3AmerchantReferenceCode%3E1%3C%2Fc%3AmerchantReferenceCode%3E%3Cc%3ArequestID%3E4204566511770176195842%3C%2Fc%3ArequestID%3E%3Cc%3Adecision%3EACCEPT%3C%2Fc%3Adecision%3E%3Cc%3AreasonCode%3E100%3C%2Fc%3AreasonCode%3E%3Cc%3ArequestToken%3EAhj%2F7wSRx9wp62npPBoEIkGTFkxZsGTGW0hyqM1s5T%2BKc38ioBT930nR%2FfSBe1k42GTSTLdIDfOUYE5HH3ATWU4gvGgY7gbB%3C%2Fc%3ArequestToken%3E%3Cc%3ApurchaseTotals%3E%3Cc%3Acurrency%3Eusd%3C%2Fc%3Acurrency%3E%3C%2Fc%3ApurchaseTotals%3E%3Cc%3AccCreditReply%3E%3Cc%3AreasonCode%3E100%3C%2Fc%3AreasonCode%3E%3Cc%3ArequestDateTime%3E2015-01-05T11%3A17%3A31Z%3C%2Fc%3ArequestDateTime%3E%3Cc%3Aamount%3E17.50%3C%2Fc%3Aamount%3E%3Cc%3AreconciliationID%3E21213021K4CJQM69%3C%2Fc%3AreconciliationID%3E%3C%2Fc%3AccCreditReply%3E%3C%2Fc%3AreplyMessage%3E%3C%2Fsoap%3ABody%3E%3C%2Fsoap%3AEnvelope%3E
*    [status]  => success
* )
*
* processRefundTransactionLog($replyDet); this function is used to store and log the refund information.
* This Funtion should be used when the refund completed successfully or failure.(this is the helper function present in exp_sp_callout.inc)
* Return $replyDet['status'] success or failure.
* 
**/

function processCalloutRefundRequest($order,$ordDet) {
	try {

	} catch(Exception $e) {
		watchdog_exception('processCalloutRefundRequest', $e);
		expertusErrorThrow($e);
	}
}
