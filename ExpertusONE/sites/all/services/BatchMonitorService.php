<?php
	/**
	* @class or PHP Name		: User Service
	* @author(s)				: Product Team
	* Version         			: 1.0
	* Date						: 23/12/2009
	* PHP Version          		: 5.2.6
	* Description     			: Web Service which is used to do CRUD operations of the User table in SmartPortal LMS.
	*/

include "../dao/BatchMonitorDAO.php";
include_once "ServiceBase.php";
include_once "Encryption.php";


class BatchMonitorService extends ServiceBase{

	function __construct(){
	}
  public function batchMonitor($requestdata)
  {

  	try
  	{

  		    $dao = new BatchMonitorDAO();
  		  //  $date =date("Y-m-d h:m:s");
  		    $date=date("Y-m-d");
  		    expDebug::dPrint("date:".$date);
			expDebug::dPrint("TYPE:".$requestdata->Type);
			$type=explode("|", $requestdata->Type);
			$result =$dao->batchmonitor();
			$lasttrundate=$result[0]->xlastrundate;
			$lastdate=explode(" ", $lasttrundate);
			expDebug::dPrint("Result:::".serialize($result));
			expDebug::dPrint("Result:::".$result[0]->xlastrundate);
			expDebug::dPrint("Resultdate:".$lastdate[0]);
			// Commented by Vincent on May 31, 2016. Since this enrollbatchMonitor()
			// function is older one and no longer used.
			/*if($type[0]=='Enroll')
			{
 				if($lastdate[0]==$date)
				{
				expDebug::dPrint("This slp_change_enrolled_status already executed");
				}
				else
				{
					//$result = $dao->enrollbatchMonitor($requestdata);
				}
			}*/
			// Commented by Vincent on Aug 28, 2015. Since we are not using the 
			// statistics anywhere in the product right now. It was used earlier
			// In case if it is required in feature. The curresponing procedures
			// are needs to be revisited before enabling this featuer.
			// Now commenting for imporve performance.
			/*if($type[2]=='Statistics')
			{
				$result = $dao->StatisticsbatchMonitor();
			}*/
			if($type[3]=='RegStatus')
			{
				$result = $dao->RegstatusbatchMonitor();
			}
			// RemindSession AND LPExpired IS MOVED INTO exp_sp_learning.module
			/*if($type[4]=='RemindSession')
			{
			   if($lastdate[0]==$date)
				{
				expDebug::dPrint("This slp_lnr_remind_session_sel already executed");
				}
				else
				{
			      $result = $dao->RemindsessionbatchMonitor();
				}
			}
  	  		if($type[5]=='LPExpired')
			{
			   if($lastdate[0]==$date)
				{
				expDebug::dPrint("This slp_change_lp_expired_status already executed");
				}
				else
				{
			      $result = $dao->LPExpiredbatchMonitor();
				}
			}*/
			
			/**
			 * To remove the memcache entry in specific intervel
			 * Default interval is 5 hours.
			 */
			  include_once "../services/GlobalUtil.php";
			  $oConfig  =  new GlobalUtil();
			  $aConfig = $oConfig->getConfig();
			  $valid = $aConfig['memcache_timeout'];
				$rst = $dao->getLastRunTime();
				$ctime = Date("Y-m-d H:i:s");
				$ltime = new DateTime($rst[0]->LastRun);
				$ltime->add(new DateInterval('PT'.$valid.'H'));
				$ltime = $ltime->format('Y-m-d H:i:s');
				expDebug::dPrint("Cache Expire time");
				expDebug::dPrint($ltime);
				if($ltime<$ctime){
					$this->clearCacheValue('cache-exp_featured_course');
					$this->clearCacheValue('cache-exp_active_course');
					$this->clearCacheValue('cache-exp_popular_course');
					$result = $dao->TimecheckbatchMonitor('Clear_Memcache');
				}
				
				// Update batch Monitor
				$result = $dao->TimecheckbatchMonitor('exp_sp_batchmonitor');


  	}
  	catch(Exception $e)
  	{
  		expDebug::dPrint("Error in JOB :".$e->getMessage());
  		throw new SoapFault("SPLMS",$e->getMessage());
  	}
  }



}
	$server = new SoapServer("../modules/core/exp_sp_core/wsdl/batchmonitorservice.wsdl");
	$server->setClass("BatchMonitorService");
	$server->handle();
?>