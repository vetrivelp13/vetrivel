<?php
include_once $_SERVER['DOCUMENT_ROOT'] . "/dataload/Database.php";
include_once $_SERVER['DOCUMENT_ROOT'] . "/sites/all/services/Trace.php";


class ReportBase {
	
	public function __construct(){
		$this->db = new DLDatabase();
	}
	
	protected function update_job_status($report_table, $operation, $status, $sync_on = null) {
		try {
			$column_name = $operation . '_query';
			$column_sync_on = $operation . '_sync_on';
			if($sync_on == null) {
				$sync_on = date("Y-m-d H:i:s");
			}
			$args = array(
					':table_name' => $report_table,
					':job_status' => $status,
					':sync_on' => $sync_on
			);
			$this->db->callQuery("update slt_report_sync_jobs set $column_name = :job_status, $column_sync_on = :sync_on where table_name = :table_name;", $args);
		} catch (Exception $ex) {
			expDebug::dPrint("sync_report.php update_job_status Exception occurred " . $ex->getMessage(), 4);
			throw new Exception("Error in update_job_status ".$ex->getMessage());
		}
	
	}
	
	protected function write_log($report_table, $operation, $status, $sync_on, $start_time, $end_time, $exception = NULL) {
		try {
			$args = array(
					':table_name' => $report_table,
					':sync_on' => $sync_on,
					':start_date' => $start_time,
					':end_date' => $end_time,
					':type' => $operation,
					':status' => $status,
					':exception' => $exception
			);
			$this->db->callQuery('insert into slt_report_sync_log (table_name, sync_on, start_date, end_date, type, status, exception) values (:table_name, :sync_on, :start_date, :end_date, :type, :status, :exception);', $args);
		} catch(Exception $ex) {
			expDebug::dPrint("sync_report.php write_log Exception occurred " . $ex->getMessage(), 4);
			throw new Exception("Error in write_log ".$ex->getMessage());
			return false;
		}
	}
	
	
	protected function queryProcess($query, $report_table, $operation, $sync_on,$start='') {
		try {
			$start = (!empty($start)) ? $start : date("Y-m-d H:i:s");
			$this->update_job_status($report_table, $operation, 'IP', $sync_on);
			// no need to handle transactions in between delete, insert and update calls
			// as the data modified in one call should be retained even though the other calls are failed.
			$this->db->callQuery($query, array(':sync_on' => $sync_on));
			$end = date("Y-m-d H:i:s");
			// update sucees status in slt_report_sync_jobs table with the lastest sync_on
			$this->update_job_status($report_table, $operation, 'CP', $start);
			$this->write_log($report_table, $operation, 'CP', $sync_on, $start, $end);
		} catch (Exception $ex) {
			expDebug::dPrint("queryProcess Exception occurred " . $ex->getMessage(), 4);
			$this->update_job_status($report_table, $operation, 'FL', $sync_on);
			throw new Exception("Error in queryProcess ".$ex->getMessage());
		}
	}
	
}