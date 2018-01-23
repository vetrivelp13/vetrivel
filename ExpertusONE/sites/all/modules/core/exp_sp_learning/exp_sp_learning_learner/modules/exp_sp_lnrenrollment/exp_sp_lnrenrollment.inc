<?php
require_once('exp_sp_launch.inc');
include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc');
include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_core/exp_sp_core.inc');
include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrlearningplan/exp_sp_lnrlearningplan.inc');

function getLnrEnrollmentResultSet(){
  try{
  $user_id   	= stripQuotes($_REQUEST['UserID']);
  /*
  * To handle Sql Injection without session cookie (anonymous users)
  * Condition Added by : Vimal DM
  */
  $getmandatory = getmandatory($user_id,'cre_sys_obt_cls');
  $getLPmandatory = getLPmandatory($user_id,'cre_sys_obt_cls');
  $isCompliance = isCompliance($user_id);
  $sltPersonUserId = getSltpersonUserId();
  $usertimezone = getUserPHPTimezoneName();
  $usertimezone = $usertimezone[0]->zone_name;
  $timeZone = date_default_timezone(false);
  $defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
  $usertimezone = empty($usertimezone)? $defaultTimezone : $usertimezone;
  if(($sltPersonUserId != $user_id) && !isset($_REQUEST['apiname'])){
  	throw new Exception('Userid and Logged in user doesnot match.');
  }

  $regstatus  	= stripRegStatus($_REQUEST['regstatuschk']);
  $regstatus  	= stripApostrophe($regstatus);
		expDebug::dPrint("reg status check".$regstatus,5);
  $grade  	= $_REQUEST['grade'];
  
  $enrollment_id = isset($_REQUEST["enr_id"]) ? $_REQUEST["enr_id"] : null;
  $gradeStr ='';
  if($grade == 1){
  	$gradeStr = " and (myenroll.grade NOT IN('lrn_cnt_grd_fai') OR myenroll.grade IS NULL)";
  }
  elseif($grade == 2){
  	$gradeStr = " and IF(myenroll.reg_status_code ='lrn_crs_cmp_cmp',myenroll.grade = 'lrn_cnt_grd_fai',1=1)";
  }
  /* Paging Related */
  $page      = stripApostrophe($_REQUEST['page']);
  $limit     = stripApostrophe($_REQUEST['rows']);

  $sidx      = stripQuotes($_REQUEST['sidx']);
  $sord      = stripApostrophe($_REQUEST['sord']);
  $count    = stripApostrophe($_REQUEST['totalrows']);
		/*if(!empty($regstatus)){
    $regstatus = str_replace("|","','",$regstatus);
    $regstatus = "'".$regstatus."'";
  }
		/*$_REQUEST['deliveryType'] = 'lrn_cls_dty_ilt|lrn_cls_dty_wbt';
		$_REQUEST['trainingType'] = 'cre_sys_inv_com|cre_sys_inv_rec|cre_sys_inv_man|cre_sys_inv_opt';
		$_REQUEST['price'] = 'free';
		$_REQUEST['location'] = 'chennai';
		$_REQUEST['scheduledDays'] = 60;
		$_REQUEST['regDays'] = 60;
		$_REQUEST['dueDays'] = '';
		$_REQUEST['assignedBy'] = 'cre_sys_fop_oth|cre_sys_fop_oth|cre_sys_fop_mgr'; */
  
  		$timeZone = date_default_timezone(false);
  		$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
		$searchText = $_REQUEST['searchText'];
		$deliveryType = stripRegStatus(stripApostrophe($_REQUEST['del_type']));
		$trainingType = stripRegStatus(stripApostrophe($_REQUEST['tra_type']));
		$price = $_REQUEST['price'];
		$location = $_REQUEST['location'];
		$scheduledDays = $_REQUEST['scheduled'];
		$regDays = $_REQUEST['reg'];
		$dueDays = $_REQUEST['due'];
		$assignedBy = $_REQUEST['assignedby'];
		$or = ' OR ';
		$and = ' AND ';
		$config=getConfig("exp_sp");
		if(isset($_REQUEST['mobile_version']) || isset($_REQUEST['apiname'])){
			$query1 = "SELECT SQL_CALC_FOUND_ROWS * FROM ( ";
		} else {
			$query1 = "SELECT * FROM ( ";
		}
		if($_REQUEST['mobile_version'] && $_REQUEST['mobile_request'] && $_REQUEST['apiname'] == 'ListEnrollmentsAPI'){
			if ($dueDays > 0  || $scheduledDays > 0) {
				$classquery = getEnrollmentQuery($user_id, 1);
			} else {
				$classquery = getEnrollmentQuery($user_id, 0);
			}
			$tpquery = getLearningPlanResultSetMobileAPI($user_id);
		
			if($config['mylearning_mobile_view'] == 1){
				if($_REQUEST['Type'] == 'TP'){
					$query1.= $tpquery;
		
				}else{
		
					$query1.= $classquery;
				}
		
		
			}else{
				
				if($_REQUEST['Type'] == 'Class'){
					$query1.= $classquery;
				}else if ($_REQUEST['Type'] == 'TP'){
					$query1.= $tpquery;
				}else{
				$query1.= $classquery.' union all '.$tpquery;
			}
		
				
			}
		
		}else{
			if ($dueDays > 0 || $scheduledDays > 0) {
				$query1 .= getEnrollmentQuery($user_id, 1);
			} else {
				$query1 .= getEnrollmentQuery($user_id, 0);
			}
		
			if(isset($_REQUEST["sortBy"])) {
				$pos = strpos($query1, 'ORDER BY');
				if($pos === false){
					$query1 .= " ORDER BY ifnull(ses.start_date,'4000-12-30') , ses.start_date";
					$query1 .= " asc";
				}else{
					$query1 .= " , ifnull(ses.start_date,'4000-12-30') , ses.start_date";
					$query1 .= " asc";
				}
			}
		}
		
		
		
		
// 		if ($dueDays > 0) {
// 			$query1 .= getEnrollmentQuery($user_id, 1);
// 		} else {
// 			$query1 .= getEnrollmentQuery($user_id, 0);
// 		}
		
// 		  if($_REQUEST['mobile_version'] && $_REQUEST['mobile_request'] && $_REQUEST['apiname'] == 'ListEnrollmentsAPI'){
		  	
// 		  		$tpquery = getLearningPlanResultSetMobileAPI($user_id);
// 				$query1 .= ' union all '.$tpquery;  
// 		  }else{
// 		if(isset($_REQUEST["sortBy"])) {
// 			$pos = strpos($query1, 'ORDER BY');
// 			if($pos === false){
// 				$query1 .= " ORDER BY ifnull(ses.start_date,'4000-12-30') , ses.start_date";
// 				$query1 .= " asc";
// 			}else{
// 				$query1 .= " , ifnull(ses.start_date,'4000-12-30') , ses.start_date";
// 				$query1 .= " asc";
// 			}
// 		}
// 		}
		$query1 .= " ) myenroll ";
		
		$notificationTemplateInfo = getNotificationTemplateInfo('compliance_expiry_remainder', 'cre_sys_lng_eng');
		//$reregister_remainder_days 	= $config['reregister_remainder_days'];
		$reregister_remainder_days 	= $notificationTemplateInfo['notify_before'];
				//$regstatus = 'lrn_crs_cmp_enr|lrn_crs_cmp_cmp|lrn_crs_cmp_cmp_atl|lrn_crs_cmp_cmp_rereg';
  if(!isset($enrollment_id) && !empty($regstatus)){
  	
			$statusArray = explode('|',$regstatus);
			$i = 0;
			$query1 .= ' WHERE ( ';
			if (in_array('lrn_crs_cmp_cmp_atl', $statusArray)){
				if($_REQUEST['mobile_version'] && $_REQUEST['apiname'] == 'ListEnrollmentsAPI'){
				
					$query1 .= "(myenroll.reg_status_code IN ('lrn_crs_cmp_cmp','lrn_tpm_ovr_cmp') AND (myenroll.launchable = 1))";
				}else{
						
					$query1 .= "(myenroll.reg_status_code IN ('lrn_crs_cmp_cmp') AND (myenroll.launchable = 1))";
				}

				$i++;
 			 }
 			 
			 
			 if (in_array('lrn_crs_cmp_cmp_rereg', $statusArray)){
				$query1 .= ($i == 0) ? '' : $or;
			
			    if($_REQUEST['mobile_version'] && $_REQUEST['apiname'] == 'ListEnrollmentsAPI'){
					//New query added for reregister filter for mobile.
					$query1 .= "((is_compliance = 1 AND cmpl_expired = 1) OR (myenroll.reg_status_code IN ('lrn_crs_cmp_cmp') AND (is_compliance = 1) AND IF(compliance_validity_days IS NOT NULL,DATE_SUB(DATE_ADD(completion_date ,INTERVAL compliance_validity_days DAY),INTERVAL $reregister_remainder_days DAY) < now(), DATE_SUB(compliance_validity_date, INTERVAL $reregister_remainder_days DAY) < now()))
					OR
					(myenroll.reg_status = 'lrn_tpm_ovr_exp')
					OR
					(
					IF(myenroll.prg_expires_in_unit = 'days', DATE_SUB(DATE_ADD(myenroll.completion_date,interval myenroll.prg_expires_in_value DAY),INTERVAL $reregister_remainder_days DAY) <= NOW() ,
					(IF(myenroll.prg_expires_in_unit = 'months',DATE_SUB(DATE_ADD(myenroll.completion_date,interval myenroll.prg_expires_in_value DAY),INTERVAL $reregister_remainder_days MONTH) <= NOW(),
					DATE_SUB(DATE_ADD(myenroll.completion_date,interval myenroll.prg_expires_in_value DAY),INTERVAL $reregister_remainder_days YEAR) <= NOW())))
					AND
					(myenroll.check_entity_type IN ('cre_sys_obt_crt'))
					AND
					myenroll.reg_status = 'lrn_tpm_ovr_cmp'))";
				}
				else 
				{
					$query1 .= " ((is_compliance = 1 AND cmpl_expired = 1) OR (myenroll.reg_status_code IN ('lrn_crs_cmp_cmp') AND (is_compliance = 1) AND
					IF(compliance_validity_days IS NOT NULL,DATE_SUB(DATE_ADD(completion_date ,INTERVAL compliance_validity_days DAY),INTERVAL $reregister_remainder_days DAY) < now(),
					DATE_SUB(compliance_validity_date, INTERVAL $reregister_remainder_days DAY) < now()))
					) ";
				}
				$i++;
			}
			 if((in_array('lrn_crs_cmp_enr', $statusArray))||(in_array('lrn_crs_cmp_cmp', $statusArray))||(in_array('lrn_crs_reg_can', $statusArray))
			 ||(in_array('lrn_crs_reg_ppm', $statusArray))||(in_array('lrn_crs_cmp_inp', $statusArray))||(in_array('lrn_crs_cmp_att', $statusArray))||
  (in_array('lrn_crs_reg_wtl', $statusArray))||(in_array('lrn_crs_cmp_inc', $statusArray))||(in_array('lrn_crs_cmp_exp', $statusArray))||(in_array('lrn_crs_cmp_nsw', $statusArray)) || (in_array('lrn_crs_reg_cnf', $statusArray))){/*Viswanathan added (in_array('lrn_crs_cmp_exp', $statusArray)) for #0073745 */
				if (($key = array_search('lrn_crs_cmp_cmp_atl', $statusArray)) !== false) {
					unset($statusArray[$key]);
				}
				if (($key = array_search('lrn_crs_cmp_cmp_rereg', $statusArray)) !== false) {
					unset($statusArray[$key]);
				}
				$query1 .= ($i == 0) ? '' : $or;
				$ids = implode("','",$statusArray);
				expDebug::dPrint("join array element".$ids,5);
				$query1 .= "(myenroll.reg_status_code in ('".$ids."')) ";
				$i++;
			}
			
			  if($i == 0 && $_REQUEST['mobile_version'] && $_REQUEST['apiname']){
			  	$ids = implode("','",$statusArray);
			    $query1 .= "(myenroll.reg_status_code in ('".$ids."')) ";

  				}
			
			
			
			$query1 .= ') ';
		}else {
			$query1 .= " WHERE (myenroll.reg_status_code in ('lrn_crs_cmp_enr','lrn_crs_cmp_inp','lrn_crs_cmp_att','lrn_crs_reg_can','lrn_crs_cmp_cmp','lrn_crs_reg_ppm','lrn_crs_reg_wtl','lrn_crs_cmp_inc','lrn_crs_cmp_nsw') or ((is_compliance = 1 AND cmpl_expired = 1) OR (myenroll.reg_status_code IN ('lrn_crs_cmp_cmp') AND (is_compliance = 1) AND
				IF(compliance_validity_days IS NOT NULL,DATE_SUB(DATE_ADD(completion_date ,INTERVAL compliance_validity_days DAY),INTERVAL $reregister_remainder_days DAY) < now(),
				DATE_SUB(compliance_validity_date, INTERVAL $reregister_remainder_days DAY) < now()))
				) or  (myenroll.reg_status_code IN ('lrn_crs_cmp_cmp') AND (myenroll.launchable = 1))) ";
		}
		// Filter - Text search filter
		if(!empty($searchText)) {
			$query1 .= " AND (myenroll.cls_title like '%".addslashes($searchText)."%' OR myenroll.CODE like '%".addslashes($searchText)."%' OR myenroll.description like '%".addslashes($searchText)."%' OR myenroll.tagname like '%".addslashes($searchText)."%' ) ";
			//$query1 .= " AND (myenroll.title like '%".addslashes($searchText)."%')";
		}
		
		// Filter - Delivery type
		if(!empty($deliveryType)) {
			$deliveryType = str_replace("|","','",$deliveryType);
			$deliveryType = "'".$deliveryType."'";
			$query1 .= " AND (myenroll.delivery_type_code in (".$deliveryType."))  ";
		}
		//  Filter - Training type
		if(!empty($trainingType)) {
			$mrovalue= explode('|',$trainingType);
			$or = ' OR ';
			$and = ' AND ';
			$i = 0;
			$query1 .= ' AND ( ';
			expDebug::dPrint("mro value check".print_r($mrovalue,1),5);
			if (in_array('cre_sys_inv_man', $mrovalue)){
				$query1 .= "(myenroll.mandatory = 'Y') ";
				$i++;
			}
			if (in_array('cre_sys_inv_opt', $mrovalue)){
				$query1 .= ($i == 0) ? '' : $or;
				$query1 .= "  (myenroll.mro = 'Optional##cre_sys_inv_opt') ";
				$i++;
			}
			if (in_array('cre_sys_inv_rec', $mrovalue)){
				$query1 .= ($i == 0) ? '' : $or;
				$query1 .= "  (myenroll.mro = 'Recommended##cre_sys_inv_rec') ";
				$i++;
			}
			if (in_array('cre_sys_inv_com', $mrovalue)){
				$query1 .= ($i == 0) ? '' : $or;
				$query1 .= " (myenroll.is_compliance = '1') ";
				$i++;
			}
			$query1 .= ') ';
		}
		// Filter - Price
		if(!empty($price)) {
			if($price == 'free') {
				$query1 .= " AND (myenroll.classprice = '0.00') ";
			}
			else if($price == 'priced') {
				$query1 .= " AND (myenroll.classprice != '0.00') ";
			}
		}
		// Filter - Location
		if (!empty($location)){

			if($_REQUEST['apiname'] == 'ListEnrollmentsAPI' && $_REQUEST['mobile_version'])
			{
				$loc_or_condition_obj=getLocationFilterQuery($_REQUEST['location'],'1','myenroll','myenroll','myenroll');
				expDebug::dPrint('$loc_or_condition_obj 112233='.print_r($loc_or_condition_obj,true),5);
					
				$query1 .= " AND (".$loc_or_condition_obj.") ";
			}
			else if(($_REQUEST['selectedLocID'])) {
				$query1 .= " AND (myenroll.LocationId = ".$_REQUEST['selectedLocID'].") ";
			}
			else {
			//$query1 .= " AND (myenroll.LocationName LIKE '%".addslashes($location)."%' AND myenroll.delivery_type_code IN ('lrn_cls_dty_ilt','lrn_cls_dty_vcl')) ";
			$query1 .= " AND (CONCAT(myenroll.LocationName,IF(myenroll.LocationAddr1 IS NOT NULL ,CONCAT(',',myenroll.LocationAddr1),''),IF(myenroll.LocationAddr2 IS NOT NULL ,CONCAT(',',myenroll.LocationAddr2),''),myenroll.LocationCity,myenroll.stateName,myenroll.LocationZip) LIKE '%".addslashes($location)."%' AND myenroll.delivery_type_code IN ('lrn_cls_dty_ilt','lrn_cls_dty_vcl')) ";
			}
		}
		// Filter - Scheduled ( Converted to user timezone)
		$userTimezone = getUserTimezone();
		if(!empty($scheduledDays)) {
			//$query1 .=" AND (sessionDate BETWEEN NOW() AND DATE_ADD(NOW(), INTERVAL $scheduled DAY)) ";
			$query1 .=" AND (myenroll.sessionDateTime BETWEEN CONVERT_TZ(NOW(),myenroll.ptimezone,'$userTimezone') AND DATE_ADD(CONVERT_TZ(NOW(),myenroll.ptimezone,'$userTimezone'), INTERVAL $scheduledDays DAY))";
				
		}
		// Filter - Date registered ( Converted to user timezone)
		if(!empty($regDays)) {
			$query1 .=" AND (myenroll.registration_date >= DATE_SUB(CONVERT_TZ(NOW(),myenroll.ptimezone,'$userTimezone'),INTERVAL $regDays DAY)) ";
			//$query1 .= " AND (myenroll.registration_date >= DATE_SUB(NOW(),INTERVAL $regDate DAY) ) ";
		
		}
		// Filter - Due Date
		if(!empty($dueDays)) {
         	
         	if($_REQUEST['apiname'] == 'ListEnrollmentsAPI' && $_REQUEST['mobile_version'])
         	{
         		$query1 .=" AND (dueDate BETWEEN CONVERT_TZ(NOW(),'$defaultTimezone','$userTimezone') AND DATE_ADD(CONVERT_TZ(NOW(),'$defaultTimezone','$userTimezone'), INTERVAL $dueDays DAY) AND myenroll.reg_status_code NOT IN ('lrn_crs_reg_can','lrn_tpm_ovr_cln')  AND myenroll.comp_status NOT IN ('lrn_crs_cmp_inc','lrn_crs_cmp_nsw','lrn_tpm_ovr_inc','lrn_crs_cmp_cmp')) ";
         	}
            else
            {
			$query1 .=" AND (dueDate BETWEEN CONVERT_TZ(NOW(),'$defaultTimezone','$userTimezone') AND DATE_ADD(CONVERT_TZ(NOW(),'$defaultTimezone','$userTimezone'), INTERVAL $dueDays DAY) AND myenroll.reg_status_code != 'lrn_crs_reg_can' AND myenroll.comp_status NOT IN ('lrn_crs_cmp_inc','lrn_crs_cmp_nsw','lrn_crs_cmp_cmp')) ";
		}
		}
		// Filter - Assigned By
	    if(!empty($assignedBy)) {
			if($assignedBy == 'cre_sys_fop_me') {
				$query1 .= " AND (myenroll.created_by = 2) ";
			}
			else if($assignedBy == 'cre_sys_fop_mgr'){
				$query1 .=" AND ((myenroll.created_by = managerID) OR myenroll.created_by IN (SELECT manager_id from slt_person_other_manager where user_id=".$user_id.")) ";
					
			}
			else if($assignedBy == 'cre_sys_fop_oth') {
				$query1 .= " AND (myenroll.created_by != ".$user_id." AND if(managerID IS NOT NULL,myenroll.created_by != managerID,1=1) AND myenroll.created_by NOT IN (SELECT manager_id from slt_person_other_manager where user_id=".$user_id.")) ";
											}
		}
		if($_REQUEST['apiname'] != 'ListEnrollmentsAPI')
		{
			$query1 .= "AND master_enrollment_id IS NULL ";
		}
		
		if($_REQUEST['apiname'] == 'ListEnrollmentsAPI' && $_REQUEST['mobile_version'])
  		{
			if(empty($_REQUEST["enroll_id"])){ //Added condition for survey DeepLink
  			$query1 .= "AND master_enrollment_id IS NULL" ;
			}
  		}

		if($_REQUEST['apiname'] == 'ListEnrollmentsAPI' && $_REQUEST["ClassTitle"]){
			$query1 .= " AND myenroll.cls_title like '%".addslashes($_REQUEST["ClassTitle"])."%'";

  }
  if($_REQUEST['apiname'] == 'ListEnrollmentsAPI' && $_REQUEST['delivery_type_code'])
    {
    		$tmp_arr_dtype = explode("|",$_REQUEST['delivery_type_code']);
	    	$del_types = "";
	    	for($i = 0; $i < sizeof($tmp_arr_dtype); $i++)
	    		if($del_types == "")
	    			$del_types = "'".stripApostrophe($tmp_arr_dtype[$i])."'";
	    		else
	    			$del_types.= ",'".stripApostrophe($tmp_arr_dtype[$i])."'";

			$query1 .= " AND myenroll.delivery_type_code in (".$del_types.")";
		}
		
	if($_REQUEST['apiname'] == 'ListEnrollmentsAPI' && $_REQUEST['compliance_mandatory'] && $_REQUEST['mobile_request']){
    	if($_REQUEST['compliance_mandatory'] == 'cre_sys_inv_man'){
    		$query1 .= " AND myenroll.mandatory = 'Y' ";
    	}else if($_REQUEST['compliance_mandatory'] == 'cre_sys_inv_com'){
    		$query1 .= " AND myenroll.is_compliance ";
    	}
    }
    
    if($_REQUEST['apiname'] == 'ListEnrollmentsAPI' && $_REQUEST['certificate_expiry'] && $_REQUEST['mobile_request']){
    	
    	$query1 .= " AND prg_expires_on between now() and now() + interval 30 DAY ";
    }
		
		
		
		
		/*  if(isset($_REQUEST["sortBy"])) {
			$sortBy = $_REQUEST["sortBy"];
			if($sortBy != 'startdate') {
			$query1 .= " GROUP BY myenroll.id ";
			}
			} */
			
    if(!isset($_REQUEST["mobile_version"])){
    	$query1 .= " GROUP BY myenroll.id "; // Commented Above code for This is #0046076 By Default it should be Group by
    }
    

  if(isset($_REQUEST["sortBy"])) {
    $sortBy = $_REQUEST["sortBy"];
    $sidx   = '';
				if($sortBy == 'orderbystatus') {
					$comma = ',';
				$enrSts = getStatusFilters('myenrollment');
				$sortbyStatusArray = explode("|",$enrSts);
				$query1 .= ' ORDER BY ';
				$p = 0;
				if(!empty($enrSts)){
					if (in_array('Enrollmentpart', $sortbyStatusArray)){
						$query1 .= " myenroll.reg_status_code in ('lrn_crs_cmp_enr','lrn_crs_cmp_inp','lrn_crs_cmp_att') DESC ";
						$p++;
					}
					if (in_array('EnrollCompleted', $sortbyStatusArray)){
						$query1 .= ($p == 0) ? '' :$comma;
						$query1 .= " myenroll.reg_status_code in ('lrn_crs_cmp_cmp') DESC  ";
						
						$p++;
					}
					if (in_array('EnrollCompletedWithAttempts', $sortbyStatusArray)){
						$query1 .= ($p == 0) ? '' :$comma;
						$query1 .= " (myenroll.reg_status_code IN ('lrn_crs_cmp_cmp') AND ((if((myenroll.delivery_type_code = 'lrn_cls_dty_wbt' or myenroll.delivery_type_code = 'lrn_cls_dty_vod'),myenroll.content_total_attempts  = 0,0)) or ((myenroll.post_ass_total_attempts + myenroll.content_total_attempts) > myenroll.total_attempts))) DESC";				
						$p++;
					}
					if (in_array('EnrollReregister', $sortbyStatusArray)){
						$config=getConfig("exp_sp");
					//$reregister_remainder_days 	= $config['reregister_remainder_days']; 
					$notificationTemplateInfo = getNotificationTemplateInfo('compliance_expiry_remainder', 'cre_sys_lng_eng');
					$reregister_remainder_days 	= $notificationTemplateInfo['notify_before'];
						$query1 .= ($p > 1) ? $comma :'';
						$query1 .= " ((is_compliance = 1 AND cmpl_expired = 1) OR (myenroll.reg_status_code IN ('lrn_crs_cmp_cmp') AND (is_compliance = 1) AND
						IF(compliance_validity_days IS NOT NULL,DATE_SUB(DATE_ADD(completion_date ,INTERVAL compliance_validity_days DAY),INTERVAL $reregister_remainder_days DAY) < now(),
						DATE_SUB(compliance_validity_date, INTERVAL $reregister_remainder_days DAY) < now()))
						) DESC";
						$p++;
					}
					if (in_array('EnrollInCompleted', $sortbyStatusArray)){
						$query1 .= ($p == 0) ? '' :$comma;
						$query1 .= " myenroll.reg_status_code in ('lrn_crs_cmp_inc','lrn_crs_cmp_nsw') DESC  ";
							
						$p++;
					}
					if (in_array('EnrollCanceled', $sortbyStatusArray)){
						$query1 .= ($p == 0) ? '' :$comma;
						$query1 .= " myenroll.reg_status_code in ('lrn_crs_reg_can') DESC ";
						
						$p++;
					}
					if (in_array('EnrollPayments', $sortbyStatusArray)){
						$query1 .= ($p == 0) ? '' :$comma;
						$query1 .= " myenroll.reg_status_code in ('lrn_crs_reg_ppm','lrn_crs_reg_wtl') DESC ";
						
						$p++;
					}
				}else{
					$query1 .= " myenroll.reg_status_code DESC  ";
				}
			}
				if($sortBy == 'AZ') {
				$query1 .= " ORDER BY myenroll.cls_title asc ";
    		}
    		if($sortBy == 'ZA') {
				$query1 .= " ORDER BY myenroll.cls_title desc ";
			}
       		if($sortBy == 'dateOld') {
				if($_REQUEST['apiname'] == 'ListEnrollmentsAPI')
				$query1 .= " ORDER BY myenroll.id asc "; // Issue ID :51553
   				else
				$query1 .= " ORDER BY myenroll.id desc ";
			}
    		if($sortBy == 'dateNew') {
         		if($_REQUEST['apiname'] == 'ListEnrollmentsAPI'&& !$_REQUEST['mobile_version'])
    				$query1 .= " ORDER BY myenroll.id desc "; // Issue ID :51553
       			 else if($_REQUEST['apiname'] == 'ListEnrollmentsAPI' && $_REQUEST['mobile_version']) 
        			$query1 .= " ORDER BY myenroll.registration_date desc "; // Issue ID :51553
    			else
    				$query1 .= " ORDER BY myenroll.id asc ";
      		}
    		if($sortBy == 'type') {
				$query1 .= " ORDER BY myenroll.delivery_type_code asc ";
      		}
      		if($sortBy == 'startdate') {
    
      				$query1 .= " ORDER BY ifnull(TIMESTAMP(DATE_FORMAT(STR_TO_DATE(myenroll.session_start, _utf8'%d-%b-%Y'), '%Y-%m-%d'),myenroll.session_start_time ),'4000-12-30 00:00:00') asc";
      			
      		}
   			 if($sortBy == 'Mandatory') {
    			 $query1 .= " ORDER BY FIELD(mro, 'Mandatory##cre_sys_inv_man') desc";
		         $query1 .= ",myenroll.mandatory = 'Y' desc";
		         $query1 .= ",myenroll.is_compliance desc";
		         $query1 .= ",FIELD(mro, 'Recommended##cre_sys_inv_rec') desc";
      		}

   }
  else{
   	if($sidx ==''){
	   	if($getmandatory > 0 || $isCompliance > 0 || $getLPmandatory > 0){
	   		$query1 .= " ORDER BY FIELD(mro, 'Mandatory##cre_sys_inv_man') desc";
	   		$query1 .= ",myenroll.mandatory = 'Y' desc";
	   		$query1 .= ",myenroll.is_compliance desc";
	   		$query1 .= ",FIELD(mro, 'Recommended##cre_sys_inv_rec') desc";
	   	}
	   	else{
	   		//$query1 .= " ORDER BY myenroll.cls_title asc ";
	   		$query1 .= " ORDER BY myenroll.id desc ";
	   	}
   	}
   }
  if($sidx !=''){
     $query1 .= " ORDER BY $sidx, $sord ";
  }
	   	expDebug::dPrintDBAPI("enrollment query check",$query1,5);
  //APIs and Mobile APP need the total number of results and the total number of avaialble pages for the given filter criteria
   	if(isset($_REQUEST['mobile_version'])) {
  if($count == 0){
  	$start = 0;
  	$total_pages = 0;
  }else{
  		$total_pages = ceil($count/$limit);
  	if($page > $total_pages){
  		$page=$total_pages;
  	}
  	$start = $limit*$page - $limit;
  	if($start < 0) {
  		$start = 0;
  	}
  }
	} 
	elseif(isset($_REQUEST['apiname'])){
		$start = $limit*$page - $limit;
	
		if($start < 0) {
			$start = 0;
		}
	}
	else {
		$start = calculate_offset($page, $limit);
		$count = $limit;
	}

  $query1 .= " LIMIT  $start, $limit ";
  //expDebug::dPrint('Total First : '.$query1 , 4);
  $cntQry = $query1;
  //expDebug::dPrint("Enrollment Query Fetch DATA :-");
  expDebug::dPrint('fetch my enrollments query with start and limit == '.$cntQry , 2);
  $selectEnrCnt = db_query($cntQry);
  //APIs and Mobile APP need the total number of results and the total number of avaialble pages for the given filter criteria 
  if(isset($_REQUEST['mobile_version']) || isset($_REQUEST['apiname'])) {
  $totRecQry  = db_query('select found_rows() as tot_records');
  $count1  = $totRecQry->fetchField();
  if($count != $count1 ){
  	$count = $count1;
	  if( $count >0 ){
	  	$total_pages = ceil($count/$limit);
	  }else{
	  	$total_pages = 0;
	  }
	  if($page > $total_pages){
	  	$page=$total_pages;
	  }
	  $start = $limit*$page - $limit;
	  if($start < 0) {
	  	$start = 0;
	  }
  }
  }
  $pagination = array();
  $lpQryRes = $selectEnrCnt->fetchAll();
  $pagination['result'] = $lpQryRes;
  $pagination['page'] = $page;
  $pagination['count'] = $count;
  $pagination['total_pages'] = $total_pages;
  return $pagination;
  }catch (Exception $ex) {
    watchdog_exception('getLnrEnrollmentResultSet', $ex);
    expertusErrorThrow($ex);
  }
}
function getListEnrollmentApiResultSet(){
  try{
   drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
   include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_survey/exp_sp_survey_learner/exp_sp_surveylearner.inc');
   if($_REQUEST['apiname'] == 'ListEnrollmentsAPI')
   {
	   	$rows = trim($_REQUEST['rows']);
	   	if($rows !== null && !is_numeric($rows)) {
	   		$errobj=new stdClass();
	   		$errobj->isValidateError = 1;
	   		$errobj->field = "rows";
	   		$errobj->errormsg = 'rows should be a number';
	   		return $errobj;
	   	}
	   $date_from = trim($_REQUEST['date_from']);
	   $date_to = trim($_REQUEST['date_to']);
	   if(!empty($date_from)){
		   	if(!validateDateFormat($date_from))
		   	{
		   		$errobj=new stdClass();
		   		$errobj->isValidateError = 1;
		   		$errobj->field = "date_from";
		   		$errobj->errormsg = 'From date should be in yyyy-mm-dd format';
		   		return $errobj;
		   	}
	   }
	   if(!empty($date_to)){
		   	if(!validateDateFormat($date_to))
		   	{
		   		$errobj=new stdClass();
		   		$errobj->isValidateError = 1;
		   		$errobj->field = "date_to";
		   		$errobj->errormsg = 'To date should be in yyyy-mm-dd format';
		   		return $errobj;
		   	}
	   }
	   if(!empty($date_from) && !empty($date_to)) {
		   	if(strtotime($date_from) > strtotime($date_to)) {
		   		$errobj=new stdClass();
		   		$errobj->isValidateError = 1;
		   		$errobj->field = "date_to";
		   		$errobj->errormsg = 'To date should be greater than or equal to from date';
		   		return $errobj;
		   	}
	   }
	   $regstatus  	= stripRegStatus($_REQUEST['regstatuschk']);
       $regstatus  	= stripApostrophe($regstatus);
	   if(!empty($regstatus)){
	   	if(strpos($regstatus, ',') !== false){
	   		$errobj=new stdClass();
	   		$errobj->isValidateError = 1;
	   		$errobj->field = "regstatuschk";
	   		$errobj->errormsg = 'Invalid separator or status';
	   		return $errobj;
	   	}
	   }
   }
   // SOLR check starts
   $srcSource = getConfigValue('solr_search');
   $solrResult = FALSE;
   
   if($srcSource == 0){
   	$getlpQryRes = getLnrEnrollmentResultSet();
   	$lpQryRes = $getlpQryRes['result'];
   	expDebug::dPrint('results from getLnrEnrollmentResultSet method '.print_r($lpQryRes, 1), 5);
   }
   else {
   	require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_mylearning_enrollment.php';
   	try{
   		expDebug::dPrint("Solr Search Testing",1);
   		$rtnType = $_REQUEST['returntype'];
   		$_REQUEST['returntype'] = 'json';
   		$solrObj = new MyLearningEnrollmentSolrSearch();
   		$getlpQryRes = $solrObj->EnrollmentSolrSearch();
   		expDebug::dPrint('Result set from solr12333lathhhaaaaaaa'.print_r($getlpQryRes,true),5);
   		$lpQryRes = $getlpQryRes['result'];
   		expDebug::dPrint('Result set from solr'.print_r($lpQryRes,true),5);
   		$count = $getlpQryRes['count'];
   		expDebug::dPrint('Result set count'.print_r($count,true),5);
   		$getlpQryRes['page']    = $_GET['page'];
   		$getlpQryRes['total_pages']   = ceil($getlpQryRes['count']/$_GET['rows']);
   		expDebug::dPrint('Result set from sol1233333r'.print_r($getlpQryRes,true),5);
   		$solrResult = TRUE;
   		$_REQUEST['returntype'] = $rtnType;
   		unset($_REQUEST['SolrImpl']); // TODO: Instead of unset this, implement below functionalities in solr srarch
   	}catch(Exception $e){
   		expDebug::dPrint("Error in Solr Search -- ".print_r($e->getMessage(),true),1);
   	}
   	expDebug::dPrint("Solr search result - Final ".print_r($getlpQryRes,1),4);
   }
   $masterEnrArr = array();
   if($getlpQryRes['count'] && count($lpQryRes)){
     $lpQryRes['totalrow'] = $getlpQryRes['count'];
   }
   $user_id   	= stripQuotes($_REQUEST['UserID']);
    expDebug::dPrint("Enrollment Query Fetch DATA :- lpQryRes".print_r($lpQryRes,true),5);
   foreach ($lpQryRes as $index => $record) {
      $record->launch = ($record->basetype=='WBT'  || $record->basetype=='VOD')?getLaunchDetail($record->id,1):array();
      $record->sessiondetails = ($record->basetype=='ILT' || $record->basetype=='VC')?getSessionDetails($record->course_id,$record->class_id,$record->delivery_type_code,"enroll"):array();
      $record->rating = getEntityRatingInfo($record->node_id,'Class');
      $record->user_rating = getUserRatingvalueforClass(getDrupalIdOfUser($user_id),$record->node_id);
      $record->created_by_name = ($record->created_by)?(($full_name=getPersonDetails($record->created_by, array('full_name')))?$full_name['full_name']:'') :'';
      $record->updated_by_name = ($record->updated_by)?(($updated_by=getPersonDetails($record->updated_by, array('full_name')))?$updated_by['full_name']:'') :'';
      $record->created_by_ins_mngr_slf = $record->created_by ? $updated_by=getUpdatedByDetails($record->user_id, $record->created_by) : t('Me');
      $record->updated_by_ins_mngr_slf = $record->updated_by ? $updated_by=getUpdatedByDetails($record->user_id, $record->updated_by) : t('Me');
   if(empty($record->master_enrollment_id))
      	//expDebug::dPrint("check_explode_value_in_web".print_r(explode("|",$mob_exempted_sts),true),5);
      	
      	if(!empty($record->exempted_sts)){
      		expDebug::dPrint("check_explode_value_in_web".print_r($record->exempted_sts,true),5);
      		$mob_exempted_sts = explode("|",$record->exempted_sts);
      		$record->waived_status = $mob_exempted_sts[0];
      		expDebug::dPrint("check_explode_value_in_web4785".print_r($mob_exempted_sts[0],true),5);
      }else{
      	$record->waived_status = "";
      }
      if($record->basetype=='ILT'  || $record->basetype=='VC'){
      $pastDateCount  = checkPastDateOrNot($record->class_id,$record->delivery_type_code);
      $record->class_status = $pastDateCount > 0 ? $class_status = t('LBL105'):$class_status = t('Scheduled');
      }

      if(!isset($_REQUEST["mobile_request"])) {
      if(!empty($record->master_enrollment_id)){
		if(in_array($record->master_enrollment_id, $masterEnrArr)){
			$oldIndex = array_search($record->master_enrollment_id, $masterEnrArr);
			$record->tp_details = $lpQryRes[$oldIndex]->tp_details;
			$record->tp_course_data = $lpQryRes[$oldIndex]->tp_course_data;
			$record->waived_status = $lpQryRes[$oldIndex]->waived_status;
		}
		else{
        $select = db_select('slt_master_enrollment','mas');
        $select->leftjoin('slt_program', 'prm', 'mas.program_id = prm.id');
	    $select->addField('mas','program_id');
	    if(contentPlayerIsActive())
	    	$select->addField('mas','overall_progress','percentage_complete');
	    else
	    	$select->addField('mas','percentage_complete','percentage_complete');
	    $select->addField('mas','overall_status');
	    $select->addField('prm','title');
	    $select->addField('prm','object_type');
	    $select->condition('mas.id',$record->master_enrollment_id);
		expDebug::dPrintDBAPI('$select',$select);
	    $tp_details = $select->execute()->fetchAll();
	    $programID=$tp_details[0]->program_id;
	    $record->tp_details=$tp_details;
	    $record->tp_course_data = lp_fetch_module_list($programID,$user_id,'N','',1);
	  //  $record->tp_course_data = tpdetailsforAPI($record->master_enrollment_id,$record->user_id,1);
	    //$record->waived_status = (empty(getRegisteredEnrollmentExmeptedorNot('tp', $record->user_id,$tp_details[0]->program_id)))? 'Yes' : 'No';
	    $waived_var = getRegisteredEnrollmentExmeptedorNot('tp', $record->user_id,$tp_details[0]->program_id);
	    $record->waived_status =(empty($waived_var))?'yes':'no';
        expDebug::dPrint("Enrollment Query Fetch DATA :- program_id".print_r($result->tp_course_data,true) ,5);
	        $masterEnrArr[$index] = $record->master_enrollment_id;
      }
      }
      }

      $userid = $record->user_id;
      $objectId = $record->class_id;
			if ($record->basetype == 'TP') {
				expDebug::dPrint ( "Inside Training plan condition" );
				$objectType = $record->delivery_type_code;
				$record->tp_expiry_date = getExpiredDate($record->comp_date,$record->prg_expires_in_value,$record->prg_expires_in_unit);
				$record->rating = getEntityRatingInfo ( $record->node_id, $objectType );
				
				$nxt_mdl = getModuleIdFromObjectId($record->course_id,($record->recertify_path)+1);
				$nxt_mdl_crs = getGrpCourseCnt($record->course_id,$nxt_mdl[0]->module_id);
				$record->recertify_before_certify_expired = ($record->completion_date  && $nxt_mdl_crs > 0 ) ? ($record->delivery_type_code == 'cre_sys_obt_crt') ? getRecertifyBeforeCertifyExpired($record->completion_date,$record->prg_expires_in_value,$record->prg_expires_in_unit) : 0 : 0;
				
				if($_REQUEST['mobile_request']){
					$attachment_status = getAttachmentDetails( $record->course_id, 'cre_sys_obt_trp', '' );
					$record->attachment_status = $attachment_status;
				}
				
			} else {
				expDebug::dPrint ( "Inside ELSE Training plan condition" );
      $objectType = 'cre_sys_obt_cls';
      
// Code Added for mobile content player

// Fetch the survey and assessment count for content palyer(0064431)
      if($_REQUEST['mobile_request']){
      	$select = db_select('slt_survey_mapping','sc');
      	$select->addField('sc','survey_id');
      	$select->addField('sc','pre_status');
      	$select->condition('sc.object_id',$objectId);
      	expDebug::dPrintDBAPI('query for nodeid223344',$select);
      	$result = $select->execute()->fetchAll();
      	expDebug::dPrint("assessment_surveyded_count4444 :  -  ".print_r($result,true),5);
      
      	$record->assessment_survey_count = count($result);
      	
      	expDebug::dPrint('enrollllllId'.print_r($result[0]->survey_id,true),5);
      	$overall_assessment_progress = '';
      	$assessment_survey_completion = 0;
      	for($i = 0; $i < count($result); $i++){
      	
       	$select = db_select('slt_attendance_summary','pr');
       	$select->addField('pr','progress');
       	$select->addField('pr','status');
       	$select->condition('pr.enrollment_id',$record->id);
       	$select->condition('pr.assessment_id',$result[$i]->survey_id );
       	expDebug::dPrintDBAPI('query for nodeid4566756gtg',$select);
       	$result1 = $select->execute()->fetchAll();
       	expDebug::dPrint("assessment_survey_count232323 :  -  ".print_r($result1,true),5);
       	
       	$overall_assessment_progress += $result1[0]->progress;
       	
       	if($result1[0]->progress == 100){
       		$assessment_survey_completion = $assessment_survey_completion + 1;
       	}
       	

       	
      	}
      	
      	expDebug::dPrint('callingafter112543r4rre4r3'.print_r($overall_assessment_progress,true),5);
      	
      	$record->assessment_survey_progress = $overall_assessment_progress;
      	$record->assessment_survey_completion = $assessment_survey_completion;
      //Added to fetch the no of attemptes and attempts for pre assessment
       
      $myasses_status = getAssignedSurveys($userid,$objectId,$objectType,'assessment','1',$record->id,'yes');
      
      expDebug::dPrint("Enrollment Query Fetch_myasses_status :- lpQryRes".print_r($myasses_status,true),5);
       
      foreach ($myasses_status as $status_record) {
      	 
      	$record->pre_total_attempts = $status_record->no_of_attempts;
      	 
      	$record->pre_attempts = $status_record->attempts;
      	 
      	expDebug::dPrint("my_survey_attempts11".print_r($my_survey_total_attempts,true),5);
      	 
      }
       
      //Added to fetch the no of attemptes and attempts for post assessment
      
      $myasses_status = getAssignedSurveys($userid,$objectId,$objectType,'assessment','0',$record->id,'yes');
       
      expDebug::dPrint("Enrollment Query Fetch_myasses_status :- lpQryRes".print_r($myasses_status,true),5);
      foreach ($myasses_status as $status_record) {
      	$record->post_total_attempts = $status_record->no_of_attempts;
      	$record->post_attempts = $status_record->attempts;
       $record->post_assessment_completion = $status_record->compstatus;
					expDebug::dPrint ( "my_survey_attempts11_post" . print_r ( $my_survey_total_attempts, true ), 5 );
				}
      
				if($record->delivery_type == 'Classroom' || $record->delivery_type == 'Virtual Class'){
					$session_detail = getClassSessionsDateTime($record->class_id);
					$sccs_date_and_time = implode(' ', array($session_detail[0][date],$session_detail[0][end_time]));
					$curr_date_and_time =  date('Y-m-d H:i',strtotime("-30 minutes"));
					if (strtotime($curr_date_and_time) >= strtotime($sccs_date_and_time)) {
						$record->ilt_sessionCheck = 'Session_completed';
					}else{
						$record->ilt_sessionCheck = 'Session_not_completed';
					}
      	 
				}
   expDebug::dPrint("getListEnrollmentApiResultSet Final Result".print_r($lpQryRes,true),5);
				expDebug::dPrint('calling after 1125433'.print_r($record,true),5);

				


      }
      
      	 	//To fetch the progress for each class
      	 	if($record->id){
      	 		$user_detail=db_query("select progress from slt_enrollment where id=".$record->id);
      	 		$userDetail = $user_detail->fetchAll();
      	 		$record->class_progress = $userDetail[0]->progress;
      	 	}
			//End of the code
			if($_REQUEST['mobile_request']){
              $attachment_status = getAttachmentDetails( $record->class_id, 'cre_sys_obt_cls', $record->course_id );
              $record->attachment_status = $attachment_status;
			}
				
			}
      }
      expDebug::dPrint("getListEnrollmentApiResultSet Final Result --- >>> ".print_r($lpQryRes,true),5);

   return  $lpQryRes;
  }catch (Exception $ex) {
    watchdog_exception('getListEnrollmentApiResultSet', $ex);
    expertusErrorThrow($ex);
  }
}

