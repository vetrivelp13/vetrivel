<?php
	
/**
    * It is an base class for all the rest apis. It contains common method implementations which used across the apis.
    * @author Sureshkumar.v
*/
include_once $_SERVER["DOCUMENT_ROOT"]."/apis/core/init.php";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/commonlib/UserInfo.php";

//include_once $_SERVER["DOCUMENT_ROOT"]."/apis/core/GlobalUtil.php";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/Trace.php";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/FPM_Includes.php";
include_once $_SERVER["DOCUMENT_ROOT"]."/apis/core/error_messages.php";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/dao/AbstractDAO.php";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/exp_sp_core.module";

class BaseAPI
{
	
	/**
	 *It is used to store the current execution api class name which will be used for logging purpose.
	 *@access private
	 *@var string
	 *
	 */
	private $className="";

	/**
	 *It is used to store the client who is calling this api either Mobile client or Web Browser.
	 *@access private
	 *@var string
	 *
	 */
	private $useragent="";

	/**
	 *It is used to store the user object.
	 *@access private
	 *@var string
	 *
	 */
	private $userData="";
	
	/**
	 *It is used to store all the incoming parameters _GET values as well as _POST values.
	 *@access private
	 *@var string
	 *
	 */
	private $params="";

	/**
	 *if any error occured while fetching the data from DAO, it stores the error data.
	 *@access private
	 *@var stdClass
	 *
	 */
	private $errorObject=null;
	protected $errorResponseFormat = 0;
	public function getClientIP(){
  	  $ip="";
  	  if (getenv("HTTP_CLIENT_IP"))
  	    $ip = getenv("HTTP_CLIENT_IP");
  	  else if(getenv("HTTP_X_FORWARDED_FOR"))
  	    $ip = getenv("HTTP_X_FORWARDED_FOR");
  	  else if(getenv("REMOTE_ADDR"))
  	    $ip = getenv("REMOTE_ADDR");
  	  else
  	    $ip = "UNKNOWN";
  	  return $ip;
    } 
    
	public function checkThrottleRange($CE){
      include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/GlobalUtil.php";
      $util=new GlobalUtil();
      $config=$util->getConfig();
			$throttle = 0;
      $throttlingByEachAPI = isset($config["throttling_by_each_api"]) ? $config["throttling_by_each_api"] : 0;
      $requestPerMinute = isset($config["max_api_request_per_minute"]) ? $config["max_api_request_per_minute"] : 50;
      $apiname = isset($_POST['apiname'])?$_POST['apiname']:$_GET['apiname'];
      $reqHeaders = getallheaders();
      $access_token = isset($reqHeaders['Authorization']) ? $reqHeaders['Authorization'] : $_REQUEST['access_token'];
      if($throttlingByEachAPI == 1){
      	$throttle = $CE->getThrottle();
      	$requestPerMinute = ($throttle > 0) ? $throttle : $requestPerMinute;
      }
//       expDebug::dPrint("Throttle test --- `".$throttlingByEachAPI." --- ".$requestPerMinute." ---- ".$apiname."--".$throttle,5);
	    
      $diffTime = time() - 60;
	    $select = db_select('slt_api_log', 'apilog');
	    $select->addExpression('COUNT(1)', 'cnt');
	    $select->condition('apilog.ip_address', $this->getClientIP(), '=');
	    if($throttlingByEachAPI == 1){
	      $select->condition('apilog.api_name', $apiname, '=');
	    }
	    $select->condition('apilog.last_access', $diffTime, '>=');
// 	    expDebug::dPrintDBAPI("API Throttle check query -- ",$select);
	    $numLog = $select->execute()->fetchField();
	    
	    if($numLog >= $requestPerMinute){
          return false;
	    }
	    else{
	      $insertStmt = db_insert('slt_api_log');
        $custom  = NULL;
        $fields = array(          
  		  'ip_address'   => $this->getClientIP(),
  		  'api_name'     => $apiname,
  		  'last_access'  => time(),
          'access_token' => $access_token,	
          'custom0'      => $custom,
          'custom1'      => $custom,
          'custom2'      => $custom,
          'custom3'      => $custom,
          'custom4'      => $custom   
          );
        $insertStmt->fields($fields);
        $insertStmt->execute();
	      return true;
	    }
	}
	
