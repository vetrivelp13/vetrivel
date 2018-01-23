<?php
require_once DRUPAL_ROOT . '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrenrollment/exp_sp_lnrenrollment.inc';
function getLnrLearningPlanResultSet(){
	try {
		$user_id = stripQuotes($_GET['UserID']);
		/*
		 * To handle Sql Injection without session cookie (anonymous users)
		 * Condition Added by : Vimal DM
		 */
		$sltPersonUserId = getSltpersonUserId();
		$getmandatory = getmandatory($user_id, 'cre_sys_obt_trn');
		$getLPmandatory = getLPmandatory($sltPersonUserId, 'cre_sys_obt_trn');
		$isCompliance = isCompliance($user_id);
		if($sltPersonUserId != $user_id) {
			throw new Exception('Userid and Logged in user doesnot match.');
		}
		$userTimezone = getUserTimezone();
		/*
		 * $regstatus = stripRegStatus($_GET['regstatuschk']);
		 * $tpSearchText = $_REQUEST['tpSearchText'];
		 * $price = $_REQUEST['price'];
		 * $tpobjecttype = $_REQUEST['tpobjecttype'];
		 * $tptrainingtype = $_REQUEST['tptrainingtype'];
		 * $assignedBy = $_REQUEST['assignedBy'];
		 * $location = $_REQUEST['location'];
		 * $scheduledDays = $_REQUEST['$scheduledDays'];
		 * $dueDays = 30;
		 */
		$timeZone = date_default_timezone(false);
		$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
		
		$regstatus = stripRegStatus($_GET['regstatuschk']);
		$tpSearchText = $_REQUEST['searchText'];
		$deliveryType = stripRegStatus(stripApostrophe($_REQUEST['del_type']));
		$trainingType = stripRegStatus(stripApostrophe($_REQUEST['tra_type']));
		$price = $_REQUEST['price'];
		$location = $_REQUEST['location'];
		$tpScheduled = $_REQUEST['scheduled'];
		$tpreg = $_REQUEST['reg'];
		$tpDue = $_REQUEST['due'];
		$assignedBy = $_REQUEST['assignedby'];
		$config = getConfig("exp_sp");
		$cert_days_expire = isset($config["cert_expire_reminder_days"]) ? $config["cert_expire_reminder_days"] : 10;
		/* Paging Related */
		$count = 0;
		$page = $_GET['page'];
		$limit = $_GET['rows'];
		$sidx = stripQuotes($_GET['sidx']);
		$sord = stripApostrophe($_GET['sord']);
		// $select = db_select('slt_master_enrollment', 'masterenroll');
		// $select->addExpression('COUNT(masterenroll.id)', 'count');
		$and = db_and();
		if(isset($_GET['enr_id'])) {
			$enroll = explode('_', $_GET['enr_id']);
			$and->condition('masterenroll.id', escape_string($enroll[1]), '=');
		}
		expDebug::dPrint('get tp enroll details '.print_r($_GET, 1), 5);
		if(!empty($user_id)) {
			$and->condition('masterenroll.user_id', $user_id, '=');
		}
		
		// Filter - Training type
		if(!empty($trainingType)) {
			$trainingType = explode("|", $trainingType);
			expDebug::dPrint("soun check" . $trainingType, 5);
			$and->condition(db_or()->condition('usrmro.mro', $trainingType, 'IN')
				->condition('mro.mro', $trainingType, 'IN'));
		}
		// Filter - Delivery type
		if(!empty($deliveryType)) {
			$deliveryType = explode("|", $deliveryType);
			$and->condition('prg.object_type', $deliveryType, 'IN');
		}
		// Filter - Price
		if(!empty($price)) {
			if($price == 'free') {
				$and->condition('prg.price', '0.00');
			} else if($price == 'priced') {
				$and->condition('prg.price', '0.00', '!=');
			}
		}
		if(!empty($location)) {
			if(($_REQUEST['selectedLocID'])) {
				$and->condition('loc.id', $_REQUEST['selectedLocID'], '=');
			} else {
				$and->where("CONCAT(loc.name,IF(loc.addr1 IS NOT NULL ,CONCAT(',',loc.addr1),''),IF(loc.addr2 IS NOT NULL ,CONCAT(',',loc.addr2),''),loc.city,loc.zipcode,state.state_name) LIKE '%" . addslashes($location) . "%'");
			}
		}
		if(!empty($tpreg)) {
			$and->where('CONVERT_TZ(masterenroll.reg_date,pli.attr2,"' . $userTimezone . '") >= DATE_SUB(CONVERT_TZ(NOW(),pli.attr2,"' . $userTimezone . '"),INTERVAL "' . $tpreg . '" DAY)');
		}
		if(!empty($tpScheduled)) {
			$and->where("DATE_FORMAT(CONVERT_TZ(CONCAT( DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.start_time,':00'),spl.attr2,'$userTimezone'),'%Y-%m-%d %H:%i:%s')
        		BETWEEN CONVERT_TZ(NOW(),spl.attr2,'$userTimezone') AND DATE_ADD(convert_tz(NOW(),spl.attr2,'$userTimezone'),interval $tpScheduled day)");
		}
		// Filter - Assigned By
		if(!empty($assignedBy)) {
			if($assignedBy == 'cre_sys_fop_me') {
				$and->condition('masterenroll.created_by', $user_id, '=');
			} else if($assignedBy == 'cre_sys_fop_mgr') {
				$and->where("masterenroll.created_by = p.manager_id OR masterenroll.created_by IN (SELECT manager_id from slt_person_other_manager where user_id=".$user_id.")");
			} else if($assignedBy == 'cre_sys_fop_oth') {
				$and->where('masterenroll.created_by !="' . $user_id . '" and if(p.manager_id IS NOT NULL ,masterenroll.created_by != p.manager_id,1=1) AND masterenroll.created_by NOT IN (SELECT manager_id from slt_person_other_manager where user_id="'.$user_id.'")');
			}
		}
		
		/*
		 * $count = $select->execute()->fetchField();
		 * if( $count >0 ) {
		 * $total_pages = ceil($count/$limit);
		 * } else {
		 * $total_pages = 0;
		 * }
		 *
		 * if ($page > $total_pages) $page=$total_pages;
		 * $start = $limit*$page - $limit;
		 * if($start < 0) {
		 * $start = 0;
		 * }
		 */
		
		$groupIdArray = getGroupsDetails($user_id,'','','',0);
		$tmp = array();
		foreach($groupIdArray as $b)
			$tmp[] = $b->id; // pull out data
		$groupIds = implode("','", $tmp); // and implode
		expDebug::dPrint(" groupIds" . print_r($groupIds, true), 5);
		
		$selectEnroll = db_select('slt_master_enrollment', 'masterenroll');
		$selectEnroll->join('slt_program', 'prg', 'prg.id = masterenroll.program_id');
		$selectEnroll->join('slt_node_learning_activity', 'snl', "snl.entity_id = prg.id AND snl.entity_type IN('cre_sys_obt_cur','cre_sys_obt_trn','cre_sys_obt_crt')");
		$selectEnroll->join('slt_enrollment', 'enr', 'enr.master_enrollment_id = masterenroll.id');
		$selectEnroll->leftjoin('slt_course_content_mapper', 'sccm', 'enr.course_id = sccm.course_id and enr.class_id = sccm.class_id');
		$selectEnroll->join('slt_person', 'p', 'p.id = masterenroll.user_id');
		// $selectEnroll -> leftjoin('slt_mro_mapping', 'mro', "mro.entity_id = masterenroll.program_id AND mro.entity_type IN('cre_sys_obt_cur','cre_sys_obt_trn','cre_sys_obt_crt') AND mro.job_role_id = (SELECT job_role FROM slt_person WHERE id = ".$user_id.")");
		$selectEnroll->leftjoin('slt_group_mapping', 'mro', "mro.entity_id = masterenroll.program_id AND mro.entity_type IN('cre_sys_obt_cur','cre_sys_obt_trn','cre_sys_obt_crt') AND mro.group_id IN('$groupIds')");
		$selectEnroll->leftjoin('slt_user_access_mapping','usrmro',"usrmro.entity_id = masterenroll.program_id AND usrmro.entity_type IN ('cre_sys_obt_cur' , 'cre_sys_obt_trn', 'cre_sys_obt_crt') AND usrmro.user_id = masterenroll.user_id");
		// $selectEnroll -> leftJoin('slt_groups', 'grp', "grp.id = mro.group_id AND grp.job_role = (SELECT job_role FROM slt_person WHERE id =".$user_id.")");
		$selectEnroll->leftJoin('slt_groups', 'grp', "grp.id = mro.group_id AND grp.job_role IN (select group_concat(job_role) from slt_person_jobrole_mapping where user_id =" . $user_id . ")");
		$selectEnroll->leftjoin('slt_attendance_summary', 'sam', 'sam.enrollment_id = enr.id');
		$selectEnroll->leftjoin('slt_course_class_session', 'ses', 'ses.course_id = enr.course_id and ses.class_id = enr.class_id');
		$selectEnroll->leftjoin('slt_profile_list_items', 'pli', 'p.time_zone = pli.code');
		
		if(!empty($location)) {
			$selectEnroll->leftjoin('slt_location', 'loc', 'loc.id = ses.location_id');
			$selectEnroll->leftjoin('slt_state', 'state', 'loc.state = state.state_code and loc.country = state.country_code');
		}
		if(!empty($tpScheduled)) {
			$selectEnroll->leftjoin('slt_profile_list_items', 'spl', 'ses.timezone = spl.code');
		}
		if(!empty($tpSearchText)) {
			$selectEnroll->leftjoin('slt_tag_entity', 'ste', 'ste.entity_id = prg.id');
			$selectEnroll->leftjoin('slt_tagdefn', 'td', 'ste.tagid = td.id and (td.entity_type IN  ("Certification","Curricula","Learning Plan")) and (td.activity_count > 0)');
		}
		
		$selectEnroll->fields('masterenroll');
		$selectEnroll->addField('prg', 'id', 'programid');
		$selectEnroll->addField('prg', 'type', 'basetype');
		$selectEnroll->addField('prg', 'title', 'prg_title');
		$selectEnroll->addField('prg', 'code', 'prg_code');
		$selectEnroll->addField('prg', 'description', 'prg_desc');
		$selectEnroll->addField('prg', 'short_desc', 'prg_shortdesc');
		$selectEnroll->addField('prg', 'start_date', 'prg_start_date');
		$selectEnroll->addField('prg', 'end_date', 'prg_end_date');
		$selectEnroll->addField('prg', 'status', 'prg_status');
		$selectEnroll->addField('prg', 'object_type', 'prg_object_type');
		$selectEnroll->addField('prg', 'expires_in_value', 'prg_expires_in_value');
		$selectEnroll->addField('prg', 'expires_in_unit', 'prg_expires_in_unit');
		//$selectEnroll->addExpression("slf_get_content_postass_total_attemps(prg.id)", 'max_attempts');
  		//$selectEnroll->addExpression('slf_get_attempts_left(masterenroll.id)', 'total_attempts');
		$selectEnroll->addExpression("IF(usrmro.mro is not null,usrmro.mro,mro.mro)", "mro");
		// $selectEnroll->addField('masterenroll', 'recertify_path', 'recertify_path'); //Commented this line for duplicate column error
		$selectEnroll->addField('masterenroll', 'mandatory', 'master_mandatory');
		$selectEnroll->addField('masterenroll', 'overall_status', 'masterenroll_overall_status');
		// $selectEnroll->addField('masterenroll', 'created_by', 'created_by');//Commented this line for duplicate column error
		// $selectEnroll->addField('masterenroll', 'updated_by', 'updated_by');//Commented this line for duplicate column error
		$selectEnroll->addField('snl', 'node_id', 'node_id');
		$selectEnroll->addField('pli', 'attr2', 'attr2');
		$selectEnroll->addField('prg', 'end_date', 'due_date');
		// $selectEnroll->addField('masterenroll', 'is_current', 'is_current');//Commented this line for duplicate column error
		$selectEnroll->addExpression("if(mro.mro = 'cre_sys_inv_opt',3,if(mro.mro = 'cre_sys_inv_man',1,2))", "ascen");
		$selectEnroll->addExpression("(SELECT CONCAT(exmp.exempted_status,'|',exmp.created_by,'|',exmp.created_on) FROM slt_enrollment_exempted exmp WHERE exmp.enrollment_id = masterenroll.id AND exmp.enroll_type = 'tp' ORDER BY id DESC LIMIT 1)", 'exempted_sts');
		$selectEnroll->addExpression("(SELECT CONCAT(exmp.exempted_status,'|',exmp.created_by,'|',exmp.created_on) FROM slt_enrollment_exempted exmp WHERE exmp.enrollment_id = masterenroll.id AND exmp.enroll_type = 'tp' ORDER BY id DESC LIMIT 1)", 'exempted_sts');
	/* 	$selectEnroll->addExpression("case when prg.expires_in_unit='days' then DATE_ADD(CONVERT_TZ(masterenroll.reg_date,pli.attr2,'$userTimezone'), interval prg.expires_in_value day)
		  when prg.expires_in_unit='months' then DATE_ADD(CONVERT_TZ(masterenroll.reg_date,pli.attr2,'$userTimezone'), interval prg.expires_in_value month)
		  when prg.expires_in_unit='years' then DATE_ADD(CONVERT_TZ(masterenroll.reg_date,pli.attr2,'$userTimezone'), interval prg.expires_in_value YEAR)
		  WHEN prg.end_date IS NOT NULL THEN prg.end_date  
		  END", "due_date"); */
		if(!empty($tpSearchText)) {
			$selectEnroll->addExpression('GROUP_CONCAT(td.tagname)', 'tagname');
		}	
		$selectEnroll->condition($and);
		$selectEnroll->groupBy('masterenroll.id');
		$selectEnroll->orderBy("ascen");
		$selectEnroll2 = db_select($selectEnroll, "acc");
		$selectEnroll2->fields('acc');
		
		if(isset($_GET["sortBy"])) {
			$sortBy = $_GET["sortBy"];
			$sidx = '';
			if($sortBy == 'orderbystatus') {
				$prgSts = getStatusFilters('myprograms');
				$sortbyStatusArray = explode("|",$prgSts);
				if(!empty($prgSts)){
					if(in_array('EnrollmentLP', $sortbyStatusArray)) {
						$selectEnroll2->orderBy('acc.masterenroll_overall_status in (\'lrn_tpm_ovr_enr|\',\'lrn_tpm_ovr_inp\')', 'desc');
					}
					if(in_array('EnrollLPCompleted', $sortbyStatusArray)) {
						$selectEnroll2->orderBy('acc.masterenroll_overall_status in (\'lrn_tpm_ovr_cmp\')', 'desc');
					}
// 					if(in_array('EnrollLPCompletedWithAttempts', $sortbyStatusArray)) {
// 						$selectEnroll2->orderBy('((acc.max_attempts-acc.total_attempts) > 0 and (acc.masterenroll_overall_status in ("lrn_tpm_ovr_cmp")))', 'desc');
// 					}
					if(in_array('EnrollLPReregister', $sortbyStatusArray)) {
						$selectEnroll2->orderBy('(IF(acc.prg_expires_in_unit = \'days\', DATE_SUB(DATE_ADD(acc.comp_date,interval acc.prg_expires_in_value DAY),INTERVAL "' . $cert_days_expire . '" DAY) <= NOW() ,
						(IF(acc.prg_expires_in_unit = \'months\',DATE_SUB(DATE_ADD(acc.comp_date,interval acc.prg_expires_in_value MONTH),INTERVAL "' . $cert_days_expire . '" DAY) <= NOW(),
						DATE_SUB(DATE_ADD(acc.comp_date,interval acc.prg_expires_in_value YEAR),INTERVAL "' . $cert_days_expire . '" DAY) <= NOW()))) AND (acc.prg_object_type IN (\'cre_sys_obt_crt\')) AND acc.masterenroll_overall_status IN  (\'lrn_tpm_ovr_cmp\',\'lrn_tpm_ovr_exp\'))', 'desc');
					}
					if(in_array('EnrollLPInCompleted', $sortbyStatusArray)) {
						$selectEnroll2->orderBy('acc.masterenroll_overall_status in (\'lrn_tpm_ovr_inc\')', 'desc');
					}
					if(in_array('EnrollLPCanceled', $sortbyStatusArray)) {
						$selectEnroll2->orderBy('acc.masterenroll_overall_status in (\'lrn_tpm_ovr_cln\')', 'desc');
					}
					if(in_array('EnrollLPPayments', $sortbyStatusArray)) {
						$selectEnroll2->orderBy('acc.masterenroll_overall_status in (\'lrn_tpm_ovr_ppv\',\'lrn_tpm_ovr_ppm\',\'lrn_tpm_ovr_wtl\')', 'desc');
					}
			    }else{
			    	$selectEnroll2->orderBy('acc.masterenroll_overall_status', 'desc');
			    }
			}
			if($sortBy == 'AZ') {
				$selectEnroll2->orderBy('acc.prg_title', 'asc');
			}
			if($sortBy == 'ZA') {
				$selectEnroll2->orderBy('acc.prg_title', 'desc');
			}
			if($sortBy == 'dateOld') {
				$selectEnroll2->orderBy('acc.id', 'desc');
			}
			if($sortBy == 'dateNew') {
				$selectEnroll2->orderBy('acc.id', 'asc');
			}
			if($sortBy == 'type') {
				$selectEnroll2->orderBy('acc.prg_object_type', 'asc');
			}
			if($sortBy == 'Mandatory') {
				if($getmandatory > 0 || $getLPmandatory > 0) {
					$selectEnroll2->orderBy('FIELD(acc.mro, \'cre_sys_inv_man\') ', 'desc');
					$selectEnroll2->orderBy('(acc.master_mandatory = \'Y\')', 'desc');
					$selectEnroll2->orderBy('FIELD(acc.mro, \'cre_sys_inv_opt\', \'cre_sys_inv_rec\') ', 'desc');
				} else {
					$selectEnroll2->orderBy('(acc.master_mandatory = \'Y\')', 'desc');
				}
			}
		} else {
			if($getmandatory > 0) {
				$selectEnroll2->orderBy('FIELD(acc.mro, \'cre_sys_inv_man\') ', 'desc');
			}
			if($getLPmandatory > 0) {
				$selectEnroll2->orderBy('(acc.master_mandatory = \'Y\')', 'desc');
				$selectEnroll2->orderBy('FIELD(acc.mro, \'cre_sys_inv_opt\', \'cre_sys_inv_rec\') ', 'desc');
			} else {
				// $selectEnroll2->orderBy('(masterenroll.mandatory = \'Y\')', 'desc');
				// $selectEnroll2->orderBy('FIELD(mro, \'cre_sys_inv_opt\', \'cre_sys_inv_rec\') ','desc');
				// $selectEnroll2->orderBy('(masterenroll.is_current = \'Y\')', 'desc');
				$selectEnroll2->orderBy('acc.id', 'desc');
			}
		}
		/*
		 * if(!empty($regstatus) && ($regstatus != 'lrn_tpm_ovr_cmp_atl' && ($regstatus != 'lrn_tpm_ovr_cmp_rec'))){
		 * $regstatus = explode("|",$regstatus);
		 * $and->condition('masterenroll.overall_status', $regstatus,'IN');
		 * }
		 */
		$selectEnroll2->where("acc.is_current = 'Y'");
		if(!isset($_GET['enr_id']) && !empty($regstatus)) {
			$or = db_or();
			$regstatusArray = explode("|", $regstatus);
			// Filter - Completed And need recertify
			$arr = array();
			// $arr[]= "(acc.is_current = 'Y')";
			
			if(in_array('lrn_tpm_ovr_cmp_atl', $regstatusArray)) {
				// $arr[] = '((acc.max_attempts-acc.total_attempts) > 0 and (acc.masterenroll_overall_status in ("lrn_tpm_ovr_cmp")))';
				$arr[] = '((acc.launchable = 1)  and (acc.masterenroll_overall_status in ("lrn_tpm_ovr_cmp")))';
			}
			if(in_array('lrn_tpm_ovr_cmp_rereg', $regstatusArray)) {
				$arr[] = "((acc.masterenroll_overall_status = 'lrn_tpm_ovr_exp') OR (IF(acc.prg_expires_in_unit = 'days', DATE_SUB(DATE_ADD(acc.comp_date,interval acc.prg_expires_in_value DAY),INTERVAL $cert_days_expire DAY) <= NOW() ,
					(IF(acc.prg_expires_in_unit = 'months',DATE_SUB(DATE_ADD(acc.comp_date,interval acc.prg_expires_in_value MONTH),INTERVAL $cert_days_expire DAY) <= NOW(),
					DATE_SUB(DATE_ADD(acc.comp_date,interval acc.prg_expires_in_value YEAR),INTERVAL $cert_days_expire DAY) <= NOW()))) AND (acc.prg_object_type IN ('cre_sys_obt_crt')) 
					AND acc.masterenroll_overall_status = 'lrn_tpm_ovr_cmp'))";
			}
			
			expDebug::dPrint("regst arr" . print_r($regstatusArray, true), 5);
			if(in_array('lrn_tpm_ovr_enr', $regstatusArray) || (in_array('lrn_tpm_ovr_inp', $regstatusArray))) {
				$arr[] = "(acc.masterenroll_overall_status IN ('lrn_tpm_ovr_enr','lrn_tpm_ovr_inp'))";
			}
			
			if(in_array('lrn_tpm_ovr_cmp', $regstatusArray)) {
				$arr[] = "(acc.masterenroll_overall_status = 'lrn_tpm_ovr_cmp')";
			}
			if(in_array('lrn_tpm_ovr_cln', $regstatusArray)) {
				$arr[] = "(acc.masterenroll_overall_status = 'lrn_tpm_ovr_cln')";
			}
			if(in_array('lrn_tpm_ovr_inc', $regstatusArray)) {
				$arr[] = "(acc.masterenroll_overall_status = 'lrn_tpm_ovr_inc')";
			}
			if(in_array('lrn_tpm_ovr_ppm', $regstatusArray) || in_array('lrn_tpm_ovr_ppv', $regstatusArray) || in_array('lrn_tpm_ovr_wtl', $regstatusArray)) {
				$arr[] = "(acc.masterenroll_overall_status IN ('lrn_tpm_ovr_ppm','lrn_tpm_ovr_ppv','lrn_tpm_ovr_wtl'))";
			}
			
			if(count($arr) > 0) {
				$condition = count($arr) > 1 ? implode(" OR ", $arr) : $arr[0];
				$selectEnroll2->where($condition);
			}
			
			expDebug::dPrintDBAPI('getLnrLearningPlanResultSet soun test--', $selectEnroll2);
			// $selectEnroll2->where('masterenroll.overall_status', $regstatus,'IN');
		} else {
			$regstatus = 'lrn_tpm_ovr_enr|lrn_tpm_ovr_inp|lrn_tpm_ovr_cmp|lrn_tpm_ovr_cln|lrn_tpm_ovr_ppv|lrn_tpm_ovr_ppm|lrn_tpm_ovr_wtl|lrn_tpm_ovr_inc';
			$regstatus = str_replace("|", "','", $regstatus);
			$regstatus = "'" . $regstatus . "'";
			$arr = array();
			$arr[] = "(acc.masterenroll_overall_status IN ('lrn_tpm_ovr_enr','lrn_tpm_ovr_inp'))";
			$arr[] = "(acc.masterenroll_overall_status = 'lrn_tpm_ovr_cmp')";
			$arr[] = "(acc.masterenroll_overall_status = 'lrn_tpm_ovr_cln')";
			$arr[] = "(acc.masterenroll_overall_status = 'lrn_tpm_ovr_inc')";
			$arr[] = "(acc.masterenroll_overall_status IN ('lrn_tpm_ovr_ppm','lrn_tpm_ovr_ppv','lrn_tpm_ovr_wtl'))";
			$arr[] = "((acc.masterenroll_overall_status = 'lrn_tpm_ovr_exp') OR(IF(acc.prg_expires_in_unit = 'days', DATE_SUB(DATE_ADD(acc.comp_date,interval acc.prg_expires_in_value DAY),INTERVAL $cert_days_expire DAY) <= NOW() ,
                    (IF(acc.prg_expires_in_unit = 'months',DATE_SUB(DATE_ADD(acc.comp_date,interval acc.prg_expires_in_value MONTH),INTERVAL $cert_days_expire DAY) <= NOW(),
                    DATE_SUB(DATE_ADD(acc.comp_date,interval acc.prg_expires_in_value YEAR),INTERVAL $cert_days_expire DAY) <= NOW()))) AND (acc.prg_object_type IN ('cre_sys_obt_crt')) AND acc.masterenroll_overall_status = 'lrn_tpm_ovr_cmp' AND acc.is_current = 'Y'))";
			//$arr[] = "((acc.max_attempts-acc.total_attempts) > 0 and (acc.masterenroll_overall_status = 'lrn_tpm_ovr_cmp'))";
			
			$condition2 = implode(" OR ", $arr);
			
			$selectEnroll2->where($condition2);
		}
		if(!empty($tpDue)) {
			$selectEnroll2->where('acc.due_date BETWEEN CONVERT_TZ(NOW(),"'.$defaultTimezone.'","' . $userTimezone . '") AND DATE_ADD(CONVERT_TZ(NOW(),"'.$defaultTimezone.'","' . $userTimezone . '"),interval ' . $tpDue . ' day) AND acc.masterenroll_overall_status NOT IN ( "lrn_tpm_ovr_cln" , "lrn_tpm_ovr_inc","lrn_tpm_ovr_cmp")');
		}
		if(!empty($tpSearchText)) {
			$selectEnroll2->where('acc.prg_title like "%' . $tpSearchText . '%" OR acc.prg_code like "%' . $tpSearchText . '%" OR acc.prg_desc like "%' . $tpSearchText . '%"
			OR acc.prg_shortdesc like "%' . $tpSearchText . '%" or acc.tagname like "%' . $tpSearchText . '%"');
		}
		
		if($sidx != '') {
			$selectEnroll2->orderBy($sidx, $sord);
		}
		
		$selectEnroll2->groupBy('acc.id');
		//APIs and Mobile APP need the total number of results and the total number of avaialble pages for the given filter criteria
		if(isset($_REQUEST['mobile_version']) || isset($_REQUEST['apiname'])) {
			$countQuery = $selectEnroll2->countQuery();
			
			// $cntQry = $selectEnroll2->execute()->fetchAll();
			// expDebug::dPrint("Enrollment Query Fetch DATA count here :-");
			// expDebug::dPrint($cntQry , 4);
			// $selectEnrCnt = count($cntQry);
			
			// expDebug::dPrint("count query checking first ".$selectEnrCnt,5);
			// $totRecQry = db_query('select found_rows() as tot_records');
			expDebug::dPrintDBAPI('learning plan count query ', $countQuery);
			// $count1 = $totRecQry->fetchField();
			$count1 = $countQuery->execute()->fetchField();
			expDebug::dPrint('learning plan found total rows ' . $count1, 5);
			if($count != $count1) {
				$count = $count1;
				if($count > 0) {
					$total_pages = ceil($count / $limit);
				} else {
					$total_pages = 0;
				}
				if($page > $total_pages) {
					$page = $total_pages;
				}
				$start = $limit * $page - $limit;
				if($start < 0) {
					$start = 0;
				}
			}
		} else {
			//Data fetch request from UI/Web doesn't need total number of results and hence count query execution is not needed
			$start = calculate_offset($page, $limit);
			$count = $limit;
		}
		$pagination = array();
		$selectEnroll2->range($start, $limit);
		expDebug::dPrintDBAPI('getLnrLearningPlanResultSet --', $selectEnroll2);
		$lpQryRes = $selectEnroll2->execute()->fetchAll();
		expDebug::dPrint('select result of tp '.print_r($lpQryRes, 1), 5);
		$pagination['result'] = $lpQryRes;
		$pagination['page'] = $page;
		$pagination['count'] = $count;
		$pagination['total_pages'] = $total_pages;
		
		return $pagination;
	} catch(Exception $ex) {
		watchdog_exception('getLnrLearningPlanResultSet', $ex);
		expertusErrorThrow($ex);
	}
}

