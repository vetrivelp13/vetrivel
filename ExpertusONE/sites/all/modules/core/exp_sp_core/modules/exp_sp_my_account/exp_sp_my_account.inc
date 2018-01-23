<?php

function getMyAccount($usid) {
    try{
	$select = db_select('users', 'drupal_users');
	$select->join('slt_person', 'person','drupal_users.name = %alias.user_name');
	$select->leftJoin('slt_profile_list_items', 'master_view', 'person.time_zone = %alias.code');
	$select->leftJoin('slt_person', 'mgr', 'person.manager_id = %alias.id');
	$select->leftJoin('slt_organization', 'org', 'person.org_id = %alias.id');
	$select->leftJoin('slt_country', 'country', 'org.country = %alias.country_code');
	$select->leftJoin('slt_profile_list_items', 'my_job', 'person.job_title = %alias.code');
	$select->leftJoin('slt_location', 'my_location', 'person.preferred_loc_id = %alias.id');

	$select->addField('person','id', 'person_id');
	$select->addField('person','first_name', 'person_first_name');
	$select->addField('person','middle_name', 'person_middle_name');
	$select->addField('person','last_name', 'person_last_name');
	$select->addField('person','full_name', 'person_full_name');
	$select->addField('person','user_name', 'person_user_name');
	$select->addField('person','location_name', 'person_location_name');
	$select->addField('person','email', 'person_email');
	$select->addField('person','manager_id', 'person_manager_id');
	$select->addField('person','phone_no', 'person_phone_no');
	$select->addField('person','mobile_no', 'person_mobile_no');
	$select->addField('person','sms_alert', 'person_sms_alert');
	$select->addField('person','status', 'person_status');
	$select->addField('person','addr1', 'person_addr1');
	$select->addField('person','addr2', 'person_addr2');
	$select->addField('person','city', 'person_city');
	$select->addField('person','state', 'person_state');
	$select->addField('person','country', 'person_country');
	$select->addField('person','zip', 'person_zip');
	$select->addField('person','user_type', 'person_user_type');
	$select->addField('person','employment_type', 'person_employment_type');
	$select->addField('person','job_title', 'person_job_title');
	$select->addField('person','org_id', 'person_org_id');
	$select->addField('person','custom2', 'Subcripe');
	$select->addField('person','dept_code', 'person_dept_code');
	$select->addField('person','preferred_loc_id', 'person_pref_loc');
	$select->addField('person','preferred_currency', 'person_pref_currency'); // simbu
	$select->addField('person','time_zone', 'person_time_zone');
	$select->addField('person','is_instructor', 'person_is_instructor');
	$select->addField('person','webex_name', 'person_webex_name');
	$select->addField('person','webex_pass', 'person_webex_pass');

	$select->addField('org','id', 'orgid');
	$select->addField('org','number', 'orgnumber');
	$select->addField('org','name', 'orgname');
	$select->addField('org','parent_org_name', 'orgparent_org_name');
	$select->addField('org','type', 'orgtype');
	$select->addField('org','status', 'Orgstats');
	$select->addField('org','add1', 'orgadd1');
	$select->addField('org','add2', 'orgadd2');
	$select->addField('org','city', 'orgcity');
	$select->addField('org','state', 'orgstate');
	$select->addField('org','zip', 'org_zip');
	$select->addField('org','phone_no', 'org_phone_no');
	$select->addField('org','fax_no', 'org_fax_no');
	$select->addField('org','website', 'org_website');
	$select->addField('org','country', 'orgcountrycode');
	$select->addField('country','country_name', 'my_country');
	$select->addField('my_job','name', 'my_job_title');
	$select->addField('my_location','name', 'my_location');

	$select->addField('master_view','code', 'person_code');
	$select->addField('master_view','name', 'person_timezone_name');

	$select->addField('drupal_users','uid', 'drupal_uid');
	$select->addField('drupal_users','picture', 'person_avatar');

	$select->condition('drupal_users.uid', $usid);
	return $select->execute()->fetchObject();
  }catch (Exception $ex) {
    watchdog_exception('getMyAccount', $ex);
    expertusErrorThrow($ex);
  }
}

