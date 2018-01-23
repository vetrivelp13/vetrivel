<?php
/*SELECT DISTINCT crs.id Id, cls.id ClassId,cls.title title, cls.description cdesc,ses.start_date eventdate,
      ses.start_time startdatetime,cls.code,enr.reg_status,
 IF(enr.reg_status IS NOT NULL,'Registered','Not Registered') statusText 
  FROM slt_course_template crs 
  JOIN  slt_course_class cls ON crs.id=cls.course_id 
  JOIN slt_course_class_session ses  ON cls.id=ses.class_id AND cls.course_id=ses.course_id 
  LEFT JOIN slt_enrollment enr ON enr.class_id = cls.id AND enr.user_id=1 AND enr.reg_status IN ('lrn_crs_reg_ppm','lrn_crs_reg_cnf')
  WHERE IF(DATE(ses.start_date) = DATE(NOW()),ses.start_time > DATE_FORMAT(NOW(),'%H:%i'),DATE(ses.start_date) > DATE(NOW()));*/
    
  function getUpcomingTrainings($dateRange = '', $mode = ''){
    try{
    	$userId = getSltpersonUserId();
    	$timezone_details = getPersonDetails($userId);
    	// Added by Vincent on Oct 30, 2013 for #0028593
    	$timeZone = date_default_timezone(false);
    	$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
    	if($userId){
    		$UserPHPTimezoneName		= getUserPHPTimezoneName();
    		$vUserPHPTimezonename		= $UserPHPTimezoneName[0]->zone_name;
    		if(!$UserPHPTimezoneName[0]->zone_name){
    			$vUserPHPTimezonename 	= date_default_timezone(FALSE);
    		}
    	}else{
    		$vUserPHPTimezonename 		= date_default_timezone(FALSE);
    	}
    	$userAccess  						= getUserAccessPriviledges($userId);
    	$isInstructor						= is_instructor($userId);
    	
    	// SOLR check starts here
    	$srcSource = getConfigValue('solr_search');
    	$solrResult = FALSE;
    	if($srcSource == 1){
    		require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_mycalendar.php';
    		try{
    			expDebug::dPrint("Solr Search Testing",1);
    			$solrObj = new MyCalendarSolrSearch();
    			$search_results = $solrObj->getUpcomingList($dateRange,$mode);
    			expDebug::dPrint('Result set from solr'.print_r($search_results,true),5);
    			$result = $search_results['result'];
    			expDebug::dPrint("Result in Solr Search -- ".print_r($result,true),1);
    			$solrResult = TRUE;
    		}
    		catch(Exception $e){
    			expDebug::dPrint("Error in Solr Search -- ".print_r($e->getMessage(),true),1);
    		}
    	}
    	
    	else {
   	  $toDay = Date("Y-m-d H:i:s"); 
  	  $toDay1 = Date("Y-m-d");  
     
      expDebug::dPrint("Date range entered is::: " . $dateRange, 4);
      expDebug::dPrint("Mode type entered is:::  " . $mode, 4);
      if ($mode == 'day') {
      	$startDate 	= $dateRange . ' 00:00:00';
      	$endDate 	= $dateRange . ' 23:59:59';
      } else {
      	$startDate 	= $dateRange . '-01 00:00:00';
      	$endDate 	= date('Y-m-t 23:59:59', strtotime($startDate));
      }
      
      expDebug::dPrint("training start date: " . $startDate, 4);
      expDebug::dPrint("training end date: " . $endDate, 4);
      
      $dateType = 'future';
      if (strtotime($endDate) <= strtotime(date('Y-m-d H:i:s'))) {
      	$dateType = 'past';
      }
      expDebug::dPrint("Date type variable infor" . var_export($dateType, 1), 4);
     
      
      $select = db_select('slt_course_class', 'cls');    
      // $select->join('slt_node_learning_activity', 'snl', 'cls.id = snl.entity_id AND snl.entity_type = \'cre_sys_obt_cls\'');
      $select->join('slt_course_class_session', 'ses', 'cls.id = ses.class_id');
      
      $select->leftJoin('slt_enrollment', 'enr', 'enr.class_id = cls.id and enr.user_id = '.$userId.' and (enr.reg_status in (\'lrn_crs_reg_ppm\',\'lrn_crs_reg_cnf\') OR enr.comp_status =\'lrn_crs_cmp_cmp\')');
      //$select->leftJoin('slt_entity_profile_mapping', 'sepm', 'cls.id = sepm.entity_id AND sepm.entity_type = \'27\'');
      $select->leftjoin('slt_profile_list_items', 'splt_timezone', 'splt_timezone.code=ses.timezone');
      //$select->leftjoin('slt_mro_mapping', 'mro_map', 'cls.id = mro_map.entity_id AND mro_map.entity_type = \'cre_sys_obt_cls\' AND FIND_IN_SET(:jobRole, mro_map.job_role_id)', array(':jobRole' => $userAccess->job_role));
      //$select->leftjoin('slt_catalog_access', 'catacs', 'cls.id = catacs.entity_id and catacs.entity_type = \'cre_sys_obt_cls\'');
      //$select->leftJoin('slt_group_mapping','group_map','group_map.entity_id = cls.id and group_map.entity_type = \'cre_sys_obt_cls\'');
     // $select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');
      
      /*
      if ((isset($_SESSION['availableFunctionalities']->exp_sp_instructor_desk) )&& $isInstructor == 1 ){
      	$select->leftjoin('slt_object_role_mapping', 'objrollmap', 'objrollmap.user_id='.$userId.' and objrollmap.object_type=\'cre_sys_obt_cls\' and objrollmap.object_id=cls.id');
      }
      */
      $select->addField('cls','id', 'classid');
      $select->addField('ses','start_date', 'eventdate');
      $select->addField('cls','title', 'title');
      $select->addField('ses','timezone', 'timezone');
      $select->addField('ses','start_time', 'startdatetime');
      $select->addField('ses','end_time', 'enddatetime');
      $select->addField('cls','delivery_type', 'delivery_type');
      $select->addField('splt_timezone','attr2', 'session_timezone');
      //$select->addField('ses_ins','instructor_id','instructor_id');
      
      if ($mode == 'day') {
      	  $select->addExpression('COUNT(ses.id)', 'session_count');
	      $select->addField('cls','short_description', 'cdesc');
	      $select->addField('cls','code', 'code');
	      $select->addField('enr','reg_status', 'reg_status');
	      $select->addField('enr','comp_status', 'comp_status');
	      $select->addField('splt_timezone','attr1', 'session_timezone_code');
	      $select->addField('splt_timezone','attr4', 'tz_code');
	      $select->addExpression('CONCAT(date(ses.start_date),\' \',ses.start_time,\':00\')','fulldate');
      }
      //$select->addExpression('IF(mro_map.mro_flag=\'cre_sys_inv_opt\', \'Optional\', IF(mro_map.mro_flag=\'cre_sys_inv_man\', \'Mandatory\', IF(mro_map.mro_flag=\'cre_sys_inv_rec\',\'Recommended\', \'\')))','mro_flag');
      $select->addExpression('IF(enr.comp_status=\'lrn_crs_cmp_cmp\',\'Completed\',IF(enr.reg_status IS NOT NULL,\'Registered\',\'Not Registered\'))', 'statustext');
      // Added by Vincent on Oct 30, 2013 for #0028593
      $select->addExpression("CONCAT( DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.end_time,':00')", 'session_end_datetime');
      if ((isset($_SESSION['availableFunctionalities']->exp_sp_instructor_desk) )&& $isInstructor == 1 ){
      	$select->leftJoin('slt_session_instructor_details', 'ses_ins', 'ses.id = ses_ins.session_id');
      	//$select->addExpression('IF(IFNULL(objrollmap.id,0)=0,\'No\',\'Instructor\')', 'myclass');
      	$select->addExpression('IF( FIND_IN_SET('.$userId.', GROUP_CONCAT(DISTINCT(ses_ins.instructor_id))) > 0,\'Instructor\',\'No\')', 'myclass');
      	$select->addExpression('GROUP_CONCAT(DISTINCT(ses_ins.instructor_id))', 'instructor_id');
      	//$select->addExpression('IF( if((select count(1) from slt_session_instructor_details as ses_ins1 where ses_ins1.class_id=cls.id and ses_ins1.instructor_id ='.$user_id.')>0,\'No\',\'Instructor\')', 'myclass');
      	$instOrQuery = '|| FIND_IN_SET('.$userId.', ses_ins.instructor_id) > 0';
      }else{
      	$select->addExpression('\'No\'', 'myclass');
      	$select->addExpression('\'\'', 'instructor_id');
      	$instOrQuery = '';
      }
      //addFilterForLoggedInUserRole($select, 'sepm');
      //filterByUserAccessPrivileges($select, 'cls','','','cre_sys_obt_cls');
      //$select->groupBy('cls.id');
      //$select->orderBy('fulldate','ASC');  
      //$select->orderBy('ses.start_time','ASC');
      $select->orderBy('statustext','DESC');
      $select->orderBy('myclass','ASC');
      $select->orderBy('ses.start_date','ASC');
      //$select->where('IF(DATE(ses.start_date) = DATE(NOW()),ses.start_time > DATE_FORMAT(NOW(),\'%H:%i\'),DATE(ses.start_date) > DATE(NOW()))');
      // Query update by vincent on Jan 29, 2013 for issue #0020966
      if ($dateType == 'past') {
      	$select->where(' cls.status IN(\'lrn_cls_sts_atv\', \'lrn_cls_sts_itv\')
      			 and IF((enr.reg_status IN (\'lrn_crs_reg_ppm\', \'lrn_crs_reg_cnf\') AND enr.comp_status NOT IN (\'lrn_crs_cmp_inc\', \'lrn_crs_cmp_exp\')) ' .$instOrQuery. ' ,1=1,0)');
      	 
      } else {
     	 $select->join('slt_person','per', 'per.id = ' . $userId);
     	 $select->leftjoin('slt_group_mapping', 'group_map', 'group_map.entity_id=cls.id AND group_map.entity_type = \'cre_sys_obt_cls\' and group_map.group_type = 0 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
     	 $select->leftjoin('slt_user_access_mapping', 'usrmap', '(usrmap.entity_id=cls.id AND usrmap.entity_type = \'cre_sys_obt_cls\' and usrmap.access_type = 0) and usrmap.user_id = per.id');
     	 $select->leftjoin('slt_groups', 'grp', 'grp.id=group_map.group_id and (grp.is_admin =0)');
     	 $select->leftjoin('slt_group_attributes','grpatt','grp.id=grpatt.group_id');
 		 $select->leftjoin('slt_person','per', 'per.id=17');
 		 $select->leftjoin('slt_person_jobrole_mapping','spjm','spjm.user_id = per.id');
     	 $select->where("IF((enr.reg_status IN ('lrn_crs_reg_ppm', 'lrn_crs_reg_cnf') AND enr.comp_status NOT IN ('lrn_crs_cmp_inc', 'lrn_crs_cmp_exp') ) " .$instOrQuery. ",(cls.status IN( 'lrn_cls_sts_atv', 'lrn_cls_sts_itv')),
      				   (ses.id=(select id from slt_course_class_session where class_id=cls.id AND ses.start_date >= curdate() order by start_date, start_time limit 1) AND cls.status = 'lrn_cls_sts_atv' ) AND
     	 		
     	 			(((((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
		   			AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
		   			AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
		   			AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
		   			AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
		   			AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
						AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id=per.id)>0,grp.job_role IS NULL OR FIND_IN_SET(ifnull(spjm.job_role,''),ifnull(grp.job_role,''))>0))
		   			AND (if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0))
		   			AND (CASE WHEN (grp.is_manager='Y' AND grp.is_instructor='Y')
                                                    THEN
                                                    (ifnull(per.is_manager,'N') = grp.is_manager or ifnull(per.is_instructor,'N') = grp.is_instructor)
                                                    WHEN (grp.is_manager='Y' AND grp.is_instructor='N')
                                                    THEN
                                                    (ifnull(per.is_manager,'N') = grp.is_manager)
                                                    WHEN (grp.is_manager='N' AND grp.is_instructor='Y')
                                                    THEN
                                                    (ifnull(per.is_instructor,'N') = grp.is_instructor)
                                                    ELSE
                                                    1=1
                                                    END)
					AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT(per.hire_date,'%Y-%m-%d'),1=0) OR
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT(per.hire_date,'%Y-%m-%d'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') 
     	 			AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1))
		   			AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(per.id,grp.added_users)>0) OR usrmap.user_id = ".$userId.")
     	 			OR (group_map.group_id IS NULL AND ((SELECT count(1)FROM slt_user_access_mapping usrmap WHERE (usrmap.entity_id=cls.id AND usrmap.entity_type = 'cre_sys_obt_cls' and usrmap.access_type = 0) )=0) )
     	 			))
     	 			
     	 		
     	 		)");
      }
      $select->orderBy('ses.start_time','ASC');
      // Added by Vincent on Oct 30, 2013 for #0028593
      //$select->groupBy('ses.id'); 
      //$select->groupBy("ses.id having session_end_datetime > IFNULL(CONVERT_TZ(now(), '".$defaultTimezone."' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone)),NOW())");
      $select->groupBy("ses.id HAVING session_end_datetime BETWEEN CONVERT_TZ('".$startDate."', '".$vUserPHPTimezonename."' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone)) AND CONVERT_TZ('".$endDate."', '".$vUserPHPTimezonename."' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone))");
      expDebug::dPrintDBAPI('MY CALENDAR INFO', $select); 
      $result = $select->execute()->fetchAll();
    	}
    expDebug::dPrint('MY CALENDAR INFO RESULT : ' . print_r($result, true) , 3);
    $set_result = array();
    $i=0;
    foreach ($result as $record) { 
    	/* conversion of timezone for vc classes */
    	expDebug::dPrint('MY CALENDAR INFO RESULT inside loop: ' . print_r($record, true) , 3);
       $ses_start_date_form     = explode(" ",$record->eventdate);
   	   $ses_start_date_format   = $ses_start_date_form[0].$record->startdatetime;
   	   $ses_end_date_format = $ses_start_date_form[0].$record->enddatetime;
   	   /*For ticket 0028936: ILT Timezone Display for My Learning page */
   	   $ilt_ses_start_date_format   = date_format(date_create($ses_start_date_format),'Y-m-d H:i');
   	   $ilt_ses_end_date_format = date_format(date_create($ses_end_date_format),'Y-m-d H:i');
   	   expDebug::dPrint('MY CALENDAR INFO RESULT inside loop: ' . print_r($ses_start_date_form, true) , 3);
       if($record->delivery_type == 'lrn_cls_dty_vcl' || ($record->delivery_type == 'lrn_cls_dty_ilt' && $record->session_timezone)){   	      
          $vStartDateTime = date_format(date_create($ses_start_date_format),'Y-m-d H:i');
    	  $vEndDateTime   = date_format(date_create($ses_end_date_format),'Y-m-d H:i');        		
    	  $vTimeZone	= $record->session_timezone;
    	  $oStartDateTime  = timeZoneConvert($vStartDateTime , $vTimeZone,  $vUserPHPTimezonename); 
    	  $ses_start_date_format 	= date_format($oStartDateTime,'Y-m-d H:i');
    	  $startdatetime = date_format($oStartDateTime,'g:i');   
    	  $oEndDateTime  = timeZoneConvert($vEndDateTime , $vTimeZone,  $vUserPHPTimezonename); 
    	  $ses_end_date_format 	= date_format($oEndDateTime,'Y-m-d H:i');
    	  $enddatetime = date_format($oEndDateTime,'g:i');      
          $eventdate = date_format(date_create($ses_start_date_format),'Y-m-d')." 00:00:00";
        }else{
          $startdatetime = date('g:i', strtotime($record->startdatetime));                    	   
          $eventdate = $record->eventdate;
       }    	  
       
       $insLnrClsDetails = ($isInstructor) ? getInstructorAndLearnerClasses($userId) : '';
       $eventEndDate = null;
       if ($mode == 'day' && $record->statustext == 'Not Registered') { 
       		$eventDetails = getLastSessionInfo($record->classid);
       		expDebug::dPrint('$eventDetails' . print_r($eventDetails, 1));
       		
       		$last_ses_end_date_form     = explode(" ",$eventDetails[0]->start_date);
       		$last_ses_end_date_format 	= $last_ses_end_date_form[0].' '.$eventDetails[0]->end_time;
       		$vLastEndDateTime 			= date_format(date_create($last_ses_end_date_format),'Y-m-d H:i');
       		$oLastEndDateTime  			= timeZoneConvert($vLastEndDateTime , $vTimeZone,  $vUserPHPTimezonename);
       		$last_ses_end_date_format 	= date_format($oLastEndDateTime,'Y-m-d H:i');
       		$Lastenddate 				= date_format(date_create($last_ses_end_date_format),'Y-m-d')." 00:00:00";
       		$Lastenddatetime 			= date_format($oLastEndDateTime,'g:i');
       		
       		$eventEndDate = array(
       			'dateWithoutTime'		=> dateTimeStrToUnivShort($Lastenddate, 'Y', false, false, false),
       			'endDateTime' 			=> $Lastenddatetime  .' ' .date_format(date_create($last_ses_end_date_format),'A'),	
       			'iltdateWithoutTime'	=> dateTimeStrToUnivShort($vLastEndDateTime, 'Y', false, false, false),
       			'iltEndDateTime' 		=> date_format(date_create($vLastEndDateTime),'g:i'),
       			
       		);
       }
       
       if ($mode == 'day') {
       		$set_result[$i] = array(
       			'classId'             			=> $record->classid, 																// classid
       			'eventTitle'               		=> $record->title,																	// title
       			'eventCode'           			=> $record->code,																	// code
       			'deliveryType'					=> $record->delivery_type,															// deliverytype
       			'iconTypeClass' 				=> getTypeImageClass($record->delivery_type),	 									// calendar_class_type_icon
       			'dateWithoutTime' 				=> dateTimeStrToUnivShort($eventdate, 'Y', false, false, false),					// date_without_time
       			'iltdateWithoutTime'			=> dateTimeStrToUnivShort($record->eventdate, 'Y', false, false, false),			// ilt_date_without_time
       			'sessiontimezone'        		=> $record->session_timezone,														// session_timezone
       			'usertimezone'           		=> $vUserPHPTimezonename,															// user_timezone
       			'tz_code'           			=> $record->tz_code,																// user_tzcode
       			'user_tzcode'           		=> $timezone_details['attr4'],														// user_tzcode
       			'sessiontimezonecode' 			=> $record->session_timezone_code,													// session_timezone_code
       			'usertimezonecode'         		=> $timezone_details['attr1'],														// usertimezonecode
       			'startDate'           			=> $eventdate,																		// eventdate
       			'iltstartDate'           		=> date_format(date_create($ilt_ses_start_date_format),'Y-m-d')." 00:00:00",		// ilt_eventdate
       			'eventDesc'               		=> $record->cdesc,																	// cdesc
       			'eventDate'           			=> $eventdate,																		// eventdate
       			'ilteventDate'         			=> date_format(date_create($ilt_ses_start_date_format),'Y-m-d')." 00:00:00",		// ilt_eventdate
       			'startDateTime'      			=> $startdatetime .' ' .date_format(date_create($ses_start_date_format),'A'),		// startdatetime
       			'endDateTime'		  			=> $enddatetime.' ' .date_format(date_create($ses_end_date_format),'A'),																	// enddatetime
       			'iltstartDateTime'       		=> date_format(date_create($ilt_ses_start_date_format),'g:i'),						// ilt_startdatetime
       			'iltendDateTime'		  		=> date_format(date_create($ilt_ses_end_date_format),'g:i'),						// ilt_enddatetime
       			'code'           				=> $record->code,																	// code
       			'regstatus'          			=> $record->reg_status,																// reg_status
       			'statustext'          			=> $record->statustext,																// statustext
       			'myclass'			  			=> $record->myclass,																// myclass
       			'insLnrClsDetails' 				=> $insLnrClsDetails,																// inslnrclsdetails
       			 
       			'session_start_time_form'   	=> date_format(date_create($ses_start_date_format),'A'),
       			'session_end_time_form'    		=> date_format(date_create($ses_end_date_format),'A'),
       			/*For ticket 0028936: ILT Timezone Display for My Learning page */
       			'ilt_session_start_time_form'  	=> date_format(date_create($ilt_ses_start_date_format),'A'),
       			'ilt_session_end_time_form'    	=> date_format(date_create($ilt_ses_end_date_format),'A'),
       			'comp_status'         			=> $record->comp_status,
       			'fulldate'            			=> $record->fulldate,
       			//'sepm'                		=> $record->sepm,
       			//'mro_flag'			  		=> $record->mro_flag,
       			'sessionCount'					=> getSessionCount($record->classid),
       			'eventEnd'						=> $eventEndDate,
       		);
       } else {
       	$set_result[$i] = array(
       			'classId'             			=> $record->classid, 																// classid
       			'eventTitle'					=> $record->title,
       			'eventDate'           			=> $eventdate,																		// eventdate
       			'statustext'          			=> $record->statustext,																// statustext
       			'myclass'			  			=> $record->myclass,																// myclass
       			'insLnrClsDetails' 				=> $insLnrClsDetails,																// inslnrclsdetails
       	);
       }
   	   
        $i++;
        expDebug::dPrint('session time format'.print_r($set_result,true),4);
    }
    return $set_result;
  }catch (Exception $ex) {
    watchdog_exception('getUpcomingTrainings', $ex);
    expertusErrorThrow($ex);
  } 
    
  }

