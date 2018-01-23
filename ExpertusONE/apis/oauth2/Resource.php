<?php 

// Include E1 OAuth2 Server
require_once $_SERVER["DOCUMENT_ROOT"].'/apis/oauth2/Server.php';

/**
 * Class for authenticate and provide access based on the 
 * access token provided in the request 
 * @author vincents
 *
 */
class ResourceValidate extends OAuth2Server{
	/**
	 * Constructor, calls parent constructor to create server and storage objcets
	 * @return none
	 */
	function __construct(){
		parent::__construct();
	}
	
	/**
	 * Validate and verify access token and all access if the token is valid
	 * other wise throw an exception. This is for verify Client Credential
	 * grant type and this is the default method for E1
	 * @param $scope String
	 * Scope string for resourse access
	 * 
	 * @return Object
	 */
	public function validate($scope){
		try{
			// Add client credential grant type 
			$this->addClientCredentials();
			
			// Call validate extend
			return $this->validateExtend($scope);
		}catch(Exception $e){
			expDebug::dPrint("Error in access token validation ".$e->getMessage(),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * Custom validator method to support other grant types
	 * This is for future use
	 * 
	 * @param $grantType String
	 * Grant type which needs to be added to the server object
	 * 
	 * @param $scope String
	 * Scope string for resourse access
	 * 
	 * @return Object
	 */
	public function validateCustom($grantType,$scope){
		try{
			// Add Grant Type
			if($grantType=='ClientCredentials'){
				$this->addClientCredentials();
			}else if($grantType=='AuthorizationCode'){
				$this->addAuthorizationCode();
			}else if($grantType=='RefreshToken'){
				$this->addRefreshToken();
			}else{
				throw Exception("Invalid grant type");
			}
			
			// Call validate extend
			return $this->validateExtend($scope);
		}catch(Exception $e){
			expDebug::dPrint("Error in access token validation ".$e->getMessage(),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * Method to validate access token
	 * @return Object
	 */
	private function validateExtend($scope){
		try{
			// Get server object
			$server = $this->getServer();

			// Verify access token
 			if (!$server->verifyResourceRequest(OAuth2\Request::createFromGlobals(),null,$scope)) {
					$signed = false;
					throw new Exception("Invalid token or scope");
			}else{
					$signed = true;
			}
			// TODO respective user id should be replaced
			$userid_from_access = (isset($_REQUEST["userid"])) ? $_REQUEST["userid"] : '';
			
			// Prepare simple object for return data
			$retObj = new stdClass();
			$retObj->userid_from_access = $userid_from_access;
			$retObj->oauth_signed = $signed;
			
			return $retObj;
		}catch(Exception $e){
			throw new Exception($e->getMessage());
		}
	}
	
}

?>