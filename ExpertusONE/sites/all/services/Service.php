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
include "ServiceBase.php";
include "../dao/WebServiceDAO.php";
include "../commonlib/UserInfo.php";

class Service extends ServiceBase{
	
	private $actionkey;
	private $rawData;
	private $certificateid;
	private $flag=true;
	private $errorMsg;
	private $srvObj;
	
	public function __construct(){
		try{
			$this->init();
		}catch (Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}
		
	}
	
	public function execute(){
		try{
			$this->validate();
			if($this->flag || $this->actionkey=='Authenticate'){
				// Memcache Impl.
				$cData = $this->getCacheValue('Action_Key_' . $this->actionkey);
				if(empty($cData)){
					$dbobj = new WebServiceDAO();
					$this->srvObj = $dbobj->fetch($this->actionkey);
					expDebug::dPrint("this->srvObj :",4);
					expDebug::dPrint($this->srvObj,4);
					if($this->srvObj!=null && $this->srvObj!=''){	
						$class = $this->srvObj->service_component;
						//expDebug::dPrint("this->srvObj->service_component :".$class);	
						$this->setCacheValue('Action_Key_' . $this->actionkey,$class);
					}else{
						throw new SoapFault("SPLMS","Page not found");
					}
				}else{
					$class = $cData;
				}
				expDebug::dPrint("this->srvObj->service_component :".$class,4);	
				require_once $class.".php";
			}else{
				expDebug::dPrint("Error Message :"+$this->errorMsg,1);
				throw new SoapFault("ExpPortal Service",$this->errorMsg);
			}
		}catch(Exception $e){
			$result1= $e->getMessage();
			expDebug::dPrint($result1,1);
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
				header("HTTP/1.1 404 Page not found");
				echo $result;
			}
		}
	}
	
	private function validate(){
		try{
			if(strlen($this->certificateid)>0){
				$enc = new Encrypt();
				$deVal = $enc->decrypt($this->certificateid);
				$liVal = explode("^#",$deVal);
				if(sizeOf($liVal)<4){
					$this->errorMsg="Invalid Certification";
					$this->flag=false;
				}/*else{
					$loginTime = $liVal[3];
					$currentTime = date("Y-m-d h:i:s");
					$expireTime = $loginTime - strtotime($currentTime);
					expDebug::dPrint("Expire time :".$expireTime);
					if($expireTime == 0 || $expireTime <= 0){
						$this->errorMsg="Your session has expired";
						$this->flag=false;
					}
				}*/
			}else{
				$this->errorMsg="Your session has expired";
				$this->flag=false;
			}
			
		}catch(Exception $e){
					throw new SoapFault("SPLMS",$e->getMessage());
				}
	}
	
	private function init(){
		try{
			$soapaction = isset($_SERVER['HTTP_SOAPACTION'])?$_SERVER['HTTP_SOAPACTION']:'';
			if(strlen($soapaction)<3) $soapaction = isset($_GET["soapaction"])?$_GET["soapaction"]:'';
			if(strlen($soapaction)<3) $soapaction = isset($_GET["actionkey"])?$_GET["actionkey"]:'';			
			expDebug::dPrint("Service - soap action ".$soapaction,4);			
			$soapaction=str_replace('"','',$soapaction);
			$this->actionkey=$this->stripApostrophe($soapaction);
			if($this->actionkey!=$soapaction){
				expDebug::dPrint("Service - Invalid Action Key>>>>>>>>>>".$this->actionkey.":::".$soapaction,1);
				throw new SoapFault("SPLMS",'Invalid Action Key');
			}
			//$this->actionkey=isset($_GET["actionkey"])?$_GET["actionkey"]:'';
			
			if($this->actionkey!='Authenticate'){
				$cert  = isset($_COOKIE["SPCertificate"])?$_COOKIE["SPCertificate"]:'';
				expDebug::dPrint("Cookie value :",4);
				expDebug::dPrint($cert,4);
				expDebug::dPrint("actionkey value :".strlen($this->actionkey),4);
				if($cert==null || $cert=='')
					$this->certificateid=$_GET["certificateid"];
				else
					$this->certificateid=$cert;
			}
			if($this->actionkey == null || $this->actionkey=='' ){
				expDebug::dPrint("actionkey value if :".$this->actionkey,4);
				$this->flag=false;
				$this->errorMsg="Action key required";
			}
		}catch(Exception $e){
					throw new SoapFault("SPLMS",$e->getMessage());
				}
	}
	
	
	/*
	 * Function to handle SQL injection
	* Returns value without single and double quotes
	*/
	function stripApostrophe($val){
		$val = preg_replace('/\'/', '', $val);
		$val = preg_replace('/\"/', '', $val);
		return $val;
	}
	
}
$fullstime= microtime(true);
//expDebug::dPrint("Full service Start Time:".$fullstime);
try{
	$obj=new Service();
	$obj->execute();
	$fulletime= microtime(true);	
}catch(Exception $e){
	throw new SoapFault("SPLMS",$e->getMessage());
}
//expDebug::dPrint("Full service End Time:".$fulletime);
expDebug::dPrint("Service Layer Time difference :  : ".round(($fulletime-$fullstime)*1000));
?>