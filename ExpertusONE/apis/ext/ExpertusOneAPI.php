<?php
/*
 * ExpertusOne API
 * 
 * ExpertusOne API v1.0
 * 
 * @author: Rajkumar U
 * @date	:	02-Jan-2012
 * 
 * Actual service
 * 
 * DONOT MODIFY THIS FILE UNTIL IS ABSOLUTELY NECESSARY
 * 
 */
include_once($_SERVER["DOCUMENT_ROOT"].'/apis/dynamicapi/includes.php');
include_once $_SERVER['DOCUMENT_ROOT']."/sites/all/dao/AbstractDAO.php";



class ExpertusOneAPI extends BaseAPI{
		
		protected $CE = null;
		protected $RE = null;
		protected $EE = null;
		
		function __construct(){
			try{
				parent::__construct();
			}catch(Exception $e){
				throw new Exception($e->getMessage());
			}
		}
		
		function full_path()
		{
			$s = &$_SERVER;
			$ssl = (!empty($s['HTTPS']) && $s['HTTPS'] == 'on') ? true:false;
			$sp = strtolower($s['SERVER_PROTOCOL']);
			$protocol = substr($sp, 0, strpos($sp, '/')) . (($ssl) ? 's' : '');
			$port = $s['SERVER_PORT'];
			$port = ((!$ssl && $port=='80') || ($ssl && $port=='443')) ? '' : ':'.$port;
			$host = isset($s['HTTP_X_FORWARDED_HOST']) ? $s['HTTP_X_FORWARDED_HOST'] : isset($s['HTTP_HOST']) ? $s['HTTP_HOST'] : $s['SERVER_NAME'];
			$uri = $host . $port . $s['REQUEST_URI'];
			$segments = explode('?', $uri, 2);
			$url = $segments[0];
			return $url;
		}
		
