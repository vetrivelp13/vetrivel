<?php 
include_once $_SERVER["DOCUMENT_ROOT"] . '/includes/bootstrap.inc';
require_once "../services/GlobalUtil.php";
require_once "../../../getLearnerInfo.php";
define('DRUPAL_ROOT', $_SERVER['DOCUMENT_ROOT']);
drupal_bootstrap(DRUPAL_BOOTSTRAP_SESSION);
$userInfo = new GetLearnerInfo();
echo $userInfo->getValue('userid');
?>