<?php
/**
* @class or PHP Name		: Notification DAO
* @author(s)				: Product Team
* Version         			: 1.0
* Date						: 09/09/2009
* PHP Version          		: 5.2.6
* Description     			: DAO Class to do basic CRUD operations in notification table.
*/

include_once "AbstractDAO.php";

class NotificationDAO extends AbstractDAO{

	public function __construct(){
		parent::__construct();
	}

	public function getNotifications($inObj){
		try{
			//$display='id,msg_id,msg_type,token_str,send_to_id,send_to_name,send_to_email,send_type,attach_content additional_text';
			$where=$inObj->sendFlag != null && $inObj->sendFlag != '' ? 'send_status = "'.$inObj->sendFlag.'"' :'null';
			$query = "CALL slp_notification_sel('".$where."');"; //'".$display."',
			expDebug::dPrint("NotificationDAO getNotifications ".$query);
			$this->connect();
			$this->query($query);
			$results = $this->fetchAllResults();
			$this->closeconnect();
			return $results;
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}

	}
	
	public function getCommerceNotifications($inObj){
      try{
        //$display='id,msg_id,msg_type,token_str,send_to_id,send_to_name,send_to_email,send_type,attach_content additional_text';
        $where[]=$inObj->sendFlag != null && $inObj->sendFlag != '' ? 'send_status = "'.$inObj->sendFlag.'"' :'null';
        $where[]=$inObj->messageId != null && $inObj->messageId != '' ? 'msg.msg_id = "'.$inObj->messageId.'"' :'null';
        $where[]=$inObj->sendToId != null && $inObj->sendToId != '' ? 'msg.send_to_id = "'.$inObj->sendToId.'"' :'null';
        $where_list = implode(' AND ', $where);
        $query = "CALL slp_notification_sel('".$where_list."');"; //'".$display."',
        expDebug::dPrint("CommerceNotificationDAO getNotifications ".$query);
        $this->connect();
        $this->query($query);
        $results = $this->fetchAllResults();
        $this->closeconnect();
        return $results;
      }catch(Exception $e){
        $this->closeconnect();
        throw new SoapFault("SPLMS",$e->getMessage());
      }

  }