	/**
	* Constructor sets up. It will verify the incoming request is signed or not. If not it throws
	* Unauthorized error to the caller.
    */	
	public function __construct()
	{
		include_once $_SERVER["DOCUMENT_ROOT"]."/apis/oauth2/Resource.php";
		$this->errorObject=new stdClass();
		$authorized = false;
		$server = new ResourceValidate();
		$exceptionobj=null;
		ini_set('display_errors',0);
		try
		{
			include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/GlobalUtil.php";
			$util=new GlobalUtil();
			$config=$util->getConfig();
			$this->errorResponseFormat = isset($config['api_error_response_format']) && trim($config['api_error_response_format']) == "1" ? 1 :0;
			$apiName 	= trim($_REQUEST['apiname']);
			$userid		= trim($_REQUEST["userid"]);
			if (!is_numeric($userid) || $userid == '') { // validate userid must be numeric
				throw new Exception("Userid is invalid"); 
			}
			if (!is_string($apiName) || $apiName == '') { // validate apiname must be string
				throw new Exception("Apiname is invalid"); 
					
			}
			$CE = new CoreEngine();
		    $CE->prepareDetals($apiName);
		    $pre_verify = $this->checkThrottleRange($CE);
		    
		    $userDetails = $this->getUserDetails($userid);
		    $userStatus = $userDetails->cnt;
		   
			// expDebug::dPrint("userStatus ".print_r($userStatus, 1),1);
		    $apiCatagory = strtolower($CE->getCatagory());
		    $adminPerm = 1;		     
           	if($apiCatagory == 'adminapi') {
           	  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
              $drupaluid    = getDrupalIdOfUser($userid);
              $account      = user_load($drupaluid,TRUE);
              $adminPerm    = user_access('Admin API Perm',$account);
              if($userid != 1 && $userStatus == 1) {
              	$privilege_parameter = $CE->getPrivilegeParams();
              	$entity_type_parameter = $CE->getEntityTypeParams();
              	if(!empty($privilege_parameter)) {
              		$privilege_check = apiUserPrivilegeCheck($privilege_parameter, $userid, $entity_type_parameter);
              		if($privilege_check == false) {
              			throw new Exception("user dont have this privilege");
              		}
              	}
			  }
		    }
		    $signedObj = $server->validate('E1.API');
			if (($pre_verify) && $signedObj->oauth_signed   && ($userStatus) && ($adminPerm))
			{
				if($signedObj->userid_from_access == $userid)
				{
					$authorized = true;				
					$userName= $userDetails->user_name;
					$fullName= $userDetails->full_name;
					$learnerInfo=$userid."##".$userName."##".$fullName."##";
					$enc11 = new Encrypt();
	        		$deVal1 = $enc11->encrypt($learnerInfo);
	        		$_COOKIE["SPLearnerInfo"]=$deVal1;
				}else {
					throw new Exception("Access token does not match with userid");
				}
			
			}
		}
		catch (Exception $e)
		{
			$exceptionobj=$e;
			// expDebug::dPrint('exception '.$e->getMessage());
		}	
				
		if (!$authorized || $pre_verify == false || empty($userStatus) || !($adminPerm) || empty($apiName))
		{
			$outxml="";
			$errobj=new stdClass();
			
			if($exceptionobj!=null)
				$errobj->errormsg=$exceptionobj->getMessage();
			// expDebug::dPrint('exception errobj'.print_r($errobj, 1));
			
			if(isset($errobj->errormsg) && $errobj->errormsg == "Invalid token or scope")
			{
				$errobj->errcode="L_009";
			}
			else
				$errobj->errcode="L_004";
			
			if($errobj->errormsg == 'user dont have this privilege') {
				$errobj->errcode="L_013";
			}
			
		  	if(empty($userStatus)){
			  	if(isset($errobj->errormsg) && $errobj->errormsg == "Userid is invalid")
			  	{
			  		$errobj->errcode="L_010";
			  	}
			  	else
				  $errobj->errcode="L_007";
			}
		  	else if(!($adminPerm)){
			  $errobj->errcode="L_008";
			}
			else if($pre_verify == false){
			  $errobj->errcode="L_006";
			}
			
			
			if (isset($errobj->errormsg) && $errobj->errormsg == 'Apiname is invalid') { // API name is missing 
				$errobj->errcode="L_011";
			}
			if($exceptionobj!=null)
				$errobj->errormsg=$exceptionobj->getMessage();
			else
				$errobj->errormsg="";
			
			// expDebug::dPrint('before header '.print_r($errobj, 1));
			if (isset($errobj->errormsg) && ($errobj->errormsg == 'Apiname is invalid' || $errobj->errormsg == 'user dont have this privilege')) {
				header("HTTP/1.1 200 OK");
			}
			else {
				header('HTTP/1.1 401 Unauthorized');
			}
			header('Content-Type: application/xml');
			
			if($this->errorResponseFormat == 1) {
				$formattedErr=ErrorMessages::getErrorMessageNew($errobj);
				$outxml.="<?xml version='1.0' encoding='UTF-8' ?>".$this->formatErrorObject($formattedErr, "xml");
			}
			else {
				$formattedErr=ErrorMessages::getErrorMessage($errobj);
				/*$outxml.="<?xml version='1.0' encoding='UTF-8' ?><error><error_code>".$formattedErr->code."</error_code><short_msg>".$formattedErr->type."</short_msg><long_msg>".$formattedErr->message."</long_msg><corrective_solution>".$formattedErr->correctivesolution."</corrective_solution></error>";*/
				$outxml.="<error><error_code>".$formattedErr->code."</error_code><short_msg>".$formattedErr->shortmessage."</short_msg><long_msg>".$formattedErr->longmessage."</long_msg><corrective_solution></corrective_solution></error>";
			}
			$this->log("Error occured");
// 			expDebug::dPrint("ERROR : OAuth Verification Failed -->> ".$outxml,1);
			echo $outxml;die();//"OAuth Verification Failed: " . $e->getCode()." m ".$e->getMessage();
		}
		
		$this->className=get_class($this);
		$this->useragent=$this->getBrowser();
		$this->userData=new stdClass();
		$this->params=new stdClass();
		$this->log("Constructor called...");
		$this->log("Authorized status....".$authorized);
		error_reporting(E_ALL);    
	}
	