function getTpDetailsForAClass() {
	try{

		// 		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc');
		$result = array();
		expDebug::dPrint("Ajay  1111");
		$select = db_select('slt_master_enrollment','mas');
		$select->leftjoin('slt_program', 'prm', 'mas.program_id = prm.id');
		$select->addField('mas','program_id');
		$select->addField('mas','overall_status');
		$select->addField('mas','percentage_complete');
		$select->addField('prm','title');
		$select->addField('prm','object_type');
		$select->condition('mas.id',$_REQUEST['master_enrollment_id']);
		expDebug::dPrintDBAPI('$select',$select);
		$tp_details = $select->execute()->fetchAll();
		$programID=$tp_details[0]->program_id;
		$result[0]->tp_details=$tp_details;
		$result[0]->tp_course_data = lp_fetch_module_list($programID,$_REQUEST['userid'],'N','',1);
		expDebug::dPrint("Enrollment Query Fetch DATA Ajay = :- program_id".print_r($result[0]->tp_course_data,true) ,5);
		return $result;
	}
	catch (Exception $ex) {
		watchdog_exception('getTpDetailsForAClass', $ex);
		expertusErrorThrow($ex);
	}
}

function getEnrollSearchResults() {
	global $tTrack;
	$tTrack['end']['before_enroll_search'] = (microtime(true) - $tTrack['start']['index_start']);
	$tTrack['start']['enroll_search'] = microtime(true);
  try{
  global $theme_key;
  $tTrack['start']['enroll_query'] = microtime(true);
  // SOLR check starts
  $srcSource = getConfigValue('solr_search');
  $solrResult = FALSE;
  
  if($srcSource == 0){
  $getlpQryRes = getLnrEnrollmentResultSet();
  $lpQryRes = $getlpQryRes['result'];
  expDebug::dPrint('results from getLnrEnrollmentResultSet method '.print_r($lpQryRes, 1), 5);
  }
  else {
		require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_mylearning_enrollment.php';
		try{
			expDebug::dPrint("Solr Search Testing",1);
			$solrObj = new MyLearningEnrollmentSolrSearch();
			$getlpQryRes = $solrObj->EnrollmentSolrSearch();
			expDebug::dPrint('Result set from solr12333lathhhaaaaaaa'.print_r($getlpQryRes,true),5);
			$lpQryRes = $getlpQryRes['result'];
			expDebug::dPrint('Result set from solr'.print_r($lpQryRes,true),5);
			$count = $getlpQryRes['count'];
			expDebug::dPrint('Result set count'.print_r($count,true),5);
			$getlpQryRes['page']    = $_GET['page'];
     		$getlpQryRes['total_pages']   = ceil($getlpQryRes['count']/$_GET['rows']);
     		expDebug::dPrint('Result set from sol1233333r'.print_r($getlpQryRes,true),5);
     		$solrResult = TRUE;
    }catch(Exception $e){
			expDebug::dPrint("Error in Solr Search -- ".print_r($e->getMessage(),true),1);
		}
		expDebug::dPrint("Solr search result - Final ".print_r($getlpQryRes,1),4);
	
  	
  }
  
  $tTrack['end']['enroll_query'] = (microtime(true) - $tTrack['start']['enroll_query']);
  expDebug::dPrint('results from getLnrEnrollmentResultSet method '.print_r($lpQryRes, 1), 5);
  $result_response= new stdClass();
  $result_response->page    = $getlpQryRes['page'];
  $result_response->total   = $getlpQryRes['total_pages'];
  $result_response->records = $getlpQryRes['count'];

  $enrids = array();
  foreach ($lpQryRes as $record) {
 
 	$enrids[] = $record->id;
  }
  expDebug::dPrint('results from getLnrEnrollmentResultSet method12333456 '.print_r($lpQryRes, 1), 5);
  $sessDts = array();
  $timezone_details = getPersonDetails($record->user_id);
  $ses = getSessionDetails($record->course_id,$record->class_id,$record->delivery_type_code,"enroll",'','',$enrids);
  $classArr = array();
  $storeArr = true;
  $classSession = array();
  $sessionDetails = array();
  $hasTriggeringLessonDetails = false;
  $i=0;
  $tTrack['start']['enroll_query_loop'] = microtime(true);
  foreach ($lpQryRes as $record) {
  	$settings = array(
  			'content_type' => 'Class',
  			'content_id'   => $record->node_id,
  			'stars'				 => 5,
  			'autosubmit'   => TRUE,
  			'allow_clear'  => FALSE,
  			'required' 		 => FALSE,
  			'tag' 				 => 'vote',
  			'style'		 		 => 'average',
  			'text' 		 		 => 'average',
  			'startype' 		 => $record->delivery_type_code,
  	);
      $complianceCompleteDate     = '';
      $complianceValidityDate     = '';
      $complianceExpired          = FALSE;
      $strtotimeNow = strtotime(date("M d, Y"));
      $recId = $record->id;
      $complianceCompleteDate[$recId]  = '';
      $complianceValidityDate[$recId]  = '';

      if(($record->compliance_complete_date != null) || ($record->compliance_complate_days != null)) {
        if($record->compliance_complate_days != null) {
          $dateToCalc = ($record->compliance_completed == 'hire_days') ? $record->hire_date : $record->reg_date;
          $complianceCompleteDate[$recId]  = ($dateToCalc != '')? (strtotime(date("Y-m-d",strtotime($dateToCalc)) . "+".$record->compliance_complate_days."days")) : ''; 
        } else {
          $complianceCompleteDate[$recId]  = strtotime($record->compliance_complete_date);
        }
      }

      if(($record->compliance_validity_date != null) || ($record->compliance_validity_days != null)) {
		if($record->compliance_validity_days != null) {

		  expDebug::dPrint("$record->compliance_validity_days".print_r($record->compliance_validity_days,true) ,5);
        	$complianceValidityDate[$recId]  = (($record->compliance_validity_days != null) ? (strtotime(date("Y-m-d",strtotime($record->comp_date)) . "+".$record->compliance_validity_days."days")) :  strtotime($record->compliance_validity_date));
        	$complianceExpiredOnValidity[$recId] = (($record->compliance_validity_days != null) ? (strtotime(date("Y-m-d",strtotime($record->reg_date . "+".$record->compliance_validity_days."day"))))  :  strtotime($record->compliance_validity_date));
        	expDebug::dPrint("$complianceExpiredOnValidity[$recId]".print_r($complianceExpiredOnValidity[$recId],true) ,5);

		} else {
			$complianceValidityDate[$recId]  = strtotime($record->compliance_validity_date);
			$complianceExpiredOnValidity[$recId] = strtotime($record->compliance_validity_date);
			expDebug::dPrint("$complianceExpiredOnValidity[$recId]".print_r($complianceExpiredOnValidity[$recId],true) ,5);
		}
		if($strtotimeNow == $complianceValidityDate[$recId]){
				$complianceExpired   = FALSE;
		}
        elseif($strtotimeNow > $complianceValidityDate[$recId]) {
          $complianceExpired   = TRUE;
        }
      }
	  $sid = $record->user_id."-".$record->course_id."-".$record->class_id;
	$hideShare = 1; // don't show the link
	if(getShareModuleStatus('mylearning')){ 
		 $hideShare = 0; //true-> show share link
	}	 
	  $exemptedDet = explode("|",$record->exempted_sts);
   	  list($mro_name, $mro_code) = explode('##',$record->mro);
 	  	$result_response->rows[$i]['id']   = $record->id;
 	  	$launch_detail = ($record->basetype=='WBT' || $record->basetype=='VC' || $record->basetype=='VOD')?getLaunchDetail($record->id,'',$sid):array();
      $result_response->rows[$i]['cell'] = array(
      'id'                         => $record->id,
      'user_id'                    => $record->user_id,
      'master_enrollment_id'       => $record->master_enrollment_id,
      'user_name'                  => $record->user_name,
      'class_id'                   => $record->class_id,
      'course_id'                  => $record->course_id,
      'reg_status'                 => $record->reg_status,
      'reg_status_code'            => $record->reg_status_code,
      'status_code'                => $record->status_code,
      'reg_date'                   => date_format(date_create($record->reg_date),'M d, Y'),
      'reg_status_date'            => date_format(date_create($record->reg_status_date),'M d, Y'),
      'remDays'                    => abs((strtotime(date_format(date_create($record->reg_date),'Y-m-d')) - strtotime(date("Y-m-d"))) / (60 * 60 * 24)),
      'comp_status'     		   => $record->comp_status,
      'update_date'     		   => $record->updated_on,
      'comp_date'				   => date_format(date_create($record->comp_date),'M d, Y'),
      'valid_from'                 => $record->valid_from,
      'valid_to'     			   => $record->valid_to,
      'score'                      => $record->score,
      'quiz_status'				   => ($record->basetype=='WBT' ? getConsolidatedQuizStatus($record->id, null, $launch_detail) : ''),
      'updated_on'                 => date_format(date_create($record->updated_on),'M d, Y'),
      'course_title'               => $record->title,
      'cls_title'                  => sanitize_data($record->cls_title),
      'cls_code'                   => $record->code,
      'cls_status'                 => $record->cls_status,
      'description'     		   => titleController('ANNOUNCEMENT_SHORTTITLE', strip_tags(str_replace(array("\n","\r"), array("<br>","&nbsp;"), trim($record->description))),($theme_key == "expertusoneV2" ? 120 : '')),
      'descriptionfull'			   => /* strip_tags( */str_replace(array("\n","\r","<p>&nbsp;</p>"), array("","&nbsp;",""), trim($record->description))/* ) */,
      'delivery_type'              => $record->delivery_type,
      'delivery_type_code'         => $record->delivery_type_code,
      'basetype'                   => $record->basetype,
      'courseid'                   => $record->courseid,
      'classprice'                 => $record->classprice,
      'orderdatetime'     		   => $record->orderdatetime,
      'session_id'                 => $record->session_id,
      'session_start'              => $record->session_start,
      'session_start_day'		   => ($record->session_start != null ) ? date_format(date_create($record->session_start),'D') : '',
      'session_end'                => '',
      'session_start_format'       => date_format(date_create($record->session_start),'M d, Y'),
      'sessionDetails'			   => ($record->basetype=='ILT' || $record->basetype=='VC')?getSessionDetails($record->course_id,$record->class_id,$record->delivery_type_code,"enroll"):array(), // (isset($ses[$record->id]) ? $ses[$record->id] : array() ), 
       'usertimezonecode'         => $timezone_details['attr1'],
      		'user_tzcode'         => $timezone_details['attr4'],
      'location_id'                => $record->locationid,
      'location_name'              => $record->locationname,
      'location_addr1'             => $record->locationaddr1,
      'location_addr2'             => $record->locationaddr2,
      'location_city'              => $record->locationcity,
      'location_state'             => $record->locationstate,
      'location_zip'               => $record->locationzip,
      'location_phone'             => $record->locationphone,
      'location_country'           => $record->locationcountry,
      'language'             	   => $record->language,
      'node_id'             	   => $record->node_id,
      'surveystatus'               => $record->survey_status,
      'assessmentstatus'           => $record->assessment_status,
      'preassessmentstatus'        => $record->preassessment_status,
      'dedicated_class_flag'	   => $record->dedicated_class_flag,
      //'attemptleft'              => assessmentAttemptLeft($record->id,$record->class_id, $record->user_id,'cre_sys_obt_cls'),
      //'maxscorevalue'              => getMaximumScoreValidation($record->class_id, 'cre_sys_obt_cls'),
      'maxscorevaluepre'           => getMaximumScoreValidation($record->class_id, 'cre_sys_obt_cls','pre'), // maxscore Validation for Pre assesment
      'maxscorevaluepost'          => getMaximumScoreValidation($record->class_id, 'cre_sys_obt_cls','post'),// maxscore Validation for Post assesment
      'mandatory'			       => $record->mandatory,
      'is_compliance'              => $record->is_compliance,
      'compliance_complete_date'   => (($complianceCompleteDate[$recId] != '') ? strftime("%b %d, %Y",$complianceCompleteDate[$recId]) : ''),
      'compliance_complete_days'   => (($complianceCompleteDate[$recId] != '') ? strftime("%b %d, %Y",$complianceCompleteDate[$recId]) : ''),
      'compliance_validity_date'   => (($complianceValidityDate[$recId] != '') ? strftime("%b %d, %Y",$complianceValidityDate[$recId]) : ''),
      'compliance_validity_days'   => (($complianceValidityDate[$recId] != '') ? strftime("%b %d, %Y",$complianceValidityDate[$recId]) : ''),
      'compliance_expired_on_validity' => (($complianceExpiredOnValidity[$recId] !='') ? strftime("%b %d, %Y",$complianceExpiredOnValidity[$recId]) : ''),
      'compliance_expire'		   => $complianceExpired,
      'recertify_compliance'  	   => ($record->is_compliance == 1 && $record->comp_date) ? checkRecertifyorNot($record->course_id,$record->user_id,$record->comp_status,$record->id) : 0,
      'full_name'                  => ($record->created_by)?(($full_name=getPersonDetails($record->created_by, array('full_name')))?$full_name['full_name']:'') :'',
      'managerid'                  => $record->created_by,
      'updated_by'             	   => $record->updated_by,
      'created_by'                 => $record->created_by,
      'updated_by_name'            => ($record->updated_by)?(($updated_by=getPersonDetails($record->updated_by, array('full_name')))?$updated_by['full_name']:'') :'',
      'launch'					   => $launch_detail,
      'show_lnr_attach'	   		   => getAttachmentDetails($record->class_id,'cre_sys_obt_cls',$record->course_id),
      'show_events'			       => '',
      'switch_events'			   => isClassSwitchAvail($record->delivery_type_code,$record->class_id,$record->course_id,$record->user_id,$record->id,'class'),
      'show_price'                 => checkPriceForSwitchClass($record->class_id,$record->course_id),
      'mro'						   => $mro_name,
      'mro_id'					   => $mro_code,
      'assigned_by'				   => $record->assigned_by,
      'labelmsg'                   => array('msg1' => t('LBL025'),'msg2' => t('LBL026'),'msg3' => t('LBL027'),'msg4' => t('Incomplete'),'msg5' => t('LBL125'), 'msg6' => t('No Show')),
      'star_widget'				   => drupal_render(drupal_get_form('fivestar_rating_widget', '', $settings)),
      'waitlist_priority'		   => $record->waitlist_priority,
      'created_by_ins_mngr_slf'	   => $record->created_by ? $updated_by=getUpdatedByDetails($record->user_id, $record->created_by) : t('Me'),
      'updated_by_ins_mngr_slf'	   => $record->updated_by ? $updated_by=getUpdatedByDetails($record->user_id, $record->updated_by) : t('Me'),
      'learning_type_image'		   => getTypeImageClass($record->delivery_type_code),
      'learning_type'		 	   => $record->delivery_type,
      'id_encrypted'               => core_encrypt($record->id),
      'class_id_encrypted'		   => core_encrypt($record->class_id),
      'is_exempted' => $exemptedDet[0],
      'exempted_by' => 	($exemptedDet[0] == 1)? (($exmpetedBy = getPersonDetails($exemptedDet[1], array('full_name'))) ? $exmpetedBy['full_name'] : ''): '',
      'exempted_on' => 	($exemptedDet[0] == 1)? date_format(date_create($exemptedDet[2]),'M d, Y') : '',
      'progress' =>   convertProgressValue(round($record->progress)),
      //'progress' =>   get_progress($launch_detail,$record->id),
 		'hide_share' => $hideShare
      );
      $config=getConfig("exp_sp");
      $mylearn_version 					= $config['mylearn_version'];
      if($mylearn_version==1){
      	$content_details = content_playlist_callback($record->id,1);
      	expDebug::dPrint('CONTEnt details of enroll id'.$record->id.'<--enroll-id-->'.print_r($content_details,1),4);
      	if(($content_details['classDetails']['defaultLaunchContent'] >= 0 && $content_details['classDetails']['defaultLaunchContent']!=='notset') && $content_details['classDetails']['total_content_count']>0){
      		$result_response->rows[$i]['cell']['mylearn_content_launchable']   = true;
      	}else{
      		$result_response->rows[$i]['cell']['mylearn_content_launchable']   = false;
      	}
      	if(isset($content_details['classDetails']['cl_ilt_have_only_post_ass_cont']) && $content_details['classDetails']['cl_ilt_have_only_post_ass_cont']==true)
      		$result_response->rows[$i]['cell']['cl_ilt_have_only_post_ass_cont']   = true;
      	$result_response->rows[$i]['cell']['mylearn_survey_pending_flage']   = $content_details['classDetails']['survey_pending_flag'];
      	$result_response->rows[$i]['cell']['total_content_count']   = $content_details['classDetails']['total_content_count'];
      	$result_response->rows[$i]['cell']['completed_content_count']   = $content_details['classDetails']['completed_content_count'];
      	$result_response->rows[$i]['cell']['content_pending_flag']   = $content_details['classDetails']['content_pending_flag'];
      }
      // Fix for issue #20086
      if (!empty($_GET['enrollmentId']) && $_GET['enrollmentId'] == $record->id && !empty($_GET['lessonId'])) {
      	$hasTriggeringLessonDetails = true;
        expDebug::dPrint('$_GET[enrollmentId] = ' . $_GET['enrollmentId'], 4);
        expDebug::dPrint('$_GET[lessonId] = ' . $_GET['lessonId'], 4);
        $result_response->triggering_enrollment_id = $_GET['enrollmentId'];
        $result_response->triggering_lesson = $_GET['lessonId'];
        foreach ($result_response->rows[$i]['cell']['launch'] as $lessonDetails) {
        	if ($lessonDetails['ID'] == $_GET['lessonId']) {
        	  $result_response->triggering_lesson_details = $lessonDetails;
        	}
        }
      }
      $i++;
  }
  $tTrack['end']['enroll_query_loop'] = (microtime(true) - $tTrack['start']['enroll_query_loop']);
	$tTrack['start']['enroll_query_after_loop'] = microtime(true);
  // Identify last rec in the list sent back to jqgrid
  if ($i > 0) {
  	$result_response->rows[$i-1]['cell']['is_last_rec'] = 'last';
  }
  // Fix for issue #20086
  if (!empty($_GET['enrollmentId']) && !empty($_GET['lessonId']) && !$hasTriggeringLessonDetails) {
    expDebug::dPrint('$_GET[enrollmentId] = ' . $_GET['enrollmentId'], 4);
    expDebug::dPrint('$_GET[lessonId] = ' . $_GET['lessonId'], 4);
  	$result_response->triggering_enrollment_id = $_GET['enrollmentId'];
    $result_response->triggering_lesson = $_GET['lessonId'];
    $launchInfo = getLaunchDetail($_GET['enrollmentId']);
    expDebug::dPrint('count of launch '.count($launchInfo));
    foreach ($launchInfo as $lessonDetails) {
      if ($lessonDetails['ID'] == $_GET['lessonId']) {
        $result_response->triggering_lesson_details = $lessonDetails;
        $content_id = $lessonDetails['ContentId'];
      }
    }
    $result_response->triggering_content_quiz_status = getConsolidatedQuizStatus($_GET['enrollmentId'], $content_id, $launchInfo);
//     getConsolidatedQuizStatus($enrollment_id, $lesson_count = 1, $content_id = 0, $launchInfo = null)
  }
  expDebug::dPrint('$result_response = ' . print_r($result_response, true), 5);
  $tTrack['end']['enroll_query_after_loop'] = (microtime(true) - $tTrack['start']['enroll_query_after_loop']);
 
  $output = drupal_json_encode($result_response);
  $tTrack['end']['enroll_search'] = (microtime(true) - $tTrack['start']['enroll_search']);
  echo $output;
  
  }catch (Exception $ex) {
    watchdog_exception('getEnrollSearchResults', $ex);
    expertusErrorThrow($ex);
  }
}

