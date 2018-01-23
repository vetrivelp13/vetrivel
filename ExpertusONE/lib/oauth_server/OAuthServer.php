<?php

/**
 * Server layer over the OAuthRequest handler
 * 
 * @version $Id: OAuthServer.php 154 2010-08-31 18:04:41Z brunobg@corollarium.com $
 * @author Marc Worrell <marcw@pobox.com>
 * @date  Nov 27, 2007 12:36:38 PM
 * 
 * 
 * The MIT License
 * 
 * Copyright (c) 2007-2008 Mediamatic Lab
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

//require_once 'OAuthRequestVerifier.php';
//require_once 'OAuthSession.php';
include_once 'OAuthRequestVerifier.php';
include_once 'OAuthSession.php';
class OAuthServer extends OAuthRequestVerifier
{
	protected $session;
	
	protected $allowed_uri_schemes = array(
		'http',
		'https',
		'x-expertusone-oauth',
		'callback'
	);

	protected $disallowed_uri_schemes = array(
		'file',
		'callto',
		'mailto'
	);

	/**
	 * Construct the request to be verified
	 * 
	 * @param string request
	 * @param string method
	 * @param array params The request parameters
	 * @param string store The session storage class.
	 * @param array store_options The session storage class parameters.
	 * @param array options Extra options:
	 *   - allowed_uri_schemes: list of allowed uri schemes.
	 *   - disallowed_uri_schemes: list of unallowed uri schemes.
	 * 
	 * e.g. Allow only http and https
	 * $options = array(
	 *     'allowed_uri_schemes' => array('http', 'https'),
	 *     'disallowed_uri_schemes' => array()
	 * );
	 * 
	 * e.g. Disallow callto, mailto and file, allow everything else
	 * $options = array(
	 *     'allowed_uri_schemes' => array(),
	 *     'disallowed_uri_schemes' => array('callto', 'mailto', 'file')
	 * );
	 * 
	 * e.g. Allow everything
	 * $options = array(
	 *     'allowed_uri_schemes' => array(),
	 *     'disallowed_uri_schemes' => array()
	 * ); 
	 *  
	 */
	function __construct ( $uri = null, $method = null, $params = null, $store = 'SESSION', 
			$store_options = array(), $options = array() )
	{
 		parent::__construct($uri, $method, $params);
 		$this->session = OAuthSession::instance($store, $store_options);
 		
	 	if (array_key_exists('allowed_uri_schemes', $options) && is_array($options['allowed_uri_schemes'])) {
	 		$this->allowed_uri_schemes = $options['allowed_uri_schemes'];
	 	}
	 	if (array_key_exists('disallowed_uri_schemes', $options) && is_array($options['disallowed_uri_schemes'])) {
	 		$this->disallowed_uri_schemes = $options['disallowed_uri_schemes'];
	 	}
	}
	
	/**
	 * Handle the request_token request.
	 * Returns the new request token and request token secret.
	 * 
	 * TODO: add correct result code to exception
	 * 
	 * @return string 	returned request token, false on an error
	 */
	public function requestToken ()
	{
		OAuthRequestLogger::start($this);
		try
		{
			$this->verify(false);
			
			$options = array();
			$ttl     = $this->getParam('xoauth_token_ttl', false);
			if ($ttl)
			{
				$options['token_ttl'] = $ttl;
			}

 			// 1.0a Compatibility : associate callback url to the request token
 			$cbUrl   = $this->getParam('oauth_callback', true);
 			if ($cbUrl) {
 				$options['oauth_callback'] = $cbUrl;
 			}
			
			// Create a request token
			$store  = OAuthStore::instance();
			$token  = $store->addConsumerRequestToken($this->getParam('oauth_consumer_key', true), $options);
			$result = 'oauth_callback_confirmed=1&oauth_token='.$this->urlencode($token['token'])
					.'&oauth_token_secret='.$this->urlencode($token['token_secret']);

			if (!empty($token['token_ttl']))
			{
				$result .= '&xoauth_token_ttl='.$this->urlencode($token['token_ttl']);
			}

			$request_token = $token['token'];
			
			header('HTTP/1.1 200 OK');
			header('Content-Length: '.strlen($result));
			header('Content-Type: application/x-www-form-urlencoded');
			expDebug::dPrint("request token....".$result);
			echo $result;
		}
		catch (OAuthException2 $e)
		{
			$request_token = false;

			header('HTTP/1.1 401 Unauthorized');
			header('Content-Type: text/plain');

			echo "OAuth Verification Failed: " . $e->getMessage();
		}

		OAuthRequestLogger::flush();
		return $request_token;
	}
	
	
	/**
	 * Handle the request_token request.
	 * Returns the new request token and request token secret.
	 * 
	 * TODO: add correct result code to exception
	 * 
	 * @return string 	returned request token, false on an error
	 */
	public function requestTokenWithOAuth_verifier()
	{
			
			$this->authorizeVerify();
			expDebug::dPrint("test 111");
			include_once $_SERVER["DOCUMENT_ROOT"]."/apis/core/login/login_dao.php";
			expDebug::dPrint("test 1112");
			$username="";
			$email="";
			$pwd="";
			$username=$_REQUEST["username"];
			$email=$_REQUEST["email"];
			$pwd=$_REQUEST["password"];
			//start of changes for sales force - by Kannank - To auto login in LMS through salesforce mobile user
			$salesforce="";
			$salesforce=$_REQUEST["salesforce"];
			//End of changes for sales force - by Kannank
			$sso =$_REQUEST["sso"];
			
			$paramsObj=new stdClass();
			$paramsObj->username=$username;//"suresh";
			$paramsObj->email=$email;
			$paramsObj->password=$pwd;//"welcome";
			$paramsObj->sso = $sso;
			
			//start of changes for sales force - by Kannank - To auto login in LMS through salesforce mobile user
			$paramsObj->salesforce = $salesforce;
			//End of changes for sales force - by Kannank
			
			$dao=new LoginDAO();
			expDebug::dPrint("test 1113");
			$result=$dao->doLogin($paramsObj);
			$uid="";
			$roleid="";
			$len=0;
			$len = count($result);
			 //print_r($result);
			 for($i=0;$i<$len;$i++)
			 {
			 		$row=$result[$i];
		  			$uid=$row["USERID"];
		  			$roleid=$row["roleid"];
			 }
			 $error="";
			 if($uid=="" || $uid==null)
			 {
			 	$error='Given credentials are not correct.';
			 }
			 
			 
			 if($error!="")
			 {
			 	$result="error=".$error."&userid=".$uid."&roleid=".$roleid."&oauth_token=".$_REQUEST["oauth_token"]."&oauth_token_secret=".$_REQUEST["oauth_token_secret"]."&oauth_verifier=null";
				expDebug::dPrint("API Login Response.".$result);
			 	header('HTTP/1.1 200 OK');
				header('Content-Length: '.strlen($result));
				header('Content-Type: application/x-www-form-urlencoded');
				echo $result;
			 }
			 else
			 {
				expDebug::dPrint("test uid.".$uid);
				
				//$oauth_verifi=$this->authorizeFinishMobile(true, 1);
				$oauth_verifi=$this->authorizeFinishMobile(true, $uid);
			
				
				//expDebug::dPrint("test verfiier.".$oauth_verifi);
				$result="error=&userid=".$uid."&roleid=".$roleid."&oauth_token=".$_REQUEST["oauth_token"]."&oauth_token_secret=".$_REQUEST["oauth_token_secret"]."&oauth_verifier=".$oauth_verifi;
				expDebug::dPrint("API Login Response.".$result);
				expDebug::dPrint("test verfiier.".$result);
				header('HTTP/1.1 200 OK');
				header('Content-Length: '.strlen($result));
				header('Content-Type: application/x-www-form-urlencoded');
				echo $result;
			 }
	}
	
	
	public function setRedirectURL($url)
	{
		$this->session->set('verify_oauth_callback', $url);
	}
	
	/**
	 * Verify the start of an authorization request.  Verifies if the request token is valid.
	 * Next step is the method authorizeFinish()
	 * 
	 * Nota bene: this stores the current token, consumer key and callback in the _SESSION
	 * 
	 * @exception OAuthException2 thrown when not a valid request
	 * @return array token description
	 */
	public function authorizeVerify ()
	{
		OAuthRequestLogger::start($this);

		$store = OAuthStore::instance();
		$token = $this->getParam('oauth_token', true);
		$rs    = $store->getConsumerRequestToken($token);
		if (empty($rs))
		{
			throw new OAuthException2('Unknown request token "'.$token.'"');
		}

		// We need to remember the callback
		$verify_oauth_token = $this->session->get('verify_oauth_token');		
		if (	empty($verify_oauth_token)
			||	strcmp($verify_oauth_token, $rs['token']))
		{
			$this->session->set('verify_oauth_token', $rs['token']);
			$this->session->set('verify_oauth_consumer_key', $rs['consumer_key']);
			$cb = $this->getParam('oauth_callback', true); 
			if ($cb)
				$this->session->set('verify_oauth_callback', $cb);
			else
				$this->session->set('verify_oauth_callback', $rs['callback_url']);
		}
		OAuthRequestLogger::flush();
		return $rs;
	}
	
	
