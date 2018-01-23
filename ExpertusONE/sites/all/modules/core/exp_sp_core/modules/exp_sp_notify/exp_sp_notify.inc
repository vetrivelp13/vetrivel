<?php 

class notificationProcess{
	private $tbleName;
	
	private $randNo;
	
	private $limit = 100;
	
	public function notifyProcess($crnType){
		try{
			$time_start = microtime(true);
			expDebug::dPrint("Testing the total execution time".$time_start."table name".$this->tbleName, 4);
			$this->tbleName = ($crnType == 'bulk') ? 'slt_bulk_notification' : 'slt_notification';
			$this->randNo = $this->setRandamID();
			$x='';
			$notifyRes = $this->notificationSelect(100);
			foreach($notifyRes as $notVal){
				$time_start1 = microtime(true);
				expDebug::dPrint("Testing the frame execution time".$time_start1, 4);
				// initialize variables
				$add_text='';
				$type=''; 
				$message = new StdClass(); 
				$object=new StdClass(); 
				$senduser=new StdClass();
				$tokenstr='';
				$notificationsubject='';
				$notificationtemplate='';
				$notifyId='';
				$sendFlag='';
				$remarks='';
				
				$senduser->uid=1;
				foreach ($notVal as $key => $notify){
					expDebug::dPrint("dsdfsfs".print_r($key,1).'=>'.print_r($notify,1) , 4);
					switch($key){
						case 'id':
							$notifyId=$notify;
							break;
						case 'msg_id':
							$type = $notify;
							break;
						case 'token_str':
							$tokenstr= $notify;
							break;
						case 'send_to_email':
							$senduser->mail=$notify;
							break;
						case 'send_to_email_cc':
							$senduser->mail_cc=$notify;
							break;
						case 'custom0':
							if($notify == 1) {
								$senduser->mail=variable_get('site_mail','info@expertus.com');
							}
							break;
						case 'send_to_id':
							$senduser->uid=$notify;
							break;
						case 'additional_text':
							$add_text=rawurldecode(html_entity_decode( $notify, ENT_QUOTES, "utf-8" ));
							break;
						case 'notification_subject':
							$notificationsubject=$notify;
							break;
						case 'notification_template':
							$notificationtemplate=$notify;
							break;
						case 'created_by':
							$created_by=1;
							break;
						case 'ical' :
							$iCal = $notify;
							break;
					}
				}
				// Token string splits
				$tsplit = $this->tokenSplit($tokenstr);
				$tokenSplit = $tsplit[0];
				$fileName = $tsplit[1];
				expDebug::dPrint('exp_sp_notify.module : $tokenSplit = ' . print_r($tokenSplit, true) , 4);
				expDebug::dPrint($senduser , 4);
				
				// Get notification subject
				$notificationsubjectFinal = $this->notifySubject($notificationsubject,$tokenSplit);
				// Get Template
				$exactTemplate = $this->notifyTemplate($notificationtemplate,$tokenSplit,$notifyId);
				expDebug::dPrint(' $exactTemplate = ' . print_r($exactTemplate, true) , 4);
				$notificationTemplateFinal = implode('',$exactTemplate);
				expDebug::dPrint(' $notificationTemplateFinal = ' . print_r($notificationTemplateFinal, true) , 4);
				if(empty($x)){
					$x = $this->getPageLayoutStyleBulk();
				}
				$notificationTemplateFinal = $x.$notificationTemplateFinal."</td></tr></table>";
				
				//Start # Added by Velu #0030384
				$this->sendBulkSMStoUser($notifyId,$tokenSplit);
				//End # Added by Velu #0030384
				
				//certification_expired push notification
				expDebug::dPrint('$notifyId$notifyId$notifyId' . print_r($type, true) , 4);
				
				if($type == "certification_expire" || $type == 'certification_expire_manager' || $type == 'cert_curr_lp_expire_reminder'){
					expDebug::dPrint('Inside certification_expire', 4);
					$this->sendPushNotificationtoUserBulk($notifyId,$type,$tokenSplit); 
				}else if($type == "compliance_expired"){
					expDebug::dPrint('Inside compliance_expired', 4);
					$this->sendPushNotificationtoUserBulk($notifyId,$type,$tokenSplit);
				}else if($type == "compliance_expiry_remainder"){
					expDebug::dPrint('Inside compliance_expired_reminder', 4);
					$this->sendPushNotificationtoUserBulk($notifyId,$type,$tokenSplit);
				}
				//end certification_expired push notification
				
				// Send notification to user
				$message->subject = $notificationsubjectFinal;
				$message->body	= $notificationTemplateFinal;
				if (!empty($tokenSplit['SharefromName']) && !empty($tokenSplit['SharefromId'])) {
					$message->sendmail_from = $tokenSplit['SharefromId'];
					$message->sendmail_from_name = $tokenSplit['SharefromName'];
				}
				$message->sendmail_to = $senduser->mail;
				
				$ccValueExplode = array_filter(explode(',', $senduser->mail_cc));
				if(($key = array_search($message->sendmail_to,$ccValueExplode )) !== false) {
					unset( $ccValueExplode[$key]);
				}
				$senduser->mail_cc = implode(',', $ccValueExplode);
				
				$message->cc = $senduser->mail_cc;
				$message->notify_id = $notifyId;
				$message->send = 'sendNotificationToUser';  // Save send email function name in message
				
				// iCalendar
				if (!empty($iCal)) {
					$message->ical = $iCal;
				}
				if(!empty($fileName)){
					$message->attachment = $fileName;
				}
				$frmTime = processedTime($time_start1);
				expDebug::dPrint("end time of processingframe".microtime(true)."converted time".gmdate('H:i:s',$frmTime), 4);
				//$this->updateBulkNotificationStatus($notifyId, 'Q', '');
				$this->sendNotificationToUser($message);
			}
			$totTime = processedTime($time_start);
			expDebug::dPrint("end time of processing notification".microtime(true)."converted time".gmdate('H:i:s',$totTime), 4);
		}catch (Exception $ex) {
			//watchdog_exception('custom_notify_cronjob', $ex);
			expertusLogError($ex);
		}
	}

