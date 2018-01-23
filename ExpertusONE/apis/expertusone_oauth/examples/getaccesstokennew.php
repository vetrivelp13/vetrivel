<?php
/**
 * @file
 * Take the user when they return from ExpertusONE. Get access tokens.
 * Verify credentials and redirect to based on response from ExpertusONE.
 */
//echo "welcome";
/* Start session and load lib */
header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

	session_start();
	require_once('../lib/expertusone_oauth.php');
	require_once('../config.php');
	$access_token= array();
	// Create ExpertusONE_OAuth object with app key/secret and token key/secret from default phase 
	$connection = new ExpertusONE_OAuth($_REQUEST["consumer_key"],$_REQUEST["consumer_secret"], $_REQUEST['oauth_token'],  $_REQUEST['oauth_token_secret']);
	// Request access tokens from ExpertusONE 
	$access_token = $connection->getAccessToken($_REQUEST['oauth_verifier']);
	echo "oauth_token=".$access_token["oauth_token"]."&oauth_token_secret=".$access_token["oauth_token_secret"];
	
  ?>
  