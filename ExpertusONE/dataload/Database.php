<?php
/**
* @class or PHP Name		: Database
* @author(s)				    : Vincent.S
* Version         			: 1.0
* Date						      : 19/11/2014
* PHP Version          	: 5.5.14
* Description     			: DAO Class to do the actual operation with table for Data Load.
*/

//include "../services/GlobalUtil.php";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/GlobalUtil.php";

class DLDatabase{
	private $link;
	private $dsn;
	private $host;
	private $username;
	private $password;
	private $db;
	private $port;
	private $query;
	private $result;
	private $statement;
	private $max_execution_time;
	public $update_queue = false;
	function __construct($operation ='', $from = '') {
		try{
			$util=new GlobalUtil();
			$conf=$util->getConfig();
			/*if($operation=='DL'){
				if($from!='execute')
					$db_url=empty($conf["FED_db_url"]) ? $conf["db_url"] : $conf["FED_db_url"];
				else
					$db_url=empty($conf["FEDX_db_url"]) ? $conf["report_db_url"] : $conf["FEDX_db_url"];
			}else{*/
				if($from!='Report')
					$db_url=$conf["db_url"];
				else
					$db_url=$conf["report_db_url"];
			//}
			$info=parse_url($db_url);
			expDebug::dPrint("Database URL  :".$db_url ." \n Parsed URL Info ".print_r($info,true));

			$this->dsn=$info["scheme"];
			$this->username=$info["user"];
			$this->password=$info["pass"];
			$this->host=$info["host"];
			if(isset($info["port"]))
				$this->port=$info["port"];
			$this->db=basename($info['path']);
			$this->max_execution_time = $conf["dataload_execution_time"];
		}catch(PDOException $e){
			expDebug::dPrint("Error in PDO :".$e->getMessage(),1);
			throw new PDOException($e->getMessage());
		}
	}

	function __destruct()
	{
		try
		{
			if($this->link){
				$this->link=null;
				expDebug::dPrint("Cursor Close destruct is called",5);
			}
		}
		catch(PDOException $e)
		{
			expDebug::dPrint("Error in PDO :".$e->getMessage(),1);
			throw new PDOException($e->getMessage());
		}
	}

	function closeconnect()
	{
		try
		{
			$this->link=null;
			expDebug::dPrint("Cursor Close is called",5);
		}
		catch(PDOException $e)
		{
			expDebug::dPrint("Error in PDO :".$e->getMessage(),1);
			throw new PDOException($e->getMessage());
		}
	}

	 /**
	  * @desc Connects to a database.
	  */
	function connect() {
		try {
			$this->link=new PDO($this->dsn.":host=$this->host;port=$this->port;dbname=$this->db", $this->username, $this->password, array(PDO::ATTR_PERSISTENT => false,PDO::MYSQL_ATTR_INIT_COMMAND => "SET NAMES utf8",PDO::MYSQL_ATTR_LOCAL_INFILE => true));
			//$this->link->setAttribute(PDO::ATTR_TIMEOUT, $this->max_execution_time);
			$this->link->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
			$this->link->setAttribute(PDO::MYSQL_ATTR_USE_BUFFERED_QUERY, true);
			return $this->link;
		} catch (PDOException $e) {
        expDebug::dPrint("Error in PDO :".$e->getMessage(),1);
        throw new PDOException($e->getMessage());
    }
	}

	/**
	 * @desc Begin Transaction
	 * @return
	 */
	public function beginTrans(){
		$this->link->beginTransaction();
	}

	/**
	 * @desc Commit Transaction
	 * @return
	 */
	public function commitTrans(){
		expDebug::dPrint("Commit Called",5);
		$this->link->commit();
	}

	/**
	 * @desc Rollback Transaction
	 * @return
	 */
	public function rollbackTrans(){
		expDebug::dPrint("Rollback Called",5);
		$this->link->rollBack();
	}


 /**
  * @desc Executes a query to the selected database by calling POD query.
  * @param $query String
  * 	(mandatory) Runs the query on the selected database.
  * @param $arguments Array
  * 	(optional) Arguments of the query
  * @return The PDO query statement object.
  */
	function query($query,$arguments=null) {
		try {
			$this->statement = $this->link->prepare($query);
			$this->statement->execute($arguments);
			$this->result=$this->statement;
			if($this->getErrorCode()!='0000'){
				$this->getErrorInfo();
			}
			return $this->statement;
		}catch (PDOException $e) {
			$this->callQuery('SHOW FULL PROCESSLIST');
			$pList = $this->fetchAllResults();
			expDebug::dPrint("Process list while error in query ".print_r($pList,true),1);
	    expDebug::dPrint("Error in PDO query:".$e->getMessage(),1);
			throw new PDOException($e->getMessage());
    }
	}

