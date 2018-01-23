<?php 

// Include E1 OAuth2 Server 
require_once $_SERVER["DOCUMENT_ROOT"].'/apis/oauth2/Server.php';

class Client extends OAuth2Server{
	/**
	 * Variable to store return format
	 * @var unknown_type
	 */
	private $return_format;
	
	/**
	 * Constructor, calls parent constructor to create server and storage objcets
	 * @return none
	 */
	function __construct($rf){
		$this->return_format=$rf;
		parent::__construct();
	}
	
	/**
	 * Create client
	 * @param $client_name String
	 *  - name of the client this should be unique
	 * @param $scope String
	 *  - Scope of the client possible values API, APP and DL
	 * @param $redirect_uri
	 *  - URL for Authentication code to redirect
	 * @param $grant_type
	 *  - Grant tupe , right now E1 support only client_credentials
	 * @return object 
	 *  - return the client_id, client_secret and other details
	 */
	public function createClient($client_name, $scope, $redirect_uri,$grant_type=''){
		try{
			// Replace if any space in name with underscore character
			$cname = str_replace(' ','_',$client_name);
			
			// Generate client unique id 
			//$client_id = md5($cname."_".$scope); // Commented Due To same Name Diff Scope Creation.
			$client_name_server_name = $cname.'_.'.$_SERVER['SERVER_NAME'];           
      $client_id = md5($client_name_server_name);
           
      // Generate client secret id
      $client_secret = md5($client_name_server_name."_keypass_".$scope);
			
			// Set grant type - currently E1 supports only client_credentials
			$grant_types = 'client_credentials';
			
			// Get Store details
			$storage = $this->getStore();
			
			// Validate if client exists
			if($storage->getClientDetails($client_id))
				throw new Exception("Client already exists");
			
			// Set actual scope
			switch($scope){
				case "API":
					$scope = "E1.API";
					break;
				case "APP":
					$scope = "E1.APP E1.API";
					break;
				case "DL":
					$scope = "E1.DL";
					break;
				case "ALL":
					$scope = "E1.API E1.APP E1.DL";
					break;
				default:
					$scope = null;
			}
			
			// Create clinet
			$storage->setClientDetails($client_name, $client_id, $client_secret, $redirect_uri, $grant_types, $scope, 1);
			
			// Get created client values
			$restult = $this->getDetailsById($client_id);
			
			// return details as per the format specified
			return $restult;
		}catch(Exception $e){
			return $this->setResponse(array('Error'=>$e->getMessage()));
		}
	}
	
/**
	 * Update client
	 * @param $client_name String
	 *  - name of the client this should be unique
	 * @param $scope String
	 *  - Scope of the client possible values API, APP and DL
	 * @param $redirect_uri
	 *  - URL for Authentication code to redirect
	 * @param $grant_type
	 *  - Grant tupe , right now E1 support only client_credentials
	 * @return object 
	 *  - return the client_id, client_secret and other details
	 */
	public function updateClient($client_name, $scope, $redirect_uri,$grant_type=''){
		try{
			// Replace if any space in name with underscore character
			$cname = str_replace(' ','_',$client_name);
			
			// Generate client unique id 
			//$client_id = md5($cname."_".$scope); // Commented Due To same Client Name with Diff Scope updation.
			$client_name_server_name = $cname.'_.'.$_SERVER['SERVER_NAME'];
			$client_id = md5($client_name_server_name);

			// To generate new uniqe ids adding random number with name
			$client_name_server_name .= rand(0,10000);

			// Generate client secret id
			$client_secret = md5($client_name_server_name."_keypass_".$scope);
			
			// Set grant type - currently E1 supports only client_credentials
			$grant_types = 'client_credentials';
			
			// Get Store details
			$storage = $this->getStore();
			
			// Validate if client exists
			if(!$storage->getClientDetails($client_id))
				throw new Exception("Client not exists");
			
			// Set actual scope
			switch($scope){
				case "API":
					$scope = "E1.API";
					break;
				case "APP":
					$scope = "E1.APP E1.API";
					break;
				case "DL":
					$scope = "E1.DL";
					break;
				case "ALL":
					$scope = "E1.API E1.APP E1.DL";
					break;
				default:
					$scope = null;
			}
			
			// Create clinet
			$storage->setClientDetails($client_name, $client_id, $client_secret, $redirect_uri, $grant_types, $scope, 1);
			
			// Get created client values
			$restult = $this->getDetailsById($client_id);
			
			// return details as per the format specified
			return $restult;
		}catch(Exception $e){
			return $this->setResponse(array('Error'=>$e->getMessage()));
		}
	}
	
	/**
	 * Return client details as per the
	 * format requested by generated clinet id
	 * @param $client_id String
	 * @return mixed object
	 */
	public function getDetailsById($client_id){
		$storage = $this->getStore();
		$rst = $storage->getClientDetails($client_id);
		if(empty($rst))
			$rst = array('Error'=>'Invalid Client Id');
		return $this->setResponse($this->splitArray($rst));
	}
	
	/**
	 * Return client details as per the
	 * format requested by client name
	 * @param $client_id String
	 * @return mixed object
	 */
	public function getDetailsByName($client_name){
		$storage = $this->getStore();
		$rst = $storage->getClientDetailsByName($client_name);
		if(empty($rst))
			$rst = array('Error'=>'Invalid Client Name');
		return $this->setResponse($this->splitArray($rst));
	}
	
	/**
	* Return client Details as per the
	* format requested by client Scope
	* @param $client_id String
	* @return mixed object
	*/
	public function getDetailsByScope($scope,$redirect_uri){
		$storage = $this->getStore();
		// Set actual scope
		switch($scope){
			case "API":
				$scope = "E1.API";
				break;
			case "APP":
				$scope = "E1.APP E1.API";
				break;
			case "DL":
				$scope = "E1.DL";
				break;
			case "ALL":
				$scope = "E1.API E1.APP E1.DL";
				break;
			default:
				$scope = null;
		}
		$rst = $storage->getClientDetailsByScope($scope,'%'.$redirect_uri.'%');
		if(empty($rst))
			$rst = array('Error'=>'Invalid Client Scope Or Uri');
		return $this->setResponse($this->splitArray($rst));
	}
	
	/**
	 * Format output based on the user specified format
	 * @param $detail Array objcet
	 * @return mixed object
	 */
	private function setResponse($detail = array()){
		$out;
		$type = !empty($this->return_format) ? $this->return_format : 'json';
		switch($type){
			case "json":
				$out = json_encode($detail);
				break;
			case "xml":
				$xml = new SimpleXMLElement('<client/>');
				foreach($detail as $key => $value){
		       $xml->addChild($key, $value);
		    }
				$out = $xml->asXML();
				break;
			default:
				$out = $detail;
		}
		expDebug::dPrint($out);
		return $out;
	}
	
	/**
	 * Extract only the associative keys from given array
	 * @param $arr Array objcet
	 * @return Array objcet
	 */
	private function splitArray($arr){
		$ren_arr = array();
		foreach($arr as $k=>$v){
			if(strlen($k)>3)
				$ren_arr[$k] = $v;
		}
		return $ren_arr;
	}
}
?>