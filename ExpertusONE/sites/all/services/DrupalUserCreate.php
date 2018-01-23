<?php

require_once "DrupalBaseService.php";
include_once "Trace.php";
require_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/dao/AbstractDAO.php";
//require_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/dao/AbstractDAO.php";
require_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/commonlib/UserInfo.php";  //../commonlib/UserInfo.php";

class DrupalUserCreate extends AbstractDAO{
	private $rData;
	
	function __construct($rawData){
		$this->rData = $rawData;
		parent::__construct();
	}
	
	public function createUser(){
		try{
			expDebug::dPrint("Drupal User creation starts");
			$oDrupal = new  DrupalBaseService();  
			$oDrupal->setDrupalBaseDir();
		    $dom = new DOMDocument('1.0','UTF-8');
		    $dom->loadXML($this->rData);
		    $uname=$dom->getElementsByTagName("UserName")->item(0)->nodeValue;
		    $uname = str_replace("''","'",$uname);
		    $pass=$dom->getElementsByTagName("Password")->item(0)->nodeValue;
		    $email=$dom->getElementsByTagName("Email")->item(0)->nodeValue;
		    $notification=$dom->getElementsByTagName("Notification")->item(0)->nodeValue;
			$details = array(
		        'name' => $uname,
		        'pass' => $pass,
		        'mail' => $email,
		        'access' => 1, 
		        'status' => 1
		      );
		     
		   //Addded by Ilayaraja on 17-Jun-2011
		   //To update timezone and timezone_name fields in users table 
		   if($dom->getElementsByTagName("TimeZone")->item(0)->nodeValue!=''){
				$tzCode	= $dom->getElementsByTagName("TimeZone")->item(0)->nodeValue;
				$tzqry = " CALL slp_get_timezone('".$tzCode."'); ";
				expDebug::dPrint(" Drupal timezone name select query -->".$tzqry);
				$this->connect();
				$tzres 		= $this->query($tzqry);
				$tzresult	= $this->fetchAllResults();				
				$tzName		= $tzresult[0]->TZName;
				expDebug::dPrint("--------------Timezone attr2 Name----------------".$tzName);
	    	    $tzdate 	= date_now($tzName);
		    	$tzoffset 	= date_offset_get($tzdate);
	        	expDebug::dPrint("--------------Timezone Offset----------------".$tzoffset);
	        	$details	= array_merge($details, array('timezone'=>$tzoffset,'timezone_name'=>$tzName));
			}			
		   //End Added by Ilayaraja
		      
		   $user = user_save(null, $details);
		   $oDrupal->restoreWorkingDir();
		   expDebug::dPrint("User $uname created in Drupal successfully.");
		   $unid = new stdClass();
		    if($notification=='sendnotification')
			{
				$this->connect();
				$qry = 'select id as Id from slt_person where user_name="'.$uname.'"';
				expDebug::dPrint("Durpal user roll query :".$qry);
				$query=$this->query($qry);
				$results = $this->fetchAllResults();
				$this->closeconnect();
				$unid->NotifyType='new_user';
				$unid->UserId=$results[0]->Id;
				$unid->OrderId='';
				$unid->Email=$email;
				$unid->Password=$pass;
				$this->insertLearnerNotification($unid);
			}
		    return "";
		}catch(Exception $e){
        	throw new SoapFault("SPLMS",$e->getMessage());
        }
	}

