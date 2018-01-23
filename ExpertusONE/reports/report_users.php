<?php
include_once $_SERVER['DOCUMENT_ROOT'] .  '/reports/ReportSyncUp.php';

class report_users extends ReportSyncUp {
	
	
	private $report_table = '';
	
	private $temp_table_name = '';
	
	private $execution_start_time = '';
	
	/**
	 * Class Constructor
	 * @param object $result
	 */
	public function __construct($result){
		parent::__construct($result);
		$this->report_table = __CLASS__;
		$this->temp_table_name = 'temp_' . $this->report_table;
		$this->execution_start_time = date("Y-m-d H:i:s");
	}

	/**
	 * Method used to Execute Data Syncup Process
	 */
	public function reportExecute(){
		try {
		//	$start = date("Y-m-d H:i:s");
			$start = $this->execution_start_time;
			expDebug::dPrint("start time >> " . $start, 4);
			// Remove deleted records in flat table
			$this->reportDelete();
		
			// Create a temp table to process insert/update records
			$this->createTempTable();
			
			// Data popuation to temp table
			$this->tempTableDataPopulate();
			
			// Update records in flat table
			//$this->reportUpdate();
			
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
			$start = $this->execution_start_time;
			$delSubQuery = $this->reportCollectIds('delete');
			$delQuery = "DELETE rprm FROM ". $this->report_table ." as rprm JOIN (".$delSubQuery.") as deleted ON rprm.UserID = deleted.UserID";
			expDebug::dPrint("reportDelete Query" . print_r($delQuery, 1), 4);
			$this->queryProcess($delQuery, $this->report_table, 'delete', $this->delSync_on,$start);
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
						  	`RUID` varchar(255),
						    `UserID` int(11) NOT NULL DEFAULT '0',
						    `FullName` varchar(255) DEFAULT NULL,
						    `LastName` varchar(255) DEFAULT NULL,
						    `FirstName` varchar(255) DEFAULT NULL,
						    `UserName` varchar(255) DEFAULT NULL,
						    `Email` varchar(255) DEFAULT NULL,
						    `ContactNumber` varchar(50) DEFAULT NULL,
						    `Address1` varchar(255) DEFAULT NULL,
						    `Address2` varchar(255) DEFAULT NULL,
						    `City` varchar(255) DEFAULT NULL,
						    `ZipCode` varchar(50) DEFAULT NULL,
						    `UserStatusCode` varchar(255) DEFAULT NULL,
						    `UserStatus` varchar(255) DEFAULT NULL,
						    `OrganizationID` int(11) DEFAULT NULL,
						    `OrganizationName` varchar(2500) DEFAULT NULL,
						    `UserType` varchar(300) DEFAULT NULL,
						    `EmploymentType` varchar(300) DEFAULT NULL,
						    `Department` varchar(300) DEFAULT NULL,
						    `JobTitle` varchar(300) DEFAULT NULL,
						    `ManagerName` varchar(255) DEFAULT NULL,
						    `ManagerUserName` varchar(255) DEFAULT NULL,
						    `Country` varchar(255) DEFAULT NULL,
						    `State` varchar(255) DEFAULT NULL,
						    `UserIsInstructor` varchar(1) NOT NULL DEFAULT 'N',
						    `UserIsManager` varchar(1) NOT NULL DEFAULT 'N',
						    `UserUID` int(10) unsigned NOT NULL DEFAULT '0',
						    `UserLogin` int(11) NOT NULL DEFAULT '0',  
						    `UserMobileNumber` varchar(50) DEFAULT NULL,
						    `UserSMSAlert` int(11) NOT NULL DEFAULT '0',
						    `UserEmployeeNumber` varchar(255) DEFAULT NULL,
						    `UserHireDate` datetime DEFAULT NULL,
						    `ManagerID` int(11) DEFAULT NULL,
						    `UserPreferredLocationID` int(11) DEFAULT NULL,
						    `UserPreferredLanguage` varchar(255) DEFAULT NULL,
						    `LocationName` varchar(2500) DEFAULT NULL,
						    `UserAboutMe` longtext,
						    `UserChatSignedIn` enum('0', '1') DEFAULT '0',
						    `UserDeletedBy` varchar(255) DEFAULT NULL,
						    `UserDeletedOn` datetime DEFAULT NULL,
						    `UserCreatedBy` varchar(255) DEFAULT NULL,
						    `UserCreatedOn` datetime DEFAULT NULL,
						    `UserUpdatedBy` varchar(255) DEFAULT NULL,
						    `UserUpdatedOn` datetime DEFAULT NULL,
						    `UserEntityType` varchar(50) DEFAULT NULL,
						    `UserTimezone` varchar(255) DEFAULT NULL,
						    `UserPreferredCurrency` varchar(255) DEFAULT NULL,
						    `LoginDateTime` varchar(24) DEFAULT NULL,
						    `DepartmentCode` varchar(255) DEFAULT NULL,
						    `ManagerEmail` varchar(255) DEFAULT NULL,
						    `AddressVerified` varchar(10) DEFAULT NULL,
						    `IsAddressValid` varchar(10) DEFAULT NULL,
						    `AddressInvalidReason` longtext,
						    `UsersStatusNumber` tinyint(4) NOT NULL DEFAULT '0',
						    `UserCustomAttribute0` VARCHAR(750) DEFAULT NULL,
                            `UserCustomAttribute1` VARCHAR(750) DEFAULT NULL,
                            `UserCustomAttribute2` VARCHAR(750) DEFAULT NULL,
                            `UserCustomAttribute3` VARCHAR(750) DEFAULT NULL,
                            `UserCustomAttribute4` VARCHAR(750) DEFAULT NULL,
                            `UserCustomAttribute5` VARCHAR(750) DEFAULT NULL,
                            `UserCustomAttribute6` VARCHAR(750) DEFAULT NULL,
                            `UserCustomAttribute7` VARCHAR(750) DEFAULT NULL,
                            `UserCustomAttribute8` VARCHAR(750) DEFAULT NULL,
                            `UserCustomAttribute9` VARCHAR(750) DEFAULT NULL, 
						  	`operation` varchar(10) NULL DEFAULT NULL,
						  PRIMARY KEY (`RUID`),
						  key sli_op(operation)
						)  ENGINE=InnoDB DEFAULT CHARSET=utf8;";
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
			$insSubQuery = $this->reportCollectIds();
		
			$tempDataQuery = "insert into ".$this->temp_table_name."
					               SELECT 
                                	concat_ws('-',`per`.`id`,`per`.`org_id`,`usr`.`uid`,`per`.`manager_id` ,`per`.`preferred_loc_id`) AS `RUID`,
                                	`per`.`id` AS `UserID`,
                                	`per`.`full_name` AS `FullName`,
                                	`per`.`last_name` AS `LastName`,
                                	`per`.`first_name` AS `FirstName`,
                                	`per`.`user_name` AS `UserName`,
                                	`per`.`email` AS `Email`,
                                	(CASE WHEN (`per`.`phone_no` <> '') THEN `per`.`phone_no` ELSE `per`.`mobile_no` END) AS `ContactNumber`,
                                	`per`.`addr1` AS `Address1`,
                                	`per`.`addr2` AS `Address2`,
                                	`per`.`city` AS `City`,
                                	`per`.`zip` AS `ZipCode`,
                                	`persts`.`code` AS `UserStatusCode`,
                                	`persts`.`name` AS `UserStatus`,
                                	`per`.`org_id` AS `OrganizationID`,
                                	`org`.`name` AS `OrganizationName`,
                                	`usertype`.`name` AS `UserType`,
                                	`emptype`.`name` AS `EmploymentType`,
                                	`depcode`.`name` AS `Department`,
                                	`jobtit`.`name` AS `JobTitle`,
                                	`mgr`.`full_name` AS `ManagerName`,
                                	`mgr`.`user_name` AS `ManagerUserName`,
                                	`country`.`country_name` AS `Country`,
                                	`sta`.`state_name` AS `State`,
                                	`per`.`is_instructor` AS `UserIsInstructor`,
                                	`per`.`is_manager` AS `UserIsManager`,
                                	`usr`.`uid` AS `UserUID`,
                                	`usr`.`login` AS `UserLogin`,	
                                	`per`.`mobile_no` AS `UserMobileNumber`,
                                	`per`.`sms_alert` AS `UserSMSAlert`,
                                	`per`.`employee_no` AS `UserEmployeeNumber`,
                                	`per`.`hire_date` AS `UserHireDate`,
                                	`per`.`manager_id` AS `ManagerID`,	
                                	`per`.`preferred_loc_id` AS `UserPreferredLocationID`,
                                	`plng`.`name` AS `UserPreferredLanguage`,	
                                	`loc`.`name` AS `LocationName`,
                                	`per`.`about_me` AS `UserAboutMe`,
                                	`per`.`chat_signed_in` AS `UserChatSignedIn`,
                                	`per`.`deleted_by` AS `UserDeletedBy`,
                                	`per`.`deleted_on` AS `UserDeletedOn`,
                                	`per`.`created_by` AS `UserCreatedBy`,
                                	`per`.`created_on` AS `UserCreatedOn`,
                                	`per`.`updated_by` AS `UserUpdatedBy`,
                                	`per`.`updated_on` AS `UserUpdatedOn`, 
                                	`perety`.`name` AS `UserEntityType`,
                                	`usrtmz`.`name` AS `UserTimezone`,
                                	`pcrncy`.`name` AS `UserPreferredCurrency`,
                                	if(usr.login>0,from_unixtime(usr.login,'%Y-%m-%d %H:%i:%s'),'') as `LoginDateTime`,	
                                	`per`.`dept_code`           AS `DepartmentCode`,
                                	`mgr`.`email` 			    AS `ManagerEmail`,
                                	`per`.`is_verified_add`		AS `AddressVerified`,
                                	`per`.`is_valid_add`		AS `IsAddressValid`,
                                	`per`.`invalid_add_reason`	AS `AddressInvalidReason`,
                                	`usr`.`status` AS `UsersStatusNumber`,						
							IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt0`.`opt_name`) FROM slt_custom_attr_options `opt0` WHERE FIND_IN_SET(`opt0`.`opt_code`,`per`.`e1_cattr0`)),`per`.`e1_cattr0`),`per`.`e1_cattr0`) AS `UserCustomAttribute0`,
                            IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt1`.`opt_name`) FROM slt_custom_attr_options `opt1` WHERE FIND_IN_SET(`opt1`.`opt_code`,`per`.`e1_cattr1`)),`per`.`e1_cattr1`),`per`.`e1_cattr1`) AS `UserCustomAttribute1`,
                            IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt2`.`opt_name`) FROM slt_custom_attr_options `opt2` WHERE FIND_IN_SET(`opt2`.`opt_code`,`per`.`e1_cattr2`)),`per`.`e1_cattr2`),`per`.`e1_cattr2`) AS `UserCustomAttribute2`,
                            IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt3`.`opt_name`) FROM slt_custom_attr_options `opt3` WHERE FIND_IN_SET(`opt3`.`opt_code`,`per`.`e1_cattr3`)),`per`.`e1_cattr3`),`per`.`e1_cattr3`) AS `UserCustomAttribute3`,
                            IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt4`.`opt_name`) FROM slt_custom_attr_options `opt4` WHERE FIND_IN_SET(`opt4`.`opt_code`,`per`.`e1_cattr4`)),`per`.`e1_cattr4`),`per`.`e1_cattr4`) AS `UserCustomAttribute4`,
                            IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt5`.`opt_name`) FROM slt_custom_attr_options `opt5` WHERE FIND_IN_SET(`opt5`.`opt_code`,`per`.`e1_cattr5`)),`per`.`e1_cattr5`),`per`.`e1_cattr5`) AS `UserCustomAttribute5`,
                            IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt6`.`opt_name`) FROM slt_custom_attr_options `opt6` WHERE FIND_IN_SET(`opt6`.`opt_code`,`per`.`e1_cattr6`)),`per`.`e1_cattr6`),`per`.`e1_cattr6`) AS `UserCustomAttribute6`,
                            IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt7`.`opt_name`) FROM slt_custom_attr_options `opt7` WHERE FIND_IN_SET(`opt7`.`opt_code`,`per`.`e1_cattr7`)),`per`.`e1_cattr7`),`per`.`e1_cattr7`) AS `UserCustomAttribute7`,
                            IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt8`.`opt_name`) FROM slt_custom_attr_options `opt8` WHERE FIND_IN_SET(`opt8`.`opt_code`,`per`.`e1_cattr8`)),`per`.`e1_cattr8`),`per`.`e1_cattr8`) AS `UserCustomAttribute8`,
                            IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt9`.`opt_name`) FROM slt_custom_attr_options `opt9` WHERE FIND_IN_SET(`opt9`.`opt_code`,`per`.`e1_cattr9`)),`per`.`e1_cattr9`),`per`.`e1_cattr9`) AS `UserCustomAttribute9`,
							NULL                                
                                FROM slt_person per   
                                INNER JOIN `users` `usr` ON CONVERT(`per`.`user_name` USING utf8) = `usr`.`name`
                                LEFT JOIN `slt_organization` `org` ON `org`.`id` = `per`.`org_id` AND `org`.`status` <> 'cre_org_sts_del' 
                                LEFT JOIN `slt_person` `mgr` ON `mgr`.`id` = `per`.`manager_id` AND `per`.`status` <> 'cre_usr_sts_del'
                                LEFT JOIN `slt_profile_list_items` `usrtmz` ON `usrtmz`.`code` = `per`.`time_zone` AND `usrtmz`.`parent_id` = 164
                                LEFT JOIN `slt_profile_list_items` `plng` ON `plng`.`code` = `per`.`preferred_language` AND `plng`.`parent_id` =  116
                                LEFT JOIN `slt_profile_list_items` `usertype` ON `usertype`.`code` = `per`.`user_type` AND `usertype`.`parent_id` = 285 AND `usertype`.`lang_code` = 'cre_sys_lng_eng'
                                LEFT JOIN `slt_profile_list_items` `emptype` ON `emptype`.`code` = `per`.`employment_type` AND `emptype`.`parent_id` = 260 AND `emptype`.`lang_code` = 'cre_sys_lng_eng'
                                LEFT JOIN `slt_profile_list_items` `depcode` ON `depcode`.`code` = `per`.`dept_code` AND `depcode`.`parent_id` = 254 AND `depcode`.`lang_code` = 'cre_sys_lng_eng'
                                
                                LEFT JOIN `slt_profile_list_items` `jobtit` ON `jobtit`.`code` = `per`.`job_title` AND `jobtit`.`parent_id` = 270 AND `jobtit`.`lang_code` = 'cre_sys_lng_eng'
                                LEFT JOIN `slt_profile_list_items` `persts` ON `persts`.`code` = `per`.`status` AND `persts`.`parent_id` = 425 AND `persts`.`lang_code` = 'cre_sys_lng_eng'
                                LEFT JOIN `slt_profile_list_items` `orgtype` ON `orgtype`.`code` = `org`.`type` AND `orgtype`.`parent_id` = 425
                                LEFT JOIN `slt_profile_list_items` `perety` ON `perety`.`code` = `per`.`entity_type` 
                                LEFT JOIN `slt_profile_list_items` `pcrncy` ON `pcrncy`.`attr1` = `per`.`preferred_currency` AND `pcrncy`.`parent_id` = 103
                                LEFT JOIN `slt_country` `country` ON `country`.`country_code` = `per`.`country`
                                LEFT JOIN `slt_state` `sta` ON `sta`.`state_code` = `per`.`state` AND `sta`.`country_code` = `country`.`country_code`
                                LEFT JOIN `slt_location` `loc` ON `loc`.`id` = `per`.`preferred_loc_id`
							 INNER JOIN (".$insSubQuery.") as inserted on inserted.UserID = per.id
							 where per.status != 'cre_usr_sts_del'";
					
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
			$start = $this->execution_start_time;
			// truncate flat table, when script invoked with flush true
			if ($this->flush == 'true') {
				$flushTable  = "TRUNCATE TABLE " . $this->report_table ."; ";
				expDebug::dPrint('$flushTable query :' . $flushTable, 4);
				$this->db->callQuery($flushTable);
			}
			
			$query = "insert into ". $this->report_table ."
					SELECT 
						`RUID` ,
						`UserID` ,
						`FullName` ,
						`LastName` ,
						`FirstName` ,
						`UserName` ,
						`Email` ,
						`ContactNumber` ,
						`Address1` ,
						`Address2` ,
						`City` ,
						`ZipCode` ,
						`UserStatusCode` ,
						`UserStatus`,
						`OrganizationID` ,
						`OrganizationName`,
						`UserType`,
						`EmploymentType`,
						`Department`,
						`JobTitle`,
						`ManagerName` ,
						`ManagerUserName` ,
						`Country` ,
						`State` ,
						`UserIsInstructor` ,
						`UserIsManager`,
						`UserUID` ,
						`UserLogin` ,
						`UserMobileNumber`,
						`UserSMSAlert` ,
						`UserEmployeeNumber` ,
						`UserHireDate`,
						`ManagerID` ,
						`UserPreferredLocationID` ,
						`UserPreferredLanguage`,
						`LocationName`,
						`UserAboutMe`,
						`UserChatSignedIn`,
						`UserDeletedBy` ,
						`UserDeletedOn`,
						`UserCreatedBy` ,
						`UserCreatedOn`,
						`UserUpdatedBy` ,
						`UserUpdatedOn`,
						`UserEntityType`,
						`UserTimezone`,
						`UserPreferredCurrency`,
						`LoginDateTime` ,
						`DepartmentCode` ,
						`ManagerEmail` ,
						`AddressVerified` ,
						`IsAddressValid` ,
						`AddressInvalidReason`,					 
						`UsersStatusNumber`,
						 `UserCustomAttribute0`,
			             `UserCustomAttribute1`,
			             `UserCustomAttribute2`,
			             `UserCustomAttribute3`,
			             `UserCustomAttribute4`,
			             `UserCustomAttribute5`,
			             `UserCustomAttribute6`,
			             `UserCustomAttribute7`,
			             `UserCustomAttribute8`,
			             `UserCustomAttribute9`			            
					FROM ". $this->temp_table_name ."
					where operation is NULL;";
			
			expDebug::dPrint("reportInsert Query" . print_r($query, 1), 4);
			$this->queryProcess($query, $this->report_table, 'insert', $this->insSync_on,$start);
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
							`flat`.`UserID` 	=	`temp`.`UserID` ,
							`flat`.`FullName` 	=	`temp`.`FullName` ,
							`flat`.`LastName` 	=	`temp`.`LastName` ,
							`flat`.`FirstName` 	=	`temp`.`FirstName` ,
							`flat`.`UserName` 	=	`temp`.`UserName` ,
							`flat`.`Email` 	=	`temp`.`Email` ,
							`flat`.`ContactNumber` 	=	`temp`.`ContactNumber` ,
							`flat`.`Address1` 	=	`temp`.`Address1` ,
							`flat`.`Address2` 	=	`temp`.`Address2` ,
							`flat`.`City` 	=	`temp`.`City` ,
							`flat`.`ZipCode` 	=	`temp`.`ZipCode` ,
							`flat`.`UserStatusCode` 	=	`temp`.`UserStatusCode` ,
							`flat`.`UserStatus`	=	`temp`.`UserStatus`,
							`flat`.`OrganizationID` 	=	`temp`.`OrganizationID` ,
							`flat`.`OrganizationName`	=	`temp`.`OrganizationName`,
							`flat`.`UserType`	=	`temp`.`UserType`,
							`flat`.`EmploymentType`	=	`temp`.`EmploymentType`,
							`flat`.`Department`	=	`temp`.`Department`,
							`flat`.`JobTitle`	=	`temp`.`JobTitle`,
							`flat`.`ManagerName` 	=	`temp`.`ManagerName` ,
							`flat`.`ManagerUserName` 	=	`temp`.`ManagerUserName` ,
							`flat`.`Country` 	=	`temp`.`Country` ,
							`flat`.`State` 	=	`temp`.`State` ,
							`flat`.`UserIsInstructor` 	=	`temp`.`UserIsInstructor` ,
							`flat`.`UserIsManager`	=	`temp`.`UserIsManager`,
							`flat`.`UserUID` 	=	`temp`.`UserUID` ,
							`flat`.`UserLogin` 	=	`temp`.`UserLogin` ,
							`flat`.`UserMobileNumber`	=	`temp`.`UserMobileNumber`,
							`flat`.`UserSMSAlert` 	=	`temp`.`UserSMSAlert` ,
							`flat`.`UserEmployeeNumber` 	=	`temp`.`UserEmployeeNumber` ,
							`flat`.`UserHireDate`	=	`temp`.`UserHireDate`,
							`flat`.`ManagerID` 	=	`temp`.`ManagerID` ,
							`flat`.`UserPreferredLocationID` 	=	`temp`.`UserPreferredLocationID` ,
							`flat`.`UserPreferredLanguage`	=	`temp`.`UserPreferredLanguage`,
							`flat`.`LocationName`	=	`temp`.`LocationName`,
							`flat`.`UserAboutMe`	=	`temp`.`UserAboutMe`,
							`flat`.`UserChatSignedIn`	=	`temp`.`UserChatSignedIn`,
							`flat`.`UserDeletedBy` 	=	`temp`.`UserDeletedBy` ,
							`flat`.`UserDeletedOn`	=	`temp`.`UserDeletedOn`,
							`flat`.`UserCreatedBy` 	=	`temp`.`UserCreatedBy` ,
							`flat`.`UserCreatedOn`	=	`temp`.`UserCreatedOn`,
							`flat`.`UserUpdatedBy` 	=	`temp`.`UserUpdatedBy` ,
							`flat`.`UserUpdatedOn`	=	`temp`.`UserUpdatedOn`,
							`flat`.`UserEntityType`	=	`temp`.`UserEntityType`,
							`flat`.`UserTimezone`	=	`temp`.`UserTimezone`,
							`flat`.`UserPreferredCurrency`	=	`temp`.`UserPreferredCurrency`,
							`flat`.`LoginDateTime` 	=	`temp`.`LoginDateTime` ,
							`flat`.`DepartmentCode` 	=	`temp`.`DepartmentCode` ,
							`flat`.`ManagerEmail` 	=	`temp`.`ManagerEmail` ,
							`flat`.`AddressVerified` 	=	`temp`.`AddressVerified` ,
							`flat`.`IsAddressValid` 	=	`temp`.`IsAddressValid` ,
							`flat`.`AddressInvalidReason`	=	`temp`.`AddressInvalidReason`,
							`flat`.`UsersStatusNumber`	=	`temp`.`UsersStatusNumber`,
							`flat`.`UserCustomAttribute0`  =   `temp`.`UserCustomAttribute0`,
                            `flat`.`UserCustomAttribute1`    =   `temp`.`UserCustomAttribute1`,
                            `flat`.`UserCustomAttribute2`    =   `temp`.`UserCustomAttribute2`,
                            `flat`.`UserCustomAttribute3`    =   `temp`.`UserCustomAttribute3`,
                            `flat`.`UserCustomAttribute4`    =   `temp`.`UserCustomAttribute4`,
                            `flat`.`UserCustomAttribute5`    =   `temp`.`UserCustomAttribute5`,
                            `flat`.`UserCustomAttribute6`    =   `temp`.`UserCustomAttribute6`,
                            `flat`.`UserCustomAttribute7`    =   `temp`.`UserCustomAttribute7`,
                            `flat`.`UserCustomAttribute8`    =   `temp`.`UserCustomAttribute8`,
                            `flat`.`UserCustomAttribute9`    =   `temp`.`UserCustomAttribute9`,
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
	protected function reportCollectIds($action = '') {
		try {
			$start = $this->execution_start_time;
			$query = "SELECT `per`.`id` AS `UserID`
					FROM slt_person per   
					INNER JOIN `users` `usr` ON CONVERT(`per`.`user_name` USING utf8) = `usr`.`name`
					LEFT JOIN `slt_organization` `org` ON `org`.`id` = `per`.`org_id` AND `org`.`status` <> 'cre_org_sts_del' 
					LEFT JOIN `report_deleted_logs` `logs` ON `logs`.`parent2_entity_id` = `usr`.`uid` and `logs`.`table_name` = 'users_roles'
			        LEFT JOIN `users_roles` `urole` ON `urole`.`uid` = `usr`.`uid`
					LEFT JOIN `role` `rl` ON `rl`.`rid` = `urole`.`rid`
					LEFT join `slt_groups` `grp` on `grp`.`name` = `rl`.`name`
					LEFT JOIN `slt_location` `loc` ON `loc`.`id` = `per`.`preferred_loc_id`
					where ( (`per`.updated_on > '".$this->delSync_on ."' and `per`.updated_on <= '".$start ."') or (`org`.updated_on > '".$this->delSync_on ."' and `org`.updated_on <= '".$start ."')
					or (`loc`.updated_on > '".$this->delSync_on ."' and `loc`.updated_on <= '".$start ."')  or (if(usr.login > 0,from_unixtime(usr.login,'%Y-%m-%d %H:%i:%s'),'".$this->delSync_on ."') > '".$this->delSync_on ."' and if(usr.login > 0,from_unixtime(usr.login,'%Y-%m-%d %H:%i:%s'),'".$this->delSync_on ."') <= '".$start ."')
					or (`logs`.deleted_on > '".$this->delSync_on ."' and `logs`.deleted_on <= '".$start ."')or (grp.updated_on > '".$this->delSync_on ."' and grp.updated_on <= '".$start ."'))  group by per.id";
			
			/* If ($action == 'delete') {
				$query .= "where (del_mdl.id is not null AND del_mdl.deleted_on > '".$this->delSync_on ."')
				 			OR (del_crp_map.id is not null AND del_crp_map.deleted_on > '".$this->delSync_on ."')
				 			OR (prm.`status` = 'lrn_lpn_sts_del' and prm.updated_on > '".$this->delSync_on ."')";
			} */
			expDebug::dPrint("reportCollectIds" . print_r($query, 1), 4);
			return $query;
			
		} catch(Exception $ex){
			throw new Exception("Error in reportCollectIds ".$ex->getMessage());
		}	
	}
} 
	
	
