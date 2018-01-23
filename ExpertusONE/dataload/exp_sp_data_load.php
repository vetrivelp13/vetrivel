<?php 

/**
 * @desc Base class for data load process
 * @author Vincent
 *
 */
class dataLoadBase{

	/**
	 * @desc to store database objcet (PDO)
	 * @var unknown_type
	 */
	protected $db;
	
	/**
	 * @desc id of currently processing job
	 * @var string 
	 */
	private $jobId;
	
	/**
	 * @desc Source table name which holds the source data
	 * loaded from feed file
	 * @var string
	 */
	private $srcTable;
	
	/**
	 * @desc Tempory table name which holds data for actual processing 
	 * @var string
	 */
	private $processTable;
	
	/**
	 * @desc Tempory table name which holds data for validate and process 
	 * @var string
	 */
	private $executeTable;
	/**
	 * @desc Batch id which is in process
	 * @var unknown_type
	 */
	private $batchId;
	
	/**
	 * @desc User Email id to send notification for that job
	 * @var string
	 */
	private $usrMailId;
	
	/**
	 * @desc User id who created the job
	 * @var string
	 */
	private $userId;

	/**
	 * @desc process type - API/Bulk upload
	 * @var string
	 */
	private $prcType;
    
    public $custom_attr_module_status;

	
	function __construct(){
		
		$util=new GlobalUtil();
		$config=$util->getConfig();
		$configTimeVal    = $config["dataload_execution_time"];
		
		expDebug::dPrint(' Inside contructor of the base class'.print_r($configTimeVal,1),5);
		// Reseting execution time
		set_time_limit($configTimeVal);
		
		// Create PDO connection object
		$this->db = new DLDatabase('DL');
		$this->db->update_queue = true;
		$this->terminateLongRunningQueries($configTimeVal);
        $this->custom_attr_module_status=$this->getCustomAttributeStatus();
		// Reset memory limit
		ini_set('memory_limit', '512M'); 
		
	}
	 
	
	protected function terminateLongRunningQueries($maxExecutionTime){
		try {
			/* $processlistQry = db_query('SHOW PROCESSLIST', array(), getReportDatabaseOption());
			$processlist = $processlistQry->fetchAll(); */
			
			$this->db->callQuery('SHOW PROCESSLIST');
			$processlist = $this->db->fetchAllResults();
			expDebug::dPrint(' Inside contructor of the base class'.print_r($processlist,true),5);
			foreach ($processlist as $proc) {
				expDebug::dPrint('Reports database process obj = ' . print_r($proc, true), 4);
				if(strtolower($proc->Command) == 'query' && $proc->Time >= $maxExecutionTime &&
						(strtolower($proc->State) == 'copying to tmp table' || strtolower($proc->State) == 'sorting result' ||
								strtolower($proc->State) == 'sending data') && stripos(strtolower($proc->Info), 'select') >= 0) {
					$killQuery = 'kill ' . $proc->Id;
					expDebug::dPrint('Query for killing db process = ' . $killQuery, 2);
					
					try{
						$this->db->callExecute($killQuery);
					}catch(PDOException $ex){
						expDebug::dPrint("PDO Error in getting terminateLongRunningQueries exp load file",1);
						throw new PDOException($ex->getMessage());
					}
				}
			}
		}
		catch (PDOException $ex) {
			expDebug::dPrint("PDO Error in getting terminateLongRunningQueries",1);
		//	throw new PDOException("Error in getting job details ".$ex->getMessage());
		}
		catch (Exception $ex) {
			expDebug::dPrint("PDO Error in getting terminateLongRunningQueries 555",1);
		}
	}
	/**
	 * @desc Getting current job id
	 * @return string
	 */
	protected function getJobId(){
		return $this->jobId;
	}
	
	/**
	 * @desc Getting source table name
	 * @return string
	 */
	protected function getSourceTable(){
		return $this->srcTable;//temp1
	}
	
	/**
	 * @desc Getting temp table name
	 * @return string
	 */
	protected function getProcessTable(){
		return $this->processTable;//temp2
	}
	
	/**
	 * @desc Getting temp table name
	 * @return string
	 */
	protected function getExecutTable(){
		return $this->executeTable;
	}
	
	/**
	 * @desc Gettign batch id which is in progress
	 * @return unknown_type
	 */
	protected function getBatchId(){
		return $this->batchId;
	}
	/**
	 * @desc Gettign batch operation which is in progress
	 * @return unknown_type
	 */
	protected function getBatchOpt(){
		return $this->batchopt;
	}
	/**
	 * @desc Function for getting the mail id for the job
	 * @return string
	 */
	protected function getMailId(){
		return $this->usrMailId;
	}
	
	/**
	 * @desc Function for getting the mail id for the job
	 * @return string
	 */
	protected function getUserId(){
		return $this->userId;
	}
	
	/**
	 * @desc Function for getting the process type for the job
	 * @return string
	 */
	protected function getPrcType(){
		return $this->prcType;
	}

	/**
	 * @desc Setting values to variables
	 * @param $id
	 * @return none
	 */
	private function setValues($id,$jbArr=array()){
		$this->jobId = $id;
		$this->srcTable = 'temp1_'.$id;
		$this->processTable = 'temp2_'.$id;
		if(count($jbArr)!=0){
			$this->usrMailId = $jbArr['mail_to'];
			$this->userId = $jbArr['user_id'];
			$this->prcType = $jbArr['process_type'];
		}
	}
	/**
	 * @desc Get Job details
	 * @param $stauts
	 * @return object
	 */
	protected function getJob($status='',$code='',$id=''){
		try{
			//Fetch job details to proceed
			$args = array();
			$conditons = array();
			$fields = array(
				'jb.job_id',
				'jb.file_name',
				'jb.entity_type',
				'jb.mail_to',
				'jb.user_id',
				'jb.process_type',
				'jb.status'
			);

			if($status){
				$conditons[] = ' jb.status = :status';
				$args[':status']= $status;
			}
			if($code){
				$conditons[] = ' jb.job_code = :job_code';
				$args[':job_code']= $code;
			}
			
			if($status == 'NS')
				$conditons[] = ' jb.starts_when <= now()';
			
			if(!empty($id))
				$conditons[] = ' jb.id = '.$id;
				
			$limt = ' LIMIT 1 ';
			// Prepare query 
			$query = $this->db->prepareQuerySelect('slt_dataload_jobs jb',$fields,$conditons,null,$limt);
			expDebug::dPrintDBAPI("Get Job Details",$query,$args);
			// Execute query
			$this->db->callQuery($query,$args);
			// Fetch results
			$JobDetails = $this->db->fetchAllResults();
			expDebug::dPrint(' $JobDetails '. print_r($JobDetails,true),5);
			$jbArr=array();
			if($JobDetails){
				$jbArr['mail_to'] = $JobDetails[0]->mail_to;
				$jbArr['user_id'] = $JobDetails[0]->user_id;
				$jbArr['process_type'] = $JobDetails[0]->process_type;
				$this->setValues($JobDetails[0]->job_id,$jbArr);
				return $JobDetails;
			}else{
				return '';
			}
		}catch (PDOException $ex) {
			expDebug::dPrint("PDO Error in getting job details",1);
			throw new PDOException("Error in getting job details ".$ex->getMessage());
		}
		catch (Exception $ex) {
			expDebug::dPrint("Error in getting job details",1);
			throw new Exception("Error in getting job details ".$ex->getMessage());
		}
	}
	
	protected function getBatchDetail($status,$jobsts,$jId=''){
		try{
			$args = array();
			$conditions = array();
			$fields = array(
					'bt.id',
					'bt.job_id',
					'bt.job_type',
					'bt.entity_name',
					'bt.operation',
					//'bt.file_path',
					'job.process_type',
					'job.file_name',
					'job.entity_type',
					'bt.entity_id',
					'job.mail_to',
					'job.user_id',
			        'job.custom0'
			);
			$conditions[] = ' bt.status = :sts';
			$conditions[] = ' job.status = :jsts';
			$args[':sts'] = $status;
			$args[':jsts'] = $jobsts;
			if(!empty($jId)){
				$conditions[] = ' job.id = :jid';
				$args[':jid'] = $jId;
			}
			$limt = ' LIMIT 1';
			$join = array(' left join slt_dataload_jobs job on job.job_id=bt.job_id');
			$query = $this->db->prepareQuerySelect('slt_dataload_batches bt',$fields,$conditions,$join,$limt);
			expDebug::dPrintDBAPI("Get Job Details",$query,$args);
			// Execute query
			$this->db->callQuery($query,$args);
			// Fetch results
			$batchdetails = $this->db->fetchAllResults();
			
			$jbArr=array();
			if($batchdetails){
				$jbArr['mail_to'] = $batchdetails[0]->mail_to;
				$jbArr['user_id'] = $batchdetails[0]->user_id;
				$jbArr['process_type'] = $batchdetails[0]->process_type;
				$this->setValues($batchdetails[0]->job_id,$jbArr);
				$this->batchId = $batchdetails[0]->id;
				$this->batchopt = $batchdetails[0]->operation;
				$this->executeTable = $this->processTable."_batch_".$batchdetails[0]->id;
				return $batchdetails;
			}else{
				return '';
			}
		}catch (Exception $ex) {
			expDebug::dPrint("Error in getting batch details",1);
			throw new Exception("Error in getting job details ".$ex->getMessage());
		}
	}
	
	/**
	 * @desc Update process status for given table
	 * @param $tblName
	 * @param $status
	 * @return none
	 */
	protected function updateJobLoadStatus($tblDet,$stsCnt = array()){ 
		try{
			expDebug::dPrint('updateJobLoadStatus'.print_r($tblDet,true).'$stsCnt'.print_r($stsCnt,true).'bbb'.count($stsCnt),5);
			$tblName = $tblDet['tblname'];
			$preSts = (!empty($tblDet['prestatus'])) ? $tblDet['prestatus'] : '';
			$pSts = (!empty($tblDet['psts'])) ? $tblDet['psts'] : '';
			$field = array();
			
			$upVal = array();
			$args = array();
			// Update the previous status of the Job
			if($preSts == 1)
				$upVal["pre_status"] = "status";
			else if(!empty($preSts)){
				$field['pre_status'] = ':prests ';
				$args[':prests'] = $preSts;
			}
			
			if((!empty($tblDet['prccnt'])) && empty($preSts)){
				$upVal["processed_cnt"] = "processed_cnt + :cnt ";
				$args[':cnt'] = 1;
			}else if(!empty($preSts) && empty($tblDet['prccnt'])){
				$field['processed_cnt'] = "if(pre_status = status,processed_cnt,:cnt )";
				$args[':cnt'] = 0;
			}
			
			if((count($stsCnt) == 0) && empty($pSts)){
				$field['status'] = ":status ";
				$args[':status'] = $tblDet['status'];
			}else if(!empty($pSts))
				$upVal["status"] = "pre_status";
			else if(count($stsCnt) >0){
				$field['status'] = ":status ";
				$args[':status'] = $tblDet['status'];
				$upVal["success_records"] = "success_records + :scnt ";
				$upVal["failure_records"] = "failure_records + :fcnt ";
				$args[':scnt'] = empty($stsCnt['sucrec']) ? 0 : $stsCnt['sucrec'];
				$args[':fcnt'] = $stsCnt['flrec'];
				if(!empty($stsCnt['recprs'])){
					$upVal["records_processed"] = "records_processed + :rprs ";
					$args[':rprs'] = $stsCnt['recprs'];
				}
			}
			
			if(!empty($tblDet['result'])){
				$field['result'] = ":result ";
				$args[':result'] = $tblDet['result'];
			}
			if(!empty($tblDet['value']) && $tblDet['value'] == 'update'){
				$field['operation'] = ":result ";
				$args[':result'] = $tblDet['value'];
			}
			
			if(!empty($tblDet['compon']))
				$field['completed_on'] = $tblDet['compon'];
			
			if(!empty($tblDet['timetaken'])){
				$totTime = ($tblDet['timetaken'] > 0) ? gmdate('H:i:s',$tblDet['timetaken']) : '00:00:00';
				$upVal["time_taken"] = "ADDTIME(time_taken,:timetaken )";
				$args[':timetaken'] = $totTime;
			}
			
			if(!empty($tblDet['total_time'])){
				$totTime = ($tblDet['total_time'] > 0) ? gmdate('H:i:s',$tblDet['total_time']) : '00:00:00';
				$upVal["time_taken"] = "ADDTIME(time_taken,:timetaken )";
				$args[':timetaken'] = $totTime;
			}
			
			if(!empty($tblDet['total_records'])){
				$field['total_records'] = ":total_records";
				$args[':total_records'] = $tblDet['total_records'];
			}
			
			if(count($field) != 0)
				$upVal = array_merge($upVal,$field);
			
			$condition = array();
			$condition[] = 'job_id = :job_id ';
			$args[':job_id']=$this->jobId;

			if(!empty($tblDet['batchid'])){
				$condition[] = 'id = :id ';
				$args[':id']=$tblDet['batchid'];
			}
			
			if(!empty($tblDet['consts'])){
				$condition[] = 'operation = :op ';
				$args[':op']=$tblDet['consts'];
				$condition[] = 'status = :consts ';
				$args[':consts']='NS';
			}
			$upQry = $this->db->prepareQueryUpdate($tblName,$upVal,$condition);
			expDebug::dPrintDBAPI("Status Update Query",$upQry,$args);
			$this->db->callExecute($upQry,$args);
			
			if($tblName == 'slt_dataload_jobs')
				$this->setMemCacheValue($this->jobId,$tblDet['status']);
			
		}catch (PDOException $ex) {
			expDebug::dPrint("Error in updating job details",1);
			throw new PDOException("Error in getting job details ".$ex->getMessage());
		}
		catch (Exception $ex) {
			expDebug::dPrint("Error in updating job details",1);
			throw new Exception("Error in getting job details ".$ex->getMessage());
		}
	}
	
	/** 
	 * @desc Get the Count of batch regarding status
	 * @param $op - insert/update
	 * @return Count of records
	 */
	protected function getBatchStatusCount($op=''){
		try{

			$conditions = array(' batches.job_id = :jbid ',' batches.status NOT IN (:sts , :fsts )');
			$args = array(':jbid'=>$this->getJobId(), ':sts'=>'CP' , ':fsts' => 'FL');
			
			if(!empty($op)){
				$conditions[] = ' batches.operation = :op ';
				$args[':op'] = $op;
			}
			
			$query = $this->db->prepareQuerySelect('slt_dataload_batches batches',array('COUNT(1) as cnt'),$conditions,null,null);
			expDebug::dPrintDBAPI("Get Job Details",$query,$args);
			// Execute query
			$this->db->callQuery($query,$args);
			// Fetch results
			return $this->db->fetchColumn();
		}catch(Exception $ex){
			expDebug::dPrint("Error in getting batch status count details",1);
			throw new Exception("Error in getting the status count ".$ex->getMessage());
		}
	}
	
	/**
	 * @desc Insert the Process 
	 * @param $op - Details of the process values to insert
	 * @return none
	 */
	protected function insertDataProcessQueue($batchId=null){
		try{
			$fields=array(
          'pid' => ':pid ',
          'pstart' => 'now()',
          'job_id' => ':job_id ',
					'batch_id' => ':batch_id ',
  	  );
			$args = array(
          ':pid' => getmypid(),
          ':job_id' => $this->jobId,
					':batch_id' => $batchId,
  	  );
			$qry = $this->db->prepareQueryInsert('slt_dataload_process_queue',$fields);
			expDebug::dPrintDBAPI("Insert process queue --> ",$qry,$args);
			$this->db->callExecute($qry,$args);
		}catch(PDOException $e){
			// TODO: do what you want here 
		}
		catch (Exception $ex) {
			expDebug::dPrint("Error in inserting process id for job details",1);
			throw new Exception("Error in getting job details ".$ex->getMessage());
		}
	}
	
	/**
	 * @desc Create new Job API
	 * @param $jobInfo - $_POST values to create jobs
	 * @return Job Id
	 */
	
