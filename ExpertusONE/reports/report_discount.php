<?php
include_once $_SERVER['DOCUMENT_ROOT'] .  '/reports/ReportSyncUp.php';

class report_discount extends ReportSyncUp {
	
	
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
			$createTable = "CREATE TABLE ".$this->temp_table_name ." (
			        `RUID` varchar(255),
                      `DiscountId` int(11) NOT NULL DEFAULT '0',
                      `DiscountName` varchar(255) NOT NULL DEFAULT '',
                      `DiscountDescription` varchar(1000) NOT NULL DEFAULT '',
                      `DiscountCode` varchar(100) NOT NULL DEFAULT '',
                      `DiscountPrice` float NOT NULL DEFAULT '0' COMMENT 'Minimum quantity or price required to qualify for this discount.',
                      `DiscountType` varchar(16) NOT NULL DEFAULT '',
                      `DiscountMaxUsers` int(11) NOT NULL DEFAULT '0' COMMENT 'Number of times this discount can be applied (0 for no limit).',
                      `DiscountMaxUsersPerUser` int(11) NOT NULL DEFAULT '1' COMMENT 'Number of times this discount can be applied to a particular user (0 for unlimited).',
                      `DiscountMaxUsersPerCode` int(11) NOT NULL DEFAULT '0' COMMENT 'Number of times this discount can be applied for a particular code (0 for unlimited).',
		          	`operation` varchar(10) NULL DEFAULT NULL,
			          PRIMARY KEY (`RUID`),
			             key sli_op(operation)
			)  ENGINE=InnoDB DEFAULT CHARSET=utf8;";
			$this->db->callQuery($createTable);
			
		//	$alterTable = "ALTER TABLE ".$this->temp_table_name." ADD COLUMN `operation` varchar(10) NULL DEFAULT NULL;";
		//	$this->db->callQuery($alterTable);
			
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
						concat_ws('-', dis.discount_id, discode.discount_code_id)  as `RUID`,
						`dis`.`discount_id` as `DiscountId`,
						`dis`.`name` as `DiscountName`,
						`dis`.`description` as `DiscountDescription`,
						`discode`.`code` as `DiscountCode`,
						`dis`.`qualifying_amount` as `DiscountPrice`,
						if(dis.discount_type = 2,'PERCENTAGE OFF','FIXED AMOUNT OFF') as `DiscountType`,
						`dis`.`max_uses` as `DiscountMaxUsers`,
						`dis`.`max_uses_per_user` as `DiscountMaxUsersPerUser`,
						`dis`.`max_uses_per_code` as `DiscountMaxUsersPerCode`,
			            NULL
					
			        FROM slt_discounts sltdis  
					     INNER JOIN uc_discounts dis ON dis.discount_id = sltdis.id
					     INNER JOIN uc_discounts_codes discode ON discode.discount_id = sltdis.id";
					
			if (!empty($this->updSync_on))		
				$tempDataQuery .=" where sltdis.updated_on >  '".$this->updSync_on ."' ";
			
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
						`DiscountId`,
						`DiscountName`,
						`DiscountDescription`,
						`DiscountCode`,
						`DiscountPrice`,
						`DiscountType`,
						`DiscountMaxUsers`,
						`DiscountMaxUsersPerUser`,
						`DiscountMaxUsersPerCode`
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
							`flat`.`DiscountId`	=	`temp`.`DiscountId`,
							`flat`.`DiscountName`	=	`temp`.`DiscountName`,
							`flat`.`DiscountDescription`	=	`temp`.`DiscountDescription`,
							`flat`.`DiscountCode`	=	`temp`.`DiscountCode`,
							`flat`.`DiscountPrice`	=	`temp`.`DiscountPrice`,
							`flat`.`DiscountType`	=	`temp`.`DiscountType`,
							`flat`.`DiscountMaxUsers`	=	`temp`.`DiscountMaxUsers`,
							`flat`.`DiscountMaxUsersPerUser`	=	`temp`.`DiscountMaxUsersPerUser`,
							`flat`.`DiscountMaxUsersPerCode`	=	`temp`.`DiscountMaxUsersPerCode`,
						        
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
						concat_ws('-', del.parent1_entity_id, del.entity_id)  as `RUID`
			        FROM slt_discounts sltdis  
					     INNER JOIN report_deleted_logs del ON del.parent1_entity_id = sltdis.uc_discount_id";
			
			If ($action == 'delete') {
				$query .= " where (del.id is not null AND del.deleted_on > '".$this->delSync_on ."')";
			}
			
			return $query;
			
		} catch(Exception $ex){
			throw new Exception("Error in reportCollectIds ".$ex->getMessage());
		}	
	}
} 
	
	
