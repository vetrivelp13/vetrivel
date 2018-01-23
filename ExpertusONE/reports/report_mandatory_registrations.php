<?php
include_once $_SERVER['DOCUMENT_ROOT'] . '/reports/ReportSyncUp.php';

class report_mandatory_registrations extends ReportSyncUp {
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
			
			//$createTable = "CREATE TABLE " . $this->temp_table_name . " LIKE " . $this->report_table . "";
			$createTable = "CREATE TABLE " . $this->temp_table_name ." (
             `RUID` varchar(255),
              `user_id` int(11) DEFAULT NULL,
              `class_id` int(11) DEFAULT NULL,
              `program_id` int(11) DEFAULT NULL,  
              `enrollment_id` int(11) DEFAULT NULL,
              `master_enrollment_id` int(11) DEFAULT NULL,
              `program_title` varchar(255) DEFAULT NULL,
              `class_title` longtext,
              `program_code` varchar(100),
              `class_code` varchar(255),
              `program_object_type` longtext,
              `user_name` varchar(255) DEFAULT NULL,
              `menr_overall_status` longtext,
              `enr_reg_status` varchar(100) DEFAULT NULL,
              `enr_comp_status` varchar(100) DEFAULT NULL,
              `enr_comp_status_name` longtext,
              `enr_reg_status_name` longtext,
              `menrexempted` int(1),
              `enrexempted` int(1),
              `menr_reg_date` datetime DEFAULT NULL,
              `enr_reg_date` datetime DEFAULT NULL,
              `menr_comp_date` datetime DEFAULT NULL,
              `enr_comp_date` datetime DEFAULT NULL,
              `program_group_id` int(11) DEFAULT 0,
              `program_group_name` varchar(100),
              `class_group_id` int(11) DEFAULT 0,
              `class_group_name` varchar(100),
              `class_mro` longtext,
              `program_mro` longtext,
			  `operation` varchar(10) NULL DEFAULT NULL,
			  PRIMARY KEY (`RUID`),
			  key sli_op(operation)
			) ENGINE=InnoDB DEFAULT CHARSET=utf8;";
			$this->db->callQuery($createTable);
			
			//$alterTable = "ALTER TABLE " . $this->temp_table_name . " ADD COLUMN `operation` varchar(10) NULL DEFAULT NULL;";
			//$this->db->callQuery($alterTable);
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
			// RUID has been changed in a way it has no duplicates and produce junk ids
			// Please have data setup with multiple learner groups with Mandatory MRO Access in case RUID needs to be changed for any reason
			$tempDataQuery = "insert into " . $this->temp_table_name . "
					SELECT
					    concat_ws('-', `enr`.`id`, ifnull(`prggrp`.`id`, 0), ifnull(`clsgrp`.`id`, 0)) AS `RUID`,
	                    `enr`.`user_id` AS `user_id`,
                        `enr`.`class_id` AS `class_id`,
	                    `prg`.`id` AS `program_id`,
						`enr`.`id` AS `enrollment_id`,
                        `enr`.`master_enrollment_id` AS `master_enrollment_id`,
                        `prg`.`title` AS `program_title`,
                        `cls`.`title` AS `class_title`,
                        `prg`.`code` AS `program_code`,
                        `cls`.`code` AS `class_code`,
                        `prgdel`.`name` AS `program_object_type`,
                        `per`.`user_name` AS `user_name`,
                        `comstatus`.`name` AS `menr_overall_status`,
                        `enr`.`reg_status` AS `enr_reg_status`,
                        `enr`.`comp_status` AS `enr_comp_status`,
                        `compst`.`name` AS `enr_comp_status_name`,
                        `cls_regsts`.`name` AS `enr_reg_status_name`,
                        `menrexmp`.`exempted_status` AS `menrexempted`,
                        `enrexmp`.`exempted_status` AS `enrexempted`,
                        `menr`.`reg_date` AS `menr_reg_date`,
                        `enr`.`reg_date` AS `enr_reg_date`,
                        `menr`.`comp_date` AS `menr_comp_date`,
                        `enr`.`comp_date` AS `enr_comp_date`,
                        `prggrp`.`id` AS `program_group_id`,
						`prggrp`.`name` AS `program_group_name`,
						`clsgrp`.`id` AS `class_group_id`,
                        `clsgrp`.`name` AS `class_group_name`,
                        `clsmro`.`name` AS `class_mro`,
                        `prgmro`.`name` AS `program_mro`,
			            NULL
   
                    from `slt_enrollment` `enr`
                        left join `slt_course_class` `cls` ON ((isnull(`enr`.`master_enrollment_id`) and (`cls`.`id` = `enr`.`class_id`) and (`cls`.`status` != 'lrn_cls_sts_del')))
                        left join `slt_master_enrollment` `menr` ON (((`enr`.`master_enrollment_id` = `menr`.`id`) and (`menr`.`user_id` = `enr`.`user_id`) and (`menr`.`overall_status` not in ('lrn_tpm_ovr_cln' , 'lrn_tpm_ovr_rsc', 'lrn_tpm_ovr_rsv'))))
                        left join `slt_program` `prg` ON (((`prg`.`id` = `menr`.`program_id`) and (`prg`.`status` != 'lrn_lpn_sts_del')))
                        left join `slt_person` `per` ON ((`per`.`id` = `enr`.`user_id`))
                        left join `slt_enrollment_exempted` `enrexmp` ON ((`enrexmp`.`id` = (select `slt_enrollment_exempted`.`id` from `slt_enrollment_exempted` where ((`slt_enrollment_exempted`.`enrollment_id` = `enr`.`id`) and (`slt_enrollment_exempted`.`enroll_type` = 'class')) order by `slt_enrollment_exempted`.`id` desc limit 1)))
                        left join `slt_enrollment_exempted` `menrexmp` ON ((`menrexmp`.`id` = (select `slt_enrollment_exempted`.`id` from `slt_enrollment_exempted`  where ((`slt_enrollment_exempted`.`enrollment_id` = `menr`.`id`) and (`slt_enrollment_exempted`.`enroll_type` = 'tp')) order by `slt_enrollment_exempted`.`id` desc limit 1)))
                        left join `slt_group_mapping` `clsmap` ON (((`clsmap`.`entity_id` = `cls`.`id`) and (`clsmap`.`entity_type` = 'cre_sys_obt_cls') and (`clsmap`.`group_type` = 0) and (`clsmap`.`mro` = 'cre_sys_inv_man')))
                        left join `slt_group_mapping` `prgmap` ON (((`prgmap`.`entity_id` = `prg`.`id`) and (`prgmap`.`entity_type` in ('cre_sys_obt_crt' , 'cre_sys_obt_cur', 'cre_sys_obt_trn', 'cre_sys_obt_trp')) and (`prgmap`.`group_type` = 0) and (`prgmap`.`mro` = 'cre_sys_inv_man')))
                        left join `slt_groups` `prggrp` ON (((`prggrp`.`id` = `prgmap`.`group_id`) and (`prggrp`.`status` = 'cre_sec_sts_atv')))
                        left join `slt_groups` `clsgrp` ON (((`clsgrp`.`id` = `clsmap`.`group_id`) and (`clsgrp`.`status` = 'cre_sec_sts_atv')))
                        left join `slt_profile_list_items` `cls_regsts` ON (((`cls_regsts`.`code` = `enr`.`reg_status`) and (`cls_regsts`.`lang_code` = 'cre_sys_lng_eng')))
                        left join `slt_profile_list_items` `compst` ON (((`compst`.`code` = `enr`.`comp_status`) and (`compst`.`lang_code` = 'cre_sys_lng_eng')))
                        left join `slt_profile_list_items` `comstatus` ON (((`comstatus`.`code` = `menr`.`overall_status`) and (`comstatus`.`lang_code` = 'cre_sys_lng_eng')))
                        left join `slt_profile_list_items` `prgdel` ON (`prgdel`.`code` = `prg`.`object_type`)
                        left join `slt_profile_list_items` `clsmro` ON (`clsmro`.`code` = `clsmap`.`mro`)
                        left join `slt_profile_list_items` `prgmro` ON (`prgmro`.`code` = `prgmap`.`mro`)
                where
	                   ((`enr`.`reg_status` not in ('lrn_crs_reg_can' , 'lrn_crs_reg_rsc', 'lrn_crs_reg_rsv'))
	                   and        
	                   ((`menrexmp`.`exempted_status` = 1)
                         or (`enrexmp`.`exempted_status` = 1)
                         or (`enr`.`mandatory` = 'Y')
                         or (`enr`.`mandatory` = '1')
                         or (`menr`.`mandatory` = '1')))";
			
			if($this->updSync_on != null) {
				$tempDataQuery .= " AND (
			        			(enr.updated_on > '" . $this->updSync_on . "')
			        			OR (cls.updated_on > '" . $this->updSync_on . "')
			        			OR (menr.updated_on > '" . $this->updSync_on . "')
			        			OR (prg.updated_on > '" . $this->updSync_on . "')
			        			OR (per.updated_on > '" . $this->updSync_on . "')
			        			)";
			}
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
			if ($this->flush == 'true') {
				$flushTable  = "TRUNCATE TABLE " . $this->report_table ."; ";
				expDebug::dPrint('$flushTable query :' . $flushTable, 4);
				$this->db->callQuery($flushTable);
			}
			
			$query = "insert into " . $this->report_table . "
					SELECT 
						`RUID` ,
                        `user_id` ,
                        `class_id` ,
                        `program_id` ,  
						`enrollment_id`,
                        `master_enrollment_id` ,
                        `program_title` ,
                        `class_title` ,
                        `program_code` ,
                        `class_code` ,
                        `program_object_type` ,
                        `user_name` ,
                        `menr_overall_status` ,
                        `enr_reg_status` ,
                        `enr_comp_status` ,
                        `enr_comp_status_name` ,
                        `enr_reg_status_name` ,
                        `menrexempted`,
                        `enrexempted`,
                        `menr_reg_date`,
                        `enr_reg_date`,
                        `menr_comp_date`,
                        `enr_comp_date`,
						`program_group_id` ,
                        `program_group_name` ,
						`class_group_id` ,
                        `class_group_name` ,
                        `class_mro` ,
                        `program_mro` 
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
                            `flat`.`user_id`             	=	`temp`.`user_id`,
                            `flat`.`class_id`           	=	`temp`.`class_id`,
                            `flat`.`program_id`         	=	`temp`.`program_id`  ,
							`flat`.`enrollment_id`  		=	`temp`.`enrollment_id`,
                            `flat`.`master_enrollment_id`  	=	`temp`.`master_enrollment_id`,
                            `flat`.`program_title`       	=	`temp`.`program_title`,
                            `flat`.`class_title`         	=	`temp`.`class_title`,
                            `flat`.`program_code`       	=	`temp`.`program_code`,
                            `flat`.`class_code`         	=	`temp`.`class_code`,
                            `flat`.`program_object_type` 	=	`temp`.`program_object_type`,
                            `flat`.`user_name`          	=	`temp`.`user_name`,
                            `flat`.`menr_overall_status` 	=	`temp`.`menr_overall_status`,
                            `flat`.`enr_reg_status`     	=	`temp`.`enr_reg_status`,
                            `flat`.`enr_comp_status`     	=	`temp`.`enr_comp_status`,
                            `flat`.`enr_comp_status_name` 	=	`temp`.`enr_comp_status_name`,
                            `flat`.`enr_reg_status_name` 	=	`temp`.`enr_reg_status_name`,
                            `flat`.`menrexempted`        	=	`temp`.`menrexempted`,
                            `flat`.`enrexempted`         	=	`temp`.`enrexempted`,
                            `flat`.`menr_reg_date`       	=	`temp`.`menr_reg_date`,
                            `flat`.`enr_reg_date`        	=	`temp`.`enr_reg_date`,
                            `flat`.`menr_comp_date`      	=	`temp`.`menr_comp_date`,
                            `flat`.`enr_comp_date`       	=	`temp`.`enr_comp_date`,
							`flat`.`program_group_id` 		=	`temp`.`program_group_id`,
                            `flat`.`program_group_name` 	=	`temp`.`program_group_name`,
							`flat`.`class_group_id` 		=	`temp`.`class_group_id`,
                            `flat`.`class_group_name`   	=	`temp`.`class_group_name`,
                            `flat`.`class_mro`           	=	`temp`.`class_mro`,
                            `flat`.`program_mro`         	=	`temp`.`program_mro`,
								
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
			/**
			 * slt_group_mapping can produce incorrect RUID and so the access modified trainings enrollments need to be removed
			 */
			$query = "(select rep.RUID
					from report_mandatory_registrations rep
					join slt_course_class cls on cls.updated_on > '" . $this->delSync_on . "' and rep.class_id = cls.id
					left join `slt_group_mapping` `clsmap` ON (
					            (`clsmap`.`entity_id` = rep.class_id) 
					            and (`clsmap`.`entity_type` = 'cre_sys_obt_cls')
					            and (`clsmap`.`group_type` = 0)
					            and (`clsmap`.`mro` = 'cre_sys_inv_man'))
					left join `slt_group_mapping` `prgmap` ON (
					            (`prgmap`.`entity_id` = `rep`.`program_id`)
					            and (`prgmap`.`entity_type` in ('cre_sys_obt_crt' , 'cre_sys_obt_cur', 'cre_sys_obt_trn', 'cre_sys_obt_trp'))
					            and (`prgmap`.`group_type` = 0)
					            and (`prgmap`.`mro` = 'cre_sys_inv_man'))
					where rep.RUID != concat_ws('-', rep.enrollment_id, ifnull(prgmap.group_id, 0), ifnull(clsmap.group_id, 0))
					UNION
					select rep.RUID
					from report_mandatory_registrations rep
					join slt_program prg on prg.updated_on > '" . $this->delSync_on . "' and rep.program_id = prg.id
					left join `slt_group_mapping` `clsmap` ON (
					            (`clsmap`.`entity_id` = rep.class_id) 
					            and (`clsmap`.`entity_type` = 'cre_sys_obt_cls')
					            and (`clsmap`.`group_type` = 0)
					            and (`clsmap`.`mro` = 'cre_sys_inv_man'))
					left join `slt_group_mapping` `prgmap` ON (
					            (`prgmap`.`entity_id` = `rep`.`program_id`)
					            and (`prgmap`.`entity_type` in ('cre_sys_obt_crt' , 'cre_sys_obt_cur', 'cre_sys_obt_trn', 'cre_sys_obt_trp'))
					            and (`prgmap`.`group_type` = 0)
					            and (`prgmap`.`mro` = 'cre_sys_inv_man'))
					where rep.RUID != concat_ws('-', rep.enrollment_id, ifnull(prgmap.group_id, 0), ifnull(clsmap.group_id, 0)))";
			
			return $query;
		} catch(Exception $ex) {
			throw new Exception("Error in reportCollectIds " . $ex->getMessage());
		}
	}
}

