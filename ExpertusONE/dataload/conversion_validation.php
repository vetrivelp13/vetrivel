<?php 
include_once $_SERVER["DOCUMENT_ROOT"]."/apis/oauth2/Resource.php";

try{
	$server = new ResourceValidate();
	$signed = $server->validate('E1.API');
	if(!$signed->oauth_signed){
		throw new Exception("Invalid token or permission denied.");
	}
	echo 'Token_verified';
}catch(Exception $e){
	throw new Exception($e->getMessage());
}

?>