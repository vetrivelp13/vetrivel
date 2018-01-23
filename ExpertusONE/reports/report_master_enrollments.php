<?php
include_once $_SERVER['DOCUMENT_ROOT'] .  '/reports/ReportSyncUp.php';

class report_master_enrollments extends ReportSyncUp {
	
	
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
              `EnrollmentID` int(11) NOT NULL DEFAULT '0',
              `TPEnrollmentID` int(11) NOT NULL DEFAULT '0',
              `TrainingPlanTitle` varchar(255) DEFAULT NULL,
              `TrainingPlanCode` varchar(100) NOT NULL,
              `TrainingPlanDescription` longtext,
              `UserID` int(11) NOT NULL,
              `TPOverallStatus` longtext,
              `TPEnrollmentDate` datetime DEFAULT NULL,
              `TPCompletionDate` datetime DEFAULT NULL,
              `TPScore` float(11,2) NOT NULL DEFAULT '0.00',
              `TrainingPlanID` int(11) DEFAULT NULL,
              `TrainingPlanObjectName` longtext,
              `TPEnrollmentIsMandatory` varchar(10) DEFAULT NULL,
              `TPPercentageComplete` int(11) DEFAULT NULL,
              `TPEnrollmentCreatedBy` varchar(100) DEFAULT NULL,
              `TPEnrollmentCreatedOn` datetime DEFAULT NULL,
              `TPContentStatus` varchar(20) DEFAULT NULL,
              `CourseID` int(11) DEFAULT NULL,
              `ClassID` int(11) DEFAULT NULL,
              `TPStatus` longtext,
              `OrderId` int(11) DEFAULT NULL,
              `TPEnrollmentStatus` longtext,
              `TPCompletionStatus` longtext,
              `FullName` varchar(255) DEFAULT NULL,
              `UserName` varchar(255) DEFAULT NULL,
              `FirstName` varchar(255) DEFAULT NULL,
              `LastName` varchar(255) DEFAULT NULL,
              `City` varchar(255) DEFAULT NULL,
              `Email` varchar(255) DEFAULT NULL,
              `UserStatus` longtext,
              `OrganizationName` longtext,
              `ManagerName` varchar(255) DEFAULT NULL,
              `ManagerFullName` varchar(255) DEFAULT NULL,
              `TPIsRequired` varchar(3) DEFAULT NULL,
              `TPIsCurrent` char(1) DEFAULT 'Y',
              `TPCertPath` int(5) DEFAULT '0',
              `TPCertStatus` varchar(11) NOT NULL DEFAULT '',
              `TPWaived` varchar(3) NOT NULL DEFAULT '',
              `TPEnrollmentCancelDate` datetime DEFAULT NULL,
              `TPEnrollmentUpdatedOn` datetime DEFAULT NULL,
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
						select 
						concat_ws('-',`menr`.`id`,`enr`.`id`) as `RUID`,
						`enr`.`id` AS `EnrollmentID`,        
						`menr`.`id` AS `TPEnrollmentID`,
				        `prm`.`title` AS `TrainingPlanTitle`,
				        `prm`.`code` AS `TrainingPlanCode`,
				        `prm`.`short_desc` AS `TrainingPlanDescription`,
				        `menr`.`user_id` AS `UserID`,
				        `comstatus`.`name` AS `TPOverallStatus`,
				        `menr`.`reg_date` AS `TPEnrollmentDate`,
				        `menr`.`comp_date` AS `TPCompletionDate`,
				        `menr`.`score` AS `TPScore`,
				        `menr`.`program_id` AS `TrainingPlanID`,
				        `prgdel`.`name` AS `TrainingPlanObjectName`,
				        `menr`.`mandatory` AS `TPEnrollmentIsMandatory`,
				        `menr`.`percentage_complete` AS `TPPercentageComplete`,
				        `menr`.`created_by` AS `TPEnrollmentCreatedBy`,
				        `menr`.`created_on` AS `TPEnrollmentCreatedOn`,
				        `menr`.`content_status` AS `TPContentStatus`,
				        `enr`.`course_id` AS `CourseID`,
						`enr`.`class_id` AS `ClassID`,
				        `prgsts`.`name` AS `TPStatus`,
				        `menr`.`order_id` AS `OrderId`,
				        `regst`.`name` AS `TPEnrollmentStatus`,
				        `compst`.`name` AS `TPCompletionStatus`,
				        `per`.`full_name` AS `FullName`,
				        `per`.`user_name` AS `UserName`,
				        `per`.`first_name` AS `FirstName`,
				        `per`.`last_name` AS `LastName`,
				        `per`.`city` AS `City`,
				        `per`.`email` AS `Email`,
						`persts`.`code` AS `UserStatus`,
				        `org`.`name` AS `OrganizationName`,
				        `perm`.`user_name` AS `ManagerName`,
				        `perm`.`full_name` AS `ManagerFullName`,
				        `mcm`.`is_required` AS `TPIsRequired`,
				        `menr`.`is_current` AS `TPIsCurrent`,
				        `menr`.`recertify_path` AS `TPCertPath`,
				        if((`menr`.`recertify_path` > 1),'Recertified','Certified') AS `TPCertStatus`,
				        if(isnull(`exmp`.`id`),'No',if(((`exmp`.`is_mandatory` = '1') and (`exmp`.`exempted_status` = 1)), 'Yes','No')) AS `TPWaived`,
				        `menr`.`cancel_date` AS `TPEnrollmentCancelDate`,
				        `menr`.`updated_on` AS `TPEnrollmentUpdatedOn`,
			             NULL
			        	from `slt_program` `prm` 
                        join `slt_master_enrollment` `menr` ON (`menr`.`program_id` = `prm`.`id`)
                        left join `slt_enrollment_exempted` `exmp` ON (`exmp`.`id` = (select `slt_enrollment_exempted`.`id` from `slt_enrollment_exempted` where ((`slt_enrollment_exempted`.`enrollment_id` = `menr`.`id`) and (`slt_enrollment_exempted`.`enroll_type` = 'tp')) order by `slt_enrollment_exempted`.`id` desc limit 1))
                        join `slt_enrollment` `enr` ON ((`enr`.`master_enrollment_id` = `menr`.`id`) and (`menr`.`user_id` = `enr`.`user_id`))
                         join `slt_module` `mdl` ON (`mdl`.`program_id` = `prm` .id  and `mdl`.`sequence` = `menr`.`recertify_path`)
						join `slt_module_crs_mapping` `mcm` ON ((`mcm`.`program_id` = `prm`.`id`) and (`mcm`.`course_id` = `enr`.`course_id`) and `mdl`.`id` = `mcm`.`module_id`)
                        join `slt_person` `per` ON (`per`.`id` = `menr`.`user_id`)
                        left join `slt_person` `perm` ON (`perm`.`id` = `per`.`manager_id`)
                        left join `slt_organization` `org` ON (`org`.`id` = `per`.`org_id`)
                        join `slt_profile_list_items` `comstatus` ON ((`comstatus`.`code` = `menr`.`overall_status`) and (`comstatus`.`lang_code` = 'cre_sys_lng_eng'))
                        join `slt_profile_list_items` `regst` ON ((`regst`.`code` = `enr`.`reg_status`) and (`regst`.`lang_code` = 'cre_sys_lng_eng'))
                        left join `slt_profile_list_items` `compst` ON ((`compst`.`code` = `enr`.`comp_status`) and (`compst`.`lang_code` = 'cre_sys_lng_eng'))
                        join `slt_profile_list_items` `prgdel` ON (`prgdel`.`code` = `prm`.`object_type`)
                        join `slt_profile_list_items` `prgsts` ON (`prgsts`.`code` = `prm`.`status`)
	                    left join `slt_profile_list_items` `persts` ON (((`persts`.`code` = `per`.`status`) and (`persts`.`parent_id` = 425)))
					
