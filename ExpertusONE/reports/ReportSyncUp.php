<?php
include_once $_SERVER['DOCUMENT_ROOT'] . "/dataload/Database.php";
include_once $_SERVER['DOCUMENT_ROOT'] . "/reports/ReportBase.php";
include_once $_SERVER['DOCUMENT_ROOT'] . "/sites/all/services/Trace.php";


abstract class ReportSyncUp extends ReportBase{
	
	protected $db = null;
	
	protected $delSync_on = '';
	protected $insSync_on = '';
	protected $updSync_on = '';
	
	public function __construct($result){
		parent::__construct();
		$this->delSync_on = $result->delete_sync_on;
		$this->insSync_on = $result->insert_sync_on;
		$this->updSync_on = $result->update_sync_on;
		$this->flush 	  = $result->flush;
		
	}
	
	/**
	 * Used to execute all CRUD operations
	 * @param datetime $sync_on
	 */
	abstract public function reportExecute();
	
	/**
	 * Used to Delete all records
	 * @param datetime $sync_on
	 */
	abstract protected function reportDelete();
	
	/**
	 * Used to create a temp table to process delta records
	 * @param datetime $sync_on
	 */
	abstract protected function createTempTable();
	
	/**
	 * Used to Delete all records
	 * @param datetime $sync_on
	 */
	abstract protected function tempTableDataPopulate();
	
	/**
	 * Used to Insert all records
	 * @param datetime $sync_on
	 */
	abstract protected function reportInsert();
	
	/**
	 * Used to Update all records
	 * @param datetime $sync_on
	 */
	abstract protected function reportUpdate();
	
	/**
	 * Used to Collect all report ids affected
	 * @param string $action`
	 */
	abstract protected function reportCollectIds($action);
	
	
}