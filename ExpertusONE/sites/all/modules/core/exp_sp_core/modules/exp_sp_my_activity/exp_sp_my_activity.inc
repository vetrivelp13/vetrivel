<?php 
/*
 * Get list of Activity Details
 */
function getMyProfileActivityDetails($usid,$countrequired=false) {
 try{
    
  $select = db_select('slt_audit_trail', 'activity');
  $select->addField('activity','entity_id','entity_id');
  $select->addField('activity','functionality_name','functionality_name');
  $select->addField('activity','entity_type','entity_type');
  $select->addField('activity','module_name','module_name');
  $select->addField('activity','old_value','old_value');
  $select->addField('activity','new_value','new_value');
  $select->addField('activity','logged_user_action','logged_user_action');
  $select->addField('activity','created_on','created_on');
  $select->addField('activity','id','activityid');
  $select->addField('activity','custom0','custom0');
  $weekAgo= date("Y-m-d",strtotime("-1 week"));
  $select->condition('activity.logged_user_id',$usid);
  $select->condition('activity.mod_user_id',$usid);
  $select->condition('activity.custom0', '', 'IS NULL');
  $select->condition('activity.custom4','','IS NULL');
  //$select->condition('activity.custom0', 1, '!=');
  //$select->condition('activity.created_on', $weekAgo, '>=');
  $select->orderBy('activity.id', 'DESC');
  $select->condition('activity.entity_type', array('cre_sys_obt_cur','cre_sys_obt_trn','cre_sys_obt_crt','cre_sys_obt_cls','cre_sys_obt_usr','cre_sys_obt_not','Class','Curricula','Certification','Notice','Learning Plan','User'),'IN');
  //for ticket 0021440 on 6/2/13 by priya.c
  //$select->condition('activity.functionality_name', array('updateUserAddressDetails','updateUserOrgDetails','updateUserBasicDetails','New User Registration','enrollUserToClass','updateClassDetails','updateSessionDetails','addNewClass','addNewSessionDetails','addNewContentDetails','contentSaveEditOption','Update Training Plan','updateEnrollment','new attachment','classcancelled','classcompleted','classenrolled','tpregistered','tpcancelled','myaccount','myprofilepersonal','tpcompleted','myprofile'),'IN');
  $select->condition('activity.functionality_name', array('enrollUserToClass','classcancelled','classcompleted','tpregistered','tpcancelled','myaccount','myprofilepersonal','tpcompleted','myprofile','updateskilldetails','addnewskilldetails','deleteskilldetails','passwordupdated','classincomplete','tpincomplete','classswitch','tprecertify','dismissannouncement','addnewCertificatedetails','updatecertificatedetails','deletecertificatedetails'),'IN');
  //$select->groupBy('activity.entity_id');
  //$select->groupBy('activity.functionality_name');
   if($countrequired==true){
    $activityDetails = count($select->execute()->fetchAll());
  }else{
    $select->range($start, $limit);
    $activityDetails = $select;
  }
  expDebug::dPrintDBAPI("Db_PrintAPI", $select);
  
  expDebug::dPrint('getMyProfileActivityDetails : obj = ' . print_r($activityDetails, true) , 3);
  expDebug::dPrint('  LIST $result = ' . print_r($activityDetails, true) , 3);
  return $activityDetails;
  }catch (Exception $ex) {
    watchdog_exception('getMyProfileActivityDetails', $ex);
    expertusErrorThrow($ex);
  }
}

