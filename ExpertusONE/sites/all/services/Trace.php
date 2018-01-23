<?php

//expDebug::debugOff();

/**
 * @author Tejus Pratap / Sunil Girdhar
 * @desc The main expertus debugging class
 */

class mainDebug
{
  private $outval;
  
  /*
   * dArray() - Parses and writes an object or array to the file
   */  
  public function dArray($arr, $traceInfo, $logLevel) {
    try {
    	$this->outval = '';
      $this->parseArray($arr); // Builds the string in $this->outval
      $this->makeOutput($traceInfo, $logLevel);
      $this->writeOutputToFile();
    }
    catch (Exception $e) {
      throw new SoapFault("SPLMS", $e->getMessage());
    }
  }

  /*
   * dPrint() - Writes a string to the file
   */   
  public function dPrint($string, $traceInfo, $logLevel) {
    try {
      $this->outval = $string;
      $this->makeOutput($traceInfo, $logLevel);
      $this->writeOutputToFile();
    }
    catch (Exception $e) {
      throw new SoapFault("SPLMS",$e->getMessage());
    }
  }

  /*
   * makeOutput)() - Prepares the text to be printed.
   */
  private function makeOutput($traceInfo, $logLevel) {
  	global $user;
  	$user_id = isset($user->uid) ? $user->uid : 0;
  	$date = new DateTime('now', new DateTimeZone('UTC'));
  	$stringData = $user_id . ' : ' . $date->format("D M j h:i:s") . ' : ';
    $stringData .= 'L' . $logLevel . ' : ';
    $stringData .= $this->getMsgSourceDetails($traceInfo);
    $stringData .= "\t" . $this->outval . "\r\n\r\n";
    $stringData .= "-----------------------------------------------------\r\n\r\n";
    $this->outval = $stringData;
  }

  /*
   * parseArray() - Parses and object or an array and converts it into a string
   */
  private function parseArray($arr) {
    if (is_array($arr) || is_object($arr)) {
      $this->outval .= "\r\n";
      foreach ($arr as $name => $value) {
        if ((is_array($value) && count($value) > 0) || is_object($value)) {
          $this->outval .= "===========================" . strtolower(trim($name)) . "====>" . "\r\n";
          $this->parseArray($value);
        }
        else {
          $this->outval .= "\t\t\t\t" . strtolower(trim($name)) . "-->" . $value . "\r\n";
        }
      }
    }
    else {
      $this->outval .= "NOT AN OBJECT OR ARRAY";
    }
  }
  
  /*
   * getMsgSourceDetails() - For a function prints file name + function name
   *                       - For a class method prints file name + class name + method name.
   */
  private function getMsgSourceDetails($traceInfo) {
    $msgSourceInfo = '';
    if (!empty($traceInfo)) {
      // If $traceInfo['file'] is present, add $traceInfo['file'] and $traceInfo['line']
      if (!empty($traceInfo['file'])) {
        $msgSourceInfo .= basename($traceInfo['file']) . ' (' . $traceInfo['line'] . ') : ';
      }
        
      // Based on type add $traceInfo['function'] or $traceInfo['class'] -> $traceInfo['function'].
      // For static class methods $traceInfo['class'] :: $traceInfo['function'] will be added
      // In both cases, if the actual function file name (determined using reflection) is different from $traceInfo['file'] name,
      // that file name is added as well before adding $traceInfo['function'] or $traceInfo['class']
      if (empty($traceInfo['type'])) {
        /* is a function call, print function name */
        if (!empty($traceInfo['function'])) {
          try {
            $refFunc = new ReflectionFunction($traceInfo['function']);
          }
          catch (ReflectionException $ex) {
            // Do nothing
          }
          if (!empty($refFunc)) {
            $realFileName = basename($refFunc->getFileName());
            if (empty($traceInfo['file']) || basename($traceInfo['file']) != $realFileName) {
              $msgSourceInfo .= $realFileName . ' : ';
            }
            unset($refFunc);
          }
          else {
            //$msgSourceInfo .= 'Reflection Exception : '; // for debugging
          }
        }
        $msgSourceInfo .= $traceInfo['function'] . '() : ';
      }
      else {
        if (!empty($traceInfo['class'])) {
          try {
            $refClass = new ReflectionClass($traceInfo['class']);
          }
          catch (ReflectionException $ex) {
            // Do nothing
          }
          if (!empty($refClass)) {
            $realFileName = basename($refClass->getFileName());
            if (empty($traceInfo['file']) || basename($traceInfo['file']) != $realFileName) {
              $msgSourceInfo .= $realFileName . ' : ';
            }
            unset($refClass);
          }
          else {
            //$msgSourceInfo .= 'ReflectionException : '; // for debugging
          }
        }    
        if ($traceInfo['type'] == '->') {
          /* is an object method call, print class name and method name */
          $msgSourceInfo .= $traceInfo['class'] . '->' . $traceInfo['function'] . '() : ';
        }
        else if ($traceInfo['type'] == '::') {
          $msgSourceInfo .= $traceInfo['class'] . '::' . $traceInfo['function'] . '() : ';
        }
      }
    }
      
    return $msgSourceInfo;
  }

