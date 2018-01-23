<?php

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the names in it.
 */
function classroom_add_more_btn_callback($form, $form_state) {
  try {
    return $form['classroom_fieldset'];
  } catch (Exception $ex) {
    watchdog_exception('classroom_add_more_btn_callback', $ex);
    expertusErrorThrow($ex);
  } 
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function classroom_add_row($form, &$form_state) {
  try {
    	if($form_state['num_classroom']==0){ // Added by Vincent on 24 Mar,2012 - to fix first time load after save shows readonly
        $form_state['num_classroom']=1;
      }
      $form_state['num_classroom']++;
      $form_state['display_class'] = TRUE;
      $form_state['rebuild'] = TRUE;
  } catch (Exception $ex) {
    watchdog_exception('classroom_add_row', $ex);
    expertusErrorThrow($ex);
  }       
}

function classroomOperations($form, $form_state,$entityId,$entityType){
  try {
     $loggedInUserId = getIdOfLoggedInUser();
    deleteClassroomDetails($form, $form_state,$entityId,$entityType);
    $cnt = $form_state['num_classroom']==0?1:$form_state['num_classroom'];
    for ($i = 0; $i < $cnt; $i++) {
      $classroomName = $form_state['values']['classroom_fieldset']['classroom']['clsroom_name_'.$i];
      $classroomMaxCapacity  = $form_state['values']['classroom_fieldset']['classroom']['clsroom_max_capacity_'.$i];
      if(!empty($classroomName) && !empty($classroomMaxCapacity)) {
        if(empty($form_state['values']['classroom_id_'.$i])) {
        	addNewClassroomDetails($form, $form_state,$entityId,$entityType,$i,$loggedInUserId);
        }else{
        	updateClassroomDetails($form, $form_state,$entityId,$entityType,$i,$loggedInUserId);
        }
      }
    }
  } catch (Exception $ex) {
    watchdog_exception('classroomOperations', $ex);
    expertusErrorThrow($ex);
  } 
}


function getFacilityClassroomDetails($entityId,$entityType) {
  try {
      $select = db_select("slt_classroom","clsroom");
      $select->addField('clsroom','id','classroom_id');
      $select->addField('clsroom','facility_id','facility_id');
      $select->addField('clsroom','name','clsroom_title');
     // $select->addField('clsroom','type','clsroom_type');
      $select->addField('clsroom','max_capacity','max_capacity');
      $select->addField('clsroom','is_active','is_active');
      $select->addField('clsroom','status','status');
      $select->condition('clsroom.facility_id',$entityId,'=');
      $select->condition('clsroom.is_active',1,'=');
      
      $classroomList = $select->execute()->fetchAll();
      
      return $classroomList;
  } catch (Exception $ex) {
    watchdog_exception('getFacilityClassroomDetails', $ex);
    expertusErrorThrow($ex);
  }       
}


function addNewClassroomDetails($form, &$form_state,$entityId,$entityType,$i, $createUserId, $callFromAPI =0) { 
	
  $txn = db_transaction();

  try {
    
    if($form_state['values']['status'] == 'lrn_res_fac_itv') {
      $clsroomStatus = 'lrn_res_rms_itv';
    }else{
      $clsroomStatus = $form_state['values']['classroom_fieldset']['classroom']['clsroom_status_'.$i];
    }
    	
      $insertStmt = db_insert('slt_classroom');
      $custom  = NULL;

      $fields = array(
                'facility_id'          => $entityId,                  
                'name'                 => $form_state['values']['classroom_fieldset']['classroom']['clsroom_name_'.$i],
                'max_capacity'     	   => $form_state['values']['classroom_fieldset']['classroom']['clsroom_max_capacity_'.$i],
                //'type'                 => $form_state['values']['classroom_fieldset']['classroom']['clsroom_type_'.$i],
                'type'                 => '',
                'status'               => $clsroomStatus,
                'is_active'            => 1,
                'created_by'           => $createUserId,
                'created_on'           => now(),
      					'updated_by'           => $createUserId,
      					'updated_on'           => now(),
                'custom0'              => $custom,
                'custom1'              => $custom,
                'custom2'              => $custom,
                'custom3'              => $custom,
                'custom4'              => $custom
              );
      $insertStmt->fields($fields);			            
       
      $classroomId = $insertStmt->execute();
      $insertclassform = $form_state['values']; 
      $insertclassform[facility_id] = $entityId;
      $insertclassform[name] = $form_state['values']['classroom_fieldset']['classroom']['clsroom_name_'.$i];
      $insertclassform[max_capacity] = $form_state['values']['classroom_fieldset']['classroom']['clsroom_max_capacity_'.$i];
      $insertclassform[class_status] = $clsroomStatus;

      make_audit_trail_entries($insertclassform, getIdOfLoggedInUser() , null, $classroomId ,'Inserted','cre_sys_obt_cls','exp_sp_administration_resources','addNewClassroomDetails',
                                          array('facility_id', 'name', 'max_capacity', 'class_status'));
    	   
      expDebug::dPrint(' $$classroomId = ' . print_r($classroomId, true) , 3);  
     
      return array((object)array('Id'=>$classroomId));
  }
  catch (Exception $ex) {
   $txn->rollback();
    unset($txn);
    watchdog_exception('addNewClassroomDetails', $ex);
    expertusErrorThrow($ex);
    if($callFromAPI){ 
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Unable to create a class room.";
    	return $errobj;
    }
    else {
        return array((object)array('Id'=>'Failiure'));
    } 
    //throw $ex;    
  }
  
  unset($txn);
  
}


function updateClassroomDetails($form, &$form_state,$entityId,$entityType,$i, $createUserId, $callFromAPI =0) {
  
  $txn = db_transaction();

  try {
    
    if($form_state['values']['status'] == 'lrn_res_fac_itv') {
      $clsroomStatus = 'lrn_res_rms_itv';
    }else{
      $clsroomStatus = $form_state['values']['classroom_fieldset']['classroom']['clsroom_status_'.$i];
    }

    $updateStmt = db_update('slt_classroom');
    $default= db_query("SELECT id,facility_id,name as class_name,max_capacity,status as class_status from slt_classroom where id=:uid",array(":uid"=>$form_state['values']['classroom_id_'.$i]))->fetchAll();
    $custom  = NULL;  

    $fields = array(
                'facility_id'          => $entityId,                  
                'name'                 => $form_state['values']['classroom_fieldset']['classroom']['clsroom_name_'.$i],
                'max_capacity'     	   => $form_state['values']['classroom_fieldset']['classroom']['clsroom_max_capacity_'.$i],
                //'type'                 => $form_state['values']['classroom_fieldset']['classroom']['clsroom_type_'.$i],
                'type'                 => '',
                'status'               => $clsroomStatus,
                'updated_by'           => $createUserId,
                'updated_on'           => now(),
                'custom0'              => $custom,
                'custom1'              => $custom,
                'custom2'              => $custom,
                'custom3'              => $custom,
                'custom4'              => $custom   
              );
    
    $updateStmt->fields($fields);    
    $updateStmt->condition('id', $form_state['values']['classroom_id_'.$i]);
    $numUpdated = $updateStmt->execute();
    
      $updateclassform = $form_state['values']; 
      $updateclassform[facility_id] = $entityId;
      $updateclassform[class_name] = $form_state['values']['classroom_fieldset']['classroom']['clsroom_name_'.$i];
      $updateclassform[max_capacity] = $form_state['values']['classroom_fieldset']['classroom']['clsroom_max_capacity_'.$i];
      $updateclassform[class_status] = $clsroomStatus;
      make_audit_trail_entries($updateclassform, getIdOfLoggedInUser() , $default, $entityId ,'Updated-class','cre_sys_obt_cls','exp_sp_administration_resources','updateClassroomDetails',
                                          array('facility_id', 'class_name', 'max_capacity', 'class_status'));
      	
    expDebug::dPrint(' $numUpdated = ' . print_r($numUpdated, true) , 3);
    if($callFromAPI){ 
        return array((object)array('Id'=>$form_state['values']['classroom_id_'.$i]));
    }
      
  }
  catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('updateClassroomDetails', $ex);
    expertusErrorThrow($ex);
    if($callFromAPI){
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Unable to update a class room.";
    	return $errobj;
    }
    else { 
        return array((object)array('Id'=>'Failiure'));
    }
    throw $ex;
  }

  unset($txn);
  
}

