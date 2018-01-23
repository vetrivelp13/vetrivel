<?php
include_once $_SERVER['DOCUMENT_ROOT'] .  '/reports/ReportSyncUp.php';

class report_tp_orders extends ReportSyncUp {
	
	
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
			//$this->reportDelete(); // no need to run the delete record bcz here there is no delete data are updated.
			
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
                      `CartOrderID` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Primary key: the order ID.',
                      `OrderID` int(11) NOT NULL DEFAULT '0',
                      `OrderTotal` decimal(16,5) NOT NULL DEFAULT '0.00000' COMMENT 'The total amount to be paid for the order.',
                      `OrderCurrencyType` varchar(4) NOT NULL COMMENT 'The currency type in which the product is purchased.',
                      `OrderPaymentMethod` varchar(32) NOT NULL DEFAULT '' COMMENT 'The method of payment.',
                      `OrderProductCount` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'The total product quantity of the order.',
                      `OrderBillingContactNumber` varchar(255) NOT NULL DEFAULT '' COMMENT 'The phone number for the billing address.',
                      `OrderBillingFirstName` varchar(255) NOT NULL DEFAULT '' COMMENT 'The first name of the person paying for the order.',
                      `OrderBillingLastName` varchar(255) NOT NULL DEFAULT '' COMMENT 'The last name of the person paying for the order.',
                      `OrderBillingStreet1` varchar(255) NOT NULL DEFAULT '' COMMENT 'The street address where the bill will be sent.',
                      `OrderBillingStreet2` varchar(255) NOT NULL DEFAULT '' COMMENT 'The second line of the street address.',
                      `OrderBillingCity` varchar(255) NOT NULL DEFAULT '' COMMENT 'The city where the bill will be sent.',
                      `OrderBillingZipCode` varchar(255) NOT NULL DEFAULT '' COMMENT 'The postal code where the bill will be sent.',
                      `OrderCreatedBy` int(11) NOT NULL DEFAULT '0' COMMENT 'The Unix timestamp indicating when the order was created.',
                      `OrderModifiedBy` int(11) NOT NULL DEFAULT '0' COMMENT 'The Unix timestamp indicating when the order was last modified.',
                      `OrderBillingZoneName` varchar(255) DEFAULT '' COMMENT 'The zone name.',
                      `OrderBillingCountryName` varchar(255) DEFAULT '' COMMENT 'The country name.',
                      `OrderDiscountAmount` decimal(16,5) DEFAULT '0.00000' COMMENT 'The amount of the line item in the store’s currency.',
                      `OrderDate` datetime DEFAULT NULL,
                      `OrderDateTime` datetime DEFAULT NULL,
                      `OrderTaxAmount` decimal(16,5) DEFAULT '0.00000' COMMENT 'The amount of the line item in the store’s currency.',
                      `OrderTotalAmount` decimal(16,5) DEFAULT '0.00000',
                      `OrderPONumber` varchar(255) DEFAULT '',
                      `OrderPrice` decimal(16,5) NOT NULL DEFAULT '0.00000' COMMENT 'The price paid for the ordered product.',
                      `OrderCourseTitle` varchar(255) NOT NULL DEFAULT '' COMMENT 'The product title, from node.title.',
                      `OrderDiscountCode` text COMMENT 'Newline delimited codes string for order.',
                      `OrderDiscountName` varchar(255) DEFAULT NULL,
                      `OrderRefundAmount` decimal(16,5) DEFAULT '0.00000',
                      `OrderStatus` longtext,
                      `OrderUserComments` text COMMENT 'The comment body.',
                      `OrderAdminComments` text COMMENT 'The comment body.',
                      `TrainingPlanTitle` varchar(255) DEFAULT NULL,
                      `TrainingPlanCode` varchar(100) NOT NULL,
                      `TrainingPlanObjectName` longtext,
                      `TPOverallStatus` longtext,
                      `TPStatus` longtext,
                      `FullName` varchar(255) DEFAULT NULL,
                      `UserName` varchar(255) DEFAULT NULL,
                      `Email` varchar(255) DEFAULT NULL,
                      `OrganizationName` longtext,
                      `Address1` varchar(255) DEFAULT NULL,
                      `Address2` varchar(255) DEFAULT NULL,
                      `City` varchar(255) DEFAULT NULL,
                      `ZipCode` varchar(50) DEFAULT NULL,
                      `Country` varchar(255) DEFAULT NULL,
                      `State` varchar(255) DEFAULT NULL,
                    	`OrdersItemType` varchar(32),
                    	`OrdersTaxType` varchar(32),
                    	`NactEntityType` varchar(100) DEFAULT NULL,
                    	`OrderProductID` int(10) unsigned NOT NULL,
                    	`OrdOrderStatus` varchar(255) DEFAULT NULL,
                    	`OrderCustomAttribute0` VARCHAR(750) DEFAULT NULL,
                        `OrderCustomAttribute1` VARCHAR(750) DEFAULT NULL,
                        `OrderCustomAttribute2` VARCHAR(750) DEFAULT NULL,
                        `OrderCustomAttribute3` VARCHAR(750) DEFAULT NULL,
                        `OrderCustomAttribute4` VARCHAR(750) DEFAULT NULL,
                        `OrderCustomAttribute5` VARCHAR(750) DEFAULT NULL,
                        `OrderCustomAttribute6` VARCHAR(750) DEFAULT NULL,
                        `OrderCustomAttribute7` VARCHAR(750) DEFAULT NULL,
                        `OrderCustomAttribute8` VARCHAR(750) DEFAULT NULL,
                        `OrderCustomAttribute9` VARCHAR(750) DEFAULT NULL, 
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
						concat_ws('-',`ord`.`id`,`menr`.`id`,`prd`.`order_product_id`,`orditem`.`id`,`ordersitem`.`line_item_id`) AS `RUID`,
                        `ucorder`.`order_id` AS `CartOrderID`,
                        `ord`.`id` AS `OrderID`,
                        `ucorder`.`order_total` AS `OrderTotal`,
                        `ucorder`.`currency_type` as `OrderCurrencyType`,
                        `ucorder`.`payment_method` AS `OrderPaymentMethod`,
                        `ucorder`.`product_count` AS `OrderProductCount`,
                        `ucorder`.`billing_phone` AS `OrderBillingContactNumber`,
                        `ucorder`.`billing_first_name` AS `OrderBillingFirstName`,
                        `ucorder`.`billing_last_name` AS `OrderBillingLastName`,
                        `ucorder`.`billing_street1` AS `OrderBillingStreet1`,
                        `ucorder`.`billing_street2` AS `OrderBillingStreet2`,
                        `ucorder`.`billing_city` AS `OrderBillingCity`,
                        `ucorder`.`billing_postal_code` AS `OrderBillingZipCode`,
                        `ucorder`.`created` AS `OrderCreatedBy`,
                        `ucorder`.`modified` AS `OrderModifiedBy`,
                        `zone`.`zone_name` AS `OrderBillingZoneName`,
                        `con`.`country_name` AS `OrderBillingCountryName`,
                        `ordersitem`.`amount` AS `OrderDiscountAmount`,
                        `ord`.`created_on` AS `OrderDate`,
                        `ord`.`order_date_time` AS `OrderDateTime`,
                        `ordertax`.`amount` AS `OrderTaxAmount`,
                        `ord`.`order_total_amt` AS `OrderTotalAmount`,
                        `po`.`po_number` AS `OrderPONumber`,
                        `prd`.`price` AS `OrderPrice`,
                        `prd`.`title` AS `OrderCourseTitle`,
                        `udordcode`.`codes` AS `OrderDiscountCode`,
                        `sltdis`.`discount_name` AS `OrderDiscountName`,
                        `orditem`.`refund_amount` AS `OrderRefundAmount`,
                        `ordsts`.`name` AS `OrderStatus`,
                        `ocmt`.`message` AS `OrderUserComments`,
                        `ucadcom`.`message` AS `OrderAdminComments`,
                        `prg`.`title` AS `TrainingPlanTitle`,
                        `prg`.`code` AS `TrainingPlanCode`,
                        `prgdel`.`name` AS `TrainingPlanObjectName`,
                        `ovrsts`.`name` AS `TPOverallStatus`,
                        `prgsts`.`name` AS `TPStatus`,
                        `per`.`full_name` AS `FullName`,
                        `per`.`user_name` AS `UserName`,
                        `per`.`email` AS `Email`,
                        `org`.`name` AS `OrganizationName`,
                        `per`.`addr1` AS `Address1`,
                        `per`.`addr2` AS `Address2`,
                        `per`.`city` AS `City`,
                        `per`.`zip` AS `ZipCode`,
                        `country`.`country_name` AS `Country`,
                        `sta`.`state_name` AS `State`,
                        `ordersitem`.`type` AS `OrdersItemType`,
                        `ordertax`.`type` AS `OrdersTaxType`,
                        `nact`.`entity_type` AS `NactEntityType`,
                        `prd`.`order_product_id` AS `OrderProductID`,
                        `ord`.`order_status` AS `OrdOrderStatus`,
                        IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt0`.`opt_name`) FROM slt_custom_attr_options `opt0` WHERE FIND_IN_SET(`opt0`.`opt_code`,`ord`.`e1_cattr0`)),`ord`.`e1_cattr0`),`ord`.`e1_cattr0`) AS `OrderCustomAttribute0`,
                        IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt1`.`opt_name`) FROM slt_custom_attr_options `opt1` WHERE FIND_IN_SET(`opt1`.`opt_code`,`ord`.`e1_cattr1`)),`ord`.`e1_cattr1`),`ord`.`e1_cattr1`) AS `OrderCustomAttribute1`,
                        IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt2`.`opt_name`) FROM slt_custom_attr_options `opt2` WHERE FIND_IN_SET(`opt2`.`opt_code`,`ord`.`e1_cattr2`)),`ord`.`e1_cattr2`),`ord`.`e1_cattr2`) AS `OrderCustomAttribute2`,
                        IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt3`.`opt_name`) FROM slt_custom_attr_options `opt3` WHERE FIND_IN_SET(`opt3`.`opt_code`,`ord`.`e1_cattr3`)),`ord`.`e1_cattr3`),`ord`.`e1_cattr3`) AS `OrderCustomAttribute3`,
                        IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt4`.`opt_name`) FROM slt_custom_attr_options `opt4` WHERE FIND_IN_SET(`opt4`.`opt_code`,`ord`.`e1_cattr4`)),`ord`.`e1_cattr4`),`ord`.`e1_cattr4`) AS `OrderCustomAttribute4`,
                        IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt5`.`opt_name`) FROM slt_custom_attr_options `opt5` WHERE FIND_IN_SET(`opt5`.`opt_code`,`ord`.`e1_cattr5`)),`ord`.`e1_cattr5`),`ord`.`e1_cattr5`) AS `OrderCustomAttribute5`,
                        IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt6`.`opt_name`) FROM slt_custom_attr_options `opt6` WHERE FIND_IN_SET(`opt6`.`opt_code`,`ord`.`e1_cattr6`)),`ord`.`e1_cattr6`),`ord`.`e1_cattr6`) AS `OrderCustomAttribute6`,
                        IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt7`.`opt_name`) FROM slt_custom_attr_options `opt7` WHERE FIND_IN_SET(`opt7`.`opt_code`,`ord`.`e1_cattr7`)),`ord`.`e1_cattr7`),`ord`.`e1_cattr7`) AS `OrderCustomAttribute7`,
                        IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt8`.`opt_name`) FROM slt_custom_attr_options `opt8` WHERE FIND_IN_SET(`opt8`.`opt_code`,`ord`.`e1_cattr8`)),`ord`.`e1_cattr8`),`ord`.`e1_cattr8`) AS `OrderCustomAttribute8`,
                        IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt9`.`opt_name`) FROM slt_custom_attr_options `opt9` WHERE FIND_IN_SET(`opt9`.`opt_code`,`ord`.`e1_cattr9`)),`ord`.`e1_cattr9`),`ord`.`e1_cattr9`) AS `OrderCustomAttribute9`,
			             NULL

			        FROM `slt_order` `ord`
                        INNER JOIN `slt_profile_list_items` `ordsts` ON `ordsts`.`code` = `ord`.`order_status`
                        INNER JOIN `uc_orders` `ucorder` ON `ucorder`.`order_id` = `ord`.`uc_order_id` AND `ucorder`.`order_status` != 'in_checkout'
                        LEFT JOIN `uc_countries` `con` ON `con`.`country_id` = `ucorder`.`billing_country`
                        LEFT JOIN `uc_zones` `zone` ON `zone`.`zone_country_id` = `con`.`country_id` AND `zone`.`zone_id` = `ucorder`.`billing_zone`
                        INNER JOIN `slt_order_items` `orditem` ON `orditem`.`order_id` = `ord`.`id`
                        INNER JOIN `slt_master_enrollment` `menr` ON `menr`.`order_id` = `ord`.`id` and menr.program_id=orditem.program_id
                        INNER JOIN `slt_program` `prg` ON `prg`.`id` = `menr`.`program_id`
                        INNER JOIN `slt_person` `per` ON `per`.`id` = `menr`.`user_id`
                        INNER JOIN `slt_profile_list_items` `ovrsts` ON `ovrsts`.`code` = `menr`.`overall_status`
                        INNER JOIN `slt_profile_list_items` `prgsts` ON `prgsts`.`code` = `prg`.`status`
                        INNER JOIN `slt_profile_list_items` `prgdel` ON `prgdel`.`code` = `prg`.`object_type`
                        LEFT JOIN `slt_organization` `org` ON `org`.`id` = `per`.`org_id`
                        LEFT JOIN `slt_country` `country` ON `country`.`country_code` = `per`.`country`
                        LEFT JOIN `slt_state` `sta` ON `sta`.`state_code` = `per`.`state` AND `sta`.`country_code` = `country`.`country_code`
                        LEFT JOIN `uc_order_line_items` `ordersitem` ON `ordersitem`.`order_id` = `ucorder`.`order_id` and `ordersitem`.`type` = 'uc_discounts'
                        LEFT JOIN `uc_order_line_items` `ordertax` ON `ordertax`.`order_id` = `ucorder`.`order_id` and `ordertax`.`type` = 'cybersource_tax'
                        LEFT JOIN `uc_payment_po` `po` ON `ucorder`.`order_id` = `po`.`order_id` 
                        LEFT JOIN `uc_discounts_order_codes` `udordcode` ON `udordcode`.`order_id` = `ucorder`.`order_id`
                        LEFT JOIN `uc_discounts_codes` `udcode` ON `udcode`.`code` = `udordcode`.`codes`
                        LEFT JOIN `slt_discounts` `sltdis` ON `sltdis`.`uc_discount_id` = `udcode`.`discount_id` 
                        LEFT JOIN `uc_order_comments` `ocmt` ON `ocmt`.`order_id` = `ucorder`.`order_id`
                        LEFT JOIN `uc_order_admin_comments` `ucadcom` ON `ucadcom`.`order_id` = `ucorder`.`order_id` and `ucadcom`.`uid`>0
                        INNER JOIN `uc_order_products` `prd` ON `prd`.`order_id` = `ucorder`.`order_id` 
                        LEFT JOIN `slt_node_learning_activity` `nact` ON `nact`.`entity_id`=`prg`.`id` and `nact`.`node_id`=`prd`.`nid`				
			        
			        where nact.entity_type IN ('cre_sys_obt_crt','cre_sys_obt_trn','cre_sys_obt_cur') AND 
						((prg.updated_on > '".$this->updSync_on ."') 
								OR (menr.updated_on > '".$this->updSync_on ."') 
									OR (orditem.updated_on > '".$this->updSync_on ."')
											OR (per.updated_on > '".$this->updSync_on ."')
						)
							AND  prg.status != 'lrn_lpn_sts_del' ";
			
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
                        `CartOrderID` ,
                        `OrderID` ,
                        `OrderTotal` ,
                        `OrderCurrencyType` ,
                        `OrderPaymentMethod` ,
                        `OrderProductCount` ,
                        `OrderBillingContactNumber` ,
                        `OrderBillingFirstName` ,
                        `OrderBillingLastName` ,
                        `OrderBillingStreet1` ,
                        `OrderBillingStreet2`,
                        `OrderBillingCity` ,
                        `OrderBillingZipCode`,
                        `OrderCreatedBy`  ,
                        `OrderModifiedBy`  ,
                        `OrderBillingZoneName`,
                        `OrderBillingCountryName`,
                        `OrderDiscountAmount` ,
                        `OrderDate` ,
                        `OrderDateTime` ,
                        `OrderTaxAmount` ,
                        `OrderTotalAmount` ,
                        `OrderPONumber` ,
                        `OrderPrice` ,
                        `OrderCourseTitle` ,
                        `OrderDiscountCode`,
                        `OrderDiscountName` ,
                        `OrderRefundAmount` ,
                        `OrderStatus` ,
                        `OrderUserComments` ,
                        `OrderAdminComments`,
                        `TrainingPlanTitle` ,
                        `TrainingPlanCode` ,
                        `TrainingPlanObjectName` ,
                        `TPOverallStatus` ,
                        `TPStatus` ,
                        `FullName` ,
                        `UserName` ,
                        `Email` ,
                        `OrganizationName` ,
                        `Address1` ,
                        `Address2` ,
                        `City` ,
                        `ZipCode` ,
                        `Country` ,
                        `State` ,
                    	`OrdersItemType` ,
	                    `OrdersTaxType` ,
	                    `NactEntityType` ,
                	    `OrderProductID` ,
                        `OrdOrderStatus`,
                        `OrderCustomAttribute0`,
                        `OrderCustomAttribute1`,
                        `OrderCustomAttribute2`,
                        `OrderCustomAttribute3`,
                        `OrderCustomAttribute4`,
                        `OrderCustomAttribute5`,
                        `OrderCustomAttribute6`,
                        `OrderCustomAttribute7`,
                        `OrderCustomAttribute8`,
                        `OrderCustomAttribute9`
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
							`flat`.`CartOrderID` 	=	`temp`.`CartOrderID` ,
                            `flat`.`OrderID` 	=	`temp`.`OrderID` ,
                            `flat`.`OrderTotal` 	=	`temp`.`OrderTotal` ,
                            `flat`.`OrderCurrencyType` 	=	`temp`.`OrderCurrencyType` ,
                            `flat`.`OrderPaymentMethod` 	=	`temp`.`OrderPaymentMethod` ,
                            `flat`.`OrderProductCount` 	=	`temp`.`OrderProductCount` ,
                            `flat`.`OrderBillingContactNumber` 	=	`temp`.`OrderBillingContactNumber` ,
                            `flat`.`OrderBillingFirstName` 	=	`temp`.`OrderBillingFirstName` ,
                            `flat`.`OrderBillingLastName` 	=	`temp`.`OrderBillingLastName` ,
                            `flat`.`OrderBillingStreet1` 	=	`temp`.`OrderBillingStreet1` ,
                            `flat`.`OrderBillingStreet2`	=	`temp`.`OrderBillingStreet2`,
                            `flat`.`OrderBillingCity` 	=	`temp`.`OrderBillingCity` ,
                            `flat`.`OrderBillingZipCode`	=	`temp`.`OrderBillingZipCode`,
                            `flat`.`OrderCreatedBy`  	=	`temp`.`OrderCreatedBy`  ,
                            `flat`.`OrderModifiedBy`  	=	`temp`.`OrderModifiedBy`  ,
                            `flat`.`OrderBillingZoneName`	=	`temp`.`OrderBillingZoneName`,
                            `flat`.`OrderBillingCountryName`	=	`temp`.`OrderBillingCountryName`,
                            `flat`.`OrderDiscountAmount` 	=	`temp`.`OrderDiscountAmount` ,
                            `flat`.`OrderDate` 	=	`temp`.`OrderDate` ,
                            `flat`.`OrderDateTime` 	=	`temp`.`OrderDateTime` ,
                            `flat`.`OrderTaxAmount` 	=	`temp`.`OrderTaxAmount` ,
                            `flat`.`OrderTotalAmount` 	=	`temp`.`OrderTotalAmount` ,
                            `flat`.`OrderPONumber` 	=	`temp`.`OrderPONumber` ,
                            `flat`.`OrderPrice` 	=	`temp`.`OrderPrice` ,
                            `flat`.`OrderCourseTitle` 	=	`temp`.`OrderCourseTitle` ,
                            `flat`.`OrderDiscountCode`	=	`temp`.`OrderDiscountCode`,
                            `flat`.`OrderDiscountName` 	=	`temp`.`OrderDiscountName` ,
                            `flat`.`OrderRefundAmount` 	=	`temp`.`OrderRefundAmount` ,
                            `flat`.`OrderStatus` 	=	`temp`.`OrderStatus` ,
                            `flat`.`OrderUserComments` 	=	`temp`.`OrderUserComments` ,
                            `flat`.`OrderAdminComments`	=	`temp`.`OrderAdminComments`,
                            `flat`.`TrainingPlanTitle` 	=	`temp`.`TrainingPlanTitle` ,
                            `flat`.`TrainingPlanCode` 	=	`temp`.`TrainingPlanCode` ,
                            `flat`.`TrainingPlanObjectName` 	=	`temp`.`TrainingPlanObjectName` ,
                            `flat`.`TPOverallStatus` 	=	`temp`.`TPOverallStatus` ,
                            `flat`.`TPStatus` 	=	`temp`.`TPStatus` ,
                            `flat`.`FullName` 	=	`temp`.`FullName` ,
                            `flat`.`UserName` 	=	`temp`.`UserName` ,
                            `flat`.`Email` 	=	`temp`.`Email` ,
                            `flat`.`OrganizationName` 	=	`temp`.`OrganizationName` ,
                            `flat`.`Address1` 	=	`temp`.`Address1` ,
                            `flat`.`Address2` 	=	`temp`.`Address2` ,
                            `flat`.`City` 	=	`temp`.`City` ,
                            `flat`.`ZipCode` 	=	`temp`.`ZipCode` ,
                            `flat`.`Country` 	=	`temp`.`Country` ,
                            `flat`.`State` 	=	`temp`.`State` ,
                            `flat`.`OrdersItemType` 	=	`temp`.`OrdersItemType` ,
                            `flat`.`OrdersTaxType` 	=	`temp`.`OrdersTaxType` ,
                            `flat`.`NactEntityType` 	=	`temp`.`NactEntityType` ,
                            `flat`.`OrderProductID` 	=	`temp`.`OrderProductID` ,
                            `flat`.`OrdOrderStatus` 	=	`temp`.`OrdOrderStatus` ,
                            `flat`.`OrderCustomAttribute0`   =   `temp`.`OrderCustomAttribute0`,
                            `flat`.`OrderCustomAttribute1`  =   `temp`.`OrderCustomAttribute1`,
                            `flat`.`OrderCustomAttribute2`  =   `temp`.`OrderCustomAttribute2`,
                            `flat`.`OrderCustomAttribute3`  =   `temp`.`OrderCustomAttribute3`,
                            `flat`.`OrderCustomAttribute4`  =   `temp`.`OrderCustomAttribute4`,
                            `flat`.`OrderCustomAttribute5`  =   `temp`.`OrderCustomAttribute5`,
                            `flat`.`OrderCustomAttribute6`  =   `temp`.`OrderCustomAttribute6`,
                            `flat`.`OrderCustomAttribute7`  =   `temp`.`OrderCustomAttribute7`,
                            `flat`.`OrderCustomAttribute8`  =   `temp`.`OrderCustomAttribute8`,
                            `flat`.`OrderCustomAttribute9`  =   `temp`.`OrderCustomAttribute9`,   
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
						DISTINCT concat_ws('-',`ord`.`id`,`menr`.`id`,`prd`.`order_product_id`,`orditem`.`id`,`ordersitem`.`line_item_id`) AS `RUID`
							FROM `slt_order` `ord` 
								join `uc_orders` `ucorder` ON ((`ucorder`.`order_id` = `ord`.`uc_order_id`) and (`ucorder`.`order_status` <> 'in_checkout'))
								join `slt_order_items` `orditem` ON (`orditem`.`order_id` = `ord`.`id`)
					 			LEFT JOIN `uc_order_line_items` `ordersitem` ON `ordersitem`.`order_id` = `ucorder`.`order_id` and `ordersitem`.`type` = 'uc_discounts'				
								join `slt_master_enrollment` `menr` ON (`menr`.`order_id` = `orditem`.`order_id`)
								INNER JOIN `slt_program` `prg` ON `prg`.`id` = `menr`.`program_id`
								join `uc_order_products` `prd` ON (`prd`.`order_id` = `ucorder`.`order_id`)
								LEFT JOIN `slt_node_learning_activity` `nact` ON `nact`.`entity_id`=`prg`.`id` and `nact`.`node_id`=`prd`.`nid`	
								JOIN `report_deleted_logs` del ON  del.table_name = 'slt_session_instructor_details'
							WHERE nact.entity_type IN ('cre_sys_obt_crt','cre_sys_obt_trn','cre_sys_obt_cur') AND orditem.course_id = del.parent1_entity_id and orditem.class_id = del.parent2_entity_id and 
							del.id is not null AND del.deleted_on > '".$this->delSync_on ."'
				";
			
			return $query;
			
		} catch(Exception $ex){
			throw new Exception("Error in reportCollectIds ".$ex->getMessage());
		}	
	}
} 
	
	
