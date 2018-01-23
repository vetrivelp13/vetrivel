<?php 
function getMySkillDetails($usid,$countrequired=false,$skillId='',$certdet= '',$fromapi= '',$type = '',$delete) {
 try{
 	expDebug::dPrint('getMySkillDetails sasa=== ' . print_r($fromapi, true) , 3);
 	expDebug::dPrint('getMySkillDetails sssseeee = ' . print_r($type, true) , 3);
    $loggedUserId = getIdOfLoggedInUser();
    $select = db_select("slt_skill_set","skill");
	//$select->innerJoin('slt_profile_list_items', 'prof1', 'prof1.code = skill.proficiency');
	if(empty($fromapi) || ($type == "skill" && $fromapi == 1)){
	    if($delete){
	        $select->leftjoin('slt_program','prog','skill.program_id = prog.id');
	        $select->addField('skill','id','skill_id');
	        $select->addField('skill','skill_name','skill_name');
	        $select->addField('skill','proficiency','proficiency');
	        $select->addField('skill','display_to_others','display_to_others');
	        $select->condition('skill.user_id',$usid,'=');
	        $select->addField('skill','program_id','programid');
	        $select->addField('skill','master_enrollment_id','masterEnrollId');
	        $select->addField('prog','title','title');
	    }
	    else{
		$select->leftjoin('slt_program','prog','skill.program_id = prog.id'); // For getting updated TP title. ticket: 0023049 //    
		$select->addField('skill','id','skill_id');
		$select->addField('skill','skill_name','skill_name');
		$select->addField('skill','proficiency','proficiency');
		$select->addField('skill','display_to_others','display_to_others');
		$select->condition('skill.user_id',$usid,'=');
		$select->addField('skill','program_id','programid');     //change for the ticket 0020909 = For updating tp in skills//
		$select->addField('skill','master_enrollment_id','masterEnrollId');
    	$select->condition('skill.is_deleted', '', 'IS NULL');
    	$select->addField('prog','title','title');
	}
	}
//	if($fromapi=''){
		if(empty($fromapi) || ($type == "certificate" && $fromapi == 1)){
			
			expDebug::dPrint('getMySkillDetails sajjjj= enteredddd ' . print_r($type, true) , 3);
			
	    $select->addField('skill','type','type');
	    $select->leftjoin('slt_person','per','per.id =skill.user_id');
	    $select->addField('per','full_name','full_name');
	    $select->addField('skill','display_to_others','display_to_others');
	    //DATE_FORMAT(($select->addField('skill','valid_from','valid_from')),"%m-%d-%Y");
	    $select->addExpression("CONCAT(DATE_FORMAT(skill.valid_from, _utf8'%Y-%m-%d'))", 'valid_from');
	    //$select->addField('skill','valid_to','valid_to');
	    $select->addExpression("CONCAT(DATE_FORMAT(skill.valid_to, _utf8'%Y-%m-%d'))", 'valid_to');
	    $select->addField('skill','company','company');
	    $select->addField('skill','certificate_type','certificate_type');
	    $select->addField('skill','certificate_number','certificate_number');
	    $select->addField('skill','verification_status','verification_status');
	    $select->addField('skill','created_on','created_on');
	    $select->addField('skill','updated_by','updated_by');
		}
		if(empty($fromapi)){
	    $select->addField('skill','certificate_img','certificate_img');
	    $select->addField('skill','user_name','user_name');
	    $select->addField('skill','email','email');
	    $select->addField('skill','mobile_no','mobile_no');
	    $select->addField('skill','created_on','created_on');
		}
	//}
    if($_REQUEST["apiname"] == "LearnerProfileAPI")
    {
    
      $userid = $_REQUEST["userid"];
      $learnerid = $_REQUEST["learnerid"];

         if($userid != $learnerid)
         {
             $select->condition('skill.display_to_others', array('Yes'), 'IN');
         }
   }
    else
    {
         if($loggedUserId != $usid && $certdet!=1)
         {
            $select->condition('skill.display_to_others', array('Yes'), 'IN');
         }
    }
    $select->orderBy('skill.id','desc');
	if(!empty($skillId)){
	$select->condition('skill.id',$skillId,'=');  
	}
   if($countrequired == true){
    $skillDetails = count($select->execute()->fetchAll());
  }else{
 //   $select->range($start, $limit);
    $skillDetails = $select;
  }
  expDebug::dPrintDBAPI("Db_PrintAPI---", $select);
  expDebug::dPrint('$skillId : obj = ' . print_r($select->programid, true) , 3);
  expDebug::dPrint('getMySkillDetails : obj = ' . print_r($skillDetails, true) , 3);
  
  return $skillDetails;
  }catch (Exception $ex) {
    watchdog_exception('getMySkillDetails', $ex);
    expertusErrorThrow($ex);
  }
}

