<?php

/**
 * @file
 * A single location to store configuration.
 */


//Retrive OAuth2 client details from exp_sp.ini file
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/GlobalUtil.php";
$util=new GlobalUtil();
$config=$util->getConfig();

define('CLIENT_ID',$config['oauth_client_id']);
define('CLIENT_SECRET',$config['oauth_client_secret']);

$hostname="";
if(isset($_SERVER["HTTPS"]) &&$_SERVER["HTTPS"] == "on" )
    $pageURL="https://";
else
    $pageURL="http://";
$hostname= $pageURL.$_SERVER['SERVER_NAME'];
	    

define('OAUTH_CALLBACK', $hostname.'/apis/expertusone_oauth/examples/callback.php');


function getOAuthClient()
{
	$access_oauth_token="";
	$access_token_secret="";
	$userid="";
	$access_oauth_token=ACCESS_TOKEN;//$_COOKIE["oauth_token"];
	$access_token_secret=ACCESS_TOKEN_SECRET;//$_COOKIE["oauth_token_secret"];
	$userid=$_GET["userid"];//$_COOKIE["userid"];
  
  
	$_GET["oauth_token"]=$access_oauth_token;
	$_GET["oauth_token_secret"]=$access_token_secret;
	$_GET["userid"]=$userid;
	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
	return $connection;
}
?>