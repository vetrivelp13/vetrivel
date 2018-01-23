<?php
include_once $_SERVER['DOCUMENT_ROOT'] .  '/reports/ReportSyncUp.php';

class report_program_details extends ReportSyncUp {
	
	
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
			$delQuery = "DELETE rprm FROM ". $this->report_table ." as rprm JOIN (".$delSubQuery.") as deleted ON rprm.TrainingPlanID = deleted.RUID";
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
              `TrainingPlanID` int(11) NOT NULL DEFAULT '0',
              `TrainingPlanTitle` varchar(255) DEFAULT NULL,
              `TrainingPlanCode` varchar(100) NOT NULL,
              `TrainingPlanLanguage` longtext,
              `TrainingPlanDescription` longtext,
              `TrainingPlanAdditionalInformation` longtext,
              `TrainingPlanAddInfoInCatalog` int(1) NOT NULL DEFAULT '0',
              `TrainingPlanAddInfoInNotification` int(1) NOT NULL DEFAULT '0',
              `TrainingPlanPrice` decimal(16,2) DEFAULT '0.00',
              `TrainingPlanExpiresInValue` int(11) DEFAULT NULL,
              `TrainingPlanExpiresInUnit` enum('days','months','years') DEFAULT NULL,
              `TrainingPlanCurrency` varchar(4) DEFAULT NULL,
              `TrainingPlanAuthorVendor` varchar(255) DEFAULT NULL,
              `TrainingPlanStatus` longtext,
              `TrainingPlanObjectName` longtext,
              `CourseID` int(11) DEFAULT NULL,
              `TrainingPlanIsRequired` varchar(3) DEFAULT NULL,
              `TrainingPlanEnforceSequence` char(1) DEFAULT 'N',
              `TrainingPlanDeletedBy` text,
              `TrainingPlanDeletedOn` datetime DEFAULT NULL,
              `TrainingPlanCreatedBy` varchar(100) DEFAULT NULL,
              `TrainingPlanCreatedOn` datetime DEFAULT NULL,
              `TrainingPlanUpdatedBy` varchar(100) DEFAULT NULL,
              `TrainingPlanUpdatedOn` datetime DEFAULT NULL,
              `TrainingPlanPublishedOn` datetime DEFAULT NULL,
              `EndDate` datetime DEFAULT NULL,
              `CourseTitle` longtext,
              `CourseCode` varchar(255),
              `ClassTitle` longtext,
              `ClassCode` varchar(255),
              `SessionTitle` longtext,
              `TrainingPlanModuleTitle` varchar(255) DEFAULT NULL,
              `ProgramStatus` varchar(100) DEFAULT NULL,
              `TrainingPlanCustomAttribute0` VARCHAR(750) DEFAULT NULL,
              `TrainingPlanCustomAttribute1` VARCHAR(750) DEFAULT NULL,
              `TrainingPlanCustomAttribute2` VARCHAR(750) DEFAULT NULL,
              `TrainingPlanCustomAttribute3` VARCHAR(750) DEFAULT NULL,
              `TrainingPlanCustomAttribute4` VARCHAR(750) DEFAULT NULL,
              `TrainingPlanCustomAttribute5` VARCHAR(750) DEFAULT NULL,
              `TrainingPlanCustomAttribute6` VARCHAR(750) DEFAULT NULL,
              `TrainingPlanCustomAttribute7` VARCHAR(750) DEFAULT NULL,
              `TrainingPlanCustomAttribute8` VARCHAR(750) DEFAULT NULL,
              `TrainingPlanCustomAttribute9` VARCHAR(750) DEFAULT NULL, 
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
							concat_ws('-', `prm`.`id`, `sg`.`id`, `mcm`.`id`, `cls`.`id`,`ses`.`id`) AS RUID,
							`prm`.`id` 									AS `TrainingPlanID`,
							`prm`.`title` 								AS `TrainingPlanTitle`,
							`prm`.`code` 								AS `TrainingPlanCode`,
							`lang`.`name` 								AS `TrainingPlanLanguage`,
							`prm`.`short_desc` 						    AS `TrainingPlanDescription`,
    						`prm`.`additional_info`                     AS `TrainingPlanAdditionalInformation`,
    						`prm`.`addn_catalog_show`                   AS `TrainingPlanAddInfoInCatalog`,
    						`prm`.`addn_notification_show`              AS `TrainingPlanAddInfoInNotification`,
							`prm`.`price` 								AS `TrainingPlanPrice`,
							`prm`.`expires_in_value` 					AS `TrainingPlanExpiresInValue`,
							`prm`.`expires_in_unit` 					AS `TrainingPlanExpiresInUnit`,
							`prm`.`currency_type` 						AS `TrainingPlanCurrency`,
							`prm`.`author_vendor` 						AS `TrainingPlanAuthorVendor`,
							`stat`.`name` 								AS `TrainingPlanStatus`,
							`pli1`.`name` 								AS `TrainingPlanObjectName`,
							`mcm`.`course_id` 							AS `CourseID`,
							`mcm`.`is_required` 						AS `TrainingPlanIsRequired`,
							`prm`.`enforce_sequence` 					AS `TrainingPlanEnforceSequence`,
							`prm`.`deleted_by` 							AS `TrainingPlanDeletedBy`,
							`prm`.`deleted_on` 							AS `TrainingPlanDeletedOn`,
 							`prm`.`created_by` 							AS `TrainingPlanCreatedBy`,
							`prm`.`created_on` 							AS `TrainingPlanCreatedOn`,
							`prm`.`updated_by` 							AS `TrainingPlanUpdatedBy`,
							`prm`.`updated_on` 							AS `TrainingPlanUpdatedOn`,
							`prm`.`published_on` 						AS `TrainingPlanPublishedOn`,
							`prm`.`end_date` 							AS `EndDate`,
							`crs`.`title` 								AS `CourseTitle`,
							`crs`.`code` 								AS `CourseCode`,
							`cls`.`title` 								AS `ClassTitle`,
							`cls`.`code` 								AS `ClassCode`,
 							`ses`.`title` 								AS `SessionTitle`,
 							`sg`.`title`								AS `TrainingPlanModuleTitle`,
							`prm`.`status`								AS `ProgramStatus`,
    						IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt0`.`opt_name`) FROM slt_custom_attr_options `opt0` WHERE FIND_IN_SET(`opt0`.`opt_code`,`prm`.`e1_cattr0`)),`prm`.`e1_cattr0`),`prm`.`e1_cattr0`) AS `TrainingPlanCustomAttribute0`,
                            IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt1`.`opt_name`) FROM slt_custom_attr_options `opt1` WHERE FIND_IN_SET(`opt1`.`opt_code`,`prm`.`e1_cattr1`)),`prm`.`e1_cattr1`),`prm`.`e1_cattr1`) AS `TrainingPlanCustomAttribute1`,
                            IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt2`.`opt_name`) FROM slt_custom_attr_options `opt2` WHERE FIND_IN_SET(`opt2`.`opt_code`,`prm`.`e1_cattr2`)),`prm`.`e1_cattr2`),`prm`.`e1_cattr2`) AS `TrainingPlanCustomAttribute2`,
                            IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt3`.`opt_name`) FROM slt_custom_attr_options `opt3` WHERE FIND_IN_SET(`opt3`.`opt_code`,`prm`.`e1_cattr3`)),`prm`.`e1_cattr3`),`prm`.`e1_cattr3`) AS `TrainingPlanCustomAttribute3`,
                            IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt4`.`opt_name`) FROM slt_custom_attr_options `opt4` WHERE FIND_IN_SET(`opt4`.`opt_code`,`prm`.`e1_cattr4`)),`prm`.`e1_cattr4`),`prm`.`e1_cattr4`) AS `TrainingPlanCustomAttribute4`,
                            IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt5`.`opt_name`) FROM slt_custom_attr_options `opt5` WHERE FIND_IN_SET(`opt5`.`opt_code`,`prm`.`e1_cattr5`)),`prm`.`e1_cattr5`),`prm`.`e1_cattr5`) AS `TrainingPlanCustomAttribute5`,
                            IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt6`.`opt_name`) FROM slt_custom_attr_options `opt6` WHERE FIND_IN_SET(`opt6`.`opt_code`,`prm`.`e1_cattr6`)),`prm`.`e1_cattr6`),`prm`.`e1_cattr6`) AS `TrainingPlanCustomAttribute6`,
                            IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt7`.`opt_name`) FROM slt_custom_attr_options `opt7` WHERE FIND_IN_SET(`opt7`.`opt_code`,`prm`.`e1_cattr7`)),`prm`.`e1_cattr7`),`prm`.`e1_cattr7`) AS `TrainingPlanCustomAttribute7`,
                            IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt8`.`opt_name`) FROM slt_custom_attr_options `opt8` WHERE FIND_IN_SET(`opt8`.`opt_code`,`prm`.`e1_cattr8`)),`prm`.`e1_cattr8`),`prm`.`e1_cattr8`) AS `TrainingPlanCustomAttribute8`,
                            IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt9`.`opt_name`) FROM slt_custom_attr_options `opt9` WHERE FIND_IN_SET(`opt9`.`opt_code`,`prm`.`e1_cattr9`)),`prm`.`e1_cattr9`),`prm`.`e1_cattr9`) AS `TrainingPlanCustomAttribute9`,
			                NULL

			        FROM `slt_program` `prm`
						LEFT JOIN `slt_module` `sg` ON `prm`.`id` = `sg`.`program_id`
						LEFT JOIN `slt_module_crs_mapping` `mcm` ON `mcm`.`module_id` = `sg`.`id`
						LEFT JOIN `slt_course_template` `crs` ON `mcm`.`course_id`  = `crs`.`id` AND `crs`.`status` <> 'lrn_crs_sts_del'
						LEFT JOIN `slt_course_class` `cls` ON  `crs`.`id` = `cls`.`course_id` AND `cls`.`status` <> 'lrn_cls_sts_del'
						LEFT JOIN  `slt_course_class_session` `ses` ON `ses`.`class_id` =  `cls`.`id`
						INNER JOIN `slt_profile_list_items` `stat` ON `prm`.`status` = `stat`.`code` AND `stat`.`parent_id` = 415
						INNER JOIN `slt_profile_list_items` `pli1` ON `pli1`.`code` = `prm`.`object_type` AND `pli1`.`parent_id` = 139
						INNER JOIN `slt_profile_list_items` `lang` ON `lang`.`code` = `prm`.`lang_code` AND `lang`.`parent_id` = 116
					
			        where ((prm.updated_on > '".$this->updSync_on ."') 
			        			OR (prm.id IN (select distinct program_id from slt_module_crs_mapping smcm LEFT JOIN `slt_course_class` `scrscls` ON `smcm`.`course_id`  = `scrscls`.`course_id` AND `scrscls`.`status` <> 'lrn_cls_sts_del' where scrscls.updated_on > '".$this->updSync_on ."')) 
			        		OR (sg.updated_on > '".$this->updSync_on ."') 
			        				OR (mcm.updated_on > '".$this->updSync_on ."')
			        						OR (crs.updated_on > '".$this->updSync_on ."')
			        								OR (cls.updated_on > '".$this->updSync_on ."')
			        										OR (ses.updated_on > '".$this->updSync_on ."'))
							AND  prm.status != 'lrn_lpn_sts_del' ";
			
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
						`TrainingPlanID` ,
						`TrainingPlanTitle`,
						`TrainingPlanCode` ,
						`TrainingPlanLanguage` ,
						`TrainingPlanDescription` ,
						`TrainingPlanAdditionalInformation` ,
						`TrainingPlanAddInfoInCatalog` ,
						`TrainingPlanAddInfoInNotification`,
						`TrainingPlanPrice` ,
						`TrainingPlanExpiresInValue`,
						`TrainingPlanExpiresInUnit`,
						`TrainingPlanCurrency`,
						`TrainingPlanAuthorVendor`,
						`TrainingPlanStatus` ,
						`TrainingPlanObjectName` ,
						`CourseID` ,
						`TrainingPlanIsRequired`,
						`TrainingPlanEnforceSequence`,
						`TrainingPlanDeletedBy`,
						`TrainingPlanDeletedOn`,
						`TrainingPlanCreatedBy` ,
						`TrainingPlanCreatedOn`,
						`TrainingPlanUpdatedBy` ,
						`TrainingPlanUpdatedOn`,
						`TrainingPlanPublishedOn`,
						`EndDate`,
						`CourseTitle` ,
						`CourseCode` ,
						`ClassTitle` ,
						`ClassCode` ,
						`SessionTitle` ,
						`TrainingPlanModuleTitle`,
						`ProgramStatus`,
                        `TrainingPlanCustomAttribute0`,
                        `TrainingPlanCustomAttribute1`,
                        `TrainingPlanCustomAttribute2`,
                        `TrainingPlanCustomAttribute3`,
                        `TrainingPlanCustomAttribute4`,
                        `TrainingPlanCustomAttribute5`,
                        `TrainingPlanCustomAttribute6`,
                        `TrainingPlanCustomAttribute7`,
                        `TrainingPlanCustomAttribute8`,
                        `TrainingPlanCustomAttribute9`			        
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
							`flat`.`TrainingPlanID` 	=	`temp`.`TrainingPlanID` ,
							`flat`.`TrainingPlanTitle`	=	`temp`.`TrainingPlanTitle`,
							`flat`.`TrainingPlanCode` 	=	`temp`.`TrainingPlanCode` ,
							`flat`.`TrainingPlanLanguage` 	=	`temp`.`TrainingPlanLanguage` ,
							`flat`.`TrainingPlanDescription` 	=	`temp`.`TrainingPlanDescription` ,
							`flat`.`TrainingPlanAdditionalInformation` 	=	`temp`.`TrainingPlanAdditionalInformation` ,
							`flat`.`TrainingPlanAddInfoInCatalog` 	=	`temp`.`TrainingPlanAddInfoInCatalog` ,
							`flat`.`TrainingPlanAddInfoInNotification`	=	`temp`.`TrainingPlanAddInfoInNotification`,
							`flat`.`TrainingPlanPrice` 	=	`temp`.`TrainingPlanPrice` ,
							`flat`.`TrainingPlanExpiresInValue`	=	`temp`.`TrainingPlanExpiresInValue`,
							`flat`.`TrainingPlanExpiresInUnit`	=	`temp`.`TrainingPlanExpiresInUnit`,
							`flat`.`TrainingPlanCurrency`	=	`temp`.`TrainingPlanCurrency`,
							`flat`.`TrainingPlanAuthorVendor`	=	`temp`.`TrainingPlanAuthorVendor`,
							`flat`.`TrainingPlanStatus` 	=	`temp`.`TrainingPlanStatus` ,
							`flat`.`TrainingPlanObjectName` 	=	`temp`.`TrainingPlanObjectName` ,
							`flat`.`CourseID` 	=	`temp`.`CourseID` ,
							`flat`.`TrainingPlanIsRequired`	=	`temp`.`TrainingPlanIsRequired`,
							`flat`.`TrainingPlanEnforceSequence`	=	`temp`.`TrainingPlanEnforceSequence`,
							`flat`.`TrainingPlanDeletedBy`	=	`temp`.`TrainingPlanDeletedBy`,
							`flat`.`TrainingPlanDeletedOn`	=	`temp`.`TrainingPlanDeletedOn`,
							`flat`.`TrainingPlanCreatedBy` 	=	`temp`.`TrainingPlanCreatedBy` ,
							`flat`.`TrainingPlanCreatedOn`	=	`temp`.`TrainingPlanCreatedOn`,
							`flat`.`TrainingPlanUpdatedBy` 	=	`temp`.`TrainingPlanUpdatedBy` ,
							`flat`.`TrainingPlanUpdatedOn`	=	`temp`.`TrainingPlanUpdatedOn`,
							`flat`.`TrainingPlanPublishedOn`	=	`temp`.`TrainingPlanPublishedOn`,
							`flat`.`EndDate`	=	`temp`.`EndDate`,
							`flat`.`CourseTitle` 	=	`temp`.`CourseTitle` ,
							`flat`.`CourseCode` 	=	`temp`.`CourseCode` ,
							`flat`.`ClassTitle` 	=	`temp`.`ClassTitle` ,
							`flat`.`ClassCode` 	=	`temp`.`ClassCode` ,
							`flat`.`SessionTitle` 	=	`temp`.`SessionTitle` ,
							`flat`.`TrainingPlanModuleTitle`	=	`temp`.`TrainingPlanModuleTitle`,
							`flat`.`ProgramStatus` 	=	`temp`.`ProgramStatus` ,
                            `flat`.`TrainingPlanCustomAttribute0`    =   `temp`.`TrainingPlanCustomAttribute0`,
                            `flat`.`TrainingPlanCustomAttribute1`   =   `temp`.`TrainingPlanCustomAttribute1`,
                            `flat`.`TrainingPlanCustomAttribute2`   =   `temp`.`TrainingPlanCustomAttribute2`,
                            `flat`.`TrainingPlanCustomAttribute3`   =   `temp`.`TrainingPlanCustomAttribute3`,
                            `flat`.`TrainingPlanCustomAttribute4`   =   `temp`.`TrainingPlanCustomAttribute4`,
                            `flat`.`TrainingPlanCustomAttribute5`   =   `temp`.`TrainingPlanCustomAttribute5`,
                            `flat`.`TrainingPlanCustomAttribute6`   =   `temp`.`TrainingPlanCustomAttribute6`,
                            `flat`.`TrainingPlanCustomAttribute7`   =   `temp`.`TrainingPlanCustomAttribute7`,
                            `flat`.`TrainingPlanCustomAttribute8`   =   `temp`.`TrainingPlanCustomAttribute8`,
                            `flat`.`TrainingPlanCustomAttribute9`   =   `temp`.`TrainingPlanCustomAttribute9`, 
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
						DISTINCT  `prm`.`id` AS RUID
							FROM `slt_program` `prm`
							LEFT JOIN `slt_module` `sg` ON `prm`.`id` = `sg`.`program_id`
							LEFT JOIN `slt_module_crs_mapping` `mcm` ON `mcm`.`module_id` = `sg`.`id`
							LEFT JOIN `slt_course_template` `crs` ON `mcm`.`course_id`  = `crs`.`id`
							LEFT JOIN `slt_course_class` `cls` ON  `crs`.`id` = `cls`.`course_id` 
							LEFT JOIN `slt_course_class_session` `ses` ON `ses`.`class_id` =  `cls`.`id`
						Where  `cls`.`updated_on` > '".$this->delSync_on ."' OR `prm`.`updated_on` > '".$this->delSync_on ."'";
			return $query;
		} catch(Exception $ex){
			throw new Exception("Error in reportCollectIds ".$ex->getMessage());
		}	
	}
} 
	
	
