<?php
include_once $_SERVER['DOCUMENT_ROOT'] .  '/reports/ReportSyncUp.php';

class report_group_users extends ReportSyncUp {
	
	
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
			
			// $createTable = "CREATE TABLE ".$this->temp_table_name." LIKE ".$this->report_table."";
			$createTable = "CREATE TABLE ".$this->temp_table_name." (
						    `RUID` varchar(255),
						    `GroupID` int(11) NOT NULL DEFAULT '0',
						    `GroupCode` varchar(15) NOT NULL,
						    `GroupName` varchar(100) NOT NULL,
						    `GroupType` varchar(100) NOT NULL,
						    `GroupStatusCode` char(15) NOT NULL,
						    `UserName` varchar(100) DEFAULT NULL,
						    `UserID` int(11) DEFAULT NULL,
						    `UserType` varchar(100) DEFAULT NULL,
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
					select
			concat_ws('-',grp.id,per.id) AS RUID,
			grp.id as GroupID,
			grp.code as GroupCode,
			grp.name as GroupName,
			IF(grp.is_admin = 1,'Admin','Learner') AS GroupType,
			grp.status as GroupStatusCode,
			per.full_name AS `UserName`,
			per.id as `UserID`,
			(case when (map.user_type = 'A')
			THEN 'Added User'
			When (map.user_type = 'R')
			THEN 'Removed User'
			when (map.user_type = 'M')
			THEN 'Criteria User'
			ELSE
			'NULL'
			END
			) AS `UserType`,
			NULL
			from 
			slt_groups grp
			left join slt_group_user_mapping map on map.group_id = grp.id 
			left join slt_person per on per.id = map.user_id
					where ((grp.updated_on > '".$this->updSync_on ."') OR (per.updated_on > '".$this->updSync_on ."'))";
			
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
						`GroupID`,
						`GroupCode`,
						`GroupName`,
						`GroupType`,
						`GroupStatusCode`,
						`UserName`,
						`UserID`,
						`UserType`
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
							`flat`.`GroupID`	        =	`temp`.`GroupID`,
							`flat`.`GroupCode`	        =	`temp`.`GroupCode`,
							`flat`.`GroupName`	        =	`temp`.`GroupName`,
							`flat`.`GroupType`       	=	`temp`.`GroupType`,
							`flat`.`GroupStatusCode`	=	`temp`.`GroupStatusCode`,
							`flat`.`UserName`	        =	`temp`.`UserName`,
							`flat`.`UserID`	            =	`temp`.`UserID`,
							`flat`.`UserType`	        =	`temp`.`UserType`,
						        
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
						concat_ws('-', `grp`.`id`, map.user_id) as RUID
						FROM slt_groups grp 
						left join slt_group_user_mapping map on map.group_id = grp.id
						where grp.updated_on > '".$this->delSync_on ."' and grp.status = 'cre_sec_sts_del' 
						UNION
						select 
						concat_ws('-', `del_grp`.`parent2_entity_id`, `del_grp`.`parent3_entity_id` )  as RUID
						FROM slt_groups grp1
						LEFT JOIN `report_deleted_logs` del_grp ON del_grp.parent2_entity_id = grp1.id and del_grp.table_name = 'slt_group_user_mapping'
						where 
						del_grp.deleted_on > '".$this->delSync_on ."' 
						UNION
						SELECT 
						concat_ws('-', `map`.`group_id`, map.user_id) as RUID
						FROM slt_person per 
						inner join slt_group_user_mapping map on map.user_id = per.id
						where per.updated_on > '".$this->delSync_on ."' and per.status != 'cre_usr_sts_atv' ";
			
			/* If ($action == 'delete') {
				$query .= " where (del_mdl.id is not null AND del_mdl.deleted_on > '".$this->delSync_on ."')
				 			OR (del_crp_map.id is not null AND del_crp_map.deleted_on > '".$this->delSync_on ."')
				 			OR (prm.`status` = 'lrn_lpn_sts_del' and prm.updated_on > '".$this->delSync_on ."')";
			} */
			
			return $query;
			
		} catch(Exception $ex){
			throw new Exception("Error in reportCollectIds ".$ex->getMessage());
		}	
	}
} 
	
	
