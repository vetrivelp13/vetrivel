<?php
/**
    * This class contains database related transactional for the OAuth and OpenID login process.
    * @author Sureshkumar.v
*/


include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/dao/AbstractDAO.php";


include_once $_SERVER["DOCUMENT_ROOT"] . '/includes/bootstrap.inc';
include_once $_SERVER["DOCUMENT_ROOT"] . '/includes/password.inc';
include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_manage/exp_sp_administration_announcement/exp_sp_administration_announcement.inc');


//include_once "../AbstractDAO.php";

class LoginDAO extends AbstractDAO
{
	/**
	* Constructor sets up. 
    */	
	public function __construct()
	{
		parent::__construct();
	}
	
		
	/**
    * This api is used to get the user id based on the openid entitity. user id and openid mapping will be available in slt_entity_profile_mapping table.
    * @param identity string 
    * @return result
    */	
	public function getUserIdbyIdentity($identity)
	{
		try
		{
			$qry="SELECT u.uid,u.pass,u.name  FROM slt_entity_profile_mapping map, slt_person p, users u WHERE     p.id = map.entity_id       AND u.name = p.user_name ";
			$qry.=" AND map.entity_type = 14  AND find_in_set('".$identity."',map.col4)";
			//$qry="select entity_id from slt_entity_profile_mapping where entity_type=14 and find_in_set(col4,'".$identity."')";
			//$qry="SELECT uid FROM authmap where authname='".$identity."'";
			//expDebug::dPrint("sql....authmap...".$qry);
			$this->connect();
			$result = $this->query($qry)->fetchAll();
			
			//$result = $this->fetchResult();
			$this->closeconnect();
			return $result;
		}catch(Exception $e){
				echo $e->getMessage();
		}
		
	}
	
	
	/**
    * This api is used to verify the user whether active or not.
    * @param uid string 
    * @return result
    */	
	public function verifyUser($uid)
	{
		try
		{
			$qry="SELECT p.id as USERID FROM users u,slt_person p where u.uid='".$uid."' and u.status=1 and p.user_name=u.name";
			//expDebug::dPrint("sql....verifyuser...".$qry);
			$this->connect();
			$result = $this->query($qry)->fetchAll();
			
			//$result = $this->fetchResult();
			$this->closeconnect();
			return $result;
		}catch(Exception $e){
				echo $e->getMessage();
		}
		
	}
	
public function getNotifCount($userId)
	{
		try
		{
			$_REQUEST['userid'] = $userId;
			$_REQUEST['apiname'] = 'login';
			$_REQUEST['mobile_request'] = 'true';
	//		$announcementListCount = 0;  //should be commented before commit
			$announcementListCount = fetchAnnouncementUserWidget('count',$userId);	//should be uncommented before commit		
			
			$select2 = db_select('slt_notification', 'notif');
			$select2->addField('notif', 'id', 'id');
			//   $select->addExpression('COUNT(notif.id)');
			$select2->condition('notif.user_id', $userId);
			$select2->condition('notif.mobile', 1);
			$select2->condition('notif.viewed', 1 , '<>');
			$select2->condition('notif.soft_delete', 1,'<>');
				
			$select1 = db_select('slt_bulk_notification', 'notif');
			$select1->addField('notif', 'id', 'id');
			//   $select->addExpression('COUNT(notif.id)');
			$select1->condition('notif.user_id', $userId);
			$select1->condition('notif.mobile', 1);
			$select1->condition('notif.viewed', 1 , '<>');
			$select1->condition('notif.soft_delete', 1,'<>');
			$selectunion = $select1->union($select2,'UNION ALL');
			$select = db_select($selectunion,'noti');
			$select->addExpression('COUNT(noti.id)');

 			$notViewedNotif =  $select->execute()->fetchField();
			
				
			expDebug::dPrint("sql....verifyuser1111111...".$notViewedNotif);
				
			$result = $notViewedNotif;

			//$not_viewed = $announcementListCount + $result[0][notification_notviewed];
			
			$not_viewed = $result;
			
			return $not_viewed;
			}catch(Exception $e){
			echo $e->getMessage();
			}
	
			}
	
