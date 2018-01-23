<?php
include_once $_SERVER['DOCUMENT_ROOT'] .  '/reports/ReportSyncUp.php';

class report_banner extends ReportSyncUp {
	
	
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
			  `BannerID` int(11) NOT NULL DEFAULT '0',
			  `BannerTitle` longtext,
			  `BannerDescription` text NOT NULL,
			  `BannerLangCode` longtext,
			  `BannerStatus` longtext,
			  `BannerDateActivate` varchar(10) DEFAULT NULL,
			  `BannerDateDeactivate` varchar(10) DEFAULT NULL,
			  `BannerSequenceNo` tinyint(4) DEFAULT NULL,
			  `BannerEntityType` longtext,
			  `ContentDeletedBy` text,
			  `ContentDeletedOn` datetime DEFAULT NULL,
			  `BannerCreatedBy` text,
			  `BannerCreatedOn` datetime DEFAULT NULL,
			  `BannerUpdatedBy` text,
			  `BannerUpdatedOn` datetime DEFAULT NULL,
			  `SAMStatus` longtext,
			  `operation` varchar(10) NULL DEFAULT NULL,
			  PRIMARY KEY (`RUID`),
			  key sli_op(operation)
			) ENGINE=InnoDB DEFAULT CHARSET=utf8;";
			$this->db->callQuery($createTable);
			expDebug::dPrint('tempTableCreate query: ' . $createTable, 4);
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
						`sam`.`id` AS `RUID`,
                        `sam`.`id` AS `BannerID`,
                        `sam`.`title` AS `BannerTitle`,
                        `sam`.`shortdesc` AS `BannerDescription`,
                        `blng`.`name` AS `BannerLangCode`,
                        `bsts`.`name` AS `BannerStatus`,
                        DATE_FORMAT(`sam`.`date_activate`,'%d-%m-%Y') AS `BannerDateActivate`,
                        DATE_FORMAT(`sam`.`date_deactivate`,'%d-%m-%Y') AS `BannerDateDeactivate`,
                        `sam`.`banner_seq_num` AS `BannerSequenceNo`,
                        `bat`.`name` AS `BannerEntityType`,
                        `sam`.`deleted_by` AS `ContentDeletedBy`,
                        `sam`.`deleted_on` AS `ContentDeletedOn`,
                        `sam`.`created_by` AS `BannerCreatedBy`,
                        `sam`.`created_on` AS `BannerCreatedOn`,
                        `sam`.`updated_by` AS `BannerUpdatedBy`,
                        `sam`.`updated_on` AS `BannerUpdatedOn`,
                        `sam`.`status` AS `SAMStatus`,
			             NULL
			        
                    FROM slt_announcement_master sam
                        INNER JOIN slt_profile_list_items blng on blng.code = sam.lang_code AND blng.parent_id =  116
                        INNER JOIN slt_profile_list_items bsts on bsts.code = sam.status AND bsts.parent_id = 2
                        INNER JOIN slt_profile_list_items bat on bat.code = sam.ann_type AND bat.parent_id = 5

			        where ( sam.status != 'cbn_anm_sts_del' AND 
							sam.updated_on > '".$this->updSync_on ."'
						  )";
			
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
			            `BannerID`,
			            `BannerTitle` ,
			            `BannerDescription`,
			            `BannerLangCode` ,
			            `BannerStatus` ,
			            `BannerDateActivate`,
			            `BannerDateDeactivate`,
			            `BannerSequenceNo` ,
			            `BannerEntityType` ,
			            `ContentDeletedBy` ,
			            `ContentDeletedOn` ,
			            `BannerCreatedBy` ,
			            `BannerCreatedOn` ,
			            `BannerUpdatedBy` ,
			            `BannerUpdatedOn` ,
			            `SAMStatus`
			        
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
						    `flat`.`BannerID`	=	`temp`.`BannerID`,
						    `flat`.`BannerTitle` 	=	`temp`.`BannerTitle` ,
						    `flat`.`BannerDescription`	=	`temp`.`BannerDescription`,
						    `flat`.`BannerLangCode` 	=	`temp`.`BannerLangCode` ,
						    `flat`.`BannerStatus` 	=	`temp`.`BannerStatus` ,
						    `flat`.`BannerDateActivate`	=	`temp`.`BannerDateActivate`,
						    `flat`.`BannerDateDeactivate`	=	`temp`.`BannerDateDeactivate`,
						    `flat`.`BannerSequenceNo` 	=	`temp`.`BannerSequenceNo` ,
						    `flat`.`BannerEntityType` 	=	`temp`.`BannerEntityType` ,
						    `flat`.`ContentDeletedBy` 	=	`temp`.`ContentDeletedBy` ,
						    `flat`.`ContentDeletedOn` 	=	`temp`.`ContentDeletedOn` ,
						    `flat`.`BannerCreatedBy` 	=	`temp`.`BannerCreatedBy` ,
						    `flat`.`BannerCreatedOn` 	=	`temp`.`BannerCreatedOn` ,
						    `flat`.`BannerUpdatedBy` 	=	`temp`.`BannerUpdatedBy` ,
						    `flat`.`BannerUpdatedOn` 	=	`temp`.`BannerUpdatedOn` ,
						    `flat`.`SAMStatus`	=	`temp`.`SAMStatus`, 
		
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
						`sam`.`id` AS `RUID`
			        FROM slt_announcement_master sam";
			
			If ($action == 'delete') {
				$query .= " where sam.`status` = 'cbn_anm_sts_del' and sam.updated_on > '".$this->delSync_on ."' ";
			}
			
			return $query;
			
		} catch(Exception $ex){
			throw new Exception("Error in reportCollectIds ".$ex->getMessage());
		}	
	}
} 
	
	
