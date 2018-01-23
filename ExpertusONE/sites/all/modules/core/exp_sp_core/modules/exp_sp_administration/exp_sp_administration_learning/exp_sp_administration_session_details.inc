<?php

/*
 * Caluculate ILT and VC session time for Duration count
 */

function durationTimeOperations ($classId){
	$txn = db_transaction();

	try {
		$classDetails    = _getSessionDetails($classId);
		$cnt             = count($classDetails);
		expDebug::dPrint('old_dateformat = ' .print_r($classDetails,true) , 4);
		for ($i = 0; $i < $cnt; $i++) {
			$get_date           =  $classDetails[$i]->start_date;
			$new_date_format    = date('m-d-Y', strtotime($get_date));
			$split_date_format  = explode("-", $new_date_format);
			$start_time         =  $classDetails[$i]->start_time;
			$end_time           =  $classDetails[$i]->end_time;
			$startdate          = $split_date_format[2].'-'.$split_date_format[0].'-'.$split_date_format[1].' '.$start_time.':00' ;
			$enddate            = $split_date_format[2].'-'.$split_date_format[0].'-'.$split_date_format[1].' '.$end_time.':00' ;
			expDebug::dPrint('Tetsfsdf $startdate '. $startdate , 4);
			expDebug::dPrint('Tetsfsdf $enddate '. $enddate , 4);

			$start_date_new     = new DateTime($startdate);
			$since_start        = $start_date_new->diff(new DateTime($enddate));
			$minutes            = $since_start->days * 24 * 60;
			$minutes += $since_start->h * 60;
			$minutes += $since_start->i;
			$totalDuration +=  $minutes;
			expDebug::dPrint('old_dateformat = ' .print_r($new_date_format,true) , 4);
		}

		expDebug::dPrint('Total Duration = ' .print_r($totalDuration,true) , 4);
		$updateStmt = db_update('slt_course_class');
		$updateStmt->condition('id', $classId);
		$fields = array(
                  'scheduled_duration' => $totalDuration,
		);
		// Select the fields and the values to be updated
		$updateStmt->fields($fields);
		$durationUpdated = $updateStmt->execute();
	}
	catch (Exception $ex) {
		$txn->rollback();
		watchdog_exception('durationTimeOperations', $ex);
		throw $ex;
	}

	unset($txn);

}

function sessionDetailOperations($form, &$form_state,$entityId,$entityType){
try {
	expDebug::dPrint('sessionDetailOperations = ' . print_r($form_state['values'], true) , 5);
	$cnt = count($form_state['values']['session_details_fieldset']['session_details']);
	$isAdded = 0;
	$isUpdated = 0;
	for ($i = 0; $i < $cnt; $i++) {
		if(!empty($form_state['values']['session_details_fieldset']['session_details']['session_name_'.$i])) {
			if(empty($form_state['values']['session_id_'.$i])) {
				expDebug::dPrint('sessionDetailOperations for -- Insert');
				addNewSessionDetails($form, $form_state,$entityId,$entityType,$i);
				$isAdded = 1;
			}else{
				expDebug::dPrint('sessionDetailOperations for -- Update');
				updateSessionDetails($form, $form_state,$form_state['values']['session_id_'.$i],$entityType,$i);
				$isUpdated = 1;
			}
		}
	}
	durationTimeOperations($form_state['values']['class_id']);
	if($isAdded){
		expertus_set_message(t('LBL272').' '.t('MSG601').'.');
	}
	elseif($isUpdated){
		expertus_set_message(t('LBL272').' '.t('MSG601').'.');
	}
	$form_state['add_another'] = 'show';
	} catch (Exception $ex) {
		watchdog_exception('sessionDetailOperations', $ex);
		expertusErrorThrow($ex);
	}
}


function _getSessionDetails($classId,$queryRequired='',$sessionId) {
try {
	$select = db_select("slt_course_class_session","sess");
	$select->innerJoin('slt_course_class','cls','cls.id = sess.class_id ');
	$select->leftJoin('slt_session_instructor_details','ses_ins','sess.id = ses_ins.session_id');
	$select->leftJoin('slt_profile_list_items','prof','sess.type = prof.code');
	$select->leftJoin('slt_profile_list_items','prof_zone','sess.timezone = prof_zone.code');
	$select->leftJoin('slt_person','per','per.id = ses_ins.instructor_id');
	
	$select->addField('sess','id','session_id');
	$select->addField('sess','course_id','course_id');
	$select->addField('sess','class_id','class_id');
	$select->addField('sess','title','title');
	$select->addField('sess','start_date','start_date');
	$select->addField('sess','start_time','start_time');
	$select->addField('sess','end_time','end_time');
	$select->addField('sess','attendee_url','attendee_url');
	$select->addField('sess','presenter_url','presenter_url');
	$select->addField('sess','timezone','timezone');
	$select->addField('sess','type','meeting_type');
	$select->addField('cls','delivery_type','delivery_type');
	$select->addField('cls','min_seats','min_seats');
	$select->addField('cls','max_seats','max_seats');
	$select->addField('cls','waitlist_count','waitlist_count');
	$select->addField('cls','location_id','location_id');
	$select->addField('sess','room_id','room_id');
	//$select->addField('sess','instructor_id','instructor_id');
	$select->addExpression('GROUP_CONCAT(DISTINCT(ses_ins.instructor_id))', 'instructor_id');
	$select->addExpression('GROUP_CONCAT(if(ses_ins.is_presenter = \'Y\',ses_ins.instructor_id, null))', 'presenter_id');
	$select->addField('sess','facility_id','facility_id');
	$select->addField('sess','attendee_url','session_attendeeurl');
	$select->addField('sess','attendee_url','session_attendeepass');
	$select->addField('sess','presenter_url','session_presenterpass');
	$select->addField('sess','presenter_url','session_presenterurl');
	$select->addField('prof','name','meeting_type_name');
	$select->addField('prof_zone','name','timezone_name');
	$select->addExpression("GROUP_CONCAT(DISTINCT(per.full_name) SEPARATOR ', ')", 'full_name');
	//$select->addField('per','full_name','full_name');
	$select->condition('sess.class_id',$classId,'=');
	if($sessionId){
		$select->condition('sess.id',$sessionId,'=');
	}
expDebug::dPrintDBAPI('sample query for checking',$select);
	/* $select->orderBy("sess.start_date", "ASC");
	$select->orderBy("sess.start_time", "ASC"); */
   $select->groupBy("sess.class_id");
   $select->groupBy("sess.id");
	if(empty($queryRequired)){
		return $select->execute()->fetchAll();
	} else {
		return $select;
	}
	} catch (Exception $ex) {
		watchdog_exception('_getSessionDetails', $ex);
		expertusErrorThrow($ex);
	}
}

function deleteSessionDetails($entityId,$entityType,$sessionId) {
	$txn = db_transaction();

	try {
		if($form_state['values']['create_user_id'])
		$loggedInUserId = $form_state['values']['create_user_id'];
		else
		$loggedInUserId  = getIdOfLoggedInUser();
		if(!empty($sessionId)){
			$last_run_date = date("Y-m-d");
			$select = db_select('slt_course_class_session', 'ses');
			$select->leftjoin('slt_session_instructor_details', 'ses_ins', 'ses_ins.session_id=ses.id');
			$select->leftjoin('slt_course_class', 'class', 'class.id=ses.class_id');
			// Join other tables.
			$select->leftJoin('slt_location', 'loc', 'ses.location_id = loc.id');
			$select->leftjoin('slt_profile_list_items', 'splt_timezone', 'splt_timezone.code=ses.timezone');
			$select->leftJoin('slt_country', 'country', 'loc.country = country.country_code');
			$select->leftJoin('slt_state', 'state', 'country.country_code = state.country_code &&  loc.state = state.state_code');
			$select->addField('ses', 'id', 'id');
			$select->addField('ses', 'title', 'title');
			$select->addField('ses', 'course_id', 'course_id');
			$select->addField('ses', 'class_id', 'class_id');
			$select->addExpression('GROUP_CONCAT(DISTINCT(ses_ins.instructor_id))', 'instructor_id');
			//$select->addField('ses', 'instructor_id', 'instructor_id');
			$select->addField('ses', 'start_time', 'start_time');
			$select->addField('ses', 'end_time', 'end_time');
			$select->addField('ses', 'start_date', 'start_date');
			$select->addField('ses', 'end_date', 'end_date');
			$select->addField('ses', 'timezone', 'timezone');
			$select->addField('loc', 'name', 'location_name');
			$select->addField('loc', 'addr1', 'addr1');
			$select->addField('loc', 'addr2', 'addr2');
			$select->addField('loc', 'city', 'city');
			$select->addField('state', 'state_name', 'state');
			$select->addField('country', 'country_name', 'country');
			$select->addField('loc', 'zipcode', 'zipcode');
			$select->addField('ses', 'ical_sequence', 'seq');
			$select->addField('splt_timezone','attr2', 'session_timezone');
			$select->addField('splt_timezone','name', 'session_timezone_name');
			$select_or = db_or();
// 			$select_or->condition('ses.created_on', $last_run_date,'<');
// 		  $select_or->condition('ses.updated_on', $last_run_date,'<');
		  $select_or->where("DATE_FORMAT(ses.created_on, '%Y-%m-%d') < DATE_FORMAT("."'".$last_run_date."'".",  '%Y-%m-%d')");
		  $select_or->where("DATE_FORMAT(ses.updated_on, '%Y-%m-%d') < DATE_FORMAT("."'".$last_run_date."'".", '%Y-%m-%d')");
		  $select->condition($select_or);
		  $select->condition('ses.id', $sessionId,'=');
		  //In this condition in feature need to check the time zone related work for vc class.
		  $select->where("DATE_FORMAT(ses.start_date, '%Y-%m-%d') > DATE_FORMAT("."'".$last_run_date."'".",  '%Y-%m-%d')");
		  //condition('ses.start_date', $last_run_date,'>=');
		  $select->condition('ses_ins.instructor_id', '','!=');
		  $select->condition('class.status',array('lrn_cls_sts_atv','lrn_cls_sts_itv'), 'IN');
			expDebug::dPrintDBAPI(' getSession Details = ' , $select);
			$result = $select->execute();
			expDebug::dPrint(' $$Info Result = ' . print_r($result, true) , 3);
			if($result->rowCount() > 0) {				
				$sessionDetails = $result->fetch();
								
			 	if(!empty($sessionDetails->location_name)) {
			 		 $LocationDel = ClassLocationDetails($entityId,$sessionDetails->location_name);
					 if(count($LocationDel) < 0) $LocationDel = $sessionDetails->location_name;
					 } else $LocationDel = ' - ';					 
 			}
 			
 			$selectstmt=db_select('slt_course_class_session','ses');
 			$selectstmt->addField('ses','id','id');
 			$selectstmt->addField('ses','class_id','p2');
 			$selectstmt->addField('ses','course_id','p1');
 			$selectstmt->condition('id', $sessionId,'=');
 			expDebug::dPrintDBAPI(' $selectstmt' , $selectstmt);
 			$selectstmt1=$selectstmt->execute()->fetchAll();
 			expDebug::dPrint('$selectstmt1' . print_r($selectstmt1, true) , 3);

			$deleteStmt = db_delete('slt_course_class_session');
			$deleteStmt->condition('id', $sessionId,'=');
			$numUpdated = $deleteStmt->execute();
			
			foreach($selectstmt1 as $val){
			$fields= array(
			        'table_name' => 'slt_course_class_session',
			        'entity_id' => $val->id,
			        'entity_type' => 'session',
			        'parent1_entity_id' => $val->p1,
			        'parent1_entity_type' => 'course_id',
			        'parent2_entity_id' => $val->p2,
			        'parent2_entity_type' => 'class_id',
			        'module_name' => 'exp_sp_administration_session_details',
			        'functionality_name' => 'deleteSessionDetails',
			        'api_name' => $_REQUEST['apiname'],
			        'deleted_on' => now(),
			        'deleted_by' => $loggedInUserId,
			
			);
			deleted_log_entry($fields);
			//Don't delete this update this is report flat table related update.
			$updComment  = db_update('slt_course_class');
			$updComment->fields(array(
					'updated_on' => now(),
			));
			$updComment->condition('id', $val->p2);
			expDebug::dPrintDBAPI(' session delete update for class SQL = ' , $updComment);
			$updComment->execute();
			}
			
			durationTimeOperations ($entityId); // #47464 Modified by joolavasavi.
			add_audit_trail_entry($loggedInUserId, $sessionId, $entityType, 'exp_sp_administration_session_details', 'exp_sp_administration_session_details', 'Session Deleted');

			$select=db_select('slt_session_instructor_details','ins');
			$select->addField('ins','id','id');
			$select->addField('ins','instructor_id','p4');
			$select->addField('ins','session_id','p3');
			$select->addField('ins','class_id','p2');
			$select->addField('ins','course_id','p1');
			$select->condition('session_id', $sessionId,'=');
			$select1=$select->execute()->fetchAll();
	
			$deleteStmt = db_delete('slt_session_instructor_details');
			$deleteStmt->condition('session_id', $sessionId,'=');
			$numUpdated = $deleteStmt->execute();
			
			foreach($select1 as $val){
			    $fields= array(
			            'table_name' => 'slt_session_instructor_details',
			            'entity_id' => $val->id,
			            'entity_type' => 'instructor_mapping',
			            'parent1_entity_id' => $val->p1,
			            'parent1_entity_type' => 'course_id',
			            'parent2_entity_id' => $val->p2,
			            'parent2_entity_type' => 'class_id',
			            'parent3_entity_id' => $val->p3,
			            'parent3_entity_type' => 'session_id',
			            'parent4_entity_id' => $val->p4,
			            'parent4_entity_type' => 'instructor_id',
			            'module_name' => 'exp_sp_administration_session_details',
			            'functionality_name' => 'deleteSessionDetails',
			            'api_name' => $_REQUEST['apiname'],
			            'deleted_on' => now(),
			            'deleted_by' => $loggedInUserId,
			            	
			    );
			    deleted_log_entry($fields);
			}
		
			$notfication_status = getPreferredNotification('class_session_cancel_notification_for_instructor','cre_sys_lng_eng');
			expDebug::dPrint(' $$notfication_status STAT = ' . print_r($notfication_status, true) , 3);
			if ($result->rowCount() > 0 && $notfication_status >0) {
				 if(!empty($sessionDetails->instructor_id)){
				 	$instructor_ids_arr = explode(',',$sessionDetails->instructor_id);
					foreach ($instructor_ids_arr as $ins_id){
						 expDebug::dPrint(' $$classSessionInfo = ' . print_r($sessionDetails, true) , 3);
						 $notificationInfo = array();
						 $classSessionInfo_arr = array();
						 $classInfo = getClassInfoForNotifications($sessionDetails->class_id);
						 expDebug::dPrint(' $classInfo = ' . print_r($classInfo, true) , 3);
						 $classInstructorInfo  = getUserInformations($ins_id);
						 $iCalArr = array();
						 $iCalArr['notificationVal'] = array('0'=>$sessionDetails);
						 $iCalArr['classInfo'] = $classInfo;
						 $iCalArr['classInstructorInfo'] = $classInstructorInfo;
						 $iCal = getCancelICal($key, 'Class',null,$iCalArr,'delete');
						 expDebug::dPrint(' $classInstructorInfo = ' . print_r($classInstructorInfo, true) , 3);
						 $notificationInfo['send_to_mail'] = $classInstructorInfo['email'];
						 $notificationInfo['user_id'] = $ins_id;
						 $notificationInfo['send_to_name'] = $classInstructorInfo['first_name'];
						 if($classInfo['delivery_type'] == 'lrn_cls_dty_vc')
						 		$del_type = t('Virtual Class');
						 else
						 		$del_type = t('Classroom');
						 $notificationInfo['tokens_string'] =	'first_name>|' . $classInstructorInfo['first_name'] . '~|' .
						 		'last_name>|' . $classInstructorInfo['last_name'] . '~|' .'delivery_type>|'.$del_type. '~|' .
						 		'class_title>|' . $classInfo['title'] . '~|' .
						 		'class_code>|' . $classInfo['code'] . '~|';
						 expDebug::dPrint(' $notificationVal = ' . print_r($notificationVal, true) , 3);


						 $sessionTitle = ($sessionDetails->title == null)? ' ' : $sessionDetails->title;
						 $sessionStartDate = dateTimeStrToUSShort($sessionDetails->start_date, true, true, true, false);
						 $sessionStartDate = convertNullValToNullStr($sessionStartDate);
						 $sessionEndDate   = dateTimeStrToUSShort($sessionDetails->end_date, true, true, true, false);
						 $sessionEndDate = convertNullValToNullStr($sessionEndDate);
						 $sessionStartTime = ($sessionDetails->start_time != '' && $sessionDetails->start_time != null)?date_format(date_create($sessionDetails->start_time),'g:i A'):'';
						 $sessionEndTime   = ($sessionDetails->end_time != '' && $sessionDetails->end_time != null)?date_format(date_create($sessionDetails->end_time),'g:i A'):'';
						 $sessionLocation  = $sessionDetails->location_name;
						 $sessionLocation = convertNullValToNullStr($sessionLocation);
						 $sessionTimeZone = convertNullValToNullStr($sessionDetails->session_timezone_name);
						 if(!empty($sessionTimeZone))
						 	$sessionTimeZoneDisp = substr($sessionTimeZone,stripos($sessionTimeZone,')')+2);
						 $classSessionInfo =	'session_name<|' . $sessionTitle . '~~' .
																	 'session_startdate<|' . $sessionStartDate . '~~' .
																	 'session_enddate<|' . $sessionEndDate . '~~' .
																	 'session_starttime<|' . $sessionStartTime . '~~' .
																	 'session_endtime<|' . $sessionEndTime .  '~~' .
																	 'session_location<|' . $sessionLocation . '~~' .
																	 'time_zone<|' . $sessionTimeZoneDisp .
																	 '~@';
						 $classSessionInfo_arr[0] = $classSessionInfo;
						 $classSessionInfo_arr[1] = $sessionLocation;
						 $classSessionInfo_arr[2] = $sessionTimeZone;

						 expDebug::dPrint(' $$classSessionInfo = ' . print_r($classSessionInfo_arr, true) , 3);
						 $timeZone = '';
						 $classSessionInformation = $classSessionInfo_arr[0];
						 if ($classSessionInformation == null) {
						 	$classSessionInformation = 'session_name<| ~~session_startdate<| ~~session_enddate<| ~~session_starttime<| ~~session_endtime<| ~~time_zone<| ~@';
						 }

						 $sessionLocation = $classSessionInfo_arr[1];
						 if ($sessionLocation == null) {
						 	$sessionLocation = '';
						 }
						 // Append tokens string with class session details
						 $notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
						 'session_location>|' . $LocationDel . '~|' .
						 'group_sessiondata>|' . $classSessionInformation;

						 // For vc class append timezone to location
						 if ( $classInfo['delivery_type'] == 'lrn_cls_dty_vc') {
						 	$notificationInfo['tokens_string'] = '~|'.$notificationInfo['tokens_string'] .
						 	' (' . $timeZone . ') ';
						 }
						 //class_session_cancel_notification_for_instructor cancel the session when notification send.
						 $notificationInfo['message_id'] = 'class_session_cancel_notification_for_instructor';
						 $notificationInfo['message_type'] = 'Class Session cancel details for instructor';
						 $notificationInfo['send_type'] = 'php mailer';
						 $nullValue = NULL;
						 $customAdmin = 1;
						 $insertStmt = db_insert('slt_notification');
						 $insertStmt ->fields(array(
						 		'msg_id' => $notificationInfo['message_id'],
						 		'lang_code' => $classInstructorInfo['preferred_language'],
						 		'msg_type' => $notificationInfo['message_type'],
						 		'token_str' => $notificationInfo['tokens_string'],
						 		'user_id' => $notificationInfo['user_id'],
						 		'send_to_id' => $notificationInfo['user_id'],
						 		'send_to_name' => $notificationInfo['send_to_name'],
						 		'send_to_email' => $notificationInfo['send_to_mail'],
						 		'send_type' => $notificationInfo['send_type'],
						 		'send_status' => 'N',
						 		'attach_content' => $nullValue,
						 		'ical' =>$iCal,
						 		'remarks' => $nullValue,
						 		'created_by' => $customAdmin,
						 		'created_on' => now(),
						 		'updated_by' => $customAdmin,
						 		'updated_on' => now(),
						 		'custom0'    => $nullValue
						 ));
						 $userNotificationId = $insertStmt->execute();
						 add_audit_trail_entry(1, $entityId, $entityType, 'exp_sp_administration_session_detail', $notificationInfo['send_type'],	$notificationInfo['message_type'] . $notificationInfo['send_to_mail']);
				 }
			}
		}
	}
	}
	catch (Exception $ex) {
		$txn->rollback();
		watchdog_exception('deleteSessionDetails', $ex);
		throw $ex;
	}

	unset($txn);

}

