<?php
include_once $_SERVER['DOCUMENT_ROOT'] . '/reports/ReportSyncUp.php';

class report_enrollment extends ReportSyncUp {
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
			
		//	$createTable = "CREATE TABLE " . $this->temp_table_name . " LIKE " . $this->report_table . "";
			$createTable = "CREATE TABLE ".$this->temp_table_name ." (
			       `RUID` varchar(255) NOT NULL,
                      `UserName` varchar(255) DEFAULT NULL,
                      `UserID` int(11) NOT NULL DEFAULT '0',
                      `FullName` varchar(255) DEFAULT NULL,
                      `Firstname` varchar(255) DEFAULT NULL,
                      `Lastname` varchar(255) DEFAULT NULL,
                      `Email` varchar(255) DEFAULT NULL,
                      `City` varchar(255) DEFAULT NULL,
                      `ContactNumber` varchar(50) DEFAULT NULL,
                      `UserStatus` varchar(255) DEFAULT NULL,
                      `UserStatusName` longtext,
                      `JobTitle` longtext,
                      `ManagerID` int(11) DEFAULT '0',
                      `ManagerName` varchar(255) DEFAULT NULL,
                      `ManagerFullName` varchar(255) DEFAULT NULL,
                    	`ManagerEmail` varchar(255) DEFAULT NULL,
                      `OrderID` int(11) DEFAULT NULL,
                    	`ProgramID` int(11) DEFAULT NULL,
                      `ProgramTitle` varchar(255) DEFAULT NULL,
                      `ProgramCode` varchar(100) DEFAULT NULL,
                      `ClassTitle` longtext,
                      `ClassCode` varchar(255) NOT NULL,
                      `ClassID` int(11) NOT NULL DEFAULT '0',
                    	`ClassStatusName` longtext,
                    	`ClassLanguage` longtext,
                      `CourseTitle` longtext,
                      `CourseCode` varchar(255) NOT NULL,
                    	`CourseStatus` longtext,
                      `ProgramType` longtext,
                      `ClassDeliveryType` longtext,
                    	`ClassDeliveryTypeCode` varchar(255) DEFAULT NULL,
                      `ProgramRegistrationDate` varchar(41) DEFAULT NULL,
                      `ClassRegistrationDate` varchar(41) DEFAULT NULL,
                      `MasterEnrollCompletionDate` varchar(41) DEFAULT NULL,
                      `EnrollCompletionDate` varchar(41) DEFAULT NULL,
                      `EnrollRegStatus` varchar(100) DEFAULT NULL,
                      `EnrollCompStatus` varchar(100) DEFAULT NULL,
                      `TPCompStatusName` longtext,
                      `EnrCompStatusName` longtext,
                      `EnrRegStatusName` longtext,
                      `MasterEnrollScrore` float(11,2) DEFAULT '0.00',
                      `EnrollScore` float(11,2) DEFAULT NULL,
                      `MasterEnrollSuccessStatus` varchar(20) DEFAULT NULL,
                      `EnrollSuccessStatus` varchar(20) DEFAULT NULL,
                      `OrganizationID` int(11) DEFAULT NULL,
                      `OrganizationName` longtext,
                      `TPIsCurrent` char(1) DEFAULT 'Y',
                      `TPCertPath` int(5) DEFAULT '0',
                    	`EnrollmentRecertifyPath` int(5) DEFAULT '0',
                      `EnrollmentIsCompliance` int(1) DEFAULT NULL,
                      `EnrollmentIsMandatory` varchar(10) DEFAULT NULL,
                      `RegistrationDate` datetime DEFAULT NULL,
                      `CourseID` int(11) DEFAULT NULL,
                      `EnrollmentID` int(11) NOT NULL DEFAULT '0',
                      `TPEnrollmentID` int(11) DEFAULT NULL,
                      `CompletionDate` datetime DEFAULT NULL,
                      `EnrollmentScore` float(11,2) DEFAULT NULL,
                      `EnrollmentContentStatus` varchar(20) DEFAULT NULL,
                    	`WaitlistPriority` int(5) DEFAULT NULL,
                    	`WaitlistFlag` longtext,
                      `EnrollmentCreatedBy` text,
                      `EnrollmentCreatedOn` datetime DEFAULT NULL,
                      `EnrollmentUpdatedBy` text,
                      `EnrollmentUpdatedOn` datetime DEFAULT NULL,
                      `EnrollmentCompletedBy` text,
                      `EnrollmentCompletedOn` datetime DEFAULT NULL,
                      `EnrollmentPreSurAssStatus` varchar(25) DEFAULT NULL,
                      `EnrollmentPreSurAssScore` float(11,2) DEFAULT NULL,
                      `EnrollmentComplianceStatus` int(1) DEFAULT '0',
                      `Waived` varchar(3) NOT NULL DEFAULT '',
                      `MandatoryWaived` varchar(3) NOT NULL DEFAULT '',
                      `CourseCompletionDate` datetime DEFAULT NULL,
                      `CourseCompletionDays` int(5) DEFAULT NULL,
                      `CourseValidityDate` datetime DEFAULT NULL,
                      `CourseValidityDays` int(5) DEFAULT NULL,
                      `Country` varchar(255) DEFAULT NULL,
                      `State` varchar(255) DEFAULT NULL,
                      `Address1` varchar(255) DEFAULT NULL,
                      `ZipCode` varchar(50) DEFAULT NULL,
                      `UserHireDate` datetime DEFAULT NULL,
		          	`operation` varchar(10) NULL DEFAULT NULL,
			          PRIMARY KEY (`RUID`),
			             key sli_op(operation)
			)  ENGINE=InnoDB DEFAULT CHARSET=utf8;";
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
					select 
						enr.id as RUID,
						usr.user_name AS 'UserName',
						usr.id AS UserID,
						usr.full_name AS FullName,
						usr.first_name AS Firstname,
						usr.last_name AS Lastname,
						usr.email AS Email,
						usr.city AS City,
						IF(usr.phone_no != '', usr.phone_no, usr.mobile_no) AS ContactNumber,
						usr.status as UserStatus,
						usrsta.name as UserStatusName,
						plt.name as JobTitle,
						perm.id AS ManagerID,
						perm.user_name AS ManagerName,
						perm.full_name AS ManagerFullName,
						perm.email as ManagerEmail,
						enr.order_id AS 'OrderID',
						prg.id AS 'ProgramID',
						prg.title as 'ProgramTitle',
						prg.code as 'ProgramCode',
						cls.title as 'ClassTitle',
						cls.code as 'ClassCode',
						cls.id AS ClassID,
						clssts.name as ClassStatusName,
						clslng.name as ClassLanguage,
						crs.title as 'CourseTitle',
						crs.code as 'CourseCode',
						crssts.name as CourseStatus,
						prgdel.name as 'ProgramType',
						delty.name as 'ClassDeliveryType',
						delty.code as 'ClassDeliveryTypeCode',
						DATE_FORMAT(menr.reg_date,'%b %d, %Y') as 'ProgramRegistrationDate',
						DATE_FORMAT(enr.reg_date,'%b %d, %Y') as 'ClassRegistrationDate',
						DATE_FORMAT(menr.comp_date,'%b %d, %Y') as 'MasterEnrollCompletionDate',
						DATE_FORMAT(enr.comp_date,'%b %d, %Y') as 'EnrollCompletionDate',
						enr.reg_status as 'EnrollRegStatus',
						enr.comp_status as 'EnrollCompStatus',
						comstatus.name as 'TPCompStatusName',
						compst.name as 'EnrCompStatusName',
						regst.name as 'EnrRegStatusName',
						menr.score as 'MasterEnrollScore',
						enr.score as 'EnrollScore',
						menr.content_status as 'MasterEnrollSuccessStatus',
						enr.content_status as 'EnrollSuccessStatus',
						org.id as OrganizationID,
						org.name AS 'OrganizationName',
						menr.is_current	AS 'TPIsCurrent',
						menr.recertify_path	AS 'TPCertPath',
						enr.recertify_path as EnrollmentRecertifyPath,
						enr.is_compliance AS EnrollmentIsCompliance,
						enr.mandatory AS EnrollmentIsMandatory,
						enr.reg_date AS RegistrationDate,
						enr.course_id AS CourseID,
						enr.id AS EnrollmentID,
						enr.master_enrollment_id AS TPEnrollmentID,
						enr.comp_date AS CompletionDate,
						enr.score AS EnrollmentScore,
						enr.content_status AS EnrollmentContentStatus,
						enr.waitlist_priority as WaitlistPriority,
						waitlist.name as WaitlistFlag,
						enr.created_by AS EnrollmentCreatedBy,
						enr.created_on AS EnrollmentCreatedOn,
						enr.updated_by AS EnrollmentUpdatedBy,
						enr.updated_on AS EnrollmentUpdatedOn,
						enr.comp_by AS EnrollmentCompletedBy,
						enr.comp_on AS EnrollmentCompletedOn,
						enr.pre_status AS EnrollmentPreSurAssStatus,
						enr.pre_score AS EnrollmentPreSurAssScore,
						enr.cmpl_expired AS EnrollmentComplianceStatus,
						IF(exmp.id IS NULL, 'No', IF(exmp.is_compliance = 1 and exmp.exempted_status = 1,'Yes','No')) AS Waived,  
						IF(exmp.id IS NULL, 'No', IF(exmp.is_mandatory = 'Y' and exmp.exempted_status = 1,'Yes','No')) AS MandatoryWaived,
						crs.complete_date AS CourseCompletionDate,
						crs.complete_days AS CourseCompletionDays,
						crs.validity_date AS CourseValidityDate,
						crs.validity_days AS CourseValidityDays,
						country.country_name AS Country,
						sta.state_name AS State,
						usr.addr1 AS Address1,
						usr.zip AS ZipCode,
						usr.hire_date AS UserHireDate,
						NULL
					FROM slt_person usr
						INNER JOIN slt_enrollment enr ON enr.user_id = usr.id
						INNER JOIN slt_course_class cls ON cls.id = enr.class_id
						INNER JOIN slt_course_template crs ON crs.id = enr.course_id AND crs.id = cls.course_id
						LEFT JOIN slt_master_enrollment menr ON  enr.master_enrollment_id = menr.id and menr.user_id =  usr.id  and menr.overall_status != 'lrn_tpm_ovr_rsc' AND  menr.overall_status != 'lrn_tpm_ovr_rsv'
						LEFT JOIN slt_program prg ON prg.id = menr.program_id AND prg.id
						LEFT JOIN slt_enrollment_exempted exmp ON exmp.id = (SELECT id FROM slt_enrollment_exempted WHERE enrollment_id = enr.id and enroll_type = 'class' order by id desc limit 1)
						LEFT JOIN slt_organization org ON org.id = usr.org_id
						LEFT JOIN slt_country country ON country.country_code = usr.country
						LEFT JOIN slt_state sta ON sta.state_code = usr.state AND sta.country_code = country.country_code
						LEFT JOIN slt_person perm ON (perm.id = usr.manager_id)
						LEFT JOIN slt_profile_list_items clssts ON clssts.code = cls.status
						LEFT JOIN slt_profile_list_items regst ON regst.code = enr.reg_status AND regst.lang_code = 'cre_sys_lng_eng'
						LEFT JOIN slt_profile_list_items compst ON compst.code = enr.comp_status AND compst.lang_code = 'cre_sys_lng_eng'
						LEFT JOIN slt_profile_list_items delty ON delty.code = cls.delivery_type
						LEFT JOIN slt_profile_list_items comstatus ON comstatus.code = menr.overall_status AND comstatus.lang_code = 'cre_sys_lng_eng'
						LEFT JOIN slt_profile_list_items prgdel ON prgdel.code = prg.object_type
						LEFT JOIN slt_profile_list_items waitlist ON waitlist.code = enr.waitlist_flag
						LEFT JOIN slt_profile_list_items clslng ON clslng.code = cls.lang_code
						LEFT JOIN slt_profile_list_items crssts ON crssts.code = crs.status
						left join slt_profile_list_items plt ON plt.code = usr.job_title
						left join slt_profile_list_items usrsta ON usrsta.code = usr.status";
			
			if($this->updSync_on != null) {
				$tempDataQuery .= " WHERE (
			        			(usr.updated_on > '" . $this->updSync_on . "')
			        			OR (enr.updated_on > '" . $this->updSync_on . "')
			        			OR (cls.updated_on > '" . $this->updSync_on . "')
			        			OR (crs.updated_on > '" . $this->updSync_on . "')
			        			OR (menr.updated_on > '" . $this->updSync_on . "')
			        			OR (prg.updated_on > '" . $this->updSync_on . "')
			        			OR (org.updated_on > '" . $this->updSync_on . "')
			        			OR (country.updated_on > '" . $this->updSync_on . "')
			        			OR (sta.updated_on > '" . $this->updSync_on . "')
			        			OR (perm.updated_on > '" . $this->updSync_on . "')
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
						RUID,
						UserName,
						UserID,
						FullName,
						Firstname,
						Lastname,
						Email,
						City,
						ContactNumber,
						UserStatus,
						UserStatusName,
						JobTitle,
						ManagerID,
						ManagerName,
						ManagerFullName,
						ManagerEmail,
						OrderID,
						ProgramID,
						ProgramTitle,
						ProgramCode,
						ClassTitle,
						ClassCode,
						ClassID,
						ClassStatusName,
						ClassLanguage,
						CourseTitle,
						CourseCode,
						CourseStatus,
						ProgramType,
						ClassDeliveryType,
						ClassDeliveryTypeCode,
						ProgramRegistrationDate,
						ClassRegistrationDate,
						MasterEnrollCompletionDate,
						EnrollCompletionDate,
						EnrollRegStatus,
						EnrollCompStatus,
						TPCompStatusName,
						EnrCompStatusName,
						EnrRegStatusName,
						MasterEnrollScrore,
						EnrollScore,
						MasterEnrollSuccessStatus,
						EnrollSuccessStatus,
						OrganizationID,
						OrganizationName,
						TPIsCurrent,
						TPCertPath,
						EnrollmentRecertifyPath,
						EnrollmentIsCompliance,
						EnrollmentIsMandatory,
						RegistrationDate,
						CourseID,
						EnrollmentID,
						TPEnrollmentID,
						CompletionDate,
						EnrollmentScore,
						EnrollmentContentStatus,
						WaitlistPriority,
						WaitlistFlag,
						EnrollmentCreatedBy,
						EnrollmentCreatedOn,
						EnrollmentUpdatedBy,
						EnrollmentUpdatedOn,
						EnrollmentCompletedBy,
						EnrollmentCompletedOn,
						EnrollmentPreSurAssStatus,
						EnrollmentPreSurAssScore,
						EnrollmentComplianceStatus,
						Waived,
						MandatoryWaived,
						CourseCompletionDate,
						CourseCompletionDays,
						CourseValidityDate,
						CourseValidityDays,
						Country,
						State,
						Address1,
						ZipCode,
						UserHireDate
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
                            flat.RUID = temp.RUID,
							flat.UserName = temp.UserName,
							flat.UserID = temp.UserID,
							flat.FullName = temp.FullName,
							flat.Firstname = temp.Firstname,
							flat.Lastname = temp.Lastname,
							flat.Email = temp.Email,
							flat.City = temp.City,
							flat.ContactNumber = temp.ContactNumber,
							flat.UserStatus = temp.UserStatus,
							flat.UserStatusName = temp.UserStatusName,
							flat.JobTitle = temp.JobTitle,
							flat.ManagerID = temp.ManagerID,
							flat.ManagerName = temp.ManagerName,
							flat.ManagerFullName = temp.ManagerFullName,
							flat.ManagerEmail = temp.ManagerEmail,
							flat.OrderID = temp.OrderID,
							flat.ProgramID = temp.ProgramID,
							flat.ProgramTitle = temp.ProgramTitle,
							flat.ProgramCode = temp.ProgramCode,
							flat.ClassTitle = temp.ClassTitle,
							flat.ClassCode = temp.ClassCode,
							flat.ClassID = temp.ClassID,
							flat.ClassStatusName = temp.ClassStatusName,
							flat.ClassLanguage = temp.ClassLanguage,
							flat.CourseTitle = temp.CourseTitle,
							flat.CourseCode = temp.CourseCode,
							flat.CourseStatus = temp.CourseStatus,
							flat.ProgramType = temp.ProgramType,
							flat.ClassDeliveryType = temp.ClassDeliveryType,
							flat.ClassDeliveryTypeCode = temp.ClassDeliveryTypeCode,
							flat.ProgramRegistrationDate = temp.ProgramRegistrationDate,
							flat.ClassRegistrationDate = temp.ClassRegistrationDate,
							flat.MasterEnrollCompletionDate = temp.MasterEnrollCompletionDate,
							flat.EnrollCompletionDate = temp.EnrollCompletionDate,
							flat.EnrollRegStatus = temp.EnrollRegStatus,
							flat.EnrollCompStatus = temp.EnrollCompStatus,
							flat.TPCompStatusName = temp.TPCompStatusName,
							flat.EnrCompStatusName = temp.EnrCompStatusName,
							flat.EnrRegStatusName = temp.EnrRegStatusName,
							flat.MasterEnrollScrore = temp.MasterEnrollScrore,
							flat.EnrollScore = temp.EnrollScore,
							flat.MasterEnrollSuccessStatus = temp.MasterEnrollSuccessStatus,
							flat.EnrollSuccessStatus = temp.EnrollSuccessStatus,
							flat.OrganizationID = temp.OrganizationID,
							flat.OrganizationName = temp.OrganizationName,
							flat.TPIsCurrent = temp.TPIsCurrent,
							flat.TPCertPath = temp.TPCertPath,
							flat.EnrollmentRecertifyPath = temp.EnrollmentRecertifyPath,
							flat.EnrollmentIsCompliance = temp.EnrollmentIsCompliance,
							flat.EnrollmentIsMandatory = temp.EnrollmentIsMandatory,
							flat.RegistrationDate = temp.RegistrationDate,
							flat.CourseID = temp.CourseID,
							flat.EnrollmentID = temp.EnrollmentID,
							flat.TPEnrollmentID = temp.TPEnrollmentID,
							flat.CompletionDate = temp.CompletionDate,
							flat.EnrollmentScore = temp.EnrollmentScore,
							flat.EnrollmentContentStatus = temp.EnrollmentContentStatus,
							flat.WaitlistPriority = temp.WaitlistPriority,
							flat.WaitlistFlag = temp.WaitlistFlag,
							flat.EnrollmentCreatedBy = temp.EnrollmentCreatedBy,
							flat.EnrollmentCreatedOn = temp.EnrollmentCreatedOn,
							flat.EnrollmentUpdatedBy = temp.EnrollmentUpdatedBy,
							flat.EnrollmentUpdatedOn = temp.EnrollmentUpdatedOn,
							flat.EnrollmentCompletedBy = temp.EnrollmentCompletedBy,
							flat.EnrollmentCompletedOn = temp.EnrollmentCompletedOn,
							flat.EnrollmentPreSurAssStatus = temp.EnrollmentPreSurAssStatus,
							flat.EnrollmentPreSurAssScore = temp.EnrollmentPreSurAssScore,
							flat.EnrollmentComplianceStatus = temp.EnrollmentComplianceStatus,
							flat.Waived = temp.Waived,
							flat.MandatoryWaived = temp.MandatoryWaived,
							flat.CourseCompletionDate = temp.CourseCompletionDate,
							flat.CourseCompletionDays = temp.CourseCompletionDays,
							flat.CourseValidityDate = temp.CourseValidityDate,
							flat.CourseValidityDays = temp.CourseValidityDays,
							flat.Country = temp.Country,
							flat.State = temp.State,
							flat.Address1 = temp.Address1,
							flat.ZipCode = temp.ZipCode,
							flat.UserHireDate = temp.UserHireDate,
								
							temp.operation =  'update' 
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
			$query = "SELECT RUID from report_enrollment repsum
					join report_deleted_logs del on del.entity_id = repsum.EnrollmentID and del.table_name = 'slt_enrollment'
					WHERE del.deleted_on > '" . $this->delSync_on . "'
						UNION
					SELECT RUID from report_enrollment repsum
					join report_deleted_logs del on del.entity_id = repsum.TPEnrollmentID and del.table_name = 'slt_master_enrollment'
					WHERE del.deleted_on > '" . $this->delSync_on . "'";
			
			return $query;
		} catch(Exception $ex) {
			throw new Exception("Error in reportCollectIds " . $ex->getMessage());
		}
	}
}