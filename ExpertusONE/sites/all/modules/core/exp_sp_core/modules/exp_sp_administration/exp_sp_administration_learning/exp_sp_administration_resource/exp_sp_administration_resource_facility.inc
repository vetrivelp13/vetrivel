<?php 
/**
 * exp_sp_administration_resource_facility_title_form().
 */

function exp_sp_administration_resource_facility_title_form($form, &$form_state, $renderMode, $locationId, $facilityId){
try {
  if(empty($locationId) && !empty($form_state['storage']['new']['location_id'])) {
      $locationId = $form_state['storage']['new']['location_id'];
      $facilityId  = $form_state['storage']['new']['facility_id'];
  }
  
   $form['edit-facility-details-start'] = array(
      '#prefix' => "<div id='resource-facility-addedit-form-details'>",
    );
  if(!empty($locationId)) {  
    $form_state['storage']['locationId'] = $locationId;
  }
  unset($addAttributes['onclick']);
  
  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $addAttributes['class'][] = 'addedit-form-expertusone-throbber narrow-search-actionbar-btnBG';
  $addAttributes['onclick'][] = '$(document).ready(function(){ addFacilityDetails('.$locationId.') }); remove_messages("facility");';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  $addAttributes['data-wrapperid'] = array('resource-facility-addedit-form-details');
  
   $form['add-facility-list'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="add-facility-list-wrapper">'
  ); 
  
  $form['add-facility-button'] = array(
    '#type' => 'submit',
    '#prefix' => '<div class="admin-save-button-container add-class-button"><div class="narrow-search-actionbar-btnLeft"></div>',
    '#suffix' => '<div class="narrow-search-actionbar-btnRight"></div></div>',
    '#value' => t('LBL408'),
    '#name' => 'Add Facility',
    '#limit_validation_errors' => array(),
    '#submit' => array('edit_facility'), 
    '#attributes' => $addAttributes,
    '#ajax' => array(
      'wrapper' => 'add-facility-wrapper',
      'callback' => 'exp_sp_administration_resource_facility_callback',
      'location_id' => $locationId,
      'facility_id' => '',
      'event' => 'click',
    ),
  );
  
  $facList       = getAdministrationFacilityDetails($locationId);
  $facListCount  = count($facList);
  
  if($facListCount == 0){

     $form['no-facility-msg'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="no-records-msg" id="add-edit-facility-norecords">'.t('MSG326').'</div>'
    );  
    
  }

  for ($i = 0; $i < $facListCount; $i++) {
    
    $facilityTitle = $facList[$i]->fac_name;
	$displayTitle = titleController('EXP-SP-ADMIN-RESOURCE-FACILITY-TITLE', $facilityTitle,65);
    
    $form['edit-facility-list-'.$i] = array(
      '#type' => 'markup',
      '#prefix' => "<div id='edit-facility-list-".$facList[$i]->fac_id."' class='edit-class-list'><table cellpadding='5' cellspacing='5' border='0' width='100%'><tr><td><div class='narrow-search-results-item-title'>",
      '#suffix' => "<span title='".$clsList[$i]->fac_name."'>".$displayTitle."</span></div></td>",        
    );
    
    // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
    $editAttributes['class'][] = 'addedit-form-expertusone-throbber';
    $editAttributes['class'][] = 'addedit-form-class-button';
    $editAttributes['class'][] = 'admin-add-edit-button';
    unset($editAttributes['onclick']);
    unset($editAttributes['onload']);
    $editAttributes['onclick'][] = '$(document).ready(function(){ editFacilityDetails('.$locationId.','.$facList[$i]->fac_id.') }); remove_messages("facility");';
    
    // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
    $editAttributes['data-wrapperid'] = array('resource-facility-addedit-form-details');  
    $ajaxEvent = 'click';
    if($facList[$i]->fac_id == $facilityId){
      $ajaxEvent = 'load click';
      $editAttributes['onload'][] = '$(document).ready(function(){ editFacilityDetails('.$locationId.','.$facList[$i]->fac_id.') }); remove_messages("facility");';
    } 
    if($facList[$i]->fac_name!='')
      $facilityName  = '<span class="narrow-search-results-item-detail-pipe-line">|</span><span title="'.t('LBL312').'">'.$facList[$i]->fac_name.'</span>';
    else
      $facilityName  = '';
      
    $form['edit-facility-list-button-'.$i] = array( 
      '#type' => 'submit',
      '#name' => 'edit-facility-'.$locationId.'-'.$facList[$i]->fac_id,
      '#prefix' => '<td class="edit-class-list-right"><br /><div class="admin-add-button-container">',
      '#suffix' => '</div></td></tr><tr><td colspan="2" class="edit-class-list-left"><div class="narrow-search-results-item-detail">Facility<span class="narrow-search-results-item-detail-pipe-line">|</span><span title="'.t('LBL038').'">'.$facList[$i]->status.'</span></div></td></table></div><div id="edit-facility-wrapper-'.$facList[$i]->fac_id.'" ></div>',
      '#submit' => array('edit_facility'),
      '#attributes' => $editAttributes,
      '#ajax' => array(
         'callback' => 'exp_sp_administration_resource_facility_callback',
         'wrapper' => 'edit-facility-wrapper-'.$facList[$i]->fac_id,
         'loop_id' => $i,
         'facility_id' => $facList[$i]->fac_id,
         'location_id' => $locationId,
         'event' => $ajaxEvent,
       ),  
    );
  }

  $form['add-facility'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="add-facility-wrapper"></div></div>'
  ); 
  
  $form['edit-facility-details-end'] = array(
      '#suffix' => "</div>",        
    );
  return $form;
  } catch (Exception $ex) {
  	watchdog_exception('exp_sp_administration_resource_facility_title_form', $ex);
  	expertusErrorThrow($ex);
  }
}