function add_new_skill_set_details($form, $form_state) {
  $txn = db_transaction();
	expDebug::dPrint('Runnnfinf the function ', 5);
	try{
		$userId = getIdOfLoggedInUser();
		$insertStmt = db_insert('slt_skill_set');
		$fields = array(
				'skill_name'           => $form_state['values']['skill_name'],
				'proficiency'          => $form_state['values']['proficiency'],
				'display_to_others'    => ($form_state['values']['display_to_others'] ==1) ? 'Yes' : 'No',
				'user_id'              => $userId,     //change for the ticket 0020909 = For updating tp in skills//
		    'created_by'           => $userId,
				'type'            => 'skill',
				'created_on'           => now(),
				'updated_by'           => $userId,
				'updated_on'           => now()
		    
		);
		$insertStmt->fields($fields);
		$skillId = $insertStmt->execute();
		expDebug::dPrint('add_new_skill_details : '.print_r($form_state['values'],true) , 5);
        
      $fieldsList = array('skill_name', 'display_to_others','proficiency','user_id');
      make_audit_trail_entries($fields,  $userId, null, $skillId, 'Inserted', 'cre_sys_obt_usr', 'exp_sp_my_skill', 'addnewskilldetails',
                                              $fieldsList,$userId);
      
      return $skillId;
                                               
	}catch (Exception $ex) {
		$txn->rollback();
		watchdog_exception('add_new_skill_details', $ex);
		throw $ex;
	}
	unset($txn);
}

function updateSkillSetDetails($form, $form_state) {
    $skillFieldId = $form_state['entity_id'];
    expDebug::dPrint('Runnnfinf the function '.$skillFieldId.print_r($form_state,true), 5);
    $fieldsList = array('skill_name', 'display_to_others','proficiency','user_id');
    $prevValues = fetchAddEditSkillSetFieldRecords($skillFieldId, $fieldsList);
	$txn = db_transaction();
	try{
		$userId = getIdOfLoggedInUser();
		$updateStmt = db_update('slt_skill_set');
		$fields = array(
				'skill_name'           => $form_state['values']['skill_name'],
				'proficiency'          => $form_state['values']['proficiency'],
				'display_to_others'    => ($form_state['values']['display_to_others'] ==1) ? 'Yes' : 'No',
				'user_id'              => $userId,
		    'updated_by'           => $userId,
				'updated_on'           => now()
		);
		$updateStmt->fields($fields);
		$updateStmt->condition('id', $skillFieldId);
		$updateStmt->execute();
		expDebug::dPrint('updateSkillDetails : '.print_r($form_state['values'],true) , 5);
      
      make_audit_trail_entries($fields, $userId, $prevValues, $skillFieldId, 'Updated', 'cre_sys_obt_usr', 'exp_sp_my_skill', 'updateskilldetails',
                                          $fieldsList,$userId);                                    
	}catch (Exception $ex) {
		$txn->rollback();
		watchdog_exception('updateSkillDetails', $ex);
		throw $ex;
	}
	unset($txn);
}

/*
 * fetchAddEditSkillFieldRecords
 */
