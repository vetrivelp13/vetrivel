<?php
include_once $_SERVER['DOCUMENT_ROOT'] .  '/reports/ReportSyncUp.php';

class report_audit_report extends ReportSyncUp {
	
	
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
			//$this->reportDelete();
			
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
			/* $delSubQuery = $this->reportCollectIds('delete');
			$delQuery = "DELETE rprm FROM ". $this->report_table ." as rprm JOIN (".$delSubQuery.") as deleted ON rprm.RUID = deleted.RUID";
			expDebug::dPrint("reportDelete Query" . print_r($delQuery, 1), 4);
			$this->queryProcess($delQuery, $this->report_table, 'delete', $this->delSync_on); */
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
			
			// $createTable = "CREATE TABLE ".$this->temp_table_name." LIKE ".$this->report_table."";
			$createTable = "CREATE TABLE " . $this->temp_table_name ." (
			  `RUID` varchar(255) NOT NULL,
			  `AuditID` int(11) NOT NULL DEFAULT '0',
			  `AuditEntityType` varchar(50) NOT NULL,
			  `AuditEntityID` int(11) NOT NULL,
			  `AuditCreatedOn` datetime NOT NULL,
			  `AuditOldValue` longtext,
			  `AuditNewValue` longtext,
			  `LoggedInUserID` int(11) NOT NULL,
			  `LoggedInUserAction` longtext NOT NULL,
			  `AuditModuleName` varchar(256) NOT NULL,
			  `AuditFunctionalityName` longtext NOT NULL,
			  `AuditEsignDateTime` datetime DEFAULT NULL,
			  `AssessmentTitle` longtext,
			  `AssessmentCode` varchar(100) DEFAULT NULL,
			  `AssessmentID` int(11) DEFAULT '0',
			  `SurveyTitle` longtext,
			  `SurveyCode` varchar(100) DEFAULT NULL,
			  `SurveyID` int(11) DEFAULT '0',
			  `TrainingPlanTitle` varchar(255) DEFAULT NULL,
			  `TrainingPlanCode` varchar(100) DEFAULT NULL,
			  `TrainingPlanID` int(11) DEFAULT '0',
			  `ClassTitle` longtext,
			  `ClassCode` varchar(255) DEFAULT NULL,
			  `ClassID` int(11) DEFAULT '0',
			  `ClassCourseID` int(11) DEFAULT NULL,
			  `ClassCourseCode` varchar(255) DEFAULT NULL,
			  `CourseID` int(11) DEFAULT '0',
			  `CourseCode` varchar(255) DEFAULT NULL,
			  `ContentCode` varchar(2048) DEFAULT NULL,
			  `ContentID` int(11) DEFAULT '0',
			  `OrganizationID` int(11) DEFAULT '0',
			  `OrganizationName` longtext,
			  `AuditCostCenter` varchar(255) DEFAULT NULL,
			  `UserID` int(11) DEFAULT '0',
			  `FullName` varchar(255) DEFAULT NULL,
			  `UserName` varchar(255) DEFAULT NULL,
			  `LoggedInUserFullName` varchar(255) DEFAULT NULL,
			  `LoggedInUserName` varchar(255) DEFAULT NULL,
			  `LoggedInUserEmail` varchar(255) DEFAULT NULL,
			  `LoggedInUserOrganization` longtext,
			  `LoggedInUserCountry` varchar(255) DEFAULT NULL,
			  `AuditBannerTitle` longtext,
			  `AuditBannerID` int(11) DEFAULT '0',
			  `operation` varchar(10) NULL DEFAULT NULL,
			  PRIMARY KEY (`RUID`),
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
						`aud`.`id` AS `RUID`,
						`aud`.`id` AS `AuditID`,
						`aud`.`entity_type` AS `AuditEntityType`,
						`aud`.`entity_id` AS `AuditEntityID`,
						`aud`.`created_on` AS `AuditCreatedOn`,
						`aud`.`old_value` AS `AuditOldValue`,
						`aud`.`new_value` AS `AuditNewValue`,
						`aud`.`logged_user_id` AS `LoggedInUserID`,
						`aud`.`logged_user_action` AS `LoggedInUserAction`,
						`aud`.`module_name` AS `AuditModuleName`,
						`aud`.`functionality_name` AS `AuditFunctionalityName`,
						`aud`.`esign_date_time` AS `AuditEsignDateTime`,
						`asmt`.`title` AS `AssessmentTitle`,
						`asmt`.`code` AS `AssessmentCode`,
						`asmt`.`id` AS `AssessmentID`,
						`asmt`.`title` AS `SurveyTitle`,
						`asmt`.`code` AS `SurveyCode`,
						`asmt`.`id` AS `SurveyID`,
						`prg`.`title` AS `TrainingPlanTitle`,
						`prg`.`code` AS `TrainingPlanCode`,
						`prg`.`id` AS `TrainingPlanID`,
						`cls`.`title` AS `ClassTitle`,
						`cls`.`code` AS `ClassCode`,
						`cls`.`id` AS `ClassID`,
						`cls`.`course_id` AS `ClassCourseID`,
						`crst`.`code` AS `ClassCourseCode`,
						`crs`.`id` AS `CourseID`,
						`crs`.`code` AS `CourseCode`,
						`cntmas`.`code` AS `ContentCode`,
						`cntmas`.`id` AS `ContentID`,
						`org`.`id` AS `OrganizationID`,
						`org`.`name` AS `OrganizationName`,
						`org`.`cost_center` AS `AuditCostCenter`,
						`pers`.`id` AS `UserID`,
						`pers`.`full_name` AS `FullName`,
						`pers`.`user_name` AS `UserName`,
						`person`.`full_name` AS `LoggedInUserFullName`,
						`person`.`user_name` AS `LoggedInUserName`,
						`person`.`email` AS `LoggedInUserEmail`,
						`perorg`.`name` AS `LoggedInUserOrganization`,
						`person`.`country` AS `LoggedInUserCountry`,
						`ann`.`title` AS `AuditBannerTitle`,
						`ann`.`id` AS `AuditBannerID`,
			             NULL
			         
