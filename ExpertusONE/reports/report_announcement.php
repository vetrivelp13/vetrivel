<?php
include_once $_SERVER['DOCUMENT_ROOT'] .  '/reports/ReportSyncUp.php';

class report_announcement extends ReportSyncUp {
	
	
	private $report_table = '';
	
	private $temp_table_name = '';
	
	/**
	 * Class Constructor
	 * @param object $result
	 */
	public function __construct($result){
		parent::__construct($result);
		$this->report_table = __CLASS__;
		$this->temp_table_name = 'temp_' . $this->report_table;
	}

	/**
	 * Method used to Execute Data Syncup Process
	 */
	public function reportExecute(){
		try {
			$start = date("Y-m-d H:i:s");
			// Remove deleted records in flat table
			$this->reportDelete();
			
			// Create a temp table to process insert/update records
			$this->createTempTable();
			
			// Data popuation to temp table
			$this->tempTableDataPopulate();
			
			// Update records in flat table
			$this->reportUpdate();
			
			// Insert a new records to flat table
			$this->reportInsert();
			
			$this->dropTempTable(); // drop temp table
			$end = date("Y-m-d H:i:s");
			
		} catch(Exception $ex) {
			if(!isset($end)) {
				$end = date("Y-m-d H:i:s");
			}
			$this->dropTempTable(); // drop temp table
			$this->write_log($this->report_table, 'temp', 'FL', $this->updSync_on, $start, $end, $ex->getMessage());
			throw new Exception("Error in reportExecute ".$ex->getMessage());
		}
	}
	
	/**
	 * Method used to remove deleted records
	 */
	protected function reportDelete() {
		try {
			$delSubQuery = $this->reportCollectIds('delete');
			$delQuery = "DELETE rprm FROM ". $this->report_table ." as rprm JOIN (".$delSubQuery.") as deleted ON rprm.RUID = deleted.RUID";
			expDebug::dPrint("reportDelete Query" . print_r($delQuery, 1), 4);
			$this->queryProcess($delQuery, $this->report_table, 'delete', $this->delSync_on);
		} catch(Exception $ex) {
			throw new Exception("Error in reportDelete ".$ex->getMessage());
		}
	}
	