 /**
  * @desc Executes a query to the selected database by calling POD execute.
  * @param $query String
  * 	(mandatory) Runs the query on the selected database.
  * @param $arguments Array
  * 	(optional) Arguments of the query
  * @return The PDO query statement object.
  */
	public function execute($qyery,$arguments=null){
		try{
			$this->statement=$this->link->prepare($qyery);
			$this->result = $this->statement->execute($arguments);
			return $this->statement;
		}catch(PDOException $e){
			expDebug::dPrint("Error in PDO :".$e->getMessage(),1);
			$this->callQuery('SHOW FULL PROCESSLIST');
			$pList = $this->fetchAllResults();
			expDebug::dPrint("Process list while error in execute ".print_r($pList,true),1);
			throw new PDOException($e->getMessage());
		}
	}

	/**
	 * To get inserted record number
	 * @return mixed id
	 */
	public function getLastRecordNo(){
		return $this->link->lastInsertId();
	}

	/**
	 * To get PDO error code
	 * @return String
	 */
	private function getErrorCode(){
		return $this->statement->errorCode();
	}

	/**
	 * To get PDO error informations
	 * @return String
	 */
	private function getErrorInfo(){
		$err =  $this->statement->errorInfo();
		throw new PDOException($err[2]);
	}

 /**
  * @desc Fetches the results for the executed query as objects.
  * @return The query results.
  */
	function fetchResult()
	{
		return $this->statement->fetch(PDO::FETCH_OBJ);
	}
	
	/**
	 * Fetches the results for the executed query as associative array
	 * @return unknown_type
	 */
	function fetchAssociate()
	{
		return $this->statement->fetch(PDO::FETCH_ASSOC);
	}	

	/**
	 * Fetches the column result for the executed query as value from single column 
	 * @return unknown_type
	 */
	function fetchColumn(){
		return $this->statement->fetch(PDO::FETCH_COLUMN);
	}
	/**
	 * Fetch all the results as mixed object
	 * @return unknown_type
	 */
	public function fetchAllResults(){
		$outObj = array(new stdClass());
		$cnt=0;
		while($ob = $this->fetchResult()){
			$outObj[$cnt]=$ob;
			$cnt++;
		}
		if($cnt==0){
			$outObj = array();
		}
		return $outObj;
	}
	
	/**
	 * Prepare a select query
	 * @param $table String
	 * 	(mandatory) name of the primary table
	 * @param $fields Array
	 *  (mandatory) list of fetch fields
	 * @param $conditon Array
	 *  (optional) list of contions which needs to be added
	 * @param $joins Array
	 *  (optional) list of join tables
	 * @param $other String
	 *  (optional) list of other attributes such as
	 *  group, sort, limit etc. Should be in proper order
	 * @return String
	 *  return executable query
	 */
	public function prepareQuerySelect($table,$fields,$conditon=array(),$joins=array(),$other=''){
		$qry = "SELECT ";
		$qry .= implode(',',$fields);
		$qry .= " ";
		$qry .= " FROM ".$table;
		if($joins)
			$qry .= " " . implode(' ',$joins);
		if($conditon)
			$qry .= " WHERE " . implode(' and ',$conditon);
		if($other)
			$qry .= ' '.$other;
		return $qry;
	}
	
	/**
	 * Prepare a update query
	 * @param $table String
	 * 	(mandatory) name of the primary table
	 * @param $fields Array
	 *  (mandatory) list of fetch fields with values
	 *  should be associated array format
	 * @param $conditon Array
	 *  (optional) list of contions which needs to be added
	 * @param $joins Array
	 *  (optional) list of join tables
	 * @param $other String
	 *  (optional) list of other attributes such as
	 *  group, sort, limit etc. Should be in proper order
	 * @return String
	 *  return executable query
	 */
	public function prepareQueryUpdate($table,$fields,$conditon=array(),$joins=array(),$other=''){
		$qry = "UPDATE ";
		$qry .= $table;
		if($joins)
			$qry .= ' '.implode(' ',$joins);
		$qry .= " SET ";
		foreach($fields as $key=>$value) { $fields_string[] = $key.'='.$value; }
		$qry .= ' ' .implode(',',$fields_string);
		if($conditon)
			$qry .= " WHERE " . implode(' and ',$conditon);
		if($other)
			$qry .= ' '.$other;
		return $qry;
	}
	
	/**
	 * Prepare a insert query
	 * @param $table String
	 * 	(mandatory) name of the primary table
	 * @param $fields Array
	 *  (mandatory) list of insert fields with values
	 *  should be associated array format
	 * @return String
	 *  return executable query
	 */
	public function prepareQueryInsert($table,$fields){
		foreach($fields as $key=>$val) { $field[] = $key; $value[]=$val; }
		$qry = "INSERT INTO ";
		$qry .= $table;
		$qry .= " (" . implode(',',$field) . ") ";
		$qry .= " VALUES ";
		$qry .= " (" . implode(',',$value) . ") ";
		return $qry;
	}
	
