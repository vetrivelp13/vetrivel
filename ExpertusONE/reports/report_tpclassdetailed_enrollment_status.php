<?php
include_once $_SERVER['DOCUMENT_ROOT'] . '/reports/ReportSyncUp.php';

class report_tpclassdetailed_enrollment_status extends ReportSyncUp {
	private $report_table = '';
	private $temp_table_name = '';

	/**
	 * Class Constructor
	 * 
	 * @param object $result        	
	 */
	public function __construct($result) {
		parent::__construct($result);
		$this->report_table = __CLASS__;
		$this->temp_table_name = 'temp_' . $this->report_table;
	}

	/**
	 * Method used to Execute Data Syncup Process
	 */
	public function reportExecute() {
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
			$delQuery = "DELETE rprm FROM " . $this->report_table . " as rprm JOIN (" . $delSubQuery . ") as deleted ON rprm.RUID = deleted.RUID";
			expDebug::dPrint("reportDelete Query" . print_r($delQuery, 1), 4);
			$this->queryProcess($delQuery, $this->report_table, 'delete', $this->delSync_on);
		} catch(Exception $ex) {
			throw new Exception("Error in reportDelete " . $ex->getMessage());
		}
	}

	/**
	 * Method used to create temp table
	 */
	protected function createTempTable() {
		try {
			// clear-memcache.php
			$dropTable = "DROP TABLE IF EXISTS " . $this->temp_table_name . "; ";
			$this->db->callQuery($dropTable);
			
			//$createTable = "CREATE TABLE " . $this->temp_table_name . " LIKE " . $this->report_table . "";
			$createTable = "CREATE TABLE " . $this->temp_table_name ." (
                `RUID` varchar(255),
              `EnrollmentID` int(11) DEFAULT NULL,
              `ProgramID` int(11) DEFAULT NULL,
              `TPEnrollmentID` int(11) DEFAULT NULL,
              `UserID` int(11) DEFAULT NULL,
              `PersonUserID` int(11) DEFAULT NULL,
              `CourseID` int(11) DEFAULT NULL,
              `ClassID` int(11) DEFAULT NULL,
              `TrainingPlanTitle` varchar(255) DEFAULT NULL,
              `CourseTitle` longtext,
              `ClassTitle` longtext,
              `ClassCode` varchar(255) NOT NULL,
              `FullName` varchar(255) DEFAULT NULL,
              `Username` varchar(255) DEFAULT NULL,
              `UserStatus` longtext,
              `Waived` varchar(3) NOT NULL DEFAULT '',
              `City` varchar(255) DEFAULT NULL,
              `State` varchar(255) DEFAULT NULL,
              `JobRole` varchar(255),
              `JobRoleMappingId` int(11) DEFAULT NULL,
              `Department` longtext,
              `TrainingPlanIsRequired` varchar(3) DEFAULT NULL,
              `EnrollmentStatus` longtext,
              `CompletionStatus` longtext,
              `Score` float(11,2) DEFAULT NULL,
              `EnrollmentDate` datetime DEFAULT NULL,
              `CompletionDate` datetime DEFAULT NULL,
	           `operation` varchar(10) NULL DEFAULT NULL,
 	           primary key (RUID),
	           key sli_op(operation)
)  ENGINE=InnoDB DEFAULT CHARSET=utf8;";				
			$this->db->callQuery($createTable);
			
			//$alterTable = "ALTER TABLE " . $this->temp_table_name . " ADD COLUMN `operation` varchar(10) NULL DEFAULT NULL;";
			//$this->db->callQuery($alterTable);
		} catch(Exception $ex) {
			throw new Exception("Error in createTempTable " . $ex->getMessage());
		}
	}

	/**
	 * Method used to drop temp table
	 */
	protected function dropTempTable() {
		try {
			$dropTable = "DROP TABLE IF EXISTS " . $this->temp_table_name . "; ";
			$this->db->callQuery($dropTable);
		} catch(Exception $ex) {
			throw new Exception("Error in dropTempTable " . $ex->getMessage());
		}
	}

	/**
	 * Method used to populte delta data to temp table
	 */
	protected function tempTableDataPopulate() {
		try {
			// RUID has been changed in a way it has no duplicates and produce junk ids
			// concat_ws('-',`enr`.`id`,`trp`.`id`,`menr`.`id`,`crs`.`id`,`mcm`.`id`,`cls`.`id`,`lpjm1`.`id`) AS `RUID`,
			$tempDataQuery = "insert into " . $this->temp_table_name . "
					SELECT
						concat_ws('-', enr.id, IFNULL(menr.id, 0), mcm.id, IFNULL(jobrole.id, 0)) AS `RUID`,
        				`enr`.`id` AS `EnrollmentID`,
						`trp`.`id` AS `ProgramID`,
						`enr`.`master_enrollment_id` AS `TPEnrollmentID`,
						`menr`.`user_id` AS `UserID`,
						`per`.`id` As `PersonUserID`,
						`crs`.`id` AS `CourseID`,
						`cls`.`id` AS `ClassID`,
        				if(`enr`.`master_enrollment_id`,`trp`.`title`,'') AS `TrainingPlanTitle`,
                        `crs`.`title` AS `CourseTitle`,
                        `cls`.`title` AS `ClassTitle`,
                        `cls`.`code` AS `ClassCode`,
                        `per`.`full_name` AS `FullName`,
                        `per`.`user_name` AS `Username`,
                        `persts`.`name` AS `UserStatus`,
                        if(isnull(`exmp`.`id`),'No',if(((`exmp`.`is_compliance` = 1)and (`exmp`.`exempted_status` = 1)),'Yes','No')) AS `Waived`,
                        `per`.`city` AS `City`,
                        `sta`.`state_name` AS `State`,
                        `lpjm1`.`job_role` AS `JobRole`,
						jobrole.id as JobRoleMappingId,
                        `depcode`.`name` AS `Department`,
                        `mcm`.`is_required` AS `TrainingPlanIsRequired`,
                        `regst`.`name` AS `EnrollmentStatus`,
                        `compst`.`name` AS `CompletionStatus`,
                        `enr`.`score` AS `Score`,
                        `enr`.`reg_date` AS `EnrollmentDate`,
                        `enr`.`comp_date` AS `CompletionDate`,
                        NULL
			
			        from `slt_program` `trp`
                        join `slt_master_enrollment` `menr` ON ((`trp`.`id` = `menr`.`program_id`))
                        join `slt_module_crs_mapping` `mcm` ON (`mcm`.`program_id` = `trp`.`id`)
                        join `slt_enrollment` `enr` ON ((`enr`.`course_id` = `mcm`.`course_id`) and (`enr`.`user_id` = `menr`.`user_id`)and ((`enr`.`master_enrollment_id` = `menr`.`id`) or isnull(`enr`.`master_enrollment_id`)))
                        join `slt_course_template` `crs` ON ((`crs`.`id` = `mcm`.`course_id`) and (`crs`.`status` != 'lrn_crs_sts_del'))
                        join `slt_course_class` `cls` ON ((`cls`.`course_id` = `crs`.`id`) and (`cls`.`status` != 'lrn_cls_sts_del') and (`cls`.`id` = `enr`.`class_id`))
                        join `slt_person` `per` ON (`per`.`id` = `menr`.`user_id`)
                        left join `slt_person_jobrole_mapping` `lpjm1` ON (`lpjm1`.`user_id` = `per`.`id`)
						left join `slt_profile_list_items` `jobrole` ON (jobrole.code = lpjm1.job_role)
                        left join `slt_enrollment_exempted` `exmp` ON ((`exmp`.`id` = (select `slt_enrollment_exempted`.`id` from `slt_enrollment_exempted` where ((`slt_enrollment_exempted`.`enrollment_id` = `enr`.`id`) and (`slt_enrollment_exempted`.`enroll_type` = 'class')) order by `slt_enrollment_exempted`.`id` desc limit 1)))
                        left join `slt_profile_list_items` `regst` ON ((`regst`.`code` = `enr`.`reg_status`) and (`regst`.`lang_code` = 'cre_sys_lng_eng') and (`regst`.`parent_id` = 358))
                        left join `slt_profile_list_items` `compst` ON ((`compst`.`code` = `enr`.`comp_status`) and (`compst`.`lang_code` = 'cre_sys_lng_eng') and (`compst`.`parent_id` = 357))
                        left join `slt_profile_list_items` `persts` ON ((`persts`.`code` = `per`.`status`) and (`persts`.`parent_id` = 425))
                        left join `slt_profile_list_items` `depcode` ON ((`depcode`.`code` = `per`.`dept_code`) and (`depcode`.`parent_id` = 254) and (`depcode`.`lang_code` = 'cre_sys_lng_eng'))
                        left join `slt_country` `country` ON (`country`.`country_code` = `per`.`country`)
                        left join `slt_state` `sta` ON ((`sta`.`state_code` = `per`.`state`)and (`sta`.`country_code` = `country`.`country_code`))";
			if($this->updSync_on != null) {
				$tempDataQuery .= " WHERE (
			        			(trp.updated_on > '" . $this->updSync_on . "')
			        			OR (menr.updated_on > '" . $this->updSync_on . "')
			        			OR (mcm.updated_on > '" . $this->updSync_on . "')
			        			OR (enr.updated_on > '" . $this->updSync_on . "')
			        			OR (crs.updated_on > '" . $this->updSync_on . "')
			        			OR (cls.updated_on > '" . $this->updSync_on . "')
			        			OR (per.updated_on > '" . $this->updSync_on . "')
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
			
			$query = "insert into " . $this->report_table . "
					SELECT
						`RUID`,
	                    `EnrollmentID`,
	                    `ProgramID`,
	                    `TPEnrollmentID`,
	                    `UserID`,
	                    `PersonUserID`,
	                    `CourseID`,
	                    `ClassID`,
                        `TrainingPlanTitle`,
                        `CourseTitle`,
                        `ClassTitle`,
                        `ClassCode`,
                        `FullName`,
                        `Username`,
                        `UserStatus`,
                        `Waived`,
                        `City`,
                        `State`,
                        `JobRole`,
						`JobRoleMappingId`,
                        `Department`,
                        `TrainingPlanIsRequired`,
                        `EnrollmentStatus`,
                        `CompletionStatus`,
                        `Score`,
                        `EnrollmentDate`,
                        `CompletionDate`	
					FROM " . $this->temp_table_name . "
					where operation is NULL;";
			
			expDebug::dPrint("reportInsert Query" . print_r($query, 1), 4);
			$this->queryProcess($query, $this->report_table, 'insert', $this->insSync_on);
		} catch(Exception $ex) {
			throw new Exception("Error in reportInsert " . $ex->getMessage());
		}
	}

	/**
	 * Method used to update into flat table
	 */
	protected function reportUpdate() {
		try {
			$query = "update " . $this->report_table . " flat
						join " . $this->temp_table_name . " temp on flat.RUID = temp.RUID
						set
	
	                           `flat`.`EnrollmentID`		= `temp`.`EnrollmentID`,
	                           `flat`.`ProgramID`		    = `temp`.`ProgramID`,
	                           `flat`.`TPEnrollmentID`		= `temp`.`TPEnrollmentID`,
	                           `flat`.`UserID`			    = `temp`.`UserID`,
	                           `flat`.`PersonUserID`		= `temp`.`PersonUserID`,
	                           `flat`.`CourseID`		    = `temp`.`CourseID`,
	                           `flat`.`ClassID`		        = `temp`.`ClassID`,
                               `flat`.`TrainingPlanTitle`	= `temp`.`TrainingPlanTitle`,
                               `flat`.`CourseTitle`		    = `temp`.`CourseTitle`,
                               `flat`.`ClassTitle`		    = `temp`.`ClassTitle`,
                               `flat`.`ClassCode`		    = `temp`.`ClassCode`,
                               `flat`.`FullName`	       	= `temp`.`FullName`,
                               `flat`.`Username`	       	= `temp`.`Username`,
                               `flat`.`UserStatus`  		= `temp`.`UserStatus`,
                               `flat`.`Waived`		    	= `temp`.`Waived`,
                               `flat`.`City`		        = `temp`.`City`,
                               `flat`. `State`		    	= `temp`.`State`,
                               `flat`.`JobRole`		        = `temp`.`JobRole`,
							   `flat`.`JobRoleMappingId`	= `temp`.`JobRoleMappingId`,
                               `flat`.`Department`	     	= `temp`.`Department`,
                               `flat`.`TrainingPlanIsRequired`	= `temp`.`TrainingPlanIsRequired`,
                               `flat`.`EnrollmentStatus`	= `temp`.`EnrollmentStatus`,
                               `flat`.`CompletionStatus`	= `temp`.`CompletionStatus`,
                               `flat`.`Score`		     	= `temp`.`Score`,
                               `flat`.`EnrollmentDate`		= `temp`.`EnrollmentDate`,
                               `flat`.`CompletionDate`		= `temp`.`CompletionDate`,
							   `temp`.`operation` =  'update'	
						where flat.RUID = temp.RUID;";
			expDebug::dPrint("reportUpdate Query" . print_r($query, 1), 4);
			$this->queryProcess($query, $this->report_table, 'update', $this->updSync_on);
		} catch(Exception $ex) {
			throw new Exception("Error in reportUpdate " . $ex->getMessage());
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
						FROM report_tpclassdetailed_enrollment_status rep
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
			            from report_tpclassdetailed_enrollment_status rep
			            left join slt_person_jobrole_mapping jobrole on rep.UserId = jobrole.user_id";
			if($this->delSync_on != NULL) {
				$query .= " AND jobrole.created_on > '" . $this->delSync_on . "'";
			}
			$query .= " where rep.JobRoleMappingId is null and jobrole.id is not null)";
			
			// delete enrollment records
			$query .= "UNION
					(SELECT RUID from report_tpclassdetailed_enrollment_status repsum
					join report_deleted_logs del on del.entity_id = repsum.EnrollmentID and del.table_name = 'slt_enrollment'";
			if($this->delSync_on != NULL) {
				$query .= " WHERE del.deleted_on > '" . $this->delSync_on . "'";
			}
			$query .= ")";
			
			return $query;
		} catch(Exception $ex) {
			throw new Exception("Error in reportCollectIds " . $ex->getMessage());
		}
	}
}