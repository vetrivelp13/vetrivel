<?php

/**
 * @file
 * Defines a class used for communicating with CyberSource via SOAP.
 *
 * Provided by Acquia, Commercially supported Drupal - http://acquia.com
 */

class CyberSourceSoapClient extends SoapClient {
	
  private $requestXMLStr;
  function __construct($wsdl, $options = NULL) {
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
	  	 * SCSoapClient.php
	  	 * CyberSourceClientLib.php
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

  /**
   * Inserts the UsernameToken information in the outgoing request.
   */
  function __doRequest($request, $location, $action, $version) {
  	try{
    $login = _uc_cybersource_soap_login_data();

    $soapHeader = '<SOAP-ENV:Header xmlns:SOAP-ENV="http://schemas.xmlsoap.org/soap/envelope/" xmlns:wsse="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd"><wsse:Security SOAP-ENV:mustUnderstand="1"><wsse:UsernameToken><wsse:Username>' . $login['merchant_id'] . '</wsse:Username><wsse:Password Type="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordText">' . $login['transaction_key'] . '</wsse:Password></wsse:UsernameToken></wsse:Security></SOAP-ENV:Header>';

    $requestDOM = new DOMDocument('1.0');
    $soapHeaderDOM = new DOMDocument('1.0');

    try {
      $requestDOM->loadXML($request);
      //$soapHeaderDOM->loadXML($soapHeader);
      // Added the conduction for #0057097 (If the soapHeader request fails then it will not re-direct to the site maintainence page).
      $value = $soapHeaderDOM->loadXML($soapHeader);
      if($value == ''){

      	expDebug::dPrint('The files folder is not configured so the request fails and no responce generated from cybersource',1);
	  
      }else{
      $node = $requestDOM->importNode($soapHeaderDOM->firstChild, TRUE);
      $requestDOM->firstChild->insertBefore($node, $requestDOM->firstChild->firstChild);
      $request = $requestDOM->saveXML();
      $this->requestXMLStr=$request;
    }
    }
    catch (DOMException $e) {
      die('Error adding UsernameToken: ' . $e->code);
    }

    $resp =  parent::__doRequest($request, $location, $action, $version);
    $this->mResponseXML=$resp;    
    return $resp;
    }catch (Exception $ex) {
    	watchdog_exception('__doRequest', $ex);
    	expertusErrorThrow($ex);
    }
  }
  public function getRequestXML()
  {
  	try{
		return $this->requestXMLStr;  
		}catch (Exception $ex) {
			watchdog_exception('getRequestXML', $ex);
			expertusErrorThrow($ex);
		}	
  }
}
