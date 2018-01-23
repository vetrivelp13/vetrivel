<?php

function getLoggedUser($userid) {
	try{
  $select = db_select('users', 'user');
  $select->addField('user','mail','email');
  $select->condition('uid', $userid);
  return $select->execute()->fetchField();
  } catch (Exception $ex) {
  	watchdog_exception('getLoggedUser', $ex);
  	expertusErrorThrow($ex);
  }
}

function getClassDetails($entityId) {
	try{

  $UserDefaultCurrencyArr = getUserDefaultCurrency();
  $UserPrefCurrCode = $UserDefaultCurrencyArr['currency_code'];
  $UserPrefCurrSym = $UserDefaultCurrencyArr['currency_sym'];

  $select = db_select('slt_course_class', 'class');
  $select->leftJoin('slt_course_class_session','session','class.id = %alias.class_id');
 // $select->leftJoin('slt_session_instructor_details','ses_ins','session.id = ses_ins.session_id'); 

  $select->addField('class','title','title');
  $select->addField('class','code','code');
  $select->addField('class','short_description','description');
  $select->addField('class','lang_code','lang_code');
  $select->addField('class','delivery_type','delivery_type');
  $select->addField('class','valid_from','valid_from');
  $select->addField('class','valid_to','valid_to');
  //$select->addField('class','price','price');
  $select->addExpression('slf_convert_price(class.price,class.currency_type,\''.$UserPrefCurrCode.'\')','price');
  $select->addExpression('\''.$UserPrefCurrSym.'\'','currency_symbol');
  $select->addExpression('\''.$UserPrefCurrCode.'\'','currency_code');
  $select->addField('class','currency_type','currency_type');

  $select->addField('session','id','session_id');
  $select->addField('session','start_time','start_time');
  $select->addField('session','end_time','end_time');
  $select->addField('session','location_id','location_id');
  $select->addField('session','room_id','room_id');
  //$select->addField('session','instructor_id','instructor_id');
  //$select->addExpression('GROUP_CONCAT(DISTINCT(ses_ins.instructor_id))', 'instructor_id');
  $select->addField('session','capacity_min','capacity_min');
  $select->addField('session','capacity_max','capacity_max');
  $select->addField('session','waitlist_count','waitlist_count');
  $select->condition('class.id', $entityId);
  $select->range(0,1);
  expDebug::dPrintDBAPI('REFER GET CLASS DETAIL QRY-->',$select);
  $result1 = $select->execute()->fetchAll();

  $select1 = db_select('slt_course_class_session', 'session');
  $select1->addExpression('MIN(start_date)','start_date');
  $select1->addExpression('MAX(start_date)','end_date');
  $select1->condition('session.class_id', $entityId);

  $result2 = $select1->execute()->fetchAll();
  $result1[0]->start_date = $result2[0]->start_date;
  $result1[0]->end_date = $result2[0]->end_date;

  return $result1;
  } catch (Exception $ex) {
  	watchdog_exception('getClassDetails', $ex);
  	expertusErrorThrow($ex);
  }
}
function getCourseDetails($entityId) {
	try{
	$select = db_select('slt_course_template', 'class');

	$select->addField('class','title','title');
	$select->addField('class','code','code');
	$select->addField('class','short_description','description');
	$select->addField('class','lang_code','lang_code');

	$select->condition('class.id', $entityId);
	$select->range(0,1);
	$result1 = $select->execute()->fetchAll();
	return $result1;
	} catch (Exception $ex) {
		watchdog_exception('getCourseDetails', $ex);
		expertusErrorThrow($ex);
	}
}
function getCurrencyFromClassId($classid){
	try {
  $select = db_select('slt_profile_list_items', 'prof');
  $select->leftJoin('slt_course_template','ctemp','prof.attr1 = %alias.currency_type');
  $select->leftJoin('slt_course_class_session','csession','ctemp.id = %alias.course_id');
  $select->condition('csession.class_id', $classid);
  $select->addField('prof','attr2','currency');
  $currency = $select->execute()->fetchField();
  if(empty($currency)){
    return '';
  }else {
    return $currency;
  }
  } catch (Exception $ex) {
  	watchdog_exception('getCurrencyFromClassId', $ex);
  	expertusErrorThrow($ex);
  }
}

function getEntityFromCode($entityId) {
	try{
  $select = db_select('slt_profile_list_items', 'entity');
  $select->addField('entity','name','name');
  $select->condition('entity.code', $entityId);
  return $select->execute()->fetchField();
  } catch (Exception $ex) {
  	watchdog_exception('getEntityFromCode', $ex);
  	expertusErrorThrow($ex);
  }
}

function getAttr2FromCode($entityId) {
	try{
  $select = db_select('slt_profile_list_items', 'entity');
  $select->addField('entity','attr2','currency');
  $select->condition('entity.code', $entityId);
  return $select->execute()->fetchField();
  } catch (Exception $ex) {
  	watchdog_exception('getAttr2FromCode', $ex);
  	expertusErrorThrow($ex);
  }
}

function getLocationFromId($locId) {
	try{
  $select = db_select('slt_location', 'loc');
  $select->addField('loc','name','name');
  $select->condition('loc.id', $locId);
  return $select->execute()->fetchField();
  } catch (Exception $ex) {
  	watchdog_exception('getLocationFromId', $ex);
  	expertusErrorThrow($ex);
  }
}

function getInstructorFromId($insId) {
	try{
		$insId_arr = explode(',',$insId);
	  $select = db_select('slt_person', 'ins');
	  $select->addExpression('GROUP_CONCAT(DISTINCT(ins.full_name))', 'name');
	  //$select->addField('ins','full_name','name');
	  $select->condition('ins.id', $insId_arr,'IN');
	  expDebug::dPrintDBAPI('REFER GET CLASS DETAIL QRY-->',$select);
	  return $select->execute()->fetchField();
  } catch (Exception $ex) {
  	watchdog_exception('getInstructorFromId', $ex);
  	expertusErrorThrow($ex);
  }
}

