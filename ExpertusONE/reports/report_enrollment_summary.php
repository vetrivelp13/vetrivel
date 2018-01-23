<?php
/**
 * Comments/Observations which are specific to this table
 * Following are the table where hard delete of records happening and are used in this flat table join
 * 		slt_course_class_session
 * 		slt_master_enrollment
 * 		slt_enrollment
 */
include_once $_SERVER['DOCUMENT_ROOT'] .  '/reports/ReportSyncUp.php';

class report_enrollment_summary extends ReportSyncUp {
	
	
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
			expDebug::dPrint("reportDelete delSubQuery " . print_r($delSubQuery, 1), 4);
			$delQuery = "DELETE rep FROM " . $this->report_table . " rep JOIN (" . $delSubQuery . ") as deleted ON rep.RUID = deleted.RUID";
			expDebug::dPrint("reportDelete delQuery " . print_r($delQuery, 1), 4);
			
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
			$dropTable = "DROP TABLE IF EXISTS " . $this->temp_table_name . ";";
			$this->db->callQuery($dropTable);
			
			// $createTable = "CREATE TABLE " . $this->temp_table_name . " LIKE " . $this->report_table . ";";
			$createTable = "CREATE TABLE ".$this->temp_table_name ." (
			         `RUID` varchar(255),
                	  `EnrollmentID` int(11) DEFAULT NULL,
                	  `ProgramID` int(11) DEFAULT NULL,
                	  `SessionID` int(11) DEFAULT NULL,
                	  `OrganizationID` int(11) DEFAULT NULL,
                	  `MasterUserID` int(11) DEFAULT NULL, 
                	  `MasterEnrollmentID` int(11) DEFAULT NULL, 
                	  `ClassTitle` longtext,
                	  `ClassCode` varchar(255) NOT NULL,
                	  `ProgramTitle` longtext,
                	  `ProgramCode` varchar(255) DEFAULT NULL,
                	  `ProgramStatus` varchar(20) DEFAULT NULL,
                	  `PersonUserID` int(11) DEFAULT NULL,
                	  `UserID` int(11) DEFAULT NULL,
                	  `EnrollmentDate` datetime DEFAULT NULL,
                	  `CompletionDate` datetime DEFAULT NULL,
                	  `FullName` varchar(255) DEFAULT NULL,
                	  `UserName` varchar(255) DEFAULT NULL,
                	  `Firstname` varchar(255) DEFAULT NULL,
                	  `Lastname` varchar(255) DEFAULT NULL,
                	  `Email` varchar(255) DEFAULT NULL,
                	  `City` varchar(255) DEFAULT NULL,
                	  `OrganizationName` longtext,
                	  `Address1` varchar(255) DEFAULT NULL,
                	  `State` varchar(255) DEFAULT NULL,
                	  `Country` varchar(255) DEFAULT NULL,
                	  `ZipCode` varchar(50) DEFAULT NULL,
                	  `ContactNumber` varchar(50) DEFAULT NULL,
                	  `EnrollmentStatusCode` varchar(100) DEFAULT NULL,
                	  `CompletionStatusCode` varchar(100) DEFAULT NULL,
                	  `EnrollmentStatus` longtext,
                	  `CompletionStatus` longtext,
                	  `EnrollmentScore` float(11,2) DEFAULT NULL,
                	  `MasterEnrollmentScore` float(11,2) DEFAULT NULL,
                	  `ClassStatus` longtext,
                	  `ClassDeliveryType` longtext,
                	  `ClassDeliveryTypeCode` varchar(255) DEFAULT NULL,
                	  `ProgramObjectType` longtext,
                	  `CourseID` int(11) DEFAULT NULL,
                	  `ClassID` int(11) DEFAULT NULL,
                	  `TPEnrollmentID` int(11) DEFAULT NULL,
                	  `EnrollmentIsMandatory` varchar(10) DEFAULT NULL,
                	  `EnrollmentIsCompliance` int(1) DEFAULT NULL,
                	  `MasterEnrollmentIsMandatory` varchar(10) DEFAULT NULL,
                	  `EnrollmentCreatedBy` text,
                	  `WaitlistPriority` int(5) DEFAULT NULL,
                	  `WaitlistFlag` longtext,
                	  `EnrollmentContentStatus` varchar(20) DEFAULT NULL,
                	  `MasterEnrollmentContentStatus` varchar(20) DEFAULT NULL,
                	  `OrderID` int(11) DEFAULT NULL,
                	  `EnrollmentRecertifyPath` int(5) DEFAULT '0',
                	  `EnrollmentPreSurAssStatus` varchar(25) DEFAULT NULL,
                	  `EnrollmentPreSurAssScore` float(11,2) DEFAULT NULL,
                	  `EnrollmentComplianceStatus` int(1) DEFAULT '0',
                	  `EnrollmentCreatedOn` datetime DEFAULT NULL,
                	  `EnrollmentUpdatedBy` text,
                	  `EnrollmentUpdatedOn` datetime DEFAULT NULL,
                	  `EnrollmentCompletedBy` text,
                	  `EnrollmentCompletedOn` datetime DEFAULT NULL,
                	  `ManagerName` varchar(255) DEFAULT NULL,
                	  `ManagerFullName` varchar(255) DEFAULT NULL,
                	  `ManagerEmail` varchar(255) DEFAULT NULL,
                	  `CourseTitle` longtext,
                	  `CourseCode` varchar(255) NOT NULL,
                	  `SessionStartDate` datetime DEFAULT NULL,
                	  `TimeSpent` varchar(50) DEFAULT NULL,
                	  `AttendanceSummaryId` int(11) DEFAULT NULL,
                	  `TotalAttempts` int(11) DEFAULT NULL,
                	  `LocationName` longtext,
                	  `ClassLanguage` longtext,
                	  `CourseStatus` longtext,
                	  `UserStatus` longtext,
                	  `RegistrationStatus` longtext,
                	  `UserHireDate` datetime DEFAULT NULL,
                	  `MasterEnrollmentRegistrationDate` datetime DEFAULT NULL,
                	  `MasterEnrollmentCompletionDate` datetime DEFAULT NULL,
                	  `MasterEnrollmentCompletionStatus` longtext,
                	  `Waived` varchar(3) NOT NULL DEFAULT '',
                	  `MandatoryWaived` varchar(3) NOT NULL DEFAULT '',
                	  `PaymentType` varchar(255) DEFAULT NULL,
                	  `OrderStatus` longtext,
                	  `TPIsCurrent` char(1) DEFAULT 'Y',
                	  `TPCertPath` int(5) DEFAULT '0',
                	  `TPCertStatus` varchar(11) NOT NULL DEFAULT '',
                	  `CompleteByDate` varchar(100) DEFAULT NULL,
                	  `ExpiryDate` varchar(100) DEFAULT NULL,
		          	`operation` varchar(10) NULL DEFAULT NULL,
			          PRIMARY KEY (`RUID`),
			             key sli_op(operation)
			)  ENGINE=InnoDB DEFAULT CHARSET=utf8;";
			$this->db->callQuery($createTable);
			
		//	$alterTable = "ALTER TABLE " . $this->temp_table_name . " ADD COLUMN `operation` varchar(10) NULL DEFAULT NULL;";
		//	$this->db->callQuery($alterTable);
		} catch(Exception $ex) {
			throw new Exception("Error in createTempTable " . $ex->getMessage());
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
			/* new records need to be inserted to report_enrollment_summary table when following tables have new records
			 * slt_enrollment
			 * slt_course_class_session
			 * slt_attendance_summary
			 * slt_attendance_details
			 *  
			 */

			$tempDataQuery = "insert into " . $this->temp_table_name . "
					SELECT
						concat_ws('-',`enr`.`id`, IFNULL(`ses`.`id`, 0), IFNULL(`summ`.`id`, 0), IFNULL(`detail`.`id`, 0)) as `RUID`,
						`enr`.`id` AS `EnrollmentID`,
						`prg`.`id` AS `ProgramID`,
						`ses`.`id` AS `SessionID`,
						`org`.`id` AS `OrganizationID`,
						`menr`.`user_id` As `MasterUserID`,
						`menr`.`id` As `MasterEnrollmentID`,
						`cls`.`title` AS `ClassTitle`,
						`cls`.`code` AS `ClassCode`,
						`prg`.`title`AS `ProgramTitle`,
						`prg`.`code` AS `ProgramCode`,
						`prg`.`status` As `ProgramStatus`,
						`usr`.`id` As `PersonUserID`,
						`enr`.`user_id` AS `UserID`,
						`enr`.`reg_date` AS `EnrollmentDate`,
						`enr`.`comp_date` AS `CompletionDate`,
						`usr`.`full_name` AS `FullName`,
						`usr`.`user_name` AS `UserName`,
						`usr`.`first_name` AS `Firstname`,
						`usr`.`last_name` AS `Lastname`,
						`usr`.`email` AS `Email`,
						`usr`.`city` AS `City`,
						`org`.`name` AS `OrganizationName`,
						`usr`.`addr1` AS `Address1`,
						`sta`.`state_name` AS `State`,
						`country`.`country_name` AS `Country`,
						`usr`.`zip` AS `ZipCode`,
						(case
						when (`usr`.`phone_no` <> '') then `usr`.`phone_no`
						else `usr`.`mobile_no`
						end) AS `ContactNumber`,
						`enr`.`reg_status` AS `EnrollmentStatusCode`,
						`enr`.`comp_status` AS `CompletionStatusCode`,
						`regst`.`name` AS `EnrollmentStatus`,
						`compst`.`name` AS `CompletionStatus`,
						`enr`.`score` AS `EnrollmentScore`,
						`menr`.`score` AS `MasterEnrollmentScore`,
						`clssts`.`name` AS `ClassStatus`,
						`delty`.`name` AS `ClassDeliveryType`,
						`cls`.`delivery_type` AS `ClassDeliveryTypeCode`,
						`prgdel`.`name` AS `ProgramObjectType`,
						`enr`.`course_id` AS `CourseID`,
						`enr`.`class_id` AS `ClassID`,
						`enr`.`master_enrollment_id` AS `TPEnrollmentID`,
						`enr`.`mandatory` AS `EnrollmentIsMandatory`,
						`enr`.`is_compliance` AS `EnrollmentIsCompliance`,
						`menr`.`mandatory` As `MasterEnrollmentIsMandatory`,
						`enr`.`created_by` AS `EnrollmentCreatedBy`,
						`enr`.`waitlist_priority` AS `WaitlistPriority`,
						`waitlist`.`name` AS `WaitlistFlag`,
						`enr`.`content_status` AS `EnrollmentContentStatus`,
						`menr`.`content_status` AS `MasterEnrollmentContentStatus`,
						`enr`.`order_id` AS `OrderID`,
						`enr`.`recertify_path` AS `EnrollmentRecertifyPath`,
						`enr`.`pre_status` AS `EnrollmentPreSurAssStatus`,
						`enr`.`pre_score` AS `EnrollmentPreSurAssScore`,
						`enr`.`cmpl_expired` AS `EnrollmentComplianceStatus`,
						`enr`.`created_on` AS `EnrollmentCreatedOn`,
						`enr`.`updated_by` AS `EnrollmentUpdatedBy`,
						`enr`.`updated_on` AS `EnrollmentUpdatedOn`,
						`enr`.`comp_by` AS `EnrollmentCompletedBy`,
						`enr`.`comp_on` AS `EnrollmentCompletedOn`,
						`perm`.`user_name` AS `ManagerName`,
						`perm`.`full_name` AS `ManagerFullName`,
						`perm`.`email` AS `ManagerEmail`,
						`crs`.`title` AS `CourseTitle`,
						`crs`.`code` AS `CourseCode`,
						`ses`.`start_date` AS `SessionStartDate`,
						`detail`.`time_spend` AS `TimeSpent`,
						`summ`.`id` AS `AttendanceSummaryId`,
						`summ`.`total_attempts` AS `TotalAttempts`,
						`loc`.`name` AS `LocationName`,
						`clslng`.`name` AS `ClassLanguage`,
						`crssts`.`name` AS `CourseStatus`,
						`lrnsts`.`name` AS `UserStatus`,
						`enrsts`.`name` AS `RegistrationStatus`,
						`usr`.`hire_date` AS `UserHireDate`,
						`menr`.`reg_date` AS `MasterEnrollmentRegistrationDate`,
						`menr`.`comp_date` AS `MasterEnrollmentCompletionDate`,
						`comstatus`.`name` AS `MasterEnrollmentCompletionStatus`,
						if(isnull(`exmp`.`id`),
						'No',
						if(((`exmp`.`is_compliance` = 1)
						and (`exmp`.`exempted_status` = 1)),
						'Yes',
						'No')) AS `Waived`,
						if(isnull(`exmp`.`id`),
						'No',
						if(((`exmp`.`is_mandatory` = 'Y')
						and (`exmp`.`exempted_status` = 1)),
						'Yes',
						'No')) AS `MandatoryWaived`,
						`odr`.`order_type` AS `PaymentType`,
						`paystatus`.`name` AS `OrderStatus`,
						`menr`.`is_current` AS `TPIsCurrent`,
						`menr`.`recertify_path` AS `TPCertPath`,
						if((`menr`.`recertify_path` > 1),
						'Recertified',
						'Certified') AS `TPCertStatus`,
						if(isnull(`crs`.`complete_date`),
						date_format((`enr`.`reg_date`  + interval `crs`.`complete_days` day),
						'%d %M %Y'),
						date_format(`crs`.`complete_date`, '%d %M %Y')) AS `CompleteByDate`,
						
						if(isnull(`crs`.`validity_date`),
						date_format((`enr`.`comp_date` + interval `crs`.`validity_days` day),
						'%d %M %Y'),
						date_format(`crs`.`validity_date`, '%d %M %Y')) AS `ExpiryDate`,
						NULL
					from `slt_enrollment` `enr`
						left join `slt_person` `usr` ON (`usr`.`id` = `enr`.`user_id`)   
						left join `slt_course_class` `cls` ON ((`enr`.`class_id` = `cls`.`id`))
						left join `slt_course_template` `crs` ON ((`crs`.`id` = `cls`.`course_id`) and (`crs`.`id` = `enr`.`course_id`))
						left join `slt_enrollment_exempted` `exmp` ON ((`exmp`.`id` = (select 
						`slt_enrollment_exempted`.`id`
						from
						`slt_enrollment_exempted`
						where
						((`slt_enrollment_exempted`.`enrollment_id` = `enr`.`id`)
						and (`slt_enrollment_exempted`.`enroll_type` = 'class'))
						order by `slt_enrollment_exempted`.`id` desc limit 1)))
						left join `slt_organization` `org` ON ((`org`.`id` = `usr`.`org_id`) and (`usr`.`status` <> 'cre_org_sts_del'))
						left join `slt_person` `perm` ON (`perm`.`id` = `usr`.`manager_id`)
						left join `slt_course_class_session` `ses` ON (`ses`.`class_id` = `cls`.`id`)
						left join `slt_location` `loc` ON (`loc`.`id` = `ses`.`location_id`)
						left join `slt_country` `country` ON (`country`.`country_code` = `usr`.`country`)
						left join `slt_state` `sta` ON ((`sta`.`state_code` = `usr`.`state`)
							and (`sta`.`country_code` = `country`.`country_code`))
						left join `slt_attendance_summary` `summ` ON (`summ`.`enrollment_id` = `enr`.`id`)
						left join `slt_attendance_details` `detail` ON (`detail`.`enrollment_id` = `enr`.`id`)	
						left join `slt_order` `odr` ON (`odr`.`id` = `enr`.`order_id`)
						left join `slt_profile_list_items` `paystatus` ON ((`paystatus`.`code` = `odr`.`order_status`)
							and (`paystatus`.`lang_code` = 'cre_sys_lng_eng'))	
						left join `slt_profile_list_items` `regst` ON ((`regst`.`code` = `enr`.`reg_status`)
							and (`regst`.`lang_code` = 'cre_sys_lng_eng')
							and (`regst`.`parent_id` = 358))
						left join `slt_profile_list_items` `compst` ON ((`compst`.`code` = `enr`.`comp_status`)
							and (`compst`.`lang_code` = 'cre_sys_lng_eng')
							and (`compst`.`parent_id` = 357))
						left join `slt_profile_list_items` `waitlist` ON ((`waitlist`.`code` = `enr`.`waitlist_flag`)
							and (`waitlist`.`parent_id` = 358))
						left join `slt_profile_list_items` `delty` ON ((`delty`.`code` = `cls`.`delivery_type`)
							and (`delty`.`parent_id` = 331))
						left join `slt_profile_list_items` `clssts` ON ((`clssts`.`code` = `cls`.`status`)
							and (`clssts`.`parent_id` = 325))
						left join `slt_profile_list_items` `clslng` ON ((`clslng`.`code` = `cls`.`lang_code`)
							and (`clslng`.`parent_id` = 116))
						left join `slt_profile_list_items` `crssts` ON ((`crssts`.`code` = `crs`.`status`)
							and (`crssts`.`parent_id` = 356))
						left join `slt_profile_list_items` `lrnsts` ON ((`lrnsts`.`code` = `usr`.`status`)
							and (`lrnsts`.`parent_id` = 425))
						left join `slt_profile_list_items` `enrsts` ON ((`enrsts`.`code` = `enr`.`reg_status`)
							and (`enrsts`.`parent_id` = 358))
						left join `slt_master_enrollment` `menr` ON ((`enr`.`master_enrollment_id` = `menr`.`id`)
							and (`menr`.`overall_status` not in ('lrn_tpm_ovr_rsc' , 'lrn_tpm_ovr_rsv')))
						left join `slt_program` `prg` ON (`prg`.`id` = `menr`.`program_id`)
						left join `slt_profile_list_items` `prgdel` ON (`prgdel`.`code` = `prg`.`object_type`)
						left join `slt_profile_list_items` `comstatus` ON ((`comstatus`.`code` = `menr`.`overall_status`)
							and (`comstatus`.`lang_code` = 'cre_sys_lng_eng')) ";
					if($this->updSync_on != null) {
						$tempDataQuery .= "where (
							(enr.updated_on > '". $this->updSync_on . "')
							OR (ses.updated_on > '" . $this->updSync_on . "')
							OR (summ.updated_on > '" . $this->updSync_on . "')
							OR (detail.updated_on > '" . $this->updSync_on . "')
							OR (usr.updated_on > '" . $this->updSync_on . "')
							OR (cls.updated_on > '" . $this->updSync_on . "')
							OR (crs.updated_on > '" . $this->updSync_on . "')
							OR (exmp.updated_on > '" . $this->updSync_on . "')
							OR (org.updated_on > '" . $this->updSync_on . "')
							OR (perm.updated_on > '" . $this->updSync_on . "')
							OR (loc.updated_on > '" . $this->updSync_on . "')
							OR (country.updated_on > '" . $this->updSync_on . "')
							OR (sta.updated_on > '" . $this->updSync_on . "')
							OR (prg.updated_on > '" . $this->updSync_on . "')
							OR (menr.updated_on > '" . $this->updSync_on . "')
						)";
					}
			
			expDebug::dPrint('tempTableDataPopulate query: ' . $tempDataQuery, 4);
			$this->db->callQuery($tempDataQuery);
		} catch(Exception $ex) {
			throw new Exception("Error in tempTableDataPopulate " . $ex->getMessage());
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
						`EnrollmentID` ,
						`ProgramID` ,
						`SessionID` ,
						`OrganizationID` ,
						`MasterUserID`,
						`MasterEnrollmentID`,
						`ClassTitle`,
						`ClassCode` ,
						`ProgramTitle`,
						`ProgramCode` ,
						`ProgramStatus`,
						`PersonUserID` ,
						`UserID` ,
						`EnrollmentDate`,
						`CompletionDate`,
						`FullName` ,
						`UserName` ,
						`Firstname` ,
						`Lastname` ,
						`Email` ,
						`City` ,
						`OrganizationName`,
						`Address1` ,
						`State` ,
						`Country` ,
						`ZipCode` ,
						`ContactNumber` ,
						`EnrollmentStatusCode` ,
						`CompletionStatusCode` ,
						`EnrollmentStatus`,
						`CompletionStatus`,
						`EnrollmentScore` ,
						`MasterEnrollmentScore` ,
						`ClassStatus`,
						`ClassDeliveryType`,
						`ClassDeliveryTypeCode` ,
						`ProgramObjectType`,
						`CourseID` ,
						`ClassID` ,
						`TPEnrollmentID` ,
						`EnrollmentIsMandatory`,
						`EnrollmentIsCompliance`,
						`MasterEnrollmentIsMandatory`,
						`EnrollmentCreatedBy`,
						`WaitlistPriority`,
						`WaitlistFlag`,
						`EnrollmentContentStatus`,
						`MasterEnrollmentContentStatus`,
						`OrderID` ,
						`EnrollmentRecertifyPath` ,
						`EnrollmentPreSurAssStatus`,
						`EnrollmentPreSurAssScore` ,
						`EnrollmentComplianceStatus`,
						`EnrollmentCreatedOn`,
						`EnrollmentUpdatedBy` ,
						`EnrollmentUpdatedOn`,
						`EnrollmentCompletedBy` ,
						`EnrollmentCompletedOn`,
						`ManagerName` ,
						`ManagerFullName` ,
						`ManagerEmail` ,
						`CourseTitle`,
						`CourseCode` ,
						`SessionStartDate`,
						`TimeSpent` ,
						`AttendanceSummaryId`,
						`TotalAttempts` ,
						`LocationName`,
						`ClassLanguage`,
						`CourseStatus`,
						`UserStatus`,
						`RegistrationStatus`,
						`UserHireDate`,
						`MasterEnrollmentRegistrationDate`,
						`MasterEnrollmentCompletionDate`,
						`MasterEnrollmentCompletionStatus`,
						`Waived`,
						`MandatoryWaived` ,
						`PaymentType` ,
						`OrderStatus`,
						`TPIsCurrent`,
						`TPCertPath` ,
						`TPCertStatus`,
						`CompleteByDate`,
						`ExpiryDate`
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
						flat.ClassTitle = temp.ClassTitle,
						flat.ClassCode = temp.ClassCode,
						flat.ProgramTitle = temp.ProgramTitle,
						flat.ProgramCode = temp.ProgramCode,
						flat.ProgramStatus = temp.ProgramStatus,
						flat.EnrollmentDate = temp.EnrollmentDate,
						flat.CompletionDate = temp.CompletionDate,
						flat.FullName = temp.FullName,
						flat.UserName = temp.UserName,
						flat.Firstname = temp.Firstname,
						flat.Lastname = temp.Lastname,
						flat.Email = temp.Email,
						flat.City = temp.City,
						flat.OrganizationName = temp.OrganizationName,
						flat.Address1 = temp.Address1,
						flat.State = temp.State,
						flat.Country = temp.Country,
						flat.ZipCode = temp.ZipCode,
						flat.ContactNumber = temp.ContactNumber,
						flat.EnrollmentStatusCode = temp.EnrollmentStatusCode,
						flat.CompletionStatusCode = temp.CompletionStatusCode,
						flat.EnrollmentStatus = temp.EnrollmentStatus,
						flat.CompletionStatus = temp.CompletionStatus,
						flat.EnrollmentScore = temp.EnrollmentScore,
						flat.MasterEnrollmentScore = temp.MasterEnrollmentScore,
						flat.ClassStatus = temp.ClassStatus,
						flat.EnrollmentIsMandatory = temp.EnrollmentIsMandatory,
						flat.EnrollmentIsCompliance = temp.EnrollmentIsCompliance,
						flat.MasterEnrollmentIsMandatory = temp.MasterEnrollmentIsMandatory,
						flat.EnrollmentCreatedBy = temp.EnrollmentCreatedBy,
						flat.WaitlistPriority = temp.WaitlistPriority,
						flat.WaitlistFlag = temp.WaitlistFlag,
						flat.EnrollmentContentStatus = temp.EnrollmentContentStatus,
						flat.MasterEnrollmentContentStatus = temp.MasterEnrollmentContentStatus,
						flat.EnrollmentRecertifyPath = temp.EnrollmentRecertifyPath,
						flat.EnrollmentPreSurAssStatus = temp.EnrollmentPreSurAssStatus,
						flat.EnrollmentPreSurAssScore = temp.EnrollmentPreSurAssScore,
						flat.EnrollmentComplianceStatus = temp.EnrollmentComplianceStatus,
						flat.EnrollmentCreatedOn = temp.EnrollmentCreatedOn,
						flat.EnrollmentUpdatedBy = temp.EnrollmentUpdatedBy,
						flat.EnrollmentUpdatedOn = temp.EnrollmentUpdatedOn,
						flat.EnrollmentCompletedBy = temp.EnrollmentCompletedBy,
						flat.EnrollmentCompletedOn = temp.EnrollmentCompletedOn,
						flat.ManagerName = temp.ManagerName,
						flat.ManagerFullName = temp.ManagerFullName,
						flat.ManagerEmail = temp.ManagerEmail,
						flat.CourseTitle = temp.CourseTitle,
						flat.CourseCode = temp.CourseCode,
						flat.TimeSpent = temp.TimeSpent,
						flat.AttendanceSummaryId = temp.AttendanceSummaryId,
						flat.TotalAttempts = temp.TotalAttempts,
						flat.LocationName = temp.LocationName,
						flat.ClassLanguage = temp.ClassLanguage,
						flat.CourseStatus = temp.CourseStatus,
						flat.UserStatus = temp.UserStatus,
						flat.RegistrationStatus = temp.RegistrationStatus,
						flat.UserHireDate = temp.UserHireDate,
						flat.MasterEnrollmentRegistrationDate = temp.MasterEnrollmentRegistrationDate,
						flat.MasterEnrollmentCompletionDate = temp.MasterEnrollmentCompletionDate,
						flat.MasterEnrollmentCompletionStatus = temp.MasterEnrollmentCompletionStatus,
						flat.Waived = temp.Waived,
						flat.MandatoryWaived = temp.MandatoryWaived,
						flat.OrderStatus = temp.OrderStatus,
						flat.TPIsCurrent = temp.TPIsCurrent,
						flat.TPCertPath = temp.TPCertPath,
						flat.TPCertStatus = temp.TPCertStatus,
						flat.CompleteByDate = temp.CompleteByDate,
						flat.ExpiryDate = temp.ExpiryDate,
						temp.operation = 'update'";
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
			$query = "SELECT RUID from report_enrollment_summary repsum
					join report_deleted_logs del on del.entity_id = repsum.SessionID and del.table_name = 'slt_course_class_session'
					WHERE del.deleted_on > '" . $this->delSync_on . "'
						UNION
					SELECT RUID from report_enrollment_summary repsum
					join report_deleted_logs del on del.entity_id = repsum.EnrollmentID and del.table_name = 'slt_enrollment'
					WHERE del.deleted_on > '" . $this->delSync_on . "'
						UNION
					SELECT RUID from report_enrollment_summary repsum
					join report_deleted_logs del on del.entity_id = repsum.MasterEnrollmentID and del.table_name = 'slt_master_enrollment'
					WHERE del.deleted_on > '" . $this->delSync_on . "'
						UNION
					SELECT rep.RUID FROM report_enrollment_summary rep
					left join slt_attendance_summary summ on rep.UserId = summ.user_id AND rep.EnrollmentID = summ.enrollment_id AND summ.updated_on > '" . $this->delSync_on . "'
					where rep.AttendanceSummaryId is null and summ.id is not null";
			
			return $query;
		} catch(Exception $ex) {
			throw new Exception("Error in reportCollectIds " . $ex->getMessage());
		}
	}
}
	
	
