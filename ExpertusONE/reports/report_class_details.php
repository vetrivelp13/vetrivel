<?php
include_once $_SERVER['DOCUMENT_ROOT'] .  '/reports/ReportSyncUp.php';

class report_class_details extends ReportSyncUp {
	
	
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
			$delQuery = "DELETE rprm FROM ". $this->report_table ." as rprm JOIN (".$delSubQuery.") as deleted ON rprm.ClassID = deleted.RUID";
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
			
			// $createTable = "CREATE TABLE ".$this->temp_table_name." LIKE ".$this->report_table."";
			$createTable = "CREATE TABLE ".$this->temp_table_name." (
						  `RUID` VARCHAR(255) NOT NULL,
						  `ClassID` int(11) NOT NULL DEFAULT '0',
						  `CourseID` int(11) DEFAULT NULL,
						  `CourseTitle` longtext,
						  `CourseCode` varchar(255) NOT NULL,
						  `ClassTitle` longtext,
						  `ClassCode` varchar(255) NOT NULL,
						  `ClassAdditionalInformation` longtext,
						  `ClassAddInfoInCatalog` int(1) NOT NULL DEFAULT '0',
						  `ClassAddInfoInNotification` int(1) NOT NULL DEFAULT '0',
						  `ClassMinSeats` int(11) DEFAULT NULL,
						  `ClassMaxSeats` int(11) DEFAULT NULL,
						  `ClassDuration` varchar(25),
						  `ClassPrice` decimal(16,2) DEFAULT '0.00',
						  `SessionID` int(11) DEFAULT '0',
						  `SessionTitle` longtext,
						  `SessionStartDate` datetime DEFAULT NULL,
						  `SessionStartTime` varchar(5) DEFAULT NULL,
						  `SessionEndTime` varchar(5) DEFAULT NULL,
						  `SessionLocationID` int(11) DEFAULT NULL,
						  `SessionInstructorID` int(11) DEFAULT NULL,
						  `instructorUsername` varchar(255) DEFAULT NULL,
						  `instructorFullname` varchar(255) DEFAULT NULL,
						  `SessionIsPresenter` varchar(3) NOT NULL DEFAULT '',
						  `SessionWaitlistCount` int(5) DEFAULT NULL,
						  `SessionPresenterURL` varchar(2000) DEFAULT ' ',
						  `SessionAttendeeURL` varchar(2000) DEFAULT ' ',
						  `SessionType` longtext,
						  `SessionTimeZone` longtext,
						  `LocationName` longtext,
						  `ClassDeliveryType` longtext,
						  `ClassGroupMapType` longtext,
						  `ClassStatus` longtext,
						  `ClassStatusCode` varchar(255) DEFAULT NULL,
						  `ClassCustomAttribute0` TEXT DEFAULT NULL,
                          `ClassCustomAttribute1` TEXT DEFAULT NULL,
                          `ClassCustomAttribute2` TEXT DEFAULT NULL,
                          `ClassCustomAttribute3` TEXT DEFAULT NULL,
                          `ClassCustomAttribute4` TEXT DEFAULT NULL,
                          `ClassCustomAttribute5` TEXT DEFAULT NULL,
                          `ClassCustomAttribute6` TEXT DEFAULT NULL,
                          `ClassCustomAttribute7` TEXT DEFAULT NULL,
                          `ClassCustomAttribute8` TEXT DEFAULT NULL,
                          `ClassCustomAttribute9` TEXT DEFAULT NULL,
							`operation` varchar(10) NULL DEFAULT NULL,
							primary key (RUID),
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
			$tempDataQuery = "insert into ".$this->temp_table_name."
					SELECT
					   concat_ws('-',`cls`.`id`,`ses`.`id`,`ses`.`location_id`,`ins_det`.`id`) AS `RUID`,
        			   `cls`.`id` AS `ClassID`,
        			   `cls`.`course_id` AS `CourseID`,
        			   `crs`.`title` AS `CourseTitle`,
        				 `crs`.`code` AS `CourseCode`,
				   	   `cls`.`title` AS `ClassTitle`,
      			       `cls`.`code` AS `ClassCode`,
     			       `cls`.`additional_info` AS `ClassAdditionalInformation`,
      			       `cls`.`addn_catalog_show` AS `ClassAddInfoInCatalog`,
        		       `cls`.`addn_notification_show` AS `ClassAddInfoInNotification`,
        		       `cls`.`min_seats` AS `ClassMinSeats`,
        		       `cls`.`max_seats` AS `ClassMaxSeats`,
        		       `cls`.`price` AS `ClassPrice`,
        					 `cls`.`scheduled_duration` AS `ClassDuration`,
        		       `ses`.`id` AS `SessionID`,
        		       `ses`.`title` AS `SessionTitle`,
        		       `ses`.`start_date` AS `SessionStartDate`,
        		       `ses`.`start_time` AS `SessionStartTime`,
        		       `ses`.`end_time` AS `SessionEndTime`,
        		       `ses`.`location_id` AS `SessionLocationID`,
        		       `ins_det`.`instructor_id` AS `SessionInstructorID`,
        		       `per`.`user_name` AS `instructorUsername`,
        		       `per`.`full_name` AS `instructorFullname`,
        		       if((`ins_det`.`is_presenter` = 'Y'), 'Yes', 'No') AS `SessionIsPresenter`,
        		       `ses`.`waitlist_count` AS `SessionWaitlistCount`,
        		       `ses`.`presenter_url` AS `SessionPresenterURL`,
        		       `ses`.`attendee_url` AS `SessionAttendeeURL`,
        		       `sestype`.`name` AS `SessionType`,
        		       `sestmz`.`name` AS `SessionTimeZone`,
        		       `loc`.`name` AS `LocationName`,
		          	   `dlty`.`name` AS `ClassDeliveryType`,
				       `type`.`name` AS `ClassGroupMapType`,
				       `sts`.`name` AS `ClassStatus`,
				       `cls`.`status` AS `ClassStatusCode`,
				        IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt0`.`opt_name`) FROM slt_custom_attr_options `opt0` WHERE FIND_IN_SET(`opt0`.`opt_code`,`cls`.`e1_cattr0`)),`cls`.`e1_cattr0`),`cls`.`e1_cattr0`) AS `ClassCustomAttribute0`,
                        IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt1`.`opt_name`) FROM slt_custom_attr_options `opt1` WHERE FIND_IN_SET(`opt1`.`opt_code`,`cls`.`e1_cattr1`)),`cls`.`e1_cattr1`),`cls`.`e1_cattr1`) AS `ClassCustomAttribute1`,
                        IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt2`.`opt_name`) FROM slt_custom_attr_options `opt2` WHERE FIND_IN_SET(`opt2`.`opt_code`,`cls`.`e1_cattr2`)),`cls`.`e1_cattr2`),`cls`.`e1_cattr2`) AS `ClassCustomAttribute2`,
                        IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt3`.`opt_name`) FROM slt_custom_attr_options `opt3` WHERE FIND_IN_SET(`opt3`.`opt_code`,`cls`.`e1_cattr3`)),`cls`.`e1_cattr3`),`cls`.`e1_cattr3`) AS `ClassCustomAttribute3`,
                        IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt4`.`opt_name`) FROM slt_custom_attr_options `opt4` WHERE FIND_IN_SET(`opt4`.`opt_code`,`cls`.`e1_cattr4`)),`cls`.`e1_cattr4`),`cls`.`e1_cattr4`) AS `ClassCustomAttribute4`,
                        IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt5`.`opt_name`) FROM slt_custom_attr_options `opt5` WHERE FIND_IN_SET(`opt5`.`opt_code`,`cls`.`e1_cattr5`)),`cls`.`e1_cattr5`),`cls`.`e1_cattr5`) AS `ClassCustomAttribute5`,
                        IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt6`.`opt_name`) FROM slt_custom_attr_options `opt6` WHERE FIND_IN_SET(`opt6`.`opt_code`,`cls`.`e1_cattr6`)),`cls`.`e1_cattr6`),`cls`.`e1_cattr6`) AS `ClassCustomAttribute6`,
                        IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt7`.`opt_name`) FROM slt_custom_attr_options `opt7` WHERE FIND_IN_SET(`opt7`.`opt_code`,`cls`.`e1_cattr7`)),`cls`.`e1_cattr7`),`cls`.`e1_cattr7`) AS `ClassCustomAttribute7`,
                        IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt8`.`opt_name`) FROM slt_custom_attr_options `opt8` WHERE FIND_IN_SET(`opt8`.`opt_code`,`cls`.`e1_cattr8`)),`cls`.`e1_cattr8`),`cls`.`e1_cattr8`) AS `ClassCustomAttribute8`,
                        IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt9`.`opt_name`) FROM slt_custom_attr_options `opt9` WHERE FIND_IN_SET(`opt9`.`opt_code`,`cls`.`e1_cattr9`)),`cls`.`e1_cattr9`),`cls`.`e1_cattr9`) AS `ClassCustomAttribute9`,
                        NULL
			        
		          from `slt_course_class` `cls`
		          					inner join `slt_course_template` `crs` ON `crs`.`id` = `cls`.`course_id`
                        join `slt_profile_list_items` `dlty` ON ((`dlty`.`code` = `cls`.`delivery_type`) and (`dlty`.`parent_id` = 331))
                        join `slt_profile_list_items` `type` ON (`type`.`code` = `cls`.`entity_type`)
                        join `slt_profile_list_items` `sts` ON (`sts`.`code` = `cls`.`status`)
                        join `slt_profile_list_items` `lng` ON ((`lng`.`code` = `cls`.`lang_code`)  and (`lng`.`parent_id` = 116))
                        left join `slt_course_class_session` `ses` ON (`ses`.`class_id` = `cls`.`id`)
                        left join `slt_profile_list_items` `sestype` ON (`sestype`.`code` = `ses`.`type`)
                        left join `slt_profile_list_items` `sestmz` ON (`sestmz`.`code` = `ses`.`timezone`)
                        left join `slt_session_instructor_details` `ins_det` ON (`ins_det`.`session_id` = `ses`.`id`)
                        left join `slt_person` `per` ON (`per`.`id` = `ins_det`.`instructor_id`)
                        left join `slt_location` `loc` ON (`loc`.`id` = `ses`.`location_id`)	
					
			      where ((cls.updated_on > '".$this->updSync_on ."') 
			      		OR (ses.updated_on > '".$this->updSync_on ."')
			      				 OR (ins_det.updated_on > '".$this->updSync_on ."')
			      				 		OR (per.updated_on > '".$this->updSync_on ."')
			      				 			OR (loc.updated_on > '".$this->updSync_on ."'))
							AND  cls.status != 'lrn_cls_sts_del' ";
			
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
						`ClassID`,
						`CourseID`,
						`CourseCode`,
						`CourseTitle`,
						`ClassTitle`,
						`ClassCode`,
						`ClassAdditionalInformation`,
						`ClassAddInfoInCatalog`,
						`ClassAddInfoInNotification`,
						`ClassMinSeats`,
						`ClassMaxSeats`,
						`ClassPrice`,
						`ClassDuration`,
						`SessionID`,
						`SessionTitle`,
						`SessionStartDate`,
						`SessionStartTime`,
						`SessionEndTime` ,
						`SessionLocationID` ,
						`SessionInstructorID`,
						`instructorUsername` ,
						`instructorFullname` ,
						`SessionIsPresenter`,
						`SessionWaitlistCount` ,
						`SessionPresenterURL` ,
						`SessionAttendeeURL` ,
						`SessionType`,
						`SessionTimeZone`,
						`LocationName`,
						`ClassDeliveryType`,
						`ClassGroupMapType`,
						`ClassStatus`,
						`ClassStatusCode`,
						`ClassCustomAttribute0`,
						`ClassCustomAttribute1`,
						`ClassCustomAttribute2`,
						`ClassCustomAttribute3`,
						`ClassCustomAttribute4`,
						`ClassCustomAttribute5`,
						`ClassCustomAttribute6`,
						`ClassCustomAttribute7`,
						`ClassCustomAttribute8`,
						`ClassCustomAttribute9`
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
							`flat`.`ClassID`	=	`temp`.`ClassID`,
							`flat`.`CourseID`	=	`temp`.`CourseID`,
							`flat`.`CourseTitle`	=	`temp`.`CourseTitle`,
							`flat`.`CourseCode`	=	`temp`.`CourseCode`,
							`flat`.`ClassTitle`	=	`temp`.`ClassTitle`,
							`flat`.`ClassCode`	=	`temp`.`ClassCode`,
							`flat`.`ClassAdditionalInformation`	=	`temp`.`ClassAdditionalInformation`,
							`flat`.`ClassAddInfoInCatalog`	=	`temp`.`ClassAddInfoInCatalog`,
							`flat`.`ClassAddInfoInNotification`	=	`temp`.`ClassAddInfoInNotification`,
							`flat`.`ClassMinSeats`	=	`temp`.`ClassMinSeats`,
							`flat`.`ClassMaxSeats`	=	`temp`.`ClassMaxSeats`,
							`flat`.`ClassPrice` =	`temp`.`ClassPrice`,
							`flat`.`ClassDuration` =	`temp`.`ClassDuration`,
							`flat`.`SessionID`	=	`temp`.`SessionID`,
							`flat`.`SessionTitle`	=	`temp`.`SessionTitle`,
							`flat`.`SessionStartDate`	=	`temp`.`SessionStartDate`,
							`flat`.`SessionStartTime`	=	`temp`.`SessionStartTime`,
							`flat`.`SessionEndTime` 	=	`temp`.`SessionEndTime` ,
							`flat`.`SessionLocationID` 	=	`temp`.`SessionLocationID` ,
							`flat`.`SessionInstructorID`	=	`temp`.`SessionInstructorID`,
							`flat`.`instructorUsername` 	=	`temp`.`instructorUsername` ,
							`flat`.`instructorFullname` 	=	`temp`.`instructorFullname` ,
							`flat`.`SessionIsPresenter`	=	`temp`.`SessionIsPresenter`,
							`flat`.`SessionWaitlistCount` 	=	`temp`.`SessionWaitlistCount` ,
							`flat`.`SessionPresenterURL` 	=	`temp`.`SessionPresenterURL` ,
							`flat`.`SessionAttendeeURL` 	=	`temp`.`SessionAttendeeURL` ,
							`flat`.`SessionType`	=	`temp`.`SessionType`,
							`flat`.`SessionTimeZone`	=	`temp`.`SessionTimeZone`,
							`flat`.`LocationName`	=	`temp`.`LocationName`,
							`flat`.`ClassDeliveryType`	=	`temp`.`ClassDeliveryType`,
							`flat`.`ClassGroupMapType`	=	`temp`.`ClassGroupMapType`,
							`flat`.`ClassStatus`	=	`temp`.`ClassStatus`,
							`flat`.`ClassStatusCode`	=	`temp`.`ClassStatusCode`,
							`flat`.`ClassCustomAttribute0`   =   `temp`.`ClassCustomAttribute0`,
                            `flat`.`ClassCustomAttribute1`  =   `temp`.`ClassCustomAttribute1`,
                            `flat`.`ClassCustomAttribute2`  =   `temp`.`ClassCustomAttribute2`,
                            `flat`.`ClassCustomAttribute3`  =   `temp`.`ClassCustomAttribute3`,
                            `flat`.`ClassCustomAttribute4`  =   `temp`.`ClassCustomAttribute4`,
                            `flat`.`ClassCustomAttribute5`  =   `temp`.`ClassCustomAttribute5`,
                            `flat`.`ClassCustomAttribute6`  =   `temp`.`ClassCustomAttribute6`,
                            `flat`.`ClassCustomAttribute7`  =   `temp`.`ClassCustomAttribute7`,
                            `flat`.`ClassCustomAttribute8`  =   `temp`.`ClassCustomAttribute8`,
                            `flat`.`ClassCustomAttribute9`  =   `temp`.`ClassCustomAttribute9`,
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
						DISTINCT `cls`.`id`  AS `RUID`
						FROM `slt_course_class` `cls`
							LEFT join `slt_course_class_session` `ses` ON (`ses`.`class_id` = `cls`.`id`)
							LEFT join `slt_session_instructor_details` `ins_det` ON (`ins_det`.`session_id` = `ses`.`id`)
							LEFT join `slt_person` `per` ON (`per`.`id` = `ins_det`.`instructor_id`)
							LEFT join `slt_location` `loc` ON (`loc`.`id` = `ses`.`location_id`)
								Where (`cls`.`updated_on` > '".$this->delSync_on ."') OR (`ses`.`updated_on` > '".$this->delSync_on ."')";
			return $query;
		} catch(Exception $ex){
			throw new Exception("Error in reportCollectIds ".$ex->getMessage());
		}	
	}
} 
	
	