function timeDifferenceLoggedAction($created_on_date){
  try{
  $currentDate= now();
  global $language;
  $start_date = new DateTime($currentDate);
  $since_start = $start_date->diff(new DateTime($created_on_date));
  if($since_start->d > 0){
  	if($since_start->d == 1) {
  		$days_ago = t('LBL862');//'A day ago';
  	}else {
  		if($language->language == 'es')
  			$days_ago = t('LBL842',array('@days' =>$since_start->d)); // Spanish translated #0043422
  		else
  			$days_ago = $since_start->d.' '.t('LBL842'); //More than 1 day
  	}
  }else if($since_start->h <= 23 && $since_start->h > 0){
  	if($since_start->h < 2) {
  		$days_ago = t('LBL861');//'An hour ago';
  	}else {
  		if($language->language == 'es')
  			$days_ago = t('LBL843',array('@hours' =>$since_start->h)); // Spanish translated #0043422
  		else
  			$days_ago = $since_start->h.' '.t('LBL843'); //More than 1 hour
  	}
  }else if($since_start->i <= 59 && $since_start->i > 0){
  	if($since_start->i < 2) {
  		$days_ago = t('LBL838');//'A minute ago';
  	}else{
  		if($language->language == 'es')
  			$days_ago = t('LBL839',array('@minutes' =>$since_start->i)); // Spanish translated #0043422
  		else
  			$days_ago = $since_start->i.' '.t('LBL839');
  	}
  }else{
  	if ($days_ago = $since_start->s < 2){
  		$days_ago = t('LBL840');//'A second ago';
  	}else {
  		if($language->language == 'es')
  			$days_ago = t('LBL841',array('@seconds' =>$since_start->s)); // Spanish translated #0043422
  		else
  			$days_ago = $since_start->s.' '.t('LBL841');
  	}
  }
    return $days_ago;
  }catch (Exception $ex) {
    watchdog_exception('timeDifferenceLoggedAction', $ex);
    expertusErrorThrow($ex);
  }   
}

function delete_activity_process($entityId,$entityType,$functionalityName,$activityId) {
    try{
  $updatedUserId = getSltpersonUserId();
  $select = db_update('slt_audit_trail');
  $select->fields(array('custom0'     => '1'));
  $select->condition(db_or()->
                      condition('id',$activityId)->
                      condition('custom4',$activityId)
                      );
  $select->condition('logged_user_id', $updatedUserId, '=');
  $result = $select->execute();
  }catch (Exception $ex) {
    watchdog_exception('delete_activity_process', $ex);
    expertusErrorThrow($ex);
  }
}
  
function getClassTypeFromId($entityId) {
    try{
  $select = db_select('slt_course_class', 'class');
  $select->addField('class','delivery_type','delivery_type');
  $select->addField('class','title','title');
  $select->condition('class.id',$entityId);
  $classDeliveryType = $select->execute()->fetchAll();
  return $classDeliveryType;
  }catch (Exception $ex) {
    watchdog_exception('getClassTypeFromId', $ex);
    expertusErrorThrow($ex);
  }
 }
function getTPTypeFromId($entityId) {
    try {
  $select = db_select('slt_program', 'prog');
  $select->addField('prog','title','title');
  $select->condition('prog.id',$entityId);
  $progDetails = $select->execute()->fetchAll();
  return $progDetails;
  }catch (Exception $ex) {
    watchdog_exception('getTPTypeFromId', $ex);
    expertusErrorThrow($ex);
  } 
  }