	/*
	 * Added createUser ReST API Version.
	 */
	public function createUserByRestAPI(){
		try{
			expDebug::dPrint("Drupal User creation starts by rest");
			$oDrupal = new  DrupalBaseService();  
			$oDrupal->setDrupalBaseDir();
		    //$dom = new DOMDocument('1.0','UTF-8');
		    //$dom->loadXML($this->rData);
		    $uname=$_REQUEST["UserName"];//$dom->getElementsByTagName("UserName")->item(0)->nodeValue;
		    $pass=$_REQUEST["Password"];//$dom->getElementsByTagName("Password")->item(0)->nodeValue;
		    $email=$_REQUEST["Email"];//$dom->getElementsByTagName("Email")->item(0)->nodeValue;
		    $notification=$_REQUEST["Notification"];//$dom->getElementsByTagName("Notification")->item(0)->nodeValue;
			$details = array(
		        'name' => $uname,
		        'pass' => $pass,
		        'mail' => $email,
		        'access' => 1, 
		        'status' => 1
		      );
		     
		   //Addded by Ilayaraja on 17-Jun-2011
		   //To update timezone and timezone_name fields in users table 
			$timezone=$_REQUEST["TimeZone"];	   
		   if($timezone!=''){
				$tzCode	= $timezone;//$dom->getElementsByTagName("TimeZone")->item(0)->nodeValue;
				$tzqry = " CALL slp_get_timezone('".$tzCode."'); ";
				expDebug::dPrint(" Drupal timezone name select query -->".$tzqry);
				$this->connect();
				$tzres 		= $this->query($tzqry);
				$tzresult	= $this->fetchAllResults();				
				$tzName		= $tzresult[0]->TZName;
				expDebug::dPrint("--------------Timezone attr2 Name----------------".$tzName);
	    	    $tzdate 	= date_now($tzName);
		    	$tzoffset 	= date_offset_get($tzdate);
	        	expDebug::dPrint("--------------Timezone Offset----------------".$tzoffset);
	        	$details	= array_merge($details, array('timezone'=>$tzoffset,'timezone_name'=>$tzName));
			}			
		   //End Added by Ilayaraja

		   $user = user_save(null, $details);
		   $oDrupal->restoreWorkingDir();
		   expDebug::dPrint("User $uname created in Drupal successfully.");
		   $unid = new stdClass();
		    if($notification=='sendnotification')
			{
				$this->connect();
				$qry = 'select id as Id from slt_person where user_name="'.$uname.'"';
				expDebug::dPrint("Durpal user roll query :".$qry);
				$query=$this->query($qry);
				$results = $this->fetchAllResults();
				$this->closeconnect();
				$unid->NotifyType='new_user';
				$unid->UserId=$results[0]->Id;
				$unid->OrderId='';
				$unid->Email=$email;
				$unid->Password=$pass;
				$this->insertLearnerNotification($unid);
			}
		    return "";
		}catch(Exception $e){
        	throw new SoapFault("SPLMS",$e->getMessage());
        }
	}
	
