<?php
include_once $_SERVER['DOCUMENT_ROOT'] .  '/reports/ReportSyncUp.php';

class report_class_orders extends ReportSyncUp {
	
	
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
			        `RUID` VARCHAR(255),
			        `CartOrderID` int(10) unsigned NOT NULL DEFAULT '0',
			        `OrderID` int(11) NOT NULL DEFAULT '0',
			        `OrderTotal` decimal(16,5) NOT NULL DEFAULT '0.00000',
			        `OrderCurrencyType` varchar(4) NOT NULL,
			        `OrderPaymentMethod` varchar(32) NOT NULL DEFAULT '',
			        `OrderProductCount` int(10) unsigned NOT NULL DEFAULT '0',
			        `OrderBillingContactNumber` varchar(255) NOT NULL DEFAULT '',
			        `OrderBillingFirstName` varchar(255) NOT NULL DEFAULT '',
			        `OrderBillingLastName` varchar(255) NOT NULL DEFAULT '',
			        `OrderBillingStreet1` varchar(255) NOT NULL DEFAULT '',
			        `OrderBillingStreet2` varchar(255) NOT NULL DEFAULT '',
			        `OrderBillingCity` varchar(255) NOT NULL DEFAULT '',
			        `OrderBillingZipCode` varchar(255) NOT NULL DEFAULT '',
			        `OrderCreatedBy` int(11) NOT NULL DEFAULT '0',
			        `OrderModifiedBy` int(11) NOT NULL DEFAULT '0',
			        `OrderBillingZoneName` varchar(255) DEFAULT '',
			        `OrderBillingCountryName` varchar(255) DEFAULT '',
			        `OrderDiscountAmount` decimal(16,5) DEFAULT '0.00000',
			        `OrderDate` datetime DEFAULT NULL,
			        `OrderDateTime` datetime DEFAULT NULL,
			        `OrderTaxAmount` decimal(16,5) DEFAULT '0.00000',
			        `OrderTotalAmount` decimal(16,5) DEFAULT '0.00000',
			        `OrderPONumber` varchar(255) DEFAULT '',
			        `OrderPrice` decimal(16,5) NOT NULL DEFAULT '0.00000',
			        `PriceofCourse` decimal(16,2) DEFAULT '0.00',
			        `OrderCourseTitle` varchar(255) NOT NULL DEFAULT '',
			        `OrderDiscountCode` text,
			        `OrderDiscountName` varchar(255) DEFAULT NULL,
			        `OrderRefundAmount` decimal(16,5) DEFAULT '0.00000',
			        `OrderStatusCode` varchar(255) DEFAULT NULL,
			        `OrderStatus` longtext,
			        `OrderUserComments` text,
			        `OrderAdminComments` text,
			        `MasterEnrollmentID` int(11) DEFAULT NULL,
			        `NlaEntityType` varchar(100) DEFAULT NULL,
			        `CourseTitle` longtext,
			        `CourseCode` varchar(255) NOT NULL,
			        `ClassTitle` longtext,
			        `ClassCode` varchar(255) NOT NULL,
			        `ClassMinSeats` int(11) DEFAULT NULL,
			        `ClassMaxSeats` int(11) DEFAULT NULL,
			        `ClassDeliveryType` longtext,
			        `ClassStatus` longtext,
			        `SessionTitle` longtext,
			        `SessionStartDate` datetime DEFAULT NULL,
			        `SessionStartTime` varchar(5) DEFAULT NULL,
			        `SessionEndTime` varchar(5) DEFAULT NULL,
			        `SessionEndDate` datetime DEFAULT NULL,
			        `ClassSessionMobileStatus` varchar(50) DEFAULT NULL,
			        `instructorUsername` varchar(255) DEFAULT NULL,
			        `CompletionStatus` longtext,
			        `EnrollmentStatus` longtext,
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
			        `OrderProductID` int(10) unsigned NOT NULL DEFAULT '0',
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
					   concat_ws('-',`ord`.`id`,`enr`.`id`,`prd`.`order_product_id`,`orditem`.`id`, IFNULL(`ses`.`id`, 0),  IFNULL(`ins_det`.`id`, 0), IFNULL(ocmt.comment_id, 0), IFNULL(ucadcom.comment_id, 0), IFNULL(ordersitem.line_item_id, 0)) AS `RUID`,
                       `ucorder`.`order_id` AS `CartOrderID`,
                       `ord`.`id` AS `OrderID`,
                       `ucorder`.`order_total` AS `OrderTotal`,
                       `ucorder`.`currency_type` AS `OrderCurrencyType`,
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
                       `crs`.`price` AS `PriceofCourse`,
                       `prd`.`title` AS `OrderCourseTitle`,
                       `udordcode`.`codes` AS `OrderDiscountCode`,
                       `sltdis`.`discount_name` AS `OrderDiscountName`,
                       `orditem`.`refund_amount` AS `OrderRefundAmount`,
		               `ord`.`order_status` AS `OrderStatusCode`,
                       `ordsts`.`name` AS `OrderStatus`,
                       `ocmt`.`message` AS `OrderUserComments`,
                       `ucadcom`.`message` AS `OrderAdminComments`,
		               `enr`.`master_enrollment_id` AS `MasterEnrollmentID`,
		               `nact`.`entity_type` AS `NlaEntityType`,
                       `crs`.`title` AS `CourseTitle`,
                       `crs`.`code` AS `CourseCode`,
                       `cls`.`title` AS `ClassTitle`,
                       `cls`.`code` AS `ClassCode`,
                       `cls`.`min_seats` AS `ClassMinSeats`,
                       `cls`.`max_seats` AS `ClassMaxSeats`,
                       `clsdel`.`name` AS `ClassDeliveryType`,
                       `clssts`.`name` AS `ClassStatus`,
                       `ses`.`title` AS `SessionTitle`,
                       `ses`.`start_date` AS `SessionStartDate`,
                       `ses`.`start_time` AS `SessionStartTime`,
                       `ses`.`end_time` AS `SessionEndTime`,
                       `ses`.`end_date` AS `SessionEndDate`,
                       `matt`.`status` AS `ClassSessionMobileStatus`,
                       `ins`.`user_name` AS `instructorUsername`,
                       `compsts`.`name` AS `CompletionStatus`,
                       `regsts`.`name` AS `EnrollmentStatus`,
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
	               	   `prd`.`order_product_id` AS `OrderProductID`,
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
                    