function getCourseClassDetail($entityId,$entityType) {
	try {
     global $user;
     if($user->uid){
       $useremail = getLoggedUser($user->uid);
     }else{
       $useremail = "";
       echo 'access_denied';
       exit();
     }
     if ($entityType == 'cre_sys_cat_shr') {

      $res = array(
        'title'         =>  t("LBL1274"),
        'email'         =>  $useremail,
        'entityId'      =>  $entityId,
        'entityType'    =>  $entityType
      );
      expDebug::dPrint('values generated for multiple share courses'.print_r($res,true));
     } else {
      if($entityType == 'cre_sys_obt_cls') {
       $classDetail = getClassDetails($entityId);
       $classDetail = $classDetail[0];

       $res = array(
         'title'         =>  $classDetail->title,
         'email'         =>  $useremail,
         'code'          =>  $classDetail->code,
         'description'   =>  $classDetail->description,
         'language'      =>  getEntityFromCode($classDetail->lang_code),
         'deliverytype'  =>  getEntityFromCode($classDetail->delivery_type),
         'deliverytypecode'  =>  $classDetail->delivery_type,
         'valid_from'    =>  date("M d, Y  h:i A", $classDetail->valid_from),
         'valid_to'      =>  date("M d, Y  h:i A", $classDetail->valid_to),
         'price'         =>  $classDetail->price,
         'duration'      =>  date("M d, Y  h:i A", $classDetail->start_date)."  -  ".date("M d, Y  h:i A", $classDetail->end_date),
         'location'      =>  getLocationFromId($classDetail->location_id),
         'room_id'       =>  $classDetail->room_id,
        // 'instructor'    =>  getInstructorFromId($classDetail->instructor_id),
         'capacity'      =>  $classDetail->capacity_min."-".$classDetail->capacity_max,
         'waitlist_count'=>  $classDetail->waitlist_count,
         'session_id'    =>  $classDetail->session_id,
         'entityId'      =>  $entityId,
         'entityType'    =>  $entityType
       );
      }
      else if($entityType == 'cre_sys_obt_crs') {
       $classDetail = getCourseDetails($entityId);
       $classDetail = $classDetail[0];
       expDebug::dPrint('values to b e printed for share'.print_r($classDetail,true));
       $res = array(
         'title'         =>  $classDetail->title,
         'email'         =>  $useremail,
         'code'          =>  $classDetail->code,
         'description'   =>  $classDetail->description,
         'language'      =>  getEntityFromCode($classDetail->lang_code),
         'deliverytype'  =>  getEntityFromCode('lrn_crs'),
         'deliverytypecode'  =>  'lrn_crs',
         'price'         =>  '',
         'entityId'      =>  $entityId,
         'entityType'    =>  $entityType
       );

      }
      else if($entityType == 'cre_sys_obt_crt' || $entityType == 'cre_sys_obt_trn' || $entityType == 'cre_sys_obt_cur') {
       $lpDetail = getLPDetails($entityId);
       $lpDetail = $lpDetail[0];

       $res = array(
         'title'         =>  $lpDetail->title,
         'email'         =>  $useremail,
         'code'          =>  $lpDetail->code,
         'description'   =>  $lpDetail->description,
         'language'      =>  getEntityFromCode($lpDetail->lang_code),
         'deliverytype'  =>  getEntityFromCode($lpDetail->delivery_type),
         'deliverytypecode'  =>  $lpDetail->delivery_type,
         //'valid_from'    =>  date("M d, Y  h:i A", $lpDetail->valid_from),
         //'valid_to'      =>  date("M d, Y  h:i A", $lpDetail->valid_to),
         'currency'      =>  $lpDetail->currency,
         'price'         =>  $lpDetail->price,
         'entityId'      =>  $entityId,
         'entityType'    =>  $entityType
       );
      }
     } 
	//include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_sitesetup/exp_sp_administration_module_info/exp_sp_administration_module_info_share.inc";
	$sharemodule=getShareModuleStatus();
    $status=$sharemodule['status'];
    $email=$sharemodule['share_email'];
    $embed=$sharemodule['share_embed'];
	 if($email == 1 && $embed == 1){
		$shareopt = 'both';
	 }elseif($email == 1 && $embed == 0){
		$shareopt = 'email';
	 }elseif($email == 0 && $embed == 1){
		$shareopt = 'embed';
     }else{
		$shareopt = 'none';
	 }
	 $res2 = array('shareopt' => $shareopt, 'sharemodule' => $status);
	 $res  = array_merge($res,$res2);
	 	 
     $res[0]->labelmsg = array('msg1' => t('MSG251'),'msg2' => t('LBL116'),'msg3' => t('LBL117'),'msg4' => t('LBL119'),'msg5' => t('LBL120'),'msg6' => t('LBL245'),'msg7' => strtolower(t('Optional')),'msg8' => t('LBL122'),'msg9' => t('LBL123'),'msg10' => t('LBL124'));
	//return json_encode($res);
  echo drupal_json_output($res);
  } catch (Exception $ex) {
  	watchdog_exception('getCourseClassDetail', $ex);
  	expertusErrorThrow($ex);
  }
}