/** query to fetch access modified trainigs and their enrollments
 * select rep.RUID, cls.id as class_id, concat_ws('-', rep.enrollment_id, ifnull(prgmap.group_id, 0), ifnull(clsmap.group_id, 0)) as curRUID,
rep.enrollment_id, clsmap.group_id, prgmap.group_id
from report_mandatory_registrations rep
join slt_course_class cls on cls.updated_on > '2017-09-06' and rep.class_id = cls.id
left join `slt_group_mapping` `clsmap` ON (((`clsmap`.`entity_id` = rep.class_id) 
            and (`clsmap`.`entity_type` = 'cre_sys_obt_cls')
            and (`clsmap`.`group_type` = 0)
            and (`clsmap`.`mro` = 'cre_sys_inv_man')))
left join `slt_group_mapping` `prgmap` ON (((`prgmap`.`entity_id` = `rep`.`program_id`)
            and (`prgmap`.`entity_type` in ('cre_sys_obt_crt' , 'cre_sys_obt_cur', 'cre_sys_obt_trn', 'cre_sys_obt_trp'))
            and (`prgmap`.`group_type` = 0)
            and (`prgmap`.`mro` = 'cre_sys_inv_man')))
            where rep.RUID != concat_ws('-', rep.enrollment_id, ifnull(prgmap.group_id, 0), ifnull(clsmap.group_id, 0));
 */