<?php

/**
 * Expertus Admin Calendar
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 06-Feb-2016
 */

include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration.inc";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/exp_sp_core.inc";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc";include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_learning/exp_sp_administration_session_details.inc";

define("EVENT_LIMIT_MONTH",4);
define("EVENT_LIMIT_WEEK",35);
define("EVENT_LIMIT_DAY",300);


function isSessionExistsAlready($locationId,$sessId,$newDate,$newStartTime, $newEndTime)
{	
		$locationScheduled = getLocationScheduledSession($locationId,$sessId);
		foreach($locationScheduled as $id => $row)
		{
				
				$roomSessId    = $row->sessionid;
	     		$vSesTimeZone = $row->session_timezone;
				$stDate = $row->ses_start_date. " " .$row->start_time;
				$enDate = $row->ses_start_date. " " .$row->end_time;
				
				$vSysPHPTimezonename = date_default_timezone(FALSE);
				$ses_start_date = timeZoneConvert($stDate, $vSesTimeZone,  $vSysPHPTimezonename);
				$ses_end_date 	= timeZoneConvert($enDate, $vSesTimeZone,  $vSysPHPTimezonename);
				$roomSesStartTime = strtotime($ses_start_date->format('Y-m-d H:i:s'));
				$roomSesEndTime = strtotime($ses_end_date->format('Y-m-d H:i:s'));
				
				$newDate = date('Y-m-d', strtotime($newDate));
				$newStartDateTime = date("Y-m-d H:i",strtotime($newDate." ".$newStartTime));
				$newEndDateTime = date("Y-m-d H:i",strtotime($newDate." ".$newEndTime));
				$new_start_date = timeZoneConvert($newStartDateTime, $vSesTimeZone,  $vSysPHPTimezonename);
				$new_end_date 	= timeZoneConvert($newEndDateTime, $vSesTimeZone,  $vSysPHPTimezonename);
				$new_start_date_time = strtotime($new_start_date->format('Y-m-d H:i:s'));
			  	$new_end_date_time = strtotime($new_end_date->format('Y-m-d H:i:s'));
			  	
			  	
			  	if(($roomSesStartTime < $new_start_date_time && $roomSesEndTime <= $new_start_date_time) || ($roomSesStartTime >= $new_end_date_time && $roomSesEndTime > $roomSesStartTime) ) { //|| ($sessionID == $roomSessId) || ($tempLocationID != $locationId) 
				//if(($roomSesStartTime < $new_start_date_time && $roomSesEndTime <= $new_start_date_time) ) {
				}
				else{
					return t('ERR146');
				}
		}
		return "";
}
function checkInstructorConflict($classId,$sessionId,$newdate,$newStartTime,$newEndTime,$type=''){
	$start_date_arr = explode(' ',$newdate);
	$newStartTime = date("H:i", strtotime("$newStartTime"));
	$newEndTime = date("H:i", strtotime("$newEndTime"));
	$tempCDate = $newdate= $start_date_arr[0];
	expDebug::dPrint("New Start Date -->> ". print_r($newdate,true),4);
	expDebug::dPrint("New Starttime -->> ". print_r($newStartTime,true),4);
	expDebug::dPrint("New End time -->> ". print_r($newEndTime,true),4);
	$tempStartHours = explode(':',$newStartTime);
	$tempCSHrs = $tempStartHours[0];
	$tempCSMins = $tempStartHours[1];
	$tempEndHours = explode(':',$newEndTime);
	$tempCEHrs = $tempEndHours[0];
	$tempCEMins = $tempEndHours[1];
	$sesstion_details = _getSessionDetails($classId,'',$sessionId);
	expDebug::dPrint("sesstion_details -->> ". print_r($sesstion_details,true),4);
	$time_zone = $sesstion_details[0]->timezone;
	if($time_zone){
		$tzArray   = getProfileListItemByCode($time_zone);
		$time_zone = $tzArray[0]->attr2;
	}else{
		$time_zone = date_default_timezone(FALSE);
	}
	//$time_zone = date_default_timezone(FALSE);
	$vSesTimeZone = $time_zone;
	list($y,$m,$d) = explode("-",$tempCDate);
	$stDate = $y.'-'.$m.'-'.$d. " " .$tempCSHrs.":".$tempCSMins;
	$enDate = $y.'-'.$m.'-'.$d. " " .$tempCEHrs.":".$tempCEMins;
	expDebug::dPrint("Start Date -->> ". print_r($stDate,true),4);
	expDebug::dPrint("End DATE -->> ". print_r($enDate,true),4);
	//convert the session time to system time from session timezone
	$vSysPHPTimezonename = date_default_timezone(FALSE);
	$ses_start_date = timeZoneConvert($stDate, $vSesTimeZone,  $vSysPHPTimezonename);
	$ses_end_date = timeZoneConvert($enDate, $vSesTimeZone,  $vSysPHPTimezonename);
	$cStartDT = strtotime($ses_start_date->format('Y-m-d H:i:s'));
	$cEndDT = strtotime($ses_end_date->format('Y-m-d H:i:s'));
	$sessionDets = array();
	$sessionAvailable = 1;
	$sessionDets = fetchSessionDetailsShort($classId,$sessionId);
	if(count($sessionDets) == 0){
		$sessionDets[0]->start_date = $newdate;
		$sessionDets[0]->start_hours = $newStartTime;
		$sessionDets[0]->end_hours = $newEndTime;
		$sessionAvailable = 0;
	}
	expDebug::dPrint("Session count" . count($sessionDets),4);
	for($j=0;$j<count($sessionDets);$j++) {
	
		$tempPDate  = $sessionDets[$j]->ses_start_date;
		$tempPSTime = $sessionDets[$j]->start_hours;
		$tempPETime = $sessionDets[$j]->end_hours;
		//condtion to check whether the id is available
		if($tempPDate != '' && $tempPDate != 'undefined') {
	
			//previous sessions date and time construct strings
			$prevStartDTStr = $tempPDate." ".$tempPSTime;
			$prevEndDTStr = $tempPDate." ".$tempPETime;
	
			$vSesTimeZone = empty($sessionDets[$j]->session_timezone)?$vSesTimeZone:$sessionDets[$j]->session_timezone;
	
			$vSysPHPTimezonename = date_default_timezone(FALSE);
			$ses_start_date = timeZoneConvert($prevStartDTStr, $vSesTimeZone,  $vSysPHPTimezonename);
			$ses_end_date = timeZoneConvert($prevEndDTStr, $vSesTimeZone,  $vSysPHPTimezonename);
			//previous session date time construct
			$pStartDT = strtotime($ses_start_date->format('Y-m-d H:i:s'));
			$pEndDT = strtotime($ses_end_date->format('Y-m-d H:i:s'));
			if($sessionAvailable == 1){
				if(($cStartDT < $pStartDT && $cEndDT <= $pStartDT) || ($cStartDT >= $pEndDT && $cEndDT > $cStartDT)) {
	
				}else {
					echo t('ERR107');
		 			drupal_exit();
				}
			}
		}
	}
	$instructorId = getInstructorInfo($sessionId);
	$instructorId = array_unique($instructorId);
	expDebug::dPrint("instructor Details Array -->> ". print_r($instructorId,true),4);
	$err_flag = 0;
	$i=0;
	$invalid_ins = array();
	//Check for instructor conflict
	if(count($instructorId)>0){ 
		foreach($instructorId as $ins){
			$instructorScheduled = getInstructorScheduledSession($ins,$sessionId);
			expDebug::dPrint("instructor conflict Array -->> ". print_r($instructorScheduled,true),4);
			foreach($instructorScheduled as $id => $row){
				$vSesTimeZone = $row->session_timezone;
				$stDate = $row->ses_start_date. " " .$row->start_time;
				$enDate = $row->ses_start_date. " " .$row->end_time;
				$vSysPHPTimezonename = date_default_timezone(FALSE);
				$ses_start_date = timeZoneConvert($stDate, $vSesTimeZone,  $vSysPHPTimezonename);
				$ses_end_date = timeZoneConvert($enDate, $vSesTimeZone,  $vSysPHPTimezonename);
				$insSesStartTime = strtotime($ses_start_date->format('Y-m-d H:i:s'));
				$insSesEndTime = strtotime($ses_end_date->format('Y-m-d H:i:s'));
				expDebug::dPrint("insSesStartTime -->> ". print_r($insSesStartTime,true),4);
				expDebug::dPrint("insSesEndTime -->> ". print_r($insSesEndTime,true),4);
				expDebug::dPrint("cStartDT -->> ". print_r($cStartDT,true),4);
				expDebug::dPrint("cStartDT -->> ". print_r($cStartDT,true),4);
				if(($insSesStartTime < $cStartDT && $insSesEndTime <= $cStartDT) || ($insSesStartTime >= $cEndDT && $insSesEndTime > $insSesStartTime)) {
	
				}
				else{
					$err_flag = 1;
					$invalid_ins[$i] = $row->first_name.' '.$row->last_name;
					$i++;
				}
			}
		}
 		expDebug::dPrint("instructor conflict session -->> ". print_r($invalid_ins,true),4);
	}
	//Check for instructor has any enrollment conflict
	if(count($instructorId)>0){
		foreach($instructorId as $ins){
			$instructorScheduled = getInstructorEnrolledSession($ins);
			expDebug::dPrint("instructor Enrolled Array -->> ". print_r($instructorScheduled,true),4);
			foreach($instructorScheduled as $id => $row){
				$vSesTimeZone = $row->session_timezone;
				$stDate = $row->ses_start_date. " " .$row->start_time;
				$enDate = $row->ses_start_date. " " .$row->end_time;
				$vSysPHPTimezonename = date_default_timezone(FALSE);
				$ses_start_date = timeZoneConvert($stDate, $vSesTimeZone,  $vSysPHPTimezonename);
				$ses_end_date = timeZoneConvert($enDate, $vSesTimeZone,  $vSysPHPTimezonename);
				$insSesStartTime = strtotime($ses_start_date->format('Y-m-d H:i:s'));
				$insSesEndTime = strtotime($ses_end_date->format('Y-m-d H:i:s'));
				if(($insSesStartTime < $cStartDT && $insSesEndTime <= $cStartDT) || ($insSesStartTime >= $cEndDT && $insSesEndTime > $insSesStartTime)) {
	
				}
				else{
					$err_flag = 1;
					$invalid_ins[$i] = $row->first_name.' '.$row->last_name;
					$i++;
				}
			}
		}
		expDebug::dPrint("instructor conflict enrollment -->> ". print_r($invalid_ins,true),4);
	}
	return $invalid_ins;
}

function updateObjectData()
{
	$id 		= 	$_REQUEST["id"];
	$type 		= 	$_REQUEST["type"];
	$newdate 	=	$_REQUEST["newdate"];
	$classid 	=   core_decrypt($_REQUEST["classid"]);
	$location_id = 	$_REQUEST["location_id"];
	$newStartTime = $_REQUEST["startTime"];
	$newEndTime = 	$_REQUEST["endTime"];
	$enddate =  	$_REQUEST["enddate"];
	$allDay  =  	$_REQUEST["allDay"];
	$currUserId = getSltpersonUserId();
	expDebug::dPrint("Request:: " . print_r($_REQUEST,true),5);
	$permissionList = getAdminCalendarSettings('list'); // retrive user permission from system

	if($type == "lrn_cls_dty_ilt" || $type == "lrn_cls_dty_vcl")
	{
		$startdate = $newdate;
		$sql = " select count(*) regcount from {slt_enrollment} enr where enr.class_id= :classid and enr.reg_status = 'lrn_crs_reg_cnf'";
		$res = db_query($sql,array(':classid' => $classid))->fetchAll();
		foreach($res as $row)
		{
			$regcount = $row->regcount;
		}
		if($regcount == 1){
			echo $regcount." ".t("ACLBL0032").".".t("ACLBL0034");
			drupal_exit();
		}
		else if($regcount > 1)
		{
			echo $regcount." ".t("ACLBL0035").".".t("ACLBL0034");
			drupal_exit();
		}
		
		$clsWarning = isSessionExistsAlready($location_id,$id,$newdate,$newStartTime,$newEndTime);
		 if($clsWarning != "")
		 {
		 	echo $clsWarning;
		 	drupal_exit();
		 }
		 $sesInsConflictUser = checkInstructorConflict($classid,$id,$newdate,$newStartTime,$newEndTime,$type);
		 if(count($sesInsConflictUser)>0){
		 	$sesInsConflictUser = array_unique($sesInsConflictUser);
		 	$err_msg = implode(',',$sesInsConflictUser).'-'.t('MSG435');
		 	echo $err_msg;
		 	drupal_exit();
		 }
		 // convert user timezone to server timezone
		 if ($type == "lrn_cls_dty_vcl") {
		 	$session_timezone 	= $_REQUEST['timezone'];
		 	$userTimezone 		= getUserPHPTimezoneName();
		 	$datetime = new DateTime($newdate);
			$enddate = $datetime->format('Y-m-d');
			$startdate = $datetime->format('Y-m-d');
		 	$vSysPHPTimezonename = date_default_timezone(FALSE);
		 	expDebug::dPrint(" vc start date :: " . $vcstartDate);
		 	expDebug::dPrint(" vc end date :: " . $vcendDate);
		 }
		 $transaction = db_transaction();		
        try{
           if($id){
           		$selectSql = db_select('slt_course_class_session','sc');
           		$selectSql->addField('sc','start_date','start_date');
           		$selectSql->addField('sc','start_time','start_time');
           		$selectSql->addField('sc','end_time','end_time');
           		$selectSql->addField('sc','updated_on','updated_on');
           		$selectSql->condition('id', $id, '=');
           		$sessionDetail = $selectSql->execute();
           		$results =  $sessionDetail->fetchAll();
                $catalog = db_update('slt_course_class_session');
            	$fields = array('start_date' => $startdate ,
                                    'start_time' => $newStartTime,
                                    'end_time' => $newEndTime,
           							'updated_on' =>now());
                $catalog->fields(array(
                                    'start_date' => $startdate ,
                                    'start_time' => $newStartTime,
                                    'end_time' => $newEndTime,
                					'updated_by' =>$currUserId,
           							'updated_on' =>now()
                                    ));
                $catalog->condition('id', $id, '=');
                 expDebug::dPrintDBAPI('ILT/VC update >>> ',$catalog);
                $calendarUpdate = $catalog->execute();
                make_audit_trail_entries($fields, $currUserId , $results ,$calendarUpdate, 'Updated','cre_sys_obt_cls','exp_sp_admincalendar','updateObject',
                		array('start_date', 'start_time', 'end_time', 'updated_on'));
               }
             }catch (Exception $e) {
                 $transaction->rollback();
                 unset($transaction);
                 expertusErrorThrow($e);
                }
	echo "success";
	drupal_exit();
	}
	else if($type == "event_regend")
	{
		if($permissionList["create_class"]	!= "enabled")
		{
			echo t("ACLBL0033");
			drupal_exit();
		}
		 $transaction = db_transaction();
        try{
           if($id){
                $catalog = db_update('slt_course_class');
                $catalog->fields(array(
                                    'registration_end_on' => $newdate,
                					'updated_by' =>$currUserId,
           							'updated_on' =>now()
                                    ));
                $catalog->condition('id', $id, '=');
                $catalog->execute();
                add_audit_trail_entry($currUserId, $id, '', 'exp_sp_admincalendar', 'updateObject', 'Updated registration date in slt_course_class ',null, null, $newdate);
                
              }
            }catch (Exception $e) {
                 $transaction->rollback();
                 unset($transaction);
                 expertusErrorThrow($e);
                }
	echo "success";
	drupal_exit();
	}
	else if($type == "event_regcancel")
	{
		if($permissionList["create_class"]	!= "enabled")
		{
			echo t("ACLBL0033");
			drupal_exit();
		}
		$transaction = db_transaction();
        try{
           if($id){
                $catalog = db_update('slt_course_class');
                $catalog->fields(array(
                                    'cancellation_deadline_date' => $newdate,
                					'updated_by' =>$currUserId,
           							'updated_on' =>now()
                                    ));
                $catalog->condition('id', $id, '=');
                //expDebug::dPrintDBAPI('ILT/VC update >>> ',$catalog);
                $catalog->execute();
				add_audit_trail_entry($currUserId, $id, '', 'exp_sp_admincalendar', 'updateObject', 'Updated cancellation_deadline_date in slt_course_class ',null, null, $newdate);
                
              }
            }catch (Exception $e) {
                 $transaction->rollback();
                 unset($transaction);
                 expertusErrorThrow($e);
                }
	echo "success";
	drupal_exit();
	}
	else if($type == "cre_sys_obt_anm")
	{
		if($permissionList["create_announcement"]	!= "enabled")
		{
			echo t("ACLBL0033");
			drupal_exit();
		}
		
		 $transaction = db_transaction();
	try{
        if($id){
				$anmRes = '';
				$anmDayTimRes = '';
				$selAnm = db_select("slt_site_notice",'ntc');
				$selAnm->addExpression('IFNULL(DATEDIFF(to_date,from_date),\'N\')','daysdiff');
				$selAnm->condition('ntc.id',$id,'=');
				$selAnm->where("'$newdate' > now()");
				$selAnm->where("if((from_date = to_date AND from_time != 'hh:mm'), (date_format(concat(from_date,' ',from_time),'%Y-%m-%d %H:%i:%s') >= now()), from_date >= date_format(now(),'%Y-%m-%d'))");
				
				expDebug::dPrintDBAPI("Query for day difference calculation:",$selAnm);
				$anmRes = $selAnm->execute()->fetchField();
				expDebug::dPrint("Value of the result from announcement".$anmRes,4);
				if($_COOKIE['admincalendar_view'] == 'agendaDay'){
					$selAnm = db_select("slt_site_notice",'ntc');
					$selAnm->addExpression("TIMESTAMPDIFF(SECOND, date_format(concat(from_date,' ',from_time),'%Y-%m-%d %H:%i:%s'), now())",'timediff');
					$selAnm->condition('ntc.id',$id,'=');
					$selAnm->where("'$newdate' > now()");
					$selAnm->where("if((from_date = to_date AND from_time != 'hh:mm'), (date_format(concat(from_date,' ',from_time),'%Y-%m-%d %H:%i:%s') >= now()), from_date >= date_format(now(),'%Y-%m-%d'))");
					
					expDebug::dPrintDBAPI("Query for day Time difference calculation:",$selAnm);
					$anmDayTimRes = $selAnm->execute()->fetchField();
					expDebug::dPrint("Value of the result from announcement".$anmDayTimRes,4);
				}
				
			  if($anmRes != '' || ($_COOKIE['admincalendar_view'] == 'agendaDay' && $anmDayTimRes!='')){
					if($anmRes == 'N'){
						$catalog = db_update('slt_site_notice');
						$catalog->fields(array(
								'updated_by' =>$currUserId,
								'updated_on' =>now()
						));
						$catalog->expression('from_date', "DATE_FORMAT('".$newdate."','%Y-%m-%d')");
						$catalog->condition('id', $id, '=');
						expDebug::dPrintDBAPI('Announcement update announcement without end date ',$catalog);
						$catalog->execute();
						add_audit_trail_entry($currUserId, $id, 'cre_sys_obt_anm', 'exp_sp_admincalendar', 'updateObject', 'Updated from_date,to_date in slt_site_notice ',null, null, $newdate." ".$enddate);
					}else if($anmRes == 0){
						$catalog = db_update('slt_site_notice');
						$catalog->fields(array(
								'updated_by' =>$currUserId,
								'updated_on' =>now()
						));
						if($_COOKIE['admincalendar_view'] == 'agendaDay'){
							$catalog->expression('from_time', "DATE_FORMAT('".$newdate."','%H:%i')");
							$catalog->expression('to_time', "DATE_FORMAT('".$enddate."','%H:%i')");
						}else{
							$catalog->expression('from_date', "DATE_FORMAT('".$newdate."','%Y-%m-%d')");
							$catalog->expression('to_date', "DATE_FORMAT('".$newdate."','%Y-%m-%d')");
						}
						
						$catalog->condition('id', $id, '=');
						expDebug::dPrintDBAPI('Announcement update for day difference zero ',$catalog);
						$catalog->execute();
						add_audit_trail_entry($currUserId, $id, 'cre_sys_obt_anm', 'exp_sp_admincalendar', 'updateObject', 'Updated from_date,to_date in slt_site_notice ',null, null, $newdate." ".$enddate);
					}else if($_COOKIE['admincalendar_view'] != 'agendaDay' && $anmRes > 0){
						$catalog = db_update('slt_site_notice');
						$catalog->fields(array(
								'updated_by' =>$currUserId,
								'updated_on' =>now()
						));
						$catalog->expression('from_date', "DATE_FORMAT('".$newdate."','%Y-%m-%d')");
						$catalog->expression('to_date', "DATE_ADD(DATE_FORMAT('".$newdate."','%Y-%m-%d'), INTERVAL ".$anmRes." DAY)");
						$catalog->condition('id', $id, '=');
						expDebug::dPrintDBAPI('Announcement update for multiple days ',$catalog);
						$catalog->execute();
						add_audit_trail_entry($currUserId, $id, 'cre_sys_obt_anm', 'exp_sp_admincalendar', 'updateObject', 'Updated from_date,to_date in slt_site_notice ',null, null, $newdate." ".$enddate);
					}
				}   

              }
            }catch (Exception $e) {
                 $transaction->rollback();
                 unset($transaction);
                 expertusErrorThrow($e);
                }
		if(($_COOKIE['admincalendar_view'] != 'agendaDay' && $anmRes == '') || ($_COOKIE['admincalendar_view'] == 'agendaDay' && empty($anmDayTimRes)))
			echo t('ACLBL0021');
		else 
			echo "success";
	drupal_exit();
	}
	else if($type == "report")
	{
	
		$transaction = db_transaction();
        try{
        	if($id){
                $catalog = db_update('slt_report_schedules');
                $catalog->fields(array(
                                    'next_run' => $newdate,
                					'updated_by' =>$currUserId,
           							'updated_on' =>now()
                                    ));
                $catalog->condition('id', $id, '=');
                $catalog->execute();
				add_audit_trail_entry($currUserId, $id, '', 'exp_sp_admincalendar', 'updateObject', 'Updated next_run in slt_report_schedules ',null, null, $newdate);
                
              }
            }catch (Exception $e) {
                 $transaction->rollback();
                 unset($transaction);
                 expertusErrorThrow($e);
                }
	echo "success";
	drupal_exit();
		
	}
	
}
//Admin Calendar
function getCalendarViewData()
{
	try
	{
	$_REQUEST["startDate"] = date("Y-m-01");//date('m-01-Y'); 
	
	$_REQUEST["startDate"] = strtotime("-15 days", strtotime($_REQUEST["startDate"]));
    $_REQUEST["startDate"] = date("Y-m-d", $_REQUEST["startDate"]);
    
	$_REQUEST["endDate"] = date("Y-m-t");//date('m-t-Y');
	

	
	$_REQUEST["endDate"] = strtotime("+15 days", strtotime($_REQUEST["endDate"]));
    $_REQUEST["endDate"] = date("Y-m-d", $_REQUEST["endDate"]);


 	return getCalendarViewDataProcess(); //echo  drupal_json_encode($consolidated);
 	} catch (Exception $e) {
        expertusErrorThrow($e);
    }
 	//drupal_exit();
 
}
function getCalendarViewDataJson()
{
	try
	{
		ob_start("ob_gzhandler");
		echo getCalendarViewDataProcess(); //echo  drupal_json_encode($consolidated);
		ob_end_flush();
		exit();
	} catch (Exception $e) {
        expertusErrorThrow($e);
    }

}

