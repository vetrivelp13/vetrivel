<?php

require_once 'init.php';
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/Trace.php";
$server = new OAuthServer();

switch($_SERVER['PATH_INFO'])
{
case '/request_token':
	$verifierwithrequesttoken_flag="no";
	if(isset($_REQUEST["flag"]))
	{
		$verifierwithrequesttoken_flag=$_REQUEST["flag"];
	}
	else
	{
		$verifierwithrequesttoken_flag="no";	
	}
	
	if($verifierwithrequesttoken_flag=="yes")
	{
			expDebug::dPrint("inside if");
		
		try
		{
			$server->requestTokenWithOAuth_verifier();
			exit;
			/*expDebug::dPrint("Token before ".$_REQUEST["token"]);
			expDebug::dPrint("Token secret before  ".$_REQUEST["token_secret"]);
			
			$server->authorizeVerify();
			$oauth_verifier=$server->authorizeFinish(true, 1);
			expDebug::dPrint("oauth::".$oauth_verifier);
			$_GET["oauth_verifier"]=$oauth_verifier;
			
			$oauth_verifier="oauth_verifier=".$oauth_verifier;
			header('HTTP/1.1 200 OK');
			header('Content-Length: '.strlen($oauth_verifier));
			header('Content-Type: application/x-www-form-urlencoded');

			echo $oauth_verifier;
			*/
			
			/*$server->accessToken(); */
			//echo $oauth_verifier;
		}
		catch (OAuthException2 $e)
		{
			expDebug::dPrint("oauth exception::". $e->getMessage());
			header('HTTP/1.1 400 Bad Request');
			header('Content-Type: text/plain');
			
			echo "Failed OAuth Request: " . $e->getMessage();
		}
		exit;

	}
	else
	{
		$server->requestToken();
		exit;
	}
	break;
case '/access_token':
	$server->accessToken();
	exit;

case '/authenticate':
	try
	{
		$_SESSION["userid"]=$_GET["userid"];
		$redirect_uri=$_GET["redirect_uri"];
		
		$tmp="";
		if($redirect_uri!="")
		{
			$tmp=$redirect_uri;//"http://".$redirect_uri;
			$tmp=str_replace("^^^","://",$tmp);	
		}
			//$_SESSION["verify_oauth_callback"]="http://".$redirect_uri;
			
		
		$server->authorizeVerify();
		$server->authorizeFinish(true, 1,$tmp);
	}
	catch (OAuthException2 $e)
	{
		header('HTTP/1.1 400 Bad Request');
		header('Content-Type: text/plain');
		
		echo "Failed OAuth Request: " . $e->getMessage();
	}
	
	exit;
case '/authorize':
	# logon
	//echo "welcome1";
	assert_logged_in();
	
	try
	{
		$server->authorizeVerify();
		
		$server->authorizeFinish(true, 1);
	}
	catch (OAuthException2 $e)
	{
		header('HTTP/1.1 400 Bad Request');
		header('Content-Type: text/plain');
		
		echo "Failed OAuth Request: " . $e->getMessage();
	}
	exit;

	
default:
	header('HTTP/1.1 500 Internal Server Error');
	header('Content-Type: text/plain');
	echo "Unknown request";
}

?>