	public function sendErrorNew($error)
	{
		header('HTTP/1.1 401 Unauthorized');
		header('Content-Type: application/xml');
		$formattedErr=ErrorMessages::getErrorMessageNew($error);
		$xmlerror = $this->formatErrorObject($formattedErr, "xml");
		$outxml ="<?xml version='1.0' encoding='UTF-8' ?>".$xmlerror;
		echo $outxml;
		$this->log("Error occured");
		die;//"OAuth Verification Failed: " . $e->getCode()." m ".$e->getMessage();
		
	}
	public function sendError($short_msg,$long_msg)
	{
		header('HTTP/1.1 401 Unauthorized');
		header('Content-Type: application/xml');
		$outxml.="<?xml version='1.0' encoding='UTF-8' ?><error><error_code>XSS_SQL_INJECTION_ERROR</error_code><short_msg>".$short_msg."</short_msg><long_msg>".$long_msg."</long_msg><corrective_solution>Use valid data</corrective_solution></error>";
		$this->log("Error occured");
		echo $outxml;die;//"OAuth Verification Failed: " . $e->getCode()." m ".$e->getMessage();
		
	}
	
	
	/**
    * This api is used to the error code and error details to the errorObject.
    * @param Exception object 
    * @return stdClass
    */
	public function setErrorObject($exception_obj)
	{
		$this->errorObject=new stdClass();
		$this->errorObject->isError="true";
		if($this->errorResponseFormat == 1) {
		$this->errorObject->errors = array();
		if($exception_obj instanceof BaseAPIException) {	//multiple validation errors
			/* $errors = $exception_obj->getErrors();
			foreach ($errors as $error) {
				
			} */
		  $this->errorObject->errors = $exception_obj->getErrors();
// 		  expDebug::dPrint('setErrorObject multiple');
		}
		else {
		  $error = new stdClass();
		  $error->errcode = $exception_obj->getCode();
		  $error->errormsg = $exception_obj->getMessage();
		  $this->errorObject->errors = array($error);
		}
		}
		else {
		  $this->errorObject->errcode=$exception_obj->getCode();
		  $this->errorObject->errormsg=$exception_obj->getMessage();
// 		  expDebug::dPrint("set error".print_r($this->errorObject,1),5);
		}
	}
	
	public function getErrorObject()
	{
		return $this->errorObject;
	}
	/*
    * Returns error code.
    * @param none 
    * @return int
    */
	public function getErrorCode()
	{
		return $this->errorObject->error_code;
	}
	
	/*
    * Returns the error message which thrown by DAO layer.
    * @param none 
    * @return string
    */
	public function getErrorMessage()
	{
		return $this->errorObject->error_message;
	}
	
	/*
    * It is used to verify the error is exists or not.
    * @param none 
    * @return boolean
    */
	public function isErrorExist()
	{
		
		if(property_exists($this->errorObject,"isError")==true && $this->errorObject->isError=="true")
			return true;
		else
			return false;
	}
	
	
	
	/*
    * It is used to collect all the $_GET and $_POST parameter  values into params object in this class.
    * @param none 
    * @return stdClass
    */
	public function getParams()
	{
		
		$this->params=new stdClass();
		foreach($_GET as $key => $value)
		{
			$this->params->$key=$value;
			//echo 'Key = ' . $key . '<br />';
			//echo 'Value= ' . $value;
		}
		foreach($_POST as $key => $value)
		{
			$this->params->$key=$value;
			//echo 'Key = ' . $key . '<br />';
			//echo 'Value= ' . $value;
		}
		
// 		expDebug::dPrint("get params from baseapi");
// 		expDebug::dPrint( $this->params);
		return $this->params;
		
		
		
	}
	public function getParamsWithSafe()
	{
		$this->params=new stdClass();
		foreach($_GET as $key => $value)
		{
			$safe_val = $this->safe($key,$value);
			$this->params->$key=$safe_val;
			$_GET[$key] = $safe_val;
			//echo 'Key = ' . $key . '<br />';
			//echo 'Value= ' . $value;
		}
		foreach($_POST as $key => $value)
		{
			//$this->params->$key=$this->safe($value);
			if($value != ''){
				$safe_val = $this->safe($key,$value);
				$this->params->$key=$safe_val;
				$_POST[$key] = $safe_val;
			}
				
			//echo 'Key = ' . $key . '<br />';
			//echo 'Value= ' . $value;
		}
		
// 		expDebug::dPrint("get params from baseapi..2");
// 		expDebug::dPrint( $this->params);
		return $this->params;
	}
	
	/* Prevent sql injection if any */
	public function safe($key,$value){
		
			$val = $this->expertus_xss_validate($key,$value);
			// return escape_string($val); // commented for 0065878
			return $val;
		
	}
	