	public function createNewJob($jobInfo){
	
		/** TODO : File format validation
		 * 	Virus scan
		 * 	Job creation
		 */

	expDebug::dPrint(' $jobInfo '.print_r($jobInfo,true),5);
	
	try{
		// Calculate file size
		$unit = intval(log($jobInfo['file_size'], 1024));
		$units = array('B', 'KB', 'MB', 'GB');
		$fileSize = round($jobInfo['file_size'] / pow(1024, $unit),2). $units[$unit];
		$fields=array(
				'entity_type' => ':entity_type ',
				'file_name' => ':file_name ',
				'file_size' => ':file_size ',
				'process_type' => ':process_type ',
				//'upload_type' => ':upload_type ',
				'user_id' => ':user_id ',
				'mail_to' => ':mail_to ',
				'starts_when' => ':starts_when ',
				'status' => ':status '
		);
		$args = array(
				':entity_type' => $jobInfo['Entity_Name'],
				':file_name' => str_replace(' ', '_', $jobInfo['file_path']),
				':file_size' => $fileSize,
				':process_type' => $jobInfo['Process_Type'],
				//':upload_type' => ($jobInfo['Upload_Type'] == 'fresh') ? 1 : 0,
				':user_id' => $jobInfo['User_Id'],
				':mail_to' => $jobInfo['Mail_To'],
				':starts_when' => ($jobInfo['Starts_when'] == '') ? 'now()' : $jobInfo['Starts_when'],
				':status' => 'NS'
		);
		$qry = $this->db->prepareQueryInsert('slt_dataload_jobs',$fields);
		expDebug::dPrintDBAPI(" createNewJob  ",$qry,$args);
		$insertId =	$this->db->callExecute($qry,$args);
		
		expDebug::dPrint(' $insertId '.$insertId, 3);
		// Create Job id and code
		$jobId = 'job'.$insertId;
		//$jobcode = md5($jobId);
		
		$this->db->callQuery("select name from slt_dataload_entity where type = :entity_type ",array(':entity_type' => $jobInfo['Entity_Name']));
		$entityName = $this->db->fetchColumn();
		
		$jobcode = str_replace(' ', '_', $entityName).'_'.$jobId;
		
		expDebug::dPrint(' Job Id '. $jobId .' Job Code '. $jobcode , 4);
	
		$fld = array(
				'job_id' => ':jid ',
				'job_code' => ':jcode '
		);
		$con = array(
			'id = :insId '
		);
		$arg = array(
			':jid' => $jobId,
			':jcode' => $jobcode,
			':insId' => $insertId
		);
		$qry = $this->db->prepareQueryUpdate('slt_dataload_jobs',$fld,$con);
		expDebug::dPrintDBAPI("Job id update  ",$qry,$arg);
		$this->db->callExecute($qry,$arg);
		
		$response['Status']	= 'Success';
		$response['Job Id'] = $jobcode;
		
		return $response;
		}catch (Exception $ex) {
			expDebug::dPrint("Error in creating new job details",1);
			throw new Exception("Error in getting job details ".$ex->getMessage());
		}
	}
	
	
	/**
	 * @desc Get the Job Status
	 * @param $jobCode - Encrypted code for that Job id
	 * @param $detType -  short/full detail about the status
	 * @return Job details
	 */
	public function getJobStatus($jobInfo){
		try{
			
			expDebug::dPrint('Job INFO'.print_r($jobInfo,true),5);
			
			$fields=array(
				'ent.name',
				'dlj.job_id',
				'dlj.status',
				'dlj.total_records as totrec',
				'dlj.records_processed as recpr',
				'dlj.starts_when as strton',
				'dlj.completed_on as compon',
				'dlj.success_records as sucrec',
				'dlj.failure_records as flrec',
				'dlj.result',
				'dlj.file_name as flname',
				'dlj.user_id'
			);
			$join = array(' left join slt_dataload_entity ent on ent.type = dlj.entity_type');
			$query = $this->db->prepareQuerySelect('slt_dataload_jobs dlj',$fields,array('dlj.job_code = :jbcd '),$join);
			expDebug::dPrintDBAPI("Get Job Details",$query,array(':jbcd'=>$jobInfo['Job_Id']));
			$this->db->callQuery($query,array(':jbcd'=>$jobInfo['Job_Id']));
			$res = $this->db->fetchAllResults();
			expDebug::dPrint('getJobStatus values'.print_r($res,true),3);
			
			
			if(count($res) == 0){
				$stsArr = array();
				$stsArr['message']="Invalid job id";
				return $stsArr;
			}else{
				$detType = $jobInfo['Status_Type'];
				$jbSts = $res[0]->status;
				$statusMess = array(
					'NS' => 'Not Started',
					'ST' => 'In Progress',
					'PR' => 'In Progress',
					'IP' => 'In Progress',
					'PS' => 'Paused',
					'TR' => 'Terminated',
					'CP' => 'Completed',
					'FL' => 'Failed');
				$stsArr = array();
					
				if(!empty($jobInfo['Message']))
					$stsArr['message'] = $jobInfo['Message'];
					
				$stsArr['EntityName'] = $res[0]->name;
				$stsArr['status'] = $statusMess[$jbSts];
				$stsArr['starts_on'] = $res[0]->strton;
					
				if($jbSts != 'NS'){
					$stsArr['total_records'] = $res[0]->totrec;
					$stsArr['records_processed'] = $res[0]->recpr;
					$stsArr['success_records'] = $res[0]->sucrec;
					$stsArr['failure_records'] = $res[0]->flrec;
					if(($res[0]->recpr > 0) && ($res[0]->totrec > 0))
						$stsArr['processed_percentage'] = round(($res[0]->recpr/$res[0]->totrec * 100),2) . '%';
				
				}
					
				// Failure Comments will be sent only when the Job got failed.
				if($jbSts == 'FL')
					$stsArr['result'] = $res[0]->result;
					
				//Completed On date and time will be sent only after Job is completed.
				if($jbSts == 'CP')
					$stsArr['completed_on'] = $res[0]->compon;
				$dlNtf = new dataLoadException;
				if($jbSts == 'CP' || $jbSts == 'FL'){
					// TODO : Have to get the result CSV file and send it to the user.
					if($jbSts=='CP'){
						$notifyDetails = $dlNtf->getNotificationDetails($res['job_id']);
						//expDebug::dPrint('notification details '.print_r($notifyDetails,true),5);
						$subj = 'Data load process';
						$body = 'status>|' . 'Completed' . '~|' ;
					}else{
						$subj = 'Data load process is failed';
						$body = 'status>|' . 'Failed' . '~|' ;
					}
					
					$notifyDetails = $dlNtf->getNotificationDetails($res['job_id']);
					if($notifyDetails[0]->entity_type == 'cre_dtl_cls_enr'  && $notifyDetails[0]->enroll_type != 'BE'){
						$query = "select course_id as course_id ,title as title,pl.name as type from slt_course_class cls
											left join slt_profile_list_items pl on  pl.code = cls.delivery_type
											where cls.id = :clsid ";
						expDebug::dPrintDBAPI('Class details ', $query,array(':clsid'=>$notifyDetails[0]->class_id));
						$this->db->callQuery($query,array(':clsid'=>$notifyDetails[0]->class_id));
						$class_details = $this->db->fetchAllResults();
						expDebug::dPrint('$class_details details'.print_r($class_details,true),5);
						expDebug::dPrint('$class_details details'.print_r($class_details,true),5);
						$subj = 'type>|'.$class_details[0]->type. '~|'.
									'course_title>|' .$class_details[0]->title. '~|'.
								'compeleted>|' . 'completed' . '~|' ;
						$body = 'registration_count>|' . ((!empty($notifyDetails[0]->success_records)) ? $notifyDetails[0]->success_records : 'Nil') . '~|' .
								'rejection_count>|' . ((!empty($notifyDetails[0]->failure_records)) ? $notifyDetails[0]->failure_records : 'Nil');
					}else{				
						$body .=  'job_id>|' . $notifyDetails[0]->job_id . '~|' .
							'created_by>|' . $notifyDetails[0]->created_by . '~|' .
							'started_on>|' . $notifyDetails[0]->started_on . '~|' .
							'completed_on>|' . $notifyDetails[0]->completed_on . '~|' .
							'registration_count>|' . ((!empty($notifyDetails[0]->total_records)) ? $notifyDetails[0]->total_records : 'Nil') . '~|' .
							'success_records>|' . ((!empty($notifyDetails[0]->success_records)) ? $notifyDetails[0]->success_records : 'Nil') . '~|' .
							'rejection_count>|' . ((!empty($notifyDetails[0]->failure_records)) ? $notifyDetails[0]->failure_records : 'Nil');
					}
					$path = explode('.csv',$res[0]->flname);
					$fPath = $path[0].'_full.csv';
					$sPath = $path[0].'_short.csv';
				
					$fileName = ($detType == 'full') ? $fPath : $sPath;
					$flExist = file_exists($fileName);
					$fileName = ($flExist) ? $fileName : null;
					$dlNtf->generateNotification($subj,$body,$res[0]->user_id,'',$fileName,$notifyDetails[0]->entity_type,$res['job_id']);
				}
				
				return $stsArr;
			}
			
		}catch (Exception $ex) {
			expDebug::dPrint("Error in getting job status details",1);
			throw new Exception("Error in getting job details ".$ex->getMessage());
		}
	}
	
	public function generateCSVFileFromSourceTable($sts,$fileName){
		try{
			$tempTab1 = $this->srcTable;
			$util=new GlobalUtil();
			$config=$util->getConfig();
			$flName = $config['dataload_file_path'] . "/" . $fileName;
			unlink($flName);
			$path = explode('.csv',$flName);
			$fPath = $path[0].'_full.csv';
			$sPath = $path[0].'_short.csv';
			$resFile = array();
			$resFile['fullfile'] = $fPath;
			$resFile['shortfile'] = 'null';
			$this->db->callExecute("ALTER TABLE $tempTab1 DROP COLUMN mapping_id");
			$this->db->callExecute("update $tempTab1 set remarks = '' where remarks is null");
			$colQry = "SHOW COLUMNS FROM $tempTab1";
			$this->db->callQuery($colQry);
			$colRes = $this->db->fetchAllResults();
			expDebug::dPrint("Column names from temp table".print_r($colRes,1),1);
			$field_names = array();
			$colNm = '';
			foreach($colRes as $val){
				$colNm .= (empty($colNm)) ? "'$val->Field'" : ','."'$val->Field'";
			}
			
			list($fields, $args) = array(array(),array());
			
			$fields=array(
					'record_status' => "CASE WHEN record_status = :in  THEN :inv 
					WHEN record_status = :cp  THEN :com 
					WHEN record_status = :r  THEN :np END"
			);
			//Changed to Rejected for #73139 by vetrivel.P
			$args=array(
					':in' => 'IN',
					':cp' => 'CP',
					':r' => 'R',
					':inv' => 'Rejected',
					':com' => 'Completed',
					':np' => 'Not Processed'
			);
			
			$qry = $this->db->prepareQueryUpdate($tempTab1,$fields);
			expDebug::dPrintDBAPI("Update record status",$qry,$args);
			$this->db->callExecute($qry,$args);
			
			$csvQry = 'SELECT '.$colNm.' UNION ALL
			SELECT * FROM '.$tempTab1.'
			INTO OUTFILE  :fullpath 
			CHARACTER SET UTF8 
			FIELDS TERMINATED BY \'|\'
			LINES TERMINATED BY \'\n\'';
			
			$this->db->callExecute($csvQry,array(':fullpath'=>"$fPath"));
			
			$this->db->callExecute("DELETE FROM $tempTab1 WHERE record_status= :comp AND (remarks IS NULL or remarks = '') ",array(':comp'=>'Completed'));
			
			$this->db->callQuery("select count(1) from $tempTab1");
			$cnt = $this->db->fetchColumn();
			//expDebug::dPrintDBAPI('Query for generating the csv file',$csvQry,array(':fullpath'=>"$fPath"));
			
			if(!empty($cnt)){
				$csvQry = 'SELECT '.$colNm.' UNION ALL
				SELECT * FROM '.$tempTab1.'
				INTO OUTFILE  :spath
				CHARACTER SET UTF8 
				FIELDS TERMINATED BY \'|\'
				LINES TERMINATED BY \'\n\'';
								
				$this->db->callExecute($csvQry,array(':spath'=>"$sPath"));
				$resFile['shortfile'] = $sPath;
				//expDebug::dPrintDBAPI('Query for generating the csv file',$csvQry,array(':spath'=>"$sPath"));
			}
			
		/* 	$updQry = "update slt_dataload_jobs set result_full_file= :fullpath , result_short_file = :spath where job_id =:jobid "; 
			expDebug::dPrintDBAPI("generateCSVFileFromSourceTable --> ",$updQry,array(':jobid'=>$this->jobId,':fullpath'=>$fPath,':spath'=>$resFile['shortfile']));
			$this->db->callExecute($updQry,array(':jobid'=>$this->jobId,':fullpath'=>$fPath,':spath'=>$resFile['shortfile'])); */
			
			return $resFile;
		}catch(Exception $e){
			expDebug::dPrint("Error in generating csv file and saving to a new path",1);
		}
	}
	
	/**
	 * @desc Update the success, failure and total no of records processed
	 *  after each batch execution even though it is success or failure
	 */	
	protected function updateRecordsProcessedInJob($rec=array(),$time=0){
		try{
			
			$totTime = ($time > 0) ? gmdate('H:i:s',$time) : '00:00:00';
			$field = array("time_taken" => "ADDTIME(time_taken,:ttime )");
			$args = array();

			if(!empty($rec['recprs'])){
				$field['records_processed'] = 'records_processed + :rprs ';
				$args[':rprs'] = $rec['recprs'];
			}
			if(!empty($rec['sucrec'])){
				$field['success_records'] = 'success_records + :srec ';
				$args[':srec'] = $rec['sucrec'];
			}
			if(!empty($rec['flrec'])){
				$field['failure_records'] = 'failure_records + :frec ';
				$args[':frec'] = $rec['flrec'];
			}
			$args[':ttime'] = $totTime;
			$args[':jobid'] = $this->getJobId();
			
			$updQry = $this->db->prepareQueryUpdate('slt_dataload_jobs',$field,array(" job_id = :jobid "));
			expDebug::dPrintDBAPI("updateRecordsProcessedInJob --> ",$updQry,$args);
			$this->db->callExecute($updQry,$args);
			
		}catch(Exception $e){
			expDebug::dPrint("Error in updating processed record count in job details",1);
		}
	}
	/**
	 * @desc return the time taken for the process
	 * @param $time_start
	 * @return time taken
	 */
	protected function getProcessedTime($time_start){
		$time = 0;
		$time_end = microtime(true);
		$time = $time_end - $time_start;
		return $time;
	} 
	
	
	protected function updateJobProcessQueue(){
		$args = array(':jobid'=>$this->jobId, ':id'=> getmypid());
		$updQry = $this->db->prepareQueryUpdate('slt_dataload_process_queue',array("sql_id"=>"NULL"),array(" job_id = :jobid ",'pid = :id '));
		expDebug::dPrintDBAPI("updateJobProcessQueue ",$updQry,$args);
		$this->db->callExecute($updQry,$args);
	}
	
protected function dropFederatedTable($tbl){
		$util=new GlobalUtil();
		$conf=$util->getConfig();
		$fedx_url=$conf["FEDX_db_url"];
		$fed_url=$conf["FED_db_url"];
		if(!empty($fedx_url)){
			$info=parse_url($fedx_url);
			$query = "DROP TABLE IF EXISTS ".basename($info['path']).".".$tbl;
			expDebug::dPrintDBAPI("dropFederatedTable ",$query);
			$this->db->callExecute($query);
		}
	}
	
	protected function setMemCacheValue($key,$value=''){
		$oConfig  =  new GlobalUtil();
		$aConfig = $oConfig->getConfig();
		$memServer = $aConfig['memcache_server1'];
		$memSrvDt = explode(':',$memServer);
		// Connect to the memcache server
		$memcache = new Memcache;
		$connectedToMemCache = $memcache->connect($memSrvDt[0], $memSrvDt[1]);
		if($connectedToMemCache){
			if($memcache->get($key)){
				$memcache->replace($key,$value);
			}else{
				$memcache->set($key,$value);
			}
			$memcache->close();
		}
		return true;
	}
	
	protected function getMemCacheValue($key){
		$oConfig  =  new GlobalUtil();
		$aConfig = $oConfig->getConfig();
		$memServer = $aConfig['memcache_server1'];
		$memSrvDt = explode(':',$memServer);
		// Connect to the memcache server
		$memcache = new Memcache;
		$connectedToMemCache = $memcache->connect($memSrvDt[0], $memSrvDt[1]);
		if($connectedToMemCache){
			$rstVal= $memcache->get($key);
			$memcache->close();
			return $rstVal;
		}
	}
	
	/**
	 * Static method to get custom values if any for the 
	 * current job
	 * @param $jobId 
	 * @return Assosiative Array 
	 */
	public static function getCustomValues($jobId){
		$tbl = 'slt_dataload_jobs';
		$fields = array('custom0','custom1','custom2','custom3','custom4');
		$condition = array('job_id = :jid ');
		expDebug::dPrint("Inside static function -- ".$jobId);
		$arg = array(':jid'=>$jobId);
		
		$dbs  = new DLDatabase();
		$qry = $dbs->prepareQuerySelect($tbl,$fields,$condition);
		expDebug::dPrintDBAPI("Get Custom values query -- ",$qry,$arg);
		$dbs->callQuery($qry,$arg);
		return $dbs->fetchAllResults();
	}
    
     public function getCustomAttributeStatus(){
        try{ 
             $this->db->callQuery("select status from system where name='exp_sp_administration_customattribute'");
             $custom_attr_module_status = $this->db->fetchColumn();
             expDebug::dPrint('$custom_attr_module_status ='.$custom_attr_module_status,5);
             return $custom_attr_module_status;
        }catch (Exception $ex) { 
            throw new dataLoadException("Error in getCustomAttributeStatus ",$this->getUserId(),$this->getMailId(),$ex->getMessage());
        }
      }
     
	
} // End of class dataLoadBase

/**
 * @desc Data load process class
 * @author Priya
 *
 */
class dataLoadProcess extends dataLoadBase{
	
	private $startTime;
	
 	function __construct(){
 		$this->startTime = microtime(true);
		parent::__construct();
	}	 
	
	private function createSourceTable($JobDetails){
		try{
			$filename = $JobDetails[0]->file_name;
			$util=new GlobalUtil();
			$config=$util->getConfig();
			$file = str_replace(' ', '_', $config['dataload_file_path'] . "/" . $filename);
			// Temp table name 
			$tempTab1 = $this->getSourceTable();
			// get structure from csv and insert db
			ini_set('auto_detect_line_endings',TRUE);
			
			//	$handle = fopen($file,'r');
			if (($handle = fopen($file, "r")) == FALSE)
				throw new dataLoadException("Cannot read from csv",$this->getUserId());
				
			// first row, structure
			if ( ($data = fgetcsv($handle,0,'|') ) === FALSE ) {
				expDebug::dPrint("Cannot read from csv",1);
				throw new dataLoadException("Cannot read from csv",$this->getUserId());
			}

			try{
				// Getting the field from upload File and Create the Temp table 1
				$fields = array();
				$field_count = 0;
				for($i=0;$i<count($data); $i++) {
					$f = mb_strtolower(trim($data[$i]));
					if ($f) {
						if(mb_detect_encoding($f)=='UTF-8' && $i==0){
							$f=mb_convert_encoding($f, 'UTF-8');
							$f=preg_replace ('/[^0-9a-z_ ]/', '', $f);
							$f=mb_convert_encoding($f, 'ASCII', 'UTF-8');
							$asciiString='';
							for($j = 0; $j != strlen($f); $j++){
							  $asciiString .= chr(ord($f[$j]));
							}
							$f=$asciiString;
						}
						// normalize the field name, strip to 30 chars if too long
						expdebug::dPrint('field name test before'. $f,5);
						
						/*#custom_attribute_0078975*/
						$f = substr(preg_replace ('/[^0-9a-z]/', '_', $f), 0, 30);
						expdebug::dPrint('field name test after'. $f,5);
						$field_count++;
						$fields[] = $f.' longtext default null';
					}
				}
				// Drop temp table1 if exists
				$this->db->callExecute("drop table if exists ".$tempTab1);
				// Create temp table1
				$qry  = "CREATE TABLE $tempTab1 (" . implode(', ', $fields) . ') ENGINE=InnoDB DEFAULT CHARSET=utf8 ';
				
				expDebug::dPrintDBAPI("Create temprory table 1 --> ",$qry);
				$this->db->callExecute($qry);
			
			}catch(PODException $ex){
				//Need to throw error and inform user
				expDebug::dPrint("PDO Error_in_temp_table_creation",1);
				throw new PDOException("PDO Error_in_temp_table_creation ".$ex->getMessage());
			}
			catch(Exception $ex){
				//Need to throw error and inform user
				expDebug::dPrint("Error_in_temp_table_creation",1);
				throw new Exception("Error_in_temp_table_creation ".$ex->getMessage());
			}
				
				
			try{
				
				//Load csv file item to table
				$sql = "LOAD DATA INFILE '".$file."'
								INTO TABLE ".$tempTab1."
								CHARACTER SET UTF8 
								FIELDS TERMINATED BY '|'
								ENCLOSED BY '\"'
								LINES TERMINATED BY '\\n' IGNORE 1 ROWS";
				expDebug::dPrintDBAPI('field name test before -- ', $sql);
				$this->db->exec($sql);
				
				$this->trimFieldValues();
	
				fclose($handle);
				ini_set('auto_detect_line_endings',FALSE);
				//To remove carriage retrun in the last column
				$column = explode(' ',$fields[sizeOf($fields)-1]);
				$sql = "UPDATE $tempTab1 SET $column[0] = REPLACE($column[0], '\r', '')";
				expDebug::dPrintDBAPI('Remove carriage return -- ',$sql);
				$this->db->callExecute($sql);
				
				$alterqry1 =	" ALTER TABLE $tempTab1 
												ADD column record_status char(15) default 'R', 
												ADD column remarks longtext default null,
												ADD mapping_id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
												ADD INDEX temp_rs1 (record_status)";
				
				if ($JobDetails[0]->entity_type == 'cre_usr') { // add index to employee no
					$alterqry1 .= ", ADD INDEX temp_eno(employee_no(255))";
				}
				expDebug::dPrintDBAPI('Alter temp table 1 query ',$alterqry1);
				$this->db->callExecute($alterqry1);

				// create fedeated table if configured
			//	$this->createFederatedTable($tempTab1);
				
				expDebug::dPrint("Completed table1 creation process",5);
					
			}catch(PDOException $exc){
				expDebug::dPrint("PDO Error_loading_data_from_file".$exc->getMessage(),1);
				throw new PDOException("PDO Error_loading_data_from_file");
			}
			catch(Exception $exc){
				expDebug::dPrint("Error_loading_data_from_file",1);
				throw new Exception("Error_loading_data_from_file");
			}

		}catch(PDOException $e) {
			expDebug::dPrint("PDO Error cannot open the file".print_r($e,true),1);
			/*if($handle){
				fclose($handle);
			}*/
			throw new PDOException("PDO Error cannot open the file ".$e->getMessage());

		}catch(dataLoadException $dlexp) {
			expDebug::dPrint("Error cannot open the file".print_r($dlexp,true),1);
			if($handle){
				fclose($handle);
			}
			throw new dataLoadException($dlexp->message,$this->getUserId(),$this->getMailId(),'cannot_read_file');

		}catch(Exception $ex){
			expDebug::dPrint("Error cannot open the file or temp table creation".print_r($ex->getMessage(),1),1);
			// Close the files if opened.
			if($handle){
				fclose($handle);
			}
			// Delete the Temp table 1 which is created.
			$this->db->callExecute("drop table if exists ".$tempTab1);
			//$this->dropFederatedTable($tempTab1);
			throw new dataLoadException($ex->getMessage(),$this->getUserId(),$this->getMailId(),$ex->getMessage());
	 	}
	}

