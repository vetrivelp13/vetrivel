<?php

// Before using this example, replace the generic values with your merchant ID and password.
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
         $this->mRequestXML = $request;
	 // printf( "Modified Request:\n*$request*\n" );

     } catch (DOMException $e) {
         die( 'Error adding UsernameToken: ' . $e->code);
     }

	 $resp = parent::__doRequest($request, $location, $action, $version);
     $this->mResponseXML=$resp;
     return $resp;
   }
   
   public function getRequestXML()
   {
   	try{
   		return $this->mRequestXML;
   	}catch (Exception $ex) {
   		watchdog_exception('getRequestXML', $ex);
   		expertusErrorThrow($ex);
   	}
   }
}
?>