			        from `slt_order` `ord` 
                        join `slt_profile_list_items` `ordsts` ON (`ordsts`.`code` = `ord`.`order_status`)
                        join `uc_orders` `ucorder` ON ((`ucorder`.`order_id` = `ord`.`uc_order_id`) and (`ucorder`.`order_status` <> 'in_checkout'))
                        left join `uc_countries` `con` ON (`con`.`country_id` = `ucorder`.`billing_country`)
                        left join `uc_zones` `zone` ON ((`zone`.`zone_country_id` = `con`.`country_id`) and (`zone`.`zone_id` = `ucorder`.`billing_zone`))
                        join `slt_order_items` `orditem` ON (`orditem`.`order_id` = `ord`.`id`)
                        join `slt_enrollment` `enr` ON (`enr`.`order_id` = `orditem`.`order_id` and enr.course_id=orditem.course_id and enr.class_id = orditem.class_id)
                        join `slt_course_template` `crs` ON (`crs`.`id` = `enr`.`course_id`)
                        join `slt_course_class` `cls` ON (`cls`.`id` = `enr`.`class_id`)
                        join `slt_person` `per` ON (`per`.`id` = `enr`.`user_id`)
                        join `slt_profile_list_items` `regsts` ON (`regsts`.`code` = `enr`.`reg_status`)
                        join `slt_profile_list_items` `clsdel` ON (`clsdel`.`code` = `cls`.`delivery_type`)
                        join`slt_profile_list_items` `clssts` ON (`clssts`.`code` = `cls`.`status`)
                        left join `slt_course_class_session` `ses` ON (`ses`.`class_id` = `enr`.`class_id`)
                        left join `slt_session_instructor_details` `ins_det` ON (`ins_det`.`session_id` = `ses`.`id`)
                        left join `slt_person` `ins` ON (`ins`.`id` = `ins_det`.`instructor_id`)
                        left join `slt_mobile_ilt_attendance` `matt` ON (`matt`.`sessionid` = `ses`.`id` and `matt`.`learnerid` = `enr`.`user_id`)
                        left join `slt_profile_list_items` `compsts` ON (`compsts`.`code` = `enr`.`comp_status`)
                        left join `slt_country` `country` ON (`country`.`country_code` = `per`.`country`)
                        left join `slt_state` `sta` ON ((`sta`.`state_code` = `per`.`state`) and (`sta`.`country_code` = `country`.`country_code`))
                        left join `slt_organization` `org` ON (`org`.`id` = `per`.`org_id`)
                        left join `uc_order_line_items` `ordersitem` ON ((`ordersitem`.`order_id` = `ucorder`.`order_id`) and (`ordersitem`.`type` = 'uc_discounts'))
                        left join `uc_order_line_items` `ordertax` ON ((`ordertax`.`order_id` = `ucorder`.`order_id`) and (`ordertax`.`type` = 'cybersource_tax'))
                        left join `uc_payment_po` `po` ON (`ucorder`.`order_id` = `po`.`order_id`)
                        left join `uc_discounts_order_codes` `udordcode` ON (`udordcode`.`order_id` = `ucorder`.`order_id`)
                        left join `uc_discounts_codes` `udcode` ON (`udcode`.`code` = `udordcode`.`codes`)
                        left join `slt_discounts` `sltdis` ON (`sltdis`.`uc_discount_id` = `udcode`.`discount_id`)
                        left join `uc_order_comments` `ocmt` ON (`ocmt`.`order_id` = `ucorder`.`order_id`)
                        left join `uc_order_admin_comments` `ucadcom` ON (`ucadcom`.`order_id` = `ucorder`.`order_id` and `ucadcom`.`uid`>0)
                        join `uc_order_products` `prd` ON (`prd`.`order_id` = `ucorder`.`order_id`)
                        left join `slt_node_learning_activity` `nact` ON ((`nact`.`entity_id` = `cls`.`id`) and (`nact`.`node_id` = `prd`.`nid`))	
      
