<?php 

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

require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);


$path = $_POST['path'];
$logId = $_POST['logId'];
$method = $_POST['method'];
$solrUrl = $_POST['solrUrl'];
$uid = $_POST['uid'];

//expDebug::dPrint("POST DATA -- ".print_r($_POST,true),4);

try{
	$check=TRUE;
	
	do{
		usleep(500000); //Wait for half a second

		$url = $solrUrl .'/'. $path . '?command=status&indent=on&wt=json';
		//expDebug::dPrint("Solr url -- ".print_r($url,true),4);
    $srcRst = doRequest($url);
    
    expDebug::dPrint("DataImport status check -- ".print_r($srcRst,true),4);
    
		if($srcRst['status'] == 'idle' || empty($srcRst))
			$check=FALSE;
	}while($check==TRUE);
	
	if(empty($srcRst)){
		$srcRst['responseHeader']['status'] = 'failure';
		$srcRst['response'] = 'Error in request';
	}
	$logDet = array(
					'status'=>$srcRst['responseHeader']['status'],
					'response'=>escape_string(serialize($srcRst)),
					'updated_by'=>$uid,
					'updated_on'=>now()
				);
	
	$qry = db_update('slt_solr_sync_log');
	$qry->fields($logDet);
	$qry->condition('id',$logId,'=');
	expDebug::dPrintDBAPI("Solr log update ",$qry);
	$logId = $qry->execute();
	
}catch(Exception $e){
	expDebug::dPrint("Error in Solr Sync log -- ".print_r($e,true),1);
}


	/**
	 * Return the current status of the on going process
	 * @param $path (String) URL
	 * @return (Object) Result set of current state
	 */
	function doRequest($url){
		try{
		 	$headers[] = 'Accept: application/json, text/plain, */*'; 
			$headers[] = 'Connection: Keep-Alive'; 
			$headers[] = 'Content-type: application/x-www-form-urlencoded;charset=UTF-8';
			$curl = curl_init();
			$curl_options = array(
	        //CURLOPT_COOKIEJAR => NULL,
	        //CURLOPT_URL => $url,
	        CURLOPT_FOLLOWLOCATION => FALSE,
	        CURLOPT_RETURNTRANSFER => TRUE,
	        CURLOPT_SSL_VERIFYPEER => FALSE, // Required to make the tests run on https.
	        CURLOPT_SSL_VERIFYHOST => FALSE, // Required to make the tests run on https.
	        CURLOPT_HEADER => FALSE,
	        //CURLOPT_BINARYTRANSFER => TRUE,
	        CURLOPT_TIMEOUT => 10,
	        CURLOPT_CONNECTTIMEOUT => 10,
	        CURLOPT_HTTPHEADER => $headers,
	        CURLOPT_USERAGENT => 'Mozilla/5.0 (Windows NT 10.0; WOW64; rv:51.0) Gecko/20100101 Firefox/51.0',
	      );
	     curl_setopt_array($curl, $curl_options);
	     
	     curl_setopt($curl, CURLOPT_URL, $url);
			$srcRst = curl_exec($curl);
			curl_close($curl);
			return drupal_json_decode($srcRst);
		}catch(Exception $e){
			expDebug::dPrint("Error in status check ".print_r($e,true),1);
		}
	}
	
?>