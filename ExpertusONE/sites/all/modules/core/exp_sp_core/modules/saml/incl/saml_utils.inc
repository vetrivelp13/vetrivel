<?php  

function loadPrivateKey(){
	
	$cryptoKey = variable_get('saml_private_certificate_path','');
	$cryptoKeyPass = variable_get('saml_private_certificate_password','');
	
	if(empty($cryptoKey)){
		throw new Exception('Unable to locate the key file. Check your settings');
	}
	
	
	$key = new XMLSecurityKey(XMLSecurityKey::RSA_1_5, array('type'=>'private'));
	
	if (!empty($cryptoKeyPass)) {
		$key->passphrase = $cryptoKeyPass;
	}

	$key->loadKey($cryptoKey,/*isFile*/TRUE);
	
	
	return $key;
}

function decryptSAMLAssertion($encryptedData){

	try{
	
		$inputKey=loadPrivateKey();
	
		$blacklist = array();
		
		
		$enc = new XMLSecEnc();
		
		$enc->setNode($encryptedData);
		$enc->type = $encryptedData->getAttribute("Type");

		$symmetricKey = $enc->locateKey($encryptedData);
		
		if (!$symmetricKey) {
			throw new Exception('Could not locate key algorithm in encrypted data.');
		}

		$symmetricKeyInfo = $enc->locateKeyInfo($symmetricKey);
		if (!$symmetricKeyInfo) {
			throw new Exception('Could not locate <dsig:KeyInfo> for the encrypted key.');
		}
		
		
		$inputKeyAlgo = $inputKey->getAlgorith();
		if ($symmetricKeyInfo->isEncrypted) {
			$symKeyInfoAlgo = $symmetricKeyInfo->getAlgorith();

			if (in_array($symKeyInfoAlgo, $blacklist, TRUE)) {
				throw new Exception('Algorithm disabled: ' . var_export($symKeyInfoAlgo, TRUE));
			}

			if ($symKeyInfoAlgo === XMLSecurityKey::RSA_OAEP_MGF1P && $inputKeyAlgo === XMLSecurityKey::RSA_1_5) {
				/*
				 * The RSA key formats are equal, so loading an RSA_1_5 key
				 * into an RSA_OAEP_MGF1P key can be done without problems.
				 * We therefore pretend that the input key is an
				 * RSA_OAEP_MGF1P key.
				 */
				$inputKeyAlgo = XMLSecurityKey::RSA_OAEP_MGF1P;
			}

			/* Make sure that the input key format is the same as the one used to encrypt the key. */
			
			if ($inputKeyAlgo !== $symKeyInfoAlgo) {
				throw new Exception('Algorithm mismatch between input key and key used to encrypt ' .
					' the symmetric key for the message. Key was: ' .
					var_export($inputKeyAlgo, TRUE) . '; message was: ' .
					var_export($symKeyInfoAlgo, TRUE));
			}

			$encKey = $symmetricKeyInfo->encryptedCtx;
			$symmetricKeyInfo->key = $inputKey->key;
			

			$keySize = $symmetricKey->getSymmetricKeySize();
			
			if ($keySize === NULL) {
				/* To protect against "key oracle" attacks, we need to be able to create a
				 * symmetric key, and for that we need to know the key size.
				 */
				throw new Exception('Unknown key size for encryption algorithm: ' . var_export($symmetricKey->type, TRUE));
			}

			try {
				$key = $encKey->decryptKey($symmetricKeyInfo);
				if (strlen($key) != $keySize) {
					throw new Exception('Unexpected key size (' . strlen($key) * 8 . 'bits) for encryption algorithm: ' .
										var_export($symmetricKey->type, TRUE));
				}
			} catch (Exception $e) {
				/* We failed to decrypt this key. Log it, and substitute a "random" key. */
				
				/* Create a replacement key, so that it looks like we fail in the same way as if the key was correctly padded. */

				/* We base the symmetric key on the encrypted key and private key, so that we always behave the
				 * same way for a given input key.
				 */
				 
				
				$encryptedKey = $encKey->getCipherValue();
				$pkey = openssl_pkey_get_details($symmetricKeyInfo->key);
				$pkey = sha1(serialize($pkey), TRUE);
				$key = sha1($encryptedKey . $pkey, TRUE);

				/* Make sure that the key has the correct length. */
				if (strlen($key) > $keySize) {
					$key = substr($key, 0, $keySize);
				} elseif (strlen($key) < $keySize) {
					$key = str_pad($key, $keySize);
				}
			}
			$symmetricKey->loadkey($key);

		} else {
			$symKeyAlgo = $symmetricKey->getAlgorith();
			/* Make sure that the input key has the correct format. */
			
			if ($inputKeyAlgo !== $symKeyAlgo) {
				throw new Exception('Algorithm mismatch between input key and key in message. ' .
					'Key was: ' . var_export($inputKeyAlgo, TRUE) . '; message was: ' .
					var_export($symKeyAlgo, TRUE));
			}
			$symmetricKey = $inputKey;
		}

		$algorithm = $symmetricKey->getAlgorith();
		if (in_array($algorithm, $blacklist, TRUE)) {
			throw new Exception('Algorithm disabled: ' . var_export($algorithm, TRUE));
		}

		$decrypted = $enc->decryptNode($symmetricKey, FALSE);
		/*
		 * This is a workaround for the case where only a subset of the XML
		 * tree was serialized for encryption. In that case, we may miss the
		 * namespaces needed to parse the XML.
		 */
		$xml = '<root xmlns:samlp="urn:oasis:names:tc:SAML:2.0:protocol" xmlns:saml="urn:oasis:names:tc:SAML:2.0:assertion">'.$decrypted.'</root>';
		$newDoc = new DOMDocument();
		if (!@$newDoc->loadXML($xml)) {
			throw new Exception('Failed to parse decrypted XML. Maybe the wrong sharedkey was used?');
		}
		$newDoc->formatOutput=TRUE;
		
		$decryptedElement = $newDoc->firstChild->firstChild;
		
		
		if ($decryptedElement === NULL) {
			throw new Exception('Missing encrypted element.');
		}

		if (!($decryptedElement instanceof DOMElement)) {
			throw new Exception('Decrypted element was not actually a DOMElement.');
		}
		
		return $decryptedElement;
		
	
	}catch(Exception $e){
		
		throw new Exception('Decryption failed: ' . $e->getMessage());
					
	}
	
	
}

