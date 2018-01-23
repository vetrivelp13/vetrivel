<?php
// Checks that the arguments are supplied, if not display help information.
if (in_array($argv, array('--help')) || $argc != 7) {
  ?>
  This is a script to run cron from the command line.
 
  It takes 6 arguments, which must both be specified:
  --key is the cron key for your website, found on your
  status report page, the part after ?cron_key=?
  --document-root is the path to your drupal root directory for
  your website.
  --cron-type is the type of cron to process bulk or regular notifications
 
  Usage:
  php notification_cron.php --key YOUR_CRON_KEY --document-root '/path/to/drupal/root' --cron-type bulk
 
<?php
} else {
	$crnType='';
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
      case '--cron-type':
       	$i++;
       	$crnType = $argv[$i];
       	break;
    }
  }
 
  chdir($path);
   
  $crnType = !empty($crnType) ? $crnType : 'regular';
    
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
  $_GET['cron_type'] = $crnType;
  define('DRUPAL_ROOT', $path);
 
  // Code below is almost verbatim from cron.php, just the messages for
  // watchdog have been changed to indicate that the problem originated from
  // this script.
  include_once DRUPAL_ROOT . '/includes/bootstrap.inc';
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  
  require_once DRUPAL_ROOT . '/sites/all/services/GlobalUtil.php';
  
  if (!isset($_GET['cron_key']) || variable_get('cron_key', 'drupal') != $_GET['cron_key']) {
    watchdog('cron', "Cron could not run via $argv[0] because an invalid key was used.", array(), WATCHDOG_NOTICE);
    drupal_access_denied();
  } else if ($crnType != 'bulk' && $crnType != 'regular') {
    watchdog('cron', "Cron could not run because an invalid cron type was used.", array(), WATCHDOG_NOTICE);
    drupal_access_denied();
  } elseif (variable_get('maintenance_mode', 0)) {
    watchdog('cron', "Cron could not run via $argv[0] because the site is in maintenance mode.", array(), WATCHDOG_NOTICE);
    drupal_access_denied();
  } else {
  	ini_set('memory_limit', '256M');
		$st = microtime(true);
		//$jobname = 'notification_cron';
		//$checkStatus     = check_monitor_jobs_status($jobname);
		$qry = "SELECT job_name AS jobs, last_run_date AS last_run, custom0 AS custom0 
						FROM slt_monitoring_jobs 
						WHERE job_name IN ( :job1 , :job2  , :job3) 
						ORDER BY job_name ";
		$jobs = array(':job1'=>'auto_register',':job2'=>'notification_cron',':job3'=>'enrollment_update');
		expDebug::dPrintDBAPI("Query to get the cron job details ",$qry,$jobs);
		$checkStatus = db_query($qry,$jobs)->fetchAll();
		expDebug::dPrint("Notification cron monitoring table status -- ".print_r($checkStatus,true),3);
		$last_run        = explode(" ",$checkStatus[2]->last_run);
		$last_run_date   = $last_run[0];
		$curr_date       = date("Y-m-d");
		expDebug::dPrint("Auto Register check".print_r($last_run, 1),3);
		
		if($crnType == 'regular'){
			// To make sure the "notification_cron" is yet to run for the day,
			// It is not in-progress and cron "auto_register" also not in progress.
			// This is to avoid dead lock or database hanging, because all the details are 
			// update in the same notification tabls.
			if($last_run_date != $curr_date && $checkStatus[1]->custom0 != 1 && $checkStatus[0]->custom0 != 1 && $checkStatus[2]->custom0 != 1) {
				//Update custom0 to not process this job again till the previous one get complete
				$update = "UPDATE slt_monitoring_jobs 
									 SET last_run_date = IF(job_name IN ('notification_cron','enrollment_update'),now(),last_run_date) , custom0 = 1 
									 WHERE job_name IN ( :job1 , :job2 , :job3 )";
				
				expDebug::dPrintDBAPI("Update monitoring table ",$update,$jobs);
				db_query($update,$jobs);
				try{
					// Call bulk process for enrollment update
			  		include_once DRUPAL_ROOT . "/sites/all/modules/core/exp_sp_core/exp_sp_core.inc";
			  		include_once DRUPAL_ROOT . "/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_people/exp_sp_administration_user/exp_sp_administration_user.inc";
			  		updateEnrollmentsGrpUserTable();
				}catch(Exception $ex){
					expDebug::dPrint("Error in runing bulk process for enrollment update " . print_r($ex, 1), 1);
				}
				try{
					// Call CRON Bulk Process
		  		include_once DRUPAL_ROOT . "/sites/all/modules/core/exp_sp_learning/exp_sp_learning_bulk_notification.inc";
		  		$bulkProcess = new BackEndBulkProcess();
		  		$bulkProcess->bulkProcess();
				}catch(Exception $ex){
					expDebug::dPrint("Error in runing bulk cron process ".print_r($ex,true),1);
				}
	  		// update monitor jobs was done
				$update = db_update('slt_monitoring_jobs');
				$update->fields(array(
				  'custom0'=>0
				));
				$update->condition('job_name',array('auto_register','notification_cron','enrollment_update'),'IN');
				$update->execute();
	  	}else{ 
	  		try{
		  		include_once DRUPAL_ROOT . "/sites/all/modules/core/exp_sp_core/modules/exp_sp_notify/exp_sp_notify.inc";
		  		$notifyObj = new notificationProcess();
		  		$notifyObj->notifyProcess($crnType);
	  		}catch(Exception $ex){
	  			expDebug::dPrint("Error in sending regular mails from slt_notification table ".print_r($ex,true),1);
	  		}
	  	}
	  	// Content transfer job
	  	include_once DRUPAL_ROOT.'sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc';
	  	$jobname = 'content_transfer_attendance_summary';
	  	$checkStatus     = check_monitor_jobs_status($jobname);
	  	expDebug::dPrint("job status for content_transfer_attendance_summary_cron".print_r($checkStatus,true),4);
	  	if($checkStatus['custom0']==1){
	  		include_once(drupal_get_path('module', 'exp_sp_administration_content') .'/exp_sp_administration_content.inc');
	  		batchCTProcess();
	  		expDebug::dPrint("after invoking batchCTProcess for content_transfer_attendance_summary_cron",4);
	  	}
	  	// Content transfer job ends
	  	
	  	
	  /* 	//Group bulk user upload
	  	//include_once DRUPAL_ROOT.'sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc';
	  	$jobname = 'group_bulk_user_upload';
	  	$checkStatus     = check_monitor_jobs_status($jobname);
	  	expDebug::dPrint("job status for group_bulk_user_upload cron".print_r($checkStatus,true),4);
	  	if($checkStatus['custom0']==1){
	  		include_once(drupal_get_path('module', 'exp_sp_administration_groups') .'/exp_sp_administration_groups.inc');
	  		batchGUProcess();
	  		expDebug::dPrint("after invoking batchGUProcess for group_bulk_user_upload_cron",4);
	  	} */
	  	
	  	
		}else{
			if($checkStatus[2]->custom0 != 1) {
				try{
				include_once DRUPAL_ROOT . "/sites/all/modules/core/exp_sp_core/modules/exp_sp_notify/exp_sp_notify.inc";
	  		$notifyObj = new notificationProcess();
	  		$notifyObj->notifyProcess($crnType);
				}catch(Exception $ex){
	  			expDebug::dPrint("Error in sending bulk mails from slt_bulk_notification table ".print_r($ex,true),1);
	  		}
			}
		}
		$ent = microtime(true);
		expDebug::dPrint("Total Execution time -- ( $ent - $st = ".($ent-$st)." -- Time in minutes = ".(($ent-$st)/60).")",4);
		
				
  }
}