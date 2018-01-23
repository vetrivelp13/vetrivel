<?php 

class SolrClient {
	// List of collections are defined as constant 
	const CatalogCore = 'catalog';
	const UserCore = 'user';
	const EnrollCore = 'enrollment';
	
	// Zookeeper URL
	private $zookeeper;
	
	// Solr Node URL
	private $solrnode;
	
	private $peerVerify;
	
	// Action of the Solr process
	private $action;
	
	// To manage logs
	protected $isLog = FALSE;
	
	// Collection Name
	protected $collName;
	
	protected $responseType = 'json';
	
	protected $dataImportType = '';
	
	function __construct(){
		try{

			$this->peerVerify = getConfigValue('peer_verify') == 0 ? FALSE : TRUE;
			
			$this->zookeeper = getConfigValue('zooclient');
			
			$this->getSolrServer();
		}catch(Exception $e){
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * Active Solr node's URL will be stored in $solrnode variable
	 * zookeeper client
	 * @return NA
	 */
	protected function getSolrServer(){
		$context = stream_context_create(array(
		'http' => array(
				'method' => 'POST',
				'header' => 'Content-Type: application/x-www-form-urlencoded; charset=utf-8',
				'content' => 'op=getsolrurl'
				),
			'ssl' => array('verify_peer'=>$this->peerVerify, 
           'verify_peer_name'=>$this->peerVerify)
		));
		$this->solrnode = file_get_contents($this->zookeeper.'/zooconnect.php', false, $context);
		if($this->solrnode === FALSE){
			expDebug::dPrint("Error on connecting to Zookeeper client. Check the URL ".$this->zookeeper.'/zooconnect.php',1);
			throw new Exception('Error on connecting to Zookeeper client');
		}
	}
	
	/**
	 * Return active Solr node URL
	 * @return URL
	 */
	protected function getSolrUrl(){
		if(empty($this->solrnode))
			$this->getSolrServer();
		return $this->solrnode;
	}
	
	/**
	 * Retrive search results from Solr
	 * @param $collection (String) Collection name 
	 * @param $data (Array) Parameters of the request
	 * @param $method (String) GET/POST
	 * @return (Object) Result sets
	 */
	protected function getData($collection,$data,$method){
		$this->action = '/select';
		$path = $collection.$this->action;
		
		// Set search result's return type
		$outMode = isset($_REQUEST['returntype'])?$_REQUEST['returntype']:'json';
		$this->responseType = $outMode;
		if($outMode == "xml")
			$outMode= "xslt&tr=e1xmlout.xsl";
		
		$data .= '&wt='.$outMode;
		
		try{
			switch(strtoupper($method)){
				case 'POST':
					return $this->sendPostRequest($path,$data);
					break;
				case 'GET':
					return $this->sendGetRequest($path,$data);
					break;
			}
		}catch(Exception $e){
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * Create/Recreate index in Solr using Solr's data import API
	 * @param $collection (String) Collection name
	 * @param $options (Array) Data import parameters
	 * @return (Object) Data import's result
	 */
	protected function reCreateIndex($collection,$options=array()){
		$this->action = '/dataimport';
		$path = $collection.$this->action;
		try{
			return $this->sendPostRequest($path,$options);
		}catch(Exception $e){
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * Generate POST request
	 * @param $path (String) URL
	 * @param $data (Array) Parameters
	 * @return (Object) Result
	 */
	private function sendPostRequest($path,$data){
		try{
			// Prepare Post data
			$param = $this->preparePost($data);
			return $this->sendRequst($path,$param,'POST');
		}catch(Exception $e){
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * Generate GET request
	 * @param $path (String) URL
	 * @param $data (String) Paramters (should be in proper format)
	 * @return (Object) Result
	 */
	private function sendGetRequest($path,$data){
		try{
			// Uppend path and data
			$strdata = $path.'?'.$data;
			return $this->sendRequst($strdata,'','GET');
		}catch(Exception $e){
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * Execute the request and send back the results
	 * @param $path (String) URL
	 * @param $param (String) Paramters (should be in proper format)
	 * @param $method (String) GET/POST
	 * @return (Object) Result set
	 */
	private function sendRequst($path,$param='',$method){
		try{
			$url = $this->solrnode; // Base URL
			expDebug::dPrint("Solr request BASE URL - ".$url,4);
			
			// Log the request and its paramters if log enabled
			$pData = !empty($param) ? $param : $path;
			$logId = $this->log(null,$pData,$method);
			
			$curl = $this->cURLInit($url); // Initialize cURL 
			if($method == 'POST'){
				expDebug::dPrint("POST Param ".print_r($param,true),4);
				curl_setopt($curl, CURLOPT_POSTFIELDS, $param); // Set post param
			}
			
			$url.= '/'.$path; // Construct full URL
			
			expDebug::dPrint("Solr request URL - ".$url,4);
			curl_setopt($curl, CURLOPT_URL, $url);
			
			$response = curl_exec($curl);

			expDebug::dPrint("Solr Search Results -- ".print_r($response, true),4);
			curl_close($curl);
			
			if($this->responseType == 'json')
				$response = drupal_json_decode($response);

			$srcRst = $this->completionCheck($path,$response,$logId,$method);
			
			// Log the response if log enabled
			$this->log($logId,$data,$method,$srcRst);
			
			// If API Request
			if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname']) && stripos($path,'dataimport')===FALSE){
				$_REQUEST['SolrImpl'] = TRUE;
			}
			return $srcRst;
		}catch(Exception $e){
			expDebug::dPrint("Error in sendRequest - ".print_r($e,true),1);
			$srcRst = empty($srcRst)? $e->getMessage() : $srcRst;
			$this->log($logId,$data,$method,$srcRst);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * In case of dataimport request Solr's API will not return the result 
	 * after completing the entire porcess, it initiate the process and send
	 * response with the current state (busy/idel). 
	 * 
	 * While the process is in busy state system needs to keep check the
	 * process status till it complete.
	 * @param $path (String) URL
	 * @param $srcRst (Object) Result set of initial request
	 * @return (Object) Final result set
	 */
	private function completionCheck($path,$srcRst,$logId,$method){
		if(stripos($path,'dataimport')!==FALSE){
			$check=TRUE;
			//$i=0;
			do{
				usleep(1000); //Wait for 1000 micro seconds
				$srcRst = $this->checkStatus($path.'?command=status&indent=on&wt=json');
				expDebug::dPrint("DataImport status check -- ".print_r($srcRst,true),4);
				
				// Skip back end off line process if it is a bulk import
				/*if($this->dataImportType == 'Bulk'){
					$i=0;
				}*/
				
				if($srcRst['status'] == 'idle' || empty($srcRst))
					$check=FALSE;
				if($i>=3 && $this->dataImportType == 'Login'){
					$check=FALSE;
					$url = getConfigValue('admin_site_url')."/sites/all/modules/core/exp_sp_solr/exp_sp_solr_dataimport_log.php";
					$uid = getIdOfLoggedInUser();
					$data = array(
									'logId'=>$logId,
									'method'=>$method,
									'path'=>$path,
									'solrUrl'=>$this->solrnode,
									'uid'=>$uid);
					$this->callOffLineLogProcess($url,$data,FALSE);
				}
				$i++;
			}while($check==TRUE);
		}
		return $srcRst;
	}
	
	/**
	 * Return the current status of the on going process
	 * @param $path (String) URL
	 * @return (Object) Result set of current state
	 */
	private function checkStatus($path){
		$url = $this->solrnode .'/' . $path;
		$curl = $this->cURLInit($url);
		curl_setopt($curl, CURLOPT_URL, $url);
		$srcRst = curl_exec($curl);
		curl_close($curl);
		return drupal_json_decode($srcRst);
	}
	
	/**
	 * Convert Array to Object
	 * @param $d (Array)
	 * @return (Object)
	 */
	protected function arrayToObject($d) {
   if (is_array($d)) {
	   /*
	    * Return array converted to object
	    * Using __METHOD__ (Magic constant)
	    * for recursive call
	    */
      return (object) array_map(__METHOD__, $d);
    }else {
      // Return object
      return $d;
    }
	}
	
	/**
	 * Initialize cURL 
	 * @return (Object) cURL resourse id
	 */
	private function cURLInit(){
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
        CURLOPT_TIMEOUT => 300,
        CURLOPT_CONNECTTIMEOUT => 300,
        CURLOPT_HTTPHEADER => $headers,
        CURLOPT_USERAGENT => 'Mozilla/5.0 (Windows NT 10.0; WOW64; rv:51.0) Gecko/20100101 Firefox/51.0',
      );
     curl_setopt_array($curl, $curl_options);
     return $curl;
	}
	
	/**
	 * Prepare post field from given parameters
	 * @param $data Array
	 * 		Post file paramters with value
	 * @return Post fields String
	 */
	private function preparePost($data){ // http_build_query
		return http_build_query($data);
	}
	
	/**
	 * Prepare field sets for log
	 * @param $data (String) Request parameters
	 * @param $method (String) GET/POST
	 * @param $response (Object) Result set
	 * @return (Array)
	 */
	private function preparelogData($data,$method,$response=''){
			$uid = getIdOfLoggedInUser();
			if(empty($response)){
				$logDet = array(
					'collection'=>$this->collName,
					'url'=>$this->solrnode,
					'method'=>$method,
					'action'=>$this->action,
					'params'=>escape_string(serialize($data)),
					'created_by'=>$uid,
					'created_on'=>now()
				);
			}else{
				$logDet = array(
					'status'=>$response['responseHeader']['status'],
					'response'=>escape_string(serialize($response)),
					'updated_by'=>$uid,
					'updated_on'=>now()
				);
			}
			return $logDet;
	}
	
	/**
	 * Log the request and response into table
	 * @param $id (INT) id of the request log, used to update the response log 
	 * @param $data (String) Request parameter
	 * @param $method (String) GET/POST
	 * @param $response (Object) Result set
	 * @return (INT) Log id
	 */
	private function log($id, $data, $method, $response=''){
		if($this->isLog == FALSE) return; // Do nothing if isLog is set to false
		
		$detail = $this->preparelogData($data,$method,$response);
		try{
			if(empty($id)){ // Insert log 
				$qry = db_insert('slt_solr_sync_log');
				$qry->fields($detail);
				expDebug::dPrintDBAPI("Solr log insert ",$qry);
				$logId = $qry->execute();
				return $logId;
			}else{ // Update log
				$qry = db_update('slt_solr_sync_log');
				$qry->fields($detail);
				$qry->condition('id',$id,'=');
				expDebug::dPrintDBAPI("Solr log update ",$qry);
				$logId = $qry->execute();
				return $logId;
			}
		}catch(Exception $e){
			expDebug::dPrint("Error in Solr Log ".print_r($e->getMessage(),true),1);
		}
	}
	
	/**
	 * Setting up access privilege filters based on the logged in users privilege
	 * @param $filter (Array)
	 * @return NA
	 */
	protected function setAdminAccessFilters(&$filter,$page=''){
		global $Solr_User;
		if(empty($Solr_User)){
			$results = $this->getLoggedUserDetails();
		}else{
			$results = $Solr_User;
		}
		$grpStr = '';
		if($page == 'user')
			$priv = 'Privilege';
		else 
			$priv = 'AdmGroupName';
		if(!empty($results)){
			$uid = $results[0]->id;
				if($results[0]->admin_groups){
				$assignedLnrGrp = explode(',',$results[0]->admin_groups);
			    if(!in_array("`Data Administer`", $assignedLnrGrp)){
					foreach($assignedLnrGrp as $key=>$val){
						if($key==0)
							$grpStr = ''.$priv.':"*'.trim(str_replace(' ','?',$val)).'*"';
						else
							$grpStr .= ' OR '.$priv.':"*'.trim(str_replace(' ','?',$val)).'*"';
					}
				    $grpStr .= ' OR '.$priv.':"*Open_Entity*"';
				  }
				}
			  else 
				$grpStr = ''.$priv.':"*Open_Entity*"';
			
		}else{
			$uid = getIdOfLoggedInUser();
		}
		
		if(!empty($grpStr))
		$grpStr .= ' OR CreatedBy:'.$uid.' OR UpdatedBy:'.$uid.'';

		
		$filter[] = urlencode($grpStr);
	}
	
/**
	 * Fetch the entire details of the logged in user
	 * @return unknown_type
	 */
	public function getLoggedUserDetails($userId=''){
		global $Solr_User;
		$uid = ($userId == ''|| empty($userId))?getIdOfLoggedInUser() : $userId;
		$apiCall='';
		if($uid==0 && isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
			 $apiCall= $_REQUEST['apiname'];
			 $uid = $_REQUEST['userid'];
			 unset($_REQUEST['apiname']); // To avoid SolrImpl set to true
		}
		
		$resBase = array(
			'id'=>null,
			'lastname'=>null,
			'firstname'=>null,
			'username'=>null,
			'email'=>null,
			'manager_id'=>null,
			'status_code'=>null,
			'addr1'=>null,
			'addr2'=>null,
			'city'=>null,
			'statecode'=>null,
			'countrycode'=>null,
			'preferredlanguagecode'=>null,
			'timezonecode'=>null,
			'register_sms'=>null,
			'orgname'=>null,
			'dottedmanagersid'=>null,
			'object_type'=>'People',
			'learner_groups'=>null,
			'admin_groups'=>null,
			'learner_grp_id'=>null,
			'admin_grp_id'=>null,
			'assigned_groups'=>null,
			'assigned_grp_id'=>null,
			'sumEdit'=>null,
			'sumDelete'=>null,
			'HireDate'=>null,
			'PagePrivilege'=>null
		);
		
		$fieldList = array(
				'id:id',
				'lastname:LastName',
				'firstname:FirstName',
				'username:Username',
				'email:Email',
				'manager_id:ManagerID',
				'status_code:Status',
				'addr1:Address1',
				'addr2:Address2',
				'city:City',
				'statecode:State',
				'countrycode:Country',
				'preferredlanguagecode:PreferredLanguage',
				'timezonecode:TimeZoneCode',
				'register_sms:RegisterSMS',
				'orgname:Organization',
				'dottedmanagersid:DottedManagerID',
				'learner_groups:LearnerGrpName',
				'learner_grp_id:LearnerGrpId',
				'admin_groups:RoleName',
				'admin_grp_id:AdminGrpId',
				'assigned_groups:AssignedGrpName',
				'assigned_grp_id:AssignedGrpId',
				'sumEdit:EditPriv',
				'sumDelete:DeletePriv',
				'HireDate',
				'PagePrivilege'
			);
		
		// Setting required filters
		$fil = array(
			"id:$uid",
			//"Status:lrn_cls_sts_atv"
		);
		
		
		// Append field list
		$data = 'fl=' . implode(',',$fieldList);
		
		// Append filters
		$data .= '&fq=' . implode('&fq=',$fil);
		
		// Append solr additional params 
		$data .= '&sort=CreatedOn+desc&indent=on&q=*:*';
		
		try{
			$prefix = getConfigValue('collection_prefix');
		
			// Set exact collection name (should match with solr's collection)
			$collName = $prefix . '_' . SolrClient::UserCore;
			$srcRst = $this->getData($collName,$data,'GET');
			
			if($this->responseType == 'xml'){
				$xml = simplexml_load_string($srcRst);
				$json = json_encode($xml);
				$array = json_decode($json,TRUE);
				$srcRst1[] = $array['results']['result'];
			}else{
				$srcRst1 = $srcRst['response']['docs'];
			}
			
			$srcRst2 = array();
			foreach($srcRst1 as $doc){
				$tdoc  = array_merge($resBase, $doc);
				$srcRst2[] = $this->arrayToObject($tdoc);
			}
			expDebug::dPrint("After merge -- ".print_r($srcRst2,true),4);
			if(!empty($apiCall)){
				 $_REQUEST['apiname'] = $apiCall;
			}
			// Due to 0084822 the user attribute store has been changed from session to global
			// hence the user call will happen once for each request.
			if($uid != 0)
				$Solr_User = $srcRst2;
			unset($_REQUEST['SolrImpl']);
			return $srcRst2;
		}catch(Exception $e){
			expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
		}
	}
	
	protected function setPrivileges(&$tdoc,$userDetails,$page){
		// Set Edit & Delete Privileges to each result set
		$priv = array();
        $entityAccess = array();
        $userAccess = array();
        $ed_priv = array();
		$perm=array('Edit'=>0,'Delete'=>0,'Validated'=>0);
		$priv = explode('##',$userDetails[0]->PagePrivilege);
		foreach($priv as $check){
			if(strpos($check,$page) !== false){
				$priv = $check;
			}
		}
		expDebug::dPrint("Privi check for each doc --1234 ".print_r($priv,true),4);
			$perm = $this->checkOwnerPriv($tdoc,$perm,$userDetails[0]->id);
			$perm = $this->checkAdminPriv($priv,$perm);
		
        /*  $tdoc['sumedit'] = $perm['Edit'];
         $tdoc['sumdelete'] = $perm['Delete'];  */
        $tdoc['sumedit'] = 0;
        $tdoc['sumdelete'] = 0;

        $userAdminGroups = explode(',',$userDetails[0]->admin_groups);

        //check for entity level access.Fix for #0088819
        if($userDetails[0]->id == 1 || in_array("`Data Administer`", $userAdminGroups) || !isset($tdoc['Privilege']) || $tdoc['Privilege'] == "Open_Entity") //If logged user is drupal admin or belongs to data administer grp or no privilege set for an entity, take access privilege of the logged in user.
        {
		$tdoc['sumedit'] = $perm['Edit'];
		$tdoc['sumdelete'] = $perm['Delete'];
        }
        else
        {
            $entityAccess = explode('##',$tdoc['Privilege']);
            foreach($entityAccess as $entitycheck)
            {
                $ed_priv = explode('|',$entitycheck);
           //     $userAdminGroups = explode(',',$userDetails[0]->admin_groups);
                foreach($userAdminGroups as $userAdminGroup)
                {
                    if(strpos($userAdminGroup,$ed_priv[1],1) !== false)  //got a common group. Offset set to 1 to escape the single quote.
                    {
                        if($tdoc['sumedit'] != 1 && $ed_priv[2] == 'E-1')     
                            $tdoc['sumedit'] = 1;
                        if($tdoc['sumdelete'] != 1 && $ed_priv[3] == 'D-1')
                            $tdoc['sumdelete'] = 1;
                    }
                }
            }
        }
		
		unset($tdoc['Privilege']);
		unset($tdoc['CreatedBy']);
		unset($tdoc['UpdatedBy']);
		expDebug::dPrint("Privi check for each doc -- ".print_r($tdoc,true),4);
	}
	
	private function checkOwnerPriv($tdoc,$perm,$uid){
		if($tdoc['CreatedBy'] == $uid || $tdoc['UpdatedBy'] == $uid){ //Owner check
			$perm=array('Edit'=>1,'Delete'=>1,'Validated'=>1);
		}
		return $perm;
	}
	
	private function checkAdminPriv($priv,$perm){
		$ed_priv = explode('|',$priv);
		// Privilege check
		if($ed_priv [1] == 'E-1')
			$perm['Edit'] = 1;
		if($ed_priv [2] == 'D-1')
			$perm['Delete'] = 1;
		return $perm;
	}
	protected function resetScoreField(&$tdoc){
		$score = $tdoc['score_id'];
		$tdoc['score'] = $score;
		unset($tdoc['score_id']);
	}
	protected function apiResponseHandler($srcRst,$from,$resBase){
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
			if($this->responseType == 'json'){
				$response = $this->apiJsonHandler($resBase,$srcRst['response'],$from); // For API just return the original results
				return $response;
			}else{
				$dom = new DOMDocument();
				$dom->loadXML($srcRst);
				$res = $dom->getElementsByTagName('results')->item(0);
				$this->apiXMLHandler($resBase,$res,$from);
				$srcRst = $dom->saveXML($res);
				expDebug::dPrint("Records form Solr Catalog Search XML Formatted -- ".print_r($srcRst,1),4);
				return $srcRst;
			}
		}
	}
	 
	private function apiJsonHandler($outFields,&$response,$from){
		foreach($response['docs'] as $set=>&$field){
			$field = array_merge($outFields,$field);
			$field = $this->removeTilt($field);
		}
		expDebug::dPrint("Records form Solr Catalog Search JSON Formatted -- ".print_r($response,1),4);
		return $response;
	}
	
	protected function removeTilt($tdoc){
		foreach($tdoc as &$doc){
			$doc = str_replace('`','',$doc);
		}
		return $tdoc;
	}
	
	private function apiXMLHandler($outFields,&$response,$from){
		$resItem = $response->getElementsByTagName('result');
		foreach($resItem as $docs){
			$docs = $this->xmlMerge($outFields,$docs);
		}
	}
	
	private function xmlMerge($outFields,$docs){
		foreach($outFields as $fld=>$val){
			$found = $docs->getElementsByTagName($fld);
			if($found->length == 0){
				$newNode = new DOMElement($fld);
				$docs->appendChild($newNode);
			}else{
				$txt = str_replace('`','',$found->item(0)->nodeValue);
				$docs->getElementsByTagName($fld)->item(0)->nodeValue = $txt;
			}
		}
		return $docs;
	}
	
	private function callOffLineLogProcess($url, $data = array(),$peerVerify=FALSE) {
	  expDebug::dPrint('call callOffLineLogProcess '.$url,4);
		expDebug::dPrint('func args callOffLineLogProcess '.print_r(func_get_args(), 1), 4);
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
				expDebug::dPrint('out sock'.print_r($out, 1),4);
		    fwrite($fp, $out);
		    fclose($fp);
		   	//sleep(2);
		    return TRUE;
		  }
		}catch(Exception $ex) {
			expDebug::dPrint('Error in call callOffLineLogProcess '.$ex->getMessage(), 1);
			expDebug::dPrint(print_r($ex), 4);
		}
	}
}

?>