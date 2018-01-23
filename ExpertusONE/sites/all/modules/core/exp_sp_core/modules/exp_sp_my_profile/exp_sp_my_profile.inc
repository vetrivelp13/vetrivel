<?php

function getMyProfile($usid) {
	try {
  $select = db_select('slt_person', 'person');
  $select->leftJoin('slt_organization', 'org', 'person.org_id = org.id');
  $select->addField('person','id', 'person_id');
  $select->addField('person','last_name', 'person_last_name');
  $select->addField('person','full_name', 'person_full_name');
  $select->addField('person','manager_id', 'person_manager_id');
  $select->addField('person','job_title', 'job_title');
  $select->addField('person','about_me', 'about_me');
  $select->addField('org','name', 'orgname');
  $select->condition('person.id', $usid);
  $result = $select->execute()->fetchObject();
  return $result;
  }catch (Exception $ex) {
  	watchdog_exception('getMyProfile', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * Get manager from slt_person
 */
function getManager($mgrId){
  try{
  $select = db_select('slt_person', 'sltperson');
  $select->addField('sltperson', 'full_name','full_name');
  $select->condition('sltperson.id', $mgrId);
  $managerdtl	= $select->execute()->fetchAll();
  $manager		= $managerdtl[0]->full_name;
  expDebug::dPrint('Manager = ' . print_r($manager, true) , 3);
  return $manager; 
  }catch (Exception $ex) {
    watchdog_exception('getManager', $ex);
    expertusErrorThrow($ex);
  } 
}

/*
 * Get list of job_title in slt_profile_list_items
 */
function getJobTitle($jobrole){
 try{
  $select = db_select('slt_profile_list_items', 'spfa');
  $select->addField('spfa', 'name','jobname');
  $select->condition('spfa.code', $jobrole);
  $jobtitle = $select->execute()->fetchField();
  return $jobtitle;
  }catch (Exception $ex) {
    watchdog_exception('getJobTitle', $ex);
    expertusErrorThrow($ex);
  } 
}

/*
 * Get Job code from slt_profile_list_items
 */
function getJobCode($jobTitle){
 try{
    $select = db_select('slt_profile_list_items', 'spfa');
    $select->addField('spfa', 'code','jobcode');
    $select->condition('spfa.name', $jobTitle , '=');
    $jobCode = $select->execute()->fetchField();
    
    expDebug::dPrintDBAPI('getJobCode QRY = ' , $select);
    expDebug::dPrint('getJobCode = ' . print_r($jobCode, true));
    
    if(!empty($jobCode)){
      return $jobCode;
    }else{
      if(!empty($jobTitle)){
        global $user;
        $fields  = array(
      	'code'        => createJobCode($jobTitle),
      	'lang_code'   => 'cre_sys_lng_eng',
      	'name'        => $jobTitle,
      	'is_active'        => 'Y',
      	'created_by'  => $user->uid,
      	'created_on'  => now(),
        'updated_by'  => $user->uid,
        'updated_on'  => now()
        );
        $insert = db_insert('slt_profile_list_items');
  	    $insert->fields($fields);
  	    $insert->execute();
  	    expDebug::dPrintDBAPI('getJobCode QRY INSERT = ' , $insert);
  	    return $fields['code'];
      }
    }
       
  }catch (Exception $ex) {
    watchdog_exception('getJobCode', $ex);
    expertusErrorThrow($ex);
  } 
}

/*
 * Create Job code if it's does not exist in slt_profile_list_items.
 */
function createJobCode($jobTitle){
 try{
  
  $jPattn = 'cre_usr_jtl_'.strtolower($jobTitle[0]); 
  $jCode = $jPattn.'01';
  $regexp = $jPattn.'[0-9]';
  
  $select = db_select('slt_profile_list_items', 'spfa');
  $select->addExpression('max(SUBSTRING(spfa.code,14))','jobcode');
  $select->condition('spfa.code', $regexp , 'REGEXP');
  $jobCode = $select->execute()->fetchField();
  
  expDebug::dPrintDBAPI('createJobCode QRY = ' , $select);
  expDebug::dPrint('createJobCode $jobCode = ' . print_r($jobCode, 3));
  
  $number = 1;
  if(empty($jobCode)){
    $newJobCode = $jCode;
    expDebug::dPrint('createJobCode ONE = ' . print_r($newJobCode, true));
  }else{
    $number = str_pad(++$jobCode, 2, '0', STR_PAD_LEFT);
    $newJobCode = 'cre_usr_jtl_'.$jPattn.$number;
    expDebug::dPrint('createJobCode TWO = ' . print_r($newJobCode, true));
  }
  
  return $newJobCode;
  
  }catch (Exception $ex) {
    watchdog_exception('createJobCode', $ex);
    expertusErrorThrow($ex);
  } 
}

/*
 * Using this function to update user information in slt_person table
 * 
 */
function updateProfile($resultRecord, $user_id, $defaultValues){
  global $theme_key; 
  $txn = db_transaction();
  try{	
  $prevValues = getMyProfile($user_id);  
  $fields = array(
    'about_me'      => ($theme_key == 'expertusoneV2') ? $resultRecord['about_me'] : $resultRecord['about_me']['value'],
  	'updated_on'    => now(),
    'updated_by'    => $user_id,
  ); 
  
  $update = db_update('slt_person');
  $update->fields($fields);  
  $update->condition('id', $user_id);
  $update->execute();
  expDebug::dPrintDBAPI("Update query", $update);
  if($prevValues->about_me != html_entity_decode($resultRecord['about_me'],ENT_QUOTES,UTF-8)){
  $userId = getIdOfLoggedInUser();	
  add_audit_trail_entry($userId, $userId, 'cre_sys_obt_usr', 'exp_sp_my_profile', 'myprofile', 'Updated ',
                                         $userId, $prevValues->about_me, html_entity_decode($resultRecord['about_me'],ENT_QUOTES,UTF-8));
  }                                        
  //Auto cancellation for compliance course registration
  //$resultsEnr = getComplianceCourseEnrollAutoCancel($user_id); Comment the funtion before user change the job role and auto cancelling the compliance courses
  }catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('updateProfile', $ex);
    expertusErrorThrow($ex);
  }
  unset($txn);  
}

/*
 *  Delete Profile picture
 */
function delete_upload_images($form, &$form_state){
  try{
    global $user;
   expDebug::dPrint('TEstsss cvalue calling',5);
   $oldPhotoFileId = $user->picture;
   $txn = db_transaction();  
   $userId = $user->uid;
    if(!empty($userId)){
    $account = user_load($userId, TRUE);
    file_usage_delete($account->picture, 'user', 'user', $userId);
    // Remove file reference from users table record
    $account = user_save($account, array('picture' => 0));
    // If any module has implemented hook_user_picture_updated invoke it
    foreach (module_implements('user_picture_updated') as $module) {
      $function = $module . '_user_picture_updated';
      expDebug::dPrint('$function = ' . $function, 4);
      $function($userId);
    }
    $Id = getIdOfLoggedInUser();
    add_audit_trail_entry($userId, $userId, 'cre_sys_obt_usr', 'exp_sp_my_account', 'myaccount', "Removed Picture", $userId, $oldPhotoFileId, 0);
    //expDebug::dPrint('User id id value' .$userId.'drupal'.$drupalUserId , 4);
    $form_state['rebuild'] = TRUE;
    } 
  }catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('delete_upload_images', $ex);
    throw $ex;
  } 
  unset($txn);

}

/*
 * addTextAreaFieldToAddEditForm() -  Use this function to add a text area to the addedit form
 */
function addProfileTextAreaFieldToAddEditForm(&$form, $id, $title = '', $defaultValue = '', $tabIndex = null, $mandatory = false, $readOnly = false) {
 try{
  $attributes = array();
  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // Add class to apply common style to all text area fields.
  $attributes['class'][] = 'addedit-edit-textarea';

  if ($readOnly) {
    $attributes['class'][] = 'addedit-readonly-textfield';
  }

  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
  }

  // Create the form element
  $form[$id] = array(
    '#type' => 'textarea',
    '#title' => $title,
    '#title_display' => 'invisible',
    '#default_value' => $defaultValue,
    '#disabled' => $readOnly,
    '#attributes' => $attributes,
    '#rows'=>'3',
    '#cols'=>'40' ,

  );
  }catch (Exception $ex) {
  	watchdog_exception('addProfileTextAreaFieldToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}

?>
