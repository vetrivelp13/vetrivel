<?php
require_once DRUPAL_ROOT . '/sites/all/services/Encryption.php';

function getTimeZoneDet() {
	try{
	$select = db_select('slt_profile_list_items', 'spfa');
    // Join the slt_node_learning_activity table.
    $select->join('slt_profile_list_items','spfb','spfa.parent_id = %alias.id');
    $select->addField('spfa', 'code','zone_code');
    $select->addField('spfa', 'name','zone_name');
    $select->condition('spfb.code', 'cre_sys_tmz');
	return $select->execute()->fetchAll();
	}catch (Exception $ex) {
		watchdog_exception('getTimeZoneDet', $ex);
		expertusErrorThrow($ex);
	}
}

function getLocation(){
try{
    $select = db_select('slt_location', 'loc');
	$select->addField('loc','id','loc_id');
    $select->addField('loc','name','loc_name');
    $select->condition('loc.status', 'lrn_res_loc_atv');
    $select->orderBy('loc.name');
	return $select->execute()->fetchAll();
	}catch (Exception $ex) {
		watchdog_exception('getLocation', $ex);
		expertusErrorThrow($ex);
	}
}

function getJobTitle(){
try{
    $select = db_select('slt_profile_list_items', 'spfa');
    $select->join('slt_profile_list_items','spfb','spfa.parent_id = %alias.id');
    $select->addField('spfa', 'code','job_code');
    $select->addField('spfa', 'name','job_name');
    $select->condition('spfb.code', 'cre_usr_jtl');
    $select->condition('spfa.is_active', 'Y');	//50763: Registration Page
    filterByUserAccessPrivileges($select, 'spfa','',1,'cre_usr_jtl','','spfa');	//50763: Registration Page
	return $select->execute()->fetchAll();
	}catch (Exception $ex) {
		watchdog_exception('getJobTitle', $ex);
		expertusErrorThrow($ex);
	}
}

function getJobRole(){
	try{
		$select = db_select('slt_profile_list_items', 'spfa');
		$select->join('slt_profile_list_items','spfb','spfa.parent_id = %alias.id');
		$select->addField('spfa', 'code','jobr_code');
		$select->addField('spfa', 'name','jobr_name');
		$select->condition('spfb.code', 'cre_usr_jrl');
		$select->condition('spfb.is_active', 'Y');
		return $select->execute()->fetchAll();
		}catch (Exception $ex) {
			watchdog_exception('getJobRole', $ex);
			expertusErrorThrow($ex);
		}
}

function getCountry($cond=''){
	try{
		$select = db_select('slt_country', 'sc');
		$select->addField('sc', 'country_code','cnt_code');
		$select->addField('sc', 'country_name','cnt_name');
                if(!empty($cond)){
                    $select->condition('sc.country_code',$cond, '=');
                }
		return $select->execute()->fetchAll();
	}catch (Exception $ex) {
		watchdog_exception('getCountry', $ex);
		expertusErrorThrow($ex);
	}
}

function getState($value1){
	try{
	$select = db_select('slt_state', 'sta');
	$select->addField('sta','state_code','sta_code');
	$select->addField('sta','state_name','sta_name');
	$select->condition('sta.country_code', $value1);
	return json_encode($select->execute()->fetchAll());
	}catch (Exception $ex) {
	watchdog_exception('getState', $ex);
	expertusErrorThrow($ex);
}
}

function getStateDef($cond=''){
	try{
                $regexp = '[a-zA-Z]';
		$select = db_select('slt_state', 'sta');
		$select->addField('sta','state_code','sta_code');
		$select->addField('sta','state_name','sta_name');
                if($cond != '' && $cond == 'US'){ // this condition only added to get the state code which USPS supports
                    $select->condition('sta.country_code',$cond,'=');
                    $select->condition('sta.state_code',$regexp , 'REGEXP');
                }else if($cond != ''){
                    $select->condition('sta.country_code',$cond,'=');
                }else{
                    //No conditions required
                }
		return $select->execute()->fetchAll();

	}catch (Exception $ex) {
		watchdog_exception('getState', $ex);
		expertusErrorThrow($ex);
	}
}
function getOrg(){
	try{
		$select = db_select('slt_organization', 'org');
		$select->addField('org', 'id','org_id');
		$select->addField('org', 'name','org_name');
		$select->condition('org.status', 'cre_org_sts_act');
		return $select->execute()->fetchAll();
	}catch (Exception $ex) {
		watchdog_exception('getOrg', $ex);
		expertusErrorThrow($ex);
	}
}