		function doPrimaryCheckToAvoidHacking()
		{
			$apiurl = $_REQUEST["apiurl"];
			$s = &$_SERVER;
			$host = isset($s['HTTP_X_FORWARDED_HOST']) ? $s['HTTP_X_FORWARDED_HOST'] : isset($s['HTTP_HOST']) ? $s['HTTP_HOST'] : $s['SERVER_NAME'];
			if(isset($apiurl))
			{
				$parse = parse_url($apiurl);
// 				expDebug::dPrint("api url host .".$host);
				if(!$parse['host'] == $host)
				{
					throw new Exception("APIURL is invalid", 5);
				}
			}
			else {
				$url = $this->full_path();
// 				expDebug::dPrint("api url .".$url, 4);
// 				expDebug::dPrint("hostname .".$host, 4);
				$predefined_api_url = $host."/apis/ext/ExpertusOneAPI";
				if(!($url == $predefined_api_url || $url == $predefined_api_url.".php" ))
				{
					throw new Exception("APIURL is invalid", 5);
				}
			}
		}
		function doExecute(){
			try{
				error_reporting(E_ALL);
				ini_set('display_errors','Off');
				ob_start();
				
				$this->doPrimaryCheckToAvoidHacking();
				$this->getParamsWithSafe();
				
				$this->CE = new CoreEngine();
				$this->RE = new RewriteEngine();
				$this->EE = new ExecutionEngine();
				$this->CE->prepareDetals($this->RE->getAPIName());
				$method = $this->CE->getMethod();
				$path = $this->CE->getFullPath($this->CE->getPath());
				$paramtemp = $this->CE->getParams();
                $custom_attr_status = getcustomattributemodulestatus();
                if($custom_attr_status == true){   //#custom_attribute_0078975 - Check module status
                        //Custom Attributes Parameters merging to Api's Actual Parameters #custom_attribute_0078975
                        $apiname = $this->RE->getAPIName();
                        expDebug::dPrint('API name = ' . print_r($apiname, true));
                        if(strtolower($apiname) == 'usercreationapi' || strtolower($apiname) == 'userupdationapi' || strtolower($apiname) == 'listuserapi' 
                                || strtolower($apiname) == 'createcourseapi' || strtolower($apiname) == 'updatecourseapi' || strtolower($apiname) == 'listcoursesapi'
                                || strtolower($apiname) == 'createclassapi' || strtolower($apiname) == 'updateclassapi' || strtolower($apiname) == 'listclassesapi' 
                                || strtolower($apiname) == 'createtpapi' || strtolower($apiname) == 'updatetpapi' || strtolower($apiname) == 'listtpapi' 
                                || strtolower($apiname) == 'addorganizationapi' || strtolower($apiname) == 'updateorganizationapi' 
                                || strtolower($apiname) == 'listorganizationapi' || strtolower($apiname) == 'createlocationapi' 
                                || strtolower($apiname) == 'updatelocationapi') {
                            include_once($_SERVER["DOCUMENT_ROOT"].'/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_manage/exp_sp_administration_customattribute/exp_sp_administration_customattribute.inc');
                            
                            if(strtolower($apiname) == 'usercreationapi' || strtolower($apiname) == 'userupdationapi' || strtolower($apiname) == 'listuserapi')
                                $customValues = getCustomAttributeValues('User','mandatory');
                                
                            if(strtolower($apiname) == 'createcourseapi' || strtolower($apiname) == 'updatecourseapi' || strtolower($apiname) == 'listcoursesapi')
                                $customValues = getCustomAttributeValues('Course','mandatory');
                            
                            if(strtolower($apiname) == 'createclassapi' || strtolower($apiname) == 'updateclassapi' || strtolower($apiname) == 'listclassesapi')
                                $customValues = getCustomAttributeValues('Class','mandatory');
                            
                            if(strtolower($apiname) == 'createtpapi' || strtolower($apiname) == 'updatetpapi' || strtolower($apiname) == 'listtpapi')
                                $customValues = getCustomAttributeValues('Training Plan','mandatory');
                            
                            if(strtolower($apiname) == 'addorganizationapi' || strtolower($apiname) == 'updateorganizationapi' || strtolower($apiname) == 'listorganizationapi')
                                $customValues = getCustomAttributeValues('Organization','mandatory');
                            
                            if(strtolower($apiname) == 'createlocationapi' || strtolower($apiname) == 'updatelocationapi')
                                $customValues = getCustomAttributeValues('Location','mandatory');
                            
                            if(strtolower($apiname) == 'coursesdetailsapi')
                                $customValues = getCustomAttributeValues('Class','mandatory');
                            
                            if(count($customValues)>0){
                            foreach($customValues as $cuskey=>$cusvalue) {
                                if(strtolower($apiname) == 'listuserapi' || strtolower($apiname) == 'listcoursesapi' || strtolower($apiname) == 'listclassesapi' 
                                        || strtolower($apiname) == 'listtpapi' || strtolower($apiname) == 'listorganizationapi' 
                                        || strtolower($apiname) == 'coursesdetailsapi')
                                    $array1[$cusvalue->attributecode]= $cusvalue->attributecode;
                                else
                                    $array1['form_state']['values'][$cusvalue->attributecode]= $cusvalue->attributecode;
                            }
                            $paramtemp = array_merge_recursive($paramtemp, $array1);
                            expDebug::dPrint('API ExecuteAPI() paramtemp after merge = ' . print_r($paramtemp, true));
                        }
                        }
                } //#custom_attribute_0078975 - End Check module status
				$params = $this->CE->getParameters();
				$dispCols = $this->RE->getDisplayColumns($this->RE->getAPIName());
				$paramarraytemp = $this->RE->constructParamArray($paramtemp,$params);
				
				$requiredParams = $this->CE->getRequiredParam();
                $emptyParams = $this->CE->checkFillRequiredParam($requiredParams,$params);
                if(count($emptyParams) > 0){
                  $this->errorHandler($emptyParams);
                  exit;
                 }
				$rs = $this->EE->ExecuteAPI($method,$path,$paramarraytemp);
				//$rs=$this->RE->object2array($rs);
// 				expDebug::dPrint("final  rs....");
				if($rs->isValidateError){
					if($this->errorResponseFormat == 1) {
					  if(isset($rs->errors) && is_array($rs->errors)) {
						throw new BaseAPIException('Validation Failed', 2, null, $rs->errors);
					  }
					  else {
					  	// 	$error_code = isset($rs->errcode) ? $rs->errcode : 5;
					  	$error_code = isset($rs->errcode) && $rs->errcode == "L_012" ? 2 : 5;
					  	throw new Exception($rs->errormsg, $error_code);
					  }
					}
					else {
						if(isset($rs->errors) && is_array($rs->errors)) {
							$error_code = isset($rs->errors[0]->errcode) && $rs->errors[0]->errcode == "L_012" ? 2 : 5;
							$error_message = $rs->errors[0]->errormsg;
							throw new Exception($error_message, $error_code);
						}
						else {
// 							$error_code = isset($rs->errcode) ? $rs->errcode : 5;
							$error_code = isset($rs->errors[0]->errcode) && $rs->errors[0]->errcode == "L_012" ? 2 : 5;
							throw new Exception($rs->errormsg, $error_code);
						}
					} 
				}
				$totalRecords = '';
				if(count($rs) && (is_array($rs))){
				  $totalRecords = $rs['totalrow'];
				  unset($rs['totalrow']);
				}
				$len=$this->getOutputRecSize(count($rs));
				//$totalRecords = $len;
				$newrs = $this->RE->rewriteResult($rs,$dispCols,$len);
				expDebug::dPrint("final new  rs....");
				expDebug::dPrint($newrs);
			}catch(Exception $e) {
					$this->setErrorObject($e);
					if($_REQUEST["returntype"]=="xml" && $this->isErrorExist())
					{
						$errobj=new stdClass();
						if($this->errorResponseFormat == 1) {
						$formattedErr=ErrorMessages::getErrorMessageNew($this->getErrorObject());
						$xmlerror = $this->formatErrorObject($formattedErr, $_REQUEST["returntype"]);
						$this->sendResponse($xmlerror);
						}
						else {
						$formattedErr=ErrorMessages::getErrorMessage($this->getErrorObject());
						$outxml.="<error><error_code>".$formattedErr->code."</error_code><short_msg>".$formattedErr->longmessage."</short_msg><long_msg>".$formattedErr->longmessage."</long_msg><corrective_solution>".$formattedErr->correctivesolution."</corrective_solution></error>";
						$this->sendResponse($outxml);
						}
						//$outxml.="<error><error_code>".$this->getErrorCode()."</error_code><error_msg>".$this->getErrorMessage()."</error_msg></error>";
						exit();
					}
					if($_REQUEST["returntype"]=="json" &&  $this->isErrorExist())
					{
						if($this->errorResponseFormat == 1) {
						$formattedErr=ErrorMessages::getErrorMessageNew($this->getErrorObject());
						$outjson = $this->formatErrorObject($formattedErr, $_REQUEST["returntype"]);
						$this->sendResponse($outjson);
						}
						else {
						  $formattedErr=ErrorMessages::getErrorMessage($this->getErrorObject());
							//$outjson.= "{\"results\":{\"jsonResponse\":[{\"iserror\":\"true\",\"errorcode\":\".$formattedErr->code.\",\"type\":\"$formattedErr->type\",\"message\":\"$formattedErr->message\"}]}}";
						  $outjson.= "{\"results\":{\"jsonResponse\":[{\"iserror\":\"true\",\"errorcode\":\"$formattedErr->code\",\"shortmessage\":\"$formattedErr->shortmessage\",\"longmessage\":\"$formattedErr->longmessage\"}]}}";
						$this->sendResponse($outjson);
						}
						exit();
					}
			}
			if(count($rs) && (is_array($rs))){
			  $newrs['totalrecords'] = $totalRecords;
			}
			$outstr=$this->RE->getFormattedOutput($newrs);
// 			expDebug::dPrint("send res");
// 			expDebug::dPrint($outstr);
			ob_clean();
			$this->sendResponse($outstr);
			//echo print_r($rs);
		}
		