function getProfileListItemAllTZs($languageCode= 'cre_sys_lng_eng')
{
	try{
		$select = db_select('slt_profile_list_items', 'plist');
		$select->addField('plist','id', 'id');
		$select->addField('plist','name', 'name');
		$select->addField('plist','code', 'code');
		$select->addField('plist','is_active', 'status');
		$select->addField('plist','attr1', 'attr1');
		$select->addField('plist','attr2', 'attr2');
		$select->addField('plist','attr3', 'attr3');
		$select->addField('plist','attr4', 'attr4');
		$select->condition('plist.code', db_like('cre_sys_tmz_') . '%', 'LIKE');
		$select->condition('plist.lang_code', $languageCode,'=');
		$result = $select->execute()->fetchAll();
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getProfileListItemByCode', $ex);
		expertusErrorThrow($ex);
	}
}
//Changed by vetrivel.P for #0064978
function getCalendarViewDataProcess($autocomplete = false)
{
	try
	{
	$sorteddata = array();

	$permissionList = getAdminCalendarSettings('list'); // retrive user permission from system
	$prefList 		= getAdminCalendarPreference();
	expDebug::dPrint('calendar permissionList: ' . print_r($permissionList,true), 4);
	$classrooms = array();
	$vcrooms = array();
	$consolidated = array();
	$events = array();
	
	$searchFilterAnn = true;
	$searchFilterClasses = true;
	$searchFilterReports = true;
	$searchFilterOrders = true;
	$searchFilterEvents = true;
	$searchFilterLocation = false;
	$searchFilterInstractor = false;

	$search_type = $_REQUEST["search_type"];
	
	$omitdatefilter = $_REQUEST["omitdatefilter"];
	if($omitdatefilter  == true || $omitdatefilter == "true")
		$autocomplete = true;
		
	$search_text = "";
	$filterApplied = false;
	
	
	$search_text =  $_REQUEST["search_text"];
	
	if($search_type != null && $search_type != "")
	{
		$filterApplied = true;
		$search_text =  $_REQUEST["search_text"];
		if($search_type == "view_classroom")
		{
			$searchFilterAnn = false;
			$searchFilterClasses = true;
			$searchFilterReports = false;
			$searchFilterOrders = false;
			$searchFilterEvents = false;
			$searchFilterLocation = false;
			$searchFilterInstractor = false;
		}
		/*else if($search_type  == "view_announcements")
		{
			$searchFilterAnn = true;
			$searchFilterClasses = false;
			$searchFilterReports = false;
			$searchFilterOrders = false;
			$searchFilterEvents = false;
			$searchFilterLocation = false;
			$searchFilterInstractor = false;
		}*/
		else if($search_type  == "view_orders")
		{
			$searchFilterAnn = false;
			$searchFilterClasses = false;
			$searchFilterReports = false;
			$searchFilterOrders = true;
			$searchFilterEvents = false;
			$searchFilterLocation = false;
			$searchFilterInstractor = false;
		}
		else if($search_type  == "view_scheduled_report")
		{
			$searchFilterAnn = false;
			$searchFilterClasses = false;
			$searchFilterReports = true;
			$searchFilterOrders = false;
			$searchFilterEvents = false;
			$searchFilterLocation = false;
			$searchFilterInstractor = false;
		}
		/*else if($search_type  == "view_events")
		{
			$searchFilterAnn = false;
			$searchFilterClasses = false;
			$searchFilterReports = false;
			$searchFilterOrders = false;
			$searchFilterEvents = true;
			$searchFilterLocation = false;
			$searchFilterInstractor = false;
		} */
		else if($search_type == "All")
		{
			$searchFilterAnn = false;
			$searchFilterClasses = true;
			$searchFilterReports = true;
			$searchFilterOrders = true;
			$searchFilterEvents = false;
			$searchFilterLocation = true;
			$searchFilterInstractor = true;
		}
		else if($search_type  == "view_locations")
		{
			$searchFilterAnn = false;
			$searchFilterClasses = false;
			$searchFilterReports = false;
			$searchFilterOrders = false;
			$searchFilterEvents = false;
			$searchFilterLocation = true;
			$searchFilterInstractor = false;
		}
		else if($search_type == "view_instructors")
		{
			$searchFilterAnn = false;
			$searchFilterClasses = false;
			$searchFilterReports = false;
			$searchFilterOrders = false;
			$searchFilterEvents = false;
			$searchFilterLocation = false;
			$searchFilterInstractor = true;
		}
	}
	
	expDebug::dPrint("suresh=".$search_type."=$searchFilterAnn=".$searchFilterAnn."=classes=".$searchFilterClasses."searchFilterReports=".$searchFilterReports."=searchFilterOrders=".$searchFilterOrders);
//In if conduction added $_GET['calendar_searchtype'] for while searching location auto-complete is seprete function but displaying result we use already existing function of class.
	if(($searchFilterClasses  && $search_type  != "view_orders" && $search_type!='view_scheduled_report' && $permissionList['view_classroom'] == 'enabled' && ($prefList->view_classroom || $prefList->view_virtualclassroom) && $_REQUEST['type'] != "more") 
			|| (empty($_GET['calendar_searchtype']) && $search_type  != "view_orders" && $search_type!='view_scheduled_report' && (($permissionList['view_instructors'] == "enabled" && $prefList->view_instructors) || ($permissionList['view_locations'] == 'enabled' && $prefList->view_locations)))
		) // Check permission enabled for classrooms and vcrooms
	{
		$classes = getScheduledClassesForAdminCalendar($_REQUEST['startDate'],$_REQUEST['endDate'],$search_text,$autocomplete,$search_type);
		
		$class_status_meta_array = getProfileListItemNamesByParentId(325);
		$all_tzs =  getProfileListItemAllTZs();

		$startDateTime = strtotime($_REQUEST['startDate']);
		$endDateTime = strtotime($_REQUEST['endDate']);
		$fields = new stdClass();
		
		
		$limitresults = limitACSearchResults();
		for($i = 0; $i < count($classes) ; $i++)
		{	
			$catalog = $classes[$i];
			$dt1 =  date('m-d-Y',strtotime($catalog->start_date));
			$dt2 =  date('m-d-Y',strtotime($classes[$i+1]->start_date));
			if($dt1 == $dt2 &&  $limitresults->evtCntPerDay != 0 && $limitresults->evtCntPerDay == $limitresults->evtLimit)
			{
				//$evtCntPerDay = 0;
				continue;
			}
			if($dt1 == $dt2)
			{
				$limitresults->evtCntPerDay++;
			}
			else
			{
				$limitresults->evtCntPerDay = 0;
			}

			if($catalog->delivery_type == "lrn_cls_dty_ilt")
			{
			 	$fields->type = "lrn_cls_dty_ilt";
			 	$classrooms[]	= rowPopulate($catalog,$fields,$class_status_meta_array,$all_tzs);
			}
			else if($catalog->delivery_type == "lrn_cls_dty_vcl")
			{
			 	$fields->type = "lrn_cls_dty_vcl";
			 	$vcrooms[] 		= rowPopulate($catalog,$fields,$class_status_meta_array,$all_tzs);
			}
		}
	   $classrooms 	= ($prefList->view_classroom) ? $classrooms : array();
	   $vcrooms 	= ($prefList->view_virtualclassroom) ? $vcrooms : array();
 	} else {
		 	$classrooms = array();
		 	$vcrooms = array();
	}
	/* if($searchFilterEvents && $permissionList['view_events'] == 'enabled' && $prefList->view_events) // Check permission enabled for events
	{
		 $admevents = getAdminCalendarEvents($_REQUEST['startDate'],$_REQUEST['endDate'],$search_text,$autocomplete);
		 
		$limitresults = limitACSearchResults();
		 $fields = new stdClass();
		 		
		for($i = 0; $i < count($admevents) ; $i++)
		{
			$catalog = $admevents[$i];
			$dt1 =  date('m-d-Y',strtotime($catalog->registration_end_on));
			$dt2 =  date('m-d-Y',strtotime($admevents[$i+1]->registration_end_on));
			if($dt1 == $dt2 &&  $limitresults->evtCntPerDay != 0 && $limitresults->evtCntPerDay == $limitresults->evtLimit)
			{
				continue;
		 	}
			if($dt1 == $dt2)
		 	{
				$limitresults->evtCntPerDay++;
			}
			else
			{
				$limitresults->evtCntPerDay = 0;
			}
		    if($catalog->registration_end_on != null && $catalog->registration_end_on != "")
		 	{
		 		$fields->type="event_regend";
		 		$events[] = rowPopulate($catalog,$fields);
		 		
		 	}
		}
	 } else {
	 	$events = array();
	 }	*/
	// Buidling classrooms, vcrooms, events, announcements and orders data
	$sorteddata = array_merge($sorteddata,$classrooms);
	$sorteddata = array_merge($sorteddata,$vcrooms);
	if($searchFilterOrders)
	{
		$consolidated["orders"]  		= ($permissionList['commerce'] == 1 && $permissionList['view_orders'] == 'enabled' && $prefList->view_orders) ? getOrdersForAdminCalendar($autocomplete) : array();
		$sorteddata = array_merge($sorteddata,$consolidated["orders"]);
	}

	// Buidling reports data
	if ($searchFilterReports && $permissionList['view_scheduled_report'] == 'enabled' && $prefList->view_scheduled_report)
	{
		$reportsRawData  = getScheduledReportDaywise($_REQUEST['startDate'],$_REQUEST['endDate'],$search_text,$autocomplete);
		$consolidated["reports"]  = $reportsRawData;
		$sorteddata = array_merge($sorteddata,$consolidated["reports"]);
		
	} else {
		$consolidated["reports"]  = array();
	}
	
	// Buidling locations data //Changed by vetrivel.P for #0064978
	if ($searchFilterLocation && $permissionList['view_locations'] == 'enabled' && $prefList->view_locations && $_REQUEST['type'] != 'more') {
		$locationsRawData	=getOccupiedLocations($_REQUEST['startDate'],$_REQUEST['endDate'],$search_text,$autocomplete);
		$consolidated["locations"] = $locationsRawData;
		$sorteddata = array_merge($sorteddata,$consolidated["locations"]);
	} else {
		$consolidated["locations"] = array();
	}
	//INSTRACTOR //Changed by vetrivel.P for #0064978
	
	if($searchFilterInstractor && $permissionList['view_instructors'] == "enabled" && $prefList->view_instructors && $_REQUEST['type'] != 'more') {
		$instructorsRawData = getOccupiedInstructorsForDay($_REQUEST['startDate'],$search_text,$autocomplete);
		$consolidated['instructors'] = $instructorsRawData;
		$sorteddata = array_merge($sorteddata,$consolidated["instructors"]);
	}else{
		$consolidated['instructors'] = array();
	}
	expDebug::dPrint('The value of $sorteddata is here '.print_r($sorteddata,1),4);
	
	$printorexport = "";
	if($_REQUEST["export"] == true || $_REQUEST["print"] == true)
	{
		$csvDataHeader = "";
		$lbl_title =  t('LBL083');
		$lbl_startDate = t('LBL045') .' / '. t('LBL1220');
		$lbl_endDate = t('LBL100') .' / '. t('LBL1220');
		$lbl_timezone = t('LBL297');
		$lbl_type = t('LBL036');
		
		if($_REQUEST["export"] == true )
		{
			 $csvDataHeader = $lbl_title . "," . $lbl_startDate . "," . $lbl_endDate . ",". $lbl_timezone . "," . $lbl_type  ."\n";	
			 $printorexport = "export";
		}
		else
		{
			$csvDataHeader =  "<table style='border-collapse: collapse; border: 1px solid #000;' class='calendar_print_table_css' width='100%'><tr><td style='border: 1px solid #000;' class='calendar_print_td_css' width='40%'><b>" . $lbl_title . "</b></td><td style='border: 1px solid #000;' class='calendar_print_td_css' width='20%'><b>" . $lbl_startDate . "</b></td><td style='border: 1px solid #000;' width='20%'><b>" . $lbl_endDate . "</b></td><td style='border: 1px solid #000;' class='calendar_print_td_css' width='10%'><b>" . $lbl_timezone . "</b></td><td style='border: 1px solid #000;' class='calendar_print_td_css' width='10%'><b>" . $lbl_type . "</b></td></tr>";
			$printorexport = "print";
		}

		//$selectedObjectsStr = $_REQUEST["selectedobjects"];
		$selectedObjectsStr = $_COOKIE['admincalendar_pref'];		
		$selectedObjects = drupal_json_decode($selectedObjectsStr);
		
		//#61417
		//$filename = time().'.csv';
		$asFilename = 'export';
		$filename = getDownloadFileName($asFilename, "csv");
		
		if($_REQUEST["export"] == true)
		{
			header('Content-Type: application/csv');
		    // tell the browser we want to save it instead of displaying it
    		header('Content-Disposition: attachment; filename="'.$filename.'";');
			//fpassthru($fp);
		}else if($_REQUEST["print"] == true)
		{	
		
		}
		$_SESSION['total_record'] =0;
		$pdfMaxRecords = 500; // default 500 records
		$configPdfMaxRecords = trim($expConfig['pdf_max_records']);
		if (!empty($configPdfMaxRecords)) {
			$pdfMaxRecords = $configPdfMaxRecords;
		}
		//var_dump($announcementHtml);
		echo $csvDataHeader;
		if($searchFilterClasses && $permissionList['view_classroom'] && $selectedObjects["view_classroom"])
		{
			$classroomsHtml = iterateDataForPrintExport($classrooms,"Classroom",$printorexport,$pdfMaxRecords);
			echo $classroomsHtml;
		}
		if($searchFilterClasses && $permissionList['view_virtualclassroom'] && $selectedObjects["view_virtualclassroom"])
		{
			if ($_SESSION['total_record'] < $pdfMaxRecords && $printorexport =='print')
				$vcroomsHtml =  iterateDataForPrintExport($vcrooms,"Virtual Classroom",$printorexport,$pdfMaxRecords);
			elseif($printorexport =='export')
				$vcroomsHtml =  iterateDataForPrintExport($vcrooms,"Virtual Classroom",$printorexport,$pdfMaxRecords);
			echo $vcroomsHtml;
		}
		/*if($permissionList['view_announcements'] && $selectedObjects["view_announcements"])
		{
			$announcementHtml =  iterateDataForPrintExport($consolidated["announcements"] ,"Announcement",$printorexport);
			echo $announcementHtml;
		}
		if($permissionList['view_events'] && $selectedObjects["view_events"])
		{
			$eventsHtml =  iterateDataForPrintExport($events,"Event",$printorexport);
			echo $eventsHtml;
		}*/
		if($permissionList['view_orders'] && $selectedObjects["view_orders"] && $permissionList['commerce'] == 1)
		{
			if ($_SESSION['total_record'] < $pdfMaxRecords && $printorexport =='print')
				$ordersHtml = 	iterateDataForPrintExport($consolidated["orders"],"Order",$printorexport,$pdfMaxRecords);
			elseif($printorexport =='export')
				$ordersHtml = 	iterateDataForPrintExport($consolidated["orders"],"Order",$printorexport,$pdfMaxRecords);
			echo $ordersHtml;
		}
		if($permissionList['view_scheduled_report'] && $selectedObjects["view_scheduled_report"])
		{
			if ($_SESSION['total_record'] < $pdfMaxRecords && $printorexport =='print')
				$reportsHtml = 	iterateDataForPrintExport($consolidated["reports"],"Report",$printorexport,$pdfMaxRecords);
			elseif($printorexport =='export')
				$reportsHtml = 	iterateDataForPrintExport($consolidated["reports"],"Report",$printorexport,$pdfMaxRecords);
		//	var_dump($reportsHtml);
			echo $reportsHtml;
		}
		if($permissionList['view_locations'] && $selectedObjects["view_locations"])
		{
			//if(($search_type == "view_classroom") || ($search_text == "" && $search_type == "")) //Commented for #0064978 by Vetrivel.P And changed to function
			if ($_SESSION['total_record'] < $pdfMaxRecords && $printorexport =='print')
				$locationsHtml = 	iterateDataForPrintExport(getOccupiedLocations($_REQUEST['startDate'],$_REQUEST['endDate'],$search_text,'',true),"Location",$printorexport,$pdfMaxRecords);
			elseif($printorexport =='export')
				$locationsHtml = 	iterateDataForPrintExport(getOccupiedLocations($_REQUEST['startDate'],$_REQUEST['endDate'],$search_text,'',true),"Location",$printorexport,$pdfMaxRecords);
			echo $locationsHtml;
		}
		if($permissionList['view_discussions'] && $selectedObjects["view_discussions"]&& $permissionList['forum'] == true)
		{
			$discussionsRawData = getDiscussions($printorexport);
			if ($_SESSION['total_record'] < $pdfMaxRecords && $printorexport =='print')
				$discussionsHtml = iterateDataForPrintExport($discussionsRawData,"Discussion",$printorexport,$pdfMaxRecords);
			elseif($printorexport =='export')
				$discussionsHtml = iterateDataForPrintExport($discussionsRawData,"Discussion",$printorexport,$pdfMaxRecords);
			echo $discussionsHtml;
		}
		if($permissionList['view_instructors'] == "enabled" && $selectedObjects["view_instructors"])
		{
			//if(($search_type == "view_classroom") || ($search_text == "" && $search_type == "")) //Commented for #0064978 by Vetrivel.P
			if ($_SESSION['total_record'] < $pdfMaxRecords && $printorexport =='print')
				$instructorsHtml = iterateDataForPrintExport(getOccupiedInstructorsForDay($_REQUEST['startDate'],$search_text,'',true),"Instructor",$printorexport,$pdfMaxRecords);
			elseif($printorexport =='export')
				$instructorsHtml = iterateDataForPrintExport(getOccupiedInstructorsForDay($_REQUEST['startDate'],$search_text,'',true),"Instructor",$printorexport,$pdfMaxRecords);
			echo $instructorsHtml;
		}
		if($printorexport=='print')
			echo "</table>";
		drupal_exit();
	}

 //  var_dump($sorteddata);
	if($_REQUEST["narrowcaldaywise"] != true){
		usort($sorteddata,"ac_events_sort");
	}
	//return '[{"id":"announcement-122","fullname":"simbu k","username":"simbu","from_date":"2016-01-04","to_date":"2016-04-01","from_time":"hh:mm","to_time":"02:00","description":"Test ANNOIMMcv","status_code":"cre_sys_obt_not_itv","priority":"1","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":"2016-04-01","ori_end_time":"02:00","ori_start_time":"hh:mm","title":"Test ANNOIMMcv","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"00:00","start":"2016-01-04T00:00:00","actualStart":"2016-01-04T00:00:00","endTime":"02:00","end":"2016-04-01T02:00:00","actualEnd":"2016-04-01T02:00:00","allDay":true,"dayDifference":"88","title_short":"Test ANNOIMMcv"},{"id":"announcement-125","fullname":"simbu k","username":"simbu","from_date":"2016-03-04","to_date":"2016-05-09","from_time":"00:45","to_time":"hh:mm","description":"04-03-2016 ANNM","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"00:45","title":"04-03-2016 ANNM","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"00:45","start":"2016-03-04T00:45:00","actualStart":"2016-03-04T00:45:00","endTime":"23:59","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"22","title_short":"04-03-2016 ANNM"},{"id":"announcement-55","fullname":"simbu k","username":"simbu","from_date":"2016-03-07","to_date":"2016-05-09","from_time":"01:30","to_time":"18:00","description":"ann till 6pm Tells the story of Jesus Christ at age seven as he and his family depart Egypt to return home to Nazareth. Told from his childhood perspective, it follows young Jesus as he grows into his religious identity","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"18:00","ori_start_time":"01:30","title":"ann till 6pm Tells the story of Jesus Christ at age seven as he and his family depart Egypt to return home to Nazareth. Told from his childhood perspective, it follows young Jesus as he grows into his religious identity","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"01:30","start":"2016-03-07T01:30:00","actualStart":"2016-03-07T01:30:00","endTime":"18:00","end":"2016-05-09T18:00:00","actualEnd":"2016-05-09T18:00:00","allDay":true,"dayDifference":"19","title_short":"ann till 6pm Tells the story of Jesus Christ at age seven as he and his family..."},{"id":"announcement-53","fullname":"simbu k","username":"simbu","from_date":"2016-03-07","to_date":"2016-05-09","from_time":"01:30","to_time":"17:00","description":"announcement - till 5pm Tells the story of Jesus Christ at age seven as he and his family depart Egypt to return home to Nazareth. Told from his childhood perspective, it follows young Jesus as he grows into his religious identity","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"17:00","ori_start_time":"01:30","title":"announcement - till 5pm Tells the story of Jesus Christ at age seven as he and his family depart Egypt to return home to Nazareth. Told from his childhood perspective, it follows young Jesus as he grows into his religious identity","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"01:30","start":"2016-03-07T01:30:00","actualStart":"2016-03-07T01:30:00","endTime":"17:00","end":"2016-05-09T17:00:00","actualEnd":"2016-05-09T17:00:00","allDay":true,"dayDifference":"19","title_short":"announcement - till 5pm Tells the story of Jesus Christ at age seven as he and..."},{"id":"announcement-45","fullname":"simbu k","username":"simbu","from_date":"2016-03-09","to_date":"2016-03-31","from_time":"05:00","to_time":"17:00","description":"ann - The Young Messiah","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":"2016-03-31","ori_end_time":"17:00","ori_start_time":"05:00","title":"ann - The Young Messiah","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"05:00","start":"2016-03-09T05:00:00","actualStart":"2016-03-09T05:00:00","endTime":"17:00","end":"2016-03-31T17:00:00","actualEnd":"2016-03-31T17:00:00","allDay":true,"dayDifference":"22","title_short":"ann - The Young Messiah"},{"id":"announcement-72","fullname":"ExpertusONE  Admin","username":"admin","from_date":"2016-03-10","to_date":"2016-05-09","from_time":"hh:mm","to_time":"hh:mm","description":"\u003cspan style=\"color: #444444; font-family: Lato, sans-serif; font-size: 16px; font-style: normal; font-variant: normal; font-weight: normal; letter-spacing: normal; line-height: 27.2px; orphans: auto; text-align: start; text-indent: 0px; text-transform: none; white-space: normal; widows: 1; word-spacing: 0px; -webkit-text-stroke-width: 0px; display: inline !important; float: none; background-color: #ffffff;\"\u003ea complete solution for event content-management. Beyond dragging an event to a different time\/day, you cannot change an event\u0027s name or other associated data. It is up to you to add this functionality through FullCalendar\u0027s event hooks\u003c\/span\u003e","status_code":"cre_sys_obt_not_itv","priority":"1","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"a complete solution for event content-management. Beyond dragging an event to a different time\/day, you cannot change an event\u0026#039;s name or other associated data. It is up to you to add this functionality through FullCalendar\u0026#039;s event hooks","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-10T00:00:00","actualStart":"2016-03-10T00:00:00","endTime":"","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"16","title_short":"a complete solution for event content-management. Beyond dragging an event to a..."},{"id":"announcement-71","fullname":"ExpertusONE  Admin","username":"admin","from_date":"2016-03-10","to_date":"2016-05-09","from_time":"08:00","to_time":"11:30","description":"FullCalendar is great for displaying events","status_code":"cre_sys_obt_not_itv","priority":"1","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":"2016-05-09","ori_end_time":"11:30","ori_start_time":"08:00","title":"FullCalendar is great for displaying events","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"08:00","start":"2016-03-10T08:00:00","actualStart":"2016-03-10T08:00:00","endTime":"11:30","end":"2016-05-09T11:30:00","actualEnd":"2016-05-09T11:30:00","allDay":true,"dayDifference":"60","title_short":"FullCalendar is great for displaying events"},{"id":"announcement-76","fullname":"ExpertusONE  Admin","username":"admin","from_date":"2016-03-11","to_date":"2016-05-09","from_time":"hh:mm","to_time":"hh:mm","description":"a complete solution for event content-management. Beyond dragging an event to a 6464645","status_code":"cre_sys_obt_not_itv","priority":"1","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"a complete solution for event content-management. Beyond dragging an event to a 6464645","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-11T00:00:00","actualStart":"2016-03-11T00:00:00","endTime":"","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"15","title_short":"a complete solution for event content-management. Beyond dragging an event to a..."},{"id":"announcement-78","fullname":"ExpertusONE  Admin","username":"admin","from_date":"2016-03-11","to_date":"2016-05-09","from_time":"hh:mm","to_time":"hh:mm","description":"\u003cspan style=\"color: #444444; font-family: Lato, sans-serif; font-size: 16px; font-style: normal; font-variant: normal; font-weight: normal; letter-spacing: normal; line-height: 27.2px; orphans: auto; text-align: start; text-indent: 0px; text-transform: none; white-space: normal; widows: 1; word-spacing: 0px; -webkit-text-stroke-width: 0px; display: inline !important; float: none; background-color: #ffffff;\"\u003eFullCalendar is great for displaying events, but it isn\u0027t a complete solution for event content-management. Beyond dragging an event to a different time\/day, you cannot change an event\u0027s name or other associated data. It is up to you to add this functionality through FullCalendar\u0027s event hooks.\u003c\/span\u003e","status_code":"cre_sys_obt_not_itv","priority":"1","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"FullCalendar is great for displaying events, but it isn\u0026#039;t a complete solution for event content-management. Beyond dragging an event to a different time\/day, you cannot change an event\u0026#039;s name or other associated data. It is up to you to add this functionality through FullCalendar\u0026#039;s event hooks.","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-11T00:00:00","actualStart":"2016-03-11T00:00:00","endTime":"","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"15","title_short":"FullCalendar is great for displaying events, but it isn\u0027t a complete solution f..."},{"id":"announcement-77","fullname":"ExpertusONE  Admin","username":"admin","from_date":"2016-03-11","to_date":"2016-05-09","from_time":"hh:mm","to_time":"hh:mm","description":"\u003cspan style=\"color: #444444; font-family: Lato, sans-serif; font-size: 16px; font-style: normal; font-variant: normal; font-weight: normal; letter-spacing: normal; line-height: 27.2px; orphans: auto; text-align: start; text-indent: 0px; text-transform: none; white-space: normal; widows: 1; word-spacing: 0px; -webkit-text-stroke-width: 0px; display: inline !important; float: none; background-color: #ffffff;\"\u003eFullCalendar is great for displaying events, but it isn\u0027t a complete solution for event content-management. Beyond dragging an event to a different time\/day, you cannot change an event\u0027s name or other associated data. It is up to you to add this functionality through FullCalendar\u0027s event hooks.\u003c\/span\u003e","status_code":"cre_sys_obt_not_itv","priority":"1","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"FullCalendar is great for displaying events, but it isn\u0026#039;t a complete solution for event content-management. Beyond dragging an event to a different time\/day, you cannot change an event\u0026#039;s name or other associated data. It is up to you to add this functionality through FullCalendar\u0026#039;s event hooks.","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-11T00:00:00","actualStart":"2016-03-11T00:00:00","endTime":"","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"15","title_short":"FullCalendar is great for displaying events, but it isn\u0027t a complete solution f..."},{"id":"announcement-84","fullname":"ExpertusONE  Admin","username":"admin","from_date":"2016-03-16","to_date":"2016-05-09","from_time":"hh:mm","to_time":"hh:mm","description":"AN5","status_code":"cre_sys_obt_not_itv","priority":"1","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"AN5","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-16T00:00:00","actualStart":"2016-03-16T00:00:00","endTime":"","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"10","title_short":"AN5"},{"id":"announcement-83","fullname":"ExpertusONE  Admin","username":"admin","from_date":"2016-03-16","to_date":"2016-05-09","from_time":"hh:mm","to_time":"hh:mm","description":"AN3","status_code":"cre_sys_obt_not_itv","priority":"1","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"AN3","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-16T00:00:00","actualStart":"2016-03-16T00:00:00","endTime":"","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"10","title_short":"AN3"},{"id":"announcement-82","fullname":"ExpertusONE  Admin","username":"admin","from_date":"2016-03-16","to_date":"2016-05-09","from_time":"hh:mm","to_time":"hh:mm","description":"AN3","status_code":"cre_sys_obt_not_itv","priority":"1","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"AN3","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-16T00:00:00","actualStart":"2016-03-16T00:00:00","endTime":"","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"10","title_short":"AN3"},{"id":"announcement-81","fullname":"ExpertusONE  Admin","username":"admin","from_date":"2016-03-16","to_date":"2016-03-30","from_time":"hh:mm","to_time":"00:45","description":"AN2","status_code":"cre_sys_obt_not_atv","priority":"2","language":"English","status":"Active","object_type":"announcement","ori_to_date":"2016-03-30","ori_end_time":"00:45","ori_start_time":"hh:mm","title":"AN2","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"00:00","start":"2016-03-16T00:00:00","actualStart":"2016-03-16T00:00:00","endTime":"00:45","end":"2016-03-30T00:45:00","actualEnd":"2016-03-30T00:45:00","allDay":true,"dayDifference":"14","title_short":"AN2"},{"id":"announcement-87","fullname":"simbu k","username":"simbu","from_date":"2016-03-16","to_date":"2016-03-31","from_time":"02:30","to_time":"10:00","description":"Two days Annoucemnt","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":"2016-03-31","ori_end_time":"10:00","ori_start_time":"02:30","title":"Two days Annoucemnt","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"02:30","start":"2016-03-16T02:30:00","actualStart":"2016-03-16T02:30:00","endTime":"10:00","end":"2016-03-31T10:00:00","actualEnd":"2016-03-31T10:00:00","allDay":true,"dayDifference":"15","title_short":"Two days Annoucemnt"},{"id":"announcement-94","fullname":"priya c","username":"priya","from_date":"2016-03-17","to_date":"2016-05-09","from_time":"hh:mm","to_time":"hh:mm","description":"trye","status_code":"cre_sys_obt_not_atv","priority":"2","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"trye","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-17T00:00:00","actualStart":"2016-03-17T00:00:00","endTime":"","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"9","title_short":"trye"},{"id":"announcement-98","fullname":"ExpertusONE  Admin","username":"admin","from_date":"2016-03-17","to_date":"2016-05-09","from_time":"hh:mm","to_time":"hh:mm","description":"rtretertertertert","status_code":"cre_sys_obt_not_itv","priority":"1","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"rtretertertertert","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-17T00:00:00","actualStart":"2016-03-17T00:00:00","endTime":"","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"9","title_short":"rtretertertertert"},{"id":"announcement-100","fullname":"ExpertusONE  Admin","username":"admin","from_date":"2016-03-18","to_date":"2016-03-29","from_time":"hh:mm","to_time":"hh:mm","description":"ewsfeaswr","status_code":"cre_sys_obt_not_itv","priority":"1","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":"2016-03-29","ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"ewsfeaswr","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-18T00:00:00","actualStart":"2016-03-18T00:00:00","endTime":"","end":"2016-03-29T23:59:59","actualEnd":"2016-03-29T23:59:59","allDay":true,"dayDifference":"11","title_short":"ewsfeaswr"},{"id":"announcement-93","fullname":"priya c","username":"priya","from_date":"2016-03-18","to_date":"2016-05-09","from_time":"hh:mm","to_time":"hh:mm","description":"Testing announcement in day view","status_code":"cre_sys_obt_not_atv","priority":"2","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"Testing announcement in day view","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-18T00:00:00","actualStart":"2016-03-18T00:00:00","endTime":"","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"8","title_short":"Testing announcement in day view"},{"id":"announcement-101","fullname":"an admin","username":"an_admin","from_date":"2016-03-19","to_date":"2016-05-09","from_time":"00:00","to_time":"hh:mm","description":"AN ANNOUNCEMENT -Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India.2.Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India.3.Bank of Baroda is an Indian state-owned banking and financial services company headquartered in","status_code":"cre_sys_obt_not_atv","priority":"2","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"00:00","title":"AN ANNOUNCEMENT -Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India.2.Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India.3.Bank of Baroda is an Indian state-owned banking and financial services company headquartered in","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"00:00","start":"2016-03-19T00:00:00","actualStart":"2016-03-19T00:00:00","endTime":"23:59","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"7","title_short":"AN ANNOUNCEMENT -Bank of Baroda is an Indian state-owned banking and financial..."},{"id":"announcement-103","fullname":"an admin","username":"an_admin","from_date":"2016-03-19","to_date":"2016-05-09","from_time":"00:00","to_time":"hh:mm","description":"AN ANNOUNCEMENT 4 - Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India.Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India.Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Va","status_code":"cre_sys_obt_not_atv","priority":"2","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"00:00","title":"AN ANNOUNCEMENT 4 - Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India.Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India.Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Va","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"00:00","start":"2016-03-19T00:00:00","actualStart":"2016-03-19T00:00:00","endTime":"23:59","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"7","title_short":"AN ANNOUNCEMENT 4 - Bank of Baroda is an Indian state-owned banking and financi..."},{"id":"announcement-104","fullname":"an admin","username":"an_admin","from_date":"2016-03-19","to_date":"2016-05-09","from_time":"00:00","to_time":"hh:mm","description":"AN ANNOUNCEMENT 5 - Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India","status_code":"cre_sys_obt_not_atv","priority":"2","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"00:00","title":"AN ANNOUNCEMENT 5 - Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"00:00","start":"2016-03-19T00:00:00","actualStart":"2016-03-19T00:00:00","endTime":"23:59","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"7","title_short":"AN ANNOUNCEMENT 5 - Bank of Baroda is an Indian state-owned banking and financi..."},{"id":"announcement-113","fullname":"vishal a","username":"vishal","from_date":"2016-03-19","to_date":"2016-03-31","from_time":"00:45","to_time":"03:00","description":"announcement to check space","status_code":"cre_sys_obt_not_atv","priority":"2","language":"English","status":"Active","object_type":"announcement","ori_to_date":"2016-03-31","ori_end_time":"03:00","ori_start_time":"00:45","title":"announcement to check space","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"00:45","start":"2016-03-19T00:45:00","actualStart":"2016-03-19T00:45:00","endTime":"03:00","end":"2016-03-31T03:00:00","actualEnd":"2016-03-31T03:00:00","allDay":true,"dayDifference":"12","title_short":"announcement to check space"},{"id":"announcement-102","fullname":"an admin","username":"an_admin","from_date":"2016-03-19","to_date":"2016-05-09","from_time":"14:00","to_time":"hh:mm","description":"AN ANNOUNCEMENT 3 - Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India.AN ANNOUNCEMENT 3 - Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India.AN ANNOUNCEMENT 3 - Bank of Baroda is an Indian state-owned banking and financ","status_code":"cre_sys_obt_not_atv","priority":"2","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"14:00","title":"AN ANNOUNCEMENT 3 - Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India.AN ANNOUNCEMENT 3 - Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India.AN ANNOUNCEMENT 3 - Bank of Baroda is an Indian state-owned banking and financ","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"14:00","start":"2016-03-19T14:00:00","actualStart":"2016-03-19T14:00:00","endTime":"23:59","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"7","title_short":"AN ANNOUNCEMENT 3 - Bank of Baroda is an Indian state-owned banking and financi..."},{"id":"announcement-6","fullname":"priya c","username":"priya","from_date":"2016-03-20","to_date":"2016-04-14","from_time":"00:45","to_time":"01:00","description":"\u003cdiv class=\"letter\"\u003e\n\u003cp\u003e\u003cstrong\u003eOur prudent spending through the last quarter has left us with a modest budget surplus. These monies have been earmarked for upgrades to our office machines.\u003c\/strong\u003e\u003c\/p\u003e\n\u003cp\u003e\u003cstrong\u003ePlease have your proposals on my desk by the end of this month so that I can evaluate our needs and make the purchases.\u003c\/strong\u003e\u003c\/p\u003e\n\u003c\/div\u003e","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":"2016-04-14","ori_end_time":"01:00","ori_start_time":"00:45","title":"\nOur prudent spending through the last quarter has left us with a modest budget surplus. These monies have been earmarked for upgrades to our office machines.\nPlease have your proposals on my desk by the end of this month so that I can evaluate our needs and make the purchases.\n","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"00:45","start":"2016-03-20T00:45:00","actualStart":"2016-03-20T00:45:00","endTime":"01:00","end":"2016-04-14T01:00:00","actualEnd":"2016-04-14T01:00:00","allDay":true,"dayDifference":"25","title_short":"Our prudent spending through the last quarter has left us with a modest budget..."},{"id":"announcement-132","fullname":"vishal a","username":"vishal","from_date":"2016-03-21","to_date":"2016-05-09","from_time":"hh:mm","to_time":"hh:mm","description":"annnnww","status_code":"cre_sys_obt_not_itv","priority":"2","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"annnnww","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-21T00:00:00","actualStart":"2016-03-21T00:00:00","endTime":"","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"5","title_short":"annnnww"},{"id":"announcement-133","fullname":"vishal a","username":"vishal","from_date":"2016-03-21","to_date":"2016-05-09","from_time":"00:00","to_time":"hh:mm","description":"\u003cspan style=\"color: #333333; font-family: \u0027Open Sans\u0027, Arial, Helvetica, sans-serif; line-height: 18px;\"\u003eExpertusONE delivers a superior LMS experience that enables adoption, speeds compliance and significantly improves training effectiveness \u2212 for your learners, partners and customers alike! Ranked the #1 LMS for 2013 and 2014, ExpertusONE\u2019s simplified architecture leverages the intuitive design principles we\u2019ve come to expect from everyday modern technology. So you get: - See more at: http:\/\/www.expertus.com\/expertus-one\/#sthash.NtOeBRuc.dpuf\u003c\/span\u003e","status_code":"cre_sys_obt_not_itv","priority":"2","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"00:00","title":"ExpertusONE delivers a superior LMS experience that enables adoption, speeds compliance and significantly improves training effectiveness \u0026minus; for your learners, partners and customers alike! Ranked the #1 LMS for 2013 and 2014, ExpertusONE\u0026rsquo;s simplified architecture leverages the intuitive design principles we\u0026rsquo;ve come to expect from everyday modern technology. So you get: - See more at: http:\/\/www.expertus.com\/expertus-one\/#sthash.NtOeBRuc.dpuf","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"00:00","start":"2016-03-21T00:00:00","actualStart":"2016-03-21T00:00:00","endTime":"23:59","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"5","title_short":"ExpertusONE delivers a superior LMS experience that enables adoption, speeds..."},{"id":"announcement-92","fullname":"priya c","username":"priya","from_date":"2016-03-21","to_date":"2016-05-09","from_time":"hh:mm","to_time":"hh:mm","description":"tr","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"tr","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-21T00:00:00","actualStart":"2016-03-21T00:00:00","endTime":"","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"5","title_short":"tr"},{"id":"announcement-70","fullname":"yogaraja k","username":"yogaraja","from_date":"2016-03-21","to_date":"2016-05-11","from_time":"hh:mm","to_time":"hh:mm","description":"20-03-16 to 10-05-16","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":"2016-05-11","ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"20-03-16 to 10-05-16","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-21T00:00:00","actualStart":"2016-03-21T00:00:00","endTime":"","end":"2016-05-11T23:59:59","actualEnd":"2016-05-11T23:59:59","allDay":true,"dayDifference":"51","title_short":"20-03-16 to 10-05-16"},{"id":"announcement-139","fullname":"an admin","username":"an_admin","from_date":"2016-03-21","to_date":"2016-05-09","from_time":"23:30","to_time":"hh:mm","description":"AN4 ANNOUNCEMENT - The country is classified as a newly industrialised country, one of the G-20 major economies, a member of BRICS and a developing economy with an average growth rate of approximately 7% over the last two decades. Maharashtra is the wealthiest Indian state and has an annual GDP of US$220 billion, nearly equal to that of Pakistan or Portugal, and accounts for 12% of the Indian GDP followed by yogaraja","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"23:30","title":"AN4 ANNOUNCEMENT - The country is classified as a newly industrialised country, one of the G-20 major economies, a member of BRICS and a developing economy with an average growth rate of approximately 7% over the last two decades. Maharashtra is the wealthiest Indian state and has an annual GDP of US$220 billion, nearly equal to that of Pakistan or Portugal, and accounts for 12% of the Indian GDP followed by yogaraja","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"23:30","start":"2016-03-21T23:30:00","actualStart":"2016-03-21T23:30:00","endTime":"23:59","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"5","title_short":"AN4 ANNOUNCEMENT - The country is classified as a newly industrialised country,..."},{"id":"announcement-118","fullname":"vishal a","username":"vishal","from_date":"2016-03-23","to_date":"2016-05-09","from_time":"00:00","to_time":"hh:mm","description":"announcement333","status_code":"cre_sys_obt_not_atv","priority":"2","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"00:00","title":"announcement333","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"00:00","start":"2016-03-23T00:00:00","actualStart":"2016-03-23T00:00:00","endTime":"23:59","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"3","title_short":"announcement333"},{"id":"announcement-106","fullname":"vishal a","username":"vishal","from_date":"2016-03-23","to_date":"2016-03-31","from_time":"00:30","to_time":"00:30","description":"Expertus was founded in the Silicon Valley in 1998. Initially, the company offered services to support global enterprises with implementation, support and administration of learning management systems (LMS). In 2010, Expertus developed and released what now serves as the company\u0027s flagship product, the ExpertusONE LMS, a cloud-based learning management system built on a software-as-a-service platform. The ExpertusONE LMS combines\u0026nbsp;formal,\u0026nbsp;informal,\u0026nbsp;social\u0026nbsp;and\u0026nbsp;collaborative learning\u0026nbsp;for employee.","status_code":"cre_sys_obt_not_atv","priority":"2","language":"English","status":"Active","object_type":"announcement","ori_to_date":"2016-03-31","ori_end_time":"00:30","ori_start_time":"00:30","title":"","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"00:30","start":"2016-03-23T00:30:00","actualStart":"2016-03-23T00:30:00","endTime":"00:30","end":"2016-03-31T00:30:00","actualEnd":"2016-03-31T00:30:00","allDay":true,"dayDifference":"8","title_short":"Expertus was founded in the Silicon Valley in 1998. Initially, the company of..."},{"id":"announcement-123","fullname":"vishal a","username":"vishal","from_date":"2016-03-24","to_date":"2016-05-09","from_time":"hh:mm","to_time":"hh:mm","description":"check","status_code":"cre_sys_obt_not_itv","priority":"2","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"check","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-24T00:00:00","actualStart":"2016-03-24T00:00:00","endTime":"","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"2","title_short":"check"},{"id":"announcement-1","fullname":"ExpertusONE  Admin","username":"admin","from_date":"2016-03-25","to_date":"2016-03-27","from_time":"hh:mm","to_time":"hh:mm","description":"Ann1","status_code":"cre_sys_obt_not_itv","priority":"1","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":"2016-03-27","ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"Ann1","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-25T00:00:00","actualStart":"2016-03-25T00:00:00","endTime":"","end":"2016-03-27T23:59:59","actualEnd":"2016-03-27T23:59:59","allDay":true,"dayDifference":"2","title_short":"Ann1"},{"id":"announcement-165","fullname":"ratheesh s","username":"ratheeshs","from_date":"2016-03-25","to_date":"2016-05-09","from_time":"00:30","to_time":"hh:mm","description":"announce ratheesh","status_code":"cre_sys_obt_not_atv","priority":"2","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"00:30","title":"announce ratheesh","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"00:30","start":"2016-03-25T00:30:00","actualStart":"2016-03-25T00:30:00","endTime":"23:59","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":false,"dayDifference":"1","title_short":"announce ratheesh"},{"id":"announcement-2","fullname":"ExpertusONE  Admin","username":"admin","from_date":"2016-03-25","to_date":"2016-03-27","from_time":"01:00","to_time":"06:00","description":"AN2","status_code":"cre_sys_obt_not_itv","priority":"1","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":"2016-03-27","ori_end_time":"06:00","ori_start_time":"01:00","title":"AN2","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"01:00","start":"2016-03-25T01:00:00","actualStart":"2016-03-25T01:00:00","endTime":"06:00","end":"2016-03-27T06:00:00","actualEnd":"2016-03-27T06:00:00","allDay":true,"dayDifference":"2","title_short":"AN2"},{"id":"announcement-156","fullname":"vishal 2","username":"vishal2","from_date":"2016-03-26","to_date":"2016-05-09","from_time":"00:30","to_time":"hh:mm","description":"annou speed","status_code":"cre_sys_obt_not_itv","priority":"2","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"00:30","title":"annou speed","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"00:30","start":"2016-03-26T00:30:00","actualStart":"2016-03-26T00:30:00","endTime":"23:59","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":null,"title_short":"annou speed"},{"title":"IE9- Class23","editable":1,"deletable":1,"dataType":null,"id":"lrn_cls_dty_ilt-68","class_id":"ZZ34ZZdb3719568b78864e","course_id":"ZZ34ZZ07f1c94944562ed0","code":"IE9- Class23","type":"lrn_cls_dty_ilt","locationname":"chennai","languagename":"English","location_id":"2","delivery_type_code":"lrn_cls_dty_ilt","session_title":"IE9- Class23","scheduled_duration":"120","instructor":"","allDay":false,"status":"Shown in Catalog","timezone":"Asia\/Kolkata","start":"2016-03-27T02:00","startTime":"02:00","end":"2016-03-27T04:00","endTime":"04:00"},{"title":"ilt class234","editable":1,"deletable":1,"dataType":null,"id":"lrn_cls_dty_ilt-152","class_id":"ZZ34ZZab835b5aa05ca8ba","course_id":"ZZ34ZZ39ca79f732333a6c","code":"ilt class234","type":"lrn_cls_dty_ilt","locationname":"Dharmapuri6","languagename":"English","location_id":"13","delivery_type_code":"lrn_cls_dty_ilt","session_title":"ilt class","scheduled_duration":"195","instructor":"","allDay":false,"status":"Hidden in Catalog","timezone":"Asia\/Kolkata","start":"2016-03-28T01:00","startTime":"01:00","end":"2016-03-28T04:15","endTime":"04:15"},{"id":"announcement-57","fullname":"ExpertusONE  Admin","username":"admin","from_date":"2016-03-29","to_date":"2016-04-01","from_time":"hh:mm","to_time":"hh:mm","description":"\u003cul\u003e\n\u003cli\u003eMaintains a pay plan by conducting periodic pay surveys; scheduling and conducting job evaluations; preparing pay budgets; monitoring and scheduling individual pay actions; recommending, planning, and implementing pay structure revisions.\u003c\/li\u003e\n\u003cli\u003eEnsures planning, monitoring, and appraisal of employee work results by training managers to coach and discipline employees; scheduling management conferences with employees; hearing and resolving employee grievances; counseling employees and supervisors.\u003c\/li\u003e\n\u003c\/ul\u003e","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":"2016-04-01","ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"\nMaintains a pay plan by conducting periodic pay surveys; scheduling and conducting job evaluations; preparing pay budgets; monitoring and scheduling individual pay actions; recommending, planning, and implementing pay structure revisions.\nEnsures planning, monitoring, and appraisal of employee work results by training managers to coach and discipline employees; scheduling management conferences with employees; hearing and resolving employee grievances; counseling employees and supervisors.\n","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-29T00:00:00","actualStart":"2016-03-29T00:00:00","endTime":"","end":"2016-04-01T23:59:59","actualEnd":"2016-04-01T23:59:59","allDay":true,"dayDifference":"3","title_short":"Maintains a pay plan by conducting periodic pay surveys; scheduling and conduc..."},{"title":"class ilt 678","editable":1,"deletable":1,"dataType":null,"id":"lrn_cls_dty_ilt-153","class_id":"ZZ34ZZ38c7afedfa5e051d","course_id":"ZZ34ZZ286cd0b8c732e9b4","code":"class ilt 678","type":"lrn_cls_dty_ilt","locationname":"Dharmapuri10","languagename":"English","location_id":"17","delivery_type_code":"lrn_cls_dty_ilt","session_title":"ilt class","scheduled_duration":"255","instructor":"AN INSTRUCTOR2,an instructor,pallavis ins","allDay":false,"status":"Hidden in Catalog","timezone":"Asia\/Kolkata","start":"2016-03-29T01:00","startTime":"01:00","end":"2016-03-29T05:15","endTime":"05:15"},{"id":"announcement-99","fullname":"simbu k","username":"simbu","from_date":"2016-03-29","to_date":"2016-04-21","from_time":"04:30","to_time":"hh:mm","description":"\u003cstrong\u003eAnnouncement\u0026nbsp;Cases: - \u003c\/strong\u003e\u003cspan style=\"font-family: verdana,sans-serif;\"\u003efrom date \/\u0026nbsp;to date\u0026nbsp;with from_time (All day with range with time)\u003c\/span\u003e","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":"2016-04-21","ori_end_time":"hh:mm","ori_start_time":"04:30","title":"","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"04:30","start":"2016-03-29T04:30:00","actualStart":"2016-03-29T04:30:00","endTime":"23:59","end":"2016-04-21T23:59:59","actualEnd":"2016-04-21T23:59:59","allDay":true,"dayDifference":"23","title_short":"AnnouncementCases: - from date \/to datewith from_time (All day with range wit..."},{"id":"announcement-68","fullname":"yogaraja k","username":"yogaraja","from_date":"2016-03-30","to_date":"2016-04-02","from_time":"hh:mm","to_time":"hh:mm","description":"30-03-16 to 02-04-16","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":"2016-04-02","ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"30-03-16 to 02-04-16","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-30T00:00:00","actualStart":"2016-03-30T00:00:00","endTime":"","end":"2016-04-02T23:59:59","actualEnd":"2016-04-02T23:59:59","allDay":true,"dayDifference":"3","title_short":"30-03-16 to 02-04-16"},{"title":"#@$#^#%$%^%$^%$^%$^$%^%$^$%^$%^%*\u0026amp;)_+)_(*^%$#@!","editable":1,"deletable":1,"dataType":null,"id":"lrn_cls_dty_ilt-67","class_id":"ZZ34ZZb513364797b1bee1","course_id":"ZZ34ZZ86c5fee3c135cd7d","code":"#@$#^#%$%^%$^%$^%$^$%^%$^$%^$%^%*\u0026)_+)_(*^%$#@!","type":"lrn_cls_dty_ilt","locationname":"chennai","languagename":"English","location_id":"2","delivery_type_code":"lrn_cls_dty_ilt","session_title":"598636\u0027;\u0027l;\u0027;\u0027l;435twertrte","scheduled_duration":"345","instructor":"","allDay":false,"status":"Shown in Catalog","timezone":"Asia\/Kolkata","start":"2016-03-30T00:00","startTime":"00:00","end":"2016-03-30T00:30","endTime":"00:30"},{"id":"announcement-61","fullname":"ExpertusONE  Admin","username":"admin","from_date":"2016-03-30","to_date":"2016-03-31","from_time":"hh:mm","to_time":"hh:mm","description":"A job description defines the duties of the position and the skills and knowledge necessary for the job. Job titles are kept generic, and do not fully explain what the position-holder is hired to do. The basic job of tenure track faculty is teaching, research, and service; many people think that they do not need a fuller description than that. But for other academic positions, especially non-ranked, non-tenurable academic positions like Research Analyst or Extension Assistant, it is very","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":"2016-03-31","ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"A job description defines the duties of the position and the skills and knowledge necessary for the job. Job titles are kept generic, and do not fully explain what the position-holder is hired to do. The basic job of tenure track faculty is teaching, research, and service; many people think that they do not need a fuller description than that. But for other academic positions, especially non-ranked, non-tenurable academic positions like Research Analyst or Extension Assistant, it is very","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-30T00:00:00","actualStart":"2016-03-30T00:00:00","endTime":"","end":"2016-03-31T23:59:59","actualEnd":"2016-03-31T23:59:59","allDay":true,"dayDifference":"1","title_short":"A job description defines the duties of the position and the skills and knowled..."},{"title":"ilt check1","editable":1,"deletable":1,"dataType":null,"id":"event_regend-163","start":"2016-03-31T00:00:00","delivery_type_code":"lrn_cls_dty_ilt","class_type":"Classroom","code":"ilt check1ilt check1","startTime":"All Day","allDay":true,"class_id":"ZZ34ZZeea6f56756c4e5f5","course_id":"ZZ34ZZ3b7b245dab7c25fd","type":"event_regend","locationname":"Dharmapuri6","languagename":"English"},{"id":"announcement-69","fullname":"yogaraja k","username":"yogaraja","from_date":"2016-03-31","to_date":"2016-04-30","from_time":"hh:mm","to_time":"hh:mm","description":"31-03-16 to 30-04-16","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":"2016-04-30","ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"31-03-16 to 30-04-16","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-31T00:00:00","actualStart":"2016-03-31T00:00:00","endTime":"","end":"2016-04-30T23:59:59","actualEnd":"2016-04-30T23:59:59","allDay":true,"dayDifference":"30","title_short":"31-03-16 to 30-04-16"},{"title":"Sample VC 1","editable":1,"deletable":1,"dataType":null,"id":"event_regend-67","start":"2016-03-31T00:00:00","delivery_type_code":"lrn_cls_dty_vcl","class_type":"Virtual Class","code":"Sample VC 1","startTime":"All Day","allDay":true,"class_id":"ZZ34ZZ7fcb26d813c1bb4c","course_id":"ZZ34ZZff2383f60cab79d2","type":"event_regend","locationname":null,"languagename":"English"},{"title":"ilt check","editable":1,"deletable":1,"dataType":null,"id":"event_regend-162","start":"2016-03-31T00:00:00","delivery_type_code":"lrn_cls_dty_ilt","class_type":"Classroom","code":"ilt checkilt check","startTime":"All Day","allDay":true,"class_id":"ZZ34ZZ935e6b9d80f41234","course_id":"ZZ34ZZ59a0d1527108a6a6","type":"event_regend","locationname":"Dharmapuri9","languagename":"English"},{"title":"ilt classroom","editable":1,"deletable":1,"dataType":null,"id":"lrn_cls_dty_ilt-144","class_id":"ZZ34ZZb846e1e565b5dfc0","course_id":"ZZ34ZZb59dd3e7ed6fd500","code":"ilt classroom","type":"lrn_cls_dty_ilt","locationname":"Dharmapuri5","languagename":"English","location_id":"12","delivery_type_code":"lrn_cls_dty_ilt","session_title":"class for ILT","scheduled_duration":"330","instructor":"vijay 1","allDay":false,"status":"Hidden in Catalog","timezone":"Asia\/Kolkata","start":"2016-03-31T01:15","startTime":"01:15","end":"2016-03-31T06:45","endTime":"06:45"},{"title":"dqe","editable":1,"deletable":1,"dataType":null,"id":"lrn_cls_dty_ilt-49","class_id":"ZZ34ZZb2c8c58dcd613a33","course_id":"ZZ34ZZ3dfdfe01c46a13a9","code":"sd","type":"lrn_cls_dty_ilt","locationname":"chennai","languagename":"English","location_id":"2","delivery_type_code":"lrn_cls_dty_ilt","session_title":"qefsdf","scheduled_duration":"15","instructor":"vijay 1","allDay":false,"status":"Shown in Catalog","timezone":"Asia\/Kolkata","start":"2016-04-01T00:00","startTime":"00:00","end":"2016-04-01T00:15","endTime":"00:15"},{"title":"Sample VC 1","editable":1,"deletable":1,"dataType":null,"id":"lrn_cls_dty_vcl-60","class_id":"ZZ34ZZ7fcb26d813c1bb4c","course_id":"ZZ34ZZff2383f60cab79d2","code":"Sample VC 1","type":"lrn_cls_dty_vcl","locationname":null,"languagename":"English","location_id":null,"delivery_type_code":"lrn_cls_dty_vcl","session_title":"Sample VC 1 - EXP","scheduled_duration":"210","instructor":"","allDay":false,"status":"Hidden in Catalog","timezone":"Asia\/Kolkata","start":"2016-04-01T00:00","startTime":"00:00","end":"2016-04-01T01:30","endTime":"01:30"},{"title":"ilt 10ilt 10ilt 10ilt 10ilt 10ilt 10ilt 10ilt 10ilt 10ilt 10ilt 10ilt 10ilt 10ilt 10ilt 10ilt 10ilt","editable":1,"deletable":1,"dataType":null,"id":"event_regend-146","start":"2016-04-01T00:00:00","delivery_type_code":"lrn_cls_dty_ilt","class_type":"Classroom","code":"ilt 10","startTime":"All Day","allDay":true,"class_id":"ZZ34ZZb96d0428bf502dbe","course_id":"ZZ34ZZf42b84174d70fb95","type":"event_regend","locationname":"Dharmapuri9","languagename":"English"},{"id":"announcement-124","fullname":"simbu k","username":"simbu","from_date":"2016-04-01","to_date":"2016-04-01","from_time":"00:00","to_time":"00:30","description":"SIM-1-ANNM","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":"2016-04-01","ori_end_time":"00:30","ori_start_time":"00:00","title":"SIM-1-ANNM","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"00:00","start":"2016-04-01T00:00:00","actualStart":"2016-04-01T00:00:00","endTime":"00:30","end":"2016-04-01T00:30:00","actualEnd":"2016-04-01T00:30:00","allDay":false,"dayDifference":"0","title_short":"SIM-1-ANNM"},{"title":"AN ILT TEST COURSE - 4","editable":1,"deletable":1,"dataType":null,"id":"lrn_cls_dty_ilt-20","class_id":"ZZ34ZZ051c1c20ea6190f9","course_id":"ZZ34ZZbde6fa9df54eefe7","code":"AN ILT TEST COURSE - 41","type":"lrn_cls_dty_ilt","locationname":"THAMARACHERRY","languagename":"English","location_id":"4","delivery_type_code":"lrn_cls_dty_ilt","session_title":"AN ILT TEST COURSE - 4 - SESSION","scheduled_duration":"15","instructor":"vijay 1","allDay":false,"status":"Hidden in Catalog","timezone":"Asia\/Kolkata","start":"2016-04-01T18:15","startTime":"18:15","end":"2016-04-01T18:30","endTime":"18:30"},{"title":"Classs \u0026#039;!@#$%^\u0026amp;*()Room Course Test1 Classs R\u0026#039;$%oom Course T","editable":1,"deletable":1,"dataType":null,"id":"lrn_cls_dty_ilt-38","class_id":"ZZ34ZZf6618fe7e457b0b7","course_id":"ZZ34ZZb2c8c58dcd613a33","code":"Classs \u0027!@#$%^\u0026*()Room Course Test1 Classs R\u0027$%oom Course T","type":"lrn_cls_dty_ilt","locationname":"THAMARACHERRY","languagename":"English","location_id":"4","delivery_type_code":"lrn_cls_dty_ilt","session_title":"Session Test 3","scheduled_duration":"285","instructor":"vishal a","allDay":false,"status":"Hidden in Catalog","timezone":"Asia\/Kolkata","start":"2016-04-02T00:00","startTime":"00:00","end":"2016-04-02T01:45","endTime":"01:45"},{"title":"Testing course8","editable":1,"deletable":1,"dataType":null,"id":"lrn_cls_dty_vcl-128","class_id":"ZZ34ZZba632cd47dd1211d","course_id":"ZZ34ZZ4e786a2cb8dda45c","code":"Testing course8","type":"lrn_cls_dty_vcl","locationname":null,"languagename":"English","location_id":null,"delivery_type_code":"lrn_cls_dty_vcl","session_title":"Testing course8","scheduled_duration":"15","instructor":"","allDay":false,"status":"Shown in Catalog","timezone":"Asia\/Kolkata","start":"2016-04-02T23:30","startTime":"23:30","end":"2016-04-02T23:45","endTime":"23:45"},{"id":"announcement-126","fullname":"simbu k","username":"simbu","from_date":"2016-04-03","to_date":"2016-05-09","from_time":"hh:mm","to_time":"hh:mm","description":"SIM-34#annnnnn","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"SIM-34#annnnnn","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-04-03T00:00:00","actualStart":"2016-04-03T00:00:00","endTime":"","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"7","title_short":"SIM-34#annnnnn"},{"id":"announcement-130","fullname":"simbu k","username":"simbu","from_date":"2016-04-03","to_date":"2016-05-09","from_time":"hh:mm","to_time":"hh:mm","description":"yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-04-03T00:00:00","actualStart":"2016-04-03T00:00:00","endTime":"","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"7","title_short":"yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy"},{"id":"announcement-129","fullname":"simbu k","username":"simbu","from_date":"2016-04-05","to_date":"2016-05-09","from_time":"hh:mm","to_time":"hh:mm","description":"llllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"llllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-04-05T00:00:00","actualStart":"2016-04-05T00:00:00","endTime":"","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"9","title_short":"llllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll"},{"title":"sdfdsfdsfdsf","editable":1,"deletable":1,"dataType":null,"id":"lrn_cls_dty_ilt-70","class_id":"ZZ34ZZ04fe59a9b628bc96","course_id":"ZZ34ZZf5a1976ef41a08a7","code":"dsfdsfds","type":"lrn_cls_dty_ilt","locationname":"miamisburg","languagename":"English","location_id":"1","delivery_type_code":"lrn_cls_dty_ilt","session_title":"sdfdsfdsfds","scheduled_duration":"45","instructor":"","allDay":false,"status":"Shown in Catalog","timezone":"America\/New_York","start":"2016-04-05T00:15","startTime":"00:15","end":"2016-04-05T01:00","endTime":"01:00"},{"title":"pallavi class 3","editable":1,"deletable":1,"dataType":null,"id":"event_regend-18","start":"2016-04-07T00:00:00","delivery_type_code":"lrn_cls_dty_ilt","class_type":"Classroom","code":"pallavi class 3","startTime":"All Day","allDay":true,"class_id":"ZZ34ZZ70f956d892125eb2","course_id":"ZZ34ZZ1807e1f16ad858eb","type":"event_regend","locationname":"chennai","languagename":"English"},{"id":"announcement-128","fullname":"simbu k","username":"simbu","from_date":"2016-04-08","to_date":"2016-05-09","from_time":"hh:mm","to_time":"hh:mm","description":"oooooooooooooooooo00000000000000000000000000","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"oooooooooooooooooo00000000000000000000000000","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-04-08T00:00:00","actualStart":"2016-04-08T00:00:00","endTime":"","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"12","title_short":"oooooooooooooooooo00000000000000000000000000"},{"id":"announcement-127","fullname":"simbu k","username":"simbu","from_date":"2016-04-09","to_date":"2016-05-09","from_time":"hh:mm","to_time":"hh:mm","description":"vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-04-09T00:00:00","actualStart":"2016-04-09T00:00:00","endTime":"","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"13","title_short":"vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv"},{"title":"Testing course9","editable":1,"deletable":1,"dataType":null,"id":"event_regend-135","start":"2016-04-09T00:00:00","delivery_type_code":"lrn_cls_dty_vcl","class_type":"Virtual Class","code":"Testing course9","startTime":"All Day","allDay":true,"class_id":"ZZ34ZZe9e65e2af81f7867","course_id":"ZZ34ZZf3eebefcbd76bf35","type":"event_regend","locationname":null,"languagename":"English"},{"title":"Testing course 6","editable":1,"deletable":1,"dataType":null,"id":"event_regend-130","start":"2016-04-09T00:00:00","delivery_type_code":"lrn_cls_dty_vcl","class_type":"Virtual Class","code":"Testing course 6","startTime":"All Day","allDay":true,"class_id":"ZZ34ZZa432c7be1bd014d4","course_id":"ZZ34ZZ0a2354fd0910a23b","type":"event_regend","locationname":null,"languagename":"English"},{"title":"Testing course 7","editable":1,"deletable":1,"dataType":null,"id":"event_regend-131","start":"2016-04-09T00:00:00","delivery_type_code":"lrn_cls_dty_vcl","class_type":"Virtual Class","code":"Testing course 7","startTime":"All Day","allDay":true,"class_id":"ZZ34ZZf847dd00f2a00c38","course_id":"ZZ34ZZb508e7d72e42735f","type":"event_regend","locationname":null,"languagename":"English"},{"id":"announcement-143","fullname":"an admin","username":"an_admin","from_date":"2016-04-09","to_date":"2016-04-09","from_time":"23:30","to_time":"23:45","description":"AN3 ANNOUNCEMENT - The country is classified as a newly industrialised country, one of the G-20 major economies, a member of BRICS and a developing \n\neconomy with an average growth rate of approximately 7% over the last two decades. Maharashtra is the wealthiest Indian state and has \n\nan annual GDP of US$220 billion, nearly equal to that of Pakistan or Portugal, and accounts for 12% of the Indian GDP followed by the \n\nstates of Tamil Nadu (US$140 billion) and Uttar Pradesh","status_code":"cre_sys_obt_not_atv","priority":"2","language":"English","status":"Active","object_type":"announcement","ori_to_date":"2016-04-09","ori_end_time":"23:45","ori_start_time":"23:30","title":"AN3 ANNOUNCEMENT - The country is classified as a newly industrialised country, one of the G-20 major economies, a member of BRICS and a developing \n\neconomy with an average growth rate of approximately 7% over the last two decades. Maharashtra is the wealthiest Indian state and has \n\nan annual GDP of US$220 billion, nearly equal to that of Pakistan or Portugal, and accounts for 12% of the Indian GDP followed by the \n\nstates of Tamil Nadu (US$140 billion) and Uttar Pradesh","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"23:30","start":"2016-04-09T23:30:00","actualStart":"2016-04-09T23:30:00","endTime":"23:45","end":"2016-04-09T23:45:00","actualEnd":"2016-04-09T23:45:00","allDay":false,"dayDifference":"0","title_short":"AN3 ANNOUNCEMENT - The country is classified as a newly industrialised country,..."},{"title":"Cls","editable":1,"deletable":1,"dataType":null,"id":"event_regend-3","start":"2016-04-12T00:00:00","delivery_type_code":"lrn_cls_dty_ilt","class_type":"Classroom","code":"cls","startTime":"All Day","allDay":true,"class_id":"ZZ34ZZ5a234ba3e2788956","course_id":"ZZ34ZZ5a234ba3e2788956","type":"event_regend","locationname":"miamisburg","languagename":"English"},{"title":"pallavi class 4","editable":1,"deletable":1,"dataType":null,"id":"event_regend-21","start":"2016-04-15T00:00:00","delivery_type_code":"lrn_cls_dty_ilt","class_type":"Classroom","code":"pallavi class 4","startTime":"All Day","allDay":true,"class_id":"ZZ34ZZf1ff5823e10f76e7","course_id":"ZZ34ZZ662da03b569c9990","type":"event_regend","locationname":"chennai","languagename":"English"},{"id":"announcement-16","fullname":"ps 1","username":"sp1","from_date":"2016-04-19","to_date":"2016-04-19","from_time":"hh:mm","to_time":"hh:mm","description":"\u003cspan style=\"color: #0000ff;\"\u003e\u003cstrong\u003epallavi announcement\u0026nbsp;pallavi announcement\u0026nbsp;pallavi announcement\u0026nbsp;pallavi announcement\u0026nbsp;pallavi announcement\u0026nbsp;pallavi announcement\u0026nbsp;pallavi anno\u003c\/strong\u003e\u003c\/span\u003eu\u003cspan style=\"text-decoration: underline; color: #ff0000;\"\u003e\u003cspan style=\"text-decoration: underline;\"\u003encement\u0026nbsp;pallavi announcement\u0026nbsp;pallavi announcement\u0026nbsp;pallavi announcement\u0026nbsp;pallavi announcement\u0026nbsp;pallavi announcement\u0026nbsp;pallavi announcement\u0026nbsp;pallavi announcement\u0026nbsp;pallavi announcement\u0026nbsp;pallavi announcement\u0026nbsp;pallavi announcement\u0026nbsp;pallavi announcement\u0026nbsp;pallavi announcement\u0026nbsp;pallavi announcement\u0026nbsp;pallavi announcement\u0026nbsp;pallavi announcement\u0026nbsp;pallavi announcement\u0026nbsp;pallavi announcem\u003c\/span\u003e\u003c\/span\u003e","status_code":"cre_sys_obt_not_itv","priority":"2","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":"2016-04-19","ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-04-19T00:00:00","actualStart":"2016-04-19T00:00:00","endTime":"","end":"2016-04-19T23:59:59","actualEnd":"2016-04-19T23:59:59","allDay":true,"dayDifference":"0","title_short":"pallavi announcementpallavi announcementpallavi announcementpallavi announcem..."},{"id":"announcement-97","fullname":"simbu k","username":"simbu","from_date":"2016-04-19","to_date":"2016-05-09","from_time":"hh:mm","to_time":"hh:mm","description":"\u003cdiv class=\"gmail_default\" style=\"font-family: verdana,sans-serif;\"\u003e\u003cstrong\u003eAnnouncement\u0026nbsp;Cases: - \u003c\/strong\u003e\u003cspan style=\"font-family: verdana,sans-serif;\"\u003efrom date \u0026nbsp;(All day with out range)\u003c\/span\u003e\u003c\/div\u003e","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-04-19T00:00:00","actualStart":"2016-04-19T00:00:00","endTime":"","end":"2016-05-09T23:59:59","actualEnd":"2016-05-09T23:59:59","allDay":true,"dayDifference":"23","title_short":"AnnouncementCases: - from date (All day with out ran..."},{"title":"pallavi class 3","editable":1,"deletable":1,"dataType":null,"id":"lrn_cls_dty_ilt-16","class_id":"ZZ34ZZ70f956d892125eb2","course_id":"ZZ34ZZ1807e1f16ad858eb","code":"pallavi class 3","type":"lrn_cls_dty_ilt","locationname":"chennai","languagename":"English","location_id":"2","delivery_type_code":"lrn_cls_dty_ilt","session_title":"terter","scheduled_duration":"30","instructor":"vijay 1,an instructor,priya c,vishal a,pallavis ins","allDay":false,"status":"Hidden in Catalog","timezone":"Asia\/Kolkata","start":"2016-04-20T01:00","startTime":"01:00","end":"2016-04-20T01:15","endTime":"01:15"},{"title":"pallavi class 3","editable":1,"deletable":1,"dataType":null,"id":"lrn_cls_dty_ilt-15","class_id":"ZZ34ZZ70f956d892125eb2","course_id":"ZZ34ZZ1807e1f16ad858eb","code":"pallavi class 3","type":"lrn_cls_dty_ilt","locationname":"chennai","languagename":"English","location_id":"2","delivery_type_code":"lrn_cls_dty_ilt","session_title":"tretretretrtrerrrrrrrrrrrrrrrrrt","scheduled_duration":"30","instructor":"an instructor,priya c,vijay 1,vishal a,pallavis ins","allDay":false,"status":"Hidden in Catalog","timezone":"Asia\/Kolkata","start":"2016-04-23T01:30","startTime":"01:30","end":"2016-04-23T01:45","endTime":"01:45"},{"title":"pallavi class 4","editable":1,"deletable":1,"dataType":null,"id":"lrn_cls_dty_ilt-19","class_id":"ZZ34ZZf1ff5823e10f76e7","course_id":"ZZ34ZZ662da03b569c9990","code":"pallavi class 4","type":"lrn_cls_dty_ilt","locationname":"chennai","languagename":"English","location_id":"2","delivery_type_code":"lrn_cls_dty_ilt","session_title":"mmll","scheduled_duration":"15","instructor":"pallavis ins","allDay":false,"status":"Hidden in Catalog","timezone":"Asia\/Kolkata","start":"2016-04-25T00:00","startTime":"00:00","end":"2016-04-25T00:15","endTime":"00:15"}]';
	//return drupal_json_encode($sorteddata); 

	$outputarr = array();
	$evtCntPerDay = 0;
	
	$evtLimit = 0;
	$view = $_REQUEST["view"];
	if($view == "month")
		$evtLimit = 4;
	elseif($view == "basicWeek")
		$evtLimit = 35;
	elseif($view == "agendaDay")
		$evtLimit = 300;
	else
		$evtLimit = 10;
		
	$requestFromNarrowCal = $_REQUEST["narrowcaldaywise"];
	$requestType	      = $_REQUEST["type"];
	$reqNarCalShrt = $_REQUEST["narrowcaldaywiseshort"];
	
	//override evtLimit
	if($requestFromNarrowCal == "true")
		$evtLimit = -1;
	
	if($reqNarCalShrt == "true")
		$evtLimit = 7;
		
	if($requestType == "more") {
		$skipDate = date('m-d-Y', strtotime($_REQUEST['startDate']));
	}
	
	for($i = 0; $i < count($sorteddata); $i++)
	{
		$sorteddata[$i]->title = sanitize_data($sorteddata[$i]->title);
		$dt1 =  date('m-d-Y',strtotime($sorteddata[$i]->start));
		$dt2 =  date('m-d-Y',strtotime($sorteddata[$i+1]->start));
		expDebug::dPrint("suresh dt1=". $dt1."==dt2=".$dt2."::count=".$evtCntPerDay."::evtView::".$view,5);
		if($dt1 == $dt2 &&  $evtCntPerDay != 0 && $evtCntPerDay == $evtLimit)
		{
			continue;
		}
		$outputarr[] = $sorteddata[$i];
		if($dt1 == $dt2 || $dt1 == $skipDate)
		{
			$evtCntPerDay++;
		}
		else
		{
			$evtCntPerDay = 0;
		}
		
	}
	//return drupal_json_encode($consolidated); 
	return drupal_json_encode($outputarr); 
	//return '[{"id":"announcement-125","fullname":"simbu k","username":"simbu","from_date":"2016-03-04","to_date":"2016-05-03","from_time":"00:45","to_time":"hh:mm","description":"04-03-2016 ANNM","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"00:45","title":"04-03-2016 ANNM","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"00:45","start":"2016-03-04T00:45:00","actualStart":"2016-03-04T00:45:00","endTime":"23:59","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"21","title_short":"04-03-2016 ANNM"},{"title":"Certification - Audit Report","editable":true,"start":"2016-03-07T00:30:00","startTime":"00:30","type":"report","id":"report-20","scheduled_id":"20","allDay":true,"report_id":"121","status":"Active","frequency":"yearly"},{"title":"Course Enrollment Report - WBT, ILT \u0026amp; VCL","editable":true,"start":"2016-03-07T00:30:00","startTime":"00:30","type":"report","id":"report-27","scheduled_id":"27","allDay":true,"report_id":"113","status":"Active","frequency":"weekly","dow":[4],"created":"2016-03-07","end":"2016-05-02"},{"title":"Course Roster - By Registration Date","editable":true,"start":"2016-03-07T00:30:00","startTime":"00:30","type":"report","id":"report-26","scheduled_id":"26","allDay":true,"report_id":"41","status":"Active","frequency":"weekly","dow":[4],"created":"2016-03-07","end":"2016-05-02"},{"title":"Classes Pending for Completion Marking","editable":true,"start":"2016-03-07T00:30:00","startTime":"00:30","type":"report","id":"report-29","scheduled_id":"29","allDay":true,"report_id":"45","status":"Active","frequency":"weekly","dow":[5],"created":"2016-03-07","end":"2016-05-02"},{"title":"Compliance Registration by Manager","editable":true,"start":"2016-03-07T00:30:00","startTime":"00:30","type":"report","id":"report-28","scheduled_id":"28","allDay":true,"report_id":"62","status":"Active","frequency":"weekly","dow":[5],"created":"2016-03-07","end":"2016-05-02"},{"title":"Class Survey Summary","editable":true,"start":"2016-03-07T01:00:00","startTime":"01:00","type":"report","id":"report-24","scheduled_id":"24","allDay":true,"report_id":"54","status":"Active","frequency":"weekly","dow":[4],"created":"2016-03-07","end":"2016-05-02"},{"title":"Waitlist Summary by Class","editable":true,"start":"2016-03-07T01:15:00","startTime":"01:15","type":"report","id":"report-5","scheduled_id":"5","allDay":true,"report_id":"35","status":"Active","frequency":"weekly","dow":[2],"created":"2016-03-07","end":"2016-05-02"},{"id":"announcement-53","fullname":"simbu k","username":"simbu","from_date":"2016-03-07","to_date":"2016-05-03","from_time":"01:30","to_time":"17:00","description":"announcement - till 5pm Tells the story of Jesus Christ at age seven as he and his family depart Egypt to return home to Nazareth. Told from his childhood perspective, it follows young Jesus as he grows into his religious identity","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"17:00","ori_start_time":"01:30","title":"announcement - till 5pm Tells the story of Jesus Christ at age seven as he and his family depart Egypt to return home to Nazareth. Told from his childhood perspective, it follows young Jesus as he grows into his religious identity","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"01:30","start":"2016-03-07T01:30:00","actualStart":"2016-03-07T01:30:00","endTime":"17:00","end":"2016-05-03T17:00:00","actualEnd":"2016-05-03T17:00:00","allDay":true,"dayDifference":"18","title_short":"announcement - till 5pm Tells the story of Jesus Christ at age seven as he and..."},{"title":"Classes Pending for Completion Marking","editable":true,"start":"2016-03-07T01:30:00","startTime":"01:30","type":"report","id":"report-25","scheduled_id":"25","allDay":true,"report_id":"45","status":"Active","frequency":"weekly","dow":[4],"created":"2016-03-07","end":"2016-05-02"},{"title":"Class survey Ratings","editable":true,"start":"2016-03-07T01:30:00","startTime":"01:30","type":"report","id":"report-18","scheduled_id":"18","allDay":true,"report_id":"55","status":"Active","frequency":"yearly"},{"id":"announcement-55","fullname":"simbu k","username":"simbu","from_date":"2016-03-07","to_date":"2016-05-03","from_time":"01:30","to_time":"18:00","description":"ann till 6pm Tells the story of Jesus Christ at age seven as he and his family depart Egypt to return home to Nazareth. Told from his childhood perspective, it follows young Jesus as he grows into his religious identity","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"18:00","ori_start_time":"01:30","title":"ann till 6pm Tells the story of Jesus Christ at age seven as he and his family depart Egypt to return home to Nazareth. Told from his childhood perspective, it follows young Jesus as he grows into his religious identity","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"01:30","start":"2016-03-07T01:30:00","actualStart":"2016-03-07T01:30:00","endTime":"18:00","end":"2016-05-03T18:00:00","actualEnd":"2016-05-03T18:00:00","allDay":true,"dayDifference":"18","title_short":"ann till 6pm Tells the story of Jesus Christ at age seven as he and his family..."},{"title":"Certification - Audit Report","editable":true,"start":"2016-03-07T02:15:00","startTime":"02:15","type":"report","id":"report-17","scheduled_id":"17","allDay":true,"report_id":"121","status":"Active","frequency":"yearly"},{"title":"Admin group with Users(Added by system or Added\/Removed by Admin)","editable":true,"start":"2016-03-07T03:30:00","startTime":"03:30","type":"report","id":"report-10","scheduled_id":"10","allDay":true,"report_id":"125","status":"Active","frequency":"yearly"},{"title":"Class Roster","editable":true,"start":"2016-03-07T05:45:00","startTime":"05:45","type":"report","id":"report-21","scheduled_id":"21","allDay":true,"report_id":"2","status":"Active","frequency":"yearly"},{"title":"Assessment Learner Level Item Analysis","editable":true,"start":"2016-03-07T18:30:00","startTime":"18:30","type":"report","id":"report-30","scheduled_id":"30","allDay":true,"report_id":"99","status":"Active","frequency":"weekly","dow":[5],"created":"2016-03-07","end":"2016-05-02"},{"title":"Catalog Access Report ","editable":true,"start":"2016-03-07T20:15:00","startTime":"20:15","type":"report","id":"report-16","scheduled_id":"16","allDay":true,"report_id":"59","status":"Active","frequency":"weekly","dow":[1],"created":"2016-03-07","end":"2016-05-02"},{"title":"User list - Daily","editable":true,"start":"2016-03-08T00:15:00","startTime":"00:15","type":"report","id":"report-33","scheduled_id":"33","allDay":true,"report_id":"131","status":"Active","frequency":"daily","end":"2016-05-02"},{"title":"Person list - PErm Thamil - Weekly","editable":true,"start":"2016-03-08T00:30:00","startTime":"00:30","type":"report","id":"report-32","scheduled_id":"32","allDay":true,"report_id":"130","status":"Active","frequency":"weekly","dow":[5],"created":"2016-03-08","end":"2016-05-02"},{"title":"User list - Thamil - Yearly","editable":true,"start":"2016-03-08T03:30:00","startTime":"03:30","type":"report","id":"report-31","scheduled_id":"31","allDay":true,"report_id":"129","status":"Active","frequency":"yearly"},{"id":"announcement-72","fullname":"ExpertusONE  Admin","username":"admin","from_date":"2016-03-10","to_date":"2016-05-03","from_time":"hh:mm","to_time":"hh:mm","description":"\u003cspan style=\"color: #444444; font-family: Lato, sans-serif; font-size: 16px; font-style: normal; font-variant: normal; font-weight: normal; letter-spacing: normal; line-height: 27.2px; orphans: auto; text-align: start; text-indent: 0px; text-transform: none; white-space: normal; widows: 1; word-spacing: 0px; -webkit-text-stroke-width: 0px; display: inline !important; float: none; background-color: #ffffff;\"\u003ea complete solution for event content-management. Beyond dragging an event to a different time\/day, you cannot change an event\u0027s name or other associated data. It is up to you to add this functionality through FullCalendar\u0027s event hooks\u003c\/span\u003e","status_code":"cre_sys_obt_not_itv","priority":"1","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"a complete solution for event content-management. Beyond dragging an event to a different time\/day, you cannot change an event\u0026#039;s name or other associated data. It is up to you to add this functionality through FullCalendar\u0026#039;s event hooks","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-10T00:00:00","actualStart":"2016-03-10T00:00:00","endTime":"","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"15","title_short":"a complete solution for event content-management. Beyond dragging an event to a..."},{"id":"announcement-71","fullname":"ExpertusONE  Admin","username":"admin","from_date":"2016-03-10","to_date":"2016-05-09","from_time":"08:00","to_time":"11:30","description":"FullCalendar is great for displaying events","status_code":"cre_sys_obt_not_itv","priority":"1","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":"2016-05-09","ori_end_time":"11:30","ori_start_time":"08:00","title":"FullCalendar is great for displaying events","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"08:00","start":"2016-03-10T08:00:00","actualStart":"2016-03-10T08:00:00","endTime":"11:30","end":"2016-05-09T11:30:00","actualEnd":"2016-05-09T11:30:00","allDay":true,"dayDifference":"60","title_short":"FullCalendar is great for displaying events"},{"id":"announcement-76","fullname":"ExpertusONE  Admin","username":"admin","from_date":"2016-03-11","to_date":"2016-05-03","from_time":"hh:mm","to_time":"hh:mm","description":"a complete solution for event content-management. Beyond dragging an event to a 6464645","status_code":"cre_sys_obt_not_itv","priority":"1","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"a complete solution for event content-management. Beyond dragging an event to a 6464645","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-11T00:00:00","actualStart":"2016-03-11T00:00:00","endTime":"","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"14","title_short":"a complete solution for event content-management. Beyond dragging an event to a..."},{"id":"announcement-77","fullname":"ExpertusONE  Admin","username":"admin","from_date":"2016-03-11","to_date":"2016-05-03","from_time":"hh:mm","to_time":"hh:mm","description":"\u003cspan style=\"color: #444444; font-family: Lato, sans-serif; font-size: 16px; font-style: normal; font-variant: normal; font-weight: normal; letter-spacing: normal; line-height: 27.2px; orphans: auto; text-align: start; text-indent: 0px; text-transform: none; white-space: normal; widows: 1; word-spacing: 0px; -webkit-text-stroke-width: 0px; display: inline !important; float: none; background-color: #ffffff;\"\u003eFullCalendar is great for displaying events, but it isn\u0027t a complete solution for event content-management. Beyond dragging an event to a different time\/day, you cannot change an event\u0027s name or other associated data. It is up to you to add this functionality through FullCalendar\u0027s event hooks.\u003c\/span\u003e","status_code":"cre_sys_obt_not_itv","priority":"1","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"FullCalendar is great for displaying events, but it isn\u0026#039;t a complete solution for event content-management. Beyond dragging an event to a different time\/day, you cannot change an event\u0026#039;s name or other associated data. It is up to you to add this functionality through FullCalendar\u0026#039;s event hooks.","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-11T00:00:00","actualStart":"2016-03-11T00:00:00","endTime":"","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"14","title_short":"FullCalendar is great for displaying events, but it isn\u0027t a complete solution f..."},{"id":"announcement-78","fullname":"ExpertusONE  Admin","username":"admin","from_date":"2016-03-11","to_date":"2016-05-03","from_time":"hh:mm","to_time":"hh:mm","description":"\u003cspan style=\"color: #444444; font-family: Lato, sans-serif; font-size: 16px; font-style: normal; font-variant: normal; font-weight: normal; letter-spacing: normal; line-height: 27.2px; orphans: auto; text-align: start; text-indent: 0px; text-transform: none; white-space: normal; widows: 1; word-spacing: 0px; -webkit-text-stroke-width: 0px; display: inline !important; float: none; background-color: #ffffff;\"\u003eFullCalendar is great for displaying events, but it isn\u0027t a complete solution for event content-management. Beyond dragging an event to a different time\/day, you cannot change an event\u0027s name or other associated data. It is up to you to add this functionality through FullCalendar\u0027s event hooks.\u003c\/span\u003e","status_code":"cre_sys_obt_not_itv","priority":"1","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"FullCalendar is great for displaying events, but it isn\u0026#039;t a complete solution for event content-management. Beyond dragging an event to a different time\/day, you cannot change an event\u0026#039;s name or other associated data. It is up to you to add this functionality through FullCalendar\u0026#039;s event hooks.","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-11T00:00:00","actualStart":"2016-03-11T00:00:00","endTime":"","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"14","title_short":"FullCalendar is great for displaying events, but it isn\u0027t a complete solution f..."},{"title":"Course Access Report","editable":true,"start":"2016-03-11T00:45:00","startTime":"00:45","type":"report","id":"report-36","scheduled_id":"36","allDay":true,"report_id":"58","status":"Active","frequency":"yearly"},{"title":"Sign-in Sheet","editable":true,"start":"2016-03-11T01:15:00","startTime":"01:15","type":"report","id":"report-35","scheduled_id":"35","allDay":true,"report_id":"1","status":"Active","frequency":"yearly"},{"id":"announcement-84","fullname":"ExpertusONE  Admin","username":"admin","from_date":"2016-03-16","to_date":"2016-05-03","from_time":"hh:mm","to_time":"hh:mm","description":"AN5","status_code":"cre_sys_obt_not_itv","priority":"1","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"AN5","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-16T00:00:00","actualStart":"2016-03-16T00:00:00","endTime":"","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"9","title_short":"AN5"},{"id":"announcement-82","fullname":"ExpertusONE  Admin","username":"admin","from_date":"2016-03-16","to_date":"2016-05-03","from_time":"hh:mm","to_time":"hh:mm","description":"AN3","status_code":"cre_sys_obt_not_itv","priority":"1","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"AN3","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-16T00:00:00","actualStart":"2016-03-16T00:00:00","endTime":"","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"9","title_short":"AN3"},{"id":"announcement-83","fullname":"ExpertusONE  Admin","username":"admin","from_date":"2016-03-16","to_date":"2016-05-03","from_time":"hh:mm","to_time":"hh:mm","description":"AN3","status_code":"cre_sys_obt_not_itv","priority":"1","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"AN3","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-16T00:00:00","actualStart":"2016-03-16T00:00:00","endTime":"","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"9","title_short":"AN3"},{"id":"announcement-98","fullname":"ExpertusONE  Admin","username":"admin","from_date":"2016-03-17","to_date":"2016-05-03","from_time":"hh:mm","to_time":"hh:mm","description":"rtretertertertert","status_code":"cre_sys_obt_not_itv","priority":"1","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"rtretertertertert","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-17T00:00:00","actualStart":"2016-03-17T00:00:00","endTime":"","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"8","title_short":"rtretertertertert"},{"id":"announcement-94","fullname":"priya c","username":"priya","from_date":"2016-03-17","to_date":"2016-05-03","from_time":"hh:mm","to_time":"hh:mm","description":"trye","status_code":"cre_sys_obt_not_atv","priority":"2","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"trye","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-17T00:00:00","actualStart":"2016-03-17T00:00:00","endTime":"","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"8","title_short":"trye"},{"title":"Users Organization Report","editable":true,"start":"2016-03-17T00:15:00","startTime":"00:15","type":"report","id":"report-39","scheduled_id":"39","allDay":true,"report_id":"48","status":"Active","frequency":"daily","end":"2016-05-02"},{"id":"announcement-93","fullname":"priya c","username":"priya","from_date":"2016-03-18","to_date":"2016-05-03","from_time":"hh:mm","to_time":"hh:mm","description":"Testing announcement in day view","status_code":"cre_sys_obt_not_atv","priority":"2","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"Testing announcement in day view","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-18T00:00:00","actualStart":"2016-03-18T00:00:00","endTime":"","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"7","title_short":"Testing announcement in day view"},{"id":"announcement-103","fullname":"an admin","username":"an_admin","from_date":"2016-03-19","to_date":"2016-05-03","from_time":"00:00","to_time":"hh:mm","description":"AN ANNOUNCEMENT 4 - Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India.Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India.Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Va","status_code":"cre_sys_obt_not_atv","priority":"2","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"00:00","title":"AN ANNOUNCEMENT 4 - Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India.Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India.Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Va","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"00:00","start":"2016-03-19T00:00:00","actualStart":"2016-03-19T00:00:00","endTime":"23:59","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"6","title_short":"AN ANNOUNCEMENT 4 - Bank of Baroda is an Indian state-owned banking and financi..."},{"id":"announcement-101","fullname":"an admin","username":"an_admin","from_date":"2016-03-19","to_date":"2016-05-03","from_time":"00:00","to_time":"hh:mm","description":"AN ANNOUNCEMENT -Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India.2.Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India.3.Bank of Baroda is an Indian state-owned banking and financial services company headquartered in","status_code":"cre_sys_obt_not_atv","priority":"2","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"00:00","title":"AN ANNOUNCEMENT -Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India.2.Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India.3.Bank of Baroda is an Indian state-owned banking and financial services company headquartered in","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"00:00","start":"2016-03-19T00:00:00","actualStart":"2016-03-19T00:00:00","endTime":"23:59","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"6","title_short":"AN ANNOUNCEMENT -Bank of Baroda is an Indian state-owned banking and financial..."},{"id":"announcement-104","fullname":"an admin","username":"an_admin","from_date":"2016-03-19","to_date":"2016-05-03","from_time":"00:00","to_time":"hh:mm","description":"AN ANNOUNCEMENT 5 - Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India","status_code":"cre_sys_obt_not_atv","priority":"2","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"00:00","title":"AN ANNOUNCEMENT 5 - Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"00:00","start":"2016-03-19T00:00:00","actualStart":"2016-03-19T00:00:00","endTime":"23:59","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"6","title_short":"AN ANNOUNCEMENT 5 - Bank of Baroda is an Indian state-owned banking and financi..."},{"title":"Course Completion Report","editable":true,"start":"2016-03-19T00:15:00","startTime":"00:15","type":"report","id":"report-40","scheduled_id":"40","allDay":true,"report_id":"86","status":"Active","frequency":"daily","end":"2016-05-02"},{"id":"announcement-102","fullname":"an admin","username":"an_admin","from_date":"2016-03-19","to_date":"2016-05-03","from_time":"14:00","to_time":"hh:mm","description":"AN ANNOUNCEMENT 3 - Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India.AN ANNOUNCEMENT 3 - Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India.AN ANNOUNCEMENT 3 - Bank of Baroda is an Indian state-owned banking and financ","status_code":"cre_sys_obt_not_atv","priority":"2","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"14:00","title":"AN ANNOUNCEMENT 3 - Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India.AN ANNOUNCEMENT 3 - Bank of Baroda is an Indian state-owned banking and financial services company headquartered in Vadodara in Gujarat, India.It is the second largest bank in India, next to State Bank Of India.AN ANNOUNCEMENT 3 - Bank of Baroda is an Indian state-owned banking and financ","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"14:00","start":"2016-03-19T14:00:00","actualStart":"2016-03-19T14:00:00","endTime":"23:59","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"6","title_short":"AN ANNOUNCEMENT 3 - Bank of Baroda is an Indian state-owned banking and financi..."},{"id":"announcement-70","fullname":"yogaraja k","username":"yogaraja","from_date":"2016-03-21","to_date":"2016-05-11","from_time":"hh:mm","to_time":"hh:mm","description":"20-03-16 to 10-05-16","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":"2016-05-11","ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"20-03-16 to 10-05-16","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-21T00:00:00","actualStart":"2016-03-21T00:00:00","endTime":"","end":"2016-05-11T23:59:59","actualEnd":"2016-05-11T23:59:59","allDay":true,"dayDifference":"51","title_short":"20-03-16 to 10-05-16"},{"id":"announcement-92","fullname":"priya c","username":"priya","from_date":"2016-03-21","to_date":"2016-05-03","from_time":"hh:mm","to_time":"hh:mm","description":"tr","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"tr","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-21T00:00:00","actualStart":"2016-03-21T00:00:00","endTime":"","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"4","title_short":"tr"},{"title":"Assessment Summary - Attempt by user","editable":true,"start":"2016-03-21T00:00:00","startTime":"00:00","type":"report","id":"report-46","scheduled_id":"46","allDay":true,"report_id":"100","status":"Active","frequency":"daily","end":"2016-05-02"},{"id":"announcement-132","fullname":"vishal a","username":"vishal","from_date":"2016-03-21","to_date":"2016-05-03","from_time":"hh:mm","to_time":"hh:mm","description":"annnnww","status_code":"cre_sys_obt_not_itv","priority":"2","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"annnnww","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-21T00:00:00","actualStart":"2016-03-21T00:00:00","endTime":"","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"4","title_short":"annnnww"},{"id":"announcement-133","fullname":"vishal a","username":"vishal","from_date":"2016-03-21","to_date":"2016-05-03","from_time":"00:00","to_time":"hh:mm","description":"\u003cspan style=\"color: #333333; font-family: \u0027Open Sans\u0027, Arial, Helvetica, sans-serif; line-height: 18px;\"\u003eExpertusONE delivers a superior LMS experience that enables adoption, speeds compliance and significantly improves training effectiveness \u2212 for your learners, partners and customers alike! Ranked the #1 LMS for 2013 and 2014, ExpertusONE\u2019s simplified architecture leverages the intuitive design principles we\u2019ve come to expect from everyday modern technology. So you get: - See more at: http:\/\/www.expertus.com\/expertus-one\/#sthash.NtOeBRuc.dpuf\u003c\/span\u003e","status_code":"cre_sys_obt_not_itv","priority":"2","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"00:00","title":"ExpertusONE delivers a superior LMS experience that enables adoption, speeds compliance and significantly improves training effectiveness \u0026minus; for your learners, partners and customers alike! Ranked the #1 LMS for 2013 and 2014, ExpertusONE\u0026rsquo;s simplified architecture leverages the intuitive design principles we\u0026rsquo;ve come to expect from everyday modern technology. So you get: - See more at: http:\/\/www.expertus.com\/expertus-one\/#sthash.NtOeBRuc.dpuf","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"00:00","start":"2016-03-21T00:00:00","actualStart":"2016-03-21T00:00:00","endTime":"23:59","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"4","title_short":"ExpertusONE delivers a superior LMS experience that enables adoption, speeds..."},{"title":"Certifications Held by External \u0026amp; Internal Learners","editable":true,"start":"2016-03-21T00:15:00","startTime":"00:15","type":"report","id":"report-49","scheduled_id":"49","allDay":true,"report_id":"109","status":"Active","frequency":"daily","end":"2016-05-02"},{"title":"Assessment - Audit Report","editable":true,"start":"2016-03-21T01:30:00","startTime":"01:30","type":"report","id":"report-43","scheduled_id":"43","allDay":true,"report_id":"116","status":"Active","frequency":"daily","end":"2016-05-02"},{"title":"Assessment Learner Level Item Analysis","editable":true,"start":"2016-03-21T02:00:00","startTime":"02:00","type":"report","id":"report-44","scheduled_id":"44","allDay":true,"report_id":"99","status":"Active","frequency":"daily","end":"2016-05-02"},{"title":"Catalog Access Report ","editable":true,"start":"2016-03-21T02:00:00","startTime":"02:00","type":"report","id":"report-48","scheduled_id":"48","allDay":true,"report_id":"59","status":"Active","frequency":"daily","end":"2016-05-02"},{"title":"Assessment Summary","editable":true,"start":"2016-03-21T05:15:00","startTime":"05:15","type":"report","id":"report-45","scheduled_id":"45","allDay":true,"report_id":"90","status":"Active","frequency":"daily","end":"2016-05-02"},{"title":"Admin Groups and attributes","editable":true,"start":"2016-03-21T06:30:00","startTime":"06:30","type":"report","id":"report-42","scheduled_id":"42","allDay":true,"report_id":"79","status":"Active","frequency":"daily","end":"2016-05-02"},{"title":"Admin group with Users(Added by system or Added\/Removed by Admin)","editable":true,"start":"2016-03-21T13:30:00","startTime":"13:30","type":"report","id":"report-41","scheduled_id":"41","allDay":true,"report_id":"125","status":"Active","frequency":"daily","end":"2016-05-02"},{"title":"All Order Details","editable":true,"start":"2016-03-21T20:00:00","startTime":"20:00","type":"report","id":"report-51","scheduled_id":"51","allDay":true,"report_id":"104","status":"Active","frequency":"daily","end":"2016-05-02"},{"title":"Admin group with Users(Added by system or Added\/Removed by Admin)","editable":true,"start":"2016-03-21T20:00:00","startTime":"20:00","type":"report","id":"report-50","scheduled_id":"50","allDay":true,"report_id":"125","status":"Active","frequency":"daily","end":"2016-05-02"},{"id":"announcement-138","fullname":"yogaraja k","username":"yogaraja","from_date":"2016-03-21","to_date":"2016-05-03","from_time":"22:45","to_time":"hh:mm","description":"teswfsd sdfsdfsdfsdfsdfsdfsdf","status_code":"cre_sys_obt_not_itv","priority":"1","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"22:45","title":"teswfsd sdfsdfsdfsdfsdfsdfsdf","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"22:45","start":"2016-03-21T22:45:00","actualStart":"2016-03-21T22:45:00","endTime":"23:59","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"4","title_short":"teswfsd sdfsdfsdfsdfsdfsdfsdf"},{"id":"announcement-139","fullname":"an admin","username":"an_admin","from_date":"2016-03-21","to_date":"2016-05-03","from_time":"23:30","to_time":"hh:mm","description":"AN4 ANNOUNCEMENT - The country is classified as a newly industrialised country, one of the G-20 major economies, a member of BRICS and a developing economy with an average growth rate of approximately 7% over the last two decades. Maharashtra is the wealthiest Indian state and has an annual GDP of US$220 billion, nearly equal to that of Pakistan or Portugal, and accounts for 12% of the Indian GDP followed by yogaraja","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"23:30","title":"AN4 ANNOUNCEMENT - The country is classified as a newly industrialised country, one of the G-20 major economies, a member of BRICS and a developing economy with an average growth rate of approximately 7% over the last two decades. Maharashtra is the wealthiest Indian state and has an annual GDP of US$220 billion, nearly equal to that of Pakistan or Portugal, and accounts for 12% of the Indian GDP followed by yogaraja","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"23:30","start":"2016-03-21T23:30:00","actualStart":"2016-03-21T23:30:00","endTime":"23:59","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"4","title_short":"AN4 ANNOUNCEMENT - The country is classified as a newly industrialised country,..."},{"title":"Banner - Audit Report","editable":true,"start":"2016-03-21T23:45:00","startTime":"23:45","type":"report","id":"report-47","scheduled_id":"47","allDay":true,"report_id":"120","status":"Active","frequency":"daily","end":"2016-05-02"},{"title":"Class Commerce Report","editable":true,"start":"2016-03-22T00:15:00","startTime":"00:15","type":"report","id":"report-54","scheduled_id":"54","allDay":true,"report_id":"105","status":"Active","frequency":"weekly","dow":[0],"created":"2016-03-22","end":"2016-05-02"},{"title":"Class Roster","editable":true,"start":"2016-03-22T00:15:00","startTime":"00:15","type":"report","id":"report-56","scheduled_id":"56","allDay":true,"report_id":"2","status":"Active","frequency":"daily","end":"2016-05-02"},{"title":"Class Survey Summary","editable":true,"start":"2016-03-22T00:15:00","startTime":"00:15","type":"report","id":"report-58","scheduled_id":"58","allDay":true,"report_id":"54","status":"Active","frequency":"daily","end":"2016-05-02"},{"title":"Classes Pending for Completion Marking","editable":true,"start":"2016-03-22T00:15:00","startTime":"00:15","type":"report","id":"report-59","scheduled_id":"59","allDay":true,"report_id":"45","status":"Active","frequency":"daily","end":"2016-05-02"},{"title":"Class Commerce Report","editable":true,"start":"2016-03-22T00:15:00","startTime":"00:15","type":"report","id":"report-53","scheduled_id":"53","allDay":true,"report_id":"105","status":"Active","frequency":"daily","end":"2016-05-02"},{"title":"Classes and associated Admin Groups","editable":true,"start":"2016-03-22T15:45:00","startTime":"15:45","type":"report","id":"report-52","scheduled_id":"52","allDay":true,"report_id":"67","status":"Active","frequency":"daily","end":"2016-05-02"},{"id":"announcement-118","fullname":"vishal a","username":"vishal","from_date":"2016-03-23","to_date":"2016-05-03","from_time":"00:00","to_time":"hh:mm","description":"announcement333","status_code":"cre_sys_obt_not_atv","priority":"2","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"00:00","title":"announcement333","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"00:00","start":"2016-03-23T00:00:00","actualStart":"2016-03-23T00:00:00","endTime":"23:59","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"2","title_short":"announcement333"},{"title":"Compliance Course - 30 day Reminder Report","editable":true,"start":"2016-03-23T00:30:00","startTime":"00:30","type":"report","id":"report-61","scheduled_id":"61","allDay":true,"report_id":"110","status":"Active","frequency":"daily","end":"2016-05-02"},{"title":"Compliance Registration by Manager","editable":true,"start":"2016-03-23T00:30:00","startTime":"00:30","type":"report","id":"report-64","scheduled_id":"64","allDay":true,"report_id":"62","status":"Active","frequency":"daily","end":"2016-05-02"},{"title":"Compliance Course Summary by Status","editable":true,"start":"2016-03-23T02:00:00","startTime":"02:00","type":"report","id":"report-63","scheduled_id":"63","allDay":true,"report_id":"57","status":"Active","frequency":"daily","end":"2016-05-02"},{"title":"Assessment Summary","editable":true,"start":"2016-03-23T04:30:00","startTime":"04:30","type":"report","id":"report-60","scheduled_id":"60","allDay":true,"report_id":"90","status":"Active","frequency":"yearly"},{"title":"All Order Details","editable":true,"start":"2016-03-23T12:30:00","startTime":"12:30","type":"report","id":"report-65","scheduled_id":"65","allDay":true,"report_id":"104","status":"Active","frequency":"daily","end":"2016-05-02"},{"title":"Classes and associated Admin Groups","editable":true,"start":"2016-03-23T12:45:00","startTime":"12:45","type":"report","id":"report-66","scheduled_id":"66","allDay":true,"report_id":"67","status":"Active","frequency":"daily","end":"2016-05-02"},{"title":"Compliance Course - 60 Days Reminder Report","editable":true,"start":"2016-03-23T23:45:00","startTime":"23:45","type":"report","id":"report-62","scheduled_id":"62","allDay":true,"report_id":"111","status":"Active","frequency":"daily","end":"2016-05-02"},{"id":"announcement-123","fullname":"vishal a","username":"vishal","from_date":"2016-03-24","to_date":"2016-05-03","from_time":"hh:mm","to_time":"hh:mm","description":"check","status_code":"cre_sys_obt_not_itv","priority":"2","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"check","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-03-24T00:00:00","actualStart":"2016-03-24T00:00:00","endTime":"","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"1","title_short":"check"},{"id":"announcement-165","fullname":"ratheesh s","username":"ratheeshs","from_date":"2016-03-25","to_date":"2016-05-03","from_time":"00:30","to_time":"hh:mm","description":"announce ratheesh","status_code":"cre_sys_obt_not_atv","priority":"2","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"00:30","title":"announce ratheesh","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"00:30","start":"2016-03-25T00:30:00","actualStart":"2016-03-25T00:30:00","endTime":"23:59","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":null,"title_short":"announce ratheesh"},{"id":"announcement-156","fullname":"vishal 2","username":"vishal2","from_date":"2016-03-26","to_date":"2016-05-03","from_time":"00:30","to_time":"hh:mm","description":"annou speed","status_code":"cre_sys_obt_not_itv","priority":"2","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"00:30","title":"annou speed","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"00:30","start":"2016-03-26T00:30:00","actualStart":"2016-03-26T00:30:00","endTime":"23:59","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":null,"title_short":"annou speed"},{"id":"announcement-131","fullname":"simbu k","username":"simbu","from_date":"2016-04-01","to_date":"2016-05-03","from_time":"hh:mm","to_time":"hh:mm","description":"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-04-01T00:00:00","actualStart":"2016-04-01T00:00:00","endTime":"","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"6","title_short":"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"},{"id":"announcement-136","fullname":"simbu k","username":"simbu","from_date":"2016-04-01","to_date":"2016-05-03","from_time":"04:30","to_time":"hh:mm","description":"VISHAL-L","status_code":"cre_sys_obt_not_itv","priority":"1","language":"English","status":"Inactive","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"04:30","title":"VISHAL-L","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"04:30","start":"2016-04-01T04:30:00","actualStart":"2016-04-01T04:30:00","endTime":"23:59","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"6","title_short":"VISHAL-L"},{"id":"announcement-126","fullname":"simbu k","username":"simbu","from_date":"2016-04-03","to_date":"2016-05-03","from_time":"hh:mm","to_time":"hh:mm","description":"SIM-34#annnnnn","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"SIM-34#annnnnn","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-04-03T00:00:00","actualStart":"2016-04-03T00:00:00","endTime":"","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"8","title_short":"SIM-34#annnnnn"},{"id":"announcement-130","fullname":"simbu k","username":"simbu","from_date":"2016-04-03","to_date":"2016-05-03","from_time":"hh:mm","to_time":"hh:mm","description":"yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-04-03T00:00:00","actualStart":"2016-04-03T00:00:00","endTime":"","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"8","title_short":"yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy"},{"id":"announcement-129","fullname":"simbu k","username":"simbu","from_date":"2016-04-05","to_date":"2016-05-03","from_time":"hh:mm","to_time":"hh:mm","description":"llllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"llllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-04-05T00:00:00","actualStart":"2016-04-05T00:00:00","endTime":"","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"10","title_short":"llllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll"},{"id":"announcement-128","fullname":"simbu k","username":"simbu","from_date":"2016-04-08","to_date":"2016-05-03","from_time":"hh:mm","to_time":"hh:mm","description":"oooooooooooooooooo00000000000000000000000000","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"oooooooooooooooooo00000000000000000000000000","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-04-08T00:00:00","actualStart":"2016-04-08T00:00:00","endTime":"","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"13","title_short":"oooooooooooooooooo00000000000000000000000000"},{"id":"announcement-127","fullname":"simbu k","username":"simbu","from_date":"2016-04-09","to_date":"2016-05-03","from_time":"hh:mm","to_time":"hh:mm","description":"vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-04-09T00:00:00","actualStart":"2016-04-09T00:00:00","endTime":"","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"14","title_short":"vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv"},{"id":"announcement-97","fullname":"simbu k","username":"simbu","from_date":"2016-04-19","to_date":"2016-05-03","from_time":"hh:mm","to_time":"hh:mm","description":"\u003cdiv class=\"gmail_default\" style=\"font-family: verdana,sans-serif;\"\u003e\u003cstrong\u003eAnnouncement\u0026nbsp;Cases: - \u003c\/strong\u003e\u003cspan style=\"font-family: verdana,sans-serif;\"\u003efrom date \u0026nbsp;(All day with out range)\u003c\/span\u003e\u003c\/div\u003e","status_code":"cre_sys_obt_not_atv","priority":"1","language":"English","status":"Active","object_type":"announcement","ori_to_date":null,"ori_end_time":"hh:mm","ori_start_time":"hh:mm","title":"","type":"cre_sys_obt_anm","editable":1,"deletable":1,"startTime":"All Day","start":"2016-04-19T00:00:00","actualStart":"2016-04-19T00:00:00","endTime":"","end":"2016-05-03T23:59:59","actualEnd":"2016-05-03T23:59:59","allDay":true,"dayDifference":"24","title_short":"AnnouncementCases: - from date (All day with out ran..."},{"title":"Class Roster","editable":true,"start":"2016-05-02","startTime":"00:15","type":"report","id":"report-57","scheduled_id":"57","allDay":true,"report_id":"2","status":"Active","frequency":"monthly","end":"2016-05-02"}]';
	 } catch (Exception $e) {
        expertusErrorThrow($e);
    }

}

