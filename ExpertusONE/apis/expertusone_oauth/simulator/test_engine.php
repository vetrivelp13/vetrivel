<?php
/*
 * ExpertusOne API
 * Test Engine.php v1.0
 * 
 * @author: Rajkumar U
 * @date	:	02-Jan-2012
 * 
 * Automated tests for the Dynamic REST API's
 * 
 * DONOT MODIFY THIS FILE UNTIL IS ABSOLUTELY NECESSARY
 */
include_once($_SERVER["DOCUMENT_ROOT"].'/apis/dynamicapi/includes.php');

class TestEngine{
	
	protected $AE = null;
	protected $CE = null;
	protected $RE = null;
	protected $EE = null;
	function __construct(){
		$this->AE = new AssertionEngine();
		$this->CE = new CoreEngine();
		$this->RE = new RewriteEngine();
		$this->EE = new ExecutionEngine();
	}
	function RunTest($xmlstring){
		
		
		if(stripos($xmlstring,"xdebug-error")>0)
		{
			$a = new CoreEngine();
			$a->prepareDetals($_GET['apiname']);
			$details = $a->getFullDetails();
  			$this->sendMail($details['api_owner']['name'], $details['api_owner']['email'],$_GET['apiname'],$xmlstring);
  			echo "Fail";
			
		}
  		$array = $this->AE->parseXML($xmlstring);
  		if($this->AE->findID($array)){
  			echo"Pass";
  		} else {
  			
  			$a = new CoreEngine();
				$a->prepareDetals($_GET['apiname']);
				$details = $a->getFullDetails();
  			$this->sendMail($details['api_owner']['name'], $details['api_owner']['email'],$_GET['apiname'],$xmlstring);
  			echo "Fail";
  		}
	}
	
	function sendMail($name,$to,$apiname,$xmlstring){
		$build_no = $this->getVersion();
		$date = date('M,d Y');
		$errorMsg = htmlspecialchars($xmlstring);
		$subject = "Dynamic Rest API Test Report";
		$message = "<br />Hi $name<br />
			$apiname API is not working properly in this $build_no build. Please resolve the issue and commit the code in SVN.<br />
			<table border='1'><tr><td colspan='3'>
			Error description</td></tr><tr><td colspan='3'>
			$errorMsg
			</td></tr>
			<tr><td colspan='3'>
			Build History
			</td></tr>
			<tr><td>Build No</td><td>Status</td><td>Date</td></tr>
			<tr><td>$build_no</td><td>Fail</td><td>$date</td></tr>
			<tr><td colspan='3'>
			If you have any issues/queries, please contact your product development head.
			</td></tr>
			</table><br />
			Thanks & Regards,<br />
			Technical Team, ExpertusONE.";
		$headers  = 'MIME-Version: 1.0' . "\r\n";
		$headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";
		mail($to,$subject,$message,$headers);
		//echo $message;
	}
	
function sendMail2head($messag){
		$build_no = $this->getVersion();
		$date = date('M,d Y');
		$to = 'sureshkumarv@expertus.com';
		$subject = "Dynamic Rest API Test Report";
		$message = "<br />Hi, <br />The following API's were tested and the results are as follows,<br /><br />
			$messag
			<br/><table border='1'>
			<tr><td colspan='3'>
			Build History
			</td></tr>
			<tr><td>Build No</td><td>Date</td></tr>
			<tr><td>$build_no</td><td>$date</td></tr>
			</table><br />
			Thanks & Regards,<br />
			Technical Team, ExpertusONE.";
		$headers  = 'MIME-Version: 1.0' . "\r\n";
		$headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";
		mail($to,$subject,$message,$headers);
		//echo $message;
	}
	
	function getVersion(){
		$version = '';
		$file = fopen($_SERVER["DOCUMENT_ROOT"].'/VERSION.txt', "r") or exit("Unable to open file!");
		while(!feof($file))
	  {
	  	$version .= fgets($file);
	  }
	  $versionArray = explode(" ",$version);
		return $versionArray[0];
	}
	
}

class AssertionEngine{
	
	function parseXML($xmlstring){
		return simplexml_load_string($xmlstring);
	}
	
	function findID($array){
		$result=array();
		$result = $array ->xpath("result/id");
		expDebug::dPrint("result in findID ".sizeof($result));
	    if (sizeof($result)==0)	
		{
			expDebug::dPrint("testing findID....");
			$result = $array ->xpath("result/Status");
			expDebug::dPrint("result in findID ".sizeof($result));
		}
		if (sizeof($result)==0)
			$result = $array ->xpath("result/Id");
		if (sizeof($result)==0)
			$result = $array ->xpath("result/ID");
		if (sizeof($result)==0)
			$result = $array ->xpath("result/PID");
	    if (sizeof($result)==0)	
			$result = $array ->xpath("result/name");
		if (sizeof($result)==0)	
			$result = $array ->xpath("result/status");	
		if (sizeof($result)==0)	
			$result = $array ->xpath("result/crs_id");
		if (sizeof($result)==0)	
			$result = $array ->xpath("result/enrol_pid");
		
			$bool = false;
		expDebug::dPrint(" last result in findID ".sizeof($result));
		if (!sizeof($result)==0) 
		{
    		$bool =  true;
		} else 
		{
		    $bool =  false;
		}
		
	
		return $bool;
	}
	
	
}
$obj = new TestEngine();
if(isset($_GET['mail2head'])){
	$obj->sendMail2head($_GET['mail2head']);
	
} else {
	//print rawurldecode($_GET['xmlstring']);
	if(isset($_GET['xmlstring']) && $_GET['xmlstring']!="")
	{
		$a = strval(htmlspecialchars_decode($_GET['xmlstring']));
		$obj->RunTest($a);
	}
}
?>