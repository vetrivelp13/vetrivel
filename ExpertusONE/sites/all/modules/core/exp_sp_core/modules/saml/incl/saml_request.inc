<?php 
function redirect($url, $parameters = array()) {
	assert(is_string($url));
	assert(strlen($url) > 0);
	assert(is_array($parameters));

	/* Verify that the URL is to a http or https site. */
	if (!preg_match('@^https?://@i', $url)) {
		throw new SimpleSAML_Error_Exception('Redirect to invalid URL: ' . $url);
	}

	/* Determine which prefix we should put before the first
	 * parameter.
	 */
	if(strpos($url, '?') === FALSE) {
		$paramPrefix = '?';
	} else {
		$paramPrefix = '&';
	}

	/* Iterate over the parameters and append them to the query
	 * string.
	 */
	foreach($parameters as $name => $value) {

		/* Encode the parameter. */
		if($value === NULL) {
			$param = urlencode($name);
		} elseif (is_array($value)) {
			$param = "";
			foreach ($value as $val) {
				$param .= urlencode($name) . "[]=" . urlencode($val) . '&';				
			}
		} else {
			$param = urlencode($name) . '=' .
				urlencode($value);
		}

		/* Append the parameter to the query string. */
		$url .= $paramPrefix . $param;

		/* Every following parameter is guaranteed to follow
		 * another parameter. Therefore we use the '&' prefix.
		 */
		$paramPrefix = '&';
	}


	
	/* Set the location header. */
	drupal_goto($url);

	

	/* End script execution. */
	exit;
}

function getRelayStateURL()
{
	if(isset($_SERVER["HTTP_REFERER"])){
	
		$urlArray=parse_url($_SERVER["HTTP_REFERER"]);
		if(isset($urlArray) && isset($urlArray['query'])){
			return urlencode(htmlspecialchars($_SERVER["HTTP_REFERER"]));
		}
		
	}
	return NULL;
}

function getAuthnRequestString($callFrom=''){
	
	$issuer=variable_get('saml_sp_entity_id');
	$destination=variable_get('saml_idp_sso_login_url');
	if(!empty($callFrom) && $callFrom=='mobile'){
		$acsURL=variable_get('saml_e1_acs_url')."/mobile";
		$issuer=variable_get('saml_sp_entity_id')."_mobile";
	}else{
		$acsURL=variable_get('saml_e1_acs_url');
		$issuer=variable_get('saml_sp_entity_id');
	}
	
	$msgstr="";
	$msgstr = generateRequestXML($issuer,$destination,$acsURL);
		
	expDebug::dPrint("Request XML -- ".print_r($msgstr,true),4);
	return $msgstr;
}

function sendSAMLAuthRequest($callFrom){
	$msgStr = getAuthnRequestString($callFrom);
	$msgStr = gzdeflate($msgStr);
	$msgStr = base64_encode($msgStr);
	$param = array('SAMLRequest' => $msgStr);
	$relayState=getRelayStateURL();
	if(isset($relayState)){
		$param['RelayState']=$relayState;
	}
	$redirectURL=variable_get('saml_idp_sso_login_url');
	redirect($redirectURL,$param);
	
}

function generateRandomId(){

	$data ='';
	for($i = 0; $i < 20; $i++) {
		$data .= chr(mt_rand(0, 255));
	}
	$ret = '';
	for($i = 0; $i < strlen($data); $i++) {
		$ret .= sprintf('%02x', ord($data[$i]));
	}
	return $ret;
	//return sha1(uniqid(mt_rand(), true));

}

function generateRequestXML($issuer,$destination,$acsURL){
	$Id='a-'.generateRandomId();
	// Store the ID in session for future comparison. 
	// We need to compare this with the "InResponseTo" in the response.
	setcookie('saml_request_id',$Id,0,"","","",TRUE);
	
	$issueTime = gmdate('Y-m-d\TH:i:s\Z', time());
	
	$document = new DOMDocument();
	$document->preserveWhiteSpace = true;
	$document->formatOutput = false;
	
	//Define root element of SAML request
	$root = $document->createElement('samlp:AuthnRequest');
	$root->setAttributeNS('http://www.w3.org/2000/xmlns/','xmlns:samlp','urn:oasis:names:tc:SAML:2.0:protocol');
	$root->setAttribute('AssertionConsumerServiceURL',$acsURL);
	$root->setAttribute('Destination',$destination);
	$root->setAttribute('ID',$Id);
	$root->setAttribute('IssueInstant',$issueTime);
	$root->setAttribute('Version','2.0');
	$document->appendChild($root);
	
	// Define Issuer
	$issuer = $document->createElementNS('urn:oasis:names:tc:SAML:2.0:assertion','saml:Issuer',$issuer);
	$issuer->setAttributeNS('http://www.w3.org/2000/xmlns/','xmlns:saml','urn:oasis:names:tc:SAML:2.0:assertion');
	$root->appendChild($issuer);
	
	$private_key = variable_get('saml_service_provider_private_key','');
	// Add signature
	if($private_key != ''){
		appendSingnature($document, $root, $Id);
	}
	
	return $document->saveXML();
}