function getAcsURL()
{
	global $base_url;
	// Changes done for 54709: In multinode setup, the ACS URL is in 'http' mode in SAML settings pop-up.
	
	/* $protocol='http';
	if(!array_key_exists('HTTPS', $_SERVER)){
		//	Not an https-request.
		return 'http://'.$_SERVER['SERVER_NAME'].'/?q=saml20/acs';
	}

	if($_SERVER['HTTPS'] === 'off') {
		//	IIS with HTTPS off.
		return 'http://'.$_SERVER['SERVER_NAME'].'/?q=saml20/acs';
	}

	//	Otherwise, HTTPS will be a non-empty string.
	if($_SERVER['HTTPS'] !== ''){
		return 'https://'.$_SERVER['SERVER_NAME'].'/?q=saml20/acs';
	};
	
	return 'http://'.$_SERVER['SERVER_NAME'].'/?q=saml20/acs'; */
	return $base_url.'/?q=saml20/acs';
}

function getXPathObject(DOMNode $node)
{
	if(empty($node)){
		return;
    }
	if ($node instanceof DOMDocument) {
		$doc = $node;
	} else {
		$doc = $node->ownerDocument;
	}
	
	$xPathObj=new DOMXPath($doc);
	$xPathObj->registerNamespace('saml_protocol', SAML2_Const::NS_SAMLP);
	$xPathObj->registerNamespace('saml_assertion', SAML2_Const::NS_SAML);
	$xPathObj->registerNamespace('saml_metadata', SAML2_Const::NS_MD);
	$xPathObj->registerNamespace('xenc',SAML2_Const::NS_XML_ENC);
	$xPathObj->registerNamespace('ds',SAML2_Const::NS_XML_DS);
	$xPathObj->registerNamespace('ds', XMLSecurityDSig::XMLDSIGNS);
	$xPathObj->registerNamespace('xenc', XMLSecEnc::XMLENCNS);
	return $xPathObj;
	
}

