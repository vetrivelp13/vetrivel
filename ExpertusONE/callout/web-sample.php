<HTML>
	<HEAD>
       	<META HTTP-EQUIV="Content-Type" content="text/html; charset=iso-8859-1">
		<TITLE>Order Status</TITLE>
	</HEAD>
	<BODY>


<?php
define( 'MERCHANT_ID', 'devexpertusone' );
define( 'TRANSACTION_KEY', 'TGTlCNuDmiH7kGEZnMm9hOx7IIC8EAKEzEij9h8qiA6pIRSI8guJf57U9cIALbjBJCRk3X9CzWSdtWQOUQvTv+lQdoAKBuew1JahDYJGJ9dYnPv7uoVypCDxC3kef3LW7mEa8iMCFF5KlL2pUSjEX+X3mjKOYs9CgJ8KrK+6ht15DNbTULzdJQtTA/uqKCvGfIi2cfxISQjykkZDTKi4pKytx+JvnIoiqZx5WE1DToQemNojOhxeXinFQAo8hGPVJdnelr3c2pA5+e74mp34jybuZFyxwaedlqmh6yqAd37VuADm/XpEQ3RFSDI9SJpWs1Ot+QuCIBvmb77OdrY+ag==' );
define( 'WSDL_URL', 'https://ics2wstest.ic3.com/commerce/1.x/transactionProcessor/CyberSourceTransaction_1.38.wsdl' );

class ExtendedClient extends SoapClient {

	public $mRequestXML='';
	public $mResponseXML='';
	
   function __construct($wsdl, $options = null) {
     parent::__construct($wsdl, $options);
   }

// This section inserts the UsernameToken information in the outgoing SOAP message.
   function __doRequest($request, $location, $action, $version) {

     $user = MERCHANT_ID;
     $password = TRANSACTION_KEY;

     $soapHeader = "<SOAP-ENV:Header xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:wsse=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\"><wsse:Security SOAP-ENV:mustUnderstand=\"1\"><wsse:UsernameToken><wsse:Username>$user</wsse:Username><wsse:Password Type=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordText\">$password</wsse:Password></wsse:UsernameToken></wsse:Security></SOAP-ENV:Header>";

     $requestDOM = new DOMDocument('1.0');
     $soapHeaderDOM = new DOMDocument('1.0');

     try {

         $requestDOM->loadXML($request);
	 $soapHeaderDOM->loadXML($soapHeader);

	 $node = $requestDOM->importNode($soapHeaderDOM->firstChild, true);
	 $requestDOM->firstChild->insertBefore(
         	$node, $requestDOM->firstChild->firstChild);

         $request = $requestDOM->saveXML();

	 // printf( "Modified Request:\n*$request*\n" );

     } catch (DOMException $e) {
         die( 'Error adding UsernameToken: ' . $e->code);
     }

     $resp = parent::__doRequest($request, $location, $action, $version);
     $this->mResponseXML=$resp;
     return $resp;
   }
}

try {
	$soapClient = new ExtendedClient(WSDL_URL, array());

	/*
	To see the functions and types that the SOAP extension can automatically
    generate from the WSDL file, uncomment this section:
	$functions = $soapClient->__getFunctions();
	print_r($functions);
	$types = $soapClient->__getTypes();
	print_r($types);
	*/
  $request = unserialize($_POST['order']);
 // global $conf;
	$card = new stdClass();
	$card->accountNumber = $_POST['card_number'];
	$card->expirationMonth = $_POST['card_expire_month'];
	$card->expirationYear = $_POST['card_expire_year'];
	/* if (!empty($conf['uc_credit_cvv_enabled'])){
		$card->cvNumber = $_POST['card_cvv'];
	} */
	$request->card = $card;

	$reply = $soapClient->runTransaction($request);

	// This section will show all the reply fields.

// Order details and information
		$orderDet = new stdClass();
		$orderRequest = unserialize($_POST['order']);
		$orderDet->order_id = $orderRequest->orderDet->order_id;
		$orderDet->user_id = $orderRequest->orderDet->user_id;
		$orderDet->data = $orderRequest->orderDet->data;
		$orderDet->responsexml = urlencode($soapClient->mResponseXML);
		$orderDet->paymethod = 'credit';
		$reply->orderDet = $orderDet;
		
	// To retrieve individual reply fields, follow these examples.
	//printf( "decision = $reply->decision<br>" );
	//printf( "reasonCode = $reply->reasonCode<br>" );
	//printf( "requestID = $reply->requestID<br>" );
	//printf( "requestToken = $reply->requestToken<br>" );
	
	//printf( "ccAuthReply->reasonCode = " . $reply->ccAuthReply->reasonCode . "<br>");
	$externalSiteLink = $_POST['response_url']; 
	$output  = '<form action="'.$externalSiteLink.'" method="post" name="responseCallForm" id="responseCallForm">';
			$output .= '<input type="hidden" value=\''. serialize($reply) .'\' name="response_info"/>';
			$output .= '</form>';
			$output .= '<script type="text/javascript">document.responseCallForm.submit();</script>';
			// print form submit and exit from the function call
			print $output;
			exit;
} catch (SoapFault $exception) {
	var_dump(get_class($exception));
	var_dump($exception);
}
?>

	</BODY>
</HTML>