	/**
    * This api is used to verify the user name and password against ExpertusONE.
    * @param params stdClass 
    * @return result
    */	
	public function doLogin($params)
	{
		try
		{
			$this->connect();
			$query = 'SELECT lang.language,lang.name,lang.prefix FROM languages lang  WHERE  lang.enabled = 1';
			$enabledLanguages = $this->query($query)->fetchAll();
			$this->closeconnect();
			expDebug::dPrint('Login enabledLanguages'.print_r($enabledLanguages,true),5);
			
			$enabledLanguagesstr ='';
			for($i=0;$i<count($enabledLanguages);$i++){
				if($i!= count($enabledLanguages)-1){
					$enabledLanguagesstr.= $enabledLanguages[$i]['language'].'~~'.$enabledLanguages[$i]['name'].'~~'.$enabledLanguages[$i]['prefix'].'--';
				}else{
					$enabledLanguagesstr.= $enabledLanguages[$i]['language'].'~~'.$enabledLanguages[$i]['name'].'~~'.$enabledLanguages[$i]['prefix'];
				}
			}
			expDebug::dPrint("enabledLanguagesstrenabledLanguagesstr".$enabledLanguagesstr);
			
			$this->connect();
			$query = 'select sys.status from system sys where sys.name="exp_sp_administration_commerce"';
			$commerceEnabled = $this->query($query)->fetchAll();
			$this->closeconnect();
				
			expDebug::dPrint("commerceEnabledcommerceEnabledcommerceEnabled".$commerceEnabled[0]['status']);
			
			//code added to fetch the rating_status
			
			$this->connect();
			$query = 'select sys.status from system sys where sys.name="exp_sp_fivestar"';
			$ratingEnabled = $this->query($query)->fetchAll();
			$this->closeconnect();
			
			expDebug::dPrint("ratingEnabled_ratingEnabled_ratingEnabled_ratingEnabled".$ratingEnabled[0]['status']);
			
			$this->connect();
			$query = 'select menu.hidden from menu_links menu where menu.link_title="CATALOG"';
			$catalogEnabled = $this->query($query)->fetchAll();
			$this->closeconnect();
			expDebug::dPrint("catalogEnabledcatalogEnabledcatalogEnabled ".$catalogEnabled[0]['hidden']);
				
			
			
			$this->connect();
			$query = 'SELECT count(splt.code) as enabledCurrency FROM slt_profile_list_items splt  WHERE  splt.is_active = "Y" AND splt.attr3 = "Y" AND splt.code like "cre_sys_crn_%" ';
			$enabledCurrencies = $this->query($query)->fetchAll();
			$this->closeconnect();
			
			$enabledCurrenciesstr = $enabledCurrencies[0]['enabledCurrency'];
// 			for($i=0;$i<count($enabledCurrencies);$i++){
// 				if($i!= count($enabledCurrencies)-1){
// 					$enabledCurrenciesstr.= $enabledCurrencies[$i]['code'].'~~'.$enabledCurrencies[$i]['name'].'~~'.$enabledCurrencies[$i]['label'].'~~'.$enabledCurrencies[$i]['symbol'].'--';
// 				}else{
// 					$enabledCurrenciesstr.= $enabledCurrencies[$i]['code'].'~~'.$enabledCurrencies[$i]['name'].'~~'.$enabledCurrencies[$i]['label'].'~~'.$enabledCurrencies[$i]['symbol'];
// 				}
// 			}
				
			expDebug::dPrint("enabledCurrenciesstrenabledCurrenciesstr".$enabledCurrenciesstr);
			
			$blocksenabled = '';
			$this->connect();
			$query = 'select region,delta from block where (delta="my_skill" or delta="my_activity" or delta="most_popular" or delta="highly_rated") and theme = "expertusoneV2"';
			$blocksenabled = $this->query($query)->fetchAll();
			$this->closeconnect();
				
			expDebug::dPrint("skillenabled".print_r($blocksenabled,true));
			$blocksstr = '';
			foreach ($blocksenabled as $block){
				expDebug::dPrint("skillenabled".print_r($block,true));
				$blocksstr.= $block['delta'].'~~'.$block['region'].'-^-';
			}
			
			$fetch_points_status = "select * from system where name = 'exp_sp_admin_userpoints'";
			$qry_res  = db_query($fetch_points_status);
			$fetch_result_points = $qry_res->fetchAll();
			$blocksstr .= 'user_points~~'.$fetch_result_points[0]->status;
			expDebug::dPrint("skillenabled".$blocksstr);
			
				
			//user_hash_password
			if($params->sso == "true" || $params->sso === true){
				include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/Encryption.php";
				$enc = new Encrypt();
				
			}
			
			if($params->email != "")
			{
			if($params->sso == "true" || $params->sso === true){
			$params->email = $enc->decrypt($params->email);
			}
			$qry="SELECT p.is_manager as is_manager,p.full_name as full_name,p.email as email_id,u.status=1 as user_status,p.is_instructor as is_instructor,p.id as USERID,u.pass pass,u.name,u.uid,se.col1 roleid,p.preferred_language preferred_language,p.preferred_currency preferred_currency,splt.name preferred_currency_name FROM users u,slt_person p LEFT JOIN slt_entity_profile_mapping se  ON se.entity_id=p.id AND se.entity_type=08 LEFT JOIN slt_profile_list_items splt ON splt.attr1=p.preferred_currency where u.mail='".$params->email."' and p.email=u.mail";
			//expDebug::dPrint("sql....dologin...".$qry);
			}
			else
			{
			if($params->sso == "true" || $params->sso === true){
			$params->username = $enc->decrypt($params->username);
			}
			else
			{
				$enc = new Encrypt();
				$params->username = $enc->getAESDecryptedValue($params->username);
				$params->password = $enc->getAESDecryptedValue($params->password);
			}
		

			$qry="SELECT p.is_manager as is_manager,p.full_name as full_name,p.email as email_id,u.status=1 as user_status,p.is_instructor as is_instructor,p.id as USERID,u.pass pass,u.name,u.uid,se.col1 roleid,p.preferred_language preferred_language,p.preferred_currency preferred_currency,splt.name preferred_currency_name FROM users u,slt_person p LEFT JOIN slt_entity_profile_mapping se  ON se.entity_id=p.id AND se.entity_type=08 LEFT JOIN slt_profile_list_items splt ON splt.attr1=p.preferred_currency where u.name='".$params->username."' and p.user_name=u.name";
			
			}
		
			expDebug::dPrint("sql....verifyuser...".$qry);
			$this->connect();
			$result = $this->query($qry)->fetchAll();
			$this->closeconnect();
			
			// Used for drupal authenticate vaidation
			$form = array();
			$form_state = array();
			
			if(count($result)){
					
			$result[0]['enabledLanguages'] = $enabledLanguagesstr;
			// 			$result[0]['enabledCurrencies'] = $enabledCurrenciesstr;
			$result[0]['commerceEnabled'] = $commerceEnabled[0]['status'];
			$result[0]['ratingEnabled'] = $ratingEnabled[0]['status'];
			$result[0]['enabledCurrency'] = $enabledCurrenciesstr;
			$result[0]['enabledblocks'] = $blocksstr;
			$result[0]['catalogEnabled'] = $catalogEnabled[0]['hidden'];
				
			}
			else{
				$form_state['uid'] = "";
				//Update flood table with Ip
				user_login_custom_final_validate($form,$form_state);
			}
			
			// Check for IP blocked or not			
			
			$form_state['values']['name'] = $params->username;
			$form_state['values']['pass'] = $params->password;
			user_login_authenticate_validate($form, $form_state);
			
			expDebug::dPrint('Whats triggered = '.$form_state['flood_control_triggered']);
			

			$len=0;
			$i=0;
			$row=array();
			$len=count($result);
			
		 	for($i=0;$i<$len;$i++)
			 {
			 		$row=$result[$i];
			 		$pass=$row["pass"];
			 		$name=$row["name"];
			 		$uid=$row["uid"];
			 		//$rolename=$row["rolename"];
			 		$account=new stdClass();
 					$account->name=$name;
 					$account->uid=$uid;
 					$account->pass=$pass;
 					//$account->roleid=$roleid;
 					//$account->rolename=$rolename;
 					
 					if($form_state['flood_control_triggered'] == 'ip')
 					{
 						$form_state['uid'] = "";
 						//Update flood table with Ip
 						user_login_custom_final_validate($form,$form_state);
 						
 						$result[0]["uid"]= "";
 						$result[0]["pass"]="temp_ip_block";
 						$result[0]["USERID"]=$row["USERID"];
 						$result[0]["name"]="";
 						return $result;
 					}
 					
 					if($params->sso == "true" || $params->sso === true)
 						return $result;
 					//start of changes for sales force - by Kannank - To auto login in LMS through salesforce mobile user
 					if($params->salesforce == "true" || $params->salesforce === true)
 						return $result;
 					//end of changes for sales force - by Kannank
 					else if(user_check_password($params->password,$account)==1)
			 		{
			 			$this->connect();					 		
			 			$qry = "SELECT
			 			pass.force_change as force_change
			 			FROM password_policy_force_change pass
			 			LEFT OUTER JOIN users urs ON urs.uid = pass.uid
			 			WHERE  pass.uid = $uid
			 			AND pass.force_change = 1
			 			AND urs.status = 1";
			 			$this->connect();
			 			$resultValue = $this->query($qry)->fetchAll();
			 			$this->closeconnect();
			 			include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_people/exp_sp_administration_user/exp_sp_administration_user.inc');
			 			
// 			 			$blocked = getAuthFloodCount($uid);
			 			
			 			
			 			if(count($resultValue) && $form_state['flood_control_triggered'] != 'user')
			 			{
			 			$result[0]["uid"]= "";
			 			$result[0]["pass"]="expired";
			 			$result[0]["USERID"]=$row["USERID"]; 
			 			$result[0]["name"]="";
			 			}else if($form_state['flood_control_triggered'] == 'user')
			 			{
			 				$result[0]["uid"]= "";
			 				$result[0]["pass"]="blocked";
			 				$result[0]["USERID"]=$row["USERID"];
			 				$result[0]["name"]="";
			 			}
			 				
			 			return $result;
			 		}
			 		else
			 		{
// 			 			include_once $_SERVER["DOCUMENT_ROOT"]."/modules/user/user.module";
// 			 			include_once $_SERVER["DOCUMENT_ROOT"]."/includes/common.inc";
			 			
			 						 			
			 			$form_state['uid'] = "";
			 			$form_state['flood_control_user_identifier'] = $row["uid"].'-'.ip_address();
			 			
			 			//Update flood table with User Id and Ip
			 			user_login_custom_final_validate($form,$form_state);
			 			$result[0]["uid"]="";
			 			$result[0]["pass"]="";
			 			$result[0]["USERID"]=""; 
			 			$result[0]["name"]="";
			 			
			 			if($form_state['flood_control_triggered'] == 'user')
			 			{
			 				$result[0]["uid"]= "";
			 				$result[0]["pass"]="blocked";
			 				$result[0]["USERID"]=$row["USERID"];
			 				$result[0]["name"]="";
			 			}
			 			
			 		}
	
			 		
			 }
			 			 
			 
			//$result = $this->fetchResult();
			//$this->closeconnect();
			return $result;
		}catch(Exception $e){
				echo $e->getMessage();
		}
		
	}
	
	
	public function isUserActive($userid)
	{
		try
		{
			$qry="select u.status=1 as userstatus from users u left join slt_person p on u.name = p.user_name where p.id=$userid";
			//expDebug::dPrint("sql....verifyuser...".$qry);
			$this->connect();
			$result = $this->query($qry)->fetchAll();
			expDebug::dPrint("enabledCurrenciesstrenabledCurrenciesstr".print_r($result,true));
				
			//$result = $this->fetchResult();
			$this->closeconnect();
			return $result;
		}catch(Exception $e){
			echo $e->getMessage();
		}
	
	}
	public function ForcedPasswordChange($userid,$newpassword)
	{
		try
		{
	
			$qry="select u.uid from users u left join slt_person p on u.name = p.user_name where p.id=$userid";
			$this->connect();
			$uid = $this->query($qry)->fetchAll();
			$u_id = $uid[0]['uid'];
			$resultValue1 = reset_password_submit_mobile($u_id,$newpassword);
	
			for($k=0;$k<count($resultValue1);$k++){
	
				expDebug::dPrint(' reset_password_submit inside k value susccessfull'.print_r($resultValue1[$k],true), 4);
				if($resultValue1[$k])
				{
					expDebug::dPrint(' reset_password_submit final error_report susccessfull'.print_r($resultValue1,true), 4);
					return $resultValue1;
				}
			}
	
			expDebug::dPrint(' reset_password_submit not return susccessfull'.print_r($resultValue1,true), 4);
	
			$qry="SELECT p.force_change as force_change FROM password_policy_force_change p  WHERE  p.uid = $u_id AND p.force_change = 0";
			$this->connect();
			$resultValue = $this->query($qry)->fetchAll();
			$this->closeconnect();
			$resultcount = count($resultValue);
			expDebug::dPrint(' reset_password_submit two susccessfull'.$resultcount, 4);
	
			return $resultcount;
		}catch(Exception $e){
			echo $e->getMessage();
		}
	
	}
	
	

