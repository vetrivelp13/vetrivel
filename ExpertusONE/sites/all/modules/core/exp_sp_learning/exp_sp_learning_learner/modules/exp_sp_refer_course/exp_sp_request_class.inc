<?php 

/**
 * Fetches all details required for rendering a request class dialog and echoes it to the requesting client in json format.
 */
function getDetailsForRequestClassDialog($courseId) {
	try {
    $select = db_select('slt_course_template', 'crs');
    $select->addField('crs', 'title', 'title');
    $select->addField('crs', 'code', 'code');
    $select->condition('crs.id', $courseId);
    expDebug::dPrintDBAPI('$select', $select);
    $courseDetail = $select->execute()->fetch();
    expDebug::dPrint('$courseDetail = ' . print_r($courseDetail, true));
    
    $res = array (
      'courseId' =>  $courseId,
      'title' => htmlspecialchars($courseDetail->title, ENT_QUOTES, 'UTF-8'),
      'code' => htmlspecialchars($courseDetail->code, ENT_QUOTES, 'UTF-8'),
      'labelmsg' => array(
        'dialog-title' => t('Request Class'), //Request Class
        'subject' => t('Course'), //Course
        'ccme' => t('LBL120'), //Send me a copy.
        'message' => t('LBL929'), //Message
        'optional' => strtolower(t('Optional')), // Label 'Optional' is present in po file    
        'close' => t('LBL123'), //Close
        'submit' => t('LBL124'), //Submit
      )
    );
    echo drupal_json_output($res);
	}
	catch (Exception $ex) {
		watchdog_exception('getDetailsForRequestClassDialog', $ex);
    expertusErrorThrow($ex);
	}
}

/**
 * Generates a notification to the admin for class request for a course. Also sends the same to user if requested to be cc'd. 
 */
function sendRequestForClass($courseId, $comment, $ccopy) {
	try {
		require_once(drupal_get_path('module', 'exp_sp_lnrsearch') .'/exp_sp_lnrsearch.inc');
	  $select = db_select('slt_course_template', 'crs');
	  $select->leftJoin('slt_profile_list_items', 'spli_lang', 'crs.lang_code = spli_lang.code');
	  $select->addField('crs', 'title', 'title');
	  $select->addField('crs', 'code', 'code');
	  $select->addField('crs', 'description', 'description');
	  $select->addField('spli_lang', 'name', 'language');
	  $select->condition('crs.id', $courseId);
	  expDebug::dPrintDBAPI('$select', $select);
	  $courseDetail = $select->execute()->fetch();
	  expDebug::dPrint('$courseDetail = ' . print_r($courseDetail, true));
	
	  $config           = getConfig('exp_sp');
	  $configSiteUrl    = $config['admin_site_url'];
	  $node_id = getNodeIdResults($courseId, 'cre_sys_obt_crs');
	  
	  $notiData = array(
	    'entityId' => $courseId,
	    'entityType' => 'cre_sys_obt_crs',
	    'title' => $courseDetail->title,
	    'code' => $courseDetail->code,
	    'description' => $courseDetail->description,
	    'language' => $courseDetail->language,
	    'comment' => $comment,
	    'ccopy' => $ccopy,
	    'viewlink' => '<a href="' . trim($configSiteUrl) . '/?q=learning/course-details/' . trim($node_id) . '">View Details</a>',
	    'sendToEmailId' => $config['reply_to'],
	  );
	
	  sendRequestClassNotification($notiData);
	  
    $res = array (
      'labelmsg' => array(
        'successmsg' => t('MSG699'), //Your request is submitted.
      )
    );
    echo drupal_json_output($res);
	}
	catch (Exception $ex) {
		watchdog('sendRequestForClass', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Checks notification override info and generates a request_class notification if isn't disabled.
 */
function sendRequestClassNotification($notiData) {
	try {
		$notificationCode = 'request_class';
		
	  $notificationTemplateInfo = getNotificationTemplateInfo($notificationCode, 'cre_sys_lng_eng');
	  $notificationTemplateInfo['notification_sendto'] = '';
	  expDebug::dPrint('$notificationTemplateInfo = ' . print_r($notificationTemplateInfo, true), 4);
	  
	  $notifyFlag = null;   	  
	  // Both 'Y' and 'cre_ntn_sts_atv' are active statuses
	  if ($notificationTemplateInfo['status'] == 'cre_ntn_sts_atv') {
	    $notifyFlag = 'Y';
	  }	
	  if ($notifyFlag != 'Y') {
	    expDebug::dPrint('Notification is disabled');
	    return;
	  }
	  
    $overrideContent = '';
    $overrideInfo = getEntityNotificationOverrideInfo($notiData['entityId'], $notiData['entityType'], $notificationCode, 'Email');
    expDebug::dPrint('$overrideInfo = ' . print_r($overrideInfo, true), 4);
    if (!empty($overrideInfo)) {
      $notifyOverrideFlag = $overrideInfo['notify_flag'];
      if ($notifyOverrideFlag == 'cre_ntn_sts_atv') {
      	$notifyOverrideFlag = 'Y';
      }
      if ($notifyOverrideFlag != 'Y') {
        expDebug::dPrint(' Notification is overridded and disabled');
        return;
      }
      
      $overrideContent = $overrideInfo['override_content'];
    }
	  
    $loggedInUserId = getSltpersonUserId();
    //$loggedInUserDetails = getLoggedInUserDetails(); // Cannot reliably use until issue #0032128 is fixed.
    $loggedInUserDetails = getPersonDetails($loggedInUserId, array('full_name', 'user_name', 'email'));

    $notificationInfo = array();
    $notificationInfo['tokens_string'] = 'course_title>|' . $notiData['title'] . '~|' .
                                         'course_code>|' . $notiData['code'] . '~|' .
                                         'description>|' . strip_tags($notiData['description']) . '~|' .
                                         'comment>|' . $notiData['comment'] . '~|' .
                                         'viewlink>|' . $notiData['viewlink'] . '~|' . 
                                         //'user_name>|' . $loggedInUserDetails['username'] . '~|' .
                                         //'user_email>|' . $loggedInUserDetails['usermail'] . '~|' .
                                         //'full_name>|' . $loggedInUserDetails['usersavedfullname'] . '~|';
                                         'user_name>|' . $loggedInUserDetails['user_name'] . '~|' .
                                         'user_email>|' . $loggedInUserDetails['email'] . '~|' .
                                         'full_name>|' . $loggedInUserDetails['full_name'] . '~|';
    
      $notificationInfo['message_id'] = $notificationCode;
      $notificationInfo['message_type'] = 'Request Class Notification';
      $notificationInfo['send_type'] = 'php mailer';
      $notificationInfo['lang_code'] = $notiData['language'];
      $notificationInfo['sendToEmail'] = $notiData['sendToEmailId'];
      $notificationInfo['ccopy'] = $notiData['ccopy'];
      $notificationInfo['sendival'] = 0;
      expDebug::dPrint('$notificationInfo = ' . print_r($notificationInfo, true), 4);
      callCommonNotificationInsert($notificationCode, $notificationInfo, $notificationTemplateInfo, $loggedInUserId, $overrideContent);
	}
	catch (Exception $ex) {
    watchdog('sendRequestClassNotification', $ex);
    expertusErrorThrow($ex);
	}
}

