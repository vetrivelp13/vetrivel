<?php 
//ini_set('display_errors', 'On');
//error_reporting(E_ALL);
 
$ssoauthenticated=FALSE;
$relayState=NULL;

function receiveSAMLResponse($callFrom=''){
  module_load_include('inc','saml','./incl/saml_utils');
	module_load_include('php','saml','./incl/xmlseclibs');
	$samlResponse = '';
	global $ssoauthenticated,$relayState;
	$ssoauthenticated=FALSE;
	$relayState=NULL;
	try{
		if(!e1_sso_login_accessible()) 
		{
			throw new Exception('Unsolicited SAML Request .....');
			exit;

		}
		
		if (array_key_exists('SAMLResponse', $_POST)) {
						
			$samlResponse = $_POST['SAMLResponse'];
			
			if(empty($samlResponse)){

				throw new Exception('Invalid SAML Response.');
			}
			$samlResponse = base64_decode($samlResponse);
			expDebug::dPrint($samlResponse,4);

		} else if (array_key_exists('SAMLResponse', $_GET)) {

			$samlResponse = $_GET['SAMLResponse'];
			if(empty($samlResponse)){

				throw new Exception('Invalid SAML Response.');
			}
			$samlResponse = base64_decode($samlResponse);
			$samlResponse  = gzinflate($samlResponse);
			expDebug::dPrint($samlResponse,4);
						
		}else
		{
		throw new Exception('Missing SAMLResponse parameter.');
		}
		if(empty($samlResponse)){

			throw new Exception('Invalid SAML Response.');
		}
		// Check for Relay State where the URL has to be redirected
		if(array_key_exists('RelayState', $_REQUEST) && isset($_REQUEST['RelayState'])){
		
			$relayState=trim($_REQUEST['RelayState']);
		}		
		$_SESSION['ses_saml_response']=$samlResponse; /* Added to hold the saml response in session variable */
		processSAMLResponse($samlResponse,$callFrom);
	}catch(Exception $e){
		expDebug::dPrint(" Exception while processing SAML Response ".$e->getMessage(),1);
		expDebug::dPrint($e,1);
		
		SAMLDebug($samlResponse,$e);
		
		$msgStr = t('ERR225');//fix for Ticket #0049658
		$msg = $msgStr;
		
		if($callFrom=='mobile'){
			expDebug::dPrint("Redirect to Mobile ERROR.....",1);
			$redirectUrl = 'x-expertusone-oauth://callback?sso=true&error=true&errormsg='.urlencode(t('ERR225'));
		?>
		  <script language='javascript'>
					window.location = "<?php print $redirectUrl;?>";
		  </script>
		<?php
		}else{
			print theme('error_page', array('content' =>$msg));          
		}	
		exit;
	}
}

/* Added new function to trigger the notification to admin or some other user, whom specified in SAML settings  */