  /*
   * writeOutputToFile() - Writes the sting in $this->outval to the debug file.
   */
  private function writeOutputToFile() {
    try {
      $dbgFilename = expDebug::getDebugFilename();
      $dbgFilePointer = fopen($dbgFilename, 'a'); // Open the file for appending (or create if not present)
      if ($dbgFilePointer === FALSE) {
        $dbgFilePointer = fopen($dbgFilename, 'w'); // If above fails, try opening for writing after truncating the file
      }
      if ($dbgFilePointer === FALSE) {
      	throw new Exception('Unable to open exp debug log file for writing.');
      }

      fwrite($dbgFilePointer, $this->outval);
      $this->outval = ''; // Reset outval
      fclose($dbgFilePointer); // Close the file
    }
    catch (Exception $e) {
    	if ($dbgFilePointer !== FAlSE) {
        fclose($dbgFilePointer); // Close the file pointer if open
    	}
      throw $e; // Rethrow the exception
    }
  }

}  // end class


/**
 * @author Tejus Pratap / Sunil Girdhar
 * @description implements the expDebug class
 */
class expDebug
{
  static private $isDebug = true;
  static private $debugLevel = 2;
  static private $debugFilename = '';
  
  /**
   * This function outputs a string, or an array (as string) or an object (as string) to the exp debug file
   * @param $variable - a string, an array or an object
   * @param $logLevel - the log level at which the $variable is to be printed (default log level : 3).<br>
   *                    Please follow these guidelines for log levels:<br>
   *                    a. log level 1 is reserved for debug messages from expertusErrorThrow()<br>
   *                    b. For a SQL query and its arguments use log level 2. Use dPrintDBAPI() for printing Drupal DBAPI queries. <br>
   *                    c. For results from a SQL query use log level 3.<br>
   *                    d. Use log level 4 or 5 for other small or large data structures respectively.<br>
   *                    e.g. use log level 5 when printing $form and $form_state to the debug file.
   * @param $traceInfo - used by expertusErrorThrow()
   */
  static function dPrint($variable, $logLevel = 3, $traceInfo = array()) {
  	try {
	  	self::updateDebugSettings(); // Freshly check the exp_sp.ini variables and update $isDebug, $debugLevel, $debugFilename
	  	
	  	if (self::$isDebug == false) {
	  		return;
	  	}
	  	
	  	if (!is_int($logLevel)) {
	  		throw new Exception('Paramter $logLevel is not an integer');
	  	}
	  	
	  	if ($logLevel > self::$debugLevel) {
	    	return;
	    }
	    
	    if (empty($traceInfo)) {
	      $traceInfoList = debug_backtrace(false);
	      if (sizeof($traceInfoList) > 1) {
	        $traceInfo = $traceInfoList[1];
	      }
	    }
	    
	    $debugObj = new mainDebug();
	    if(is_array($variable) || is_object($variable)) {
	      $debugObj->dArray($variable, $traceInfo, $logLevel);
	    }
	    else {
	      $debugObj->dPrint($variable, $traceInfo, $logLevel);
	    }
	    unset($debugObj);
  	}
    catch (SoapFault $sf) {
    	// expDebug::dPrint() should  not be throwing any exception, but should be dealing with all exceptions.
      die(t("ERR155")); // What else can we do, except die?
    }
    catch (Exception $e) {
    	// Cannot output exception trace to the log file if debug is off or debug level is less than 1.
    	// So, throw exception message as a SoapFault.
    	if(self::$isDebug == false || self::$debugLevel < 1) {
    		// expDebug::dPrint() should  not be throwing any exception.
    		die(t("ERR155")); // What else can be do, except die?
    	}
    	
    	// Else try to print the exception trace to the log file
    	try {
	      $expDebugMsg = "Expertus ERROR : ";
	      $expDebugMsg .= $e->getMessage() . "\r\n\r\n";
	      $expDebugMsg .= "Trace info :\r\n";
	      $expDebugMsg .= $e->getTraceAsString() . "\r\n\r\n";
	      
	      $traceInfo = array();
	      $traceInfoList = debug_backtrace(false);
	      if (sizeof($traceInfoList) > 0) {
	        $traceInfo = $traceInfoList[0];
	      }

	      $debugObj = new mainDebug();
	      $debugObj->dPrint($expDebugMsg, $traceInfo, 1);
	      unset($debugObj);
    	}
    	catch (Exception $ex) {
    		// What else can we do, except die?
    	}
      die(t("ERR155"));
    }
  }

/**
 * This function outputs the SQL string of a Drupal database API object to the exp debug file at log level 2 with all placeholder arguments filled with
 * corresponding values; currently, objects returned by db_query(), db_select)(), db_insert(), db_update(), db_delete() and db_truncate() are supported.
 * @param $msgStr - A message to be prefixed to the SQL in the output debug message
 * @param $dbVariable - The Drupal database API object 
 * @param $args - Used only for generating SQL from db_query() object. Arguments placeholder => value map array.
 * @param $traceInfo - Reserved parameter.
 */
  static function dPrintDBAPI($msgStr, $dbVariable = null, $args = array(), $traceInfo = array()) {
  	try {
  	  $logLevel = 2; // All SQL strings and their arguments are output at log level 2
  	  self::updateDebugSettings(); // Freshly check the exp_sp.ini variables and update $isDebug, $debugLevel, $debugFilename
      if(self::$isDebug == false || $logLevel > self::$debugLevel) {
    	  return;
      }
    
      if (empty($traceInfo)) {
        $traceInfoList = debug_backtrace(false);
        if (sizeof($traceInfoList) > 1) {
          $traceInfo = $traceInfoList[1];
        }
      }

      $debugMsg = '';       
      if (is_string($msgStr)) {
        $debugMsg = $msgStr;
      }
      else {
      	throw new Exception('First argument is not a string');
      }
    
      if(is_object($dbVariable)) {
        if ($dbVariable instanceof DatabaseStatementInterface) { // object returned by db_query()
      	  $sqlStr = $dbVariable->getQueryString();
          if (is_array($args) && !empty($args)) {
        	  $sqlStr = self::showQuery($sqlStr, $args);
          }
          $debugMsg .= " SQL = " . $sqlStr;
        
        }
        else if ($dbVariable instanceof SelectQuery || $dbVariable instanceof ExpertusSelectQueryExtender) { // object returned by db_select()
      	  $sqlStr = $dbVariable->__toString();
      	  $sqlStr = self::showTableNamesInQuery($sqlStr);
      	  $args = $dbVariable->getArguments();
      	  $sqlStr = self::showQuery($sqlStr, $args);
      	  $sqlStr = self::showUnionedQuery($sqlStr, $dbVariable);
      	  $debugMsg .= " SQL = " . $sqlStr;
        }
        else if ($dbVariable instanceof InsertQuery) { // object returned by db_insert()
          $sqlStr = $dbVariable->__toString();
          $sqlStr = self::showTableNamesInQuery($sqlStr);
        
          try {
            $refObj = new ReflectionObject($dbVariable);
          }
          catch (ReflectionException $ex) {
            // Do nothing
          }
          if (empty($refObj)) {
        	  $debugMsg .= " SQL = " . $sqlStr;
            $debugMsg .= "\r\n\r\n**** InsertQuery object for referencing fields / values ****\r\n";
            $debugMsg .= print_r($dbVariable, true);
          }
          else {
            try {
              $valuesProperty = $refObj->getProperty('insertValues');
              unset($refObj);  // Not needed anymore
              $valuesProperty->setAccessible(true);
              $values = $valuesProperty->getValue($dbVariable);
              unset($valuesProperty); // Not needed anymore
              $sqlStr = self::showInsertQuery($sqlStr, $values);
            }
            catch (ReflectionException $ex) {
          	// Do nothing
            }
            $debugMsg .= " SQL = " . $sqlStr;
          }
        }
        else if ($dbVariable instanceof UpdateQuery) { // object returned by db_update()
      	  $sqlStr = $dbVariable->__toString();
      	  $sqlStr = self::showTableNamesInQuery($sqlStr);
      	  $args = $dbVariable->arguments();
      	  $sqlStr = self::showQuery($sqlStr, $args);
      	
          try {
            $refObj = new ReflectionObject($dbVariable);
          }
          catch (ReflectionException $ex) {
            // Do nothing
          }
        
          if (empty($refObj)) {
        	  $debugMsg .= " SQL = " . $sqlStr;
      	    $debugMsg .= "\r\n\r\n**** UpdateQuery object for referencing fields / values ****\r\n";
      	    $debugMsg .= print_r($dbVariable, true);
          }
          else {
        	  try {
        	    $fieldsProperty = $refObj->getProperty('fields');
        	    unset($refObj); // Not needed anymore
        	    $fieldsProperty->setAccessible(true);
        	    $fields = $fieldsProperty->getValue($dbVariable);
        	    unset($fieldsProperty); // Not needed anymore
        	    $sqlStr = self::showUpdateQuery($sqlStr, $fields);
        	  }
        	  catch (ReflectionException $ex) {
        		  // Do nothing
        	  }
        	  $debugMsg .= " SQL = " . $sqlStr;
          }
        }
        else if ($dbVariable instanceof TruncateQuery) { // object returned by db_truncate()
      	  $sqlStr = $dbVariable->__toString();
      	  $sqlStr = self::showTableNamesInQuery($sqlStr);
      	  $debugMsg .= " SQL = " . $sqlStr;
        }
        else if ($dbVariable instanceof DeleteQuery) { // object returned by db_delete()
      	  $sqlStr = $dbVariable->__toString();
      	  $sqlStr = self::showTableNamesInQuery($sqlStr);
          $args = $dbVariable->arguments();
          $sqlStr = self::showQuery($sqlStr, $args);
          $debugMsg .= " SQL = " . $sqlStr;
        }
        else {
      	  $debugMsg .= ': UNKNOWN DRUPAL DATABASE API OBJECT TYPE';
        }
      }
      else if(is_string($dbVariable)){
      	// Replacing arguments
      	if($args){
      		foreach($args as $key=>$value){
      			$key = substr($key,1);
      			$pattern = "/:\b$key\b\s/";
      			$dbVariable = preg_replace($pattern," '$value' ",$dbVariable);
      		}
      	}
      	$debugMsg .= " SQL = " . $dbVariable;
      }else{
    	  $debugMsg .= ': UNKNOWN DRUPAL DATABASE API DATA STRUCTURE';
      }
    
      $debugObj = new mainDebug();
      $debugObj->dPrint($debugMsg, $traceInfo, $logLevel);
      unset($debugObj);
  	}
  	catch (SoapFault $sf) {
      // expDebug::dPrintDBAPI() should  not be throwing any exception, but should be dealing with all exceptions.
      die(t("ERR155")); // What else can we do?
    }
  	catch (Exception $e) {
  		// try to print stack trace of any other exception into the log file
  		try {
	      $expDebugMsg = "Expertus ERROR : ";
	      $expDebugMsg .= $e->getMessage() . "\r\n\r\n";
	      $expDebugMsg .= "Trace info :\r\n";
	      $expDebugMsg .= $e->getTraceAsString() . "\r\n\r\n";
	      
	      $traceInfo = array();
	      $traceInfoList = debug_backtrace(false);
	      if (sizeof($traceInfoList) > 0) {
	        $traceInfo = $traceInfoList[0];
	      }
	      
	      self::dPrint($expDebugMsg, 1, $traceInfo);
  		}
  		catch (Exception $ex) {
  			// What else can we do, except die?
  		}
      
      die(t("ERR155"));
  	}
  }

/**
 * Turns debug off
 */
  static function debugOff() {
    self::$isDebug = false;
  }

/**
 * Turns debug on
 */
  static function debugOn() {
    self::$isDebug = true;
  }
  