function ac_events_sort($a,$b)
{
	if (strtotime($a->start)==strtotime($b->start)) return 0;
		return (strtotime($a->start)<strtotime($b->start))?-1:1;
}

function getAnnouncementData($search_text = "", $autocomplete = false)
{
	try
	{
	$_GET["_search"] = "false";
	$_GET["nd"] = "1451583582118";
	$_GET["rows"] = "100";
	$_GET["page"] = "1";
	$_GET["sidx"] = "";
	$_GET["sord"] = "desc";
	$_GET["deliverytype"] = "";
	$_GET["catalogclassstatus"] = "";
	$_GET["catalogcoursestatus"] = "";
	$_GET["courselangtype"] = "";
	$_GET["classlangtype"] = "";
	$_GET["countrytype"] = "";
	$_GET["coursemanage"] = "";
	$_GET["classmanage"] = "";
	$_GET["clspgopened"] = "";
	$_GET["classcurrency"] = "";
	$_GET["catalogcoursetag"] = "";
	$_GET["catalogclasstag"] = "";
	$_GET["classLocation"] = "";
	$_GET["classInstructor"] = "";
	$_GET["classdaterange"] = "|";
	$_GET["classContent"] = "";
	$_GET["classSurvey"] = "";
	$_GET["classAssessment"] = "";
	$_GET["classPrice"] = "";
	$_GET["sortby"] = "";
	$_GET["announcemanage"] = "cre_sys_fop_me|cre_sys_fop_oth";
	$_GET["announcementlang"] =	"cre_sys_lng_eng|cre_sys_lng_frh|cre_sys_lng_den|cre_sys_lng_itn|cre_sys_lng_jap|cre_sys_lng_kon|cre_sys_lng_pts|cre_sys_lng_rus|cre_sys_lng_esp|cre_sys_lng_gzh";
	$userId = getSltpersonUserId();

	//$_GET['daterange'] =  $_REQUEST['startDate']."|".$_REQUEST['endDate'];
	if($autocomplete)
	{
	if(isset($_GET['calendar_searchtype']) && !empty($_GET['calendar_searchtype'])){
		$_GET['textfilter'] = $_GET['z'];
		}
	}
	else
	{
		if(isset($_GET['calendar_searchtype']) && !empty($_GET['calendar_searchtype'])){
			$_GET['textfilter'] = $_GET['z'];
		}
		$st = "";
		$ed = "";
		if(isset($_REQUEST['startDate']))
			$st =  date('m-d-Y',strtotime($_REQUEST['startDate']));
	
		
		if(isset($_REQUEST['endDate']) && $_REQUEST['endDate'] != "")
			$ed =  date('m-d-Y',strtotime($_REQUEST['endDate']));
		
		
		expDebug::dPrint("From date >>". $_REQUEST['startDate'],5);
		expDebug::dPrint("To date >>". $_REQUEST['endDate'],5);
		// echo $st."|".$ed;
		$_GET["announcementdaterange"] = $st."|";//.$ed;//01-22-2016|03-10-2016 
	}
//$_GET["announcementdaterange"] = "2016-01-22|2016-03-22";
//	echo $_GET["announcementdaterange"];
	include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_manage/exp_sp_administration_announcement/exp_sp_administration_announcement.inc";
	$announcementList = sendAnnouncementListNew($search_text);
	$announcements = array();
	$daywise = false;
	if($st == $ed)
	{
		$daywise = true;
	}
	foreach ($announcementList as $announcement) {
		//expDebug::dPrint('New Announcement List------------>herererere'.print_r($announcement,1),4);
		$today = date("Y-m-d");
		$splitRequired = false;
		$fields = new stdClass();
		$fields = clone $announcement;
		$fields->ori_to_date = $announcement->to_date;
		$fields->ori_end_time = $announcement->to_time;
		$fields->ori_start_time = $announcement->from_time;
		$fields->dataType =  $announcement->datatype;
		if($announcement->to_date == ''){	
		if($_REQUEST['endDate'] != null && $_REQUEST['endDate'] != "")
			{
				$EndDate = strtotime("+1 day", strtotime($_REQUEST['endDate']));
				$EndDate = date("Y-m-d", $EndDate);
				$org_ed_date = $EndDate;
		    }
		    else
		    {
		    	$EndDate = strtotime("+30 days", strtotime(date("Y-m-d")));
				$EndDate = date("Y-m-d", $EndDate);
				$org_ed_date = $EndDate;

		    }
		    
		}
		else
			$org_ed_date = $announcement->to_date;
		$fields->from_date =$announcement->from_date;
		$fields->to_date =$org_ed_date;
		if(($fields->to_date!='' && strcmp($announcement->to_time,"00:00") == 0)){
			$EndDate = strtotime("-1 day", strtotime($fields->to_date));
			$fields->to_date = date("Y-m-d", $EndDate);
			$announcement->to_time = 'hh:mm';
			$announcement->to_date =$fields->to_date;
		}
		$fields->title = html_entity_decode(strip_tags(html2textforac($announcement->description,true)));		
		$fields->type = "cre_sys_obt_anm";
		$fields->id = "announcement-".$announcement->id;
		$fields->editable = $announcement->sumedit;
		$fields->deletable = $announcement->sumdelete;
		//It should be 24 hours format and attached to start and end fields
		$timePrefix = adminCalendarTimePrefix($announcement->from_time,"00:00:00");
		//It should be 12 hours format
		$fields->startTime = date("H:i", strtotime( $announcement->from_time));
		$fields->start = $announcement->from_date."T".$timePrefix;//" ".$announcement->from_time;
		$fields->actualStart = $announcement->from_date."T".$timePrefix;
		$timePrefix = adminCalendarTimePrefix($announcement->to_time,"23:59:59");
		$fields->endTime = date("H:i", strtotime( $announcement->to_time));
		$fields->end = $fields->to_date."T".$timePrefix;
		$fields->actualEnd= $fields->to_date."T".$timePrefix;
		if(strcmp($announcement->to_time,"hh:mm") == 0 && strcmp($announcement->from_time,"hh:mm") == 0 || strcmp($announcement->to_time,"00:00") == 0 && strcmp($announcement->from_time,"00:00") == 0){
			$fields->allDay = true;
			$fields->endTime = "";
			$fields->startTime = "";
		}elseif(strcmp($announcement->to_time,"hh:mm") == 0 && strcmp($announcement->from_time,"hh:mm") != 0){
			$fields->startTime = date("H:i", strtotime( $announcement->from_time));
			$fields->endTime = "23:59";
			$fields->allDay = false;
		}
		elseif(strcmp($announcement->to_time,"hh:mm") != 0 && strcmp($announcement->from_time,"hh:mm") == 0){
			$fields->startTime = '00:00';
			$fields->endTime = date("H:i", strtotime( $announcement->to_time));
			$fields->allDay = false;
		}
		else{
			$fields->allDay = false;
		}
		$dayDifference = findDayDifference($announcement->from_date, $announcement->to_date);
		$fields->dayDifference = $dayDifference;
		if($dayDifference>1)
			$fields->allDay = true;
		if($announcement->to_date == '' && $dayDifference ==0){
			$fields->dayDifference = null;
			$fields->allDay = true;
		}
		/*if($dayDifference==0){
			$fields->allDay = false;
		}*/
		$fields->title_short = titleController('ANNOUNCEMENT_SHORTTITLE', $fields->title,80);
		if ($userId == 1) { // grant all privilege to super admin
			$fields->editable = 1;
			$fields->deletable = 1;
		}
		$announcements[] = $fields;
 	}
	expDebug::dPrint('Announcement List final------------>'.print_r($announcements,1),4);
	return $announcements;
	} catch (Exception $e) {
        expertusErrorThrow($e);
    }

}
function createAllDayEvents($announcement, &$announcementList_new) {
	expDebug::dPrint("createAllDayEvents called");
	$dayDifference = findDayDifference($announcement->from_date, $announcement->to_date);
	$org_st_date = $announcement->from_date;
	if($announcement->to_date == ''){
		$EndDate = strtotime("+1 day", strtotime($_REQUEST['endDate']));
		$EndDate = date("Y-m-d", $EndDate);
		$org_ed_date = $EndDate;
	}
	else
		$org_ed_date = $announcement->to_date;
	$org_st_time = $announcement->from_time;
	$org_ed_time = $announcement->to_time;	
	$announcement->to_date = $org_ed_date;
	$announcement->dayDifference = $dayDifference;
	$announcementList_new[] = $announcement;
}