function getCertificationModules($prgId,$is_API=0){
	try{		
		$userId = getSltpersonUserId();
		$module_seq = getModuleId($userId,$prgId);

		if(count($module_seq)>0) {						
			$module_path = $module_seq[0]->recertify_path;				
		} else {		
			$module_path = '1'; 
		}

		
		$user_id   	= stripQuotes($_GET['UserID']);
		/*
		 *  To handle Sql Injection without session cookie (anonymous users)
		* Condition Added by : Vimal DM
		*/
		$sltPersonUserId = getSltpersonUserId();
		$getmandatory = getmandatory($user_id,'cre_sys_obt_trn');
		$getLPmandatory = getLPmandatory($sltPersonUserId,'cre_sys_obt_trn');
		$isCompliance = isCompliance($user_id);
		if($sltPersonUserId!=$user_id){
			throw new Exception('Userid and Logged in user doesnot match.');
		}
		//$regstatus  	= stripRegStatus($_GET['regstatuschk']);

		/* Paging Related */
		$count = 0;
		$page      = $_GET['page'];
		$limit     = $_GET['rows'];
		$sidx      = stripQuotes($_GET['sidx']);
		$sord      = stripApostrophe($_GET['sord']);
		$select = db_select('slt_master_enrollment', 'masterenroll');
		$select->addExpression('COUNT(masterenroll.id)', 'count');
		$and = db_and();
		if(!empty($user_id)){
			$and->condition('masterenroll.user_id', $user_id,'=');
		}
		$select->condition($and);
		$count = $select->execute()->fetchField();
		if( $count >0 ) {
			$total_pages = ceil($count/$limit);
		} else {
			$total_pages = 0;
		}

		if ($page > $total_pages) $page=$total_pages;
		$start = $limit*$page - $limit;
		if($start < 0) {
			$start = 0;
		}

		$groupIdArray = getGroupsDetails($user_id,'','','',0);
		$tmp = array();
		foreach ($groupIdArray as $b) $tmp[] = $b->id; //pull out data
		$groupIds = implode("','", $tmp); //and implode
		expDebug::dPrint(" groupIds".print_r($groupIds,true),5);

		$selectEnroll = db_select('slt_master_enrollment', 'masterenroll');
		$selectEnroll -> join('slt_program', 'prg', 'prg.id = masterenroll.program_id');
		//$selectEnroll -> leftjoin('slt_mro_mapping', 'mro', "mro.entity_id = masterenroll.program_id AND mro.entity_type IN('cre_sys_obt_cur','cre_sys_obt_trn','cre_sys_obt_crt') AND mro.job_role_id = (SELECT job_role FROM slt_person WHERE id = ".$user_id.")");
		$selectEnroll -> leftjoin('slt_group_mapping', 'mro', "mro.entity_id = masterenroll.program_id AND mro.entity_type IN('cre_sys_obt_cur','cre_sys_obt_trn','cre_sys_obt_crt') AND mro.group_id IN('$groupIds') and if((mro.entity_type='cre_sys_obt_crt'),masterenroll.is_current = 'N',masterenroll.is_current = 'Y')");
		//$selectEnroll -> leftJoin('slt_groups', 'grp', "grp.id = mro.group_id AND grp.job_role = (SELECT job_role FROM slt_person WHERE id =".$user_id.")");
		$selectEnroll -> leftJoin('slt_groups', 'grp', "grp.id = mro.group_id AND grp.job_role IN (select group_concat(job_role) from slt_person_jobrole_mapping where user_id =".$user_id.")");
		$selectEnroll -> leftjoin('slt_node_learning_activity', 'snl', "snl.entity_id = prg.id AND snl.entity_type IN('cre_sys_obt_cur','cre_sys_obt_trn','cre_sys_obt_crt')");
		
		$selectEnroll->innerJoin('slt_module_crs_mapping', 'crsmap', 'crsmap.program_id = prg.id');
		//$selectEnroll->innerJoin('slt_module','module','module.id=crsmap.module_id');
		
		$selectEnroll->fields('masterenroll');
		$selectEnroll->addField('prg', 'id', 'programid');
		$selectEnroll->addField('prg', 'type', 'basetype');
		$selectEnroll->addField('prg', 'title', 'prg_title');
		$selectEnroll->addField('prg', 'code', 'prg_code');
		$selectEnroll->addField('prg', 'description', 'prg_desc');
		$selectEnroll->addField('prg', 'short_desc', 'prg_shortdesc');
		$selectEnroll->addField('prg', 'start_date', 'prg_start_date');
		$selectEnroll->addField('prg', 'end_date', 'prg_end_date');
		$selectEnroll->addField('prg', 'status', 'prg_status');
		$selectEnroll->addField('prg', 'object_type', 'prg_object_type');
		$selectEnroll->addField('prg', 'expires_in_value', 'prg_expires_in_value');
		$selectEnroll->addField('prg', 'expires_in_unit', 'prg_expires_in_unit');
		// $selectEnroll->addField('masterenroll', 'recertify_path', 'recertify_path'); //Commented this line for duplicate column error
		$selectEnroll->addField('masterenroll', 'mandatory', 'master_mandatory');
		//  $selectEnroll->addField('masterenroll', 'created_by', 'created_by');//Commented this line for duplicate column error
		// $selectEnroll->addField('masterenroll', 'updated_by', 'updated_by');//Commented this line for duplicate column error
		$selectEnroll->addField('mro', 'mro', 'mro');
		$selectEnroll->addField('snl', 'node_id', 'node_id');
		//  $selectEnroll->addField('masterenroll', 'is_current', 'is_current');//Commented this line for duplicate column error
		$selectEnroll->addExpression("if(mro.mro = 'cre_sys_inv_opt',3,if(mro.mro = 'cre_sys_inv_man',1,2))", "ascen");
		$selectEnroll->addExpression("(SELECT CONCAT(exmp.exempted_status,'|',exmp.created_by,'|',exmp.created_on) FROM slt_enrollment_exempted exmp WHERE exmp.enrollment_id = masterenroll.id AND exmp.enroll_type = 'tp' ORDER BY id DESC LIMIT 1)", 'exempted_sts');
		if(!empty($prgId)){
			$and->condition('prg.id', $prgId,'=');
		}
		$and->condition('masterenroll.is_current', 'N');
		$selectEnroll->condition($and);
		$selectEnroll2 = db_select($selectEnroll, "acc");
		$selectEnroll2->distinct();
		$selectEnroll2->fields('acc');
		$selectEnroll2->groupBy('acc.id');
				$selectEnroll2->orderBy('acc.id', 'desc');

		if($sidx !=''){
			$selectEnroll2->orderBy($sidx, $sord);
		}
		
		expDebug::dPrintDBAPI('getLnrCertificationResultSet --',$selectEnroll2,5);
		
		$lpQryRes = $selectEnroll2->execute()->fetchAll();
		
		expDebug::dPrint("result ::::".print_r($lpQryRes,1),5);
		
		$result_response = array();
		$i=0;
		
		foreach ($lpQryRes as $record) {
			$nxt_mdl = getModuleIdFromObjectId($record->programid,($record->recertify_path)+1);
			$nxt_mdl_crs = getGrpCourseCnt($record->programid,$nxt_mdl[0]->module_id);
			$prgTypeName = getProfileListItemName($record->prg_object_type,null,$record->recertify_path);
			$result_response[$i] = array(
// 					'recertify_path' => $record->recertify_path,
// 					'is_current' => $record->is_current,
// 					'prg_title' => $record->prg_title,
					
					'master_enroll_id' => $record->id,
					'user_id'		    => $record->user_id,
					'overall_status'	=> $record->overall_status,
					'reg_date'		=> date_format(date_create($record->reg_date),'M d, Y'),
					'update_date'		=> date_format(date_create($record->updated_on),'M d, Y'),
					'cancel_date'		=> date_format(date_create($record->cancel_date),'M d, Y'),
					'comp_date'		=> date_format(date_create($record->comp_date),'M d, Y'),
					'expired_date'    => getExpiredDate($record->comp_date,$record->prg_expires_in_value,$record->prg_expires_in_unit),
					'expires_on_lbl' => getExpiredDate($record->comp_date,$record->prg_expires_in_value,$record->prg_expires_in_unit,1),
					'percentage_complete'	=> $record->percentage_complete,
					'prg_id'		    => $record->programid,
					'base_type'		=> $record->basetype,
					'prg_title'		=> $record->prg_title,
					'prg_code'		=> $record->prg_code,
					'prg_desc'		=> $record->prg_desc,
					'prg_shortdesc'	=> titleController('ANNOUNCEMENT_SHORTTITLE', strip_tags(str_replace(array("\n","\r"), array("<br>","&nbsp;"), trim($record->prg_shortdesc))),($theme_key == "expertusoneV2" ? 120 : '')),
					'prg_fulldescription' => /* strip_tags( */str_replace(array("\n","\r","<p>&nbsp;</p>"), array("","&nbsp;",""), trim($record->prg_shortdesc))/* ) */,
					'prg_start_date'	=> $record->prg_start_date,
					'prg_end_date'	=> date_format(date_create($record->prg_end_date),'M d, Y'),
					'prg_status'	    => $record->prg_status,
					'prg_object_type' => $record->prg_object_type,
					'prg_type'        => $prgTypeName,
					'node_id'			=> $record->node_id,
					'recertify_path'	=> $record->recertify_path,
					'survey_status'	=> getSurveyAssesmentDetail($record->programid,$record->prg_object_type,'survey',$record->recertify_path),
					'assessment_status'	=> getAssessmentDetail($record->programid,$record->prg_object_type,$record->recertify_path),
					'preassessment_status'=> getPreAssessmentDetail($record->programid,$record->prg_object_type,$record->recertify_path),
					//'attemptleft'       => getAttemptsCount($record->id,$record->programid,$record->prg_object_type,$record->user_id),
					// 'max_score_validation'	=> getMaxScoreValidation($record->programid,$record->prg_object_type),
					'maxscorevaluepre'           => getMaxScoreValidation($record->programid,$record->prg_object_type,'pre'), // maxscore Validation for Pre assesment
					'maxscorevaluepost'          => getMaxScoreValidation($record->programid,$record->prg_object_type,'post'),// maxscore Validation for Post assesment
					'inp_class_count' => getCourseCompletionStatus($record->id),
					'enr_comp_status' => getLPCourses($record->id,$record->programid,$record->recertify_path,'GetCompStaus',$record->overall_status,''),
					'show_lnr_lp_attach' => getAttachmentDetails($record->programid,'cre_sys_obt_trp'),
					'master_mandatory'  => $record->master_mandatory,
					'full_name'       => ($record->created_by)?(($full_name=getPersonDetails($record->created_by, array('full_name')))?$full_name['full_name']:'') :'',
					'managerid'       => $record->created_by,
					'updated_by'      => $record->updated_by,
					'updated_by_name' => ($record->updated_by)?(($updated_by=getPersonDetails($record->updated_by, array('full_name')))?$updated_by['full_name']:'') :'',
					'mro' => $record->mro,
					'mro_name' => empty($record->mro)?'':getProfileListItemName($record->mro),
					'assigned_by'	=> getAssignedBy($record->user_id),
					'labelmsg'    => array('msg1' => t('LBL025'),'msg2' => t('LBL026'),'msg3' => t('LBL027'),'msg4' => t('LBL028')),
					'star_widget'				 => drupal_render(drupal_get_form('fivestar_rating_widget', '', $settings)),
					'created_by_ins_mngr_slf'	 => $record->created_by ? $updated_by=getUpdatedByDetails($record->user_id, $record->created_by) : t('Me'),
					'updated_by_ins_mngr_slf'	 => $record->updated_by ? $updated_by=getUpdatedByDetails($record->user_id, $record->updated_by) : t('Me'),
					'learning_type_image'		 => getTypeImageClass($record->prg_object_type,$record->recertify_path,$record->overall_status,'R'),
					'learning_type'		 	 => t($prgTypeName),
					'recertify_before_certify_expired'    => ($record->comp_date && $nxt_mdl_crs>0) ? (($record->prg_object_type == 'cre_sys_obt_crt') ? getRecertifyBeforeCertifyExpired($record->comp_date,$record->prg_expires_in_value,$record->prg_expires_in_unit) : 0) : 0,
					'is_current' => $record->is_current,
					'master_enroll_id_encrypted'  => core_encrypt($record->id),
					'prg_id_encrypted'		    => core_encrypt($record->programid),
					'is_exempted' => $exemptedDet[0],
					'exempted_by' => 	($exemptedDet[0] == 1)? (($exmpetedBy = getPersonDetails($exemptedDet[1], array('full_name'))) ? $exmpetedBy['full_name'] : ''): '',
					'exempted_on' => 	($exemptedDet[0] == 1)? date_format(date_create($exemptedDet[2]),'M d, Y') : ''
			);
			$i++;
		}  
		
		expDebug::dPrint('module $result_response = ' . print_r($result_response, true), 5);
		if($is_API){
			return $result_response;
			
		}
		
	  echo drupal_json_output($result_response);
 		 
		}catch (Exception $ex) {
			watchdog_exception('getCertificationModules', $ex);
			expertusErrorThrow($ex);
		}
}

