<?php

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the names in it.
 */
function equipment_add_more_btn_callback($form, $form_state) {
  try {
    return $form['equipment_fieldset'];
  } catch (Exception $ex) {
    watchdog_exception('equipment_add_more_btn_callback', $ex);
    expertusErrorThrow($ex);
  } 
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function equipment_add_row($form, &$form_state) {
  try {
  	if($form_state['num_equipment']==0){ // Added by Vincent on 24 Mar,2012 - to fix first time load after save shows readonly
      $form_state['num_equipment']=1;
    }
    $form_state['num_equipment']++;
    $form_state['display_class'] = TRUE;
    $form_state['rebuild'] = TRUE;
  } catch (Exception $ex) {
    watchdog_exception('equipment_add_row', $ex);
    expertusErrorThrow($ex);
  } 
}

function equipmentOperations($form, $form_state,$entityId,$entityType){
  try {
      deleteEquipmentDetails($form, $form_state,$entityId,$entityType);
      $cnt = $form_state['num_equipment']==0?1:$form_state['num_equipment'];
      for ($i = 0; $i < $cnt; $i++) {
        $equipmentName = $form_state['values']['equipment_fieldset']['equipment']['equip_name_'.$i];
        $equipmentMaxCapacity  = $form_state['values']['equipment_fieldset']['equipment']['equip_status_'.$i];
        if(!empty($equipmentName) && !empty($equipmentMaxCapacity)) {
          if(empty($form_state['values']['equipment_id_'.$i])) {
          	addNewEquipmentDetails($form, $form_state,$entityId,$entityType,$i);
          }else{
          	updateEquipmentDetails($form, $form_state,$entityId,$entityType,$i);
          }
        }
      }
  } catch (Exception $ex) {
    watchdog_exception('equipmentOperations', $ex);
    expertusErrorThrow($ex);
  } 
}


function getFacilityEquipmentDetails($entityId,$entityType) {
  try {
      $select = db_select("slt_equipment","eq");
      $select->addField('eq','id','equipment_id');
      $select->addField('eq','facility_id','facility_id');
      $select->addField('eq','name','title');
      $select->addField('eq','partno','part_no');
      $select->addField('eq','vendor','vendor');
      $select->addField('eq','manuf','manuf');
      $select->addField('eq','is_active','is_active');
      $select->addField('eq','status','status');
      $select->condition('eq.facility_id',$entityId,'=');
      $select->condition('eq.is_active',1,'=');
      
      $equipmentList = $select->execute()->fetchAll();
      
      return $equipmentList;
  } catch (Exception $ex) {
    watchdog_exception('getFacilityEquipmentDetails', $ex);
    expertusErrorThrow($ex);
  }       
}


function addNewEquipmentDetails($form, &$form_state,$entityId,$entityType,$i) { 
	
  $txn = db_transaction();

  try {

    	
      $insertStmt = db_insert('slt_equipment');
      $custom  = NULL;
      $currentUserId = getIdOfLoggedInUser();
      $fields = array(
                'facility_id'          => $entityId,                  
                'name'                 => $form_state['values']['equipment_fieldset']['equipment']['equip_name_'.$i],
                'partno'     	         => '',
                'vendor'               => '',
                'manuf'                => '',
                'is_active'            => 1,
      			    //'status'               => $equipStatus,
                'created_by'           => $currentUserId,
                'created_on'           => now(),
      					'updated_by'           => $currentUserId,
      					'updated_on'           => now(),
                'custom0'              => $custom,
                'custom1'              => $custom,
                'custom2'              => $custom,
                'custom3'              => $custom,
                'custom4'              => $custom   
              );
      $insertStmt->fields($fields);			            
       
      $equipmentId = $insertStmt->execute();
      $insertequipform = $form_state['values'];
      $insertequipform[name] = $form_state['values']['equipment_fieldset']['equipment']['equip_name_'.$i]; 
      $insertequipform[equip_status] = $equipStatus;  
      
      make_audit_trail_entries($insertequipform, getIdOfLoggedInUser() , null, $equipmentId ,'Inserted','cre_sys_obt_equ','exp_sp_administration_resources','addNewEquipmentDetails',
                                          array('facility_id', 'name', 'equip_status'));
     
    	   
      expDebug::dPrint(' $$equipmentId = ' . print_r($equipmentId, true) , 3);  
     
      return array((object)array('id'=>$equipmentId));
  }
  catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('addNewEquipmentDetails', $ex);
    expertusErrorThrow($ex);
    //throw $ex;    
  }
  
  unset($txn);
  
}