function appendSingnature(DOMDocument &$document,DOMElement &$root, $Id){
	// Defined Algorithm. Supports sha1 and sha256
	$algo = variable_get('saml_certificate_algo','sha256');
  
	// Public key of IdP
	$pCertificate = variable_get('saml_idp_public_key','');

	// Define singnature 
	$signature = $document->createElement('ds:Signature');
	//$root->appendChild($signature);
	
	// Define signature informations
	$sigInfo = $document->createElementNS('http://www.w3.org/2000/09/xmldsig#','ds:SignedInfo');
	$sigInfo->setAttributeNS('http://www.w3.org/2000/xmlns/','xmlns:ds','http://www.w3.org/2000/09/xmldsig#');
	$signature->appendChild($sigInfo);
	
	$canon = $document->createElementNS('http://www.w3.org/2000/09/xmldsig#','ds:CanonicalizationMethod');
	$canon->setAttribute('Algorithm','http://www.w3.org/2001/10/xml-exc-c14n#');
	$sigInfo->appendChild($canon);
	
	$sigMethod = $document->createElementNS('http://www.w3.org/2000/09/xmldsig#','ds:SignatureMethod');
	if($algo == 'sha256')
		$sigMethod->setAttribute('Algorithm','http://www.w3.org/2001/04/xmldsig-more#rsa-sha256');
	else
		$sigMethod->setAttribute('Algorithm','http://www.w3.org/2000/09/xmldsig#rsa-sha1');
	$sigInfo->appendChild($sigMethod);
	
	$reference = $document->createElementNS('http://www.w3.org/2000/09/xmldsig#','ds:Reference');
	$reference->setAttribute('URI','#'.$Id);
	$sigInfo->appendChild($reference);
	
	$transforms = $document->createElementNS('http://www.w3.org/2000/09/xmldsig#','ds:Transforms');
	$reference->appendChild($transforms);
	
	$transform = $document->createElementNS('http://www.w3.org/2000/09/xmldsig#','ds:Transform');
	$transform->setAttribute('Algorithm','http://www.w3.org/2000/09/xmldsig#enveloped-signature');
	$transforms->appendChild($transform);
	
	$transform = $document->createElementNS('http://www.w3.org/2000/09/xmldsig#','ds:Transform');
	$transform->setAttribute('Algorithm','http://www.w3.org/2001/10/xml-exc-c14n#');
	$transforms->appendChild($transform);
	
	$digMethod = $document->createElementNS('http://www.w3.org/2000/09/xmldsig#','ds:DigestMethod');
	if($algo == 'sha256')
		$digMethod->setAttribute('Algorithm','http://www.w3.org/2001/04/xmlenc#sha256');
	else
		$digMethod->setAttribute('Algorithm','http://www.w3.org/2000/09/xmldsig#sha1');
	$reference->appendChild($digMethod);
	
	$digest = calculateDigestValue($document,$algo);
	$digMethod = $document->createElementNS('http://www.w3.org/2000/09/xmldsig#','ds:DigestValue',$digest);
	$reference->appendChild($digMethod);
	
	$root->appendChild($signature);
	
	//Add signature value
	$signVal = calculateSignValue($document,$algo);
	$signValue = $document->createElementNS('http://www.w3.org/2000/09/xmldsig#','ds:SignatureValue',$signVal);
	$signature->appendChild($signValue);
	
	$keyInfo = $document->createElementNS('http://www.w3.org/2000/09/xmldsig#','ds:KeyInfo');
	$signature->appendChild($keyInfo);
	
	$xData = $document->createElementNS('http://www.w3.org/2000/09/xmldsig#','ds:X509Data');
	$keyInfo->appendChild($xData);
	
	$certificate = $document->createElementNS('http://www.w3.org/2000/09/xmldsig#','ds:X509Certificate',$pCertificate);
	$xData->appendChild($certificate);
}

// Calculate singature value using openssl private key
function calculateSignValue(DOMDocument $dom, $algo){
	
	// Private key of SP
	$privatekey = variable_get('saml_service_provider_private_key','');
	// $privatekey = "-----BEGIN PRIVATE KEY-----\n".trim($privatekey)."\n-----END PRIVATE KEY-----";
	$privatekey = str_replace(PHP_EOL,'',$privatekey);
	$privatekey = "-----BEGIN PRIVATE KEY-----\n".chunk_split($privatekey, 64, "\n")."-----END PRIVATE KEY-----";
	
	expDebug::dPrint("Private Key -- > ".$privatekey,4);
	$domSignedInfo = $dom->getElementsByTagName("ds:Signature");
	$sigInfo = '';
	foreach($domSignedInfo as $domNode){
		expDebug::dPrint(print_r($domNode,true),4);
		$sigInfo .= DOMinnerHTML($domNode);
	}
	if($algo == 'sha256')
		$algo = OPENSSL_ALGO_SHA256;
	else
		$algo = OPENSSL_ALGO_SHA1;
		
	expDebug::dPrint("SignInfo -- ".print_r($sigInfo,true),4);
	$sig = openssl_sign( $sigInfo , $signatureValue , $privatekey, $algo );
	expDebug::dPrint("Signature value -- > ".base64_encode($signatureValue),4);
	return base64_encode($signatureValue);
}

	//Calculate Digest value
function calculateDigestValue($xml, $algo){
	$canonicalized = $xml->C14N(); // for the whole document  
	$digest = base64_encode(pack("H*", hash($algo,$canonicalized)));
	
	return $digest;
}

function DOMinnerHTML(DOMNode $element) 
{ 
    $innerHTML = ""; 
    $children  = $element->childNodes;

    foreach ($children as $child){ 
      $innerHTML .= $element->ownerDocument->saveHTML($child);
    }

    return $innerHTML; 
} 

?>