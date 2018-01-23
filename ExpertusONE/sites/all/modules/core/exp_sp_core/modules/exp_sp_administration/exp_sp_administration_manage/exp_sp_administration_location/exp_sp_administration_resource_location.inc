<?php


/*
 * locationAdminILTClassEnrollmentsConflict() - Checks whether there is an ILT class whose enrollments exceed the specified capacity.
 */
function locationAdminILTClassEnrollmentsConflict($locationId, $capacity) {
	try {
  expDebug::dPrint('$locationId = ' . $locationId, 4);
  expDebug::dPrint('$capacity = ' . $capacity, 4);

  // Select tables
  $select = db_select('slt_enrollment', 'enr');
  $select->leftJoin('slt_course_class', 'cls', 'enr.class_id = cls.id');
  $select->leftJoin('slt_course_template', 'crs', 'cls.course_id = crs.id');

  // Add fields to fetch
  $select->addField('cls', 'title', 'clstitle');
  $select->addExpression('COUNT(enr.id)', 'enrcount');

  // Add conditions and groupBy
  $select->condition('cls.delivery_type', 'lrn_cls_dty_ilt', '=');
  $select->condition('cls.location_id', $locationId, '=');
  $select->condition('cls.status', array('lrn_cls_sts_atv', 'lrn_cls_sts_itv'), 'IN');
  $select->condition('crs.status', array('lrn_crs_sts_atv', 'lrn_crs_sts_itv'), 'IN');
  $select->condition(db_or()
                     ->condition(db_and()
                                 ->condition('enr.reg_status', 'lrn_crs_reg_cnf', '=')
                                 ->condition('enr.comp_status', array('lrn_crs_cmp_inp', 'lrn_crs_cmp_enr'), 'IN'))
                     ->condition('enr.reg_status', array('lrn_crs_reg_ppm', 'lrn_crs_reg_wtl'), 'IN'));
  $select->groupBy('enr.class_id');
  $select->orderBy('enrcount', 'DESC');
  $select->range(0,1);
  expDebug::dPrintDBAPI('$select', $select);

  $maxEnrollmentsClassObj = $select->execute()->fetch();
  expDebug::dPrint('$maxEnrollmentsClassObj = ' . print_r($maxEnrollmentsClassObj, true) , 3);

  if (empty($maxEnrollmentsClassObj) || $maxEnrollmentsClassObj->enrcount <= $capacity) {
    return false; // No conflict
  }

  return $maxEnrollmentsClassObj->clstitle; // return class title on conflict
  } catch (Exception $ex) {
  	watchdog_exception('locationAdminILTClassEnrollmentsConflict', $ex);
  	expertusErrorThrow($ex);
  }
}


function exp_sp_administration_location_location_addedit_form_submit($form, &$form_state) {
  try {
  $custom_esign_check = 0;
  $chkValidation = 1;
  expDebug::dPrint('$form_state[triggering_element][#name] = ' . $form_state['triggering_element']['#name'], 4);
  if (module_exists('exp_sp_esignature') && isActiveEsignRegion() && $custom_esign_check == 1) {
     $chkValidation = 0;
     if ($form_state['hidden-save-submit'] == true && $_SESSION['esign_validate'] ==  1) {
       $chkValidation = 1;
       $form_state['storage']['submit_triggering_element'] = $form_state['storage']['triggering_button_element'];
     }
     else{
       $form_state['storage']['triggering_button_element'] = $form_state['triggering_element']['#name'];
     }
   }
   else{
     $form_state['storage']['submit_triggering_element'] = $form_state['triggering_element']['#name'];
   }

   //if($form_state['hidden-save-submit']==true){
   if ($chkValidation) {
     $form_state['esign_validation_status'] = true;
     expDebug::dPrint('$form_state = ' . print_r($form_state, true), 5);
     // Check whether add or edit
     $loggedInUserId = getIdOfLoggedInUser();
     if(empty($form_state['values']["loc_id"])) { // Add

        $locationId  = _addLocationDetails($form, $form_state,$loggedInUserId);

        if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Save Custom Attribute Values
            include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
            saveCustomAttributesInAddEditForm($form, $form_state, $locationId, 'cre_sys_obt_loc', $loggedInUserId, 'Inserted');
        }else{
        saveCustomFieldsInAddEditForm($form, $form_state, $locationId, 'location', $loggedInUserId);
        } //#custom_attribute_0078975 - End Check module status 
        
        expertus_set_message(t('MSG601').'.');
        $form_state['build_info']['args'][] = $locationId;

        $form_state['storage']['locationId'] = $locationId;
        $form_state['storage']['locationName'] = $form_state['values']['name'];

        $form_state['storage']['new_entity'] = TRUE;
     }
     else {
	     if ($form_state['storage']['submit_triggering_element'] == 'unpublish') {
	     	 include_once('exp_sp_administration_location.inc');
	       publishOrUnpublishLocationStatus($form_state['values']['loc_id'], false);
	     	 expertus_set_message(t('MSG348'));
	     }
       else {
         _updateLocationDetails($form, $form_state);
         
         if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Save Custom Attribute Values
             include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
             saveCustomAttributesInAddEditForm($form, $form_state, $form_state['values']["loc_id"], 'cre_sys_obt_loc', $loggedInUserId, 'Updated');
         }else{
         saveCustomFieldsInAddEditForm($form, $form_state, $form_state['values']["loc_id"], 'location', $loggedInUserId);
         } //#custom_attribute_0078975 - End Check module status 
         
         updateSessionTimeZone($form_state['values']['loc_id'],$form_state['values']['timezone']);
         expertus_set_message(t('MSG601').'.');
       }
       $form_state['storage']['step']          = 'edit';
       $entityId                               = $form_state['values']['loc_id'];
       $form_state['storage']['edited_entity'] = TRUE;
     }

          unset($form_state['input']['latitude']);
          unset($form_state['input']['longitude']);
          unset($form_state['values']['latitude']);
          unset($form_state['values']['longitude']);

     // Setting $form_state['rebuild'] = TRUE causes the form to be rebuilt again.
     expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 5);

     $form_state['rebuild'] = TRUE;
  }
  } catch (Exception $ex) {
  	watchdog_exception('exp_sp_administration_location_location_addedit_form_submit', $ex);
  	expertusErrorThrow($ex);
  }
}

function updateSessionTimeZone($locationId, $timezone){
  $txn = db_transaction();
	try{
	  $timezone = ($timezone) ? $timezone : NULL;
	  $updateSesSts = db_update('slt_course_class_session');
  	$updateSesSts->condition('location_id',$locationId);
  	$updateSesSts->fields(array(
                      'timezone' => $timezone,
                      ));
    $updateSesSts->execute();
    }catch (Exception $ex) {
      $txn->rollback();
	    unset($txn);
	   watchdog_exception('updateSessionTimeZone', $ex);
	   expertusErrorThrow($ex);
  	}
  unset($txn);
}

function edit_location_details(&$form, &$form_state, $defaultValue){
  try {
  unset($editAttributes['onclick']);
  $editAttributes['class'][]         = 'addedit-form-expertusone-throbber';
  $editAttributes['class'][]         = 'edit-location-btn';
  $editAttributes['data-wrapperid']  = array('location-basic-addedit-form');
  $editAttributes['class'][]         = 'admin-add-edit-button';
  $editAttributes['onclick'][]       = '$(document).ready(function(){ editLocationDetailsView(); }); remove_messages("location");';

  $form['edit_btn'] = array(
      '#type' => 'submit',
	  '#value'      => t('LBL063'),
      '#name'       => 'Edit Location',
      '#prefix'     => '<div id="wizard-location-form-wrapper"><div class="admin-course-edit-button-container">',
      '#suffix'     => '</div></div>',
      '#attributes' => $editAttributes,
      '#ajax' => array(
  		'callback' => 'addEditRepaintForm',
        'wrapper'  => 'location-basic-addedit-form',
        'event'	   => 'click'
      ),
    );
  } catch (Exception $ex) {
  	watchdog_exception('edit_location_details', $ex);
  	expertusErrorThrow($ex);
  }

}

