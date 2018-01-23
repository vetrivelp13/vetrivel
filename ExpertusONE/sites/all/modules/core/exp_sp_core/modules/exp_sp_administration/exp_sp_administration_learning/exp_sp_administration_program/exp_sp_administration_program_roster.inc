<?php 


/*
 * launchRoster() 
 */
function launchProgramRoster($programId, $entityType, $otherParams){
  
  try {
  
      //$usersList = listEnrolledUsers($courseId, $classId);
    
      $qry_search_results = listProgramEnrolledUsers($programId, $entityType, $otherParams);
      $srch_result_response_roster->total   = $qry_search_results['total_pages'];
      $srch_result_response_roster->records = $qry_search_results['count'];
      $srch_result_response_roster->page    = $qry_search_results['page'];
      $srch_result_response_roster->classId = $classId;
      $srch_result_response_roster->courseId = $courseId;
      $overall_status= countenrollments($programId);
      $srch_result_response_roster->events_filter  =  $qry_search_results['events_filter'];
      expDebug::dPrint('Results in module: '.print_r($qry_search_results, true), 4);
      $_SESSION['jq_count'] = array();
      $i=0;
      $enrolledCnt      = 0;
      $completedCnt     = 0;
      $inprogresCnt     = 0;
      $cancelCount      = 0;
      $incompletCount   = 0;
      $waitlstCount     = 0;
      $pendingCount     = 0;
      $showCount        = 0;
      foreach ($qry_search_results['result'] as $rec_search_results) 
      {
       if($rec_search_results->reg_status_code == 'lrn_tpm_ovr_enr'){
        $enrolledCnt++;  
        }
       if($rec_search_results->reg_status_code == 'lrn_tpm_ovr_cmp'){
           $completedCnt++;
         }
       if($rec_search_results->reg_status_code == 'lrn_tpm_ovr_inp'){
           $inprogresCnt++;
         }
       if($rec_search_results->reg_status_code == 'lrn_tpm_ovr_cln'){
           $cancelCount++;
         }
       if($rec_search_results->reg_status_code == 'lrn_tpm_ovr_inc'){
           $incompletCount++;
         }
      if($rec_search_results->reg_status_code == 'lrn_tpm_ovr_wtl'){
           $waitlstCount++;
         } 
      if($rec_search_results->reg_status_code == 'lrn_tpm_ovr_ppv'){
           $pendingCount++;
         }
      if($rec_search_results->reg_status_code == 'lrn_tpm_ovr_exp'){
           $showCount++;
         }          
        $srch_result_response_roster->rows[$i]['id']   = $rec_search_results->enrol_pid;
        $srch_result_response_roster->rows[$i]['cell'] = array(
             'Username' 	      => $rec_search_results->user_name,
             'FullName'           => ucwords(strtolower($rec_search_results->userfullname)),
             'RegistrationStatus' => $rec_search_results->reg_status,
             'CompletionStatus'   => $rec_search_results->comp_status,
             'Score'              => $rec_search_results->score,
             'Grade'              => $rec_search_results->grade,
    		);
        $i++;  
      }
     // array('lrn_tpm_ovr_cmp','lrn_tpm_ovr_enr','lrn_tpm_ovr_inp','lrn_tpm_ovr_ppv','lrn_tpm_ovr_ppm','lrn_tpm_ovr_exp','lrn_tpm_ovr_wtl')
      $srch_result_response_roster->count_result   = 'Statistics - Enrolled: '.$enrolledCnt.', Completed: '.$completedCnt.', In-Progress: '.$inprogresCnt.', Incomplete: '.$incompletCount.', No Show: '.$showCount.', Waitlist: '.$waitlstCount.', Pending: '.$pendingCount.', Canceled: '.$cancelCount.'';
      $srch_result_response_roster->count_result = str_replace(',','<span class="narrow-search-results-item-detail-pipe-line">|</span>',$srch_result_response_roster->count_result);  
      echo drupal_json_encode($srch_result_response_roster);
  } catch (Exception $ex) {
    watchdog_exception('launchProgramRoster', $ex);
    expertusErrorThrow($ex);
  }       
}