	public function is_htmleditor_fields($fieldname)
	{
		$this->CE = new CoreEngine(); // #50172: HTML tags not accepted in course description
		$this->RE = new RewriteEngine();
		$this->CE->prepareDetals($this->RE->getAPIName());
		$htmltags = $this->CE->getHtmltags();
		
		if($fieldname == "description" || $fieldname == "crs_short_description" ||  $fieldname == "short_description" || 
			$fieldname == "shortdesc" || $fieldname == "shortdescription" || $fieldname == "survey_description" || ($htmltags && $fieldname == "value" ))
			return true;
		else
			return false;
	}
   public function getHtmlEventPatternAPI($op=0){
		$html_events = 'onafterprint|onbeforeprint|onbeforeunload|onerror|onhashchange|onload|onmessage|onoffline|ononline|onpagehide|onpageshow|onpopstate|onresize|onstorage|onunload|ondblclick|ondrag|ondragend|ondragenter|ondragleave|ondragover|ondragstart|ondrop|onmousedown|onmousemove|onmouseout|onmouseover|onmouseup|onmousewheel|onscroll|onwheel|onblur|onchange|oncontextmenu|onfocus|oninput|oninvalid|onreset|onsearch|onselect|onsubmit|onkeydown|onkeypress|onkeyup|onclick|oncopy|oncut|onpaste|onabort|oncanplay|oncanplaythrough|oncuechange|ondurationchange|onemptied|onended|onerror|onloadeddata|onloadedmetadata|onloadstart|onpause|onplay|onplaying|onprogress|onratechange|onseeked|onseeking|onstalled|onsuspend|ontimeupdate|onvolumechange|onwaiting|onerror|onshow|ontoggle|onerror';
	
		$pattern = "/(<[A-Z][A-Z0-9]*[^>]*)($html_events)([\s]*=[\s]*)('[^>]*'|\"[^>]*\"|[^>]*)([^>]*>)/i";
		
		$pattern2 = "/($html_events)([\s]*=[\s]*)/i";
		return ($op==1) ? $pattern : $pattern2;
	}
	public function cleanEventsFormValueAPI(&$string)
	{
		//Remove the html events in description field
		try{
			$pattern = $this->getHtmlEventPatternAPI(1);
			$replacement = '$1$5';
			while( preg_match($pattern, $string) ){
				$string = preg_replace($pattern, $replacement, $string);
			}
			expDebug::dPrint("cleanEventsFormValue ".$string,4);
			$string = preg_replace('#</*(?:applet|b(?:ase|gsound|link)|embed|frame(?:set)?|i(?:frame|layer)|l(?:ayer|ink)|meta|object|s(?:cript)|title|xml|iframe|confirm\(|alert\()[^>]*+>#i', '', $string);
			return $string;
		}catch (Exception $ex) {
			watchdog_exception('cleanEventsFormValue', $ex);
			expertusErrorThrow($ex);
		}
	}
	/**
	 * Form validation for expertus_xss_validate().
	 *
	 */
	public function expertus_xss_validate($fieldname,$fieldvalue) {
		try{
// 			expDebug::dPrint("xss_validate $fieldname ".$fieldname,4);
// 			expDebug::dPrint("xss_validate $fieldvalue ".$fieldvalue,4);
				$pattern = $this->getHtmlEventPatternAPI();
  				$replacement = '$1$5';
				if(!$this->is_htmleditor_fields($fieldname))
				{
				//	$terms =array('(?i)<\s*script','%2fscript','(?i)<\s*img','(?i)<\s*b','(?i)<\s*input','(?i)<\s*a','(?i)<\s*body','(?i)<\s*meta','(?i)<\s*\?');
					$terms =array('(?i)<\s*applet','(?i)<\s*base','(?i)<\s*bgsound','(?i)<\s*big','(?i)<\s*blink','(?i)<\s*blockquote','(?i)<\s*br','(?i)<\s*caption','(?i)<\s*center','(?i)<
							\s*cite','(?i)<\s*code','(?i)<\s*del','(?i)<\s*dir','(?i)<\s*div','(?i)<\s*embed','(?i)<\s*font',
							'(?i)<\s*form','(?i)<\s*frame','(?i)<\s*frameset','(?i)<\s*h','(?i)<\s*head','(?i)<\s*hr','(?i)<\s*html','(?i)<\s*i','(?i)<\s*link','(?i)<\s*marquee','(?i)<
							\s*menu','(?i)<\s*note','(?i)<\s*ol','(?i)<\s*p','(?i)<\s*param','(?i)<\s*pre','(?i)<\s*q','(?i)<\s*select','(?i)<\s*small','(?i)<\s*strike','(?i)<\s*stron
							g','(?i)<\s*sub','(?i)<\s*table','(?i)<\s*tbody','(?i)<\s*td','(?i)<\s*textarea','(?i)<\s*th','(?i)<\s*thead','(?i)<\s*title','(?i)<\s*tr','(?i)<\s*ul','(?i)<\s*var',
							'(?i)<\s*script','(?i)<\s*img','(?i)<\s*b','(?i)<\s*input','(?i)<\s*a','(?i)<\s*body','(?i)<\s*meta','(?i)<\s*iframe','(?i)<\s*\?');
					$result = array();
					
					foreach($terms as $term){
						if(!count($result)){
							$result = preg_filter('~' . $term . '~','$0', $fieldvalue);
						}else{
							break;
						}
					}
					while( preg_match($pattern, $fieldvalue) ){
						$fieldvalue = preg_replace($pattern, $replacement, $fieldvalue);
					}
					if(count($result))
					{
						if($this->errorResponseFormat == 1) {
						$errobj = new stdClass();
						$errobj->errcode = '6';
						$errobj->errorfield = $fieldname;
						$errobj->errormsg = $fieldname." contains invalid text. Given data is having possibility of cross site scripting or sql injection.";
						$this->sendErrorNew($errobj);
						}
						else {
						$this->sendError($fieldname." contains invalid text","Given data is having possibility of cross site scripting or sql injection.");
							return $fieldname." contains invalid text";
						}
					}
					else
					{
						$fieldvalue = $this->xss_clean($fieldvalue);
					}
				}
				else{
					$fieldvalueCleanEvent=$this->cleanEventsFormValueAPI($fieldvalue);//Remove the html events in description field */
					$fieldvalue = strip_tags($fieldvalueCleanEvent,'<em><span><p><div><a><b><i><u><ul><ol><li><h1><h2><h3><h4><h5><h6><hr><pre><strike><strong><sub><sup><table><tbody><thead><tr><td><th><tfoot><dd><dl><dt><blockquote><hr><cite><code><font>');
					if($fieldvalue == "" || empty($fieldvalue)) {
						if($this->errorResponseFormat == 1) {
							$errors = array();
							$errobj = new stdClass();
							$errobj->errcode = 'L_012';
							$errobj->errorfield = $fieldname;
							$errobj->errormsg =$fieldname." contains non allowable tags.";
							$this->sendErrorNew($errobj);
						}
						else {
							$this->sendError($fieldname." contains non allowable tags.");
							return $fieldname." contains non allowable tags";
						}
					}
				}
				return $fieldvalue;
			}catch (Exception $ex) {
				watchdog_exception('expertus_xss_validate', $ex);
				expertusErrorThrow($ex);
			}
			
	}
	