function queryXPath(DOMXPath $xPath,$node,$query){

	if(empty($xPath) || empty($query) || empty($node))
	{
		return;
	}
	$results = $xPath->query($query, $node);
	$ret = array();
	for ($i = 0; $i < $results->length; $i++) {
		$ret[$i] = $results->item($i);
	}

	return $ret;

	}
	
	/**
 * parseSAML2Time is from simpleSAMLphp Utilities
 *
 * This function converts a SAML2 timestamp on the form
 * yyyy-mm-ddThh:mm:ss(\.s+)?Z to a UNIX timestamp. The sub-second
 * part is ignored.
 *
 * Andreas comments:
 *  I got this timestamp from Shibboleth 1.3 IdP: 2008-01-17T11:28:03.577Z
 *  Therefore I added to possibliity to have microseconds to the format.
 * Added: (\.\\d{1,3})? to the regex.
 *
 *
 * @param string $time  The time to convert in SAML2 format
 * @return string  $time converted to a unix timestamp.
 */
function parseSAML2Time($time) {
  $matches = array();


  /* We use a very strict regex to parse the timestamp. */
  if (preg_match('/^(\\d\\d\\d\\d)-(\\d\\d)-(\\d\\d)' .
                  'T(\\d\\d):(\\d\\d):(\\d\\d)(?:\\.\\d+)?Z$/D',
                  $time, $matches) == 0) {
    throw new Exception(
            'Invalid SAML2 timestamp passed to' .
            ' parseSAML2Time: ' . $time);
  }

  /* Extract the different components of the time from the
   * matches in the regex. intval will ignore leading zeroes
   * in the string.
   */
  $year = intval($matches[1]);
  $month = intval($matches[2]);
  $day = intval($matches[3]);
  $hour = intval($matches[4]);
  $minute = intval($matches[5]);
  $second = intval($matches[6]);

  /* We use gmmktime because the timestamp will always be given
   * in UTC.
   */
  $ts = gmmktime($hour, $minute, $second, $month, $day, $year);

  return $ts;
}

/**
 * checkCurrentTime is from simpleSAMLphp Utilities
 *
 * Check to verify that the current time is between
 * the specified start and end boundary
 *
 * @param string $start time in SAML2 format
 * @param string $end time in SAML2 format
 * @return boolean
 */
