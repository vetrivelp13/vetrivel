<?php
include_once $_SERVER['DOCUMENT_ROOT'] .  '/reports/ReportSyncUp.php';

class report_admin_manager_tp_enrollments extends ReportSyncUp {
	
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
			  `TrainingPlanTitle` varchar(255) DEFAULT NULL,
			  `TrainingPlanCode` varchar(100) NOT NULL,
			  `TrainingPlanStatus` longtext,
			  `TPEnrollmentIsMandatory` varchar(10) DEFAULT NULL,
			  `TPEnrollmentDate` datetime DEFAULT NULL,
			  `TrainingPlanID` int(11) DEFAULT NULL,
			  `TPEnrollmentID` int(11) NOT NULL DEFAULT '0',
			  `TPCompletionDate` datetime DEFAULT NULL,
			  `TPScore` float(11,2) NOT NULL DEFAULT '0.00',
			  `TPContentStatus` varchar(20) DEFAULT NULL,
			  `TPCreatedBy` varchar(100) DEFAULT NULL,
			  `TPPercentageComplete` int(11) DEFAULT NULL,
			  `UserID` int(11) NOT NULL DEFAULT '0',
			  `UserName` varchar(255) DEFAULT NULL,
			  `FullName` varchar(255) DEFAULT NULL,
			  `Email` varchar(255) DEFAULT NULL,
			  `JobTitle` longtext,
			  `OrganizationName` longtext,
			  `State` varchar(255) DEFAULT NULL,
			  `City` varchar(255) DEFAULT NULL,
			  `ManagerName` varchar(255) DEFAULT NULL,
			  `ManagerFullName` varchar(255) DEFAULT NULL,
			  `ManagerID` int(11) NOT NULL DEFAULT '0',
			  `DirectManager` varchar(3) NOT NULL DEFAULT '',
			  `TPWaived` varchar(3) NOT NULL DEFAULT '',
			  `TPEnrollmentStatus` longtext,
			  `TrainingPlanObjectName` longtext,
			  `TPIsCurrent` char(1) DEFAULT 'Y',
			  `TPCertPath` int(5) DEFAULT '0',
			  `TPCertStatus` varchar(11) NOT NULL DEFAULT '',
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
			            concat_ws('-',`enr`.`program_id`,`enr`.`id`,`per`.`id`,`perm`.`id`) as `RUID`,
                        `prg`.`title` AS `TrainingPlanTitle`,
                        `prg`.`code` AS `TrainingPlanCode`,
                        `pstatus`.`name` AS `TrainingPlanStatus`,
                        `enr`.`mandatory` AS `TPEnrollmentIsMandatory`,
                        `enr`.`reg_date` AS `TPEnrollmentDate`,
                        `enr`.`program_id` AS `TrainingPlanID`,
                        `enr`.`id` AS `TPEnrollmentID`,
                        `enr`.`comp_date` AS `TPCompletionDate`,
                        `enr`.`score` AS `TPScore`,
                        `enr`.`content_status` AS `TPContentStatus`,
                        `enr`.`created_by` AS `TPCreatedBy`,
                        `enr`.`percentage_complete` AS `TPPercentageComplete`,
                        `per`.`id` AS `UserID`,
                        `per`.`user_name` AS `UserName`,
                        `per`.`full_name` AS `FullName`,
                        `per`.`email` AS `Email`,
                        `pjt`.`name` AS `JobTitle`,
                        `sorg`.`name` AS `OrganizationName`,
                        `st`.`state_name` AS `State`,
                        `per`.`city` AS `City`,
                        `perm`.`user_name` AS `ManagerName`,
                        `perm`.`full_name` AS `ManagerFullName`,
                        `perm`.`id` AS `ManagerID`,
                        if((`othr`.`is_direct` = 'Y'),'Yes','No') AS `DirectManager`,
                        if(isnull(`exmp`.`id`), 'No', if(((`exmp`.`is_mandatory` = 'Y') and (`exmp`.`exempted_status` = 1)),'Yes','No')) AS `TPWaived`,
                        `ostatus`.`name` AS `TPEnrollmentStatus`,
                        `enrdeltype`.`name` AS `TrainingPlanObjectName`,
                        `enr`.`is_current` AS `TPIsCurrent`,
                        `enr`.`recertify_path` AS `TPCertPath`,
                        if((`enr`.`recertify_path` > 1), 'Recertified','Certified') AS `TPCertStatus`,
                        NULL
			