	public function xss_clean($data){ 
		// Fix &entity\n;
		$data = html_entity_decode($data, ENT_COMPAT, 'UTF-8');
		$data = rawurldecode($data);
		$data = str_replace(array('&amp;','&lt;','&gt;'), array('&amp;amp;','&amp;lt;','&amp;gt;'), $data);
		$data = preg_replace('/(&#*\w+)[\x00-\x20]+;/u', '$1;', $data);
		$data = preg_replace('/(&#x*[0-9A-F]+);*/iu', '$1;', $data);
	
	
		// Remove any attribute starting with "on" or xmlns
		$data = preg_replace('#(<[^>]+?[\x00-\x20"\'])(?:on|xmlns)[^>]*+>#iu', '$1>', $data);
	
		// Remove javascript: and vbscript: protocols
		$data = preg_replace('#([a-z]*)[\x00-\x20]*=[\x00-\x20]*([`\'"]*)[\x00-\x20]*j[\x00-\x20]*a[\x00-\x20]*v[\x00-\x20]*a[\x00-\x20]*s[\x00-\x20]*c[\x00-\x20]*r[\x00-\x20]*i[\x00-\x20]*p[\x00-\x20]*t[\x00-\x20]*:#iu', '$1=$2nojavascript...', $data);
		$data = preg_replace('#([a-z]*)[\x00-\x20]*=([\'"]*)[\x00-\x20]*v[\x00-\x20]*b[\x00-\x20]*s[\x00-\x20]*c[\x00-\x20]*r[\x00-\x20]*i[\x00-\x20]*p[\x00-\x20]*t[\x00-\x20]*:#iu', '$1=$2novbscript...', $data);
		$data = preg_replace('#([a-z]*)[\x00-\x20]*=([\'"]*)[\x00-\x20]*-moz-binding[\x00-\x20]*:#u', '$1=$2nomozbinding...', $data);
	
		// Only works in IE: <span style="width: expression(alert('Ping!'));"></span>
		$data = preg_replace('#(<[^>]+?)style[\x00-\x20]*=[\x00-\x20]*[`\'"]*.*?expression[\x00-\x20]*\([^>]*+>#i', '$1>', $data);
		$data = preg_replace('#(<[^>]+?)style[\x00-\x20]*=[\x00-\x20]*[`\'"]*.*?behaviour[\x00-\x20]*\([^>]*+>#i', '$1>', $data);
		$data = preg_replace('#(<[^>]+?)style[\x00-\x20]*=[\x00-\x20]*[`\'"]*.*?s[\x00-\x20]*c[\x00-\x20]*r[\x00-\x20]*i[\x00-\x20]*p[\x00-\x20]*t[\x00-\x20]*:*[^>]*+>#iu', '$1>', $data);
	
		// Remove namespaced elements (we do not need them)
		$data = preg_replace('#</*\w+:\w[^>]*+>#i', '', $data);
// 		expDebug::dPrint("xss clean 2".$data,4);
		do{
			// Remove really unwanted tags
			$old_data = $data;
			$data = preg_replace('#</*(?:applet|b(?:ase|gsound|link)|embed|frame(?:set)?|i(?:frame|layer)|l(?:ayer|ink)|meta|object|s(?:cript|tyle)|title|xml)[^>]*+>#i', '', $data);
		}while ($old_data !== $data);
// 		expDebug::dPrint("xss clean 3".$data,4);
		// we are done...
		return $data;
	}
	/*
    * It is used to collect all the $_GET and $_POST parameter  values into params object in this class.
    * @param none 
    * @return stdClass
    */
	public static function getParamsStatic()
	{
		$paramsObj=new stdClass();
		foreach($_GET as $key => $value)
		{
			$paramsObj->$key=$value;
		   //echo 'Key = ' . $key . '<br />';
		   //echo 'Value= ' . $value;
		}
		foreach($_POST as $key => $value)
		{
			$paramsObj->$key=$value;
		   //echo 'Key = ' . $key . '<br />';
		   //echo 'Value= ' . $value;
		}
		
		
		return $paramsObj;
	}
	