function edit_location_view(&$form, &$form_state, $defaultValue){
  try {
   $form['location_name'] = array(
        '#type'     => 'item',
        '#title'    => t('LBL311').' '.':',
    	'#prefix'   => '<div class="addedit-new-field"><div class="addedit-course-field-title">',
        '#markup'   => '</div><div class="addedit-new-field-value view-mode-field-value-txt">'.$defaultValue['loc_name'].'</div><div class="clearBoth"></div>',
    	'#suffix'   => '</div>',
     );
   } catch (Exception $ex) {
   	watchdog_exception('edit_location_view', $ex);
   	expertusErrorThrow($ex);
   }
}

function _getDefaultLocationAddEditFormFieldValue($fields, $form_state, $locationDetails) {
	try {
  $defaultValue 	= '';
  $locationDefaultValue 	= array();
  for($i=0;$i<count($fields);$i++) {
    $field                   = $fields[$i];
    $locationDefaultValue[$field] = empty($form_state['values'][$field]) ? (!empty($locationDetails->$field) ? $locationDetails->$field : '') : $form_state['values'][$field];
  }
  return $locationDefaultValue;
  } catch (Exception $ex) {
  	watchdog_exception('_getDefaultLocationAddEditFormFieldValue', $ex);
  	expertusErrorThrow($ex);
  }
}

function _getLocationDetails($locationId) {
	try {
  $select = db_select("slt_location","loc");
  $select->addField('loc','id', 'loc_id');
  $select->addField('loc','name');
  $select->addField('loc','capacity');
  $select->addField('loc','addr1');
  $select->addField('loc','addr2');
  $select->addField('loc','city');
  $select->addField('loc','country');
  $select->addField('loc','state');
  $select->addField('loc','phone');
  $select->addField('loc','status');
  $select->addField('loc','zipcode');
  $select->addField('loc','contact_fname');
  $select->addField('loc','email_id');
  $select->addField('loc','equipment');
  $select->addField('loc','timezone');
  $select->addField('loc','latitude');
  $select->addField('loc','longitude');
  $select->condition('loc.id',$locationId,'=');
  $locationList = $select->execute()->fetchAssoc();

  return $locationList;
  } catch (Exception $ex) {
  	watchdog_exception('_getLocationDetails', $ex);
  	expertusErrorThrow($ex);
  }
}