	public function updateNotification($inObj){
		$vUserId 				= $this->getLoggedInUserId();
		try{
			$query = "CALL slp_notification_upd('".$inObj->notificationId."','".$inObj->sendFlag."','".$inObj->remarks."','".$vUserId."');";
			expDebug::dPrint("NotificationDAO updateNotification ".$query);
			$this->connect();
			$this->query($query);
			$results = $this->fetchAllResults();
			$this->closeconnect();
			return $results;
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	public function insertLearnerNotification($inObj){
		$vLoggedUserId = $this->getLoggedInUserId();
		try{
		    if($inObj->NotifyType=='assign_dotted_manager'){
    			$query = "CALL slp_lnr_notification_ins('".$inObj->NotifyType."','".$inObj->UserId."'";
    			$query .= ",null";
    			$query .= ($inObj->managersName!=null && $inObj->managersName!='')?",'".$inObj->managersName."'" :",null";
    			$query .= ",null,'".$vLoggedUserId."');";
		    }
		    elseif($inObj->NotifyType=='assign_dotted_organization'){
    			$query = "CALL slp_lnr_notification_ins('".$inObj->NotifyType."','".$inObj->UserId."'";
    			$query .= ",null";
    			$query .= ($inObj->organizationsName!=null && $inObj->organizationsName!='')?",'".$inObj->organizationsName."'" :",null";
    			$query .= ",null,'".$vLoggedUserId."');";
		    }else{
    			$query = "CALL slp_lnr_notification_ins('".$inObj->NotifyType."','".$inObj->UserId."'";
    			$query .= ($inObj->OrderId!=null && $inObj->OrderId!='')?",".$inObj->OrderId :",null";//altered query to pass integer value for reading id when empty or null or any value is passed too
    			$query .= ",'".$inObj->Email."','".$inObj->Password."','".$vLoggedUserId."');";
		    }
			expDebug::dPrint("NotificationDAO insertLearnerNotification ".$query);
			$this->connect();
			$this->query($query);
			//$results = $this->fetchAllResults();
			$this->closeconnect();
			return '';
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	public function insertMasterNotification($inObj){
		try{
			$vUserId = 	$this->getLoggedInUserId();
			if($inObj->notification_id==0)
				$query = "call slp_notificationinfo_ins('".$inObj->notification_code."','".$inObj->notification_language."','".$inObj->notification_local."','".$inObj->notification_entity_type."','".$inObj->notification_sendto."','".$inObj->notification_email_cc."','".$inObj->notification_admin_email."','".$inObj->notification_title."','".$inObj->notification_desc."','".$inObj->notification_type."','".$inObj->notification_procedure."','".$inObj->frequency_data_type."','".$inObj->frequency_data_value."','".$inObj->notification_status."','$vUserId',NOW(),'$vUserId',NOW(),NULL, NULL, NULL, NULL, NULL)";
				else
				$query = "call slp_notificationinfo_upd('".$inObj->notification_id."','".$inObj->notification_code."','".$inObj->notification_language."','".$inObj->notification_local."','".$inObj->notification_entity_type."','".$inObj->notification_sendto."','".$inObj->notification_email_cc."','".$inObj->notification_admin_email."','".$inObj->notification_title."','".$inObj->notification_desc."','".$inObj->notification_type."','".$inObj->notification_procedure."','".$inObj->frequency_data_type."','".$inObj->frequency_data_value."','".$inObj->notification_status."','$vUserId',NOW(),'$vUserId',NOW(),NULL, NULL, NULL, NULL, NULL)";
			$this->connect();
			expDebug::dPrint("NotificationDAO insertMasterNotification() query ".$query);
			$this->query($query);
			if($inObj->notification_id==0)
				$result = $this->fetchAllResults();
			else
				$result = "";
			$this->closeconnect();
			return $result;
		}catch(Exception $e){
			$this->rollbackTrans();
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	public function searchNotification($reqData){
		try{
			$name = $reqData->SearchKey->name;
			$value = $reqData->SearchKey->value;
			if(!empty($name))
				$query = "call slp_notification_template_sel('id, notification_title, notification_code, notification_description, slf_past_time(ifnull(updated_on,created_on)) as LastEditedDays ','".$name." like \"%".trim($value)."%\"')";
			else
				$query = "call slp_notification_template_sel('id, notification_title, notification_code, notification_description, slf_past_time(ifnull(updated_on,created_on)) as LastEditedDays ','1=1')";
			$this->connect();
			expDebug::dPrint("NotificationDAO searchNotification() query ".$query);
			$this->query($query);
			$result = $this->fetchAllResults();
			$this->closeconnect();
			return $result;
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	public function getMasterNotification($reqData){
		try{
			$query = "call slp_notification_template_sel('*','id=\"".$reqData->id."\"')";
			$this->connect();
			expDebug::dPrint("NotificationDAO getMasterNotification() query ".$query);
			$this->query($query);
			$result = $this->fetchAllResults();
			$this->closeconnect();
			return $result;
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	public function insertFrameNotification($inObj){
		try{
			$vUserId = 	$this->getLoggedInUserId();
			$oDrupal = new  DrupalBaseService();
			$oDrupal->setDrupalBaseDir();
			if($inObj->id==0){
				//$query = "call slp_notificationframe_ins('".$inObj->notification_master_id."','".$inObj->send_option."','".$inObj->notification_subject."','".$inObj->notification_template."','".$inObj->notification_header."','".$inObj->notification_footer."','$vUserId',NOW(),'$vUserId',NOW(),NULL, NULL, NULL, NULL, NULL)";
				$insertStmt = db_insert('slt_notification_frame');
                $insertStmt->fields(array(
                  'notification_id'      => $inObj->notification_master_id,
                  'send_options'         => $inObj->send_option,
                  'notification_subject' => $inObj->notification_subject,
                  'notification_template'=> $inObj->notification_template,
                  'header_template'      => $inObj->notification_header,
                  'footer_template'      => $inObj->notification_footer,
                  'created_by'           => $vUserId,
                  'created_on'           => now(),
                  'custom0'              => NULL,
                  'custom1'              => NULL,
                  'custom2'              => NULL,
                  'custom3'              => NULL,
                  'custom4'              => NULL
                ));
                $notificationId = $insertStmt->execute();
                expDebug::dPrint('NotificationDAO insertFrameNotification() query = ' . print_r($notificationId, true));
    }
			else{
				//$query = "call slp_notificationframe_upd('".$inObj->id."','".$inObj->notification_master_id."','".$inObj->send_option."','".$inObj->notification_subject."','".$inObj->notification_template."','".$inObj->notification_header."','".$inObj->notification_footer."','$vUserId',NOW(),'$vUserId',NOW(),NULL, NULL, NULL, NULL, NULL)";
				$updateStmt = db_update('slt_notification_frame');  
          		// Set conditions
          	    $updateStmt->condition('id', $inObj->id);
          	    $updateStmt->fields(array(
          	                        'notification_id'      => $inObj->notification_master_id,
          	                        'send_options'         => $inObj->send_option,
          	                        'notification_subject' => $inObj->notification_subject,
                                    'notification_template'=> $inObj->notification_template,  
                                    'header_template'      => $inObj->notification_header,  
                                    'footer_template'      => $inObj->notification_footer,  
                                    'updated_by'           => $vUserId,
                                    'updated_on'		   => NOW(),
                                    'custom0'              => NULL,
                                    'custom1'              => NULL,
                                    'custom2'              => NULL,
                                    'custom3'              => NULL,
                                    'custom4'              => NULL
          	                        ));	
          	    $updateStmt->execute(); 

			    expDebug::dPrint("NotificationDAO insertFrameNotification() query ");
			    
			}
/*			$this->connect();
			expDebug::dPrint("NotificationDAO insertNotificationFrame() query ".$query);
			$this->query($query);*/
			if($inObj->id==0)
				$result = $notificationId;//$this->fetchAllResults();
			else
				$result = "";
			$oDrupal->restoreWorkingDir();
			//$this->closeconnect();
			return $result;
		}catch(Exception $e){
			$this->rollbackTrans();
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	public function getNotificationFrame($reqData){
		try{
			$query = "call slp_notification_frame_sel('*','notification_id=\"".$reqData->notification_master_id."\" and send_options=\"".$reqData->send_option."\"')";
			$this->connect();
			expDebug::dPrint("NotificationDAO getNotificationFrame() query ".$query);
			$this->query($query);
			$result = $this->fetchAllResults();
			$this->closeconnect();
			return $result;
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
}
?>