function getLPSearchResults() {
	global $tTrack;
	$tTrack['end']['before_my_programs'] = (microtime(true) - $tTrack['start']['index_start']);
	$tTrack['start']['my_programs'] = microtime(true);
try{
  global $theme_key;
  $solrSource = getConfigValue('solr_search');
  expDebug::dPrint("solr source check on ".$solrSource,5);
  if($solrSource == 1){
  	require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_mylearning_program.php';
  	try{
  		expDebug::dPrint ( "Solr my program Search Testing", 1 );
  		$solrObj = new MyLearningProgramSolrSearch();
  		$getlpQryRes = $solrObj->ProgramSolrSearch(); 
  	}catch(Exception $e){
  		expDebug::dPrint("Error in my program Solr Search -- ".print_r($e->getMessage(),true),1);
  	}
  }
  else {
  	$getlpQryRes = getLnrLearningPlanResultSet();	
  }
  expDebug::dPrint("soun solr result".print_r($getlpQryRes,1),5);
  $lpQryRes = $getlpQryRes['result'];
  $result_response= new stdClass();
  $result_response->page    = $getlpQryRes['page'];
  $result_response->total   = $getlpQryRes['total_pages'];
  $result_response->records = $getlpQryRes['count'];

  $i=0;
  foreach ($lpQryRes as $record) {
   	  $settings = array(
				  'content_type' => $record->prg_object_type,
				  'content_id' => $record->node_id,
				  'stars' => 5,
				  'autosubmit' => TRUE,
				  'allow_clear' => FALSE,
				  'required' => FALSE,
				  'tag' => 'vote',
                  'style' => 'average',
      			  'text' => 'average',
   	  			  'startype' => $record->prg_object_type,
                  //'feedback_enable' => TRUE
	  );

	  $hideShare = 1; // don't show the link
	  if(getShareModuleStatus('mylearning')){ 
			 $hideShare = 0; //true-> show share link
		}	
	  $prgTypeName = getProfileListItemName($record->prg_object_type,null,$record->recertify_path);
	  $nxt_mdl = getModuleIdFromObjectId($record->programid,($record->recertify_path)+1);
	  expDebug::dPrint("next module >>> ".$nxt_mdl[0]->module_id, 5);
	  $nxt_mdl_crs = getGrpCourseCnt($record->programid,$nxt_mdl[0]->module_id);
	  expDebug::dPrint("module crs cnt ".$nxt_mdl_crs , 5);
	  $exemptedDet = explode("|",$record->exempted_sts);
 	  $result_response->rows[$i]['id']   = 'prg_'.$record->id;
      $result_response->rows[$i]['cell'] = array(
      'master_enroll_id'              => $record->id,
      'user_id'		    => $record->user_id,
      'overall_status'	=> $record->overall_status,
      'reg_date'		=> date_format(date_create($record->reg_date),'M d, Y'),
      'update_date'		=> date_format(date_create($record->updated_on),'M d, Y'),
      'cancel_date'		=> date_format(date_create($record->cancel_date),'M d, Y'),
      'comp_date'		=> !empty($record->comp_date) ? date_format(date_create($record->comp_date),'M d, Y') : '',
      'expired_date'    => getExpiredDate($record->comp_date,$record->prg_expires_in_value,$record->prg_expires_in_unit),
      'percentage_complete'	=> $record->percentage_complete,
      'prg_id'		    => $record->programid,
      'base_type'		=> $record->basetype,
      'prg_title'		=> sanitize_data($record->prg_title),
      'prg_code'		=> $record->prg_code,
      'prg_desc'		=> $record->prg_desc,
      'prg_shortdesc'	=> titleController('ANNOUNCEMENT_SHORTTITLE', strip_tags(str_replace(array("\n","\r"), array("<br>","&nbsp;"), trim($record->prg_shortdesc))),($theme_key == "expertusoneV2" ? 120 : '')),
      'prg_fulldescription' => /* strip_tags( */str_replace(array("\n","\r","<p>&nbsp;</p>"), array("","&nbsp;",""), trim($record->prg_shortdesc))/* ) */,
      'prg_start_date'	=> $record->prg_start_date,
      'prg_end_date'	=> date_format(date_create($record->prg_end_date),'M d, Y'),
      'prg_status'	    => $record->prg_status,
      'prg_object_type' => $record->prg_object_type,
      'prg_type'        => $prgTypeName,
      'node_id'			=> $record->node_id,
      'recertify_path'	=> $record->recertify_path,
      'survey_status'	=> getSurveyAssesmentDetail($record->programid,$record->prg_object_type,'survey',$record->recertify_path),
      'assessment_status'	=> getAssessmentDetail($record->programid,$record->prg_object_type,$record->recertify_path),
      'preassessment_status'=> getPreAssessmentDetail($record->programid,$record->prg_object_type,$record->recertify_path),
       //'attemptleft'       => getAttemptsCount($record->id,$record->programid,$record->prg_object_type,$record->user_id),
     // 'max_score_validation'	=> getMaxScoreValidation($record->programid,$record->prg_object_type),
      'maxscorevaluepre'           => getMaxScoreValidation($record->programid,$record->prg_object_type,'pre'), // maxscore Validation for Pre assesment
      'maxscorevaluepost'          => getMaxScoreValidation($record->programid,$record->prg_object_type,'post'),// maxscore Validation for Post assesment
      'inp_class_count' => getCourseCompletionStatus($record->id),
      'enr_comp_status' => getLPCourses($record->id,$record->programid,$record->recertify_path,'GetCompStaus',$record->overall_status,$record->prg_object_type),
 	  'show_lnr_lp_attach' => getAttachmentDetails($record->programid,'cre_sys_obt_trp'),
      'master_mandatory'  => $record->master_mandatory,
      'full_name'       => ($record->created_by)?(($full_name=getPersonDetails($record->created_by, array('full_name')))?$full_name['full_name']:'') :'',
      'managerid'       => $record->created_by,
      'updated_by'      => $record->updated_by,
      'updated_by_name' => ($record->updated_by)?(($updated_by=getPersonDetails($record->updated_by, array('full_name')))?$updated_by['full_name']:'') :'',
      'mro' => $record->mro,
      'mro_name' => empty($record->mro)?'':getProfileListItemName($record->mro),
      'assigned_by'	=> getAssignedBy($record->user_id),
      'labelmsg'    => array('msg1' => t('LBL025'),'msg2' => t('LBL026'),'msg3' => t('LBL027'),'msg4' => t('LBL028')),
      'star_widget'				 => drupal_render(drupal_get_form('fivestar_rating_widget', '', $settings)),
      'created_by_ins_mngr_slf'	 => $record->created_by ? $updated_by=getUpdatedByDetails($record->user_id, $record->created_by) : t('Me'),
      'updated_by_ins_mngr_slf'	 => $record->updated_by ? $updated_by=getUpdatedByDetails($record->user_id, $record->updated_by) : t('Me'),
      'learning_type_image'		 => getTypeImageClass($record->prg_object_type,$record->recertify_path,$record->overall_status),
      'learning_type'		 	 => t($prgTypeName),
      'recertify_before_certify_expired'    => ($record->comp_date && $nxt_mdl_crs > 0) ? ($record->prg_object_type == 'cre_sys_obt_crt') ? getRecertifyBeforeCertifyExpired($record->comp_date,$record->prg_expires_in_value,$record->prg_expires_in_unit) : 0 : 0,
      'is_current' => $record->is_current,
      'master_enroll_id_encrypted'  => core_encrypt($record->id),
      'prg_id_encrypted'		    => core_encrypt($record->programid),
      'is_exempted' => $exemptedDet[0],
      'exempted_by' => 	($exemptedDet[0] == 1)? (($exmpetedBy = getPersonDetails($exemptedDet[1], array('full_name'))) ? $exmpetedBy['full_name'] : ''): '',
      'exempted_on' => 	($exemptedDet[0] == 1)? date_format(date_create($exemptedDet[2]),'M d, Y') : '',
      'expired_on' => ($record->prg_object_type == 'cre_sys_obt_crt' && $record->overall_status =="lrn_tpm_ovr_exp") ?  getExpiredDate($record->comp_date,$record->prg_expires_in_value,$record->prg_expires_in_unit) :'' ,
		'hide_share' => $hideShare      
		);
      $config=getConfig("exp_sp");
      $mylearn_version 					= $config['mylearn_version'];
      if($mylearn_version==1){
      	$content_details = content_player_program_callback($record->id,1);
      	expDebug::dPrint('CONTEnt details of enroll id'.$record->id.'<--enroll-id-->'.print_r($content_details,1),4);
      	if(($content_details['classDetails']['defaultLaunchContent'] >= 0 && $content_details['classDetails']['defaultLaunchContent']!=='notset') && $content_details['classDetails']['total_content_count']>0){
      		$result_response->rows[$i]['cell']['mylearn_content_launchable']   = true;
      	}else{
      		$result_response->rows[$i]['cell']['mylearn_content_launchable']   = false;
      	}
      	if(isset($content_details['classDetails']['lp_have_only_post_ass_cont']) && $content_details['classDetails']['lp_have_only_post_ass_cont']==true)
      		$result_response->rows[$i]['cell']['lp_have_only_post_ass_cont']   = true;
      	$result_response->rows[$i]['cell']['mylearn_survey_pending_flage']   = $content_details['classDetails']['survey_pending_flag'];
      	$result_response->rows[$i]['cell']['total_content_count']   = $content_details['classDetails']['total_content_count'];
      	$result_response->rows[$i]['cell']['content_pending_flag']   = $content_details['classDetails']['content_pending_flag'];
      }
      $i++;
  }

  // Identify last rec in the list sent back to jqgrid.
  if ($i > 0) {
    $result_response->rows[$i-1]['cell']['is_last_rec'] = 'last';
  }

  // Fix for issue #20086
  if (!empty($_GET['enrollmentId']) && !empty($_GET['lessonId'])) {
    expDebug::dPrint('$_GET[enrollmentId] = ' . $_GET['enrollmentId'], 4);
    expDebug::dPrint('$_GET[lessonId] = ' . $_GET['lessonId'], 4);
    $result_response->triggering_enrollment_id = $_GET['enrollmentId'];
    $result_response->triggering_lesson = $_GET['lessonId'];
    $launchInfo = getLaunchDetail($_GET['enrollmentId']);
    foreach ($launchInfo as $lessonDetails) {
      if ($lessonDetails['ID'] == $_GET['lessonId']) {
        $result_response->triggering_lesson_details = $lessonDetails;
        $contentId = isset($contentId) ? $contentId : $lessonDetails['ContentId'];
      }
    }
    $result_response->triggering_content_quiz_status = getConsolidatedQuizStatus($_GET['enrollmentId'], $contentId, $launchInfo);
//     $result_response->triggering_content_quiz_status = 'ayyappan';
    expDebug::dPrint('ayyappan status');
  }

  expDebug::dPrint('$result_response = ' . print_r($result_response, true), 5);
  $tTrack['end']['my_programs'] = (microtime(true) - $tTrack['start']['my_programs']);
  echo drupal_json_encode($result_response);
 }catch (Exception $ex) {
    watchdog_exception('getLPSearchResults', $ex);
    expertusErrorThrow($ex);
  }
}