function SAMLDebug($samlResponse,$e,$from_process='add'){
	
	 $saml_debug = variable_get('saml_debug_textbox');
	 expDebug::dPrint('saml_debug_texbox = '.$saml_debug,5);
	 
	 if($saml_debug!=''){ //To check debug is in disabled status
	 	expDebug::dPrint('SAML Debugging is ON',5);
	 	
	 	expDebug::dPrint('$saml response before apply HTML Special Char = '.print_r($samlResponse,true),5);
	 	$saml_response = htmlspecialchars($samlResponse);
	 	expDebug::dPrint('$saml response After apply HTML Special Char = '.print_r($samlResponse,true),5);
	 	
	 	expDebug::dPrint('Exception Message / Object $e = '.print_r($e,true),5);
	 	expDebug::dPrint('$from_process = '.print_r($from_process,true),5);
	 	
	 	if($from_process=="update"){
	 		$exception_message=$e;
	 	}else{
	 		$exception_message=$e->getMessage();
	 	}
	 	
	 	//To get SSO User_Name
	 	$sso_user = $_SESSION['ses_saml_username'];
	 	expDebug::dPrint('SSO User_Name = '.$sso_user,5);
	 	
	 	//To Check notification exists or not
	 	$select = db_select('slt_notification_info', 'sni');
	 	$select->innerJoin('slt_notification_frame', 'snf', 'snf.notification_id=sni.id');
	 	$select->addExpression('COUNT(sni.id)', 'count');
	 	$select->addField('sni', 'notification_cc', 'cc_mail');
	 	$select->condition('sni.notification_code','notify_saml_debug','=');
	 	$select->condition('sni.status', 'cre_ntn_sts_atv','=');
	 	
	 	expDebug::dPrintDBAPI('get Saml Notification $select = ', $select,5);
	 	$saml_notify_result = $select->execute()->fetchAll();
	 	expDebug::dPrint('$saml_notify_result = '.print_r($saml_notify_result,true),5);
	 	$saml_debug_notify_count = $saml_notify_result[0]->count;
	 	expDebug::dPrint('$saml_debug_notify_count = '.$saml_debug_notify_count,5);
	 	
	 	if($saml_debug_notify_count > 0 && $saml_debug_notify_count == "1"){
	 		expDebug::dPrint('SAML debug notification exists',5);
	 	
	 		$receipt_user_email = variable_get('saml_debug_textbox');
	 		expDebug::dPrint('$receipt_user_email = '.$receipt_user_email,5);
	 		
	 		$receipt_cc_mail = (string) $saml_notify_result[0]->cc_mail;
	 		expDebug::dPrint('$receipt_cc_email = '.$receipt_cc_mail,5);
	 	
	 		if($receipt_user_email!=""){ //To check recipient email exists
	 				
	 			$receipt_user_id = getUserIdFromEmail($receipt_user_email);
	 			expDebug::dPrint('$receipt_user_id '.$receipt_user_id,5);
	 				
	 			$result = getUserInformations($receipt_user_id);
	 			$receipt_user_full_name = trim($result['first_name']).' '.trim($result['last_name']);
	 			expDebug::dPrint('$receipt_user_full_name '.$receipt_user_full_name,5);
	 				
	 			$nullValue = NULL;
	 			$insertStmt1 = db_insert('slt_notification');
	 			$insertStmt1->fields(array(
	 					'msg_id' => 'notify_saml_debug',
	 					'lang_code' => 'cre_sys_lng_eng',
	 					'msg_type' => 'SAML Debugging',
	 					'token_str' => 'full_name>|' .$receipt_user_full_name. '~|' . 'sso_log_msg>|' .$exception_message. '~|' . 'SAML_Response>|' .$saml_response.'~|' . 'sso_user_name>|' .$sso_user. '~|',
	 					'user_id' => 1,
	 					'send_to_id' => $receipt_user_id,
	 					'send_to_name' => $receipt_user_full_name,
	 					'send_to_email' => $receipt_user_email,
	 					'send_to_email_cc' => (empty($receipt_cc_mail)) ? NULL : $receipt_cc_mail,
	 					'send_type' => 'php mailer',
	 					'send_status' => 'N',
	 					'attach_content' => $nullValue,
	 					'remarks' => $nullValue,
	 					'created_by' => 1,
	 					'created_on' => now(),
	 					'updated_by' => 1,
	 					'updated_on' => now(),
	 					'custom0'    => $nullValue
	 			));
	 	
	 			expDebug::dPrintDBAPI('$insertStmt1 = ', $insertStmt1,5);
	 	
	 			$samlNotificationId = $insertStmt1->execute();
	 	
	 			expDebug::dPrint('$Notification Insertion Id  $samlNotificationId = ' . print_r($samlNotificationId, true) , 5);
	 		}else{
	 			expDebug::dPrint('Please enter recipient email to get SAML Debug Notification.',5);
	 		}
	 	
	 	}else{
	 		expDebug::dPrint('SAML debug notification does not exist or Notification may be in disabled status.',5);
	 	}
	 }
}


