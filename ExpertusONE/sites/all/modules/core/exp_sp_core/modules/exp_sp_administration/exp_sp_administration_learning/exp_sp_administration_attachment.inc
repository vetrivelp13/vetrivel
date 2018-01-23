<?php

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the names in it.
 */
function attach_add_more_btn_callback($form, $form_state) {
	try {
  return $form['attachment_fieldset'];
  } catch (Exception $ex) {
  	watchdog_exception('attach_add_more_btn_callback', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function attach_add_row($form, &$form_state, $deleteCall = 0) {
 try { 
  if($deleteCall == 0){
    $numCount = $form_state['triggering_element']['#ajax']['num_count'];
    $form_state['num_attachment'] = $numCount;
  }
  
  if($form_state['num_attachment']==0){ // Added by Vincent on 24 Mar,2012 - to fix first time load after save shows readonly
    $form_state['num_attachment']=1;
  }
  $form_state['show_add_another'] = FALSE;
  $form_state['num_attachment']++;
  $form_state['display_class'] = TRUE;
  $form_state['rebuild'] = TRUE;
  } catch (Exception $ex) {
  	watchdog_exception('attach_add_row', $ex);
  	expertusErrorThrow($ex);
  }
}

function attachmentOperations($form, $form_state,$entityId,$entityType){
	try {
  deleteAttachmentDetails($form, $form_state,$entityId,$entityType);
  $cnt = $form_state['num_attachment']==0?1:$form_state['num_attachment']; //Add by Vincent
  $isAdded = 0;
  $isUpdated = 0;
  $isUploadError = 1;
  $form_state['show_add_another'] = TRUE;
  expDebug::dPrint(' $cnt'.$cnt , 4);
  for ($i = 0; $i < $cnt; $i++) {
    $attachmentName = $form_state['values']['attachment_fieldset']['attachment']['attachment_name_'.$i];
    $attachmentType = $form_state['values']['attachment_fieldset']['attachment']['attachment_radioname_'.$i];
    expDebug::dPrint(' Attachment type ' .$attachmentType , 5);
    if($attachmentType == 'URL'){
       if(empty($form_state['values']['attachment_id_'.$i])) {
         $attachmentURL = $form_state['values']['attachment_fieldset']['attachment']['attachment_url_'.$i];
       }else{ 
      $attachmentURL = $form_state['values']['attachment_fieldset']['attachment']['attachment_url_upd_'.$i];
       }
    }
    else{
      $attachmentURL = 'attachment_fieldset';//$form_state['values']['attachment_fieldset']['attachment']['attachment_url_browse_'.$i];      
    }
    expDebug::dPrint(' attachmentURL ' .$attachmentName.'-url-'.$attachmentURL , 4);
    expDebug::dPrint(' attachmentURL ' .print_r($form_state['values'],true) , 5);
    if(!empty($attachmentName) && !empty($attachmentURL) && $attachmentName != 'Attachment Name' && $attachmentURL !='Attachment URL') {
       expDebug::dPrint(' Operation add' .$isAddAttachment[0]->id.'updae'.$isUpdated.'added'.$isAdded.'eroor'.$isUploadError , 4);
      if(empty($form_state['values']['attachment_id_'.$i])) {
        expDebug::dPrint(' Operation add');
      	$isAddAttachment = addNewAttachmentDetails($form, $form_state,$entityId,$entityType,$i);
      	expDebug::dPrint(' Operation add' .$isAddAttachment[0]->id , 4);      	
      	$isAdded = $isAddAttachment[0]->id;
      	$isUploadError  = $isAddAttachment[0]->id;    	
      }else{
         expDebug::dPrint(' Operation add' .$isAddAttachment[0]->id.'updae'.$isUpdated.'added'.$isAdded.'eroor'.$isUploadError , 4);
        expDebug::dPrint(' Operation Update');
        updateAttachmentDetails($form, $form_state,$entityId,$entityType,$i);
		$isUpdated = 1;
      }
    }
  }
  if($isAdded){
    drupal_set_message(t('Attachment added successfully.'));
    $form_state['storage']['new_entity'] = TRUE;
  }
  elseif($isUpdated && ($isUploadError)){
    drupal_set_message(t('Attachment updated successfully.'));
  }
  } catch (Exception $ex) {
  	watchdog_exception('attachmentOperations', $ex);
  	expertusErrorThrow($ex);
  }
}


function getAttachDetails($entityId,$entityType) {
 try { 
  $select = db_select("slt_entity_reading_mapper","attach");
  $select->addField('attach','id','attachment_id');
  $select->addField('attach','reading_id','reading_id');
  $select->addField('attach','reading_type','reading_type');
  $select->addField('attach','reading_content','reading_content');
  $select->addField('attach','created_on','created_on');
  $select->addField('attach','updated_on','updated_on');
  $select->addField('attach','custom3','is_file');
  $select->addField('attach','custom4','custom4');
  $select->condition('attach.entity_id',$entityId,'=');
  $select->condition('attach.entity_type',$entityType,'=');	
  //$select->condition('attach.custom0',null,'=');
  
  $attachmentList = $select->execute()->fetchAll();
  
  return $attachmentList;
  } catch (Exception $ex) {
  	watchdog_exception('getAttachDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function getAttachmentCount ($entityId,$entityType) {
	try {
  $select = db_select('slt_entity_reading_mapper','reading');
  $select->addExpression('COUNT(reading.id)', 'cnt');
  $select->condition('reading.entity_id', $entityId,'=');
  $select->condition('reading.entity_type', $entityType,'=');
  
  $attachmentMaxCnt = $select->execute()->fetchField();
  
  expDebug::dPrintDBAPI(' $$$attachmentMaxCnt = ' , $select); 
  expDebug::dPrint(' $$$attachmentMaxCnt = ' . print_r($attachmentMaxCnt, true) , 3);
  
  if($attachmentMaxCnt > 0) {		
    $xreadingid = getAttachmentMaxReadingId($entityId,$entityType);		
  }else{
    $xreadingid = 1;
  }      
  return $xreadingid;
  } catch (Exception $ex) {
  	watchdog_exception('getAttachmentCount', $ex);
  	expertusErrorThrow($ex);
  }
}

function getAttachmentMaxReadingId ($entityId,$entityType) {
	try {
  $select = db_select('slt_entity_reading_mapper','reading');
  $select->addExpression('MAX(reading.reading_id) + 1', 'cnt');
  $select->condition('reading.entity_id', $entityId,'=');
  $select->condition('reading.entity_type', $entityType,'=');
  
  $attachmentMaxReadingId = $select->execute()->fetchField();
  
  return $attachmentMaxReadingId;
  } catch (Exception $ex) {
  	watchdog_exception('getAttachmentMaxReadingId', $ex);
  	expertusErrorThrow($ex);
  }
}

function addNewAttachmentDetails($form, &$form_state,$entityId,$entityType,$i) { 
  $txn = db_transaction();

  try {
      	
      $xreadingid = getAttachmentCount($entityId,$entityType);
      $currentUserId = getIdOfLoggedInUser();
      $attachmentType = $form_state['values']['attachment_fieldset']['attachment']['attachment_radioname_'.$i];
      $isUpload = 0;
      if($attachmentType == 'FILE'){
        $isUpload = 1;
      }
      expDebug::dPrint('addNewAttachmentDetails reading_content'. $reading_content , 4);
      if($isUpload){
         include_once $_SERVER["DOCUMENT_ROOT"]."/includes/file.inc";
         $validators  = array(
           'file_validate_extensions' => array('txt doc docx ppt pptx xls xlsx pdf png jpg jpeg gif'), // Validate extensions.
          // 'file_validate_is_image' => array(), // Validates file is really an image.
         // 'file_validate_image_resolution' => array('85x85'),
         );
    
         $attachment = file_save_upload('attachment_fieldset',$validators);
         expDebug::dPrint(' $picture returned by file_save_upload() = ' . print_r($attachment, true) , 4);
        
          if(!empty($attachment)) {
            if (!$attachment->status) { // i.e. if picture is a temporary file
              // Prepare the pictures directory.
              $filedirectory = file_default_scheme() . '://' . variable_get('user_attachment_path', 'attachment');
              expDebug::dPrint(' $filedirectory = ' . print_r($filedirectory, true) , 4);
              file_prepare_directory($filedirectory, FILE_CREATE_DIRECTORY);
              
              // Prepare the unique file name to place the picture under the $filedirectory.
              expDebug::dPrint(' $info returned by image_get_info = ' . print_r($info, true) , 4);
              $destination = file_stream_wrapper_uri_normalize($filedirectory . '/'.REQUEST_TIME . '-' .$attachment->filename);
              expDebug::dPrint(' $destination = ' . print_r($destination, true) , 4);
              
              // Move the temporary file into the final location.
              if ($attachmentmove = file_move($attachment, $destination, FILE_EXISTS_RENAME)) {
                expDebug::dPrint(' $picture returned by file_move() = ' . print_r($attachmentmove, true) , 4);
             
                $attachmentmove->status = FILE_STATUS_PERMANENT;
                $save_attachment = file_save($attachmentmove);
                expDebug::dPrint(' $picture returned by file_save() = ' . print_r($save_attachment, true) , 4);
                
              }
            }
          }
       }
     $is_attachable = 1; 
     if($attachmentType == 'URL'){
        $reading_content = $form_state['values']['attachment_fieldset']['attachment']['attachment_url_'.$i];
        $reading_type = 'URL';
     }
     else{
       $config=getConfig("exp_sp");
       $admin_site_url = $config['admin_site_url'];
       $reading_content = '/sites/default/files/attachment/' . $save_attachment->filename;
       $reading_type = 'FILE';  
       $is_attachable = ($save_attachment->filename) ? 1 : 0;            
     }
     if(!empty($reading_content) && $is_attachable){	
       $insertStmt = db_insert('slt_entity_reading_mapper');
        $custom  = NULL;
      	
        $fields = array(
                  'entity_id'          => $entityId,                  
                  'entity_type'        => $entityType,
                  'reading_id'     	   => $xreadingid,
                  'reading_type'       => $form_state['values']['attachment_fieldset']['attachment']['attachment_name_'.$i],
                  'reading_content'    => $reading_content,
                  'showlnr'            => 'Y',
                  'created_by'         => $currentUserId,
                  'created_on'         => now(),
        					'updated_by'         => $currentUserId,
        					'updated_on'         => now(),
                  'custom0'            => $custom,
                  'custom1'            => $custom,
                  'custom2'            => $custom,
                  'custom3'            => $reading_type,
                  'custom4'            => $custom   
                );
        $insertStmt->fields($fields);			            
         
        $attachmentId = $insertStmt->execute();
        $attFormState = array();
        $attFormState['id'] = $attachmentId;
        $attFormState['reading_type'] =  $form_state['values']['attachment_fieldset']['attachment']['attachment_name_'.$i];
        $attFormState['reading_content'] = $reading_content;
        $attFormState['reading_id'] = $xreadingid;
        $attFormState['custom3'] = $reading_type;
        
        make_audit_trail_entries($attFormState, getIdOfLoggedInUser() , null ,$entityId, 'Inserted',$entityType,'exp_sp_administration_attachment','new attachment',
                                          array('id', 'reading_type', 'reading_content', 'reading_id', 'custom3'));
        expDebug::dPrint(' $$attachmentId = ' . print_r($attachmentId, true) , 3);
        
        return array((object)array('id'=>$attachmentId));
     }
     else{
       drupal_set_message(t('ERR105'), $type = 'error');
       return array((object)array('id'=>0));
     }
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('addNewAttachmentDetails', $ex);
    throw $ex;    
  }
  
  unset($txn);
  
}


function updateAttachmentDetails($form, &$form_state,$entityId,$entityType,$i) {
  
  $txn = db_transaction();
  
  try {
	$default= db_query("SELECT id,reading_type,reading_content,custom3 from slt_entity_reading_mapper where id=:rid",array(":rid"=> $form_state['values']['attachment_id_'.$i]))->fetchAll();
    $updateStmt = db_update('slt_entity_reading_mapper');
    $custom  = NULL;  
    $attachmentType = $form_state['values']['attachment_fieldset']['attachment']['attachment_radioname_'.$i];
    if($attachmentType == 'URL'){
      $reading_type = 'URL';
    }
    else{
      $reading_type = 'FILE';              
    }
    $fields = array(
                  'reading_type'       => $form_state['values']['attachment_fieldset']['attachment']['attachment_name_'.$i],
                  'reading_content'    => $form_state['values']['attachment_fieldset']['attachment']['attachment_url_upd_'.$i],
                  'showlnr'            => 'Y',
                  'updated_by'         => 1,
                  'updated_on'         => now(),
                  'custom0'            => $custom,
                  'custom1'            => $custom,
                  'custom2'            => $custom,
                  'custom3'            => $reading_type,
                  'custom4'            => $custom   
                );
    
    $updateStmt->fields($fields);    
    $updateStmt->condition('id', $form_state['values']['attachment_id_'.$i]);
    $numUpdated = $updateStmt->execute();
    
    $editFormState = array();
    $editFormState['id'] = $form_state['values']['attachment_id_'.$i];
    $editFormState['reading_type'] = $form_state['values']['attachment_fieldset']['attachment']['attachment_name_'.$i];
    $editFormState['reading_content'] = $form_state['values']['attachment_fieldset']['attachment']['attachment_url_upd_'.$i];
    $editFormState['custom3'] = $reading_type;
    
    make_audit_trail_entries($editFormState, getIdOfLoggedInUser() , $default ,$entityId, 'Updated',$entityType,'exp_sp_administration_attachment','old attachment',
                                          array('id', 'reading_type', 'reading_content', 'reading_id', 'custom3'));
      	
    expDebug::dPrint(' $numUpdated = ' . print_r($numUpdated, true) , 3);
      
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('updateAttachmentDetails', $ex);
    throw $ex;
  }

  unset($txn);
  
}

/**
 * Submit handler for the "Delete" button.
 *
 * Decrements the Deleted value and causes a form rebuild.
 */
function delete_attachment($form, &$form_state) {
 try { 
  $numCount = $form_state['triggering_element']['#ajax']['num_count'];
  $form_state['num_attachment'] = $numCount;
      
  $attachmentId = $form_state['triggering_element']['#ajax']['param'];
  $indexId = $form_state['triggering_element']['#ajax']['loop_id'];
  $form_state['delete_attachment_fieldset'][] = $attachmentId;    
  $form_state['delete_attachment_index'][] = $indexId;
  $form_state['display_class'] = TRUE;
	if($form_state['num_attachment']==count($form_state['delete_attachment_index']) || $form_state['num_attachment']==0){
    attach_add_row($form, $form_state, 1);
  }    
  $form_state['rebuild'] = TRUE;
  } catch (Exception $ex) {
  	watchdog_exception('delete_attachment', $ex);
  	expertusErrorThrow($ex);
  }
}


function deleteAttachmentDetails($form, &$form_state,$entityId,$entityType) {
  
  $attachmentId = $form_state['values']['attachment_fieldset']['attach_hidden_delete'];
  
  $attachmentIdArr = explode(',', $attachmentId);
 
  $txn = db_transaction();
  
  try {
      expDebug::dPrint('inside attachments---');
    if(!empty($attachmentId)){
        
        
      /*  $delete_id = db_select('slt_entity_reading_mapper','r');
        $delete_id->addField('r','id','id');
        $delete_id->addField('r','entity_id','entity_id');
        $delete_id->addField('r','reading_id','reading_id');
        $delete_id->condition('id', $attachmentIdArr,'IN');
        expDebug::dPrint('slt_entity_reading_mapper table log----');
        expDebug::dPrintDBAPI('$delete_id',$delete_id);
        $del = $delete_id->execute()->fetchAll();*/
        
        $deleteStmt = db_delete('slt_entity_reading_mapper');
        $deleteStmt->condition('id', $attachmentIdArr,'IN');
        $numUpdated = $deleteStmt->execute();
        
        //Make delete log entries
        /*foreach ($del as $val){
            $fields = array(
                    'table_name' => 'slt_entity_reading_mapper',
                    'entity_id' => $val->id,
                    'entity_type' => 'attachments',
                    'module_name' => 'exp_sp_administration_attachment',
                    'functionality_name' => 'deleteAttachmentDetails',
                    'api_name' => null,
                    'deleted_on' => now(),
                    'deleted_by' => getIdOfLoggedInUser(),
                    'parent1_entity_id' => $val->entity_id,
                    'parent1_entity_type' => 'entity_id',
                    'parent2_entity_id' =>$val->reading_id,
                    'parent2_entity_type' => 'reading_id',
            );
            deleted_log_entry($fields);
        }expDebug::dPrint('slt_entity_reading_mapper table log');*/
        
        foreach($attachmentIdArr as $record){    
        	$deleteFormState = array();
        	$deleteFormState['id'] =  $record;  
        	make_audit_trail_entries($deleteFormState, getIdOfLoggedInUser() , null ,$entityId, 'Deleted',$entityType,'exp_sp_administration_attachment','old attachment',
                                          array('id'));    	
        }                               
        expDebug::dPrint(' $numUpdated = ' . print_r($form_state['values']['attachment_id_'.$i], true) , 5);
    }
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('deleteAttachmentDetails', $ex);
    throw $ex;
  }

  unset($txn);
  
}

function displayAttachmentDetails(&$form, &$form_state, $entityId, $entityType, $uniqueId){
	try {
  $attachmentDetails = array();
  if (empty($form_state['values']) && !empty($entityId)) { 
    $attachmentDetails = getAttachDetails($entityId, $entityType);
    $form_state['num_attachment'] = count($attachmentDetails);
    $colapasedStatus = count($attachmentDetails) > 0 ? TRUE : FALSE;    
  }
  
  if($form_state['display_class'] == TRUE){    
    if ((!empty($form_state['executed']) && $form_state['triggering_element']['#value'] == 'Save') || $form_state['storage']['step'] == 'edit') {
        $attachmentDetails = getAttachDetails($entityId, $entityType);
        $hideShowClass = (count($attachmentDetails) > 0 || $form_state['num_attachment'] > 0) ? '' : 'disable-class';
    } else{
       $hideShowClass = '';
    }
  }else{
     $hideShowClass = count($attachmentDetails) > 0 ? '' : 'disable-class';
  }

  $form['attr_action'] = array(
    '#type' => 'hidden',
    '#name' => 'attr_action',
    '#value' => 'attachment',
  );
/*  $form['attachment_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Attachments'),
    '#prefix' => '<div id="attachment-fieldset-wrapper-'.$uniqueId.'" class="admin-tab-list-row attachment-fieldset-wrapper">',
    //'#prefix' => '<div id="attachment-fieldset-wrapper-'.$uniqueId.'" class="admin-tab-list-row attachment-fieldset-wrapper '.$hideShowClass.'">',
    '#suffix' => '</div>',
    '#collapsible' => TRUE,
    //'#collapsed' => $colapasedStatus
  );*/
  
   $form['attachment_fieldset'] = array(
    '#type'       => 'markup',
    '#prefix' => '<div id="attachment-fieldset-wrapper-'.$uniqueId.'" class="admin-tab-list-row attachment-fieldset-wrapper">',
    //'#prefix' => '<div id="attachment-fieldset-wrapper-'.$uniqueId.'" class="admin-tab-list-row attachment-fieldset-wrapper '.$hideShowClass.'">',
    '#suffix' => '</div>',
    '#attributes' => array ('enctype' => "multipart/form-data" ),
   );
   $form['attachment_fieldset']['attachment_filetype'] = array(
     '#type' => 'markup',
     '#prefix' => '<div id="attachment_file_type" class= "attachment-file-type">'.t('Files with following extensions can be uploaded: txt, doc, docx, ppt, pptx, xls, xlsx, pdf, png, jpg, jpeg, gif.'),
     '#suffix' => '</div>',                                                                                                     
   );

  if (!empty($entityId)) { 
  		$attachmentDetails = getAttachDetails($entityId, $entityType);
  }
  
  if(count($attachmentDetails) > 0){
    unset($form['list_type_'.$uniqueId]['#options']['attachment_fieldset']);
    if(count($form['list_type_'.$uniqueId]['#options']) == 1){
      unset($form['list_type_'.$uniqueId]);
    }
  } else {
    $form_state['show_add_another'] = FALSE;
  }
/*  if (!empty($form_state['executed']) && $form_state['triggering_element']['#value'] == 'Save') {
      //$form_state['num_attachment'] = count($attachmentDetails);  
      expDebug::dPrint('count($attachmentDetails) : '.count($attachmentDetails));
  }  */
  $attachmentCount = $form_state['num_attachment'];
  if (empty($form_state['num_attachment']) || $form_state['num_attachment'] == 0) {
    //$form_state['num_attachment'] = 1;
    $attachmentCount=1;
  }  
  
  expDebug::dPrint('$form_state[num_attachment] : '.print_r($form_state['num_attachment'],true) , 5);
  expDebug::dPrint('$form_state[delete_attachment_fieldset] : '.print_r($form_state['delete_attachment_fieldset'],true) , 5);
  expDebug::dPrint('$form_state[delete_attachment_index] : '.print_r($form_state['delete_attachment_index'],true) , 5);
  expDebug::dPrint('$attachmentDetails : '.print_r($attachmentDetails,true) , 4);
  
  for ($i = 0,$j=0; $i < $attachmentCount; $i++) {
    
  		// Added by Vincent to fix delete element issue
      if(in_array( $attachmentDetails[$j]->attachment_id, $form_state['delete_attachment_fieldset'])){
      	$j++;
      }
      if(!in_array( $i, $form_state['delete_attachment_index'])){
        
        addHiddenValueToAddEditForm($form, 'attachment_id_'.$i, $attachmentDetails[$j]->attachment_id);
        
        $readingType = $attachmentDetails[$j]->reading_type;
        $readingContent = $attachmentDetails[$j]->reading_content;
        
        // Field label Commented - Can be added later  
        if((!empty($attachmentDetails[$j]->reading_type)) && ($attachmentDetails[$j]->reading_type) != '') {
          $readingTypeAttributes = array();
          //$readingTypeAttributes['readonly'] = 'readonly';
          //$readingTypeAttributes['class'][] = 'addedit-readonly-textfield'; // Add another class
          $readingTypeAttributes['class'][] = 'input-field-set input-attachment-title addedit-edit-ac-textfield'; // Add another class
        } else {          
          $readingTypeClass = '';
          $readingTypeEmptyValue = 'Attachment Name';
          $formAttachmentName = $form_state['values']['attachment_fieldset']['attachment']['attachment_name_'.$j];
          $readingTypeClass = ($formAttachmentName == $readingTypeEmptyValue || empty($formAttachmentName) )? 'input-field-grey' : '';
          $readingType = $readingTypeEmptyValue;
          
          
          $readingTypeAttributes = array();
          $readingTypeAttributes['class'][] = 'input-field-set input-attachment-title addedit-edit-ac-textfield '.$readingTypeClass; // Add another class
          $readingTypeAttributes['onblur'][] = 'textfieldTitleBlur(this, "'.$readingTypeEmptyValue.'");';
          $readingTypeAttributes['onfocus'][] = 'textfieldTitleClick(this, "'.$readingTypeEmptyValue.'");';
        } 
        
        
        if(!empty($attachmentDetails[$j]->reading_content)) {
          $readingContentAttributes = array();
          //$readingContentAttributes['readonly'] = 'readonly';
          //$readingContentAttributes['class'][]  = 'addedit-readonly-textfield'; // Add another class
          $readingContentAttributes['class'][]  = 'input-field-set input-attachment-url addedit-edit-ac-textfield'; // Add another class
        } else {   
          $readingContentClass = '';
          $readingContentEmptyValue = 'Attachment URL';
          $formAttachmentURL = $form_state['values']['attachment_fieldset']['attachment']['attachment_url_'.$j];
          $readingContentClass = ($formAttachmentURL == $readingContentEmptyValue || empty($formAttachmentURL) )? 'input-field-grey' : '';
          $readingContent = $readingContentEmptyValue;
          
          $readingContentAttributes = array();
          $readingContentAttributes['class'][]  = 'input-field-set input-attachment-url addedit-edit-ac-textfield '.$readingContentClass; // Add another class
          $readingContentAttributes['onblur'][] = 'textfieldTitleBlur(this, "'.$readingContentEmptyValue.'");';
          $readingContentAttributes['onfocus'][] = 'textfieldTitleClick(this, "'.$readingContentEmptyValue.'");';
        }
        
        /*if(!empty($attachmentDetails[$j]->reading_type)) {
          $readingTypeAttributes = array();
          $readingTypeAttributes['readonly'] = 'readonly';
          $readingTypeAttributes['class'][] = 'addedit-readonly-textfield'; // Add another class
          $readingTypeAttributes['class'][] = 'input-field-set input-attachment-title addedit-edit-ac-textfield'; // Add another class
        } else {
          $readingTypeAttributes = array();
          $readingTypeAttributes['class'][] = 'input-field-set input-attachment-title addedit-edit-ac-textfield'; // Add another class
        }*/
        $form['attachment_fieldset']['attachment']['attachment_name_'.$i] = array(
          '#type' => 'textfield',     
          '#prefix' => '<div class="attachment-details-row admin-text-field-column" ><table cellpadding="2" width="100%" cellspacing="2" border="0"><tr><td>',
          '#suffix' => '</td>',  	
          '#default_value' => $readingType,  
          '#size' => 20,
          //'#maxlength' => 50,  	
          '#attributes' => $readingTypeAttributes,
          //'#attributes' => array('Class' => 'input-field-set input-attachment-set addedit-edit-ac-textfield'),
        );

       /*if(!empty($attachmentDetails[$j]->reading_content)) {
          $readingContentAttributes = array();
          $readingContentAttributes['readonly'] = 'readonly';
          $readingContentAttributes['class'][]  = 'addedit-readonly-textfield'; // Add another class
          $readingContentAttributes['class'][]  = 'input-field-set input-attachment-url addedit-edit-ac-textfield'; // Add another class
        } else {
          $readingContentAttributes = array();
          $readingContentAttributes['class'][]  = 'input-field-set input-attachment-url addedit-edit-ac-textfield'; // Add another class
        }*/
        
        $form['attachment_fieldset']['attachment']['file_type_start_'.$i] = array(
    	'#type'       => 'markup',
    	'#prefix' => '<td class="attachment-remove-space">',
        );
        
        
        
        $defaultValue = ($attachmentDetails[$j]->is_file == '' || $attachmentDetails[$j]->is_file == 'URL') ? 'URL' : 'FILE';
        if($defaultValue == 'URL' || (!empty($readingContent))){
          $urlSelStyle  = 'style="display:block;"';
          $browseSelStyle  = 'style="display:none;"'; 
        }else{
          $browseSelStyle  = 'style="display:block;"';
          $urlSelStyle  = 'style="display:none;"'; 
        }  
        
        if($readingContent == 'Attachment URL') {          
          $form['attachment_fieldset']['attachment']['attachment_url_'.$i] = array(
            '#type' => 'textfield',
            '#prefix' => '<div id="attach_url_control'.$i.'" '.$urlSelStyle.'>',
            '#suffix' => '</div>',
            '#default_value' => $readingContent,
            '#size' => 18,
            //'#maxlength' => 50,
            '#attributes' => $readingContentAttributes,
            //'#attributes' => array('Class' => 'input-field-set input-attachment-textfield addedit-edit-ac-textfield'),   
          );  
        }
        else {
          if($defaultValue!='URL'){
          $readingContentAttributes['readonly'] = 'readonly';
          $readingContentAttributes['class'][]  = 'addedit-readonly-textfield'; // Add another class
         } 
		 $form['attachment_fieldset']['attachment']['attachment_url_upd_'.$i] = array(
          '#type' => 'textfield',
          '#prefix' => '<div id="attach_url_control_upd'.$i.'" '.$urlSelStyle.'>',
          '#suffix' => '</div>',
          '#default_value' => $readingContent,
          '#size' => 18,
          '#attributes' => $readingContentAttributes,
          );
        }       
          
          
        $form['attachment_fieldset']['attachment']['attachment_url_browse_'.$i] = array(
          '#type' => 'file',
          '#prefix' => '<div id="attach_browse_control'.$i.'" '.$browseSelStyle.'>',
          '#suffix' => '</div>',
          '#default_value' => $readingContent,
          '#size' => 26,
          //'#maxlength' => 50,
          '#attributes' => $readingContentAttributes,
          //'#attributes' => array('Class' => 'input-field-set input-attachment-textfield addedit-edit-ac-textfield'),   
        );           
        
        $form['attachment_fieldset']['attachment']['file_type_end_'.$i] = array(
    	  '#type'       => 'markup',
    	  '#suffix' => '</td>',
        );
        
        //$readOnly = false;//(!empty($readingContent) && $readingContent != 'Attachment URL') ? TRUE : FALSE;        
        $readOnly = (!empty($readingContent) && $readingContent != 'Attachment URL') ? TRUE : FALSE;        
        $options = array('URL' => t('URL'), 'FILE' => t('LBL293'));
        $attributes = array();
        $attributes['class'] = array('addedit-edit-attachment'); 
        $attributes['class'][] = 'addedit-edit-radioset';        
        $attributes['onclick'][] = '$(document).ready(function(){ opt_sel_attach('.$i.'); });';
        
        $form['attachment_fieldset']['attachment']['attachment_radioname_'.$i] = array (
        '#type' => 'radios',
        '#prefix' => '<td>',
        '#suffix' => '</td>',    
        '#title' => '',
        '#title_display' => 'invisible',
        '#default_value' =>  $defaultValue,
        '#options' => $options,
        '#disabled' => $readOnly,       
        '#attributes' => $attributes
        );
        
        // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
        $delAttachAttributes['class'][] = 'addedit-form-expertusone-throbber';
        $delAttachAttributes['class'][] = 'admin-save-button-middle-bg';
        $delAttachAttributes['class'][] = 'admin-delete-img';
        // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
        $delAttachAttributes['data-wrapperid'] = array('attachment-fieldset-wrapper-'.$uniqueId);
  
        $form['attachment_fieldset']['attachment']['attachment_delete_'.$i] = array(
          //'#type' => 'submit',    
          '#type' => 'image_button', '#src' => base_path().'sites/all/themes/core/expertusone/expertusone-internals/images/close.png',
          //'#value' => 'Delete',
          '#title' => t('LBL286'),
          '#name' => 'Delete Attachment'.$i,
          '#prefix' => '<td><div class="admin-add-button-container">',
          '#suffix' => '</div></td></tr></table></div>',    
          '#submit' => array('delete_attachment'),
          '#attributes' => $delAttachAttributes,
          '#ajax' => array(
            'callback' => 'attach_add_more_btn_callback',
            'wrapper' => 'attachment-fieldset-wrapper-'.$uniqueId,
            'param' =>$attachmentDetails[$j]->attachment_id,
        	'num_count' => $form_state['num_attachment'],
            'loop_id' =>$i
          ),  
        );
        $j++;
      }
  }
  
  $form['attachment_fieldset']['attach_hidden_delete'] = array(
    '#type' => 'value',
    '#value' => implode(",",$form_state['delete_attachment_fieldset'])    
  );
  
  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $addAttachAttributes['class'][] = 'addedit-form-expertusone-throbber';
  $addAttachAttributes['class'][] = 'admin-save-button-middle-bg';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  $addAttachAttributes['data-wrapperid'] = array('attachment-fieldset-wrapper-'.$uniqueId);  

  if($form_state['show_add_another'] || $entityType == 'cre_sys_obt_cls'){
    
    $form['attachment_fieldset']['add_attachment'] = array(
      '#type' => 'submit',
      //'#type' => 'image_button', '#src' => base_path().'sites/all/themes/core/AdministrationTheme/images/exp_sp_icon_16x16_MoveDown.gif',
      '#value' => t('LBL386'),
      '#name' => 'Add Attachment',
      '#prefix' => '<div class="add-more-attachment admin-add-more-field-column" id="add-more-attachment"><table cellpadding="2" cellspacing="2" border="0"><tr><td><div class="admin-save-button-left-bg"></div>',
      '#suffix' => '<div class="admin-save-button-right-bg"></div></td></tr></table></div>',
      '#submit' => array('attach_add_row'),
    	'#attributes' => $addAttachAttributes,
      '#ajax' => array(
        'callback' => 'attach_add_more_btn_callback',
        'wrapper' => 'attachment-fieldset-wrapper-'.$uniqueId,
         'num_count' => $form_state['num_attachment'],
      ),
    );
  } else {
    $form['attachment_fieldset']['add_attachment'] = array(
      '#type' => 'markup',
      '#prefix' => '<div class="add-more-attachment admin-add-more-field-column" id="add-more-attachment"><table cellpadding="2" cellspacing="2" border="0"><tr><td>',
      '#suffix' => '</td></tr></table></div>',
    );
  }
  } catch (Exception $ex) {
  	watchdog_exception('displayAttachmentDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function attachmentDetailsView(&$form, &$form_state, $entityId, $entityType, $uniqueId){
 try {
  $attachmentDetails = getAttachDetails($entityId, $entityType);
  
  if(count($attachmentDetails)>0) {
  
      /*$form['attach_fieldset'] = array(
        '#type' => 'fieldset',
        '#title' => t('Attachments'),
        '#prefix' => '<div id="tag-fieldset-wrapper-'.$uniqueId.'" class="admin-tab-list-row tag-fieldset-wrapper '.$hideShowClass.'">',
        '#suffix' => '</div>',
        '#collapsible' => TRUE,
        '#collapsed' =>TRUE
      );*/
      $form['attach_fieldset'] = array(
    	'#type'       => 'markup',
        '#prefix' => '<div id="tag-fieldset-wrapper-'.$uniqueId.'" class="admin-tab-list-row tag-fieldset-wrapper">',
		'#suffix' => '</div>',
      );
      
     $form['attach_fieldset']['table_start'] = array(
      '#type'   => 'markup',
      '#markup' => '<table cellpadding="3" cellspacing="3" width="80%" border="0">',
     );

     for($i=0;$i<count($attachmentDetails);$i++) {
       
       $readingType     = $attachmentDetails[$i]->reading_type;
       $readingContent  = $attachmentDetails[$i]->reading_content;
       
       $form['attach_fieldset']['attach_title_'.$i] = array(
          '#type'     => 'item',
          //'#markup'   => '<tr><td><div class="addedit-new-field-value addedit-view-field-value input-field-set input-attachment-textfield addedit-edit-ac-textfield">'.$readingType.'</div></td>',
          '#markup'   => '<tr><td width="225"><input type="text" value="'.$readingType.'" readonly="readonly" class="reading-type-txtBox"></td>',
        );
       
       $form['attach_fieldset']['attach_url_'.$i] = array(
          '#type'     => 'item',
          '#markup'   => '<td><div class="addedit-new-field-value addedit-view-field-value input-field-set input-attachment-textfield addedit-edit-ac-textfield">'.$readingContent.'</div></td></tr>',
          '#markup'   => '<td><input type="text" value="'.$readingContent.'" readonly="readonly" class="reading-content-txtBox"></td></tr>',
        );
     }
      
     $form['attach_fieldset']['table_end'] = array(
      '#type'   => 'markup',
      '#markup' => '</table>',
     );
  }
  else{
    $form['no-records-msg'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="no-records-msg">'.t('MSG278').'</div>'
    );  
  }
  } catch (Exception $ex) {
  	watchdog_exception('attachmentDetailsView', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 *  New mockup design implemented for Attachment - #0013357
 */

function catalog_attachment_display($entityId, $entityType) {
 try {
  $form_state = array(
   'no_redirect'  => TRUE,
   'build_info'   => array('args' => array('ajax')),
   'entity_id'    => $entityId,
   'entity_type'  => $entityType
  );      
  
  $formId                 = 'attachment_addedit_form_html';
  $builtForm              = drupal_build_form($formId, $form_state);
  $renderedTabContentHTML = drupal_render($builtForm);   
  $drupalSettings         = getDrupalSettings();

  $response       = array(
    'drupal_settings'     => $drupalSettings,
    'render_content_main' => $renderedTabContentHTML,
  ); 
 
  echo drupal_json_output($response);
  exit;  
  } catch (Exception $ex) {
  	watchdog_exception('catalog_attachment_display', $ex);
  	expertusErrorThrow($ex);
  }
}


function attachment_addedit_form_html($form, &$form_state) {
  try {
  //$form 		       = newAddEditFormWithCancelAndSave('catalog-attachment-addedit-form');  
  	include_once(drupal_get_path('module', 'exp_sp_administration_catalog') .'/exp_sp_administration_catalog_course.inc');
  $entity_id         = $form_state["entity_id"];
  $entity_type       = $form_state["entity_type"];
  $attachmentDetails = getAttachDetailList($entity_id, $entity_type);
    
  $form = array();
  /*
   * This is a original container
   */
  //$form['#prefix'] = '<div id="catalog-attachment-disp-container">';
  /*
   * Following $form['#prefix'] line is temporary for Chrome broser nester form issue
   * 0020803: Functionality issue in attachments>>upload url-----Chrome 
   */
  $form['#prefix'] = /* (( stripos($_SERVER['HTTP_USER_AGENT'], 'Chrome') || stripos ($_SERVER['HTTP_USER_AGENT'], 'Firefox') )? '</form>' : '' ) */ '</form>'. '<div id="catalog-attachment-disp-container">';
  
  $form['#suffix'] = '</div>';  
  $form['#attributes'] = array('enctype' => "multipart/form-data");
  //$form = newAddEditFormWithCancelAndSave('attachment_addedit_form');
   /*$form['attachment_start'] = array(
      '#type'       => 'markup',
      '#markup'     => '<div id="catalog-attachment-disp-container">',
      '#attributes' => array ('enctype' => "multipart/form-data" ),
     );*/

    if(count($attachmentDetails)>0) {
      
      $form['table_start'] = array(
        '#type'   => 'markup',
        '#markup' => '<div id="scrolldiv"><table id="scrolltable" cellpadding="0" cellspacing="0" width="100%" border="0">',
      );
     
     
     $attachCount                          = count($attachmentDetails);
     
     addHiddenValueToAddEditForm($form, 'noof_attachment', $attachCount);
     
     
     for($i=0;$i<$attachCount;$i++) {
       
       $readingType     = $attachmentDetails[$i]->reading_type;
       $readingContent  = $attachmentDetails[$i]->reading_content;
       $attachment_id   = $attachmentDetails[$i]->attachment_id;
       $created_on      = $attachmentDetails[$i]->created_on;
       $updated_on      = $attachmentDetails[$i]->updated_on;
       $file_size       = $attachmentDetails[$i]->custom4;
       $is_file         = $attachmentDetails[$i]->is_file;
       
       addHiddenValueToAddEditForm($form, 'attachment_id'.$i, $attachment_id);
       
       $attachListAttributes                 = array();
       $attachListAttributes['class'][]      = 'addedit-edit-crs_code addedit-edit-textfield non-editable-txt attachment-txt-box';     
       $attachListAttributes['class'][]      = 'attachment-list-txtbox';     
       $attachListAttributes['onchange'][]   = 'document.getElementById(\'attachment_done_btn\').click();return false;';
       //$attachListAttributes['onchange'][]   = 'attachSubmitUrl()';          
       //$attachListAttributes['onclick'][]   = 'toEditInline("attachment_list'.$i.'","click")';
       //$attachListAttributes['onclick'][]   = 'toEditAttachment('.$i.',"txtbox");';
       //$attachListAttributes['onblur'][]   = 'toEditInline("attachment_list'.$i.'","blur")'; //onblur
       //$attachListAttributes['onblur'][]   = 'toEditInline("attachment_list'.$i.'","blur"); toEditAttachment('.$i.',"txtbox");'; //onblur
       $attachListAttributes['onblur'][]   = 'toEditAttachment('.$i.',"txtbox");'; //onblur
       $attachListAttributes['onkeydown'][]   = 'toEditAttachmentKeyDown(event);';
     
       $txt_box_length = strlen($readingType)-1;
       $txt_box_length = ($txt_box_length <30) ? $txt_box_length : 30;
       
       $form["attachment_edit_title".$i] = array(
            '#type'         => 'textfield',    
            '#name'         => 'attachment_list'.$i,              
            '#title'        => 'attachment_list'.$i,
            '#id'           => 'attachment_list'.$i,
       		'#attributes'   => $attachListAttributes,
           	'#prefix'       => '<tr><td>
                                 <div id="attachment_list_label_'.$i.'" onblur="toEditAttachment('.$i.',\'label\')" onclick="toEditAttachment('.$i.',\'label\')" class="attachment-label-text addedit-edit-crs_code addedit-edit-textfield non-editable-txt attachment-txt-box vtip" title="'.sanitize_data($readingType).'">'.titleController('ADDEDIT-CATAOG-ATTACHMENT-NAME', $readingType,48).'</div>
           						<div id="attachment_list_edit_'.$i.'" style="display:none;"  onblur="toEditAttachment('.$i.',\'txtbox\')">',
            '#title_display' => 'invisible',
            '#size'         => $txt_box_length,//strlen($readingType)-1,
            '#default_value'=> $readingType,
       		'#maxlength' => 2050,
         );
        addHiddenValueToAddEditForm($form, 'attachment_list_type'.$i, $is_file);
        addHiddenValueToAddEditForm($form, 'attachment_list_content_old'.$i, $readingType);
        addHiddenValueToAddEditForm($form, 'attachment_list_original'.$i, $readingContent);
             
        $delAttributes             = array();
        $delAttributes['class'][]  = 'attach-delete-btn';
     
        $form['del_attach_list'.$i] = array(
            '#type'       => 'submit',
            '#prefix'     => '</div><div id="attachment-del-btn" class="attachment-list-btn">',
            '#suffix'     => '</div>',                          
            '#title'      => 'del_attach_list'.$i,
            '#id'         => 'del_attach_list'.$i,
            '#name'       => 'del_attach_list'.$i,
            '#attributes' => $delAttributes,
            '#value'      => t('X'),
            '#ajax' => array(
                'wrapper'         => 'catalog-attachment-disp-container',
                'callback'        => 'exp_sp_administration_attachment_repaint',
          	  	'choice_count'    => $attachCount,
                'del_list_no'     => $i,
                'attachment_id'   => $attachment_id,
                'method'          => 'replace',
                'effect'          => 'none',
                'event'           => 'click',
                'keypress'        => true,
                'progress' => array(
                  'type'    => 'throbber',
                  'message' => ''
                ),
            )
          );
          
        $date_format = date("F d, Y",strtotime($created_on));
          
  	    $form['attach_title_'.$i.'_end'] = array(
          '#type'     => 'markup',
          '#markup'   => '<span class="attachment-label attachment-detail-list"> '.(!empty($file_size) ? ($file_size.' KB | ') : '').$date_format.'</span></td></tr>',
        );
       
     }
      
     $form['table_end'] = array(
      '#type'   => 'markup',
      '#markup' => '</table></div>',
     );
  
    } 
     
  $form['attachment_detail_disp'] = array(
  	  '#type'       => 'markup',
      '#prefix'     => '<div id="catalog-attachment-addedit-form-disp" class="attachment-display-text">'.(((count($attachmentDetails)==0) && ($form_state['storage']['mode']  != 'add_url')) ? '<span id="no_attachment_disp">'.t('MSG370').'</span>' : '').'',
      '#suffix'     => '</div><div id="txt_control_attach_url">'
    );
    
   addHiddenValueToAddEditForm($form, 'entity_id', $entity_id);  
   addHiddenValueToAddEditForm($form, 'entity_type', $entity_type);  
   
  if($form_state['storage']['mode']  == 'add_url') { 
    
    $addAttachUrlAttributes                = array();
    $addAttachUrlAttributes['onchange'][]  = 'document.getElementById(\'attachment_done_btn\').click(); return false;';
   // $addAttachUrlAttributes['onchange'][]   = 'attachSubmitUrl()';  
    $strVariable = t("LBL954").' '. "url";  
    $addAttachUrlAttributes['onfocus'][]   = 'textfieldTitleClick(this,"'.$strVariable.'");';
    $addAttachUrlAttributes['onblur'][]    = 'textfieldTitleBlur(this,"'.$strVariable.'");';
    $addAttachUrlAttributes['class'][]     = 'input-field-grey'; 
    $addAttachUrlAttributes['class'][]     = 'attachment-url-input-field';
    $addAttachUrlAttributes['onkeydown'][] = 'toEditAttachmentKeyDown(event);'; 
    $defaultValue                          = $strVariable;    
    
     $form["attachment_url"] = array(
      '#type'           => 'textfield',    
      '#name'           => 'attachment_url',     
      '#id'             => 'attachment_url',
      '#title'	        => "",
      '#prefix'         => '<div id="provide_url_id" class="attachment-provide-url-disp">',
      '#suffix'         => '</div>',
      '#title_display'  => 'invisible',
      '#default_value'  => $defaultValue,
      '#attributes'     => $addAttachUrlAttributes,
      '#size'			=> 55,
      '#maxlength' => 2050,
     );
     
	$form['load_select_drop_down_'] = array (
		'#prefix' => '<img src=\'themes/seven/images/buttons.png\' width="0" height="0" style="display:none;" onload="$(\'#attachment_url\').focus();" />',
	);
     
  }
    
  $form['attachment_detail_disp1'] = array(
  	  '#type'       => 'markup',
      '#markup'     => '</div>'
   );
  $form['attachment_part_disp_start'] = array(
  	  '#type'       => 'markup',      
      '#markup'     => '<table cellpadding="1" cellspacing="0" width="100%" border="0">
       					'.((count($attachmentDetails)>0) ? ('<tr><td colspan="3" class="attachment-label-bottom">'.t('MSG374').'</td></tr>') : '').'
      					 <tr class="attachment-detail-seperator">',
    );
   $form['attachment_upload_file_disp_start'] = array(
  	  '#type'       => 'markup',      
      '#markup'     => '<td width="40%" class="attachment-btn-set"><table cellpadding="0" cellspacing="0" width="100%" border="0"><tr><td class="attachment-title-text attachment-display-btn-text">'.t('MSG371').'</td></tr>'
   					    .'<tr><td class="attachment-label-btn-bottom">'.t('MSG373', array('@extensions'=>'txt, doc(x), ppt(x), xls(x), pdf, png, jpg, jpeg, gif')).'</td></tr><tr><td class="attachment-display-btn-text"><span class="attach-btn-group-middle"><div class="admin-save-button-left-bg"></div><span class="admin-save-button-middle-bg" >'.t('LBL609').'</span><div class="admin-save-button-right-bg"></div></span><div style="display:inline;" id="attach_up_file" >',
    );
   $browseAttributes             = array();
   $browseAttributes['class'][]  = 'attach-browse';
   $browseAttributes['class'][]  = 'attach-browse-hid';
   $browseAttributes['onChange'] = 'document.getElementById(\'attachment_done_btn\').click()';
    $form['attachment_upload_file'] = array(
      '#type'          => 'file',
      '#id'            => 'attachment_upload_file',
      '#title'   	   => t('Choose a file'),
     // '#title_display' => 'invisible',
      '#attributes'    => $browseAttributes,   
      '#size'          => 1,
      '#theme_wrappers' => array(),
    );
    
   /*$form['attachment_upload_file_disp'] = array(
  	  '#type'       => 'markup',
      '#prefix'     => '<table cellpadding="1" cellspacing="0" width="100%" border="0">
       					'.((count($attachmentDetails)>0) ? ('<tr><td colspan="3" class="attachment-label-bottom">'.t('MSG374').'</td></tr>') : '').'
      					 <tr class="attachment-detail-seperator"><td width="40%" class="attachment-btn-set">
      					 <table cellpadding="0" cellspacing="0" width="100%" border="0"><tr><td class="attachment-display-btn-text">'.t('MSG371').'</td></tr>'
   					     .'<tr><td class="attachment-label-btn-bottom">'.t('MSG373').'</td></tr><tr><td class="attachment-display-btn-text"><span class="attach-btn-catalog-container" onclick="attachmentUploadFile()">'.t('LBL609').'</span></td></tr>',
                         
    );*/
    // t('Files with following extensions can be uploaded: txt, doc, docx, ppt, pptx, xls, xlsx, pdf, png.')
    $provideUrlAttributes             = array();
    $provideUrlAttributes['class'][]  = 'attach-btn-catalog-provide-url';
    $wrapperId                        = 'catalog-attachment-addedit-form-disp';
    $wrapperId                        = 'catalog-attachment-disp-container';
    
   /* $form["upload_file_btn"] = array(
  	    '#type'       => 'submit',
  	    '#prefix'     => '<tr><td>',
  	    '#suffix'     => '</td></tr>',
  	    '#value'      => t('Upload File'),
        '#name'       => 'upload_file_btn',
  	    '#attributes' => $provideUrlAttributes,       
        '#ajax' => array(
		      'wrapper'     => $wrapperId,
		      //'callback'  => 'addEditRepaintForm',
              'callback'    => 'exp_sp_administration_attachment_repaint',    
		      'method'      => 'replace',
		      'effect'      => 'none', // 'fade',
		      'event'       =>'click',
		      'keypress'    => true,
		      'progress'    => array(
		        'type'    => 'throbber',
		        'message' => ''
		      ),
	    	),
  	   );*/
    
    $form['attachment_upload_file_disp_end'] = array(
  	  '#type'       => 'markup',      
      '#markup'     => '</div></td></tr></table></td>'
    );
    
    $form['attachment_link_url_disp'] = array(
  	  '#type'       => 'markup',
      '#markup'     => '<td class="attachment-seperate-display-td-cls" width="5%"><span class="attachment-seperate-display"></span> '.t('LBL644').' <span class="attachment-seperate-display"></span></td><td width="40%" class="attachment-btn-set">
      					 <table cellpadding="0" cellspacing="0" width="100%">
      					 <tr><td class="attachment-title-text attachment-display-btn-text">'.t('MSG372').'</td></tr><tr><td class="attachment-empty-column-height">&nbsp;</td></tr>
      					 <tr><td><span class="attach-btn-group-provide-url"><div class="admin-save-button-left-bg"></div>',
    );
    
    $provideUrlAttributes                    = array();
    $provideUrlAttributes['class'][]         = 'admin-save-button-middle-bg';
    //$provideUrlAttributes['onclick'][]     = 'toEditInline("attachment_list'.$i.'")';
    //$provideUrlAttributes['onclick'][]     = 'attachmentUrlCall()';    
    $wrapperId                               = 'catalog-attachment-addedit-form-disp';
    $wrapperId                               = 'catalog-attachment-disp-container';
    //$provideUrlAttributes['data-wrapperid']  = array($wrapperId); 
    
    $form["attachment_provide_url"] = array(
  	    '#type'       => 'submit',
  	    '#value'      => t('LBL643'),
        '#name'       => 'provide_url',
  	    '#attributes' => $provideUrlAttributes,       
        '#ajax' => array(
		      'wrapper'     => $wrapperId,//$wrapperId,
		      //'callback'  => 'addEditRepaintForm',
              'callback'    => 'exp_sp_administration_attachment_repaint',    
		      'method'      => 'replace',
		      'effect'      => 'none', // 'fade',
		      'event'       =>'click',
		      'keypress'    => true,
		      'progress'    => array(
		        'type'    => 'throbber',
		        'message' => ''
		      ),
	    	),
  	   );
    
  	$form['attachment_link_url_disp_end'] = array(
  	  '#type'       => 'markup',
      '#markup'     => '<div class="admin-save-button-right-bg"></div></span></td></tr></table></td>'
    );
    $form['attachment_part_disp_end'] = array(
  	  '#type'       => 'markup',      
      '#markup'     => '</tr></table>',
    );
    // catalog-attachment-disp-container
    $wrapperId                              = 'catalog-attachment-disp-container';    
    $doneBtnAttributes                      = array();
    $doneBtnAttributes['class'][]           = 'attach-btn-catalog-provide-url';
    $doneBtnAttributes['class'][]           = 'addedit-form-expertusone-throbber';
    $doneBtnAttributes['data-wrapperid']    = array($wrapperId);
    $doneBtnAttributes['onclick'][]         = 'return false;';  
    $form["attachment_done"] = array(
  	    '#type'       => 'submit',
        '#id'         => 'attachment_done_btn',
  	    '#prefix'     => '<div style="display:none;">',
  	    '#suffix'     => '</div>',
  	    '#value'      => t('Done'),
        '#name'       => 'done',
  	    '#attributes' => $doneBtnAttributes,       
        '#ajax' => array(
		      'wrapper'   => $wrapperId,
		      //'callback'  => 'addEditRepaintForm',
    		  'callback'    => 'exp_sp_administration_attachment_repaint',
		      'method'    => 'replace',
		      'effect'    => 'none', // 'fade',
		      'event'     =>'click',
		      'keypress'  => true,
		      'progress' => array(
		        'type'    => 'throbber',
		        'message' => ''
		      ),
	    	),
  	   );
  	   
  	   /*$form["attachment_done_clone"] = array(
  	    '#type'       => 'submit',
        '#id'         => 'attachment_done_btn_clone',
  	    //'#prefix'     => '<div style="display:none;">',
  	    //'#suffix'     => '</div>',
  	    '#value'      => t('Done'),
        '#name'       => 'done',
  	    //'#attributes' => $doneBtnAttributes,       
       
  	   );*/

  	 $popupId =  'qtipAttachIdqtip_visible_disp_'.$entity_id.'_'.$entity_type;
  	 $popuprenderAttachId = "paintContentqtip_attachment_disp_".$entity_id.'_'.$entity_type;
  	 /*$dispId = 'qtipAttachIdqtip_visible_disp_'.$entity_id; // Added by Vincent for remove the entire content while closing.
  	 						qtipAttachIdqtip_visible_disp_4_cre_sys_obt_crs_disp*/
  	 if($entity_type == 'cre_sys_obt_trp' && $entity_id < 1) {
  	 $tpEmptyClose = "closeQtyp('qtipAttachIdqtip_visible_disp_0_cre_sys_obt_trp','');";
  	 }
  	 $form['attachment_close_btn'] = array(
  	  '#type'       => 'markup',
      //'#markup'     => '<span class="attachment-close-btn" onclick="$(\'#catalog-attachment-disp-container\').closest(\'.qtip-active\').hide(); $(\'#'.$popupId.'_disp\').html(\'\'); return false;">'.t('LBL123').'</span>'
      '#markup'     => '<div class="addedit-form-cancel-container-actions"><span class="white-btn-bg-left"></span><span class="attachment-close-btn white-btn-bg-middle" onClick=\'closeQtip("","'.$entity_id.'"); return false;\' >'.t('LBL123').'</span><span class="white-btn-bg-right"></span></div>'
    );

  	return $form;
  	} catch (Exception $ex) {
  		watchdog_exception('attachment_addedit_form_html', $ex);
  		expertusErrorThrow($ex);
  	}
}

function getAttachDetailList($entity_id,$entityType) {
  try {
  $entityId   = ($entity_id < 1) ? 0 : $entity_id;
  $custom0    = ($entity_id < 1) ? $entity_id : NULL;
  
  
  $select = db_select("slt_entity_reading_mapper","attach");
  $select->addField('attach','id','attachment_id');
  $select->addField('attach','reading_id','reading_id');
  $select->addField('attach','reading_type','reading_type');
  $select->addField('attach','reading_content','reading_content');
  $select->addField('attach','created_on','created_on');
  $select->addField('attach','updated_on','updated_on');
  $select->addField('attach','custom3','is_file');
  $select->addField('attach','custom4','custom4');
  $select->condition('attach.entity_id',$entityId,'=');
  if($entityId == 0)
  {
  	$currentUserId = getIdOfLoggedInUser();
  	$select->condition('attach.created_by',$currentUserId,'=');
  }
  
  if(!empty($entityType)) {
    $select->condition('attach.entity_type',$entityType,'=');
  }	
  if($entity_id < 1) {
    $select->condition('attach.custom0',$custom0,'=');
  }
  $attachmentList = $select->execute()->fetchAll();
  expDebug::dPrint("inside get details function : ".$currentUserId." , ".$entity_id." , ".$entityId.' , '.$entityType.', '.$custom0.' | '.print_r($attachmentList,true) , 3);
  return $attachmentList;
  } catch (Exception $ex) {
  	watchdog_exception('getAttachDetailList', $ex);
  	expertusErrorThrow($ex);
  }
}

function attachment_addedit_form_html_validate($form, &$form_state) {
 try {
   $form_state["storage"]["form_validate"] = true; 
   
   //$form_state["storage"]["action_mode"] = "submit";   
   // '#name'       => 'upload_file_btn',
   
   $attachment_url_txt = trim($form_state['values']['attachment_url']);
   if(strlen($attachment_url_txt) > 2048)
   {
   	form_set_error('attachment_url', t("!name cannot be longer than %max characters but is currently %length characters long.",array("!name"=>"URL", "%max"=>"2048", "%length"=>strlen($attachment_url_txt))));
   	return ;
   }
   $noof_attachment = $form_state['values']["noof_attachment"];
    
   if($noof_attachment > 0)
   {
   	for($i=0;$i<$noof_attachment;$i++) {
   
   		if(!empty($form_state['input']['attachment_list'.$i])) {
   			if($form_state["values"]['attachment_list_type'.$i] == 'URL') {
   				 
   				$new_file = $form_state['input']['attachment_list'.$i];
   				if(strlen($new_file) > 2048)
   				{
   					form_set_error('attachment_url', t("!name cannot be longer than %max characters but is currently %length characters long.",array("!name"=>"URL", "%max"=>"2048", "%length"=>strlen($new_file))));
   					return ;
   				}
   			}
   		}
   	}
   }
   
   if ($form_state['triggering_element']['#name'] == 'provide_url') {
      $form_state["storage"]["action_mode"]   = "provide_url"; 
      $form_state['rebuild']                  = TRUE;
      $form_state['storage']['mode']          = 'add_url'; 
      $form_state["storage"]["form_validate"] = false;        
      return ;
   }
 
   if ($form_state['triggering_element']['#value'] == t('X')) {
       $form_state["storage"]["action_mode"]   = "delete";
       $form_state['storage']['mode']          = "view";        
       $attachment_id                          = $form_state['triggering_element']['#ajax']['attachment_id'];
       $form_state["delete_id"]                = $attachment_id;    
   }
   
   if((count($_FILES["files"])>0) && !empty($_FILES['files']['name']['attachment_upload_file'])) {
     
      if (attachment_file_validate($_FILES['files']['name']['attachment_upload_file'])) {  
        $form_state['rebuild']                  = TRUE;
        $form_state['storage']['mode']          = 'view';
        $form_state["storage"]["form_validate"] = false;
        form_set_error('attachment_upload_file', t('ERR171') . ' ' . t('ERR206') . ' ' . t('Format') . ' - ' . t('MSG658').': txt, doc(x), ppt(x), xls(x), pdf, png, jpg, jpeg, gif');
        return ;
      }  
      
   }
   } catch (Exception $ex) {
   	watchdog_exception('attachment_addedit_form_html_validate', $ex);
   	expertusErrorThrow($ex);
   }
}

// Validate attachment file extension - return true when given extension is not valid 
function attachment_file_validate($file_name) {
	try {
  $extensions         = array('.txt', '.doc', '.docx', '.ppt', '.pptx', '.xls', '.xlsx', '.pdf', '.png','.jpg','.jpeg','.gif');
  $extension          = strrchr($file_name, '.');  
  $return             = false;
  
  if (!in_array(strtolower($extension), $extensions))    {
      $return = true;  
  }
  return $return ;
  } catch (Exception $ex) {
  	watchdog_exception('attachment_file_validate', $ex);
  	expertusErrorThrow($ex);
  }
}

function updateAttachmentInfoOnAddForm($entity_id,$dummy_attachment_id) {
  
  $dummy_attachment_id = ereg_replace(" ","",$dummy_attachment_id);
  
  $get_dummy_ids = getAttachDetailList($dummy_attachment_id,'');
  
  
    try {
        $txn = db_transaction();         
              
              $updateStmt  = db_update('slt_entity_reading_mapper');
              $custom      = NULL;     
              $fields      = array(
                            'entity_id'          => $entity_id,                             
                            'custom0'            => $custom                               
                          );
              
              $updateStmt->fields($fields);    
              $updateStmt->condition('entity_id', 0);
              $updateStmt->condition('custom0', "$dummy_attachment_id");
              $numUpdated = $updateStmt->execute();
            
           // $form_state['rebuild']                  = TRUE;
            unset($txn);
        }
        catch (Exception $ex) {
          $txn->rollback();
          unset($txn);
          watchdog_exception('updateAttachmentInfoOnAddForm', $ex);
          throw $ex;
        }
  
}

function attachment_addedit_form_html_submit($form, &$form_state) {
  try {
  $config           = getConfig("exp_sp");
  $admin_site_url   = $config['admin_site_url'];
  $currentUserId = getIdOfLoggedInUser();

  // Add attachment ----------------------------
  
  expDebug::dPrint('Entittty form state valruiuoiklh '.print_r($form_state["values"],true) , 5);

  $attachment_url_txt = trim($form_state['values']['attachment_url']);
  if(($form_state["storage"]["action_mode"] == "provide_url") && $form_state["storage"]["form_validate"] && ($form_state['values']['attachment_url'] != "Paste or Type url") && !empty($attachment_url_txt)) {
     $url = $form_state["values"]["attachment_url"];
     expDebug::dPrint(' URL Match to ins :'.preg_match('|^http(s)?://[a-z0-9-]+(.[a-z0-9-]+)*(:[0-9]+)?(/.*)?$|i', $url) , 4);

     $reading_content_txt = $form_state['values']['attachment_url'];
     if(preg_match('|^http(s)?://[a-z0-9-]+(.[a-z0-9-]+)*(:[0-9]+)?(/.*)?$|i', $reading_content_txt) === 0) {
       $reading_content_txt = 'http://'.$form_state['values']['attachment_url'];
     }
     //$reading_type = 'URL';$reading_type = 'FILE';
    
      $custom           = NULL;
      //$entityId         = !empty($form_state['values']['entity_id']) ? $form_state['values']['entity_id'] : 0;
      $entityId         = ($form_state['values']['entity_id'] > 0) ? $form_state['values']['entity_id'] : 0;
      $entityType       = $form_state['values']['entity_type'];
      $reading_type     = 'URL';
      $reading_content  = $form_state['values']['attachment_url'];      
      $xreadingid       = getAttachmentCount($entityId,$entityType);
      $custom0          = ($form_state['values']['entity_id'] > 0) ? NULL : $form_state['values']['entity_id'];
      $entityId         = ($form_state['values']['entity_id'] < 1) ? 0 : $form_state['values']['entity_id'];
      $custom0          = ($form_state['values']['entity_id'] < 1) ? $form_state['values']['entity_id'] : NULL;
      
    
      if(!empty($reading_content)) {
        
          $txn = db_transaction();
      
          try {
                $insertStmt       = db_insert('slt_entity_reading_mapper');
                	
                $fields = array(
                          'entity_id'          => $entityId,                  
                          'entity_type'        => $entityType,
                          'reading_id'     	   => $xreadingid,
                          'reading_type'       => $reading_content,
                          'reading_content'    => $reading_content_txt,
                          'showlnr'            => 'Y',
                          'created_by'         => $currentUserId,
                          'created_on'         => now(),
                					'updated_by'         => $currentUserId,
                					'updated_on'         => now(),
                          'custom0'            => $custom0,
                          'custom1'            => $custom,
                          'custom2'            => $custom,
                          'custom3'            => $reading_type,
                          'custom4'            => $custom   
                        );
                        
                $insertStmt->fields($fields);			            
                 
                $attachmentId = $insertStmt->execute();
                $attFormState = array();
        		$attFormState['id'] = $attachmentId;
        		$attFormState['reading_type'] =  $form_state['values']['attachment_fieldset']['attachment']['attachment_name_'.$i];
        		$attFormState['reading_content'] = $reading_content;
        		$attFormState['reading_id'] = $xreadingid;
        		$attFormState['custom3'] = $reading_type;
        
        		make_audit_trail_entries($attFormState, getIdOfLoggedInUser() , null ,$entityId, 'Inserted',$entityType,'exp_sp_administration_attachment','new attachment',
                                          array('id', 'reading_type', 'reading_content', 'reading_id', 'custom3'));
        
               
                $form_state['rebuild']              = TRUE;
                $form_state['storage']['mode']      = 'view';
                
             } catch (Exception $ex) {
                $txn->rollback();
                watchdog_exception('addattachment', $ex);
                throw $ex;
             }
            
          unset($txn);
       }
    
  }
  
  
  // Delete attachment ---------------------------- 
  
  expDebug::dPrint('Entittty form state before delete '.$form_state["storage"]["action_mode"]." | attachId : ".$form_state["delete_id"]." | ".print_r($form_state["values"],true) , 5);
  if($form_state["storage"]["action_mode"] == "delete") {
  
      $txn = db_transaction();
      
      try {
            $attachmentId = $form_state["delete_id"];
            $entityId         = ($form_state['values']['entity_id'] > 0) ? $form_state['values']['entity_id'] : 0;
      		$entityType       = $form_state['values']['entity_type'];
            
            if(!empty($attachmentId)) { 
                
                if($form_state['values']['create_user_id'])
                    $loggedInUserId = $form_state['values']['create_user_id'];
                else
                    $loggedInUserId  = getIdOfLoggedInUser();
                
                $attachmentIdArr   = array($form_state["delete_id"]);
                
                expDebug::dPrint('inside attachments');
                $delete_id = db_select('slt_entity_reading_mapper','r');
                $delete_id->addField('r','id','id');
                $delete_id->addField('r','entity_id','entity_id');
                $delete_id->addField('r','entity_type','entity_type');
                $delete_id->addField('r','reading_id','reading_id');
                $delete_id->condition('id', $attachmentIdArr,'IN');
                expDebug::dPrint('slt_entity_reading_mapper table log----');
                expDebug::dPrintDBAPI('$delete_id',$delete_id);
                $del = $delete_id->execute()->fetchAll();
                
                $deleteStmt        = db_delete('slt_entity_reading_mapper');
                $deleteStmt->condition('id', $attachmentIdArr,'IN');
                $numUpdated        = $deleteStmt->execute();      

                //Make delete log entries
                    foreach ($del as $val){
                 $fields = array(
                 'table_name' => 'slt_entity_reading_mapper',
                 'entity_id' => $val->id,
                 'entity_type' => 'attachments',
                 'module_name' => 'exp_sp_administration_attachment',
                 'functionality_name' => 'attachment_addedit_form_html_submit',
                 'api_name' => null,
                 'deleted_on' => now(),
                 'deleted_by' => getIdOfLoggedInUser(),
                 'parent1_entity_id' => $val->entity_id,
                 'parent1_entity_type' => $val->entity_type,
                 'parent2_entity_id' =>$val->reading_id,
                 'parent2_entity_type' => 'reading_id',
                 );
                 deleted_log_entry($fields);
                 }expDebug::dPrint('slt_entity_reading_mapper table log');
                
                $deleteFormState = array();
        	    $deleteFormState['id'] =  $attachmentId;  
        	    make_audit_trail_entries($deleteFormState, getIdOfLoggedInUser() , null ,$entityId, 'Deleted',$entityType,'exp_sp_administration_attachment','old attachment',
                                          array('id')); 
        	  
            
          }  
            
        } catch (Exception $ex) {
          $txn->rollback();
          watchdog_exception('deleteattachment', $ex);
          throw $ex;
        }
    
      unset($txn);
      
      $form_state['rebuild']              = TRUE;
      $form_state['storage']['mode']      = 'view';
  }

  // Update attachment ----------------------------  
  
    $noof_attachment = $form_state['values']["noof_attachment"];
    if($noof_attachment>0) {
      
        try {
        		$entityId         = ($form_state['values']['entity_id'] > 0) ? $form_state['values']['entity_id'] : 0;
      			$entityType       = $form_state['values']['entity_type'];

              for($i=0;$i<$noof_attachment;$i++) {
                
                  if(!empty($form_state['input']['attachment_list'.$i])) {
                    
                    if($form_state["values"]['attachment_list_type'.$i] == 'FILE') {
                      
                      $new_file     = $form_state['input']['attachment_list'.$i];
                      $old_file     = $form_state['values']['attachment_list_content_old'.$i];
                      $path_parts   = pathinfo($new_file);
                      
                      if(!empty($path_parts["filename"]) && !empty($path_parts["extension"])) {
                        $reading_type = $new_file;
                       } else {
                         $reading_type = $old_file;
                       }
                      
                      $reading_content  = '/sites/default/files/attachment/' . $reading_type;//$form_state['input']['attachment_list_original'.$i];
                      //$old_content      = $admin_site_url.'/sites/default/files/attachment/' . $form_state["values"]['attachment_list_original'.$i];
                      $old_content      =  $form_state["values"]['attachment_list_original'.$i];
                      $reading_content  = $old_content;
                      expDebug::dPrint('to renameeeee $reading_content '.$reading_content.' || $old_content '.$old_content , 5);
                      //rename($old_content,$reading_content);
                    } else {
                      $new_file     = trim($form_state['input']['attachment_list'.$i]);
                      $old_file     = $form_state['values']['attachment_list_content_old'.$i];
                      $reading_type    = !empty($new_file) ? $new_file : $old_file;//$form_state['input']['attachment_list'.$i];
                      $reading_content = $reading_type; //$form_state['input']['attachment_list'.$i];
                      
                      if(preg_match('|^http(s)?://[a-z0-9-]+(.[a-z0-9-]+)*(:[0-9]+)?(/.*)?$|i', $reading_content) === 0) {
                         $reading_content = 'http://'.$reading_content;
                      }
                    }
                    
                
                      $updateStmt  = db_update('slt_entity_reading_mapper');
                      $custom      = NULL;     
                      $fields      = array(
                                    'reading_type'       => $reading_type,//$form_state['input']['attachment_list'.$i],
                                    'reading_content'    => $reading_content,//$form_state['input']['attachment_list'.$i],
                                    'showlnr'            => 'Y',
                                    'updated_by'         => 1,
                                    'updated_on'         => now(),
                                    //'custom0'            => $custom,
                                    'custom1'            => $custom,
                                    'custom2'            => $custom,
                                    //'custom3'            => $reading_type,
                                   // 'custom4'            => $custom   
                                  );
                      
                      $updateStmt->fields($fields);    
                      $updateStmt->condition('id', $form_state['values']['attachment_id'.$i]);
                      $numUpdated = $updateStmt->execute();
					  $UpdateFormState = array();
        			  $UpdateFormState['reading_type'] =  $reading_type;
					  $UpdateFormState['id'] =  $form_state['values']['attachment_id'.$i];
					  make_audit_trail_entries($UpdateFormState, getIdOfLoggedInUser() ,'' ,$entityId, 'Updated',$entityType,'exp_sp_administration_attachment','update attachment',
                                         array('reading_type','id'));
					  
					 // make_audit_trail_entries($fields, getIdOfLoggedInUser() , $reading_content ,$entityId, 'Updated',$entityType,'exp_sp_administration_attachment','update attachment',
                                        // array('reading_content'));
                  }
              }
              $form_state['storage']['mode']          = "view";
              $form_state['rebuild']                  = TRUE;
          }
          catch (Exception $ex) {
            $txn->rollback();
            watchdog_exception('updateattachment', $ex);
            throw $ex;
          }
      
    }
   
  
  // Upload attachment ----------------------------
  expDebug::dPrint('Entittty form state before upload '.print_r($form_state["values"],true) , 5);
    if((count($_FILES["files"])>0) && !empty($_FILES['files']['name']['attachment_upload_file'])) {
      
        $txn = db_transaction();
      
        try {
            
             include_once $_SERVER["DOCUMENT_ROOT"]."/includes/file.inc";
             
             $validators  = array('file_validate_extensions' => array('txt doc docx ppt pptx xls xlsx pdf png jpg jpeg gif'));        
             $attachment  = file_save_upload('attachment_upload_file',$validators);
             $currentUserId = getIdOfLoggedInUser();
              if(!empty($attachment)) {

                  if (!$attachment->status) {
  
                    $filedirectory   = file_default_scheme() . '://' . variable_get('user_attachment_path', 'attachment');
  
                    file_prepare_directory($filedirectory, FILE_CREATE_DIRECTORY);
                    
                    // Prepare the unique file name to place the picture under the $filedirectory.
                    $destination     = file_stream_wrapper_uri_normalize($filedirectory . '/'.REQUEST_TIME . '-' .$attachment->filename);
                    
                    // Move the temporary file into the final location.
                    if ($attachmentmove = file_move($attachment, $destination, FILE_EXISTS_RENAME)) {
                                    
                      $attachmentmove->status = FILE_STATUS_PERMANENT;
                      $save_attachment        = file_save($attachmentmove);
                    }
              
             
                  $reading_content  = '/sites/default/files/attachment/' . $save_attachment->filename;
                  $reading_type     = 'FILE';  
                  $is_attachable    = ($save_attachment->filename) ? 1 : 0;  
                  
                  
                  $insertStmt           = db_insert('slt_entity_reading_mapper');
                  $custom               = NULL;
                  $entityId             = $form_state['values']['entity_id'];
                  $entityType           = $form_state['values']['entity_type'];
                  $reading_content_type = $_FILES['files']['name'];
                  $entityId             = ($form_state['values']['entity_id'] < 1) ? 0 : $form_state['values']['entity_id'];
                  $custom0              = ($form_state['values']['entity_id'] < 1) ? $form_state['values']['entity_id'] : NULL;
                  
                  if(!empty($reading_content_type)) {
                    //$reading_content = $_FILES['files']['name'];
                    $filesize   =  $_FILES['files']['size']['attachment_upload_file'];
                    $filesize   = $filesize / 1000;
                    $custom4    = round($filesize, 1);                
                    $xreadingid = getAttachmentCount($entityId,$entityType);
                    	
                    $fields = array(
                              'entity_id'          => $entityId,                  
                              'entity_type'        => $entityType,
                              'reading_id'     	   => $xreadingid,
                              'reading_type'       => $reading_content_type,
                              'reading_content'    => $reading_content,
                              'showlnr'            => 'Y',
                              'created_by'         => $currentUserId,
                              'created_on'         => now(),
                    					'updated_by'         => $currentUserId,
                    					'updated_on'         => now(),
                              'custom0'            => $custom0,
                              'custom1'            => $custom,
                              'custom2'            => $custom,
                              'custom3'            => $reading_type,
                              'custom4'            => $custom4   
                            );
                    $insertStmt->fields($fields);			            
                     
                    $attachmentId = $insertStmt->execute();
                    $attFormState = array();
        			$attFormState['id'] = $attachmentId;
        			$attFormState['reading_type'] =  $form_state['values']['attachment_fieldset']['attachment']['attachment_name_'.$i];
        			$attFormState['reading_content'] = $reading_content;
        			$attFormState['reading_id'] = $xreadingid;
        			$attFormState['custom3'] = $reading_type;
        
        			make_audit_trail_entries($attFormState, getIdOfLoggedInUser() , null ,$entityId, 'Inserted',$entityType,'exp_sp_administration_attachment','new attachment',
                                          array('id', 'reading_type', 'reading_content', 'reading_id', 'custom3'));
        

                  }
                }
                $form_state['storage']['mode']          = "view";
                $form_state['rebuild']                  = TRUE;
              }
             
        } catch (Exception $ex) {
          $txn->rollback();
          watchdog_exception('addattachment', $ex);
          throw $ex;    
        }
        
        unset($txn);
  }
  } catch (Exception $ex) {
  	watchdog_exception('attachment_addedit_form_html_submit', $ex);
  	expertusErrorThrow($ex);
  }
}

function attach_file_trigger_as($form, &$form_state){
  try {
  	
  } catch (Exception $ex) {
  	watchdog_exception('displayAttachCourseDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function exp_sp_administration_attachment_repaint($form, &$form_state) { 
 try {
  $form_state['storage']['mode']    = 'add_url';
  $entityId                         = $form_state['values']['entity_id'];
  $entityType                       = $form_state['values']['entity_type'];
          
  $min_form_state       = array(
   		'ajax'         => TRUE,
   		're_render'    => TRUE,
   		'no_redirect'  => TRUE,
   		'build_info'   => array('args' => array('ajax'))
        );
        
  $form_state                      = $min_form_state;  
  $form_state['storage']['mode']   = 'view';
  $form_state['entity_id']         = $entityId;
  $form_state['entity_type']       = $entityType;
  //$builtForm                       = drupal_build_form('attachment_addedit_form_html', $form_state); 
  $commands                        = array();
  //$commands[]                      = ajax_command_replace(NULL, drupal_render($builtForm));
  $commands[]                      = ajax_command_replace(NULL, drupal_render($form));
  $commands[]                      = ajax_command_prepend(NULL, theme('status_messages'));
  $commands[]                      = exp_ctools_modal_adjust();  
  $commands[]                      = ajax_command_invoke(NULL, "getScrollDive",array($entityId));
  //$commands[]                      = ajax_command_refresh_narrow_search_results('onclose');  
  $form_state['input']             = array();
  $form_state['values']            = array();
  
  return array('#type' => 'ajax', '#commands' => $commands); 
  } catch (Exception $ex) {
  	watchdog_exception('displayAttachCourseDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

?>