<?php
/**
* @class or PHP Name		: UserInfo
* @author(s)				: Product Team
* Version         			: 1.0
* Date						: 09/09/2009
* PHP Version          		: 5.2.6
* Description     			: Class used to get the currently logged in user related information.
*/
class ExtendedClient extends SoapClient {

  public $mRequestXML='';
  public $mResponseXML='';

   function __construct($wsdl, $options = null,$mer,$trans) {
		/**
	  	 * PHP 5.6.x has peer verification in SoapClient, 
	  	 * it is throw Could not connect to host error if  
	  	 * the certificate is not match.
	  	 * 
	  	 * TODO: If the issue has been resolved in PHP
	  	 * or if there is any alternative to do peer
	  	 * verification, then the below opetion parameter 
	  	 * should be removed. 
	  	 * 
	  	 * This will be implemented in few places, whoever 
	  	 * removing this options should take care of removing 
	  	 * in the below placess as well.
	  	 * 
	  	 * exp_sp_payment.soap.inc
	  	 * SCSoapClient.php
	  	 * 
	  	 * Added by Vincent on 24 July, 2015
	  	 * 
	  	 */
   	$peerVerify = getConfigValue('peer_verify') == 0 ? FALSE : TRUE;
  	$options = array('stream_context' =>stream_context_create(array(
            'ssl' => array('verify_peer'=>$peerVerify, 
            							 'verify_peer_name'=>$peerVerify)
        )));
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
         $value = $soapHeaderDOM->loadXML($soapHeader);
         // Added the conduction for #0057097 (If the soapHeader request fails then it will not re-direct to the site maintainence page).
      if($value == ''){
      	expDebug::dPrint('The files folder is not configured so the request fails and no responce generated from cybersource in tax part',1);

      }else{
	 $node = $requestDOM->importNode($soapHeaderDOM->firstChild, true);
	 $requestDOM->firstChild->insertBefore(
     $node, $requestDOM->firstChild->firstChild);
     $request = $requestDOM->saveXML();
     $this->mRequestXML = $request; 
      }
	   
	    
     } catch (DOMException $e) {
         die( 'Error adding UsernameToken: ' . $e->code);
     }
          
     $resp =  parent::__doRequest($request, $location, $action, $version);
	 $this->mResponseXML=$resp;    
     return $resp; 
   }
}


?>