function getEmptype(){
	try{
		$select = db_select('slt_profile_list_items','spfa');
		$select->join('slt_profile_list_items','spfb','spfa.parent_id = %alias.id');
		$select->addField('spfa','code','emp_code');
		$select->addField('spfa','name','emp_name');
		$select->condition('spfa.is_active','Y');
		$select->condition('spfb.code','cre_usr_etp');

		return $select->execute()->fetchAll();
		}catch (Exception $ex) {
			watchdog_exception('getEmptype', $ex);
			expertusErrorThrow($ex);
		}
}

function getDept(){
	try{
		$select = db_select('slt_profile_list_items','spfa');
		$select->join('slt_profile_list_items','spfb','spfa.parent_id = %alias.id');
		$select->addField('spfa','code','dept_code');
		$select->addField('spfa','name','dept_name');
		$select->condition('spfa.is_active','Y');
		$select->condition('spfb.code','cre_usr_dpt');

		return $select->execute()->fetchAll();
	}catch (Exception $ex) {
		watchdog_exception('getDept', $ex);
		expertusErrorThrow($ex);
	}
}

function getUsrtype(){
	try{
		$select = db_select('slt_profile_list_items','spfa');
		$select->join('slt_profile_list_items','spfb','spfa.parent_id = %alias.id');
		$select->addField('spfa','code','usrtype_code');
		$select->addField('spfa','name','usrtype_name');
		$select->condition('spfa.is_active','Y');
		$select->condition('spfb.code','cre_usr_ptp');
		return $select->execute()->fetchAll();
	}catch (Exception $ex) {
		watchdog_exception('getUsrtype', $ex);
		expertusErrorThrow($ex);
	}
}