	public function sendNotification($message) {
		try{
			$this->tbleName = 'slt_notification';
			$this->randNo = $this->setRandamID();
			expDebug::dPrint("IP Block message" . print_r($message, true),4);
			$this->sendNotificationToUser($message);
		}catch (Exception $ex) {
			expertusLogError($ex);
		}
	}

	private function notifyTemplate($notificationtemplate,$tokenSplit,$notifyId){
		try{
			$splitContent = explode('#',$notificationtemplate);
			expDebug::dPrint('exp_sp_notify.module : $splitContent = ' . print_r($splitContent, true) , 4);
			$exactTemplate = Array();
			for($i=0; $i<count($splitContent); $i++) {
				if(strlen($splitContent[$i]) > 2 && $splitContent[$i][0] == '@' &&  substr($splitContent[$i], -1) == '@') {
					$strTemplate = ereg_replace('@', '', $splitContent[$i]);
					if($strTemplate == 'group_data' || $strTemplate == 'group_discount_data' 
					|| $strTemplate == 'group_sessiondata' || $strTemplate == 'group_instructordata' 
					|| $strTemplate == 'group_tax_data') {
						$groupDataSplit = explode('~@', $tokenSplit[$strTemplate]);
						$newArrKey = $this->generateGroupData($groupDataSplit,$notifyId);
						$arKey = array_keys($newArrKey);
						foreach($arKey as $valT){
							$tokenSplit[$valT] = implode("<br>", $newArrKey[$valT]); // Save new (token,value) pair in $tokenSplit
							$splitContent[$i] = $tokenSplit[$valT] ; // This does not seem getting used anywhere.
						}
					}
					if($tokenSplit[$strTemplate]) {
						if ($strTemplate !='group_data' && $strTemplate !='group_discount_data' 
						&& $strTemplate !='group_sessiondata' && $strTemplate != 'group_instructordata' 
						&& $strTemplate != 'group_tax_data'){
							$dispTemplate = $tokenSplit[$strTemplate];
						}else{
							$dispTemplate = '';
						}
					}else{ // $strTemplate does not have a value. Check if $strTemplate is to be processed in code.
						global $base_url;
						expDebug::dPrint(' $base_url = ' . print_r($base_url, true) , 4);
						expDebug::dPrint(' $strTemplate = ' . print_r($strTemplate, true) . ', $dispTemplate before = ' . print_r($dispTemplate, true) , 4);
						$dispTemplate = $this->getDisplayTemplate($strTemplate);
						
					}
				}else{
					$dispTemplate = $splitContent[$i];
				}
				expDebug::dPrint(' $dispTemplate after = ' . print_r($dispTemplate, true) , 4);
				$exactTemplate[] = $dispTemplate;
			} // loop ends
			return $exactTemplate;
		}catch(Exception $e){
			expertusLogError($ex);
			throw new Exception($e->getMessage());
		}
	}
	
