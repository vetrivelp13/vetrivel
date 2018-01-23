<?php 
//include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/GlobalUtil.php";
//include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/Trace.php";
include_once $_SERVER["DOCUMENT_ROOT"]."/apis/oauth2/Resource.php";

try{
	$server = new ResourceValidate();
	$signed = $server->validate('E1.DL');
	if(!$signed->oauth_signed){
		throw new Exception("Invalid token or permission denied.");
	}
	echo true;
}catch(Exception $e){
	throw new Exception($e->getMessage());
}

?>