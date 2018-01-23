<?php 

define('DRUPAL_ROOT', $_SERVER['DOCUMENT_ROOT']);

include_once DRUPAL_ROOT . '/sites/all/modules/core/exp_sp_core/modules/simpletest/drupal_web_test_case.php';
include_once DRUPAL_ROOT . '/sites/all/modules/core/exp_sp_core/modules/simpletest/expertus_web_test_case.php';
//include_once DRUPAL_ROOT . '/apis/tests/apiTestCases.php';

include_once DRUPAL_ROOT."/includes/errors.inc";
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_DATABASE);
		
/**
 * Class to get the test cases
 * 
 * @param $apiname (String)
 *     If API name is given then only that respective API's test case will be return
 * @param $tests
 * 		Each test have 2 test types one for datasource and the other for validation
 *    whatever the value (datasource/validation) passed on this pararm the test
 *    which associated it will be return
 * @author Vincent
 *
 */
class APITestCase extends ExpertusAPITestCase{

	/**
	 * Merge both types (datasource/validation) of test cases and 
	 * return as single test array
	 * @param $apiname
	 * 		If API name passed all the test types of this API will be merged. 
	 *    In case the name is not given then all the APIs test cases will be 
	 *    merged API name wise and return
	 * @param $tests
	 * 		If the test type is given only the test cases written fot this type will be return.
	 *    
	 * @return test cases as Array
	 */
	public static function mergeTestCases($apiname = '',$tests = ''){
		try{
			// Include test case file if apiname is not empty
			if(!empty($apiname))
				include_once DRUPAL_ROOT . "/apis/tests/$apiname.php";
				
			$tcase = array();
			
			// If test type is given then collect only that test cases
			if(!empty($tests)){
				$tcase[$apiname] = $testCases[$apiname][$tests];
				return $tcase;
			}
			
			// If Api name only given the merge both type of test cases
			if(!empty($apiname)){
				foreach($testCases[$apiname] as $test){
					if(empty($tcase[$apiname])){
						$tcase[$apiname] = $test;
					}else{
						$tcase[$apiname] = array_merge($tcase[$apiname],$test);
					}
				}
				return $tcase;
			}
			
			// If Api name not given collect all the apis and its test cases
			foreach(self::getApis() as $tapis=>$value){
				return self::mergeTestCases($tapis);
			}
		}catch(Exception $e){
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * Format the API attribites. In API config file the attributes are
	 * defined in different format which to support documentaion but before
	 * we use the attributes need to format as common array structure.
	 * This functio will do that.
	 * @param $ini_arr
	 * 	API atrribute to format
	 * @return Formatted attribute as Array
	 */
	public static function formatAPIParam(&$ini_arr) {
		foreach ($ini_arr AS $key => &$value) {
			if (is_array($value)) {
				self::formatAPIParam($value);
			}
      if (strpos($key, '.') !== FALSE) {
      	$key_arr = explode('.', $key);
        $last_key = array_pop($key_arr);
        $cur_elem = &$ini_arr;
        foreach ($key_arr AS $key_step) {
        	if (!isset($cur_elem[$key_step])) {
        		$cur_elem[$key_step] = array();
        	}
        	$cur_elem = &$cur_elem[$key_step];
        }
        $cur_elem[$last_key] = $value;
        unset($ini_arr[$key]);
      }
    }
  }
  
  /**
   * Bind the API's request parametet and the respected value
   * of the parameter which is given in the test case
   * @param $params Array
   * 		List of request parameters
   * @param $value Array
   * 		Test case value of the requst parameters
   * @return Binded Array
   */
  public static function bindAPIParams(&$params,$value){
  	foreach($params as $param=>$val){
  		if (is_array($val)) {
  			self::bindDeepParams($val, $value, $params);
  			unset($params[$param]);
  		} else {
  			$params[$param] = isset($value[$param]) ? $value[$param] : null;
  		}
  	}
  }
  

  
  public static function bindDeepParams(&$inputs, $value, &$paramArray) {
  	foreach($inputs as $key=>$val){
  		if (is_array($val)) {
  			self::bindDeepParams($val, $value, $paramArray);
  		} else {
  			$paramArray[$key] = $value[$key];
  		}
  	}
  }
  /**
   * Construct the base URL from the SERVER variable,
   * 
   * @return URL String
   */
  public static function constructURL(){
  	global $base_url;
  	return $base_url;
  	/*$schema = (strtoupper($_SERVER['HTTPS']) == 'ON') ? 'https' : 'http';
  	$host = $_SERVER['HTTP_HOST'];
  	$baseURL = $schema . '://' . $host;
  	return $baseURL;*/
  }
}

/**
 * ExpertusONE API test calss
 * It extends the ExpertusAPITestCase
 * @author Vincent
 *
 */

class ExpertusAPITest extends ExpertusAPITestCase {
	
	/**
	 * Stores the list of API names as array which needs to be tested
	 * @var Array
	 */
	private $apiname;
	
	/**
	 * Stores the test cases type which needs to be tested.
	 * @var String
	 */
	private $type;
	
	/**
	 * Constructor
	 * @param $apiname Array
	 * 		List of API names which passed in query 
	 * @param $type
	 * 		Test type passed in query
	 * @return unknown_type
	 */
	function __construct($apiname,$type){
		$this->apiname = $apiname;
		$this->type = $type;
		self::$timestamp = time();
		parent::__constructor();
	}
	
	/**
	 * Main finction to call for testing
	 * @return NA
	 */
	public function testAPIs(){
		try{
			$apiToTest = '';
			$apitestcases = '';
			$requestParams = '';
			if(count($this->apiname)>0){
				foreach($this->apiname as $api){
					$apiToTest[$api] = $this->getApis($api);
				}
			}else{
				$apiToTest = $this->getApis();
			}
			
			// Get Base URL;
			$url = APITestCase::constructURL();
			
				// Get Access token
			$token = $this->getToken($url);
			//Extend Access token validity to one day
			self::extendTokenValidity($token);
			
			foreach($apiToTest as $api=>$param){
				// Format separate request and response parameters
				APITestCase::formatAPIParam($apiToTest[$api]);
				$apiParam['apiname'] = $api;
				$apiParam['params'] = $apiToTest[$api]['params'];
				$apiParam['response'] = $apiToTest[$api]['response'];
				
				// Get Test cases
				$apitestcases = $this->getTestCase($api);
				
				//Process each test
				foreach($apitestcases[$api] as $test){
					expDebug::dPrint("VINCENT TEST CASE -- ".print_r($apitestcases[$api],true),1);
					// Bind request param with test value
					APITestCase::bindAPIParams($apiParam['params'],$test['input']);
					expDebug::dPrint("VINCENT TEST CASE -- ".print_r($apiToTest[$api],true),1);
					// Add addtional required details in request param
					$apiParam['params']["display_cols"]=implode(',',$test['output']);
					$apiParam['params']["returntype"]=$test['output_type']; 
					$apiParam['params']["apiname"] = $api;
					// Assign access token into api parameters
					$apiParam['params']['access_token'] = $token;
					
					// Empty response param
					APITestCase::bindAPIParams($apiParam['response'],array());
					
					expDebug::dPrint("Run Test for API ". $api ." with test params " . print_r($apiParam,true),4);
					expDebug::dPrint("Run Test for API ". $api ." with test case " . print_r($test,true),4);
					
					//Run Test
					$this->runTest($url,$apiParam,$test);
				}
			}
			
			// Show results after completion of all tests
			self::showResults();
			
			//Delete test results after display
			$this->deleteTest();
			
		}catch(Exception $e){
			expDebug::dPrint("Error ON testAPIs function -- ".print_r($e->getMessage(),true),1);
		}
	}
	
	/**
	 * Return the list of test casees which needs to be tested
	 * @param $api String
	 * 		Name of the API which need to be tested
	 * @return Test case Array
	 */
	private function getTestCase($api){
		try{
			if(!empty($this->type)){
				return APITestCase::mergeTestCases($api,$this->type);
			}
			return APITestCase::mergeTestCases($api);
		}catch(Exception $e){
			throw new Exception($e->getMessage());
		}
	}
}

$apiname = array();
$name = isset($_GET['apiname']) ?  $_GET['apiname'] : (isset($_POST['apiname']) ? $_POST['apiname'] : '');
$tests = isset($_GET['type']) ?  $_GET['type'] : (isset($_POST['type']) ? $_POST['type'] : '');
if(stripos($name,',')!==false){
	$apiname = explode(',',$name);
}else{
	$apiname = !empty($name) ? array($name) : array();
}
$obj = new ExpertusAPITest($apiname,strtolower($tests));
$obj->testAPIs();
?>