function getAssignedBy($user_id){
try{
  $job_role_name = '';
  $job_role_arr = getUserJobrole($user_id);
  if(!empty($job_role)){
  	foreach ($job_role_arr as $value){
  		if(empty($job_role_name))
  			$job_role_name .= $value->name;
  		else
  			$job_role_name .= ','.$value->name;
  	}
  }
  return $job_role_name;
  }catch (Exception $ex) {
    watchdog_exception('getAssignedBy', $ex);
    expertusErrorThrow($ex);
  }
}

function getSurveyAssesmentDetail($prgid,$obj_type,$callType,$recertifypath){
  try{
  $select = db_select('slt_survey_mapping', 'survey');
  $select->join('slt_survey', 'sur', 'sur.id = survey.survey_id');
  if($obj_type=="cre_sys_obt_crt" && !empty($recertifypath)) {
  	$select->join('slt_module','module','module.id=survey.module_id');
  	$select->condition('module.sequence', $recertifypath,'=');
  }
  $select->addExpression('COUNT(survey.id)', 'count');
  $select->condition('survey.object_id', $prgid,'=');
  $select->condition('survey.object_type', $obj_type,'=');
  if($callType == 'survey')
  	$select->condition('sur.type', 'sry_det_typ_ass', '!=');
  else
  	$select->condition('sur.type', 'sry_det_typ_ass', '=');
  expDebug::dPrintDBAPI(' getSurveyAssesmentDetail= ',$select,5);
  $count = $select->execute()->fetchField();
  expDebug::dPrint(' getSurveyAssesmentDetail result = '.$count,5);
  return $count;
  }catch (Exception $ex) {
    watchdog_exception('getSurveyAssesmentDetail', $ex);
    expertusErrorThrow($ex);
  }
}

function getAssessmentDetail($prgid,$obj_type,$recertifypath=''){
  try{
  $select = db_select('slt_survey_mapping', 'survey');
  $select->join('slt_survey', 'sur', 'sur.id = survey.survey_id');
  if($obj_type=="cre_sys_obt_crt" && !empty($recertifypath)) {
  $select->join('slt_module','module','module.id=survey.module_id');
  $select->condition('module.sequence', $recertifypath,'=');
  }
  $select->addExpression('COUNT(survey.id)', 'count');
  $select->condition('survey.object_id', $prgid,'=');
  $select->condition('survey.object_type', $obj_type,'=');
  $select->condition('sur.type', 'sry_det_typ_ass', '=');
  $select->condition('survey.pre_status', 0 , '=');
  expDebug::dPrintDBAPI(' getassessment detail module ',$select,5);
  $count = $select->execute()->fetchField();
  return $count;
  }catch (Exception $ex) {
    watchdog_exception('getAssessmentDetail', $ex);
    expertusErrorThrow($ex);
  }

}
function getPreAssessmentDetail($prgid,$obj_type,$recertifypath=''){
  try{
  $select = db_select('slt_survey_mapping', 'survey');
  $select->join('slt_survey', 'sur', 'sur.id = survey.survey_id');
  if($obj_type=="cre_sys_obt_crt" && !empty($recertifypath)) {
	  $select->join('slt_module','module','module.id=survey.module_id');
	  $select->condition('module.sequence', $recertifypath,'=');
  }
  $select->addExpression('COUNT(survey.id)', 'count');
  $select->condition('survey.object_id', $prgid,'=');
  $select->condition('survey.object_type', $obj_type,'=');
  $select->condition('sur.type', 'sry_det_typ_ass', '=');
  $select->condition('survey.pre_status', 1, '=');
  expDebug::dPrintDBAPI('$select', $select);
  $count = $select->execute()->fetchField();
  return $count;
  }catch (Exception $ex) {
    watchdog_exception('getPreAssessmentDetail', $ex);
    expertusErrorThrow($ex);
  }
}

/* function getAttemptsCount($masenrollId,$prgid,$obj_type,$userId){
	try{ */
/* 		$select = db_select('slt_survey_mapping', 'surMap');
		$select->leftjoin('slt_survey_results','surRes','surRes.survey_id = surMap.survey_id');
		$select->addExpression('COUNT(surMap.id)', 'count');
		$select->addField('surMap', 'no_of_attempts', 'attempts_map');
		$select->addExpression('Max(surRes.no_of_attempts)', 'attempts_res');
		//$select->condition('surMap.survey_id',$surveyId,'=');
		$select->condition('surMap.object_id',$prgid,'=');
		$select->condition('surMap.object_type',$obj_type,'=');
		$select->condition('surRes.user_id',$userId,'=');
		expDebug::dPrintDBAPI('ddssadadads->',$select);
		$result = $select->execute()->fetchAssoc();
		if ($result['attempts_map'] == $result['attempts_res']) {
			return false;
		}else {
			return true;
		}	 */
/* 		expDebug::dPrint('1-->'.$masenrollId.'2-->'.$prgid.'3--->'.$obj_type.'4-->'.$userId);
		$query = db_query("select if(count(1) = 0,
				(select no_of_attempts from slt_survey_mapping map
				join slt_survey sur on sur.id = map.survey_id and sur.type = 'sry_det_typ_ass'
				where map.object_id=:objectId
				and map.object_type =:objectType),
				((select  ifnull(map.no_of_attempts,0) from slt_survey_mapping map
				join slt_survey sur on sur.id = map.survey_id and sur.type = 'sry_det_typ_ass' where map.object_id=res.entity_id
				and map.object_type = res.entity_type) - ifnull(max(res.no_of_attempts),0))) as resatt
				from slt_survey_results res
				where res.entity_id=:objectId and res.user_id=:userId and res.enrollment_id=:enrollId",
				array(':userId' => $userId,':objectId' => $prgid,':objectType' => $obj_type,':enrollId' => $masenrollId));
		$qryRes = $query->fetchField();
		if($qryRes == 0) {
			return false;
		}else {
			return true;
		}
	}catch (Exception $ex) {
		watchdog_exception('getAttemptsCount',$ex);
		expertusErrorThrow($ex);
	}
} */

function getMaxScoreValidation($prgid,$obj_type,$ass_type=''){
	try{
		$select = db_select('slt_survey_mapping', 'sur_map');
		$select->join('slt_survey', 'sur', 'sur.id = sur_map.survey_id');
		// issue in the slt_survey_groups on 'sur.id = sur_grp.survey_id' => Cant Get this sur_grp.survey_id as Survey id.
		/* $select->join('slt_survey_groups', 'sur_grp', 'sur.id = sur_grp.survey_id');
		 $select->join('slt_survey_groups_questions', 'sur_grp_ques', 'sur.id = sur_grp_ques.survey_id and sur_grp.id = sur_grp_ques.survey_group_id'); */
		$select->join('slt_survey_groups_questions', 'sur_grp_ques', 'sur.id = sur_grp_ques.survey_id');
		$select->join('slt_survey_groups', 'sur_grp', 'sur_grp.id = sur_grp_ques.survey_group_id');
		$select->addExpression('sur.max_mark', 'max_score_value');
		$select->addExpression('SUM(sur_grp_ques.score)', 'total_score');
		$select->condition('sur_map.object_id', $prgid,'=');
		$select->condition('sur_map.object_type', $obj_type,'=');
		$select->condition('sur.type','sry_det_typ_ass', '=');
		$select->condition('sur_grp.status','sry_det_grp_atv', '=');
		$select->groupBy('sur_map.id');
		if($ass_type == 'pre'){
			$select->condition('sur_map.pre_status', '1','=');
		}else if ($ass_type == 'post'){
			$select->condition('sur_map.pre_status', '0','=');
		}
		expDebug::dPrintDBAPI('getMaxScoreValidation = ',$select);
		$result = $select->execute();
		if($result->rowCount()>0){
			$max_score_results = $result->fetchAll();
			foreach($max_score_results as $value){
				if($value->max_score_value == $value->total_score)
					return 1;
			}
		}
		return 0;
	}catch (Exception $ex) {
		watchdog_exception('getMaxScoreValidation', $ex);
		expertusErrorThrow($ex);
	}
}

function getCourseCompletionStatus($masterEnrollId){
	try{
	    //added for #0089761.
	    $selectStmt = db_select('slt_master_enrollment', 'enr');
	    $selectStmt->addField('enr','recertify_path','recertify_path');
	    $selectStmt->addField('enr','program_id','program_id');
	    $selectStmt->condition('enr.id',$masterEnrollId,'=');
	    $selectStmt->condition('enr.is_current','Y','=');
	    $result = $selectStmt->execute()->fetchAll();
	    expDebug::dPrintDBAPI("Fetch the module and current recert path: ",$selectStmt);
	    expDebug::dPrint(' result for module and current recert path:  ' . print_r($result, true), 5);
	    $programID = $result[0]->program_id;
	    $recertFlag = $result[0]->recertify_path;
	    $module_id = getModuleIdFromObjectId($programID,$recertFlag);
	    
	 $select = db_select('slt_enrollment','enr');
     $select -> join('slt_master_enrollment', 'menr', 'menr.id=enr.master_enrollment_id');
     $select -> join('slt_module_crs_mapping', 'map', 'map.program_id=menr.program_id and enr.course_id=map.course_id');
     $select->addExpression('COUNT(1)', 'count');
     $select->condition('map.is_required', 'Y', '=');
     $select->condition('map.module_id',$module_id[0]->module_id,'=');
     $select->condition('enr.comp_status', array('lrn_crs_cmp_att','lrn_crs_cmp_enr','lrn_crs_cmp_inp'),'IN');
     $select->condition('menr.id', $masterEnrollId, '=');
     $count = $select->execute()->fetchField();
     expDebug::dPrintDBAPI("Class Inprogress Count",$select);
     return $count;
	}catch(Exception $e){

	}
}

