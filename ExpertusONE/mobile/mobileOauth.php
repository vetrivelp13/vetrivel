<?php
// Added by Ajay.R. For oauth2 in mobile app

include_once $_SERVER['DOCUMENT_ROOT']."/sites/all/services/GlobalUtil.php";
include_once $_SERVER['DOCUMENT_ROOT']."/sites/all/services/Trace.php";
define('DRUPAL_ROOT', $_SERVER["DOCUMENT_ROOT"]);

include_once DRUPAL_ROOT . '/includes/bootstrap.inc';

drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc');
include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrsearch/exp_sp_lnrsearch.inc');
include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_core/exp_sp_core.module');
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_sitesetup/exp_sp_administration_module_info/exp_sp_administration_module_info_share.inc";

include_once $_SERVER["DOCUMENT_ROOT"]."/apis/core/login/login_dao.php";
$username="";
$email="";
$pwd="";

if(empty($_REQUEST))
{
	
	$result="error=Invalid host";
	//expDebug::dPrint("API Login Response.".$result);
	header('HTTP/1.1 200 OK');
	header('Content-Length: '.strlen($result));
	header('Content-Type: application/x-www-form-urlencoded');
	echo $result;
	return;
}

//force password change in mobile.

if (isset ( $_REQUEST ['ForcedPasswordChange'] )) {
	$dao = new LoginDAO ();
	$result = $dao->ForcedPasswordChange ( $_REQUEST ['user_id'], $_REQUEST ['newpassword'], $_REQUEST ['confirmpassword'] );
	expDebug::dPrint ( "ForcedPasswordChange ..." . print_r ( $result, true ), 4 );

	if (is_array ( $result )) {
		$result_str = "ForcedPasswordChange=";
		for($k = 0; $k < count($result); $k ++) {
			if ($result[$k] != "") {
				expDebug::dPrint ( "ForcedPasswordChange if looppppppp result string..." . print_r ( $string, true ), 4 );
				$result_str .= $result[$k]."~";
			}
		}
	} else if ($result == 1) {
		expDebug::dPrint ( "ForcedPasswordChange successss result..." . print_r ( $result, true ), 4 );
		$result_str = "ForcedPasswordChange=0";
	}
	echo $result_str;
	return;
}

//GetCatalogSearchPrams in mobile.
if (isset ( $_REQUEST ['GetCatalogSearchPrams'] )) {
	$dao = new LoginDAO ();
	$result = $dao->GetCatalogSearchPrams ( $_REQUEST ['user_id'], $_REQUEST ['searchUrl']);
	expDebug::dPrint ( "GetCatalogSearchPrams ..." . print_r ( $result, true ), 4 );
	if (!empty($result)){
		$result_str ="GetCatalogSearchPrams=success".$result;
	}
	else{
		$result_str ="GetCatalogSearchPrams=failure";
	}

	echo $result_str;
	return;
}


if(isset($_REQUEST['check_user'])){
	$dao=new LoginDAO();
	$result=$dao->isUserActive($_REQUEST['user_id']);
	expDebug::dPrint("whats the value of video_streaming...".$result);
	header('HTTP/1.1 200 OK');
	header('Content-Length: '.strlen($result));
	header('Content-Type: application/x-www-form-urlencoded');
	
	$status ="userstatus=".$result[0]['userstatus'];
	echo $status;
	return;
}

$username=$_REQUEST["username"];
$email=$_REQUEST["email"];
$pwd=$_REQUEST["password"];
$salesforce="";
$salesforce=$_REQUEST["salesforce"];
$sso =$_REQUEST["sso"];
	
$paramsObj=new stdClass();
$paramsObj->username=$username;
$paramsObj->email=$email;
$paramsObj->password=$pwd;
$paramsObj->sso = $sso;
$paramsObj->salesforce = $salesforce;

