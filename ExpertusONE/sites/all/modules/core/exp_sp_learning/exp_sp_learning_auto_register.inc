<?php

include_once(drupal_get_path('module', 'exp_sp_learning') .'/exp_sp_learning.inc');
include_once DRUPAL_ROOT."/dataload/Database.php";
include_once DRUPAL_ROOT."/dataload/enrollment_upload.php";


/**
 * @desc Base class for Auto Enrollment via Cron  
 * @author Shobana
 *
 */
class auto_register_enrollment extends enrollment_upload {
	
	/**
	 * @desc Batch id which is in process
	 * @var Integer
	 */
	private $btId;
	
	/**
	 * @desc Course id which is in process
	 * @var Integer
	 */
	private $courseId;
	
	/**
	 * @desc Class id which is in process
	 * @var Integer
	 */
	private $classId;
	
	/**
	 * @desc is_comp which denotes whether the class is compliance or not
	 * @var Integer
	 */
	private $is_comp;
	
	/**
	 * @desc delType which refers the Delivery type of the Class is in process
	 * @var String
	 */
	private $delType;
	
	/**
	 * @desc objType which refers the object is Class or TP
	 * @var String
	 */
	private $objType;
	
	/**
	 * @desc $tblDet which have the table details
	 * @var Array
	 */
	private $tblDet;
	
	/**
	 * @desc Delay time for DB action performs in loops
	 * It should be in Micro seconds.
	 * @return unknown_type
	 */
	private $delay;
	
	/**
	 * @desc Local variable to hold max of record grop value
	 * for TP enrollment of class temp table
	 * @var unknown_type
	 */
	private $recordGrp;
	
	/**
	 * @desc $tblDet which have the table details
	 * @var Array
	 */
	private $cmplRereg;
	
	private $recertify_path;
	/**
	 * @desc $tpClassIds which refers the selected class ids in tp
	 * @var String
	 */
	
	private $tpClassIds;
	
	/**
	 * @desc $tpUserID which refers unique reference for tp enrollment background process
	 * @var String
	 */
	
	private $custom1;
	/**
	 * @desc $tpUserID which refers the tp enrollment job created user ID
	 * @var Integer
	 */
	
	private $tpUserID;
	
	/**
	 * @desc $mroId which refers class mandatory access
	 * @var String
	 */
	private $mroId;
	/**
	 * @desc Constructor of this class which set the delay time
	 * for redundant process to avoid dead locks.
	 * The default delay time is 5000 milliseconds 
	 * @return NA
	 */
	function __construct(){
		$this->delay = 5000;
	}
	
	/**
	 * (non-PHPdoc)
	 * @see dataload/enrollment_upload#setTableDetails()
	 */
	protected function setTableDetails($tblDet){
		$this->tblDet = $tblDet;
	}
	
	/**
	 * @desc Get stored table details from local variables
	 * @return unknown_type
	 */
	private function getTableDetails(){
		return $this->tblDet;
	}

	/**
	 * @desc Prepare the list of Compliance/Mandatory trainings which 
	 * need to be auto register to permitted users (based on access priv.)
	 * Also process the re-registration for compliance classes whoever 
	 * in expired state.
	 * The list of users who are eligible for register/re-register to a
	 * compliance/mandatory courses will be collected and those records 
	 * will pouplate in to auto registration table.
	 * @return unknown_type
	 */
	public function populateAutoRegstrationRecords(){
		// Process Compliance Re-Registration 
		try{
			$this->complianceAutoReregistration();
		}catch(Exception $e){
			expDebug::dPrint("ERROR in Compliance Re-Registration ".print_r($e,true),1);
		}
		
		// Process Compliance/Mandatory Registration (for First time)
		try{
			$this->mandatoryComplianceAutoRegister();
		}catch(Exception $e){
			expDebug::dPrint("ERROR in Compliance/Mandatory Registration ".print_r($e,true),1);
		}
        
		// Process Mandatory Re-certification
	 	try{
			$this->mandatoryCertificationAutoRegister();
		}catch(Exception $e){
			expDebug::dPrint("ERROR in Compliance/Mandatory Registration ".print_r($e,true),1);
	}
	
		 
	}
	
	/**
	 * @desc Process the records available in auto registration table
	 * Basically it does the actual registration for the 
	 * compliance/mandatory trainings. 
	 * @return unknown_type
	 */
	public function autoRegisterEnrollment(){
		try{
			
			// # Registering shutdown function
			register_shutdown_function(array($this, "autoRegisterErrorHandler"));
			
			$tempDetails = $this->getBatchRecords();
			
			// Values from slt_autoregister_temp table
			$this->courseId = $tempDetails[0]->course_id;
			$this->classId = $tempDetails[0]->class_id;
			$this->btId = $tempDetails[0]->btid;
			$this->is_comp = $tempDetails[0]->is_compliance;
			$this->delType = $tempDetails[0]->delivery_type_code;
			$this->objType = $tempDetails[0]->object_type;
			$this->cmplRereg = $tempDetails[0]->cmpl_rereg;
			$this->tpClassIds = $tempDetails[0]->tp_class_ids;
			$this->custom1 = $tempDetails[0]->tp_enrollment_jobid;
			$this->tpUserID = $tempDetails[0]->tp_user_id;
			$this->recertify_path = $tempDetails[0]->recertify_path;
			$this->mroId = $tempDetails[0]->mro_id;
			// Update the record status of the slt_autoregister_temp
			$this->updateRecStatus('PR', 'R');
			
			// Class Enrollment part
			if($this->objType == 'Class'){
				
				$tblDet['clsTblName'] = 'temp_autoreg_b'.$this->btId;
				$tblDet['loadby'] = 'A-'.$this->btId;
				// If process is tp background enrollment then insert job creator id otherwise insert 2.
				$tblDet['user_id'] = (isset($this->custom1) && (strpos($this->custom1, 'AR-') !== false)) ? $this->tpUserID :  2;
			//	$tblDet['user_id'] = 2;
				$this->setTableDetails($tblDet);
				
				$this->classEnrollment();
			} // TP Enrollment part
			else if($this->objType == 'cre_sys_obt_cur' || $this->objType == 'cre_sys_obt_trn' || $this->objType == 'cre_sys_obt_crt' && $this->recertify_path == 1){
				$tblDet['clsTblName'] = 'temp_autoreg_class_b'.$this->btId;
				$tblDet['tpTblName'] = 'temp_autoreg_b'.$this->btId;
				$tblDet['loadby'] = 'A-'.$this->btId;
				// If process is tp background enrollment then insert job creator id otherwise insert 2.
				$tblDet['user_id'] = (isset($this->custom1) && (strpos($this->custom1, 'AR-') !== false)) ? $this->tpUserID :  2;
			//	$tblDet['user_id'] = 2;
				$this->setTableDetails($tblDet);
				
				$this->tpEnrollment();
			}
			else if($this->objType == 'cre_sys_obt_crt' && $this->recertify_path > 1){
				$tblDet['clsTblName'] = 'temp_autoreg_class_b'.$this->btId;
				$tblDet['tpTblName'] = 'temp_autoreg_b'.$this->btId;
				$tblDet['loadby'] = 'A-'.$this->btId;
				$tblDet['user_id'] = 2;
				$this->setTableDetails($tblDet);
			
				$this->tpEnrollment('R');
			}		
		}catch(Exception $e){
			$pList = db_query('SHOW FULL PROCESSLIST')->fetchAll();
			expDebug::dPrint("Process list while error in Auto Register Enrollment ".print_r($pList,true),1);
			expDebug::dPrint("ERROR in autoRegisterEnrollment ".print_r($e,true),1);
			//trigger_error('Fatal Error', E_USER_ERROR);
			$this->autoRegisterErrorHandler(array('type'=>E_USER_ERROR));
		}
		
	}
	
	
	/*
	 * @desc Auto registration Error Handler.
	 * To take care of rollback the processed records
	 * if any exception occurs in middle of the process.
	 */
	public function autoRegisterErrorHandler($err = array()){
		//Status change when error in creation
		$error = !empty($err) ? $err : error_get_last();
		if(($error['type'] === E_ERROR) || ($error['type'] === E_USER_ERROR)){
			expDebug::dPrint("With in autoRegisterErrorHandler".print_r($error,true),1);
			$fields = array('record_status' => 'PR', 'notifysts' => NULL,'notifyuser' => NULL, 'remarks' => null,'custom3' => null);
			$qry = db_update('slt_autoregister_temp');
			$qry->fields($fields);
			$qry->condition('batch_id',$this->btId,'=');
			$qry->condition('record_status',array('R','MN','IN'),'IN');
			expDebug::dPrintDBAPI("Error handler",$qry);
			$qry->execute();
		}
		expDebug::dPrint("INSIDE auto register error handler ".print_r($err,true),4);
		// remove temp tables
		$this->dropTempTable('temp_autoreg_b');
		if($this->objType != 'Class'){
			$this->dropTempTable('temp_autoreg_class_b');
		} 
	}
	
