<?php

class GlobalUtil{

	function __construct(){

	}

	public function getConfig()
	{
		$ph = dirname(__FILE__);
    $ph = str_replace("\\","/",(substr($ph,0,stripos($ph,'sites'))));
		$confDir = !empty($_SERVER["SERVER_NAME"]) && is_dir($_SERVER["DOCUMENT_ROOT"]."/sites/".$_SERVER["SERVER_NAME"])?$_SERVER["SERVER_NAME"]:'default';
		$conf = parse_ini_file($ph . "sites/" . $confDir . "/exp_sp.ini");
    return $conf;
	}
}

?>