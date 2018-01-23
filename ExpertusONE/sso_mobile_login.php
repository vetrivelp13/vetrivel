<?php 
define('DRUPAL_ROOT', getcwd());
include_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_VARIABLES);

$saml_module = db_query('select status from system where name = :name and type= :type',array(':name'=>'saml',':type'=>'module'))->fetchCol(0);

if($saml_module[0] == 1){
	$rtnStr = "{'sso':true,'url':'".$base_url."/sso/authenticate/mobile','multimode_auth':'".variable_get('saml_multimode_auth')."'}";
}else{
	$rtnStr = "{'sso':false,'url':'','multimode_auth':''}";
}
print $rtnStr;
?>