  /**
   * This function sets the debug level; only messages output at log level equal to or below this debug level shall be printed.
   * This function can be used to set a higher debug level within a function when developing or debugging.
   * @param int $debugLevel - The debug level to set
   * @return int - The previously set debug level
   */
  static function setDebugLevel($debugLevel) {
  	$prevDebugLevel = self::$debugLevel;
    self::$debugLevel = $debugLevel;
    return $prevDebugLevel;
  }

  /**
   * Fetch the debug file name.
   * @return string - debug file name
   */
  static function getDebugFilename() {
    return self::$debugFilename;
  }

  /*
   * updateDebugSettings() - Get the latest debug settings from exp_sp.ini file
   */
  static private function updateDebugSettings() {
    try {
      include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/GlobalUtil.php";
      $gutil = new GlobalUtil();
      $dbug = $gutil->getConfig();
      
      // Messages at and below set debug_level in exp_sp.ini shall only be printed
      $debugLevel = trim($dbug['debug_level']);
      // Save debug level
      if (is_numeric($debugLevel)) {
        self::setDebugLevel($debugLevel);
      }
  
      // To turn on debug messages set debug in exp_sp.ini to 1
      // To turn off debug messages set it to 0.
      if (strtolower(trim($dbug['debug'])) == 1) {
        self::debugOn();
      }
      else {
        self::debugOff();
      }
      
      // Get the debug file name
      self::$debugFilename = $dbug['debug_path'] . '/' . $dbug['debug_file_name'];
      
    }
    catch (Exception $ex) {
      throw new SoapFault("SPLMS", $ex->getMessage());
    }
  }

