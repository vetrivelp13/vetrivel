<?php
require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solrclient.php';

class MyLearningMyClassesSolrSearch extends SolrClient{

	function __construct(){
		// Get collection prefix from configuration
		$prefix = getConfigValue('collection_prefix');

		// Set exact collection name (should match with solr's collection)
		$this->collName = $prefix . '_' . SolrClient::CatalogCore;

		expDebug::dPrint("Core name -- ".$this->collName);

		//Call parent constructor
		parent::__construct();
	}
	public function getsolrUserDetails() {
		global $Solr_User;
		if(empty($Solr_User)){
			$userdetails = $this->getLoggedUserDetails();
		}else{
			$userdetails = $Solr_User;
		}
	
		return $userdetails;
	}
	public function myClassesSolrSearch(){
		expDebug::dPrint("Solr search is triggered2");
		$fieldList = array(
				'cls_id:EntityId',
				'ses_timezone:SessionTimezone',
				'title:CourseTitle',
				'CODE:Code',
				'cls_title:Title',
				'crs_code:Code',
				'description:Description',
				'delivery_type_code:Type',
				'courseid:CourseId',
				'classprice:Price',
				'LangCode:LangCode',
				'LocationId:LoctionId',
				'locationname:ClassLocationNames',
				'session_max_capacity:SessionMaxCapacity',
				'Language:LangName',
				'loc_timezone:LocationTimeZone',
				'LocationCity:LocationCity',
				'LocationZip:LocationZip',
				'LocationPhone:LocationPhone',
				'session_id:SessionId',
				'LocationAddr1:LocationAddr1',
				'LocationAddr2:LocationAddr2',
				'session_start_date:SessionStartDate',
		   		'session_start_time:SessionStartTime',
				'session_end_time:SessionEndTime',
				'session_title:SessionTitle',
				//'sess_type:SessionType',
				'session_start:SessionStart',
				'delivery_type:DeliveryTypeName',
				'basetype:BaseType',
				'LocationState:LocationStateName',
				'LocationCountry:LocationCountryName',
				'classStatus:ClassStatus',
				'MaxSessionDateTime:MaxSessionDateTime',
				'classstatus:ClassStatus'			
);

		// Setting required filters
		$filter = array();
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
			$uid = $_REQUEST['UserID'];
		} else {
			$solruserDet = $this->getSolrUserDetails();
			$uid = $solruserDet[0]->id;
		}
		$manager_id = $solruserDet[0]->manager_id;
		expDebug::dPrint('User details--->'.$uid,4);
		$this->setMyClassesAccessFilters($filter,$uid);
		$this->setMyClassesSearchFilters($filter);
		$this->setMyClassTextSearchFilter($filter);
		$this->setMyClassLocFilter($filter);
	    $this->setDeliveryTypeFilter($filter);
	    if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
	    	
