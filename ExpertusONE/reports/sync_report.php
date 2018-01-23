<?php 
$key = '';
$doc_root = '';
$sync_frequency = '';
$flush = false;
$table_name = '';

if (in_array($argv, array('--help')) || $argc < 5) {
  ?>
  This is a script to run cron from the command line.
 
  It takes 2 arguments, which must both be specified:
  --key is the job operation in which is to be
  invoked.
  
  --document-root is the path to your drupal root directory for
  your website.
 
  --flush is used to populate delta(partial) or full result to flat table. 
  default: false (optional)
  
  --table-name is used to run a sync up for specfic flat table: 
  default: all (optional)
  
  Usage:
  php sync_report.php --key Job_operation --document-root '/path/to/drupal/root' --flush false --table-name all
 
<?php
} else {
	// Split passed arguments
	for($i = 1; $i < $argc; $i++) {
		switch($argv[$i]) {
			case '--key':
				$i++;
				$key = $argv[$i];
				break;
			case '--document-root':
				$i++;
				$doc_root = rtrim(rtrim($argv[$i], '/'), '\\');
				break;
			case '--flush':
				$i++;
				$flush = $argv[$i];
				break;
			case '--table-name':
				$i++;
				$table_name = trim($argv[$i]);
				break;
		}
	}
	//
	
	$_SERVER['DOCUMENT_ROOT'] = empty($doc_root) ? $_SERVER['DOCUMENT_ROOT'] : $doc_root;
	$_SERVER['HTTP_HOST'] = 'default';
	$_SERVER['REMOTE_ADDR'] = '127.0.0.1';
	$_SERVER['SERVER_SOFTWARE'] = NULL;
	$_SERVER['REQUEST_METHOD'] = 'GET';
	$_SERVER['QUERY_STRING'] = '';
	$_SERVER['HTTP_USER_AGENT'] = 'console';
	
	include_once $_SERVER['DOCUMENT_ROOT'] . "/sites/all/services/Trace.php";
	include_once $_SERVER['DOCUMENT_ROOT'] . "/reports/ReportBase.php";
	include_once $_SERVER['DOCUMENT_ROOT'] . "/dataload/Database.php";
	
	$err_log = array();
	expDebug::dPrint('$cron_key - '.print_r(empty($err_log),true),5);
	$db = new DLDatabase();
	$db->callQuery('select value from variable where name = \'cron_key\';');
	$cron_key = unserialize($db->fetchColumn());
	expDebug::dPrint('$cron_key - '.$cron_key);
	if(!isset($key) || $cron_key != $key) {
		die("Cron could not run because an invalid key was used.");
	}
	// check if any process is in progress already
	// new table slt_report_sync_jobs created
	$count_qry = "select count(1) from slt_report_sync_jobs where sync_status = 'IP';";
	$db->callQuery($count_qry);
	$recordcnt = $db->fetchColumn();
	expDebug::dPrint("sync_report.php no of jobs already in progress... " . $recordcnt, 4);
	if($recordcnt > 0) {
		die("One job is already in progress...");
	} else {
		// pick next job and start running
		$tableQuery = (!empty($table_name)) ? " and table_name = :table_name" : "";
		
		$qry = "select table_name, sync_on, delete_sync_on, insert_sync_on, update_sync_on from slt_report_sync_jobs where active = 'Y' ".  $tableQuery . ";";
		if (!empty($table_name)) 
			$db->callQuery($qry, array(':table_name' => $table_name));
		else 
			$db->callQuery($qry);
		
		$results = $db->fetchAllResults();
		expDebug::dPrint('table results : ' . print_r($results, 1), 4);
		
			foreach ( $results as $result) {
				try {
					if ($flush == 'true'){
						$result->sync_on = null;
						$result->delete_sync_on = null;
						$result->insert_sync_on = null;
						$result->update_sync_on = null;
						/*$flushTable  = "TRUNCATE TABLE " . $result->table_name ."; ";
						$db->callQuery($flushTable);*/
					} 
					$result->flush = $flush;
					$report_table = $result->table_name;
					$last_sync_on = $result->sync_on;
					expDebug::dPrint("sync_report.php table name to process " . print_r($result, 1), 4);
					expDebug::dPrint('$report_table - '.$report_table, 4);
					expDebug::dPrintDBAPI('select table to process ', $qry, array(':sync_frequency' => $sync_frequency));
					$report_file = $doc_root . '/reports/' . $report_table . '.php';
					if(!file_exists($report_file)) {
						expDebug::dPrint( $report_table . " file doesn't exist, Continue to next file.", 4);
						continue;
					}
					require_once $report_file;
					// update now() in sync_on column
					$upd_sync = $db->callQuery('update slt_report_sync_jobs set sync_on = now() where table_name = :table_name', array(':table_name' => $report_table));
					expDebug::dPrint('update sync_on - '.print_r($upd_sync, 1), 4);
				
					if (!class_exists($report_table)){
						expDebug::dPrint( $report_table . "class name doesn't exist ", 4);
					}
					
					$clsObj = new $report_table($result);
					$invoke_method = 'reportExecute';

					if(method_exists($clsObj, $invoke_method)) {
						expDebug::dPrint("method avaiable " . print_r($invoke_method, 1), 4);
						$status_updated = $db->callQuery('update slt_report_sync_jobs set sync_status = \'IP\' where table_name = :table_name', array(':table_name' => $report_table));
						$query = call_user_func_array(array($clsObj, $invoke_method), array($result));
						$upd_status = $db->callQuery('update slt_report_sync_jobs set sync_status = \'RD\' where table_name = :table_name', array(':table_name' => $report_table));
					}
				} catch (Exception $ex) {
					expDebug::dPrint('Exception in report execution'.$ex->getMessage());
					$err_log [] = $report_table.'-'.$ex->getMessage();
					if(isset($status_updated)) {
						$upd_status = $db->callQuery('update slt_report_sync_jobs set sync_status = \'RD\' where table_name = :table_name', array(':table_name' => $report_table));
					}
				}	
			}
			expDebug::dPrint("ErrorLOGSSS Latha test 123 ::: " . print_r($err_log, 1), 4);
			if(!empty($err_log))
				sendnotification($err_log);
	}
}

