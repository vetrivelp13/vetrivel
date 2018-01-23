<?php
/*
 * Display notification details TP/Class to the form
 */
function addNotificationsToAddEditForm(&$form,$entityId,$entityType,$readonly=FALSE){
	try {
  $notificationList  = getNotificationList($entityId,$entityType);
  $notificationId    = 'notification_check';
  if(!empty($notificationList)){
    $form['attr_action'] = array(
      '#type' => 'hidden',
      '#name' => 'attr_action',
      '#value' => 'notifications',
    );  
    /*$form['notification_fieldset'] = array(
        '#type' => 'fieldset',
        '#title' => t('Notification'),
        '#prefix' => '<div id="notification-fieldset-wrapper" class="admin-tab-list-row">',
        '#suffix' => '</div>',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );*/
     $form['notification_fieldset'] = array(
        '#type' => 'markup',
        '#prefix' => '<div id="notification-fieldset-wrapper" class="admin-tab-list-row">',
        '#suffix' => '</div>',
     );  

     $form['notification_fieldset']['note_msg'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="note-msg-txt">'.t('MSG281'),
        '#suffix' => '</div>',
     );
      
    foreach($notificationList as $key=>$val){
      expDebug::dPrint('addNotificationsToAddEditForm =>$key -> $val'.$key.'->'.$val->notifytitle, 4);
      if($val->notifystatus=='N')
        $defaultValue  = 0;
      else
        $defaultValue  = 1;
      addTwoColumnRowToAddEditForm($form, 'notification_frequencylabel_'.$val->notifyid);
      $toolTipClass = ' vtip custom-field-label';
      $toolTipTitle = ' title="'.sanitize_data($val->notifytitle).'"';
      $val->notifytitle = titleController('EXP-SP-ADMIN-NOTIFICATION-TITLE', $val->notifytitle,35);
          
      $form['notification_fieldset'][$notificationId.'_'.$val->notifyid] = array (
        '#type' => 'checkbox',
        '#prefix' => '<div class="addedit-check-notification-firstcol"><div class="addedit-firstcol-field-value"><span class="'.$toolTipClass.'" '.$toolTipTitle.'">',
      	'#suffix' => '</span>',   
        '#title' => $val->notifytitle,
        '#default_value' =>  $defaultValue,
      	'#disabled' => $readonly,
      );
      $form['notification_fieldset']['notification_hidden'][$val->notifyid] = array('#type' => 'hidden', '#value' => $val->notifyid);
      
      if($val->frequencyvalue!=''){
        $form['notification_fieldset']['notification_frequency_'.$val->notifyid] = array (
          '#type' => 'textfield',
          '#attributes' => array('class' => array('notify-freq-value')),
          '#suffix' => '</div></div>',          
		  '#size' => 2,
          '#maxlength' => 2,
          '#default_value' =>  $val->frequencyvalue,
          '#disabled' => $readonly,
        );
      }else{
        $form['notification_fieldset']['notification_frequency_'.$val->notifyid] = array (
          '#type' => 'item',
          '#title'=> '',
          '#suffix' => '</div></div>',
          '#attributes' => array('class' => array('notify-freq-value')),
        );
      }
      endAddEditTwoColumnRow($form);
    }//end foreach
  }//end if
  } catch (Exception $ex) {
  	watchdog_exception('addNotificationsToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}//end addNotificationsToAddEditForm()

/*
 * List all the notification settings for the TP/Class
 */
function getNotificationList($entityId,$entityType){
	try {
  expDebug::dPrint(' $entityId,$entityType => '.$entityId.','.$entityType, 4);
  $selectStmt = db_select('slt_notification_info', 'notify');
  $selectStmt->leftjoin('slt_entity_notification_mapping', 'map', 'notify.id=map.notify_id and map.entity_id=\''.$entityId.'\' and map.entity_type=\''.$entityType.'\'');
  $selectStmt->join('slt_notification_frame', 'frame', 'frame.notification_id=notify.id');
  // Set conditions
  $selectStmt->condition('notify.status', 'cre_ntn_sts_atv');
  $selectStmt->condition('notify.entity_type', $entityType);
  
  // Select fields and/or add expressions
  $selectStmt->addField('notify','id', 'NotifyId'); 
  $selectStmt->addField('notify', 'notification_title', 'NotifyTitle');
  $selectStmt->addField('notify', 'notification_code', 'NotifyCode');
  $selectStmt->addField('map', 'id', 'MapTableId');
  $selectStmt->addField('map', 'notify_text', 'NotifyAppendText');
  $selectStmt->addField('map', 'notify_id', 'NotifyMapId');
  $selectStmt->addField('map', 'status', 'NotifyStatus');
  $selectStmt->distinct(); 
  $selectStmt->addExpression( "IFNULL(map.notify_frequency_type, notify.frequency_data_type)", "FrequencyType" );
  $selectStmt->addExpression( "IF(map.notify_frequency>0, map.notify_frequency, notify.frequency_data_value)", "FrequencyValue" );
  $selectStmt->addExpression( "IFNULL(map.notify_send_type, frame.send_options)", "NotifySendType" );
  $result = $selectStmt->execute()->fetchAll();
  expDebug::dPrint(' $result = ' . print_r($result, true),3);
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('getNotificationList', $ex);
  	expertusErrorThrow($ex);
  }
}//end getNotificationList//end getNotificationList()

function getNotificationCertificationList($entityId, $entityType,$mapped_cert= false) {
	try {
  $debugLevel = strtolower(trim($dbug["debug_level"]));
  expDebug::dPrint(' $entityId, $entityType => ' .$entityId. ',' . $entityType, 4);
  /*-- #35127 : certificate lang selection based on the entity type ---*/
  $courseTable = ($entityType == 'cre_sys_obt_trp') ? 'slt_program' : 'slt_course_template';
  $selectStmt = db_select('slt_notification_info', 'ninfo');
  $selectStmt->join('slt_notification_frame', 'nframe', 'ninfo.id = nframe.notification_id');
  $selectStmt->leftJoin('slt_entity_notification_mapping',
                               'nmap', 'ninfo.id = nmap.notify_id AND nmap.entity_id = :entity_id AND nmap.entity_type = :entity_type',
                                                                                 array(':entity_id' => $entityId, ':entity_type' => $entityType));  
  // Select fields and/or add expressions
  $selectStmt->addField('ninfo','id', 'notifyid');
  $selectStmt->addField('ninfo', 'notification_title', 'notifytitle');
  $selectStmt->addField('nmap', 'status', 'mapped');
  //@TODO: system default language should be from language setting in exp_sp.ini file. The template code can also be configurable
  $selectStmt->addExpression('IF(ninfo.lang_code = :langcode AND ninfo.notification_code = :certicode, CONCAT(\'Y\'), CONCAT(\'N\'))',
                                        'systemdefault',
                                          array(':langcode' => 'cre_sys_lng_eng', ':certicode' => 'prncert'));
   //added for ticket no-0020671: Incorrect print certificate template selection method - shobana                                       
  if(!empty($entityId)) {                                        
  $selectStmt->leftJoin($courseTable, 'sct', 'nframe.lang_code = sct.lang_code'); 
  $selectStmt->condition('sct.id', $entityId, ' = ');
  }else {
  	$selectStmt->condition('nframe.lang_code', 'cre_sys_lng_eng');
  }  
  if($mapped_cert== true)
  {
  	$selectStmt->condition('nmap.status', 'Y');
  } else{
  	filterByUserAccessPrivileges($selectStmt, 'ninfo','',1,'cre_cer','','ninfo');
  }
  
  // Set conditions
 
  $selectStmt->condition('ninfo.status', 'cre_ntn_sts_atv');
  $selectStmt->condition('nframe.send_options', 'Certificate');
  $selectStmt->orderBy('ninfo.notification_title');
  //Group by id
  $selectStmt->groupby('ninfo.id');
  // Order results by title
  $selectStmt->orderBy('ninfo.notification_title', 'ASC');
  expDebug::dPrintDBAPI(' $selectStmt SQL = ' , $selectStmt);
  $result = $selectStmt->execute()->fetchAll();
  expDebug::dPrint(' $result = ' . print_r($result, true),3);
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('getNotificationCertificationList', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getMappedCertificateTempateIdxFromList() - From the list returned by getNotificationCertificationList(), this function returns the
 *                                            index of the record that is to be shown as mapped to the admin user
 */
function getMappedCertificateIdxFromList($certificationList) {
	try {
	$numTemplates = count($certificationList);
  // Find mapped template idx
  // Also in the same loop find the system default template that is to be shown as selected by default in case no template is mapped
  $mappedTemplateIdx = -1;
  $defaultTemplateIdx = -1;    
  for ($i = 0; $i < $numTemplates; $i++) {
    $val = $certificationList[$i];
    if ($mappedTemplateIdx < 0 && $val->mapped == 'Y') {
      $mappedTemplateIdx = $i;
      break; // If a template is mapped, we can break out of the loop
    }

    // Also find index of system default certificate template. It is the first english certificate having code prncert
    if ($defaultTemplateIdx == -1 && $val->systemdefault == 'Y') {
      $defaultTemplateIdx = $i;
    }
  } // end for loop

  // If no template is mapped, show default template as mapped
  if ($mappedTemplateIdx < 0) {
    $mappedTemplateIdx = $defaultTemplateIdx;
  }
  
  return $mappedTemplateIdx;
  } catch (Exception $ex) {
  	watchdog_exception('getMappedCertificateIdxFromList', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * Add/Update notification settings for TP/Class
 */
function updateNotificationList($entityId,$entityType,$notifyArray){
	try {
  $newFreqVal  = '';
  $insertFlag  = false;
  $notifyHiddenValues  = $notifyArray['notification_fieldset']['notification_hidden'];
  expDebug::dPrint(' $notifyHiddenValues = ' . print_r($notifyHiddenValues, true),5);
  
  $num_deleted = db_delete('slt_entity_notification_mapping')
  ->condition('entity_id', $entityId)
  ->condition('entity_type', $entityType)
  ->condition('notify_send_type', 'Email')
  ->execute();
  $txn = db_transaction(); 
  
  $isAdded = 0; 
  foreach($notifyHiddenValues as $key=>$val){
      $insertFlag  = false;
      $formFreqVal  = $notifyArray['notification_fieldset']['notification_frequency_'.$val];
      $formNotifyStatus  = $notifyArray['notification_fieldset']['notification_check_'.$val];
      if($formFreqVal!=''){
        $freqSelect = db_select('slt_notification_info','info');
        $freqSelect->condition('info.id', $key);
        $freqSelect->addField('info','frequency_data_value');
        $oldFreqVal= $freqSelect->execute()->fetchField();
        $newFreqVal  = $formFreqVal;
        if($oldFreqVal!='' && $oldFreqVal>0 && $oldFreqVal!=$newFreqVal)
          $insertFlag  = true;
      }  
      
      expDebug::dPrint(' $ = ' . print_r($formNotifyStatus, true),4);
      
      if($formNotifyStatus==0){
        $status='N';
        $insertFlag = true;
      }else{
        $status='Y';
      }
      try {      
        if($insertFlag){
          $insertStmt = db_insert('slt_entity_notification_mapping');
          $insertStmt->fields(array(
            'entity_id' => $entityId,
            'entity_type' => $entityType,
            'notify_id' => $key,
            'notify_frequency' => $newFreqVal,
            'notify_send_type' => 'Email',
            'status' => $status
            ));
          expDebug::dPrintDBAPI(' $insertStmt = ' ,$insertStmt);
          $mapId = $insertStmt->execute();
          $isAdded = 1;
        }
      }catch (Exception $ex) {
        $txn->rollback();
        watchdog_exception('updateattachment', $ex);
        throw $ex;
      }
  }//end foreach
  if($isAdded){
    drupal_set_message(t('MSG601').'.');
  }
  unset($txn);       
  } catch (Exception $ex) {
  	watchdog_exception('updateNotificationList', $ex);
  	expertusErrorThrow($ex);
  }
}//end updateNotificationList()

/*
 * Validation on notification save
 */
function addEditNotificationMandatoryCheck($form,&$form_state){
 //notification frequency validation
 try {
 $notifyHiddenValues  = $form_state['values']['notification_fieldset']['notification_hidden'];
 foreach($notifyHiddenValues as $key=>$val){
   if(isset($form_state['values']['notification_fieldset']['notification_frequency_'.$val]) && isset($form_state['values']['notification_fieldset']['notification_check_'.$val])){
      $formFreqVal  =  $form_state['values']['notification_fieldset']['notification_frequency_'.$val];
      if (empty($formFreqVal)) {
        expDebug::dPrint(' addEditCheckMandatoryFields() : ' . $form['notification_fieldset']['notification_check_'.$key]['#title'] . ' is missing.' , 5);
        form_set_error('notification_fieldset][notification_frequency_'.$key, $form['notification_fieldset']['notification_check_'.$key]['#title'] . t('ERR091'));
         $form_state['esign_show_popup']= false;
      }else if (!is_numeric($formFreqVal)) {
        form_set_error('notification_fieldset][notification_frequency_'.$key, $form['notification_fieldset']['notification_check_'.$key]['#title'] . t('ERR092'));
        $form_state['esign_show_popup']= false;
      }
   }
 }//End notficatin frequency validation
 } catch (Exception $ex) {
 	watchdog_exception('addEditNotificationMandatoryCheck', $ex);
 	expertusErrorThrow($ex);
 }
}//end addEditNotificationMandatoryCheck()

/*
 * printcertificate_addedit_form() - not used anywhere
 */
function printcertificate_addedit_form($js = NULL, $entityId, $entityType,$entityTitle) {
	try {
  expDebug::dPrint(":Calling form stateeeee : id ".$entityId."Type".$entityType, 4);
  if ($js) {
    ctools_include('modal');
    ctools_include('ajax');
  
    $form_state           = array();
    $catalogTitle             =  titleController('EXP-SP-ADMIN-NOTIFICATION-ENTITYTITLE', $entityTitle,36);
    $form_state['title']  = t('LBL340').' - '.$catalogTitle;
    $form_state['ajax']   = TRUE;
    
    if (!empty($entityId)) {
      $form_state['build_info'] = array('args' => array('ajax', $entityId, $entityType));
    }
  
    $output = ctools_modal_form_wrapper('exp_sp_administration_course_print_certificate_form', $form_state);

    if (!empty($form_state['executed'])) {
      $output = array();
      $inplace = ctools_ajax_text_button(t('LBL338'), ('administration/print-certificate/nojs/'.$entityId.'/'.$entityType.'/'.$entityTitle), t('LBL339'));
	  $close = ctools_ajax_text_button(t('LBL123'), 'administration/print-certificate/nojs/close', t('LBL123'));
      $output[] = ctools_modal_command_display(t('LBL340').' - '.$entityTitle, '<div class="modal-message catalog-control-success-msg">Tags details saved successfully.</br></br><div class="admin-catalog-control-actions">'. $inplace .'<span>or</span>' .$close.'</div></div>');
    }
    print ajax_render($output);
    exit;
  }
  else {
    return drupal_get_form('exp_sp_administration_course_print_certificate_form', 'nojs', $entityId, $entityType);
  }
  } catch (Exception $ex) {
  	watchdog_exception('printcertificate_addedit_form', $ex);
  	expertusErrorThrow($ex);
  }
}



function exp_sp_administration_course_print_certificate_form($entityId, $entityType, $emptyId){
 try {
  $form_state = array(
   'no_redirect' => TRUE, // To confirm whether necessary
   'build_info' => array('args' => array('ajax')), // The form builder function expects the first argument to be ajax or nojs.
   'entity_id' => $entityId,
   'entity_type' => $entityType,
   'empty_id' => $emptyId   
  );      
  
  $formId = 'exp_sp_administration_course_print_certificate_form_html';
  $builtForm = drupal_build_form($formId, $form_state);
  $renderedForm = drupal_render($builtForm);

  $drupalSettings = getDrupalSettings();

  $certificateResponse = array(
    'drupal_settings' => $drupalSettings,
    'render_content_main' => $renderedForm,

  ); 

  echo drupal_json_output($certificateResponse);
  exit;
  } catch (Exception $ex) {
  	watchdog_exception('exp_sp_administration_course_print_certificate_form', $ex);
  	expertusErrorThrow($ex);
  }
}

function exp_sp_administration_course_print_certificate_form_html($form, &$form_state, $renderMode, $entity_id, $entityType) {
	try {
  include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_notification.inc');  
  $css_theme_optional = array('type' => 'file', 'group' => CSS_THEME);
  global $theme_key;
  	if($theme_key == 'expertusoneV2') {
  		//NEWUI THEME STYLE SHEET
  		drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_v2.css', $css_theme_optional);
  	} else {
		drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration.css', $css_theme_optional);  		
  	}
  //drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration.css', $css_theme_optional);
  
  $dynamicwrapperid = "certificate-addedit-form" . $form_state['entity_id'] . $form_state['entity_type'];
  $popuprenderCertsId = "paintContentqtip_visible_certificatedisp_" . $form_state['entity_id'] . '_' . $form_state['entity_type'];
  
  $form = certificatenewAddEditFormWithCancelAndSave($dynamicwrapperid);
  
  addHiddenValueToAddEditForm($form, 'entity_id', $form_state['entity_id']);
  addHiddenValueToAddEditForm($form, 'entity_type', $form_state['entity_type']);
  addHiddenValueToAddEditForm($form, 'empty_id', $form_state['empty_id']);
  
  $form['attr_action'] = array(
      '#type' => 'hidden',
      '#name' => 'attr_action',
      '#value' => 'certificate',
  );
  
  $certificationList  = getNotificationCertificationList($form_state['entity_id'], $form_state['entity_type']);  
  $Attachedcertification  = getNotificationCertificationList($form_state['entity_id'], $form_state['entity_type'],true);
  
  if(!empty($Attachedcertification)){
  	
  for($k=0;$k<count($certificationList);$k++)
  {
 
  	if($certificationList[$k]->notifyid == $Attachedcertification[0]->notifyid)
  		{
  			unset($Attachedcertification[0]);
  		} 

  } 
  if(!empty($Attachedcertification))
  $certificationList[] = $Attachedcertification[0];
  }
  if (!empty($certificationList)) {
     $form['certificate_table_start'] = array(
        '#type' => 'markup',
        '#markup' => '<div id=certificate-display-table><table width="100%" cellpadding="4" cellspacing="0" border="0">' .
                       '<tr>' .
                         '<td colspan="2" class="info-text">' . t('MSG375') . '</td>' .
                       '</tr>',
     );
     
     $mappedTemplateIdx = getMappedCertificateIdxFromList($certificationList);
     $numTemplates = count($certificationList);
     $readOnlyType = $readonly == true ? 'disabled="disabled"' : '';
     for ($i = 0; $i < $numTemplates; $i++) {
     	 $val = $certificationList[$i];
       $form['certificate_table_row' . $i] = array(
	       '#type' => 'markup',
	       '#markup' => '<tr>' .
	                       '<td width="98%" class="certificate-column">' .
	                         '<span class="vtip" title="'.sanitize_data($val->notifytitle).'">'.
                              titleController('ADDEDIT-CATAOG-CERTIFICATE-NAME', $val->notifytitle,45) .
                           '</span>' .
                         '</td>' .
                         '<td>' .
                           '<input type="radio" name="attach_certificate" ' . $readOnlyType . ' value="' . $val->notifyid . '" ' .
                                            ($i == $mappedTemplateIdx ? "checked='checked'" : "").'>' .
                         '</td>' .
                       '</tr>',
	      );
    } // end for loop
    
    $form['certificate_table_end'] = array(
        '#type' => 'markup',
        '#markup' => '</table></div>',
     );  
    //}
  } else {
      $form['no-records-msg'] = array(
      	'#type' => 'markup',
      	'#markup' => '<div id="no-certificate-msg" class="no-records-msg">'.t('MSG278').'</div>'
      );
  }
  //end ifs

  if(!empty($numTemplates))
 		certificateAddEditFormWithCancelAndSaveClose($form,$dynamicwrapperid,'newtheme-save-button','newtheme-cancel-link',$popuprenderCertsId,$numTemplates);
 
  return $form;
  } catch (Exception $ex) {
  	watchdog_exception('exp_sp_administration_course_print_certificate_form_html', $ex);
  	expertusErrorThrow($ex);
  }
} 

function exp_sp_administration_course_print_certificate_form_html_submit($form, &$form_state){
  try {
    expDebug::dPrint(":Calling form submit calling : ".print_r($form_state['entity_id'],true),5);
    updateCertificationList($form_state['entity_id'], $form_state['entity_type'],$form_state['empty_id'], $form_state['values'],$form_state);
    expertus_set_message(t('MSG601').'.');
    //drupal_set_message(t('Updated the success'));
    $form_state['rebuild'] = TRUE; 
    } catch (Exception $ex) {
    	watchdog_exception('exp_sp_administration_course_print_certificate_form_html_submit', $ex);
    	expertusErrorThrow($ex);
    }
}

/*
 * Display Certificate details TP/Course to the form
 * 
 */
function displayCertificateToAddEditForm(&$form, $form_state, $entityId, $entityType, $uniqueid, $readonly=FALSE){
	try {
  $form['attr_action'] = array(
      '#type' => 'hidden',
      '#name' => 'attr_action',
      '#value' => 'certificate',
  );
  
  $certificationList  = getNotificationCertificationList($entityId,$entityType); 
  if (!empty($certificationList)) {   
    $form['certificate_table_start'] = array (
      '#type' => 'markup',
      '#markup' => '<div id="certificate-fieldset-wrapper" class="admin-tab-list-row"></div>' .
                   '<table width="98%" cellpadding="4" cellspacing="0" border="1">' .
                     '<tr>' .
                       '<td class="businessrules-table-title">Certificate Template Name</td>' .
                       '<td class="businessrules-table-title businessrules-add-border" width="40">Select</td>' .
                      '</tr>',
     );  

     $mappedTemplateIdx = getMappedCertificateIdxFromList($certificationList);
     $numTemplates = count($certificationList);
     $readOnlyType = $readonly == true ? 'disabled="disabled"' : '';    
     for ($i=0; $i < $numTemplates; $i++) {
       $form['certificate_table_row'.$i] = array(
	       '#type' => 'markup',
	       '#markup' => '<tr>' . 
                        '<td class="businessrules-table-value">' . $val->notifytitle . '</td>' .
                        '<td class="businessrules-table-value businessrules-add-border">' .
                          '<input type="radio" name="attach_certificate" ' . $readOnlyType . ' value="' . $val->notifyid . '" ' .
                                              ($i == $mappedTemplateIdx? "checked='checked'" : "") . '>' .
                        '</td>' .
                      '</tr>'
	    );
    }//end foreach
    
    $form['certificate_table_end'] = array(
        '#type' => 'markup',
        '#markup' => '</table>',
     );
  }else{
      $form['no-records-msg'] = array(
      	'#type' => 'markup',
      	'#markup' => '<div class="no-records-msg">'.t('There are no records.').'</div>'
      );
  }
  //end if
  } catch (Exception $ex) {
  	watchdog_exception('displayCertificateToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
} //end displayCertificateToAddEditForm()

/*
 * Add/Update certification settings for TP/Course
 */
function updateCertificationList($entityId, $entityType, $emptyId, $form_state_values, $form_state){
	try {
  expDebug::dPrint(' inputttttttttttt = ' . print_r($form_state['input'],true),5);
  expDebug::dPrint(' inputttttttttttt11 = ' . print_r($form_state,true),5);

  $notifyid  = $form_state['input']['attach_certificate'];
  //attach_certificate
  
  expDebug::dPrint(' $notifyid = ' . $emptyId , 4);
  
  
 if($entityId != 0){
  	$emptyId=NULL;
  }
  
  $num_deleted = db_delete('slt_entity_notification_mapping')
  ->condition('entity_id', $entityId)
  ->condition('entity_type', $entityType)
  ->condition('notify_send_type', 'Certificate')
  ->execute();
  $txn = db_transaction(); 
  $insertFlag = ($notifyid) ? 1 : 0;
  $isAdded = 0; 
  try {      
    if($insertFlag){
      $insertStmt = db_insert('slt_entity_notification_mapping');
      $insertStmt->fields(array(
        'entity_id' => $entityId,
        'entity_type' => $entityType,
        'notify_id' => $notifyid,
        'notify_send_type' => 'Certificate',
        'status'    => 'Y',
        'custom0'   => $emptyId,  
        ));
        
      expDebug::dPrintDBAPI(' $insertStmt = ' ,$insertStmt);
      $mapId = $insertStmt->execute();
      $updatecertification[entity_id] = $mapId;
      $updatecertification[entity_type] = $entityType;
      $updatecertification[notify_id] = $notifyid;
      $updatecertification[notify_send_type] = 'Certificate';
      $updatecertification[status] = 'Y';
      make_audit_trail_entries($updatecertification, getIdOfLoggedInUser() , null, $entityId ,'Updated', $entityType, 'exp_sp_administration_notification','updateCertificationList',
                                          array('entity_id', 'entity_type', 'notify_id', 'notify_send_type', 'status'));
    	 $isAdded = 1;
    }
  }catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('updateattachment', $ex);
    throw $ex;
  }
  /*
  if($isAdded){
    drupal_set_message(t('MSG305'));
  }*/
  unset($txn);       
  } catch (Exception $ex) {
  	watchdog_exception('updateCertificationList', $ex);
  	expertusErrorThrow($ex);
  }
}//end updateCertificationList()

/**
 * _updateCertificateEntity()
 * @param Entity Id
 * @param Entity Type
 * @param Empty ID 
 */
function _updateCertificateEntity($entityId, $entityType, $emptyId){
  try {
  $fields = array();
  $fields['custom0'] = NULL;
  $fields['entity_id'] = $entityId;
  
  // Update catalog certificate entry 
  $catalogUpdate = db_update('slt_entity_notification_mapping');
  $catalogUpdate->fields($fields);
  $catalogUpdate->condition('custom0', $emptyId);
  $catalogUpdate->condition('entity_type', $entityType);
  $catalogUpdate->execute();
  } catch (Exception $ex) {
  	watchdog_exception('_updateCertificateEntity', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * certificatenewAddEditFormWithCancelAndSave() - Use this function to create your addedit form having Cancel and Save buttons at the bottom 
 */
function certificatenewAddEditFormWithCancelAndSave($wrapperId = 'addedit-form-wrapper', $miscAction = array()) {
  try {
  $form = array();
  $form['#prefix'] = /* (( stripos($_SERVER['HTTP_USER_AGENT'], 'Chrome') || stripos ($_SERVER['HTTP_USER_AGENT'], 'Firefox') )? '</form>' : '' ) */ '</form>'.'<div id="' . $wrapperId . '" class="addedit-form-wrapper">';
  $form['#suffix'] = '</div>';  
  
  // Add a class to the form so that we can apply custom style (margin, padding, etc).  
  $form['#attributes'] = array('class' => array('addedit-form'));
  $form['#attributes']['autocomplete'] = 'off';
  $form['#tree'] = TRUE; // Show full path of values (hierarchical values)
  
  $form['addedit-left-col-start'] = array(
    '#type' => 'markup',
    '#markup' => '<div>',
  );

  // These hidden form fields are needed to be able to communicate to ctools modal to refresh grid on close after a save op
  //addHiddenValueToAddEditForm($form, 'js_object_init_id', 'root-admin');
  //addHiddenValueToAddEditForm($form, 'js_object', 'narrowsearch');
      
  return $form;
  } catch (Exception $ex) {
  	watchdog_exception('certificatenewAddEditFormWithCancelAndSave', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * certificateAddEditFormWithCancelAndSaveClose() - Closure for newAddEditFormWithCancelAndSave()
 */
function certificateAddEditFormWithCancelAndSaveClose(&$form, $wrapperId = '', $saveButtonId = 'save-button', $cancelButtonId = 'cancel-button', $popuprenderCertsId = "", $expandPopup = FALSE, $saveTabIndex = '', $esignature=false) {
	try {
  $customHiddenButtonId = 'esign-button-'.$wrapperId;

  // Remove the temporary storage for API's to exchange information during form creation
  unset($form['addedit_tmp_storage']);
  
  if (isset($saveTabIndex) && $saveTabIndex > 0) {
    $saveAttributes['tabindex'] = $saveTabIndex;
    $cancelAttributes['tabindex'] = $saveTabIndex+1;
  }
  
  // Add the Cancel button
  $ajaxCallbackCancel = 'addEditCancelRepaintForm';
  
  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $cancelAttributes['class'] = array('addedit-edit-' . $cancelButtonId); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
  
  // For common styling of all addedit cancel buttons
  //$cancelAttributes['class'][] = 'addedit-edit-cancel';
  
  // Using this class to applying bg color to the save button
  $cancelAttributes['class'][] = 'admin-action-button-middle-bg';

  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-bubble-close';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  // Also used by multiselect to show input as selected for only the current form
  $cancelAttributes['data-wrapperid'] = array($wrapperId);
  
  // A common class for all addedit form cancel button, for styling to appear as a link.
  $cancelAttributes['class'][] = 'exp-addedit-form-cancel-button white-btn-bg-middle';
  $cancelAttributes['class'][] = 'tags-close-button';
  $cancelAttributes['onclick'] = "closeQtip('','$cancelButtonId');  return false; ";
  /*if(empty($entityId)){
    //$cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss();return false;';
     
  }*/
  
  $form[$cancelButtonId] = array(
    '#type' => 'submit',
    '#prefix' => '<div class="addedit-form-cancel-and-certificate-done-actions-row certificate-btn-container">' .
     			'<div class="addedit-form-cancel-container-actions resize-save_btn"><div class="white-btn-bg-left"></div>',
    '#suffix' =>   '<div class="white-btn-bg-right"></div>',
    '#value' => t('LBL123'),
    '#attributes' => $cancelAttributes,
  );
  
  // Add the Save button
  $ajaxCallbackSave = 'addEditRepaintForm';
  
  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $saveAttributes['class'] = array('addedit-edit-' . $saveButtonId); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // For common styling of all addedit save buttons
  //$saveAttributes['class'][] = 'addedit-edit-save';
  
  // Using this class to applying bg color to the save button
  $saveAttributes['class'][] = 'admin-save-button-middle-bg';

  //Increase the width of the popup while clicking on save button
  if ($wrapperId != 'resource-addedit-form' && $wrapperId != 'organization-addedit-form' && $wrapperId != 'user-org-addedit-form'  && $wrapperId != 'user-addr-addedit-form' && $expandPopup) {
  $saveAttributes['onclick'][] = 'increasePopWidth();';
  }
  
  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  // Also used by multiselect to show input as selected for only the current form
  $saveAttributes['data-wrapperid'] = array($wrapperId);
  
/*	  $form[$saveButtonId] = array(
	    '#type' => 'submit',
	    '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
	    '#suffix' => 
	               '<div class="admin-save-button-right-bg"></div></div>' .
	               '<div class="clearBoth"></div></div>' .
	             '<div class="clearBoth"></div></div>',
	    '#value' => t('LBL141'),
	    '#name' => 'save',
	    '#attributes' => $saveAttributes,
	    '#ajax' => array(
	      'wrapper' => $wrapperId,
	      'callback' => $ajaxCallbackSave,
	      'method' => 'replace',
	      'effect' => 'none', // 'fade',
	      'event'=>'click',
	      'keypress' => true,
	      'progress' => array(
	        'type' => 'throbber',
	        'message' => ''
	      ),
	    )
	  );
  
  	 //$saveAttributes['onclick'] = 'hideMessageInfo();';
  	 $saveAttributes['class'] = array('common-esign-save');//IMPORTANT!!!!!!! do not use this class anywhere else -- only for esign click ------- IMPORTANT!!!!!!!
  	 $form['common-esign-save'] = array(
	      '#type' => 'submit',
	      '#value' => t('LBL141'),
  	      '#name' => 'save',
      	'#prefix' => '<span id = "esign-admin-div" style="display:none;">',
	      '#suffix' => '</span>',
	      
  		  '#attributes' => $saveAttributes,
	      '#ajax' => array(
		      'wrapper' => $wrapperId,
		      'callback' => $ajaxCallbackSave,
		      'method' => 'replace',
		      'effect' => 'none', // 'fade',
		      'event'=>'click',
		      'keypress' => true,
		      'progress' => array(
		        'type' => 'throbber',
		        'message' => ''
		      ),
	    	)
	      );
*/  
  
  
      //$saveAttributes['onclick'][] = '$("#'.$customHiddenButtonId.'").click();';	 
      $form[$saveButtonId] = array(
	    '#type' => 'submit',
	    '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
	    '#suffix' => 
	               '<div class="admin-save-button-right-bg"></div></div>' .
	               '<div class="clearBoth"></div></div>' .
	             '<div class="clearBoth"></div></div>',
	    '#value' =>  t('LBL569'),
      	'#name' => 'save',
	    '#attributes' => $saveAttributes,
        '#ajax' => array(
  	      'wrapper' => $wrapperId,
  	      'callback' => $ajaxCallbackSave,
          'triggerEvent' => 'hidden-save',
  	      'method' => 'replace',
  	      'effect' => 'none', // 'fade',
  	      'event'=>'click',
  	      'keypress' => true,
  	      'progress' => array(
  	        'type' => 'throbber',
  	        'message' => ''
  	      ),
      	)
	  );

  $form['addedit-left-col-end'] = array(
    '#type' => 'markup',
    '#markup' => '</div><div class="clearBoth"></div>',
  );
  } catch (Exception $ex) {
  	watchdog_exception('certificateAddEditFormWithCancelAndSaveClose', $ex);
  	expertusErrorThrow($ex);
  }
}
?>