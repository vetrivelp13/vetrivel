<?php
include_once('core_engine.php');
include_once('execution_engine.php');
include_once('rewrite_engine.php');

chdir("../../");
define('DRUPAL_ROOT', getcwd());
include_once $_SERVER["DOCUMENT_ROOT"] . '/includes/database/database.inc';
include_once $_SERVER["DOCUMENT_ROOT"] . '/includes/common.inc';
include_once $_SERVER["DOCUMENT_ROOT"] . '/includes/errors.inc';
include_once $_SERVER["DOCUMENT_ROOT"] . '/includes/bootstrap.inc';

include_once $_SERVER["DOCUMENT_ROOT"] . "/sites/all/dao/AbstractDAO.php";
//include_once $_SERVER["DOCUMENT_ROOT"] . "/apis/core/init.php";
include_once $_SERVER["DOCUMENT_ROOT"] . "/apis/core/BaseAPI.php";
include_once $_SERVER["DOCUMENT_ROOT"] . "/sites/all/modules/core/exp_sp_core/exp_sp_core.inc";
drupal_settings_initialize();
//drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
?>