function invoke_report_sync_call(&$db, $report_table, $operation, $result) {
	try {
		
		if (!class_exists($report_table)){
			expDebug::dPrint( $report_table . "class name doesn't exist " . $ex->getMessage(), 4);
			return false;
		}
		
		$clsObj = new $report_table($result);
		$invoke_method = 'report' . $operation;
		
		expDebug::dPrint("sync_report method name " . print_r($invoke_method, 1), 4);
		
		if(method_exists($clsObj, $invoke_method)) {
			expDebug::dPrint("method avaiable " . print_r($invoke_method, 1), 4);
			
			//$query = call_user_func_array($invoke_call, array($sync_on));
			$query = call_user_func_array(array($clsObj, $invoke_method), array($result));
			
			expDebug::dPrint($operation . ' script to be run ' . $query, 4);
// 			if(!empty($query)) {
// 				$start = date("Y-m-d H:i:s");
// 				update_job_status($db, $report_table, $operation, 'IP', $sync_on);
// 				// no need to handle transactions in between delete, insert and update calls
// 				// as the data modified in one call should be retained even though the other calls are failed.
// 				$db->callQuery($query, array(':sync_on' => $sync_on));
// 				$end = date("Y-m-d H:i:s");
// 				// update sucees status in slt_report_sync_jobs table with the lastest sync_on
// 				update_job_status($db, $report_table, $operation, 'CP', $start);
// 				write_log($db, $report_table, $operation, 'CP', $sync_on, $start, $end);
// 			}
		}
		return true;
	} catch (Exception $ex) {
		// expDebug::dPrint('exception type '.var_export(get_class($ex), 1), 1);
		expDebug::dPrint("sync_report.php invoke_report_sync_call Exception occurred " . $ex->getMessage(), 4);
		if(!isset($end)) {
			$end = date("Y-m-d H:i:s");
		}
		// update failure status in slt_report_sync_jobs table with the older sync_on value
		//update_job_status($db, $report_table, $operation, 'FL', $sync_on);
		//write_log($db, $report_table, $operation, 'FL', $sync_on, $start, $end, $ex->getMessage());
		throw new Exception("Error in invoke_report_sync_call ".$ex->getMessage());
		return false;
	}
}
function sendnotification($table){
	
	include_once $_SERVER['DOCUMENT_ROOT'] . "/sites/all/commonlib/phpmailer/PHPMailerAutoload.php";
	include_once $_SERVER['DOCUMENT_ROOT'] . "/sites/all/modules/core/exp_sp_core/exp_sp_core.module";
	expDebug::dPrint('System is sending notification mail'.print_r($table,true),5);
	
	$mail_to = getConfigValue('mail_report_fail');
	$site = getConfigValue('site_name');
	$site_name = !empty($site) ? $site : 'ExpertusONE';
	$base_url = getConfigValue('admin_site_url');
	expDebug::dPrint('Send mail to the following recepients'.$site_name);
	
	if (!empty($mail_to)) {
		$Body = "<html><br>Hi,</br><br> The report sync has failed for the following tables </br> <br> <table  width='780px' cellspacing='0' cellpadding='5'><tbody style='font-size:10pt;font-family:arial,helvetica,sans-serif'><tr style='font-weight: bold;'><td width='30%'>TableName</td><td width='70%'>Reason</td></tr>";
		foreach ($table as $field=>$val) {
			$det = explode('-',$val);
			expDebug::dPrint('System is sending notification mail'.print_r($det,true),5);
			$Body .= "<tr><td>";
			$Body .= $det[0];
			$Body .= "</td><td>";
			$Body .= $det[1];
			$Body .= "</td></tr>";
		}
		$Body .= "</tbody></table></br>";
		$Body .= '<br> Thank You, </br><br>'.$site_name.'&nbsp;Team<br>';
		$Body .= '<a href="'.$base_url.'">'.$site_name.'</a></br></br></html>';
		
		$phpMailer = new PHPMailer();
		$phpMailer->CharSet = 'UTF-8';
		$phpMailer->setFrom('info@expertusone.com','ExpertusONE');
		//$phpMailer->addReplyTo('lathas@peopleone.co');
		$phpMailer->WordWrap = 50;
		$phpMailer->Subject  = "Report sync has failed";
		$phpMailer->Body = $Body;
		$phpMailer->IsHTML(true);
		foreach (explode(',', $mail_to) as $address) {
			$address = trim($address);
			if (!empty($address)) {
				$phpMailer->AddAddress($address);
			}
			else {
				expDebug::dPrint('mail_report_fail is not configured.');
				//throw new Exception ('Bad schedule email address list');
			}
		}
		if(!$phpMailer->Send()) { 
			expDebug::dPrint('System could not email the notification of report sync up mail to recipient. ' . $phpMailer->ErrorInfo, 4);
		}
		else {
			expDebug::dPrint('System has sent mail successfully ');
		} 
	}	
	
}

?>