			        where ((prm.updated_on > '".  $this->updSync_on . "') OR (`menr`.updated_on > '" .$this->updSync_on . "') OR (`enr`.updated_on > '" .$this->updSync_on . "')
			        		OR (`per`.updated_on > '".  $this->updSync_on . "') OR (`org`.updated_on > '".  $this->updSync_on . "') OR (`perm`.updated_on > '".  $this->updSync_on . "'))
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
						`RUID`,
                        `EnrollmentID`,
                        `TPEnrollmentID`,
                        `TrainingPlanTitle`,
                        `TrainingPlanCode`,
                        `TrainingPlanDescription`,
                        `UserID`,
                        `TPOverallStatus`,
                        `TPEnrollmentDate`,
                        `TPCompletionDate`,
                        `TPScore`,
                        `TrainingPlanID`,
                        `TrainingPlanObjectName`,
                        `TPEnrollmentIsMandatory`,
                        `TPPercentageComplete`,
                        `TPEnrollmentCreatedBy`,
                        `TPEnrollmentCreatedOn`,
                        `TPContentStatus`,
                        `CourseID`,
                        `ClassID`,
                        `TPStatus`,
                        `OrderId`,
                        `TPEnrollmentStatus`,
                        `TPCompletionStatus`,
                        `FullName`,
                        `UserName`,
                        `FirstName`,
                        `LastName`,
                        `City`,
                        `Email`,
                        `UserStatus`,
                        `OrganizationName`,
                        `ManagerName`,
                        `ManagerFullName`,
                        `TPIsRequired`,
                        `TPIsCurrent`,
                        `TPCertPath`,
                        `TPCertStatus`,
                        `TPWaived`,
                        `TPEnrollmentCancelDate`,
                        `TPEnrollmentUpdatedOn`
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
	                        