function updateEquipmentDetails($form, &$form_state,$entityId,$entityType,$i) {
  
  $txn = db_transaction();

  try {
    
    $updateStmt = db_update('slt_equipment');
    $default= db_query("SELECT id,facility_id,name as equip_name,status as equip_status from slt_equipment where id=:uid",array(":uid"=>$form_state['values']['equipment_id_'.$i]))->fetchAll();
    $custom  = NULL;  

    $fields = array(
                'facility_id'          => $entityId,                  
                'name'                 => $form_state['values']['equipment_fieldset']['equipment']['equip_name_'.$i],
                'partno'     	       => '',
                'vendor'               => '',
                'manuf'                => '',
      			    //'status'               => $equipStatus,
                'updated_by'           => 1,
                'updated_on'           => now(),
                'custom0'              => $custom,
                'custom1'              => $custom,
                'custom2'              => $custom,
                'custom3'              => $custom,
                'custom4'              => $custom   
              );
    
    $updateStmt->fields($fields);    
    $updateStmt->condition('id', $form_state['values']['equipment_id_'.$i]);
    $numUpdated = $updateStmt->execute();
    $updateequipform = $form_state['values']; 
    $updateequipform[facility_id] = $entityId;
    $updateequipform[equip_name] = $form_state['values']['equipment_fieldset']['equipment']['equip_name_'.$i];
    $updateequipform[equip_status] = $equipStatus;
      expDebug::dPrint(' $$updateequipform = ' . print_r($updateequipform, true) , 4);
      make_audit_trail_entries($updateequipform, getIdOfLoggedInUser() , $default, $entityId ,'Updated','cre_sys_obt_equ','exp_sp_administration_resources','updateEquipmentDetails',
                                          array('facility_id', 'equip_name', 'equip_status'));
      	
    expDebug::dPrint(' $numUpdated = ' . print_r($numUpdated, true) , 3);
      
  }
  catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('updateEquipmentDetails', $ex);
    expertusErrorThrow($ex);
    //throw $ex;
  }

  unset($txn);
  
}

/**
 * Submit handler for the "Delete" button.
 *
 * Decrements the Deleted value and causes a form rebuild.
 */
function deleteEquipment($form, &$form_state) {  
  try {
      $equipmentId = $form_state['triggering_element']['#ajax']['param'];
      $indexId = $form_state['triggering_element']['#ajax']['loop_id'];
      $form_state['delete_equipment_fieldset'][] = $equipmentId;    
      $form_state['delete_equipment_index'][] = $indexId;
      $form_state['display_class'] = TRUE;
    	if($form_state['num_equipment']==count($form_state['delete_equipment_index']) || $form_state['num_equipment']==0){
        equipment_add_row($form, $form_state);
      }    
      $form_state['rebuild'] = TRUE;
  } catch (Exception $ex) {
    watchdog_exception('deleteEquipment', $ex);
    expertusErrorThrow($ex);
  } 
}