function processSAMLResponse($responseString,$callFrom=''){
	
	$document = new DOMDocument(); 
	$document->loadXML($responseString);
	$element=$document->firstChild;
	
	// 1. Check the namespace
	if ($element->namespaceURI !== SAML2_Const::NS_SAMLP){
		throw new Exception('Unknown namespace of SAML message: ' . var_export($element->namespaceURI, TRUE));
	}
	
	
	// 2. Check if  the localName of the first Element is Response
	
	if ($element->localName !== 'Response'){
		
		throw new Exception('Missing Response in SAML message: ' . var_export($element->localName, TRUE));
	} 
	
	
	
	// 3. Check if the response has valid InResponseTo. If it is unsolicited reject it.
	/*$InResponseTo=NULL;
	if ($element->hasAttribute('InResponseTo')){
	
		$InResponseTo=$element->getAttribute('InResponseTo');
	}
	
	if(empty($InResponseTo)){
		
		throw new Exception('Unsolicited SAML Authentication post. Rejecting.... ');
	}
	
	// Compare the inResponseTo with that of the one sent as part of
	// saml authentication request.
	$sessInResponseTo=$_COOKIE['saml_request_id'];
	
	if($InResponseTo != $sessInResponseTo){
		throw new Exception('Unsolicited SAML Authentication post. Rejecting.... ');

	}*/
	$xPath=getXPathObject($element);
	
	// 4. Get the Status code (Success or Failure)
	$responseStatus=getResponseStatus($xPath,$element);
	
	if(empty($responseStatus) || ( $responseStatus['code']!=SAML2_Const::STATUS_SUCCESS)){
	
		throw new Exception('Authentication attempt failed: ');
		
	}
	// 4. Get the Assertion Node from XML;
	$assertion=getSamlAssertion($xPath,$element);
	if(empty($assertion)){
	
		throw new Exception('Missing Assertion in SAML message: ');
	}
	// Reinitialize xPath because assertion can be encrypted or plain.
	// This is critical.
	$xPath=getXPathObject($assertion);
	// 5. Get NameID value and type from Saml Assertion
	
	$nameIdArray = getNameIdFromSAMLSubject($xPath,$assertion);
	
	// 5. Process Attributes
	$attributes=processSAMLAttributes($xPath,$assertion);

		authenticateUserInExpertusONE($nameIdArray,$attributes,$callFrom);
}
	
function getSamlAssertion($xPath,$element){

    if ($element === NULL) {
    return;
    }
    for ($node = $element->firstChild; $node !== NULL; $node = $node->nextSibling) {
            

            if ($node->namespaceURI !== SAML2_Const::NS_SAML) {
                    continue;
            }
            if ($node->localName === 'Assertion') {
                 return $node;
            }elseif($node->localName === 'EncryptedAssertion') 
			{
				return decryptAssertion($xPath,$node);
			}

    }
}

function processSAMLAttributes($xPath,$assertion){

	if(empty($assertion)){
		return;
	}
	// Check if ID exists
	
	if (!$assertion->hasAttribute('ID')) {
		throw new Exception('Missing ID attribute on SAML assertion.');
	}
	
	// Check if the version attribute exists and the value is 2.0
	if ($assertion->getAttribute('Version') !== '2.0') {
		/* Currently a very strict check. */
		throw new Exception('Unsupported version: ' . $xml->getAttribute('Version'));
	}
	
	if (!$assertion->hasAttribute('IssueInstant')) {
		throw new Exception('Missing Issuing time attribute on SAML assertion.');
	}
			

	$IssueInstant = $assertion->getAttribute('IssueInstant');
	if(!checkCurrentTime($IssueInstant,NULL)){
		throw new Exception('Time Mismatch. Rejecting the request');
		
	}
	
	
	
	$issuer = queryXPath($xPath,$assertion,'./saml_assertion:Issuer');
	if (empty($issuer)) {
		throw new Exception('Missing <saml:Issuer> in assertion.');
	}
	
	$configured_issuer=variable_get('saml_idp_issuer_id');
	if(empty($configured_issuer)){
		throw new Exception('Missing IdP entity Id in Configuration. Check your configuration');
	}
	if(trim($configured_issuer) !== trim($issuer[0]->textContent)){
		throw new Exception('Configured IdP entity and IdP entity in response do not match.'.$configured_issuer.' is not equal to '.$issuer[0]->textContent);
	}
	
	
	$conditions= getSAMLConditions($xPath,$assertion);
	if(!empty($conditions) && !empty($conditions['notbefore']) && !empty($conditions['notafter'])){
	
		if(!checkCurrentTime($conditions['notbefore'],$conditions['notafter'])){
	
			throw new Exception('Time Mismatch. Rejecting the request');
		
		}
	}
	
	
	$attributes=getAttributes($xPath,$assertion);
	
	return $attributes;

}