	private function getDisplayTemplate($strTemplate){
		try{
			global $base_url;
			$config=getConfig("exp_sp");
			switch ($strTemplate) {
				case 'logo_path':
					// $dispTemplate = '<img src="'.getContentUploadPath().'/notification_logo.png"></img>';
					/**
					 * 0040106: The upload customer logo in the site should be used for the notification(amaran)
					 */
					// $currentDefaultThemePath = $base_url.base_path().drupal_get_path('theme',variable_get('theme_default', NULL));
					$dispTemplate = '<img src="'.get_logo_path().'"></img>';
					break;
				case 'cmp_name':
					$dispTemplate = variable_get('site_name', 'ExpertusONE');
					break;
				case 'reply_to':
					$dispTemplate = $config["reply_to"];
					break;
				case 'site_url':
					$dispTemplate = '<a href="'.$base_url.'">'.variable_get('site_name', 'ExpertusONE').'</a>';
					break;
				case 'about_us':
					$dispTemplate = '<a href="'.$base_url.'/?q=node/1">'.t('About Us').'</a>';
					break;
				case 'user_account':
					$dispTemplate = '<a href="'.$base_url.'/?q=learning/my-account">'.t('Account').'</a>';
					break;
				case 'my_learning':
					$dispTemplate = '<a href="'.$base_url.'/?q=learning/enrollment-search">'.t('My Enrollment').'</a>';
					break;
				case 'req_new_pwd':
					$dispTemplate = '<a href="'.$base_url.'/?q=user/password">'.t('Request new password').'</a>';
					break;
				case 'subject_name':
					$dispTemplate = '<a style="font-family:arial,helvetica,sans-serif" href="'.$base_url.'"></a>';
					break;
				case 'site_link':
					$dispTemplate = $base_url;
					break;
				case 'survey_logo_path':
					$dispTemplate = '<img border="0" src="'.getNotificationImagePath().'/survey_notification.png"></img>';
					break;
				case 'catalog_search':
					//$dispTemplate = '<a href="'.$base_url.'/?q=learning/catalog-search">'.t('CATALOG').'</a>';
					$dispTemplate = t('CATALOG');
					break;
				case 'my_programs':
					$dispTemplate = '<a href="'.$base_url.'/?q=learning/enrollment-search">'.t('LBL023').'</a>';
					break;
				case 'my_transcripts':
					$dispTemplate = '<a href="'.$base_url.'/?q=learning/enrollment-search">'.t('LBL009').'</a>';
					break;
				case 'my_certificates':
					$dispTemplate = '<a href="'.$base_url.'/?q=learning/my-profile">'.t('LBL877').'</a>';
					break;
				case 'my_team':
					$dispTemplate = '<a href="'.$base_url.'/?q=learning/myteam-search">'.t('My Team').'</a>';
					break;
				default: // Unrecognized token
					$dispTemplate = '-';
	
			} // end switch
			return $dispTemplate;
		}catch(Exception $e){
			expertusLogError($ex);
			throw new Exception($e->getMessage());
		}
	}
	private function generateGroupData($groupDataSplit,$notifyId){
		try{
			$newArrKey = Array();
			for($n = 0; $n < count($groupDataSplit); $n++) {
				$groupArr=explode('~~', $groupDataSplit[$n]);
				for($j = 0; $j < count($groupArr); $j++){
					$groupVal = explode('<|', $groupArr[$j]);
					$val1 = $groupVal[0];
					$val2 = $groupVal[1];
					expDebug::dPrint(' ($val1, $val2) = ("' . $val1 . '", "' . $val2 . '")' , 4);
					if($val1 == 'tax_title' && !empty($notifyId)) {
						$select = db_select($this->tbleName,'sn');
						$select->innerJoin('slt_profile_list_items', 'spli', 'spli.code = sn.lang_code');
						$select->innerJoin('locales_target', 'lotar', 'lotar.language = spli.attr1');
						$select->innerJoin('locales_source', 'losou', 'lotar.lid = losou.lid');
						$select->addField('lotar','translation', 'translation');
						$select->condition('losou.source', 'Tax', '=');
						$select->condition('sn.id',$notifyId, '=');
						expDebug::dPrintDBAPI ( 'select query for tax title', $select );
						$result = $select->execute()->fetchAssoc();
						expDebug::dPrint('check for tax title in multi language= ' . print_r($result, true) , 4);
						$val2  = $result['translation'];
					}
					// If session_startdate or session_enddate, use the correct date format
					switch ($val1) {
						case 'session_startdate':
						case 'session_enddate':
							$val2 = dateTimeStrToUnivShort(trim($val2), true, false, false, false); // Don't show time
							break;
					}
					$newArrKey[$val1][] =  $val2;
				}
			}
			return $newArrKey;
		}catch(Exception $e){
			expertusLogError($ex);
			throw new Exception($e->getMessage());
		}
	}
	private function notifySubject($notificationsubject, $tokenSplit){
		try{
			$splitSubject = explode('#',$notificationsubject);
			expDebug::dPrint('exp_sp_notify.module : $splitSubject = ' . print_r($splitSubject, true) , 4);
			$keywords = array();
			$exactArr = Array();
			for($i=0;$i<count($splitSubject);$i++) {
				if (strlen($splitSubject[$i]) > 2 && $splitSubject[$i][0] == '@' &&  substr($splitSubject[$i], -1) == '@') {
					$str = ereg_replace('@','',$splitSubject[$i]);
					$keywords[] = ereg_replace('@','',$splitSubject[$i]);
					$dispSubject = $tokenSplit[$str];
					if(!$tokenSplit[$str]){
						if($str == 'cmp_name'){
							$dispSubject = variable_get('site_name', 'ExpertusONE');
						}
						else {
							$dispSubject = '-';
						}
					}
				}else {
					$dispSubject = $splitSubject[$i];
				}
				$exactArr[] = $dispSubject;
			}
			return implode('',$exactArr);
		}catch(Exception $e){
			expertusLogError($ex);
			throw new Exception($e->getMessage());
		}
	}
	
