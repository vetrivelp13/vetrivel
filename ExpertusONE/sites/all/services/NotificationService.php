<?php
/**
* @class or PHP Name		: Notification Service
* @author(s)				: Product Team
* Version         			: 1.0
* Date						: 23/02/2010
* PHP Version          		: 5.2.6
* Description     			: Web Service which is used to do CRUD operations of the notifications in SmartPortal LMS.
*/

include "../dao/NotificationDAO.php";
include_once "ServiceBase.php";
include_once "DrupalBaseService.php";

class NotificationService extends ServiceBase{
	public function getNotifications($reqData){
		try {
			$dao = new NotificationDAO();
			$result=$dao->getNotifications($reqData);			
			$outData = $this->toXML("Notifications",$result,null);
			$outData = new SoapVar($outData, XSD_ANYXML, null, null, null);			
			return $outData;
		} catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}	
	}

  public function getCommerceNotifications($reqData){
    try {
      $dao = new NotificationDAO();
      $result=$dao->getCommerceNotifications($reqData);     
      $outData = $this->toXML("Notifications",$result,null);
      $outData = new SoapVar($outData, XSD_ANYXML, null, null, null);     
      return $outData;
    } catch(Exception $e){
      throw new SoapFault("SPLMS",$e->getMessage());
    } 
  }

	public function updateNotification($reqData){
		try {
			$dao = new NotificationDAO();
			$result=$dao->updateNotification($reqData);
			$outData = $this->toXML("Notifications",$result,null);
			$outData = new SoapVar($outData, XSD_ANYXML, null, null, null);			
			return $outData;
		} catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	
	
	public function insertMasterNotification($requestData){
		try{
			$dao = new NotificationDAO();
			$result = $dao->insertMasterNotification($requestData);
			expDebug::dprint("InsertMasterNotification results ");
			expDebug::dprint($result);
			$outData = $this->toXML("InsertNotificationRes",$result,null);
			$outData = new SoapVar($outData, XSD_ANYXML, null, null, null);
			return ($outData);
		}catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	
	public function searchNotification($reqData){
		try{
			$dao = new NotificationDAO();
			$result = $dao->searchNotification($reqData);
			$action[0]->action="Edit";
	    	$action[0]->script="editNotificationMaster";
	    	$action[0]->type="Ajax";
	    	$action[0]->params="id";
			$outData = $this->toXML("SearchNotificationResponse",$result,$action);
			$outData = new SoapVar($outData, XSD_ANYXML, null, null, null);
			return $outData;
		}catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	
	public function getMasterNotification($requestData){
		try{
			$dao = new NotificationDAO();
			$result = $dao->getMasterNotification($requestData);
			$outData = $this->toXML("GetNotificationResponse",$result,null);
			$outData = new SoapVar($outData, XSD_ANYXML, null, null, null);
			return $outData;
		}catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	
	public function insertFrameNotification($requestData){
		try{
			$dao = new NotificationDAO();
			$result = $dao->insertFrameNotification($requestData);
			expDebug::dprint("InsertFrameNotification results ");
			expDebug::dprint($result);
			$outData = $this->toXML("InsertFrameNotificationRes",$result,null);
			$outData = new SoapVar($outData, XSD_ANYXML, null, null, null);
			return ($outData);
		}catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	
	public function getNotificationFrame($requestData){
		try{
			$dao = new NotificationDAO();
			$result = $dao->getNotificationFrame($requestData);
			$outData = $this->toXML("GetNotificationFrameRes",$result,null);
			$outData = new SoapVar($outData, XSD_ANYXML, null, null, null);
			return $outData;
		}catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	
	
}

$server = new SoapServer("../modules/core/exp_sp_core/wsdl/notificationservice.wsdl");
$server->setClass("NotificationService");
$server->handle();
?>