function getCCDetailInArray($entityId,$entityType) {
	try{
  global $user;
  if($user->uid){
    $useremail = getLoggedUser($user->uid);
  }else{
    $useremail = "";
  }
  if($entityType == 'cre_sys_obt_cls') {
    $classDetail = getClassDetails($entityId);
    $classDetail = $classDetail[0];

      $res = array(
      	'title'         =>  $classDetail->title,
      	'email'         =>  $useremail,
      	'code'          =>  $classDetail->code,
      	'description'   =>  $classDetail->description,
      	'language'      =>  getEntityFromCode($classDetail->lang_code),
      	'deliverytype'  =>  getEntityFromCode($classDetail->delivery_type),
        'deliverytypecode'  =>$classDetail->delivery_type,
      	'valid_from'    =>  $classDetail->valid_from,
      	'valid_to'      =>  $classDetail->valid_to,
      	'price'         =>  $classDetail->price,
        'start_date'    =>  dateTimeStrToUnivShort($classDetail->start_date, 'Y','','',''),
      	'end_date'      =>  dateTimeStrToUnivShort($classDetail->end_date, 'Y','','',''),
        'start_time'    =>  $classDetail->start_time,
      	'end_time'      =>  $classDetail->end_time,
      	//'currency'      =>  getAttr2FromCode($classDetail->currency_type),
      	'currency'      =>  $classDetail->currency_code,
      	'location'      =>  getLocationFromId($classDetail->location_id),
      	'room_id'       =>  $classDetail->room_id,
      	//'instructor'    =>  getInstructorFromId($classDetail->instructor_id),
      	'capacity'      =>  $classDetail->capacity_min."  -  ".$classDetail->capacity_max,
      	'waitlist_count'=>  $classDetail->waitlist_count,
        'session_id'    =>  $classDetail->session_id,
      	'entityId'      =>  $entityId,
      	'entityType'    =>  $entityType
	  );
  }else if($entityType == 'cre_sys_obt_crs') {
    $classDetail = getCourseDetails($entityId);
  	$classDetail = $classDetail[0];
  	expDebug::dPrint('values to b e printed for share'.print_r($classDetail,true));
  	$res = array(
  			'title'         =>  $classDetail->title,
  			'email'         =>  $useremail,
  			'code'          =>  $classDetail->code,
  			'description'   =>  $classDetail->description,
  			'language'      =>  getEntityFromCode($classDetail->lang_code),
  			'deliverytype'  =>  getEntityFromCode('lrn_crs'),
  			'deliverytypecode'  =>  'lrn_crs',
  			'price'         =>  '-',
  			'entityId'      =>  $entityId,
  			'entityType'    =>  $entityType
  	);
  }
  else if($entityType == 'cre_sys_obt_crt' || $entityType == 'cre_sys_obt_trn' || $entityType == 'cre_sys_obt_cur') {
    $lpDetail = getLPDetails($entityId);
    $lpDetail = $lpDetail[0];

      $res = array(
      	'title'         =>  $lpDetail->title,
      	'email'         =>  $useremail,
      	'code'          =>  $lpDetail->code,
      	'description'   =>  $lpDetail->description,
      	'language'      =>  getEntityFromCode($lpDetail->lang_code),
      	'deliverytype'  =>  getEntityFromCode($lpDetail->delivery_type),
        'deliverytypecode'  =>  $lpDetail->delivery_type,
      	'valid_from'    =>  '',
      	'valid_to'      =>  '',
        'start_date'    =>  '',
      	'end_date'      =>  '',
        'start_time'    =>  '',
      	'end_time'      =>  '',
      	'location'      =>  '',
      	'room_id'       =>  '',
      	//'instructor'    =>  '',
      	'capacity'      =>  '',
      	'waitlist_count'=>  '',
        'session_id'    =>  '',
      	//'currency'      =>  getAttr2FromCode($lpDetail->currency),
      	'currency'      =>  $lpDetail->currency_code,
      	'price'         =>  $lpDetail->price,
      	'entityId'      =>  $entityId,
      	'entityType'    =>  $entityType
      );
  }

      return $res;
      } catch (Exception $ex) {
      	watchdog_exception('getCCDetailInArray', $ex);
      	expertusErrorThrow($ex);
      }
}

function shareClassAPI($user_id, $userid, $entityId,$entityType,$referFrom,$referTo,$comment,$ccopy){
try{

 	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

	//$uid = getDrupalIdOfUser($user_id);
 	  if (!valid_email_address(trim($referFrom))) {
 	  	$errobj=new stdClass();
 	  	$errobj->isValidateError = 1;
 	  	$errobj->errcode = 'L_012';
 	  	$errobj->errormsg = "Please enter a valid email in referFrom";
 	  	return $errobj;
	  }
 	  if (!valid_email_address(trim($referTo))) {
 	  	$errobj=new stdClass();
 	  	$errobj->isValidateError = 1;
 	  	$errobj->errcode = 'L_012';
 	  	$errobj->errormsg = "Please enter a valid email in referTo";
 	  	return $errobj;
	  }
 	  $status = sendCourseClassDetail($entityId,$entityType,$referFrom,$referTo,$comment,$ccopy,'api');
 	  expDebug::dPrint("$result$result$result$result Ajay: ".$status,4);
 	  if ($status) { // 46911 - return object array.
	  	return $status;
 	  } else {
 	  	//return array((object)array('status'=>'Failure'));
 	  	$errobj=new stdClass();
 	  	$errobj->isValidateError = 1;
 	  	$errobj->errcode = 'L_012';
 	  	$errobj->errormsg = "Enter a valid email.";
 	  	return $errobj;
 	  }
	}
catch (Exception $ex) {
	watchdog_exception('shareClassAPI', $ex);
	expertusErrorThrow($ex);
	}
}