                   from `slt_person` `perm`
                        join `slt_person_other_manager` `othr` ON (`othr`.`manager_id` = `perm`.`id`)
                        join `slt_person` `per` ON (`per`.`id` = `othr`.`user_id`)
                        join `slt_master_enrollment` `enr` ON (`enr`.`user_id` = `per`.`id`)
                        left join `slt_enrollment_exempted` `exmp` ON (`exmp`.`id` = (select `slt_enrollment_exempted`.`id` from `slt_enrollment_exempted` where ((`slt_enrollment_exempted`.`enrollment_id` = `enr`.`id`) and (`slt_enrollment_exempted`.`enroll_type` = 'tp'))  order by `slt_enrollment_exempted`.`id` desc limit 1))
                        join `slt_program` `prg` ON (`prg`.`id` = `enr`.`program_id`)
                        join `slt_profile_list_items` `pstatus` ON (`prg`.`status` = `pstatus`.`code`)
                        join `slt_profile_list_items` `ostatus` ON (`enr`.`overall_status` = `ostatus`.`code`)
                        join `slt_profile_list_items` `enrdeltype` ON (`enrdeltype`.`code` = `prg`.`object_type`)
                        left join `slt_organization` `sorg` ON (`sorg`.`id` = `per`.`org_id`)
                        left join `slt_profile_list_items` `pjt` ON (`pjt`.`code` = `per`.`job_title`)
                        left join `slt_state` `st` ON ((`st`.`state_code` = `per`.`state`) and (`st`.`country_code` = `per`.`country`))
					