	private function tokenSplit($tokenstr){
		try{
			$tokenSplit = array();
			$fileName = "";
			if($tokenstr != null && $tokenstr != ''){
				$tokenArr=explode('~|',$tokenstr);
				for($i=0;$i<count($tokenArr);$i++) {
					$keyVal=explode('>|',$tokenArr[$i]);
					expDebug::dPrint('toketn $keyVal'.print_r($keyVal, true),4);
					if($keyVal[0]!= 'Actions') {
						// If a date, use correct date format
						switch ($keyVal[0]) {
			
							case 'session_startdate':
							case 'session_enddate':
							case 'learning_dateend':
							case 'learning_datestart':
								if($keyVal[0] == 'session_startdate' && $type =='class_cancel'){
									$keyVal[1] = dateTimeStrToUnivShort(trim($keyVal[1]), true, false, false, false); //Do not show time
								}
								else{
									$keyVal[1] = dateTimeStrToUnivShort(trim($keyVal[1]), true, true, false, false); // Show time
								}
								break;
							case 'order_date':
							case 'tp_expirydate':
							case 'course_completed_date':
								$keyVal[1] = dateTimeStrToUnivShort(trim($keyVal[1]), true, false, false, false); // Do not show time
								break;
							case 'order_refnumber': // ticket #0024165 - special characters in order comments cause mail to be incompletely rendered in outlook
								$keyVal[1] = check_plain($keyVal[1]);
								break;
							case 'job_file_name':
								$fileName = $keyVal[1];
							case 'certificate_attachment':
									$fileName = $keyVal[1];
								break;
						} // End switch
			
						$tokenSplit[$keyVal[0]]=(!empty($keyVal[1])) ? $keyVal[1] : ' ';
						expDebug::dPrint('toketn $tokenSplit'.print_r($tokenSplit, true),4);
					}
				} // End for loop
			}
			return array($tokenSplit,$fileName);
		}catch(Exception $e){
			expertusLogError($ex);
			throw new Exception($e->getMessage());
		}
	}
	
	 
	/*
	 * Worker for exp_email queue.
	*/
	private function sendNotificationToUser($message) {
		try {
			$time_start2 = microtime(true);
			expDebug::dPrint("Testing the frame execution time".$time_start2, 4);
			expDebug::dPrint('$message = ' . print_r($message, true), 5);
			
			require_once('sites/all/commonlib/phpmailer/PHPMailerAutoload.php');
			$phpMailer = new PHPMailer();
			$phpMailer->CharSet = 'UTF-8';
			
			$from = $message->sendmail_from;
			$fromName = $message->sendmail_from_name;
			if (empty($from)) {
				$from = variable_get('site_mail', 'info@expertusone.com');
				$fromName = variable_get('site_name', 'ExpertusONE');
			}
			$cc = empty($message->cc) ? '' : $message->cc;
			$phpMailer->setFrom($from,$fromName);
			$phpMailer->addReplyTo($message->sendmail_to);
			$emails = explode(",",$cc);
			foreach ($emails as $cc_mail){
				$phpMailer->addCC($cc_mail);
			}
			$phpMailer->WordWrap = 50;
			$phpMailer->Subject  = $message->subject;
			$phpMailer->Body = $message->body;
			$phpMailer->IsHTML(true);
			foreach (explode(',',$message->sendmail_to) as $address) {
				$address = trim($address);
				if (!empty($address)) {
					$phpMailer->AddAddress($address);
				}
				else {
					throw new Exception ('Bad email address list');
				}
			}
			
			if(!empty($message->attachment)){
				
				if(strpos($message->attachment,".png")!==false){
					
					$flName = basename($message->attachment);
					$phpMailer->addAttachment($message->attachment, $flName, 'base64', 'image/png');
					
					
				}elseif(strpos($message->attachment,".pdf")!==false){
					$flName = basename($message->attachment);
					$phpMailer->addAttachment($message->attachment, $flName, 'base64', 'application/pdf');
				}
				
				else{
						$fileName = explode('_',$message->attachment);
						$flName = array_splice($fileName,-3,count($fileName),array('.csv'));
						$flName = implode('_',$fileName);
						$flName = str_replace($config['dataload_file_path'].'/', ' ', substr_replace($flName, now().'.csv', strpos($flName,'.csv')));
						$phpMailer->addAttachment($message->attachment, $flName, 'base64', 'image/png');
				}
			}else	if(!empty($message->ical)){
				$iCalObj = unserialize($message->ical);
				expdebug::dPrint('$iCalObj = ' . print_r($iCalObj, true), 4);
				$iCalArr = (array) $iCalObj;
				expdebug::dPrint('$iCalArr = ' . print_r($iCalArr, true), 4);
				$cancelIdx = 1;
				$publishIdx = 1;
				foreach ($iCalArr as $iCalWithType) {
					$iCal = base64_decode($iCalWithType->ical); // for debugging
					expDebug::dPrint('$iCal = ' . print_r($iCal, true), 4);
					expDebug::dPrint('$iCal type = ' . print_r($iCalWithType->type, true), 4);
					if ($iCalWithType->type == 'publish') {
						$icsAttachmentName = 'event'; // #52682 - File name change from event1.ics to event.ics
						$publishIdx++;
					}
					else {
						$icsAttachmentName = 'cancelEV'; // #52682 - File name change from cancelEV.ics to cancelEV.ics
						$cancelIdx++;
					}
					$icsAttachmentName .= '.ics';
					$phpMailer->AddStringAttachment($iCal,$icsAttachmentName,'base64','text/calendar');
					$i++;
				}
			}
			
			
			if(!$phpMailer->Send()) { // Currently capturing result only for the last attachment
				expDebug::dPrint('System could not email the report to recipient. ' . $phpMailer->ErrorInfo, 4);
				$this->updateBulkNotificationStatus($message->notify_id, 'E', 'Error Occured when sending message to user.'); //System could not email the report to recipient.
				expDebug::dPrint('Informing schedule owner', 4);
			} else { // Currently capturing result only after the last attachment is emailed
				// Save status
				$this->updateBulkNotificationStatus($message->notify_id, 'Y', '');
			}
			/* if (file_exists($message->attachment)) {
				unlink($message->attachment);
			} */
			$totTime2 = processedTime($time_start2);
			expDebug::dPrint("end time of processing of notification send".microtime(true)."converted time".gmdate('H:i:s',$totTime2), 4);
		}catch(Exception $e){
			expertusLogError($ex);
			throw new Exception($e->getMessage());
		}
	}
	
