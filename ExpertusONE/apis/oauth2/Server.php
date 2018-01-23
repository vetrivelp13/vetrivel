<?php 

include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/Trace.php";
/**
 * OAuth2 server class for E1 implementation
 * @author vincents
 *
 */
abstract class OAuth2Server{
	/**
	 * Stores server object
	 * @var $server Object
	 */
	private $server;
	
	/**
	 * Keeps the Storage
	 * @var $storage Object 
	 */
	private $storage;
	
	/**
	 * Constructor - which creates server and storage objects
	 * @return none
	 */
	function __construct(){
		include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/GlobalUtil.php";
		
		// Collect database credentials
		$util=new GlobalUtil();
		$conf=$util->getConfig();
		$db_url=$conf["db_url"];
		$info=parse_url($db_url);
		
		$host = $info['host'];
		/*47123: oauth key generation issue(When port number changed).*/
                
		/*if(isset($info["port"])){
    $host.=":".$info["port"];
    }*/
                
    $dsn      = 'mysql:dbname='.basename($info['path']).';host='.$host;
    if(isset($info["port"])){
    	$dsn.=";port=".$info["port"];
     }
                
     /*end oauth key generation issue(When port number changed).*/
		$username = $info['user'];
		$password = $info['pass'];
		
		// error reporting 
		ini_set('display_errors',1);error_reporting(E_ALL);
		
		// Autoloading (composer is preferred) TODO: Change it to composer
		require_once($_SERVER["DOCUMENT_ROOT"].'/sites/all/libraries/oauth2-server-php/src/OAuth2/Autoloader.php');
		OAuth2\Autoloader::register();
		
		//Database configuration - overriding default tables with E1
		$dbconfig  = array(
					            'client_table' => 'slt_oauth_clients',
					            'access_token_table' => 'slt_oauth_access_tokens',
					            'refresh_token_table' => 'slt_oauth_refresh_tokens',
					            'code_table' => 'slt_oauth_authorization_codes',
					            'user_table' => 'slt_oauth_users',
					            'jwt_table'  => 'slt_oauth_jwt',
					            'scope_table'  => 'slt_oauth_scopes',
					            'public_key_table'  => 'slt_oauth_public_keys',
					        );
		
		// $dsn is the Data Source Name for your database
		$this->storage = new OAuth2\Storage\Pdo(array('dsn' => $dsn, 'username' => $username, 'password' => $password),$dbconfig);
		
		// Get scope object
		$scope = new OAuth2\Scope($this->storage);

		// Access token life time 3600 is default the same is overriding from ini file 
		// here because it is easy to override in feture based on the clinet needs 
		$conf = array('access_lifetime'=>$conf['access_token_lifttime']);
		
		// Pass a storage object or array of storage objects to the OAuth2 server class
		$this->server = new OAuth2\Server($this->storage,$conf,array(),array(),null,$scope);

	}
	
	/**
	 * Add the "Client Credentials" grant type into server objcet (it is the simplest of the grant types)
	 * @return none
	 */
	protected function addClientCredentials(){
		$this->server->addGrantType(new OAuth2\GrantType\ClientCredentials($this->storage));
	}
	
	/**
	 * Add the "Authorization Code" grant type into server objcet 
	 * @return none
	 */
	protected function addAuthorizationCode(){
		$this->server->addGrantType(new OAuth2\GrantType\AuthorizationCode($this->storage));
	}
	
	/**
	 * Add the "Authorization Code" grant type for refresh token into server object
	 * @return none
	 */
	protected function addRefreshToken(){
		$this->server->addGrantType(new OAuth2\GrantType\RefreshToken($this->storage));
	}
	
	/**
	 * Return server objcet
	 * @return Object
	 */
	protected function getServer(){
		return $this->server;
	}
	
	/**
	 * Return storage object
	 * @return Object
	 */
	protected function getStore(){
		return $this->storage;
	}
}

?>