function listProgramEnrolledUsers($programId, $entityType, $otherParams){
  try {
      list($userName, $fullName, $regStatus, $compStatus, $score, $grade) = explode('$$$', $otherParams);
      
      // slp_bulkreg_roster_catalog_registered_users_sel
      
      $page      = $_GET['page']; 
      $limit     = $_GET['rows']; 
      $sidx      = $_GET['sidx'];
      $sord      = $_GET['sord'];
      $tab       = $_GET['tab'];
        
      $listquery = db_select('slt_master_enrollment', 'enrol');
      $listquery->innerjoin('slt_person', 'person', 'enrol.user_id=person.id');
      $listquery->innerjoin('slt_profile_list_items', 'regstat', 'enrol.overall_status=regstat.code');
      
      $listquery->addField('enrol', 'id', 'enrol_pid');
      $listquery->addField('enrol', 'score');   
      $listquery->addExpression('slf_get_profile_list_name(enrol.grade,NULL)', 'grade');
      $listquery->addField('enrol', 'user_id', 'userid');
      $listquery->addField('enrol', 'overall_status', 'reg_status_code');
      $listquery->addExpression('DATE_FORMAT(enrol.comp_date,\'%m/%d/%Y\')', 'comp_date');
      $listquery->addExpression('DATE_FORMAT(enrol.reg_date,\'%m/%d/%Y\')', 'reg_date');
      
      $listquery->addField('regstat', 'name', 'reg_status');
      $listquery->addField('regstat', 'name', 'comp_status');
      
      
      $listquery->addField('person', 'user_name');
      $listquery->addField('person', 'full_name', 'userfullname');
      
      $listquery->condition('enrol.program_id', $programId, '=');
      
      if(isset($_GET['FullName'])){
        $getfullname = $_GET['FullName'];
        $listquery->condition('person.full_name', "%$getfullname%", 'like');
      }
      if(isset($_GET['Username'])){
        $getusername = $_GET['Username'];
        $listquery->condition('person.user_name', "%$getusername%", 'like');
      }
      if(isset($_GET['CompletionStatus'])){
        $completestatus = $_GET['CompletionStatus'];
       // $listquery->condition('regstat.name', "%$completestatus%", 'like');
         $listquery->where('slf_get_profile_list_name(enrol.overall_status,NULL)  like \'%'.$completestatus.'%\'');
        
      }
      if(isset($_GET['Score'])){
        $getscore = $_GET['Score'];
        $listquery->condition('enrol.score', "$getscore", '=');
      }
      if(isset($_GET['Grade'])){
        $getgrade = $_GET['Grade'];
        //$listquery->condition('enrol.grade', "%$getgrade%", 'like');
        $listquery->where('slf_get_profile_list_name(enrol.grade,NULL)  like \'%'.$getgrade.'%\'');
      }   
      
      // Sorting related
      $sord = $sord==''?'asc' : $sord;
      if($sidx == 'Username'){
        $listquery->orderBy('person.user_name', $sord);
      } else if($sidx == 'FullName'){
        $listquery->orderBy('person.full_name', $sord);
      } else if($sidx == 'CompletionStatus'){
        $listquery->orderBy('enrol.overall_status', $sord);
      } else if($sidx == 'Score'){ 
        $listquery->orderBy('enrol.score', $sord);
      } else if($sidx == 'Grade'){
        $listquery->orderBy('enrol.grade', $sord);
      } else {
        $listquery->orderby('enrol.id', 'asc');
      }
      
      expDebug::dPrintDBAPI('listEnrolledUsers query : ',$listquery);
      //$result = $listquery->execute()->fetchAll();
       
      $count = 0;
      $count = count($listquery->execute()->fetchAll());
    
      /* Paging Related */
      $start = $limit * $page - $limit;
      $listquery->range($start, $limit);
    
      // Execute query
      $queryResult = $listquery->execute();
      expDebug::dPrint('listEnrolledUsers : '.print_r($queryResult, true),3);
      
      if( $count >0 ){ 
        $total_pages = ceil($count/$limit); 
      }else{ 
        $total_pages = 0; 
      }	  
      if($page > $total_pages) 
      	$page = $total_pages;
      if($start < 0) {
      	$start = 0;
      }
      
      $pagination['events_filter'] = '0';
      if(isset($_GET['FullName']) ||isset($_GET['Username']) || isset($_GET['CompletionStatus']) ||isset($_GET['Score'])|| isset($_GET['Grade'])){
      
       $pagination['events_filter'] = '1'; 
      }
      
      $pagination['result'] = $queryResult;
      $pagination['page'] = $page;
      $pagination['count'] = $count;
      $pagination['total_pages'] = $total_pages;
        
      return $pagination; 
  } catch (Exception $ex) {
    watchdog_exception('listProgramEnrolledUsers', $ex);
    expertusErrorThrow($ex);
  }       
}

function getProgramDropDownList($code){
  try {  
      foreach(getProfileItemNamesKeyed($code) as $dropDownIndex => $dropDownValue){
        $dropDownListArray[] = $dropDownIndex .'$$$'. $dropDownValue;
      }
      $dropDownList = implode('###', $dropDownListArray);
      expDebug::dPrint('$dropDownList - '. $dropDownList, 4);
      print $dropDownList;
  } catch (Exception $ex) {
    watchdog_exception('getProgramDropDownList', $ex);
    expertusErrorThrow($ex);
  }       
}

