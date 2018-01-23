<?php
	
function getLaunchDetail($enrollId, $callFromAPI = 0,$sid = ''){
	try{
		$usertimezone = getUserPHPTimezoneName();
		$usertimezone = $usertimezone[0]->zone_name;
		$timeZone = date_default_timezone(false);
		$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
		$scheculeRes = getScheduleClassDetails($enrollId);
		$usertimezone = empty($usertimezone)? $defaultTimezone : $usertimezone;
		$convertTimezone = timeZoneConvert($scheculeRes["regdate"] , $defaultTimezone,  $usertimezone);
		$date = $convertTimezone->format('Y-m-d H:i:s');
		$errorMsg    = '';
		$errflag     = false;

		/*if($scheculeRes["regstatus"] == 'lrn_crs_reg_cnf') {
		 if(dateCheck($scheculeRes["ValidTo"]) == false){
		 $errorMsg = "Content validity period is expired";
		 }
		 } else {
		 $errorMsg = "Registration is not yet confirmed";
		 }*/
		//expDebug::dPrint('$Reg date = --' . $scheculeRes["regdate"]."Validty date-".$validityDays."Date-".$validityDays.$scheculeRes["deliverycode"], 4);
		$resultScore = getScoreForMultiContent($enrollId);
		$result   = doGetLaunchDetails($scheculeRes["basetype"],$scheculeRes["deliverycode"],$enrollId);
		//$result=$this->doGetLaunchDetails($baserst[0]->BaseType,$baserst[0]->DeliveryCode,$reqData);
		$launch  = array();
		if($errflag == false) {

			if(count($result)>0) {
				for($i=0;$i<count($result);$i++){
					//$scheculeRes["RegDate"];
					if(!empty($result[$i]["validitydays"])){
						$validityDays = $result[$i]["validitydays"];
						if($validityDays > 1){
							$expiryDate = strtotime(date("Y-m-d H:i:s", strtotime($date)) . "+".$validityDays."  days");
						}else {
							$expiryDate = strtotime(date("Y-m-d H:i:s", strtotime($date)) . "+".$validityDays."  day");
						}
						$expiryDate = date("Y-m-d H:i:s", $expiryDate);
						expDebug::dPrint('$Reg date = --' . $scheculeRes["regdate"]."Validty date-".$validityDays."Date-".$expiryDate, 4);
					}else {
						$expiryDate =null;
					}
					$dateCheck   = remainDaysCheck($expiryDate);
					$launch[$i]  = getLaunchInfo($dateCheck,$result[$i],$callFromAPI,$resultScore);
					$sidval = $sid."-".$launch[$i]["ID"]."-".$launch[$i]["VersionId"]."-".$enrollId."-".$launch[$i]["LessonLocation"];
					expDebug::dPrint('aicc id launch detail - '.$sidval, 5);
					$enc1 = new Encrypt();
					$sidval = $enc1->encrypt($sidval);
					$launch[$i]['AICC_SID']=$sidval;
				}
			}
			/*else
			 {
			 if(empty($result))
			 {
			 $dateCheck       = remainDaysCheck($result[0]["validto"]);
			 $launch[0]       = getLaunchInfo($dateCheck,$result[0]);
			 }
			 } */
		}
		return $launch;
	}catch (Exception $ex) {
		watchdog_exception('getLaunchDetail', $ex);
		expertusErrorThrow($ex);
	}
}

function remainDaysCheck($dat) {
	try{
		$valid    = true;
		$retArr   = array();

		if($dat != null) {
		//	$todays_date     = date("Y-m-d");
			$usertimezone = getUserPHPTimezoneName();
			$usertimezone = $usertimezone[0]->zone_name;
			$timeZone = date_default_timezone(false);
			$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
			$usertimezone = empty($usertimezone)? $defaultTimezone : $usertimezone;
			$convertTimezoneuser = timeZoneConvert(now() , $defaultTimezone,  $usertimezone);
			//$todays_date     = $convertTimezoneuser->date;
			$todays_date     = $convertTimezoneuser->format('Y-m-d H:i:s');
			$today           = strtotime($todays_date);
			$expiration_date = strtotime($dat);
          
			if($expiration_date > $today) {
				$timeleft           = $expiration_date - $today;
				$daysleft           = round((($timeleft/24)/60)/60);
				$retArr['valid']    = true;
				$retArr['validMsg'] = t('Days Remaining')." :".$daysleft;
				$retArr['daysLeft'] = $daysleft;
			} else {
				$retArr['valid']    = false;
				$retArr['validMsg'] = 'Expired';
			}
		} else {
			$retArr['valid']    = true;
		}
		return $retArr;
	}catch (Exception $ex) {
		watchdog_exception('remainDaysCheck', $ex);
		expertusErrorThrow($ex);
	}
}


function getLaunchInfo($dateValidate,$result,$callFromAPI = 0,$resultScore = array()){
	try{
		expDebug::dPrint(' $result = ' . print_r($result, true) , 4);
		// Added by vincent on Nov 06,2013 for #0029109
		$vUserPHPTimezonename = date_default_timezone(FALSE);
		$sessionTZ = new DateTimeZone($vUserPHPTimezonename);
		$currentDate = new DateTime(now(),$sessionTZ) ;
		$usertimezone = getUserPHPTimezoneName();
		$usertimezone = $usertimezone[0]->zone_name;
		$timeZone = date_default_timezone(false);
		$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
		$usertimezone = empty($usertimezone)? $defaultTimezone : $usertimezone;
		$convertTimezone = timeZoneConvert($result["reg_date"] , $defaultTimezone,  $usertimezone);
		$convertTimezoneuser = timeZoneConvert(now() , $defaultTimezone,  $usertimezone);
		
		$launchArr                       = array();
		$launchArr["ID"]                 = $result["id"];
		$launchArr["ContentId"]          = $result["contentid"];
		$launchArr["VersionId"]          = $result["versionid"];
		$launchArr["VersionNum"]          = $result["versionno"];
		$launchArr["ValidTo"]            = $result["validto"];
		$launchArr["Code"]               = $result["code"];
		$launchArr["Title"]              = rawurlencode($result["title"]);
		$launchArr["server_date_time"]   = $currentDate->format('M d, Y H:i');
			
		/*if(in_array("status",$result))
		 $launchArr["Status"]             = $result["status"];
		 else
		 $launchArr["Status"]             = "";*/
		$launchArr["Status"]             = empty($result["status"]) ? "" : $result["status"];
		$launchArr["Lessoncnt"]          = empty($result["lessoncnt"]) ? "" : $result["lessoncnt"];
		$launchArr["LesScore"]           = empty($result["lesscore"]) ? "" : $result["lesscore"];
		$launchArr["ClsScore"]           = empty($result["clsscore"]) ? "" : $result["clsscore"];
		$launchArr["ContScore"]          = empty($resultScore[$result["contentid"]]) ? "" : $resultScore[$result["contentid"]];
		$launchArr["ValidityDays"]       = empty($result["validitydays"]) ? "" : $result["validitydays"];
		$launchArr["regDate"]            = empty($result["regdate"]) ? "" : $result["regdate"];
		$launchArr["regDate"]            = empty($result["regdate"]) ? "" : $result["regdate"];
		$launchArr["contentQuizStatus"]  = empty($result["contentquizstatus"]) ? "" : $result["contentquizstatus"];
		$launchArr["ContentCompletionStatus"]  = empty($result["contentcompletionstatus"]) ? "" : $result["contentcompletionstatus"];
		if($launchArr["regDate"] != ''){
			$cur_date = $convertTimezoneuser->format('Y-m-d H:i:s');			
			$remDays = abs((strtotime($convertTimezone->format('Y-m-d H:i:s')) - strtotime($cur_date)) / (60 * 60 * 24));
		}
		$launchArr["remDays"]            = round($remDays);
		$launchArr["LaunchType"]         = $result["basetype"];//basetype launchtype
		$launchArr["ContentType"]        = $result["contenttype"];
			
		if (array_key_exists("contentsubtypecode", $result)) {
			$launchArr["ContentSubTypeCode"]        = $result["contentsubtypecode"];
		} else {
			$launchArr["ContentSubTypeCode"]       = "";
		}
			
		//This accept http:// and https://. Ticket #0025197
		if(($result["contenttypecode"] == 'lrn_cnt_typ_knc' || $result["contenttypecode"] == 'lrn_cnt_typ_vod') && $launchArr["ContentSubTypeCode"] == '2'){
			if (!preg_match("~^(?:ht)tps?://~i", $result["launchurl"])) {
				$launchArr["LearnerLaunchURL"] = "http://" . $result["launchurl"];
			}else{
				$launchArr["LearnerLaunchURL"] = $result["launchurl"];
			}
		}else{
			$launchArr["LearnerLaunchURL"] = $result["launchurl"];
		}
			
		if($callFromAPI){
			$launchArr["ContentUploadedData"] = array(getUploadFileInfo($launchArr["LearnerLaunchURL"],$launchArr["ContentType"],$result["basetype"] ));
		}
		//$launchArr["LearnerLaunchURL"]   = $result["learnerlaunchurl"];
			
		if(in_array("presenterlaunchurl",$result))
		$launchArr["PresenterLaunchURL"]             = $result["presenterlaunchurl"];
		else
		$launchArr["PresenterLaunchURL"]             = "";

		//$launchArr["PresenterLaunchURL"] = $result["presenterlaunchurl"];

		$launchArr["MaxAttempt"]         = $result["maxattempt"];
		if(empty($result["maxattempt"])){
			$al = 'notset'; //If not set the Maximum Attempts in class creation
		}
		else {
			if (array_key_exists("contentattempts", $result)) {
				$attempts = $result["contentattempts"];
			}
			else {
				$attempts = $result["attempts"];
			}

			expDebug::dPrint('$attempts = ' . $attempts, 4);
			expDebug::dPrint('$result[maxattempt] = ' . $result["maxattempt"], 4);

			if ($attempts < $result["maxattempt"]) {
				$al = $result["maxattempt"] - $attempts;
				$al = (string)$al;
			}
			else {
				$al = '0';
			}
		}
		expDebug::dPrint('$al = ' . $al, 4);
		$launchArr["AttemptLeft"]         = $al;

		$contValidateMsg                  = !empty($dateValidate['validMsg']) ? $dateValidate['validMsg'] : '';
		//$launchArr["contValidateMsg"]     = !empty($dateValidate['validMsg']) ? $dateValidate['validMsg'] : '';
		if ($dateValidate['valid'] && ($al == '0')) {
			
			$launchArr["contValidateMsg"] = " ".t("No attempts left");
		}else if($dateValidate['valid'] && ($al > 1)){
			$launchArr["contValidateMsg"] = $contValidateMsg." ".$al." ".t("Attempts left");
		}else if($dateValidate['valid'] && ($al == 1)){
			$launchArr["contValidateMsg"] = $contValidateMsg." ".$al." ".t("Attempt left");
		}else{
			$launchArr["contValidateMsg"] = $contValidateMsg;
		}
		$launchArr['daysLeft'] = $dateValidate['daysLeft'];
		if(($al > 0 || $al =='notset') && ($dateValidate['valid'])) {
			$launchArr["IsLaunchable"]         = true;
		}
		else {
			$launchArr["IsLaunchable"]         = false;
			//$launchArr["NonLaunchableMessage"] = $dateValidate['validMsg'];
			if(array_key_exists("lastattempt", $result))
			$launchArr["LastAttempt"]          = $result['lastattempt'];
			else
			$launchArr["LastAttempt"]          = "";
		}
			
		$launchArr["Handler"]             = "Scorm1.2";
			
		if(array_key_exists("lessonlocation",$result))
		$launchArr["LessonLocation"]          = $result['lessonlocation'];
		else
		$launchArr["LessonLocation"]          = '';

		if(array_key_exists("launchdata",$result))
		$launchArr["LaunchData"]          = $result['launchdata'];
		else
		$launchArr["LaunchData"]          = '';

		if(array_key_exists("suspenddata",$result))
		$launchArr["SuspendData"]          = $result['suspenddata'];
		else
		$launchArr["SuspendData"]          = '';

		if(array_key_exists("progress",$result))
			$launchArr["progress"]          = $result['progress'];
		else
			$launchArr["progress"]          = '';

		if(array_key_exists("cmiexit",$result))
		$launchArr["CmiExit"]          = $result['cmiexit'];
		else
		$launchArr["CmiExit"]          = '';
			
		if (array_key_exists('contentstatus', $result)) {
			$launchArr['ContentStatus'] = $result['contentstatus'];
		}
		else {
			$launchArr['ContentStatus'] = '';
		}
		if (array_key_exists('masteryscore', $result)) {
			$launchArr['masteryscore'] = $result['masteryscore'];
		}
		else {
			$launchArr['masteryscore'] = 0;
		}
		if($callFromAPI == 0) {
			$launchArr['StatusCode'] = $result['statuscode'];
		}
		$launchArr["subtype"]        = $result["subtype"];
		expDebug::dPrint(' $launchArr = ' . print_r($launchArr, true) , 4);
		return $launchArr;
	}catch (Exception $ex) {
		watchdog_exception('getLaunchInfo', $ex);
		expertusErrorThrow($ex);
	}
}


function getScheduleClassDetails($enrId) {
	try{
		$scheduledClassQry ='SELECT
		cls.id AS Id,
		cls.course_id AS CourseId,
		cls.title AS Title,
		cls.code AS CODE,
		(SELECT lst.name FROM slt_profile_list_items lst WHERE lst.code=cls.delivery_type) AS DeliveryType,
		(SELECT lst.attr1 FROM slt_profile_list_items lst WHERE lst.code=cls.delivery_type) AS BaseType,
		cls.delivery_type AS DeliveryCode,
		enr.reg_status AS RegStatus,
		enr.comp_status AS CompStatus,
		enr.reg_date AS RegDate,
		enr.valid_to AS ValidTo
	FROM
		slt_enrollment enr,
		slt_course_class cls
	WHERE 
		enr.class_id=cls.id
		AND enr.id=:enrId';


		$classDetailsQry  = db_query($scheduledClassQry, array(':enrId' => $enrId));
		$classDetailsRes  = $classDetailsQry->fetchAssoc();

		return $classDetailsRes;
	}catch (Exception $ex) {
		watchdog_exception('getScheduleClassDetails', $ex);
		expertusErrorThrow($ex);
	}
}

function doGetLaunchDetails($btype,$type,$enrollId) {
	try{
		if($btype == "WBT") {
			$result = getWBTLaunchDetail($enrollId,$type);
		} else if($btype == "VC") {
			$result = getVCLaunchDetail($enrollId);
		} else if ($btype == "VOD") {
			$result = getVODLaunchDetail($enrollId);
		}

		return $result;
	}catch (Exception $ex) {
		watchdog_exception('doGetLaunchDetails', $ex);
		expertusErrorThrow($ex);
	}
}


