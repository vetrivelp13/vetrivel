<?php
include_once $_SERVER['DOCUMENT_ROOT'] .  '/reports/ReportSyncUp.php';

class report_admin_manager_class_enrollments extends ReportSyncUp {
	
	
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
            `ClassTitle` longtext,
            `ClassCode` varchar(255) NOT NULL,
            `ClassID` int(11) DEFAULT NULL,
            `Type` varchar(5) NOT NULL DEFAULT '',
            `ClassStatus` longtext,
            `UserID` int(11) DEFAULT NULL,
            `UserName` varchar(255) DEFAULT NULL,
            `FullName` varchar(255) DEFAULT NULL,
            `Email` varchar(255) DEFAULT NULL,
            `JobRoleMappingId` int(11) DEFAULT NULL,
            `JobRole` longtext,
            `JobTitle` longtext,
            `OrganizationName` longtext,
            `State` varchar(255) DEFAULT NULL,
            `City` varchar(255) DEFAULT NULL,
            `ManagerID` int(11) DEFAULT NULL,
            `ManagerName` varchar(255) DEFAULT NULL,
            `ManagerFullName` varchar(255) DEFAULT NULL,
            `ManagerEmail` varchar(255) DEFAULT NULL,
            `EnrollmentIsCompliance` int(1) DEFAULT NULL,
            `EnrollmentIsMandatory` varchar(10) DEFAULT NULL,
            `RegistrationDate` datetime DEFAULT NULL,
            `CourseID` int(11) DEFAULT NULL,
            `EnrollmentID` int(11) DEFAULT NULL,
            `CompletionDate` datetime DEFAULT NULL,
            `TPEnrollmentID` int(11) DEFAULT NULL,
            `EnrollmentScore` float(11,2) DEFAULT NULL,
            `EnrollmentContentStatus` varchar(20) DEFAULT NULL,
            `EnrollmentCreatedBy` text,
            `Waived` varchar(3) NOT NULL DEFAULT '',
            `MandatoryWaived` varchar(3) NOT NULL DEFAULT '',
            `CourseCompletionDate` datetime DEFAULT NULL,
            `CourseCompletionDays` int(5) DEFAULT NULL,
            `CourseTitle` longtext,
            `CourseValidityDate` datetime DEFAULT NULL,
            `CourseValidityDays` int(5) DEFAULT NULL,
            `CompletionStatus` longtext,
            `EnrollmentStatus` longtext,
            `ClassDeliveryType` longtext,
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
			$tempDataQuery = "insert into " . $this->temp_table_name . "
					SELECT
			            	concat_ws('-', `cls`.`id`, `per`.`id`, `perm`.`id`, `enr`.`course_id`, `enr`.`id`, IFNULL(`enr`.`master_enrollment_id`, 0), IFNULL(`pjt`.`id`, 0)) as `RUID`,
                            `cls`.`title`           AS      `ClassTitle`,
                            `cls`.`code`            AS      `ClassCode`,
                            `cls`.`id`              AS      `ClassID`,
                            'Class'                 AS      `Type`,
                            `clssts`.`name`         AS      `ClassStatus`,
                            `per`.`id`              AS      `UserID`,
                            `per`.`user_name`       AS      `UserName`,
                            `per`.`full_name`       AS      `FullName`,
                            `per`.`email`           AS      `Email`,
							`pjt`.`id` AS `JobRoleMappingId`,
                            `pjt`.`name`            AS      `JobRole`,
                            `plt`.`name`            AS      `JobTitle`,
                            `sorg`.`name`           AS      `OrganizationName`,
                            `st`.`state_name`       AS      `State`,
                            `per`.`city`                AS      `City`,
                            `perm`.`id`                 AS      `ManagerID`,
                            `perm`.`user_name`          AS      `ManagerName`,
                            `perm`.`full_name`          AS      `ManagerFullName`,
		                    `perm`.`email`              AS      `ManagerEmail`,
                            `enr`.`is_compliance`       AS      `EnrollmentIsCompliance`,
                            `enr`.`mandatory`           AS      `EnrollmentIsMandatory`,
                            `enr`.`reg_date`            AS      `RegistrationDate`,
                            `enr`.`course_id`           AS      `CourseID`,
                            `enr`.`id`                  AS      `EnrollmentID`,
                            `enr`.`comp_date`           AS      `CompletionDate`,
                            `enr`.`master_enrollment_id` AS `TPEnrollmentID`,
                            `enr`.`score` AS `EnrollmentScore`,
                            `enr`.`content_status` AS `EnrollmentContentStatus`,
                            `enr`.`created_by` AS `EnrollmentCreatedBy`,
                            if(isnull(`exmp`.`id`),'No', if(((`exmp`.`is_compliance` = 1) and (`exmp`.`exempted_status` = 1)), 'Yes', 'No')) AS `Waived`,
                            if(isnull(`exmp`.`id`),'No', if(((`exmp`.`is_mandatory` = 'Y') and (`exmp`.`exempted_status` = 1)), 'Yes','No')) AS `MandatoryWaived`,
                            `temp`.`complete_date` AS `CourseCompletionDate`,
                            `temp`.`complete_days` AS `CourseCompletionDays`,
                            `temp`.`title` AS `CourseTitle`,
                            `temp`.`validity_date` AS `CourseValidityDate`,
                            `temp`.`validity_days` AS `CourseValidityDays`,
                            `cmpstatus`.`name` AS `CompletionStatus`,
                            `enrstatus`.`name` AS `EnrollmentStatus`,
                            `enrdeltype`.`name` AS `ClassDeliveryType`,
                            NULL
			        from `slt_person` `perm`
                            join `slt_person_other_manager` `othr` ON (`othr`.`manager_id` = `perm`.`id`)
                            join `slt_person` `per` ON (`per`.`id` = `othr`.`user_id`)
	   	                    join `slt_enrollment` `enr` ON (`enr`.`user_id` = `per`.`id`)
                            left join `slt_enrollment_exempted` `exmp` ON (`exmp`.`id` = (select  `slt_enrollment_exempted`.`id` from `slt_enrollment_exempted` where ((`slt_enrollment_exempted`.`enrollment_id` = `enr`.`id`) and (`slt_enrollment_exempted`.`enroll_type` = 'class')) order by `slt_enrollment_exempted`.`id` desc limit 1))
                            join `slt_course_class` `cls` ON (`cls`.`id` = `enr`.`class_id`)
                            join `slt_course_template` `temp` ON (`temp`.`id` = `enr`.`course_id`)
                            join `slt_profile_list_items` `enrstatus` ON (`enr`.`reg_status` = `enrstatus`.`code`)
                            left join `slt_profile_list_items` `cmpstatus` ON (`enr`.`comp_status` = `cmpstatus`.`code`)
                            join `slt_profile_list_items` `enrdeltype` ON (`enrdeltype`.`code` = `cls`.`delivery_type`)
                            join `slt_profile_list_items` `clssts` ON (`clssts`.`code` = `cls`.`status`)
                            left join `slt_person_jobrole_mapping` `jrlmap` ON (`jrlmap`.`user_id` = `per`.`id`)
                            left join `slt_profile_list_items` `pjt` ON (`pjt`.`code` = `jrlmap`.`job_role`)
                            left join `slt_profile_list_items` `plt` ON (`plt`.`code` = `per`.`job_title`)
                            left join `slt_organization` `sorg` ON (`sorg`.`id` = `per`.`org_id`)
                            left join `slt_state` `st` ON ((`st`.`state_code` = `per`.`state`) and (`st`.`country_code` = `per`.`country`))";
			
