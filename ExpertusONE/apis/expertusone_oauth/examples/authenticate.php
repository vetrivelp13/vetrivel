<?php

/* Start session and load library. */
session_start();
require_once('../lib/expertusone_oauth.php');
require_once('../config.php');

	
	/* Build ExpertusONE_OAuth object with client credentials. */
	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET);
	 
	/* Get temporary credentials. */
	
	$request_token = $connection->getRequestToken(OAUTH_CALLBACK);
	
	/* Save temporary credentials to session. */
	$_SESSION['oauth_token'] = $token = $request_token['oauth_token'];
	$_SESSION['oauth_token_secret'] = $request_token['oauth_token_secret'];
	 
	/* If last connection failed don't display authorization link. */
	switch ($connection->http_code) {
	  case 200:
	  	
	    /* Build authorize URL and redirect user to ExpertusONE. */
	    $url = $connection->getAuthorizeURL($token,"true");
	    $url=$url."&sitename=www.testportal.com";
	    header('Location: ' . urldecode($url)."&redirect_uri=".rawurlencode("https://www.google.com")); 
	    break;
	  default:
	    /* Show notification if something went wrong. */
	    echo 'Could not connect to ExpertusONE. Refresh the page or try again later.';
	}
