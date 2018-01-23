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
  php clicron.php --key YOUR_CRON_KEY --document-root '/path/to/drupal/root'
 
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
  $_SERVER['SCRIPT_NAME'] = $argv[0];
 
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
  
/**
 * Root directory of Drupal installation.
 */

include_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

if (!isset($_GET['cron_key']) || variable_get('cron_key', 'drupal') != $_GET['cron_key']) {
  watchdog('cron', 'Cron could not run because an invalid key was used.', array(), WATCHDOG_NOTICE);
  drupal_access_denied();
}
elseif (variable_get('maintenance_mode', 0)) {
  watchdog('cron', 'Cron could not run because the site is in maintenance mode.', array(), WATCHDOG_NOTICE);
  drupal_access_denied();
}
else {
  include_once DRUPAL_ROOT . "/sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc";
	try{
			ini_set('memory_limit', '256M');
			$st = microtime(true);
			expDebug::dPrint("Currency Conversion Check - ".getConfigValue('cc_update_frequency'));
			$cron_run_cnt_mc = checkCustomCronJob('multicurrency_conversion_update',getConfigValue('cc_update_frequency'));
			expDebug::dPrint('cron count for multi currency'.$cron_run_cnt_mc,3);
			if($cron_run_cnt_mc>0){
				expDebug::dPrint("Currency Conversion Cron Run - ".getConfigValue('cc_update_frequency'));

				$update = db_update('slt_monitoring_jobs');
				$update->fields(array(
					'last_run_date'=>now(),
				  'custom0'=>1
				));
				$update->condition('job_name','multicurrency_conversion_update','=');
				$update->execute();
				
				// Call multicurrency_conversion
				multicurrencyConversionUpdate();
				
				$update = db_update('slt_monitoring_jobs');
				$update->fields(array(
				  'custom0'=>0
				));
				$update->condition('job_name','multicurrency_conversion_update','=');
				$update->execute();
			}
			$ent = microtime(true);
			expDebug::dPrint("Total Execution time -- ( $ent - $st = ".($ent-$st).")",4);
		}catch(Exception $ex){
			expDebug::dPrint("Error in CRON - multicurrency_conversion -- ".print_r($ex,true),1);
		}
	}
}

?>