   private function trimFieldValues(){
		try{ 
			$tempTab1 = $this->getSourceTable();
			
			$fieldsDetailsQry =	"SHOW COLUMNS FROM $tempTab1";
			
			expDebug::dPrintDBAPI('$fieldsDetails Qry - ',$fieldsDetailsQry); 
			
			$this->db->callQuery($fieldsDetailsQry);
			$fieldsDetailsRes = $this->db->fetchAllResults();
			expDebug::dPrint(' $fieldsDetailsRes -  '.print_r($fieldsDetailsRes,true),5);
			
			if(count($fieldsDetailsRes)>0){ // Apply trim for all fields value, which are loaded from feefile
			    $trimfieldset='';
				foreach($fieldsDetailsRes as $fd_key => $fd_val_res){
					$trimfieldset.=','.$fd_val_res->Field.'=TRIM('.$fd_val_res->Field.')';
				} 
				
				if($trimfieldset!=''){
					$trimfieldset=trim($trimfieldset,',');   
					expDebug::dPrint(' $trimfieldset -  '.print_r($trimfieldset,true),5);
					
					$upd_fld_qry = 'UPDATE '.$tempTab1.' SET '.$trimfieldset;
					expDebug::dPrintDBAPI("Trim in Update Query - ",$upd_fld_qry);
					$this->db->callExecute($upd_fld_qry); 
				} 
				
			} 
				
		}catch (Exception $ex) { 
			throw new dataLoadException("Error in trimFieldValues ",$this->getUserId(),$this->getMailId(),$ex->getMessage());
		}
	}  
  
	private function validateSource(){
		try{ 
			$conds = '';
			$tempTab1 = $this->getSourceTable();
             
             //$custom_attr_module_status=$this->getCustomAttributeStatus();
            /* $this->db->callQuery("select status from system where name='exp_sp_administration_customattribute'");
             $custom_attr_module_status = $this->db->fetchColumn();
             expDebug::dPrint('$custom_attr_module_status ='.$custom_attr_module_status,5); */
            
           // $custom_attr_module_status=$this->custom_attr_module_status;
            
            if($this->custom_attr_module_status==1){
            //#custom_attribute_0078975
            $fields = array(
                    '(if(map.reference_column is null or (map.reference_column is not null and map.avail_column = 1),map.data_column,null)) as data_column',
                    '(if(map.reference_column is null or (map.reference_column is not null and map.avail_column = 1),map.base_column,null)) as base_column',
                    '(if(map.key_column=\'unique\',map.base_column,null)) as key_column',
                    '(if(map.key_column=\'unique\',map.data_column,null)) as unique_column',
                    '(if(map.mandatory_column=\'M\',map.data_column,null)) as mandatory_column',
                    '(if(map.indexing = 1,map.data_column,null)) as index_column',
                    'en.base_table',
                    'en.type as entity_type',
                    'jb.file_name',
                    'jb.upload_type',
                    'jb.user_id',
                    'jb.mail_to');
            
            $joins = array(" right Join slt_dataload_entity en on en.id = map.entity_id"," right Join slt_dataload_jobs jb on jb.entity_type = en.type");
            
            $conditons[] = ' jb.job_id = :jobid ';
            $args[':jobid']= $this->getJobId();
            
            $conditons[] = ' map.data_column IS NOT NULL';
    
            $query = $this->db->prepareQuerySelect('slt_dataload_table_mapping map',$fields,$conditons,$joins);
             //#custom_attribute_0078975
             
            //$checkcus = db_query("select count(1) as count from slt_custom_attr_mapping map1 join slt_custom_attr cattr on cattr.id = map1.cattr_id where map1.entity_screen_opt = 1 and cattr.status='cre_cattr_sts_atv' ");
            //$checkcus= $checkcus->fetchField();
            
             $checkcus_qry="select count(1) as count from slt_custom_attr_mapping map1 join slt_custom_attr cattr on cattr.id = map1.cattr_id where map1.entity_screen_opt = 1 and cattr.status='cre_cattr_sts_atv'";
             $this->db->callQuery($checkcus_qry);
             $checkcus = $this->db->fetchColumn(); 
            
            expDebug::dPrint("count" . print_r($checkcus,1),5);
                
            if($checkcus > 0)
            {
            $fields1 = array (
                        '(if(cattr.cattr_name is not null,LOWER(REPLACE(cattr.cattr_name," ","_")),null)) as data_column',
                        '(if(map1.entity_ref_tbl_col is not null
                ,map1.entity_ref_tbl_col,
            null)) as base_column',
                        'null as key_column',
                        '(if(cattr.cattr_unique = 1,
            LOWER(REPLACE(cattr.cattr_name," ","_")),
            null)) as unique_column',
                        '(if(cattr.cattr_mandatory = 1,
            LOWER(REPLACE(cattr.cattr_name," ","_")),
            null)) as mandatory_column',
                        'null as index_column',
                        'en.base_table',
                        'en.type as entity_type',
                        'jb.file_name',
                        'jb.upload_type',
                        'jb.user_id',
                        'jb.mail_to' 
                );
                
                $joins1 = array (
                        " right Join slt_custom_attr cattr on cattr.id = map1.cattr_id and cattr.status='cre_cattr_sts_atv'",
                        " right Join slt_dataload_entity en on en.type = map1.entity_type",
                        " right Join slt_dataload_jobs jb on jb.entity_type = en.type" 
                );
                $conditons1 [] = ' jb.job_id = :jobid ';
                
                $conditons1 [] = 'LOWER(REPLACE(cattr.cattr_name," ","_")) IS NOT NULL';
                
                $conditons1 [] = 'map1.entity_screen_opt = 1';
                
                $querycusatt = $this->db->prepareQuerySelect ( 'slt_custom_attr_mapping map1', $fields1, $conditons1, $joins1);
                expDebug::dPrintDBAPI ( "Get Details custom", $querycusatt, $args );
                $unionquery = "$query UNION ($querycusatt)";
            }
            else {
                $unionquery = $query;
            }
                expDebug::dPrint ( "Get  Details 1" . print_r ( $unionquery, 1 ), 5 );
            
            $fields2 = array (
                    'group_concat(a.data_column) as data_column',
                    'group_concat(a.base_column) as base_column',
                    'group_concat(a.key_column) as key_column',
                    'group_concat(a.unique_column) as unique_column',
                    'group_concat(a.mandatory_column) as mandatory_column',
                    'group_concat(a.index_column) as index_column',
                    'a.base_table as base_table',
                    'a.entity_type as entity_type',
                    'a.file_name as file_name',
                    'upload_type as upload_type',
                    'user_id as user_id',
                    'mail_to as mail_to' 
            );
            $sel = $this->db->prepareQuerySelect ( "($unionquery) a", $fields2 );
            expDebug::dPrint ("Get  Details" . print_r ($sel,1),5);
            $this->db->callQuery($sel,$args);
            $mappingFields = $this->db->fetchAllResults();
            }
            else {
			$fields = array(
					'group_concat(if(map.reference_column is null or (map.reference_column is not null and map.avail_column = 1),map.data_column,null)) as data_column',
					'group_concat(if(map.reference_column is null or (map.reference_column is not null and map.avail_column = 1),map.base_column,null)) as base_column',
					'group_concat(if(map.key_column=\'unique\',map.base_column,null)) as key_column',
					'group_concat(if(map.key_column=\'unique\',map.data_column,null)) as unique_column',
					'group_concat(if(map.mandatory_column=\'M\',map.data_column,null)) as mandatory_column',
					'group_concat(if(map.indexing = 1,map.data_column,null)) as index_column',
					'en.base_table',
					'en.type as entity_type',
					'jb.file_name',
					'jb.upload_type',
					'jb.user_id',
					'jb.mail_to');

			$joins = array(" right Join slt_dataload_entity en on en.id = map.entity_id"," right Join slt_dataload_jobs jb on jb.entity_type = en.type");
			
			$conditons[] = ' jb.job_id = :jobid ';
			$args[':jobid']= $this->getJobId();
			
			$conditons[] = ' map.data_column IS NOT NULL';
	
			$query = $this->db->prepareQuerySelect('slt_dataload_table_mapping map',$fields,$conditons,$joins);
			expDebug::dPrintDBAPI("Get Mapping Details",$query,$args);

			$this->db->callQuery($query,$args);

			$mappingFields = $this->db->fetchAllResults();
            }
			expDebug::dPrint(' Mapping Fields '.print_r($mappingFields,true), 5);
			if(!empty($mappingFields[0]->index_column)){
				$ind = '';
				$indexCol = explode(',',$mappingFields[0]->index_column);
				
				expDebug::dPrint(' $indexCol '.print_r($indexCol,true), 5);
				$conditon = array();
				foreach($indexCol as $col){
					$ind .= (empty($ind)) ? ' add index temp_'.$col.' ('.$col.' (255))':' ,add index temp_'.$col.' ('.$col.' (255))';
					
				}

				$altertab =	"alter table $tempTab1 $ind";
				expDebug::dPrintDBAPI('Alter temp table 1 query ',$altertab);
				$this->db->callExecute($altertab);
			}
			
			if(!empty($mappingFields[0]->mandatory_column)){
				$mandatoryCol = explode(',',$mappingFields[0]->mandatory_column);
				
				expDebug::dPrint(' $mandatoryCol '.print_r($mandatoryCol,true), 5);
				$conditon = array();
                foreach ( $mandatoryCol as $col ) { //#custom_attribute_0078975
                    $conds .= (empty ( $conds )) ? '(' . '`'.$col.'`' . " IS NULL OR " . '`'.$col.'`' . " = ''" . ')' : ' OR ' . '(' . '`'.$col.'`'. " IS NULL OR " . '`'.$col.'`' . " = ''" . ')';
				}
				$conditon[] = $conds;
                $field = array (
                        'record_status' => ':record_status ',
                        'remarks' => ':remarks ' 
                );

				$arg =  array(
										':record_status' => 'IN',
										':remarks'  => 'Mandatory column missing.\r'
										);
				$qry = $this->db->prepareQueryUpdate($tempTab1,$field,$conditon);
				expDebug::dPrintDBAPI(" update mandatory columns ",$qry,$arg);
				$this->db->callExecute($qry,$arg);
			}
            if(!empty($mappingFields[0]->unique_column)){ //#custom_attribute_0078975
                $uniqueCol = explode(',',$mappingFields[0]->unique_column);
                expDebug::dPrint(' $uniqueCol '.print_r($uniqueCol,true), 5);
               
                foreach($uniqueCol as $col){
                $joins = array(' join (SELECT '.$col.'
																FROM '.$tempTab1.' 
                                                                GROUP BY '.$col.'
																HAVING count(1) > 1) t2 
                                                                on  t1.'.$col.' = t2.'.$col);
																
               $flds = array('record_status' => ':record_status ',
                                            'remarks'  => 'CONCAT(IFNULL(remarks, ""), 
                                            "Duplicate '.$col.' within a feed.")');
               $params =  array(':record_status' => 'IN',
                                    ':nul'=>''
               );
               $concustomatt=array(
                    't1.'.$col.' != :nul ',
                    't1.'.$col.' IS NOT NULL'
               );
               
                $query1 = $this->db->prepareQueryUpdate($tempTab1." t1",$flds,$concustomatt,$joins);
				expDebug::dPrintDBAPI("update dataload jobs query1 ",$query1,$params);
				$this->db->callExecute($query1,$params);
                
                expDebug::dPrint('$uniqueCol', 5);
                if($checkcus > 0 && $this->custom_attr_module_status==1)
                {
                     /*   $usrId = db_query("select map1.entity_ref_tbl_col,cusattr.status,cusattr.cattr_datatype,cusattr.cattr_length from slt_custom_attr_mapping map1 join slt_custom_attr cusattr on map1.cattr_id = cusattr.id
                        where LOWER(REPLACE(cusattr.cattr_name,' ','_')) = '$col'");
                
                $usrId= $usrId->fetchAll(); */
                $userIdQuery="select map1.entity_ref_tbl_col,cusattr.status,cusattr.cattr_datatype,cusattr.cattr_length from slt_custom_attr_mapping map1 join slt_custom_attr cusattr on map1.cattr_id = cusattr.id
                        where LOWER(REPLACE(cusattr.cattr_name,' ','_')) = '$col' and map1.entity_type='cre_usr'";
                        
                    expDebug::dPrint('$userIdQuery='.$userIdQuery,5);
                    $this->db->callQuery($userIdQuery);
                    $usrId = $this->db->fetchAllResults();
                    
                    expDebug::dPrint('$usrId='.print_r($usrId,true), 5);
                      if(!empty($usrId) && is_array($usrId)){
                          if($usrId[0]->entity_ref_tbl_col != '')
                          {                 
                            $joins = array(' join (SELECT temp1.'.$col.' as name,temp1.mapping_id as id
                                                                        FROM '.$tempTab1.' temp1
                                    join '.$mappingFields[0]->base_table.' per on per.'.$usrId[0]->entity_ref_tbl_col.' = temp1.'.$col.' and per.user_name != temp1.user_name) t2
                                                                        on  t1.'.$col.' = t2.name and t1.mapping_id = t2.id');
                            $flds = array('record_status' => ':record_status ',
                                    'remarks'  => 'CONCAT(IFNULL(remarks, ""),
                                                    "Duplicate '.$col.'.")');               
                            $query2 = $this->db->prepareQueryUpdate($tempTab1." t1",$flds,$concustomatt,$joins);
                            expDebug::dPrintDBAPI("update dataload jobs query2 >>>>> ",$query2,$params);
                            $this->db->callExecute($query2,$params);
                            
        			         }
        			 }
               }
              }
             }
			

            /* 
			$fld['failure_records'] = ':failure_records ';
			$cond[] = "job_id = :jobid ";	
			$param[':failure_records'] = $failurecnt;
			$param[':jobid'] = $this->getJobId();
			
			$query2 = $this->db->prepareQueryUpdate('slt_dataload_jobs',$fld,$cond);
			expDebug::dPrintDBAPI("update dataload jobs  query2",$query2,$param);
			$this->db->callExecute($query2,$param); */
			try{
			//Call to create destination table
				$qrysel = "select count(1) from ".$tempTab1." where record_status != :record_status or record_status is null or record_status = :nul ";
				$this->db->callQuery($qrysel,array(':record_status'=> 'IN',':nul'=>''));
				$recordcnt = $this->db->fetchColumn();
				if($recordcnt > 0){
					$this->createDestTable($mappingFields);
				}else{
					$fld1['status'] = ':sts ';
					$cond1[] = "job_id = :jobid ";
					$param1[':sts'] = 'CP';
					$param1[':jobid'] = $this->getJobId();
					$query = $this->db->prepareQueryUpdate('slt_dataload_jobs',$fld1,$cond1);
					expDebug::dPrintDBAPI("update dataload jobs  query2",$query,$param1);
					$this->db->callExecute($query,$param1);
					
					$filePath = $this->generateCSVFileFromSourceTable('Completed',$mappingFields[0]->file_name);
					$dlNtf = new dataLoadException;
                                        $jobId = $this->getJobId();
					$notifyDetails = $dlNtf->getNotificationDetails($jobId);
					expDebug::dPrint('notification details 1'.print_r($notifyDetails,true),5);
					if($notifyDetails[0]->entity_type == 'cre_dtl_cls_enr' && $notifyDetails[0]->enroll_type != 'BE'){
						$query = "select course_id as course_id ,title as title,pl.name as type from slt_course_class cls
											left join slt_profile_list_items pl on  pl.code = cls.delivery_type
											where cls.id = :clsid ";
						expDebug::dPrintDBAPI('Class details ', $query,array(':clsid'=>$notifyDetails[0]->class_id));
						$this->db->callQuery($query,array(':clsid'=>$notifyDetails[0]->class_id));
						$class_details = $this->db->fetchAllResults();
						expDebug::dPrint('$class_details details'.print_r($class_details,true),5);
						$subj = 'type>|'.$class_details[0]->type. '~|'.
									'course_title>|' .$class_details[0]->title. '~|'.
								'compeleted>|' . 'completed' . '~|' ;
							$body = 'registration_count>|' . ((!empty($notifyDetails[0]->success_records)) ? $notifyDetails[0]->success_records : 'Nil') . '~|' .
									'rejection_count>|' . ((!empty($notifyDetails[0]->failure_records)) ? $notifyDetails[0]->failure_records : 'Nil');
					}else{
					$subj = 'Data load process';
					$body = 'job_id>|' . $notifyDetails[0]->job_id . '~|' .
									'status>|' . 'Completed' . '~|' .
									'created_by>|' . $notifyDetails[0]->created_by . '~|' .
									'started_on>|' . $notifyDetails[0]->started_on . '~|' .
									'completed_on>|' . $notifyDetails[0]->completed_on . '~|' .
									'registration_count>|' . ((!empty($notifyDetails[0]->total_records)) ? $notifyDetails[0]->total_records : 'Nil') . '~|' .
									'success_records>|' . ((!empty($notifyDetails[0]->success_records)) ? $notifyDetails[0]->success_records : 'Nil') . '~|' .
									'rejection_count>|' . ((!empty($notifyDetails[0]->failure_records)) ? $notifyDetails[0]->failure_records : 'Nil');
					}
					
					$flExist = file_exists($filePath['shortfile']);
					$fileName = ($flExist) ? $filePath['shortfile'] : null;
						
					$dlNtf->generateNotification($subj,$body,$mappingFields[0]->user_id,$mappingFields[0]->mail_to,$fileName,$notifyDetails[0]->entity_type,$jobId);
					//Delete the Temp tables after the process completed.
					//$this->db->callExecute("drop table if exists ".$this->getSourceTable());
				}
			}catch (PDOException $e) {
				expDebug::dPrint("PDO Error in Job Creation".$e->getMessage(),1);
				throw new PDOException($e->getMessage());
			}catch (Exception $ex) {
				expDebug::dPrint("Error in creating temp2 and load data into it",1);
				throw new Exception($ex->getMessage());
			}
			
		}catch (PDOException $e) {
			expDebug::dPrint("PDO Error in Job Creation".$e->getMessage(),1);
			throw new PDOException("Error in source table validation".$e->getMessage());
		}catch (Exception $ex) {
			expDebug::dPrint("Error in validation",1);
			// Delete the Temp tables 1 and 2.
		  $filePath = $this->generateCSVFileFromSourceTable('Failed',$mappingFields[0]->file_name);
		  $this->db->callExecute("drop table if exists ".$tempTab1);
		  $this->db->callExecute("drop table if exists ".$this->getProcessTable());
		 // $this->dropFederatedTable($tempTab1);
		 // $this->dropFederatedTable($this->getProcessTable());
			throw new dataLoadException("Error in source table validation ",$this->getUserId(),$this->getMailId(),'Error_in_validation');
		}
	}
	
	private function createDestTable($mappingFields){
		try{
			$tempTb2 = $this->getProcessTable();
			$tempTab1 = $this->getSourceTable();
			// Create target temp table for other process
			$qry = 'SHOW CREATE TABLE '.$mappingFields[0]->base_table;
			$this->db->callQuery($qry);
			$tblsc = $this->db->fetchAssociate();
			$dbsrc = $tblsc['Create Table'];
			expDebug::dPrint($dbsrc,5);
			
			$crtQry = substr($dbsrc,0,strrpos($dbsrc,')')).' ) ';
			$crtQry .= 'ENGINE=InnoDB ';
			$crtQry .= 'DEFAULT CHARSET=utf8 ';
			$crtQry = str_replace($mappingFields[0]->base_table,$tempTb2,$crtQry);
			//Destination table creation
			$qry1  = "drop table if exists ".$tempTb2;
			expDebug::dPrintDBAPI("createDestTable qry1 ",$qry1);
			$this->db->callExecute($qry1);
			//$qry2  = "CREATE TABLE ".$tempTb2." LIKE ".basename($info['path']).".".$mappingFields[0]->base_table;
			expDebug::dPrintDBAPI("createDestTable qry2 ",$crtQry);
			$this->db->callExecute($crtQry);
			$qry3  = "alter table ".$tempTb2." 
								add column operation char(10),
								add column record_status char(2) default 'R',
								add column primary_id int(11) default null,
								add column batch_id int(11),
								add column mapping_id int(11),
								add column notifyuser longtext default null,
								add key tmp_rec_stat (record_status),
								add index temp_batch_id (batch_id),
								add index temp_oper (operation),
								add index tmp_mapid (mapping_id)";
			
			if ($mappingFields[0]->entity_type == 'cre_usr') {
				$qry3  .= ", add column groups varchar(255) default null";
			}
			expDebug::dPrintDBAPI("createDestTable qry3 ",$qry3);
			$this->db->callExecute($qry3);
			
			// create fedeated table if configured
			//$this->createFederatedTable($tempTb2);
			
			//Insert Query to move Valid from temp1 to temp2
			$insert = array();
			$insert['table'] = $tempTb2;
			$insert['fields'] = array($mappingFields[0]->base_column,'mapping_id');

			$select = array();
			$select['table'] = $tempTab1;
			$select['fields']= array($mappingFields[0]->data_column,'mapping_id');
			$select['condition'] = array('record_status != :record_status or record_status = :r ');
			$arg = array(':record_status' => 'IN' ,':r' =>'R');
			
			if ($mappingFields[0]->entity_type == 'cre_usr') {
				$insert['fields'][] = 'groups';
				$select['fields'][] = 'groups';
			}
			
			$query = $this->db->prepareInsertBySelect($insert,$select);
			expDebug::dPrintDBAPI("Insert by select destination table ",$query,$arg);
			$this->db->callExecute($query,$arg);
			
			//Truncate base table if job is fresh and update
			/* if($mappingFields[0]->upload_type == 1){
				$sql = "truncate ".$mappingFields[0]->base_table;
				$this->db->callExecute($sql);
			} */
			if(!empty($mappingFields[0]->key_column)){
			//Update query to update operation and primary id from base table to temp2
			$select1 = array();
			$selectQry = array();
			$select1['table'] = $tempTb2.' a';
			$select1['fields']= array(
					'b.id as id',
					"a.".$mappingFields[0]->key_column." as code",
			);
			$select1['joins'] = array(
					"inner join ".$mappingFields[0]->base_table." b on if('".$mappingFields[0]->base_table."' = 'slt_profile_list_items' ,CONCAT('".$mappingFields[0]->entity_type."','_',a.".$mappingFields[0]->key_column."),a.".$mappingFields[0]->key_column.") = b.".$mappingFields[0]->key_column
			);
			
			$select1['alias'] = "x";
			$selectQry = array($select1);
			
			$update = array();
			$update['table'] = $tempTb2.' tm';
			$update['fields'] = array(
					'tm.primary_id'=>'x.id',
					'tm.operation' => ':operation'
			);
			$update['condition']=array(
					"tm.".$mappingFields[0]->key_column." = x.code"
			);
			$arg=array(':operation' => 'update');
					
			$updQry1 = $this->db->prepareUpdateBySelect($update,$selectQry);
			expDebug::dPrintDBAPI(" updQry1 ",$updQry1,$arg);
			$this->db->callExecute($updQry1,$arg);
			}
			//Update insert record in opreation field
			$updQry2 = "update ".$tempTb2." set operation = :operation where operation is null";
			expDebug::dPrintDBAPI(" updQry2 ",$updQry2,array(':operation'=>'insert'));
			$this->db->callExecute($updQry2,array(':operation'=>'insert'));

		}catch (PDOException $e) {
			expDebug::dPrint("PDO Error in Job Creation".$e->getMessage(),1);
			throw new PDOException($e->getMessage());
		}catch (Exception $ex) {
			expDebug::dPrint("Error create temp2",1);
			throw new Exception("Error in target temp table creation ".$ex->getMessage());
		}
	}
	
	private function createFederatedTable($tbl){
		$util=new GlobalUtil();
		$conf=$util->getConfig();
		$fedx_url=$conf["FEDX_db_url"];
		$fed_url=$conf["FED_db_url"];
		if(!empty($fedx_url)){
			$info=parse_url($fedx_url);
			$qry = 'SHOW CREATE TABLE '.$tbl;
			$this->db->callQuery($qry);
			$tblsc = $this->db->fetchAssociate();
			$dbsrc = $tblsc['Create Table'];
			//expDebug::dPrint($dbsrc,5);
			$crtQry = substr($dbsrc,0,strrpos($dbsrc,')')).' ) ';
			$crtQry = str_replace($tbl,basename($info['path'])."`.`".$tbl,$crtQry);
			$crtQry .= 'ENGINE=FEDERATED ';
			$crtQry .= 'DEFAULT CHARSET=utf8 ';
			$fedxQry = $crtQry . "CONNECTION='" . $fed_url ."/" .$tbl. "' ; ";
			
			//Drop before create if exists
			$this->db->callExecute('DROP TABLE IF EXISTS '.basename($info['path']).".".$tbl);
			
			expDebug::dPrint($fedxQry,5);
			
			// Create federated tables
			$this->db->callExecute($fedxQry);
		}
	}
	
	function jobCreationErrorHandler(){
		//Status change when error in creation
		$error = error_get_last();
		if(($error['type'] === E_ERROR) || ($error['type'] === E_USER_ERROR)){
				
			$query = "select processed_cnt from slt_dataload_jobs where job_id=:jbid ";
			$this->db->callQuery($query,array(':jbid'=>$this->getJobId()));
			$prcCnt = $this->db->fetchColumn();
			$sts = ($prcCnt == 3) ? 'FL' : 'NS';
			
			$field = array();
			$condition = array();
			$args = array();
			$field['status'] = ':status';
			if($prcCnt == 3){
				$field['result'] = ':result';
				$args[':result'] = 'Failed';
			}
			
			$field['processed_cnt'] = 'processed_cnt+1';
			$condition[] = "job_id = :jobid";

			$args[':status'] = $sts;
			$args[':jobid'] = $this->getJobId();

			$qry = $this->db->prepareQueryUpdate('slt_dataload_jobs',$field,$condition);
			expDebug::dPrintDBAPI("jobCreationErrorHandler --> ",$qry,$args);
			$this->db->callExecute($qry,$args);
			
			// remove temp tables
			$this->db->callExecute("drop table if exists ".$this->getSourceTable());
    		$this->db->callExecute("drop table if exists ".$this->getProcessTable());
		}
	}
	
	public function callStart($jId=''){
		 try{
		 	// # Registering shutdown function
		 	register_shutdown_function(array($this, "jobCreationErrorHandler"));

		 	// Get not started job to process
			$JobDetails = empty($jId) ? $this->getJob('NS') : $this->getJob('','',$jId);
			
			expDebug::dPrint('checking the calla start time starting--->'.microtime(true).'<<___>>'.print_r($JobDetails,1),4);
			if(!empty($JobDetails)){
				$this->insertDataProcessQueue();
				// Update job status as started
				$tblsts = array('tblname'=>'slt_dataload_jobs','status'=>'ST','prestatus'=>1);
				$this->updateJobLoadStatus($tblsts); // $tblsts
				
				//Create and load data in temp table - source
				$this->createSourceTable($JobDetails); 

				// Validate source temp table
				$this->validateSource();
			  
                                
                                $fields = array('count(1)');
				$query = $this->db->prepareQuerySelect($this->getSourceTable(),$fields);
				expDebug::dPrintDBAPI("Translate text query - ",$query);
				$this->db->callQuery($query);
				$totCnt = $this->db->fetchColumn();
				expDebug::dPrint("Test ing ".$totCnt);
				$tblsts = array('tblname'=>'slt_dataload_jobs','total_records'=>$totCnt);
				$this->updateJobLoadStatus($tblsts);
                                
				// Extended function specific to each entity
				$jobId = $this->getJobId();
				$qrysel = "select file_path from slt_dataload_entity en left join slt_dataload_jobs jb on en.type = jb.entity_type where jb.job_id = '".$jobId."'";
				expDebug::dPrint('query select '. $qrysel);
				$this->db->callQuery($qrysel);
				$path = $this->db->fetchColumn();
				
				$file = $_SERVER["DOCUMENT_ROOT"].'/'.$path;
				require_once $file;
				$className = explode('/',$path);
				$className = substr(array_pop($className),0,-4);
				$obj = new $className;
				$batobj = new JobValidation();
				$batchrecords = $batobj->getBatchRecords($this->getJobId());
				$jobDetail = array();
				$jobDetail['jobId']= $this->getJobId();
				$jobDetail['sTable']=$this->getSourceTable(); //temp1
				$jobDetail['pTable']=$this->getProcessTable();
				$obj->extenderValidate($jobDetail,$batchrecords);
				
			
                                // update job status
				$qrysel = "select count(1) from ".$this->getSourceTable()." where record_status=:record_status ";
				$this->db->callQuery($qrysel,array(':record_status'=> 'IN'));
				$failurecnt = $this->db->fetchColumn();
                                
                                $fields = array('count(1)');
				$query = $this->db->prepareQuerySelect($this->getSourceTable(),$fields);
				expDebug::dPrintDBAPI("Translate text query - ",$query);
				$this->db->callQuery($query);
				$totCnt = $this->db->fetchColumn();
				expDebug::dPrint("Test ing ".$totCnt);
				//$rec_process = $totCnt - $failurecnt;
				$tblsts = array('tblname'=>'slt_dataload_jobs','status'=>'PR','total_records'=>$totCnt,'prestatus'=>'','total_time'=> $this->getProcessedTime($this->startTime));
				$stsCnt = array('recprs' => $failurecnt ,'flrec' => $failurecnt);
				$this->updateJobLoadStatus($tblsts,$stsCnt);
				$this->updateJobProcessQueue();
                                
				/* $qrysel = "select status from slt_dataload_jobs where job_id = $this->getJobId() ";
				$this->db->callQuery($qrysel);
				$sts = $this->db->fetchColumn();
				if($sts != 'CP'){
					$fields = array('count(*)');
					
					$query = $this->db->prepareQuerySelect($this->getSourceTable(),$fields);
					expDebug::dPrintDBAPI("Translate text query - ",$query);
					$this->db->callQuery($query);
					$totCnt = $this->db->fetchColumn();
					
					$tblsts = array('tblname'=>'slt_dataload_jobs','status'=>'PR','total_records'=>$totCnt,'prestatus'=>'','total_time'=> $this->getProcessedTime($this->startTime));
					$this->updateJobLoadStatus($tblsts); 
					
					$this->updateJobProcessQueue();
					expDebug::dPrint('checking the call start time ending--->'.microtime(true),1);
				} */
			}
		 }catch (PDOException $e) {
				expDebug::dPrint("PDO Error in getting".$e->getMessage(),1);
				trigger_error('Fatal Error', E_USER_ERROR);
			}
			catch (dataLoadException $dlexp) {
			expDebug::dPrint("Error in main method call dataload exception".print_r($dlexp,1).'error code'.$dlexp->getCode(),1);
			$tblsts = array('tblname'=>'slt_dataload_jobs','status'=>'FL','result'=>$dlexp->getMessage(),'compon'=>'now()','total_time'=>$this->getProcessedTime($this->startTime));
		 	$this->updateJobLoadStatus($tblsts);
		 	$jobId = $this->getJobId();
		 	$notifyDetails = $dlexp->getNotificationDetails($jobId);
		 	expDebug::dPrint('notification details 2'.print_r($notifyDetails,true),5);
		 	if($notifyDetails[0]->entity_type == 'cre_dtl_cls_enr' && $notifyDetails[0]->enroll_type != 'BE'){
				$query = "select course_id as course_id ,title as title,pl.name as type from slt_course_class cls
									left join slt_profile_list_items pl on  pl.code = cls.delivery_type
									where cls.id = :clsid ";
				expDebug::dPrintDBAPI('Class details ', $query,array(':clsid'=>$notifyDetails[0]->class_id));
						$this->db->callQuery($query,array(':clsid'=>$notifyDetails[0]->class_id));
						$class_details = $this->db->fetchAllResults();
						expDebug::dPrint('$class_details details'.print_r($class_details,true),5);
				$subj = 'type>|'.$class_details[0]->type. '~|'.
									'course_title>|' .$class_details[0]->title. '~|'.
						'compeleted>|' . 'completed' . '~|' ;
							$body = 'registration_count>|' . ((!empty($notifyDetails[0]->success_records)) ? $notifyDetails[0]->success_records : 'Nil') . '~|' .
									'rejection_count>|' . ((!empty($notifyDetails[0]->failure_records)) ? $notifyDetails[0]->failure_records : 'Nil');
			}else{
		 	$subj = 'Data load process is failed. ' ;
		 	$body = 'job_id>|' . $notifyDetails[0]->job_id . '~|' .
		 			'status>|' . 'Failed' . '~|' .
		 			'created_by>|' . $notifyDetails[0]->created_by . '~|' .
		 			'started_on>|' . $notifyDetails[0]->started_on . '~|' .
		 			'completed_on>|' . $notifyDetails[0]->completed_on . '~|' .
		 			'registration_count>|' . ((!empty($notifyDetails[0]->total_records)) ? $notifyDetails[0]->total_records : 'Nil') . '~|' .
		 			'success_records>|' . ((!empty($notifyDetails[0]->success_records)) ? $notifyDetails[0]->success_records : 'Nil') . '~|' .
		 			'rejection_count>|' . ((!empty($notifyDetails[0]->failure_records)) ? $notifyDetails[0]->failure_records : 'Nil');
		 	}
		 	
		 	$dlexp->generateNotification($subj,$body,'','','',$notifyDetails[0]->entity_type,$jobId);
		 	
			$this->db->callExecute('delete from slt_dataload_process_queue where job_id= :jbid ',array(':jbid'=>$jobId));
		}
		catch (Exception $ex) {
			expDebug::dPrint("Error in main method call normal exception",1);
			$query = "select processed_cnt from slt_dataload_jobs where job_id=:jbid ";
			$this->db->callQuery($query,array(':jbid'=>$this->getJobId()));
			$prcCnt = $this->db->fetchColumn();
			
			$sts = ($prcCnt == 3) ? 'FL' : 'NS';
			$tblsts = array('tblname'=>'slt_dataload_jobs','status'=>$sts,'prccnt'=>1,'result'=>$ex->getMessage(),'compon'=>'now()','total_time'=>$this->getProcessedTime($this->startTime));
			$this->updateJobLoadStatus($tblsts);
			expDebug::dPrint("Error in data load process ".$ex->getMessage(), 1);
		}
	} 
	
	
} // End of class dataLoadProcess