function splitDoubleEvents($announcement, &$announcementList_new) {
	expDebug::dPrint("splitDoubleEvents called");
	$ann1 = new stdClass();
	$ann2 = new stdClass();
	$org_st_date = $announcement->from_date;
	$dayDifference = findDayDifference($announcement->from_date, $announcement->to_date);
	if($announcement->to_date == ''){
		$EndDate = strtotime("+1 day", strtotime($_REQUEST['endDate']));
		$EndDate = date("Y-m-d", $EndDate);
		$org_ed_date = $EndDate;
	}
	else
		$org_ed_date = $announcement->to_date;
	$org_st_time = $announcement->from_time;
	$org_ed_time = $announcement->to_time;
	
	// First Split (Time based)
	$ann1 = clone $announcement;
	if(($announcement->from_time !='hh:mm' && $announcement->from_time !='00:00'))
		$ann1->from_time = $org_st_time;
	else
		$ann1->from_time = 'hh:mm';
	$ann1->to_time = 'hh:mm'; // day last
	$ann1->from_date = $org_st_date;
	$ann1->dayDifference = $dayDifference;
	expDebug::dPrint("splitDoubleEvents Day difference:" . $dayDifference);	
	// Suppose long event has to time and no from time
	if ($announcement->from_time !='hh:mm' && $announcement->to_time !='hh:mm' && $dayDifference == 1) {
		expDebug::dPrint('to date time check0' . $dayDifference);
		$ann1->to_date = $org_st_date;
	} else if(($announcement->from_time =='hh:mm' && $announcement->to_time !='hh:mm')){
		expDebug::dPrint('to date time check1' . $dayDifference);
		$EndDate = strtotime("-1 day", strtotime($org_ed_date));
		$EndDate = date("Y-m-d", $EndDate);
		$ann1->to_date = $EndDate;
	} else if (($announcement->from_time !='hh:mm' && $announcement->to_time =='hh:mm')) {
		expDebug::dPrint('to date time check2' . $dayDifference);
		$ann1->to_date = $org_st_date;
	}
	else
		$ann1->to_date = $announcement->to_date;
	$announcementList_new[] = $ann1;
	
	// Second Split (Allday)
	$ann2 = clone $announcement;
	if(($announcement->to_time !='hh:mm' && $announcement->to_time !='00:00'))
		$ann2->to_time = $org_ed_time;
	else
		$ann2->to_time = 'hh:mm';
	$ann2->dayDifference = $dayDifference;
	if ($announcement->from_time !='hh:mm' && $announcement->to_time !='hh:mm' && $dayDifference == 1) {
		expDebug::dPrint('to date time check00' . $dayDifference);
		$ann2->to_date = $org_ed_date;
		$ann2->from_date = $org_ed_date;
	} else if(($announcement->from_time =='hh:mm' && $announcement->to_time !='hh:mm')){
		$ann2->from_date = $org_ed_date;
		$ann2->to_date = $org_ed_date;
	}else if (($announcement->from_time !='hh:mm' && $announcement->to_time =='hh:mm')) {
		expDebug::dPrint('to date time check3' . $dayDifference);
		$stDate = strtotime("+1 day", strtotime($org_st_date));
		$stDate = date("Y-m-d", $stDate);
		$ann2->from_date = $stDate;
		$ann2->to_date   = $org_ed_date;
	} else {
		$conStartDate = strtotime("+".$dayDifference." day", strtotime($org_st_date));
		expDebug::dPrint('to date time check4' . $dayDifference);
		$ann2->from_date = date("Y-m-d", $conStartDate);
		$conEndDate = strtotime("-1 day", strtotime($org_ed_date));
		$ann2->to_date = date("Y-m-d", $conEndDate);
	}

	$ann2->from_time = 'hh:mm';
	$announcementList_new[] = $ann2;
}