/**
 * Submit handler for the "Delete" button.
 *
 * Decrements the Deleted value and causes a form rebuild.
 */
function deleteClassroom($form, &$form_state) { 
  try { 
      $classroomId = $form_state['triggering_element']['#ajax']['param'];
      $indexId = $form_state['triggering_element']['#ajax']['loop_id'];
      $form_state['delete_classroom_fieldset'][] = $classroomId;    
      $form_state['delete_classroom_index'][] = $indexId;
      $form_state['display_class'] = TRUE;
    	if($form_state['num_classroom']==count($form_state['delete_classroom_index']) || $form_state['num_classroom']==0){
        classroom_add_row($form, $form_state);
      }    
      $form_state['rebuild'] = TRUE;
  } catch (Exception $ex) {
    watchdog_exception('deleteClassroom', $ex);
    expertusErrorThrow($ex);
  }       
 
}


function deleteClassroomDetails($form, &$form_state,$entityId,$entityType) {
  
  $classroomId = $form_state['values']['classroom_fieldset']['classroom_hidden_delete'];
  
  $classroomIdArr = explode(',', $classroomId);
 
  $txn = db_transaction();
  
  try {
    if(!empty($classroomId)){
       
        // Delete or Inactive the records from slt_classroom
        $deleteStmt = db_update('slt_classroom');
        $custom  = NULL;  
        
        $fields = array(
                    'is_active'               => 0
                  );
        
        $deleteStmt->fields($fields);    
        $deleteStmt->condition('id', $classroomIdArr, 'IN');
        $numUpdated = $deleteStmt->execute();
        
        /*UPDATE  slt_classroom 
        SET  is_active = '0' 
        WHERE slt_classroom.id = xid
        LIMIT 1;
        UPDATE slt_master_search 
        	SET STATUS	= 'Inactive',
        	updated_on 	= NOW(),
        	updated_by 	= xUpdatedBy 
        	WHERE pk_id=xid AND TYPE = 'Classroom'; */
        
        //Delete or Inactive the records from slt_master_search
        $updateMasterStmt = db_update('slt_master_search');
        $custom  = NULL;  
        
        $fields = array(
                    'status'               => 'Inactive',
                    'updated_by'           => 1,
                    'updated_on'           => now()
                  );
        
        $updateMasterStmt->fields($fields);    
        $updateMasterStmt->condition('pk_id', $classroomIdArr, 'IN');
        $updateMasterStmt->condition('type', 'Classroom', '=');
        $numMasterUpdated = $updateMasterStmt->execute();    
    
        expDebug::dPrint(' $numUpdated = ' . print_r($form_state['values']['classroom_id_'.$i], true) , 5);
    }
  } catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('deleteClassroomDetails', $ex);
    expertusErrorThrow($ex);
  }

  unset($txn);
  
}