function getLang(){
	try{
		$select = db_select('slt_profile_list_items', 'spfa');
		$select->join('slt_profile_list_items','spfb','spfa.parent_id = %alias.id');
		$select->addField('spfa', 'code','lang_code');
		$select->addField('spfa', 'name','lang_name');
		$select->condition('spfb.code', 'cre_sys_lng');
		$select->condition('spfa.is_active','Y');
		return $select->execute()->fetchAll();
	}catch (Exception $ex) {
		watchdog_exception('getLang', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * Added by Simbu Sep 25, 2015
 */
function getCurrency(){
	try{
		$select = db_select('slt_profile_list_items', 'spfa');
		$select->addField('spfa', 'name','currency_name');
		$select->addField('spfa', 'attr1','currency_code');
		$select->addField('spfa', 'attr2','currency_symbol');
		$select->Condition('spfa.code','cre_sys_crn_%','LIKE');
		$select->condition('spfa.attr3', 'Y');
		$select->condition('spfa.is_active','Y');
		return $select->execute()->fetchAll();
	}catch (Exception $ex) {
		watchdog_exception('getLang', $ex);
		expertusErrorThrow($ex);
	}
}

function generateCaptcha(){
	try{
	global $base_url;
 			$length=6;
		    $_rand_src = array(
		        array(48,57) //digits
		        , array(97,122) //lowercase chars
		//        , array(65,90) //uppercase chars
		    );
		    srand ((double) microtime() * 1000000);
		    $random_string = "";
		    for($i=0;$i<$length;$i++){
		        $i1=rand(0,sizeof($_rand_src)-1);
		        $random_string .= chr(rand($_rand_src[$i1][0],$_rand_src[$i1][1]));
		    }
		    $enc = new Encrypt();
			$random_string = $enc->encrypt($random_string);

        	$captcha_url = $base_url.'/sites/all/services/Captcha.php?captcha='.$random_string;
			$res_captcha = array('url' => $captcha_url,'num' => $random_string);

			return $res_captcha;
			}catch (Exception $ex) {
				watchdog_exception('generateCaptcha', $ex);
				expertusErrorThrow($ex);
			}
 }

 function getEncryptedCaptcha($entered){
 	try{
		 	$enc = new Encrypt();
			$entered=$enc->encrypt($entered);
			return $entered;
			}catch (Exception $ex) {
				watchdog_exception('getEncryptedCaptcha', $ex);
				expertusErrorThrow($ex);
			}
 }

 function insertUserInfo($val_obj,&$form_state){
 	try {
		   global $user;
		   $select = db_select('slt_location', 'loc');
		   $select->addField('loc','name','loc_name');
		   $select->condition('loc.id', $val_obj['ploc_val']);
		   $loc_name = $select->execute()->fetchAll();

           $language = $_SESSION['language'];
           if(isset($language)){
		     $selLanguage = db_select('slt_profile_list_items', 'prf');
		     $selLanguage->addField('prf','code','lang_code');
		     $selLanguage->condition('prf.attr1', $language);
		     $selLanguage->condition('prf.code', db_like('cre_sys_lng_') . '%', 'LIKE');
		     $userLanguage = $selLanguage->execute()->fetchField();
           }

           if ((variable_get('registration_Language') == '1') && (!empty($form_state['values']['lang'])))
           {
           	$userLanguage=$form_state['values']['lang'];
           }

           else{

           $siteLmsDefaultLanguage = getSiteDefaultLanguageCode();
           $userLanguage = ($userLanguage) ? $userLanguage : $siteLmsDefaultLanguage;
           $language     = ($language) ? $language : $GLOBALS['language']->language;
           }
           if(empty($val_obj['mname_val'])){
           	$full_name = $val_obj['fname_val'].' '.$val_obj['lname_val'];
           }
           else{
           	$full_name = $val_obj['fname_val'].' '.$val_obj['mname_val'].' '.$val_obj['lname_val'];
           }
		   $insert_query = db_insert('slt_person')
		        ->fields(array(
                        'first_name' => $val_obj['fname_val'],
                        'middle_name' => $val_obj['mname_val'],
                        'last_name' => $val_obj['lname_val'],
                        'full_name' => $full_name,
                        'user_name' => $val_obj['uname_val'],
                        'phone_no' => $val_obj['phone_val'],
                        'status'	=>	'cre_usr_sts_atv',
                        'job_title'	=>	$val_obj['jtitle_val'],
                        'email' => $val_obj['email_val'],
                        'is_rehire' => 'N',
                        'is_instructor' => 'N',
                        'is_manager' => 'N',
                        'addr1' => $val_obj['addr1_val'],
                        'addr2' => $val_obj['addr2_val'],
                        'city' => $val_obj['city_val'],
                        'state' => $val_obj['state_val'],
                        'country' => $val_obj['country_val'],
                        'time_zone' => $val_obj['tzone_val'],
                        'preferred_loc_id' => $val_obj['ploc_val'],
                        'preferred_currency' => $val_obj['currency_val'],
                        'location_name' => $loc_name[0]->loc_name,
                        'custom2' => $val_obj['sub_val'],
                        'preferred_language' => $userLanguage,
                        'created_on' =>now(),
                        'updated_on' =>now(),
                        'mobile_no' =>$val_obj['mobile_val'],
                        'user_type' =>$val_obj['usrtype_val'],
                        'employment_type' =>$val_obj['emptype_val'],
                        'employee_no' =>$val_obj['empid_val'],
                        'org_id'     =>(empty($val_obj['org_val'])) ? null : $val_obj['org_val'],
                        'zip' =>$val_obj['zip_val'],
                        'dept_code' =>$val_obj['dept_val'],
                        'is_verified_add' =>$val_obj['is_verified_add'],
                        'is_valid_add'=>$val_obj['is_valid_add'],
                        'invalid_add_reason'=>$val_obj['invalid_add_reason']

		   ));


       $last_insertid = $insert_query->execute();

       expDebug::dPrint('User Details : '.print_r($insert_query , true), 4);
       updateGroupMappingDetails($last_insertid, 'cre_usr', $last_insertid,'emptyadd',0);
       
       if(!empty($last_insertid)){
		$updateStmt = db_update('slt_person');
        $updateStmt->condition('id', $last_insertid);
		$updateStmt->fields(array(
		'created_by' => $last_insertid,
		'updated_by' => $last_insertid
		));
		$updateStmt->execute();

		}

		   $multi_jobrole=array();
		   $multi_jobrole=explode(',',$form_state['values']['load_multiselect_jobrole']);

			foreach ($multi_jobrole as $job_role_code)
			{
				if(!empty($job_role_code)){
						$usrJobRole  = db_insert('slt_person_jobrole_mapping');
				$usrJobRole->fields(array(
						'user_id' => $last_insertid,
						'job_role' =>$job_role_code,
						'created_by' => $last_insertid,
						'created_on' => now(),
				));
					$usrJobRole->execute();
		             }
			}




		// Timezone is missing when User Register, Sample addNewUser()
		if(!empty($val_obj['tzone_val'])){
			$preferredShortTimeCode = getProfileListItemByCode($val_obj['tzone_val']);
			if($preferredShortTimeCode){
				$timezone  = $preferredShortTimeCode[0]->attr2;
			}
		}else{ // it should not be Executed Because timezone is mandatory in user Registration, if it is not mandatory it will take care of the Default Timezone
			$defaultTimezone = defaultSiteTimeZone();
			$timezone  = $defaultTimezone['sysTimezone'];

		}
		// To get the language from the form and update the same in drupal users table
		if(!empty($val_obj['lang_val'])){
			$preferredShortLangCode = getProfileListItemByCode($val_obj['lang_val']);
			if($preferredShortLangCode){
				$language = $preferredShortLangCode[0]->attr1;
			}
		}else{
			$language = $GLOBALS['language']->language;
		}

		expDebug::dPrint('$timezone  = ' . print_r($timezone,true),3);
	     $details = array(
			        'name' => $val_obj['uname_val'],
			        'pass' => $val_obj['pass_val'],
			        'mail' => $val_obj['email_val'],
	     			'timezone' => $timezone,
	                'language' => $language,
			        'access' => 1,
			        'status' => 1
		  );

		  // Added by Vincent on 24 Jan, 2017 for #0071187 
		  // #0071187 fix starts
		  $newGrpList = getGroupsDetails($newUserId,'',1,'',0);
	      foreach($newGrpList as $key => $value)
	      {
	      	if(isset($value->name))
	      	{
	      		$newGrpListarr[] = $value->id;
	      	}
	      }
	      $delUserGrp = 'Delete'; // While populat flat table the existing entry should be delete once/very first call
	      if(count($newGrpListarr) > 0){
		    	foreach($newGrpListarr as $grpId){
		    		updatesGrpList($drupalUserId, $grpId,$newUserId,$loggedInUserId);
		    		populateGroupUsersInFlatTable($grpId,$newUserId,$delUserGrp); // For Admin groups
		    		$delUserGrp = '';
		    	}
		    }

      		populateGroupUsersInFlatTable(null,$newUserId,$delUserGrp); // For learner groups
      		// #0071187 fix end
      		
			//$account = user_save(null, $details);
			expDebug::dPrint('$details : '.print_r($details , true), 4);
			$account = user_save(drupal_anonymous_user(), $details);
	    	sendNewUserNotification($last_insertid, $last_insertid); // The second argument, which is supposed to be new_user creator's slt_person table
	                                                             // user id, is currently ignored in slp_lnr_notification_ins() MySQL stored procedure.
	                                                             // This should be learner's slt_person id (again $last_insertid), and not drupal uid
	                                                             // of the learner.


	     	    make_audit_trail_entries($val_obj, $last_insertid , null ,$last_insertid, 'Inserted','cre_sys_obt_usr','exp_sp_userregister','New User Registration',
	    		array('fname_val', 'lname_val', 'uname_val','email_val','addr1_val', 'addr2_val', 'country_val', 'state_val', 'city_val', 'zip_val' ,'phone_val','mobile_val','usrtype_val','empid_val','org_val','dept_val','jtitle_val','emptype_val','tzone_val','lang_val','jobrole_val'),$last_insertid);



	    /*0031138: Minor issue when using Force pwd*/
	    $drupalUid = getDrupalIdOfUser($last_insertid);
	    $updateStmt = db_update('password_policy_force_change');
	    $updateStmt->condition('uid', $drupalUid);
	    $updateStmt->fields(array(
	    		'force_change' => 0,
	    ));
	    $updateStmt->execute();


	    // Log user in.
		  $form_state['uid'] = $account->uid;
		  user_login_submit(array(), $form_state);
		  }catch (Exception $ex) {
		  	watchdog_exception('insertUserInfo', $ex);
		  	expertusErrorThrow($ex);
		  }
 }

 /*
  * addMultiselectDropDownToRegForm - Use this function to add a multiselect dropdown (select) to the Registration form
  */
 function addMultiselectDropDownToRegForm(&$form, $id, $title = '', $options = array(), $defaultOption = array(), $tabIndex = null, $mandatory = false, $readOnly = false,$attributes = array(),  $customDropdown = array()) {
 	try {
 		$attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

 		// We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
 		// each time the form is re-built.
 		$attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

 		// Add class to apply common style to all drop-downs
 		$attributes['class'][] = 'addedit-edit-dropdown';

 		if ($readOnly) {
 			$attributes['class'][] = 'addedit-readonly-dropdown';
 		}

 		if (isset($tabIndex) && $tabIndex > 0) {
 			$attributes['tabindex'] = $tabIndex;
 		}

 		if ($mandatory) {
 			$nullOption[] = t('LBL674');
 		} else {
 			$nullOption[] = t('LBL674');
 		}
 		//$finalOptions = array_merge($nullOption, $options);
 		//$finalOptions = $nullOption + $options;
 		$finalOptions = $options;
 		foreach($finalOptions as $key => $value){
 			$finalOptions[$key] = t($value);
 		}
 		$prefix = '';
 		$suffix = '';
 		if (empty($form['addedit_tmp_storage']['two_column'])) {
 			$prefix = addEditGetPrefix('FIELD', $title, $mandatory);
 			$suffix = addEditGetSuffix('FIELD');
 		}
 		else {
 			if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
 				$prefix = addEditGetPrefix('TWOCOL_FIRSTCOL_FIELD', $title, $mandatory);
 				$suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD');
 				$form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
 			} else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
 				$prefix = addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $title, $mandatory);
 				$suffix = addEditGetSuffix('TWOCOL_SECONDCOL_FIELD');
 				$form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
 			} else {
 				throw new Exception('exp_sp_administration.inc : addDropDownToAddEditForm() : Attempt to add a third field to a two column row.');
 			}
 		}

 		//registeruser

 		$selectedCount = count($defaultOption);
 		$selectedText = $selectedCount > 0 && is_array($defaultOption) ? $selectedCount.' selected' : t('LBL674');
 		expDebug::dPrint('selected '.$selectedText, 4);
 		expDebug::dPrint('defaultOption '.print_r($defaultOption,true), 4);

 		$prefix .= '<div id="menu_'.$id.'" onClick="$(\'#ajax-userregister\').data(\'registeruser\').callMultiSelect(\''. $id . '\');" class="ui-multiselect multiselect-custom-dropdown"><span class="dropdown-arrow ui-icon  ui-icon-triangle-2-n-s"></span><span class="selected-list-text">' . $selectedText . '</span>';
 		$prefix .= '</div><div id="avil_' . $id .'" class="multiselect-custom-dropdown-results" style="display:none"></div>';
 		$prefix .= '<img style="display:none" src=\'themes/seven/images/buttons.png\' width="0" height="0" onload="$(\'#ajax-userregister\').data(\'registeruser\').updateSelectedText(\''.$id.'\');"/>';

 		$form['load_multiselect_'.$id] = array(
 				'#type' => 'hidden',
 				'#prefix' => $prefix,
 				'#suffix' => $suffix,
 				'#default_value' => implode(',', $defaultOption),
 				'#attributes' => array('id' => 'load_multiselect_'.$id)
 		);




 	} catch (Exception $ex) {
 		watchdog_exception('addMultiselectDropDownToRegForm', $ex);
 		expertusErrorThrow($ex);
 	}
 }
?>