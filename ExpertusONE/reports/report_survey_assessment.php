<?php
include_once $_SERVER['DOCUMENT_ROOT'] .  '/reports/ReportSyncUp.php';

class report_survey_assessment extends ReportSyncUp {
	
	
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
			  `RUID` varchar(255) NOT NULL,
			  `SurAssID` int(11) NOT NULL DEFAULT '0',
			  `SurAssTitle` longtext,
			  `SurAssDescription` longtext,
              `SurAssCode` varchar(100) DEFAULT NULL,
              `SurAssMaxScore` float(11,2) DEFAULT NULL,
              `SurAssMinScore` float(11,2) DEFAULT NULL,
              `SurAssType` longtext,
	          `SurAssStatus` varchar(100) DEFAULT NULL,
              `QuestionsPerPage` int(11) NOT NULL DEFAULT '0',
              `SurAssLanguage` longtext,
              `SurAssStatusName` longtext,
              `SurAssCreatedBy` text,
              `SurAssCreatedOn` datetime DEFAULT NULL,
              `SurAssUpdatedBy` text,
              `SurAssUpdatedOn` datetime DEFAULT NULL,
              `SurAssDeletedBy` text,
              `SurAssDeletedOn` datetime DEFAULT NULL,
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
						 sur.id AS `RUID`,
						`sur`.`id` as `SurAssID`,
						`sur`.`title` as `SurAssTitle`,
						`sur`.`short_description` as `SurAssDescription`,
						`sur`.`code` as `SurAssCode`,
						`sur`.`max_mark` as `SurAssMaxScore`,
						`sur`.`min_mark` as `SurAssMinScore`,
						`surtype`.`name` AS `SurAssType`,
						`sur`.`status` AS `SurAssStatus`,
						`sur`.`question_per_page` AS `QuestionsPerPage`,
						`lang`.`name` AS `SurAssLanguage`,
						`sta`.`name` AS `SurAssStatusName`,
						`sur`.`created_by` AS `SurAssCreatedBy`,
						`sur`.`created_on` AS `SurAssCreatedOn`,
						`sur`.`updated_by` AS `SurAssUpdatedBy`,
						`sur`.`updated_on` AS `SurAssUpdatedOn`,
						`sur`.`deleted_by` AS `SurAssDeletedBy`,
						`sur`.`deleted_on` AS `SurAssDeletedOn`,
						NULL
			        FROM slt_survey sur
						INNER JOIN slt_profile_list_items surtype ON (surtype.code = sur.type) AND (surtype.lang_code = 'cre_sys_lng_eng')
						INNER JOIN slt_profile_list_items lang ON sur.lang_code = lang.code
						INNER JOIN slt_profile_list_items sta ON sur.status = sta.code
				    where ((sur.updated_on > '".$this->updSync_on ."')) AND	sur.status != 'sry_det_sry_del' ";
			
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
						`RUID`,
						`SurAssID` ,
						`SurAssTitle` ,
						`SurAssDescription` ,
						`SurAssCode`,
						`SurAssMaxScore`,
						`SurAssMinScore`,
						`SurAssType` ,
						`SurAssStatus`,
						`QuestionsPerPage` ,
						`SurAssLanguage` ,
						`SurAssStatusName` ,
						`SurAssCreatedBy` ,
						`SurAssCreatedOn` ,
						`SurAssUpdatedBy` ,
						`SurAssUpdatedOn` ,
						`SurAssDeletedBy` ,
						`SurAssDeletedOn` 
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
							`flat`.`SurAssID` 	=	`temp`.`SurAssID` ,
							`flat`.`SurAssTitle` 	=	`temp`.`SurAssTitle` ,
							`flat`.`SurAssDescription` 	=	`temp`.`SurAssDescription` ,
							`flat`.`SurAssCode`	=	`temp`.`SurAssCode`,
							`flat`.`SurAssMaxScore`	=	`temp`.`SurAssMaxScore`,
							`flat`.`SurAssMinScore`	=	`temp`.`SurAssMinScore`,
							`flat`.`SurAssType` 	=	`temp`.`SurAssType` ,
							`flat`.`SurAssStatus`	=	`temp`.`SurAssStatus`,
							`flat`.`QuestionsPerPage` 	=	`temp`.`QuestionsPerPage` ,
							`flat`.`SurAssLanguage` 	=	`temp`.`SurAssLanguage` ,
							`flat`.`SurAssStatusName` 	=	`temp`.`SurAssStatusName` ,
							`flat`.`SurAssCreatedBy` 	=	`temp`.`SurAssCreatedBy` ,
							`flat`.`SurAssCreatedOn` 	=	`temp`.`SurAssCreatedOn` ,
							`flat`.`SurAssUpdatedBy` 	=	`temp`.`SurAssUpdatedBy` ,
							`flat`.`SurAssUpdatedOn` 	=	`temp`.`SurAssUpdatedOn` ,
							`flat`.`SurAssDeletedBy` 	=	`temp`.`SurAssDeletedBy` ,
							`flat`.`SurAssDeletedOn` 	=	`temp`.`SurAssDeletedOn` ,
						        
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
						sur.id AS `RUID`
			 		  FROM 
						slt_survey sur";
			
			If ($action == 'delete') {
				$query .= " where (sur.status = 'sry_det_sry_del' and sur.updated_on > '".$this->delSync_on ."')";
			}
			
			return $query;
			
		} catch(Exception $ex){
			throw new Exception("Error in reportCollectIds ".$ex->getMessage());
		}	
	}
} 
	
	
