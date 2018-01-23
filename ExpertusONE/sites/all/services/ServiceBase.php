<?php 

//include "GlobalExecution.php";

class ServiceBase{
	
	function __construct(){
		
	}
	
	function executeBase(){
		
	}
	
	protected function toXMLBase($root,$group,$resultSet){
		//expDebug::dPrint("array size : ".sizeOf($resultSet));
		try{
			$outStr='';
			for($i=0;$i<sizeOf($resultSet);$i++){
				//expDebug::dPrint("********/////////////*************");
				$outStr1='';
				foreach($resultSet[$i] as $key=>$value ){
					//expDebug::dPrint($key.":===:".$value);
					$outStr1 .= "<".$key.">".$value."</".$key.">";
				}
				if($group != null && $group != '')
					$outStr .= "<".$group.">".$outStr1."</".$group.">";
			}
			if($root != null && $root != '')
				$outStr = "<".$root.">".$outStr."</".$root.">";
				
			expDebug::dPrint("Xml Output :");
			expDebug::dPrint($outStr);
			return $outStr;
		} catch(Exception $e){
				throw new SoapFault("SPLMS",$e->getMessage());
			}
	}
	
	protected function toXML($opname,$resultSet,$action){
		try{
			$oGlobalExec = new GlobalExecution();
			$recCount = sizeOf($resultSet);
			//expDebug::dPrint("array size : ".$recCount);
			$outStr='';
			$outStr1='';
			if(is_array($resultSet)){
				for($i=0;$i<$recCount;$i++){
					//expDebug::dPrint("********/////////////*************");
					$outStr1='';
					foreach($resultSet[$i] as $key=>$value ){
						//expDebug::dPrint($key.":===:".$value);
						if(($value instanceof stdClass) || is_array($value)){
							$crVal=$this->getSubLevelNodes($value,$key);
						}else{
							$crVal=$value;
							$isutfstr = mb_detect_encoding($crVal,"ASCII");
							if (is_string($isutfstr)){
								//$crVal = preg_replace('/[^\x{0009}\x{000a}\x{000d}\	x{0020}-\x{D7FF}\x{E000}-\x{FFFD}]+/u', '', $crVal);
								$crVal = htmlspecialchars($crVal);
								//$crVal = $crVal;
							}
						}					   
						$outStr1 .= "<".$key.">".$crVal."</".$key.">";	
					}
					$actionStr='';
					/* Adding action for each record : Starts*/
					if($action){
						for($j=0;$j<sizeOf($action);$j++){
							//expDebug::dPrint("Action Name :===:".$action[$j]->action);
							$actionStr .= "<Action name='".$action[$j]->action."' type='".$action[$j]->type."'><Parameters>";
							if($action[$j]->params){
								$actionStr .= " <![CDATA[ ".$action[$j]->script."( ";
								$param = explode(",",$action[$j]->params);
								for($x=0;$x<sizeOf($param);$x++){
									//expDebug::dPrint("Param Name :===:".$param[$x]." -- Value ".$resultSet[$i]->$param[$x]);
									$actionStr .= ($x==0)?"'".$resultSet[$i]->$param[$x]."'":",'".$resultSet[$i]->$param[$x]."'";
								}
								$actionStr .= " ); ]]>";
							}
							$actionStr .= "</Parameters></Action>";
						}
					}
					$outStr1 .= "<Actions>".$actionStr."</Actions>";
					/* Adding action for each record : Ends*/
					$outStr .= "<Item>".$outStr1."</Item>";
				}
			}else{
				foreach($resultSet as $key=>$value){
					//expDebug::dPrint($key.":=11==:".$value);
					if(($value instanceof stdClass) || is_array($value))
						$crVal=$this->getSubLevelNodes($value,$key);
					else{
						$crVal=$value;
						$isutfstr = mb_detect_encoding($crVal,"ASCII");
						if (is_string($isutfstr)){
						//	$crVal = preg_replace('/[^\x{0009}\x{000a}\x{000d}\	x{0020}-\x{D7FF}\x{E000}-\x{FFFD}]+/u', '', $crVal);
							$crVal = htmlspecialchars($crVal);
							//$crVal = $crVal;
						}
					}
					$outStr1 .= "<".$key.">".$crVal."</".$key.">";
				}
				/* Adding action for each record : Starts*/
				$actionStr='';
				if($action){
					for($j=0;$j<sizeOf($action);$j++){
						//expDebug::dPrint("Action Name :===:".$action[$j]->action);
						$actionStr .= "<Action name='".$action[$j]->action."' type='".$action[$j]->type."'><Parameters>";
						if($action[$j]->params){
							$actionStr .= " <![CDATA[ ".$action[$j]->script."( ";
							$param = explode(",",$action[$j]->params);
							for($x=0;$x<sizeOf($param);$x++){
								//expDebug::dPrint("Param Name :===:".$param[$x]." -- Value ".$resultSet[$i]->$param[$x]);
								$actionStr .= ($x==0)?"'".$resultSet[$i]->$param[$x]."'":",'".$resultSet[$i]->$param[$x]."'";
							}
							$actionStr .= " ); ]]>";
						}
						$actionStr .= "</Parameters></Action>";
					}
				}
				$outStr1 .= "<Actions>".$actionStr."</Actions>";
				/* Adding action for each record : Ends*/
				$outStr .= "<Item>".$outStr1."</Item>";
			}
			$outStr = "<Items>".$outStr."</Items>";
			$exTime = $oGlobalExec->getExecutionTime(); 
			$outStr = "<Provider name='SPLMS' executiontime='".$exTime."sec' totalrecords='".$recCount."'>".$outStr."</Provider>";
			$outStr = "<".$opname." executiontime='".$exTime."sec' totalrecords='".$recCount."'>".$outStr."</".$opname.">";
			//expDebug::dPrint("Xml Output :".$outStr);
			$this->resetCertificate();
			return ($outStr);
		} catch(Exception $e){
				throw new SoapFault("SPLMS",$e->getMessage());
			}
	}
	
