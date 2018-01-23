<?php 
class dataloadDirect{
	
	private $config;
	
	function __construct(){
		
	}
	
	public function start($data=array()){
		$_SERVER['DOCUMENT_ROOT'] = $data['document_root'];
		define('DRUPAL_ROOT', $_SERVER["DOCUMENT_ROOT"]);
		$_SERVER['REMOTE_ADDR'] = $data['remote_addr'];;
		include_once DRUPAL_ROOT . "/sites/all/services/GlobalUtil.php";
		include_once DRUPAL_ROOT . "/dataload/Database.php"; 
		include_once DRUPAL_ROOT . "/sites/all/services/Trace.php";
		
		if(isset($data['operation']) && $data['operation']=='Instant'){
			// Instant process wait till the entire process completes
			$result = $this->instantValidate($data);
			if($result == 'Validation Success'){
				return $this->instantProcess($data['id']);
			}else{
				return $result;
			}
		}else{
			// Instant process start and run in backround, UI will notwait for
			// the process to complete
			if(isset($_POST['id']))
				$this->instantProcess($data['id']);
			else{
				$result = $this->instantValidate($data);
				if($result == 'Validation Success'){
					return $this->requestCall($data);
				}else{
					return $result;
				}
			}
		}
	}
	
	private function instantValidate($data=array()){
		$util=new GlobalUtil();
		$this->config=$util->getConfig();
			
		// Check whether the process can be a instant or not
		$db = new DLDatabase();
		$db->connect();
		$db->query('select file_name, entity_type, file_size from slt_dataload_jobs where id = :id ',array(':id'=>$data['id'])); //Changed for #70900 by vetrivel.P
		$filedetail = $db->fetchAssociate();
		expDebug::dPrint("File detail = ".print_r($filedetail,true),4);
		
		$file = $this->config['dataload_file_path'] . "/" . $filedetail['file_name'];
		$size = intval(preg_replace('/[a-zA-Z]+/','',$filedetail['file_size']));
		//055580: Instant upload not happening when the file size is in decimal value.
		//$measure = preg_replace('/[0-9]+/','',$filedetail['file_size']);
		$measure = preg_replace('/[0-9\.]+/','',$filedetail['file_size']);
		$entity_type = $filedetail['entity_type'];
		if($entity_type == 'cre_usr' || ($measure == 'B' || ($measure == 'KB' && $size < 150))){ //Added by vetrivel.P for #0070900
			$fp = file($file);
			$instantCount = $this->config['instant_load_count'];
			if($entity_type == 'cre_usr'){ //Changed for #70900 by vetrivel.P
				$instantCount = $this->config['instant_user_load_count'];
			}
			expDebug::dPrint('The value of $instantCount is here '.print_r($instantCount,1),3);
			if(count($fp) > $instantCount+1){
				expDebug::dPrint("Must be run as backend job, because record count is exceeding the limit",4);
				return "Backend Process"; // Process should run as backend job since the records count exceeded the limit 
			}
		}else{
			expDebug::dPrint("Must be run as backend job, because file size is exceeding the limit",4);
			return "Backend Process"; // Process should run as backend job since the file size exceeded the limit 
		}
		return 'Validation Success';
	}
	
	private function requestCall($data = array()) {
		try {
			
			// Instant load initiated
			$url = $this->config['admin_site_url']."/dataload/dataloadDirectExec.php";
			$peerVerify = $this->config['peer_verify'] == 0 ? FALSE : TRUE;
		  $parts = parse_url($url);
		  $host = $parts['host'];
		  if (isset($parts['port'])) {
		    $port = $parts['port'];
		  }
		  else {
		    $port = $parts['scheme'] == 'https' ? 443 : 80;
		  }
		  if ($port == 443) {
		    $host = 'ssl://' . $host;
		  }
		 
		  $data_params = array();
		  foreach ($data as $k => $v) {
		    $data_params[] = urlencode($k) . '=' . urlencode($v);
		  }
		  $data_str = implode('&', $data_params);
		 
		  //sockopen doesn't support stream contexts, so use stream_socket_client instead.
		  /*$fp = fsockopen(
		    $host,
		    $port,
		    $errno,
		    $errstr,
		    10
		  );*/
		  
		   $contextOptions = array(
		    'ssl' => array(
		        'verify_peer' => $peerVerify, // You could skip all of the trouble by changing this to false, but it's WAY uncool for security reasons.
		  			'verify_peer_name'=>$peerVerify
		    )
			);
			$context = stream_context_create($contextOptions);
			$fp = stream_socket_client("{$host}:{$port}", $errno, $errstr, 20, STREAM_CLIENT_CONNECT, $context);
		 
		  if (!$fp) {
		    return "Error $errno: $errstr";
		  }else {
		    $out = "POST " . $parts['path'] . " HTTP/1.1\r\n";
		    $out .= "Host: " . $parts['host'] . "\r\n";
		    $out .= "Content-Type: application/x-www-form-urlencoded\r\n";
		    if (!empty($data_str)) {
		      $out .= "Content-Length: " . strlen($data_str) . "\r\n";
		    }
		    $out .= "Connection: Close\r\n\r\n";
		    if (!empty($data_str)) {
		      $out .= $data_str;
		    }
				expDebug::dPrint('out sock'.print_r($out, 1));
		    fwrite($fp, $out);
		    fclose($fp);
		   	sleep(2);
		    return "Successfully Started";
		  }
		}catch(Exception $ex) {
			expDebug::dPrint('dataload exception thrown '.$ex->getMessage(), 4);
			expDebug::dPrint(print_r($ex), 4);
		}
	}
	
	private function instantProcess($jobId){
		try{
				include_once 'exp_sp_data_load.php';
				
				// Call dataload process for create temp tables
			 	$dlObjCr = new dataLoadProcess();
				$dlObjCr->callStart($jobId);
				
				// Call data load batch spliting 
				$dlObjBat = new batchProcess();
				$dlObjBat->callSplitBatche($jobId);
				
				// Call data load validation
				$dlValidate = new JobValidation();
				$dlValidate->batchValidate($jobId);
				
				// Call data laod execution
				$dlExecute = new JobExecution();
				$dlExecute->processBatchRecords($jobId); 
				
				//Changed for #70900 by vetrivel.P
				$db = new DLDatabase();
				$db->connect();
				$db->query('select job.entity_type, bth.operation, bth.status from slt_dataload_jobs job inner join slt_dataload_batches bth on bth.job_id = job.job_id  where job.id = :id ',array(':id'=>$jobId));
				$filedetail = $db->fetchAllResults();
				
				foreach($filedetail as $file){
					if($file->entity_type == 'cre_usr' && $file->operation == 'update' && $file->status == 'RV'){
						// Call data load validation
						$dlValidate->batchValidate($jobId);
						// Call data laod execution
						$dlExecute->processBatchRecords($jobId);
					}
				}
				
				
				return "Successfully Completed";
		}catch(Exception $e){
			throw new Exception($e);
		}
	}
}

if(isset($_POST['id'])){
	$obj =  new dataloadDirect();
	$enRt = $obj->start($_POST);
}
?>