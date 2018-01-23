<?php 
	/**
	* @class or PHP Name		: Profileconfig Service
	* @author(s)				: Product Team
	* Version         			: 1.0
	* Date						: 10/12/2009
	* PHP Version          		: 5.2.6
	* Description     			: Web Service which is used to do CRUD operations of the Profileconfig table in SmartPortal LMS.
	*/

include "../dao/ProfileConfigDAO.php";
include_once "ServiceBase.php";

class ProfileConfigService extends ServiceBase{

	function __construct(){
	}

	public function getInitialData($requestData){
		try{
			$dao = new ProfileConfigDAO();
			expDebug::dPrint("getInitialData starts");
			$result = $dao->getInitialData($requestData);
//			$outData = $this->toXML("GetInitialDataResponse",$result,null);
			$outData = new SoapVar($result, XSD_ANYXML, null, null, null);
			return $outData;
		}catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	
	public function loadProfileFields($requestData){
		try{
			$dao = new ProfileConfigDAO();
			expDebug::dPrint("getInitialData starts");
			$result = $dao->loadProfileFields($requestData);
//			$outData = $this->toXML("LoadProfileFieldsResponse",$result,null);
			$outData = new SoapVar($result, XSD_ANYXML, null, null, null);
			return $outData;
		}catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	
	public function saveDataMap($requestData){
		try{
			$dao = new ProfileConfigDAO();
			expDebug::dPrint("saveDataMap starts");
			$result = $dao->saveDataMap($requestData);
			//$outData = $this->toXML("SaveDataMapResponse",$result,null);
			//$outData = new SoapVar($outData, XSD_ANYXML, null, null, null);
			return $result;
		}catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	public function getScreenData($requestData){
		try{
			$dao = new ProfileConfigDAO();
			expDebug::dPrint("getScreenData starts");
			$result = $dao->getScreenFields($requestData);
			//$outData = $this->toXML("SaveDataMapResponse",$result,null);
			$outData = new SoapVar($result, XSD_ANYXML, null, null, null);
//			expDebug::dPrint("data ".$result );
			return $outData;
		}catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	
	public function getAvailCols($requestData){
		try{
			$dao = new ProfileConfigDAO();
			expDebug::dPrint("getScreenData starts");
			$result = $dao->getAvailCols($requestData);
			$outData = new SoapVar($result, XSD_ANYXML, null, null, null);
			return $outData;
		}catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}
		
	}

	public function saveProfileField($requestData){
		try{
			$dao = new ProfileConfigDAO();
			expDebug::dPrint("saveProfileField starts");
			$result = $dao->saveProfileField($requestData);
			$outData = new SoapVar($result, XSD_ANYXML, null, null, null);
			return $outData;
		}catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}
		
	}

	public function loadProfileField($requestData){
		try{
			$dao = new ProfileConfigDAO();
			expDebug::dPrint("saveProfileField starts");
			$result = $dao->loadProfileField($requestData);
			$outData = new SoapVar($result, XSD_ANYXML, null, null, null);
			return $outData;
		}catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}
		
	}
	
	public function deleteProfileField($requestData){
		try{
			$dao = new ProfileConfigDAO();
			expDebug::dPrint("deleteProfileField starts");
			$result = $dao->deleteProfileField($requestData);
			$outData = new SoapVar($result, XSD_ANYXML, null, null, null);
			return $outData;
		}catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
}

	$server = new SoapServer("../modules/core/exp_sp_core/wsdl/profileconfigservice.wsdl");
	$server->setClass("ProfileConfigService");
	$server->handle();
?>