	/*
    * It is used to encode the given string.
    * @param string 
    * @return string
    */
	public function url_encode($string)
	{
        return urlencode(utf8_encode($string));
    }
    
	/*
    * It is used to decode the given string.
    * @param string 
    * @return string
    */
    public function url_decode($string)
    {
        return utf8_decode(urldecode($string));
    }
    
	/*
    * It parses the given result set and returns the xml. Column name becomes node name while parsing.
    * @param $resultset
    * @return string ( xml format )
    */
	public function getXML($result)
	{
		$outxml="";
		$outxml='<?xml version="1.0" encoding="UTF-8" ?><results>';
		if($this->isErrorExist())
		{
			$errobj=new stdClass();
			$errobj->errcode=$this->getErrorCode();
			$errobj->errormsg=$this->getErrorMessage();
			$formattedErr=ErrorMessages::getErrorMessage($errobj);
			//$outxml.="<error><error_code>".$this->getErrorCode()."</error_code><error_msg>".$this->getErrorMessage()."</error_msg></error>";
			$outxml.="<error><error_code>".$formattedErr->code."</error_code><short_msg>".$formattedErr->shortmessage."</short_msg><long_msg>".$formattedErr->longmessage."</long_msg><corrective_solution></corrective_solution></error>";
			//$outxml.="<error><error_code>".$formattedErr->code."</error_code><short_msg>".$formattedErr->type."</short_msg><long_msg>".$formattedErr->message."</long_msg><corrective_solution></corrective_solution></error>";
		}
		else
		{
			$len=0;
			$len=$this->getOutputRecSize(count($result));
			
			for($i=0;$i<$len;$i++)
			{
				$outxml.="<result>";
			 	$row=$result[$i];
			 	$rowdata=$this->iterateRow($row);
			 	$outxml.=$rowdata;
			 	$outxml.="</result>";
		  		//$code=$row["CODE"];
			}
		}
		$outxml.='</results>';
		return $outxml;
		
	}
	
	
	/*
    * Returns recommended record size for output.
    * @param $tot_len - Total length of the result set.
    * @return int ( if specied in the request else returns the total length of result set. )
    */
	public function getOutputRecSize($tot_len)
	{
		$limit=0;
// 		expDebug::dPrint("LIMIT value");
// 		expDebug::dPrint($this->params);
		if(property_exists($this->params,"limit")==true)
			$limit=intval($this->params->limit);
// 		expDebug::dPrint("read limit value:".$limit." total leng".$tot_len);
		if($limit>0)
		{	
			if($tot_len>$limit)
				$tot_len=$limit;
		} 	
		
		return $tot_len;	
	}
	