	/*
	 * Added createUser ReST API Version.
	 */
	public function createUserByLDAP(){
		try{
			expDebug::dPrint(" ldap Drupal user creation and ");
			//User creation invoked by LDAP server, this method will trigger the notification.
			
		    $notification=$_REQUEST["Notification"];//$dom->getElementsByTagName("Notification")->item(0)->nodeValue;
		    $uname=$_REQUEST["UserName"];//$dom->getElementsByTagName("UserName")->item(0)->nodeValue;
		    $pass=$_REQUEST["Password"];//$dom->getElementsByTagName("Password")->item(0)->nodeValue;
		    $email=$_REQUEST["Email"];//$dom->getElementsByTagName("Email")->item(0)->nodeValue;
			$details = array(
		        'name' => $uname,
		        'pass' => $pass,
		        'mail' => $email,
		        'access' => 1, 
		        'status' => 1
		      );
		     
		   //Addded by Ilayaraja on 17-Jun-2011
		   //To update timezone and timezone_name fields in users table 
			$timezone=$_REQUEST["timezone"];	   
		   if($timezone!=''){
				$tzCode	= $timezone;//$dom->getElementsByTagName("TimeZone")->item(0)->nodeValue;
				$tzqry = " CALL slp_get_timezone('".$tzCode."'); ";
				expDebug::dPrint(" ldap Drupal timezone name select query -->".$tzqry);
				$this->connect();
				$tzres 		= $this->query($tzqry);
				$tzresult	= $this->fetchAllResults();				
				$tzName		= $tzresult[0]->TZName;
				expDebug::dPrint("--------------Timezone attr2 Name----------------".$tzName);
	    	    $tzdate 	= date_now($tzName);
		    	$tzoffset 	= date_offset_get($tzdate);
	        	expDebug::dPrint("--------------Timezone Offset----------------".$tzoffset);
	        	$details	= array_merge($details, array('timezone'=>$tzoffset,'timezone_name'=>$tzName));
			}			
		   //End Added by Ilayaraja
		      
		   $unid = new stdClass();
		    if($notification=='sendnotification')
			{
				$this->connect();
				$qry = 'select id as Id from slt_person where user_name="'.$uname.'"';
				expDebug::dPrint(" ldap Durpal user roll query :".$qry);
				$query=$this->query($qry);
				$results = $this->fetchAllResults();
				$this->closeconnect();
				$unid->NotifyType='new_user';
				$unid->UserId=$results[0]->Id;
				$unid->OrderId='';
				$unid->Email=$email;
				$unid->Password=$pass;
				$this->insertLearnerNotification($unid);
			}
		    return "";
		}catch(Exception $e){
        	throw new SoapFault("SPLMS",$e->getMessage());
        }
	}
	
	
	public function insertLearnerNotification($inObj){
	 	$oUserInfo = new UserInfo();
	    $vLoggedUserId=$oUserInfo->getLoggedInUserId();
		if(!isset($vLoggedUserId) || $vLoggedUserId == ''){
			$vLoggedUserId = 1;
		}
		
		try{			
			$query = "CALL slp_lnr_notification_ins('".$inObj->NotifyType."','".$inObj->UserId."','".$inObj->OrderId."','".$inObj->Email."','".$inObj->Password."','".$vLoggedUserId."');";
			expDebug::dPrint("NotificationDAO insertLearnerNotification ".$query);
			$this->connect();
			$this->query($query);
			//$results = $this->fetchAllResults();
			return '';
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	
	/*
	 * Added updateUser ReST API Version.
	 */
	public function updateUserByRestAPI(){
		try{
			expDebug::dPrint("Drupal User update starts by REST");
		    $unid	= $_REQUEST["Id"];
			$uname	= $_REQUEST["UserName"];
			$pass	= $_REQUEST["Password"];
			$email	= $_REQUEST["Email"];
			$status	= $_REQUEST["Status"];
			$assignedRoles	=$_REQUEST["AssignedRoles"]; 
			expDebug::dPrint("roleids :");
			expDebug::dPrint($assignedRoles);
			/*$query = "call slp_user_role_sel('".$unid."',null);";
			expDebug::dPrint("Durpal user roll query :".$query);
			$this->connect();
			$res1 = $this->query($query);
			$result1=$this->fetchAllResults();
			$this->closeconnect();
			$roles = array();
			for($i=0;$i<sizeOf($result1);$i++){
				expDebug::dPrint("Role value:".$result1[$i]->rid);
				$roles[$result1[$i]->rid]=$result1[$i]->rid;
			}*/
			$xroles	= array();
			$roles	= array();
			if($assignedRoles!='' && $assignedRoles!='undefined')
				$xroles	= explode("|",$assignedRoles);
			foreach($xroles as $val){
				$roles[$val]=$val;
				expDebug::dPrint("Role value : $val ");
			}
			$qry = "call slp_drupal_user_sel(null,'".addslashes($uname)."');";
			expDebug::dPrint(" Drupal user select query -->".$qry);
			$this->connect();
			$res = $this->query($qry);
			$result=$this->fetchAllResults();
			$this->closeconnect();
		
			$details = array('access' => 1);
		  if(isset($uname) && $uname!=null && $uname!=''){
				$details=array_merge($details,array('name'=>$uname));
			}
			if(isset($pass) && $pass!=null && $pass!=''){
				$details=array_merge($details,array('pass'=>$pass));
			}
			if(isset($status) && $status!=null && $status!=''){
				if($status=='Active' || $status=='cre_usr_sts_atv'){
					$status=1;
				}else{
					$status=0;
				}
				$details=array_merge($details,array('status'=>$status));
			}
			if(isset($email) && $email!=null && $email!=''){
				$details=array_merge($details,array('mail'=>$email));
			}
			if(sizeOf($roles)>0){
				$details=array_merge($details,array('roles'=>$roles));
			}
	    $oDrupal = new  DrupalBaseService();  
			$oDrupal->setDrupalBaseDir();
			
		   //Addded by Ilayaraja on 17-Jun-2011
		   //To update timezone and timezone_name fields in users table
		   $timeZone=$_REQUEST["TimeZone"];
			if($timeZone!="")
			{
				$tzCode	= $timeZone;//$dom->getElementsByTagName("TimeZone")->item(0)->nodeValue;
				$tzqry = " CALL slp_get_timezone('".$tzCode."'); ";
				expDebug::dPrint(" Drupal timezone name select query -->".$tzqry);
				$this->connect();
				$tzres 		= $this->query($tzqry);
				$tzresult	= $this->fetchAllResults();				
				$tzName		= $tzresult[0]->TZName;
				expDebug::dPrint("--------------Timezone attr2 Name----------------".$tzName);
	    	    $tzdate 	= date_now($tzName);
		    	$tzoffset 	= date_offset_get($tzdate);
	        	expDebug::dPrint("--------------Timezone Offset----------------".$tzoffset);
	        	$details	= array_merge($details, array('timezone'=>$tzoffset,'timezone_name'=>$tzName));
			}			
			//End Added by Ilayaraja
			
		   // $user = user_save((object) array('uid' => $result[0]->uid), $details);
		   $account = user_load($result[0]->uid);
		    $user = user_save($account, $details);
		    $oDrupal->restoreWorkingDir();
		    expDebug::dPrint("User $uname updated in Drupal successfully.");
		    return "";
		}catch(Exception $e){
        	throw new SoapFault("SPLMS",$e->getMessage());
        }
	}
	
	
	public function updateUser(){
		try{
			expDebug::dPrint("Drupal User update starts");
			$dom = new DOMDocument('1.0','UTF-8');
		    $dom->loadXML($this->rData);
		    $unid	= $dom->getElementsByTagName("Id")->item(0)->nodeValue;
			$uname	= $dom->getElementsByTagName("UserName")->item(0)->nodeValue;
			$uname = str_replace("''","'",$uname);
			$pass	= $dom->getElementsByTagName("Password")->item(0)->nodeValue;
			$email	= $dom->getElementsByTagName("Email")->item(0)->nodeValue;
			$status	= $dom->getElementsByTagName("Status")->item(0)->nodeValue;
			$assignedRoles	= $dom->getElementsByTagName("AssignedRoles")->item(0)->nodeValue;
			expDebug::dPrint("roleids :");
			expDebug::dPrint($assignedRoles);
			/*$query = "call slp_user_role_sel('".$unid."',null);";
			expDebug::dPrint("Durpal user roll query :".$query);
			$this->connect();
			$res1 = $this->query($query);
			$result1=$this->fetchAllResults();
			$this->closeconnect();
			$roles = array();
			for($i=0;$i<sizeOf($result1);$i++){
				expDebug::dPrint("Role value:".$result1[$i]->rid);
				$roles[$result1[$i]->rid]=$result1[$i]->rid;
			}*/
			$xroles	= array();
			$roles	= array();
			if($assignedRoles!='' && $assignedRoles!='undefined')
				$xroles	= explode("|",$assignedRoles);
			foreach($xroles as $val){
				$roles[$val]=$val;
				expDebug::dPrint("Role value : $val ");
			}
			$qry = 'call slp_drupal_user_sel(null,"'.$uname.'");';
			expDebug::dPrint(" Drupal user select query -->".$qry);
			$this->connect();
			$res = $this->query($qry);
			$result=$this->fetchAllResults();
			$this->closeconnect();
						
			if($status=='Active' || $status=='cre_usr_sts_atv'){
				$status=1;
			}else{
				$status=0;
			}
			if($pass!=null && $pass!='undefined' && $pass!=''){		
				$details = array(
			        'name' => $uname,
			        'pass' => $pass,
			        'mail' => $email,
			        'access' => 1, 
			        'status' => $status,
			       	'roles' =>$roles
			      );
			}else{
				$details = array(
			        'name' => $uname,
			        'mail' => $email,
			        'access' => 1, 
			        'status' => $status,
			       	'roles' =>$roles
			      );
			}
		    $oDrupal = new  DrupalBaseService();  
			$oDrupal->setDrupalBaseDir();
			
		   //Addded by Ilayaraja on 17-Jun-2011
		   //To update timezone and timezone_name fields in users table 
			if($dom->getElementsByTagName("TimeZone")->item(0)->nodeValue!=''){
				$tzCode	= $dom->getElementsByTagName("TimeZone")->item(0)->nodeValue;
				$tzqry = " CALL slp_get_timezone('".$tzCode."'); ";
				expDebug::dPrint(" Drupal timezone name select query -->".$tzqry);
				$this->connect();
				$tzres 		= $this->query($tzqry);
				$tzresult	= $this->fetchAllResults();				
				$tzName		= $tzresult[0]->TZName;
				expDebug::dPrint("--------------Timezone attr2 Name----------------".$tzName);
	    	    $tzdate 	= date_now($tzName);
		    	$tzoffset 	= date_offset_get($tzdate);
	        	expDebug::dPrint("--------------Timezone Offset----------------".$tzoffset);
	        	$details	= array_merge($details, array('timezone'=>$tzoffset,'timezone_name'=>$tzName));
			}			
			//End Added by Ilayaraja
			
		   // $user = user_save((object) array('uid' => $result[0]->uid), $details);
		   $account = user_load($result[0]->uid);
		    $user = user_save($account, $details);
		    $oDrupal->restoreWorkingDir();
		    expDebug::dPrint("User $uname updated in Drupal successfully.");
		    return "";
		}catch(Exception $e){
        	throw new SoapFault("SPLMS",$e->getMessage());
        }
	}
}
expDebug::dPrint('Drupal User Create');
//Added for ReST API.

if(isset($_GET["mode"]) && ($_GET["mode"]=="ldap_insert") )
{
	global $rawData;
	$obj = new DrupalUserCreate($rawData);
	if($_GET["mode"]=="ldap_insert" )
		$obj->createUserByLDAP();
	
}
else if((isset($_GET["mode"]) || isset($_REQUEST["mode"])) && ($_REQUEST["mode"]=="restapi_insert"  || $_REQUEST["mode"]=="restapi_update" || $_GET["mode"]=="restapi_insert"  || $_GET["mode"]=="restapi_update") )
{
	global $rawData;
  expDebug::dPrint('Drupal User Create in restapi_insert');
	$obj = new DrupalUserCreate($rawData);
	if($_GET["mode"]=="restapi_insert" || $_REQUEST["mode"]=="restapi_insert")
		$obj->createUserByRestAPI();
	else if($_GET["mode"]=="restapi_update" || $_REQUEST["mode"]=="restapi_update")
		$obj->updateUserByRestAPI();
		ob_end_clean();
		ob_start();

}
else
{
	global $rawData;
	expDebug::dPrint('Drupal User Create in else');
	//$rawData = isset($HTTP_RAW_POST_DATA)?'':$HTTP_RAW_POST_DATA;
	if($rawData==null || $rawData=='')
		$rawData=file_get_contents("php://input");
		
	$obj = new DrupalUserCreate($rawData);
	if((stripos($rawData,'InsertUser')!='' && stripos($rawData,'InsertUser')>0) || (stripos($rawData,'UserCreate')!='' && stripos($rawData,'UserCreate')>0)){
		$obj->createUser();
	}else if ((stripos($rawData,'UpdateUser')!='' && stripos($rawData,'UpdateUser')>0) || (stripos($rawData,'UpdateUserPassword')!='' && stripos($rawData,'UpdateUserPassword')>0)){
	  	$obj->updateUser();
	}
}
?>