	    	$this->setMyClassDateFilter($filter);
	    }
		$results = $this->getMyClassesDetails($filter,$fieldList,'GET');
		
	
		return $results;
	}
	
	public function myClassesSolrSearchAutoComplete(){
		$fieldList = array('cls_title:Title');
		$filter = array();
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname']) ){
			$uid = $_REQUEST['UserID'];
		} else {
			$solruserDet = $this->getSolrUserDetails();
			$uid = $solruserDet[0]->id;
		}
		$this->setMyClassesAccessFilters($filter,$uid);
		$this->setMyClassSearchAutocompleteFilters($filter);
		$results = $this->getMyClassDetailsAutoComplete($filter,$fieldList,'GET');
		return $results;
	}
	private function getMyClassDetailsAutoComplete($filter,$fieldList,$method){
		$resBase = array('cls_title' => NULL);
	
		// Append field list
		$data = 'fl=' . implode(',',$fieldList);
		// Append filters
		$data .= '&fq=' . implode('&fq=',$filter);
	
		$data .= '&indent=on&q=*:*';
	
		$data .= '&rows=1000';
	
		try{
			$srcRst = $this->getData($this->collName,$data,'GET');
			$this->modifyResultSet($srcRst);
			//$srcRst = drupal_json_decode($srcRst);
	
			$srcRst1 = $srcRst['response']['docs'];
			$srcRst2 = array();
			$srcRst2['recCount'] = $srcRst['response']['numFound'];
			foreach($srcRst1 as $doc){
				$tdoc  = array_merge($resBase, $doc);
				$srcRst2['records'][] = $this->arrayToObject($tdoc);
			}
	
			expDebug::dPrint("Solr search result - Final ".print_r($srcRst2,1),4);
			expDebug::dPrint("Solr search result - Final Count".print_r($srcRst['response']['numFound'],1),4);
			return $srcRst2;
		}catch(Exception $e){
			expDebug::dPrint("Error in My program Search -- ".$e->getMessage(),1);
		}
	}
	private function getMyClassesDetails($filter,$fieldList,$method){
		// List of required fields to be set as NULL,
		// because Solr will not keep any null fields
		
		$baseList = array(
				'cls_id'=>NULL,
				'ses_timezone'=>NULL,
				'title'=>NULL,
				'CODE'=>NULL,
				'cls_title'=>NULL,
				'description'=>NULL,
				'delivery_type_code'=>NULL,
				'courseid'=>NULL,
				'classprice'=>NULL,
				'LangCode'=>NULL,
				'LocationId'=>NULL,
				'LocationName'=>NULL,
				'session_max_capacity'=>NULL,
				'Language'=>NULL,
				'LocationCity'=>NULL,
				'LocationZip'=>NULL,
				'LocationPhone'=>NULL,
				'loc_timezone'=>NULL,
				'session_id'=>NULL,
				'LocationAddr1'=>NULL,
				'LocationAddr2'=>NULL,
				'session_start_date'=>NULL,
				'session_start_time'=>NULL,
				'session_end_time'=>NULL,
				'session_title'=>NULL,
				'sess_type'=>NULL,
				'session_start'=>NULL,
				'delivery_type'=>NULL,
				'basetype'=>NULL,
				'LocationState'=>NULL,
				'LocationCountry'=>NULL,	
				'classStatus'=>NULL,
				'classstatus'=>NULL,
				'MaxSessionDateTime'=>NULL				
		);
		// Append field list
		$data = 'fl=' . implode(',',$fieldList);

		// Append filters
		$data .= '&fq=' . implode('&fq=',$filter);
		
	//	$sortStr = '';
		// Sort
		if($_REQUEST['sortBy'] && $_REQUEST['sortBy'] != 'undefined'){
			if($_REQUEST['sortBy'] == 'AZ')
				$sort = 'TitleSrt+asc';
			else if($_REQUEST['sortBy'] == 'ZA')
				$sort = 'TitleSrt+desc';
			else if($_REQUEST['sortBy'] == 'dateOld')
				$sort = 'EntityId+desc';
			else if($_REQUEST['sortBy'] == 'dateNew')
				$sort = 'EntityId+asc';
			else if($_REQUEST['sortBy'] == 'type')
				$sort = 'DeliveryType+asc';
			 else if($_REQUEST['sortBy'] == 'orderbystatus')
				$sort = 'ClassStatus+desc'; 
			}
		else {
			$sort = 'TitleSrt+asc';
		}
		$rows = 10;
		//Pagination$rows
		$rows = !empty($_REQUEST['rows']) ? $_REQUEST['rows'] : $rows;
		$start = ($_REQUEST['page']*$_REQUEST['rows'])-$rows;
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
			$start = ($_REQUEST['page']*$_REQUEST['limit'])-$_REQUEST['limit'];
			$rows = (!empty($_REQUEST['limit'])) ? $_REQUEST['limit'] : $rows;
		}
		expDebug::dPrint('Sort by value is'.$sort,4);
		$data .= '&sort='.$sort.'&indent=on&q=*:*&start='.$start.'&rows='.$rows;


		try{
			$srcRst = $this->getData($this->collName,$data,'GET');

			//$srcRst = drupal_json_decode($srcRst);
			$this->modifyResultSet($srcRst);
			expDebug::dPrint('Solr Out put -- > '.print_r(($srcRst),true),4);
				
			$srcRst1 = $srcRst['response']['docs'];
			expDebug::dPrint("Solr result count ".print_r($srcRst['response']['numFound'],1),4);
			$srcRst2 = array();
			$srcRst2['count'] = $srcRst['response']['numFound'];
			foreach($srcRst1 as $doc){
				$tdoc  = array_merge($baseList, $doc);
				//$this->modifyResultSet($tdoc);
				$this->modifyResultSetMyClasses($tdoc);
				expDebug::dPrint("Solr result each doc ".print_r($tdoc,1),4);
				$srcRst2['result'][] = $this->arrayToObject($tdoc);
			}
				
			expDebug::dPrint("Solr search result - Final ".print_r($srcRst2,1),4);
			expDebug::dPrint("Solr search result - Final Count".print_r($srcRst['response']['numFound'],1),4);
			return $srcRst2;
		}catch(Exception $e){
			expDebug::dPrint("Error in Solr My classes Search -- ".$e->getMessage(),1);
		}

	}

	private function setMyClassesSearchFilters(&$filter){
		
			$regstatus  	= $_REQUEST['regstatuschk'];
		
		expDebug::dPrint("Solr search result - Final regstatuschkdssdsaaaaas ".print_r($_REQUEST['regstatuschk'],1),4);
		if(!empty($regstatus)) {
			$regstatus  	= stripApostrophe($regstatus);
			$statusArray = explode('|',$regstatus);
			expDebug::dPrint("Solr search result - Final regstatuschk ".print_r($statusArray,1),4);
			$i = 0;
			$afterCompleteAllowTime = getConfigValue('allow_meeting_launch_complete');
			$addSessionTime = ($afterCompleteAllowTime) ? $afterCompleteAllowTime : 30;
			$sts = '';
			if(isset($_REQUEST['apiname']) && $_REQUEST['apiname'] == 'ListInstructorClassesAPI') {
				if (in_array('scheduled_completed',$statusArray)){
					$sts .= "(MaxSessionDateTime:[NOW TO *])";
					$i++;
				}
				if (in_array('completed_scheduled', $statusArray)){
					$sts .= ($i == 0) ? '' : ' OR ';
					$sts .= "(MaxSessionDateTime:[* TO NOW])";
					$i++;
				}
			} else {
				if (in_array('scheduled',$statusArray)){
					$sts .= "(MaxSessionDateTime:[NOW TO *])";
					$i++;
				}
				if (in_array('delivered', $statusArray)){
					$sts .= ($i == 0) ? '' : ' OR ';
					$sts .= "(MaxSessionDateTime:[* TO NOW])";
					$i++;
				}
			}	
			$status = $sts;
			array_push($filter, urlencode($status));
		}
	}
	private function setMyClassSearchAutocompleteFilters(&$filter){
		$searchCurText =  str_replace('"', '\"', $_REQUEST['z']);
		if(strpos($searchCurText,"'") !== false){
			$type = explode("'",$searchCurText);
			foreach($type as $key=>$val){
				if($key==0)
					$textfilter = '(TextSearch:"'.$val.'")';
				else
					$textfilter .= ' AND (TextSearch:"'.$val.'")';
			}
		}
		else {
			$textfilter = 'TextSearch:"'.$searchCurText.'"';
		}
		array_push($filter, urlencode($textfilter));
	}
	private function setMyClassTextSearchFilter(&$filter){
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
			$textsearch = str_replace('"', '\"', $_REQUEST['searchtext']);
		} else {
			$textsearch = str_replace('"', '\"', $_REQUEST['searchText']);
		}
		if($textsearch){
			if(strpos($textsearch,"'") !== false){
			$type = explode("'",$textsearch);
			foreach($type as $key=>$val){
				if($key==0)
					$textfilter = '(TextSearch:"'.$val.'")';
				else
					$textfilter .= ' AND (TextSearch:"'.$val.'")';
			}
		}
		else {
			$textfilter = 'TextSearch:"'.$textsearch.'"';
		}
		array_push($filter, urlencode($textfilter));
		}
	}
	private function setMyClassLocFilter(&$filter){
		 if(!empty($_REQUEST['selectedLocID'])){
			$locFilter = 'LoctionId:'.$_REQUEST['selectedLocID'].'';
			array_push($filter, urlencode($locFilter));
		} 
		else if($_REQUEST['location']) {
			$location   = preg_replace("/[^a-zA-z]/", ' ', $_REQUEST['location']);
			$location = preg_replace('!\s+!', ' ', $location);
			$type = explode(' ',$location);
			$i = 0;
			foreach($type as $key=>$val){
				if($i == 0){
					$loc = 'LocationAddress:*'.$val.'*';
					$i++;
				}
				else {
					$loc .= ' AND LocationAddress:*'.$val.'*';
				}
			
			}
			array_push($filter, urlencode($loc));
		}
	}
	private function setDeliveryTypeFilter(&$filter){
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
			$_REQUEST['del_type'] = $_REQUEST['delivery_type'];
		}
		if($_REQUEST['del_type']){
			$deltype = explode('|',$_REQUEST['del_type']);
			foreach($deltype as $key=>$val){
				if($key==0)
					$deltypefilter = 'Type:(*'.trim($val).'*';
				else
					$deltypefilter .= ' OR *'.trim($val).'*';
			}
			$deltypefilter .= ')';
			array_push($filter, urlencode($deltypefilter));
		}
	}
	private function setMyClassDateFilter(&$filter){
		if ($_REQUEST ['startdate'] || $_REQUEST ['enddate']) {
			if ($_REQUEST ['startdate']) {
				$start_date = $_REQUEST ['startdate'];
				$start_date .= 'T00:00:00Z';
			} else
				$start_date = '*';
			if ($_REQUEST ['enddate']) {
				$end_date = $_REQUEST ['enddate'];
				$end_date .= 'T00:00:00Z';
			} else
				$end_date = '*';
			$date_range = '' . $start_date . ' TO ' . $end_date . '';
			$datefilter = 'SessionStartDate:[' . $date_range . ']';
			array_push ( $filter, urlencode ( $datefilter ) );
		}
	}
	private function setMyClassesAccessFilters(&$filter,&$uid){
		$grpstr = 'EntityTypeName:Class AND Type:(*lrn_cls_dty_ilt* OR *lrn_cls_dty_vcl*) AND InstructorId:'.$uid;
		array_push($filter, urlencode($grpstr));
		expDebug::dPrint('TP Registration level check'.print_r($filter,true),4);
	}

	private function modifyResultSet(&$srcRst){
		$obj = $this;
		if($this->responseType == 'json'){
			foreach($srcRst['response']['docs'] as $key=>&$doc){
				// To do
			}
		}else if($this->responseType == 'xml'){
			try{
			$obj = $this;
			$recs = $this->apiResponseHandler($srcRst,'mylearning');
			$dom = new DOMDocument();
			$dom->loadXML($recs);
			$srcRst = $dom->saveXML();
			}catch(Exception $e){
				expDebug::dPrint("Error in parsing XML data -- ".print_r($e,true),1);
			}
		}

	}
	private function modifyResultSetMyClasses(&$tdoc){
		$baseType = $tdoc['basetype'];
		$maxSession = $tdoc['MaxSessionDateTime'];
		$timeZone = date_default_timezone(false);
		$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
		$convertDateTime = timeZoneConvert(now(),$defaultTimezone,'UTC');
		$convertDateTime = date_format($convertDateTime,'Y-m-d H:i:s');
		$utcDateTime = date("Y-m-d H:i:s",strtotime($maxSession));
		expDebug::dPrint("converted now date time".$convertDateTime,4);
		expDebug::dPrint("converted session date time".$utcDateTime,4);
		
		if($baseType == 'ILT' || $baseType == 'VC') {
			if($utcDateTime > $convertDateTime){
				expDebug::dPrint("CDN111");
				$classSts = 'scheduled';
			} else {
				expDebug::dPrint("CDN222");
				$classSts = 'completed';
			}
			$tdoc['classStatus'] = $classSts;
			$tdoc['classstatus'] = $classSts;
			
		}

	}

	/* private function regEndDateCheck(&$doc) {
		$reg_end_date = str_replace('T',' ',$doc['SessionEndSearch']);
		$reg_end_date = str_replace('Z','',$reg_end_date);
		$doc['SessionEndSearch'] = $reg_end_date;
	} */

}
?>