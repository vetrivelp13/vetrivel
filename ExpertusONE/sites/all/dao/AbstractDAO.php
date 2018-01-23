<?php
/**
* @class or PHP Name		: AbstractDAO
* @author(s)				: Vincent.S
* Version         			: 1.0
* Date						: 09/07/2009
* PHP Version          		: 5.2.6
* Description     			: DAO Class to do the actual operation with table.
*/

//include "../services/GlobalUtil.php";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/GlobalUtil.php";

class AbstractDAO{
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
	function __construct($from = '') {
		try{
			$util=new GlobalUtil();
			$conf=$util->getConfig();
			if($from!='Report')
				$db_url=$conf["db_url"];
			else
				$db_url=$conf["report_db_url"];
			expDebug::dPrint("DB URL --->".$db_url);
			/*$tmp = explode(":",$db_url);
			$this->dsn=$tmp[0];
			$this->username=substr($tmp[1],2,strlen($tmp[1]));
			$this->password=substr($tmp[2],0,stripos($tmp[2],"@"));
			$this->host=substr($tmp[2],stripos($tmp[2],"@")+1,(stripos($tmp[2],"/")-stripos($tmp[2],"@"))-1);
			$this->db=substr($tmp[2],stripos($tmp[2],"/")+1,strlen($tmp[2]));*/
			
			$tmp1=explode("@",$db_url);
			$tmp = explode(":",$db_url);
			$this->dsn=$tmp[0];
			$this->username=substr($tmp[1],2,strlen($tmp[1]));
			$this->password=substr($tmp[2],0,stripos($tmp[2],"@"));
			$hst=explode(":",substr($tmp1[1],0,stripos($tmp1[1],"/")));
			$this->host=$hst[0];
			if(isset($hst[1]))
				$this->port=$hst[1]!=NULL || $hst[1] != 'undefined' || $hst[1] != '' ?$hst[1]:NULL; 
			$this->db=substr($tmp1[1],stripos($tmp1[1],"/")+1,strlen($tmp1[1]));
			expDebug::dPrint("dsn --->>".$this->dsn);
			expDebug::dPrint("username --->>".$this->username);
			expDebug::dPrint("password --->>".$this->password);
			expDebug::dPrint("host --->>".$this->host);
			expDebug::dPrint("port --->>".$this->port);
			expDebug::dPrint("db --->>".$this->db);
		}catch(Exception $e){
			expDebug::dPrint("Error in PDO :".$e->getMessage());
			//throw new Exception($e->getMessage());
			throw new Exception('Website encountered an error.');
		}
	}

	function __destruct()
	{
		try
		{
			if($this->link){

				//$this->statement->closeCursor();
				$this->link=null;
				expDebug::dPrint("Cursor Close destruct is called");
			}
		}
		catch(PDOException $e)
		{
			expDebug::dPrint("Error in PDO :".$e->getMessage());
			//throw new Exception($e->getMessage());
			throw new Exception('Website encountered an error.');
	        //print "Error!: " . $e->getMessage() . "in PDO Module"."<br/>";
	        //die();
		}
	}

	function closeconnect()
	{
		try
		{
			//$this->statement->closeCursor();
			$this->link=null;
			expDebug::dPrint("Cursor Close is called");
		}
		catch(PDOException $e)
		{
			expDebug::dPrint("Error in PDO :".$e->getMessage());
			//throw new Exception($e->getMessage());
			throw new Exception('Website encountered an error.');
	        //print "Error!: " . $e->getMessage() . "in PDO Module"."<br/>";
	        //die();
		}
	}