	private function notificationSelect(){
		try{
			$selQry = db_select($this->tbleName,'msg');
			$fields = array('id','msg_id','msg_type','user_id','token_str','send_to_id','send_to_name','send_to_email','send_to_email_cc','send_type','ical','created_by','custom0');
			$selQry->fields('msg',$fields);
			$selQry->addField('msg', 'attach_content','additional_text');
			$selQry->addField('notiinfo', 'id','templateid');
			$selQry->addField('notiinfo', 'notification_code');
			$selQry->addField('notiinfo', 'lang_code');
			$selQry->addField('notiframe', 'notification_subject');
			$selQry->addField('notiframe', 'notification_template');
			$selQry->addField('notiframe', 'sms_message');
			
			$selQry->innerJoin('slt_notification_info','notiinfo','msg.msg_id = notiinfo.notification_code');
			$selQry->innerJoin('slt_notification_frame','notiframe','notiinfo.id = notiframe.notification_id AND notiframe.send_options=\'Email\' AND msg.lang_code= notiframe.lang_code');
			$selQry->condition("msg.send_status",'Q','=');
			$selQry->condition("msg.process_id",$this->randNo,'=');
			$selQry->range(0,$this->limit);
			expDebug::dPrintDBAPI("Query for notification select",$selQry);
			$res = $selQry->execute()->fetchAll();
			expDebug::dPrint("Result of the notification query".print_r($res,true));
			return $res;
		}catch(Exception $e){
			expertusLogError($ex);
			throw new Exception($e->getMessage());
		}
	}
	