function _updateLocationDetails($form, &$form_state, $createUserId, $callFromAPI =0) {

  $txn = db_transaction();
  expDebug::dPrint(' _updateLocationDetails  :'.print_r($form_state['values'], true) , 5);
  try {

    $emptyField = null;
    $selectedEquipments = $form_state['values']['equipment'];
    if (!empty($selectedEquipments)) {
      if(!$callFromAPI){
         $updateFields['equipment'] = implode(',', $selectedEquipments);
       }
       else{
         $updateFields['equipment'] = $selectedEquipments;
       }
    } else {
      //$updateFields['equipment'] = $emptyField;
    }

    $trigger_element = $form_state['storage']['submit_triggering_element'];
    if ($trigger_element == 'saveandpublish') {
      $location_status = 'lrn_res_loc_atv';
    }
    else if ($trigger_element == 'save') {
      $location_status = 'lrn_res_loc_itv';
    }
    else if($form_state['values']['status']) {
  	   $location_status = $form_state['values']['status'];
    }

    expDebug::dPrint(' Geographical Form State Values Edit = ' . print_r($form_state['values'], true) , 4);
    expDebug::dPrint(' $callFromAPI = ' .print_r($form_state['$callFromAPI'], true) , 4);

    $country = getCountryValue($form_state['values']['country']);
    $state = getStateValue($form_state['values']['country'],$form_state['values']['state']);
	expDebug::dPrint('$state' . print_r($state, true) , 4);
		  	      expDebug::dPrint('$state[0][cnt]' . print_r($state[0]->cnt, true) , 4);
		if(!empty($callFromAPI)){
				  				  					// state exists or not in slt state table			  
				$selstate  = getstate($form_state['values']['state']);
			    expDebug::dPrint('$selstate' . print_r($selstate, true) , 4);
				// country exists or not in slt country table			  
				$selcountry = getcountry($form_state['values']['country']);

                expDebug::dPrint('$selcountry' . print_r($selcountry[0]->cnt1, true) , 4);
				expDebug::dPrint('$selcountry' . print_r($selcountry[0]->country_name, true) , 4);

				// states exists or not for the given country
				$statecountry=	getStatecountrycheck($form_state['values']['country']);		  
				expDebug::dPrint('$statecountry' . print_r($statecountry, true) , 4);
				if(empty($selstate) && ($form_state['values']['state']!='NA'))
				{
				expDebug::dPrint(' hi 1 update = ' . print_r($selstate, true) , 4);
				$errobj=new stdClass();
			    $errobj->isValidateError = 1;
			    $errobj->errcode = 'L_012';
			    $errobj->errormsg = "Invalid State code.";
			    expDebug::dPrint(' error message= ' . print_r($selstate, true) , 4);
			    return $errobj;
				} else {
			      // Invalid state entered in the valid country
	              if($state[0]->cnt == 0 && $selcountry[0]->cnt1>0 && !empty($statecountry)){
	              expDebug::dPrint(' hi 2 update= ' . print_r($state[0]->cnt, true) , 4);
	              $errobj=new stdClass();
			      $errobj->isValidateError = 1;
			      $errobj->errcode = 'L_012';
			      $errobj->errormsg = "Invalid State code.";
			      expDebug::dPrint(' error message= ' . print_r($errobj->errormsg, true) , 4);
				  return $errobj;

				 } else if($state[0]->cnt == 0 && $selcountry[0]->cnt1==0) {
		         expDebug::dPrint(' here 2 update= ' . print_r($state[0]->cnt, true) , 4);
	             $errobj=new stdClass();
			     $errobj->isValidateError = 1;
			     $errobj->errcode = 'L_012';
			     $errobj->errormsg = "Invalid Country code.";
			       expDebug::dPrint(' error message= ' . print_r($errobj->errormsg, true) , 4);
				 return $errobj;
				 } else if ($state[0]->cnt == 0){
	             expDebug::dPrint(' hi 3 update = ' . print_r($state[0]->cnt, true) , 4);
                 $updateFields['state']=null;
	} else {
				 expDebug::dPrint(' hi 4= ' . print_r($state[0]->cnt, true) , 4);
                 $updateFields['state']= trim($form_state['values']['state']);
				  	}
}
    //if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Custom Attrbiutes Field Validation
		$ret_tmp = getcustomattributemodulestatus();
		if($ret_tmp == true){
        include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'exp_sp_administration_customattribute.inc');
        $custom_attr_error = validateCustomAttribute($form_state,'cre_sys_obt_loc');
        expDebug::dPrint('Update Location API custom attribute Validation Result--> '.print_r($custom_attr_error,true), 5);
        if($custom_attr_error != '') {
            $customErrmsg=new stdClass();
            $customErrmsg->isValidateError = 1;
            $customErrmsg->errormsg = $custom_attr_error;
            return $customErrmsg;
        }
    } //#custom_attribute_0078975 - End Check module status
}
else {
	             $updateFields['state'] = empty($form_state['values']['state']) ? $emptyField : trim($form_state['values']['state']);
}

	
    expDebug::dPrint(' Geographical Country Value Edit = ' . print_r($country, true) , 4);
    expDebug::dPrint(' Geographical State Value Edit = ' . print_r($state, true) , 4);

    $default= db_query("SELECT id,name,capacity,addr1,addr2,city,country,state,zipcode,phone,email_id,contact_fname,equipment,status from slt_location where id=:uid",array(":uid"=>$form_state['values']['loc_id']))->fetchAll();
    expDebug::dPrint(' Default Values = ' . print_r($default, true) , 4);

    $arrDefaultValues = array($default[0]->country,$default[0]->state,$default[0]->city);
    $arrCurrentValues = array($form_state['values']['country'],$form_state['values']['state'],$form_state['values']['city']);
    expDebug::dPrint(' Default Array Values = ' . print_r($arrDefaultValues, true) , 4);
    expDebug::dPrint(' Current Array Values = ' . print_r($arrCurrentValues, true) , 4);

    $arrValues = array_diff($arrDefaultValues, $arrCurrentValues);
    expDebug::dPrint(' Current Array Values = ' . print_r($arrValues, true) , 4);

  if(empty($form_state['values']['latitude']) && empty($form_state['values']['longitude'])){
      $address = trim($country) . ',' . trim($state) . ',' . trim($form_state['values']['city']) . ',' . trim($form_state['values']['addr2']) . ',' . trim($form_state['values']['addr1']);
      $geo = getLatitudeLongitudeFromGoogleAPI($address);
      expDebug::dPrint(' Geographical Position Velu = ' . print_r($geo, true) , 4);
      $latitude = $geo['latitude'];
      $longitude = $geo['longitude'];
    }else{
      $latitude = $form_state['values']['latitude'];
      $longitude = $form_state['values']['longitude'];
    }

    $locationId = $form_state['values']['loc_id'];
    $locExist =_getLocationDetails($locationId); // Get the Location Details
     if (empty($locExist)) { // If Location details Not Exist Return The Error Location Not Exist.
    	//return array((object)array('id'=>'Location does not exist.'));
     	$errobj=new stdClass();
     	$errobj->isValidateError = 1;
     	$errobj->errcode = 'L_012';
     	$errobj->errormsg ='Location does not exist.';
     	return $errobj;
    }
    $updateQuery  	= db_update('slt_location');

    $updateFields['name'] = empty($form_state['values']['name']) ? $emptyField : trim($form_state['values']['name']);
    $updateFields['capacity'] = empty($form_state['values']['capacity']) ? $emptyField : trim($form_state['values']['capacity']);
    if(isset($form_state['values']['addr1']))
    $updateFields['addr1'] = empty($form_state['values']['addr1']) ? $emptyField : trim($form_state['values']['addr1']);
    if(isset($form_state['values']['addr2']))
    $updateFields['addr2'] = empty($form_state['values']['addr2']) ? $emptyField : trim($form_state['values']['addr2']);
    $updateFields['city'] = empty($form_state['values']['city']) ? $emptyField : trim($form_state['values']['city']);
    $updateFields['country'] = empty($form_state['values']['country']) ? $emptyField : trim($form_state['values']['country']);
    //$updateFields['state'] = empty($form_state['values']['state']) ? $emptyField : trim($form_state['values']['state']);
    $updateFields['zipcode'] = empty($form_state['values']['zipcode']) ? $emptyField : trim($form_state['values']['zipcode']);
    if(isset($form_state['values']['contact_fname']))
    $updateFields['contact_fname'] = empty($form_state['values']['contact_fname']) ? $emptyField : trim($form_state['values']['contact_fname']);
    if(isset($form_state['values']['contact_lname']))
    $updateFields['contact_lname'] = $emptyField;
    if(isset($form_state['values']['phone']))
    $updateFields['phone'] = empty($form_state['values']['phone']) ? $emptyField : trim($form_state['values']['phone']);
    if(isset($form_state['values']['email_id']))
    $updateFields['email_id'] = empty($form_state['values']['email_id']) ? $emptyField : trim($form_state['values']['email_id']);
    $updateFields['direction'] = $emptyField;
    if(isset($form_state['values']['latitude']))
    $updateFields['latitude'] = empty($latitude) ? 0 : $latitude;
    if(isset($form_state['values']['longitude']))
    $updateFields['longitude'] = empty($longitude) ? 0 : $longitude;
    $updateFields['timezone'] = empty($form_state['values']['timezone']) ? $emptyField : trim($form_state['values']['timezone']);
    $updateFields['gmtoffset'] = 0;
    $updateFields['dstoffset'] = 0;
    $updateFields['updated_by'] = $createUserId;
    $updateFields['updated_on'] = now();
    $updateFields['is_active'] = 1;
    $updateFields['status'] = $location_status;//'lrn_res_loc_atv';

    $updateQuery->fields($updateFields);
    $updateQuery->condition('id', $locationId);
     // Execute the update statement.
    $numUpdated = $updateQuery->execute();

    make_audit_trail_entries($updateFields, getIdOfLoggedInUser() , $default, $locationId ,'Updated','cre_sys_obt_loc','exp_sp_administration_resources','_updateLocationDetails',
                                          array('name','capacity','addr1','addr2','city','country','state','zipcode','contact_fname','phone','email_id','equipment','status','latitude','longitude'));

    if($callFromAPI){
        //if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Save Custom Attributes
    	$ret_tmp = getcustomattributemodulestatus();
    	if($ret_tmp == true){
        include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
        saveCustomAttributesInAddEditForm($form, $form_state, $locationId, 'cre_sys_obt_loc', $createUserId,'Updated',1);
        } //#custom_attribute_0078975 - End Check module status
      return array((object)array('id' => $locationId));
    }

  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('_updateLocationDetails', $ex);
    if($callFromAPI){
      //return array((object)array('id'=>'Failiure'));
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Error in Location Id.";
    	return $errobj;
    }
    throw $ex;
  }

  // Commit the transaction
  unset($txn);

}