		function errorHandler($ex){
		  $totMandatory = count($ex);
			if($this->errorResponseFormat == 1) {
		  	$errors = array();
    	  	for($i=0; $totMandatory > $i; $i++){
    	    $errobj=new stdClass();
    	    $errobj->errcode = 'L_005';
    	    $errobj->errorfield = $ex[$i];
    	    $errobj->errormsg = $ex[$i]." is required";
    	    $errors[] = $errobj;
    	   }
    	  $formattedErr=ErrorMessages::getErrorMessageNew($errors);
		  }
		  else {
    	  for($i=0; $totMandatory > $i; $i++){
    	    $mandatoryFields .=  $i+1 . '.' . $ex[$i] ."\n";
    	  }
    	  $errobj=new stdClass();
    	  $errobj->errcode = 'L_005';    			
    	  $errobj->errormsg = $mandatoryFields;
    	  $formattedErr=ErrorMessages::getErrorMessage($errobj);		  
		  }
    	  if($_REQUEST["returntype"]=="xml")
    		{      		    
    			if($this->errorResponseFormat == 1 ) {
				$xmlerror = $this->formatErrorObject($formattedErr, $_REQUEST["returntype"]);
    			$this->sendResponse($xmlerror);
    			}
    			else {
    		  	$outxml.="<error><error_code>".$formattedErr->code."</error_code><short_msg>".$formattedErr->shortmessage."</short_msg><long_msg>".$formattedErr->longmessage."</long_msg><corrective_solution>".$formattedErr->correctivesolution."</corrective_solution></error>";
    		  	$this->sendResponse($outxml);    		  
    			}
    		}
    	 if($_REQUEST["returntype"]=="json")
  		  {
  		  	if($this->errorResponseFormat == 1 ) {
			$jsonerror = $this->formatErrorObject($formattedErr, $_REQUEST["returntype"]);
  		  	$this->sendResponse($jsonerror);
  		  	}
  		  	else {
  		    $outjson .=  "{\"results\":{\"jsonResponse\":[{\"iserror\":\"true\",\"errorcode\":\".$formattedErr->code.\",\"shortmessage\":\"$formattedErr->shortmessage\",\"longmessage\":\"$formattedErr->longmessage\"}]}}";
  			$this->sendResponse($outjson);    			
  		  }
		}
}
}

if(isset($_REQUEST['mobile_request']) && isset ( $_REQUEST ['apiname'] ))
{
	
	$pdao=new AbstractDAO();
	$pdao->connect();
	$qry = "select status  from system where name= 'ip_ranges' and type = 'module'";
	$res = $pdao->query($qry);
	$param=$pdao->fetchResult();
	$pdao->closeconnect();
	$status =$param->status;
	
	
	
	if ($status) {
		include_once ($_SERVER ["DOCUMENT_ROOT"] . '/sites/all/modules/core/exp_sp_core/modules/ip_ranges/ip_ranges.module');
		$current_ip = ip_address();
		$blacklist = ip_ranges_get_ip_list( 'blacklist' );
		foreach ( $blacklist as $ip ) {
			if (ip_ranges_check_ip( $ip->ip, $current_ip ) && $_SESSION['widgetCallback'] == false) {
				ip_ranges_deny_access( $current_ip );
			}
		}
	}
	
}

$obj = new ExpertusOneAPI();
$obj->doExecute();
?>