function displaySessionDetails(&$form, &$form_state, $course_id, $class_id, $entityType, $uniqueId, $deliveryType, $readonly = false,$oldClassId, $newlyCreated = false){
try {
	$newlyCreated = ($newlyCreated == true ? 'new-class' : 'old-class');
	global $theme;
	/* $js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
	 drupal_add_js(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_addedit_behaviours.js', $js_module_optional);
	 drupal_add_js(drupal_get_path('module', 'exp_sp_administration') .'/jquery.timePicker.js', $js_module_optional);*/
	expDebug::dPrint('$form[session_details_fieldset] = '. print_r($form['session_details_fieldset'], true), 5);

	$sessionDetails = array();
	$form['attr_action'] = array(
    '#type' => 'hidden',
    '#name' => 'attr_action',
    '#value' => 'sessiondetail',
	);
	
	if($deliveryType == 'lrn_cls_dty_ilt'){
		$msg = t('MSG360');
	}/*else{
		//$msg = t('MSG606');
		}*/
	$uniqueId = "session-".core_encrypt($class_id)."-".$entityType;
	$wrapperId = 'session-detail-wrapper-'.$uniqueId;
	$formName = 'session_details_fieldset';
	$form_state['form_unique_name'] = 'session_details_fieldset'; //$formName;
	if(count($form[$formName])){
		unset($form[$formName]);
	}
	if($deliveryType == 'lrn_cls_dty_ilt'){
		$form[$formName] = array(
				'#type' => 'markup',
				'#prefix' => '<div id="admin-data-grid"><div id="session-details-fieldset-wrapper-'.$uniqueId.'" class="admin-tab-list-row session-details-fieldset-wrapper"><div id="'.$wrapperId.'" class="admin-datagrid-pagination"><div class="admin-datagrid-session-with-addanother classroom-session">',
				'#suffix' => '</div></div></div></div>',
		);
	}
 else {
	$form[$formName] = array(
	    '#type' => 'markup',
	    '#prefix' => '<div id="admin-data-grid"><div id="session-details-fieldset-wrapper-'.$uniqueId.'" class="admin-tab-list-row session-details-fieldset-wrapper"><div id="'.$wrapperId.'" class="admin-datagrid-pagination"><div class="admin-datagrid-session-with-addanother virtualclass-session">',
	    '#suffix' => '</div></div></div></div>',
	);
 }
	$form[$formName]['hidden_idlist'] = array(
	    '#type' => 'hidden',
	    '#name' => 'hidden_idlist_'.$uniqueId,
	    '#id' => 'session-idlist-'.$uniqueId,
	);


	$form[$formName]['add-session-delivery-type'.$uniqueId] = array (
		'#type' => 'hidden',
		'#value' => $deliveryType,
		'#attributes' => array('id'=>'add-session-delivery-type'.$uniqueId)
	);

	if(empty($class_id)) {
		$class_id = $form_state['class_id'];
	}
	if (!empty($class_id)) {
		$sessionDetails = _getSessionDetails($class_id);
	}
	$sessionDetailsCount  = count($sessionDetails);
	$startSession = ($sessionDetailsCount == 0) ? $form_state['num_session'] : $sessionDetailsCount;
	$displayMode = $form_state['storage']['mode'];
	expDebug::dPrint('sessionCount=>'.$form_state['num_session'] . ' displayMode --' . $displayMode . ' -- sessiondetailscount ' . $sessionDetailsCount . ' class ' . $class_id , 5);
	
	if ($theme == 'expertusoneV2') {
		$webSessionWidth 	= 515;
		$otherSessionWidth	= 515;
	} else {
		$webSessionWidth 	= 600; //551;
		$otherSessionWidth	= 550; //475;
	}

	if($startSession == 0){   //&& empty($displayMode)
		$addAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
		$addAttributes['data-wrapperid'] = array($wrapperId);
		if(isset($form_state['storage']['display_mode']) && $form_state['storage']['display_mode'] == 'view'){
			$form[$formName]['add_session_text'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.t('There are no sessions available for this class.').'</div>',
			);
		} else {

			$form[$formName]['session_type_load'] = array(
     	'#type' => 'markup',
     	'#prefix' => '<div style="display:none"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'#root-admin\').data(\'narrowsearch\').reverseSessionType();return false;" width="100" height="132" /></div>',
			);
			$popupentityId      = $class_id;
			$popupAddSessionentityType    = 'session';
			$popupAddSessionIdInit        	 = $popupentityId.'_'.$popupAddSessionentityType;
			$popupAddSessionvisibPopupId  = 'qtip_visible_disp_addsession_'.$popupAddSessionIdInit;
			$anchorId = 'frm-'.$popupAddSessionvisibPopupId;
			$popupAddSession    = "{'entityId':".$popupentityId.",'entityType':'".$popupAddSessionentityType."','url':'ajax/class-add-session/".core_encrypt($course_id)."/".core_encrypt($class_id)."/Ilt/Add/0/".core_encrypt($oldClassId)."/".$newlyCreated."','popupDispId':'".$popupAddSessionvisibPopupId."','catalogVisibleId':'qtipAddSessionIdqtip_visible_disp_".$popupAddSessionIdInit."','wBubble':$otherSessionWidth,'hBubble':'auto','tipPosition':'tipfaceMiddleRight','qtipClass':'admin-qtip-access-parent'}";

	  if($deliveryType == 'lrn_cls_dty_ilt'){
	  	$form[$formName]['add_session'] = array(
	        '#type' => 'markup',
	        '#markup' => '<div class="admin-datagrid-add-btn-wrapper-container"><div class="add_session_popup"><div class="add-session-popup-box" id="'.$popupAddSessionvisibPopupId.'"><a onclick = "$(\'#root-admin\').data(\'narrowsearch\').getQtipDiv('.$popupAddSession.'); return false;"><span class="admin-save-button-left-bg"></span><span class="admin-save-button-middle-bg">'.t('LBL599').'</span><span class="admin-save-button-right-bg"></span><div class="clearBoth"></div></a></div></div>',
	  	);
	  }else{
	  	$meetingTypeinURL = getProfileItemNamesKeyed('lrn_cls_vct_');
	  	$meetingTypeOrder = array();
	  	$keyFind = '';
	  	$meetingList = '';
	  	$meetingTypeinURLCount = count($meetingTypeinURL);
	  	$flag = false;
	  	$backgroundFlag =  false;
	  	$i = 0;
	  	foreach($meetingTypeinURL as $key=>$value){
	  		if($key == 'lrn_cls_vct_oth'){ 
	  			$keyFind = $value;
	  		}else{
	  			$meetingTypeOrder[$key] = $value;
	  			$popupAddSessionVC    = "{'entityId':".$popupentityId.",'entityType':'".$popupAddSessionentityType."','url':'ajax/class-add-session/".core_encrypt($course_id)."/".core_encrypt($class_id)."/".$key."/Add/0/".core_encrypt($oldClassId)."/".$newlyCreated."','popupDispId':'add_new_".$popupAddSessionvisibPopupId."','catalogVisibleId':'qtipAddNewSessionIdqtip_visible_disp_".$popupAddSessionIdInit."','wBubble':$otherSessionWidth,'hBubble':'196px','tipPosition':'tipfaceMiddleRight','qtipClass':'admin-qtip-access-parent ".$key."'}";
	  			if($key != 'lrn_cls_vct_exp'){
	  			  if(array_key_exists('lrn_cls_vct_oth',$meetingTypeinURL)){
	  			    if($backgroundFlag == false) {
	  				  $meetingList .= '<li id = '.$key.' class="'.(($i==1 or $i==0) ? 'admin-save-button-middle-bg' : '').' session_type_list " onclick = "$(\'#root-admin\').data(\'narrowsearch\').getQtipDiv('.$popupAddSessionVC.'); return false;">'.(($i==1) ? t('LBL287') : '').' '.t($value).'</li>';
	  			      if(array_key_exists('lrn_cls_vct_exp',$meetingTypeinURL)){
	  			      	$msg = t('MSG800');
	  			      } else {
	  			      	$msg = t('MSG756');
	  			      }
	  			    }else{
	  			      $meetingList .= '<li id = '.$key.' class="'.(($i==0) ? 'admin-save-button-middle-bg' : '').' session_type_list " onclick = "$(\'#root-admin\').data(\'narrowsearch\').getQtipDiv('.$popupAddSessionVC.'); return false;">'.(($i==1) ? t('LBL287') : '').' '.t($value).'</li>';
	  			       }}
	  			  else{
	  			    $meetingList .= '<li id = '.$key.' class="'.(($i==0) ? 'admin-save-button-middle-bg' : '').' session_type_list " onclick = "$(\'#root-admin\').data(\'narrowsearch\').getQtipDiv('.$popupAddSessionVC.'); return false;">'.(($i==0) ? t('LBL287') : '').' '.t($value).'</li>';
	  			    $backgroundFlag = true;
	  			       if(array_key_exists('lrn_cls_vct_exp',$meetingTypeinURL)){
		  			    $msg = t('MSG800');
		  			  } else {
		  			  	$msg = t('MSG756');
		  			  }
	  			  }
	  			}
	  			else{
	  				$meetingList .= '<li id = '.$key.' class="'.(($i==0) ? 'admin-save-button-middle-bg' : '').' session_type_list " onclick = "$(\'#root-admin\').data(\'narrowsearch\').getQtipDiv('.$popupAddSessionVC.'); return false;">'.(($i==0) ? t('LBL287') : '').' '.t($value).'</li>';
	  				$msg = t('MSG800');
	  			}

			   if((array_key_exists('lrn_cls_vct_oth',$meetingTypeinURL)) && ($i == 1) && ($flag == false)){
			   	  $meetingList .= '<span class="add-more-session-list session-list dropdownadd-dd-list" style="top:0px;"><div class="dropdownadd-dd-list-arrow"></div>';
			   }
			   elseif($i==0){
			     $meetingList .= '<span class="add-more-session-list session-list dropdownadd-dd-list" style="top:0px;"><div class="dropdownadd-dd-list-arrow"></div>';
			     $flag = true;
			   }
			}
	  		$i++;
	  	}
	  	if(!empty($keyFind)){
	  		$meetingTypeOrder['lrn_cls_vct_oth'] = $keyFind;
	  		$popupAddSessionVC    = "{'entityId':".$popupentityId.",'entityType':'".$popupAddSessionentityType."','url':'ajax/class-add-session/".core_encrypt($course_id)."/".core_encrypt($class_id)."/lrn_cls_vct_oth/Add/0/".core_encrypt($oldClassId)."/".$newlyCreated."','popupDispId':'add_new_".$popupAddSessionvisibPopupId."','catalogVisibleId':'qtipAddNewSessionIdqtip_visible_disp_".$popupAddSessionIdInit."','wBubble':$otherSessionWidth,'hBubble':'auto','tipPosition':'tipfaceMiddleRight','qtipClass':'admin-qtip-access-parent lrn_cls_vct_oth'}";
	  		if($meetingTypeinURLCount == 1){
	  			$meetingList .= '<li id = "lrn_cls_vct_oth" class="admin-save-button-middle-bg"  onclick = "$(\'#root-admin\').data(\'narrowsearch\').getQtipDiv('.$popupAddSessionVC.'); return false;">'.t('LBL287').' '.t($keyFind).'</li>';
	  			$msg = t('MSG757');
	  			}
	  		else{
	  		  $meetingList .= '<li id ="lrn_cls_vct_oth" class="session_type_list " onclick = "$(\'#root-admin\').data(\'narrowsearch\').getQtipDiv('.$popupAddSessionVC.'); return false;">'.(($i==0) ? t('LBL287') : '').' '.t($keyFind).'</li>';
	  		  }
	  	}
	  	if($meetingTypeinURLCount == 1){
	  		 $class = "admin-save-button-right-bg";
	  		 $nomeet = "admin-save-button-left-bg";
	  	} else if($meetingTypeinURLCount == 0){
	  	 	$class = "";
	  	 	$nomeet = "";
	  	} else{
	  		 $class = "pub-unpub-add-action-wrapper pub-unbpub-more-btn ";
	  		 $nomeet = "admin-save-button-left-bg";
	  	}
		  	$meetingList .= '</span></ul><span id="pub-unpub-action-btn" onclick="displayPubActionList(\'session-list\')" class="'.$class.'"></span></div>';
		  	$form[$formName]['add_session'] = array(
		        '#type' => 'markup',
		        '#prefix' => '<div class="admin-datagrid-add-btn-wrapper-container expertus-meeting-button-container">
		        				<div class="add_session_popup" id="add_new_'.$popupAddSessionvisibPopupId.'" ></div>
				        		<div class="add_session_button_div"><ul class="session-list-ul"><span class="'.$nomeet.'"></span>'.
		  	$meetingList.'
				        			',
		  	);
	  }
	  if($meetingTypeinURLCount == 0){
	  	$form[$formName]['add_session_msg'] = array(
	  			'#type' => 'markup',
	  			'#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.t('There are no sessions available for this class.').'</div></div></div>',
	  	);

	  }else{
	  	$form[$formName]['add_session_msg'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.$msg.'</div></div></div>',
	  	);
	  }
		}

	} // no session end
	else{
		$entityId = core_encrypt($class_id);
		$entityType = "cre_sys_obt_cls";
		$uniqueId = "session-".$entityId."-".$entityType;
		$excludedCourseId= $oldClassId;

		$form[$formName]['session_view_grid_markup'] = array(
      '#type' => 'markup',
      '#markup' => '<div id="datagrid-div-'.$uniqueId.'"><table id="datagrid-container-'.$uniqueId.'" class="datagrid-container-common"></table><div id="pager-datagrid-'.$uniqueId.'" class="pager-datagrid-common"></div></div>',
		);

		$displayModeDataGrid = 'view_only';

		$form[$formName]['session_view_grid_load'] = array(
     '#type' => 'markup',
     '#prefix' => '<div style="display:none"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-'.$uniqueId.'\'); $(\'body\').data(\'mulitselectdatagrid\').loadDataGrid(\''.$displayModeDataGrid.'\', \'session\', \'\', \''.$entityId.'\', \''.$entityType.'\', \''.$excludedCourseId.'\');" width="100" height="132" /></div></div>',
		);

		$popupentityId      = $class_id;
	  $popupAddSessionentityType    = 'session';
	  $popupAddSessionIdInit        	 = $popupentityId.'_'.$popupAddSessionentityType;
	  $popupAddSessionvisibPopupId  = 'qtip_visible_disp_addsession_'.$popupAddSessionIdInit;
	  $anchorId = 'frm-'.$popupAddSessionvisibPopupId;

		if($deliveryType == 'lrn_cls_dty_ilt'){


	  $popupAddSession    = "{'entityId':".$popupentityId.",'entityType':'".$popupAddSessionentityType."','url':'ajax/class-add-session/".core_encrypt($course_id)."/".core_encrypt($class_id)."/Ilt/Addanother/0/".core_encrypt($oldClassId)."/".$newlyCreated."','popupDispId':'".$popupAddSessionvisibPopupId."','catalogVisibleId':'qtipAddSessionIdqtip_visible_disp_".$popupAddSessionIdInit."','wBubble':$otherSessionWidth,'hBubble':'auto','tipPosition':'tipTopLeft','qtipClass':'admin-qtip-access-parent'}";


	  $form[$formName]['add_session'] = array(
		    '#type' => 'markup',
		    '#markup' => '<div class="session-add-another-popup-container"><div class="addedit-form-cancel-container-actions"><div class="add_session_popup" id="'.$popupAddSessionvisibPopupId.'" ></div><a class="add-custombtn-symbol" onclick="$(\'#root-admin\').data(\'narrowsearch\').getQtipDiv('.$popupAddSession.'); return false;">'.t('LBL386').'</a></div></div>',
	  );
		}else{

	  	/**************************************************/
		$meetingTypeinURL = getProfileItemNamesKeyed('lrn_cls_vct_');
	  	$meetingTypeOrder = array();
	  	$keyFind = '';
	  	$meetingList = '';
	  	$i = 0;
	  	foreach($meetingTypeinURL as $key=>$value){
	  		if($key == 'lrn_cls_vct_oth'){
	  			$keyFind = $value;
	  		}else{
	  			$meetingTypeOrder[$key] = $value;
	  			if($key == 'lrn_cls_vct_web'){
	  				$popupAddSessionVC    = "{'entityId':".$popupentityId.",'entityType':'".$popupAddSessionentityType."','url':'ajax/class-add-session/".core_encrypt($course_id)."/".core_encrypt($class_id)."/".$key."/Addanother/0/".core_encrypt($oldClassId)."/".$newlyCreated."','popupDispId':'add_more_".$popupAddSessionvisibPopupId."','catalogVisibleId':'qtipAddMoreSessionIdqtip_visible_disp_".$popupAddSessionIdInit."','wBubble':$webSessionWidth,'hBubble':'auto','tipPosition':'tipTopLeft','qtipClass':'admin-qtip-access-parent another-".$key."'}";
	  			}else{
	  				$popupAddSessionVC    = "{'entityId':".$popupentityId.",'entityType':'".$popupAddSessionentityType."','url':'ajax/class-add-session/".core_encrypt($course_id)."/".core_encrypt($class_id)."/".$key."/Addanother/0/".core_encrypt($oldClassId)."/".$newlyCreated."','popupDispId':'add_more_".$popupAddSessionvisibPopupId."','catalogVisibleId':'qtipAddMoreSessionIdqtip_visible_disp_".$popupAddSessionIdInit."','wBubble':$otherSessionWidth,'hBubble':'auto','tipPosition':'tipTopLeft','qtipClass':'admin-qtip-access-parent another-".$key."'}";
	  			}
	  		if($i==0){
			   	$meetingList .= '<span>';
			   }
	  			$meetingList .= '<li id = '.$key.' onclick = "displayPubActionList(\'session-add-list\');$(\'#meeting-title\').html(\''.t($value).'\');$(\'#root-admin\').data(\'narrowsearch\').getQtipDiv('.$popupAddSessionVC.'); return false;">'
	  											.t($value).'</li>';


	  		}
	  		$i++;
	  	}
	  	if(!empty($keyFind)){
	  		$meetingTypeOrder['lrn_cls_vct_oth'] = $keyFind;
	  		$popupAddSessionVC    = "{'entityId':".$popupentityId.",'entityType':'".$popupAddSessionentityType."','url':'ajax/class-add-session/".core_encrypt($course_id)."/".core_encrypt($class_id)."/lrn_cls_vct_oth/Addanother/0/".core_encrypt($oldClassId)."/".$newlyCreated."','popupDispId':'add_more_".$popupAddSessionvisibPopupId."','catalogVisibleId':'qtipAddMoreSessionIdqtip_visible_disp_".$popupAddSessionIdInit."','wBubble':$otherSessionWidth,'hBubble':'auto','tipPosition':'tipTopLeft','qtipClass':'admin-qtip-access-parent another-lrn_cls_vct_oth'}";
	  		$meetingList .= '<li id ="lrn_cls_vct_oth" onclick="displayPubActionList(\'session-add-list\');$(\'#meeting-title\').html(\''.t($keyFind).'\');$(\'#root-admin\').data(\'narrowsearch\').getQtipDiv('.$popupAddSessionVC.'); return false;">'
	  										.t($keyFind).'</li>';
	  	}
	  	$meetingList .= '</span>';




	  	/**************************************************/

		  $form[$formName]['add_session'] = array(
			    '#type' => 'markup',
			    '#markup' => '<div class="session-add-another-popup-container"><div class="addedit-form-cancel-container-actions"><div class="add_session_popup" id="add_more_'.$popupAddSessionvisibPopupId.'" ></div><span class="add-custombtn-symbol admin-addanother-session-details-info">'.t('LBL386').'</span>&nbsp;<span class="chosen-meeting-type" id="meeting-title">&nbsp;</span><a class="add-another-arrow-link" onclick="if(document.getElementById(\'add-session-details\')!=null){return false;}else{displayPubActionList(\'session-add-list\')}">&nbsp;</a></div></div>',
		  );
		  $form[$formName]['add_session_vc'] = array(
	        '#type' => 'markup',
	        '#prefix' => '<div class="add-session-popup-list">
	        				<div class="add_session_popup" id="'.$popupAddSessionvisibPopupId.'" ></div>


		  								<ul id="" class="add-more-session-list session-add-list dropdown-dd-list"><div class="dropdown-dd-list-arrow"></div>'.
	  	$meetingList.'</ul></div>',
	  	);
		}
	}

	$saveAttributes = array();
	$saveAttributes['class'][] = 'admin-save-button-middle-bg';
	$saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
	expDebug::dPrint('checkin gthe clone button for ilt and vc'.$form_state["cloneEnroll"].'gggg'.$oldClassId.'///'.$startSession);
	if($form_state["cloneEnroll"] == 0  && $oldClassId != 0 && $startSession != 0){
		$valuespre = "<div class='clone-session-enrollment'>";
		$valuessuf = "</div>";
	}else{
		$valuespre = "<div class='clone-session-enrollment-none'>";
		$valuessuf = "</div>";
	}
	$saveAttributes['data-wrapperid'][] = 'catalog-class-basic-addedit-form-disp-'.core_encrypt($class_id);
	//$saveAttributes['onclick'][] = 'clearMessages();';
	$form[$formName]['clone_enrollment-class'] = array(
			'#id' => 'session-clone-class',
			'#type' => 'submit',
			'#prefix' => $valuespre."<div id='clone-selected-confirm'><div class='admin-save-button-left-bg'></div>",
			'#suffix' => "<div class='admin-save-button-right-bg'></div></div>".$valuessuf,
			'#value' => t('Clone Enrollment'),
			'#name' => 'clone_enrollment',
			'#attributes' => $saveAttributes,
			'#ajax' => array(
					'wrapper' => 'catalog-class-basic-addedit-form-'.core_encrypt($class_id),
					'callback' => 'addEditCatalogRepaintForm',
					//	'customButtonId'=>$esignHiddenButt,
					'method' => 'replace',
					'effect' => 'none', // 'fade',
					'event'=>'click',
					'keypress' => true,
					'progress' => array(
							'type' => 'throbber',
							'message' => ''
					),
			)
	);
	} catch (Exception $ex) {
		watchdog_exception('displaySessionDetails', $ex);
		expertusErrorThrow($ex);
	}
}

function cancelSessionDetails($form, &$form_state){
	try {
	$form_state['num_session'] = 0;

	$form_state['input']     = array(); // Clear user input - raw
	$form_state['values']    = array();

	$form_state['storage']['mode'] = '';
	// $form_state['num_session'] = $form_state['values']['hid_num_session_cnt'];
	$form_state['rebuild'] = TRUE;
	} catch (Exception $ex) {
		watchdog_exception('cancelSessionDetails', $ex);
		expertusErrorThrow($ex);
	}
}


/*
 * Get list of TimeZone in slt_profile_list_items
 */
function _getClassInfo($clsId){
	try {
	$select = db_select('slt_course_class', 'cls');
	$select->leftJoin('slt_location', 'loc','cls.location_id = loc.id');
	$select->addField('cls', 'min_seats','min_seats');
	$select->addField('cls', 'max_seats','max_seats');
	$select->addField('cls', 'waitlist_count','waitlist_count');
	$select->addField('cls', 'location_id','location_id');
	$select->addField('cls', 'delivery_type','delivery_type');
	$select->addField('loc', 'timezone','loc_timezone');


	$select->condition('cls.id', $clsId);
	$clsInfo = $select->execute()->fetchAll();

	return $clsInfo;
	} catch (Exception $ex) {
		watchdog_exception('_getClassInfo', $ex);
		expertusErrorThrow($ex);
	}
}


/*
 * Get list of TimeZone in slt_profile_list_items
 */
