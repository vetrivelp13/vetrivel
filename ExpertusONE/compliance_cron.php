<?php

if (in_array($argv, array('--help')) || $argc != 5) {
?>
  This is a script to run cron from the command line.
 
  It takes 2 arguments, which must both be specified:
  --key is the cron key for your website, found on your
  status report page, the part after ?cron_key=?
  --document-root is the path to your drupal root directory for
  your website.
 
  Usage:
  php compliance_cron.php --key YOUR_CRON_KEY --document-root '/path/to/drupal/root'
 
<?php
} else {
  // Loop through arguments and extract the cron key and drupal root path.
  for ($i = 1; $i < $argc; $i++) {
    switch ($argv[$i]) {
      case '--key':
        $i++;
        $key = $argv[$i];
        break;
      case '--document-root':
        $i++;
        $path = $argv[$i];
        break;
    }
  }
 
  chdir($path);
  // Sets script name
  $_SERVER['SCRIPT_NAME'] = 'clicron.php';
 
  // Values as copied from drupal.sh
  $_SERVER['HTTP_HOST'] = 'default';
  $_SERVER['REMOTE_ADDR'] = '127.0.0.1';
  $_SERVER['SERVER_SOFTWARE'] = NULL;
  $_SERVER['REQUEST_METHOD'] = 'GET';
  $_SERVER['QUERY_STRING'] = '';
  $_SERVER['HTTP_USER_AGENT'] = 'console';
  $_SERVER['DOCUMENT_ROOT'] = $path;
 
  // Set cron key get variable to use below code with as
  // little modification as possible.
  $_GET['cron_key'] = $key;
  define('DRUPAL_ROOT', $path);
  
	include_once DRUPAL_ROOT . '/includes/bootstrap.inc';
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

	include_once DRUPAL_ROOT . "/sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc";

	$st = microtime(true);
	try{
		autoComplianceIncompleteUpdate();
	}catch(Exception $e){
		expDebug::dPrint("Error in Auto compliance incomplete update ".print_r($e,true),1);
	}

	try{
		autoComplianceMandatoryCompletionDaysReminder('is_compliance');
	}catch(Exception $ex){
		expDebug::dPrint("Error in CRON - Remainder notification for Compliance completion -- ".print_r($ex,true),1);
	}

	// Remainder notification for Mandatory completion (Class)
	try{
		autoComplianceMandatoryCompletionDaysReminder('mandatory');
	}catch(Exception $ex){
		expDebug::dPrint("Error in CRON - Remainder notification for Mandatory completion -- ".print_r($ex,true),1);
	}

	// Remainder notification for Compliance Expired
	try{
		autoComplianceExpiredReminder();
	}catch(Exception $ex){
		expDebug::dPrint("Error in CRON - Remainder notification for Compliance expiry -- ".print_r($ex,true),1);
	}

	// Remainder notification for Compliance Expiry
	try{
		autoComplianceExpiryDaysReminder();
	}catch(Exception $ex){
		expDebug::dPrint("Error in CRON - Remainder notification for Compliance expiry -- ".print_r($ex,true),1);
	}

	try{
		expDebug::dPrint("complianceReregistration inside ",4);
		complianceReregistration();
		expDebug::dPrint("outside complianceReregistration ",4);
	}catch(Exception $ex){
		expDebug::dPrint("Error in CRON - Remainder notification for Compliance expiry -- ".print_r($ex,true),1);
	}
	$ent = microtime(true);
	expDebug::dPrint("Total Execution time -- ( $ent - $st = ".($ent-$st)." -- Time in minutes = ".(($ent-$st)/60).")",1);
}

?>