	/**
	 * Method used to create temp table
	 */
	protected function createTempTable() {
		try {
			//clear-memcache.php
			$dropTable  = "DROP TABLE IF EXISTS " . $this->temp_table_name ."; ";
			$this->db->callQuery($dropTable);
			
			//$createTable = "CREATE TABLE ".$this->temp_table_name." LIKE ".$this->report_table."";
			$createTable = "CREATE TABLE " . $this->temp_table_name ." (
			  `RUID` varchar(255),
			  `AnnID` int(11) NOT NULL DEFAULT '0',
			  `AnnUserID` int(11) NOT NULL,
			  `AnnDescription` longtext,
			  `AnnLangCode` longtext,
			  `AnnStatus` longtext,
			  `AnnFromDate` date DEFAULT NULL,
			  `AnnToDate` date DEFAULT NULL,
			  `AnnFromTime` varchar(5) DEFAULT NULL,
			  `AnnToTime` varchar(5) DEFAULT NULL,
			  `AnnPriority` varchar(4) NOT NULL DEFAULT '',
			  `CreatedBy` int(11) DEFAULT NULL,
			  `CreatedOn` datetime DEFAULT NULL,
			  `UpdatedBy` int(11) DEFAULT NULL,
			  `UpdatedOn` datetime DEFAULT NULL,
			  `operation` varchar(10) NULL DEFAULT NULL,
			  PRIMARY KEY (`RUID`),
			  key sli_op(operation)
			) ENGINE=InnoDB DEFAULT CHARSET=utf8;";
			$this->db->callQuery($createTable);
			
			//$alterTable = "ALTER TABLE ".$this->temp_table_name." ADD COLUMN `operation` varchar(10) NULL DEFAULT NULL;";
			//$this->db->callQuery($alterTable);
			
		} catch(Exception $ex) {
			throw new Exception("Error in createTempTable ".$ex->getMessage());
		}
	}
	
	/**
	 * Method used to drop temp table
	 */
	protected function dropTempTable() {
		try {
			$dropTable  = "DROP TABLE IF EXISTS " . $this->temp_table_name ."; ";
			$this->db->callQuery($dropTable);
		} catch(Exception $ex) {
			throw new Exception("Error in dropTempTable ".$ex->getMessage());
		}
	}
	
	/**
	 * Method used to populte delta data to temp table
	 */
	protected function tempTableDataPopulate() {
		try {
			$tempDataQuery = "insert into ".$this->temp_table_name."
					SELECT
    					`ssn`.`id` as `RUID`,
    					`ssn`.`id` AS `AnnID`,
    					`ssn`.`user_id` AS `AnnUserID`,
    					`ssn`.`description` AS `AnnDescription`,
    					`lang`.`name` AS `AnnLangCode`,
    					`sta`.`name` AS `AnnStatus`,
    					`ssn`.`from_date` AS `AnnFromDate`,
    					`ssn`.`to_date` AS `AnnToDate`,
    					IF(`ssn`.`from_time` = 'hh:mm','NULL',`ssn`.`from_time`) AS `AnnFromTime`,
    					IF(`ssn`.`to_time` = 'hh:mm','NULL',`ssn`.`to_time`) AS `AnnToTime`,
    					IF(`ssn`.`priority`=1,'Low','High') AS `AnnPriority`,
    					`ssn`.`created_by` AS `CreatedBy`,
    					`ssn`.`created_on` AS `CreatedOn`,
    					`ssn`.`updated_by` AS `UpdatedBy`,
    					`ssn`.`updated_on` AS `UpdatedOn`,
			            NULL
			        
			        FROM  slt_site_notice ssn
    					INNER JOIN slt_profile_list_items lang ON lang.code = ssn.lang_code
    					INNER JOIN slt_profile_list_items sta ON sta.code = ssn.status
    				
			        WHERE `ssn`.`status` <> 'cre_sys_obt_not_del' AND ssn.updated_on > '".$this->updSync_on ."'";

			expDebug::dPrint('tempTableDataPopulate query: ' . $tempDataQuery, 4);
			$this->db->callQuery($tempDataQuery);
		} catch (Exception $ex){
			throw new Exception("Error in tempTableDataPopulate ".$ex->getMessage());
		}
	}
	
	/**
	 * Method used to insert into flat table
	 */
	
	protected function reportInsert() {
		try {
			
			// truncate flat table, when script invoked with flush true
			if ($this->flush == 'true') {
				$flushTable  = "TRUNCATE TABLE " . $this->report_table ."; ";
				expDebug::dPrint('$flushTable query :' . $flushTable, 4);
				$this->db->callQuery($flushTable);
			}
			
			$query = "insert into ". $this->report_table ."
					SELECT 
						`RUID` ,
 						`AnnID` ,
 						`AnnUserID` ,
  					    `AnnDescription` ,
						`AnnLangCode` ,
						`AnnStatus` ,
						`AnnFromDate` ,
						`AnnToDate` ,
						`AnnFromTime`,
						`AnnToTime`,
						`AnnPriority`,
						`CreatedBy` ,
						`CreatedOn`,
						`UpdatedBy` ,
						`UpdatedOn`
					FROM ". $this->temp_table_name ."
					where operation is NULL;";
			
			expDebug::dPrint("reportInsert Query" . print_r($query, 1), 4);
			$this->queryProcess($query, $this->report_table, 'insert', $this->insSync_on);
		} catch(Exception $ex) {
			throw new Exception("Error in reportInsert ".$ex->getMessage());
		}	
	}
	
	/**
	 * Method used to update into flat table
	 */
	protected function reportUpdate() {
		try {
		$query = "update ". $this->report_table ." flat
						join ". $this->temp_table_name ." temp on flat.RUID = temp.RUID
						set
							`flat`.`AnnID` 	=	`temp`.`AnnID`, 
							`flat`.`AnnUserID` 	=	`temp`.`AnnUserID` ,
							`flat`.`AnnDescription` 	=	`temp`.`AnnDescription` ,
							`flat`.`AnnLangCode` 	=	`temp`.`AnnLangCode`,
							`flat`.`AnnStatus` 	=	`temp`.`AnnStatus` ,
							`flat`.`AnnFromDate` 	=	`temp`.`AnnFromDate`, 
							`flat`.`AnnToDate` 	=	`temp`.`AnnToDate` ,
							`flat`.`AnnFromTime`	=	`temp`.`AnnFromTime`,
							`flat`.`AnnToTime`	=	`temp`.`AnnToTime`,
							`flat`.`AnnPriority`	=	`temp`.`AnnPriority`,
							`flat`.`CreatedBy` 	=	`temp`.`CreatedBy`, 
							`flat`.`CreatedOn`	=	`temp`.`CreatedOn`,
							`flat`.`UpdatedBy` 	=	`temp`.`UpdatedBy`, 
							`flat`.`UpdatedOn`	=	`temp`.`UpdatedOn`,
						        
							`temp`.`operation` =  'update'	
						where flat.RUID = temp.RUID;";
			expDebug::dPrint("reportUpdate Query" . print_r($query, 1), 4);
			$this->queryProcess($query, $this->report_table, 'update', $this->updSync_on);
		} catch(Exception $ex){
			throw new Exception("Error in reportUpdate ".$ex->getMessage());
		}		
	}
	
	/**
	 * Method used to collect delta Ids
	 * 
	 * @param string $action
	 * @return string
	 */
	protected function reportCollectIds($action) {
		try {
			$query = "SELECT
						`ssn`.`id` as `RUID`
			        FROM slt_site_notice ssn";
			
			If ($action == 'delete') {
				$query .= " where ssn.`status` = 'cre_sys_obt_not_del' and ssn.updated_on > '".$this->delSync_on ."' ";
			}
			
			return $query;
			
		} catch(Exception $ex){
			throw new Exception("Error in reportCollectIds ".$ex->getMessage());
		}	
	}
} 
	
	