function getLPCourses($masterEnrollId,$prgId,$recertify_path,$getFrom ='',$overall_status,$object_type){
  try{
   $userId = getSltpersonUserId();
 
   
   if($object_type=='cre_sys_obt_crt') {
   	$moduleseq = "and module.sequence='".$recertify_path."'";
   }
   
//    if(!$recertify_path){
     $scheduledCrsQry ="SELECT f.id crs_id,
  		f.code crs_code,
  		f.title crs_title,
  		f.short_description crs_desc,
  		f.valid_from start_date,
  		f.valid_to end_date,
  		e.sequence sequence_no,
  		e.is_required is_required,
  		IFNULL((SELECT enr.comp_status FROM slt_enrollment enr WHERE enr.course_id= f.id AND enr.user_id=:user_id AND enr.reg_status NOT IN('lrn_crs_reg_rsv','lrn_crs_reg_rsc') AND enr.master_enrollment_id =:master LIMIT 1),'lrn_crs_cmp_enr') AS comp_status,
  		IFNULL(slf_get_profile_list_name (IFNULL((SELECT enr.comp_status FROM slt_enrollment enr WHERE enr.course_id= f.id AND enr.user_id=:user_id AND enr.reg_status NOT IN('lrn_crs_reg_rsv','lrn_crs_reg_rsc') AND enr.master_enrollment_id =:master LIMIT 1),'lrn_crs_cmp_enr'),NULL),'Enrolled') complition_status
  	 FROM  slt_course_template f
	 inner join slt_module_crs_mapping e on f.id=e.course_id
	 inner join slt_module module on module.id=e.module_id
   	 WHERE e.program_id=:prgid ".$moduleseq."
  	 ORDER BY f.title";
   
   // and module.sequence=:recert_id
   
//    }
//    else{
/*
     $scheduledCrsQry ="SELECT f.id crs_id,
  		f.code crs_code,
  		f.title crs_title,
  		f.short_description crs_desc,
  		f.valid_from start_date,
  		f.valid_to end_date,
  		e.sequence sequence_no,
  		e.is_required is_required,
  		IFNULL((SELECT enr.comp_status FROM slt_enrollment enr WHERE enr.course_id= f.id AND enr.user_id=:user_id AND enr.reg_status NOT IN('lrn_crs_reg_rsv','lrn_crs_reg_rsc') AND enr.master_enrollment_id =:master LIMIT 1),'lrn_crs_cmp_enr') AS comp_status,
  		IFNULL(slf_get_profile_list_name (IFNULL((SELECT enr.comp_status FROM slt_enrollment enr WHERE enr.course_id= f.id AND enr.user_id=:user_id AND enr.reg_status NOT IN('lrn_crs_reg_rsv','lrn_crs_reg_rsc') AND enr.master_enrollment_id =:master LIMIT 1),'lrn_crs_cmp_enr'),NULL),'Enrolled') complition_status
  	 FROM  slt_re_certify e,
  	      slt_course_template f
  	 WHERE f.id=e.course_id
  	 AND   e.program_id = :prgid
  	 ORDER BY f.title";
  	 */
//    }

  $qry  = db_query($scheduledCrsQry, array(':prgid' => $prgId,':user_id' => $userId,':master' =>$masterEnrollId));
  expDebug::dPrintDBAPI("getLPCourses query= ",$qry);
  expDebug::dPrint(' getLPCourses = '.$prgId.' userId '.$userId.' mnerid '.$masterEnrollId,4);
  $result = $qry->fetchAll();
  if($getFrom == 'GetCompStaus'){
    $isRequiredCount = 0;
    $compCount = 0;
    $enrollCount = 0;
    $incCount = 0;
    $inpCount = 0;
    $completedPercentage = 0;
    $compPerEnrCount = 0;
    foreach ($result as $record) {
      $status = $record->comp_status;
      if($record->is_required == 'Y'){
      	$isRequiredCount++;
      	if($status == "lrn_crs_cmp_cmp"){
      	  $compPerEnrCount++;
      	}
      }
      if($status == "lrn_crs_cmp_cmp")
      	$compCount++;
      else if($status == "lrn_crs_cmp_enr")
      	$enrollCount++;
      else if($status == "lrn_crs_cmp_inc")
      	$incCount++;
      else
      	$inpCount++;
    }
    if($compPerEnrCount != 0){
      $completedPercentage = ($compPerEnrCount/$isRequiredCount)*100;
      $completedPercentage = round($completedPercentage);
    }
    $overallProgress = db_query(' select overall_progress from slt_master_enrollment where id = :master_enroll_id ', array(':master_enroll_id' => $masterEnrollId))->fetchField();
    $enrStatus = array();
    $enrStatus['req_count'] = $isRequiredCount;
    $enrStatus['comp_count'] = $compCount;
    $enrStatus['enr_count'] = $enrollCount;
    $enrStatus['inp_count'] = $inpCount;
    $enrStatus['inc_count'] = $incCount;
    $enrStatus['comp_percentage'] = convertProgressValue($completedPercentage);
    $enrStatus['overall_progress'] = convertProgressValue($overallProgress);
    return $enrStatus;
  }
  else
    echo drupal_json_output($result);
  }catch (Exception $ex) {
    watchdog_exception('getLPCourses', $ex);
    expertusErrorThrow($ex);
  }
}


function getLPClasses($master_enrollment_id,$prgId,$recertifyPath,$object_type,$overall_status,$userid='',$is_API=0) {
  try{
  expDebug::dPrint('$getLPClassesgetLPClassesgetLPClasses$result$result$result$result'.$prgId,5);
  if($is_API){
  $user_id = $userid;
  }else{
  $user_id = getSltpersonUserId();
  }

  expDebug::dPrint("user id ::".$user_id,5);
  
//   if($overall_status=="lrn_tpm_ovr_enr" && $object_type == "cre_sys_obt_crt") {  
//   if(!empty($recertifyPath)){
//   	$recertify_cond = " AND recertify_path = ".$recertifyPath."";
//   }else{
//   	$recertify_cond = " ";
//   }
//   }

  
   /* if(!empty($object_type) && $object_type != "cre_sys_obt_crt" ){
    	$prgmap_cond = "(select program_id from slt_master_enrollment where id = ".$master_enrollment_id.")";
  }else{*/
    	$prgmap_cond = $prgId;
  //}
    //   IF(myenroll.recertify_path > 1,rec.module_id,NULL) AS rec_moduleid,
    //   IF(myenroll.recertify_path > 1,rec.group_title,NULL) AS rec_grouptitle
  $enrollQry = "SELECT myenroll.*,map.sequence AS seq_no,map.module_id AS moduleid,mgrp.title AS map_grouptitle FROM ( ";
  $enrollQry .= getEnrollmentQuery($user_id, 0);
  $enrollQry .= " ) myenroll ";
  $enrollQry .= " LEFT JOIN slt_module lp_map ON lp_map.program_id=$prgmap_cond AND lp_map.sequence = myenroll.recertify_path";
  $enrollQry .= " LEFT JOIN slt_module_crs_mapping map ON map.program_id=$prgmap_cond AND map.course_id = myenroll.course_id and map.module_id = lp_map.id";
  $enrollQry .= " LEFT JOIN slt_module_groups mgrp ON mgrp.id = map.group_id ";
  

 // if(!empty($master_enrollment_id) && $recertifyPath == 1 ){
  //  $enrollQry .= " WHERE myenroll.master_enrollment_id = ".$master_enrollment_id."";
 // }else{
  //	$enrollQry .= " WHERE myenroll.master_enrollment_id IS NOT NULL AND myenroll.comp_status IN ('lrn_crs_cmp_enr','lrn_crs_cmp_cmp','lrn_crs_cmp_inc','lrn_crs_cmp_inp')";
 // }
  
  //if(!empty($object_type) && $object_type != "cre_sys_obt_crt") {
    $enrollQry .= " WHERE myenroll.master_enrollment_id = ".$master_enrollment_id."";
 /* } else {
  	$enrollQry .= " WHERE myenroll.master_enrollment_id in (select id from slt_master_enrollment where user_id=$user_id and program_id=$prgId and overall_status='".$overall_status."' and myenroll.recertify_path='".$recertifyPath."')";
  }*/
  
  
  $enrollQry .= " GROUP BY myenroll.id  ORDER BY master_enrollment_id DESC,lp_map.sequence, mgrp.sequence,map.sequence,course_id";
  expDebug::dPrint("Learning plan Query Fetch DATA :-" . print_r($enrollQry,true),4);
  $selectEnroll = db_query($enrollQry);
  $lpQryRes = $selectEnroll->fetchAll();
  expDebug::dPrint('lp result set '.print_r($lpQryRes, 1));
  $result_response = array();
  $i=0;
  $prevCount = 1;
  $prevCompStatus = '';
  $prevInc = 0;
  foreach ($lpQryRes as $record) {
  	$timezone_details = getPersonDetails($record->user_id);
   	$sid = $record->user_id."-".$record->course_id."-".$record->class_id;
   	$isRequired = getCourseRequiredStatus($prgId,$record->course_id,$record->recertify_path);
   	$enforceSequence = getEnforceSequenceStatus($prgId);
   	if($enforceSequence == 'Y' && $prevCount == 1 && $isRequired == 'Y' && $record->comp_status != 'lrn_crs_cmp_cmp'){
   		$prevCompStatus = 'notallow';
   		$prevClassTitle = $record->cls_title;
   		$prevCount = 0;
   		$prevInc = $i;
   	}
	//include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_sitesetup/exp_sp_administration_module_info/exp_sp_administration_module_info_share.inc";
	  $hideShare = 1; // don't show the link
	  if(getShareModuleStatus('mylearning')){ 
			 $hideShare = 0; //true-> show share link
		}		
   	$launch_detail = getLaunchDetail($record->id,'',$sid);
   	expDebug::dPrint('$prevCompStatus'.$prevCompStatus.'required status'.$prevCount.'ith values'.$i);
   	$record->moduleid = $record->moduleid;
 	  $result_response[$i] = array(
      'id'                         => $record->id,
      'user_id'                    => $record->user_id,
      'master_enrollment_id'       => $record->master_enrollment_id,
      'user_name'                  => $record->user_name,
      'class_id'                   => $record->class_id,
      'course_id'                  => $record->course_id,
      'reg_status'                 => $record->reg_status,
      'reg_status_code'            => $record->reg_status_code,
 	  'reg_status_date'	           => date_format(date_create($record->reg_status_date),'M d, Y'),
      'status_code'                => $record->status_code,
      'reg_date'                   => date_format(date_create($record->reg_date),'M d, Y'),
      'comp_status'     		   => $record->comp_status,
      'update_date'     		   => $record->updated_on,
      'comp_date'				   => date_format(date_create($record->comp_date),'M d, Y'),
      'valid_from'                 => $record->valid_from,
      'valid_to'     			   => $record->valid_to,
      'score'                      => $record->score,
 	  'course_quiz_status'		   => ($record->basetype=='WBT' ? getConsolidatedQuizStatus($record->id, null, $launch_detail) : ''),	//added for scorm changes
      'grade'                      => $record->grade,
      'updated_on'                 => date_format(date_create($record->updated_on),'M d, Y'),
      'course_title'               => addslashes($record->title),
 	  'course_code'                => $record->crs_code,
      'cls_title'                  => addslashes(($record->cls_title)),
      'cls_code'                   => $record->code,
 	  'cls_status'                 => $record->cls_status,
      'description'     		   => /* strip_tags( */str_replace(array("\n","\r","<p>&nbsp;</p>"), array("","&nbsp;",""), trim($record->description))/* ) */,
      'delivery_type'              => $record->delivery_type,
      'delivery_type_code'         => $record->delivery_type_code,
      'delivery_type_image'		   => getTypeImageClass($record->delivery_type_code),
      'basetype'                   => $record->basetype,
      'courseid'                   => $record->courseid,
      'classprice'                 => $record->classprice,
      'orderdatetime'     		   => $record->orderdatetime,
 	  'usertimezonecode'         => $timezone_details['attr1'],
      'session_id'                 => $record->session_id,
      'session_start'              => $record->session_start,
      'session_start_day'		   => ($record->session_start != null ) ? date_format(date_create($record->session_start),'D') : '',
      'session_end'                => '',
      'session_start_format'       => date_format(date_create($record->session_start),'M d, Y'),
      'sessionDetails'			   => getSessionDetails($record->course_id,$record->class_id,$record->delivery_type_code,'enroll'),
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
      'node_id'             	   => $record->nodeid,
      'surveystatus'               => $record->survey_status,
      'launch'					   => $launch_detail,
      'show_lnr_attach'	  		   => getAttachmentDetails($record->class_id,'cre_sys_obt_cls',$record->courseid),
      'show_events'			       => isClassSwitchAvail($record->delivery_type_code,$record->class_id,$record->course_id,$record->user_id,$record->id,'program'),
 	  'recertify_path'			   => $record->recertify_path,
 	  'isRequired'    			   => $isRequired,
 	  'seqNo'					   => $record->seq_no,
      'surveystatus'               => $record->survey_status,
      'assessmentstatus'           => $record->assessment_status,
 	  'preassessmentstatus'        => $record->preassessment_status,
 	  'dedicated_class_flag'	   => $record->dedicated_class_flag,
 	  //'attemptleft'              => assessmentAttemptLeft($record->id,$record->class_id, $record->user_id,'cre_sys_obt_cls'),
 	  //'maxscorevalue'            => getMaximumScoreValidation($record->class_id, 'cre_sys_obt_cls'),
      'maxscorevaluepre'           => getMaximumScoreValidation($record->class_id, 'cre_sys_obt_cls','pre'),
 	  'maxscorevaluepost'          => getMaximumScoreValidation($record->class_id, 'cre_sys_obt_cls','post'),
 	  'moduleid'              	   => $record->moduleid,
      'module_name'       		   => getModuleTitle($record->moduleid,'title'),
 	  'module_code'       	   	   => getModuleTitle($record->moduleid,'code'),
 	  'groupname'       		   => $record->map_grouptitle, 	  
 	  'labelmsg'                   => array('msg1' => t('LBL025'),'msg2' => t('LBL026'),'msg3' => t('LBL027'),'msg4' => t('Incomplete'),'msg5' => t('LBL125'), 'msg6' => t('No Show')),
 	  'waitlist_priority'          => $record->waitlist_priority,
 	  'prev_comp_status'		   => (($prevInc < $i) && $record->comp_status != 'lrn_crs_cmp_cmp') ? $prevCompStatus : '',
 	  'prev_class_title'		   => addslashes($prevClassTitle),
 	  'progress' =>   convertProgressValue(round($record->progress)),
		'hideShare'                  => $hideShare
      );
 	  $config=getConfig("exp_sp");
 	  $mylearn_version 					= $config['mylearn_version'];
 	  if($mylearn_version==1 || ($_REQUEST['mobile_version'] && $_REQUEST['apiname']=='getTPClassesAPI')){
 	  	$content_details = content_playlist_callback($record->id,1,'',1);
 	  	expDebug::dPrint('CONTEnt details of enroll id'.$record->id.'<--enroll-id-->'.print_r($content_details,1),4);
 	  	if(($content_details['classDetails']['defaultLaunchContent'] > 0 && $content_details['classDetails']['defaultLaunchContent']!=='notset') || $content_details['classDetails']['total_content_count']>0){
 	  		$result_response[$i]['mylearn_content_launchable']   = true;
 	  	}else{
 	  		$result_response[$i]['mylearn_content_launchable']   = false;
 	  	}
 	  	$result_response[$i]['mylearn_survey_pending_flage']   = $content_details['classDetails']['survey_pending_flag'];
 	  	$result_response[$i]['total_content_count']   = $content_details['classDetails']['total_content_count'];
 	  	$result_response[$i]['content_pending_flag']   = $content_details['classDetails']['content_pending_flag'];
 	  }
      $i++;

  }
  expDebug::dPrint('$getLPClasses$result$result : '.print_r($result_response,true),5);
   if($is_API){
  return $result_response;
  }
  echo drupal_json_output($result_response);
  }catch (Exception $ex) {
    watchdog_exception('getLPClasses', $ex);
    expertusErrorThrow($ex);
  }
}


