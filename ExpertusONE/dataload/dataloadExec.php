<?php 
$key='';
$path='';
$process='';
if (in_array($argv, array('--help')) || $argc != 5) {
  ?>
  This is a script to run cron from the command line.
 
  It takes 2 arguments, which must both be specified:
  --key is the job operation in which is to be
  invoked.
  --document-root is the path to your drupal root directory for
  your website.
 
  Usage:
  php dataloadExec.php --key Job_operation --document-root '/path/to/drupal/root'
 
<?php
} else {

	// Split passed arguments
	for ($i = 1; $i < $argc; $i++) {
		switch ($argv[$i]) {
			case '--key':
				$i++;
				$key = $argv[$i];
				break;
			case '--process':
				$i++;
				$process = $argv[$i];
				break;
			case '--document-root':
				$i++;
				$path = $argv[$i];
				break; 
		}
	}
}
$_SERVER['DOCUMENT_ROOT'] = empty($path) ? $_SERVER['DOCUMENT_ROOT'] : $path;

// Includes 

// Invoke respective class for actual execution
$_SERVER['HTTP_HOST'] = 'default';
$_SERVER['REMOTE_ADDR'] = '127.0.0.1';
$_SERVER['SERVER_SOFTWARE'] = NULL;
$_SERVER['REQUEST_METHOD'] = 'GET';
$_SERVER['QUERY_STRING'] = '';
$_SERVER['HTTP_USER_AGENT'] = 'console';
  
include_once $_SERVER['DOCUMENT_ROOT']."/sites/all/services/GlobalUtil.php";
include_once $_SERVER['DOCUMENT_ROOT']."/sites/all/services/Trace.php";
define('DRUPAL_ROOT', $_SERVER["DOCUMENT_ROOT"]);


if(isset($_POST) && !empty($_POST)){ 
	// Invoke background process
	$dpath = $_POST['path'];
	$proc = $_POST['process'];
	$util=new GlobalUtil();
	$config=$util->getConfig();
	$exec = $config['cli_cron_path'].'/php '.$dpath.'/dataload/dataloadExec.php  --process '.$proc.' --document-root '.$dpath;
	expDebug::dPrint('dataload exec Command '.print_r($exec, 1),4);
	exec($exec,$op,$ret);
	expDebug::dPrint('dataload exec output '.print_r($op, 1),4);
	expDebug::dPrint('dataload exec return'.print_r($ret, 1),4);
	
}else if(empty($process)){
	
	// Initial call when cron get invoked by scheduled job

  include_once $_SERVER['DOCUMENT_ROOT']."/dataload/Database.php";
	
  $db = new DLDatabase();
  $qry  = "select name,value from variable where name in (:cron,:mode)";
  $arg = array(':cron'=>'cron_key',':mode'=>'maintenance_mode');
  $db->callQuery($qry,$arg);
  $rst = $db->fetchAllResults();
  $cronKey='drupal';
  $mode=0;
  foreach($rst as $rest){
  	if($rest->name == 'cron_key') $cronKey = unserialize ($rest->value);
  	if($rest->name == 'maintenance_mode') $mode = unserialize ($rest->value);
  }
	// Cron key validation
	if (!isset($key) || $cronKey != $key) {
	  die("Cron could not run because an invalid key was used");
	} // Maintanance mode validation
	elseif ($mode) {
	  die("Cron could not run because the site is in maintenance mode.");
	}
	else {
		// Invoke process as asynchronous 
		include_once $_SERVER['DOCUMENT_ROOT']."/sites/all/services/GlobalUtil.php";
		include_once $_SERVER['DOCUMENT_ROOT']."/dataload/Database.php";
		$util=new GlobalUtil();
		$config=$util->getConfig();
		$url = $config['admin_site_url']."/dataload/dataloadExec.php";
		$peerVerify = $config['peer_verify'] == 0 ? FALSE : TRUE;
		$db = new DLDatabase();
		$qrysel = "select count(1) from slt_dataload_jobs where status in (:ns ,:pr ,:ip )";
		$db->callQuery($qrysel,array(':ns'=> 'NS',':pr' => 'PR', ':ip' => 'IP'));
		$recordcnt = $db->fetchColumn();
		expDebug::dPrint(" record count ". $recordcnt, 4);
		if($recordcnt > 0){
			for($i=0;$i<4;$i++){
				if($i==0){
					$p['process']='processJobCreation';
				}else if($i==1){
					$p['process']='processJobSplitting';
				}else if($i==2){
					$p['process']='processJobValidation';
				}else if($i==3){
					$p['process']='processJobExecution';
				}
				$p['path']=$_SERVER['DOCUMENT_ROOT'];
				callCronProcess($url,$p,$peerVerify);
			}
		}
	}
}else{
	include_once $_SERVER['DOCUMENT_ROOT']."/sites/all/services/GlobalUtil.php";
	include_once $_SERVER['DOCUMENT_ROOT'].'/dataload/exp_sp_data_load.php';
	include_once $_SERVER['DOCUMENT_ROOT']."/dataload/Database.php";
	try{
expDebug::dPrint('dataload process initiate '.$process);
		switch($process){
			case "processJobCreation":
				$obj = new dataLoadProcess();
				$obj->callStart();
				break;
			case "processJobSplitting":
				$obj = new batchProcess();
				$obj->callSplitBatche();
				break;
			case "processJobValidation":
				$obj = new JobValidation();
				$obj->batchValidate();
				break;
			case "processJobExecution":
				$obj = new JobExecution();
				$obj->processBatchRecords();
				break;
			}
	}catch(Exception $e){
		expDebug::dPrint("Error in ".$process." -- ".$e->getMessage());
		// TODO: any implement for process exception
	}
}

function callCronProcess($url, $data = array(),$peerVerify=FALSE) {
  expDebug::dPrint('call dataload '.$url);
expDebug::dPrint('func args dataload '.print_r(func_get_args(), 1), 4);
try {
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
  //It returns a resource that can be used with all the commands that fsockopen resources can.
  /*$fp = fsockopen(
    $host,
    $port,
    $errno,
    $errstr,
    30
  );*/
  
  $contextOptions = array(
    'ssl' => array(
        'verify_peer' => $peerVerify, // You could skip all of the trouble by changing this to false, but it's WAY uncool for security reasons.
  			'verify_peer_name'=>$peerVerify
    )
	);
	$context = stream_context_create($contextOptions);
	$fp = stream_socket_client("{$host}:{$port}", $errno, $errstr, 20, STREAM_CLIENT_CONNECT, $context);
  
  if (!$fp){
    return "Error $errno: $errstr";
  }else{
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
    return TRUE;
  }
}catch(Exception $ex) {
expDebug::dPrint('dataload exception thrown '.$ex->getMessage(), 4);
expDebug::dPrint(print_r($ex), 4);
}
}

?>