$dao=new LoginDAO();
$util = new GlobalUtil();
$conf = $util->getConfig();

			$result=$dao->doLogin($paramsObj);
			$uid="";
			$roleid="";
			$is_manager="";
			$is_instructor="";
			$user_status="";
			$len=0;
			$len = count($result);
			$pref_lang ="";
			$pref_currency ="";
			$registration_level = $conf['registration_level'];
			$mobile_analytics = getConfigValue ( 'mobile_analytics' );
			$mylearning_mobile_view = getConfigValue ('mylearning_mobile_view');
			$enabledLanguages ="";
			$commerceEnabled ="";
			$ratingEnabled ="";
			$mobile_catalog = "";
			$iphonedownloadurl = $conf['iphone_download_url'];
			$androiddownloadurl = $conf['android_download_url'];
			$default_profile_currency = getCurrencyDefDetails();
			$allow_currency_change = getConfigValue ( 'allow_currency_change' );
			$share_module_status = getShareModuleStatus ( 'api' );
			
			$priceRange = getSliderPriceRange();
			
			if(empty($priceRange['startprice']))
			{
				$priceRange['startprice'] = 0;
			}
			
			if(empty($priceRange['endprice']))
			{
				$priceRange['endprice'] = 0;
			}
		
			$minprice = $priceRange['startprice'];
			$maxPrice = $priceRange['endprice'];
			 //print_r($result);
				
			 for($i=0;$i<$len;$i++)
			 {
			 		$row=$result[$i];
		  			$uid=$row["USERID"];
		  			$roleid=$row["roleid"];
		  			$pass=$row["pass"];
		  			$is_manager=$row["is_manager"];
		  			$is_instructor=$row["is_instructor"];
		  			$user_status=$row["user_status"];
		  			$pref_lang=$row["preferred_language"];
		  			$full_name=$row["full_name"];
		  			$email_id=$row["email_id"];
		  			$pref_currency = $row["preferred_currency"].'-~'.$row["preferred_currency_name"];
		  			$enabledLanguages = $row["enabledLanguages"];
		  			$commerceEnabled = $row["commerceEnabled"];
		  			$ratingEnabled = $row["ratingEnabled"];
 		  			$enabledCurrency = $row["enabledCurrency"];
 		  			$enabledBlocks = $row["enabledblocks"];
 		  			$mobile_catalog = $row["catalogEnabled"];
 		  			
			 }
			 $error="";
			 if($pass == 'temp_ip_block')
			 {
			 	$error='temp_ip_block';
			 }
			 else if(count($result)==0){
			 	$error='Invalid User';
			 }else if($uid=="" || $uid==null)
			 {
			 	$error='Invalid Password';
			 }
			 else if($pass == 'expired')
			 {
			 	$error='Password expired';
			 	//force password change in mobile.
			 	include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_core/modules/password_policy/password_policy.module');
			 	$policy = _password_policy_load_active_policy(array(DRUPAL_AUTHENTICATED_RID));
			 	if (!empty($policy['policy'])) {
			 		foreach ($policy['policy'] as $key => $value) {
			 			$file_uri = '/sites/all/modules/core/exp_sp_core/modules/password_policy/constraints/constraint_'.$key.'.inc';
			 			include_once($_SERVER["DOCUMENT_ROOT"].$file_uri);
			 			$error_constraint['constraint_'. $key] = _password_policy_constraint_error($key, $value);
			 		}
			 	}
			 	
			 	$error_constraint = implode("|",$error_constraint);
			 	expDebug::dPrint("Print out each constraint".print_r($error_constraint,true));
			 	
			 		
			 }else if($pass == 'blocked')
			 {
			 	$error='Password blocked';
			 }
			 
			 
			 
			 if($error!="")
			 {
			 	$result="error_constraint=".$error_constraint."&error=".$error."&userid=".$uid."&roleid=".$roleid."&oauth_token=".$_REQUEST["oauth_token"]."&oauth_token_secret=".$_REQUEST["oauth_token_secret"]."&oauth_verifier=null";
				//expDebug::dPrint("API Login Response.".$result);
			 	header('HTTP/1.1 200 OK');
				header('Content-Length: '.strlen($result));
				header('Content-Type: application/x-www-form-urlencoded');
				echo $result;
			 }
			 else
			 {
				$event = new Client();
				$action = $event->getDetailsByScope('APP',$_SERVER["HTTP_HOST"]);
				
				expDebug::dPrint("Oauth client details ".print_r($action,true),4);
				
				
				$drupalUid = getDrupalIdOfUser($uid);
				
				$account = user_load($drupalUid);
				
				$is_training_admin = user_access('Administration Perm', $account);
				$training_admin = ($is_training_admin == 1 || $is_training_admin == "1" )?"Y":"N";
				include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_sitesetup/exp_sp_administration_module_info/exp_sp_administration_module_info.inc');
				$cdn_status  = getDrupalModuleStatus(array('cloud_files'));
				$obj = json_decode($action);
				
				$notification_notviewed=$dao->getNotifCount($uid);
				
				$video_streaming  = getDrupalModuleStatus(array('exp_sp_stream_server'));
			
				
				expDebug::dPrint("whats the value of video_streaming...".$video_streaming);
				
 				$result="error=";
				$result.="&userid=".$uid;
				$result.="&roleid=".$roleid;
				$result.="&client_id=".$obj->client_id;
				$result.="&client_secret=".$obj->client_secret;
				$result.="&redirect_uri=".$obj->redirect_uri;
				$result.="&grant_type=".$obj->grant_type;
				$result.="&scope=".$obj->scope;
				$result.="&user_id=".$obj->user_id;
				$result.="&full_name=".$full_name;
				$result.="&email_id=".$email_id;
				$result.="&is_manager=".$is_manager;
				$result.="&is_instructor=".$is_instructor;
				$result.="&is_training_admin=".$training_admin;
				$result.="&user_status=".$user_status;
				$result.="&pref_lang=".$pref_lang;
				$result.="&registration_level=".$registration_level;
				$result.="&mobile_analytics=" . $mobile_analytics;
				$result.="&mylearning_mobile_view=" . $mylearning_mobile_view;
				$result.="&default_profile_currency=" . $default_profile_currency;
				$result.="&allow_currency_change=" . $allow_currency_change;
				$result.="&share_module_status=" . $share_module_status;
				
				$result.="&notification_notviewed=".$notification_notviewed;
				$result.="&cdn_status=".$cdn_status;
				$result.="&video_streaming=".$video_streaming;
				$result.="&preferred_currency=".$pref_currency;
				$result.="&enabledLanguages=".$enabledLanguages;
				$result.="&commerceEnabled=".$commerceEnabled;
				$result.="&ratingEnabled=".$ratingEnabled;
				$result.="&mobile_catalog=".$mobile_catalog;
				$result.="&iphonedownloadurl=".$iphonedownloadurl;
				$result.="&androiddownloadurl=".$androiddownloadurl;
				$result.="&min_price=".$minprice;
				$result.="&max_price=".$maxPrice;
				$result.="&enabledCurrency=".$enabledCurrency;
				$result.="&enabledBlocks=".$enabledBlocks;
  				$result.="&publicIp=".get_remote_address();
				$result.="&currencyServerUrl=".$conf['ip_locator_url'];
				
				
				$selectStmt = db_select('slt_person', 'prsn');
				$selectStmt->leftJoin('slt_profile_list_items', 'prfl', 'prfl.code = prsn.time_zone');
				$selectStmt->condition('prsn.id', $uid);
				$selectStmt->addField('prfl', 'name', 'timezone_name');
				$timezone =  $selectStmt->execute()->fetchAll();
				$result.="&timezone_name=".$timezone[0]->timezone_name;
				
				add_audit_trail_entry($uid, $uid, 'cre_sys_obt_usr', 'exp_sp_login', 'Login', 'Login via Mobile');
				
				//CODE ADDED FOR LAST LOGIN
				// GETTING PERSONS DETAILS
				$selectStmt = db_select('slt_person', 'prsn');
				$selectStmt->join('users', 'usr', 'prsn.user_name = usr.name');
				$selectStmt->leftJoin('slt_profile_list_items', 'prfl', 'prfl.code = prsn.time_zone');
				$selectStmt->condition('usr.uid', $drupalUid);
				$selectStmt->addField('usr', 'uid', 'id');
				$selectStmt->addExpression('CONCAT(prsn.first_name,\' \',prsn.last_name)','full_name');
				$selectStmt->addField('prsn', 'user_name', 'user_name');
				$selectStmt->addField('prfl', 'attr1', 'time_zone');
				$selectStmt->addField('prfl', 'code', 'time_zone_code');
				$selectStmt->addField('prfl', 'attr2', 'timezone_attr');
				expDebug::dPrintDBAPI('MY ESIGN INFO ' , $selectStmt);
				$Per_result =  $selectStmt->execute()->fetchAll();
				if(!$Per_result[0]->timezone_attr){
					$Per_result[0]->timezone_attr = 'Asia/Calcutta';
				}
				//GETTING THE CURRNET DATE AND TIME//DEFALULT TIME ZONE FOR LAST LOGIN
				$currDateTime = date_format(date_create($ses_start_date_format),'Y-m-d H:i');
				$selectStmt = db_select('variable', 'variable');
				$selectStmt->condition('variable.name', "date_default_timezone");
				$selectStmt->addField('variable', 'name', 'name');
				$selectStmt->addField('variable', 'value', 'value');
				$defaultTimeZone =  $selectStmt->execute()->fetchAll();
				$defaultTimeZone_value = unserialize($defaultTimeZone[0]->value);
				expDebug::dPrint ( "Last_Login inside defaultTimeZone_value  " . print_r ( $defaultTimeZone_value, true ), 4 );
				//GETTING THE TIME STAMP FOR LAST LOGIN
				$convertedCurrTime  = timeZoneConvert($currDateTime , $defaultTimeZone_value , $Per_result[0]->timezone_attr);
				$formattedCurrTime 	= date_format($convertedCurrTime,'Y-m-d H:i');
				$formattedCurrTime = date_format(date_create($formattedCurrTime),'Y-m-d H:i:s');
				$formattedCurrTime = strtotime($formattedCurrTime);
				expDebug::dPrint ( "Last_Login formattedCurrTime " . print_r ( $convertedCurrTime, true ), 4 );

				//UPDATING DB FOR LAST LOGIN
				$query = db_merge('users');
				$query->key(array(
						'uid' => $drupalUid,
				));
				$query->fields(array(
						'login' => $formattedCurrTime,
				));
				$query->fields(array(
						'access' => $formattedCurrTime,
				));
					
				$query->execute();
				
				
				$query = db_merge('slt_manage_pushnotification');
				$query->key(array(
						'user_id' => $uid,
				));
				$query->fields(array(
						'logged_out' => 0,
				));
				$query->execute();
				
				

// 				$result.="&enabledCurrencies=".$enabledCurrencies;
				
				
				expDebug::dPrint("CDN status = ....".$cdn_status);
				expDebug::dPrint("whats the value of username...".$obj->user_id);
				
				if(module_exists('saml')){
					$result.="&sso=true";
					$result.="&multimode_auth=".variable_get('saml_multimode_auth');
				}else{
					$result.="&sso=false";
				}
				header('HTTP/1.1 200 OK');
				header('Content-Length: '.strlen($result));
				header('Content-Type: application/x-www-form-urlencoded');
				
				echo $result;
			 }


//echo 'tes';//$tets;



?>