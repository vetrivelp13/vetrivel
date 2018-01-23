<?php
/**
 * @class or PHP Name		: User DAO
 * @author(s)				: Product Team
 * Version         			: 1.0
 * Date						: 23/12/2009
 * PHP Version          		: 5.2.6
 * Description     			: DAO Class to do basic CRUD operations in User table.
 */
 
include_once "AbstractDAO.php";

class BatchMonitorDAO extends AbstractDAO{


	public function __construct(){
		parent::__construct();
	}

	public function enrollbatchMonitor($requestdata)
	{
		try
		{
			$query="call slp_change_enrolled_status()";
			$this->connect();
			$this->beginTrans();
			expDebug::dPrint("BatchMonitorDAO enrollbatchMonitor() query EnrollStatus: ".$query);
			$this->execute($query);
			$this->commitTrans();
			$result = "";
			$this->closeconnect();
			return $result;
		}
		catch(Exception $e)
		{
			$this->rollbackTrans();
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}



	public function StatisticsbatchMonitor()
	{
		try
		{
			$query="call slp_statistics_upd()";
			$this->connect();
			$this->beginTrans();
			expDebug::dPrint("BatchMonitorDAO StatisticsbatchMonitor() query StatisticsStatus: ".$query);
			$this->execute($query);
			$this->commitTrans();
			$result = "";
			$this->closeconnect();
			return $result;
		}
		catch(Exception $e)
		{
			$this->rollbackTrans();
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	public function RegstatusbatchMonitor()
	{
		try
		{
			$util=new GlobalUtil();
			$config=$util->getConfig();
			//72265: Enrollment getting failed in admin order creation ( added 2 min for back end cancel time )
			$checkoutTime = $config['checkout_timeout']+2;
			
			$query="call slp_change_reg_status($checkoutTime)";
			$this->connect();
			$this->beginTrans();
			expDebug::dPrint("BatchMonitorDAO RegstatusbatchMonitor() query RegistrationStatus: ".$checkoutTime);
			$this->execute($query);
			$this->commitTrans();
			$result = "";
			$this->closeconnect();
			return $result;
		}
		catch(Exception $e)
		{
			$this->rollbackTrans();
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	public function RemindsessionbatchMonitor()
	{
		try
		{
			$query="call slp_lnr_remind_session_sel()";
			$this->connect();
			$this->beginTrans();
			expDebug::dPrint("BatchMonitorDAO RemindsessionbatchMonitor() query RemindSession: ".$query);
			$this->execute($query);
			$this->commitTrans();
			$result="";
			//$result=$this->fetchAllResults();
			$this->closeconnect();
			return $result;
		}
		catch(Exception $e)
		{
			$this->rollbackTrans();
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	/* public function LPExpiredbatchMonitor()
	{
		try
		{
			$query="call slp_change_lp_expired_status()";
			$this->connect();
			$this->beginTrans();
			expDebug::dPrint("BatchMonitorDAO LPExpiredbatchMonitor() query LPExpiredStatus: ".$query);
			$this->execute($query);
			$this->commitTrans();
			$result = "";
			$this->closeconnect();
			return $result;
		}
		catch(Exception $e)
		{
			$this->rollbackTrans();
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	} */

	public function batchmonitor()
	{
		try
		{
				
			$query="call slp_monitoring_jobs_sel('exp_sp_batchmonitor')";
			expDebug::dPrint("BatchMonitorDAO RemindsessionbatchMonitor() query RemindSession: ".$query);
			$this->connect();
			$res = $this->query($query);
			$result=$this->fetchAllResults();
			$this->closeconnect();
			return $result;
		}
		catch(Exception $e)
		{
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	public function TimecheckbatchMonitor($param)
	{
		try
		{
			//$query="call slp_monitoring_jobs_upd('exp_sp_batchmonitor','1')";
			$query="call slp_monitoring_jobs_upd(\"$param\",'1')";
			$this->connect();
			$this->beginTrans();
			expDebug::dPrint("BatchMonitorDAO TimecheckbatchMonitor() query Timecheck: ".$query);
			$this->execute($query);
			$this->commitTrans();
			$result = "";
			$this->closeconnect();
			return $result;
		}
		catch(Exception $e)
		{
			$this->rollbackTrans();
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	public function getLastRunTime(){
		try{
			$query="select id,job_name,last_run_date LastRun from slt_monitoring_jobs where job_name='Clear_Memcache'";
			expDebug::dPrint("BatchMonitorDAO getLastRunTime() query : ".$query);
			$this->connect();
			$res = $this->query($query);
			$result=$this->fetchAllResults();
			$this->closeconnect();
			return $result;
		}catch(Exception $e){
			$this->closeconnect();
			expDebug::dPrint("Error in Query :".$e->getMessage());
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}


}
?>