function splitThreeEvents($announcement, &$announcementList_new) {
	expDebug::dPrint("splitThreeEvents called");
	$ann1 = new stdClass();
	$ann2 = new stdClass();
	$ann3 = new stdClass();
	$dayDifference = findDayDifference($announcement->from_date, $announcement->to_date);
	$org_st_date = $announcement->from_date;
	if($announcement->to_date == ''){
		$EndDate = strtotime("+1 day", strtotime($_REQUEST['endDate']));
		$EndDate = date("Y-m-d", $EndDate);
		$org_ed_date = $EndDate;
	}
	else
		$org_ed_date = $announcement->to_date;
	$org_st_time = $announcement->from_time;
	$org_ed_time = $announcement->to_time;
	
	// First Split (Time based)
	$ann1 = clone $announcement;
	$ann1->from_date = $org_st_date;
	$ann1->to_date = $org_st_date;
	$ann1->dayDifference = $dayDifference;
	if(($announcement->from_time !='hh:mm' && $announcement->from_time !='00:00'))
		$ann1->from_time = $org_st_time;
	else
		$ann1->from_time = 'hh:mm';
	$ann1->to_time = 'hh:mm'; // day last
	$announcementList_new[] = $ann1;
	
	expDebug::dPrint('splitThree event $ann1' . print_r($ann1, 1));
	
	// Second Split (Allday)
	$ann2 = clone $announcement;
	$ann2->dayDifference = $dayDifference;
	$conStartDate = strtotime("+1 day", strtotime($org_st_date));
	$ann2->from_date = date("Y-m-d", $conStartDate);
	$conEndDate = strtotime("-1 day", strtotime($org_ed_date));
	$ann2->to_date = date("Y-m-d", $conEndDate);
	$ann2->from_time = 'hh:mm';
	$ann2->to_time = 'hh:mm';
	$announcementList_new[] = $ann2;
	expDebug::dPrint('splitThree event $ann2' . print_r($ann2, 1));
	
	// THird Split (Time based)
	$ann3 = clone $announcement;
	$ann3->dayDifference = $dayDifference;
	if($org_ed_time== 'hh:mm' || $org_ed_time== '00:00')
		$end_tim= 'hh:mm';
	else
		$end_tim= $org_ed_time;
	$ann3->from_date = $org_ed_date;
	$ann3->to_date = $org_ed_date;
	$ann3->from_time = 'hh:mm';
	$ann3->to_time = $end_tim;
	$announcementList_new[] = $ann3;
	expDebug::dPrint('splitThree event $ann3' . print_r($ann3, 1));
}