                   where
                        ((`per`.`status` != 'cre_usr_sts_del')
						and (`perm`.`status` != 'cre_usr_sts_del')
						and (`perm`.`is_manager` = 'Y'))";
			if($this->updSync_on != null) {
				$tempDataQuery .= "AND (
			        			(perm.updated_on > '" . $this->updSync_on . "')
			        			OR (per.updated_on > '" . $this->updSync_on . "')
			        			OR (enr.updated_on > '" . $this->updSync_on . "')
			        			OR (prg.updated_on > '" . $this->updSync_on . "')
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
						`RUID`,
                        `TrainingPlanTitle`,
                        `TrainingPlanCode`,
                        `TrainingPlanStatus`,
                        `TPEnrollmentIsMandatory`,
                        `TPEnrollmentDate`,
                        `TrainingPlanID`,
                        `TPEnrollmentID`,
                        `TPCompletionDate`,
                        `TPScore`,
                        `TPContentStatus`,
                        `TPCreatedBy`,
                        `TPPercentageComplete`,
                        `UserID`,
                        `UserName`,
                        `FullName`,
                        `Email`,
                        `JobTitle`,
                        `OrganizationName`,
                        `State`,
                        `City`,
                        `ManagerName`,
                        `ManagerFullName`,
                        `ManagerID`,
                        `DirectManager`,
                        `TPWaived`,
                        `TPEnrollmentStatus`,
                        `TrainingPlanObjectName`,
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
	                           `flat`.`TrainingPlanTitle`		= `temp`.`TrainingPlanTitle`,
	                           `flat`.`TrainingPlanCode`		= `temp`.`TrainingPlanCode`,
	                           `flat`.`TrainingPlanStatus`		= `temp`.`TrainingPlanStatus`,
	                           `flat`.`TPEnrollmentIsMandatory`	= `temp`.`TPEnrollmentIsMandatory`,
	                           `flat`.`TPEnrollmentDate`		= `temp`.`TPEnrollmentDate`,
	                           `flat`.`TrainingPlanID`		    = `temp`.`TrainingPlanID`,
	                           `flat`.`TPEnrollmentID`		    = `temp`.`TPEnrollmentID`,
                               `flat`.`TPCompletionDate`	    = `temp`.`TPCompletionDate`,
                               `flat`.`TPScore`		            = `temp`.`TPScore`,
                               `flat`.`TPContentStatus`		    = `temp`.`TPContentStatus`,
                               `flat`.`TPCreatedBy`		        = `temp`.`TPCreatedBy`,
                               `flat`.`TPPercentageComplete`	= `temp`.`TPPercentageComplete`,
                               `flat`.`UserID`	             	= `temp`.`UserID`,
                               `flat`.`UserName`  	         	= `temp`.`UserName`,
                               `flat`.`FullName`		    	= `temp`.`FullName`,
                               `flat`.`Email`		            = `temp`.`Email`,
                               `flat`.`JobTitle`		    	= `temp`.`JobTitle`,
                               `flat`.`OrganizationName`		= `temp`.`OrganizationName`,
                               `flat`.`State`	     	        = `temp`.`State`,
                               `flat`.`City`	                = `temp`.`City`,
                               `flat`.`ManagerName`	            = `temp`.`ManagerName`,
                               `flat`.`ManagerFullName`	        = `temp`.`ManagerFullName`,
                               `flat`.`ManagerID`		     	= `temp`.`ManagerID`,
                               `flat`.`DirectManager`		    = `temp`.`DirectManager`,
                               `flat`.`TPWaived`		        = `temp`.`TPWaived`,
						       `flat`.`TPEnrollmentStatus`	    = `temp`.`TPEnrollmentStatus`,
                               `flat`.`TrainingPlanObjectName`	= `temp`.`TrainingPlanObjectName`,
                               `flat`.`TPIsCurrent`		     	= `temp`.`TPIsCurrent`,
                               `flat`.`TPCertPath`		        = `temp`.`TPCertPath`,
                               `flat`.`TPCertStatus`    		= `temp`.`TPCertStatus`,
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
			// delete slt_master_enrollment records
			$query = "(SELECT RUID from report_admin_manager_tp_enrollments reptp
					join report_deleted_logs del on del.entity_id = reptp.TPEnrollmentID and del.table_name = 'slt_master_enrollment'";
			if($this->delSync_on != NULL) {
				$query .= " WHERE del.deleted_on > '" . $this->delSync_on . "'";
			}
			$query .= ")";
			
			// delete non manager user records
			$query .= "UNION
					(select reptp.RUID from report_admin_manager_tp_enrollments reptp
					JOIN slt_person sp on reptp.ManagerID = sp.id and is_manager != 'Y'";
			if($this->delSync_on != NULL) {
				$query .= " and sp.updated_on > '" . $this->delSync_on . "'";
			}
			$query .= ")";
			
			// delete manager user records which were soft deleted
			$query .= "UNION
					(select reptp.RUID from report_admin_manager_tp_enrollments reptp
					JOIN slt_person sp on reptp.ManagerID = sp.id and sp.status = 'cre_usr_sts_del'";
			if($this->delSync_on != NULL) {
				$query .= " and sp.updated_on > '" . $this->delSync_on . "'";
			}
			$query .= ")";
			
			// delete reportee user records if soft deleted
			$query .= "UNION
					(select reptp.RUID from report_admin_manager_tp_enrollments reptp
					JOIN slt_person sp on reptp.UserID = sp.id and sp.status = 'cre_usr_sts_del'";
			if($this->delSync_on != NULL) {
				$query .= " and sp.updated_on > '" . $this->delSync_on . "'";
			}
			$query .= ")";
			
			// delete program records
			/*$query .= "UNION
					(select reptp.RUID from report_admin_manager_tp_enrollments reptp
					JOIN slt_program prg on prg.id = reptp.TrainingPlanID and prg.status = 'lrn_lpn_sts_del'";
			if($this->delSync_on != NULL) {
				$query .= " and prg.updated_on > '" . $this->delSync_on . "'";
			}
			$query .= ")";*/
			
			// delete slt_person_other_manager records
			$query .= "UNION
					(select reptp.RUID from report_admin_manager_tp_enrollments reptp
					join report_deleted_logs del on del.table_name = 'slt_person_other_manager' and reptp.UserID = del.parent1_entity_id and reptp.ManagerID = del.parent2_entity_id
					left join slt_person_other_manager omgr on omgr.user_id = del.parent1_entity_id and omgr.manager_id = del.parent2_entity_id
					where omgr.id is null";
			if($this->delSync_on != NULL) {
				$query .= " and del.deleted_on > '" . $this->delSync_on . "'";
			}
			$query .= ")";
			
			return $query;
			
		} catch(Exception $ex){
			throw new Exception("Error in reportCollectIds ".$ex->getMessage());
		}	
	}
}