function _addLocationDetails($form, &$form_state, $createUserId, $callFromAPI =0) {

  $txn = db_transaction();

  try {

    $locationName = $form_state['values']['loc_name'];
    $emptyField = null;

    $selectedEquipments = $form_state['values']['equipment'];

    if (!empty($selectedEquipments)) {
       if(!$callFromAPI){
         $insertFields['equipment'] = implode(',', $selectedEquipments);
       }
       else{
         $insertFields['equipment'] = $selectedEquipments;
       }
    } else {
      $insertFields['equipment'] = $emptyField;
    }

    $trigger_element = $form_state['storage']['submit_triggering_element'];
     if($trigger_element == 'saveandpublish'){
       $location_status = 'lrn_res_loc_atv';
     }
     else if($trigger_element == 'save'){
       $location_status = 'lrn_res_loc_itv';
     }
     else if($form_state['values']['status']){
  	   $location_status =$form_state['values']['status'];
     }

     expDebug::dPrint(' Geographical Form State Values Add = ' . print_r($form_state['values'], true) , 4);
    expDebug::dPrint(' $callFromAPI = ' .print_r($form_state['$callFromAPI'], true) , 4);

     $country = getCountryValue($form_state['values']['country']);
     $state = getStateValue($form_state['values']['country'],$form_state['values']['state']);
	 expDebug::dPrint('$state' . print_r($state, true) , 4);
     expDebug::dPrint('$state[0][cnt]' . print_r($state[0]->cnt, true) , 4);
	 
	 if(!empty($callFromAPI)){
				  					// state exists or not in slt state table			  
	 	$selstate  = getstate($form_state['values']['state']);
		expDebug::dPrint('$selstate' . print_r($selstate, true) , 4);
		// country exists or not in slt country table			  
		$selcountry = getcountry($form_state['values']['country']);
		
		expDebug::dPrint('$selcountry' . print_r($selcountry[0]->cnt1, true) , 4);
		expDebug::dPrint('$selcountry' . print_r($selcountry[0]->country_name, true) , 4);
		
		// states exists or not for the given country 			  
		$statecountry =	getStatecountrycheck($form_state['values']['country']);		  
		expDebug::dPrint('$statecountry' . print_r($statecountry, true) , 4);
		if(empty($selstate) && ($form_state['values']['state']!='NA'))
		{
			expDebug::dPrint(' hi 1= ' . print_r($selstate, true) , 4);
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Invalid State code.";
			expDebug::dPrint(' error message= ' . print_r($selstate, true) , 4);
			return $errobj;
		} else {
			// Invalid state entered in the valid country
			if($state[0]->cnt == 0 && $selcountry[0]->cnt1>0 && !empty($statecountry)) {
				expDebug::dPrint(' hi 2= ' . print_r($state[0]->cnt, true) , 4);
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Invalid State code.";
			     expDebug::dPrint(' error message= ' . print_r($errobj->errormsg, true) , 4);
				return $errobj;
				
			} else if($state[0]->cnt == 0 && $selcountry[0]->cnt1==0) {
				expDebug::dPrint(' here 2= ' . print_r($state[0]->cnt, true) , 4);
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Invalid Country code.";
			    expDebug::dPrint(' error message= ' . print_r($errobj->errormsg, true) , 4);
				return $errobj;
			} else if ($state[0]->cnt == 0) {
				expDebug::dPrint(' hi 3= ' . print_r($state[0]->cnt, true) , 4);
     			$insertFields['state']=null;
			} else {
				expDebug::dPrint(' hi 4= ' . print_r($state[0]->cnt, true) , 4);
    			$insertFields['state']= trim($form_state['values']['state']);
			}
	}

    //if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Custom Attrbiutes Field Validation
	    $ret_tmp = getcustomattributemodulestatus();
	    if($ret_tmp == true){
        include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'exp_sp_administration_customattribute.inc');
        $custom_attr_error = validateCustomAttribute($form_state,'cre_sys_obt_loc');
        expDebug::dPrint('Create Location API custom attribute Validation Result--> '.print_r($custom_attr_error,true), 5);
        if($custom_attr_error != '') {
            $customErrmsg=new stdClass();
            $customErrmsg->isValidateError = 1;
            $customErrmsg->errormsg = $custom_attr_error;
            return $customErrmsg;
        }
    } //#custom_attribute_0078975 - End Check module status
}
else {
	$insertFields['state'] = empty($form_state['values']['state']) ? $emptyField : trim($form_state['values']['state']);
	
}

		  
	 
     expDebug::dPrint(' Geographical Country Value Add = ' . print_r($country, true) , 4);
     expDebug::dPrint(' Geographical State Value Add = ' . print_r($state, true) , 4);

    //Here we get latitude and longitude from given address
    if(empty($form_state['values']['latitude']) && empty($form_state['values']['longitude'])){
      $address = trim($country) . ',' . trim($state) . ',' . trim($form_state['values']['city']) . ',' . trim($form_state['values']['addr2']) . ',' . trim($form_state['values']['addr1']);
      $geo = getLatitudeLongitudeFromGoogleAPI($address);
      expDebug::dPrint(' Geographical Position Velu = ' . print_r($geo, true) , 4);
      $latitude = $geo['latitude'];
      $longitude = $geo['longitude'];
    }else{
      $latitude = $form_state['values']['latitude'];
      $longitude = $form_state['values']['longitude'];
    }

    $insertFields['name'] = empty($form_state['values']['name']) ? $emptyField : trim($form_state['values']['name']);
    $insertFields['capacity'] = empty($form_state['values']['capacity']) ? $emptyField : trim($form_state['values']['capacity']);
    $insertFields['addr1'] = empty($form_state['values']['addr1']) ? $emptyField : trim($form_state['values']['addr1']);
    $insertFields['addr2'] = empty($form_state['values']['addr2']) ? $emptyField : trim($form_state['values']['addr2']);
    $insertFields['city'] = empty($form_state['values']['city']) ? $emptyField : trim($form_state['values']['city']);
    $insertFields['country'] = empty($form_state['values']['country']) ? $emptyField : trim($form_state['values']['country']);
    // $insertFields['state'] = empty($state) ? $emptyField : trim($form_state['values']['state']);
    $insertFields['zipcode'] = empty($form_state['values']['zipcode']) ? $emptyField : trim($form_state['values']['zipcode']);
    $insertFields['contact_fname'] = empty($form_state['values']['contact_fname']) ? $emptyField : trim($form_state['values']['contact_fname']);
    $insertFields['contact_lname'] = $emptyField;
    $insertFields['phone'] = empty($form_state['values']['phone']) ? $emptyField : trim($form_state['values']['phone']);
    $insertFields['email_id'] = empty($form_state['values']['email_id']) ? $emptyField : trim($form_state['values']['email_id']);
    $insertFields['direction'] = $emptyField;
    $insertFields['latitude'] = empty($latitude) ? 0 : $latitude;
    $insertFields['longitude'] = empty($longitude) ? 0 : $longitude;
    $insertFields['timezone'] = empty($form_state['values']['timezone']) ? $emptyField : trim($form_state['values']['timezone']);
    $insertFields['gmtoffset'] = 0;
    $insertFields['dstoffset'] = 0;
    $insertFields['created_by'] = $createUserId;
    $insertFields['created_on'] = now();
    $insertFields['updated_by'] = $createUserId;
    $insertFields['updated_on'] = now();
    $insertFields['status'] = $location_status;//'lrn_res_loc_atv';
    $insertFields['is_active'] = 1;

    expDebug::dPrint(' $insertFields = ' . print_r($insertFields, true) , 4);

    $insertQuery  	= db_insert('slt_location');
    $insertQuery->fields($insertFields);
    expDebug::dPrintDBAPI(' Query string : ' , $insertQuery);
    $locationId     = $insertQuery->execute();
    $emptyId = $form_state['values']['empty_id'];
    if($callFromAPI) {
    	$classCreateUser = $createUserId;
    }
    else {
    	$classCreateUser = getIdOfLoggedInUser();
    }
    updateGroupMappingDetails($locationId, 'cre_sys_obt_loc', $classCreateUser,'emptyadd',0);
    _updateAccessEntity($locationId, 'cre_sys_obt_loc', $emptyId);
    make_audit_trail_entries($insertFields, getIdOfLoggedInUser() , null, $locationId ,'Inserted','cre_sys_obt_loc','exp_sp_administration_resources','_addLocationDetails',
                                          array('name','capacity','addr1','addr2','country','city','state','zipcode','contact_fname','phone','email_id','equipment','status','latitude','longitude'));

    if($callFromAPI){
        
        //if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Save Custom Attributes
    	$ret_tmp = getcustomattributemodulestatus();
    	if($ret_tmp == true){
            include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
            saveCustomAttributesInAddEditForm($form, $form_state, $locationId, 'cre_sys_obt_loc', $createUserId,'Inserted',1);
        } //#custom_attribute_0078975 - End Check module status
        
      return array((object)array('id'=>$locationId));
    } else {
      return $locationId;
    }

  } catch (Exception $ex) {

    $txn->rollback();
    watchdog_exception('_addLocationDetails', $ex);

    if($callFromAPI){
     // return array((object)array('id'=>'Failure'));
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Error in Location Id.";
    	return $errobj;
    }
    throw $ex;
  }

  // Commit the transaction
  unset($txn);
}

