<?php
include_once $_SERVER['DOCUMENT_ROOT'] .  '/reports/ReportSyncUp.php';

class report_location extends ReportSyncUp {
	
	
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
              `LocationId` int(11) NOT NULL DEFAULT '0',
              `LocationName` longtext,
              `LocationAddress1` varchar(100) DEFAULT NULL,
              `LocationAddress2` varchar(100) DEFAULT NULL,
              `LocationCity` varchar(50) DEFAULT NULL,
              `LocationState` varchar(255) DEFAULT NULL,
              `LocationCountry` varchar(255) DEFAULT NULL,
              `LocationZipCode` varchar(15) DEFAULT NULL,
              `LocationContactFirstName` varchar(255) DEFAULT NULL,
              `Locationemail` varchar(100) DEFAULT NULL,
              `LocationCapacity` int(11) DEFAULT NULL,
              `LocationLatitude` float NOT NULL,
              `LocationLongitude` float NOT NULL,
              `LocationTimezone` varchar(100) DEFAULT NULL,
              `LocationStatus` longtext,
              `LocationContactPhone` varchar(100) DEFAULT NULL,
              `LocationCreatedBy` text,
              `LocationCreatedOn` datetime DEFAULT NULL,
              `LocationUpdatedOn` datetime DEFAULT NULL,
              `LocationDeletedBy` text,
              `LocationDeletedOn` datetime DEFAULT NULL,
              `LocationUpdatedBy` text,
              `LocationCustomAttribute0` VARCHAR(750) DEFAULT NULL,
              `LocationCustomAttribute1` VARCHAR(750) DEFAULT NULL,
              `LocationCustomAttribute2` VARCHAR(750) DEFAULT NULL,
              `LocationCustomAttribute3` VARCHAR(750) DEFAULT NULL,
              `LocationCustomAttribute4` VARCHAR(750) DEFAULT NULL,
              `LocationCustomAttribute5` VARCHAR(750) DEFAULT NULL,
              `LocationCustomAttribute6` VARCHAR(750) DEFAULT NULL,
              `LocationCustomAttribute7` VARCHAR(750) DEFAULT NULL,
              `LocationCustomAttribute8` VARCHAR(750) DEFAULT NULL,
              `LocationCustomAttribute9` VARCHAR(750) DEFAULT NULL, 
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
						loc.id` as RUID`,
                        `loc`.`id` as `LocationId`,
                        `loc`.`name` as `LocationName`,
                        `loc`.`addr1` as `LocationAddress1`,
                        `loc`.`addr2` as `LocationAddress2`,
                        `loc`.`city` as `LocationCity`,
                        `sta`.`state_name` as `LocationState`,
                        `cnt`.`country_name` as `LocationCountry`,
                        `loc`.`zipcode` as `LocationZipCode`,
                        `loc`.`contact_fname` as `LocationContactFirstName`,
                        `loc`.`email_id` as `Locationemail`,
                        `loc`.`capacity` as `LocationCapacity`,
                        `loc`.`latitude` as `LocationLatitude`,
                        `loc`.`longitude` as `LocationLongitude`,
                        `loc`.`timezone` as `LocationTimezone`,
                        `pli`.`name` as `LocationStatus`,
                        `loc`.`phone` as `LocationContactPhone`,
                        `loc`.`created_by` AS `LocationCreatedBy`,
                        `loc`.`created_on` AS `LocationCreatedOn`,
                        `loc`.`updated_on` AS `LocationUpdatedOn`,
                        `loc`.`deleted_by` AS `LocationDeletedBy`,
                        `loc`.`deleted_on` AS `LocationDeletedOn`,
                        `loc`.`updated_by` AS `LocationUpdatedBy` ,
            			IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt0`.`opt_name`) FROM slt_custom_attr_options `opt0` WHERE FIND_IN_SET(`opt0`.`opt_code`,`loc`.`e1_cattr0`)),`loc`.`e1_cattr0`),`loc`.`e1_cattr0`) AS `LocationCustomAttribute0`,
            			IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt1`.`opt_name`) FROM slt_custom_attr_options `opt1` WHERE FIND_IN_SET(`opt1`.`opt_code`,`loc`.`e1_cattr1`)),`loc`.`e1_cattr1`),`loc`.`e1_cattr1`) AS `LocationCustomAttribute1`,
            			IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt2`.`opt_name`) FROM slt_custom_attr_options `opt2` WHERE FIND_IN_SET(`opt2`.`opt_code`,`loc`.`e1_cattr2`)),`loc`.`e1_cattr2`),`loc`.`e1_cattr2`) AS `LocationCustomAttribute2`,
            			IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt3`.`opt_name`) FROM slt_custom_attr_options `opt3` WHERE FIND_IN_SET(`opt3`.`opt_code`,`loc`.`e1_cattr3`)),`loc`.`e1_cattr3`),`loc`.`e1_cattr3`) AS `LocationCustomAttribute3`,
            			IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt4`.`opt_name`) FROM slt_custom_attr_options `opt4` WHERE FIND_IN_SET(`opt4`.`opt_code`,`loc`.`e1_cattr4`)),`loc`.`e1_cattr4`),`loc`.`e1_cattr4`) AS `LocationCustomAttribute4`,
            			IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt5`.`opt_name`) FROM slt_custom_attr_options `opt5` WHERE FIND_IN_SET(`opt5`.`opt_code`,`loc`.`e1_cattr5`)),`loc`.`e1_cattr5`),`loc`.`e1_cattr5`) AS `LocationCustomAttribute5`,
            			IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt6`.`opt_name`) FROM slt_custom_attr_options `opt6` WHERE FIND_IN_SET(`opt6`.`opt_code`,`loc`.`e1_cattr6`)),`loc`.`e1_cattr6`),`loc`.`e1_cattr6`) AS `LocationCustomAttribute6`,
            			IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt7`.`opt_name`) FROM slt_custom_attr_options `opt7` WHERE FIND_IN_SET(`opt7`.`opt_code`,`loc`.`e1_cattr7`)),`loc`.`e1_cattr7`),`loc`.`e1_cattr7`) AS `LocationCustomAttribute7`,
            			IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt8`.`opt_name`) FROM slt_custom_attr_options `opt8` WHERE FIND_IN_SET(`opt8`.`opt_code`,`loc`.`e1_cattr8`)),`loc`.`e1_cattr8`),`loc`.`e1_cattr8`) AS `LocationCustomAttribute8`,
            			IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt9`.`opt_name`) FROM slt_custom_attr_options `opt9` WHERE FIND_IN_SET(`opt9`.`opt_code`,`loc`.`e1_cattr9`)),`loc`.`e1_cattr9`),`loc`.`e1_cattr9`) AS `LocationCustomAttribute9`,
			             NULL

			        FROM slt_location loc 
                        INNER JOIN slt_country cnt ON cnt.country_code = loc.country
                        LEFT JOIN slt_state sta ON sta.country_code = cnt.country_code AND sta.state_code = loc.state
                        INNER JOIN slt_profile_list_items pli ON loc.status = pli.code
					
			        where loc.status != 'lrn_res_loc_del' and (loc.updated_on > '".$this->updSync_on ."')";
			
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
						`LocationId` ,
						`LocationName` ,
						`LocationAddress1` ,
						`LocationAddress2` ,
						`LocationCity`,
						`LocationState` ,
						`LocationCountry` ,
						`LocationZipCode`,
						`LocationContactFirstName` ,
						`Locationemail` ,
						`LocationCapacity`,
						`LocationLatitude` ,
						`LocationLongitude` ,
						`LocationTimezone` ,
						`LocationStatus` ,
						`LocationContactPhone` ,
						`LocationCreatedBy` ,
						`LocationCreatedOn` ,
						`LocationUpdatedOn` ,
						`LocationDeletedBy` ,
						`LocationDeletedOn` ,
						`LocationUpdatedBy` ,
						`LocationCustomAttribute0`,
						`LocationCustomAttribute1`,
						`LocationCustomAttribute2`,
						`LocationCustomAttribute3`,
						`LocationCustomAttribute4`,
						`LocationCustomAttribute5`,
						`LocationCustomAttribute6`,
						`LocationCustomAttribute7`,
						`LocationCustomAttribute8`,
						`LocationCustomAttribute9`   
					
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
							`flat`.`LocationId` 		=	`temp`.`LocationId` ,
							`flat`.`LocationName` 		=	`temp`.`LocationName`, 
							`flat`.`LocationAddress1` 	=	`temp`.`LocationAddress1`, 
							`flat`.`LocationAddress2` 	=	`temp`.`LocationAddress2` ,
							`flat`.`LocationCity`		=	`temp`.`LocationCity`,
							`flat`.`LocationState` 		=	`temp`.`LocationState`, 
							`flat`.`LocationCountry` 	=	`temp`.`LocationCountry`, 
							`flat`.`LocationZipCode`	=	`temp`.`LocationZipCode`,
							`flat`.`LocationContactFirstName` 	=	`temp`.`LocationContactFirstName`,
							`flat`.`Locationemail` 		=	`temp`.`Locationemail` ,
							`flat`.`LocationCapacity`	=	`temp`.`LocationCapacity`,
							`flat`.`LocationLatitude` 	=	`temp`.`LocationLatitude` ,
							`flat`.`LocationLongitude` 	=	`temp`.`LocationLongitude` ,
							`flat`.`LocationTimezone` 	=	`temp`.`LocationTimezone` ,
							`flat`.`LocationStatus` 	=	`temp`.`LocationStatus` ,
							`flat`.`LocationContactPhone` 	=	`temp`.`LocationContactPhone`, 
							`flat`.`LocationCreatedBy` 	=	`temp`.`LocationCreatedBy` ,
							`flat`.`LocationCreatedOn` 	=	`temp`.`LocationCreatedOn` ,
							`flat`.`LocationUpdatedOn` 	=	`temp`.`LocationUpdatedOn` ,
							`flat`.`LocationDeletedBy` 	=	`temp`.`LocationDeletedBy` ,
							`flat`.`LocationDeletedOn` 	=	`temp`.`LocationDeletedOn` ,
							`flat`.`LocationUpdatedBy` 	=	`temp`.`LocationUpdatedBy`,
							`flat`.`LocationCustomAttribute0`	=	`temp`.`LocationCustomAttribute0`,
							`flat`.`LocationCustomAttribute1`	=	`temp`.`LocationCustomAttribute1`,
							`flat`.`LocationCustomAttribute2`	=	`temp`.`LocationCustomAttribute2`,
							`flat`.`LocationCustomAttribute3`	=	`temp`.`LocationCustomAttribute3`,
							`flat`.`LocationCustomAttribute4`	=	`temp`.`LocationCustomAttribute4`,
							`flat`.`LocationCustomAttribute5`	=	`temp`.`LocationCustomAttribute5`,
							`flat`.`LocationCustomAttribute6`	=	`temp`.`LocationCustomAttribute6`,
							`flat`.`LocationCustomAttribute7`	=	`temp`.`LocationCustomAttribute7`,
							`flat`.`LocationCustomAttribute8`	=	`temp`.`LocationCustomAttribute8`,
							`flat`.`LocationCustomAttribute9`	=	`temp`.`LocationCustomAttribute9`,
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
						loc.id as `RUID`
			          FROM slt_location loc";

			If ($action == 'delete') {
				$query .= " where loc.`status` = 'lrn_res_loc_del' and loc.updated_on > '".$this->delSync_on ."' ";
			}
			
			return $query;
			
		} catch(Exception $ex){
			throw new Exception("Error in reportCollectIds ".$ex->getMessage());
		}	
	}
} 
	
	