function getAttributes(DOMXPath $xPath,DOMElement $xml) {

	$attrib= array();

	$firstAttribute = TRUE;
	$attributes = queryXPath($xPath,$xml, './saml_assertion:AttributeStatement/saml_assertion:Attribute');
	foreach ($attributes as $attribute) {
		if (!$attribute->hasAttribute('Name')) {
			throw new Exception('Missing name on <saml:Attribute> element.');
		}
		$name = $attribute->getAttribute('Name');

		if ($attribute->hasAttribute('NameFormat')) {
			$nameFormat = $attribute->getAttribute('NameFormat');
		} else {
			$nameFormat = SAML2_Const::NAMEFORMAT_UNSPECIFIED;
		}
		
		if (!array_key_exists($name, $attrib)) {
			$attrib[$name] = array();
		}

		$values = queryXPath($xPath,$attribute, './saml_assertion:AttributeValue');
		foreach ($values as $value) {
			$attrib[$name][] = trim($value->textContent);
		}
	}
	
	return $attrib;
}

function getNameIdFromSAMLSubject(DOMXPath $xPath,DOMElement $xml) {

	$subjectNameId=queryXPath($xPath,$xml, './saml_assertion:Subject');
			
	if (empty($subjectNameId)) {
		/* No Subject node. */
		return;
	} elseif (count($subjectNameId) > 1) {
		throw new Exception('More than one <saml:Subject> in <saml:Assertion>.');
	}
	$subject = $subjectNameId[0];
	
	$nameId = NULL;
	// Check if the NameID is encrypted first.
	$nameId = queryXPath($xPath,$subject, './saml_assertion:EncryptedID/xenc:EncryptedData');
	// NameID may not be encryted, now going for NameID check with out encryption
	if(empty($nameId)){
		$nameId = queryXPath($xPath,$subject, './saml_assertion:NameID');			
	}	
	// NameID not available either in encrypted or plain format.
	if (empty($nameId)) {
		throw new Exception('Missing <saml:NameID> or <saml:EncryptedID> in <saml:Subject>.');
	} elseif (count($nameId) > 1) {
		throw new Exception('More than one <saml:NameID> or <saml:EncryptedD> in <saml:Subject>.');
	}
	// Let us get the first node.
	$nameId = $nameId[0];
	
	$nameIdArray=array('isEncryptedNameID'=>FALSE);
	
	if ($nameId->localName === 'EncryptedData') {
		/* The NameID element is encrypted. */
		$nameIdArray['isEncryptedNameID'] = TRUE;
		$nameIdArray['NameIDValue'] = $nameId;
		
	} else {
	
		$nameIdArray['NameIDValue']= trim($nameId->textContent);
		
		foreach (array('NameQualifier', 'SPNameQualifier', 'Format') as $attr) {
			if ($nameId->hasAttribute($attr)) {
				$nameIdArray[$attr] = $nameId->getAttribute($attr);
			}
		}
	
	}
	return $nameIdArray;
	
}

function getResponseStatus(DOMXPath $xPath,DOMElement $xml){

	$status = queryXPath($xPath,$xml, './saml_protocol:Status');
	if (empty($status)) {
		throw new Exception('Missing status code on response.');
	}
	$status = $status[0];

	$statusCode = queryXPath($xPath,$status, './saml_protocol:StatusCode');
	if (empty($statusCode)) {
		throw new Exception('Missing status code in status element.');
	}
	$statusCode = $statusCode[0];

	$statusArray=array('code'=> $statusCode->getAttribute('Value'));
	

	$subCode = queryXPath($xPath,$statusCode, './saml_protocol:StatusCode');
	if (!empty($subCode)) {
		$statusArray['subcode'] = $subCode[0]->getAttribute('Value');
	}
	
	return $statusArray;

}