	private function sendBulkSMStoUser($notifyId,$tokenString){
		try{
			include_once $_SERVER["DOCUMENT_ROOT"]."/apis//sendsms/ExpertusONESMSAPI.php";
	
			// Create an object for the current API
			$obj = new ExpertusONESMSAPI();
	
			expDebug::dPrint(' sendBulkSMStoUser $notifyId = '.$notifyId);
	
			$select = db_select($this->tbleName,'notify');
			$select->leftJoin('slt_notification_info', 'info', 'info.notification_code = notify.msg_id');
			$select->leftJoin('slt_notification_frame', 'frame', 'frame.notification_id = info.id AND frame.lang_code = notify.lang_code');
			$select->leftJoin('slt_person', 'person', 'person.id = notify.user_id');
	
			$select->addField('frame', 'sms_message', 'sms_message');
			$select->addField('person', 'first_name', 'first_name');
			$select->addField('person', 'mobile_no', 'mobile_no');
			$select->addField('person', 'sms_alert', 'sms_alert');
	
			$select->condition('notify.id', $notifyId, '=');
			$select->condition('notify.send_status','N', '=');
	
			$results = $select->execute()->fetchAssoc();
			expDebug::dPrintDBAPI(' sendBulkSMStoUser $select = ' ,$select);
			expDebug::dPrint(' sendBulkSMStoUser $results = '.print_r($results,true),5);
	
			// Remove #@ and @# from SMS text
			$smsMessage = str_replace(array("#@","@#,","@#.","@#"),'',$results['sms_message']);
	
			// Explode SMS text after removed #@ and @# from message text
			$expSmsMessage = explode(" ",$smsMessage);
			$expSmsMessage = array_values(array_filter($expSmsMessage));
			expDebug::dPrint(' sendBulkSMStoUser $expSmsMessage = ' .print_r($expSmsMessage,true),5);
	
			// Make SMS text as a associative array values
			foreach ($expSmsMessage as $key => $value) {
				$assocArrSmsMessage[$value] = $value;
			}// end foreach
			expDebug::dPrint(' sendBulkSMStoUser $assocArrSmsMessage before string replace = ' .print_r($assocArrSmsMessage,true),5);
	
			// $tokenString from slt_notification
			expDebug::dPrint(' sendBulkSMStoUser $tokenSplit = ' .print_r($tokenString,true),5);
	
			foreach($tokenString as $key => $value){
				if($key == $assocArrSmsMessage[$key]){
					$assocArrSmsMessage[$key] = $value;
				}// end if
			}// end foreach
			expDebug::dPrint(' sendBulkSMStoUser $assocArrSmsMessage after string replace = ' .print_r($assocArrSmsMessage,true),5);
	
			// Final SMS text with replaced token string
			$finalSMSMessage = implode(" ",$assocArrSmsMessage);
			expDebug::dPrint(' sendBulkSMStoUser $finalSMSMessage = '.$finalSMSMessage);
	
			// Get SMS configuration details from exp_sp.ini
			$config = getConfig("exp_sp");
			expDebug::dPrint(' sendBulkSMStoUser $config = ' .print_r($config,true),5);
	
			// For clicking URL go to notification details
			$finalSMSMessage = titleController('EXP-SP-NOTIFY-MODULE-FINALMESSAGE',$finalSMSMessage,80).' '.t('LBL1236').' ('.$config['admin_site_url'].'/sms.php?id='.$notifyId.')';
			expDebug::dPrint(' sendBulkSMStoUser with URL finalSMSMessage = '.$finalSMSMessage);
	
			/*
			 * sms_api_flag = True
			* Mobile No. Should not be empty
			* SMS Subscription should be checked
			* SMS text should not be empty
			*/
	
			if($config['sms_api_flag'] == 1 && !empty($results['mobile_no']) && $results['sms_alert'] == 'Y' && !empty($smsMessage)){
	
				expDebug::dPrint(' sendBulkSMStoUser Validation Success ');
	
				// Set parameters for the API in ExpertusONESMSAPI
				$obj->setProvider($config['sms_api_name']);
	
				//For gateway160
				if($config['sms_api_name'] == 'gateway160'){
					expDebug::dPrint(' sendBulkSMStoUser Gateway clickatell ');
					$obj->setDetailsForGateway160Provider($config['sms_api_path'],$config['sms_api_uname'],$config['sms_api_key']);
				}// end if
	
				//For clickatell
				if($config['sms_api_name'] == 'clickatell'){
					expDebug::dPrint(' sendBulkSMStoUser Gateway clickatell ');
					$obj->setDetailsForClickATellProvider($config['sms_api_path'],$config['sms_api_id'],$config['sms_api_uname'],$config['sms_api_key']);
				}// end if
	
				// Call send SMS from ExpertusONESMSAPI
				$obj->sendSMS($results['mobile_no'],$finalSMSMessage);
	
			}else{
				expDebug::dPrint(' sendBulkSMStoUser Validation Failed ');
			}// end if
	
	
		}catch(Exception $e){
			expertusLogError($ex);
			throw new Exception($e->getMessage());
		}// end catch
	
	}
	//End # Added by Velu #0030384
	
