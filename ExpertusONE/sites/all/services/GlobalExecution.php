<?php 

/**
* @class or PHP Name	: GlobalExecution
* @author(s)		  	: vivekanandan
* Version         	  	: 1.0
* Date			  		: 05/04/2009
* Description     	  	: This class is used as store time of Request and Response   
* PHP Version          	: 5.2.6
*/

class GlobalExecution {

	public $mStartTime;
	public $mEndTime;
	public $mExecTime;
	public $mProviderName;
	public $mProviderXML;

/*	function GlobalExecution(){
		$this->mStartTime		= $_SERVER["REQUEST_TIME"]; 
	}*/
	
	function GlobalExecution($pmProviderName=""){
		$this->mProviderName	= $pmProviderName;
		$this->mStartTime		= $_SERVER["REQUEST_TIME"]; 
	}
	
	public function setCurrentEndTime(){
		$this->mEndTime = time(); 
	} 

	public function setCurrentMicroSecStartTime(){ 
		$this->mMicroSecStartTime = microtime(true);
		return $this->mMicroSecStartTime;  
	}
	public function setCurrentMicroSecEndTime(){ 
		$this->mMicroSecEndTime = microtime(true);
		return $this->mMicroSecEndTime;
	} 
	
	public function getCalcMicroSecTimeDiff(){ 
		return $this->mMicroSecEndTime -  $this->mMicroSecStartTime;
		
	} 
	
	public function getCalcMilliSecTimeDiff(){ 
		$vres = round(($this->mMicroSecEndTime -  $this->mMicroSecStartTime)*1000);
		$v = explode(".",$vres);
		return $v[0]; 	  	 
		
	}
	
	public function setStartTime($time){  
		$this->mStartTime=$time;
	}
	public function setEndTime($time){ 
		$this->mEndTime=$time;
	} 
	
	public function getStartTime(){
		return $this->mStartTime;
	}

	public function getEndTime(){
		return $this->mEndTime;
	}

	public function getExecutionTime(){
		$this->setCurrentEndTime();	 	
		$this->exeTime = $this->CalcExecutionTime(); 
		return $this->exeTime;
	}

	public function CalcExecutionTime(){
		$this->exeTime = $this->mEndTime - $this->mStartTime ; 
		return $this->exeTime; 
	}

	public function getProviderStr($pmReqXml, $pmTotalRec){
		 		 
		$this->mProviderXML = '<Provider name="'.$this->mProviderName.'" executiontime="'.$this->exeTime.'" totalrecords="'.$pmTotalRec.'" type="location">'.$pmReqXml .'</Provider>';     
		return $this->mProviderXML;
		
	}


		

}
?>