function dropTPclassunderManagerAPI($userId,$user_id,$Enrolled_id,$isCommerce)
{
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	$select = db_select('slt_master_enrollment', 'mas');
	$select->addField('mas', 'program_id');
	$select->condition('mas.id',$Enrolled_id);
	$program_id = $select->execute()->fetchField();
	$prgDetails = getProgramObjDetails($program_id);
	expDebug::dPrint("dropTPclassunderManagerAPI $prgDetails::".print_r($prgDetails,TRUE) , 4);

	$select = db_select('system', 'sys');
	$select->addField('sys', 'status', 'status');
	$select->condition('name', 'exp_sp_commerce');
	$commerceEnabled =  $select->execute()->fetchField();
	expDebug::dPrint("dropTPclassunderManagerAPI $commerceEnabled::".print_r($commerceEnabled,TRUE) , 4);

	$drupalUid = getDrupalIdOfUser($userId);
	$account = user_load($drupalUid);
	$is_training_admin = user_access('Administration Perm', $account);
	$training_admin = ($is_training_admin == 1 || $is_training_admin == "1" )?"Y":"N";
	expDebug::dPrint("Check training_admin ".$training_admin);

	if(($training_admin == "N" || $userId == $user_id ) && $commerceEnabled == 1)
	{
		$fromTeam=1;
		if($_REQUEST['from_team'] == 0){
			$fromTeam=0;
		}

		expDebug::dPrint("getDropTrainingProgram r".$fromTeam);
		$result = getDropTrainingProgram($Enrolled_id,$program_id,'true',$commerceEnabled,'',$fromTeam,'api');
		return $result;
	}
	else if($commerceEnabled == 1)
	{
		$isPricedTP =0;
		if($commerceEnabled == 1){
			$isPricedTP = $prgDetails[0]->price;
		}
		if ($isPricedTP > 0) {
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Priced training cannot be cancelled. Contact support team.";
			//return array((object)array('status'=>'Priced training cannot be registered. Contact support team.'));
			return $errobj;
		}
		else {
			$result = getDropTrainingProgram($Enrolled_id,$program_id,'','','',$fromTeam,'api');
			return $result;
		}
	}
	else {
		$result = getDropTrainingProgram($Enrolled_id,$program_id,'','','',$fromTeam,'api');
		return $result;
	}

}



function getDropTrainingProgram($masterEnrollId,$prgId,$refundFlag,$isCommerceEnabled,$assMand,$fromMyteam,$isAPI=''){
	try{
	$select = db_select('slt_master_enrollment', 'mas');
		$select->addField('mas', 'user_id');
		$select->condition('mas.id',$masterEnrollId);
		$user_id = $select->execute()->fetchField();

		expDebug::dPrint("getDropTrainingProgram refund flag::".$refundFlag." isCommerce.".$isCommerceEnabled." assMand=".$assMand , 4);
		$cancelEnr=true;
		$isRefunded = 0;
		$refund_amount = 0;
		$ucOrderId = '';
		if($refundFlag=="true")
		{
			$arr=getDropPolicyDetailsLearner($user_id,'tp',$masterEnrollId,"",$isCommerceEnabled,$assMand);
			$ucOrderId = $arr["uc_order_id"];
			$refund_amount = $arr["refund_amt"];
			$deducted_amount=$arr["deducted_amount"];
			$isRefunded = 1;
			if($arr["next_action"]=="showdroppolicy" && $refund_amount > 0)
			{
				$currencyCode = (!empty($arr["currency_code"])) ? $arr["currency_code"] : 'USD';
				$reply=refund_cc($arr["slt_order_id"],$arr["uc_order_id"],$arr["refund_amt"],"",$currencyCode);
				if($reply->serviceresponse=="success")
					$cancelEnr=true;
				else
					$cancelEnr=false;

				//payment gateway
			}
			expDebug::dPrint("getDropTrainingProgram next action::".$arr["next_action"] , 4);
		}
		
		
		expDebug::dPrint("getDropTrainingProgram msg::".$arr["msg"] , 4);
		//0080779 //next_action == showdroppolicy for exp_paypal,exp_paymetric,exp_cybersource //next_action == showmsg for exp_paymetric and all other type of purchase
		if($arr["next_action"] == "showmsg" && isset($_REQUEST['apiname']) && $_REQUEST['apiname'] == 'dropTPclassesAPI'){
			$tmp->status="failure";
			$tmp->msg=$arr["msg"];
			$tmparr=array();
			$tmparr[0]=$tmp;
			return $tmparr;
		}
		
		
		$arr=array();
		if($cancelEnr==true)
		{
			$updateUserId = getSltpersonUserId();
			$param =  array();
			$param['master_enroll_id'] = $masterEnrollId;
			$param['prg_id'] 					 = $prgId;
			$param['is_refunded']      = $isRefunded;
			$param['refund_amount']    = $refund_amount;
			$param['user_id']          = $user_id;
			$param['update_user_id']   = $updateUserId;
			$param['cancel_from']      = 'learner';
			$param['uc_order_id']			 = $ucOrderId;
			$param['from_manager']  	 = $fromMyteam;
			dropTPLineItem($param,1);
			$arr=array("status"=>"success","msg"=>"");
		}
		else
		{
	  $arr=array("status"=>"failure","msg"=>"Please contact the local training office to cancel this registration.");
		}

		if($isAPI == 'api')
      {
         return $arr;
      }
      else
      {
      echo "var drop_policy_result=".drupal_json_encode($arr);
      }
	}catch (Exception $ex) {
		watchdog_exception('getDropTrainingProgram', $ex);
		expertusErrorThrow($ex);
	}
}

function getCourseRequiredStatus($prgId,$courseId,$recertifyPath){
  try{
  	//include_once(drupal_get_path('module', 'exp_sp_administration_program') .'/exp_sp_administration_program.inc');
  	//$modId = getDefaultModuleId($prgId,$recertifyPath);
    //$module_id = $modId->module_id;
    $select = db_select('slt_module_crs_mapping','map');
    $select->innerJoin('slt_module','module','module.id = map.module_id');
    $select->addField('map','is_required');
    $select->condition('map.program_id', $prgId,'=');
    $select->condition('map.course_id', $courseId,'=');
    $select->condition('module.sequence', $recertifyPath,'=');
    expDebug::dPrintDBAPI("getCourseRequiredStatus query= ",$select);
  $required  = $select->execute()->fetchField();
  return $required;
  }catch (Exception $ex) {
    watchdog_exception('getCourseRequiredStatus', $ex);
    expertusErrorThrow($ex);
  }
}

//Get module title
function getModuleTitle($moduleId,$setInput) {
 try{
 if(empty($moduleId)){
 	return '';
 }
 $moduleDetails = db_select('slt_module', 'module');
 $moduleDetails->addField('module',$setInput, 'moduledetail');
 $moduleDetails->condition('module.id',$moduleId);
 expDebug::dPrintDBAPI("getModuleTitle query= ",$moduleDetails);
 $result = $moduleDetails->execute()->fetchField();
 return $result;
 }catch (Exception $ex) {
    watchdog_exception('getModuleTitle', $ex);
    expertusErrorThrow($ex);
  }
}


/**
 * Get results for Class title autocomplete.
 *
 * @return
 *   Auto Complete Course-Class title Results
 */
function getClsChangeAutoComplete($delType,$userId,$changeClassId,$courseId,$myEnroll){

	$sltPersonUserId = getSltpersonUserId();

	try{
		if($sltPersonUserId!=$userId){
			throw new Exception('Userid and Logged in user doesnot match.');
		}
		module_load_include('inc', 'exp_sp_lnrsearch', 'exp_sp_lnrsearch');
		$delType = ($delType == t('LBL428'))? '' : $delType;
		$lnrSrch = new LnrSearchCatalog();
		$lnrSrch->select = catalogClassSearchQuery($userId);
		$lnrSrch->searchTitleClass($_GET['z']);
		$lnrSrch->searchTypeClass($delType,'');
		$lnrSrch->select->condition('cls.status','lrn_cls_sts_atv','=');
		$lnrSrch->select->groupby('cls.id');
		filterByUserAccessPrivileges($lnrSrch->select, 'cls','','','','Class');
		$lnrSrch->select->condition('cls.course_id',$courseId);
		$lnrSrch->select->condition('cls.id', $changeClassId, '!=');
		if(!empty($myEnroll) && $myEnroll == 'Enroll'){
			$lnrSrch->select->condition(db_or()->
					condition('cls.dedicated_class_flag','N','=')->
					condition('cls.dedicated_class_flag','', 'is null')
			);
		}
		expDebug::dPrintDBAPI('getClsChangeAutoComplete query:', $lnrSrch->select);
		$qry_search_results = $lnrSrch->select->execute()->fetchAll();
		foreach ($qry_search_results as $rec_srch_autocomplete) {
			print $rec_srch_autocomplete->cls_title."\n";
		}

	}catch (Exception $ex) {
		watchdog_exception('getAvailableClassResults', $ex);
		expertusErrorThrow($ex);
	}

}

/**
 * Change class enrollment through api
 */
function changeClassAPI($userId,$enrollId,$currentClassId,$regFrom){
	try{
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$regFrom = ($regFrom == 'tp') ? 'TP':'Enroll';
		$select = db_select('slt_course_class', 'cls');
		$select->leftJoin('slt_enrollment','enr','enr.course_id = cls.course_id');
		$select->addField('cls','id', 'id');
		$select->condition('enr.id',$enrollId, '=');
		$select->condition('cls.id',$currentClassId, '=');
		expDebug::dPrintDBAPI('changeClassAPI=', $select);
		$result = $select->execute()->fetchField();
		expDebug::dPrint('result changeClassAPI--->>' . $result,4);
		if(empty($result)){
			//return array((object)array('id' => 'Class Id does not belongs to same course'));
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Class Id does not belongs to same course";
			return $errobj;
		}
		$DedicatedChk = ($regFrom == 'Enroll') ? is_dedicated($currentClassId) : '';
		if(!empty($DedicatedChk)){
			return array((object)array('status' =>'Class is a dedicated class'));
		}
		$classId = array();
		$classId[0] = $currentClassId;
		$availableSeats = getAvailableSeatInClasses($classId,$userId);
		if($availableSeats == 0){
			//return array((object)array('status' => 'Seats not available'));
			$errobj= new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Seats not available";
			return $errobj;
		}
		$WaitlistSeats = getWaitlistCatalogInfo($currentClassId);
		$regFrom = ($regFrom == 'tp') ? 'TP':'Enroll';
		$changeId = changeClassEnroll($userId, $currentClassId, $enrollId, $WaitlistSeats, $regFrom,'API');
		return $changeId;
	}catch (Exception $ex) {
	  	watchdog_exception('changeClassAPI', $ex);
	  	expertusErrorThrow($ex);
  }
}

/**
 * Register the selected Class from change class popup in My Program
 */