function dateCheck($dat) {
	try{
		$valid               = true;
		if($dat != null) {
			$todays_date     = date("Y-m-d");
			$today           = strtotime($todays_date);
			$expiration_date = strtotime($dat);

			if($expiration_date > $today){
				$timeleft = $expiration_date - $today;
				$daysleft = round((($timeleft/24)/60)/60);
				$valid = $daysleft;//true;
			}else{
				$valid = false;
			}

		}
		return $valid;
	}catch (Exception $ex) {
		watchdog_exception('dateCheck', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 $future = strtotime('4 July 2010');
 $now = time();
 $timeleft = $future-$now;
 $daysleft = round((($timeleft/24)/60)/60);
 */
function getWBTLaunchDetail($enrId,$type) {
	try{
		//44353: When an admin user deletes the content the physical file should be deleted from the content folder
		if($type == 'lrn_cls_dty_vlb'){
			$vlabLaunchQry = 'SELECT
		cls.id AS Id,
		cls.title AS Title,		
		(SELECT prof.name FROM slt_profile_list_items prof WHERE prof.code = enr.comp_status) AS STATUS, 
		(SELECT lst.attr1 FROM slt_profile_list_items lst WHERE lst.code=cls.delivery_type_code) AS BaseType,
		(SELECT lst1.name FROM slt_profile_list_items lst1 WHERE lst1.code=cls.delivery_type_code) AS DeliveryType,
		cls.delivery_type_code AS DeliveryCode,
	 	attn.total_attempts AS Attempts,
		attn.last_attempt_date AS LastAttempt,
		attn.last_lesson_location AS LessonLocation,
		enr.valid_to AS ValidTo
	FROM
		slt_enrollment enr,
		slt_course_class cls
	 	LEFT JOIN slt_attendance_summary attn ON attn.enrollment_id=xregid
	WHERE
		cls.id=enr.classid
		AND enr.id=:enrId';	

		}else{
			//$vSQL = "CALL slp_wbt_launch_detail_sel($reqData->RegistrationID,null)";
			$vlabLaunchQry = 'SELECT
		conles.id AS Id,
		conmast.id AS ContentId,
		emap.version_id as VersionId,
		conmast.code AS Code,
		conles.title AS Title,
		conles.masteryscore AS masteryscore,
		(select count(1) from slt_content_lesson les  where les.content_version_id in( select vers.id from slt_content_version vers where vers.content_master_id =conmast.id and vers.id=emap.version_id)) AS Lessoncnt,
		enr.score AS ClsScore,
		attn.score AS LesScore,
		attn.status AS StatusCode,
		attn.content_status AS ContentQuizStatus,
		attn.completion_status AS ContentCompletionStatus,
		IF(attn.status IS NULL, attn.status,(SELECT prof.name FROM slt_profile_list_items prof WHERE prof.code = attn.status)) AS Status,
		';
			//conles.launchurl AS LaunchURL,
			$vlabLaunchQry .='conles.launchurl AS LaunchURL,
		(SELECT lst.attr1 FROM slt_profile_list_items lst WHERE lst.code=cls.delivery_type) AS BaseType,
		(SELECT lst1.name FROM slt_profile_list_items lst1 WHERE lst1.code=cls.delivery_type) AS DeliveryType,
		cls.delivery_type AS DeliveryCode,
		(SELECT lst2.name FROM slt_profile_list_items lst2 WHERE lst2.code=conver.content_sub_type) AS ContentType,
		conmap.max_attempts AS MaxAttempt,
		conmap.validity_days AS ValidityDays,
	 	attn.total_attempts AS Attempts,
		attn.last_attempt_date AS LastAttempt,
		attn.last_lesson_location AS LessonLocation,
		attn.launch_data AS LaunchData,
		attn.suspend_data AS SuspendData,
		attn.progress AS progress,
		attn.cmiexit AS CmiExit,
		per.first_name AS FirstName,
		per.last_name AS LastName,
		per.email AS Email,
		conmast.type AS ContentTypeCode,
		conver.hosted_type AS ContentSubTypeCode,
		enr.valid_to AS ValidTo,
		DATE_FORMAT(enr.reg_date, \'%Y-%m-%d\') AS regDate,
		enr.reg_date as reg_date,
		conver.version AS versionno,
		conmast.subtype AS subtype
	FROM
		slt_enrollment enr,
		slt_course_class cls,
		slt_course_content_mapper conmap,
		slt_content_master conmast,
		slt_content_version conver,
		slt_person per,
		slt_enrollment_content_mapping emap,
		slt_content_lesson conles
	 	LEFT JOIN slt_attendance_summary attn ON attn.enrollment_id=:enrId AND attn.lesson_id=conles.id AND attn.content_version_id = conles.content_version_id
	WHERE
		enr.class_id=conmap.class_id
		AND cls.id=enr.class_id
		AND conmap.content_id=conmast.id
		AND conmap.content_id=conver.content_master_id
		AND conver.id=conles.content_version_id
		and emap.enroll_id = enr.id
		and emap.version_id = conver.id
		AND enr.id=:enrId
		AND per.id=enr.user_id
		AND conver.status!=\'lrn_cnt_sts_del\'
		AND conmast.status!=\'lrn_cnt_sts_del\'
	ORDER BY
		conmap.sequence_number';
		}
		expDebug::dPrint('WBT query checking'.$vlabLaunchQry.'<--enroll-id-->'.$enrId);
		$wbtLaunchDetQry  = db_query($vlabLaunchQry, array(':enrId' => $enrId));
		$wbtLaunchDetRes  = $wbtLaunchDetQry->fetchAll(PDO::FETCH_BOTH);
		expDebug::dPrint('WBT launch query results before= ' . print_r($wbtLaunchDetRes, true), 4);
		$config=getConfig("exp_sp");
		$mylearn_version 					= $config['mylearn_version'];
		$content_conversion		= $config['content_conversion'];
		if($mylearn_version && $content_conversion){
			$new_launch_url = '';
			$old_launch_url = '';
			//$image_ext =  array('gif','png' ,'jpg', 'jpeg','pptx','ppt','pps','ppsx');
			$image_ext =  array('gif','png' ,'jpg', 'jpeg');
			foreach ($wbtLaunchDetRes as $key=>$content_details){
				$old_launch_url = $content_details['launchurl'];
				$copy_content_details = $content_details;
				if($content_details['contenttype']=='Knowledge Content' && strpos($content_details['launchurl'],'/files/contentupload')>0){
					//find expention of upload file
					$url_arr  = explode("/", $content_details['launchurl']);
					$filename = end($url_arr);
					$file_arr = explode(".", $filename);
					$ext = strtolower(end($file_arr));
					if($ext == 'pdf' || in_array($ext, $image_ext))
						$new_launch_url = $content_details['launchurl'];
					else{
						$info = pathinfo($content_details['launchurl']);
						//$new_launch_url = str_replace($ext,'pdf',$content_details['launchurl']);
						$new_launch_url = $info['dirname']. DIRECTORY_SEPARATOR	. $info['filename']	. '.'. 'pdf';
						$new_launch_url = str_replace('contentupload','contentupload/convertioncontent',$new_launch_url);
						$finfo = finfo_open(FILEINFO_MIME_TYPE);
						$mime = finfo_file($finfo, $_SERVER['DOCUMENT_ROOT'].$new_launch_url);
						$new_file_root_url = $config['pdf_temp_dir'].$new_launch_url;
						if($mime != 'application/pdf' && !file_exists($new_file_root_url)){
							$new_launch_url = $content_details['launchurl'];
							expDebug::dPrint('exist check comes here----------->'.print_r(file_exists($new_file_root_url),1),4);
						}
						expDebug::dPrint('FIND PDFT MIME TYPE URL----------->'.print_r($new_launch_url,1),4);
					}
					$wbtLaunchDetRes[$key]['launchurl'] =$new_launch_url;
					unset($copy_content_details['launchurl']);
					$replace_key= array_search($old_launch_url, $copy_content_details);
					$wbtLaunchDetRes[$key][$replace_key] = $new_launch_url;
				}
			}
		}
		expDebug::dPrint('WBT launch query results After= ' . print_r($wbtLaunchDetRes, true), 4);
		
		// Determine status for each content and add it to each lesson
		// Also, simultaneously determine attempts left for each content and add it to each lesson.
		$startLessonIdx = -1;
		$currentContentId = -1;
		$numLessons = count($wbtLaunchDetRes);
		for ($currentIdx = 0; $currentIdx < $numLessons; $currentIdx++) {
			$lesson = $wbtLaunchDetRes[$currentIdx];
			if ($lesson['contentid'] != $currentContentId) {
				if ($currentContentId != -1) {
					expDebug::dPrint('$statusList = ' . print_r($statusList, true), 4);
					//make status decision for the content, and save status in each lesson
					$contentStatus = evaluateContentStatusFromLessonStatuses($statusList);
					expDebug::dPrint('$contentStatus = ' . print_r($contentStatus, true), 4);

					expDebug::dPrint('$contentAttempts = ' . $contentAttempts, 4);

					for ($i = $startLessonIdx; $i < $currentIdx; $i++) {
						$wbtLaunchDetRes[$i]['contentstatus'] = $contentStatus;
						$wbtLaunchDetRes[$i]['contentattempts'] = $contentAttempts;
					}
				}

				// reset status capturing process for the new content id.
				$currentContentId = $lesson['contentid'];
				expDebug::dPrint('$currentContentId = ' . print_r($currentContentId, true), 4);
				$statusList = array($lesson['statuscode']);
				$startLessonIdx = $currentIdx;
				expDebug::dPrint('$startLessonIdx = ' . print_r($startLessonIdx, true), 4);

				$contentAttempts = empty($lesson['attempts'])? 0 : $lesson['attempts'];
			}
			else {
				// add status to the status list
				$statusList[] = $lesson['statuscode'];
					
				// add lesson attempts to content attempts
				$contentAttempts += empty($lesson['attempts'])? 0 : $lesson['attempts'];
			}
			$wbtLaunchDetRes[$currentIdx]['suspenddata'] = rawurlencode($wbtLaunchDetRes[$currentIdx]['suspenddata']);
			expDebug::dPrint('suspenddata decode = ' . $wbtLaunchDetRes[$i]['suspenddata'], 4);
		} // end for loop
		if ($currentContentId != -1) {
			expDebug::dPrint('$statusList = ' . print_r($statusList, true), 4);
			//make status decision for the content, and save status in each lesson
			$contentStatus = evaluateContentStatusFromLessonStatuses($statusList);
			expDebug::dPrint('$contentStatus = ' . print_r($contentStatus, true), 4);

			expDebug::dPrint('$contentAttempts = ' . $contentAttempts, 4);

			for ($i = $startLessonIdx; $i < $currentIdx; $i++) {
				$wbtLaunchDetRes[$i]['contentstatus'] = $contentStatus;
				$wbtLaunchDetRes[$i]['contentattempts'] = $contentAttempts;
			}
		}

		expDebug::dPrint('after adding content statuses $wbtLaunchDetRes = ' . print_r($wbtLaunchDetRes, true), 4);
		return $wbtLaunchDetRes;

	}catch (Exception $ex) {
		watchdog_exception('getWBTLaunchDetail', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * evaluateContentStatusFromLessonStatuses() -
 */
function evaluateContentStatusFromLessonStatuses($statusList) {
	try{
		$emptyStatusFound = false;
		$nonEmptyStatusFound = false;
		$inProgressStatusFound = false;
		$incompleteStatusFound = false;
		foreach ($statusList as $statusCode) {
			if (empty($statusCode) || $statusCode == 'lrn_crs_cmp_enr') {
				$emptyStatusFound = true;
			}
			else {
				$nonEmptyStatusFound = true;
				if ($statusCode == 'lrn_crs_cmp_inp') {
					$inProgressStatusFound = true;
				}
				else if ($statusCode == 'lrn_crs_cmp_inc') {
					$incompleteStatusFound = true;
					break;
				}
			}
		} // end for loop

		if ($nonEmptyStatusFound == false) {
			$contentStatus = '';
		}
		else if ($incompleteStatusFound) {
			$contentStatus = 'lrn_crs_cmp_inc';
		}
		else if ($emptyStatusFound || $inProgressStatusFound) {
			$contentStatus = 'lrn_crs_cmp_inp';
		}
		else {
			$contentStatus = 'lrn_crs_cmp_cmp';
		}

		if (!empty($contentStatus)) {
			$contentStatus = getProfileListItemName($contentStatus);
		}

		return $contentStatus;
	}catch (Exception $ex) {
		watchdog_exception('evaluateContentStatusFromLessonStatuses', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getVODLaunchDetail() - Get launch details for user enrolled vod class of .
 */
function getVODLaunchDetail($enrId) {
	try{
		//44353: When an admin user deletes the content the physical file should be deleted from the content folder
		$vodLaunchQry = 'SELECT
    conles.id AS Id,
    conmast.id AS ContentId,
    emap.version_id VersionId,
    conmast.code AS Code,
    conles.title AS conTitle,
    if(conmast.type IN(\'lrn_cnt_typ_vod\',\'lrn_cnt_typ_knc\'), conmast.code ,conles.title) AS Title,
    enr.score AS ClsScore,
    attn.score AS LesScore,
	IF((conmap.max_attempts = attn.total_attempts AND enr.comp_status = \'lrn_crs_cmp_inc\'), enr.comp_status, attn.status) AS StatusCode,
	  IF(attn.status IS NULL, attn.status,(SELECT prof.name FROM slt_profile_list_items prof WHERE prof.code = IF((conmap.max_attempts = attn.total_attempts AND enr.comp_status = \'lrn_crs_cmp_inc\'), enr.comp_status, attn.status))) AS Status,
    conles.launchurl AS LaunchURL,
    (SELECT lst.attr1 FROM slt_profile_list_items lst WHERE lst.code=cls.delivery_type) AS BaseType,
    (SELECT lst1.name FROM slt_profile_list_items lst1 WHERE lst1.code=cls.delivery_type) AS DeliveryType,
    cls.delivery_type AS DeliveryCode,
    (SELECT lst2.name FROM slt_profile_list_items lst2 WHERE lst2.code=conver.content_sub_type) AS ContentType,
    conmap.max_attempts AS MaxAttempt,
    conmap.validity_days AS ValidityDays,
    attn.total_attempts AS Attempts,
    attn.last_attempt_date AS LastAttempt,
    attn.last_lesson_location AS LessonLocation,
    attn.launch_data AS LaunchData,
    attn.suspend_data AS SuspendData,
	attn.progress AS progress,
    attn.cmiexit AS CmiExit,
    per.first_name AS FirstName,
    per.last_name AS LastName,
    per.email AS Email,
    conmast.type AS ContentTypeCode,
    conver.hosted_type AS ContentSubTypeCode,
    enr.valid_to AS ValidTo,
    DATE_FORMAT(enr.reg_date, \'%Y-%m-%d\') AS regDate,
    enr.reg_date as reg_date,
		conver.version AS versionno,
		conmast.subtype AS subtype
  FROM
    slt_enrollment enr,
    slt_course_class cls,
    slt_course_content_mapper conmap,
    slt_content_master conmast,
    slt_content_version conver,
    slt_person per,
    slt_enrollment_content_mapping emap,
    slt_content_lesson conles
    LEFT JOIN slt_attendance_summary attn ON attn.enrollment_id=:enrId AND attn.lesson_id=conles.id AND attn.content_version_id = conles.content_version_id
  WHERE
    enr.class_id=conmap.class_id
    AND cls.id=enr.class_id
    AND conmap.content_id=conmast.id
    AND conmap.content_id=conver.content_master_id
    AND conver.id=conles.content_version_id
    and emap.enroll_id = enr.id	
	  and emap.version_id = conver.id
    AND enr.id=:enrId
    AND per.id=enr.user_id
    AND conver.status!=\'lrn_cnt_sts_del\'
		AND conmast.status!=\'lrn_cnt_sts_del\'
  ORDER BY
    conmap.sequence_number';
		expDebug::dPrint('query checking'.$vodLaunchQry.'<--enroll-id-->'.$enrId);

		$vodLaunchDetQry  = db_query($vodLaunchQry, array(':enrId' => $enrId));

		$vodLaunchDetRes  = $vodLaunchDetQry->fetchAll(PDO::FETCH_BOTH);

		expDebug::dPrint('$vodLaunchDetRes = ' . print_r($vodLaunchDetRes, true), 4);
		return $vodLaunchDetRes;
	}catch (Exception $ex) {
		watchdog_exception('getVODLaunchDetail', $ex);
		expertusErrorThrow($ex);
	}
}

function getVCLaunchDetail($enrId) {
	try{
   	$afterCompleteAllowTime = getConfigValue('ALLOW_MEETING_LAUNCH_COMPLETE'); 
		$addSessionTime = ($afterCompleteAllowTime) ? $afterCompleteAllowTime : 30;
		$timeZone = date_default_timezone(false);
		$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
		$vlcLaunchQry = 'SELECT
		ses.id AS Id,
		cls.title AS Title,
		slf_get_profile_list_name(enr.comp_status,NULL) AS STATUS,
		ses.attendee_url AS launchurl,
		ses.presenter_url AS presenterlaunchurl,
		(SELECT attr1 FROM slt_profile_list_items lst WHERE lst.code=cls.delivery_type) AS BaseType,
		(SELECT `name` FROM slt_profile_list_items lst WHERE lst.code=cls.delivery_type) AS DeliveryType,
		cls.delivery_type AS DeliveryCode,
		ses.start_date AS SessionStarts,
		ses.end_date AS SessionEnds,
		ses.timezone AS SessionTimeZone,
	 	attn.total_attempts AS Attempts,
		attn.last_attempt_date AS LastAttempt,
		attn.last_lesson_location AS LessonLocation
	FROM
		 slt_enrollment enr
       LEFT JOIN slt_course_class cls ON cls.id=enr.class_id
       LEFT JOIN slt_course_class_session ses on ses.class_id=enr.class_id AND ses.course_id=enr.course_id
       AND ses.id = if((select count(id) from slt_course_class_session where class_id=cls.id and (CONCAT(DATE(start_date),\' \',CONCAT(end_time,\':00\'))) > date_sub(IFNULL(CONVERT_TZ(now(), \''.$defaultTimezone.'\' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone)),NOW()), INTERVAL \''.$addSessionTime.'\' MINUTE))=0,
                     (select id from slt_course_class_session where class_id=cls.id order by start_date desc, start_time desc limit 1),
                     (select id from slt_course_class_session where class_id=cls.id and (CONCAT(DATE(start_date),\' \',CONCAT(end_time,\':00\'))) > date_sub(IFNULL(CONVERT_TZ(now(), \''.$defaultTimezone.'\' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone)),NOW()), INTERVAL \''.$addSessionTime.'\' MINUTE) order by start_date, start_time limit 1)) 
       LEFT JOIN slt_attendance_summary attn ON attn.enrollment_id=:enrId
    WHERE
        enr.id=:enrId';
			
		$vlcLaunchDetQry  = db_query($vlcLaunchQry, array(':enrId' => $enrId));
		expDebug::dPrint('$$vlcLaunchQry enroll id = ' . $enrId);
		expDebug::dPrintDBAPI('defaultquery',$vlcLaunchDetQry );
		$vlcLaunchDetRes  = array($vlcLaunchDetQry->fetchAssoc());

		return $vlcLaunchDetRes;
	}catch (Exception $ex) {
		watchdog_exception('getVCLaunchDetail', $ex);
		expertusErrorThrow($ex);
	}
}


function resetH5PLaunchUserData($h5pid,$enrid,$progressData)
{
	$progressData["answers"]=array(); //will update answers to empty for next launch
	$updateStmt = db_update('h5p_content_user_data');
	$fields = array('data'=>'');//'json_content'=>$json_content,
	$updateStmt -> fields($fields);
	$updateStmt -> condition('content_main_id', $h5pid);
	$updateStmt -> condition('enrollId', $enrid);
	expDebug::dPrintDBAPI(' resetH5PLaunchUserData id='.$h5pid.'  $updateStmt:', $updateStmt);
	$numUpdated = $updateStmt -> execute();
		
	
}
function getH5PProgress($enrollId,$h5p_cnt_id)
{
	try {
		global $user;

		
		$select = db_select('h5p_content_user_data', 'mast');
		$select->leftJoin('h5p_points', 'points', 'mast.content_main_id=points.content_id and points.enrollId='.$enrollId);
		$select->innerJoin('h5p_nodes', 'h5p', 'h5p.content_id='.$h5p_cnt_id.' and mast.content_main_id=h5p.nid');
		
		$select -> addField('mast', 'data', 'data');
		$select -> addField('points', 'points', 'points');
		$select -> addField('points', 'max_points', 'max_points');
		$select -> addField('h5p', 'json_content', 'json_content');
		$select -> addField('mast', 'content_main_id', 'content_main_id');
		$select -> condition('mast.enrollId',$enrollId , '='); 
		//$select -> condition('mast.content_main_id',$h5p_cnt_id , '=');
		expDebug::dPrintDBAPI('$select h5p progress=', $select);
		$result = $select -> execute() -> fetchAll();
		expDebug::dPrint('getH5PProgress->' . print_r($result, true), 4);
		return $result;
	} catch (Exception $ex) {
		watchdog_exception('getContentDetailsInfoView', $ex);
		expertusErrorThrow($ex);
	}
}
function updateLaunch($op){
	try{
		 expDebug::dPrint('suresh updateLaunch Session type = '.$op."====".$_REQUEST["type"], 5);
		 expDebug::dPrint('suresh updateLaunch Session CONTENT type = '.$op."====".$_REQUEST["contenttype"], 5);
		 
		
		 
		 if($_REQUEST["type"]== "h5p" && $_REQUEST["contenttype"] == "Knowledge Content")
		 {
		 	expDebug::dPrint('suresh updateLaunch Session max attempttttt = '.$_REQUEST["AttemptLeft"]."====".$_REQUEST["MaxAttempt"], 5);
		 	
		 	$h5p_cnt_id = $_REQUEST['h5p_cnt_id'];
		 	expDebug::dPrint('suresh h5p_cnt_id  = '.$_REQUEST['h5p_cnt_id'],4);
		 	$data = getH5PProgress($_REQUEST["regid"],$h5p_cnt_id);
			if(isset($data))
			{
			 	$progressData 	= json_decode($data[0]->data,true);
				$json_content 	= $data[0]->json_content;
				$jsonData		= json_decode($json_content,true);
				$passingscore   = $jsonData['presentation']['passingscore'];
				$numberOfSlides = count($jsonData['presentation']['slides']);
				$noOfSlidesSeen = $progressData["progress"];
               //As this fits to logic
				$noOfSlidesSeen = $noOfSlidesSeen +1;
				expDebug::dPrint("numberOfSlides:".$numberOfSlides . "noOfSlidesSeen:".$noOfSlidesSeen ,4);
				expDebug::dPrint("the entire answer data obtained.presentation.:".print_r($progressData,true),4);
				expDebug::dPrint("the entire jsondata obtainedddd.presentation.:".print_r($jsonData,true),4);
				expDebug::dPrint("passingScoreeee:".$passingscore);
				
				
				$questions = array();
				$questions1 = array();
				$questions2 = array();
				$questions3 = array();
				$summary = array();
				$totalQuestions = 0;
				$correctAnswers = 0;
				$wrongAnswers = 0;
				$quesInteractions = 0;
				$attemptedInteractions = 0; 
				$qaExists = false;
				for($k = 0; $k < $numberOfSlides; $k++)
				{
					for($i = 0; $i < count($jsonData['presentation']['slides'][$k]['elements']); $i++)
					{
						$qrow = $jsonData['presentation']['slides'][$k]['elements'][$i];
						if($qrow["action"]["library"] == "H5P.SingleChoiceSet 1.3")
						{
							$quesInteractions += 1;
							$qaExists = true;
							$questions1 =$qrow["action"]["params"]["choices"];
							$totalQuestions = $totalQuestions + count($questions1);
							if(isset($progressData["answers"][$k][$i]["answers"]['corrects']))
							{
								$correctAnswers =  $correctAnswers + $progressData["answers"][$k][$i]["answers"]['corrects'];
								$wrongAnswers =  $wrongAnswers + $progressData["answers"][$k][$i]["answers"]['wrongs'];
								
							}
							if(!(($progressData["answers"][$k][$i]["answers"]['corrects'] == 0) && ($progressData["answers"][$k][$i]["answers"]['wrongs'] == 0))){
								$attemptedInteractions += 1;
							}
						}
						else if($qrow["action"]["library"] == "H5P.Blanks 1.4")
						{
							$quesInteractions += 1;
							$qaExists = true;
							$questions2 =$qrow["action"]["params"]["questions"];
							$answers = $progressData["answers"][$k][$i];
							expDebug::dPrint("questions array.of blanks.".print_r($questions2,true)."====".print_r($answers,true),4);
							expDebug::dPrint("questions array.of blanks.".$questions2[0]."+++++".$answers[0],4);
							$totalQuestions = $totalQuestions + count($questions2);
						/*	if(count($answers) > 0){
								$attemptedInteractions += 1;
								expDebug::dPrint("incrementing inside blanks");
							} */
							$l = 0;
							$attemptCheck = 0;
							for($z = 0; $z < count($answers); $z++)
							{
							// I am *fill* and *fill1*
							 if($answers[$z] != "" && $attemptCheck == 0){
								$attemptCheck = 1;		
							  }
								//	$pos = strrpos($questions2[0], "*".$answers[$z]."*");
							   $pos = strrpos($questions2[$l++], "*".$answers[$z]."*");
							    if ($pos === false) { // note: three equal signs
							// not found...
								  $wrongAnswers = $wrongAnswers + 1;
							    }else
							    {
							     $correctAnswers = $correctAnswers + 1;
							    }
							}
							if($attemptCheck == 1){
								$attemptedInteractions += 1;
								expDebug::dPrint("incrementing inside blanksss correct and wrong answerrs".$wrongAnswers." spacingggggg ".$correctAnswers,4);
							
							}
						}
							else if($qrow["action"]["library"] == "H5P.MarkTheWords 1.5")
							{
							$quesInteractions += 1;
							$qaExists = true;	
							$questions3 =$qrow["action"]["params"]["textField"];
							$answers = $progressData["answers"][$k][$i];
						
							expDebug::dPrint("questions mark the words.".print_r($questions3,true)."====".print_r($answers,true),4);
							expDebug::dPrint("questions mark the words.".$questions3."+++++".$answers[0],4);
							if(count($answers) > 0){
								$attemptedInteractions += 1;
							}
							
							
									$markwordqarr = explode(" ",$questions3);
									$count_mark_the_words = 0;
									foreach($markwordqarr as $contents)
									{
									
										
									$res =  preg_match("/\*(.*?)\*/", $contents, $blank_ans);
								expDebug::dPrint("the match foundddd".$blank_ans[0],4);
							if($res !=0 ) //&& $blank_ans[1] == $progressData["answers"][$i][$l]){
							$count_mark_the_words += 1 ;
								
							}
								//	{
								$totalQuestions = $totalQuestions + $count_mark_the_words;
								$markTheWords_WrongAns = 0;
								$markTheWords_CorrectAns = 0;
								for($z = 0; $z < count($answers); $z++)
								{
								//	$totalQuestions = $totalQuestions + 1;
							//	$markwordqarr = explode(" ",$questions3);
								
							$markword = $markwordqarr[$answers[$z]];
							$res_ans =  preg_match("/\*(.*?)\*/", $markword, $blank_ans1);
							expDebug::dPrint("the actual match foundddd".$blank_ans1[0],4);
								if($res_ans !=0 ){
						
								$markTheWords_CorrectAns += 1;
								//$correctAnswers = $correctAnswers + 1;
							}
								else{
								$markTheWords_WrongAns =  $markTheWords_WrongAns + 1;
								$wrongAnswers = $wrongAnswers + 1;
							}
								
							/*		if($markword[0]== "*" && $markword[count($markword)-1] == "*")
							$correctAnswers = $correctAnswers + 1;
								else{
									$markTheWords_WrongAns =  $markTheWords_WrongAns + 1;
									$wrongAnswers = $wrongAnswers + 1;
							}
								
							*/
							expDebug::dPrint("questions mark the words.single.".$markword[0]."===".$markword[count($markword)-1],4);
								
							}
							if($markTheWords_CorrectAns > $markTheWords_WrongAns)
								$correctAnswers = $correctAnswers + ($markTheWords_CorrectAns - $markTheWords_WrongAns);

							}
							else if($qrow["action"]["library"] == "H5P.DragText 1.4")
							{
							$quesInteractions += 1;
							$qaExists = true;
							$questions4 =$qrow["action"]["params"]["textField"];
							$answers = $progressData["answers"][$k][$i];
								
							if(count($answers) > 0){
								$attemptedInteractions += 1;
							}
							
							expDebug::dPrint("questions DRag text.".print_r($questions4,true)."====".print_r($answers,true),4);
							expDebug::dPrint("questions DRag text.".$questions4."+++++".$answers[0],4);
						
							//count the number of asterisks and divide by 2 for total number of questions.
							$count_drag_text = floor((substr_count($questions4,"*"))/2);
									$ques4 = count($questions4);									
							$totalQuestions = $totalQuestions + $count_drag_text;
							for($z = 0; $z < count($answers); $z++)
							{
							 if($answers[$z]['draggable'] == $answers[$z]['droppable'])
							  $correctAnswers = $correctAnswers + 1;
							 else
							 	$wrongAnswers = $wrongAnswers + 1;
							 }
						
									
							 }
									
									
						}
				 }
						 expDebug::dPrint("total questions:".$totalQuestions ."total correct:".$correctAnswers . "total wrong:".$wrongAnswers ."qaExists :".$qaExists ."quesInteractions presentation:".$quesInteractions ."attemptedInteractions presentation:".$attemptedInteractions ,4);

						 $pre_progress = intval($noOfSlidesSeen,10)/$numberOfSlides*100;
						 if($quesInteractions != 0){
						 	
						 	$quesInteractions += 1;
						 	$pre_progress_interactions = (1/$quesInteractions)*$pre_progress;
						 	
						 	$pre_progress_withQues = intval($attemptedInteractions,10)/$quesInteractions*100 + $pre_progress_interactions;
						 	expDebug::dPrint("pre_progress with questions:::::".$pre_progress_withQues ,4);
						 }	
				
				
				
			   if( $qaExists) //if content has q&a
				{


				$obtainedScore  = $correctAnswers/$totalQuestions*100;
				$pre_progress_withQues = round($pre_progress_withQues); // rounding the value of progress 
                if($obtainedScore >= $passingscore) // passed the test
				{
					if($pre_progress_withQues == "100"){  //whether attempt is set or not doesn not matter
						$_REQUEST["contentstatus"]	= 'completed';
						$_REQUEST["status"]			= 'lrn_crs_cmp_cmp';
						$_REQUEST["score"]          = $obtainedScore;
						$_REQUEST["progress"]       = "100";
					}
					else{  // total progress is not 100%
						$_REQUEST["contentstatus"]	= 'incomplete';
						$_REQUEST["status"]			= 'lrn_crs_cmp_inp';
						$_REQUEST["score"]          = $obtainedScore;
						$_REQUEST["progress"]       = $pre_progress_withQues;
					}
				}
				else{
					if(($pre_progress_withQues == "100" || round($pre_progress_withQues) == 100) && $_REQUEST["AttemptLeft"] == "notset" && ($_REQUEST["MaxAttempt"] == '' || $_REQUEST["MaxAttempt"] == NULL || $_REQUEST["MaxAttempt"] == 'null'))
					{
						$_REQUEST["contentstatus"]	= 'incomplete';
						$_REQUEST["status"]			= 'lrn_crs_cmp_inc';
						$_REQUEST["score"]          = $obtainedScore;
						$_REQUEST["progress"]       = "100";
					}
					else{  // total progress is not 100%
						$_REQUEST["contentstatus"]	= 'incomplete';
						$_REQUEST["status"]			= 'lrn_crs_cmp_inp';
						$_REQUEST["score"]          = $obtainedScore;
						$_REQUEST["progress"]       = $pre_progress_withQues;
					}
					
				}	

				
				
				
				
				}
				
				else if($numberOfSlides == $noOfSlidesSeen && $noOfSlidesSeen > 0)
				{
					$_REQUEST["contentstatus"]	= 'completed';
					$_REQUEST["status"]			= 'lrn_crs_cmp_cmp';
				//	$_REQUEST["score"]          = "100";
					$_REQUEST["progress"]       = "100";
				}
				else if($numberOfSlides != $noOfSlidesSeen && $numberOfSlides > 0)
				{
					$_REQUEST["contentstatus"]	= 'incomplete';
					$_REQUEST["status"]			= 'lrn_crs_cmp_inp';
					$_REQUEST["progress"]       =  $pre_progress;
				}
				$h5p_status = $_REQUEST["status"];
				$suspendData = array();
				$suspendData["session_id"]= "";
				$suspendData["current_position"]= intval($noOfSlidesSeen,10);
				
				if($qaExists){
					$suspendData["progress"]= $pre_progress_withQues;
					$suspendData["status"]  = $h5p_status;
					$suspendData["score"]   = $obtainedScore;
					$suspendData["pass_score"] = $passingscore;
					expDebug::dPrint("suspend data abhishek:".print_r($suspendData,true),5);
				}
				else{
				$suspendData["progress"]= intval($noOfSlidesSeen,10)/$numberOfSlides*100;
				expDebug::dPrint("ss:progress no question interactions:".$suspendData["progress"],4);
				}	
				//$suspendData["video_duration"] =$totalVideoTime;
				$suspendData["additional_data"] ="{video_session_id:''}";
				$_POST['video_suspend_data'] = $suspendData;
				$_REQUEST["suspend_data"] = rawurlencode(json_encode($suspendData));
				$_REQUEST["compstatus"] = "";
				
				//resetH5PLaunchUserData($data[0]->content_main_id,$_REQUEST['regid'],$progressData);
				$launchInfo = updateLaunchWrapper();
				expDebug::dPrint("suspend data suresh:".print_r($launchInfo,true),5);
				echo drupal_json_output($launchInfo);
				exit();	
			}
		 	
		 }
		 else if($_REQUEST["type"]== "h5p") //video
		 {
		 	$h5p_cnt_id = $_REQUEST['h5p_cnt_id'];		
		 	 if($h5p_cnt_id == null || $h5p_cnt_id == "" )
		 	 	exit();
			 $data = getH5PProgress($_REQUEST["regid"],$h5p_cnt_id);
			 $videoSeen = "";
			 $totalVideoTime = "";	
		 	if($data != null && $data != "" && isset($data))
		 	{
			 	$progressData = json_decode($data[0]->data,true);
			 	expDebug::dPrint("the entire data obtained..:".print_r($progressData,true),4);
				$videoSeen =ceil($progressData["progress"]);
			 	//$videoSeen = $progressData["progress"];
				$totalVideoTime = $progressData["totaltime"];
				expDebug::dPrint("video total and video seen....".$totalVideoTime. "===".$videoSeen ,4);
				$json_content = $data[0]->json_content;
				$jsonData = json_decode($json_content,true);
				expDebug::dPrint("the entire jsondata obtainedddd..:".print_r($jsonData,true),4);
				$passingScore =		$jsonData['interactiveVideo']['video']['passingscore'];
				$retryOption = $jsonData['override']['overrideRetry'];
				$interactions = $jsonData['interactiveVideo']["assets"]["interactions"];
				$summaries = $jsonData['interactiveVideo']["summary"]["task"]["library"];
				$questions = array();
				$questions1 = array();
				$questions2 = array();
				$questions3 = array();
				$summary = array();
				$totalQuestions = 0;
				$correctAnswers = 0;
				$wrongAnswers = 0;
				$quesInteractions = 0;
				$attemptedInteractions = 0;
				$qaExists = false;
				for($k = 0; $k < count($interactions); $k++)
				{
					$qrow = $interactions[$k];
					expDebug::dPrint("Lib:".$qrow["action"]["library"] ,4);
					if($qrow["action"]["library"] == "H5P.SingleChoiceSet 1.3")
					{
						$quesInteractions += 1;
						$qaExists = true;
						$questions1 =$qrow["action"]["params"]["choices"];
						$totalQuestions = $totalQuestions + count($questions1);
						if(isset($progressData["answers"][$k]["answers"]['corrects']))
						{
							$correctAnswers =  $correctAnswers + $progressData["answers"][$k]["answers"]['corrects'];
							$wrongAnswers =  $wrongAnswers + $progressData["answers"][$k]["answers"]['wrongs'];
							if(!(($progressData["answers"][$k]["answers"]['corrects'] == 0) && ($progressData["answers"][$k]["answers"]['wrongs'] == 0))){
								$attemptedInteractions += 1;
							}
						}
					}
					else if($qrow["action"]["library"] == "H5P.Blanks 1.4")
					{
						$quesInteractions += 1;
						$qaExists = true;
						$questions2 =$qrow["action"]["params"]["questions"];
						$answers = $progressData["answers"][$k];
						expDebug::dPrint("questions array.of blanks.".print_r($questions2,true)."====".print_r($answers,true),4);
						expDebug::dPrint("questions array.of blanks.".$questions2[0]."+++++".$answers[0],4);
						$totalQuestions = $totalQuestions + count($questions2);
						$l = 0;
						$attemptCheck = 0;
						for($z = 0; $z < count($answers); $z++)
						{ 
						if($answers[$z] != "" && $attemptCheck == 0){
							$attemptCheck = 1;
							
						}
						
						 $pos = strrpos($questions2[$l++], "*".$answers[$z]."*");
							if ($pos === false) { // note: three equal signs
								// not found...
								$wrongAnswers = $wrongAnswers + 1;
							}else
							{
								$correctAnswers = $correctAnswers + 1;			
							}
						}
				
						if($attemptCheck == 1){
							$attemptedInteractions += 1;
							expDebug::dPrint("incrementing inside blanksss correct and wrong answerrs".$wrongAnswers." spacingggggg ".$correctAnswers ,4);
								
						}
					}
					else if($qrow["action"]["library"] == "H5P.MarkTheWords 1.5")
					{
						$qaExists = true;
						$quesInteractions += 1;
						$questions3 =$qrow["action"]["params"]["textField"];
						$answers = $progressData["answers"][$k];
						
						expDebug::dPrint("questions mark the words.".print_r($questions3,true)."====".print_r($answers,true),4);
						expDebug::dPrint("questions mark the words.".$questions3."+++++".$answers[0],4);
						if(count($answers) > 0){
							$attemptedInteractions += 1;
						}
					
						$markwordqarr = explode(" ",$questions3);
						$count_mark_the_words = 0;
						foreach($markwordqarr as $contents)
						{
						  
							$res =  preg_match("/\*(.*?)\*/", $contents, $blank_ans);
							if($res !=0 ) //&& $blank_ans[1] == $progressData["answers"][$i][$l]){
								$count_mark_the_words += 1 ;
							
						}
					//	{
						$totalQuestions = $totalQuestions + $count_mark_the_words;
					    $markTheWords_WrongAns = 0;
					    $markTheWords_CorrectAns = 0;
							for($z = 0; $z < count($answers); $z++)
							{
							//	$totalQuestions = $totalQuestions + 1;
						 	//	$markwordqarr = explode(" ",$questions3);
						 		
						 		$markword = $markwordqarr[$answers[$z]];
						 		$res_ans =  preg_match("/\*(.*?)\*/", $markword, $blank_ans1);
						 		if($res_ans !=0 ){
						 			
						 			$markTheWords_CorrectAns += 1;
						 			//$correctAnswers = $correctAnswers + 1;
						 		}
						 		else{
						 			$markTheWords_WrongAns =  $markTheWords_WrongAns + 1;
						 			$wrongAnswers = $wrongAnswers + 1;
						 		}		 		
							}
							if($markTheWords_CorrectAns > $markTheWords_WrongAns)
								$correctAnswers = $correctAnswers + ($markTheWords_CorrectAns - $markTheWords_WrongAns);	
						
					}
					else if($qrow["action"]["library"] == "H5P.DragText 1.4")
					{
						$quesInteractions += 1;
						$qaExists = true;
						$questions4 =$qrow["action"]["params"]["textField"];
						$answers = $progressData["answers"][$k];
					
						expDebug::dPrint("questions DRag text.".print_r($questions4,true)."====".print_r($answers,true),4);
						//count the number of asterisks and divide by 2 for total number of questions.
						$count_drag_text = floor((substr_count($questions4,"*"))/2);
						$ques4 = count($questions4);

						if(count($answers) > 0){
							$attemptedInteractions += 1;
						}
		
						$totalQuestions = $totalQuestions + $count_drag_text;
						for($z = 0; $z < count($answers); $z++)
						{
							if($answers[$z]['draggable'] == $answers[$z]['droppable'])
								$correctAnswers = $correctAnswers + 1;
							else
								$wrongAnswers = $wrongAnswers + 1;
						}
						
					
					}
					
					
				}
				
				expDebug::dPrint("total questions:".$totalQuestions ."total correct:".$correctAnswers . "total wrong:".$wrongAnswers ."qaExists :".$qaExists ."quesInteractions presentation:".$quesInteractions ."attemptedInteractions presentation:".$attemptedInteractions ,4);
			
				$enc1 = new Encrypt();
				$video_session_id = $enc1->encrypt(implode('_', array($_REQUEST['courseid'], $_REQUEST['classid'], $_REQUEST['lessonid'], $_REQUEST['versionid'], $_REQUEST['enrollid'])));
					
				$minutesandseconds = explode(":",$totalVideoTime);
				
				
				if(count($minutesandseconds) == 3){
					$hours = $minutesandseconds[0];
					$minutes =$minutesandseconds[1]; //0:29
					$sec =  $minutesandseconds[2];
					$totalVideoTime = ($hours * 60 *60)+($minutes * 60)+$sec;
				}
				else{
				$minutes =$minutesandseconds[0]; //0:29
				$sec =  $minutesandseconds[1];	
				$totalVideoTime = ($minutes * 60)+$sec;
				}
				$vid_progress = intval($videoSeen,10)/$totalVideoTime*100;
				
				
				
				if($quesInteractions != 0){
					
					$quesInteractions += 1;
					
					$vid_progress_interactions = (1/$quesInteractions)*$vid_progress;
					
					
					
				  $vid_progress_withQues = intval($attemptedInteractions,10)/$quesInteractions*100 + $vid_progress_interactions;
			// ADDING FOR 0073768
				  if($vid_progress_withQues > 100)
				  {
				  	$vid_progress_withQues = 100;
				  }
				  
				  
				}
				
			// ADDING FOR 0073768
				if($vid_progress > 100)
				{
					$vid_progress = 100;
				}
				
				if($qaExists)
			 	{
					if($totalQuestions == $correctAnswers && $correctAnswers != 0 ) // && (round($videoSeen) >= $totalVideoTime)) added condition if entire video is seen
					{
							$_REQUEST["contentstatus"]	= 'completed';
							$_REQUEST["status"]			= 'lrn_crs_cmp_cmp';
							$_REQUEST["score"]          = "100";
							$_REQUEST["progress"]       =  $vid_progress_withQues;
					} elseif($correctAnswers == 0 && $wrongAnswers == 0 && $vid_progress_withQues == 0) 
					{
							$_REQUEST["contentstatus"]	= 'not attempted';	// attempts should not be reduced if the progress is zero
							$_REQUEST["status"]			= 'lrn_crs_cmp_inp';
							$_REQUEST["progress"]       =  "0";
					}
					
					 else {
					 	
					 	 expDebug::dPrint("test actual value of progress---". var_export($vid_progress_withQues,1) );
							$obtainedScore = $correctAnswers/$totalQuestions*100;
							$_REQUEST["score"]          =  $obtainedScore;
							$_REQUEST["progress"]       =  $vid_progress_withQues;
							if($obtainedScore >= $passingScore )
							{
								$_REQUEST["contentstatus"]	= 'completed';
								$_REQUEST["status"]			= 'lrn_crs_cmp_cmp';
								
							} 
							//  $retryOption should not be considered here : as even though the user has retry option but he has seen entire video.
							// considering retry option brings uncertainty for the completion logic i.e until the user passes the quiz he stays in enrolled status.
							else if(($obtainedScore < $passingScore) && ($_REQUEST["AttemptLeft"] == "notset") && ($_REQUEST["MaxAttempt"] == '' || $_REQUEST["MaxAttempt"] == NULL || $_REQUEST["MaxAttempt"] == 'null')  && (round($vid_progress_withQues) == 100) ) // (round($videoSeen) >= $totalVideoTime || $totalVideoTime == round($videoSeen))){				
							{
								$_REQUEST["contentstatus"]	= 'incomplete';
								$_REQUEST["status"]			= 'lrn_crs_cmp_inc';
							
							}
							
							else{
								$_REQUEST["contentstatus"]	= 'incomplete';
								$_REQUEST["status"]			= 'lrn_crs_cmp_inp';
							}
					}
				}
				else //no question & answer
				{

						if(round($videoSeen) >= $totalVideoTime || $totalVideoTime == round($videoSeen))
						{
							$_REQUEST["contentstatus"]	= 'completed';
							$_REQUEST["status"]			= 'lrn_crs_cmp_cmp';
							$_REQUEST["progress"]       = "100";
						}
 						else
					    {
							$_REQUEST["contentstatus"]	= 'incomplete';
							$_REQUEST["status"]			= 'lrn_crs_cmp_inp';
							$_REQUEST["progress"]       =  $vid_progress;
						}
				}
				//resetH5PLaunchUserData($data[0]->content_main_id,$_REQUEST['regid'],$progressData);

				expDebug::dPrint("attempts leftttt:".$_REQUEST["AttemptLeft"],4);
				
				
				$select = db_select("slt_enrollment",'enr');
				$select->addField('enr','reg_status', 'reg_status');
				$select->condition('enr.id',$_REQUEST['regid'],'=');
				$selectRes = $select->execute()->fetchAssoc();
				$reg_status = $selectRes['reg_status'];
				if($reg_status == 'lrn_crs_reg_cnf' ) {
					
					
					$suspendData = array();
					$suspendData["session_id"]= $video_session_id;
					$suspendData["current_position"]= $videoSeen;
				
				if($qaExists){
					$suspendData["progress"]= $vid_progress_withQues;
					$suspendData["quesInteractions"]  = true;
					expDebug::dPrint("ss:progress when question interactions are present:".$suspendData["progress"],4);
				}
				else{	
				
				    $suspendData["progress"] = $vid_progress;
				    $suspendData["quesInteractions"]  = false;
                	expDebug::dPrint("ss:progress when no question interactions:".$suspendData["progress"],4);
				}
					
					
					$suspendData["video_duration"] =$totalVideoTime;
					$suspendData["additional_data"] ="{video_session_id:'".$video_session_id."'}";
					$_POST['video_suspend_data'] = $suspendData;
					
					
					$_REQUEST["suspend_data"] = rawurlencode(json_encode($suspendData));
					
					$launchInfo = updateLaunchWrapper();
					unset($_SESSION[$video_session_id]);
					unset($_SESSION['progress_over']);
					unset($_SESSION['suspendData'][$video_session_id]);
					expDebug::dPrint("suspend data suresh:".print_r($launchInfo,true),5);
					
					
					echo drupal_json_output($launchInfo);
				}	
				exit();
			 }
			 else //if no record found
			 {
			 	exit();
			 }
		}
		if($op == 'vod') {
			
			
			//process video launch progres information
			$video_session_id == '';
			$video_session_id = $_POST['additional_data']['video_session_id'];
			if($video_session_id == '') {
				$video_session_id = $_REQUEST['session_id'];
			}
			if(!isset($_SESSION[$video_session_id]) || $_REQUEST['progress'] > $_SESSION[$video_session_id]['progress']) {
				$request_array = $_REQUEST;
				if((array_key_exists('q', $request_array)) || (array_key_exists('__csrf_magic', $request_array))) {
					unset($request_array['q']);
					unset($request_array['__csrf_magic']);	
				}
				$_SESSION[$video_session_id] = $request_array;
			}
			expDebug::dPrint('video progress $video_session_id'.print_r($video_session_id, 5));
			expDebug::dPrint('video progress details current'.print_r($_REQUEST, 5));
			expDebug::dPrint('video progress details session'.print_r($_SESSION[$video_session_id], 5));
			expDebug::dPrint('Bookmark = return from update launch method as this request is just to update the video progress in session');
			return true;	//return from update launch method as this request is just to update the video progress in session
		} else if (isset($_POST['video_suspend_data'])) {
			// this block comes when video closed
			if($_POST['video_suspend_data'] != "null") {
				// video has been started and closed
				expDebug::dPrint('Bookmark = video has been started and closed', 5);
				$video_suspend_data = json_decode($_POST['video_suspend_data'], true);
				$video_session_id = $video_suspend_data['additional_data']['video_session_id'];
				if(isset($_SESSION[$video_session_id])) {
					// some progress is stored in session already
					expDebug::dPrint('Bookmark = some progress is stored '.print_r($_SESSION[$video_session_id], 1), 5);
					if($video_suspend_data['progress'] >= $_SESSION[$video_session_id]['progress']) {
						
						$_SESSION[$video_session_id] = $video_suspend_data;
					}
				} else {
					//no previous progress is stored. Progress sent on close needs to be stored
					expDebug::dPrint('Bookmark = no previous progress is stored. Progress sent on close needs to be stored'.print_r($video_suspend_data,1),5);				
					$_SESSION[$video_session_id] = $video_suspend_data;
				}
				$progress = $_SESSION[$video_session_id]['progress'];
				if($progress >= 100) {
					$_REQUEST["contentstatus"]	= 'completed';
					$_REQUEST["status"]			= 'lrn_crs_cmp_cmp';
				} elseif($progress == 0) {
					$_REQUEST["contentstatus"]	= 'not attempted';	// attempts should not be reduced if the progress is zero
					$_REQUEST["status"]			= 'lrn_crs_cmp_inp';
				} else {
					$_REQUEST["contentstatus"]	= 'incomplete';
					$_REQUEST["status"]			= 'lrn_crs_cmp_inp';
				}
				$_REQUEST["progress"] = $progress;
			} else {
				$enc1 = new Encrypt();
				$video_session_id = $enc1->encrypt(implode('_', array($_REQUEST['courseid'], $_REQUEST['classid'], $_REQUEST['lessonid'], $_REQUEST['versionid'], $_REQUEST['enrollid'])));
				expDebug::dPrint('Error in launching the video and video_suspend_data is null, suspend_data set in session to be saved', 5);	
				$_REQUEST["contentstatus"]	= "not attempted";		// attempts should not be reduced as the video was not played
				$_REQUEST["status"]			= ($_REQUEST['prev_content_status'] != 'null' && $_REQUEST['prev_content_status'] != '' ? $_REQUEST['prev_content_status'] : 'lrn_crs_cmp_enr');
			}			
			$_REQUEST["suspend_data"] = rawurlencode(json_encode($_SESSION[$video_session_id]));
		}
		expDebug::dPrint('video progress details post on update'.print_r($_POST, 1), 5);
			$launchInfo = updateLaunchWrapper();
			unset($_SESSION[$video_session_id]);
			unset($_SESSION['progress_over']);
			unset($_SESSION['suspendData'][$video_session_id]);
			echo drupal_json_output($launchInfo);

	}catch (Exception $ex) {
		watchdog_exception('updateLaunch', $ex);
		expertusErrorThrow($ex);
	}
}

function updateLaunchWrapper(){
	try{
		ini_set('display_errors', FAlSE);
		$lunObj = new stdClass();
		$lunObj->userId       = getSltpersonUserId();

		if($_REQUEST['apiname'])
		{
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_core/exp_sp_core.module');
		}

		$objecttype = 'cre_sys_obt_cls';
		$lunObj->calltype = 'null';
		if(isset($_REQUEST["objecttype"])){
			$objecttype   = ($_REQUEST["objecttype"]!=null && $_REQUEST["objecttype"]!='') ?"'".$_REQUEST["objecttype"]."'":"null";
			$enrollid   = ($_REQUEST["enrollid"]!=null && $_REQUEST["enrollid"]!='') ?"'".orgValidate($_REQUEST["enrollid"])."'":"null";
			$objecttype = str_replace("'", "", $objecttype);
			$enrollid = str_replace("'", "", $enrollid);
		}

		if(isset($_REQUEST["calltype"])){
			$callType = stripApostrophe(stripRegStatus($_REQUEST["calltype"]));
			$lunObj->calltype = !empty($callType) ? "'".$callType."'":'';
		}
		// Certificate 	-> cre_sys_obt_crt
		// Curricula 		-> cre_sys_obt_cur
		// Learning plan 	-> cre_sys_obt_trn
		// Class 			-> cre_sys_obt_cls
		expDebug::dPrint('REQUEST variable --> '.print_r($_REQUEST, 1), 2);
		
		
		$lunObj->RegId        = ($_REQUEST["regid"]!=null && $_REQUEST["regid"]!='') ?orgValidate($_REQUEST["regid"]):"null";
		$lunObj->LearnerId    = ($_REQUEST["stid"]!=null && $_REQUEST["stid"]!='') ?orgValidate($_REQUEST["stid"]):"null";
		$lunObj->CourseId     = ($_REQUEST["courseid"]!=null && $_REQUEST["courseid"]!='') ?orgValidate($_REQUEST["courseid"]):"null";
		$lunObj->ClassId      = ($_REQUEST["classid"]!=null && $_REQUEST["classid"]!='') ?orgValidate($_REQUEST["classid"]):"null";
		$lunObj->LessonId     = ($_REQUEST["lessonid"]!=null && $_REQUEST["lessonid"]!='' && $_REQUEST["lessonid"]!='undefined') ?orgValidate($_REQUEST["lessonid"]):"null";
		$lunObj->VersionId     = ($_REQUEST["versionid"]!=null && $_REQUEST["versionid"]!='') ?orgValidate($_REQUEST["versionid"]):"null";
		$lunObj->Status       = ($_REQUEST["status"]!=null && $_REQUEST["status"]!='') ?stripApostrophe(stripRegStatus($_REQUEST["status"])):"null";
		$lunObj->SessionTime  = ($_REQUEST["sestime"]!=null && $_REQUEST["sestime"]!='') ?stripApostrophe($_REQUEST["sestime"]):"null";
		$lunObj->Score        = ($_REQUEST["score"]!=null && $_REQUEST["score"]!='') ?orgValidate($_REQUEST["score"]):"0.00";
		//$lunObj->Grade        = ($_GET["grade"]!=null && $_REQUEST["grade"]!='') ?stripApostrophe($_REQUEST["grade"]):"null";
		$lunObj->Location     = ($_REQUEST["location"]!=null && $_REQUEST["location"]!='') ?rawurlencode($_REQUEST["location"]):"null";
		/**
		 * 	$_REQUEST["contentstatus"]		//for SCORM 1.2 => lesson_status and for 2004 => success_status
			$_REQUEST["compstatus"]			//for SCORM 2004 => completion_status and rest of the contents, it will be null
		 */
		$lunObj->ContentStatus = ($_REQUEST["contentstatus"]!=null && $_REQUEST["contentstatus"]!='') ?stripApostrophe($_REQUEST["contentstatus"]):"null";
		$lunObj->CompletionStatus = ($_REQUEST["compstatus"]!=null && $_REQUEST["compstatus"]!='') ?strtolower(trim(stripApostrophe($_REQUEST["compstatus"]))):"null";	
		$lunObj->launch_data	= ($_REQUEST["launch_data"]!=null && $_REQUEST["launch_data"]!='') ?stripApostrophe($_REQUEST["launch_data"]):"null";
		$lunObj->suspend_data	= ($_REQUEST["suspend_data"]!=null && $_REQUEST["suspend_data"]!='') ? $_REQUEST["suspend_data"]:"null";
		$lunObj->exit					= ($_REQUEST["exit"]!=null && $_REQUEST["exit"]!='') ?stripApostrophe($_REQUEST["exit"]):"null";
		$lunObj->orgScore 		= $Score;
		$lunObj->assessCheck       = ($_REQUEST["prestatus"]!=null && $_REQUEST["prestatus"]!='') ?stripApostrophe($_REQUEST["prestatus"]):"null";
		$lunObj->survey_id       = ($_REQUEST["surveyid"]!=null && $_REQUEST["surveyid"]!='') ?stripApostrophe($_REQUEST["surveyid"]):"null";
		//49519: SCORM 1.2 and 2004 - Need to change content completion and attempts reduce logic in product
		//content type and version is checked for SCORM 1.2 and SCORM 2004
		$lunObj->contentType = ($_REQUEST["contenttype"]!=null && $_REQUEST["contenttype"]!='') ?stripApostrophe($_REQUEST["contenttype"]):"null";
		$lunObj->contentVersion = ($_REQUEST["contentversion"]!=null && $_REQUEST["contentversion"]!='') ?stripApostrophe($_REQUEST["contentversion"]):"null"; 
		$lunObj->launchflag = ($_REQUEST["launchflag"]!=null && $_REQUEST["launchflag"]!='') ?orgValidate($_REQUEST["launchflag"]):"null";
		$lunObj->contentCode = ($_REQUEST["contentcode"]!=null && $_REQUEST["contentcode"]!='') ?stripApostrophe($_REQUEST["contentcode"]):"null";
		$lunObj->total_page = ($_REQUEST["total_page"]!=null && $_REQUEST["total_page"]!='') ?stripApostrophe($_REQUEST["total_page"]):"null";
		$lunObj->current_page = ($_REQUEST["current_page"]!=null && $_REQUEST["current_page"]!='') ?stripApostrophe($_REQUEST["current_page"]):"null";
		$lunObj->playfrom = ($_REQUEST["playfrom"]!=null && $_REQUEST["playfrom"]!='') ?stripApostrophe($_REQUEST["playfrom"]):"null";
		$lunObj->progress = ($_REQUEST["progress"]!=null && $_REQUEST["progress"]!='') ?stripApostrophe($_REQUEST["progress"]):"null";

        $lunObj->AttemptLeft = ($_REQUEST["AttemptLeft"]!=null && $_REQUEST["AttemptLeft"]!='') ?stripApostrophe($_REQUEST["AttemptLeft"]):"null";
        $lunObj->type = ($_REQUEST["type"]!=null && $_REQUEST["type"]!='') ?stripApostrophe($_REQUEST["type"]):"null";
        
        
        
		$lunObj->prestatus = ($_REQUEST['prestatus']!=null && $_REQUEST['prestatus']!='') ?stripApostrophe($_REQUEST['prestatus']):0;
		expDebug::dPrint("lunObj--->".print_r($lunObj,true),5);
		$config=getConfig("exp_sp");
		$mylearn_version = $config['mylearn_version'];
		expDebug::dPrint('conten code receive: ' . $lunObj->contentCode );
		$content_conversion = $config['content_conversion'];
		if($_REQUEST['apiname'] && $callType == "assessment" ){
			$lunObj->contentCode = "sry_det_typ_ass";
		}elseif($_REQUEST['apiname'] && $callType == "survey" ){
			$lunObj->contentCode = "sry_det_typ_sry";
		}
		
		
		if($lunObj->playfrom == 'contentplayer'){
			if($lunObj->contentType == 'Knowledge Content'){
					$launchURL = $_REQUEST['launchUrl'];
					$urlinfo = pathinfo($launchURL);
					$filename = $urlinfo['basename'];
					$extension = _mime_content_type($urlinfo['basename']);
					$lunObj->contentCode = 'lrn_cnt_typ_knc';
					if($extension == "application/pdf"){
						$attsummaryDetails = get_attendance_summary($lunObj->RegId, $lunObj->CourseId, $lunObj->ClassId, $lunObj->LessonId, $lunObj->VersionId, NULL);
						$prevCurrPage = json_decode(unserialize($attsummaryDetails->suspend_data));
						$prevProgress = $attsummaryDetails->progress;
						$currentpage = $lunObj->current_page;
				$progress = $lunObj->current_page*100/$lunObj->total_page;
				$suspenddata = array("total_page"=>$lunObj->total_page,'current_page'=>$lunObj->current_page,'progress'=>$progress);
				$suspend_data = json_encode($suspenddata);
						if($currentpage > $prevCurrPage->current_page){
							$lunObj->suspend_data = $suspend_data;
							$lunObj->progress = round($progress);
						}else{
							$lunObj->suspend_data = $attsummaryDetails->suspend_data;
							$lunObj->progress = $prevProgress;
					}
					}
					else if ($_REQUEST["type"]== "h5p"){
					$attsummaryDetails = get_attendance_summary($lunObj->RegId, $lunObj->CourseId, $lunObj->ClassId, $lunObj->LessonId, $lunObj->VersionId, 0);
					$prev_suspend_data =    json_decode(rawurldecode($attsummaryDetails->suspend_data), true);
					expDebug::dPrint("debug of the entire suspended data Abhishek:".print_r($prev_suspend_data, true),4);
					$update_suspend_data =  new stdClass();
			
					$prevProgress = $attsummaryDetails->progress;
					$currentpage = $lunObj->current_page;
					$current_suspend_data	= json_decode(rawurldecode($_REQUEST['suspend_data']), true);
					expDebug::dPrint("debug of the entire current suspended data Abhishek:".print_r($current_suspend_data, true),4);
					$suspend_data = json_encode($current_suspend_data);
				
					if(isset($prev_suspend_data['progress']) && $prev_suspend_data['progress'] >= $current_suspend_data['progress'] && $prev_suspend_data['score'] > $lunObj->Score && $prev_suspend_data['score'] > $prev_suspend_data['pass_score'] ) {
						$update_suspend_data->progress  = $prev_suspend_data['progress'];
						$update_suspend_data->score  = $prev_suspend_data['score'];
						$lunObj->progress = $prev_suspend_data['progress'];
						$lunObj->Score    = $prev_suspend_data['score'];
						if($lunObj->progress == "100" || $lunObj->progress == 100){
							$lunObj->Status = 'lrn_crs_cmp_cmp';
							$lunObj->ContentStatus = "";
						}

					}
					else if(isset($prev_suspend_data['progress']) && $prev_suspend_data['progress'] >= $current_suspend_data['progress'] && (!($prev_suspend_data['score'] > $lunObj->Score && $prev_suspend_data['score'] > $prev_suspend_data['pass_score'])))
					{
						$update_suspend_data->progress  = $prev_suspend_data['progress'];
						$update_suspend_data->score  = $current_suspend_data['score'];
						$lunObj->progress = $prev_suspend_data['progress'];

					}
					else if(!(isset($prev_suspend_data['progress']) && $prev_suspend_data['progress'] >= $current_suspend_data['progress']) && ($prev_suspend_data['score'] > $lunObj->Score && $prev_suspend_data['score'] > $prev_suspend_data['pass_score']))
					{
						$update_suspend_data->progress = $current_suspend_data['progress'];
						$update_suspend_data->score = $prev_suspend_data['score'];
						$lunObj->Score    = $prev_suspend_data['score'];
						if($lunObj->progress == "100" || $lunObj->progress == 100){
							$lunObj->Status = 'lrn_crs_cmp_cmp';
							$lunObj->ContentStatus = "";
						}

					}
					 else {
						$update_suspend_data->progress = $current_suspend_data['progress'];
						$update_suspend_data->score    = $current_suspend_data['score'];
						//$lunObj->suspend_data = $_REQUEST["suspend_data"];
						$lunObj->progress = $current_suspend_data['progress'];
						$lunObj->Score    = $current_suspend_data['score'];
					}
					
					$lunObj->suspend_data = json_encode($update_suspend_data);
				}
				 else{
				 	//73059: Content completion % is not getting updated unless we close the window and reopen it or move to the next content
						$progress = 100;
						$lunObj->ContentStatus = "Completed";
						$lunObj->CompletionStatus = "Completed";
						$lunObj->progress = $progress;
				} 
			} else if($lunObj->contentType == 'SCORM'){
				if($lunObj->Status == "lrn_crs_cmp_cmp" || $lunObj->Status == "lrn_crs_cmp_inc"){
					$progress = 100;
				}else{
					$progress = 0;
				}
				$lunObj->progress = $progress;	
				// Content Code added by Thamil
				if ($lunObj->contentVersion == '1.2') 
					$lunObj->contentCode = 'lrn_cnt_typ_srm_s12';
				if ($lunObj->contentVersion == '2004') 
					$lunObj->contentCode = 'lrn_cnt_typ_srm_s24';
			} else if($lunObj->contentType == 'Video'){
				$lunObj->contentCode = 'lrn_cnt_typ_vod';
			} else {
				// to do
			}
		}else{
			if($lunObj->contentType == 'Knowledge Content'){
				$lunObj->contentCode = 'lrn_cnt_typ_knc';
				$lunObj->progress =100;
				if ($_REQUEST["type"]== "h5p"){
					$attsummaryDetails = get_attendance_summary($lunObj->RegId, $lunObj->CourseId, $lunObj->ClassId, $lunObj->LessonId, $lunObj->VersionId, 0);
					$prev_suspend_data =    json_decode(rawurldecode($attsummaryDetails->suspend_data), true);
					$prevProgress = $attsummaryDetails->progress;
					$currentpage = $lunObj->current_page;
					$current_suspend_data	= json_decode(rawurldecode($_REQUEST['suspend_data']), true);
					$suspend_data = json_encode($current_suspend_data);
					if(isset($prev_suspend_data['progress']) && $prev_suspend_data['progress'] >= $current_suspend_data['progress']) {
						$lunObj->suspend_data = $attsummaryDetails->suspend_data;
						$lunObj->progress = $prev_suspend_data['progress'];
					}
					else{
						$lunObj->suspend_data = $_REQUEST["suspend_data"];
						$lunObj->progress = $current_suspend_data['progress'];
					}
				}
			} else if($lunObj->contentType == 'SCORM'){
				if($lunObj->Status == "lrn_crs_cmp_cmp" || $lunObj->Status == "lrn_crs_cmp_inc"){
					$progress = 100;
				}else{
					$progress = 0;
				}
				$lunObj->progress = $progress;
				// Content Code added by Thamil
				if ($lunObj->contentVersion == '1.2')
					$lunObj->contentCode = 'lrn_cnt_typ_srm_s12';
				if ($lunObj->contentVersion == '2004')
					$lunObj->contentCode = 'lrn_cnt_typ_srm_s24';
			} else if($lunObj->contentType == 'Video' || isset($_REQUEST['video_suspend_data'])){
				$lunObj->contentCode = 'lrn_cnt_typ_vod';
			} else {
				// to do
			}
			
		}
		
		if($_REQUEST['apiname'] == 'UpdateScoreAPI'){
					$lunObj->suspend_data = urldecode($lunObj->suspend_data);
					$lunObj->launch_data = urldecode($lunObj->launch_data);
		}
		$completeflag=false;
		$calltype = str_replace("'","",$lunObj->calltype);
		$prevSts = '';
		if($calltype != 'assessment' && isset($_REQUEST['video_suspend_data'])) {	//applicable only for VOD contents
			
			if($_REQUEST["type"]== "h5p"){  // code for video h5p
				$prev_attn_summary = get_attendance_summary($lunObj->RegId, $lunObj->CourseId, $lunObj->ClassId, $lunObj->LessonId, $lunObj->VersionId, 0);
				$prev_suspend_data =    json_decode(rawurldecode($prev_attn_summary->suspend_data), true);
			}
			else{
			$prev_attn_summary = get_attendance_summary($lunObj->RegId, $lunObj->CourseId, $lunObj->ClassId, $lunObj->LessonId, $lunObj->VersionId, NULL);
			$prev_suspend_data		= is_object($prev_attn_summary) ? json_decode(rawurldecode($prev_attn_summary->suspend_data), true) : null;
			}
			$current_suspend_data	= json_decode(rawurldecode($_REQUEST['suspend_data']), true);
			expDebug::dPrint('get attn summary '.print_r($prev_suspend_data, 1), 4);
			expDebug::dPrint('current attn summary '.print_r($current_suspend_data, 1), 4);
			if(isset($prev_suspend_data['progress']) && $prev_suspend_data['progress'] >= $current_suspend_data['progress']) {
				$lunObj->suspend_data = $prev_attn_summary->suspend_data;
				
				if($_REQUEST["type"]== "h5p"){  // code for video h5p
				$lunObj->progress = $prev_suspend_data['progress'];
				}
			} 
			if(isset($prev_suspend_data["quesInteractions"]) && $_REQUEST["type"]== "h5p" && ($prev_suspend_data['progress'] == 100 || $prev_suspend_data['progress'] == "100") &&  $prev_suspend_data["quesInteractions"] == false ){
			   // $lunObj->progress = $prev_suspend_data['progress'];
			    $lunObj->ContentStatus = "completed";
			    $lunObj->status = "lrn_crs_cmp_cmp";
			}
			        
			        
			        
			        
			expDebug::dPrint('get attn summary '.print_r($prev_suspend_data['progress'], 1), 2);
			expDebug::dPrint('current attn summary '.print_r($current_suspend_data['progress'], 1), 2);
		}
		
		// rawurlencode(json_encode($_SESSION[$video_session_id]))
		
		// return;
		if($objecttype == 'cre_sys_obt_crt' || $objecttype == 'cre_sys_obt_cur' || $objecttype == 'cre_sys_obt_trn'){
			$programDetails = getProgramDetails($enrollid);
			$lunObj->MasterId = $programDetails[0]->master_enrollment_id;
			$surQry_program  = db_select('slt_master_enrollment','mas_enroll');
			$surQry_program->addField('mas_enroll','program_id', 'program_id');
			$surQry_program->addField('mas_enroll','overall_status', 'comp_status');
			$surQry_program->condition('mas_enroll.id', $lunObj->MasterId,'=');
			$surQryRes_program = $surQry_program->execute()->fetchAssoc();
			expDebug::dPrint('survey status '. print_r($surQryRes_program,true),4);
			$entity_id = $surQryRes_program['program_id'];
			$prevSts = $surQryRes_program['comp_status'];
		}else if($objecttype == 'cre_sys_obt_cls'){
			$entity_id = $_REQUEST['classid'];
		}


		if($calltype == 'assessment'){
			$ContentStatus = $_REQUEST['contentstatus'];
			$surQry  = db_select('slt_survey_results','sur_res');
			$surQry->join('slt_survey_mapping', 'sur_map', 'sur_map.object_id = sur_res.entity_id');
			$surQry->join('slt_survey', 'sur', 'sur.id = sur_map.survey_id AND sur.id = sur_res.survey_id');
			$surQry->addField('sur_map','no_of_attempts', 'no_of_attempts');
			$surQry->addExpression('max(sur_res.no_of_attempts)', 'total_attempts');
			$surQry->addExpression('count(sur_res.id)', 'result_count');
			$surQry->condition('sur_res.entity_id', $entity_id,'=');
			$surQry->condition('sur_res.enrollment_id',$_REQUEST['enrollid'],'=');
			$surQry->condition('sur.type','sry_det_typ_ass','=');
			//$surQry->condition('sur_res.pre_status',$lunObj->prestatus,'=');
			$surQry->condition('sur_res.pre_status',0,'=');
			expDebug::dPrintDBAPI('defaultenrollquery1111', $surQry);
			//$surQry->condition('sur_res.survey_id',$lunObj->survey_id,'=');
			$surQryRes = $surQry->execute()->fetchAssoc();
			$no_of_attempts = $surQryRes['no_of_attempts'];
			$total_attempts = $surQryRes['total_attempts'];
			
			expdebug::dPrint('test status : ' . $_REQUEST['prestatus'] , 4);
			expdebug::dPrint('test status launch : ' . print_r($lunObj,1) , 4);
			//if($_REQUEST['prestatus'] == 0 || $objecttype == 'cre_sys_obt_cls' ){
			if((($_REQUEST['prestatus'] != '') && ($_REQUEST['prestatus'] == 0))  && (!empty($objecttype))){
				if($ContentStatus == 'failed' && $no_of_attempts == $total_attempts && $surQryRes['result_count'] > 0) {// failed
					$lunObj->Status = 'lrn_crs_cmp_inc';
				}else if($ContentStatus == 'Completed' && $surQryRes['result_count'] > 0){
					$lunObj->Status = 'lrn_crs_cmp_cmp';
				}else{
					$lunObj->Status = 'lrn_crs_cmp_enr';
				}
				$suspendData = json_decode(urldecode($lunObj->suspend_data));
				if ($lunObj->contentCode == 'sry_det_typ_sry') {
					$surveyStatus = ($_REQUEST["status"]!=null && $_REQUEST["status"]!='') ?stripApostrophe($_REQUEST["status"]):"null";
					if($lunObj->playfrom == 'contentplayer'){
						$lunObj->progress = $suspendData->pages->progress;
						$lunObj->suspend_data = ($lunObj->progress==100) ? null : serialize(urldecode($lunObj->suspend_data));
					} else {
						$lunObj->progress 		= '100';
						$lunObj->suspend_data 	= '';
					}
				} else {
					if($lunObj->playfrom == 'contentplayer'){
						$lunObj->progress = $suspendData->pages->progress;
						$lunObj->suspend_data = ($lunObj->progress==100) ? null : serialize(urldecode($lunObj->suspend_data));
					} else {
						$lunObj->progress 		= '100';
						$lunObj->suspend_data 	= '';
					}	
				}
			}else{
				$suspendData = json_decode(urldecode($lunObj->suspend_data));
				$lunObj->Status = ($prevSts == 'lrn_tpm_ovr_cmp') ? 'lrn_crs_cmp_cmp' : 'lrn_crs_cmp_enr';
				// Training plan section
				if ($lunObj->contentCode == 'sry_det_typ_sry') {
					$surveyStatus = ($_REQUEST["status"]!=null && $_REQUEST["status"]!='') ?stripApostrophe($_REQUEST["status"]):"null";
					if($lunObj->playfrom == 'contentplayer'){
						$lunObj->progress = $suspendData->pages->progress;
						$lunObj->suspend_data = ($lunObj->progress==100) ? null : serialize(urldecode($lunObj->suspend_data));
					} else {
						$lunObj->progress 		= '100';
						$lunObj->suspend_data 	= '';
					}	
					
				} else {
					if($lunObj->playfrom == 'contentplayer'){
						$lunObj->progress = $suspendData->pages->progress;
						$lunObj->suspend_data = ($lunObj->progress==100) ? null : serialize(urldecode($lunObj->suspend_data));
					} else {
						$lunObj->progress 		= '100';
						$lunObj->suspend_data 	= '';
					}	
				}
			}
			$lunObj->PreStatus = NULL;
			if($_REQUEST['prestatus'] == 1){
				$lunObj->PreStatus = ($_REQUEST["status"]!=null && $_REQUEST["status"]!='') ?stripApostrophe($_REQUEST["status"]):"null";
			}
			if($mylearn_version==0){
				$lunObj->contentCode = ($_REQUEST["contentCode"]!=null && $_REQUEST["contentCode"]!='') ?stripApostrophe($_REQUEST["contentCode"]):"sry_det_typ_ass";
			}
		}
		if($objecttype == 'cre_sys_obt_crt' || $objecttype == 'cre_sys_obt_cur' || $objecttype == 'cre_sys_obt_trn'){
			// $programDetails = getProgramDetails($enrollid);
			if($calltype == 'assessment'){
				$lunObj->RegId = $programDetails[0]->id;
				$lunObj->ClassId  = $programDetails[0]->class_id;
				$lunObj->CourseId = $programDetails[0]->course_id;
				$lunObj->LessonId = $programDetails[0]->master_enrollment_id;
				$lunObj->calltype ="'assesmentTP'";
				$result = attendanceInsert($lunObj);
				if($result->CompStatus == 'Completed') {
					$completeflag=TRUE;
					insertUserPointsForEachActionsPerformed ($lunObj->userId,'complete_class_training');
					sendCourseCompletionNotification($lunObj->LearnerId, $lunObj->ClassId);
				}
			}else{
				foreach($programDetails as $details){
					$lunObj->RegId = $details->id;
					$lunObj->ClassId  = $details->class_id;
					$lunObj->CourseId = $details->course_id;
					$lunObj->LessonId = $details->master_enrollment_id;
					$lunObj->Score = '0.00';
					$result = attendanceInsert($lunObj);
					if($result->CompStatus == 'Completed') {
						$completeflag=TRUE;
						insertUserPointsForEachActionsPerformed ($lunObj->userId,'complete_class_training');
						sendCourseCompletionNotification($lunObj->LearnerId, $lunObj->ClassId);
					}
				}
			}
		}else {
			$query = db_select('slt_enrollment', 'enr');
			$query->addField('enr','id');
			$query->addField('enr','user_id');
			$query->addField('enr','class_id');
			$query->addField('enr','comp_status');
			// below line commented for Mantis - SQL Injection #31297
			// $query->condition('enr.id', $lunObj->RegId);
			$query->where('enr.id=:regid',array(':regid'=>$lunObj->RegId));

			expDebug::dPrintDBAPI('defaultenrollquery', $query);
			$defaultEnrollStatus = $query->execute()->fetchAll();
			expDebug::dPrint('defaultenroll'.print_r($defaultEnrollStatus,true),4);
			$lunObj->calltype = $lunObj->calltype == '' ? '' : $lunObj->calltype;
			expDebug::dPrint('final launch object' .print_r($lunObj, 1) );
			$result = attendanceInsert($lunObj);
			if($result->CompStatus != "Not Changed"){
				$prevCompStatus = $defaultEnrollStatus[0]->comp_status;
				expDebug::dPrint('Comp status'.$result->CompStatus." Prev status".$prevCompStatus."courseid".$lunObj->CourseId."classid".$lunObj->ClassId,4);
				if($result->CompStatus == 'Completed' && ($prevCompStatus != 'lrn_crs_cmp_cmp')) {
					//commented for 50238: no user point entries captured for completing video class
					//$delType    = getClassDelType($lunObj->CourseId,$lunObj->ClassId);
					//$maxAttempt  = getMaxAttempts($lunObj->CourseId,$lunObj->ClassId,$lunObj->survey_id);
					//if($delType!='lrn_cls_dty_vod' || ($delType=='lrn_cls_dty_vod' && $maxAttempt==1)){
						insertUserPointsForEachActionsPerformed ($lunObj->userId,'complete_class_training');
						sendCourseCompletionNotification($lunObj->LearnerId, $lunObj->ClassId, $defaultEnrollStatus[0]->id);
					//}
					//insert audit trail
					$insertaudit=array(
	    					'id'  =>$lunObj->RegId,
	                        'user_id' =>$lunObj->userId,                                                   
	                        'class_id'=>str_replace("'","",$lunObj->ClassId),
	                        'comp_status' =>'lrn_crs_cmp_cmp',					          
					);
					expDebug::dPrint('audit for clscomplete'.$lunObj->userId,4);
					make_audit_trail_entries($insertaudit, $lunObj->userId ,$defaultEnrollStatus,str_replace("'","",$lunObj->ClassId),'completed','cre_sys_obt_cls','exp_sp_launch','classcompleted',
					array('id','user_id', 'class_id', 'comp_status'), $lunObj->userId);
				}
				expDebug::dPrint('compstatus = ' . $result->CompStatus , 4);
				if($result->CompStatus == 'Incomplete'){
					$insertauditin=array(
	      					'id'  =>$lunObj->RegId,
	                          'user_id' =>$lunObj->userId,                                                   
	                          'class_id'=>str_replace("'","",$lunObj->ClassId),
	                          'comp_status' =>'lrn_crs_cmp_inc',					          
					);
					make_audit_trail_entries($insertauditin, $lunObj->userId , $defaultEnrollStatus,str_replace("'","",$lunObj->ClassId),'incomplete','cre_sys_obt_cls','exp_sp_launch','classincomplete',
					array('id','user_id', 'class_id', 'comp_status'),$lunObj->userId);
						
				}
				if($calltype != 'assessment'){
					updateOverallPercentage($enrollid, $lunObj->Score, $objecttype);
				}
			}
		}
		//73059: Content completion % is not getting updated unless we close the window and reopen it or move to the next content
		if(($lunObj->launchflag==1 || ($lunObj->launchflag==0 && $calltype == 'assessment')) && !isset($_REQUEST['mobile_request']) ){
			$Current_progress = 0;
			if($calltype == 'assessment')
				$attsummaryDetails = get_attendance_summary($lunObj->RegId, $lunObj->CourseId, $lunObj->ClassId, $lunObj->LessonId, $lunObj->VersionId, $lunObj->survey_id);
			else
				$attsummaryDetails = get_attendance_summary($lunObj->RegId, $lunObj->CourseId, $lunObj->ClassId, $lunObj->LessonId, $lunObj->VersionId, NULL);
			if($attsummaryDetails->progress>0)
				$Current_progress = $attsummaryDetails->progress;
			$lunObj->progress = convertProgressValue($Current_progress);
			if($calltype == 'assessment')
				$lunObj->content_id = $lunObj->survey_id;
			else
				$lunObj->content_id = getContentMasterId($lunObj->VersionId);
			$lunObj->tot_progress = convertProgressValue(getMultiLessonDetails($lunObj->VersionId,$lunObj));
			if($objecttype == 'cre_sys_obt_crt' || $objecttype == 'cre_sys_obt_cur' || $objecttype == 'cre_sys_obt_trn'){
				$lunObj->launchFrom ='TP';
			}
			$launchInfo =$lunObj;
		}else{
			$launchInfo = getLaunchDetail($lunObj->RegId);
			$launchInfo[0]['CompMessage']='';
			if($result->CompMessage == 'Attempts Over'){
				$launchInfo[0]['CompMessage']="Attempts Over";	
			}
		}
		
		if($_REQUEST['mobile_request']){
			
			$user_detail=db_query("select progress from slt_enrollment where id=".$lunObj->RegId);
			expDebug::dPrintDBAPI("query12345rchangese3edede ", $user_detail);
			$userDetail = $user_detail->fetchAll();
			$launchInfo[0]['overallProgress'] = $userDetail[0]->progress;
			
		}
		
		
		syncSolrData('MasterEnrollment');
		expDebug::dPrint("launchurlllllll1212121".print_r($launchInfo,true),5);
		
		return $launchInfo;
	}catch (Exception $ex) {
		// $txn->rollback();
		//unset($txn);
		watchdog_exception('updateLaunchWrapper', $ex);
		expertusErrorThrow($ex);
	}
	// Commit the transaction
	unset($txn);
}

/*
 * To insert the attendance details of the learner
 */
function attendanceInsert($lunObj){
	try{
		expDebug::dPrint("all the values in atten".print_r($lunObj,true),4);
		global $tincan;
		if(empty($lunObj->calltype) || $lunObj->calltype == 'null' || $lunObj->calltype == "'assessment'" || $lunObj->calltype == null){
			$query = db_select('slt_enrollment', 'enr');
			$query->addField('enr','id');
			$query->addField('enr','user_id');
			$query->addField('enr','class_id');
			$query->addField('enr','comp_status');
			$query->where('enr.id=:regid',array(':regid'=>$lunObj->RegId));
		}else if($lunObj->calltype == "'assesmentTP'"){
			$query = db_select('slt_master_enrollment', 'enr');
			$query->addField('enr','id');
			$query->addField('enr','user_id');
			$query->addField('enr','program_id');
			$query->addField('enr','overall_status','comp_status');
			$query->where('enr.id=:regid',array(':regid'=>$lunObj->RegId));
		}
		//expDebug::dPrint($lunObj->calltype,4);
		expDebug::dPrintDBAPI('defaultenrollquery', $query);
		$defaultEnrollStatus = $query->execute()->fetchAll();
		expDebug::dPrint('defaultenroll'.print_r($defaultEnrollStatus,true),4);
		$prevCompStatus = $defaultEnrollStatus[0]->comp_status;
		expDebug::dPrint('defaultenroll dsd sd'.print_r($prevCompStatus,true),4);

		//Getting LRS enabled settings
		$lrs = getConfigValue('lrs_enable');
		$tincan = getConfigValue('tincanapi');
	
		$lrsEnabled = ($tincan == 1 && $lrs == 1) ? 'Y' : 'N';
		expDebug::dPrint('LRS enabled value'.$lrsEnabled,4);
		
		//$txn = db_transaction(); //Commented for dbStatementsExecution();
		//completion_status of scorm 2004 content is sent to procedure
		expDebug::dPrint('content completion status' . $lunObj->CompletionStatus, 4);
		expDebug::dPrint('content lesson status' . $lunObj->ContentStatus, 4);
		expDebug::dPrint('content content enroll status' . $lunObj->Status, 4);
		expDebug::dPrint('content content calltype' . $lunObj->calltype, 4);
		
		expDebug::dPrint("TESTING ",4);
		$api_name = isset($_REQUEST['apiname']) ? $_REQUEST['apiname'] : '';
		if($api_name != 'UpdateScoreAPI'){
			if($lunObj->contentType == 'SCORM' && $lunObj->calltype == 'null'){
				include_once($_SERVER["DOCUMENT_ROOT"] . '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrenrollment/exp_sp_scorm_utils.php');
				if($lunObj->contentVersion == '1.2') {
					$lunObj->calltype = "'SCORM_1.2'";
					// SCORM 1.2 will have session_time like 0000:00:43.44
					$tmeSpt = ConvertCMITimeSpanToMS($lunObj->SessionTime) / 1000;
					$totTime = gmdate("H:i:s", $tmeSpt);
				} else {
					$lunObj->calltype = "'SCORM_2004'";
					// SCORM 2004 will have session_time like PT1M17.76S
					$tmeSpt = ConvertScorm2004TimeToMS($lunObj->SessionTime) / 1000;
					$totTime = gmdate("H:i:s", $tmeSpt);
				}
			} else {
				$totTime = $lunObj->SessionTime;
			}
		} else {
			// time spent from mobile is expected to be in 00:00:43 format
			include_once($_SERVER["DOCUMENT_ROOT"] . '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrenrollment/exp_sp_scorm_utils.php');
			$tmeSpt = ConvertCMITimeSpanToMS($lunObj->SessionTime) / 1000;
			$totTime = gmdate("H:i:s", $tmeSpt);
		}
		
		
		expDebug::dPrint('launch content type = '.$lunObj->contentType, 4);
		expDebug::dPrint('launch content version = '.$lunObj->contentVersion,4);
		expDebug::dPrint('launch session time = '.$totTime, 4);
		$lunObj->launchflag=escape_string($_REQUEST['launchflag']);
		expDebug::dPrint('launch Contentlaunchflag = '.$lunObj->launchflag, 4);
		
		if($_REQUEST['apiname'] == 'UpdateScoreAPI'){
			if(urldecode($lunObj->contentType) == 'SCORM')
			{
				$lunObj->calltype ="'SCORM_1.2'";
			}
			else if(urldecode($lunObj->contentType) == 'SCORM 2004')
			{
				$lunObj->calltype ="'SCORM_2004'";
			}
		}
		// expDebug::dPrint('$_REQUEST params on attendance insert '.print_r($_REQUEST, 1));
		expDebug::dPrint('$_REQUEST params on attendance insert is_scorm_api_changed = '.var_export($_REQUEST['is_scorm_api_changed'], 1), 5);
		if(($lunObj->calltype == "'SCORM_2004'" || $lunObj->calltype == "'SCORM_1.2'") && $_REQUEST['is_scorm_api_changed'] == 'false') {
			//skip all the attendance entries, user points, audit trails if the scorm apis are same as initialized
			$result = new stdClass();
			$result->CompStatus = "Not Changed";
			$result->Id = 0;
			return $result;
		}
		/*$query = "call slp_attendance_details_ins(".$lunObj->RegId.",'".$lunObj->LearnerId."','".$lunObj->CourseId."','".$lunObj->ClassId."','".$lunObj->LessonId."','".$lunObj->VersionId."','".$lunObj->Status."','".$lunObj->PreStatus."','".$lunObj->Score."','".$lunObj->Location."','".$lunObj->ContentStatus."','".$lunObj->CompletionStatus."','".$lunObj->userId."','".$lunObj->launch_data."','".$lunObj->suspend_data."','".$lunObj->exit."','".$lunObj->launchflag."',null,$lunObj->calltype,$lunObj->assessCheck,$lunObj->survey_id,'".$lrsEnabled."','".$totTime."',@insval,@compstatus)";
		$query = "call slp_attendance_details_ins(".$lunObj->RegId.",'".$lunObj->LearnerId."','".$lunObj->CourseId."','".$lunObj->ClassId."','".$lunObj->LessonId."','".$lunObj->VersionId."','".$lunObj->Status."','".$lunObj->PreStatus."','".$lunObj->Score."','".$lunObj->Location."','".$lunObj->ContentStatus."','".$lunObj->CompletionStatus."','".$lunObj->userId."','".$lunObj->launch_data."','".$lunObj->suspend_data."','".$lunObj->exit."',null,$lunObj->calltype,$lunObj->assessCheck,$lunObj->survey_id,'".$lrsEnabled."','".$totTime."','".$lunObj->contentCode."','".$lunObj->progress."',@insval,@compstatus)";
		//$query = "call slp_attendance_details_ins($lunObj->RegId,$lunObj->LearnerId,$lunObj->CourseId,$lunObj->ClassId,$lunObj->LessonId,$lunObj->VersionId,$lunObj->Status,$lunObj->SessionTime,$lunObj->Score,$lunObj->Grade,$lunObj->Location,$lunObj->ContentStatus,$lunObj->userId,$lunObj->launch_data,$lunObj->suspend_data,$lunObj->exit,null,$lunObj->calltype,@insval,@compstatus)";
		$dao = new AbstractDAO();
		$dao->connect();
		$dao->beginTrans();
		expDebug::dPrint(' SQL query = ' . $query , 4);
		$dao->execute($query);
		$dao->commitTrans();
		$dao->query("select @insval Id,@compstatus CompStatus");
		$result = $dao->fetchResult();
		expDebug::dPrint(' Final result = ' . print_r($result, true) , 4);
		$dao->closeconnect();*/
		if(strtolower($lunObj->ContentStatus) != 'not attempted'){
			if($lunObj->survey_id == 0){
				
				//Get max_attempts
				$selectStmt = db_select('slt_enrollment_content_mapping', 'emap');
				$selectStmt->leftJoin('slt_content_version', 'cv', 'cv.id = emap.version_id');
				$selectStmt->leftJoin('slt_content_lesson', 'cl', 'cv.id = cl.content_version_id');
				$selectStmt->leftJoin('slt_course_content_mapper', 'ccm', 'ccm.content_id=cv.content_master_id');
				//$selectStmt->addExpression('DISTINCT(cl.id)','lessonid');
				//$selectStmt->addExpression('ccm.content_id','content_id');
				$selectStmt->addExpression('ccm.max_attempts','max_attempts');
				$selectStmt->condition('ccm.class_id',$lunObj->ClassId,'=');
				$selectStmt->condition('emap.enroll_id', $lunObj->RegId,'=');
				$selectStmt->condition('cl.id', $lunObj->LessonId,'=');
				$selectStmt->condition('cv.id', $lunObj->VersionId,'=');
				/* $selectStmt->orderBy('ccm.content_id');
				$selectStmt->orderBy('cl.id'); */
				expDebug::DPrintDBAPI("content max attempt() test ",$selectStmt);
				$lesson_attempts = $selectStmt->execute()->fetchCol();
				
				if($lesson_attempts[0]!==null && $lesson_attempts[0]!=0){
					$attempts = getAttemptNumber($lunObj->LearnerId,$lunObj->CourseId,$lunObj->ClassId,$lunObj->LessonId,$lunObj->VersionId,$lunObj->RegId);
					
					
					$selectStmtCustom = db_select('slt_attendance_details', 'det');
					$selectStmtCustom->addExpression('COUNT(1)','att_cnt');
					$selectStmtCustom->addField('det','id','id');
					$selectStmtCustom->condition('det.user_id',$lunObj->LearnerId,'=');
					$selectStmtCustom->condition('det.course_id', $lunObj->CourseId,'=');
					$selectStmtCustom->condition('det.class_id',$lunObj->ClassId,'=');
					$selectStmtCustom->condition('det.content_version_id',$lunObj->VersionId,'=');
					$selectStmtCustom->condition('det.lesson_id',$lunObj->LessonId,'=');
					$selectStmtCustom->condition('det.enrollment_id',$lunObj->RegId,'=');
					$selectStmtCustom->condition('det.custom2',0,'!=');
					expDebug::dPrintDBAPI("launch flag count ",$selectStmtCustom);
					$att_res = $selectStmtCustom->execute()->fetchAll();
					$attendId = $att_res[0]->id;
					$custom = $att_res[0]->att_cnt;
					
					
					
					if($lesson_attempts[0]<$attempts && $custom==0){
						//skip all the attendance entries, user points
						$result = new stdClass();
						$result->CompStatus = "Not Changed";
						$result->CompMessage = "Attempts Over";
						$result->Id = 0;
						return $result;
					}
				}
				
				
				expDebug::dPrint(' Max attempts for all the lessons= ' . print_r($lesson_attempts, true) , 4);
				expDebug::dPrint(' current attempt for all the lessons= ' . print_r($attempts, true) , 4);
			}
		}
		$statements = array('statement' => true);
		// Passing additional parameter $lunObj->type as 'h5p' and $statements for final executions
		$results = lp_attendance_details_ins($lunObj->RegId,$lunObj->LearnerId,$lunObj->CourseId,$lunObj->ClassId,$lunObj->LessonId,$lunObj->VersionId,$lunObj->Status,$lunObj->PreStatus,$lunObj->Score,$lunObj->Location,$lunObj->ContentStatus,$lunObj->CompletionStatus,$lunObj->userId,$lunObj->launch_data,$lunObj->suspend_data,$lunObj->exit,$lunObj->launchflag,'',$lunObj->calltype,$lunObj->assessCheck,$lunObj->survey_id,$lrsEnabled,$totTime,$lunObj->contentCode,$lunObj->progress, $lunObj->type, $statements); 
		expDebug::dPrint(' Final attendance details $statements= ' . print_r($statements, true) , 4);
		dbStatementsExecution($statements);
		expDebug::dPrint(' Final result attendance details= ' . print_r($results, true) , 4);
		
		$selectStmt = db_select('slt_enrollment', 'enr');
		$selectStmt->addField('enr','comp_status','comp_status');
		$selectStmt->condition('enr.id',$lunObj->RegId,'=');
		$compstaft = $selectStmt->execute()->fetchField();
		
		if($compstaft == 'lrn_crs_cmp_cmp'){
			$contentstatus = $compstaft;
		}
		
		$updsts = ($prevCompStatus == $compstaft) ? "Not Changed" : getProfileListItemName($contentstatus);
		$result = new stdClass();
		$result->Id = $results->Id;
		$result->CompStatus = $updsts;
		
		$assessCheck = $lunObj->assessCheck;
		if($tincan == 1 && (empty($assessCheck) || $assessCheck == 0)){
			$lunObj->time_spent = $totTime;
			tincanCreateStatement($lunObj,$prevCompStatus);
		}
		syncSolrData('MasterEnrollment');
		return $result;
	}catch (Exception $ex) {
		//$txn->rollback();
		//unset($txn);
		watchdog_exception('updateLaunchWrapper', $ex);
		expertusErrorThrow($ex);
	}

}
function getUserDetails($userId){
	try {
		$select = db_select('slt_person', 'per');
		$select->addExpression('CONCAT(per.first_name,\' \',per.last_name)','user_firstlast_name');
		$select->addField('per','email','email');
		$select->condition('per.id',$userId);
		expDebug::dPrintDBAPI("get Person details Query", $select);
		// Execute the query
		$result = $select->execute()->fetchAll();
		return $result;
	} catch (Exception $ex) {
		watchdog_exception('getUserDetails', $ex);
		expertusErrorThrow($ex);
	}
}
function getAssessmentCount($classID,$type){
	try {
		$select = db_select('slt_survey_mapping', 'surmap');
		$select->addExpression('COUNT(1)','assessment_count');
		$select->condition('surmap.object_id',$classID);
		$select->condition('surmap.object_type',$type);
		$select->condition('surmap.pre_status',0);
		expDebug::dPrintDBAPI("getAssessmentCount Query", $select);
		$result = $select->execute()->fetchField();
		return $result;
	} catch (Exception $ex) {
		watchdog_exception('getAssessmentCount', $ex);
		expertusErrorThrow($ex);
	}
}
function tincanCreateStatement($obj,$prevCompStatus){
	try{
		global $base_url;
		expDebug::dPrint("tincan staemnt create statement" . print_r($obj, true), 4);
		require_once ('sites/all/libraries/tincan_php_library/tincan_client.php');
		if((isset($obj->launchflag) && $obj->launchflag == 1) || ($obj->contentCode == 'sry_det_typ_sry') 
				|| ($obj->contentCode == 'sry_det_typ_ass' && $obj->ContentStatus == 'suspend')){ // skip content refresh calls or survey or assessment suspend mode
			return;
		}
		require_once ('sites/all/libraries/tincan_php_library/tincan_client.php');
		$api_client = TinCanClient::Instance();
		$personDetail = getUserDetails($obj->userId);
		expDebug::dPrint("Previous Completed Status " . print_r($prevCompStatus, true), 5);
		$fullName = $personDetail[0]->user_firstlast_name;
		$email = $personDetail[0]->email;
		$calltype = $obj->calltype;
		expDebug::dPrint("call type " . print_r($calltype, true), 5);
		$Status = $obj->Status;
// 		$content_id = db_query("SELECT content_master_id FROM slt_content_version  where id =:mid", array(':mid' => $obj->VersionId))->fetchField();
		
		if ($calltype == "'assessment'" || $calltype == "'assesmentTP'") {
			$getContentInfo = db_select('slt_survey', 'sur');
			$getContentInfo->condition('id', $obj->survey_id);
			$getContentInfo->addField('sur', 'id', 'content_id');
			$getContentInfo->addField('sur', 'code', 'content_code');
			expDebug::dPrintDBAPI('get assessment infor', $getContentInfo);
			$content_info = $getContentInfo->execute()->fetch();
			$content_id = empty($content_info->content_id) ? 0 :  $content_info->content_id;
			$content_code = empty($content_info->content_code) ? 0 : $content_info->content_code;
		} else {
			$getContentInfo = db_select('slt_content_version', 'ver');
			$getContentInfo->innerJoin('slt_content_master', 'mst', 'mst.id=ver.content_master_id');
			$getContentInfo->condition('ver.id', $obj->VersionId);
			$getContentInfo->addField('mst', 'id', 'content_id');
			$getContentInfo->addField('mst', 'code', 'content_code');
			expDebug::dPrintDBAPI('get content infor', $getContentInfo);
			$content_info = $getContentInfo->execute()->fetch();
			expDebug::dPrint('get content info result '.var_export($content_info, 1), 5);
			expDebug::dPrint("contente id :--> " . $content_id, 5);
			$content_id = empty($content_info->content_id) ? 0 : $content_info->content_id;  
			$content_code = empty($content_info->content_code) ? 0 : $content_info->content_code;
		}
		
		
		if($calltype == 'null' || $calltype == "'assessment'" || $calltype == null || $calltype == 'null' || $calltype == "'SCORM_2004'" || $calltype == "'SCORM_1.2'"){
			$getAssessmentCount = getAssessmentCount($obj->ClassId,'cre_sys_obt_cls');
			$query = db_select('slt_course_class', 'cls');
			$query->leftJoin('slt_course_template', 'crs', 'crs.id=cls.course_id');
			if($obj->assessCheck == '0'){
				$query->leftJoin('slt_survey_mapping', 'sur', 'sur.object_id = cls.id and sur.pre_status=0');
				$query->addField('sur','no_of_attempts','no_of_attempts');
				$summarycheck = " and summ.assessment_id = sur.survey_id";
			}else{
				$summarycheck = "";
			}
			$query->leftJoin('slt_enrollment', 'enr', 'enr.class_id=cls.id and enr.course_id=cls.course_id and enr.user_id='.$obj->userId);
			$query->leftJoin('slt_course_content_mapper', 'map', 'map.class_id=cls.id and map.course_id=crs.id and map.content_id ='.$content_id);
			// Joined slt_content_version table for mapping content id in slt_course_content_mapper table
			$query->leftJoin('slt_content_version', 'vers', 'vers.content_master_id = map.content_id and vers.id = '.$obj->VersionId);
			$query->leftJoin('slt_attendance_summary', 'summ', 'summ.content_version_id = '.$obj->VersionId.' AND summ.class_id=cls.id and summ.course_id=crs.id '.$summarycheck);
			$query->leftJoin('slt_profile_list_items', 'pro', 'pro.code=enr.comp_status');

			$query->addField('cls','title','class_name');
			$query->addField('crs','title','course_name');
			$query->addField('map','max_attempts','max_attempts');
			$query->addField('map','validity_days','validity_days');
			//$query->addField('summ','total_attempts','total_attempts');
			$query->addExpression('SUM(summ.total_attempts)', 'total_attempts');
			$query->addField('summ','last_lesson_location','last_lesson_location');
			$query->addField('summ','suspend_data','suspend_data');
			//$query->addField('summ','score','Score');
			//	$query->addExpression('SUM(summ.score)', 'Score');
			$query->addField('enr','score','Score');
			$query->addField('pro','name','statusName');
			$query->addField('enr','comp_status','comp_status');
			$query->addField('vers','version','version_no');
			//$query->addExpression("TIME_TO_SEC(summ.total_time_spend)", 'total_time_spend');
			$query->addExpression("DATE_FORMAT((summ.last_attempt_date),'%d %b %Y %h:%i %p')", 'last_attempt_date');
			$query->condition('cls.id', $obj->ClassId, '=');
			$query->condition('summ.user_id', $obj->userId, '=');
			$query->condition('enr.reg_status', 'lrn_crs_reg_cnf', '=');
			//$query->condition('summ.lesson_id', $obj->LessonId, '=');
			$query->condition('summ.content_version_id', $obj->VersionId, '=');
			$query->groupBy('cls.id');
			if($obj->assessCheck == '0'){
				$query->groupBy("cls.title");
			}
			
			expDebug::dPrintDBAPI("Query for Survey or assessment",$query);
			$result = $query->execute()->fetchAll();
			end($result);
			$key = key($result);
			expDebug::dPrint("key ".$key);
			$courseName = $result[$key]->course_name;
			$classname = $result[$key]->class_name;
			if($obj->assessCheck == '0'){
				$max_attempts = $result[$key]->no_of_attempts;
			}else{
				$max_attempts = $result[$key]->max_attempts;
			}
			$validity_days = $result[$key]->validity_days;
			if($validity_days == 1){
				$days = 'day';
			}else{
				$days = 'days';
			}

			$total_attempts = $result[$key]->total_attempts;
			if($total_attempts == 1){
				$totalAttempts = $result[$key]->total_attempts."st";
			}elseif($total_attempts == 2){
				$totalAttempts = $result[$key]->total_attempts."nd";
			}elseif($total_attempts == 3){
				$totalAttempts = $result[$key]->total_attempts."rd";
			}else{
				$totalAttempts = $result[$key]->total_attempts."th";
			}
			if($max_attempts > 0){
				$attempt_left = $max_attempts - $total_attempts;
			}else{
				$attempt_left = 0;
			}
			$last_lesson_location = $result[$key]->last_lesson_location;
			$suspend_data = $result[$key]->suspend_data;
			expDebug::dPrint("lession location last ".print_r($last_lesson_location,true),5);
			expDebug::dPrint("susend data ".print_r($suspend_data,true),5);
			if($last_lesson_location != 'null'){
				$last_lesson_location = $result[$key]->last_lesson_location;
			}elseif($last_lesson_location == 'null' && $suspend_data != 'null'){
				expDebug::dPrint("susend data ".print_r($suspend_data,true),5);
				$suspend_dataArray =  $suspend_data;
				$exploe = explode("|",$suspend_dataArray);
				$string = explode("=",$exploe[1]);
				$last_lesson_location = $string[1];
				expDebug::dPrint("susend last_lesson_location ".print_r($last_lesson_location,true),5);
			}

			$last_attempt_date = $result[$key]->last_attempt_date;
			//$total_time_spend = $result[$key]->total_time_spend;
			$SessionTime = $obj->SessionTime;
			$parts = explode('.', $SessionTime);
			$total_time_spend = $parts[0]*60 + $parts[1];

			$explode = explode(" ", $last_attempt_date);
			$toDate = $explode[3]." ".$explode[4];

			$newtimestamp = strtotime("$last_attempt_date - $total_time_spend second");
			$FromTime =  date('jS M Y h:i A', $newtimestamp);
			expDebug::dPrint("from time".print_r($FromTime,true),5);
			expDebug::dPrint("to time".print_r($toDate,true),5);
			$statusName = $result[$key]->statusname;
			expDebug::dPrint('course class score: '.print_r($score, true) , 3);
			if($getAssessmentCount > 0 && $calltype == "'assessment'"){
				$score = $result[$key]->score;
			}else if($getAssessmentCount > 0 && ($calltype == null || $calltype == 'null' || $calltype == "'SCORM_2004'" || $calltype == "'SCORM_1.2'")){
				$score = '';
			}else if($getAssessmentCount == 0 && ($calltype == null || $calltype == 'null' || $calltype == "'SCORM_2004'" || $calltype == "'SCORM_1.2'")){
				$score = $result[$key]->score;
			}

			expDebug::dPrintDBAPI("query for tincan test", $query);
			expDebug::dPrint('course class information: '.print_r($result, true) , 3);
			expDebug::dPrint('course class validity_days: '.print_r($result[0]->validity_days, true) , 5);
			expDebug::dPrint('course class fullname: '.print_r($fullName, true) , 3);
			if($attempt_left == 1 || $max_attempts == 1){
				$attempt = 'attempt';
			}else{
				$attempt = 'attempts';
			}
			
			$versionNo = $result[$key]->version_no;

		}else if($calltype == "'assesmentTP'"){
			$query = db_select('slt_program', 'prm');
			$query->leftJoin('slt_master_enrollment', 'master_enr', 'master_enr.program_id=prm.id');
			$query->leftJoin('slt_enrollment', 'enr', 'enr.master_enrollment_id=master_enr.id');
			$query->leftJoin('slt_survey_results', 'surres', 'surres.enrollment_id=master_enr.id and surres.entity_id=master_enr.program_id');
			$query->leftJoin('slt_profile_list_items', 'pro', 'pro.code=master_enr.overall_status');
			if($obj->assessCheck == '0'){
				$query->leftJoin('slt_survey_mapping', 'sur', 'sur.object_id = prm.id and sur.pre_status=0');
				$query->addField('sur','no_of_attempts','max_attempts');
			}
			$query->addField('prm','title','class_name');
			$query->addField('sur','no_of_attempts','max_attempts');
			$query->addField('surres','no_of_attempts','total_attempts');
			$query->addField('master_enr','score','Score');
			$query->addField('pro','name','statusName');
			$query->addField('master_enr','overall_status','comp_status');

			$query->condition('master_enr.id', $obj->MasterId, '=');
			$query->condition('surres.survey_id', $obj->survey_id, '=');
			$query->condition('master_enr.user_id', $obj->userId, '=');
			$query->groupBy('prm.id');
			if($obj->assessCheck == '0'){
				$query->groupBy("prm.title");
			}
			expDebug::dPrintDBAPI("Query for tincanapi Traning Program ", $query);
			$result = $query->execute()->fetchAll();
			expDebug::dPrint("Query result for tincanapi Traning Program ".print_r($result,true),5);
			end($result);
			$key = key($result);
			$classname = $result[$key]->class_name;
			if($obj->assessCheck == '0'){
				$max_attempts = $result[$key]->max_attempts;
			}else{
				$max_attempts = $result[$key]->max_attempts;
			}

			$total_attempts = $result[$key]->total_attempts;
			if($total_attempts == 1){
				$totalAttempts = $result[$key]->total_attempts."st";
			}elseif($total_attempts == 2){
				$totalAttempts = $result[$key]->total_attempts."nd";
			}elseif($total_attempts == 3){
				$totalAttempts = $result[$key]->total_attempts."rd";
			}else{
				$totalAttempts = $result[$key]->total_attempts."th";
			}

			$statusName = $result[$key]->statusname;
			$score = $result[$key]->score;

			if($max_attempts > 0){
				$attempt_left = $max_attempts - $total_attempts;
			}else{
				$attempt_left = 0;
			}

			if($attempt_left == 1 || $max_attempts == 1){
				$attempt = 'attempt';
			}else{
				$attempt = 'attempts';
			}

		}
		$dateNow = new DateTime();
		$dateNow->format('u');
		$stored = $dateNow->format('Y-m-d\TH:i:s.u\Z');
// 		$duration = $obj->ti
		expDebug::dPrint('launch params '.print_r($obj, 1), 5);
		//$actor will be commnon for all the statements
		$actor = array('name' => "$fullName",
						'objectType' => 'Agent',
						'mbox' => "mailto:" . $email
						);
		
		
		/* $object = array('objectType' => 'Activity',
						'id' => 'http://Effective11_identifier',	//Question name/ Slide name
						'definition' => array(
											'name' => array('en-US' => 'Effective11_title'),
											'description' => array('en-US' => 'Course Description.')
											)
						); */
		$statuses = array('completed', 'passed'); // failed
		$content_status = strtolower(trim($obj->ContentStatus));
		$completion_status = strtolower(trim($obj->CompletionStatus));
		$sidval = "-".$obj->LessonId."-".$obj->VersionId."-".$obj->RegId."-".$obj->Location;
		expDebug::dPrint('aicc id'. $sidval, 5);
		$enc1 = new Encrypt();
		$aicc_id = $enc1->encrypt($sidval);
		expDebug::dPrint('aicc id ency'. $sidval);
		
		$attachResults = true; // 77518 - Fix for Knowledge content and VOD
		if ($obj->contentCode == 'lrn_cnt_typ_knc' || $obj->contentCode == 'lrn_cnt_typ_vod') {
			$attachResults = false;
		} 
		expDebug::dPrint('skip results values' . var_export($attachResults,1), 4);
		/*
		 * Used to capture result details of the content
		 * @username attempted @course_name with a score for @score_value
		 */
		$completeStatus = getContentCompletionStatus($obj, $completion_status);
		$content_result = array(
				'duration' => $obj->time_spent,	//this need to be changed after checking with LRS
				'success' => $completeStatus['contentStatus'],	//true/false
				'completion' => $completeStatus['completionStatus'], //in_array($completion_status, $statuses),//true
				'score' => array(
						//'max' => 100,	//maximum score of the content if any
						//'min' => 0,		//maximum score of the content if any
						'raw' => $obj->Score,	//the score achieved by the learner in deimal number
						//"scaled" => 0.95		//the score achieved by the learner in percentage
				),
		);
		// additional info, optional , shoule be sent on completion
		$context = array(
				'course_name' => $classname,			//mandatory param, refers to Class Name/Programe Name in ExpertusONE
				'course_id' => $obj->ClassId,			//mandatory param, refers to Class Id in ExpertusONE
				'content_id' => ($calltype == "'assessment'" || $calltype == "'assesmentTP'") ? 'q-' . $content_id : $content_id,//mandatory param, refers to Content Id in ExpertusONE // assessment id as q-number otherwise number
				'content_name' => $content_code,
				'content_completed' => $completeStatus['contentStatus'], //in_array($content_status, $statuses),	//optional , shoule be sent on completion
				'course_completed' => in_array($result[$key]->comp_status, array('lrn_tpm_ovr_cmp', 'lrn_crs_cmp_cmp')),
				'registration' => $aicc_id,	//aicc id
		);
		
		/*
		 * Used to capture status details of the course
		 * @username attempted @course_name with a score for @score_value
		 */
		$statusDetails = array('actor' => $actor,
								//'result' => $content_result,
								'context' => $context,
								'verb' => array('id' => 'http://adlnet.gov/expapi/verbs/' . $statusName,
												'display' => array("en-US" => "$statusName")
												), 
								'object' => array('type' => 'course',
												'definition' => array('name' => array('en-US' => "$classname"))
												),
								'timestamp' => $stored,
								'stored' => $stored
						);
		
		/*
		 * Used to capture score details of the content
		 * @username attempted @course_name with a score for @score_value
		 */
		$scoreDetails = array('actor' => $actor,
								//'result' => $content_result,
								'context' => $context,
								'verb' => array('id' => 'http://adlnet.gov/expapi/verbs/scored', 'display' => array("en-US" => "scored")),
								'object' => array('type' => 'course', 'definition' => array('name' => array('en-US' => "$score for $classname"))),
								'timestamp' => $stored,
								'stored' => $stored
						);
		/*
		 * Used to capture validity details of the content
		 * statement: @username attempted '@course_name and their Validity is @validity_days days' (with a score of @score_value)
		 */
		$validityDetails = array('actor' => $actor,
								//'result' => $content_result,
								'context' => $context,
								'verb' => array('id' => 'http://adlnet.gov/expapi/verbs/attempted', 'display' => array("en-US" => "attempted")), 
								'object' => array('type' => 'course', 'definition' => array('name' => array('en-US' => "$classname and their Validity is $validity_days $days"))),
								'timestamp' => $stored,
								'stored' => $stored
						);
		/*
		 * Used to capture bookmark details of the content
		 * statement: @username bookmarked '@coursename. The book marked page is @last_location' (with a score of @score_value)
		 */
		$bookDetails = array('actor' => $actor,
							//'result' => $content_result,
							'context' => $context,
							'verb' => array('id' => 'http://adlnet.gov/expapi/verbs/bookmarked', 'display' => array("en-US" => "bookmarked")), 
							'object' => array('type' => 'course', 'definition' => array('name' => array('en-US' => "$classname. The book marked page is $last_lesson_location"))),
							'timestamp' => $stored,
							'stored' => $stored
					);
		
		/*
		 * Used to capture launch durarion details of the content
		 * statement: @username attempted '@course_name and their launch period is from @timestamp' (with a score of @score_value)
		 */
		$launchDateTime = array('actor' => $actor,
								//'result' => $content_result,
								'context' => $context,
								'verb' => array('id' => 'http://adlnet.gov/expapi/verbs/attempted', 'display' => array("en-US" => "attempted")), 
								'object' => array('type' => 'course', 'definition' => array('name' => array('en-US' => "$classname and their launch period is from $FromTime to $toDate"))),
								'timestamp' => $stored,
								'stored' => $stored
						);
		
		/*
		 * Used to capture attempt details of the content
		 * statement: @username attempted '@course_name the @attempt_number time out of their @total_attempts attempts' (with a score of @score_value)
		 */
		$track = array('actor' => $actor,
						//'result' => $content_result,
						'context' => $context,
						'verb' => array('id' => 'http://adlnet.gov/expapi/verbs/attempted', 'display' => array("en-US" => "attempted")), 
						'object' => array('type' => 'course', 'definition' => array('name' => array('en-US' => "$classname the $totalAttempts time out of their $max_attempts $attempt"))),
						'timestamp' => $stored,
						'stored' => $stored
				);
		/*
		 * Used to capture attempt left details of the content
		 * statement: @username attempted '@course_name and has @attempt_left attempts left' (with a score of @score_value)
		 */
		$launchDetails = array('actor' => $actor,
								//'result' => $content_result,
								'context' => $context,
								'verb' => array('id' => 'http://adlnet.gov/expapi/verbs/attempted', 'display' => array("en-US" => "attempted")), 
								'object' => array('type' => 'course', 'definition' => array('name' => array('en-US' => "$classname and has $attempt_left $attempt left"))),
								'timestamp' => $stored,
								'stored' => $stored
						);
		/*
		 * Used to capture version details of the content
		 * statement: @username attempted '@course_name and version number of the content is @version_number' (with a score of @score_value)
		 */
		$versionDetails = array('actor' => $actor,
								//'result' => $content_result,
								'context' => $context,
								'verb' => array('id' => 'http://adlnet.gov/expapi/verbs/attempted', 'display' => array("en-US" => "attempted")), 
								'object' => array('type' => 'course', 'definition' => array('name' => array('en-US' => "$classname and version number of the content is $versionNo"))),
								'timestamp' => $stored,
								'stored' => $stored
						);
		
		if ($attachResults == true) { // attach results object only to specfic content types
			//$statusDetails['result'] 		= $content_result;
			//$scoreDetails ['result']		= $content_result;
			//$validityDetails['result']		= $content_result;
			//$bookDetails['result']			= $content_result;
			//$launchDateTime['result'] 		= $content_result;
			//$track['result'] 				= $content_result;
			//$launchDetails['result'] 		= $content_result;
			$versionDetails['result'] 		= $content_result;
		}

		$statusDetails 		= json_encode($statusDetails);
		$scoreDetails 		= json_encode($scoreDetails);
		$validityDetails	= json_encode($validityDetails);
		$bookDetails		= json_encode($bookDetails);
		$launchDateTime 	= json_encode($launchDateTime);
		$track 				= json_encode($track);
		$launchDetails 		= json_encode($launchDetails);
		$versionDetails 	= json_encode($versionDetails);

		if(empty($last_lesson_location) || $last_lesson_location == 'null'){
			$last_lesson_location = "";
		}

		/*expDebug::dPrint('$$statusDetails '.print_r($statusDetails, 1));
		expDebug::dPrint('$$scoreDetails '.print_r($scoreDetails, 1));
		expDebug::dPrint('$$validityDetails '.print_r($validityDetails, 1));
		expDebug::dPrint('$$bookDetails '.print_r($bookDetails, 1));
		expDebug::dPrint('$$launchDateTime '.print_r($launchDateTime, 1));
		expDebug::dPrint('$$track '.print_r($track, 1));
		expDebug::dPrint('$$launchDetails '.print_r($launchDetails, 1));
		expDebug::dPrint('$$versionDetails '.print_r($versionDetails, 1));*/
		
		expDebug::dPrint("Details for AICC Content luanch inc".print_r($scoreDetails,1).'status name'.$statusName.'<--score-->>'.$score,4);
		expDebug::dPrint("2nd time Details for AICC Content luanch inc".print_r($result[$key]->comp_status,1).'previous status name'.$prevCompStatus.'<--score-->>'.$score,4);
		
		if(!empty($statusDetails) && $statusName == 'Completed' && ($prevCompStatus != 'lrn_crs_cmp_cmp' && $prevCompStatus != 'lrn_tpm_ovr_cmp')){
			$api_client->create_statements(array('statements' => $statusDetails));
		}sleep(1);if((!empty($scoreDetails)) && ($statusName != 'Completed' || $score != 0) && ($result[$key]->comp_status == 'lrn_crs_cmp_cmp' || $result[$key]->comp_status == 'lrn_tpm_ovr_cmp')  && !empty($score) && ($prevCompStatus != 'lrn_crs_cmp_cmp' && $prevCompStatus != 'lrn_tpm_ovr_cmp')){
			$api_client->create_statements(array('statements' => $scoreDetails));
		}sleep(1);if(!empty($validityDetails) && $validity_days > 0 && ($prevCompStatus != 'lrn_crs_cmp_cmp' && $prevCompStatus != 'lrn_tpm_ovr_cmp') && ($total_attempts == 1 || empty($total_attempts))){
			$api_client->create_statements(array('statements' => $validityDetails));
		}sleep(1);if((!empty($bookDetails)) && ($last_lesson_location != '') && $calltype != "'assesmentTP'"){
			$api_client->create_statements(array('statements' => $bookDetails));
		}sleep(1);if(!empty($launchDateTime)  && $calltype != "'assesmentTP'"  && $attempt_left >= 0){
			$api_client->create_statements(array('statements' => $launchDateTime));
		}sleep(1);if(!empty($track) && $attempt_left >= 0){
			$api_client->create_statements(array('statements' => $track));
		}sleep(1);if(!empty($launchDetails) && $attempt_left >= 0){
			$api_client->create_statements(array('statements' => $launchDetails));
		}sleep(1);if(!empty($versionDetails) && $versionNo >= 0){
			$api_client->create_statements(array('statements' => $versionDetails));
		}

	}catch (Exception $ex) {
		watchdog_exception('tincanCreateStatement', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * To set the completion status as completed for all the classes with the same id
 */
function getUpdateCompletionStatus($LearnerId, $ClassId, $enrollid){
	try{
		$LearnerId = str_replace("'","",$LearnerId);
		$ClassId = str_replace("'","",$ClassId);
		$query = db_select('slt_enrollment', 'enr');
		$query->addField('enr','id','id');
		$query->condition('enr.user_id', $LearnerId, '=');
		$query->condition('enr.class_id', $ClassId, '=');
		$query->condition('enr.id', $enrollid, '!=');
		$query->condition('enr.comp_status', 'lrn_crs_cmp_cmp', '!=');
		$result = $query->execute()->fetchAll();
		expDebug::dPrint('Completion Result: '.print_r($result, true) , 3);
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getUpdateCompletionStatus', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * To calculate and update the overall percentage
 */
function updateOverallPercentage($enrollid, $score, $objecttype){
	try{
		$score = str_replace("'","",$score);
		if($objecttype == 'cre_sys_obt_cls'){
			$totalscore = 0;
			$result = db_query("select score, master_enrollment_id from slt_enrollment where master_enrollment_id IN (select master_enrollment_id from slt_enrollment where id = '$enrollid')");
			$totalenrollment = $result->rowCount();
			foreach($result as $enrolldetails){
				$totalscore += $enrolldetails->score;
				$master_enrollment_id = $enrolldetails->master_enrollment_id;
			}
			$enrollid = (integer)$master_enrollment_id;
			$score = round($totalscore/$totalenrollment, 0);
		}
		// Update master enrollment table
		//$grade = str_replace("'","",$grade);
		expDebug::dPrint('$enrollid, $score,$objecttype :'.$enrollid.' '.$score.' '.$objecttype , 4);
		//$txn = db_transaction();
		//try{
			db_update('slt_master_enrollment')
			->fields(array('score' => $score,'updated_on'=>now()))
			->condition('id', $enrollid, '=')
			->execute();
		/*}catch (Exception $ex) {
			$txn->rollback();
			unset($txn);
			watchdog_exception('updateOverallPercentage', $ex);
			expertusErrorThrow($ex);
		}
		// Commit the transaction
		unset($txn);*/
	}catch (Exception $ex) {
		watchdog_exception('updateOverallPercentage', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * sendCourseCompletionNotification() - Sends course_completed notification to the learner on his course completion
 */
function sendCourseCompletionNotification($learnerId, $classId, $enrollmentId = '') {
	try{
		$sel_surveymap = db_select('slt_survey_mapping','mapp');
		$sel_surveymap->addExpression('count(mapp.id)','mcount');
		$sel_surveymap->condition('mapp.object_id',$classId,'=');
		$sel_surveymap->condition('mapp.object_type','cre_sys_obt_cls','=');
		$result_surveymap = $sel_surveymap->execute()->fetchAssoc();
		
		$select = db_select('slt_enrollment','enr');
		$select->addExpression('count(enr.id)','enrcount');
		$select->condition('enr.class_id',$classId,'=');
		$select->condition('enr.user_id',$learnerId,'=');
		$select->condition('enr.master_enrollment_id','','IS NULL');
		expDebug::dPrintDBAPI('EnrollmentofCourse ', $select);
		$result = $select->execute()->fetchAssoc();
		
		$encrypted1 = new Encrypt();
		$encrypted2 = new Encrypt();
		
		$objType = $encrypted2->encrypt('Class');
		
		$survey_url = $GLOBALS['base_url']. '?q=share/surveylink/multiple/'.$encrypted1->encrypt($classId).'/'.$objType.'/';
		$survey_sharelink = '<a href="'.$survey_url.'">'.t('LBL3087').'</a>'.' '.t('MSG860', array('@delivry_type'=>t('Class')));
		
		if($result['enrcount'] > 0){
		$dao = new AbstractDAO();
		$dao->connect();
		$courseCompletedNotify = "CALL slp_lnr_notification_ins(" .
  						   		"'course_completed', " .
		$learnerId . ", " .
		((!empty($classId))? $classId : 'null') . ", " . "'', ". (($result_surveymap['mcount']>0) ? "'$survey_sharelink'" : "''") .", " . $learnerId . ")";
		expDebug::dPrint(" SQL query = " . $courseCompletedNotify , 4);
		$dao->query($courseCompletedNotify);
		$dao->closeconnect();
		}
		/*
		 * Start # 0039858 -  Salesforce Chatter Feed
		 * Added By : Ganesh Babu V, Oct 10th 2014 4:15 PM
		 * Description: Send the notification parametre to post the feed to salesforce user according to notification type and notification code
		 * Notification Type : course_completed , Course Completion for WBT / Video Delivery Type
		 * Ticket : #0039858: Implement the salesforce chatter feed in ExpertusONE
		 */

		if(module_exists('salesforce_integration')){
			module_load_include('inc','salesforce_integration','salesforce_integration_chatter');
			postSalesforceFeed('course_completed','course_completed','', $classId, $learnerId);
		}
			
		/* End # 0039858 -  Salesforce Chatter Feed */

	}catch (Exception $ex) {
		watchdog_exception('sendCourseCompletionNotification', $ex);
		expertusErrorThrow($ex);
	}
}

function getLaunchDetailWrapperAPI($enrollId){
	try{
		$results = getLaunchDetail($enrollId);
		expDebug::dPrint("launch data....");
		expDebug::dPrint($results , 4);
		return $results;
	}catch (Exception $ex) {
		watchdog_exception('getLaunchDetailWrapperAPI', $ex);
		expertusErrorThrow($ex);
	}
}
function getMaxAttempts($courseId,$classId,$survey_id){
	try{
		$learnerId  = getSltpersonUserId();
		expDebug::dPrint('checkMaxAttempts Result $courseId,$classId: '.$courseId.','.$classId , 4);
		$query = db_select('slt_attendance_summary', 'att');

		$query->addExpression('sum(total_attempts)','max_attempt');
		$query->where('att.class_id='.$classId.' and att.course_id='.$courseId.' and att.user_id='.$learnerId.' and assessment_id='.$survey_id);
		$maxAttempt = $query->execute()->fetchField();
		expDebug::dPrint('getMaxAttempts $maxAttempt: '.print_r($maxAttempt, true) , 3);
		return $maxAttempt;
	}catch (Exception $ex) {
		watchdog_exception('getMaxAttempts', $ex);
		expertusErrorThrow($ex);
	}
}
function getClassDelType($courseId,$classId){
	try{
		$query = db_select('slt_course_class', 'cls');
		$query->addField('cls','delivery_type','del_type');
		$query->where('cls.id='.$classId.' and cls.course_id='.$courseId);
		$delType = $query->execute()->fetchField();
		expDebug::dPrint('getClassDelType $$delType: '.print_r($delType, true) , 3);
		return $delType;
	}catch (Exception $ex) {
		watchdog_exception('getMaxAttempts', $ex);
		expertusErrorThrow($ex);
	}
}
function getScoreForMultiContent($enrollId){
	$txn = db_transaction();
	try{
		$select = db_select('slt_attendance_summary', 'summ');
		$select->leftJoin('slt_content_lesson', 'les', 'les.id=summ.lesson_id');
		$select->leftJoin('slt_content_version', 'ver', 'ver.id = les.content_version_id');
		$select->leftJoin('slt_content_master', 'mst', 'mst.id=ver.content_master_id');
		$select->join('slt_enrollment_content_mapping', 'emap', 'emap.version_id = summ.content_version_id');
		$select->addExpression('sum(summ.score)', 'ContScore');
		$select->addField('mst', 'id','id');
		$select->condition('summ.enrollment_id' , $enrollId, '=');
		$select->condition('emap.enroll_id' , $enrollId, '=');
		$select->groupBy('summ.enrollment_id');
		$select->groupBy('mst.id');
		expDebug::dPrintDBAPI('getting score for content level', $select);
		$result1 = $select->execute()->fetchAllKeyed();
		return $result1;
	}catch (Exception $ex) {
		$txn->rollback();
		unset($txn);
		watchdog_exception('getMaxAttempts', $ex);
		expertusErrorThrow($ex);
	}
	unset($txn);
}
?>