function findDayDifference($fromDate, $toDate) {
	$datetime1 = new DateTime($fromDate);
	$datetime2 = new DateTime($toDate);
	$interval = $datetime1->diff($datetime2);
	return $interval->format('%a');
}

function rowPopulateAnn($data, $fields){
	expDebug::dPrint('fields final------------>'.print_r($fields,1),4);
	$userId = getSltpersonUserId();
	$row = new stdClass();
	$row->title = html2textforac($data->description,true);
	$row->type = "cre_sys_obt_anm";
	$row->id = "announcement-".$data->id;
	$row->editable = $data->sumedit;
	$row->deletable = $data->sumdelete;
	//It should be 24 hours format and attached to start and end fields
	$timePrefix = adminCalendarTimePrefix($data->from_time,"00:00:00");
	//It should be 12 hours format
	$row->startTime = date("H:i", strtotime( $data->from_time));
	if($fields->startDate != ""){
		$row->start = $fields->startDate."T00:00:00";//.$timePrefix;
		$row->actualStart = $data->from_date."T".$timePrefix;
	}
	else{
		$row->start = $data->from_date."T".$timePrefix;//" ".$announcement->from_time;
		$row->actualStart = $data->from_date."T".$timePrefix;
	}
	$timePrefix = adminCalendarTimePrefix($data->to_time,"23:59:59");
	$row->endTime = date("H:i", strtotime( $data->to_time));
	if($fields->splitEndDate != ""){
		$row->end = $fields->splitEndDate."T".$timePrefix;//24:00:00";//;//" ". $announcement->to_time;//$announcement->to_date;
		$row->actualEnd= $data->to_date;
	}
	else
	{
		if( $data->to_date !=null &&  $data->to_date != ""){
			$row->end = $data->to_date."T".$timePrefix;
			$row->actualEnd= $data->to_date;
		}
		else
		{
			$row->end ='';
			$row->actualEnd='';
		}
			
	}
	if(strcmp($data->to_time,"hh:mm") == 0 && strcmp($data->from_time,"hh:mm") == 0){
		$row->allDay = true;
		$row->endTime = "";
		$row->startTime = "All day";
	}elseif(strcmp($data->to_time,"hh:mm") == 0 && strcmp($data->from_time,"hh:mm") != 0){
		$row->startTime = date("H:i", strtotime( $data->from_time));
		$row->endTime = "23:59";
		$row->allDay = false;
	}
	elseif(strcmp($data->to_time,"hh:mm") != 0 && strcmp($data->from_time,"hh:mm") == 0){
		$row->startTime = '00:00';
		$row->endTime = date("H:i", strtotime( $data->to_time));
		$row->allDay = false;
	}
	else{
		$row->allDay = false;
	}
	$row->title_short = titleController('ANNOUNCEMENT_SHORTTITLE', $row->title, 80);
	if ($userId == 1) { // grant all privilege to super admin
		$row->editable = 1;
		$row->deletable = 1;
	}
	expDebug::dPrint('fields final------------>'.print_r($fields,1),4);
	expDebug::dPrint('fields final------------>'.print_r($fields,1),4);
	expDebug::dPrint('fields final------------>'.print_r($fields,1),4);
	$today = date("Y-m-d");
	if($data->dayDifference==0 || $data->dayDifference==1)
		$row->dayDifference  = $data->dayDifference;
 	elseif($data->to_from ==$today && $data->to_date ==$today){
 		$row->end = "2016-03-14T".$timePrefix;
 	}
	expDebug::dPrint('ROW OBJECT VALUE------------->'.print_r($row,1),4);
	return $row;
}
function adminCalendarTimePrefix($dateval,$defaultVal)
{
	if($dateval != null && $dateval != "" && $dateval != "hh:mm")
		$timePrefix = date("H:i:s", strtotime( $dateval));
	else
		$timePrefix = $defaultVal;
	return $timePrefix;
}


/*
 * getProfileListItemName() - Returns the profile list item name for the profile list item code.
 */
function getProfileListItemNamesByParentId($pid, $languageCode = 'cre_sys_lng_eng') {
	try{
		if (is_null($languageCode)) {
			$languageCode = 'cre_sys_lng_eng';
		}
		// Select table
		$selectStmt = db_select('slt_profile_list_items', 'pli');
		// Set conditions
		$selectStmt->condition('pli.parent_id', $pid);
		$selectStmt->condition('pli.lang_code', $languageCode);
		// Select fields and/or add expressions
		$selectStmt->addField('pli', 'name', 'name');
		$selectStmt->addField('pli', 'code', 'code');
		expDebug::dPrintDBAPI('$selectStmt = ', $selectStmt);
		// Execute query
		$result = $selectStmt->execute();
		if ($result->rowCount() <= 0) {
			expDebug::dPrint('unknown item code',3);
			return null;
		}
		$result = $result->fetchAll();
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getProfileListItemName', $ex);
		expertusErrorThrow($ex);
	}
}

function rowPopulate($data,$fields,$class_status_meta_array = '',$all_tzs = '')
{
	$userId = getSltpersonUserId();
	$row = new stdClass();
	//expDebug::dPrint("announcement details ".print_r($data,true),5);
	if($fields->type == "cre_sys_obt_anm")
	{
		
		$row->title = html2textforac($data->description,true);
		$row->type = "cre_sys_obt_anm";
		$row->id = "announcement-".$data->id;
	}
	else
	{
		$row->title = $data->title; //." - ".$data->session_title;
	}
	$row->editable = $data->sumedit;
	$row->deletable = $data->sumdelete;	
	$row->dataType =  $data->datatype;	
	if($fields->type == "lrn_cls_dty_ilt" || $fields->type == "lrn_cls_dty_vcl")
	{
		$row->id = $fields->type."-".$data->sessionid;
	    $row->class_id = core_encrypt($data->class_id); // to encrypt plain id to env
	    $row->course_id = core_encrypt($data->course_id);
	    $row->code = $data->code;
		$row->type = $fields->type;
		$row->locationname = $data->location_name;
		$row->languagename = $data->language_name;
		$row->location_id = $data->location_id;
		$row->delivery_type_code = $data->delivery_type;
		$row->session_title = $data->session_title;
		$row->scheduled_duration = $data->scheduled_duration;
		$row->instructor = $data->instructor;
		$row->allDay = false;
		if($class_status_meta_array != '')
		{
			for($j=0; $j < count($class_status_meta_array); $j++)
			{
				$rowClsMeta = $class_status_meta_array[$j];
				//expDebug::dPrint("rowClsMeta".$rowClsMeta->code);
				if($rowClsMeta->code == $data->status)
				{
					$row->status 	= $rowClsMeta->name;			
					break;
				}
			}
		}
		else
		{
		$row->status =  getProfileListItemName($data->status);
		}
		$vSesTimeZone = ($row->delivery_type_code == "lrn_cls_dty_vcl") ? $data->timezone : $data->location_timezone;
		if($all_tzs != '')
		{
		
			for($j=0; $j < count($all_tzs); $j++)
			{
				$rowClsMeta = $all_tzs[$j];
				if($rowClsMeta->code == $vSesTimeZone)
				{
					$row->timezone 	= $rowClsMeta->attr1;			
					break;
				}
			}
		}
		else
		{
				 $tzArray = getProfileListItemByCode($vSesTimeZone);
				 $time_zone = $tzArray[0]->attr1;
				$row->timezone = $time_zone;
		
		}
	
	    		 
				 $stDate = date('Y-m-d', strtotime($data->start_date))."T" .$data->start_time;
				 $enDate = date('Y-m-d', strtotime($data->start_date))."T" .$data->end_time;
				 
				 $row->start = $stDate;
				 $row->startTime = strtoupper(date('H:i', strtotime($data->start_time)));
				 $row->end =  $enDate;//$ses_end_date->format('Y-m-d H:i:s');
				 $row->endTime =strtoupper( date('H:i', strtotime($data->end_time)));
 
     } 
	else if($fields->type == "event_regend" || $fields->type == "event_regcan")
	{
			 	$row->id = $fields->type."-".$data->class_id;
			 	if($fields->type == "event_regend")
			 	{
				 	$row->title = $data->title;
				 	$row->start = substr($data->registration_end_on,0,10)."T00:00:00";

				 }
				 else
				 {
				 	$row->title = $data->title;
				 	$row->start = substr($data->cancellation_deadline_date,0,10)."T00:00:00";

				 }

			 	$row->delivery_type_code = $data->delivery_type;
			 	$row->class_type = $data->class_type;
			 	$row->code = $data->code;
			 	$row->startTime = ""; // "All day";
			 	$row->allDay = true;
			 	$row->class_id = core_encrypt($data->class_id);
			 	$row->course_id = core_encrypt($data->course_id);
			 	$row->type = $fields->type;
			 	$row->editable = $data->sumedit;
			 	$row->deletable = $data->sumdelete;
			 	$row->locationname = $data->location_name;
			 	$row->languagename = $data->language_name;
			 //	if($eventStartTime >= $startDateTime && $eventStartTime <= $endDateTime)
			 	if($fields->type == "event_regcan")
			 	{
			 		$eventStartTime = strtotime(substr($data->cancellation_deadline_date,0,10));

			 	}
	
	}

	else if($fields->type == "cre_sys_obt_anm")
	{
		//It should be 24 hours format and attached to start and end fields
		$timePrefix = adminCalendarTimePrefix($data->from_time,"00:00:00");
		//It should be 12 hours format
		$row->startTime = date("H:i", strtotime( $data->from_time));
		if($fields->startDate != "")
		{
			$row->start = $fields->startDate."T00:00:00";//.$timePrefix;
			$row->actualStart = $data->from_date."T".$timePrefix;
		}
		else
		{
			$row->start = $data->from_date."T".$timePrefix;//" ".$announcement->from_time;
			$row->actualStart = $data->from_date."T".$timePrefix;

		}

		$timePrefix = adminCalendarTimePrefix($data->to_time,"23:59:59");
		$row->endTime = date("H:i", strtotime( $data->to_time));
		if($fields->splitEndDate != "")
			$row->end = $fields->splitEndDate."T".$timePrefix;//24:00:00";//;//" ". $announcement->to_time;//$announcement->to_date;
		else
		{
			if( $data->to_date !=null &&  $data->to_date != ""){
				$row->end = $data->to_date."T".$timePrefix;
				$row->actualEnd= $data->to_date;
			}
			else
			{
				$row->end = ""; //$_REQUEST["endDate"];
				$row->actualEnd="";
			}
			
		}
		if(strcmp($data->to_time,"hh:mm") == 0 && strcmp($data->from_time,"hh:mm") == 0){
			$row->allDay = true;
			$row->endTime = "";
			$row->startTime = "All day";
		}elseif(strcmp($data->to_time,"hh:mm") == 0 && strcmp($data->from_time,"hh:mm") != 0){
			$row->startTime = date("H:i", strtotime( $data->from_time));
			$row->endTime = "23:59";
			$row->allDay = false;
		}
		elseif(strcmp($data->to_time,"hh:mm") != 0 && strcmp($data->from_time,"hh:mm") == 0){
			$row->startTime = '00:00';
			$row->endTime = date("H:i", strtotime( $data->to_time));
			$row->allDay = false;
		}
		else{
			$row->allDay = false;
		}
		$row->title_short = titleController('ANNOUNCEMENT_SHORTTITLE', $row->title, 80);
	}
	
	if ($userId == 1) { // grant all privilege to super admin
		$row->editable = 1;
		$row->deletable = 1;
	}
	return $row;
}
function html2textforac($html, $advanced = false)
{    
     return stripRegStatus(strip_tags($html));
}
    
function iterateDataForPrintExport($data,$type,$printorexport,$pdfMaxRecords)
{
	try
	{
	$csvDataValues = "";
	for($i = 0; $i < count($data); $i++)
		{
			$row = $data[$i];
			expDebug::dPrint("iterateDataForPrintExport >>". $type.'---'.print_r($row,true),5);
			$title = '"'.$row->title.'"';
			if(substr($row->start,0,10) !=''){
				$start_date = date("m-d-Y", strtotime(substr($row->start,0,10)));	
			}else{
				$start_date = "";
			}
			$timezone = (!empty($row->timezone)) ? $row->timezone : date_default_timezone(FALSE);
			if($type=='Event')
				$type_str = t('LBL565'); // registration end text change
			if($type=='Instructor')
				$type_str = t('Instructor');
			if($type=='Location')
				$type_str = t('Location');
			if($type=='Classroom')
				$type_str = t('Classroom');
			if($type=='Virtual Classroom')
				$type_str = t('Virtual Class');
			if($type=='Announcement'){
				$type_str = t('LBL196');
			}
			if($type=='Order')
				$type_str = t('LBL1025');
			if($type == "Report"){
				$end_date = "";
				$type_str = t('Report');
			}else{
			if(substr($row->end,0,10) !=''){
				$end_date = date("m-d-Y", strtotime(substr($row->end,0,10)));	
			}else{
				$end_date = "";
			}
			}
			if($type == "Discussion"){
				$type_str = t('DISCUSSIONS');
				$starttime = translateTimingStr($row->starttime);
			}else{
				$starttime = translateTimingStr($row->startTime);
			}
			
			if($row->endTime != ''){
				$end_time=translateTimingStr($row->endTime);
			}else{
				$end_time="";
			}
			if($type=='Announcement'){
				if($row->ori_to_date == '')
					$end_date = "";
				if($row->ori_start_time == 'hh:mm')
					$starttime = "";
				if($row->ori_end_time == 'hh:mm')
					$end_time = "";
			}
			if($csvDataValues == "")
			{
				
				if($printorexport == "export")
					$csvDataValues  = $title.",".$start_date." ".$starttime.",".$end_date." ".$end_time.",".$timezone.",$type_str"."\n";
				else
					$csvDataValues  = "<tr><td style='border: 1px solid #000;' class='calendar_print_td_css'>".$row->title."</td><td style='border: 1px solid #000;' class='calendar_print_td_css'>".$start_date." ".$starttime."</td><td style='border: 1px solid #000;' class='calendar_print_td_css'>".$end_date." ".$end_time."</td><td style='border: 1px solid #000;' class='calendar_print_td_css'> ".$timezone."</td><td style='border: 1px solid #000;' class='calendar_print_td_css'> ".$type_str."</td></tr>";
			}
			else
			{
				if($printorexport == "export")
					$csvDataValues  .= $title.",".$start_date." ".$starttime.",".$end_date." ".$end_time.",".$timezone.",$type_str"."\n";
				else
					$csvDataValues  .= "<tr><td style='border: 1px solid #000;' class='calendar_print_td_css'>".$row->title."</td><td style='border: 1px solid #000;' class='calendar_print_td_css'>".$start_date." ".$starttime."</td><td style='border: 1px solid #000;' class='calendar_print_td_css'>".$end_date." ".$end_time."</td><td style='border: 1px solid #000;' class='calendar_print_td_css'> ".$timezone."</td><td style='border: 1px solid #000;' class='calendar_print_td_css'> ".$type_str."</td></tr>";
			}
			$_SESSION['total_record'] = $_SESSION['total_record']+1;
			if ($_SESSION['total_record'] > $pdfMaxRecords && $printorexport == "print") {
				$msgFontSize = (($fontSize > 8)? 8 : $fontSize);
				$csvDataValues .=
				'<tr>' .
				'<td align="center" valign="top" colspan="5" align="right" style="color:red; font-size:' . $msgFontSize . 'pt; border-bottom:none; border-right:none; border-left:none; padding-top: 1em; padding-right: 0;">' .
				t('MSG705') . ' (' . $pdfMaxRecords . '). ' . t('MSG706') . //There is a limit to the number of records that can be exported in pdf format	//Use csv format for list of all records.
				'</td>' .
				'</tr>';
				break;
				//return $csvDataValues;
			}
		}
	return $csvDataValues;
	} catch (Exception $e) {
        expertusErrorThrow($e);
    }

}

function viewdaywisereportorderlocations()
{
	try
	{
	$consolidated = array();
	$permissionList = getAdminCalendarSettings('list'); // retrive user permission from system
	$prefList 		= getAdminCalendarPreference();
	$_GET["_search"] = "false";
	$_GET["nd"] = "1451583582118";
	$_GET["rows"] = "10";
	$_GET["page"] = "1";
	$_GET["sidx"] = "";
	$_GET["sord"] = "desc";


	$_GET["deliverytype"] = "";
	$_GET["catalogclassstatus"] = "";
	$_GET["catalogcoursestatus"] = "";
	$_GET["courselangtype"] = "";
	$_GET["classlangtype"] = "";
	$_GET["countrytype"] = "";
	$_GET["coursemanage"] = "";
	$_GET["classmanage"] = "";
	$_GET["clspgopened"] = "";
	$_GET["classcurrency"] = "";
	$_GET["catalogcoursetag"] = "";
	$_GET["catalogclasstag"] = "";
	$_GET["classLocation"] = "";
	$_GET["classInstructor"] = "";
	$_GET["classdaterange"] = "|";
	$_GET["classContent"] = "";
	$_GET["classSurvey"] = "";
	$_GET["classAssessment"] = "";
	$_GET["classPrice"] = "";
	$_GET["sortby"] = "";
	//$_GET['daterange'] =  $_REQUEST['startDate']."|".$_REQUEST['endDate'];
		
	//$_GET['daterange'] =	"02-02-2016|02-02-2016";

	$consolidated["reports"] 		= ($permissionList['view_scheduled_report'] == 'enabled' && $prefList->view_scheduled_report) ? getScheduledReportDaywise($_REQUEST['startDate'],$_REQUEST['endDate']) : array();
	$consolidated["orders"] 		= ($permissionList['commerce'] == true  && $permissionList['view_orders'] == 'enabled' && $prefList->view_orders) ? getOrdersForAdminCalendar() : array();
	$consolidated["locations"] 		= ($permissionList['view_locations'] == 'enabled'  && $prefList->view_locations) ? getOccupiedLocationList($_REQUEST['startDate'],$_REQUEST['startDate']) : array();
	$consolidated["discussions"] 	= ($permissionList['forum'] == true  && $permissionList['view_discussions'] == 'enabled' && $prefList->view_discussions) ? getDiscussions() : array();
	$consolidated["instructors"] 	= ($permissionList['view_instructors'] == 'enabled'  && $prefList->view_instructors) ? getOccupiedInstructorsForList($_REQUEST['startDate']) : array();//

	
	$sorteddata = array();
	for($i = 0; $i < count($consolidated["reports"]); $i++){
		$consolidated["reports"][$i]->title = sanitize_data($consolidated["reports"][$i]->title);
		$sorteddata[] = $consolidated["reports"][$i];
	}
	for($i = 0; $i < count($consolidated["orders"]); $i++){
		$consolidated["orders"][$i]->title = sanitize_data($consolidated["orders"][$i]->title);
		$sorteddata[] = $consolidated["orders"][$i];
	}
	for($i = 0; $i < count($consolidated["locations"]); $i++){
		$consolidated["locations"][$i]->title = sanitize_data($consolidated["locations"][$i]->title);
		$sorteddata[] = $consolidated["locations"][$i];
	}
	for($i = 0; $i < count($consolidated["discussions"]); $i++){
		$consolidated["discussions"][$i]->title = sanitize_data($consolidated["discussions"][$i]->title);
		$sorteddata[] = $consolidated["discussions"][$i];
	}
	for($i = 0; $i < count($consolidated["instructors"]); $i++){
		$consolidated["instructors"][$i]->title = sanitize_data($consolidated["instructors"][$i]->title);
		$sorteddata[] = $consolidated["instructors"][$i];
	}


	usort($sorteddata,"ac_events_sort");
	
	echo drupal_json_encode($sorteddata);
	drupal_exit();
	 } catch (Exception $e) {
        expertusErrorThrow($e);
    }


}
//Added by vetrivel.P for #0064978

function locationDisplay($search_string){
	try{
		$loc_or_comma = db_or()->condition('loc.name', '%' . db_like($search_string) . '%', 'LIKE')
		->condition('loc.addr1', '%' . db_like($search_string) . '%', 'LIKE')
		->condition('loc.addr2', '%' . db_like($search_string) . '%', 'LIKE')
		->condition('loc.city', '%' . db_like($search_string) . '%', 'LIKE')
		->condition('loc_stats.state_name', '%' . db_like($search_string) . '%', 'LIKE')
		->condition('loc_ctry.country_name', '%' . db_like($search_string) . '%', 'LIKE')
		->condition('loc.zipcode', '%' . db_like($search_string) . '%', 'LIKE')
		->where('concat_ws(", ",loc.name,loc.addr1,loc.addr2,loc.city,loc.zipcode,loc_stats.state_name,loc_ctry.country_name) LIKE "%'. db_like($search_string).'%"'." ESCAPE '\\\'");

	/* $loc_split_arr=array();
	if(strpos($search_string, ',') !== false){
		$loc_split_arr=explode(',',$search_string);
		$loc_split_arr = array_map('trim',$loc_split_arr);
		if(isset($loc_split_arr) && !empty($loc_split_arr) && count($loc_split_arr)>0){ //Full Location Search
			$remove_comma_from_loc_str=addslashes(implode('',$loc_split_arr));
			$tmp_full_loc_cond="replace(concat_ws('',loc.name,loc.addr1,loc.addr2,loc.city,loc.zipcode,loc_stats.state_name,loc_ctry.country_name),',','') like \"%$remove_comma_from_loc_str%\"";
		}
		$loc_or_comma = db_and()->where($tmp_full_loc_cond);
	}else{
		$name = $addr1 = $addr2 = $city = $state_name = $country = $zipcode = $search_string;
		$loc_or_comma = db_or()->condition('loc.name', '%' . db_like($name) . '%', 'LIKE')
		->condition('loc.addr1', '%' . db_like($addr1) . '%', 'LIKE')
		->condition('loc.addr2', '%' . db_like($addr2) . '%', 'LIKE')
		->condition('loc.city', '%' . db_like($city) . '%', 'LIKE')
		->condition('loc_stats.state_name', '%' . db_like($state_name) . '%', 'LIKE')
		->condition('loc_ctry.country_name', '%' . db_like($country) . '%', 'LIKE')
		->condition('loc.zipcode', '%' . db_like($zipcode) . '%', 'LIKE');
	}*/
	return $loc_or_comma;
	} catch (Exception $e) {
		expertusErrorThrow($e);
	}
	
}

