<?php

/**
* @class or PHP Name		: PickListService
* @author(s)				: Vincent.S
* Version         			: 1.0
* Date						: 09/07/2009
* PHP Version          		: 5.2.6
* Description     			: Picklist Web Service is used to get the list of values.
*/

include "../dao/PickListDAO.php";
include_once "ServiceBase.php";
include_once "Trace.php";

class PickListService extends ServiceBase{
	
	
	public function __construct(){
		
	}
	
	public function picklist($reqdata){
		try{
			$plistDAO = new PickListDAO(); 
			$outstr = "<Provider name='SPLMS'>";
			$len=sizeof($reqdata);
			if($len>=1){
				$listln = sizeof($reqdata->Lists);
				if($listln==1){
					$params=new stdClass();
					$params->Code=$reqdata->Lists->Code;
					$params->Language=$reqdata->Lists->Language;
					$result = $plistDAO->fetchList($params);
					$outstr .= "<ListItem code='".$params->Code."' totalrecords='".sizeOf($result)."'>".$this->toXMLBase('Items','Item',$result)."</ListItem>";  
				}else{
					for($i=0;$i<$listln;$i++){
						$params=new stdClass();
						$params->Code=$reqdata->Lists[$i]->Code;
						$params->Language=$reqdata->Lists[$i]->Language;
						$result = $plistDAO->fetchList($params);
						$outstr .= "<ListItem code='".$params->Code."' totalrecords='".sizeOf($result)."'>".$this->toXMLBase('Items','Item',$result)."</ListItem>";  
					}
				}
			}
			$outstr .= "</Provider>";
			$outData = new SoapVar($outstr, XSD_ANYXML, null, null, null);
			return $outData;
		}catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
}

$server = new SoapServer("../modules/core/exp_sp_core/wsdl/picklist.wsdl");
$server->setClass("PickListService");
$server->handle();

?>