                            `flat`.`EnrollmentID`			   = `temp`.`EnrollmentID`,
                            `flat`.`TPEnrollmentID`		       = `temp`.`TPEnrollmentID`,
                            `flat`.`TrainingPlanTitle`		   = `temp`.`TrainingPlanTitle`,
                            `flat`.`TrainingPlanCode`		   = `temp`.`TrainingPlanCode`,
                            `flat`.`TrainingPlanDescription`   = `temp`.`TrainingPlanDescription`,
                            `flat`.`UserID`			           = `temp`.`UserID` ,
                            `flat`.`TPOverallStatus`		   = `temp`.`TPOverallStatus`,
                            `flat`.`TPEnrollmentDate`		   = `temp`.`TPEnrollmentDate`,
                            `flat`.`TPCompletionDate`		   = `temp`.`TPCompletionDate`,
                            `flat`.`TPScore` 			       = `temp`.`TPScore`,
                            `flat`.`TrainingPlanID`		       = `temp`.`TrainingPlanID`,
                            `flat`.`TrainingPlanObjectName`	   = `temp`.`TrainingPlanObjectName`,
                            `flat`.`TPEnrollmentIsMandatory`   = `temp`.`TPEnrollmentIsMandatory`,
                            `flat`.`TPPercentageComplete`	   = `temp`.`TPPercentageComplete`,
                            `flat`.`TPEnrollmentCreatedBy`	   = `temp`.`TPEnrollmentCreatedBy`,
                            `flat`.`TPEnrollmentCreatedOn`	   = `temp`.`TPEnrollmentCreatedOn`,
                            `flat`.`TPContentStatus`		   = `temp`.`TPContentStatus`,
                            `flat`.`CourseID`			       = `temp`.`CourseID`,
                            `flat`.`ClassID`			       = `temp`.`ClassID`,
                            `flat`.`TPStatus`			       = `temp`.`TPStatus`,
                            `flat`.`OrderId`			       = `temp`.`OrderId`,
                            `flat`.`TPEnrollmentStatus`		   = `temp`.`TPEnrollmentStatus`,
                            `flat`.`TPCompletionStatus`		   = `temp`.`TPCompletionStatus`,
                            `flat`.`FullName`			       = `temp`.`FullName`,
                            `flat`.`UserName`			       = `temp`.`UserName`,
                            `flat`.`FirstName`			       = `temp`.`FirstName`,
                            `flat`.`LastName`			       = `temp`.`LastName`,
                            `flat`.`City`				       = `temp`.`City`,
                            `flat`.`Email`			           = `temp`.`Email`,
                            `flat`.`UserStatus`			       = `temp`.`UserStatus`,
                            `flat`.`OrganizationName`		   = `temp`.`OrganizationName`,
                            `flat`.`ManagerName`			   = `temp`.`ManagerName`,
                            `flat`.`ManagerFullName`		   = `temp`.`ManagerFullName`,
                            `flat`.`TPIsRequired`			   = `temp`.`TPIsRequired`,
                            `flat`.`TPIsCurrent`			   = `temp`.`TPIsCurrent`,
                            `flat`.`TPCertPath`			       = `temp`.`TPCertPath`,
                            `flat`.`TPCertStatus`			   = `temp`.`TPCertStatus`,
                            `flat`.`TPWaived`			       = `temp`.`TPWaived`,
                            `flat`.`TPEnrollmentCancelDate`	   = `temp`.`TPEnrollmentCancelDate`,
                            `flat`.`TPEnrollmentUpdatedOn`	   = `temp`.`TPEnrollmentUpdatedOn`,    
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
			$query = "select 
						concat_ws('-',`menr`.`id`,`enr`.`id`) as `RUID` 
						from
						`slt_program` `prm` 
						join `slt_master_enrollment` `menr` ON (`menr`.`program_id` = `prm`.`id`)
						join `slt_enrollment` `enr` ON  (`menr`.`id` = `enr`.`master_enrollment_id`)";
			
			If ($action == 'delete') {
				$query .= " where (`prm`.`updated_on` > '".$this->delSync_on ."' AND  `prm`.`status` = 'lrn_lpn_sts_del')";
			}
			
			return $query;
			
		} catch(Exception $ex){
			throw new Exception("Error in reportCollectIds ".$ex->getMessage());
		}	
	}
} 
	
	