/**
 * @desc Class for batche prepartion
 * @author Shobana
 *
 */
class batchProcess extends dataLoadBase{
	
	private $startTime;

	function __construct(){
		
		$this->startTime = microtime(true);
		parent::__construct();
	}
	
	public function jobSplittingErrorHandler(){
		$error = error_get_last();
		if(($error['type'] === E_ERROR) || ($error['type'] === E_USER_ERROR)){
			$updQry = "update slt_dataload_jobs set status = :sts,pre_status= :prests where job_id = :jbid ";
			$this->db->callExecute($updQry,array(':sts'=> 'PR', ':prests'=> 'NS', ':jbid'=> $this->getJobId()));
		}
	}
	
	
	public function callSplitBatche($jId='') {
			try {
				//# Registering shutdown function
				register_shutdown_function(array($this,'jobSplittingErrorHandler'));
				expDebug::dPrint('checking the callSplitBatche time starting'.microtime(true),4);
				// Get job which ready for process
				$JobDetails = empty($jId) ? $this->getJob('PR') : $this->getJob('','',$jId);

				if(!empty($JobDetails)){
					$this->insertDataProcessQueue();
					
					$entType = $JobDetails[0]->entity_type;
					
					$query = "select bulk_batch_records from slt_dataload_entity where type=:type ";
					$this->db->callQuery($query,array(':type'=>$entType));
					$batRec = $this->db->fetchColumn();
					
					$tblsts = array('tblname'=>'slt_dataload_jobs','status'=>'IP','prestatus'=>1);
					$this->updateJobLoadStatus($tblsts); 

					// Split batches for insert records
					$insertres = $this->getOpCount('insert');
				 	if(count($insertres) >0 ){
				 		//$firstBatch = 1;
						$this->splitBatches($insertres,'insert',$batRec);
				 	}
						
					// Split batches for update records
					$updateres = $this->getOpCount('update');
					if(count($updateres) >0 ){
						//$firstBatch = $firstBatch+1;
						$this->splitBatches($updateres,'update',$batRec);
					}
						
					// Split batches for upsert records
					$upsertres = $this->getOpCount('upsert');
					if(count($upsertres) >0 ){
						//$firstBatch = $firstBatch+1;
						$this->splitBatches($upsertres,'upsert',$batRec);
					}
					
					// Mark job as completed if all the records are Invalid
					$temp2 = $this->getProcessTable();
					$this->db->callQuery("SELECT COUNT(1) FROM ".$temp2." WHERE record_status !=:in ",array(":in"=> "IN"));
		 			$valid_rec = $this->db->fetchColumn();
		 			expDebug::dPrint("valid record count ".$valid_rec,4);
					if($valid_rec <= 0){
						
						/*$fld1['status'] = ':sts ';
						$cond1[] = "job_id = :jobid ";
						$param1[':sts'] = 'CP';
						$param1[':jobid'] = $this->getJobId();*/
						
						$fld1['status'] = ':sts ';
						$fld1['success_records'] = ':nul ';
						$fld1['failure_records'] = 'total_records';
						$cond1[] = "job_id = :jobid ";
						$param1[':sts'] = 'CP';
						$param1[':jobid'] = $this->getJobId();
						$param1[':nul'] = 0;						
						$query = $this->db->prepareQueryUpdate('slt_dataload_jobs',$fld1,$cond1);
						expDebug::dPrintDBAPI("update dataload jobs  query2",$query,$param1);
						$this->db->callExecute($query,$param1);
							
						//get notification details
						
						$query = " select file_name,user_id,mail_to from slt_dataload_jobs where job_id = :jid ";
						expDebug::dPrintDBAPI("get job details  ",$query,array(':jid'=>$this->getJobId()));
						$this->db->callQuery($query,array(':jid'=>$this->getJobId()));
						//expDebug::dPrintDBAPI($msgStr)
						$jobdetails = $this->db->fetchAllResults();
						
						$filePath = $this->generateCSVFileFromSourceTable('Completed',$jobdetails[0]->file_name);
						$dlNtf = new dataLoadException;
                                                $jobId = $this->getJobId();
						$notifyDetails = $dlNtf->getNotificationDetails($jobId);
						expDebug::dPrint('notification details 3'.print_r($notifyDetails,true),5);
						if($notifyDetails[0]->entity_type == 'cre_dtl_cls_enr' && $notifyDetails[0]->enroll_type != 'BE'){
							$query = "select course_id as course_id ,title as title,pl.name as type from slt_course_class cls
												left join slt_profile_list_items pl on  pl.code = cls.delivery_type
												where cls.id = :clsid ";
							expDebug::dPrintDBAPI('Class details ', $query,array(':clsid'=>$notifyDetails[0]->class_id));
							$this->db->callQuery($query,array(':clsid'=>$notifyDetails[0]->class_id));
							$class_details = $this->db->fetchAllResults();
							expDebug::dPrint('$class_details details'.print_r($class_details,true),5);
							$subj = 'type>|'.$class_details[0]->type. '~|'.
									'course_title>|' .$class_details[0]->title. '~|'.
									'compeleted>|' . 'completed' . '~|' ;
							$body = 'registration_count>|' . ((!empty($notifyDetails[0]->success_records)) ? $notifyDetails[0]->success_records : 'Nil') . '~|' .
									'rejection_count>|' . ((!empty($notifyDetails[0]->failure_records)) ? $notifyDetails[0]->failure_records : 'Nil');
						}else{
							$subj = 'Data load process';
							$body = 'job_id>|' . $notifyDetails[0]->job_id . '~|' .
									'status>|' . 'Completed' . '~|' .
									'created_by>|' . $notifyDetails[0]->created_by . '~|' .
									'started_on>|' . $notifyDetails[0]->started_on . '~|' .
									'completed_on>|' . $notifyDetails[0]->completed_on . '~|' .
									'registration_count>|' . ((!empty($notifyDetails[0]->total_records)) ? $notifyDetails[0]->total_records : 'Nil') . '~|' .
									'success_records>|' . ((!empty($notifyDetails[0]->success_records)) ? $notifyDetails[0]->success_records : 'Nil') . '~|' .
									'rejection_count>|' . ((!empty($notifyDetails[0]->failure_records)) ? $notifyDetails[0]->failure_records : 'Nil');
						}
							
						$flExist = file_exists($filePath['shortfile']);
						$fileName = ($flExist) ? $filePath['shortfile'] : null;
					
						$dlNtf->generateNotification($subj,$body,$jobdetails[0]->user_id,$jobdetails[0]->mail_to,$fileName,$notifyDetails[0]->entity_type,$jobId);
					}
					
					$this->updateRecordsProcessedInJob(array(),$this->getProcessedTime($this->startTime));   
					$this->updateJobProcessQueue(); // Update the SQL_ID as null
					expDebug::dPrint('checking the callSplitBatche time ending--->'.microtime(true),4);
				}
			}catch (PDOException $e) {
				expDebug::dPrint("PDO Error in getting 1212123242q".$e->getMessage(),1);
				trigger_error("Fatal error", E_USER_ERROR);
			}catch (dataLoadException $dlexp) {
				expDebug::dPrint("Error in main method call for batch split custom exception",1);
				
				// Get the processed count and uploaded file name
				$query = "select processed_cnt,file_name from slt_dataload_jobs where job_id=:jbid ";
				$this->db->callQuery($query,array(':jbid'=>$this->getJobId()));
				$prcCnt = $this->db->fetchAllResults();
				expDebug::dPrint("Checking for TEST !!!".print_r($prcCnt,1), 1);
				$sts = ($prcCnt[0]->processed_cnt == 3) ? 'FL' : 'PR';
				if($sts = 'FL'){
					$this->generateCSVFileFromSourceTable('Failed',$prcCnt[0]->file_name);
					//Delete the Temp tables after the process completed.
					$this->db->callExecute("drop table if exists ".$this->getSourceTable());
    			$this->db->callExecute("drop table if exists ".$this->getProcessTable());
    	//		$this->dropFederatedTable($this->getSourceTable());
    	//		$this->dropFederatedTable($this->getProcessTable());
				}
				$tblsts = array('tblname'=>'slt_dataload_jobs','status'=>$sts,'prestatus'=>'','prccnt'=>1,'result'=>$dlexp->getMessage(),'compon'=>'now()','total_time'=>$this->getProcessedTime($this->startTime));
				$this->updateJobLoadStatus($tblsts);
				$jobId = $this->getJobId();
				$notifyDetails = $dlexp->getNotificationDetails($jobId);
				expDebug::dPrint('notification details 4'.print_r($notifyDetails,true),5);
				if($notifyDetails[0]->entity_type == 'cre_dtl_cls_enr' && $notifyDetails[0]->enroll_type != 'BE'){
					$query = "select course_id as course_id ,title as title,pl.name as type from slt_course_class cls
										left join slt_profile_list_items pl on  pl.code = cls.delivery_type
										where cls.id = :clsid ";
					expDebug::dPrintDBAPI('Class details ', $query,array(':clsid'=>$notifyDetails[0]->class_id));
					$this->db->callQuery($query,array(':clsid'=>$notifyDetails[0]->class_id));
					$class_details = $this->db->fetchAllResults();
					expDebug::dPrint('$class_details details'.print_r($class_details,true),5);
					$subj = 'type>|'.$class_details[0]->type. '~|'.
							'course_title>|' .$class_details[0]->title. '~|'.
							'compeleted>|' . 'completed' . '~|' ;
					$body = 'registration_count>|' . ((!empty($notifyDetails[0]->success_records)) ? $notifyDetails[0]->success_records : 'Nil') . '~|' .
							'rejection_count>|' . ((!empty($notifyDetails[0]->failure_records)) ? $notifyDetails[0]->failure_records : 'Nil');
				}else{
				$subj = 'Data load process is failed ' ;
				$body = 'job_id>|' . $notifyDetails[0]->job_id . '~|' .
						'status>|' . 'Failed' . '~|' .
						'created_by>|' . $notifyDetails[0]->created_by . '~|' .
						'started_on>|' . $notifyDetails[0]->started_on . '~|' .
						'completed_on>|' . $notifyDetails[0]->completed_on . '~|' .
						'registration_count>|' . ((!empty($notifyDetails[0]->total_records)) ? $notifyDetails[0]->total_records : 'Nil') . '~|' .
						'success_records>|' . ((!empty($notifyDetails[0]->success_records)) ? $notifyDetails[0]->success_records : 'Nil') . '~|' .
						'rejection_count>|' . ((!empty($notifyDetails[0]->failure_records)) ? $notifyDetails[0]->failure_records : 'Nil');
				}
				
		 		$dlexp->generateNotification($subj,$body,'','','',$notifyDetails[0]->entity_type,$jobId);
		 		$this->db->callExecute('delete from slt_dataload_process_queue where job_id= :jbid ',array(':jbid'=>$this->getJobId())); 
		 	
			}catch (Exception $ex) {
				expDebug::dPrint("Error in main method call for batch split in main exception",1);
				$query = "select processed_cnt,file_name from slt_dataload_jobs where job_id=:jbid ";
				$this->db->callQuery($query,array(':jbid'=>$this->getJobId()));
				$prcCnt = $this->db->fetchAllResults();
				
				$sts = ($prcCnt[0]->processed_cnt == 3) ? 'FL' : 'PR';
				if($sts = 'FL'){
					$this->generateCSVFileFromSourceTable('Failed',$prcCnt[0]->file_name);
					//Delete the Temp tables after the process completed.
					$this->db->callExecute("drop table if exists ".$this->getSourceTable());
					$this->db->callExecute("drop table if exists ".$this->getProcessTable());
			//		$this->dropFederatedTable($this->getSourceTable());
    	//		$this->dropFederatedTable($this->getProcessTable());
				}
				$tblsts = array('tblname'=>'slt_dataload_jobs','status'=>$sts,'prccnt'=>1,'prestatus'=>'','result'=>$ex->getMessage(),'compon'=>'now()','total_time'=>$this->getProcessedTime($this->startTime));
				$this->updateJobLoadStatus($tblsts);
				$this->db->callExecute('delete from slt_dataload_process_queue where job_id= :jbid ',array(':jbid'=>$this->getJobId())); 
			}
		}
		
