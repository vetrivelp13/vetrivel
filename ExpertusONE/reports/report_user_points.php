<?php
include_once $_SERVER['DOCUMENT_ROOT'] .  '/reports/ReportSyncUp.php';

/**
 * report_user_points
 * 
 * 
 * Table Informations
 * slt_person usr					- Soft Delete				 
 * slt_user_points up 				- Hard Delete
 * slt_enrollment enr 				- Hard Delete
 * slt_master_enrollment menr 		- Hard Delete
 * slt_course_class cls  			- Soft Delete 
 * slt_program prg 					- Soft Delete 
 *
 */

class report_user_points extends ReportSyncUp {
	
	
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
			// $this->reportDelete(); // no need to run the delete record bcz here we need to show deleted data also.
			
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
			
			// $createTable = "CREATE TABLE ".$this->temp_table_name." LIKE ".$this->report_table."";
			$createTable = "CREATE TABLE ".$this->temp_table_name." (
						  `RUID` varchar(23) NOT NULL,
						  `UserID` int(11) NOT NULL,
						  `UserName` varchar(255) DEFAULT NULL,
						  `FirstName` varchar(255) DEFAULT NULL,
						  `LastName` varchar(255) DEFAULT NULL,
						  `FullName` varchar(255) DEFAULT NULL,
						  `UserPointId` int(11) DEFAULT NULL,
						  `UserPointsUserID` int(11) DEFAULT NULL,
						  `EntityID` int(11) DEFAULT NULL,
						  `EntityType` varchar(255) DEFAULT NULL,
						  `UserAction` varchar(255) DEFAULT NULL,
						  `Title` longtext,
						  `Code` varchar(255) DEFAULT NULL,
						  `Type` longtext,
						  `RegistrationDate` datetime DEFAULT NULL,
						  `CompletionDate` datetime DEFAULT NULL,
						  `Status` longtext,
						  `EarnedPoints` int(11) DEFAULT NULL,
						  `GamificationAction` varchar(23) DEFAULT NULL,
						  `GamificationPoints` bigint(11) NOT NULL DEFAULT '0',
						  `OrganizationName` longtext,
						  `JobRole` varchar(255),
						  `TotalPoints` int(11) DEFAULT NULL,
						  `RegistrationPoints` int(11) DEFAULT NULL,
						  `CompletionPoints` int(11) DEFAULT NULL,
						  `VotePoints` int(11) DEFAULT NULL,
						  `SharePoints` int(11) DEFAULT NULL,
						  `CommentPoints` int(11) DEFAULT NULL,
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
					   concat_ws('-',`usr`.`id`,`up`.`id`) as `RUID`,
					   usr.id as 'UserID',
					   usr.user_name as 'UserName',
					   usr.first_name as 'FirstName',
					   usr.last_name as 'LastName',
					   usr.full_name as 'FullName',
					   up.id AS 'UserPointId',
					   up.user_id AS 'UserPointsUserID',
					   up.entity_id AS 'EntityID',
					   (case up.entity_type when 'class' then 'Class' when 'tp' then 'Training Plan' end) AS 'EntityType',
					   up.action_code as 'UserAction',
					   if(up.entity_type = 'tp' , prg.title,cls.title) 'Title',
					   if(up.entity_type = 'tp' , prg.code,cls.code) 'Code',
					   IF(up.entity_type = 'tp' , prgdel.name, delty.name) 'Type',
					   if(up.entity_type = 'tp',menr.reg_date,enr.reg_date) 'RegistrationDate',
					   if(up.entity_type = 'tp',if(up.action_code = 'complete_class_training',menr.comp_date,NULL),if(up.action_code = 'complete_class_training',enr.comp_date,NULL)) 'CompletionDate',
					   if(up.entity_type = 'tp',if(up.action_code = 'complete_class_training',comstatus.name,'Enrolled'),if(enr.reg_status = 'lrn_crs_reg_cnf',if(enr.comp_status is not null AND up.action_code = 'complete_class_training',compst.name,'Enrolled') ,regst.name )) Status,
					   up.earned_points AS 'EarnedPoints',
					   CASE WHEN up.action_code='register_class' THEN IF(up.entity_type = 'tp','Registering for a TP','Registering for a Class') WHEN up.action_code='complete_class_training' THEN IF(up.entity_type = 'tp','Completing a TP','Completing a Class') WHEN up.action_code='rating_class' THEN 'Rating a class' WHEN up.action_code='sharing_class' THEN 'Sharing a class' END AS 'GamificationAction',
			           IFNULL(up.total_points,0) AS 'GamificationPoints',
			           (select name from slt_organization  where id = usr.org_id) as `OrganizationName`,
			           (select group_concat(spli.name) from slt_person_jobrole_mapping jobrole JOIN slt_profile_list_items spli ON spli.code = jobrole.job_role WHERE jobrole.user_id = usr.id) as `JobRole`,
			           (SELECT SUM(total_points) FROM slt_user_points WHERE user_id=`usr`.`id`) AS `TotalPoints`,
			           (SELECT SUM(total_points) FROM slt_user_points WHERE action_code LIKE 'register_%' AND user_id=`usr`.`id`) AS `RegistrationPoints`,
			           (SELECT SUM(total_points) FROM slt_user_points WHERE action_code LIKE 'complete_%' AND user_id=`usr`.`id`) AS `CompletionPoints`,
			           (SELECT SUM(total_points) FROM slt_user_points WHERE action_code LIKE 'rating_%' AND user_id=`usr`.`id`) AS `VotePoints`,
			           (SELECT SUM(total_points) FROM slt_user_points WHERE action_code LIKE 'shar_%' AND user_id=`usr`.`id`) AS `SharePoints`,
			           (SELECT SUM(total_points) FROM slt_user_points WHERE action_code LIKE 'add_topic_%' AND user_id=`usr`.`id`) AS `CommentPoints`,
			           NULL
			        
