<?php
include_once $_SERVER['DOCUMENT_ROOT'] .  '/reports/ReportSyncUp.php';

class report_content_launch_detail extends ReportSyncUp {
	
	
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
			//$this->queryProcess($delQuery, $this->report_table, 'delete', $this->delSync_on);
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
			$createTable = "CREATE TABLE ".$this->temp_table_name ." (
			        `RUID` varchar(255),
                      `UserID` int(11) NOT NULL,
                      `EnrollmentID` int(11) DEFAULT NULL,
                      `MaxAttempts` int(11) DEFAULT NULL,
                      `AttemptNumber` int(11) DEFAULT NULL,
                      `AttemptStatus` longtext,
                      `AttemptDate` datetime DEFAULT NULL,
                      `TimeSpent` varchar(50) DEFAULT NULL,
                      `TotalAttempts` int(11) DEFAULT NULL,
                      `SuspendData` longtext,
                      `ContentStatus` varchar(75) DEFAULT NULL,
                      `CompletionStatus` varchar(75) DEFAULT NULL,
                      `Score` float(11,2) DEFAULT NULL,
                      `CourseID` int(11) DEFAULT NULL,
                      `ClassID` int(11) DEFAULT NULL,
                      `LastAttemptDate` datetime DEFAULT NULL,
                      `ContentCode` varchar(2048) NOT NULL,
                      `TotalTimeSpent` varchar(50) DEFAULT NULL,
                      `ContentID` int(11) NOT NULL DEFAULT '0',
                      `LessonID` int(11) DEFAULT NULL,
                      `ContentVersion` int(11) NOT NULL,
		          	`operation` varchar(10) NULL DEFAULT NULL,
			          PRIMARY KEY (`RUID`),
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
						concat_ws('-', attend.id, detail.id) AS `RUID`,
						`attend`.`user_id` AS `UserID`,
						`attend`.`enrollment_id` AS `EnrollmentID`,
						`cmap`.`max_attempts`  AS `MaxAttempts`,
						`detail`.`attempt_number` AS `AttemptNumber`,
						`spli`.`name` AS `AttemptStatus`,
						`detail`.`attempt_date` AS `AttemptDate`,
						`detail`.`time_spend` AS `TimeSpent`,
						`attend`.`total_attempts` AS `TotalAttempts`,
						`attend`.`suspend_data` AS `SuspendData`,
						`detail`.`content_status` AS `ContentStatus`,
						`detail`.`completion_status` AS `CompletionStatus`,
						`detail`.`score` AS `Score`,
						`attend`.`course_id` AS `CourseID`,
						`attend`.`class_id` AS `ClassID`,
						`attend`.`last_attempt_date` AS `LastAttemptDate`,
						`cmas`.`code` AS `ContentCode`,
						`attend`.`total_time_spend` AS `TotalTimeSpent`,
						`cmas`.`id` AS ContentID, 
						`attend`.`lesson_id` AS `LessonID`,
						`vers`.`version` AS `ContentVersion`,
			        	NULL
			        
					FROM slt_attendance_summary attend
						INNER JOIN slt_content_version vers ON attend.content_version_id = vers.id
						INNER JOIN slt_content_master cmas ON cmas.id = vers.content_master_id
						LEFT JOIN slt_course_content_mapper cmap ON cmap.content_id = cmas.id AND cmap.class_id=attend.class_id
						LEFT JOIN slt_attendance_details detail ON attend.enrollment_id = detail.enrollment_id AND detail.course_id=attend.course_id AND detail.class_id=attend.class_id AND detail.user_id=attend.user_id  AND detail.lesson_id=attend.lesson_id 
                        LEFT JOIN slt_profile_list_items spli on spli.code=attend.status
					
			        where ((detail.updated_on > '".$this->updSync_on ."') 
			        			OR (attend.updated_on > '".$this->updSync_on ."') 
			        			OR (vers.updated_on > '".$this->updSync_on ."') 
								OR (cmas.updated_on > '".$this->updSync_on ."') 
								OR (cmap.updated_on > '".$this->updSync_on ."'))";
			
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
						`UserID`,
						`EnrollmentID`,
						`MaxAttempts`,
						`AttemptNumber`,
						`AttemptStatus`,
						`AttemptDate`,
						`TimeSpent`,
						`TotalAttempts`,
						`SuspendData`,
						`ContentStatus`,
						`CompletionStatus`,
						`Score`,
						`CourseID`,
						`ClassID`,
						`LastAttemptDate`,
						`ContentCode`,
						`TotalTimeSpent`,
						`ContentID`,
						`LessonID`,
						`ContentVersion`
					
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
							`flat`.`UserID`	=	`temp`.`UserID`,
							`flat`.`EnrollmentID`	=	`temp`.`EnrollmentID`,
							`flat`.`MaxAttempts`	=	`temp`.`MaxAttempts`,
							`flat`.`AttemptNumber`	=	`temp`.`AttemptNumber`,
							`flat`.`AttemptStatus`	=	`temp`.`AttemptStatus`,
							`flat`.`AttemptDate`	=	`temp`.`AttemptDate`,
							`flat`.`TimeSpent`	=	`temp`.`TimeSpent`,
							`flat`.`TotalAttempts`	=	`temp`.`TotalAttempts`,
							`flat`.`SuspendData`	=	`temp`.`SuspendData`,
							`flat`.`ContentStatus`	=	`temp`.`ContentStatus`,
							`flat`.`CompletionStatus`	=	`temp`.`CompletionStatus`,
							`flat`.`Score`	=	`temp`.`Score`,
							`flat`.`CourseID`	=	`temp`.`CourseID`,
							`flat`.`ClassID`	=	`temp`.`ClassID`,
							`flat`.`LastAttemptDate`	=	`temp`.`LastAttemptDate`,
							`flat`.`ContentCode`	=	`temp`.`ContentCode`,
							`flat`.`TotalTimeSpent`	=	`temp`.`TotalTimeSpent`,
							`flat`.`ContentID`	=	`temp`.`ContentID`,
							`flat`.`LessonID`	=	`temp`.`LessonID`,
							`flat`.`ContentVersion`	=	`temp`.`ContentVersion`,    
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
		 		DISTINCT concat_ws('-', attend.id, detail.id) AS `RUID`
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
			
			return $query;
			
		} catch(Exception $ex){
			throw new Exception("Error in reportCollectIds ".$ex->getMessage());
		}	
	}
} 
	
	