function getMaximumScoreValidation($prgid,$obj_type,$ass_type=''){
  try{
  $select = db_select('slt_survey_mapping', 'sur_map');
  $select->join('slt_survey', 'sur', 'sur.id = sur_map.survey_id');
  $select->join('slt_survey_groups_questions', 'sur_grp_ques', 'sur.id = sur_grp_ques.survey_id');
  $select->join('slt_survey_groups', 'sur_grp', 'sur_grp.id = sur_grp_ques.survey_group_id');
  $select->addExpression('sur.max_mark', 'max_score_value');
  $select->addExpression('SUM(sur_grp_ques.score)', 'total_score');
  $select->condition('sur_map.object_id', $prgid,'=');
  $select->condition('sur_map.object_type', $obj_type,'=');
  $select->condition('sur.type','sry_det_typ_ass', '=');
  $select->condition('sur_grp.status','sry_det_grp_atv', '=');
  if($ass_type == 'pre'){
  	$select->condition('sur_map.pre_status', '1','=');
  }else if ($ass_type == 'post'){
  	$select->condition('sur_map.pre_status', '0','=');
  }
  $select->groupBy('sur_map.id');
  expDebug::dPrintDBAPI('getMaximumScoreValidation', $select);
  $result = $select->execute();

  if($result->rowCount()>0){
    $max_score_results = $result->fetchAll();
    expDebug::dPrint('$max_score_results : '.print_r($max_score_results,true), 4);
    foreach($max_score_results as $value){
        if($value->max_score_value == $value->total_score)
            return 1;
    }
  }
  return 0;
  }catch (Exception $ex) {
    watchdog_exception('getMaximumScoreValidation', $ex);
    expertusErrorThrow($ex);
  }
}
function getEnrollStatus($userid){
  try{
  $select = db_select('slv_myenrollment', 'myenroll');
  $select->addExpression('DISTINCT(myenroll.reg_status_code)', 'reg_status');
  if(!empty($userid)){
     $select->condition('myenroll.user_id', $userid,'=');
  }
  $regQryRes = $select->execute()->fetchAll();
  echo drupal_json_output($regQryRes);
  }catch (Exception $ex) {
    watchdog_exception('getEnrollStatus', $ex);
    expertusErrorThrow($ex);
  }
}