function deleteEquipmentDetails($form, &$form_state,$entityId,$entityType) {
  
  $equipmentId = $form_state['values']['equipment_fieldset']['equipment_hidden_delete'];
  
  $equipmentIdArr = explode(',', $equipmentId);
 
  $txn = db_transaction();
  
  try {
    if(!empty($equipmentId)){
      
        // Delete or Inactive the records from slt_classroom
        $deleteStmt = db_update('slt_equipment');
        $custom  = NULL;  
        
        $fields = array(
                    'is_active'               => 0
                  );
        
        $deleteStmt->fields($fields);    
        $deleteStmt->condition('id', $equipmentIdArr, 'IN');
        $numUpdated = $deleteStmt->execute();
    
        
        /*UPDATE slt_equipment 
             SET 
             `is_active` = '0' 
        WHERE `slt_equipment`.`id` = xid LIMIT 1 ;
        
        UPDATE slt_master_search 
        	SET STATUS 	= 'Inactive',
        	updated_on 	= NOW(),
        	updated_by 	= xUpdatedBy  
        	WHERE pk_id=xid AND TYPE = 'Equipment';*/
        
        //Delete or Inactive the records from slt_master_search
        $updateMasterStmt = db_update('slt_master_search');
        $custom  = NULL;  
        
        $fields = array(
                    'status'               => 'Inactive',
                    'updated_by'           => 1,
                    'updated_on'           => now()
                  );
        
        $updateMasterStmt->fields($fields);    
        $updateMasterStmt->condition('pk_id', $equipmentIdArr, 'IN');
        $updateMasterStmt->condition('type', 'Equipment', '=');
        $numMasterUpdated = $updateMasterStmt->execute();        
	        
        expDebug::dPrint(' $numUpdated = ' . print_r($form_state['values']['equipment_id_'.$i], true) , 5);
    }
  }
  catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('deleteEquipmentDetails', $ex);
    expertusErrorThrow($ex);
    //throw $ex;
  }

  unset($txn);
  
}

