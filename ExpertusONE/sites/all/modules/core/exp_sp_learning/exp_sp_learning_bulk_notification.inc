<?php 

class BackEndBulkProcess{
	
	public function bulkProcess(){
		try{
			$this->bulkNotificationProcess();
		}catch(Exception $e){
			expDebug::dPrint("Error in CRON - Bulk Process".print_r($e,true),1);
		}
		
		try{
			$this->bulkUpdateProcess();
		}catch(Exception $e){
			expDebug::dPrint("Error in CRON - Bulk Process".print_r($e,true),1);
		}
	}

	/**
	 * List of functions to send notification for various process like remainders.
	 * @return NA
	 */
	
	private function bulkNotificationProcess(){
	
		//Certification expire notification
		try{
			expDebug::dPrint('Certification expire notification - batch monitor start',4);
			$this->LPExpiryDetailBulkinsertNotification();
			expDebug::dPrint('Certification expire notification - batch monitor Completed ',4);
		}catch(Exception $ex){
			expDebug::dPrint("Error in CRON - Certification expire notification -- ".print_r($ex,true),1);
		}
		
		
		try{
			expDebug::dPrint('Certification verifyyyy notification - batch monitor start',4);
			$this->CertVerificationReminerBulkinsertNotification();
			expDebug::dPrint('Certification verifyyyy notification - batch monitor Completed ',4);
		}catch(Exception $ex){
			expDebug::dPrint("Error in CRON - Certification verifyyyy notification -- ".print_r($ex,true),1);
		}
		
		
		//Certification expire notification
		try{
			expDebug::dPrint('Certification expire notification admin - batch monitor start',4);
			$this->LPExpiryAdminBulkinsertNotification();
			expDebug::dPrint('Certification expire notification admin  - batch monitor Completed ',4);
		}catch(Exception $ex){
			expDebug::dPrint("Error in CRON - Certification expire notification admin  -- ".print_r($ex,true),1);
		}
	
		//Get list of programs which starts before x days
		try{
			expDebug::dPrint('Certification/Carriculla/LP reminder notification - batch monitor Start ', 4);
			$this->LPRemainderBulkinsertNotification();
			expDebug::dPrint('Certification/Carriculla/LP reminder notification - batch monitor Completed ',4);
		}catch(Exception $ex){
			expDebug::dPrint("Error in CRON - Certification/Carriculla/LP reminder notification -- ".print_r($ex,true),1);
		}
	
		//Certification expire notification to manager
		try{
			expDebug::dPrint('Certification expire notification to manager- batch monitor start',4);
			$this->ManagerCertExpiryRemainderBulkinsertNotification();
			expDebug::dPrint('Certification expire notification to manager- batch monitor Completed ',4);
		}catch(Exception $ex){
			expDebug::dPrint("Error in CRON - Certification expire notification to manager -- ".print_r($ex,true),1);
		}
	
		//Learning Plan Termination notification learners
		try{
			expDebug::dPrint('Learning Plan Termination notification learners- batch monitor start',4);
			// Get list of learners to send notification
			$this->LPTerminateReminderBulkinsertNotification();
			expDebug::dPrint('Learning Plan Termination notification learners- batch monitor Completed ',4);
		}catch(Exception $ex){
			expDebug::dPrint("Error in CRON - Learning Plan Termination notification learners -- ".print_r($ex,true),1);
		}
	
		//Compliance enrollment/inprogress status change
		try{
			expDebug::dPrint('Compliance enrollment/inprogress status change- incomplete start',4);
			$this->autoComplianceIncompleteBulkNotification();
			expDebug::dPrint('Compliance enrollment/inprogress status change- incomplete end',4);
		}catch(Exception $ex){
			expDebug::dPrint("Error in CRON - Compliance enrollment/inprogress status change -- ".print_r($ex,true),1);
		}
	
		// Remainder notification for Compliance completion
		try{
			expDebug::dPrint('Remainder notification for Compliance completion - start',4);
			$this->autoComplianceMandatoryCompletionDaysBulkReminder('is_compliance');
			expDebug::dPrint('Remainder notification for Compliance completion- completed',4);
		}catch(Exception $ex){
			expDebug::dPrint("Error in CRON - Remainder notification for Compliance completion -- ".print_r($ex,true),1);
		}
	
		// Remainder notification for Mandatory completion (Class)
		try{
			expDebug::dPrint('Remainder notification for Mandatory completion- start',4);
			$this->autoComplianceMandatoryCompletionDaysBulkReminder('mandatory');
			expDebug::dPrint('Remainder notification for Mandatory completion- complete',4);
		}catch(Exception $ex){
			expDebug::dPrint("Error in CRON - Remainder notification for Mandatory completion -- ".print_r($ex,true),1);
		}
	
		// Remainder notification for Mandatory completion (TP)
		try{
			expDebug::dPrint('Remainder notification for Mandatory (TP) completion- start',4);
			$this->autoMandatoryCompletionDaysBulkReminderForTP();
			expDebug::dPrint('Remainder notification for Mandatory (TP) completion- start',4);
		}catch(Exception $ex){
			expDebug::dPrint("Error in CRON - Remainder notification for Mandatory (TP) completion -- ".print_r($ex,true),1);
		}
	
		//Remainder notification for Compliance expiry
		try{
			expDebug::dPrint('Remainder notification for Compliance expiry - start',4);
			$this->complianceExpiryBulkNotification();
			expDebug::dPrint('Remainder notification for Compliance expiry - Complete',4);
		}catch(Exception $ex){
			expDebug::dPrint("Error in CRON - Remainder notification for Compliance expiry -- ".print_r($ex,true),1);
		}
	
		//Remainder for session
		try{
			expDebug::dPrint('Remainder for session - Start',4);
			$this->RemindsessionBulkbatchMonitor();
			expDebug::dPrint('Remainder for session - Complete',4);
		}catch(Exception $ex){
			expDebug::dPrint("Error in CRON - Remainder for session -- ".print_r($ex,true),1);
		}
		
		// Learning plan expire notification
		try{
			expDebug::dPrint('LP Expire - Start',4);
			$this->LPExpiredBulkbatchMonitor();
			expDebug::dPrint('LP Expire - Complete',4);
		}catch(Exception $ex){
			expDebug::dPrint("Error in CRON - LP Expire -- ".print_r($ex,true),1);
		}
	
		//is_complaince update in enrollment table for Compliance classes
		/* try{
			isComplianceUpdate();
		}catch(Exception $ex){
			expDebug::dPrint("Error in CRON - Mandatory Compliance Auto Register-- ".print_r($ex,true),1);
		} */
	
		//Notification to Instructor about the class session
		try{
			expDebug::dPrint('Notification to Instructor about the class session - Start',4);
	 		classSessionNotificationDetailsForInstructor($last_run_date);
	 		expDebug::dPrint('Notification to Instructor about the class session - Complete',4);
	 	}catch(Exception $ex){
	 		expDebug::dPrint("Error in CRON - Notification to Instructor about the class session -- ".print_r($ex,true),1);
	 	}
	 	syncSolrData('Enrollment');
	 	syncSolrData('MasterEnrollment');
	}
	
	/**
	 * List of functions which update and send notifications for various process
	 * @return NA
	 */
	public function bulkUpdateProcess(){
		
		//Compliance expiry and notifiy
		try{
			expDebug::dPrint('Compliance expiry and notifiy - Start',4);
			$this->autoComplianceBulkExpiredReminder();
			expDebug::dPrint('Compliance expiry and notifiy - Start',4);
		}catch(Exception $ex){
			expDebug::dPrint("Error in CRON - Remainder notification for Compliance expiry -- ".print_r($ex,true),1);
		}
		
		//Waitlisted trainings expired
		try{
			expDebug::dPrint('Waitlisted trainings expired and not confirmed before on start date - batch monitor start',4);
			$this->waitlistBulkExpiryNotification();
			expDebug::dPrint('Waitlisted trainings expired and not confirmed before on start date - batch monitor Completed ',4);
		}catch(Exception $ex){
			expDebug::dPrint("Error in CRON - Waitlisted trainings expired and not confirmed before on start date -- ".print_r($ex,true),1);
		}
		
		//Wbt enrollment status change
		try{
			expDebug::dPrint('Wbt enrollment status change- batch monitor start',4);
			$this->getBulkExpiredWbtContent();
			expDebug::dPrint('Wbt enrollment status change- batch monitor Completed ',4);
		}catch(Exception $ex){
			expDebug::dPrint("Error in CRON - Wbt enrollment status change -- ".print_r($ex,true),1);
		} 
		
		//Compliance enrollment/inprogress status change
		try{
			expDebug::dPrint('Class complete -  start',4);
			$this->bulkUpdateClassAsCompleted();
			expDebug::dPrint('Class complete - incomplete end',4);
		}catch(Exception $ex){
			expDebug::dPrint("Error in CRON - Class complete -- ".print_r($ex,true),1);
		} 
		
		
		// Update announcements which was expired
		try{
			expDebug::dPrint("Update announcements which was expired Start",4);
			update_announcement_expired();
			expDebug::dPrint("Update announcements which was expired Complete",4);
		}catch(Exception $ex){
			expDebug::dPrint("Error in CRON - Update announcement -- ".print_r($ex,true),1);
		} 
		
		//Update learning plan completed by date
		try{
			expDebug::dPrint('Update learning plan completed by - Start',4);
			update_learningplan_completedbydate();
			expDebug::dPrint('Update learning plan completed by - Completed',4);
		}catch(Exception $ex){
			expDebug::dPrint("Error in CRON - Update learning plan completed by -- ".print_r($ex,true),1);
		}
		
		//Remove expired OAuth tokens
		try{
			expDebug::dPrint('Remove expired OAuth tokens - Start',4);
			$this->remove_expired_oAuth_access_tokens();
			expDebug::dPrint('Remove expired OAuth tokens - Complete',4);
		}catch(Exception $ex){
			expDebug::dPrint("Error in CRON - Remove expired OAuth tokens -- ".print_r($ex,true),1);
		}
	}
	
