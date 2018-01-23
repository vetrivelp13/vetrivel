<?php

/*
include_once $_SERVER['DOCUMENT_ROOT']."/getLearnerInfo.php";
$obj = new GetLearnerInfo();
$userId = $obj->getValue('userid');

print $userId;
if(!$userId){
  print "You are not Authorized to access this page.";
  exit();         
}*/
/*$pathInfoCount = substr_count($_SERVER['PHP_SELF'], '/');
$directory = str_repeat('../', $pathInfoCount - 1);
if(!empty($directory)){
  chdir($directory);
}
*/

// To change the directory
//chdir($_SERVER['DOCUMENT_ROOT']);

define('DRUPAL_ROOT', getcwd());
require_once 'includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_SESSION);

global $user;
if(!$user->uid){
  print "You are not Authorized to access this page.";
  exit();         
}