			$tempDataQuery .= "where
                            ((`per`.`status` <> 'cre_usr_sts_del') and (`perm`.`status` <> 'cre_usr_sts_del') and (`perm`.`is_manager` = 'Y'))";
			if($this->updSync_on != null) {
				$tempDataQuery .= "AND (
			        			(perm.updated_on > '" . $this->updSync_on . "')
			        			OR (per.updated_on > '" . $this->updSync_on . "')
			        			OR (enr.updated_on > '" . $this->updSync_on . "')
			        			OR (cls.updated_on > '" . $this->updSync_on . "')
			        			OR (temp.updated_on > '" . $this->updSync_on . "')
			        			OR (sorg.updated_on > '" . $this->updSync_on . "')
			        			OR (st.updated_on > '" . $this->updSync_on . "')
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
						`RUID` ,
                        `ClassTitle` ,
                        `ClassCode` ,
                        `ClassID` ,
                        `Type` ,
                        `ClassStatus` ,
                        `UserID` ,
                        `UserName`  ,
                        `FullName`  ,
                        `Email`  ,
						JobRoleMappingId,
                        `JobRole` ,
                        `JobTitle` ,
                        `OrganizationName` ,
                        `State`  ,
                        `City`  ,
                        `ManagerID` ,
                        `ManagerName`  ,
                        `ManagerFullName`  ,
                        `ManagerEmail`  ,
                        `EnrollmentIsCompliance` ,
                        `EnrollmentIsMandatory` ,
                        `RegistrationDate` ,
                        `CourseID` ,
                        `EnrollmentID` ,
                        `CompletionDate` ,
                        `TPEnrollmentID` ,
                        `EnrollmentScore` ,
                        `EnrollmentContentStatus` ,
                        `EnrollmentCreatedBy`,
                        `Waived` ,
                        `MandatoryWaived` ,
                        `CourseCompletionDate` ,
                        `CourseCompletionDays`  ,
                        `CourseTitle` ,
                        `CourseValidityDate` ,
                        `CourseValidityDays`  ,
                        `CompletionStatus` ,
                        `EnrollmentStatus` ,
                        `ClassDeliveryType` 
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

	                           `flat`.`ClassTitle`         		= `temp`.`ClassTitle`,
	                           `flat`.`ClassCode`          		= `temp`.`ClassCode`,
	                           `flat`.`ClassID`		            = `temp`.`ClassID`,
	                           `flat`.`Type`	                = `temp`.`Type`,
	                           `flat`.`ClassStatus`		        = `temp`.`ClassStatus`,
	                           `flat`.`UserID`		            = `temp`.`UserID`,
	                           `flat`.`UserName`		        = `temp`.`UserName`,
                               `flat`.`FullName`	            = `temp`.`FullName`,
                               `flat`.`Email`		            = `temp`.`Email`,
								`flat`.`JobRoleMappingId`		= `temp`.`JobRoleMappingId`,
                               `flat`.`JobRole`		            = `temp`.`JobRole`,
                               `flat`.`JobTitle`		        = `temp`.`JobTitle`,
                               `flat`.`OrganizationName`	    = `temp`.`OrganizationName`,
                               `flat`.`State`	             	= `temp`.`State`,
                               `flat`.`City`  	         	    = `temp`.`City`,
                               `flat`.`ManagerID`		    	= `temp`.`ManagerID`,
                               `flat`.`ManagerName`		        = `temp`.`ManagerName`,
                               `flat`.`ManagerFullName`		    = `temp`.`ManagerFullName`,
                               `flat`.`ManagerEmail`	       	= `temp`.`ManagerEmail`,
                               `flat`.`EnrollmentIsCompliance`	= `temp`.`EnrollmentIsCompliance`,
                               `flat`.`EnrollmentIsMandatory`	= `temp`.`EnrollmentIsMandatory`,
                               `flat`.`RegistrationDate`	    = `temp`.`RegistrationDate`,
                               `flat`.`CourseID`	            = `temp`.`CourseID`,
                               `flat`.`EnrollmentID`		    = `temp`.`EnrollmentID`,
                               `flat`.`CompletionDate`		    = `temp`.`CompletionDate`,
                               `flat`.`TPEnrollmentID`		    = `temp`.`TPEnrollmentID`,
						       `flat`.`EnrollmentScore`	        = `temp`.`EnrollmentScore`,
                               `flat`.`EnrollmentContentStatus`	= `temp`.`EnrollmentContentStatus`,
                               `flat`.`EnrollmentCreatedBy`		= `temp`.`EnrollmentCreatedBy`,
                               `flat`.`Waived`		            = `temp`.`Waived`,
                               `flat`.`MandatoryWaived`    		= `temp`.`MandatoryWaived`,
						       `flat`.`CourseCompletionDate`	= `temp`.`CourseCompletionDate`,
                               `flat`.`CourseCompletionDays`    = `temp`.`CourseCompletionDays`,
                               `flat`.`CourseTitle`	    	    = `temp`.`CourseTitle`,
                               `flat`.`CourseValidityDate`		= `temp`.`CourseValidityDate`,
						       `flat`.`CourseValidityDays`	    = `temp`.`CourseValidityDays`,
                               `flat`.`CompletionStatus`	    = `temp`.`CompletionStatus`,
                               `flat`.`EnrollmentStatus`		= `temp`.`EnrollmentStatus`,
                               `flat`.`ClassDeliveryType`		= `temp`.`ClassDeliveryType`,
                         
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
			/**
			 * Following are the table where hard delete of records happening and are used in this flat table join
			 * slt_enrollment
			 * slt_person_jobrole_mapping
			 */
			// delete jobrole records
			$query = "(SELECT RUID
						FROM report_admin_manager_class_enrollments rep
						JOIN (
						select sp.id as JobRoleMappingId, del.parent1_entity_id as UserID from report_deleted_logs del
						left join slt_person_jobrole_mapping job on del.parent1_entity_id = job.user_id and del.parent1_entity_type = 'user_id' and del.custom0 = job.job_role
						left join slt_profile_list_items sp on sp.code = del.custom0
						where del.table_name = 'slt_person_jobrole_mapping' and job.id is null";
			if($this->delSync_on != NULL) {
				$query .= " AND del.deleted_on > '" . $this->delSync_on . "'";
			}
			$query .= ") deljobrole ON deljobrole.JobRoleMappingId = rep.JobRoleMappingId AND rep.UserID = deljobrole.UserID)";
			
			// delete null job role records when new job role is assigned
			$query .= "UNION (select rep.RUID
			            from report_admin_manager_class_enrollments rep
			            left join slt_person_jobrole_mapping jobrole on rep.UserId = jobrole.user_id";
			if($this->delSync_on != NULL) {
				$query .= " AND jobrole.created_on > '" . $this->delSync_on . "'";
			}
			$query .= " where rep.JobRoleMappingId is null and jobrole.id is not null)";
			
			// delete enrollment records
			$query .= "UNION
					(SELECT RUID from report_admin_manager_class_enrollments repsum
					join report_deleted_logs del on del.entity_id = repsum.EnrollmentID and del.table_name = 'slt_enrollment'";
			if($this->delSync_on != NULL) {
				$query .= " WHERE del.deleted_on > '" . $this->delSync_on . "'";
			}
			$query .= ")";
			// delete non manager user records
			$query .= "UNION
					(select rep.RUID from report_admin_manager_class_enrollments rep
					JOIN slt_person sp on rep.ManagerID = sp.id and is_manager != 'Y'";
			if($this->delSync_on != NULL) {
				$query .= " and sp.updated_on > '" . $this->delSync_on . "'";
			}
			$query .= ")";
			// delete manager user records
			$query .= "UNION
					(select rep.RUID from report_admin_manager_class_enrollments rep
					JOIN slt_person sp on rep.ManagerID = sp.id and sp.status = 'cre_usr_sts_del'";
			if($this->delSync_on != NULL) {
				$query .= " and sp.updated_on > '" . $this->delSync_on . "'";
			}
			$query .= ")";
			
			// delete reportee user records if soft deleted
			$query .= "UNION
					(select rep.RUID from report_admin_manager_class_enrollments rep
					JOIN slt_person sp on rep.UserID = sp.id and sp.status = 'cre_usr_sts_del'";
			if($this->delSync_on != NULL) {
				$query .= " and sp.updated_on > '" . $this->delSync_on . "'";
			}
			$query .= ")";
			
			// delete class records
			/*$query .= "UNION
					(select rep.RUID from report_admin_manager_class_enrollments rep
					JOIN slt_course_class cls on cls.id = rep.ClassID and cls.status = 'lrn_cls_sts_del'";
			if($this->delSync_on != NULL) {
				$query .= " and cls.updated_on > '" . $this->delSync_on . "'";
			}
			$query .= ")";
			*/
			// delete slt_person_other_manager records
			$query .= "UNION
					(select rep.RUID from report_admin_manager_class_enrollments rep
					join report_deleted_logs del on del.table_name = 'slt_person_other_manager' and rep.UserID = del.parent1_entity_id and rep.ManagerID = del.parent2_entity_id
					left join slt_person_other_manager omgr on omgr.user_id = del.parent1_entity_id and omgr.manager_id = del.parent2_entity_id
					where omgr.id is null";
			if($this->delSync_on != NULL) {
				$query .= " and del.deleted_on > '" . $this->delSync_on . "'";
			}
			$query .= ")";
			
			expDebug::dPrint('delSync_on = ' . $this->delSync_on, 4);
			expDebug::dPrintDBAPI('query to select delete ids', $query);
			return $query;
		} catch(Exception $ex){
			throw new Exception("Error in reportCollectIds ".$ex->getMessage());
		}	
	}
}