function getOccupiedLocations($startdate,$enddate,$search_text = "",$autocomplete = false,$from=false) {
	try{
		
	$locations = array();
	$Url = $_REQUEST['q'];
	
	$select = db_select('slt_master_search', 'sltm');
	$select->innerJoin('slt_location',  'loc', 'sltm.pk_id = loc.id AND sltm.type = \'Location\'');
	$select->innerJoin('slt_course_class_session','sess','loc.id = sess.location_id');
	$select->leftJoin('slt_country','loc_ctry','loc_ctry.country_code=loc.country');
	$select->leftJoin('slt_state','loc_stats','loc_stats.state_code = loc.state and loc_stats.country_code=loc.country');
	$select->addExpression('loc.id', 'id');
	$select->addExpression('loc.name', 'name');
	$select->addExpression('loc.addr1', 'addr1');
	$select->addExpression('loc.addr2', 'addr2');
	$select->addExpression('loc.city', 'city');
	$select->addExpression('loc.zipcode', 'zipcode');
	$select->addExpression('loc.timezone', 'location_timezone');
	$select->addExpression('loc_stats.state_name', 'state_name');
	$select->addExpression('loc_ctry.country_name', 'country_name');
	$select->addExpression('sess.start_date', 'start_date');
	$select->addExpression('sess.start_time', 'start_time');
	$select->addExpression('sess.end_time', 'end_time');
	$select->condition('loc.status', 'lrn_res_loc_atv', '=');
	if($startdate!='' && $enddate!=''){
		$select->condition('sess.start_date', $startdate, '>=');
		$select->condition('sess.start_date', $enddate, '<=');
	}
	filterByUserAccessPrivileges($select, 'loc','',1,'cre_sys_obt_loc','location','loc');
	
if(($autocomplete == true && $Url != "administration/calendarviewapi")){
	
	if($search_text != "" && (strtolower($search_text) != strtolower(t('LBL304')))){
		$loc_or_condition = db_or()->condition('loc.name', '%' . db_like($search_text) . '%', 'LIKE')
									->condition('loc.addr1', '%' . db_like($search_text) . '%', 'LIKE')
									->condition('loc.addr2', '%' . db_like($search_text) . '%', 'LIKE')
									->condition('loc.city', '%' . db_like($search_text) . '%', 'LIKE')
									->condition('loc_stats.state_name', '%' . db_like($search_text) . '%', 'LIKE')
									->condition('loc_ctry.country_name', '%' . db_like($search_text) . '%', 'LIKE')
									->condition('loc.zipcode', '%' . db_like($search_text) . '%', 'LIKE');
		$select->condition($loc_or_condition);
	}
	$select->groupBy('loc.id');
	expDebug::dPrintDBAPI('fetchLOCATION------------->',$select);
	$occupiedLocations = $select->execute()->fetchAll();
	
		foreach($occupiedLocations as $rec){
			//Added if conduction for #0072579
			$name = ($rec->name.', ');
			if(!empty($rec->addr1)){$name .=$rec->addr1.', ';}
			if(!empty($rec->addr2)){$name .=$rec->addr2.', ';}
			if(!empty($rec->city)){$name .=$rec->city.', ';}
			if(!empty($rec->zipcode)){$name .=$rec->zipcode.', ';}
			if(!empty($rec->state_name)){$name .=$rec->state_name.', ';}
			if(!empty($rec->country_name)){$name .=$rec->country_name;}
			$row = new stdClass();
			$row->title = sanitize_data($name);
			if(!empty($rec->start_date)){
				$row->start = date("Y-m-d",strtotime($rec->start_date));
				$row->startTime = date("H:i",strtotime($rec->start_time));
				$row->end = date("Y-m-d",strtotime($rec->start_date));
				$row->endTime = date("H:i",strtotime($rec->end_time));
			}else{
				$row->start = '';
				$row->startTime = '';
				$row->end ='';
				$row->endTime = '';
			}
			$row->type ="location";
			$row->id = "location-".$rec->id;
			$row->allDay = false;
			$locations[]=$row;
		}
	return $locations;
}else
	{
		$search_type = $_REQUEST['search_type'];
		if($from  || ($search_text!='' && (strtolower($search_text) != strtolower(t('LBL304'))) && ($search_type  == "view_locations" || $search_type == "All" ))){
			if($search_text!='' && (strtolower($search_text) != strtolower(t('LBL304')))){
				$value = locationDisplay($search_text);
				$select->condition($value);
			}
			expDebug::dPrintDBAPI('fetchLOCATION ELSE------------->',$select);
			$occupiedLocations = $select->execute()->fetchAll();
		
			foreach($occupiedLocations as $rec)
			{
				$row = new stdClass();
				$row->title = sanitize_data($rec->name);
			
				if(!empty($rec->start_date)){
					$row->start = date("Y-m-d",strtotime($rec->start_date));
					$row->startTime = date("H:i",strtotime($rec->start_time));
					$row->end = date("Y-m-d",strtotime($rec->start_date));
					$row->endTime = date("H:i",strtotime($rec->end_time));
				}else{
					$row->start = '';
					$row->startTime = '';
					$row->end ='';
					$row->endTime = '';
				}
			
				$row->type ="location";
				$row->id = "location-".$rec->id;
				$row->allDay = false;
				$vSesTimeZone = $rec->location_timezone;
					$tzArray = getProfileListItemByCode($vSesTimeZone);
					$time_zone = $tzArray[0]->attr1;
					$row->timezone = $time_zone;
			
				$locations[]=$row;
			}
		}
	}
	return $locations;
	 } catch (Exception $e) {
        expertusErrorThrow($e);
    }

}