	private function sendPushNotificationtoUserBulk($notifyId, $type, $tokenString) {
		try {
	
			expDebug::dPrint ( 'Inside sendPushnotification', 4 );
	
			expDebug::dPrint ( 'sendPushnotification $notifyId = ' . $notifyId );
	
			$select = db_select ( $this->tbleName, 'notify' );
			$select->addField ( 'notify', 'user_id', 'user_id' );
			$select->condition ( 'notify.id', $notifyId, '=' );
			$results = $select->execute ()->fetchAssoc ();
			expDebug::dPrintDBAPI ( ' sendPushnotification $select = ', $select );
			expDebug::dPrint ( ' sendPushnotification $results = ' . print_r ( $results, true ), 5 );
			$userId = $results ['user_id'];
			expDebug::dPrint ( ' sendPushnotification UserId = ' . $userId, 5 );
	
			$action_detail = 'objID1>|' . $tokenString ['program_id'] . '~|objID2>|~|objID3>|~|objID4>|' .$type. '~|objID5>|';
	
			$updStmt = db_update ( $this->tbleName );
			$updStmt->fields ( array (
					'detail_value' => $action_detail,
					'mobile' => 1,
					'updated_by' => null,
					'updated_on' => now ()
			) );
	
			$updStmt->condition ( 'id', $notifyId );
			expDebug::dPrintDBAPI ( '$updStmt', $updStmt );
			$result = $updStmt->execute ();
			expDebug::dPrint ( '$result = ' . $result );
	
			$drupalUser = getDrupalUserForNotificationInsert ( $userId );
			$fireMobileNotification = checkForManageNotification ( $userId, $type );
	
			expDebug::dPrint ( ' fireMobileNotification  = ' . $fireMobileNotification, 5 );
	
			if ($fireMobileNotification === true || $fireMobileNotification == true) {
				$list = explode ( '~|', $tokenString );
				$result = array ();
	
				$notif_type = $type;
	
				foreach ( $list as $pair ) {
					$arr = explode ( ">|", $pair );
					$arr [1] && ($result [$arr [0]] = $arr [1]);
				}
	
				$selectCrsId = db_select ( 'slt_mobiledevice_tokens', 'gcm' );
				$selectCrsId->addField ( 'gcm', 'gcm_regid' );
				$selectCrsId->condition ( 'gcm.name', $drupalUser ['full_name'], '=' );
				$selectCrsId->groupBy ( 'gcm.id' );
				$registatoin_ids = $selectCrsId->execute ()->fetchAll ();
				expDebug::dPrint ( 'Reggggg idd =' . print_r ( $registatoin_ids, true ), 4 );
	
				for($j = 0; $j < count ( $registatoin_ids ); $j++) {
	
					$message = "";
					$select = db_select ( 'slt_notification_info', 'info' );
					$select->leftJoin ( 'slt_notification_frame', 'frame', 'frame.notification_id = info.id' );
					$select->addField ( 'frame', 'notification_subject', 'notification_subject' );
					$select->condition ( 'info.notification_code', $type );
					$select->condition ( 'frame.lang_code', 'cre_sys_lng_eng' );
					expDebug::dPrintDBAPI ( 'narrow search sql query-->>', $select );
					$notification_subject = $select->execute ()->fetchField ();
					// $message = getNotificationSubject($tokenString,$notification_subject);
	
					$splitSubject = explode ( '#', $notification_subject );
					$keywords = array ();
					$exactArr = Array ();
					for($i = 0; $i < count ( $splitSubject ); $i++) {
						if (strlen ( $splitSubject [$i] ) > 2 && $splitSubject [$i] [0] == '@' && substr ( $splitSubject [$i], - 1 ) == '@') {
							$str = ereg_replace ( '@', '', $splitSubject [$i] );
							$keywords [] = ereg_replace ( '@', '', $splitSubject [$i] );
							$dispSubject = $tokenString [$str];
							if (! $tokenString [$str]) {
								if ($str == 'cmp_name') {
									$dispSubject = variable_get ( 'site_name', 'ExpertusONE' );
								} else {
									$dispSubject = $tokenString [$str];
								}
							}
						} else {
							$dispSubject = $splitSubject [$i];
						}
						$exactArr [] = $dispSubject;
					}
					$notificationsubjectFinal = implode ( '', $exactArr );
					expDebug::dPrint ( 'Notification subject ajayyyyyy= ' . $notificationsubjectFinal, 4 );
					$message = $notificationsubjectFinal;
	
					expDebug::dPrint ( 'Learning message ajay = ' . $message . $registatoin_ids [$j]->gcm_regid);
					if (strpos ( $registatoin_ids [$j]->gcm_regid, '~~ios' ) !== false) {
						// Put your device token here (without spaces):
						$regIdArray = explode ( '~~', $registatoin_ids [$j]->gcm_regid );
						include_once ($_SERVER ["DOCUMENT_ROOT"] . '/mobile/GCM.php');
						$gcm = new GCM ();
						$logged_in_user = "";
	
						$deviceToken = $regIdArray [0];
						$addId = 0;
						$new_notifyId = ($notifyId . $addId);
						$result = $gcm->send_notificationAPN ( $deviceToken, $message, $action_detail, $type, $new_notifyId );			
						} else {
						include_once ($_SERVER ["DOCUMENT_ROOT"] . '/mobile/GCM.php');
						$gcm = new GCM ();
						expDebug::dPrint ( 'Innnnnn =' . print_r ( $registatoin_ids [$j]->gcm_regid, true ), 4 );
	
						$message = array (
								"message" => $message
						);
						
						$addId = 0;
						$new_notifyId = ($notifyId . $addId);
						$result = $gcm->send_notification ( $registatoin_ids [$j]->gcm_regid, $message, $action_detail, $type, $new_notifyId );
						
					}
				}
			}
		}catch(Exception $e){
			expertusLogError($ex);
			throw new Exception($e->getMessage());
		}
	}
	
