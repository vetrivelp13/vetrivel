<?php
/*
 * ExpertusOne API
 * Rewrite Engine.php v1.0
 * 
 * @author: Rajkumar U
 * @date	:	02-Jan-2012
 * 
 * This is the rewrite layer of the Dynamic REST API.
 * 
 * DONOT MODIFY THIS FILE UNTIL IS ABSOLUTELY NECESSARY
 */
include_once('includes.php');

class RewriteEngine{

	function constructParamArray($paramTemplate,$params){
		$paramArray = array();
		$this->formatParamArray($paramTemplate,$params,$paramArray);
		return $paramArray;
	}
	
	function formatParamArray($paramTemplate,$params,&$paramArray){
		foreach($paramTemplate as $key => $val){
			if(is_array($val)){
				$this->formatParamArray($val,$params,$paramArray[$key]);
			} else {
				$paramArray[$key] = $params[$key];
			}
		}
	}
	
	function rewriteResult($resultSet,$dispCols,$len=0){
		// Result from Solr Search
		if($_REQUEST['SolrImpl'] === true){
			return $resultSet;
		}
		
		expDebug::dPrint("required length::".$len);
		if($dispCols != '' && !empty($resultSet))
		{
			$newResult = array();
			if(is_array($resultSet))
			{
// 				expDebug::dPrint(" function rewriteResult result set is array");	
				$record_cnt=1;
				foreach($resultSet as $key => $valtmp)
				{
					if($len==0 || $record_cnt<=$len)
					{
						$temparr = array();
						foreach($dispCols as $cid => $colName)
						{
								if(is_array($valtmp))
								{
// 									expDebug::dPrint("is array");
									$temparr[$cid] =$valtmp[$cid];
								}
								else
								{
// 									expDebug::dPrint("is not array");
									$temparr[$cid] =$valtmp->$cid;
								}
						}
						$newResult[] =  $temparr;
					}
					$record_cnt++;
				}
			
// 				expDebug::dPrint("new result set");
				expDebug::dPrint($newResult);
				if(empty($newResult)){
// 					expDebug::dPrint(" function rewriteResult formatted result set is  empty");
					return (array('0' => "There are some errors in the ORM, please verify."));
				} else {
// 					expDebug::dPrint(" function rewriteResult formatted result set is not empty");
					return $newResult;
				}
			} else {
				
					foreach($resultSet as $key => $valtmp)
					{
						foreach($dispCols as $cid => $colName)
						{
// 							expDebug::dPrint("new test");
// 							expDebug::dPrint($key);
// 							expDebug::dPrint($valtmp);
							
							if(strtolower(trim($key))==strtolower(trim($cid)))
							{
								$temparr[$key] =$valtmp;
							}
						}
					}
					$newResult[] =  $temparr;
// 				expDebug::dPrint(" function rewriteResult result set is not array");	
				return $newResult;
			}
		} else {
// 			expDebug::dPrint(" function rewriteResult result set is empty");
			return $resultSet;
		}
	}
	
	function rewriteSolrResult($resultSet){
		if($this->getReturnType() == 'json') {
			return '{"results":{"jsonResponse":'.json_encode($resultSet['docs']).',"totalrecords":'.$resultSet['numFound'].'}}';
		}else{
			return $resultSet;
		}
		
	}
	function rewriteResult1($resultSet,$dispCols){
		if($dispCols != '' && !empty($resultSet)){
			$newResult = array();
// 			expDebug::dPrint(" function rewriteResult result set not empty");
			if(!is_array($resultSet))
			{
				$resultSet=object2array($resultSet);
// 				expDebug::dPrint("object converted to array");
// 				expDebug::dPrint($resultSet);
			}
			
			if(is_array($resultSet)){
// 			expDebug::dPrint(" function rewriteResult result set is array");	
				foreach($resultSet as $key => $valtmp){
					$temparr = array();
					
// 					expDebug::dPrint(" function rewriteResult result set iteration key ".$key);
					
// 					expDebug::dPrint("displ col");
					
					if(!is_int($key) && !is_array($valtmp))
					{	
						foreach($dispCols as $cid => $colName)
						{
// 							expDebug::dPrint($key);
// 							expDebug::dPrint($cid);
							if(strtolower($key)==strtolower($cid))
							{
								$temparr[$cid] =$valtmp;
								
							}
						}
						if(!empty($temparr))
							$newResult[] =  $temparr;
					}
					else
					{
						foreach($dispCols as $cid => $colName){
							/*foreach($valtmp as $a => $b){
						    expDebug::dPrint("test");
							expDebug::dPrint($a);
							expDebug::dPrint($b);	
							}*/
							if(is_array($valtmp))
							{
// 								expDebug::dPrint("is array");
								$temparr[$cid] =$valtmp[$cid];
							}
							else
							{
// 								expDebug::dPrint("is not array");
								$temparr[$cid] =$valtmp->$cid;
							}
						}
						$newResult[] =  $temparr;
						
					}
					
				}
// 				expDebug::dPrint("new result set");
				expDebug::dPrint($newResult);
				if(empty($newResult)){
// 					expDebug::dPrint(" function rewriteResult formatted result set is  empty");
					return (array('0' => "There are some errors in the ORM, please verify."));
				} else {
// 					expDebug::dPrint(" function rewriteResult formatted result set is not empty");
					return $newResult;
				}
			} else {
// 				expDebug::dPrint(" function rewriteResult result set is not array");	
				return $resultSet;
			}
		} else {
// 			expDebug::dPrint(" function rewriteResult result set is empty");
			return $resultSet;
		}
	}
	
	
	
