<?php 

function notificationInsert($type, $objectId1, $objectId2, $enrollUserId, $userId, $iCal = null,&$statements=array()){
	try {
  include_once(drupal_get_path('module', 'exp_sp_learning') .'/exp_sp_learning.inc');
  expDebug::dPrint($type,4);  
  insertNotification($type, $objectId1, $objectId2, $enrollUserId, $userId, $iCal,$statements);
  } catch (Exception $ex) {
  	watchdog_exception('notificationInsert', $ex);
  	expertusErrorThrow($ex);
  }
}

// To send notification
function notificationInsert_($type, $objectId1, $objectId2, $enrollUserId, $userId){
  try {
  $conditionArray = array();

  if($type == 'Register'){
    
    // $objectId1 -> Course Id  - $objectId2 -> Class Id
    $notifyType = 'register';
    
    $conditionArray['mapp.entity_id'] = $objectId2;
    $conditionArray['mapp.entity_type'] = 'cre_sys_obt_cls';
    $conditionArray['mapp.notify_send_type'] = 'Email';
    $conditionArray['info.notification_code'] = $notifyType;
    
  } else if($type == 'Waitlist'){
    
    // $objectId1 -> Course Id  - $objectId2 -> Class Id
    $notifyType = 'waitlist';
    
    $conditionArray['mapp.entity_id'] = $objectId2;
    $conditionArray['mapp.entity_type'] = 'cre_sys_obt_cls';
    $conditionArray['mapp.notify_send_type'] = 'Email';
    $conditionArray['info.notification_code'] = $notifyType;
    
  } else if($type == 'WaitlistPromotion'){
    
    // $objectId1 -> Course Id  - $objectId2 -> Class Id
    $notifyType = 'waitlist_promo';
    
    $conditionArray['mapp.entity_id'] = $objectId2;
    $conditionArray['mapp.entity_type'] = 'cre_sys_obt_cls';
    $conditionArray['mapp.notify_send_type'] = 'Email';
    $conditionArray['info.notification_code'] = $notifyType;
    
  } else if($type == 'Cancel'){
    
    // $objectId1 -> Enroll Id  - $objectId2 -> Class Id
    $notifyType = 'class_cancel';
    
    $conditionArray['mapp.entity_id'] = $objectId2;
    $conditionArray['mapp.entity_type'] = 'cre_sys_obt_cls';
    $conditionArray['mapp.notify_send_type'] = 'Email';
    $conditionArray['info.notification_code'] = $notifyType;
    
  } else if($type == 'WBTCancel'){
    
    // $objectId1 -> Enroll Id  - $objectId2 -> Class Id
    $notifyType = 'class_cancel_wbt_vod';
    
    $conditionArray['mapp.entity_id'] = $objectId2;
    $conditionArray['mapp.entity_type'] = 'cre_sys_obt_cls';
    $conditionArray['mapp.notify_send_type'] = 'Email';
    $conditionArray['info.notification_code'] = $notifyType;
        
  } else if($type == 'WaitlistLocationAdmin'){
    
    // $objectId1 -> Enroll Id  - $objectId2 -> Class Id
    $notifyType = 'waitlist_location_admin';
    
    $conditionArray['mapp.entity_id'] = $objectId2;
    $conditionArray['mapp.entity_type'] = 'cre_sys_obt_cls';
    $conditionArray['mapp.notify_send_type'] = 'Email';
    $conditionArray['info.notification_code'] = $notifyType;
    
  } else if($type == 'CancelLocationAdmin'){
    
    // $objectId1 -> Enroll Id  - $objectId2 -> Class Id
    $notifyType = 'cancel_location_admin';
    
    $conditionArray['mapp.entity_id'] = $objectId2;
    $conditionArray['mapp.entity_type'] = 'cre_sys_obt_cls';
    $conditionArray['mapp.notify_send_type'] = 'Email';
    $conditionArray['info.notification_code'] = $notifyType;
    
  } else if($type == 'Completed'){
    
    // $objectId1 -> Enroll Id  - $objectId2 -> Class Id
    $notifyType = 'course_completed';
    $notifyFlag = 'Y';
    
  } else if($type == 'CertCurrLPComplete'){
    
    // $objectId1 -> Master Enroll Id  - $objectId2 -> Program Id
    $notifyType = 'cert_curr_lp_completed';
    $notifyFlag = 'Y';

  } 
  
  if(count($conditionArray) > 0){
    $mappingInfo = getNotificationMappingInfo($conditionArray);
    $notifyFlag = $mappingInfo['notify_flag'];
    $attachContent = $mappingInfo['attach_content'];
  }

  // Get the notification email info
  $selectNotiInfo = db_select('slt_notification_info', 'info');
  $selectNotiInfo->addField('info', 'status', 'notify_status');
  $selectNotiInfo->addField('info', 'notification_code', 'notify_name');
  $selectNotiInfo->addField('info', 'notification_sendto', 'email_send_to');
  $selectNotiInfo->addField('info', 'notification_cc', 'email_cc');
  $selectNotiInfo->addField('info', 'notification_admin', 'admin_email');
  $selectNotiInfo->condition('info.notification_code', $notifyType);
  $selectNotiInfo->condition('info.lang_code', 'cre_sys_lng_eng');
  $notiInfoResult = $selectNotiInfo->execute()->fetchAll();
  $notifyStatus   = $notiInfoResult[0]->notify_status;
  $notifyName     = $notiInfoResult[0]->notify_name;
  $emailSendTo    = $notiInfoResult[0]->email_send_to;
  $emailCC        = $notiInfoResult[0]->email_cc;
  $emailAdmin     = $notiInfoResult[0]->admin_email;

  $notifyStatus = $notifyStatus == 'cre_ntn_sts_atv'? 'Y' : $notifyStatus; 

  // Nofication required check for the types - WaitlistLocationAdmin / CancelLocationAdmin  
  if($type == 'WaitlistLocationAdmin' || $type == 'CancelLocationAdmin'){
    $clsSessSelect = db_select('slt_course_class_session', 'sess');
    $clsSessSelect->addField('sess', 'location_id');
    $clsSessSelect->condition('sess.class_id', $objectId2);
    $clsSessResult = $clsSessSelect->execute()->fetchAll();
    $locationId = $clsSessResult[0]->location_id;
    
    $deliveryTypeArray = getDeliveryTypeNoti($objectId2, 'All');
    $deliveryTypeCode = $deliveryTypeArray['code'];
    $isILTCount = $deliveryTypeCode != 'wbt' ? 1 : 0;
    
    if($isILTCount != 1 || empty($locationId)){
      $notifyStatus = 'N';
    }
    
  }
  
  $tokenString = array();
  
  // Check whether this type has notification status as Y   
  if($notifyStatus == 'Y' && ($notifyFlag == 'Y' || empty($notifyFlag) )){

    if($type == 'Register' || $type == 'Waitlist' || $type == 'WaitlistPromotion' || $type == 'WaitlistLocationAdmin'){
      
      // Waitist position
      $waitlistPosition = getWaitlistPositionNoti($objectId1, $objectId2, $enrollUserId);
      
      // Class title and delivery type name
      $deliveryTypeArray = getDeliveryTypeNoti($objectId2, 'All');
      $deliveryTypeCode = $deliveryTypeArray['code'];
      $deliveryTypeAttr = $deliveryTypeArray['attr_name'];
      $title = $deliveryTypeArray['title'];
      $deliveryTypeName = $deliveryTypeArray['name'];
      
      // Person details
      $personDetails = getPersonDetails($enrollUserId, '');
      
      // Token string array
      $tokenString[] = 'first_name>|'.$personDetails['first_name'].'~|';  
      $tokenString[] = 'last_name>|'.$personDetails['last_name'].'~|';
      $tokenString[] = 'delivery_type>|'.$deliveryTypeName.'~|';
      $tokenString[] = 'course_title>|'.$title.'~|';
      
      // Organization details
      $organizationId = $personDetails['org_id'];
      $organizationDetails = getOrganizationDetails($organizationId, '');

      // Country name
      $countryName = getCountryName($organizationDetails['country']);
      
      $tokenString[] = 'waitlist_position>|'.$waitlistPosition.'~|';
      
      if($deliveryTypeAttr == 'ilt' || $deliveryTypeAttr == 'vc'){
        
        // Session details
        $sessionDetails = getSessionDetailsNoti($objectId1, $objectId2);
        
        // Token string array
        $tokenStringSession[] = 'session_name<|'.$sessionDetails['title'];          
        $tokenStringSession[] = '~~session_startdate<|'.$sessionDetails['start_date'];
        $tokenStringSession[] = '~~session_enddate<|'.$sessionDetails['end_date'];
        $tokenStringSession[] = '~~session_starttime<|'.$sessionDetails['start_time'];
        $tokenStringSession[] = '~~session_endtime<|'.$sessionDetails['end_time'];
        $tokenStringSession[] = '~~session_location<|'.$sessionDetails['location_name'];
        $tokenStringSession[] = '~@';
        
        if($deliveryTypeAttr == 'ilt'){
          
          // Intructor info for ILT classes
          $instructorDetails = getInstructorDetails($objectId2);
          $tokenStringInstructor[] = 'instructor_first_name<|'.$instructorDetails['first_name'];
          $tokenStringInstructor[] = '~~instructor_last_name<|'.$instructorDetails['last_name'];
          $tokenStringInstructor[] = '~@';
          
          $tokenString[] = 'group_instructordata>|'.implode('', $tokenStringInstructor).'~|';
          $tokenString[] = 'group_sessiondata>|'.implode('', $tokenStringSession).'~|';
          $tokenString[] = 'session_location>|'.$sessionDetails['location_name'];
        }
        
        if($deliveryTypeAttr == 'vc'){
          //$tokenString[] = 'timezone>|'.$sessionDetails['timezone'];
        }
        $tokenString[] = '~|';
        
      } else {
        
        // To get content URL
        $contentDetails = getContentDetails($objectId1, $objectId2);
        $tokenString[] = 'launch_url>|'.($contentDetails['launch_url']).'~|';
      }
      
      $tokenString[] = 'dt_code>|'.$deliveryTypeAttr.'~|';
      
      // Form message id and message type to be inserted in DB
      if($type == 'Register'){
        
        if($deliveryTypeAttr == 'wbt'){
          
          $msgId = 'register_wbt';
          $msgType = 'Course Register Wbt';
          
        } else {
          
          $msgId = 'register';
          $msgType = 'Course Register';
          
        }
        
      } else if($type == 'Waitlist'){
        
        $msgId = 'waitlist';
        $msgType = 'Waitlist Confirmation';
          
      } else if($type == 'WaitlistPromotion'){
        
        // Enrolled Check
        $enrolledCount = enrollCheck($objectId2);
        
        // Maximum capacity
        $maxCapacity = $sessionDetails['capacity_max'];
        
        $seatCount = $maxCapacity - $enrolledCount;  
        $tokenString[] = '~|';
        $tokenString[] = 'seat_count>|'.$seatCount;
        
        $msgId = 'waitlist_promo';
        $msgType = 'Automatic Waitlist Promotion';
        
      } else if($type == 'WaitlistAllocated'){
        
        $msgId = 'seat_occupied_for_waitlist';
        $msgType = 'Seat Occupied for Waitlisted learners';
          
      } else if($type == 'WaitlistLocationAdmin'){
        
        $tokenString[] = '~|';
        $tokenString[] = 'org_name>|'.$organizationDetails['name'].'~|';
        $tokenString[] = 'org_add1>|'.$organizationDetails['add1'].'~|';
        $tokenString[] = 'org_add2>|'.$organizationDetails['add2'].'~|';
        $tokenString[] = 'org_city>|'.$organizationDetails['city'].'~|';
        $tokenString[] = 'org_state>|'.$organizationDetails['state'].'~|';
        $tokenString[] = 'org_zip>|'.$organizationDetails['zip'].'~|';
        $tokenString[] = 'org_country>|'.$organizationDetails['country'].'~|';
        $tokenString[] = 'org_phone>|'.$organizationDetails['phone_no'].'~|';
        $tokenString[] = 'user_email>|'.$personDetails['email'].'~|';
        $tokenString[] = 'org_fax>|'.$organizationDetails['fax_no'].'~|';
        
        $msgId = 'waitlist_location_admin';
        $msgType = 'Waitlist Location Admin';
        
      }
      
      $mailSendingType = 'php mailer';
      
    }
    
    if($type == 'CertCurrLPComplete'){
      $msgId = 'cert_curr_lp_completed';
      $msgType = 'Cert/Curr/LP Completed';
      
      // Get master enrollment details
      $masterEnrollmentDetails = masterEnrollmentDetails($objectId1);
      
      $tokenString[] = 'full_name>|'.$masterEnrollmentDetails['full_name'].'~|';
      $tokenString[] = 'delivery_type>|'.$masterEnrollmentDetails['deliverytype_name'].'~|';
      $tokenString[] = 'course_title>|'.$masterEnrollmentDetails['course_title'];

      $mailSendingType = 'php mailer';
    }
    
    if($type == 'Cancel' || $type == 'WBTCancel' || $type == 'CancelLocationAdmin'){
      
      // $objectId1 -> Enroll User Id
      $classDetails = classDetailsNoti($objectId1);
      
      $classId = $classDetails['id'];
      $classTitle = $classDetails['title'];
      //$enrollUserId = $classDetails['user_id'];
      $courseId = $classDetails['course_id'];
      $orderId = $classDetails['order_id'];
      
      $deliveryTypeCode = $classDetails['delivery_type'];
      $deliveryTypeName = getProfileListName($deliveryTypeCode, '');
      
      $deliveryTypeArray = getDeliveryTypeNoti($classId, 'All');
      $deliveryTypeAttr = $deliveryTypeArray['attr_name']; 
      
      $sessionDetails = getSessionDetailsNoti($courseId, $classId);
      
      $locationName = $sessionDetails['location_name'];
      $startDateFormatted = $sessionDetails['start_date_formatted'];
      $endDateFormatted = $sessionDetails['end_date_formatted'];
      
      if($deliveryTypeAttr == 'ilt'){
        $instructorDetails = getInstructorDetails($classId);

        // Intructor info for ILT classes
        $instructorDetails = getInstructorDetails($classId);
        $tokenStringInstructor[] = 'instructor_first_name<|'.$instructorDetails['first_name'];
        $tokenStringInstructor[] = '~~instructor_last_name<|'.$instructorDetails['last_name'];
        $tokenStringInstructor[] = '~@';
        
        $tokenString[] = 'group_instructordata>|'.implode('', $tokenStringInstructor).'~|';
      }
      
      // Order details
      $orderDetails = getOrderDetailsNoti($courseId, $classId, $enrollUserId, $orderId);
      
      // Person details
      $personDetails = getPersonDetails($enrollUserId, '');
      
      // Manager details
      $managerDetails = getPersonDetails($userId, '');
      $managerName = $managerDetails['full_name'];

      // Organization details
      $organizationId = $personDetails['org_id'];
      $organizationDetails = getOrganizationDetails($organizationId, '');

      // Country name
      $countryName = getCountryName($organizationDetails['country']);

      $tokenString[] = 'delivery_type>|'.$deliveryTypeName.'~|';
      $tokenString[] = 'course_title>|'.$classTitle.'~|';
      
      $tokenString[] = 'dt_code>|'.$deliveryTypeAttr.'~|';
      $tokenString[] = 'session_startdate>|'.$sessionDetails['start_date'].'~|';
      $tokenString[] = 'session_enddate>|'.$sessionDetails['end_date'].'~|';
      $tokenString[] = 'session_location>|'.$sessionDetails['location_name'].'~|';
      
      $tokenString[] = 'first_name>|'.$personDetails['first_name'].'~|';  
      $tokenString[] = 'last_name>|'.$personDetails['last_name'].'~|';
      
      $tokenString[] = 'course_price>|'.$orderDetails['item_price'].'~|';
      $tokenString[] = 'refund_amount>|'.$orderDetails['refund_amount'].'~|';

      // Organization details
      $organizationId = $personDetails['org_id'];
      $organizationDetails = getOrganizationDetails($organizationId, '');
      
      if($type == 'Cancel'){
        
        $msgId = 'class_cancel';
        $msgType = 'Class Cancel By Admin';
        
      } else if($type == 'WBTCancel'){
        
        $msgId = 'class_cancel_wbt_vod';
        $msgType = 'WBT/VOD Cancel by Admin';
      
      } else if($type == 'CancelLocationAdmin'){
      
        $tokenString[] = '~|';
        $tokenString[] = 'org_name>|'.$organizationDetails['name'].'~|';
        $tokenString[] = 'org_add1>|'.$organizationDetails['add1'].'~|';
        $tokenString[] = 'org_add2>|'.$organizationDetails['add2'].'~|';
        $tokenString[] = 'org_city>|'.$organizationDetails['city'].'~|';
        $tokenString[] = 'org_state>|'.$organizationDetails['state'].'~|';
        $tokenString[] = 'org_zip>|'.$organizationDetails['zip'].'~|';
        $tokenString[] = 'org_country>|'.$organizationDetails['country'].'~|';
        $tokenString[] = 'org_phone>|'.$organizationDetails['phone_no'].'~|';
        $tokenString[] = 'user_email>|'.$personDetails['email'].'~|';
        $tokenString[] = 'org_fax>|'.$organizationDetails['fax_no'].'~|';
        
        $msgId = 'cancel_location_admin';
        $msgType = 'Cancel Location Admin';
        
      }
        			
      
      $mailSendingType = 'php mailer';
      
    }
    
    if($type == 'Completed'){
      
      $msgId = 'course_completed';
      $msgType = 'Course Completed';

      $enrollSelect = db_select('slt_enrollment', 'enroll');
      $enrollSelect->innerjoin('slt_course_class', 'cls', 'enroll.class_id = cls.id');
      $enrollSelect->addField('cls', 'title');
      $enrollSelect->addField('cls', 'course_id');
      $enrollSelect->addField('enroll', 'comp_status');
      $enrollSelect->addExpression("DATE_FORMAT(enroll.comp_date,'%m/%d/%Y')", 'completion_date');
      $enrollSelect->condition('enroll.user_id', $enrollUserId);
      $enrollSelect->condition('enroll.id', $objectId1);
      $enrollSelect->condition('enroll.class_id', $objectId2);
      $enrollSelect->condition('enroll.reg_status', 'lrn_crs_reg_cnf');
      
      $enrollDetails = $enrollSelect->execute()->fetchAssoc(); 
      
      // Person details
      $personDetails = getPersonDetails($enrollUserId, '');
      
      $tokenString = array();
      $tokenString[] = 'first_name>|'.$personDetails['first_name'].'~|';
      $tokenString[] = 'last_name>|'.$personDetails['last_name'].'~|';
      $tokenString[] = 'user_email>|'.$personDetails['email'].'~|';
      $tokenString[] = 'course_title>|'.$enrollDetails['title'].'~|';
      $tokenString[] = 'course_completed_status>|'.$enrollDetails['comp_status'].'~|';
      $tokenString[] = 'course_completed_date>|'.$enrollDetails['completion_date'].'~|';
      
      $mailSendingType = 'php mailer';      
        
    }
  	
    // Drupal User Details
    $drupalUserDetails = drupalUserDetails($enrollUserId);
    $drupalUserId = $drupalUserDetails['drupal_user_id'];
    
    // Person details
    $personDetails = getPersonDetails($enrollUserId, '');
    $username = $personDetails['user_name'];
    $firstname = $personDetails['first_name'];
    $tokenString[] = '~|';
    $tokenString[] = 'user_name>|'.$username.'~|';
    $tokenString[] = 'first_name>|'.$firstname.'~|';
    $tokenString[] = 'last_name>|'.$personDetails['last_name'].'~|';
    $tokenString[] = 'user_email>|'.$personDetails['email'].'~|';
    $tokenString[] = 'user_phone>|'.$personDetails['phone_no'].'~|';
    $tokenString[] = 'learning_fullname>|'.$personDetails['full_name'].'~|';
    $tokenString[] = 'full_name>|'.$personDetails['full_name'];    
    
    $tokenStringValue = implode('', $tokenString);
    
    $remark = '';
    
    (!empty($emailCC)) ? '$emailSendToCc = $emailCC' :'$emailSendToCc = ""';
    if($emailSendTo == 'Admin'){
      $emailToBeSend = $emailAdmin;
      //CALL slp_notification_ins_script(xmsgid,xmsgtype,xtokenstr,xuserid,xdruid,xusername,xsendtoemail,xsendtype,xattach_content,xremark,xupdateuser);
      notificationDBInsert($msgId, $msgType, $tokenStringValue, $enrollUserId, $drupalUserId, $username, $emailToBeSend, $mailSendingType, $attachContent, $remark, $userId, $emailSendToCc);
    }

    if($emailSendTo == 'User'){
      $emailToBeSend = $personDetails['email'];
      
      notificationDBInsert($msgId, $msgType, $tokenStringValue, $enrollUserId, $drupalUserId, $username, $emailToBeSend, $mailSendingType, $attachContent, $remark, $userId, $emailSendToCc);
      
    }
	
    $managerId = $personDetails['manager_id'];
    if($emailSendTo == 'Manager' && !empty($managerId)){

      // Manager details
      $managerDetails = getPersonDetails($managerId, '');
      $managerEmail = $managerDetails['email'];
      
      $emailToBeSend = $managerEmail;
      
      notificationDBInsert($msgId, $msgType, $tokenStringValue, $enrollUserId, $drupalUserId, $username, $emailToBeSend, $mailSendingType, $attachContent, $remark, $userId, $emailSendToCc);
    }
    
    if($type == 'WaitlistLocationAdmin' || $type == 'CancelLocationAdmin'){
      $locationDetails = facilityDetailsNoti($locationId);
      $emailToBeSend = $locationDetails['contact_email'];
      if(!empty($emailToBeSend)){
        notificationDBInsert($msgId, $msgType, $tokenStringValue, $enrollUserId, $drupalUserId, $username, $emailToBeSend, $mailSendingType, $attachContent, $remark, $userId, $emailSendToCc);
      }
    }
	
//     if(!empty($emailCC)){
//       $emailToBeSend = $emailCC; 
//       notificationDBInsert($msgId, $msgType, $tokenStringValue, $enrollUserId, $drupalUserId, $username, $emailToBeSend, $mailSendingType, $attachContent, $remark, $userId);
//     }
			
  } 
  } catch (Exception $ex) {
  	watchdog_exception('notificationInsert_', $ex);
  	expertusErrorThrow($ex);
  } 
}