				    FROM `slt_audit_trail` `aud` 
						LEFT JOIN `slt_person` `person` force index(pk_person_id) ON `person`.`id` = `aud`.`logged_user_id`
						LEFT JOIN `slt_course_template` `crs` ON `crs`.`id` = `aud`.`entity_id` AND 'Course'  = `aud`.`entity_type`
						LEFT JOIN `slt_course_class` `cls` ON `cls`.`id` = `aud`.`entity_id` AND 'Class' = `aud`.`entity_type`
						LEFT JOIN `slt_course_template` `crst` ON `crst`.`id` = `cls`.`course_id` AND 'Class' = `aud`.`entity_type`
						LEFT JOIN `slt_program` `prg` ON `prg`.`id` = `aud`.`entity_id` AND  `aud`.`entity_type` IN ('Certification','Training Plan','Curricula','Learning Plan')
						LEFT JOIN `slt_person` `pers` ON `pers`.`id` = `aud`.`entity_id` AND 'User' = `aud`.`entity_type`
						LEFT JOIN `slt_organization` `perorg` ON `perorg`.`id` = `pers`.`org_id` AND `perorg`.`status` = 'cre_org_sts_act'
						LEFT JOIN `slt_survey` `asmt` ON `asmt`.`id` = `aud`.`entity_id` AND `aud`.`entity_type` IN ('Survey','Assessment')
						LEFT JOIN `slt_content_master` `cntmas` ON `cntmas`.`id` = `aud`.`entity_id` AND   `aud`.`entity_type` IN ('AICC','Knowledge Content','SCORM 1.2','SCORM 2004','Video on Demand','Tin Can','Content')
						LEFT JOIN `slt_organization` `org` ON `org`.`id` = `aud`.`entity_id` AND 'Organization' = `aud`.`entity_type`
						LEFT JOIN `slt_announcement_master` `ann` ON `ann`.`id` = `aud`.`entity_id` AND 'Banner' = `aud`.`entity_type`
						where `aud`.created_on > '".$this->updSync_on ."'";
			
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
						`AuditID` ,
						`AuditEntityType`,
						`AuditEntityID`,
						`AuditCreatedOn`,
						`AuditOldValue`,
						`AuditNewValue`,
						`LoggedInUserID` ,
						`LoggedInUserAction`,
						`AuditModuleName`,
						`AuditFunctionalityName` ,
						`AuditEsignDateTime`,
						`AssessmentTitle`,
						`AssessmentCode` ,
						`AssessmentID`,
						`SurveyTitle`,
						`SurveyCode`,
						`SurveyID`,
						`TrainingPlanTitle`,
						`TrainingPlanCode`,
						`TrainingPlanID`,
						`ClassTitle`,
						`ClassCode` ,
						`ClassID` ,
						`ClassCourseID` ,
						`ClassCourseCode` ,
						`CourseID` ,
						`CourseCode` ,
						`ContentCode` ,
						`ContentID` ,
						`OrganizationID` ,
						`OrganizationName`,
						`AuditCostCenter` ,
						`UserID` ,
						`FullName` ,
						`UserName` ,
						`LoggedInUserFullName` ,
						`LoggedInUserName` ,
						`LoggedInUserEmail` ,
						`LoggedInUserOrganization`,
						`LoggedInUserCountry` ,
						`AuditBannerTitle`,
						`AuditBannerID` 
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
							`flat`.`AuditID` 	=	`temp`.`AuditID` ,
							`flat`.`AuditEntityType`	=	`temp`.`AuditEntityType`,
							`flat`.`AuditEntityID`	=	`temp`.`AuditEntityID`,
							`flat`.`AuditCreatedOn`	=	`temp`.`AuditCreatedOn`,
							`flat`.`AuditOldValue`	=	`temp`.`AuditOldValue`,
							`flat`.`AuditNewValue`	=	`temp`.`AuditNewValue`,
							`flat`.`LoggedInUserID` 	=	`temp`.`LoggedInUserID` ,
							`flat`.`LoggedInUserAction`	=	`temp`.`LoggedInUserAction`,
							`flat`.`AuditModuleName`	=	`temp`.`AuditModuleName`,
							`flat`.`AuditFunctionalityName` 	=	`temp`.`AuditFunctionalityName` ,
							`flat`.`AuditEsignDateTime`	=	`temp`.`AuditEsignDateTime`,
							`flat`.`AssessmentTitle`	=	`temp`.`AssessmentTitle`,
							`flat`.`AssessmentCode` 	=	`temp`.`AssessmentCode` ,
							`flat`.`AssessmentID`	=	`temp`.`AssessmentID`,
							`flat`.`SurveyTitle`	=	`temp`.`SurveyTitle`,
							`flat`.`SurveyCode`	=	`temp`.`SurveyCode`,
							`flat`.`SurveyID`	=	`temp`.`SurveyID`,
							`flat`.`TrainingPlanTitle`	=	`temp`.`TrainingPlanTitle`,
							`flat`.`TrainingPlanCode`	=	`temp`.`TrainingPlanCode`,
							`flat`.`TrainingPlanID`	=	`temp`.`TrainingPlanID`,
							`flat`.`ClassTitle`	=	`temp`.`ClassTitle`,
							`flat`.`ClassCode` 	=	`temp`.`ClassCode` ,
							`flat`.`ClassID` 	=	`temp`.`ClassID` ,
							`flat`.`ClassCourseID` 	=	`temp`.`ClassCourseID` ,
							`flat`.`ClassCourseCode` 	=	`temp`.`ClassCourseCode` ,
							`flat`.`CourseID` 	=	`temp`.`CourseID` ,
							`flat`.`CourseCode` 	=	`temp`.`CourseCode` ,
							`flat`.`ContentCode` 	=	`temp`.`ContentCode` ,
							`flat`.`ContentID` 	=	`temp`.`ContentID` ,
							`flat`.`OrganizationID` 	=	`temp`.`OrganizationID` ,
							`flat`.`OrganizationName`	=	`temp`.`OrganizationName`,
							`flat`.`AuditCostCenter` 	=	`temp`.`AuditCostCenter` ,
							`flat`.`UserID` 	=	`temp`.`UserID` ,
							`flat`.`FullName` 	=	`temp`.`FullName` ,
							`flat`.`UserName` 	=	`temp`.`UserName` ,
							`flat`.`LoggedInUserFullName` 	=	`temp`.`LoggedInUserFullName` ,
							`flat`.`LoggedInUserName` 	=	`temp`.`LoggedInUserName` ,
							`flat`.`LoggedInUserEmail` 	=	`temp`.`LoggedInUserEmail` ,
							`flat`.`LoggedInUserOrganization`	=	`temp`.`LoggedInUserOrganization`,
							`flat`.`LoggedInUserCountry` 	=	`temp`.`LoggedInUserCountry` ,
							`flat`.`AuditBannerTitle`	=	`temp`.`AuditBannerTitle`,
							`flat`.`AuditBannerID` 	=	`temp`.`AuditBannerID` ,
						        
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
			/* $query = "SELECT 
		 		DISTINCT concat_ws('-', `prm`.`id`, `sg`.`id`, `mcm`.`id`, `mcm`.`course_id`) as RUID
			FROM slt_program prm 
			LEFT JOIN `slt_module` `sg` ON `prm`.`id` = `sg`.`program_id`
			LEFT JOIN `slt_module_crs_mapping` `mcm` ON `mcm`.`module_id` = `sg`.`id`
			LEFT JOIN `report_deleted_logs` del_crp_map ON del_crp_map.entity_id = mcm.id and del_crp_map.table_name = 'slt_module_crs_mapping'
			LEFT JOIN `report_deleted_logs` del_mdl ON del_mdl.entity_id = sg.id and del_mdl.table_name = 'slt_module'";
			
			If ($action == 'delete') {
				$query .= "where (del_mdl.id is not null AND del_mdl.deleted_on > '".$this->delSync_on ."')
				 			OR (del_crp_map.id is not null AND del_crp_map.deleted_on > '".$this->delSync_on ."')
				 			OR (prm.`status` = 'lrn_lpn_sts_del' and prm.updated_on > '".$this->delSync_on ."')";
			}
			
			return $query; */
			
		} catch(Exception $ex){
			throw new Exception("Error in reportCollectIds ".$ex->getMessage());
		}	
	}
} 
	
	