	function object2array($object) {
		$arr=array();
	    if (is_object($object)) {
	        foreach ($object as $key => $value) {
	        	//expDebug::dPrint("object2array key===". $key."==". $value);
	            $arr[$key] = $value;
	        }
	    }
	    else {
	        $arr = $object;
	    }
	    return $arr;
	}

	function constructFullXML($data,$totalRecords){
		//expDebug::dPrint("xml data");
		//expDebug::dPrint($data);
		
		$string = '<?xml version="1.0" encoding="UTF-8" ?>';
		$string .= '<results totalrecords = "'.$totalRecords.'">';
		$string .= $this->constructXML($data);
		$string .= '</results>';
		
		return $string;
	}
	
	function constructXML($data){
// 		expDebug::dPrint("construct xml");
// 		expDebug::dPrint($data);
		$string = '';
		if(is_array($data)){
			foreach($data as $key => $val){
				if(is_numeric($key)){
					$string .= '<result>';
				} else {
					$string .= '<'.$key.'>';
				}
				if(is_array($val)){
					$string .= $this->constructXML($val);
				} else {
					if( $val instanceof stdClass)
					{
						$arr=$this->object2array($val);
						$string .= $this->constructXML($arr);
					}
					else
					{
						$string .= $val;
					}
				}
				if(is_numeric($key)){
					$string .= '</result>';
				} else {
					$string .= '</'.$key.'>';
				}
			}
			return $string;
		} else {
			if($data instanceof stdClass)
			{
				$arr=$this->object2array($data);
				return $this->constructXML($arr);
			}
			
		}
	}
	
	function getJSONData($jsonstring,$totalRecords=''){
	//	$jsonstring = stripslashes($jsonstring);
		return '{"results":{"jsonResponse":'.$jsonstring.',"totalrecords" : "'.$totalRecords.'"}}';
	}
	
	function getDisplayColumns($apiname){
		$CE = new CoreEngine();
		$CE->prepareDetals($apiname);
		$dispcols = '';
		if(isset($_GET['display_cols']) && $_GET['display_cols'] != ''){
			$dispcols = $_GET['display_cols'];
		} else if(isset($_POST['display_cols']) && $_POST['display_cols'] != ''){
			$dispcols = $_POST['display_cols'];
		} else {
			$dispcols = $CE->getRespCols();
		}
		if(is_array($dispcols)){
			return $dispcols;
		} else {
			$disparray = explode(',',$dispcols);
			$newdisparray = array();
			foreach($disparray as $key => $val){
				$newdisparray[$val] = ' ';
			}
			return $newdisparray;
		}
	}
	
	function getReturnType(){
		$returntype = '';
		if(isset($_GET['returntype'])){
			$returntype = $_GET['returntype'];
		} else if(isset($_POST['returntype'])){
			$returntype = $_POST['returntype'];
		} else {
			$returntype = '';
		}
		return $returntype;
	}
	
	function getFormattedOutput($rs){
// 		expDebug::dPrint("construct xml".$this->getReturnType());
// 		expDebug::dPrint($rs);
		if($_REQUEST['SolrImpl'] === true){
			return $this->rewriteSolrResult($rs);
		}
		$returnstring = '';
		$totalRecords = $rs['totalrecords'];
		unset($rs['totalrecords']);
		if($this->getReturnType() == 'xml'){
			$returnstring = $this->constructFullXML($rs,$totalRecords);
		} else if($this->getReturnType() == 'json') {
			$returnstring = $this->getJSONData(json_encode($rs),$totalRecords);
		}
		return $returnstring;
	}
	
	function constructFullXMLDoc($data){
		$string = htmlspecialchars('<?xml version="1.0" encoding="UTF-8" ?>');
		$string .= "\n";
		$string .= htmlspecialchars('<results>');
		$string .= "\n";
		$string .= $this->constructXMLDoc($data);
		$string .= htmlspecialchars('</results>');
		
		return $string;
	}
	
	function constructXMLDoc($data){
		$string = '';
		foreach($data as $key => $val){
			if(is_numeric($key)){
				$string .= htmlspecialchars(' <result>');
			} else {
				$string .= htmlspecialchars('  <'.$key.'>');
			}
			if(is_array($val)){
				$string .= "\n";
				$string .= $this->constructXMLDoc($val);
			} else {
				$string .= $val;
			}
			if(is_numeric($key)){
				$string .= htmlspecialchars(' </result>');
				$string .= "\n";
			} else {
				$string .= htmlspecialchars('</'.$key.'>');
				$string .= "\n";
			}
		}
		return $string;
	}
	
	function getAPIName(){
		$apiname = '';
		if(isset($_GET['apiname'])){
			$apiname = $_GET['apiname'];
		} else if(isset($_POST['apiname'])){
			$apiname = $_POST['apiname'];
		} else {
			$apiname = '';
		}
		return $apiname;
	}
}
?>