  /*
   * showTableNamesInQuery() - to remove '{' and '}' from table names in query
   */  
  static private function showTableNamesInQuery($queryStr) {
    $queryStr = preg_replace('/{(.+)}/U', '$1', $queryStr);
    
    return $queryStr;
  }
  
  /*
   * showQuery() - to get a query string with all placeholder arguments substituted with values
   */
  static private function showQuery($queryStr, $params) {
    $keys = array();
    $values = array();

    # build a regular expression for each parameter
    foreach ($params as $key => $value) {
      if (is_string($key)) {
        if (substr($key, 0, 1) == ':') {
          $keys[] = '/' . $key . '/';
        }
        else {
          $keys[] = '/:' . $key . '/';
        }
      }
      else {
        $keys[] = '/[?]/';
      }
            
      if (is_numeric($value)) {
        $values[] = intval($value);
      }
      else {
        $values[] = '"'.$value .'"';
      }
    }
        
    $queryStr = preg_replace($keys, $values, $queryStr, 1, $count);

    return $queryStr;
  }
  
  /*
   * showUpdateQuery() - to get an update query string with all placeholder arguments substituted with values
   */
  static private function showUpdateQuery($queryStr, $params) {
    $keys = array();
    $values = array();

    # build a regular expression for each parameter
    foreach ($params as $key => $value) {
      $keys[] = '/' . $key . '=:db_update_placeholder_[0-9]+/';
            
      if (is_numeric($value)) {
        $values[] = $key . '=' . intval($value);
      }
      else {
        $values[] = $key . '=' . '"' . $value . '"';
      }
    }
    
    $queryStr = preg_replace($keys, $values, $queryStr, 1, $count);

    return $queryStr;
  }
  