	private function LPExpiryDetailBulkinsertNotification(){
		try{
			expDebug::dPrint("Inside Expiry notification",4);
			$startTime = microtime(true);
			expDebug::dPrint("Start time of LPExpiryDetailBulkinsertNotification".$startTime,4);
			$notificationTemplateInfo = $this->getBulkNotificationTemplateInfo('cert_curr_lp_expire_reminder', 'cre_sys_lng_eng');
			expDebug::dPrint(" notificationTemplateInfo ".$notificationTemplateInfo ,5);
			$notifyFlag = ($notificationTemplateInfo['status']  == 'cre_ntn_sts_atv') ? 'Y' : 'N';
			$emailSendTo = explode(',',$notificationTemplateInfo['notification_sendto']);
			$ccEmail = $notificationTemplateInfo['notification_cc'];
			$action_detail =  'objID1>|'.$objectId1.'~|objID2>|~|objID3>|~|objID4>|~|objID5>|';
			if($notifyFlag == 'Y'){
				foreach ($emailSendTo as $usertype){
					expDebug::dPrint(" email send to ".$usertype ,5);
					$config           = getConfig("exp_sp");
					$cert_days_expire = isset($config["cert_expire_reminder_days"]) ? $config["cert_expire_reminder_days"] : 10;
					$select = db_select('slt_program','prgm');
					$select->innerJoin('slt_master_enrollment','mse','mse.program_id = prgm.id');
					$select->innerJoin('slt_person','prs','prs.id = mse.user_id and prs.status = \'cre_usr_sts_atv\'');
					$select->innerJoin('users','usr','usr.name = prs.user_name');
					$select->innerJoin('slt_profile_list_items','prof','prof.code = prgm.object_type');
	
	
					$select->addExpression('\'cert_curr_lp_expire_reminder\'', 'msg_id');
					$select->addExpression("if(find_in_set(prs.preferred_language,'".$notificationTemplateInfo['frmlng']."')>=0, prs.preferred_language,'cre_sys_lng_eng')", 'lang_code');
					$select->addExpression('\'Cert/Curr/Learning plan expiry remainder\'', 'msg_type');
					$select->addExpression("concat('first_name>|',ifnull(prs.first_name,''),'~|delivery_type>|',ifnull(prof.name,''),'~|training_title>|',ifnull(prgm.title,''),'~|program_id>|',ifnull(prgm.id,''),'~|tp_title>|',ifnull(prgm.title,''),'~|tp_description>|',ifnull(prgm.short_desc,''),
							if(prgm.addn_notification_show = 0,'',if(prgm.additional_info IS NOT NULL AND prgm.addn_notification_show = 1,concat('~|add_notes>|',prgm.additional_info,''),'~|add_notes>|-')),
							'~|course_title>|',ifnull(prgm.title,''),'~|session_enddate>|',DATE_FORMAT(
							IF (prgm.expires_in_unit = 'days', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value DAY),
							IF (prgm.expires_in_unit = 'months', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value MONTH),
							DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value YEAR))),
							'%b %e, %Y'),'~|session_endtime>|',DATEDIFF(
							IF (prgm.expires_in_unit = 'days', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value DAY),
							IF (prgm.expires_in_unit = 'months', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value MONTH),
							DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value YEAR))),
							NOW()))", "token_str");
	
					//$select->addExpression("concat('objID1>|',prgm.id,'~|objID2>|~|objID3>|~|objID4>|~|objID5>|')","detail_value");
	
					if($usertype == "cre_ntn_rpt_adm"){
						$select->addField('prs','id','user_id');
						$select->addExpression('null','send_to_id');
						$select->addExpression('null','send_to_name');
						$select->addExpression('null','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
						$select->addExpression('1', 'custom0');
					}
					if($usertype == "cre_ntn_rpt_usr"){
						$select->addField('prs','id','user_id');
						$select->addField('usr','uid','send_to_id');
						$select->addField('prs','full_name','send_to_name');
						$select->addField('usr','mail','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
					}
	
	
					if($usertype == "cre_ntn_rpt_mgr"){
						$select->leftJoin('slt_person','mgr','mgr.id = prs.manager_id');
						$select->leftJoin('users','mgr1','mgr1.name = mgr.user_name');
						$select->addField('mgr','id','user_id');
						$select->addField('mgr1','uid','send_to_id');
						$select->addField('mgr','full_name','send_to_name');
						$select->addField('mgr1','mail','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
					}
	
					$select->addExpression('\'php mailer\'', 'send_type');
					$select->addExpression('\'N\'', 'send_status');
					$select->addExpression('1', 'created_by');
					$select->addExpression('now()', 'created_on');
					$select->addExpression('1', 'updated_by');
					$select->addExpression('now()', 'updated_on');
					//$select->addExpression('1', 'mobile');
						
					$select->condition('prgm.object_type', 'cre_sys_obt_crt', '=');
					$select->condition('mse.overall_status', 'lrn_tpm_ovr_cmp', '=');
					$select->isNotNull('mse.comp_date');
					$select->isNotNull('prgm.expires_in_value');
					$select->isNotNull('prgm.expires_in_unit');
					$select->condition('prgm.expires_in_value', 0, '>');
					$select->where("DATEDIFF(
							IF (prgm.expires_in_unit = 'days', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value DAY),
							IF (prgm.expires_in_unit = 'months', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value MONTH),
							DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value YEAR))),
							NOW()) = $cert_days_expire");
	
					expDebug::dPrintDBAPI('$select getCertCurrLPExpiryDetailsForNotifications', $select);
					db_insert('slt_bulk_notification')
					->from($select)
					->execute();
				}
			}
			$totTime = $this->processedTime($startTime);
			expDebug::dPrint("end time of LPExpiryDetailBulkinsertNotification".gmdate('H:i:s',$totTime),4);
		}catch (Exception $ex) {
			throw new Exception($ex->getMessage());
		}
	
	}
	
	private function LPExpiryAdminBulkinsertNotification(){
		try{
			expDebug::dPrint("Inside LPExpiryAdminBulkinsertNotification priya",4);
			$startTime = microtime(true);
			expDebug::dPrint("Start time of LPExpiryAdminBulkinsertNotification".$startTime,4);
			$notificationTemplateInfo = $this->getBulkNotificationTemplateInfo('cert_path_admin_remainder', 'cre_sys_lng_eng');
			expDebug::dPrint(" notificationTemplateInfo ".$notificationTemplateInfo ,5);
			$notifyFlag = ($notificationTemplateInfo['status']  == 'cre_ntn_sts_atv') ? 'Y' : 'N';
			$emailSendTo = explode(',',$notificationTemplateInfo['notification_sendto']);
			$ccEmail = $notificationTemplateInfo['notification_cc'];
			$action_detail =  'objID1>|'.$objectId1.'~|objID2>|~|objID3>|~|objID4>|~|objID5>|';
			$overall_sts = 'lrn_tpm_ovr_cmp';
			$cert_type = 'cre_sys_obt_crt';
			if($notifyFlag == 'Y'){
				foreach ($emailSendTo as $usertype){
					expDebug::dPrint(" email send to ".$usertype ,5);
					$config           = getConfig("exp_sp");
					$cert_expire_reminder_days_usr = isset($config["cert_expire_reminder_days"]) ? $config["cert_expire_reminder_days"] : 10;
					$cert_days_expire_remainder = isset($config["cert_expire_reminder_days_admin"]) ? $config["cert_expire_reminder_days_admin"] : 30;
					$cert_days_expire_stop_remainder = isset($config["cert_expire_stop_reminder_admin"]) ? $config["cert_expire_stop_reminder_admin"] : 30;
					$select = db_select('slt_program','prgm');
					$select->innerJoin('slt_master_enrollment','mse','mse.program_id = prgm.id');
					
					
					//Inner sub query
					$select1 = db_select('slt_master_enrollment', 'me');
					$select1->addField('me','program_id', 'prgId');
					$select1->addField('me','overall_status', 'overallsts');
					$select1->addExpression('MAX(me.recertify_path)','path');
					$select1->where('me.overall_status = "lrn_tpm_ovr_cmp"');
					$select1->groupBy('me.program_id');
					
					expDebug::dPrintDBAPI('$select LPExpiryAdminBulkinsertNotification priya 111', $select1);
					
					$select2 = db_select('slt_master_enrollment', 'menr');
					$select2->innerJoin($select1,'e','e.prgId = menr.program_id AND e.path = menr.recertify_path AND e.overallsts = menr.overall_status');
					//$select2->condition('menr.overall_status', 'e.overall_status','=');
					$select2->addField('menr', 'program_id', 'prgramId');
					$select2->addField('menr', 'recertify_path', 'recertifyPath');
					$select2->addField('menr','overall_status', 'overall_status');
					$select2->addExpression('MIN(comp_date)','completionDate');
					$select2->groupBy('menr.program_id');
					
					expDebug::dPrintDBAPI('$select LPExpiryAdminBulkinsertNotification priya 222', $select2);
					$select->innerJoin($select2,'d','d.completionDate = mse.comp_date AND d.recertifyPath = mse.recertify_path AND d.prgramId = mse.program_id AND d.overall_status = mse.overall_status');
					
					$select->leftJoin('slt_module','mdl','mdl.program_id = mse.program_id AND mdl.sequence = (mse.recertify_path + 1)');
					$select->leftJoin('slt_module_crs_mapping','map','map.module_id = mdl.id');
					$select->innerJoin('slt_module','currmod','currmod.program_id = mse.program_id AND currmod.sequence = mse.recertify_path');
					$select->innerJoin('slt_person','prs','(prs.id = prgm.created_by OR prs.id = prgm.updated_by) AND prs.status = \'cre_usr_sts_atv\'');
					$select->innerJoin('users','usr','usr.name = prs.user_name');
					$select->innerJoin('slt_profile_list_items','prof','prof.code = prgm.object_type');
	
					
					
	
					$select->addExpression('\'cert_path_admin_remainder\'', 'msg_id');
					$select->addExpression("if(find_in_set(prs.preferred_language,'".$notificationTemplateInfo['frmlng']."')>=0, prs.preferred_language,'cre_sys_lng_eng')", 'lang_code');
					$select->addExpression('\'Cert/Curr/Learning plan admin expiry remainder\'', 'msg_type');
					$select->addExpression("CONCAT('first_name>|',IFNULL(prs.first_name,''),'~|module_title>|',IFNULL(currmod.title,''),'~|training_title>|',IFNULL(prgm.title,''),'~|program_id>|',IFNULL(prgm.id,''),'~|tp_title>|',IFNULL(prgm.title,''),
											if(prgm.addn_notification_show = 0,'',if(prgm.additional_info IS NOT NULL AND prgm.addn_notification_show = 1,concat('~|add_notes>|',prgm.additional_info,''),'~|add_notes>|-')),
				                            '~|session_enddate>|',DATE_FORMAT(
											IF (prgm.expires_in_unit = 'days', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value DAY),
											IF (prgm.expires_in_unit = 'months', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value MONTH),
											DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value YEAR))),
											'%b %e, %Y'))", "token_str");
	
					
				//	if($usertype == "cre_ntn_rpt_adm"){
						$select->addField('prs','id','user_id');
						$select->addField('usr','uid','send_to_id');
						$select->addField('prs','full_name','send_to_name');
						$select->addField('usr','mail','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
				//	}
	
	
					$select->addExpression('\'php mailer\'', 'send_type');
					$select->addExpression('\'N\'', 'send_status');
					$select->addExpression('1', 'created_by');
					$select->addExpression('now()', 'created_on');
					$select->addExpression('1', 'updated_by');
					$select->addExpression('now()', 'updated_on');
					//$select->addExpression('1', 'mobile');
						
					$select->condition('prgm.object_type','cre_sys_obt_crt','=');
					$select->condition('mse.overall_status','lrn_tpm_ovr_cmp');
					$select->isNotNull('mse.comp_date');
					$select->isNotNull('prgm.expires_in_value');
					$select->isNotNull('prgm.expires_in_unit');
					$select->condition('prgm.expires_in_value', '0', '>');
					$select->isNull('map.id');
					$select->where("(CURDATE() > DATE_FORMAT((IF (prgm.expires_in_unit = 'days', 
						DATE_SUB(DATE_SUB((DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value DAY)), INTERVAL $cert_expire_reminder_days_usr DAY),INTERVAL $cert_days_expire_remainder DAY),
						IF (prgm.expires_in_unit = 'months', 
						DATE_SUB(DATE_SUB((DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value MONTH)), INTERVAL $cert_expire_reminder_days_usr DAY),INTERVAL $cert_days_expire_remainder DAY),
						DATE_SUB(DATE_SUB((DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value YEAR)), INTERVAL $cert_expire_reminder_days_usr DAY),INTERVAL $cert_days_expire_remainder DAY))))
						, _utf8'%Y-%m-%d'))
						AND(
						CURDATE() <=
						DATE_FORMAT((IF (prgm.expires_in_unit = 'days', 
						DATE_ADD(DATE_SUB(DATE_SUB((DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value DAY)), INTERVAL $cert_expire_reminder_days_usr DAY),INTERVAL $cert_days_expire_remainder DAY),INTERVAL $cert_days_expire_stop_remainder DAY),
						IF (prgm.expires_in_unit = 'months', 
						DATE_ADD(DATE_SUB(DATE_SUB((DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value MONTH)), INTERVAL $cert_expire_reminder_days_usr DAY),INTERVAL $cert_days_expire_remainder DAY),INTERVAL $cert_days_expire_stop_remainder DAY),
						DATE_ADD(DATE_SUB(DATE_SUB((DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value YEAR)), INTERVAL $cert_expire_reminder_days_usr DAY),INTERVAL $cert_days_expire_remainder DAY),INTERVAL $cert_days_expire_stop_remainder DAY))))
						, _utf8'%Y-%m-%d'))");
					$select->groupBy('mse.program_id');
					$select->groupBy('prs.id');
					expDebug::dPrintDBAPI('$select LPExpiryAdminBulkinsertNotification priya', $select);
					db_insert('slt_bulk_notification')
					->from($select)
					->execute();
				}
			}
			$totTime = $this->processedTime($startTime);
			expDebug::dPrint("end time of LPExpiryAdminBulkinsertNotification".gmdate('H:i:s',$totTime),4);
		}catch (Exception $ex) {
			throw new Exception($ex->getMessage());
		}
	
	}
	
	
	private function LPRemainderBulkinsertNotification(){
	
		try{
			$startTime = microtime(true);
			expDebug::dPrint("Start time of LPRemainderBulkinsertNotification".$startTime,4);
			expDebug::dPrint(" Inside LPRemainderBulkinsertNotification ",4);
	
			$notificationTemplateInfo = $this->getBulkNotificationTemplateInfo('cert_curr_lp_reminder', 'cre_sys_lng_eng');
			expDebug::dPrint(" notificationTemplateInfo ".$notificationTemplateInfo ,5);
			$notifyFlag = ($notificationTemplateInfo['status']  == 'cre_ntn_sts_atv') ? 'Y' : 'N';
			$emailSendTo = explode(',',$notificationTemplateInfo['notification_sendto']);
			$ccEmail = $notificationTemplateInfo['notification_cc'];
			if($notifyFlag == 'Y'){
				foreach ($emailSendTo as $usertype){
					$select = db_select('slt_program','prgm');
					$select->innerJoin('slt_master_enrollment','mse','mse.program_id = prgm.id');
					$select->innerJoin('slt_person','prs','prs.id = mse.user_id and prs.status = \'cre_usr_sts_atv\'');
					$select->innerJoin('users','usr','usr.name = prs.user_name');
					$select->innerJoin('slt_profile_list_items','prof','prof.code = prgm.object_type');
					$select->innerJoin('slt_enrollment','enr','enr.master_enrollment_id = mse.id');
					$select->innerJoin('slt_course_class_session','ses','ses.course_id = enr.course_id and ses.class_id = enr.class_id');
					$select->leftJoin('slt_notification_info','info','info.notification_code = \'cert_curr_lp_reminder\'');
	
					$select->addExpression('\'cert_curr_lp_reminder\'', 'msg_id');
					$select->addExpression("if(find_in_set(prs.preferred_language,'".$notificationTemplateInfo['frmlng']."')>=0, prs.preferred_language,'cre_sys_lng_eng')", 'lang_code');
					$select->addExpression('\'Cert/Curr/Learning plan reminder\'', 'msg_type');
					$select->addExpression("concat('first_name>|',ifnull(prs.first_name,''),
					        '~|full_name>|',ifnull(prs.full_name,''),'~|last_name>|',ifnull(prs.last_name,''),'~|user_name>|',ifnull(prs.user_name,''),
					        '~|learning_delivery_type>|',ifnull(prof.name,''),'~|course_title>|',ifnull(prgm.title,''),'~|tp_description>|',ifnull(prgm.short_desc,''),
										if(prgm.addn_notification_show = 0,'',if(prgm.additional_info IS NOT NULL AND prgm.addn_notification_show = 1,concat('~|add_notes>|',prgm.additional_info,''),'~|add_notes>|-')),
							'~|session_startdate>|',ifnull((CONCAT( DATE_FORMAT(MIN(ses.start_date), _utf8'%Y-%m-%d'),' ',MIN(ses.start_time),':00')),''),'~|session_endtime>|',DATEDIFF(ses.start_date,NOW()))", "token_str");
						
					if($usertype == "cre_ntn_rpt_adm"){
						$select->addField('prs','id','user_id');
						$select->addExpression('null','send_to_id');
						$select->addExpression('null','send_to_name');
						$select->addExpression('null','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
						$select->addExpression('1', 'custom0');
					}
					if($usertype == "cre_ntn_rpt_usr"){
						$select->addField('prs','id','user_id');
						$select->addField('usr','uid','send_to_id');
						$select->addField('prs','full_name','send_to_name');
						$select->addField('usr','mail','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
					}
						
					if($usertype == "cre_ntn_rpt_mgr"){
						$select->leftJoin('slt_person','mgr','mgr.id = prs.manager_id');
						$select->leftJoin('users','mgr1','mgr1.name = mgr.user_name');
						$select->addField('mgr','id','user_id');
						$select->addField('mgr1','uid','send_to_id');
						$select->addField('mgr','full_name','send_to_name');
						$select->addField('mgr1','mail','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
					}
						
					$select->addExpression('\'php mailer\'', 'send_type');
					$select->addExpression('\'N\'', 'send_status');
					$select->addExpression('1', 'created_by');
					$select->addExpression('now()', 'created_on');
					$select->addExpression('1', 'updated_by');
					$select->addExpression('now()', 'updated_on');
	
					$select->groupBy('prs.id');
					$select->groupBy('prs.first_name');
					$select->groupBy('prgm.title');
					$select->groupBy('prof.name');
	
					$select->condition('prgm.status','lrn_lpn_sts_atv','='); // added this Status Condition for this issue #0046707
					$select->condition('info.status','cre_ntn_sts_atv','=');
					$select->condition('mse.overall_status',array('lrn_tpm_ovr_inp','lrn_tpm_ovr_enr'),'IN'); // added this overall status Condition for this issue #0064459
					$select->where("DATEDIFF(ses.start_date,NOW())  = info.frequency_data_value");
	
					expDebug::dPrintDBAPI('LPRemainderBulkinsertNotification : ', $select);
					db_insert('slt_bulk_notification')
					->from($select)
					->execute();
	
				}
			}
			$totTime = $this->processedTime($startTime);
			expDebug::dPrint("end time of LPRemainderBulkinsertNotification".gmdate('H:i:s',$totTime),4);
		}catch (Exception $ex) {
			throw new Exception($ex->getMessage());
		}
	
	}
	
	private function ManagerCertExpiryRemainderBulkinsertNotification(){
		try{
	
			$startTime = microtime(true);
			expDebug::dPrint("Start time of ManagerCertExpiryRemainderBulkinsertNotification".$startTime,4);
			expDebug::dPrint(" ManagerCertExpiryRemainderBulkinsertNotification ",4);
	
			$notificationTemplateInfo = $this->getBulkNotificationTemplateInfo('certification_expire_manager', 'cre_sys_lng_eng');
			expDebug::dPrint(" notificationTemplateInfo ".$notificationTemplateInfo ,5);
			$notifyFlag = ($notificationTemplateInfo['status']  == 'cre_ntn_sts_atv') ? 'Y' : 'N';
			$emailSendTo = explode(',',$notificationTemplateInfo['notification_sendto']);
			$ccEmail = $notificationTemplateInfo['notification_cc'];
			if($notifyFlag == 'Y'){
				foreach ($emailSendTo as $usertype){
					$select = db_select('slt_program','prgm');
					$select->innerJoin('slt_master_enrollment','mse','mse.program_id = prgm.id');
					$select->innerJoin('slt_person','prs','prs.id = mse.user_id and prs.status = \'cre_usr_sts_atv\'');
					$select->innerJoin('users','usr','usr.name = prs.user_name'); //0084991
					$select->leftJoin('slt_person','mgr','mgr.id = prs.manager_id');//0084991
					$select->addExpression('\'certification_expire_manager\'', 'msg_id');
					$select->addExpression("if(find_in_set(prs.preferred_language,'".$notificationTemplateInfo['frmlng']."')>=0, prs.preferred_language,'cre_sys_lng_eng')", 'lang_code');
					$select->addExpression('\'Certification expiry reminder to manager\'', 'msg_type');
					$select->addExpression("concat('first_name>|',ifnull(prs.first_name,''),
							'~|full_name>|',ifnull(prs.full_name,''),
							'~|user_name>|',ifnull(prs.user_name,''),
							'~|manager_name>|',ifnull(mgr.full_name,''),
							'~|tp_title>|',ifnull(prgm.title,''),
							'~|tp_description>|',ifnull(prgm.short_desc,''),
							if(prgm.addn_notification_show = 0,'',if(prgm.additional_info IS NOT NULL AND prgm.addn_notification_show = 1,concat('~|add_notes>|',prgm.additional_info,''),'~|add_notes>|-')),
							'~|delivery_type>|',ifnull(prgm.object_type,''),
							'~|session_enddate>|',DATE_FORMAT(IF(prgm.expires_in_unit = 'days', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value DAY),
							IF (prgm.expires_in_unit = 'months', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value MONTH),
							DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value YEAR))),'%b %e, %Y'),
							'~|user_jobtitle>|',ifnull(prs.job_title,''),
							'~|tp_code>|',ifnull(prgm.code,''))", "token_str");
					/** 0084991 **/
					if($usertype == "cre_ntn_rpt_mgr"){
						$select->leftJoin('users','mgr1','mgr1.name = mgr.user_name');
						$select->addField('mgr','id','user_id');
						$select->addField('mgr1','uid','send_to_id');
						$select->addField('mgr','full_name','send_to_name');
						$select->addField('mgr1','mail','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
					}
					if($usertype == "cre_ntn_rpt_usr"){
						$select->addField('prs','id','user_id');
						$select->addField('usr','uid','send_to_id');
						$select->addField('prs','full_name','send_to_name');
						$select->addField('usr','mail','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
					}
					if($usertype == "cre_ntn_rpt_adm"){
						$select->addField('prs','id','user_id');
						$select->addExpression('null','send_to_id');
						$select->addExpression('null','send_to_name');
						$select->addExpression('null','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
						$select->addExpression('1', 'custom0');
					}
					/** 0084991**/	
					$select->addExpression('\'php mailer\'', 'send_type');
					$select->addExpression('\'N\'', 'send_status');
					$select->addExpression('1', 'created_by');
					$select->addExpression('now()', 'created_on');
					$select->addExpression('1', 'updated_by');
					$select->addExpression('now()', 'updated_on');
	
					$select->isNotNull('prs.manager_id');
					$select->condition('prgm.object_type', 'cre_sys_obt_crt', '=');
					$select->condition('mse.overall_status', 'lrn_tpm_ovr_cmp', '=');
					$select->isNotNull('mse.comp_date');
					$select->isNotNull('prgm.expires_in_value');
					$select->isNotNull('prgm.expires_in_unit');
					$select->condition('prgm.expires_in_value', 0, '>');
					$select->where("DATEDIFF(
							IF (prgm.expires_in_unit = 'days', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value DAY),
							IF (prgm.expires_in_unit = 'months', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value MONTH),
							DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value YEAR))),
							NOW()) < 0");

					expDebug::dPrintDBAPI('$ManagerCertExpiryRemainderBulkinsertNotification', $select);
					db_insert('slt_bulk_notification')
					->from($select)
					->execute();
				}
			}
			$totTime = $this->processedTime($startTime);
			expDebug::dPrint("end time of ManagerCertExpiryRemainderBulkinsertNotification".gmdate('H:i:s',$totTime),4);
		}catch (Exception $ex) {
			throw new Exception($ex->getMessage());
		}
	}
	
	
	private function LPTerminateReminderBulkinsertNotification(){
		try{
			$startTime = microtime(true);
			expDebug::dPrint("Start time of LPTerminateReminderBulkinsertNotification".$startTime,4);
			expDebug::dPrint(" ManagerCertExpiryRemainderBulkinsertNotification ",4);
	
			$notificationTemplateInfo = $this->getBulkNotificationTemplateInfo('lp_termination_reminder', 'cre_sys_lng_eng');
			expDebug::dPrint(" notificationTemplateInfo ".$notificationTemplateInfo ,5);
			$notifyFlag = ($notificationTemplateInfo['status']  == 'cre_ntn_sts_atv') ? 'Y' : 'N';
			$emailSendTo = explode(',',$notificationTemplateInfo['notification_sendto']);
			$ccEmail = $notificationTemplateInfo['notification_cc'];
			if($notifyFlag == 'Y'){
				foreach ($emailSendTo as $usertype){
					$select = db_select('slt_program','prgm');
					$select->innerJoin('slt_master_enrollment','mse','mse.program_id = prgm.id');
					$select->innerJoin('slt_person','prs','prs.id = mse.user_id and prs.status = \'cre_usr_sts_atv\'');
					$select->innerJoin('users','usr','usr.name = prs.user_name');
					$select->leftJoin('slt_notification_info','info','info.notification_code = \'lp_termination_reminder\' AND  info.lang_code = \'cre_sys_lng_eng\'');
					$select->leftJoin('slt_entity_notification_mapping','map','map.notify_id = info.id AND map.entity_id=prgm.id');
	
					$select->addExpression('\'lp_termination_reminder\'', 'msg_id');
					$select->addExpression("if(find_in_set(prs.preferred_language,'".$notificationTemplateInfo['frmlng']."')>=0, prs.preferred_language,'cre_sys_lng_eng')", 'lang_code');
					$select->addExpression('\'Learning Plan Termination Reminder\'', 'msg_type');
					$select->addExpression("concat('first_name>|',ifnull(prs.first_name,''),
							'~|tp_title>|',ifnull(prgm.title,''),
							'~|tp_description>|',ifnull(prgm.short_desc,''),
							if(prgm.addn_notification_show = 0,'',if(prgm.additional_info IS NOT NULL AND prgm.addn_notification_show = 1,concat('~|add_notes>|',prgm.additional_info,''),'~|add_notes>|-')),
							'~|session_enddate>|',ifnull(prgm.end_date,''),
							'~|session_endtime>|',DATEDIFF(prgm.end_date,NOW()))", "token_str");
						
					//$select->addExpression("concat('objID1>|',prgm.id,'~|objID2>|~|objID3>|~|objID4>|~|objID5>|')","detail_value");
						
					if($usertype == "cre_ntn_rpt_adm"){
						$select->addField('prs','id','user_id');
						$select->addExpression('null','send_to_id');
						$select->addExpression('null','send_to_name');
						$select->addExpression('null','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
						$select->addExpression('1', 'custom0');
					}
					if($usertype == "cre_ntn_rpt_usr"){
						$select->addField('prs','id','user_id');
						$select->addField('usr','uid','send_to_id');
						$select->addField('prs','full_name','send_to_name');
						$select->addField('usr','mail','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
					}
						
						
					if($usertype == "cre_ntn_rpt_mgr"){
						$select->leftJoin('slt_person','mgr','mgr.id = prs.manager_id');
						$select->leftJoin('users','mgr1','mgr1.name = mgr.user_name');
						$select->addField('mgr','id','user_id');
						$select->addField('mgr1','uid','send_to_id');
						$select->addField('mgr','full_name','send_to_name');
						$select->addField('mgr1','mail','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
					}
						
					$select->addExpression('\'php mailer\'', 'send_type');
					$select->addExpression('\'N\'', 'send_status');
					$select->addExpression('1', 'created_by');
					$select->addExpression('now()', 'created_on');
					$select->addExpression('1', 'updated_by');
					$select->addExpression('now()', 'updated_on');
	
	
	
					$select->condition('prgm.object_type','cre_sys_obt_trn','=');
					$select->condition('mse.overall_status','lrn_tpm_ovr_enr','=');
					$select->condition('info.status', 'cre_ntn_sts_atv');
					$select->where("IFNULL(map.status,'Y') = 'Y' AND DATEDIFF(prgm.end_date,NOW())  = IFNULL(map.notify_frequency,info.frequency_data_value)");

					expDebug::dPrintDBAPI(' getLPTerminateDetailsForReminderNotifications SQL:  = ', $select);
					db_insert('slt_bulk_notification')
					->from($select)
					->execute();
	
				}
			}
			$totTime = $this->processedTime($startTime);
			expDebug::dPrint("end time of getLPTerminateDetailsForReminderNotifications".gmdate('H:i:s',$totTime),4);
		}catch (Exception $ex) {
			throw new Exception($ex->getMessage());
		}
	}
	
	private function waitlistBulkExpiryNotification(){
		try{
			$startTime = microtime(true);
			expDebug::dPrint("Start time of waitlistBulkExpiryNotification".$startTime,4);
	
			$timeZone = date_default_timezone(false);
			$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
			$wtlQry = db_select('slt_enrollment','enr');
			$wtlQry->innerJoin('slt_course_class_session','sess','sess.class_id = enr.class_id and sess.id = (select id from slt_course_class_session where class_id = sess.class_id order by start_date limit 1)');
			$wtlQry->addField('enr', 'class_id');
			$wtlQry->addField('enr', 'course_id');
			$wtlQry->addField('sess', 'timezone','session_timezone');
			$wtlQry->condition('enr.reg_status','lrn_crs_reg_wtl','=');
			$wtlQry->addExpression("CONCAT( DATE_FORMAT(sess.start_date, _utf8'%Y-%m-%d'),' ',sess.end_time,':00')", 'session_end_datetime');
			$wtlQry->groupBy('enr.course_id');
			$wtlQry->groupBy('enr.class_id');
			$wtlQry->having("session_end_datetime < IFNULL(CONVERT_TZ(now(), '".$defaultTimezone."' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= sess.timezone)),NOW())");
			expDebug::dPrintDBAPI("query to get the grouped waitlist enrollments by course and class id",$wtlQry);
			$wtlRes = $wtlQry->execute()->fetchAll();
	
			foreach($wtlRes as $resVal){
				expDebug::dPrint("Waitlist Value".print_r($resVal,1),4);
				$this->WaitlistExpiredBulkinsertNotification($resVal->course_id,$resVal->class_id);
			}
			
			$bcnt = 10000;
			$clsBtcDet = $this->getBulkWaitlistExpiredForNotification($bcnt,'COUNT');
			expDebug::dPrint("Count of WaitList items ".print_r($clsBtcDet,true),5);
			
			foreach($clsBtcDet as $btcDet){
				$lCnt = ($btcDet->cnt) ? $btcDet->cnt : 0;
				$crsId = $btcDet->course_id;
				$clsId = $btcDet->class_id;
			for($l=1;$l<=$lCnt;$l++){
				$refund_flag = 0;//0034090: Feedback -- Cancellation of waitlisted seats for priced class
				// Get list of learners to send notification
				expDebug::dPrint("Waitlist notification limit".$l,4);
				$expiryWaitlistTrainingNotifyList = $this->getBulkWaitlistExpiredForNotification($bcnt,'LIST',$l,$crsId,$clsId);
				foreach($expiryWaitlistTrainingNotifyList as $expiryWaitlistTrainingNotifyRow) {
					$updateUserId   = 1;
					$courseId       = $expiryWaitlistTrainingNotifyRow->course_id;
					$classId        = $expiryWaitlistTrainingNotifyRow->class_id;
					$waitListUserId = $expiryWaitlistTrainingNotifyRow->user_id;
					$enrId          = $expiryWaitlistTrainingNotifyRow->enroll_id;
					//0034090: Feedback -- Cancellation of waitlisted seats for priced class
					$list=module_list(false,false,false);
					if(!empty($list['exp_sp_commerce']))
						$refund_flag = automaticRefundForWaitlistCancel($enrId);

					// insert certification expire remainder to notification
					if($refund_flag == 0)
						automaticCancelWaitlistUpdate($enrId);
						
				}
			}
			}
	
	
			$totTime = $this->processedTime($startTime);
			expDebug::dPrint("end time of waitlistBulkExpiryNotification".gmdate('H:i:s',$totTime),4);
	
		} catch (Exception $ex) {
			throw new Exception($ex->getMessage());
		}
	}
	
	/*
	 * To get the list of user, expire in waitlist
	 */
	private function getBulkWaitlistExpiredForNotification($bcnt,$op,$l = '',$crsId = '',$clsId = '') {
	  try{
	  	expDebug::dPrint("Query for waitlist expired aere");
		  // Added by Vincent on Nov 01, 2013 for #0028593
		  $timeZone = date_default_timezone(false);
		  $defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
		  $selectStmt = db_select('slt_enrollment', 'enr');
		  $selectStmt->innerJoin('slt_course_class_session','sess','sess.class_id = enr.class_id and sess.id = (select id from slt_course_class_session where class_id = sess.class_id order by start_date limit 1)');
		  $selectStmt->condition('enr.waitlist_flag', 'lrn_crs_reg_wtl');
		  
		  if($op == 'COUNT'){
		  	$selectStmt->addExpression("CEIL(COUNT(1)/".$bcnt.")", 'cnt');
		  	$selectStmt->addExpression("(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= sess.timezone)", 'session_code');
		  	$selectStmt->addExpression("CONCAT( DATE_FORMAT(sess.start_date, _utf8'%Y-%m-%d'),' ',sess.end_time,':00')", 'session_end_datetime');
		  	$selectStmt->addField('enr','course_id','course_id');
		  	$selectStmt->addField('enr','class_id','class_id');
		  	$selectStmt->groupBy('enr.course_id');
				$selectStmt->groupBy('enr.class_id');
		  	$selectStmt->having("session_end_datetime < IFNULL(CONVERT_TZ(now(), '".$defaultTimezone."' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= session_code)),NOW())");
		  	expDebug::dPrintDBAPI("Query for wailist count",$selectStmt);
		  	$results     = $selectStmt->execute()->fetchAll();
		  //	$results    = ($result[0]->cnt) ? $result[0]->cnt : 0;
		  }else{
		  	$selectStmt->addField('enr','user_id','user_id');
		  	$selectStmt->addField('enr','id','enroll_id');
		  	$selectStmt->addField('enr','course_id','course_id');
		  	$selectStmt->addField('enr','class_id','class_id');
		  	// Added by Vincent on Nov 01, 2013 for #0028593
		  	$selectStmt->addField('sess','timezone','timezone');
		  	$selectStmt->addExpression("CONCAT( DATE_FORMAT(sess.start_date, _utf8'%Y-%m-%d'),' ',sess.end_time,':00')", 'session_end_datetime');
		  	$selectStmt->condition('enr.course_id',$crsId,'=');
		  	$selectStmt->condition('enr.class_id',$clsId,'=');
		  	if($l){
		  		$start = ($l > 1) ? (($l-1) * $bcnt) : 0;
	}
		  	$selectStmt->range($start,$bcnt);
		  	$selectStmt->groupBy("enr.id having session_end_datetime < IFNULL(CONVERT_TZ(now(), '".$defaultTimezone."' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= sess.timezone)),NOW())");
		  	expDebug::dPrintDBAPI("Query for wailist detail ",$selectStmt);
		  	$result     = $selectStmt->execute();
		  	$results    = $result->fetchAll();
		  }
	
		  return $results;
	  }catch (Exception $ex) {
			throw new Exception($ex->getMessage());
	  }
	}
	
	private function WaitlistExpiredBulkinsertNotification($crsId,$clsId){
		try{
			$notificationTemplateInfo = $this->getBulkNotificationTemplateInfo('waitlist_cancellation', 'cre_sys_lng_eng');
			expDebug::dPrint(" notificationTemplateInfo ".$notificationTemplateInfo ,5);
			$notifyFlag = ($notificationTemplateInfo['status']  == 'cre_ntn_sts_atv') ? 'Y' : 'N';
			$emailSendTo = explode(',',$notificationTemplateInfo['notification_sendto']);
			$ccEmail = $notificationTemplateInfo['notification_cc'];
			if($notifyFlag == 'Y'){
				foreach ($emailSendTo as $usertype){
					$timeZone = date_default_timezone(false);
					$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
	
					$pmtDet = t('For details regarding payment, contact Support');
	
					$notificationInfo =	"'first_name>|',ifnull(prs.first_name,''),'~|last_name>|',ifnull(prs.last_name,''),'~|delivery_type>|'
					,ifnull(cls.delivery_type,''),'~|course_title>|',ifnull(crs.title,''),'~|class_title>|',ifnull(cls.title,''),'~|class_description>|',ifnull(cls.short_description,''),'~|full_name>|',ifnull(prs.full_name,''),'~|learning_fullname>|'
					,ifnull(prs.full_name,''),'~|user_name>|',ifnull(prs.user_name,''),'~|user_email>|',ifnull(prs.email,''),'~|user_phone>|',ifnull(prs.phone_no,''),'~|waitlist_position>|',ifnull(enr.waitlist_priority,''),'~|',
					if(cls.addn_notification_show = 0,'',if(cls.additional_info IS NOT NULL AND cls.addn_notification_show = 1,concat('add_notes>|',cls.additional_info,''),'add_notes>|-')),'~|',
					if(ifnull(ord.order_type,'') != 'credit','payment_details>|$pmtDet~|','')";
	
					$classDeliveryType = getClassDeliveryType($clsId, 'Short');
					if ($classDeliveryType == 'ilt' || $classDeliveryType == 'vc') {
						$classSessionInfo = getClassMultiSessionInfoForNotifications($crsId, $clsId,$classDeliveryType,'',true);
						$classInstructorInfo  = getClassInstructorInfoForNotifications($clsId);
						$classSessionInformation = $classSessionInfo[0];
						if ($classSessionInformation == null) {
							$classSessionInformation = 'session_name<| ~~session_startdate<| ~~session_enddate<| ~~session_starttime<| ~~session_endtime<| ~~time_zone<| ~@';
						}
	
						$sessionLocation = $classSessionInfo[1];
						if ($sessionLocation == null) {
							$LocationDel = ' - ';
						} else {						
						$LocationDel = ClassLocationDetails($clsId,$sessionLocation);
						if(count($LocationDel) < 0) $LocationDel = $sessionLocation;	
						}
	
						$notificationInfo = $notificationInfo .
						",'session_location>|','". $LocationDel ."','~|group_sessiondata>|','".$classSessionInformation ."','~|group_instructordata>|','". $classInstructorInfo."~|'";
					}
	
	
					$select = db_select('slt_enrollment', 'enr');
					$select->innerJoin("slt_person",'prs','prs.id = enr.user_id and prs.status = \'cre_usr_sts_atv\'');
					$select->innerJoin('users','usr','usr.name = prs.user_name');
					$select->leftJoin("slt_profile_list_items", "pro","pro.code = prs.time_zone and pro.is_active = 'Y' and prs.time_zone is not null");
					$select->innerJoin('slt_course_template','crs','crs.id = enr.course_id');
					$select->innerJoin('slt_course_class','cls','cls.id = enr.class_id and cls.course_id = crs.id');
					$select->innerJoin('slt_course_class_session','sess','sess.class_id = enr.class_id and sess.id = (select id from slt_course_class_session where class_id = sess.class_id order by start_date limit 1)');
					$select->innerJoin('slt_order','ord','ord.id = enr.order_id');
					$select->addExpression('\'waitlist_cancellation\'', 'msg_id');
					$select->addExpression("if(find_in_set(prs.preferred_language,'".$notificationTemplateInfo['frmlng']."')>=0, prs.preferred_language,'cre_sys_lng_eng')", 'lang_code');
					$select->addExpression('\'Waitlist seat automatically cancelled\'', 'msg_type');
					$select->addExpression("CONCAT(".$notificationInfo.")", "token_str");
						
					if($usertype == "cre_ntn_rpt_adm"){
						$select->addField('prs','id','user_id');
						$select->addExpression('null','send_to_id');
						$select->addExpression('null','send_to_name');
						$select->addExpression('null','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
						$select->addExpression('1', 'custom0');
					}
					if($usertype == "cre_ntn_rpt_usr"){
						$select->addField('prs','id','user_id');
						$select->addField('usr','uid','send_to_id');
						$select->addField('prs','full_name','send_to_name');
						$select->addField('usr','mail','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
					}
						
					if($usertype == "cre_ntn_rpt_mgr"){
						$select->leftJoin('slt_person','mgr','mgr.id = prs.manager_id');
						$select->leftJoin('users','mgr1','mgr1.name = mgr.user_name');
						$select->addField('mgr','id','user_id');
						$select->addField('mgr1','uid','send_to_id');
						$select->addField('mgr','full_name','send_to_name');
						$select->addField('mgr1','mail','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
					}
						
					$select->addExpression('\'php mailer\'', 'send_type');
					$select->addExpression('\'N\'', 'send_status');
					$select->addExpression('1', 'created_by');
					$select->addExpression('now()', 'created_on');
					$select->addExpression('1', 'updated_by');
					$select->addExpression('now()', 'updated_on');
	
					$select->condition('enr.waitlist_flag', 'lrn_crs_reg_wtl');
					$select->condition('enr.course_id', $crsId,'=');
					$select->condition('enr.class_id', $clsId,'=');
	
					$select->groupBy("enr.id");
					$select->where("CONCAT(DATE_FORMAT(sess.start_date, _utf8'%Y-%m-%d'),' ',sess.end_time,':00') < IFNULL(CONVERT_TZ(now(), '".$defaultTimezone."' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= sess.timezone)),NOW())");

					expDebug::dPrintDBAPI("Waitlist notification query",$select);
					db_insert('slt_bulk_notification')
					->from($select)
					->execute();
	
				}
			}
		}catch (Exception $ex) {
			throw new Exception($ex->getMessage());
		}
	}
	
	
	private function getBulkExpiredWbtContent(){
		try{
			
			// Update the enrollment status as incomplete
			try{
				$updQry = "UPDATE slt_enrollment enr
									 INNER JOIN (SELECT map.class_id, max(map.validity_days) AS maxcnt,count(map.class_id) AS clscount, SUM(IF(map.validity_days IS NULL,0,1)) AS validnull,
							      enr.reg_date, DATE_ADD(enr.reg_date,INTERVAL min(map.validity_days) DAY) AS expire_date,enr.id AS id,enr.user_id AS user_id
							      FROM slt_course_content_mapper map
							      LEFT JOIN slt_enrollment enr on enr.class_id = map.class_id
							      where enr.comp_status IN (:enrsts ,:inpsts )
							      group by map.class_id,enr.user_id,enr.id
							      HAVING validnull > 0 AND date_format(expire_date,'%Y-%m-%d') < date_format(now(),'%Y-%m-%d')) x on x.id = enr.id AND x.user_id = enr.user_id
									 SET enr.comp_status = :incsts , enr.comp_date = now(), enr.updated_on = now(), enr.updated_by = 1
									 WHERE enr.reg_status = :regsts AND enr.comp_status IN (:enrsts ,:inpsts )";
				
				$arg = array(':incsts' => 'lrn_crs_cmp_inc',':regsts' => 'lrn_crs_reg_cnf',':enrsts' =>'lrn_crs_cmp_enr', ':inpsts' => 'lrn_crs_cmp_inp');
				
				expDebug::dPrintDBAPI("Query for update the expired wbt content",$updQry,$arg);
				db_query($updQry,$arg);
			}catch(Exception $ex){
				throw new Exception($ex->getMessage());
			}
			
			// Update overall_status of tp as incomplete
			try{
			$updMenrQry = "UPDATE slt_enrollment enr
	     INNER join slt_master_enrollment ms on enr.master_enrollment_id = ms.id
	     INNER join slt_module_crs_mapping map on map.program_id = ms.program_id 
	     SET ms.overall_status = :mincsts , ms.comp_date = now(), ms.updated_on = now(), ms.updated_by = 1
		   WHERE enr.comp_status = :incsts AND ms.overall_status = :enrsts
	     AND enr.master_enrollment_id IS NOT NULL AND map.is_required = 'Y' AND enr.course_id = map.course_id";
			
			$args = array(':incsts' => 'lrn_crs_cmp_inc',':enrsts' =>'lrn_tpm_ovr_enr', ':mincsts' => 'lrn_tpm_ovr_inc');

			expDebug::dPrintDBAPI("Query for update the expired wbt content in tp",$updMenrQry,$args);
			db_query($updMenrQry,$args);
			}catch(Exception $ex){
				throw new Exception($ex->getMessage());
			}
			
		} catch (Exception $ex) {
			throw new Exception($ex->getMessage());
		}
	}
	
	private function bulkUpdateClassAsCompleted(){
		try{
			expDebug::dPrint("bulkUpdateClassAsCompleted",4);
			$timeZone = date_default_timezone(false);
			$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
			$upd = "UPDATE slt_course_class cls
			LEFT JOIN slt_course_class_session ses ON cls.id = ses.class_id and ses.id = (select id from slt_course_class_session where class_id = ses.class_id order by start_date desc limit 1)
			LEFT JOIN slt_profile_list_items splitz ON ses.timezone = splitz.code AND ses.timezone IS NOT NULL AND splitz.lang_code = :lng
			SET cls.status = :dld
			WHERE (cls.status = :atv ) AND (cls.delivery_type IN  ( :ilt , :vcl ))
			AND (IFNULL(CONVERT_TZ(CONCAT(DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'), ' ', ses.end_time, ':00'), :tmz ,splitz.attr2),NOW()) <= IFNULL(CONVERT_TZ(NOW(), :tmz ,splitz.attr2),NOW()))";
	
			$args = array(':tmz' => $defaultTimezone, ':atv' => 'lrn_cls_sts_atv', ':dld' => 'lrn_cls_sts_dld',
					':lng' => 'cre_sys_lng_eng',':ilt' => 'lrn_cls_dty_ilt', ':vcl' => 'lrn_cls_dty_vcl');
	
			expDebug::dPrintDBAPI('Query for update the class as completed',$upd,$args);
			db_query($upd,$args);
	
		} catch (Exception $ex) {
			throw new Exception($ex->getMessage());
		}
	}
	
	/**
	 *  Automatic expired compliance courses to in-complete tab
	 */
	function autoComplianceIncompleteBulkNotification() {
		// $txn = db_transaction();
		try{
			$startTime = microtime(true);
			try{
				expDebug::dPrint("Start time of autoComplianceIncompleteBulkNotification".$startTime,4);
				expDebug::dPrint(" autoComplianceIncompleteBulkNotification ",4);
				expDebug::dPrint(" notificationTemplateInfo ".$notificationTemplateInfo ,5);
				
				$compRes = $this->autocomplianceIncompleteDetails();
					
				// Compliance In complete notification
				$notificationTemplateInfo1 = $this->getBulkNotificationTemplateInfo('compliance_class_incomplete', 'cre_sys_lng_eng');
				
				// Non- compliance in complete notification
				$notificationTemplateInfo2 = $this->getBulkNotificationTemplateInfo('class_incomplete', 'cre_sys_lng_eng');
				
				foreach($compRes as $comp){
					$courseId= $comp->course_id;
					$classId = $comp->class_id;
					if($comp->is_compliance){
						$notifyFlag = ($notificationTemplateInfo1['status']  == 'cre_ntn_sts_atv') ? 'Y' : 'N';
						$emailSendTo = explode(',',$notificationTemplateInfo1['notification_sendto']);
						$ccEmail = $notificationTemplateInfo1['notification_cc'];
						$msgtype = 'Compliance Course class Incomplete Reminder';
						$msg_id = 'compliance_class_incomplete';
					}else{
						$notifyFlag = ($notificationTemplateInfo2['status']  == 'cre_ntn_sts_atv') ? 'Y' : 'N';
						$emailSendTo = explode(',',$notificationTemplateInfo2['notification_sendto']);
						$ccEmail = $notificationTemplateInfo2['notification_cc'];
						$msgtype = 'Course class Incomplete Reminder';
						$msg_id = 'class_incomplete';
					}
					
					
					$notificationInfo =	"'first_name>|',ifnull(prs.first_name,''),
					'~|last_name>|',ifnull(prs.last_name,''),
					'~|delivery_type>|',ifnull(prof.name,''),
					'~|course_title>|',ifnull(crs.title,''),
					'~|class_title>|',ifnull(cls.title,''),
					'~|class_description>|',ifnull(cls.short_description,''),
					if(cls.addn_notification_show = 0,'',if(cls.additional_info IS NOT NULL AND cls.addn_notification_show = 1,concat('~|add_notes>|',cls.additional_info,''),'~|add_notes>|-')),
					'~|full_name>|',ifnull(prs.full_name,''),
					'~|learning_fullname>|',ifnull(prs.full_name,''),
					'~|user_name>|',ifnull(prs.user_name,''),
					'~|user_email>|',ifnull(prs.email,''),
					'~|user_phone>|',ifnull(prs.phone_no,''),
					'~|waitlist_position>|',ifnull(enr.waitlist_priority,''),
					'~|compliance_mandatory>|Compliance~|'";
		
					$classDeliveryType = getClassDeliveryType($classId, 'Short');
		
					if ($classDeliveryType == 'ilt' || $classDeliveryType == 'vc') {
						// Get class session details
						/* Added extra param = class del type attr*/
						$classSessionInfo = getClassMultiSessionInfoForNotifications($courseId, $classId,$classDeliveryType);
						$classInstructorInfo  = getClassInstructorInfoForNotifications($classId);
						$timeZone = '';
						$classSessionInformation = $classSessionInfo[0];
						if ($classSessionInformation == null) {
							$classSessionInformation = 'session_name<| ~~session_startdate<| ~~session_enddate<| ~~session_starttime<| ~~session_endtime<| ~~time_zone<| ~@';
						}
							
						$sessionLocation = $classSessionInfo[1];
						if ($sessionLocation == null) {
							$sessionLocation = '';
						}
						// Append tokens string with class session details
						$notificationInfo  = $notificationInfo .
						",'session_location>|','". $sessionLocation ."','~|group_sessiondata>|','".$classSessionInformation ."','~|group_instructordata>|','". $classInstructorInfo."~|'";
		
					}
					else {
						// Append launch url for wbt class to the tokens string
						$launchUrlinfo = getClassLaunchUrl($courseId, $classId);
						expDebug::dPrint("launch url-->>".print_r($launchUrlinfo,true),5);
						$launchUrl = convertNullValToNullStr($launchUrlinfo['launch_url']);
						$notificationInfo = $notificationInfo .
						",'launch_url>|','" . $launchUrl['launch_url']. "~|'";
					}
		
					$notificationInfo = $notificationInfo .
					",'dt_code>|','" . $classDeliveryType. "~|'";
					if($notifyFlag == 'Y'){
						foreach ($emailSendTo as $usertype){
							$select = db_select('slt_course_template','crs');
							$select->innerJoin('slt_enrollment','enr','enr.course_id = crs.id and (enr.is_compliance=1 || crs.compliance_completed is not null )');
							$select->innerJoin('slt_course_class','cls','cls.course_id = crs.id and cls.id = enr.class_id');
							$select->innerJoin('slt_profile_list_items','prof','prof.code = cls.delivery_type');
							$select->innerJoin('slt_person','prs','prs.id = enr.user_id and prs.status = \'cre_usr_sts_atv\'');
							$select->innerJoin('users','usr','usr.name = prs.user_name');
		
							$select->addExpression("'".$msg_id."'", 'msg_id');
							
							$select->addExpression("if(find_in_set(prs.preferred_language,'".$notificationTemplateInfo['frmlng']."')>=0, prs.preferred_language,'cre_sys_lng_eng')", 'lang_code');
							$select->addExpression("'".$msgtype."'", 'msg_type');
							$select->addExpression("concat(".$notificationInfo.")", "token_str");
		
							if($usertype == "cre_ntn_rpt_adm"){
								$select->addField('prs','id','user_id');
								$select->addExpression('null','send_to_id');
								$select->addExpression('null','send_to_name');
								$select->addExpression('null','send_to_email');
								$select->addExpression('null', 'send_to_email_cc');
								$select->addExpression('1', 'custom0');
							}
							if($usertype == "cre_ntn_rpt_usr"){
								$select->addField('prs','id','user_id');
								$select->addField('usr','uid','send_to_id');
								$select->addField('prs','full_name','send_to_name');
								$select->addField('usr','mail','send_to_email');
								$select->addExpression('null', 'send_to_email_cc');
							}
		
		
							if($usertype == "cre_ntn_rpt_mgr"){
								$select->leftJoin('slt_person','mgr','mgr.id = prs.manager_id');
								$select->leftJoin('users','mgr1','mgr1.name = mgr.user_name');
								$select->addField('mgr','id','user_id');
								$select->addField('mgr1','uid','send_to_id');
								$select->addField('mgr','full_name','send_to_name');
								$select->addField('mgr1','mail','send_to_email');
								$select->addExpression('null', 'send_to_email_cc');
							}
		
							$select->addExpression('\'php mailer\'', 'send_type');
							$select->addExpression('\'N\'', 'send_status');
							$select->addExpression('1', 'created_by');
							$select->addExpression('now()', 'created_on');
							$select->addExpression('1', 'updated_by');
							$select->addExpression('now()', 'updated_on');
		
						//	$select->condition(db_or()->condition('crs.compliance_completed','hire_days','=')
							//		->condition('crs.is_compliance',1,'='));
							//$select->condition('crs.is_compliance', 1,'=');
							$select->condition('enr.course_id', $courseId,'=');
							$select->condition('enr.class_id', $classId,'=');
							$select->condition('enr.comp_status', array('lrn_crs_cmp_enr','lrn_crs_cmp_inp'),'IN');
							$select->where("if( (crs.complete_days IS NOT NULL AND crs.complete_days != '') or (crs.complete_date IS NOT NULL AND crs.complete_date != ''),
									(CURDATE() >= (CASE WHEN (crs.complete_days IS NOT NULL AND crs.complete_days != '')
									THEN
									(date_format(date_add(if((crs.compliance_completed = 'hire_days'), IF(prs.hire_date is not null, prs.hire_date, 1=2), enr.reg_date),interval crs.complete_days+1 day),'%Y-%m-%d'))
									WHEN (crs.complete_date IS NOT NULL AND crs.complete_date != '')
									THEN
									(date_format(date_add(crs.complete_date,interval 1 day),'%Y-%m-%d'))
									END)),1=0)");

							expDebug::dPrintDBAPI(' autoComplianceIncompleteBulkNotification SQL:  = ', $select);
							db_insert('slt_bulk_notification')
							->from($select)
							->execute();
						}
					}
				}
			}catch(Exception $ex){
				throw new Exception($ex->getMessage());
			}
			
			try{
				// update incomplete enrollments when course is value set for completed by days or date.
				$updqry1 = "UPDATE slt_enrollment a
				INNER JOIN slt_course_template crs on a.course_id = crs.id
				INNER JOIN slt_person per on a.user_id = per.id
				SET
				a.comp_status = 'lrn_crs_cmp_inc',
				a.comp_on = now(),
				a.comp_date = now(),
				a.comp_by = 1,
				a.updated_by = 1,
				a.updated_on = now()
				where
				a.master_enrollment_id is null AND
				((crs.complete_days IS NOT NULL AND crs.complete_days != '') OR (crs.complete_date IS NOT NULL AND crs.complete_date != '') )		
				AND (a.comp_status IN('lrn_crs_cmp_enr','lrn_crs_cmp_inp'))
				AND (if(( ifnull(crs.complete_days,'') != '') or ( ifnull(crs.complete_date,'') != ''),
				(CURDATE() >= (CASE WHEN (crs.complete_days IS NOT NULL AND crs.complete_days != '')
				THEN
				(date_format(date_add(if((crs.compliance_completed = 'hire_days'), IF(per.hire_date is not null, per.hire_date, 1=2), a.reg_date),interval crs.complete_days+1 day),'%Y-%m-%d'))
				WHEN (ifnull(crs.complete_date,'') != '')
				THEN
				(date_format(date_add(crs.complete_date,interval 1 day),'%Y-%m-%d'))
				END)),1=0)) ";
		
				expDebug::dPrint('Update skill :: '. $updqry1);
				db_query($updqry1);
			}catch(Exception $ex){
				$plist = db_query('show full processlist')->fetchAll();
				expDebug::dPrint("Process List ".print_r($plist,true),1);
				throw new Exception($ex->getMessage());
			}
			$totTime = $this->processedTime($startTime);
			expDebug::dPrint("end time of autoComplianceIncompleteBulkNotification".gmdate('H:i:s',$totTime),4);
		}catch (Exception $ex) {
			throw new Exception($ex->getMessage());
		}
	}
	
	/**
	 *  Compliance/Mandatory reminder notification for completed due date/ day
	 */
	private function autoComplianceMandatoryCompletionDaysBulkReminder($field){
		try{
			if($field == 'is_compliance'){
				$val = 1;
			}else{
				$val = 'Y';
			}
			$is_compliance ='';
			if($field == 'is_compliance'){
				$mandatoryCompliance = 'compliance_class_remind_session';
				$mandatoryCompMsg = ' Compliance Course class Reminder';
				$type = 'Compliance';
				$is_compliance = 1;
			}else{
				$mandatoryCompliance = 'mandatory_class_remind_session';
				$mandatoryCompMsg = ' Mandatory Course class Reminder';
				$type = 'Mandatory';
			}
			expDebug::dPrint(" autoComplianceMandatoryCompletionDaysBulkReminder ".$field,4);
			$startTime = microtime(true);
			expDebug::dPrint("Start time of autoComplianceMandatoryCompletionDaysBulkReminder".$startTime,4);
			$select = db_select('slt_enrollment','enr');
			$select->innerJoin('slt_person','per','enr.user_id = per.id');
			$select->innerJoin('slt_course_class','cls','cls.id = enr.class_id');
			$select->innerJoin('slt_course_template','crs','crs.id = cls.course_id');
			$select->innerJoin('slt_notification_info','notify','notify.notification_code='."'".$mandatoryCompliance."'");
			//if($field == 'is_compliance'){
			$select->innerJoin('slt_notification_info','notify2','notify2.notification_code= "class_completion_remind"');
			//}
			$select->addExpression('MOD(DATEDIFF(curdate(),IF((crs.complete_date IS NULL AND crs.complete_days IS NULL),(DATE_ADD(enr.reg_date, interval notify.notify_before day)),IF(crs.complete_date IS NOT NULL,DATE_SUB(crs.complete_date,interval notify.notify_before day),
					DATE_SUB(DATE_ADD(enr.reg_date, INTERVAL crs.complete_days DAY),interval notify.notify_before day)))),notify.frequency_data_value)','resultcnt');
			$select->addField('enr','course_id', 'course_id');
			$select->addField('enr','class_id', 'class_id');
			$select->addField('enr','user_id', 'user_id');
			$select->addField('enr','is_compliance', 'is_compliance');
			$select->addField('enr','mandatory', 'mandatory');
			$select->addExpression("DATE_FORMAT(DATE_ADD(if(crs.compliance_completed = 'hire_days',per.hire_date,enr.reg_date), INTERVAL crs.complete_days DAY),'%d %M %Y')", 'complete_days');
			$select->addExpression("DATE_FORMAT(crs.complete_date,'%d %M %Y')",'complete_date');
			if($field == 'is_compliance'){
				//$select->condition('enr.'.$field, $val,'=');
				$select->condition(db_or()->condition('enr.'.$field, $val,'=')
						->condition('crs.compliance_completed','', 'IS NOT NULL'));
				$select->condition('enr.mandatory','', 'IS NULL');
			}else{ 
			$select->condition('enr.'.$field, $val,'=');
			}
			$select->condition('cls.delivery_type', array('lrn_cls_dty_wbt','lrn_cls_dty_vod'),'IN');
			$select->condition('enr.comp_status',array('lrn_crs_cmp_inp','lrn_crs_cmp_enr'),'IN');
			// $select->where("curdate() > DATE_FORMAT(DATE_ADD(enr.reg_date, interval notify.notify_before day),'%Y-%m-%d')");
			$select->where("curdate() >= IF((crs.complete_date IS NULL AND crs.complete_days IS NULL),DATE_FORMAT(DATE_ADD(enr.reg_date, interval notify.notify_before day),'%Y-%m-%d'),
					IF(crs.complete_date IS NOT NULL,DATE_FORMAT(DATE_SUB(crs.complete_date,interval notify.notify_before day),'%Y-%m-%d'),
					DATE_FORMAT(DATE_SUB(DATE_ADD(if(crs.compliance_completed = 'hire_days',IF(per.hire_date is not null, per.hire_date, 1=2), enr.reg_date), INTERVAL crs.complete_days DAY),interval if(crs.compliance_completed = 'hire_days',notify2.notify_before,notify.notify_before) day),'%Y-%m-%d')))");
	
			$select->where("MOD(DATEDIFF(curdate(),IF((crs.complete_date IS NULL AND crs.complete_days IS NULL),(DATE_ADD(enr.reg_date, interval notify.notify_before day)),IF(crs.complete_date IS NOT NULL,DATE_SUB(crs.complete_date,interval notify.notify_before day),
					DATE_SUB(DATE_ADD(if(crs.compliance_completed = 'hire_days',IF(per.hire_date is not null, per.hire_date, 1=2), enr.reg_date), INTERVAL crs.complete_days DAY),interval if(crs.compliance_completed = 'hire_days',notify2.notify_before,notify.notify_before) day)))),notify.frequency_data_value) = 0");
			//$select->where("MOD(DATEDIFF(curdate(),(DATE_ADD(enr.reg_date, interval notify.notify_before day))),notify.frequency_data_value) = 0");
			$select->groupBy('enr.course_id');
			$select->groupBy('enr.class_id');
			
			expDebug::dPrintDBAPI(' Select Query for Compliance reminder notification = ' , $select);
			$result =  $select->execute()->fetchAll();
			expDebug::dPrint("Select Result for Compliance reminder notification ".print_r($result,true),3);
	
			$notificationTemplateInfo = $this->getBulkNotificationTemplateInfo($mandatoryCompliance, 'cre_sys_lng_eng');
			if($is_compliance == 1)
				$notificationTemplateInfo1 = $this->getBulkNotificationTemplateInfo('class_completion_remind', 'cre_sys_lng_eng');
			expDebug::dPrint(" notificationTemplateInfo ".print_r($notificationTemplateInfo,true) ,5);
			
			/* $notifyFlag = ($notificationTemplateInfo['status']  == 'cre_ntn_sts_atv') ? 'Y' : 'N';
			$emailSendTo = explode(',',$notificationTemplateInfo['notification_sendto']);
			$ccEmail = $notificationTemplateInfo['notification_cc']; */
			foreach($result as $res){
				$courseId= $res->course_id;
				$classId = $res->class_id;
				$comp_val = $res->is_compliance;
				$mand_val = $res->mandatory;
				if($comp_val == 1 || $mand_val == 'Y'){
					$notifyFlag = ($notificationTemplateInfo['status']  == 'cre_ntn_sts_atv') ? 'Y' : 'N';
					$emailSendTo = explode(',',$notificationTemplateInfo['notification_sendto']);
					$ccEmail = $notificationTemplateInfo['notification_cc'];
					$mandatoryCompliance = ($comp_val == 1) ? 'compliance_class_remind_session' : 'mandatory_class_remind_session';
					$mandatoryCompMsg = ($comp_val == 1) ?  'Compliance Course class Reminder' : ' Mandatory Course class Reminder';
					$notify_before = $notificationTemplateInfo['notify_before'];
					$frequency_value = $notificationTemplateInfo['frequency_data_value'];
				}else{
					$notifyFlag = ($notificationTemplateInfo1['status']  == 'cre_ntn_sts_atv') ? 'Y' : 'N';
					$emailSendTo = explode(',',$notificationTemplateInfo1['notification_sendto']);
					$ccEmail = $notificationTemplateInfo1['notification_cc'];
					$mandatoryCompliance = 'class_completion_remind';
					$mandatoryCompMsg = ' Course class Reminder';
					$notify_before = $notificationTemplateInfo1['notify_before'];
					$frequency_value = $notificationTemplateInfo1['frequency_data_value'];
				}
				$completecomplete_date = $res->complete_date;
				$completecomplete_days = $res->complete_days;
				$notificationInfo =	"'first_name>|',ifnull(prs.first_name,''),
				'~|last_name>|',ifnull(prs.last_name,''),
				'~|delivery_type>|',ifnull(prof.name,''),
				'~|course_title>|',ifnull(crs.title,''),
				'~|class_title>|',ifnull(cls.title,''),
				'~|class_description>|',ifnull(cls.short_description,''),
				'~|full_name>|',ifnull(prs.full_name,''),
				'~|learning_fullname>|',ifnull(prs.full_name,''),
				'~|user_name>|',ifnull(prs.user_name,''),
				'~|user_email>|',ifnull(prs.email,''),
				'~|user_phone>|',ifnull(prs.phone_no,''),
				'~|waitlist_position>|',ifnull(enr.waitlist_priority,''),'~|".
				(($is_compliance == 1) ? ("compliance_mandatory>|Compliance~|") : '');
				
				$classInfo = getClassInfoForNotifications($classId);
				$classadditionalinfo = convertNullValToNullStr($classInfo['additional_info']);
                $classissnoti = convertNullValToNullStr($classInfo['addn_notification_show']);
	
				$classDeliveryType = getClassDeliveryType($classId, 'Short');
	
				// Append launch url for wbt class to the tokens string
				$launchUrlinfo = getClassLaunchUrl($courseId, $classId);
				expDebug::dPrint("launch url-->>".print_r($launchUrlinfo,true),5);
				$launchUrl = convertNullValToNullStr($launchUrlinfo['launch_url']);
				$notificationInfo = $notificationInfo .
				"launch_url>|','" . $launchUrl['launch_url']. "~|'";
	
				$notificationInfo = $notificationInfo .
				",'dt_code>|','" . $classDeliveryType. "~|course_code>|',ifnull(cls.code,''),
				'~|compliance_mandatory>|','" . $type. "~|'";
				
				if($classissnoti == 1){
					if($classadditionalinfo != ""){
						$notificationInfo  = $notificationInfo .
						",'add_notes>|','" . $classadditionalinfo. "~|'";
					}
					else{
						$notificationInfo  = $notificationInfo .
						",'add_notes>|-~|'";
					}
				}
				if($field == 'is_compliance'){
					if($completecomplete_date != ""){
						$notificationInfo  = $notificationInfo .
						",'complete_by_date>|','" . $completecomplete_date. "~|'";
					}elseif($completecomplete_days != ""){
						$notificationInfo  = $notificationInfo .
						",'complete_by_date>|','" . $completecomplete_days. "~|'";
					}else{
						$notificationInfo  = $notificationInfo .
						",'complete_by_date>|-~|'";
					}
				}
	
				if($notifyFlag == 'Y'){
					foreach ($emailSendTo as $usertype){
						$select = db_select('slt_course_template','crs');
						$select->innerJoin('slt_enrollment','enr','enr.course_id = crs.id');
						$select->innerJoin('slt_person','per','enr.user_id = per.id');
						$select->innerJoin('slt_course_class','cls','cls.course_id = crs.id and cls.id = enr.class_id');
						//$select->innerJoin("slt_notification_info","notify","notify.notification_code=".$notificationTemplateInfo['notify_before'] .");
						$select->innerJoin('slt_profile_list_items','prof','prof.code = cls.delivery_type');
						$select->innerJoin('slt_person','prs','prs.id = enr.user_id and prs.status = \'cre_usr_sts_atv\'');
						$select->innerJoin('users','usr','usr.name = prs.user_name');
	
						$select->addExpression("'".$mandatoryCompliance."'", "msg_id");
						$select->addExpression("if(find_in_set(prs.preferred_language,'".$notificationTemplateInfo['frmlng']."')>=0, prs.preferred_language,'cre_sys_lng_eng')", 'lang_code');
						$select->addExpression("'".$mandatoryCompMsg."'", "msg_type");
						$select->addExpression("concat(".$notificationInfo.")", "token_str");
	
						//$select->addExpression("concat('objID1>|',prgm.id,'~|objID2>|~|objID3>|~|objID4>|~|objID5>|')","detail_value");
	
						if($usertype == "cre_ntn_rpt_adm"){
							$select->addField('prs','id','user_id');
							$select->addExpression('null','send_to_id');
							$select->addExpression('null','send_to_name');
							$select->addExpression('null','send_to_email');
							$select->addExpression('null', 'send_to_email_cc');
							$select->addExpression('1', 'custom0');
						}
						if($usertype == "cre_ntn_rpt_usr"){
							$select->addField('prs','id','user_id');
							$select->addField('usr','uid','send_to_id');
							$select->addField('prs','full_name','send_to_name');
							$select->addField('usr','mail','send_to_email');
							$select->addExpression('null', 'send_to_email_cc');
						}
	
	
						if($usertype == "cre_ntn_rpt_mgr"){
							$select->leftJoin('slt_person','mgr','mgr.id = prs.manager_id');
							$select->leftJoin('users','mgr1','mgr1.name = mgr.user_name');
							$select->addField('mgr','id','user_id');
							$select->addField('mgr1','uid','send_to_id');
							$select->addField('mgr','full_name','send_to_name');
							$select->addField('mgr1','mail','send_to_email');
							$select->addExpression('null', 'send_to_email_cc');
						}
	
						$select->addExpression('\'php mailer\'', 'send_type');
						$select->addExpression('\'N\'', 'send_status');
						$select->addExpression('1', 'created_by');
						$select->addExpression('now()', 'created_on');
						$select->addExpression('1', 'updated_by');
						$select->addExpression('now()', 'updated_on');
						if($field == 'is_compliance'){
							//$select->condition('enr.'.$field, $val,'=');
							$select->condition(db_or()->condition('crs.compliance_completed','','IS NOT NULL')
									->condition('enr.'.$field, $val,'='));
							$select->condition('enr.mandatory','', 'IS NULL');
						}else{
							$select->condition('enr.'.$field, $val,'=');
						}
						$select->condition('enr.course_id',$courseId,'=');
						$select->condition('enr.class_id',$classId,'=');
						$select->condition('enr.comp_status', array('lrn_crs_cmp_enr','lrn_crs_cmp_inp'),'IN');
						$select->where("curdate() >= IF((crs.complete_date IS NULL AND crs.complete_days IS NULL),DATE_FORMAT(DATE_ADD(enr.reg_date, interval ".$notify_before." day),'%Y-%m-%d'),
								IF(crs.complete_date IS NOT NULL,DATE_FORMAT(DATE_SUB(crs.complete_date,interval ".$notify_before." day),'%Y-%m-%d'),
								DATE_FORMAT(DATE_SUB(DATE_ADD(if(crs.compliance_completed = 'hire_days',IF(per.hire_date is not null, per.hire_date, 1=2), enr.reg_date), INTERVAL crs.complete_days DAY),interval ".$notify_before." day),'%Y-%m-%d')))");
	
						$select->where("MOD(DATEDIFF(curdate(),IF((crs.complete_date IS NULL AND crs.complete_days IS NULL),(DATE_ADD(enr.reg_date, interval ".$notify_before." day)),IF(crs.complete_date IS NOT NULL,DATE_SUB(crs.complete_date,interval ".$notify_before." day),
								DATE_SUB(DATE_ADD(if(crs.compliance_completed = 'hire_days',IF(per.hire_date is not null, per.hire_date, 1=2), enr.reg_date), INTERVAL crs.complete_days DAY),interval ".$notify_before." day)))),".$frequency_value.") = 0");

						expDebug::dPrintDBAPI(' autoComplianceMandatoryCompletionDaysBulkReminder SQL:  = ', $select);
						db_insert('slt_bulk_notification')
						->from($select)
						->execute();
					}
				}
				$totTime = $this->processedTime($startTime);
				expDebug::dPrint("end time of autoComplianceMandatoryCompletionDaysBulkReminder".gmdate('H:i:s',$totTime),4);
			}
		}catch (Exception $ex) {
			throw new Exception($ex->getMessage());
		}
	}
	
	
	/**
	 *  Mandatory reminder notification for completed due date/ day (TP)
	 */
	private function autoMandatoryCompletionDaysBulkReminderForTP(){
		try{
	
			expDebug::dPrint(" autoMandatoryCompletionDaysBulkReminderForTP ",4);
			$startTime = microtime(true);
			expDebug::dPrint("Start time of autoMandatoryCompletionDaysBulkReminderForTP".$startTime,4);
			$notificationTemplateInfo = $this->getBulkNotificationTemplateInfo('mandatory_class_remind_session', 'cre_sys_lng_eng');
			expDebug::dPrint(" notificationTemplateInfo ".$notificationTemplateInfo ,5);
			$notifyFlag = ($notificationTemplateInfo['status']  == 'cre_ntn_sts_atv') ? 'Y' : 'N';
			$emailSendTo = explode(',',$notificationTemplateInfo['notification_sendto']);
			$ccEmail = $notificationTemplateInfo['notification_cc'];
			$notificationInfo =	"'first_name>|',ifnull(prs.first_name,''),
			'~|last_name>|',ifnull(prs.last_name,''),
			'~|course_title>|',ifnull(prgm.title,''),
			'~|tp_description>|',ifnull(prgm.short_desc,''),
			'~|course_code>|',ifnull(prgm.code,''),
			'~|full_name>|',ifnull(prs.full_name,''),
			'~|learning_fullname>|',ifnull(prs.full_name,''),
			'~|user_name>|',ifnull(prs.user_name,''),
			'~|user_email>|',ifnull(prs.email,''),
			'~|user_phone>|',ifnull(prs.phone_no,''),
			if(prgm.addn_notification_show = 0,'',if(prgm.additional_info IS NOT NULL AND prgm.addn_notification_show = 1,concat('~|add_notes>|',prgm.additional_info,'~|'),'~|add_notes>|-~|')),
			'~|compliance_mandatory>|Mandatory~|'";
			
			
	
			if($notifyFlag == 'Y'){
				foreach ($emailSendTo as $usertype){
					$select = db_select('slt_program','prgm');
					$select->innerJoin('slt_master_enrollment','mse','mse.program_id = prgm.id');
					$select->innerJoin('slt_person','prs','prs.id = mse.user_id and prs.status = \'cre_usr_sts_atv\'');
					$select->innerJoin('users','usr','usr.name = prs.user_name');
	
					$select->addExpression('\'mandatory_class_remind_session\'', 'msg_id');
					$select->addExpression("if(find_in_set(prs.preferred_language,'".$notificationTemplateInfo['frmlng']."')>=0, prs.preferred_language,'cre_sys_lng_eng')", 'lang_code');
					$select->addExpression('\' Mandatory TP Reminder\'', 'msg_type');
					$select->addExpression("concat(".$notificationInfo.")", "token_str");
						
					//$select->addExpression("concat('objID1>|',prgm.id,'~|objID2>|~|objID3>|~|objID4>|~|objID5>|')","detail_value");
						
					if($usertype == "cre_ntn_rpt_adm"){
						$select->addField('prs','id','user_id');
						$select->addExpression('null','send_to_id');
						$select->addExpression('null','send_to_name');
						$select->addExpression('null','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
						$select->addExpression('1', 'custom0');
					}
					if($usertype == "cre_ntn_rpt_usr"){
						$select->addField('prs','id','user_id');
						$select->addField('usr','uid','send_to_id');
						$select->addField('prs','full_name','send_to_name');
						$select->addField('usr','mail','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
					}
						
						
					if($usertype == "cre_ntn_rpt_mgr"){
						$select->leftJoin('slt_person','mgr','mgr.id = prs.manager_id');
						$select->leftJoin('users','mgr1','mgr1.name = mgr.user_name');
						$select->addField('mgr','id','user_id');
						$select->addField('mgr1','uid','send_to_id');
						$select->addField('mgr','full_name','send_to_name');
						$select->addField('mgr1','mail','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
					}
						
					$select->addExpression('\'php mailer\'', 'send_type');
					$select->addExpression('\'N\'', 'send_status');
					$select->addExpression('1', 'created_by');
					$select->addExpression('now()', 'created_on');
					$select->addExpression('1', 'updated_by');
					$select->addExpression('now()', 'updated_on');
	
	
	
					$select->condition('mse.mandatory', '1','=');
					$select->condition('mse.overall_status',array('lrn_tpm_ovr_inp','lrn_tpm_ovr_enr'),'IN');
					$select->where("curdate() >= DATE_FORMAT(DATE_ADD(mse.reg_date, interval ".$notificationTemplateInfo['notify_before']." day),'%Y-%m-%d')");
					$select->where("MOD(DATEDIFF(curdate(),(DATE_ADD(mse.reg_date, interval ".$notificationTemplateInfo['notify_before']." day))),".$notificationTemplateInfo['frequency_data_value'].") = 0");
					
					expDebug::dPrintDBAPI(' autoMandatoryCompletionDaysBulkReminderForTP SQL:  = ', $select);
					db_insert('slt_bulk_notification')
					->from($select)
					->execute();
	
				}
			}
			$totTime = $this->processedTime($startTime);
			expDebug::dPrint("end time of autoMandatoryCompletionDaysBulkReminderForTP".gmdate('H:i:s',$totTime),4);
	
		}catch (Exception $ex) {
			throw new Exception($ex->getMessage());
		}
	}
	
	private function autoComplianceBulkExpiredReminder(){
		try{
	
			$this->notificationForComplianceClassExpired();
			//$this->notificationForComplianceClassExpiredandRegister();
			$this->updateNotificationForComplianceClassExpired();
	
		} catch(Exception $ex){
			throw new Exception($ex->getMessage());
		}
	}
	
	
	private function notificationForComplianceClassExpired(){
		try{
			expDebug::dPrint(" notificationForComplianceClassExpired ",4);
	
			$notificationTemplateInfo = $this->getBulkNotificationTemplateInfo('compliance_expiry_remainder', 'cre_sys_lng_eng');
			expDebug::dPrint(" notificationTemplateInfo ".$notificationTemplateInfo ,5);
			$notifyFlag = ($notificationTemplateInfo['status']  == 'cre_ntn_sts_atv') ? 'Y' : 'N';
			$emailSendTo = explode(',',$notificationTemplateInfo['notification_sendto']);
			$ccEmail = $notificationTemplateInfo['notification_cc'];
			if($notifyFlag == 'Y'){
				foreach ($emailSendTo as $usertype){
					$select = db_select('slt_enrollment','enr');
					$select->innerJoin('slt_course_template','crs','crs.id=enr.course_id');
					$select->innerJoin('slt_course_class','cls','cls.id=enr.class_id and cls.status=\'lrn_cls_sts_atv\'');
					$select->innerJoin('slt_person','prs','prs.id = enr.user_id and prs.status = \'cre_usr_sts_atv\'');
					$select->innerJoin('users','usr','usr.name = prs.user_name');
				//	$select->leftJoin('slt_enrollment','e','e.course_id = enr.course_id and e.user_id = enr.user_id and e.reg_status not in (\'lrn_crs_reg_can\',\'lrn_crs_reg_rsc\') and e.comp_status in (\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_att\', \'lrn_crs_cmp_cmp\')
				//			and e.course_id = enr.course_id and e.user_id = enr.user_id and e.id != enr.id');
	
					$select->addExpression('\'compliance_expired\'', 'msg_id');
					$select->addExpression("if(find_in_set(prs.preferred_language,'".$notificationTemplateInfo['frmlng']."')>=0, prs.preferred_language,'cre_sys_lng_eng')", 'lang_code');
					$select->addExpression('\'Compliance Class Expired\'', 'msg_type');
	
					$notificationInfo =	"'first_name>|',ifnull(prs.first_name,''),'~|last_name>|',ifnull(prs.last_name,''),'~|deliver_type>|',ifnull(cls.delivery_type,''),
					if(cls.addn_notification_show = 0,'',if(cls.additional_info IS NOT NULL AND cls.addn_notification_show = 1,concat('~|add_notes>|',cls.additional_info,''),'~|add_notes>|-')),
					'~|course_title>|',ifnull(cls.title,''),'~|class_description>|',ifnull(cls.short_description,''),
					'~|full_name>|',ifnull(prs.full_name,''),'~|learning_fullname>|'
					,ifnull(prs.full_name,''),'~|user_name>|',ifnull(prs.user_name,''),'~|user_email>|',ifnull(prs.email,''),'~|user_phone>|',ifnull(prs.phone_no,''),
					'~|course_completed_date>|',DATE_FORMAT(ifnull(enr.comp_date,''),'%d %M %Y'),
					'~|tp_expirydate>|',if(crs.validity_date IS NULL,(DATE_FORMAT(DATE_ADD(ifnull(enr.comp_date,''), INTERVAL ifnull(crs.validity_days,'') DAY),'%d %M %Y')), (DATE_FORMAT(ifnull(crs.validity_date,''),'%d %M %Y')) ),'~|'";
	
					$select->addExpression("CONCAT(".$notificationInfo.")", "token_str");
						
					if($usertype == "cre_ntn_rpt_adm"){
						$select->addField('prs','id','user_id');
						$select->addExpression('null','send_to_id');
						$select->addExpression('null','send_to_name');
						$select->addExpression('null','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
						$select->addExpression('1', 'custom0');
					}
					if($usertype == "cre_ntn_rpt_usr"){
						$select->addField('prs','id','user_id');
						$select->addField('usr','uid','send_to_id');
						$select->addField('prs','full_name','send_to_name');
						$select->addField('usr','mail','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
					}
						
					if($usertype == "cre_ntn_rpt_mgr"){
						$select->leftJoin('slt_person','mgr','mgr.id = prs.manager_id');
						$select->leftJoin('users','mgr1','mgr1.name = mgr.user_name');
						$select->addField('mgr','id','user_id');
						$select->addField('mgr1','uid','send_to_id');
						$select->addField('mgr','full_name','send_to_name');
						$select->addField('mgr1','mail','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
					}
						
					$select->addExpression('\'php mailer\'', 'send_type');
					$select->addExpression('\'N\'', 'send_status');
					$select->addExpression('1', 'created_by');
					$select->addExpression('now()', 'created_on');
					$select->addExpression('1', 'updated_by');
					$select->addExpression('now()', 'updated_on');
	
	
					$select->condition('enr.is_compliance', 1,'=');
					$select->condition('crs.is_compliance',1,'=');
					$select->condition('enr.cmpl_expired',0,'=');
					$select->condition('enr.comp_status','lrn_crs_cmp_cmp','=');
					$select->condition(db_or()->condition('crs.validity_date','','IS NOT NULL')
							->condition('crs.validity_days','','IS NOT NULL'));
	
					$select->where("if(crs.validity_date IS NULL,
							(curdate() >= DATE_FORMAT(DATE_ADD(enr.comp_date, interval crs.validity_days+1 day),'%Y-%m-%d')),
							(date_format(DATE_ADD(crs.validity_date, interval 1 day),'%Y-%m-%d')<=curdate()))");
	
					$select->groupBy('enr.id');
					
					expDebug::dPrintDBAPI(' notificationForComplianceClassExpired SQL:  = ', $select);
					db_insert('slt_bulk_notification')
					->from($select)
					->execute();
				}
			}
		} catch(Exception $ex){
			throw new Exception($ex->getMessage());
		}
	}
	
	
	private function notificationForComplianceClassExpiredandRegister(){
		try{
			expDebug::dPrint(" notificationForComplianceClassExpiredandRegister ",4);
	
			$notificationTemplateInfo = $this->getBulkNotificationTemplateInfo('compliance_expiry_remainder', 'cre_sys_lng_eng');
			expDebug::dPrint(" notificationTemplateInfo ".$notificationTemplateInfo ,5);
			$notifyFlag = ($notificationTemplateInfo['status']  == 'cre_ntn_sts_atv') ? 'Y' : 'N';
			$emailSendTo = explode(',',$notificationTemplateInfo['notification_sendto']);
			$ccEmail = $notificationTemplateInfo['notification_cc'];
			if($notifyFlag == 'Y'){
				foreach ($emailSendTo as $usertype){
					$select = db_select('slt_enrollment','enr');
					$select->innerJoin('slt_course_template','crs','crs.id=enr.course_id');
					$select->innerJoin('slt_course_class','cls','cls.id=enr.class_id and cls.status=\'lrn_cls_sts_atv\'');
					//	$select->innerJoin('slt_notification_info','notify','notify.notification_code=\'compliance_expiry_remainder\'');
					$select->innerJoin('slt_person','prs','prs.id = enr.user_id and prs.status = \'cre_usr_sts_atv\'');
					$select->innerJoin('users','usr','usr.name = prs.user_name');
					/* $select->leftJoin('slt_enrollment','e','e.course_id = enr.course_id and e.reg_status not in (\'lrn_crs_reg_can\',\'lrn_crs_reg_rsc\') and e.comp_status in (\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_att\', \'lrn_crs_cmp_cmp\')
					 and e.course_id = enr.course_id and e.user_id = enr.user_id and e.id != enr.id'); */
					$select->leftJoin('slt_enrollment','e','e.course_id = enr.course_id and e.user_id = enr.user_id and (enr.is_compliance = 1) 
							 AND (enr.comp_status = \'lrn_crs_cmp_cmp\') AND (enr.cmpl_expired = 1) AND (e.cmpl_expired != 1) 
							 AND (e.reg_status = \'lrn_crs_reg_cnf\')
							 AND (e.comp_status IN (\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_att\'))
							 AND (if(crs.validity_date IS NULL,
							(curdate() >= DATE_FORMAT(DATE_ADD(enr.comp_date, interval crs.validity_days+1 day),\'%Y-%m-%d\')),
							(date_format(DATE_ADD(crs.validity_date, interval 1 day),\'%Y-%m-%d\') <= curdate()))) 
							AND( (crs.validity_date IS NOT NULL ) OR (crs.validity_days IS NOT NULL ) )');
	
					$select->addExpression('\'compliance_expired\'', 'msg_id');
					$select->addExpression("if(find_in_set(prs.preferred_language,'".$notificationTemplateInfo['frmlng']."')>=0, prs.preferred_language,'cre_sys_lng_eng')", 'lang_code');
					$select->addExpression('\'Compliance Class Expired\'', 'msg_type');
	
					$notificationInfo =	"'first_name>|',ifnull(prs.first_name,''),'~|last_name>|',ifnull(prs.last_name,''),'~|course_title>|',ifnull(cls.title,''),'~|class_description>|',ifnull(cls.short_description,''),
					'~|full_name>|',ifnull(prs.full_name,''),'~|learning_fullname>|'
					,ifnull(prs.full_name,''),'~|user_name>|',ifnull(prs.user_name,''),'~|user_email>|',ifnull(prs.email,''),'~|user_phone>|',ifnull(prs.phone_no,''),
					'~|course_completed_date>|',DATE_FORMAT(ifnull(enr.comp_date,''),'%d %M %Y'),'~|class_re_registered>|You have already re-registered for the same course.One of the class under the course is in enrolled status.~|',
					'tp_expirydate>|',if(crs.validity_date IS NULL,(DATE_FORMAT(DATE_ADD(ifnull(enr.comp_date,''), INTERVAL ifnull(crs.validity_days,'') DAY),'%d %M %Y')), (DATE_FORMAT(ifnull(crs.validity_date,''),'%d %M %Y')) ),'~|'";
	
					$select->addExpression("CONCAT(".$notificationInfo.")", "token_str");
						
					if($usertype == "cre_ntn_rpt_adm"){
						$select->addField('prs','id','user_id');
						$select->addExpression('null','send_to_id');
						$select->addExpression('null','send_to_name');
						$select->addExpression('null','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
						$select->addExpression('1', 'custom0');
					}
					if($usertype == "cre_ntn_rpt_usr"){
						$select->addField('prs','id','user_id');
						$select->addField('usr','uid','send_to_id');
						$select->addField('prs','full_name','send_to_name');
						$select->addField('usr','mail','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
					}
						
					if($usertype == "cre_ntn_rpt_mgr"){
						$select->leftJoin('slt_person','mgr','mgr.id = prs.manager_id');
						$select->leftJoin('users','mgr1','mgr1.name = mgr.user_name');
						$select->addField('mgr','id','user_id');
						$select->addField('mgr1','uid','send_to_id');
						$select->addField('mgr','full_name','send_to_name');
						$select->addField('mgr1','mail','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
					}
						
					$select->addExpression('\'php mailer\'', 'send_type');
					$select->addExpression('\'N\'', 'send_status');
					$select->addExpression('1', 'created_by');
					$select->addExpression('now()', 'created_on');
					$select->addExpression('1', 'updated_by');
					$select->addExpression('now()', 'updated_on');
	
	
					$select->condition('crs.is_compliance', 1,'=');
					$select->condition('enr.reg_status',array('lrn_crs_reg_can','lrn_crs_reg_rsc'),'NOT IN');
					$select->condition('enr.comp_status',array('lrn_crs_cmp_enr','lrn_crs_cmp_inp','lrn_crs_cmp_att', 'lrn_crs_cmp_cmp'),'IN');
					$select->condition('e.id','','IS NOT NULL');
					$select->groupBy('enr.course_id');
					$select->groupBy('enr.user_id');
					
					expDebug::dPrintDBAPI(' notificationForComplianceClassExpiredandRegister SQL:  = ', $select);
					db_insert('slt_bulk_notification')
					->from($select)
					->execute();
				}
			}
		} catch(Exception $ex){
			throw new Exception($ex->getMessage());
		}
	}
	
	
	private function updateNotificationForComplianceClassExpired(){
		try{
			$updSql = "update slt_enrollment e
			INNER JOIN (SELECT enr.user_id AS user_id, enr.id,
			enr.course_id, enr.class_id
			FROM
			slt_enrollment enr
			LEFT OUTER JOIN slt_course_template crs ON crs.id = enr.course_id 
			WHERE  ((crs.validity_days IS NOT NULL AND crs.validity_days != '') OR (crs.validity_date IS NOT NULL AND crs.validity_date != '') ) AND (enr.comp_status = 'lrn_crs_cmp_cmp') AND (enr.cmpl_expired != 1) AND (if(crs.validity_date IS NULL,
			(curdate() >= DATE_FORMAT(DATE_ADD(enr.comp_date, interval crs.validity_days+1 day),'%Y-%m-%d')),
			(date_format(DATE_ADD(crs.validity_date, interval 1 day),'%Y-%m-%d')<=curdate()))) AND( (crs.validity_date IS NOT NULL ) OR (crs.validity_days IS NOT NULL ) )
			GROUP BY enr.id) x on x.course_id = e.course_id and x.user_id = e.user_id
			SET e.comp_status = 'lrn_crs_cmp_exp',e.cmpl_expired = 1, e.updated_on = now(),e.updated_by = x.user_id
			where e.reg_status not in ('lrn_crs_reg_can','lrn_crs_reg_rsc') and e.comp_status in ('lrn_crs_cmp_enr','lrn_crs_cmp_inp','lrn_crs_cmp_att', 'lrn_crs_cmp_cmp')
			and e.course_id = x.course_id and e.user_id = x.user_id and e.id = x.id";
			
			expDebug::dPrintDBAPI("Update qry for compliance class expire ",$updSql);
			db_query($updSql);
	
		} catch(Exception $ex){
			throw new Exception($ex->getMessage());
		}
	}
	
	
	private function complianceExpiryBulkNotification(){
		try{
	
			expDebug::dPrint(" complianceExpiryBulkNotification ",4);
			$startTime = microtime(true);
			expDebug::dPrint("Start time of complianceExpiryBulkNotification".$startTime,4);
	
			$notificationTemplateInfo = $this->getBulkNotificationTemplateInfo('compliance_expiry_remainder', 'cre_sys_lng_eng');
			expDebug::dPrint(" notificationTemplateInfo ".$notificationTemplateInfo ,5);
			$notifyFlag = ($notificationTemplateInfo['status']  == 'cre_ntn_sts_atv') ? 'Y' : 'N';
			$emailSendTo = explode(',',$notificationTemplateInfo['notification_sendto']);
			$ccEmail = $notificationTemplateInfo['notification_cc'];
			if($notifyFlag == 'Y'){
				foreach ($emailSendTo as $usertype){
					$select = db_select('slt_enrollment','enr');
					$select->innerJoin('slt_course_template','crs','crs.id=enr.course_id');
					$select->innerJoin('slt_course_class','cls','cls.id=enr.class_id and cls.status=\'lrn_cls_sts_atv\'');
					$select->innerJoin('slt_notification_info','notify','notify.notification_code=\'compliance_expiry_remainder\'');
					$select->innerJoin('slt_person','prs','prs.id = enr.user_id and prs.status = \'cre_usr_sts_atv\'');
					$select->innerJoin('users','usr','usr.name = prs.user_name');
					$select->leftJoin('slt_entity_notification_mapping','map','map.notify_id = notify.id AND map.entity_id=crs.id');
	
					$select->addExpression('\'compliance_expiry_remainder\'', 'msg_id');
					$select->addExpression("if(find_in_set(prs.preferred_language,'".$notificationTemplateInfo['frmlng']."')>=0, prs.preferred_language,'cre_sys_lng_eng')", 'lang_code');
					$select->addExpression('\'Compliance Class Expiry Reminder\'', 'msg_type');
	
					$notificationInfo =	"'first_name>|',ifnull(prs.first_name,''),'~|last_name>|',ifnull(prs.last_name,''),'~|deliver_type>|',ifnull(cls.delivery_type,''),'~|course_title>|',ifnull(cls.title,''),
					'~|full_name>|',ifnull(prs.full_name,''),'~|learning_fullname>|'
					,ifnull(prs.full_name,''),'~|user_name>|',ifnull(prs.user_name,''),'~|user_email>|',ifnull(prs.email,''),'~|user_phone>|',ifnull(prs.phone_no,''),
			        if(cls.addn_notification_show = 0,'',if(cls.additional_info IS NOT NULL AND cls.addn_notification_show = 1,concat('~|add_notes>|',cls.additional_info,''),'~|add_notes>|-')),
					'~|course_completed_status>|~|course_completed_date>|~|',
					'expiry_date>|',if(crs.validity_date IS NULL,(DATE_FORMAT(DATE_ADD(ifnull(enr.comp_date,''), INTERVAL ifnull(crs.validity_days,'') DAY),'%d %M %Y')), (DATE_FORMAT(ifnull(crs.validity_date,''),'%d %M %Y')) ),'~|'";
	
					$select->addExpression("CONCAT(".$notificationInfo.")", "token_str");
						
					if($usertype == "cre_ntn_rpt_adm"){
						$select->addField('prs','id','user_id');
						$select->addExpression('null','send_to_id');
						$select->addExpression('null','send_to_name');
						$select->addExpression('null','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
						$select->addExpression('1', 'custom0');
					}
					if($usertype == "cre_ntn_rpt_usr"){
						$select->addField('prs','id','user_id');
						$select->addField('usr','uid','send_to_id');
						$select->addField('prs','full_name','send_to_name');
						$select->addField('usr','mail','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
					}
						
						
					if($usertype == "cre_ntn_rpt_mgr"){
						$select->leftJoin('slt_person','mgr','mgr.id = prs.manager_id');
						$select->leftJoin('users','mgr1','mgr1.name = mgr.user_name');
						$select->addField('mgr','id','user_id');
						$select->addField('mgr1','uid','send_to_id');
						$select->addField('mgr','full_name','send_to_name');
						$select->addField('mgr1','mail','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
					}
						
					$select->addExpression('\'php mailer\'', 'send_type');
					$select->addExpression('\'N\'', 'send_status');
					$select->addExpression('1', 'created_by');
					$select->addExpression('now()', 'created_on');
					$select->addExpression('1', 'updated_by');
					$select->addExpression('now()', 'updated_on');
	
	
					$select->condition('enr.is_compliance', 1,'=');
					$select->condition('enr.cmpl_expired','1','!=');
					$select->condition('enr.comp_status','lrn_crs_cmp_cmp','=');
					$select->condition(db_or()->condition('crs.validity_date','','IS NOT NULL')
							->condition('crs.validity_days','','IS NOT NULL'));
					$select->where("if(crs.validity_days IS NOT NULL,(curdate() >= DATE_FORMAT(DATE_SUB(DATE_ADD(enr.comp_date, interval crs.validity_days day),
							interval notify.notify_before day),'%Y-%m-%d')),(curdate() >= DATE_FORMAT(DATE_SUB(crs.validity_date, interval notify.notify_before day),'%Y-%m-%d')))");
	
					$select->where("if(crs.validity_date IS NULL,
							(curdate() != DATE_FORMAT(DATE_ADD(enr.comp_date, interval crs.validity_days+1 day),'%Y-%m-%d')),
							(date_format(DATE_ADD(crs.validity_date, interval 1 day),'%Y-%m-%d')!=curdate()))");
	
					$select->where("MOD(DATEDIFF(curdate(),if(crs.validity_days IS NOT NULL,(DATE_SUB(DATE_ADD(enr.comp_date, interval crs.validity_days+1 day), interval notify.notify_before day)),
							(DATE_SUB(crs.validity_date, interval notify.notify_before-1 day)))),notify.frequency_data_value) = 0");
					
					expDebug::dPrintDBAPI(' complianceExpiryBulkNotification SQL:  = ', $select);
					db_insert('slt_bulk_notification')
					->from($select)
					->execute();
				}
			}
			$totTime = $this->processedTime($startTime);
			expDebug::dPrint("end time of complianceExpiryBulkNotification".gmdate('H:i:s',$totTime),4);
		} catch(Exception $ex){
			throw new Exception($ex->getMessage());
		}
	}
	
	
	private function autocomplianceIncompleteDetails(){
		try{
			$compQry = db_select('slt_enrollment','enr');
			$compQry->innerJoin('slt_course_template','crs','enr.course_id = crs.id');
			$compQry->innerJoin('slt_person','per','per.id = enr.user_id');
			$compQry->addField('enr', 'class_id');
			$compQry->addField('enr', 'course_id');
			$compQry->addField('enr', 'class_id');
			$compQry->addField('enr', 'is_compliance');
			//$compQry->condition('enr.is_compliance',1,'=');
			//$compQry->condition(db_or()->condition('crs.compliance_completed','hire_days','=')
			//		->condition('enr.is_compliance',1,'='));
			$compQry->condition('enr.comp_status',array('lrn_crs_cmp_enr','lrn_crs_cmp_inp'),'IN');
			$compQry->where('if(( ifnull(crs.complete_days,\'\') != \'\') or ( ifnull(crs.complete_date,\'\') != \'\'),
					(CURDATE() >= (CASE WHEN (crs.complete_days IS NOT NULL AND crs.complete_days != \'\')
					THEN
					(date_format(date_add(if((crs.compliance_completed = \'hire_days\'), IF(per.hire_date is not null, per.hire_date, 1=2), enr.reg_date),interval crs.complete_days+1 day),\'%Y-%m-%d\'))
					WHEN (crs.complete_date IS NOT NULL AND crs.complete_date != \'\')
					THEN
					(date_format(date_add(crs.complete_date,interval 1 day),\'%Y-%m-%d\'))
					END)),1=0)');
		
			$compQry->groupBy('enr.course_id');
			$compQry->groupBy('enr.class_id');
			expDebug::dPrintDBAPI("query to get the grouped compliance enrollments by course and class id",$compQry);
			$compRes = $compQry->execute()->fetchAll();
		return $compRes;
		}catch(Exception $ex){
			throw new Exception($ex->getMessage());
		}
	}
	
	private function RemindsessionBulkbatchMonitor()
	{
		try{
			//Virtual class remind session
			try{
				$this->classSessionRemainderCronJob('lrn_cls_dty_vcl');
			}catch(Exception $ex){
				expDebug::dPrint("Error in CRON - Virtual class session remainder - ".print_r($ex,true),1);
			}
			
			//ILT class remind session
			try{
				$this->classSessionRemainderCronJob('lrn_cls_dty_ilt');
			}catch(Exception $ex){
				expDebug::dPrint("Error in CRON - ILT class session remainder - ".print_r($ex,true),1);
			}
		}catch (Exception $ex) {
			throw new Exception($ex->getMessage());
		}
	}
	
	private function classSessionRemainderCronJob($deliveryType){
		try{
			$reminderType = ($deliveryType == 'lrn_cls_dty_vcl') ? 'virtual_class_reminder' : 'class_remind_session';
			$select = db_select('slt_enrollment', 'a');
			$select->innerJoin('slt_course_class_session', 'b', 'b.course_id=a.course_id AND b.class_id=a.class_id');
			$select->innerJoin('slt_course_class', 'c', 'b.class_id=c.id');
			$select->innerJoin('slt_notification_info', 'e', 'e.lang_code = \'cre_sys_lng_eng\' AND e.notification_code=\''.$reminderType.'\'');
			$select->addField('a', 'user_id');
			$select->addField('a', 'course_id');
			$select->addField('a', 'class_id');
	
			$select->condition('a.reg_status', 'lrn_crs_reg_cnf');
			$select->condition('a.comp_status', 'lrn_crs_cmp_enr');
			$select->condition('c.status', 'lrn_cls_sts_atv');
			$select->condition('c.delivery_type', $deliveryType);
			$select->where('e.frequency_data_value = DATEDIFF(DATE_FORMAT(b.start_date,\'%Y-%m-%d\'),
					IFNULL(CONVERT_TZ(now(), \''.$defaultTimezone.'\' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= b.timezone)),NOW()))');
			$select->groupBy('c.id');
	
			expDebug::dPrintDBAPI('classRemainderSessionCronJob query ' , $select);
			$result =  $select->execute()->fetchAll();
			expDebug::dPrint('classRemainderSessionCronJob  '. print_r($result,true). "delivery type".$deliveryType,3);
			foreach($result as $resVal){
				$this->classRemindSessionBulkNotification($deliveryType,$reminderType,$resVal->class_id,$resVal->course_id);
			}
	
		} catch(Exception $ex){
			throw new Exception($ex->getMessage());
		}
	
	}
	
	private function classRemindSessionBulkNotification($delType,$reminderType,$classId,$courseId){
		try{
			expDebug::dPrint(" classRemindSessionBulkNotification ",4);
			$startTime = microtime(true);
			expDebug::dPrint("Start time of classRemindSessionBulkNotification".$startTime,4);
			$notificationTemplateInfo = $this->getBulkNotificationTemplateInfo($reminderType, 'cre_sys_lng_eng');
			expDebug::dPrint(" notificationTemplateInfo ".$notificationTemplateInfo ,5);
			$notifyFlag = ($notificationTemplateInfo['status']  == 'cre_ntn_sts_atv') ? 'Y' : 'N';
			$emailSendTo = explode(',',$notificationTemplateInfo['notification_sendto']);
			$ccEmail = $notificationTemplateInfo['notification_cc'];
			$notificationInfo =	"'first_name>|',ifnull(prs.first_name,''),
			'~|last_name>|',ifnull(prs.last_name,''),
			'~|class_title>|',ifnull(c.title,''),
			'~|class_description>|',ifnull(c.short_description,''),
			'~|course_title>|',ifnull(d.title,''),
			'~|course_code>|',ifnull(c.code,''),
			'~|full_name>|',ifnull(prs.full_name,''),
			'~|learning_fullname>|',ifnull(prs.full_name,''),
			'~|user_name>|',ifnull(prs.user_name,''),
			'~|user_email>|',ifnull(prs.email,''),
			if(c.addn_notification_show = 0,'',if(c.additional_info IS NOT NULL AND c.addn_notification_show = 1,concat('~|add_notes>|',c.additional_info,''),'~|add_notes>|-'))";

	
	        $classInfo = getClassInfoForNotifications($classId);
			$classadditionalinfo = convertNullValToNullStr($classInfo['additional_info']);
            $classissnoti = convertNullValToNullStr($classInfo['addn_notification_show']);
	        $classSessionInfo = getClassMultiSessionInfoForNotifications($courseId, $classId, $delType, '', true);
			$classInstructorInfo  = getClassInstructorInfoForNotifications($classId);
			$classSessionInformation = $classSessionInfo[0];
			if ($classSessionInformation == null) {
				$classSessionInformation = 'session_name<| ~~session_startdate<| ~~session_enddate<| ~~session_starttime<| ~~session_endtime<| ~~time_zone<| ~@';
			}
			$sessionLocation = $classSessionInfo[1];
						if ($sessionLocation == null) {
							$sessionLocation = '';
						} else { 
							$LocationDel = ClassLocationDetails($classId,$sessionLocation);
							if(count($LocationDel) < 0)  $LocationDel = $sessionLocation;
						}
	
			$notificationInfo = $notificationInfo .
			",'~|session_location>|','". $LocationDel ."','~|group_sessiondata>|','".$classSessionInformation ."','~|group_instructordata>|','". $classInstructorInfo."~|'";
			if($notifyFlag == 'Y'){
				foreach ($emailSendTo as $usertype){
					$msgType = ($delType == 'lrn_cls_dty_vcl') ? 'Virtual Class Reminder' : 'Class Reminder';
					$select = db_select('slt_enrollment', 'a');
					$select->innerJoin('slt_course_class_session', 'b', 'b.course_id=a.course_id AND b.class_id=a.class_id');
					$select->innerJoin('slt_course_class', 'c', 'b.class_id=c.id');
					$select->innerJoin('slt_course_template', 'd', 'd.id=b.course_id');
					$select->innerJoin("slt_person",'prs','prs.id = a.user_id and prs.status = \'cre_usr_sts_atv\'');
					$select->innerJoin('users','usr','usr.name = prs.user_name');
					$select->leftJoin("slt_profile_list_items", "pro","pro.code = prs.time_zone and pro.is_active = 'Y' and prs.time_zone is not null");
	
					$select->addExpression("'".$reminderType."'", 'msg_id');
					$select->addExpression("if(find_in_set(prs.preferred_language,'".$notificationTemplateInfo['frmlng']."')>=0, prs.preferred_language,'cre_sys_lng_eng')", 'lang_code');
					$select->addExpression("'".$msgType."'", 'msg_type');
					$select->addExpression("concat(".$notificationInfo.")", "token_str");
						
					if($usertype == "cre_ntn_rpt_adm"){
						$select->addField('prs','id','user_id');
						$select->addExpression('null','send_to_id');
						$select->addExpression('null','send_to_name');
						$select->addExpression('null','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
						$select->addExpression('1', 'custom0');
					}
					if($usertype == "cre_ntn_rpt_usr"){
						$select->addField('prs','id','user_id');
						$select->addField('usr','uid','send_to_id');
						$select->addField('prs','full_name','send_to_name');
						$select->addField('usr','mail','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
					}
						
					if($usertype == "cre_ntn_rpt_mgr"){
						$select->leftJoin('slt_person','mgr','mgr.id = prs.manager_id');
						$select->leftJoin('users','mgr1','mgr1.name = mgr.user_name');
						$select->addField('mgr','id','user_id');
						$select->addField('mgr1','uid','send_to_id');
						$select->addField('mgr','full_name','send_to_name');
						$select->addField('mgr1','mail','send_to_email');
						$select->addExpression('null', 'send_to_email_cc');
					}
						
					$select->addExpression('\'php mailer\'', 'send_type');
					$select->addExpression('\'N\'', 'send_status');
					$select->addExpression('1', 'created_by');
					$select->addExpression('now()', 'created_on');
					$select->addExpression('1', 'updated_by');
					$select->addExpression('now()', 'updated_on');
	
					$select->condition('a.reg_status', 'lrn_crs_reg_cnf');
					$select->condition('a.comp_status', 'lrn_crs_cmp_enr');
					$select->condition('c.status', 'lrn_cls_sts_atv');
					$select->condition('c.delivery_type', $delType);
					$select->condition('c.id', $classId,'=');
	
					$select->where($notificationTemplateInfo['frequency_data_value'] .' = DATEDIFF(DATE_FORMAT(b.start_date,\'%Y-%m-%d\'),
							IFNULL(CONVERT_TZ(now(), \''.$defaultTimezone.'\' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= b.timezone)),NOW()))');
					$select->groupBy('a.id');
					expDebug::dPrintDBAPI('Query for classRemindSessionBulkNotification ' , $select);
					db_insert('slt_bulk_notification')
					->from($select)
					->execute();
	
				}
			}
			$totTime = $this->processedTime($startTime);
			expDebug::dPrint("end time of classRemindSessionBulkNotification".gmdate('H:i:s',$totTime),4);
		} catch(Exception $ex){
			throw new Exception($ex->getMessage());
		}
	}
	
	private function remove_expired_oAuth_access_tokens(){
		try{
			$access_token_time = date('Y-m-d H:i:s', time());
			$deleteStmt = db_delete('slt_oauth_access_tokens');
			$deleteStmt->condition('expires', $access_token_time,'<');
			expDebug::dPrintDBAPI(' Delete Token = ' ,$deleteStmt);
			$deleteStmt->execute();
		}catch(Exception $ex){
			throw new Exception($ex->getMessage());
		}
	}
	
	/* TP certificate Expiry and TP Learning plan Incomplete */
	private function LPExpiredBulkbatchMonitor(){
	
		try{
			$notificationTemplateInfo = $this->getBulkNotificationTemplateInfo('certification_expire', 'cre_sys_lng_eng');
		
			$notifyFlag = ($notificationTemplateInfo['status']  == 'cre_ntn_sts_atv') ? 'Y' : 'N';
			$emailSendTo = explode(',',$notificationTemplateInfo['notification_sendto']);
			$ccEmail = $notificationTemplateInfo['notification_cc'];
			
			$randId = substr((time().mt_rand(1,100)), -9);
			expDebug::dPrint("Random no for dataload by column to revert if any error occured".$randId."value for before sub strin".$vl,4);
			
			if($notifyFlag == 'Y'){
				foreach ($emailSendTo as $value){
					$this->insertBulkNotificationScript($value,$randId);
				}
			}
		}catch(Exception $e){
			throw new Exception($e->getMessage());
		}
		
		// TODO: Have a track of the previous status and whichever the records get updated to do rollback
		try{
			$updqry  = "UPDATE slt_master_enrollment mse
			INNER JOIN slt_program prgm ON prgm.id = mse.program_id
			SET mse.overall_status = 'lrn_tpm_ovr_exp', 
			mse.updated_on = NOW(),
			mse.custom_dataload = $randId
			where
			(prgm.object_type = 'cre_sys_obt_crt') AND (mse.overall_status = 'lrn_tpm_ovr_cmp') AND (mse.is_current = 'Y') AND (mse.comp_date IS NOT NULL )
			AND (prgm.expires_in_value IS NOT NULL ) AND (prgm.expires_in_unit IS NOT NULL ) AND (prgm.expires_in_value > 0)
			AND (DATEDIFF(
			IF (prgm.expires_in_unit = 'days', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value DAY),
			IF (prgm.expires_in_unit = 'months', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value MONTH),
			DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value YEAR))),
			CURDATE()) < 0) ";
			
			expDebug::dPrintDBAPI('update master enrolment :: ', $updqry);
			db_query($updqry)->execute();
		}catch(Exception $ex){
			
			$menrUpd = db_update('slt_master_enrollment');
			$menrUpd->fields(array('overall_status' => 'lrn_tpm_ovr_cmp','updated_on'=>now()));
			$menrUpd->condition('custom_dataload',$randId);
			$menrUpd->execute();
			
			$query = db_delete('slt_bulk_notification');
			$query->condition('dataload_by',$randId);
			expDebug::dPrintDBAPI('Delete Bulk notification',$query);
			$query->execute();
		
			//TODO: remove all the inserted notification in above process.
			throw new Exception($ex->getMessage());
		}
		
		try{
			$updqry1 = "UPDATE slt_skill_set a
			INNER JOIN slt_master_enrollment mse on mse.id = a.master_enrollment_id
			INNER JOIN slt_program prgm ON prgm.id = mse.program_id
			SET a.is_deleted  = 1
			where
			(prgm.object_type = 'cre_sys_obt_crt') AND (mse.overall_status = 'lrn_tpm_ovr_exp') AND (mse.is_current = 'Y') AND (mse.comp_date IS NOT NULL )
			AND (prgm.expires_in_value IS NOT NULL ) AND (prgm.expires_in_unit IS NOT NULL ) AND (prgm.expires_in_value > 0)
			AND (DATEDIFF(
			IF (prgm.expires_in_unit = 'days', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value DAY),
			IF (prgm.expires_in_unit = 'months', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value MONTH),
			DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value YEAR))),
			CURDATE()) < 0) ";
			
			expDebug::dPrintDBAPI('Update skill :: ', $updqry1);
			db_query($updqry1);
		}catch(Exception $ex){
			//TODO: Rollback the previous action.
			
			$sklQry = "UPDATE slt_skill_set a INNER JOIN slt_master_enrollment mse on mse.id = a.master_enrollment_id
								 SET a.is_deleted = null WHERE mse.custom_dataload = '$randId'";
			db_query($sklQry);
			
			$query = db_delete('slt_bulk_notification');
			$query->condition('dataload_by',$randId);
			expDebug::dPrintDBAPI('Delete Bulk notification',$query);
			$query->execute();
			
			$menrUpd = db_update('slt_master_enrollment');
			$menrUpd->fields(array('overall_status' => 'lrn_tpm_ovr_cmp'));
			$menrUpd->condition('custom_dataload',$randId);
			$menrUpd->execute();
			
			throw new Exception($ex->getMessage());
		}
		
		$trnRandId = substr((time().mt_rand(1,100)), -9);
		expDebug::dPrint("Random no for dataload by column to revert if any error occured".$randId,4);
		try{
			$query = "UPDATE slt_enrollment a
			INNER JOIN slt_master_enrollment mse on mse.id = a.master_enrollment_id
			INNER JOIN slt_program prgm ON prgm.id = mse.program_id
			set a.comp_status = 'lrn_crs_cmp_inc',
			a.updated_on = now(),
			a.updated_by = 0,
			a.custom_dataload = $trnRandId,
			a.custom4 = a.comp_status
			where prgm.object_type = 'cre_sys_obt_trn' AND prgm.end_date IS NOT NULL
			AND (IF(prgm.end_date IS NOT NULL, DATEDIFF(CURDATE(), prgm.end_date) > 0,1=1))
			AND (if(a.comp_status IS NOT NULL, a.comp_status IN ('lrn_crs_cmp_enr','lrn_crs_cmp_inp'),a.reg_status IN('lrn_crs_reg_ppm','lrn_crs_reg_ppv','lrn_crs_reg_wtl')))";
		
			expDebug::dPrintDBAPI("Update slt_enrollment table :: ",$query);
			db_query($query);
		}catch(Exception $ex){
			//TODO: Rollback the previous action.
			
			$enrUpd = db_update('slt_enrollment');
			$enrUpd->expression('overall_status','custom4');
			$enrUpd->condition('custom_dataload',$trnRandId);
			$enrUpd->execute();
			
			throw new Exception($ex->getMessage());
		}
	
		try{
			$qry = "UPDATE slt_master_enrollment mse
			INNER JOIN slt_program prgm ON prgm.id = mse.program_id
			SET mse.overall_status  = 'lrn_tpm_ovr_inc',
			mse.updated_on = now(),
			mse.comp_date = now(),
			mse.updated_by = 0,
			mse.custom_dataload = $trnRandId,
			mse.custom4 = mse.overall_status
			WHERE prgm.object_type = 'cre_sys_obt_trn' AND prgm.end_date IS NOT NULL
			AND (IF(prgm.end_date IS NOT NULL, DATEDIFF(CURDATE(), prgm.end_date) > 0,1=1))
			AND mse.overall_status IN ('lrn_tpm_ovr_enr','lrn_tpm_ovr_inp','lrn_tpm_ovr_ppm','lrn_tpm_ovr_ppv')";
		
			expDebug::dPrintDBAPI("Update slt_master_enrollment table :: ",$qry);
			db_query($qry);
		}catch(Exception $ex){
			//TODO: Rollback the previous action.
			
			$enrUpd = db_update('slt_enrollment');
			$enrUpd->expression('comp_status','custom4');
			$enrUpd->condition('custom_dataload',$trnRandId);
			$enrUpd->execute();
			
			$msenrUpd = db_update('slt_master_enrollment');
			$msenrUpd->expression('overall_status','custom4');
			$msenrUpd->condition('custom_dataload',$trnRandId);
			expDebug::dPrintDBAPI("Query for master enrollment update",$msenrUpd);
			$msenrUpd->execute();
			
			throw new Exception($ex->getMessage());
		} 
		
		try{
			
			$menrUpd = db_update('slt_master_enrollment');
			$menrUpd->fields(array('custom_dataload' => null));
			$menrUpd->condition('custom_dataload',$randId);
			expDebug::dPrintDBAPI("Query for master enrollment update with Expiry",$menrUpd);
			$menrUpd->execute();
			
			$enrUpd = db_update('slt_enrollment');
			$enrUpd->fields(array('custom4' => null,'custom_dataload' => null));
			$enrUpd->condition('custom_dataload',$trnRandId);
			$enrUpd->execute();
				
			$msenrUpd = db_update('slt_master_enrollment');
			$msenrUpd->fields(array('custom4'=>null,'custom_dataload' => null));
			$msenrUpd->condition('custom_dataload',$trnRandId);
			expDebug::dPrintDBAPI("Query for master enrollment update with incomplete",$msenrUpd);
			$msenrUpd->execute();
			
		}catch(Exception $e){
			throw new Exception($e->getMessage());
		}
	}
	
	private function insertBulkNotificationScript($emailSendTo,$randId){
		try{
			$select = db_select('slt_master_enrollment','mse');
			$select->innerJoin('slt_program','prgm','prgm.id = mse.program_id');
			$select->innerJoin('slt_profile_list_items','pfli','pfli.code =  prgm.object_type');
			$select->innerJoin('slt_person','per','per.id = mse.user_id and per.status = \'cre_usr_sts_atv\'');
			$select->innerJoin('users','usr','usr.name = per.user_name');
			$select->addExpression('\'certification_expire\'', 'msg_id');
			//$select->addExpression('\'cre_sys_lng_eng\'', 'lang_code');
			$select->addExpression("if(find_in_set(per.preferred_language,'".$notificationTemplateInfo['frmlng']."')>=0, per.preferred_language,'cre_sys_lng_eng')", 'lang_code');
			$select->addExpression('\'Certification Expiry\'', 'msg_type');
			$select->addExpression("concat('delivery_type>|',ifnull(pfli.name,''),'~|course_title>|',ifnull(prgm.title,''),'~|tp_title>|',ifnull(prgm.title,''),
					'~|tp_expirydate>|',
					IF (prgm.expires_in_unit = 'days', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value DAY),
					IF (prgm.expires_in_unit = 'months', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value MONTH),
					DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value YEAR))),'~|course_completed_date>|',mse.comp_date,
					if(prgm.addn_notification_show = 0,'',if(prgm.additional_info IS NOT NULL AND prgm.addn_notification_show = 1,concat('~|add_notes>|',prgm.additional_info,''),'~|add_notes>|-')),
				    '~|program_id>|',ifnull(prgm.id,''),'~|user_name>|',ifnull(per.user_name,''),'~|first_name>|',ifnull(per.first_name,''),
					'~|last_name>|',ifnull(per.last_name,''),'~|user_email>|',ifnull(per.email,''),'~|user_phone>|',ifnull(per.phone_no,''),'~|learning_fullname>|',
					ifnull(per.full_name,''),'~|full_name>|',ifnull(per.full_name,''))", "token_str");
		
			if($emailSendTo == "cre_ntn_rpt_adm"){
				$select->addField('per','id','user_id');
				$select->addExpression('null','send_to_id');
				$select->addExpression('null','send_to_name');
				$select->addExpression('null','send_to_email');
				$select->addExpression('null', 'send_to_email_cc');
				$select->addExpression('1', 'custom0');
			}
			if($emailSendTo == "cre_ntn_rpt_usr"){
				$select->addField('per','id','user_id');
				$select->addField('usr','uid','send_to_id');
				$select->addField('per','full_name','send_to_name');
				$select->addField('usr','mail','send_to_email');
				$select->addExpression('null', 'send_to_email_cc');
			}
		
		
			if($emailSendTo == "cre_ntn_rpt_mgr"){
				$select->leftJoin('slt_person','mgr','mgr.id = per.manager_id');
				$select->leftJoin('users','mgr1','mgr1.name = mgr.user_name');
				$select->addField('mgr','id','user_id');
				$select->addField('mgr1','uid','send_to_id');
				$select->addField('mgr','full_name','send_to_name');
				$select->addField('mgr1','mail','send_to_email');
				$select->addExpression('null', 'send_to_email_cc');
			}
		
			$select->addExpression('\'php mailer\'', 'send_type');
			$select->addExpression('\'N\'', 'send_status');
			$select->addExpression('1', 'created_by');
			$select->addExpression('now()', 'created_on');
			$select->addExpression('1', 'updated_by');
			$select->addExpression('now()', 'updated_on');
			$select->addExpression("'".$randId."'", 'dataload_by');
		
			$select->condition('prgm.object_type', 'cre_sys_obt_crt', '=');
			$select->condition('mse.overall_status', 'lrn_tpm_ovr_cmp', '=');
			$select->condition('mse.is_current','Y','=');
			$select->isNotNull('mse.comp_date');
			$select->isNotNull('prgm.expires_in_value');
			$select->isNotNull('prgm.expires_in_unit');
			$select->condition('prgm.expires_in_value', 0, '>');
			$select->where("DATEDIFF(
					IF (prgm.expires_in_unit = 'days', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value DAY),
					IF (prgm.expires_in_unit = 'months', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value MONTH),
					DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value YEAR))),
					CURDATE()) < 0");
		
			expDebug::dPrintDBAPI('change_lp_expired_status :: ', $select);
		
		
			db_insert('slt_bulk_notification')
			->from($select)
			->execute();
		}catch(Exception $ex){
			throw new Exception($ex->getMessage());
		}
	}
	
	private function processedTime($time_start){
		$time = 0;
		$time_end = microtime(true);
		$time = $time_end - $time_start;
		return $time;
	}
	
	/*
	 * getBulkNotificationTemplateInfo() - Returns the details of a notification template from slt_notification_info table
	*/
	private function getBulkNotificationTemplateInfo($notificationCode, $notificationLanguageCode) {
		try{
			// Select table
			$selectStmt = db_select('slt_notification_info', 'ninfo');
			$selectStmt->innerJoin('slt_notification_frame','frm','frm.notification_id = ninfo.id');
			// Set conditions
			$selectStmt->condition('ninfo.notification_code', $notificationCode);
			$selectStmt->condition('ninfo.lang_code', $notificationLanguageCode);
			// Select fields and/or add expressions
			$selectStmt->addField('ninfo', 'id', 'id');
			$selectStmt->addField('ninfo', 'notification_code', 'notification_code');
			$selectStmt->addField('ninfo', 'lang_code', 'lang_code');
			$selectStmt->addField('ninfo', 'locale_code', 'locale_code');
			$selectStmt->addField('ninfo', 'profile_code', 'profile_code');
			$selectStmt->addField('ninfo', 'notification_sendto', 'notification_sendto');
			$selectStmt->addField('ninfo', 'entity_type', 'entity_type');
			$selectStmt->addField('ninfo', 'notification_cc', 'notification_cc');
			$selectStmt->addField('ninfo', 'notification_admin', 'notification_admin');
			$selectStmt->addField('ninfo', 'notification_title', 'notification_title');
			$selectStmt->addField('ninfo', 'notification_description', 'notification_description');
			$selectStmt->addField('ninfo', 'notification_type', 'notification_type');
			$selectStmt->addField('ninfo', 'notify_before', 'notify_before');
			$selectStmt->addField('ninfo', 'procedure_name', 'procedure_name');
			$selectStmt->addField('ninfo', 'frequency_data_type', 'frequency_data_type');
			$selectStmt->addField('ninfo', 'frequency_data_value', 'frequency_data_value');
			$selectStmt->addField('ninfo', 'status', 'status');
			//$selectStmt->addExpression("GROUP_CONCAT('\"',frm.lang_code,'\"')",'frmlng');
			$selectStmt->addExpression("GROUP_CONCAT(frm.lang_code)",'frmlng');
			$selectStmt->groupBy('ninfo.id');
			expDebug::dPrintDBAPI('Query for getBulkNotificationTemplateInfo ' , $selectStmt);
			// Execute query
			$result = $selectStmt->execute();
			if ($result->rowCount() <= 0) {
				return null;
			}
			$result = $result->fetchAssoc();
			expDebug::dPrint('$result = ' . print_r($result, true) , 3);
			return $result;
		}
		catch (Exception $ex) {
			throw new Exception($ex->getMessage());
		}
	}
	private function CertVerificationReminerBulkinsertNotification(){
		try{
			expDebug::dPrint("Inside Expiry notification",4);
			$startTime = microtime(true);
			expDebug::dPrint("Start time of CertVerificationReminerBulkinsertNotification".$startTime,4);
			$notificationTemplateInfo = $this->getBulkNotificationTemplateInfo('verification_reminder', 'cre_sys_lng_eng');
			expDebug::dPrint(" notificationTemplateInfo ".$notificationTemplateInfo ,5);
			$notifyFlag = ($notificationTemplateInfo['status']  == 'cre_ntn_sts_atv') ? 'Y' : 'N';
			$emailSendTo = explode(',',$notificationTemplateInfo['notification_sendto']);
			$ccEmail = $notificationTemplateInfo['notification_cc'];
			
		
			
			
			
			
			//$action_detail =  'objID1>|'.$objectId1.'~|objID2>|~|objID3>|~|objID4>|~|objID5>|';
			if($notifyFlag == 'Y'){
				$config           = getConfig("exp_sp");
				$configSiteUrl    = $config["admin_site_url"];
				$verify = "Verified";
				$reject = "Rejected";
				$notificationInfo =	"'first_name>|',ifnull(per.first_name,''),
					'~|verifier>|',ifnull(prs.user_name,''),
					'~|certificate>|',ifnull(skill.skill_name,''),
					'~|company>|',ifnull(skill.company,''),
					'~|certificate_number>|',ifnull(skill.certificate_number,''),
					'~|validity>|DATEDIFF(DATE_ADD(skill.created_on, INTERVAL $cert_expiry_reminder DAY),NOW())~|'";
				//foreach ($emailSendTo as $usertype){
					expDebug::dPrint(" email send to ".$usertype ,5);
					$config           = getConfig("exp_sp");
					$cert_expiry_reminder = isset($config["Certificate_verification_expiry"]) ? $config["Certificate_verification_expiry"] : 10;
					$select1 = db_select('slt_skill_set','skill');
					$select1->innerJoin('slt_person','per','per.id = skill.user_id');
					$select1->innerJoin('slt_person','prs','prs.user_name =skill.user_name');
					$select1->innerJoin('users','usr','usr.name = per.user_name');
					$select1->addField('skill','id','skill_id');
					$select1->addField('skill','certificate_type','certificate_type');
					$select1->addField('skill','certificate_img','certificate_img');
					$select1->addField('skill','valid_from','valid_from');
					$select1->addField('skill','valid_to','valid_to');
					$select1->addExpression('\'verification_reminder\'', 'msg_id');
					$select1->addExpression("if(find_in_set(prs.preferred_language,'".$notificationTemplateInfo['frmlng']."')>=0, prs.preferred_language,'cre_sys_lng_eng')", 'lang_code');
					$select1->addExpression('\'verification expiry reminder\'', 'msg_type');
					$select1->addExpression("concat(".$notificationInfo.")", "token_str");
					//$select1->addExpression("DATEDIFF(DATE_ADD(skill.created_on, INTERVAL $cert_expiry_reminder DAY","Validity");
					$select1->addExpression("DATEDIFF( DATE_ADD(skill.created_on, INTERVAL $cert_expiry_reminder DAY),NOW()) ","Validity");
					//$select->addExpression("concat('objID1>|',prgm.id,'~|objID2>|~|objID3>|~|objID4>|~|objID5>|')","detail_value");
	
					
					
	
	
					//if($usertype == "cre_ntn_rpt_mgr"){
						
						$select1->addField('prs','id','user_id');
						$select1->addField('usr','uid','send_to_id');
						$select1->addField('prs','full_name','send_to_name');
						$select1->addField('prs','email','send_to_email');
						$select1->addExpression('null', 'send_to_email_cc');
				//	}
	
					$select1->addExpression('\'php mailer\'', 'send_type');
					$select1->addExpression('\'N\'', 'send_status');
					$select1->addExpression('1', 'created_by');
					$select1->addExpression('now()', 'created_on');
					$select1->addExpression('1', 'updated_by');
					$select1->addExpression('now()', 'updated_on');
					//$select->addExpression('1', 'mobile');
	
					$select1->condition('skill.verification_status', 'Verification pending', '=');
					$select1->condition('skill.type', 'certificate', '=');
	
					$select1->where("(DATEDIFF(DATE_ADD(skill.created_on, INTERVAL $cert_expiry_reminder DAY),NOW())) = 3");
					
					expDebug::dPrintDBAPI('$select CertVerificationReminerBulkinsertNotification', $select1);
					$result = $select1->execute()->fetchAll();
					
					expDebug::dPrintDBAPI('$select CertVerificationReminerBulkinsertNotification aaaaa', $select);
					
					foreach($result as $certresults){
						
						$certId = md5($certresults->skill_id);
						$insertstmt = db_insert("slt_external_certificate");
						$fields = array(
								'encrypted_id' => $certId,
								'certificate_id' => $certresults->skill_id
						);
						$insertstmt->fields($fields);
						expDebug::dPrintDBAPI(' Fetch users list query 2 =sasas ' , $insertStmt );
						
						$config           = getConfig("exp_sp");
						$configSiteUrl    = $config["admin_site_url"];
						$verify = "Verified";
						$reject = "Rejected";
						
					
						
						$validityfrom =date(" jS F Y",strtotime($certresults->valid_from));
						$validityto = date(" jS F Y",strtotime($certresults->valid_to));
						
						$validitydates = $validityfrom.' to '.$validityto;
						$validity = '<strong>Validity:</strong> '.$validitydates;
						if($certresults->valid_from == "0000-00-00"){
							$validity = '';
						}elseif($certresults->valid_to == "0000-00-00"){
							$validity = '<strong>Validity:</strong> from'.$validityfrom;
						}
						
						$config           = getConfig("exp_sp");
						$cert_expiry_reminder = isset($config["Certificate_verification_expiry"]) ? $config["Certificate_verification_expiry"] : 10;
						$fullnameofuser = $certresults->send_to_name;
						$subject = t('MSG1116').' '.$fullnameofuser;
						$VerifyUrl = trim($configSiteUrl).'/externalCertificate.php/?cretid='.$certId;
						$filepath =  $_SERVER['DOCUMENT_ROOT'] .'/'.$certresults->certificate_img;
						//$filepath = trim($configSiteUrl).'/'.$certresults->certificate_img;
						$link = '
					    <iframe id = "verify_cert" src="'.$VerifyUrl.'" name="foo" style = "border:none; height:39px; width:84px; display: inline-block;vertical-align: middle;" >
					      <form action="../mydetail.asp" method="post" target="foo">
					       <input type="submit" value="Yes" name = "yes">
					      </form>
					    </iframe>';
						$notificationInfo =	"'full_name>|',ifnull(per.full_name,''),
						'~|verifier>|',ifnull(prs.user_name,''),
						'~|certificate>|',ifnull(skill.skill_name,''),
						'~|company>|',ifnull(skill.company,'  -'),
						'~|certificate_number>|',ifnull(skill.certificate_number,'  -'),
								'~|certificate_attachment>|','".$filepath."',
					'~|vreifylink>|','".$link."',
						'~|subject>|','".$subject."',
								'~|validity>|','".$validity."',
						'~|verification_duration>|','".$certresults->validity."'";
						//foreach ($emailSendTo as $usertype){
						expDebug::dPrint(" email send to ".$usertype ,5);
					
						$select = db_select('slt_skill_set','skill');
						$select->innerJoin('slt_person','per','per.id = skill.user_id');
						$select->innerJoin('slt_person','prs','prs.user_name =skill.user_name');
						$select->innerJoin('users','usr','usr.name = per.user_name');
						//$select->addField('skill','id','skill_id');
							
							
						$select->addExpression('\'verification_reminder\'', 'msg_id');
						
						//$select->addExpression("if(find_in_set(prs.preferred_language,'".$notificationTemplateInfo['frmlng']."')>=0, prs.preferred_language,'cre_sys_lng_eng')", 'lang_code');
						
						
						
						$select->addExpression('\'verification expiry reminder\'', 'msg_type');
						$select->addExpression("concat(".$notificationInfo.")", "token_str");
							
						
						//$select->addExpression("concat('objID1>|',prgm.id,'~|objID2>|~|objID3>|~|objID4>|~|objID5>|')","detail_value");
						
							
							
						
						
						//if($usertype == "cre_ntn_rpt_mgr"){
						
						$select->addField('prs','id','user_id');
						$select->addField('usr','uid','send_to_id');
						$select->addField('prs','full_name','send_to_name');
						
						if($certresults->certificate_type == "Internal"){
							$select->addField('prs','email','send_to_email');
						 	$select->addField('prs','preferred_language','lang_code');
						}else{
							$select->addField('skill','email','send_to_email');
							$select->addExpression('\'cre_sys_lng_eng\'', 'lang_code');
						}
						
						$select->addExpression('null', 'send_to_email_cc');
						//	}
						
						$select->addExpression('\'php mailer\'', 'send_type');
						$select->addExpression('\'N\'', 'send_status');
						$select->addExpression('1', 'created_by');
						$select->addExpression('now()', 'created_on');
						$select->addExpression('1', 'updated_by');
						$select->addExpression('1', 'attach_content');
						$select->addExpression('now()', 'updated_on');
						//$select->addExpression('1', 'mobile');
						$select->condition('skill.id', $certresults->skill_id, '=');
						$select->condition('skill.verification_status', 'Verification pending', '=');
						$select->condition('skill.type', 'certificate', '=');
						
						$select->where("DATEDIFF(DATE_ADD(skill.created_on, INTERVAL $cert_expiry_reminder DAY),NOW()) =3");
						
						expDebug::dPrint("Error in sending der ".print_r($certresults,true),1);
						expDebug::dPrintDBAPI('$select CertVerificationReminerBulkinsertNotification sandhyaaaaa', $select);
						db_insert('slt_bulk_notification')
						->from($select)
						->execute();
						
					}
					
					
				
				/* 	db_insert('slt_bulk_notification')
					->from($select)
					->execute(); */
				//}
			}
			$totTime = $this->processedTime($startTime);
			expDebug::dPrint("end time of CertVerificationReminerBulkinsertNotification".gmdate('H:i:s',$totTime),4);
		}catch (Exception $ex) {
			throw new Exception($ex->getMessage());
		}
	}
}
?>