	/*
    * It sets the display_columns in the param variable.
    * @param $result
    * @return none
    */
	private function setDisplayColumns($result)
	{
			$disp_fields="";
			if($this->params->display_columns=="")  //if empty retrieve all the columns
			{
				foreach(array_keys($result[0]) as $key=>$value)
				{
					if(!is_numeric($value)) 
					{
						if($disp_fields=="")
							$disp_fields=$value;
						else
							$disp_fields.=",".$value;
						
						
					}
				}
				$this->params->display_columns=$disp_fields;
			
			}

		
	}
	
	
	/*
    * It parses the given result set and returns the html code. Column name becomes column header in the html table.
    * @param $resultset
    * @return string ( html format )
    */
	private function getHTML($result)
	{
			
			$html="<table cellspacing=0 cellpadding=0 width='100%;'>";
			$html.="<tr>";
	    
		
			$tmparr=explode(",",$this->params->display_columns);
			foreach ($tmparr as $key => $value) 
			{
				$html.="<td class='tdheader'>".ucfirst($value)."</td>";
			}
		
			$html.="</tr>";
			
			$len=$this->getOutputRecSize(count($result));
			for($i=0;$i<$len;$i++)
			{
				$html.="<tr>";
			 	$row=$result[$i];
			 	$html.=$this->iterateRow($row,"html");
			 	$html.="</tr>";
			 	
			}			
			$html.="</table>";
			return $html;
			
		
	}
	
	
	/*
    * It parses the given result set and returns the json object. Column name becomes json key.
    * @param $resultset
    * @return string ( json format )
    */
	private function getJSON($result)
	{
		
		if($this->isErrorExist())
		{
			$errobj=new stdClass();
			$errobj->errcode=$this->getErrorCode();
			$errobj->errormsg=$this->getErrorMessage();
			$formattedErr=ErrorMessages::getErrorMessage($errobj);
			return "{\"results\":{\"jsonResponse\":[{\"iserror\":\"true\",\"errorcode\":\".$formattedErr->code.\",\"shortmessage\":\"$formattedErr->shortmessage\",\"longmessage\":\"$formattedErr->longmessage\"}]}}";
			
		}
		else
		{
			$len=0;
			$len=$this->getOutputRecSize(count($result));
// 			expDebug::dPrint("len of rec size:".$len);
			$rowdata="";
			$row="";
			$tmp="";
			for($i=0;$i<$len;$i++)
			{
			 	$row=$result[$i];
				$tmp=$this->iterateRow($row,"json");
				if($rowdata=="")
					$rowdata.=json_encode($tmp);
				else
					$rowdata.=",".json_encode($tmp);
				//	$this->log("raw::".$tmp);
				//	$this->log("encoded...".json_encode($tmp));
			}
		return "{\"results\":{\"jsonResponse\":[".$rowdata."]}}";
		}
			//expDebug::dPrint("len of records:"."{\"results\":{\"jsonResponse\":[".$rowdata."]}}");
		
		
	}
	
	
	/*
    * It iterates the row based on the requested return type.
    * @param $row,$returntype
    * @return string ( xml or json or html format )
    */
	private function iterateRow($row,$type="xml")
	{
		$rowdata="";
		$tmp_arr=array();
		foreach ($row as $key => $value) 
		{
			if($this->isDisplayable(strtolower($key)))
			{
			    if($type=="xml")
					$rowdata.="<".$key.">".htmlspecialchars($value, ENT_QUOTES)."</".$key.">";
				elseif($type=="json")
				{
					if(!is_numeric($key)) //only maintain the columnname wise array idx
						$tmp_arr[$key]=$value;
				}
				elseif($type=="html")
				{
					if(!is_numeric($key)) //only maintain the columnname wise array idx
					{
				  		$rowdata.="<td class='tddata'  >".$value."</td>";
					}
				}
			}
		}
		if($type=="xml" || $type=="html")
			return $rowdata;
		else
			return $tmp_arr;
	}
	
	/*
    * It sends output to the stream.
    * @param $outstr
    * @return none
    */
	public function sendResponse($outstr)
	{
		ob_clean();
		ob_end_clean();
		header('HTTP/1.1 200 OK');
		//Commented for php 5.3.26 stack. It is misbehaving.
		//header('Content-Length: '.strlen($outstr));
// 	    expDebug::dPrint("LENGTH::".strlen($outstr)." outstr  ==>".$outstr);
	    $returntype = trim($_REQUEST['returntype']);
	    if($returntype == "json")
	      header('Content-Type: application/json');
	    else 
	      header('Content-Type: application/xml');
		echo $outstr;
	}
	
	/*
    * It identifies the given column name is allowed for display.
    * @param $colname
    * @return boolean
    */
	public function isDisplayable($colname)
	{

		$tmparr=explode(",",$this->params->display_columns);
		
		foreach ($tmparr as $key => $value) 
		{
			if(strcmp(strtolower($value), $colname)==0)
				return true;
		}
		return false;
	}
	
	/*
    * Based on the requested return type, it calls the appropriate parser.
    * @param $result
    * @return string
    */
	public function getFormattedData($result)
	{
			$this->setDisplayColumns($result);
			$returnData="";
			if($this->params->returntype=="xml")
			{
				$returnData= $this->getXML($result);
			} elseif ($this->params->returntype=="json")
			{
				$returnData= $this->getJSON($result);
			}elseif ($this->params->returntype=="html")
			{
				$returnData= $this->getHTML($result);
			}
// 			expDebug::dPrint("response data:".$returnData);
			
			return $returnData;
	}
	
	/*
    * It logs the message into txt file for debugging purpose.
    * @param $msg
    * @return none
    */
	public function log($msg)
	{
		//expDebug::dPrint(" Logged by \"$this->className\" class and  userid is (".$this->userData->username.") ==>Msg is====> ".$msg);		
	}
	
	/*
    * It is used to retrieve the value from query string.
    * @param $qrystring string
    * @param $keystr string
    * @return string
    */
	public static function getURLParamValue($qrystring,$keystr)
	{
		$retvalue="";
    	$splitData= explode("?",$qrystring);
    	$keyvals=explode("&",$splitData[1]);

    	for($i=0;$i<count($keyvals);$i++)
		{
			$pairdata=explode("=",$keyvals[$i]);
			if($pairdata[0]==$keystr)
			{
				$retvalue=$pairdata[1];
				return $retvalue;
			}
		} 
    	return $retvalue;
	}

