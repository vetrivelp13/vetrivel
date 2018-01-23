<?php

//session_start();
include_once $_SERVER["DOCUMENT_ROOT"].'/apis/expertusone_oauth/lib/expertusone_oauth.php';
include_once $_SERVER["DOCUMENT_ROOT"].'/apis/expertusone_oauth/config.php';
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/GlobalUtil.php";
// Modified by Vincent for OAuth2 validation on Nov 03, 2014

try{

	// API access call
	$url = (!empty($_SERVER['HTTPS'])) ? "https://".$_SERVER['SERVER_NAME'] : "http://".$_SERVER['SERVER_NAME'];
	$fields_string ='';
	foreach($_GET as $key=>$value) { $fields_string .= $key.'='.$value.'&'; }
	rtrim($fields_string, '&');
	//$fields_string = http_build_query($_GET); // Common api fix for special characters.
	$util=new GlobalUtil();
	$config=$util->getConfig();
	$peerVerify = $config['peer_verify'] == 0 ? FALSE : TRUE;
	$context = stream_context_create(array(
		'http' => array(
				'method' => 'POST',
				'header' => 'Content-Type: application/x-www-form-urlencoded',
				'content' => $fields_string,
				'timeout' => 1200,
				'ignore_errors' => true
				),
		'ssl' => array('verify_peer'=>$peerVerify,
           'verify_peer_name'=>$peerVerify)
	));

	$response = file_get_contents($url.'/apis/ext/ExpertusOneAPI.php?', false, $context);
  echo $response;
}catch(Exception $e){
	echo $e->getMessage();
}

?>