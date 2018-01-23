<?php 


/*
 * launchRoster() 
 */
function launchRoster($courseId, $classId, $otherParams){
  try {
      //$usersList = listEnrolledUsers($courseId, $classId);
    
      $qry_search_results = listEnrolledUsers($courseId, $classId, $otherParams);
      $srch_result_response_roster->total   = $qry_search_results['total_pages'];
      $srch_result_response_roster->records = $qry_search_results['count'];
      $srch_result_response_roster->page    = $qry_search_results['page'];
      $srch_result_response_roster->classId = $classId;
      $srch_result_response_roster->courseId = $courseId;
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
        if($rec_search_results->comp_status_code == 'lrn_crs_cmp_enr'){
        $enrolledCnt++;  
        }
       if($rec_search_results->comp_status_code == 'lrn_crs_cmp_cmp'){
           $completedCnt++;
         }
       if($rec_search_results->comp_status_code == 'lrn_crs_cmp_inp'){
           $inprogresCnt++;
         }
       if($rec_search_results->reg_status_code == 'lrn_crs_reg_can'){
           $cancelCount++;
         }
       if($rec_search_results->comp_status_code == 'lrn_crs_cmp_inc'){
           $incompletCount++;
         }
      if($rec_search_results->reg_status_code == 'lrn_crs_reg_wtl'){
           $waitlstCount++;
         } 
      if($rec_search_results->reg_status_code == 'lrn_crs_reg_ppm'){
           $pendingCount++;
         }
      if($rec_search_results->comp_status_code == 'lrn_crs_cmp_nsw'){
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
       $srch_result_response_roster->count_result   = 'Statistics - Enrolled: '.$enrolledCnt.', Completed: '.$completedCnt.', In-Progress: '.$inprogresCnt.', Incomplete: '.$incompletCount.', No Show: '.$showCount.', Waitlist: '.$waitlstCount.', Pending: '.$pendingCount.', Canceled: '.$cancelCount.'';
       $srch_result_response_roster->count_result = str_replace(',','<span class="narrow-search-results-item-detail-pipe-line">|</span>',$srch_result_response_roster->count_result);
       
      echo drupal_json_encode($srch_result_response_roster);
  } catch (Exception $ex) {
    watchdog_exception('catalogRoster', $ex);
    expertusErrorThrow($ex);
  }       
}

function listEnrolledUsers($courseId, $classId){
  
  try {
      list($userName, $fullName, $regStatus, $compStatus, $score, $grade) = explode('$$$', $otherParams);
      
      // slp_bulkreg_roster_catalog_registered_users_sel
      
      $page          = $_GET['page']; 
      $limit         = $_GET['rows']; 
      $sidx          = $_GET['sidx'];
      $sord          = $_GET['sord'];
      $tab           = $_GET['tab'];
      $getusername   = $_GET['Username'];
        
      $listquery = db_select('slt_enrollment', 'reg');
      $listquery->addField('reg', 'id', 'enrol_pid');
      $listquery->addField('reg', 'score', 'score');
      $listquery->addField('reg', 'reg_status', 'reg_status_code');
      $listquery->addField('reg', 'comp_status', 'comp_status_code');
      
      
      $listquery->addField('per', 'user_name', 'user_name');
      $listquery->addField('per', 'full_name', 'userfullname');
      $listquery->addField('per', 'email', 'email');
      $listquery->addField('reg', 'class_id', 'class_id');
      $listquery->addField('reg', 'course_id', 'course_id');
      
      $listquery->addExpression('slf_get_profile_list_name(reg.reg_status,NULL)', 'reg_status');
      $listquery->addExpression('slf_get_profile_list_name(reg.comp_status,NULL)', 'comp_status');
      $listquery->addExpression('slf_get_profile_list_name(reg.grade,NULL)', 'grade');
      $listquery->addExpression('slf_get_profile_list_name(orde.order_status,NULL)', 'payment_status ');
      $listquery->addExpression('DATE_FORMAT(reg.reg_status_date,\'%m/%d/%Y\')', 'reg_status_date');
      $listquery->addExpression('DATE_FORMAT(reg.valid_to,\'%m/%d/%Y\')', 'valid_to');
      $listquery->addExpression('DATE_FORMAT(reg.comp_date,\'%m/%d/%Y\')', 'comp_date');
      $listquery->addExpression('DATE_FORMAT(reg.reg_date,\'%m/%d/%Y\')', 'reg_date');
      
      $listquery->innerjoin('slt_person', 'per', 'reg.user_id= per.id');
      $listquery->leftjoin('slt_order', 'orde', 'orde.id=reg.order_id'); 
      
      
      $listquery->condition('reg.class_id', $classId, '=');
      $listquery->condition('reg.course_id', $courseId, '=');
      $listquery->condition('reg.reg_status', array('lrn_crs_reg_rsc', 'lrn_crs_reg_rsv'), 'NOT IN');
      $listquery->condition('reg.master_enrollment_id', '', 'IS NULL' );
      
      if(!empty($userName) && $userName != 'null'){
        $listquery->condition('per.user_name', "%$classId%", 'like');
      }
      
      if(isset($_GET['FullName'])){
        $getfullname = $_GET['FullName'];
        $listquery->condition('per.full_name', "%$getfullname%", 'like');
      }
      if(isset($_GET['Username'])){
        $getusername = $_GET['Username'];
        $listquery->condition('per.user_name', "%$getusername%", 'like');
      }
      if(isset($_GET['RegistrationStatus'])){
        $getregstatus = $_GET['RegistrationStatus'];
        $listquery->where('slf_get_profile_list_name(reg.reg_status,NULL)  like \'%'.$getregstatus.'%\'');
        //$listquery->condition('slf_get_profile_list_name(reg.reg_status,NULL), \'%'.$getregstatus.'%\'', 'like');
      } 
      if(isset($_GET['CompletionStatus'])){
        $completestatus = $_GET['CompletionStatus'];
        $listquery->where('slf_get_profile_list_name(reg.comp_status,NULL)  like \'%'.$completestatus.'%\'');
        //$listquery->condition('reg.comp_status', "%$completestatus%", 'like');
      }
      if(isset($_GET['Score'])){
        $getscore = $_GET['Score'];
        $listquery->condition('reg.score', "$getscore", '=');
      }
      if(isset($_GET['Grade'])){
        $getgrade = $_GET['Grade'];
        //$listquery->condition('reg.grade', "%$getgrade%", 'like');
        $listquery->where('slf_get_profile_list_name(reg.grade,NULL)  like \'%'.$getgrade.'%\'');
      }   
    
      
      // Sorting related
      $sord = $sord==''?'asc' : $sord;
      if($sidx == 'Username'){
        $listquery->orderBy('per.user_name', $sord);
      } else if($sidx == 'FullName'){
        $listquery->orderBy('per.full_name', $sord);
      } else if($sidx == 'RegistrationStatus'){
        $listquery->orderBy('reg.reg_status', $sord);
      } else if($sidx == 'CompletionStatus'){
        $listquery->orderBy('reg.comp_status', $sord);
      } else if($sidx == 'Score'){ 
        $listquery->orderBy('reg.score', $sord);
      } else if($sidx == 'Grade'){
        $listquery->orderBy('reg.grade', $sord);
      } else {
        $listquery->orderby('reg.id', 'asc');
      }
      
      expDebug::dPrintDBAPI(' $insertStmt obj = ' , $listquery);  
      //$result = $listquery->execute()->fetchAll();
       
      $count = 0;
      $count = count($listquery->execute()->fetchAll());
    
      /* Paging Related */
      $start = $limit * $page - $limit;
      $listquery->range($start, $limit);
    
      // Execute query
      $queryResult = $listquery->execute();
      expDebug::dPrint('listEnrolledUsers : '.print_r($queryResult, true), 3);
      
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
      if(isset($_GET['FullName']) ||isset($_GET['Username']) ||isset($_GET['RegistrationStatus']) || isset($_GET['CompletionStatus']) ||isset($_GET['Score'])|| isset($_GET['Grade'])){
      
       $pagination['events_filter'] = '1'; 
      }
      
      $pagination['result'] = $queryResult;
      $pagination['page'] = $page;
      $pagination['count'] = $count;
      $pagination['total_pages'] = $total_pages;
        
      return $pagination; 
  } catch (Exception $ex) {
    watchdog_exception('catalogRoster', $ex);
    expertusErrorThrow($ex);
  }       
}

function getDropDownList($code){  
  try {
      foreach(getProfileItemNamesKeyed($code) as $dropDownIndex => $dropDownValue){
        $dropDownListArray[] = $dropDownIndex .'$$$'. $dropDownValue;
      }
      $dropDownList = implode('###', $dropDownListArray);
      expDebug::dPrint('$dropDownList - '. $dropDownList, 4);
      print $dropDownList;
   } catch (Exception $ex) {
    watchdog_exception('catalogRoster', $ex);
    expertusErrorThrow($ex);
  } 
}

function updateRoster($params, $response = 0, $callFromAPI = 0){
  try {
      expDebug::dPrint('updateRoster starts '.print_r($params, true), 4);
      
      list($courseId, $classId, $selectedEnrolledIds, $registrationStatus, $registrationStatusDate, $completionStatus, $completionDate, $grade, $score, $contentValidFrom, $contentValidTo) = explode('$$$', $params);
    
      $registrationStatus  = ($registrationStatus == null || $registrationStatus == 'undefined' || $registrationStatus == '' ) ? 'null' : $registrationStatus;
      $completionStatus    = ($completionStatus == null || $completionStatus == 'undefined' || $completionStatus == '' ) ? 'null' : $completionStatus;
      $score 			   = empty($score) ? 0 : $score;
      $grade 			   = empty($grade) ? 0 : $grade;
      
      $completionDate = str_replace('-', '/', $completionDate);
      $registrationStatusDate = str_replace('-', '/', $registrationStatusDate);
      $contentValidFrom = str_replace('-', '/', $contentValidFrom);
      $contentValidTo = str_replace('-', '/', $contentValidTo);
      
      expDebug::dPrint('Completion Date - Registration Date - 1 : '.$completionDate.' '.$registrationStatusDate, 4);
      
      $completionDate	   = ($completionDate == null || $completionDate == 'undefined' || $completionDate=='') ? 'null' : date('Y-m-d H:i',strtotime($completionDate));
      $registrationStatusDate	   = ($registrationStatusDate == null || $registrationStatusDate == 'undefined' || $registrationStatusDate=='') ? 'null' : date('Y-m-d H:i',strtotime($registrationStatusDate));
      
      expDebug::dPrint('Completion Date - Registration Date : '.$completionDate.' '.$registrationStatusDate, 4);
      
      $contentValidFrom	   = ($contentValidFrom == null || $contentValidFrom == 'undefined' || $contentValidFrom == '') ? 'null' : date('Y-m-d H:i',strtotime($contentValidFrom));
      $contentValidTo	   = ($contentValidTo == null || $contentValidTo == 'undefined' || $contentValidTo == '') ? 'null' : date('Y-m-d H:i',strtotime($contentValidTo));
      $classId 		       = empty($classId) ? "null" : $classId;
      $courseId 		   = empty($courseId) ? "null" : $courseId;
              
      $selectedEnrolledIdsArray = explode(',', $selectedEnrolledIds);
      $selectedEnrolledIdsCount = count($selectedEnrolledIdsArray);
      
      $responseContent = '';  
      $opRosterArr = array();
      for($i = 0; $i < $selectedEnrolledIdsCount; $i++){
        $enrollId = $selectedEnrolledIdsArray[$i];
        expDebug::dPrint('Roster $rosterStatus starts');
        // Validation and get the status message to display
        $rosterStatus = getCatalogRosterStatus($courseId, $classId, $enrollId, $registrationStatus, $registrationStatusDate, $completionStatus, $completionDate, $grade, $score, $contentValidFrom, $contentValidTo,$callFromAPI);
        $rosterOverallStatus[$enrollId] = $rosterStatus;
        expDebug::dPrint('Roster $rosterStatus = '.print_r($rosterStatus, true), 4);
        // Form the response content 
        
        
        $regStatusResponse = $rosterStatus['reg_status'] == 'null' ? '-' : ( $rosterStatus['reg_status'] == 'Ok' ? 'Updated' : $rosterStatus['reg_status'] ); 
        $compStatusResponse = $rosterStatus['comp_status'] == 'null' ? '-' : ( $rosterStatus['comp_status'] == 'Ok' ? 'Updated' : $rosterStatus['comp_status'] );
        $scoreResponse = $rosterStatus['score_status'] == 'null' ? '-' : ( $rosterStatus['score_status'] == 'Ok' ? 'Updated' : $rosterStatus['score_status'] );
        $gradeResponse = $rosterStatus['grade_status'] == 'null' ? '-' : ( $rosterStatus['grade_status'] == 'Ok' ? 'Updated' : $rosterStatus['grade_status'] );
        
        //Added "&& $rosterStatus['score_status'] != 'Ok'" for ticket #16730: Error Message when updating the score in the Roster Page 
        if($regStatusResponse != 'Updated' && $rosterStatus['score_status'] != 'Ok'){
          $responseContent .= '<tr>';
          $responseContent .= '<td>'.$rosterStatus['user_name'].' - </td>';
          $responseContent .= '<td>'.$regStatusResponse.'</td>';
          //$responseContent .= '<td>'.$compStatusResponse.'</td>';
          //$responseContent .= '<td>'.$scoreResponse.'</td>';
          //$responseContent .= '<td>'.$gradeResponse.'</td>';
          $responseContent .= '</tr>';
        }
        
        // Begin - Update the status after the validation
        if($rosterStatus['reg_status'] == 'Ok') {
        	$vRegistrationStatus = $registrationStatus;
        	$vRegistrationStatusDate	= $registrationStatusDate;
        } else {
        	$vRegistrationStatus = 'null';
        	$vRegistrationStatusDate	= 'null';
        }
    
    	if($rosterStatus['comp_status'] == 'Ok') {
    		$vCompletionStatus   = $completionStatus;
    		$vCompletionDate 	 = ($completionDate == 'null') ? 'null' : $completionDate;
    	} else {
    		$vCompletionStatus = 'null';
    		$vCompletionDate   = 'null';
    	}
    
    	$vPaymentStatus = 'null';
    	$vPaymentDate 	= 'null';
    
    	if($rosterStatus['val_status'] == 'Ok') {
    		$vContentValidFrom	= $contentValidFrom;
    		$vContentValidTo	= $contentValidTo;
    	} else {
    		$vContentValidFrom	= 'null';
    		$vContentValidTo	= 'null';
    	}
    
    	if($rosterStatus['score_status'] == 'Ok') {
    		$vScore	= $score;
    	} else {
    		$vScore	= 'null';
    	}
    
    	if($rosterStatus['grade_status'] == 'Ok') {
    		$vGrade	= $grade;
    	} else {
    		$vGrade	= 'null';
    	}
    	$userId = $rosterStatus['user_id'];    
    	$orderId = $rosterStatus['order_id'];
    	// End - Update the status after the validation
    	
    	
        // Update enrollment table
      $notUpdatedId =  updateEnrollment($enrollId, $vRegistrationStatus, $vRegistrationStatusDate, $vCompletionStatus, $vCompletionDate, $vPaymentStatus, $vPaymentDate, $vContentValidFrom, $vContentValidTo, $vScore, $vGrade, $userId, $orderId, $classId, $courseId, $callFromAPI);

      if(!empty($notUpdatedId)){
      	$reson = t('MSG748');
      	$qry = "SELECT concat('<tr><td>', per.user_name, ' - </td><td>', '".$reson."', '</td></tr>') status
      					FROM slt_person per
      					INNER JOIN slt_enrollment enr ON enr.user_id = per.id 
      					WHERE enr.id = ".$notUpdatedId ;
      	expDebug::dPrintDBAPI("Qry to list users who's data not updated ",$qry);
      	$rst = db_query($qry)->fetchAll();
      	$responseContent .= $rst[0]->status;
      } 
      
        if($callFromAPI){
          $opRosterArr['Id']      = $enrollId;
          $opRosterArr['Status']  = $regStatusResponse;
          $opRosterArr['UserId']  = $userId;
          $opRosterArr['UserName']= $rosterStatus['user_name'];
        }    
      }
      // To sync solr data
      
      syncSolrData('Enrollment','','RosterBulk');
      syncSolrData('MasterEnrollment','','RosterBulk');
      syncSolrData('User','','Bulk');
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
    watchdog_exception('catalogRoster', $ex);
    expertusErrorThrow($ex);
  }         
}

// To update the enrollment details 
function updateEnrollment($enrollId, $vRegistrationStatus, $vRegistrationStatusDate, $vCompletionStatus, $vCompletionDate, $vPaymentStatus, $vPaymentDate, $vContentValidFrom, $vContentValidTo, $vScore, $vGrade, $enrollUserId, $orderId, $classId, $courseId, $callFromAPI = 0){
   try {
      $default= db_query("SELECT id,reg_status,reg_status_date,comp_status,waitlist_priority,score,comp_date from slt_enrollment where course_id=:cid and class_id = :lid and order_id = :oid",array(":cid"=>$courseId ,":lid"=>$classId, ":oid"=>$orderId))->fetchAll();
	  $waived =  db_query("SELECT exempted_status from slt_enrollment_exempted where enrollment_id=:eid",array(":eid"=>$enrollId))->fetchField();
      $userId = getSltpersonUserId();
      $currentDateTime = now();
      $emptyValue = NULL;
      $notUpdatedId= '';
      $updateFields = array();
      $enrollQuery = db_select('slt_enrollment', 'enroll');
      $enrollQuery->leftJoin('slt_course_class','scc','enroll.class_id=scc.id');
      $enrollQuery->addField('scc', 'delivery_type' ,'delivery_type');
      $enrollQuery->addField('enroll', 'reg_status' ,'regstatus');
	  $enrollQuery->addField('enroll', 'comp_status' ,'compstatus');
      $enrollQuery->addField('enroll', 'valid_from' ,'validfrom');
      $enrollQuery->addField('enroll', 'is_compliance' ,'is_compliance');
      $enrollQuery->addField('enroll', 'progress' ,'progress');
      $enrollQuery->addField('enroll', 'master_enrollment_id' ,'masterenrollment');
	  $enrollQuery->condition('enroll.order_id', $orderId);
      $enrollQuery->condition('enroll.class_id', $classId);
      $enrollQuery->condition('enroll.course_id', $courseId);
      $enrollResult = $enrollQuery->execute()->fetchAll();      

      $dbRegistrationStatus = $enrollResult[0]->regstatus;
	  $dbCompletionStatus = $enrollResult[0]->compstatus;
      $dbValidFrom = $enrollResult[0]->validfrom;
      $dbProgress = $enrollResult[0]->progress;
      $delivery_type = $enrollResult[0]->delivery_type;
      expDebug::dPrint('$registrationStatusDate - ' . print_r($registrationStatusDate, true), 4);
      expDebug::dPrint('$vCompletionDate - ' . print_r($vCompletionDate, true), 4);
	  
      $updateQuery = db_update('slt_enrollment');
      
      // Update query field updation for registration status
      if($vRegistrationStatus != 'null'){
        $updateFields['reg_status']       = $vRegistrationStatus;
        if($vRegistrationStatusDate!= 'null' && !empty($vRegistrationStatusDate)){
          $updateFields['reg_status_date']  = $vRegistrationStatusDate;
        }
        else
        	$updateFields['reg_status_date']  = now();  // In UI, system takes the current date. 
        
        
        if($vRegistrationStatus != 'lrn_crs_reg_cnf' && ($vRegistrationStatus == 'lrn_crs_reg_wtl' || $vRegistrationStatus == 'lrn_crs_reg_can')){
          $updateFields['comp_status']  = $emptyValue;
          $updateFields['comp_date']  = $emptyValue;
        /*  if($vRegistrationStatus == 'lrn_crs_reg_wtl'){
          	$updateQuery->isNull('comp_status');
          }else{*/
          	$updateQuery->condition(db_or()->condition('comp_status', array('lrn_crs_cmp_enr','lrn_crs_cmp_inp'))
          		->isNull('comp_status'));
          	$updateQuery->condition('reg_status','lrn_crs_reg_can','!=');
         // }
        } 
        
        if($vRegistrationStatus == 'lrn_crs_reg_cnf'){
        	expDebug::dPrint("With in registration status".$vRegistrationStatus,4);
        	$regSts = array(
        		'lrn_crs_reg_can',
        		'lrn_crs_reg_ppm',
        		'lrn_crs_reg_rsv'
        	);
          $updateFields['comp_status']  = 'lrn_crs_cmp_enr';
          $updateFields['waitlist_flag']  = $emptyValue;
          $updateFields['waitlist_priority']  = $emptyValue;
          //$updateQuery->isNull('comp_status');
          $updateQuery->condition('reg_status',$regSts,'NOT IN');
        }
      }
      
      // Update query field updation for completion status
      if($vCompletionStatus != 'null'){
      	expDebug::dPrint("With in completion status".$vRegistrationStatus,4);
        $updateFields['comp_status']  = $vCompletionStatus;
        $updateQuery->condition('comp_status',array('lrn_crs_cmp_cmp','lrn_crs_cmp_inc','lrn_crs_cmp_nsw'),'NOT IN');
        $updateQuery->condition('reg_status','lrn_crs_reg_cnf');
	        if($vCompletionStatus != 'lrn_crs_cmp_enr' && $vCompletionStatus != 'lrn_crs_cmp_inp') {
		        if($vCompletionDate != 'null'){
		          $updateFields['comp_date'] = $vCompletionDate;
		        }
	        }
	        else {
	        	$updateFields['comp_date'] = NULL;
	        }
       }
    
      // Update query field updation for Valid from and Valid To
      if($vContentValidFrom != 'null' && $vContentValidTo != 'null'){
        $updateFields['valid_from']  = $vContentValidFrom;
        $updateFields['valid_to']  = $vContentValidTo;
      } else if(!empty($dbValidFrom) && $dbValidFrom != 'null' && $vContentValidTo != 'null'){
        $updateFields['valid_from']  = $vContentValidFrom;
      } else if(empty($dbValidFrom) && $vContentValidTo != 'null'){
        $updateFields['valid_from']  = $currentDateTime;
        $updateFields['valid_to']  = $vContentValidTo;
      }  
      
      // Update query field updation for Score, Grade and Updated by
      if($vScore == '-'){
        $updateFields['score']  = 0;
      } else if($vScore != 'null'){
        $updateFields['score']  = $vScore;
      }
      
      if($vGrade != 'null'){
        $updateFields['grade']  = $vGrade;
      }
      if(count($updateFields)>0){
      $updateFields['updated_by']  = $userId;
      $updateFields['updated_on']  = $currentDateTime;
      if ($vCompletionStatus != 'lrn_crs_cmp_cmp') {
       if(($waived == 1 && $vRegistrationStatus != 'lrn_crs_reg_can') || ($vCompletionStatus == 'lrn_crs_cmp_enr' || $vCompletionStatus == 'lrn_crs_cmp_inp')){
         $updateFields['comp_by']  = NULL;
	    }else{
         $updateFields['comp_by']  = $userId;
	   }
      }
      else {
      	$updateFields['comp_by']  = $userId;
      }  
      if($vCompletionStatus != 'lrn_crs_cmp_enr' && $vCompletionStatus != 'lrn_crs_cmp_inp') {  
	      $updateFields['comp_on']  = $currentDateTime;
      }
      else {
      	  $updateFields['comp_on']  = NULL;
      }
      }
      
      if($vCompletionStatus=='lrn_crs_cmp_cmp'){
      	/* if($delivery_type == 'lrn_cls_dty_ilt' || $delivery_type == 'lrn_cls_dty_vcl'){
      		$selectStmt = db_select('slt_course_content_mapper', 'content');
			  // Set conditions
			  $selectStmt->condition('content.course_id', $courseId);
			  $selectStmt->condition('content.class_id', $classId);
			  // Select fields and/or add expressions
			  $selectStmt->addField('content', 'id', 'id');
			  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
			  // Execute query
			  $result = $selectStmt->execute();
			  // expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
			  if ($result->rowCount() <= 0) {
			  		$updateFields['progress']  = 100;
			  }else{
			  	$updatePrgVal = ($dbProgress > 0 ) ? $dbProgress : 0;
			  	$updateFields['progress']  = $updatePrgVal;
			  }
      	}else{ */
      		$updateFields['progress']  = 100;
     // 	}
      }
      $updateQuery->fields($updateFields);
      $updateQuery->condition('id', $enrollId);
      
      
      expDebug::dPrintDBAPI(' $UpdateQuery = ' , $updateQuery,5);
      if(count($updateFields)>0){
      	$rosUpdated = $updateQuery->execute();
      }else{
      	$rosUpdated = -1;
      }
      
      if($rosUpdated == 0){
      	$notUpdatedId=$enrollId;
      }
      expDebug::dPrint(' $UpdateQuery  Status = '.print_r($rosUpdated,true),5);
      if($_REQUEST['apiname'] == 'UpdateRosterInternalAPI'){
      	// 61266: "Update internal roster api" is not updating status correctly
      	if($vRegistrationStatus != 'null' && $vRegistrationStatus != '') {
      		$updateFields1['reg_status'] = $vRegistrationStatus;
      		if($vRegistrationStatus == 'lrn_crs_reg_can') {
      			$updateFields1['comp_status'] = NULL;
      		} elseif ($vRegistrationStatus == 'lrn_crs_reg_cnf') {
      			$updateFields1['comp_status'] = 'lrn_crs_cmp_enr';
      		}
      	}
      	if($vRegistrationStatusDate != 'null'){

      		$updateFields1['reg_date'] = $vRegistrationStatusDate;
      		$updateFields1['reg_status_date'] = $vRegistrationStatusDate;
      	}

      	if($vCompletionStatus != 'lrn_crs_cmp_cmp'){
      		
      	
      		$updateFields1['comp_on'] = null;
      		$updateFields1['comp_by'] = null;
      		$updateFields1['pre_status']  = null;
      		$updateFields1['pre_score']  = null;
      		$updateFields1['content_status']  = null;
      		$updateFields1['comp_date']  = null;
      		$updateFields1['score'] = null;
      		
      		}
      	else{
      		if ($vCompletionDate != 'null'){
      			$updateFields1['comp_date']  = $vCompletionDate;
      		}
      		if ($vScore != 'null'){
      			$updateFields1['score'] = $vScore;
      		}
      	}
      	if($vCompletionStatus != 'null'){
      		$updateFields1['comp_status']  = $vCompletionStatus;
      	}
      	$updateQuery1 = db_update('slt_enrollment');
      	$updateQuery1->fields($updateFields1);
      	$updateQuery1->condition('id', $enrollId);
      	expDebug::dPrintDBAPI(' $UpdateQuery1 = ' , $updateQuery1,5);
      	$updateQuery1->execute();
      	
      	make_audit_trail_entries($updateFields1, getIdOfLoggedInUser() , $default ,$enrollId, 'Inserted','cre_sys_obt_cls','exp_sp_learning','updateEnrollment',
      			array('reg_status','reg_status_date', 'comp_status', 'waitlist_priority', 'comp_date','score'));
      	}
      if($rosUpdated > 0){ // Skip further porcess if there is no update
	      if ($_REQUEST['apiname'] != 'UpdateRosterInternalAPI') { // update audit trail only if changes made in enrollment table
	      make_audit_trail_entries($updateFields, getIdOfLoggedInUser() , $default ,$enrollId, 'Inserted','cre_sys_obt_cls','exp_sp_learning','updateEnrollment',
                                              array('reg_status','reg_status_date', 'comp_status', 'waitlist_priority', 'comp_date','score'));

      // To get the delivery type
      $classQuery = db_select('slt_course_class', 'class');
      $classQuery->addField('class', 'delivery_type' ,'delivery_type');
      $classQuery->condition('class.id', $classId);
      $classResult = $classQuery->execute()->fetchAll();
      $dbDeliveryType = $classResult[0]->delivery_type;
      
      if($vRegistrationStatus == 'lrn_crs_reg_cnf' && ($dbCompletionStatus != $vCompletionStatus) && ($vCompletionStatus != 'lrn_crs_cmp_cmp' && $vCompletionStatus != 'lrn_crs_cmp_inc' && $vCompletionStatus != 'lrn_crs_cmp_nsw')){ 
        // Notification Insert has to be implemented - CALL slp_notification_ins('Register',xCourseId,xClassId,xUserId,xupdateuserid);
        $iCal = null;
      	if ($dbDeliveryType == 'lrn_cls_dty_ilt' || $dbDeliveryType == 'lrn_cls_dty_vcl') {
          $iCal = getPublishICal($enrollId, 'Class');
      	}
		if(!isset($_SESSION['waived_enroll_id_'.$enrollId])){
			notificationInsert('Register', $courseId, $classId, $enrollUserId, $userId, $iCal);
		}else{
			if($_SESSION['waived_enroll_id_'.$enrollId] == $enrollId){
				unset($_SESSION['waived_enroll_id_'.$enrollId]);	
			}
		}        
        // Code start Tp move from incomplet to enroll tab
        markEnrolledStatus($enrollId, $classId, $courseId, $enrollUserId,$updateFields);     
      }
      if($vRegistrationStatus == 'lrn_crs_reg_can' || $vRegistrationStatus =='lrn_crs_reg_rjt'){
        
        if($dbDeliveryType == 'lrn_cls_dty_ilt' || $dbDeliveryType == 'lrn_cls_dty_vcl'){
          // CALL slp_notification_ins('Cancel',xenrollid,xClassId,xUserId,xupdateuserid);
        	$iCal = getCancelICal($enrollId, 'Class');
          notificationInsert('Cancel', $enrollId, $classId, $enrollUserId, $userId, $iCal);
        } else if($dbDeliveryType == 'lrn_cls_dty_wbt' || $dbDeliveryType == 'lrn_cls_dty_vod'){
          // CALL slp_notification_ins('WBTCancel',xenrollid,xClassId,xUserId,xupdateuserid);
          notificationInsert('WBTCancel', $enrollId, $classId, $enrollUserId, $userId);
        }
        
        if($vRegistrationStatus == 'lrn_crs_reg_can'){
          // CALL slp_notification_ins('CancelLocationAdmin',xenrollid,xClassId,xUserId,xupdateuserid);
          notificationInsert('CancelLocationAdmin', $enrollId, $classId, $enrollUserId, $userId);
          expDebug::dPrint('delete user points when cancel' . $enrollUserId . 'enroll id' . $enrollId, 4);
          insertUserPointsForEachActionsPerformed($enrollUserId,'register_class',$enrollId,'delete','class'); // deleting user points while cancelling
        }
        if($dbRegistrationStatus == 'lrn_crs_reg_cnf' || $dbRegistrationStatus == 'lrn_crs_reg_wtl'){
          /* Waitlist Promotion Call */  
          waitlistPromotionUpdate($classId,$dbRegistrationStatus);
        }
        
      }
      if($vCompletionStatus == 'lrn_crs_cmp_cmp' || $vCompletionStatus == 'lrn_crs_cmp_inc' || $vCompletionStatus == 'lrn_crs_cmp_nsw' ){
        if($vCompletionStatus == 'lrn_crs_cmp_cmp'){
          // CALL slp_notification_ins('Completed',xenrollid,xClassId,xUserId,xupdateuserid);
          insertUserPointsForEachActionsPerformed($enrollUserId,'complete_class_training',$enrollId,'','class');
          notificationInsert('Completed', $enrollId, $classId, $enrollUserId, $userId);
        }else if($vCompletionStatus == 'lrn_crs_cmp_inc' && $enrollResult[0]->is_compliance == 1){ // Complaince incomplete status #0047747
        	notificationInsert('ComplianceClassIncomplete', $courseId, $classId, $enrollUserId, $userId);
        }
        
         // CALL slp_mark_course_complete_class_level(xenrollid,xupdateuserid,xcompDate,xScore,xnotifyBy);
        $notifyBy = '';
        $type = 'class';
        $objParam = new stdClass();
        $objParam->compstatus = $vCompletionStatus;
        markCompletionCrsCls($enrollId, $classId, $courseId, $enrollUserId, $vCompletionDate, $vScore, $vGrade, $notifyBy, $type,$objParam);
      }
      
      
      if($vRegistrationStatus == 'lrn_crs_reg_wtl'){
    	// CALL slp_notification_ins('Waitlist',xCourseId,xClassId,xUserId,xupdateuserid);
    	// CALL slp_notification_ins('WaitlistLocationAdmin',xCourseId,xClassId,xUserId,xupdateuserid);
        notificationInsert('Waitlist', $courseId, $classId, $enrollUserId, $userId);
        notificationInsert('WaitlistLocationAdmin', $courseId, $classId, $enrollUserId, $userId);
      }
      }
      }
      
      return $notUpdatedId;
  } catch (Exception $ex) {
    watchdog_exception('catalogRoster', $ex);
    expertusErrorThrow($ex);
  } 
} 

// Course / Class mark completion
function markCompletionCrsCls($enrollId, $classId, $courseId, $enrollUserId, $vCompletionDate, $vScore, $vGrade, $notifyBy, $type = 'class',$objParam,$isFromAdmin=0){
  try {
    $userId = getSltpersonUserId();
    $currentDateTime = now();
    $nullValue = NULL;
    $integerNullValue = 0;
    $vCompletionDate = $vCompletionDate == 'null' ? $currentDateTime : $vCompletionDate;
    $compStatus = $objParam->compstatus;
    if($type == 'program'){
  
      $enrollListQuery = db_select('slt_enrollment', 'enroll');
      $enrollListQuery->addField('enroll', 'course_id' ,'course_id');
      $enrollListQuery->addField('enroll', 'master_enrollment_id' ,'master_enrollment_id');
      $enrollListQuery->addField('enroll', 'class_id' ,'class_id');
      $enrollListQuery->condition('enroll.id', $enrollId);
      expDebug::dPrintDBAPI(' $enrollListQuery = ' , $enrollListQuery);
      $enrollList = $enrollListQuery->execute()->fetchAll();
      
      $classId  = $enrollList[0]->class_id;
      $courseId = $enrollList[0]->course_id;
      
      $enrollQuery = db_select('slt_enrollment', 'enroll');
      $enrollQuery->addField('enroll', 'id' ,'id');
      $enrollQuery->addField('enroll', 'course_id' ,'course_id');
      $enrollQuery->addField('enroll', 'class_id' ,'class_id');
      $enrollQuery->addField('enroll', 'user_id' ,'user_id');
      $enrollQuery->addField('enroll', 'master_enrollment_id' ,'master_enrollment_id');
      $enrollQuery->condition('enroll.class_id', $classId);
      $enrollQuery->condition('enroll.course_id', $courseId);
      $enrollQuery->condition('enroll.user_id', $enrollUserId);
      $enrollQuery->condition('enroll.comp_status' , 'lrn_crs_cmp_inc', '!=');//'lrn_crs_cmp_nsw'
      $enrollQuery->condition('enroll.reg_status' , array('lrn_crs_reg_ppm','lrn_crs_reg_cnf'), 'IN');
      expDebug::dPrintDBAPI('$enrollQuery = ' , $enrollQuery);
      $enrollResult = $enrollQuery->execute()->fetchAll();
    } else if($type == 'class'){
      $enrollQuery = db_select('slt_enrollment', 'enroll');
      $enrollQuery->addField('enroll', 'id' ,'id');
      $enrollQuery->addField('enroll', 'course_id' ,'course_id');
      $enrollQuery->addField('enroll', 'class_id' ,'class_id');
      $enrollQuery->addField('enroll', 'user_id' ,'user_id');
      $enrollQuery->addField('enroll', 'comp_status' ,'comp_status');
      $enrollQuery->addField('enroll', 'master_enrollment_id' ,'master_enrollment_id');
      $enrollQuery->condition('enroll.class_id', $classId);
      $enrollQuery->condition('enroll.course_id', $courseId);
      $enrollQuery->condition('enroll.user_id', $enrollUserId);
      $enrollQuery->condition('enroll.reg_status' , array('lrn_crs_reg_ppm','lrn_crs_reg_cnf'), 'IN');
      expDebug::dPrintDBAPI(' $enrollQuery= ' , $enrollQuery);
      //$enrollQuery->condition('enroll.id', $enrollId, '!=');
      $enrollResult = $enrollQuery->execute()->fetchAll();
      
    }
    
    foreach($enrollResult as $enroll){
      $dbEnrollId       = $enroll->id;
      $dbMasterEnrollId = $enroll->master_enrollment_id;
      $dbUserId         = $enroll->user_id;
      $dbCourseId       = $enroll->course_id;
      $dbClassId        = $enroll->class_id;
      $dbClasscomp        = $enroll->comp_status;
      
      // Check the completion status
      $compStatusCount = 0;
      $completion_status_check = array('lrn_crs_cmp_cmp');
      if($compStatus == 'lrn_crs_cmp_cmp'){
        $completion_status_check = array('lrn_crs_cmp_cmp');
      }
      else if($compStatus == 'lrn_crs_cmp_inc' || $compStatus=='lrn_crs_cmp_nsw'){
        $completion_status_check = array('lrn_crs_cmp_cmp','lrn_crs_cmp_inc','lrn_crs_cmp_nsw');
      }
      if($dbClasscomp == 'lrn_crs_cmp_inc' || $dbClasscomp=='lrn_crs_cmp_nsw'){
      	$completion_status_check = array('lrn_crs_cmp_cmp','lrn_crs_cmp_inc','lrn_crs_cmp_nsw');
      }
      $compStatusQuery = db_select('slt_enrollment', 'enroll');
      $compStatusQuery->addField('enroll', 'id');
      $compStatusQuery->addExpression('IFNULL(comp_status,0 )', 'completion_status_check' );
      $compStatusQuery->condition('enroll.id', $dbEnrollId);
      $compStatusQuery->condition('enroll.reg_status' , array('lrn_crs_reg_ppm','lrn_crs_reg_cnf'), 'IN');
      $compStatusQuery->groupBy('enroll.id');
      $compStatusQuery->havingCondition('completion_status_check', $completion_status_check, 'NOT IN');
      expDebug::dPrintDBAPI(' $compStatusQuery = ' , $compStatusQuery);
      $compStatusResult = $compStatusQuery->execute()->fetchAll();
      $compStatusCount = count($compStatusResult[0]->id);
      if($compStatusCount > 0){
      	// Update enrollment
      	$updateEnrollment  = db_update('slt_enrollment');
      	$updateEnrollmentFields['comp_status'] = $compStatus;
      	$updateEnrollmentFields['comp_date'] = $vCompletionDate;
      	// #55538 - Update slt_enrollment class registered in My enrollment When admin mark complete in program. 
      	$updateEnrollmentFields['comp_by']  = $userId;
      	$updateEnrollmentFields['comp_on'] = $vCompletionDate;
      	
      	if(!empty($vScore) && $vScore != 'null') $updateEnrollmentFields['score'] = $vScore;
      	if(!empty($vGrade) && $vGrade != 'null') $updateEnrollmentFields['grade'] = $vGrade;
      	$updateEnrollmentFields['updated_by']  = $userId;
      	$updateEnrollmentFields['updated_on']  = $currentDateTime;
      	if($compStatus == 'lrn_crs_cmp_cmp'){
      	$updateEnrollmentFields['progress']  = 100;
      	}
      	$updateEnrollment->fields($updateEnrollmentFields);
      	$updateEnrollment->condition('id', $compStatusResult[0]->id);
      	$updateEnrollment->condition('reg_status',  array('lrn_crs_reg_ppm','lrn_crs_reg_cnf'), 'IN');
      	$updateEnrollment->condition('comp_status', $completion_status_check, 'NOT IN');
      	$updateEnrollment->execute();
      	expDebug::dPrintDBAPI(' $$updateEnrollment here  ' , $updateEnrollment);
      }
      
      
      	if(!empty($dbMasterEnrollId) && $dbMasterEnrollId != NULL){
        // Get program id from master enrollment table
        $masterEnrollQuery = db_select('slt_master_enrollment', 'master');
        $masterEnrollQuery->addField('master', 'program_id');
        $masterEnrollQuery->addField('master', 'overall_status');
        $masterEnrollQuery->condition('master.id' , $dbMasterEnrollId);
        $masterEnrollResult = $masterEnrollQuery->execute()->fetchAll();
        $programId = $masterEnrollResult[0]->program_id;
        $overallStatusId = $masterEnrollResult[0]->overall_status;
        if($overallStatusId != 'lrn_tpm_ovr_cmp' && $compStatus == 'lrn_crs_cmp_cmp'){
        	// CALL slp_mark_program_complete(xuserid,xmasterenrid,xprogramid,xupduser_id,xnotifyby);
        	rosterProgramComplete($enrollUserId, $dbMasterEnrollId, $programId, $userId, $notifyBy, $vCompletionDate,$isFromAdmin);
        }
        else if($overallStatusId != 'lrn_tpm_ovr_inc' && ($compStatus == 'lrn_crs_cmp_inc' || $compStatus == 'lrn_crs_cmp_nsw')){
        	//rosterProgramInComplete($enrollUserId, $dbMasterEnrollId, $programId, $userId, $notifyBy, $vCompletionDate,$dbCourseId);
        	rosterProgramComplete($enrollUserId, $dbMasterEnrollId, $programId, $userId, $notifyBy, $vCompletionDate,$isFromAdmin);
        }
      	} else {
        if($compStatusCount > 0){
        	if($compStatus == 'lrn_crs_cmp_cmp'){
        		// CALL slp_notification_ins('Completed',xenrid,xclsid,xuserid,xupduser_id);
        		notificationInsert('Completed', $enrollId, $classId, $enrollUserId, $userId);
        		$compStatusCount = 0;
        	}
        }
      }
      
    }
	syncSolrData('Enrollment');
    syncSolrData('MasterEnrollment');
    syncSolrData('User');
  } catch (Exception $ex) {
    watchdog_exception('catalogRoster', $ex);
    expertusErrorThrow($ex);
  } 
  
}

// To complete the program after the course/class completion
function rosterProgramComplete($enrollUserId, $masterEnrollId, $programId, $userId, $notifyBy, $vCompletionDate = '',$isFromAdmin=0){
  try {
      $userId = ($isFromAdmin==0) ? $userId : getSltpersonUserId();
      $currentDateTime = now();
      //Check whether it is in normal registration/recertification path.
      $select = db_select('slt_master_enrollment','sme');
      $select->addField('sme','recertify_path','recflag');
      $select->condition('sme.id',$masterEnrollId,'=');
      $select->condition('sme.is_current','Y','=');
      expDebug::dPrintDBAPI(' recertify flag ' , $select);
      $recertFlag = $select->execute()->fetchField();

//       if($recertFlag > 1) {
//         //Modified by Shobana and Priya on DEC 12, 2013 - #25277
//       	$mandaroty = db_select('slt_re_certify','pmap');
//       	$mandaroty->addExpression('COUNT(1)','rec_count');
//       	$mandaroty->condition('pmap.program_id',$programId,'=');
//       	$mandaroty->condition('pmap.is_required','Y','=');
//       	expDebug::dPrintDBAPI(' slt_re_certify' , $mandaroty);
//       	$mandarotyCount = $mandaroty->execute()->fetchField();
     
//       }else{
      	/*
      	 // Check program completion status before update
      	$beforeUpdate = programCompletionStatusCheck($masterEnrollId, $userId, $programId);
      	*/
      	//Check if mandatory training is there or not 
      	//Modified by Vincent on Jun 06, 2013 - for #0024393
      	$mandaroty = db_select('slt_module_crs_mapping','pmap');
      	$mandaroty->addExpression('COUNT(1)','rec_count');
      	$mandaroty->condition('pmap.program_id',$programId,'=');
      	$mandaroty->condition('pmap.is_required','Y','=');
      	$mandarotyCount = $mandaroty->execute()->fetchField();
      	
      	//Adding for #0089761
      	$module_id = getModuleIdFromObjectId($programId,$recertFlag);
      	expDebug::dPrint("getting the moduleID = ".print_r($module_id, true), 3);
      //}
      //Check for any assessment is associated to TP
      //Done by Vincent on Sep 07, 2012
      if($isFromAdmin==0){
	      $assess = db_select('slt_survey_mapping','assmap');
	      $assess->join('slt_survey','srv','srv.id = assmap.survey_id AND srv.type = \'sry_det_typ_ass\'');
	      $assess->addExpression('COUNT(1)','rec_count');
	      $assess->condition('assmap.object_type','cre_sys_obt_cls','!=');
	      $assess->condition('assmap.object_id',$programId,'=');
	      $assessCount = $assess->execute()->fetchField();
      }else{
      	$assessCount == 0; //Bypass the TP check if the completion happend by TP Roster
      }
      //Check for any incomplete class available in TP enrollment
      //Done by Vincent on Sep 07, 2012
      $incCls = db_select('slt_enrollment','enroll');
      $incCls->join('slt_master_enrollment','menroll','menroll.id = enroll.master_enrollment_id');
//       if($recertFlag > 0)
//       	$incCls->join('slt_re_certify','map','map.program_id= menroll.program_id AND enroll.course_id = map.course_id');
//       else
      $incCls->join('slt_module_crs_mapping','map','map.program_id = menroll.program_id AND enroll.course_id = map.course_id');
      $incCls->addExpression('count(DISTINCT enroll.id)','rec_count');
      $incCls->condition('enroll.comp_status',array('lrn_crs_cmp_inc','lrn_crs_cmp_nsw'),'IN');
      $incCls->condition('map.is_required','Y','=');
      $incCls->condition('menroll.id',$masterEnrollId,'=');
      $incClsCount = $incCls->execute()->fetchField();
      
      // If the TP has assessment and there is no incompleted class then should not do the rollup
      // Added by Vincent on Sep 07, 2012
      expDebug::dPrint("ASSESSMENT COUNT = ".$assessCount."  INCOMPLETE COUNT = ".$incClsCount);
      if($assessCount == 0 || $incClsCount > 0){  
	      if($mandarotyCount>0){
		      // Enrollment values needs to be checked before updating the program  
		      $selectEnrollQuery = db_select('slt_enrollment', 'enroll');
// 		      if($recertFlag > 1) 
// 		      	$selectEnrollQuery->innerjoin('slt_re_certify', 'mapping', 'mapping.course_id = enroll.course_id');
// 		      else 
		      	$selectEnrollQuery->innerjoin('slt_module_crs_mapping', 'mapping', 'mapping.course_id = enroll.course_id');
		      $selectEnrollQuery->condition('mapping.program_id', $programId);
		      $selectEnrollQuery->addField('enroll', 'master_enrollment_id');
		      $selectEnrollQuery->addField('enroll', 'user_id');
		      $selectEnrollQuery->addExpression('MAX(CASE WHEN enroll.comp_status=\'lrn_crs_cmp_cmp\' THEN 0 ELSE 1 END)', 'completion_status');
		      $selectEnrollQuery->addExpression('MAX(CASE WHEN enroll.comp_status in (\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_att\',\'lrn_crs_cmp_nsw\') THEN 1 ELSE 0 END)', 'inporgress_status');
		      $selectEnrollQuery->addExpression('MAX(CASE WHEN enroll.comp_status in (\'lrn_crs_cmp_inc\',\'lrn_crs_cmp_nsw\') THEN 1 ELSE 0 END)', 'incomplete_status');
		      $selectEnrollQuery->condition('enroll.master_enrollment_id', $masterEnrollId);
		      $selectEnrollQuery->condition('mapping.is_required', 'Y');
		      $selectEnrollQuery->condition('mapping.module_id',$module_id[0]->module_id,'=');
		      $selectEnrollQuery->groupby('enroll.master_enrollment_id');
		      $selectEnrollQuery->groupby('enroll.user_id');
		      expDebug::dPrintDBAPI(' $selectEnrollQuery = ' , $selectEnrollQuery);
		      $selectEnrollResult = $selectEnrollQuery->execute()->fetchAll();
	      }else{
	      	 // Enrollment values needs to be checked before updating the program  
		      $selectEnrollQuery = db_select('slt_enrollment', 'enroll');
		      $selectEnrollQuery->addField('enroll', 'master_enrollment_id');
		      $selectEnrollQuery->addField('enroll', 'user_id');
		      $selectEnrollQuery->addExpression('MIN(CASE WHEN enroll.comp_status=\'lrn_crs_cmp_cmp\' THEN 0 ELSE 1 END)', 'completion_status');
		      $selectEnrollQuery->addExpression('MAX(CASE WHEN enroll.comp_status in (\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_att\') THEN 1 ELSE 0 END)', 'inporgress_status');
		      $selectEnrollQuery->addExpression('0','incomplete_status');
// 		      if($recertFlag > 1)
// 		      	$selectEnrollQuery->innerjoin('slt_re_certify', 'mapping', 'mapping.course_id = enroll.course_id');
// 		      else 
		      	$selectEnrollQuery->innerjoin('slt_module_crs_mapping', 'mapping', 'mapping.course_id = enroll.course_id');
		      $selectEnrollQuery->condition('mapping.program_id', $programId);
		     // $selectEnrollQuery->condition('mapping.program_id', $programId);
		      $selectEnrollQuery->condition('enroll.master_enrollment_id', $masterEnrollId);
		      $selectEnrollQuery->condition('mapping.module_id',$module_id[0]->module_id,'=');
		      $selectEnrollQuery->groupby('enroll.master_enrollment_id');
		      $selectEnrollQuery->groupby('enroll.user_id');
		      expDebug::dPrintDBAPI(' $selectEnrollQuery = ' , $selectEnrollQuery);
		      $selectEnrollResult = $selectEnrollQuery->execute()->fetchAll();
	      }
	      $completionStatus = $selectEnrollResult[0]->completion_status;
	      $InprogressStatus = $selectEnrollResult[0]->inporgress_status;
	      $IncompleteStatus = $selectEnrollResult[0]->incomplete_status;
	      expDebug::dPrint(' $completionStatus '.$completionStatus .' $InprogressStatus '.$InprogressStatus .' $IncompleteStatus '.$IncompleteStatus ,5);
	      if(($completionStatus !='' && ($completionStatus == 0 || $completionStatus == 1)) && ($InprogressStatus != '' && ($InprogressStatus==0 || $IncompleteStatus==1))){
	        
	        $enrollCompletionDate = getMasterEnrollCompletionDate($masterEnrollId);
	        $overallstatus='';
	        if($completionStatus == 0 && $IncompleteStatus==0){
	        	$overallstatus = 'lrn_tpm_ovr_cmp';
	        }else{
	        	$overallstatus = 'lrn_tpm_ovr_inc';
	        }
	        
	        // Update master enrollment
	        $updateMasterEnroll = db_update('slt_master_enrollment');
	        $updateMasterFields = array(
	        		
	                          'overall_status' => $overallstatus,
	                          'comp_date' => empty($enrollCompletionDate) ? $currentDateTime : $enrollCompletionDate,
	                          'updated_by' => $userId,
	        									'updated_on' => $currentDateTime,
	        									'custom4'	=> $overallstatus
	        );  
	      $updateMasterEnroll->fields($updateMasterFields);
	      $updateMasterEnroll->condition('id', $masterEnrollId);
	      $updateMasterEnroll->condition('user_id', $enrollUserId);
	      $updateMasterEnroll->condition('program_id', $programId);
	      $updateMasterEnroll->condition('overall_status', array('lrn_tpm_ovr_cln','lrn_tpm_ovr_cmp','lrn_tpm_ovr_exp','lrn_tpm_ovr_ppm','lrn_tpm_ovr_ppv'), 'NOT IN');
	      $updateMasterEnroll->condition('is_current', 'Y','=');
	      $updateMasterEnroll->execute();   
	      expDebug::dPrintDBAPI('updateMasterEnroll query :',$updateMasterEnroll);
	
	      //change for the ticket 0020909 = For updating tp in skills//
	      $objecttype = db_select('slt_program', 'prg');
		  	$objecttype->addField('prg','object_type','objecttype');
		  	$objecttype->addField('prg','title','title');
				$objecttype->condition('id', $programId);
				$objResult = $objecttype->execute()->fetchAll();
				
				expDebug::dPrintDBAPI('objecttype query :', $objecttype);
	  
		    if($objResult[0]->objecttype == 'cre_sys_obt_crt') {  
	      updateSkillForTpCompletion($programId,$enrollUserId,$masterEnrollId,$objResult[0]->title,$userId);
		    }  
	     
	      $overallcomplete = db_select('slt_master_enrollment', 'enroll');
	      $overallcomplete->addField('enroll', 'overall_status');
	      $overallcomplete->addField('enroll', 'recertify_path');
	      $overallcomplete->condition('id', $masterEnrollId);
	      $overallcompleted = $overallcomplete->execute()->fetchAll();
	      $autitTxt =  '';
	      if($overallcompleted[0]->overall_status == 'lrn_tpm_ovr_cmp'){
	      	if($overallcompleted[0]->recertify_path == 1)
	        	insertUserPointsForEachActionsPerformed($enrollUserId,'complete_class_training',$masterEnrollId,'','tp');  
	      	    
	      	$autitTxt =  "rosterProgramComplete";
	      	// CALL slp_notification_ins('CertCurrLPComplete',xmaster_enrollment_id,xprogram_id,xuser_id,xupduser_id);
	        notificationInsert('CertCurrLPComplete', $masterEnrollId, $programId, $enrollUserId, $userId);
	      }else if($overallcompleted[0]->overall_status == 'lrn_tpm_ovr_inc'){
	      	$autitTxt =  "rosterProgramInComplete";
	      }
	      if($autitTxt!='')
	       	make_audit_trail_entries($updateMasterFields, getIdOfLoggedInUser() , null ,$programId, 'Updated',$objResult[0]->objecttype,'exp_sp_learning',$autitTxt,
	                                              array('overall_status','comp_date', 'updated_by'));  
	      }
	    }
	    // To update  a overall percentage complete used in content player - new column
	    updateTPEnrollmentOverallPercentComplete($programId, $masterEnrollId, $recertFlag);
	    // To update percentage of complete - old column
	    updateTPEnrollmentPercentComplete($programId, $masterEnrollId, $recertFlag);

      /*
      // Check program completion status after update
      $afterUpdate = programCompletionStatusCheck($masterEnrollId, $userId, $programId);
      
      // To send notification 
      if($beforeUpdate == 0 && $afterUpdate == 1){
        // CALL slp_notification_ins('CertCurrLPComplete',xmaster_enrollment_id,xprogram_id,xuser_id,xupduser_id);
        notificationInsert();
      }
      */
  } catch (Exception $ex) {
    watchdog_exception('catalogRoster', $ex);
    expertusErrorThrow($ex);
  }       
  
}

// To incomplete the program after the course/class incompletion
function rosterProgramInComplete($enrollUserId, $masterEnrollId, $programId, $userId, $notifyBy, $vCompletionDate = '',$dbCourseId){
  try {
      $userId = getSltpersonUserId();
      $currentDateTime = now();
         
      // Enrollment values needs to be checked before updating the program  
      $selectEnrollQuery = db_select('slt_module_crs_mapping', 'mapping');
      $selectEnrollQuery->addExpression('count(1)', 'mappingcount');
      $selectEnrollQuery->condition('mapping.program_id', $programId);
      $selectEnrollQuery->condition('mapping.course_id', $dbCourseId);
      $selectEnrollQuery->condition('mapping.is_required', 'Y');
      expDebug::dPrintDBAPI(' $selectEnrollQuery = ' , $selectEnrollQuery);
      $isRequired = $selectEnrollQuery->execute()->fetchField();    
      
      if($isRequired){        
        // Update master enrollment
        $updateMasterEnroll = db_update('slt_master_enrollment');
        $updateMasterFields = array(
                          'overall_status' => 'lrn_tpm_ovr_inc',
                          'comp_date' => $currentDateTime,
                          'updated_by' => $userId,
                          'updated_on' => $currentDateTime,
        );
        $updateMasterEnroll->fields($updateMasterFields);
        $updateMasterEnroll->condition('id', $masterEnrollId);
        $updateMasterEnroll->condition('user_id', $enrollUserId);
        $updateMasterEnroll->condition('program_id', $programId);
        $updateMasterEnroll->condition('overall_status', array('lrn_tpm_ovr_cln','lrn_tpm_ovr_cmp','lrn_tpm_ovr_exp','lrn_tpm_ovr_ppm','lrn_tpm_ovr_ppv','lrn_tpm_ovr_inc'), 'NOT IN');
        $updateMasterEnroll->execute();
                
         make_audit_trail_entries($updateMasterFields, getIdOfLoggedInUser() , null ,$programId, 'Updated','cre_sys_obt_cls','exp_sp_learning','rosterProgramInComplete',
                                                array('overall_status','comp_date', 'updated_by'));  
      }
  } catch (Exception $ex) {
    watchdog_exception('catalogRoster', $ex);
    expertusErrorThrow($ex);
  }       
  
}

// To check the program completion status
function programCompletionStatusCheck($dbMasterEnrollId, $userId, $programId){
  try {
    $masterSelectQuery = db_select('slt_master_enrollment' , 'master');
    $masterSelectQuery->addField('master', 'id');
    $masterSelectQuery->condition('master.id', $dbMasterEnrollId);
    $masterSelectQuery->condition('master.user_id', $userId);
    $masterSelectQuery->condition('master.program_id', $programId);
    $masterSelectQuery->condition('master.overall_status', 'lrn_tpm_ovr_cmp');
    $masterSelectResult = $masterSelectQuery->execute()->fetchAll();
    return count($masterSelectResult);
  } catch (Exception $ex) {
    watchdog_exception('catalogRoster', $ex);
    expertusErrorThrow($ex);
  }     
}

// To get the waitlist availablility
function getCatalogWaitlistAvailable($classId, $courseId){
  try {
      // To get the max capacity for class
      $sessionQuery = db_select('slt_course_class_session', 'session');
      $sessionQuery->addField('session','capacity_max');
      $sessionQuery->condition('session.class_id', $classId);
      $sessionQuery->condition('session.course_id', $courseId);
      $sessionResult = $sessionQuery->execute()->fetchAll();
      $maxCapacity = $sessionResult[0]->capacity_max;
      
      // To get the enrolled count for class
      $enrolledCntQuery = db_select('slt_enrollment', 'enroll');
      $enrolledCntQuery->addField('enroll', 'id');
      $enrolledCntQuery->condition('enroll.class_id', $classId);
      $enrolledCntQuery->condition('enroll.course_id', $courseId);
      $enrolledCntQuery->condition('enroll.reg_status', array('lrn_crs_reg_cnf', 'lrn_crs_reg_ppm'), 'IN');
      $enrolledCntResult = $enrolledCntQuery->execute()->fetchAll();
      $enrolledCapacity = count($enrolledCntResult);
    
      if($maxCapacity != 0 && $enrolledCapacity !=0 && $enrolledCapacity == $maxCapacity){
        return 1;
      } else {
        return 0;
      }
  } catch (Exception $ex) {
    watchdog_exception('catalogRoster', $ex);
    expertusErrorThrow($ex);
  }       
}


// To get the roster status (slp_roster_upd.sql)
function getCatalogRosterStatus($courseId, $classId, $enrolledId, $registrationStatus, $registrationStatusDate, $completionStatus, $completionDate, $grade, $score, $contentValidFrom, $contentValidTo,$callFromAPI){
  try {
      expDebug::dPrint('listEnrolledUsers query starts ');
      
      $enrollQuery = db_select('slt_enrollment', 'enroll');
      
      $enrollQuery->addField('enroll', 'user_id' ,'userid');
      $enrollQuery->addField('enroll', 'reg_status' ,'regstatus');
      $enrollQuery->addField('enroll', 'comp_status' ,'compstatus');
      $enrollQuery->addField('enroll', 'order_id' ,'orderid');
      $enrollQuery->addField('person', 'user_name' ,'username');
      $enrollQuery->leftjoin('slt_person', 'person', 'person.id = enroll.user_id');
      
      $enrollQuery->condition('enroll.id', $enrolledId);
      $enrollQuery->condition('enroll.class_id', $classId);
      $enrollQuery->condition('enroll.course_id', $courseId);
      
      expDebug::dPrintDBAPI('listEnrolledUsers query : ', $enrollQuery);
      $enrollResult = $enrollQuery->execute()->fetchAll();
      expDebug::dPrint('$$enrollResult = '.print_r($enrollResult, true), 3);
      $dbRegistrationStatus = $enrollResult[0]->regstatus;
      $dbCompletionStatus = $enrollResult[0]->compstatus;
      $dbUserId = $enrollResult[0]->userid;
      $dbUserName = $enrollResult[0]->username;
      $dbOrderId = $enrollResult[0]->orderid;
      // Registration status check
      $rosterStatus = array();
      if( (empty($registrationStatus) || $registrationStatus == 'null') && (empty($registrationStatusDate) || $registrationStatusDate == 'null') && $callFromAPI != 1){
        $rosterStatus['reg_status'] = 'null';
      } else if(empty($registrationStatus) || $registrationStatus == 'null' && $callFromAPI != 1){
        $rosterStatus['reg_status'] = t('LBL102').' '. t('MSG438');
      } else if(empty($registrationStatusDate) || $registrationStatusDate == 'null' && $callFromAPI != 1){
        $rosterStatus['reg_status'] = t('LBL042').' '. t('MSG438');
      } else if($dbCompletionStatus == 'lrn_crs_cmp_cmp' && $_REQUEST['apiname'] != 'UpdateRosterInternalAPI'){
        $rosterStatus['reg_status'] = t('MSG431').' '. t('Completed');  // "Already Completed"
      } else if($dbCompletionStatus == 'lrn_crs_cmp_nsw' ){
        $rosterStatus['reg_status'] = t('MSG431').' '. t('Completed');  // Here it should come "Already Completed with No Show" but displayed only "Already Completed"
      }else if($dbRegistrationStatus =='lrn_crs_reg_can' && $callFromAPI == 1){
      	$rosterStatus['reg_status'] = 'This Enrollment is already Cancelled.';
      }  else if( ($registrationStatus == 'lrn_crs_reg_cnf') && ($dbRegistrationStatus =='lrn_crs_reg_wtl' || $dbRegistrationStatus =='lrn_crs_reg_rsc' || $dbRegistrationStatus =='lrn_crs_reg_can') ){
        // To get the max capacity for class
        $sessionQuery = db_select('slt_course_class_session', 'session');
        $sessionQuery->addField('session','capacity_max');
        $sessionQuery->condition('session.class_id', $classId);
        $sessionQuery->condition('session.course_id', $courseId);
        $sessionResult = $sessionQuery->execute()->fetchAll();
        $maxCapacity = $sessionResult[0]->capacity_max;
        
        // To get the enrolled count for class
        $enrolledCntQuery = db_select('slt_enrollment', 'enroll');
        $enrolledCntQuery->addField('enroll', 'id');
        $enrolledCntQuery->condition('enroll.class_id', $classId);
        $enrolledCntQuery->condition('enroll.course_id', $courseId);
        //$enrolledCntQuery->condition('enroll.reg_status', array('lrn_crs_reg_rsv', 'lrn_crs_reg_cnf', 'lrn_crs_reg_ppm', 'lrn_crs_reg_ppv'), 'IN');
        $enrolledCntQuery->condition(db_or()
                                  ->condition('enroll.reg_status', 'lrn_crs_reg_rsv')
                                  ->condition('enroll.reg_status', 'lrn_crs_reg_cnf')
                                  ->condition('enroll.reg_status', 'lrn_crs_reg_ppm')
                                  ->condition('enroll.reg_status', 'lrn_crs_reg_ppv'));
        //print_r($enrolledCntQuery->__toString());                          
        $enrolledCntResult = $enrolledCntQuery->execute()->fetchAll();
        $enrolledCapacity = count($enrolledCntResult);
        
        if($enrolledCapacity >= $maxCapacity && !empty($maxCapacity)){
          $rosterStatus['reg_status'] = t('MSG439').' '. t('Class').' '. t('LBL267'); //t('Exceeding class max capacity');
        } else {
          $rosterStatus['reg_status'] = 'Ok';
        }
      } else if($registrationStatus == 'lrn_crs_reg_wtl' && $dbRegistrationStatus != 'lrn_crs_reg_wtl'){
        // To get the waitlisted capacity available for class
        $sessionQuery = db_select('slt_course_class_session', 'session');
        $sessionQuery->addField('session', 'waitlist_count');
        $sessionQuery->condition('session.class_id', $classId);
        $sessionQuery->condition('session.course_id', $courseId);
        $sessionResult = $sessionQuery->execute()->fetchAll();
        $waitlistAvailCapacity = $sessionResult[0]->waitlist_count;
        
        // To get the waitlisted capacity for class
        $enrolledCntQuery = db_select('slt_enrollment', 'enroll');
        $enrolledCntQuery->addField('enroll', 'id', 'id');
        $enrolledCntQuery->condition('enroll.class_id', $classId);
        $enrolledCntQuery->condition('enroll.course_id', $courseId);
        $enrolledCntQuery->condition('enroll.reg_status', array('lrn_crs_reg_wtl'), 'IN');
        $enrolledCntResult = $enrolledCntQuery->execute()->fetchAll();
        $waitlistedCapacity = count($enrolledCntResult);  
        
        if($waitlistedCapacity >= $waitlistAvailCapacity && !empty($waitlistAvailCapacity)){
          $rosterStatus['reg_status'] = t('MSG439').' '. t('Class').' '. t('LBL268');//t('Exceeding class waitlist capacity');
        } else {
          $rosterStatus['reg_status'] = 'Ok';
        }
      } else {
        $rosterStatus['reg_status'] = 'Ok';
      }
      
      // Completion Status check
      if(!empty($completionStatus) && $completionStatus != 'null'){
        if($dbRegistrationStatus == 'lrn_crs_reg_cnf' && $dbCompletionStatus != 'lrn_crs_cmp_cmp' && $dbCompletionStatus != 'lrn_crs_cmp_nsw' && (empty($registrationStatus) || $registrationStatus == 'null' || $registrationStatus == 'lrn_crs_reg_cnf')){
          $rosterStatus['comp_status'] = 'Ok';
        } else if($_REQUEST['apiname'] != 'UpdateRosterInternalAPI') {
          $rosterStatus['comp_status'] = t('MSG440');      
        }
        else if ($dbCompletionStatus == 'lrn_crs_cmp_cmp' && $_REQUEST['apiname'] == 'UpdateRosterInternalAPI'){
        	$rosterStatus['comp_status'] = 'Ok';
        }
      } else {
        $rosterStatus['comp_status'] = 'null';
      }
      
      // Payment Status - Not applicable
      $rosterStatus['pay_status'] = 'null';
      
      // Valid Status check
      
      // To get the waitlisted capacity for class
      $validQuery = db_select('slt_enrollment', 'enroll');
      $validQuery->addField('enroll', 'valid_from');
      $validQuery->condition('enroll.class_id', $classId);
      $validQuery->condition('enroll.course_id', $courseId);
      $validQuery->condition('enroll.user_id', $dbUserId, '=');
      $validResult = $validQuery->execute()->fetchAll();
      $dbValidFrom = $validResult[0]->valid_from;  
      
      if(!empty($contentValidFrom) && $contentValidFrom != 'null' && !empty($contentValidTo) && $contentValidTo != 'null'){
        if($dbRegistrationStatus == 'lrn_crs_reg_cnf' && $dbCompletionStatus != 'lrn_crs_cmp_cmp'){
          $rosterStatus['valid_status'] = 'Ok';
        } 
      } else if(!empty($dbValidFrom) && $dbValidFrom != 'null' && !empty($contentValidTo) && $contentValidTo != 'null'){
        if($dbRegistrationStatus == 'lrn_crs_reg_cnf' && $dbCompletionStatus != 'lrn_crs_cmp_cmp'){
          $rosterStatus['valid_status'] = 'Ok';
        }
      } else if(empty($dbValidFrom) && !empty($contentValidTo) && $contentValidTo != 'null'){
        if($dbRegistrationStatus == 'lrn_crs_reg_cnf' && $dbCompletionStatus != 'lrn_crs_cmp_cmp' && $_REQUEST['apiname'] != 'UpdateRosterInternalAPI' ){
          $rosterStatus['valid_status'] = 'Ok';
        } else {
          if($dbCompletionStatus == 'lrn_crs_cmp_cmp' && $_REQUEST['apiname'] != 'UpdateRosterInternalAPI'){
            $rosterStatus['valid_status'] = t('Course').' '.t('MSG431').' '. t('Completed');
          } else if($dbRegistrationStatus != 'lrn_crs_reg_cnf' && $_REQUEST['apiname'] != 'UpdateRosterInternalAPI'){
            $rosterStatus['valid_status'] = t('MSG440');
          }
        }
      } else {
        $rosterStatus['valid_status'] = 'null';
      }
    
      // Score status check
      if(!empty($score) && $score != 'null'){
        if( ($dbCompletionStatus == 'lrn_crs_cmp_cmp' || $completionStatus == 'lrn_crs_cmp_cmp' || $dbCompletionStatus == 'lrn_crs_cmp_nsw' || $completionStatus == 'lrn_crs_cmp_nsw' ) && ($dbRegistrationStatus == 'lrn_crs_reg_cnf') ){
          $rosterStatus['score_status'] = 'Ok';
        } else {
          $rosterStatus['score_status'] = t('Course').' '.t('MSG446').' '.t('Completed');//t('Course not completed');
        }
      } else {
        $rosterStatus['score_status'] = 'null';
      }
      
      // Grade status check
      if(!empty($grade) && $grade != 'null'){
        if( ($dbCompletionStatus == 'lrn_crs_cmp_cmp' || $completionStatus == 'lrn_crs_cmp_cmp' ) && ($dbRegistrationStatus == 'lrn_crs_reg_cnf') ){
          $rosterStatus['grade_status'] = 'Ok';
        } else {
          $rosterStatus['grade_status'] = t('Course').' '.t('MSG446').' '.t('Completed');//t('Course not completed');
        }
      } else {
        $rosterStatus['grade_status'] = 'null';
      }
      
      $rosterStatus['user_id'] = $dbUserId;
      $rosterStatus['user_name'] = $dbUserName;
      $rosterStatus['order_id'] = $dbOrderId;
    
      expDebug::dPrint('Roster Status = '.print_r($rosterStatus, true), 4);
      return $rosterStatus;
  } catch (Exception $ex) {
    watchdog_exception('catalogRoster', $ex);
    expertusErrorThrow($ex);
  }       
}


function getClassEnrolledCompStatus($masterEnrollmentId,$userId){
  try{
  	  $select = db_select('slt_enrollment', 'enroll');
  	  $select->addExpression('COUNT(1)','comp_count');
  	  $select->condition('enroll.comp_status', array('lrn_crs_cmp_cmp','lrn_crs_cmp_inp','lrn_crs_cmp_inc','lrn_crs_cmp_nsw'),'IN');
  	  $select->condition('enroll.master_enrollment_id', $masterEnrollmentId);
  	  $select->condition('enroll.user_id', $userId);
  	  expDebug::dPrintDBAPI('getProgramEnrolledStatus = ', $select);
  	  $result = $select->execute()->fetchField();
  	  return $result;	
     }catch (Exception $ex) {
    watchdog_exception('getProgramEnrolledStatus', $ex);
    expertusErrorThrow($ex);
   }       
}

function markEnrolledStatus($enrollId, $classId, $courseId, $enrollUserId,$updateFields) {	
	$updateFields['comp_date'] = NULL;
	$updateClassEnrollment = db_update('slt_enrollment');
	$updateClassEnrollment->fields($updateFields);
	$updateClassEnrollment->condition('id', $enrollId);
	$updateClassEnrollment->condition('user_id', $enrollUserId);
	$updateClassEnrollment->condition('course_id', $courseId);
	$updateClassEnrollment->condition('class_id', $classId);
	$updateClassEnrollment->condition('comp_status', array('lrn_crs_cmp_inc','lrn_crs_cmp_nsw'),'IN');
	$updateClassEnrollment->execute();
	expDebug::dPrintDBAPI(' $enrollQuery= ' , $updateClassEnrollment);
	$enrollQuery = db_select('slt_enrollment', 'enroll');
	$enrollQuery->addField('enroll', 'id' ,'id');
	$enrollQuery->addField('enroll', 'course_id' ,'course_id');
	$enrollQuery->addField('enroll', 'class_id' ,'class_id');
	$enrollQuery->addField('enroll', 'user_id' ,'user_id');
	$enrollQuery->addField('enroll', 'master_enrollment_id' ,'master_enrollment_id');
	$enrollQuery->condition('enroll.class_id', $classId);
	$enrollQuery->condition('enroll.course_id', $courseId);
	$enrollQuery->condition('enroll.user_id', $enrollUserId);
	$enrollQuery->condition('enroll.reg_status' , array('lrn_crs_reg_ppm','lrn_crs_reg_cnf'), 'IN');
	expDebug::dPrintDBAPI(' $enrollQuery= ' , $enrollQuery);
	$enrollResult = $enrollQuery->execute()->fetchAll();
	
	foreach($enrollResult as $enroll){
		$dbEnrollId       = $enroll->id;
		$dbMasterEnrollId = $enroll->master_enrollment_id;
		$dbUserId         = $enroll->user_id;
		$dbCourseId       = $enroll->course_id;
		$dbClassId        = $enroll->class_id;
	
		if(!empty($dbMasterEnrollId) && $dbMasterEnrollId != NULL){
			$programEnrolledCount = getClassEnrolledCompStatus($dbMasterEnrollId,$enrollUserId);
			if(!$programEnrolledCount){
				$currentDateTime = now();
				// Update the Master Enrollment Overall Status
				$updateStmt = db_update('slt_master_enrollment');
				$fields = array('overall_status' => 'lrn_tpm_ovr_enr',
						'updated_by' => $userId,
						'updated_on' => $currentDateTime
				);
				$updateStmt->condition('id', $dbMasterEnrollId);
				$updateStmt->condition('user_id', $enrollUserId);
				$updateStmt->condition('overall_status', 'lrn_tpm_ovr_inc');
				$updateStmt->fields($fields);
				$updateStmt->execute();
				expDebug::dPrintDBAPI(' $updateStmt for Master enrollment obj = ' , $updateStmt);
			}
		}
	}
	
}