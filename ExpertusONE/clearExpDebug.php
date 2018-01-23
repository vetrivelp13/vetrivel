<?php
define('DRUPAL_ROOT', getcwd());
include_once "./includes/common.inc";
include_once "./includes/database/database.inc";
include_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

global $user;
if($user->uid==1)
{
  file_put_contents("exp_debug.txt", "");
  file_put_contents("exp_sp_debug.txt", "");
  print "Debug file cleared<br>";
}
else
{
  print "You are not Authorized to access this Service.";         
}
 // CC test
?>