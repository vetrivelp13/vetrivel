<?php 
include $_SERVER["DOCUMENT_ROOT"]."/apis/oauth2/Resource.php";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/Trace.php";
include_once $_SERVER['DOCUMENT_ROOT']."/sites/default/settings.php";

$url = $_SERVER['SCRIPT_URI'].'?'.$_SERVER['REQUEST_URI'];

if(strpos($_SERVER['REQUEST_URI'],"?")>1){
	$url = $_SERVER['SCRIPT_URI'].'?'.substr($_SERVER['REQUEST_URI'],strpos($_SERVER['REQUEST_URI'],"?")+1);
}

if(substr_count($_SERVER['REQUEST_URI'],"sites/default/files") > 0){
	$url = $base_url.$_SERVER['REQUEST_URI'];
	
	if(substr_count($_SERVER['REQUEST_URI'],"?")>1){
		$url = $base_url.substr($_SERVER['REQUEST_URI'],strpos($_SERVER['REQUEST_URI'],"?")+1);
	}
}


if(stripos($url,'wysiwyg')!==false) 
	exit;
	
$access_token = isset($_GET['access_token'])?$_GET['access_token']: 
				(isset($_POST['access_token'])? $_POST['access_token'] : '');

if(empty($access_token) && isset($_SERVER['HTTP_REFERER']) && !empty($_SERVER['HTTP_REFERER'])){
	$reqUrl = parse_url($_SERVER['HTTP_REFERER']);
	$reqTok = (!empty($reqUrl['query'])) ? explode('&',$reqUrl['query']) : '';
	if(!empty($reqTok)){
		foreach($reqTok as $val){
			if(stripos($val,'access_token')!==false){
				$tmp = explode('=',$val);
				$access_token = $tmp[1];
				$_GET['access_token'] = $access_token;
				break;
			}
		}
	}
}

if(!empty($access_token)){
	try{
		$server = new ResourceValidate();
		$signedObj = $server->validate('E1.API');
		if($signedObj->oauth_signed){
			setcookie("SPCertificate", $access_token, null, "/", "",
                         (isset($_SERVER["HTTPS"]) && $_SERVER["HTTPS"]=='on'?TRUE:""),
                         (isset($_SERVER["HTTPS"]) && $_SERVER["HTTPS"]=='on'?TRUE:""));
			if(strpos($url,'http://')== 0 ){
				$url = preg_replace('/'.preg_quote('http://', '/').'/', 'https://', $url, 1);
			} 
	  	header ("Location: $url");
		}else{
			echo "Invalid token or scope. You are not authorized to view this content.";
		}
	}catch(Exception $e){
		expDebug::dPrint("Error in token validation -- ".$e->getMessage(),1);
		echo "Invalid token or scope. You are not authorized to view this content.";
	}
}else{
	print "You are not authorized to view this content.";
}
?>