function updateAccount($updateRec,$uid,$edit_address='') {
  $txn = db_transaction();
  try{
	$custom0 = LdapFetchUSerDetails();
  	$customfield = $custom0[0]->custom0;
    if($custom0[0]->first_name != $updateRec['FirstName']){
  		if(strpos($customfield,'firstname') === false){
		 $customfield .= ($customfield) ? ',firstname' : 'firstname';
  		}
  	}
  	if($custom0[0]->last_name != $updateRec['LastName']){
  		if(strpos($customfield,'lastname') === false){
		 $customfield .= ($customfield) ? ',lastname' : 'lastname';
  		}
  	}

   //Start # Added by Velu #0030384
    $mobile_no = (!empty($updateRec['MobileNo']) && $updateRec['MobileNo'] != t('LBL1233')) ? $updateRec['MobileNo'] : '';
  	$sms_alert_check = (!empty($updateRec['MobileNo']) && $updateRec['MobileNo'] != t('LBL1233') && $updateRec['SmsAlertCheck']) ? 'Y' : 'N' ;
  	$updateRec['SmsAlertCheck'] = $sms_alert_check;
   //End # Added by Velu #0030384

	$personUpd = db_update('slt_person');
        if($edit_address == 1){
	$personUpd->fields(
	array(
            'first_name'  => trim($updateRec['FirstName']),
            'last_name'	=> trim($updateRec['LastName']),
            'middle_name' => trim($updateRec['MiddleName']),
            'full_name'	=> trim($updateRec['FirstName'].' '.$updateRec['MiddleName'].' '.$updateRec['LastName']),
            'email'		=> trim($updateRec['Email']),
            'mobile_no'   => $mobile_no, //Added by Velu #0030384
            'sms_alert'   => $sms_alert_check, //Added by Velu #0030384
            'custom0' 	=> $customfield,
            'addr1'   => trim($updateRec['addr1']),
            'addr2'   => trim($updateRec['addr2']),
            'city'   => trim($updateRec['city']),
            'zip'   => trim($updateRec['zipcode']),
            'state'   => $updateRec['state'],
            'country'   => $updateRec['country'],
            'is_verified_add' => $updateRec['is_verified_add'],
            'is_valid_add' => $updateRec['is_valid_add'],
            'invalid_add_reason' => $updateRec['invalid_add_reason'],
            )
	);
        }else{
            $personUpd->fields(
            array(
            'first_name'  => trim($updateRec['FirstName']),
            'last_name'	=> trim($updateRec['LastName']),
            'middle_name' => trim($updateRec['MiddleName']),
            'full_name'	=> trim($updateRec['FirstName'].' '.$updateRec['MiddleName'].' '.$updateRec['LastName']),
            'email'		=> trim($updateRec['Email']),
            'mobile_no'   => $mobile_no, //Added by Velu #0030384
            'sms_alert'   => $sms_alert_check, //Added by Velu #0030384
            'custom0' 	=> $customfield,
                )
            );
        }
	$personUpd->condition('user_name',$updateRec['UserName']);
	$personUpd->execute();

	expDebug::dPrintDBAPI(' updateAccount Velu = ' ,$personUpd);

	$usrInputArr = array('mail' => trim($updateRec['Email']));
	if(!empty($updateRec['Photo'])) {
		$pictArr     = array('picture' => $updateRec['Photo']);
		$usrInputArr = array_merge($usrInputArr,$pictArr);
	}
	$password  = $updateRec['Password'];
	if(!empty($password)) {
		$passArr     = array('pass' => $password);
		$usrInputArr = array_merge($usrInputArr, $passArr);
	}

	$usrUpd = db_update('users');
	$usrUpd->fields($usrInputArr);
	$usrUpd->condition('uid',$uid);
	$usrUpd->execute();

	/* Add the new password to the password policy hitory table */
  updatePasswordPolicyHistory($uid, $password);
  make_myaccount_audit_trail_entries($updateRec, $uid, $custom0);
  }catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('updateAccount', $ex);
    expertusErrorThrow($ex);
  }
  // Commit the transaction
  unset($txn);
}

function getPrefix($fldName,$type,$mandatory){
  try{
	$required  = '';
	if((strtoupper($mandatory) == 'TRUE') && ($mandatory != 'undefined'))
	{
		$required = "<span class='require-text'>*</span>";
	}
	switch (strtoupper($type))
	{
		case 'PREFIX':
                    if($fldName == t('LBL054')){
			return '<table cellspacing="2" cellpadding="2" class="username-row my-account-box my-account-box-cont-cls"><tr><td class="field-title">'.$fldName.':'.$required.'</td><td class="field-value"><div class="expertus-dropdown-bg">
				    <div class="expertus-dropdown-icon ">';
                    }else if($fldName == t('LBL152') || $fldName == t('LBL039')){
                        return '<table cellspacing="2" cellpadding="2" class="state-cntry-row my-account-box my-account-box-cont-cls"><tr><td class="field-title">'.$fldName.':'.$required.'</td><td class="field-value"><div class="expertus-dropdown-bg">
				    <div class="expertus-dropdown-icon ">';
                    }else{
                        return '<table cellspacing="2" cellpadding="2" class="my-account-box my-account-box-cont-cls"><tr><td class="field-title">'.$fldName.':'.$required.'</td><td class="field-value"><div class="expertus-dropdown-bg">
				    <div class="expertus-dropdown-icon ">';
                    }
			break;
		case 'TITLE':
			return '<table cellspacing="2" cellpadding="2" class="my-account-box"><thead><tr><td class="my-account-title"><span class="my-account-title-text">'.$fldName.'</span></td></tr></thead></table>';
			break;
		case 'ACTION' :
			return '<table cellspacing="2" cellpadding="2" class="my-account-box my-account-box-act-cls"><thead><tr><td class="my-account-title"><span class="my-account-title-text">'.$fldName.'</span><span class="action-button">';
			break;
		case 'ACTION_SAVE' :
			return '<table cellspacing="2" cellpadding="2" class="my-account-box my-account-action-box"><thead><tr><td class="my-account-title"><span class="my-account-title-text">'.$fldName.'</span><span class="action-button">';
			break;
	}
  }catch (Exception $ex) {
    watchdog_exception('getPrefix', $ex);
    expertusErrorThrow($ex);
  }
}