function edit_facility($form, &$form_state) { 
	try { 
  $locationId = $form_state['triggering_element']['#ajax']['location_id'];
  $facilityId = $form_state['triggering_element']['#ajax']['facility_id'];
  
  $form_state['facility_id'] = $facilityId;
  $form_state['location_id'] = $locationId;

  $form_state['rebuild'] = TRUE;
  } catch (Exception $ex) {
  	watchdog_exception('edit_facility', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * exp_sp_administration_resource_facility_callback() - Multi-Form implementation for location / facility
 */

function exp_sp_administration_resource_facility_callback($js = NULL, &$form_state){
  try {
  include_once(drupal_get_path('module', 'exp_sp_administration_resource') .'/exp_sp_administration_resource_location.inc');

  $min_form_state = array(
   'ajax' => TRUE, 
   're_render' => FALSE, 
   'no_redirect' => TRUE, 
   'location_id' => $form_state['triggering_element']['#ajax']['location_id'],
   'facility_id' => $form_state['triggering_element']['#ajax']['facility_id'],
   'build_info' => array('args' => array('ajax')) 
  );
  
  $form_state = $min_form_state;

  $form_state['input'] = array(); // Clear user input - raw
  $form_state['values'] = array(); // Clear user input - security cleared
      
  $formId = 'exp_sp_administration_resource_facility_addedit_form';
  $builtForm = drupal_build_form($formId, $form_state);

  $commands = array();
  $commands[] = ajax_command_replace(NULL, drupal_render($builtForm));
  $commands[] = exp_ctools_modal_adjust();
  
  return array('#type' => 'ajax', '#commands' => $commands);
  } catch (Exception $ex) {
  	watchdog_exception('exp_sp_administration_resource_facility_callback', $ex);
  	expertusErrorThrow($ex);
  }
} 

/*
 * getDefaultClassDescriptionAddEditFormFieldValue() - The function determines the default value for a class addedit form field
 */
function getDefaultFacilityDescriptionAddEditFormFieldValue($field, $form_state, $dbDetail) {
  try { 
  $defaultValue = empty($form_state['values'][$field])? (empty($dbDetail[$field])? '' : $dbDetail[$field]) :
                                                        $form_state['values'][$field]['value'];
  return $defaultValue;
  } catch (Exception $ex) {
  	watchdog_exception('getDefaultFacilityDescriptionAddEditFormFieldValue', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getNewFacilityDefaultValues() 
 */
function getNewFacilityDefaultValues($locationId) {
	try {
  $select = db_select("slt_location", "loc");
  $select->addField('loc', 'name', 'loc_name');
  $select->condition('loc.id', $locationId, '='); 
  $result = $select->execute()->fetchAssoc();
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('getNewFacilityDefaultValues', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * exp_sp_administration_resource_facility_addedit_form() - Build the addedit form for Facility.
 */

function exp_sp_administration_resource_facility_addedit_form($form, &$form_state, $renderMode){
  try {
  $showCancelSave = 1; 
  include_once('exp_sp_administration_resource.inc');
  
  $css_theme_optional = array('type' => 'file', 'group' => CSS_THEME);
  global $theme_key;
	if($theme_key == 'expertusoneV2') {
		//NEWUI THEME STYLE SHEET
		drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_v2.css', $css_theme_optional);
	} else {
		drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration.css', $css_theme_optional);  		
	}
  //drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration.css', $css_theme_optional);
  
  $facilityDetails = array();
  
  $locationId = $form_state['location_id'];
  $facilityId = $form_state['facility_id'];

  expDebug::dPrint('exp_sp_administration_resource_facility_addedit_form - $form_state[location_id]: '. print_r($form_state['location_id'], true) , 5);
  expDebug::dPrint('exp_sp_administration_resource_facility_addedit_form - $form_state[facility_id]: '. print_r($form_state['facility_id'], true) , 5);
  
  $facilityDetails['location_id'] = $locationId;
  if (!empty($facilityId)) {
    $facilityDetails['facility_id'] = $facilityId;
    // Fetch facility details to be displayed in the form
    $facilityDetails = getResourceFacilityDetails($facilityId);
  } else if (empty($form_state['values'])) {
    $facilityDetails   = getNewFacilityDefaultValues($locationId);
    $facilityDetails['location_name'] = $facilityDetails['loc_name'];
  }  
  if(!empty($form_state['values']['country'])){
    $facilityDetails['country'] = $form_state['values']['country'];
  }
  
  expDebug::dPrint('exp_sp_administration_resource_facility_addedit_form - $facilityDetails : '. print_r($facilityDetails, true) , 5);
  
  expDebug::dPrint('exp_sp_administration_resource_facility_addedit_form - country : '. print_r($form_state['values']['country'], true) , 5);
  
  // Create a new add/edit form
  $form = newAddEditFormWithCancelAndSave('resource-facility-basic-addedit-form-'.$facilityId);
  
  addHiddenValueToAddEditForm($form, 'id', $facilityId);
  addHiddenValueToAddEditForm($form, 'location_id', $locationId);
  
  if(empty($facilityId)){
      $form['new-facility-title-box'] = array(
      '#type' => 'markup',
      '#markup' => '<div id="add-class-title-box">New Facility</div>'
      ); 
  }
  
  $form['emptyspace'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="admin-class-title-arrow-icon"></div>'
  ); 
  
  $form['facility_detail_disp'] = array(
  	'#type'       => 'markup',
  	'#prefix'     => '<div style="padding-top:20px;" id="resource-facility-basic-addedit-form-disp-'.$facilityId.'">',    
  );
  
  //list_facility_attributes($form, $form_state, $facilityId, $facilityDetails);
  $form['facility_detail_wrapper'] = array(
    '#type'       => 'markup',
    '#prefix'     => '<div class="resource-facility-basic-addedit-form-container" id="resource-facility-basic-addedit-form-container">',    
  );
  
  if($form_state['storage']['substage'] == 'details' || empty($form_state['storage']['substage'])) {
    
    $form_state['storage']['substage'] = 'details';
      
    addHiddenValueToAddEditForm($form, 'location_id', $locationId);
    
    //$facilityId =  getDefaultResAddEditFormFieldValue('facility_id', $form_state, $facilityDetails);
    addHiddenValueToAddEditForm($form, 'facility_id', $facilityId);
    
    $defaultValue =  getDefaultResAddEditFormFieldValue('facility_name', $form_state, $facilityDetails);
    addTextFieldToAddEditForm($form, 'facility_name', t('LBL312'), $defaultValue, 2, true);
    
    $defaultValue =  getDefaultResAddEditFormFieldValue('addr1', $form_state, $facilityDetails);
    addTextFieldToAddEditForm($form, 'addr1', t('LBL064'), $defaultValue, 3, true);
    
    $defaultValue =  getDefaultResAddEditFormFieldValue('addr2', $form_state, $facilityDetails);
    addTextFieldToAddEditForm($form, 'addr2', t('LBL065'), $defaultValue, 4, false);
    
    $defaultAffectorValue =  getDefaultResAddEditFormFieldValue('country', $form_state, $facilityDetails);
    $defaultAffectedValue =  getDefaultResAddEditFormFieldValue('state', $form_state, $facilityDetails);
    
    if ($form_state['triggering_element']['#name'] == 'country') {
      $defaultAffectedValue = '';
    }
   
    $affector = array(
      'id' => 'country',
      'title' => t('LBL039'),
      'fetch_options' => getCountries,
      'default_value' => $defaultAffectorValue,
      'tab_index' => 5,
      'mandatory' => true,
    );

    $facility_wrapper_id="adddedit-state-wrapper-".$facilityId;
    $affected = array(
      'id' => 'state',
      'wrapper_id' => $facility_wrapper_id,
      'title' => t('LBL152'),
      'fetch_options' => getStatesForCountry,
      'default_value' => $defaultAffectedValue,
      'tab_index' => 6,
      'mandatory' => false,
    );
    
    addAjaxDependantDropDownPairToAddEditForm($form, $affector, $affected);
    
    $defaultValue =  getDefaultResAddEditFormFieldValue('city', $form_state, $facilityDetails);
    addTextFieldToAddEditForm($form, 'city', t('LBL066'), $defaultValue, 7);
  
    $defaultValue =  getDefaultResAddEditFormFieldValue('zipcode', $form_state, $facilityDetails);
    addTextFieldToAddEditForm($form, 'zipcode', t('LBL562'), $defaultValue, 8, false, false, 40);
                                      
    $defaultValue =  getDefaultResAddEditFormFieldValue('first_name', $form_state, $facilityDetails);
    addTextFieldToAddEditForm($form, 'first_name', t('LBL313'), $defaultValue, 9, false);
  
    $defaultValue =  getDefaultResAddEditFormFieldValue('last_name', $form_state, $facilityDetails);
    addTextFieldToAddEditForm($form, 'last_name', t('LBL314'), $defaultValue, 10, false);
    
    $defaultValue =  getDefaultResAddEditFormFieldValue('fax', $form_state, $facilityDetails);
    addTextFieldToAddEditForm($form, 'fax', t('LBL315'), $defaultValue, 11, false, false, 40);
    
    $defaultValue =  getDefaultResAddEditFormFieldValue('phone_no', $form_state, $facilityDetails);
    addTextFieldToAddEditForm($form, 'phone_no', t('LBL070'), $defaultValue, 12, false, false, 40);
  
    $defaultValue =  getDefaultResAddEditFormFieldValue('email', $form_state, $facilityDetails);
    addTextFieldToAddEditForm($form, 'email', t('LBL061'), $defaultValue, 13);
    
    $statusUniqueCode = 'Resource';
    $attributes['onchange'][] = 'getStatusReason(this,\''.$statusUniqueCode.'\');'; 
  	      
    $defaultValue =  getDefaultResAddEditFormFieldValue('status', $form_state, $facilityDetails);
    $defaultValue = empty($defaultValue) ? 'lrn_res_fac_atv'  : $defaultValue;
    addDropDownToAddEditForm($form, 'status', t('LBL102'), getProfileItemNamesKeyed('lrn_res_fac_'), $defaultValue, 14, true, false, $attributes);
    
    /* -------Inactive Reason text Area Start----------*/
    if($defaultValue == 'lrn_res_fac_itv') {
      $displayPage = "display:block";
    }else{
      $displayPage = "display:none";
    }        
    
    $form['admin_status_inactive_disp_start'] = array(
  		'#type' => 'markup',
  		'#markup' => '<div id="admin-status-inactive-part-'.$statusUniqueCode.'" style="'.$displayPage.'">'
    );
  
    $defaultValue =  getDefaultResAddEditFormFieldValue('status_inactive_reason', $form_state, $facilityDetails);
    addTextAreaFieldToAddEditForm($form, 'status_inactive_reason', t('Inactive Reason'), $defaultValue, 8, false);
  
    $form['admin_status_inactive_disp_end'] = array(
  		'#type' => 'markup',
  		'#markup' => '</div>'
    );
  	/* -------Inactive Reason text Area End----------*/
    
  }
  
  $entityType = 'facility';
  $entityId = $facilityId;
  
  $uniqueId = $entityId.'-'.$entityType;

  
  if(!empty($entityId)){
    $form['list_type_'.$uniqueId] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="admin-course-add-attachment admin-course-add-attachment-select"><span onclick="displayResourceActionList()" class="resource-add-action-wrapper">Add</span> <ul class="resource-add-list">',          
    );
    $form['list_type_classroom_'.$uniqueId] = array(
        '#type' => 'markup',
        '#prefix' => '<li id="classroom_fieldset"><a href="javascript:void(0);" onclick="showHide(this, \''.$uniqueId.'\')">'.t('Classroom').'</a></li>',          
    );
    $form['list_type_equipment_'.$uniqueId] = array(
        '#type' => 'markup',
        '#prefix' => '<li id="equipment_fieldset"><a href="javascript:void(0);" onclick="showHide(this, \''.$uniqueId.'\')">'.t('Equipment').'</a></li>',          
    );
    $form['list_type_custom_'.$uniqueId] = array(
        '#type' => 'markup',
        '#prefix' => '<li id="custom_fieldset"><a href="javascript:void(0);" onclick="showHide(this, \''.$uniqueId.'\')">'.t('LBL276').'</a></li>',          
    );    
    $form['list_type_end_'.$uniqueId] = array(
        '#type' => 'markup',
        '#suffix' => '</ul></div>',          
    );
  }
    
  expDebug::dPrint('exp_sp_administration_resource_facility_addedit_form - $uniqueId : '.$uniqueId , 4);
  
  $resourceType = 'Facility';
  
  if(!empty($entityId)){
    //$form_state['display_class'] = TRUE;
  }
    
  //To Display Classroom  
  displayClassroomDetails($form, $form_state, $entityId, $entityType, $uniqueId, $resourceType);
  
  //To Display Equipment
  displayEquipmentDetails($form, $form_state, $entityId, $entityType, $uniqueId, $resourceType);

  //To display custom fields
  displayCustomFieldsDetails($form, $form_state, $entityId, $entityType, $uniqueId, TRUE);
  $form_state['cutom_fields_wrapper_id']= 'custom-fieldset-wrapper-'.$uniqueId;//this is needs for custom attribute validation
      
  /*
  if(!empty($entityId)){

    if($form_state['storage']['substage'] == 'classroom') {
      
      //To Display Classroom  
      displayClassroomDetails($form, $form_state, $entityId, $entityType, $uniqueId, 'Classroom');
            
    } else if($form_state['storage']['substage'] == 'equipment') {
      
      //To Display Equipment
      displayEquipmentDetails($form, $form_state, $entityId, $entityType, $uniqueId, 'Equipment');
      
    } else if($form_state['storage']['substage'] == 'custom') {
      
      //To display custom fields
      displayCustomFieldsDetails($form, $form_state, $entityId, $entityType, $uniqueId, TRUE);
      $form_state['cutom_fields_wrapper_id']= 'custom-fieldset-wrapper-'.$uniqueId;//this is needs for custom attribute validation
            
    }
  }  
  */
    
  
  addEditFormWithFacilityCancelAndSaveClose($form, 'resource-facility-basic-addedit-form-'.$facilityId, 'newtheme-save-button-'.$facilityId, 'newtheme-cancel-link-'.$facilityId);
  
  $form['facility_detail_disp_end'] = array(
    '#type'       => 'markup',
    '#suffix'     => '<div class="clearBoth"></div></div>',      
  );
  
  $form['facility_detail_wrapper_end'] = array(
    '#type'       => 'markup',      
    '#suffix'     => '</div>',     
  );
  
  expDebug::dPrint('exp_sp_administration_resource_facility_addedit_form - Display ends ');
  
  return $form;
  } catch (Exception $ex) {
  	watchdog_exception('exp_sp_administration_resource_facility_addedit_form', $ex);
  	expertusErrorThrow($ex);
  }
}


/**
 * exp_sp_administration_resource_facility_addedit_form() - Facility form validation.
 */
function exp_sp_administration_resource_facility_addedit_form_validate($form, &$form_state){
	try {
  $form_state['hidden-save-submit'] = false;
  $form_state['esign_show_popup']= false;
  
  if (!empty($form_state['storage']['new_entity'])) {
    unset($form_state['storage']['new_entity']); // Unset the variable to remove it from $form_state
    unset($form_state['storage']['form_info_list']); // No more required to be present in $form_state
  }
  
  if (!empty($form_state['storage']['edited_entity'])) {
  	unset($form_state['storage']['edited_entity']); // Unset the variable to remove it from $form_state
  }
  
  if ($form_state['triggering_element']['#name'] == 'country') {
  	// Since country has changed, we need to unset any selection for state made by the user
  	unset($form_state['values']['state']);
  	unset($form_state['input']['state']);
  	$form_state['storage']['affected_dropdown'] = 'state';
  	return;
  }  
  
  if ($form_state['values']['op'] == t('LBL109')) {
  	$form_state['input'] = array(); // Clear user input - raw
  	$form_state['values'] = array(); // Clear user input - security cleared
    $form_state['rebuild'] = TRUE; // Rebuild the form. Submit handler will not be called.
    return;
  }    
  if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
    $triggerValue  = 'save';
  }else{
    $triggerValue  = 'hidden-save';
  }

  if ($form_state['triggering_element']['#name'] == $triggerValue){
  	$form_state['esign_show_popup']= true;
  
    if($form_state['storage']['substage'] == 'details' || empty($form_state['storage']['substage'])) {
      // Check all mandatory fields exist
      addEditCheckMandatoryFields($form, $form_state, array('facility_name', 'addr1','country', 'status'));
      
      // Validate email address
      if (!empty($form_state['values']['email']) && !validateEmail($form_state['values']['email'])) {
        form_set_error('email', t('ERR071') . ' ' . $form['email']['#title'] . '.');
        $form_state['esign_show_popup']= false;
      }
      
    }
   
  }
  if ($form_state['triggering_element']['#name'] == 'hidden-save') {
       $form_state['hidden-save-submit']=true;
       $form_state['esign_show_popup']= false;   
  }
  /*
  if ($form_state['triggering_element']['#name'] == 'details') {
    $form_state['display_class']   = TRUE;
    $form_state['input']     = array(); // Clear user input - raw
    $form_state['values']    = array(); // Clear user input - security cleared    
    $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
    $form_state['storage']['substage'] = 'details';
    return;
  }  
  
  if ($form_state['triggering_element']['#name'] == 'classroom') {
    $form_state['display_class']   = TRUE;
    $form_state['input']     = array(); // Clear user input - raw
    $form_state['values']    = array(); // Clear user input - security cleared    
    $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
    $form_state['storage']['substage'] = 'classroom';
    unset($form_state['delete_attachment_index']);
    unset($form_state['delete_attachment_fieldset']);
    $form_state['num_attachment'] = 0;
    return;
  }  
  
  if ($form_state['triggering_element']['#name'] == 'equipment') {
    $form_state['display_class']   = TRUE;
    $form_state['input']     = array(); // Clear user input - raw
    $form_state['values']    = array(); // Clear user input - security cleared    
    $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
    $form_state['storage']['substage'] = 'equipment';
    unset($form_state['delete_tag_index']);
    unset($form_state['delete_tag_fieldset']);
    $form_state['num_tag'] = 0;
    return;
  } 

  if ($form_state['triggering_element']['#name'] == 'custom') {
    $form_state['display_class']   = TRUE;
    $form_state['input']     = array(); // Clear user input - raw
    $form_state['values']    = array(); // Clear user input - security cleared    
    $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
    $form_state['storage']['substage'] = 'custom';
    unset($form_state['delete_custom_index']);
    unset($form_state['delete_custom_fieldset']);
    $form_state['num_custom'] = 0;
    return;
  } 
  */
  } catch (Exception $ex) {
  	watchdog_exception('exp_sp_administration_resource_facility_addedit_form_validate', $ex);
  	expertusErrorThrow($ex);
  }
}

function list_facility_attributes(&$form, &$form_state, $facilityId, $facilityDetails){
  try {
  $form['facility_actions_disp'] = array(
  	  '#type'       => 'markup',
      '#prefix'     => '<div class="catalog-course-basic-addedit-action-disp" id="resource-facility-basic-addedit-form-action-disp-'.$facilityId.'">',
  );
  $isSelected = ($form_state['storage']['substage'] == 'details' || empty($form_state['storage']['substage'])) ? 'selected' : ''; 
  $form['facility_btn_details'] = array(
  	  '#type'       => 'submit',      
	  '#value'      => t('LBL272'),
      '#name'       => 'details',
      '#attributes' => ($form_state['storage']['substage'] == 'details' || empty($form_state['storage']['substage']))? array('Class' => 'selected'): '',
      '#prefix'     => '<ul><li>',
      '#suffix'     => '</li>',
      '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'resource-facility-basic-addedit-form-disp-'.$facilityId),
      '#ajax' => array(  
  		'callback' => 'addEditRepaintForm',
        'wrapper'  => 'resource-facility-basic-addedit-form-'.$facilityId,
        'event'	   => 'click'
      ), 
    );
    /*
    if($facilityId){
      
      $isSelected = ($form_state['storage']['substage'] == 'classroom') ? 'highlighted' : '';	
      $form['facility_btn_cls'] = array(
        '#type'       => 'submit',      
  	    '#value'      => t('LBL316'),
        '#name'       => 'classroom',
        '#prefix'     => '<li>',
        '#suffix'     => '</li>',
        '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'resource-facility-basic-addedit-form-disp-'.$facilityId),
        '#ajax' => array(  
    	  'callback' => 'addEditRepaintForm',
          'wrapper'  => 'resource-facility-basic-addedit-form-'.$facilityId,
          'event'	   => 'click'
        ), 
      );
      
      $isSelected = ($form_state['storage']['substage'] == 'equipment') ? 'highlighted' : '';	
      $form['facility_btn_equ'] = array(
        '#type'       => 'submit',      
  	    '#value'      => t('LBL317'),
        '#name'       => 'equipment',
        '#prefix'     => '<li>',
        '#suffix'     => '</li>',
        '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'resource-facility-basic-addedit-form-disp-'.$facilityId),
        '#ajax' => array(  
    	  'callback' => 'addEditRepaintForm',
          'wrapper'  => 'resource-facility-basic-addedit-form-'.$facilityId,
          'event'	   => 'click'
        ), 
      );
    
      $isSelected = ($form_state['storage']['substage'] == 'custom') ? 'highlighted' : '';	
      $form['facility_btn_cus'] = array(
        '#type'       => 'submit',      
  	    '#value'      => t('LBL319'),
        '#name'       => 'custom',
        '#prefix'     => '<li>',
        '#suffix'     => '</li>',
        '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'resource-facility-basic-addedit-form-disp-'.$facilityId),
        '#ajax' => array(  
    	  'callback' => 'addEditRepaintForm',
          'wrapper'  => 'resource-facility-basic-addedit-form-'.$facilityId,
          'event'	   => 'click'
        ), 
      );
    
    }
    else{
      $form['facility_actions_ul_container_disp_end'] = array(
      	'#type'       => 'markup',
      	'#suffix'     => '</ul>',  
      );
    }
    
    $form['facility_actions_disp_end'] = array(
  	  '#type'       => 'markup',
      '#suffix'     => '</div>',  
    );    
    */
  } catch (Exception $ex) {
  	watchdog_exception('list_facility_attributes', $ex);
  	expertusErrorThrow($ex);
  }
}


/**
 * exp_sp_administration_resource_facility_addedit_form_submit() - Facility form submit.
 */
function exp_sp_administration_resource_facility_addedit_form_submit($form, &$form_state){
  try {
  expDebug::dPrint('exp_sp_administration_resource_facility_addedit_form_submit - Starts');
    if($form_state['hidden-save-submit']==true){
  
  //if ($form_state['triggering_element']['#value'] == 'Save'){
     
    include_once('exp_sp_administration_resource.inc');
    
    $loggedInUserId = getIdOfLoggedInUser();
    
    // Check whether add or edit 
    if (empty($form_state['values']['id']) && $form_state['values']['id']=='') { // Add
      
      $facilityId = _addNewFacility($form, $form_state, $loggedInUserId);
      
      drupal_set_message('Facility details saved successfully.');
      
      $form_state['build_info']['args'][] = $facilityId;
      $form_state['storage']['new_entity'] = TRUE;
      
    } else { //Edit
      
      $facilityId = $form_state['values']['id'];
      
      unset($form_state['storage']['new_entity']); // Unset the variable to remove it from $form_state
      unset($form_state['storage']['form_info_list']); // No more required to be present in $form_state
      
      if($form_state['storage']['substage'] == 'details' || empty($form_state['storage']['substage'])) {
        
      	_updateFacilityDetails($form, $form_state, $loggedInUserId);
        drupal_set_message('Facility details updated successfully.');
        $form_state['storage']['substage'] = 'details';
        
      }
      $form_state['storage']['edited_entity'] = TRUE;
    }
    
    $form_state['display_class'] = TRUE;
    
    if(empty($form_state['facility_id'])){
      $entityId = $form_state['values']['id'];
    }else{
      $entityId = $form_state['facility_id'];
    }

    $form_state['storage']['facility_list']['location_id'] = $locationId;
    $form_state['storage']['facility_list']['facility_id']  = $facilityId;
    
    $entityType = 'facility';
    
    $uniqueId = $entityId.'-'.$entityType;

    expDebug::dPrint('exp_sp_administration_resource_facility_addedit_form_submit - $uniqueId : '.$uniqueId , 4);
    expDebug::dPrint('exp_sp_administration_resource_facility_addedit_form_submit - attr_action : '.$form_state['values']['attr_action'] , 5);
    
    classroomOperations($form, $form_state, $entityId, $entityType, $uniqueId);
    
    equipmentOperations($form, $form_state, $entityId, $entityType, $uniqueId);
    
    if(!module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status,Below thing should be apply if not enable the custom attribute module
    customfieldsOperationsForDropdown($form, $form_state,$entityId,$entityType);
    }
        
    /*
    if(isset($form_state['values']['attr_action']) && isset($entityId)) {
      
      if($form_state['values']['attr_action'] == 'classroom') {   
        
        classroomOperations($form, $form_state, $entityId, $entityType, $uniqueId);
        
      } else if($form_state['values']['attr_action'] == 'equipment') {
        
        equipmentOperations($form, $form_state, $entityId, $entityType, $uniqueId);
        
      } else if($form_state['values']['attr_action'] == 'custom') {
        
        customfieldsOperations($form, $form_state, $entityId, $entityType);
      
      }
      
    }
    */
    
    $form_state['input']    = array();
    $form_state['values']   = array();
    $form_state['rebuild']  = TRUE;
  }
  } catch (Exception $ex) {
  	watchdog_exception('exp_sp_administration_resource_facility_addedit_form_submit', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * addNewFacility() - New Facility Entry
 */
function _addNewFacility($form, &$form_state, $createUserId, $callFromAPI =0){

  $txn = db_transaction();

  try {
    
    expDebug::dPrint('_addNewFacility - Starts');
    
    $locationId = $form_state['values']['location_id'];
    
    $inactivereasontext    = empty($form_state['values']['status_inactive_reason']) ? NULL : $form_state['values']['status_inactive_reason'];
    $in_active_on          = $form_state['values']['status'] == 'lrn_res_fac_itv' ? now() : NULL;
    $in_active_reason      = $form_state['values']['status'] == 'lrn_res_fac_itv' ? $inactivereasontext : NULL;
    
   /* Insert Statement for Facility  */           
    $insertFacStmt = db_insert('slt_facility');
    $custom  = NULL;    	
    $fields = array(
              'location_id'        => $locationId,                  
              'name'               => $form_state['values']['facility_name'],
              'contact_addr1'      => $form_state['values']['addr1'],
              'contact_addr2'      => $form_state['values']['addr2'],
              'contact_country'    => $form_state['values']['country'],
              'contact_state'      => $form_state['values']['state'],
              'contact_city'       => $form_state['values']['city'],
              'contact_zipcode'    => $form_state['values']['zipcode'],
              'contact_fname'      => $form_state['values']['first_name'],
              'contact_lname'      => $form_state['values']['last_name'],
              'contact_fax'        => $form_state['values']['fax'],
              'contact_phone'      => $form_state['values']['phone_no'],
              'contact_email'      => $form_state['values']['email'],
    		  'status'             => $form_state['values']['status'],
    		  'is_active'          => 1,
    		  'in_active_on'       => $in_active_on,
              'in_active_reason'   => $in_active_reason,
              'created_by'         => $createUserId,
              'created_on'         => now(),
              'updated_by'         => $createUserId,
              'updated_on'         => now(),
    		  'custom0'            => $custom,
              'custom1'            => $custom,
              'custom2'            => $custom,
              'custom3'            => $custom,
              'custom4'            => $custom   
            );
    $insertFacStmt->fields($fields);			            
    $facilityId = $insertFacStmt->execute();
    
    make_audit_trail_entries($form_state['values'], getIdOfLoggedInUser() , null, $facilityId ,'Inserted','cre_sys_obt_fac','exp_sp_administration_resources','_addNewFacility',
                                          array('location_id', 'facility_name', 'addr1', 'addr2', 'country', 'state', 'city', 'zipcode', 'first_name','last_name','fax','phone_no','email','status'));
    
    expDebug::dPrint('_addNewFacility - Ends - '.$facilityId , 3);
    
    $form_state['values']['facility_id'] = $facilityId;
    $form_state['values']['id'] = $facilityId;
    if($callFromAPI){
      return array((object)array('id'=>$facilityId));
    }
    
  } catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('_addNewFacility', $ex);
    if($callFromAPI){
      //return array((object)array('id'=>'Failure'));
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Facility Id is not valid";
    	return $errobj;
    }
    throw $ex;
  }
  
  // Commit the transaction
  unset($txn);
  
  return $facilityId;
}

/**
 * updateFacilityDetails() 
 */
function _updateFacilityDetails($form, &$form_state, $createUserId, $callFromAPI = 0){

  $txn = db_transaction();

  try {
    
    $facilityId = $form_state['values']['facility_id'];
    $locationId = $form_state['values']['location_id'];
    
    $inactivereasontext    = empty($form_state['values']['status_inactive_reason']) ? NULL : $form_state['values']['status_inactive_reason'];
    $in_active_on          = $form_state['values']['status'] == 'lrn_res_fac_itv' ? now() : NULL;
    $in_active_reason      = $form_state['values']['status'] == 'lrn_res_fac_itv' ? $inactivereasontext : NULL;
          
    /* Update Statement for Facility */    
    $updateFacStmt = db_update('slt_facility');
    $default= db_query("SELECT id,location_id,name as facility_name,contact_addr1 as addr1,contact_addr2 as addr2,contact_country as country,contact_state as state,contact_city as city,contact_zipcode as zipcode,contact_fname as first_name,contact_lname as last_name,contact_fax as fax,contact_phone as phone_no,contact_email as email,status from slt_facility where id=:uid",array(":uid"=>$form_state['values']['facility_id']))->fetchAll();
    $custom  = NULL;  
    $fields = array(
                'location_id'        => $locationId,                  
                'name'               => $form_state['values']['facility_name'],
                'contact_addr1'      => $form_state['values']['addr1'],
                'contact_addr2'      => $form_state['values']['addr2'],
                'contact_country'    => $form_state['values']['country'],
                'contact_state'      => $form_state['values']['state'],
                'contact_city'       => $form_state['values']['city'],
                'contact_zipcode'    => $form_state['values']['zipcode'],
                'contact_fname'      => $form_state['values']['first_name'],
                'contact_lname'      => $form_state['values']['last_name'],
                'contact_fax'        => $form_state['values']['fax'],
                'contact_phone'      => $form_state['values']['phone_no'],
                'contact_email'      => $form_state['values']['email'],
      			'status'             => $form_state['values']['status'],
      			'in_active_on'       => $in_active_on,
                'in_active_reason'   => $in_active_reason,    
                'updated_by'         => 1,
                'updated_on'         => now(),
                'custom0'            => $custom,
                'custom1'            => $custom,
                'custom2'            => $custom,
                'custom3'            => $custom,
                'custom4'            => $custom   
              );
    
    $updateFacStmt->fields($fields);    
    $updateFacStmt->condition('id', $facilityId);
    $numFacUpdated = $updateFacStmt->execute();
    make_audit_trail_entries($form_state['values'], getIdOfLoggedInUser() , $default, $form_state['values']['facility_id'] ,'Updated','cre_sys_obt_fac','exp_sp_administration_resources','_updateFacilityDetails',
                                          array('location_id', 'facility_name', 'addr1', 'addr2', 'country', 'state', 'city', 'zipcode', 'first_name','last_name','fax','phone_no','email','status'));
    
    $form_state['values']['facility_id'] = $facilityId;

    if($callFromAPI){ 
      return array((object)array('id'=>$facilityId));
    }     
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('_updateFacilityDetails', $ex);
    if($callFromAPI){ 
        //return array((object)array('id'=>'Failure'));
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Facility Id is not valid";
    	return $errobj;
    } 
    throw $ex;    
  }
  
  unset($txn);
  
  return $facilityId;
} 


/**
 * getResourceFacilityDetails() - Facility details
 */
function getResourceFacilityDetails($facilityId){
 try {
  $select  = db_select('slt_facility','facility');
  $select -> innerjoin('slt_location', 'location', 'facility.location_id = location.id');
  $select -> addField('facility','id','facility_id');
  $select -> addField('facility','name','facility_name');
  $select -> addField('facility','contact_addr1','addr1');
  $select -> addField('facility','contact_addr2','addr2');
  $select -> addField('facility','contact_country','country');
  $select -> addField('facility','contact_state','state');
  $select -> addField('facility','contact_city','city');
  $select -> addField('facility','contact_zipcode','zipcode');
  $select -> addField('facility','contact_fname','first_name');
  $select -> addField('facility','contact_lname','last_name');
  $select -> addField('facility','contact_fax','fax');
  $select -> addField('facility','contact_phone','phone_no');
  $select -> addField('facility','contact_email','email');
  $select -> addField('facility','status','status');
  $select -> addField('facility','in_active_reason','status_inactive_reason');
  $select -> addField('facility','location_id','location_id');
  $select -> addField('location','name','location_name');
  
  $select -> condition('facility.id', $facilityId, '=');  
  
  $resourceList = $select->execute()->fetchAssoc();
    
  return $resourceList;
  } catch (Exception $ex) {
  	watchdog_exception('getResourceFacilityDetails', $ex);
  	expertusErrorThrow($ex);
  }
}


function getAdministrationFacilityDetails($locationId) {
	try {
  $select = db_select('slt_facility', 'fac');
  $select->leftjoin('slt_location', 'loc', 'loc.id = fac.location_id');
  $select->leftjoin('slt_profile_list_items', 'items', 'items.code = fac.status');
  $select->addField('fac','id','fac_id');
  $select->addField('fac','name','fac_name');
  $select->addField('loc','name','loc_name');
  $select->addField('items','name','status');
  $select->orderby('fac.id','asc');  
  $select->condition('fac.location_id', $locationId, '=');	
  $facilityList = $select->execute()->fetchAll();
  return $facilityList;
  } catch (Exception $ex) {
  	watchdog_exception('getAdministrationFacilityDetails', $ex);
  	expertusErrorThrow($ex);
  }
}
 

function addEditFormWithFacilityCancelAndSaveClose(&$form, $wrapperId = 'addedit-form-wrapper', $saveButtonId = 'save-button', $cancelButtonId = 'cancel-button') {
	try {
  // Remove the temporary storage for API's to exchange information during form creation
  unset($form['addedit_tmp_storage']);
  $esignHiddenButt = 'esign-'.$wrapperId;
  // Add the Cancel button
  $ajaxCallbackCancel = 'addEditResourceCancelRepaintForm';
  $wrapperId = 'add-facility-list-wrapper';
  
  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $cancelAttributes['class'] = array('addedit-edit-' . $cancelButtonId); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
  
  // For common styling of all addedit cancel buttons
  //$cancelAttributes['class'][] = 'addedit-edit-cancel';
  
  // Using this class to applying bg color to the save button
  $cancelAttributes['class'][] = 'admin-action-button-middle-bg';
  
  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  $cancelAttributes['data-wrapperid'] = array($wrapperId);
  
  // A common class for all addedit form cancel button, for styling to appear as a link.
  $cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';

  // Remove the error messages or success messages on clicking cancel
  $cancelAttributes['onclick'][] = 'remove_messages("facility");';
  
  $form[$cancelButtonId] = array(
    '#type' => 'submit',
    '#prefix' => '<div class="addedit-form-cancel-and-save-actions-row">' .
                   '<div class="addedit-form-cancel-container-actions"><div class="admin-action-button-left-bg"></div>',
    '#suffix' =>   '<div class="admin-action-button-right-bg"></div>',
    '#value' => t('LBL109'),
    '#attributes' => $cancelAttributes,
    '#ajax' => array(
      'wrapper' => $wrapperId,
      'callback' => $ajaxCallbackCancel,
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
  
  // Add the Save button
  
  
  $ajaxCallbackSave = 'addEditResourceRepaintForm';
  
  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $saveAttributes['class'] = array('addedit-edit-' . $saveButtonId); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // For common styling of all addedit save buttons
  //$saveAttributes['class'][] = 'addedit-edit-save';
  
  // Using this class to applying bg color to the save button
  $saveAttributes['class'][] = 'admin-save-button-middle-bg';

  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  $saveAttributes['data-wrapperid'] = array($wrapperId);
  
  // Using this onclick function to hide message div

  
  //Increase the width of the popup while clicking on save button
  //$saveAttributes['onclick'][] = 'increasePopWidth();';
  
  // Remove the error messages or success messages on clicking submit
  
  
  if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){ 
  	  /*$saveAttributes['onclick'][] = '$("body").data("learningcore").getEsignPopup({"popupDiv":"ctools-face-table","esignFor":"AddAdmin","esignButtId":"'.$esignHiddenButt.'"});return false;'; 
  	  $form[$saveButtonId] = array(
	    '#type' => 'submit',
	    '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
	    '#suffix' => 
	               '<div class="admin-save-button-right-bg"></div></div>' .
	               '<div class="clearBoth"></div></div>' .
	             '<div class="clearBoth"></div><div class="admin-class-course-separater"></div></div>',
	    '#value' => t('LBL141'),
	    '#name' => 'save',
	    '#attributes' => $saveAttributes,
	  );   	*/
  	
  	$saveAttributes['onclick'] = array();
     // $saveAttributes['onclick'][] = 'hideMessageInfo();';
      //$saveAttributes['onclick'][] = 'increasePopWidth();';
     // $saveAttributes['onclick'][] = 'clearMessages();';
      $saveAttributes['onclick'][] = 'Drupal.ajax.prototype.commands.addEditHideMessages();';	 
      $form[$saveButtonId] = array(
      
    	      '#type' => 'submit',
    	      '#value' => t('LBL141'),
              '#name' => 'save',
    	   	  '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
    	       '#suffix' => 
    	               '<div class="admin-save-button-right-bg"></div></div>' .
    	               '<div class="clearBoth"></div></div>' .
    	             '<div class="clearBoth"></div></div>',
      	      '#id' =>$saveButtonId,
      		  '#attributes' => $saveAttributes,
    	      '#ajax' => array(
    		      'wrapper' => $wrapperId,
    		      'callback' => $ajaxCallbackSave,
    		      'method' => 'replace',
    		      'effect' => 'none', // 'fade',
    		      'event'=>'click',
                  'customButtonId'=>$esignHiddenButt,
    		      'keypress' => true,
    		      'progress' => array(
    		        'type' => 'throbber',
    		        'message' => ''
    		      ),
    		      
    	    	)
    	    	
    	      );
  }else{
/*  	  $saveAttributes['onclick'][] = 'hideMessageInfo();';
  	  $saveAttributes['onclick'][] = 'remove_messages("class");';*/
	 /* $form[$saveButtonId] = array(
	    '#type' => 'submit',
	    '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
	    '#suffix' => 
	               '<div class="admin-save-button-right-bg"></div></div>' .
	               '<div class="clearBoth"></div></div>' .
	             '<div class="clearBoth"></div><div class="admin-class-course-separater"></div></div>',
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
	  );  */
  	
  	 $saveAttributes['onclick'] = array();
     // $saveAttributes['onclick'][] = 'hideMessageInfo();';
      //$saveAttributes['onclick'][] = 'increasePopWidth();';
     // $saveAttributes['onclick'][] = 'clearMessages();';
      $saveAttributes['onclick'][] = '$("#'.$esignHiddenButt.'").click();';	 
      $form[$saveButtonId] = array(
	    '#type' => 'button',
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
  	      'callback' => '',
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
  }
  /*$saveAttributes['onclick'] = array();
/*  $saveAttributes['onclick'][] = 'hideMessageInfo();';
  $saveAttributes['onclick'][] = 'remove_messages("facility");';
  
  $form[$esignHiddenButt] = array(
	    '#type' => 'submit',
  		'#prefix' => '<span id = "esign-admin-div" style="display:none;">',
	    '#suffix' => '</span>',
	    '#value' => t('LBL141'),
	    '#name' => 'save',
  		'#id' =>$esignHiddenButt,
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
	  );  */
  
  $saveAttributes['onclick'] = array();
 // $saveAttributes['onclick'][] = 'hideMessageInfo();';	 
 // $saveAttributes['onclick'][] = 'increasePopWidth();';
  $form[$esignHiddenButt] = array(
	      '#type' => 'submit',
	      '#value' => t('hidden-save'),
	      '#name'	=> 'hidden-save',
      	  '#prefix' => '<span id = "esign-admin-div" style="display:none;">',
	      '#suffix' => '</span>',
	      '#id' =>$esignHiddenButt,
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
  $form['addedit-left-col-end'] = array(
    '#type' => 'markup',
    '#markup' => '</div><div class="clearBoth"></div>',
  );
  
  $form['load-scripts'.$uniqueId] = array(
	'#prefix' => '<script language="Javascript" type="text/javascript">selecteDropdown()</script>',
  );
  
  } catch (Exception $ex) {
  	watchdog_exception('addEditFormWithFacilityCancelAndSaveClose', $ex);
  	expertusErrorThrow($ex);
  }
}


function addEditResourceRepaintForm($form, &$form_state) {
	try {
  $custButtonId  = $form_state['triggering_element']['#ajax']['customButtonId'];
  $esignPopup = $form_state['esign_show_popup'];
  $triggerElement = $form_state['triggering_element'];
  $newEntity      = !empty($form_state['storage']['new_entity']); // will not be present when trigger element is cancel
  $editedEntity   = !empty($form_state['storage']['edited_entity']);
  $narrowSearchRefresh = 0;
  if ($form_state['storage']['new_entity'] == TRUE || $form_state['storage']['edited_entity'] == TRUE) {
    $narrowSearchRefresh = 1;
  }
    
  //$triggerElement = $form_state['triggering_element']['#value'];
  
  $form_validate  = $form_state["storage"]["form_validate"];
  if($triggerElement['#name'] == "hidden-save"){  
	  $locationId             = $form_state['location_id'];
	  $facilityId              = $form_state['storage']['facility_list']['facility_id'];

	  if($newEntity) {      
		  $min_form_state       = array(
		   		'ajax'         => TRUE, // As I have merged code from ctools_modal_form_render() in this function, this may no longer be needed. Retaining as in ctools_modal_form_wrapper(), and because form_state is passed around by reference.
		   		're_render'    => TRUE, // Doesn't seem of being used by ctools or drupal forms API. Retaining as in ctools_modal_form_wrapper()
		   		'no_redirect'  => TRUE, // Prevent any redirection, required forms are built in ajax callback
		   		'build_info'   => array('args' => array('ajax')) // The the form builder function expects the first argument to be ajax or nojs.
		        );
		  $form_state                                = $min_form_state;   
		  $form_state['locationId']                    = $locationId;
		  $form_state['storage']['new']['location_id'] = $locationId;      
		  $form_state['storage']['new']['facility_id']  = $facilityId;
		  
		  //$classListrenderedForm                     = exp_ctools_modal_multi_ajaxonly_forms_renderform('exp_sp_administration_resource_facility_title_form', $form_state);
		  $builtForm            = drupal_build_form('exp_sp_administration_resource_facility_title_form', $form_state);
		  
		  $commands             = array();
		  $commands[]           = ajax_command_replace(NULL, drupal_render($builtForm));
	  }
  }
  
  $statusMsgHTML = theme('status_messages');
  if (!empty($statusMsgHTML)) {
  	  $commands[]           = ajax_command_prepend(NULL, $statusMsgHTML);
	  if (!empty($form_state['triggering_element']['#ajax']['wrapper'])) {
	  	$commands[] = addedit_hide_messages($form_state['triggering_element']['#ajax']['wrapper']);
	  }  
	  else {
	    $commands[] = addedit_hide_messages();
	  }
  }  
  $commands[]           = exp_ctools_modal_adjust();
  
  if ($newEntity || $editedEntity) {
    $commands[]   = ajax_command_refresh_narrow_search_results('onclose');
  }

  if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){      
    $esignObj  = '{"popupDiv":"ctools-face-table","esignFor":"AddAdmin","esignButtId":"'.$custButtonId.'"}';
    if($esignPopup == true){
      $commands[] = ajax_command_invoke(NULL, 'getNewEsignPopup',array($esignObj));
      $form_state['esign_show_popup']=false; // this is ignored, as form state is not saved after the ajax callback is executed
    }
  }
 // $form_state['input']  = array();
  //$form_state['values'] = array();
  
  
  return array('#type' => 'ajax', '#commands' => $commands);
  } catch (Exception $ex) {
  	watchdog_exception('addEditResourceRepaintForm', $ex);
  	expertusErrorThrow($ex);
  }
}




function addEditResourceCancelRepaintForm($form, &$form_state) {
	try {
  $locationId             = $form_state['location_id'];
  $facilityId              = $form_state['storage']['facility_list']['facility_id'];
  $triggerElement = $form_state['triggering_element'];
  
    
  //$triggerElement = $form_state['triggering_element']['#value'];
  
  $form_validate  = $form_state["storage"]["form_validate"];
  

          
/*  $min_form_state       = array(
   		'ajax'         => TRUE, // As I have merged code from ctools_modal_form_render() in this function, this may no longer be needed. Retaining as in ctools_modal_form_wrapper(), and because form_state is passed around by reference.
   		're_render'    => TRUE, // Doesn't seem of being used by ctools or drupal forms API. Retaining as in ctools_modal_form_wrapper()
   		'no_redirect'  => TRUE, // Prevent any redirection, required forms are built in ajax callback
   		'build_info'   => array('args' => array('ajax')) // The the form builder function expects the first argument to be ajax or nojs.
        );
  $form_state                                = $min_form_state;   
  $form_state['locationId']                    = $locationId;
  $form_state['storage']['new']['location_id'] = $locationId;      
  $form_state['storage']['new']['facility_id']  = $facilityId;*/
  
  //$classListrenderedForm                     = exp_ctools_modal_multi_ajaxonly_forms_renderform('exp_sp_administration_resource_facility_title_form', $form_state);
  //$builtForm            = drupal_build_form('exp_sp_administration_resource_facility_title_form', $form_state);
  
  $commands             = array();
    
  // Validate class form input
  /*if($form_validate || ($triggerElement['#value'] == 'Cancel')) {
    $commands[]           = ajax_command_replace(NULL, drupal_render($builtForm));
  }*/
  $statusMsgHTML = theme('status_messages');
  if (!empty($statusMsgHTML)) {
  	  $commands[]           = ajax_command_prepend(NULL, $statusMsgHTML);
	  if (!empty($form_state['triggering_element']['#ajax']['wrapper'])) {
	  	$commands[] = addedit_hide_messages($form_state['triggering_element']['#ajax']['wrapper']);
	  }  
	  else {
	    $commands[] = addedit_hide_messages();
	  }
  }  
  $commands[]           = exp_ctools_modal_adjust();
  
  if (!empty($triggerElement['#name']) && $triggerElement['#name'] == 'save') {
	  if (!empty($form_state['triggering_element']['#ajax']['wrapper'])) {
	  	$commands[] = addedit_hide_messages($form_state['triggering_element']['#ajax']['wrapper']);
	  }  
	  else {
	    $commands[] = addedit_hide_messages();
	  }
  }  
  $commands[] = ajax_command_invoke(NULL, 'cancelFacilityDetails',array($facilityId));

  
  
  return array('#type' => 'ajax', '#commands' => $commands);
  } catch (Exception $ex) {
  	watchdog_exception('addEditResourceCancelRepaintForm', $ex);
  	expertusErrorThrow($ex);
  }
}

function addNewFacilityByRestAPI( &$form_state,$createuserid="") 
{
	try {
	    drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
			$form=array();
			$form["form_id"]="";
			$facilityId =  _addNewFacility($form,$form_state,$createuserid,1);
			expDebug::dPrint(' addNewFacilityByRestAPI created facility:' . print_r($facilityId,true) , 4);
	    return $facilityId;  
    } catch (Exception $ex) {
    	watchdog_exception('addNewFacilityByRestAPI', $ex);
    	expertusErrorThrow($ex);
    }
}

function updateFacilityByRestAPI( &$form_state,$createuserid="") 
{
	try{
	    drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
			$form=array();
			$form["form_id"]="";
			$facilityId =  _updateFacilityDetails($form,$form_state,$createuserid,1);
			expDebug::dPrint(' updateFacilityByRestAPI facility:' . print_r($facilityId,true) , 4);
	    return $facilityId;  
    } catch (Exception $ex) {
    	watchdog_exception('updateFacilityByRestAPI', $ex);
    	expertusErrorThrow($ex);
    }
}

?>