function getDropEnrollment($userId,$baseType='',$enrollId,$refundFlag,$isCommerceEnabled,$assMand,$fromMyteam=0, $fromInstructor=0){
  try{
  	$sltPersonUserId = getSltpersonUserId();
  	if($sltPersonUserId != $userId){
  		$userAccess = 0;
  		$userAccess = is_manager($sltPersonUserId)?1:is_instructor($sltPersonUserId)?1:0;
  		if($userAccess == 0){
  			throw new Exception('Userid and Logged in user doesnot match.');
  		}
  	}

    $tmp=getDropEnrollmentWrapper($userId,$baseType,$enrollId,$refundFlag,$isCommerceEnabled,$assMand,$fromMyteam,$fromInstructor);
    $statusMsg = object2array($tmp[0]);
    echo "var drop_policy_result=".drupal_json_encode($statusMsg);
    }catch (Exception $ex) {
      watchdog_exception('getDropEnrollment', $ex);
      expertusErrorThrow($ex);
    }
}


 function getDropEnrollmentWrapper($userId,$baseType='',$enrollId,$refundFlag,$isCommerceEnabled,$assMand,$fromMyteam=0, $fromInstructor=0){
  try{

	expDebug::dPrint("user:".$userId ."enroll id:".$enrollId." refund flag::".$refundFlag." isCommerce.".$isCommerceEnabled." assMand=".$assMand." frommytem=".$frommyteam." fromInstructor=".$fromInstructor , 4);

 // include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc');
	//includes/module.inc
	//module_load_all(TRUE);
	// check if status is inprogress before cancelling
	$regQry  = db_select('slt_enrollment', 'enr');
	$regQry->addField('enr','reg_status');
	$regQry->condition('enr.id', $enrollId, '=');
	$regQry->condition('enr.reg_status', 'lrn_crs_reg_can', '!=');
	
	$comp_sts = array(
		'lrn_crs_cmp_cmp',
		'lrn_crs_cmp_inc',
		'lrn_crs_cmp_nsw',
		'lrn_crs_cmp_inp'
	);

	$regQry->condition(db_or()->condition('enr.comp_status', '', 'IS NULL')
							  ->condition('enr.comp_status', $comp_sts, 'NOT IN'));
	
	expDebug::dPrintDBAPI('cancel class check ', $regQry, 1);
	//$regQry->condition('enr.master_enrollment_id');
	$totalRecords = $regQry->countQuery()->execute()->fetchField();

	if($totalRecords < 1) {	// the enrollment is lrn_crs_cmp_cmp or lrn_crs_cmp_inc or lrn_crs_cmp_nsw or lrn_crs_cmp_inp
		$tmp->status = "failure";
		$tmp->msg = t("MSG748");
		$tmparr = array();
		$tmparr[0] = $tmp;
		return $tmparr;
	}
	
	$cancelEnr=true;
	$tmp=new stdClass();
	$isRefunded = 0;
	$refund_amount = 0;
	$ucOrderId = '';
	$gateway = variable_get('uc_payment_credit_gateway','');

	if($refundFlag=="true")
	{
		$arr=getDropPolicyDetailsLearner($userId,$baseType,$enrollId,"",$isCommerceEnabled,$assMand);
		$refund_amount = $arr["refund_amt"];
		$isRefunded = 1;
		$ucOrderId = $arr["uc_order_id"];
		if($arr["next_action"]=="showdroppolicy")
		{
		    if($arr["refund_amt"]==0.0 ||$arr["refund_amt"]==0 || $gateway == 'exp_paymetric')
		    {
		        $cancelEnr=true;
		    }
		    else
		    {
				$currencyCode = (!empty($arr["currency_code"])) ? $arr["currency_code"] : 'USD';
    			$reply=refund_cc($arr["slt_order_id"],$arr["uc_order_id"],$arr["refund_amt"],"",$currencyCode);
    			if($reply->serviceresponse=="success")
    			{
    				$cancelEnr=true;
    				}
    			else
    			{
    				$cancelEnr=false;
    				}
		    }
		}
		expDebug::dPrint("next action::".$arr["next_action"] , 4);

	}
	
	if($arr["next_action"] == "showmsg" && isset($_REQUEST['apiname']) && $_REQUEST['apiname'] == 'cancelPricedClassAPI' ){
	
		$tmp->status="failure";
		$tmp->msg=$arr["msg"];
		$tmparr=array();
		$tmparr[0]=$tmp;
		return $tmparr;
	}
	

	$arr=array();
	if($cancelEnr==true){
		$updateUserId = getSltpersonUserId();
		$param =  array();
		$param['enroll_id']		   = $enrollId;
		$param['is_refunded']      = $isRefunded;
		$param['refund_amount']    = $refund_amount;
		$param['user_id']          = $userId;
		$param['update_user_id']   = $updateUserId;
		$param['cancel_from']      = 'learner';
		$param['uc_order_id']	   = $ucOrderId;
		$param['from_instructor']  = $fromInstructor;
		$param['from_manager']     = $fromMyteam;
		$param['base_type']        = strtoupper($baseType);
		dropClassLineItem($param);
		$tmp->status="success";
		$tmp->msg="";
	}
	else
	{
	  $tmp->status="failure";
	  $tmp->msg=t("MSG270");
	}
	$tmparr=array();
	$tmparr[0]=$tmp;
	return $tmparr;
	}catch (Exception $ex) {
      watchdog_exception('getDropEnrollmentWrapper', $ex);
      expertusErrorThrow($ex);
    }
}

function getAttachmentDetails($entityId,$entityType,$courseid='') {
  try{
  $attachment = db_select('slt_entity_reading_mapper', 'mapper');
  $attachment->addField('mapper','id', 'attachment_id');
  $attachment->addField('mapper','reading_id', 'reading_sequence');
  $attachment->addField('mapper','entity_id', 'entity_id');
  $attachment->addField('mapper','reading_type','reading_title');
  $attachment->addField('mapper','reading_content','reading_content');
  if($entityType == 'cre_sys_obt_cls'){// added for this ticket #0041140 : To Show Course attachment with class attachment in my learning
  	$conjuction_or = db_or(); // Concated the Two Conditions
  	$attachment_condition_and = db_and(); // Giving the Class Type and Class id
  	$attachment_condition_and->condition('mapper.entity_id',$entityId);
  	$attachment_condition_and->condition('mapper.entity_type',$entityType);
  	$conjuction_or->condition($attachment_condition_and); // Concated it Under OR Operator

  	$attachmentType_condition_and = db_and(); // Giving the Course Type and Course id
  	$attachmentType_condition_and->condition('mapper.entity_id',$courseid);
  	$attachmentType_condition_and->condition('mapper.entity_type','cre_sys_obt_crs');
  	$conjuction_or->condition($attachmentType_condition_and);// Concated it Under OR Operator

  	$attachment->condition($conjuction_or);
  }else{ // For Tp or other Details
  	$attachment->condition('mapper.entity_id',$entityId);
  	$attachment->condition('mapper.entity_type',$entityType);
  }
  $attachment->condition('mapper.showlnr','Y');
  expDebug::dPrintDBAPI('$attachment', $attachment);
  $result = $attachment->execute()->fetchAll();
  return $result;
  }catch (Exception $ex) {
    watchdog_exception('getAttachmentDetails', $ex);
    expertusErrorThrow($ex);
  }
}

//Implements stored procedure slp_registered_class_detail_sel
function getClassOption($regId) {
  try{
	  $select = db_select('slt_enrollment', 'enr');
	  $select -> join('slt_course_template', 'course', 'course.id = enr.course_id');
	  $select -> join('slt_course_class', 'class', 'course.id = class.course_id AND class.id <> enr.class_id');
	  $select -> join('slt_profile_list_items', 'profile_lang', 'profile_lang.code = class.lang_code');
	  $select -> join('slt_profile_list_items', 'profile_base', 'profile_base.code = class.delivery_type');
	  $select -> join('slt_profile_list_items', 'profile_delivery', 'profile_delivery.code = class.delivery_type');
	  $select -> leftJoin('slt_course_class_session', 'ses', 'ses.class_id=class.id AND ses.course_id=class.course_id');
	  $select -> leftJoin('slt_profile_list_items', 'master_view', 'class.delivery_type = master_view.code');
	  slf_get_node_id($select, 'class', 'cre_sys_obt_cls');
	  $select -> addField('class', 'course_id', 'CourseId');
	  $select -> addField('class', 'id', 'ClassId');
	  $select -> addField('class', 'CODE', 'Code');
	  $select -> addField('class', 'title', 'Name');
	  $select -> addField('class', 'description', 'Description');
	  $select -> addField('class', 'delivery_type', 'DeliveryTypeCode');
	  $select -> addField('class', 'price', 'Price');
	  $select -> addField('class', 'scheduled_duration', 'Duration');
	  $select -> addField('class', 'max_seats', 'MaxCapacity');
	  $select -> addField('class', 'waitlist_count', 'AvailablibleSeatCount');
	  $select -> addField('class', 'valid_to', 'EnrollmentValidity');
	  $select -> addField('ses', 'start_date', 'StartDate');
	  $select -> addField('ses', 'end_date', 'EndDate');
	  $select -> addField('ses', 'timezone', 'TimeZone');
	  $select -> addField('profile_lang', 'name', 'Lang');
	  $select -> addField('profile_base', 'attr1', 'BaseType');
	  $select -> addField('profile_delivery', 'name', 'DeliveryType');
	  $select -> addExpression('DATE_FORMAT(ses.start_date, \'%b %d, %Y\')', 'StartDateFormat');
	  $select -> addExpression('DATE_FORMAT(ses.start_date, \'%h:%i %p\')', 'StartTime');
	  $select -> addExpression('DATE_FORMAT(ses.end_date, \'%h:%i %p\')', 'EndTime');
	  $select -> condition('enr.id', $regId, '=');
	  $select -> condition('class.status','lrn_cls_sts_atv','=');
	  $select -> condition('course.status','lrn_crs_sts_atv','=');
	  $select -> condition('enr.class_id', 'class.id', '<>');
	  $select -> where('IF((master_view.attr1=\'ILT\' OR master_view.attr1=\'VC\'), IF(class.id <> enr.class_id,ses.start_date >= NOW(),1=1), 1=1)');
	  $result = $select->execute()->fetchAll();
  	echo drupal_json_output($result);
  }catch (Exception $ex) {
    watchdog_exception('getClassOption', $ex);
    expertusErrorThrow($ex);
  }
}

function slf_get_node_id(&$select, $join_table_alias, $xtype) {
  try{
  $select -> leftJoin('slt_node_learning_activity', 'nla', 'nla.entity_id = '.$join_table_alias.".id and nla.entity_id = '".$xtype."'");
  $select -> addField('nla', 'node_id', 'NodeId');
  }catch (Exception $ex) {
    watchdog_exception('slf_get_node_id', $ex);
    expertusErrorThrow($ex);
  }
}

function getSwitchEnrollment($classId,$enrollId){
  $txn = db_transaction();
  try{
  $userId = getSltpersonUserId();
  $updateStmt = db_update('slt_enrollment');
  // Set conditions
  $updateStmt->condition('id', $enrollId);
  $updateStmt->condition('user_id', $userId);

  // Set new values of fields
  $updateStmt->fields(array(
                        'class_id'      => $classId,
                        'comp_status'   => 'lrn_crs_cmp_enr',
                        'updated_on'    => now(),
                        'updated_by'    => $userId
                        ));

   expDebug::dPrintDBAPI(' $updateStmt = ' , $updateStmt);
    // Execute query
   $updateStmt->execute();
   }catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('getSwitchEnrollment', $ex);
    expertusErrorThrow($ex);
  }
  // Commit the transaction
  unset($txn);

}

function getDropPolicy($userId,$baseType='',$enrollId,$classId,$isCommerce,$assMand,$mandByRole){
  try{
  	$sltPersonUserId = getSltpersonUserId();
  	// Added for #0032681 and #0032859
  	if($sltPersonUserId != $userId){
  		$userAccess = 0;
  		$userAccess = is_manager($sltPersonUserId)?1:is_instructor($sltPersonUserId)?1:0;
  		if($userAccess == 0){
  			throw new Exception('Userid and Logged in user doesnot match.');
  		}
  	}
	$arr=getDropPolicyDetailsLearner($userId,$baseType,$enrollId,$classId,$isCommerce,$assMand,$mandByRole);
	echo "var drop_policy_info=".drupal_json_encode($arr);
	}catch (Exception $ex) {
    watchdog_exception('getDropPolicy', $ex);
    expertusErrorThrow($ex);
  }
}