function getOccupiedLocationList($startdate,$enddate) {
	try
	{
		$startdate 	= $startdate." 00:00:00";
		$enddate 	= $enddate." 23:59:59";
		$locSearch = "";
	
		$select = db_select('slt_master_search', 'sltm');
		$select->innerJoin('slt_location',  'loc', 'sltm.pk_id = loc.id AND sltm.type = \'Location\'');
		$select->innerJoin('slt_course_class_session', 'sess','loc.id = sess.location_id');
		$select->leftJoin('slt_country','loc_ctry','loc_ctry.country_code=loc.country');
		$select->leftJoin('slt_state','loc_stats','loc_stats.state_code = loc.state and loc_stats.country_code=loc.country');
		$select->addField('loc', 'id', 'id');
		$select->addField('loc', 'name', 'name');
		$select->addField('sess', 'start_date', 'start_date');
		$select->addField('sess', 'start_time', 'start_time');
		$select->addField('sess', 'end_time', 'end_time');
		$select->condition('sess.start_date', $startdate, '>=');
		$select->condition('sess.start_date', $enddate, '<=');
		$select->where('sess.location_id = loc.id');
		filterByUserAccessPrivileges($select, 'loc','',1,'cre_sys_obt_loc','location','loc');
		expDebug::dPrintDBAPI('fetchLOCATION------------->',$select);
		$search_text = $_REQUEST['search_text'];
		$search_type = $_REQUEST['search_type'];
		if($search_text != "" && (strtolower($search_text) != strtolower(t('LBL304'))) && ($search_type  == "view_locations" || $search_type == "All" )){
			if($search_text!='' && (strtolower($search_text) != strtolower(t('LBL304')))){
				$value = locationDisplay($search_text);
				$select->condition($value);
			}
		}
		$occupiedLocation = $select->execute()->fetchAll();
		$locations = array();
		foreach($occupiedLocation as $rec)
		{
			$row = new stdClass();
			$row->title = $rec->name;
			$row->start = date("Y-m-d",strtotime($rec->start_date));
			$row->startTime = date("H:i",strtotime($rec->start_time));
			$row->end = date("Y-m-d",strtotime($rec->start_date));
			$row->endTime = date("H:i",strtotime($rec->end_time));
			$row->type ="location";
			$row->id = "location-".$rec->id;
			$row->allDay = false;
			$locations[]=$row;
	
		}
	
		return $locations;
	} catch (Exception $e) {
		expertusErrorThrow($e);
	}
}
//End of #0064978 by vetrivel.P
function getScheduledReportDaywise($startdate,$enddate,$search_text = '',$autocomplete = false)
{
	try
	{
		//expDebug::dPrint("report schedule start ".$startdate,5);
		//expDebug::dPrint("report schedule end date ".$enddate,5);
		global $user;
		$dayWise = false;
		if($enddate == null || $enddate == "")
		{
			$enddate = $startdate;
			$dayWise =true;
		}
		else if($startdate == $enddate)
		{
			$dayWise = true;
		}
		$userId = getSltpersonUserId();
		if(!is_data_admin($userId) && $userId!=1) {
			$is_mgr = is_manager($userId);
			$is_ins = is_instructor($userId);
			$utype = $is_mgr?($is_ins?"manager,instructor":"manager"):($is_ins?"instructor":'');
			$grpIds = getGroupsDetails($userId,'','',$utype,0);
			expDebug::dPrint('$grpIds' . print_r($grpIds, 1));
		}
		$select = db_select('slt_report_schedules', 's');
		$select->join('slt_report_details', 'rpt', 'rpt.id = s.report_id');
		$select->addExpression('DATE_FORMAT(s.next_run, \'%Y-%m-%d\')', 'next_run');
		$select->addExpression('DATE_FORMAT(s.created_on, \'%Y-%m-%d\')', 'created_on');
		$select->addExpression('DATE_FORMAT(s.freq_time, \'%H:%i\')', 'next_runtime');
		$select->addField('rpt', 'id', 'id');
		$select->addField('rpt', 'title', 'title');
		$select->addField('s', 'report_id', 'report_id');
		$select->addField('s', 'frequency', 'frequency');
		$select->addField('s', 'freq_month', 'freq_month');
		$select->addField('s', 'freq_day', 'freq_day');
		$select->addField('s', 'id', 'scheduled_id');
		$select->addField('s', 'status', 'status');
		$select->condition('rpt.status', array('cre_rpt_rps_del'), 'NOT IN');
		$select->condition('s.status', array('cre_rpt_rss_del'), 'NOT IN');
		if(!is_data_admin($userId) && $userId!=1) {
			$orSelect = db_or();
			$orSelect->where('rpt.created_by = '.$userId);
			if(!empty($grpIds)){
				foreach($grpIds as $key => $gId){
					$orSelect->where('FIND_IN_SET('.$gId->id.',rpt.report_visibility)');
				}
				$select->condition($orSelect);
				expDebug::dPrint('$$orQuery  : '.print_r($orQuery,1), 4);
			}
		}	
		if($autocomplete)
		{
			if((strtolower($search_text) != strtolower(t('LBL304'))))
			$select->condition('rpt.title', '%' . db_like($search_text) . '%', 'LIKE');
		}
		else
		{
			if($search_text != null && $search_text != "" && (strtolower($search_text) != strtolower(t('LBL304'))))
			{
				$select->condition('rpt.title', '%' . db_like($search_text) . '%', 'LIKE');
			}
			// Commented for ticket no - 0062971
			//	 if($dayWise)
			if(($_REQUEST['narrowcaldaywiseshort'] == true  || $dayWise )){
				$select->where('(CASE WHEN(s.frequency = \'daily\') THEN
								(1=1)
								WHEN s.frequency = \'weekly\' THEN
								(DAYOFWEEK(\''.$startdate.'\') = s.freq_day)
								WHEN s.frequency = \'monthly\' THEN
								(EXTRACT(DAY FROM \''.$startdate.'\')) = s.freq_day
								ELSE
								(EXTRACT(MONTH  FROM \''.$startdate.'\')) = s.freq_month AND (EXTRACT(DAY FROM \''.$startdate.'\')) = s.freq_day
								END)');
				$select->where('DATE_FORMAT(s.created_on, \'%Y-%m-%d\') <= \''.$startdate.'\'');
			}
		}
		$select->orderBy('DATE_FORMAT(s.freq_time, \'%H:%i\')');
		expDebug::dPrintDBAPI(' $getScheduledReportDaywise obj = ' , $select);
		//$scheduledReports = db_query($sql, array(':startdate' => $startdate,':enddate' => $enddate))->fetchAll();
		$scheduledReports =  $select->execute()->fetchAll();
		expDebug::dPrint("schedule report full ".print_r($scheduledReports,true),5);
		$reports = array();
		$yearsplit1 = explode('-',$startdate);
		$yearsplit2 = explode('-',$enddate);
		
		$limitresults = limitACSearchResults();
		for($i = 0; $i < count($scheduledReports); $i++)
		{
			$row = new stdClass();
		$report = $scheduledReports[$i];
		$dt1 =  date('m-d-Y',strtotime($scheduledReports->created_on));
		$dt2 =  date('m-d-Y',strtotime($scheduledReports[$i+1]->created_on));
		if($dt1 == $dt2 &&  $limitresults->evtCntPerDay != 0 && $limitresults->evtCntPerDay == $limitresults->evtLimit)
		{
			continue;
		}
		if($dt1 == $dt2)
		{
			$limitresults->evtCntPerDay++;
		}
		else
		{
			$limitresults->evtCntPerDay = 0;
		}
		
			$row->title = (empty($report->title) || $report->title == null) ? '[Draft]' : $report->title;
			$row->editable = user_access('Create Report Perm');
			$row->start = $report->created_on."T".date("H:i:s",strtotime($report->next_runtime));
			$row->startTime = $report->next_runtime;
			$row->type ="report";
			$row->id = "report-".$report->scheduled_id;
			$row->scheduled_id = $report->scheduled_id;
			$row->allDay = false;
			$row->report_id = $report->report_id;
			$row->status = ($report->status == 'cre_rpt_rss_atv') ? t("Active") : t("Suspended");
			$row->frequency = $report->frequency;
			if($report->frequency == "daily"){
				$row->allDay = true;
				$row->end = $_REQUEST['endDate'];
				$reports[]=$row;
			}
			else if($report->frequency == "weekly"){
				$row->allDay = true;
				$row->dow = array($report->freq_day-1);
				$row->created = $report->created_on;
				$row->end = $_REQUEST['endDate'];
				$reports[]=$row;
			}else if($report->frequency == "monthly"){
				$sched_day1 = (checkdate($yearsplit1[1],$report->freq_day,$yearsplit1[0])) ? 
						date_format(date_create($yearsplit1[0].'-'.$yearsplit1[1].'-'.$report->freq_day),"Y-m-d") : '';
				$next_month = date('Y-m',strtotime($startdate . "+1 months"));
				$next_split = explode('-',$next_month);
				$sched_day2 = (checkdate($next_split[1],$report->freq_day,$next_split[0])) ? 
				        date_format(date_create($next_month.'-'.$report->freq_day),"Y-m-d") : '';
				        
				$after_next = date('Y-m',strtotime($startdate . "+2 months"));
				$after_next_split = explode('-',$after_next);
				$sched_day3 = (checkdate($after_next_split[1],$report->freq_day,$after_next_split[0])) ? 
				        date_format(date_create($after_next.'-'.$report->freq_day),"Y-m-d") : '';	
				        			        
				if(!empty($sched_day1)&&($sched_day1 >= $startdate) && ($sched_day1 <= $enddate) && ($sched_day1 >= $report->created_on)){
					$row->allDay = true;
					$row->start =$sched_day1;
					$row->end =$sched_day1;
					$reports[] = $row;
				}
				if(!empty($sched_day2)&&($sched_day2 >= $startdate) && ($sched_day2 <= $enddate) && ($sched_day2 >= $report->created_on)){
					$row2 = new stdClass();
					$row2->title = $report->title;
					$row2->editable = user_access('Create Report Perm');
					$row2->start =$sched_day2;
					$row2->startTime = $report->next_runtime;
					$row2->type ="report";
					$row2->id = "report-".$report->scheduled_id;
					$row2->scheduled_id = $report->scheduled_id;
					$row2->allDay = true;
					$row2->end =$sched_day2;
					$row2->report_id = $report->report_id;
					$reports[] = $row2;
				}
			if(!empty($sched_day3)&&($sched_day3 >= $startdate) && ($sched_day3 <= $enddate) && ($sched_day3 >= $report->created_on) ){
					$row3 = new stdClass();
					$row3->title = $report->title;
					$row3->editable = user_access('Create Report Perm');
					$row3->start =$sched_day3;
					$row3->startTime = $report->next_runtime;
					$row3->type ="report";
					$row3->id = "report-".$report->scheduled_id;
					$row3->scheduled_id = $report->scheduled_id;
					$row3->allDay = true;
					$row3->end =$sched_day3;
					$row3->report_id = $report->report_id;
					$reports[] = $row3;
				}
				
			}else if($report->frequency == "yearly"){
				$row->allDay = true;
				if($yearsplit1[0] == $yearsplit2[0]){
					$sched_day = (checkdate($report->freq_month,$report->freq_day,$yearsplit1[0])) ?  
					date_format(date_create($yearsplit1[0].'-'.$report->freq_month.'-'.$report->freq_day),"Y-m-d") : '';
					if(!empty($sched_day)&&($sched_day >= $startdate) && ($sched_day <= $enddate)){
						$row->end = $sched_day;
						$row->start = $row->end;
					} 
					
				}else{
					$sched_day1 = (checkdate($report->freq_month,$report->freq_day,$yearsplit1[0])) ?  
					date_format(date_create($yearsplit1[0].'-'.$report->freq_month.'-'.$report->freq_day),"Y-m-d") : '';
					$sched_day2 = (checkdate($report->freq_month,$report->freq_day,$yearsplit1[0])) ?  
					date_format(date_create($yearsplit2[0].'-'.$report->freq_month.'-'.$report->freq_day),"Y-m-d") : '';
					$sched_day = !empty($sched_day1)? $sched_day1 : (!empty($sched_day2) ? $sched_day2 : '');
					if(!empty($sched_day)&&($sched_day >= $startdate) && ($sched_day <= $enddate) && ($sched_day >= $report->created_on) ){
						$row->end = $sched_day;
						$row->start = $row->end;
					}
				}
				$reports[]=$row;
			}else{
				$row->allDay = false;
				$reports[]=$row;
			}

		}
			
		expDebug::dPrint("final report schedule ".print_r($reports,true),5);
		return $reports;
	} catch (Exception $e) {
        expertusErrorThrow($e);
    }


}

function getReportsForCalendar()
{
	try
	{
		include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrreports/exp_sp_lnrreports.inc";
		$manageBy = 'both';
		$adminAccess = reportAdminAccess();
		$type = $_GET['calendar_searchtype'];
		if($type=='view_scheduled_report')
			$_GET['report_name'] = $_GET['z'];
		$query = getLearnerReportsDetailsQuery($adminAccess, '',$manageBy,'event');
		// Execute query
		$result = $query->execute()->fetchAll();
		if($type=='report' || $type == "view_scheduled_report")
			return $result;
		//$sql = "select rpt.id,rpt.title from {slt_report_details} rpt";// limit 5";
		//$result = db_query($sql)->fetchAll()
		echo drupal_json_encode($result);
		drupal_exit();
	 } catch (Exception $e) {
        expertusErrorThrow($e);
    }

}

function getDiscussions($printorexport = '')
{
	try
	{
	include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_collaboration/exp_sp_collaboration_learner/modules/exp_sp_forum/exp_sp_forum.inc";
    
    $lnrSrch = new ForumDetails();
    $val = empty($printorexport) ? 'list' : $printorexport;
    $resultQry = $lnrSrch -> forumSearchQuery($val);
    
    $result = $resultQry->execute()->fetchAll();
    expDebug::dPrintDBAPI("Query for discussion",$resultQry);
		return $result;
	 } catch (Exception $e) {
      expertusErrorThrow($e);
   }
}	

function getOrdersForAdminCalendar($autocomplete = false)
{
 	try
 	{
 	if($autocomplete)
 	{
 	if(isset($_GET['calendar_searchtype']) && !empty($_GET['calendar_searchtype'])){
 		$_GET['textfilter'] = $_GET['z'];
 		}
 	}
 	else
 	{
 	 	if(isset($_GET['calendar_searchtype']) && !empty($_GET['calendar_searchtype']))
 			$_GET['textfilter'] = $_GET['z'];
 				
 		$st = "";
 		$ed = "";
 		if(isset($_REQUEST['startDate']))
 			$st =  date('m-d-Y',strtotime($_REQUEST['startDate']));
 		
 		
 		if(isset($_REQUEST['endDate']) && $_REQUEST['endDate'] != "")
 			$ed =  date('m-d-Y',strtotime($_REQUEST['endDate']));
 		else
 			$ed = $st;//$_REQUEST['endDate'];
 		
 		
 		$_GET['daterange'] = $st."|".$ed;
 	}
	$_GET['rows'] = 100;
	include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_commerce/exp_sp_administration_order/exp_sp_administration_order.inc";
	$orderList = sendManagerOrderList();
	expDebug::dPrint('ORDER STATUS LIST---->'.print_r($orderList,1),4);
	//{"slt_order_id":"103","uc_order_id":"103","order_status":"completed","order_total":"16.09000","currency_type":"USD","currency_symbol":"$","product_count":"1","purchase_date":"2015-12-25 13:56:15","discount_amount":"1.09000","lastname":"testuser25","firstname":"testuser25","username":"testuser25"},
	$orders = array();
	
	$limitresults = limitACSearchResults();

	for($i = 0; $i < count($orderList); $i++)
	{
		$order = $orderList[$i];
		$dt1 =  date('m-d-Y',strtotime($order->purchase_date));
		$dt2 =  date('m-d-Y',strtotime($orderList[$i+1]->purchase_date));
		if($dt1 == $dt2 &&  $limitresults->evtCntPerDay != 0 && $limitresults->evtCntPerDay == $limitresults->evtLimit)
		{
			continue;
		}
		if($dt1 == $dt2)
		{
			$limitresults->evtCntPerDay++;
		}
		else
		{
			$limitresults->evtCntPerDay = 0;
		}
		$row = new stdClass();
		$row->id = "order-".$order->slt_order_id;
		$row->slt_order_id = $order->slt_order_id;
		$row->uc_order_id = $order->uc_order_id;
		if(strpos(round(abs($order->price), 2),".") !== false){
    		$row->price = round(abs($order->price), 2);
		}else{
    		$row->price = round(abs($order->price), 2).'.00';
		}	
		
		$row->currency_type = $order->currency_type;
		$row->currency_symbol = $order->currency_symbol;
		$row->order_status = $order->order_status;
		if($order->order_status == 'payment_received' || $order->order_status == 'completed')
			$row->lan_order_status = t("Payment Confirmed");
		else if($order->order_status == 'canceled')
			$row->lan_order_status = t("Payment Rejected");
		else if($order->order_status == 'pending')
			$row->lan_order_status = t("Payment Pending");
			
		$row->title = "Order by ".$order->username; //.' - '. $order->slt_order_id;
		$row->start = $order->purchase_date;
		$row->startTime = strtoupper(date('H:i' , strtotime($order->purchase_date)));



		$isNotCancellable = 0;
		if( $order->order_status=='payment_received')
			$isNotCancellable = getCancelStatusFromSltOrderId($order->slt_order_id);
		$row->complete_count = $isNotCancellable;
		if(strlen($row->startTime) == 7)
		  $row->startTime = "0".$row->startTime;
		
		$row->start = date('Y-m-d',strtotime($order->purchase_date))."T".strtoupper(date('H:i:s',strtotime($order->purchase_date)));
		//62985: In week view,more pop up is getting displayed incorrectly
		$row->end = date('Y-m-d' , strtotime($order->purchase_date))."T".strtoupper(date('H:i:s' , strtotime($order->purchase_date." +15 minutes")));
		//$row->endTime = strtoupper(date('h:i a' , strtotime($order->purchase_date." +30 minutes")));

		//$row->color = "#00FF00";
		$row->type = 'order';
		$orders[] = $row;
	}
	expDebug::dPrint('ORDER STATUS LIST---->'.print_r($orders,1),4);
	return $orders;
	 } catch (Exception $e) {
        expertusErrorThrow($e);
    }

	
}
function getSearchCalendarAutoComplete(){
	$type = $_GET['calendar_searchtype'];
	
	$_REQUEST["search_type"] = $type;
	$_REQUEST["search_text"] = $_GET['z'];

	$autocomplete = true;
 	$result =  drupal_json_decode(getCalendarViewDataProcess($autocomplete)); //echo  drupal_json_encode($consolidated);
 //	echo count($result);
	foreach ($result as $val){
		//	echo $val->title."\n";
			print drupal_json_encode($val) . "\n";
	}
}
function setQueryMaxTime(){
	$config=getConfig('exp_sp');
	$admin_calendar_search_max_time = (!empty($config['admin_calendar_search_max_time'])) ? ($config['admin_calendar_search_max_time']*1000) : 29000;
	db_query('SET SESSION MAX_EXECUTION_TIME='.$admin_calendar_search_max_time)->execute();
}
function getScheduledClassesForAdminCalendar($from='',$to='',$search_string='',$autocomplete = false,$search_type)
{
	try
	{
		$userId = getSltpersonUserId();
//		$maxExecutionTime = (1 * 1000);
// 		$rqry = 'INSERT INTO slt_web_report_running_query (report_id, user_id, session_id, pid, started_on)
// 									VALUES ( 500 , '.$userId.' , "'.session_id().'" , CONNECTION_ID() , now())';
// 		expDebug::dPrintDBAPI("TO CAPTURE LONG RUNNING QUERY  -- ",$rqry);
// 		$pid =db_query($rqry)->execute();
//		$setmax = db_query('SET SESSION MAX_EXECUTION_TIME=29000')->execute();
		setQueryMaxTime();
		$startdate = $_REQUEST['startDate'];
		$endDate = $_REQUEST['endDate'];
		$daywise = false;
		$result = array();
		if($startdate == $endDate || (!isset($_REQUEST['endDate']) || $_REQUEST['endDate'] ==''))
			$daywise = true;
		$Url = $_REQUEST['q'];
		$result = "";
		$select = db_select('slt_course_class_session', 'sess');
		$select->leftJoin('slt_location','loc','loc.id = sess.location_id');
		$select->leftJoin('slt_course_class','crs','crs.id = sess.class_id and crs.delivery_type IN (\'lrn_cls_dty_ilt\',\'lrn_cls_dty_vcl\')');
		$select->leftJoin('slt_profile_list_items','lst','lst.code = crs.lang_code');
		$select->leftJoin('slt_session_instructor_details','ins','sess.id = ins.session_id');
		$select->leftJoin('slt_person','per','per.id = ins.instructor_id');
		if($search_string!='' && (strtolower($search_string) != strtolower(t('LBL304')))){
			$select->leftJoin('slt_country','loc_ctry','loc_ctry.country_code=loc.country');
			$select->leftJoin('slt_state','loc_stats','loc_stats.state_code = loc.state and loc_stats.country_code=loc.country');
			$select->leftJoin('slt_tag_entity','te','((crs.id=te.entity_id and te.entity_type=\'Class\'))');
			$select->leftJoin('slt_tagdefn','td','te.tagid= td.id');
			$select->leftJoin('slt_tag_entity','te1','((crs.course_id=te1.entity_id and te1.entity_type=\'Course\'))');
			$select->leftJoin('slt_tagdefn','td1','te1.tagid= td1.id');
		}
		$select->addField('sess', 'location_id', 'location_id');
		$select->addField('sess', 'timezone', 'timezone');
		$select->addField('sess', 'id', 'sessionid');
		$select->addField('sess', 'title', 'session_title');
		$select->addField('sess', 'course_id', 'course_id');
		$select->addField('sess', 'class_id', 'class_id');
		$select->addField('sess', 'start_date', 'start_date');
		$select->addField('sess', 'end_date', 'end_date');
		//$select->addField('sess', 'start_time', 'start_time');
		$select->addExpression('TIME_FORMAT(sess.start_time,"%H:%i")', 'start_time');
		$select->addField('sess', 'end_time', 'end_time');
		$select->addField('crs', 'delivery_type', 'delivery_type');
		$select->addField('crs', 'lang_code', 'lang_code');
		$select->addField('crs', 'title', 'title');
		$select->addField('crs', 'status', 'status');
		$select->addField('crs', 'code', 'code');
		$select->addField('crs', 'scheduled_duration', 'scheduled_duration');
		//$select->addField('crs', 'registration_end_on', 'registration_end_on');
		$select->addField('loc', 'name', 'location_name');
		$select->addField('loc', 'timezone', 'location_timezone');
		$select->addField('lst', 'name', 'language_name');
		//$select->addField('td','tagname', 'tagname');
		$select->addExpression('ifnull(GROUP_CONCAT(distinct per.full_name SEPARATOR \', \'),\'\')','instructor');
		/*$select->addExpression('CASE WHEN (DATEDIFF(CONCAT(DATE_FORMAT(sess.start_date,"%Y-%m-%d")," ",sess.end_time),DATE_FORMAT(NOW(),"%Y-%m-%d %H:%i:%s"))) = 0 
					THEN if(TIMESTAMPDIFF(MINUTE,DATE_FORMAT(NOW(),"%Y-%m-%d %H:%i:%s"),CONCAT(DATE_FORMAT(sess.start_date,"%Y-%m-%d")," ",sess.end_time))<0,\'fc-past\',\'fc-today\')
					WHEN (DATEDIFF(CONCAT(DATE_FORMAT(sess.start_date,"%Y-%m-%d")," ",sess.end_time),DATE_FORMAT(NOW(),"%Y-%m-%d %H:%i:%s")))>0 THEN \'fc-future\'
					ELSE
					\'fc-past\'
					END', 'datatype'); */
		$select->addExpression('CASE WHEN (DATEDIFF(CONCAT(DATE_FORMAT(sess.start_date,\'%Y-%m-%d\'),\' \',sess.end_time),DATE_FORMAT(NOW(),\'%Y-%m-%d %H:%i:%s\'))) = 0
				THEN if(TIMESTAMPDIFF(MINUTE,DATE_FORMAT(NOW(),\'%Y-%m-%d %H:%i:%s\'),CONCAT(DATE_FORMAT(sess.start_date,\'%Y-%m-%d\'),\' \',sess.end_time))<0,\'fc-past\',\'fc-today\')
				WHEN (DATEDIFF(CONCAT(DATE_FORMAT(sess.start_date,\'%Y-%m-%d\'),\' \',sess.end_time),DATE_FORMAT(NOW(),\'%Y-%m-%d %H:%i:%s\')))>0 THEN \'fc-future\'
				ELSE
				\'fc-past\'
				END', 'datatype');
		if($autocomplete && $Url != "administration/calendarviewapi")
		{
			if($search_string!='' && (strtolower($search_string) != strtolower(t('LBL304')))){
				$select->condition(db_or()
				->condition('crs.title', '%' . db_like($search_string) . '%', 'LIKE')
				->condition('td.tagname', '%' . db_like($search_string) . '%', 'LIKE')
				->condition('td1.tagname', '%' . db_like($search_string) . '%', 'LIKE')
				);			
			}
		}
		else{	
			//Changed by vetrivel.P for #0064978
			if(($search_type == 'view_locations' && $search_type != 'view_classroom') && $search_string!='' && (strtolower($search_string) != strtolower(t('LBL304')))){
				if($search_string!='' && (strtolower($search_string) != strtolower(t('LBL304')))){
					$value = locationDisplay($search_string);
					$select->condition($value);
				}
			}elseif(($search_type == 'view_instructors' && $search_type != 'view_classroom') && $search_string!='' && (strtolower($search_string) != strtolower(t('LBL304')))){
				$select->condition(db_or()
						->condition('per.full_name', '%' . db_like($search_string) . '%', 'LIKE')
						->condition('per.user_name', '%' . db_like($search_string) . '%', 'LIKE')
						->condition('per.last_name', '%' . db_like($search_string) . '%', 'LIKE')
				);
			}elseif ($search_type == 'view_classroom' && $search_string!='' && (strtolower($search_string) != strtolower(t('LBL304')))){
				$select->condition(db_or()
				->condition('crs.title', '%' . db_like($search_string) . '%', 'LIKE')
				->condition('td.tagname', '%' . db_like($search_string) . '%', 'LIKE')
				->condition('td1.tagname', '%' . db_like($search_string) . '%', 'LIKE')
				);
			}elseif ($search_type == 'All' && $search_string!='' && (strtolower($search_string) != strtolower(t('LBL304')))){
				$value = locationDisplay($search_string);
				$select->condition(db_or()->condition($value)
						->condition('per.full_name', '%' . db_like($search_string) . '%', 'LIKE')
						->condition('per.user_name', '%' . db_like($search_string) . '%', 'LIKE')
						->condition('per.last_name', '%' . db_like($search_string) . '%', 'LIKE')
						->condition('crs.title', '%' . db_like($search_string) . '%', 'LIKE')
						->condition('td.tagname', '%' . db_like($search_string) . '%', 'LIKE')
						->condition('td1.tagname', '%' . db_like($search_string) . '%', 'LIKE')
				);
			}
			//if($autocomplete && $Url != "administration/calendarviewapi"){
				if ($from == $to && $from!='') {
					$select->where("DATE_FORMAT(sess.start_date, '%Y-%m-%d') = "."'".$from."'");
				} elseif($from!='' && $to!='') {
					$select->where("DATE_FORMAT(sess.start_date, '%Y-%m-%d') >= DATE_FORMAT('".$from."', '%Y-%m-%d')"); // $to ,'<='
					$select->where("DATE_FORMAT(sess.start_date, '%Y-%m-%d') <= "."'".$to."'");
				}
			//}
		}
		$select->condition('crs.status', array('lrn_cls_sts_del','lrn_cls_sts_dft'),'NOT IN');
		$select->groupBy('sess.id');
		if ($userId != 1) // skip privilege check for global admin
			filterByUserAccessPrivileges($select, 'crs', $userId, 1,'cre_sys_obt_cls','Class','crs');
		if($daywise==true){
			$select->orderBy('TIME_FORMAT(sess.start_time,"%H:%i")');
			$select->orderBy('sess.title');
		}
		else
			$select->orderBy('sess.start_date,TIME_FORMAT(sess.start_time,"%H:%i")');
		expDebug::dPrintDBAPI(' $getScheduledClassesForAdminCalendar obj = ' , $select);
		$result =  $select->execute()->fetchAll();
		expDebug::dPrint('getScheduledClassesForAdminCalendar result = ' . print_r($result, 1));
		return $result;
	} catch (Exception $exception) {
		$expErrorMsg = $exception->getMessage();
		//Added for #0070534
		if (strpos($expErrorMsg, 'maximum statement execution time exceeded') > 0 ) {
			$expErrorMsg = t('ERR255');
			expDebug::dPrint("Error in Query execution ".$expErrorMsg. " ---- ".$callFrom);
			return FALSE;
		}else if (strpos($expErrorMsg, 'MySQL server has gone away') > 0 ) {
			$expErrorMsg = t('ERR170');
		}else if (!empty($exception->errorInfo[2])) {
			// Vincent modified on 11 Aug, 2015 for # 0052759 item 4
			$expErrorMsg = t('ERR167'); //$exception->errorInfo[2]; //SQL syntax error
		}else {
			$expErrorMsg = t('ERR154'); //System error, Contact Support Team
		}
		expDebug::dPrint('$expErrorMsg = ' . print_r($expErrorMsg, true) , 4);
        expertusErrorThrow($exception);
    }

}

function getAdminCalendarEvents($from,$to,$search_text,$autocomplete = false)
{
	try
	{
		$from .= " 00:00:00";
		$to .= " 23:59:59";
		$userId = getSltpersonUserId();
		$result = "";
		$select = db_select('slt_course_class', 'cls');
		$select->leftJoin('slt_location','loc','loc.id = cls.location_id');
		$select->leftJoin('slt_profile_list_items','lst','lst.code = cls.lang_code');
		$select->leftJoin('slt_profile_list_items','lst2','lst2.code = cls.delivery_type');
		if($search_text != "" && (strtolower($search_text) != strtolower(t('LBL304'))))
		{
			$select->leftJoin('slt_tag_entity','te','cls.course_id=te.entity_id and te.entity_type=\'Course\'');
			$select->leftJoin('slt_tagdefn','td','te.tagid= td.id');
			$select->leftJoin('slt_tag_entity','te1','cls.id=te1.entity_id and te1.entity_type=\'Class\'');
			$select->leftJoin('slt_tagdefn','td1','te1.tagid= td1.id');
		}
		$select->distinct();
		$select->addField('cls', 'id', 'class_id');
		$select->addField('cls', 'course_id', 'course_id');
		$select->addField('cls', 'code', 'code');
		$select->addField('cls', 'title', 'title');
		$select->addField('cls', 'delivery_type', 'delivery_type');
		$select->addField('lst2', 'name', 'class_type');
		$select->addField('cls', 'lang_code', 'lang_code');
		$select->addField('cls', 'status', 'status');
		$select->addField('cls', 'registration_end_on', 'registration_end_on');
		$select->addField('loc', 'name', 'location_name');
		$select->addField('lst', 'name', 'language_name');
		//$select->addField('td','tagname', 'tagname');
		$select->addExpression('CASE WHEN DATEDIFF(DATE_FORMAT(cls.registration_end_on,\'%Y-%m-%d %H:%i:%s\'),DATE_FORMAT(NOW(),\'%Y-%m-%d %H:%i:%s\')) = 0 THEN \'fc-today\'
					WHEN (DATEDIFF(DATE_FORMAT(cls.registration_end_on,\'%Y-%m-%d\'),DATE_FORMAT(NOW(),\'%Y-%m-%d %H:%i:%s\')))>0 THEN \'fc-future\'
					ELSE
					\'fc-past\' END', 'datatype');
		if($autocomplete)
		{
			if((strtolower($search_text) != strtolower(t('LBL304')))){
				$select->condition(db_or()->
			condition('cls.title', '%' . db_like($search_text) . '%', 'LIKE')
			->condition('td.tagname', '%' . db_like($search_text) . '%', 'LIKE')
			->condition('td1.tagname', '%' . db_like($search_text) . '%', 'LIKE')
			);
		}
		}	
		else
		{
			if($search_text != "" && (strtolower($search_text) != strtolower(t('LBL304'))))
			{
				$select->condition(db_or()->
				  condition('cls.title', '%' . db_like($search_text) . '%', 'LIKE')
				->condition('td.tagname', '%' . db_like($search_text) . '%', 'LIKE')
				->condition('td1.tagname', '%' . db_like($search_text) . '%', 'LIKE')
			);
			}	
			$select->condition(db_or()->
					condition(db_and()
						->condition('cls.registration_end_on', $from, '>=')
						->condition('cls.registration_end_on', $to, '<='))->
						condition(db_and()
						->condition('cls.cancellation_deadline_date', $from, '>=')
						->condition('cls.cancellation_deadline_date', $from, '>=')));
		}
		$select->condition('cls.status', array('lrn_cls_sts_atv','lrn_cls_sts_itv'),'IN');
		if ($userId != 1) // skip privilege check for global admin
			filterByUserAccessPrivileges($select, 'cls', $userId, 1,'cre_sys_obt_cls','Class','cls');
		expDebug::dPrintDBAPI(' getAdminCalendarEvents obj = ' , $select);
		$result =  $select->execute()->fetchAll();
		expDebug::dPrint('getAdminCalendarEvents result = ' . print_r($result, 1));
		return $result;
	}catch(Exception $e)
	{
		expertusErrorThrow($e);

		//var_dump($e);
	}
}
 //Added for #0064978 by vetrivel.P
function getOccupiedInstructorsForDay($startdate,$search_text,$autocomplete = false,$from=false) {	
	
	$endDate = $_REQUEST['endDate'];
	$daywise = false;
	$Url = $_REQUEST['q'];
	$result = array();
	if($startdate == $endDate || (!isset($_REQUEST['endDate']) || $_REQUEST['endDate'] ==''))
		$daywise = true;
	else
		$daywise = false;
	
	try {
		$select = db_select('slt_course_class_session', 'sess');
		$select->leftJoin('slt_session_instructor_details','ins','sess.id = ins.session_id');
		$select->leftJoin('slt_person','per','per.id = ins.instructor_id');
		$select->leftJoin('slt_course_class','cls','cls.id = sess.class_id');
		$select->leftJoin('slt_location','loc','loc.id = sess.location_id');
		$select->distinct();
		$select->addField('sess', 'id', 'ses_id');
		$select->addField('ins', 'instructor_id', 'instructor_id');
		$select->addField('per', 'id', 'id');
		$select->addField('per', 'full_name', 'full_name');
		$select->addField('per', 'last_name', 'last_name');
		$select->addField('cls', 'delivery_type', 'delivery_type');
		$select->addField('sess', 'timezone', 'timezone');
		$select->addField('loc', 'timezone', 'location_timezone');
		$select->addField('sess', 'start_date', 'start_date');
		$select->where('ins.instructor_id IS NOT NULL');
		if($daywise && $startdate!='')
			$select->where("DATE_FORMAT(sess.start_date, '%Y-%m-%d') = "."'".$startdate."'");
		elseif($startdate!='' && $endDate!='')
			$select->where("DATE_FORMAT(sess.start_date, '%Y-%m-%d') >= "."'".$startdate."' and DATE_FORMAT(sess.start_date, '%Y-%m-%d') <= "."'".$endDate."'"); //or sess.end_date is null
		$select->condition('cls.status', array('lrn_cls_sts_atv','lrn_cls_sts_itv'),'IN');
		filterByUserAccessPrivileges($select, 'per','',1,'cre_usr','','per');
			$select->addField('sess', 'start_date', 'start_date');
			$select->addField('sess', 'start_time', 'start_time');
			$select->addField('sess', 'end_date', 'end_date');
			$select->addField('sess', 'end_time', 'end_time');
		
		if(($autocomplete == true && $Url != "administration/calendarviewapi") || ($search_text!='' && (strtolower($search_text) != strtolower(t('LBL304'))))){
			if($search_text!='' && (strtolower($search_text) != strtolower(t('LBL304')))){
				$select->condition(db_or()
						->condition('per.full_name', '%' . db_like($search_text) . '%', 'LIKE')
						->condition('per.user_name', '%' . db_like($search_text) . '%', 'LIKE')
						->condition('per.last_name', '%' . db_like($search_text) . '%', 'LIKE')
				);
			}
			$select->groupBy('per.full_name');
			expDebug::dPrintDBAPI(' getOccupiedInstructorsForDay IF obj = ' , $select);
		$instructors = $select->execute()->fetchAll();	
		
		foreach($instructors as $instructor)
		{
			$row = new stdClass();
			$row->title = sanitize_data($instructor->full_name);
			$row->instructor_id = $instructor->instructor_id;
			$row->type = "instructor";
			$row->id = "instructor-".$row->instructor_id;
			$row->class_id = core_encrypt($instructor->class_id);
			$row->course_id = core_encrypt($instructor->course_id);
			$row->start = date('Y-m-d', strtotime($instructor->start_date));
			$row->startTime = date("H:i",strtotime($instructor->start_time));
			$row->end = $instructor->start_date;
			$row->endTime = date("H:i",strtotime($instructor->end_time));
			
			$result[] = $row;
		}	
		return $result;
	}else{
		$search_type = $_REQUEST['search_type'];
		if($from || ($search_text!='' && (strtolower($search_text) != strtolower(t('LBL304'))) && ($search_type  == "view_instructors" || $search_type == "All" ))){
				if($search_text!='' && (strtolower($search_text) != strtolower(t('LBL304')))){
					$select->condition(db_or()
							->condition('per.full_name', '%' . db_like($search_text) . '%', 'LIKE')
							->condition('per.user_name', '%' . db_like($search_text) . '%', 'LIKE')
							->condition('per.last_name', '%' . db_like($search_text) . '%', 'LIKE')
					);
				}
				expDebug::dPrintDBAPI(' getOccupiedInstructorsForDay ELSE obj = ' , $select);
				$instructors = $select->execute()->fetchAll();
				
				foreach($instructors as $instructor)
				{
					$row = new stdClass();
					$row->title = sanitize_data($instructor->full_name);
					$row->instructor_id = $instructor->instructor_id;
					$row->type = "instructor";
					$row->id = "instructor-".$row->instructor_id;
					$row->class_id = core_encrypt($instructor->class_id);
					$row->course_id = core_encrypt($instructor->course_id);
					$row->start = date('Y-m-d', strtotime($instructor->start_date));
					$row->startTime = date("H:i",strtotime($instructor->start_time));
					$row->end = $instructor->start_date;
					$row->endTime = date("H:i",strtotime($instructor->end_time));
					$vSesTimeZone = ($instructor->delivery_type == "lrn_cls_dty_vcl") ? $instructor->timezone : $instructor->location_timezone;
						$tzArray = getProfileListItemByCode($vSesTimeZone);
						$time_zone = $tzArray[0]->attr1;
						$row->timezone = $time_zone;
					
					$result[]=$row;
				}	
			}
		}
		return $result;
	}catch(Exception $e)
	{
		expertusErrorThrow($e);
	}
}
//TO Display all the Instructors for all the day even we don't have the privillege for learning module in LIST ENTITY Icon.
function getOccupiedInstructorsForList($startdate)
{
	$search_text = $_REQUEST['search_text'];
	$search_type = $_REQUEST['search_type'];
	//if(($search_type == "view_classroom") || ($search_text == "" && $search_type == ""))
	{
		$endDate = $_REQUEST['endDate'];
		$daywise = false;
		if($startdate == $endDate || (!isset($_REQUEST['endDate']) || $_REQUEST['endDate'] ==''))
			$daywise = true;
		else
			$daywise = false;
		try
		{
			$select = db_select('slt_course_class_session', 'sess');
			$select->leftJoin('slt_session_instructor_details','ins','sess.id = ins.session_id');
			$select->leftJoin('slt_person','per','per.id = ins.instructor_id');
			$select->leftJoin('slt_course_class','cls','cls.id = sess.class_id');
			$select->distinct();
			$select->addField('sess', 'id', 'ses_id');
			$select->addField('sess', 'start_date', 'start_date');
			$select->addField('sess', 'start_time', 'start_time');
			$select->addField('sess', 'end_date', 'end_date');
			$select->addField('sess', 'end_time', 'end_time');
			$select->addField('ins', 'instructor_id', 'instructor_id');
			$select->addField('per', 'id', 'id');
			$select->addField('per', 'full_name', 'full_name');
			$select->addField('per', 'last_name', 'last_name');
			$select->addField('cls', 'delivery_type', 'delivery_type');
			$select->addField('sess', 'timezone', 'timezone');
			$select->condition('cls.status', 'lrn_cls_sts_del', '!=');
			$select->where('ins.instructor_id IS NOT NULL');
			if($daywise && $startdate!='')
				$select->where("DATE_FORMAT(sess.start_date, '%Y-%m-%d') = "."'".$startdate."'");
			elseif($startdate!='' && $endDate!='')
				$select->where("DATE_FORMAT(sess.start_date, '%Y-%m-%d') >= "."'".$startdate."' and DATE_FORMAT(sess.start_date, '%Y-%m-%d') <= "."'".$endDate."'"); //or sess.end_date is null
			filterByUserAccessPrivileges($select, 'per','',1,'cre_usr','','per');
			expDebug::dPrintDBAPI(' getOccupiedInstructorsForDay obj = ' , $select);
			if($search_text!='' && (strtolower($search_text) != strtolower(t('LBL304')) && ($search_type  == "view_instructors" || $search_type == "All" ))){
				$select->condition(db_or()
						->condition('per.full_name', '%' . db_like($search_text) . '%', 'LIKE')
						->condition('per.user_name', '%' . db_like($search_text) . '%', 'LIKE')
						->condition('per.last_name', '%' . db_like($search_text) . '%', 'LIKE')
				);
			}
			$instructors = $select->execute()->fetchAll();
			$result = array();
			foreach($instructors as $instructor)
			{
				$row = new stdClass();
				$row->title = $instructor->full_name;
				$row->instructor_id = $instructor->instructor_id;
				$row->type = "instructor";
				$row->id = "instructor-".$row->instructor_id;
				$row->start = date('Y-m-d', strtotime($instructor->start_date));
				$row->startTime = date("H:i",strtotime($instructor->start_time));
				$row->end = $instructor->start_date;
				$row->endTime = date("H:i",strtotime($instructor->end_time));
				$result[] = $row;
			}
			return $result;
		}catch(Exception $e)
		{
			expertusErrorThrow($e);
		}
	}
}
//End for #0064978 by vetrivel.P

//Admin Calendar Start
function annoncementSelectQuery () {
	$select = db_select('slt_site_notice', 'notice');
	$select->leftJoin('slt_person', 'per', 'notice.user_id = per.id');
	$select->leftJoin('slt_profile_list_items', 'plilang', 'notice.lang_code = plilang.code AND plilang.lang_code = \'cre_sys_lng_eng\'');
	$select->leftJoin('slt_profile_list_items', 'plistatus', 'notice.status = plistatus.code AND plistatus.lang_code = \'cre_sys_lng_eng\'');
	$select->distinct();
	$select->addField('notice', 'id', 'id');
	$select->addField('per', 'full_name', 'fullname');
	$select->addField('per', 'user_name', 'username');
	$select->addField('notice', 'from_date', 'from_date');
	$select->addField('notice', 'to_date', 'to_date');
	$select->addField('notice', 'from_time', 'from_time');
	$select->addField('notice', 'to_time', 'to_time');
	$select->addField('notice', 'description', 'description');
	$select->addField('notice', 'status', 'status_code');
	$select->addField('notice', 'priority', 'priority');
	$select->addField('plilang', 'name', 'language');
	$select->addField('plistatus', 'name', 'status');
	$select->addExpression('\'announcement\'', 'object_type');
	$select->addExpression('CASE WHEN (DATEDIFF(CONCAT(DATE_FORMAT(notice.to_date,\'%Y-%m-%d\'),\' \',notice.to_time),DATE_FORMAT(NOW(),\'%Y-%m-%d %H:%i:%s\'))) = 0 
				THEN if(TIMESTAMPDIFF(MINUTE,DATE_FORMAT(NOW(),\'%Y-%m-%d %H:%i:%s\'),CONCAT(DATE_FORMAT(notice.to_date,\'%Y-%m-%d\'),\' \',notice.to_time))<0,\'fc-past\',\'fc-today\')
				WHEN (DATEDIFF(CONCAT(DATE_FORMAT(notice.to_date,\'%Y-%m-%d\'),\' \',notice.to_time),DATE_FORMAT(NOW(),\'%Y-%m-%d %H:%i:%s\')))>0 THEN \'fc-future\'
				ELSE
				\'fc-past\'
				END', 'datatype');
	$select->condition('notice.status', 'cre_sys_obt_not_del', '!=');	
	return $select;
}

function sendAnnouncementListNew($search_text)
{
	$userId = getSltpersonUserId();
	$start_date = $_REQUEST["startDate"];
	$end_date = $_REQUEST["endDate"];
	$select1 = annoncementSelectQuery();
	filterByUserAccessPrivileges($select1, 'notice',$userId,1,'cre_sys_obt_not','','notice');
	if(isset($_GET['calendar_searchtype']) && !empty($_GET['calendar_searchtype']) )
	{
		$select1->condition('notice.description', '%'.$_GET['z'].'%','LIKE');
	}else if($search_text != "")
	{
		$select1->condition('notice.description', '%'.$search_text.'%','LIKE');
	}else{
		$select1->condition('notice.from_date', $end_date , '<=');
		$or_query = db_or();
					$or_query->isNULL('notice.to_date');
					$or_query->condition('notice.to_date', $start_date , '>=');
		$select1->condition($or_query);
	}
	expDebug::dPrintDBAPI(' sendAnnouncementListNew select1 = ' , $select1);
	$result = $select1->execute()->fetchAll();
	return $result;
}

function sendAnnouncementList()
{
 try{
  expDebug::dPrint(' ' .
                        'page = "' . $_GET['page'] . '" ,' .
                        'rows = "' . $_GET['rows'] . '" ,' .
                        'sort(sidx) = "' . $_GET['sidx'] . '" ,' .
                        'order(sord) = "' . $_GET['sord'] . '" ,' .
                        'search = "' . $_GET['search'] . '" ,' .
                        'nd = "' . $_GET['nd'] . '" ,' .
                        'id = "' . $_GET['id'] . '" ,' .
                        'oper = "' . $_GET['oper'] . '" ,' .
                        'editoper = "' . $_GET['editoper'] . '" ,' .
                        'addoper = "' . $_GET['addoper'] . '" ,' .
                        'deloper = "' . $_GET['deloper'] . '" ,' .
                        'totalrows = "' . $_GET['totalrows'] . '" ,' .
                        'npage = "' . $_GET['npage'] , 4
  );
  
  $page = $_GET['page']; // get the requested page 
  $limit = $_GET['rows'];

  $annCount = fetchAllAnnouncements('COUNT');
   
  expDebug::dPrint(' $annCount = ' . print_r($annCount, true) , 3);
  
  $total_pages = 0;
  $start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $annCount);
  expDebug::dPrint(' $start = ' . print_r($start, true) , 4);
  
  
  $announcementList = fetchAllAnnouncements('LISTS', $start, $limit);
  expDebug::dPrint(' $announcementList = ' . print_r($announcementList, true) , 3);  

  // Prepare and send the response
  $response->page = $page;
  $response->total = $total_pages;
  $response->records = $annCount; 
                                             // have to do this.
  $response->initial_sort_type_html_id = 'announcementdetails-sort-new'; 
  return $announcementList[0];
  } catch (Exception $ex) {
  	watchdog_exception('sendAnnouncementListToJqGrid', $ex);
  	expertusErrorThrow($ex);
  }
}

function sendManagerOrderList()
{
 try {
  setQueryMaxTime();
  $orderList = fetchManageOrderAll('LISTS', '', '','admincalendar');
  return $orderList;
  } catch (Exception $ex) {
  	watchdog_exception('sendManageOrderListToJqGrid', $ex);
  	expertusErrorThrow($ex);
  }
}
//Admin Calendar End


function updateOrderSatus($orderId, $status) {
	try {	
	include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_commerce/exp_sp_administration_order/exp_sp_administration_order.inc";
	//global $base_url, $user;
	$userId = getSltpersonUserId();

	$form_state['values']['order_id'] =$orderId;
    $form_state['values']['change_status'] = $status;
	//$res =  json_decode($response);
	$res = updateOrderByRestAPI($form_state,$userId);
	echo $res;
	} catch (Exception $ex) {
		watchdog_exception('updateOrderSatus', $ex);
		expertusErrorThrow($ex);
	}
}

function generateAccessToken() {
	try {
		global $base_url;
		$client_id= getConfigValue('oauth_client_id');
		$client_secret= getConfigValue('oauth_client_secret');
		$client_details = base64_encode("$client_id:$client_secret");
		$context = stream_context_create(array(
				'http' => array(
						'method' => 'POST',
						'header' => "Content-Type: application/x-www-form-urlencoded\r\n" .
						"Authorization : Basic $client_details\r\n",
						'content' => "grant_type=client_credentials"
				)
		));

		$response = file_get_contents($base_url.'/apis/oauth2/Token.php', false, $context);
		$res =  json_decode($response);
		$token = $res->access_token;
		return $token;
	} catch (Exception $ex) {
		watchdog_exception('generateAccessToken', $ex);
		expertusErrorThrow($ex);
	}
}

function translateTimingStr($time)
{
	expDebug::dPrint('translateTimingStr = ' . $time, 4);
	if($time == '')
		return $time;
	if(strpos($time, 'All day') !== false){
		$timing = "";
	}else{
		$timing = date("g:i a", strtotime($time));
	}
	return $timing;
}

function limitACSearchResults()
{
	$evtCntPerDay = 0;
	
	$evtLimit = 0;
	$view = $_REQUEST["view"];
	if($view == "month")
		$evtLimit = EVENT_LIMIT_MONTH;
	elseif($view == "basicWeek")
		$evtLimit = EVENT_LIMIT_WEEK;
	elseif($view == "agendaDay")
		$evtLimit = EVENT_LIMIT_DAY;
	else
		$evtLimit = 10;
		
	$requestFromNarrowCal = $_REQUEST["narrowcaldaywise"];
	$requestType	      = $_REQUEST["type"];
	$reqNarCalShrt 		  = $_REQUEST["narrowcaldaywiseshort"];
	
	//override evtLimit
	if($requestFromNarrowCal == "true")
		$evtLimit = -1;
	
	if($reqNarCalShrt == "true")
		$evtLimit = 7;
		
	if($requestType == "more") {
			$skipDate = date('m-d-Y', strtotime($_REQUEST['startDate']));
	}
	
	$retValue = new stdClass();
	$retValue->skipDate = $skipDate;
	$retValue->evtLimit = $evtLimit;
	$retValue->evtCntPerDay = $evtCntPerDay;
	return $retValue;
}
// end comment