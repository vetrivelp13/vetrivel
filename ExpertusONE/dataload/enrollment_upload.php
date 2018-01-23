<?php 
require_once "data_load_validate.php";
include_once $_SERVER['DOCUMENT_ROOT'].'/includes/bootstrap.inc';
include_once $_SERVER['DOCUMENT_ROOT'].'/includes/common.inc';
include_once $_SERVER['DOCUMENT_ROOT'].'/includes/database/database.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

//require_once DRUPAL_ROOT."/sites/all/commonlib/ExpertusSelectQueryExtender.php";

class enrollment_upload extends dataloadValidate{
	
	private $db;
	private $jobDetail;
	private $batchrecords;
	private $tblDet;
	
	function __construct($from=''){
		$this->db = new DLDatabase('DL',$from);
		$this->db->update_queue = true;
	}
	
	public function extenderValidate($jobDetail=array(),$batchrecords=array()){
		try{
			expDebug::dPrint('Job details '.print_r($jobDetail,true),4);
			expDebug::dPrint('Batch records '.print_r($batchrecords,true),4);
			
			$this->setJobDetails($jobDetail);
			$this->setBatchRecords($batchrecords);
			
			//Alter tables
			$this->alterTables();
			
			//Update class id in temp1 
			$this->updateTemp1ClassID();
			
			//Validation for irrelevant class code
			$this->validateClassCode();
                        
                        //validate the lang for the class
                        $this->updateInvalidLang();
                        			
			// Update Invalid class update
			$this->updateInvalidClass();
			
			//Update class id in temp2 table
			$this->updateTemp2ClassID();
			
			// Delete invalid record from temp2 table where class id or course id is null
			$this->deleteTemp2InvalidRecords();
			
			// Update Invalid course in temp1 and temp2 
			$this->updateInvalidCourse();	
			
			//if the class having buisness rule custom2 column from temp2 is updated
			$this->checkBusinessRule();
                        	
			// Duplicate status check
			$this->duplicateCheck();
			
			//Update for registration status
			$this->updateRegStatus();
			
		}catch(Exception $e){
			$this->db->callQuery('SHOW FULL PROCESSLIST');
			$pList = $this->db->fetchAllResults();
			expDebug::dPrint("Process list while error in extenderValidate ".print_r($pList,true),1);
			expDebug::dPrint("Exception in validation process ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	public function validate($jobDetail=array(),$batchrecords=array()){}
	
	public function bulkValidate($jobDetail=array(),$batchrecords=array()){
		try{
			expdebug::dPrint(' Job Details '.print_r($jobDetail,true),5);
			expdebug::dPrint(' batchrecords '.print_r($batchrecords,true),5);
			
			$this->setJobDetails($jobDetail);
			$this->setBatchRecords($batchrecords);
			
			/** TODO : No need to change the registration and registration status date if already enrolled and not recurring registration
			 */
			
			//Update Invalid record from temp1 to temp2. (Moved from extender validate to validate since the process is in progress when the batch id is null)
			$this->updateInvalidRecs();
			
			// Update User id column and update the remark if user is not active in Temp2 table
			$this->updateUserId();	
			
			//Update the reg_date and comp_date column
			$this->updateStatusDates();
			
			// Check if alreay enrolled or not
			$this->enrollCheck();
                        
                        //Check if priced class
                        $this->priceClassCheck();
			
                        //Check if score is provided when user enrollment status provided as enrolled
                        $this->scoreEnrollCheck();
			
                        // Check if the class got content & session attached to it
			$this->contentSessionCheck();
			
			//Reg date and comp_date is null check
			$this->dateValidation();
			
			//Update Already canceled record
			$this->updateCancelRecords();
			
			// Update delivery type each class in Temp2 table
			$this->updateDeliveryType();
			
			// noshow status sholud not allowed for WBT and VOD
			$this->selfLearnNoShowCheck();
			
			// Update instructor conflict for ILT/VC class enrollments.
			$this->instructorConflict();
			
			//Update the session conflict to user and session conflict if the person is manager.
			$this->sessionConflict();
                        
                        //Update the completed course validation
			$this->courseCompleteCheck();
			
			// Pre-requisite Validation
			$this->preRequisiteValidation();

			// User Access validation 
		//	$this->userAccessCheck();
			
			// User Privilege validation 
			$this->userPrivilegeCheck();
                        
                        //User Access Privilege validation as per Data Segmentation
                     // $this->userEnrollGroupPrivilegeCheck();
			
			// User Hire date Validation 
			$this->hireDateValidation();
			
			// mandatoryCode  check
			$this->mandatoryCodeCheck();
			
			// Completion status check 
			$this->completionCheck(); 
			 
			// Update is_compliance column in temp2 table
			$this->complianceUpdate(); 
			
			
		
			 
			// Update mandatory column in temp2 table
			$this->updateMandatory(); 
			 
			//Update primary id if already registered and update the operation column as insert
			$this->checkAlreadyRegistered(); 	
			
			//Update for Recurring Registration.
			$this->updateRecurringReg();	
			 
			// Update notifyuser column for notification update in temp2 table
			$this->updateNofifyType();
			
			$tblDet['tblname'] = $jobDetail['pTable'];
			$tblDet['btc_id'] = $jobDetail['batchId'];
			$this->setTableDetails($tblDet);
			// Update the User email id and user uid for notification.
			$this->updateManagerEmailId();
			
			// Update the manager email id and user uid for notification.
			$this->updateManagerEmailId(TRUE);
			
			$this->updateEnrollStatus();
			$this->updateProgress();
		}catch (PDOException $ex) {
			$this->db->callQuery('SHOW FULL PROCESSLIST');
			$pList = $this->db->fetchAllResults();
			expDebug::dPrint("Process list while error in bulkValidate ".print_r($pList,true),1);
			expDebug::dPrint("PDO Error in Validating the batch records for users",1);
			throw new PDOException($ex->getMessage());
		}catch(Exception $e){
			$this->db->callQuery('SHOW FULL PROCESSLIST');
			$pList = $this->db->fetchAllResults();
			expDebug::dPrint("Process list while error in bulkValidate ".print_r($pList,true),1);
				// TODO: throw exception
			expDebug::dPrint("ERROR in bulkValidate ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	public function execute($updVal){}
	
	public function bulkExecute($tblDet=array()){
		expDebug::dPrint(' table det '. print_r($tblDet,true),5);
		try{
		   // $tblDet['user_id'] = (!empty($tblDet['custom0']) && $tblDet['custom0'] > 0 ) ? $tblDet['user_id'] : 2;
			$this->setTableDetails($tblDet);
			
			// Insert operation Starts
			$dtOpt = array('lrn_cls_dty_wbt','lrn_cls_dty_vod',$tblDet['oper']);
			//Fetch count of wbt and vod class.
			$cntWbtVod = $this->getDTCount($dtOpt); 		

		 	if($cntWbtVod){
		 		//Only for WBT and VOD
				// Insert for slt_order table.
				$this->insertOrder($dtOpt);	

				//Update order_id in temp2 table.
 				$this->updateTemp2OrderId($dtOpt);
 					
				// Insert for slt_order_items.
				$this->insertOrderItem($dtOpt);
					
				//Insert for slt_enrollment table.
				$this->insertEnrollment($dtOpt);
				
				//Insert for slt_enrollment content mapping table.
				$this->insertEnrollContentMap();				
 		  }

 		  $dtOpt = array('lrn_cls_dty_ilt','lrn_cls_dty_vcl',$tblDet['oper']);
		 	//Fetch count of ilt and vc class.
		 	$cntIltVcl = $this->getDTCount($dtOpt);
		 		 
		 	if($cntIltVcl){
		 		//Allocating seats
		 		$this->seatAllocation();
					
		 		// Insert for slt_order table.
				$this->insertOrder($dtOpt);

				//Update order_id in temp2 table.
				$this->updateTemp2OrderId($dtOpt);
				
				// Insert for slt_order_items.
				$this->insertOrderItem($dtOpt);
				
				//Insert for slt_enrollment table.
				$this->insertEnrollment($dtOpt);
 		  }
		 	// Insert operation End
 		  
			// Update operation Start.
 		  $dtOpt = array('lrn_cls_dty_ilt','lrn_cls_dty_vcl','update');
		 	//Fetch count of ilt and vc class.
		 	$updCntIltVc = $this->getDTCount($dtOpt);
		 	
			if($updCntIltVc){
				// Update Enrollment
				$this->updateEnrollment($dtOpt);
					
				//Seat Allocation
				$this->seatReAllocation();
		 			
		 		//Update order_id in temp2 table.
		 		$this->updateOrderId($dtOpt);
 		  }
 		  
 		  $dtOpt = array('lrn_cls_dty_wbt','lrn_cls_dty_vod','update');
		 	//Fetch count of wbt and vod class.
		 	$updCntwbtVod = $this->getDTCount($dtOpt);
		 		
	 		if($updCntwbtVod){
		 		// Update enrollment
		 		$this->updateEnrollment($dtOpt);
		 		
		 		//Update order_id in temp2 table.
		 		$this->updateOrderId($dtOpt);
	 		}
		 	// Update operation End.
		 	
	 		// TP Rollup
	 		$this->tpRollup();
	 		
		 	// Audit trail entry for Registration and completion
			$this->insertAuditTrail();
						
			//User Points for Insert for registered and completion status
			$this->insertUserPoints();

			//Update query to update the user points if the registration cancelled.
			$this->updateUserPoint();
				
			// Notification part
			$this->insertNotification();
			
			//reset slt_order custom_dataload column to null.
			$this->resetOrderTableEntry();	
		 	
			// Solr sync up for enrollmets if the sync throws any exception 
			// that should not call deleteInsertRecords(), hence used try..catch.
			try{
				syncSolrData('Enrollment','','Bulk');
				syncSolrData('MasterEnrollment','','Bulk');	
				syncSolrData('User','','Bulk');
			}catch(Exception $e){
				// Do nothing
			}
		}catch (PDOException $ex) {
			expDebug::dPrint("PDO Error in Executing the batch records for users",1);
			$this->db->callQuery('SHOW FULL PROCESSLIST');
			$pList = $this->db->fetchAllResults();
			expDebug::dPrint("Process list while error in batch execute ".print_r($pList,true),1);
			//$this->db->rollbackTrans();
			//$this->db->closeconnect();
			$this->deleteInsertRecords();
			throw new PDOException($ex->getMessage());
		}catch(Exception $e){
			$this->db->callQuery('SHOW FULL PROCESSLIST');
			$pList = $this->db->fetchAllResults();
			expDebug::dPrint("Process list while error in batch execute ".print_r($pList,true),1);
			//$this->db->rollbackTrans();
			//$this->db->closeconnect();
			$this->deleteInsertRecords();
			expDebug::dPrint("ERROR in bulkExecute ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function enrollmentConflictforUser($personId, $objectId,$mapping_id,$jobDetail,$sessionDt){
	 	//try{
 		$classSessionsDateTimeList = $sessionDt;
 		$numClassSessions = count($classSessionsDateTimeList);
 		if ($numClassSessions <= 0) {
 			// No conflict since no sessions.
 			expDebug::dPrint(' No conflict since no sessions.', 4);
 			return;
 		}
	 	$enrolledClassesList = getUserEnrollments($personId, true);
  	expDebug::dPrint(' $enrolledClassesList = ' . print_r($enrolledClassesList, true) ,4);
  
  	// Check for session timing overlap
  	$conflictStatus = 0;
  	try {
            //check if user is an instructor to a class with session conflictd
            $conflictStatus = getSessionClassInstructorConflict($personId, $objectId);
            expDebug::dPrint(' getSessionClassInstructorConflict = ' . print_r($conflictStatus, true),5);
            if(!empty($conflictStatus)){
                    $updQry = "update ".$jobDetail['pTable']." t2 
                                                            set t2.custom4 = concat(ifnull(t2.custom4,''), 'Session conflict with already enrolled class.'),
                                                                t2.record_status = :invr  
                                                            where t2.batch_id = :btcid and t2.mapping_id = :mpid ";
                    $args = array(':btcid'=> $jobDetail['batchId'],':mpid'=> $mapping_id,':invr'=>'IN');
                    expDebug::dPrintDBAPI('Session conflict update -- ',$updQry,$args);
                    $this->db->callExecute($updQry,$args);
	      }
              
                //To check if the feed contain user to enroll
                $sql = "select t2.user_id as userId, group_concat(t2.class_id) as classIds from ".$jobDetail['pTable']." t1
                      inner join ".$jobDetail['pTable']." t2 on t2.mapping_id = t1.mapping_id
                      where t2.custom3 IN ('lrn_cls_dty_ilt','lrn_cls_dty_vcl')
                      and t2.record_status = 'R'
                      group by t2.user_id having count(t2.user_id)>1";
                // -- left join slt_course_class_session ses on ses.class_id = t2.class_id

                $this->db->callQuery($sql);
                $checkFromFeed = $this->db->fetchAllResults();

                expDebug::dPrintDBAPI("The Query to check conflict class-> ",$sql);
                expDebug::dPrintDBAPI("The Query to check conflictconflict class-> " .print_r($checkFromFeed,true),5);
                
                if(!empty($checkFromFeed)){
                        foreach($checkFromFeed as $user){
                            $classIds = explode(',',$user->classIds);
                            $conflictStatus = trainingProgramMultiClassConflictCheck($classIds);
                            expDebug::dPrint(' MultiClassConflictCheck = ' . print_r($conflictStatus, true),5);
                                if(!empty($conflictStatus)){
                                        $updQry = "update ".$jobDetail['pTable']." t2 
                                                                                set t2.custom4 = concat(ifnull(t2.custom4,''), 'Session conflict with already enrolled class.'),
                                                                                    t2.record_status = :invr  
                                                                                where t2.batch_id = :btcid and t2.mapping_id = :mpid ";
                                        $args = array(':btcid'=> $jobDetail['batchId'],':mpid'=> $mapping_id,':invr'=>'IN');
                                        expDebug::dPrintDBAPI('Session conflict update -- ',$updQry,$args);
                                        $this->db->callExecute($updQry,$args);
                                  }
                        }
                }
    	// Check conflict with each enrolled class
    	foreach ($enrolledClassesList as $enrolledClass) {
      
	      // if the registered class is same in traing plan
	      if($enrolledClass['class_id'] == $objectId)
	      {
	        expDebug::dPrint(' if the registered class is in traing plan then no conflict', 4);
	        continue; // There is no conflict if the registered class is in traing plan then no conflict
	      }
	      // If the enrolledClass is not ilt or vc, we do not have to check for datetime conflict
	      /*$enrClassDeliveryType = getClassDeliveryType($enrolledClass['class_id'], 'Short');
	      expDebug::dPrint(' $enrClassDeliveryType = ' . print_r($enrClassDeliveryType, true) , 4);
	      if ($enrClassDeliveryType != 'ilt' && $enrClassDeliveryType != 'vc') {
	        expDebug::dPrint(' enrolled Class is not ilt or vc - skip', 4);
	        continue; // There is no conflict with this enrolled class. Skip to the next enrolled class
	      }*/
	      // Get the date, start_time and end_time of all the sessions in the enrolledClass.
	      // The results are sorted by date ascending.
	      // Each session starts and ends the same day.
	      $enrClassSessionsDateTimeList = getClassSessionsDateTime($enrolledClass['class_id']);
	      expDebug::dPrint(' $enrClassSessionsDateTimeList = ' . print_r($enrClassSessionsDateTimeList, true) , 4);
	      $numEnrClassSessions = count($enrClassSessionsDateTimeList);
	      
	      // If the enrolled class does not have any sessions, there is no datetime conflict with this class.
	      // However, when admin alters (deletes all in particular) sessions of a class, the user enrollment for the class
	      // is most likely automatically cancelled. Cancelled classes would not be returned by getUserEnrollments() above.
	      if ($numEnrClassSessions <= 0) {
	        expDebug::dPrint(' enrolled Class does not have sessions - skip', 4);
	        continue; // There is no conflict with this enrolled class. Skip to the next enrolled class
	      }
	      
	      // If the enrolledClass's min date - max date are outside the this class's min date - max date, there is no
	      // datetime conflict with this class.
	      // If class min start date > enrClass max start date, there is no conflict.
	      // If class max start date < enrClass min start date, there is no conflict  
	      if ($classSessionsDateTimeList[0]['date'] > $enrClassSessionsDateTimeList[$numEnrClassSessions - 1]['date'] ||
	            $classSessionsDateTimeList[$numClassSessions - 1]['date'] < $enrClassSessionsDateTimeList[0]['date'] ) {
	        expDebug::dPrint(' class min-max dates outside enr class min-max dates - skip', 4);
	        continue; // There is no conflict with this enrolled class. Skip to the next enrolled class.
	      }
	      
	      // Check for conflict for each classSession with every enrClassSession
	      foreach ($classSessionsDateTimeList as $classSessionsDateTime) {
	        foreach ($enrClassSessionsDateTimeList as $enrClassSessionsDateTime) {
	          // If the classSession is on a different date than enrClassSession, there is no conflict
	          if ($classSessionsDateTime['date'] != $enrClassSessionsDateTime['date']) {
	            expDebug::dPrint(' enrClassSession date is different - skip', 4);
	            continue; // There is no conflict with this enrolled class session. Skip to the next enrolled class session.
	          }
	          
	          // classSession is on the same date as enrClassSession
	          // If the classSession's start_time - end_time is outside enrClassSession's start_time - end_time, there is no
	          // conflict
	          // If classSession start_time >= enrClassSession end_time, there is no conflict.
	          // If classSession end_time <= enrClassSession start_time, there is no conflict          
	          if ($classSessionsDateTime['start_time'] >= $enrClassSessionsDateTime['end_time'] ||
	                $classSessionsDateTime['end_time'] <= $enrClassSessionsDateTime['start_time']) {
	            expDebug::dPrint(' classSession\'s start_time - end_time is outside enrClassSession\'s start_time - end_time - skip', 4);
	            continue; // There is no conflict with this enrolled class session. Skip to the next enrolled class session.
	          }
	          
	          if(!empty($enrolledClass['class_id'])){
		          // A conflict is found. Save conflicting classId in $conflictStatus exit all loops
		          $conflictStatus = $enrolledClass['class_id'];
		          continue;
		          //throw new Exception('Conflict');
	          }
	        } // end foreach ($enrClassSessionsDateTimeList as $enrClassSessionsDateTime)
	      } // end foreach ($classSessionsDateTimeList as $classSessionsDateTime)
	    	if(!empty($conflictStatus)){
	    		$updQry = "update ".$jobDetail['pTable']." t2 
	    							set t2.custom4 = concat(ifnull(t2.custom4,''), 'Session conflict with already enrolled class.'),
	    							    t2.record_status = :invr  
	    							where t2.batch_id = :btcid and t2.mapping_id = :mpid ";
	    		$args = array(':btcid'=> $jobDetail['batchId'],':mpid'=> $mapping_id,':invr'=>'IN');
	    		expDebug::dPrintDBAPI('Session conflict update -- ',$updQry,$args);
	      	$this->db->callExecute($updQry,$args);
	      }
	    } // end foreach ($enrolledClassesList as $enrolledClass)
	  } catch (Exception $ex) { // end try
	    expDebug::dPrint(' In exception handler with $conflictStatus = ' . print_r($conflictStatus, true) , 4);
	  }
  }
  
	private function updateInvalidRecs(){
	  try{
	  	$jobDetail = $this->getJobDetails();

	  	$t2Tbl = explode('_',$jobDetail['sTable']);
	  	//Update temp table 2 with invalid records
	  	list($fields, $args,$join,$condition) = array(array(),array(),array(),array());
	  	$fields=array(
	  			't2.custom4' => 't1.remarks',
	  			't2.record_status' => 't1.record_status'
	  	);
	  	$condition=array(
	  			't2.batch_id = :batch_id '
	  	);
	  	$join=array(
	  			'inner join '.$jobDetail['sTable'].' t1 on t1.mapping_id = t2.mapping_id AND t1.record_status = :inv',
	  	);
	  	$args = array(
	  			':batch_id' => $jobDetail['batchId'],
	  			':inv' => 'IN'
	  	);
	  	$qry1 = $this->db->prepareQueryUpdate("temp2_".$t2Tbl[1]." t2",$fields,$condition,$join);
	  	expDebug::dPrintDBAPI(" Update Temp 2 table ",$qry1,$args);
	  	$this->db->callExecute($qry1,$args); 
	  	
	    
	    $arg=array(
	         ':sts' => 'IN',
	         ':batch_id' => $jobDetail['batchId']
	       );
	    $query = 'DELETE t2 FROM '.$jobDetail['pTable'].' t2 INNER JOIN '.$jobDetail['sTable'].
	             ' t1 on t1.mapping_id = t2.mapping_id AND t1.record_status = :sts
	               WHERE t2.batch_id = :batch_id ';
	    expDebug::dPrintDBAPI(" DELETE Invalid record from temp2. ",$query,$arg);
	    $this->db->callExecute($query,$arg);
	  }catch(Exception $e){
	  	expDebug::dPrint("ERROR in update invalid record ".print_r($e,true),1);
	    throw new Exception($e->getMessage());
    }
  }
	
	private function updateUserId(){
		try{
			$jobDetail = $this->getJobDetails();
			$batchrecords = $this->getBatchRecords();
			$update = array();
			$update['table'] = $jobDetail['pTable'].' tm';
			$update['fields'] = array(
					'tm.user_id'=> "CASE WHEN per.id IS NULL THEN NULL ELSE per.id END",
					'tm.custom4' => "CASE WHEN per.status != :sts THEN CONCAT(ifnull(tm.custom4,''), :err )
					WHEN per.id IS NULL THEN CONCAT(ifnull(tm.custom4,''), :usrerr ) ELSE tm.custom4 END",
					'tm.record_status' => "CASE WHEN per.status != :sts THEN :recst
					WHEN per.id IS NULL THEN :recst ELSE tm.record_status END"
			);
			$update['joins'] = array (
				"INNER JOIN ".$jobDetail['sTable']." t1 on t1.mapping_id = tm.mapping_id ",
				"LEFT JOIN slt_person per on t1.".$batchrecords['user_id']. " = per.user_name "
			);
			$update['condition']=array(
					"tm.batch_id = :batch_id ",
					"tm.operation = :opt "
			);
			$arg=array(':sts' => 'cre_usr_sts_atv',
					':usrerr' => 'User not exist in LMS. ',
					':err' => 'User is not active. ',
					':batch_id' => $jobDetail['batchId'],
					':recst' => 'IN'
			);
				
			//$updQry1 = $this->db->prepareUpdateBySelect($update,$selectQry);
			$updQry1 = $this->db->prepareQueryUpdate($update['table'],$update['fields'],$update['condition'],$update['joins']);
			//expDebug::dPrintDBAPI(" Query for user id and active user update ",$updQry1,$arg);
			$oper = array('insert','update','upsert');
			foreach($oper as $opt){
				$arg[':opt'] = $opt;
				expDebug::dPrintDBAPI(" Query for user id and active user update ",$updQry1,$arg);
			$this->db->callExecute($updQry1,$arg);
			}
		}catch(Exception $e){
			expDebug::dPrint("ERROR in update userid ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function enrollCheck(){
		try{
			$jobDetail = $this->getJobDetails();
			$batchrecords = $this->getBatchRecords();
			$update = array();
			$update['table'] = $jobDetail['pTable'].' t2';
			$update['joins'] = array(
				'INNER JOIN '.$jobDetail['sTable'].' t1 on t1.mapping_id=t2.mapping_id',
				'INNER JOIN slt_enrollment e on e.id = (select max(id) from slt_enrollment where user_id= t2.user_id and class_id = t2.class_id  )'
								);
			
			$update['fields'] = array(
					't2.custom4' => "CASE 
                                                                        WHEN (t1.".$batchrecords['reg_status']." = :reg AND e.comp_status = :compst AND t2.custom2 IS NULL AND t2.course_id NOT IN (select crs.id
																		 from slt_course_template crs 
																		inner join slt_enrollment enr on enr.course_id =  crs.id 
																		inner join slt_notification_info notify on notify.notification_code= 'compliance_expiry_remainder'
																		where 
																		enr.user_id = t2.user_id  and 
																		
																		if(enr.comp_status= 'lrn_crs_cmp_cmp' AND e.is_compliance = 1 AND crs.is_compliance= 1 AND (crs.validity_date IS NOT NULL OR crs.validity_days IS NOT NULL),
																		
																		  			if(((crs.validity_date IS NOT NULL and (curdate() > crs.validity_date)) OR (crs.validity_days IS NOT NULL and (curdate() > DATE_ADD(enr.comp_date, interval crs.validity_days day)) )),
																		  			(DATEDIFF(curdate(),if(crs.validity_days IS NOT NULL,(DATE_SUB(DATE_ADD(enr.comp_date, interval crs.validity_days day), interval notify.notify_before day)),
																		  			(DATE_SUB(crs.validity_date, interval notify.notify_before day)))) > 0),1=2)
																		,1=2))) THEN concat(ifnull(t2.custom4,''),  :alcomp )
                                                                        WHEN (t1.".$batchrecords['reg_status']." = :reg AND e.comp_status  IN (:inp,:enr) AND t2.custom2 = :brule ) THEN concat(ifnull(t2.custom4,''),  :alenroll )
                                                                        WHEN (t1.".$batchrecords['reg_status']." = :reg AND e.comp_status NOT IN (:nsw,:inc,:expi,:compst) AND t2.custom2 IS NULL) THEN concat(ifnull(t2.custom4,''),  :alenroll )
																		WHEN ((t1.".$batchrecords['reg_status']." = :comp OR t1.".$batchrecords['reg_status']." = :noshow OR t1.".$batchrecords['reg_status']." = :cncl OR t1.".$batchrecords['reg_status']." = :incmp) AND (DATE_FORMAT(t2.reg_date,'%Y-%m-%d') != DATE_FORMAT(e.reg_date,'%Y-%m-%d')) AND e.comp_status = :enr ) THEN  concat(ifnull(t2.custom4,''),  :ir )
					                               						WHEN (((t1.".$batchrecords['reg_status']." = :comp AND (e.score !='')) OR t1.".$batchrecords['reg_status']." = :cncl )  AND e.comp_status = :compst ) AND (e.score !='') AND (t2.custom2 IS NULL) THEN  concat(ifnull(t2.custom4,''),  :alcomp )
                                                                        WHEN (t1.".$batchrecords['reg_status']." = :cncl AND e.comp_status = :attsts ) THEN concat(ifnull(t2.custom4,''),  :att )
                                                                        WHEN (t1.".$batchrecords['reg_status']." = :incmp AND e.comp_status = :compst AND t2.custom2 IS NULL) THEN  concat(ifnull(t2.custom4,''),  :alcomp )
                                                                        WHEN (t1.".$batchrecords['reg_status']." IN (:noshow,:incmp) AND e.comp_status IN (:nsw,:inc)) THEN  concat(ifnull(t2.custom4,''),  :alnsw )
                                                                        ELSE t2.custom4 END",
					't2.record_status' => "CASE 
                                                                        WHEN (t1.".$batchrecords['reg_status']." = :reg AND e.comp_status = :compst AND t2.custom2 IS NULL AND t2.course_id NOT IN (select crs.id
																			 from slt_course_template crs 
																			inner join slt_enrollment enr on enr.course_id =  crs.id 
																			inner join slt_notification_info notify on notify.notification_code= 'compliance_expiry_remainder'
																			where 
																			enr.user_id = t2.user_id and 
																			
																			if(enr.comp_status= 'lrn_crs_cmp_cmp' AND e.is_compliance = 1 AND crs.is_compliance= 1 AND (crs.validity_date IS NOT NULL OR crs.validity_days IS NOT NULL),
																			
																			  			if(((crs.validity_date IS NOT NULL and (curdate() > crs.validity_date)) OR (crs.validity_days IS NOT NULL and (curdate() > DATE_ADD(enr.comp_date, interval crs.validity_days day)) )),
																			  			(DATEDIFF(curdate(),if(crs.validity_days IS NOT NULL,(DATE_SUB(DATE_ADD(enr.comp_date, interval crs.validity_days day), interval notify.notify_before day)),
																			  			(DATE_SUB(crs.validity_date, interval notify.notify_before day)))) > 0),1=2)
																			,1=2) )) THEN :recst
                                                                        WHEN (t1.".$batchrecords['reg_status']." = :reg AND e.comp_status NOT IN (:nsw,:inc,:expi,:compst) AND t2.custom2 IS NULL) THEN :recst
                                                                        WHEN (t1.".$batchrecords['reg_status']." = :reg AND e.comp_status  IN (:inp,:enr) AND t2.custom2 =:brule) THEN :recst					                                                    WHEN ((t1.".$batchrecords['reg_status']." = :comp OR t1.".$batchrecords['reg_status']." = :noshow OR t1.".$batchrecords['reg_status']." = :cncl OR t1.".$batchrecords['reg_status']." = :incmp) AND (DATE_FORMAT(t2.reg_date,'%Y-%m-%d') != DATE_FORMAT(e.reg_date,'%Y-%m-%d')) AND e.comp_status = :enr ) THEN  :recst
                                                                        WHEN (((t1.".$batchrecords['reg_status']." = :comp AND (e.score !='')) OR t1.".$batchrecords['reg_status']." = :cncl ) AND e.comp_status = :compst  AND t2.custom2 IS NULL) THEN :recst 
                                                                        WHEN (t1.".$batchrecords['reg_status']." = :cncl AND e.comp_status = :attsts ) THEN :recst
                                                                        WHEN (t1.".$batchrecords['reg_status']." = :incmp AND e.comp_status = :compst ) THEN :recst 
                                                                        WHEN (t1.".$batchrecords['reg_status']." IN (:noshow,:incmp) AND e.comp_status IN (:nsw,:inc)) THEN :recst 
                                                                        ELSE t2.record_status END"
			);
			$update['condition']=array(
					"e.reg_status != :can ",
					"(e.comp_status NOT IN ( :inc , :nsw ) OR ( e.comp_status IN ( :inc , :nsw ) AND DATE_FORMAT(t2.reg_date,'%Y-%m-%d') = DATE_FORMAT(e.reg_date,'%Y-%m-%d')) or e.comp_status is null )",
					"t2.custom2 is null OR t2.custom2= :brule",
					"t2.batch_id = :batch_id ",
					"t2.record_status != :recst "
			);
			$arg=array(':batch_id' => $jobDetail['batchId'],
								 ':recst' => 'IN',
								 ':reg' => 'enrolled',
			 					 ':comp' => 'completed',
								 ':cncl' => 'canceled',
					':inp' => 'In Progress',
								 ':incmp' => 'incomplete',
								 ':noshow' => 'noshow',
								 ':compst' => 'lrn_crs_cmp_cmp',
								 ':enr' => 'lrn_crs_cmp_enr',
								 ':can' => 'lrn_crs_reg_can',
			 					 ':inc' => 'lrn_crs_cmp_inc',
								 ':nsw' => 'lrn_crs_cmp_nsw',
				':brule' =>	'cre_sys_brl_rra',
                                                                 ':expi' => 'lrn_crs_cmp_exp',
								 ':alenroll' => 'Already enrolled',
								 ':alcomp' => 'Already completed',
								 ':attsts'	=> 'lrn_crs_cmp_att',
								 ':att'	=> 'Already attended',
								 ':alnsw' => 'Already marked as noshow/incomplete',
					             ':ir' => 'Incorrect regdate',
			);
					
			$updQry1 = $this->db->prepareQueryUpdate($update['table'],$update['fields'],$update['condition'],$update['joins']);
			expDebug::dPrintDBAPI(" Query for update alreay enrolled check ",$updQry1,$arg);
			$this->db->callExecute($updQry1,$arg);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in enroll check ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
        
        private function priceClassCheck(){
            try{
                $jobDetail = $this->getJobDetails();
                $batch_table=$jobDetail['pTable'];
                $qry = "SELECT scc.id AS class_id, scc.price AS price, bt.comp_status AS status, bt.reg_status AS reg_status, se.comp_status AS enr_status
                                FROM slt_course_class scc
                                INNER JOIN ".$batch_table." bt ON bt.class_id=scc.id
                                LEFT JOIN slt_enrollment se ON se.class_id=bt.class_id 
                                and se.class_id=scc.id and se.user_id=bt.user_id
                                WHERE scc.status !='lrn_cls_sts_del' AND scc.price>0
                                GROUP BY bt.class_id, bt.user_id";
                $this->db->callQuery($qry);
                $checkIsPricedClass = $this->db->fetchAllResults();
                
                expDebug::dPrintDBAPI("The Query to check price class-> ",$qry);
                
                foreach($checkIsPricedClass as $class){
                    $compStatus = $class->status;
                    $regStatus = $class->reg_status;
                    $enrollSts = $class->enr_status;
                    if(($compStatus=='lrn_crs_cmp_enr' || $compStatus=='lrn_crs_cmp_cmp') && (empty($enrollSts) || $enrollSts == 'lrn_crs_cmp_inc')){
                        $sql = "Update ".$batch_table." set record_status = :recsts, custom4 = :err where class_id = :clsId and comp_status= :cmpsts";
                        $arg = array(
                                ':clsId' => $class->class_id,
                                ':cmpsts' => $compStatus,
                                ':recsts' => 'IN',
                                ':err' => 'Priced offering cannot be registered. Registration can be done from the order screen.'
                            );
                        $this->db->callExecute($sql,$arg);
                    }else if($regStatus=='lrn_crs_reg_can'){
                        $sql1 = "Update ".$batch_table." set record_status = :recsts, custom4 = :err where class_id = :clsId and reg_status= :regsts";
                        $arg1 = array(
                                ':clsId' => $class->class_id,
                                ':regsts' => $regStatus,
                                ':recsts' => 'IN',
                                ':err' => 'Priced training cannot be canceled. Cancellation can be done from the order screen.'
                            );
                        $this->db->callExecute($sql1,$arg1);
                    }
                }
            } catch (Exception $ex) {
                expDebug::dPrint("ERROR in Price class check ".print_r($e,true),1);
		throw new Exception($e->getMessage());
            }
        }
        
        private function scoreEnrollCheck(){
            try{
                $jobDetail = $this->getJobDetails();
                $batch_table=$jobDetail['pTable'];
                $qry = "SELECT bt.course_id AS course_id, bt.class_id AS class_id, bt.user_id AS user_id
                        FROM ".$batch_table." bt
                        WHERE bt.class_id IS NOT NULL 
                        AND bt.comp_status = 'lrn_crs_cmp_enr' 
                        AND bt.score is not null";
                $this->db->callQuery($qry);
                $checkInvalidEnroll = $this->db->fetchAllResults();
                expDebug::dPrintDBAPI("The Query to check invalid enrollment record from feed-> ",$qry);
                
                foreach($checkInvalidEnroll as $class){
                    $classId = $class->class_id;
                    $courseId = $class->course_id;
                    $userId = $class->user_id;
                    
                    $sql1 = "Update ".$batch_table." set record_status = :recsts, custom4 = :err where class_id = :clsId and user_id = :usrId";
                    $arg1 = array(
                            ':clsId' => $classId,
                            ':usrId' => $userId,
                            ':recsts' => 'IN',
                            ':err' => 'Score can be updated only for completed class.'
                        );
                    expDebug::dPrintDBAPI('When score is provided for enrolled status-->>' ,$sql1);
                    $this->db->callExecute($sql1,$arg1);

                }
            } catch (Exception $ex) {
                expDebug::dPrint("ERROR in score Enroll check ".print_r($e,true),1);
		throw new Exception($e->getMessage());
            }
        }
        
        private function contentSessionCheck(){
            try{
                $jobDetail = $this->getJobDetails();
                $batch_table=$jobDetail['pTable'];
                $qry = "SELECT scm.content_id AS content_id, scs.id AS session_id, scc.id AS class_id, scc.delivery_type AS delivery_type from slt_course_class scc
                        INNER JOIN ".$batch_table." bt ON scc.id=bt.class_id and scc.course_id = bt.course_id
                        LEFT JOIN slt_course_content_mapper scm ON scm.class_id = scc.id and scm.class_id = bt.class_id
                        LEFT JOIN slt_course_class_session scs on scs.class_id = scc.id and scs.course_id = scc.course_id and bt.class_id = scs.class_id
                        WHERE scc.status !='lrn_cls_sts_del' AND scc.price=0
                        GROUP BY bt.class_id";
                $this->db->callQuery($qry);
                $checkDeliveryMethods = $this->db->fetchAllResults();
                
                expDebug::dPrintDBAPI("The Query to check Delivery Methods of a class-> ",$qry);
                
                foreach($checkDeliveryMethods as $class){
                    $clsId = $class->class_id;
                    $contentId = $class->content_id;
                    $sessionId = $class->session_id;
                    $delType = $class->delivery_type;
                    
                    if(($delType == 'lrn_cls_dty_wbt' || $delType == 'lrn_cls_dty_vod') && empty($contentId)){
                        $sql1 = "Update ".$batch_table." set record_status = :recsts, custom4 = :err where class_id = :clsId ";
                        $arg1 = array(
                                ':clsId' => $clsId,
                                ':recsts' => 'IN',
                                ':err' => 'There is no content attached to the class.'
                            );
                        expDebug::dPrintDBAPI('When No content attached-->>' ,$sql1);
                        $this->db->callExecute($sql1,$arg1);
                    }
                    if(($delType == 'lrn_cls_dty_ilt' || $delType == 'lrn_cls_dty_vcl') && empty($sessionId)){
                        $sql2 = "Update ".$batch_table." set record_status = :recsts, custom4 = :err where class_id = :clsId ";
                        $arg2 = array(
                                ':clsId' => $clsId,
                                ':recsts' => 'IN',
                                ':err' => 'There are no sessions in the class.'
                            );
                        expDebug::dPrintDBAPI('When No session attached-->>' ,$sql2);
                        $this->db->callExecute($sql2,$arg2);
                    }
                }                
            } catch (Exception $ex) {
                expDebug::dPrint("ERROR in contentSessionCheck ".print_r($e,true),1);
		throw new Exception($e->getMessage());
            }
        }
	
	private function updateStatusDates(){
		try{
			$jobDetail = $this->getJobDetails();
			$batchrecords = $this->getBatchRecords();
			$uid =  $jobDetail['uploaded_user_id'];
			$update = array();
			$update['table'] = $jobDetail['pTable'].' tm';
			$update['fields'] = array(
					'tm.reg_date'=> "DATE_FORMAT(t1.".$batchrecords['reg_date'].",'%Y-%m-%d')",
					'tm.comp_date' => "CASE WHEN (t1.".$batchrecords['reg_status']." = :reg or t1.".$batchrecords['reg_status']." = :inp ) THEN NULL ELSE DATE_FORMAT(t1.".$batchrecords['comp_date'].",'%Y-%m-%d') END",
					'tm.comp_by' => "CASE WHEN (t1.".$batchrecords['reg_status']." = :reg or t1.".$batchrecords['reg_status']." = :inp ) THEN NULL ELSE $uid END",
					'tm.score' => "CASE WHEN t1.".$batchrecords['score']." is not null and t1.".$batchrecords['score']." !='' and t1.".$batchrecords['score']." REGEXP (:regex ) THEN t1.".$batchrecords['score']."
												 WHEN t1.".$batchrecords['score']." is null or t1.".$batchrecords['score']." = '' THEN IF(t1.".$batchrecords['reg_status']." = :comp ,0,NULL) END",
					'tm.custom4' => "CASE 
													 WHEN (t1.".$batchrecords['reg_status']." = :comp or t1.".$batchrecords['reg_status']." = :inc ) and (t1.".$batchrecords['score']." != '' and t1.".$batchrecords['score']." NOT REGEXP (:regex )) THEN concat(ifnull(tm.custom4,''), :screrr )
													 WHEN (date_format(s.start_date,\"%Y-%m-%d\") <  t1.".$batchrecords['reg_date'].") THEN concat(ifnull(tm.custom4,''), :sesregdate ) 
													 WHEN ((t1.".$batchrecords['reg_status']." = :comp or t1.".$batchrecords['reg_status']." =:inc or t1.".$batchrecords['reg_status']." =:nsw ) and t1.".$batchrecords['reg_date']." > t1.".$batchrecords['comp_date'].") THEN concat(ifnull(tm.custom4,''), :dateerr ) 
													 WHEN ((t1.".$batchrecords['reg_status']." = :comp or t1.".$batchrecords['reg_status']." =:inc or t1.".$batchrecords['reg_status']." =:nsw) and date_format(s.start_date,\"%Y-%m-%d\") >  t1.".$batchrecords['comp_date'].") THEN concat(ifnull(tm.custom4,''), :sesdate )
                                                                                                         WHEN ((t1.".$batchrecords['reg_status']." = :comp or t1.".$batchrecords['reg_status']." =:inc or t1.".$batchrecords['reg_status']." =:nsw) and date_format(now(),\"%Y-%m-%d\") < date_format(s.start_date,\"%Y-%m-%d\") ) THEN concat(ifnull(tm.custom4,''), :seserr )
													 ELSE tm.custom4 END",
					'tm.record_status' => "CASE 
													 WHEN t1.".$batchrecords['score']." is not null and t1.".$batchrecords['score']." !='' and t1.".$batchrecords['reg_status']." = :comp and t1.".$batchrecords['score']." NOT REGEXP (:regex ) THEN :recst
													 WHEN ((t1.".$batchrecords['reg_status']." = :comp or t1.".$batchrecords['reg_status']." =:inc or t1.".$batchrecords['reg_status']." =:nsw ) and t1.".$batchrecords['reg_date']." > t1.".$batchrecords['comp_date'].") THEN :recst
													 WHEN ((t1.".$batchrecords['reg_status']." = :comp or t1.".$batchrecords['reg_status']." =:inc or t1.".$batchrecords['reg_status']." =:nsw ) and date_format(s.start_date,\"%Y-%m-%d\") > t1.".$batchrecords['comp_date'].") THEN :recst
                                                                                                         WHEN ((t1.".$batchrecords['reg_status']." = :comp or t1.".$batchrecords['reg_status']." =:inc or t1.".$batchrecords['reg_status']." =:nsw) and date_format(now(),\"%Y-%m-%d\") < date_format(s.start_date,\"%Y-%m-%d\") ) THEN :recst
WHEN (date_format(s.start_date,\"%Y-%m-%d\") < t1.".$batchrecords['reg_date'].") THEN :recst
													 ELSE tm.record_status END"
			);
			$update['joins']=array(
				'INNER JOIN '.$jobDetail['sTable'].' t1 ON t1.mapping_id = tm.mapping_id and t1.record_status !=  \'IN\'',
				'LEFT JOIN slt_course_class_session s ON s.class_id = t1.class_id AND s.id = (SELECT id FROM slt_course_class_session WHERE class_id = t1.class_id ORDER BY start_date, start_time LIMIT 1) '
			);
			$update['condition']=array(
					"tm.batch_id = :batch_id "
			);
			$arg=array(':batch_id' => $jobDetail['batchId'],
								 ':recst' => 'IN',
								 ':reg' => 'enrolled',
			 					 ':comp' => 'completed',
			 					 ':inp' => 'In Progress',
								// ':can' => 'canceled',
			 					 ':inc' => 'incomplete',
								 ':nsw' => 'noshow',
								 ':err' => 'Mandatory column is missing.',
								 ':screrr' => 'Score must be integer',
								 ':dateerr' => 'Completion date should be greater than registration date. ',
								 ':sesdate' => 'Completion date should be greater than session start date. ',
                                                                 ':seserr' => 'User cannot be marked Complete or Incomplete for upcoming classes as session hasn\'t started.',
	        			 ':sesregdate'=> 'Registration date should be less than session start date',
								 //':regex' => '^[0-9]+$'
					       ':regex' => '^[0-9]+(\.[0-9]{1,2})?$'
			);
					
			//$updQry1 = $this->db->prepareUpdateBySelect($update,$selectQry);
			$updQry1 = $this->db->prepareQueryUpdate($update['table'],$update['fields'],$update['condition'],$update['joins']);
			expDebug::dPrintDBAPI(" Query for update registration status and completion status ",$updQry1,$arg);
			$this->db->callExecute($updQry1,$arg);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in update status date ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function dateValidation(){
		try{
			$jobDetail = $this->getJobDetails();
			$batchrecords = $this->getBatchRecords();
			$updQry = "update ".$jobDetail['sTable']." t1
			inner join ".$jobDetail['pTable']." t2 on t1.mapping_id = t2.mapping_id
			set t2.record_status= :val , 
			t2.custom4 = concat(ifnull(t2.custom4,''),:err )
			where
			(DATE_FORMAT(t1.".$batchrecords['reg_date'].",'%Y-%m-%d') is null OR (t1.".$batchrecords['reg_date']." NOT REGEXP '[0-9]{4}')) and t2.batch_id =:bid ";
			
			expDebug::dPrintDBAPI("Reg date is null check ",$updQry,array(':bid' => $jobDetail['batchId'],':val'=> 'IN', ':err'=> 'Reg Date is not valid format or empty.'));
			$this->db->callExecute($updQry,array(':bid' => $jobDetail['batchId'],':val'=> 'IN', ':err'=> 'Reg Date is not in valid format or empty.'));
			
			$updQry = "update ".$jobDetail['sTable']." t1
			inner join ".$jobDetail['pTable']." t2 on t1.mapping_id = t2.mapping_id
			set t2.record_status= :val ,
			t2.custom4 = concat(ifnull(t2.custom4,''),:err )
			where
			(DATE_FORMAT(t1.".$batchrecords['comp_date'].",'%Y-%m-%d') is null OR (t1.".$batchrecords['comp_date']." NOT REGEXP '[0-9]{4}')) and t1.".$batchrecords['reg_status']." IN (:cmp ,:inc , :nsw ) and t2.batch_id =:bid ";
				
			expDebug::dPrintDBAPI("Completion date is null check ",$updQry,array(':bid' => $jobDetail['batchId'],':val'=> 'IN', ':err'=> 'Comp Date is not valid format or empty.',':cmp'=>'completed',':inc'=>'incomplete',':nsw'=>'noshow'));
			$this->db->callExecute($updQry,array(':bid' => $jobDetail['batchId'],':val'=> 'IN', ':err'=> 'Comp Date is not valid format or empty.',':cmp'=>'completed',':inc'=>'incomplete',':nsw'=>'noshow'));
				
			// Registration date should be current date or past date check
			
			$updQry = "update ".$jobDetail['pTable']." t2
			set t2.record_status= :val ,
			t2.custom4 = concat(ifnull(t2.custom4,''),:err )
			where
			DATE_FORMAT(t2.reg_date,'%Y-%m-%d') > DATE_FORMAT(NOW(),'%Y-%m-%d') and t2.batch_id =:bid and t2.record_status = :r ";
				
			expDebug::dPrintDBAPI("Reg date is null check ",$updQry,array(':bid' => $jobDetail['batchId'],':r' => 'R',':val'=> 'IN', ':err'=> 'Registration date should be less than or equal to current date.'));
			$this->db->callExecute($updQry,array(':bid' => $jobDetail['batchId'],':r' => 'R',':val'=> 'IN', ':err'=> 'Registration date should be less than or equal to current date.'));
		}catch(Exception $e){
			expDebug::dPrint("ERROR in date validation ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}		
	}
	
	private function updateCancelRecords(){
		try{
			$jobDetail = $this->getJobDetails();
			$batchrecords = $this->getBatchRecords();
			$update = array();
			$update['table'] = $jobDetail['pTable'].' t2';
			$update['joins'] = array(
				'INNER JOIN '.$jobDetail['sTable'].' t1 on t1.mapping_id=t2.mapping_id',
				'INNER JOIN slt_enrollment e on e.id = (SELECT enr.id FROM slt_enrollment enr WHERE enr.user_id=t2.user_id AND enr.class_id=t2.class_id AND  date_format(t2.reg_date,\'%Y-%m-%d\') = date_format(enr.reg_date,\'%Y-%m-%d\') ORDER BY enr.id DESC LIMIT 1)'
			);
			
			$update['fields'] = array(
					't2.custom4' => "concat(ifnull(t2.custom4,''), :err )",
					't2.record_status' => ":recst "
			);
			$update['condition']=array(
					"t1.".$batchrecords['reg_status']." = :can ",
					"e.reg_status = :canst ",
					//"date_format(t2.reg_date,'%Y-%m-%d') = date_format(e.reg_date,'%Y-%m-%d')",
					"t2.batch_id = :batch_id ",
					"t2.record_status != :recst "
			);
			$arg=array(':batch_id' => $jobDetail['batchId'],
								 ':recst' => 'IN',
			 					 ':can' => 'canceled',
								 ':canst' => 'lrn_crs_reg_can',
			 					 ':err' => 'Already Canceled'
			);
			$updQry1 = $this->db->prepareQueryUpdate($update['table'],$update['fields'],$update['condition'],$update['joins']);
			expDebug::dPrintDBAPI(" Query for Update Already canceled record ",$updQry1,$arg);
			$this->db->callExecute($updQry1,$arg);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in update cancel records ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function updateDeliveryType(){
		try{
			$jobDetail = $this->getJobDetails();
			$update = array();
			$update['table'] = $jobDetail['pTable'].' tm';
			$update['fields'] = array(
					'tm.custom3'=>'b.delivery_type',
					'tm.custom4' => "CASE WHEN b.status = :dsts THEN CONCAT(ifnull(tm.custom4,''), :err )  ELSE tm.custom4 END",
					'tm.record_status' => "CASE WHEN b.status = :dsts THEN  :recst ELSE tm.record_status END"
			);
			$update['condition']=array(
					//"tm.mapping_id = x.mapid",
					"tm.batch_id = :batch_id "
			);
			$update['joins'] = array(
				"inner join slt_course_class b on tm.class_id = b.id "
			);
			$arg=array( ':dsts' => 'lrn_cls_sts_del',
									 ':err' => 'Class is not active. ',
									 ':batch_id' => $jobDetail['batchId'],
									 ':recst' => 'IN'
			);
					
			$updQry1 = $this->db->prepareQueryUpdate($update['table'],$update['fields'],$update['condition'],$update['joins']);
			expDebug::dPrintDBAPI(" Query for delivery type and active class update ",$updQry1,$arg);
			$this->db->callExecute($updQry1,$arg);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in update delivery type ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function selfLearnNoShowCheck(){
		try{
			$jobDetail = $this->getJobDetails();
			list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
					't22.custom4' => "concat(ifnull(t22.custom4,''), :err ) ",
					't22.record_status' => ":recst "
			);
			$condition=array(
					't22.batch_id = :batch_id ',
					't22.custom3 in (:wbt ,:vod )',
					't22.comp_status = :nsw '
			);
			$arg = array(
					':batch_id' => $jobDetail['batchId'],
					':recst' => 'IN',
					':err' => 'Cannot mark noshow status for WBT/VOD classes. ',
					':nsw' => 'lrn_crs_cmp_nsw',
					':wbt' => 'lrn_cls_dty_wbt',
			 		':vod' => 'lrn_cls_dty_vod'
			);
			$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition);
			expDebug::dPrintDBAPI(" noshow status sholud not allowed for WBT and VOD ",$qry1,$arg);
			$this->db->callExecute($qry1,$arg);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in no show check ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	protected function instructorConflict(){
		try{
			$jobDetail = $this->getJobDetails();
			$update = array();
			$update['table'] = $jobDetail['pTable'].' tm';
			$update['fields'] = array(
					'tm.custom4' => "CASE WHEN ins.id IS NOT NULL THEN concat(ifnull(tm.custom4,''), :err ) ELSE tm.custom4 END",
					'tm.record_status' => "CASE WHEN ins.id IS NOT NULL THEN :recst  ELSE tm.record_status END" 
			);
			$update['condition']=array(
					//"tm.mapping_id = x.mapid",
					"tm.batch_id = :batch_id "
			);
			$update['joins'] = array(
				//"INNER JOIN slt_course_class_session b on tm.class_id = b.class_id and (tm.custom3 = :vc or tm.custom3 = :itl ) and tm.user_id = b.instructor_id",
				"INNER JOIN slt_session_instructor_details ins on tm.class_id = ins.class_id and (tm.custom3 = :vc or tm.custom3 = :itl ) and tm.user_id = ins.instructor_id ",
				"INNER JOIN slt_person c on c.id = tm.user_id AND ins.instructor_id = c.id AND c.is_instructor = :ins "
			);
			$arg=array(':vc' => 'lrn_cls_dty_vcl',
								 ':itl' => 'lrn_cls_dty_ilt',
								 ':ins'	=> 'Y',
								 ':err' => 'Cannot enroll the user, since he is the instructor for the class. ',
								 ':batch_id' => $jobDetail['batchId'],
								 ':recst' => 'IN'
			);
					
			$updQry1 = $this->db->prepareQueryUpdate($update['table'],$update['fields'],$update['condition'],$update['joins']);
			expDebug::dPrintDBAPI(" Query for update of instructor conflict",$updQry1,$arg);
			$this->db->callExecute($updQry1,$arg);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in instructor conflict ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
        
        private function courseCompleteCheck(){
            try{
                    $jobDetail = $this->getJobDetails();
                    $update = array();
                    $update['table'] = $jobDetail['pTable'].' t1';
                    $update['fields'] = array(
                                    't1.custom4' => ':err' ,
                                    't1.record_status' => ':recsts' 
                    );
                    $update['condition']=array(
                                    "t1.batch_id = :batch_id ",
                                    "crs.status != :sts",
                                    "date_format(now(),'%Y-%m-%d') > date_format(crs.complete_date, '%Y-%m-%d')"
                    );
                    $update['joins'] = array(
				"LEFT JOIN slt_course_template crs ON crs.id = t1.course_id "
                    );
                    $arg = array(':sts' => 'lrn_crs_sts_del',
                                ':err' => 'Course is already completed. ',
                                ':batch_id' => $jobDetail['batchId'],
                                ':recsts' => 'IN'
                        );
                    $updQry1 = $this->db->prepareQueryUpdate($update['table'],$update['fields'],$update['condition'],$update['joins']);
                    expDebug::dPrintDBAPI(" Query for update of complete date conflict",$updQry1,$arg);
                    $this->db->callExecute($updQry1,$arg);
                
            } catch (Exception $e) {
                    expDebug::dPrint("ERROR in course complete check ".print_r($e,true),1);
                    throw new Exception($e->getMessage());
            }
        }
	
	private function sessionConflict(){
		try{
			$jobDetail = $this->getJobDetails();
			$qry = "select t2.user_id,t2.course_id,t2.class_id,per.is_instructor,t2.id,t2.mapping_id,t2.comp_status from ".$jobDetail['pTable']." t2 
							left join slt_person per on per.id = t2.user_id 
							where (t2.custom3 = :ilt or t2.custom3 = :vlc ) and t2.batch_id = :btid and record_status = :recsts ";
			$args = array(':vlc' => 'lrn_cls_dty_vcl',
								 ':ilt' => 'lrn_cls_dty_ilt',
								 ':btid' => $jobDetail['batchId'],
								':recsts' => 'R'
			);
			expDebug::dPrintDBAPI('Session Conflict for user enrolled class and assigned as a instructor ', $qry,$args);
			$this->db->callQuery($qry,$args);
			$classList = $this->db->fetchAllResults();
			
			/* include_once $_SERVER['DOCUMENT_ROOT'].'/includes/bootstrap.inc';
			include_once $_SERVER['DOCUMENT_ROOT'].'/includes/common.inc';
			include_once $_SERVER['DOCUMENT_ROOT'].'/includes/database/database.inc';
			drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL); */
			include_once($_SERVER['DOCUMENT_ROOT'].'/'.drupal_get_path('module', 'exp_sp_learning').'/exp_sp_learning.inc');
			$clsid = 0;
			$session = array();
			foreach($classList as $res){
				$user_id = $res->user_id;
				$class_id = $res->class_id;
				$ins = $res->is_intructor;
				$mapping_id = $res->mapping_id;
				if($clsid != $class_id){
					expDebug::dPrint("GET SESSION DETAILS -- >",5);
					$session = getClassSessionsDateTime($class_id);
					$clsid = $class_id;
				}
				expDebug::dPrint("SESSION DETAILS -- >".print_r($session,true),5);
				$skipStatus = array('lrn_crs_cmp_cmp', 'lrn_crs_cmp_nsw');
				if(!in_array($res->comp_status, $skipStatus)) {
					//Session conflict for Enrolled classes by the user.
					$this->enrollmentConflictforUser($user_id,$class_id,$mapping_id,$jobDetail,$session);
				}
			}
		}catch(Exception $e){
			expDebug::dPrint("ERROR in session conflict ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function preRequisiteValidation(){
		try{
                        $this->db->connect();
                        $this->db->beginTrans();
			$jobDetail = $this->getJobDetails();
                        // Pre-requisite Enrollment check
                        $qry = "SELECT  t1.user_id AS user_id from ".$jobDetail['pTable']." t1 
                                INNER JOIN slt_common_mapping cmap ON cmap.id2 = t1.course_id AND cmap.type=5 
                                AND cmap.object_type = :objTyp WHERE t1.batch_id = :btid and t1.comp_status='lrn_crs_cmp_cmp'
                                GROUP BY t1.user_id HAVING COUNT(t1.user_id)>1";
                        $args = array(
                                ':objTyp' => 'cre_sys_obt_crs',
                                ':btid' => $jobDetail['batchId'],
                                );
                        expDebug::dPrintDBAPI('Prerequisite Conflict for user enrolled class', $qry,$args);
                                $this->db->callQuery($qry,$args);
                                $userList = $this->db->fetchAllResults();
                                if(!empty($userList)){
                                    foreach($userList as $usr){
                                        $userId = $userId . $usr->user_id;
                                    }
                                    $userId = trim($userId,",");
                                    $checkUser = "t22.user_id NOT IN (".$userId.")";
                                }else{
                                    $checkUser='';
                                }
                                //start validation 
                                    list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
                                     $fields=array(
                                                    't22.custom4' => "concat(ifnull(t22.custom4,''), :err ) ",
                                                    't22.record_status' => ":recst "
                                     );
                                     if(empty($checkUser)){
                                         $condition=array(
                                                    't22.batch_id = :batch_id ',
                                                    'map.id is not null',
                                                    '(enr.comp_status != \'lrn_crs_cmp_cmp\' OR enr.comp_status is null OR enr.reg_status=\'lrn_crs_reg_can\')'
                                         );
                                     }else{
                                         $condition=array(
                                                    't22.batch_id = :batch_id ',
                                                    'map.id is not null',
                                                    '(enr.comp_status != \'lrn_crs_cmp_cmp\' OR enr.comp_status is null OR enr.reg_status=\'lrn_crs_reg_can\')',
                                                    $checkUser
                                         );
                                     }
                                     $join=array(
                                                    'inner join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id',
                                                    'inner join slt_common_mapping map on map.id1 = t22.course_id and map.object_type = \'cre_sys_obt_crs\' and map.type = 5 ',
                                                    'left join slt_enrollment enr on enr.user_id = t22.user_id and enr.course_id = map.id2 '
                                     );
                                     $arg = array(
                                                    ':batch_id' => $jobDetail['batchId'],
                                                    ':recst' => 'IN',
                                                    ':err' => 'Prerequisite course needs to be completed. '
                                     );
                                     $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
                                     expDebug::dPrintDBAPI(" Prerequisite Validation ",$qry1,$arg);
                                     $this->db->callExecute($qry1,$arg);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in prerequisite ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function userAccessCheck(){
        try{
         $jobDetail = $this->getJobDetails();
         list($selectQry, $select1,$update,$arg) = array(array(),array(),array(),array());
                              $selectQry['table'] = $jobDetail['pTable'].' t22';
                              $selectQry['fields']= array(
                                             //"t22.mapping_id as mapping_id",
                                             //"if(map.id is not null, '1','0') as access"
                                             "ifnull((GROUP_CONCAT(DISTINCT t22.user_id)),0) AS usrId"
                              );
                              $selectQry['joins'] = array(
                                             'left join slt_group_mapping map on t22.class_id  = map.entity_id and map.entity_type = \'cre_sys_obt_cls\'',
                                             'left join slt_groups grp on grp.id = map.group_id and group_type = 0',
                                             'left join slt_group_attributes grpatt on grpatt.group_id = grp.id',
                                             'left join slt_person per on per.id = t22.user_id'
                              );
                              $selectQry['alias'] = "x";

                              $selectQry['condition']=array(
                                             "t22.batch_id = :batch_id ",
                                             "grp.status = :atv ",
                              );
                              // User Group access check query formation based with activated group attributes
                              $this->userGroupAccessValidate($selectQry);
                              $select1 = array($selectQry);
                              $update['table'] = $jobDetail['pTable'].' t2';
                              $update['fields'] = array(
                                             //"t2.record_status" => "if(x.access = :val ,t2.record_status ,:rec )",
                                             //"t2.custom4" => "if(x.access = :val ,t2.custom4 ,concat(ifnull(t2.custom4,''),:err ))"
                                             "t2.record_status" => ":rec ",
                                             "t2.custom4" => "concat(ifnull(t2.custom4,''),:err )"
                              );
                              $update['condition']=array(
                                             //"t2.mapping_id = x.mapping_id"
                                             "FIND_IN_SET(t2.user_id,x.usrId) <=0",
                                             "t2.class_id IN (select mp1.entity_id from slt_group_mapping mp1 where mp1.entity_type = 'cre_sys_obt_cls' and mp1.group_id != 0 and mp1.group_type != 1 and mp1.entity_id = t2.class_id )"
                              );
                              $arg = array(':batch_id' => $jobDetail['batchId'],
                                             ':atv' => 'cre_sec_sts_atv',
                                             ':all' => 'All',
                                             ':n' => 'N',
                                             ':rec' => 'IN',
                                             ':val' => '1',
                                             ':err' => 'User doesnt have privilege to register the class.'
                              );

                              $updQry1 = $this->db->prepareUpdateBySelect($update,$select1);
                              expDebug::dPrintDBAPI(" Query for update of user access ",$updQry1,$arg);
                              $this->db->callExecute($updQry1,$arg);
			 
		}catch(Exception $e){
			expDebug::dPrint("ERROR in user access ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	protected function userGroupAccessValidate(&$select = array()) {
		try{
			$enabledValues = getEnabledAttrForVisibility();
			$enabledCount  = count($enabledValues);
			$closeBracket='';
			$lastid = 1;
			if($enabledCount > 0){
				$conditionQuery = 'if(map.id is not null,
				((';
				$andStr		 = '';
				foreach($enabledValues as $key => $value){
					$closeBracket = ($lastid == $enabledCount) ? " AND (FIND_IN_SET(per.id,IFNULL(grp.removed_users,''))= 0))" :'';
		
					switch ($value) {
						case 'job_role':
						$conditionQuery .= $andStr . "(if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where per.id= jobmap.user_id)>0,(case when (grp.job_role is not null) then ((select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id = per.id and FIND_IN_SET(ifnull(jobmap.job_role,'T'),ifnull(grp.job_role,'T'))>0)) else 1=1 end)))";
						break;
						case 'role':
							$conditionQuery .= $andStr. "(IF (grp.is_instructor !='N' AND grp.is_manager !='N', IF(per.is_instructor='Y' OR per.is_manager ='Y', 1=1, 1=0), (((IF(grp.is_instructor !='N',per.is_instructor='Y',1=1))) AND ((IF(grp.is_manager !='N',per.is_manager='Y',1=1)))) ))";
							break;
							// person table column changed for this attributes
						case 'department':
						case 'language':
							$perColumn = ($value == 'department') ? 'dept_code' : 'preferred_language';
							$conditionQuery .= $andStr. "(if(grp.".$value."=:all ,per.".$perColumn." is not null,if(grp.".$value." is null,1=1,FIND_IN_SET(ifnull(per.".$perColumn.",''),grp.".$value."))))";
							break;
							// state attributes compared with country
						case 'state':
							$perColumn = "concat(per.country,'-',per.state)";
							$conditionQuery .= $andStr. "(if(grp.".$value."=:all ,per.".$value." is not null,if(grp.".$value." is null,1=1,FIND_IN_SET(ifnull(".$perColumn.",''),grp.".$value."))))";
							break;
						case 'hire_date':
							$conditionQuery .= $andStr. "(if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT(per.hire_date,'%Y-%m-%d'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT(per.hire_date,'%Y-%m-%d'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1)  ) ";
							break;
						default:
							$conditionQuery .= $andStr. "(if(grp.".$value."=:all ,per.".$value." is not null,if(grp.".$value." is null,1=1,FIND_IN_SET(ifnull(per.".$value.",''),grp.".$value."))))";
							break;
					}
		
					$andStr		 = 'AND ';
					$lastid++;
				}
				$conditionQuery .=  $closeBracket;
				$conditionQuery .= " OR (grp.added_users IS NOT NULL AND grp.added_users != '' AND FIND_IN_SET(per.id,IFNULL(grp.added_users,''))> 0 )),(umap.id IS NULL OR umap.user_id = per.id))";
				expDebug::dPrint('$conditionQuery formed with dynmic array response' . $conditionQuery, 4);
				if(empty($select))
					return $conditionQuery;
					
				if (!is_array($select['condition'])) {
					$select['condition'] = array();
				}
				array_push($select['condition'], $conditionQuery); // add access query in update condition.
			}
		}catch(Exception $e){
			expDebug::dPrint("ERROR in user group access validate ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function userPrivilegeCheck(){
		try{
		 $jobDetail = $this->getJobDetails();
		// check user belongs to super admin group
		$conditions = array(' grp.code = :code ',' FIND_IN_SET(:uid ,IFNULL(grp.userslist,0)) > 0');
		$args = array(':code'=>'grp_sup', ':uid'=> $jobDetail['uploaded_user_id']);
			
		$query = $this->db->prepareQuerySelect('slt_groups grp',array('COUNT(1) as cnt'),$conditions,null,null);
		expDebug::dPrintDBAPI("check user belongs to super admin group ",$query,$args);
		
		// Execute query
		$this->db->callQuery($query,$args);
		// Fetch results
		$checkSupAdmin = $this->db->fetchColumn();
		expDebug::DPrint("super admin >>> ".$checkSupAdmin,5 );
		if($jobDetail['uploaded_user_id'] == 1 || $checkSupAdmin > 0){
			
		}else{	
		 
 	   	 list($selectQry, $select1,$update,$arg) = array(array(),array(),array(),array());
		 $selectQry['table'] = $jobDetail['pTable'].' temp1';
		 $selectQry['fields']= array(
		 		"temp1.user_id as user_id",
		 		"GROUP_CONCAT(grpmp.group_id) as group_id",
		 		"GROUP_CONCAT(grps.userslist) AS userlist"
		 );
		 $selectQry['joins'] = array(
		 		'LEFT JOIN slt_group_mapping grpmp ON grpmp.entity_id = temp1.user_id AND grpmp.entity_type = \'cre_usr\'',
		 		'LEFT JOIN slt_groups grps ON grps.id = grpmp.group_id',
				'LEFT JOIN slt_person per ON per.id = temp1.user_id'
		 );
		 $selectQry['alias'] = "x";
		 
		 $selectQry['condition']=array(
		 		"grpmp.entity_id IS NOT NULL and grps.status='cre_sec_sts_atv'",
				"(per.created_by!=".$jobDetail['uploaded_user_id']." AND per.updated_by!=".$jobDetail['uploaded_user_id'].")"
		 );
		  $selectQry['others'] = ' GROUP BY temp1.user_id HAVING FIND_IN_SET('.$jobDetail['uploaded_user_id'].',IFNULL(userlist,0)) <=0 ';
		 // User Group access check query formation based with activated group attributes
		
		 $select1 = array($selectQry);
		 $update['table'] = $jobDetail['pTable'].' t2';
		 $update['fields'] = array(
		 		//"t2.record_status" => "if(x.access = :val ,t2.record_status ,:rec )",
		 		//"t2.custom4" => "if(x.access = :val ,t2.custom4 ,concat(ifnull(t2.custom4,''),:err ))"
		 		"t2.record_status" => ":rec ",
		 		"t2.custom4" => "concat(ifnull(t2.custom4,''),:err )"
		 );
		 $update['condition']=array(
		 		"t2.batch_id = :batch_id ",
		 		"t2.user_id = x.user_id"
		 		//"FIND_IN_SET(t2.user_id,x.usrId) <=0"
		 );
		 $arg = array(':batch_id' => $jobDetail['batchId'],
		 		//':atv' => 'cre_sec_sts_atv',
		 		//':all' => 'All',
		 		//':n' => 'N',
		 		':batch_id' => $jobDetail['batchId'],
		 		':rec' => 'IN',
		 		//':val' => '1',
		 		':err' => 'Access to this User is restricted. '
		 );
		 
		 $updQry1 = $this->db->prepareUpdateBySelect($update,$select1);
		 expDebug::dPrintDBAPI(" Query for update of userPrivilegeCheck ",$updQry1,$arg);
		 $this->db->callExecute($updQry1,$arg);
		}
		}catch(Exception $e){
			expDebug::dPrint("ERROR in userPrivilegeCheck ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
        
        
        private function userEnrollGroupPrivilegeCheck(){
		try{
			
		$jobDetail = $this->getJobDetails();
		// check user belongs to super admin group
		$conditions = array(' grp.code = :code ',' FIND_IN_SET(:uid ,IFNULL(grp.userslist,0)) > 0');
		$args = array(':code'=>'grp_sup', ':uid'=> $jobDetail['uploaded_user_id']);
		$query = $this->db->prepareQuerySelect('slt_groups grp',array('COUNT(1) as cnt'),$conditions,null,null);
		
		expDebug::dPrintDBAPI("check user belongs to super admin group ",$query,$args);
		
		$this->db->callQuery($query,$args);
		$checkSupAdmin = $this->db->fetchColumn();
		expDebug::DPrint("super admin >>> ".$checkSupAdmin,5 );
		if($jobDetail['uploaded_user_id'] == 1 || $checkSupAdmin > 0){
				// do nothing here
		}else{
			$batch_table=$jobDetail['pTable'];
				
				expDebug::dPrint('$batch_table='.$batch_table,5);
				$access_qry = "SELECT up_per.id AS user_id, crs.id AS class_id, (CASE when group_map.id IS NOT NULL AND catacs.status='cre_sec_sts_atv' AND (crs.created_by !=".$jobDetail['uploaded_user_id']." OR crs.updated_by != ".$jobDetail['uploaded_user_id'].")
                                                                        THEN (SELECT IF(gm.id IS NULL,1,ifnull(max(p.priv_edit),0))
                                                                        FROM slt_group_privilege p
                                                                        left join slt_groups g on p.group_id=g.id
                                                                        left join slt_group_mapping gm on gm.group_id = g.id and gm.entity_type='cre_sys_obt_cls' and gm.group_type=1
                                                                        where FIND_IN_SET(".$jobDetail['uploaded_user_id'].",g.userslist) > 0 AND g.status = 'cre_sec_sts_atv'
                                                                        AND if(g.is_admin=1,p.page_code = 'cre_sys_obt_crs',p.page_code = 'cre_sys_obt_crs') AND gm.entity_id=crs.id)
                                                                        ELSE CASE WHEN crs.created_by = ".$jobDetail['uploaded_user_id']." OR crs.updated_by = ".$jobDetail['uploaded_user_id']." OR
                                                                        ((SELECT ifnull(max(p.priv_edit),0)
                                                                        FROM slt_group_privilege p
                                                                        left join slt_groups g on p.group_id=g.id
                                                                        where FIND_IN_SET(".$jobDetail['uploaded_user_id'].",g.userslist) > 0 AND g.status = 'cre_sec_sts_atv'
                                                                        AND if(g.is_admin=1,p.page_code = 'cre_sys_obt_crs',p.page_code = 'cre_sys_obt_crs')) > 0)
                                                                        THEN 1
                                                                        ELSE 0 END
                                                                        END ) AS sumEdit, (CASE
                                                                        when group_map.id IS NOT NULL AND catacs.status='cre_sec_sts_atv' AND (crs.created_by != ".$jobDetail['uploaded_user_id'].")
                                                                        THEN (SELECT IF(gm.id IS NULL,1,ifnull(max(p.priv_delete),0))
                                                                        FROM slt_group_privilege p
                                                                        left join slt_groups g on p.group_id=g.id
                                                                        left join slt_group_mapping gm on gm.group_id = g.id and gm.entity_type='cre_sys_obt_cls' and gm.group_type=1
                                                                        where FIND_IN_SET(".$jobDetail['uploaded_user_id'].",g.userslist) > 0 AND g.status = 'cre_sec_sts_atv'
                                                                        AND if(g.is_admin=1,p.page_code = 'cre_sys_obt_crs',p.page_code = 'cre_sys_obt_crs') AND gm.entity_id=crs.id)
                                                                        ELSE CASE WHEN crs.created_by = ".$jobDetail['uploaded_user_id']." OR ((SELECT ifnull(max(p.priv_delete),0)
                                                                        FROM slt_group_privilege p
                                                                        left join slt_groups g on p.group_id=g.id
                                                                        where FIND_IN_SET(".$jobDetail['uploaded_user_id'].",g.userslist) > 0 AND g.status = 'cre_sec_sts_atv'
                                                                        AND if(g.is_admin=1,p.page_code = 'cre_sys_obt_crs',p.page_code = 'cre_sys_obt_crs')) > 0)
                                                                        THEN 1
                                                                        ELSE 0 END
                                                                        END ) AS sumDelete
                                        FROM 
                                        slt_course_class crs FORCE INDEX (sli_ccls_code)
                                        INNER JOIN ".$batch_table." bt on bt.class_id=crs.id and bt.course_id=crs.course_id
                                        LEFT OUTER JOIN slt_person up_per on up_per.id=".$jobDetail['uploaded_user_id']."
                                        LEFT JOIN slt_person_jobrole_mapping lpjm ON lpjm.user_id = up_per.id
                                        LEFT OUTER JOIN slt_profile_list_items plistatus ON crs.status = plistatus.code AND plistatus.lang_code = 'cre_sys_lng_eng'
                                        LEFT OUTER JOIN slt_profile_list_items plilang ON crs.lang_code = plilang.code AND plilang.lang_code = 'cre_sys_lng_eng'
                                        LEFT OUTER JOIN slt_profile_list_items plidelivery ON crs.delivery_type = plidelivery.code AND plidelivery.lang_code = 'cre_sys_lng_eng'
                                        LEFT OUTER JOIN slt_profile_list_items plicurr ON crs.currency_type = plicurr.attr1 AND plicurr.lang_code = 'cre_sys_lng_eng'
                                        LEFT OUTER JOIN slt_tag_entity te ON ((crs.id=te.entity_id and te.entity_type='Class') OR (crs.course_id=te.entity_id and te.entity_type='Course'))
                                        LEFT OUTER JOIN slt_tagdefn td ON te.tagid= td.id
                                        LEFT OUTER JOIN slt_group_mapping group_map ON ((group_map.parent_id=concat(crs.course_id, '-', IFNULL(crs.id,0)) AND group_map.parent_type = 'learning')) and group_map.group_type = 1 AND 'cre_sec_sts_atv' = (SELECT status FROM slt_groups where id = group_map.group_id)
                                        LEFT OUTER JOIN slt_groups catacs ON catacs.id=group_map.group_id and (catacs.is_admin =1)
                                        LEFT OUTER JOIN slt_group_attributes grpatt ON catacs.id=grpatt.group_id
                                        LEFT OUTER JOIN slt_group_privilege priv ON priv.group_id = catacs.id and priv.page_code= 'cre_sys_obt_crs'
                                        LEFT OUTER JOIN slt_course_class_session ses ON crs.id=ses.class_id AND ses.id=(SELECT id FROM  slt_course_class_session WHERE class_id=ses.class_id ORDER BY start_date LIMIT 1)
                                        LEFT OUTER JOIN slt_session_instructor_details ses_ins ON ses_ins.session_id = ses.id
                                        WHERE  (if(crs.created_by =".$jobDetail['uploaded_user_id']." OR crs.updated_by = ".$jobDetail['uploaded_user_id']." OR FIND_IN_SET(".$jobDetail['uploaded_user_id'].",catacs.added_users)>0,1=1,(catacs.removed_users IS NULL OR FIND_IN_SET(".$jobDetail['uploaded_user_id'].",catacs.removed_users) <= 0) AND (if(catacs.code = 'grp_adm',FIND_IN_SET(".$jobDetail['uploaded_user_id'].",catacs.added_users)>0,1=1)) 

                                        AND (if(catacs.org_id='All',up_per.org_id is not null,catacs.org_id is null OR FIND_IN_SET(ifnull(up_per.org_id,''),catacs.org_id)>0))
                                                                    AND (if(catacs.user_type='All',up_per.user_type is not null,catacs.user_type is null OR FIND_IN_SET(ifnull(up_per.user_type,''),catacs.user_type)>0 ))
                                                                    AND (if(catacs.employment_type='All',up_per.employment_type is not null,catacs.employment_type is null OR FIND_IN_SET(ifnull(up_per.employment_type,''),catacs.employment_type)>0))
                                                                    AND (if(catacs.country='All',up_per.country is not null,catacs.country is null OR FIND_IN_SET(ifnull(up_per.country,''),catacs.country)>0))
                                                                    AND (if(catacs.state='All',up_per.state is not null,catacs.state is null OR FIND_IN_SET(ifnull(concat(up_per.country,'-',up_per.state),''),catacs.state)>0))
                                                                    AND (if(catacs.department='All',up_per.dept_code is not null,catacs.department is null OR FIND_IN_SET(ifnull(up_per.dept_code,''),catacs.department)>0))
                                                                    AND (if(catacs.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id = up_per.id)>0,catacs.job_role is null OR FIND_IN_SET(ifnull(lpjm.job_role,''),ifnull(catacs.job_role,''))>0))
                                                                    AND (if(catacs.language='All',up_per.preferred_language is not null,catacs.language is null OR FIND_IN_SET(ifnull(up_per.preferred_language,''),catacs.language)>0))
                                                                    AND (CASE WHEN (catacs.is_manager='Y' AND catacs.is_instructor='Y')
                                                    THEN
                                                    (ifnull(up_per.is_manager,'N') = catacs.is_manager or ifnull(up_per.is_instructor,'N') = catacs.is_instructor)
                                                    WHEN (catacs.is_manager='Y' AND catacs.is_instructor='N')
                                                    THEN
                                                    (ifnull(up_per.is_manager,'N') = catacs.is_manager)
                                                    WHEN (catacs.is_manager='N' AND catacs.is_instructor='Y')
                                                    THEN
                                                    (ifnull(up_per.is_instructor,'N') = catacs.is_instructor)
                                                    ELSE
                                                    1=1
                                                    END) AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,UNIX_TIMESTAMP(grpatt.on_or_after_start_date) <= UNIX_TIMESTAMP(up_per.hire_date),1=0) OR 
                                                                if(grpatt.on_or_before_start_date is not null, UNIX_TIMESTAMP(grpatt.on_or_before_start_date) >= UNIX_TIMESTAMP(up_per.hire_date), 1=0) OR
                                                                if(grpatt.between_start_date is not null ,UNIX_TIMESTAMP(up_per.hire_date) between UNIX_TIMESTAMP(grpatt.between_start_date) AND UNIX_TIMESTAMP(grpatt.between_end_date), 1=0),1=1)  ) AND( (group_map.group_type = 1) OR (group_map.group_id IS NULL) )AND (if((catacs.is_admin = 1),FIND_IN_SET(".$jobDetail['uploaded_user_id'].",catacs.userslist)>0,1=1)))) AND (crs.status != 'lrn_cls_sts_del') 
                                        GROUP BY crs.id"; 
							
				$this->db->callQuery($access_qry);
                                $grouppriv = $this->db->fetchAllResults();
                                
                                foreach($grouppriv as $groups){
                                        $edit = $groups->sumEdit;
                                        if($edit){
                                            $sql = "Update ".$batch_table." set custom1 = 'Y' where class_id = :clsId";
                                            $arg = array(':clsId' => $groups->class_id);
                                            $this->db->callExecute($sql,$arg);
                                        }
                                }
				
                                //update the Error for user enrollment not having privilege to enroll to the class
                                $updSql = "Update ".$batch_table." set record_status = :recsts ,custom4 = :err_edit where custom1 is null";
				  $args = array(
                                            ':err_edit'=>'Class access denied.',
                                            ':recsts'=> 'IN'
                                        );
				expDebug::dPrintDBAPI("Get the updSql query-->>>",$updSql,$arg);
				 $this->db->callExecute($updSql,$args);  
				 
		}
		}catch(Exception $e){
			expDebug::dPrint("ERROR in userEnrollGroupPrivilegeCheck ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
}
	
	private function hireDateValidation(){
		try{
			$jobDetail = $this->getJobDetails();
			list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			 $fields=array(
				 	't22.custom4' => "concat(ifnull(t22.custom4,''), :err ) ",
			 		't22.record_status' => ":recst "
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id ',
			 		'(IF(template.id IS NOT NULL AND per.hire_date IS NOT NULL,(DATE_ADD(DATE_FORMAT(per.hire_date, "%Y-%m-%d"), INTERVAL template.complete_days DAY) < CURDATE()),1=0))  '
			 );
			 $join=array(
			 		'INNER JOIN '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id',
			 		'INNER JOIN slt_course_template template ON template.id = t22.course_id AND template.compliance_completed = \'hire_days\' ',
			 		'INNER JOIN slt_person per ON per.id = t22.user_id'
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':recst' => 'IN',
			 		':err' => 'Hire date for User is not matching with course completion hire date. '
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI(" hireDateValidation ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in hireDateValidation check ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function completionCheck(){
		try{
			$jobDetail = $this->getJobDetails();
			$batchrecords = $this->getBatchRecords();
			list($fields, $args,$join,$condition) = array(array(),array(),array(),array());
				 	
			 $fields = array(
			 		"t22.custom4" => "concat(ifnull(t22.custom4,''), :msg )",
			 		"t22.record_status" => ":recst "
			 );
			 $args = array(
			 		':batch' => $jobDetail['batchId'],
			 		':stat1' => 'enrolled',
			 		':stat2' => 'completed',
			 		':stat3' => 'canceled',
			 		':stat4' => 'noshow',
			 		':stat5' => 'incomplete',
			 		':recst' => 'IN',
			 		':msg' => 'Completion_status can be only enrolled/completed/incomplete/canceled/noshow . '
			 );
			 $join = array(
			 		" inner join ".$jobDetail['sTable']." t1 on t22.mapping_id = t1.mapping_id"
			 );
			 $condition = array(
			 		't22.batch_id = :batch ',
			 		't1.'.$batchrecords['reg_status'].' not in ( :stat1 , :stat2 ,:stat3 ,:stat4 ,:stat5 )'
			 );
			 $qry = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI("Completion status check ",$qry,$args);
			 $this->db->callExecute($qry,$args);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in completion check ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function complianceUpdate(){
		try{
			$jobDetail = $this->getJobDetails();
			list($fields,$join) = array(array(),array());
			$fields=array(
				't22.is_compliance' => 'crs.is_compliance'
			);
			$join=array(
				'inner join slt_course_template crs on crs.id = t22.course_id'
			);
			$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,array(),$join);
			expDebug::dPrintDBAPI(" Update is_compliance column ",$qry1);
			$this->db->callExecute($qry1);
			
			
			$fields=array(
					//'t22.mandatory' => 'if(t22.is_compliance!=1,if(map.mro = \'cre_sys_inv_man\',\'Y\',if(t22.mandatory=\'Y\',\'Y\',\'\'))'
					't22.mandatory' => 'null'
			);
			$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,array('t22.is_compliance=1'));
			expDebug::dPrintDBAPI(" Update mandatory column ",$qry1);
			$this->db->callExecute($qry1);
			
			
		}catch(Exception $e){
			expDebug::dPrint("ERROR in compliance update ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	//code check
	private function mandatoryCodeCheck(){
		try{
			$jobDetail = $this->getJobDetails();
			list($fields, $arg,$condition) = array(array(),array(),array());
			$fields=array(
					't22.custom4' => "concat(ifnull(t22.custom4,''), :err ) ",
					't22.record_status' => ":recst "
			);
			$condition=array(
					't22.batch_id = :batch_id ',
					't22.mandatory NOT IN (\'Y\',\'N\',\'\')'
			);	
			$arg = array(
					':batch_id' => $jobDetail['batchId'],
					':recst' => 'IN',
					':err' => 'Invalid value for mandatory column. Accepted values are \'Y\',\'N\' and empty'
			);
			$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition);
			expDebug::dPrintDBAPI(" mandatoryCodeCheck ",$qry1,$arg);
			$this->db->callExecute($qry1,$arg);			
		}catch(Exception $e){
			expDebug::dPrint("ERROR in mandatory update ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	private function updateMandatory(){
		try{
			$jobDetail = $this->getJobDetails();
			list($fields,$join) = array(array(),array()); 
			 $fields=array(
			 		't22.mandatory' => 'if((usrmap.mro =\'cre_sys_inv_man\' OR map.mro = \'cre_sys_inv_man\'),\'Y\',if(t22.mandatory=\'Y\',\'Y\',null))'
			 );
			 $join=array(
			 		'left join slt_group_mapping map on t22.class_id  = map.entity_id and map.entity_type = \'cre_sys_obt_cls\'',
			 		'left join slt_user_access_mapping usrmap on t22.class_id  = usrmap.entity_id and usrmap.entity_type = \'cre_sys_obt_cls\' and usrmap.user_id = t22.user_id'
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,array('t22.is_compliance is null'),$join);
			 expDebug::dPrintDBAPI(" Update Mandatory column ",$qry1);
			 $this->db->callExecute($qry1);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in mandatory update ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function checkAlreadyRegistered(){
		try{ 
			$jobDetail = $this->getJobDetails();
			$update = array();
			$update['table'] = $jobDetail['pTable'].' tm';
			$update['fields'] = array(
			 		'tm.operation'=>'If(enr.class_id IS NOT NULL and tm.custom2 IS NULL, :upd , :ins )',
			 		'tm.primary_id' => "If(enr.class_id IS NOT NULL and tm.custom2 IS NULL, enr.id , tm.primary_id)",
			 		'tm.record_status' => 'If(tm.custom2 = \'cre_sys_brl_dtc\', :recst , tm.record_status)',
			 		'tm.custom4' => 'If(tm.custom2 = \'cre_sys_brl_dtc\', concat(ifnull(tm.custom4,\'\'), :err ) , tm.custom4)'
			);
			$update['condition']=array(
			 	//	"tm.mapping_id = x.mapid",
			 		"tm.batch_id = :batch_id ",
			 		"tm.record_status != :recst "
			);
			$update['joins'] = array(
			 		"inner join slt_enrollment enr on enr.user_id = tm.user_id AND enr.course_id=tm.course_id 
			 							AND enr.class_id = tm.class_id and enr.reg_status in (:rgsts ,:wtl ) 
			 							and ifnull(enr.comp_status,0) not in ( :incsts , :nswsts ) and (DATE_FORMAT(tm.reg_date,'%Y-%m-%d') = DATE_FORMAT(enr.reg_date,'%Y-%m-%d'))",
			 		"inner join slt_course_class cls ON cls.course_id = enr.course_id AND cls.id= enr.class_id ",
			 		//	"left join slt_business_rule_mapping busrule ON busrule.entity_id = cls.id AND busrule.entity_type = :ent "
			);
			$arg=array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':recst' => 'IN',
			 		':err' => 'Class is dedicated.',
			 		':rgsts' => 'lrn_crs_reg_cnf',
			 		':wtl' => 'lrn_crs_reg_wtl',
			 		//':ent' => 'cre_sys_obt_cls',
			 		':upd' => 'update',
			 		':ins' => 'insert',
			 		':incsts' => 'lrn_crs_cmp_inc',
			 		':nswsts' => 'lrn_crs_cmp_nsw'
			);
			 	
			$updQry1 = $this->db->prepareQueryUpdate($update['table'],$update['fields'],$update['condition'],$update['joins']);
			expDebug::dPrintDBAPI(" Query for update the primary id and operation column ",$updQry1,$arg);
			$this->db->callExecute($updQry1,$arg);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in already register ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function updateRecurringReg(){
		try{
			$jobDetail = $this->getJobDetails();
			$select1 = array();
			$selectQry = array();
			$select1['table'] = $jobDetail['pTable'].' t2';
			$select1['fields']= array(
			 		'enr.class_id as class_id',
			 		//	"t2.custom2 as brleid",
			 		't2.mapping_id as mapid',
			 		"enr.id as enrid"
			);
			$select1['joins'] = array(
			 		"inner join slt_enrollment enr on enr.user_id = t2.user_id 
			 								AND enr.course_id=t2.course_id 
			 								AND enr.class_id = t2.class_id 
			 								AND t2.reg_date = enr.reg_date 
			 								AND enr.reg_status= :rgsts 
			 								AND enr.comp_status != :cmpsts ",
			 		"inner join slt_course_class cls ON cls.course_id = enr.course_id 
			 								AND cls.id= enr.class_id 
			 								AND (cls.delivery_type = :wbt OR cls.delivery_type = :vod ) "
			);
			 
			$select1['condition']=array(
			 		//	"enr.master_enrollment_id IS NULL",
			 		"t2.batch_id = :batch_id ",
			 		"t2.record_status != :recst ",
			 		"t2.custom2 = :rra ",
					"enr.comp_status != t2.comp_status"
			);
			 
			$select1['others'] = " group by enr.user_id,enr.course_id,enr.class_id,enr.reg_date ";
			//$select1['alias'] = "x";
			$arg=array(
					':vod' => 'lrn_cls_dty_vod',
			 		':wbt' => 'lrn_cls_dty_wbt',
			 		':batch_id' => $jobDetail['batchId'],
			 		':recst' => 'IN',
			 		':rra' => 'cre_sys_brl_rra',
			 		':rgsts' => 'lrn_crs_reg_cnf',
					':cmpsts' => 'lrn_crs_cmp_cmp'
			);
			$selectQry = $this->db->prepareQuerySelect($select1['table'],$select1['fields'],$select1['condition'],$select1['joins'],$select1['others']);
			expDebug::dPrintDBAPI(" GET Recurring registration records ",$selectQry,$arg);
			$this->db->callQuery($selectQry,$arg);
			$recurr = $this->db->fetchAllResults();
			
			foreach($recurr as $recs){
				$update = array();
				$update['table'] = $jobDetail['pTable'].' tm';
				$update['fields'] = array(
				 		'tm.operation'=>':upd ',
				 		'tm.primary_id' => $recs->enrid
				);
				$update['condition']=array(
				 		"tm.mapping_id = ".$recs->mapid,
				 		"tm.batch_id = :batch_id "
				);
				$arg = array(':upd' => 'update', ':batch_id' => $jobDetail['batchId']);
				$updQry1 = $this->db->prepareQueryUpdate($update['table'],$update['fields'],$update['condition']);
				expDebug::dPrintDBAPI(" Query Update for Recurring Registration ",$updQry1,$arg);
				$this->db->callExecute($updQry1,$arg);
			}
		}catch(Exception $e){
			expDebug::dPrint("ERROR in recurring reg ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function updateNofifyType(){
		try{
			$jobDetail = $this->getJobDetails();
			list($fields, $args,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
			 		"t22.notifyuser" => "CASE WHEN t22.reg_status = :reg and t22.comp_status = :cmp and t22.operation = :ins and t22.is_compliance != :iscmp THEN :regcmp 
													 WHEN t22.reg_status = :cnl and t22.operation = :ins and t22.is_compliance != :iscmp THEN :regcnl 
													 WHEN t22.reg_status = :reg and t22.comp_status = :incsts and t22.operation = :ins and t22.is_compliance != :iscmp THEN :regnot
													 WHEN t22.is_compliance = :iscmp and t22.reg_status = :reg and t22.comp_status = :cmp and t22.operation = :ins THEN :comprc
													 WHEN t22.is_compliance = :iscmp and t22.reg_status = :cnl and t22.operation = :ins THEN :comprcn 
													 WHEN t22.is_compliance = :iscmp and t22.reg_status = :inc and t22.operation = :ins THEN :comprinc
			 										 WHEN t22.is_compliance = :iscmp and t22.reg_status = :reg and t22.comp_status = :incsts and t22.operation = :ins THEN :comprinc
			 										 WHEN t22.is_compliance = :iscmp and t22.reg_status = :reg and t22.comp_status = :incsts and t22.operation = :upd THEN :compincomp  
													 WHEN t22.is_compliance = :iscmp and t22.comp_status = :enr and t22.operation = :ins and t22.custom3 = :ilt THEN :compregilt 
													 WHEN t22.is_compliance = :iscmp and t22.comp_status = :enr and t22.operation = :ins and t22.custom3 = :vlc THEN :compregvlc 
													 WHEN t22.is_compliance = :iscmp and t22.comp_status = :enr and t22.operation = :ins and (t22.custom3 = :wbt or t22.custom3 = :vod ) THEN :compregwbt 
													 WHEN t22.is_compliance = :iscmp and t22.comp_status = :inc and t22.operation = :upd THEN :compincomp 
													 WHEN t22.comp_status is not null and t22.comp_status = :enr THEN :regnot
													 WHEN t22.comp_status = :cmp THEN :cmpnot
													 WHEN t22.reg_status = :cnl and (t22.custom3 = :ilt or t22.custom3 = :vlc ) THEN :cnlnot ELSE :cnlwbt END",
			);
			$condition=array(
			 		't22.batch_id = :batch_id ',
			 		't22.record_status = :r '
			);
			 
			$arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':reg' => 'lrn_crs_reg_cnf',
			 		':cmp' => 'lrn_crs_cmp_cmp',
			 		':cnl' => 'lrn_crs_reg_can',
			 		':inc' => 'lrn_crs_cmp_nsw',
			 		':enr' => 'lrn_crs_cmp_enr',
			 		':ilt' => 'lrn_cls_dty_ilt',
			 		':vlc' => 'lrn_cls_dty_vcl',
			 		':wbt' => 'lrn_cls_dty_wbt',
			 		':vod' => 'lrn_cls_dty_vod',
			 		':incsts' => 'lrn_crs_cmp_inc',
			 		':regnot' => 'Register',
			 		':cnlnot' => 'Cancel',
			 		':cnlwbt' => 'WBTCancel',
			 		':cmpnot' => 'Completed',
			 		':compregilt' => 'RegisterMandatoryCompliance',
			 		':compregvlc' => 'RegisterVCCompliance',
			 		':compregwbt' => 'RegisterWBTCompliance',
			 		':compincomp' => 'ComplianceClassIncomplete',
			 		':regcmp' => 'registeredcomplete',//
			 		':regcnl' => 'registeredcancel',
			 		':comprc' => 'complianceregisteredcomplete',
			 		':comprcn' => 'complianceregisteredcancel',
			 		':comprinc' => 'complianceregisteredincomplete',
			 		':ins' => 'insert',
			 		':upd' => 'update',
			    ':r' => 'R',
			 		':iscmp' => 1
			);
			$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition);
			expDebug::dPrintDBAPI(" Update temp2 table qry1 for notifyuser",$qry1,$arg);
			$this->db->callExecute($qry1,$arg);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in notify type update ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function alterTables(){
		try{
			$jobDetail = $this->getJobDetails();
			//alter column to add class_id in temp1 table
			$alterqry1 =	" ALTER TABLE ".$jobDetail['sTable']."
			ADD column class_id int(11) default null, ADD column course_id int(11) default null";
			expDebug::dPrintDBAPI('Alter temp table 1 query ',$alterqry1);
			$this->db->callExecute($alterqry1);
			
			$atrQry1 = ' ALTER TABLE '.$jobDetail['pTable']." 
									 ADD column user_email varchar(255) default null, 
									 ADD column drup_uid int(10) default null, 
									 ADD column manager_email varchar(255) default null, 
									 ADD column mgr_uid int(10) default null, 
									 ADD column mgr_drup_uid int(10) default null, 
									 ADD column menrid int(11) default null, 
									 ADD column enr_sts varchar(25) default null, 
									 ADD column menr_sts varchar(25) default null, 
									 ADD KEY map (mapping_id), ADD KEY cus2 (custom2(255)) ";
			$this->db->callExecute($atrQry1);
			
			/*$atrQry2 = ' ALTER TABLE '.$jobDetail['pTable']." ADD KEY cus2 (custom2(255)) ";
			$this->db->callExecute($atrQry2);*/
		}catch(Exception $e){
			expDebug::dPrint("ERROR in alter tables ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function updateTemp1ClassID(){
		try{
			$jobDetail = $this->getJobDetails();
			//Update class id in temp1 
			list($fields,$join) = array(array(),array());
			$fields=array(
					't1.class_id' => 'cls.id',
					't1.course_id' => 'cls.course_id'
			);
			$join=array(
					'inner join slt_course_class cls on t1.class_code = cls.code and cls.status NOT IN (\'lrn_cls_sts_del\',\'lrn_cls_sts_can\') and cls.lang_code IN (select code from slt_profile_list_items where name = t1.language and is_active = \'Y\' and code like \'cre_sys_lng_%\')'
			);
			$qry1 = $this->db->prepareQueryUpdate($jobDetail['sTable']." t1",$fields,array(),$join);
			expDebug::dPrintDBAPI(" Update class id in temp1  ",$qry1);
			$this->db->callExecute($qry1);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in update class id in temp 1 ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
        
        private function validateClassCode(){
		try{
			$jobDetail = $this->getJobDetails();
			$custom_details = dataLoadBase::getCustomValues($jobDetail['jobId']);
			//expDebug::dPrint('Uploaded Class code '. print_r($custom_details,true),5);
			//expDebug::dPrint('Class code '. $custom_details[0]->custom0,5);
			$uploaded_cls_id =  $custom_details[0]->custom0;
			
			list($fields,$join) = array(array(),array());
			$fields=array(
					't1.record_status' => ':record_status ',
					//'t2.record_status' => ':record_status ',
					't1.remarks'			=> 'CONCAT(IFNULL(t1.remarks, ""), :err )'
			);
			/*$join=array(
					"inner join ".$jobDetail['pTable']." t2 ON t1.mapping_id = t2.mapping_id "
			);*/
			$params =  array(':record_status' => 'IN',':err' => 'Uploaded Class code not matching with Feed file class code. ',':cls_id' => $uploaded_cls_id);
			//$qry1 = $this->db->prepareQueryUpdate($jobDetail['sTable']." t1",$fields,array('t1.class_id != :cls_id '),$join);
			$qry1 = $this->db->prepareQueryUpdate($jobDetail['sTable']." t1",$fields,array('t1.class_id != :cls_id '));
			expDebug::dPrintDBAPI("Validation for irrelevant class code  ",$qry1,$params);
			$this->db->callExecute($qry1,$params);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in class code validate ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
        
        private function updateInvalidLang(){
		try{
			$jobDetail = $this->getJobDetails();
			list($fields,$join) = array(array(),array());
			$fields=array(
					't1.record_status' => ':record_status ',
					't1.remarks' => 'CONCAT(IFNULL(t1.remarks, ""), :err )'
			);
			$join=array(
					'inner join slt_profile_list_items spli on t1.language = spli.name and spli.is_active = :is_active and spli.code like :code ',
                                        'left join slt_course_class cls on t1.class_code = cls.code and cls.status not in (:del ,:can )'
			);
			$params =  array(':record_status' => 'IN',
                                         ':err' => 'Either the class code or language entered is incorrect.',
                                         ':del' => 'lrn_cls_sts_del',
                                         ':can' => 'lrn_cls_sts_can',
                                         ':is_active' => 'Y',
                                         ':code' => 'cre_sys_lng_%');
			// $qry1 = $this->db->prepareQueryUpdate($jobDetail['sTable']." t1",$fields,array('t1.class_id is null'),$join);
			$qry1 = $this->db->prepareQueryUpdate($jobDetail['sTable']." t1",$fields,array('t1.class_id is null'),$join);
			expDebug::dPrintDBAPI("update temp table 1 query  ",$qry1,$params);
			$this->db->callExecute($qry1,$params);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in invalid language update ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function updateInvalidClass(){
		try{
			$jobDetail = $this->getJobDetails();
			list($fields,$join) = array(array(),array());
			$fields=array(
					't1.record_status' => ':record_status ',
					//'t2.record_status' => ':record_status ',
					't1.remarks'			=> 'CONCAT(IFNULL(t1.remarks, ""), :err )'
			);
			/*$join=array(
					"inner join ".$jobDetail['pTable']." t2 ON t1.mapping_id = t2.mapping_id "
			);*/
			$params =  array(':record_status' => 'IN',':err' => 'Invalid Class Code. ');
			// $qry1 = $this->db->prepareQueryUpdate($jobDetail['sTable']." t1",$fields,array('t1.class_id is null'),$join);
			$qry1 = $this->db->prepareQueryUpdate($jobDetail['sTable']." t1",$fields,array('t1.class_id is null and record_status!=\'IN\''));
			expDebug::dPrintDBAPI("update temp table 1 query  ",$qry1,$params);
			$this->db->callExecute($qry1,$params);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in invalid class update ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function updateTemp2ClassID(){
		try{
			$jobDetail = $this->getJobDetails();
			$batchrecords = $this->getBatchRecords();
			list($fields,$join) = array(array(),array());
			$fields=array(
					't2.class_id' => 't1.class_id',
					't2.course_id' => 't1.course_id'
			);
			$join=array(
					"inner join ".$jobDetail['sTable']." t1 ON t1.mapping_id = t2.mapping_id ",
					//"left join slt_course_class cls on t2.class_id = cls.id and cls.status != 'lrn_cls_sts_del' ",
					//"left join slt_course_template crs on t1.".$batchrecords['course_id']." = crs.code and cls.course_id = crs.id and crs.status != 'lrn_crs_sts_del'"
			);
			$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t2",$fields,array(),$join);
			expDebug::dPrintDBAPI(" Update class id in temp2 table ",$qry1);
			$this->db->callExecute($qry1);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in update class id in temp2 ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function updateInvalidCourse(){
		try{
			$jobDetail = $this->getJobDetails();
			list($fields,$join,$condition) = array(array(),array(),array());
			$fields=array(
					't1.record_status' => ':record_status ',
					//'t2.record_status' => ':record_status ',
					't1.remarks'			=> 'CONCAT(IFNULL(t1.remarks, ""), :err )'
			);
			$join=array(
					"inner join ".$jobDetail['pTable']." t2 ON t1.mapping_id = t2.mapping_id ",
                                        "inner join slt_course_template crs ON t2.course_id = crs.id "
			);
			$condition = array('t1.course_code != crs.code');
			$params =  array(':record_status' => 'IN',':err' => 'Invalid Course Code. ');
			$qry1 = $this->db->prepareQueryUpdate($jobDetail['sTable']." t1",$fields,$condition,$join);
			expDebug::dPrintDBAPI("Update Invalid course in temp1 and temp2   ",$qry1,$params);
			$this->db->callExecute($qry1,$params);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in update invalid course ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function checkBusinessRule(){
		try{
			$jobDetail = $this->getJobDetails();
			$qry = "SELECT DISTINCT class_id FROM ".$jobDetail['pTable'];
			expDebug::dPrintDBAPI("Distinct class id ",$qry);
			$this->db->callQuery($qry);
			$classids = $this->db->fetchAllResults();
			expDebug::dPrint("Distinct class ids -- ".print_r($classids,true),4);
			$allClsIds = array();
			foreach($classids as $clid){
				$allClsIds[] = $clid->class_id;
			}
                        //$isClassIdExist = array_filter($allClsIds);
			expDebug::dPrint(" allClsIds class ids -- ".print_r($allClsIds,true),4);
                        if(!empty($allClsIds)){
			$qry = "SELECT entity_id, business_rule_code FROM slt_business_rule_mapping 
							WHERE entity_type = 'cre_sys_obt_cls' AND entity_id IN (".implode(',',$allClsIds).')';
			expDebug::dPrintDBAPI("Business rule qry ",$qry);
			$this->db->callQuery($qry);
			$brule = $this->db->fetchAllResults();
			expDebug::dPrint("Business rule  rst -- ".print_r($classids,true),3);
			
			foreach($brule as $rec){
				list($fields,$join) = array(array(),array());
				
				if($rec->business_rule_code == 'cre_sys_brl_dtc'){
					$fields = array(
						"t1.record_status" => ":record_status ",
						"t1.remarks"  => "concat(ifnull(t1.remarks,''), :err )"
					);
					$cond = array(
						't1.class_id = :clsid'
					);
					$params =  array(
						':record_status' => 'IN',
						':err' => 'Dedicated class for Training Program. ',
						':clsid' => $rec->entity_id
					);
					$qry1 = $this->db->prepareQueryUpdate($jobDetail['sTable']." t1",$fields,$cond);
					expDebug::dPrintDBAPI(" Update buisness rule temp1 ",$qry1,$params);
					$this->db->callExecute($qry1,$params);
					
					// Update record as invalid in temp2
					$fields=array(
						't2.record_status' => ":record_status"
					);
					$cond = array(
						't2.class_id = :clsid'
					);
					$params =  array(
						':record_status' => 'IN',
						':clsid' => $rec->entity_id
					);
					$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t2",$fields,$cond);
					expDebug::dPrintDBAPI(" Update buisness rule temp2 ",$qry1,$params);
					$this->db->callExecute($qry1,$params);
				}else if($rec->business_rule_code == 'cre_sys_brl_rra'){
					$fields=array(
						't2.custom2' => ":brule"
					);
					$cond = array(
						't2.class_id = :clsid'
					);
					$params =  array(
						':brule' => $rec->business_rule_code,
						':clsid' => $rec->entity_id
					);
					$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t2",$fields,$cond);
					expDebug::dPrintDBAPI(" Update buisness rule ",$qry1,$params);
					$this->db->callExecute($qry1,$params);
				}
				
			}
                    }
		}catch(Exception $e){
			expDebug::dPrint("ERROR in business rule ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function duplicateCheck(){
		try{
			$jobDetail = $this->getJobDetails();
			$batchrecords = $this->getBatchRecords();
			
			$fields = array(
				't11.class_id as class_id',
				$batchrecords['user_id'].' as user_name'
			);
			$joins = array(
				'inner join '.$jobDetail['pTable'].' t22 on t11.mapping_id = t22.mapping_id'
			);
			$cond = array(
				't22.custom2 is null'
			);
			$others = ' GROUP BY t11.class_id,t11.'.$batchrecords['user_id'].'	HAVING count(1) > 1 ';
			
			$qry = $this->db->prepareQuerySelect($jobDetail['sTable']. ' t11',$fields,$cond,$joins,$others);
			expDebug::dPrintDBAPI("Get duplicate records ",$qry);
			$this->db->callQuery($qry);
			$duplicate = $this->db->fetchAllResults();
			
			foreach($duplicate as $dp){
				list($flds,$params,$joins) = array(array(),array(),array());
				$flds = array("t1.record_status" => ":record_status ",
						"t1.remarks"  => "concat(ifnull(t1.remarks,''), :err )");
				$condi = array(
					't1.class_id = :clsid ',
					't1.'.$batchrecords['user_id'].' = :name '
				);
				$params =  array(
					':record_status' => 'IN',
					':err' => 'User have more than one enrollment for a same class in a Feed. ',
					':clsid' => $dp->class_id,
					':name' => $dp->user_name
				);
				$query1 = $this->db->prepareQueryUpdate($jobDetail['sTable']." t1",$flds,$condi,$joins);
				expDebug::dPrintDBAPI("Duplicate status update ",$query1,$params);
				$this->db->callExecute($query1,$params);
			}
		}catch(Exception $e){
			expDebug::dPrint("ERROR in duplicate check ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function updateRegStatus(){
		try{
			$jobDetail = $this->getJobDetails();
			$batchrecords = $this->getBatchRecords();
			$update = array();
			$update['table'] = $jobDetail['pTable'].' tm';
			$update['fields'] = array(
					'tm.reg_status' => '(CASE WHEN (t1.'.$batchrecords['reg_status'].' = :reg 
																		or t1.'.$batchrecords['reg_status'].' = :comp 
																		or t1.'.$batchrecords['reg_status'].' = :inp 
																		or t1.'.$batchrecords['reg_status'].' = :inc 
																		or t1.'.$batchrecords['reg_status'].' = :nsw ) THEN :rgsts 
															WHEN t1.'.$batchrecords['reg_status'].' = :can THEN :cansts
															ELSE tm.reg_status END)',
					'tm.comp_status' => '(CASE WHEN t1.'.$batchrecords['reg_status'].' = :reg THEN :enrsts 
																		 WHEN t1.'.$batchrecords['reg_status'].' = :comp THEN :cmpsts
																		 WHEN t1.'.$batchrecords['reg_status'].' = :inp THEN :inpsts 
																		 WHEN t1.'.$batchrecords['reg_status'].' = :inc THEN :incsts 
																		 WHEN t1.'.$batchrecords['reg_status'].' = :nsw THEN :nswsts END)'
			);
			$update['condition']=array(
					//"tm.mapping_id = x.mapid",
					//"tm.batch_id = :batch_id ",
					"tm.record_status = :recsts ",
			);
			$update['joins'] = array(
				'INNER JOIN '.$jobDetail['sTable'].' t1 ON t1.mapping_id = tm.mapping_id AND t1.record_status = :recsts '
			);
			$arg=array(//':batch_id' => $jobDetail['batchId'],
					':reg' => 'enrolled',
					':comp' => 'completed',
					':inp' => 'In Progress',
					':inc' => 'Incomplete',
					':nsw' => 'noshow',
					':can' => 'canceled',
					':rgsts' => 'lrn_crs_reg_cnf',
					':cansts' => 'lrn_crs_reg_can',
					':cmpsts' => 'lrn_crs_cmp_cmp',
					':inpsts' => 'lrn_crs_cmp_inp',
					':incsts' => 'lrn_crs_cmp_inc',
					':nswsts' => 'lrn_crs_cmp_nsw',
					':enrsts' => 'lrn_crs_cmp_enr',
					//':attsts' => 'lrn_crs_cmp_att',
					':recsts' => 'R'
			);
			
			$updQry1 = $this->db->prepareQueryUpdate($update['table'],$update['fields'],$update['condition'],$update['joins']);
			expDebug::dPrintDBAPI(" Query for update of registration status",$updQry1,$arg);
			$this->db->callExecute($updQry1,$arg);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in reg status update ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function getDTCount($opt = array()){
		try{
			$tblDet = $this->getTableDetails();
			$args = array();
	 		$conditons = array();
	 		$fields = array(
				'count(1) ',
	 		);
	 		//if($status){
	 			$conditons = array(
						'temp2.batch_id = :btid ',
						'temp2.record_status = :r ',
						'temp2.operation = :opr ',
						'temp2.custom3 IN (:opt1 , :opt2 )'
						);
						$args=array(
						':btid' => $tblDet['btc_id'],
						':opr' => $opt[2],
						':r' => 'R',
						':opt1' => $opt[0],
						':opt2' => $opt[1]
						);
	 		//}
	
	 		// Prepare query
	 		$query = $this->db->prepareQuerySelect($tblDet['tblname']." temp2",$fields,$conditons);
	 		expDebug::dPrintDBAPI("Get class count based on delivery type - ",$query,$args);
	 		// Execute query
	 		$this->db->callQuery($query,$args);
	 		// Fetch results
	 		$cntDtCnt = $this->db->fetchColumn();
	 		expDebug::dPrint('count -->> '. print_r($cntDtCnt,true),5);
	 		return $cntDtCnt;
		}catch(Exception $e){
			expDebug::dPrint("ERROR in DT Count ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	protected function insertOrder($opt = array()){
		try{
			$tblDet = $this->getTableDetails();
			$tbl = $tblDet['tblname'];
			$insert = array();
			$insert['table'] = 'slt_order';
			$insert['fields'] = array('user_id','order_status','created_on','created_by','updated_on','updated_by','custom_dataload','dataload_by');
	
			$select = array();
			$select['table'] = "$tbl temp2";
			$select['fields']= array('temp2.user_id','temp2.reg_status','now()', ':by ','now()', ':by ','temp2.mapping_id',':loadby ');
				
			$select['condition'] = array(
					'temp2.batch_id = :btid ',
					'temp2.record_status = :r ',
					'temp2.operation = :opr ',
					'temp2.custom3 IN (:opt1 , :opt2 )'
			);
			$arg=array(
					':btid' => $tblDet['btc_id'],
					':opr' => $opt[2],
					':r' => 'R',
					':by'=> $tblDet['user_id'],
					':loadby'=> $tblDet['loadby'],
					':opt1' => $opt[0],
					':opt2' => $opt[1]
			);
			
			$query = $this->db->prepareInsertBySelect($insert,$select);
			expDebug::dPrintDBAPI("Query for slt order bulk execute ", $query,$arg);
			$this->db->callExecute($query,$arg);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in order insert ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	protected function updateTemp2OrderId($opt = array()){
		try{
			$tblDet = $this->getTableDetails();
			$tbl = $tblDet['tblname'];
			$field= array('temp2.order_id' => 'ord.id');
	 		$joins= array('INNER JOIN slt_order ord ON ord.custom_dataload = temp2.mapping_id AND ord.dataload_by = :loadby  ');
			$condition = array(
					'temp2.batch_id = :btid ',
					'temp2.record_status = :r ',
					'temp2.operation = :opr ',
					'temp2.custom3 IN (:opt1 , :opt2 )'
			);
			$args=array(
					':btid' => $tblDet['btc_id'],
					':loadby' => $tblDet['loadby'],
					':opr' => $opt[2],
					':r' => 'R',
					':opt1' => $opt[0],
					':opt2' => $opt[1]
			);
			
			$qry = $this->db->prepareQueryUpdate("$tbl temp2",$field,$condition,$joins);
			expDebug::dPrintDBAPI("UPDATE ORDER ID IN TEMP TALE --> ",$qry,$args);
			$this->db->callExecute($qry,$args);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in update order id in temp2 ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	protected function insertOrderItem($opt = array()){
		try{
			$tblDet = $this->getTableDetails();
			$tbl = $tblDet['tblname'];
			$insert = array();
			$insert['table'] = 'slt_order_items';
			$insert['fields'] = array('order_id','user_id','course_id','class_id','created_on','created_by','updated_on','updated_by','dataload_by');
	
			$select = array();
			$select['table'] = "$tbl temp2";
			$select['fields']= array('ord.id','temp2.user_id','temp2.course_id','temp2.class_id','now()', ':by ','now()', ':updby ',':loadby ');
			$select['joins']= array('INNER JOIN slt_order ord ON ord.custom_dataload = temp2.mapping_id AND ord.dataload_by = :loadby  ');
				
			$select['condition'] = array(
					'temp2.batch_id = :btid ',
					'temp2.record_status = :r ',
					'temp2.operation = :opr ',
					'temp2.custom3 IN (:opt1 , :opt2 )'
			);
			$arg=array(
					':btid' => $tblDet['btc_id'],
			    ':loadby' => $tblDet['loadby'],
					':opr' => $opt[2],
					':r' => 'R',
					':by'=> $tblDet['user_id'],
					':updby' => $tblDet['user_id'],
					':opt1' => $opt[0],
					':opt2' => $opt[1]
			);
			
			$query = $this->db->prepareInsertBySelect($insert,$select);
			expDebug::dPrintDBAPI("Query for slt order items bulk execute ", $query,$arg);
			$this->db->callExecute($query,$arg);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in insert order items ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function insertEnrollment($opt = array()){
		try{
			$tblDet = $this->getTableDetails();
			$fields = array();
			$selFields = array();
			if($opt[0] == 'lrn_cls_dty_wbt'){
				$createdBy = array('reg_status_date','comp_status','order_id','created_on','created_by','updated_on','updated_by','custom_dataload','dataload_by', 'comp_by', 'comp_on','custom_status','is_compliance','progress');
				$createdByArgs = array('enr.reg_date','enr.comp_status','ord.id','now()', ':by ','now()', ':updby ',':cust ',':loadby ', 'enr.comp_by', 'enr.comp_date','"i"','enr.is_compliance','enr.progress');
			}else{
				$createdBy = array('waitlist_flag','waitlist_priority','reg_status_date','comp_status','order_id','created_on','created_by','updated_on','updated_by','custom_dataload','dataload_by', 'comp_by', 'comp_on','custom_status','is_compliance','progress');
				$createdByArgs = array('enr.waitlist_flag','enr.waitlist_priority','enr.reg_date','enr.comp_status','ord.id','now()', ':by ','now()', ':updby ',':cust ',':loadby ', 'enr.comp_by', 'enr.comp_date','"i"','enr.is_compliance','enr.progress');
			}
			$query = $this->db->prepareQuerySelect('slt_dataload_table_mapping map',array('map.base_column'),array('map.entity_id= :entid ','map.base_column not like :col '));
			$this->db->callQuery($query,array(':entid' => $tblDet['ent_id'],':col'=>'%custom%'));
			
			$columnDetails = $this->db->fetchAllResults();
			foreach($columnDetails as $col){
				$fields[] = $col->base_column;
				$selFields[] = 'enr.'.$col->base_column;
			}
			
			$tbl = $tblDet['tblname'];
			$insert = array();
			$insert['table'] = 'slt_enrollment';
			$insert['fields'] = array_merge($fields,$createdBy);
	
			$select = array();
			$select['table'] = "$tbl enr";
			$select['fields']= array_merge($selFields,$createdByArgs);
			$select['joins']= array('INNER JOIN slt_order ord ON ord.custom_dataload = enr.mapping_id and ord.dataload_by = :loadby  ');
				
			$select['condition'] = array(
					'enr.batch_id = :btid ',
					'enr.record_status = :r ',
					'(enr.operation = :opr OR enr.operation = :oprsts )',
					'enr.custom3 IN (:opt1 , :opt2 )'
			);
			$arg=array(
					':btid' => $tblDet['btc_id'],
					':opr' => $opt[2],
					':oprsts' => 'upsert',
					':r' => 'R',
					':by'=> $tblDet['user_id'],
					':updby' => $tblDet['user_id'],
					':cust' => $tblDet['btc_id'],
					':opt1' => $opt[0],
					':opt2' => $opt[1],
					//':cmby' => $tblDet['user_id'],
					':loadby' => $tblDet['loadby'],
			);
			
			$query = $this->db->prepareInsertBySelect($insert,$select);
			expDebug::dPrintDBAPI("Query for slt enrollment bulk execute ", $query,$arg);
			$this->db->callExecute($query,$arg);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in insert enrollment ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	protected function insertEnrollContentMap(){
		try{
			$tblDet = $this->getTableDetails();
			//$tbl = $tblDet['tblname'];
			$insert = array();
			$insert['table'] = 'slt_enrollment_content_mapping';
			$insert['fields'] = array('enroll_id','user_id','course_id','class_id','content_id','version_id','created_on','created_by','updated_on','updated_by');
	
			$select = array();
			$select['table'] = "slt_enrollment enr";
			$select['fields']=  array('enr.id','enr.user_id','enr.course_id','enr.class_id','cmap.content_id','cver.id','now()', ':by ','now()', ':updby ');
			$select['joins']= array(//'INNER JOIN slt_course_class cls ON cls.id = enr.class_id AND (cls.delivery_type = :wbt OR cls.delivery_type = :vod ) AND enr.custom_dataload= :btid AND enr.dataload_by= :loadby  ',
															'INNER JOIN slt_course_content_mapper cmap ON cmap.class_id = enr.class_id AND cmap.course_id = enr.course_id',
															'INNER JOIN slt_content_version cver ON cver.content_master_id = cmap.content_id AND cver.status = "lrn_cnt_sts_atv"'
														  );
				
			$select['condition'] = array(
					'enr.custom_dataload = :btid ',
					'enr.dataload_by = :loadby ',
					//'cls.id IS NOT NULL'
			);
			$arg=array(
					':btid' => $tblDet['btc_id'],
					//':r' => 'R',
					':by'=> $tblDet['user_id'],
					':updby' => $tblDet['user_id'],
					':loadby' => $tblDet['loadby']
					//':wbt' => 'lrn_cls_dty_wbt',
					//':vod' => 'lrn_cls_dty_vod'
			);
			
			$query = $this->db->prepareInsertBySelect($insert,$select);
			expDebug::dPrintDBAPI("Query for slt enrollment content mapping bulk execute", $query,$arg);
			$this->db->callExecute($query,$arg);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in insert content mapping ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function updateEnrollment($opt = array()){
		try{
			$tblDet = $this->getTableDetails();
			$update = array();
			$update['table'] = 'slt_enrollment enr';
			$update['fields'] = array(
					'enr.mandatory' => 't2.mandatory ',
					'enr.is_compliance' => 't2.is_compliance',
					'enr.reg_status' => 't2.reg_status',
					'enr.comp_status' => 't2.comp_status',
					'enr.comp_date' => 't2.comp_date',
					'enr.score' => 't2.score',
					'enr.waitlist_flag' => 't2.waitlist_flag',
					'enr.waitlist_priority' => 't2.waitlist_priority',
					'enr.cmpl_expired' => 't2.cmpl_expired',
                    'enr.updated_by' => $tblDet['user_id'],
					'enr.updated_on' => 'now()',
					'enr.custom_dataload' => ':batch_id',
					'enr.dataload_by' => ':loadby',
					'enr.comp_by' => $tblDet['user_id'],
					'enr.comp_on' => 't2.comp_date',
					'enr.custom_status' => ':upd ',
					'enr.progress' => 't2.progress'
			);
			$update['condition']=array(
					"enr.class_id = t2.class_id",
					"enr.course_id = t2.course_id",
					"enr.user_id = t2.user_id ",
					"t2.custom3 IN (:opt1 , :opt2 )",
					"t2.operation = :opr ",
					"IF(t2.reg_status = :cnlsts , enr.master_enrollment_id IS NULL ,1=1 )"
					//"enr.id = x.primary_id"
			);
			$update['joins'] = array(
				'INNER JOIN '.$tblDet['tblname'].' t2 ON enr.id = t2.primary_id AND t2.batch_id = :batch_id '
			);
			$arg=array(
				':batch_id' => $tblDet['btc_id'],
				':loadby' => $tblDet['loadby'],
				':opr' => $opt[2],
				':opt1' => $opt[0],
				':opt2' => $opt[1],
				':upd' => 'u',
				':cnlsts' => 'lrn_crs_reg_can'
			);
					
			$updQry1 = $this->db->prepareQueryUpdate($update['table'],$update['fields'],$update['condition'],$update['joins'] );
			expDebug::dPrintDBAPI(" Query to update slt_enrollment table for update records ",$updQry1,$arg);
			$this->db->callExecute($updQry1,$arg);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in enrollment update ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function updateOrderId($opt = array()){
		try{
			$tblDet = $this->getTableDetails();
			$field= array('temp2.order_id' =>'enr.order_id');
	 		$joins= array('INNER JOIN slt_enrollment enr ON enr.id = temp2.primary_id');
	 		$condition = array(
	 				'temp2.batch_id = :btid ',
	 				'temp2.operation = :opr ',
	 				'temp2.custom3 IN (:opt1 , :opt2 )',
	 		);
	 		$args=array(
	 				':btid' => $tblDet['btc_id'],
	 				':opr' => $opt[2],
					':opt1' => $opt[0],
					':opt2' => $opt[1]
	 		);
	 			
	 		$qry = $this->db->prepareQueryUpdate($tblDet['tblname']." temp2",$field,$condition,$joins);
	 		expDebug::dPrintDBAPI("UPDATE ORDER ID IN TEMP TALE --> ",$qry,$args);
	 		$this->db->callExecute($qry,$args);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in order id update ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function seatReAllocation(){
		try{
			//Only for ILT and VCL
			$tblDet = $this->getTableDetails();
			$query = "select distinct(class_id) from ".$tblDet['tblname']." where batch_id=:bid and operation=:opr and custom3 IN (:ilt , :vcl ) and reg_status = :regsts ";
			expDebug::dPrintDBAPI("getclass ids  ",$query,array(':bid'=>$tblDet['btc_id'],':opr' => 'update',':ilt' => 'lrn_cls_dty_ilt',
							':vcl' => 'lrn_cls_dty_vcl',':regsts' => 'lrn_crs_reg_can'));
			$this->db->callQuery($query,array(':bid'=>$tblDet['btc_id'],':opr' => 'update',':ilt' => 'lrn_cls_dty_ilt',
							':vcl' => 'lrn_cls_dty_vcl',':regsts' => 'lrn_crs_reg_can'));
			//expDebug::dPrintDBAPI($msgStr)
			$classlist = $this->db->fetchAllResults();
			expDebug::dPrint('class ids for update -->> '.print_r($classlist,true),5);
	 		foreach($classlist as $key => $value){
				//expDebug::dPrint('c  ids -->> '. $value->class_id);
				$cid = $value->class_id;
				
				$query = "select count(reg_status) as cnt, reg_status from slt_enrollment where class_id =:cid  group by reg_status";
				$this->db->callQuery($query,array(':cid'=>$cid));
				$enr_details = $this->db->fetchAllResults();
				expDebug::dPrint('enrollment details '.print_r($enr_details,true),5 );
				if($enr_details[0]->reg_status == 'lrn_crs_reg_cnf')
					$confirm_cnt = $enr_details[0]->cnt;
				else if($enr_details[1]->reg_status == 'lrn_crs_reg_cnf')
					$confirm_cnt = $enr_details[1]->cnt;
				else
					$confirm_cnt = 0;
				if($enr_details[1]->reg_status == 'lrn_crs_reg_wtl' || $enr_details[2]->reg_status == 'lrn_crs_reg_wtl'){
					$query1 = "select max_seats,waitlist_count from slt_course_class where id =:cid ";
					$this->db->callQuery($query1,array(':cid'=>$cid));
					$class_details = $this->db->fetchAllResults();
					expDebug::dPrint('inside update records waitlist seats -->> '. print_r($class_details,true),5);
					if($class_details[0]->max_seats - ($confirm_cnt) > 0){
							$range = $class_details[0]->max_seats - ($confirm_cnt) ;	
							$updQry = "update slt_enrollment enr ,
							(select en.id from slt_enrollment en
							where en.class_id = :cid and en.reg_status = :wtl LIMIT $range)x
							set enr.reg_status= :sts ,
							enr.comp_status = :enr ,
							enr.waitlist_priority = :nul ,
							enr.waitlist_flag = :nul
							where
							enr.id = x.id";
							expDebug::dPrintDBAPI("Update waitlist seat in enrollment for update records ",$updQry,array(':cid'=> $cid, ':sts'=> 'lrn_crs_reg_cnf',':enr'=> 'lrn_crs_cmp_enr', ':wtl' => 'lrn_crs_reg_wtl',':nul'=> NULL));
							$this->db->callExecute($updQry,array(':cid'=> $cid, ':sts'=> 'lrn_crs_reg_cnf',':enr'=> 'lrn_crs_cmp_enr',':wtl' => 'lrn_crs_reg_wtl',':nul'=> NULL));
							
							//Waitlist priority Update
							$updqry1 = "SET @wtl:=0;
							update slt_enrollment
							set waitlist_priority=@wtl:=@wtl+1 , waitlist_flag = :wtlsts 
							
							where reg_status = :wtlsts and class_id = :cid ";
							expDebug::dPrintDBAPI("Update waitlist priority in enrollment for update records ",$updqry1,array(':cid'=> $cid,':wtlsts' => 'lrn_crs_reg_wtl'));
							$this->db->callExecute($updqry1,array(':cid'=> $cid,':wtlsts' => 'lrn_crs_reg_wtl'));
					}
					
				}
	 		}
		}catch(Exception $e){
			expDebug::dPrint("ERROR in seat reallocation ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	protected function seatAllocation(){
		try{
			$tblDet = $this->getTableDetails();
			$this->db->callQuery("SELECT COUNT(1) FROM ".$tblDet['tblname']." WHERE batch_id =:bid ",array(":bid"=> $tblDet['btc_id']));
	 		$batch_size = $this->db->fetchColumn();
	 		//Only for ILT and VCL
	 		//50823: Alert message is in correct for VOD and WBT class
			//$query = "select distinct(class_id) from ".$tblDet['tblname']." where batch_id=:bid ";
	 		//$this->db->callQuery($query,array(':bid'=>$tblDet['btc_id']));
			$query = "select distinct(class_id) from ".$tblDet['tblname']." where batch_id=:bid and custom3 in(:ilt ,:vcl ) ";
			$this->db->callQuery($query,array(':bid'=>$tblDet['btc_id'],':ilt'=>'lrn_cls_dty_ilt',':vcl'=>'lrn_cls_dty_vcl'));
			
			$class_ids = $this->db->fetchAllResults();
			expDebug::dPrint('class ids -->> '.print_r($class_ids,true),5);
			foreach($class_ids as $key => $value){
				//expDebug::dPrint('c  ids -->> '. $value->class_id);
				$cid = $value->class_id;
				$query1 = "select max_seats,waitlist_count from slt_course_class where id =:cid ";
				$this->db->callQuery($query1,array(':cid'=>$cid));
				$class_details = $this->db->fetchAllResults();
				expDebug::dPrint('waitlist seats -->> '. print_r($class_details,true),5);
				//expDebug::dPrint('max seats -->> '. $class_details[0]->max_seats);
			
				// count of confirmed seats in enrollment and cancelled seats in feed file
				list($fields,$args,$joins,$conditons) = array(array(),array(),array(),array());
				$args = array();
				$conditons = array();
				$fields = array(
						'count(t2.reg_status) as cancnt ',
						'enr.reg_status as enrsts',
						'count(enr.reg_status) as enrcnt'
				);
				$joins = array(
						'left join '.$tblDet['tblname'].' t2 on t2.class_id = enr.class_id and t2.user_id = enr.user_id and t2.reg_status = \'lrn_crs_reg_can\' and t2.batch_id = :bid '
						);
				$conditons = array(
						'enr.class_id = :cid ',
						'enr.reg_status in (:cnf ,:wtl ) ',
						//'t2.batch_id = :bid '
				);
				$args=array(
						':cid'=>$cid,
						':bid' => $tblDet['btc_id'],
						':cnf' => 'lrn_crs_reg_cnf',
						':wtl' => 'lrn_crs_reg_wtl'
				);
				
				$other = 'GROUP BY enr.reg_status';
				
				// Prepare query
				$query = $this->db->prepareQuerySelect("slt_enrollment enr ",$fields,$conditons,$joins, $other);
				expDebug::dPrintDBAPI("Get confirmed seat and cancel seat count",$query,$args);
				// Execute query
				$this->db->callQuery($query,$args);
				// Fetch results
				$seatDetails = $this->db->fetchAllResults();
				expDebug::dPrint(' seatDetails count '. print_r($seatDetails,true),5);
				
				//expDebug::dPrint('test 1111'. $seatDetails[0]->enrsts);
				$nullcnt = 0;
				$confirmed_seats = ($seatDetails[0]->enrsts == 'lrn_crs_reg_cnf') ? $seatDetails[0]->enrcnt : $nullcnt;
				$waitlist_seats = (!empty($seatDetails[1]->enrsts) && $seatDetails[1]->enrsts == 'lrn_crs_reg_wtl') ?  $seatDetails[1]->enrcnt : $nullcnt;
				$confirm_cancel =	($seatDetails[0]->enrsts == 'lrn_crs_reg_cnf') ?  $seatDetails[0]->cancnt : $nullcnt;
				$waitlist_cancel = (!empty($seatDetails[1]->enrsts) && $seatDetails[1]->enrsts == 'lrn_crs_reg_wtl') ? $seatDetails[1]->cancnt : $nullcnt;
				expDebug::dPrint('$confirmed_seats ::: '.$confirmed_seats . ' $waitlist_seats ::: '.$waitlist_seats .' $confirm_cancel ::: '. $confirm_cancel .' $waitlist_cancel ::: '.$waitlist_cancel );
				$avail_seats = $class_details[0]->max_seats - $confirmed_seats + $confirm_cancel;
				if($avail_seats > 0 ){
					if($waitlist_seats > 0){ //Waitlist promotion
						if($avail_seats >= ($waitlist_seats - $waitlist_cancel )){
							//Waitlist cancel 
							if($waitlist_cancel >0 ){
								$field=array(
										'enr.reg_status' =>':can',
										'enr.waitlist_priority' =>':nul',
										'enr.waitlist_flag' =>':nul',
										't2.record_status' => 'CP',
										't2.notifyuser' => ':wtldrp '
								);
								$condition=array(
										'enr.reg_status = :wtl ',
										'enr.class_id = :cid '
								);
								$joins = array(
										'left join '.$tblDet['tblname'].' t2 on t2.class_id = enr.class_id and t2.user_id = enr.user_id and t2.reg_status = :can and t2.record_status = :r and t2.batch_id = :bid '		
								);
								$arg = array(
										':bid' => $tblDet['btc_id'],
										':cid' => $cid,
										':wtl' => 'lrn_crs_reg_wtl',
										':can' => 'lrn_crs_reg_can',
										':r'  => 'R',
										':nul' => 'null',
										':wtldrp' => 'WaitlistDrop'
								);
								$qry1 = $this->db->prepareQueryUpdate("slt_enrollment enr",$field,$condition,$joins);
								expDebug::dPrintDBAPI(" Bulk Waitlist Cancel first part ",$qry1,$arg);
								$this->db->callExecute($qry1,$arg);
							}
							
							//Update Enrollment table
							$field=array(
									'reg_status' => 'lrn_crs_reg_cnf'
							);
							$condition=array(
									'class_id = :cid ',
									'reg_status = :wtl '
							);
							$arg = array(
									':cid' => $cid,
									':wtl' => 'lrn_crs_reg_wtl'
									
							);
							$qry1 = $this->db->prepareQueryUpdate("slt_enrollment",$field,$condition);
							expDebug::dPrintDBAPI(" Bulk Waitlist promotion ",$qry1,$arg);
							$this->db->callExecute($qry1,$arg);
							
							$newAvl_seats = $avail_seats - ($waitlist_seats - $waitlist_cancel );
							// limit count for seat full
							$limit1 = $newAvl_seats .','. $batch_size;
							if(($class_details[0]->waitlist_count)>0){
							 	//Update waitlist seat in temp table 2
								$limit =  $newAvl_seats.','.$class_details[0]->waitlist_count;
								
								//$updQry = "update ".$tblDet['tblname']." set reg_status= :sts where class_id = :cid and reg_status = :prevsts and record_status = :r and batch_id = :bid and operation = :oper LIMIT $limit";
								$updQry = "update ".$tblDet['tblname']." t22, 
													(select t2.mapping_id from ".$tblDet['tblname']." t2
													where t2.class_id = :cid and t2.reg_status = :prevsts and t2.record_status = :r and t2.batch_id = :bid and t2.operation = :oper LIMIT $limit)x
													set t22.reg_status=  :sts ,
													t22.comp_status = :nul ,
													t22.notifyuser = :wtl
													where 
													t22.mapping_id = x.mapping_id";
								expDebug::dPrintDBAPI("Update waitlist seat in temp table 2 ",$updQry,array(':cid'=> $cid, ':sts'=> 'lrn_crs_reg_wtl',':nul'=> NULL, ':wtl' => 'Waitlist',':prevsts' => 'lrn_crs_reg_cnf',':bid' => $tblDet['btc_id'],':r' => 'R',':oper' => 'insert'));
								$this->db->callExecute($updQry,array(':cid'=> $cid, ':sts'=> 'lrn_crs_reg_wtl',':nul'=> NULL, ':wtl' => 'Waitlist', ':prevsts' => 'lrn_crs_reg_cnf',':bid' => $tblDet['btc_id'],':r' => 'R',':oper' => 'insert'));
								
								//Update Waitlist priority
								
								$updqry1 = "SET @wtl:=0;
														update ".$tblDet['tblname']."
														set waitlist_priority=@wtl:=@wtl+1,
														waitlist_flag = :wtlsts 
														where reg_status = :wtlsts and class_id = :cid  ";
								expDebug::dPrintDBAPI("Update waitlist seat in temp table 2 ",$updqry1,array(':cid'=> $cid,':wtlsts' => 'lrn_crs_reg_wtl'));
								$this->db->callExecute($updqry1,array(':cid'=> $cid,':wtlsts' => 'lrn_crs_reg_wtl'));
								
								// limit count for seat full
								$limit1 = $newAvl_seats + $class_details[0]->waitlist_count;
							}
							//$updQry = "update ".$tblDet['tblname']." set record_status= :val, concat(ifnull(custom4,''),:err ) where class_id = :cid and reg_status = :prevsts and record_status = :r and batch_id = :bid and operation = :oper LIMIT $limit1";
							$updQry = "update ".$tblDet['tblname']." t22,
							(select t2.mapping_id from ".$tblDet['tblname']." t2
							where t2.class_id = :cid and t2.reg_status = :prevsts and t2.record_status = :r and t2.batch_id = :bid and t2.operation = :oper LIMIT $limit1)x
							set t22.record_status= :val, t22.custom4 = concat(ifnull(t22.custom4,''),:err )
							where
							t22.mapping_id = x.mapping_id";
							
							expDebug::dPrintDBAPI("Update waitlist seat in temp table 2 ",$updQry,array(':val'=> 'IN', ':err'=> 'Max seats Full.',':cid'=> $cid, ':prevsts' => 'lrn_crs_reg_cnf',':bid' => $tblDet['btc_id'],':r' => 'R',':oper' => 'insert'));
							$this->db->callExecute($updQry,array(':val'=> 'IN', ':err'=> 'Max seats Full.',':cid'=> $cid, ':prevsts' => 'lrn_crs_reg_cnf',':bid' => $tblDet['btc_id'],':r' => 'R',':oper' => 'insert'));
						}else{
							if($waitlist_cancel >0 ){
								$field=array(
										'enr.reg_status' =>':can',
										'enr.waitlist_priority' =>':nul',
										'enr.waitlist_flag' =>':nul',
										't2.record_status' => 'CP',
										't2.notifyuser' => ':wtldrp '
								);
								$condition=array(
										'enr.reg_status = :wtl '
								);
								$joins = array(
										'left join '.$tblDet['tblname'].' t2 on t2.class_id = enr.class_id and t2.user_id = enr.user_id and t2.reg_status = :can and t2.record_status = :r and t2.batch_id = :bid '
								);
								$arg = array(
										':bid' => $tblDet['btc_id'],
										':wtl' => 'lrn_crs_reg_wtl',
										':can' => 'lrn_crs_reg_can',
										':r'  => 'R',
										':nul' => 'null',
										':wtldrp' => 'WaitlistDrop'
								);
								$qry1 = $this->db->prepareQueryUpdate("slt_enrollment enr ",$field,$condition,$joins);
								expDebug::dPrintDBAPI(" Bulk Waitlist Cancel ",$qry1,$arg);
								$this->db->callExecute($qry1,$arg);
							}
							
							//Update Enrollment table
							//$updQry = "update slt_enrollment set reg_status= :sts,waitlist_priority = :nul where class_id = :cid and reg_status = :prevsts LIMIT $avail_seats";
							$updQry = "update slt_enrollment t22,
							(select t2.mapping_id from slt_enrollment t2
							where t2.class_id = :cid and t2.reg_status = :prevsts LIMIT $avail_seats)x
							set t22.reg_status=  :sts,t22.waitlist_priority = :nul ,t22.waitlist_flag  = :nul 
							where
							t22.mapping_id = x.mapping_id";
							expDebug::dPrintDBAPI(" Waitlist promotion ",$updQry,array(':cid'=> $cid, ':sts'=> 'lrn_crs_reg_cnf', ':prevsts' => 'lrn_crs_reg_wtl',':nul' => 'null'));
							$this->db->callExecute($updQry,array(':cid'=> $cid, ':sts'=> 'lrn_crs_reg_cnf', ':prevsts' => 'lrn_crs_reg_wtl',':nul' => 'null'));
							
							//Update Waitlist priority
							
							$updqry1 = "SET @wtl:=0;
							update slt_enrollment
							set waitlist_priority=@wtl:=@wtl+1 , waitlist_flag = :wtlsts 
							where reg_status = :wtlsts and class_id = :cid ";
							expDebug::dPrintDBAPI("Update waitlist priority in temp table 2 ",$updqry1,array(':cid'=> $cid,':wtlsts' => 'lrn_crs_reg_wtl'));
							$this->db->callExecute($updqry1,array(':cid'=> $cid,':wtlsts' => 'lrn_crs_reg_wtl'));
							
							//$newAvl_seats = $avail_seats - ($waitlist_seats - $waitlist_cancel )
							//Waitlist Update in temp table
							$avail_wtl_seats = $class_details[0]->waitlist_count - ($waitlist_seats - $waitlist_cancel - $avail_seats);
							//$updQry = "update ".$tblDet['tblname']." set reg_status= :sts where class_id = :cid and reg_status = :prevsts and record_status = :r and batch_id = :bid  and operation = :oper LIMIT $avail_wtl_seats";
							$updQry = "update ".$tblDet['tblname']." t22,
							(select t2.mapping_id from ".$tblDet['tblname']." t2
							where t2.class_id = :cid and t2.reg_status = :prevsts and t2.record_status = :r and t2.batch_id = :bid and t2.operation = :oper LIMIT $avail_wtl_seats)x
							set t22.reg_status= :sts ,
							t22.comp_status = :nul ,
							t22.notifyuser = :wtl 
							where
							t22.mapping_id = x.mapping_id";
							expDebug::dPrintDBAPI("Update waitlist seat in temp table 2 ",$updQry,array(':cid'=> $cid, ':sts'=> 'lrn_crs_reg_wtl',':nul'=> NULL, ':wtl' => 'Waitlist', ':prevsts' => 'lrn_crs_reg_cnf',':bid' => $tblDet['btc_id'],':r' => 'R',':oper'=> 'insert'));
							$this->db->callExecute($updQry,array(':cid'=> $cid, ':sts'=> 'lrn_crs_reg_wtl',':nul'=> NULL, ':wtl' => 'Waitlist', ':prevsts' => 'lrn_crs_reg_cnf',':bid' => $tblDet['btc_id']));
							
							//Update Waitlist priority
								
							//select MAX(user_id) from slt_enrollment where class_id =4
							$this->db->callQuery("SELECT ifnull(MAX(waitlist_priority),0) FROM slt_enrollment WHERE class_id =:cid and reg_status = :wtl ",array(':cid'=> $cid, ':wtl'=> 'lrn_crs_reg_wtl'));
							$max_wtl_count = $this->db->fetchColumn();
							expDebug::dPrint('max waitlist count 1'. $max_wtl_count,4);
							$updqry1 = "SET @wtl:=".$max_wtl_count.";
							update ".$tblDet['tblname']."
							set waitlist_priority=@wtl:=@wtl+1 , waitlist_flag  =:wtlsts 
							where reg_status = :wtlsts and class_id = :cid ";
							expDebug::dPrintDBAPI("Update waitlist priority in temp table 2 ",$updqry1,array(':cid'=> $cid,':wtlsts' => 'lrn_crs_reg_wtl'));
							$this->db->callExecute($updqry1,array(':cid'=> $cid,':wtlsts' => 'lrn_crs_reg_wtl'));
							
							
							//$updQry = "update ".$tblDet['tblname']." set record_status= :val, concat(ifnull(custom4,''),:err ) where class_id = :cid and record_status = :r and batch_id = :bid  and operation = :oper LIMIT $limit1";
							$updQry = "update ".$tblDet['tblname']." t22,
							(select t2.mapping_id from ".$tblDet['tblname']." t2
							where t2.class_id = :cid and t2.record_status = :r and t2.batch_id = :bid  and t2.operation = :oper LIMIT $limit1)x
							set t22.record_status= :val, t22.custom4 = concat(ifnull(t22.custom4,''),:err )
							where
							t22.mapping_id = x.mapping_id";
							expDebug::dPrintDBAPI("Update waitlist seat in temp table 2 ",$updQry,array(':cid'=> $cid, ':val'=> 'IN', ':err'=> 'Max seats Full.',':bid' => $tblDet['btc_id'],':oper' => 'insert',':r' => 'R'));
							$this->db->callExecute($updQry,array(':cid'=> $cid, ':val'=> 'IN', ':err'=> 'Max seats Full.',':bid' => $tblDet['btc_id'],':oper' => 'insert',':r' => 'R'));
							
						}
					}else{
						//Update waitlist in temp2 table
						$avail_wtl_seats = $class_details[0]->waitlist_count - $waitlist_seats + $waitlist_cancel;
						$range = $avail_seats .','.$batch_size;
						if($avail_wtl_seats > 0){
							$limit = $avail_seats .','.$avail_wtl_seats;
							//$updQry = "update ".$tblDet['tblname']." set reg_status= :sts where class_id = :cid and reg_status = :prevsts and record_status = :r and batch_id = :bid and operation = :oper LIMIT $limit";
							$updQry = "update ".$tblDet['tblname']." t22,
							(select t2.mapping_id from ".$tblDet['tblname']." t2
							where t2.class_id = :cid and t2.reg_status = :prevsts and t2.record_status = :r and t2.batch_id = :bid and t2.operation = :oper LIMIT $limit)x
							set t22.reg_status= :sts ,
							t22.comp_status = :nul ,
							t22.notifyuser = :wtl 
							where
							t22.mapping_id = x.mapping_id";
							expDebug::dPrintDBAPI("Update waitlist seat in temp table 2 ",$updQry,array(':cid'=> $cid, ':sts'=> 'lrn_crs_reg_wtl',':nul'=> NULL, ':wtl' => 'Waitlist', ':prevsts' => 'lrn_crs_reg_cnf',':bid' => $tblDet['btc_id'],':r' => 'R',':oper' => 'insert'));
							$this->db->callExecute($updQry,array(':cid'=> $cid, ':sts'=> 'lrn_crs_reg_wtl',':nul'=> NULL, ':wtl' => 'Waitlist', ':prevsts' => 'lrn_crs_reg_cnf',':bid' => $tblDet['btc_id'],':r' => 'R',':oper' => 'insert'));
							
							$this->db->callQuery("SELECT ifnull(MAX(waitlist_priority),0) FROM slt_enrollment WHERE class_id =:cid and reg_status = :wtl ",array(':cid'=> $cid, ':wtl'=> 'lrn_crs_reg_wtl'));
							$max_wtl_count = $this->db->fetchColumn();
							expDebug::dPrint('max waitlist count 2 '. $max_wtl_count,4);
							$updqry1 = "SET @wtl:=".$max_wtl_count.";
							update ".$tblDet['tblname']."
							set waitlist_priority=@wtl:=@wtl+1 , waitlist_flag = :wtlsts 
							where reg_status = :wtlsts and class_id = :cid ";
							expDebug::dPrintDBAPI("Update waitlist priority in temp table 2 ",$updqry1,array(':cid'=> $cid,':wtlsts' => 'lrn_crs_reg_wtl'));
							$this->db->callExecute($updqry1,array(':cid'=> $cid,':wtlsts' => 'lrn_crs_reg_wtl'));
							
							$range = ($avail_seats + $avail_wtl_seats) .','.$batch_size;
						}
						//$updQry = "update ".$tblDet['tblname']." set record_status= :val, concat(ifnull(custom4,''),:err ) where class_id = :cid and record_status = :r and batch_id = :bid and operation = :oper LIMIT $range";
						$updQry = "update ".$tblDet['tblname']." t22,
						(select t2.mapping_id from ".$tblDet['tblname']." t2
						where t2.class_id = :cid and t2.record_status = :r and t2.batch_id = :bid and t2.operation = :oper LIMIT $range)x
						set t22.record_status= :val , t22.custom4 = concat(ifnull(t22.custom4,''),:err )
						where
						t22.mapping_id = x.mapping_id";
						expDebug::dPrintDBAPI("Update Max seat full in temp table 2  if part ",$updQry,array(':cid'=> $cid, ':val'=> 'IN', ':err'=> 'Max seats Full.',':bid' => $tblDet['btc_id'],':oper' => 'insert',':r' => 'R'));
						$this->db->callExecute($updQry,array(':cid'=> $cid, ':val'=> 'IN', ':err'=> 'Max seats Full.',':bid' => $tblDet['btc_id'],':oper' => 'insert',':r' => 'R'));
					}
				}else{
					$limit = '0,'.$batch_size;
					$avail_wtl_seats = $class_details[0]->waitlist_count - $waitlist_seats + $waitlist_cancel;
					if($avail_wtl_seats >0){
						//$updQry = "update ".$tblDet['tblname']." set reg_status= :sts where class_id = :cid and reg_status = :prevsts and record_status = :r and batch_id = :bid and operation = :oper LIMIT $avail_wtl_seats";
						$updQry = "update ".$tblDet['tblname']." t22,
						(select t2.mapping_id from ".$tblDet['tblname']." t2
						where t2.class_id = :cid and t2.reg_status = :prevsts and t2.record_status = :r and t2.batch_id = :bid and t2.operation = :oper LIMIT $avail_wtl_seats)x
						set t22.reg_status= :sts ,
						t22.comp_status = :nul ,
						t22.notifyuser = :wtl 
						where
						t22.mapping_id = x.mapping_id";
						expDebug::dPrintDBAPI("Update waitlist seat in temp table 2 ",$updQry,array(':cid'=> $cid, ':sts'=> 'lrn_crs_reg_wtl',':nul'=> NULL, ':wtl' => 'Waitlist',':prevsts' => 'lrn_crs_reg_cnf',':bid' => $tblDet['btc_id'],':oper' => 'insert',':r' => 'R'));
						$this->db->callExecute($updQry,array(':cid'=> $cid, ':sts'=> 'lrn_crs_reg_wtl',':nul'=> NULL, ':wtl' => 'Waitlist', ':prevsts' => 'lrn_crs_reg_cnf',':bid' => $tblDet['btc_id'],':oper' => 'insert',':r' => 'R'));
						
						//Waitlist priority Update
						
						$this->db->callQuery("SELECT ifnull(MAX(waitlist_priority),0) FROM slt_enrollment WHERE class_id =:cid and reg_status = :wtl ",array(':cid'=> $cid, ':wtl'=> 'lrn_crs_reg_wtl'));
						$max_wtl_count = $this->db->fetchColumn();
						expDebug::dPrint('max waitlist count 3 '. $max_wtl_count,4);
						$updqry1 = "SET @wtl:=".$max_wtl_count.";
						update ".$tblDet['tblname']."
						set waitlist_priority=@wtl:=@wtl+1, waitlist_flag = :wtlsts 
						where reg_status = :wtlsts and class_id = :cid ";
						expDebug::dPrintDBAPI("Update waitlist priority in temp table 2 ",$updqry1,array(':cid'=> $cid,':wtlsts' => 'lrn_crs_reg_wtl'));
						$this->db->callExecute($updqry1,array(':cid'=> $cid,':wtlsts' => 'lrn_crs_reg_wtl'));
						
						$limit = $avail_wtl_seats.','.$batch_size;
					}
					//$updQry = "update ".$tblDet['tblname']." set record_status= :val, concat(ifnull(custom4,''),:err ) where class_id = :cid and record_status = :r and batch_id = :bid and operation = :oper LIMIT $limit";
					$updQry = "update ".$tblDet['tblname']." t22,
					(select t2.mapping_id from ".$tblDet['tblname']." t2
					where t2.class_id = :cid and t2.record_status = :r and t2.batch_id = :bid and t2.operation = :oper LIMIT $limit)x
					set t22.record_status= :val , t22.custom4 = concat(ifnull(t22.custom4,''),:err )
					where
					t22.mapping_id = x.mapping_id";
					expDebug::dPrintDBAPI("Update Max seat full in temp table 2  else part ",$updQry,array(':cid'=> $cid, ':val'=> 'IN', ':err'=> 'Max seats Full.',':bid' => $tblDet['btc_id'],':oper' => 'insert',':r' => 'R'));
					$this->db->callExecute($updQry,array(':cid'=> $cid, ':val'=> 'IN', ':err'=> 'Max seats Full.',':bid' => $tblDet['btc_id'],':oper' => 'insert',':r' => 'R'));
					
				} 
				
			}
		}catch(Exception $e){
			expDebug::dPrint("ERROR in seat allowcation ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function tpRollup(){
		/* 54622 - Start Complete all classes in TP then automatically TP should be moved to completed status */
		try{
			$tblDet = $this->getTableDetails();
	 		$update = array();
	 		$update['table'] = 'slt_enrollment enr';
	 		$update['fields'] = array(
	 				'enr.comp_status' => 'tmp.comp_status',
    	 		    'enr.comp_date' => "IF(DATE_FORMAT(enr.comp_date,'%Y-%m-%d') < DATE_FORMAT(tmp.comp_date,'%Y-%m-%d'), tmp.comp_date, ifnull(enr.comp_date,NOW()))",
    	 		    'enr.comp_on' => 'tmp.comp_date',
	 		        'enr.updated_by' => $tblDet['user_id'],
    	 		    'enr.comp_by' => $tblDet['user_id'],
	 				'enr.progress' => 'tmp.progress'
	 		);
	 		$update['condition']=array(
	 			//	"enr.id = x.id",
	 				"enr.comp_status IN (:comp_status1, :comp_status2)",
	 				"enr.master_enrollment_id IS NOT NULL",
	 				"tmp.record_status = :r ",
    	 		    "tmp.reg_status != :cansts "
	 		);
	 		$update['joins'] = array(
	 			'INNER JOIN '.$tblDet['tblname'].' tmp ON enr.user_id=tmp.user_id and enr.course_id=tmp.course_id and enr.class_id=tmp.class_id'
	 		);
	 		$arg=array(
	 				':comp_status1' => 'lrn_crs_cmp_enr',
	 				':comp_status2' => 'lrn_crs_cmp_inp',
	 				//':comp_status3' => 'lrn_crs_cmp_nsw',
	 				//':comp_status4' => 'lrn_crs_cmp_inc',
	 				//':comp_status5' => 'lrn_crs_cmp_cmp',
	 				//':comp_status6' => 'lrn_crs_cmp_att',
	 				//':comp_status7' => 'lrn_crs_cmp_exp',
	 				':r' => 'R',
    	 		    ':cansts' => 'lrn_crs_reg_can'
	 		);
	 		 
	 		$updQry1 = $this->db->prepareQueryUpdate($update['table'],$update['fields'],$update['condition'],$update['joins']);
	 		expDebug::dPrintDBAPI("Query to mark complete TP classes ",$updQry1,$arg);
	 		$this->db->callExecute($updQry1,$arg);
	 		 
	 		$joins= array('INNER JOIN slt_enrollment se on se.master_enrollment_id = sme.id INNER JOIN '.$tblDet['tblname'].' tmp on (se.user_id=tmp.user_id AND se.course_id=tmp.course_id AND se.class_id=tmp.class_id)');
	 		$fields = array(
	 				"sme.user_id",
	 				"sme.id",
	 				"sme.program_id",
	 				"se.updated_by"
	 		);
	 		 
	 		$conditons =array(
	 				"tmp.record_status = :r ",
	 				"se.master_enrollment_id > 0",
	 				"tmp.comp_status IN (:comp_status1, :comp_status2, :comp_status3, :comp_status4, :comp_status5)"
	 		);
	 		$args=array(
	 				':comp_status1' => 'lrn_crs_cmp_enr',
	 				':comp_status2' => 'lrn_crs_cmp_inp',
	 				':comp_status3' => 'lrn_crs_cmp_nsw',
	 				':comp_status4' => 'lrn_crs_cmp_inc',
	 				':comp_status5' => 'lrn_crs_cmp_cmp',
	 				':r' => 'R'
	 		);
	 		 
	 		$groupBy = "GROUP BY sme.id";
	 		$query = $this->db->prepareQuerySelect('slt_master_enrollment sme',$fields,$conditons,$joins,$groupBy);
	 		expDebug::dPrintDBAPI("Query to fetech TP rollup records ",$query,$arg);
	 		$this->db->callQuery($query,$args);
	 		$mstr_enroll_dtl = $this->db->fetchAllResults();
	 		expDebug::dPrint('$mstr_enroll_id test :'.print_r($mstr_enroll_dtl, 1), 5);
	 		 
	 		 
	 		foreach($mstr_enroll_dtl as $record){
	 			/*
	 			$qry = "CALL slp_mark_program_complete('".$record->user_id."', '".$record->id."', ".$record->program_id.", ".$record->updated_by.", '', '0')";
	 			expDebug::dPrint('$qry procedure :'.$qry, 5);
	 			$this->db->callQuery($qry);
	 			*/
	 			lp_mark_program_complete($record->user_id, $record->id, $record->program_id, $record->updated_by,'','0');	 			
	 		}
		}catch(Exception $e){
			expDebug::dPrint("ERROR in TP rollup ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	protected function insertAuditTrail(){
		try{
			$tblDet = $this->getTableDetails();
			$insert = array();
			$insert['table'] = 'slt_audit_trail';
			$insert['fields'] = array('logged_user_id','entity_id','entity_type','module_name','functionality_name','logged_user_action','mod_user_id',
																'new_value','esign_date_time','timezone','created_on','dataload_by');
	
			$select = array();
			$select['table'] = "slt_enrollment enr";
			$select['fields']=  array(':by ','enr.class_id','\'cre_sys_obt_cls\'','\'exp_sp_learning\'','\'enrollUserToClass\'','if(t2.operation = \'insert\',\'Inserted Enrolled_id\',\'Updated Enrolled_id\')','enr.user_id','enr.id', 
																'CONVERT_TZ(CONCAT(DATE_FORMAT(CURDATE(), _utf8\'%Y-%m-%d\'),\' \',CURTIME()) ,(SELECT pro1.attr2 FROM slt_profile_list_items pro1 WHERE pro1.code= per.time_zone), spli.attr2) ','spli.code','now()',':loadby ');
			$select['joins']= array("INNER JOIN ".$tblDet['tblname']." t2 ON enr.user_id = t2.user_id and enr.class_id =t2.class_id and t2.record_status = :r and t2.batch_id = :btid ",
															"INNER JOIN slt_person per ON per.id = enr.user_id ",
															"LEFT JOIN slt_profile_list_items spli ON spli.code = per.time_zone"
														 );
														
			$select['condition'] = array(
				'enr.custom_dataload = :btid ',
				'enr.dataload_by = :loadby ',
				't2.record_status = :r '
			);
			$arg=array(
			':btid' => $tblDet['btc_id'],
			':loadby' => $tblDet['loadby'],
			':r' => 'R',
			':by'=> 1,
			);
	
			$query = $this->db->prepareInsertBySelect($insert,$select);
			expDebug::dPrintDBAPI("Query for Audit trail entry insert bulk execute", $query,$arg);
			$this->db->callExecute($query,$arg);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in insert audit trail ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	protected function insertUserPoints(){
		try{
			$moduleSts = $this->getModuleStatus('exp_sp_admin_userpoints');
			if($moduleSts == 0) return '';
			
			$tblDet = $this->getTableDetails();
			$insert = array();
			$insert['table'] = 'slt_user_points';
			$insert['fields'] = array('user_id','entity_id','entity_type','action_code','operation_flag','earned_points','total_points',
																'created_by','created_on','updated_by','updated_on','dataload_by');
	
			$select = array();
			$select['table'] = "slt_enrollment enr";
			$select['fields']=  array('enr.user_id','enr.id','\'class\'','msp.code','\'insert\'','msp.points','msp.points',':by ', 
																'now()',':by ','now()',':loadby ');
			// Added by Vincent on 11 June, 2016. when auto enroll the primary id is not null 
			// whereas in roster upload it is null. hence this conditon is added
			// TODO: need to check with Shobana why this missleading in both the cause
			//$con = stripos($tblDet['tblname'],'temp2_job') !== false ? "t2.primary_id is null" : "t2.primary_id is not null";
			$select['joins']= array("INNER JOIN ".$tblDet['tblname']." t2 ON t2.user_id = enr.user_id 
															 and t2.class_id = enr.class_id and enr.master_enrollment_id is null and t2.record_status=:r ",
															'LEFT JOIN slt_master_points msp ON msp.code = IF(enr.comp_status = :cmpsts , :mspcmp , :mspreg )',
														 );
														
			$select['condition'] = array(
				'enr.custom_dataload = :btid ',
				'enr.dataload_by = :loadby ',
				'enr.reg_status != :cansts ',
				'enr.comp_status != :incsts ',
				't2.record_status = :r '
			);
			$arg=array(
			':btid' => $tblDet['btc_id'],
			':cansts' => 'lrn_crs_reg_can',
			':incsts' => 'lrn_crs_cmp_inc',
			':cmpsts' => 'lrn_crs_cmp_cmp',
			':mspcmp' => 'complete_class_training',
			':mspreg' => 'register_class',
			//':ins' => 'insert',
			':r' => 'R',
			':by'=> $tblDet['user_id'],
			':loadby'=> $tblDet['loadby']
			);
	
			$query = $this->db->prepareInsertBySelect($insert,$select);
			expDebug::dPrintDBAPI("Query for User points entry for insert bulk execute", $query,$arg);
			$this->db->callExecute($query,$arg);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in insert user points ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function updateUserPoint(){
		try{
			$moduleSts = $this->getModuleStatus('exp_sp_admin_userpoints');
			if($moduleSts == 0) return '';
			
			$tblDet = $this->getTableDetails();
			$update = array();
			$update['table'] = 'slt_user_points upt';
			$update['fields'] = array(
					'upt.operation_flag'=>'\'delete\'',
					'upt.total_points' => 0,
					'upt.updated_by' => 'enr.user_id',
					'upt.updated_on' => 'now()'
			);
			$update['condition']=array(
					"upt.action_code = :regsts ",
				//	"upt.entity_id = x.entid",
				//	"upt.user_id = x.uid",
					"upt.operation_flag = :ins",
					"upt.entity_type = :cls " 
			);
			$update['joins'] = array(
				'INNER JOIN slt_enrollment enr ON enr.id = upt.entity_id AND enr.user_id = upt.user_id ',
				"INNER JOIN ".$tblDet['tblname']." t2 ON t2.primary_id = enr.id and t2.operation= :upd and t2.reg_status = 'lrn_crs_reg_can'"
			);
			$arg=array(':upd' => 'update',
								 ':regsts' => 'register_class',
								 ':ins' => 'insert',
								 ':cls' => 'class'
								);
					
			$updQry1 = $this->db->prepareQueryUpdate($update['table'],$update['fields'],$update['condition'],$update['joins']);
			expDebug::dPrintDBAPI(" updQry1 ",$updQry1,$arg);
			$this->db->callExecute($updQry1,$arg);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in update user points ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	protected function insertNotification(){
		try{
			$tblDet = $this->getTableDetails();
			$util = new GlobalUtil();
			$config = $util->getConfig();
			$notifyParam    = $config["send_notification"];
			if($notifyParam == 1){
				
				$colNm = 't2.class_id as class_id ,t2.course_id as course_id,t2.notifyuser as notifystring,t2.custom3 as deltype,t2.is_compliance as is_comp,enr.recertify_path as module_seq ';
				$grpBy = 'group by t2.notifyuser,t2.custom3,t2.class_id,t2.course_id ';
		 		$unionQry = 'SELECT '.$colNm.' from '.$tblDet['tblname'].' t2 LEFT JOIN slt_enrollment enr on enr.order_id = t2.order_id WHERE t2.batch_id =  :btid  and t2.record_status =  :r AND (t2.comp_status !=:nsw OR t2.comp_status is null)'. $grpBy;
		 		$args=array(
		 				':btid' => $tblDet['btc_id'],
		 				':r' => 'R',
		 				':nsw' =>'lrn_crs_cmp_nsw',
		 		);
		 		$this->db->callExecute($unionQry,$args);
		 		expDebug::dPrintDBAPI('Query for generating records for Notification insert',$unionQry,$args);
		 		$notDetails = $this->db->fetchAllResults();
		 						
		 		$args = array();
			 		$conditons = array();
			 		$fields = array(
			 				'ninfo.id',
			 				'ninfo.notification_code',
			 				'ninfo.lang_code',
			 				'ninfo.locale_code',
			 				'ninfo.profile_code',
			 				'ninfo.notification_sendto',
			 				'ninfo.entity_type',
			 				'ninfo.notification_cc',
			 				'ninfo.notification_admin',
			 				'ninfo.notification_title',
			 				'ninfo.notification_description',
			 				'ninfo.notification_type',
			 				'ninfo.frequency_data_type',
			 				'ninfo.frequency_data_value',
			 				'ninfo.status',
			 				'group_concat("\'",frame.lang_code,"\'") as frame_lang'
			 		);
			 		$join = array('left join slt_notification_frame frame on frame.notification_id = ninfo.id');
			 		$conditons[] = ' ninfo.lang_code = :langcode ';
			 		$conditons[] = " ninfo.notification_code IN ('register_wbt_by_admin','register_vc_by_admin','register_by_admin','class_cancel','class_cancel_wbt_vod','course_completed','compliance_class_incomplete','register_mandatory_compliance_by_admin','compliance_register_vc_by_admin','compliance_register_wbt_by_admin','compliance_course_re_register_by_admin','man_comp_multiple_class_register') ";
			 		$args[':langcode']= 'cre_sys_lng_eng';
			 		$others = "group by ninfo.id";
			 		
			 		// Prepare query
			 		$query = $this->db->prepareQuerySelect('slt_notification_info ninfo',$fields,$conditons,$join,$others);
			 		expDebug::dPrintDBAPI("Get Notification details",$query,$args);
			 		// Execute query
			 		$this->db->callQuery($query,$args);
			 		// Fetch results
			 		$NotInfoDetails = $this->db->fetchAllResults(); 
			 		expDebug::dPrint("testing the value for notification details".print_r($NotInfoDetails,true),5);
			 		//$notDetArr = array();
			 		foreach($NotInfoDetails as $key => $value){
			 			expDebug::dPrint("Notification details key".$key.'values'.print_r($value,1),4);
			 			$notDetArr[$value->notification_code] = $value;
			 			expDebug::dPrint("notification multidimentional array".print_r($notDetArr,1),5);
			 		}

			 	foreach ($notDetails as $res => $val ){
			 			expDebug::dPrint("Fetch the result of notification".print_r($val,1),4);
			 			$courseId = $val->course_id;
			 			$classId = $val->class_id;
			 			$delType = $val->deltype;
			 			$iscomp = $val->is_comp;
			 			$notifyString = $val->notifystring;
			 			expDebug::dPrint("Passed Notify String ".$notifyString,4);
			 			
			 			//fix for issue #0059935;
			 			if($notifyString == 'Waitlist')
			 				continue;
			 				
			 			if($iscomp == 1 && $notifyString == 'complianceregisteredcomplete'){
			 					$regNot = ($delType== 'lrn_cls_dty_ilt') ? 'register_mandatory_compliance_by_admin' : (($delType== 'lrn_cls_dty_vcl') ? 'compliance_register_vc_by_admin' : 'compliance_register_wbt_by_admin');
			 					$this->enrollmentNotificationInsert($regNot, $courseId, $classId,  1,$notDetArr,$notifyString);
			 					$this->enrollmentNotificationInsert('course_completed', $courseId, $classId, 1,$notDetArr,$notifyString);
			 				}else if($iscomp == 1 && $notifyString == 'complianceregisteredcancel'){
			 					$regNot = ($delType== 'lrn_cls_dty_ilt') ? 'register_mandatory_compliance_by_admin' : (($delType== 'lrn_cls_dty_vcl') ? 'compliance_register_vc_by_admin' : 'compliance_register_wbt_by_admin');
			 					$canNot = ($delType== 'lrn_cls_dty_ilt' || $delType == 'lrn_cls_dty_vcl') ? 'class_cancel' : 'class_cancel_wbt_vod';
			 					$this->enrollmentNotificationInsert($regNot, $courseId, $classId, 1,$notDetArr,$notifyString);
			 					$this->enrollmentNotificationInsert($canNot, $courseId, $classId, 1,$notDetArr,$notifyString);
			 				}else if($iscomp == 1 && ($notifyString == 'complianceregisteredincomplete' || $notifyString == 'ComplianceClassIncomplete')){
			 					$regNot = ($delType== 'lrn_cls_dty_ilt') ? 'register_mandatory_compliance_by_admin' : (($delType== 'lrn_cls_dty_vcl') ? 'compliance_register_vc_by_admin' : 'compliance_register_wbt_by_admin');
			 					if($notifyString == 'complianceregisteredincomplete')
			 					  $this->enrollmentNotificationInsert($regNot, $courseId, $classId, 1,$notDetArr,$notifyString);
			 					
			 					$this->enrollmentNotificationInsert('compliance_class_incomplete', $courseId, $classId, 1,$notDetArr,$notifyString);
			 				}	else if($notifyString == 'registeredcomplete'){
			 					$regNot = ($delType == 'lrn_cls_dty_vcl') ? 'register_vc_by_admin' : (($delType== 'lrn_cls_dty_ilt' ) ? 'register_by_admin' : 'register_wbt_by_admin');
			 					$this->enrollmentNotificationInsert($regNot, $courseId, $classId, 1,$notDetArr,$notifyString);
			 					$this->enrollmentNotificationInsert('course_completed', $courseId, $classId, 1,$notDetArr,$notifyString);
			 				}else if($notifyString == 'registeredcancel'){
			 					$canNot = ($delType== 'lrn_cls_dty_ilt' || $delType == 'lrn_cls_dty_vcl') ? 'class_cancel' : 'class_cancel_wbt_vod';
			 					$regNot = ($delType == 'lrn_cls_dty_vcl') ? 'register_vc_by_admin' : (($delType== 'lrn_cls_dty_ilt' ) ? 'register_by_admin' : 'register_wbt_by_admin');
			 					$this->enrollmentNotificationInsert($regNot, $courseId, $classId, 1,$notDetArr,$notifyString);
			 					$this->enrollmentNotificationInsert($canNot, $courseId, $classId, 1,$notDetArr,$notifyString);
			 				}else if($notifyString == 'WBTCancel' || $notifyString == 'Cancel'){
			 					$canNot = ($delType== 'lrn_cls_dty_ilt' || $delType == 'lrn_cls_dty_vcl') ? 'class_cancel' : 'class_cancel_wbt_vod';
			 					$this->enrollmentNotificationInsert($canNot, $courseId, $classId, 1,$notDetArr,$notifyString);
			 				}else if($notifyString == 'MultipleMandatoryClassNotification' || $notifyString == 'MultipleMandatoryClassNotification'){
			 					$this->enrollmentNotificationInsert('man_comp_multiple_class_register', $courseId, $classId, 1,$notDetArr,$notifyString);
			 				}
			 				else{
			 					if($notifyString == 'Register')
			 						$not = ($delType == 'lrn_cls_dty_vcl') ? 'register_vc_by_admin' : (($delType== 'lrn_cls_dty_ilt' ) ? 'register_by_admin' : 'register_wbt_by_admin');
			 					else if($notifyString == 'Completed')
			 						$not = 'course_completed';
			 					else 
			 						$not = ($delType== 'lrn_cls_dty_ilt') ? 'register_mandatory_compliance_by_admin' : (($delType== 'lrn_cls_dty_vcl') ? 'compliance_register_vc_by_admin' : 'compliance_register_wbt_by_admin');
			 					$this->enrollmentNotificationInsert($not, $courseId, $classId, 1,$notDetArr,$notifyString);
			 				}
			 		}
			}
		}catch(Exception $e){
			expDebug::dPrint("ERROR in insert notification ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function resetOrderTableEntry(){
		try{
			$tblDet = $this->getTableDetails();
			$updqry =	" Update slt_order set custom_dataload = NULL where custom_dataload IS NOT NULL and dataload_by = :loadby  ";
			$arg=array(
				//':bid'=>$tblDet['btc_id'],// Modified by Vincent on 10 June, 2016 for #0065353
				':loadby'=>$tblDet['loadby'],
			);
			expDebug::dPrintDBAPI('Update query for custom column in slt_order ',$updqry,$arg);
			$this->db->callExecute($updqry,$arg);
				
	 		// Moved from validate to here by Vincent.
	 		//Update temp table 1
			list($fields, $args,$join,$condition) = array(array(),array(),array(),array());
			$spTables = explode('_',$tblDet['tblname']);
			$fields=array(
			 		't11.remarks' => 't2.custom4',
			 		't11.record_status' => 't2.record_status'
			);
			$condition=array(
			 		't2.batch_id = :batch_id '
			);
			$join=array(
			 		'inner join '.$tblDet['tblname'].' t2 on t2.mapping_id =t11.mapping_id',
			);
			$arg = array(
					':batch_id' => $tblDet['btc_id']
			);
			$qry1 = $this->db->prepareQueryUpdate('temp1_'.$spTables[1]." t11",$fields,$condition,$join);
			expDebug::dPrintDBAPI(" Source table qry1 ",$qry1,$arg);
			$this->db->callExecute($qry1,$arg);
			
			//Update temp table 2
			list($fields, $args,$join,$condition) = array(array(),array(),array(),array());
			$spTables = explode('_',$tblDet['tblname']);
			$fields=array(
					't11.custom4' => 't2.custom4',
					't11.record_status' => 't2.record_status'
			);
			$condition=array(
					't2.batch_id = :batch_id '
			);
			$join=array(
					'inner join '.$tblDet['tblname'].' t2 on t2.mapping_id =t11.mapping_id',
			);
			$arg = array(
					':batch_id' => $tblDet['btc_id']
			);
			$qry1 = $this->db->prepareQueryUpdate('temp2_'.$spTables[1]." t11",$fields,$condition,$join);
			expDebug::dPrintDBAPI(" Source table qry1 ",$qry1,$arg);
			$this->db->callExecute($qry1,$arg);
			
			 	
			$qry = "update ".$tblDet['tblname']." set custom4 = null where batch_id = ".$tblDet['btc_id'];
			$this->db->callExecute($qry);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in reset order table entry ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function setBatchRecords($batchrecords){
		$this->batchrecords = $batchrecords;
	}
	
	private function getBatchRecords(){
		return $this->batchrecords;
	}
	
	protected function setJobDetails($jobDetail){
		$this->jobDetail = $jobDetail;
	}
	
	private function getJobDetails(){
		return $this->jobDetail;
	}
	/**
	 * @desc Set table details to a local variable
	 * @param $tblDet
	 * @return unknown_type
	 */
	protected function setTableDetails($tblDet){
		$this->tblDet = $tblDet;
	}
	
	private function getTableDetails(){
		return $this->tblDet;
	}
	
	protected function enrollmentNotificationInsert($notStr, $courseId, $classId='', $updByUsrId,$notDetArr,$notifyString){
	 	try{
			// Get the Override info for each Class.
		 	expDebug::dPrint("Print the function enrollmentNotificationInsert".$notifyString." --- code-- ".$notStr,4);
		 	
		 	$overrideDetails = $this->getEnrollmentNotificationOverrideInfo($classId,'cre_sys_obt_cls',$notStr,'Email');
		 	
		 	$notifyFlag = null;
		 	$emailSendTo = null;
		 	$ccEmail = null;
		 	if ($notDetArr[$notStr] != null) {
		 		$notifyFlag = $notDetArr[$notStr]->status;
		 		$emailSendTo = $notDetArr[$notStr]->notification_sendto;
		 		$ccEmail = $notDetArr[$notStr]->notification_cc;
		 		 	
		 		if ($notifyFlag == 'cre_ntn_sts_itv') {
		 			expDebug::dPrint('Notification is disabled',4);
		 			return;
		 		}
		 		if(!empty($ccEmail))
		 			$mailIdWithCc = $emailSendTo.",".$ccEmail;
		 		else
		 			$mailIdWithCc =  $emailSendTo;
		 		
		 		expDebug::dPrint(' $mailIdWithCc = ' . print_r($mailIdWithCc, true) , 4);
		 		$explodeMailIdWithCc = array_filter(array_unique(explode(",", $mailIdWithCc )));
		 		expDebug::dPrint('$explodeMailIdWithCc =' . print_r($explodeMailIdWithCc, true) , 4);
	 	
		 	}
		 	if($notStr == 'cert_curr_lp_register_by_admin' || $notStr == 'cert_curr_lp_recertify_by_admin'  || $notStr == 'cert_curr_lp_completed' || $notStr == 'cert_curr_lp_waitlist_register_by_admin'){
		 		// To find moduleseq
		 		$moduleseq = $notDetArr[$notStr]->module_seq;
		 		expDebug::dPrint("Print the function enrollmentNotificationInsert 5 ".print_r($moduleseq,true)." --- user-- ".print_r($classId,true),4);
		 	}
			$notTokenStr = $this->generateNotificationsTokenString($notStr,$courseId, $classId,$moduleseq);
		
			$tblDet = $this->getTableDetails();
			$notVal = $notDetArr[$notStr];
			expDebug::dPrint("Values from notification array".print_r($notVal->notification_sendto,1),4);
			expDebug::dPrint("Testing the Notification token string values".print_r($notTokenStr,1),4);
			$emailSendTo = explode(',', $notVal->notification_sendto);
			if ((in_array("cre_ntn_rpt_adm", $emailSendTo)) && (!empty($explodeMailIdWithCc))) {
				list($fields, $args) = array(array(),array());
				$fields = array(':msg_id ', "if(p.preferred_language IN (".$notVal->frame_lang."), p.preferred_language, :lngcode )", ':msgtype ', "CONCAT(".$notTokenStr['tokens_string'].")", 'temp.user_id', 'temp.drup_uid',
					'p.full_name', 'temp.user_email', ':nullval ', ':sendtype ', ':sndsts ', ':ovrdcnt ', ':updusr ', 'now()', ':updusr ', 'now()',':cus0 ',':loadby ');
				$args = array(
						':msg_id' => $notTokenStr['message_id'],
						':lngcode' => 'cre_sys_lng_eng',
						':msgtype' => $notTokenStr['message_type'],
						':sendtype' => $notTokenStr['send_type'],
						':ovrdcnt' => $overrideDetails['notify_text'],
						':sndsts' => 'N',
						':nullval' => NULL,
						':updusr' => $tblDet['user_id'],
						':cus0' => 1,
						':atv' => 'cre_usr_sts_atv',
						':recsts' => 'R',
						':btid' => $tblDet['btc_id'],
						':loadby' => $tblDet['loadby'],
				    	':ntype' => $notifyString,
						':mnrec' => 'MN',
						':class_id' => $classId
						
				);
				$this->insertEnrollmentNotification($fields,$args,$notifyString);
				
			} 
			
			if ((in_array("cre_ntn_rpt_usr", $emailSendTo)) && (!empty($explodeMailIdWithCc))) {
				list($fields, $args) = array(array(),array());
				$fields = array(':msg_id ', "if(p.preferred_language IN (".$notVal->frame_lang."), p.preferred_language, :lngcode )", ':msgtype ', "CONCAT(".$notTokenStr['tokens_string'].")", 'temp.user_id', 'temp.drup_uid',
						'p.full_name', 'temp.user_email', ':nullval ', ':sendtype ', ':sndsts ', ':ovrdcnt ', ':updusr ', 'now()', ':updusr ', 'now()', ':nullval ',':loadby ');
				$args = array(
						':msg_id' => $notTokenStr['message_id'],
						':lngcode' => 'cre_sys_lng_eng',
						':msgtype' => $notTokenStr['message_type'],
						':sendtype' => 'php mailer',
						':ovrdcnt' => $overrideDetails['notify_text'],
						':sndsts' => 'N',
						':nullval' => NULL,
						':updusr' => $tblDet['user_id'],
						':atv' => 'cre_usr_sts_atv',
						':recsts' => 'R',
						':btid' => $tblDet['btc_id'],
						':loadby' => $tblDet['loadby'],
				    	':ntype' => $notifyString,
						':mnrec' => 'MN',
						':class_id' => $classId
				);
				$this->insertEnrollmentNotification($fields,$args,$notifyString);
				
			  }
			  if ((in_array("cre_ntn_rpt_mgr", $emailSendTo)) && (!empty($explodeMailIdWithCc))) {
		    	list($fields, $args) = array(array(),array());
		    	$fields = array(':msg_id ', "if(p.preferred_language IN (".$notVal->frame_lang."), p.preferred_language, :lngcode )", ':msgtype ', "CONCAT(".$notTokenStr['tokens_string'].")", 'temp.mgr_uid', 'temp.mgr_drup_uid',
		    				'p.full_name', 'temp.manager_email', ':nullval ', ':sendtype ', ':sndsts ', ':ovrdcnt ', ':updusr ', 'now()', ':updusr ', 'now()', ':nullval ',':loadby ');
		    	$args = array(
		    				':msg_id' => $notTokenStr['message_id'],
		    				':lngcode' => 'cre_sys_lng_eng',
		    				':msgtype' => $notTokenStr['message_type'],
		    				':sendtype' => 'php mailer',
		    				':ovrdcnt' => $overrideDetails['notify_text'],
		    				':sndsts' => 'N',
		    				':nullval' => NULL,
		    				':updusr' => $tblDet['user_id'],
		    				':atv' => 'cre_usr_sts_atv',
		    				':recsts' => 'R',
		    				':btid' => $tblDet['btc_id'],
		    				':loadby' => $tblDet['loadby'],
		    				':ntype' => $notifyString,
							':mnrec' => 'MN',
		    				':class_id' => $classId
		    	);
		    	$this->insertEnrollmentNotification($fields,$args,$notifyString);
		    }
			    
		    if((in_array("cre_ntn_rpt_ins", $emailSendTo) && $sendStatus) && (!empty($explodeMailIdWithCc))) {
		    	list($fields, $args) = array(array(),array());
					$fields = array(':msg_id ', "if(p.preferred_language IN (".$notVal->frame_lang."), p.preferred_language, :lngcode )", ':msgtype ', "CONCAT(".$notTokenStr['tokens_string'].")", 'temp.user_id', 'temp.drup_uid',
							'p.full_name', 'temp.user_email', ':nullval ', ':sendtype ', ':sndsts ', ':ovrdcnt ', ':updusr ', 'now()', ':updusr ', 'now()', ':nullval ',':loadby ');
					$args = array(
							':msg_id' => $notTokenStr['message_id'],
							':lngcode' => 'cre_sys_lng_eng',
							':msgtype' => $notTokenStr['message_type'],
							':sendtype' => 'php mailer',
							':ovrdcnt' => $overrideDetails['notify_text'],
							':sndsts' => 'N',
							':nullval' => NULL,
							':updusr' => $tblDet['user_id'],
							':atv' => 'cre_usr_sts_atv',
							':recsts' => 'R',
							':btid' => $tblDet['btc_id'],
							':loadby' => $tblDet['loadby'],
							':ntype' => $notifyString,
							':mnrec' => 'MN',
							':class_id' => $classId
					);
					$this->insertEnrollmentNotification($fields,$args,$notifyString);
		    }
			    
		     if(!empty($ccEmail) && (!empty($explodeMailIdWithCc))) {
		    	list($fields, $args) = array(array(),array());
					$fields = array(':msg_id ', "if(p.preferred_language IN (".$notVal->frame_lang."), p.preferred_language, :lngcode )", ':msgtype ', "CONCAT(".$notTokenStr['tokens_string'].")", 'temp.user_id', 'temp.drup_uid',
							'p.full_name', ':sndtoml ', ':nullval ', ':sendtype ', ':sndsts ', ':ovrdcnt ', ':updusr ', 'now()', ':updusr ', 'now()', ':nullval ',':loadby ');
					$args = array(
							':msg_id' => $notTokenStr['message_id'],
							':lngcode' => 'cre_sys_lng_eng',
							':msgtype' => $notTokenStr['message_type'],
							':sendtype' => 'php mailer',
							':ovrdcnt' => $overrideDetails['notify_text'],
							':sndsts' => 'N',
							':nullval' => NULL,
							':updusr' => $tblDet['user_id'],
							':sndtoml' => $ccEmail,
							':atv' => 'cre_usr_sts_atv',
							':recsts' => 'R',
							':btid' => $tblDet['btc_id'],
							':loadby' => $tblDet['loadby'],
							':ntype' => $notifyString,
							':mnrec' => 'MN',
							':class_id' => $classId
					);
					$this->insertEnrollmentNotification($fields,$args,$notifyString);
		    }
	 	}catch(Exception $e){
	 		expDebug::dPrint("ERROR in enrollmentNotificationInsert ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}  
	}

	private function insertEnrollmentNotification($fields,$args,$notStr){
		try{
			$tblDet = $this->getTableDetails();
			$tbl = $tblDet['tblname'];
			$insert = array();
			$insert['table'] = 'slt_bulk_notification';
			$insert['fields'] = array(
					'msg_id',
					'lang_code',
					'msg_type',
					'token_str',
					'user_id',
					'send_to_id',
					'send_to_name',
					'send_to_email',
					'send_to_email_cc',
					'send_type',
					'send_status',
					'attach_content',
					'created_by',
					'created_on',
					'updated_by',
					'updated_on',
					'custom0',
					'dataload_by');
			
			$select = array();
			
			$select['fields']= $fields;
			
			
			if($tblDet['enr_type'] == 'tp'){
				$enrTbl = $tblDet['clstblname'];
				
				$select['table'] = $tblDet['tblname']." temp";
				
				$select['joins'] = array(' LEFT JOIN slt_master_enrollment enr ON enr.user_id = temp.user_id and enr.dataload_by = :loadby ',
																 ' LEFT JOIN '.$enrTbl.' e ON e.master_enrollment_id = enr.id ',
										 						 ' LEFT JOIN slt_profile_list_items ests ON ests.code = e.comp_status',
																 ' LEFT JOIN slt_profile_list_items enrsts ON enrsts.code = enr.overall_status',
																 ' INNER JOIN slt_program prg ON prg.id = temp.program_id',
																 ' INNER JOIN slt_person p ON p.id = temp.user_id'
																);
				if($notStr == 'certCurrLPRegister'){
					$select['condition'] = array(
							'temp.record_status IN (:recsts , :mnrec )',
							'p.status = :atv ',
							'temp.batch_id =  :btid ',
							'temp.notifyuser IN (:ntype , \'certCurrLPRegisterAndCompleted\')'
					);
				}elseif($notStr == 'certCurrLPWaitlistRegister'){
					$select['condition'] = array(
							'temp.record_status IN (:recsts , :mnrec )',
							'p.status = :atv ',
							'temp.batch_id =  :btid ',
							'temp.notifyuser = :ntype ',
							'e.waitlist_priority IS NOT NULL'
					);
				} else{
					$select['condition'] = array(
							'temp.record_status IN (:recsts , :mnrec )',
							'p.status = :atv ',
							'temp.batch_id =  :btid ',
							'temp.notifyuser = :ntype '
					);
				}
				//if($notStr == 'MultipleMantadoryTPNotification')
					$select['others'] = " GROUP BY temp.id";
			//	else
				//	$select['others'] = " GROUP BY enr.id";
				
				
			} else{
				$select['table'] = $tblDet['tblname']." temp";
				
				$select['joins'] = array(' LEFT JOIN slt_enrollment enr on enr.user_id = temp.user_id and enr.custom_dataload = :btid and enr.dataload_by = :loadby AND enr.order_id = temp.order_id',
						' LEFT JOIN slt_profile_list_items ests ON ests.code = enr.comp_status',
						' INNER JOIN slt_person p on p.id = temp.user_id',
						' INNER JOIN slt_course_template crs on crs.id = temp.course_id',
						' INNER JOIN slt_course_class cls on cls.id = temp.class_id',
					//	' left join slt_course_content_mapper ctnmap on ctnmap.course_id = crs.id and ctnmap.class_id = cls.id'
				);
				$select['condition'] = array(
						'temp.record_status IN (:recsts , :mnrec )',
						'p.status = :atv ',
						'temp.batch_id =  :btid ',
						'temp.notifyuser = :ntype ',
						'temp.class_id = :class_id '
				);
			}
				
			
			
			$arg= $args;
			
			$query = $this->db->prepareInsertBySelect($insert,$select);
			expDebug::dPrintDBAPI("Query for slt_bulk_notification bulk execute ", $query,$arg);
			$this->db->callExecute($query,$arg);
			
		}catch(Exception $e){
			expDebug::dPrint("ERROR in insertEnrollmentNotification ".print_r($e,true),1);
				throw new Exception($e->getMessage());
		}
		
	}

	private function generateNotificationsTokenString($notStr,$crsId,$clsId,$moduleseq){
		try{
			expDebug::dPrint("values with in generateNotificationsTokenString".print_r($notStr,1),4);
			
			if($notStr == 'course_completed')
			  $tokenStr = $this->generateEnrollmentCompletedNotification($notStr);
			else if($notStr == 'class_cancel' || $notStr == 'class_cancel_wbt_vod'){
				$sessInfo = '';
				$instInfo ='';
			  if($notStr == 'class_cancel'){
			  	$sessInfo = $this->getClassSessionInfoForEnrollmentNotifications($crsId,$clsId);
			  	$instInfo = $this->getClassInstructorInfoForEnrollmentNotifications($clsId);
			  }
			  $tokenStr = $this->generateEnrollmentDropAndCancelNotifications($notStr,$sessInfo,$instInfo,$clsId);
			} else if($notStr == 'cert_curr_lp_register_by_admin' || $notStr == 'cert_curr_lp_recertify_by_admin'  || $notStr == 'cert_curr_lp_completed' || $notStr == 'cert_curr_lp_waitlist_register_by_admin'){
				$tokenStr = $this->generateCertCurrLPRegisterNotifications($crsId,$notStr,$moduleseq);
			}else
			  $tokenStr = $this->generateEnrollmentRegisterAndWaitlistNotifications($notStr,$clsId,$crsId);
			
			expDebug::dPrint("Notification token string".print_r($tokenStr,1),4);
			return $tokenStr; 
		}catch(Exception $e){
			expDebug::dPrint("ERROR in generateNotificationTokenString ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function generateEnrollmentRegisterAndWaitlistNotifications($notStr,$clsId,$crsId){
		try{
		$notificationInfo = array();
		$delivery_type = $this->getDeliveryTypeCode($clsId,'Long');
		if($notStr == 'mandatory_tp_multiple_register'){
			$notificationInfo['tokens_string'] =	"'first_name>|',ifnull(p.first_name,''),'~|last_name>|',ifnull(p.last_name,''),'~|delivery_type>| ~|course_title>| ~|class_title>|~',
			if(prg.addn_notification_show = 0,'',if(prg.additional_info IS NOT NULL AND prg.addn_notification_show = 1,concat('|add_notes>|',prg.additional_info,''),'|add_notes>|-')),
			'~|full_name>|',ifnull(p.full_name,''),'~|learning_fullname>|'
			,ifnull(p.full_name,''),'~|user_name>|',ifnull(p.user_name,''),'~|user_email>|',ifnull(p.email,''),'~|user_phone>|',ifnull(p.phone_no,''),'~|waitlist_position>| ~|'";
		}else{
            $notificationInfo['tokens_string'] =	"'first_name>|',ifnull(p.first_name,''),'~|last_name>|',ifnull(p.last_name,''),'~|delivery_type>|'
			,'".$delivery_type."',
			if(cls.addn_notification_show = 0,'',if(cls.additional_info IS NOT NULL AND cls.addn_notification_show = 1,concat('~|add_notes>|',cls.additional_info,''),'~|add_notes>|-')),
			'~|course_title>|',ifnull(crs.title,''),'~|class_title>|',ifnull(cls.title,''),'~|full_name>|',ifnull(p.full_name,''),'~|learning_fullname>|'
			,ifnull(p.full_name,''),'~|user_name>|',ifnull(p.user_name,''),'~|user_email>|',ifnull(p.email,''),'~|user_phone>|',ifnull(p.phone_no,''),'~|waitlist_position>|',ifnull(enr.waitlist_priority,''),'~|',
			if(ifnull(enr.is_compliance,'') = 1,'compliance_mandatory>|Compliance~|',if(ifnull(enr.mandatory,'') = 'Y','compliance_mandatory>|Mandatory~|',''))";
			
			
			$classDeliveryType = $this->getDeliveryTypeCode($clsId,'Short');
			if ($classDeliveryType == 'ilt' || $classDeliveryType == 'vc') {
				// Get class session details
				/* Added extra param = class del type attr*/
				$classSessionInfo = $this->getClassMultiSessionInfoForNotifications($crsId, $clsId,$classDeliveryType);
				$classInstructorInfo  = $this->getClassInstructorInfoForNotifications($clsId);
				$classSessionInformation = $classSessionInfo[0];
				if ($classSessionInformation == null) {
					$classSessionInformation = 'session_name<| ~~session_startdate<| ~~session_enddate<| ~~session_starttime<| ~~session_endtime<| ~~time_zone<| ~@';
				}
			
				$sessionLocation = $classSessionInfo[1];
				if ($sessionLocation == null) {
					$LocationDel = ' - ';
				} else {
				$LocationDel = ClassLocationDetails($clsId,$sessionLocation);
				if(count($LocationDel) < 0) $LocationDel = $sessionLocation;
				}
				// Append tokens string with class session details
				$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
				",'session_location>|','". $LocationDel ."','~|group_sessiondata>|','".$classSessionInformation ."','~|group_instructordata>|','". $classInstructorInfo."~|'";
			
			}else {
				// Append launch url for wbt class to the tokens string
				/*$notificationInfo['tokens_string'] = $notificationInfo['tokens_string'] .
				",'launch_url>|',ifnull(ctnmap.launch_url,''),'~|'";*/
			}
			$notificationInfo['tokens_string'] = $notificationInfo['tokens_string'] .
			",'dt_code>|','".$classDeliveryType."', '~|'";
			
			
			
		}
		
		if($notStr =='register_mandatory_compliance_by_admin'){
			$notificationInfo['message_id'] 	= 'register_mandatory_compliance_by_admin';
			$notificationInfo['message_type'] = 'Course Register Compliance';
		}else if($notStr =='compliance_register_vc_by_admin'){
			$notificationInfo['message_id'] 	= 'compliance_register_vc_by_admin';
			$notificationInfo['message_type'] = 'Course VC Register Compliance';
		}else if($notStr == 'compliance_register_wbt_by_admin'){
			$notificationInfo['message_id'] 	= 'compliance_register_wbt_by_admin';
			$notificationInfo['message_type'] = 'Course WBT Register Compliance';
		}else if($notStr == 'register_wbt_by_admin'){
			$notificationInfo['message_id'] = 'register_wbt_by_admin';
			$notificationInfo['message_type'] = 'Course Register Wbt';
		}else if($notStr == 'register_vc_by_admin'){
				$notificationInfo['message_id'] = 'register_vc_by_admin';
				$notificationInfo['message_type'] = 'Course Register VC';
		}else if($notStr == 'register_by_admin'){
				$notificationInfo['message_id'] = 'register_by_admin';
				$notificationInfo['message_type'] = 'Course Register';
		}else if($notStr == 'man_comp_multiple_class_register'){
			$multipleInfo = $this->multipleComplianceMandatoryClassNotification($crsId);
			$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
			",'compliance_mandatory>|', if(ifnull(enr.is_compliance,'') = 1, 'Compliance', 'Mandatory'),
			if(cls.addn_notification_show = 0,'',if(cls.additional_info IS NOT NULL AND cls.addn_notification_show = 1,concat('~|add_notes>|',cls.additional_info,''),'~|add_notes>|-')),'~|'";
			
			$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
			",'course_code>|',ifnull(crs.code,''),'~|'";
			
			if($complete_date != ""){
				$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
				",'complete_by_date>|',ifnull(DATE_FORMAT(crs.complete_date,'%d %M %Y'),''),'~|'";
			}else{
				$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
				",'complete_by_date>|',ifnull(DATE_FORMAT(DATE_ADD(enr.reg_date, INTERVAL ifnull(crs.complete_days,'') DAY),'%d %M %Y'),''),'~|'";
			}
			
			$classtittle = '';
			$classcode = '';
			$deleverytype = '';
			$sessionlocation = '';
			$session_startdate = '';
			$i = 0;
			$classdetails = '';
			foreach ($multipleInfo as $classvalues){
				$classtittle = $classvalues->classtitle;
				$classcode = $classvalues->classcode;
				$deleverytype = $classvalues->deleverytype;
			
				$sessionlocation = $classvalues->location;
				$session_startdate = $classvalues->startdate;
				$loc_clsid = $classvalues->classid;
				
				if(!empty($sessionLocation)) {
				$LocationDel = ClassLocationDetails($loc_clsid,$sessionlocation);				
				if(count($LocationDel) < 0)  $LocationDel = $sessionlocation;
			 	} else $LocationDel = ' - ';
				$classdetails .= "'class_title<|','".$classtittle."', '~~' ,
						'class_code<|','".$classcode."', '~~' ,
						'delivery_type<|','". $deleverytype."', '~~' ,
						'session_location<|','".$LocationDel."', '~~' ,
						'session_startdate<|','".$session_startdate."', '~@',";
				$i++;
			}
			if(!empty($classdetails)){
				$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
				",'group_data>|',$classdetails'~|'";
			}
			
			
			$notificationInfo['message_id'] 	= 'man_comp_multiple_class_register';
			$notificationInfo['message_type'] = ' Multiple Compliance/Mandatory Class Notification'; 
		}else if($notStr == 'man_comp_multiple_re-register'){
			$notificationInfo['message_id'] 	= 'man_comp_multiple_re-register';
			$notificationInfo['message_type'] = ' Multiple Compliance Class Notification for re-register'; 
		
		}else if($notStr == "mandatory_tp_multiple_register"){
			$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
			",'tp_title>|',ifnull(prg.title,''),'~|tp_code>|',ifnull(prg.code,''),'~|compliance_mandatory>|Mandatory~|'";
			
			$notificationInfo['message_id'] 	= 'mandatory_tp_multiple_register';
			$notificationInfo['message_type'] = ' Mandatory TP Multiple Register';
		}else if($notStr == 'compliance_course_re_register_by_admin'){
			$notificationInfo['message_id'] 	= 'compliance_course_re_register_by_admin';
			if($classDeliveryType == 'ilt')
				$notificationInfo['message_type'] = 'Compliance ILT Course Re-Register';
			else if($classDeliveryType == 'vc')
				$notificationInfo['message_type'] = 'Compliance VC Course Re-Register';
			else
				$notificationInfo['message_type'] = 'Compliance WBT Course Re-Register';
		}else if ($notStr == 'compliance_class_incomplete'){
		    $notificationInfo['message_id'] 	= 'compliance_class_incomplete';
		    $notificationInfo['message_type'] = ' Compliance Course class Incomplete Reminder';
		}
		expDebug::dPrint("Checking the generateEnrollmentRegisterAndWaitlistNotifications".print_r($notificationInfo,1));
		return $notificationInfo;
		}catch(Exception $e){
			expDebug::dPrint("ERROR in enrollment register waitlist notification ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function generateEnrollmentDropAndCancelNotifications($notStr,$sessInfo,$instInfo,$clsId){
		try{
		$notificationInfo = array();
		$stDate = $sessInfo[0]->start_date;
		$endDat = $sessInfo[0]->end_date;
		$sesName = $sessInfo[0]->name;
		$timezne = $sessInfo[0]->timezonename;
		
		if(!empty($sesName)) {
		$LocationDel = ClassLocationDetails($clsId,$sesName);
		if(count($LocationDel) < 0)  $LocationDel = $sesName;
		} else $LocationDel = ' - ';
		
		$notificationInfo['tokens_string'] = "'delivery_type>|',ifnull(cls.delivery_type,''),'~|course_title>|',ifnull(crs.title,''),'~|
				training_title>|',ifnull(crs.title,''),'~|class_title>|',ifnull(cls.title,''),'~|dt_code>|',ifnull(cls.delivery_type,''),
					if(cls.addn_notification_show = 0,'',if(cls.additional_info IS NOT NULL,concat('~|add_notes>|',cls.additional_info,''),'~|add_notes>|-')),
                '~|session_startdate>|','".$stDate."','
				~|session_enddate>|','". $endDat ."','~|session_location>|','". $LocationDel ."','~|time_zone>|','" . $timezne  ."','~|first_name>|',ifnull(p.first_name,''),
				'~|last_name>|',ifnull(p.last_name,''),'~|full_name>|',ifnull(p.full_name,''),'~|learning_fullname>|',ifnull(p.full_name,''),'~|user_name>|',ifnull(p.user_name,''),
				'~|user_email>|',ifnull(p.email,''),'~|user_phone>|',ifnull(p.phone_no,''),'~|'";
		
		if(!empty($instInfo)){
			$notificationInfo['tokens_string'] = $notificationInfo['tokens_string'].",'group_instructordata>|','".$instInfo."'";
		}
		expDebug::dPrint("Checking the generateEnrollmentDropAndCancelNotifications".print_r($notificationInfo,1));
		if($notStr =='class_cancel'){
		  $notificationInfo['message_id'] = 'class_cancel';
		  $notificationInfo['message_type'] = 'Class Cancel By Admin';
		}else if($notStr =='class_cancel_wbt_vod'){
		  $notificationInfo['message_id'] = 'class_cancel_wbt_vod';
		  $notificationInfo['message_type'] = 'WBT/VOD Cancel by Admin';
		}
		
		return $notificationInfo;
		}catch(Exception $e){
			expDebug::dPrint("ERROR in Drop Cancel notification ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	private function generateEnrollmentCompletedNotification($notStr){
		try{
		$notificationInfo = array();
		$notificationInfo['tokens_string'] = "'first_name>|',ifnull(p.first_name,''),'~|last_name>|',ifnull(p.last_name,''),'~|user_email>|',ifnull(p.email,''),
		if(cls.addn_notification_show = 0,'',if(cls.additional_info IS NOT NULL AND cls.addn_notification_show = 1,concat('~|add_notes>|',cls.additional_info,''),'~|add_notes>|-')),
		'~|course_title>|',ifnull(crs.title,''),'~|course_completed_status>|',ifnull(ests.name,''),'~|course_completed_date>|',ifnull(DATE_FORMAT(crs.complete_date,'%d %M %Y'),''),
		'~|full_name>|',ifnull(p.full_name,''),'~|learning_fullname>|',ifnull(p.full_name,''),'~|user_name>|',ifnull(p.user_name,''),'~|user_phone>|',ifnull(p.phone_no,'')";
		
		$notificationInfo['message_id'] = 'course_completed';
		$notificationInfo['message_type'] = 'Course Completed';
		expDebug::dPrint("Checking the generateEnrollmentDropAndCancelNotifications".print_r($notificationInfo,1));
		return $notificationInfo;
		}catch(Exception $e){
			expDebug::dPrint("ERROR in complete notification ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function generateCertCurrLPRegisterNotifications($programId,$notStr,$moduleseq){
		try{
			$getInfo          = $this->getCertCurrLPRegisterDetailsForNotifications($programId,$moduleseq);
			expDebug::dPrint("The enrollment detials for course".print_r($getInfo,1));
			$notificationInfo = array();
	
			$classesInfoStr   = '';
			$isMandatory      = array();
			$delivery_type    = '';
			$course_title     = '';
			$dt_code          = '';
			$prg_lang_code    = '';
			$totalRec         = count($getInfo);
	
			$sno=1;
			foreach($getInfo as $getInfo) {
				if($getInfo->coursemandatory == 'Y') {
					$isMandatory[] = $getInfo->coursemandatory;
				}
				$crsTitle = addslashes($getInfo->coursetitle);
				$crsCode = addslashes($getInfo->coursecode);
				$crsMand = $getInfo->coursemandatory;
				$module_title   = convertNullValToNullStr($getInfo->moduletitle);
			  if($notStr == 'cert_curr_lp_waitlist_register_by_admin'){
			  	$classesInfoStr .= "tp_title<|','".$crsTitle."', '~~tp_code<|','".$crsCode."', '~~serial_no<|','". $sno."',
			  	'~~module_title<|','".$module_title."', '~~tp_mandatory_optional<|','".(($crsMand =='Y') ? "Mandatory" : "Optional" )."',
			  	'~~tp_status<|',if(e.comp_status is null, concat('Waitlist Position',ifnull(e.waitlist_priority,'')),ifnull(ests.name,'')), '~@";
			  } else{
			  	$classesInfoStr .= "tp_title<|','".$crsTitle."', '~~tp_code<|','".$crsCode."', '~~serial_no<|','". $sno."',
			  	'~~module_title<|','".$module_title."', '~~tp_mandatory_optional<|','".(($crsMand =='Y') ? "Mandatory" : "Optional" )."',
			  	'~~tp_status<|',ifnull(enrsts.name,''), '~@";
			  }
			  
				$sno++;
			}
	
			$mandatory       = count($isMandatory);
			$nonMandatory    = $totalRec - $mandatory;
			$delivery_type  = $getInfo->deliverytype;
			$course_title   = addslashes($getInfo->programtitle);
			$dt_code        = addslashes($getInfo->programcode);
			$tpadditional_info = $getInfo->additional_info;
			$tpaddn_notification_show = $getInfo->addn_notification_show;
			
			$notificationInfo['tokens_string'] = "'full_name>|',ifnull(p.first_name,''),'~|delivery_type>|','". $delivery_type ."','~|tp_title>|','". $course_title ."',
            '~|course_title>|','". $course_title ."','~|dt_code>|','". $dt_code ."','~|tp_mandatory_courses>|','" . (($mandatory>0)?$mandatory:'Nil')  ."',
			'~|tp_optional_course>|','". (($nonMandatory)>0?$nonMandatory:'Nil') ."','~|group_sessiondata>|','". $classesInfoStr ."'";
			
			if($tpaddn_notification_show == 1){
					if($tpadditional_info != ""){
						$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
						",'add_notes>|','" . $tpadditional_info. "~|'";
					}
					else{
						$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
						",'add_notes>|-~|'";
					}
				}
			if($notStr == 'cert_curr_lp_register_by_admin'){
				$notificationInfo['message_id']    = 'cert_curr_lp_register_by_admin';
				$notificationInfo['message_type']  = 'Cert/Curr/Learning Plan registered';
			} else if($notStr == 'cert_curr_lp_waitlist_register_by_admin'){
				$notificationInfo['message_id']    = 'cert_curr_lp_waitlist_register_by_admin';
				$notificationInfo['message_type']  = 'Cert/Curr/Learning Plan Waitlist registered';
			} else if($notStr == 'cert_curr_lp_recertify_by_admin'){
				$notificationInfo['message_id']    = 'cert_curr_lp_recertify_by_admin';
				$notificationInfo['message_type']  = 'Certification Recertify by an Admin';
			} else{
				$notificationInfo['message_id']    = 'cert_curr_lp_completed';
				$notificationInfo['message_type']  = 'Cert/Curr/LP Completed';
			}
	
			expDebug::dPrint("Checking the generateCertCurrLPRegisterNotifications".print_r($notificationInfo,1));
			return $notificationInfo;
		} catch(Exception $e){
			throw new Exception($e->getMessage());
		}
	}
	
	
	private function getCertCurrLPRegisterDetailsForNotifications($programId,$moduleseq) {
		try{
			if($moduleseq == '' || !(isset($moduleseq))) $moduleseq=1;
			$args = array(':prgid' => $programId,':moduleseq' => $moduleseq);
			$fields = array( 'prof.name as deliverytype',
					'crs.title as coursetitle',
					'crs.code as coursecode',
					'prgm.title as programtitle',
					'prgm.code as programcode',
					'prgm.additional_info as additional_info',
					'prgm.addn_notification_show as addn_notification_show',
					'crsmap.is_required as coursemandatory',
					'modu.title as moduletitle'
			);
	
			$conditons = array(' prgm.id = :prgid AND module.sequence = :moduleseq ');
	
			$join = array(' INNER JOIN slt_module_crs_mapping crsmap on crsmap.program_id = prgm.id',
					' INNER JOIN slt_module module on module.id = crsmap.module_id and module.program_id =  prgm.id',
					' INNER JOIN slt_course_template crs on crs.id = crsmap.course_id and crs.status = \'lrn_crs_sts_atv\'',
					' INNER JOIN slt_profile_list_items prof on prof.code = prgm.object_type',
					' INNER JOIN slt_module modu on modu.id = crsmap.module_id');
			$grpBy = "GROUP BY crsmap.course_id ";
			// Prepare query
			$query = $this->db->prepareQuerySelect('slt_program prgm',$fields,$conditons,$join,$grpBy);
			expDebug::dPrintDBAPI("Get Notification details",$query,$args);
			// Execute query
			$this->db->callQuery($query,$args);
			// Fetch results
			$userDetails = $this->db->fetchAllResults();
			return $userDetails;
		}catch (Exception $ex) {
			watchdog_exception('getCertCurrLPRegisterDetailsForNotifications', $ex);
			expertusErrorThrow($ex);
		}
	
	}
	private function userPreferredLanguage($userId,$notStr,$notLang){
		try{
			$setLanguage = 'cre_sys_lng_eng';
		
			$userDetails = getDrupalIdofuser($userId);
			$user_preffered_language = $userDetails[0]->preferred_language;
			if(!empty($user_preffered_language) && $user_preffered_language != $setLanguage){
				$isPreferredNotification  = getPreferredNotificationLanguage($notStr,$user_preffered_language);
				if($isPreferredNotification){
					$setLanguage = $user_preffered_language;
				}
			}
			if($isPreferredNotification == 0 ){
				if(!empty($notLang) && $notLang != $setLanguage){
					$isNotificationLanguage  = getPreferredNotificationLanguage($notStr,$notLang);
					if($isNotificationLanguage){
						$setLanguage = $notLang;
					}
				}
			}
			return $setLanguage;
		}catch(Exception $e){
			expDebug::dPrint("ERROR in user preferred lang ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}

	private function getPreferredNotificationLanguage($msgId,$langCode){
		try{
			$this->db->callQuery("select count(1) from slt_notification_info notiinfo left join slt_notification_frame frame frame.notification_id = notiinfo.id
					where notiinfo.notification_code = :notcode and frame.lang_code = :langcode and notiinfo.status = :sts ",
					array(':notcode' => $msgId,':langcode'=> $langCode,':sts' => 'cre_ntn_sts_atv'));
			$langCount = $this->db->fetchColumn();
			return $langCount;
		}catch(Exception $e){
			expDebug::dPrint("ERROR in preferred notification lang ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function getDrupalIdofUser($userId){
		try{
			$args = array();
			$conditons = array();
			$fields = array(
					'usr.uid',
					'prsn.full_name',
					'prsn.manager_id',
					'usr.mail',
					'prsn.preferred_language',
					'prsn.status',
			);
		
			$conditons[] = ' prsn.id = :usrid ';
			$args[':usrid']= $userId;
			$join = array(' left join users usr on prsn.user_name = usr.name');
			// Prepare query
			$query = $this->db->prepareQuerySelect('slt_person prsn',$fields,$conditons,$join);
			expDebug::dPrintDBAPI("Get Notification details",$query,$args);
			// Execute query
			$this->db->callQuery($query,$args);
			// Fetch results
			$userDetails = $this->db->fetchAllResults();
			return $userDetails;
		}catch(Exception $e){
			expDebug::dPrint("ERROR in get drupal user id ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}

	private function getSendNotificationToMailCc($notification_sendto, $drupalUser) {
		try{
			$explodeNotificationSendTo = explode(',',$notification_sendto );
			expDebug::dPrint(' $explodeNotificationSendTo ' . print_r($explodeNotificationSendTo, true) , 4);
			foreach( $explodeNotificationSendTo as $key => $value) {
				if(trim($value) == "cre_ntn_rpt_adm") {
					$explodeNotificationSendTo[$key] =  variable_get('site_mail', 'info@expertusone.com');
					//$explodeNotificationSendTo[$key] = "NULL";
				}
				if(trim($value) == "cre_ntn_rpt_mgr") {
					$manager_id = $drupalUser['manager_id'];
					if(!empty($manager_id)) {
						$getManagerRes = getDrupalIdofUser($manager_id);
						$managerEmailId = $getManagerRes['email'];
						$explodeNotificationSendTo[$key] =  $managerEmailId;
					}
					else {
						$explodeNotificationSendTo[$key] =  NULL;
					}
				}
				if (trim($value) == "cre_ntn_rpt_usr") {
					$explodeNotificationSendTo[$key] =  NULL;
				}
				if (trim($value) == "cre_ntn_rpt_ins") {
					$explodeNotificationSendTo[$key] =  $drupalUser['email'];
				}
			}
		
			$explodeNotificationSendTo = array_filter(array_unique($explodeNotificationSendTo));
			$returnCcValue = str_replace("NULL,", "", implode(",",$explodeNotificationSendTo ));
			expDebug::dPrint(' $explodeNotificationSendTo after = ' . print_r($explodeNotificationSendTo, true) , 4);
			expDebug::dPrint(' $returnCcValue = ' . print_r($returnCcValue, true) , 4);
			return $returnCcValue;
		}catch(Exception $e){
			expDebug::dPrint("ERROR in send CC notification ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
 
	private function getEnrollmentNotificationOverrideInfo($entityId, $entityType, $notificatonCode, $notificationSendType){
		try{
			expDebug::dPrint("values with in getEnrollmentNotificationOverrideInfo");
			$args = array();
			$conditons = array();
			$fields = array(
					'nmap.status',
					'nmap.notify_text',
			);
			
			$conditons[] = ' nmap.entity_id = :entid ';
			$conditons[] = ' nmap.entity_type = :enttype ';
			$conditons[] = ' info.notification_code = :notcode ';
			$conditons[] = ' nmap.notify_send_type = :nottype ';
			$args[':entid']= $entityId;
			$args[':enttype']= $entityType;
			$args[':notcode']= $notificatonCode;
			$args[':nottype']= $notificationSendType;
			$join = array(' left join slt_notification_info info on nmap.notify_id = info.id');
			// Prepare query
			$query = $this->db->prepareQuerySelect('slt_entity_notification_mapping nmap',$fields,$conditons,$join);
			expDebug::dPrintDBAPI("Get Notification details",$query,$args);
			// Execute query
			$this->db->callQuery($query,$args);
			// Fetch results
			$overrideDetails = $this->db->fetchAssociate();
			expDebug::dPrint("Result of getEnrollmentNotificationOverrideInfo".print_r($overrideDetails,1),4);
			return $overrideDetails;
		}catch(Exception $e){
			expDebug::dPrint("ERROR in get notification override info ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}

	private function getClassSessionInfoForEnrollmentNotifications($courseId, $classId){
		try{
			include_once($_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_core/exp_sp_core.inc');
			$args = array();
			$conditons = array();
			$fields = array(
					"ifnull(loc.name,'') as name",
					"ifnull(ses.start_date,'') as start_date",
					"ifnull(ses.end_date,'') as end_date",
					"ifnull(ses.timezone,'') as timezone",
					"ifnull(ses.capacity_max,'') as capacity_max",
					"ifnull(splt_timezone.attr2,'') as attr2",
					"ifnull(splt_timezone.name,'') as timezonename"
			);
			
			$conditons[] = ' ses.course_id = :crsid ';
			$conditons[] = ' ses.class_id = :clsid ';
			$args[':crsid']= $courseId;
			$args[':clsid']= $classId;
			$join = array(' left join slt_profile_list_items splt_timezone on splt_timezone.code=ses.timezone',
						  ' left join slt_location loc on ses.location_id = loc.id');
			$range = "limit 1";
			// Prepare query
			$query = $this->db->prepareQuerySelect('slt_course_class_session ses',$fields,$conditons,$join,$range);
			expDebug::dPrintDBAPI("Get Notification details",$query,$args);
			// Execute query
			$this->db->callQuery($query,$args);
			// Fetch results
			$classSessionInfo = $this->db->fetchAllResults();
			expDebug::dPrint("Result of getEnrollmentNotificationOverrideInfo".print_r($classSessionInfo,1),4);
			
			$classSessionInfo[0]->start_date = dateTimeStrToUSShort($classSessionInfo[0]->start_date, true, true, true, false);
			$classSessionInfo[0]->end_date = dateTimeStrToUSShort($classSessionInfo[0]->end_date, true, true, true, false);
			return $classSessionInfo;
		}catch(Exception $e){
			expDebug::dPrint("ERROR in get session info for notification ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function getClassInstructorInfoForEnrollmentNotifications($classId) {
		try {
		  $instructorInfo = "instructor_first_name<| ~~instructor_last_name<| ~@";
		  $deliveryType = $this->getDeliveryTypeCode($classId, 'Short');
		  if($deliveryType=='ilt'){
			$args = array();
			$conditons = array();
			$fields = array(
					'pers.first_name',
					'pers.last_name',
			);
			
			$conditons[] = ' rm.object_id = :clsid ';
			$conditons[] = ' rm.object_type = :clstype ';
			$args[':clstype']= 'cre_sys_obt_cls';
			$args[':clsid']= $classId;
			$join = array(' left join slt_person pers on pers.id = rm.user_id');
			// Prepare query
			$query = $this->db->prepareQuerySelect('slt_object_role_mapping rm',$fields,$conditons,$join);
			expDebug::dPrintDBAPI("Get Notification details",$query,$args);
			// Execute query
			$this->db->callQuery($query,$args);
			// Fetch results
			$instructorList = $this->db->fetchAssociate();
			expDebug::dPrint("Result of getEnrollmentNotificationOverrideInfo".print_r($classSessionInfo,1),4);
			
		  $instructorInfo  = '';
	      foreach ($instructorList as $instructor) {
	        $instructorFirstName = ($instructor['first_name'] == null)? '' : $instructor['first_name'];
	        $instructorLastName = ($instructor['last_name'] == null)? '' : $instructor['last_name'];
	        $instructorInfo =	$instructorInfo .
	                        "'instructor_first_name<|','". $instructorFirstName ."', '~~' .
	                        'instructor_last_name<|','". $instructorLastName ."',
	    					  '~@'";
	      }
		  }
		  return $instructorInfo;
			
		}catch (Exception $e) {
		 	expDebug::dPrint("ERROR in get class instructor info for enrollment notification ".print_r($e,true),1);
			throw new Exception($e->getMessage());
	  }
	}

	private function getDeliveryTypeCode($clsId,$retType=''){
		try{
			$args = array();
			$conditons = array();
			$fields = array(
					'pli.name as long_name',
					'pli.attr1 as short_name',
			);
			
			$conditons[] = ' cls.id = :clsid ';
			$args[':clsid']= $clsId;
			$join = array(' left join slt_profile_list_items pli on cls.delivery_type = pli.code');
			// Prepare query
			$query = $this->db->prepareQuerySelect('slt_course_class cls',$fields,$conditons,$join);
			expDebug::dPrintDBAPI("Get Delivery type details",$query,$args);
			// Execute query
			$this->db->callQuery($query,$args);
			// Fetch results
			$delTypeInfo = $this->db->fetchAllResults();
			expDebug::dPrint("Result of getDeliveryTypeCode".print_r($delTypeInfo,1),4);
			
			if ($retType == 'Short') {
				return strtolower($delTypeInfo[0]->short_name);
			}
			if ($retType == 'Long') {
				return $delTypeInfo[0]->long_name;
			}
		}catch(Exception $e){
			expDebug::dPrint("ERROR in get DT Code ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}

	private function getClassMultiSessionInfoForNotifications($courseId, $classId,$delType) {
	  /* Derive timezone attr of session from slt_profile_list_items table*/
	  include_once($_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_core/exp_sp_core.inc');
	  try {
		  $args = array();
		  $conditons = array();
		  $fields = array(
		  		'loc.name as location_name',
		  		'ses.id as id',
		  		'ses.start_date as start_date',
		  		'ses.end_date as end_date',
		  		'ses.timezone as timezone',
		  		'ses.capacity_max as capacity_max',
		  		'ses.title as title',
		  		'ses.start_time as start_time',
		  		'ses.end_time as end_time',
		  		'splt_timezone.attr2 as session_timezone',
		  		'splt_timezone.name as session_timezone_name'
		  );
		  
		  $conditons[] = ' ses.course_id = :crsid ';
		  $conditons[] = ' ses.class_id = :clsid ';
		  $args[':crsid']= $courseId;
		  $args[':clsid']= $classId;
		  $join = array(' left join slt_profile_list_items splt_timezone on splt_timezone.code=ses.timezone',
		  				' left join slt_location loc on ses.location_id = loc.id');
		  // Prepare query
		  $query = $this->db->prepareQuerySelect('slt_course_class_session ses',$fields,$conditons,$join);
		  expDebug::dPrintDBAPI("Get Notification details",$query,$args);
		  // Execute query
		  $this->db->callQuery($query,$args);
		  // Fetch results
		  $sessionList = $this->db->fetchAllResults();
		  
		  expDebug::dPrint(' $$sessionList = ' . print_r($sessionList, true) , 3);
		  $classSessionInfo1 = array();
		  $sessionLocation = '';
		  $classSessionInfo = '';
		  $sessionTimeZone = '';
		  $sessionTimeZoneDisp = '';
		  foreach ($sessionList as $key => $session) {
		  	expDebug::dPrint("Test he value from session valiue".print_r($session,1));
        $sessionTitle = ($session->title == null)? ' ' : $session->title;
        $sessionStartDate = dateTimeStrToUSShort($session->start_date, true, true, true, false);
        $sessionStartDate = convertNullValToNullStr($sessionStartDate);
        $sessionEndDate   = dateTimeStrToUSShort($session->end_date, true, true, true, false);
        $sessionEndDate = convertNullValToNullStr($sessionEndDate);
        $sessionStartTime = ($session->start_time != '' && $session->start_time != null)?date_format(date_create($session->start_time),'g:i A'):'';
        $sessionEndTime   = ($session->end_time != '' && $session->end_time != null)?date_format(date_create($session->end_time),'g:i A'):'';
        $sessionLocation  = $session->location_name;
        $sessionLocation = convertNullValToNullStr($sessionLocation);
        $sessionTimeZone = convertNullValToNullStr($session->session_timezone_name);
		    if(!empty($sessionTimeZone))
		    		$sessionTimeZoneDisp = substr($sessionTimeZone,stripos($sessionTimeZone,')')+2);
		    $classSessionInfo =	$classSessionInfo .
		                      'session_name<|' . $sessionTitle . '~~' .
		                      'session_startdate<|' . $sessionStartDate . '~~' .
		    				  'session_enddate<|' . $sessionEndDate . '~~' .
		  					  'session_starttime<|' . $sessionStartTime . '~~' .
		  					  'session_endtime<|' . $sessionEndTime .  '~~' .
		    				  'session_location<|' . $sessionLocation . '~~' .
		              'time_zone<|' . $sessionTimeZoneDisp .
		  					  '~@';
		  } // end foreach
		  $classSessionInfo1[0] = $classSessionInfo;
		  $classSessionInfo1[1] = $sessionLocation;
		  $classSessionInfo1[2] = $sessionTimeZone;
		  return $classSessionInfo1;
		}catch (Exception $e) {
	    expDebug::dPrint("ERROR in get multi session info for notification ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}

	private function getClassInstructorInfoForNotifications($classId) {
		try {
		  $instructorInfo = 'instructor_first_name<| ~~instructor_last_name<| ~@';
		  $deliveryType = $this->getDeliveryTypeCode($classId, 'Short');
		  if($deliveryType=='ilt'){
		  	
		  	$args = array();
		  	$conditons = array();
		  	$fields = array(
		  			'pers.first_name as first_name',
		  			'pers.last_name as last_name'
		  	);
		  	
		  	$conditons[] = ' rm.object_id = :clsid ';
		  	$conditons[] = ' rm.object_type = :clstype ';
		  	$args[':clstype']= 'cre_sys_obt_cls';
		  	$args[':clsid']= $classId;
		  	$join = array(' left join slt_person pers on pers.id = rm.user_id');
		  	// Prepare query
		  	$query = $this->db->prepareQuerySelect('slt_object_role_mapping rm',$fields,$conditons,$join);
		  	expDebug::dPrintDBAPI("Get Instructor details",$query,$args);
		  	// Execute query
		  	$this->db->callQuery($query,$args);
		  	// Fetch results
		  	$instructorList = $this->db->fetchAllResults();
		  	
		    foreach ($instructorList as $key => $instructor) {
		      $instructorFirstName = ($instructor->first_name == null)? ' ' : $instructor->first_name;
		      $instructorLastName = ($instructor->last_name == null)? ' ' : $instructor->last_name;
		      $instructorInfo =	$instructorInfo .
		                        'instructor_first_name<|' . $instructorFirstName . '~~' .
		                        'instructor_last_name<|' . $instructorLastName .
		    					  '~@';
		    } // end foreach
		  }// end if
		  return $instructorInfo;
		}catch (Exception $e) {
			expDebug::dPrint("ERROR in get class instructor info for notification ".print_r($e,true),1);
			throw new Exception($e->getMessage());
	  }
	}

	private function multipleComplianceMandatoryClassNotification($courseId){
		try{
			$args = array();
			$conditons = array();
			$fields = array(
					'cls.title as classtitle',
					'cls.code as classcode',
					'prof.name as deleverytype',
					'loc.name as location',
					"DATE_FORMAT((sess.start_date),'%d %M %Y') as startdate",
					'sess.class_id as classid',
			);
			
			$conditons[] = ' cls.course_id = :crsid ';
			$args[':crsid']= $courseId;
			$join = array(' left join slt_course_class_session sess on sess.class_id = cls.id',
					' left join slt_location loc on loc.id = sess.location_id',
					' left join slt_profile_list_items prof on prof.code = cls.delivery_type');
			// Prepare query
			$query = $this->db->prepareQuerySelect('slt_course_class cls',$fields,$conditons,$join);
			expDebug::dPrintDBAPI("Get Mandatory notification session details",$query,$args);
			// Execute query
			$this->db->callQuery($query,$args);
			// Fetch results
			$classSessionInfo = $this->db->fetchAllResults();
			expDebug::dPrint("Result of multipleComplianceMandatoryClassNotification".print_r($classSessionInfo,1),4);
			return $classSessionInfo;
		}catch (Exception $e) {
		  expDebug::dPrint("ERROR in multiple complainace for notification ".print_r($e,true),1);
			throw new Exception($e->getMessage());
	  }
	}


protected function updateManagerEmailId($mgnr = FALSE){
	try{
		$tblDet = $this->getTableDetails();
		$update = array();
		$update['table'] = $tblDet['tblname'].' tm';
		
		if($mgnr == TRUE){
			$update['fields'] = array(
					'tm.manager_email'=> "pm.email",
					'tm.mgr_uid'=> "pm.id",
					'tm.mgr_drup_uid'=> "usr.uid"
			);
			$update['joins'] = array (
					"LEFT JOIN slt_person per on per.id = tm.user_id ",
					"LEFT JOIN slt_person pm on pm.id = per.manager_id ",
					"LEFT JOIN users usr on usr.name = pm.user_name"
			);
		}else{
			$update['fields'] = array(
					'tm.user_email'=> "pm.email",
					'tm.drup_uid'=> "usr.uid"
			);
			$update['joins'] = array (
					"LEFT JOIN slt_person pm on pm.id = tm.user_id ",
					"LEFT JOIN users usr on usr.name = pm.user_name "
			);
		}
		
		$update['condition']=array(
				"tm.batch_id = :batch_id ",
				"tm.record_status IN (:recst , :recmn )"
		);
		$arg=array(':batch_id' => $tblDet['btc_id'],
							 ':recst' => 'R',
							 ':recmn' => 'MN'
		);
		
		$updQry1 = $this->db->prepareQueryUpdate($update['table'],$update['fields'],$update['condition'],$update['joins']);
		expDebug::dPrintDBAPI(" Query for update user manager email id ",$updQry1,$arg);
		$this->db->callExecute($updQry1,$arg);

	}catch (PDOException $ex) {
			expDebug::dPrint("PDO Error in Validating the batch records for users ".print_r($ex,true),1);
			throw new PDOException($ex->getMessage());
		}catch(Exception $e){
				expDebug::dPrint("ERROR in update manager id ".print_r($e,true),1);
				throw new Exception($e->getMessage());
		}
}
	
	/**
	 * 
	 * @param String - $mdleName - Name of the module to check whether it is active or not
	 * @throws Exception
	 * @return Integer - $moduleSts. 0 - Module is diabled, 1 - Module is Enabled 
	 */
	protected function getModuleStatus($mdleName){
		try{
			
			$fields = array(
					'sys.status as sts'
			);
			$cond = array(
					'sys.type = :typ ',
					'sys.name = :nme '
			);
			$args = array(
					':typ' => 'module',
					':nme' => $mdleName
			);
			$qry = $this->db->prepareQuerySelect('system sys',$fields,$cond);
			expDebug::dPrintDBAPI("Get getModuleStatus records ",$qry,$args);
			$this->db->callQuery($qry,$args);
			$moduleSts = $this->db->fetchColumn();
			expDebug::dPrint("Module enabled status".print_r($moduleSts,1));
			return $moduleSts;
		} catch(Exception $e){
			expDebug::dPrint("ERROR in get module status ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function deleteInsertRecords(){
		
		try{
			$tblDet = $this->getTableDetails();
			
			$updQry2 = "delete from slt_audit_trail where dataload_by = :dlby ";
			$this->db->callExecute($updQry2,array(':dlby'=> $tblDet['loadby']));
			
			$updQry2 = "delete from slt_user_points where dataload_by = :dlby ";
			$this->db->callExecute($updQry2,array(':dlby'=> $tblDet['loadby']));
			
			$updQry2 = "delete from slt_bulk_notification where dataload_by = :dlby ";
			$this->db->callExecute($updQry2,array(':dlby'=> $tblDet['loadby']));
			
			$updQry2 = "delete from slt_order_items where dataload_by = :dlby ";
			$this->db->callExecute($updQry2,array(':dlby'=> $tblDet['loadby']));
			
			$updQry2 = "delete from slt_order where dataload_by = :dlby ";
			$this->db->callExecute($updQry2,array(':dlby'=> $tblDet['loadby']));
			
			$updQry2 = "delete map from slt_enrollment_content_mapping map
			inner join slt_enrollment enr on enr.id = map.enroll_id
			where enr.dataload_by = :dlby and enr.custom_status = :ins ";
			$this->db->callExecute($updQry2,array(':dlby'=> $tblDet['loadby'],':ins' => 'i'));
			
			$updQry2 = "delete from slt_enrollment where dataload_by = :dlby and custom_status = :ins ";
			$this->db->callExecute($updQry2,array(':dlby'=> $tblDet['loadby'],':ins' => 'i'));
			
			// Update master enrollment table overall status
			$update = array();
			$update['table'] = 'slt_master_enrollment me';
				
			$update['fields'] = array(
					'me.overall_status'=> "tmp.menr_sts",
					'me.updated_on'=>now(),
			);
			$update['joins'] = array (
					"LEFT JOIN ".$tblDet['tblname']." tmp on tmp.menrid = me.id",
			);
			
			$update['condition']=array(
					"tmp.batch_id = :batch_id ",
					"tmp.menr_sts IS NOT NULL "
			);
			$arg=array(':batch_id' => $tblDet['btc_id']);
			
			$updQry1 = $this->db->prepareQueryUpdate($update['table'],$update['fields'],$update['condition'],$update['joins']);
			expDebug::dPrintDBAPI(" Query for update master enrollment status ",$updQry1,$arg);
			$this->db->callExecute($updQry1,$arg);
			
			// Update enrollment table overall status
			$update = array();
			$update['table'] = 'slt_enrollment me';
				
			$update['fields'] = array(
					'me.comp_status'=> "tmp.enr_sts",
					'me.updated_on'=>now(),
			);
			$update['joins'] = array (
					"LEFT JOIN ".$tblDet['tblname']." tmp on tmp.class_id = me.class_id and tmp.course_id = me.course_id and tmp.user_id = me.user_id and tmp.batch_id = me.custom_dataload",
			);
			
			$update['condition']=array(
					"tmp.batch_id = :batch_id ",
					"tmp.enr_sts IS NOT NULL ",
					"me.dataload_by = :dlby ",
					"me.custom_status = :upd "
			);
			$arg=array(':batch_id' => $tblDet['btc_id'],
					':dlby' => $tblDet['loadby'],
					':upd' => 'u'
			);
			
			$updQry1 = $this->db->prepareQueryUpdate($update['table'],$update['fields'],$update['condition'],$update['joins']);
			expDebug::dPrintDBAPI(" Query for update enrollment status ",$updQry1,$arg);
			$this->db->callExecute($updQry1,$arg);
			
		}catch(Exception $e){
			expDebug::dPrint("ERROR in deleteInsertRecords ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function updateEnrollStatus(){
		try{
			$tblDet = $this->getTableDetails();
			// Update enrollment table overall status
			$update = array();
			$update['table'] = $tblDet['tblname']." tmp";
			
			$update['fields'] = array(
					'tmp.menrid'=> "menr.id",
					'tmp.enr_sts'=> "enr.comp_status",
					'tmp.menr_sts'=> "menr.overall_status",
			);
			$update['joins'] = array (
					"INNER JOIN slt_enrollment enr on tmp.class_id = enr.class_id and tmp.course_id = enr.course_id and tmp.user_id = enr.user_id",
					"LEFT JOIN slt_master_enrollment menr on menr.id = enr.master_enrollment_id"
			);
				
			$update['condition']=array(
					"tmp.batch_id = :batch_id ",
					"tmp.operation = :upd ",
					"enr.comp_status IN (:comp_status1, :comp_status2, :comp_status3, :comp_status4, :comp_status5, :comp_status6, :comp_status7)"
			);
			$arg=array(':batch_id' => $tblDet['btc_id'],
					":upd" => 'update',
					':comp_status1' => 'lrn_crs_cmp_enr',
					':comp_status2' => 'lrn_crs_cmp_inp',
					':comp_status3' => 'lrn_crs_cmp_nsw',
					':comp_status4' => 'lrn_crs_cmp_inc',
					':comp_status5' => 'lrn_crs_cmp_cmp',
					':comp_status6' => 'lrn_crs_cmp_att',
					':comp_status7' => 'lrn_crs_cmp_exp',
			);
				
			$updQry1 = $this->db->prepareQueryUpdate($update['table'],$update['fields'],$update['condition'],$update['joins']);
			expDebug::dPrintDBAPI(" Query for update temp table status ",$updQry1,$arg);
			$this->db->callExecute($updQry1,$arg);
			
		} catch(Exception $e){
			expDebug::dPrint("ERROR in updateEnrollStatus ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	private function updateProgress(){
		try{
				$jobDetail = $this->getJobDetails();
			$update = array();
			$update['table'] = $jobDetail['pTable'].' tm';
			$update['fields'] = array(
			 		'tm.progress'=>100,
			 	
			);
			$update['condition']=array(
			 	//	"tm.mapping_id = x.mapid",
			 		"tm.batch_id = :batch_id ",
					"tm.comp_status= :cmpsts ",
					
			);
			$arg=array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':cmpsts' => 'lrn_crs_cmp_cmp',
			 		
			);
			 	
			$updQry1 = $this->db->prepareQueryUpdate($update['table'],$update['fields'],$update['condition']);
			expDebug::dPrintDBAPI(" Query for update the progresss ",$updQry1,$arg);
			$this->db->callExecute($updQry1,$arg);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in progress Count ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	private function deleteTemp2InvalidRecords(){
    try{
    	$jobDetail = $this->getJobDetails();
    	
    	//Update temp table 2 with invalid records
	  	list($fields, $args,$join,$condition) = array(array(),array(),array(),array());
	  	$fields=array(
	  			't2.custom4' => 't1.remarks',
	  			't2.record_status' => 't1.record_status'
	  	);
	  	$condition=array(
	  			't1.record_status = :inv ',
	  			't2.record_status = :r '
	  	);
	  	$join=array(
	  			'inner join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t2.mapping_id',
	  	);
	  	$arg = array(
	  			':inv' => 'IN',
	  			':r' => 'R'
	  	);
	  	$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t2",$fields,$condition,$join);
	  	expDebug::dPrintDBAPI(" Update Temp table ",$qry1,$arg);
	  	$this->db->callExecute($qry1,$arg);  
    	
      
      $updQry = "delete from ".$jobDetail['pTable']." where class_id is null OR course_id is null ";
      $this->db->callExecute($updQry);
    }catch(Exception $e){
      expDebug::dPrint("ERROR in deleteTemp2InvalidRecords ".print_r($e,true),1);
      throw new Exception($e->getMessage());
    }
  }
}
?>