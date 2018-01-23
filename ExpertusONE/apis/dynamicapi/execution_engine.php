<?php
/*
 * ExpertusOne API
 * Execution Engine.php v1.0
 * 
 * @author: Rajkumar U
 * @date	:	02-Jan-2012
 * 
 * This is the execution layer of the Dynamic REST API.
 * 
 * DONOT MODIFY THIS FILE UNTIL IS ABSOLUTELY NECESSARY
 */

class ExecutionEngine{
	
	function ExecuteAPI($APIMethod,$APIPath,$param){
    expDebug::dPrint('API execution_engine.php : ExecuteAPI() : $APIMethod = ' . print_r($APIMethod, true));
		expDebug::dPrint('API execution_engine.php : ExecuteAPI() : $APIPath = ' . print_r($APIPath, true));
    expDebug::dPrint('API execution_engine.php : ExecuteAPI() : $param = ' . print_r($param, true));

		include_once($APIPath);
		$newParam = array();
		
		foreach($param as $key=> $val){
			$$key = $val;
			$newParam[$key] = &${$key};
		}
		$results = call_user_func_array($APIMethod, $newParam);
		return $results;
	}
}
?>