// Facility details
function facilityDetailsNoti($locationId){
	try {
  $select = db_select('slt_facility', 'fac');
  $select->addField('fac', 'contact_email');
  $select->condition('fac.location_id', $locationId);
  return $select->execute()->fetchAssoc();
  } catch (Exception $ex) {
  	watchdog_exception('facilityDetailsNoti', $ex);
  	expertusErrorThrow($ex);
  }
}

// Drupal user details
function drupalUserDetails($enrollUserId){
  try {
  // Person details from drupal users table and person table
  $drupalUserSelect = db_select('slt_person', 'prsn');
  $drupalUserSelect->innerjoin('users', 'usr', 'prsn.user_name = usr.name');
  $drupalUserSelect->addField('usr', 'uid', 'drupal_user_id');
  $drupalUserSelect->addField('usr', 'mail', 'drupal_email_id');
  $drupalUserSelect->condition('prsn.id', $enrollUserId);
  
  return $drupalUserSelect->execute()->fetchAssoc();
  } catch (Exception $ex) {
  	watchdog_exception('drupalUserDetails', $ex);
  	expertusErrorThrow($ex);
  }
  
}

// Order details
function getOrderDetailsNoti($courseId, $classId, $enrollUserId, $orderId){
  try {
  $select = db_select('slt_order_items', 'order_item');
  $select->addField('order_item', 'item_price');
  $select->addField('order_item', 'refund_amount');
  $select->condition('order_item.course_id', $courseId);
  $select->condition('order_item.class_id', $classId);
  $select->condition('order_item.user_id', $enrollUserId);
  $select->condition('order_item.order_id', $orderId);
  $select->range(0,1);
  
  return $select->execute()->fetchAssoc();
  } catch (Exception $ex) {
  	watchdog_exception('getOrderDetailsNoti', $ex);
  	expertusErrorThrow($ex);
  }
  
}