function getSuffix($type){
  try{
	switch (strtoupper($type))
	{
		case 'SUFFIX':
			return '</div></div></td></tr></table>';
			break;
		case 'ACTION':
			return '</span></td></tr></table>';
			break;
	}
  }catch (Exception $ex) {
    watchdog_exception('getSuffix', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * User Account Information
 *
 */

function getMyAccountDetails($usid) {
  try{
	$select = db_select('users', 'drupal_users');
	$select->join('slt_person', 'person','drupal_users.name = %alias.user_name');
	$select->addField('person','id', 'person_id');
	$select->addField('person','first_name', 'person_first_name');
	$select->addField('person','middle_name', 'person_middle_name');
	$select->addField('person','last_name', 'person_last_name');
	$select->addField('person','full_name', 'person_full_name');
	$select->addField('person','user_name', 'person_user_name');
	$select->addField('person','location_name', 'person_location_name');
	$select->addField('person','email', 'person_email');
	$select->addField('person','manager_id', 'person_manager_id');
	$select->addField('person','phone_no', 'person_phone_no');
	$select->addField('person','mobile_no', 'person_mobile_no');
	$select->addField('person','sms_alert', 'person_sms_alert');
	$select->addField('person','status', 'person_status');
	$select->addField('person','addr1', 'person_addr1');
	$select->addField('person','addr2', 'person_addr1');
	$select->addField('person','city', 'person_city');
	$select->addField('person','state', 'person_state');
	$select->addField('person','country', 'person_country');
	$select->addField('person','zip', 'person_zip');
	$select->addField('person','user_type', 'person_user_type');
	$select->addField('person','employment_type', 'person_employment_type');
	$select->addField('person','job_title', 'person_job_title');
	$select->addField('person','org_id', 'person_org_id');
	$select->addField('person','custom2', 'Subcripe');
	$select->addField('person','dept_code', 'person_dept_code');
	$select->addField('person','preferred_loc_id', 'person_pref_loc');
	$select->addField('person','preferred_currency', 'person_pref_currency'); // simbu
	$select->addField('person','time_zone', 'person_time_zone');
	$select->addField('person','is_instructor', 'person_is_instructor');
	$select->addField('person','webex_name', 'person_webex_name');
	$select->addField('person','webex_pass', 'person_webex_pass');

	$select->condition('drupal_users.uid', $usid);

	$result[0]  = $select->execute()->fetchAll();
	$account    = user_load($usid,TRUE);
	$result[1]  = '';
	if(!empty($account->picture)){
		$filepath  = $account->picture->uri;
		$result[1] = $filepath;
	}
	return $result;
  }catch (Exception $ex) {
    watchdog_exception('getMyAccountDetails', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * Using this function to update user information in slt_person table
 *
 */
/*function updateProfile($resultRecord, $user_id, $defaultValues){
  $txn = db_transaction();
  try{
  	
  	expDebug::dPrint('Beofre Sesion updateProfile called to check ' . $_SESSION['user_preferred_currency_code']);
  $orgName = TRIM($resultRecord['company_name']);
  $orgid   = db_select('slt_organization', 'org');
  $orgid->addField('org','id','org_id');
  $orgid->condition('org.name', $orgName);
  $orgId = $orgid->execute()->fetchField();

  if(!$orgId){
    $maxorgid = db_select('slt_organization', 'org');
    $maxorgid->addExpression('max(id)','max_no');
    $OrgMaxId    = $maxorgid->execute()->fetchField();
    $orgnumber   = $OrgMaxId +1;
    $padded_num  = str_pad($orgnumber, 10, 'X', STR_PAD_LEFT);
    $insertorg   = db_insert('slt_organization');
    $insertorg->fields(array(
        'name'		 =>     $orgName,
        'type'       => 	'cre_org_typ_ext',
        'status'     => 	'cre_org_sts_act',
        'number'     =>     $padded_num,
    ));
    $orgId = $insertorg ->execute();

    // Checking and Updating Taxonomy vocabulary terms
    $orgVocab = db_select('taxonomy_vocabulary','v');
    $orgVocab->addField('v','vid','vid');
    $orgVocab->condition('machine_name','organization_nav_vocabulary','=');
    $orgVocabId = $orgVocab->execute()->fetchField();

    $allOrg = db_select('taxonomy_term_data','a');
    $allOrg->addField('a','tid','tid');
    $allOrg->condition('name','Any','=');
    $allOrgId = $allOrg->execute()->fetchField();
    $terms               = new stdClass;
    $terms->name         = ($orgName.'-'.$padded_num);
    $terms->vid          = $orgVocabId;
    $terms->description  = $orgId;
    $terms->parent       = $allOrgId;
    taxonomy_term_save($terms);
  }

	$custom0 = LdapFetchUSerDetails();
  	$customfield = $custom0[0]->custom0;
    if($custom0[0]->tzone != $resultRecord['timezone']){
  		if(strpos($customfield,'timezone') === false){
		 $customfield .= ($customfield) ? ',timezone' : 'timezone';
  		}
  	}

  $update = db_update('slt_person');
  $update->fields(array(
      'addr1'               =>     TRIM($resultRecord['add1']),
      'addr2'               =>     TRIM($resultRecord['add2']),
      'city'                =>     TRIM($resultRecord['city']),
      'state'               =>     TRIM($resultRecord['state']),
      'country'             =>     TRIM($resultRecord['country']),
      'zip'                 =>     TRIM($resultRecord['zip']),
      'phone_no'            =>     TRIM($resultRecord['phone_no']),
      //   'job_title'           =>     $resultRecord['jobtitle'],
  	  'updated_on'          =>     now(),
  	  'custom0' 			=>	   $customfield
  ));

  $update->condition('id', $user_id);
  $update->execute();

  make_mypreference_audit_trail_entries($resultRecord, $user_id, $defaultValues,
                                           array('add1', 'add2', 'city', 'state', 'country', 'zip', 'phone_no'));
  }catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('updateProfile', $ex);
    expertusErrorThrow($ex);
  }
  // Commit the transaction
  unset($txn);
} */

/*
 * Using this function to update user information in slt_person table
 *
 */
function updateAccountPreference($resultRecord, $user_id, $defaultValues){

  $txn = db_transaction();
  try{
  global $user;
  $sub_val = 'N';
  if($resultRecord['sub_check']){
    $sub_val = 'Y';
  }else{
    $sub_val = 'N';
  }
  $resultRecord['sub_check'] = $sub_val;
  $update = db_update('slt_person');
  $update->fields(array(
      'preferred_loc_id'    =>     ($resultRecord['preflocation'])? $resultRecord['preflocation'] : NULL,
  	  'preferred_currency'  =>     ($resultRecord['prefcurrency'])? $resultRecord['prefcurrency'] : NULL, // Added by Simbu Sep 25, 2015
      'preferred_language'  =>     $resultRecord['preflanguage'],
      'time_zone'           =>     $resultRecord['timezone'],
      'custom2'             =>     $sub_val,
  	  'updated_on'          =>     now()
  ));

  // Added by Vincent for performance imporvement on Sep 2, 2015
  if(isset($_SESSION['Pref_Language']) && !empty($resultRecord['preflanguage'])){
  	$_SESSION['Pref_Language'] = $resultRecord['preflanguage'];
  	unset($_SESSION['user_timezone']);
  }
  if(isset($_SESSION['user_timezone']) && !empty($resultRecord['timezone'])){
  	unset($_SESSION['user_timezone']);
  }
  // Added by Simbu Sep 25, 2015
  expDebug::dPrint('Beofre Sesion ' . $_SESSION['user_preferred_currency_code']);
  expDebug::dPrint('Beofre Sesion SYM ' . $_SESSION['user_preferred_currency_sym']);
  if(isset($_SESSION['user_preferred_currency_code']) && !empty($resultRecord['prefcurrency'])){
  	$_SESSION['user_preferred_currency_code'] = $resultRecord['prefcurrency'];
  	$prefcurrencysymbol = getMyAccountCurrency($resultRecord['prefcurrency']);
  	$_SESSION['user_preferred_currency_sym'] = $prefcurrencysymbol[0]->currency_symbol;
  }else{
  	$getDefaultCurrency = getDefaultCurrency();
  	$_SESSION['user_preferred_currency_code'] = $getDefaultCurrency['preferred_currency'];
  	$_SESSION['user_preferred_currency_sym']  = $getDefaultCurrency['attr2'];
  }
  if($resultRecord['timezone'] != $defaultValues['timezone'] ) {
  	$uid = getIdOfLoggedInUser();
  	if($user_id == $uid && isset($_SESSION['person_detail']) && count($_SESSION['person_detail'])){
  		$profile_list_item_value = getProfileListItemByCode($resultRecord['timezone']);
  		if(isset($profile_list_item_value[0])){
  			$_SESSION['person_detail']['time_zone'] = $resultRecord['timezone'];
  			$_SESSION['person_detail']['attr2'] = $profile_list_item_value[0]->attr2;
  			$_SESSION['person_detail']['attr1'] = $profile_list_item_value[0]->attr1;
  			$_SESSION['person_detail']['attr4'] = $profile_list_item_value[0]->attr4;
  		}
  	}
  
  }
  expDebug::dPrint('Afetr Sesion ' . $_SESSION['user_preferred_currency_code']);
  expDebug::dPrint('Afetr Sesion SYM ' . $_SESSION['user_preferred_currency_sym']);

  $update->condition('id', $user_id);
  $update->execute();

  $prefLangName = '';
  $prefTimeZone = '';
  if(!empty($resultRecord['preflanguage'])){
    $prefLang = db_select('slt_profile_list_items','prof');
    $prefLang->addField('prof','attr1','attr1');
    $prefLang->condition('code',$resultRecord['preflanguage']);
    $prefLangName = $prefLang->execute()->fetchField();
    
    // Added by Vincent on 24 Jan, 2017 for #0071187
    // If end user changes his language the groups needs to be reset based on the 
    // current users selection.
    // #0071187 fix starts
    include_once(drupal_get_path('module', 'exp_sp_administration_user') .'/exp_sp_administration_user.inc');
    $exroleList = getExistingGrpList($user_id);
	$newGrpList = getGroupsDetails($user_id,'',1,'',0);
	$ExistingRoleList = array();
	$newGrpListarr = array();
	$drupalUserId = getDrupalIdOfUser($entityId);
	$loggedInUserId = getIdOfLoggedInUser();
	foreach($exroleList as $key => $value){
		if(isset($value->name)){
	  		$ExistingRoleList[] = $value->id;
	  	}
	}
	foreach($newGrpList as $key => $value){
	 	if(isset($value->name)){
	 	 			$newGrpListarr[] = $value->id;
	 	}
 	}
	$delUserGrp = 'Delete'; // While populat flat table the existing entry should be delete once/very first call
	foreach($newGrpListarr as $grpId){
	    if(($keyval = array_search($grpId, $ExistingRoleList)) !== false) {
	    	unset($ExistingRoleList[$keyval]);
		}else{
			updatesGrpList($drupalUserId, $grpId,$user_id,$loggedInUserId);
		}
		populateGroupUsersInFlatTable($grpId,$user_id,$delUserGrp,FALSE);
		$delUserGrp = '';
	}
	if(!empty($ExistingRoleList)){
	    foreach($ExistingRoleList as $grpId){
			deleteGrpList($drupalUserId,$grpId,$user_id);
	    }
	}
	populateGroupUsersInFlatTable(null,$user_id,$delUserGrp,FALSE);
	// #0071187 fix end
	
  }
  if(!empty($resultRecord['timezone'])){
    $prefTime = db_select('slt_profile_list_items','tzone');
    $prefTime->addField('tzone','attr2','attr2');
    $prefTime->condition('code',$resultRecord['timezone']);
    $prefTimeZone = $prefTime->execute()->fetchField();
  }
  if(!empty($prefLangName) || !empty($prefLangName)){
    $account = user_load($user->uid, TRUE);
    $accoutArray = array();
    $accoutArray['language'] = $prefLangName;
    $accoutArray['timezone'] = $prefTimeZone;
    $account = user_save($account, $accoutArray);
  }
  if($resultRecord['preflanguage'] != $defaultValues['preflanguage']){
  	unset($_SESSION['parent_array']);
  	unset($_SESSION['BasePath']);
  	cache_set('program_filters');
  	//cache_set('catalog_filters');
  }
  make_mypreference_audit_trail_entries($resultRecord, $user_id, $defaultValues,
                                         array('preflocation','prefcurrency','preflanguage', 'timezone','sub_check'));
                                         		
  syncSolrData('User','','Bulk');
  }catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('updatePreferenceProfile', $ex);
    expertusErrorThrow($ex);
  }
  // Commit the transaction
  unset($txn);
}

function getMyAccountProfileDetails($usid) {
  try{
  $select = db_select('users', 'drupal_users');
  $select->join('slt_person', 'person','drupal_users.name = %alias.user_name');
  $select->leftJoin('slt_profile_list_items', 'master_view', 'person.time_zone = %alias.code');
  $select->leftJoin('slt_person', 'mgr', 'person.manager_id = %alias.id');
  $select->leftJoin('slt_location', 'my_location', 'person.preferred_loc_id = %alias.id');

  $select->addField('person','id', 'person_id');
  $select->addField('person','preferred_loc_id', 'person_pref_loc');
  $select->addField('person','preferred_currency', 'person_pref_currency'); // simbu
  $select->addField('person','location_name', 'person_location_name');
  $select->addField('person','time_zone', 'person_time_zone');
  $select->addField('person','preferred_language', 'person_pref_language');
  $select->addField('person','custom2', 'Subcripe');
  $select->addField('person','is_instructor', 'person_instructor');
  $select->addField('person','webex_name', 'person_webex_name');
  $select->addField('person','webex_pass', 'person_webex_pass');
  $select->addField('my_location','name', 'my_location');
  $select->addField('master_view','code', 'person_code');
  $select->addField('master_view','name', 'person_timezone_name');

  $select->addField('drupal_users','uid', 'drupal_uid');

  $select->condition('drupal_users.uid', $usid);
  $result = array();
  $result = $select->execute()->fetchObject();

  expDebug::dPrint('getOrgState : $getOrgState obj = ' . print_r($result, true) , 3);

  return $select->execute()->fetchObject();
  }catch (Exception $ex) {
    watchdog_exception('getMyAccountProfileDetails', $ex);
    expertusErrorThrow($ex);
  }
}



/*
 * Get slt_country table information
 */
function getMyAccountLocation(){
 try{
  $select = db_select('slt_location', 'loc');
  $select->addField('loc','id','location_id');
  $select->addField('loc','name','location_name');
  $select->condition('loc.status', 'lrn_res_loc_atv');
  $select->orderBy('loc.name');
  $location = $select->execute()->fetchAll();
  return $location;
   }catch (Exception $ex) {
    watchdog_exception('getLocation', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * Get list of completed orders
 */
function getMyAccountOrdersDetails_New($usid,$countrequired=false) {
  try{

  $select = db_select('uc_orders', 'ucorders');
  $select->Join('slt_order', 'orders', 'orders.uc_order_id = ucorders.order_id');
  $select->leftJoin('uc_order_line_items', 'ordersitem', 'ordersitem.order_id = ucorders.order_id');
  if($countrequired==true){
    $select->addExpression('(count(1))', 'count');
  } else {
    $select->addField('ucorders','order_id','order_id');
    $select->addField('ucorders','order_status','order_status');
    $select->addField('ucorders','order_total','order_total');
    $select->addField('ucorders','product_count','product_count');
    $select->addField('orders','created_on','purchase_date');
    $select->addField('ordersitem','amount','discount_amount');
    $select->addField('orders','currency_type','currency_type');

  }
  $select->condition('ucorders.order_status', array('completed','payment_received'),'IN');

  $select->condition('ucorders.uid',$usid);
  if($countrequired==true){
    $ordersDetails = $select->execute()->fetchField();
  }else{
    $select->range($start, $limit);
    $ordersDetails = $select;
    //$selectEnrollTp->range($start, $limit);
  }
  expDebug::dPrintDBAPI("Db_PrintAPI", $select);

  expDebug::dPrint('getMyAccountOrdersDetails : $getOrgState obj = ' . print_r($ordersDetails, true) , 3);
  expDebug::dPrint('  LIST $result = ' . print_r($ordersDetails, true) , 3);
  return $ordersDetails;
  }catch (Exception $ex) {
    watchdog_exception('getMyAccountOrdersDetails', $ex);
    expertusErrorThrow($ex);
  }
}
function getMyAccountOrdersDetails($usid,$countrequired=false) {
  try{
  $select = db_select('slt_order', 'orders');
  $select->LeftJoin('slt_profile_list_items', 'spli', 'spli.attr1=orders.currency_type AND spli.code like \'cre_sys_crn_%\'');
  $select->LeftJoin('slt_order_items', 'orditem', 'orditem.order_id=orders.id');
  $select->LeftJoin('slt_enrollment', 'enrollment', 'enrollment.course_id=orditem.course_id and enrollment.class_id=orditem.class_id and enrollment.user_id=orditem.user_id and enrollment.order_id=orditem.order_id');
  $select->LeftJoin('uc_orders', 'ucorder', 'ucorder.order_id =orders.uc_order_id and ucorder.order_status in (\'completed\',\'payment_received\',\'pending\',\'canceled\') ');
  $select->leftJoin('uc_order_line_items', 'ordersitemtax', 'ordersitemtax.order_id =ucorder.order_id  and ordersitemtax.type = \'cybersource_tax\'');
  //$select->leftJoin('uc_order_line_items', 'ordersitemdis', 'ordersitemdis.order_id =ucorder.order_id and ordersitemdis.type = \'uc_discounts\'');
  $select->addField('orders','id','order_id');
  //$select->addField('ucorder','order_id','order_id');
  $select->addField('orders','uc_order_id','uc_order_id');
  $select->addField('ucorder','order_status','order_status');
  $select->addField('ucorder','order_total','order_total');
  $select->addField('ucorder','product_count','product_count');
  $select->addField('orders','created_on','purchase_date');
  $select->addField('ordersitemtax','amount','tax_amount');
  $select->addField('spli','attr2','attr2');
  $select->addField('orders','currency_type','currency_type');
  // Modified for multiple discounts with same price.
  $select->addExpression('(select sum(ordersitemdis.amount) from uc_order_line_items ordersitemdis where ordersitemdis.order_id= ucorder.order_id and ordersitemdis.type = \'uc_discounts\')','discount_amount');
  //$select->addExpression('sum(orditem.item_price)','total_amount');
  $select->addExpression('(select sum(ucordprod.price) from uc_order_products ucordprod where ucordprod.order_id= ucorder.order_id)','total_amount');
  //$select->condition('ucorders.order_status', array('completed','payment_received'),'IN');
  $select->condition('enrollment.reg_status', array('lrn_crs_reg_rsv','lrn_crs_reg_rsc'),'NOT IN');
  $select->condition('orders.user_id',$usid);
  $select->GroupBy('orders.id');
  
  if($_REQUEST['apiname'] && $_REQUEST['searchTxt']){
  	$select->leftJoin('uc_order_products', 'uop', 'uop.order_id =ucorder.order_id');
	$select->condition(db_or()->Condition('uop.title','%'.TRIM($_REQUEST['searchTxt']).'%','LIKE')
							  ->Condition('ucorder.order_id','%'.TRIM($_REQUEST['searchTxt']).'%','LIKE'));
  }
  
  if($countrequired==true){
    $ordersDetails = count($select->execute()->fetchAll());
  }else{
    $select->range($start, $limit);
    $ordersDetails = $select;
    //$selectEnrollTp->range($start, $limit);
  }
  expDebug::dPrintDBAPI("Db_PrintAPI", $select);

  expDebug::dPrint('getMyAccountOrdersDetails : $getOrgState obj = ' . print_r($ordersDetails, true) , 3);
  expDebug::dPrint('  LIST $result = ' . print_r($ordersDetails, true) , 3);
  return $ordersDetails;
  }catch (Exception $ex) {
    watchdog_exception('getMyAccountOrdersDetails', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * In the datagrid get total completed orders
 */
function getOrderResultForGrid(){
  try{
   expDebug::dPrint('getOrderResultForGrid');
  	
    global $user;
    $page      = $_GET['page'];
    $limit     = $_GET['rows'];
    $sidx      = $_GET['sidx'];
    $sord      = $_GET['sord'];
    
    if($_REQUEST['apiname'] ){
    	$page      = $_REQUEST['page'];
    	$limit     = $_REQUEST['rows'];
    	$sidx      = $_REQUEST['sidx'];
    	$sord      = $_REQUEST['sord'];
    expDebug::dPrint('getOrderResultForGrid'.$page.$limit.$sidx.$sord);
    }
    
    
    $userid     = getSltpersonUserId();
    $count      = getMyAccountOrdersDetails($userid,true);
    /* Paging Related */
    $start = $limit * $page - $limit;
    $listquery  = getMyAccountOrdersDetails($userid,false);
    $xorderBy = ($sidx != '') ? $sidx : 'uc_order_id';
    $listquery->orderBy($xorderBy,$sord);

    $listquery->range($start, $limit);
    // Execute query
    expDebug::dPrintDBAPI("Db_PrintAPI", $listquery);
    $queryResult = $listquery->execute()->fetchAll();
    //print_r($queryResult);
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

    $srch_result_response_orders->total   = $total_pages;
    $srch_result_response_orders->records = $count;
    $srch_result_response_orders->page    = $page;
    $srch_result_response_orders->unique_id    = $userid.'-enrollments';
    $_SESSION['jq_count'] = array();
    $i=0;
    if($page > 1){$incre=($page*10-10);$k=1+$incre;}else{$k=1;}
    foreach ($queryResult as $rec_search_results) {
      if (!empty($rec_search_results->uc_order_id)) {
      	$order_id_url = "user/".$user->uid."/orders/".$rec_search_results->uc_order_id."/print";
        $viewinvoice="<a href='#' onclick='window.open(\"?q=".$order_id_url."\",\"printinvoice\",\"width=1050,height=900, toolbar=yes, location=yes, directories=yes, status=yes, menubar=yes,scrollbars=yes, copyhistory=yes, resizable=no\")'>".t(LBL826)."</a>";
      }else{
        $viewinvoice="<a href='#' class='grey-out' >".t(LBL826)."</a>";
      }
      $discount_amount = str_replace('-','',$rec_search_results->discount_amount);
      $discount_field_value=   $discount_amount==NUll ? number_format(0, 2, '.', '') :number_format($discount_amount, 2, '.', '');
      $price_field_value=   ($rec_search_results->total_amount)==NUll ? number_format(0, 2, '.', '') :number_format($rec_search_results->total_amount, 2, '.', '');
      $tax_field_value=   ($rec_search_results->tax_amount)==NUll ? number_format(0, 2, '.', '') :number_format($rec_search_results->tax_amount, 2, '.', '');
      $srch_result_response_orders->rows[$i]['id']   = $rec_search_results->order_id;

       $ord_curr_symbol = $rec_search_results->currency_type;
      $ord_curr_sym = $rec_search_results->attr2;
      if(empty($ord_curr_symbol)){
      	$ord_curr_symbol = 'USD';
      	$ord_curr_sym = ''; // 0056924 - default $ removal
      }
      $srch_result_response_orders->rows[$i]['cell'] = array(
      	 'uc_order_id' 	       => ($rec_search_results->uc_order_id) ? $rec_search_results->uc_order_id : '-',
      	 'product_count'           => $rec_search_results->product_count== NUll ? 1 : $rec_search_results->product_count,
      	 'purchase_date' 	   => date('M, d Y', strtotime($rec_search_results->purchase_date)),
      	 'Price' 	       =>  $ord_curr_sym.'&nbsp'.$price_field_value.'&nbsp;&nbsp;'.$ord_curr_symbol,
         'Discount'        => $ord_curr_sym.'&nbsp'.$discount_field_value.'&nbsp;&nbsp;'.$ord_curr_symbol, 
         'Tax'             => $ord_curr_sym.'&nbsp'.$tax_field_value.'&nbsp;&nbsp;'.$ord_curr_symbol,
         'order_total' 	       =>  $ord_curr_sym.'&nbsp'.number_format($rec_search_results->order_total, 2, '.', '').'&nbsp;&nbsp;'.$ord_curr_symbol,
         'Invoice' 	       => $viewinvoice,
      );
      if($_REQUEST['apiname']){
      	if($rec_search_results->uc_order_id){
      		$select = db_select('uc_order_products', 'ucp');
      		$select->addField('ucp','title','title');
      		$select->addField('ucp','model','model');
      		$select->addField('ucp','cost','cost');
      		$select->addField('ucp','price','price');
      		$select->addField('ucp','currency_type','currency_type');
      		$select->condition('ucp.order_id',$rec_search_results->uc_order_id);
      		$products = $select->execute()->fetchAll();
      		$srch_result_response_orders->rows[$i]['cell']['products'] = $products;
      	}
      	
      }
      $i++; $k++;
    }
    
    expDebug::dPrint('getOrderResultForGrid'.print_r($srch_result_response_orders,true));
    if($_REQUEST['apiname'] ){
    	$result->orders = $srch_result_response_orders;
    	return $result;
    }
    
    
    print drupal_json_encode($srch_result_response_orders);
  }catch (Exception $ex) {
    watchdog_exception('getOrderResultForGrid', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * make_myaccount_audit_trail_entries() - This function checks for changed values in my account form and makes audit trail entries
 */
function make_myaccount_audit_trail_entries($resultRecord, $userId, $defaultValues) {
  try{
	global $user;

	$formFieldsList = array('FirstName', 'MiddleName', 'LastName', 'Email', 'MobileNo','SmsAlertCheck');
  foreach ($formFieldsList as $formField) {
    expDebug::dPrint(' $formField = ' . print_r($formField, true) , 4);
    expDebug::dPrint(' newVal = ' . print_r($resultRecord[$formField], true) , 4);
    expDebug::dPrint(' oldVal = ' . print_r($defaultValues[$formField], true) , 4);
    if ($resultRecord[$formField] != $defaultValues[$formField]) {
      $insertId = add_audit_trail_entry($userId, $userId, 'cre_sys_obt_usr', 'exp_sp_my_account', 'myaccount', 'Updated ' . $formField,
                                         $userId, $defaultValues[$formField], $resultRecord[$formField],null,null,null,null,$activity_id);
    if($activity_id==null){
		 	$activity_id = $insertId;
		 }
    }
  }

  if (!empty($resultRecord['Password'])) {
    $account = user_load($user->uid, TRUE); // Get the latest values
    if (!empty($resultRecord['Password'])) {
      $insertId = add_audit_trail_entry($userId, $userId, 'cre_sys_obt_usr', 'exp_sp_my_account', 'passwordupdated', 'Changed password', $userId, $oldPassword, $account->pass);
    if($activity_id==null){
		 	$activity_id = $insertId;
		 }
    }

    /*if ($updatedPhoto) {
    	$photoMsg = 'Changed picture';
    	if (empty($oldPhotoFileId)) {
    	  $photoMsg = 'Added picture';
    	}
      add_audit_trail_entry($userId, $userId, 'cre_sys_obt_usr', 'exp_sp_my_account', 'My Account', $photoMsg, null, $oldPhotoFileId, $account->picture->fid);
    }*/
  }
  }catch (Exception $ex) {
    watchdog_exception('make_myaccount_audit_trail_entries', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * make_mypreference_audit_trail_entries() - This function checks for changed values and makes audit trail entries
 */
function make_mypreference_audit_trail_entries($resultRecord, $userId, $defaultValues, $formFieldsList = array()) {
	try{
	foreach ($formFieldsList as $formField) {
		expDebug::dPrint(' $formField = ' . print_r($formField, true) , 4);
		expDebug::dPrint(' newVal = ' . print_r(TRIM($resultRecord[$formField]), true) , 4);
    expDebug::dPrint(' oldVal = ' . print_r($defaultValues[$formField], true) , 4);
		if (TRIM($resultRecord[$formField]) != $defaultValues[$formField]) {
		  $insertId=add_audit_trail_entry($userId, $userId, 'cre_sys_obt_usr', 'exp_sp_my_profile', 'myprofilepersonal', 'Updated ' . $formField,
		                                     $userId, $defaultValues[$formField], TRIM($resultRecord[$formField]),null,null,null,null,$activity_id);
	  }
	if($activity_id==null){
		 	$activity_id = $insertId;
		 }
	}
  }catch (Exception $ex) {
    watchdog_exception('make_mypreference_audit_trail_entries', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * Get slt_profile_list_items table attr1,attr2 (Condition if attr3 value is Y)
 * Added By Simbu Sep 25, 2015
*/
function getMyAccountCurrency($code=NULL){
	try{
		expDebug::dPrint(' $getMyAccountCurrency = ' . $code);
		$select = db_select('slt_profile_list_items', 'spli');
		$select->addField('spli','name','currency_name');
		$select->addField('spli','attr1','currency_code');
		$select->addField('spli','attr2','currency_symbol');
		$select->Condition('spli.code','cre_sys_crn_%','LIKE');
		$select->condition('spli.attr3', 'Y');
		$select->condition('spli.is_active', 'Y');
		if($code!=''){
			expDebug::dPrint(' $getMyAccountCurrency Yes= ' . $code);
			$select->condition('spli.attr1', $code);
		}
		$select->orderBy('spli.attr1');
		$currency = $select->execute()->fetchAll();
		return $currency;
	}catch (Exception $ex) {
		watchdog_exception('getMyAccountCurrency', $ex);
		expertusErrorThrow($ex);
	}
}

/*
* Update the webex username and password in slt_person
* Added by Indhuja Apr 19,2016
*/
function updateMyWebexdetails($updateRec,$userid,$defaultValues){
	
	$txn = db_transaction();
	try{
		$personUpd = db_update('slt_person');
		$personUpd->fields(
				array(
						'webex_name'  => trim($updateRec['webex_name']),
						'webex_pass'  => $updateRec['webex_pass']
				)
		);
		$personUpd->condition('id',$userid);
		$personUpd->execute();
	
		expDebug::dPrintDBAPI(' updateMyWebexdetails = ' ,$personUpd,5);
		make_mymeeting_audit_trail_entries($updateRec, $userid, $defaultValues,
				array('webex_name', 'webex_pass'));
		
		
	}catch (Exception $ex) {
		$txn->rollback();
		unset($txn);
		watchdog_exception('updateMyWebexdetails', $ex);
		expertusErrorThrow($ex);
	}
	// Commit the transaction
	unset($txn);
}
/*
 * make_mymeeting_audit_trail_entries() - This function checks for changed values and makes audit trail entries
* Added by Indhuja Apr 19,2016
*/
function make_mymeeting_audit_trail_entries($resultRecord, $userId, $defaultValues, $formFieldsList = array()) {
	try{
		foreach ($formFieldsList as $formField) {
			if ($resultRecord[$formField] != $defaultValues[$formField]) {
		  $insertId=add_audit_trail_entry($userId, $userId, 'cre_sys_obt_usr', 'exp_sp_my_meeting', 'mymeetingpersonal', 'Updated ' . $formField,
		  		$userId, $defaultValues[$formField], $resultRecord[$formField],null,null,null,null,$activity_id);
	  }
	  if($activity_id==null){
		 	$activity_id = $insertId;
		 }
		}
	}catch (Exception $ex) {
		watchdog_exception('make_mymeeting_audit_trail_entries', $ex);
		expertusErrorThrow($ex);
	}
}

function getInvoice(){
	include_once $_SERVER["DOCUMENT_ROOT"].'/sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc';
	
	$orderid = $_REQUEST['order_id'];
	
	$select = db_select('uc_orders', 'ord');
	$select->addField('ord','order_status','order_status');
	$select->condition('ord.order_id', $orderid);
	expDebug::dPrintDBAPI(' getInvoice ' ,$select);
	
	$order_status = $select->execute()->fetchField();
	$query = db_query("SELECT slf_get_paymentstatus_name('".$order_status."','name')");
	$status = $query->fetchField();
	expDebug::dPrint('$status=====' . print_r($status, true) , 4);

	$order_string ='%|order_id>|'.$orderid.'~|%';
	$order_status_string ='%|order_status>|'.$status.'~|%';
	
	$select = db_select('slt_notification', 'noti');
	$select->addField('noti','token_str','token_str');
	$select->addField('snf','notification_template','notification_template');
	$select->leftJoin('slt_notification_info', 'sni', 'sni.notification_code = noti.msg_id');
	$select->leftJoin('slt_notification_frame', 'snf', 'snf.notification_id =sni.id');
	$select->Condition('noti.token_str',$order_string,'LIKE');
	$select->Condition('noti.token_str',$order_status_string,'LIKE');
	$select->condition('snf.lang_code', 'cre_sys_lng_eng');
	
	expDebug::dPrintDBAPI(' getInvoice ' ,$select);
	$result  = $select->execute()->fetchAssoc();
	$results->invoicehtml = base64_encode(generateNotificationDetailsFromToken($result['token_str'],$result['notification_template']));
	expDebug::dPrint('$result ======' . print_r($results->invoicehtml, true) , 4);
	return $results;
	
}

?>