	private function getPageLayoutStyleBulk(){
		try{
			$config=getConfig("exp_sp");
			$data = file_get_contents(path_to_theme()."/expertusone-internals/css/layout-fixed.css", FILE_USE_INCLUDE_PATH);
			$data = stripslashes($data);
			$str_start = stripos($data,"body");
			$str_end = (stripos($data,"}",$str_start) - $str_start) + 1;
			$resString = substr($data,$str_start,$str_end);
	
			if (stripos($resString,"background") != -1) {
				$str_start = stripos($resString,"background")+10;
				$str_end = (stripos($resString,";",$str_start) - $str_start) + 1;
				$resString = substr($resString,$str_start,$str_end);
			}
	
			$resString = substr($resString,stripos($resString,":")+1);
			$resString = str_replace(";","",$resString);
	
			$resStrArray = explode(" ",trim($resString));
			$background_position = "";
			foreach($resStrArray as $elem => $value) {
				if (is_numeric(stripos($value,"url"))) {
	
					$background_image = str_replace("url(","",$value);
					$background_image = str_replace(")","",$background_image);
					$background_image = str_replace("'","",$background_image);
					$background_image = str_replace('"','',$background_image);
					if(is_numeric(stripos($background_image,"http"))) {
						$background_image =  ' background="'.$background_image.'"';
					} else {
						$background_image = str_replace("../","",$background_image);
						if(isset($background_image)) {
							$background_image = ' background="'.$config['admin_site_url'].'/'.path_to_theme().'/expertusone-internals/'.$background_image.'"';
						}
					}
	
				} else if (is_numeric(stripos($value,"#"))) {
					$background_color = "background-color: ".$value."; ";
				} else if (is_numeric(stripos($value,"repeat"))) {
					$background_repeat = "background-repeat: ".$value."; ";
				} else if (is_numeric(stripos($value,"scroll")) || is_numeric(stripos($value,"fixed"))) {
					$background_attachment = "background-attachment: ".$value."; ";
				} else {
					$background_position .= " ".$value;
				}
			}
	
			$background_position = "background-position: ".$background_position."; ";
	
			$style = ' style ="'.$background_color.$background_repeat.$background_attachment.$background_position.'" ';
	
			$noti_frame  = '<table width="100%" cellpadding="0" cellspacing="0" border="0" '.$style.' '.$background_image.'>';
			$noti_frame .= '<tr><td style="padding: 15px;">';
	
			return $noti_frame;
		}catch(Exception $e){
			expertusLogError($ex);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * Update notification status
	 */
	private function updateBulkNotificationStatus($notifyId, $status, $remark,$randId='') {
		try {
			//update the send status for the notification in slt_notification to Q
			/* $updStmt = db_update($this->tbleName);
			$updStmt->fields(array(
					'send_status' => $status,
					'remarks' => $remark,
					'updated_by' => null,
					'updated_on' => now(),
			));
			if(!empty($randId)){
				$updStmt->fields[] = array('process_id'=>$randId);
				$updStmt->condition('send_status', 'N');
				$updStmt->isNull('process_id');
			}else{
				$updStmt->condition('id', $notifyId);
			}
			
			expDebug::dPrintDBAPI('$updStmt', $updStmt);
			$result = $updStmt->execute();
			expDebug::dPrint('$result = ' . $result); */
			
			
			$args = array(':sts' => $status,':rmks' => $remark,':by' => null,':on' => now());
			
			if(!empty($randId)){
				$updQry = 'UPDATE '.$this->tbleName.' SET process_id = :prcid, send_status = :sts , remarks = :rmks , 
									updated_by = :by, updated_on = :on WHERE send_status = :sndsts AND process_id IS NULL LIMIT '.$this->limit;
				$args[':prcid'] = $randId;
				$args[':sndsts'] = 'N';
				
			}else{
				$updQry = 'UPDATE '.$this->tbleName.' SET send_status = :sts , remarks = :rmks , updated_by = :by, 
									updated_on = :on WHERE id = :notid';
				$args[':notid'] = $notifyId;
			}
			
			expDebug::dPrintDBAPI("Update the notification table with process id",$updQry,$args);
			
			db_query($updQry,$args);
			
		}catch(Exception $e){
			expertusLogError($ex);
			throw new Exception($e->getMessage());
		}
	}
	
	private function setRandamID(){
		$loop = true;
		$randId = '';
		while($loop==true){
			$randId = time().'_'.mt_rand(1,$this->limit);
			$qry = db_select($this->tbleName,'notify');
			$qry->addExpression('COUNT(DISTINCT process_id)','pid');
			$qry->condition('notify.process_id',$randId);
			$qry->condition('notify.send_status','N');
			expDebug::dPrintDBAPI("Query for random id",$qry);
			$rst = $qry->execute()->fetchField();
			if($rst == 0){
				$this->updateBulkNotificationStatus($notifyId, 'Q', '',$randId);
				$loop = false;
			}
		}
		return $randId;
	}
}

function processedTime($time_start){
	$time = 0;
	$time_end = microtime(true);
	$time = $time_end - $time_start;
	return $time;
}

?>