// Class details
function classDetailsNoti($enrollUserId){
  try {
  $select = db_select('slt_course_class', 'cls');
  $select->innerjoin('slt_enrollment', 'enr', 'cls.id = enr.class_id');
  $select->addField('cls', 'id');
  $select->addField('cls', 'title');
  $select->addField('cls', 'delivery_type');
  $select->addField('enr', 'user_id');
  $select->addField('enr', 'order_id');
  $select->addField('cls', 'course_id');
  $select->condition('enr.id', $enrollUserId);
  
  return $select->execute()->fetchAssoc();
  } catch (Exception $ex) {
  	watchdog_exception('classDetailsNoti', $ex);
  	expertusErrorThrow($ex);
  }
}


// Master enrollment details
function masterEnrollmentDetails($masterId){
  try {
  $select = db_select('slt_master_enrollment', 'me');
  $select->innerjoin('slt_person', 'prs', 'prs.id = me.user_id');
  $select->innerjoin('slt_program', 'prg', 'prg.id = me.program_id');
  $select->innerjoin('slt_profile_list_items', 'prof', 'prof.code = prg.object_type');
  $select->addField('prs', 'full_name', 'full_name');
  $select->addField('prg', 'title', 'course_title');
  $select->addField('prof', 'name', 'deliverytype_name');
  $select->condition('me.id', $masterId);
  return $select->execute()->fetchAssoc();
  } catch (Exception $ex) {
  	watchdog_exception('masterEnrollmentDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

// Enrollment check
function enrollCheck($classId){
	try {
  $select = db_select('slt_enrollment', 'enroll');
  $select->addField('enroll', 'id');
  $select->condition('enroll.class_id', $classId);
  $result = $select->execute()->fetchAssoc();
  return count($result);
  } catch (Exception $ex) {
  	watchdog_exception('enrollCheck', $ex);
  	expertusErrorThrow($ex);
  }
}

// To get content details
function getContentDetails($objectId1, $objectId2){
	try {
  $select = db_select('slt_course_content_mapper', 'map');
  $select->addField('map', 'launch_url');
  $select->condition('map.course_id', $objectId1);
  $select->condition('map.class_id', $objectId2);
  return $select->execute()->fetchAssoc();
  } catch (Exception $ex) {
  	watchdog_exception('getContentDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

// To get the instructor details for ILT classes
function getInstructorDetails($classId){
  try {
  $select = db_select('slt_object_role_mapping', 'rm');
  $select->leftjoin('slt_person', 'pers', 'pers.id=rm.user_id');
  $select->addField('pers', 'first_name');
  $select->addField('pers', 'last_name');
  $select->condition('rm.object_id', $classId);
  $select->condition('rm.object_type', 'cre_sys_obt_cls');
  //$select->condition('rm.role_code', 'cre_pgn_rls_ins');
  return $select->execute()->fetchAssoc();
  } catch (Exception $ex) {
  	watchdog_exception('getInstructorDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

// To get session details
function getSessionDetailsNoti($objectId1, $objectId2){
  try { 
  $select = db_select('slt_course_class_session', 'cls');
  $select->leftjoin('slt_location', 'sl', 'cls.location_id = sl.id');
  $select->addField('cls', 'title');
  $select->addField('cls', 'start_date');
  $select->addExpression("IFNULL(DATE_FORMAT(cls.start_date,'%m/%d/%Y'),' ')", 'start_date');
  $select->addExpression("IFNULL(DATE_FORMAT(cls.end_date,'%m/%d/%Y'),' ')", 'end_date');
  $select->addExpression("IFNULL(TIME_FORMAT(cls.start_time,'%h:%i %p'), ' ')", 'start_time');
  $select->addExpression("IFNULL(TIME_FORMAT(cls.end_time,'%h:%i %p'), ' ')", 'end_time');
  $select->addField('cls', 'timezone');
  $select->addField('cls', 'capacity_max');
  $select->addField('sl', 'name', 'location_name');
  $select->addField('cls', 'start_date', 'start_date_formatted');
  $select->addField('cls', 'end_date', 'end_date_formatted');
  $select->condition('cls.course_id', $objectId1);
  $select->condition('cls.class_id', $objectId2);
  $select->range(0, 1);
  return $select->execute()->fetchAssoc();
  } catch (Exception $ex) {
  	watchdog_exception('getSessionDetailsNoti', $ex);
  	expertusErrorThrow($ex);
  }
}

// To get waitlist position 
function getWaitlistPositionNoti($courseId, $classId, $enrollUserId){
	try {
  $select = db_select('slt_enrollment', 'enroll');
  $select->addField('enroll', 'waitlist_priority');
  $select->condition('enroll.user_id', $enrollUserId);
  $select->condition('enroll.course_id', $courseId);
  $select->condition('enroll.class_id', $classId);
  $select->range(0, 1);
  $result = $select->execute()->fetchAssoc();
  return $result['waitlist_priority'];
  } catch (Exception $ex) {
  	watchdog_exception('getWaitlistPositionNoti', $ex);
  	expertusErrorThrow($ex);
  }
}

// To get profile list values
function getProfileListName($code, $languageCode){
	try {
  $languageCode = empty($languageCode) ? 'cre_sys_lng_eng' : $languageCode;
  
  $select = db_select('slt_profile_list_items', 'pli');
  $select->addField('pli', 'name');
  $select->condition('pli.code', $code);
  $select->condition('pli.lang_code', $languageCode);
  
  $result = $select->execute()->fetchAssoc();
  return $result['name'];
  } catch (Exception $ex) {
  	watchdog_exception('getProfileListName', $ex);
  	expertusErrorThrow($ex);
  }
} 

// To get delivery type 
function getDeliveryTypeNoti($classId, $returnType){
	try {
  $select = db_select('slt_course_class', 'cls');
  $select->innerjoin('slt_profile_list_items', 'pli', 'pli.code = cls.delivery_type');
  $select->addField('pli', 'name');
  $select->addField('pli', 'code');
  $select->addField('cls', 'title');
  $select->addExpression('LOWER(pli.attr1)', 'attr_name');
  $select->condition('cls.id', $classId);
  $result = $select->execute()->fetchAssoc();
  
  if($returnType == 'Long'){
    return $result['name'];
  } else if($returnType == 'Short'){
    return $result['attr_name'];
  } else if($returnType == 'All'){
    return array(	'name' => $result['name'], 
                    'code' => $result['code'],
                    'attr_name' => $result['attr_name'], 
                    'title' => $result['title'] );
  }
  } catch (Exception $ex) {
  	watchdog_exception('getDeliveryTypeNoti', $ex);
  	expertusErrorThrow($ex);
  }
}

// To get class id from entity id
function getClassId($entityId){
	try {
  $select = db_select('slt_enrollment', 'enroll');
  $select->addField('enroll', 'class_id');
  $select->condition('enroll.id', $entityId);
  $result = $select->execute()->fetchAssoc();
  return $result['class_id'];
  } catch (Exception $ex) {
  	watchdog_exception('getClassId', $ex);
  	expertusErrorThrow($ex);
  }
}

// To get the notification mapping details 
function getNotificationMappingInfo($conditionArray){
	try {
  $select = db_select('slt_entity_notification_mapping', 'mapp');
  $select->innerjoin('slt_notification_info', 'info', 'mapp.notify_id = info.id');
  $select->addField('mapp', 'status', 'notify_flag');
  $select->addField('mapp', 'notify_text', 'attach_content');
  foreach($conditionArray as $fieldName => $fieldValue){
    $select->condition($fieldName, $fieldValue);
  }
  return $select->execute()->fetchAssoc();
  } catch (Exception $ex) {
  	watchdog_exception('getNotificationMappingInfo', $ex);
  	expertusErrorThrow($ex);
  }
}


function notificationDBInsert($msgId, $msgType, $tokenStringValue, $enrollUserId, $drupalUserId, $username, $emailToBeSend, $mailSendingType, $attachContent, $remark, $userId, $emailSendToCc){
  try {
  $insert = db_insert('slt_notification');
  
  $fieldsArray['msg_id'] =  $msgId;
  $fieldsArray['lang_code'] =  'cre_sys_lng_eng';
  $fieldsArray['msg_type'] =  $msgType;
  $fieldsArray['token_str'] =  $tokenStringValue;
  $fieldsArray['user_id'] =  $enrollUserId;
  $fieldsArray['send_to_id'] =  $drupalUserId;
  $fieldsArray['send_to_name'] =  $username;
  $fieldsArray['send_to_email'] =  $emailToBeSend;
  $fieldsArray['send_to_email_cc'] = $emailSendToCc;
  $fieldsArray['send_type'] =  $mailSendingType;
  $fieldsArray['send_status'] =  'N';
  $fieldsArray['attach_content'] =  $attachContent;
  $fieldsArray['remarks'] =  $remark;
  $fieldsArray['created_by'] =  $userId;
  $fieldsArray['created_on'] =  now();
  $fieldsArray['updated_by'] =  $userId;
  $fieldsArray['updated_on'] =  now();
  
  $insert->fields($fieldsArray);
  $insert->execute();
  } catch (Exception $ex) {
  	watchdog_exception('notificationDBInsert', $ex);
  	expertusErrorThrow($ex);
  }
}