	/**
	 * @desc: Function for Class Enrollment validation and Execution.
	 */
	private function classEnrollment(){
		try{
			
			// Common Validation starts from here
			// Validation for Registration End date
			$this->registrationEndDateValidation();
				
			// Validation part for Pre-requisite
			$this->prerequisiteValidation();
				
			//Validation for Single Class or Not
			$this->singleClassValidation();
			
			if($this->cmplRereg != 1){
				// Validation for Active Class or not
				$this->aciveClassValidation();
			}
			
			// Validation for multi registration business rule set for the class.
			$ismulreg = $this->multiregistrationValidation();
			if($ismulreg == false)
				$this->enrollmentExistValidation();
				
			if($this->delType == 'lrn_cls_dty_ilt' || $this->delType == 'lrn_cls_dty_vcl'){
				$this->instructorConflictValidation($this->delType);
				$this->sessionCompletionConflic();
			}
			
			$tempAllDetails = $this->getBatchRecords();
		
			$this->validateActiveUser();
		
			if($this->delType == 'lrn_cls_dty_ilt' || $this->delType == 'lrn_cls_dty_vcl'){
				foreach($tempAllDetails as $key => $details){
					expDebug::dPrint("Values for each Result from a Batch".print_r($details,1),4);
						
					$tid = $details->id;
					$userId = $details->userid;
					$cls_id = $details->class_id;
				
					// Enrollment Conlfict with already enrolled Class
					$conflictingClassId = enrollmentsConflictCheck($userId, $cls_id, 'Class');
					if($conflictingClassId > 0){
						$this->updateTempTable('Already existing enrollment has conflict with this class','IN',null,$tid);
						continue;
					}
				
					// Session Conflict for the user as instructor
					$conflictInstructorSession = getSessionClassInstructorConflict($userId, $cls_id);
					if ($conflictInstructorSession > 0 ) {
						$this->updateTempTable('Already existing enrollment has conflict with session of the user as instructor','IN',null,$tid);
					}
				}
			}
			
			
			// Execution part Starts from here
			// Create a Table like slt_enrollment.
			$this->dropTempTable('temp_autoreg_b');
			$this->createTempTable('slt_enrollment','alter');
				
			// Move datas from slt_autoregister_temp table to temp table.
			$this->moveDatatoTempTable();
			
			
			$tabDetail['tblname'] = "temp_autoreg_b".$this->btId;
			$tabDetail['btc_id'] = $this->btId;
			$tabDetail['user_id'] = 2;
			$tabDetail['loadby'] = 'A-'.$this->btId;
			
			 if($this->cmplRereg == 1){
				$this->expiredEnrollmentClassIdUpdate();
				$this->updateExpiredEnrollment();
				
				$cntWbtVod = db_query("SELECT COUNT(1) FROM ".$tabDetail['tblname']." t1 WHERE t1.record_status = :r 
								AND (t1.custom3 = :wbt OR t1.custom3 = :vod ) ",
				array(':r' => 'R',':wbt' => 'lrn_cls_dty_wbt', ':vod' => 'lrn_cls_dty_vod'))->fetchField();
				
				$cntIltVcl = db_query("SELECT COUNT(1) FROM ".$tabDetail['tblname']." t1 WHERE t1.record_status = :r 
								AND (t1.custom3 = :ilt OR t1.custom3 = :vcl ) ",
				array(':r' => 'R',':ilt' => 'lrn_cls_dty_ilt', ':vcl' => 'lrn_cls_dty_vcl'))->fetchField();
				
				if($cntIltVcl > 0){
					$this->updateRecordStatus('WR',$tabDetail['tblname']);
					
					$this->itlVcClassInsert($tabDetail);
					
					$this->updateRecordStatus('R',$tabDetail['tblname']);
			} 
			
				if($cntWbtVod > 0){
			
					$this->wbtVodClassInsert($tabDetail);
				}
				
			
			} else {
			if($this->delType == 'lrn_cls_dty_ilt' || $this->delType == 'lrn_cls_dty_vcl'){
				
				$this->itlVcClassInsert($tabDetail);
			
			}else{
			
				$this->wbtVodClassInsert($tabDetail);
				
			}
			}
				
			$upld = new enrollment_upload();
			$upld->setTableDetails($tabDetail);
			
			// Audit trail entry for Registration and completion
			$upld->insertAuditTrail();
				
			// User Points Insert for registered status
			$upld->insertUserPoints();
			
			// Update the User email id and user uid for notification.
			$upld->updateManagerEmailId();
				
			// Update the manager email id and user uid for notification.
			$upld->updateManagerEmailId(TRUE);
			
			// Insert Notification
			$this->insertMandatoryComplianceNotification();
				
			// Reset the value in custom datalod column in order related tables and slt_autoregister_temp table.
			$this->updCustomDataload();  
			
			if($this->cmplRereg == 1){
				// Update the invalid records to slt_autoregister_temp table
				$this->moveInvalidRecordstoTemp("temp_autoreg_b".$this->btId);
			}
				
			// Update the record status of the slt_autoregister_temp
			$this->updateRecStatus(array('R','MN'), 'CP');
				
			// Delete the executed Records from slt_autoregister_temp table.
			$this->deleteRecordsFromTemp();
			$this->activeMonitoringJobCheck('class');
			$this->dropTempTable('temp_autoreg_b');
			
			// Solr sync up for enrollmets if the sync throws any exception 
			// that should not call deleteInsertRecords(), hence used try..catch.
			try{
				syncSolrData('Enrollment','','Bulk');
				syncSolrData('User','','Bulk');
			}catch(Exception $e){
				// Do nothing
			}
		}catch(Exception $e){
			expDebug::dPrint("ERROR in classEnrollment ".print_r($e,true),1);
			$this->deleteInsertRecords();
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * @desc: Function for TP Enrollment validation and Execution.
	 */
	private function tpEnrollment($recertify_flag=''){
		try{
			
			// Validation part for Pre-requisite
			$this->prerequisiteValidation();
			// Validatin for active tp or not
			$this->validateTpStatus();
			if(!empty($this->tpClassIds)) {
				$this->tpClassValidation();
			}
			// Validation part for TP already registered or not
			$this->validateTpRegisteredorNot();
			
			// Checks Next Module is available for re-certificaion.
			$this->validateModuleExistsorNot();
			
			// Validation for Course and Class count associated to the TP.
			if(empty($this->tpClassIds)) {
			  $this->validateCourseClassCount();
			}
			// Get the program details for valid records.
			if($recertify_flag=='R' && $this->recertify_path > 1 && $this->objType=="cre_sys_obt_crt") {
			$tpDetails = $this->getProgramDetails($recertify_flag);
			} else {				
			$tpDetails = $this->getProgramDetails();
			}
			
			$mandCnt = $tpDetails[0]->mandcnt;
			$checkconf = false;
			$getTPConflictCheck = 0;
			if(!empty($this->tpClassIds)) {
				$classId = $this->tpClassIds;
				$listIds =  explode(',',$classId);
			}
			else {
				$listIds = !empty($tpDetails[0]->clsids) ? explode(',',$tpDetails[0]->clsids) : array();
			}
				$prmId = $tpDetails[0]->program_id;

			//$listDt = explode(',',$tpDetails[0]->dtype);
			if(count($listIds) > 0){
				// Validations for class session conflict with in the classes associated to the TP.
				$getTPConflictCheck = trainingProgramMultiClassConflictCheck($listIds);
					
				if($getTPConflictCheck > 0){
						$this->updateTempTable('Already existing enrollment has conflict with this class','IN',null);
				}else{
					$checkconf = true;
				}
				}
				$this->validateActiveUser();
			if($checkconf === true){
				foreach($tpDetails as $key => $tpVal){
					
					$tId = $tpVal->tid;
					$uId = $tpVal->userids;
					
				if($getTPConflictCheck == 0){
					for($i=0;$i<count($listIds);$i++){
						$conflictInstructor = true;
						$conflictInstructorSession = 0;
						// Validation for class session conflict for the TP with already registered classes.
						$getConflictCheck  = enrollmentsConflictCheck($uId,$listIds[$i],'Class');
						if($getConflictCheck > 0){
							$this->updateTempTable('Already existing enrollment session has conflict with this class','IN',null,$tId);
							break;
						}
							
						$deliveryType = getClassDeliveryType($listIds[$i], 'Short');
						$Instr_check = is_instructor($uId);
						if(($deliveryType == 'ilt' || $deliveryType == 'vc') && $Instr_check == true){
							// Confirm that the user not assigned this class to instruct for learners
							$conflictInstructor = getClassInstructorConflict($uId, $listIds[$i]);
			
							if($conflictInstructor == false){
								$this->updateTempTable('Instructor conflict with this class','IN',null,$tId);
								break;
							}
								
							// Confirm that the user not scheduled to some other class in same session time as instructor
							$conflictInstructorSession = getSessionClassInstructorConflict($uId, $listIds[$i]);
			
							if($conflictInstructorSession > 0 ){
								$this->updateTempTable('Instructor conflict with this class','IN',null,$tId);
								break;
							}
						}
					}
				}
					
			}
			}
			
			$this->updateNotifyUser('certCurrLPRegister');
			
			// Execution part Starts from here
			// Drop and Create Table like slt_enrollment & slt_master_enrollment
			$this->dropTempTable('temp_autoreg_class_b');
			$this->dropTempTable('temp_autoreg_b');
			$this->createTempTable('slt_enrollment','alter','temp_autoreg_class_b');
			$this->createTempTable('slt_master_enrollment','alter');
			
			// Move datas from slt_autoregister_temp table to temp table.
			$this->moveTPDatatoTempTable();
			
			// Waitlist check for ITL/VC Class associated to that TP.
			$tabDetail['tblname'] = "temp_autoreg_class_b".$this->btId;
			$tabDetail['btc_id'] = $this->btId;
			$tabDetail['user_id'] = 2;
			$tabDetail['loadby'] = 'A-'.$this->btId;
			$dtOpt = array('lrn_cls_dty_ilt','lrn_cls_dty_vcl','insert');
			$upld = new enrollment_upload();
			$upld->setTableDetails($tabDetail);
			
			//Allocating seats
			$upld->seatAllocation(); 
			
			// Validation function for Overall status of the training plan.
			//$this->validateProgramOverallStatus(); //Not Needed
			
			// Update the Record status to tp temp table.
			$this->updateTPRecordStatus();
			
			// Insert into slt_order table for TP enrollment.
			$this->insertTPOrder();
			
			// Insert records from temp table into slt_master_enrollment.
			$this->insertTPEnrollment();
			
			// Insert the order items for TP enrollment
			$this->insertTPOrderItems("temp_autoreg_class_b".$this->btId);
			
			// Insert for slt_enrollment table.
			$this->insertEnrollment("temp_autoreg_class_b".$this->btId);
			
			$cntQry = "SELECT COUNT(1) FROM temp_autoreg_class_b".$this->btId." WHERE custom3 IN(:wbt , :vod ) 
														AND record_status = :r AND master_enrollment_id IS NOT NULL AND batch_id = :btid ";
			$args = array(':r' => 'R',':btid' => $this->btId,':wbt' => 'lrn_cls_dty_wbt',':vod' => 'lrn_cls_dty_vod');
			expDebug::dPrintDBAPI("Query for getting the WBT/VOD Class Count",$cntQry,$args);
			$wbtVodCnt = db_query($cntQry, $args)->fetchField();
			expDebug::dPrint("Result of the WBT/VOD Class Count".print_r($wbtVodCnt,1));
			if($wbtVodCnt > 0){
				
				//Insert for slt_enrollment content mapping table.
				$this->insertEnrollmentContentMapping("temp_autoreg_class_b".$this->btId);
			}
			
			// TP roll up for Mark completion status
			$this->markTpCompletionStatus($mandCnt);
			
			$this->markPercentageCompleted();
			$this->insertTPAuditTrailEntry();
			
			$this->insertTPUserPoints();
			
			$tabDetail['tblname'] = "temp_autoreg_b".$this->btId;
			$tabDetail['btc_id'] = $this->btId;
			$tabDetail['user_id'] = 2;
			$tabDetail['loadby'] = 'A-'.$this->btId;
			$upld = new enrollment_upload();
			$upld->setTableDetails($tabDetail);
			
			// Update the User email id and user uid for notification.
			$upld->updateManagerEmailId();
			
			// Update the manager email id and user uid for notification.
			$upld->updateManagerEmailId(TRUE);
			
			// Insert Notification
			$this->insertMandatoryComplianceNotification();
			
			// Reset the value in custom datalod column in order related tables and slt_autoregister_temp table.
			$this->updCustomDataload();
			
			// Update the record status of the slt_autoregister_temp
			$this->updateRecStatus(array('R','MN'), 'CP');
			
			if($this->recertify_path > 1) {
			$this->updatePreviousMasterenrollment();
			}
			
			// Delete the executed Records from slt_autoregister_temp table.
			$this->deleteRecordsFromTemp();
			$this->activeMonitoringJobCheck('tp');
			
			// Delete the Invalid Records from slt_autoregister_temp table.					
			$this->deleteInvalidRecordsFromTemp();
			
			$this->dropTempTable('temp_autoreg_class_b');
			$this->dropTempTable('temp_autoreg_b');  
			
			// Solr sync up for enrollmets if the sync throws any exception 
			// that should not call deleteInsertRecords(), hence used try..catch.
			try{
				syncSolrData('Enrollment','','Bulk');
				syncSolrData('MasterEnrollment','','Bulk');	
				syncSolrData('User','','Bulk');
			}catch(Exception $e){
				// Do nothing
			}
		}catch(Exception $e){
			expDebug::dPrint("ERROR in tpEnrollment ".print_r($e,true),1);
			$this->deleteInsertRecords();
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * @desc: Function for ILT/VC class Insertion
	 */
	private function itlVcClassInsert($tabDetail){
		try{
			
			$dtOpt = array('lrn_cls_dty_ilt','lrn_cls_dty_vcl','insert');
			$upld = new enrollment_upload();
			$upld->setTableDetails($tabDetail);
				
			//Allocating seats
			$upld->seatAllocation();
				
			// Insert for slt_order table.
			$upld->insertOrder($dtOpt);
				
			//Update order_id in temp_autoreg_b.$this->btId table.
			$upld->updateTemp2OrderId($dtOpt);
				
			// Insert for slt_order_items.
			$upld->insertOrderItem($dtOpt);
				
			//Insert for slt_enrollment table.
			$this->insertEnrollment("temp_autoreg_b".$this->btId);
			
		}catch(Exception $e){
			expDebug::dPrint("ERROR in itlVcClassInsert ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * @desc: Function for WBT/VOD class Insertion
	 */
	private function wbtVodClassInsert($tabDetail){
		try{
			
			$dtOpt = array('lrn_cls_dty_wbt','lrn_cls_dty_vod','insert');
				
			$upld = new enrollment_upload();
			$upld->setTableDetails($tabDetail);
				
			//Only for WBT and VOD
			// Insert for slt_order table.
			$upld->insertOrder($dtOpt);
				
			//Update order_id in temp_autoreg_b.$this->btId table.
			$upld->updateTemp2OrderId($dtOpt);
				
			// Insert for slt_order_items.
			$upld->insertOrderItem($dtOpt);
				
			//Insert for slt_enrollment table.
			$this->insertEnrollment("temp_autoreg_b".$this->btId);
			
			//Insert for slt_enrollment content mapping table.
			$upld->insertEnrollContentMap();
		}catch(Exception $e){
			expDebug::dPrint("ERROR in wbtVodClassInsert ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * @desc Fetch the valid records from slt_autoregister_temp table
	 * @return multitype: array
	 */
	private function getBatchRecords(){
		try{
			
			$select = db_select('slt_autoregister_temp','temp');
			$select->addField('temp', 'id', 'id');
			$select->addField('temp', 'object_type');
			$select->addField('temp', 'userids','userid');
			$select->addField('temp', 'crs_id','course_id');
			$select->addField('temp', 'cls_id','class_id');
			$select->addField('temp', 'mro_id','mro_id');
			$select->addField('temp', 'is_compliance','is_compliance');
			$select->addField('temp', 'published_on','published_on');
			$select->addField('temp', 'batch_id','btid');
			$select->addField('temp', 'delivery_type_code','delivery_type_code');
			$select->addField('temp', 'custom4','cmpl_rereg');
			$select->addField('temp', 'recertify_path','recertify_path');
			$select->addField('temp', 'tp_class_ids','tp_class_ids');
			$select->addField('temp', 'tp_enrollment_jobid','tp_enrollment_jobid');
			$select->addField('temp', 'tp_user_id','tp_user_id');
			if(empty($this->btId)){
				//$select->where("temp.batch_id = (select temp.batch_id from slt_autoregister_temp temp where temp.record_status = 'PR' group by batch_id limit 1)");
				$select->condition('temp.record_status', 'PR', '=');
				$select->range(0,1);
			}else{
				$select->condition('temp.batch_id', $this->btId, '=');
				$select->condition('temp.record_status', 'R', '=');
			}
			
			expDebug::dPrintDBAPI("Course details to auto register ",$select);
			$tempDetails = $select->execute()->fetchAll();
				
			expDebug::dPrint("Temp table record details".print_r($tempDetails,true),5);
			
			return $tempDetails;
			
		}catch(EXception $e){
			expDebug::dPrint("ERROR in getBatchRecords ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * @desc Function to get the course and class id associated to the TP
	 * @return multitype: array
	 */
	private function getProgramDetails($recertify_flag=''){
		try{
			
			$select = db_select('slt_autoregister_temp','temp');
			$select->innerJoin("slt_module_crs_mapping",'map','map.program_id = temp.cls_id');
			if($recertify_flag=="R" && $this->objType=="cre_sys_obt_crt" && $this->recertify_path > 1) {
			$select->innerJoin("slt_module",'module','module.id = map.module_id');
			$select->condition('module.sequence',$this->recertify_path,'=');
			}
			$select->innerJoin("slt_course_class",'cls','cls.course_id = map.course_id');
			$select->innerJoin("slt_profile_list_items",'lst','lst.code = cls.delivery_type');
			$select->addField('temp', 'id','tid');
			$select->addField('map', 'program_id');
			$select->addField('temp', 'userids');
			//$select->addExpression('GROUP_CONCAT(DISTINCT map.course_id)','crsids');
			$select->addExpression('GROUP_CONCAT(distinct if(cls.delivery_type = \'lrn_cls_dty_ilt\' OR cls.delivery_type = \'lrn_cls_dty_vcl\' ,cls.id,null))','clsids');
			//$select->addExpression('GROUP_CONCAT(DISTINCT lst.attr1)','dtype');
			$select->addExpression('COUNT(IF(map.is_required=\'Y\',map.id,NULL))','mandcnt');
			$select->condition('temp.batch_id',$this->btId,'=');
			$select->condition('temp.record_status','R','=');
			$select->groupBy('temp.cls_id');
			$select->groupBy('temp.userids');
			$select->orderBy('cls.id');
			
			expDebug::dPrintDBAPI("Training plan details for validation ",$select);
			$res = $select->execute()->fetchAll();
			
			expDebug::dPrint("Result of valid tp records".print_r($res,true),3);
			return $res;
		}catch(Excpetion $ex){
			expDebug::dPrint("ERROR in getProgramDetails ".print_r($ex,true),1);
			throw new Exception($ex->getMessage());
		}
	}
	
	/**
	 * @desc Funtion to validate the registration end date for class with in the batch.
	 */
	private function registrationEndDateValidation(){
		try{
			//expDebug::dPrint('With in registrationEndDateValidation function',4);
			$regEndDte = registerEndDateCheck($this->courseId,$this->classId);
			
			if($regEndDte == 1)
				$this->updateTempTable('Registration End Date is Completed','IN');
		}catch(Excpetion $ex){
			expDebug::dPrint("ERROR in getProgramDetails ".print_r($ex,true),1);
			throw new Exception($ex->getMessage());
		}
	}
	
	/**
	 * @desc Funtion to validate the pre-requisite for class with in the batch.
	 */
	private function prerequisiteValidation(){
		try{
			
			expDebug::dPrint('With in prerequisiteValidation function',4);
			 
			if($this->objType == 'Class'){
				$updPreQry = "UPDATE slt_autoregister_temp tm
											INNER JOIN (SELECT comMap.id1, temp.id FROM slt_autoregister_temp temp
											INNER JOIN slt_common_mapping comMap ON comMap.id1 = temp.crs_id AND comMap.type = 5 AND temp.batch_id= :batch_id
											LEFT JOIN slt_enrollment enr ON enr.user_id = temp.userids AND enr.course_id = comMap.id2 AND enr.comp_status = :cmp
											WHERE comMap.id1 = :crsid AND comMap.type = 5
											AND comMap.object_type = :obttyp AND temp.batch_id = :batch_id AND temp.record_status = :r 
											AND IFNULL(enr.comp_status,'')!= :cmp 
											GROUP BY comMap.id1, temp.userids) x ON x.id = tm.id
											SET tm.record_status = :recst , tm.remarks = :err 
											WHERE tm.batch_id = :batch_id AND x.id = tm.id ";
				
				$args=array(
						':err' => 'Pre-requisite Course exist',
						':batch_id' => $this->btId,
						':recst' => 'IN',
						':r' =>'R',
						':crsid' => $this->courseId,
						':cmp' => 'lrn_crs_cmp_cmp',
						':obttyp' => 'cre_sys_obt_crs'
				);
			}else{
				$updPreQry = "UPDATE slt_autoregister_temp tm
											INNER JOIN (SELECT comMap.id1, temp.id FROM slt_autoregister_temp temp
											INNER JOIN slt_common_mapping comMap ON comMap.id1 = temp.cls_id AND comMap.type = 5 AND temp.batch_id= :batch_id
											LEFT JOIN slt_master_enrollment msenr ON msenr.program_id = comMap.id2 AND msenr.user_id = temp.userids AND msenr.overall_status  = :cmpsts
											LEFT JOIN slt_enrollment enr ON enr.user_id = temp.userids AND IF(comMap.prereq_type= :pretyp ,(msenr.id = enr.master_enrollment_id),(enr.course_id = comMap.id2 )) AND enr.comp_status = :cmp
											WHERE comMap.id1 = :prgid AND comMap.type = 5
											AND comMap.object_type = :obttyp AND temp.batch_id = :batch_id AND temp.record_status = :r 
											AND ((IF(comMap.prereq_type= :pretyp ,IFNULL(msenr.overall_status,'')!= :cmpsts ,IFNULL(enr.comp_status,'')!= :cmp )))
											GROUP BY comMap.id1, temp.userids) x ON x.id = tm.id
											SET tm.record_status = :recst , tm.remarks = :err 
											WHERE tm.batch_id = :batch_id AND x.id = tm.id ";
				
				$args=array(
						':err' => 'Pre-requisite Course exist',
						':batch_id' => $this->btId,
						':recst' => 'IN',
						':r' =>'R',
						':prgid' => $this->classId,
						':pretyp' => 'cre_sys_pre_trp',
						':cmp' => 'lrn_crs_cmp_cmp',
						':cmpsts' => 'lrn_tpm_ovr_cmp',
						':obttyp' => 'cre_sys_obt_trp'
				);
			}
			
			expDebug::dPrintDBAPI("Query for Pre-requisite validation",$updPreQry,$args);
			db_query($updPreQry,$args);
			
		}catch (Exception $ex) {
			expDebug::dPrint("ERROR in prerequisiteValidation ".print_r($ex,true),1);
			throw new Exception($ex->getMessage());
		}
	}
	
	/**
	 * @desc Funtion to validate the course which has single or multiple class.
	 */
	private function singleClassValidation(){
		try{
			/*$maxCnt = getIfSingleClassCron($this->courseId,'',$this->is_comp);
			if($maxCnt[0]->maxcnt > 1) {
			  $sts = empty($this->is_comp) ? 'MultipleMandatoryClassNotification' : 'MultipleComplianceClassNotification';
			  $this->updateTempTable('Multiple Class exist under that Course','MN',$sts);
			}else if($maxCnt[0]->maxcnt == 1){
				$this->updateTempTable('','','SingleComplianceMandatoryNotification');
			}*/
			
			$qrystr = "
				UPDATE slt_autoregister_temp tmp
				INNER JOIN
					(SELECT cls.course_id,auto.userids, COUNT(distinct cls.id) AS maxcnt, COUNT(DISTINCT enr.id) AS cmpcnt
					FROM slt_autoregister_temp auto
					INNER JOIN slt_course_class cls ON cls.course_id = auto.crs_id
					LEFT JOIN slt_group_mapping map ON map.entity_id=cls.id AND map.entity_type = 'cre_sys_obt_cls' and map.group_type = 0
					INNER JOIN slt_person per ON per.id = auto.userids 
					LEFT JOIN slt_user_access_mapping umap ON umap.entity_id=cls.id AND umap.entity_type = 'cre_sys_obt_cls' AND umap.access_type = 0 AND umap.user_id=per.id
					LEFT OUTER JOIN slt_course_class_session ses ON ses.class_id=cls.id AND ses.course_id=cls.course_id
					LEFT OUTER JOIN slt_course_content_mapper content ON content.class_id=cls.id AND content.course_id=cls.course_id
					LEFT OUTER JOIN slt_profile_list_items master_view ON master_view.code=cls.delivery_type
					LEFT OUTER JOIN slt_groups grp ON grp.id=map.group_id and (grp.is_admin =0) and grp.status ='cre_sec_sts_atv'
					LEFT OUTER JOIN slt_group_attributes grpatt ON grpatt.group_id = grp.id";
			if($this->cmplRereg == 1){
				$qrystr .= " LEFT OUTER JOIN slt_enrollment enr ON enr.course_id = auto.crs_id AND enr.class_id = auto.cls_id AND enr.user_id = auto.userids
										WHERE enr.comp_status != 'lrn_crs_cmp_exp' AND (auto.batch_id = :batchId ) AND (cls.course_id = :crsId ) AND auto.record_status = :rst 
										AND (cls.status = 'lrn_cls_sts_atv') AND ((if(umap.id IS NOT NULL, umap.mro, map.mro) = 'cre_sys_inv_man') OR (cls.is_compliance = 1))
						AND (IF ((master_view.attr1='ILT' OR master_view.attr1='VC'), 
						  (SELECT COUNT(cs.id) FROM slt_course_class_session cs 
						    WHERE cs.class_id = ses.class_id 
						      AND cs.id=(SELECT id FROM  slt_course_class_session 
						            WHERE class_id=cs.class_id ORDER BY start_date LIMIT 1) 
												      AND CONCAT(DATE(cs.start_date),' ',CONCAT(cs.start_time,':00')) <  NOW() = 0), 1=1)) AND ";
			}else{
				$qrystr .= " LEFT OUTER JOIN slt_enrollment enr ON enr.user_id = auto.userids AND enr.course_id = auto.crs_id AND enr.reg_status = 'lrn_crs_reg_cnf' AND enr.comp_status = 'lrn_crs_cmp_cmp'
				WHERE (auto.batch_id = :batchId ) AND (cls.course_id = :crsId ) AND auto.record_status = :rst 
				AND ((if(umap.id IS NOT NULL, umap.mro, map.mro) = 'cre_sys_inv_man') OR (cls.is_compliance = 1))
						AND (IF ((master_view.attr1='ILT' OR master_view.attr1='VC'), 
						  (SELECT COUNT(cs.id) FROM slt_course_class_session cs 
						    WHERE cs.class_id = ses.class_id 
						      AND cs.id=(SELECT id FROM  slt_course_class_session 
						            WHERE class_id=cs.class_id ORDER BY start_date LIMIT 1) 
						      AND CONCAT(DATE(cs.start_date),' ',CONCAT(cs.start_time,':00')) <  NOW() = 0 AND cls.status = 'lrn_cls_sts_atv') 
						     , (cls.status = 'lrn_cls_sts_atv')) OR enr.id > 0) AND 
			";
			}
					
					
			$upld = new enrollment_upload();
			$qrystr .= $upld->userGroupAccessValidate();
			$qrystr .= "GROUP BY auto.userids,cls.course_id having maxcnt > 1 ) x ON x.userids = tmp.userids and x.course_id = tmp.crs_id
					SET tmp.notifyuser = if(ifnull(cmpcnt,0) > 0 , :notify2 , :notify )  , 
							tmp.record_status = if(ifnull(cmpcnt,0) > 0 , :status2 , :status ) ,
							tmp.remarks = CONCAT(IFNULL(tmp.remarks,''), ' ' , if(ifnull(cmpcnt,0) > 0 , :remark2 , :remark ))
					WHERE tmp.batch_id = :batchId AND tmp.record_status = :rst ";
			$sts = empty($this->is_comp) ? 'MultipleMandatoryClassNotification' : 'MultipleComplianceClassNotification';
			$arg = array(
					':crsId' => $this->courseId,
					':batchId' => $this->btId,
					':notify' => $sts,
					':notify2' => 'MultipleMandatoryClassRe-registerNotification',
					':status' => 'MN',
					':status2' => 'MN',
					':remark' => 'Multiple Class exist under that Course',
					':remark2' => 'Completed Class exist under that Course',
					':all' => 'All',
					':rst' => 'R'
			);
			expDebug::dPrintDBAPI("Update multiple class entry ",$qrystr,$arg);
			db_query($qrystr,$arg);
			
			$qrystr = "
					UPDATE slt_autoregister_temp 
						SET notifyuser = :notify
					WHERE batch_id = :batchId AND crs_id = :crsId AND record_status = :state 
			";
			if(empty($this->comp) && empty($this->mroId)) {
				$notification = 'RegisterByAdmin';
			}
			else {
				$notification = 'SingleComplianceMandatoryNotification';
			}
			$arg = array(
					':crsId' => $this->courseId,
					':batchId' => $this->btId,
					':notify' => $notification,
					':state' => 'R',
			);
			
			expDebug::dPrintDBAPI("Update notification type for single class entry ",$qrystr,$arg);
			db_query($qrystr,$arg);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in singleClassValidation ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * @desc Funtion to validate the class is active or not
	 */
	private function aciveClassValidation(){
		try{
		$atvSts = isActiveClass($this->courseId, $this->classId);
		if($atvSts == false)
		  $this->updateTempTable('Either (or both) class and course are not active.','IN');
		}catch(Exception $e){
			expDebug::dPrint("ERROR in activeClassValidation ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * @desc Funtion to validate the instructor conflict with the class(ILT/VC)
	 */
	private function instructorConflictValidation($delType){
		try{
			
		  $updateStmt = db_update('slt_autoregister_temp');
		  // Set conditions
		  $updateStmt->condition('batch_id',$this->btId,'=');
		  // Set new values of fields
		  $updateStmt->fields(array(
				'custom3' => $delType,
		  ));
		  expDebug::dPrintDBAPI(' Update for register end on date ', $updateStmt);
		  $updateStmt->execute();
			
		  $updQry = "UPDATE slt_autoregister_temp tm
		  					 INNER JOIN slt_session_instructor_details b on tm.cls_id = b.class_id and (tm.custom3 = :vc or tm.custom3 = :itl ) and tm.userids = b.instructor_id
		  					 INNER JOIN slt_person c on c.id = tm.userids AND b.instructor_id = c.id AND c.is_instructor = :ins
		  					 SET tm.custom4  = CASE WHEN b.id IS NOT NULL THEN concat(ifnull(tm.custom4,''), :err ) ELSE tm.custom4 END,
		  					 tm.record_status = CASE WHEN b.id IS NOT NULL THEN :recst  ELSE tm.record_status END
		  					 WHERE tm.batch_id = :batch_id ";
		  
		  $args=array(':vc' => 'lrn_cls_dty_vcl',
		  		':itl' => 'lrn_cls_dty_ilt',
		  		':ins'	=> 'Y',
		  		':err' => 'Cannot enroll the user, since he is the instructor for the class. ',
		  		':batch_id' => $this->btId,
		  		':recst' => 'IN'
		  );
		  expDebug::dPrintDBAPI("Query for Instructor validation",$updQry,$args);
		  db_query($updQry,$args);
		  //db_query("update slt_autoregister_temp set remarks = custom4 where batch_id = ".$btId)->execute();
		  
		}catch (Exception $ex) {
			expDebug::dPrint("ERROR in instructorConflictValidation ".print_r($ex,true),1);
			throw new Exception($ex->getMessage());
		}
	}
	
	/**
	 * @desc Funtion to validate the enrollment for the class already exist or not.
	 */
	private function enrollmentExistValidation(){
		try {

			//Modified for Validating the Enrollment Commonly
			$sql = "
						UPDATE slt_autoregister_temp temp
						INNER JOIN slt_enrollment enr ON enr.user_id = temp.userids and enr.class_id = temp.cls_id
						SET temp.record_status = :sts ,	temp.remarks = :rmks
						WHERE temp.batch_id = :btid 
						AND enr.reg_status IN ('lrn_crs_reg_cnf','lrn_crs_reg_rsv','lrn_crs_reg_wtl','lrn_crs_reg_ppm')
						AND IF(enr.reg_status = 'lrn_crs_reg_cnf', enr.comp_status IN('lrn_crs_cmp_cmp','lrn_crs_cmp_inp','lrn_crs_cmp_enr','lrn_crs_cmp_att'),1=1)
						";
			
			$args = array(':sts' => 'IN', ':rmks' => 'Enrollment already Exist',':btid' => $this->btId);
			expDebug::dPrintDBAPI('Query for testing the enrollment validation',$sql,$args);
			db_query($sql, $args);
			
		}catch (Exception $ex) {
			expDebug::dPrint("ERROR in enrollmentExistValidation ".print_r($ex,true),1);
			throw new Exception($ex->getMessage());
		}
	}
	private function sessionCompletionConflic(){
		try {
		
			//Modified for Validating the completion date with session date
			$sql = "UPDATE slt_autoregister_temp temp
					INNER JOIN slt_course_class cls ON cls.id = temp.cls_id
					inner join slt_course_template crs on crs.id = cls.course_id
					inner join slt_course_class_session ses on ses.class_id = cls.id
					inner join slt_person per on per.id = temp.userids
					left join slt_profile_list_items usrtime on usrtime.code = per.time_zone
					left join slt_profile_list_items prf on prf.code = ses.timezone
					SET temp.record_status = :sts ,	temp.remarks = :rmks
					WHERE temp.batch_id = :btid
					AND crs.compliance_completed = 'hire_days'
					AND DATE_FORMAT(CONVERT_TZ(ses.start_date,prf.attr2,usrtime.attr2),'%Y-%m-%d') >= DATE_FORMAT(DATE_ADD(per.hire_date, INTERVAL crs.complete_days DAY),'%Y-%m-%d')";
	
			$args = array(':sts' => 'IN', ':rmks' => 'Registration cannot be done as you have a session date after the course completion date. Contact Admin for details.',':btid' => $this->btId);
			expDebug::dPrintDBAPI('Query for testing the sessionCompletionConflict ',$sql,$args);
			db_query($sql, $args);
			
		}catch (Exception $ex) {
			expDebug::dPrint("ERROR in sessionCompletionConflict ".print_r($ex,true),1);
			throw new Exception($ex->getMessage());
		}
	}
	
	private function validateActiveUser(){
		try {

			$sql = "
						UPDATE slt_autoregister_temp temp
						INNER JOIN slt_person per on per.id = temp.userids 
						SET temp.record_status = :sts ,	temp.remarks = :rmks
						WHERE temp.batch_id = :btid AND per.status not in ('cre_usr_sts_atv')";
				
			$args = array(':sts' => 'IN', ':rmks' => 'User is deactived',':btid' => $this->btId);
			expDebug::dPrintDBAPI('Query for testing the validateActiveUserForClass validation',$sql,$args);
			db_query($sql, $args);
				
		}catch (Exception $ex) {
			expDebug::dPrint("ERROR in validateActiveUserForClass ".print_r($ex,true),1);
			throw new Exception($ex->getMessage());
		}
	}
	/**
	 * @desc Funtion to validate the class can be registered multiple times or not.
	 * @return true or false
	 */
	private function multiregistrationValidation(){
		try{
			$select = db_select('slt_business_rule_mapping', 'rules');
			$select->leftJoin('slt_profile_list_items','prof','prof.code = rules.business_rule_code');
			$select->addField('rules', 'id');
			$select->condition('rules.entity_id', $this->classId);
			$select->condition('rules.entity_type', 'cre_sys_obt_cls');
			$select->condition('rules.business_rule_code', 'cre_sys_brl_rra');
			$select->condition('prof.is_active', 'Y','=');
			$result = $select->execute()->fetchAll();
			expDebug::dPrint("Multi Registration Result".print_r($result,true),3);
			if(count($result) > 0){
		
				$deliveryTypeAllowed[] = 'lrn_cls_dty_vod';
				$deliveryTypeAllowed[] = 'lrn_cls_dty_wbt';
		
				$select = db_select('slt_course_class', 'cls');
				$select->addField('cls','delivery_type');
				$select->condition('cls.id', $this->classId);
				$select->condition('cls.delivery_type', $deliveryTypeAllowed, 'IN');
				$deliveryTypeResult = $select->execute()->fetchAll();
				return count($deliveryTypeResult) > 0 ? true : false;
			}
			
			return false;
		}catch (Exception $e) {
			expDebug::dPrint("ERROR in multiregistrationValidation ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * @desc Funtion to validate whether the TP is active or not.
	 * If it is inactive or deleted, then that record will be marked as invalid in record_status 
	 * and will not be involved in the next step process.
	 */
	private function validateTpStatus(){
		try{
			$updQry = "UPDATE slt_autoregister_temp temp
								
								INNER JOIN slt_program prg on temp.cls_id = prg.id
								SET temp.record_status = :recsts , temp.remarks = CONCAT(IFNULL(temp.remarks,''), :rmk )
								WHERE (temp.batch_id = :btid AND prg.id = :clsid
								AND (prg.status not in ('lrn_lpn_sts_atv') or ((prg.price != '') or (prg.price != '0.00'))))";
				
			$args = array(':clsid' => $this->classId, ':recsts' => 'IN', ':btid' => $this->btId,
					 ':rmk' => 'TP is either priced or is in active/deleted state ');
				
			expDebug::dPrintDBAPI("Validation for validateTpStatus", $updQry, $args);
			db_query($updQry,$args);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in validateTpStatus ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	/**
	 * @desc Funtion to validate whether the TP attached classes is active or not.
	 * If it is inactive or deleted, then that record will be marked as invalid in record_status
	 * and will not be involved in the next step process.
	 */
	private function tpClassValidation() {
		try {
			expDebug::dPrint("tp enrollment selected class ids".$this->tpClassIds,5);
			expDebug::dPrint("tp enrollment selected class ids tttttttt".$this->classId,5);
			$updQry = "UPDATE slt_autoregister_temp tmp
			left join slt_course_class cls on cls.id in ($this->tpClassIds)
			left join slt_course_template crs on crs.id = cls.course_id
			SET tmp.record_status = :recsts , tmp.remarks = :rmk 
			where (cls.status not in ('lrn_cls_sts_atv')
			or crs.status not in ('lrn_crs_sts_atv')) and tmp.batch_id = :btid AND tmp.cls_id = :clsid";
			$args = array(':clsid' => $this->classId, ':recsts' => 'IN', ':btid' => $this->btId,
					':rmk' => 'Either (or both) class and course are not active');
	
					expDebug::dPrintDBAPI("Validation for tpClassValidation", $updQry, $args);
					db_query($updQry,$args);
		}catch(Exception $e){
				expDebug::dPrint("ERROR in tpClassValidation ".print_r($e,true),1);
				throw new Exception($e->getMessage());
		}
	}
		
	/**
	 * @desc Funtion to validate whether the TP is already registered or not.
	 * If already registered, then that record will be marked as invalid in record_status 
	 * and will not be involved in the next step process.
	 */
	private function validateTpRegisteredorNot(){
		try{
			
			// Update slt_autoregister_temp record_status column as IN if enrollment already exist.
			$updQry = "UPDATE slt_autoregister_temp temp
								INNER JOIN slt_master_enrollment mse on temp.cls_id = mse.program_id and temp.userids = mse.user_id and mse.recertify_path = :recertify_path
								SET temp.record_status = :recsts , temp.remarks = CONCAT(IFNULL(temp.remarks,''), :rmk )
								WHERE temp.batch_id = :btid AND mse.program_id = :clsid  AND temp.record_status =  :sts  
								AND mse.overall_status IN ( :cmp , :enr , :inp , :ppv , :ppm , :exp , :wtl )
								AND IF(mse.overall_status IN (:cmp , :exp ),mse.is_current = :cur ,1=1 ) ";
			
			$args = array(':clsid' => $this->classId, ':sts' => 'R', ':recsts' => 'IN', ':btid' => $this->btId,
									 ':cmp' => 'lrn_tpm_ovr_cmp', ':enr' => 'lrn_tpm_ovr_enr', ':inp' => 'lrn_tpm_ovr_inp',
									 ':ppv' => 'lrn_tpm_ovr_ppv', ':ppm' => 'lrn_tpm_ovr_ppm', ':exp' => 'lrn_tpm_ovr_exp',
									 ':wtl' => 'lrn_tpm_ovr_wtl', ':rmk' => 'Already TP registered',':cur' => 'Y', ':recertify_path'  => $this->recertify_path);
			
			expDebug::dPrintDBAPI("Validation for TP already registered or not", $updQry, $args,5);
			db_query($updQry,$args);
			
		}catch (Exception $e) {
			expDebug::dPrint("ERROR in validateTpRegisteredorNot ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/*
	 *  Function to validate whether Next level of Module Exists or Not
	 */
	private function validateModuleExistsorNot(){
		try{  
			$qry = db_select("slt_module",'module');
			$qry->leftJoin("slt_autoregister_temp",'temp',"module.id = temp.cls_id and object_type='cre_sys_obt_crt'");
			$qry->addExpression("COUNT(1)","cnt");			
			$qry->condition("module.sequence",$this->recertify_path,'=');
 			expDebug::dPrintDBAPI("Query for checking next ModuleExistsorNot",$qry);
			$modulecnt = $qry->execute()->fetchField();
			expDebug::dPrint("Result of module count".$modulecnt,5);
			
			if($modulecnt == 0) {				
			// Update slt_autoregister_temp record_status column as IN if Next level of module path is not exists for Certification.
			$updQry = "UPDATE slt_autoregister_temp temp
			SET temp.record_status = :recsts , temp.remarks = CONCAT(IFNULL(temp.remarks,''), :rmk )
			WHERE temp.batch_id = :btid AND temp.cls_id = :clsid  AND temp.record_status =  :sts AND temp.recertify_path = :recertify_path";
				
			$args = array(':clsid' => $this->classId, ':sts' => 'R', ':recsts' => 'IN', ':btid' => $this->btId,
					 ':rmk' => 'Next level of module path is not exists for Certification',':recertify_path'  => $this->recertify_path);
			
			expDebug::dPrintDBAPI("Validation for TP already registered or not", $updQry, $args);
			db_query($updQry,$args);
			}
			
			
		}catch (Exception $e) {
			expDebug::dPrint("ERROR in validateTpRegisteredorNot ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * @desc - Function for validating the count of courses and classes associated to the TP.
	 */
	private function validateCourseClassCount(){
		try{
			
			$totcrsmapcnt = getCourseMappingCount($this->classId);
			
			$updQry = "UPDATE slt_autoregister_temp temp
								INNER JOIN (SELECT IF(min(clsdt.maxcnt) = 0, 0, IFNULL(max(clsdt.maxcnt),0)) AS cnt, COUNT(clsdt.crsid) AS crscnt , clsdt.uid AS userid, clsdt.btid, clsdt.tid FROM
								(SELECT COUNT(DISTINCT(if(cls.status='lrn_cls_sts_atv',cls.id,null))) as maxcnt, t1.userids as uid, t1.batch_id as btid, t1.id as tid,
								count(DISTINCT(crs.course_id)) as crsid FROM slt_course_class cls
								INNER JOIN slt_module_crs_mapping crs ON crs.course_id = cls.course_id
								INNER JOIN slt_module module on module.id = crs.module_id
								INNER JOIN slt_autoregister_temp t1 ON t1.cls_id = crs.program_id
								LEFT JOIN slt_course_class_session ses force index(sli_ses_crs) ON ses.class_id=cls.id AND ses.course_id=cls.course_id
								LEFT JOIN slt_course_content_mapper content ON content.class_id=cls.id AND content.course_id=cls.course_id
								LEFT JOIN slt_business_rule_mapping sbrm ON sbrm.entity_id=cls.id AND sbrm.business_rule_code='cre_sys_brl_rra'
								LEFT JOIN slt_enrollment enr ON enr.course_id=cls.course_id AND enr.user_id = t1.userids 
								AND enr.reg_status IN (:cnf , :ppm , :rsv , :wtl )
   							AND IF(enr.reg_status = :cnf , enr.comp_status IN( :cmp , :inp , :enr , :att ),1=1)
   							AND IF(enr.course_id=cls.course_id,enr.class_id=cls.id,1=1)
								WHERE t1.batch_id = :btid AND crs.program_id = :prmid  and module.sequence = :modulesequence
								AND	IF(enr.class_id IS NULL ,1=1,IF (sbrm.id > 0 && sbrm.id is not null, cls.status IN ( :atv , :itv ), cls.status= :atv )
								AND IF(cls.delivery_type= :itl OR cls.delivery_type= :vcl , (SELECT COUNT(cs.id) FROM slt_course_class_session cs WHERE cs.class_id = ses.class_id AND cs.id=(SELECT id FROM  slt_course_class_session WHERE class_id=cs.class_id ORDER BY start_date LIMIT 1) AND CONCAT(DATE(cs.start_date),' ',CONCAT(cs.start_time,':00')) <  IFNULL(CONVERT_TZ(now(), :dttmz ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone)),NOW()) = 0), 1=1)
								AND (IF(cls.delivery_type= :wbt OR cls.delivery_type= :vod , content.id IS NOT NULL AND IF (sbrm.id > 0 && sbrm.id is not null, cls.status IN ( :atv , :itv ), cls.status= :atv ), 1=1)))
								AND IF (sbrm.id > 0 && sbrm.id is not null, 1=1, (cls.registration_end_on >= now() or cls.registration_end_on is null))
								GROUP BY cls.course_id, t1.userids) clsdt
								GROUP BY clsdt.uid
								having crscnt = :mapcnt ) x ON temp.id = x.tid AND temp.batch_id = x.btid
								SET temp.record_status = IF(x.cnt > 1, :notsts , IF(x.cnt <= 0, :recsts , temp.record_status) ), 
								temp.remarks = IF(x.cnt > 1, CONCAT(IFNULL(temp.remarks,''), :rmk ), IF(x.cnt <= 0, CONCAT(IFNULL(temp.remarks,''), :invrmk ) ,temp.remarks) ) ,
								temp.notifyuser = IF(x.cnt > 1, :notstr , temp.notifyuser)
								WHERE temp.record_status = :sts AND temp.batch_id = x.btid";
			
			$args = array(':btid' => $this->btId, ':prmid' => $this->classId, ':mapcnt' => $totcrsmapcnt, ':sts' => 'R', ':recsts' => 'IN', ':notsts' => 'MN', ':modulesequence' => $this->recertify_path, 
										':dttmz' => $defaultTimezone, ':cnf' => 'lrn_crs_reg_cnf', ':ppm' => 'lrn_crs_reg_ppm', ':rsv' => 'lrn_crs_reg_rsv', ':wtl' => 'lrn_crs_reg_wtl',
										':cmp' => 'lrn_crs_cmp_cmp', ':inp' => 'lrn_crs_cmp_inp', ':enr' => 'lrn_crs_cmp_enr', ':att' => 'lrn_crs_cmp_att', ':atv' => 'lrn_cls_sts_atv', 
										':itv' => 'lrn_cls_sts_itv', ':itl' => 'lrn_cls_dty_ilt', ':vcl' => 'lrn_cls_dty_vcl', ':wbt' => 'lrn_cls_dty_wbt', ':vod' => 'lrn_cls_dty_vod',
										':notstr' => 'MultipleMantadoryTPNotification', ':rmk' => 'Multiple Class associated to the Course under this TP', ':invrmk'=>'No Classes under any one of the course.');
			expDebug::dPrintDBAPI("Query for validateCourseClassCount",$updQry, $args);
			db_query($updQry,$args);
			
		}catch(Exception $e){
			expDebug::dPrint("ERROR in validateCourseClassCount ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * @desc Funtion to update slt_autoregister_temp table.
	 */
	private function updateTempTable($errStr='', $status = '', $notifySts = null,$tid = '', $rcdSts = ''){
		try{
			expDebug::dPrint("Inside the updateTempTable function".$status."notify user".$notifySts."id value".$tid);
			//$status = empty($status) ? 'IN' : $status;
			$updateStmt = db_update('slt_autoregister_temp');
			// Set conditions
			$updateStmt->condition('batch_id',$this->btId,'=');
			
			// Set new values of fields
			$fields = array('notifyuser' => $notifySts);
			
			if(!empty($status))
				$fields['record_status'] = $status;
			
			$updateStmt->fields($fields);
			if(!empty($errStr))
				$updateStmt->expression('remarks', "CONCAT(ifnull(remarks,''),'".$errStr."')");
			
			if(!empty($tid))
				$updateStmt->condition('id',$tid,'=');
			
			expDebug::dPrintDBAPI(' Update for register end on date ', $updateStmt);
			$updateStmt->execute();
		}catch (Exception $e) {
			expDebug::dPrint("ERROR in updateTempTable ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * @desc Funtion to delete records from slt_autoregister_temp after the process is completed.
	 * Also all the notifications in the batch is active and inserted in slt_bulk_notification.
	 */
	private function deleteRecordsFromTemp(){
		try{
			$deleteStmt = db_delete('slt_autoregister_temp');
			$deleteStmt->condition('batch_id', $this->btId,'=');
			$deleteStmt->condition('notifysts', 'Y','=');
			$deleteStmt->condition('record_status', 'CP','=');
			expDebug::dPrintDBAPI("delete query for temp table",$deleteStmt);
			$deleteStmt->execute();
		}catch (Exception $e) {
			expDebug::dPrint("ERROR in deleteRecordsFromTemp ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
		
	private function deleteInvalidRecordsFromTemp(){
		try{
			$deleteStmt = db_delete('slt_autoregister_temp');		
			$deleteStmt->where('recertify_path IS NULL');
 			$deleteStmt->condition('object_type', 'cre_sys_obt_crt','=');
			expDebug::dPrintDBAPI("delete query for temp table",$deleteStmt);
			$deleteStmt->execute();
		}catch (Exception $e) {
			expDebug::dPrint("ERROR in deleteInvalidRecordsFromTemp ".print_r($e,true),1);
			throw new Exception($e->getMessage());
	}
	}
	
	/**
	 * @desc Funtion to create temp table in batch process.
	 */
	private function createTempTable($tblName, $op = '', $tblPrf = ''){
		try{
			expDebug::dPrint("Create temp table prefix details".$tblPrf,4);
			if(!empty($tblPrf))
				$tempTblName = $tblPrf.$this->btId;
			else
				$tempTblName = "temp_autoreg_b".$this->btId;
			
			expDebug::dPrint("Create temp table Name details".$tempTblName,4);
			db_query("CREATE TABLE ".$tempTblName."  LIKE ".$tblName);
			if(!empty($op)){
				db_query("ALTER TABLE ".$tempTblName."
					ADD COLUMN operation char(10),
					ADD COLUMN record_status char(2) default 'R',
					ADD COLUMN primary_id int(11) default null,
					ADD COLUMN batch_id int(11),
					ADD COLUMN mapping_id int(11),
					ADD COLUMN notifyuser longtext default null,
					ADD COLUMN user_email varchar(255) default null,
					ADD COLUMN drup_uid int(10) default null,
					ADD COLUMN manager_email varchar(255) default null,
					ADD COLUMN mgr_uid int(10) default null,
					ADD COLUMN mgr_drup_uid int(10) default null,
					ADD COLUMN record_grp int(10) default 1,
					ADD INDEX temp_autoreg_batch_id (batch_id),
					ADD INDEX temp_oper (operation),
					ADD INDEX temp_prime (primary_id),
					ADD INDEX temp_recst (record_status),
					ADD INDEX temp_mapid (mapping_id),
					ADD INDEX temp_cust3 (custom3(255)),
					ADD INDEX temp_recgrp (record_grp) "
					);
			}
		}catch (Exception $e) {
			expDebug::dPrint("ERROR in createTempTable ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * @desc Funtion to drop the created temp table in batch process.
	 */
	private function dropTempTable($tblPrf){
		try{
			db_query("drop table if exists ".$tblPrf.$this->btId);
		}catch (Exception $e) {
			expDebug::dPrint("ERROR in dropTempTable ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * @desc Function to move the valid records from slt_autoregister_temp 
	 * to newly created temp table regarding that processing batch.
	 */
	private function moveDatatoTempTable(){
		try{
			$tblDet = $this->getTableDetails();
		  $enrFields = array('notifyuser','record_status'); 
		  $launchable = 1;
		  expDebug::dPrint("inside comes class id test befffffff".$this->delType,5);
		  if($this->delType == 'lrn_cls_dty_ilt' || $this->delType == 'lrn_cls_dty_vcl'){
		  	$launchable  = 0;
		  	$isPostAssessmentAttached = isPostAssessmentAttached($this->classId,'class');
		  	if($isPostAssessmentAttached == 1){
		  		$launchable  = 1;
		  	}
		  } 
		  $tempquery = db_select('slt_autoregister_temp', 'temp');
		  $tempquery->fields('temp',$enrFields);
		  $tempquery->addExpression("if(temp.mro_id is not null and temp.mro_id = 'cre_sys_inv_man' ,'Y',null)",'mandatory');
		  $tempquery->addExpression("if(temp.is_compliance = 1,1,null)",'is_compliance');
		  $tempquery->addExpression('tp_enrollment_jobid','custom1');
		  $tempquery->addExpression('userids','user_id');
		  $tempquery->addExpression('crs_id','course_id');
		  $tempquery->addExpression('cls_id','class_id');
		  $tempquery->addExpression("'lrn_crs_reg_cnf'",'reg_status');
		  $tempquery->addExpression('now()','reg_date');
		  $tempquery->addExpression('now()','reg_status_date');
		  $tempquery->addExpression("'lrn_crs_cmp_enr'",'comp_status');
		  $tempquery->addExpression("'".$tblDet['user_id']."'",'created_by');
		  $tempquery->addExpression('now()','created_on');
		  $tempquery->addExpression("'".$tblDet['user_id']."'",'updated_by');
		  $tempquery->addExpression('now()','updated_on');
		  $tempquery->addExpression('delivery_type_code','custom3');
		  $tempquery->addExpression("'insert'",'operation');
		  $tempquery->addExpression($this->btId,'batch_id');
		  $tempquery->addExpression('temp.id','mapping_id');
		  $tempquery->addExpression('temp.cls_id','custom2');
		  $tempquery->addExpression("'".$tblDet['loadby']."'",'dataload_by');
		  $tempquery->addExpression("'".$launchable."'",'launchable');
		  $tempquery->condition('temp.batch_id',$this->btId,'=');
		  $tempquery->condition('temp.record_status', array('R', 'MN'),'IN');
		  expDebug::dPrintDBAPI('Data from autoregister table to temporary table', $tempquery);
		  
		  db_insert("temp_autoreg_b".$this->btId)
		  ->from($tempquery)
		  ->execute();
		  
		  $this->groupTempTableRecords("temp_autoreg_b".$this->btId);
	  }catch (Exception $e) {
	  	expDebug::dPrint("ERROR in moveDatatoTempTable ".print_r($e,true),1);
			throw new Exception($e->getMessage());
	  }
	}
	
	/**
	 * @desc Function to move the valid training plan records from slt_autoregister_temp
	 * to newly created temp table regarding that processing batch.
	 */
	private function moveTPDatatoTempTable(){
		try{
			$tblDet = $this->getTableDetails();
			$enrTpFields = array('notifyuser','record_status');
			$temptpquery = db_select('slt_autoregister_temp', 'temp');
			$temptpquery->fields('temp',$enrTpFields);
			$temptpquery->addExpression('cls_id','program_id');
			$temptpquery->addExpression('tp_enrollment_jobid','custom1');
			$temptpquery->addExpression('userids','user_id');
			$temptpquery->addExpression("'lrn_tpm_ovr_enr'",'overall_status');
			$temptpquery->addExpression("if(temp.mro_id is not null,1,0)",'mandatory');
			$temptpquery->addExpression('now()','reg_date');
			$temptpquery->addExpression("'".$tblDet['user_id']."'",'created_by');
			$temptpquery->addExpression('now()','created_on');
			$temptpquery->addExpression("'".$tblDet['user_id']."'",'updated_by');
			$temptpquery->addExpression('now()','updated_on');
			$temptpquery->addExpression('object_type','custom3');
			$temptpquery->addExpression("'insert'",'operation');
			$temptpquery->addExpression($this->btId,'batch_id');
			$temptpquery->addExpression('temp.id','mapping_id');
			$temptpquery->addExpression("'".$tblDet['loadby']."'",'dataload_by');
			$temptpquery->addExpression('temp.recertify_path','recertify_path');
			$temptpquery->condition('temp.batch_id',$this->btId,'=');
			$temptpquery->condition('temp.record_status', array('R', 'MN'),'IN');
			expDebug::dPrintDBAPI('TP Data from autoregister table to temporary table', $temptpquery);
			
			db_insert("temp_autoreg_b".$this->btId)
			->from($temptpquery)
			->execute(); 
			
			// Insert class enrollment in temp table
			$enrFields = array('notifyuser','user_id','batch_id','operation','created_by','created_on','updated_by','updated_on','reg_date');
			$tempclsquery = db_select('temp_autoreg_b'.$this->btId, 'temp');
			$tempclsquery->innerJoin('slt_module_crs_mapping','map','map.program_id = temp.program_id');
			$tempclsquery->innerJoin('slt_module','module','module.id = map.module_id');
			$tempclsquery->innerJoin('slt_course_template','crs','crs.id=map.course_id and crs.status = \'lrn_crs_sts_atv\'');
		//	$tempclsquery->innerJoin('slt_course_class','cls','cls.course_id = crs.id');
		//	$tempclsquery->leftJoin('slt_enrollment','enr','enr.course_id = cls.course_id AND enr.class_id = cls.id AND enr.user_id = temp.user_id AND enr.comp_status = \'lrn_crs_cmp_cmp\'');
			$tempclsquery->fields('temp',$enrFields);
			$tempclsquery->addExpression('crs.id','course_id');
		//	$tempclsquery->addExpression('cls.id','class_id');
			$tempclsquery->addExpression("'lrn_crs_reg_cnf'",'reg_status');
			$tempclsquery->addExpression('now()','reg_status_date');
			$tempclsquery->addExpression("'lrn_crs_cmp_enr'",'comp_status');
		//	$tempclsquery->addExpression("if(enr.id is not null, 'lrn_crs_cmp_cmp', 'lrn_crs_cmp_enr')",'comp_status');
		//	$tempclsquery->addExpression("if(enr.id is not null, enr.compdate, 'null')",'comp_date');
		//	$tempclsquery->addExpression('cls.delivery_type','custom3');
			$tempclsquery->addExpression('temp.custom1','custom1');
			$tempclsquery->addExpression('temp.id','mapping_id');
			$tempclsquery->addExpression('temp.record_status','record_status');
			$tempclsquery->addExpression("'".$tblDet['loadby']."'",'dataload_by');
			$tempclsquery->addExpression('temp.recertify_path','recertify_path');
			$tempclsquery->condition('temp.batch_id',$this->btId,'=');
			$tempclsquery->condition('temp.record_status', 'R','=');
			$tempclsquery->condition('module.sequence', $this->recertify_path,'=');				
			$tempclsquery->where('map.program_id = temp.program_id');
			$tempclsquery->orderBy('temp.user_id');
			expDebug::dPrintDBAPI('Data from moveTPDatatoTempTable - autoregister table to temporary table', $tempclsquery,5);
			
			db_insert("temp_autoreg_class_b".$this->btId)
			->from($tempclsquery)
			->execute();
			
			// Update record group ids for performance improvement
			$this->groupTempTableRecords("temp_autoreg_class_b".$this->btId);
			if(!empty($this->tpClassIds)) {
				$this->updateEnrolledClassId("temp_autoreg_class_b".$this->btId);
			// Update the class id and record status.
			    $this->updateTempClassId("temp_autoreg_class_b".$this->btId);
			}
			else {
				$this->updateEnrolledClassId("temp_autoreg_class_b".$this->btId);
				// Update the class id and record status.
				$this->updateTempClassId("temp_autoreg_class_b".$this->btId);
			}
			
		}catch (Exception $e) {
	  	expDebug::dPrint("ERROR in moveTPDatatoTempTable ".print_r($e,true),1);
			throw new Exception($e->getMessage());
	  }
	}
	
	/**
	 * @desc Set group id in for the populated records. 
	 * Pouplated records needs to be grouped based on certine limit
	 * to avoid database locking or halting.
	 * 
	 * Records should taken for a process based on the group id one by one.
	 * 
	 * @param $tblName
	 * @return unknown_type
	 */
	private function groupTempTableRecords($tblName){
		$limit = getConfigValue('autoregister_limit');
		$count = db_query("SELECT count(1) from $tblName ")->fetchField();
		if($count > $limt){
			$pcnt = round($count/$limit) + 1;
			$grpId = 2;
			for($o=0;$o<$pcnt;$o++){
				$upd = "UPDATE $tblName SET record_grp = :grp_id 
						WHERE record_grp = 1 AND id > $limit  LIMIT $limit ";
				$arg = array(
					':grp_id'=>$grpId,
				);
				expDebug::dPrintDBAPI("Auto Reg temp table group id update qry- ",$upd,$arg);
				db_query($upd,$arg);
				$grpId++;
			}
		}
		$this->recordGrp = db_query("SELECT max(record_grp) from $tblName ")->fetchField();
	}
	
	/**
	 * @desc Insert the records from temp table to slt_enrollment table.
	 */

	private function insertEnrollment($tblName){
		try{
			$tblDet = $this->getTableDetails();
		
			for($l=1;$l<=$this->recordGrp;$l++){
			// For inserting enrollment entries from temp table to slt_enrollment table
			$recFields = array('master_enrollment_id', 'mandatory','is_compliance','user_id', 'course_id', 'class_id','order_id','reg_status','reg_date','reg_status_date',
					'recertify_path', 'comp_status', 'pre_status', 'comp_date', 'valid_from', 'valid_to', 'score', 'content_status', 'pre_score', 'grade', 'progress',
					'waitlist_flag', 'waitlist_priority', 'cmpl_expired', 'created_by', 'created_on', 'updated_by', 'updated_on', 'comp_by', 'comp_on','custom1','launchable');
			
			$recquery = db_select($tblName, 'temp');
			//$recquery->leftJoin('slt_order','ord','ord.custom_dataload = temp.mapping_id');
			$recquery->fields('temp',$recFields);
			$recquery->addExpression('temp.batch_id','custom_dataload');
			$recquery->addExpression("'".$tblDet['loadby']."'",'dataload_by');
			$recquery->condition('temp.record_status', 'R','=');
			$recquery->condition('temp.batch_id', $this->btId,'=');
				$recquery->condition('temp.record_grp', $l,'=');
			expDebug::dPrintDBAPI('Insert the enrollment records from temp table', $recquery,5);
			
			db_insert('slt_enrollment')
			->from($recquery)
			->execute();
				
				//Delay to avoid dead lock
				usleep($this->delay);
				
				$qry = "UPDATE ".$tblName." t
					   INNER JOIN slt_enrollment e on e.course_id = t.course_id and e.class_id = t.class_id and e.user_id = t.user_id
					   SET t.primary_id = e.id 
					   WHERE t.record_grp = :rgrp  and e.dataload_by = :loadby  and t.dataload_by = :loadby  ";
				$arg = array(':rgrp'=>$l, ':loadby'=>$tblDet['loadby']);
				expDebug::dPrintDBAPI("Update enrollment temp table with enrollment id ",$qry,$arg);
				db_query($qry,$arg);
				
			}
		}catch (Exception $e) {
			expDebug::dPrint("ERROR in insertEnrollment ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * @desc Update the custom_dataload column in slt_order and slt_enrollment table 
	 */
	private function updCustomDataload(){
		try{
			$tblDet = $this->getTableDetails();
			$updateStmt = db_update('slt_order');
			
			// Set new values of fields
			$updateStmt->fields(array(
					'custom_dataload' => null
			));
			// Modified by Vincent on 10 June, 2016 for #0065353
			$updateStmt->isNotNull('custom_dataload'); 
			$updateStmt->condition('dataload_by',$tblDet['loadby'],'=');
			$updateStmt->execute();
			
			$tblName = 'temp_autoreg_b'.$this->btId;
			if($this->objType != 'Class'){
				$tblName = 'temp_autoreg_class_b'.$this->btId;
			}
			for($l=1;$l<=$this->recordGrp;$l++){
				$qry = "UPDATE slt_enrollment e
								INNER JOIN ".$tblName." t ON t.primary_id = e.id 
								SET e.custom_dataload = NULL 
								WHERE t.record_grp = :rgrp AND e.custom_dataload = :btid
								AND e.dataload_by = :loadby AND t.dataload_by = :loadby
								";
				$arg = array(
						':rgrp' => $l,
						':btid' => $this->btId,
						':loadby' => $tblDet['loadby']
				);
				expDebug::dPrintDBAPI("Update enrollment custom dataload as null ",$qry,$arg);
				db_query($qry,$arg);
				
				//Delay to avoid dead lock
				usleep($this->delay);
			}
			
			if($this->objType != 'Class'){
				
				$updateStmt = db_update('slt_master_enrollment');
				// Set conditions
				// Modified by Vincent on 10 June, 2016 for #0065353
				$updateStmt->isNotNull('custom_dataload'); 
				$updateStmt->condition('dataload_by',$tblDet['loadby'],'=');
				// Set new values of fields
				$updateStmt->fields(array(
						'custom_dataload' => null
				));
				$updateStmt->execute();
			}
		}catch (Exception $e) {
			expDebug::dPrint("ERROR in updCustomDataload ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * @desc Update the record status column in slt_autoregister_temp
	 * @param $sts - Status of which is to be updated,$updSts - Status to be updated with thae value
	 */
	private function updateRecStatus($sts, $updSts){
		try{
			$updateStmt = db_update('slt_autoregister_temp');
			// Set conditions
			$updateStmt->condition('batch_id',$this->btId,'=');
			$updateStmt->condition('record_status',$sts,is_array($sts) ? 'IN' : '=');
			// Set new values of fields
			$updateStmt->fields(array(
					'record_status' => $updSts
			));
			expDebug::dPrintDBAPI("Update query for temp table",$updateStmt);
			$updateStmt->execute();
		}catch (Exception $e) {
			expDebug::dPrint("ERROR in updateRecStatus ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function updatePreviousMasterenrollment($updSts){		
		try {			
			$updateStmt = db_update('slt_master_enrollment');
			// Set conditions			
			$updateStmt->condition('recertify_path',$this->recertify_path-1, '=');
			$updateStmt->condition('overall_status','lrn_tpm_ovr_exp', '=');
			// Set new values of fields
			$updateStmt->fields(array(
					'overall_status' => 'lrn_tpm_ovr_cmp',
					'is_current' => 'N'
			));
			expDebug::dPrintDBAPI("Update query for slt_master_enrollment table",$updateStmt);
			$updateStmt->execute();			
			
		} catch(Exception $e) {
			expDebug::dPrint("ERROR in updatePreviousMasterenrollment ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
		
	}
	
	/**
	 * @desc Funciton for making the notification entries
	 */
	private function insertMandatoryComplianceNotification(){
		try{
			
			// Validate the Notification frequency Date TODO
			
			
			$tblDet['tblname'] = "temp_autoreg_b".$this->btId;
			$tblDet['clstblname'] = "temp_autoreg_class_b".$this->btId;
			$tblDet['btc_id'] = $this->btId;
			$tblDet['user_id'] = 2;
			$tblDet['loadby'] = 'A-'.$this->btId;
			$tblDet['enr_type'] = ($this->objType == 'Class') ? 'Class' : 'tp';
			$notDetArr = array();
			$upld = new enrollment_upload();
			$upld->setTableDetails($tblDet);
			if($this->objType == 'Class'){
				$grpBy = 'group by t2.notifyuser,t2.custom3,t2.class_id,t2.course_id ';
				$colNm = 't2.class_id as class_id ,t2.course_id as course_id,t2.notifyuser as notifystring,t2.custom3 as deltype,t2.is_compliance as is_comp';
				$unionQry = 'SELECT '.$colNm.' from '."temp_autoreg_b".$this->btId.' t2 LEFT JOIN slt_enrollment enr on enr.order_id = t2.order_id WHERE t2.batch_id =  :btid  and t2.record_status IN ( :r , :n )AND (t2.comp_status !=:nsw OR t2.comp_status is null)'. $grpBy;
				$args=array(
						':btid' => $this->btId,
						':r' => 'R',
						':n' => 'MN',
						':nsw' =>'lrn_crs_cmp_nsw',
				);
			}else {
				$grpBy = 'group by t2.notifyuser,t2.custom3,t2.program_id';
				$colNm = 't2.program_id as program_id,t2.notifyuser as notifystring,t2.custom3 as deltype';
				$unionQry = 'SELECT '.$colNm.' from '."temp_autoreg_b".$this->btId.' t2 LEFT JOIN slt_enrollment enr on enr.order_id = t2.order_id WHERE t2.batch_id =  :btid  and t2.record_status IN ( :r , :n ) '. $grpBy;
				$args=array(
						':btid' => $this->btId,
						':r' => 'R',
						':n' => 'MN',
				);
			}
			
			
			expDebug::dPrintDBAPI('Query for Notification details',$unionQry,$args);
			$notDetails = db_query($unionQry, $args)->fetchAll();
			
			$selNotDet = db_select('slt_notification_info','ninfo');
			$selNotDet->innerJoin('slt_notification_frame','frame','frame.notification_id = ninfo.id');
			$selNotDet->addField('ninfo', 'id');
			$selNotDet->addField('ninfo', 'notification_code');
			$selNotDet->addField('ninfo', 'lang_code');
			$selNotDet->addField('ninfo', 'locale_code');
			$selNotDet->addField('ninfo', 'profile_code');
			$selNotDet->addField('ninfo', 'notification_sendto');
			$selNotDet->addField('ninfo', 'entity_type');
			$selNotDet->addField('ninfo', 'notification_cc');
			$selNotDet->addField('ninfo', 'notification_admin');
			$selNotDet->addField('ninfo', 'notification_title');
			$selNotDet->addField('ninfo', 'notification_description');
			$selNotDet->addField('ninfo', 'notification_type');
			$selNotDet->addField('ninfo', 'frequency_data_type');
			$selNotDet->addField('ninfo', 'frequency_data_value');
			$selNotDet->addField('ninfo', 'status');
			$selNotDet->addExpression("group_concat(concat('''', frame.lang_code, ''''))", 'frame_lang');
			$selNotDet->groupBy('ninfo.id');
			$selNotDet->condition('ninfo.lang_code','cre_sys_lng_eng','=');
			$selNotDet->condition('ninfo.notification_code',array('man_comp_multiple_class_register','man_comp_multiple_re-register','register_mandatory_compliance_by_admin','compliance_register_vc_by_admin','compliance_register_wbt_by_admin','mandatory_tp_multiple_register','cert_curr_lp_register_by_admin','cert_curr_lp_recertify_by_admin','cert_curr_lp_completed','cert_curr_lp_waitlist_register_by_admin','compliance_course_re_register_by_admin','register_by_admin','register_wbt_by_admin','register_vc_by_admin'),'IN');
			expDebug::dPrintDBAPI("Notification info details",$selNotDet);
			
			$resNotDet = $selNotDet->execute()->fetchAll();
			expDebug::dPrint("Get the Notification details".print_r($resNotDet,1));
			
			$notDetArr = array();
			
			foreach($resNotDet as $key => $value){
				expDebug::dPrint("Notification details key".$key.'values'.print_r($value,1),4);
				$notDetArr[$value->notification_code] = $value;
				
				$notSts = $value->status == 'cre_ntn_sts_itv' ? 'N' : 'Y';
				$this->updateNotifyStatus($value->notification_code, $notSts);
				
				expDebug::dPrint("notification multidimentional array".print_r($notDetArr,1),5);
			}
			
			foreach ($notDetails as $res => $val ){
				expDebug::dPrint("Fetch the result of notification".print_r($val,1),4);
				
				if($this->objType == 'Class'){
					$courseId = $val->course_id;
					$classId = $val->class_id;
					$iscomp = $val->is_comp;
				} else
					$programId = $val->program_id;
				
				$delType = $val->deltype;
				$notifyString = $val->notifystring;
				
				expDebug::dPrint("Passed Notify String ".$notifyString,4);
				//fix for issue #0059935;
	 			if($notifyString == 'Waitlist')
	 				continue;
			
				if($notifyString == 'MultipleMandatoryClassNotification' || $notifyString == 'MultipleComplianceClassNotification'){
					$upld->enrollmentNotificationInsert('man_comp_multiple_class_register', $courseId, $classId, 2,$notDetArr,$notifyString);
				} else if($notifyString == 'MultipleMantadoryTPNotification'){
					$upld->enrollmentNotificationInsert('mandatory_tp_multiple_register', $programId, '', 2,$notDetArr,$notifyString);
				} else if($notifyString == 'certCurrLPRegister' || $notifyString == 'certCurrLPWaitlistRegister' || $notifyString == 'certCurrLPRegisterAndCompleted'){
					
				if($this->recertify_path > 1) {
					$notCode = 'cert_curr_lp_recertify_by_admin';
					} else {	 				
					$notCode = ($notifyString == 'certCurrLPRegister') ? 'cert_curr_lp_register_by_admin' : 'cert_curr_lp_waitlist_register_by_admin';
					}
					$upld->enrollmentNotificationInsert($notCode, $programId, '', 2,$notDetArr,$notifyString,$this->recertify_path);
					if($notifyString == 'certCurrLPRegisterAndCompleted')
						$upld->enrollmentNotificationInsert('cert_curr_lp_completed', $programId, '', 2,$notDetArr,'certCurrLPRegisterAndCompleted');
				} else if($notifyString == 'MultipleMandatoryClassRe-registerNotification'){
					$upld->enrollmentNotificationInsert('man_comp_multiple_re-register', $courseId, $classId, 2,$notDetArr,$notifyString);
				}else if($notifyString == 'RegisterByAdmin'){
					$classDeliveryType = getClassDeliveryType($classId, 'Short');
					
					switch ($classDeliveryType) {
						case 'wbt':
						case 'vod':
							$notificationCode = 'register_wbt_by_admin';
							break;
						case 'vc':
							$notificationCode = 'register_vc_by_admin';
							break;
						default:
							$notificationCode = 'register_by_admin';
							break;
					}
					//		$notCode = ($notifyString == 'Register') ? 'cert_curr_lp_register_by_admin' : 'cert_curr_lp_waitlist_register_by_admin';
						
						$upld->enrollmentNotificationInsert($notificationCode, $courseId, $classId, 2,$notDetArr,$notifyString);
					}
				 
				else{
					if($this->cmplRereg == 1)
						$not = 'compliance_course_re_register_by_admin';
					else
					$not = ($delType== 'lrn_cls_dty_ilt') ? 'register_mandatory_compliance_by_admin' : (($delType== 'lrn_cls_dty_vcl') ? 'compliance_register_vc_by_admin' : 'compliance_register_wbt_by_admin');
					$upld->enrollmentNotificationInsert($not, $courseId, $classId, 2,$notDetArr,$notifyString);
				}
			}
			
		}catch(Exception $e){
			expDebug::dPrint("ERROR in insertMandatoryComplianceNotification ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	
	}
	
	/**
	 * @desc Update the notifysts column in slt_autoregister_temp
	 * @param $notCode - Notification Code,$notSts - Status to update 'Y' or 'N'
	 */
	private function updateNotifyStatus($notCode,$notSts){
		try{
			
				// Update slt_autoregister_temp for notify status
				$updateStmt = db_update('slt_autoregister_temp');
				// Set conditions
				$updateStmt->condition('batch_id',$this->btId,'=');
				
				if($notSts == 'Y')
					$updateStmt->condition('notifysts', '' ,'IS NULL');
					
				if($notCode == 'man_comp_multiple_class_register')
					$updateStmt->condition('notifyuser',array('MultipleMandatoryClassNotification','MultipleComplianceClassNotification','MultipleMandatoryClassRe-registerNotification'),'IN');
				else if($notCode == 'man_comp_multiple_re-register')
					$updateStmt->condition('notifyuser','MultipleMandatoryClassRe-registerNotification','=');
				else if($notCode =='mandatory_tp_multiple_register')
					$updateStmt->condition('notifyuser','MultipleMantadoryTPNotification','=');
				else if($notCode =='cert_curr_lp_register_by_admin')
					$updateStmt->condition('notifyuser','certCurrLPRegister','=');
				else if($notCode =='register_by_admin')
					$updateStmt->condition('notifyuser','RegisterByAdmin','=');
				else
					$updateStmt->condition('notifyuser','SingleComplianceMandatoryNotification','=');
					
				// Set new values of fields
				$updateStmt->fields(array(
						'notifysts' => $notSts
				));
				expDebug::dPrintDBAPI("Update query notify status",$updateStmt);
				$updateStmt->execute();
			
		}catch(Exception $e){
			expDebug::dPrint("ERROR in updateNotifyStatus ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * @desc Update the Overall status of the training plan in temp table
	 * Already Enrolled status, Waitlist status, Expired and Completed status will 
	*/
	private function validateProgramOverallStatus(){
		try{
			
			$config           = getConfig("exp_sp");
			$cert_days_expire = isset($config["cert_expire_reminder_days"]) ? $config["cert_expire_reminder_days"] : 10;
			
			$updQry = "UPDATE slt_autoregister_temp temp
			INNER JOIN (SELECT mse.id,CASE WHEN mse.overall_status = :cmp AND
 			(NOW() > (CASE WHEN p.expires_in_unit = 'days' then DATE_SUB(DATE_ADD(mse.comp_date, INTERVAL p.expires_in_value DAY), INTERVAL :intv DAY)
			WHEN p.expires_in_unit = 'months' THEN DATE_SUB(DATE_ADD(mse.comp_date, INTERVAL p.expires_in_value DAY), INTERVAL :intv MONTH)
			WHEN p.expires_in_unit = 'years' THEN DATE_SUB(DATE_ADD(mse.comp_date, INTERVAL p.expires_in_value DAY), INTERVAL :intv YEAR)
			END)) THEN temp.record_status ELSE :recsts END as status,
			mse.program_id AS prgid,mse.comp_date,mse.overall_status,mse.recertify_path,mse.custom4,t1.id AS tid, t1.batch_id as btid 
			FROM slt_master_enrollment mse
			LEFT JOIN slt_autoregister_temp t1 ON t1.cls_id = mse.program_id AND t1.userids = mse.user_id
			LEFT JOIN slt_program p ON p.id = t1.cls_id
			WHERE t1.batch_id = :btid AND t1.record_status = :sts AND mse.is_current = :cur AND 
			mse.overall_status NOT IN ( :cln , :rsc , :rsv , :inc ) ) x ON temp.id = x.tid and temp.batch_id = x.btid
			SET temp.record_status = x.status, temp.remarks = CONCAT(IFNULL(remarks,''), :rmk )
			WHERE temp.record_status = :sts AND temp.batch_id = x.btid";
				
			$args = array(':btid' => $this->btId, ':sts' => 'R', ':recsts' => 'IN', ':intv' => $defaultTimezone, ':rsv' => 'lrn_tpm_ovr_rsv', ':rsc' => 'lrn_tpm_ovr_rsc',
					':cmp' => 'lrn_tpm_ovr_cmp', ':cln' => 'lrn_tpm_ovr_cln', ':inc' => 'lrn_tpm_ovr_inc', ':cur' => 'Y',':rmk' => 'TP Already registered.');
			expDebug::dPrintDBAPI("Query for validateProgramOverallStatus",$updQry, $args);
			db_query($updQry,$args);
			
			
		}catch(Exception $e){
			expDebug::dPrint("ERROR in validateProgramOverallStatus ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	
	}
	
	/**
	 * @desc : Function to insert order details for TP enrollments from temp table.
	 */
	private function insertTPOrder(){
		try{
			
			// For Insert the Order entries for TP Enrollment
			$tblDet = $this->getTableDetails();
			$cusFields = array('created_on', 'created_by', 'updated_by', 'updated_on');
			$cusquery = db_select("temp_autoreg_b".$this->btId, 'customfield');
			$cusquery->fields('customfield',$cusFields);
			$cusquery->addField('customfield','user_id');
			$cusquery->addExpression("'lrn_crs_reg_cnf'",'order_status');
			$cusquery->addExpression('customfield.mapping_id','custom_dataload');
			$cusquery->addExpression("'".$tblDet['loadby']."'",'dataload_by');
			$cusquery->condition('customfield.batch_id', $this->btId,'=');
			$cusquery->condition('customfield.record_status', 'R','=');
			expDebug::dPrintDBAPI('Custom field attached TP query', $cusquery);
			
			db_insert('slt_order')
			->from($cusquery)
			->execute();
			
			//Update order_id in temp_autoreg_b.$this->btId table.
			$this->updateAutoTempOrderId();
			
		}catch(Exception $e){
			expDebug::dPrint("ERROR in insertTPOrder ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * @desc Update order id in temp table
	 * @return unknown_type
	 */
	private function updateAutoTempOrderId(){
		
		try{
			$tblDet = $this->getTableDetails();
			$updQry = "UPDATE temp_autoreg_b".$this->btId." temp2 
								 INNER JOIN slt_order ord ON ord.custom_dataload = temp2.mapping_id AND ord.dataload_by = '".$tblDet['loadby']."'
								 SET temp2.order_id = ord.id
								 WHERE temp2.batch_id = :btid AND temp2.record_status = :r ";
			
			$args = array(':btid' => $this->btId,':r' => 'R');
			db_query($updQry,$args);
			}catch(Exception $e){
				expDebug::dPrint("ERROR in updateAutoTempOrderId ".print_r($e,true),1);
			throw new Exception($e->getMessage());
			}
	} 
	
	/**
	 * @desc: Function to insert the master enrollment details
	 */
	private function insertTPEnrollment(){
		try{
			$tblDet = $this->getTableDetails();
			// For inserting tp enrollment entries from temp table to slt_master_enrollment table
			$recFields = array('order_id', 'mandatory','program_id','user_id', 'overall_status', 'reg_date','recertify_path', 'created_by', 'created_on', 'updated_by', 'updated_on','custom1');
				
			$recquery = db_select("temp_autoreg_b".$this->btId, 'temp');
			$recquery->innerJoin('slt_order','ord',"ord.custom_dataload = temp.mapping_id AND ord.dataload_by = '".$tblDet['loadby']."'");
			$recquery->fields('temp',$recFields);
			$recquery->addExpression('temp.id','custom_dataload');
			$recquery->addExpression("'".$tblDet['loadby']."'",'dataload_by');
			$recquery->condition('temp.record_status', 'R','=');
			$recquery->condition('temp.batch_id', $this->btId,'=');
			expDebug::dPrintDBAPI('Insert the enrollment records from temp table', $recquery);
				
			db_insert('slt_master_enrollment')
			->from($recquery)
			->execute();
			/* expDebug::dPrint("get master enrollment last inserted id".$lastId,5);
			$isProgramLaunchable = isProgramLaunchable($lastId);
			if($isProgramLaunchable){
				$updateStmt = db_update('slt_master_enrollment');
				$updateStmt->condition('id', $lastId,'=');
				$updateStmt->fields(array('launchable' => 1));
				expDebug::dPrintDBAPI(' update isProgramLaunchable master enrolment ' , $updateStmt);
				$master_enroll_upd = $updateStmt->execute();
			} */
			
			// Update the master_enrollment_id to enrollments temp table.
			$this->updateMasterEnrollmentId("temp_autoreg_class_b".$this->btId);
			
		}catch(Exception $e){
				expDebug::dPrint("ERROR in insertTPEnrollment ".print_r($e,true),1);
			throw new Exception($e->getMessage());
			}
	}
	
	/**
	 * @desc Update each record status into the base temp table.
	 * @return unknown_type
	 */
	private function updateTPRecordStatus(){
		try{
			$qry = "UPDATE temp_autoreg_b".$this->btId." t1
							INNER JOIN temp_autoreg_class_b".$this->btId." t2 on t2.mapping_id = t1.id
							SET t1.record_status = :inv 
							WHERE t2.record_status = :inv ";
			
			$args = array(":inv" => 'IN');
			expDebug::dPrintDBAPI("Query for updating the TP record status from enrollment waitlist validation",$qry,$args);
			db_query($qry,$args);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in updateTPRecordStatus ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * @desc Update master enrollment id in temp table
	 * @param $tblName
	 * @return unknown_type
	 */
	private function updateMasterEnrollmentId($tblName){
		try{
			$tblDet = $this->getTableDetails();
			
			for($l=1;$l<=$this->recordGrp;$l++){
				$updQry = "UPDATE temp_autoreg_class_b".$this->btId." temp
				INNER JOIN temp_autoreg_b".$this->btId." t1 ON t1.id = temp.mapping_id
				INNER JOIN slt_master_enrollment mse ON mse.custom_dataload = t1.id AND mse.dataload_by = '".$tblDet['loadby']."'
				SET temp.master_enrollment_id = mse.id, temp.order_id = mse.order_id,
				temp.custom2 = mse.program_id
				WHERE temp.mapping_id = t1.id AND t1.batch_id = :btid AND temp.record_grp = ".$l;
					
				$args = array(':btid' => $this->btId);
					
				expDebug::dPrintDBAPI("Update master enrollment id in enrollment temp table",$updQry,$args);
				db_query($updQry,$args);
				
				//Delay to avoid dead lock
				usleep($this->delay);
			}
			
		}catch(Exception $e){
			expDebug::dPrint("ERROR in updateMasterEnrollmentId ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function insertTPOrderItems($tblName){
		try{
			$tblDet = $this->getTableDetails();
			
			for($l=1;$l<=$this->recordGrp;$l++){
			// For inserting enrollment entries from temp table to slt_enrollment table
			$recFields = array('order_id', 'user_id', 'course_id', 'class_id','created_on', 'created_by', 'updated_by', 'updated_on');
				
			$recquery = db_select("temp_autoreg_class_b".$this->btId, 'temp');
			$recquery->innerJoin('temp_autoreg_b'.$this->btId,'t2','t2.id = temp.mapping_id');
			$recquery->fields('temp',$recFields);
			$recquery->addExpression('temp.custom2','program_id');
			$recquery->addExpression('t2.custom3','program_type');
			$recquery->addExpression('1.00000','convertion_rate');
			$recquery->addExpression("'USD'",'currency_type');
			$recquery->addExpression("'".$tblDet['loadby']."'",'dataload_by');
			$recquery->condition('temp.record_grp', $l,'=');
			$recquery->condition('t2.record_status', 'R','=');
			$recquery->condition('t2.batch_id', $this->btId,'=');
			
			expDebug::dPrintDBAPI('Insert the order items records from temp table', $recquery);
				
			db_insert('slt_order_items')
			->from($recquery)
			->execute();
				
				//Delay to avoid dead lock
				usleep($this->delay);
			}
			
		}catch(Exception $e){
			expDebug::dPrint("ERROR in insertTPOrderItems ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	
	private function markTpCompletionStatus($req){
		try{
			expDebug::dPrint("Query for markTpCompletionStatus");
			
			// Update the Enrollment Completion status for already enrolled classes.
		//	$this->updateEnrollmentCompletionStatus();
			
			$assCnt = db_select("slt_survey_mapping",'assmap');
			$assCnt->innerJoin("slt_survey",'srv',"srv.id = assmap.survey_id AND srv.type = 'sry_det_typ_ass'");
			$assCnt->addExpression("COUNT(1)","cnt");
			$assCnt->condition("assmap.object_type","cre_sys_obt_cls",'!=');
			$assCnt->condition("assmap.object_id",$this->classId,'=');
			$assCnt->condition("assmap.pre_status",'0','=');
			expDebug::dPrintDBAPI("Query for checking the assessment count",$assCnt);
			$hasAssemt = $assCnt->execute()->fetchField(); 
			expDebug::dPrint("REsult of assessment count".$hasAssemt,4);
			if($hasAssemt > 0)
				return '';
			
			if($req == 0){
				for($l=1;$l<=$this->recordGrp;$l++){
					$updQry = "UPDATE slt_master_enrollment AS me
					INNER JOIN (SELECT e.master_enrollment_id,e.user_id,e.comp_status, e.comp_date as compdate,
					MAX(CASE WHEN e.comp_status= :cmp THEN 0 ELSE 1 END) cmp_status,
					MAX(CASE WHEN e.comp_status in (:enr ,:inp ,:att ,:nsw ) THEN 1 ELSE 0 END) inp_status,
					MAX(CASE WHEN e.comp_status in (:inc ,:nsw ) THEN 1 ELSE 0 END) inc_status
					FROM  temp_autoreg_class_b".$this->btId." e
					LEFT JOIN  slt_module_crs_mapping m ON m.course_id = e.course_id and m.program_id= :prgmid
					WHERE m.program_id = :prgmid AND m.is_required = :req	
					and e.master_enrollment_id is not null and e.record_grp = :rgrp 
					GROUP BY e.master_enrollment_id, e.user_id) enrol ON me.id = enrol.master_enrollment_id 
					SET me.overall_status = CASE WHEN (enrol.cmp_status = 0 AND enrol.inp_status = 0) THEN :tpcmp ELSE
					(CASE WHEN (enrol.cmp_status = 1 AND enrol.inc_status = 1) THEN :tpinc ELSE me.overall_status  END) END
					,   me.comp_date  = CASE WHEN enrol.cmp_status IN (0,1) AND enrol.inp_status = 0 THEN enrol.compdate ELSE me.comp_date END
					,   me.updated_by = CASE WHEN enrol.cmp_status IN (0,1) AND enrol.inp_status = 0 THEN enrol.user_id ELSE me.updated_by END
					,   me.updated_on = CASE WHEN enrol.cmp_status IN (0,1) AND enrol.inp_status = 0 THEN NOW() ELSE me.updated_on END
					WHERE me.overall_status NOT IN (:cnl ,:tpcmp ,:exp ,:ppm ,:ppv ,:tpinc )
					AND   me.program_id = :prgmid ";
					$args = array(':prgmid' => $this->classId, ':req' => 'Y', ':tpcmp' => 'lrn_tpm_ovr_cmp',':cnl' => 'lrn_tpm_ovr_cln',':ppv' => 'lrn_tpm_ovr_ppv',
							':exp' =>	'lrn_tpm_ovr_exp',':tpinc' => 'lrn_tpm_ovr_inc',':cmp' => 'lrn_crs_cmp_cmp',':inc' => 'lrn_crs_cmp_inc',':ppm' => 'lrn_tpm_ovr_ppm',
							':enr' =>	'lrn_crs_cmp_enr',':inp' => 'lrn_crs_cmp_inp',':att' => 'lrn_crs_cmp_att',':nsw' => 'lrn_crs_cmp_nsw', ':rgrp'=>$l);
				
					expDebug::dPrintDBAPI("Update Query for Mark completion status",$updQry,$args);
					db_query($updQry,$args);
					
					//Delay to avoid dead lock
					usleep($this->delay);
				}
			} else {
				for($l=1;$l<=$this->recordGrp;$l++){
					$updQry = "UPDATE slt_master_enrollment AS me
					INNER JOIN (SELECT e.master_enrollment_id,e.user_id, e.comp_date as compdate,
					MIN(CASE WHEN e.comp_status= :cmp THEN 0 ELSE 1 END) cmp_status,
					MAX(CASE WHEN e.comp_status in (:enr ,:inp ,:att ) THEN 1 ELSE 0 END) inp_status,
					MAX(CASE WHEN e.comp_status in (:inc ,:nsw ) THEN 1 ELSE 0 END) inc_status
					FROM  temp_autoreg_class_b".$this->btId." e
					WHERE e.master_enrollment_id is not null and e.record_grp = :rgrp 
					GROUP BY e.master_enrollment_id, e.user_id) enrol ON me.id = enrol.master_enrollment_id AND me.user_id = enrol.user_id 
					SET me.overall_status = CASE WHEN (enrol.cmp_status = 0 AND enrol.inp_status = 0) THEN :tpcmp ELSE
					(CASE WHEN (enrol.cmp_status = 1 AND enrol.inp_status = 0 AND enrol.inc_status = 1) THEN :tpinc ELSE me.overall_status  END) END
					,   me.comp_date  = CASE WHEN enrol.cmp_status IN (0,1) AND enrol.inp_status = 0 THEN enrol.compdate ELSE me.comp_date END
					,   me.updated_by = CASE WHEN enrol.cmp_status IN (0,1) AND enrol.inp_status = 0 THEN enrol.user_id ELSE me.updated_by END
					,   me.updated_on = CASE WHEN enrol.cmp_status IN (0,1) AND enrol.inp_status = 0 THEN NOW() ELSE me.updated_on END
					WHERE me.overall_status NOT IN (:cnl ,:tpcmp ,:exp ,:ppm ,:ppv ,:tpinc )
					AND   me.program_id = :prgmid ";
				
					$args = array(':prgmid' => $this->classId, ':req' => 'Y', ':tpcmp' => 'lrn_tpm_ovr_cmp',':cnl' => 'lrn_tpm_ovr_cln',':ppv' => 'lrn_tpm_ovr_ppv',
							':exp' =>	'lrn_tpm_ovr_exp',':tpinc' => 'lrn_tpm_ovr_inc',':cmp' => 'lrn_crs_cmp_cmp',':inc' => 'lrn_crs_cmp_inc',':ppm' => 'lrn_tpm_ovr_ppm',
							':enr' =>	'lrn_crs_cmp_enr',':inp' => 'lrn_crs_cmp_inp',':att' => 'lrn_crs_cmp_att',':nsw' => 'lrn_crs_cmp_nsw', ':rgrp'=>$l);
				
					expDebug::dPrintDBAPI("Update Query for Mark completion status",$updQry,$args);
					db_query($updQry,$args);
					
					//Delay to avoid dead lock
					usleep($this->delay);
				}
			}
			
			$this->updateWaitlistStatus();
			$this->updateCompletionNotification();
			// Mark percentage Completed
			//$this->markPercentageCompleted();
			$this->UpdateProgressforTP();
			
		}catch(Exception $e){
			expDebug::dPrint("ERROR in markTpCompletionStatus ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function insertTPAuditTrailEntry(){
		try{
			$tblDet = $this->getTableDetails();
			$audQry = db_select('slt_master_enrollment','menr');
			$audQry->innerJoin("temp_autoreg_b".$this->btId,"temp","temp.user_id = menr.user_id and temp.batch_id =".$this->btId." and temp.record_status = 'R' and temp.program_id = menr.program_id");
			$audQry->innerJoin("slt_person","per","per.id = menr.user_id");
			$audQry->leftJoin("slt_profile_list_items","spli","spli.code = per.time_zone");
			$audQry->addExpression('menr.created_by','logged_user_id');
			$audQry->addExpression('menr.program_id','entity_id');
			$audQry->addExpression("'cre_sys_obt_cls'",'entity_type');
			$audQry->addExpression("'exp_sp_learning'",'module_name');
			$audQry->addExpression("'tpregistered'",'functionality_name');
			$audQry->addExpression("'Inserted'",'logged_user_action');
			$audQry->addExpression('menr.user_id','mod_user_id');
			$audQry->addExpression('menr.id','new_value');
			$audQry->addExpression('CONVERT_TZ(CONCAT(DATE_FORMAT(CURDATE(), _utf8\'%Y-%m-%d\'),\' \',CURTIME()) ,(SELECT pro1.attr2 FROM slt_profile_list_items pro1 WHERE pro1.code= per.time_zone), spli.attr2)','esign_date_time');
			$audQry->addExpression('spli.code','timezone');
			$audQry->addExpression('now()','created_on');
			$audQry->addExpression("'".$tblDet['loadby']."'",'dataload_by');
			$audQry->where('menr.custom_dataload = temp.id');
			$audQry->condition('menr.dataload_by',$tblDet['loadby'],'=');
				
			expDebug::dPrintDBAPI("Query for Audit Trail entry for TP enrollment",$audQry);
			
			db_insert('slt_audit_trail')
			->from($audQry)
			->execute();
			
		}catch(Exception $e){
			expDebug::dPrint("ERROR in insertTPAuditTrailEntry ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function insertTPUserPoints(){
		try{
			$upld = new enrollment_upload();
			$moduleSts = $upld->getModuleStatus('exp_sp_admin_userpoints');
			
			if($moduleSts == 0)
				return '';
			
			$tblDet = $this->getTableDetails();
			$sel = db_select('slt_master_enrollment','menr');
			$sel->innerJoin("temp_autoreg_b".$this->btId,"temp","temp.user_id = menr.user_id and temp.batch_id =".$this->btId." and temp.record_status = 'R' and temp.program_id = menr.program_id");
			$sel->leftJoin("slt_master_points","msp","msp.code = IF(menr.overall_status = 'lrn_tpm_ovr_cmp' , 'complete_class_training' , 'register_class' )");
			$sel->addField('menr', 'user_id');
			$sel->addField('menr', 'id','entity_id');
			$sel->addExpression("'tp'",'entity_type');
			$sel->addExpression('msp.code','action_code');
			$sel->addExpression("'insert'",'operation_flag');
			$sel->addExpression('msp.points','earned_points');
			$sel->addExpression('msp.points','total_points');
			$sel->addExpression(1,'created_by');
			$sel->addExpression('now()','created_on');
			$sel->addExpression(1,'updated_by');
			$sel->addExpression('now()','updated_on');
			$sel->addExpression("'".$tblDet['loadby']."'",'dataload_by');
			$sel->where('menr.custom_dataload = temp.id');
			$sel->condition('menr.dataload_by',"A-$this->btId",'=');
			$sel->condition('menr.overall_status',array('lrn_tpm_ovr_cln','lrn_tpm_ovr_inc'),'NOT IN');
			
			expDebug::dPrintDBAPI("Query for User points entry for TP enrollment",$audQry);
			
			db_insert('slt_user_points')
				->from($sel)
				->execute();
			
		}catch(Exception $e){
			expDebug::dPrint("ERROR in insertTPUserPoints ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
		
	}
	private function exportToCsv($jobName,$enrType) {
		try {
		$resFile = array();
		$fileName = 'autoregister'.$jobName.'.csv';
		$filePath = getConfigValue('dataload_file_path');
		$fileFullpath = $filePath."/".$fileName;

		if($enrType == 'tp') {
			$sql = 'SELECT "Tp code", "Tp title", "Username", "Fullname", "Email", "Remarks"
			UNION ALL
			select tmp.cls_code,tmp.cls_title,per.user_name,per.first_name,per.email,tmp.remarks from slt_autoregister_temp tmp inner join 
					slt_person per on tmp.userids=per.id
			WHERE tmp.tp_enrollment_jobid = "'.$jobName.'" and tmp.record_status in (\'IN\')
			INTO OUTFILE "'.$fileFullpath.'"
			CHARACTER SET UTF8 
			FIELDS TERMINATED BY \'|\'
			LINES TERMINATED BY \'\n\'';
		} else {
			$sql = 'SELECT "Class code", "Class title", "Username", "Fullname", "Email", "Remarks"
			UNION ALL
			select tmp.cls_code,tmp.cls_title,per.user_name,per.first_name,per.email,tmp.remarks from slt_autoregister_temp tmp inner join
					slt_person per on tmp.userids=per.id
			WHERE tmp.tp_enrollment_jobid = "'.$jobName.'" and tmp.record_status in (\'IN\')
			INTO OUTFILE "'.$fileFullpath.'"
			CHARACTER SET UTF8
			FIELDS TERMINATED BY \'|\'
			LINES TERMINATED BY \'\n\'';
		}
		expDebug::dPrintDBAPI('export to csv ',$sql,5);
		$res = db_query($sql);
		
		$update = db_update('slt _temp_background_jobs');
					$update->fields(array(
							'file_name'=> $fileName,
					));
					$update->condition('job_name',$jobName,'=');
		 $update->execute();
		 if(file_exists($fileFullpath)) {
		 	$result = $fileFullpath;
		 }
		 else {
		 	$result = '';
		 }
		
		return $result;	
		}catch(Exception $e){
			expDebug::dPrint("ERROR in exportToCsv ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	
	}
	// Notification triggered once the tp enrollment background process completes.
	private function generateTPNotification($subMsg='',$bodyMsg='',$name='',$emailId='',$userid='',$enrType) {
		try {
		if($enrType == 'class') 
			$notiName = 'class_background_enrollment';
		else 
			$notiName = 'tp_background_enrollment';
		$ccEmail = null;
		$selectSql = db_select('slt_notification_info','sni');
		$selectSql->addField('sni', 'status', 'status');
		$selectSql->addField('sni', 'notification_cc', 'notification_cc');
		$selectSql->addField('sni', 'notification_sendto', 'notification_sendto');
		$selectSql->condition('sni.notification_code',$notiName,'=');
		$notificationStatus = $selectSql->execute()->fetchAll();
		$notification_status = $notificationStatus[0]->status;
		if($notification_status == 'cre_ntn_sts_atv') {
		$ccEmail = $notificationStatus[0]->notification_cc;
		$emailSendTo = $notificationStatus[0]->notification_sendto;
		if(!empty($ccEmail))
			$mailIdWithCc=  $emailSendTo.",".$ccEmail;
		else
			$mailIdWithCc=  $emailSendTo;

		$explodeMailIdWithCc = array_filter(array_unique(explode(",", $mailIdWithCc )));
		if((count($explodeMailIdWithCc) > 1) &&(in_array("cre_ntn_rpt_usr", $explodeMailIdWithCc))) {
				$removedFirstOne = $explodeMailIdWithCc;
				$removedFirstValue = array_shift($removedFirstOne);
				$ccValue = getSendToMailCc(implode(',',$removedFirstOne));
		}
		$notificationInfo = array();
		$notificationInfo['tokens_string'] = 'subject>|' . $subMsg . '~|' . $bodyMsg;
		if($enrType == 'class') {
			$notificationInfo['message_id'] = 'class_background_enrollment';
			$notificationInfo['message_type'] = 'Class Enrollment Job Status to admin';
		}else {
			$notificationInfo['message_id'] = 'tp_background_enrollment';
			$notificationInfo['message_type'] = 'TP Enrollment Job Status to admin';
		}
		$notificationInfo['send_type'] = 'php mailer';
		$notificationInfo['lang_code'] = 'cre_sys_lng_eng';
		$setLanguage = 'cre_sys_lng_eng';
		expDebug::dPrint("email id check".$emailId,5);
		// Notification for users whose Email Id given in the Job creation.
			if($emailId){
				$nid = db_insert('slt_notification') // Table name no longer needs {}
				->fields(array(
						'msg_id' => $notificationInfo['message_id'],
						'lang_code' => $setLanguage,
						'msg_type' => $notificationInfo['message_type'],
						'token_str' => $notificationInfo['tokens_string'],
						'user_id' => $userid,
						'send_to_id' => $userid,
						'send_to_name' => $name,
						'send_to_email' => $emailId,
						'send_to_email_cc' => $ccValue,
						'send_type' => $notificationInfo['send_type'],
						'send_status' => 'N',
						'attach_content' => '1',
						'created_by'=> $userid,
						'created_on' => now(),
						'updated_on' => now()
				))
				->execute();
			}
		}
		} catch(Exception $e){
			expDebug::dPrint("ERROR in generateTPNotification ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	private function activeMonitoringJobCheck($enrType){
		try {
		
			$res = $this->getAutoRegisterCount('AR-');
			$select = db_select('slt_temp_background_jobs','tpj');
			$select->addField('tpj','job_name','job_name');
			$select->addField('tpj','custom1','custom1');
			$select->addField('per','email','email');
			$select->addField('tpj','created_by','created_by');
			$select->addField('tpj','created_on','created_on');
			$select->addField('per','full_name','full_name');
			$select->addField('per','first_name','first_name');
			$select->condition('tpj.status', 'inprogress', '=');
			$select->condition('tpj.job_name', 'AR-%', 'LIKE');
			$select->innerjoin('slt_person', 'per', 'per.id = tpj.created_by and per.id = tpj.updated_by');
			expDebug::dPrintDBAPI('inside comes slt_temp_background_jobs',$select);
			$jobDetails = $select->execute()->fetchAll();
			expDebug::dPrint("job detail array".print_r($jobDetails,1),5);
			foreach($jobDetails as $result) {
				// Check the records count for each job
				$jobCount = $this->getAutoRegisterCount($result->job_name);
				
				if ($jobCount == 0 ) {
					if($enrType == 'tp') {
						$selectSql =  db_select('slt_master_enrollment','mas');
						$selectSql->addExpression('count(1)','cnt');
						$selectSql->addField('prg','title','title');
						$selectSql->condition('mas.custom1', $result->job_name, '=');
						$selectSql->innerjoin('slt_program', 'prg', 'prg.id = mas.program_id');
						$selectSql->groupBy('mas.custom1');
						expDebug::dPrintDBAPI("success record count",$selectSql,5);
	
						$selectRes = $selectSql->execute()->fetchAll();
						expDebug::dPrint("query record count".print_r($selectRes,1),5);
						$successRecordsCount = $selectRes[0]->cnt;
						expDebug::dPrint("success record count".$failureRecordsCount,5);
	
						// Calculate failure record count
						$failureRecordsCount = (($result->custom1) - ($successRecordsCount));
						expDebug::dPrint("failed record count".$failureRecordsCount,5);
					} else {
						$selectSql =  db_select('slt_enrollment','enr');
						$selectSql->addExpression('count(1)','cnt');
						$selectSql->addField('cls','title','title');
						$selectSql->condition('enr.custom1', $result->job_name, '=');
						$selectSql->innerjoin('slt_course_class', 'cls', 'cls.id = enr.class_id');
						$selectSql->groupBy('enr.custom1');
						expDebug::dPrintDBAPI("success record count",$selectSql,5);
						
						$selectRes = $selectSql->execute()->fetchAll();
						expDebug::dPrint("query record count".print_r($selectRes,1),5);
						$successRecordsCount = $selectRes[0]->cnt;
						expDebug::dPrint("success record count".$failureRecordsCount,5);
						
						// Calculate failure record count
						$failureRecordsCount = (($result->custom1) - ($successRecordsCount));
						expDebug::dPrint("failed record count".$failureRecordsCount,5);
					}
					// Update the status once process is completed
					$update = db_update('slt_temp_background_jobs');
					$update->fields(array(
							'status'=>'completed',
							'updated_on' => now()
					));
					$update->condition('job_name',$result->job_name,'=');
					expDebug::dPrintDBAPI("slt_temp_background_jobs status check",$update);
					$update->execute();
					// Get the invalid record count
					$invalidRecCount = $this->getAutoRegisterInvalidRecordCount($result->job_name);
					expDebug::dPrint("invalid record count".$invalidRecCount,5);
					$dataloadFilePath = getConfigValue('dataload_file_path');
					// Check folder exists or not ( dataload folder path in ini file)
					$flag = 0;
					if( !empty($dataloadFilePath) && is_dir($dataloadFilePath)) {
						expDebug::dPrint("folder exists");
						$flag = 1;
					}
				
				$noteMsg = '';
				if($invalidRecCount > 0 && !empty($dataloadFilePath) && $flag == 1) {
					
					$updateRes = $this->exportToCsv($result->job_name,$enrType);
					
					expDebug::dPrint("update rsul test".$updateRes,5);
					if( !empty($updateRes) ) {
						expDebug::dPrint("inside comes rrrrrrrr");
						$noteMsg = '';
						$attachmentFilePath = $updateRes;
					} else {
						expDebug::dPrint("inside comes else");
						$noteMsg = 'Note: Please contact support if attachment with list of failed enrollment records is missing in this email.';
						$attachmentFilePath = 'Nil';
					}
					
				}elseif ($invalidRecCount > 0 && !empty($dataloadFilePath) && $flag == 0){
					expDebug::dPrint("inside comes else yyyyy");
					$noteMsg = 'Note: Please contact support if attachment with list of failed enrollment records is missing in this email.';
					$attachmentFilePath = 'Nil';
				}
				elseif  ($invalidRecCount > 0 && empty($dataloadFilePath)) {
					expDebug::dPrint("inside else comes yyyyyyefdsfsd");
					$noteMsg = 'Note: Please contact support if attachment with list of failed enrollment records is missing in this email.';
					$attachmentFilePath = 'Nil';
				}
				else {
								expDebug::dPrint("inside else comes fdgfdf");
					$noteMsg = '';
					$attachmentFilePath = 'Nil';
				}
				if($enrType == 'tp') {
				$select = db_select('slt_temp_background_jobs','stb');
				$select->innerjoin('slt_program','prg','prg.id = stb.program_id');
				$select->addField('stb','updated_on','updated_on');
				$select->addField('stb','file_name','file_name');
				$select->addField('prg','title','title');
				$select->condition('stb.job_name', $result->job_name, '=');
				expDebug::dPrintDBAPI("soundari testing",$select);
				$selectResult = $select->execute()->fetchAll();
				expDebug::dPrint("soun result array".print_r($selectResult,1),5);
				
				expDebug::dPrint("soun result array".print_r($selectRes,1),5);
				
				$subj = 'Enrollments for the training, '.$selectResult[0]->title.', is completed - '.$selectResult[0]->updated_on;
					$body = 'tp_title>|' . $selectResult[0]->title . '~|' .
							'first_name>|' . $result->first_name . '~|' .
							'job_file_name>|' .$attachmentFilePath . '~|' .
							'created_by>|' . $result->full_name . '~|' .
							'started_on>|' . $result->created_on . '~|' .
							'completed_on>|' . $selectResult[0]->updated_on . '~|' .
							'total_records>|' . ((!empty($result->custom1)) ? $result->custom1 : 'Nil') . '~|' .
							'success_records>|' . ((!empty($successRecordsCount)) ? $successRecordsCount : 'Nil') . '~|' .
							'failure_records>|' . ((!empty($failureRecordsCount)) ? $failureRecordsCount : 'Nil') . '~|' .
							'note_msg>|' . ($noteMsg);
					$this->generateTPNotification($subj,$body,$result->full_name,$result->email,$result->created_by,$enrType);
				} else {
					$select = db_select('slt_temp_background_jobs','stb');
					$select->innerjoin('slt_course_class','cls','cls.id = stb.program_id');
					$select->addField('stb','updated_on','updated_on');
					$select->addField('stb','file_name','file_name');
					$select->addField('cls','title','title');
					$select->condition('stb.job_name', $result->job_name, '=');
					expDebug::dPrintDBAPI("soundari testing class enr",$select);
					$selectResult = $select->execute()->fetchAll();
					expDebug::dPrint("soun result array".print_r($selectResult,1),5);
					
					expDebug::dPrint("soun result array class enrollmenr".print_r($selectRes,1),5);
					
					$subj = 'Enrollments for the class, '.$selectResult[0]->title.', is completed - '.$selectResult[0]->updated_on;
					$body = 'class_title>|' . $selectResult[0]->title . '~|' .
							'first_name>|' . $result->first_name . '~|' .
							'job_file_name>|' .$attachmentFilePath . '~|' .
							'created_by>|' . $result->full_name . '~|' .
							'started_on>|' . $result->created_on . '~|' .
							'completed_on>|' . $selectResult[0]->updated_on . '~|' .
							'total_records>|' . ((!empty($result->custom1)) ? $result->custom1 : 'Nil') . '~|' .
							'success_records>|' . ((!empty($successRecordsCount)) ? $successRecordsCount : 'Nil') . '~|' .
							'failure_records>|' . ((!empty($failureRecordsCount)) ? $failureRecordsCount : 'Nil') . '~|' .
							'note_msg>|' . ($noteMsg);
					$this->generateTPNotification($subj,$body,$result->full_name,$result->email,$result->created_by,$enrType);
					
				}
					
				}
			}
				
			if($res == 0) {
				$jobName = 'tp_enrollment_background_process';
				
				// update monitoring jobs, when all records proccessed.
				$updateSql = db_update('slt_monitoring_jobs');
				$updateSql->fields(array(
						'custom0'=>0
				));
				$updateSql->condition('job_name',$jobName,'=');
				$updateSql->execute();
			}
		}catch(Exception $e){
			expDebug::dPrint("ERROR in activeMonitoringJobCheck ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
// Get user count based on job id - TP background enrollment process
	private function getAutoRegisterCount($searchTerm = '') {
		try {
			if ($searchTerm == '') {
				return 0;
			}
			$select = db_select('slt_autoregister_temp','tmp');
			$select->addExpression('count(1)','cnt');
			$select->condition('tmp.tp_enrollment_jobid', '%'.$searchTerm.'%', 'LIKE');
			$select->condition('tmp.record_status', array('IN'), 'NOT IN');
			expDebug::dPrintDBAPI("soun cdn check",$select);
			$res = $select->execute()->fetchField();
			return $res;	
		} catch(Exception $e){
			expDebug::dPrint("ERROR in activeMonitoringJobCheck ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	// Get the invalid record count for each job
	private function getAutoRegisterInvalidRecordCount($searchTerm) {
		try {
		
			$select = db_select('slt_autoregister_temp','tmp');
			$select->addExpression('count(1)','cnt');
			$select->condition('tmp.tp_enrollment_jobid', '%'.$searchTerm.'%', 'LIKE');
			$select->condition('tmp.record_status', array('IN'), 'IN');
			expDebug::dPrintDBAPI("soun cdn check getAutoRegisterInvalidRecordCount",$select);
			$res = $select->execute()->fetchField();
			return $res;
		} catch(Exception $e){
			expDebug::dPrint("ERROR in getAutoRegisterInvalidRecordCount ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	private function updateNotifyUser($notifyStr){
		try{
			$fields = array('notifyuser' => $notifyStr);
			$updTemp = db_update("slt_autoregister_temp");
			$updTemp->fields($fields);
			$updTemp->condition('record_status', 'R', '=');
			$updTemp->condition('notifyuser', '', 'IS NULL');
			$updTemp->condition('batch_id',$this->btId,'=');
			expDebug::dPrintDBAPI("Query for updating the notification string",$updTemp);
			$updTemp->execute();
		} catch(Exception $e){
			expDebug::dPrint("ERROR in updateNotifyUser ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	
	private function deleteInsertRecords(){
		
		$tblDet = $this->getTableDetails();
		//if load by empty or null not need to continue
		if(empty($tblDet['loadby'])) return;
		
		if($form_state['values']['create_user_id'])
		    $loggedInUserId = $form_state['values']['create_user_id'];
		else
		    $loggedInUserId  = getIdOfLoggedInUser();
		
		// Audit trail - Delele log entry 
		$fields = array(
				'table_name' => 'slt_audit_trail',
				'dataload_by' => $tblDet['loadby'],
				'entity_type' => 'audit_trail',
				'module_name' => 'exp_sp_learning_auto_register',
				'functionality_name' => 'deleteInsertRecords',
				'api_name' => null,
				'deleted_on' => now(),
				'deleted_by' => $loggedInUserId,
				'parent1_entity_type' => 'logged_user_id',
				'parent2_entity_type' => 'entity_id',
				'parent3_entity_type' => 'mod_user_id'
		);
		deleted_log_entry_bulk($fields);
		
		
		$query = db_delete('slt_audit_trail');
		$query->condition('dataload_by',$tblDet['loadby'],'=');
		expDebug::dPrintDBAPI('Delete audit entry -->>',$query);
		$result = $query->execute();
			
		// User points - Delele log entry
		$fields = array(
				'table_name' => 'slt_user_points',
				'dataload_by' => $tblDet['loadby'],
				'entity_type' => 'user_points',
				'module_name' => 'exp_sp_learning_auto_register',
				'functionality_name' => 'deleteInsertRecords',
				'api_name' => $_REQUEST['apiname'],
				'deleted_on' => now(),
				'deleted_by' => $loggedInUserId,
				'parent1_entity_type' => 'user_id',
				'parent2_entity_type' => 'entity_id',
		);
		deleted_log_entry_bulk($fields);
		
		$query = db_delete('slt_user_points');
		$query->condition('dataload_by',$tblDet['loadby'],'=');
		expDebug::dPrintDBAPI('Delete user points -->>',$query);
		$query->execute();
			
			
		$query = db_delete('slt_bulk_notification');
		$query->condition('dataload_by',$tblDet['loadby'],'=');
		expDebug::dPrintDBAPI('Delete notification -->>',$query);
		$query->execute();
			
		// order - Delele log entry		
		$fields= array(
				'table_name' => 'slt_order',
				'dataload_by' => $tblDet['loadby'],
				'entity_type' => 'order',
				'parent1_entity_type' => 'user_id',
				'parent2_entity_type' => 'uc_order_id',
				'module_name' => 'exp_sp_learning_auto_register',
				'functionality_name' => 'deleteInsertRecords',
				'api_name' => $_REQUEST['apiname'],
				'deleted_on' => now(),
				'deleted_by' => $loggedInUserId,
				 
		);
		deleted_log_entry_bulk($fields);
		
		$tblName = 'temp_autoreg_b'.$this->btId;
		if($this->objType != 'Class'){
			$tblName = 'temp_autoreg_class_b'.$this->btId;
		}
		// order items - Delele log entry		
		$fields= array(
				'table_name' => 'slt_order_items',
				'dataload_by' => $tblDet['loadby'],
				'entity_type' => 'order_items',
				'parent1_entity_type' => 'order_id',
				'parent2_entity_type' => 'user_id',
				'parent3_entity_type' => 'class_id',
				'parent4_entity_type' => 'program_id',
				'module_name' => 'exp_sp_learning_auto_register',
				'functionality_name' => 'deleteInsertRecords',
				'api_name' => $_REQUEST['apiname'],
				'deleted_on' => now(),
				'deleted_by' => $loggedInUserId,
					
		);
		deleted_log_entry_bulk($fields);
		
		
		for($l=1;$l<=$this->recordGrp;$l++){
			$query = "DELETE map FROM slt_order_items map
						INNER JOIN ".$tblName." temp ON temp.dataload_by = map.dataload_by
						WHERE temp.record_grp = :rgrp  AND temp.dataload_by = :dlby ";
			expDebug::dPrintDBAPI('Delete order items -->>',$query,array(':dlby'=> $tblDet['loadby'],':rgrp'=>$l));
			db_query($query,array(':dlby'=> $tblDet['loadby'],':rgrp'=>$l));
		}
			
		$select=db_select('slt_order','ord');
		$select->addfield('ord','id','id');
		$select->addField('ord','uc_order_id','p2');
		$select->addField('ord','user_id','p1');
		$select->condition('dataload_by',$tblDet['loadby'],'=');
		$select1=$select->execute()->fetchAll();
					
		$query = db_delete('slt_order');
		$query->condition('dataload_by',$tblDet['loadby'],'=');
		expDebug::dPrintDBAPI('Delete order -->>',$query);
		$query->execute();

		    

		
// 		// Enrollment content mapping - Delele log entry
// 		$fields= array(
// 				'table_name' => 'slt_enrollment_content_mapping',
// 				'dataload_by' => $tblDet['loadby'],
// 				'entity_type' => 'enrollment_content_mapping',
// 				'parent1_entity_type' => 'enroll_id',
// 				'parent2_entity_type' => 'user_id',
// 				'parent3_entity_type' => 'class_id',
// 				'parent4_entity_type' => 'content_id',
// 				'parent5_entity_type' => 'version_id',
// 				'module_name' => 'exp_sp_learning_auto_register',
// 				'functionality_name' => 'deleteInsertRecords',
// 				'api_name' => $_REQUEST['apiname'],
// 				'deleted_on' => now(),
// 				'deleted_by' => $loggedInUserId,
					
// 		);
// 		deleted_log_entry_bulk($fields);
		
		for($l=1;$l<=$this->recordGrp;$l++){
			$updQry = "DELETE map FROM slt_enrollment_content_mapping map
						INNER JOIN slt_enrollment enr ON enr.id = map.enroll_id
						INNER JOIN ".$tblName." temp ON temp.dataload_by = enr.dataload_by
						WHERE temp.record_grp = :rgrp  AND enr.dataload_by = :dlby ";
			expDebug::dPrintDBAPI('Delete enrollment Content mapping -->>',$updQry,array(':dlby'=> $tblDet['loadby'],':rgrp'=>$l));
			db_query($updQry,array(':dlby'=> $tblDet['loadby'],':rgrp'=>$l));
		}
			
		// Enrollment - Delele log entry
		$fields= array(
				'table_name' => 'slt_enrollment',
				'dataload_by' => $tblDet['loadby'],
				'entity_type' => 'enrollment',
				'parent1_entity_type' => 'master_enrollment_id',
				'parent2_entity_type' => 'user_id',
				'parent2_entity_type' => 'user_id',
				'parent3_entity_type' => 'class_id',
				'parent4_entity_type' => 'order_id',
				'module_name' => 'exp_sp_learning_auto_register',
				'functionality_name' => 'deleteInsertRecords',
				'api_name' => $_REQUEST['apiname'],
				'deleted_on' => now(),
				'deleted_by' => $loggedInUserId,
					
		);
		deleted_log_entry_bulk($fields);
		
		for($l=1;$l<=$this->recordGrp;$l++){
			$query = "DELETE map FROM slt_enrollment map
						INNER JOIN ".$tblName." temp ON temp.dataload_by = map.dataload_by
						WHERE temp.record_grp = :rgrp  AND temp.dataload_by = :dlby ";
			expDebug::dPrintDBAPI('Delete enrollment -->>',$query,array(':dlby'=> $tblDet['loadby'],':rgrp'=>$l));
			db_query($query,array(':dlby'=> $tblDet['loadby'],':rgrp'=>$l));
		}
		if($this->objType != 'Class'){
			// Master Enrollment - Delele log entry
			$fields= array(
					'table_name' => 'slt_master_enrollment',
					'dataload_by' => $tblDet['loadby'],
					'entity_type' => 'master_enrollment',
					'parent1_entity_type' => 'order_id',
					'parent2_entity_type' => 'program_id',
					'parent3_entity_type' => 'user_id',
					'parent4_entity_type' => 'recertify_path',
					'module_name' => 'exp_sp_learning_auto_register',
					'functionality_name' => 'deleteInsertRecords',
					'api_name' => $_REQUEST['apiname'],
					'deleted_on' => now(),
					'deleted_by' => $loggedInUserId,
			
			);
			deleted_log_entry_bulk($fields);
			
			$query = db_delete('slt_master_enrollment');
			$query->condition('dataload_by',$tblDet['loadby'],'=');
			expDebug::dPrintDBAPI('Delete master enrollment -->>',$query);
			$query->execute();
			
			
			
		}
	}
	
	
	private function insertEnrollmentContentMapping($tblName){
		try{
			
			$tblDet = $this->getTableDetails();
			for($l=1;$l<=$this->recordGrp;$l++){
			
				// For inserting enrollment entries from temp table to slt_enrollment table
				$recFields = array('created_on','created_by','updated_on','updated_by');
				
				//$recquery = db_select('slt_enrollment', 'enr');
				$recquery = db_select($tblName, 'enr');
				//$recquery->innerJoin($tblName,'temp',"temp.master_enrollment_id = enr.master_enrollment_id and temp.class_id = enr.class_id and temp.course_id = enr.course_id and temp.user_id = enr.user_id");
				$recquery->innerJoin('slt_course_content_mapper','cmap','cmap.class_id = enr.class_id AND cmap.course_id = enr.course_id');
				$recquery->innerJoin('slt_content_version','cver','cver.content_master_id = cmap.content_id AND cver.status = \'lrn_cnt_sts_atv\'');
				//$recquery->innerJoin('slt_content_version','cver','cver.content_master_id = cmap.content_id');
				$recquery->fields('enr',$recFields);
				$recquery->addExpression('enr.primary_id','enroll_id');
				$recquery->addExpression('enr.user_id','user_id');
				$recquery->addExpression('enr.course_id','course_id');
				$recquery->addExpression('enr.class_id','class_id');
				$recquery->addExpression('cmap.content_id','content_id');
				$recquery->addExpression('cver.id','version_id');
				$recquery->condition('enr.record_grp', $l,'=');
				//$recquery->condition('enr.custom_dataload', $this->btId,'=');
				$recquery->condition('enr.record_status', 'R','=');
				$recquery->condition('enr.dataload_by', $tblDet['loadby'],'=');
				expDebug::dPrintDBAPI('Insert the enrollment content mapping records from temp table', $recquery);
				
				db_insert('slt_enrollment_content_mapping')
				->from($recquery)
				->execute();
				
				//Delay to avoid dead lock
				usleep($this->delay);
			}
			
		} catch(Exception $e){
			expDebug::dPrint("ERROR in insertEnrollmentContentMapping ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
		
	}
	
	private function updateEnrolledClassId($tblName){
		try{
			for($l=1;$l<=$this->recordGrp;$l++){
				$updQry = "UPDATE temp_autoreg_class_b".$this->btId." temp
				INNER JOIN slt_enrollment enr ON enr.course_id = temp.course_id AND enr.user_id = temp.user_id 
				INNER JOIN slt_course_class cls ON cls.id = enr.class_id
				SET temp.class_id = enr.class_id, temp.custom3 = cls.delivery_type,
				temp.comp_status = CASE WHEN enr.comp_status = :cmpsts THEN :cmpsts ELSE temp.comp_status END,
				temp.progress = CASE WHEN  enr.comp_status = :cmpsts THEN enr.progress ELSE temp.progress END,
				temp.comp_date = CASE WHEN enr.comp_status = :cmpsts THEN enr.comp_date ELSE temp.comp_date END,
				temp.comp_by = CASE WHEN enr.comp_status = :cmpsts THEN enr.comp_by ELSE temp.comp_by END,
				temp.comp_on = CASE WHEN enr.comp_status = :cmpsts THEN enr.comp_on ELSE temp.comp_on END,
				temp.operation = CASE WHEN cls.delivery_type IN (:ilt , :vcl ) AND (enr.reg_status = :wtlsts OR enr.comp_status IN (:cmpsts, :enrsts , :atdsts )) THEN 'update' ELSE temp.operation END
				WHERE temp.record_status = :r AND enr.comp_status = :cmpsts and enr.recertify_path = :recertify_path AND temp.record_grp = ".$l;
					
				$args = array(':r' => 'R',':cmpsts' => 'lrn_crs_cmp_cmp', ':enrsts' => 'lrn_crs_cmp_enr', ':wtlsts' => 'lrn_crs_reg_wtl',
						':atdsts' => 'lrn_crs_cmp_att', ':ilt' => 'lrn_cls_dty_ilt', ':vcl' => 'lrn_cls_dty_vcl' ,':recertify_path' => $this->recertify_path);
					
				expDebug::dPrintDBAPI("Update Query for Enrolled Class id",$updQry,$args);
				db_query($updQry,$args);
				
				//Delay to avoid dead lock
				usleep($this->delay);
			}
			
		} catch(Exception $e){
			expDebug::dPrint("ERROR in updateEnrolledClassId ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function updateTempClassId($tblName){
		try{
			
			for($l=1;$l<=$this->recordGrp;$l++){
				$updQry = "UPDATE temp_autoreg_class_b".$this->btId." temp
									RIGHT JOIN (SELECT cls.id as clsid, cls.status, cls.delivery_type, t1.user_id, t1.id FROM temp_autoreg_class_b".$this->btId." t1";
					if(!empty($this->tpClassIds))
					{
						$updQry .= " INNER JOIN slt_course_class cls ON cls.course_id = t1.course_id and cls.id IN ($this->tpClassIds)";
					}
						
				else {
					$updQry .= " INNER JOIN slt_course_class cls ON cls.course_id = t1.course_id AND t1.class_id IS NULL";
					}
				$updQry .= " WHERE t1.record_status = :r AND t1.class_id IS NULL AND t1.record_grp = ".$l." AND cls.status = :atv 
									) x ON temp.id = x.id
									SET temp.class_id = x.clsid,
									temp.custom3 = x.delivery_type
									WHERE temp.user_id = x.user_id AND temp.record_status = :r AND temp.record_grp = ".$l;
				
				$args = array(':r' => 'R', ':atv' => 'lrn_cls_sts_atv');
				
				expDebug::dPrintDBAPI("Update Query for Class id",$updQry,$args);
				
				db_query($updQry,$args);
				
				//Delay to avoid dead lock
				usleep($this->delay);
				
				/*$updQry1 = "UPDATE temp_autoreg_class_b".$this->btId." t1 
				INNER JOIN temp_autoreg_class_b".$this->btId." t2 ON t2.mapping_id = t1.mapping_id
				SET t1.record_status = :inv
				WHERE t2.mapping_id = t1.mapping_id and t2.record_status = :inv ";
				
				$args1 = array(':inv' => 'IN');
				
				expDebug::dPrintDBAPI("Update Query for Class id",$updQry1,$args1);
				
				db_query($updQry1,$args1);*/
			}
			
		} catch(Exception $e){
			expDebug::dPrint("ERROR in updateEnrolledClassId ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/* private function updateEnrollmentCompletionStatus(){
		try{
			$updQry = "UPDATE temp_autoreg_class_b".$this->btId." temp 
								 INNER JOIN slt_enrollment enr ON enr.course_id = temp.course_id AND enr.class_id = temp.class_id AND enr.user_id = temp.user_id
								 SET temp.comp_status = :cmpsts , temp.comp_date = enr.comp_date
								 WHERE enr.comp_status = :cmpsts ";
			
			$args = array(':r' => 'R', ':cmpsts' => 'lrn_crs_cmp_cmp');
			
			expDebug::dPrintDBAPI("Update Query for Enrolled Class id",$updQry,$args);
			db_query($updQry,$args);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in updateEnrollmentCompletionStatus ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	} */
	
	private function markPercentageCompleted(){
		try{
				
			 	$updQry = "UPDATE
									slt_master_enrollment menr,
									(SELECT
										(CASE WHEN (total_courses_count = 0) THEN
												NULL
												ELSE
												ROUND(CASE WHEN (mandatory_courses_count = 0) THEN
														optional_completed_courses_count / total_courses_count
														ELSE
														mandatory_completed_courses_count / mandatory_courses_count
														END * 100, 0)
												END) AS percentage_completed, menrid
										FROM
										(SELECT enr.master_enrollment_id as menrid,
												COUNT(CASE WHEN enr.comp_status = :cmpsts AND crsmap.is_required = :req THEN enr.id END) mandatory_completed_courses_count,
												COUNT(CASE WHEN enr.comp_status = :cmpsts AND crsmap.is_required != :req THEN enr.id END) optional_completed_courses_count,
												COUNT(CASE WHEN crsmap.is_required = :req THEN enr.id END) mandatory_courses_count,
												COUNT(enr.id) total_courses_count
												FROM
												slt_enrollment enr
												INNER JOIN temp_autoreg_class_b".$this->btId." temp on temp.master_enrollment_id = enr.master_enrollment_id
												INNER JOIN slt_module_crs_mapping crsmap ON (enr.course_id = crsmap.course_id AND crsmap.program_id = :prmid )
												WHERE
												enr.master_enrollment_id = temp.master_enrollment_id
												AND enr.reg_status NOT IN(:rsv , :rsc )
												GROUP BY
												enr.master_enrollment_id) grouped_data
								)x
								SET percentage_complete = x.percentage_completed
								WHERE
								menr.id = x.menrid";
			
			$args = array(':req' => 'Y', ':cmpsts' => 'lrn_crs_cmp_cmp', ':prmid' => $this->classId,':rsv' => 'lrn_crs_reg_rsv',':rsc' => 'lrn_crs_reg_rsc');
			$this->UpdateProgressforTP();
			expDebug::dPrintDBAPI("Update Query for mark percentage Complete",$updQry,$args);
			db_query($updQry,$args);
				
		} catch (Exception $e){
			expDebug::dPrint("ERROR in markPercentageCompleted ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function updateCompletionNotification() {
		try{
			$tblDet = $this->getTableDetails();
			$updQry = "UPDATE temp_autoreg_b".$this->btId." temp 
								INNER JOIN slt_master_enrollment mse on mse.order_id = temp.order_id and mse.overall_status IN (:cmpsts , :wtlsts ) 
								SET notifyuser = CASE WHEN mse.overall_status = :cmpsts THEN :compstr ELSE :wtlstr END
								WHERE temp.notifyuser = :regstr AND temp.record_status = :r AND mse.dataload_by = :dlby 
								AND mse.overall_status IN (:cmpsts , :wtlsts ) ";
			
			$args = array(':r' => 'R', ':cmpsts' => 'lrn_tpm_ovr_cmp', ':dlby' => $tblDet['loadby'], ':wtlsts' => 'lrn_tpm_ovr_wtl',
					':compstr' => 'certCurrLPRegisterAndCompleted',':wtlstr' => 'certCurrLPWaitlistRegister',':regstr' => 'certCurrLPRegister');
			
			expDebug::dPrintDBAPI("Update Query for Completion notification",$updQry,$args);
			db_query($updQry,$args);
			
		} catch(Exception $e){
			expDebug::dPrint("ERROR in updateCompletionNotification ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function updateWaitlistStatus(){
		try{
			
			$tblDet = $this->getTableDetails();
			$updQry = "UPDATE slt_master_enrollment menr
			INNER JOIN (select mse.id from slt_master_enrollment mse
			INNER JOIN temp_autoreg_class_b".$this->btId." temp ON temp.master_enrollment_id =  mse.id AND mse.overall_status = :enrsts 
			WHERE temp.reg_status = :wtlsts and temp.record_status = :r AND mse.dataload_by = :dlby ) x on x.id = menr.id
			SET menr.overall_status = :wtltpsts 
			WHERE menr.dataload_by = :dlby AND menr.overall_status = :enrsts ";
				
			$args = array(':r' => 'R', ':enrsts' => 'lrn_tpm_ovr_enr',':wtlsts' => 'lrn_crs_reg_wtl', ':dlby' => $tblDet['loadby'],
					':wtltpsts' => 'lrn_tpm_ovr_wtl');
				
			expDebug::dPrintDBAPI("Update Query for waitlist status in master enrollment",$updQry,$args);
			db_query($updQry,$args);
			
		}catch(Exception $e){
			expDebug::dPrint("ERROR in updateWaitlistStatus ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	//Mandatory class and program auto_registration start
	private function mandatoryComplianceAutoRegister(){
		try{
			module_load_include('inc', 'exp_sp_lnrsearch', 'exp_sp_lnrsearch');
			global $catalog_reg;
			$catalog_reg = 'Class';
			$lnrSrch = new LnrSearchCatalog();
			$lnrSrch->catalogSearchQueryClassCron('','auto_register');
			if(module_exists('exp_sp_commerce')){
				$lnrSrch->searchPriceClass('$0-$0');
	
			}
			$lnrSrch->setTrainingType('cre_sys_inv_com|cre_sys_inv_man',true);
			$classQuery=$lnrSrch->select;
			$lnrSrch->catalogSearchQueryTPCron();
			if(module_exists('exp_sp_commerce')){
				$lnrSrch->searchPriceTP('$0-$0');
			}
			//$lnrSrch->searchDate('', '');
			$lnrSrch->setTrainingTypeTP('cre_sys_inv_man',true);
			//$TPQuery=$lnrSrch->select;
	
			$classQuery->union($lnrSrch->select,'UNION ALL');
			$resultSet = $classQuery->execute()->fetchAll();
			expdebug::dPrintDBAPI("Class Fetch Query - ",$classQuery);
			expDebug::dPrint("Class Query Result".print_r($resultSet,true),3);
			$userGetArr = array();
			
			// Get the Maximum Batch id available from slt_autoregister_temp table
			/*$tmpSelect = db_select('slt_autoregister_temp','temp');
			$tmpSelect->addExpression('MAX(batch_id)','bid');
			$btcCnt = $tmpSelect->execute()->fetchField();*/
			$tmpSelect = db_select('slt_enrollment','temp');
			$tmpSelect->addField('temp','dataload_by','bid');
			$tmpSelect->isNotNull('temp.dataload_by');
			$tmpSelect->where('SUBSTRING(temp.dataload_by, 1, 1) = \'A\'');
			$tmpSelect->orderBy('temp.id','desc');
			$tmpSelect->range(0,1);
			expdebug::dPrintDBAPI("GET BATCH ID - ",$tmpSelect);
			$btcCnt = $tmpSelect->execute()->fetchField();
			$btcCnt = explode('-',$btcCnt);
			$btId = $btcCnt[1] + 1;
			expDebug::dPrint("BAtchId".print_r($btcCnt,true),3);
			expDebug::dPrint("BAtchId".print_r($btId,true),3);
			
			
			//$btId = $btcCnt + 1;
			$autoreglmt = getConfigValue('autoregister_limit');
			foreach ($resultSet as $result) {
				expDebug::dPrint("Class Query Result".print_r($result,true),4);
				$classId = $result->cls_id;
				$courseid = $result->crs_id;
				$groupid = $result->groupid;
				$result->cls_code = addslashes($result->cls_code);
				$result->cls_title = addslashes($result->cls_title);
				$result->cls_short_description = addslashes($result->cls_short_description);
				$delivery_type_name = $result->delivery_type_name;
				expDebug::dPrint("Group id - ".$groupid,4);
				
				$obTypeCode = $result->object_type == 'Class' ? 'cre_sys_obt_cls' : $result->object_type; 
				$userAccessQry = db_select('slt_user_access_mapping','umap');
				$userAccessQry->addExpression('count(1)','cnt');
				$userAccessQry->condition('umap.access_type',0,'=');
				$userAccessQry->condition('umap.entity_id',$classId,'=');
				$userAccessQry->condition('umap.entity_type',$obTypeCode,'=');
				$userAccess = $userAccessQry->execute()->fetchField();
				expDebug::dPrint("Associated users to an entity -- ".print_r($userAccess,true),4);
				
				//Mark empty columns as null - fix for #0060011
				foreach($result as $key=>&$res){
					if($key != 'object_type')
						$res = empty($res) ? "NULL" : "'".$res."'";
				}
				
				$select = db_select('slt_person', 'person');
				$select->distinct();
				$select->addField('person','id','userids');
				
				$select->addExpression('NULL','batch_id');
				$select->addExpression("'PR'",'record_status');
				$select->addExpression('NULL','prcsts');
				$select->addExpression('NULL','notifysts');
				$select->addExpression('NULL','notifyuser');
				$select->addExpression('NULL','remarks');
				$select->addExpression('NULL','custom0');
				$select->addExpression('NULL','custom1');
				$select->addExpression('NULL','custom2');
				$select->addExpression('NULL','custom3');
				$select->addExpression('NULL','custom4');
				
				$select->addExpression($result->cls_id,'cls_id');
				$select->addExpression($result->cls_code,'cls_code');
				$select->addExpression($result->cls_title,'cls_title');
				$select->addExpression($result->cls_short_description,'cls_short_description');
				$select->addExpression($result->price,'price');
				$select->addExpression($result->currency_type,'currency_type');
				$select->addExpression($result->registration_end_on,'registration_end_on');
				$select->addExpression($result->created_on,'created_on');
				$select->addExpression($result->published_on,'published_on');
				$select->addExpression($result->mro_id,'mro_id');
				$select->addExpression($result->language_code,'language_code');
				$select->addExpression($result->language,'language');
				$select->addExpression($result->start_date,'start_date');
				$select->addExpression($result->location_id,'location_id');
				$select->addExpression($result->crs_id,'crs_id');
				$select->addExpression($result->delivery_type_code,'delivery_type_code');
				$select->addExpression($result->delivery_type_name,'delivery_type_name');
				$select->addExpression($result->expires_in_value,'expires_in_value');
				$select->addExpression($result->expires_in_unit,'expires_in_unit');
				$select->addExpression($result->prm_created_on,'prm_created_on');
				$select->addExpression($result->export_compliance,'export_compliance');
				$select->addExpression("'".$result->object_type."'",'object_type');
				$select->addExpression($result->is_compliance,'is_compliance');
				$select->addExpression($result->avgvote,'avgvote');
				$select->addExpression($result->ascen,'ascen');
				
				if(!empty($groupid)){
					$classQuery = '';
					$enabledValues = getEnabledAttrForVisibility();
					$closeBracket='';
					$lastid = 1;
					$enabledCount = count($enabledValues);
					if($enabledCount > 0){
						$classQuery = 'AND (IF(usrmap.user_id IS NOT NULL, usrmap.mro = \'cre_sys_inv_man\', (if(FIND_IN_SET(person.id,catacs.added_users)>0,1=1, ';
						foreach($enabledValues as $key => $value){
							if($value == 'state')
								$userValue = 'IF(person.state IS NOT NULL, CONCAT(person.country,\'-\',person.state),\'\' )';
							else
								$userValue = ($value == 'department') ? 'person.dept_code' : (($value == 'language') ? 'person.preferred_language' : 'person.'.$value);
			
							if($value != 'role' && $value != 'hire_date'){
			
								if($value == 'job_role'){
									$classQuery .= ' (if(catacs.'.$value.' = \'All\',(lpjm.job_role IS NOT NULL), (catacs.'.$value.' is null or catacs.'.$value.'=\'\' or (FIND_IN_SET(lpjm.job_role,catacs.'.$value.'))>0)))';
									
									if($lastid != $enabledCount)
										$classQuery .= ' AND ';
								}else{
			
									$classQuery .= ' (if(catacs.'.$value.' = \'All\',catacs.'.$value.'!=\'\',(catacs.'.$value.' is null or catacs.'.$value.'=\'\' or (FIND_IN_SET('.$userValue.',catacs.'.$value.'))))) ';
									
									if($lastid != $enabledCount)
									$classQuery .= ' AND ';
								}
							}
			
							if($value == 'role'){
								$classQuery .= '(CASE WHEN (catacs.is_manager=\'Y\' AND catacs.is_instructor=\'Y\') THEN (person.is_manager = catacs.is_manager OR person.is_instructor = catacs.is_instructor) 
								WHEN (catacs.is_manager=\'Y\' AND catacs.is_instructor=\'N\') THEN (person.is_manager = catacs.is_manager) 
							  WHEN (catacs.is_manager=\'N\' AND catacs.is_instructor=\'Y\') THEN (person.is_instructor = catacs.is_instructor) ELSE 1=1 END) ';
								if($lastid != $enabledCount)
									$classQuery .= ' AND ';
							}
							if($value == 'hire_date'){
								$classQuery .= '(IF((select att.id from slt_group_attributes att where att.group_id = catacs.id)>0,
					  (select att.id from slt_group_attributes att where att.group_id = catacs.id and (
					  (DATE_FORMAT(person.hire_date,\'%Y-%m-%d\') >= DATE_FORMAT(att.on_or_after_start_date,\'%Y-%m-%d\') ) or
					  (DATE_FORMAT(person.hire_date,\'%Y-%m-%d\') <= DATE_FORMAT(att.on_or_before_start_date,\'%Y-%m-%d\')) or
					  (DATE_FORMAT(person.hire_date,\'%Y-%m-%d\') between DATE_FORMAT(att.between_start_date,\'%Y-%m-%d\') AND DATE_FORMAT(att.between_end_date,\'%Y-%m-%d\'))
					  )) >0
					  ,1=1)) ';
								if ($lastid != $enabledCount)
									$classQuery .= ' AND ';
							}
							
							
							$lastid++;
						}
						$classQuery .= ' AND (catacs.removed_users IS NULL OR FIND_IN_SET(person.id,catacs.removed_users) <= 0)';
						$classQuery .= ')) ))';
					}
					
					$select->leftJoin('slt_user_access_mapping','usrmap','usrmap.entity_id = '.$classId.' AND usrmap.entity_type = \''.$obTypeCode.'\' AND usrmap.access_type = 0 AND usrmap.user_id = person.id');
					$select->leftjoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id = person.id');
					$select->leftjoin('slt_groups', 'catacs', 'catacs.id='.$groupid. ' ' . $classQuery);
				}else if($userAccess>0){
					$select->leftJoin('slt_user_access_mapping','usrmap','usrmap.entity_id = '.$classId.' AND usrmap.entity_type = \''.$obTypeCode.'\' AND usrmap.access_type = 0 ');
					$select->where('person.id = usrmap.user_id');
				}
				if($delivery_type_name == 'Curricula' || $delivery_type_name == 'Certification' || $delivery_type_name == 'Learning Plan'){
					$select->leftjoin('slt_master_enrollment', 'master_enroll', "master_enroll.program_id = $classId AND master_enroll.user_id = person.id AND master_enroll.overall_status IN  ('lrn_tpm_ovr_enr', 'lrn_tpm_ovr_inp', 'lrn_tpm_ovr_ppv', 'lrn_tpm_ovr_ppm', 'lrn_tpm_ovr_wtl')");
					$select->addExpression('1','recertify_path');
				}else{
					$select->leftjoin('slt_enrollment', 'enroll', "(enroll.class_id = $classId or enroll.course_id = $courseid) AND enroll.user_id = person.id AND enroll.reg_status IN  ('lrn_crs_reg_cnf', 'lrn_crs_reg_ppm', 'lrn_crs_reg_wtl')");
					$select->leftjoin('slt_course_template', 'crs', "crs.id = $courseid AND crs.compliance_completed = 'hire_days'");
				}
				$select->leftjoin('slt_autoregister_temp', 'temp', "temp.cls_id = $classId AND temp.userids = person.id and temp.object_type = '".$result->object_type."'");
				if($delivery_type_name == 'Curricula' || $delivery_type_name == 'Certification' || $delivery_type_name == 'Learning Plan'){
					$select->condition('master_enroll.id','','is null');
				}else{
					$select->condition('enroll.id','','is null');
				//	$select->where('(IF(crs.id IS NOT NULL AND person.hire_date IS NOT NULL,(DATE_ADD(person.hire_date, INTERVAL crs.complete_days DAY) >= NOW()),1=1))');
				    $select->where('(IF( crs.id IS NOT NULL AND crs.compliance_completed = \'hire_days\' AND person.hire_date IS NOT NULL,(DATE_ADD(DATE_FORMAT(person.hire_date, "%Y-%m-%d"), INTERVAL crs.complete_days DAY) >= CURDATE()),IF(crs.complete_date IS NOT NULL AND crs.complete_date != "",crs.complete_date >= CURDATE(),1=1)))');
				}
				$select->condition('person.id',array(1,2),'NOT IN');
				$select->condition('person.status','cre_usr_sts_atv','=');
				$select->condition('temp.id','','is null');
				if(!empty($groupid)){
				  $select->condition('catacs.id','','IS NOT NULL');
				}
				//$userdetails = $select->execute()->fetchAll();
				expDebug::dPrintDBAPI("Query result for users ----->",$select);
				db_insert('slt_autoregister_temp')
	                       ->from($select)
	                       ->execute();
				
	      db_query('COMMIT');
				$regCnt=0;				
				
				$sel = db_select('slt_autoregister_temp', 'autotmp');
				/*$sel->fields('autotmp','cls_id');
				$sel->fields('autotmp','object_type');*/
				$sel->addExpression('count(1)','cnt');
				$sel->condition('autotmp.cls_id',$classId);
				$sel->condition('autotmp.object_type',$result->object_type);
				$sel->where('autotmp.batch_id is null');
				expDebug::dPrintDBAPI("Auto Reg count qry- ",$sel);
				$cnt = $sel->execute()->fetchField();
				expDebug::dPrint("Auto Reg count - ".$cnt);
				
				$pcnt = round($cnt/$autoreglmt) + 1;
				for($o=0;$o<$pcnt;$o++){
					// Cross check the batch id and avoid skip batchid if no update happen
					$tmpSelect = db_select('slt_autoregister_temp','temp');
					$tmpSelect->addExpression('MAX(batch_id)','bid');
					$btcCnt = $tmpSelect->execute()->fetchField();
					if($btcCnt == $btId){
						$btId++;
					}elseif($btcCnt > $btId){
						$btId = $btcCnt;
					}
					
					$upd = "UPDATE slt_autoregister_temp SET batch_id = :batch_id 
							WHERE cls_id = :cls_id  AND object_type = :object_type  AND batch_id IS NULL LIMIT $autoreglmt ";
					$arg = array(
						':batch_id'=>$btId,
						':cls_id'=>$classId,
						':object_type'=>$result->object_type,
					);
					expDebug::dPrintDBAPI("Auto Reg batch id update qry- ",$upd,$arg);
					db_query($upd,$arg);
					db_query('COMMIT');
				}
			}
		}catch (Exception $ex) {
			$pList = db_query('SHOW FULL PROCESSLIST')->fetchAll();
			expDebug::dPrint("Process list while error in mandatoryComplianceAutoRegister ".print_r($pList,true),1);
			throw new Exception($ex->getMessage());
		}
	}

	
	//Mandatory class and program auto_registration start
	private function mandatoryCertificationAutoRegister(){
		try{
			expDebug::dPrint("testing inside loop");
			// Get the Maximum Batch id available from slt_autoregister_temp table
			$tmpSelect = db_select('slt_autoregister_temp','temp');
			$tmpSelect->addExpression('MAX(batch_id)','bid');
			$btcCnt = $tmpSelect->execute()->fetchField();
	
			$btId = $btcCnt + 1;
			$autoreglmt = getConfigValue('autoregister_limit');
				
			$select = db_select('slt_master_enrollment','enr');
			$select->innerJoin('slt_person','per','per.id = enr.user_id');
			$select->innerJoin('slt_program','prg','prg.id = enr.program_id');
			$select->innerJoin('slt_profile_list_items','prf1','prf1.code = prg.lang_code');
			$select->innerJoin('slt_profile_list_items','prf2','prf2.code = prg.object_type');
			// Fields to be inserted in auto register temp table
			$select->addField('enr','user_id','userids');
				
				
			$select->addExpression('NULL','batch_id');
			$select->addExpression("'PR'",'record_status');
			$select->addExpression('NULL','prcsts');
			$select->addExpression('NULL','notifysts');
			$select->addExpression('NULL','notifyuser');
			$select->addExpression('NULL','remarks');
			$select->addExpression('NULL','custom0');
			$select->addExpression('NULL','custom1');
			$select->addExpression('NULL','custom2');
			$select->addExpression('NULL','custom3');
			$select->addExpression('(enr.recertify_path)+1','custom4');
	
			$select->addExpression('prg.id','cls_id');
			$select->addExpression('prg.code','cls_code');
			$select->addExpression('prg.title','cls_title');
			$select->addExpression('prg.short_desc','cls_short_description');
			$select->addExpression('prg.price','price');
			$select->addExpression('prg.currency_type','currency_type');
			$select->addExpression('prg.registration_close_date','registration_end_on');
			$select->addExpression('prg.created_on','created_on');
			$select->addExpression('prg.published_on','published_on');
			$select->addExpression('\'cre_sys_inv_man\'','mro_id');
			$select->addExpression('prg.lang_code','language_code');
			$select->addExpression('prf1.name','language');
			$select->addExpression('NULL','start_date');
			$select->addExpression('NULL','location_id');
			$select->addExpression('NULL','crs_id');
			$select->addExpression('NULL','delivery_type_code');
			$select->addExpression('prf2.name','delivery_type_name');
			$select->addExpression('prg.expires_in_value','expires_in_value');
			$select->addExpression('prg.expires_in_unit','expires_in_unit');
			$select->addExpression('prg.created_on','prm_created_on');
			$select->addExpression('NULL','export_compliance');
			$select->addExpression('prg.object_type','object_type');
			$select->addExpression('NULL','is_compliance');
			$select->addExpression('NULL','avgvote');
			$select->addExpression('NULL','ascen');
			//$select->addExpression('max(master_enroll.recertify_path)+1','recertify_path');	
				
				
			/*$qry->condition(db_and()->condition('enr.mandatory', 1, '=')
					->condition(db_or()->isNull('crs.complete_date')
							->condition('crs.complete_date', NOW(), '>')
					)
			);*/
			$select->condition('enr.mandatory', 1, '=');
			$select->condition('prg.status', 'lrn_lpn_sts_atv','=');
		//	$qry->condition('cls.status', 'lrn_cls_sts_atv','=');	// this was uncommented for #64835
			$price_condition_or = db_or();
			$price_condition_or->condition('prg.price', 0);
			$price_condition_or->isNull('prg.price');
			$select->condition($price_condition_or);
			$select->condition('enr.overall_status', 'lrn_tpm_ovr_exp','=');
			$select->condition('per.status', 'cre_usr_sts_atv','=');
			//$qry->groupBy('enr.course_id');
			$select->groupBy('enr.id');
				
			expDebug::dPrintDBAPI("Query result for auto TP re registration",$select);
			db_insert('slt_autoregister_temp')
			->from($select)
			->execute();
			db_query('COMMIT');
				
			$res = db_select('slt_autoregister_temp', 'tmp');
			$res->addField('tmp', 'cls_id');
			$res->addField('tmp', 'crs_id');
			$res->condition('tmp.custom4',1,'>');
			$res->condition('tmp.batch_id','','IS NULL');
			$res->groupBy('tmp.cls_id');
			$res->groupBy('tmp.crs_id');
			$res->groupBy('tmp.object_type');
			expDebug::dPrintDBAPI("Select query for temp table",$res);
			$clsRes = $res->execute()->fetchAll();
				
			foreach($clsRes as $key){
				$sel = db_select('slt_autoregister_temp', 'autotmp');
				$sel->addExpression('count(1)','cnt');
				$sel->condition('autotmp.cls_id',$key->cls_id,'=');
				$sel->condition('autotmp.custom4',1,'>');
				$sel->condition('autotmp.object_type','Class','!=');
				$sel->where('autotmp.batch_id is null');
				expDebug::dPrintDBAPI("Auto TP Reg count qry- ",$sel);
				$cnt = $sel->execute()->fetchField();
				expDebug::dPrint("Auto TP Reg count - ".$cnt,3);
					
				$pcnt = round($cnt/$autoreglmt) + 1;
				for($o=0;$o<$pcnt;$o++){
					$upd = "UPDATE slt_autoregister_temp SET batch_id = :batch_id
					WHERE custom4 is not null AND cls_id = :clsid AND object_type != :object_type AND batch_id IS NULL LIMIT $autoreglmt ";
					$arg = array(
							':batch_id'=>$btId,
							':clsid' => $key->cls_id,
							':object_type'=>'Class',
					);
					expDebug::dPrintDBAPI("Auto TP Reg batch id update qry- ",$upd,$arg);
					db_query($upd,$arg);
					db_query('COMMIT');
					$btId++;
				}
			}
			
			
		}catch (Exception $ex) {
			$pList = db_query('SHOW FULL PROCESSLIST')->fetchAll();
			expDebug::dPrint("Process list while error in mandatoryCertificationAutoRegister ".print_r($pList,true),1);
			throw new Exception($ex->getMessage());
		}
	}
	
	

	private function complianceAutoReregistration(){
		try{
				
			// Get the Maximum Batch id available from slt_autoregister_temp table
			$tmpSelect = db_select('slt_autoregister_temp','temp');
			$tmpSelect->addExpression('MAX(batch_id)','bid');
			$btcCnt = $tmpSelect->execute()->fetchField();
	
			$btId = $btcCnt + 1;
			$autoreglmt = getConfigValue('autoregister_limit');
				
			$qry = db_select('slt_enrollment','enr');
			$qry->innerJoin('slt_course_template','crs','crs.id = enr.course_id');
			$qry->innerJoin('slt_course_class','cls','cls.id = enr.class_id');
			$qry->innerJoin('slt_person','per','per.id = enr.user_id');
			$qry->leftjoin('slt_autoregister_temp', 'temp', "temp.cls_id = enr.class_id AND temp.userids = enr.user_id and temp.object_type = 'Class'");
			$qry->addField('enr','user_id','userids');
				
				
			$qry->addExpression('NULL','batch_id');
			$qry->addExpression("'PR'",'record_status');
			$qry->addExpression('NULL','prcsts');
			$qry->addExpression('NULL','notifysts');
			$qry->addExpression('NULL','notifyuser');
			$qry->addExpression('NULL','remarks');
			$qry->addExpression('NULL','custom0');
			$qry->addExpression('NULL','custom1');
			$qry->addExpression('NULL','custom2');
			$qry->addExpression('NULL','custom3');
			$qry->addExpression(1,'custom4');
				
			$qry->addExpression('cls.id','cls_id');
			$qry->addExpression('cls.code','cls_code');
			$qry->addExpression('cls.title','cls_title');
			$qry->addExpression('cls.short_description','cls_short_description');
			$qry->addExpression('cls.price','price');
			$qry->addExpression('cls.currency_type','currency_type');
			$qry->addExpression('cls.registration_end_on','registration_end_on');
			$qry->addExpression('cls.created_on','created_on');
			$qry->addExpression('cls.published_on','published_on');
			$qry->addExpression('NULL','mro_id');
			$qry->addExpression('cls.lang_code','language_code');
			$qry->addExpression('NULL','language');
			$qry->addExpression('NULL','start_date');
			$qry->addExpression('cls.location_id','location_id');
			$qry->addExpression('cls.course_id','crs_id');
			$qry->addExpression('cls.delivery_type','delivery_type_code');
			$qry->addExpression('NULL','delivery_type_name');
			$qry->addExpression('NULL','expires_in_value');
			$qry->addExpression('NULL','expires_in_unit');
			$qry->addExpression('NULL','prm_created_on');
			$qry->addExpression('cls.custom4','export_compliance');
			$qry->addExpression('\'Class\'','object_type');
			$qry->addExpression('crs.is_compliance','is_compliance');
			$qry->addExpression('NULL','avgvote');
			$qry->addExpression('NULL','ascen');
				
				
			$qry->condition('temp.id','','is null');
			$qry->condition(db_and()->condition('crs.is_compliance', 1, '=')
					->condition(db_or()->isNull('crs.complete_date')
							->condition('crs.complete_date', NOW(), '>')
					)
			);
			$qry->where('if(crs.validity_date IS NOT NULL,crs.validity_date >= curdate(),1=1 )');
			$qry->condition('crs.status', 'lrn_crs_sts_atv','=');
		//	$qry->condition('cls.status', 'lrn_cls_sts_atv','=');	// this was uncommented for #64835
			$price_condition_or = db_or();
			$price_condition_or->condition('cls.price', 0);
			$price_condition_or->isNull('cls.price');
			$qry->condition($price_condition_or);
			$qry->condition('enr.is_compliance', 1,'=');
			$qry->condition('enr.comp_status', 'lrn_crs_cmp_exp','=');
			$qry->condition('per.status', 'cre_usr_sts_atv','=');
			//$qry->groupBy('enr.course_id');
			$qry->groupBy('enr.id');
				
			expDebug::dPrintDBAPI("Query result for Compliance re registration",$qry);
			db_insert('slt_autoregister_temp')
			->from($qry)
			->execute();
			db_query('COMMIT');
				
			$res = db_select('slt_autoregister_temp', 'tmp');
			$res->addField('tmp', 'cls_id');
			$res->addField('tmp', 'crs_id');
			$res->condition('tmp.custom4',1,'=');
			$res->condition('tmp.batch_id','','IS NULL');
			$res->groupBy('tmp.cls_id');
			$res->groupBy('tmp.crs_id');
			$res->groupBy('tmp.object_type');
			expDebug::dPrintDBAPI("Select query for temp table",$res);
			$clsRes = $res->execute()->fetchAll();
				
			foreach($clsRes as $key){
				$sel = db_select('slt_autoregister_temp', 'autotmp');
				$sel->addExpression('count(1)','cnt');
				$sel->condition('autotmp.cls_id',$key->cls_id,'=');
				$sel->condition('autotmp.custom4',1,'=');
				$sel->condition('autotmp.object_type','Class','=');
				$sel->where('autotmp.batch_id is null');
				expDebug::dPrintDBAPI("Auto Reg count qry- ",$sel);
				$cnt = $sel->execute()->fetchField();
				expDebug::dPrint("Auto Reg count - ".$cnt,3);
					
				$pcnt = round($cnt/$autoreglmt) + 1;
				for($o=0;$o<$pcnt;$o++){
					$upd = "UPDATE slt_autoregister_temp SET batch_id = :batch_id
					WHERE custom4 = :cstm AND cls_id = :clsid AND object_type = :object_type AND batch_id IS NULL LIMIT $autoreglmt ";
					$arg = array(
							':batch_id'=>$btId,
							':cstm'=>1,
							':clsid' => $key->cls_id,
							':object_type'=>'Class',
					);
					expDebug::dPrintDBAPI("Auto Reg batch id update qry- ",$upd,$arg);
					db_query($upd,$arg);
					db_query('COMMIT');
					$btId++;
				}
			}
				
		}catch (Exception $ex) {
			$pList = db_query('SHOW FULL PROCESSLIST')->fetchAll();
			expDebug::dPrint("Process list while error in ComplianceAutoRERegister ".print_r($pList,true),1);
			throw new Exception($ex->getMessage());
		}
	}
	
	private function updateExpiredEnrollment(){
		try{
			$upd = "UPDATE slt_enrollment enr
							INNER JOIN temp_autoreg_b".$this->btId." temp ON temp.custom2 = enr.class_id and 
							temp.course_id = enr.course_id and temp.user_id=enr.user_id
							SET enr.comp_status = 'lrn_crs_cmp_cmp', enr.cmpl_expired = 1, enr.updated_on = now(),enr.updated_by=temp.user_id
							WHERE temp.record_status = 'R' and enr.comp_status = 'lrn_crs_cmp_exp' and enr.cmpl_expired = 1";
			expDebug::dPrintDBAPI("updateExpiredEnrollment query ",$upd);
		  db_query($upd);
		} catch (Exception $ex) {
			throw new Exception($ex->getMessage());
		}
	}
	
	private function expiredEnrollmentClassIdUpdate(){
		try{
			$qry = "UPDATE temp_autoreg_b".$this->btId." t1
							INNER JOIN (select count(DISTINCT (cls.id)) AS clscnt, enr.class_id AS clsid, 
							enr.course_id AS course_id, enr.user_id AS user_id, cls.id as classid, cls.delivery_type as deltype 
							FROM slt_enrollment enr
							INNER JOIN temp_autoreg_b".$this->btId." temp ON temp.course_id = enr.course_id AND temp.class_id = enr.class_id
							AND temp.user_id = enr.user_id AND enr.comp_status = :exp
							LEFT JOIN slt_course_class cls ON cls.course_id = temp.course_id
							INNER JOIN slt_person per ON per.id = temp.user_id 
							LEFT JOIN slt_group_mapping map ON map.entity_id = cls.id AND map.entity_type = 'cre_sys_obt_cls' and map.group_type = 0
							LEFT JOIN slt_user_access_mapping umap ON umap.entity_id = cls.id AND umap.entity_type = 'cre_sys_obt_cls' and umap.access_type = 0
							LEFT OUTER JOIN slt_groups grp ON grp.id = map.group_id and (grp.is_admin =0) and grp.status ='cre_sec_sts_atv'
							LEFT OUTER JOIN slt_group_attributes grpatt ON grp.id = grpatt.group_id
							WHERE "; 
			$upld = new enrollment_upload();
			$qry .= $upld->userGroupAccessValidate();
			$qry .= " AND
							CASE WHEN cls.id = temp.class_id AND cls.status != :atv THEN temp.class_id != cls.id AND cls.status = :atv
							WHEN temp.custom3 = :ilt OR temp.custom3 = :vcl AND cls.status != :atv THEN temp.class_id != cls.id AND cls.status = :atv
							ELSE temp.class_id = cls.id END
							GROUP BY enr.id) x ON x.course_id = t1.course_id AND x.user_id = t1.user_id
							SET t1.class_id = (CASE WHEN x.clscnt = 1 THEN x.classid ELSE t1.class_id END), 
							t1.custom3 = (CASE WHEN x.clscnt = 1 THEN x.deltype ELSE t1.custom3 END),  
							t1.record_status = (CASE WHEN x.clscnt > 1 THEN :mn WHEN (x.clscnt = 0 AND (t1.custom3 != :wbt AND t1.custom3 != :vod )) 
							THEN :in ELSE t1.record_status END),
							t1.notifyuser = (CASE WHEN x.clscnt > 1 THEN 'MultipleComplianceClassNotification' ELSE t1.notifyuser END)
							WHERE t1.record_status = :r ";
			
			$args = array(':r' => 'R',':in' => 'IN',':mn' => 'MN',':atv' => 'lrn_cls_sts_atv',':exp' => 'lrn_crs_cmp_exp',
					':wbt' => 'lrn_cls_dty_wbt',':vod' => 'lrn_cls_dty_vod',':all' => 'All',':n' => 'N',':ilt' => 'lrn_cls_dty_ilt', ':vcl' => 'lrn_cls_dty_vcl');
			expDebug::dPrintDBAPI("Query for update the enrollment class id",$qry,$args);
			db_query($qry,$args);
			
			// Active Class Validation
			
			$atvQry = "UPDATE temp_autoreg_b".$this->btId." tmp
							LEFT JOIN slt_course_class cls ON cls.id = tmp.class_id AND cls.status != 'lrn_cls_sts_atv'
							LEFT JOIN slt_course_template crs on crs.id = tmp.course_id AND crs.status != 'lrn_crs_sts_atv'
							SET tmp.record_status = :in ,
							tmp.custom4 = concat(ifnull(tmp.custom4,''), 'Class is not active.')
							WHERE	(cls.status != 'lrn_cls_sts_atv' OR crs.status != 'lrn_crs_sts_atv') AND tmp.record_status = :r ";
			$args = array(':r' => 'R',':in' => 'IN');
			expDebug::dPrintDBAPI("Update Query for active class validation",$atvQry,$args);
			db_query($atvQry,$args);
		} catch (Exception $ex) {
			throw new Exception($ex->getMessage());
		}
	}
	
	private function moveInvalidRecordstoTemp($tblName){
		try{
			$updQry = "UPDATE slt_autoregister_temp t1
								INNER JOIN ".$tblName." t2 ON t2.mapping_id = t1.id
								SET t1.record_status = t2.record_status, t1.remarks = concat(ifnull(t1.remarks,''), ifnull(t2.custom4,''))
								WHERE t1.batch_id = :btid AND t2.record_status= :in ";
			$args = array(':btid' => $this->btId,':in' => 'IN');
			expDebug::dPrintDBAPI("Query for moving the invalid records to temp table",$updQry,$args);
			db_query($updQry,$args);
		}catch(Exception $ex){
			throw new Exception($ex->getMessage());
		}
	}
	
	private function updateRecordStatus($sts,$tblName){
		try{
			$updQry = "UPDATE ".$tblName." t1
								SET t1.record_status = :sts
								WHERE t1.batch_id = :btid AND t1.record_status NOT IN ( :in , :cp ) 
								AND (t1.custom3 = :wbt OR t1.custom3 = :vod ) ";
			$args = array(':btid' => $this->btId,':in' => 'IN', ':cp' => 'CP', ':wbt' => 'lrn_cls_dty_wbt', ':vod' => 'lrn_cls_dty_vod', ':sts' => $sts);
			expDebug::dPrintDBAPI("Query for updating record status to temp table",$updQry,$args);
			db_query($updQry,$args);
		} catch(Exception $ex){
			throw new Exception($ex->getMessage());
}
	}
	
	
	private function UpdateProgressforTP(){
		try{
				
			$selQry = 'SELECT  menr.id , menr.recertify_path  from slt_master_enrollment menr inner join temp_autoreg_class_b'.$this->btId.' t2  on menr.id  = t2.master_enrollment_id
          WHERE t2.batch_id =  :btid  and menr.program_id =  :prgid ';
			expDebug::dPrint(' $object result>>> omggg prii '.print_r($selQry,true),5);
			$args=array(
					':btid' => $this->btId,
					':prgid' => $this->classId,
			);
			$TpDetails = db_query($selQry, $args)->fetchAll();
			expDebug::dPrintDBAPI("Query for updating saaaa",$selQry,$args);
			expDebug::dPrint(' $object result>>> omggg '.print_r($TpDetails,true),5);
			foreach($TpDetails as $tp_detail){
				updateTPEnrollmentOverallPercentComplete($this->classId,$tp_detail->id,$tp_detail->recertify_path);
			}
		} catch(Exception $ex){
			throw new Exception($ex->getMessage());
		}
	}
}
?>