function locationAddEditFormWithCancelAndSaveClose(&$form, $wrapperId = 'addedit-form-wrapper', $saveButtonId = 'save-button', $cancelButtonId = 'cancel-button',  $locationDetails = array(),$fromToolTip) {
	try {
  expDebug::dPrint(' userAddEditFormWithCancelAndSaveClose() called.');

  $customHiddenButtonId = 'last-button-account';
  $esignHiddenButt = 'esign-'.$wrapperId;
  $esignHiddenButt1= 'status-upd-esign-'.$wrapperId;

  $ajaxCallbackCancel = 'addEditCancelRepaintForm';

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $cancelAttributes['class'] = array('addedit-edit-' . $cancelButtonId);

  // For common styling of all addedit cancel buttons
 // $cancelAttributes['class'][] = 'addedit-edit-cancel';
  $cancelAttributes['class'][] = 'admin-action-button-middle-bg';

  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  $cancelAttributes['data-wrapperid'] = array($wrapperId);

  // A common class for all addedit form cancel button, for styling to appear as a link.
  $cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';

  // Should be closed always - Ticket # 13936
  //if(empty($userDetails)){
    $cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss();return false;';
  //}

  $form[$cancelButtonId] = array(
    '#type' => 'submit',
     '#prefix' => '<div class="addedit-form-cancel-and-save-actions-row">' .
                   '<div class="addedit-form-cancel-container-actions">',
    //'#suffix' =>   '<div class="admin-save-button-right-bg"></div>',
    '#value' => t('LBL123'),
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
  $ajaxCallbackSave = 'addEditRepaintForm';

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $saveAttributes['class'] = array('addedit-edit-' . $saveButtonId);

  // For common styling of all addedit save buttons
  //$saveAttributes['class'][] = 'addedit-edit-save';
  $saveAttributes['class'][] = 'admin-save-button-middle-bg';

  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  $saveAttributes['data-wrapperid'] = array($wrapperId);

  if($fromToolTip == 'Yes') {
    $eSignatureAvailable = false;
  }else{
    $eSignatureAvailable = true;
  }
  if(!empty($locationDetails)){
    $saveAttributes['class'][] = 'edit-catalog-course-save-publish';
    if(module_exists('exp_sp_esignature') && isActiveEsignRegion() && $eSignatureAvailable){
      $saveAttributes['onclick'] = array();
      //$saveAttributes['onclick'][] = 'Drupal.ajax.prototype.commands.addEditHideMessages();';
      $form[$saveButtonId] = array(
    	      '#type' => 'submit',
    	      '#value' => t('LBL141'),
              '#name' => 'esign-call-save',
    	   	  /*'#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
    	       '#suffix' =>
    	               '<div class="admin-save-button-right-bg"></div></div>' .
    	               '<div class="clearBoth"></div></div>' .
    	             '<div class="clearBoth"></div></div>',*/
              '#prefix' => '<div class="admin-save-pub-unpub-button-container">',
              '#suffix' => '<span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn ">&nbsp;</span></div>',
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

      $saveAttributes['onclick'] = array();
      $form[$esignHiddenButt] = array(
	      '#type' => 'submit',
	      '#value' => t('hidden-save'),
	      '#name'	=> 'hidden-save',
      	  '#prefix' => '<span id = "esign-admin-div" style="display:none">',
	      '#suffix' => '</span>',
	      '#id' =>$esignHiddenButt,
  		  '#attributes' => $saveAttributes,
	      '#ajax' => array(
		      'wrapper' => $wrapperId,
		      'callback' => $ajaxCallbackSave,
		      'method' => 'replace',
		      'effect' => 'none', // 'fade',
		      'event'=>'click',
  			  'userActionStatus'=>'',
		      'keypress' => true,
		      'progress' => array(
		      'type' => 'throbber',
		      'message' => ''
		      ),
	    	)
	      );

      }
      else{
        $saveAttributes['onclick'] = array();
        //$saveAttributes['onclick'][] = '$("#'.$esignHiddenButt.'").click();';
        $form[$saveButtonId] = array(
        '#type' => 'submit',
        '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
        /*'#suffix' =>
                   '<div class="admin-save-button-right-bg"></div></div>' .
                   '<div class="clearBoth"></div></div>' .
                 '<div class="clearBoth"></div></div>',*/
        '#prefix' => '<div class="admin-save-pub-unpub-button-container">',
        '#suffix' => '<span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn ">&nbsp;</span></div>',
          '#value' => t('LBL141'),
        	'#name' => 'save',
        '#attributes' => $saveAttributes,
          '#ajax' => array(
            'wrapper' => $wrapperId,
            'callback' => $ajaxCallbackSave,
            'method' => 'replace',
            'effect' => 'none', // 'fade',
            'event'=>'click',
            'userActionStatus'=>'',
            'keypress' => true,
            'progress' => array(
              'type' => 'throbber',
              'message' => ''
            ),
        	)
        );
      }



      $form['addedit-button-group-ul-start'] = array(
          '#type' => 'markup',
          '#markup' => '<ul class="catalog-pub-add-list">',
        );

  	  if($locationDetails['status'] == 'lrn_res_loc_atv'){
  	    $activateSuspendLabel  = t('LBL572');
  	    $locationActionStatus  = 'Suspend';
  	  }
  	  else{
  	    $activateSuspendLabel  = t('LBL573');
  	    $locationActionStatus  = 'Activate';
  	  }
  	  $activateSuspendButtonId = 'location-activate-suspend-button';
  	  if(module_exists('exp_sp_esignature') && isActiveEsignRegion() && $eSignatureAvailable){
        $activateSuspendAttributes['onclick'] = array();
        $activateSuspendAttributes['onclick'][] = 'increasePopWidth();';
        $form[$activateSuspendButtonId] = array(
  	    '#type' => 'submit',
  	    '#prefix' => '<li class="save-pub-unpub-sub-menu">',
  	    '#suffix' => '</li>',
  	    '#value' => $activateSuspendLabel,
        '#name' => 'location-activate-suspend',
        '#id' =>$activateSuspendButtonId,
  	    '#attributes' => $activateSuspendAttributes,
        	'#ajax' => array(
    	      'wrapper' => $wrapperId,
    	      'callback' => $ajaxCallbackSave,
    	      'method' => 'replace',
    	      'effect' => 'none', // 'fade',
    	      'event'=>'click',
        	  'customButtonId'=>$esignHiddenButt1,
    	      'keypress' => true,
    	      'progress' => array(
    	        'type' => 'throbber',
    	        'message' => ''
    	      ),
        	)
  	   );
  	  }else{
    	  $activateSuspendAttributes['onclick'] = array();
          $activateSuspendAttributes['onclick'][] = '$("#'.$esignHiddenButt1.'").click();';
          $form[$activateSuspendButtonId] = array(
  	    	'#type' => 'submit',
  	    	'#prefix' => '<li class="save-pub-unpub-sub-menu">',
  	    	'#suffix' => '</li>',
            '#value' => $activateSuspendLabel,
          	'#name' => 'location-activate-suspend',
    	    '#attributes' => $activateSuspendAttributes,
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
      $activateSuspendAttributes['onclick'] = array();
      $form[$esignHiddenButt1] = array(
    	      '#type' => 'submit',
    	      '#value' => t('hidden-save'),
    	      '#name'	=> 'location-status-upd-hidden-save',
          	  '#prefix' => '<span id = "esign-admin-div" style="display:none">',
    	      '#suffix' => '</span>',
    	      '#id' =>$esignHiddenButt1,
      		  '#attributes' => $activateSuspendAttributes,
    	      '#ajax' => array(
    		      'wrapper' => $wrapperId,
    		      'callback' => $ajaxCallbackSave,
    		      'method' => 'replace',
    		      'effect' => 'none', // 'fade',
    		      'event'=>'click',
                  'locationActionStatus' => $locationActionStatus,
    		      'keypress' => true,
    		      'progress' => array(
    		      'type' => 'throbber',
    		      'message' => ''
    		      ),
    	    	)
    	      );

      $deleteAccess = delete_object_record($locationDetails['loc_id'], 'location');
      if(!$deleteAccess){
        $displayTitle = titleController('ADMIN-RESOURCE-LOCATION-DELETE-ACCESS', $locationDetails['name'],34);
  	    $deleteButtonId = 'user-delete-dropdown-button';
  	    $deleteAttributes['onclick'] = array();
        $deleteAttributes['onclick'][] = '$("#root-admin").data("narrowsearch").displayDeleteWizard("'.t('MSG357').' '.strtolower(t('Location')).' ' .escape_string($locationDetails['name']) . '","'.$locationDetails['loc_id'].'","location");';
        $deleteAttributes['class'][] = 'admin-delete-button-bg';
        $form[$deleteButtonId] = array(
    	    '#type' => 'button',
    	    '#prefix' => '<li class="save-pub-unpub-sub-menu">',
    	    '#suffix' => '</li>',
    	    '#value' => t('LBL286'),
            '#name' => 'delete',
    	    '#attributes' => $deleteAttributes,
    	  );
      }
      $form['addedit-button-group-ul-end'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="clearBoth"></div></div>' .
  	             '<div class="clearBoth"></div></div></ul>',
      );
  	}else{
      if(module_exists('exp_sp_esignature') && isActiveEsignRegion() && $eSignatureAvailable){
        $saveAttributes['onclick'] = array();
        //$saveAttributes['onclick'][] = 'Drupal.ajax.prototype.commands.addEditHideMessages();';
        $form[$saveButtonId] = array(
      	      '#type' => 'submit',
      	      '#value' => t('LBL141'),
                '#name' => 'esign-call-save',
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

        $saveAttributes['onclick'] = array();
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
              //'userActionStatus'=>'',
		      'keypress' => true,
		      'progress' => array(
		      'type' => 'throbber',
		      'message' => ''
		      ),
	    	)
	      );
    }else{
  	  $saveAttributes['onclick'] = array();
      //$saveAttributes['onclick'][] = '$("#'.$esignHiddenButt.'").click();';
      $form[$saveButtonId] = array(
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
      	  'userActionStatus'=>'',
  	      'keypress' => true,
  	      'progress' => array(
  	        'type' => 'throbber',
  	        'message' => ''
  	      ),
      	)
    );
  }


  	}
  $form['addedit-left-col-end'] = array(
    '#type' => 'markup',
    '#markup' => '</div><div class="clearBoth"></div>',
  );
  } catch (Exception $ex) {
  	watchdog_exception('locationAddEditFormWithCancelAndSaveClose', $ex);
  	expertusErrorThrow($ex);
  }
}


function location_display($arg) {
	try {
  $js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
  drupal_add_js(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_addedit_behaviours.js', $js_module_optional);

  $form_state = array(
   'no_redirect'  => TRUE, // To confirm whether necessary
   'build_info'   => array('args' => array('ajax')), // The form builder function expects the first argument to be ajax or nojs.
   'fromtooltip'     => 'Yes',
   'fromtooltipClassId'     => $arg,
  );

  $formId                 = 'exp_sp_administration_location_location_addedit_form';
  $builtForm              = drupal_build_form($formId, $form_state);
  $renderedTabContentHTML = drupal_render($builtForm);
  $drupalSettings         = getDrupalSettings();

  $commerceResponse = array(
    'drupal_settings'     => $drupalSettings,
    'render_content_main' => $renderedTabContentHTML,
  );
  echo drupal_json_output($commerceResponse);
  exit;
  } catch (Exception $ex) {
  	watchdog_exception('location_display', $ex);
  	expertusErrorThrow($ex);
  }
}

function addNewLocationByRestAPI( &$form_state,$createuserid="")
{
	try {
    expDebug::dPrint(' addNewLocationByRestAPI called');
	if((!empty($form_state['values']['latitude']) && !preg_match('/^[-]?(([0-8]?[0-9])\.(\d+))|(90(\.0+)?);[-]?((((1[0-7][0-9])|([0-9]?[0-9]))\.(\d+))|180(\.0+)?)$/',$form_state['values']['latitude'])) || (!empty($form_state['values']['longitude']) && !preg_match('/^[-]?(([0-8]?[0-9])\.(\d+))|(90(\.0+)?);[-]?((((1[0-7][0-9])|([0-9]?[0-9]))\.(\d+))|180(\.0+)?)$/',$form_state['values']['longitude']))){
       $errobj=new stdClass();
       $errobj->isValidateError = 1;
       $errobj->errormsg = 'Invalid latitude/longitude.';
       return $errobj;
	}
    drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	$form=array();
	$form["form_id"]="";
	$form_state['values']['name'] = stripslashes($form_state['values']['name']);
	$locationId =  _addLocationDetails($form,$form_state,$createuserid,1);
	expDebug::dPrint(' addNewLocationByRestAPI created location:' . print_r($locationId,true) , 4);
    return $locationId;
    } catch (Exception $ex) {
    	watchdog_exception('addNewLocationByRestAPI', $ex);
    	expertusErrorThrow($ex);
    }
}

function updateLocationByRestAPI(&$form_state,$createuserid="")
{
	try {
		$form_state['values'] = stripslashes_deep($form_state['values']); // Strip the slashes
		expDebug::dPrint('updateLocationByRestAPI values  :'.print_r($form_state['values'], true) , 5);
		$diff_array = array_diff_key($form_state['values'],$_REQUEST);
		expDebug::dPrint('updateLocationByRestAPI valuesfsfsdad  :'.print_r($diff_array, true) , 5);
		if(!empty($diff_array)){
			foreach($diff_array as $key=>$values){
				expDebug::dPrint('updateLocationByRestAPI values  :'.print_r($key, true) , 5);
				unset($form_state['values'][$key]);
			}
		}
		else{
			$form_state['values']= array_filter($form_state['values']);
			expDebug::dPrint('updateLocationByRestAPI valuesfsfs  :'.print_r($form_state['values'], true) , 5);
		}
    if((!empty($form_state['values']['latitude']) && !is_numeric($form_state['values']['latitude'])) || (!empty($form_state['values']['longitude']) && !is_numeric($form_state['values']['longitude']))){
       $errobj=new stdClass();
       $errobj->isValidateError = 1;
       $errobj->errormsg = 'Invalid latitude/longitude. Should be enter only in numeric values.';
       return $errobj;
	}
    drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	$form=array();
	$form["form_id"]="";
	$locationId = _updateLocationDetails($form,$form_state,$createuserid,1);
	expDebug::dPrint(' updateLocationByRestAPI location:' . print_r($locationId,true) , 4);
    return $locationId;
    } catch (Exception $ex) {
    	watchdog_exception('updateLocationByRestAPI', $ex);
    	expertusErrorThrow($ex);
    }
}

function locationNewSubmitAddEditFormWithCancelAndSaveClose(&$form, $wrapperId = 'addedit-form-wrapper', $saveButtonId = 'save-button', $cancelButtonId = 'cancel-button', $locationId, $subStage = '', $emptyId = '',$fromToolTip='',$fromToolTipClassId='') {
	try {
	global $theme_key;
  expDebug::dPrint(' locationNewSubmitAddEditFormWithCancelAndSaveClose() called.');

  $customHiddenButtonId = 'last-button-account';

  $esignHiddenButt = 'esign-'.$wrapperId;

  // Remove the temporary storage for API's to exchange information during form creation
  unset($form['addedit_tmp_storage']);

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
  $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  $cancelAttributes['data-wrapperid'] = array($wrapperId);

  // A common class for all addedit form cancel button, for styling to appear as a link.
  $cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';

  $cancelAttributes['tabindex'][] = '15';
  $prfixPgLeft = '';
  $suffixPgRight = '';
  if($theme_key == 'expertusoneV2'){
  	$prfixPgLeft   = '<div class="white-btn-bg-left"></div>';
  	$suffixPgRight = '<div class="white-btn-bg-right"></div>';
  	$cancelAttributes['class'][] = 'white-btn-bg-middle';
  }
  if($fromToolTip == 'Yes'){
    $popuprenderLocationId = "qtipAttachIdqtip_addlocation_visible_disp_".$fromToolTipClassId;
    $cancelAttributes['onclick'] = '$("#'.$popuprenderLocationId.'").closest(".qtip-active").hide(); $(\'#'.$popuprenderLocationId.'_disp\').html(\'\'); return false;';
    $cancelLabel  = t('LBL123'); // Close
    $form[$cancelButtonId] = array(
      '#type' => 'submit',
      '#prefix' => '<div class="addedit-form-cancel-and-save-actions-row">' .
                     '<div class="addedit-form-cancel-container-actions">' . $prfixPgLeft,
      '#suffix' => $suffixPgRight,
      '#value' => $cancelLabel,
      '#attributes' => $cancelAttributes
    );
  }else{
    $cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss(); return false;';
    $cancelLabel  = t('LBL123'); // Close
    $form[$cancelButtonId] = array(
      '#type' => 'submit',
      '#prefix' => '<div class="addedit-form-cancel-and-save-actions-row">' .
                     '<div class="addedit-form-cancel-container-actions">' . $prfixPgLeft,
      //'#suffix' =>   '<div class="admin-save-button-right-bg"></div>',
      '#suffix' => $suffixPgRight,
      '#value' => $cancelLabel,
      '#attributes' => $cancelAttributes
    );
  }

  // Add the Save button
  $ajaxCallbackSave = 'addEditRepaintForm';

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $saveAttributes['class'] = array('addedit-edit-' . $saveButtonId); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // For common styling of all addedit save buttons
  //$saveAttributes['class'][] = 'addedit-edit-save';

  // Using this class to applying bg color to the save button
  //$saveAttributes['class'][] = 'admin-save-button-middle-bg';
  $saveAttributes['class'][] = 'edit-catalog-course-save-publish';
  $saveAttributes['tabindex'][] = '13';


  //Increase the width of the popup while clicking on save button
  $saveAttributes['onclick'][] = 'increasePopWidth();';

  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $saveAttributes['class'][] = 'addedit-form-expertusone-throbber';

  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  $saveAttributes['data-wrapperid'] = array($wrapperId);
  $eSignatureAvailable = ($subStage == 'Attachment') ? false : true;


  if($subStage == 'details' || empty($subStage)) {
    $locationStatus     = '';
    if (!empty($locationId)) {
      $locationDetails   = _getLocationDetails($locationId);
      $locationStatus    =  $locationDetails['status'];
    }


    expDebug::dPrint('Testing the value22222'.print_r($locationDetails,true) , 4);
     expDebug::dPrint('Testing the value'.$locationDetails['status'] , 4);
   $isSaveParent = 0;
   $leftBg = '';
   if($theme_key == 'expertusoneV2'){
  	 $leftBg = '<div class="admin-save-button-left-bg"></div>';
   }
   if(empty($locationId) || $locationStatus != 'lrn_res_loc_atv'){
      $isSaveParent = 1;
      $saveAttributes['onclick'] = array();
      $saveAttributes['onclick'][] = 'increasePopWidth();';
      $form[$saveButtonId] = array(
	    '#type' => 'submit',
	    '#prefix' => '<div class="admin-save-pub-unpub-button-container">' . $leftBg,
	    '#suffix' => '<span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn ">&nbsp;</span></div>',
	    '#value' => t('LBL141'), // Save
      	'#name' => 'save',
	    '#attributes' => $saveAttributes,
        '#ajax' => array(
  	      'wrapper' => $wrapperId,
  	      'callback' => $ajaxCallbackSave,
          'customButtonId'=>$customHiddenButtonId,
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

   $resultCnt = 1; // by default, don't show delete button. show only when delete_object_record() below returns 0
   $suspendLoc = 1; // by default, don't show delete button. show only when getLocationSuspendAccess() below returns 0
   if($locationId){
      $resultCnt = delete_object_record($locationId,'location');
      $suspendLoc = getLocationSuspendAccess($locationId);
	 }

    if($isSaveParent){
      $form['addedit-button-group-ul-start'] = array(
        '#type' => 'markup',
        '#markup' => '<ul class="catalog-pub-add-list">',
      ); //  catalog-save-btn
      $savePublishPrefix = '<li class="save-pub-unpub-sub-menu">';
      $savePublishSuffix = '</li>';
    }else{
      $savePublishPrefix = '<div class="admin-save-pub-unpub-button-container">' . $leftBg;
      $savePublishSuffix = ((($resultCnt==0) || ($locationStatus == 'lrn_res_loc_atv') && $suspendLoc==0) ?
         ('<span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn ">&nbsp;</span>') :
           '<div class="admin-save-button-right-bg"></div>' ).'</div>';
    }
	  $saveButtonId = 'catalog-course-save-publish';
	  $savePublishAttributes['onclick'] = array();
	  $savePublishAttributes['tabindex'][] = '14';


    if($fromToolTip == 'Yes') {
    	$from_tooltip_location = 'Yes';
    }else{
    	$from_tooltip_location = 'No';
    }

      $savePublishAttributes['onclick'][] = 'increasePopWidth();';
     $buttonLabel = ($locationStatus == 'lrn_res_loc_atv') ? t('LBL141') : t('LBL641');
     $saveOnlyBtnCls = ($theme_key == 'expertusoneV2') ? 'admin-save-button-middle-bg' : 'pub-unpub-only-save-btn';
     $savePublishAttributes['class'][] = (($resultCnt==0) || ($locationStatus == 'lrn_res_loc_atv')) ?
                                                                      'edit-catalog-course-save-publish' : $saveOnlyBtnCls;
     $savePublishAttributes['class'][] = 'addedit-form-expertusone-throbber';
     $savePublishAttributes['data-wrapperid'] = array($wrapperId);
      $form[$saveButtonId] = array(
	    '#type' => 'submit',
	    '#prefix' => $savePublishPrefix,
	    '#suffix' => $savePublishSuffix,
	    '#value' => $buttonLabel,
      	'#name' => 'saveandpublish',
	    '#attributes' => $savePublishAttributes,
        '#ajax' => array(
  	      'wrapper' => $wrapperId,
  	      'callback' => $ajaxCallbackSave,
          'customButtonId'=>$customHiddenButtonId,
          'from_tooltip_location'=>$from_tooltip_location,
          'from_tooltip_location_classId'=>$fromToolTipClassId,
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

   if(!$isSaveParent){
      $form['addedit-button-group-ul-start'] = array(
        '#type' => 'markup',
        '#markup' => '<ul class="catalog-pub-add-list">',
      );
    }

	if(($locationId) && $locationStatus == 'lrn_res_loc_atv'){
  	    $saveButtonId = 'catalog-course-save-unpublish';
  	    $saveUnPublishAttributes['onclick'] = array();
        $saveUnPublishAttributes['onclick'][] = 'increasePopWidth();';
        $form[$saveButtonId] = array(
  	    '#type' => 'submit',
  	    '#prefix' => '<li class="save-pub-unpub-sub-menu">',
  	    '#suffix' => '</li>',
  	    '#value' => t('LBL572'),
        '#name' => 'unpublish',
  	    '#attributes' => $saveUnPublishAttributes,
        '#ajax' => array(
    	      'wrapper' => $wrapperId,
    	      'callback' => $ajaxCallbackSave,
              'customButtonId'=>$customHiddenButtonId,
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

    if($locationId ){
      if($resultCnt == 0){
        $displayTitle = titleController('ADMIN-RESOURCE-LOCATION-LOCATION-DETAILS', $locationDetails['title'],34);
  	   $saveButtonId = 'catalog-course-delete';

  	   $deleteAttributes['onclick'] = array();
        $deleteAttributes['onclick'][] = '$("#root-admin").data("narrowsearch").displayDeleteWizard("'.t('MSG357'). ' '.
                                            strtolower(t('Location')).' '.escape_string($locationDetails['name']). '","'.$locationId.'","location"); return false;';
        $deleteAttributes['class'][] = 'admin-delete-button-bg';
         $form[$saveButtonId] = array(
    	    '#type' => 'button',
    	    '#prefix' => '<li class="save-pub-unpub-sub-menu">',
    	    '#suffix' => '</li>',
    	    '#value' => t('LBL286'),
            '#name' => 'delete',
    	    '#attributes' => $deleteAttributes,
    	  );
      }
    }

	$form['addedit-button-group-ul-end'] = array(
      '#type' => 'markup',
      '#markup' => '</ul><div class="clearBoth"></div></div>' .
	             '<div class="clearBoth"></div></div>',
    );
  }
else{
   $saveAttributes['onclick'] = array();
  $saveAttributes['onclick'][] = 'Drupal.ajax.prototype.commands.addEditHideMessages();';
  $saveAttributes['onclick'][] = 'increasePopWidth();';
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
          'customButtonId'=>$customHiddenButtonId,
      'keypress' => true,
      'progress' => array(
        'type' => 'throbber',
        'message' => ''
      ),
  	  )
  );

}
  unset($saveAttributes['tabindex']);
  $saveAttributes['onclick'] = array();
  $saveAttributes['onclick'][] = 'Drupal.ajax.prototype.commands.addEditHideMessages();';
  $saveAttributes['onclick'][] = 'increasePopWidth();';
  $form[$customHiddenButtonId] = array(
	      '#type' => 'submit',
	      '#value' => t('hidden-save'),
	      '#name'	=> 'hidden-save',
      	  '#prefix' => '<span id = "esign-admin-div" style="display:none;">',
	      '#suffix' => '</span>',
	      '#id' =>$customHiddenButtonId,
  		  '#attributes' => $saveAttributes,
	      '#ajax' => array(
		      'wrapper' => $wrapperId,
		      'callback' => 'addEditRepaintForm',
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
  // Catalog control
   $entityId          	 = $locationId;//.',"'.$entityType
  if(!empty($emptyId)){
  	$entityId = 0;
  } else {
    $emptyId = 0;
  }
  $entityType        	 = 'cre_sys_obt_loc';
  $qtipIdInit        	 = $entityId.'_'.$entityType;
  $qtipOptAccessObj      = "{'entityId':".$entityId.",'entityType':'$entityType',
  'url':'administration/catalogaccess/".$entityId."/".$entityType."/".$emptyId."',
  'popupDispId':'qtip_visible_disp_".$qtipIdInit."','catalogVisibleId':'qtipAccessqtip_visible_disp_".$qtipIdInit."',
  'wid':650,'heg':'270','postype':'topleft','poslwid':'40','qdis':'ctool','linkid':'visible-user-".$entityId."'}";


  $pngPath = base_path().'sites/all/themes/core/expertusone/expertusone-internals/images/spacer.gif';

  $form['addedit-left-col-end'] = array(
    '#type' => 'markup',
    '#markup' => '</div><div class="clearBoth"></div>',
  );
  
  $form['addedit-left-col-extend-func'] = array(
  		'#type' => 'markup',
  		'#markup' => //$paintMultiAction,
  		'<div class="crs-tab-titles-container"><div class="access-tab-icon"></div>'.
  		"<div id='qtip_visible_disp_".$qtipIdInit."'><a id='visible-user-".$entityId."' onclick = \"callVisibility(".$qtipOptAccessObj.");\" class=\"tab-title\">".t('LBL642')."</a><span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div>"
  		//'</div>'
  		,
  
  );
  
  } catch (Exception $ex) {
  	watchdog_exception('locationNewSubmitAddEditFormWithCancelAndSaveClose', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Function for getting geo graphical positions.
 */
function getLatitudeLongitudeFromGoogleAPI($address){
try {
$address = urlencode($address);
//here is the google api url
$url = "http://maps.googleapis.com/maps/api/geocode/json?address=$address&sensor=false";
//get the content from the api using file_get_contents
$getmap = file_get_contents($url);
//the result is in json format. To decode it use json_decode
$googlemap = json_decode($getmap);
//get the latitute, longitude from the json result by doing a for loop

foreach($googlemap->results as $res){
  $address = $res->geometry;
  $latlng = $address->location;
  $formattedaddress = $res->formatted_address;
}

$geo = array('latitude' => $latlng->lat, 'longitude' => $latlng->lng);

return $geo;
} catch (Exception $ex) {
	watchdog_exception('getLatitudeLongitudeFromGoogleAPI', $ex);
	expertusErrorThrow($ex);
}
}
// latitude and longitude
 function getCountryValue($cntryCode) {
   try{
    $select = db_select('slt_country', 'country');
    $select->addField('country', 'country_name', 'country_name');
    $select->condition('country.country_code', $cntryCode, '=');
    $value  = $select->execute()->fetchField();
    expDebug::dPrintDBAPI('getCountryValue Qry ', $select);
    expDebug::dPrint('getCountryValue Value = ' . print_r($value, true), 5);
    //return $entityId;
    return $value;
    }catch (Exception $ex) {
      watchdog_exception('getCountryValue', $ex);
      expertusErrorThrow($ex);
    }
 }
 // country exists or not in slt country table
function getCountry($cntryCode) {
   try{
    $select = db_select('slt_country', 'country');
    $select->addField('country', 'country_name', 'country_name');
	  $select->addExpression('COUNT(country.id)', 'cnt1');	
	 $select->condition('country.country_code', $cntryCode, '=');
    $value  = $select->execute()->fetchAll();
    expDebug::dPrintDBAPI('getCountry Qry ', $select);
    expDebug::dPrint('getCountry Value = ' . print_r($value, true), 5);
    //return $entityId;
    return $value;
    }catch (Exception $ex) {
      watchdog_exception('getCountryValue', $ex);
      expertusErrorThrow($ex);
    }
 }
// state exists or not in the country given
  function getStateValue($cntryCode,$statCode) {
   try{
    $select = db_select('slt_state', 'state');
    $select->addField('state', 'state_name', 'state_name');
  $select->addExpression('COUNT(state.id)', 'cnt');	
    $select->condition('state.country_code', $cntryCode, '=');
    $select->condition('state.state_code', $statCode, '=');
    //$value  = $select->execute()->fetchField();
    $value  = $select->execute()->fetchAll();
    expDebug::dPrintDBAPI('getStateValue Qry ', $select);
    expDebug::dPrint('getStateValue Value = ' . print_r($value, true), 5);
    //return $entityId;
    return $value;
    }catch (Exception $ex) {
      watchdog_exception('getStateValue', $ex);
      expertusErrorThrow($ex);
    }
 }
   // state exists or not in slt state table
  function getState($statCode) {
   try{
    $select = db_select('slt_state', 'state');
    $select->addField('state', 'state_name', 'state_name');
    $select->condition('state.state_code', $statCode, '=');
    //$value  = $select->execute()->fetchField();
    $value  = $select->execute()->fetchField();
    expDebug::dPrintDBAPI('getState Qry ', $select);
    expDebug::dPrint('getState Value = ' . print_r($value, true), 5);
    //return $entityId;
    return $value;
    }catch (Exception $ex) {
      watchdog_exception('getStateValue', $ex);
      expertusErrorThrow($ex);
    }
 }
  function getStatecountrycheck($cntryCode) {
   try{
    $select = db_select('slt_state', 'state');
  $select->addExpression('COUNT(state.id)', 'cnt');	
    $select->condition('state.country_code', $cntryCode, '=');
    $value  = $select->execute()->fetchField();
    expDebug::dPrintDBAPI('getStatecountrycheck Qry ', $select);
    expDebug::dPrint('getStatecountrycheck Value = ' . print_r($value, true), 5);
    return $value;
    }catch (Exception $ex) {
      watchdog_exception('getStateValue', $ex);
      expertusErrorThrow($ex);
    }
 }
 function getLocationdetailsByRestAPI($locname, $userid="")
 {
 	try {
 		expDebug::dPrint("sdasdasdsadad");
 		$form=array();
 		$form["form_id"]="";
 		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
 		$select = db_select('slt_location', 'location');
 		$select->addField('location', 'id', 'id');
 		$select->addField('location', 'name', 'name');
 		$select->condition('location.name','%' . db_like($locname) . '%', 'LIKE');
 		$select->range(0,1);
 		$locationDetail  = $select->execute()->fetchAll();
 		return $locationDetail;
 	} catch (Exception $ex) {
 		watchdog_exception('getLocationdetailsByRestAPI', $ex);
 		expertusErrorThrow($ex);
 	}
 }

?>