function sendCourseClassDetail($entityId,$entityType,$referFrom,$referTo,$comment,$ccopy,$isAPI = '') {
	try {

  $emailList = explode(",",$referTo);
  $ccDetails = getCCDetailInArray($entityId,$entityType);
  expDebug::dPrint("SHARE TEST  to list :: ".print_r($emailList,true));
  expDebug::dPrint("SHARE TEST  originall list :: ".print_r($referTo,true));
  expDebug::dPrint("cc test list for share :: ".print_r($ccDetails,true));
  $userId = getIdOfLoggedInUser();
  $referrerId = getUserIdFromEmail($referFrom);
  $config           = getConfig("exp_sp");
  $configSiteUrl    = $config["admin_site_url"];
  $url_str = generateRandomSerialNumber(10);
  $shareUrl = trim($configSiteUrl).'/?q=share/training/'.$url_str;
  $subcomment_arr = explode('@|@',$comment);
  $subject = urldecode ($subcomment_arr[0]);
  $comment = urldecode ($subcomment_arr[1]);
  if($ccopy == false || $ccopy == 'false')
  	$ccopy = '';
  if ($entityType == 'cre_sys_cat_shr') {
    $shareType = 'Multiple Course - Share';
    $widgetMode = 'multiple';
    $widgetRedirectLink = trim($configSiteUrl).'/?q=learning/catalog-search/share';
    //link name given staticaly bcz share course mail template only in engish format.
    $viewLink ='<a href="'.$shareUrl.'">catalog</a>';
    $params = array(
      'entityId'          => $entityId,
      'entityType'        => $entityType,
      'title'             => $subject,
      'comment'			  => $comment,
      'ccopy'			  => $ccopy,
      'referfrom'		  => $referFrom,
      'viewlink'          => $viewLink
    );
  } else {
    $shareType = 'Single class - Share';
    $widgetMode = 'single';
    $params = array(
      'entityId'          =>  $entityId,
      'entityType'        =>  $entityType,
      'title'             =>  urldecode($ccDetails['title']),
      'code'              =>  $ccDetails['code'],
      'description'       =>  $ccDetails['description'],
      'language'          =>  $ccDetails['language'],
      'deliverytype'      =>  $ccDetails['deliverytype'],
      'deliverytypecode'  =>  $ccDetails['deliverytypecode'],
      'valid_from'     	  =>  $ccDetails['valid_from'],
      'valid_to'          =>  $ccDetails['valid_to'],
      'price'             =>  $ccDetails['price'],
      'start_date'        =>  $ccDetails['start_date'],
      'end_date'          =>  $ccDetails['end_date'],
      'start_time'		  =>  date("g:i A", strtotime($ccDetails['start_time'])),
      'end_time'          =>  date("g:i A", strtotime($ccDetails['end_time'])),
      'currency'          =>  $ccDetails['currency'],
      'location'          =>  $ccDetails['location'],
      'room_id'           =>  $ccDetails['room_id'],
      //'instructor'        =>  $ccDetails['instructor'],
      'capacity'          =>  $ccDetails['capacity'],
      'session_id'	      =>  $ccDetails['session_id'],
      'waitlist_count'    =>  $ccDetails['waitlist_count'],
      'comment'			  =>  $comment,
      'ccopy'			  =>  $ccopy,
      'referfrom'		  =>  $referFrom

    );
    $isLp=0;
    if($entityType == 'cre_sys_obt_crt' || $entityType == 'cre_sys_obt_trn' || $entityType == 'cre_sys_obt_cur') {
     $isLp =1;
     $widgetRedirectLink = trim($configSiteUrl).'/?q=share/learning/learning-plan-details/'.trim($params['entityId']).'/'.trim($params['session_id']);
     $viewLink ='<a href="'.$shareUrl.'">View Details</a>';
    }else if ($entityType == 'cre_sys_obt_crs') {
     $node_id = getNodeIdResults($entityId,$entityType);
     $widgetRedirectLink = trim($configSiteUrl).'/?q=share/learning/course-details/'.trim($node_id);
     $viewLink ='<a href="'.$shareUrl.'">View Details</a>';
    }
    else{
     $widgetRedirectLink = trim($configSiteUrl).'/?q=share/learning/class-details/'.trim($params['entityId']);
     $viewLink ='<a href="'.$shareUrl.'">View Details</a>';
    }
    $params['viewlink']= trim($viewLink);

    if ($params['deliverytypecode'] == 'lrn_cls_dty_wbt' ||
      $params['deliverytypecode'] == 'lrn_cls_dty_vod' ||
      $params['deliverytypecode'] == 'lrn_cls_dty_vcl') {
     $params['location'] = '-';
    }

    if ($params['deliverytypecode'] == 'lrn_cls_dty_wbt' || $params['deliverytypecode'] == 'lrn_cls_dty_vod') {
     $params['start_date_time'] = '-';
     $params['end_date_time'] = '-';
    }
    else {
     $params['start_date_time'] = $params['start_date'] . ' ' . $params['start_time'];
     $params['end_date_time'] = $params['end_date'] . ' ' . $params['end_time'];
    }
  }

  // Widget form params
  $widget_arr = array();
  if($widgetMode == 'single'){
  	$widget_arr['ent_id'] = $entityId;
   	$widget_arr['ent_type'] = $entityType;
   	$widget_arr['viewlink'] = $widgetRedirectLink;
   	$widget_str = serialize($widget_arr);
  } else if($widgetMode == 'multiple'){

  		if(!isset($_COOKIE['catalog_searchStr']) || $_COOKIE['catalog_searchStr']=='')
   		$widget_arr['rows'] = 10;
  		else{
  			$cookie_arr = explode('&',$_COOKIE['catalog_searchStr']);
  			foreach ($cookie_arr as $key=>$val){
  				if($key!=''){
  					$search_str_arr = explode('=',$val);
  					$widget_arr[$search_str_arr[0]] = $search_str_arr[1];
  				}
  			}
  		}
  	$widget_arr['viewlink'] = $widgetRedirectLink;
   	$widget_str = serialize($widget_arr);
  }
  // add new entry in the widget table
  $fields = array(
    'user_id' => $userId,
    'type' => 'email',
    'url' => $url_str,
    'mode' => $widgetMode,
    'options' => null,
    'parameters' => $widget_str,
    'theme' => null,
    'created_on' => now(),
    'created_by' => $userId
  );
  $insertStmt = db_insert('slt_widget');
  $insertStmt->fields($fields);
  $insertStmt->execute();

  if ($entityType == 'cre_sys_cat_shr') {
   /*--- Multi class share block ---*/
   foreach($emailList as $emailTo) {
    $referredToId = getUserIdFromEmail($emailTo);
    if ($referredToId > 0) {
     add_audit_trail_entry($referrerId, $entityId, $entityType, 'exp_sp_refer_course', $shareType,
       'Muliple catalog Shared with a registered email ' . $emailTo . ' (id = ' . $referredToId . ')');
    }
    else {
     add_audit_trail_entry($referrerId, $entityId, $entityType, 'exp_sp_refer_course', $shareType,
       'Muliple catalog Shared with a non-registered email ' . $emailTo);
    }
   } // end foreach
   $countShare = count($emailList);
   for($i=0;$i<$countShare;$i++){
    $params['sendToEmailId'] = $emailList[$i];
    $params['sendival'] = $i;
    $userExist = getUserIdFromEmail($emailList[$i]);
    $params['userExistinLMS'] = !empty($userExist) ? $userExist : '';
    checkShareNotificationOverRideInfo($entityId,$entityType,$params,'share_notification_catalog',getIdOfLoggedInUser());
   }
  } else {

   /*--- Single class share block ---*/
  // Make the audit trail entries
  foreach($emailList as $emailTo) {
   $referredToId = getUserIdFromEmail($emailTo);
   if ($referredToId > 0) {
    insertUserPointsForEachActionsPerformed($userId,'sharing_class');
    add_audit_trail_entry($referrerId, $entityId, $entityType, 'exp_sp_refer_course', $shareType,
      'Shared with a registered email ' . $emailTo . ' (id = ' . $referredToId . ')');
   }
   else {
    insertUserPointsForEachActionsPerformed($userId,'sharing_class');
    add_audit_trail_entry($referrerId, $entityId, $entityType, 'exp_sp_refer_course', $shareType,
      'Shared with a non-registered email ' . $emailTo);
   }
  } // end foreach
  $countShare = count($emailList);
  for($i=0;$i<$countShare;$i++){
     $params['sendToEmailId'] = $emailList[$i];
     $params['sendival'] = $i;
     $userExist = getUserIdFromEmail($emailList[$i]);
     $params['userExistinLMS'] = !empty($userExist) ? $userExist : '';
     checkShareNotificationOverRideInfo($entityId,$entityType,$params,'share_notification_class_tp',getIdOfLoggedInUser());
  }
  }



  if($isAPI == 'api'){
   		//$result = array();
  		return array((object)array('status'=>'success'));
  }

  } catch (Exception $ex) {
  	watchdog_exception('sendCourseClassDetail', $ex);
  	expertusErrorThrow($ex);
  }
}