/**
	 * Overrule this method when you want to display a nice page when
	 * the authorization is finished.  This function does not know if the authorization was
	 * succesfull, you need to check the token in the database.
	 * 
	 * @param boolean authorized	if the current token (oauth_token param) is authorized or not
	 * @param int user_id			user for which the token was authorized (or denied)
	 * @return string verifier  For 1.0a Compatibility
	 */
	public function authorizeFinishMobile ( $authorized, $user_id )
	{
		OAuthRequestLogger::start($this);
	expDebug::dPrint("test1");
		$token = $this->getParam('oauth_token', true);
expDebug::dPrint("test2");
		$verifier = null;
		if ($this->session->get('verify_oauth_token') == $token)
		{
			expDebug::dPrint("test3");
			// Flag the token as authorized, or remove the token when not authorized
			$store = OAuthStore::instance();
			expDebug::dPrint("test4");
			// Fetch the referrer host from the oauth callback parameter
			$referrer_host  = '';
			$oauth_callback = false;
			$verify_oauth_callback = $this->session->get('verify_oauth_callback');
			if (!empty($verify_oauth_callback) && $verify_oauth_callback != 'oob') // OUT OF BAND
			{
				expDebug::dPrint("test5");
				$oauth_callback = $this->session->get('verify_oauth_callback');
				$ps = parse_url($oauth_callback);
				if (isset($ps['host']))
				{
					expDebug::dPrint("test6");
					$referrer_host = $ps['host'];
				}
			}
			
			if ($authorized)
			{
				expDebug::dPrint("test7");
				OAuthRequestLogger::addNote('Authorized token "'.$token.'" for user '.$user_id.' with referrer "'.$referrer_host.'"');
 				// 1.0a Compatibility : create a verifier code
				$verifier = $store->authorizeConsumerRequestToken($token, $user_id, $referrer_host);
			}
			else
			{
				expDebug::dPrint("test8");
				OAuthRequestLogger::addNote('Authorization rejected for token "'.$token.'" for user '.$user_id."\nToken has been deleted");
				$store->deleteConsumerRequestToken($token);
			}
			
			if (!empty($oauth_callback))
			{
				expDebug::dPrint("test9");
				$params = array('oauth_token' => rawurlencode($token));
 				// 1.0a Compatibility : if verifier code has been generated, add it to the URL
 				if ($verifier) {
					expDebug::dPrint("test10");
 					$params['oauth_verifier'] = $verifier;
 				}
 				
				$uri = preg_replace('/\s/', '%20', $oauth_callback);
				expDebug::dPrint("uri ".$uri);
				if (!empty($this->allowed_uri_schemes)) 
				{
					expDebug::dPrint("test11");
					if (!in_array(substr($uri, 0, strpos($uri, '://')), $this->allowed_uri_schemes)) 
					{
						throw new OAuthException2('Illegal protocol in redirect uri '.$uri);
					}
				} 
				else if (!empty($this->disallowed_uri_schemes)) 
				{
					if (in_array(substr($uri, 0, strpos($uri, '://')), $this->disallowed_uri_schemes))
					{
						throw new OAuthException2('Illegal protocol in redirect uri '.$uri);
					}
				}
				//echo $_SESSION["userid"]; exit();
				expDebug::dPrint("test12");
				$params["userid"]=$_SESSION["userid"];
 				//$this->redirect($oauth_callback, $params);
			}
		}
		OAuthRequestLogger::flush();
		return $verifier;
	}
	
	/**
	 * Overrule this method when you want to display a nice page when
	 * the authorization is finished.  This function does not know if the authorization was
	 * succesfull, you need to check the token in the database.
	 * 
	 * @param boolean authorized	if the current token (oauth_token param) is authorized or not
	 * @param int user_id			user for which the token was authorized (or denied)
	 * @return string verifier  For 1.0a Compatibility
	 */
	public function authorizeFinish ( $authorized, $user_id,$overrideRedirectURL="" )
	{
		OAuthRequestLogger::start($this);

		$token = $this->getParam('oauth_token', true);
		$verifier = null;
		if ($this->session->get('verify_oauth_token') == $token)
		{
			// Flag the token as authorized, or remove the token when not authorized
			$store = OAuthStore::instance();

			// Fetch the referrer host from the oauth callback parameter
			$referrer_host  = '';
			$oauth_callback = false;
			if($overrideRedirectURL!="")
				$verify_oauth_callback=$overrideRedirectURL;
			else
				$verify_oauth_callback = $this->session->get('verify_oauth_callback');
				
			
			if (!empty($verify_oauth_callback) && $verify_oauth_callback != 'oob') // OUT OF BAND
			{
				$oauth_callback = $this->session->get('verify_oauth_callback');
				if($overrideRedirectURL!="")
					$oauth_callback=$overrideRedirectURL;
				
				$ps = parse_url($oauth_callback);
				if (isset($ps['host']))
				{
					$referrer_host = $ps['host'];
				}
			}
			
			if ($authorized)
			{
				OAuthRequestLogger::addNote('Authorized token "'.$token.'" for user '.$user_id.' with referrer "'.$referrer_host.'"');
 				// 1.0a Compatibility : create a verifier code
				$verifier = $store->authorizeConsumerRequestToken($token, $user_id, $referrer_host);
			}
			else
			{
				OAuthRequestLogger::addNote('Authorization rejected for token "'.$token.'" for user '.$user_id."\nToken has been deleted");
				$store->deleteConsumerRequestToken($token);
			}
			
			if (!empty($oauth_callback))
			{
 				$params = array('oauth_token' => rawurlencode($token));
 				// 1.0a Compatibility : if verifier code has been generated, add it to the URL
 				if ($verifier) {
 					$params['oauth_verifier'] = $verifier;
 				}
 				
				$uri = preg_replace('/\s/', '%20', $oauth_callback);
				if (!empty($this->allowed_uri_schemes)) 
				{
					if (!in_array(substr($uri, 0, strpos($uri, '://')), $this->allowed_uri_schemes)) 
					{
						throw new OAuthException2('Illegal protocol in redirect uri '.$uri);
					}
				} 
				else if (!empty($this->disallowed_uri_schemes)) 
				{
					if (in_array(substr($uri, 0, strpos($uri, '://')), $this->disallowed_uri_schemes))
					{
						throw new OAuthException2('Illegal protocol in redirect uri '.$uri);
					}
				}
				//echo $_SESSION["userid"]; exit();
				$params["userid"]=$_SESSION["userid"];
 				$this->redirect($oauth_callback, $params);
			}
		}
		OAuthRequestLogger::flush();
		return $verifier;
	}
	
	
	/**
	 * Exchange a request token for an access token.
	 * The exchange is only succesful iff the request token has been authorized.
	 * 
	 * Never returns, calls exit() when token is exchanged or when error is returned.
	 */
	public function accessToken ()
	{
		OAuthRequestLogger::start($this);

		try
		{
			$this->verify('request');

			$options = array();
			$ttl     = $this->getParam('xoauth_token_ttl', false);
			if ($ttl)
			{
				$options['token_ttl'] = $ttl;
			}

			$verifier = $this->getParam('oauth_verifier', false);
 			if ($verifier) {
 				$options['verifier'] = $verifier;
 			}
			
			$store  = OAuthStore::instance();
			$token  = $store->exchangeConsumerRequestForAccessToken($this->getParam('oauth_token', true), $options);
			$result = 'oauth_token='.$this->urlencode($token['token'])
					.'&oauth_token_secret='.$this->urlencode($token['token_secret']);
					
			if (!empty($token['token_ttl']))
			{
				$result .= '&xoauth_token_ttl='.$this->urlencode($token['token_ttl']);
			}
					
			header('HTTP/1.1 200 OK');
			header('Content-Length: '.strlen($result));
			header('Content-Type: application/x-www-form-urlencoded');

			echo $result;
		}
		catch (OAuthException2 $e)
		{
			header('HTTP/1.1 401 Access Denied');
			header('Content-Type: text/plain');

			echo "OAuth Verification Failed: " . $e->getMessage();
		}
		
		OAuthRequestLogger::flush();
		exit();
	}	
}

/* vi:set ts=4 sts=4 sw=4 binary noeol: */

?>