function checkCurrentTime($start=NULL, $end=NULL) {
  $currentTime = time();

  if (!empty($start)) {
    $startTime = parseSAML2Time($start);
    /* Allow for a 10 minute difference in Time */
    if (($startTime < 0) || (($startTime - 600) > $currentTime)) {
      return FALSE;
    }
  }
  if (!empty($end)) {
    $endTime = parseSAML2Time($end);
    if (($endTime < 0) || ($endTime <= $currentTime)) {
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * Determine if the SSO login link is accessible.
 *
 * This is a menu access callback.
 */
function e1_sso_login_accessible() {
  if(module_exists('saml') && (arg(3) != 'widget' && !(isset($_SESSION['widgetCallback']) && $_SESSION['widgetCallback']==TRUE))){
  
	$c=variable_get('saml_idp_sso_login_url');
	$d = variable_get('saml_sp_entity_id');
	return !empty($c) && !empty($d) && user_is_anonymous();
  }
  
  return FALSE;
}
	
class SAML2_Const {

	/**
	 * Password authentication context.
	 */
	const AC_PASSWORD = 'urn:oasis:names:tc:SAML:2.0:ac:classes:Password';

	/**
	 * Unspecified authentication context.
	 */
	const AC_UNSPECIFIED = 'urn:oasis:names:tc:SAML:2.0:ac:classes:unspecified';


	/**
	 * The URN for the HTTP-POST binding.
	 */
	const BINDING_HTTP_POST = 'urn:oasis:names:tc:SAML:2.0:bindings:HTTP-POST';

	/**
	 * The URN for the HTTP-Redirect binding.
	 */
	const BINDING_HTTP_REDIRECT = 'urn:oasis:names:tc:SAML:2.0:bindings:HTTP-Redirect';

	/**
	 * The URN for the HTTP-ARTIFACT binding.
	 */
	const BINDING_HTTP_ARTIFACT = 'urn:oasis:names:tc:SAML:2.0:bindings:HTTP-Artifact';

	/**
	 * The URN for the SOAP binding.
	 */
	const BINDING_SOAP = 'urn:oasis:names:tc:SAML:2.0:bindings:SOAP';

	/**
	 * The URN for the Holder-of-Key Web Browser SSO Profile binding
	 */
	const BINDING_HOK_SSO = 'urn:oasis:names:tc:SAML:2.0:profiles:holder-of-key:SSO:browser';


	/**
	 * Bearer subject confirmation method.
	 */
	const CM_BEARER = 'urn:oasis:names:tc:SAML:2.0:cm:bearer';

	/**
	* Holder-of-Key subject confirmation method.
	*/
	const CM_HOK = 'urn:oasis:names:tc:SAML:2.0:cm:holder-of-key';


	/**
	 * The URN for the unspecified attribute NameFormat.
	 */
	const NAMEFORMAT_UNSPECIFIED = 'urn:oasis:names:tc:SAML:2.0:attrname-format:unspecified';


	/**
	 * Unspecified NameID format.
	 */
	const NAMEID_UNSPECIFIED = 'urn:oasis:names:tc:SAML:1.1:nameid-format:unspecified';

	/**
	 * Persistent NameID format.
	 */
	const NAMEID_PERSISTENT = 'urn:oasis:names:tc:SAML:2.0:nameid-format:persistent';

	/**
	 * Transient NameID format.
	 */
	const NAMEID_TRANSIENT = 'urn:oasis:names:tc:SAML:2.0:nameid-format:transient';

	/**
	 * Encrypted NameID format.
	 */
	const NAMEID_ENCRYPTED = 'urn:oasis:names:tc:SAML:2.0:nameid-format:encrypted';


	/**
	 * The namespace for the SOAP protocol.
	 */
	const NS_SOAP = 'http://schemas.xmlsoap.org/soap/envelope/';

	/**
	 * The namespace for the SAML 2 protocol.
	 */
	const NS_SAMLP = 'urn:oasis:names:tc:SAML:2.0:protocol';

	/**
	 * The namespace for the SAML 2 assertions.
	 */
	const NS_SAML = 'urn:oasis:names:tc:SAML:2.0:assertion';

	/**
	 * The namespace for the SAML 2 metadata.
	 */
	const NS_MD = 'urn:oasis:names:tc:SAML:2.0:metadata';

	/**
	 * The namespace fox XML schema.
	 */
	const NS_XS = 'http://www.w3.org/2001/XMLSchema';

	/**
	 * The namespace for XML schema instance.
	 */
	const NS_XSI = 'http://www.w3.org/2001/XMLSchema-instance';

	/**
	 * The namespace for the SAML 2 HoK Web Browser SSO Profile.
	 */
	const NS_HOK = 'urn:oasis:names:tc:SAML:2.0:profiles:holder-of-key:SSO:browser';

	/**
	 * Top-level status code indicating successful processing of the request.
	 */
	const STATUS_SUCCESS = 'urn:oasis:names:tc:SAML:2.0:status:Success';

	/**
	 * Top-level status code indicating that there was a problem with the request.
	 */
	const STATUS_REQUESTER = 'urn:oasis:names:tc:SAML:2.0:status:Requester';

	/**
	 * Top-level status code indicating that there was a problem generating the response.
	 */
	const STATUS_RESPONDER = 'urn:oasis:names:tc:SAML:2.0:status:Responder';

	/**
	 * Top-level status code indicating that the request was from an unsupported version of the SAML protocol.
	 */
	const STATUS_VERSION_MISMATCH = 'urn:oasis:names:tc:SAML:2.0:status:VersionMismatch';


	/**
	 * Second-level status code for NoPassive errors.
	 */
	const STATUS_NO_PASSIVE = 'urn:oasis:names:tc:SAML:2.0:status:NoPassive';

	/**
	 * Second-level status code for PartialLogout.
	 */
	const STATUS_PARTIAL_LOGOUT = 'urn:oasis:names:tc:SAML:2.0:status:PartialLogout';

	/**
	 * Second-level status code for ProxyCountExceeded.
	 */
	const STATUS_PROXY_COUNT_EXCEEDED = 'urn:oasis:names:tc:SAML:2.0:status:ProxyCountExceeded';
	
	const NS_XML_ENC = 'http://www.w3.org/2001/04/xmlenc#';
	
	const NS_XML_DS = 'http://www.w3.org/2000/09/xmldsig#';
}

?>
