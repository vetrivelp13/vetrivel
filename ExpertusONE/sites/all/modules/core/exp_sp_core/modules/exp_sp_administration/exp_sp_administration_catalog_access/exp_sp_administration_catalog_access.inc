<?php

/*
 * addEditRepaintFormMulti() - 
 */
function addEditRepaintFormMulti($form, &$form_state) {
 try{
  $commands = array();
 	// Show the status messages, if any.
  // Ensure the status messages are auto removed after a brief while
  $statusMsgHTML = theme('status_messages');
  expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true) , 4);
  if (!empty($statusMsgHTML)) {
  	// Show the messages
    //$commands[] = ajax_command_prepend(NULL, $statusMsgHTML);
    $commands[] = ajax_command_html('#paintContentVisiblePopup #show_expertus_message', $statusMsgHTML);
  }  $commands[] = ajax_command_replace(NULL, drupal_render($form));
  //$commands[] = ajax_command_prepend(NULL, theme('status_messages'));  
  expDebug::dPrint(' $commands = ' . print_r($commands, true),4);

  return array('#type' => 'ajax', '#commands' => $commands);
  }catch (Exception $ex) {
  	watchdog_exception('addEditRepaintFormMulti', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * newAddEditFormMulti() -
 */
function newAddEditFormMulti($miscAction = array(),$ajaxCall = false) {
  try{
  $form = array();
  $form['#prefix'] = /* (( stripos($_SERVER['HTTP_USER_AGENT'], 'Chrome') || stripos($_SERVER['HTTP_USER_AGENT'], 'Firefox') )? '</form>' : '' ) */'</form>' .'<div id="addedit-form-wrapper" class="addedit-form-wrapper access-mro-visibility">';
  $form['#suffix'] = '</div>';

  // Add a class to the form so that we can apply custom style (margin, padding, etc).
  $form['#attributes'] = array('class' => array('addedit-form-wrapper'));
  $form['#tree'] = TRUE; // Show full path of values (hierarchical values)

  $form['addedit-left-col-start'] = array(
    '#type' => 'markup',
    '#markup' => '<div><div class="addedit-left-col mro-wrapper">',
  );
      
  return $form;
  }catch (Exception $ex) {
  	watchdog_exception('newAddEditFormMulti', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * addEditFormMultiClose()
 */
function addEditFormMultiClose(&$form, $uniqueId,$entityId,$popupId='') {
  try{
  $cancelButtonId                      = 'mroCancelbtn';
  $wrapper                             = 'addedit-form-wrapper';
  $ajaxCallbackCancel                  = 'addEditCancelRepaintForm';
  $cancelAttributes['class']           = array('addedit-edit-' . $cancelButtonId);
  $cancelAttributes['class'][]         = 'admin-action-button-middle-bg white-btn-bg-middle';
  $cancelAttributes['class'][]         = 'addedit-form-expertusone-throbber';
  $cancelAttributes['data-wrapperid']  = array($wrapper);
  $cancelAttributes['class'][]         = 'exp-addedit-form-cancel-button';
  $cancelAttributes['class'][]         = 'admin-bubble-close';
  $destroyId = 'qtip_visible_disp_'.$uniqueId;
  $removeId = 'qtipAccessqtip_visible_disp_'.$uniqueId;
  //$("#'.$destroyId.'").qtip("destroy");
   $cancelAttributes['onclick']   = 'closeQtip(\'\','.$entityId.',$("#root-admin").data("accessgroup").accessClose); $("#'.$removeId.'").closest(".qtip-active").hide();$(\'#'.$removeId.'_disp\').html(\'\'); return false;';
  
  $form[$cancelButtonId] = array(
    '#type' => 'submit',
    '#prefix' => '<div class="addedit-form-cancel-and-save-actions-row">' .
                   '<div class="addedit-form-cancel-container-actions resize-save_btn"><div class="white-btn-bg-left"></div>',
    '#suffix' =>   '<div class="white-btn-bg-right"></div>',
    '#value' => t('LBL123'),
    '#attributes' => $cancelAttributes,
  );
  
  $ajaxCallback = 'addEditRepaintFormMulti';
  
  $id = 'mrosubmit'.'-'.$uniqueId;
  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-' . $id);

  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $attributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  $attributes['data-wrapperid'] = array($wrapper);

  $form[$id] = array(
    '#type' => 'submit',
    '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
    '#suffix' =>
               '<div class="admin-save-button-right-bg"></div></div>' .
               '<div class="clearBoth"></div></div>' .
             '<div class="clearBoth"></div></div></div>',
    '#value' => t('LBL141'),
    '#ajax' => array(
	      'wrapper' => $wrapper,//$wrapperId,
	      'callback' => $ajaxCallback,
	      'method' => 'replace',
	      'effect' => 'none', // 'fade',
	      'event'=>'click',
	      'keypress' => true,
	      'progress' => array(
	        'type' => 'throbber',
	        'message' => ''
	      ),
    	),
    '#disabled' => false,
    '#attributes' => $attributes,
  );

  $form['action_buttons'] = array(
    '#type' => 'markup',
    '#markup' => empty($miscAction)? '' : theme('addedit-misc-actions', $miscAction),
  );

  $form['addedit-left-col-end'] = array(
    '#type' => 'markup',
    '#markup' => '</div><div class="clearBoth"></div>',
  );
  }catch (Exception $ex) {
  	watchdog_exception('addEditFormMultiClose', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * To get the default values while edit catalog/MRO access form
 */
function getDefaultCatAccessAddEditFormFieldValue($field, $form_state, $cataloAccessDetails) {
  try{
  $defaultValue 	= '';

  // $cataloAccessDetails
  $defaultValue = isset($cataloAccessDetails->$field) ? $cataloAccessDetails->$field : '';

   switch ($field) {
   	case "user_type":
    case "org_id":
    case "job_role":
      $defaultValue = isset($cataloAccessDetails->$field) ? explode(",",$cataloAccessDetails->$field) : '';
      break;
    case "job_role_id":
      $defaultValue = groupMroLst($cataloAccessDetails); //isset($cataloAccessDetails->$field) ? explode(",",$cataloAccessDetails->$field) : '';
      break;
    default:
      $defaultValue = isset($cataloAccessDetails->$field) ? $cataloAccessDetails->$field : '';
      break;
  }

  return $defaultValue;
  }catch (Exception $ex) {
  	watchdog_exception('getDefaultCatAccessAddEditFormFieldValue', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * To prepare MRO group list
 */
function groupMroLst($cataloAccessDetails) {
	try{

  $retMrLst = array();
  foreach($cataloAccessDetails as $getMrRoleLst) {
	$retMrLst[] =  $getMrRoleLst->job_role_id;
  }
  return $retMrLst;
  }catch (Exception $ex) {
  	watchdog_exception('groupMroLst', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * To store catalog access details from the form
 */
function addCatalogRoleAccess($form, &$form_state, $callFromAPI =0) {
	try{
  global $user;
  if($callFromAPI){
    $creatorId = $form_state['values']['creator_user_id'];
    $is_manager = $form_state['values']['is_manager'];
  }
  else{
    $creatorId = getIdOfLoggedInUser();
    $is_manager = $form_state['values']['manager']['fld'];
  }
  
  $txn = db_transaction();

  try {
    $insertStmt = db_insert('slt_catalog_access');

	// Find and remove duplicate value
	$user_type 	=  array_unique($form_state['values']['user_type']['fld']);
	arsort($user_type);
	if(in_array('0',$user_type)) {
		array_shift($user_type);
	}

    $fields = array(
              'entity_id' => $form_state['values']['entity_id'],
              'entity_type' => $form_state['values']['entity_type'],
              'user_type' => implode(",",$user_type),
              'is_manager' => $is_manager,
              'org_id' => implode(",",array_unique($form_state['values']['fldorganization'])),
              'job_role' => implode(",",array_unique($form_state['values']['fldroles'])),
              'created_by' => $creatorId,
              'created_on' => now(),
    					'updated_by' => $creatorId,
              'updated_on' => now()
            );
  
    // Select the fields and the values to be inserted
    $insertStmt->fields($fields);

    // Execute the insert statement. Fetch the new org id.
    $catAccessId = $insertStmt->execute();
    
   
    make_audit_trail_entries($fields, $creatorId , null, $form_state['values']['entity_id'] ,'Inserted',$form_state['values']['entity_type'],'exp_sp_administration_catalog_access','addCatalogRoleAccess',
                                          array('user_type', 'is_manager', 'org_id', 'job_role'));
    	   
     expDebug::dPrint(' addnewcatalog formstatevaluesUpdate : '.print_r($form_state['values'],true), 5);
     if($callFromAPI){
       return array((object)array('id'=>$catAccessId));
     }
     //return $catAccessId;
  } catch (Exception $ex) {
    watchdog_exception('catalog Access', $ex);
    if($callFromAPI){
//       return array((object)array('id'=>'Failure'));
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Catalog access role id is invalid.";
    	return $errobj;
    }
    else {
    	return array((object)array('id'=>'Failure'));
    }
    $txn->rollback();
    unset($txn);
    watchdog_exception('CatalogAccess', $ex);
    expertusErrorThrow($ex);
  }
  unset($txn);
  }catch (Exception $ex) {
  	watchdog_exception('addCatalogRoleAccess', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * To update catalog access detail information
 */
function updateCatalogRoleAccess($form, &$form_state) {
	
  global $user;
      
  $txn = db_transaction();

  try {
    
    $entityId = $form_state['values']['entity_id'];
    $entityType = $form_state['values']['entity_type'];
    
    // Getting Job Role before update
    $catalogRoleAccessValues = getCatalogRoleAccess($entityId, $entityType);
    $jobRoleBeforeUpdate = $catalogRoleAccessValues[0]->job_role;
    
    // Select table to update
    $updateStmt = db_update('slt_catalog_access');
    
    $default= db_query("SELECT id,user_type,is_manager,org_id,job_role from slt_catalog_access where id=:uid",array(":uid"=>$catalogId))->fetchAll();
    // Set conditions
    $updateStmt->condition('id', $form_state['values']['id']);
    $updateStmt->condition('entity_id', $form_state['values']['entity_id']);
    $updateStmt->condition('entity_type', $form_state['values']['entity_type']);

	// Find and remove duplicate value
    $user_type 	=  array_unique($form_state['values']['user_type']['fld']);
	arsort($user_type);
	
	if(in_array('0',$user_type)) {
		array_shift($user_type);
	}
	//$user_type = in_array(0,$user_type) ? array_shift($user_type) : $user_type;
	
	$fldorganization = isset($_POST["fldorganization"]) ? $form_state['values']['fldorganization'] : '';
	$fldroles 		 = isset($_POST["fldroles"]) ? $form_state['values']['fldroles'] : '';
    
    $fields = array(
      'user_type' => implode(",",$user_type),
      'is_manager' => $form_state['values']['manager']['fld'],
      'org_id' => implode(",",$fldorganization),
      'job_role' => implode(",",$fldroles),
      'updated_by' => $user->uid,
      'updated_on' => now()
    );

    // Select the fields and the values to be updated
    $updateStmt->fields($fields);
      
    // Execute query
    $numUpdated = $updateStmt->execute();
   
    make_audit_trail_entries($fields, getIdOfLoggedInUser() ,$default, $entityId ,'Updated',$entityType,'exp_sp_administration_catalog_access','addCatalogRoleAccess',
                                          array('user_type', 'is_manager', 'org_id', 'job_role'));
    	   
    
    // Getting Job Role after update
    $catalogRoleAccessValues = getCatalogRoleAccess($entityId, $entityType);
    $jobRoleAfterUpdate = $catalogRoleAccessValues[0]->job_role;
    $jobRoleBeforeUpdateArray = explode(',', $jobRoleBeforeUpdate);
    $jobRoleAfterUpdateArray = explode(',', $jobRoleAfterUpdate);
    $jobRoleDeleted = array_diff($jobRoleBeforeUpdateArray, $jobRoleAfterUpdateArray);
    
    expDebug::dPrint(' JobRoleBeforeUpdate : '.$jobRoleBeforeUpdate,4);
    expDebug::dPrint(' JobRoleAfterUpdate : '.$jobRoleAfterUpdate, 4);
    expDebug::dPrint(' Difference : '.print_r($jobRoleDeleted, true), 4);

    // Delete the entry in mro table if the job role removed in catalog access
    /* if(count($jobRoleDeleted) > 0){
      foreach($jobRoleDeleted as $mroFlag){
        delMrRoleAccessForCatalogDel($entityId, $entityType, $mroFlag);
      }
    } */
    
  } catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('catalogAccess', $ex);
    expertusErrorThrow($ex); 
  }
  unset($txn);
    
}

/*
 * To delete MRO option for particular catalog type if the job role removed from catalog access
 */
function delMrRoleAccessForCatalogDel($entity_id,$entity_type, $job_role) {
  try{
    $updateStmt = db_delete('slt_mro_mapping');
    // Set conditions
    $updateStmt->condition('entity_id', $entity_id);
    $updateStmt->condition('entity_type', $entity_type);
    $updateStmt->condition('job_role_id', $job_role);
    $numUpdated = $updateStmt->execute();
  }catch (Exception $ex) {
    watchdog_exception('delMrRoleAccessForCatalogDel', $ex);
    expertusErrorThrow($ex);
  } 
}

/*
 * To Get catalog access details for particular catalog type
 */
function getCatalogRoleAccess($entity_id,$entity_type, $emptyId) {
  try{
    $select = db_select("slt_catalog_access","cata");
    $select->addField('cata','id','id');
    $select->addField('cata','entity_id','entity_id');
    $select->addField('cata','entity_type','entity_type');
    $select->addField('cata', 'user_type','user_type');
    $select->addField('cata','is_manager','is_manager');
    $select->addField('cata','org_id','org_id');
    $select->addField('cata','job_role','job_role');
    if($entity_id == 0){
      $select->condition('cata.custom0',$emptyId,'=');
    } else {
      $select->condition('cata.entity_id',$entity_id,'=');
    }
    $select->condition('cata.entity_type',$entity_type,'=');
  
    $orgNameList = $select->execute()->fetchAll();
    	
    return $orgNameList;
   }catch (Exception $ex) {
    watchdog_exception('getCatalogRoleAccess', $ex);
    expertusErrorThrow($ex);
  } 
}

/*
 * To Get MRO role access for particular  catalog type
 */
function getMrRoleAccess($entity_id,$entity_type,$emptyId, $mro_flag = 0) {
  try {
    $select = db_select("slt_mro_mapping","mro");
    $select->addField('mro','id','id');
    $select->addField('mro','entity_id','entity_id');
    $select->addField('mro','entity_type','entity_type');
    $select->addField('mro', 'job_role_id','job_role_id');
    $select->addField('mro','mro_flag','mro_flag');
    if($entity_id == 0){
      $select->condition('mro.custom0',$emptyId,'=');
    } else {
      $select->condition('mro.entity_id',$entity_id,'=');
    }
    $select->condition('mro.entity_type',$entity_type,'=');
    
    if($mro_flag) {
    	$select->condition('mro.mro_flag',$mro_flag,'=');
    }  
    $orgNameList = $select->execute()->fetchAll();
    	
    return $orgNameList;
  }catch (Exception $ex) {
    watchdog_exception('getMrRoleAccess', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * To delete and add catalog access option for particular catalog type
 */
function addMrRoleAccess($form, &$form_state, $callFromAPI=0) {
  global $user;
  try {
    if($callFromAPI){
      $creatorId = $form_state['values']['creator_user_id'];
    }
    else{
      $creatorId = getIdOfLoggedInUser();
    }  
  
    // Find and remove duplicate value
    $job_role_ids 	=  isset($_POST["fldroles"]) ? $form_state['values']['fldroles'] : '';
    $entity_id 		= $form_state['values']['entity_id'];
    $entity_type 	    = $form_state['values']['entity_type'];
    $mro_flag		    = $form_state['values']['mro_flag']['fld'];

    // Find the MRO records
    $emptyId = 0;
    $getMrList 	    = getMrRoleAccess($entity_id,$entity_type, $emptyId, $mro_flag);	
    $retMrLst 	    = array();
    	
    foreach($getMrList as $getMrLst) {  		
    	$retMrLst[] =  $getMrLst->job_role_id;
    }
    
    //Delete MRO option While do Add/Edit  
    //delMrRoleAccess($entity_id,$entity_type,$mro_flag);
    $rollIds = array();
    for($i=0;$i<count($job_role_ids);$i++) {  	
      $job_role_id 	= $job_role_ids[$i];  	
    	$rollIds[] = addMrRoleAccessIns($entity_id,$entity_type,$mro_flag,$job_role_id,$creatorId);
    }
    if($callFromAPI){
      return $rollIds;
    }
  }catch (Exception $ex) {
    watchdog_exception('addMrRoleAccess', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * To delete MRO option for particular catalog type
 */
function delMrRoleAccess($emptyId,$entity_type) {
  
  try{
  	expDebug::dPrint(' emptyId '.$emptyId.' entity type '.$entity_type ,4);
    $updateStmt = db_delete('slt_group_mapping');
    
    // Set conditions

    $updateStmt->condition('entity_id', 0);
    if(!empty($entity_type))
    	$updateStmt->condition('entity_type', $entity_type);  
    $updateStmt->condition('custom0', $emptyId);
    expdebug::dPrintDBAPI('delete group mapping',$updateStmt);
    $numUpdated = $updateStmt->execute();
 }catch (Exception $ex) {
    watchdog_exception('delMrRoleAccess', $ex);
    expertusErrorThrow($ex);
  } 
    
}

/*
 * To add MRO option for particular catalog type
 */
function addMrRoleAccessIns($entity_id,$entity_type,$mro_flag,$job_role_id,$creatorId) {
  global $user;
  $txn = db_transaction();
  try{
    $insertStmt = db_insert('slt_mro_mapping');   
    $fields     = array(
                'entity_id' => $entity_id,
                'entity_type' => $entity_type,
    			  		'mro_flag' => $mro_flag,
                'job_role_id' => $job_role_id,
                'created_by' => $creatorId,
                'created_on' => now(),
    						'updated_by' => $creatorId,
    						'updated_on' => now()
              );
    
    // Select the fields and the values to be inserted
    $insertStmt->fields($fields);
        
    // Execute the insert statement. Fetch the new org id. 
    $catAccessId = $insertStmt->execute();
    return $catAccessId;
 }catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('addMrRoleAccessIns', $ex);
    expertusErrorThrow($ex);
  }
  // Commit the transaction
  unset($txn);    
}

/*
 * To update MRO role access
 */
function updateMrRoleAccess($entity_id,$entity_type,$mro_flag,$job_role_id) {
  global $user;
      
  $txn = db_transaction();

  try {
    // Select table to update
    $updateStmt = db_update('slt_mro_mapping');
          
    // Set conditions
    //$updateStmt->condition('id', $form_state['values']['id']);
    $updateStmt->condition('entity_id', $entity_id);
    $updateStmt->condition('entity_type', $entity_type);  
    $updateStmt->condition('mro_flag', $mro_flag);
    
    $fields = array(
                'entity_id' => $entity_id,
                'entity_type' => $entity_type,
    			'mro_flag' => $mro_flag,
                'job_role_id' => $job_role_id,
                'created_by' => $user->uid,
                'created_on' => now()
              );
     // Select the fields and the values to be updated
    $updateStmt->fields($fields);
      
    // Execute query
    $numUpdated = $updateStmt->execute();    
  } catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('updateMrRoleAccess', $ex);
    unset($txn);
    expertusErrorThrow($ex);
  }
  unset($txn);  
}


/*
 * To get organization list
 */
function getOrganizationLst($org_name='', $selectedId=array(), $type, $iscount='', $page){
  try {
  		$start = $page*15;
  		$select = db_select('slt_organization', 'org');
  		if($iscount=='Count'){
  			$select->addExpression('count(1)','count');
  		}else{
	      $select->addField('org', 'id','org_id');
  	    $select->addField('org', 'name','org_name');
  		}
      $select->condition('org.status', 'cre_org_sts_act');
      if(!empty($org_name)){
      	$select->condition('org.name', '%' . db_like($org_name) . '%', 'LIKE');
      }

      if(sizeOf($selectedId)>0){
      	if($type!='Include'){
      		$select->condition('org.id', $selectedId,'NOT IN');
      		$select->range($start,15);
      	}else{
      		$select->condition('org.id', $selectedId,'IN');
      	}
      }else{
      	$select->range($start,15);
      }
      
			expDebug::dPrintDBAPI("Access getOrganizationLst Query ",$select);
      if($iscount=='Count'){
      	$organization = $select->execute()->fetchField();
      }else{
      	$select->orderBy('org.name');
      	$organization = $select->execute()->fetchAll();
      }
      
     // $organizationarr = array();
    
      /*foreach($organization as $key=>$value) {
       if(!empty($value->org_name)) {
         $org_id 	= $value->org_id;
     	 $org_name 	= $value->org_name;
     	 $organizationarr[$org_id] = $org_name;
       }
     }  */
     return $organization;
   }catch (Exception $ex) {
    watchdog_exception('getOrganizationLst', $ex);
    expertusErrorThrow($ex);
  } 
}
/*
 * Get list of options for MRO 
 */
function getMROList($codePrefix) {
  try {
    $select = db_query('SELECT code,name FROM slt_profile_list_items WHERE code like \''.$codePrefix.'%\' AND is_active = \'Y\' ORDER BY FIELD(CODE,\'cre_sys_inv_man\',\'cre_sys_inv_rec\',\'cre_sys_inv_opt\')');
    $result = $select->fetchAllKeyed();
    return $result;
  }catch (Exception $ex) {
    watchdog_exception('getMROList', $ex);
    expertusErrorThrow($ex);
  } 
}

/*
 * To store catalog access details from the API
 */
function addCatalogRoleAccessByAPI( &$form_state,$createuserid="") {
  try {
    $form=array();
    $form["form_id"]="";
    drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
    $form_state['values']['creator_user_id'] = $createuserid;
    $form_state['values']['user_type']['fld'] = explode(',',$form_state['values']['user_type']['fld']);
    $form_state['values']['fldorganization'] = explode(',',$form_state['values']['fldorganization']);
    $form_state['values']['fldroles'] = explode(',',$form_state['values']['fldroles']);
    $rollAccessId =  addCatalogRoleAccess($form,$form_state,1);
    expDebug::dPrint(' addCatalogRoleAccessByAPI created $rollAccessId id:' . print_r($rollAccessId,true), 4);
    return $rollAccessId;
  }catch (Exception $ex) {
    watchdog_exception('addCatalogRoleAccessByAPI', $ex);
    expertusErrorThrow($ex);
  } 
}

/*
 * To store MRO access details from the API
 */
function addMrRoleAccessByAPI( &$form_state,$createuserid="") {
  try {
    $form=array();
    $form["form_id"]="";
    drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
    $form_state['values']['creator_user_id'] = $createuserid;
    $form_state['values']['fldroles'] = explode(',',$form_state['values']['fldroles']);
    $_POST["fldroles"] = $form_state['values']['fldroles'];
    $rollAccessId =  addMrRoleAccess($form,$form_state,1);
    expDebug::dPrint(' addMrRoleAccessByAPI created $rollAccessId id:' . print_r($rollAccessId,true), 4);
    if(count($rollAccessId)){
      return array((object)array('id'=>implode(',',$rollAccessId)));
    }
    else{
      $errobj=new stdClass();
   	  $errobj->isValidateError = 1;
      $errobj->errcode = 'L_012';
      $errobj->errormsg = "MRO access id is invalid";
      return $errobj;
   }
  } catch (Exception $ex) {
    watchdog_exception('addMrRoleAccessByAPI', $ex);
    expertusErrorThrow($ex);
  } 
}

/**
 * _updateAccessEntity()
 * @param Entity Id
 * @param Entity Type
 * @param Empty ID 
 */
function _updateAccessEntity($entityId, $entityType, $emptyId){
$txn = db_transaction();
  try {
    $fields = array();
    $fields['custom0'] = NULL;
    $fields['entity_id'] = $entityId;
    if($entityType == 'cre_sys_obt_trp' || $entityType == 'cre_sys_obt_cur' || $entityType == 'cre_sys_obt_crt') {
    	$fields['entity_type']	= $entityType;
    	$fields['parent_id']	= $entityId.'-0';
    	$fields['parent_type']	= 'program';
    } else if($entityType == 'cre_sys_obt_crs') {
    	$fields['parent_id']	= $entityId.'-0';
    	$fields['parent_type']	= 'learning';
    } else if($entityType == 'cre_sys_obt_cls') {
    	$course_id = getCourseIdFromClassId($entityId);
    	$fields['parent_id']	= $course_id.'-'.$entityId;
    	$fields['parent_type']	= 'learning';
    }

    // Update catalog access entry 
    $catalogUpdate = db_update('slt_group_mapping');
    $catalogUpdate->fields($fields);
    $catalogUpdate->condition('custom0', $emptyId);
    if($entityType != 'cre_sys_obt_trp' && $entityType != 'cre_sys_obt_cur' && $entityType != 'cre_sys_obt_crt')
    	$catalogUpdate->condition('entity_type', $entityType);
    expDebug::dPrintDBAPI('access update group', $catalogUpdate);
    $catalogUpdate->execute();
    
    $UserUpdate = db_update('slt_user_access_mapping');
    $UserUpdate->fields($fields);
    $UserUpdate->condition('custom0', $emptyId);
    if($entityType != 'cre_sys_obt_trp' && $entityType != 'cre_sys_obt_cur' && $entityType != 'cre_sys_obt_crt')
    	$UserUpdate->condition('entity_type', $entityType);
    expDebug::dPrintDBAPI('access update user', $UserUpdate);
    $UserUpdate->execute();
    
    $select = db_select('slt_group_mapping','grp_map');
    $select->addfield('grp_map','group_id');
    $select->addfield('grp_map','created_by');
    $select->addfield('grp_map','created_on');
    $select->addfield('grp_map','updated_by');
    $select->addfield('grp_map','updated_on');
    $select->condition('grp_map.entity_id',$entityId,'=');
    $select->condition('grp_map.entity_type',$entityType,'=');
    $select->condition('grp_map.group_type','1','=');
    $result = $select->execute()->fetchAll();
    expDebug::dPrint('$result-->'.print_r($result,true),4);
    if(count($result)>0 && !empty($result))
   		updateGroupMappingDetails($entityId, $entityType, $result[0]->created_by,'addupdate',$result);
    
  }catch (Exception $ex) {
    watchdog_exception('_updateAccessEntity', $ex);
    expertusErrorThrow($ex);
    $txn->rollback();
    unset($txn);
    expertusErrorThrow($ex);
  } 
  
}

/**
 * getProfileItemNamesKeyed() - Function to fetch all profile list item names keyed by code whose code is prefixed as $codePrefix.
 *                              The result is ordered by name. This function is used to build selection controls (e.g. dropdown and checkboxes)
 *                              for the new admin screen but can be used in other places as well.
 */
function getProfileItemNamesKeyedForAccessControl($codePrefix, $langCode = 'cre_sys_lng_eng', $name, $selectedId=array(), $type, $iscount='', $page) {
  try {
  		$start = $page*15;
  		$select = db_select('slt_profile_list_items','spli');
		  // Select the fields to fetch
		  if($iscount=='Count'){
		  	$select->addExpression('count(1)','count');
		  }else{
		  	$select->addField('spli','code', 'code');
		  	$select->addField('spli','name', 'name');
		  }
      $select->condition('spli.code', db_like($codePrefix) . '%', 'LIKE');
  		$select->condition(db_and()->condition('spli.lang_code', $langCode, '='));
      if(!empty($name)){
      	$select->condition('spli.name', '%'. db_like($name) . '%', 'LIKE');
      }

      if(sizeOf($selectedId)>0){
      	if($type!='Include'){
      		$select->condition('spli.code', $selectedId,'NOT IN');
      		$select->range($start,15);
      	}else{
      		$select->condition('spli.code', $selectedId,'IN');
      	}
      }else{
      	$select->range($start,15);
      }
      
			expDebug::dPrintDBAPI("Access getProfileItemNamesKeyedUserType Query ",$select);
      if($iscount=='Count'){
      	$result = $select->execute()->fetchField();
      }else{
      	$select->orderBy('spli.name');
      	$result = $select->execute()->fetchAllKeyed();
      }
     return $result;
   }catch (Exception $ex) {
    watchdog_exception('getProfileItemNamesKeyedForAccessControl', $ex);
    expertusErrorThrow($ex);
  } 
  
  
}
function getUserAccessDetails($uniqueId,$emptyId){
	try{
		expDebug::dPrint('autocomplete enter');
		$emptyId = escape_string($emptyId); // For Security Fix
		expDebug::dPrint('$uniqueId-->'.$uniqueId,4);
		$uniqueId = escape_string($uniqueId); // For Security Fix
		expDebug::dPrint('After string escape of $uniqueId-->'.$uniqueId,4);
		$getId = explode('-',$uniqueId);
		expDebug::dPrint('$getId-->'.print_r($getId,true),4);
		$entityId = (int)$getId[0]; // For Security Fix
		$entityType = escape_string($getId[1]); // For Security Fix
		$courseId = (int)$getId[2]; // For Security Fix
		expDebug::dPrint('entityId-->'.$entityId.'entityType--->'.$entityType,4);

		$searchType = $_GET['type'];
		$page = (int)$_GET['page'];
		$limit = (int)$_GET['rows'];
		$start = $limit * $page - $limit;
		expDebug::dPrint('limit-->'.$limit.'start--->'.$start,4);
		
		
		$nameSubstr = $_GET['z'];
		$userId = getIdOfLoggedInUser();
		
		if($entityType == 'cre_sys_obt_crt' || $entityType == 'cre_sys_obt_trn' || $entityType == 'cre_sys_obt_cur'){
			$page_code = 'cre_sys_obt_trp';
		}else if($entityType == 'cre_sys_obt_cls'){
			$page_code = 'cre_sys_obt_crs';
		}else if($entityType == 'cre_sec'){
			$grpType = db_select('slt_groups','grps');
			$grpType->addField("grps",'is_admin');
			$grpType->condition("grps.id",$entityId,'=');
			$resGrpType = $grpType->execute()->fetchField();
			
			if($resGrpType == 1)
				$page_code = $entityType;
			else
				$page_code = 'cre_sec_learner';
		}else{
			$page_code = $entityType;
		}
		
		$select = db_select('slt_person','per');
		$select->condition('per.id',array(1,2),'NOT IN');
		$select->condition('per.status','cre_usr_sts_atv','=');
		if(!empty($emptyId))
			$select->leftjoin('slt_user_access_mapping', 'grpmap', 'grpmap.user_id=per.id and grpmap.entity_id ='.$entityId.' and grpmap.entity_type =\''.$entityType.'\''. ' and grpmap.custom0 =\''.$emptyId.'\'');
		else 		
			$select->leftjoin('slt_user_access_mapping', 'grpmap', 'grpmap.user_id=per.id and grpmap.entity_id ='.$entityId.' and grpmap.entity_type =\''.$entityType.'\'');
		
		//$select->leftjoin('slt_group_privilege', 'priv', 'priv.group_id = grps.id and priv.page_code = \''.$page_code.'\'');
			
		$select->addField('per','id','opt');
		//$select->addField('per','full_name','val');
		$select->addExpression('CONCAT(per.full_name," (",per.user_name,")")','val');
		//$select->addExpression('concat("<table width=\"210px\"><tr><td width=\"110px\" class=\'access-fullname\'><div class=\'fade-out-title-container group-name-option-div user-name\'>",per.full_name,"<span class=\'fade-out-image\'></div></td><td width=\"100px\" class=\'access-username\'>", per.user_name,"</td></tr></table>")','dispVal');
		//$select->addExpression('concat("<table width=\"210px\"><tr><td width=\"110px\" class=\'access-fullname fade-out-title-container group-name-option-div user-name\'>",per.full_name,"<span class=\'fade-out-image\'></td><td width=\"100px\" class=\'access-username\'>", per.user_name,"</td></tr></table>")','dispVal');
		$select->addExpression('concat("<div style=\"max-width:205px\"><div style=\"min-width:105px;float:left;\" class=\'access-fullname fade-out-title-container user-name\'><span class=\'title-lengthy-text\'>",per.full_name,"</span><span class=\'fade-out-image-access\'></div><div style=\"min-width:100px;float:right;\" class=\'access-username\'>", per.user_name,"</div></div>")','dispVal');
		
		filterByUserAccessPrivileges($select,'per','',1,'cre_usr','','per'); 
		
		
		if($entityType == 'cre_sys_obt_cls' ){
		  $select->leftJoin('slt_course_class','cls','cls.id='.$entityId.'');
		  $select->addField('cls', 'is_compliance','is_compliance');
		} 
		if($entityType == 'cre_sys_obt_crs'){
		 $select->leftJoin('slt_course_template','crs','crs.id='.$entityId.'');
		 $select->addField('crs', 'is_compliance','is_compliance');
		}
		$select->addExpression('if(grpmap.id is not null,1,0)','sel');
		$select->addField('grpmap', 'mro','mro');
		$select->orderBy('sel','DESC');
		$select->orderBy('val');
		//$select->addField('grps', 'is_admin','dropdown');
		//$select->condition('grps.status', 'cre_sec_sts_atv');
		//$select->condition('grps.code', 'grp_sup', "!=");
		//$select->where("if(grps.is_admin = 1,(priv.group_id is not null or grpmap.group_id is not null),1=1)");
		$nameSubstr = empty($nameSubstr) ? '%' : '%'.$nameSubstr.'%';
		switch($searchType){
			case 'username':
				$select->condition('per.user_name', $nameSubstr, "LIKE");
				break;
			case 'name':
				$select->condition('per.full_name', $nameSubstr, "LIKE");
				break;
			case 'org':
				$select->leftJoin('slt_organization','org','per.org_id = org.id');
				$select->condition('org.name', $nameSubstr, "LIKE");
				break;
			case 'jobrole':
				$select->leftJoin('slt_person_jobrole_mapping','jr','per.id = jr.user_id');
				$select->leftJoin('slt_profile_list_items','plist','plist.code = jr.job_role');
				$select->condition('plist.name', $nameSubstr, "LIKE");
				break;
			case 'usertype': 
				$select->leftJoin('slt_profile_list_items','plist','plist.code = per.user_type');
				$select->condition('plist.name', $nameSubstr, "LIKE");
				break;
			case 'manager': 
				$select->condition('per.full_name', $nameSubstr, "LIKE");
				$select->condition('per.is_manager', 'Y', "=");
				break;
			case 'group':
				$select->leftJoin('slt_group_user_mapping','grpm','grpm.user_id = per.id and grpm.user_type !=\'R\' ');
				$select->leftJoin('slt_groups','grp','grpm.group_id = grp.id');
				$select->condition('grp.name', $nameSubstr, "LIKE");
				break;
		}
		
		$count = $select->countQuery()->execute()->fetchField();
		$select->range($start,$limit);
		expDebug::dPrintDBAPI("Access getGroupDetails Query ",$select);
		$result = $select->execute()->fetchAll();
		
		// Check compliance course and disable the options
		if ($entityType == 'cre_sys_obt_cls' && isset($courseId) ) {
		 $selectCrsCmp = db_select('slt_course_template','crs');
		 $selectCrsCmp->addField('crs','is_compliance');
		 $selectCrsCmp->condition('crs.id',$courseId,'=');
		 expDebug::dPrintDBAPI('$selectCrsCmp ' , $selectCrsCmp, 4);
		 $isComplianceCourse = $selectCrsCmp->execute()->fetchField();
		 //expDebug::dPrint('$isComplianceCourse' . $isComplianceCourse, 4);
		 if ($isComplianceCourse) {
    		  foreach($result as $res){
    		   $res->is_compliance = 1;
    		  }
		  }
		}
		
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

		expDebug::dPrint('$$result$$result$$result--'.print_r($result,true),4);
		foreach($result as $key=>$val){
			$result[$key]->sel = !empty($val->sel) ? "1" : "0";		
			$result[$key]->dropdown = ($val->dropdown == 1 || $val->is_compliance == 1)? 'disable' : 'enable';
		}	
		expDebug::dPrint('selllll--'.print_r($result,true),4);
		
		$output = array();
		$output['records'] = $count;
		$output['page'] = $page ;
		$output['data'] = $result;
		expDebug::dPrint('$output$output$output--'.print_r($output,true),4);
		echo drupal_json_output($output);
		expDebug::dPrint('after echo--'.print_r($output,true),4);
		
	}catch(Exception $ex){
		watchdog_exception('getGroupAutocomplete', $ex);
		expertusErrorThrow($ex);
	}
}

function getGroupAccessDetails($uniqueId,$emptyId){
	try{
	    $grpDisType = array();
		$grpDisType['cre_sys_obt_crs'] = 'both';
		$grpDisType['cre_sys_obt_cls'] = 'both';
		$grpDisType['cre_sys_obt_crt'] = 'both';
		$grpDisType['cre_sys_obt_cur'] = 'both';
		$grpDisType['cre_sys_obt_trn'] = 'both';
		$grpDisType['cre_usr'] = 'admin';
		$grpDisType['cre_org'] = 'admin';
		$grpDisType['cre_sec'] = 'admin';
		$grpDisType['sry_det_typ_sry'] = 'admin';
		$grpDisType['sry_det_typ_ass'] = 'admin';
		$grpDisType['sry_det_typ_sry_qus'] = 'admin';
		$grpDisType['sry_det_typ_ass_qus'] = 'admin';
		$grpDisType['cre_sys_obt_cnt'] = 'admin';
		$grpDisType['cbn_anm_typ_ban'] = 'both';
		$grpDisType['cre_ntn'] = 'admin';
		$grpDisType['cre_cer'] = 'admin';
		$grpDisType['cre_sys_obt_loc'] = 'admin';
		$grpDisType['cre_sys_obt_not'] = 'both';
		$grpDisType['cme_ord'] = 'admin';
		$grpDisType['cme_dis'] = 'admin'; 
		$grpDisType['cme_pmt'] = 'admin';
		$grpDisType['cre_usr_dpt'] = 'admin';
		$grpDisType['cre_usr_etp'] = 'admin';
		$grpDisType['cre_usr_jrl'] = 'admin';
		$grpDisType['cre_usr_jtl'] = 'admin';
		$grpDisType['cre_usr_ptp'] = 'admin'; 
		$grpDisType['cre_sys_obt_cnt_aut'] = 'admin';
		$grpDisType['cre_sys_obt_cnt_aut_pre'] = 'admin';
		$emptyId = escape_string($emptyId); // For Security Fix
		expDebug::dPrint('$uniqueId-->'.$uniqueId,4);
		$uniqueId = escape_string($uniqueId); // For Security Fix
		expDebug::dPrint('After string escape of $uniqueId-->'.$uniqueId,4);
		$getId = explode('-',$uniqueId);
		expDebug::dPrint('$getId-->'.print_r($getId,true),4);
		$entityId = (int)$getId[0]; // For Security Fix
		$entityType = escape_string($getId[1]); // For Security Fix
		$courseId = (int)$getId[2]; // For Security Fix
		expDebug::dPrint('entityId-->'.$entityId.'entityType--->'.$entityType,4);
		$searchText =  $_GET['z'];
		$searchType = $_GET['type'];
		$page = (int)$_GET['page'];
		$limit = (int)$_GET['rows'];
		$start = $limit * $page - $limit;
		expDebug::dPrint('limit-->'.$limit.'start--->'.$start,4);
		
		if($entityType == 'cre_sys_obt_crt' || $entityType == 'cre_sys_obt_trn' || $entityType == 'cre_sys_obt_cur'){
			$page_code = 'cre_sys_obt_trp';
		}else if($entityType == 'cre_sys_obt_cls'){
			$page_code = 'cre_sys_obt_crs';
		}else if($entityType == 'cre_sec'){
			$grpType = db_select('slt_groups','grps');
			$grpType->addField("grps",'is_admin');
			$grpType->condition("grps.id",$entityId,'=');
			$resGrpType = $grpType->execute()->fetchField();
			
			if($resGrpType == 1)
				$page_code = $entityType;
			else
				$page_code = 'cre_sec_learner';
		}else{
			$page_code = $entityType;
		}
		
		/* if($entityType == 'cre_sys_obt_cls'){
			$selectClass = db_select('slt_group_mapping','grp_map');
			$selectClass->addExpression('COUNT(1)','count');
			$selectClass->condition('grp_map.entity_id',$entityId,'=');
			$selectClass->condition('grp_map.entity_type',$entityType,'=');
			$clsMapCnt = $selectClass->execute()->fetchField();
			expDebug::dPrint('checking the results of the mapped class count'.$clsMapCnt,3);
		}
		if($entityType == 'cre_sys_obt_cls' && $clsMapCnt == 0  && $entityId !=0 ){ // Entity id Not Equal To zero issue is Fixed For this Ticket #0036447
			$selectCrsId = db_select('slt_course_class','cls');
			$selectCrsId->addField('cls','course_id');
			$selectCrsId->condition('cls.id',$entityId,'=');
			$courseId = $selectCrsId->execute()->fetchField();
			
			$select = db_select('slt_groups', 'grps');
			$select->leftjoin('slt_group_mapping', 'grpmap', 'grpmap.group_id=grps.id and grpmap.entity_id ='.$courseId.' and grpmap.entity_type =\'cre_sys_obt_crs\'');
		  $select->leftJoin('slt_course_class','cls','cls.id='.$entityId.'');
			
			$select->addField('grps', 'id','opt');
			$select->addField('grps', 'name','val');
			$select->addField('grpmap', 'group_id','sel');
			$select->addField('grpmap', 'mro','mro');
			$select->addField('grps', 'is_admin','dropdown');
			$select->addField('cls', 'is_compliance','is_compliance');
			$select->condition('grps.status', 'cre_sec_sts_atv');
		}else{	 */
		 	$select = db_select('slt_groups', 'grps');
			if(!empty($emptyId))
				$select->leftjoin('slt_group_mapping', 'grpmap', 'grpmap.group_id=grps.id and grpmap.entity_id ='.$entityId.' and grpmap.entity_type =\''.$entityType.'\''. ' and grpmap.custom0 =\''.$emptyId.'\'');
			else 		
				$select->leftjoin('slt_group_mapping', 'grpmap', 'grpmap.group_id=grps.id and grpmap.entity_id ='.$entityId.' and grpmap.entity_type =\''.$entityType.'\'');
			
			$select->leftjoin('slt_group_privilege', 'priv', 'priv.group_id = grps.id and priv.page_code = \''.$page_code.'\'');
			if($entityType == 'cre_sys_obt_cls' ){
			  $select->leftJoin('slt_course_class','cls','cls.id='.$entityId.'');
			  $select->addField('cls', 'is_compliance','is_compliance');
			} 
			if($entityType == 'cre_sys_obt_crs'){
			 $select->leftJoin('slt_course_template','crs','crs.id='.$entityId.'');
			 $select->addField('crs', 'is_compliance','is_compliance');
			}
			$select->addField('grps', 'id','opt');
			$select->addField('grps', 'name','val');
			$select->addField('grpmap', 'group_id','sel');
			$select->addField('grpmap', 'mro','mro');
			$select->addField('grps', 'is_admin','dropdown');
			$select->condition('grps.status', 'cre_sec_sts_atv');
			$select->condition('grps.code', 'grp_sup', "!=");
			$select->where("if(grps.is_admin = 1,(priv.group_id is not null or grpmap.group_id is not null),1=1)");
	//	}
		filterByUserAccessPrivileges($select, 'grps','',1,'cre_sec','attach','grps','','','grpmap.group_id is not null or');   //68252 : Admin cannot see inaccessible groups attached in access popup
		if($searchText != '' && $searchText != null) {
			$select->condition('grps.name', '%' . db_like($searchText) . '%', 'LIKE');
		}
		 	$select->groupBy('grps.id');
			$select->orderBy('sel','desc');
			$select->orderBy('val');
		
		if($grpDisType[$entityType] == 'admin' || (!empty($searchType) && $searchType =='Admin')) {
			$select->condition('grps.is_admin',1,'=');
		}
		if(!empty($searchType) && $searchType =='Learner') {
			$select->condition('grps.is_admin',1,'!=');
		}
		$count = $select->countQuery()->execute()->fetchField();
		$select->range($start,$limit);
		expDebug::dPrintDBAPI("Access getGroupDetails Query ",$select);
		$result = $select->execute()->fetchAll();
		
		// Check compliance course and disable the options
		if ($entityType == 'cre_sys_obt_cls' && isset($courseId) ) {
		 $selectCrsCmp = db_select('slt_course_template','crs');
		 $selectCrsCmp->addField('crs','is_compliance');
		 $selectCrsCmp->condition('crs.id',$courseId,'=');
		 expDebug::dPrintDBAPI('$selectCrsCmp ' , $selectCrsCmp, 4);
		 $isComplianceCourse = $selectCrsCmp->execute()->fetchField();
		 //expDebug::dPrint('$isComplianceCourse' . $isComplianceCourse, 4);
		 if ($isComplianceCourse) {
    		  foreach($result as $res){
    		   $res->is_compliance = 1;
    		  }
		  }
		}
		
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

	expDebug::dPrint('$$result$$result$$result--'.print_r($result,true),4);
	foreach($result as $key=>$val){
		$result[$key]->sel = !empty($val->sel) ? "1" : "0";		
		
		/* if($result[$key]->mro == null) {
			$result[$key]->mro= "0";
		}else {
		 $result[$key]->mro = ($val->mro == 'M') ? "1" : "2"; 
	  } */			
	  
	  
		$result[$key]->dropdown = ($val->dropdown == 1 || $val->is_compliance == 1)? 'disable' : 'enable';
				
	}	
	expDebug::dPrint('selllll--'.print_r($result,true),4);
	
	$output = array();
	$output['records'] = $count;
	$output['page'] = $page ;
	$output['data'] = $result;
	expDebug::dPrint('$output$output$output--'.print_r($output,true),4);
	echo drupal_json_output($output);
		expDebug::dPrint('after echo--'.print_r($output,true),4);
	}catch (Exception $ex) {
		watchdog_exception('getGroupAccessDetails', $ex);
		expertusErrorThrow($ex);
	}
}

function getoptionsList(){
	$select = db_select('slt_profile_list_items', 'pf');
	$select->addField('pf', 'code','opt');
	$select->addField('pf', 'name','val');
	$select->condition('pf.code', array('cre_sys_inv_opt','cre_sys_inv_man', 'cre_sys_inv_rec'),'IN');
	$select->orderBy('pf.attr1');
	$result = $select->execute()->fetchAll();
	expDebug::dPrintDBAPI("getoptionsList",$select);
	expDebug::dPrint('getoptionsList value'.print_r($result, true),4);
	//return drupal_json_output($result);
	$out = "[";
	foreach($result as $r){
		$out .= ($out=="[")?'':',';
		$out .= '{"opt":"'.$r->opt.'","val":"'.$r->val.'"}';
	}
	$out .= "]";
	
	expDebug::dPrint('getoptionsList value shobana'.$out,4);
	return $out;
}

function getGroupAutocomplete(){
	try{
		expDebug::dPrint('autocomplete enter');
		$searchText =  $_GET['z'];
		$searchType    =  $_GET['type'];
		if (strpos($_GET['uniqueId'], VAR_START_CHAR) !== false && function_exists('alterArg'))
			$uniqueId = core_decrypt($_GET['uniqueId']);
		else
			$uniqueId = $_GET['uniqueId'];
		
		expDebug::dPrint('$uniqueId-->'.$uniqueId,4);
		$getId = explode('-',$uniqueId);
		expDebug::dPrint('$getId-->'.print_r($getId,true),4);
		$entityId = $getId[0];
		$entityType = $getId[1];
		$courseId = $getId[2];
		expDebug::dPrint('$searchType -->> : '.$searchType, 4);
		if($entityType == 'cre_sys_obt_crt' || $entityType == 'cre_sys_obt_trn' || $entityType == 'cre_sys_obt_cur'){
			$page_code = 'cre_sys_obt_trp';
		}else if($entityType == 'cre_sys_obt_cls'){
			$page_code = 'cre_sys_obt_crs';
		}else{
			$page_code = $entityType;
		}
		
		if($entityType == 'cre_sys_obt_cls'){
			$selectClass = db_select('slt_group_mapping','grp_map');
			$selectClass->addExpression('COUNT(1)','count');
			$selectClass->condition('grp_map.entity_id',$entityId,'=');
			$selectClass->condition('grp_map.entity_type',$entityType,'=');
			$clsMapCnt = $selectClass->execute()->fetchField();
			expDebug::dPrint('checking the results of the mapped class count'.$clsMapCnt,3);
		}
		
		if($entityType == 'cre_sys_obt_cls' && $clsMapCnt == 0  && $entityId !=0 ){ // Entity id Not Equal To zero issue is Fixed For this Ticket #0036447
			$selectCrsId = db_select('slt_course_class','cls');
			$selectCrsId->addField('cls','course_id');
			$selectCrsId->condition('cls.id',$entityId,'=');
			$courseId = $selectCrsId->execute()->fetchField();
				
			$select = db_select('slt_groups', 'grps');
			$select->addField('grps', 'name', 'name');
			$select->addField('grps', 'id','id');
			$select->leftjoin('slt_group_mapping', 'grpmap', 'grpmap.group_id=grps.id and grpmap.entity_id ='.$courseId.' and grpmap.entity_type =\'cre_sys_obt_crs\'');
			$select->leftJoin('slt_course_class','cls','cls.id='.$entityId.'');
			$select->leftjoin('slt_group_privilege', 'priv', 'priv.group_id = grps.id and priv.page_code = \''.$page_code.'\'');
			$select->where("if(grps.is_admin = 1,(priv.group_id is not null or grpmap.group_id is not null),1=1)");
		}else{	
		 	$select = db_select('slt_groups', 'grps');
			if(!empty($emptyId))
				$select->leftjoin('slt_group_mapping', 'grpmap', 'grpmap.group_id=grps.id and grpmap.entity_id ='.$entityId.' and grpmap.entity_type =\''.$entityType.'\''. 'and grpmap.custom0 =\''.$emptyId.'\'');
			else 		
				$select->leftjoin('slt_group_mapping', 'grpmap', 'grpmap.group_id=grps.id and grpmap.entity_id ='.$entityId.' and grpmap.entity_type =\''.$entityType.'\'');

			$select->leftjoin('slt_group_privilege', 'priv', 'priv.group_id = grps.id and priv.page_code = \''.$page_code.'\'');
			if($entityType == 'cre_sys_obt_cls' ){
			  $select->leftJoin('slt_course_class','cls','cls.id='.$entityId.'');
			} 
			if($entityType == 'cre_sys_obt_crs'){
			 $select->leftJoin('slt_course_template','crs','crs.id='.$entityId.'');
			}
			$select->addField('grps', 'name','name');
			$select->addField('grps', 'id','id');
			$select->where("if(grps.is_admin = 1,(priv.group_id is not null or grpmap.group_id is not null),1=1)");
		}
		// Add condition
		// Order in dictionary sort order
		if($searchType =='Learner' || $searchType =='Discount'){
			$select->condition('grps.is_admin', '0', '=');
		}else if($searchType =='Admin'){
			$select->condition('grps.is_admin', '1', '=');
		}
		filterByUserAccessPrivileges($select, 'grps','',1,'cre_sec','attach','grps');
		$select->condition('grps.status', 'cre_sec_sts_atv', '=');
		$select->condition('grps.name', '%' . db_like($searchText) . '%', 'LIKE');
		if($entityType == 'cme_pmt'){
			$select->condition('grps.is_admin', '1', '=');
		}
		$select->orderBy('name');
		$select->groupby('id');

		expDebug::dPrintDBAPI(' $select obj = ' , $select);
		$matchinggrpNames = $select->execute()->fetchAll();
		expDebug::dPrint(' $matchingorgNames = '. print_r($matchinggrpNames, true) , 4);

		foreach ($matchinggrpNames as $grpName) {
			print $grpName->name . "\n";
		}
	}catch (Exception $ex) {
		watchdog_exception('getGroupAutocomplete', $ex);
		expertusErrorThrow($ex);
	}
}

function getAccessUserAutocomplete(){
	try{
		expDebug::dPrint('autocomplete enter',4);
		//$searchText =  $_GET['z'];
		$searchType    =  $_GET['type'];
		if (strpos($_GET['uniqueId'], VAR_START_CHAR) !== false && function_exists('alterArg'))
			$uniqueId = core_decrypt($_GET['uniqueId']);
		else
			$uniqueId = $_GET['uniqueId'];
		
		expDebug::dPrint('$uniqueId-->'.$uniqueId,4);
		$getId = explode('-',$uniqueId);
		expDebug::dPrint('$getId-->'.print_r($getId,true),4);
		$entityId = $getId[0];
		$entityType = $getId[1];
		$courseId = $getId[2];
		
		//$entityId   = $_GET['entity_id'];
		//$entityType = $_GET['entity_type'];
		//$searchType = $_GET['search_type'];
		$nameSubstr = $_GET['z'];
		$userId = getIdOfLoggedInUser();

		$runQuery = 1;
		if($searchType == 'username' || $searchType == ''){
			// Select the table
			$select = db_select('slt_person', 'per');

			// Add fields to fetch
			$select->addExpression('DISTINCT(per.user_name)', 'name');

			// Add conditions
			if(!empty($nameSubstr)){
				//Modified for the ticket no:18743
				$select->condition('per.user_name', '%' . db_like($nameSubstr) . '%' ,'like');
			}
			$select->condition('per.user_name', array('admin','guest'),'NOT IN');
			$select->condition('per.status', "cre_usr_sts_atv");
			filterByUserAccessPrivileges($select,'per','',1,'cre_usr','autocomplete','per');
			$select->orderBy('name');
		}elseif($searchType == 'name'){
			$select = db_select('slt_person', 'per');
			$select->addExpression('DISTINCT(per.full_name)', 'name');
			$select->condition('per.full_name', '%' . db_like($nameSubstr) . '%' ,'like');
			if (!empty($excludedUserId)) {
				$select->condition('per.id', $excludedUserId, 'NOT IN');
			}

			$select->condition('per.full_name', array('admin','guest'),'NOT IN');
			$select->condition('per.status', "cre_usr_sts_atv");
			filterByUserAccessPrivileges($select,'per','',1,'cre_usr','autocomplete','per');
			if($userId != 1)
			filterByGroupAccessPrivileges($select,$entityId,'class');
			$select->orderBy('name');
		}
		elseif($searchType == 'org'){
			$select = db_select('slt_organization', 'org');
			$select->leftjoin('slt_person', 'per', 'per.org_id = org.id');
			$select->addExpression('DISTINCT(org.name)', 'name');
			$select->condition('org.status', array('cre_org_sts_itv','cre_org_sts_act'), 'IN');
			$select->condition('org.name', '%' . db_like($nameSubstr) . '%', 'LIKE');
			$select->orderBy('name');
		}
		elseif($searchType == 'jobrole'){
			$select = getMatchingProfileItemNamesKeyedautocomplete($_GET['z'], 'cre_usr_jrl_');
			$select->distinct();
			$select->leftjoin('slt_person_jobrole_mapping', 'permap', 'permap.job_role = spli.code');
			$select->leftjoin('slt_person', 'per', 'per.id = permap.user_id');
			filterByUserAccessPrivileges($select, 'spli','',1,'cre_usr_jrl','','spli');
		}
		elseif($searchType == 'usertype'){
			$select = getMatchingProfileItemNamesKeyedautocomplete($_GET['z'], 'cre_usr_ptp_');		
			$select->distinct();	
			$select->leftjoin('slt_person', 'per', 'per.user_type = spli.code');
			$runQuery = 1;
			}
		elseif($searchType == 'manager'){
			$select = db_select('slt_person', 'per');
			$select->addExpression('DISTINCT(per.full_name)', 'name');
			$select->condition(db_or()->condition('per.user_name', '%' . db_like($nameSubstr) . '%' ,'like')
			->condition('per.full_name', '%' . db_like($nameSubstr) . '%' ,'like'));
			$select->condition('per.user_name', array('admin','guest'),'NOT IN');
			$select->condition('per.status', "cre_usr_sts_atv");
			//$select->condition('per.is_manager', "1");
    		$select->where('per.is_manager=\'Y\'');
			$select->orderBy('name');
			expDebug::dPrintDBAPI('The Jooooooooe--->' , $select);		
		}
		elseif($searchType == 'group'){
			$select = db_select('slt_groups', 'grps');
			$select->leftjoin('slt_group_mapping', 'grpmap', 'grps.id = grpmap.group_id');
			$select->addExpression('DISTINCT(grps.name)', 'name');
			$select->condition('grps.name', '%' . db_like($nameSubstr) . '%' ,'like');
			$select->condition('grpmap.entity_type','cre_sys_obt_cls','=');
			$select->condition('grpmap.entity_id',$entityId,'=');
			$select->condition('grps.is_admin',0,'=');
			$select->orderBy('name');

			$matchingQuery1 = $select->execute()->fetchAll();
			expDebug::dPrint('The $matchingQuery1 value is here for the '.print_r($matchingQuery1,1),2);
			if($matchingQuery1[0]->name == '')
			{
				$select = db_select('slt_groups', 'grps');
				$select->addExpression('DISTINCT(grps.name)', 'name');
				$select->condition('grps.name', '%' . db_like($nameSubstr) . '%' ,'like');
				$select->condition('grps.is_admin',0,'=');
				$select->orderBy('name');
				$matchingQuery2 = $select->execute()->fetchAll();
				foreach ($matchingQuery2 as $result) {
					print $result->name . "\n";
				}
			}
			else
			{
				foreach ($matchingQuery1 as $result) {
					print $result->name . "\n";
		}
			}
			$runQuery = 0;
		}
	/*	if($searchType != 'group'){
			$userId = getIdOfLoggedInUser();		
			if($userId != 1)
				filterByGroupAccessPrivileges($select,$entityId,'class');
		}*/
			if($searchType == 'user' || $searchType == '' || $searchType == 'fullname'){
				$select->leftjoin('slt_course_class', 'class', "class.id = $entityId");
				$select->leftjoin('slt_course_template', 'course', "course.id = class.course_id AND course.compliance_completed IN('hire_days','date')");
				//$select->where('(IF(course.id IS NOT NULL AND per.hire_date IS NOT NULL,(DATE_ADD(DATE_FORMAT(per.hire_date, "%Y-%m-%d"), INTERVAL course.complete_days DAY) >= CURDATE()),1=1))');
				$select->where('(IF(course.compliance_completed = "hire_days" AND per.hire_date IS NOT NULL,(DATE_ADD(DATE_FORMAT(per.hire_date, "%Y-%m-%d"), INTERVAL course.complete_days DAY) >= CURDATE()),IF(course.complete_date IS NOT NULL,course.complete_date >= CURDATE(),1=1)))');
			}
		if($runQuery){
			expDebug::dPrintDBAPI(' $select SQL = ' , $select);

			$matchingQuery = $select->execute()->fetchAll();
			expDebug::dPrint(' $matchingPersons = '. print_r($matchingPersons, true),3);
			foreach ($matchingQuery as $result) {
				print $result->name . "\n";
			}
		}
	}catch(Exception $ex){
		watchdog_exception('getGroupAutocomplete', $ex);
		expertusErrorThrow($ex);
	}
}

function getSelectedValues($entityId, $entityType,$emptyId =''){
	try{
		/* if($entityType == 'cre_sys_obt_cls'){
			$selectClass = db_select('slt_group_mapping','grp_map');
			$selectClass->addExpression('COUNT(1)','count');
			$selectClass->condition('grp_map.entity_id',$entityId,'=');
			$selectClass->condition('grp_map.entity_type',$entityType,'=');
			$clsMapCnt = $selectClass->execute()->fetchField();
			expDebug::dPrint('cehckign the result of the file in grpmap'.$clsMapCnt);
		}
		
		if($entityType == 'cre_sys_obt_cls' && $clsMapCnt == 0){
			$selectCrsId = db_select('slt_course_class','cls');
			$selectCrsId->addField('cls','course_id');
			$selectCrsId->condition('cls.id',$entityId,'=');
			$courseId = $selectCrsId->execute()->fetchField();
			
			$select = db_select('slt_groups', 'grps');
			$select->leftjoin('slt_group_mapping', 'grpmap', 'grpmap.group_id=grps.id');
			$select->leftjoin('slt_profile_list_items','pli','grpmap.mro=pli.code');
			$select->addField('grps', 'id','id');
			$select->addField('grps', 'name','name');
			$select->addField('grps', 'is_admin','admin');
			$select->addField('pli', 'name','mro');
			$select->addField('pli', 'code','code');
			$select->condition('grpmap.entity_id', $courseId, '=');
			$select->condition('grpmap.entity_type', 'cre_sys_obt_crs', '=');
			$select->condition('grps.status', 'cre_sec_sts_atv', '=');
			//$select->addExpression('if(grps.'.$fld3.'=\'All\' or grp.id is not null,1,0)','sel');
			$select->groupBy('grps.id');
			$select->orderBy('grps.name');
			expDebug::dPrintDBAPI("selected value from query ",$select);
			$result = $select->execute()->fetchAll();
			return $result;
		}else{ */
			$select = db_select('slt_groups', 'grps');
			$select->leftjoin('slt_group_mapping', 'grpmap', 'grpmap.group_id=grps.id');
			$select->leftjoin('slt_profile_list_items','pli','grpmap.mro=pli.code');
			$select->addField('grps', 'id','id');
			$select->addField('grps', 'name','name');
			$select->addField('grps', 'is_admin','admin');
			$select->addField('pli', 'name','mro');
			$select->addField('pli', 'code','code');
			$select->condition('grpmap.entity_id', $entityId, '=');
			$select->condition('grpmap.entity_type', $entityType, '=');
			$select->condition('grps.status', 'cre_sec_sts_atv', '=');
			if(!empty($emptyId))
				$select->condition('grpmap.custom0', $emptyId, '=');
			$select->groupBy('grps.id');
			$select->orderBy('grps.name');
			filterByUserAccessPrivileges($select, 'grps','',1,'cre_sec','attach','grps','','','grpmap.group_id is not null or');   //68252 : Admin cannot see inaccessible groups attached in access popup
			expDebug::dPrintDBAPI("selected value from query ",$select);
			$result = $select->execute()->fetchAll();
			return $result;
		//}
		
		
	}catch (Exception $ex) {
		watchdog_exception('getSelectedValues', $ex);
		expertusErrorThrow($ex);
	}
}

function getUserSelectedValues($entityId, $entityType,$emptyId =''){
	try{
		$select = db_select('slt_person', 'per');
		$select->leftjoin('slt_user_access_mapping', 'grpmap', 'grpmap.user_id=per.id');
		$select->leftjoin('slt_profile_list_items','pli','grpmap.mro=pli.code');
		$select->addField('per', 'id','id');
		$select->addField('per', 'full_name','fullname');
		$select->addField('per', 'user_name','username');
		$select->addField('grpmap', 'access_type','admin');
		$select->addField('pli', 'name','mro');
		$select->addField('pli', 'code','code');
		$select->condition('grpmap.entity_id', $entityId, '=');
		$select->condition('grpmap.entity_type', $entityType, '=');
		$select->condition('per.status', 'cre_usr_sts_atv', '=');
		$select->condition('per.id', array(1,2), 'NOT IN');
		if(!empty($emptyId))
			$select->condition('grpmap.custom0', $emptyId, '=');
		$select->groupBy('per.id');
		$select->orderBy('per.user_name');
		filterByUserAccessPrivileges($select,'per','',1,'cre_usr','','per');
		expDebug::dPrintDBAPI("selected value from query ",$select);
		$result = $select->execute()->fetchAll();
		return $result;
	}catch(Exception $ex){
		watchdog_exception('getSelectedValues', $ex);
		expertusErrorThrow($ex);
	}
}

function getDiscountGroupAccessDetails($uniqueId){
	try{

		expDebug::dPrint('$uniqueId-->'.$uniqueId,4);
		$getId = explode('-',$uniqueId);
		expDebug::dPrint('$getId-->'.print_r($getId,true),4);
		$entityId = $getId[0];
		$entityType = $getId[1];
		expDebug::dPrint('tttt-->'.$entityId.'ssss--->'.$entityType);
		$searchText =  $_GET['z'];
		$page = $_GET['page'];
		$limit = $_GET['rows'];
		$start = $limit * $page - $limit;
		expDebug::dPrint('tttt1111-->'.$limit.'ssss222--->'.$start);

		$select = db_select('slt_groups', 'grps');
		$select->leftjoin('slt_discounts', 'disc', 'FIND_IN_SET(grps.id,disc.group_id)>0 and disc.uc_discount_id='.$entityId.'');
		$select->addField('grps', 'id','opt');
		$select->addField('grps', 'name','val');
		$select->addExpression('if(disc.group_id is not null,grps.id,\'\')','sel');
		//$select->addField('grpmap', 'mro','mro');
		$select->addField('grps', 'is_admin','dropdown');
		$select->condition('grps.status', 'cre_sec_sts_atv');
		//38776: Need to remove admin group in Discount -->Users link
		$select->condition('grps.is_admin', 1,'!=');
		filterByUserAccessPrivileges($select, 'grps','',1,'cre_sec','attach','grps');
		if($searchText != '' && $searchText != null)
			$select->condition('grps.name', '%' . db_like($searchText) . '%', 'LIKE');

		$select->groupBy('grps.id');
		$select->orderBy('sel','desc');
		$select->orderBy('grps.name');


		$count = count($select->execute()->fetchAll());
		$select->range($start,$limit);
		expDebug::dPrintDBAPI("Access getGroupDetails Query ",$select);
		$result = $select->execute()->fetchAll();
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
		expDebug::dPrint('autocomplete enter'.$searchText);
		expDebug::dPrint('autocomplete enter'.$searchText);

		expDebug::dPrint('$$result$$result$$result--'.print_r($result,true),4);
		foreach($result as $key=>$val){
			$result[$key]->sel = !empty($val->sel) ? "1" : "0";

			/* if($result[$key]->mro == null) {
			 $result[$key]->mro= "0";
			}else {
		 $result[$key]->mro = ($val->mro == 'M') ? "1" : "2";
	  } */
			$result[$key]->dropdown = ($val->dropdown == 1)? 'disable' : 'enable';

		}
		expDebug::dPrint('selllll--'.print_r($result,true),4);

		$output = array();
		$output['records'] = $count;
		$output['page'] = $page ;
		$output['data'] = $result;
		expDebug::dPrint('$output$output$output--'.print_r($output,true),4);
		echo drupal_json_output($output);
		expDebug::dPrint('after echo--'.print_r($output,true),4);
	}catch (Exception $ex) {
		watchdog_exception('getGroupAccessDetails', $ex);
		expertusErrorThrow($ex);
	}
}

function getEntityAccess($entityId = '', $entityType = '',$courseId ='') {
 $selectEntity = db_select('slt_group_mapping','grp_map');
 $selectEntity->addExpression('COUNT(1)','count');
 if($entityType == 'cre_sys_obt_cls'){// added for this ticket #0042295 : To Show Course Access or  class Access in Catalog
 	$selectEntityId_and = db_and(); // Giving the Course id and Class id
 	$selectEntityId_and->condition('grp_map.entity_id',$entityId,'=');
 	$selectEntityId_and->condition('grp_map.entity_type',$entityType,'=');
 	$selectEntityId_and->condition('grp_map.group_type',0,'=');
 	$selectEntityId_and1 = db_and();
 	$selectEntityId_and1->condition('grp_map.entity_id',$courseId,'=');
 	$selectEntityId_and1->condition('grp_map.entity_type','cre_sys_obt_crs','=');
 	$selectEntityId_and1->condition('grp_map.group_type',0,'=');
 	$selectEntityType_or = db_or();
 	$selectEntityType_or->condition($selectEntityId_and);
 	$selectEntityType_or->condition($selectEntityId_and1);
 	$selectEntity->condition($selectEntityType_or);
 }else{ // For Tp or other Details
 	$selectEntity->condition('grp_map.entity_id',$entityId,'=');
 	$selectEntity->condition('grp_map.entity_type',$entityType,'=');
 	$selectEntity->condition('grp_map.group_type',0,'=');
 }
    $selectEntity->where('\'cre_sec_sts_atv\' = (SELECT status FROM slt_groups grp where grp.id = grp_map.group_id)');
 expDebug::dPrintDBAPI("Access getEntityAccess Query ",$selectEntity);
 $selectEntityCount = $selectEntity->execute()->fetchField();
 //expDebug::dPrint('cehckign the result of the file in grpmap'.$selectEntityCount);
 if($selectEntityCount > 0) {
  return true;
 } else {
  return false;
 }
}

function checkUserEntityAccess($entityId = '', $entityType = '', $userId = '',$courseId ='') {
 $selectEntity = db_select('slt_groups','grp');
 $selectEntity->leftjoin('slt_group_mapping','grp_map','grp.id=grp_map.group_id');
 $selectEntity->leftjoin('slt_group_attributes','grpatt','grp.id=grpatt.group_id');
 $selectEntity->join('slt_person','per');
 $selectEntity->addExpression('if(grp_map.id is null,1,COUNT(1))','count');
 $selectEntity->condition('per.id',$userId,'=');
 if($entityType == 'cre_sys_obt_cls'){// added for this ticket #0042295 : To Show Course Access or  class Access in Catalog
 	$selectEntityId_and = db_and(); // Giving the Course id and Class id
 	$selectEntityId_and->condition('grp_map.entity_id',$entityId,'=');
 	$selectEntityId_and->condition('grp_map.entity_type',$entityType,'=');
 	$selectEntityId_and1 = db_and();
 	$selectEntityId_and1->condition('grp_map.entity_id',$courseId,'=');
 	$selectEntityId_and1->condition('grp_map.entity_type','cre_sys_obt_crs','=');
 	$selectEntityType_or = db_or();
 	$selectEntityType_or->condition($selectEntityId_and);
 	$selectEntityType_or->condition($selectEntityId_and1);
 	$selectEntity->condition($selectEntityType_or);
 }else{
 	$selectEntity->condition('grp_map.entity_id',$entityId,'=');
 	$selectEntity->condition('grp_map.entity_type',$entityType,'=');
 }
 //$selectEntity->where("FIND_IN_SET('".$userId ."',grp.added_users) > 0");
 //$selectEntity->where("(grp.removed_users is null OR FIND_IN_SET('".$userId ."',grp.removed_users)<= 0)  OR (FIND_IN_SET('".$userId ."',grp.added_users)>0)");
 $selectEntity->leftjoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id = per.id');
 $selectEntity->where("(((((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
 		AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
 		AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
 		AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
 		AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
 		AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
 		AND ((if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id =per.id)>0,(grp.job_role IS NULL)) OR (grp.job_role='')) OR (FIND_IN_SET(lpjm.job_role,grp.job_role)>0) )
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
					if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1))
 		AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(per.id,grp.added_users)>0))))");
 expDebug::dPrintDBAPI("Access getEntityAccess Query ",$selectEntity);
 $selectEntityCount = $selectEntity->execute()->fetchField();
 expDebug::dPrint('checking the result checkUserEntityAccessp'.$selectEntityCount);
 if($selectEntityCount > 0) {
  return true;
 } else {
  return false;
 }
}


function deleteAttachedGroup($entityId,$entityType,$group_id = array()){
	try{
	    
	    
	    $sel = db_select('slt_group_mapping','g');
		if($entityType == 'cre_sys_obt_cls')
			$entityId = explode(',',$entityId);
	    $sel->addField('g','id','id');
	    $sel->addField('g','entity_id','entity_id');
	    $sel->addField('g','group_id','group_id');
	    $sel->addField('g','group_type','group_type');
	    $sel->condition('entity_id',$entityId,is_array($entityId) ? 'IN' : '=');
	    $sel->condition('entity_type',$entityType,is_array($entityType) ? 'IN' : '=');
	    if(!empty($group_id))
			$sel->condition('group_id',$group_id,'IN');
	    $sel1=$sel->execute()->fetchAll();
	    
	  expDebug::dPrint("result of ids-->".print_r($sel1,1),4);
	  foreach($sel1 as $val)
	  {
	      $fields = array(
		            'table_name' => 'slt_group_mapping',
		            'entity_id' => $val->id,
		            'entity_type' => 'group_mapping',
		            'module_name' => 'exp_sp_administration_catalog_access',
		            'functionality_name' => 'deleteAttachedGroup',
		            'api_name' => null,
		            'deleted_on' => now(),
		            'deleted_by' => getIdOfLoggedInUser(),
		            'parent1_entity_id' => $val->entity_id,
		            'parent1_entity_type' => $entityType,
		            'parent2_entity_id' => $val->group_id,
		            'parent2_entity_type' => 'group_id',
		            'parent3_entity_id' => $val->group_type,
		            'parent3_entity_type' => 'group_type',
		    );
		    deleted_log_entry($fields);
	      //deleted_log_entry('slt_group_mapping',$val->id,'group_mapping','exp_sp_administration_catalog_access','deleteAttachedGroup',null,getIdOfLoggedInUser());
	  }
	   
		$delQry = db_delete('slt_group_mapping');
		//if($entityType == 'cre_sys_obt_cls')
		//	$entityId = explode(',',$entityId);
		$delQry->condition('entity_id',$entityId,is_array($entityId) ? 'IN' : '=');
		$delQry->condition('entity_type',$entityType,is_array($entityType) ? 'IN' : '=');
		if(!empty($group_id))
			$delQry->condition('group_id',$group_id,'IN');
		
		expDebug::dPrintDBAPI('delete group attached to class ' , $delQry);
		$delQry->execute();
		
		$catalogFields['updated_on'] = now();
		if($entityType == 'cre_sys_obt_cls'){
			$update = db_update('slt_course_class');
			$update->fields($catalogFields);
			$update->condition('id', $entityId, '=');
			$update->execute();
		}
		else if($entityType == 'cre_sys_obt_crs'){
			$update = db_update('slt_course_template');
			$update->fields($catalogFields);
			$update->condition('id', $entityId,'=');
			$update->execute();
			$update_class = db_update('slt_course_class');
			$update_class->fields($catalogFields);
			$update_class->condition('course_id', $entityId,'=');
			$update_class->execute();
		}
		else {
			$update = db_update('slt_program');
			$update->fields($catalogFields);
			$update->condition('id', $entityId,'=');
			$update->execute();
		}
		updateGroupMappingDetails($entityId, $entityType, '','delete',$group_id);
	}catch(Exception $ex){
		expDebug::dPrintDBAPI('Error in deleteAttachedGroup' ,1);
	}
}

function deleteAttachedUsers($entityId,$entityType,$user_id = array()){
	try{
	    if($form_state['values']['create_user_id'])
	        $loggedInUserId = $form_state['values']['create_user_id'];
	    else
	        $loggedInUserId  = getIdOfLoggedInUser();
	    
	    expDebug::dPrint('$delete_id--->1'.print_r($entityId,true),5);
	    
	    $entityId = ($entityType == 'cre_sys_obt_cls') ? explode(',',$entityId): $entityId;
	    
	    $delete_id = db_select('slt_user_access_mapping', 'uam');
	    $delete_id->addField('uam', 'id', 'id');
	    $delete_id->addField('uam', 'user_id', 'user_id');
	    $delete_id->condition('entity_id',$entityId,is_array($entityId) ? 'IN' : '=');
	    $delete_id->condition('entity_type',$entityType,is_array($entityType) ? 'IN' : '=');
	    expDebug::dPrintDBAPI('$delete_id>>> ', $delete_id);
	    $del=$delete_id->execute()->fetchAll();
	    expDebug::dPrint('$delete_id'.print_r($del,true),5);
	   
	    
		$delQry = db_delete('slt_user_access_mapping');
		//if($entityType == 'cre_sys_obt_cls')
			//$entityId = explode(',',$entityId);
		$delQry->condition('entity_id',$entityId,is_array($entityId) ? 'IN' : '=');
		$delQry->condition('entity_type',$entityType,is_array($entityType) ? 'IN' : '=');
		if(!empty($user_id))
			$delQry->condition('user_id',$user_id,'IN');
		
		expDebug::dPrintDBAPI('delete group attached to class ' , $delQry);
		$delQry->execute();
		
		//Make delete log entries
		foreach ($del as $val){
		    $fields = array(
		            'table_name' => 'slt_user_access_mapping',
		            'entity_id' => $val->id,
		            'entity_type' => 'user_access',
		            'module_name' => 'exp_sp_administration_catalog_access',
		            'functionality_name' => 'deleteAttachedUsers',
		            'api_name' => null,
		            'deleted_on' => now(),
		            'deleted_by' => $loggedInUserId,
		            'parent1_entity_id' => $entityId,
		            'parent1_entity_type' => 'entity_id',
		            'parent2_entity_id' => $val->user_id,
		            'parent2_entity_type' => 'user_id',
		    );
		    deleted_log_entry($fields);
		  //  deleted_log_entry('slt_user_access_mapping',$val->id,'user_access','exp_sp_administration_catalog_access','deleteAttachedUsers', null, $loggedInUserId);
		}expDebug::dPrint('slt_user_access_mapping table log');
		
	   $catalogFields['updated_on'] = now();
		if($entityType == 'cre_sys_obt_cls'){
			$update = db_update('slt_course_class');
			$update->fields($catalogFields);
			$update->condition('id', $entityId, '=');
			$update->execute();
		}
		else if($entityType == 'cre_sys_obt_crs'){
			$update = db_update('slt_course_template');
			$update->fields($catalogFields);
			$update->condition('id', $entityId,'=');
			$update->execute();
			$update_class = db_update('slt_course_class');
			$update_class->fields($catalogFields);
			$update_class->condition('course_id', $entityId,'=');
			$update_class->execute();
		}
		else {
			$update = db_update('slt_program');
			$update->fields($catalogFields);
			$update->condition('id', $entityId,'=');
			$update->execute();
		}
		
		
		//updateGroupMappingDetails($entityId, $entityType, '','delete',$user_id);
	}catch(Exception $ex){
		expDebug::dPrintDBAPI('Error in deleteAttachedGroup' ,1);
	}
}

function insertCourseAttachedGroups($classId,$courseId){
	try{
		expDebug::dPrint("class id-->".$classId." and course id").$courseId;
		$loggedInUserId = getIdOfLoggedInUser();
		$fields = array('group_id','group_type','mro');
		$grpmapquery = db_select('slt_group_mapping', 'grp_map');
		$grpmapquery->fields('grp_map',$fields);
		$grpmapquery->addExpression('0','entity_id');
		$grpmapquery->addExpression('\'cre_sys_obt_cls\'','entity_type');
		$grpmapquery->addExpression($loggedInUserId,'created_by');
		$grpmapquery->addExpression('now()','created_on');
		$grpmapquery->addExpression($loggedInUserId,'updated_by');
		$grpmapquery->addExpression('now()','updated_on');
		$grpmapquery->addExpression("'$classId'",'custom0');
		$grpmapquery->condition('grp_map.entity_id', $courseId,'=');
		$grpmapquery->condition('grp_map.entity_type', 'cre_sys_obt_crs','=');
		
		expDebug::dPrintDBAPI('Group mapping attached to course query', $grpmapquery);
		
		db_insert('slt_group_mapping')
		->from($grpmapquery)
		->execute();
		
	}catch(Exception $ex){
	
	}
}
function insertCourseAttachedUsers($classId,$courseId){
	try{
		expDebug::dPrint("class id user-->".$classId." and course id user").$courseId;
		$loggedInUserId = getIdOfLoggedInUser();
		$fields = array('user_id','access_type','mro');
		$usrmapquery = db_select('slt_user_access_mapping', 'usr_map');
		$usrmapquery->fields('usr_map',$fields);
		$usrmapquery->addExpression('0','entity_id');
		$usrmapquery->addExpression('\'cre_sys_obt_cls\'','entity_type');
		$usrmapquery->addExpression($loggedInUserId,'created_by');
		$usrmapquery->addExpression('now()','created_on');
		$usrmapquery->addExpression($loggedInUserId,'updated_by');
		$usrmapquery->addExpression('now()','updated_on');
		$usrmapquery->addExpression("'$classId'",'custom0');
		$usrmapquery->condition('usr_map.entity_id', $courseId,'=');
		$usrmapquery->condition('usr_map.entity_type', 'cre_sys_obt_crs','=');

		expDebug::dPrintDBAPI('User mapping attached to course query', $usrmapquery);

		db_insert('slt_user_access_mapping')
		->from($usrmapquery)
		->execute();

	}catch(Exception $ex){

	}
}
?>