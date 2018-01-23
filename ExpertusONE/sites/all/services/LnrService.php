<?php
/**
* @class or PHP Name		: Service
* @author(s)				: Vincent.S
* Version         			: 1.0
* Date						: 09/07/2009
* PHP Version          		: 5.2.6
* Description     			: Main Front Controller PHP for SmartPortal Web Services. 
* The service is responsible for processing all web services requests. It centralizes functions such as validating the service request and instantiating * the appropriate Service component using reflection concept based on the service no and delegate the control to the actual web service component to do 
* the required job.
*/

include "Encryption.php";
include "Trace.php";
include "GlobalExecution.php";
include "../dao/WebServiceDAO.php";
include "../commonlib/UserInfo.php";

class LnrService {
	
	private $actionkey;
	private $rawData;
	private $certificateid;
	private $flag=true;
	private $errorMsg;
	private $srvObj;
	
	public function __construct(){
		try{
			$this->init();
		}catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}	
	}
	
	public function execute(){
		try{
			if($this->flag){				
				$URI=$_SERVER["REQUEST_URI"];
				if(stristr($URI,"WSDL")){
					$this->viewServiceWSDL();
				} else {					
					$this->processService();
				}	
			} else {
				expDebug::dPrint("Error Message :"+$this->errorMsg,1);
				throw new SoapFault("ExpPortal Service",$this->errorMsg);
			}	
						
		}catch(Exception $e){
			$result1= $e->getMessage();
			if($result1!="Page not found"){
				$result="<?xml version='1.0' encoding='UTF-8'?>";
				$result.="<SOAP-ENV:Envelope xmlns:SOAP-ENV='http://schemas.xmlsoap.org/soap/envelope/'><SOAP-ENV:Body><SOAP-ENV:Fault>";
				$result.="<faultcode>ExpertusONE</faultcode><faultstring>$result1</faultstring></SOAP-ENV:Fault></SOAP-ENV:Body></SOAP-ENV:Envelope>";
				header("Content-type: text/xml");
				header("HTTP/1.1 500 Internal Service Error");
				echo $result; 
			}else{
				$result="<?xml version='1.0' encoding='UTF-8'?>";
				$result.="<SOAP-ENV:Envelope xmlns:SOAP-ENV='http://schemas.xmlsoap.org/soap/envelope/'><SOAP-ENV:Body><SOAP-ENV:Fault>";
				$result.="<faultcode>ExpertusONE</faultcode><faultstring>$result1</faultstring></SOAP-ENV:Fault></SOAP-ENV:Body></SOAP-ENV:Envelope>";
				header("Content-type: text/xml");
				header("HTTP/1.1 404 Page Not Found");
				echo $result;
			}
		}
	}	
	
	private function viewServiceWSDL(){
		$soapaction=$this->actionkey;
		header("Content-Type:text/xml"); 
		if(stristr($soapaction,"lnr")){
			$wsdl='http://'.$_SERVER['HTTP_HOST'].'/sites/all/wsdl/learner/'.$soapaction.'service.wsdl';
		} else {
			$wsdl='http://'.$_SERVER['HTTP_HOST'].'/sites/all/wsdl/'.$soapaction.'service.wsdl';	
		}	
		try{
			$lines = file($wsdl);
			if($lines){
				foreach ($lines as $line_num => $line) {
			    	echo $line;
				}		
			} 
		} catch(Exception $e){
			echo "WSDL does not exists";			
		}
	}
	
	private function processService(){
		$dbobj = new WebServiceDAO();
		$this->srvObj = $dbobj->fetch($this->actionkey);
		if($this->srvObj!=null && $this->srvObj!=''){	
			$class = $this->srvObj->service_component;
			expDebug::dPrint("this->srvObj->Lnrservice_component :".$class,4);				
			//if(!ob_start("ob_gzhandler")) ob_start();
			require_once $class.".php";	
		}else{
				throw new SoapFault("SPLMS","Page Not Found");
		}
	}
	
	private function init(){
		try{
			$soapaction = isset($_SERVER['HTTP_SOAPACTION'])?$_SERVER['HTTP_SOAPACTION']:'';
			if(strlen($soapaction)==0) $soapaction = $_GET["soapaction"];
			if(strlen($soapaction)==0) $soapaction = $_GET["actionkey"];			
			expDebug::dPrint("LnrService###### soap action ".$soapaction,4);			
			$soapaction=str_replace('"','',$soapaction);
			$this->actionkey=$soapaction;	
			if($this->actionkey != $this->xss_clean($soapaction)){
				expDebug::dPrint("Service - soap action invlid ---> ".$soapaction,1);
				throw new SoapFault("SPLMS","Invalid soapaction");
			}
			if($this->actionkey == null || $this->actionkey=='' ){				
				$this->flag=false;
				$this->errorMsg="Service Number Required";
			}			
		}catch(Exception $e){
				throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	private function xss_clean($data){
		// Fix &entity\n;
		$data = html_entity_decode($data, ENT_COMPAT, 'UTF-8');
		$data = urldecode($data);
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
		$data = preg_replace('/\'/', '', $data);
		$data = preg_replace('/\"/', '', $data);
		do{
			// Remove really unwanted tags
			$old_data = $data;
			$data = preg_replace('#</*(?:applet|b(?:ase|gsound|link)|embed|frame(?:set)?|i(?:frame|layer)|l(?:ayer|ink)|meta|object|s(?:cript|tyle)|title|xml)[^>]*+>#i', '', $data);
		}while ($old_data !== $data);
		// we are done...
		return $data;
	}
}

try{
	global $rawData;
	$rawData = '';
	if($rawData==null || $rawData=='')
		$rawData=file_get_contents("php://input");
	$obj=new LnrService();
	$obj->execute();
}catch(Exception $e){
		//throw new SoapFault("SPLMS",$e->getMessage());
		$result="<?xml version='1.0' encoding='UTF-8'?>";
		$result.="<SOAP-ENV:Envelope xmlns:SOAP-ENV='http://schemas.xmlsoap.org/soap/envelope/'><SOAP-ENV:Body><SOAP-ENV:Fault>";
		$result.="<faultcode>ExpertusONE</faultcode><faultstring>".$e->getMessage()."</faultstring></SOAP-ENV:Fault></SOAP-ENV:Body></SOAP-ENV:Envelope>";
		header("Content-type: text/xml");
		header("HTTP/1.1 500 Internal Service Error");
		echo $result; 
}
?>
