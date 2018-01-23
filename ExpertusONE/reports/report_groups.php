<?php
include_once $_SERVER['DOCUMENT_ROOT'] .  '/reports/ReportSyncUp.php';

class report_groups extends ReportSyncUp {
	
	
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
			`RUID` VARCHAR(255),
      `GroupID` int(11) NOT NULL DEFAULT '0',
      `GroupName` varchar(100) NOT NULL,
      `GroupCode` varchar(15) NOT NULL,
      `GroupStatusCode` varchar(100), 
      `GroupStatus` longtext,
      `OnOrAfterStartDate` datetime DEFAULT NULL,
      `OnOrBeforeStartDate` datetime DEFAULT NULL,
      `BetweenStartDate` datetime DEFAULT NULL,
      `BetweenEndDate` datetime DEFAULT NULL,
      `GroupUsersList` longtext,
      `GroupAddedUsersList` longtext,
      `GroupRemovedUsersList` longtext,
      `GroupType` tinyint(1) DEFAULT NULL,
      `DepartmentCode` longtext,
      `EmploymentTypeCode` longtext,
      `UserTypeCode` longtext,
      `JobRoleCode` longtext,
      `OrganizationID` longtext,
      `ManagerRole` varchar(1) DEFAULT NULL,
      `InstructorRole` char(1) DEFAULT NULL,
      `LanguageCode` longtext,
      `CountryCode` longtext,
      `StateCode` longtext,
      `Department` longtext,
      `UserType` longtext,
      `EmploymentType` longtext,
      `Language` longtext,
      `JobRole` longtext,
      `Country` longtext,
      `State` longtext,
      `Organization` longtext,
      `GroupCreatedBy` text,
      `GroupCreatedOn` datetime DEFAULT NULL,
      `GroupUpdatedBy` text,
      `GroupUpdatedOn` datetime DEFAULT NULL,
	  `operation` varchar(10) NULL DEFAULT NULL,  
 	primary key (RUID),
	key sli_op(operation)
)  ENGINE=InnoDB DEFAULT CHARSET=utf8;";
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
					   `grp`.`id` as `RUID`,
                       `grp`.`id` AS `GroupID`,
                       `grp`.`name` AS `GroupName`,
					   `grp`.`code` AS `GroupCode`,
		               `grp`.`status` AS `GroupStatusCode`,
                       `stats`.`name` AS `GroupStatus`,
                       `grpatt`.`on_or_after_start_date` AS `OnOrAfterStartDate`,
                       `grpatt`.`on_or_before_start_date` AS `OnOrBeforeStartDate`,
                       `grpatt`.`between_start_date` AS `BetweenStartDate`,
                       `grpatt`.`between_end_date` AS `BetweenEndDate`,
                       `grp`.`userslist` AS `GroupUsersList`,
                       `grp`.`added_users` AS `GroupAddedUsersList`,
                       `grp`.`removed_users` AS `GroupRemovedUsersList`,
                       `grp`.`is_admin` AS `GroupType`,
                       `grp`.`department` AS `DepartmentCode`,
                       `grp`.`employment_type` AS `EmploymentTypeCode`,
                       `grp`.`user_type` AS `UserTypeCode`,
                       `grp`.`job_role` AS `JobRoleCode`,
                       `grp`.`org_id` AS `OrganizationID`,
                       `grp`.`is_manager` AS `ManagerRole`,
                       `grp`.`is_instructor` AS `InstructorRole`,
                       `grp`.`language` AS `LanguageCode`,
                       `grp`.`country` AS `CountryCode`,
                       `grp`.`state` AS `StateCode`,
                       if(grp.department='All','All',(select group_concat(distinct(ifnull(name,''))) from slt_profile_list_items p1 where FIND_IN_SET(p1.code,grp.department)>0)) as 'Department',
			           if(grp.user_type='All','All',(select group_concat(distinct(ifnull(name,''))) from slt_profile_list_items p1 where FIND_IN_SET(p1.code,grp.user_type)>0)) as 'UserType',
                       if(grp.employment_type='All','All',(select group_concat(distinct(ifnull(name,''))) from slt_profile_list_items p1 where FIND_IN_SET(p1.code,grp.employment_type)>0)) as 'EmploymentType',
                       if(grp.language='All','All',(select group_concat(distinct(ifnull(name,''))) from slt_profile_list_items p1 where FIND_IN_SET(p1.code,grp.language)>0)) as 'Language',
                       if(grp.job_role='All','All',(select group_concat(distinct(ifnull(name,''))) from slt_profile_list_items p1 where FIND_IN_SET(p1.code,grp.job_role)>0)) as 'JobRole',

                       if(grp.country='All','All',(select group_concat(distinct(ifnull(p1.country_name,''))) from slt_country p1 where FIND_IN_SET(p1.country_code,grp.country)>0)) as 'Country',
                       if(grp.state ='All','All',(select group_concat(distinct(ifnull(p1.state_name,''))) from slt_state p1 where FIND_IN_SET(concat(p1.country_code,'-',p1.state_code),grp.state)>0)) as 'State',
                       if(grp.org_id='All','All',(select group_concat(distinct(ifnull(name,''))) from slt_organization p1 where FIND_IN_SET(p1.id,grp.org_id)>0)) as 'Organization',
                       `grp`.`created_by` AS `GroupCreatedBy`,
                       `grp`.`created_on` AS `GroupCreatedOn`,
                       `grp`.`updated_by` AS `GroupUpdatedBy`,
                       `grp`.`updated_on` AS `GroupUpdatedOn`,
						NULL
                   		from `slt_groups` `grp` 
	                   join `slt_profile_list_items` `stats` ON (`stats`.`code` = `grp`.`status`)
                       left join `slt_group_attributes` `grpatt` ON (`grpatt`.`group_id` = `grp`.`id`)
					   left join `report_deleted_logs`  `log` ON (`log`.parent1_entity_id = `grp`.`id` and  `log`.`table_name` = 'slt_group_attributes')
						where grp.status != 'cre_sec_sts_del' 
					AND (grp.updated_on > '".$this->updSync_on ."' OR `grpatt`.updated_on > '".$this->updSync_on ."'
							OR (`log`.`id` is not null)) ";
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
						`GroupID` ,
						`GroupName` ,
						`GroupCode`,
						`GroupStatusCode` , 
						`GroupStatus` ,
						`OnOrAfterStartDate` ,
						`OnOrBeforeStartDate` ,
						`BetweenStartDate` ,
						`BetweenEndDate` ,
						`GroupUsersList` ,
						`GroupAddedUsersList` ,
						`GroupRemovedUsersList` ,
						`GroupType` ,
						`DepartmentCode` ,
						`EmploymentTypeCode` ,
						`UserTypeCode` ,
						`JobRoleCode` ,
						`OrganizationID` ,
						`ManagerRole` ,
						`InstructorRole`,
						`LanguageCode` ,
						`CountryCode` ,
						`StateCode` ,
						`Department` ,
						`UserType` ,
						`EmploymentType` ,
						`Language` ,
						`JobRole` ,
						`Country` ,
						`State` ,
						`Organization` ,
						`GroupCreatedBy` ,
						`GroupCreatedOn` ,
						`GroupUpdatedBy` ,
						`GroupUpdatedOn` 
					
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
							`flat`.`GroupID` 	=	`temp`.`GroupID` ,
							`flat`.`GroupName` 	=	`temp`.`GroupName` ,
							`flat`.`GroupStatusCode`  	=	`temp`.`GroupStatusCode` , 
							`flat`.`GroupStatus` 	=	`temp`.`GroupStatus` ,
							`flat`.`OnOrAfterStartDate` 	=	`temp`.`OnOrAfterStartDate` ,
							`flat`.`OnOrBeforeStartDate` 	=	`temp`.`OnOrBeforeStartDate` ,
							`flat`.`BetweenStartDate` 	=	`temp`.`BetweenStartDate` ,
							`flat`.`BetweenEndDate` 	=	`temp`.`BetweenEndDate` ,
							`flat`.`GroupUsersList` 	=	`temp`.`GroupUsersList` ,
							`flat`.`GroupAddedUsersList` 	=	`temp`.`GroupAddedUsersList` ,
							`flat`.`GroupRemovedUsersList` 	=	`temp`.`GroupRemovedUsersList` ,
							`flat`.`GroupType` 	=	`temp`.`GroupType` ,
							`flat`.`DepartmentCode` 	=	`temp`.`DepartmentCode` ,
							`flat`.`EmploymentTypeCode` 	=	`temp`.`EmploymentTypeCode` ,
							`flat`.`UserTypeCode` 	=	`temp`.`UserTypeCode` ,
							`flat`.`JobRoleCode` 	=	`temp`.`JobRoleCode` ,
							`flat`.`OrganizationID` 	=	`temp`.`OrganizationID` ,
							`flat`.`ManagerRole` 	=	`temp`.`ManagerRole` ,
							`flat`.`InstructorRole`	=	`temp`.`InstructorRole`,
							`flat`.`LanguageCode` 	=	`temp`.`LanguageCode` ,
							`flat`.`CountryCode` 	=	`temp`.`CountryCode` ,
							`flat`.`StateCode` 	=	`temp`.`StateCode` ,
							`flat`.`Department` 	=	`temp`.`Department` ,
							`flat`.`UserType` 	=	`temp`.`UserType` ,
							`flat`.`EmploymentType` 	=	`temp`.`EmploymentType` ,
							`flat`.`Language` 	=	`temp`.`Language` ,
							`flat`.`JobRole` 	=	`temp`.`JobRole` ,
							`flat`.`Country` 	=	`temp`.`Country` ,
							`flat`.`State` 	=	`temp`.`State` ,
							`flat`.`Organization` 	=	`temp`.`Organization` ,
							`flat`.`GroupCreatedBy` 	=	`temp`.`GroupCreatedBy` ,
							`flat`.`GroupCreatedOn` 	=	`temp`.`GroupCreatedOn` ,
							`flat`.`GroupUpdatedBy` 	=	`temp`.`GroupUpdatedBy` ,
							`flat`.`GroupUpdatedOn` 	=	`temp`.`GroupUpdatedOn` ,
						        
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
						 `grp`.`id`  as RUID
						FROM slt_groups grp";
			If ($action == 'delete') {
				$query .= " where grp.`status` = 'cre_sec_sts_del' and grp.updated_on > '".$this->delSync_on."'";
			}
			return $query;
			
		} catch(Exception $ex){
			throw new Exception("Error in reportCollectIds ".$ex->getMessage());
		}	
	}
} 
	
	