function _getLocationTimeZone($locId){
	try {
  $locTimezone = '';
  if($locId){
    $select = db_select('slt_location', 'loc');
    $select->addField('loc', 'timezone');
    $select->condition('loc.id', $locId);
    $locTimezone = $select->execute()->fetchField();
  }
  return $locTimezone;
  } catch (Exception $ex) {
  	watchdog_exception('_getLocationTimeZone', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * Get list of Room in class Facility
 */
function _getFacilityRoom($facilityId){
	try {
	$roomList = array();
	$roomList[''] = t('LBL674');
	if(empty($facilityId)){
		return $roomList;
	}
	$select = db_select('slt_classroom', 'room');
	$select->addField('room', 'id','roomid');
	$select->addField('room', 'name','roomname');
	$select->condition('room.facility_id', $facilityId);
	$select->condition('room.is_active', 1);
	$room = $select->execute()->fetchAll();
	foreach ($room as $id => $row) {
		$roomList[$row->roomid] = $row->roomname;
	}
	return $roomList;
	} catch (Exception $ex) {
		watchdog_exception('_getFacilityRoom', $ex);
		expertusErrorThrow($ex);
	}
}



/*
 * Get list of Instructor enrolled sessions
 */
function getInstructorEnrolledSession($instructorId){
	try {
  $select = db_select('slt_course_class_session', 'sess');
  $select->leftjoin('slt_course_class','cls','cls.id = sess.class_id');
  $select->leftjoin('slt_enrollment','enr','enr.class_id=cls.id');
  $select->leftjoin('slt_person','per','per.id = enr.user_id');
  $select->addField('sess', 'id','sessionid');
  $select->addField('sess', 'start_date');
  $select->addField('per', 'first_name');
  $select->addField('per', 'last_name');
  $select->addExpression("DATE_FORMAT((sess.start_date),'%Y-%m-%d')", 'ses_start_date');
  $select->addField('sess', 'start_time');
  $select->addField('sess', 'end_time');
  $select->addField('sess', 'timezone');
  $select->leftjoin('slt_profile_list_items', 'splt_timezone', 'splt_timezone.code=sess.timezone');
  $select->addField('splt_timezone','attr2', 'session_timezone');
  $select->condition('enr.reg_status', array('lrn_crs_reg_wtl','lrn_crs_reg_ppm','lrn_crs_reg_cnf'),'IN');
  $select->condition('enr.comp_status',array('lrn_crs_cmp_cmp','lrn_crs_cmp_inc','lrn_crs_cmp_nsw'),'NOT IN');
  $select->condition('cls.status', array('lrn_cls_sts_dld','lrn_cls_sts_del','lrn_cls_sts_can'),'NOT IN');
  $select->condition('cls.delivery_type', array('lrn_cls_dty_ilt','lrn_cls_dty_vcl'),'IN');
  $select->condition('enr.user_id', $instructorId);
  expDebug::dPrintDBAPI('getInstructorEnrolledSession -->', $select);
  $instructor = $select->execute()->fetchAll();
  return $instructor;
  } catch (Exception $ex) {
  	watchdog_exception('getInstructorEnrolledSession', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * Get list of Location Scheduled Session
 */
function getLocationScheduledSession($locationId,$sessId){
	try {
  $select = db_select('slt_course_class_session', 'sess');
  $select->leftjoin('slt_course_class','cls','cls.id = sess.class_id');
  $select->addField('sess', 'id','sessionid');
  $select->addField('sess', 'start_date');
  $select->addExpression("DATE_FORMAT((sess.start_date),'%Y-%m-%d')", 'ses_start_date');
  $select->addField('sess', 'start_time');
  $select->addField('sess', 'end_time');
  $select->leftjoin('slt_profile_list_items', 'splt_timezone', 'splt_timezone.code=sess.timezone');
  $select->addField('splt_timezone','attr2', 'session_timezone');
  $select->condition('cls.status', array('lrn_cls_sts_dld','lrn_cls_sts_del','lrn_cls_sts_can'),'NOT IN');
  $select->condition('sess.location_id', $locationId);
  if(!empty($sessId)){
	$select->condition('sess.id', $sessId,'<>');
  }
  expDebug::dPrintDBAPI('getLocationScheduledSession -->', $select);
  $location = $select->execute()->fetchAll();
  return $location;
  } catch (Exception $ex) {
  	watchdog_exception('getLocationScheduledSession', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addACFieldSessionToAddEditForm -  Use this function to add an autocomplete text fieldset to the addedit form
 */
function addACFieldSessionToAddEditForm(&$form, $formName, $id, $acMenuPath, $title = '', $defaultValue = '', $tabIndex = null, $dependsUponFields = array(), $addlDependencies = array(), $mandatory = false, $readOnly = false,$deliveryType,$meetType,$preTextValue='') {
  try {
	$attributes = array();
	$autocompleteWithId=false;
	$attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

	// We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
	// each time the form is re-built.
	$attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

	// Add a classes to apply common style to all autocomplete fields
	$attributes['class'][] = 'addedit-edit-ac-textfield';
	$attributes['class'][] = 'ac_input';
	$attributes['class'][] = 'admin-session-detail-textfield';

	if ($readOnly) {
		$attributes['readonly'] = 'readonly';
		$attributes['class'][] = 'addedit-readonly-textfield'; // Add another class
	} else {
		$extraParams = '';
		foreach ($dependsUponFields as $dependancy) {
			$extraParams .= (($extraParams == '')? '' : ', ') .
			$dependancy .
                            ': function() { return jQuery(".addedit-edit-' .
			$dependancy . '").val(); }';
		}

		foreach ($addlDependencies as $paramName => $paramValue) {
			if($paramName=='id'){ // Added by vincent on 26 Mar, 2012 to handle result id in autocomplete
				$autocompleteWithId=true;
			}
			$extraParams .= (($extraParams == '')? '' : ', ') .
			$paramName . ': ' . $paramValue;
		}

		if($preTextValue == t('LBL181')) {
			$readingTypeClass = 'input-field-grey';
			$defaultValue = $preTextValue;
			$attributes['class'][] = $readingTypeClass; // Add another class
			$attributes['onblur'][] = 'textfieldTitleBlur(this, "'.$preTextValue.'",\'\',1);';
			$attributes['onfocus'][] = 'textfieldTitleClick(this, "'.$preTextValue.'");';

		}
		expDebug::dPrint('$extraParams = '. print_r($extraParams, true), 4);


		if($autocompleteWithId==true){ // Added by vincent on 26 Mar, 2012 to show text in UI when use autocomplete with id
			$attributes['onfocus'][] = '(function ($) {' .
    		                        'jQuery(".addedit-edit-' . $id . '").unautocomplete();' .
                                'jQuery(".addedit-edit-' . $id . '").autocomplete("/?q=' . $acMenuPath . '", ' .
                                                    '{' .
                                                      'minChars: 3, ' .
                                                      'max: 50, ' .
                                                      'autoFill: true, ' .
                                                      'mustMatch: false, ' .
                                                      'matchContains: false, ' .
    																									'formatItem:function(pmRec){return pmRec[1]},'.
                                                      'inputClass: "ac_input", ' .
                                                      'loadingClass: "ac_loading", ' .
                                                      'extraParams: {' . $extraParams . '}' .
                                                     '}' .
                                                  ');' .
                                          '}' .
                               ')(jQuery);';
		}else{
			$attributes['onfocus'][] = '(function ($) {' .
     		                        'jQuery(".addedit-edit-' . $id . '").unautocomplete();' .
                                'jQuery(".addedit-edit-' . $id . '").autocomplete("/?q=' . $acMenuPath . '", ' .
                                                    '{' .
                                                      'minChars: 3, ' .
                                                      'max: 50, ' .
                                                      'autoFill: true, ' .
                                                      'mustMatch: false, ' .
                                                      'matchContains: false, ' .
                                                      'inputClass: "ac_input", ' .
                                                      'loadingClass: "ac_loading", ' .
                                                      'extraParams: {' . $extraParams . '}' .
                                                     '}' .
                                                  ');' .
                                          '}' .
                               ')(jQuery);';
		}
	}
	expDebug::dPrint(' 	$deliveryType = ' . print_r($deliveryType, true) , 4);
  $colspan = ($deliverytype=='lrn_cls_dty_ilt' || ($deliveryType == 'lrn_cls_dty_vcl' && $meetType != 'lrn_cls_vct_web'))? '2' : '1';

	$form[$formName]['session_details']['session_details_name_'.$id] = array(
        '#title' => $title,
        '#type' => 'textfield',
        '#prefix' => '<td colspan="'.$colspan.'" class="admin-addedit-class-instructor-name">',
        '#suffix' => '</td>',
        '#default_value' => $defaultValue,
        '#attributes' => $attributes,
        '#disabled' => $readOnly,
  		'#size' => 59,
	);
	} catch (Exception $ex) {
		watchdog_exception('addACFieldSessionToAddEditForm', $ex);
		expertusErrorThrow($ex);
	}
}

function adminGetDeliveryType($classid){
	try {
	$select = db_select('slt_course_class', 'course');
	$select->addField('course','delivery_type');
	//$select->addField('course','user_id');
	$select->condition('course.id', $classid);
	$deliveryTypes = $select->execute()->fetchField();
	return $deliveryTypes;
	} catch (Exception $ex) {
		watchdog_exception('adminGetDeliveryType', $ex);
		expertusErrorThrow($ex);
	}
}

//API for ILT session creation
function addNewSessionByRestAPI( &$form_state,$createuserid="")
{
	try {
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	$form=array();
	$form["form_id"]="";
	$classId = $form_state['values']['class_id'];
	$delivery_type = array('lrn_cls_dty_vcl','lrn_cls_dty_ilt'); //0073051: Able to Create session for WBT/VOD class via API
	$select = db_select('slt_course_class', 'class');
	$select->addField('class','min_seats','min_seats');
	$select->addField('class','max_seats','max_seats');
	$select->addField('class','waitlist_count','waitlist_count');
	$select->addField('class','location_id','class_location');
	$select->condition('class.id', $classId);
	$select->condition('class.delivery_type',$delivery_type,'IN'); //0073051: Able to Create session for WBT/VOD class via API
	$select->range(0,1);
	$classDetail = $select->execute()->fetchAssoc();
	if(!$classDetail){                                             //0073051: Able to Create session for WBT/VOD class via API
		$error -> errormsg = t('Session cannot be created for WBT/VOD class');
		$error -> isValidateError = true;
		return $error;
	}
   $locationTz = '';
   if($classDetail['class_location']){
      $select = db_select('slt_location', 'loca');
      $select->addField('loca','timezone');
      $select->condition('loca.id', $classDetail['class_location']);
      $locationTz = $select->execute()->fetchField();
   }
 	$form_state['values']['ses_min_seats'] = $classDetail['min_seats'];
	$form_state['values']['ses_max_seats'] = $classDetail['max_seats'];
	$form_state['values']['ses_waitlist_count'] = $classDetail['waitlist_count'];
	$form_state['values']['ses_location_id'] = $classDetail['class_location'];
	$form_state['values']['ses_loc_timezone'] = $locationTz;
	$form_state['values']['create_user_id'] = $createuserid;
	$form_state['meet_type']= "Ilt";
	$form_state['input']['hid_instructor_id'] = $form_state['values']['session_details_fieldset']['session_details']['instructor_id'];
	$form_state['input']['session_details_fieldset']['session_details']['session_details_name_instructor_0'] = 'dummy';
	$form_state['num_session'] = 1;
	$form_state['is_new'] = "Add";
	$form_state['call_from'] = "API";
	$form_state['rec_sess_enabled'] = true;
	$days = explode(',', trim($form_state['values']['days']));
	foreach($days as $day){
	$form_state['input'][$day] = 1;
	}
	expDebug::dPrint(' $$affectorOptions12333 = ' . print_r($days, true) , 4);
	$form_state['values']['session_name'] = mb_strimwidth(stripslashes($form_state['values']['session_name']), 0, 50);//Fix for 65843
	//46173: PS - Create ILT class session
	$form_state['values']['session_name'] = stripslashes($form_state['values']['session_name']);
	$error= (object)array();
	if(!validateDateFormat($form_state['values']['start_date'])){
		$error -> errormsg = t('ERR171').' '.t('LBL045');
		$error -> isValidateError = true;
		return $error;
	}
	if(!validateDateFormat($form_state['values']['end_date'])){
		$error -> errormsg = t('ERR171').' '.t('LBL100');
		$error -> isValidateError = true;
		return $error;
	}
	if(!isActiveClass($form_state['values']['course_id'],$form_state['values']['class_id'],true)){
		$error -> errormsg = t('Invalid course or class id');
		$error -> isValidateError = true;
		return $error;
	}
	//#50203: API allows to create multiple ILT session in a single Class modified by joolavasavi
 	$enrollcount = getTotalClassEnroll($classId);
 	expDebug::dPrint('$result $enrollcount = ' .$enrollcount , 4);
	
	if($enrollcount > 0 ){
		$error -> errormsg = 'Session cannot be added as there are active enrollments for this class.';
		$error -> isValidateError = true;
		return $error;
	}
	$ValidDays = array('Mon','Tue','Wed','Thu','Fri','Sat','Sun');
	if(!empty(trim($form_state['values']['days'])) && count(array_intersect($days, $ValidDays)) != count($days)){ //
		$error -> errormsg = 'Invalid Day or Days';
		$error -> isValidateError = true;
		return $error;
	}
	add_class_session_html_validate($form, $form_state);
	$validate = form_get_errors();
	if(count($validate)){
		return array((object)array('id'=>implode(',' ,$validate)));
	}
	$sessionId =  add_class_session_html_submit($form, $form_state) ;
	expDebug::dPrint(' addNewSessionByRestAPI created session:' . $sessionId , 4);
	return $sessionId;
	} catch (Exception $ex) {
		watchdog_exception('addNewSessionByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}

//API for ILT session creation
function updateSessionByRestAPI( &$form_state,$createuserid="")
{
	try {
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	$form=array();
	$form["form_id"]="";
	$sessId = $form_state['values']['hid_session_id'];
	$select = db_select('slt_course_class_session', 'sess');
	$select->addField('sess','course_id','course_id');
	$select->addField('sess','class_id','class_id');
	$select->addField('sess','capacity_min','min_seats');
	$select->addField('sess','capacity_max','max_seats');
	$select->addField('sess','waitlist_count','waitlist_count');
	$select->addField('sess','location_id','class_location');
	$select->addField('sess','timezone','ses_timezone');
	$select->condition('sess.id', $sessId);
	$select->range(0,1);
	$sessDetail = $select->execute()->fetchAssoc();
	if(empty($sessDetail)) {
		form_set_error('','Invalid Id');
	}
	if($sessDetail['class_id'] != $form_state['values']['class_id'] ){
		form_set_error('','Session ID and Class ID donot match');
	}
	$form_state['values']['course_id'] = $sessDetail['course_id'];
	$form_state['values']['class_id'] = $sessDetail['class_id'];
	$form_state['values']['ses_min_seats'] = $sessDetail['min_seats'];
	$form_state['values']['ses_max_seats'] = $sessDetail['max_seats'];
	$form_state['values']['ses_waitlist_count'] = $sessDetail['waitlist_count'];
	$form_state['values']['ses_location_id'] = $sessDetail['class_location'];
	$form_state['values']['ses_loc_timezone'] = $sessDetail['ses_timezone'];
	$form_state['values']['create_user_id'] = $createuserid;
	$form_state['input']['hid_instructor_id'] = $form_state['values']['session_details_fieldset']['session_details']['instructor_id'];
	$form_state['input']['session_details_fieldset']['session_details']['session_details_name_instructor_0'] = 'dummy';
	$form_state['num_session'] = 1;
	$form_state['meet_type']= "Ilt";
	$form_state['is_new'] = "Edit";
	$form_state['call_from'] = "API";
	add_class_session_html_validate($form, $form_state);
	$validate = form_get_errors();
	if(count($validate)){
		return array((object)array('id'=>implode(',' ,$validate)));
	}
	// #53306: API allows to edit ILT session with an enrolled user modified by joolavasavi
	$enrolledcount = getTotalClassEnroll($sessDetail['class_id']);
	expDebug::dPrint('$result $enrolledcount = ' .$enrolledcount , 4);
	$error=(object)array();
	if($enrolledcount > 0 ){
		$error -> errormsg = 'Session cannot be updated as there are active enrollments for this class.';
		$error -> isValidateError = true;
		return $error;
	}
	$sessionId =  add_class_session_html_submit($form, $form_state) ;
	expDebug::dPrint(' updateSessionByRestAPI updated session:' . $sessionId , 4);
	return $sessionId;
	} catch (Exception $ex) {
		watchdog_exception('updateSessionByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function checkFIeldInProfileList($val, $code) {
	$val = trim($val);
	if($val == "")
		return true;

	$select = db_select('slt_profile_list_items', 'spli');
	$select->addField('spli','id','id');
	$select->condition('spli.code', $val, '=');
	$select->condition('spli.is_active', 'Y', '=');
	$select->condition('spli.code', $code.'%', 'LIKE');
	$result = $select->execute();
	if($result->rowCount() <= 0){
		return true;
	}
	return false;
}

//API for VC session creation
function addNewVCSessionByRestAPI( &$form_state,$createuserid="")
{
	$error=(object)array();
	try {
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	$form=array();
	$form["form_id"]="";
	$classId = $form_state['values']['class_id'];
	$select = db_select('slt_course_class', 'class');
	$select->addField('class','min_seats','min_seats');
	$select->addField('class','max_seats','max_seats');
	$select->addField('class','waitlist_count','waitlist_count');
	$select->condition('class.id', $classId);
	$select->range(0,1);
	$classDetail = $select->execute()->fetchAssoc();
	expDebug::dPrint("Add new session class". print_r($form_state,true),5);
	$form_state['values']['ses_min_seats'] = $classDetail['min_seats'];
	$form_state['values']['ses_max_seats'] = $classDetail['max_seats'];
	$form_state['values']['ses_waitlist_count'] = $classDetail['waitlist_count'];
	$form_state['input']['hid_instructor_id'] = $form_state['values']['session_details_fieldset']['session_details']['instructor_id'];
	$form_state['values']['session_presenter'] = $form_state['values']['session_details_fieldset']['session_details']['presenter_id'];
	$form_state['input']['session_details_fieldset']['session_details']['session_details_name_instructor_0'] = 'dummy';
	$form_state['values']['create_user_id'] = $createuserid;
	$form_state['values']['time_zone_list'] = $form_state['values']['session_details_fieldset']['session_details']['time_zone'];
	$form_state['num_session'] = 1;
	$form_state['is_new'] = "Add";
	$form_state['call_from'] = "API";
	$form_state['meet_type'] = $form_state['values']['session_details_fieldset']['session_details']['meeting_type'];
	$form_state['values'] = trimInputFields($form_state['values']); // #42124 - Trim input values
	$form_state['values']['session_name'] = stripslashes($form_state['values']['session_name']);
	$form_state['rec_sess_enabled'] =  false;
	if(($form_state['is_new'] == "Addanother" || $form_state['is_new'] == "Add") && $form_state['meet_type'] != 'lrn_cls_vct_oth'){
		$form_state['rec_sess_enabled'] =  true;
	}
	if($form_state['meet_type']=="lrn_cls_vct_oth"){
		 $form_state['values']['days'] = '';
	     $form_state['values']['end_date'] = '';
		
	}
	$days = explode(',', $form_state['values']['days']);
	foreach($days as $day){
		$form_state['input'][$day] = 1;
	}
	
	if ( ($form_state['meet_type']!="lrn_cls_vct_oth") && (empty($form_state['values']['end_date']) || $form_state['values']['end_date'] =='mm-dd-yyyy') ){
		$error -> errormsg = t('LBL100').t('ERR101');
		$error -> isValidateError = true;
		return $error;
	}
	if(!isActiveClass($form_state['values']['course_id'],$form_state['values']['class_id'],true)){
		$error -> errormsg = t('Invalid course or class id');
		$error -> isValidateError = true;
		return $error;
	}
	$time_zone = $form_state['values']['time_zone_list'];
	$meet_type = $form_state['meet_type'];
	
	if(checkFIeldInProfileList($time_zone, 'cre_sys_tmz_')){
		$error -> errormsg = t('Invalid time_zone');
		$error -> isValidateError = true;
		return $error;
	}
	if(checkFIeldInProfileList($meet_type, 'lrn_cls_vct_')){
		$error -> errormsg = t('Invalid meeting_type');
		$error -> isValidateError = true;
		return $error;
	}
	if($form_state['meet_type']!="lrn_cls_vct_oth" && !validateDateFormat($form_state['values']['end_date'])){
		$error -> errormsg = t('ERR243');
		$error -> isValidateError = true;
		return $error;
	}
	if(!validateDateFormat($form_state['values']['start_date'])){
		$error -> errormsg = t('ERR243');
		$error -> isValidateError = true;
		return $error;
	}
	$ValidDays = array('Mon','Tue','Wed','Thu','Fri','Sat','Sun');
	if($form_state['meet_type']!="lrn_cls_vct_oth" && (!empty($form_state['values']['days']) && count(array_intersect($days, $ValidDays)) != count($days))){ //
		$error -> errormsg = 'Invalid Day or Days';
		$error -> isValidateError = true;
		return $error;
	}
	
	add_class_session_html_validate($form, $form_state);
	$validate = form_get_errors();
	if(count($validate)){
		return array((object)array('id'=>implode(',' ,$validate)));
	}
	$sessionId =  add_class_session_html_submit($form, $form_state) ;
	expDebug::dPrint(' addNewVCSessionByRestAPI created session:' . $sessionId , 4);
	return $sessionId;
	} catch (Exception $ex) {
		watchdog_exception('addNewVCSessionByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}

//API for VC session creation
function updateVCSessionByRestAPI( &$form_state,$createuserid="")
{
	try {
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	$form=array();
	$form["form_id"]="";
	$sessId = $form_state['values']['hid_session_id'];
	$select = db_select('slt_course_class_session', 'sess');
	$select->addField('sess','course_id','course_id');
	$select->addField('sess','class_id','class_id');
	$select->addField('sess','capacity_min','min_seats');
	$select->addField('sess','capacity_max','max_seats');
	$select->addField('sess','waitlist_count','waitlist_count');
	$select->addField('sess','location_id','class_location');
	$select->condition('sess.id', $sessId);
	$select->range(0,1);
	$sessDetail = $select->execute()->fetchAssoc();
	if(empty($sessDetail)) {
		form_set_error('','Invalid Id');
	}
	$form_state['values']['course_id'] = $sessDetail['course_id'];
	$form_state['values']['class_id'] = $sessDetail['class_id'];
	$form_state['values']['ses_min_seats'] = $sessDetail['min_seats'];
	$form_state['values']['ses_max_seats'] = $sessDetail['max_seats'];
	$form_state['values']['ses_waitlist_count'] = $sessDetail['waitlist_count'];
	$form_state['values']['create_user_id'] = $createuserid;
	$form_state['values']['session_presenter'] = $form_state['values']['session_details_fieldset']['session_details']['presenter_id'];
	$form_state['input']['hid_instructor_id'] = $form_state['values']['session_details_fieldset']['session_details']['instructor_id'];
	$form_state['input']['session_details_fieldset']['session_details']['session_details_name_instructor_0'] = 'dummy';
	$form_state['num_session'] = 1;
	$form_state['values']['time_zone_list'] = $form_state['values']['session_details_fieldset']['session_details']['time_zone'];
	$form_state['is_new'] = "Edit";
	$form_state['call_from'] = "API";
	$form_state['meet_type'] = $form_state['values']['session_details_fieldset']['session_details']['meeting_type'];
	$form_state['values'] = trimInputFields($form_state['values']); // #42124 - Trim input values
	$time_zone = $form_state['values']['time_zone_list'];
	$meet_type = $form_state['meet_type'];
	
	if(checkFIeldInProfileList($time_zone, 'cre_sys_tmz_')){
		$error -> errormsg = t('Invalid time_zone');
		$error -> isValidateError = true;
		return $error;
	}
	if(checkFIeldInProfileList($meet_type, 'lrn_cls_vct_')){
		$error -> errormsg = t('Invalid meeting_type');
		$error -> isValidateError = true;
		return $error;
	}
	add_class_session_html_validate($form, $form_state);
	$validate = form_get_errors();
	if(count($validate)){
		return array((object)array('id'=>implode(',' ,$validate)));
	}
	// #53306: API allows to edit VC session with an enrolled user modified by joolavasavi
	$enrolledVCcount = getTotalClassEnroll($sessDetail['class_id']);
	expDebug::dPrint('$result $enrolledVCcount = ' .$enrolledVCcount , 4);
	if($enrolledVCcount > 0 ){
		$error -> errormsg = 'Session cannot be updated as there are active enrollments for this class.';
		$error -> isValidateError = true;
		return $error;
	}
	$sessionId =  add_class_session_html_submit($form, $form_state) ;
	expDebug::dPrint(' addNewVCSessionByRestAPI updated session:' . $sessionId , 4);
	return $sessionId;
	} catch (Exception $ex) {
		watchdog_exception('updateVCSessionByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function meeting_detail_virtual_class($classId,$sessionId){

	try {
		$form_state = array(
   'no_redirect'  => TRUE,
   'build_info'   => array('args' => array('ajax')),
   'entity_id'    => $classId,
   'class_id' => $classId,
		//'entity_type'  => $entityType,
   'session_id_more'	  => $sessionId,
		//'session_form' => $sessionFormName
		);
		$formId                 = 'meeting_detail_virtual_class_html_form';
		//$builtForm              = drupal_build_form($formId, $form_state, $entityId);
		//$builtForm =  meeting_detail_virtual_class_html_form($form, $form_state, $courseId, $entityId, $entityType, $uniqueId, $clsDeliveryType, $readOnlyForView);
		$builtForm              = drupal_build_form($formId, $form_state);
		//print_r($builtForm);
		$renderedTabContentHTML = drupal_render($builtForm);
		$drupalSettings         = getDrupalSettings();

		$response       = array(
    'drupal_settings'     => $drupalSettings,
    'render_content_main' => $renderedTabContentHTML,
		);
		echo drupal_json_output($response);
		exit;
	} catch (Exception $ex) {
		watchdog_exception('meeting_detail_virtual_class', $ex);
		expertusErrorThrow($ex);
	}

}

function meeting_detail_virtual_class_html_form($form, &$form_state) {
	try {
	$class_id = $form_state['entity_id'];
	$form['addedit-vc_start'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="addedit-vc_start">',
	);
	meeting_detail_virtual_class_html_form_display($form, $form_state);
	$form['addedit-vc_end'] = array(
    '#type' => 'markup',
    '#markup' => '</div>',
	);
	return $form;
	} catch (Exception $ex) {
		watchdog_exception('meeting_detail_virtual_class_html_form', $ex);
		expertusErrorThrow($ex);
	}
}

function meeting_detail_virtual_class_html_form_display(&$form, &$form_state,$readonly = false) {
 try {
	$i = 0;
	$class_id = $form_state['class_id'];
	$defaultValueObj = _getSessionDetails($class_id,'',$form_state['session_id_more']);
	expDebug::dPrint('entering for more details'.$defaultValueObj[0]->meeting_type);
	$AttPassArray = explode('/',$defaultValueObj[0]->session_attendeeurl);
	$PrePassArray = explode('/',$defaultValueObj[0]->session_presenterurl);
	$AttPassLen = count($AttPassArray);
	$PrePassLen = count($PrePassArray);
	$AttendeePass =  $AttPassArray[$AttPassLen - 1];
	$PrePass =  $PrePassArray[$PrePassLen - 1];
	$meetId =  $AttPassArray[$AttPassLen - 2];
	$formName = 'meeting_detail_virtual_class_html_form';
	expDebug::dPrint('meeting type '.$defaultValueObj[0]->meeting_type);
	$form[$formName]['session_details']['session_title_row_start'] = array(
		'#type' => 'markup',
		'#markup' => '<div id="meeting-container" class="meeting-session-container"><table cellpadding="0" cellspacing="0" id="admin-addedit-session-vc-details-wrapper" class="admin-addedit-session-details-wrapper admin-addedit-session-details-meeting" border="0">',
	);
	$form[$formName]['session_details']['more_form_load'] = array(
     '#type' => 'markup',
     '#prefix' => '<div style="display:none"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="vtip();" width="100" height="132" /></div></div>',
	);
	$form[$formName]['session_details']['sd_firstmeet_row_start'] = array(
		'#type' => 'markup',
		'#markup' => '<tr>',
	);
	$timeZoneList = _getTimeZone();
	$optTZ['']= t('LBL674');
	foreach ($timeZoneList as $id => $row) {
		$zoneName = explode(')',$row->zonename);
		$timeZone[$row->zonecode] = $row->zonename;//str_replace('(','',$zoneName[0]);;
	}

	$form[$formName]['session_details']['time_zone_list_more'] = array(
		'#id' => 'time_zone_list_more',
		'#type' => 'markup',
		'#markup' => '<td class="admin-more-edit-session-column">'.t('LBL297').':</td> <td>'.$defaultValueObj[0]->timezone_name.'</td>',
	);
	$form[$formName]['session_details']['sd_firstmeet_row_end'] = array(
		'#type' => 'markup',
		'#markup' => '</tr>',
	);


	/*if($defaultValueObj[0]->meeting_type == "lrn_cls_vct_web"){
		$form[$formName]['session_details']['sd_secondmeet_row_start'] = array(
			'#type' => 'markup',
			'#markup' => '<tr id="attendee_password_more">',
		);
		$form[$formName]['session_details']['session_attende_password_more'] = array(
			'#id' => 'session_attende_password_more',
			'#type' => 'markup',
			'#markup' => '<td class="admin-more-edit-session-column admin-addedit-class-time-zone">'.t('LBL859').':</td> <td class="admin-addedit-class-time-zone">'.$AttendeePass.'</td>',
		);
		$form[$formName]['session_details']['sd_secondmeet_row_end'] = array(
			'#type' => 'markup',
			'#markup' => '</tr>',
		);

		$form[$formName]['session_details']['sd_thirdmeet_row_start'] = array(
			'#type' => 'markup',
			'#markup' => '<tr id="presenter_password_more">',
		);
		$form[$formName]['session_details']['session_presenter_password_more'] = array(
			'#id' => 'session_presenter_password_more',
			'#type' => 'markup',
			'#markup' => '<td class="admin-more-edit-session-column admin-addedit-class-time-zone">'.t('LBL860').':</td> <td class="admin-addedit-class-time-zone">'.$PrePass.'</td>',
		);
		$form[$formName]['session_details']['sd_thirdmeet_row_end'] = array(
			'#type' => 'markup',
			'#markup' => '</tr>',
		);
	}*/

	if($defaultValueObj[0]->meeting_type == "lrn_cls_vct_oth"){
		$form[$formName]['session_details']['sd_attendeeurl_row_start'] = array(
			'#type' => 'markup',
			'#markup' => '<tr id="attendee_url_more">',
		);
		$form[$formName]['session_details']['session_attendee_url_more'] = array(
			'#id' => 'session_attendee_url_more',
			'#type' => 'markup',
			'#markup' => '<td class="admin-more-edit-session-column admin-addedit-class-time-zone">'.t('LBL296').':</td> <td class="admin-addedit-class-time-zone"><span class="vtip" title="'.rawurldecode($defaultValueObj[0]->session_attendeeurl).'">'.titleController('EXP-SP-ADMIN-SESSION-DETAILS-ATTENDEEURL',rawurldecode($defaultValueObj[0]->session_attendeeurl),40).'</span></td>',
		);
		$form[$formName]['session_details']['sd_attendeeurl_row_end'] = array(
			'#type' => 'markup',
			'#markup' => '</tr>',
		);

		$form[$formName]['session_details']['sd_presenterurl_row_start'] = array(
			'#type' => 'markup',
			'#markup' => '<tr id="presenter_url_more">',
		);

		$form[$formName]['session_details']['session_presenter_url_more'] = array(
			'#id' => 'session_presenter_url_more',
			'#type' => 'markup',
			'#markup' => '<td class="admin-more-edit-session-column admin-addedit-class-time-zone">'.t('LBL977').':</td> <td class="admin-addedit-class-time-zone"><span class="vtip" title="'.rawurldecode($defaultValueObj[0]->session_presenterurl).'">'.titleController('EXP-SP-ADMIN-SESSION-DETAILS-PRESENTERURL',rawurldecode($defaultValueObj[0]->session_presenterurl),40).'</span></td>',
		);
		$form[$formName]['session_details']['sd_presenterurl_row_end'] = array(
			'#type' => 'markup',
			'#markup' => '</tr>',
		);
	}

	$form[$formName]['session_details']['sd_fourthmeet_row_start'] = array(
		'#type' => 'markup',
		'#markup' => '<tr>',
	);

	$form[$formName]['session_details']['hid_instructor_id_more'] = array (
		'#id' => 'hid_instructor_id_more',
		'#type' => 'hidden',
		'#default_value' => $defaultValueObj[0]->instructor_id,
		'#attributes' => array('id'=>'hid_instructor_id_more')
	);
	$fullName = '';
	if(!empty($defaultValueObj[0]->instructor_id)){
		$user_detail=db_query("select group_concat(Distinct(full_name) SEPARATOR ' , ' ) as full_name from slt_person WHERE id IN (".$defaultValueObj[0]->instructor_id.")");
		expDebug::dPrintDBAPI('featch user details = ', $user_detail);
	$userDetail = $user_detail->fetchAll();
		expDebug::dPrint('user names----->'.print_r($userDetail,1),4);
	$fullName = $userDetail[0]->full_name;
	}
	$form[$formName]['session_details']['session_instructor_more'] = array(
		'#id' => 'session_instructor_more',
		'#type' => 'markup',
		'#markup' => '<td class="admin-more-edit-session-column admin-addedit-class-time-zone">'.t('Instructor').':</td> <td class="admin-addedit-class-time-zone vtip" title="'.$fullName.'">'.titleController('INSTRUCTOR-FULLNAME',$fullName,100).'</td>',
		'#maxlength' => 200,
	);

	$form[$formName]['session_details']['sd_fourthmeet_row_end'] = array(
		'#type' => 'markup',
		'#markup' => '</tr>',
	);

	$form[$formName]['session_details']['sd_sixthmeet_row_start'] = array(
			'#type' => 'markup',
			'#markup' => '<tr>',
	);
	
	$form[$formName]['session_details']['hid_presenter_id_more'] = array (
			'#id' => 'hid_presenter_id_more',
			'#type' => 'hidden',
			'#default_value' => $defaultValueObj[0]->presenter_id,
			'#attributes' => array('id'=>'hid_instructor_id_more')
	);
	$fullName = '';
	if(!empty($defaultValueObj[0]->presenter_id)){
		$user_detail=db_query("select full_name from slt_person WHERE id = ".$defaultValueObj[0]->presenter_id);
		$userDetail = $user_detail->fetchAll();
		$fullName = $userDetail[0]->full_name;
	}
	
	$type = $defaultValueObj[0]->meeting_type == 'lrn_cls_vct_web' ? t('LBL1314'): ($defaultValueObj[0]->meeting_type == 'lrn_cls_vct_exp' ? t('LBL1315'):t('Presenter')); 
	$form[$formName]['session_details']['session_presenter_more'] = array(
			'#id' => 'session_presenter_more',
			'#type' => 'markup',
			'#markup' => '<td class="admin-more-edit-session-column admin-addedit-class-time-zone">'.$type.':</td> <td class="admin-addedit-class-time-zone vtip" title="'.$fullName.'">'.titleController('INSTRUCTOR-FULLNAME',$fullName,100).'</td>',
			'#maxlength' => 200,
	);
	
	$form[$formName]['session_details']['sd_sixthmeet_row_start'] = array(
			'#type' => 'markup',
			'#markup' => '</tr>',
	);
	
	
	$form[$formName]['session_details']['sd_fifthmeet_row_start'] = array(
		'#type' => 'markup',
		'#markup' => '<tr>',
	);


	$cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-action-button-middle-bg white-btn-bg-middle';
	$cancelAttributes['onclick'] = '$(\'.active-qtip-div\').hide();return false;';

	$form[$formName]['cancel_content_edit_mode'] = array(
      '#type' => 'submit',
      '#value' => t('LBL123'),//close
      '#title' => t('LBL123'),
      '#prefix' => '<td>&nbsp;</td><td class="adddit-form-cancel-session-colse-btn addedit-form-cancel-container-actions admin-save-button-container"><div class="white-btn-bg-left"></div>',
	  '#suffix' => '<div class="white-btn-bg-right"></div></td>',
      '#attributes' => $cancelAttributes,
      '#ajax' => array(
         'event'=>'click',
	),
	);

	$form[$formName]['session_details']['sd_fifthmeet_row_end'] = array(
	'#type' => 'markup',
	'#markup' => '</tr>',
	);

	$div  = '</table></div>';

	$form[$formName]['session_title_row_end'] = array(
	'#type' => 'markup',
	'#markup' => $div,
	);

	return $form;
	} catch (Exception $ex) {
		watchdog_exception('meeting_detail_virtual_class_html_form_display', $ex);
		expertusErrorThrow($ex);
	}
}



function getClassAddSessionDisplay($courseId,$classId,$meetType,$isNew,$sessionIdForEdit,$oldClassId,$classType){
	try {

		$form_state = array(
	   		'no_redirect'  => TRUE,
	    	'build_info'   => array('args' => array('ajax')),
	   		'course_id'    => $courseId,
	        'class_id'      => $classId,
	    	'meet_type'	   => $meetType,
	    	'is_new'	   => $isNew,
	    	'session_id_foredit' => $sessionIdForEdit,
			'oldClassId'	=>$oldClassId,
			'cloneEnroll' => ($oldClassId !=0) ? 0 : 1,
			'classType' => $classType
		);

		$formId                 = 'add_class_session_html';
		$builtForm              = drupal_build_form($formId, $form_state);
		$renderedTabContentHTML = drupal_render($builtForm);
		$drupalSettings         = getDrupalSettings();

		$commerceResponse = array(
        'drupal_settings'     => $drupalSettings,
        'render_content_main' => $renderedTabContentHTML
		);





		echo drupal_json_output($commerceResponse);
		exit;


	} catch (Exception $ex) {
		watchdog_exception('getClassAddSessionDisplay', $ex);
		expertusErrorThrow($ex);
	}

}

/******************** For Web-Ex Feature *******************
 * Author: N.Rajesh
* Date: 25-MAR-2016
* Team: PS-Consulting
* Function Names: getPresenterCredentials, getPresenterDisplay, add_presenter_html, add_presenter_callback, add_presenter_html_validate, add_presenter_html_submit and add_class_session_html
*******************************************/

function getPresenterCredentials($presenterid) {

	try {

		$loggedUserId = getSltpersonUserId();
		$webex_module=getProfileListItemByCode('lrn_cls_vct_web');
		
		if ($loggedUserId == $presenterid && variable_get('webex_User') == 1  && $webex_module[0]->status == 'Y') {
			$presenter = getPresenterWebex($presenterid);

			expDebug::dPrint(' $select getPresenterWebex result = '. print_r($presenter,true),5);

			if ($presenter[0]->webex_name =="" || $presenter[0]->webex_pass =="") {
					echo "1";
			}
		} else {
			 		echo "0";
		}

	}  catch (Exception $ex) {
		watchdog_exception('getPresenterCredentials', $ex);
		expertusErrorThrow($ex);
	}
}

function getPresenterDisplay(){

	try {
		
	$form_state = array(
			'no_redirect'  => TRUE,
			'build_info'   => array('args' => array('ajax'))
	);
	
		$formId                 = 'add_presenter_html';
		$builtForm              = drupal_build_form($formId, $form_state);
		$renderedTabContentHTML = drupal_render($builtForm);
		$drupalSettings         = getDrupalSettings();

		$commerceResponse = array(
        'drupal_settings'     => $drupalSettings,
        'render_content_main' => $renderedTabContentHTML
		);

		echo drupal_json_output($commerceResponse);
		exit;


	} catch (Exception $ex) {
		watchdog_exception('getPresenterDisplay', $ex);
		expertusErrorThrow($ex);
	}

}

function add_presenter_html($form, &$form_state) {
	try {

		expDebug::dPrint("auto close variable back ".$form_state['autoclose'],5);
		
		$form = array();
		//$formWrapperId = 'add-presenter-credentials';
		$uniqueId = "webex-presenter";
		$wrapperId="add-edit-presenter".$uniqueId;
		$formName = "add_presenter";
		
		
		$form['#prefix'] = '</form>';
		//$form['#suffix'] = '</div>';
		
		$form[$formName]['add_presenter']['session_heading_row_start'] = array(
				'#type' => 'markup',
				'#markup' => '<div id="add-presenter-details"><div id="meeting-type"><div id="show_webex_detail_message"></div><table cellpadding="0" cellspacing="0" class="admin-addedit-presenter-details-wrapper" border="0">',
		);
		
		$form[$formName]['add_presenter']['sd_first_row_start'] = array(
				'#type' => 'markup',
				'#markup' => '<tr>',
				'#prefix' => '<td class="admin-addedit-presenter-title">'.t('LBL3012'),
				'#suffix' => '</td>',
		);
		
		$form[$formName]['add_presenter']['sd_first_row_end'] = array(
				'#type' => 'markup',
				'#markup' => '</tr>',
		);
		
		$form[$formName]['add_presenter']['sd_second_row_start'] = array(
				'#type' => 'markup',
				'#markup' => '<tr>',
		);
		
		$form[$formName]['add_presenter']['presenter_username'] = array(
				'#type' => 'textfield',
				'#title' => t('LBL054').t(':'),
				'#prefix' => '<td colspan="2" class="admin-addedit-presenter_username">',
				'#suffix' => '</td>',
				'#size' => 15,
				'#maxlength' => 50,
		);
		
		$form[$formName]['add_presenter']['sd_second_row_end'] = array(
				'#type' => 'markup',
				'#markup' => '</tr>',
		);
		
		$form[$formName]['add_presenter']['sd_third_row_start'] = array(
				'#type' => 'markup',
				'#markup' => '<tr>',
		);
		
		$form[$formName]['add_presenter']['presenter_password'] = array(
				'#type' => 'password',
				'#title' => t('LBL060').t(':'),
				'#prefix' => '<td colspan="2" class="admin-addedit-presenter_password">',
				'#suffix' => '</td>',
				'#size' => 15,
				'#maxlength' => 50,
		);
		
		$form[$formName]['add_presenter']['sd_third_row_end'] = array(
				'#type' => 'markup',
				'#markup' => '</tr>',
		);
		
		$form[$formName]['add_presenter']['session_heading_row_end'] = array(
				'#type' => 'markup',
				'#markup' => '</table></div></div>',
		);
		
		$cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-action-button-middle-bg white-btn-bg-middle';
		$cancelAttributes['data-wrapperid'] = array($wrapperId);
		$cancelAttributes['onclick'][]         = '$("#exp_meeting_qtip_visible_disp_addpresenter_0_presenter").empty();return false;';
				
		$form[$formName]['cancel_content_edit_mode'] = array(
				'#type' => 'submit',
				'#value' => t('LBL123'),
				'#title' => t('LBL123'),
				'#prefix' =>  '<div class="addedit-form-cancel-container-actions addedit-form-cancel-container-webex-presenter admin-session-save-button admin-save-button-container resize-save_btn"><div class="white-btn-bg-left"></div>',
				'#suffix' =>  '<div class="white-btn-bg-right"></div>',
				'#name' => 'Cancel Presenter',
				'#submit' => array('cancelPresenterDetails'),
				'#attributes' => $cancelAttributes,
		);
		
		
		$buttonName  = 'set-presenter';
		$setAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
		$setAttributes['data-wrapperid'] = array($wrapperId);
		$form[$formName]['set_presenter_edit_mode'] = array(
				'#type' => 'submit',
				'#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
				'#suffix' => '<div class="admin-save-button-right-bg"></div></div></div>',
				'#title' => t('LBL141'),
				'#value' => t('LBL141'),
				'#name' => $buttonName,
				'#attributes' => $setAttributes,
				'#ajax' => array(
						'wrapper' => $wrapperId,
						'callback' => 'add_presenter_callback',
						/*'entity_id' => $entityId,
						'entity_type' => $entityType,
						'course_id' => $courseId,*/
						'entity_plugin'=>'presenter',
						//'customButtonId'=>$customHiddenButtonId,
						'method' => 'replace',
						'effect' => 'none', // 'fade',
						'event'=>'click',
						'keypress' => true,
						'progress' => array(
								'type' => 'throbber',
								'message' => ''
						),
				),
		);
		
		$form_state['storage']['mode'] = '';
		
		if($form_state['autoclose']=='close') {
			$form[$formName]['timepicker_load'] = array(          
				'#type' => 'markup',
				'#prefix' => '<div style="display:none"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="setTimeout(function() { $(\'#exp_meeting_qtip_visible_disp_addpresenter_0_presenter\').empty(); }, 250);return false;" width="100" height="132" /></div>',
			);
		}

		
				
	return $form;
	
	} catch (Exception $ex) {
		watchdog_exception('add_presenter_html', $ex);
		expertusErrorThrow($ex);
	}
}

function add_presenter_callback(&$form, &$form_state){
	try {
		$commands = array();
		if($form_state["storage"]["form_validate"] == true){
			$formName = "add_presenter";
			$commands[] = ajax_command_replace(NULL, drupal_render($form[$formName]));
		}else{
			$formName = "add_presenter";
			$commands[] = ajax_command_replace('#add_presenter_details', drupal_render($form[$formName]));
		}

		$statusMsgHTML = theme('status_messages');

		if (!empty($statusMsgHTML)) {
			// Show the messages
			$commands[] = ajax_command_html(' #bubble-face-table #add-presenter-details #show_webex_detail_message', $statusMsgHTML);//ajax_command_prepend(NULL, $statusMsgHTML);
			// Set the trigger to remove the messages after a while
			if (!empty($form_state['triggering_element']['#ajax']['wrapper'])) {
				$commands[] = addedit_hide_messages($form_state['triggering_element']['#ajax']['wrapper']);
			}
			else {
				$commands[] = addedit_hide_messages();
			}
		}
		$refresh_grid = ($form_state['classType'] == 'new-class' ? true : false);
// 		if($form_state["storage"]["form_validate"]) {
			$commands[] = ajax_command_refresh_narrow_search_results('onclose', $refresh_grid);
// 		}
		return array('#type' => 'ajax', '#commands' => $commands);

	} catch (Exception $ex) {
		watchdog_exception('add_presenter_callback', $ex);
		expertusErrorThrow($ex);
	}
}

function add_presenter_html_validate($form, &$form_state) {
	try {

		$form_state["storage"]["form_validate"] = true;
			
		if(empty($form_state['input']['presenter_username']) && !empty($form_state['input']['presenter_password'])){
			form_set_error('',t('LBL054').t('ERR101'));
			$form_state['esign_show_popup']= false;
		}
		if(!empty($form_state['input']['presenter_username']) && empty($form_state['input']['presenter_password'])){
			form_set_error('',t('LBL060').t('ERR101'));
			$form_state['esign_show_popup']= false;
		}
		if(empty($form_state['input']['presenter_username']) && empty($form_state['input']['presenter_password'])){
			form_set_error('',t('LBL054').t(' ').strtolower(t('LBL647')).t(' ').t('LBL060').t('ERR169'));
			$form_state['esign_show_popup']= false;
		}
	} catch (Exception $ex) {
		watchdog_exception('add_presenter_html_validate', $ex);
		expertusErrorThrow($ex);
	}
}

function add_presenter_html_submit($form, &$form_state) {
	try {
		$loggedUserId = getSltpersonUserId();
		
		$enc = new Encrypt();
		$name=$form_state['input']['presenter_username'];
		$pass=$enc->encrypt($form_state['input']['presenter_password']);

		$fields=array(
			'webex_name' => trim($name),
			'webex_pass' => $pass
		);
		$upd=db_update('slt_person');
		$upd->fields($fields);
		$upd->condition('id',$loggedUserId);
		$upd->execute();
		
		make_audit_trail_entries($fields, $loggedUserId,null, $loggedUserId ,'WebEx','cre_sys_obt_usr','exp_sp_administration_session_details','InsertSessionHostWebExdetails',array('webex_name','webex_pass'));
		
		$form_state['autoclose'] = 'close';
		
		$form_state['rebuild']  = TRUE;
		drupal_set_message(t('MSG689'));
							
	} catch (Exception $ex) {
		watchdog_exception('add_presenter_html_submit', $ex);
		expertusErrorThrow($ex);
	}
}
	
/********************************** Web-Ex Feature Ends *****************************************/

function add_class_session_html($form, &$form_state) {
	try {
	include_once(drupal_get_path('module', 'exp_sp_administration_catalog') .'/exp_sp_administration_catalog_class.inc');
	$form = array();
	$course_id = $form_state['course_id'];
	$class_id = $form_state['class_id'];
	$meetType = $form_state['meet_type'];
	$uniqueId = "session-".core_encrypt($class_id)."-cre_sys_obt_cls";
	$wrapperId = 'session-detail-wrapper-'.$uniqueId;
	$formName = "classSession";
	$form_state['rec_sess_enabled'] =  false;
	$form_state['class_type'] =  $form_state['classType'];
	if(($form_state['is_new'] == "Addanother" || $form_state['is_new'] == "Add") && $meetType != 'lrn_cls_vct_oth'){
		$form_state['rec_sess_enabled'] =  true;
	}
  expDebug::dPrint(' 	$meetType = ' . print_r($meetType, true) , 4);
   if($form_state['is_new'] == "Edit"){
       if($meetType=='lrn_cls_vct_exp' || $meetType=='lrn_cls_vct_web'){
		  $Idtype='editexp';
		 }else if($meetType=='lrn_cls_vct_oth'){
		 	$Idtype='editothers';
		 }
		 else{
		  $Idtype='editclassroom';
		  }

   }else if($form_state['is_new'] == "Add"){
		if($meetType=='lrn_cls_vct_exp' || $meetType=='lrn_cls_vct_web'){
		  $Idtype='expmeeting';
		 }else if($meetType=='lrn_cls_vct_oth'){
		 	$Idtype='others';
		 }
		 else{
		  $Idtype='classroom';
		  }
  }else if($form_state['is_new'] == "Addanother"){
		if($meetType=='lrn_cls_vct_exp' || $meetType=='lrn_cls_vct_web'){
		  $Idtype='addexpmeeting';
		 }else if($meetType=='lrn_cls_vct_oth'){
		 	$Idtype='addothers';
		 }
		 else{
		  $Idtype='addclassroom';
		  }
  }
	$classInfoDet = _getClassInfo($class_id);
	expDebug::dPrint(' $classInfoDet ');
  $form['#prefix'] = /* (( stripos($_SERVER['HTTP_USER_AGENT'], 'Chrome') || stripos ($_SERVER['HTTP_USER_AGENT'], 'Firefox') )? '</form>' : '' )  */'</form>'. '<div id="session_detail_container_sample">';
  $form['#suffix'] = '</div>';
	addHiddenValueToAddEditForm($form, 'course_id', $course_id);
	addHiddenValueToAddEditForm($form, 'class_id', $class_id);
	addHiddenValueToAddEditForm($form, 'ses_min_seats', $classInfoDet[0]->min_seats);
	addHiddenValueToAddEditForm($form, 'ses_max_seats', $classInfoDet[0]->max_seats);
	addHiddenValueToAddEditForm($form, 'ses_waitlist_count', $classInfoDet[0]->waitlist_count);
	addHiddenValueToAddEditForm($form, 'ses_location_id', $classInfoDet[0]->location_id);
	addHiddenFieldToAddEditForm($form, 'cls_delivery_type_'.$class_id, $classInfoDet[0]->delivery_type);
	addHiddenValueToAddEditForm($form, 'ses_loc_timezone', $classInfoDet[0]->loc_timezone);
	addHiddenFieldToAddEditForm($form, 'change_instructor', '1');
	$isEnrolled = getTotalClassEnroll($class_id);
	$deliveryType = $classInfoDet[0]->delivery_type;
	if (!empty($class_id) && $form_state['is_new'] == "Edit") {
		$sessionDetails = _getSessionDetails($class_id,'',$form_state['session_id_foredit']);
	}

	$form[$formName]['timepicker_load'] = array(
   '#type' => 'markup',
   '#prefix' => '<div style="display:none"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'#root-admin\').data(\'narrowsearch\').timepickerLoad();return false;" width="100" height="132" /></div>',
       );

	$form[$formName]['session_details']['session_heading_row_start'] = array(
      '#type' => 'markup',
      '#markup' => '<div id="add-session-details"><div id="meeting-type-'.$Idtype.'"><div id="show_vc_detail_message"></div><table cellpadding="0" cellspacing="0" class="admin-addedit-session-details-wrapper" border="0">',
	);

	$titleAttributes['class'] = array();
	$titleAttributes['class'][] = 'input-field-set addedit-edit-ac-textfield';

	if ($isEnrolled) {
		$titleAttributes['disabled'] = 'disabled';
		$titleAttributes['class'][] = 'addedit-readonly-textfield'; // Add another class
	}

	$form[$formName]['session_details']['sd_first_row_start'] = array(
      '#type' => 'markup',
      '#markup' => '<tr>',
	);
	$form[$formName]['session_details']['hid_session_id'] = array (
				'#id' => 'hid_session_id',
				'#type' => 'hidden',
				'#default_value' => $sessionDetails[0]->session_id,
				'#attributes' => array('id'=>'hid_session_id')
	);
	$sesstitle = ($deliveryType == 'lrn_cls_dty_vcl')? t('LBL1316') : t('LBL295');
	$form[$formName]['session_details']['session_name'] = array(
      '#type' => 'textfield',      
      '#title' => $sesstitle.':<span class="addedit-mandatory">*</span>',
      '#prefix' => '<td colspan="2" class="admin-addedit-class-session-name">',
      '#suffix' => '</td>',
      '#default_value' => $sessionDetails[0]->title,
      '#size' => 10,
      '#maxlength' => 300,
      '#attributes' => $titleAttributes,
	);



	$form[$formName]['session_details']['sd_first_row_end'] = array(
      '#type' => 'markup',
      '#markup' => '</tr>',
	);

	$form[$formName]['session_details']['sd_second_row_start'] = array(
      '#type' => 'markup',
      '#markup' => '<tr>',
	);

	$attributes = array();
	$attributes['autocomplete'] = 'off';
	$attributes['class'] = array('addedit-edit-start'); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

	$attributes['data-datePickerDatesDisplayOption'] = "all";
	$attributes['data-emptyDateFieldText'] = 'mm-dd-yyyy';
	$attributes['data-datePickerTooltip'] = t('LBL218');


	$session_start_date =  array();

	$session_start_date = !empty($sessionDetails[0]->start_date) ? explode(" ",$sessionDetails[0]->start_date) : array();
	$session_start_time = !empty($sessionDetails[0]->start_time) ? $sessionDetails[0]->start_time : '00:00';

	$sessionStartDate = !empty($session_start_date[0]) ? date("m-d-Y", strtotime($session_start_date[0])):'';


	if ($isEnrolled) {
		$attributes['disabled'] = 'disabled';
		$attributes['class'][] = 'addedit-readonly-textfield';
		$attributes['class'][] = 'addedit-edit-ac-textfield';
	}else{
		$attributes['class'][] = 'addedit-edit-datefield';
		$attributes['class'][] = 'addedit-edit-ac-textfield';

	}
	$attributes['onclick'][]         = 'datepickerAddEdit();';
	$form[$formName]['session_details']['start_date'] = array(
     '#title' => t('LBL648').':<span class="addedit-mandatory">*</span>',
     '#id'   => 'start_date',
     '#type' => 'textfield',
     '#prefix' => '<td class="admin-addedit-class-start-on admin-session-date-container">',
     '#suffix' => '</td>',
     '#default_value' => $sessionStartDate,
     '#size' => 10,
     '#attributes' => $attributes,
		
			
	);
	if($form_state['rec_sess_enabled']==true){
	$form[$formName]['session_details']['end_date'] = array(
			'#title' => t('LBL3054').':<span class="addedit-mandatory">*</span>',
			'#id'   => 'end_date',
			'#type' => 'textfield',
			'#prefix' => '<td class="admin-addedit-class-ends-on admin-session-date-container">',
			'#suffix' => '</td>',
			'#default_value' => $sessionStartDate,
			'#size' => 10,
			'#attributes' => $attributes,
	);

	$form[$formName]['session_details']['sd_second_row_end'] = array(
			'#type' => 'markup',
			'#markup' => '</tr>',
	);
	
	
	$form[$formName]['session_details']['sd_third_row_start'] = array(
			'#type' => 'markup',
			'#markup' => '<tr>',
	);
	
	$readOnlycheckbox = ($isEnrolled) ? 'true' : 'false';
	$disabled_cond= '';
	if($isEnrolled){
	$disabled_cond =  'disabled = "true"';
	}
	 $form[$formName]['session_details']['checkbox-days'] = array('#type' => 'markup', '#markup' => '<tr><td colspan="2"><div id = "days">');
	 $form[$formName]['session_details']['checkbox-title'] = array(
	 		'#type' => 'markup',
	 		'#prefix' => '<span id = "addsession_days_title" class= "title_days">'.t('LBL3057').':</span>',
	 
	 );
	 $form[$formName]['session_details']['days_string'] = array(
	 		'#type' => 'markup',
	 		'#prefix' => '<div id = "days_string">',
	 
	 );
	$days =  array(
	 		'Mon' =>  'LBL3058',
	 		'Tue' =>  'LBL3048',
	 		'Wed' =>  'LBL3049',
	 		'Thu' =>  'LBL3050',
	 		'Fri' =>  'LBL3051',
	 		'Sat' =>  'LBL3052',
	 		'Sun' =>  'LBL3053',
	 );
	 foreach($days as $eachday => $labelforeachday){
	 	$form[$formName]['session_details'][] =array(
	 			'checkbox'-$eachday =>
	 			array(
	 					'#type' => 'markup',
	 					'#prefix' => '<div id= "session_det_weekdays"><span id = "'.$eachday.'-text" class= "session_det_eachday">'.t("$labelforeachday").'</span><div class="'.$eachday.' checkbox-unselected"><input class="weekday-checkbox-input" id="'.$eachday.'" type="checkbox"  name="'.$eachday.'" onclick="checkboxSelectedUnselectedCommon(this);" value=1 ></div></div>',
	 	
	 			));
	 }
	$form[$formName]['session_details']['days_string_close'] = array('#type' => 'markup', '#markup' => '</div>');
	$form[$formName]['session_details']['checkbox-days-close'] = array('#type' => 'markup', '#markup' => '</div></td></tr>'); 
	
	$form[$formName]['session_details']['sd_third_row_end'] = array(
			'#type' => 'markup',
			'#markup' => '</tr>',
	);
	
	
	$form[$formName]['session_details']['sd_fourth_row_start'] = array(
			'#type' => 'markup',
			'#markup' => '<tr>',
	);
	}
	$attributesTime = array();
	$attributesTime['autocomplete'] = 'off';
	$attributesTime['class'] = array('addedit-edit-start');
	$attributesTime['data-emptyDateFieldText'] = 'hh:mm';
	$attributesTime['data-datePickerTooltip'] = t('LBL218');
	
	if ($isEnrolled) {
		$attributesTime['disabled'] = 'disabled';
		$attributesTime['class'][] = 'addedit-readonly-textfield';
		$attributesTime['class'][] = 'addedit-edit-ac-textfield';
	}else{
		$attributesTime['class'][] = 'addedit-edit-ac-textfield';
		$attributesTime['class'][] = 'exp-timepicker';
	}
	
	$attributesTime['readonly'][] = 'readonly';
	
	//else {
	if(($deliveryType == 'lrn_cls_dty_vcl' && $form_state['is_new'] == "Edit") || (($form_state['is_new'] == "Addanother" || $form_state['is_new'] == "Add") && $meetType == 'lrn_cls_vct_oth') ){

		$timeZoneList = _getTimeZone();
		$optTZ['']= t('LBL674');
		foreach ($timeZoneList as $id => $row) {
			$zoneName = explode(')',$row->zonename);
			$timeZone[$row->zonecode] = $row->zonename;//str_replace('(','',$zoneName[0]);;
		}
		$timezoneAttributes['class'] = array();
		$timezoneAttributes['class'][] = 'addedit-edit-select ctrDropDown';
		$timezoneAttributes['onblur'][] = 'this.className="addedit-edit-select ctrDropDown";';
		$timezoneAttributes['onmousedown'][] = 'this.className="addedit-edit-select ctrDropDownClick"';
		$timezoneAttributes['onchange'][] = 'this.className="addedit-edit-select ctrDropDown"';
		if ($isEnrolled) {
			$timezoneAttributes['disabled'] = 'disabled';
		}
		//Using the below veriable we can find current loaded theme name
		global $theme_key;
		
		if($theme_key == 'expertusoneV2') {
			$newuiPrefix = '<div class="expertus-dropdown-bg"><div class="expertus-dropdown-icon ">';
			$newuiSuffix = '</div></div>';
		} else {
			$newuiPrefix = '';
			$newuiSuffix = '';
		}
		
		
		$pfTimeZone = array_merge($optTZ,$timeZone);
		$form[$formName]['session_details']['time_zone_list'] = array(
				'#id' => 'timezone',
				//'#title' => '',
				'#type' => 'select',
				'#prefix' => '<td class="admin-addedit-class-time-zone"><div class="crt-paint-timezone"><div class="form-item-time-zone-list"><label>'.t('LBL297').':<span class="addedit-mandatory">*</span></label></div>'.$newuiPrefix,
				'#suffix' => $newuiSuffix.'</div></td>',
				'#options'=> $pfTimeZone,
				'#default_value' => $sessionDetails[0]->timezone,
				'#maxlength' => 50,
				'#attributes' => $timezoneAttributes,//array('Class' => 'addedit-edit-select'),
		);
	}else{
		
	$form[$formName]['session_details']['start_hours'] = array(
			'#title' => t('LBL649').':<span class="addedit-mandatory">*</span>',
			'#id'   => 'start_hours',
			'#type' => 'textfield',
			'#prefix' => '<td class="admin-addedit-class-start-time admin-session-from-container">',
			'#suffix' => '</td>',
			'#default_value' => $sessionDetails[0]->start_time != '' ? $sessionDetails[0]->start_time : '00:00',
			'#size' => 10,
			'#attributes' => $attributesTime,
	);
	}
		/*$form[$formName]['session_details']['hid_instructor_id'] = array (
				'#id' => 'hid_instructor_id',
				'#type' => 'hidden',
				'#default_value' => $sessionDetails[0]->instructor_id,
				'#attributes' => array('id'=>'hid_instructor_id')
		);

		$defaultValue =  getInstructorInfo($form_state['session_id_foredit']);
		$instructorOption = getAllInstructor();
		$colspan = ($deliverytype=='lrn_cls_dty_ilt' || $deliveryType == 'lrn_cls_dty_vcl')? '2' : '1';
		addMultiselectDropDownToAddEditForm($form[$formName]['session_details'], 'session_instructor', t('Instructor'), $instructorOption, $defaultValue, 5, false, false, array('style'=>'display: none;','class'=>'instructor_classroom'),array('code' => 'ins', 'user_id' => ($form_state['session_id_foredit'])),0,false,$colspan);
		//$acMenuPath = 'administration/learning/instructor/instructor-autocomplete';
		//addACFieldSessionToAddEditForm($form, $formName, 'instructor_' , $acMenuPath, t('Instructor').':', $fullName, '', array(), array('id' => '"hid_instructor_id"', 'class_id' => $class_id), false, $readOnly,$deliveryType,'',$insPreText);*/
//}
	if(($form_state['is_new'] == "Edit" && $meetType!= 'lrn_cls_vct_oth') || (($form_state['is_new'] == "Addanother" || $form_state['is_new'] == "Add") && $meetType == 'lrn_cls_vct_oth')){
		$form[$formName]['session_details']['sd_third_row_end'] = array(
			'#type' => 'markup',
			'#markup' => '</tr>',
	);
	
	
	$form[$formName]['session_details']['sd_fourth_row_start'] = array(
			'#type' => 'markup',
			'#markup' => '<tr>',
	);
	}
	if(($deliveryType == 'lrn_cls_dty_vcl' && $form_state['is_new'] == "Edit" || (($form_state['is_new'] == "Addanother" || $form_state['is_new'] == "Add") && $meetType == 'lrn_cls_vct_oth'))){
		$form[$formName]['session_details']['start_hours'] = array(
				'#title' => t('LBL649').':<span class="addedit-mandatory">*</span>',
				'#id'   => 'start_hours',
				'#type' => 'textfield',
				'#prefix' => '<td class="admin-addedit-class-start-time admin-session-from-container">',
				'#suffix' => '</td>',
				'#default_value' => $sessionDetails[0]->start_time != '' ? $sessionDetails[0]->start_time : '00:00',
				'#size' => 10,
				'#attributes' => $attributesTime,
		);
		$form[$formName]['session_details']['end_hours'] = array(
				'#title' => t('LBL621').':<span class="addedit-mandatory">*</span>',
				'#id'   => 'end_hours',
				'#type' => 'textfield',
				'#prefix' => '<td class="admin-addedit-class-start-time admin-session-to-container">',
				'#suffix' => '</td>',
				'#default_value' => $sessionDetails[0]->end_time != '' ? $sessionDetails[0]->end_time : '00:00',
				'#size' => 10,
				'#attributes' => $attributesTime,
		);

		$form[$formName]['session_details']['sd_fourth_row_end'] = array(
				'#type' => 'markup',
				'#markup' => '</tr>',
		);
		
		$form[$formName]['session_details']['sd_fifth_row_start'] = array(
				'#type' => 'markup',
				'#markup' => '<tr>',
		);
	}
	
	if($form_state['is_new'] == "Addanother" || $form_state['is_new'] == "Add" ){
	$form[$formName]['session_details']['end_hours'] = array(
      '#title' => t('LBL621').':<span class="addedit-mandatory">*</span>',
      '#id'   => 'end_hours',
      '#type' => 'textfield',
      '#prefix' => '<td class="admin-addedit-class-start-time admin-session-to-container">',
      '#suffix' => '</td>',
      '#default_value' => $sessionDetails[0]->end_time != '' ? $sessionDetails[0]->end_time : '00:00',
      '#size' => 10,
      '#attributes' => $attributesTime,
	);


	$form[$formName]['session_details']['sd_fourth_row_end'] = array(
			'#type' => 'markup',
			'#markup' => '</tr>',
	);
	
	$form[$formName]['session_details']['sd_fifth_row_start'] = array(
			'#type' => 'markup',
			'#markup' => '<tr>',
	);
	}
	if($deliveryType == 'lrn_cls_dty_ilt'){
		/* $instructorId = '';//$form_state['input']['meeting_detail_virtual_class_html_form']['session_details']['hid_session_details_id'];
			$user_detail=db_query("select full_name from slt_person WHERE id = '".$sessionDetails[0]->instructor_id."'");
			$userDetail = $user_detail->fetchAll();
			$fullName = $userDetail[0]->full_name;
			if(!empty($userDetail)){
			$fullName = $userDetail[0]->full_name;
			$insPreText = '';
			}
			else{
			$fullName = t('LBL181');
			$insPreText = t('LBL181');
			} */
	
		/*$form[$formName]['session_details']['start_hours'] = array(
		 '#title' => t('LBL649').':<span class="addedit-mandatory">*</span>',
		 '#id'   => 'start_hours',
		 '#type' => 'textfield',
		 '#prefix' => '<td class="admin-addedit-class-start-time admin-session-from-container">',
		 '#suffix' => '</td>',
		 '#default_value' => $sessionDetails[0]->start_time != '' ? $sessionDetails[0]->start_time : '00:00',
		 '#size' => 10,
		 '#attributes' => $attributesTime,
		);*/
		$form[$formName]['session_details']['hid_instructor_id'] = array (
				'#id' => 'hid_instructor_id',
				'#type' => 'hidden',
				'#default_value' => $sessionDetails[0]->instructor_id,
				'#attributes' => array('id'=>'hid_instructor_id')
		);
	
		$defaultValue =  getInstructorInfo($form_state['session_id_foredit']);
		$instructorOption = ''; //getAllInstructor();
		$colspan = ($deliverytype=='lrn_cls_dty_ilt' || $deliveryType == 'lrn_cls_dty_vcl')? '2' : '1';
		addMultiselectDropDownToAddEditForm($form[$formName]['session_details'], 'session_instructor', t('Instructor'), $instructorOption, $defaultValue, 5, false, false, array('style'=>'display: none;','class'=>'instructor_classroom'),array('code' => 'ins', 'user_id' => ($form_state['session_id_foredit'])),0,false,$colspan);
		//$acMenuPath = 'administration/learning/instructor/instructor-autocomplete';
		//addACFieldSessionToAddEditForm($form, $formName, 'instructor_' , $acMenuPath, t('Instructor').':', $fullName, '', array(), array('id' => '"hid_instructor_id"', 'class_id' => $class_id), false, $readOnly,$deliveryType,'',$insPreText);
		if($form_state['is_new'] == "Edit"){
			$form[$formName]['session_details']['end_hours'] = array(
					'#title' => t('LBL621').':<span class="addedit-mandatory">*</span>',
					'#id'   => 'end_hours',
					'#type' => 'textfield',
					'#prefix' => '<td class="admin-addedit-class-start-time admin-session-to-container">',
					'#suffix' => '</td>',
					'#default_value' => $sessionDetails[0]->end_time != '' ? $sessionDetails[0]->end_time : '00:00',
					'#size' => 10,
					'#attributes' => $attributesTime,
			);
			
			
			$form[$formName]['session_details']['sd_fourth_row_end'] = array(
					'#type' => 'markup',
					'#markup' => '</tr>',
			);
			
			$form[$formName]['session_details']['sd_fifth_row_start'] = array(
					'#type' => 'markup',
					'#markup' => '<tr>',
			);
		}
		
		
	}else{
		/* $form[$formName]['session_details']['start_hours'] = array(
				'#title' => t('LBL649').':<span class="addedit-mandatory">*</span>',
				'#id'   => 'start_hours',
				'#type' => 'textfield',
				'#prefix' => '<td class="admin-addedit-class-start-time admin-session-from-container">',
				'#suffix' => '</td>',
				'#default_value' => $sessionDetails[0]->start_time != '' ? $sessionDetails[0]->start_time : '00:00',
				'#size' => 10,
				'#attributes' => $attributesTime,
		); */
		if($meetType!= 'lrn_cls_vct_oth'){
		$timeZoneList = _getTimeZone();
		$optTZ['']= t('LBL674');
		foreach ($timeZoneList as $id => $row) {
			$zoneName = explode(')',$row->zonename);
			$timeZone[$row->zonecode] = $row->zonename;//str_replace('(','',$zoneName[0]);;
		}
		$timezoneAttributes['class'] = array();
		$timezoneAttributes['class'][] = 'addedit-edit-select ctrDropDown';
		$timezoneAttributes['onblur'][] = 'this.className="addedit-edit-select ctrDropDown";';
		$timezoneAttributes['onmousedown'][] = 'this.className="addedit-edit-select ctrDropDownClick"';
		$timezoneAttributes['onchange'][] = 'this.className="addedit-edit-select ctrDropDown"';
		if ($isEnrolled) {
			$timezoneAttributes['disabled'] = 'disabled';
		}
		//Using the below veriable we can find current loaded theme name
		global $theme_key;
	
		if($theme_key == 'expertusoneV2') {
			$newuiPrefix = '<div class="expertus-dropdown-bg"><div class="expertus-dropdown-icon ">';
			$newuiSuffix = '</div></div>';
		} else {
			$newuiPrefix = '';
			$newuiSuffix = '';
		}
	   
	
		$pfTimeZone = array_merge($optTZ,$timeZone);
		$form[$formName]['session_details']['time_zone_list'] = array(
				'#id' => 'timezone',
				//'#title' => '',
				'#type' => 'select',
				'#prefix' => '<td class="admin-addedit-class-time-zone"><div class="crt-paint-timezone"><div class="form-item-time-zone-list"><label>'.t('LBL297').':<span class="addedit-mandatory">*</span></label></div>'.$newuiPrefix,
				'#suffix' => $newuiSuffix.'</div></td>',
				'#options'=> $pfTimeZone,
				'#default_value' => $sessionDetails[0]->timezone,
				'#maxlength' => 50,
				'#attributes' => $timezoneAttributes,//array('Class' => 'addedit-edit-select'),
		);
		}
	}
	if($deliveryType == 'lrn_cls_dty_vcl' && ($meetType == 'lrn_cls_vct_exp' || $meetType == 'lrn_cls_vct_web')){
		/* $instructorId = '';//$form_state['input']['meeting_detail_virtual_class_html_form']['session_details']['hid_session_details_id'];
		$user_detail=db_query("select full_name from slt_person WHERE id = '".$sessionDetails[0]->instructor_id."'");
		$userDetail = $user_detail->fetchAll();
		$fullName = $userDetail[0]->full_name;
		if(!empty($userDetail)){
			$fullName = $userDetail[0]->full_name;
			$insPreText = '';
		}
		else{
			$fullName = t('LBL181');
			$insPreText = t('LBL181');
		} */
		addTwoColumnRowToAddEditForm($form[$formName]['session_details'], 'instructor_presenter');
		$form[$formName]['session_details']['hid_instructor_id'] = array (
				'#id'     => 'hid_instructor_id',
				'#type'   => 'hidden',
				'#default_value' => $sessionDetails[0]->instructor_id,
				'#attributes' => array('id'=>'hid_instructor_id')
		);
		$defaultIns =  getInstructorInfo($form_state['session_id_foredit']);
		$instructorOption = ''; //getAllInstructor();
		$colspan = '2';
		if($meetType == 'lrn_cls_vct_web'){
		addMultiselectDropDownToAddEditForm($form[$formName]['session_details'], 'session_instructor', t('Instructor'), $instructorOption, $defaultIns, 5, false, false, array('style'=>'display: none;','class'=>'instructor_expmeeting'),array('code' => 'ins', 'user_id' => ($form_state['session_id_foredit'])),0,false,$colspan);
		}else{
		addMultiselectDropDownToAddEditForm($form[$formName]['session_details'], 'session_instructor', t('Instructor'), $instructorOption, $defaultIns, 5, false, false, array('style'=>'display: none;','class'=>'instructor_expmeeting'),array('code' => 'ins', 'user_id' => ($form_state['session_id_foredit'])),0,false,$colspan);
		}
		$defPresenter = getInstructorInfo($form_state['session_id_foredit'],'Y');
		$presenterList = getInstructorInfo($form_state['session_id_foredit'],'','presenter');
		expDebug::dPrint("get Presenter list ".print_r($presenterList,true),5);
		$presenterOpt = array();
		foreach($presenterList as $id => $row){
			expDebug::dPrint("testing ");
			$presenterOpt[$row->opt] = $row->full_name; 
		}
		if($meetType == 'lrn_cls_vct_exp' ){
			if($form_state['is_new'] != "Edit"){
			$form[$formName]['session_details']['sd_fifth_row_end'] = array(
					'#type' => 'markup',
					'#markup' => '</tr>',
			);
			
			$form[$formName]['session_details']['sd_web_row_start'] = array(
					'#type' => 'markup',
					'#markup' => '<tr>',
			);
			}
		addDropDownToAddEditForm($form[$formName]['session_details'], 'session_presenter', t('Moderator'), $presenterOpt, $defPresenter,null,false,false,array(),'exp_meeting');		
		}
		elseif($meetType == 'lrn_cls_vct_web'){

	
			
		//addDropDownToAddEditForm($form[$formName]['session_details'], 'session_presenter', t('Host'), $presenterOpt, $defPresenter,null,false,false,array(),'exp_meeting');
		
		global $user;
		
		$qtip_lang = $user->language;
       if($form_state['is_new'] != "Edit"){
		$form[$formName]['session_details']['sd_fifth_row_end'] = array(
				'#type' => 'markup',
				'#markup' => '</tr>',
		);
			
		$form[$formName]['session_details']['sd_web_row_start'] = array(
				'#type' => 'markup',
				'#markup' => '<tr>',
		);
        }
			
		addDropDownToAddEditForm($form[$formName]['session_details'], 'session_presenter', t('Host'), $presenterOpt, $defPresenter,null,false,false,array('onClick' => 'webex_presenter_validate(this.value,"'.$qtip_lang.'"); return false;'),'exp_meeting');
			
			$form[$formName]['add_presenter_start'] = array(
					'#type' => 'markup',
					'#markup' => '<div class="admin-datagrid-add-btn-wrapper-container expertus-meeting-button-container">
					<div class="add_presenter_popup" id="exp_meeting_qtip_visible_disp_addpresenter_0_presenter">',
			);
				
			$form[$formName]['add_presenter_end'] = array(
					'#type' => 'markup',
					'#markup' => '</div></div>',
			);
			

			
		}else{
			/*$pres_id=$sessionDetails[0]->presenter_id;
			expDebug::dPrint('hidden id' .$pres_id);
			$form[$formName]['session_details']['hid_presenter_id'] = array (
					'#id'     => 'hid_presenter_id',
					'#type'   => 'hidden',
					'#default_value' => $sessionDetails[0]->presenter_id,
					'#attributes' => array('id'=>'hid_presenter_id','onclick' => 'textfieldTitleChange1(id);')
			);*/
		addDropDownToAddEditForm($form[$formName]['session_details'], 'session_presenter', t('Presenter'), $presenterOpt, $defPresenter,null,true,false,array(),'web_ex');
		
	}
		endAddEditTwoColumnRow($form[$formName]['session_details']);
	}
	$AttPassArray = explode('/',$sessionDetails[0]->session_attendeeurl);
	$PrePassArray = explode('/',$sessionDetails[0]->session_presenterurl);
	$AttPassLen = count($AttPassArray);
	$PrePassLen = count($PrePassArray);
	$AttendeePass =  $AttPassArray[$AttPassLen - 1];
	$PrePass =  $PrePassArray[$PrePassLen - 1];
	$meetId =  $AttPassArray[$AttPassLen - 2];
	/*if($deliveryType == 'lrn_cls_dty_vcl' && $meetType == 'lrn_cls_vct_web'){

		$form[$formName]['session_details']['session_attende_password'] = array(
			'#id' => 'session_attende_password',
			'#title' => t('LBL859').':<span class="addedit-mandatory session-pass-mandatory">*</span>',
			'#type' => 'textfield',
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#default_value' => $AttendeePass,
			'#disabled' => $readOnly,
			'#size' => 13,
			'#maxlength' => 100,
		);

		$readOnly = ($isEnrolled) ? true : false;
		$form[$formName]['session_details']['session_meeting_id'] = array(
			'#id' => 'session_meeting_id',
			'#title' => t('LBL858').':<span class="addedit-mandatory session-pass-mandatory">*</span>',
			'#type' => 'textfield',
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#default_value' => $meetId,
			'#size' => 13,
			'#maxlength' => 100,
			'#disabled' => $readOnly,
		);
	}*/

	if($deliveryType == 'lrn_cls_dty_vcl' && $meetType == 'lrn_cls_vct_oth'){
		$form[$formName]['session_details']['session_attendee_url'] = array(
			'#id' => 'session_attendee_url',
			'#title' => t('LBL296').':<span class="addedit-mandatory session-pass-mandatory">*</span>',
			'#type' => 'textfield',
			'#prefix' => '<td colspan="2">',
			'#suffix' => '</td>',
			'#default_value' => rawurldecode($sessionDetails[0]->session_attendeeurl),
			'#size' => 13,
			'#maxlength' => 200,
		);
		$form[$formName]['session_details']['sd_att_row_end'] = array(
				'#type' => 'markup',
				'#markup' => '</tr>',
		);
			
		$form[$formName]['session_details']['sd_pre_row_start'] = array(
				'#type' => 'markup',
				'#markup' => '<tr>',
		);

	}

	

	/*if($deliveryType == 'lrn_cls_dty_vcl' && $meetType == 'lrn_cls_vct_web'){
		$form[$formName]['session_details']['session_presenter_password'] = array(
			'#id' => 'session_presenter_password',
			'#title' => t('LBL860').':<span class="addedit-mandatory session-pass-mandatory">*</span>',
			'#type' => 'textfield',
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#default_value' => $PrePass,
			'#size' => 13,
			'#maxlength' => 100,
			'#disabled' => $readOnly,
		);

		$instructorId = '';//$form_state['input']['meeting_detail_virtual_class_html_form']['session_details']['hid_session_details_id'];
		$user_detail=db_query("select full_name from slt_person WHERE id = '".$sessionDetails[0]->instructor_id."'");
		$userDetail = $user_detail->fetchAll();
		$fullName = $userDetail[0]->full_name;
		if(!empty($userDetail)){
			$fullName = $userDetail[0]->full_name;
			$insPreText = '';
		}
		else{
			$fullName = t('LBL181');
			$insPreText = t('LBL181');
		}
		$form[$formName]['session_details']['hid_instructor_id'] = array (
				'#id' => 'hid_instructor_id',
				'#type' => 'hidden',
				'#default_value' => $sessionDetails[0]->instructor_id,
				'#attributes' => array('id'=>'hid_instructor_id')
		);
		$defaultIns =  getInstructorInfo($form_state['session_id_foredit']);
		$instructorOption = getAllInstructor();
		$colspan = '2';
		addMultiselectDropDownToAddEditForm($form[$formName]['session_details'], 'session_instructor', t('Instructor'), $instructorOption, $defaultIns, 5, false, false, array('style'=>'display: none;','class'=>'instructor_webex'),array('code' => 'ins', 'user_id' => ($form_state['session_id_foredit'])),0,false,$colspan);

		$form[$formName]['session_details']['sd_fifth_row_end'] = array(
				'#type' => 'markup',
				'#markup' => '</tr>',
		);
		$form[$formName]['session_details']['sd_sixth_row_start'] = array(
				'#type' => 'markup',
				'#markup' => '<tr>',
		);
		$defPresenter = getInstructorInfo($form_state['session_id_foredit'],'Y');
		$presenterList = getInstructorInfo($form_state['session_id_foredit'],'','presenter');
		expDebug::dPrint("get instructor list ".print_r($presenterList,true),5);
		$presenterOpt = array();
		foreach($presenterList as $id => $row){
			$presenterOpt[$row->opt] = $row->full_name; 
	}
		addDropDownToAddEditForm($form[$formName]['session_details'], 'session_presenter', t('Presenter'), $presenterOpt, $defPresenter,null,false,false,array(),'web_ex');
		$form[$formName]['session_details']['sd_sixth_row_end'] = array(
				'#type' => 'markup',
				'#markup' => '</tr>',
		);
	}*/

	if($deliveryType == 'lrn_cls_dty_vcl' && $meetType == 'lrn_cls_vct_oth'){
		$form[$formName]['session_details']['session_presenter_url'] = array(
			'#id' => 'session_presenter_url',
			'#title' => t('LBL977').':<span class="addedit-mandatory session-pass-mandatory">*</span>',
			'#type' => 'textfield',
			'#prefix' => '<td colspan="2">',
			'#suffix' => '</td>',
			'#default_value' => rawurldecode($sessionDetails[0]->session_presenterurl),
			'#size' => 13,
			'#maxlength' => 200,
		);

		$form[$formName]['session_details']['sd_fifth_row_end'] = array(
				'#type' => 'markup',
				'#markup' => '</tr>',
		);
	}
	if(($deliveryType == 'lrn_cls_dty_vcl' && ($meetType == 'lrn_cls_vct_exp' || $meetType == 'lrn_cls_vct_web')) || ($deliveryType == 'lrn_cls_dty_ilt')){
	$form[$formName]['session_details']['sd_fifth_row_end'] = array(
      '#type' => 'markup',
      '#markup' => '</tr>',
	);
	}

	$form[$formName]['session_details']['sd_sixth_row_start'] = array(
      '#type' => 'markup',
      '#markup' => '<tr>',
	);

	if($deliveryType == 'lrn_cls_dty_vcl' && $meetType == 'lrn_cls_vct_oth'){
		$instructorId = '';//$form_state['input']['meeting_detail_virtual_class_html_form']['session_details']['hid_session_details_id'];
		$user_detail=db_query("select full_name from slt_person WHERE id = '".$sessionDetails[0]->instructor_id."'");
		$userDetail = $user_detail->fetchAll();
		$fullName = $userDetail[0]->full_name;

		if(!empty($userDetail)){
			$fullName = $userDetail[0]->full_name;
			$insPreText = '';
		}
		else{
			$fullName = t('LBL181');
			$insPreText = t('LBL181');
		}
		addTwoColumnRowToAddEditForm($form[$formName]['session_details'], 'instructor_presenter');
		$form[$formName]['session_details']['hid_instructor_id'] = array (
			'#id' => 'hid_instructor_id',
			'#type' => 'hidden',
			'#default_value' => $sessionDetails[0]->instructor_id,
			'#attributes' => array('id'=>'hid_instructor_id')
		);
		$defaultIns =  getInstructorInfo($form_state['session_id_foredit']);
		$instructorOption = ''; //getAllInstructor();
		$colspan = '2';
		addMultiselectDropDownToAddEditForm($form[$formName]['session_details'], 'session_instructor', t('Instructor'), $instructorOption, $defaultIns, 5, false, false, array('style'=>'display: none;','class'=>'instructor_others'),array('code' => 'ins', 'user_id' => ($form_state['session_id_foredit'])),0,false,$colspan);
		$defPresenter = getInstructorInfo($form_state['session_id_foredit'],'Y');
		$presenterList = getInstructorInfo($form_state['session_id_foredit'],'','presenter');
		expDebug::dPrint("get instructor list ".print_r($presenterList,true),5);
		$presenterOpt = array();
		foreach($presenterList as $id => $row){
			$presenterOpt[$row->opt] = $row->full_name; 
		}
		//$presenterlabel=titleController('PRESENTER-LABEL',t('Presenter'));
		addDropDownToAddEditForm($form[$formName]['session_details'], 'session_presenter', t('Presenter'), $presenterOpt, $defPresenter,null,false,false,array(),'other_meet');
		endAddEditTwoColumnRow($form[$formName]['session_details']);
	}
	if($meetType== 'lrn_cls_vct_oth'){
		
		$timeZoneList = _getTimeZone();
		$optTZ['']= t('LBL674');
		foreach ($timeZoneList as $id => $row) {
			$zoneName = explode(')',$row->zonename);
			$timeZone[$row->zonecode] = $row->zonename;//str_replace('(','',$zoneName[0]);;
		}
		$timezoneAttributes['class'] = array();
		$timezoneAttributes['class'][] = 'addedit-edit-select ctrDropDown';
		$timezoneAttributes['onblur'][] = 'this.className="addedit-edit-select ctrDropDown";';
		$timezoneAttributes['onmousedown'][] = 'this.className="addedit-edit-select ctrDropDownClick"';
		$timezoneAttributes['onchange'][] = 'this.className="addedit-edit-select ctrDropDown"';
		if ($isEnrolled) {
			$timezoneAttributes['disabled'] = 'disabled';
		}
		//Using the below veriable we can find current loaded theme name
		global $theme_key;
	
		if($theme_key == 'expertusoneV2') {
			$newuiPrefix = '<div class="expertus-dropdown-bg"><div class="expertus-dropdown-icon ">';
			$newuiSuffix = '</div></div>';
		} else {
			$newuiPrefix = '';
			$newuiSuffix = '';
		}
		if($form_state['is_new'] == "Addanother" || $form_state['is_new'] == "Add" ){		
		$form[$formName]['session_details']['sd_sixth_row_end'] = array(
				'#type' => 'markup',
				'#markup' => '<tr>',
		);
		}
		$pfTimeZone = array_merge($optTZ,$timeZone);
		$form[$formName]['session_details']['time_zone_list'] = array(
				'#id' => 'timezone',
				//'#title' => '',
				'#type' => 'select',
				'#prefix' => '<td class="admin-addedit-class-time-zone"><div class="crt-paint-timezone"><div class="form-item-time-zone-list"><label>'.t('LBL297').':<span class="addedit-mandatory">*</span></label></div>'.$newuiPrefix,
				'#suffix' => $newuiSuffix.'</div></td></tr>',
				'#options'=> $pfTimeZone,
				'#default_value' => $sessionDetails[0]->timezone,
				'#maxlength' => 50,
				'#attributes' => $timezoneAttributes,//array('Class' => 'addedit-edit-select'),
		);
	}
	$form[$formName]['session_details']['sd_sixth_row_end'] = array(
      '#type' => 'markup',
      '#markup' => '</tr>',
	);
	$form[$formName]['session_heading_row_end'] = array(
      '#type' => 'markup',
      '#markup' => '</table></div></div>',
	);

	$cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-action-button-middle-bg white-btn-bg-middle';
	$cancelAttributes['data-wrapperid'] = array($wrapperId);
	$cancelAttributes['onclick'][]         = '$(".active-qtip-div").remove();$(\'#meeting-title\').html(\'\');return false;';
	$form[$formName]['cancel_content_edit_mode'] = array(
      '#type' => 'submit',
      '#value' => t('LBL109'),
      '#title' => t('LBL109'),
      '#prefix' =>  '<div class="addedit-form-cancel-container-actions admin-session-save-button admin-save-button-container resize-save_btn"><div class="white-btn-bg-left"></div>',
      '#suffix' =>  '<div class="white-btn-bg-right"></div>',
      '#name' => 'Cancel Session',
      '#submit' => array('cancelSessionDetails'),
      '#attributes' => $cancelAttributes,
	);


	$buttonName  = 'set-session';
	$setAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
	$setAttributes['data-wrapperid'] = array($wrapperId);
	$form[$formName]['set_session_edit_mode'] = array(
      '#type' => 'submit',
      '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
      '#suffix' => '<div class="admin-save-button-right-bg"></div></div></div>',
      '#title' => t('LBL569'),
      '#value' => t('LBL569'),
      '#name' => $buttonName,
      '#attributes' => $setAttributes,
      '#ajax' => array(
  	    'wrapper' => $wrapperId,
  	    'callback' => 'add_class_session_callback',
        'entity_id' => $entityId,
        'entity_type' => $entityType,
				'course_id' => $courseId,
        'entity_plugin'=>'session',
				//'customButtonId'=>$customHiddenButtonId,
        'method' => 'replace',
        'effect' => 'none', // 'fade',
        'event'=>'click',
        'keypress' => true,
        'progress' => array(
          'type' => 'throbber',
          'message' => ''
          ),
          ),
          );


          $form_state['storage']['mode'] = '';

          return $form;
      } catch (Exception $ex) {
          watchdog_exception('add_class_session_html', $ex);
          expertusErrorThrow($ex);
       }
}

function add_class_session_callback(&$form, &$form_state){
	/*$form['input'] = array();
	 $form['values'] = array();*/
	try {
	$commands = array();
	if($form_state["storage"]["form_validate"] == true){
		$courseId = $form_state['course_id'];
		$classId = $form_state['class_id'];
		$oldClassId = $form_state['oldClassId'];
		$delType = $form_state['values']['cls_delivery_type_'.$classId];
		displaySessionDetails($form, $form_state, $courseId, $classId, 'cre_sys_obt_cls', '', $delType, false,$oldClassId);
		$formName = "session_details_fieldset";
		$commands[] = ajax_command_replace(NULL, drupal_render($form[$formName]));
	}else{
		$formName = "classSession";
		$commands[] = ajax_command_replace('#add_session_details', drupal_render($form[$formName]));
	}

	$statusMsgHTML = theme('status_messages');

	expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true) , 4);

	if (!empty($statusMsgHTML)) {
		// Show the messages
		$commands[] = ajax_command_html(' #bubble-face-table #show_vc_detail_message', $statusMsgHTML);//ajax_command_prepend(NULL, $statusMsgHTML);
		// Set the trigger to remove the messages after a while
		if (!empty($form_state['triggering_element']['#ajax']['wrapper'])) {
			$commands[] = addedit_hide_messages($form_state['triggering_element']['#ajax']['wrapper']);
		}
		else {
			$commands[] = addedit_hide_messages();
		}
	}
	//expDebug::dPrint('form storage add_class_session_callback '.print_r($form_state, 1));
	$refresh_grid = ($form_state['classType'] == 'new-class' ? true : false);
	//refreshing the narrow searcg results has been chaned from immediate to close as immediate mode will try to refresh the grid for each session addition
	//and the grid row id will set set to null after the first session itself. 
// 	if($form_state["storage"]["form_validate"]) {
		$commands[] = ajax_command_refresh_narrow_search_results('onclose', $refresh_grid);
// 	}
	//expDebug::dPrint('$commands in add_class_session_callback '.print_r($commands, 1));
	return array('#type' => 'ajax', '#commands' => $commands);

	} catch (Exception $ex) {
		watchdog_exception('add_class_session_callback', $ex);
		expertusErrorThrow($ex);
	}
}



function add_class_session_html_validate($form, &$form_state) {
	try {
	$classId = $form_state['values']['class_id'];
	$meetingType = $form_state['meet_type'];
	expDebug::dPrint(' meeting type -------> '.$meetingType);
	expDebug::dPrint("instructor session -->>> ".print_r($form_state['values'],true),5);
	
	$form_state["storage"]["form_validate"] = true;
	$form_state['values']['session_presenter'] = (strtolower($form_state['values']['session_presenter']) == "select") ? '' : $form_state['values']['session_presenter'];
	//	changes by ayyappans for 43810: Issue in Session Creation VC Class API when Expertus meeting is in inactive status
	if($form_state['call_from'] == "API" && $meetingType != 'Ilt') {
		if(!checkIfMeetingEnabled($meetingType)) {
			form_set_error('meet_type', $meetingType.' is not active');
			$form_state["storage"]["form_validate"] = false;
			$form_state['esign_show_popup']= false;
		}
	}

	if($form_state['call_from'] == "API"){
		$form_state['values']['load_multiselect_session_instructor'] = $form_state['input']['hid_instructor_id'];
	}
	
	if($form_state['call_from'] == "API" && updateSessionByRestAPI || $form_state['call_from'] == "API" && updateVCSessionByRestAPI){
		$form_state['values']['session_name'] = mb_strimwidth(stripslashes($form_state['values']['session_name']), 0, 50);//Fix for 65843
	}
	
	if (empty($form_state['values']['session_name'])) {
		form_set_error('session_name',$deliveryType == 'lrn_cls_dty_vcl' ? t('LBL1316') : t('LBL295').t('ERR101'));
		$form_state["storage"]["form_validate"] = false;
		$form_state['esign_show_popup']= false;
	}
	$deliveryType= adminGetDeliveryType($classId);
	if($deliveryType == 'lrn_cls_dty_vcl') {
		$time_zone = $form_state['values']['time_zone_list'];
		if(empty($time_zone) && $form_state['values']['cls_delivery_type_'.$classId] == 'lrn_cls_dty_vcl'){
			form_set_error('session_details_fieldset][session_details][time_zone_list]',t('LBL297').t('ERR101'));
			$form_state["storage"]["form_validate"] = false;
			$form_state['esign_show_popup']= false;
		}
      
		if($meetingType != 'lrn_cls_vct_exp' && $meetingType != 'lrn_cls_vct_oth' && $meetingType != 'lrn_cls_vct_web'){
			if (empty($form_state['values']['session_meeting_id'])) {
				form_set_error('session_details_fieldset][session_details][session_meeting_id]',t('LBL858').t('ERR101'));
				$form_state["storage"]["form_validate"] = false;
				$form_state['esign_show_popup']= false;
			}
			if (empty($form_state['values']['session_attende_password'])) {
				form_set_error('session_details_fieldset][session_details][session_attende_password]',t('LBL859').t('ERR101'));
				$form_state["storage"]["form_validate"] = false;
				$form_state['esign_show_popup']= false;
			}

			if (empty($form_state['values']['session_presenter_password'])) {
				form_set_error('session_details_fieldset][session_details][session_presenter_password]',t('LBL860').t('ERR101'));
				$form_state["storage"]["form_validate"] = false;
				$form_state['esign_show_popup']= false;
			}
		}
		if($meetingType == 'lrn_cls_vct_oth'){
			if (!empty($form_state['values']['session_attendee_url'])) {
				if (!valid_url($form_state['values']['session_attendee_url'],TRUE)) { // Check email address format
					form_set_error('email', t('ERR208'));
					$form_state["storage"]["form_validate"] = false;
					$form_state['esign_show_popup']= false;
				}
			}
			else if (empty($form_state['values']['session_attendee_url'])) {
				form_set_error('session_details_fieldset][session_details][session_attendee_url]',t('LBL296').t('ERR101'));
				$form_state["storage"]["form_validate"] = false;
				$form_state['esign_show_popup']= false;
			}
			if (!empty($form_state['values']['session_presenter_url'])) {
				if (!valid_url($form_state['values']['session_presenter_url'],TRUE)) { // Check email address format
					form_set_error('email', t('ERR208'));
					$form_state["storage"]["form_validate"] = false;
					$form_state['esign_show_popup']= false;
				}
			}
			else if (empty($form_state['values']['session_presenter_url'])) {
				form_set_error('session_details_fieldset][session_details][session_presenter_url]',t('LBL977').t('ERR101'));
				$form_state["storage"]["form_validate"] = false;
				$form_state['esign_show_popup']= false;
			}

		}
		if($meetingType == 'lrn_cls_vct_web'){
		 if(!empty($form_state['values']['load_multiselect_session_instructor']) && empty($form_state['values']['session_presenter'])){
		 	form_set_error('session_details_fieldset][session_details][session_presenter]',t('LBL1314').t('ERR101'));
		 	$form_state["storage"]["form_validate"] = false;
		 	$form_state['esign_show_popup']= false;
		 }
		}

	}
	// Check for Session date
	$startdate = $form_state['values']['start_date'];
	if (empty($startdate) || $startdate =='mm-dd-yyyy') {
		form_set_error('start_date', t('LBL648').t('ERR101'));
		$form_state["storage"]["form_validate"] = false;
		$form_state['esign_show_popup']= false;
	}
	$enddate = $form_state['values']['end_date'];
	if($form_state['rec_sess_enabled']==true){
	if (empty($enddate) || $enddate =='mm-dd-yyyy') {
			form_set_error('end_date', t('LBL3054').t('ERR101'));
			$form_state["storage"]["form_validate"] = false;
			$form_state['esign_show_popup']= false;
	}
	/*Viswanathan added for #75685 */
	$StartDate = str_replace("-","/", $startdate);
	$EndDate = str_replace("-","/", $enddate);
	if(strtotime($EndDate) < strtotime($StartDate)){
		form_set_error('end_date',t('ERR059'));
		$form_state["storage"]["form_validate"] = false;
		$form_state['esign_show_popup']= false;
		
	}
	}
	//get course completion date if it is enables
	
	$selcompDate = db_select('slt_course_template', 'crs');
	$selcompDate->innerJoin('slt_course_class','cls','crs.id = cls.course_id ');
	$selcompDate->addExpression("DATE_FORMAT((crs.complete_date),'%m-%d-%Y')", 'complete_date');
	$selcompDate->condition('cls.id', $classId, '=');
	$compDate = $selcompDate->execute()->fetchField();
	
	if(!empty($compDate) && $compDate <= $enddate){
		form_set_error('end_date', t('MSG725'));
		$form_state["storage"]["form_validate"] = false;
		$form_state['esign_show_popup']= false;
	}
	
	/* Start #0050212 - Added by ganesh babu v for avoid fatal error during security fix on June 12 2015 12:47 PM */
	expDebug::dPrint("Call From API :".$form_state['call_from'], 5);
	if($form_state['call_from'] != "API"){
		if(!validateDateFormat($form_state['values']['start_date'],'m-d-Y')){
			form_set_error('start_date', t('ERR242'));
			$form_state["storage"]["form_validate"] = false;
			$form_state['esign_show_popup']= false;
		}
		if($form_state['rec_sess_enabled']==true){
			if(!validateDateFormat($form_state['values']['end_date'],'m-d-Y')){
				form_set_error('end_date', t('ERR242'));
				$form_state["storage"]["form_validate"] = false;
				$form_state['esign_show_popup']= false;
			}
		}
	}
	
	
	
	/* End #0050212 */

	/* $instructorname = $form_state['input']['session_details_name_instructor_'];
	expDebug::dPrint('$instructorname ='.$instructorname);
	if((empty($instructorname) || $instructorname == t('LBL181')) && $form_state['call_from'] != "API"){
		$instructorId='';
	}else {
		$instructorId = $form_state['input']['hid_instructor_id'];
	} */
	$instructorId = array_filter(explode(',',$form_state['values']['load_multiselect_session_instructor']));
	$locationId   = $form_state['values']['ses_location_id'];
	expDebug::dPrint("instructor Value -->> ".count($instructorId),5);
	$tempCDate =  $form_state['values']['start_date'];
	$tempEDate = $form_state['values']['end_date'];
	//
	$old_startdateformat  = str_replace("-","/",$tempCDate);
	$start_date_rec      = date_format(date_create( $old_startdateformat),'Y-m-d H:i:s');
	$old_enddateformat  = str_replace("-","/",$tempEDate);
	$end_date_rec     = date_format(date_create( $old_enddateformat),'Y-m-d H:i:s');
	$days = array("Mon","Tue","Wed","Thu","Fri","Sat","Sun");
	foreach($days as $day){
		if($form_state['input'][$day]==1){
			$day_array[]= $day;
		}
	}
	$InvalidDays =  getDateForSpecificDayBetweenDates($start_date_rec,$end_date_rec,$day_array,1);
	
	$countdays=0;
	foreach($day_array as $selectedDay){
		if(count($InvalidDays[$selectedDay])<=0){
			$countdays++;
		}
	}
	if($form_state['rec_sess_enabled']==true){
	if($tempCDate!= 'mm-dd-yyyy' && $tempEDate!= 'mm-dd-yyyy' && $countdays>0){
		if( ($form_state['call_from'] != "API" && $enddate > $startdate) || $form_state['call_from'] == "API"){
		form_set_error('invalid days', t('LBL3055'));
		$form_state["storage"]["form_validate"] = false;
		$form_state['esign_show_popup']= false;
		}
	}
	if($tempCDate!= 'mm-dd-yyyy' && $tempEDate!= 'mm-dd-yyyy' && count($day_array)==0 && $tempCDate!=$tempEDate && ($enddate > $startdate)) {
		form_set_error('invalid dates', t('LBL3056'));
		$form_state["storage"]["form_validate"] = false;
		$form_state['esign_show_popup']= false;
	} 
	}
	$datesforsession =  getDateForSpecificDayBetweenDates($start_date_rec,$end_date_rec,$day_array);
	//
	if(count($datesforsession) == 0){
		$datesforsession[0] =  $form_state['values']['start_date'];
	}
	for($k=0;$k<count($datesforsession);$k++) {
		$tempstartdate =   count(getDateForSpecificDayBetweenDates($start_date_rec,$end_date_rec,$day_array)) == 0 ? $datesforsession[$k] :(($form_state["call_from"] == "API")? $datesforsession[$k] : date("m-d-Y", strtotime($datesforsession[$k])));
	$tempStartHours = explode(':',$form_state['values']['start_hours']);
	$tempCSHrs = $tempStartHours[0];
	$tempCSMins = $tempStartHours[1];
	$tempEndHours = explode(':',$form_state['values']['end_hours']);
	$tempCEHrs = $tempEndHours[0];
	$tempCEMins = $tempEndHours[1];
	$minute_defaultval= array('00','15','30','45'); // Minute valdiator For Session stored in array
	expDebug::dPrint('$minute_default value : '.print_r($minute_defaultval ,true), 4);
	if(($tempCSHrs > $tempCEHrs) || (($tempCSHrs == $tempCEHrs) && ($tempCSMins > $tempCEMins))){
		$form_state["storage"]["form_validate"] = false;
		form_set_error('end_hours', t('ERR140'));
		$form_state['esign_show_popup']= false;
	}
	else if(($tempCSHrs == $tempCEHrs) && ($tempCSMins == $tempCEMins)){
		form_set_error('end_hours', t('ERR141'));
		$form_state["storage"]["form_validate"] = false;
		$form_state['esign_show_popup']= false;
	}else if(!in_array($tempCSMins, $minute_defaultval) || !in_array($tempCEMins, $minute_defaultval)){ // minute Validation for this ticket #0042366
		form_set_error('invalid session time', t('ERR141'));
		$form_state["storage"]["form_validate"] = false;
		$form_state['esign_show_popup']= false;
	}
	//current sessions date and time construct strings
	if($form_state["call_from"] == "API"){
		list($y,$m,$d) = explode("-",$tempstartdate);
	}
	else{
		list($m,$d,$y) = explode("-",$tempstartdate);
	}
	 if(!form_get_errors()){ // Added By Ram For Empty Session and Timezone throws Error
	//Added by Vincent on Nov 08, 2013 for #0029222
	if($deliveryType == 'lrn_cls_dty_vcl') {
    $time_zone = $form_state['values']['time_zone_list'];
  }else{
    $time_zone = $form_state['values']['ses_loc_timezone'];
  }
	if($time_zone){
  	 $tzArray   = getProfileListItemByCode($time_zone);
  	 $time_zone = $tzArray[0]->attr2;
 	}else{
     $time_zone = date_default_timezone(FALSE);
	}
	$startDTStr = $tempcurDateFormat." ".$tempCSHrs.":".$tempCSMins;
	$endDTStr = $tempcurDateFormat." ".$tempCEHrs.":".$tempCEMins;
    
	$vSesTimeZone = $time_zone;
	$stDate = $y.'-'.$m.'-'.$d. " " .$tempCSHrs.":".$tempCSMins;
	$enDate = $y.'-'.$m.'-'.$d. " " .$tempCEHrs.":".$tempCEMins;
	//convert the session time to system time from session timezone
	$vSysPHPTimezonename = date_default_timezone(FALSE);
  $ses_start_date = timeZoneConvert($stDate, $vSesTimeZone,  $vSysPHPTimezonename);
  $ses_end_date = timeZoneConvert($enDate, $vSesTimeZone,  $vSysPHPTimezonename);
  $cStartDT = strtotime($ses_start_date->format('Y-m-d H:i:s'));
  $cEndDT = strtotime($ses_end_date->format('Y-m-d H:i:s'));

//	expDebug::dPrint("START DATE -- >".$stDate."  --- END DATE -- ".$enDate."  ---- locationId -- ".$locationId,5);
// 	expDebug::dPrint("CONVERTED START DATE -- >".$ses_start_date->format('Y-m-d H:i:s')."  --- END DATE -- ".$ses_end_date->format('Y-m-d H:i:s'),5);
// 	expDebug::dPrint("TIMEZONE ---> ".$vSesTimeZone."  ----  ".$vSysPHPTimezonename,5);


	//traversing through all the available sessions in the class
	$sessId = ($form_state['is_new'] == "Edit") ? $form_state['values']['hid_session_id'] : '';
	$sessionDets = array();
	$sessionAvailable = 1;
	$sessionDets = fetchSessionDetailsShort($classId,$sessId);
	if(count($sessionDets) == 0){
		$sessionDets[0]->start_date = $form_state['values']['start_date'];
		$sessionDets[0]->start_hours = $form_state['values']['start_hours'];
		$sessionDets[0]->end_hours = $form_state['values']['end_hours'];
		$sessionAvailable = 0;
	}
	expDebug::dPrint("Session count" . count($sessionDets),4);
	for($j=0;$j<count($sessionDets);$j++) {

		$tempPDate  = $sessionDets[$j]->ses_start_date;
		$tempPSTime = $sessionDets[$j]->start_hours;
		$tempPETime = $sessionDets[$j]->end_hours;
		$sessionID  = $form_state['values']['hid_session_id'];
		$tempInstructorID = $form_state['input']['hid_instructor_id'];
		$tempLocationID = $form_state['values']['ses_location_id'];
	
		//condtion to check whether the id is available
		if($tempPDate != '' && $tempPDate != 'undefined') {

			//previous sessions date and time construct strings
      $prevStartDTStr = $tempPDate." ".$tempPSTime;
      $prevEndDTStr = $tempPDate." ".$tempPETime;

      $vSesTimeZone = empty($sessionDets[$j]->session_timezone)?$vSesTimeZone:$sessionDets[$j]->session_timezone;

			$vSysPHPTimezonename = date_default_timezone(FALSE);
   		$ses_start_date = timeZoneConvert($prevStartDTStr, $vSesTimeZone,  $vSysPHPTimezonename);
	  	$ses_end_date = timeZoneConvert($prevEndDTStr, $vSesTimeZone,  $vSysPHPTimezonename);
			//previous session date time construct
			$pStartDT = strtotime($ses_start_date->format('Y-m-d H:i:s'));
			$pEndDT = strtotime($ses_end_date->format('Y-m-d H:i:s'));
			if($sessionAvailable == 1){
				if(($cStartDT < $pStartDT && $cEndDT <= $pStartDT) || ($cStartDT >= $pEndDT && $cEndDT > $cStartDT)) {

				}else {
					form_set_error('start_date', t('ERR107'));
					$form_state["storage"]["form_validate"] = false;
					$form_state['esign_show_popup']= false;
					break;
				}
			}
		}
		// Check for is the instructor is valid or not
		/* if(!empty($form_state['input']['session_details_name_instructor_']) && ($form_state['input']['session_details_name_instructor_'] != t('LBL181'))) {
		  //$instName = preg_replace('\(.*?\)~', '', $form_state['input']['session_details_name_instructor_']);
		  $instName = $form_state['input']['session_details_name_instructor_'];
		  $nameSplit = array();
		  if(preg_match('/[(]/',$instName)){
		    $nameSplit = explode('(',$instName);
		    $instName = $nameSplit[0];
		  }
		  else {
		    $nameSplit[0] = $instName;
		  }
		  if(!empty($nameSplit[0])) {
		    $instructorDetail=db_query("select id from {slt_person} WHERE full_name =:fullname",array(':fullname' => $nameSplit[0]));
		  }
		  $countresult = $instructorDetail->rowCount();
	      $instructorResult = $instructorDetail->fetchAll();
	      if($countresult == 0){
	          form_set_error('session_details_name_instructor_',t('ERR171').' '.t('Instructor'));
	          $form_state["storage"]["form_validate"] = false;
	          $form_state['esign_show_popup']= false;
	      }
		} */
		
		if(count($instructorId) > 1 && empty($form_state['values']['session_presenter']) && $deliveryType == 'lrn_cls_dty_vcl' && $meetingType == 'lrn_cls_vct_exp'){
			form_set_error('edit-session-presenter',t('LBL1315').t('ERR101'));
			$form_state["storage"]["form_validate"] = false;
			$form_state['esign_show_popup']= false;
		}else if(count($instructorId) > 1 && empty($form_state['values']['session_presenter']) && $deliveryType == 'lrn_cls_dty_vcl'){
			form_set_error('edit-session-presenter',t('Presenter').t('ERR101'));
			$form_state["storage"]["form_validate"] = false;
			$form_state['esign_show_popup']= false;
		}
		if($form_state['call_from'] == "API" && count($instructorId) == 0 && !empty($form_state['values']['session_presenter']) && $deliveryType == 'lrn_cls_dty_vcl'){
			form_set_error('edit-session-presenter',t('Select Presenter from instructor list'));
			$form_state["storage"]["form_validate"] = false;
			$form_state['esign_show_popup']= false;
		}
		if($form_state['call_from'] == "API"){
		if(count($instructorId) >0){		//check for valid instructor id for rest api -> 42597: Through API can able to assign non-instructor for ILT/VC class
			$i = 0;
			$invalid_ins = array();
			foreach($instructorId as $ins){
			$selectIns = db_select('slt_person', 'per');
				$selectIns->condition('id', $ins);
			$selectIns->condition('is_instructor', 'Y');
			$selectIns->condition('status', 'cre_usr_sts_atv');
			$selectIns = $selectIns->countQuery();
			$countIns = $selectIns->execute()->fetchField();
			if($countIns < 1) {
					$invalid_ins[$i] = $ins;
					$i++;
				}
			}
			//0069117: Create/Update-ILT/VC Session API-Other Group users able to set as Instructor/presentor
			$instructorOption = getAllInstructor('session_api');
					
			if(!empty($instructorOption)) {
				$j = 0;
				$valid_ids = array();
				foreach($instructorOption as $opt){
					$valid_ids[$j] = $opt->id;
					$j++;
				}
			}
						
			$valid_ids = array_intersect($valid_ids,$instructorId);
						
			$invalid_ins = array_diff($instructorId,$valid_ids);
						
			if(count($invalid_ins) > 0 || empty($instructorOption) || (count($valid_ids) != count($instructorId))){
				form_set_error('hid_instructor_id', t('ERR171').' '.t('Instructor') .' - '.implode(',',$invalid_ins));
				$form_state["storage"]["form_validate"] = false;
				$form_state['esign_show_popup']= false;
			}
	    	}
		}
		
		if((count($instructorId) > 0) && !empty($form_state['values']['session_presenter'])){
			$err_flag = '';
			if((count($instructorId) == 1) && $form_state['values']['session_presenter'] != $instructorId[0]){
				$err_flag = 1;
			}else if(count($instructorId) > 1){
				$instructor_value = explode(',',$form_state['values']['load_multiselect_session_instructor']);
				$err_flag = (!in_array($form_state['values']['session_presenter'], $instructor_value)) ? 1 : '';
			}			
			
			if(!empty($err_flag)){
				form_set_error('edit-session-presenter',t('ERR171').' '.t('Presenter'));
				$form_state["storage"]["form_validate"] = false;
				$form_state['esign_show_popup']= false;
			}
		}
		//Check for instructor conflict
		if(count($instructorId)>0){ // Instructor name check is removed for #0022514
			$i=0;
			$invalid_ins = array();
			foreach($instructorId as $ins){
				$instructorScheduled = getInstructorScheduledSession($ins,$sessId);
			foreach($instructorScheduled as $id => $row){
				//Added by Vincent on Nov 08, 2013 for #0029222
				$vSesTimeZone = $row->session_timezone;
				$stDate = $row->ses_start_date. " " .$row->start_time;
				$enDate = $row->ses_start_date. " " .$row->end_time;
				$vSysPHPTimezonename = date_default_timezone(FALSE);
		     	$ses_start_date = timeZoneConvert($stDate, $vSesTimeZone,  $vSysPHPTimezonename);
			  $ses_end_date = timeZoneConvert($enDate, $vSesTimeZone,  $vSysPHPTimezonename);
			  $insSesStartTime = strtotime($ses_start_date->format('Y-m-d H:i:s'));
			  $insSesEndTime = strtotime($ses_end_date->format('Y-m-d H:i:s'));
				if(($insSesStartTime < $cStartDT && $insSesEndTime <= $cStartDT) || ($insSesStartTime >= $cEndDT && $insSesEndTime > $insSesStartTime)) {

				}
				else{
						 $invalid_ins[$i] = $row->first_name.' '.$row->last_name;
						 $i++;
					}
				}
			}
			expDebug::dPrint("instructor conflict -->> ". print_r($invalid_ins,true),5);
			if(count($invalid_ins) > 0){
				$invalid_ins = array_unique($invalid_ins);
				$err_msg = implode(',',$invalid_ins).'-'.t('MSG435');
				form_set_error('start_date', $err_msg);
					$form_state["storage"]["form_validate"] = false;
					$form_state['esign_show_popup']= false;
					break;
				}
			}

		//Added by Vincent on Nov 08, 2013 for #0029222
		//Check for instructor has any enrollment conflict
		if(count($instructorId)>0){ // Instructor name check is removed for #0022514
			$err_flag = 0;
			$i=0;
			$invalid_ins_enr = array();
			foreach($instructorId as $ins){
			$instructorScheduled = getInstructorEnrolledSession($instructorId);
			foreach($instructorScheduled as $id => $row){
				$vSesTimeZone = $row->session_timezone;
				$stDate = $row->ses_start_date. " " .$row->start_time;
				$enDate = $row->ses_start_date. " " .$row->end_time;
				$vSysPHPTimezonename = date_default_timezone(FALSE);
		   	$ses_start_date = timeZoneConvert($stDate, $vSesTimeZone,  $vSysPHPTimezonename);
			  $ses_end_date = timeZoneConvert($enDate, $vSesTimeZone,  $vSysPHPTimezonename);
			  $insSesStartTime = strtotime($ses_start_date->format('Y-m-d H:i:s'));
			  $insSesEndTime = strtotime($ses_end_date->format('Y-m-d H:i:s'));
				if(($insSesStartTime < $cStartDT && $insSesEndTime <= $cStartDT) || ($insSesStartTime >= $cEndDT && $insSesEndTime > $insSesStartTime)) {

				}
				else{
						$invalid_ins_enr[$i] = $row->first_name.' '.$row->last_name;
						$i++;
						$err_flag = 1;
					}
				}
				if($err_flag == 1){
					expDebug::dPrint("instructor conflict enrolled -->> ". print_r($invalid_ins_enr,true),5);
					$invalid_ins_enr = array_unique($invalid_ins_enr);
					$err_msg = implode(',',$invalid_ins_enr).'-'.t('MSG435');
					form_set_error('start_date', $err_msg);
					$form_state["storage"]["form_validate"] = false;
					$form_state['esign_show_popup']= false;
					break;
				}
			}
		}
		// Check for location conflict
		if(!empty($locationId)){
			$locationScheduled = getLocationScheduledSession($locationId,$sessId);
			foreach($locationScheduled as $id => $row){
				$roomSessId    = $row->sessionid;

				//Added by Vincent on Nov 08, 2013 for #0029222
				$vSesTimeZone = $row->session_timezone;
				$stDate = $row->ses_start_date. " " .$row->start_time;
				$enDate = $row->ses_start_date. " " .$row->end_time;
				$vSysPHPTimezonename = date_default_timezone(FALSE);
		   	$ses_start_date = timeZoneConvert($stDate, $vSesTimeZone,  $vSysPHPTimezonename);
			  $ses_end_date = timeZoneConvert($enDate, $vSesTimeZone,  $vSysPHPTimezonename);
			  $roomSesStartTime = strtotime($ses_start_date->format('Y-m-d H:i:s'));
			  $roomSesEndTime = strtotime($ses_end_date->format('Y-m-d H:i:s'));

				if(($roomSesStartTime < $cStartDT && $roomSesEndTime <= $cStartDT) || ($roomSesStartTime >= $cEndDT && $roomSesEndTime > $roomSesStartTime) || ($sessionID == $roomSessId) || ($tempLocationID != $locationId) ) {
				}
				else{
					form_set_error('start_date', t('ERR146'));
					$form_state["storage"]["form_validate"] = false;
					$form_state['esign_show_popup']= false;
					break;
				}
			}
		}
	}
  }
	}
	} catch (Exception $ex) {
		watchdog_exception('add_class_session_html_validate', $ex);
		expertusErrorThrow($ex);
	}
}
function getDateForSpecificDayBetweenDates($startDate,$endDate,$days,$validate=''){
	//$days = explode(',', $days);
	$date_array = array();
	$Weekdays= array();
	$endDate = strtotime($endDate);
	foreach($days as $day){
		$week_date_array = array();
		for($i = strtotime($day, strtotime($startDate)); $i <= $endDate; $i = strtotime('+1 week', $i)){
			$date_array[]=date('Y-m-d',$i);
			$week_date_array[]=date('Y-m-d',$i);
		}
		$Weekdays[$day] = $week_date_array; 
	}
	expDebug::dPrint('add_forum_dayss check no of days '.print_r($date_array,true) , 5);
	if($validate ==1 ){
		return $Weekdays;
	}
	return $date_array;
}
function add_class_session_html_submit($form, &$form_state) {
	try {
	expDebug::dPrint('add_forum_topic_html_submit : '.print_r($form_state,true) , 5);
	$courseId = $form_state['course_id'];
	$classId = $form_state['class_id'];
	if(($form_state['is_new'] == "Add")||($form_state['is_new'] == "Addanother")){
		$old_startdateformat  = str_replace("-","/",$form_state['values']['start_date']);
		$start_date      = date_format(date_create( $old_startdateformat),'Y-m-d H:i:s');
		$old_enddateformat  = str_replace("-","/",$form_state['values']['end_date']);
		$end_date      = date_format(date_create( $old_enddateformat),'Y-m-d H:i:s');
		$days = array("Mon","Tue","Wed","Thu","Fri","Sat","Sun");
		foreach($days as $day){
		 if($form_state['input'][$day]==1){
		 	$day_array[]= $day;
		 }
		}
		$datesforsession =  getDateForSpecificDayBetweenDates($start_date,$end_date,$day_array);
		if(!empty($datesforsession) && $form_state['rec_sess_enabled']==true){
		foreach($datesforsession as $rec_sess){
			$sessionId = addNewSessionDetailsNew($form, $form_state, $courseId, $classId,'',$rec_sess);
		 }
		}
		
		else{
		$sessionId = addNewSessionDetailsNew($form, $form_state, $courseId, $classId);
		}
	}else{
		$sessionId = updateSessionDetailsNew($form, $form_state, $courseId, $classId);
	}


	durationTimeOperations($classId);
	$form_state['rebuild']  = TRUE;
	if($form_state['call_from'] == "API")
		return $sessionId;
	} catch (Exception $ex) {
		watchdog_exception('add_class_session_html_submit', $ex);
		expertusErrorThrow($ex);
	}
}

function addNewSessionDetailsNew($form, &$form_state,$courseId,$entityId,$meetingType,$rec_sess='') {

	$txn = db_transaction();

	try {
		if($form_state['values']['create_user_id'])
		$loggedInUserId = $form_state['values']['create_user_id'];
		else
		$loggedInUserId  = getIdOfLoggedInUser();
		$meetingType = $form_state['meet_type'];
		$tzone = ($meetingType == "Ilt") ? $form_state['values']['ses_loc_timezone'] : $form_state['values']['time_zone_list'];
		$meetingType = ($meetingType == "Ilt") ? '' : $meetingType;
		$insertStmt = db_insert('slt_course_class_session');
		$custom  = NULL;
		
		$old_dateformat  = str_replace("-","/",$form_state['values']['start_date']);
		$start_date      = !empty($rec_sess)? $rec_sess : date_format(date_create( $old_dateformat),'Y-m-d H:i:s'); 
		
		$old_dateformat  = str_replace("-","/",$form_state['values']['end_date']);
		$end_date      = !empty($rec_sess)? $rec_sess : date_format(date_create( $old_dateformat),'Y-m-d H:i:s');

		$start_time = $form_state['values']['start_hours'];
		$end_time   = $form_state['values']['end_hours'];
		$entityType = 'cre_sys_obt_cls';
		$uniqueId = $entityId.'-'.$entityType;
		expDebug::dPrint("testing session id ");
		$callfrom = $form_state['call_from'];
		if($callfrom == 'API'){
			$instructorHId = array_filter(explode(',',$form_state['values']['session_details_fieldset']['session_details']['instructor_id']));
		}else{
			$instructorHId = array_filter(explode(',',$form_state['values']['load_multiselect_session_instructor']));
		}
		$instructorHName = $form_state['input']['session_details_name_instructor'];
		$instructorTxtName = $form_state['input']['session_details_name_instructor_'];
		expDebug::dPrint('Meeting Values= ' . print_r($form_state['input'], true) , 5);
		//$config=getConfig('exp_sp');
		//$meetingUrl = $config["meeting_url"];
		$meetingUrl = "";
		$attendeeUrl = null;
		$presenterUrl = null;
		$meetingId = null;
		//$meetingType  = ($form_state['values']['meeting_type']) ? $form_state['values']['meeting_type'] : null;
		if($meetingType){
			$meetingId         = $form_state['values']['session_meeting_id'];
			$attendeePassword  = $form_state['values']['session_attende_password'];
			$presenterPassword = $form_state['values']['session_presenter_password'];
			$attendeeURL  = $form_state['values']['session_attendee_url'];
			$presenterURL = $form_state['values']['session_presenter_url'];
		}
		$classId = $form_state['values']['class_id'];
		$delType = $form_state['values']['cls_delivery_type_'.$classId];
		if($meetingType == 'lrn_cls_vct_exp'){
			$attendeeUrl  = $meetingUrl.'/?q=learning/expertus/meeting/join/attendee/'.$classId;
			$presenterUrl = $meetingUrl.'/?q=learning/expertus/meeting/join/presenter/'.$classId;
		}
		elseif($meetingType == 'lrn_cls_vct_lmt'){
			$attendeeUrl  = $meetingUrl.'/?q=learning/live/meeting/join/attendee/'.$classId.'/'.$meetingId.'/'.$attendeePassword;
			$presenterUrl = $meetingUrl.'/?q=learning/live/meeting/join/presenter/'.$classId.'/'.$meetingId.'/'.$presenterPassword;
		}
		elseif($meetingType == 'lrn_cls_vct_oth'){
			$attendeeUrl  = $attendeeURL;
			$presenterUrl = $presenterURL;
		}else if($meetingType == 'lrn_cls_vct_web'){
			$attendeeUrl = $meetingUrl.'/?q=learning/webex/meeting/join/attendee/'.$classId;
			$presenterUrl = $meetingUrl.'/?q=learning/webex/meeting/join/presenter/'.$classId;
		}
		$fields = array(
                  'course_id'          => $form_state['values']['course_id'],
    			  'class_id'           => $form_state['values']['class_id'],
      			  'capacity_min'       => (empty($form_state['values']['ses_min_seats'])) ? NULL : $form_state['values']['ses_min_seats'],
    			  'capacity_max'       => $form_state['values']['ses_max_seats'],
    			  'waitlist_count'     => (empty($form_state['values']['ses_waitlist_count'])) ? NULL : $form_state['values']['ses_waitlist_count'],
                  'location_id'        => (empty($form_state['values']['ses_location_id'])) ? NULL : $form_state['values']['ses_location_id'],
                  //'instructor_id'      => (empty($instructorHId)) ? NULL : $instructorHId,
                  'title'              => $form_state['values']['session_name'],
                  'attendee_url'       => $attendeeUrl,
                  'presenter_url'      => $presenterUrl,
                  'timezone'           => $tzone,
      			  'type'               => $meetingType,
    			  'start_date'         => $start_date,
				//  'end_date'      => $end_date,
                  'created_on'		   => now(),
                  'created_by'		   => $loggedInUserId,
				  'updated_on'		   => now(),
				  'updated_by'		   => $loggedInUserId,
      			  'start_time'         => $start_time,
                  'end_time'           => $end_time,
                  'custom0'            => $custom,
                  'custom1'            => $custom,
                  'custom2'            => $custom,
                  'custom3'            => $custom,
                  'custom4'            => $custom
		);
		$fields = trimInputFields($fields);
		$insertStmt->fields($fields);

		expDebug::dPrint('inside session form State insert : '.print_r($form_state['values'],true) , 5);
		$sessionId = $insertStmt->execute();
		
		if($meetingType == 'lrn_cls_vct_web'){
			
			$attendeeUrl = $meetingUrl.'/?q=learning/webex/meeting/join/attendee/'.$classId.'/'.$sessionId;
			$updtfields = array(
					'attendee_url'=>$attendeeUrl,
					);
			$update=db_update('slt_course_class_session');
			$update->fields($updtfields);
			$update->condition('id',$sessionId);
			$update->condition('class_id',$classId);
			expDebug::dPrintDBAPI('update session with meeting id' ,$update);
			$result=$update->execute();
		}
		
	//$presenter_id = ($meetingType && (count($instructorHId) == 1)) ? $instructorHId[0] : '';
		if($meetingType && (count($instructorHId) == 1)){
			$Insfields = array(
					'course_id' => $form_state['values']['course_id'],
					'class_id' => $form_state['values']['class_id'],
					'session_id' => $sessionId,
					'instructor_id'=> $instructorHId[0],
					'is_presenter' => "Y",
					'created_by' => $loggedInUserId,
					'created_on' => now(),
					'updated_by' => $loggedInUserId,
					'updated_on' => now(),
			);
			$insertIns = db_insert('slt_session_instructor_details');
			$insertIns->fields($Insfields);
			expDebug::dPrintDBAPI(' Instructor session = ' ,$insertIns);
			$session_instructor = $insertIns->execute();
		}else{
			foreach($instructorHId as $ins){
				if( ($form_state['values']['session_presenter'] == $ins)){
					$presenter = 'Y';
					$presenter_id = empty($presenter_id) ? $ins : $presenter_id;
				}
				else{
					$presenter = 'N';
				}

				$Insfields = array(
						'course_id' => $form_state['values']['course_id'],
						'class_id' => $form_state['values']['class_id'],
						'session_id' => $sessionId,
						'instructor_id'=> $ins,
						'is_presenter' => $presenter,
						'created_by' => $loggedInUserId,
						'created_on' => now(),
						'updated_by' => $loggedInUserId,
						'updated_on' => now(),
				);
				$insertIns = db_insert('slt_session_instructor_details');
				$insertIns->fields($Insfields);
				expDebug::dPrintDBAPI(' Instructor session = ' ,$insertIns);
				$session_instructor = $insertIns->execute();
			}
		}
		//Don't delete this update this is report flat table related update.
		$updComment  = db_update('slt_course_class');
		$updComment->fields(array(
				'updated_on' => now(),
		));
		$updComment->condition('id', $form_state['values']['class_id']);
		expDebug::dPrintDBAPI(' session delete update for class SQL = ' , $updComment);
		$updComment->execute();
		$default_ins = array((object)array('instructor_id' => ''));
		make_audit_trail_entries(array('instructor_id' => implode(',',$instructorHId)), $loggedInUserId,$default_ins, $sessionId ,'Instructor Added','cre_sys_obt_cls','exp_sp_administration_session_details','InsertSessionInstructor',
				array('instructor_id'));
		
		if(!empty($presenter_id)){
			$default_pre = array((object)array('presenter_id' => ''));
			make_audit_trail_entries(array('presenter_id' => $presenter_id), $loggedInUserId,$default_pre,$sessionId ,'Presenter Inserted','cre_sys_obt_cls','exp_sp_administration_session_details','InsertSessionPresenter',
					array('presenter_id'));
		}
		
		make_audit_trail_entries($fields, $loggedInUserId , null, $sessionId ,'Inserted','cre_sys_obt_cls','exp_sp_administration_session_details','addNewSessionDetails',
		array('course_id', 'class_id', 'capacity_min', 'capacity_max', 'waitlist_count', 'location_id','instructor_id','facility_id', 'room_id', 'title', 'attendee_url','presenter_url', 'timezone', 'start_date', 'start_time', 'end_time'));

		expDebug::dPrint(' $$sessionId = ' . print_r($sessionId, true) , 3);
		$form_state['storage']['mode'] = 'edit';
		return array((object)array('id'=>$sessionId));
	}
	catch (Exception $ex) {
		$txn->rollback();
		watchdog_exception('addNewSessionDetailsNew', $ex);
		throw $ex;
	}

	unset($txn);

}


function updateSessionDetailsNew($form, &$form_state,$session_id,$entityType) {

	$txn = db_transaction();

	try {
		if($form_state['values']['create_user_id'])
		$loggedInUserId = $form_state['values']['create_user_id'];
		else
		$loggedInUserId  = getIdOfLoggedInUser();
		$updateStmt = db_update('slt_course_class_session');
		$default= db_query("SELECT id,course_id,class_id,capacity_min,capacity_max,waitlist_count,location_id,facility_id,room_id,title,attendee_url,timezone,start_date,start_time,end_time,custom4 from slt_course_class_session where id=:uid",array(":uid"=>$form_state['values']['hid_session_id']))->fetchAll();
		$def_insDetails= db_query("SELECT instructor_id,is_presenter from slt_session_instructor_details where session_id=:uid",array(":uid"=>$form_state['values']['hid_session_id']))->fetchAllKeyed();
		//multi instructor related work.
		$last_run_date = date("Y-m-d");
		$select = db_select('slt_session_instructor_details','ses_ins');
		$select->addField('ses_ins', 'instructor_id');
		$select->condition('ses_ins.session_id', $form_state['values']['hid_session_id']);
		$select_or = db_or();
		$select_or->where("DATE_FORMAT(ses_ins.created_on, '%Y-%m-%d') < DATE_FORMAT("."'".$last_run_date."'".",  '%Y-%m-%d')");
		$select_or->where("DATE_FORMAT(ses_ins.updated_on, '%Y-%m-%d') < DATE_FORMAT("."'".$last_run_date."'".", '%Y-%m-%d')");
		$select->condition($select_or);
		$default_ins = $select->execute()->fetchCol();
		$custom  = NULL;

		$old_dateformat  = str_replace("-","/",$form_state['values']['start_date']);
		$start_date      = date_format(date_create( $old_dateformat),'Y-m-d H:i:s');

		$start_time = $form_state['values']['start_hours'];
		$end_time   = $form_state['values']['end_hours'];

		$classId = $form_state['values']['class_id'];
		$delType = $form_state['values']['cls_delivery_type_'.$classId];
		$callfrom = $form_state['call_from'];
		if($callfrom == 'API'){
			$instructorHId = array_filter(explode(',',$form_state['values']['session_details_fieldset']['session_details']['instructor_id']));
		}else{
		$instructorHId = array_filter(explode(',',$form_state['values']['load_multiselect_session_instructor']));
		}
		$instructorHName   = $form_state['input']['session_details_name_instructor'];
		$instructorTxtName = $form_state['input']['session_details_name_instructor_'];
		if($form_state['call_from'] == "API")
			$instructorTxtName = 'dummy';
		//$config=getConfig('exp_sp');
		//$meetingUrl = $config["meeting_url"];
		$meetingUrl = "";
		$attendeeUrl = null;
		$presenterUrl = null;
		$meetingId = null;
		$meetingType = $form_state['meet_type'];
		$tzone = ($meetingType == "Ilt") ? $form_state['values']['ses_loc_timezone'] : $form_state['values']['time_zone_list'];
		$meetingType = ($meetingType == "Ilt") ? '' : $meetingType;
		//$meetingType  = ($form_state['values']['meeting_type']) ? $form_state['values']['meeting_type'] : null;
		if($meetingType){
			$meetingId         = $form_state['values']['session_meeting_id'];
			$attendeePassword  = $form_state['values']['session_attende_password'];
			$presenterPassword = $form_state['values']['session_presenter_password'];
			$attendeeURL  = $form_state['values']['session_attendee_url'];
			$presenterURL = $form_state['values']['session_presenter_url'];
		}
		$classId = $form_state['values']['class_id'];
		if($meetingType == 'lrn_cls_vct_web'){
			$attendeeUrl  = $meetingUrl.'/?q=learning/webex/meeting/join/attendee/'.$classId.'/'.$form_state['values']['hid_session_id'];			
			$presenterUrl = $meetingUrl.'/?q=learning/webex/meeting/join/presenter/'.$classId;
		}
		elseif($meetingType == 'lrn_cls_vct_exp'){
			$attendeeUrl  = $meetingUrl.'/?q=learning/expertus/meeting/join/attendee/'.$classId;
			$presenterUrl = $meetingUrl.'/?q=learning/expertus/meeting/join/presenter/'.$classId;
		}
		elseif($meetingType == 'lrn_cls_vct_lmt'){
			$attendeeUrl  = $meetingUrl.'/?q=learning/live/meeting/join/attendee/'.$classId.'/'.$meetingId.'/'.$attendeePassword;
			$presenterUrl = $meetingUrl.'/?q=learning/live/meeting/join/presenter/'.$classId.'/'.$meetingId.'/'.$presenterPassword;
		}
		elseif($meetingType == 'lrn_cls_vct_oth'){
			$attendeeUrl  = $attendeeURL;
			$presenterUrl = $presenterURL;
		}
		//multi instructor related work.
		expDebug::dPrint('default instructor ids-->'.print_r($default_ins,1),4);
		expDebug::dPrint('default instructor ids pre-->'.print_r($default[0]->custom4,1),4);
		expDebug::dPrint('default instructor ids now-->'.print_r($instructorHId,1),4);
		$ins_diff_arr = array_diff($default_ins,$instructorHId);
		expDebug::dPrint('default instructor ids ins_diff_arr-->'.print_r($ins_diff_arr,1),4);
		//if(count($ins_diff_arr)>0){
		$pre_cus_ins = explode(',',$default[0]->custom4);
		$merge_arr = array_merge($pre_cus_ins,$ins_diff_arr);
		$uniq_ins = array_unique($merge_arr);
		expDebug::dPrint('default instructor ids uniq_ins-->'.print_r($uniq_ins,1),4);
		$ins_str = '';
		foreach ($uniq_ins as $ins){
			if(!in_array($ins,$instructorHId)){
				if(empty($ins_str))
					$ins_str = $ins;
				else
					$ins_str .=','.$ins;
			}
		}
		if(empty($ins_str))
		$custom4 = $custom;
		else
			$custom4 = $ins_str;
		//}
		expDebug::dPrint('default instructor ids custom4-->'.print_r($custom4,1),4);
		$fields = array(
                  'course_id'          => $form_state['values']['course_id'],
    			  'class_id'           => $form_state['values']['class_id'],
    			  'capacity_min'       => (empty($form_state['values']['ses_min_seats'])) ? NULL : $form_state['values']['ses_min_seats'],
    			  'capacity_max'       => $form_state['values']['ses_max_seats'],
    			  'waitlist_count'     => (empty($form_state['values']['ses_waitlist_count'])) ? NULL : $form_state['values']['ses_waitlist_count'],
    			 				'location_id'        => (empty($form_state['values']['ses_location_id'])) ? NULL : $form_state['values']['ses_location_id'],
    			  			//'instructor_id'      => (empty($instructorTxtName) || $instructorTxtName == t('LBL181') || empty($instructorHId)) ? NULL : $instructorHId,
                  'title'              => $form_state['values']['session_name'],
                  'attendee_url'       => $attendeeUrl,
                  'presenter_url'      => $presenterUrl,
                  'timezone'           => $tzone,
                  'type'               => $meetingType,
    			  			'start_date'         => $start_date,
      			  		'start_time'         => $start_time,
                  'end_time'           => $end_time,
                  'updated_on'         => now(),
    			  			'updated_by'		   	=> $loggedInUserId,
                  'custom0'            => $custom,
                  'custom1'            => $custom,
                  'custom2'            => $custom,
                  'custom3'            => $custom,
                  'custom4'            => $custom4
		);

		expDebug::dPrint('inside session form State : '.print_r($form_state['values'],true) , 5);
		$fields = trimInputFields($fields);
		$updateStmt->fields($fields);
		$updateStmt->condition('id', $form_state['values']['hid_session_id']);
		expDebug::dPrintDBAPI('updateSessionDetailsNew Velu = ', $updateStmt);
		$numUpdated = $updateStmt->execute();


		expDebug::dPrint(' ERRor AFTER0-----> ' . print_r($numUpdated, true) , 3);
		expDebug::dPrint(' after update def_insDetails = ' . print_r($def_insDetails, true) , 5);
		$default_ins = implode(',',array_keys($def_insDetails));
		//$presenter_id = ($meetingType && (count($instructorHId) == 1)) ? $instructorHId[0] : '';
		if($meetingType && (count($instructorHId) == 1)){
		    
			if(!in_array($instructorHId[0], explode(',',$default_ins))){
			    
			   // expDebug::dPrint(' session_id-----> ' . print_r($form_state['values']['hid_session_id'], true) , 3);
			    expDebug::dPrint('selecting session_instructor2-----> ');
			    
			    $select=db_select('slt_session_instructor_details','ins');
			    $select->addField('ins','id','id');
			    $select->addField('ins','instructor_id','p4');
			    $select->addField('ins','session_id','p3');
			    $select->addField('ins','class_id','p2');
			    $select->addField('ins','course_id','p1');
			    $select->condition('session_id', $form_state['values']['hid_session_id'],'=');
			    $select1=$select->execute()->fetchAll();
			    
			    expDebug::dPrint('deleting session_instructor2-----> ' . print_r($selectstmt1, true) , 3);
			    
				$deleteStmt = db_delete('slt_session_instructor_details');
				$deleteStmt->condition('session_id', $form_state['values']['hid_session_id'],'=');
				expDebug::dPrintDBAPI(' Delete session instructor = ' ,$deleteStmt);
				$deleteStmt->execute();
			     
				foreach($select1 as $val){
				    $fields= array(
				            'table_name' => 'slt_session_instructor_details',
				            'entity_id' => $val->id,
				            'entity_type' => 'instructor_mapping',
				            'parent1_entity_id' => $val->p1,
				            'parent1_entity_type' => 'course_id',
				            'parent2_entity_id' => $val->p2,
				            'parent2_entity_type' => 'class_id',
				            'parent3_entity_id' => $val->p3,
				            'parent3_entity_type' => 'session_id',
				            'parent4_entity_id' => $val->p4,
				            'parent4_entity_type' => 'instructor_id',
				            'module_name' => 'exp_sp_administration_session_details',
				            'functionality_name' => 'updateSessionDetailsNew',
				            'api_name' => $_REQUEST['apiname'],
				            'deleted_on' => now(),
				            'deleted_by' => $loggedInUserId,
				
				    );
				    deleted_log_entry($fields);
				}
				
				$Insfields = array(
						'course_id' => $form_state['values']['course_id'],
						'class_id' => $form_state['values']['class_id'],
						'session_id' => $form_state['values']['hid_session_id'],
						'instructor_id'=> $instructorHId[0],
						'is_presenter' => "Y",
						'created_by' => $loggedInUserId,
						'created_on' => now(),
						'updated_by' => $loggedInUserId,
						'updated_on' => now(),
				);
				$insertIns = db_insert('slt_session_instructor_details');
				$insertIns->fields($Insfields);
				expDebug::dPrintDBAPI(' Instructor session = ' ,$insertIns);
				$session_instructor = $insertIns->execute();
			}else{
				$presenter = 'Y';
				$updateStmt = db_update('slt_session_instructor_details');
				$updateStmt->fields(array('is_presenter' => $presenter));
				$updateStmt->condition('session_id', $form_state['values']['hid_session_id']);
				$updateStmt->condition('instructor_id', $instructorHId[0]);
				expDebug::dPrintDBAPI('updateSessionDetailsNew Single Velu = ', $updateStmt);
				$numUpdated = $updateStmt->execute();
				
				expDebug::dPrint('selecting session_instructor3-----> ');
				expDebug::dPrint('deleting session_instructor3-----> ' . print_r($form_state['values']['hid_session_id'], true) , 3);
				expDebug::dPrint('deleting session_instructor3-----> ' . print_r($instructorHId[0], true) , 3);
				
				$sesid=$form_state['values']['hid_session_id'];
				$insid=$instructorHId[0];
				
				$select=db_select('slt_session_instructor_details','ins');
				$select->addfield('ins','id','id');
				$select->addField('ins','instructor_id','p4');
				$select->addField('ins','session_id','p3');
				$select->addField('ins','class_id','p2');
				$select->addField('ins','course_id','p1');
				$select->condition('session_id', $sesid,'=');
				$select->condition('instructor_id',$insid,'!=');
				$select1=$select->execute()->fetchAll();
				
				$fields['parent1_entity_id']=$select1->p1;
				$fields['parent1_entity_type']='course_id';
				$fields['parent2_entity_id']=$select1->p2;
				$fields['parent2_entity_type']='class_id';
				$fields['parent3_entity_id']=$select1->p3;
				$fields['parent3_entity_type']='session_id';
				$fields['parent4_entity_id']=$select1->p4;
				$fields['parent4_entity_type']='instructor_id';

				expDebug::dPrint('deleting session_instructor33234-----> ' . print_r($sel1, true) , 3);

				$deleteStmt = db_delete('slt_session_instructor_details');
				$deleteStmt->condition('session_id', $form_state['values']['hid_session_id'],'=');
				$deleteStmt->condition('instructor_id', $instructorHId[0],'!=');
				expDebug::dPrintDBAPI(' Delete session instructor = ' ,$deleteStmt);
				$deleteStmt->execute();
				
				foreach($select1 as $val)
				{
				    $fields= array(
				            'table_name' => 'slt_session_instructor_details',
				            'entity_id' => $val->id,
				            'entity_type' => 'instructor_mapping',
				            'parent1_entity_id' => $val->p1,
				            'parent1_entity_type' => 'course_id',
				            'parent2_entity_id' => $val->p2,
				            'parent2_entity_type' => 'class_id',
				            'parent3_entity_id' => $val->p3,
				            'parent3_entity_type' => 'session_id',
				            'parent4_entity_id' => $val->p4,
				            'parent4_entity_type' => 'instructor_id',
				            'module_name' => 'exp_sp_administration_session_details',
				            'functionality_name' => 'updateSessionDetailsNew',
				            'api_name' => $_REQUEST['apiname'],
				            'deleted_on' => now(),
				            'deleted_by' => $loggedInUserId,
				    
				    );
				    deleted_log_entry($fields);
				}
	
				
			}
				$presenter_id = $instructorHId[0];

			
			expDebug::dPrint(' before update def_insDetails = ' . print_r($def_insDetails, true) , 5);
			
		}else{
			$delinsId = array_diff(explode(',',$default_ins),$instructorHId);
			$newinsId = array_diff($instructorHId, explode(',',$default_ins));
			$updinsId = array_intersect($instructorHId, explode(',',$default_ins));
			foreach ($delinsId as $inst){
			    
			    expDebug::dPrint('selecting session_instructor4-----> ');
			    
			    $select=db_select('slt_session_instructor_details','ins');
			    $select->addfield('ins','id','id');
			    $select->addField('ins','instructor_id','p4');
			    $select->addField('ins','session_id','p3');
			    $select->addField('ins','class_id','p2');
			    $select->addField('ins','course_id','p1');
			    $select->condition('session_id', $form_state['values']['hid_session_id'],'=');
			    $select->condition('instructor_id', $inst,'=');
			    $select1=$select->execute()->fetchAll();
			    
			    expDebug::dPrint('deleting session_instructor4-----> ' . print_r($slt1, true) , 3);
			    
				$deleteStmt = db_delete('slt_session_instructor_details');
				$deleteStmt->condition('session_id', $form_state['values']['hid_session_id'],'=');
				$deleteStmt->condition('instructor_id', $inst,'=');
				expDebug::dPrintDBAPI(' Delete session instructor = ' ,$deleteStmt);
				$deleteStmt->execute();
				
				foreach($select1 as $val)
				{
				    $fields= array(
				            'table_name' => 'slt_session_instructor_details',
				            'entity_id' => $val->id,
				            'entity_type' => 'instructor_mapping',
				            'parent1_entity_id' => $val->p1,
				            'parent1_entity_type' => 'course_id',
				            'parent2_entity_id' => $val->p2,
				            'parent2_entity_type' => 'class_id',
				            'parent3_entity_id' => $val->p3,
				            'parent3_entity_type' => 'session_id',
				            'parent4_entity_id' => $val->p4,
				            'parent4_entity_type' => 'instructor_id',
				            'module_name' => 'exp_sp_administration_session_details',
				            'functionality_name' => 'updateSessionDetailsNew',
				            'api_name' => $_REQUEST['apiname'],
				            'deleted_on' => now(),
				            'deleted_by' => $loggedInUserId,
				    
				    );
				    deleted_log_entry($fields);
				}
				   
			}
			expDebug::dPrint("instructor insert >>> ".print_r($instructorHId,true),5);
			foreach($newinsId as $ins){
				if( ($form_state['values']['session_presenter'] == $ins)){
					$presenter = 'Y';
					$presenter_id = empty($presenter_id) ? $ins : $presenter_id;
				}
				else{
					$presenter = 'N';
				}
				$Insfields = array(
						'course_id' => $form_state['values']['course_id'],
						'class_id' => $form_state['values']['class_id'],
						'session_id' => $form_state['values']['hid_session_id'],
						'instructor_id'=> $ins,
						'is_presenter' => $presenter,
						'created_by' => $loggedInUserId,
						'created_on' => now(),
						'updated_by' => $loggedInUserId,
						'updated_on' => now(),
				);
				$insertIns = db_insert('slt_session_instructor_details');
				$insertIns->fields($Insfields);
				expDebug::dPrintDBAPI(' Instructor session = ' ,$insertIns);
				$session_instructor = $insertIns->execute();
			}
			foreach ($updinsId as $instr){
				if( ($form_state['values']['session_presenter'] == $instr)){
					$presenter = 'Y';
					$presenter_id = empty($presenter_id) ? $instr : $presenter_id;
				}
				else{
					$presenter = 'N';
				}
				$updateStmt = db_update('slt_session_instructor_details');
				$updateStmt->fields(array('is_presenter' => $presenter));
				$updateStmt->condition('session_id', $form_state['values']['hid_session_id']);
				$updateStmt->condition('instructor_id', $instr);
		expDebug::dPrintDBAPI('updateSessionDetailsNew Velu = ', $updateStmt);
				$numUpdated = $updateStmt->execute();
			}

		}
		//Don't delete this update this is report flat table related update.
		$updComment  = db_update('slt_course_class');
		$updComment->fields(array(
				'updated_on' => now(),
		));
		$updComment->condition('id', $form_state['values']['class_id']);
		expDebug::dPrintDBAPI(' session delete update for class SQL = ' , $updComment);
		$updComment->execute();
		//Instructor Audit trail
		$default_inslist = array((object)array('instructor_id' => $default_ins));
		make_audit_trail_entries(array('instructor_id' => implode(',',$instructorHId)), $loggedInUserId,$default_inslist, $form_state['values']['hid_session_id'] ,'Instructor Updated','cre_sys_obt_cls','exp_sp_administration_session_details','UpdateSessionInstructor',
				array('instructor_id'));
		if(!empty($presenter_id)){
			$default_pre = array((object)array('presenter_id' => array_search('Y', $def_insDetails)));
			make_audit_trail_entries(array('presenter_id' => $presenter_id), $loggedInUserId,$default_pre,$form_state['values']['hid_session_id'] ,'Presenter Updated','cre_sys_obt_cls','exp_sp_administration_session_details','UpdateSessionPresenter',
					array('presenter_id'));
		}
		expDebug::dPrint(' $numUpdated = ' . print_r($numUpdated, true) , 3);
		make_audit_trail_entries($fields, $loggedInUserId, $default, $form_state['values']['hid_session_id'] ,'Updated','cre_sys_obt_cls','exp_sp_administration_session_details','updateSessionDetails',
		array('course_id', 'class_id', 'capacity_min', 'capacity_max', 'waitlist_count', 'location_id','facility_id', 'room_id', 'title', 'attendee_url', 'timezone', 'start_date', 'start_time', 'end_time'));
		if($form_state['values']['create_user_id']){
			return array((object)array('id'=>$form_state['values']['hid_session_id']));
		}

	}
	catch (Exception $ex) {
		$txn->rollback();
		watchdog_exception('updateSessionDetailsNew', $ex);
		if($form_state['values']['create_user_id']){
			//return array((object)array('id'=>'Failure'));
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Session Id is not valid";
			return $errobj;
		}
		throw $ex;
	}

	unset($txn);

}

function fetchSessionDetailsShort($classId,$sessionId){
	try {
	$select = db_select("slt_course_class_session","sess");
	$select->addField('sess','id','session_id');
	$select->addField('sess','start_date','start_date');
	$select->addField('sess','start_time','start_hours');
	$select->addField('sess','end_time','end_hours');
	$select->addField('sess','timezone','timezone');
	//$select->addField('sess','instructor_id','instructor_id');
	$select->addExpression("DATE_FORMAT((sess.start_date),'%Y-%m-%d')", 'ses_start_date');
	$select->leftjoin('slt_profile_list_items', 'splt_timezone', 'splt_timezone.code=sess.timezone');
	$select->addField('splt_timezone','attr2', 'session_timezone');
	$select->condition('sess.class_id',$classId,'=');
	if(!empty($sessionId)){
		$select->condition('sess.id',$sessionId,'<>');

	}
	expDebug::dPrintDBAPI('fetchSessionDetailsShort -->> ',$select);
	return $select->execute()->fetchAll();
	} catch (Exception $ex) {
		watchdog_exception('fetchSessionDetailsShort', $ex);
		expertusErrorThrow($ex);
	}
}

function checkIfMeetingEnabled($meetingType, $langCode = 'cre_sys_lng_eng') {
	try{
		$select = db_select('slt_profile_list_items','spli');
		$select->addField('spli','code', 'code');
		$select->condition('spli.code', $meetingType , '=');
		$select->condition('spli.lang_code', $langCode, '=');
		$select->condition('spli.is_active', 'Y', '=');
		$result = $select->countQuery()->execute()->fetchColumn();
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('checkIfMeetingEnabled', $ex);
		expertusErrorThrow($ex);
	}
}

function getInstructorInfo($sesId,$presenter ='',$from='') {
	try{
		$selectins = db_select('slt_session_instructor_details','ins');
		$selectins->leftjoin('slt_person', 'per', 'per.id=ins.instructor_id');
		$selectins->addField('per','id', 'opt');
		$selectins->condition('ins.session_id',$sesId,'=');
		if(!empty($from))
			$selectins->addField('per','full_name', 'full_name');
		if(!empty($presenter))
			$selectins->condition('ins.is_presenter','Y','=');
		expDebug::dPrintDBAPI('Instructor details -->>',$selectins);
		if(!empty($from))
			return $selectins->execute()->fetchAll();
		else
		return $selectins->execute()->fetchcol();
	 }catch (Exception $ex) {
		watchdog_exception('getInstructorDetails', $ex);
		expertusErrorThrow($ex);
	} 
} 

function getAllInstructor($from ='',$selectedId = ''){ 
	try {
		$searchtxt = $_GET['z'];
		$page		= $_GET['page'];
		$limit		= $_GET['rows'];
		$start		= $limit * $page - $limit;
		expDebug::dPrint("get value -->>> ".print_r($_GET,true),5);
		$select = db_select('slt_person', 'per');
		$select->distinct(true);
		if($from == 'ajaxcall')
			$select->addField('per','id', 'opt');
		
		$select->addField('per', 'full_name', 'val');
		$select->condition('per.is_instructor','Y');
		$select->condition('per.status', 'cre_usr_sts_atv');
		$select->addExpression('if(find_in_set(per.id, :select_id) > 0 ,1 ,0)', 'sel', array('select_id'=>$selectedId));
		$select->orderBy('sel','desc');
		$select->orderBy('full_name');
		$select->orderBy('last_name');
		if(!empty($searchtxt)){
			$db_or = db_or();
			$db_or	->condition('per.first_name', '%' . ($searchtxt) . '%', 'LIKE')
					->condition('per.last_name', '%' . ($searchtxt) . '%', 'LIKE')
					->condition('per.full_name', '%' . ($searchtxt) . '%', 'LIKE')
					->condition('per.user_name', '%' . ($searchtxt) . '%', 'LIKE');
			$select->condition($db_or);
		}
		filterByUserAccessPrivileges($select, 'per','',1,'cre_usr','','per');
		expDebug::dPrintDBAPI(' $select obj = ' , $select);
		if($from == 'session_api'){
			$select->addField('per','id', 'id');
			return $select->execute()->fetchAll();
		}	
		if($from == 'ajaxcall'){
			$count = $select->countQuery()->execute()->fetchField();
			
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
			$select->range($start,$limit);
			expDebug::dPrintDBAPI(' $select obj inside = ' , $select);
			 $instructor_list =  $select->execute()->fetchAll();
			 expDebug::dPrint("instructor list -->>> ". print_r($instructor_list,true),5);
			 $output = array();
			 $output['records'] = $count ;
			 $output['page'] = $page ;
			 $output['data'] = $instructor_list;
			 expDebug::dPrint('$output'.print_r($output, 1));
			 return $output;
		}
		else{
			return $select->execute()->fetchcol();
		}
	} catch (Exception $ex) {
		watchdog_exception('getInstructorAutoCompleteWithoutId', $ex);
		expertusErrorThrow($ex);
	}
}

function getInstructorForSession($sessionId,$insList){
	if($sessionId == "undefined" || empty($sessionId)) {
		$otherInstructorOptions = getAllInstructor('ajaxcall',null);
	}
	else {
		/* 
		$selList = getInstructorInfo($sessionId,'ajaxcall');
		$otherInstructorOptions = getAllInstructor('ajaxcall',$selList); */
		$selList = getInstructorInfo($sessionId);
		expDebug::dPrint("test selList " .print_r($selList,true),5);
		$otherInstructorOptions = getAllInstructor('ajaxcall',implode(',',$selList));
	}
	echo drupal_json_output($otherInstructorOptions);
}

function getInstructorForSessionautocomplete(){
	//$matchingUserNames = getAllInstructor('ajaxcall');
	//expDebug::dPrint("matching user name ".print_r($matchingUserNames,true),5);
	
	$searchtxt	= $_GET['z'];
	$limit		= $_GET['limit'];
	
	$select = db_select('slt_person', 'per');
	$select->addField('per','id', 'opt');
	$select->addField('per', 'full_name', 'val');
	$select->condition('per.is_instructor','Y');
	$select->condition('per.status','cre_usr_sts_atv'); //Added for #0072764
	$select->addExpression('if(find_in_set(per.id, :select_id) > 0 ,1 ,0)', 'sel', array('select_id'=>$selectedId));
	if(!empty($searchtxt)){
		$db_or = db_or();
		$db_or	->condition('per.first_name', '%' . ($searchtxt) . '%', 'LIKE')
		->condition('per.last_name', '%' . db_like($searchtxt) . '%', 'LIKE')
		->condition('per.full_name', '%' . db_like($searchtxt) . '%', 'LIKE')
		->condition('per.user_name', '%' . db_like($searchtxt) . '%', 'LIKE');
		$select->condition($db_or);
	}
	filterByUserAccessPrivileges($select, 'per','',1,'cre_usr','','per');
	if ($limit > 0) {
		$select->range(0, $limit);
	}
	expDebug::dPrint('$start and $end'.$start.' '.$end, 4);
	$select->orderBy('full_name');
	expDebug::dPrintDBAPI(' $select obj = ' , $select);
	$instructor_role = $select->execute()->fetchAll();
	foreach ($instructor_role as $userName) {
		print rawurlencode($userName->val) . "\n";
	}
}
function getPresenterAll($selectedId){
	$select = db_select('slt_person', 'per');
	$select->addField('per','id');
	$select->addField('per', 'full_name');
	$select->condition('per.is_instructor','Y');
	if(!empty($selectedId)){
		$selVal = explode(',',$selectedId);
		$select->condition('per.id',$selVal,'IN');
	}
	expDebug::dPrintDBAPI("get presenter ",$select);
	$presenter = $select->execute()->fetchAll();
	expDebug::dPrint("get presenter value ". $presenter);
	echo drupal_json_output($presenter);
}

function getPresenterWebex($userid){
	$select = db_select('slt_person', 'per');
	$select->addField('per','webex_name');
	$select->addField('per', 'webex_pass');
	$select->condition('per.id',$userid);
	$result=$select->execute()->fetchAll();
	return $result;
	}
?>