function changeClassEnroll($userId, $currentClassId, $enrollId, $waitlist, $myEnrollChangeCls, $FromApi = ''){
	$sltPersonUserId = getSltpersonUserId();

	try{
		if($sltPersonUserId!=$userId && (empty($FromApi))){
			throw new Exception('Userid and Logged in user doesnot match.');
		}
  expDebug::dPrint('changeClassEnroll called. enrollId->' . $enrollId .' classId -> '.$currentClassId ,4);
 
  if($myEnrollChangeCls == 'Enroll') {
  	if($FromApi){
  		$classId = MyEnrollmentChangeClass($userId, $currentClassId, $enrollId, $waitlist,$FromApi);
  		return $classId;
  	}else{
  		MyEnrollmentChangeClass($userId, $currentClassId, $enrollId, $waitlist);
  	}
  }else if($myEnrollChangeCls == 'compliance'){
  		complianceExpiryRegistration($userId, $currentClassId, $enrollId, $waitlist);
  }else {
	  if($enrollId && $currentClassId){
	     $select = db_select('slt_enrollment','enr');
	     $select->leftJoin('slt_master_enrollment', 'masenr', 'enr.master_enrollment_id = masenr.id');
	     $select->addField('masenr', 'id','mas_enroll_id');
	     $select->addField('masenr', 'overall_status');
	     $select->addField('masenr', 'program_id');
	     $select->addField('enr', 'reg_status');
	     $select->addField('enr', 'comp_status');
	     $select->addField('enr', 'class_id');
	     $select->addField('enr', 'course_id');
	     $select->condition('enr.id', $enrollId);
	     expDebug::dPrintDBAPI('enroll status query:', $select);
	     $result = $select->execute()->fetchAll();
	     expDebug::dPrint('enroll status query:' . print_r($result,true),3);

	     $masterEnrollId  = $result[0]->mas_enroll_id;
	     $tpOverallStatus = $result[0]->overall_status;
	     $classRegStatus  = $result[0]->reg_status;
	     $clsCompStatus   = $result[0]->comp_status;
	     $prevClassId     = $result[0]->class_id;
	     $courseId        = $result[0]->course_id;
	     $programId       = $result[0]->program_id;

	     $regStatus = 'lrn_crs_reg_cnf';
	     $errObj = array();
	     $updStatus = '';
	     $currentClassDelType = getClassDeliveryType($currentClassId, 'Short');
	     $tpCancelICal = null;
	     $tpClassCancelICal = null;
	     if(($clsCompStatus != 'lrn_crs_cmp_enr' && $clsCompStatus != 'lrn_crs_cmp_nsw') && !empty($clsCompStatus)){
	       $errObj[] = t('MSG587');
	     }
	     elseif(!isActiveClass($courseId, $currentClassId)){
	       $errObj[] = t('ERR134');
	     }
	     // Course conflict check removed, since allow multiple registration under a course.
	     /* elseif(courseConflictCheckInClsSwitch($userId, $courseId, $currentClassId, $enrollId)){
	       $errObj[] = t('ERR049');
	     } */
	     elseif ($currentClassDelType == 'ilt' || $currentClassDelType == 'vc') {
	       // Confirm that the user not assigned this class to instruct for learners
	       $conflictInstructor = getClassInstructorConflict($userId, $currentClassId);
	       if ($conflictInstructor == false ) {
	         $errObj[] = t('ERR075');
	       }
	       else{
	         $waitlistNumber = waitListCheck($courseId, $currentClassId);
	         if($waitlistNumber == 0 || $waitlistNumber == -1) {
	            $regStatus = 'lrn_crs_reg_cnf';
	         }
	         else { // waitlist - do further check against waitlist capacity, otherwise class is full.
	           $classWaitlistCapacity = getClassWaitlistCapacity(null, $currentClassId);
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
	    	if ($tpOverallStatus == 'lrn_tpm_ovr_enr') {
	    		$tpClassCancelICal = getCancelICal($masterEnrollId, 'TP', $prevClassId); // to be used when switching to confirmed class (only class_switch notification will be sent)
	    		$tpCancelICal = getCancelICal($masterEnrollId, 'TP'); // to be used when whole tp moves to waitlist status due to enrolling to waitlisted class from a confirmed class.
	    	}
	      expDebug::dPrint(' class enroll updation start = ' , 4);
	      $updateStmt = db_update('slt_enrollment');
	      $updateStmt->condition('user_id', $userId);
	      $updateStmt->condition('master_enrollment_id', $masterEnrollId);
	      $updateStmt->condition('id', $enrollId);
	      $updateStmt->fields(array(
	                          'class_id'          => $currentClassId,
	                          'reg_status'        => $regStatus,
	                          'updated_on'        => now(),
	                          'updated_by'        => $userId,
	                          'comp_status'       => ($regStatus == 'lrn_crs_reg_cnf') ? 'lrn_crs_cmp_enr' : NULL,
	      										'waitlist_flag'     => ($regStatus == 'lrn_crs_reg_cnf') ? NULL : 'lrn_crs_reg_wtl',
	      										'waitlist_priority' => ($regStatus == 'lrn_crs_reg_cnf') ? NULL : $waitlistNumber,
	                          ));
	      $isUpdated = $updateStmt->execute();
	      if($isUpdated){
	        //Insert Enrollment Content Mapping
	        removeEnrollmentContentMapping($enrollId);
	        if ($currentClassDelType == 'wbt' || $currentClassDelType == 'vod') {
	          $enrollContentObj = new stdClass();
	          $enrollContentObj->enroll_id = $enrollId;
	          $enrollContentObj->class_id  = $currentClassId;
	          $enrollContentObj->course_id = $courseId;
	          $enrollContentObj->user_id   = $userId;
	          insertEnrollmentContentMapping($enrollContentObj);
	        }
	        //Class Switch Notification
	        $notifyResult = new stdClass();
	        $oldClsDetails = getClassInfoForNotifications($prevClassId);
	        $newClsDetails = getClassInfoForNotifications($currentClassId);
	        $tpDetails     = getTrainingProgramDetails($programId, array('title','object_type'));
	        $courseInfo =   getCourseInfoForNotifications($courseId);
	        if($courseInfo != null) {
	          $courseTitle = convertNullValToNullStr($courseInfo['title']);
	        }
	        $oldRegStatus  = getProfileListItemName($classRegStatus);
	        $newRegStatus  = getProfileListItemName($regStatus);
	        $personInfo    = getPersonDetailsForNotifications($userId);
	        $notifyResult->old_delivery_type  = convertNullValToNullStr($oldClsDetails['delivery_type_name']);
	        $notifyResult->old_class_title  = convertNullValToNullStr($oldClsDetails['title']);
	        $notifyResult->old_class_code   = convertNullValToNullStr($oldClsDetails['code']);
	        $notifyResult->old_class_status = convertNullValToNullStr($oldRegStatus);
	        $notifyResult->delivery_type    = convertNullValToNullStr($newClsDetails['delivery_type_name']);
	        $notifyResult->new_class_title  = convertNullValToNullStr($newClsDetails['title']);
	        $notifyResult->new_class_code   = convertNullValToNullStr($newClsDetails['code']);
		    $notifyResult->additional_info   = convertNullValToNullStr($newClsDetails['additional_info']);
			$notifyResult->addn_notification_show   = convertNullValToNullStr($newClsDetails['addn_notification_show']);
			
	        $notifyResult->new_class_status = convertNullValToNullStr($newRegStatus);
	        $notifyResult->tp_title         = convertNullValToNullStr($tpDetails['title']);
	        $notifyResult->first_name       = convertNullValToNullStr($personInfo['first_name']);
	        $notifyResult->lang_code        = $newClsDetails['lang_code'];
	        $notifyResult->course_title     = $courseTitle;
	        $iCal = null;
	        if ($tpOverallStatus == 'lrn_tpm_ovr_enr' && $regStatus == 'lrn_crs_reg_cnf') {
	        	// Only class_switch notification will get sent. ical class cancel and class enroll needs to be sent, however UID should have tp id.
	        	$tpClassPublishICal = getPublishICal($masterEnrollId, 'TP', $currentClassId);
	        	$iCal = consolidateICals(array($tpClassCancelICal, $tpClassPublishICal));
	        }
	        insertChangeClassNotification('class_switch',$objectId1,$notifyResult,$userId, $iCal); //39278: iCal files were not sent with the Change Class Notification in TP

	        //Audit Trail
	        $defaultvalue[0] =new stdClass();
	        $defaultvalue[0]->id=$enrollId;
	        $defaultvalue[0]->class_id=$prevClassId;
	        $defaultvalue[0]->reg_status=$classRegStatus;
	        $defaultvalue[0]->updated_on=now();
	        $defaultvalue[0]->updated_by=$user_id;
	        $insertAudit = array('id'      =>$enrollId,
	                          'class_id'   => $currentClassId,
	                          'reg_status' => $regStatus,
	                          'updated_on' => now(),
	      										'updated_by' => $userId
	                    );
	  	  	$objType =$select->execute()->fetchField();
	  	  	make_audit_trail_entries( $insertAudit, $userId ,$defaultvalue, $programId ,'updated',$tpDetails['object_type'],'exp_sp_lnrlearningplan','classswitch',
	                                          array('id','class_id','reg_status','updated_on','updated_by'),$userId);

	        if($regStatus == 'lrn_crs_reg_wtl' && $tpOverallStatus =='lrn_tpm_ovr_enr'){
	          $updateMasterEnr = db_update('slt_master_enrollment');
	          $updateMasterEnr->condition('id', $masterEnrollId);
	          $updateMasterEnr->condition('user_id', $userId);
	          $updateMasterEnr->condition('overall_status',array('lrn_tpm_ovr_enr'), 'IN');
	          $updateMasterEnr->fields(array(
	                              'overall_status' => 'lrn_tpm_ovr_wtl',
	                              'updated_on'    => now(),
	          										'updated_by'    => $userId
	                              ));
	          $updateMasterEnr->execute();
	           insertNotification('certCurrLPWaitlistRegister', $masterEnrollId, $programId, $userId, $userId, $tpCancelICal);
	          expDebug::dPrintDBAPI('update=', $updateMasterEnr);
	        }
	        elseif($regStatus == 'lrn_crs_reg_cnf' && $tpOverallStatus =='lrn_tpm_ovr_wtl'){
	          //waitlist promotion update for changing class user
	          waitlistTPPromotionUpdate($masterEnrollId,$userId); // has ical generation
	        }
	        /* Waitlist Promotion Call for $prevClassId */
	        waitlistPromotionUpdate($prevClassId,$classRegStatus);
	      }
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
   }
  } catch (Exception $ex) {
  	watchdog_exception('getAvailableClassResults', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Register the selected Class from change class popup in My Enrollment
 */
function MyEnrollmentChangeClass($userId, $currentClassId, $enrollId, $waitlist,$FromApi = ''){
	try{
	expDebug::dPrint('changeClassEnroll called. enrollId->' . $enrollId .' classId -> '.$currentClassId ,4);
		if($enrollId && $currentClassId){
			$select = db_select('slt_enrollment','enr');
			$select->addField('enr', 'reg_status');
			$select->addField('enr', 'comp_status');
			$select->addField('enr', 'class_id');
			$select->addField('enr', 'course_id');
			$select->addField('enr', 'is_compliance');
			$select->condition('enr.id', $enrollId);
			expDebug::dPrintDBAPI('enroll status query:', $select);
			$result = $select->execute()->fetchAll();
			expDebug::dPrint('enroll status query:' . print_r($result,true),3);
			$classRegStatus  = $result[0]->reg_status;
			$clsCompStatus   = $result[0]->comp_status;
			$prevClassId     = $result[0]->class_id;
			$courseId        = $result[0]->course_id;
			
				$currentWaivedOrNot = checkComplianceAndMandatory($enrollId,'class', true,$currentClassId);
				$checkCourseClassCompliance = checkCourseClassCompliance($courseId);

				if($currentWaivedOrNot[0]->exempted_status == 1 && $checkCourseClassCompliance[0]->is_compliance == 1){
					$is_compliance = NULL;
				}elseif($currentWaivedOrNot[0]->exempted_status == 0 && $checkCourseClassCompliance[0]->is_compliance == 1){
					$is_compliance = '1';
				}elseif(empty($currentWaivedOrNot) && $checkCourseClassCompliance[0]->is_compliance == 1){
					$is_compliance = '1';
				}elseif(empty($currentWaivedOrNot) && $checkCourseClassCompliance[0]->is_compliance != 1){
					$is_compliance =$result[0]->is_compliance;
				}

			$regStatus = 'lrn_crs_reg_cnf';
			$errObj = array();
			$updStatus = '';
			$currentClassDelType = getClassDeliveryType($currentClassId, 'Short');
			$previousClassDelType = getClassDeliveryType($prevClassId, 'Short');
			$cancelICal = null;
			if((!empty($clsCompStatus) && $clsCompStatus != 'lrn_crs_cmp_enr' && $clsCompStatus != 'lrn_crs_cmp_nsw')){
				$errObj[] = t('MSG587');
			}
			/* elseif(courseConflictCheckInClsSwitch($userId, $courseId, $currentClassId, $enrollId)){
				$errObj[] = t('ERR049');
			} */
			else if(enrollmentsConflictCheck($userId, $currentClassId, 'Class')){
			 $errObj[] = t('ERR048');
			}
			elseif ($currentClassDelType == 'ilt' || $currentClassDelType == 'vc') {
				// Confirm that the user not assigned this class to instruct for learners
				$conflictInstructor = getClassInstructorConflict($userId, $currentClassId);
				if ($conflictInstructor == false ) {
					$errObj[] = t('ERR075');
				}
				else{
					$waitlistNumber = waitListCheck($courseId, $currentClassId);
					if($waitlistNumber == 0 || $waitlistNumber == -1) {
						$regStatus = 'lrn_crs_reg_cnf';
						if ($classRegStatus == 'lrn_crs_reg_cnf' && ($previousClassDelType == 'ilt' || $previousClassDelType == 'vc')) {
							$cancelICal = getCancelICal($enrollId, 'Class');
						}
					}
					else { // waitlist - do further check against waitlist capacity, otherwise class is full.
						$classWaitlistCapacity = getClassWaitlistCapacity(null, $currentClassId);
						expDebug::dPrint(' $classWaitlistCapacity = ' . print_r($classWaitlistCapacity, true) , 4);
						$classWaitlistCapacity = (is_null($classWaitlistCapacity))? 0 : $classWaitlistCapacity;
						if ($classWaitlistCapacity >= $waitlistNumber) {
							$regStatus = 'lrn_crs_reg_wtl';
							if ($classRegStatus == 'lrn_crs_reg_cnf' && ($previousClassDelType == 'ilt' || $previousClassDelType == 'vc')) {
								$cancelICal = getCancelICal($enrollId, 'Class');
							}
						}
						else {
							$errObj[] = t('ERR022');
						}
					}
				}
			}
			expDebug::dPrint(' class enroll error = ' .print_r($errObj,true), 4);
			if(!count($errObj)){
				if($result[0]->is_compliance != 1){
					$manCnt = getMandatoryOrNot($currentClassId,$userId);
				}
				expDebug::dPrint(' class enroll updation start = ' , 4);
				$updateStmt = db_update('slt_enrollment');
				$updateStmt->condition('user_id', $userId);
				$updateStmt->condition('id', $enrollId);
				$updateStmt->fields(array(
						'class_id'          => $currentClassId,
						'reg_status'        => $regStatus,
						'mandatory' 		=> ($manCnt == 'cre_sys_inv_man') ? 1 : NULL,
						'is_compliance' 	=>  $is_compliance,
						'updated_on'        => now(),
						'updated_by'        => $userId,
						'comp_status'       => ($regStatus == 'lrn_crs_reg_cnf') ? 'lrn_crs_cmp_enr' : NULL,
						'waitlist_flag'     => ($regStatus == 'lrn_crs_reg_cnf') ? NULL : 'lrn_crs_reg_wtl',
						'waitlist_priority' => ($regStatus == 'lrn_crs_reg_cnf') ? NULL : $waitlistNumber,
				));
				$isUpdated = $updateStmt->execute();
				if($isUpdated){
					//Insert Enrollment Content Mapping
					removeEnrollmentContentMapping($enrollId);
					if ($currentClassDelType == 'wbt' || $currentClassDelType == 'vod') {
						$enrollContentObj = new stdClass();
						$enrollContentObj->enroll_id = $enrollId;
						$enrollContentObj->class_id  = $currentClassId;
						$enrollContentObj->course_id = $courseId;
						$enrollContentObj->user_id   = $userId;
						insertEnrollmentContentMapping($enrollContentObj);
					}
					//Class Switch Notification
					$notifyResult = new stdClass();
					$oldClsDetails = getClassInfoForNotifications($prevClassId);
					$newClsDetails = getClassInfoForNotifications($currentClassId);
					$switchClassDetails = getClassDetailsForAuditTrail($prevClassId, array('title','delivery_type'));
					$courseInfo =   getCourseInfoForNotifications($courseId);
					if($courseInfo != null) {
						$courseTitle = convertNullValToNullStr($courseInfo['title']);
					}
					$oldRegStatus  = getProfileListItemName($classRegStatus);
					$newRegStatus  = getProfileListItemName($regStatus);
					$personInfo    = getPersonDetailsForNotifications($userId);
					$notifyResult->old_delivery_type  = convertNullValToNullStr($oldClsDetails['delivery_type_name']);
					$notifyResult->old_class_title  = convertNullValToNullStr($oldClsDetails['title']);
					$notifyResult->old_class_code   = convertNullValToNullStr($oldClsDetails['code']);
					$notifyResult->old_class_status = convertNullValToNullStr($oldRegStatus);
					$notifyResult->delivery_type  = convertNullValToNullStr($newClsDetails['delivery_type_name']);
					$notifyResult->new_class_title  = convertNullValToNullStr($newClsDetails['title']);
					$notifyResult->new_class_code   = convertNullValToNullStr($newClsDetails['code']);
					$notifyResult->additional_info   = convertNullValToNullStr($newClsDetails['additional_info']);
			        $notifyResult->addn_notification_show   = convertNullValToNullStr($newClsDetails['addn_notification_show']);
					$notifyResult->new_class_status = convertNullValToNullStr($newRegStatus);
					$notifyResult->tp_title         = $courseTitle;
					$notifyResult->first_name       = convertNullValToNullStr($personInfo['first_name']);
					$notifyResult->lang_code        = $newClsDetails['lang_code'];
					$notifyResult->course_title     = $courseTitle;
					$publishICal = null;
					if ($regStatus == 'lrn_crs_reg_cnf' && ($currentClassDelType == 'ilt' || $currentClassDelType == 'vc')) {
						$publishICal = getPublishICal($enrollId, 'Class');
					}
					$iCal = consolidateICals(array($cancelICal, $publishICal));
					insertChangeClassNotification('class_switch', $objectId1, $notifyResult, $userId, $iCal);

					//Audit Trail
					$defaultvalue[0] =new stdClass();
					$defaultvalue[0]->id=$enrollId;
					$defaultvalue[0]->class_id=$prevClassId;
					$defaultvalue[0]->reg_status=$classRegStatus;
					$defaultvalue[0]->updated_on=now();
					$defaultvalue[0]->updated_by=$user_id;
					$insertAudit = array('id'      =>$enrollId,
							'class_id'   => $currentClassId,
							'reg_status' => $regStatus,
							'updated_on' => now(),
							'updated_by' => $userId
					);
					$objType =$select->execute()->fetchField();
					//Audit trail Entry
					make_audit_trail_entries( $insertAudit, $userId ,$defaultvalue, $prevClassId ,'updated',$switchClassDetails['delivery_type'],'exp_sp_lnrenrollment','classswitch',
								array('id','class_id','reg_status','updated_on','updated_by'),$userId);

					if ($previousClassDelType == 'ilt' || $previousClassDelType == 'vc') {
						/* Waitlist Promotion Call for $prevClassId */
						waitlistPromotionUpdate($prevClassId,$classRegStatus);
					}

				}

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

function getAvailableClassResultsAPI()
{
	try{
		/*Function added for Switch class feature in mobile - To get the classes available to switch and to get the swithc between available classes*/		
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrlearningplan/exp_sp_lnrlearningplan.module');
	if($_REQUEST['action'] == "getclasses")
	{
		$userId = $_REQUEST['userid'];
		$changesClassId = $_REQUEST['changesClassId'];
		$courseId = $_REQUEST['courseId'];
		$myEnrollChangeCls = $_REQUEST['myEnrollChangeCls'];
		$classIdArr = getAvailableClassResults($userId,$changesClassId,$courseId,$myEnrollChangeCls );
		
		if($classIdArr)
		{
		$createDate = new DateTime($classIdArr[0]->registration_end_on);
		$registration_end_on = $createDate->format('Y-m-d');
		$presentDate=date('Y-m-d');
		if(strtotime($registration_end_on) >= strtotime($presentDate) || empty($createDate)){
			$classIdArr[0]->registration_ended= "false";
		}
		else{
			$classIdArr[0]->registration_ended="true";
		}
		}
		
		
		$resClsDetials->availableclasses = $classIdArr;
		expDebug::dPrint(' getAvailableClassResultsAPI $$resClsDetials= ' . print_r($resClsDetials, true) , 4);
		return $resClsDetials;
	}
	else
	{
		$userId = $_REQUEST['userid'];
		$currentClassId = $_REQUEST['currentClassId'];
		$enrollId = $_REQUEST['enrollId'];
		$waitlist = $_REQUEST['waitlist'];
		$myEnrollChangeCls = $_REQUEST['myEnrollChangeCls'];
		if ($myEnrollChangeCls == "Enroll" ){
			$classIdArr = changeClassEnroll($userId, $currentClassId, $enrollId, $waitlist, $myEnrollChangeCls,'1');
		}else{
			$classIdArr =  MyEnrollmentChangeClass($userId, $currentClassId, $enrollId, $waitlist,'1');
		}
		$resClsDetials->availableclasses = $classIdArr;
		expDebug::dPrint(' getAvailableClassResultsAPI $$resClsDetials= ' . print_r($resClsDetials, true) , 4);
		return $resClsDetials;
	}
	}catch (Exception $ex) {
		watchdog_exception('getAvailableClassResultsAPI', $ex);
		expertusErrorThrow($ex);
	}
}


function insertChangeClassNotification($notificationCode, $objectId1,$notifyResult,$userId, $iCal){
	try{
		/*
		 * Start # 0039858 -  Salesforce Chatter Feed
		* Added By : Ganesh Babu V, Oct 17th 2014 4:45 PM
		* Description: Send the notification parametre to post the feed to salesforce user according to notification type and notification code
		* Notification Type : Class Change Notification
		* Ticket : #0039858: Implement the salesforce chatter feed in ExpertusONE
		*/

		if(module_exists('salesforce_integration')){
			module_load_include('inc','salesforce_integration','salesforce_integration_chatter');
			$ch_clasa_det_arr=(array)$notifyResult;
			postSalesforceFeed('class_switch','class_switch',$ch_clasa_det_arr, $objectId1, $userId);
		}

		/* End # 0039858 -  Salesforce Chatter Feed */

  $notifyOverrideFlag = null;
  $overrideContent = '';
  $overrideInfo = getEntityNotificationOverrideInfo($objectId1, 'cre_sys_obt_cls', $notificationCode, 'Email');
  expDebug::dPrint(' contentmoveusers $overrideInfo = ' . print_r($overrideInfo, true) , 4);
  if($overrideInfo != null) {
    $notifyOverrideFlag = $overrideInfo['notify_flag'];
    $overrideContent = $overrideInfo['override_content'];
  }
  $notificationTemplateInfo = getNotificationTemplateInfo($notificationCode, 'cre_sys_lng_eng');
  $notifyFlag = null;
	// Both 'Y' and 'cre_ntn_sts_atv' are active statuses
	if ($notificationTemplateInfo['status'] == 'cre_ntn_sts_atv') {
		$notifyFlag = 'Y';
	}
  if ($notifyOverrideFlag == 'cre_ntn_sts_atv') {  // Added
		$notifyOverrideFlag = 'Y';
	}
	$frameString = true;
	if ($notifyFlag != 'Y' || ($notifyOverrideFlag != null && $notifyOverrideFlag != 'Y')) {
		if($notifyFlag != 'Y'){
			// If the notification is disabled status in template, do not generate notification
    	expDebug::dPrint('Notification is disabled',3);
    	$frameString = false;
		}else{
			//If the notification status has been overridden and set to disabled, do not generate notification.
			expDebug::dPrint(' Notification is overridded and disabled',3);
			$frameString = false;
		}
	}
	if($frameString){
    	$notificationInfo = null;
		  $notificationInfo['tokens_string'] =         'first_name>|' . $notifyResult->first_name . '~|' .
				  																				 'delivery_type>|' . $notifyResult->delivery_type . '~|' .
                                                   'tp_title>|' . $notifyResult->tp_title . '~|' .
                                                   (($notifyResult->addn_notification_show == 1) ? (($notifyResult->additional_info) ? ('add_notes>|'. $notifyResult->additional_info) : ('add_notes>|'.'-')) : '') . '~|' .  
												   
                                                   'course_title>|' . $notifyResult->course_title . '~|' .
                                                   'class_title>|' . $notifyResult->new_class_title . '~|' .
                                                   'course_code>|' . $notifyResult->new_class_code . '~|' .
		  																						 'class_status>|' . $notifyResult->new_class_status . '~|' .
		  																						 'old_delivery_type>|' . $notifyResult->old_delivery_type . '~|' .
                                                   'old_class_title>|' . $notifyResult->old_class_title . '~|' .
                                                   'old_class_code>|' . $notifyResult->old_class_code . '~|' .
                                                   'old_class_status>|' . $notifyResult->old_class_status;

		  $notificationInfo['message_id']    = $notificationCode;
		  $notificationInfo['message_type']  = 'Class Switch Notification';
		  $notificationInfo['send_type']     = 'php mailer';
		  $notificationInfo['lang_code']     = $notifyResult->lang_code;
		  callCommonNotificationInsert($notificationCode, $notificationInfo, $notificationTemplateInfo, $userId, $overrideContent, $iCal);
	}
	}catch (Exception $ex) {
		watchdog_exception('insertChangeClassNotification', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * Get the Certificate expired date
 */
function getExpiredDate($completedDate, $expiredValue ,$expiredUnit,$isfuture =''){
	try{
   $expiredDate="";
   $expiredDateformatted="";
   $isfutureDate='';
   if(!empty($completedDate)){
       $completedDateFormat = date("Y-m-d",strtotime($completedDate));
       if($expiredUnit =='years' && !empty($expiredValue)) {
        $expiredDate = strtotime(date("Y-m-d", strtotime($completedDateFormat)) . " +".$expiredValue." years");
        $expiredDateformatted = date("M d, Y", $expiredDate);
       }elseif($expiredUnit =='months' && (!empty($expiredValue))){
         $expiredDate = strtotime(date("Y-m-d", strtotime($completedDateFormat)) . " +".$expiredValue." months");
         $expiredDateformatted = date("M d, Y", $expiredDate);
       }elseif($expiredUnit =='days' && (!empty($expiredValue))){
         $expiredDate = strtotime(date("Y-m-d", strtotime($completedDateFormat)) . " +".$expiredValue." days");
         $expiredDateformatted = date("M d, Y", $expiredDate);
       }
   }
  // $todaydate = time();
  $expiry_date = date('Y-m-d' ,$expiredDate );
   expDebug::dPrint(' Names expiryy= '.$expiry_date );
   $presentDate=date('Y-m-d');
   expDebug::dPrint(' Names current date= '.$presentDate );
   if($isfuture){
    $isfutureDate =   (!empty($expiredDate) && $expiry_date>$presentDate )? 'yes' :'no';
   	return $isfutureDate;
   }
   return $expiredDateformatted;
   }catch (Exception $ex) {
   	watchdog_exception('getExpiredDate', $ex);
   	expertusErrorThrow($ex);
   }
}
function getClassDetailsForAuditTrail($classId, $fields) {
  try{
    if (empty($fields)) {
      // Get all fields
      $fields = array('id','title', 'code', 'description', 'short_desc', 'price', 'delivery_type',
          'currency_type', 'status', 'created_by', 'created_on', 'updated_by', 'updated_on',
          'custom0', 'custom1', 'custom2', 'custom3', 'custom4');
    }
    // Select table
    $selectStmt = db_select('slt_course_class', 'cls');
    // Set conditions
    $selectStmt->condition('cls.id', $classId);
    // Select fields and/or add expressions
    foreach($fields as $fieldName) {
      $selectStmt->addField('cls', $fieldName, $fieldName);
    }
    expDebug::dPrintDBAPI(' $selectStmt = ' ,$selectStmt);
    // Execute query
    $result = $selectStmt->execute();
    return $result->fetchAssoc();
  }catch (Exception $ex) {
    watchdog_exception('getClassDetailsForAuditTrail', $ex);
    expertusErrorThrow($ex);
  }
}
function getEnforceSequenceStatus($prgId){
	try{
		$select = db_select('slt_program','prog');
		$select->addField('prog','enforce_sequence');
		$select->condition('prog.id', $prgId,'=');
		expDebug::dPrintDBAPI(' $select query for sequencing in learner side ' ,$select);

		$required  = $select->execute()->fetchField();
		return $required;
	}catch(Exception $ex){
		watchdog_exception('enforce Sequence', $ex);
		expertusErrorThrow($ex);
	}
}

function get_course_sequence($master_enrollment_id,$recertifyPath,$enr_id){
	try{
		 /*  if(isset($_SESSION['menrid'][$master_enrollment_id]['next_launchable_sequence']) && empty($enr_id)) {
			return unserialize($_SESSION['menrid'][$master_enrollment_id]['next_launchable_sequence']);
		}   */

		$selectSeq = db_select('slt_enrollment', 'enr');
		$selectSeq->leftJoin('slt_master_enrollment','menr','menr.id = enr.master_enrollment_id');
		$selectSeq->leftJoin('slt_module_crs_mapping','map','map.program_id=menr.program_id and map.course_id = enr.course_id');
		$selectSeq->leftJoin('slt_course_class','cls','cls.id = enr.class_id');
		$selectSeq->innerJoin('slt_module','module','module.id = map.module_id');
		$selectSeq->addField('enr','id','enrid');
		$selectSeq->addField('menr','id','menrid');
		$selectSeq->addField('cls', 'id', 'class_id');
		$selectSeq->addField('enr', 'comp_status', 'comp_status');
		$selectSeq->addField('cls', 'title', 'cls_title');
		$selectSeq->addField('map', 'sequence', 'sequence');
		$selectSeq->addField('map', 'is_required', 'is_required');
		$selectSeq->condition('menr.id', $master_enrollment_id,'=');
		$selectSeq->condition('module.sequence', $recertifyPath,'=');
		//$selectSeq->condition('map.is_required', 'Y','=');
		
		
		if(!empty($enr_id)) {
		   
			$selectSeq->condition('enr.id', $enr_id,'=');
		} else {
		    $selectSeq->condition('map.is_required', 'Y','=');
			$selectSeq->condition('enr.comp_status', 'lrn_crs_cmp_cmp','!=');
		}
		$selectSeq->orderBy('map.sequence');
		$selectSeq->range(0, 1);
		expDebug::dPrintDBAPI('$selectSeq abhi= ', $selectSeq);
		$resultSeq = $selectSeq->execute()->fetchObject();
		/* if(empty($enr_id)) {
			// this block is to fetch next launchable sequence and hence can be saved in session
			$_SESSION['menrid'][$master_enrollment_id]['next_launchable_sequence'] = serialize($resultSeq);
		} */
		
		return $resultSeq;
		
	}catch(Exception $ex){
		watchdog_exception('course enforce Sequence', $ex);
		expertusErrorThrow($ex);
	}
}
// Autocomplete for tp text search filter
function getAutoCompleteForTPText() {
try {
		$searchText =  $_GET['z'];
		$loggedUserId = getSltpersonUserId();
		$solrSource = getConfigValue ( 'solr_search' );
		if ($solrSource == 0) {
			$select = db_select('slt_master_enrollment', 'mas');
			$select->distinct();
			$select->addField('prg', 'id', 'prgid');
			$select->addField('prg', 'title', 'title');
			$select->innerJoin('slt_program','prg','prg.id=mas.program_id');
			$select->leftjoin('slt_tag_entity','tagentity','tagentity.entity_id=mas.program_id');
			$select->leftjoin('slt_tagdefn', 'tagdef', 'tagentity.tagid=tagdef.id');
			$select->condition(db_or()
					->condition('prg.title', '%' . db_like($searchText) . '%', 'LIKE')
					->condition('prg.code', '%' . db_like($searchText) . '%', 'LIKE')
					->condition('prg.description', '%' . db_like($searchText) . '%', 'LIKE')
					->condition('prg.short_desc', '%' . db_like($searchText) . '%', 'LIKE')
					->condition('tagdef.tagname', '%' . db_like($searchText) . '%', 'LIKE')
			);
			$select->condition('mas.user_id',$loggedUserId,'=');
			$select->condition('mas.is_current','Y','=');
		//	$select->groupBy ( 'prg.id');
			expDebug::dPrintDBAPI(' $select tp autocomplete query = ' , $select);
			$names = $select->execute()->fetchAll();
		} else {
			require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_mylearning_program.php';
				try {
					$solrObj = new MyLearningProgramSolrSearch();
					$solrList = $solrObj->myProgramSolrSearchAutoComplete();
					$names = $solrList ['records'];
					expDebug::dPrint ( ' $prgList123 = ' . print_r ( $names, true ), 4 );
					$numCatalog = $solrList ['recCount'];
					expDebug::dPrint ( ' Total matiching records ' . print_r ( $numCatalog, true ), 4 );
				} catch ( Exception $e ) {
					expDebug::dPrint ( "Error in Solr Search for Text search autocomplete - " . print_r ( $e->getMessage (), true ), 1 );
				}

		}
		expDebug::dPrint(' Names = '. print_r($names, true) , 3);
	  	foreach($names as $result){
	  		if ($solrSource == 0) {
	  			print rawurlencode($result->title) . "\n";
	  		} else{
	  			print rawurlencode($result->prg_title) . "\n";
	  		}
	    	
	    }
} catch (Exception $ex) {
		watchdog_exception('getAutoCompleteForTPText', $ex);
		expertusErrorThrow($ex);
	}
}
// Auto Complete for Location 
function getLPClassLocation(){
	try {
		$loggedInUserId = getSltpersonUserId();
		$searchCurText =  $_GET['z'];
		$select = db_select('slt_location', 'location');
		$select->leftjoin('slt_state','state','state.state_code=location.state and state.country_code=location.country');
		$select->leftJoin('slt_country','country','location.country=country.country_code');
		$select->addField('location','name');
		//$select->orderBy('location.name');
		$select->condition(db_or()
				->condition('location.name', '%' . db_like($searchCurText) . '%', 'LIKE')
				->condition('state.state_name', '%' . db_like($searchCurText) . '%', 'LIKE')
				->condition('country.country_name', '%' . db_like($searchCurText) . '%', 'LIKE')
		);
		expDebug::dPrintDBAPI("Learning Plan Autocomplete --->> " , $select);
		$result = $select->execute()->fetchAll();
		foreach ($result as $loc) {
			print $loc->name . "\n";
		}
	} catch (Exception $ex) {
		watchdog_exception('getLPClassLocation', $ex);
		expertusErrorThrow($ex);
	}
}
?>
