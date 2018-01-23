<?php
include_once $_SERVER['DOCUMENT_ROOT'] . '/reports/ReportSyncUp.php';

/**
 * report_user_points_summary
 *
 * Table Informations
 * slt_person usr - Soft Delete
 * slt_user_points up - Hard Delete
 */
class report_user_points_summary extends ReportSyncUp {
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
			if(! isset($end)) {
				$end = date("Y-m-d H:i:s");
			}
			$this->dropTempTable(); // drop temp table
			$this->write_log($this->report_table, 'temp', 'FL', $this->updSync_on, $start, $end, $ex->getMessage());
			throw new Exception("Error in reportExecute " . $ex->getMessage());
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
			
			$createTable = "CREATE TABLE " . $this->temp_table_name . " LIKE " . $this->report_table . "";
			$this->db->callQuery($createTable);
			
			$alterTable = "ALTER TABLE " . $this->temp_table_name . " ADD COLUMN `operation` varchar(10) NULL DEFAULT NULL;";
			$this->db->callQuery($alterTable);
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
			$tempDataQuery = "insert into " . $this->temp_table_name . "
					SELECT
					   usr.id as 'RUID',
					   usr.id as 'UserID',
					   usr.full_name as 'FullName',
					   usr.user_name as 'UserName',
			           (select name from slt_organization  where id = usr.org_id) as `OrganizationName`,
			           (select group_concat(spli.name) from slt_person_jobrole_mapping jobrole JOIN slt_profile_list_items spli ON spli.code = jobrole.job_role WHERE jobrole.user_id = usr.id) as `JobRole`,
			           (SELECT SUM(total_points) FROM slt_user_points WHERE user_id=`usr`.`id`) AS `TotalPoints`,
			           (SELECT SUM(total_points) FROM slt_user_points WHERE action_code LIKE 'register_%' AND user_id=`usr`.`id`) AS `RegistrationPoints`,
			           (SELECT SUM(total_points) FROM slt_user_points WHERE action_code LIKE 'complete_%' AND user_id=`usr`.`id`) AS `CompletionPoints`,
			           (SELECT SUM(total_points) FROM slt_user_points WHERE action_code LIKE 'rating_%' AND user_id=`usr`.`id`) AS `VotePoints`,
			           (SELECT SUM(total_points) FROM slt_user_points WHERE action_code LIKE 'shar_%' AND user_id=`usr`.`id`) AS `SharePoints`,
			           (SELECT SUM(total_points) FROM slt_user_points WHERE action_code LIKE 'add_topic_%' AND user_id=`usr`.`id`) AS `CommentPoints`,
					   (SELECT COUNT(user_id) FROM slt_user_points WHERE user_id = `usr`.`id`) AS `PointsCount`,
			           NULL
			        
			        from  slt_person usr ";
			if($this->updSync_on != null) {
				$tempDataQuery .= " where ((usr.updated_on > '" . $this->updSync_on . "')
					   			OR (usr.id IN (select distinct user_id from slt_user_points sup where sup.updated_on > '" . $this->updSync_on . "'))
					   		 )";
			}
			
			// OR (enr.updated_on > '".$this->updSync_on ."') OR (menr.updated_on > '".$this->updSync_on ."')
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
			if($this->flush == 'true') {
				$flushTable = "TRUNCATE TABLE " . $this->report_table . "; ";
				expDebug::dPrint('$flushTable query :' . $flushTable, 4);
				$this->db->callQuery($flushTable);
			}
			
			$query = "insert into " . $this->report_table . "
					SELECT 
						`RUID`,
						`UserID`,
						`FullName`,
						`UserName`,
						`OrganizationName`,
						`JobRole`,
						`TotalPoints`,
						`RegistrationPoints`,
						`CompletionPoints`,
						`VotePoints`,
						`SharePoints`,
						`CommentPoints`,
						`PointsCount`					
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
							`flat`.`UserID` 			=	`temp`.`UserID`,
							`flat`.`FullName` 			=	`temp`.`FullName`,
							`flat`.`UserName` 			=	`temp`.`UserName`,
							`flat`.`OrganizationName`	=	`temp`.`OrganizationName`,
							`flat`.`JobRole` 			=	`temp`.`JobRole`,
							`flat`.`TotalPoints` 		=	`temp`.`TotalPoints`,
							`flat`.`RegistrationPoints` =	`temp`.`RegistrationPoints`,
							`flat`.`CompletionPoints` 	=	`temp`.`CompletionPoints`,
							`flat`.`VotePoints` 		=	`temp`.`VotePoints`,
							`flat`.`SharePoints` 		=	`temp`.`SharePoints`,
							`flat`.`CommentPoints` 		=	`temp`.`CommentPoints`,
							`flat`.`PointsCount` 		=	`temp`.`PointsCount`,
						        
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
			$query = "SELECT usr.id as RUID from slt_person usr
					WHERE usr.updated_on > '" . $this->delSync_on . "'
						UNION
					SELECT pts.user_id as RUID from slt_user_points pts
					WHERE pts.updated_on > '" . $this->delSync_on . "'";
			expDebug::dPrint('query to delete invalid RUID ' . $query, 2);
			return $query;
		} catch(Exception $ex) {
			throw new Exception("Error in reportCollectIds " . $ex->getMessage());
		}
	}
} 
	
	