function getSAMLConditions(DOMXPath $xPath,DOMElement $xml){

	$cond=queryXPath($xPath,$xml, './saml_assertion:Conditions');
	
	
	if(empty($cond)){
		return;
	}
	$cond=$cond[0];
	$notbefore=$cond->hasAttribute('NotBefore')?$cond->getAttribute('NotBefore'):NULL;
	$notafter=$cond->hasAttribute('NotOnOrAfter')?$cond->getAttribute('NotOnOrAfter'):NULL;
	
	$samlcond= array('notbefore'=>$notbefore,'notafter'=>$notafter);
	
}

function authenticateUserInExpertusONE($nameId,$samlAttributes,$callFrom){

	$whichUserAttribute=variable_get('saml_nameid_format');
	expDebug::dPrint("SAML - LMS Match attribute -- ".$whichUserAttribute,4);
	if(empty($whichUserAttribute)){
	
		throw new Exception(' Improper Saml settings. Unable to fetch the saml nameId from settings');
	}

	switch($whichUserAttribute){
	
		case 'email': 
			authUser($nameId['NameIDValue'],'email',$callFrom,$samlAttributes);
			break;
			
		case 'persistent' :
		    authUser($nameId['NameIDValue'],'name',$callFrom,$samlAttributes);
			break;
			
		case 'attribute' :
			$uname=variable_get('saml_nameid_attribute_name');
			if(empty($uname)){
				throw new Exception(' Improper Saml settings. Name ID is set to Attribute based, but the name of the attribute is not set');
			}
			authUser($samlAttributes[$uname][0],'name',$callFrom,$samlAttributes);
			break;
		default :
			throw new Exception(' Improper Saml settings. Improer NameID format in settings');
	
	}
}