		private function getOpCount($status){
			try{
				$query = "select COUNT(1) as cnt from ".$this->getProcessTable()." temp where temp.operation=:sts and temp.batch_id IS NULL and record_status != 'IN' ";
				$this->db->callQuery($query,array(':sts'=>$status));
				return $this->db->fetchColumn();
			}catch (Exception $ex) {
				expDebug::dPrint("Error in getting records count",1);
				throw new Exception("Error in getting operation count ".$ex->getMessage());
			}
		}
		
		private function splitBatches($rec,$op,$btchsize){
			try{
				while($rec != 0 && $rec%$btchsize >=0){
					$reccnt = ($rec < $btchsize) ? $rec : $btchsize;
					$this->createBatchId($reccnt,$op,$btchsize);
					$rec = $rec - $btchsize;
					//if($firstBatch == 1){
						$tblsts = array('tblname'=>'slt_dataload_jobs','status'=>'IP','prestatus'=>1);
						$this->updateJobLoadStatus($tblsts);
						//$firstBatch = 0;
			//	}
				}
		
			}catch (Exception $ex) {
				expDebug::dPrint("Error in splitting batches normal exception",1);
				throw new dataLoadException($ex->getMessage(),$this->getUserId(),$this->getMailId(),'Error_in_splitting_batches');
			}
		}
		
		
		private function createBatchId($cnt,$op,$limit){
			try{
				$tbName = $this->getProcessTable();
				$jobId = $this->getJobId();
				$entityInfo = $this->getEntityDetails($jobId);
				$fields = array(
						'job_id' => ':jbid ',
						'job_type' => ':type ',
						'operation' => ':op ',
						'status' => ':sts ',
						'entity_id' => ':entid ',
						'entity_name' => ':entname ',
						//'file_path' => ':fp ',
						'record_count' => ':reccnt ');
				
				$args = array(
						':jbid' => $jobId,
						':type' => $this->getPrcType(),
						':op' => $op,
						':sts' => 'NS',
						':entid' => $entityInfo[0]->ent_id,
						':entname' => $entityInfo[0]->entity_name,
						//':fp' => $entityInfo[0]->path,
						':reccnt' => $cnt);
				
				$qry = $this->db->prepareQueryInsert('slt_dataload_batches',$fields);
				expDebug::dPrintDBAPI("Insert Batch Id ",$qry,$args);
			  $batchId = $this->db->callExecute($qry,$args);
			 
				expDebug::dPrint('fetch result-->>'.$batchId, 3);
				if($batchId){
					// Update TEMP table with Batch Id
					$ord = (!empty($entityInfo[0]->sort)) ? 'order by '.$entityInfo[0]->sort : '';
					$updQry = "update ".$tbName." set batch_id= :btid where operation = :op and batch_id IS NULL and record_status != 'IN' $ord LIMIT $limit";
					expDebug::dPrintDBAPI("Sort temp table befor batch creation ",$updQry,array(':btid'=> $batchId, ':op'=> $op,':ord' => $entityInfo[0]->sort ));
					$this->db->callExecute($updQry,array(':btid'=> $batchId, ':op'=> $op));  
				 	
					// Update the status as RP for Bulk upload
					$status = 'RV';
					if($op == 'update')
						$status = ($this->getBatchStatusCount('insert') != 0) ? 'NS' :$status; 
					
					$tblsts = array('tblname'=>'slt_dataload_batches','status'=>$status,'batchid'=> $batchId,'prestatus'=>1);
					$this->updateJobLoadStatus($tblsts);  
				}
			
			}catch (PDOException $e) {
				expDebug::dPrint("PDO Error in Job Splitting".$e->getMessage(),1);
				throw new PDOException($e->getMessage());
			}catch (Exception $ex) {
				/**
				 * TODO: what about the status of the batch if problem in insert/update
				 */
				expDebug::dPrint("Error in create batch",1);
				throw new Exception("Error in create batch ".$ex->getMessage());
			}
		}
		
		private function getEntityDetails($JobId){
			try{

				$fields = array(
						'de.type as entity_type',
						'de.name as entity_name',
						'de.id as ent_id',
						'de.file_path as path',
						'group_concat(if(map.sorting is not null,map.base_column,null) order by sorting) as sort'
						);
				$joins = array(
						" left Join slt_dataload_entity de on de.type = dj.entity_type",
						" left Join slt_dataload_table_mapping map on map.entity_id = de.id" );
				$query = $this->db->prepareQuerySelect('slt_dataload_jobs dj',$fields,array('dj.job_id = :jbid '),$joins,null);
				expDebug::dPrintDBAPI("Get Job Details ",$query,array(':jbid'=>$JobId));
				// Execute query
				$this->db->callQuery($query,array(':jbid'=>$JobId));
				// Fetch results
				$entityInfo = $this->db->fetchAllResults();
				return $entityInfo;
			}catch (Exception $ex) {
				expDebug::dPrint("Error in getting entity details",1);
				throw new Exception("Error in getting the entity details ".$ex->getMessage());
			}
		}
} // End of class batchProcess

/**
 * @desc Class for job validation record level
 * @author Priya
 *
 */
class JobValidation extends dataLoadBase{
	
	private $startTime;
	
	function __construct(){
		$this->startTime = microtime(true);
		parent::__construct();
	}
	
	public function jobValidationErrorHandler(){
		
		$error = error_get_last();
		if(($error['type'] === E_ERROR) || ($error['type'] === E_USER_ERROR)){
			$condition = array();
			$condition[] = 'job_id = :id ';
			$condition[] = 'id = :btid ';
			$condition[] = 'status NOT IN (:psts , :tsts , :csts )';
			$field = array('status' => ':status','pre_status'  => ':pre_status');
			$arg =  array(':status' => 'RV',':pre_status'  => 'NS',':id' => $this->getJobId(),':btid' => $this->getBatchId(),
					':psts'=> 'PS',':tsts'=>'TR',':csts'=>'CP');
			$qry = $this->db->prepareQueryUpdate('slt_dataload_batches',$field,$condition);
			expDebug::dPrintDBAPI("jobCreationErrorHandler --> ",$qry,$arg);
			$this->db->callExecute($qry,$arg);
		}
	
	}
	
	public function batchValidate($jId=''){
		try{
			//# Registering shutdown function
			register_shutdown_function(array($this,'jobValidationErrorHandler'));
			
			expDebug::dPrint('checking the batchValidate time starting--->'.microtime(true),4);
			$batchdetails = $this->getBatchDetail('RV','IP',$jId);
			
			if(!empty($batchdetails)){
				// Validation for each batch which is ready for validation.
				$JobId = $this->getJobId();
				$batchId = $this->getBatchId();
				expDebug::dPrint('batch details '.print_r($batchdetails,true),5);
				
				$this->insertDataProcessQueue($batchId);
				$tbName = $this->getProcessTable();
				expDebug::dPrint('job Id ' .$JobId .' tab Name '. $tbName, 4);
				
				$tblsts= array('tblname'=>'slt_dataload_batches','status'=>'ST','batchid'=>$batchId,'prestatus'=>1);
				$this->updateJobLoadStatus($tblsts);
				
				$query = "select file_path from slt_dataload_entity where type = :typ ";
			  $this->db->callQuery($query,array(':typ'=>$batchdetails[0]->entity_type));
				$path = $this->db->fetchColumn();
				
				$batchrecords = $this->getBatchRecords($JobId);
				//create executable data temp table
				$this->createExecuteTable();
				
				$file = $_SERVER["DOCUMENT_ROOT"].'/'.$path;
				$objectType = $batchdetails[0]->entity_type;
				$entityName = $batchdetails[0]->entity_name;
				$className = explode('/',$path);
				$className = substr(array_pop($className),0,-4);
				// Call external validator fuunction 
				require_once $file;
				$obj = new $className;
				$jobDetail = array();
				$jobDetail['jobId']=$JobId;
				$jobDetail['batchId']=$batchId;
				$jobDetail['batchopt'] = $this->getBatchOpt();
				$jobDetail['sTable']=$this->getSourceTable(); //temp1
				$jobDetail['pTable']=$this->getExecutTable(); //temp2
				$jobDetail['oType']=$objectType;
				$jobDetail['entity'] = $entityName;
				$jobDetail['uploaded_user_id'] = $batchdetails[0]->user_id;
				try{
					$obj->bulkValidate($jobDetail,$batchrecords);
				}catch(PDOException $ex){
					expDebug::dPrint("Error in Job validation for each batch".$ex->getMessage(),1);
					throw new PDOException($ex->getMessage());
				}
				
				$this->updateStatusExistRec();
				
				$tblsts= array('tblname'=>'slt_dataload_batches','status'=>'RP','batchid'=>$batchId,'prestatus'=>'','time_taken'=>$this->getProcessedTime($this->startTime));
				$this->updateJobLoadStatus($tblsts);
				$this->updateRecordsProcessedInJob(array(),$this->getProcessedTime($this->startTime));
				
				/* $this->db->callQuery("select count(1) from ".$this->getProcessTable()." where record_status= :sts and batch_id = :btid ",array(':sts'=>'IN',':btid'=>$batchId));
				$flCnt = $this->db->fetchColumn();
				
				$this->db->callQuery("select count(1) from ".$this->getProcessTable()." where record_status= :sts and batch_id = :btid ",array(':sts'=>'CP',':btid'=>$batchId));
				$sucCnt = $this->db->fetchColumn();
				
				$arg = array();
				$arg[':jbid'] = $JobId;
				$arg[':frecs'] = $flCnt;
				$arg[':srecs'] = $sucCnt;
				$fields['failure_records'] = 'failure_records + :frecs ';
				$fields['success_records'] = 'success_records + :srecs ';
				$timeTaken = $this->getProcessedTime($this->startTime);
				$totTime = ($timeTaken > 0) ? gmdate('H:i:s',$timeTaken) : '00:00:00';
				$fields["time_taken"] = "ADDTIME(time_taken,:timetaken )";
				$arg[':timetaken'] = $totTime;
				
				$qry = $this->db->prepareQueryUpdate('slt_dataload_jobs',$fields,array('job_id = :jbid '));
				$this->db->callExecute($qry,$arg); */ 
				$this->updateJobProcessQueue(); // Update the SQL_ID as null
				expDebug::dPrint('checking the batchValidate time ending--->'.microtime(true),4);
			}
		}
		catch (PDOException $e) {
			expDebug::dPrint("PDO Error in getting 1212123242q".$e->getMessage(),1);
			trigger_error($e->getMessage(), E_USER_ERROR);
		}catch (Exception $ex) {
			/**
			 * TODO: batch validate failure process
			 */
			expDebug::dPrint("Error in main method for Job Validation -- ".print_r($ex,true),1);
			
			$query = "select processed_cnt from slt_dataload_batches where id=:btid ";
			$this->db->callQuery($query,array(':btid'=>$batchId));
			$prcCnt = $this->db->fetchColumn();
			
			$sts = ($prcCnt == 3) ? 'FL' : 'RV';
			$tblsts= array('tblname'=>'slt_dataload_batches','status'=>$sts,'batchid'=>$batchId,'prccnt'=>1);
			$this->updateJobLoadStatus($tblsts);
			$this->db->callExecute('delete from slt_dataload_process_queue where job_id= :jbid ',array(':jbid'=>$this->getJobId()));
			$this->updateRecordsProcessedInJob(array(),$this->getProcessedTime($this->startTime));
		}
		
	}
	