function activityDisplayDetails($keyword) {
  try{
  $keyword = strtoupper($keyword);
  switch($keyword) {
    /*for ticket 0021440 on 6/2/13 by priya.c
     * case 'ADDNEWUSER':
      $activityAction = t('Created User');
      break;
    case 'NEW USER REGISTRATION':
      $activityAction = t('Created User');
      break;  
    case 'UPDATEUSERADDRESSDETAILS':
       $activityAction = t('Updated Address');
      break;      
    case 'UPDATEUSERORGDETAILS':
      $activityAction = t('Updated Organization');
      break;
    case 'UPDATEUSERBASICDETAILS':
      $activityAction = t('Updated User Details');
      break;*/
    case 'ENROLLUSERTOCLASS':
      $activityAction = t('Class').' '.t('Registered');
      break; 
    /*for ticket 0021440 on 6/2/13 by priya.c
     * case 'UPDATECLASSDETAILS':
      $activityAction = t('Updated Class');
      break;       
    case 'UPDATESESSIONDETAILS':
      $activityAction = t('Updated Session');
      break;
    case 'ADDNEWCLASS':
     $activityAction = t('Add New Class');
      break;
    case 'ADDNEWSESSIONDETAILS':
      $activityAction = t('Add New Session');
      break;
    case 'ADDNEWCONTENTDETAILS':
      $activityAction = t('Add New Content');
      break; 
    case 'UPDATE TRAINING PLAN':
       $activityAction = t('Update Training Plan');
      break;  
   case 'UPDATEENROLLMENT':
     $activityAction = t('Update enrollment');
     break; 
   case 'ADD NEW TRAINING PLAN':
     $activityAction = t('Created Training Plan');
     break;
  case 'NEW ATTACHMENT':
     $activityAction = t('Created New Attachment');
     break;*/
  case 'CLASSCANCELLED':
     $activityAction = t('MSG551');
     break;
  case 'CLASSCOMPLETED':
     $activityAction = t('Class').' '.t('Completed');
     break;  
 /* for ticket 0021440 on 6/2/13 by priya.c
  * case 'CLASSENROLLED':
     $activityAction = t('Class Enrolled');
     break;  */
  case 'TPREGISTERED':
     $activityAction = t('MSG552');
     break;
  case 'TPCANCELLED':
     $activityAction = t('MSG555');
     break; 
  case 'MYACCOUNT':
     $activityAction = t('MSG553');
     break;
  case 'MYPROFILEPERSONAL':
     $activityAction = t('MSG553');
     break;
  case 'TPCOMPLETED':
     $activityAction = t('MSG554');
     break;
  case 'MYPROFILE':
     $activityAction = t('MSG553');
     break;
  case 'UPDATESKILLDETAILS':   
     $activityAction = t('MSG557');
     break;
  case 'ADDNEWSKILLDETAILS': 
     $activityAction = t('MSG558');
     break; 
  case 'DELETESKILLDETAILS': 
     $activityAction = t('MSG668');
     break; 
  case 'PASSWORDUPDATED':
     $activityAction = t('MSG559');
     break;   
  case 'CLASSINCOMPLETE':
     $activityAction = t('Class').' '.strtolower(t('Incomplete'));
     break;  
  case 'TPINCOMPLETE':
     $activityAction = t('MSG556');
     break; 
  case 'CLASSSWITCH':
     $activityAction = t('Class Changed');
     break;   
  case 'TPRECERTIFY':
        $activityAction = t('Tp Recertified');
        break;
  case 'DISMISSANNOUNCEMENT':
        $activityAction = t('Announcement Dismissed');
        break;
  case 'ADDNEWCERTIFICATEDETAILS':
        $activityAction = 'Certificate Added';
        break;
  case 'UPDATECERTIFICATEDETAILS':
        $activityAction = 'Certificate Updated';
        break;
  case 'DELETECERTIFICATEDETAILS':
        $activityAction = 'Certificate deleted';
        break;
  }
  return $activityAction;
  }catch (Exception $ex) {
    watchdog_exception('activityDisplayDetails', $ex);
    expertusErrorThrow($ex);
  }  
}

/*
 * getUserIdFromSkillId: get the usreId from skill id
*/
function getUserIdFromSkillId($entityId){
    try{
  $selectStmt = db_select('slt_skill_set', 'skills');
  $selectStmt->addField('skills','user_id','logged_user_id');
  $selectStmt->condition('skills.id', $entityId);
  expDebug::dPrintDBAPI('getUserIdFromSkillId ' , $selectStmt);   
  $result =  $selectStmt->execute()->fetchField();
  return $result;
  }catch (Exception $ex) {
    watchdog_exception('getUserIdFromSkillId', $ex);
    expertusErrorThrow($ex);
  }
 }
?>