function initializeUserAttributes($userAttributes) {
	$form_state = array();
	global $userSSOAttributes;
	$userSSOAttributes = $userAttributes;
	expDebug::dPrint('SAML User Attributes = ' .print_r($userAttributes, true),5);
			
	if(isset($userAttributes)){
		
		$form_state['values']['first_name'] = $userAttributes['FirstName'][0];
		$form_state['values']['last_name'] = $userAttributes['LastName'][0];
	    
	    $form_state['values']['user_name'] = $userAttributes['UserName'][0];
	    $form_state['values']['email'] = $userAttributes['Email'][0];
			
		$form_state['values']['addr1'] = $userAttributes['Addr1'][0];
		$form_state['values']['addr2'] = $userAttributes['Addr2'][0];

		//Get Country Name
		$countryname = $userAttributes['Country'][0];
		$countrycode = db_select('slt_country', 'cnt');
		$countrycode->addField('cnt','country_code','cnt_code');
		$countrycode->condition('country_name',$countryname,'=');
		$temp_country_code = $countrycode->execute()->fetchField();
		$form_state['values']['country'] = $temp_country_code;
			
		//Get State Name
		$statename = $userAttributes['State'][0];
		$statecode = db_select('slt_state', 'st');
		$statecode->addField('st','state_code','st_code');
		$statecode->condition('country_code',$temp_country_code,'=');
		$statecode->condition('state_name',$statename,'=');
		$temp_state_code = $statecode->execute()->fetchField();
		$form_state['values']['state'] = $temp_state_code;
			
			
		$form_state['values']['city'] = $userAttributes['City'][0];
		$form_state['values']['zip'] = $userAttributes['Zip'][0];
		$form_state['values']['phone_no']= $userAttributes['PhoneNo'][0];
		
		if(!empty($userAttributes['JobRole'][0])){
			$form_state['values']['jobrole'] = key(getMatchingProfileItemNamesKeyed($userAttributes['JobRole'][0], 'cre_usr_jrl_'));
		}else{
			$form_state['values']['jobrole']='';
		}
		 
		//To check whether or not the JobRole is present in the LMS.
		if($form_state['values']['jobrole']==''){
			expDebug::dPrint(("SAML JobRole -> ".$userAttributes['JobRole'][0]." does not exist in the LMS."),5);
		}
		
		if(!empty($userAttributes['JobTitle'][0])){
			$form_state['values']['jobtitle'] = key(getMatchingProfileItemNamesKeyed($userAttributes['JobTitle'][0], 'cre_usr_jtl_'));
		}else{
			$form_state['values']['jobtitle']='';
		}
		
		//To check whether or not the JobTitle is present in the LMS.
	    if($form_state['values']['jobtitle']==''){
			expDebug::dPrint(("SAML JobTitle -> ".$userAttributes['JobTitle'][0]." does not exist in the LMS."),5);
		}
		
		if(!empty($userAttributes['EmplType'][0])){
			$form_state['values']['empltype'] = key(getMatchingProfileItemNamesKeyed($userAttributes['EmplType'][0], 'cre_usr_etp_'));
		}else{
			$form_state['values']['empltype']='';
		}
		
		//To check whether or not the EmplType is present in the LMS.
	    if($form_state['values']['empltype']==''){
			expDebug::dPrint(("SAML EmplType -> ".$userAttributes['EmplType'][0]." does not exist in the LMS."),5);
		}
		
		$form_state['values']['employee_no'] = $userAttributes['EmployeeNo'][0];
		
		if(!empty($userAttributes['Department'][0])){
			$form_state['values']['deptcode'] = key(getMatchingProfileItemNamesKeyed($userAttributes['Department'][0], 'cre_usr_dpt_'));
		}else{
			$form_state['values']['deptcode']='';
		}
		
		//To check whether or not the Department is present in the LMS.
	    if($form_state['values']['deptcode']==''){
			expDebug::dPrint(("SAML Department -> ".$userAttributes['Department'][0]." does not exist in the LMS."),5);
		} 
		
		if(!empty($userAttributes['UserType'][0])){
			$form_state['values']['usertype'] = key(getMatchingProfileItemNamesKeyed($userAttributes['UserType'][0], 'cre_usr_ptp_'));
		}else{
			$form_state['values']['usertype']='';
		}
		
		//To check whether or not the UserType is present in the LMS.
	    if($form_state['values']['usertype']==''){
			expDebug::dPrint(("SAML UserType -> ".$userAttributes['UserType'][0]." does not exist in the LMS."),5);
		}
	
		$orgname = $userAttributes['Organization'][0];
		$orgid = db_select('slt_organization', 'org');
		$orgid->addField('org','id','org_id');
		$orgid->condition('name',$orgname,'=');
		$temp_org_id = $orgid->execute()->fetchField();
		$form_state['values']['org_id'] = $temp_org_id;
		//To check whether or not the Organization is present in the LMS.
		if($form_state['values']['org_id']==''){
			expDebug::dPrint(("SAML Organization -> ".$userAttributes['Organization'][0]." does not exist in the LMS."),5);
		}
		
	    $mgrname = $userAttributes['ManagerUserName'][0];
		$mgrid = db_select('slt_person', 'per');
		$mgrid->addField('per','id','mgr_id');
		$mgrid->condition('user_name',$mgrname,'=');
		$temp_mgr_id = $mgrid->execute()->fetchField();
		$form_state['values']['manager_id'] = $temp_mgr_id;
		//To check whether or not the ManagerUserName is present in the LMS.
		if($form_state['values']['manager_id']==''){
			expDebug::dPrint(("SAML ManagerUserName -> ".$userAttributes['ManagerUserName'][0]." does not exist in the LMS."),5);
		}
		
		if(!empty($userAttributes['TimeZone'][0])) {
			$form_state['values']['preferred_timezone'] = key(getMatchingProfileItemNamesKeyed($userAttributes['TimeZone'][0], 'cre_sys_tmz_'));
		}
		$form_state['values']['status'] = "cre_usr_sts_atv";
	}
	return $form_state;
}