function displayClassroomDetails(&$form, &$form_state, $entityId, $entityType, $uniqueId, $resourceType){
  try {
      $classroomDetails = array();
      if (empty($form_state['values']) && !empty($entityId)) { 
        $classroomDetails = getFacilityClassroomDetails($entityId, $entityType);
        $form_state['num_classroom'] = count($classroomDetails);
        $collapsedStatus = count($classroomDetails) > 0 ? TRUE : FALSE;    
      }
      
      if($resourceType == 'Classroom') {
        $collapsedStatus = FALSE;
      }
      if($form_state['display_class'] == TRUE){    
        
        if (!empty($form_state['executed']) && $form_state['triggering_element']['#value'] == 'Save') {
          
            $classroomDetails = getFacilityClassroomDetails($entityId, $entityType);
            $hideShowClass = (count($classroomDetails) > 0 || $form_state['num_classroom'] > 0) ? '' : 'disable-class';
        } else{
           $hideShowClass = '';
        }
      }else{
         $hideShowClass = count($classroomDetails) > 0 ? '' : 'disable-class';
      }
      
      $form['attr_action'] = array(
        '#type' => 'hidden',
        '#name' => 'attr_action',
        '#value' => 'classroom',
      );
      
      $form['classroom_fieldset'] = array(
        '#type' => 'fieldset',
        '#title' => t('Classroom'),
        '#prefix' => '<div id="classroom-fieldset-wrapper-'.$uniqueId.'" class="admin-tab-list-row classroom-fieldset-wrapper '.$hideShowClass.'">',
        '#suffix' => '</div>',
        '#collapsible' => TRUE,
        '#collapsed' => $collapsedStatus
      );
    
      
      if (!empty($entityId)) { 
      		$classroomDetails = getFacilityClassroomDetails($entityId, $entityType);
      }
      
      if(count($classroomDetails) > 0){
        unset($form['list_type_'.$uniqueId]['#options']['classroom_fieldset']);
        if(count($form['list_type_'.$uniqueId]['#options']) == 1){
          unset($form['list_type_'.$uniqueId]);
        }
      }
    
      $classroomCount = $form_state['num_classroom'];
      if (empty($form_state['num_classroom']) || $form_state['num_classroom'] == 0) {
        //$form_state['num_classroom'] = 1;
        $classroomCount=1;
      }  
      
      for ($i = 0,$j=0; $i < $classroomCount; $i++) {
        
      		// Added by Vincent to fix delete element issue
          if(in_array( $classroomDetails[$j]->classroom_id, $form_state['delete_classroom_fieldset'])){
          	$j++;
          }
          if(!in_array( $i, $form_state['delete_classroom_index'])){
    
            addHiddenValueToAddEditForm($form, 'classroom_id_'.$i, $classroomDetails[$j]->classroom_id);
            
            $form['classroom_fieldset']['classroom']['clsroom_name_'.$i] = array(
              '#type' => 'textfield',  
              '#prefix' => '<div class="classroom-details-row admin-text-field-column" ><table width="100%" cellpadding="3" cellspacing="0"><tr><td class="components-sub-title">'.t("LBL107").' :</td><td>',
              '#suffix' => '</td>',  	
              '#default_value' => $classroomDetails[$j]->clsroom_title,  
              '#size' => 20,
              '#maxlength' => 100,  	
              '#attributes' => array('class' => array('input-field-set input-classroom-set input-name-field')),
            );
              
            $form['classroom_fieldset']['classroom']['clsroom_max_capacity_'.$i] = array(
              '#type' => 'textfield',
              '#prefix' => '<td class="components-sub-title">'.t("LBL318").' :</td><td>',
              '#suffix' => '</td>',
              '#default_value' => $classroomDetails[$j]->max_capacity,
              '#size' => 5,
              '#maxlength' => 10,
              '#attributes' => array('class' => array('input-field-set input-classroom-set input-capacity-field')),
            );
            
            $form['classroom_fieldset']['classroom']['clsroom_status_'.$i] = array(
              '#type' => 'select',
              '#prefix' => '<td class="components-sub-title">'.t("LBL102").' :</td><td>',
              '#suffix' => '</td>',  
              '#options'=> getProfileItemNamesKeyed('lrn_res_rms_'),
              '#default_value' => $classroomDetails[$j]->status,  
              '#attributes' => array('class' => array('addedit-edit-select')),
            );
              
           /* $form['classroom_fieldset']['classroom']['clsroom_type_'.$i] = array(
              '#type' => 'select',
              '#prefix' => '<td class="components-sub-title">Type :</td><td>',
              '#suffix' => '</td>',
              '#options'=> getProfileItemNamesKeyed('lrn_res_rmt_'),
              '#default_value' => $classroomDetails[$j]->clsroom_type,
              '#attributes' => array('class' => array('addedit-edit-select')),
            ); */
    
            // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
            $delClassroomAttributes['class'][] = 'addedit-form-expertusone-throbber';
            $delClassroomAttributes['class'][] = 'admin-save-button-middle-bg';
            $delClassroomAttributes['class'][] = 'admin-delete-img';
            // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
            $delClassroomAttributes['data-wrapperid'] = array('classroom-fieldset-wrapper-'.$uniqueId);
      
            $form['classroom_fieldset']['classroom']['classroom_delete_'.$i] = array(
              '#type' => 'image_button', '#src' => base_path().'sites/all/themes/core/expertusone/expertusone-internals/images/close.png',
              '#title' => t('LBL286'),
              '#name' => 'Delete Classroom'.$i,
              '#prefix' => '<td style="width: 50px"><div class="admin-add-button-container">',
              '#suffix' => '</div></td></tr></table></div>',    
              '#submit' => array('deleteClassroom'),
              '#attributes' => $delClassroomAttributes,
              '#ajax' => array(
                'callback' => 'classroom_add_more_btn_callback',
                'wrapper' => 'classroom-fieldset-wrapper-'.$uniqueId,
                'param' =>$classroomDetails[$j]->classroom_id,
                'loop_id' =>$i
              ),  
            );
            $j++;
          }
      }
      
      $form['classroom_fieldset']['classroom_hidden_delete'] = array(
        '#type' => 'value',
        '#value' => implode(",",$form_state['delete_classroom_fieldset'])    
      );
      
      // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
      $addClassroomAttributes['class'][] = 'addedit-form-expertusone-throbber';
      $addClassroomAttributes['class'][] = 'admin-save-button-middle-bg';
      // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
      $addClassroomAttributes['data-wrapperid'] = array('classroom-fieldset-wrapper-'.$uniqueId);  
    
      $form['classroom_fieldset']['add_classroom'] = array(
        '#type' => 'submit',
        '#value' => t('LBL386'),
        '#name' => 'Add Classroom',
        '#prefix' => '<div class="add-more-classroom admin-add-more-field-column" id="add-more-classroom"><table cellpadding="2" cellspacing="2" border="0"><tr><td><div class="admin-save-button-left-bg"></div>',
        '#suffix' => '<div class="admin-save-button-right-bg"></div></td></tr></table></div>',
        '#submit' => array('classroom_add_row'),
      	'#attributes' => $addClassroomAttributes,
        '#ajax' => array(
          'callback' => 'classroom_add_more_btn_callback',
          'wrapper' => 'classroom-fieldset-wrapper-'.$uniqueId,
        ),
      );
      
  } catch (Exception $ex) {
    watchdog_exception('displayClassroomDetails', $ex);
    expertusErrorThrow($ex);
  }       

}
function addNewClassroomByRestAPI($entityId, &$form_state,$createuserid="") 
{
  try {
    drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	$form=array();
	$form["form_id"]="";
	$classroomId =  addNewClassroomDetails($form, $form_state,$entityId,'',1,$createuserid,1);
	expDebug::dPrint(' addNewClassroomByRestAPI created classroom:' . print_r($classroomId,true) , 4);
    return $classroomId;
  } catch (Exception $ex) {
    watchdog_exception('addNewClassroomByRestAPI', $ex);
    expertusErrorThrow($ex);
  }         
}

function updateClassroomByRestAPI($entityId, &$form_state,$createuserid="") 
{
  try {
    drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	$form=array();
	$form["form_id"]="";
	$classroomId =  updateClassroomDetails($form, $form_state,$entityId,'',1,$createuserid,1);
	expDebug::dPrint(' updateClassroomByRestAPI updated classroom:' . print_r($classroomId,true) , 4);
    return $classroomId;
  } catch (Exception $ex) {
    watchdog_exception('updateClassroomByRestAPI', $ex);
    expertusErrorThrow($ex);
  }         
}
?>