	/**
	 * Call execute query with options
	 * this makes db conections and after runing a query 
	 * the connect will get closed
	 * @param $query String
	 *  (mandatory) query to be execute
	 * @param $arguments Array
	 *  (optional) list of arguments which to be substitute to the query
	 *  should be associated array
	 * @return unknown_type
	 */
	public function callExecute($query,$arguments=null){
		try{
			$this->connect();
			if($this->update_queue){
				$tmp = 'UPDATE slt_dataload_process_queue SET sql_id = CONNECTION_ID() WHERE pid = :pid ';
				$this->execute($tmp, array(':pid'=>getmypid()));
			} 
			$rtn = $this->execute($query,$arguments);
			$rtn = $this->link->lastInsertId();
			$this->closeconnect();
			return $rtn;
		}catch(PDOException $ex){
			$this->callQuery('SHOW FULL PROCESSLIST');
			$pList = $this->fetchAllResults();
			expDebug::dPrint("Process list while error in callExecute ".print_r($pList,true),1);
			$this->closeconnect();
			throw new PDOException($ex->getMessage());
		}
	}
	
	/**
	 * Call query with options
	 * this makes db conections and after runing a query 
	 * the connect will get closed
	 * @param $query String
	 *  (mandatory) query to be execute
	 * @param $arguments Array
	 *  (optional) list of arguments which to be substitute to the query
	 *  should be associated array 
	 * @return unknown_type
	 */
	public function callQuery($query,$arguments=null){
		try{
			$this->connect();
			if($this->update_queue){
				$tmp = 'UPDATE slt_dataload_process_queue SET sql_id = CONNECTION_ID() WHERE pid = :pid ';
				$this->execute($tmp, array(':pid'=>getmypid()));
			}
			$rtn = $this->query($query,$arguments);
			$this->closeconnect();
			return $rtn;
		}catch(PDOException $ex){
			$this->callQuery('SHOW FULL PROCESSLIST');
			$pList = $this->fetchAllResults();
			expDebug::dPrint("Process list while error in batch execute ".print_r($pList,true),1);
			$this->closeconnect();
			throw new PDOException($ex->getMessage());
		}
	}
	
	/**
	 * Prepare insert statement by selecting records from other table
	 * @param $insert Array
	 *  - This array objcet shoud contain the following elements
	 *   - table String - name of the table to insert record
	 *   - fields Array - list of columns to be insert
	 * @param $select Array
	 *  - This array object should contain the following elements
	 *   - table String - from where the source records shold fetch
	 *   - fields Array - list of columns to fetch
	 *   - condition Array - (optional) list of conditions
	 *   - join Array - (optional) list of join tables
	 *   - other String - (optional) other query statements 
	 *     such as group by, order by, limit etc.
	 * @return $qry String
	 */
	public function prepareInsertBySelect($insert,$select){
		$qry = " INSERT INTO ";
		$qry .= $insert['table'];
		if(isset($insert['fields']))
			$qry .= " (". implode(',',$insert['fields']) . ") ";
		$table = $select['table'];
		$fields = $select['fields'];
		$condition = isset($select['condition'])?$select['condition']:null;
		$joins = isset($select['joins'])?$select['joins']:null;
		$others = isset($select['others'])?$select['others']:null;
		$qry .= " (". $this->prepareQuerySelect($table,$fields,$condition,$joins,$others) .") ";
		return $qry;
	}
	
	/**
	 * Prepare update statement by selecting values from other tables 
	 * @param $update Array
	 *  - This array should contain following elements
	 *   - table String - (mandatory) name of the table to update
	 *   - fields Array - (mandatory) list of columns to be update
	 *   - condition Array - (optional) list common condition for update
	 * @param $select Array
	 *  - This array should contain arrays elements,
	 *  each element should be an array with the following elements
	 *   - table String - name of the table
	 *   - fields Array - list of fetch fields
	 *   - condition Array - list of conditions
	 *   - joins Array - list of joins
	 *   - others String - any other statements such as
	 *     group by, order by, limit etc.
	 *   - alias - alias name of this select
	 *  
	 * @return $qry String
	 */
	public function prepareUpdateBySelect($update,$select){
		$qry = " UPDATE ";
		$qry .= $update['table'];
		if($select){
			foreach($select as $sel){
				$table = $sel['table'];
				$fields = $sel['fields'];
				$condition = isset($sel['condition'])?$sel['condition']:null;
				$joins = isset($sel['joins'])?$sel['joins']:null;
				$others = isset($sel['others'])?$sel['others']:null;
				$qry .= " , ( " . $this->prepareQuerySelect($table,$fields,$condition,$joins,$others) .") ".$sel['alias'];
			}
			$qry .= " SET ";
			foreach($update['fields'] as $key=>$value) { $fields_string[] = $key.'='.$value; }
			$qry .= ' ' .implode(',',$fields_string);
			if(isset($update['condition']))
			$qry .= " WHERE " . implode(' and ',$update['condition']);
			return $qry;
		}
	}
	
	public function exec($query){
		$this->connect();
		$this->link->exec($query);
		$this->closeconnect();
	}
	
	public function getAttribute($attr){
		$this->connect();
		$attribute = $this->link->getAttribute($attr);
		$this->closeconnect();
		return $attribute;
	}
}
?>