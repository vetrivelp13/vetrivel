<?php
/**
 * @file
 * Take the user when they return from ExpertusONE. Get access tokens.
 * Verify credentials and redirect to based on response from ExpertusONE.
 */

/* Start session and load lib */
	session_start();
	require_once('../lib/expertusone_oauth.php');
	require_once('../config.php');

	$access_token= array();

	/* Create ExpertusONE_OAuth object with app key/secret and token key/secret from default phase */
	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $_SESSION['oauth_token'], $_SESSION['oauth_token_secret']);
	
	
	/* Request access tokens from twitter */
	$access_token = $connection->getAccessToken($_REQUEST['oauth_verifier']);
	
	/* Save the access tokens. Normally these would be saved in a database for future use. */
	$_SESSION['access_token'] = $access_token;
	
	/* Remove no longer needed request tokens */
	unset($_SESSION['oauth_token']);
	unset($_SESSION['oauth_token_secret']);
	
	/* If HTTP response is 200 continue otherwise send to connect page to retry */
	if (200 == $connection->http_code) {
	  /* The user has been verified and the access tokens can be saved for future use */
	  $_SESSION['status'] = 'verified';
	
	  
	  setcookie("oauth_token", $access_token['oauth_token'], time()+(3600*24));  /* expire in 24 hours */
	  setcookie("oauth_token_secret", $access_token['oauth_token_secret'], time()+(3600*24));  /* expire in 24 hours */
	  setcookie("userid", $_REQUEST["userid"], time()+(3600*24));  /* expire in 24 hours */
	  
	  $callapi=true;
	  
	}
	else
	{
	 	header('Location: clearsessions.php');
	}
	 if($callapi==true)
	  {
		  echo "Authentication Successful. Call necessary APIs.";
		  echo "<br/> Access Token:".$access_token['oauth_token']." Access Token Secret::".$access_token['oauth_token_secret'];
	  }
	  else
	  {
	  	 echo "Authentication unsuccessful. Contact customer care if it is persist.";
	  }
  ?>
  