			        from  slt_person usr 
			           LEFT JOIN slt_user_points up on up.user_id = usr.id
			           LEFT JOIN slt_enrollment enr ON enr.user_id = up.user_id and up.entity_id = enr.id and enr.reg_status not in ('lrn_crs_reg_rsc','lrn_crs_reg_rsv') and enr.master_enrollment_id is null
			           LEFT JOIN slt_master_enrollment menr ON  menr.id = up.entity_id and menr.overall_status not in ('lrn_tpm_ovr_rsc','lrn_tpm_ovr_rsv')
			           LEFT JOIN slt_course_class cls ON cls.id=enr.class_id 
			           LEFT JOIN slt_program prg ON prg.id=menr.program_id 
			           LEFT JOIN slt_profile_list_items regst ON regst.code = enr.reg_status AND regst.lang_code = 'cre_sys_lng_eng'
			           LEFT JOIN slt_profile_list_items compst ON compst.code = enr.comp_status AND compst.lang_code = 'cre_sys_lng_eng'
			           LEFT JOIN slt_profile_list_items delty ON delty.code = cls.delivery_type
			           LEFT JOIN slt_profile_list_items comstatus ON comstatus.code = menr.overall_status AND comstatus.lang_code = 'cre_sys_lng_eng'
			           LEFT JOIN slt_profile_list_items prgdel ON prgdel.code = prg.object_type