function getLPDetails($entityId) {
	try{
		$UserDefaultCurrencyArr = getUserDefaultCurrency();
        $UserPrefCurrCode = $UserDefaultCurrencyArr['currency_code'];
  		$UserPrefCurrSym = $UserDefaultCurrencyArr['currency_sym'];

	  $select = db_select('slt_program', 'prg');
	   $select->addField('prg','title','title');
	  $select->addField('prg','code','code');
	  $select->addField('prg','short_desc','description');
	  $select->addField('prg','lang_code','lang_code');
	  $select->addField('prg','object_type','delivery_type');
	  $select->addField('prg','currency_type','currency');
	  //$select->addField('prg','price','price');
	  $select->addExpression('slf_convert_price(prg.price,prg.currency_type,\''.$UserPrefCurrCode.'\')','price');
	  $select->addExpression('\''.$UserPrefCurrSym.'\'','currency_symbol');
	  $select->addExpression('\''.$UserPrefCurrCode.'\'','currency_code');
	  $select->condition('prg.id', $entityId);
	  $select->range(0,1);
	  $result = $select->execute()->fetchAll();
   	return $result;
   } catch (Exception $ex) {
   	watchdog_exception('getLPDetails', $ex);
   	expertusErrorThrow($ex);
   }
}


function checkShareNotificationOverRideInfo($objectId1,$entityType,$notifyResult,$notificationCode,$userId) {
	try{
		expDebug::dPrint('$notifyResult Info'.print_r($notifyResult,true),4);
  /*-- Muliple catalog share email insert --*/
  if ($entityType == 'cre_sys_cat_shr') {
    $overrideContent = '';
    $notificationTemplateInfo = getNotificationTemplateInfo($notificationCode, 'cre_sys_lng_eng');
    $notifyFlag = null;
    // Both 'Y' and 'cre_ntn_sts_atv' are active statuses
    if ($notificationTemplateInfo['status'] == 'cre_ntn_sts_atv') {
     $notifyFlag = 'Y';
    }
    $frameString = true;
    if ($notifyFlag != 'Y') {
      expDebug::dPrint('Multiple Notification is disabled');
      $frameString = false;
    }
    $getPersonDetails = getPersonDetails($userId, array('email','full_name'));
    if (!empty($notifyResult['referfrom'])){
      $referemail= $notifyResult['referfrom'];
    } else{
      $referemail= $getPersonDetails['email'];
    }
    if($frameString){
     $notificationInfo = null;
     $title = ($notifyResult['title'] == '') ? t('LBL1274') : $notifyResult['title'];
     $notificationInfo['tokens_string'] =     'course_title>|' .$title. '~|'.
       'email_id>|' .$notifyResult['sendToEmailId']. '~|'.
       'SharefromId>|' .$referemail.'~|'.
       'SharefromName>|' .$getPersonDetails['full_name'].'~|' .
       'comment>|' . $notifyResult['comment'] . '~|'.
       'catalog_link>|' . $notifyResult['viewlink'];

     $notificationInfo['message_id']    = $notificationCode;
     $notificationInfo['message_type']  =  'Multiple Catalog Share Notification';
     $notificationInfo['send_type']     = 'php mailer';
     $notificationInfo['lang_code']     = 'cre_sys_lng_eng'; // temp value. Need to check with vincent
     $notificationInfo['sendToEmail']   = $notifyResult['sendToEmailId'];
     $notificationInfo['ccopy']         = $notifyResult['ccopy'];
     $notificationInfo['referfrom']     = $notifyResult['referfrom'];
     $notificationInfo['sendival']      = $notifyResult['sendival'];
     $notificationInfo['userExistinLMS'] = $notifyResult['userExistinLMS'];

     expDebug::dPrint('Notification Template Info'.print_r($notificationTemplateInfo,true),4);
     expDebug::dPrint('Notification Info'.print_r($notificationInfo,true),4);
     callCommonNotificationInsert($notificationCode,$notificationInfo,$notificationTemplateInfo,$userId,$overrideContent);
    }

  } else {
   /*-- Single share email insert --*/
  $notifyOverrideFlag = null;
  $overrideContent = '';
  $overrideInfo = getEntityNotificationOverrideInfo($objectId1, $entityType, $notificationCode, 'Email');
  expDebug::dPrint(' share $overrideInfo = ' . print_r($overrideInfo, true) , 4);
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
    	expDebug::dPrint('Notification is disabled');
    	$frameString = false;
		}else{
			//If the notification status has been overridden and set to disabled, do not generate notification.
			expDebug::dPrint(' Notification is overridded and disabled');
			$frameString = false;
		}
	}
	$getPersonDetails = getPersonDetails($userId, array('email','full_name'));
	if(!empty($notifyResult['referfrom'])){ $referemail= $notifyResult['referfrom'];}else{ $referemail= $getPersonDetails['email'];}
	$price = !empty($notifyResult['price'])?$notifyResult['currency'].' '.$notifyResult['price']:'Nil';
	if($frameString){
    	  $notificationInfo = null;
		  $notificationInfo['tokens_string'] =     'course_title>|' .$notifyResult['title']. '~|'.
                                                   'code>|' .$notifyResult['code'] . '~|' .
                                                   'description>|' .$notifyResult['description'].'~|' .
                                                   'language>|' .$notifyResult['language'] . '~|' .
                                                   'delivery_type>|'.$notifyResult['deliverytype'].'~|' .
		                                           'comment>|' . $notifyResult['comment'] . '~|'.
		                                           'viewlink>|' . $notifyResult['viewlink'].'~|'.
		                                           'SharefromId>|' .$referemail.'~|'.
		                                           'SharefromName>|' .$getPersonDetails['full_name'].'~|' .
                                                   'price>|' . trim($price);

		  $notificationInfo['message_id']    = $notificationCode;
		  $notificationInfo['message_type']  = 'Share Notification';
		  $notificationInfo['send_type']     = 'php mailer';
		  $notificationInfo['lang_code']     = $notifyResult['language'];
		  $notificationInfo['sendToEmail']   = $notifyResult['sendToEmailId'];
		  $notificationInfo['ccopy']         = $notifyResult['ccopy'];
		  $notificationInfo['referfrom']     = $notifyResult['referfrom'];
		  $notificationInfo['sendival']      = $notifyResult['sendival'];
		  $notificationInfo['userExistinLMS'] = $notifyResult['userExistinLMS'];
		  expDebug::dPrint('Notification Template Info'.print_r($notificationTemplateInfo,true),4);
		  expDebug::dPrint('Notification Info'.print_r($notificationInfo,true),4);
		  callCommonNotificationInsert($notificationCode,$notificationInfo,$notificationTemplateInfo,$userId,$overrideContent);
	}
  }
	} catch (Exception $ex) {
		watchdog_exception('checkShareNotificationOverRideInfo', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Function used to render the embed widget
 *
 */
function getEmbedCourseClassDetail($entityId,$entityType) {
	try {
	global $user;
	if (user_access('Administration Perm', $user) == false){
	 echo 'access_denied';
	 exit();
	}

	$uniqueId = generateRandomSerialNumber(10);
	// standard video vector size.
    $videoSizeOptions = array(
      '640x480' => '640 x 480',
      '768x576' => '768 x 576',
      '800x600' => '800 x 600',
      '1024x768' => '1024 x 768'
    );
    $res = array(
   	  'entityId'      => $entityId,
   	  'entityType'    => $entityType,
      'sizeOptions'   => $videoSizeOptions,
      'uniqueId'      => $uniqueId
    );
	$sharemodule=getShareModuleStatus();
    $status=$sharemodule['status'];
    $email=$sharemodule['share_email'];
    $embed=$sharemodule['share_embed'];
	 if($email == 1 && $embed == 1){
		$shareopt = 'both';
	 }elseif($email == 1 && $embed == 0){
		$shareopt = 'email';
	 }elseif($email == 0 && $embed == 1){
		$shareopt = 'embed';
     }else{
		$shareopt = 'none';
	 }
	 $res2 = array('shareopt' => $shareopt, 'sharemodule' => $status);
	 $res  = array_merge($res,$res2);
     $res[0]->labelmsg = array('msg9' => t('LBL123'),'msg10' => t('LBL124'));
    echo drupal_json_output($res);
  } catch (Exception $ex) {
  	watchdog_exception('getEmbedCourseClassDetail', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Function to save the embed widget parameters
 */
function saveEmbedCourseClassDetail($widgetMode, $combinedParam) {
 try {
 		$parameter_arr = explode('|',$combinedParam);
 		$url_str = $parameter_arr[0];
 		$displayParameters_arr['show_icon'] = ($parameter_arr[3]=='true')?1:0;
 		$displayParameters_arr['show_description'] = ($parameter_arr[4]=='true')?1:0;
 		$displayParameters_arr['show_button'] = ($parameter_arr[5]=='true')?1:0;
 		$displayParameters_arr['show_separator'] = ($parameter_arr[6]=='true')?1:0;
 		$display_size = $parameter_arr[7];
 		$displayParameters_arr['pass_url'] = ($parameter_arr[8]=='true')?1:0;
 		$displayParameters_str = serialize($displayParameters_arr);
   if($widgetMode=='single'){
   		$widget_arr['ent_id'] = $parameter_arr[1];
    	$widget_arr['ent_type'] = ($parameter_arr[2] == 'cre_sys_obt_crt' || $parameter_arr[2] == 'cre_sys_obt_trn' || $parameter_arr[2] == 'cre_sys_obt_cur')?'cre_sys_obt_trp':$parameter_arr[2];
    	$widget_arr['rows'] = 10;
    	if($widget_arr['ent_type'] == 'cre_sys_obt_crs'){
				$select = db_select ( 'slt_node_learning_activity', 'node' );
				$select->addField ( 'node', 'node_id' );
				$select->condition ( 'node.entity_id', $widget_arr ['ent_id'], '=' );
				$select->condition ( 'node.entity_type', 'cre_sys_obt_crs', '=' );
				$nodeId = $select->execute ()->fetchField ();
				$widget_arr ['viewlink'] = 'learning/course-details/' . $nodeId . '';
			}
    	else if($widget_arr['ent_type'] == 'cre_sys_obt_trp')
    		$widget_arr['viewlink'] = 'learning/learning-plan-details/'.$widget_arr['ent_id'].'';
    	
    	else 
    		$widget_arr['viewlink'] = 'learning/class-details/'.$widget_arr['ent_id'].'';
    	
    	 
    	$widget_str = serialize($widget_arr);
   }elseif($widgetMode=='multiple'){
   		$widget_arr = array();
   		global $language ;
   		if(!isset($_COOKIE['catalog_searchStr']) || $_COOKIE['catalog_searchStr']==''){
   			$widget_arr['rows'] = 10;
   			$selectStmt = db_select('slt_profile_list_items','spli');
   			$selectStmt->addField('spli','code','code');
   			$selectStmt->condition('spli.attr1',$language->language,'=');
   			$result = $selectStmt->execute()->fetchAssoc();
    		$widget_arr['lg_type'] = $result['code'];
    		$widget_arr['language'] = $language->language ;
   		}
   		else{
   			$cookie_arr = explode('&',$_COOKIE['catalog_searchStr']);
   			foreach ($cookie_arr as $key=>$val){
   				if($key!=''){
   					$search_str_arr = explode('=',$val);
   					$widget_arr[$search_str_arr[0]] = $search_str_arr[1];
   				}
   			}
   			$widget_arr['language'] = $language->language;
   		}
    	$widget_str = serialize($widget_arr);
   }
   $userId = getIdOfLoggedInUser();

   if(checkUniqueIdExist($url_str)){
	   	$updatewidget = db_update('slt_widget');
	   	$updatewidget->fields(array(
   			'mode'													=> $widgetMode,
	   		'display_size'									=> $display_size,
   			'options'     									=> $displayParameters_str,
   			'parameters'          					=> $widget_str,
   			'updated_on'           					=> now(),
   			'updated_by'										=> $userId
   	));
	   	$updatewidget->condition('url', $url_str, '=');
	   	$updatewidget->condition('user_id', $userId, '=');
	   	$updatewidget->execute();
   }else{
   	$fields = array(
   			'user_id'              					=> $userId,
   			'type'					 								=> 'embed',
   			'url'					 					  			=> $url_str,
   			'mode'													=> $widgetMode,
   			'display_size'									=> $display_size,
   			'options'     									=> $displayParameters_str,
   			'parameters'          					=> $widget_str,
   			'theme'              						=> null,
   			'created_on'           					=> now(),
   			'created_by'										=> $userId
   	);
	   $insertStmt = db_insert('slt_widget');
	   $insertStmt->fields($fields);
	   $insertStmt->execute();
   }
  }catch (Exception $ex) {
   watchdog_exception('saveEmbedCourseClassDetail', $ex);
   expertusErrorThrow($ex);
 }
}

/**
 * Function to generate random unique id for embed url
 */
function generateRandomSerialNumber($tokenLength = 10) {
 $uniqueId = getToken($tokenLength);
 $checkIdExist = checkUniqueIdExist($uniqueId);
 if($checkIdExist) {
  $uniqueId = getToken($tokenLength);
 }
 return $uniqueId;
}
/**
 * Function used to check the unique id present in the table
 */
function checkUniqueIdExist($uniqueId) {
 $select = db_select('slt_widget', 'widget')
 ->fields('widget',array('id'))
 ->condition('url', $uniqueId,'=');
 //expDebug::dPrintDBAPI(' $$$select = ' ,$select);
 $result= $select->execute();
 $num_of_results = $result->rowCount();
 if ($num_of_results > 0) {
  return true;
 } else {
  return false;
 }
}

function crypto_rand_secure($min, $max) {
  $range = $max - $min;
  if ($range < 0) return $min;
  $log = log($range, 2);
  $bytes = (int) ($log / 8) + 1;
  $bits = (int) $log + 1;
  $filter = (int) (1 << $bits) - 1;
  do {
   $rnd = hexdec(bin2hex(openssl_random_pseudo_bytes($bytes)));
   $rnd = $rnd & $filter;
  } while ($rnd >= $range);
  return $min + $rnd;
}

function getToken($length){
  $token = "";
  $codeAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  $codeAlphabet.= "abcdefghijklmnopqrstuvwxyz";
  $codeAlphabet.= time();
  for($i=0;$i<$length;$i++){
   $token .= $codeAlphabet[crypto_rand_secure(0,strlen($codeAlphabet))];
  }
  return $token;
}

function shareTrainingRedirect($url_str = '') {
 $selectStmt = db_select('slt_widget','widget');
 $selectStmt->addField('widget','type','widget_type');
 $selectStmt->addField('widget','mode','widget_mode');
 $selectStmt->addField('widget','parameters','widget_parameters');
 $selectStmt->condition('widget.url',$url_str,'=');
 $result = $selectStmt->execute()->fetchAssoc();
 expDebug::dPrintDBAPI(' $selectStmt SQL = ' , $selectStmt);
 expDebug::dPrint('Notification is disabled' . print_r($result, 1), 4);
 
 // Check the widget type is mail share
 if ($result['widget_type'] == 'email' || $result['widget_type'] == 'embed') {
 	$_SESSION['base_url_from'] = 'share';
  $parameters = unserialize($result['widget_parameters']);
   if ($result['widget_mode'] == 'single')   {
  	if($result['widget_type'] == 'email'){
     if(strpos($parameters['viewlink'], 'share/learning/course-details') > 0){
   	    	$modName = 'exp_sp_coursedetail';
   	    	$deltaName = 'course_details';
   	    	//$destStr = 'share/learning/course-details';
   	    }else if(strpos($parameters['viewlink'], 'share/learning/class-details') > 0){
   	    	$modName = 'exp_sp_classdetail';
   	    	$deltaName = 'class_details';
   	    	$destStr = 'share/learning/class-details';
   	    }else{
   	    	$modName = 'exp_sp_learning_plan_detail';
   	    	$deltaName = 'learning_details';
   	    	$destStr = '';
   	    }
   	    // Function to get the block permission
   	    $blckPrm = block_role_permission_check($modName,$deltaName);
   	    if($blckPrm === false ){
   	    	$dest = explode('?q=',$parameters['viewlink']);
   	    	$query = array('destination' => $dest[1]);
   	    	drupal_goto('share/unauthorised',array('query' => $query)); 
   	    }else{
    			drupal_goto($parameters['viewlink']);
   	    }
  	}
   	else{
   		expDebug::dPrint('Notification is disabled embed single' . print_r($parameters['viewlink'], 1), 4);
   		drupal_goto($parameters['viewlink']);
   	}
  } else {
   $widget_parameters_str = '';
   foreach ($parameters as $key=>$val)
   {
    if($key != 'viewlink') {
     $widget_parameters_str .= '&'.$key.'='.$val;
    }
   }
   //echo $widget_parameters_str;
    //setcookie("thamil", $parameters);
    if($result['widget_type'] == 'embed'){
    	setcookie("catalog_searchStr_passurl", $widget_parameters_str,time() + (300), "/");
    	$arg_arr = array('query' =>array('page_number'=>$_GET['page_number'],'click_id'=>$_GET['click_id'],'row_number'=>$_GET['row_number']));
    	
    	drupal_goto('learning/catalog-search/share',$arg_arr);
    	
    }else{
    	$_SESSION['share_details'] = $widget_parameters_str;
//     	expDebug::dPrint('catalog filters in session '.var_export($_SESSION['catalogcookies']['catalogAppliedFilters'], 1), 5);
    	$_SESSION['catalogcookies']['catalogAppliedFilters'] = $parameters['catalogAppliedFilters'];
    	drupal_add_js(array('catalog_variables' => array('catalogAppliedFilters' => json_decode($_SESSION['catalogcookies']['catalogAppliedFilters']))), 'setting');
    	setcookie("catalog_searchStr", "", time()-3600,'/');
    	setcookie("catalog_temp_searchStr", "", time()-3600,'/');
    	setcookie("current_page", "", time()-3600,'/');
    	setcookie("widget_searchstr", "", time()-3600,'/');
    	setcookie("searchStr_read", "", time()-3600,'/');
    	setcookie("catalog_searchStr", $widget_parameters_str, time()+3600,'/');
   	    $parameters['viewlink'] = str_replace("//?","/?",$parameters['viewlink']);
   	    expDebug::dPrint("Check the view link for multiple class".strpos($parameters['viewlink'], 'share/learning/course-details'));
   	    if($result['widget_type'] == 'multiple'){
   	    	$modName = 'exp_sp_lnrsearch';
   	    	$deltaName = 'tab_find_training';
   	    	//$destStr = 'learning/catalog-search';
   	    }else if(strpos($parameters['viewlink'], 'share/learning/course-details') > 0){
   	    	$modName = 'exp_sp_coursedetail';
   	    	$deltaName = 'course_details';
   	    	//$destStr = 'share/learning/course-details';
   	    }else if(strpos($parameters['viewlink'], 'share/learning/class-details') > 0){
   	    	$modName = 'exp_sp_classdetail';
   	    	$deltaName = 'class_details';
   	    	$destStr = 'share/learning/class-details';
   	    }else{
   	    	$modName = 'exp_sp_learning_plan_detail';
   	    	$deltaName = 'learning_details';
   	    	$destStr = '';
   	    }
   	    // Function to get the block permission
   	    $blckPrm = block_role_permission_check($modName,$deltaName);
   	    expDebug::dPrint("Block permission result".$blckPrm,4);
   	    if($blckPrm === false){
   	    	$dest = explode('?q=',$parameters['viewlink']);
   	    	$query = array('destination' => $dest[1]);
   	    	drupal_goto('share/unauthorised',array('query' => $query)); 
   	    }else
    			drupal_goto($parameters['viewlink']);
    }

  }
 } else  {
   return t("It's not a valid URL. Please contact support Team");
 }
}
?>