function fetchAddEditSkillSetFieldRecords($skillFieldId, $fieldsList) {
  try{
  $select = db_select('slt_skill_set', 'sfields');
  $select->condition('sfields.id', $skillFieldId, '=');
  foreach ($fieldsList as $field) {
    $select->addField('sfields', $field, $field);
  }
  expDebug::dPrintDBAPI(' $select obj = ' ,$select);  
  $result = $select->execute()->fetchAll(); 
  expDebug::dPrint(' $result1231 = ' . print_r($result, true),3);
  
  return $result;
  }catch (Exception $ex) {
  	watchdog_exception('fetchAddEditSkillFieldRecords', $ex);
  	throw $ex;
  }
}
function delete_skills_process($skillId){
	try{
  $updatedUserId = getSltpersonUserId();
  $select = db_update('slt_skill_set');
  $select->fields(array('is_deleted'     => '1'));     //change for the ticket 0020909 = For updating tp in skills//
  $select->condition('id', $skillId, '=');
  $select->condition('user_id', $updatedUserId, '=');
  $result = $select->execute();
  
  $fieldsList = array('skill_name','type');
  
  $prevValues = fetchAddEditSkillSetFieldRecords($skillId, $fieldsList);
  $fields = $prevValues;

  
  $fieldsList = array('skill_name');
  if($prevValues[0]->type == "certificate"){
      $fieldsList = array('certificate_name');       
      $prevValues_cert[0]->certificate_name = $prevValues[0]->skill_name;
      $prevValues_cert[0]->type = $prevValues[0]->type;
      $fields_cert[0]->certificate_name = $fields[0]->skill_name;
      $fields_cert[0]->type = $fields[0]->type;
      expDebug::dPrint(' $result123444441 = ' . print_r($fields_cert, true),3);
      make_audit_trail_entries($fields_cert, $updatedUserId, $prevValues_cert, $skillId, 'Deleted', 'cre_sys_obt_usr', 'exp_sp_my_skill', 'deletecertificatedetails',
              $fieldsList,$updatedUserId);
      
      
      
      expDebug::dPrint('delete skillprocess  $fields name: '.print_r($fields,true) , 5);
      $userId = getIdOfLoggedInUser();
      
      $skill_name = $fields[0]->skill_name;
      expDebug::dPrint('delete skillprocess  $skill_name name: '.print_r($skill_name,true) , 5);
      
      $qry1 = "select user_name from slt_skill_set where skill_name = '".$skill_name."' ";
      expDebug::dPrint('delete skillprocess  manager $qry1 name: '.print_r($qry1,true) , 5);
      
      $mgruserName = db_query($qry1)->fetchField();
      $qry = "select id from slt_person where user_name= '".$mgruserName."' order by id desc";
      expDebug::dPrint('delete skillprocess  manager name: '.print_r($qry,true) , 5);
      $verifier_id = db_query($qry)->fetchField();
      
      insertNotification('DeleteCertificateByLearner', $skillId, $verifier_id, $userId, $userId);
  }
  else{
  make_audit_trail_entries($fields, $updatedUserId, $prevValues, $skillId, 'Deleted', 'cre_sys_obt_usr', 'exp_sp_my_skill', 'deleteskilldetails',
                                          $fieldsList,$updatedUserId);        
  }                            
  }catch (Exception $ex) {
  	watchdog_exception('fetchAddEditSkillFieldRecords', $ex);
  	throw $ex;
  }                                   
} 
function getUsernameAutocomp(){
	try{
		$userId = getSltpersonUserId();
		$username = $_GET['z'];
		$userStatus = 'cre_usr_sts_atv';
		$select = db_select('slt_person', 'per');
		$select->addField('per', 'user_name');
		//$select->condition('per.id',$userId,'=');
		$select->condition('per.user_name', '%' . db_like($username) . '%', 'LIKE');
		$select->condition('per.status',$userStatus,'=');
	
		$select->condition('per.id',$userId,'!=');
		
		$select->groupBy('per.id');
		expDebug::dPrintDBAPI("Get User id:-",$select);
		$qry_user_autocomplete = $select->execute()->fetchAll();
		expDebug::dPrint(' $result saaaaa= ' . print_r($qry_user_autocomplete, true),3);
		foreach ($qry_user_autocomplete as $rec_user_autocomplete) {
			print $rec_user_autocomplete->user_name."\n";
		}
	}catch (Exception $ex) {
		watchdog_exception('getUsernameAutocomp', $ex);
		expertusErrorThrow($ex);
	}
}
function getmgrUsername(){
	try{
		$userId = getSltpersonUserId();
		$username = $_GET['z'];
		$userStatus = 'cre_usr_sts_atv';
		$select = db_select('slt_person', 'per');
		$select->leftJoin('slt_person_other_manager', 'pom','pom.user_id = per.id');
		$select->leftJoin('slt_person', 'persn','persn.id = pom.manager_id');
		$select->addField('persn', 'user_name');
		$select->condition('per.id',$userId,'=');
		$select->condition('persn.status',$userStatus,'=');
		$select->condition('pom.is_direct','Y','=');
		//$select->condition('per.user_name', '%' . db_like($username) . '%', 'LIKE');
		$select->groupBy('per.id');
		expDebug::dPrintDBAPI("Get User id:-",$select);
		$qry_user_autocomplete = $select->execute()->fetchField();
		return $qry_user_autocomplete;
	}catch (Exception $ex) {
		watchdog_exception('getmgrUsernameAutocomp', $ex);
		expertusErrorThrow($ex);
	}
}

function myCertificateDeleteImg($certId){
    try{
     
        
        $updatestmt =  db_update('slt_skill_set');
        $updatestmt->fields(array(
               'certificate_img' => NULL,
		        'updated_on' => NOW()));
        $updatestmt->condition('id', $certId);
        expDebug::dPrintDBAPI("Get User id sandhyyyyyaaaaaa:-",$updatestmt);
        $updatestmt->execute();
  
    }catch (Exception $ex) {
        watchdog_exception('myCertificateDeleteImg', $ex);
        expertusErrorThrow($ex);
    }
}

?>