					   where ((usr.updated_on > '".$this->updSync_on ."') 
					   			OR (usr.id IN (select distinct user_id from slt_user_points sup where sup.updated_on > '".$this->updSync_on ."')) 
					   			OR (cls.updated_on > '".$this->updSync_on ."')
					   			OR (prg.updated_on > '".$this->updSync_on ."')
					   		 )
					   AND up.id is not null";
			// OR (enr.updated_on > '".$this->updSync_on ."') OR (menr.updated_on > '".$this->updSync_on ."')
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
						`UserID` ,
						`UserName` ,
						`FirstName` ,
						`LastName` ,
						`FullName` ,
						`UserPointId` ,
						`UserPointsUserID` ,
						`EntityID`,
						`EntityType` ,
						`UserAction` ,
						`Title`,
						`Code` ,
						`Type`,
						`RegistrationDate`,
						`CompletionDate`,
						`Status`,
						`EarnedPoints` ,
						`GamificationAction`,
						`GamificationPoints` ,
						`OrganizationName`,
						`JobRole` ,
						`TotalPoints` ,
						`RegistrationPoints` ,
						`CompletionPoints` ,
						`VotePoints` ,
						`SharePoints` ,
						`CommentPoints` 
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
							`flat`.`UserID` 		=	`temp`.`UserID` ,
							`flat`.`UserName` 		=	`temp`.`UserName` ,
							`flat`.`FirstName` 		=	`temp`.`FirstName` ,
							`flat`.`LastName` 		=	`temp`.`LastName` ,
							`flat`.`FullName` 		=	`temp`.`FullName` ,
							`flat`.`UserPointId` 	=	`temp`.`UserPointId` ,
							`flat`.`UserPointsUserID` 	=	`temp`.`UserPointsUserID` ,
							`flat`.`EntityID`	=	`temp`.`EntityID`,
							`flat`.`EntityType` 	=	`temp`.`EntityType` ,
							`flat`.`UserAction` 	=	`temp`.`UserAction` ,
							`flat`.`Title`	=	`temp`.`Title`,
							`flat`.`Code` 	=	`temp`.`Code` ,
							`flat`.`Type`	=	`temp`.`Type`,
							`flat`.`RegistrationDate`	=	`temp`.`RegistrationDate`,
							`flat`.`CompletionDate`	=	`temp`.`CompletionDate`,
							`flat`.`Status`	=	`temp`.`Status`,
							`flat`.`EarnedPoints` 	=	`temp`.`EarnedPoints` ,
							`flat`.`GamificationAction`	=	`temp`.`GamificationAction`,
							`flat`.`GamificationPoints` 	=	`temp`.`GamificationPoints` ,
							`flat`.`OrganizationName`	=	`temp`.`OrganizationName`,
							`flat`.`JobRole` 	=	`temp`.`JobRole` ,
							`flat`.`TotalPoints` 	=	`temp`.`TotalPoints` ,
							`flat`.`RegistrationPoints` 	=	`temp`.`RegistrationPoints` ,
							`flat`.`CompletionPoints` 	=	`temp`.`CompletionPoints` ,
							`flat`.`VotePoints` 	=	`temp`.`VotePoints` ,
							`flat`.`SharePoints` 	=	`temp`.`SharePoints` ,
							`flat`.`CommentPoints` 	=	`temp`.`CommentPoints` ,
						        
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
					   concat_ws('-',`usr`.`id`,`up`.`id`) as `RUID`
					  from  slt_person usr 
			           LEFT JOIN slt_user_points up on up.user_id = usr.id
			           LEFT JOIN slt_enrollment enr ON enr.user_id = up.user_id and up.entity_id = enr.id and enr.reg_status in ('lrn_crs_reg_rsc','lrn_crs_reg_rsv') and enr.master_enrollment_id is null
			           LEFT JOIN slt_master_enrollment menr ON  menr.id = up.entity_id and menr.overall_status in ('lrn_tpm_ovr_rsc','lrn_tpm_ovr_rsv')
			           LEFT JOIN slt_course_class cls ON cls.id=enr.class_id  and cls.status in('lrn_cls_sts_del') 
			           LEFT JOIN slt_program prg ON prg.id=menr.program_id and prg.status in('lrn_lpn_sts_del')  ";

			If ($action == 'delete') {
				$query .= " where ((enr.updated_on > '".$this->delSync_on ."')
						OR (menr.updated_on > '".$this->delSync_on ."')
						OR (cls.updated_on > '".$this->delSync_on ."')
						OR (prg.updated_on > '".$this->delSync_on ."'))";
			}
			
			return $query;
			
		} catch(Exception $ex){
			throw new Exception("Error in reportCollectIds ".$ex->getMessage());
		}	
	}
} 
	
	