  /*
   * showInsertQuery() - to get an insert query string with all placeholder arguments substituted with values
   */
  static private function showInsertQuery($queryStr, $paramValuesList) {
    $keys = array();
    $values = array();
    $placeHolderIdx = 0;

    # build a regular expression for each parameter
    foreach ($paramValuesList as $paramValues) {
      foreach ($paramValues as $value) {
        $keys[] = '/:db_insert_placeholder_' . $placeHolderIdx . '/';
        if (is_numeric($value)) {
          $values[] = intval($value);
        }
        else {
          $values[] = '"' . $value . '"';
        }
        $placeHolderIdx++;
      } 
    }
   
    $queryStr = preg_replace($keys, $values, $queryStr, 1, $count);

    return $queryStr;
  }
  
  /*
   * showUnionedQuery() - Substitutes all placeholder arguments with values for any unioned queries
   */
  static private function showUnionedQuery($queryStr, $dbVariable) {
    try {
      $refObj = new ReflectionObject($dbVariable); // will throw ReflectionException on failure
      $unionProperty = $refObj->getProperty('union'); // will throw ReflectionException if property is not present
      unset($refObj); // No more needed
      $unionProperty->setAccessible(true);
      $unionsList = $unionProperty->getValue($dbVariable);
      unset($unionProperty); // No more needed
    }
    catch (ReflectionException $ex) {
      return $queryStr; // return $queryStr without any substitution
    }
    
    foreach($unionsList as $unionItem) {
      if (!empty($unionItem['query']) && $unionItem['query'] instanceof SelectQuery) {
        $args = $unionItem['query']->getArguments();
        $queryStr = self::showQuery($queryStr, $args);
      }
    }
    return $queryStr;
  }

} // end class


