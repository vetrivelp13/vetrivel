<?php

/**
* @class or PHP Name	: SCSoapClient
* @author(s)		  	: Vincent
* Version         	  	: 1.0
* Date			  		: 10/04/2009
* Description     	  	: Wrapper class for SoapClient, this class will handles
* 						  to set request cookies and also it handles to set the 
* 						  response header in the call back url.
* PHP Version          	: 5.2.6
*/

class SCSoapClient{
	
	function __construct(){
		
	}
	
	public function connect($wsdl,$url,$request,$soapaction,$cookie){
		try{
			$peerVerify = getConfigValue('peer_verify') == 0 ? FALSE : TRUE;
			$opts = array(
            'ssl' => array('verify_peer'=>$peerVerify, 
            							 'verify_peer_name'=>$peerVerify)
        );
	      $soapOptions = array(
	      	'location'=>$url,
	      	'uri'=>$url,
	      	'exceptions'=>FALSE,
	      	'trace'=>true,
	      	'stream_context' => stream_context_create($opts)
	      );
			$client = new SoapClient($wsdl,$soapOptions);
			//$client = new SoapClient($wsdl,array('location'=>$url,'uri'=>$url,'exceptions'=>FALSE,'trace'=>true));
			$cookie = ($cookie==null || $cookie=='')?$_COOKIE:$cookie;
			foreach($cookie as $key=>$value){
				//expDebug::dPrint("Cookies in SCSoapClient  ".$key."==:==".$value);
				$client->__setCookie($key,$value);
			}
			$xmlstr = $client->__doRequest($request,$url,"$soapaction",1);
			$res = $client->__getLastResponseHeaders();
			$tmp = explode("\n",$res);
			for($i=0;$i<sizeOf($tmp);$i++){
				//expDebug::dPrint("Header in SCSoapClient: $i : ".$tmp[$i]);
				if(stripos($tmp[$i],"Cookie")>0){
					header($tmp[$i],false);
				}else{
					header($tmp[$i]);
				}
			}
			return $xmlstr;
		}catch(Exception $e){
				throw new SoapFault("SPLMS",$e->getMessage());
			}
	}
}
?>