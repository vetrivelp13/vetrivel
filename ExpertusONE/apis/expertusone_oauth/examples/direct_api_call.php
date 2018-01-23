<?php 
session_start();

require_once('expertusone_oauth.php');
require_once('config.php');


/* Remove no longer needed request tokens */
unset($_SESSION['oauth_token']);
unset($_SESSION['oauth_token_secret']);

/* If HTTP response is 200 continue otherwise send to connect page to retry */
  /* The user has been verified and the access tokens can be saved for future use */
  $_SESSION['status'] = 'verified';

  $access_oauth_token=$_COOKIE["oauth_token"];
  $access_token_secret=$_COOKIE["oauth_token_secret"];
  $userid=$_COOKIE["userid"];
  
  
  $_GET["oauth_token"]=$access_oauth_token;
  $_GET["oauth_token_secret"]=$access_token_secret;
  $_GET["userid"]=$userid;
  
  
  $connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
  $content = $connection->get("/apis/learner/enrollments/get_enrollments_api",$_GET);
  echo $content;
  ?>
  
		
