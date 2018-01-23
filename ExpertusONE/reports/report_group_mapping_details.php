<?php
include_once $_SERVER['DOCUMENT_ROOT'] .  '/reports/ReportSyncUp.php';

class report_group_mapping_details extends ReportSyncUp {
	
	
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
			$createTable = "CREATE TABLE ".$this->temp_table_name ." (
                      `RUID` VARCHAR(255),
                      `entityId` int(11) NOT NULL DEFAULT '0',
                      `entityType` longtext,
                      `mro` longtext,
                      `type` varchar(5) NOT NULL DEFAULT '',
                      `grpId` int(11) NOT NULL DEFAULT '0',
                      `grpName` varchar(255) DEFAULT NULL,
                      `grpType` int(11) DEFAULT NULL,
                      `entityName` longtext,
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
						concat_ws('-',`map`.`id`,'Group') as `RUID`,
                        `map`.`entity_id` AS `entityId`,
                        `pli`.`name` AS `entityType`,
                        `mro`.`name` AS `mro`,
                        'Group' AS `Type`,
                        `grp`.`id` AS `grpId`,
                        `grp`.`name` AS `grpName`,
                        `grp`.`is_admin` AS `grpType`,
                        (CASE `pli`.`name` 
												  WHEN 'Course' THEN (SELECT title FROM slt_course_template where id = `map`.`entity_id`) 
												  WHEN 'Class' THEN (SELECT title FROM slt_course_class where id = `map`.`entity_id`)
												  WHEN 'Curricula' THEN (SELECT title FROM slt_program where id = `map`.`entity_id`) 
												  WHEN 'Certification' THEN (SELECT title FROM slt_program where id = `map`.`entity_id`)
												  WHEN 'Learning Plan' THEN (SELECT title FROM slt_program where id = `map`.`entity_id`)
												  WHEN 'User' THEN (SELECT full_name FROM slt_person where id = `map`.`entity_id`)
												  WHEN 'Organization' THEN (SELECT name FROM slt_organization where id = `map`.`entity_id`)
												  WHEN 'Survey' THEN (SELECT title FROM  slt_survey where id = `map`.`entity_id`)
												  WHEN 'Assessment' THEN (SELECT title FROM slt_survey where id = `map`.`entity_id`)
												  WHEN 'Content' THEN (SELECT description FROM slt_content_master where id = `map`.`entity_id`)
												  WHEN 'Discount' THEN (SELECT discount_name FROM slt_discounts where id = `map`.`entity_id`)
												  WHEN 'Security' THEN (SELECT name FROM slt_groups where id = `map`.`entity_id`) 
												  ELSE '' END ) AS `entityName`,
			             NULL
			        
                    from
                        (((`slt_group_mapping` `map`
                        join `slt_groups` `grp` ON (((`grp`.`id` = `map`.`group_id`)
                        and (`grp`.`status` <> 'cre_sec_sts_del'))))
                        join `slt_profile_list_items` `pli` ON ((`pli`.`code` = `map`.`entity_type`)))
                        left join `slt_profile_list_items` `mro` ON ((`mro`.`code` = `map`.`mro`))) 
					where (`grp`.updated_on > '".$this->updSync_on ."' OR map.updated_on > '".$this->updSync_on ."')
                    union all select 
	                   concat_ws('-',`umap`.`id`,'User') as `RUID`,
                       `umap`.`entity_id` AS `entityId`,
                       `pli`.`name` AS `entityType`,
                       `mro`.`name` AS `mro`,
                       'User' AS `Type`,
                       `per`.`id` AS `grpId`,
                       `per`.`user_name` AS `grpName`,
                       `umap`.`access_type` AS `grpType`,
                       (SELECT full_name FROM slt_person where id = `umap`.`entity_id`) AS `entityName`,
						NULL
                    from
                        (((`slt_user_access_mapping` `umap`
                        join `slt_person` `per` ON (((`per`.`id` = `umap`.`user_id`)
                        and (`per`.`status` <> 'cre_usr_sts_del'))))
                        join `slt_profile_list_items` `pli` ON ((`pli`.`code` = `umap`.`entity_type`)))
                        left join `slt_profile_list_items` `mro` ON ((`mro`.`code` = `umap`.`mro`)))
					 where (`per`.updated_on > '".$this->updSync_on ."' OR umap.updated_on > '".$this->updSync_on ."')";
			
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
						`entityId`,
						`entityType`,
						`mro`,
						`type`,
						`grpId`,
						`grpName`,
						`grpType`,
						`entityName`
			        
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
							`flat`.`entityId`	=	`temp`.`entityId`,
							`flat`.`entityType`	=	`temp`.`entityType`,
							`flat`.`mro`	    =	`temp`.`mro`,
							`flat`.`type`	    =	`temp`.`type`,
							`flat`.`grpId`   	=	`temp`.`grpId`,
							`flat`.`grpName`	=	`temp`.`grpName`,
							`flat`.`grpType`	=	`temp`.`grpType`,
							`flat`.`entityName`	=	`temp`.`entityName`,
							
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
					concat_ws('-',`logs`.`entity_id`,'Group') as `RUID`
					from
					`report_deleted_logs` `logs`
					where `logs`.`table_name` = 'slt_group_mapping' and `logs`.deleted_on > '".$this->delSync_on ."'
					union all 
					select 
					concat_ws('-',`logs`.`entity_id`, 'User') as `RUID`
					from
					`report_deleted_logs` `logs`
					where `logs`.`table_name` = 'slt_user_access_mapping' and `logs`.deleted_on > '".$this->delSync_on ."'";
			
			return $query;
			
		} catch(Exception $ex){
			throw new Exception("Error in reportCollectIds ".$ex->getMessage());
		}	
	}
} 
	
	