	/*
    * It is used to detect the user agent.
    * @return string
    */
	public static function getBrowser()
	{
	    $u_agent = $_SERVER['HTTP_USER_AGENT'];
	    $bname = 'Unknown';
	    $platform = 'Unknown';
	    $version= "";
	    $ub="";
	
	    //First get the platform?
	    if (preg_match('/linux/i', $u_agent)) {
	        $platform = 'linux';
	    }
	    elseif (preg_match('/macintosh|mac os x/i', $u_agent)) {
	        $platform = 'mac';
	    }
	    elseif (preg_match('/windows|win32/i', $u_agent)) {
	        $platform = 'windows';
	    }
	   
	    // Next get the name of the useragent yes seperately and for good reason
	    if(preg_match('/MSIE/i',$u_agent) && !preg_match('/Opera/i',$u_agent))
	    {
	        $bname = 'Internet Explorer';
	        $ub = "MSIE";
	    }
	    elseif(preg_match('/Firefox/i',$u_agent))
	    {
	        $bname = 'Mozilla Firefox';
	        $ub = "Firefox";
	    }
	    elseif(preg_match('/Chrome/i',$u_agent))
	    {
	        $bname = 'Google Chrome';
	        $ub = "Chrome";
	    }
	    elseif(preg_match('/Safari/i',$u_agent))
	    {
	        $bname = 'Apple Safari';
	        $ub = "Safari";
	    }
	    elseif(preg_match('/Opera/i',$u_agent))
	    {
	        $bname = 'Opera';
	        $ub = "Opera";
	    }
	    elseif(preg_match('/Netscape/i',$u_agent))
	    {
	        $bname = 'Netscape';
	        $ub = "Netscape";
	    }
	   
	    // finally get the correct version number
	    $known = array('Version', $ub, 'other');
	    $pattern = '#(?<browser>' . join('|', $known) .
	    ')[/ ]+(?<version>[0-9.|a-zA-Z.]*)#';
	    if (!preg_match_all($pattern, $u_agent, $matches)) {
	        // we have no matching number just continue
	    }
	   
	    // see how many we have
	    $i = count($matches['browser']);
	    if ($i != 1) {
	        //we will have two since we are not using 'other' argument yet
	        //see if version is before or after the name
	        if (strripos($u_agent,"Version") < strripos($u_agent,$ub)){
	            $version= $matches['version'][0];
	        }
	        else {
	            $version= $matches['version'][1];
	        }
	    }
	    else {
	        $version= $matches['version'][0];
	    }
	   
	    // check if we have a number
	    if ($version==null || $version=="") {$version="?";}
	   
	    return array(
	        'userAgent' => $u_agent,
	        'name'      => $bname,
	        'version'   => $version,
	        'platform'  => $platform,
	        'pattern'    => $pattern
	    );
	}


	/*
	 * Returns the drupal user id.
	 * @param $userid string
	 * @return string
	 */
	public static function getDrupalUid($username)
	{
		$user_detail = db_select('users', 'usr');
		$user_detail->addField('usr','uid','uid');
		
		$results = new stdClass();
		$user_detail->condition('usr.name',$username);
		
		$userDetail = $user_detail->execute();
		$results =  $userDetail->fetchAll();
		return $results[0]->uid;
		
		
	}

	/*
	 * Returns the user details.
	 * @param $userid string
	 * @return string
	 */
	public static function getUserDetails($userid)
	{
		
		$user_detail = db_select('slt_person', 'per');
		$user_detail->addField('per','full_name','full_name');
		$user_detail->addField('per','user_name','user_name');
		
		$results = new stdClass();
		$user_detail->condition('per.id',$userid);
		$user_detail->condition('per.status','cre_usr_sts_atv','=');
		$userDetail = $user_detail->execute();
		$results =  $userDetail->fetchAll();
		
		if ($userDetail->rowCount() <= 0) {
			$results[0]->cnt = 0; //($results->cnt) ? 1 : 0;
		}
		else
		{
				
			$results[0]->cnt = 1; //($results->cnt) ? 1 : 0;
		}
		
		
		
		/*$dao = new AbstractDAO();
		$dao->connect();
		$dao->query("select count(1) as cnt,full_name,user_name from slt_person where status='cre_usr_sts_atv' and id=".$userid);
			
		$results = $dao->fetchResult();
		$dao->closeconnect();
		$results->cnt = ($results->cnt) ? 1 : 0;
		*/
		
		
		
		return $results[0];
	}
	public function formatErrorObject($errors, $type) {
// 		expDebug::dPrint('format error object '.print_r($errors, 1));
		$error_message = '';
		if ($type == "xml") {
			$error_message = "<errors>";
			foreach ( $errors as $error ) {
				$error_message .= "<error>" . "<type>" . $error->type . "</type><field>" . $error->field . "</field><message>" . $error->message . "</message></error>";
			}
			$error_message .= "</errors>";
		} else {
			$json_array = array();
			$json_res = new stdClass();
			$json_res->errors = $errors;
			$json_res->iserror = "true";
			$json_array[] = $json_res;
			$error_message = json_encode (array("results"=>array("jsonResponse"=> $json_array)));
			// "{\"iserror\":\"true\",\"errorcode\":\".$formattedErr->code.\",\"type\":\"$formattedErr->type\",\"message\":\"$formattedErr->message\"}]}}";
		}
		return $error_message;
	}
	/*
	 * To get the error format response
	 */
	public function getErrorResponse() {
		return $this->errorResponseFormat;
	}
	
	
}
/**
 * 
 * @author ayyappans
 * $errors should be provided in case an exception with multiple validation errors needs to be thrown
 */
class BaseAPIException extends Exception {
	private $errors;
	public function __construct($message, $code = 0, Exception $previous = null, $errors = array('errors')) {
		parent::__construct ( $message, $code, $previous );
		
		$this->errors = $errors;
	}
	public function getErrors() {
		return $this->errors;
	}
}

?>