	private function createExecuteTable(){
		try{
			$tempTb2 = $this->getProcessTable();
			$tempTab1 = $this->getSourceTable();
			$exeTbl	= $this->getExecutTable();
			// Create target temp table for other process
			$qry = 'SHOW CREATE TABLE '.$tempTb2;
			$this->db->callQuery($qry);
			$tblsc = $this->db->fetchAssociate();
			$dbsrc = $tblsc['Create Table'];
			expDebug::dPrint($dbsrc,5);
			
			$crtQry = substr($dbsrc,0,strrpos($dbsrc,')')).' ) ';
			$crtQry .= 'ENGINE=InnoDB ';
			$crtQry .= 'DEFAULT CHARSET=utf8 ';
			$crtQry = str_replace($tempTb2,$exeTbl,$crtQry);
			//Destination table creation
			$qry1  = "drop table if exists ".$exeTbl;
			expDebug::dPrintDBAPI("dropExecTable qry1 ",$qry1);
			$this->db->callExecute($qry1);
			
			expDebug::dPrintDBAPI("createExecTable qry2 ",$crtQry);
			$this->db->callExecute($crtQry);
			
			$qry2 = "INSERT INTO ".$exeTbl." SELECT * FROM ".$tempTb2." WHERE batch_id = ".$this->getBatchId();
			expDebug::dPrintDBAPI("createExecTable qry1 ",$qry2);
			$this->db->callExecute($qry2);
		}catch(Exception $e){
			expDebug::dPrint("ERROR in create executable table -- ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	public function getBatchRecords($JobId){
				
           /* $this->db->callQuery("select status from system where name='exp_sp_administration_customattribute'");
            $custom_attr_module_status = $this->db->fetchColumn();
            expDebug::dPrint('$custom_attr_module_status ='.$custom_attr_module_status,5); */
            
           // $custom_attr_module_status=$this->custom_attr_module_status;
            
           // $custom_attr_module_status=$this->getCustomAttributeStatus();
            
			$fields = array('map.base_column','map.data_column');
				
			$joins = array(" right Join slt_dataload_entity en on en.id = map.entity_id"," right Join slt_dataload_jobs jb on jb.entity_type = en.type");
				
			$conditons[] = ' jb.job_id = :jobid';
			$args[':jobid']= $JobId;
			
			$query = $this->db->prepareQuerySelect('slt_dataload_table_mapping map',$fields,$conditons,$joins); 
            if($this->custom_attr_module_status==1)
            {
            // custom attribute #custom_attribute_0078975
            $conditons1[] = ' jb.job_id = :jobid';
            $conditons1[] = 'map1.entity_screen_opt = 1';
            $fields1 = array('map1.entity_ref_tbl_col as base_column','cattr.cattr_name as data_column');                       
            $joins1 = array(" right Join slt_custom_attr cattr on cattr.id = map1.cattr_id and cattr.status='cre_cattr_sts_atv'"," right Join slt_dataload_entity en on en.type = map1.entity_type"," right Join slt_dataload_jobs jb on jb.entity_type = en.type");
            $query1 = $this->db->prepareQuerySelect('slt_custom_attr_mapping map1',$fields1,$conditons1,$joins1);
            $query = "$query UNION ($query1)";
            }
            
			expDebug::dPrintDBAPI("get Batch Records",$query,$args);
			// Execute query
			$this->db->callQuery($query,$args);
			// Fetch results
			$columnDetails = $this->db->fetchAllResults();
			$colArray = array();
			foreach($columnDetails as $key => $value){
				$colArray[$value->base_column] =  $value->data_column;
				}
			expDebug::dPrint("column details value ".print_r($colArray,true),5);
			return $colArray;

		
			}
					
	private function updateStatusExistRec(){
			
		$fields = array(
				'map.base_column',
				'group_concat(map.base_column) as base_column',
				'group_concat(if(map.key_column=\'unique\',map.base_column,null)) as key_column',
				'group_concat(Distinct(en.base_table)) as base_table'
		);
			
		$joins = array(" right Join slt_dataload_entity en on en.id = map.entity_id"," right Join slt_dataload_jobs jb on jb.entity_type = en.type");
		
		$conditons[] = ' jb.job_id = :jobid';
		$args[':jobid']= $this->getJobId();
	
		$query = $this->db->prepareQuerySelect('slt_dataload_table_mapping map',$fields,$conditons,$joins);
		expDebug::dPrintDBAPI("updateStatusExistRec s",$query,$args);
		// Execute query
		$this->db->callQuery($query,$args);
		// Fetch results
		$columnDetails = $this->db->fetchAllResults();
		if(!empty($columnDetails[0]->key_column)){
		$tblName = $this->getProcessTable();
		$baseCol = explode(',',$columnDetails[0]->base_column);
		expDebug::dPrint('base column '.print_r($baseCol,true),5);
		$condition = array();
		foreach($baseCol as $col){
				$condition[] = "bt.".$col." = t2.".$col;
		}
		$join = array(" join ".$columnDetails[0]->base_table." bt on bt.".$columnDetails[0]->key_column." = t2.".$columnDetails[0]->key_column);
		$updQry = $this->db->prepareQueryUpdate($tblName.' t2 ',array('record_status' => ':sts '),$condition,$join);
		expDebug::dPrintDBAPI("Update temp table 2 --> ",$updQry,array(':sts'=>'CP'));
		$this->db->callExecute($updQry,array(':sts'=>'CP'));
		}
		
		
	}
	
	
} // End of class JobValidation

/**
 * @desc Class for actual dataload
 * @author Shobana
 */
class JobExecution extends dataLoadBase{
	
	private $statusCnt = array('sucrec'=>'','flrec'=>'');
	private $startTime;
	private $tblName;
	function __construct(){
		$this->startTime = microtime(true);
		parent::__construct();
	}
	
	public function jobExecutionErrorHandler() {
		$jobId = $this->getJobId();
		$jobSts = '';
		$error = error_get_last();
		$btId = $this->getBatchId();
		
		expDebug::dPrint("checking the status count".print_r($error,true),4);
		if(($error['type'] === E_ERROR) || ($error['type'] === E_USER_ERROR)){
			expDebug::dPrint("checking the status count".print_r($this->statusCnt,true),4);
			if(count($this->statusCnt) == 0){
				$tbl = $this->tblName;
				$this->db->callQuery("SELECT COUNT(1) FROM $tbl WHERE custom_dataload=:val ",array(":val"=> $btId));
				$sucRec = $this->db->fetchColumn();
				$flRec = 0;
				
				$basecolumn = explode(',',$this->baseCol);
				if(count($basecolumn) > 1)
					$basecolCon = " t2.".$basecolumn[0]." = per.".$basecolumn[0] ." AND t2.".$basecolumn[1]." = per.".$basecolumn[1];
				else
					$basecolCon = " t2.".$basecolumn[0]." = per.".$basecolumn[0];
				
				$t1 = $this->getSourceTable();
				$t2 = $this->getProcessTable();
				$field = array();
				$args = array();
				$condition= array();
				$field['t2.record_status'] = ":csts ";
				$field['t1.record_status'] = ":csts ";
				$field['per.custom_dataload'] = "null";
				$args[':btid'] = $btId;
				$args[':csts'] = 'CP';
				//$args[':cus'] = 1;
				$args[':inv'] = 'IN';
				$condition[] =  " per.custom_dataload = :btid ";
				$condition[] =  "t2.batch_id = :btid ";
				$condition[] =  "t2.record_status != :inv ";
				$join = array(" left join $t1 t1 on t1.mapping_id = t2.mapping_id and t2.batch_id = :btid ", " left join $tbl per on ".$basecolCon);
				$updQry = $this->db->prepareQueryUpdate("$t2 t2",$field,$condition,$join);
				expDebug::dPrintDBAPI("jobExecutionErrorHandler update temp table",$updQry,$args);
				$this->db->callExecute($updQry,$args);
				
			}else{
				$sucRec = (!empty($this->statusCnt['sucrec'])) ? $this->statusCnt['sucrec'] : 0;
				$flRec = (!empty($this->statusCnt['flrec'])) ? $this->statusCnt['flrec'] : 0;
			}
			
			$field = array();
			$args = array();
			$field['status'] = "if(status IN( :csts , :tsts , :fsts , :psts ) , status, :sts )";
			$field['pre_status'] = "if(status IN( :csts , :tsts , :fsts , :psts ) , pre_status, :prests )";
			$field['success_records'] = "success_records + :screc ";
			$field['failure_records'] = "failure_records + :flrec ";
			$args[':jid'] = $jobId;
			$args[':btid'] = $btId;
			$args[':sts'] = 'RP';
			$args[':prests'] = 'RV';
			$args[':screc'] = $sucRec;
			$args[':flrec'] = $flRec;
			$args[':csts'] = 'CP';
			$args[':tsts'] = 'TR';
			$args[':fsts'] = 'FL';
			$args[':psts'] = 'PS';
			
			$updQry = $this->db->prepareQueryUpdate('slt_dataload_batches',$field,array(" job_id = :jid ","id = :btid "));
			expDebug::dPrintDBAPI("jobExecutionErrorHandler batches",$updQry,$args);
			$this->db->callExecute($updQry,$args);
			
			$upfields = array();
			$time = microtime(true) - $this->startTime;
			$totTime = ($time > 0) ? gmdate('H:i:s',$time) : '00:00:00';
			$upfields['success_records'] = 'success_records + :screc ';
			$upfields['failure_records'] = 'failure_records + :flrec ';
			$upfields['time_taken'] = "ADDTIME(time_taken,:ttime )";
			$jbQry = $this->db->prepareQueryUpdate('slt_dataload_jobs',$upfields,array(" job_id = :jid "));
			expDebug::dPrintDBAPI("jobExecutionErrorHandler Jobs ",$jbQry,array(':screc'=>$sucRec,':flrec'=>$flRec,':jid'=>$jobId,':ttime' => $totTime));
			$this->db->callExecute($jbQry,array(':screc'=>$sucRec,':flrec'=>$flRec,':jid'=>$jobId,':ttime' => $totTime));
		}
	
	}
	
	private function batchExecute($item){
		try{
			$i = 0;
			$j = 0;
			//$statusCnt = array();
			$jobId = $this->getJobId();
			$batchId = $this->getBatchId();
			$op = $item[0]->operation;
			//$path = $item[0]->file_path;
			$entityType = $item[0]->entity_type;
			$t1 = $this->getSourceTable();
			//$t2 = $this->getProcessTable();
			$t2 = $this->getExecutTable();
			
			$query = "select file_path from slt_dataload_entity where type = :typ ";
			$this->db->callQuery($query,array(':typ'=>$entityType));
			$path = $this->db->fetchColumn();

			$field = array();
			$condition = array();
			$join = array( 'left join slt_dataload_table_mapping map on map.entity_id = ent.id');
			$field[] = 'api_batch_records as rec';
			$field[] = 'base_table as tbl';
			$field[] = 'group_concat(map.base_column) as unqcol';
			$condition[] = 'type = :type ';
			$condition[] = '(map.key_column = :unq OR map.sorting is not null)';
			$arg=array(':type'=>$entityType, ':unq'=> 'unique');
			$lmtQuery = $this->db->prepareQuerySelect('slt_dataload_entity ent',$field,$condition,$join);
			expDebug::dPrintDBAPI('Query for entity mapping ',$lmtQuery,$arg);
			$this->db->callQuery($lmtQuery,$arg);
			$limit = $this->db->fetchAllResults();
			expDebug::dPrint("Query for entity mapping count - ".count($limit));
			$this->tblName = $limit[0]->tbl;
			$this->baseCol = $limit[0]->unqcol;
			
			try{
				$file = $_SERVER["DOCUMENT_ROOT"].'/'.$path;
				require_once $file;
				$className = explode('/',$path);
				$className = substr(array_pop($className),0,-4);
				$obj = new $className("execute");
				if($this->getPrcType() == 'api'){
					expDebug::dPrint('api batch execution'.microtime(true),4);
				
				$batchRecords= $this->getRecordsFromBatch($op,$limit[0]->rec);
				expDebug::dPrint("batch records to execute in API upload".print_r($batchRecords,true),4);
				foreach($batchRecords as $val){
					$jobSts = $this->getMemCacheValue($jobId);
					expDebug::dPrint("checking the pause and resume status".$jobSts, 4);
					if($jobSts != 'PS' && $jobSts != 'TR'){
						try{
							$resArr = $obj->execute($val);
						}catch (PDOException $e) {
							expDebug::dPrint("PDO Error in API Job Execution".$e->getMessage(),1);
							throw new PDOException($e->getMessage());
						}
						//(!empty($resArr[0]->status) && ($resArr[0]->status == 'success')) ? $i++ : $j++;
						if(($val->operation == 'upsert' && $resArr[0]->status != 'success') || ($val->operation == 'insert' || $val->operation == 'update')){
							$this->updateTempTableStatus($t1,$val->mapping_id,$resArr[0]->status);
							$this->updateTempTableStatus($t2,$val->mapping_id,$resArr[0]->status);
						}else if($val->operation == 'upsert' && $resArr[0]->status == 'success'){
							$this->updateTempTableStatus($t2,$val->mapping_id,$resArr[0]->status,$val->operation);
						}
					}else{
						/* if($jobSts == 'PS'){
							expDebug::dPrint('api batch execution error calling'.microtime(true),4);
							throw new PDOException('Job Paused');
						}else */
							break;
					}
				}
					expDebug::dPrint('api batch execution ending'.microtime(true),4);
					/* $this->statusCnt = array('sucrec'=>$i,'flrec'=>$j); */
				}else{
					// BULK Upload
					expDebug::dPrint('bulk batch execution---->>>'.microtime(true),4);
					$tblDet['tblname'] = $t2;
					$tblDet['btc_id'] = $batchId;
					$tblDet['oper'] = $op;
					$tblDet['ent_id'] = $item[0]->entity_id;
					$tblDet['user_id'] = $item[0]->user_id;
					$tblDet['custom0'] = $item[0]->custom0;
					$tblDet['loadby'] = 'R-'.$batchId;
					$tblDet['enr_type'] = 'Class';
					try{
						$obj->bulkExecute($tblDet);
					}catch (PDOException $e) {
						expDebug::dPrint("PDO Error in BULK Job Execution".$e->getMessage(),1);
						throw new PDOException($e->getMessage());
					}
					
					if($op == 'insert' || $op == 'upsert'){
							
						$field = array();
						$args = array();
						$field['tmp1.record_status'] = "if(tmp1.operation=:oper , :r ,:recsts )";
						$field['tmp1.batch_id'] = "if(tmp1.operation=:oper ,null,tmp1.batch_id)";
						$field['tmp1.operation'] = "if(tmp1.operation=:oper ,:updval ,tmp1.operation)";
						$field['tmp1.custom_dataload'] = "if(tmp1.operation=:oper ,:btid ,tmp1.custom_dataload)";
						$args[':btid'] = $batchId;
						$args[':oper'] = 'upsert';
						$args[':updval'] = 'update';
						$args[':recsts'] = 'CP';
						$args[':r'] = 'R';
						//$args[':cus'] = 1;
						$updQry = $this->db->prepareQueryUpdate($t2.' tmp1',$field,array(" tmp1.batch_id = :btid " , "tmp1.record_status = :r "));
						expDebug::dPrintDBAPI("updateRecordsProcessedInJob --> ",$updQry,$args);
						$this->db->callExecute($updQry,$args);
							
					}else{
						$qry = "update $t2 set record_status=:sts where batch_id = :btid and record_status = :r ";
						$this->db->callExecute($qry,array(':sts'=>'CP',':btid'=>$batchId , ':r' => 'R'));
					}

					if($op != 'upsert'){
							
						$upQry= "update $t1 tmp1,
						(select t2.mapping_id from $t2 t2 left join $t1 t1 on t1.mapping_id=t2.mapping_id
						where t2.batch_id = :btid and t2.record_status = :sts )tmp2
						set tmp1.record_status= :sts
						where tmp1.mapping_id = tmp2.mapping_id";
						$this->db->callExecute($upQry,array(':btid'=>$batchId,':sts'=>'CP'));
					}
				}
				
				$sucquery = "select count(1) from $t2 where (batch_id= :btid and (record_status= :sts and custom_dataload is null)) or (batch_id is null and (record_status= :r and custom_dataload = :cus )) ";
				$args = array();
			  	$args[':btid'] = $batchId;
			  	$args[':sts'] = 'CP'; 
			 	$args[':r'] = 'R';
			  	$args[':cus'] = $batchId;
				$this->db->callQuery($sucquery,$args);
				$sucCnt = $this->db->fetchColumn();
				
				$flquery = "select count(1) from $t2 where batch_id= :btid and ((record_status IN(:fsts , :ists)  and custom_dataload is null) or (record_status= :r and custom_dataload = :cus )) ";
				$args = array();
				$args[':btid'] = $batchId;
				$args[':fsts'] = 'FL';
				$args[':ists'] = 'IN';
				$args[':r'] = 'R';
				$args[':cus'] = $batchId;
				$this->db->callQuery($flquery,$args);
				$flCnt = $this->db->fetchColumn();
				
				$flquery = "select count(1) from $t2 where record_status= :r and operation = :op";
				$args = array();
				$args[':r'] = 'R';
				$args[':op'] = 'update';
				$this->db->callQuery($flquery,$args);
				$reproCnt = $this->db->fetchColumn();

				expDebug::dPrintDBAPI('The value of$flquery is here ',$flquery,$args);
				
				$this->statusCnt = array('sucrec'=>$sucCnt,'flrec'=>$flCnt,'reprorec'=>$reproCnt);
				
				expDebug::dPrint('bulk batch execution end---->>>'.microtime(true),4);
				
				$jobSts = $this->getMemCacheValue($jobId);
				if($jobSts != 'TR' && $jobSts != 'PS'){
				// Update the Base table custom_dataload column to null
				$basecolumn = explode(',',$this->baseCol);
				if(count($basecolumn) > 1)
					$basecolCon = " t2.".$basecolumn[0]." = bastbl.".$basecolumn[0] ." AND t2.".$basecolumn[1]." = bastbl.".$basecolumn[1];
				else
					$basecolCon = " t2.".$basecolumn[0]." = bastbl.".$basecolumn[0];
				
				// To make performance imporvement in enrollment upload the exising design is not allowed to extend some 
				// functionalities, the way we designed for other entities are have unique fieled as basecolumn but in 
				// enrollment the basecolumns are not unique so the query is performing very slow. To overcome this
				// have hardcoded the conditon 
				// TODO: this should be enhanced in core and this specific condition should be removed. 
				if($entityType == "cre_dtl_cls_enr"){
					$basecolCon = " t2.order_id = bastbl.order_id ";
				}
				
				$field = array();
				$args = array();
				$cond = array();
				$field['bastbl.custom_dataload'] = "null";
				//$cond[] = 't2.batch_id = :btid ';
				$cond[] = '(t2.batch_id = :btid  and bastbl.custom_dataload = :btid ) or (t2.custom_dataload = :btid and t2.batch_id = :btid )';
				//$cond[] = 't2.record_status = :recsts ';
				$args[':btid'] = $batchId;
				$args[':recsts'] = 'CP';
				$join = array(" left join ".$t2." t2 on ".$basecolCon);
				$updQry = $this->db->prepareQueryUpdate($this->tblName.' bastbl',$field,$cond,$join);
				expDebug::dPrintDBAPI("Update the custom_dataload column with null ",$updQry,$args);
				$this->db->callExecute($updQry,$args);
				}
				$this->resetTempStatus($batchId);
				return $this->statusCnt;
					
			}catch (PDOException $e) {
				expDebug::dPrint("PDO Error in Batch Execution".$e->getMessage(),1);
				throw new PDOException($e->getMessage());
			}catch(Exception $ex){
				expDebug::dPrint("Error in getting batch records for process",1);
				// Throws exception when the file for execution is not there
				throw new Exception("Error in getting record for batch process ".$ex->getMessage());
			}
		}catch (PDOException $e) {
			expDebug::dPrint("PDO Error in Job Execution".$e->getMessage(),1);
			throw new PDOException($e->getMessage());
		}catch(Exception $ex){
			/**
			 * TODO: Exception handling
			 */
			expDebug::dPrint("Error in batch execute process ".print_r($ex,true),1);
			throw new Exception("Error in getting record for batch process ".$ex->getMessage());
		}
	}
	
	private function resetTempStatus($batchId){
		try{
			$tmp2 = $this->getProcessTable();
			$tmp3 = $this->getExecutTable();
	
			$field = array();
			$args = array();
			$cond = array();
			$field['t2.record_status'] = "t3.record_status";
			$field['t2.operation'] = "t3.operation";
			//$cond[] = 't2.batch_id = :btid ';
			$cond[] = '(t2.batch_id = :btid )';
			//$cond[] = 't2.record_status = :recsts ';
			$args[':btid'] = $batchId;
			$join = array(" left join ".$tmp2." t2 on t3.mapping_id = t2.mapping_id");
			$updQry = $this->db->prepareQueryUpdate($tmp3.' t3',$field,$cond,$join);
			expDebug::dPrintDBAPI("Update the custom_dataload column with null ",$updQry,$args);
			$this->db->callExecute($updQry,$args);
			
		}catch(Exception $e){
			expDebug::dPrint("Error in resetTempStatus ".print_r($e,true),1);
			throw new Exception("Error in resetTempStatus ".$e->getMessage());
		}
	}
	
	private function updateTempTableStatus($tblName,$clmVal,$status='',$op=''){
		try{
			
			$field = array();
			$args = array();
			$batch_id = $this->getBatchId();
			if(empty($op)){
				$field['record_status'] = ':sts';
				$args[':sts'] = ($status == 'success' ? 'CP' : 'FL');
			}else if($op == 'upsert'){
				$field['batch_id'] = 'null';
				$field['operation'] = ':recsts ';
				$field['custom_dataload'] = ':cus ';
				$args[':recsts'] = 'update';
				$args[':cus'] = $batch_id;
			}
			$args[':mapid'] = $clmVal;
			$args[':r'] = 'R';
			$updQry = $this->db->prepareQueryUpdate($tblName,$field,array(" mapping_id = :mapid " , "record_status = :r "));
			expDebug::dPrintDBAPI("updateRecordsProcessedInJob --> ",$updQry,$args);
			$this->db->callExecute($updQry,$args);
			
		}catch(Exception $ex){
			expDebug::dPrint("Error in updating temp table statuses",1);
			throw new Exception("Update temp table record for batch process ".$ex->getMessage());
		}
	}
	
	private function getRecordsFromBatch($op,$limit){
		try{
			
			$temp2 = $this->getProcessTable();
			$condition = array();
			$args = array();
			$condition[] = ' batchtemp.batch_id = :btid ';
			$condition[] = ' batchtemp.record_status = :r ';
			$args[':btid'] = $this->getBatchId();
			$args[':r'] = 'R';
			if($op == 'insert'){
				$condition[] = ' batchtemp.operation IN(:op ,:opsts )';
				$args[':op'] = $op;
				$args[':opsts'] = 'upsert';
			}else{
				$condition[] = ' batchtemp.operation = :op ';
				$args[':op'] = $op;
			}
			$limit = " LIMIT $limit";
			$joins = array(" left join slt_dataload_entity de on de.type = dj.entity_type");
			$query = $this->db->prepareQuerySelect("$temp2 batchtemp",array('*'),$condition,null,$limit);
			expDebug::dPrintDBAPI("Records from getRecordsFromBatch",$query,$args);
			// Execute query
			$this->db->callQuery($query,$args);
			// Fetch results
			$batchRec = $this->db->fetchAllResults();
			return $batchRec;
		}catch(Exception $ex){
			expDebug::dPrint("Error in getting records for processing batch",1);
			throw new Exception("Error in getting record for batch process ".$ex->getMessage());
		}
	}
	
	private function getBatchIdCnt(){
		try{
			$query = "select count(1) as cnt from ".$this->getProcessTable()." where batch_id IS NULL";
			$this->db->callQuery($query,null);
			$resCnt = $this->db->fetchColumn();
			
			return $resCnt;
		}catch(Exception $ex){
			expDebug::dPrint('Error in getting Batch details'.$ex->getMessage(), 1);
		}
	}
	
	public function processBatchRecords($jId=''){
		try{
			// Registering shutdown function
			register_shutdown_function(array($this,'jobExecutionErrorHandler'));
			expDebug::dPrint('checking the processBatchRecords time starting--->'.microtime(true),4);
			$item = $this->getBatchDetail('RP','IP',$jId);
				
			expDebug::dPrint('$item = list in the batches' . print_r($item, true), 4);
			if ($item) {
				$batSts= array('tblname'=>'slt_dataload_batches','status'=>'QP','batchid'=>$item[0]->id,'prestatus'=>1);
				$this->updateJobLoadStatus($batSts);
				$this->insertDataProcessQueue($item[0]->id);
				try{
					expDebug::dPrint("MAXIMUM EXECUTION TIME LIMIT ".ini_get('max_execution_time'), 4);
						
					// Execute the batch records
					$status = $this->batchExecute($item);

					$jobSts = $this->getMemCacheValue($item[0]->job_id);
					if((count($this->statusCnt) != 0) && ($jobSts != 'TR' && $jobSts != 'PS')){

						$query = "select count(1) from ".$this->getProcessTable()." where record_status = :r AND batch_id = :btid ";
						expDebug::dPrintDBAPI("Get Job Details ",$query,array(':btid'=>$item[0]->id, ':r'=>'R'));
						$this->db->callQuery($query,array(':btid'=>$item[0]->id,':r'=>'R'));
						$recCnt = $this->db->fetchColumn();
						//Changed for #70900 by vetrivel.P
						$time = $this->getProcessedTime($this->startTime);
						$sts = ($recCnt == 0) ? 'CP' : (($item[0]->entity_type == 'cre_usr' && $status['reprorec'] !=0) ? 'RV' : 'RP');
						$operation = ($recCnt == 0) ? 'insert' : (($item[0]->entity_type == 'cre_usr' && $status['reprorec'] !=0) ? 'update' : 'insert');
						$stsCnt = array();
						if($sts == 'CP'){
							$temp2 = $this->getProcessTable();
							$sucquery = "select count(1) from $temp2 where (batch_id= :btid and (record_status= :sts and custom_dataload is null)) or (batch_id is null and (record_status= :r and custom_dataload = :cus )) ";
							$args = array();
							$args[':btid'] = $item[0]->id;
							$args[':sts'] = 'CP';
							$args[':r'] = 'R';
							$args[':cus'] = $item[0]->id;
							$this->db->callQuery($sucquery,$args);
							$batSucCnt = $this->db->fetchColumn();
							
							$flquery = "select count(1) from $temp2 where batch_id= :btid and ((record_status IN(:fsts , :ists)  and custom_dataload is null) or (record_status= :r and custom_dataload = :cus )) ";
							$args = array();
							$args[':btid'] = $item[0]->id;
							$args[':fsts'] = 'FL';
							$args[':ists'] = 'IN';
							$args[':r'] = 'R';
							$args[':cus'] = $item[0]->id;
							$this->db->callQuery($flquery,$args);
							$batFlCnt = $this->db->fetchColumn();
							$stsCnt['sucrec'] = $batSucCnt;
							$stsCnt['flrec'] = $batFlCnt;
							
							$tmp3 = $this->getExecutTable();
							$qry1  = "drop table if exists ".$tmp3;
							expDebug::dPrintDBAPI("dropExecTable qry1 after exection ",$qry1);
								
							$this->db->callExecute($qry1);
						}
						
						$batSts= array('tblname'=>'slt_dataload_batches','value'=>$operation,'status'=>$sts,'batchid'=>$item[0]->id,'prestatus'=>'','timetaken'=>$time);
				    $this->updateJobLoadStatus($batSts,$stsCnt);
						$status['recprs'] = $this->statusCnt['sucrec'] + $this->statusCnt['flrec'];

						$this->updateRecordsProcessedInJob($status,$time);
					}

				}/* catch (PDOException $e) {
					expDebug::dPrint("PDO Error in Job Execution".$e->getMessage(),1);
					throw new PDOException($e->getMessage());
				} */catch(Exception $ex){
					$time = $this->getProcessedTime($this->startTime);
					expDebug::dPrint("Error in batch execute calling process",1);
					$this->db->callQuery('SHOW FULL PROCESSLIST');
					$pList = $this->db->fetchAllResults();
					expDebug::dPrint("Process list while error in batch execute ".print_r($pList,true),1);
					// Update the Batch status to RP.
					$query = "select processed_cnt from slt_dataload_batches where id = :btid ";
					$this->db->callQuery($query,array(':btid'=>$this->getBatchId()));
					$prcCnt = $this->db->fetchColumn();

					$sts = ($prcCnt == 3) ? 'FL' : 'RP';
					
					
					$stsCnt = array();
					if($sts == 'FL'){
						$temp2 = $this->getProcessTable();
						$sucquery = "select count(1) from $temp2 where (batch_id= :btid and (record_status= :sts and custom_dataload is null)) or (batch_id is null and (record_status= :r and custom_dataload = :cus )) ";
						$args = array();
						$args[':btid'] = $item[0]->id;
						$args[':sts'] = 'CP';
						$args[':r'] = 'R';
						$args[':cus'] = $item[0]->id;
						$this->db->callQuery($sucquery,$args);
						$batSucCnt = $this->db->fetchColumn();
							
						$flquery = "select count(1) from $temp2 where batch_id= :btid and ((record_status IN(:fsts , :ists)  and custom_dataload is null) or (record_status= :r and custom_dataload = :cus )) ";
						$args = array();
						$args[':btid'] = $item[0]->id;
						$args[':fsts'] = 'FL';
						$args[':ists'] = 'IN';
						$args[':r'] = 'R';
						$args[':cus'] = $item[0]->id;
						$this->db->callQuery($flquery,$args);
						$batFlCnt = $this->db->fetchColumn();
						$stsCnt['sucrec'] = $batSucCnt;
						$stsCnt['flrec'] = $batFlCnt;
						
						$tmp3 = $this->getExecutTable();
						$qry1  = "drop table if exists ".$tmp3;
						expDebug::dPrintDBAPI("dropExecTable qry1 after exection ",$qry1);
							
						$this->db->callExecute($qry1);
						
					}
					
					$batSts= array('tblname'=>'slt_dataload_batches','status'=>$sts,'batchid'=>$item[0]->id,'prccnt'=>1,'timetaken'=>$time);
				  $this->updateJobLoadStatus($batSts,$stsCnt);
					$sucrec = !empty($this->statusCnt['sucrec']) ? $this->statusCnt['sucrec'] : 0;
					$flrec = !empty($this->statusCnt['flrec']) ? $this->statusCnt['flrec'] : 0;
					$status['recprs'] = $sucrec + $flrec;
					$this->updateRecordsProcessedInJob($status,$time);
				}
				expDebug::dPrint("Checking for the result of batches count ".$this->getBatchStatusCount(), 4);
				
				if(($this->getBatchStatusCount()==0) && $jobSts != 'TR'){
					$bIdCnt = $this->getBatchIdCnt();
					if($bIdCnt){
						$jobSts= array('tblname'=>'slt_dataload_jobs','status'=>'PR','prestatus'=>'NS');
						$this->updateJobLoadStatus($jobSts);
					}else{
						expDebug::dPrint('Checking whether the process is completed or not',4);
						$jobSts= array('tblname'=>'slt_dataload_jobs','status'=>'CP','prestatus'=>1,'result'=>'Completed','compon'=>'now()');
						$this->updateJobLoadStatus($jobSts);
						// Remove the process id's created for this job after the job completion.
						$this->db->callExecute("delete from slt_dataload_process_queue where job_id= :jbid ",array(':jbid'=>$item[0]->job_id));


						$filePath = $this->generateCSVFileFromSourceTable('Completed',$item[0]->file_name);
						$dlNtf = new dataLoadException;
                                                $jobId = $this->getJobId();
						$notifyDetails = $dlNtf->getNotificationDetails($jobId);
						expDebug::dPrint('notification details 5'.print_r($notifyDetails,true),5);
						if($notifyDetails[0]->entity_type == 'cre_dtl_cls_enr' && $notifyDetails[0]->enroll_type != 'BE'){
							$query = "select course_id as course_id ,title as title,pl.name as type from slt_course_class cls
												left join slt_profile_list_items pl on  pl.code = cls.delivery_type
												where cls.id = :clsid ";
							expDebug::dPrintDBAPI('Class details ', $query,array(':clsid'=>$notifyDetails[0]->class_id));
							$this->db->callQuery($query,array(':clsid'=>$notifyDetails[0]->class_id));
							$class_details = $this->db->fetchAllResults();
							expDebug::dPrint('$class_details details'.print_r($class_details,true),5);
							$subj = 'type>|'.$class_details[0]->type. '~|'.
									'course_title>|' .$class_details[0]->title. '~|'.
									'compeleted>|' . 'completed' . '~|' ;
							$body = 'registration_count>|' . ((!empty($notifyDetails[0]->success_records)) ? $notifyDetails[0]->success_records : 'Nil') . '~|' .
									'rejection_count>|' . ((!empty($notifyDetails[0]->failure_records)) ? $notifyDetails[0]->failure_records : 'Nil');
						}else{
						$subj = 'Data load process';
						$body = 'job_id>|' . $notifyDetails[0]->job_id . '~|' .
								'status>|' . 'Completed' . '~|' .
								'created_by>|' . $notifyDetails[0]->created_by . '~|' .
								'started_on>|' . $notifyDetails[0]->started_on . '~|' .
								'completed_on>|' . $notifyDetails[0]->completed_on . '~|' .
								'registration_count>|' . ((!empty($notifyDetails[0]->total_records)) ? $notifyDetails[0]->total_records : 'Nil') . '~|' .
								'success_records>|' . ((!empty($notifyDetails[0]->success_records)) ? $notifyDetails[0]->success_records : 'Nil') . '~|' .
								'rejection_count>|' . ((!empty($notifyDetails[0]->failure_records)) ? $notifyDetails[0]->failure_records : 'Nil');
						}

						$flExist = file_exists($filePath['shortfile']);
						$fileName = ($flExist) ? $filePath['shortfile'] : null;
						$dlNtf->generateNotification($subj,$body,$item[0]->user_id,$item[0]->mail_to,$fileName,$notifyDetails[0]->entity_type,$jobId);
						//Delete the Temp tables after the process completed.
						$this->db->callExecute("drop table if exists ".$this->getSourceTable());
						$this->db->callExecute("drop table if exists ".$this->getProcessTable());
						//			$this->dropFederatedTable($this->getSourceTable());
						//		$this->dropFederatedTable($this->getProcessTable());
					}
				}
				expDebug::dPrint("Checking for the result of status count ".$this->getBatchStatusCount('insert'), 4);
				if(($this->getBatchStatusCount('insert')==0) && $jobSts != 'TR'){
					$batSts= array('tblname'=>'slt_dataload_batches','status'=>'RV','prestatus'=>'NS','consts'=>'update');
					$this->updateJobLoadStatus($batSts);
				}
			}
			else {
				// No items currently available to claim.
				return FALSE;
			}

			$this->updateJobProcessQueue(); // Update the SQL_ID as null

			expDebug::dPrint('checking the processBatchRecords time ending--->'.microtime(true),4);
		}catch (PDOException $e) {
			expDebug::dPrint("PDO Error in batch execution".$e->getMessage(),1);
			$this->db->callQuery('SHOW FULL PROCESSLIST');
			$pList = $this->db->fetchAllResults();
			expDebug::dPrint("Process list while error in batch execute ".print_r($pList,true),1);
			trigger_error("Fatal error", E_USER_ERROR);
		}catch(Exception $ex){
			expDebug::dPrint("Error in main method call for batch execution",1);
			$this->db->callQuery('SHOW FULL PROCESSLIST');
			$pList = $this->db->fetchAllResults();
			expDebug::dPrint("Process list while error in batch execute ".print_r($pList,true),1);
			/**
			 * TODO: error handling
			 */
		//	throw new Exception("Error in batch validate ".$ex->getMessage());
		}
	}
}
	
	
class dataLoadException extends Exception {
	
	protected $message;
	/**
	 * @desc User id to send notification
	 * @var Integer
	 */
	private $userId;
	
	/**
	 * @desc Email Id to send notification
	 * @var String
	 */
	private $usrEmail;
	
	private $db;
	
	/**
	 * @desc Notify code is to differentiate the notification.
	 * @var String
	 */
	private $notifyCode;
					
	public function __construct($message=null,$userId='',$email='',$notifyCode=''){
		try{
			//$this->notifyCode = $notifyCode;
			expDebug::dPrint('getting the message value string from exception'.$message,4);
			$this->message = $message;
			$this->usrEmail	= $email;
			$this->userId	= $userId;
			$this->db = new DLDatabase();
		}catch(Exception $ex){
			// Nothing to do
		}
	}
	/**
	 * 
	 * @param unknown_type $errMsg
	 */
	public function generateNotification($subMsg='',$bodyMsg='',$userId='',$emailId='',$fileName='',$data_load_type='', $jobId=''){
		try{
			$userId = ($this->userId) ? $this->userId : $userId;
			$emailId = ($this->usrEmail) ? $this->usrEmail : $emailId;
			//$msg = ($msg) ? $msg : $this->notifyCode;
            $enroll_type = (!empty($jobId)) ? dataLoadBase::getCustomValues($jobId) : '';
			$notificationInfo = array();
			if($data_load_type == 'cre_dtl_cls_enr' && $enroll_type[0]->custom4 != 'BE')
				$notificationInfo['tokens_string'] = 'subject>|'. '~|'. $subMsg . '~|' . $bodyMsg;
			else 
                $notificationInfo['tokens_string'] = 'subject>|' . $subMsg . '~|' . $bodyMsg;
			
			if($fileName){
				$notificationInfo['tokens_string'] .= $notificationInfo['tokens_string'] .
										  '~|job_file_name>|' . $fileName;
			}
			expDebug::dPrint(' $data_load_type = ' . print_r($data_load_type, true) , 4);
			if($data_load_type == 'cre_dtl_cls_enr' && $enroll_type[0]->custom4 != 'BE'){
				$notificationInfo['message_id'] = 'enrollment_process_complete';
			}else{
				$notificationInfo['message_id'] = 'data_load_status';
			}
			$notificationInfo['message_type'] = 'Job Status to User';
			$notificationInfo['send_type'] = 'php mailer';
			$notificationInfo['lang_code'] = 'cre_sys_lng_eng';
			$setLanguage = 'cre_sys_lng_eng';
			
			// Notification for user Id who created the Job
			if(!empty($userId)){
				$drupalUser = $this->getDrupalUserForNotificationInsert($userId,false);
				if(!empty($drupalUser['full_name']))
					$notificationInfo['tokens_string'] .= $notificationInfo['tokens_string'].'~|full_name>|' . $drupalUser['full_name'];
				expDebug::dPrint(' $drupalUser = ' . print_r($drupalUser, true) , 4);
				$user_preffered_language = $drupalUser['preferred_language'];
				$isPreferredNotification = 0 ;
				if(!empty($user_preffered_language) && $user_preffered_language != $setLanguage){
					$isPreferredNotification  = $this->getPreferredNotification($notificationInfo['message_id'],$user_preffered_language);
					if($isPreferredNotification){
						$setLanguage = $user_preffered_language;
					}
				}
				if($isPreferredNotification == 0 ){
					expDebug::dPrint(' notification language = ' . $notificationInfo['lang_code'] , 4);
					if(!empty($notificationInfo['lang_code']) && $notificationInfo['lang_code'] != $setLanguage){
						$isNotificationLanguage  = $this->getPreferredNotification($notificationInfo['message_id'],$notificationInfo['lang_code']);
						expDebug::dPrint(' isNotificationLanguage = ' . $isNotificationLanguage , 4);
						if($isNotificationLanguage){
							$setLanguage = $notificationInfo['lang_code'];
						}
					}
				}
				if($drupalUser['id']){
					$args = array(
							':msgid' => $notificationInfo['message_id'],
							':lng' => $setLanguage,
							':type' => $notificationInfo['message_type'],
							':tknstr' => $notificationInfo['tokens_string'],
							':usrid' => $drupalUser['id'],
							':snd_id' => $drupalUser['id'],
							':snd_name' => $drupalUser['full_name'],
							':snd_ml' => $drupalUser['email'],
							':sndtyp' => $notificationInfo['send_type'],
							':sndsts' => 'N',
							':attc_cnt' => '',
							':crt_by' => $drupalUser['id']
					);
					
					$insFields = array(
							'msg_id' => ':msgid ',
							'lang_code' => ':lng ',
							'msg_type' => ':type ',
							'token_str' => ':tknstr ',
							'user_id' => ':usrid ',
							'send_to_id' => ':snd_id ',
							'send_to_name' => ':snd_name ',
							'send_to_email' => ':snd_ml ',
							'send_type' => ':sndtyp ',
							'send_status' => ':sndsts ',
							'attach_content' => ':sndsts ',
							'created_by' => ':crt_by',
							'created_on' => 'now()'
					);
					$qry = $this->db->prepareQueryInsert('slt_notification',$insFields);
					expDebug::dPrintDBAPI("Insert Notification --> ",$qry,$args);
					$insertId =	$this->db->callExecute($qry,$args);
				}
			}
			
			// Notification for users whose Email Id given in the Job creation.
			if($emailId){
				$mail_id = explode(',',$emailId);
				foreach($mail_id as $val){
					expDebug::dPrint('notification $sendToEmail = ' . $val, 4);
					$args = array(
							':msgid' => $notificationInfo['message_id'],
							':lng' => $setLanguage,
							':type' => $notificationInfo['message_type'],
							':tknstr' => $notificationInfo['tokens_string'],
							':usrid' => 1,
							':snd_id' => '',
							':snd_name' => '',
							':snd_ml' => $val,
							':sndtyp' => $notificationInfo['send_type'],
							':sndsts' => 'N',
							':attc_cnt' => '',
							':crt_by' => 1,
					);
						
					$insFields = array(
							'msg_id' => ':msgid ',
							'lang_code' => ':lng ',
							'msg_type' => ':type ',
							'token_str' => ':tknstr ',
							'user_id' => ':usrid ',
							'send_to_id' => ':snd_id ',
							'send_to_name' => ':snd_name ',
							'send_to_email' => ':snd_ml ',
							'send_type' => ':sndtyp ',
							'send_status' => ':sndsts ',
							'attach_content' => ':sndsts ',
							'created_by' => ':crt_by ',
							'created_on' => 'now()'
					);
					$qry = $this->db->prepareQueryInsert('slt_notification',$insFields);
					expDebug::dPrintDBAPI("Insert Notification for email id --> ",$qry,$args);
					$insertId =	$this->db->callExecute($qry,$args);
					expDebug::dPrint(' $userNotificationId = ' . print_r($insertId, true) , 3);
				}
			}
		} catch (Exception $ex) {
			expDebug::dPrint("Error in notification insert dataload exception",1);
			// Need to do some thing if there is error in generating the notification.
		}
	}
	
	
	private function getDrupalUserForNotificationInsert($personId, $getDrupalLangCode = false){
		try{
			
			$fields = array(
					'usr.uid as id',
					'usr.mail as email',
					'prsn.full_name as full_name',
					'prsn.preferred_language',
					'prsn.status'
					);
			$joins = array();
			$args = array();
			$cond = array();
			$joins[] = ' join users usr on prsn.user_name = usr.name';
			if ($getDrupalLangCode) {
				$joins[] = ' left join slt_profile_list_items lang_spli on prsn.preferred_language = lang_spli.code && lang_spli.lang_code = \'cre_sys_lng_eng\'';
				$args[':lng'] = 'cre_sys_lng_eng';
			}
				$cond[] = 'prsn.id = :perid ';
				$args[':perid'] = $personId;
			if ($getDrupalLangCode) {
				$selectStmt->addField('lang_spli', 'attr1', 'drupal_lang_code');
				$fields[] = 'lang_spli.attr1 as drupal_lang_code';
			}
			$query = $this->db->prepareQuerySelect("slt_person prsn",$fields,$cond,$joins);
			expDebug::dPrintDBAPI("Get Job Details ",$query,$args);
			// Execute query
			$this->db->callQuery($query,$args);
			$result = $this->db->fetchAssociate();
			if (count($result) <= 0) {
				return null;
			}
			expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
			return $result;
			
			
		}catch (Exception $ex) {
			//watchdog_exception('getDrupalUserForNotificationInsert', $ex);
		}
	}
	
	private function getPreferredNotification($msgId, $languageCode){
		try{
			$join = array(' left join slt_notification_frame frame on frame.notification_id = notiinfo.id');
			$cond = array();
			$cond[] = 'notiinfo.notification_code = :notcode ';
			$cond[] = 'frame.lang_code = :langcode ';
			$cond[] = 'notiinfo.status = :sts ';
			$query = $this->db->prepareQuerySelect("slt_notification_info notiinfo",array('COUNT(1) as cnt'),$cond,$join);
			expDebug::dPrintDBAPI("Get Job Details ",$query,array(':notcode' => $msgId,':langcode' =>$languageCode,':sts'=>'cre_ntn_sts_atv'));
			// Execute query
			$this->db->callQuery($query,array(':notcode' => $msgId,':langcode' =>$languageCode,':sts'=>'cre_ntn_sts_atv'));
			$result = $this->db->fetchColumn();
			expDebug::dPrint('getPreferredNotification result'.print_r($result,true), 4);
			return $result;
			
		}catch (Exception $ex) {
			// TODO : Need to handle 
		}
	}
	/**
	 * @desc get Notification Details for Job
	 * @author Priya
	 * @return notification details in array
	 */
	public function getNotificationDetails($job_id){
		$join = array(' left join slt_person per on jb.user_id = per.id');
		$fields = array('jb.job_code as job_id',
										'per.full_name as created_by' ,
										'jb.starts_when as started_on',
										'jb.entity_type as entity_type',
										'jb.custom0 as class_id',
                                                                                'jb.custom4 as enroll_type',
										'jb.completed_on as completed_on',
										'jb.total_records as total_records',
										'jb.records_processed as records_processed',
										'jb.success_records as success_records',
										'jb.failure_records as failure_records');
		$cond = array();
		$cond[] = 'jb.job_id = :jobId ';
		$args = array(':jobId'=>$job_id);
		$query = $this->db->prepareQuerySelect("slt_dataload_jobs jb",$fields,$cond,$join);
		expDebug::dPrintDBAPI("getNotificationDetails ",$query,$args);
		$this->db->callQuery($query,$args);
		// Fetch results
		$notifyDetails = $this->db->fetchAllResults();
		return $notifyDetails;
	}
}

/**
 * @desc Class for Pause/Terminate the Job process
 * @author Shobana
 */
class JobTermination extends dataLoadBase{
	
	/**
	 * @desc have the job code to pause/terminate
	 * @var string
	 */
	private $jobCode;
	
	/**
	 * @desc have the process id to terminate or pause
	 * @var string
	 */
	//private $prcId;
	
	/**
	 * @desc get the process Id of that Job code specified
	 * @return process Id - integer
	 */
	private function getProcessId(){
		$val = array(); 
		$joins = array(' left join slt_dataload_jobs jbs on jbs.job_id=prc.job_id');
		$sel = $this->db->prepareQuerySelect('slt_dataload_process_queue prc',array('prc.pid','prc.sql_id'),array('jbs.job_code = :jbid '),$joins);
		expDebug::dPrintDBAPI("Translate text query - ",$sel,array(':jbid'=>$this->jobCode));
		$this->db->callQuery($sel,array(':jbid'=>$this->jobCode));
		$pId = $this->db->fetchAllResults();
		foreach($pId as $key=>$value){
			$val[] = $value;
		}
		return $val;
	}
	/**
	 * @desc Terminate/Pause the processing job.
	 * @param string $jobCode
	 */
	public function setJobProcessingStatus($jobCode,$status,$output){
		try{
		
			$this->jobCode = $jobCode;
			$jobDet = $this->getJob('',$this->jobCode);
			
			$pId = $this->getProcessId();
			expDebug::dPrint("CHECKING THE PROCESS IDS".print_r($pId,true),4);
			$sts = $jobDet[0]->status;
			if($status == 'pause'){
				$response = $this->pauseJobProcess($output,$pId,$sts,$jobDet);
			}else if($status == 'resume'){
				$response = $this->resumeJobProcess($output,$sts);
			}else{
				$response = $this->terminateJobProcess($output,$pId,$jobDet);
			}
			return $response;
		}catch(Exception $e){
			expDebug::dPrint("Error in main method call for terminate/pause/resume",1);
			// Handle Exception if any error occurs
		}
	}
	
	private function terminateJobProcess($output,$pId,$jobDet){
		try{
			expDebug::dPrint("JOB DETAILS DEBUG".print_r($jobDet,true),4);
			$job_id = $this->getJobId();
			$sts = $jobDet[0]->status;
			$jobInfo['Return_Type'] = $output;
			$jobInfo['Job_Id'] = $this->jobCode;
			$jobInfo['Status_Type'] ='';
			$killQuery = '';
			if($sts != 'FL' && $sts != 'TR' && $sts != 'CP'){
				$jobInfo['Message'] = 'Process terminated successfully';
				$batSts= array('tblname'=>'slt_dataload_batches','status'=>'TR');
				$this->updateJobLoadStatus($batSts);
					
				$jobSts= array('tblname'=>'slt_dataload_jobs','status'=>'TR','result'=>'Terminated','compon'=>'now()');
				$this->updateJobLoadStatus($jobSts);
				
				foreach($pId as $val){
					expDebug::dPrint('process id to kill the job process'.print_r($val,1));
					$this->killProcess($val->pid);
					$this->db->callExecute("delete from slt_dataload_process_queue where pid= :id and job_id= :jbid ",array(':id'=>$val->pid,':jbid'=>$job_id));
					if(!empty($val->sql_id))
						$killQuery .= 'kill ' . $val->sql_id .'; ';
				}
					
				try{
					if(!empty($killQuery)){
						$this->db->callExecute($killQuery);
					}
				}catch(PDOException $p){
					expDebug::dPrint("Error in process kill -- ".$p->getMessage(),1);
				} 

				$entType = $jobDet[0]->entity_type;
				$this->db->callQuery("select base_table from slt_dataload_entity where type = '$entType'");
				$baseTable = $this->db->fetchColumn();
				
				$this->db->callQuery('select count(1) as cnt,custom_dataload as batId from '.$baseTable.' where custom_dataload is not null group by custom_dataload');
				$cnt = $this->db->fetchAllResults();
				expDebug::dPrint('count for test'. print_r($cnt,true),5);
				$recCnt = 0;
				foreach($cnt as $key => $val){
					$field = array('success_records' => 'success_records + :sucrec ');
					$arg[':sucrec'] = $val->cnt;
					$arg[':bid'] = $val->batId;
					$updQry = $this->db->prepareQueryUpdate('slt_dataload_batches',$field,array('id = :bid'));
					$this->db->callExecute($updQry,$arg);
					$recCnt += $val->cnt;
				}
				$rec['sucrec'] = $recCnt;
				$rec['recprs'] = $recCnt;
				if(!empty($recCnt)){
					$this->updateRecordsProcessedInJob($rec);
				
					$updQry = "update ".$baseTable." set custom_dataload = null where custom_dataload is not null ";
					$this->db->callExecute($updQry);
				}
				
				$filePath = $this->generateCSVFileFromSourceTable('Completed',$jobDet[0]->file_name);
					
				/* $subj = 'Job process have been terminated successfully.';
				$body = 'Job process have been terminated successfully.'; */
				$dlNtf = new dataLoadException;
                                $jobId = $this->getJobId();
				$notifyDetails = $dlNtf->getNotificationDetails($jobId);
				expDebug::dPrint('notification details 6'.print_r($notifyDetails,true),5);
				expDebug::dPrint('notification details type '.print_r($notifyDetails[0]->entity_type,true),5);
				
				if($notifyDetails[0]->entity_type == 'cre_dtl_cls_enr' && $notifyDetails[0]->enroll_type != 'BE'){
					$query = "select course_id as course_id ,title as title,pl.name as type from slt_course_class cls
										left join slt_profile_list_items pl on  pl.code = cls.delivery_type
										where cls.id = :clsid ";
					expDebug::dPrintDBAPI('Class details ', $query,array(':clsid'=>$notifyDetails[0]->class_id));
					$this->db->callQuery($query,array(':clsid'=>$notifyDetails[0]->class_id));
					$class_details = $this->db->fetchAllResults();
					expDebug::dPrint('$class_details details'.print_r($class_details,true),5);
					$subj = 'type>|'.$class_details[0]->type. '~|'.
							'course_title>|' .$class_details[0]->title. '~|'.
							'compeleted>|' . 'completed' . '~|' ;
					$body = 'registration_count>|' . ((!empty($notifyDetails[0]->success_records)) ? $notifyDetails[0]->success_records : 'Nil') . '~|' .
							'rejection_count>|' . ((!empty($notifyDetails[0]->failure_records)) ? $notifyDetails[0]->failure_records : 'Nil');
				}else{
				$subj = 'Data load process';
				$body = 'job_id>|' . $notifyDetails[0]->job_id . '~|' .
								'status>|' . 'Terminated' . '~|' .
								'created_by>|' . $notifyDetails[0]->created_by . '~|' .
								'started_on>|' . $notifyDetails[0]->started_on . '~|' .
								'completed_on>|' . $notifyDetails[0]->completed_on . '~|' .
								'registration_count>|' . ((!empty($notifyDetails[0]->total_records)) ? $notifyDetails[0]->total_records : 'Nil') . '~|' .
								'success_records>|' . ((!empty($notifyDetails[0]->success_records)) ? $notifyDetails[0]->success_records : 'Nil') . '~|' .
								'rejection_count>|' . ((!empty($notifyDetails[0]->failure_records)) ? $notifyDetails[0]->failure_records : 'Nil');
				}
								
				$flExist = file_exists($filePath['shortfile']);
				$fileName = ($flExist) ? $filePath['shortfile'] : null;
				$dlNtf->generateNotification($subj,$body,$jobDet[0]->user_id,$jobDet[0]->mail_to,$fileName,$notifyDetails[0]->entity_type,$jobId);
					
				$this->db->callExecute("drop table if exists ".$this->getSourceTable());
				$this->db->callExecute("drop table if exists ".$this->getProcessTable());
				//		$this->dropFederatedTable($this->getSourceTable());
				//	$this->dropFederatedTable($this->getProcessTable());
				$response = $this->getJobStatus($jobInfo);

				
			}else{
				$jobInfo['Message'] = 'Cannot terminate the job';
				$response = $this->getJobStatus($jobInfo);
			}
			return $response;
		}catch(Exception $e){
			expDebug::dPrint("Error in main method call for terminate",1);
			// Handle Exception if any error occurs
		}
	}
	
	
	private function pauseJobProcess($output,$pId,$sts,$jobDet){
		try{
			$killQuery = '';
			$job_id = $this->getJobId();
			$jobInfo['Return_Type'] = $output;
			$jobInfo['Job_Id'] = $this->jobCode;
			$jobInfo['Status_Type'] ='';
			if($sts != 'FL' && $sts != 'TR' && $sts != 'PS' && $sts != 'CP'){
				$jobInfo['Message'] = 'Process paused successfully';
				$batSts= array('tblname'=>'slt_dataload_batches','status'=>'PS','prestatus'=>1);
				$this->updateJobLoadStatus($batSts);

				$jobSts= array('tblname'=>'slt_dataload_jobs','status'=>'PS','prestatus'=>1);
				$this->updateJobLoadStatus($jobSts);

				foreach($pId as $val){
					expDebug::dPrint('process id to kill the job process'.print_r($val,1));
					$this->killProcess($val->pid);
					$this->db->callExecute("delete from slt_dataload_process_queue where pid= :id and job_id= :jbid ",array(':id'=>$val->pid,':jbid'=>$job_id));
					if(!empty($val->sql_id))
						$killQuery .= 'kill ' . $val->sql_id .'; ';
				}
				try{
					if(!empty($killQuery)){
						$this->db->callExecute($killQuery);
					}
				}catch(PDOException $p){
					expDebug::dPrint("Error in process kill -- ".$p->getMessage(),1);
				}
				
				$entType = $jobDet[0]->entity_type;
				$this->db->callQuery("select base_table from slt_dataload_entity where type = '$entType'");
				$baseTable = $this->db->fetchColumn();
				
				$this->db->callQuery('select count(1) as cnt,custom_dataload as batId from '.$baseTable.' where custom_dataload is not null group by custom_dataload');
				$cnt = $this->db->fetchAllResults();
				$recCnt = 0;
				foreach($cnt as $key => $val){
					$field = array('success_records' => 'success_records + :sucrec ');
					$arg[':sucrec'] = $val->cnt;
					$arg[':bid'] = $val->batId;
					$updQry = $this->db->prepareQueryUpdate('slt_dataload_batches',$field,array('id = :bid'));
					$this->db->callExecute($updQry,$arg);
					$recCnt += $val->cnt;
				}
				$rec['sucrec'] = $recCnt;
				$rec['recprs'] = $recCnt;
				if(!empty($recCnt)){
					$this->updateRecordsProcessedInJob($rec);

					$updQry = "update ".$baseTable." set custom_dataload = null where custom_dataload is not null ";
					$this->db->callExecute($updQry);
				} 
					
			}else{
				$jobInfo['Message'] = 'Cannot pause the job';
			}
			$response = $this->getJobStatus($jobInfo);
			return $response;
		}catch(Exception $e){
			expDebug::dPrint("Error in main method call for pause",1);
			// Handle Exception if any error occurs
		}
	}
	
	private function resumeJobProcess($output,$sts){
		try{
			
			$job_id = $this->getJobId();
			$jobInfo['Return_Type'] = $output;
			$jobInfo['Job_Id'] = $this->jobCode;
			$jobInfo['Status_Type'] ='';
			
			$this->setMemCacheValue($job_id);
			
			if($sts=='PS'){
				$jobInfo['Message'] = 'Process resumed successfully';
				$upBatchQry = 'update slt_dataload_batches set status = if(pre_status IN( :rv ,:rp , :fl , :cp , :tr ), pre_status,if(pre_status = :qp , :rp ,if(pre_status= :ns ,:ns ,:rv ))) where job_id= :jbid ';
				$arg = array(':rv'=>'RV',':rp'=>'RP',':qp'=>'QP',':ns'=>'NS',':fl' =>'FL',':cp' => 'CP',':tr'=>'TR',':jbid'=>$job_id);
				expDebug::dPrintDBAPI('Resume the batch process', $upBatchQry,$arg);
				$this->db->callExecute($upBatchQry,$arg);
				
				$upJobQry = 'update slt_dataload_jobs set status = if(pre_status IN (:ns , :pr , :ip ) , pre_status,if(pre_status= :bs , :pr , :ns )) where job_id= :jbid ';
				$jbarg = array(':ns'=>'NS',':pr'=>'PR',':bs'=>'BS',':ip'=>'IP',':jbid' => $job_id);
				expDebug::dPrintDBAPI('Resume the job process', $upJobQry,$jbarg);
				$this->db->callExecute($upJobQry,$jbarg);
				
			}else{
				//$jobInfo['Message'] = 'Cannot resume the job';
				/** TODO : Update the message when the job is not paused. */
			}
			$response = $this->getJobStatus($jobInfo);
			return $response;
		}catch(Exception $e){
			expDebug::dPrint("Error in main method call for resume",1);
			// Handle Exception if any error occurs
		}
	}
	
	private function killProcess($pid) {
		try {
			$status = 0;
			$phpUname = strtolower(php_uname('s'));
			expDebug::dPrint('$phpUname = ' . print_r($phpUname, true), 4);
			if (stripos($phpUname, 'win') > -1) {
				$winTaskkillCmd = 'taskkill.exe';
				/* require_once($_SERVER['DOCUMENT_ROOT'].'/sites/all/services/GlobalUtil.php');
				$gutil = new GlobalUtil();
				$expConfig = $gutil->getConfig();
				$configWinTaskkillCmd = trim($expConfig['win_taskkill_cmd_fullpath']); // Windows: Full path of the windows taskkill.exe command
				// Run 'where taskkill' at the command prompt to know the full path
				// Default location: C:\Windows\System32\taskkill.exe
				if (!empty($configWinTaskkillCmd)) {
					$winTaskkillCmd = $configWinTaskkillCmd;
				} */
				expDebug::dPrint('$winTaskkillCmd = ' . $winTaskkillCmd, 4);
					
				$killCmd = $winTaskkillCmd . " /F /PID $pid";
			}
			else {
				$linuxKillCmd = 'kill';
				/* require_once($_SERVER['DOCUMENT_ROOT'].'/sites/all/services/GlobalUtil.php');
				$gutil = new GlobalUtil();
				$expConfig = $gutil->getConfig();
				$configLinuxKillCmd = trim($expConfig['linux_kill_cmd_fullpath']); // Linux: Full path of the kill command
				//        Run 'which kill' in the terminal to know the full path
				// Default location: /bin/kill
				if (!empty($configLinuxKillCmd)) {
					$linuxKillCmd = $configLinuxKillCmd;
				} */
				expDebug::dPrint('$linuxKillCmd = ' . $linuxKillCmd, 4);
				$killCmd = $linuxKillCmd . " -9 $pid";
			}
			expDebug::dPrint('$killCmd = ' . $killCmd, 4);
			exec($killCmd);
	
			// If pid was for an existing process, it should have been killed by above.
		}
		catch (Exception $ex) {
			expDebug::dPrint('Throwing new killProcess exception', 4);
		}
	}
}

/**
 * Since data load is not using Drupal,
 * but some of the pages we have Drupal's t() function
 * this is the override of the existing one
 * @param $str
 * @return unknown_type
 */
/* if(!function_exists('t')){
	function t($str){
		//return $str;
		if(!preg_match('/^(MSG|ERR|LBL)+[0-9]/i',$str))
			return $str;
		$db = new DLDatabase();
		$arg = array();
		$fields = array(
			's.lid', 
			's.source', 
			's.context', 
			't.translation', 
			't.language'
		);
		$joins = array(
			'LEFT JOIN locales_target t ON s.lid = t.lid AND t.language = :lang'
		);
		$arg[':lang'] = 'en-us';
		$condition = array(
			's.textgroup = :group',
			//'s.version = :version', 
			's.source = :source'
		);
		$arg[':group'] = 'default';
		//$arg[':version'] = '7.4';
		$arg[':source'] = $str;
		
		$query = $db->prepareQuerySelect('locales_source s',$fields,$condition,$joins);
		expDebug::dPrintDBAPI("Translate text query - ",$query,$arg);
		$db->callQuery($query,$arg);
		$result = $db->fetchResult();
		return !empty($result->translation)?$result->translation:$str;
	}
} */
?>