					where ord.order_status NOT IN('cme_pmt_sts_rcl','cme_pmt_sts_rsv') and enr.master_enrollment_id IS NULL AND  nact.entity_type = 'cre_sys_obt_cls' AND
					((ord.updated_on > '".$this->updSync_on ."') 
							OR (enr.updated_on > '".$this->updSync_on ."') 
									OR (orditem.updated_on > '".$this->updSync_on ."')
											OR (ses.updated_on > '".$this->updSync_on ."')
													OR (crs.updated_on > '".$this->updSync_on ."') 
														OR (cls.updated_on > '".$this->updSync_on ."')
															OR (per.updated_on > '".$this->updSync_on ."')
																	OR (sltdis.updated_on > '".$this->updSync_on ."')
																		OR (ins_det.updated_on > '".$this->updSync_on ."')
					)";
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
                           `CartOrderID`,
                           `OrderID`,
                           `OrderTotal` ,
                           `OrderCurrencyType` ,
                           `OrderPaymentMethod` ,
                           `OrderProductCount`,
                           `OrderBillingContactNumber` ,
                           `OrderBillingFirstName` ,
                           `OrderBillingLastName` ,
                           `OrderBillingStreet1` ,
                           `OrderBillingStreet2` ,
                           `OrderBillingCity` ,
                           `OrderBillingZipCode`,
                           `OrderCreatedBy` ,
                           `OrderModifiedBy`,
                           `OrderBillingZoneName`,
                           `OrderBillingCountryName` ,
                           `OrderDiscountAmount` ,
                           `OrderDate` ,
                           `OrderDateTime` ,
                           `OrderTaxAmount` ,
                           `OrderTotalAmount` ,
                           `OrderPONumber` ,
                           `OrderPrice` ,
                           `PriceofCourse` ,
                           `OrderCourseTitle`,
                           `OrderDiscountCode` ,
                           `OrderDiscountName` ,
                           `OrderRefundAmount` ,
                           `OrderStatusCode` ,
                           `OrderStatus` ,
                           `OrderUserComments` ,
                           `OrderAdminComments` ,
                           `MasterEnrollmentID` ,
                           `NlaEntityType` ,
                           `CourseTitle` ,
                           `CourseCode` ,
                           `ClassTitle` ,
                           `ClassCode` ,
                           `ClassMinSeats` ,
                           `ClassMaxSeats` ,
                           `ClassDeliveryType` ,
                           `ClassStatus` ,
                           `SessionTitle` ,
                           `SessionStartDate` ,
                           `SessionStartTime` ,
                           `SessionEndTime`,
                           `SessionEndDate` ,
                           `ClassSessionMobileStatus` ,
                           `instructorUsername` ,
                           `CompletionStatus` ,
                           `EnrollmentStatus` ,
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
                           `OrderProductID`,
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
                            `flat`.`CartOrderID`	=	`temp`.`CartOrderID`,
                            `flat`.`OrderID`	=	`temp`.`OrderID`,
                            `flat`.`OrderTotal` 	=	`temp`.`OrderTotal` ,
                            `flat`.`OrderCurrencyType` 	=	`temp`.`OrderCurrencyType` ,
                            `flat`.`OrderPaymentMethod` 	=	`temp`.`OrderPaymentMethod` ,
                            `flat`.`OrderProductCount`	=	`temp`.`OrderProductCount`,
                            `flat`.`OrderBillingContactNumber` 	=	`temp`.`OrderBillingContactNumber` ,
                            `flat`.`OrderBillingFirstName` 	=	`temp`.`OrderBillingFirstName` ,
                            `flat`.`OrderBillingLastName` 	=	`temp`.`OrderBillingLastName` ,
                            `flat`.`OrderBillingStreet1` 	=	`temp`.`OrderBillingStreet1` ,
                            `flat`.`OrderBillingStreet2` 	=	`temp`.`OrderBillingStreet2` ,
                            `flat`.`OrderBillingCity` 	=	`temp`.`OrderBillingCity` ,
                            `flat`.`OrderBillingZipCode`	=	`temp`.`OrderBillingZipCode`,
                            `flat`.`OrderCreatedBy` 	=	`temp`.`OrderCreatedBy` ,
                            `flat`.`OrderModifiedBy`	=	`temp`.`OrderModifiedBy`,
                            `flat`.`OrderBillingZoneName`	=	`temp`.`OrderBillingZoneName`,
                            `flat`.`OrderBillingCountryName` 	=	`temp`.`OrderBillingCountryName` ,
                            `flat`.`OrderDiscountAmount` 	=	`temp`.`OrderDiscountAmount` ,
                            `flat`.`OrderDate` 	=	`temp`.`OrderDate` ,
                            `flat`.`OrderDateTime` 	=	`temp`.`OrderDateTime` ,
                            `flat`.`OrderTaxAmount` 	=	`temp`.`OrderTaxAmount` ,
                            `flat`.`OrderTotalAmount` 	=	`temp`.`OrderTotalAmount` ,
                            `flat`.`OrderPONumber` 	=	`temp`.`OrderPONumber` ,
                            `flat`.`OrderPrice` 	=	`temp`.`OrderPrice` ,
                            `flat`.`PriceofCourse` 	=	`temp`.`PriceofCourse` ,
                            `flat`.`OrderCourseTitle`	=	`temp`.`OrderCourseTitle`,
                            `flat`.`OrderDiscountCode` 	=	`temp`.`OrderDiscountCode` ,
                            `flat`.`OrderDiscountName` 	=	`temp`.`OrderDiscountName` ,
                            `flat`.`OrderRefundAmount` 	=	`temp`.`OrderRefundAmount` ,
                            `flat`.`OrderStatusCode` 	=	`temp`.`OrderStatusCode` ,
                            `flat`.`OrderStatus` 	=	`temp`.`OrderStatus` ,
                            `flat`.`OrderUserComments` 	=	`temp`.`OrderUserComments` ,
                            `flat`.`OrderAdminComments` 	=	`temp`.`OrderAdminComments` ,
                            `flat`.`MasterEnrollmentID` 	=	`temp`.`MasterEnrollmentID` ,
                            `flat`.`NlaEntityType` 	=	`temp`.`NlaEntityType` ,
                            `flat`.`CourseTitle` 	=	`temp`.`CourseTitle` ,
                            `flat`.`CourseCode` 	=	`temp`.`CourseCode` ,
                            `flat`.`ClassTitle` 	=	`temp`.`ClassTitle` ,
                            `flat`.`ClassCode` 	=	`temp`.`ClassCode` ,
                            `flat`.`ClassMinSeats` 	=	`temp`.`ClassMinSeats` ,
                            `flat`.`ClassMaxSeats` 	=	`temp`.`ClassMaxSeats` ,
                            `flat`.`ClassDeliveryType` 	=	`temp`.`ClassDeliveryType` ,
                            `flat`.`ClassStatus` 	=	`temp`.`ClassStatus` ,
                            `flat`.`SessionTitle` 	=	`temp`.`SessionTitle` ,
                            `flat`.`SessionStartDate` 	=	`temp`.`SessionStartDate` ,
                            `flat`.`SessionStartTime` 	=	`temp`.`SessionStartTime` ,
                            `flat`.`SessionEndTime`	=	`temp`.`SessionEndTime`,
                            `flat`.`SessionEndDate` 	=	`temp`.`SessionEndDate` ,
                            `flat`.`ClassSessionMobileStatus` 	=	`temp`.`ClassSessionMobileStatus` ,
                            `flat`.`instructorUsername` 	=	`temp`.`instructorUsername` ,
                            `flat`.`CompletionStatus` 	=	`temp`.`CompletionStatus` ,
                            `flat`.`EnrollmentStatus` 	=	`temp`.`EnrollmentStatus` ,
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
                            `flat`.`OrderProductID`	=	`temp`.`OrderProductID`,
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
						DISTINCT concat_ws('-',`ord`.`id`,`enr`.`id`,`prd`.`order_product_id`,`orditem`.`id`,`del`.`parent3_entity_id`,`del`.`entity_id`) AS `RUID`
							FROM `slt_order` `ord` 
								join `uc_orders` `ucorder` ON ((`ucorder`.`order_id` = `ord`.`uc_order_id`) and (`ucorder`.`order_status` <> 'in_checkout'))
								join `slt_order_items` `orditem` ON (`orditem`.`order_id` = `ord`.`id`)
								join `slt_enrollment` `enr` ON (`enr`.`order_id` = `orditem`.`order_id` and enr.course_id=orditem.course_id and enr.class_id = orditem.class_id)
								join `uc_order_products` `prd` ON (`prd`.`order_id` = `ucorder`.`order_id`)
								left join `slt_node_learning_activity` `nact` ON ((`nact`.`entity_id` = `enr`.`class_id`) and (`nact`.`node_id` = `prd`.`nid`))
								JOIN `report_deleted_logs` del ON  del.table_name = 'slt_session_instructor_details'
							WHERE ord.order_status NOT IN('cme_pmt_sts_rcl','cme_pmt_sts_rsv') and enr.master_enrollment_id IS NULL AND  nact.entity_type = 'cre_sys_obt_cls' AND orditem.course_id = del.parent1_entity_id and orditem.class_id = del.parent2_entity_id and 
							del.id is not null AND del.deleted_on > '".$this->delSync_on ."'
					";
			expDebug::dPrint("reportCollectIds Query" . print_r($query, 1), 4);
			return $query;
			
		} catch(Exception $ex){
			throw new Exception("Error in reportCollectIds ".$ex->getMessage());
		}	
	}
} 
	
	