function getInstructorAndLearnerClasses($userId){
  try{
  $select = db_select('slt_enrollment', 'enr');
  $select->addField('enr','class_id', 'class_id');
  $select->condition('enr.comp_status',array('lrn_crs_cmp_enr','lrn_crs_cmp_inp'),'IN');
  $select->condition('enr.user_id', $userId, '=');
  expDebug::dPrintDBAPI('getInstructorAndLearnerClasses QRY ONE ', $select); 
  $classid = $select->execute()->fetchAll();
  expDebug::dPrint('getInstructorAndLearnerClasses $classid = ' . print_r($classid, true), 4);
  
  foreach($classid as $cls_id){
    $clsId[] = $cls_id->class_id;
  }
  $clsId = array_filter($clsId); /*Viswanathan added for #63790*/
  expDebug::dPrint('getInstructorAndLearnerClasses $clsId = ' . print_r($clsId, true), 4);
  
  //$clsIdVal = implode(',',$clsId);
  expDebug::dPrint('getInstructorAndLearnerClasses $clsIdVal = ' . print_r($clsIdVal, true), 4);
  //if(!empty($clsIdVal)){
  $select1 = db_select('slt_course_class_session', 'sccs');
  $select1->leftjoin('slt_session_instructor_details', 'ses_ins', 'ses_ins.session_id=sccs.id');
  $select1->addField('sccs','end_time', 'end_time');
  //$select1->addField('sccs','instructor_id', 'instructor_id');
  $select1->addExpression('GROUP_CONCAT(DISTINCT(ses_ins.instructor_id))', 'instructor_id');
  $select1->condition('sccs.start_date', Date("Y-m-d"), '>=');
  if(!empty($clsId)){
  	$select1->condition(db_or()->condition('ses_ins.instructor_id ',$userId, '=')->condition('sccs.class_id', $clsId, 'IN'));
  } else {
  	$select1->where('ses_ins.instructor_id = '.$userId );
  }
  $select1->orderBy('sccs.end_time','ASC');
  expDebug::dPrintDBAPI('getInstructorAndLearnerClasses QRY TWO ', $select1); 
  $classid1 = $select1->execute()->fetchAll();
  expDebug::dPrint('$getInstructorAndLearnerClasses $classid1 = ' . print_r($classid1, true), 4);
  
  foreach($classid1 as $insClsId){
    $end_time[]= $insClsId->end_time;
    if($insClsId->instructor_id!='' && $insClsId->instructor_id !=null){
    	$ins_id = explode(',',$insClsId->instructor_id);
    	foreach ($ins_id as $instrucId){
    		$instructor_id[]= $instrucId;
    	}
    }
    
  }
  expDebug::dPrint('getInstructorAndLearnerClasses $end_time  = ' . print_r($end_time, true), 4);
  expDebug::dPrint('getInstructorAndLearnerClasses $instructor_id  = ' . print_r($instructor_id, true), 4);

  foreach($instructor_id as $ins_id){
    $instructorId[] = empty($ins_id[i]) ? 'REG' : 'INS';
  }
    
  expDebug::dPrint('getInstructorAndLearnerClasses $instructorId = ' . print_r($instructorId, true), 4);
  
  $endTime = implode(',',$end_time);
  $instrucId = implode(',',$instructorId);
  
  $isInstructor = is_instructor($userId);
  expDebug::dPrint('$getInstructorAndLearnerClasses $isInstructor  = ' . print_r($isInstructor, true), 4);
  
  $val = trim($endTime.'#'.$instrucId.'#'.$isInstructor);
  expDebug::dPrint('$getInstructorAndLearnerClasses $val  = ' . print_r($val, true), 4);
  
  //}
  if(!empty($endTime)){
    return $val;
  }
    }catch (Exception $ex) {
    watchdog_exception('getInstructorAndLearnerClasses', $ex);
    expertusErrorThrow($ex);
  } 
  
}

function getLastSessionInfo($classId = '') {
	$select = db_query('select start_date, end_time from slt_course_class_session where class_id=:class_id order by start_date desc, start_time desc limit 1', array(':class_id' => $classId));
	$result = $select->fetchAll();
	expDebug::dPrintDBAPI('getLastSessionInfo', $select, array(':class_id', $classId));
	return $result;
}

function getSessionCount($classId = '') {
	$select = db_query("select COUNT(1) from slt_course_class_session where class_id=:class_id", array(':class_id' => $classId));
	$result = $select->fetchField();
	return $result;
}
?>