function updateProgramRoster($params, $response = 0, $callFromAPI = 0){
  try {
      expDebug::dPrint('updateRoster starts '.print_r($params, true), 4);
      
      list($programId, $entityType, $selectedEnrolledIds, $registrationStatus, $registrationStatusDate, $completionStatus, $completionDate, $grade, $score, $contentValidFrom, $contentValidTo) = explode('$$$', $params);
    
      $registrationStatus  = ($registrationStatus == null || $registrationStatus == 'undefined' || $registrationStatus == '' ) ? '' : $registrationStatus;
      $completionStatus    = ($completionStatus == null || $completionStatus == 'undefined' || $completionStatus == '' ) ? '' : $completionStatus;
      $score 			   = empty($score) ? '' : $score;
      $grade 			   = empty($grade) ? '' : $grade;
      
      $completionDate = str_replace('-', '/', $completionDate);
      $registrationStatusDate = str_replace('-', '/', $registrationStatusDate);
      $completionDate	   = ($completionDate == null || $completionDate == 'undefined' || $completionDate=='') ? '' : date('Y-m-d H:i',strtotime($completionDate));
      $registrationStatusDate	   = ($registrationStatusDate == null || $registrationStatusDate == 'undefined' || $registrationStatusDate=='') ? '' : date('Y-m-d H:i',strtotime($registrationStatusDate));
      
      $contentValidFrom	   = ($contentValidFrom == null || $contentValidFrom == 'undefined' || $contentValidFrom == '') ? '' : date('Y-m-d H:i',strtotime($contentValidFrom));
      $contentValidTo	   = ($contentValidTo == null || $contentValidTo == 'undefined' || $contentValidTo == '') ? '' : date('Y-m-d H:i',strtotime($contentValidTo));
      $classId 		       = empty($classId) ? "null" : $classId;
      $courseId 		   = empty($courseId) ? "null" : $courseId;
              
      $selectedEnrolledIdsArray = explode(',', $selectedEnrolledIds);
      $selectedEnrolledIdsCount = count($selectedEnrolledIdsArray);
      
      $responseContent = '';
      $opRosterArr = array();
      for($i = 0; $i < $selectedEnrolledIdsCount; $i++){
        
        expDebug::dPrint('Roster $rosterStatus starts');
        $enrollId = $selectedEnrolledIdsArray[$i];
    
        $enrollQuery = db_select('slt_master_enrollment', 'enroll');
        $enrollQuery->addField('person', 'user_name' ,'username');
        $enrollQuery->addField('person', 'id' ,'enrollUserId');
        $enrollQuery->addField('enroll', 'overall_status' ,'overall_status');
        $enrollQuery->leftjoin('slt_person', 'person', 'person.id = enroll.user_id');
        $enrollQuery->condition('enroll.id', $enrollId);
        $enrollResult = $enrollQuery->execute()->fetchAssoc();
        $dbUserName = $enrollResult['username'];
        $enrollUserId = $enrollResult['enrolluserid'];
        $overallStatus = $enrollResult['overall_status'];
        
        $sessionFlag = isSessionStartForTPinClassLevel($enrollId);
        expDebug::dPrint('The value of $overallStatus is here vetrivel '.print_r($overallStatus,1),4);
        if($completionStatus == 'lrn_crs_cmp_enr'){
          $registrationStatus = 'lrn_crs_reg_cnf';
        } 
        
        if($overallStatus == 'lrn_tpm_ovr_cmp'){
          $errorMsg = t('ERR115');
        }
        else if($overallStatus == 'lrn_tpm_ovr_cln'){
          $errorMsg = t('MSG609');
        }
        else if($overallStatus == 'lrn_tpm_ovr_wtl' && $registrationStatus == 'lrn_crs_reg_cnf'){
          $errorMsg =  t('MSG439').' '. t('Class').' '. t('LBL267');
        }else if($sessionFlag != 1 && $registrationStatus != 'lrn_crs_reg_can'){
          $errorMsg =  t('Cannot change the status for future class.');
        }else {
          // Call course complete
          // CALL slp_mark_course_complete(xprogram_id,xcourse_id,xclass_id,xenroll_id,xupduser_id,xreg_status,xreg_status_dt,xcomp_status,xcomp_date,xscore,@xresult);
          $isFromAdmin = 1; // Variable to notify the completion marking is happen by TP Roster
          $msg = markCourseComplete($programId, $enrollId, $enrollUserId, $registrationStatus, $registrationStatusDate, $completionStatus, $completionDate, $score, $grade,$isFromAdmin);
          $waive_flag = false;
          if(isset($_SESSION['enrollment_id_mark_complete_'.$selectedEnrolledIds]) || isset($_SESSION['enrollment_id_mark_complete_revert_'.$selectedEnrolledIds])) {
          	$waive_flag = true;
          }
         if(!$waive_flag){
          $errorMsg = empty($msg) ? t('LBL356') : t('MSG748');
          }else {
          	$chngByLbl = (isset($_SESSION['enrollment_id_mark_complete_'.$selectedEnrolledIds])) ? t("Changed to Waived by")." " : t("MSG749")." ";
          	$uid = getIdOfLoggedInUser();
          	$update_person_details = getPersonDetails($uid) ;
          	$user_details = $chngByLbl.$update_person_details['full_name'];
          	$errorMsg = $user_details;
          	unset($_SESSION['enrollment_id_mark_complete_'.$selectedEnrolledIds]);
          	unset($_SESSION['enrollment_id_mark_complete_revert_'.$selectedEnrolledIds]);
          }
        }
        
        if($errorMsg != t('LBL356')){
          $responseContent .= '<tr>';
          $responseContent .= '<td>'.$dbUserName.' - </td>';
          $responseContent .= '<td>'.$errorMsg.'</td>';
          $responseContent .= '</tr>';
        }
        if($callFromAPI){
        	$enrollQuery = db_select('slt_master_enrollment', 'mse');
        	$enrollQuery->addExpression('count(1)', 'cnt');
        	$enrollQuery->condition('mse.id', $enrollId);
        	$enrollQuery->condition('mse.program_id', $programId);
        	$enrCnt = $enrollQuery->execute()->fetchField();
        	
          $opRosterArr['Id']      = $enrollId;
          $opRosterArr['Status']  = (empty($enrCnt)) ?  t('ERR171'). ' ' . t('LBL272') : $errorMsg;
          $opRosterArr['UserId']  = $enrollUserId;
          $opRosterArr['UserName']= $dbUserName;
        }    
      }
      // To sync solr data
      syncSolrData('MasterEnrollment');
      
      if($callFromAPI){
        return $opRosterArr;
      }
      expDebug::dPrint('Roster Overall Status = '.print_r($rosterOverallStatus, true), 4);
      expDebug::dPrint('Roster $responseContent = '.print_r($responseContent, true), 4);
      if($response)
        return $responseContent;
      else
        print $responseContent;
  } catch (Exception $ex) {
    watchdog_exception('updateProgramRoster', $ex);
    expertusErrorThrow($ex);
  }         
}

