<?php 

if (in_array($argv, array('--help')) || $argc < 5) {
?>
  This is a script to run cron from the command line.
 
  It takes 2 mandatory arguments, which must both be specified:
  --key is the cron key for your website, found on your
  status report page, the part after ?cron_key=?
  --document-root is the path to your drupal root directory for
  your website.
  --flush is an optional argument, which can be used to delete records from autoregister temp table
  	Please use this option only if required and when the site is not accessed from anywhere (during deployment) and all the crons stopped as this has impact
    use --flush = 'invalid' to delete invalid temp records
    use --flush = 'completed' to delete completed records which remain in temp table due to notification disabled
    use --flush = 'all' to delete all records
  Usage:
  php autoregister_cron.php --key YOUR_CRON_KEY --document-root '/path/to/drupal/root'
Note: --flush argument should not be set with the auto register cron setup and can only be used from the cli if required to delete temp table records.
<?php
} else {
  // Loop through arguments and extract the cron key and drupal root path.
  $flush = false; 
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
      case '--flush':
      	$i++;
      	$flush = $argv[$i];
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
elseif($flush !== false) {
	try{
		if(strtolower($flush) == 'all') {
// 			$query = 'TRUNCATE TABLE slt_autoregister_temp;';
			$result = db_truncate('slt_autoregister_temp')->execute();
			updateAutoRegMonitoringJobs();
			expDebug::dPrint('slt_autoregister_temp table flush '. $flush, 1);
		} elseif(strtolower($flush) == 'completed')  {
// 			$query = 'DELETE FROM TABLE slt_autoregister_temp WHERE record_status = \'CP\';';
			$deleteStmt = db_delete('slt_autoregister_temp');
			$deleteStmt->condition('record_status', 'CP', '=');
			expDebug::dPrintDBAPI('slt_autoregister_temp table flush '. $flush.' delete query for temp table', $deleteStmt);
			$deleteStmt->execute();
		}else {
// 			$query = 'DELETE FROM TABLE slt_autoregister_temp WHERE record_status = \'IN\';';
			$deleteStmt = db_delete('slt_autoregister_temp');
			$deleteStmt->condition('record_status', 'IN', '=');
			expDebug::dPrintDBAPI('slt_autoregister_temp table flush '. $flush.' delete query for temp table', $deleteStmt);
			$deleteStmt->execute();
		}
	}catch (Exception $e) {
		expDebug::dPrint("ERROR in deleteRecordsFromTemp ".print_r($e,true),1);
		throw new Exception($e->getMessage());
	}
}
else {
  include_once DRUPAL_ROOT . "/sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc";
  include_once DRUPAL_ROOT . "/sites/all/modules/core/exp_sp_learning/exp_sp_learning_auto_register.inc";
	try{
			ini_set('memory_limit', '256M');
			
			$st = microtime(true);
			$jobname = 'auto_register';
			$checkStatus     = check_monitor_jobs_status($jobname);
			$last_run        = $checkStatus["last_run"];
			$next_run        = date("Y-m-d H:i:s", strtotime($last_run));
			$curr_time			 = date("Y-m-d H:i:s",time());
			expDebug::dPrint("Auto Register check".$last_run,3);
			$last_run_date = date("Y-m-d",strtotime($last_run));
			$curr_date = date("Y-m-d",time());
			// Validate the auto register jobs needs to be happen once 
			// the previous jobs if any has to be complete 
			
			$obj = new auto_register_enrollment();
			
			if($last_run_date < $curr_date){
			    // Populate data in temp file for further porcess
			    updateAutoRegMonitoringJobs();
			    $obj->populateAutoRegstrationRecords();
			    
			}elseif($next_run <= $curr_time && $checkStatus["custom0"] != 1) {
				$update = db_update('slt_monitoring_jobs');
				$update->fields(array(
					'last_run_date'=>now(),
				    'custom0'=>1
				));
				$update->condition('job_name',$jobname,'=');
				$update->execute();
				
				// Call register process to do enrollments
				$obj->autoRegisterEnrollment();
				
				updateAutoRegMonitoringJobs();
			}
			$ent = microtime(true);
			expDebug::dPrint("Total Execution time -- ( $ent - $st = ".($ent-$st)." -- Time in minutes = ".(($ent-$st)/60).")",4);

		}catch(Exception $ex){
		    updateAutoRegMonitoringJobs();
			expDebug::dPrint("Error in CRON - Auto registration -- ".print_r($ex,true),1);
		}
	}
}

function updateAutoRegMonitoringJobs(){
    try{

        $update = db_update('slt_monitoring_jobs');
        $update->fields(array(
                'last_run_date'=>now(),
                'custom0'=>0
        ));
        $update->condition('job_name','auto_register','=');
        $update->execute();

    }catch(Exception $ex){
        expDebug::dPrint("Error in CRON - Auto registration updateMonitoringJobs -- ".print_r($ex,true),1);
    }

}


?>