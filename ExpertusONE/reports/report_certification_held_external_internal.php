<?php
include_once $_SERVER['DOCUMENT_ROOT'] .  '/reports/ReportSyncUp.php';

class report_certification_held_external_internal extends ReportSyncUp {
	
	
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
			$delQuery = "DELETE rprm FROM ". $this->report_table ." as rprm JOIN (".$delSubQuery.") as deleted ON rprm.TPEnrollmentID = deleted.TPEnrollmentID";
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
			
			$dropTable  = "DROP TABLE IF EXISTS " . $this->temp_table_name ."; ";
			$this->db->callQuery($dropTable);
			
			// $createTable = "CREATE TABLE ".$this->temp_table_name." LIKE ".$this->report_table."";
			$createTable = "CREATE TABLE ".$this->temp_table_name." (
						  `RUID` varchar(255),
						  `TPEnrollmentID` int(11) NOT NULL DEFAULT '0',
						  `TPUserID` int(11) NOT NULL DEFAULT '0',
						  `TPOrgID` int(11) NOT NULL DEFAULT '0',
						  `TrainingPlanID` int(11) NOT NULL DEFAULT '0',
						  `TrainingPlanTitle` varchar(255) DEFAULT NULL,
						  `TrainingPlanCode` varchar(100) NOT NULL,
						  `TrainingPlanObjectName` varchar(255) DEFAULT NULL,
						  `TrainingPlanStatusCode` varchar(100) DEFAULT NULL,
						  `TrainingPlanStatus` longtext,
						  `TPEnrollmentDate` datetime DEFAULT NULL,
						  `FullName` varchar(255) DEFAULT NULL,
						  `JobTitle` longtext,
						  `JobRole` longtext,
						  `OrganizationName` longtext,
						  `OrganizationStatus` varchar(50) DEFAULT NULL,
						  `OrganizationType` longtext,
						  `TPOverallStatus` longtext,
						  `TPCompletionDate` datetime DEFAULT NULL,
						  `TPIsCurrent` char(1) DEFAULT 'Y',
						  `TPCertPath` int(5) DEFAULT '0',
						  `TPCertStatus` varchar(11) NOT NULL DEFAULT '',
							`operation` varchar(10) NULL DEFAULT NULL,
							primary key(RUID),
							key sli_op(operation)
						) ENGINE=InnoDB DEFAULT CHARSET=utf8;";
			$this->db->callQuery($createTable);
			
			// $alterTable = "ALTER TABLE ".$this->temp_table_name." ADD COLUMN `operation` varchar(10) NULL DEFAULT NULL;";
			// $this->db->callQuery($alterTable);
			
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
						concat_ws('-', `menr`.`id`, `lpjm1`. `id`)  AS `RUID`,
						`menr`.`id`                        	  AS `TPEnrollmentID`,	
                        `menr`.`user_id`        			  AS `TPUserID`,
                        `org_id`				              AS `TPOrgID`,	
                        `prm`.`id` 				              AS `TrainingPlanID`,
						`prm`.`title`  			              AS `TrainingPlanTitle`,
 						`prm`.`code`    			              AS `TrainingPlanCode`,
 						`prm`.`object_type`                	  AS `TrainingPlanObjectName`,
						`prm`.`status`                 	      AS `TrainingPlanStatusCode`,
						`prgsts`.`name` 			              AS `TrainingPlanStatus`,
						`menr`.`reg_date`           		      AS `TPEnrollmentDate`,
 						`per`.`full_name` 			              AS `FullName`,
 						`jobtit`.`name` 			 			  AS `JobTitle`,
 						`jobrole`.`name` 			 			  AS `JobRole`,
 						`org`.`name` 				 			  AS `OrganizationName`,
 						`org`.`status` 			 			  AS `OrganizationStatus`,
 						`orgtype`.`name` 			 			  AS `OrganizationType`,
 						`comstatus`.`name`                  	  AS `TPOverallStatus`,
						`menr`.`comp_date`                  	  AS `TPCompletionDate`,
						`menr`.`is_current`		              AS `TPIsCurrent`,
						`menr`.`recertify_path`		          AS `TPCertPath`,
						if(`menr`.`recertify_path` > 1, 'Recertified','Certified') AS `TPCertStatus`,
                        NULL
			        FROM slt_program prm 
						INNER JOIN `slt_master_enrollment` `menr` ON  `menr`.`program_id` = `prm`.`id`AND `prm`.`status` != 'lrn_lpn_sts_del'
						INNER JOIN `slt_person` `per` ON `per`.`id` = `menr`.`user_id` AND `per`.`status` != 'cre_usr_sts_del'
						LEFT JOIN `slt_organization` `org` ON `org`.`id` = `per`.`org_id`  AND `org`.`status` != 'cre_org_sts_del' 
						LEFT JOIN `slt_person_jobrole_mapping` `lpjm1` ON `lpjm1`.`user_id` = `per`.`id`
						LEFT JOIN `slt_profile_list_items` `jobrole` ON `jobrole`.`code` = `lpjm1`.`job_role` AND `jobrole`.`parent_id` = 267 
						LEFT JOIN `slt_profile_list_items` `jobtit` ON `jobtit`.`code` = `per`.`job_title` AND `jobtit`.`parent_id` = 270 AND `jobtit`.`lang_code` = 'cre_sys_lng_eng'
						LEFT JOIN `slt_profile_list_items` `orgtype` ON `orgtype`.`code` = `org`.`type` AND `orgtype`.`parent_id` = 39
						INNER JOIN `slt_profile_list_items` `prgsts` ON `prgsts`.`code` = `prm`.`status`
						INNER JOIN `slt_profile_list_items` `comstatus` ON `comstatus`.`code` = `menr`.`overall_status` AND `comstatus`.`lang_code` = 'cre_sys_lng_eng'
					
			        where ((prm.updated_on > '".$this->updSync_on ."') 
			        		OR (menr.updated_on > '".$this->updSync_on ."') 
			        		OR (per.updated_on > '".$this->updSync_on ."')
							OR (org.updated_on > '".$this->updSync_on ."')) ";
			
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
						`TPEnrollmentID`,
			            `TPUserID`,
                        `TPOrgID`,	
						`TrainingPlanID`,
						`TrainingPlanTitle`,
						`TrainingPlanCode`,
						`TrainingPlanObjectName`,
						`TrainingPlanStatusCode`,
						`TrainingPlanStatus` ,
						`TPEnrollmentDate`,
						`FullName`,
						`JobTitle` ,
						`JobRole` ,
						`OrganizationName` ,
						`OrganizationStatus`,
						`OrganizationType` ,
						`TPOverallStatus` ,
						`TPCompletionDate`,
						`TPIsCurrent`,
						`TPCertPath`,
						`TPCertStatus`
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
							`flat`.`TPEnrollmentID`	=	`temp`.`TPEnrollmentID`,
						    `flat`.`TPUserID`	=	`temp`.`TPUserID`,
						    `flat`.`TPOrgID`	=	`temp`.`TPOrgID`,        
							`flat`.`TrainingPlanID`	=	`temp`.`TrainingPlanID`,
							`flat`.`TrainingPlanTitle`	=	`temp`.`TrainingPlanTitle`,
							`flat`.`TrainingPlanCode`	=	`temp`.`TrainingPlanCode`,
							`flat`.`TrainingPlanObjectName`	=	`temp`.`TrainingPlanObjectName`,
							`flat`.`TrainingPlanStatusCode`	=	`temp`.`TrainingPlanStatusCode`,
							`flat`.`TrainingPlanStatus` 	=	`temp`.`TrainingPlanStatus` ,
							`flat`.`TPEnrollmentDate`	=	`temp`.`TPEnrollmentDate`,
							`flat`.`FullName`	=	`temp`.`FullName`,
							`flat`.`JobTitle` 	=	`temp`.`JobTitle` ,
							`flat`.`JobRole` 	=	`temp`.`JobRole` ,
							`flat`.`OrganizationName` 	=	`temp`.`OrganizationName` ,
							`flat`.`OrganizationStatus`	=	`temp`.`OrganizationStatus`,
							`flat`.`OrganizationType` 	=	`temp`.`OrganizationType` ,
							`flat`.`TPOverallStatus` 	=	`temp`.`TPOverallStatus` ,
							`flat`.`TPCompletionDate`	=	`temp`.`TPCompletionDate`,
							`flat`.`TPIsCurrent`	=	`temp`.`TPIsCurrent`,
							`flat`.`TPCertPath`	=	`temp`.`TPCertPath`,
							`flat`.`TPCertStatus`	=	`temp`.`TPCertStatus`,
						        
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
					DISTINCT `menr`.`id`	AS `TPEnrollmentID`	
		 		FROM slt_program prm 
						INNER JOIN `slt_master_enrollment` `menr` ON  `menr`.`program_id` = `prm`.`id`
						INNER JOIN `slt_person` `per` ON `per`.`id` = `menr`.`user_id`
						LEFT JOIN `slt_organization` `org` ON `org`.`id` = `per`.`org_id`  
						LEFT JOIN `slt_person_jobrole_mapping` `lpjm1` ON `lpjm1`.`user_id` = `per`.`id`";
			
			If ($action == 'delete') {
				$query .= "where ((prm.`status` = 'lrn_lpn_sts_del' and prm.updated_on > '".$this->delSync_on ."')
							OR (per.updated_on > '".$this->delSync_on ."'))";
			}
			
			return $query;
			
		} catch(Exception $ex){
			throw new Exception("Error in reportCollectIds ".$ex->getMessage());
		}	
	}
} 
	
	