function displayEquipmentDetails(&$form, &$form_state, $entityId, $entityType, $uniqueId, $resourceType){
  try {
    $equipmentDetails = array();
    if (empty($form_state['values']) && !empty($entityId)) { 
      $equipmentDetails = getFacilityEquipmentDetails($entityId, $entityType);
      $form_state['num_equipment'] = count($equipmentDetails);
      $collapsedStatus = count($equipmentDetails) > 0 ? TRUE : FALSE;    
    }
    
    if($resourceType == 'Equipment') {
      $collapsedStatus = FALSE;
    } 
    
    if($form_state['display_class'] == TRUE){    
      
      if (!empty($form_state['executed']) && $form_state['triggering_element']['#value'] == 'Save') {
        
          $equipmentDetails = getFacilityEquipmentDetails($entityId, $entityType);
          $hideShowClass = (count($equipmentDetails) > 0 || $form_state['num_equipment'] > 0) ? '' : 'disable-class';
      } else{
         $hideShowClass = '';
      }
    }else{
       $hideShowClass = count($equipmentDetails) > 0 ? '' : 'disable-class';
    }
    
    $form['attr_action'] = array(
      '#type' => 'hidden',
      '#name' => 'attr_action',
      '#value' => 'equipment',
    );
    
    $form['equipment_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Equipment'),
      '#prefix' => '<div id="equipment-fieldset-wrapper-'.$uniqueId.'" class="admin-tab-list-row equipment-fieldset-wrapper '.$hideShowClass.'">',
      '#suffix' => '</div>',
      '#collapsible' => TRUE,
      '#collapsed' => $collapsedStatus
    );
  
    
    if (!empty($entityId)) { 
    		$equipmentDetails = getFacilityEquipmentDetails($entityId, $entityType);
    }
    
    if(count($equipmentDetails) > 0){
      unset($form['list_type_'.$uniqueId]['#options']['equipment_fieldset']);
      if(count($form['list_type_'.$uniqueId]['#options']) == 1){
        unset($form['list_type_'.$uniqueId]);
      }
    }
  
    $equipmentCount = $form_state['num_equipment'];
    if (empty($form_state['num_equipment']) || $form_state['num_equipment'] == 0) {
      //$form_state['num_equipment'] = 1;
      $equipmentCount=1;
    }  
    
    for ($i = 0,$j=0; $i < $equipmentCount; $i++) {
      
    		// Added by Vincent to fix delete element issue
        if(in_array( $equipmentDetails[$j]->equipment_id, $form_state['delete_equipment_fieldset'])){
        	$j++;
        }
        if(!in_array( $i, $form_state['delete_equipment_index'])){
  
          addHiddenValueToAddEditForm($form, 'equipment_id_'.$i, $equipmentDetails[$j]->equipment_id);
          
          $form['equipment_fieldset']['equipment']['equip_name_'.$i] = array(
            '#type' => 'textfield',   
            '#prefix' => '<div class="equipment-details-row admin-text-field-column" ><table width="100%" cellpadding="2" cellspacing="2" border="0"><tr><td class="components-sub-title">'.t("LBL107").' :</td><td>',
            '#suffix' => '</td>',  	
            '#default_value' => $equipmentDetails[$j]->title,  
            '#size' => 20,
            '#maxlength' => 100,  	
            '#attributes' => array('class' => array('input-field-set input-equipment-set')),
          );
            
          $form['equipment_fieldset']['equipment']['equip_status_'.$i] = array(
            '#type' => 'select',
            '#prefix' => '<td class="components-sub-title">'.t("LBL102").' :</td><td>',   
            '#suffix' => '</td>',
            '#options'=> array(),//getProfileItemNamesKeyed('lrn_res_eqs_'),
            '#default_value' => $equipmentDetails[$j]->status,
            '#attributes' => array('class' => array('addedit-edit-select')),
          );
  
          // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
          $delEquipmentAttributes['class'][] = 'addedit-form-expertusone-throbber';
          $delEquipmentAttributes['class'][] = 'admin-save-button-middle-bg';
          $delEquipmentAttributes['class'][] = 'admin-delete-img';
          // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
          $delEquipmentAttributes['data-wrapperid'] = array('equipment-fieldset-wrapper-'.$uniqueId);
    
          $form['equipment_fieldset']['equipment']['equipment_delete_'.$i] = array(
            '#type' => 'image_button', '#src' => base_path().'sites/all/themes/core/expertusone/expertusone-internals/images/close.png',
            '#title' => t('LBL286'),
            '#name' => 'Delete Equipment'.$i,
            '#prefix' => '<td style="width: 41px;"><div class="admin-add-button-container">',
            '#suffix' => '</div></td></tr></table></div>',    
            '#submit' => array('deleteEquipment'),
            '#attributes' => $delEquipmentAttributes,
            '#ajax' => array(
              'callback' => 'equipment_add_more_btn_callback',
              'wrapper' => 'equipment-fieldset-wrapper-'.$uniqueId,
              'param' =>$equipmentDetails[$j]->equipment_id,
              'loop_id' =>$i
            ),  
          );
          $j++;
        }
    }
    
    $form['equipment_fieldset']['equipment_hidden_delete'] = array(
      '#type' => 'value',
      '#value' => implode(",",$form_state['delete_equipment_fieldset'])    
    );
    
    // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
    $addEquipmentAttributes['class'][] = 'addedit-form-expertusone-throbber';
    $addEquipmentAttributes['class'][] = 'admin-save-button-middle-bg';
    // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
    $addEquipmentAttributes['data-wrapperid'] = array('equipment-fieldset-wrapper-'.$uniqueId);  
  
    $form['equipment_fieldset']['add_equipment'] = array(
      '#type' => 'submit',
      '#value' => t('LBL386'),
      '#name' => 'Add Equipment',
      '#prefix' => '<div class="add-more-equipment admin-add-more-field-column" id="add-more-equipment"><table cellpadding="2" cellspacing="2" border="0"><tr><td><div class="admin-save-button-left-bg"></div>',
      '#suffix' => '<div class="admin-save-button-right-bg"></div></td></tr></table></div>',
      '#submit' => array('equipment_add_row'),
    	'#attributes' => $addEquipmentAttributes,
      '#ajax' => array(
        'callback' => 'equipment_add_more_btn_callback',
        'wrapper' => 'equipment-fieldset-wrapper-'.$uniqueId,
      ),
    );
  } catch (Exception $ex) {
    watchdog_exception('displayEquipmentDetails', $ex);
    expertusErrorThrow($ex);
  } 
}
?>