	 /**
	  * @desc Connects to a database.
	  */
	function connect() {
		try {
			$this->link=new PDO($this->dsn.":host=$this->host;port=$this->port;dbname=$this->db", $this->username, $this->password, array( PDO::ATTR_PERSISTENT => false,PDO::MYSQL_ATTR_INIT_COMMAND => "SET NAMES utf8"));
			$this->link->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
			$this->link->setAttribute(PDO::MYSQL_ATTR_USE_BUFFERED_QUERY, true);
			return $this->link;
		} catch (PDOException $e) {
	        //print "Error!: " . $e->getMessage() . "in PDO Module". "<br/>";
	        //die();
	        expDebug::dPrint("Error in PDO :".$e->getMessage());
	        //throw new Exception($e->getMessage());
	        throw new Exception('Website encountered an error.');
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
		expDebug::dPrint("Commit Called");
		$this->link->commit();
	}

	/**
	 * @desc Rollback Transaction
	 * @return
	 */
	public function rollbackTrans(){
		expDebug::dPrint("Rollback Called");
		$this->link->rollBack();
	}


 /**
  * @desc Executes a query to the selected database.
  * @param $query
  * 	(mandatory) Runs the query on the selected database.
  * @return The PDO query statement object.
  */
	function query($query, $throw = FALSE) {
		try {
			$this->statement = $this->link->prepare($query);
			$this->statement->execute();
			$this->result=$this->statement;
			if($this->getErrorCode()!='0000'){
				$this->getErrorInfo();
			}
			return $this->statement;
		} catch (PDOException $e) {
	        expDebug::dPrint("Error in PDO query:".$e->getMessage());
	         expDebug::dPrint("Error in PDO opt :".print_r($throw,true),1);
			//return "Error!: " . $e->getMessage() . "<br/>";
			//throw new Exception($e->getMessage());
			if($throw === TRUE){
				expDebug::dPrint("Error in PDO opt if:".print_r($throw,true),1);
				throw new PDOException($e->getMessage());
			}else{
				expDebug::dPrint("Error in PDO opt else:".print_r($throw,true),1);
				throw new Exception('Website encountered an error.');
	        die();
			}
    }
	}

	public function execute($qyery){
		try{
			$this->statement=$this->link->prepare($qyery);
			$rest = $this->statement->execute();
			return $rest;
		}catch(PDOException $e){
			expDebug::dPrint("Error in PDO :".$e->getMessage());
			//return "Error!: " . $e->getMessage() . "<br/>";
			//throw new Exception($e->getMessage());
			throw new Exception('Website encountered an error.');
	        die();
		}
	}

	public function getLastRecordNo(){
		return $this->link->lastInsertId();
	}

	private function getErrorCode(){
		return $this->statement->errorCode();
	}

	private function getErrorInfo(){
		$err =  $this->statement->errorInfo();
		//throw new Exception($err[2]);
		throw new Exception('Website encountered an error.');
	}

 /**
  * @desc Fetches the results for the exeuted quuery.
  * @return The query results.
  */
	function fetchResult()
	{
		return $this->result->fetch(PDO::FETCH_OBJ);
	}
	
/**
	 * @desc Fetches the result column for the exeuted quuery.
	 * @return The query results.
	 */
	function fetchColumn()
	{
		return $this->result->fetchColumn();
	}
	function select($tableName,$colulmnName) {
		$this->connect();
		$query="select ".$colulmnName." from ".$tableName;
		return $query;
	}
	function insert($tableName,$colulmnName,$columnValue) {
		$this->connect();
		$query="insert into ".$tableName."(".$colulmnName.") values(".$columnValue.")";
		return $query;
	}
	function update($tableName,$colulmnName,$columnValue) {
		$this->connect();
		$query="update ".$tableName." set ".$colulmnName."=".$columnValue;
		return $query;
	}
	function delete($tableName,$colulmnName,$columnValue) {
		$this->connect();
		$query="delete * from ".$tableName;
		return $query;
	}

	function callProcedure($procedureName){
		$this->connect();
		$query="call ".$procedureName;
		return $query;

	}

	function getLoggedInUserId(){

	    $oUserInfo = new UserInfo();
	    $userId=$oUserInfo->getLoggedInUserId();
		if(!isset($userId) || $userId == ''){
			$userId = 1;
		}
	  	return $userId;

	}

	public function fetchAllResults(){
		$outObj = array(new stdClass());
		$cnt=0;
		while($ob = $this->fetchResult()){
			//expDebug::dPrint("****************************************");
			//foreach($ob as $key=>$value ){
				//expDebug::dPrint($key.":===:".$value);
				//$outObj[$cnt]->$key=$value;
				$outObj[$cnt]=$ob;
			//}
			$cnt++;
		}
		if($cnt==0){
			$outObj = array();
		}

		return $outObj;
	}
}
?>