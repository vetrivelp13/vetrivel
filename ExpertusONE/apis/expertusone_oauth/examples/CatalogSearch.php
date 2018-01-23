<?php 
	session_start();

	require_once('../lib/expertusone_oauth.php');
	require_once('../config.php');

	/* retrieve access_token from cookies */
  	$access_oauth_token=ACCESS_TOKEN;
  	$access_token_secret=ACCESS_TOKEN_SECRET;//$_COOKIE["oauth_token_secret"];
  	$userid="1";//$_COOKIE["userid"];
  
  
  	/*set necessary parameters for this api call. */
  	$_GET["oauth_token"]=$access_oauth_token;
  	$_GET["oauth_token_secret"]=$access_token_secret;
  	$_GET["userid"]=$userid;
  	$_GET["display_columns"]="title,code";
  	$_GET["limit"]="10";
  	$_GET["title"]=""; 
	$_GET["actionkey"]="getcatalogs"; 
	$_GET["returntype"]="json";
  	
    /* Create connection using client credentials and access tokens */
  	$connection = new ExpertusONE_OAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_oauth_token,$access_token_secret);
  	$content = $connection->get("http://qa.expertusone.com/apis/learner/catalog/CatalogSearchResults",$_GET);
  	echo $content;
?>
  
		
