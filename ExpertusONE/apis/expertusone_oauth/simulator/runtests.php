<?php
/*
 * ExpertusOne API
 * Test Runner v1.0
 * 
 * @author: Rajkumar U
 * @date	:	02-Jan-2012
 * 
 * All tests, functions related to the Dynamic REST API resides here.
 * 
 * DONOT MODIFY THIS FILE UNTIL IS ABSOLUTELY NECESSARY
 * 
 */

include_once($_SERVER["DOCUMENT_ROOT"].'/apis/dynamicapi/includes.php');

class TestRunner{
	var $CE = null;
	
	function __construct(){
		$this->CE = new CoreEngine();
	}
	function getListAPIS(){
		$apiList = $this->CE->getDocumentation();
		$string = array();
		$list = array();
		foreach($apiList as $key => $val){
			if($key != 'documentation'){
				 if($val['test_category'] == 'list'){
					$list[] = $key;
				} 
			}
		}
		
		foreach($list as $key => $val){
			$string[] = $val;
		}
		return $string;
	}
	
	function getAddAPIS(){
		$apiList = $this->CE->getDocumentation();
		$string = array();
		$add = array();
		foreach($apiList as $key => $val){
			if($key != 'documentation'){
				 if($val['test_category'] == 'add'){
					$add[] = $key;
				} 
			}
		}
		
		foreach($add as $key => $val){
			$string[] = $val;
		}
		return $string;
	}
	
	function getUpdateAPIS(){
		$apiList = $this->CE->getDocumentation();
		$string = array();
		$update = array();
		foreach($apiList as $key => $val){
			if($key != 'documentation'){
				 if($val['test_category'] == 'update'){
					$update[] = $key;
				} 
			}
		}
		
		foreach($update as $key => $val){
			$string[] = $val;
		}
		return $string;
	}
	
	function testAPIListFull(){
		$apiList = $this->CE->getDocumentation();
		//expDebug::dPrint(" APIS LIST...");
		//expDebug::dPrint($apiList);
		$string = array();
		$add = array();
		$list = array();
		$update = array();
		foreach($apiList as $key => $val){
			if($key != 'documentation'){
				expDebug::dPrint(" test engine....");
				expDebug::dPrint($val["testengine"]);
				$string[(int)($val["testengine"]["apitestsequence"])]=$key;
			}
		}
		return $string;
	}
	
	function getTestVal(){
		$valarry = $this->CE->getParams();
		$paramary = array();
		$this->formatTestVal($paramary,$valarry);
		return $paramary;
	}
	
	function formatTestVal(&$paramArray,$valArray){
		foreach($valArray as $par => $pval){
			if(is_array($pval)){
				$this->formatTestVal($paramArray,$pval);
			} else {
				$temparr = explode('>',$pval);
				$paramArray[$par] = $temparr[2];
			}
		}
	}
	function stringAccumulator(&$string,$array,$parm){
	foreach($array as $key => $val){
			if(is_array($val)){
					$this->stringAccumulator($string,$val,$parm);
			} else {
				$string .= '&'.trim($key).'='.urlencode(trim($parm[$key]));
			}
		}
	}
	