/**
 * @author Expertus Inc.
 * @description implements the expertusCustomException class
 */
class expertusCustomException extends Exception {  
  public function __construct($message, $code = 0, Exception $previous = null) {
    // TBD
  }
}

/*
 * expertusErrorThrow() - Tries to print an exception trace to the log file before it halts execution with a standard message to the client
 */
function expertusErrorThrow($exception) {
	try {
	  $expDebugMsg = "Expertus ERROR : ";
	  if (is_object($exception) && $exception instanceof Exception) {
	    $expDebugMsg .= $exception->getMessage() . "\r\n\r\n";
	    $expDebugMsg .= "Trace info :\r\n";
	    $expDebugMsg .= $exception->getTraceAsString() . "\r\n\r\n";
	  }
	  
	  $traceInfo = array();
	  $traceInfoList = debug_backtrace(false);
	  if (sizeof($traceInfoList) > 1) {
	    $traceInfo = $traceInfoList[1];
	  }
	  expDebug::dPrint($expDebugMsg, 1, $traceInfo);
	}
	catch (Exception $e) {
		// What else can we do, but die?
	}
	//to return API error response if the exception should be catched for API.
	$functionCall = end($traceInfoList);
	if($functionCall['function'] == 'doExecute' && $functionCall['class'] == 'ExpertusOneAPI') {
		include_once '../apis/core/BaseAPI.php';
		$baseAPI = new BaseAPI();
		$errors = array();
		if($baseAPI->getErrorResponse() === 1) {
		$exception = new stdClass();
		$exception->isValidateError = true;
		$exception->errormsg = t("ERR155");
		$errors[] = $exception;
		}
		else {
		$exception = new stdClass();
		$exception->errormsg = t("ERR155");
		}
		if(($baseAPI->getErrorResponse() == 1)){
			$formattedErr=ErrorMessages::getErrorMessageNew($errors);
			$outxml = $baseAPI->formatErrorObject($formattedErr, getURLParam('returntype'));
		}
		else {
			$formattedErr=ErrorMessages::getErrorMessage($exception);
			if(getURLParam('returntype') == "xml") {
				$outxml = "<?xml version='1.0' encoding='UTF-8' ?><error><error_code>".$formattedErr->code."</error_code><short_msg>".$formattedErr->shortmessage."</short_msg><long_msg>".$formattedErr->longmessage."</long_msg><corrective_solution></corrective_solution></error>";
			}
			else {
				$outxml = "{\"results\":{\"jsonResponse\":[{\"iserror\":\"true\",\"errorcode\":\"$formattedErr->code\",\"shortmessage\":\"$formattedErr->shortmessage\",\"longmessage\":\"$formattedErr->longmessage\"}]}}";
			}
			
		}
		$baseAPI->sendResponse($outxml);
		exit();
	}
	if (isset($_GET['cron_key'])) {
		throw new Exception($ex);
	}else{
		die(t("ERR155"));
	}
}

/*
 * expertusLogError() - Tries to print an exception trace to the log file (same as expertusErrorThrow() except does not die)
 *                      Cannot use/call in expertusErrorThrow() as can be called directly and $traceInfo needs to be correctly
 *                      captured.
 */
function expertusLogError($exception) {
  try {
    $expDebugMsg = "Expertus ERROR : ";
    if (is_object($exception) && $exception instanceof Exception) {
      $expDebugMsg .= $exception->getMessage() . "\r\n\r\n";
      $expDebugMsg .= "Trace info :\r\n";
      $expDebugMsg .= $exception->getTraceAsString() . "\r\n\r\n";
    }
    
    $traceInfo = array();
    $traceInfoList = debug_backtrace(false);
    if (sizeof($traceInfoList) > 1) {
      $traceInfo = $traceInfoList[1];
    }
    expDebug::dPrint($expDebugMsg, 1, $traceInfo);
  }
  catch (Exception $e) {
    // What else can we do?
  }
}

?>