// To complete course
function markCourseComplete($programId, $masterEnrollId, $enrollUserId, $registrationStatus, $registrationStatusDate, $completionStatus, $completionDate, $score, $grade,$isFromAdmin=0){
  try {
      $msg = '';
      $notUpdateId = '';
      $userId = getSltpersonUserId();
      $currentDateTime = now();
      $emptyDateTime = NULL;
      
      $recPathQuery = db_select('slt_master_enrollment', 'master_enroll');
      $recPathQuery->addField('master_enroll','recertify_path', 'recertify_path');
      $recPathQuery->condition('master_enroll.id', $masterEnrollId);
      $recPathCount = $recPathQuery->execute()->fetchField();
      
      $enrollIdList = array();
      $enrollQuery = db_select('slt_enrollment', 'enroll');
	  $enrollQuery->addField('enroll', 'reg_status' ,'regstatus');
	  $enrollQuery->addField('enroll', 'comp_status' ,'compstatus');
      $enrollQuery->addExpression('GROUP_CONCAT(enroll.id)', 'enrollIdList');
      $enrollQuery->condition('enroll.master_enrollment_id', $masterEnrollId);
      $enrollResult = $enrollQuery->execute()->fetchAssoc();
	  
	  $dbRegistrationStatus = $enrollResult['regstatus'];
	  $dbCompletionStatus = $enrollResult['compstatus'];
	  
	  if($recPathCount > 1 && ($registrationStatus == 'lrn_crs_reg_can' || $completionStatus == 'lrn_crs_cmp_inc')){
  		
		$selectStmt = db_select('slt_master_enrollment', 'ms');
		$selectStmt->addField('ms','id','id');
		$selectStmt->addField('ms','custom4','custom4');
		$selectStmt->addField('ms','comp_date','comp_date');
		$selectStmt->condition('ms.program_id',$programId,'=');
		$selectStmt->condition('ms.user_id',$enrollUserId,'=');
		$selectStmt->condition('ms.recertify_path',($recPathCount-1),'=');
		$selectStmt->orderBy('ms.id', 'desc');
		$selectStmt->range(0, 1);
		expDebug::dPrintDBAPI('previous enrollment status update query :',$selectStmt);
		$prev_menr = $selectStmt->execute()->fetchAll();
		expDebug::dPrint("prev enrollment  >> ".print_r($prev_menr,true),5);
		$prev_menrid = $prev_menr[0]->id;
		$prev_custom4 = $prev_menr[0]->custom4;
		$prev_comp = $prev_menr[0]->comp_date;
       	expDebug::dPrint("master enrollmetn id prev ". $prev_menrid ,5);
	       
       	$cert_days_expire = isset($config["cert_expire_reminder_days"]) ? $config["cert_expire_reminder_days"] : 10;
       	//get certificate expired or not
       	$select = db_select('slt_program', 'p');
       	$select->addExpression('(CASE WHEN(DATEDIFF(
               IF (p.expires_in_unit = \'days\', DATE_ADD(\''.$prev_comp.'\', INTERVAL p.expires_in_value DAY),
                     IF (p.expires_in_unit = \'months\', DATE_ADD(\''.$prev_comp.'\', INTERVAL p.expires_in_value MONTH),
                           DATE_ADD(\''.$prev_comp.'\', INTERVAL p.expires_in_value YEAR))),
               CURDATE()) < 0)THEN \'lrn_tpm_ovr_exp\'
               ELSE \'lrn_tpm_ovr_cmp\' END )','prev_status');
			
 	 	$select->condition('p.id',$programId);
 	 	expDebug::dPrintDBAPI('certificate expiry or not :',$select);
       	$prev_sts = $select->execute()->fetchField();
		
	  }
      
      if($registrationStatus == 'lrn_crs_reg_can'){
        /* Waitlist Promotion Classes */
        $listClasses = db_select('slt_enrollment', 'enr');
        $listClasses->addField('enr','class_id');
        $listClasses->addField('enr','course_id');
        $listClasses->addField('enr','reg_status');
        $listClasses->condition('enr.master_enrollment_id', $masterEnrollId,'=');
        $resultEnrClasses =$listClasses->execute()->fetchAll();
        
        //Get Previous master_enroll_status
        $selectOverallStatus = db_select('slt_master_enrollment','mas');
        $selectOverallStatus->addField('mas','overall_status');  
        $selectOverallStatus->condition('mas.id', $masterEnrollId);
        $prevOverallStatus   = $selectOverallStatus->execute()->fetchField();
      }
    
      if(count($enrollResult) > 0){
        $enrollIdList = explode(',', $enrollResult['enrollidlist']);
      }
      
      // Update enrollment
      $updateEnroll = db_update('slt_enrollment');
      // Update enrollment fields
      $updateEnrollFields['updated_by'] = $userId;
      $updateEnrollFields['updated_on'] = $currentDateTime;
      if(!empty($registrationStatus)){
        $updateEnrollFields['reg_status'] = $registrationStatus;
        if($registrationStatus == 'lrn_crs_reg_can'){
          $updateEnrollFields['comp_status'] = NULL;
          $updateEnroll->condition(db_or()->condition('comp_status', array('lrn_crs_cmp_enr','lrn_crs_cmp_inp'))
          		->isNull('comp_status'));
        }
      }
      if(!empty($registrationStatusDate)){
        $updateEnrollFields['reg_status_date'] = $registrationStatusDate;
      }
      if(!empty($completionStatus)){
        $updateEnrollFields['comp_status'] = $completionStatus;
        if($completionStatus == 'lrn_crs_cmp_cmp' && empty($completionDate)){
          $updateEnrollFields['comp_date'] = $currentDateTime;
          $updateEnrollFields['comp_on'] = $currentDateTime;
          $updateEnrollFields['comp_by'] = $userId;
        }
        if($completionStatus == 'lrn_crs_cmp_cmp'){
        	$updateEnroll->condition('comp_status',array('lrn_crs_cmp_cmp','lrn_crs_cmp_inc','lrn_crs_cmp_nsw'),'NOT IN');
        	$updateEnrollFields['progress']  = 100;
      	}
      	if($completionStatus == 'lrn_crs_cmp_inc'){
      		$updateEnroll->condition('comp_status',array('lrn_crs_cmp_cmp'),'NOT IN');//If admin/manager marks the TP as incomplete skip the completed classes #83881
      	}
      	
      }
      if(!empty($completionDate)){
        $updateEnrollFields['comp_date'] = $completionDate;
        $updateEnrollFields['comp_on'] = $completionDate;
        $updateEnrollFields['comp_by'] = $userId;
      }
    
      if(!empty($score)){
        $updateEnrollFields['score'] = $score;
      }
      if(!empty($grade)){
        $updateEnrollFields['grade'] = $grade;
      }
      $updateEnroll->fields($updateEnrollFields);
      // Update enrollment conditions
      if(!empty($programId) && !empty($masterEnrollId)){
        $updateEnroll->condition('master_enrollment_id', $masterEnrollId);
      }
      if(!empty($registrationStatus) && ($registrationStatus!='lrn_crs_reg_can') && (empty($completionStatus) || ($completionStatus != 'lrn_crs_cmp_cmp' && $completionStatus != 'lrn_crs_cmp_inc'))){
        $updateEnroll->condition('comp_status', 'lrn_crs_cmp_cmp');
      }
      if(!empty($registrationStatusDate) && ($registrationStatus!='lrn_crs_reg_can') && (empty($completionStatus) || ($completionStatus != 'lrn_crs_cmp_cmp' && $completionStatus != 'lrn_crs_cmp_inc'))){
        $updateEnroll->condition('comp_status', 'lrn_crs_cmp_cmp');
      }
      if(!empty($completionStatus) && (empty($registrationStatus) || $registrationStatus != 'lrn_crs_reg_cnf')){
        $updateEnroll->condition('reg_status', 'lrn_crs_reg_cnf');
      }
      if(!empty($completionDate) && (empty($registrationStatus) || $registrationStatus != 'lrn_crs_reg_cnf')){
        if($registrationStatus == 'lrn_crs_reg_can'){
          $updateEnroll->condition('reg_status', array('lrn_crs_reg_cnf','lrn_crs_reg_wtl'), 'IN');
        }
        else{
          $updateEnroll->condition('reg_status', 'lrn_crs_reg_cnf');
        }
      }
      if(!empty($score) && (empty($completionStatus) || $completionStatus != 'lrn_crs_cmp_cmp')){
        $updateEnroll->condition('comp_status', 'lrn_crs_cmp_cmp');
      }
      if(!empty($grade) && (empty($completionStatus) || $completionStatus != 'lrn_crs_cmp_cmp')){
        $updateEnroll->condition('comp_status', 'lrn_crs_cmp_cmp');
      }
      if(!empty($completionStatus) && $completionStatus == 'lrn_crs_cmp_cmp'){
        $updateEnroll->condition('comp_status', 'lrn_crs_cmp_cmp' ,'!=');
      }  
      if(empty($registrationStatus) || ($registrationStatus != 'lrn_crs_reg_can' && $registrationStatus != 'lrn_crs_reg_cnf')){
        $updateEnroll->condition('course_id', '', 'IS NOT NULL');
      }
    
      $updateEnroll->condition('id', $enrollIdList, 'IN');
      expDebug::dPrintDBAPI('Update Enrollment query', $updateEnroll);
      $notUpdateId = $updateEnroll->execute();
    
      // Master update
      if(!empty($registrationStatus) && $registrationStatus == 'lrn_crs_reg_can'){
    	/*
      	CALL slp_mark_program_roster_complete(xenroll_id,xmaster_enrollment_id,xprogram_id,xupduser_id);
      	*/
        /*foreach($enrollIdList as $id){
          $msg = programRosterCancel($masterEnrollId, $programId, $userId, $registrationStatusDate, $completionDate);
        }*/        
        $currentDateTime = now();
    
        $updateMaster = db_update('slt_master_enrollment');
        $updateMasterFields['overall_status'] = 'lrn_tpm_ovr_cln';
        /*if($recPathCount > 1){
        	$updateMasterFields['is_current'] = 'N';
        }*/
        $updateMasterFields['cancel_date'] = empty($registrationStatusDate) ? $currentDateTime : $registrationStatusDate;
        $updateMasterFields['updated_by'] = $userId;
        $updateMasterFields['updated_on'] = $currentDateTime;
        $updateMasterFields['custom4'] = 'lrn_tpm_ovr_cln';
        $updateMaster->fields($updateMasterFields);
        $updateMaster->condition('id', $masterEnrollId);
        $updateMaster->condition('overall_status', array('lrn_tpm_ovr_cln','lrn_tpm_ovr_cmp','lrn_tpm_ovr_inc','lrn_tpm_ovr_rsv','lrn_tpm_ovr_exp'), 'NOT IN');
        $updateMaster->condition('program_id', $programId);
        expDebug::dPrintDBAPI("Update TP with Cancel/Expired state ",$updateMaster);
        $notUpdateId = $updateMaster->execute();
        
      	/*if(count($prev_menr) > 0){    
	        $updateMasterFields1['overall_status'] = $prev_sts;
	        $updateMasterFields1['is_current'] = 'Y';
	        $updateMasterEnroll1 = db_update('slt_master_enrollment');
	        $updateMasterEnroll1->fields($updateMasterFields1);
	        $updateMasterEnroll1->condition('id', $prev_menrid);
	        $updateMasterEnroll1->condition('program_id', $programId);
	        expDebug::dPrintDBAPI('master enrollment update query 222 :',$updateMasterEnroll1);
	        $updateMasterEnroll1->execute();  
	    }*/
        
        $objecttype = db_select('slt_program', 'prg');
        $objecttype->addField('prg','object_type','objecttype');
        $objecttype->addField('prg','title','title');
        $objecttype->condition('id', $programId);
        $objResult = $objecttype->execute()->fetchAll();
        make_audit_trail_entries($updateMasterFields, 1 , null ,$programId, 'Updated',$objResult[0]->objecttype,'exp_sp_learning','rosterProgramCanceled',
        		array('overall_status','cancel_date', 'updated_by','updated_on','custom4'));
       if($notUpdateId > 0){ // Skip below oprations if nothing is update
	        // TODO: The following query need to remove after confirming is it used anywhere
        // Get list of user id
        $selectMaster = db_select('slt_master_enrollment', 'master');
        $selectMaster->addField('master', 'id', 'master_enroll_id');
        $selectMaster->addExpression('GROUP_CONCAT(master.user_id)', 'user_id_list');
        $selectMaster->condition('master.id', $masterEnrollId);
        $masterResult = $selectMaster->execute()->fetchAssoc();
        $userIdList = explode(',',$masterResult['user_id_list']);
        $dbMasterEnrollId = $masterResult['master_enroll_id'];
        
        // Role access
        $selectRole = db_select('slt_object_role_mapping', 'role');
        $selectRole->addExpression('GROUP_CONCAT(role.id)', 'role_map_id');
        $selectRole->condition('role.object_id', $programId);
        $selectRole->condition('role.object_type', 'cre_sys_obt_trp');
        //$selectRole->condition('role.role_code', 'cre_pgn_rls_clp');
        $selectRole->condition('role.user_id', $userIdList, 'IN');
        $roleResult = $selectRole->execute()->fetchAssoc();
        $roleIdList = explode(',', $roleResult['role_map_id']);
        
        // Delete mapping
        $delete = db_delete('slt_object_role_mapping');
        $delete->condition('id', $roleIdList, 'IN');
        $delete->execute();
        
        if($prevOverallStatus == 'lrn_tpm_ovr_wtl'){
    	  $objectId1 = $masterEnrollId;
  	      $objectId2 = $programId;
  	      $notificationType = "certCurrLPWaitlistCancel";
	    		}else{
    	  $objectId1 = $programId;
  	    $objectId2 = $masterEnrollId;
    	  $notificationType = "certCurrLPCancel";
    	}
    	  $iCal = null;
    	  if ($notificationType == 'certCurrLPCancel') {
    	  	$iCal = getCancelICal($masterEnrollId, 'TP');
    	  }
        insertNotification($notificationType, $objectId1, $objectId2, $enrollUserId, $userId, $iCal);
        
        /* Waitlist Promotion Call */
        foreach ($resultEnrClasses as $record) {
          $classId = $record->class_id;        
          $prevRegStatus = $record->reg_status;   
          waitlistPromotionUpdate($classId,$prevRegStatus);        
        }
        insertUserPointsForEachActionsPerformed($enrollUserId,'register_class',$masterEnrollId,'delete','tp');
      	}
      } else if(!empty($registrationStatus) && $registrationStatus == 'lrn_crs_reg_cnf'){

        $updateMaster = db_update('slt_master_enrollment');
        $updateMasterFields['overall_status'] = 'lrn_tpm_ovr_enr';
        $updateMasterFields['updated_by'] = $userId;
        $updateMasterFields['updated_on'] = $currentDateTime;
        $updateMaster->fields($updateMasterFields);
        $updateMaster->condition('id', $masterEnrollId);
        $updateMaster->condition('overall_status',array('lrn_tpm_ovr_cmp','lrn_tpm_ovr_ppv','lrn_tpm_ovr_ppm','lrn_tpm_ovr_rsv'), 'NOT IN');
        expDebug::dPrintDBAPI("Update TP with Confirm state ",$updateMaster);
        $notUpdateId = $updateMaster->execute();
    
        if($notUpdateId > 0){ // Skip below oprations if nothing is update
        $notificationType = "certCurrLPRegister";
        $iCal = getPublishICal($masterEnrollId, 'TP');
		if(!isset($_SESSION['waived_enroll_id_'.$masterEnrollId]) && ($dbCompletionStatus != $vCompletionStatus)){
        	insertNotification($notificationType, $masterEnrollId, $programId, $enrollUserId, $userId, $iCal);
		}else{
			if($_SESSION['waived_enroll_id_'.$masterEnrollId] == $masterEnrollId){
				unset($_SESSION['waived_enroll_id_'.$masterEnrollId]);	
			}			
		}
        }
      }  else if($completionStatus == 'lrn_crs_cmp_inc'){
        
        $updateMaster = db_update('slt_master_enrollment');
        $updateMasterFields['overall_status'] = 'lrn_tpm_ovr_inc';
        /*if($recPathCount > 1){
        	$updateMasterFields['is_current'] = 'N';
        }*/
        $updateMasterFields['comp_date'] = $currentDateTime;
        $updateMasterFields['updated_by'] = $userId;
        $updateMasterFields['updated_on'] = $currentDateTime;
        $updateMasterFields['custom4'] = 'lrn_tpm_ovr_inc';
        $updateMaster->fields($updateMasterFields);
        $updateMaster->condition('id', $masterEnrollId);
        $updateMaster->condition('overall_status', 'lrn_tpm_ovr_cmp', '!=');
        $updateMaster->condition('overall_status', array('lrn_tpm_ovr_enr','lrn_tpm_ovr_inp'), 'IN');
        expDebug::dPrintDBAPI("Update TP with Incomplete state ",$updateMaster);
        $notUpdateId = $updateMaster->execute();
        
      /*if(count($prev_menr) > 0){    
	        $updateMasterFields1['overall_status'] = $prev_sts;
	        $updateMasterFields1['is_current'] = 'Y';
	        $updateMasterEnroll1 = db_update('slt_master_enrollment');
	        $updateMasterEnroll1->fields($updateMasterFields1);
	        $updateMasterEnroll1->condition('id', $prev_menrid);
	        $updateMasterEnroll1->condition('program_id', $programId);
	        expDebug::dPrintDBAPI('master enrollment update query 222 :',$updateMasterEnroll1);
	        $updateMasterEnroll1->execute();  
	    }*/
        if($notUpdateId > 0){ // Skip below oprations if nothing is update
        $notifyBy = '';
        $classId = '';
        $courseId = '';
        $type = 'program';
        $objParam = new stdClass();
        $objParam->compstatus = $completionStatus;
        foreach($enrollIdList as $enrollId){
          markCompletionCrsCls($enrollId, $classId, $courseId, $enrollUserId, $completionDate, $score, $grade, $notifyBy, $type,$objParam,$isFromAdmin);
        }
      }
      }else if($completionStatus == 'lrn_crs_cmp_cmp'){      
        // CALL slp_mark_course_complete_class_level(xenrollid,xupdateuserid,xcompDate,xScore,xnotifyBy);
        $notifyBy = '';
        $classId = '';
        $courseId = '';
        $type = 'program';
        $objParam = new stdClass();
        $objParam->compstatus = $completionStatus;
        foreach($enrollIdList as $enrollId){
          markCompletionCrsCls($enrollId, $classId, $courseId, $enrollUserId, $completionDate, $score, $grade, $notifyBy, $type,$objParam,$isFromAdmin);
        }
        
      }
      if($notUpdateId == 0)
      	$msg = $masterEnrollId;
      syncSolrData('Enrollment');
      syncSolrData('MasterEnrollment');
      syncSolrData('User');
      return $msg;
  } catch (Exception $ex) {
    watchdog_exception('markCourseComplete', $ex);
    expertusErrorThrow($ex);
  }       
  
}

function programRosterCancel($enrollId, $programId, $userId, $registrationStatusDate, $completionDate){
  try {
      $currentDateTime = now();
      /*
      $masterSelect = db_select('slt_master_enrollment', 'master');
      $masterSelect->addField('master', 'user_id');
      $masterSelect->condition('master.overall_status', array('lrn_tpm_ovr_cmp','lrn_tpm_ovr_cln'), 'NOT IN');
      $masterSelect->condition('master.program_id', $programId);
      $masterSelect->condition('master.comp_date', '', 'IS NULL');
      $masterResult = $masterSelect->execute()->fetchAssoc();
      $dbUserId = $masterResult['user_id'];
    */
      
      $enrollSelect = db_select('slt_enrollment', 'e');
      $enrollSelect->addField('e', 'master_enrollment_id');
      $enrollSelect->addField('e', 'user_id');
      $enrollSelect->addExpression("MAX(CASE WHEN (e.reg_status='lrn_crs_reg_can' OR (e.reg_status='lrn_crs_reg_cnf' AND e.comp_status='lrn_crs_cmp_cmp')) THEN 0 ELSE 1 END)", 'completion_status');
      $enrollSelect->condition('e.id', $enrollId);
      $enrollSelect->groupby('e.master_enrollment_id');
      $enrollSelect->groupby('e.user_id');
      //print_r($enrollSelect->__toString());
       
      $enrollResult = $enrollSelect->execute()->fetchAssoc();
      $completionStatus = $enrollResult['completion_status'];
      
      if(count($enrollResult) > 0 && $completionStatus == 0){
        
        $dbMasterEnrollId = $enrollResult['master_enrollment_id'];
        $dbUserId = $enrollResult['user_id'];
    
        $updateMaster = db_update('slt_master_enrollment');
        $updateMasterFields['overall_status'] = 'lrn_tpm_ovr_cln';
        $updateMasterFields['cancel_date'] = empty($registrationStatusDate) ? $currentDateTime : $registrationStatusDate;
        $updateMasterFields['updated_by'] = $userId;
        $updateMasterFields['updated_on'] = $currentDateTime;
        $updateMaster->fields($updateMasterFields);
        $updateMaster->condition('id', $dbMasterEnrollId);
        $updateMaster->condition('user_id', $dbUserId);
        $updateMaster->condition('overall_status', array('lrn_tpm_ovr_cln','lrn_tpm_ovr_cmp'), 'NOT IN');
        $updateMaster->condition('program_id', $programId);
        $updateMaster->execute();
        
        return '';
        
      } else {
        
        return 'Cannot be cancelled';
        
      }
  } catch (Exception $ex) {
    watchdog_exception('programRosterCancel', $ex);
    expertusErrorThrow($ex);
  }       
  
}

function countenrollments($programId){
  try {
    $select = db_select('slt_master_enrollment', 'master_enroll');
    $select->addExpression('COUNT(1)','total_enroll');
    $select->condition('master_enroll.program_id', $programId, '=');
    $select->condition('master_enroll.overall_status',array('lrn_tpm_ovr_cmp','lrn_tpm_ovr_enr','lrn_tpm_ovr_inp','lrn_tpm_ovr_ppv','lrn_tpm_ovr_ppm','lrn_tpm_ovr_exp','lrn_tpm_ovr_wtl'), 'IN');
    $total_enroll = $select->execute()->fetchField();
    expDebug::dPrint('Results in overall status module: '.print_r($total_enroll, true),3);
    return $total_enroll;
  } catch (Exception $ex) {
    watchdog_exception('countenrollments', $ex);
    expertusErrorThrow($ex);
  } 
} 