function cancelPricedClassAPI($user_id,$learnerId,$baseType,$enrollId,$classId,$isCommerce,$assMand){
  try{
 drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  $result = getDropEnrollmentWrapper($learnerId,$baseType,$enrollId,"true",$isCommerce,$assMand);

  expDebug::dPrint('doing cancel = ' . print_r($result , true), 5);
  return $result[0];
  }catch (Exception $ex) {
		watchdog_exception('cancelPricedClassAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function getDropPolicyDetailsLearner($userId,$baseType='',$enrollId,$classId,$isCommerce,$assMand,$mandByRole){
  try{
	expDebug::dPrint("drop policy details. input userid=".$userId." base type=".$baseType." classId=".$classId." enroll id=".$enrollId." iscommerce.=".$isCommerce." assMand=".$assMand , 4);
	expDebug::dPrint("$mandByRole =".$mandByRole , 4);
	$next_action="";
	$msg="";
	$unit_price=0.0;
	$currency_type="";
	$refund_percentage=0;
	$refund_amt=0;
	$start_date="";
	$uc_order_id="";
	$delivery_type="";
	$slt_order_id="";
	$payment_gateway="";

	$result=getDropPolicyRecord($enrollId,$baseType);
	expDebug::dPrint("===== drop policy data ===");
	expDebug::dPrint($result , 4);
	if(count($result) == 0){
		$next_action="showmsg";
		$msg= t('MSG748');
		return $arr=array("next_action"=>$next_action,"msg"=>$msg);
	}
	foreach ($result as $row)
	{
		$unit_price=$row->price;
		$currency_type=$row->currency_type;
		$classId=$row->class_id;
		$delivery_type=$row->delivery_type;
		$currency_sym = $row->currency_type;
	}
    if($baseType == 'tp'){
      $sltOrderId = db_query("SELECT order_id FROM slt_enrollment  where master_enrollment_id =:mid limit 1", array(':mid' => $enrollId))->fetchField();
      $sltOrderRow = db_query("SELECT overall_status FROM slt_master_enrollment WHERE id=:mid limit 1", array(':mid' => $enrollId))->fetchAssoc();
      $waitlistItem = $sltOrderRow['overall_status'];
    }
    else{
      $sltOrderRow = db_query("SELECT order_id,waitlist_flag FROM slt_enrollment WHERE id=:eid limit 1", array(':eid' => $enrollId))->fetchAssoc();
      $sltOrderId   = $sltOrderRow['order_id'];
      $waitlistItem = $sltOrderRow['waitlist_flag'];

    }

	if(($isCommerce=="" || $isCommerce=="0") && ( $assMand=="" || $assMand=="0") && ( $mandByRole=="" || $mandByRole=="0"))
		$next_action="drop";
	else if ($assMand=="1" || $mandByRole=="1")
	{
		$next_action="showmsg";
		$msg=t("MSG269");
	}
	else if($isCommerce=="1")
	{
		$payment_gateway = variable_get('uc_payment_credit_gateway','');
		$pay_type=getPaymentMethodType($enrollId,$sltOrderId);
		
		if(($pay_type->payment_method=="credit" && $payment_gateway != 'exp_paymetric') || $pay_type->payment_method=="Zero Cost")
		{    
			$isDiscounted = db_query("SELECT count(1) FROM uc_order_line_items where order_id =:oid and type =:typ and amount < 0", array(':oid' => $pay_type->uc_order_id, ':typ'=>'uc_discounts'))->fetchField();
		    if($isDiscounted){
		      $uc_order_id = $pay_type->uc_order_id;
		      $drupalUid   = $pay_type->uid;
		      $selectNode = db_select('slt_node_learning_activity','nde');
	        $selectNode->addField('nde','node_id');
          $selectNode->condition('nde.entity_id', $classId);
		      if(strtolower($baseType)=="tp"){
		        $selectNode->condition('nde.entity_type', array('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn'),'IN');
	        }
	        else{
            $selectNode->condition('nde.entity_type', 'cre_sys_obt_cls');
	        }
	        $nodeId = $selectNode->execute()->fetchField();
	        $discountAmt = 0;
	        expDebug::dPrint("getDropPolicyDetailsLearner--> drop class nodeid=".$nodeId , 3);
	        $discountAmt = getDropListDiscountAmount($nodeId,$uc_order_id);
            expDebug::dPrint("getDropPolicyDetailsLearner--> drop class discount amount=".$discountAmt , 3);
            $unit_price = getDropListUnitPrice($nodeId,$uc_order_id);
	        $unit_price = $unit_price - $discountAmt;
	        if($pay_type->payment_method=="Zero Cost")
	        	$unit_price = ($unit_price < 0) ? 0 : $unit_price;
	        expDebug::dPrint("getDropPolicyDetailsLearner--> drop class discounted unit price=".$unit_price , 3);
		      $next_action="showdroppolicy";
		    }
		    else{
			$next_action="showdroppolicy";
			$uc_order_id=$pay_type->uc_order_id;
		    }
		}
			else{
			$next_action="showmsg";
			$msg=t("MSG270");
	}

		$taxForOrderItem = getChargedTaxForOrderItem($sltOrderId, $classId, $baseType);
		$refund_amt = getRefundAmount($classId, $baseType, $unit_price, $waitlistItem, $taxForOrderItem);
	}
	
	if(($unit_price=="" || $unit_price=="0" ) && $assMand==0 && $mandByRole==0 && $pay_type->payment_method!="Zero Cost")
	{
		$next_action="drop";
	}
	if($result[0]->reg_status == "lrn_crs_reg_can"){
		$next_action="showmsg";
		$msg=t("MSG609");
	}
	$arr=array("next_action"=>$next_action,"msg"=>$msg,"price"=>$unit_price,"currency_type"=>$currency_sym,"refund_amt"=>$refund_amt['refund_value'],"deducted_amount"=>$refund_amt['deducted_amount'],"uc_order_id"=>$uc_order_id,"slt_order_id"=>$sltOrderId,"currency_code"=>$currency_type,"payment_gateway"=>$payment_gateway);
	expDebug::dPrint("drop policy details.");
	expDebug::dPrint(drupal_json_encode($arr) , 4);
	return $arr;
  }catch (Exception $ex) {
    watchdog_exception('getDropPolicyDetailsLearner', $ex);
    expertusErrorThrow($ex);
  }
}

function getDropPolicyRecord($enrollId,$baseType){
  try{
	 $select = "";
	 if(strtolower($baseType)=="tp")
	 {
	   $select = db_select('slt_master_enrollment', 'mas');
	 	 $select->leftJoin('slt_program', 'prg',"prg.id=mas.program_id");
	 	 $select->leftJoin('slt_order', 'ord',"ord.id = mas.order_id AND mas.id = ".$enrollId);
	 	 $select->leftJoin('slt_order_items', 'oitems',"oitems.order_id=ord.id AND oitems.program_id = prg.id AND oitems.program_id IS NOT NULL");
	 	 $select->addField('mas','overall_status', 'reg_status');
	   $select->addField('mas','program_id', 'class_id');
	   $select->addField('oitems','item_price', 'price');
	   $select->addField('ord','currency_type', 'currency_type');
	   $select->addField('prg','type', 'delivery_type');
	   $select->condition('mas.id', $enrollId);
	   $select->condition('mas.overall_status',array('lrn_tpm_ovr_cln','lrn_tpm_ovr_cmp','lrn_tpm_ovr_inc'),'NOT IN');
	  $select->groupBy('oitems.program_id');
	 }
	 else
	 {
		 	$comp_sts = array(
				'lrn_crs_cmp_cmp',
				'lrn_crs_cmp_inc',
				'lrn_crs_cmp_nsw',
		 		'lrn_crs_cmp_inp'
			);
	 	 $select = db_select('slt_enrollment', 'sr');
	 	 $select->leftJoin('slt_course_class', 'cl',"cl.id=sr.class_id AND sr.master_enrollment_id IS NULL");
	 	 $select->leftJoin('slt_order', 'ord',"ord.id = sr.order_id AND sr.id = ".$enrollId);
	 	 $select->leftJoin('slt_order_items', 'oitems',"oitems.order_id=ord.id AND oitems.class_id = cl.id AND oitems.program_id IS NULL");
	 	 $select->addField('sr','reg_status', 'reg_status');
	   $select->addField('sr','class_id', 'class_id');
	   $select->addField('oitems','item_price', 'price');
	   $select->addField('ord','currency_type', 'currency_type');
	   $select->addField('cl','delivery_type', 'delivery_type');
	   $select->condition('sr.id', $enrollId);
// 	   $select->condition('comp_status', $comp_sts,'NOT IN');
	   $select->condition(db_or()->condition('sr.comp_status', '', 'IS NULL')
   								 ->condition('sr.comp_status', $comp_sts, 'NOT IN'));
  	   $select->condition('reg_status', 'lrn_crs_reg_can','!=');
	   $select->condition('sr.master_enrollment_id', '','IS NULL');
	 }
	 expDebug::dPrintDBAPI("Query for getDropPolicyRecord", $select);
 	 $result =  $select->execute()->fetchAll();
 	 return $result;
   }catch (Exception $ex) {
    watchdog_exception('getDropPolicyRecord', $ex);
    expertusErrorThrow($ex);
  }
}

function getPaymentMethodType($enrollId,$sltOrderId){
  try{
	$paymenttype=new stdClass();
	$sqlqry="SELECT payment_method,order_id,uid FROM uc_orders WHERE order_id = (SELECT uc_order_id FROM slt_order where id = $sltOrderId)";
	$result=db_query($sqlqry);
	foreach ($result as $row)
	{
		$paymenttype->payment_method=$row->payment_method;
		$paymenttype->uc_order_id=$row->order_id;
		$paymenttype->uid=$row->uid;
	}

	expDebug::dPrint("payment type ..sqlqry." . $sqlqry, 2);
	expDebug::dPrint($paymenttype , 4);
	return $paymenttype;
  }catch (Exception $ex) {
    watchdog_exception('getPaymentMethodType', $ex);
    expertusErrorThrow($ex);
  }
}

function getDropEnrollmentWrapperByRest($userId,$enrollId,$refundFlag="false",$isCommerceEnabled="0",$assMand="0",$fromMyteam=0){
  try{
    drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
    /**	Change made by: ayyappans
     * 	34740: Related to ical Features while cancelling the class through API notification not triggering
     * 	Root Cause: second argument of the getDropEnrollmentWrapper() method call $baseType which refers to the delivery/class type of the enrollment doesnt have a value as expected.
     * 	Fix: delivery/class type is fetched from the database by enrollId and passed to the getDropEnrollmentWrapper()
     * */
  if(isset($_REQUEST['mobile_request'])){
			$enrollIds = explode(",",$enrollId);
			expDebug::dPrint("dededededededed");
			$resultArr = array();
			for($i=0;$i<count($enrollIds);$i++){
				$baseType = getClassDeliverTypeByEnrollId($enrollIds[$i]);
				$resultArr[$i]= getDropEnrollmentWrapper($userId,$baseType,$enrollIds[$i],$refundFlag,$isCommerceEnabled,$assMand,$fromMyteam);
			}
			return $resultArr[0];
		}else{
			$baseType = getClassDeliverTypeByEnrollId($enrollId);
			
			$isPriced =0;
			$select = db_select('system', 'sys');
			$select->addField('sys', 'status', 'status');
			$select->condition('name', 'exp_sp_commerce');
			$commerceEnabled =  $select->execute()->fetchField();
			expDebug::dPrint("dropTPclassunderManagerAPI $commerceEnabled::".print_r($commerceEnabled,TRUE) , 4);
			if($commerceEnabled == 1){
			    $enrollmentDetails = getEnrollmentDetails($enrollId, array('class_id'));
			    $isPriced = getClassPrice($enrollmentDetails->class_id);
			}
			if ($isPriced > 0) {
			    $errobj=new stdClass();
			    $errobj->isValidateError = 1;
			    $errobj->errcode = 'L_012';
			    $errobj->errormsg = "Priced training cannot be cancelled. Contact support team.";
			    //return array((object)array('status'=>'Priced training cannot be registered. Contact support team.'));
			    return $errobj;
			}
			
			return getDropEnrollmentWrapper($userId,$baseType,$enrollId,$refundFlag,$isCommerceEnabled,$assMand,$fromMyteam);
		}
	}catch (Exception $ex) {
		watchdog_exception('getDropEnrollmentWrapperByRest', $ex);
		expertusErrorThrow($ex);
	}
}

function getmandatory($user_id,$type){
	try{
		$select = db_select('slt_group_mapping', 'mapping');
		if($type == 'cre_sys_obt_cls'){
			$select ->leftJoin('slt_enrollment', 'enroll',"enroll.class_id = mapping.entity_id");
		}else{
			$select ->leftJoin('slt_master_enrollment', 'enroll',"enroll.program_id = mapping.entity_id");
		}
		$select->addExpression('COUNT(1)','countMandatory');
		$select->condition('enroll.user_id', $user_id,'=');
		$select->condition('mapping.mro', 'cre_sys_inv_man','=');
		if($type == 'cre_sys_obt_cls'){
			$select->condition('mapping.entity_type', $type,'=');
		}else{
			$select->condition('mapping.entity_type',array('cre_sys_obt_trn','cre_sys_obt_cur','cre_sys_obt_crt'),'IN');
		}
		// For user access
		$select1 = db_select('slt_user_access_mapping', 'mapping');
		if($type == 'cre_sys_obt_cls'){
			$select1 ->leftJoin('slt_enrollment', 'enroll',"enroll.class_id = mapping.entity_id");
		}else{
			$select1 ->leftJoin('slt_master_enrollment', 'enroll',"enroll.program_id = mapping.entity_id");
		}
		$select1->addExpression('COUNT(1)','countMandatory');
		$select1->condition('enroll.user_id', $user_id,'=');
		$select1->condition('mapping.mro', 'cre_sys_inv_man','=');
		if($type == 'cre_sys_obt_cls'){
			$select1->condition('mapping.entity_type', $type,'=');
		}else{
			$select1->condition('mapping.entity_type',array('cre_sys_obt_trn','cre_sys_obt_cur','cre_sys_obt_crt'),'IN');
		}
		$select->union($select1,'UNION ALL');
		
		$select2 = db_select($select,'man');
		$select2->addExpression('SUM(man.countMandatory)','countMandatory');
		$result =  $select2->execute()->fetchField();

		expDebug::dPrintDBAPI("getmandatory ", $select2);
		expDebug::dPrint("getmandatory result ".print_r($result,true),4);
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getmandatory', $ex);
		expertusErrorThrow($ex);
	}
}

function getLPmandatory($user_id,$type){
	try{
		if($type == 'cre_sys_obt_cls'){
			$select = db_select('slt_enrollment', 'menroll');
			$select ->leftJoin('slt_course_class', 'prm',"prm.id = menroll.class_id");
		}else{
		$select = db_select('slt_master_enrollment', 'menroll');
		$select ->leftJoin('slt_program', 'prm',"prm.id = menroll.program_id");
		}
		$select->addExpression('COUNT(1)','countMandatory');
		$select->condition('menroll.user_id', $user_id,'=');
		$select->condition('menroll.mandatory', 'Y','=');
		$result =  $select->execute()->fetchField();
		expDebug::dPrintDBAPI("LPgetmandatory ", $select);
		expDebug::dPrint("getLPmandatory result ".print_r($result,true),4);
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getLPmandatory', $ex);
		expertusErrorThrow($ex);
	}
}
function isCompliance($user_id){
	$select = db_select('slt_enrollment', 'enroll');
	$select->addExpression('COUNT(1)','countCompliance');
	$select->condition('enroll.user_id', $user_id,'=');
	$select->condition('enroll.is_compliance', 1,'=');
	expDebug::dPrintDBAPI("isCompliance Query ", $select);
	$result =  $select->execute()->fetchField();
	return $result;
}

/*
 * get active class count for compliance courses
*
*/
function single_or_multipleclass($userId,$classId,$courseId,$enrollId){
	try{
		$selectCls = db_select('slt_course_class', 'cls');
		$selectCls->leftjoin('slt_course_class_session', 'ses','ses.class_id=cls.id AND ses.course_id=cls.course_id');
		$selectCls->leftjoin('slt_course_content_mapper', 'content','content.class_id=cls.id AND content.course_id=cls.course_id');
		$selectCls->leftjoin('slt_profile_list_items', 'master_view','master_view.code=cls.delivery_type');
		$selectCls->addField('cls', 'id');
		$selectCls->addField('cls', 'price');
		//$selectCls->condition('cls.id',$classId,'!=');
		$selectCls->condition('cls.course_id',$courseId,'=');
		$selectCls->where('cls.status = \'lrn_cls_sts_atv\'
				AND IF(master_view.attr1=\'ILT\' OR master_view.attr1=\'VC\', (SELECT COUNT(cs.id) FROM slt_course_class_session cs WHERE cs.class_id = ses.class_id AND cs.id=(SELECT id FROM  slt_course_class_session WHERE class_id=cs.class_id ORDER BY start_date LIMIT 1) AND CONCAT(DATE(cs.start_date),\' \',CONCAT(cs.start_time,\':00\')) <  IFNULL(CONVERT_TZ(now(), \''.$defaultTimezone.'\' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone)),NOW()) = 0), 1=1)
				AND (IF(master_view.attr1=\'WBT\' OR master_view.attr1=\'VOD\', content.id IS NOT NULL AND cls.status=\'lrn_cls_sts_atv\', 1=1))');
		$selectCls->where('cls.registration_end_on >= now() or cls.registration_end_on is null');
		filterByUserAccessPrivileges($selectCls, 'cls','','','','Class');
		expDebug::dPrintDBAPI('enroll status query:', $selectCls);
		$result = $selectCls->execute()->fetchAll();
		expDebug::dPrint('multiple class-->'.print_r($result,true),4);
		$cntRes = count($result);
		 if($result[0]->price == '0.00'){
			$result[0]->price = 0;
		}
		if($cntRes>1){
			$msgType = "MultiRegister";
			print $msgType;
		}else if($cntRes == 1 && empty($result[0]->price)){
			expDebug::dPrint('$result : '.$result[0]->id);
			complianceExpiryRegistration($userId, $result[0]->id, $enrollId);
		}else if($cntRes == 1 && !empty($result[0]->price)){
			//$msgType = 'Only priced class is available.Register from Catalog page.';
			$msgType = 'pricedclass';
			print $msgType;
		}else{
			$ClassDelType = getClassDeliveryType($classId, 'Short');
			if ($ClassDelType == 'wbt' || $ClassDelType == 'vod') {
				complianceExpiryRegistration($userId, $classId, $enrollId);
			}else{
				$msgType = 'noclasses';
				print $msgType;
			}
		}
	}catch (Exception $ex) {
		watchdog_exception('single_or_multipleclass', $ex);
		expertusErrorThrow($ex);
	}

}

/*
* function for compliance re-gister after it gets expires
* $userId - slt_person User id.
* $classId - new registered class Id.
* $enrollId - expired class enrollId.
*
* */
function complianceExpiryRegistration($userId, $classId, $enrollId){
	try{
		expDebug::dPrint(' $userId '. $userId .' $classId '.$classId. ' $enrollId '.$enrollId ,4);
		if($enrollId && $classId){
			$loggedUser = getSltpersonUserId();
			$select = db_select('slt_enrollment','enr');
			$select->addField('enr', 'reg_status');
			$select->addField('enr', 'comp_status');
			$select->addField('enr', 'class_id');
			$select->addField('enr', 'course_id');
			$select->condition('enr.id', $enrollId);
			expDebug::dPrintDBAPI('enroll status query:', $select);
			$result = $select->execute()->fetchAll();
			expDebug::dPrint('enroll status query:' . print_r($result,true),3);
			$classRegStatus  = $result[0]->reg_status;
			$clsCompStatus   = $result[0]->comp_status;
			$prevClassId     = $result[0]->class_id;
			$courseId        = $result[0]->course_id;

			$regStatus = 'lrn_crs_reg_cnf';
			$errObj = array();
			$updStatus = '';
			$currentClassDelType = getClassDeliveryType($classId, 'Short');
			if ($currentClassDelType == 'ilt' || $currentClassDelType == 'vc') {
				// Confirm that the user not assigned this class to instruct for learners
				$conflictInstructor = getClassInstructorConflict($userId, $classId);
				if ($conflictInstructor == false ) {
					$errObj[] = t('ERR075');
				}
				else{
					$waitlistNumber = waitListCheck($courseId, $classId);
					if($waitlistNumber == 0 || $waitlistNumber == -1) {
						$regStatus = 'lrn_crs_reg_cnf';
						$completionStatus = 'lrn_crs_cmp_enr';
					}
					else { // waitlist - do further check against waitlist capacity, otherwise class is full.
						$classWaitlistCapacity = getClassWaitlistCapacity(null, $classId);
						expDebug::dPrint(' $classWaitlistCapacity = ' . print_r($classWaitlistCapacity, true) , 4);
						$classWaitlistCapacity = (is_null($classWaitlistCapacity))? 0 : $classWaitlistCapacity;
						if ($classWaitlistCapacity >= $waitlistNumber) {
							$regStatus = 'lrn_crs_reg_wtl';
						}
						else {
							$errObj[] = t('ERR022');
						}
					}
				}
			}
			expDebug::dPrint(' class enroll error = ' .print_r($errObj,true), 4);
			if(!count($errObj)){
				expDebug::dPrint(' class enroll updation start = ' , 4);
				$waitlist_flag = null;
				if($waitlistNumber > 0) {
					$waitlist_flag = 'lrn_crs_reg_wtl';
				}
				$completionStatus = ($regStatus == 'lrn_crs_reg_cnf') ? 'lrn_crs_cmp_enr' : null;
				$aResp = CreateNewOrder($userId, $regStatus, $userId);
				$vOrderId = $aResp['OrderId'];
				// Update the compliance expiry column when registering from completed and expiry tab.
				$updateStmt = db_update('slt_enrollment');
				$updateStmt->fields(array(
					'cmpl_expired' => 1,
					'updated_by'   => $loggedUser,
					'updated_on'   => now(),
				));
				$updateStmt->condition('id',$enrollId);
				$newEnrollmentId = $updateStmt->execute();

				// Insert Class for compliance re-register
				$insertStmt = db_insert('slt_enrollment');
				$insertStmt->fields(array(
					'user_id'			=> $userId,
					'order_id' 			=> $vOrderId,
					'class_id' 			=> $classId,
					'course_id' 		=> $courseId,
					'reg_status'		=> $regStatus,
					'reg_date'		 	=> now(),
					'reg_status_date'   => now(),
					'comp_status' 		=> $completionStatus,
					'waitlist_priority' => $waitlistNumber,
					'waitlist_flag'     => $waitlist_flag,
					'created_by' 		=> $loggedUser,
					'created_on' 		=> now(),
					'updated_by'		=> $loggedUser,
					'updated_on' 		=> now(),
					'mandatory' 		=> null,
					'is_compliance'		=> 1
				));
				$newEnrollmentId = $insertStmt->execute();
				if ($currentClassDelType == 'wbt' || $currentClassDelType == 'vod') {
					$contents = getContentAndVersionForClass($classId);
					foreach ($contents as $content){
						expDebug::dPrint('content version id for compliance  '.$content->content_id.' --- '.$content->version_id);
						$enrollContentStmt = db_insert('slt_enrollment_content_mapping');
						$enrollContentStmt->fields(array(
								'enroll_id'  => $newEnrollmentId,
								'user_id'    => $userId,
								'course_id'  => $courseId,
								'class_id'   => $classId,
								'content_id' => $content->content_id,
								'version_id' => $content->version_id,
								'created_by' => $loggedUser,
								'created_on' => now(),
								'updated_by' => $loggedUser,
								'updated_on' => now(),
						));
						$enrollContentStmt->execute();
					}
				}
				$insertaudit = array(
						'user_id' 			=> $userId,
						'order_id'			=> $vOrderId,
						'class_id' 			=> $classId,
						'course_id' 		=> $courseId,
						'reg_status' 		=> $regStatus,
						'reg_date'			=> now(),
						'reg_status_date'   => now(),
						'comp_status' 		=> $completionStatus,
						'waitlist_priority' => $waitlistNumber,
						'waitlist_flag' 	=> $waitlist_flag,
						'created_by'		=> $loggedUser,
						'created_on' 		=> now(),
				);
				//update expire state to completed and its no more view in Expired tab.
				$updateStmt = db_update('slt_enrollment');
				$updateStmt->condition('user_id', $userId);
				$updateStmt->condition('id', $enrollId);
				$updateStmt->fields(array(
						'comp_status'  => 'lrn_crs_cmp_cmp',
						'updated_on'   => now(),
						'updated_by'   => $loggedUser
				));
				$isUpdated = $updateStmt->execute();
				expDebug::dPrintDBAPI(' To string = ' , $insertStmt);
				$insertaudit['Enrolled_id'] = $newEnrollmentId;
				make_audit_trail_entries($insertaudit,$loggedUser , null ,$classId, 'Inserted','cre_sys_obt_cls','exp_sp_lnrenrollment','enrollUserToClass',
						array('Enrolled_id','user_id', 'order_id', 'class_id', 'course_id', 'reg_status', 'comp_status','waitlist_priority','waitlist_flag','mandatory','created_by'),$userId);

				$updStatus = 'success';
			}
			if($FromApi){
				$updStatus = ($updStatus == '') ? $errObj : $updStatus;
				return array((object)array('status' => $updStatus));
			}else{
				$result_response = array('status' => $updStatus, 'msg' => $errObj);
				echo drupal_json_output($result_response);
			}
		}
	}catch (Exception $ex) {
		watchdog_exception('MyEnrollmentChangeClass', $ex);
		expertusErrorThrow($ex);
	}

}
/**
 *
 * @param unknown $enrollment_id
 * @param string $content_id if null, all content status will be checked.
 * @param string $launchInfo
 * @return Ambigous <string, unknown>
 */
function getConsolidatedQuizStatus($enrollment_id, $content_id = null, $launchInfo = null) {
	/* expDebug::dPrint('result set quizez'.print_r(func_get_args(), 1), 5);
	$select = db_select('slt_attendance_summary','attnsum');
	$select->addField('attnsum','content_status');
	$select->condition('enrollment_id', $enrollment_id);
	if($content_id != 0) {
		$select->condition('enrollment_id', $enrollment_id);
	}
	expDebug::dPrintDBAPI('$select getConsolidatedQuizStatus',$select);
	$result = $select->execute()->fetchAll(); */
// 	expDebug::dPrint('result set quizez'.print_r(func_get_args(), 1), 5);exit;
// 	expDebug::dPrint('func args '.print_r(func_get_args(), 1), 5);
	$quiz_status = "";
	$lesson_count = 0;
	expDebug::dPrint('launch info'.print_r($launchInfo, 1), 5);
	foreach ($launchInfo as $launch) {
		if($content_id == null || ($launch['ContentId'] == $content_id)) {	//if multiple contents attached to single enrollment, separate status should be considered
			$lesson_count ++;
			$success_status = $launch ['contentQuizStatus'];
			expDebug::dPrint('$quiz_status '.$launch['Code'].' '.$lesson_count.$success_status);
			if ($success_status == "failed" || $success_status == 'incomplete' || trim($success_status) == '') {
				$quiz_status = $success_status;
				break;
			} else if(( $launch ['ContentCompletionStatus'] == 'completed' || $launch ['ContentCompletionStatus'] == '') && $launch['ContentType'] != 'Knowledge Content') {
				$quiz_status = "passed";
			} else {
				$quiz_status = "";
			}
		}
	}
	//check if all lessons of contents were launched before sending passed status
	/* if($quiz_status == "passed" && ($lesson_count != count($result))) {
		$quiz_status = "";
	} */
	expDebug::dPrint('result set quizez'.print_r($result, 1), 5);
	expDebug::dPrint('$quiz_status'.$quiz_status);
	return $quiz_status;
}


function tpdetailsforAPI($menrId,$userId,$getUserDefaultCurrency){
	$recertifyValue = $_REQUEST['recertifyid'];
	if($getUserDefaultCurrency == true) {
		$UserDefaultCurrencyArr = getUserDefaultCurrency($is_API);
	}
	$UserPrefCurrCode = $UserDefaultCurrencyArr['currency_code'];
	$UserPrefCurrSym = $UserDefaultCurrencyArr['currency_sym'];
	$UserPrefCurrLongCode = $UserDefaultCurrencyArr['currency_long_code'];
	
	$selQry = db_select('slt_master_enrollment','mas');
	$selQry->innerJoin('slt_program', 'prm', 'mas.program_id = prm.id');
	$selQry->innerJoin('slt_module', 'module', 'prm.id = module.program_id');
	if($recertifyValue == 'R')
		$selQry->innerJoin('slt_re_certify', 'rec', 'prm.id = rec.program_id AND rec.module_id = module.id');
	else
		$selQry->innerJoin('slt_module_crs_mapping', 'map', 'prm.id = map.program_id AND map.module_id = module.id');
	
	$selQry->innerJoin('slt_course_template', 'crs', 'map.course_id = crs.id');
	$selQry->innerJoin('slt_course_class', 'cl', 'crs.id = cl.course_id');
	$selQry->leftjoin('slt_enrollment', 'enroll', 'enroll.course_id = crs.id and enroll.user_id = '.$userId.' AND enroll.reg_status IN (\'lrn_crs_reg_cnf\',\'lrn_crs_reg_ppm\',\'lrn_crs_reg_rsv\',\'lrn_crs_reg_wtl\')
			AND IF(enroll.reg_status = \'lrn_crs_reg_cnf\', enroll.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'),1=1)');
	$selQry->leftjoin('slt_master_search', 'ms','ms.pk_id=cl.id AND ms.type=\'Class\'');
	$selQry->leftjoin('slt_course_class_session', 'cs', 'ms.pk_id=cs.class_id');
	$selQry->leftjoin('slt_location', 'ss','cs.location_id= ss.id');
	$selQry->leftjoin('slt_facility', 'facl','facl.location_id= ss.id and cs.location_id = facl.location_id');
	$selQry->leftjoin('slt_profile_list_items', 'splt_deliverytype', 'splt_deliverytype.code=ms.delivery_type');
	$selQry->leftjoin('slt_profile_list_items', 'splt_status', 'splt_status.code=ms.status');
	$selQry->leftjoin('slt_profile_list_items', 'splt_language', 'splt_language.code=ms.lang_code');
	$selQry->leftjoin('slt_profile_list_items', 'splt_timezone', 'splt_timezone.code=cs.timezone');
	$selQry->leftjoin('slt_node_learning_activity', 'nl',	'nl.entity_id =ms.pk_id and if(ms.type=\'Class\',nl.entity_type = \'cre_sys_obt_cls\',nl.entity_type in (\'cre_sys_obt_trn\',\'cre_sys_obt_cur\',\'cre_sys_obt_crt\'))');
	$selQry->leftjoin('slt_country', 'sc', 'sc.country_code= ss.country');
	$selQry->leftjoin('slt_state', 'st', 'st.country_code= ss.country and st.state_code= ss.state ');
	$selQry->leftjoin('slt_session_instructor_details', 'ses_ins', 'ses_ins.session_id=cs.id');
	$selQry->leftjoin('slt_location', 'splt_loc', 'splt_loc.id = cs.location_id');
	$selQry->leftjoin('slt_profile_list_items', 'loc_timezone', 'loc_timezone.code=splt_loc.timezone');
	
	$selQry->addField('mas','program_id');
	$selQry->addField('mas','overall_status');
	$selQry->addField('mas','percentage_complete');
	$selQry->addField('prm','title');
	$selQry->addField('prm','object_type');
	$selQry->addField('module', 'id', 		'module_id');
	$selQry->addField('module', 'title', 		'module_title');
	$selQry->addField('module', 'code', 		'module_code');
	$selQry->addField('module', 'short_desc', 		'module_desc');
	
	$selQry->addField('map', 'course_id', 		'crs_id');
	$selQry->addField('enroll', 'class_id', 		'cls_id');
	$selQry->addField('map', 'is_required', 		'is_required');
	$selQry->addField('map', 'sequence', 		'sequence_no');
	$selQry->addField('crs', 'title', 			'crs_title');
	$selQry->addField('crs', 'code', 				'crs_code');
	$selQry->addField('crs', 'short_description', 		'crs_desc');
	$selQry->addField('crs', 'valid_from', 		'start_date');
	$selQry->addField('crs', 'valid_to', 		'end_date');
	
	$selQry->addField('ms','type', 'object_type');
	$selQry->addField('ms','parent_pk', 'crs_id');
	$selQry->addField('ms','pk_id', 'cls_id');
	$selQry->addField('ms','code', 'cls_code');
	$selQry->addField('ms','name', 'cls_title');
	$selQry->addField('ms','description', 'cls_short_description');
	$selQry->addField('ms','delivery_type', 'delivery_type_code');
	$selQry->addField('cl','currency_type', 'currency_type');
	$selQry->addField('cl','custom4', 'export_compliance');
	$selQry->addField('splt_deliverytype','name', 'delivery_type_name');
	$selQry->addField('splt_status','name', 'status');
	$selQry->addField('splt_language','name', 'language');
	$selQry->addField('ss','name', 'location');
	$selQry->addField('ss','name', 'locationName');
	$selQry->addField('ss','addr1', 'loationAddr1');
	$selQry->addField('ss','addr2', 'locationAddr2');
	$selQry->addField('ss','city', 'LocationCity');
	$selQry->addField('st','state_name', 'locationState');
	$selQry->addField('ss','zipcode', 'locationZip');
	$selQry->addField('ss','phone', 'locationPhone');
	$selQry->addField('facl','id', 'facility_id');
	$selQry->addField('facl','contact_addr1', 'loationAddr1');
	$selQry->addField('facl','contact_addr2', 'loationAddr2');
	$selQry->addField('facl','contact_city', 'locationCity');
	$selQry->addField('facl','contact_state', 'locationState');
	$selQry->addField('facl','contact_zipcode', 'locationZip');
	$selQry->addField('facl','contact_phone', 'locationPhone');
	$selQry->addField('nl','node_id', 'node_id');
	$selQry->addField('cs','id', 'session_id');
	$selQry->addField('cs','start_date', 'sess_start_date');
	$selQry->addField('cs','start_time', 'sess_start_time');
	$selQry->addField('cs','end_time', 'sess_end_time');
	$selQry->addField('cs','end_date', 'sess_end_date');
	$selQry->addField('cs','timezone', 'timezone'); // Added by Priya on Oct 30, 2013 for #0028593
	$selQry->addField('sc','country_name', 'country_name');
	$selQry->addField('cl','lang_code', 'language_code');
	$selQry->addField('sc','country_code', 'country_code');
	$selQry->addField('cl','registration_end_on', 'registration_end_on');
	$selQry->addField('enroll','course_id', 'enrcrs_id');
	$selQry->addField('enroll','class_id', 'enrclsid');
	$selQry->addField('cl','course_id', 'clscrsid');
	$selQry->addField('cl','id', 'clsid');
	
	$selQry->addField('cs','title', 'session_title');
	$selQry->addField('cs','course_id', 'course_id');
	$selQry->addField('cs','class_id', 'class_id');
	$selQry->addField('splt_timezone','attr2', 'session_timezone');
	$selQry->addField('splt_timezone','name', 'session_fulltimezone');
	$selQry->addField('loc_timezone','attr2', 'location_timezone');
	$selQry->addField('loc_timezone','name', 'location_fulltimezone');
	$selQry->addField('splt_loc','name', 'name');
	$selQry->addField('splt_loc','addr1', 'contact_addr1');
	$selQry->addField('splt_loc','addr2', 'contact_addr2');
	$selQry->addField('splt_loc','city', 'contact_city');
	$selQry->addField('splt_loc','zipcode', 'contact_zipcode');
	$selQry->addField('splt_loc','latitude', 'contact_latitude');
	$selQry->addField('splt_loc','longitude', 'contact_longitude');
	$selQry->addField('cs','title', 'session_title');
	
	$selQry->addExpression('count(1)', 'clscount');
	$selQry->addExpression('slf_convert_price(cl.price,cl.currency_type,\''.$UserPrefCurrCode.'\')','price');
	$selQry->addExpression('\''.$UserPrefCurrSym.'\'','currency_symbol');
	$selQry->addExpression('\''.$UserPrefCurrCode.'\'','currency');
	$selQry->addExpression('\''.$UserPrefCurrLongCode.'\'','currency_code');
	$selQry->addExpression("CONCAT( DATE_FORMAT(cs.start_date, _utf8'%Y-%m-%d'),' ',cs.start_time,':00')", 'session_end_datetime'); // Added by Priya on Oct 30, 2013 for #0028593
	$selQry->addExpression('GROUP_CONCAT(DISTINCT(ses_ins.instructor_id))', 'session_instructor_id');
	$selQry->addExpression('(select country_name from slt_country where country_code = splt_loc.country)', 'contact_country');
	$selQry->addExpression('(select state_name from slt_state where state_code = splt_loc.state and country_code =  splt_loc.country)', 'contact_state');
	
	$selQry->condition('mas.id',$menrId);
	$selQry->where('IF(ms.type=\'Class\',
			( splt_deliverytype.lang_code=\'cre_sys_lng_eng\'
			and splt_status.lang_code=\'cre_sys_lng_eng\'
			and splt_language.lang_code=\'cre_sys_lng_eng\'
			and nl.entity_type=\'cre_sys_obt_cls\'
			and IF(enroll.course_id=cl.course_id AND enroll.class_id=cl.id, 1=1 ,ms.status in (\'lrn_cls_sts_atv\',\'lrn_crs_sts_atv\'))
	),1=1 )'
	);
	$selQry->where('cl.registration_end_on > now() or cl.registration_end_on is null');
	$selQry->where('cl.registration_end_on > now() or cl.registration_end_on is null');
	
	$selQry->groupBy('cs.id');
	$selQry->groupBy("cl.id having IF(enroll.course_id=cl.course_id AND enroll.class_id=cl.id , 1=1, (CONCAT( DATE_FORMAT(cs.start_date, _utf8'%Y-%m-%d'),' ',cs.start_time,':00')) > IFNULL(CONVERT_TZ(now(), '".$defaultTimezone."' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= cs.timezone)),NOW()) or (CONCAT( DATE_FORMAT(cs.start_date, _utf8'%Y-%m-%d'),' ',cs.start_time,':00')) is null)");
	$selQry->orderBy('map.sequence');
	$selQry->orderBy('cs.start_date', 'asc');
	$selQry->orderBy('cs.start_time', 'asc');
	
	expDebug::dPrintDBAPI("Query for tpdetailsforAPI",$selQry);
	
	$res = $selQry->execute()->fetchAll();
	$cntVal = count($res);
	expDebug::dPrint("Value of count of the result".$cntVal,4);
	expDebug::dPrint("Result of tp details for list enrollments API".print_r($res,true),4);
	
	$tpDet = '';
	$modId ='';
	$courseId ='';
	$classId = '';
	$sessionId = '';
	$cnt = 0;
	foreach($res as $key => $val){
		
		if(!empty($classId) && $classId != $val->cls_id){
			$clsArr['class_details'] = $clsDts;
			$clsArr['session_details'] = $clsSesDts;
			$classArr[] = $clsArr;
			$clsArr = array();
			expDebug::dPrint("Class and session details Added to the array value".print_r($classArr,true),4);
			
			if(!empty($courseId) && $courseId != $val->crs_id){
				$crsResDet->class_details = $classArr;
				$crsDet[] = $crsResDet;
				expDebug::dPrint("Course details Added to the array value".print_r($crsDet,true),4);
			}
			
			if(!empty($modId) && $modId != $val->module_id){
				$mdDet['course_details'] = $crsDet;
				$mdDet['module_details'] = $modVal;
				$modArr[] = $mdDet;
				$mdDet = array();
				expDebug::dPrint("Module details Added to the array value".print_r($modArr,true),4);
			}
		} 
		
	  // Session Details

		if($sessionId == '' || $sessionId != $val->session_id){
			if($val->delivery_type_code == 'lrn_cls_dty_ilt' || $val->delivery_type_code == 'lrn_cls_dty_vcl'){
				$vTimeZone = ($val->delivery_type_code == 'lrn_cls_dty_ilt') ? $val->location_timezone : $val->session_timezone;
				$ses_start_date_form     = explode(" ",$val->sess_start_date);
				$ses_start_date_format   = $ses_start_date_form[0]." ".$val->sess_start_time;
				$ses_end_date_form       = explode(" ",$val->sess_start_date);
				$ses_end_date_format     = $ses_end_date_form[0]." ".$val->sess_end_time;
					
				if($val->delivery_type_code == 'lrn_cls_dty_ilt'){
					$session_day = ($record->sess_start_date != null ) ? date_format(date_create($ses_start_date_format),'D') : '';
						
					$sesDet->session_title = $val->session_title;
					$sesDet->session_day = $session_day;
					$sesDet->start_date = date_format(date_create($ses_start_date_format),'M d, Y');
					$sesDet->start_time = date_format(date_create($ses_start_date_format),'h:i');
					$sesDet->start_form = date_format(date_create($ses_start_date_format),'A');
					$sesDet->end_time = date_format(date_create($ses_end_date_format),'h:i');
					$sesDet->end_form = date_format(date_create($ses_end_date_format),'A');
						
				}else{
					$vStartDateTime = $ses_start_date_format;
					$vEndDateTime = $ses_end_date_format;
					$UserPHPTimezoneName		= getUserPHPTimezoneName($userId);
					$vUserPHPTimezonename		= $UserPHPTimezoneName[0]->zone_name;
					if(!$UserPHPTimezoneName[0]->zone_name){
						$vUserPHPTimezonename = date_default_timezone(FALSE);
					}
					$oStartDateTime  = timeZoneConvert($vStartDateTime , $vTimeZone,  $vUserPHPTimezonename);
					$ses_start_date_format 	= date_format($oStartDateTime,'Y-m-d H:i');
					$oEndDateTime    = timeZoneConvert($vEndDateTime, $vTimeZone,  $vUserPHPTimezonename);
					$ses_end_date_format 	= date_format($oEndDateTime,'Y-m-d H:i');
					$session_day = ($record->session_start != null ) ? date_format(date_create($ses_start_date_format),'D') : '';
						
					$sesDet->session_title = $val->session_title;
					$sesDet->session_day = $session_day;
					$sesDet->start_date = date_format(date_create($ses_start_date_format),'M d, Y');
					$sesDet->start_time = date_format(date_create($ses_start_date_format),'h:i');
					$sesDet->start_form = date_format(date_create($ses_start_date_format),'A');
					$sesDet->end_time = date_format(date_create($ses_end_date_format),'h:i');
					$sesDet->end_form = date_format(date_create($ses_end_date_format),'A');
						
				}
				$clsSesDts[] = $sesDet;
				expDebug::dPrint("Session detail values".print_r($clsSesDts,true),4);
			}
			$sessionId = $val->session_id;
		}
		
		if($classId == '' || $classId != $val->cls_id){
			// Class Details
			$clsDts->object_type = $val->ms_type;
			$clsDts->crs_id = $val->ms_parent_pk;
			$clsDts->cls_id = $val->ms_pk_id;
			$clsDts->cls_code = $val->cls_code;
			$clsDts->cls_title = $val->cls_title;
			$clsDts->cls_short_description = $val->cls_short_description;
			$clsDts->delivery_type_code = $val->delivery_type_code;
			$clsDts->currency_type = $val->currency_type;
			$clsDts->export_compliance = $val->export_compliance;
			$clsDts->delivery_type_name = $val->delivery_type_name;
			$clsDts->status = $val->status;
			$clsDts->language = $val->language;
			$clsDts->location = $val->location;
			$clsDts->locationname = $val->locationname;
			$clsDts->loationaddr1 = $val->loationaddr1;
			$clsDts->locationaddr2 = $val->locationaddr2;
			$clsDts->locationcity = $val->locationcity;
			$clsDts->locationstate = $val->locationstate;
			$clsDts->locationzip = $val->locationzip;
			$clsDts->locationphone = $val->locationphone;
			$clsDts->facility_id = $val->facility_id;
			$clsDts->facl_contact_addr1 = $val->facl_contact_addr1;
			$clsDts->loationaddr2 = $val->loationaddr2;
			$clsDts->facl_contact_state = $val->facl_contact_state;
			$clsDts->facl_contact_zipcode = $val->facl_contact_zipcode;
			$clsDts->facl_contact_phone = $val->facl_contact_phone;
			$clsDts->node_id = $val->node_id;
			$clsDts->session_id = $val->session_id;
			$clsDts->sess_start_date = $val->sess_start_date;
			$clsDts->sess_start_time = $val->sess_start_time;
			$clsDts->sess_end_time = $val->sess_end_time;
			$clsDts->sess_end_date = $val->sess_end_date;
			$clsDts->timezone = $val->timezone;
			$clsDts->country_name = $val->country_name;
			$clsDts->language_code = $val->language_code;
			$clsDts->country_code = $val->country_code;
			$clsDts->registration_end_on = $val->registration_end_on;
			$clsDts->enrcrs_id = $val->enrcrs_id;
			$clsDts->enrclsid = $val->enrclsid;
			$clsDts->clscrsid = $val->clscrsid;
			$clsDts->clsid = $val->clsid;
			$clsDts->price = $val->price;
			$clsDts->currency_symbol = $val->currency_symbol;
			$clsDts->currency = $val->currency;
			$clsDts->currency_code = $val->currency_code;
			$clsDts->session_end_datetime = $val->session_end_datetime;
			$clsDts->enrolled_id = getRegisteredOrNot($val->crs_id,$val->cls_id,$userId);
			$lstEnrid = getLastCompCourseClass($courseId,$userId);
			$clsDts->class_last_comp = ($lstEnrid->enrolled_id) ? $lstEnrid : '';
			$clsDts->waitlist_status = getWaitlistCatalogInfo($val->cls_id);
			$clsDts->availableSeats = getAvailableSeats($val->cls_id);
			$classId = $val->cls_id;
			expDebug::dPrint("Class detail values".print_r($clsDts1,true),4);
		}	
		
		if($courseId == '' || $courseId != $val->crs_id){
			// Course Details
			$crsResDet->clscount = $val->clscount;
			$crsResDet->crs_id = $val->crs_id;
			$crsResDet->is_required = $val->is_required;
			$crsResDet->sequence_no = $val->sequence_no;
			$crsResDet->crs_title = $val->crs_title;
			$crsResDet->crs_code = $val->crs_code;
			$crsResDet->crs_desc = $val->crs_desc;
			$crsResDet->start_date = $val->start_date;
			$crsResDet->end_date = $val->end_date;
			$crsResDet->registered_cnt = getRegisteredStatusForCourse($val->crs_id,$userId);
			$maxCount = getIfSingleClass($record->crs_id);
			$crsResDet->single_class = ($maxCount[0]->maxcnt == 1) ? $maxCount[0]->classids : '';
			$courseId = $val->crs_id;
			expDebug::dPrint("Course detail values".print_r($crsResDet,true),4);
		}
			
		if($modId == '' || $modId != $val->module_id){
			// Module Details
			$modVal->module_id = $val->module_id;
			$modVal->module_title = $val->module_title;
			$modVal->module_code = $val->module_code;
			$modVal->module_desc = $val->module_desc;
			$modId = $val->module_id;
			expDebug::dPrint("Module detail values".print_r($modVal,true),4);
		}
		$cnt++;
		expDebug::dPrint("Values of count".$cnt.'total count'.$cntVal,4);
		if($cnt == $cntVal){
			$mdDet['module_details'] = $modVal;
			$mdDet['course_details'] = $crsDet;
			$modArr[] = $mdDet;
			$mdDet = array();
		}
		
	}
	
	expDebug::dPrint("Values after formated the result---->>>".print_r($modArr,true),4);
	return $modArr;
}


function getMyDashboardAPI(){

	$userid = $_REQUEST ['user_id'];
	expDebug::dPrint("Inside getMyDashboardAPI");
	if ($_REQUEST ['type'] == 'overall_progress') {

		if ($_REQUEST ['action'] == 'class') {
			$select = db_select ( 'slt_enrollment', 'enroll' );
			$select->addExpression ( '(select count(id) from slt_enrollment where user_id='.$userid .' and comp_status=\'lrn_crs_cmp_cmp\' and reg_status = \'lrn_crs_reg_cnf\' and master_enrollment_id is null)', 'completed_count' );
			$select->addExpression ( '(select count(id) from slt_enrollment where user_id=\'' . $userid . '\' and comp_status=\'lrn_crs_cmp_inc\' and reg_status = \'lrn_crs_reg_cnf\' and master_enrollment_id is null)', 'incomplete_count' );
			$select->addExpression ( '(select count(id) from slt_enrollment where user_id=\'' . $userid . '\' and comp_status in (\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_inp\') and reg_status = \'lrn_crs_reg_cnf\' and master_enrollment_id is null)', 'enrolled_count' );
			$select->addExpression ( '(select count(id) from slt_enrollment where user_id=\'' . $userid . '\' and comp_status=\'lrn_crs_cmp_exp\' and reg_status = \'lrn_crs_reg_cnf\' and master_enrollment_id is null)', 'expired_count' );
			$select->condition ( 'enroll.user_id', $userid );
			$select->groupBy ( 'enroll.user_id' );
			expDebug::dPrintDBAPI ( '$select', $select );
			$count = $select->execute()->fetchAll ();
			$result->overallprogress = $count[0];
			expDebug::dPrint ( "getMyDashboardAPI getMyDashboardAPIgetMyDashboardAPI" . print_r ( $result, true ), 4 );
				
			return $result;
		}else if ($_REQUEST ['action'] == 'tp') {
			$select = db_select ( 'slt_master_enrollment', 'sme' );
			$select->addExpression ( '(select count(id) from slt_master_enrollment where user_id=\'' . $userid . '\' and overall_status=\'lrn_tpm_ovr_cmp\')', 'completed_count' );
			$select->addExpression ( '(select count(id) from slt_master_enrollment where user_id=\'' . $userid . '\' and overall_status=\'lrn_tpm_ovr_inc\')', 'incomplete_count' );
			$select->addExpression ( '(select count(id) from slt_master_enrollment where user_id=\'' . $userid . '\' and overall_status=\'lrn_tpm_ovr_enr\')', 'enrolled_count' );
			$select->addExpression ( '(select count(id) from slt_master_enrollment where user_id=\'' . $userid . '\' and overall_status=\'lrn_tpm_ovr_exp\')', 'expired_count' );
			$select->condition ( 'sme.user_id', $userid );
			$select->groupBy ( 'sme.user_id' );
			expDebug::dPrintDBAPI ( '$select', $select );
			$count = $select->execute()->fetchAll ();
			$result->overallprogress = $count[0];
			expDebug::dPrint ( "getMyDashboardAPI getMyDashboardAPIgetMyDashboardAPI" . print_r ( $result, true ), 4 );
			return $result;
		}
	}else if($_REQUEST ['type'] == 'learning_activity'){
		$select = db_select ( 'slt_enrollment', 'enroll' );
		$select->addExpression ('count(enroll.id)' , 'enrolled_count' );
		$select->addExpression ('CAST(reg_date AS DATE)' , 'enrolled_date' );
		$select->condition ( 'enroll.user_id', $userid );
		$select->condition ( 'enroll.reg_status', 'lrn_crs_reg_cnf');
		$select->condition('enroll.reg_date', array($_REQUEST['startdate'],$_REQUEST['enddate']), 'BETWEEN');
		$select->groupBy ( 'enrolled_date' );
		expDebug::dPrintDBAPI ( '$select', $select );
		$enrolled_status = $select->execute()->fetchAll();
		
		$select = db_select ( 'slt_enrollment', 'enroll' );
		$select->addExpression ('count(enroll.id)' , 'completed_count' );
		$select->addExpression ('CAST(comp_date AS DATE)' , 'completed_date' );
		$select->condition ( 'enroll.user_id', $userid );
		$select->condition ( 'enroll.reg_status', 'lrn_crs_reg_cnf' );
		$select->condition ( 'enroll.comp_status', 'lrn_crs_cmp_cmp' );
		$select->condition('enroll.comp_date', array($_REQUEST['startdate'],$_REQUEST['enddate']), 'BETWEEN');
		$select->groupBy ( 'completed_date' );
		expDebug::dPrintDBAPI ( '$select', $select );
		$completed_status = $select->execute()->fetchAll();
		
		$result->learningactivity->completed_status =$completed_status;
		$result->learningactivity->enrolled_status =$enrolled_status;
		
// 		$result->tagname = $enrolled_status;
		expDebug::dPrint ( "getMyDashboardAPI getMyDashboardAPIgetMyDashboardAPI" . print_r ( $result, true ), 4 );
		return $result;

	}else if($_REQUEST ['type'] == 'things_to_be_done'){
		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_manage/exp_sp_administration_announcement/exp_sp_administration_announcement.inc');
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		expDebug::dPrint('things_to_be_done test 11');
		$select = db_select('slt_enrollment', 'enroll');
		$select->addExpression ('count(id)' , 'total' );
		$select->addExpression ('sum(case when enroll.comp_status = \'lrn_crs_cmp_cmp\' then 1 else 0 end)' , 'comp_count' );
		$select->condition ( 'enroll.user_id', $userid );
		$select->condition ( 'enroll.reg_status', 'lrn_crs_reg_cnf' );
		$select->condition ( 'enroll.is_compliance',  1);
		expDebug::dPrintDBAPI ( 'is_compliance', $select );
		$compliance = $select->execute()->fetchAll();
		expDebug::dPrint('$compliance'.print_r($compliance,true));
		$select = db_select('slt_enrollment', 'enroll');
		$select->addExpression ('count(id)' , 'total' );
		$select->addExpression ('sum(case when enroll.comp_status = \'lrn_crs_cmp_cmp\' then 1 else 0 end)' , 'comp_count' );
		$select->condition ( 'enroll.user_id', $userid );
		$select->condition ( 'enroll.reg_status', 'lrn_crs_reg_cnf' );
		$select->condition ( 'enroll.mandatory', 'Y');
		expDebug::dPrintDBAPI ( 'mandatory', $select );
		$mandatory = $select->execute()->fetchAll();
		expDebug::dPrint('$mandatory'.print_r($mandatory,true));
		
		
		$comp_sts = array(
				'lrn_tpm_ovr_rsc',
				'lrn_tpm_ovr_cln',
				'lrn_tpm_ovr_ppm',
				'lrn_tpm_ovr_exp',
		);
		
		
		$select = db_select('slt_master_enrollment', 'sme');
		$select->leftjoin('slt_program', 'prm', 'sme.program_id = prm.id and sme.id = (select max(id) from slt_master_enrollment where program_id = sme.program_id and user_id='.$userid.' ) ');
		$select->addExpression ('count(*)' , 'total' );
		$select->addExpression ('sum(case when sme.overall_status = \'lrn_tpm_ovr_cmp\' then 1 else 0 end)' , 'comp_count' );
		$select->condition ( 'sme.user_id', $userid );
		$select->condition ( 'prm.object_type', 'cre_sys_obt_crt' );
		$select->condition ( 'sme.overall_status',$comp_sts ,'NOT IN' );
		expDebug::dPrintDBAPI ( 'Certification mandatory', $select );
		$certification = $select->execute()->fetchAll();
		expDebug::dPrint('$certification'.print_r($certification,true));
		
		
		$query = db_query('SELECT count(*) as count
							FROM (select prg.expires_in_value AS prg_expires_in_value, prg.expires_in_unit AS prg_expires_in_unit,masterenroll.comp_date as comp_date,
							case when prg.expires_in_unit = \'days\' then (SELECT DATE_ADD(comp_date,INTERVAL prg.expires_in_value DAY) )
								when prg.expires_in_unit = \'months\' then (SELECT DATE_ADD(comp_date,INTERVAL prg.expires_in_value MONTH) )
								when prg.expires_in_unit = \'years\' then (SELECT DATE_ADD(comp_date,INTERVAL prg.expires_in_value YEAR) )
 								else 0 end as expires_on 
								from slt_master_enrollment masterenroll
								INNER JOIN slt_program prg ON prg.id = masterenroll.program_id
								where masterenroll.user_id = '.$userid.' and masterenroll.overall_status = \'lrn_tpm_ovr_cmp\' and prg.object_type =\'cre_sys_obt_crt\' ) acc where acc.expires_on between now() and now() + interval 30 DAY');
		
		expDebug::dPrintDBAPI ( 'Expires in', $query );
		$certification_expiry = $query->fetchAll();
		
		
		$query = db_query('select count(*) as count from slt_enrollment enroll 
							left join slt_course_class scc on enroll.class_id = scc.id and enroll.course_id = scc.course_id 
							left join slt_course_class_session sess on sess.class_id = scc.id and sess.course_id = scc.course_id 
							where user_id = '.$userid.' and scc.`delivery_type` in (\'lrn_cls_dty_vcl\',\'lrn_cls_dty_ilt\') and (sess.start_date between now() and now() + interval 7 DAY ) and enroll.reg_status =\'lrn_crs_reg_cnf\'
							 group by enroll.class_id and  enroll.id;');
		$upcoming_classes = $query->fetchAll();
		
		expDebug::dPrint('$things_to_be_done0000');
		
		$announcementListCount = fetchAnnouncementUserWidget('count',$userid);
		expDebug::dPrint('$things_to_be_done111'.$announcementListCount);
		
// 		$compliancequery = db_query('select count(*) total,sum(case when comp_status = \'lrn_crs_cmp_cmp\' then 1 else 0 end) comp_count from slt_enrollment where user_id="'.$userid.'" and is_compliance =1 and reg_status=\'lrn_crs_reg_cnf\'');
// 		expDebug::dPrint('sfsafsafsaf111'.$compliancequery);
		
// 		$compliance = $query->fetchAll();
// 		$mandatoryquery = db_query("select count(*) total,sum(case when comp_status = \'lrn_crs_cmp_cmp\' then 1 else 0 end) comp_count from slt_enrollment where user_id='".$userid."' and is_compliance =1 and reg_status='lrn_crs_reg_cnf'");
// 		$mandatory = $query->fetchAll();
// 		$certificationquery = db_query("select count(*) total,sum(case when overall_status = \'lrn_tpm_ovr_cmp\' then 1 else 0 end) comp_count from `slt_master_enrollment` where user_id='".$userid."' and overall_status not in ('lrn_tpm_ovr_cln','lrn_tpm_ovr_ppm','lrn_tpm_ovr_rsc')");
// 		$certification = $query->fetchAll();
		
		
		$things_to_be_done->compliance = $compliance[0];
		$things_to_be_done->mandatory = $mandatory[0];
		$things_to_be_done->certification = $certification[0];
		$things_to_be_done->certification_expiry = $certification_expiry[0];
		$things_to_be_done->upcoming_classes = $upcoming_classes[0];
		$things_to_be_done->announcement = $announcementListCount;
		
		
		
		
		expDebug::dPrint('$things_to_be_done'.print_r($things_to_be_done,true));
// 		return '';
		
		$result->things_to_be_done = $things_to_be_done;
		return $result;
		
		
	}else if($_REQUEST ['type'] == 'announcement_list'){
		if ($_REQUEST ['action'] == 'fetch') {
			include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_manage/exp_sp_administration_announcement/exp_sp_administration_announcement.inc');
			drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
			$announcementListDetails = fetchAnnouncementUserWidget('list',$userid,0,$_REQUEST['limit']);
			$i=0;
			foreach ($announcementListDetails as $announcement) {
				
				$response->rows[$i]['id']=$announcement->uid;
				$response->rows[$i]['personid']=$announcement->personid;
				$response->rows[$i]['fullname']=$announcement->full_name;
				$response->rows[$i]['nid'] = $announcement->nid;
				$response->rows[$i]['description'] = $announcement->description;
				$response->rows[$i]['from_date'] = Date('M,d Y',strtotime($announcement->from_date));
				$response->rows[$i]['announce_fulldescription'] = $announcement->description;
				
				if (!empty($announcement->uri)) {
					$response->rows[$i]['image_url'] = file_create_url($announcement->uri);
				}
				else {
					$response->rows[$i]['image_url'] = '';
				}
				$i++;
			}
			expDebug::dPrint('$things_to_be_done111'.print_r($response,true));
			$result->announcements = $response;
			return $result;
		}else if($_REQUEST ['action'] == 'update'){
			expDebug::dPrint('announcement viewed update',5);
			if(!empty($_REQUEST['announcement_id'])){
				$dbquery = "UPDATE slt_site_notice SET viewed = IFNULL (CONCAT( viewed , ',".$userid."' ), ".$userid.") WHERE (Not FIND_IN_SET(".$userid.", viewed) or viewed is null) AND id in (".$_REQUEST['announcement_id'].")";
				expDebug::dPrint('update with id'.$dbquery);
				$query = db_query($dbquery);
				expDebug::dPrintDBAPI ( 'Update Viewed', $query );
				$result->announcements = 'success';
				
			}else{
				drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
			$dbquery = "UPDATE slt_site_notice SET viewed = IFNULL (CONCAT( viewed , ',".$userid."' ), ".$userid.") WHERE Not FIND_IN_SET(".$userid.", viewed) or viewed is null";
			expDebug::dPrint('$wewewewewewewewewewewewewewewe'.$dbquery);
			$query = db_query($dbquery);
			expDebug::dPrintDBAPI ( 'Update Viewed', $query );
			
				
					
					
					  $updateStmt = db_update('slt_person_preference');
					  $updateStmt->condition('user_id', $userid);

					  $updateStmt->fields(array(
   					                     'announcement_visited'      => now(),
					'updated_on' => now(),
      					                  'updated_by'    => $userid
       					                 ));

   						expDebug::dPrintDBAPI(' $updateStmt = ' , $updateStmt);
   						$updateStmt->execute();
			
			
			$result->announcements = 'success';
			}
			return $result;
		}else if($_REQUEST ['action'] == 'delete'){
			expDebug::dPrint('announcement delete',5);
			drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
			include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_manage/exp_sp_administration_announcement/exp_sp_administration_announcement.inc');
			
			$tmp_arr_announcement_id = explode("|",$_REQUEST['announcement_id']);
			foreach ($tmp_arr_announcement_id as $announcement_id) {
				if(!empty($announcement_id))
				dismiss_announcement($announcement_id,$userid);
			}
			
			$select = db_select('slt_notification', 'notif');
			$select->addExpression('COUNT(notif.id)');
			$select->condition('notif.user_id', $userid);
			$select->condition('notif.mobile', 1);
			$select->condition('notif.viewed', 1 , '<>');
			$select->condition('notif.soft_delete', 1,'<>' );
			$notViewedNotif =  $select->execute()->fetchField();
			$announcementListCount = fetchAnnouncementUserWidget('count',$userid);
			
			expDebug::dPrint('notviewed_con'.$announcementListCount,5);
				
			$notification_notviewed = $notViewedNotif+$announcementListCount;
			$result->announcements->status = 'success';
			$result->announcements->notviewed = $notification_notviewed;
			return $result;
		}		
	}
}


function UpdateTimeSpentMobileAPI(){
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	expDebug::dPrint ( "Inside UpdateTimeSpentMobileAPI", 4 );
	$userid = $_REQUEST ['userid'];
	
	if($_REQUEST ['action'] == 'set'){
		$timespent = $_REQUEST ['time_spent'];
		$insertStmt = db_insert('slt_mobile_time_spent');
		$insertStmt->fields(array(
				'user_id'			=> $userid,
				'time_spent' 		=> $timespent,
				'spent_on' 			=> now()
		));
		$newId = $insertStmt->execute();
		expDebug::dPrint ( " newId".$newId, 4 );
		$result->status = ( !empty($newId) )?'success':'failure';
		return $result;
	}else if($_REQUEST ['action'] == 'get'){
		$select = db_select ( 'slt_mobile_time_spent', 'smt' );
		$select->addExpression ('SUM(smt.time_spent)' , 'timespent' );
		$select->addExpression ('CAST(spent_on AS DATE)' , 'spenton' );
		$select->condition ( 'smt.user_id', $userid );
		$select->condition('smt.spent_on', array($_REQUEST['startdate'],$_REQUEST['enddate']), 'BETWEEN');
		$select->groupBy ( 'spenton' );
		expDebug::dPrintDBAPI ( '$select', $select );
		$result->time_spent = $select->execute ()->fetchAll();
		expDebug::dPrint ( " timespent".print_r($result,true), 4 );
		return $result;
	}
}


function getLearningPlanResultSetMobileAPI($user_id){
	
	try{
		
				$searchtxt = 	$_REQUEST['searchText'] ;
		
				$userTimezone = getUserTimezone();
// 				$user_id   	= $user_id;
				$regstatus  	= stripRegStatus($_GET['regstatuschk']);
	
	
				$groupIdArray = getGroupsDetails($user_id,'','','',0);
				$tmp = array();
				foreach ($groupIdArray as $b) $tmp[] = $b->id; //pull out data
				$groupIds = implode("','", $tmp); //and implode
				expDebug::dPrint(" groupIds".print_r($groupIds,true),5);
	
				$dueDays = $_REQUEST['due'];
				$scheduledDays = $_REQUEST['scheduled'];
				
				
				
				if(!empty($searchtxt)){
					$fieldQueryforTags = "group_concat(distinct(td.tagname)) as tagname,";
					$joinQueryfroTags = "LEFT JOIN slt_tag_entity ste  on (ste.entity_id = masterenroll.program_id )
            					 LEFT JOIN slt_tagdefn td on ste.tagid = td.id and (td.entity_type IN  ('Certification','Curricula','Course')) and (td.activity_count > 0)";
				}else {
					$fieldQueryforTags = '';
					$joinQueryfroTags = '';
				}
				
				
				
				
				if($dueDays > 0 || $scheduledDays > 0){
				
				$fieldquery = "prg.end_date as dueDate,
            DATE_FORMAT(CONVERT_TZ(concat(date_format(ses.start_date,'%Y-%m-%d'),' ',ses.start_time),spl.attr2,'UTC'),'%Y-%m-%d %H:%i:%s') as sessionDateTime,";
				
				$joinQuery = "LEFT OUTER JOIN slt_profile_list_items spl ON ses.timezone = spl.code";
				
				}else{
					$fieldquery = '';
					$joinQuery = '';
				}
				
				$entity_type = "'cre_sys_obt_cur','cre_sys_obt_trn','cre_sys_obt_crt'";	
			 $enrollQuery = "select
		     masterenroll.id AS id,
		     masterenroll.launchable AS launchable,
			 masterenroll.user_id AS user_id,
			 null AS master_enrollment_id,
		     null AS class_id,
		   	 masterenroll.program_id  AS course_id,
		     null AS check_entity_id,
		     prg.object_type AS check_entity_type,
		     if(masterenroll.mandatory = '1','Y',0) as mandatory,
		     null AS is_compliance,
		     null AS compliance_complete_date,
		     null AS compliance_complate_days,
		     null AS compliance_validity_date,
		     null AS compliance_validity_days,
		     null AS `compliance_completed`,
		     masterenroll.recertify_path AS recertify_path,
		     masterenroll.overall_status As reg_status,
		     masterenroll.overall_status AS reg_status_code,
		     null AS status_code,
		     DATE_FORMAT(`masterenroll`.`reg_date`,_utf8'%d-%b-%Y') AS reg_date,
		     masterenroll.overall_status AS comp_status,
		     DATE_FORMAT(`masterenroll`.`comp_date`,_utf8'%d-%b-%Y') AS comp_date,
		     DATE_FORMAT(`masterenroll`.`updated_on`,_utf8'%d-%b-%Y') AS update_date,
		     null AS reg_status_date,
		     null AS valid_from,
		     null AS valid_to,
		     null AS score,
		   	 null AS grade,
		   	 masterenroll.overall_progress AS progress,
		     null AS updated_on,
		     null AS updated_by,
		     masterenroll.created_by AS created_by,
		     null AS waitlist_priority,
		  	 null AS preassesment_completion_status,
		  	 prg.title AS title,
		  	 null AS crs_code,
		  	 prg.title AS cls_title,
		 	 prg.code AS CODE,
		 	 null AS cls_status,
		     prg.short_desc AS description,
		     prg.object_type AS delivery_type_code,
		     null AS dedicated_class_flag,
		     null AS delivery_type,
		 	 prg.type AS BaseType,
		 	 null AS courseid,
		     
			 slf_convert_price(`prg`.`price`,prg.currency_type,'USD') AS `classprice`,
			 		
		 	 null AS currency_type,
		  	 null AS currency_symbol,
		     null AS currency,
		     null AS currency_code,
		     null AS LangCode,
		     snl.node_id AS node_id,
			 		
             `loc`.`id` AS `LocationId`,
			 `loc`.`name`  AS `LocationName`,
			 `loc`.`addr1` AS `LocationAddr1`,
             `loc`.`addr2` AS `LocationAddr2`,
             `loc`.`city` AS `LocationCity`,
			 `loc`.`latitude` AS `latitude`,
			 `loc`.`longitude` AS `longitude`,
             (SELECT  IF(COUNT(state_name)>0,state_name,loc.state)  stname FROM slt_state WHERE state_code= loc.state limit 1) AS `LocationState`,
             `state`.`state_name` AS `stateName`,
             `loc`.`zipcode` AS `LocationZip`,
             `loc`.`phone` AS `LocationPhone`,
             (SELECT `country_name` FROM `slt_country` WHERE `country_code`=`loc`.`country` LIMIT 1) AS `LocationCountry`,
			 		
			 null AS Language,
		     null AS user_name,
		     null AS orderdatetime,
		     null AS session_id,
		     null AS session_start,
		     null AS session_start_time,
		     null AS session_end_time,
		     masterenroll.content_status AS content_status,
		     null AS session_title,
		     if(mro.mro = 'cre_sys_inv_opt',3,if(mro.mro = 'cre_sys_inv_man',1,2)) as ascen,
  			null  as `cmpl_expired`,
			 		
			 		
			 		
			 		
			 		
  			p.manager_id as `managerID`,
  		   `p`.`hire_date` as `hire_date`,
			
			spli.attr2 as `timezone`,
  			spli.attr2 as `ptimezone`,
  			`masterenroll`.`comp_date` as `completion_date`,
             CONVERT_TZ(masterenroll.reg_date,'".$userTimezone."','UTC') AS `registration_date`,	
			 		
			 	 ". $fieldquery ."	
			 		
             ".$fieldQueryforTags."
             
             		
             		
             		
             		
             IF(exmp.exempted_status = 1 , NULL,CONCAT(splt_mro.name,'##',splt_mro.code)) AS mro,
		     null as assigned_by,
		     null AS survey_status,
		     null AS assessment_status,
			 null AS preassessment_status,
		     (SELECT CONCAT(exmp.exempted_status,'|',exmp.created_by,'|',exmp.created_on) FROM slt_enrollment_exempted exmp WHERE exmp.enrollment_id = masterenroll.id AND exmp.enroll_type = 'tp' ORDER BY id DESC LIMIT 1) as exempted_sts,
			masterenroll.overall_progress AS percentage_complete,
  			case when prg.expires_in_unit = 'days' then (SELECT DATE_ADD(masterenroll.comp_date,INTERVAL prg.expires_in_value DAY) )
								when prg.expires_in_unit = 'months' then (SELECT DATE_ADD(masterenroll.comp_date,INTERVAL prg.expires_in_value MONTH) )
								when prg.expires_in_unit = 'years' then (SELECT DATE_ADD(masterenroll.comp_date,INTERVAL prg.expires_in_value YEAR) )
 								else 0 end  AS prg_expires_on,
			 		
  			prg.expires_in_value AS prg_expires_in_value,
 			prg.expires_in_unit AS prg_expires_in_unit,
     		prg.start_date  AS prg_start_date,
      		prg.end_date  AS prg_end_date,
			prg.enforce_sequence  AS enforce_sequence,
			masterenroll.is_current  AS is_current
			from slt_master_enrollment masterenroll
			INNER JOIN slt_program prg ON prg.id = masterenroll.program_id
			LEFT OUTER JOIN slt_group_mapping mro ON mro.entity_id = masterenroll.program_id AND mro.entity_type IN(".$entity_type.") AND mro.group_id IN('".$groupIds."')
			LEFT OUTER JOIN slt_groups grp ON grp.id = mro.group_id AND grp.job_role IN (select group_concat(job_role) from slt_person_jobrole_mapping where user_id =".$user_id.")
			LEFT OUTER JOIN slt_node_learning_activity snl ON snl.entity_id = prg.id AND snl.entity_type IN('cre_sys_obt_cur','cre_sys_obt_trn','cre_sys_obt_crt')
		    
					
					
		     LEFT JOIN slt_enrollment_exempted exmp ON exmp.id = (SELECT exp1.id FROM slt_enrollment_exempted exp1 WHERE exp1.enroll_type = 'tp' AND exp1.enrollment_id = masterenroll.id ORDER BY id DESC LIMIT 1)
             LEFT JOIN slt_profile_list_items splt_mro ON mro.mro = splt_mro.code
		     LEFT OUTER JOIN slt_enrollment enr ON enr.master_enrollment_id = masterenroll.id
			 LEFT OUTER JOIN slt_person p ON p.id = masterenroll.user_id
             LEFT OUTER JOIN slt_course_class_session ses ON ses.course_id = enr.course_id and ses.class_id = enr.class_id
             LEFT OUTER JOIN slt_location loc ON loc.id = ses.location_id
             
					
					".$joinQuery."
					
			 LEFT JOIN slt_profile_list_items spli on spli.code = p.time_zone
             LEFT OUTER JOIN slt_profile_list_items pli ON p.time_zone = pli.code

			".$joinQueryfroTags."
					
  			 LEFT JOIN slt_state state on loc.state=state.state_code and loc.country = state.country_code
			WHERE (masterenroll.user_id = ".$user_id.") ";

			
			 //comment code for fetching Tag
			// group_concat(distinct(td.tagname)) as tagname,
			 
			expDebug::dPrint("courseIddddashutosh5454545".print_r($enrollid,true),5);
			
			 if($_REQUEST['apiname'] == 'ListEnrollmentsAPI' && $_REQUEST['mobile_version'])
			 {
			 	$enrollid = $_REQUEST["enroll_id"];
			 	if($enrollid){ //Added condition for survey DeepLink
			 		$enrollQuery .= "AND masterenroll.id = ".$enrollid ;
			 		//$enrollQuery .="AND masterenroll.is_current = 'Y'";
			 	}else{
			 		$enrollQuery .="AND masterenroll.is_current = 'Y'";
			 	}
			 }
			
			
			
		if(!empty($regstatus)) {
			$regstatusArray = explode("|",$regstatus);
			
			expDebug::dPrint("regst arr111" .print_r($regstatusArray,true),5);
			$arr=array();
			
			// Filter  - Completed And attempt_left filter
			
			if(in_array('lrn_tpm_ovr_cmp_atl', $regstatusArray)) {
				$arr[] = "(masterenroll.launchable = 1  and masterenroll.overall_status in ('lrn_tpm_ovr_cmp'))";
			}
		
			
			if(in_array('lrn_tpm_ovr_enr', $regstatusArray) || (in_array('lrn_tpm_ovr_inp', $regstatusArray))){
				$arr[] ="(masterenroll.recertify_path = (select max(recertify_path) from slt_master_enrollment me where me.program_id = masterenroll.program_id and me.user_id = masterenroll.user_id and me.overall_status IN  ('lrn_tpm_ovr_enr','lrn_tpm_ovr_inp')) AND masterenroll.overall_status IN ('lrn_tpm_ovr_enr','lrn_tpm_ovr_inp'))";
			}
			if(in_array('lrn_tpm_ovr_cmp', $regstatusArray)){
				$arr[] ="(masterenroll.recertify_path = (select max(recertify_path) from slt_master_enrollment me where me.program_id = masterenroll.program_id and me.user_id = masterenroll.user_id and me.overall_status = 'lrn_tpm_ovr_cmp') AND masterenroll.overall_status = 'lrn_tpm_ovr_cmp')";
			}
			if(in_array('lrn_tpm_ovr_cln', $regstatusArray)){
				$arr[] ="(masterenroll.recertify_path = (select max(recertify_path) from slt_master_enrollment me where me.program_id = masterenroll.program_id and me.user_id = masterenroll.user_id and me.overall_status = 'lrn_tpm_ovr_cln' ) AND masterenroll.overall_status = 'lrn_tpm_ovr_cln')";
			}
			if(in_array('lrn_tpm_ovr_inc', $regstatusArray)){
				$arr[] ="(masterenroll.recertify_path = (select max(recertify_path) from slt_master_enrollment me where me.program_id = masterenroll.program_id and me.user_id = masterenroll.user_id and me.overall_status = 'lrn_tpm_ovr_inc' ) AND masterenroll.overall_status = 'lrn_tpm_ovr_inc')";
			}
			if(in_array('lrn_tpm_ovr_ppm', $regstatusArray) || in_array('lrn_tpm_ovr_ppv', $regstatusArray) || in_array('lrn_tpm_ovr_wtl', $regstatusArray)){
				$arr[] ="(masterenroll.recertify_path = (select max(recertify_path) from slt_master_enrollment me where me.program_id = masterenroll.program_id and me.user_id = masterenroll.user_id and me.overall_status IN  ('lrn_tpm_ovr_ppm','lrn_tpm_ovr_ppv','lrn_tpm_ovr_wtl')) AND masterenroll.overall_status IN ('lrn_tpm_ovr_ppm','lrn_tpm_ovr_ppv','lrn_tpm_ovr_wtl'))";
			}
			if(in_array('lrn_tpm_ovr_cmp_rereg', $regstatusArray)){
				$arr[] ="(masterenroll.recertify_path = (select max(recertify_path) from slt_master_enrollment me where me.program_id = masterenroll.program_id and me.user_id = masterenroll.user_id and me.overall_status IN  ('lrn_tpm_ovr_exp','lrn_tpm_ovr_cmp')) AND masterenroll.overall_status IN ('lrn_tpm_ovr_exp','lrn_tpm_ovr_cmp'))";
			}
				
			if(count($arr) > 1)
				$condition = implode (" OR ",$arr);
			else{
				$condition = $arr[0];
			}
			
			if(count($arr) > 0)
			$enrollQuery.=' and ('.$condition.')';
			
			expDebug::dPrintDBAPI('getLearningPlanResultSetMobileAPI soun test-- '.$enrollQuery);
			//$selectEnroll2->where('masterenroll.overall_status', $regstatus,'IN');
		} 
		else{
			$regstatus ='lrn_tpm_ovr_enr|lrn_tpm_ovr_inp|lrn_tpm_ovr_cmp|lrn_tpm_ovr_cln|lrn_tpm_ovr_ppv|lrn_tpm_ovr_ppm|lrn_tpm_ovr_wtl|lrn_tpm_ovr_inc';
			$regstatus = str_replace("|","','",$regstatus);
			$regstatus = "'".$regstatus."'";
			$arr = array();
			$arr[] ="(masterenroll.recertify_path = (select max(recertify_path) from slt_master_enrollment me where me.program_id = masterenroll.program_id and me.user_id = masterenroll.user_id and me.overall_status IN  ('lrn_tpm_ovr_enr','lrn_tpm_ovr_inp')) AND masterenroll.overall_status IN ('lrn_tpm_ovr_enr','lrn_tpm_ovr_inp'))";
			$arr[] ="(masterenroll.recertify_path = (select max(recertify_path) from slt_master_enrollment me where me.program_id = masterenroll.program_id and me.user_id = masterenroll.user_id and me.overall_status = 'lrn_tpm_ovr_cmp') AND masterenroll.overall_status = 'lrn_tpm_ovr_cmp')";
			$arr[] ="(masterenroll.recertify_path = (select max(recertify_path) from slt_master_enrollment me where me.program_id = masterenroll.program_id and me.user_id = masterenroll.user_id and me.overall_status = 'lrn_tpm_ovr_cln' ) AND masterenroll.overall_status = 'lrn_tpm_ovr_cln')";
			$arr[] ="(masterenroll.recertify_path = (select max(recertify_path) from slt_master_enrollment me where me.program_id = masterenroll.program_id and me.user_id = masterenroll.user_id and me.overall_status = 'lrn_tpm_ovr_inc' ) AND masterenroll.overall_status = 'lrn_tpm_ovr_inc')";
			$arr[] ="(masterenroll.recertify_path = (select max(recertify_path) from slt_master_enrollment me where me.program_id = masterenroll.program_id and me.user_id = masterenroll.user_id and me.overall_status IN  ('lrn_tpm_ovr_ppm','lrn_tpm_ovr_ppv','lrn_tpm_ovr_wtl')) AND masterenroll.overall_status IN ('lrn_tpm_ovr_ppm','lrn_tpm_ovr_ppv','lrn_tpm_ovr_wtl'))";
			$arr[] ="(masterenroll.recertify_path = (select max(recertify_path) from slt_master_enrollment me where me.program_id = masterenroll.program_id and me.user_id = masterenroll.user_id and me.overall_status IN  ('lrn_tpm_ovr_exp','lrn_tpm_ovr_cmp')) AND masterenroll.overall_status IN ('lrn_tpm_ovr_exp','lrn_tpm_ovr_cmp'))";
				
			$condition2 = implode (" OR ",$arr);
			
			
			$enrollQuery.=' and '.$condition2;
			
		}
			
		if( (isset($_REQUEST["mobile_version"]) && $_REQUEST['apiname'] == 'ListEnrollmentsAPI') )
		{
   		$enrollQuery .=" GROUP BY masterenroll.id ";//code added by siva
		}
			
			
			return $enrollQuery;
	
			}catch (Exception $ex) {
				watchdog_exception('getLearningPlanResultSetMobileAPI', $ex);
				expertusErrorThrow($ex);
			}
	
}
function content_survey_list($enrollId,$userId,$classId) {
try{
	include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_survey/exp_sp_survey_learner/exp_sp_surveylearner.inc');
	$result_survey = getAssignedSurveys($userId,$classId,'cre_sys_obt_cls','survey','0',$enrollId,'yes');
	$attached_Surveys = array();
	if(!empty($result_survey)){
		foreach($result_survey as $survey){
			if($survey->surveycount<=0){
			$attached_Surveys[] = $survey->surveyid ;
			}

		}
	}
	$selectStmt = db_select('slt_survey_mapping', 'sur');
	$selectStmt->leftJoin('slt_attendance_summary','summ','sur.object_id=summ.class_id  AND sur.survey_id = summ.assessment_id AND sur.object_type = "cre_sys_obt_cls"');
	$selectStmt->condition('sur.survey_id',$attached_Surveys ,'IN');
	$selectStmt->condition('summ.enrollment_id',$enrollId ,'=');
	$selectStmt->condition('summ.class_id',$classId ,'=');
	$selectStmt->condition('summ.user_id',$userId ,'=');
	$selectStmt->condition('summ.progress','100' ,'!=');
	$selectStmt->addField('summ', 'progress', 'progress');
	$selectStmt->addField('sur', 'survey_id', 'survey_id');
	$selectStmt->orderBy('sur.survey_id', 'asc');
	$selectStmt->orderBy('summ.progress', 'asc');
	expDebug::dPrintDBAPI('$selectStmt = ', $selectStmt);
	$defaultID = $selectStmt->execute()->fetchAll();
		
	$sendcontid = !empty($defaultID[0]->survey_id)? $defaultID[0]->survey_id :$attached_Surveys[0] ;
	$getdefaultsurveyid = content_playlist_callback($enrollId,1,$sendcontid);
	$result = array(
			'contentid' => $sendcontid,
			'defaultcontent'	   => $getdefaultsurveyid
	);
return  drupal_json_output($result);
}catch (Exception $ex) {
  	watchdog_exception('content_survey_list', $ex);
  	expertusErrorThrow($ex);
  }
}
function program_content_survey_list($masterEnrollId,$userId,$classId) {
try{
	include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_survey/exp_sp_survey_learner/exp_sp_surveylearner.inc');
	$selectTp = db_select('slt_master_enrollment', 'menr');
	$selectTp->innerJoin('slt_program','prg','prg.id=menr.program_id');
	$selectTp->addField('menr', 'user_id', 'user_id');
	$selectTp->addField('menr', 'program_id', 'program_id');
	$selectTp->addField('menr', 'overall_status', 'overall_status');
	$selectTp->addField('prg', 'object_type', 'object_type');
	$selectTp->addField('prg', 'title', 'class_title');
	$selectTp->condition('menr.id', $masterEnrollId);
	$result = $selectTp->execute()->fetchObject();
	$result_survey = getAssignedSurveys($result->user_id, $result->program_id, $result->object_type, 'survey', '0', $masterEnrollId, 'yes');
	$attached_Surveys = array();
	if(!empty($result_survey)){
		foreach($result_survey as $survey){
		if($survey->surveycount<=0){
			$attached_Surveys[] = $survey->surveyid ;
			}
				
		}
	}
	$selectStmt = db_select('slt_survey_mapping', 'sur');
	$selectStmt->leftJoin('slt_attendance_summary','summ','sur.object_id=summ.class_id  AND sur.survey_id = summ.assessment_id AND sur.object_type = "cre_sys_obt_cls"');
	$selectStmt->condition('sur.survey_id',$attached_Surveys ,'IN');
	$selectStmt->condition('summ.enrollment_id',$masterEnrollId ,'=');
	$selectStmt->condition('summ.class_id',$classId ,'=');
	$selectStmt->condition('summ.user_id',$userId ,'=');
	$selectStmt->condition('summ.progress','100' ,'!=');
	$selectStmt->addField('summ', 'progress', 'progress');
	$selectStmt->addField('sur', 'survey_id', 'survey_id');
	$selectStmt->orderBy('sur.survey_id', 'asc');
	$selectStmt->orderBy('summ.progress', 'asc');
	expDebug::dPrintDBAPI('$selectStmt = ', $selectStmt);
	$defaultID = $selectStmt->execute()->fetchAll();

	$sendcontid = !empty($defaultID[0]->survey_id)? $defaultID[0]->survey_id :$attached_Surveys[0] ;
	$getdefaultsurveyid = content_player_program_callback($masterEnrollId,1,$sendcontid);
	$result = array(
			'contentid' => $sendcontid,
			'defaultcontent'	   => $getdefaultsurveyid,
			'programId' => $result->program_id
	);
	return  drupal_json_output($result);
}catch (Exception $ex) {
  	watchdog_exception('program_content_survey_list', $ex);
  	expertusErrorThrow($ex);
  }
}

function getLaunchablecontentAPI(){
	try{
		/*Function to fectch all the launchable content for a class*/
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_survey/exp_sp_survey_learner/exp_sp_surveylearner.inc');
		expDebug::dPrint("regededededstatuscheck11111",5);
		$user_id   	= stripQuotes($_REQUEST['userid']);
		$classId = ($_REQUEST['ClassId']);
		$courseId = ($_REQUEST['courseId']);
		$delType = ($_REQUEST['delivery_type']);
		$callType = $_REQUEST['callType'];
		$objectType = 'cre_sys_obt_cls';
		$enrollId = $_REQUEST['enrollId'];


		if($delType == 'lrn_cls_dty_wbt' || $delType == 'lrn_cls_dty_vod'){
			$content = getLaunchDetail($enrollId);
			//expDebug::dPrint("contentcontentcontentcontentcontentcontent1111".print_r($content,1),5);
				
			$class_status = db_query("select comp_status from slt_enrollment where id =".$enrollId);
			$cls_status = $class_status->fetchAll();
			$content[0]['cls_status'] = $cls_status[0]->comp_status;
			expDebug::dPrint("alllclassstatus55555".print_r($cls_status[0]->comp_status,1),5);
			expDebug::dPrint("contentcontentcontentcontentcontentcontent1111".print_r($content,1),5);
				
				
		}
		$preassessment = getAssignedSurveys ( $user_id, $classId, 'cre_sys_obt_cls', 'assessment', 1, $enrollId, 'yes' );
		$postassessment = getAssignedSurveys ( $user_id, $classId, 'cre_sys_obt_cls', 'assessment', 0, $enrollId, 'yes' );
		$survey = getAssignedSurveys ( $user_id, $classId, 'cre_sys_obt_cls', 'survey', '1', $enrollId, 'yes' );
		$availableclasses = '';
		$availableclasses->launch = $content;
		$availableclasses->preassessment = $preassessment;
		$availableclasses->postassessment = $postassessment;
		$availableclasses->survey = $survey;
			
		if($availableclasses->preassessment[0]->surveyid){
			$class_pre_progress = db_query("select progress from slt_attendance_summary where assessment_id = ".$availableclasses->preassessment[0]->surveyid." and enrollment_id = ".$enrollId);
			$progress_pre = $class_pre_progress->fetchAll();
			$availableclasses->preassessment[0]->progress = $progress_pre[0]->progress;
			expDebug::dPrint("alllaunchablecontentresultssssss111".print_r($progress_pre[0]->progress,1),5);
		}
			
		if($availableclasses->postassessment[0]->surveyid){
			$class_post_progress = db_query("select progress from slt_attendance_summary where assessment_id = ".$availableclasses->postassessment[0]->surveyid." and enrollment_id = ".$enrollId);
			$progress_post = $class_post_progress->fetchAll();
			$availableclasses->postassessment[0]->progress = $progress_post[0]->progress;
		}
			
		if($availableclasses->survey[0]->surveyid){
			for($s=0;$s<count($availableclasses->survey);$s++){
				$surveyId = $availableclasses->survey[$s]->surveyid;
				$class_survey_progress = db_query("select progress from slt_attendance_summary where assessment_id = ".$surveyId." and enrollment_id = ".$enrollId);
				$progress_survey = $class_survey_progress->fetchAll();
				$availableclasses->survey[$s]->progress = $progress_survey[0]->progress;
			}
		}
			
		$content_details = content_playlist_callback($enrollId,1);
			
		$ClassDelType = getClassDeliveryType($classId, 'Short');
			
		expDebug::dPrint("delcodecodecodecode33333".print_r($ClassDelType,1),5);
			
		if($ClassDelType == 'ilt' || $ClassDelType == 'vc'){
			$session_detail = getClassSessionsDateTime($record->class_id);
			$sccs_date_and_time = implode(' ', array($session_detail[0][date],$session_detail[0][end_time]));
			$curr_date_and_time =  date('Y-m-d H:i',strtotime("-30 minutes"));
			if (strtotime($curr_date_and_time) >= strtotime($sccs_date_and_time)) {
				$availableclasses->postassessment[0]->ilt_sessionCheck = 'Session_completed';
			}else{
				$availableclasses->postassessment[0]->ilt_sessionCheck = 'Session_not_completed';
			}
				
		}
			
			
		expDebug::dPrint("alllaunchablecontentresultssssss".print_r($content_details,1),5);
		return $availableclasses;
	}catch (Exception $ex) {
		watchdog_exception('getLaunchablecontentAPI', $ex);
		expertusErrorThrow($ex);
	}
}

?>
