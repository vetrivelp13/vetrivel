<?php 

// Include E1 OAuth2 Server 
require_once $_SERVER["DOCUMENT_ROOT"].'/apis/oauth2/Server.php';

/**
 * Class for generate/validate access token
 * @author vincents
 *
 */
class TokenGenerate extends OAuth2Server{
	/**
	 * Constructor, calls parent constructor to create server and storage objcets
	 * @return none
	 */
	function __construct(){
		parent::__construct();
	}
	
	/**
	 * Mehtod to get token for grant type client credential
	 * this will returns access token,expire time, scope and type of the token (default: Bearer)
	 * this is a default method of E1
	 * 
	 * @return Json 
	 */
	public function getToken(){
		try{
			// Add Grant Type
			$this->addGrantType('ClientCredentials');
			
			// Get server object
			$server = $this->getServer();
	
			// Handle a request for an OAuth2.0 Access Token and send the response to the client
			$server->handleTokenRequest(OAuth2\Request::createFromGlobals())->send();
		}catch(Exception $e){
			expDebug::dPrint("Error in access token generation ".$e->getMessage(),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * Other types of token generator
	 * Right now this is not handled in E1, this is for future use 
	 * this will returns access token,expire time, scope and type of the token (default: Bearer)
	 * 
	 * @param $grantType String
	 * grant type what needs to be added to the server object
	 * 
	 * @return Json
	 */
	public function getCustomToken($grantType){
		try{
			// Add Grant Type
			$this->addGrantType($grantType);
			
			// Get server object
			$server = $this->getServer();
			
			// Handle a request for an OAuth2.0 Access Token and send the response to the client
			$server->handleTokenRequest(OAuth2\Request::createFromGlobals())->send();
		}catch(Exception $e){
			expDebug::dPrint("Error in access token generation ".$e->getMessage(),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * Method to verify the existing token whether it is expired or not
	 * @param $action String
	 * if the action is not create_on_expire
	 *  - if the token is valid send the same token 
	 *  - otherwise send empty token with status as invalid
	 * 
	 * if the action is create_on_expire 
	 *  - if the token is valid send the same token
	 *  - otherwise create new token
	 *  
	 * @param $grantType String
	 * Grant type which you want to use for token generation
	 * 
	 * @return Token Json
	 */
	public function checkToken($action,$grantType){
		try{
			// Add Grant Type
			$this->addGrantType($grantType);
			
			// Get server object
			$server = $this->getServer();
			
			// Verify token this will return token if it is valid other wise return error
			$res = $server->getAccessTokenData(OAuth2\Request::createFromGlobals());
			
			if($res){
				// If it is a valid token return the same token
				return '{"access_token":"'.$res['access_token'].'","token_type":"Bearer","scope":"'.$res['scope'].'","status":"Valid"}';
			}else{
				if($action == "create_on_expire"){
					// In case of invalid token create a new token
					$server->handleTokenRequest(OAuth2\Request::createFromGlobals())->send();
				}else{
					// Return invalid status
					return '{"access_token":"","token_type":"","scope":"","status":"Invalid"}';
				}
			}
		}catch(Exception $e){
			expDebug::dPrint("Error in access token validation ".$e->getMessage(),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * Adding grant type to the server objcet
	 * @param $grantType String
	 * @return none
	 */
	private function addGrantType($grantType){
		
		if($grantType=='ClientCredentials'){
			$this->addClientCredentials();
		}else if($grantType=='AuthorizationCode'){
			$this->addAuthorizationCode();
		}else if($grantType=='RefreshToken'){
			$this->addRefreshToken();
		}else{
			throw Exception("Invalid grant type");
		}
		
	}
	
}


$action = isset($_POST['action'])?strtolower($_POST['action']):'';
// Create instance
$token = new TokenGenerate();

if(!empty($action)){
	// Verify token is valid (supports only client credential)
	// TODO: should be get from post param and make it dynamic
	echo $token->checkToken($action,'ClientCredentials');
}else{
	// Generate new token for client credential grant type
	echo $token->getToken();
}

?>