	private function getSubLevelNodes($node,$kv){
		//expDebug::dPrint("Recursion entered");
		try{
			$out='';
			$recCnt = sizeOf($node);
			for($i=0;$i<$recCnt;$i++){
				$out.="<".$kv."SubItem>";
				foreach($node[$i] as $key=>$value ){
					//expDebug::dPrint($key.":=Recursion==:".$value);
					if(($value instanceof stdClass) || is_array($value)){
						$crVal=$this->getSubLevelNodes($value,$key);
					}else{
							$crVal=$value;
							$isutfstr = mb_detect_encoding($crVal,"ASCII");
							if (is_string($isutfstr)){
								$crVal = htmlspecialchars($crVal);
								//$crVal = $crVal;
							}
					}
					$out .= "<".$key.">".$crVal."</".$key.">";
				}
				$out.="</".$kv."SubItem>";
				//expDebug::dPrint("Recursion -- ".$out);
			}
			return $out;
		} catch(Exception $e){
				throw new SoapFault("SPLMS",$e->getMessage());
			}
	}
	
	/* public function fetchFaqSearchOptions($reqdata){
		try{
			require_once "../dao/CommonDAO.php";
			$dao = new CommonDAO();
			$result=$dao->fetchFaqSearchOptions($reqdata);
			$outData = $this->toXML("FAQSearchOptsResponse",$result,null);
	    	$outData = new SoapVar($outData, XSD_ANYXML, null, null, null);
			return $outData;	
		} catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	} */
	/*
	public function fetchAdvancedSearchOptions($reqdata){
		try{
			require_once "../dao/CommonDAO.php";
			$dao = new CommonDAO();
			$result=$dao->fetchAdvancedSearchOptions($reqdata);
			$outData = $this->toXML("AdvancedSearchOptsResponse",$result,null);
	    	$outData = new SoapVar($outData, XSD_ANYXML, null, null, null);
			return $outData;	
		} catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	*/
	
		private function resetCertificate(){
			$certificate  = $_COOKIE["SPCertificate"];
				expDebug::dPrint("Cookie value :");
				expDebug::dPrint($certificate);
				//expDebug::dPrint("actionkey value :".strlen($this->actionkey));
				if($certificate==null || $certificate=='')
					$certificate=$_GET["certificateid"];
				
		
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
			expDebug::dPrint("valid ----------->>>>>>>>>>> : ".$valid);
			$loginTime = $timems+($valid*60);
			$token = $tokenItem[0]."^#".$tokenItem[1]."^#".$tokenItem[2]."^#".$loginTime;
			expDebug::dPrint("Token : ".$token);
			$enc1 = new Encrypt();
			$deVal = $enc1->encrypt($token);
			expDebug::dPrint("Token Encrypt : ".$deVal);
			setcookie("SPCertificate",$deVal,null,"/","",(isset($_SERVER["HTTPS"]) && $_SERVER["HTTPS"]=='on'?TRUE:""),(isset($_SERVER["HTTPS"]) && $_SERVER["HTTPS"]=='on'?TRUE:""));
		}
	} 
	
	public function clearCacheValue($key){
		include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/GlobalUtil.php";
		$oConfig  =  new GlobalUtil();
		$aConfig = $oConfig->getConfig();
		$memServer = $aConfig['memcache_server1'];
		$memSrvDt = explode(':',$memServer);
		// Connect to the memcache server
		$memcache = new Memcache;
    $connectedToMemCache = $memcache->connect($memSrvDt[0], $memSrvDt[1]);
    if($connectedToMemCache){
    	if($memcache->get($key)){
    		$memcache->delete($key);
    	}
    	$memcache->close();
    }
    return true;
	}
	
	public function setCacheValue($key,$value){
		include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/GlobalUtil.php";
		$oConfig  =  new GlobalUtil();
		$aConfig = $oConfig->getConfig();
		$memServer = $aConfig['memcache_server1'];
		$memSrvDt = explode(':',$memServer);
		// Connect to the memcache server
		$memcache = new Memcache;
    $connectedToMemCache = $memcache->connect($memSrvDt[0], $memSrvDt[1]);
    if($connectedToMemCache){
    	if($memcache->get($key)){
    		$memcache->replace($key,$value);
    	}else{
    		$memcache->set($key,$value);
    	}
    	$memcache->close();
    }
    return true;
	}
	
	public function getCacheValue($key){
		include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/GlobalUtil.php";
		$oConfig  =  new GlobalUtil();
		$aConfig = $oConfig->getConfig();
		$memServer = $aConfig['memcache_server1'];
		$memSrvDt = explode(':',$memServer);
		// Connect to the memcache server
		$memcache = new Memcache;
    $connectedToMemCache = $memcache->connect($memSrvDt[0], $memSrvDt[1]);
    if($connectedToMemCache){
    	$rstVal= $memcache->get($key);
    	$memcache->close();
    	return $rstVal;
    }
	}
	
	public function stripRegStatus($val){
		return preg_replace('/[^a-zA-Z0-9\_ |]/si' , '' , $val);
	}
	
	public function stripApostrophe($val){
		$val = preg_replace('/\'/', '', $val);
		$val = preg_replace('/\"/', '', $val);
		return $val;
	}
}
?>