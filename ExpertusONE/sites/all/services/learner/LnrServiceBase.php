<?php

class LnrServiceBase{
	private $licensekey;
	private $certificateid;
	private $flag=true;
	private $errorMsg;

	function __construct(){}

	function executeBase(){}

	public function MessageHeaderInfo($headers){
		$this->licensekey=$headers->PortalLicenseKey;
		$this->certificateid=$headers->PortalAuthKey;
		$this->validateAccess();
	}
	 
	protected function toSOAPResponse($opname,$resultSet,$action=''){
		try{
			$oGlobalExec = new GlobalExecution();
			$exTime = $oGlobalExec->getExecutionTime();
			$recCount = sizeOf($resultSet);
				
			$outVal=new StdClass();
			$outVal->ExecutionTime=$exTime." secs";
			$outVal->TotalRecords=$recCount;
			$outVal->Provider=new StdClass();
			$outVal->Provider->Name="SPLMS";
			$outVal->Provider->ExecutionTime=$exTime." secs";
			$outVal->Provider->TotalRecords=$recCount;
				
			//form the items list
			$outVal->Provider->Items=array();
			if(isset($resultSet) && is_array($resultSet)){
				for($i=0;$i<sizeOf($resultSet);$i++){
					$arr=array();
					foreach($resultSet[$i] as $key=>$value ){
						if($value instanceof stdClass){							
							$arr[$key]=$this->getSOAPSubClassNode($value);
						} else if(is_array($value)) {
							$arr[$key]=$this->getSOAPSubArrayNodes($value);
						} else {							
							$crVal=$value;
							$isutfstr = mb_detect_encoding($crVal,"ASCII");
							if (is_string($isutfstr)){
								$crVal = htmlspecialchars($crVal);
							}
							$arr[$key]=$crVal;
						}
					}					
					
					if($action){
						$arr["Actions"]=array();
						for($j=0;$j<sizeOf($action);$j++){
							$actionItem=new StdClass();
							$actionItem->name=$action[$j]->action;
							$actionItem->type=$action[$j]->type;
							$actionStr .= " <![CDATA[ ".$action[$j]->script."( ";
							$param = explode(",",$action[$j]->params);
							for($x=0;$x<sizeOf($param);$x++){
								$actionStr .= ($x==0)?"'".$resultSet[$i]->$param[$x]."'":",'".$resultSet[$i]->$param[$x]."'";
							}
							$actionStr .= " ); ]]>";
							$actionItem->Parameters=array("_"=>$actionStr);
							$arr["Actions"][$j]=$actionItem;
						}
					}					
					$outVal->Provider->Items[$i]=$arr;
				}
			}else if(isset($resultSet)){
				$arr=array();
				foreach($resultSet as $key=>$value){
					$crVal=$value;
					$isutfstr = mb_detect_encoding($crVal,"ASCII");
					if (is_string($isutfstr)){
						$crVal = htmlspecialchars($crVal);
					}
					$arr[$key]=$crVal;
						
				}
				$outVal->Provider->Items[0]=$arr;
			}
			$outData=array();
			$outData[$opname]=$outVal;
			$this->resetCertificate();
			return $outData;
		} catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	
	private function getSOAPSubClassNode($classObj){
		$arr1=array();
		foreach($classObj as $key1=>$value1 ){
			if($value1 instanceof stdClass){
				$arr1[$key]=$this->getSOAPSubClassNode($value1);
			} else if(is_array($value1)){
				$arr[$key1]=$this->getSOAPSubArrayNodes($value1);
			} else {
				$crVal=$value1;
				$isutfstr = mb_detect_encoding($crVal,"ASCII");
				if (is_string($isutfstr)){
					$crVal = htmlspecialchars($crVal);
				}
				$arr1[$key1]=$crVal;
			}	
		}
		
		return $arr1;
	}
	
	private function getSOAPSubArrayNodes($arrObj){
		$retArr=array();
		for($i=0;$i<sizeOf($arrObj);$i++){
			$arr=array();
			foreach($arrObj[$i] as $key=>$value ){
				if($value instanceof stdClass){
					$arr[$key]=$this->getSOAPSubClassNode($value);
				} else if(is_array($value)){
					$arr[$key]=$this->getSOAPSubArrayNodes($value);
				} else {
					$crVal=$value;
					$isutfstr = mb_detect_encoding($crVal,"ASCII");
					if (is_string($isutfstr)){
						$crVal = htmlspecialchars($crVal);
					}
					$arr[$key]=$crVal;	
				}					
			}
			$retArr[$i]=$arr;			
		}
		return $retArr;
		
	}

	private function validateAccess(){
		$this->checkSiteAccess();
		if($this->flag == 1)
		$this->checkUserAuthentication();
		if($this->flag == 1)
		$this->checkUserAuthorization();
		if($this->flag == 0){
			throw new SoapFault("ExpPortal Service",$this->errorMsg);
		}
	}

	private function checkSiteAccess(){
		expDebug::dPrint("LnrServiceBase###### License key ----");
		expDebug::dPrint($this->licensekey);
		if(sizeof($this->licensekey)<=0){
			expDebug::dPrint("LnrServiceBase###### License Key is required to proceed.");
			$this->flag=0;
			$this->errorMsg="License Key is required to proceed.";
		}else{
			$enc = new Encrypt();
			$deVal = $enc->decrypt($this->licensekey);
			$liVal=array();
			$liVal = explode("^#",$deVal);

			if(sizeof($liVal)<2){
				expDebug::dPrint("LnrServiceBase###### Invalid License Key.");
				$this->flag=0;
				$this->errorMsg="Invalid License Key.";
			}else{
				//check the URL & the date
				$this->flag=$this->checkClientURL($liVal[0]) && $this->dateValidity($liVal[1]);
			}			
			expDebug::dPrint("LnrServiceBase###### Site Access Flag---".$this->flag);
		}
	}

	private function checkClientURL($url){
		$isValidUrl=false;
		$reqURL=isset($_SERVER['HTTP_REFERER'])?$_SERVER['HTTP_REFERER']:'';
		
		if($reqURL==null || $reqURL==''){
			$reqURL=$this->isSSL()==TRUE?"https":"http";
			$reqURL.="://".$this->ipCheck();
			if(strval(stripos($url,$reqURL))!="" && stripos($url,$reqURL)>=0)
				$isValidUrl=true;
		}else if($reqURL){
			if(strval(stripos($reqURL,$url))!="" && stripos($reqURL,$url)>=0)
			$isValidUrl=true;
		}
		if($isValidUrl == 0) {
			expDebug::dPrint("LnrServiceBase###### invalid client access");
			$this->errorMsg="Invalid Client Access.";
		}
		return $isValidUrl;
	}

	private function isSSL(){
		if($_SERVER['https'] == 1) /* Apache */ {
			return TRUE;
		} elseif ($_SERVER['https'] == 'on') /* IIS */ {
			return TRUE;
		} elseif ($_SERVER['SERVER_PORT'] == 443) /* others */ {
			return TRUE;
		} else {
			return FALSE; /* just using http */
		}
	}

	private function ipCheck() {
		if (getenv('HTTP_CLIENT_IP')) {
			$ip = getenv('HTTP_CLIENT_IP');
		} elseif (getenv('HTTP_X_FORWARDED_FOR')) {
			$ip = getenv('HTTP_X_FORWARDED_FOR');
		}
		elseif (getenv('HTTP_X_FORWARDED')) {
			$ip = getenv('HTTP_X_FORWARDED');
		}
		elseif (getenv('HTTP_FORWARDED_FOR')) {
			$ip = getenv('HTTP_FORWARDED_FOR');
		}
		elseif (getenv('HTTP_FORWARDED')) {
			$ip = getenv('HTTP_FORWARDED');
		}
		else {
			$ip = $_SERVER['HTTP_HOST'];
		}
		return $ip;
	}
	
	private function dateValidity($dt){
		$cDate=date('Y-m-d');
		expDebug::dPrint("LnrServiceBase###### date validity input ".$dt);
		$d1=date('Y-m-d',strtotime($dt));
		$crDate=explode('-', $cDate);
		$d1date=explode("-", $d1);		
		$date1 = mktime(0,0,0,$crDate[1],$crDate[2],$crDate[0]);
		$date2 = mktime(0,0,0,$d1date[1],$d1date[2],$d1date[0]);		
		if ($date1-$date2 >= 0){
			expDebug::dPrint("LnrServiceBase###### date expired ");
			$this->errorMsg="License validity period has expired.";
			return 0;
		}else{
			return 1;
		}
	}

	private function checkUserAuthentication(){
		expDebug::dPrint("LnrServiceBase###### Certificate ID ----".$this->certificateid);
		try{
			if(strlen($this->certificateid)>0){
				$enc = new Encrypt();
				$deVal = $enc->decrypt($this->certificateid);
				$liVal = explode("^#",$deVal);
				if(sizeOf($liVal)<4){
					$this->errorMsg="Invalid Certification";
					$this->flag=0;
				}else{
					$loginTime = $liVal[3];
					$currentTime = date("Y-m-d h:i:s");
					$expireTime = $loginTime - strtotime($currentTime);
					//expDebug::dPrint("Expire time :".$expireTime);
					if($expireTime == 0 || $expireTime <= 0){
						$this->errorMsg="Your session has expired";
						$this->flag=0;
					}
				}
			}else{
				$this->errorMsg="Certificate Id is Required";
				$this->flag=0;
			}
		}catch(Exception $e){
			expDebug::dPrint($e->getMessage());
			throw new SoapFault("SPLMS",$e->getMessage());
		}
		expDebug::dPrint($this->errorMsg);
		expDebug::dPrint("LnrServiceBase###### User Authentication Flag---".$this->flag);
	}
	 

	private function checkUserAuthorization(){
		$this->flag=1;
		expDebug::dPrint("LnrServiceBase###### User Authorization Flag---".$this->flag);
	}
	
	private function resetCertificate(){
		$certificate = $this->certificateid;
		//$response=$this->globrep->getResponse();
		//$validResponse=stripos($response,'Your session has expired');
		//$isAuthenticate=stripos($this->globrep->getQueryString(),'Authenticate');
		//expDebug::dPrint("Valid response:".$certificate);
		if($certificate!=null && $certificate!='undifined' && $certificate!='' /*&& $validResponse<=0 && $isAuthenticate<=0*/){
			include_once "../services/GlobalUtil.php";
			$enc = new Encrypt();
			$deVal = $enc->decrypt($certificate);
			$tokenItem=explode('^#',$deVal);
			$time = date("Y-m-d h:i:s");
			$timems = strtotime($time);
			$oConfig  =  new GlobalUtil();
			$aConfig = $oConfig->getConfig();
			$valid = $aConfig['timeout'];
			//expDebug::dPrint("valid ----------->>>>>>>>>>> : ".$valid);
			$loginTime = $timems+($valid*60);
			$token = $tokenItem[0]."^#".$tokenItem[1]."^#".$tokenItem[2]."^#".$loginTime;
			expDebug::dPrint("Token : ".$token);
			$enc1 = new Encrypt();
			$deVal = $enc1->encrypt($token);
			//expDebug::dPrint("Token Encrypt : ".$deVal);
			setcookie("SPCertificate",$deVal,null,"/","",(isset($_SERVER["HTTPS"]) && $_SERVER["HTTPS"]=='on'?TRUE:""),(isset($_SERVER["HTTPS"]) && $_SERVER["HTTPS"]=='on'?TRUE:""));
		}
	} 
}
?>