	function rewriteParams(&$paramArray,$srcArray,$buildinfo){
		
		foreach($srcArray as $key => $val){
			if(is_array($val)){
				$this->rewriteParams($paramArray,$val,$buildinfo);
			} else {
				$paramArray[$key] = str_replace("VERSION",$buildinfo,$val);
			}
		}
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
	
	function getTestEngineParameters()
	{
		return $this->CE->getTestEngineParams();
	}
	
	function getValueFromXML($xmlstr,$xpath)
	{
		expDebug::dPrint("xmlstr");
		expDebug::dPrint($xmlstr);
		$value="";
		if(!stripos($xmlstr,"xdebug-error")>0 && !stripos($xmlstr,"error_code")>0)
		{
			$v=simplexml_load_string($xmlstr);
			$tmp_arr=$v->xpath($xpath);//"result/id");
			if(isset($tmp_arr[0]))
				$value=$tmp_arr[0];
				
			else 
			{
				$tmp_arr=$v->xpath($xpath);//"result/Id");
				if(!isset($tmp_arr[0]))
				{
					$tmp_arr=$v->xpath($xpath);//"result/ID");
					if(isset($tmp_arr[0]))
						$value=$tmp_arr[0];
				}
				else
					$value=$tmp_arr[0];
			}
			expDebug::dPrint("test....".$value);
			return " ".$value;
		}
		else
		{
			return "";
		}
	}
	function SimParamForm($apiname){
		$this->CE->prepareDetals($apiname);
		$dispparams = $this->CE->getRespCols();
		$disp = '';
		$i = 1;
		foreach($dispparams as $key => $val){
			$disp .= trim($key);
			if($i < count($dispparams)){
				$disp .= ",";
				$i++;
			}
		}
		$parm = array();
		$parm1 = $this->getTestVal();
		$this->rewriteParams($parm,$parm1,$this->getVersion());
		$apiparams = $this->CE->getParams();
		$string = 'returntype=xml&apiurl='.$this->getDomainDoc().'/apis/ext/ExpertusOneAPI';
		$string .= '&apiname='.$apiname.'&display_cols='.$disp;
		
		$this->stringAccumulator($string,$apiparams,$parm);
		expDebug::dPrint("string....".$string);
		return $string;
	}

	function getDomainDoc()
	{
	   $url = (!empty($_SERVER['HTTPS'])) ? "https://".$_SERVER['SERVER_NAME'].$_SERVER['REQUEST_URI'] : "http://".$_SERVER['SERVER_NAME'].$_SERVER['REQUEST_URI'];
	   $parts = parse_url($url);
	   return $parts['scheme'].'://'.$parts['host'];
	}	
	
	function HTTP_GET_REQUEST($url){
		$ch = curl_init();
		curl_setopt($ch, CURLOPT_URL, $url);
		curl_setopt($ch, CURLOPT_HEADER, 0);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
		$output = curl_exec($ch);
		curl_close($ch);
		return  $output;	
	}
	
	function constructFullURL($filename,$params){
		$url = $_SERVER['SERVER_NAME']."/apis/expertusone_oauth/simulator/$filename.php?$params";
		return $url;
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
		function modifyParams($params,$newval,$col)
		{
				$pos1=stripos($params,"&".$col."=");
				$pos2=stripos($params,"&",($pos1+1));
				expDebug::dPrint("pos2...".$pos2." new vaL.".$newval);
				$str1="";
				if($pos2=="" || $pos2<=0 )
				{
					expDebug::dPrint("inside if");
				
					$str1=substr($params,$pos1);
					expDebug::dPrint($str1." pos2".$pos2);
				}
				else
				{
					$val1=$pos2-$pos1;
					$str1=substr($params,$pos1,$val1);
				}
				$splitkeyval=explode("=",$str1);
				$newidval=$splitkeyval[0]."=".$newval;
				
				$str2=substr($params,0,$pos1);
				$str2.=$newidval;
				$str3="";
				if($pos2>0)
					$str3=substr($params,$pos1+ ($pos2-$pos1));
				
				$modified_params=$str2.$str3;
				expDebug::dPrint("modified params..".$modified_params);
				return $modified_params;
		}
		function getParamValue($params,$col)
		{
				$pos1=stripos($params,"&".$col."=");
				$pos2=stripos($params,"&",($pos1+1));
				$val1=$pos2-$pos1;
				$str1=substr($params,$pos1,$val1);
				expDebug::dPrint("string value from getParamValue.".$str1);
				return $str1;
		}
		function disallow_apis_for_test($apiname)
		{
			if(	
			    //$apiname=="AddOrganizationAPI" || 
			//	$apiname=="UserCreationAPI" ||
			//	$apiname=="CreateCourseAPI" ||
			//	$apiname=="CreateClassAPI" || 
			//	$apiname=="UpdateOrganizationAPI" ||
			//	$apiname=="UserUpdationAPI" ||
			//	$apiname=="UpdateCourseAPI" ||
			//	$apiname=="UpdateClassAPI"  ||
				$apiname=="AddRosterAPI"  ||
				$apiname=="UpdateRosterAPI" 
				//|| $apiname=="ListAnnouncementsAPI" 
			)
				return true;
			else
				return false;
		}
		public $wbtSessionContentId="";
		public $iltSessionContentId="";
		public $locId="1";
		public $facId="1";
		public $pre_req_courseid="31";
		public $pre_req_course_map_id="";
		public $equv_map_id="";
		public $enrollId="";
		public $contentLessionId="";
		public $userId="";
		public $orgId="";
		public $courseId="";
		public $iltClassId="";
		public $wbtClassId="";		
		public $userName="";
		
}
$SE = new TestRunner();

$apilist = $SE->testAPIListFull();
ksort($apilist);
expDebug::dPrint(" api list order ");
expDebug::dPrint($apilist);

$str =  '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Dynamic API Automated Testing</title></hrad><body style="background-color: #99ccFF"><div style="width:50%;clear:both;margin:auto;">
<h1 style="text-align:center;">Dynamic API Automated Test</h1>
<div id="tabledata">';
$mailstr = '<table border="1" style="width:100%;border:solid;"><tr><td>API Name</td><td>Status</td><td>Result</td></tr>';
$str .= '<table border="1" style="width:100%;border:solid;"><tr><td>API Name</td><td>Status</td><td>Result</td></tr>';



/*for($i=0;$i<3;$i++)
{
	if($i==0)
		$apilist=$SE->getAddAPIS();
	else if($i==1)
		$apilist=$SE->getUpdateAPIS();
	else
		$apilist=$SE->getListAPIS();
*/	
	$test_value_for_nextapi="";	
	$iltClassId="1";
	$wbtClassId="2";
	$contentId="1";
	$wbtSessionContentId="";
	
	foreach($apilist as $key => $val){
		expDebug::dPrint("VAL");
		expDebug::dPrint($val);
		if($SE->disallow_apis_for_test($val))
		{
			/*$params = $SE->SimParamForm($val);
			$testparams=$SE->getTestEngineParameters();
			if($val=="UpdateOrganizationAPI")
			{
				$params=$SE->modifyParams($params,$orgId,"Id");
			}
			if($val=="UserUpdationAPI")
			{
				$params=$SE->modifyParams($params,$userId,"Id");
			}
			if($val=="UpdateCourseAPI")
			{
				$params=$SE->modifyParams($params,$courseId,"Id");
			}
			$url = $SE->constructFullURL("simulator_invoker",$params);
			$res=$SE->HTTP_GET_REQUEST($url);
			
			
			if($val=="AddOrganizationAPI")
			{
				$test_value_for_nextapi=trim($SE->getValueFromXML($res,"Id"));
				$orgId=$test_value_for_nextapi;
			}
			else if($val=="UserCreationAPI")
			{
				$test_value_for_nextapi=trim($SE->getValueFromXML($res,"Id"));
				$userId=$test_value_for_nextapi;
			}
			else if($val=="CreateCourseAPI")
			{
				$test_value_for_nextapi=trim($SE->getValueFromXML($res,"Id"));
				$courseId=$test_value_for_nextapi;
			}
			else if($val=="CreateClassAPI")
			{
				$test_value_for_nextapi=trim($SE->getValueFromXML($res,"Id"));
				$classId=$test_value_for_nextapi;
			}
			expDebug::dPrint(" update apivalue .");
			expDebug::dPrint($test_value_for_nextapi);
			
			$res = htmlspecialchars($res);
			$teparams = $params."&xmlstring=".rawurlencode($res);
			$teurl = $SE->constructFullURL("test_engine",$teparams);
			$teres = $SE->HTTP_GET_REQUEST($teurl);
			$mailstr .= "<tr><td>$val</td><td>$teres</td><td>$res</td></tr>";
			$str .= "<tr><td>$val</td><td>$teres</td><td>$res</td></tr>"; */
		}
		else
		{
			$params = $SE->SimParamForm($val);
			$testparams=$SE->getTestEngineParameters();
			expDebug::dPrint(" update apivalue else .".$params);
			//expDebug::dPrint($test_value_for_nextapi);
			if($val=="UpdateOrganizationAPI")
			{
				$params=$SE->modifyParams($params,$SE->orgId,"Id");
				expDebug::dPrint("update org params....".$params);
			}
			else if($val=="UserUpdationAPI")
			{
				$params=$SE->modifyParams($params,$SE->userId,"Id");
			}
			else if($val=="UpdateCourseAPI")
			{
				$params=$SE->modifyParams($params,$SE->courseId,"Id");
			}
			else if($val=="CreateClassAPI")
			{
				$params=$SE->modifyParams($params,$SE->courseId,"Id");
			}
			else if($val=="UpdateClassAPI")
			{
				$params=$SE->modifyParams($params,$SE->courseId,"Id");
				$params=$SE->modifyParams($params,$SE->wbtClassId,"ClassId");
			}
		
			else if($val=="AssociateContentwithWBTClassAPI")
			{
				$params=$SE->modifyParams($params,$SE->courseId,"CourseId");
				$params=$SE->modifyParams($params,$SE->wbtClassId,"ClassId");
				expDebug::dPrint("AssociateContentwithWBTClassAPI");
				expDebug::dPrint('wbtclass'.$SE->wbtClassId);
				
				
				$params=$SE->modifyParams($params,$contentId,"ContentId");
				$params=$SE->modifyParams($params,"WBT","DTType");
			}
			else if($val=="AssociateFacilityLocationRoomAPI")
			{
				$params=$SE->modifyParams($params,$SE->courseId,"CourseId");
				$params=$SE->modifyParams($params,$SE->iltClassId,"ClassId");
				$params=$SE->modifyParams($params,$SE->facId,"FacilityId");
				$params=$SE->modifyParams($params,$SE->locId,"LocationId");
				expDebug::dPrint("modified resp ".$params);
			}
			else if($val=="EditAssociatedFacilityLocationRoomAPI")
			{
				$params=$SE->modifyParams($params,$SE->courseId,"CourseId");
				$params=$SE->modifyParams($params,$SE->iltClassId,"ClassId");
				$params=$SE->modifyParams($params,$SE->facId,"FacilityId");
				$params=$SE->modifyParams($params,$SE->locId,"LocationId");
				$params=$SE->modifyParams($params,$SE->iltSessionContentId,"SessionId");
				expDebug::dPrint("modified resp ".$params);
			}
			else if($val=="RemoveContentFacilityLocationRoomAPI")
			{
				$params=$SE->modifyParams($params,$SE->iltSessionContentId,"SessionId");
			}
			else if($val=="EditAssociatedContentwithWBTClassAPI")
			{
				$params=$SE->modifyParams($params,$SE->courseId,"CourseId");
				$params=$SE->modifyParams($params,$SE->wbtClassId,"ClassId");
				$params=$SE->modifyParams($params,$contentId,"ContentId");
				$params=$SE->modifyParams($params,"WBT","DTType");
				expDebug::dPrint("wbt session id::".$SE->wbtSessionContentId);
				$params=$SE->modifyParams($params,$SE->wbtSessionContentId,"SessionId");
			}
			else if($val=="CoursePreRequisitesAPI")
			{
				$params=$SE->modifyParams($params,$SE->courseId,"id1");
				$params=$SE->modifyParams($params,$SE->pre_req_courseid,"id2");
			}
			else if($val=="RemoveCoursePreRequisitesAPI")
			{
				$params=$SE->modifyParams($params,$SE->pre_req_course_map_id,"id");
				
			}
			else if($val=="CourseEquivalenceAPI")
			{
				$params=$SE->modifyParams($params,$SE->courseId,"id1");
				$params=$SE->modifyParams($params,$SE->pre_req_courseid,"id2");
			} 
			else if($val=="RemoveCourseEquivalenceAPI")
			{
				$params=$SE->modifyParams($params,$SE->equv_map_id,"id");
				
			}
			else if($val=="RegistrationAPI")
			{
				$params=$SE->modifyParams($params,$SE->courseId,"CourseId");
				$params=$SE->modifyParams($params,$SE->wbtClassId,"Classid");
				expDebug::dPrint("wbt class id for register".$SE->wbtClassId);
				$params=$SE->modifyParams($params,$SE->userId,"LearnerId");
			}
			else if($val=="ListEnrollmentsAPI")
			{
				$params=$SE->modifyParams($params,$SE->userId,"userid");
			}
			else if($val=="GetLaunchModulesAPI")
			{
				$params=$SE->modifyParams($params,$SE->enrollId,"enrollid");
				expDebug::dPrint("get launch modules::".$params);
			}
			
			else if($val=="UpdateScoreAPI")
			{
				$params=$SE->modifyParams($params,$SE->enrollId,"regid");
				$params=$SE->modifyParams($params,$SE->courseId,"courseid");
				$params=$SE->modifyParams($params,$SE->wbtClassId,"classid");
				$params=$SE->modifyParams($params,$SE->userId,"stid");
				$params=$SE->modifyParams($params,$SE->contentLessionId,"lessonid");
				
				
				expDebug::dPrint("get launch modules::".$params);
			}
			else  if($val=="CancelEnrollmentAPI")
			{
				$params=$SE->modifyParams($params,$SE->enrollId,"enrollId");
				$params=$SE->modifyParams($params,$SE->userId,"userid");
				expDebug::dPrint($SE->enrollId. " cancel enrollment ::".$params);
			}
			
			else if($val=="GetCourseDetailsAPI")
			{
				$params=$SE->modifyParams($params,$SE->courseId,"courseId");
				
			}
			else if($val=="GetClassDetailsAPI")
			{
				$params=$SE->modifyParams($params,$SE->wbtClassId,"classId");
				
			}
			
			else if($val=="ListRosterAPI")
			{
				$params=$SE->modifyParams($params,$SE->wbtClassId,"ClassId");
				$params=$SE->modifyParams($params,$SE->courseId,"CourseId");
				
			}
			/*else if($val=="UserSearchAPI")
			{
				$params=$SE->modifyParams($params,$SE->userName,"UserName");
				expDebug::dPrint("user search params....".$params);
			}*/
			
			
			$url = $SE->constructFullURL("simulator_invoker",$params);
			$res=$SE->HTTP_GET_REQUEST($url);
			
			
			expDebug::dPrint($res);
			if($val=="AddOrganizationAPI")
			{
				$SE->orgId=trim($SE->getValueFromXML($res,"result/Id"));
				expDebug::dPrint("org Id::".$SE->orgId);
			}
			else if($val=="UserCreationAPI")
			{
				$SE->userId=trim($SE->getValueFromXML($res,"result/Id"));
				$SE->userName=$SE->getParamValue($params,"UserName");//,"ClassId");
			}
			else if($val=="CreateCourseAPI")
			{
				$SE->courseId=trim($SE->getValueFromXML($res,"result/Id"));
				
				$params=$SE->modifyParams($params,"RESTAPI_PREREQC1".time(),"Code");
				$params=$SE->modifyParams($params,"RESTAPI_PREREQC1TITLE".time(),"Title");
				
				$url = $SE->constructFullURL("simulator_invoker",$params);
				$res=$SE->HTTP_GET_REQUEST($url);
				$SE->pre_req_courseid=trim($SE->getValueFromXML($res,"result/Id"));
			}
			else if($val=="CreateClassAPI")
			{
				$SE->wbtClassId=trim($SE->getValueFromXML($res,"result/Id"));
				expDebug::dPrint("CreateClassAPI wbtclassid ".$SE->wbtClassId);
				$params=$SE->modifyParams($params,"lrn_cls_dty_ilt","DeliveryTypeId");
				$params=$SE->modifyParams($params,"Dynamic_RESTAPI_ILT","Code");
				expDebug::dPrint("CreateClassAPI wbtclassid ".$SE->wbtClassId);
				$url = $SE->constructFullURL("simulator_invoker",$params);
				$res=$SE->HTTP_GET_REQUEST($url);
				$SE->iltClassId=trim($SE->getValueFromXML($res,"Id"));
				expDebug::dPrint("CreateClassAPI iltclassid ".$SE->iltClassId);
			
			
				
			}
			else if($val=="AssociateContentwithWBTClassAPI")
			{
				$SE->wbtSessionContentId=trim($SE->getValueFromXML($res,"result/Id"));
				expDebug::dPrint("wbt session id gen::".$SE->wbtSessionContentId);
			}
			else if($val=="AssociateFacilityLocationRoomAPI")
			{
				$SE->iltSessionContentId=trim($SE->getValueFromXML($res,"result/Id"));
				expDebug::dPrint("ilt session id gen::".$SE->iltSessionContentId);
			}
			else if($val=="CoursePreRequisitesAPI")
			{
				$SE->pre_req_course_map_id=trim($SE->getValueFromXML($res,"result/Id"));
				expDebug::dPrint("ilt session id gen::".$SE->pre_req_course_map_id);
			}
			else if($val=="CourseEquivalenceAPI")
			{
				$SE->equv_map_id=trim($SE->getValueFromXML($res,"result/Id"));
				expDebug::dPrint("ilt session id gen::".$SE->equv_map_id);
			}
			else if($val=="RegistrationAPI")
			{
				$SE->enrollId=trim($SE->getValueFromXML($res,"result/id"));
				expDebug::dPrint("generated enroll id::".$SE->enrollId);
			}
			else if($val=="GetLaunchModulesAPI")
			{
				$SE->contentLessionId=trim($SE->getValueFromXML($res,"result/ID"));
			}
			$res = htmlspecialchars($res);
			$teparams = $params."&xmlstring=".rawurlencode($res);
			$teurl = $SE->constructFullURL("test_engine",$teparams);
			$teres = $SE->HTTP_GET_REQUEST($teurl);
			$mailstr .= "<tr><td>$val</td><td>$teres</td><td>$res</td></tr>";
			$str .= "<tr><td>$val</td><td>$teres</td><td>$res</td></tr>";
			//CancelEnrollmentAPI
			
			
		}
	}
//}
	$mailstr .= '</table>';
$str .= '</table></div></body>';
$SE->sendMail2head($mailstr);

print $str;

?>