function authUser($identity,$from,$callFrom,$userAttributes){

	global $user;
	include_once $_SERVER["DOCUMENT_ROOT"].'/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_people/exp_sp_administration_user/exp_sp_administration_user.inc';
	
	if($identity === 'admin'){
		throw new Exception("user name received as admin. This is reserved for internal use");
	}
	if($from == 'email'){
		$account = user_load_by_mail($identity);
		$user=user_load($account->uid);			
	}else{
		$account = user_load_by_name($identity);
		$user=user_load($account->uid);
	}
   
   $_SESSION['ses_saml_username']=$identity; //To hold the SSO user_name in session variable
   
   $lms_update_option="";
   $autocreate = variable_get('saml_Auto_create','no');
   expDebug::dPrint("Auto Create/Update option -  " .$autocreate,4);
	//Check whether user exists in LMS or not
	if(!isset($account->uid) || $account->uid=="") {
				
		expDebug::dPrint('New user creation in LMS through SAML',4);
		
		if($autocreate=='yes'){

			//Get Attributes from SAML 
			$attributes=initializeUserAttributes($userAttributes);
			expDebug::dPrint('Attributes for New user - $userAttributes : ' .print_r($attributes, true),4);
			
			$attributes['values']['roles'] = "N";
			$attributes['values']['sendnotification'] = "N";
			
			$returnObj = addNewUserByRestAPI($attributes,"1");
			expDebug::dPrint('$Add New User - returnObj = ' .print_r($returnObj, true),4);
			
			if($returnObj->isValidateError == 1){
				$errormsg=$returnObj->errormsg;
				expDebug::dPrint('$errormsg while Creating new user through SSO = '.print_r($errormsg,true),4);
				throw new Exception($errormsg);
		    }
			
			$lms_user_id=$returnObj[0]->Id;
			expDebug::dPrint('$New User Id in LMS = ' .$lms_user_id,4);
			
			
			if($lms_user_id!="" && $lms_user_id>0){
								
				if($from == 'email'){
					$account = user_load_by_mail($identity);
					$user=user_load($account->uid);
					add_audit_trail_entry($lms_user_id, $lms_user_id, 'cre_sys_obt_usr', 'saml_response', 'Login', 'Login via SSO - New user');
				}else{
					$account = user_load_by_name($identity);
					$user=user_load($account->uid);
					add_audit_trail_entry($lms_user_id, $lms_user_id, 'cre_sys_obt_usr', 'saml_response', 'Login', 'Login via SSO - New user');
				}					 
			}
		}
    }else{
		if($autocreate=='yes'){
		expDebug::dPrint('Update existing user in LMS through SAML',4);
		$lms_update_option="1";
	    }
	}
	
	$saml_err="";
	if(isset($account->uid)){	  // For updating users

		// Check if user is blocked.
		$state['values']['name'] = $account->name;
		user_login_name_validate(array(), $state);
		
		if (!form_get_errors()) {

		   //Update user details 
           if($autocreate=='yes' && $lms_update_option=="1"){ 
				$attributes=initializeUserAttributes($userAttributes);
				   expDebug::dPrint('Attributes for Existing user : ' .print_r($attributes, true),4);
				
				//Get LMS USER ID.
				$lmsUserId = 0;
				if($account->uid){
					$lmsUserId = getUserIdFromDrupalUid($account->uid);
				}
				expDebug::dPrint("LMS USER ID : ".$lmsUserId);
				
				//In order to sustain user's role for is_manager & is_instructor
				$current_val = db_select('slt_person','per');
				$current_val->addField('per','is_manager','mgr');
				$current_val->addField('per','is_instructor','ins');
				$current_val->condition('id',$lmsUserId,'=');
				$get_current_val= $current_val->execute()->fetchAll();
				
				expDebug::dPrint("Roles->" .print_r($get_current_val,true),4);
					
			    if($get_current_val[0]->ins=='Y')
			    {
			    	$is_instructor="instructor";
			    } else {
			    	$is_instructor="";
			    }
			    				  
				if($get_current_val[0]->mgr=='Y') {
					$is_manager="manager";
				} else {
					$is_manager="";
				}
				 
				if($is_manager!="" && $is_instructor!="") { $role = "$is_instructor,$is_manager"; }
				else if($is_manager!="" && $is_instructor=="") { $role = "$is_manager";  }
				else if($is_instructor!="" && $is_manager=="") { $role = "$is_instructor"; } 
				else if($is_manager=="" && $is_instructor=="") { $role = "N";  }  
 				
				$attributes['values']['roles'] = $role;
				expDebug::dPrint(" Attributes for Roles " .print_r($attributes['values']['roles'],true),4);
				
				$attributes['values']['id'] = $lmsUserId;
				$attributes['values']['request_source'] = 'saml'; // to identify request called from saml
				$returnObj = updateUserByRestAPI($attributes, "1");
				expDebug::dPrint('$update returnObj = ' .print_r($returnObj, true),4);

				if($returnObj->isValidateError == 1){
					$errormsg=$returnObj->errormsg;
					expDebug::dPrint('$errormsg while Updating existing user through SSO= '.print_r($errormsg,true),4);
					expDebug::dPrint('$_SESSION[ses_saml_response] =  '.print_r($_SESSION['ses_saml_response'],true),4);

					if(trim($_SESSION['ses_saml_response'])!="" && isset($_SESSION['ses_saml_response'])){						
						SAMLDebug($_SESSION['ses_saml_response'],$errormsg,'update');
						unset($_SESSION['ses_saml_response']);
					} 
					
				}
				
				add_audit_trail_entry($lmsUserId, $lmsUserId, 'cre_sys_obt_usr', 'saml_response', 'Login', 'Login via SSO - Exisiting user');
			}
			
			//Check where the call from mobile or web
			
		if(isset($callFrom) && $callFrom=='mobile'){
						// Redirect to mobile app with required params
						expDebug::dPrint("Redirect to Mobile .....".$from." --- identity -- ".$identity,5);
						include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/Encryption.php";
						$enc = new Encrypt();
						$ideVal = $enc->encrypt($identity);
						expDebug::dPrint("Redirect to Mobile (encrypted).....".$from." --- identity -- ".$ideVal,5);
						// Generate redirect url for mobile
						if($from=='email'){
							$redirectUrl = 'x-expertusone-oauth://callback?sso=true&error=false&matchEntity=email&entitydata='.$ideVal;
						}else {
							$redirectUrl = 'x-expertusone-oauth://callback?sso=true&error=false&matchEntity=name&entitydata='.$ideVal;
						}
						?>
								  <script language='javascript'>
											window.location = "<?php print $redirectUrl;?>";
								  </script>
				   <?php
		}else{
						// Set the cookie to indicate that the saml 
						// authentication is successful.
					    global $ssoauthenticated;
						$ssoauthenticated = 'saml';
						// Load global $user and perform final login tasks.
						$form_state['name'] = $account->name;
						$form_state['pass'] = $account->pass;
						$form_state['redirect']='learning/enrollment-search';
						$edit['redirect']='learning/enrollment-search';
						$edit["values"]["name"]=$account->name;
						$edit["values"]["pass"]=$account->pass;
						user_login_finalize($edit);
			}	
	  }else{
		   	   $saml_err="1";
	  }
      }else{
	  	    $saml_err="1";
      }
	  
	 if($saml_err=="1"){
	  	throw new Exception('We are sorry. Unable to locate your account -> '.$identity.'. Please contact support for details.');
	  }
}


function decryptAssertion(DOMXPath $xPath,$node){
		
	
	$rootElement=queryXPath($xPath,$node,'./xenc:EncryptedData');

	if(!empty($rootElement)){
	
		return decryptSAMLAssertion($rootElement[0]);
		
	}else{
		throw new Exception('Unable to find the Encrypted Data in SAML response');
	}
}	
?>