	public function GetCatalogSearchPrams($userid,$searchUrl)
	{
		try
		{
				
			expDebug::dPrint('$searchUrl id = ' . print_r($searchUrl,true), 4);
	
			$short_parameter = $searchUrl;
			$short_url_arr = explode('&',$short_parameter);
			expDebug::dPrint('$short_parameter id = ' . print_r($short_parameter,true), 4);
			expDebug::dPrint('$$short_url_arr id = ' . print_r($short_url_arr,true), 4);
	
	
			$selectStmt = db_select('slt_widget','widget');
			$selectStmt->addField('widget','type','widget_type');
			$selectStmt->addField('widget','mode','search_mode');
			$selectStmt->addField('widget','display_size','display_size');
			$selectStmt->addField('widget','options','catalog_display_parameters');
			$selectStmt->addField('widget','parameters','widget_parameters');
			$selectStmt->addField('widget','theme','theme');
			$selectStmt->condition('widget.url',$short_url_arr[0],'=');
			$result = $selectStmt->execute()->fetchAssoc();
			//expDebug::dPrintDBAPI(' $selectStmt SQL = ' , $selectStmt);
			//expDebug::dPrint('slt_widget id = ' . print_r($result,true), 4);
			//expDebug::dPrint('slt_widget server ' . print_r($_SERVER,true), 4);
			// echo '<pre>'; print_r($result); echo '</pre>'; die;
	
			$widget_parameters_str = "";
			if (!empty($result)){
				$widget_parameters_arr = unserialize($result['widget_parameters']);
				$widget_parameters_str = '';
				$userId = getSltpersonUserId();
				foreach ($widget_parameters_arr as $key=>$val)
				{
					if(!empty($userId) && $userId > 0){
						if($key!='language')
							$widget_parameters_str .= '&'.$key.'='.$val;
					}else{
		  		$widget_parameters_str .= '&'.$key.'='.$val;
					}
				}
			}
			return $widget_parameters_str;
	
	
	
	
		}catch(Exception $e){
			echo $e->getMessage();
		}
	
	}
	
	
	
	
	
}

