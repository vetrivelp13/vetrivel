<?php
require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solrclient.php';

class UserEnrollments extends SolrClient{

	function __construct(){
		// Get collection prefix from configuration
		$prefix = getConfigValue('collection_prefix');

		// Set exact collection name (should match with solr's collection)
		$this->collName = $prefix . '_' . SolrClient::EnrollCore;

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
	
/** Fetch all the enrollments of the user in USer Page section begins**/
	
	public function UserEnrollmentSolrSearch($userid,$keyword){
		$fieldList = array(
				'id:EnrollId',
				'title:Title',
				'code:Code',
				'status:CurrentStatusName',
				'delivery_type:TypeName',
				'reg_date:RegistrationDate',
				'comp_date:CompletionDate',
				'exempted_sts:ExemptedStatus'
		);

		// Setting required filters
		$filter = array();
		$this->setUserEnrollmentsAccessFilters($filter,$userid);
		$this->setUserEnrollmentStatusFilter($filter);
		if(!empty($keyword))
			$this->setUserEnrollmentsTextFilter($filter,$keyword);
		
		$results = $this->getUserEnrollmentDetails($filter,$fieldList,'GET');
		return $results;
	}
	private function setUserEnrollmentsAccessFilters(&$filter,&$userid){
		$str = 'UserId:'.$userid.' AND EnrType:(Class OR TP)';
		array_push($filter, urlencode($str));
	}
	private function setUserEnrollmentStatusFilter(&$filter){
		$enroll = '-CurrentStatus:lrn_tpm_ovr_exp';
		array_push($filter, urlencode($enroll));
	}
	private function setUserEnrollmentsTextFilter(&$filter,&$keyword){
		$str = 'SrchText:*'.$keyword.'*';
		array_push($filter, urlencode($str));
	}
	public function UserEnrollmentSolrSearchAutoComplete($userid){
		$fieldList = array('title:Title');
		$filter = array();
		$this->setUserEnrollmentsAccessFilters($filter,$userid);
		$this->setUserEnrollmentAutocompleteFilters($filter);
		$results = $this->getUserEnrollmentDetailsAutoComplete($filter,$fieldList,'GET');
		return $results;
	}
	private function setUserEnrollmentAutocompleteFilters(&$filter){
		$searchCurText =  str_replace('"', '\"', $_REQUEST['z']);
		if(!empty($searchCurText)) {
			if(strpos($searchCurText,"'") !== false){
				$type = explode("'",$searchCurText);
				foreach($type as $key=>$val){
					if($key==0)
						$textfilter = '(SrchText:"'.$val.'")';
					else
						$textfilter .= ' AND (SrchText:"'.$val.'")';
				}
			}
			else {
				$textfilter = 'TextSearch:"'.$searchCurText.'"';
			}
			array_push($filter, urlencode($textfilter));
		}
	}
	private function getUserEnrollmentDetailsAutoComplete ($filter,$fieldList,$method){
		$resBase = array('title' => NULL);	
		// Append field list
		$data = 'fl=' . implode(',',$fieldList);
		// Append filters
		$data .= '&fq=' . implode('&fq=',$filter);
	
		$data .= '&indent=on&q=*:*';
	
		$data .= '&rows=1000';
	
		try{
			$srcRst = $this->getData($this->collName,$data,'GET');
			$srcRst1 = $srcRst['response']['docs'];
			$srcRst2 = array();
			$srcRst2['recCount'] = $srcRst['response']['numFound'];
			foreach($srcRst1 as $doc){
				$tdoc  = array_merge($resBase, $doc);
				$this->modifyResultSet($tdoc);
				$srcRst2['records'][] = $this->arrayToObject($tdoc);
			}
			expDebug::dPrint("Solr search result - Final ".print_r($srcRst2,1),4);
			return $srcRst2;
		}catch(Exception $e){
			expDebug::dPrint("Error in user enrollment autocomplete -- ".$e->getMessage(),1);
		}
	}
	private function getUserEnrollmentDetails($filter,$fieldList,$method){
		// List of required fields to be set as NULL,
		// because Solr will not keep any null fields

		$baseList = array(
				'id'=>NULL,
				'title'=>NULL,
				'code'=>NULL,
				'Status'=>NULL,
				'delivery_type'=>NULL,
				'reg_date'=>NULL,
				'comp_date'=>NULL,
				'exempted_sts'=>NULL,		
		);
		// Append field list
		$data = 'fl=' . implode(',',$fieldList);

		// Append filters
		$data .= '&fq=' . implode('&fq=',$filter);
		//Pagination
		$rows = !empty($_REQUEST['rows']) ? $_REQUEST['rows'] : 5;
		$start = ($_REQUEST['page']*$_REQUEST['rows'])-$rows;
		$start = $start < 0 ? 0 : $start;
		$sidx = $_REQUEST['sidx'];
		$sord = $_REQUEST['sord'];
		 if(!empty($sidx)) {
			if($sidx == 'title') {
			$sort = 'TitleSrt+'.$sord;
			}
			else if($sidx == 'code') {
			$sort = 'CodeSrt+'.$sord;
			}
			else if($sidx == 'delivery_type') {
			$sort = 'TypeName+'.$sord;
			}
			else if($sidx == 'Status') {
			$sort = 'CurrentStatusName+'.$sord;
			}
		} else {
			$sort = 'TitleSrt+desc';
		}
		$data .= '&sort='.$sort.'&indent=on&q=*:*&start='.$start.'&rows='.$rows.'&facet=true&facet.field=CurrentStatusName';

		try{
			$srcRst = $this->getData($this->collName,$data,'GET');
			expDebug::dPrint('Solr Out put -- > '.print_r(($srcRst),true),4);				
			$srcRst1 = $srcRst['response']['docs'];
			$srcRst2 = array();
			$srcRst2['count'] = $srcRst['response']['numFound'];
			$srcRst3 = $srcRst['facet_counts']['facet_fields']['CurrentStatusName'];
			$srcRst2['statusCount'] = $this->getStatusCount($srcRst3);
			foreach($srcRst1 as $doc){
				$tdoc  = array_merge($baseList, $doc);
			//	$this->modifyResultSet($tdoc);
				$srcRst2['result'][] = $this->arrayToObject($tdoc);
			}			
			expDebug::dPrint("Solr search result - Final user enrollmentS---> ".print_r($srcRst2,1),4);
			return $srcRst2;
		}catch(Exception $e){
			expDebug::dPrint("Error in Solr User enrollment Search -- ".$e->getMessage(),1);
		}

	}
	/** Fetch all the enrollments of the user in USer Page section ends **/
	
	
	/** Fetch All Enrollments of TP -- TP enrollments roster page sections begins**/
	
	public function TPEnrollments($entityId,$entityType,$searchKeyword='',$from,$enrType){
		
			$fieldList = array(
				'id:EnrollId',
				'registered_user_id:UserId',
				'overall_status:CurrentStatus',
				'reg_date:RegisteredON',
				'cancel_date:CancelledON',
				'comp_date:CompletionDate',
				'updated_on:UpdatedOn',
				'user_name:Username',
				'full_name:FullName',
				'mandatory:MasterMandatory',
				'certifypath:EnrollmentPath',
				'program_id:CourseId',
				'module_title:ModuleTitle',
				'overall_status_name:CurrentStatusName',
				'date:CurrentStatus',
				'exempted_sts:ExemptedStatus',
				'exempted_overallstatus:ExemptedStatus'
		);
	
		$filter = array();
		$this->setEnrollmentFilter($filter,$entityId);

		if($_REQUEST['apiname']) {
			$this->setDateFilter($filter);
			$this->setUserNameFilter($filter);
		}
		$ddfault = array(t('LBL181'),t('LBL036')." ".t('LBL107'),t('LBL036')." ".t('LBL137'),t('LBL036')." ".t('LBL133')." ".t('LBL107'),t('LBL036')." ".t('LBL173')
				,t('LBL036')." ".t('LBL134'),t('LBL1270'),t('LBL036')." ".t('LBL102'),t('LBL036')." ".t('LBL3060'));
		$searchType = ($_REQUEST['searhType']) ? $_REQUEST['searhType'] : $_REQUEST['search_type'];
			if($searchKeyword && $searchKeyword!= '' && !in_array($searchKeyword, $ddfault)) {
				$searchType = ($_REQUEST['searhType']) ? $_REQUEST['searhType'] : $_REQUEST['search_type'];
				if(mb_stripos(t('Waived'),$searchKeyword,null,'UTF-8') !== false) {
					$text_filter = 'ExemptedStatus:*1*';
				} else {
					$text_filter = $this->setSearchFiler($searchKeyword);
				}
				
				if($from != 'autocomplete') {
					array_push($filter,urlencode($text_filter));
				} else {
					$result = $this->setFetchUserListCSN($filter,$searchType,$text_filter);
					return $result;
				}
					
			}

		$results = $this->getEnrollmentMyProgramDetails($filter,$fieldList,'GET',$from);
		return $results;
	}
	private function setEnrollmentFilter(&$filter,$entityId){
		// fetch master enrollments
		$fetch_master= 'BaseType:TP';
		array_push($filter, urlencode($fetch_master));
		// enrollments of program
		$entity = 'CourseId:'.$entityId.'';
		array_push($filter, urlencode($entity));
		// current module and Omit reserved canceled
		$cur_module = 'CurrentModule:Y AND -CurrentStatusName:ReservedCancelled';
		array_push($filter, urlencode($cur_module));
		
	}
	
	private function setSearchFiler($search){
		$searchType = ($_REQUEST['searhType']) ? $_REQUEST['searhType'] : $_REQUEST['search_type'];
		if($searchType == 'user'){
			$searchTypeVal = 'Username:"';
		}else if($searchType == 'fullname'){
			$searchTypeVal = 'FullName:"'; // usernamesearch
		}
		elseif($searchType == 'org'){
			$searchTypeVal = 'Organization:"'; // Dotted org
		}
		elseif($searchType == 'jobrole'){
			$searchTypeVal = 'JobRole:"';
		}
		elseif($searchType == 'usertype'){
			$searchTypeVal = 'UserType:"';
		}
		elseif($searchType == 'manager'){
			$searchTypeVal = 'UserManagerName:"';
		}elseif($searchType == 'group'){
			$searchTypeVal = 'LearnerGrpName:"';
		}
		if(strpos($searchTypeVal,"&") !== false){
			$val = explode('&',$searchTypeVal);
			$text = $val[0].':*'.$search.'* OR '.$val[1].':*'.$search.'*';
		}
		else {
			if($searchType == 'status') {
				if(mb_stripos(t('Waived'),$search,null,'UTF-8') !== false) {
					$text = 'ExemptedStatus:*1*';
				} else {
					$text = 'CurrentStatusName:*'.ucfirst($search).'*'; }
			}else if($searchType == 'path') {
				$text = 'ModuleTitle:*'.ucfirst($search).'*'; 
			} 
			else {
			//	$text = $searchTypeVal.':*'.$search.'*';
				if(strpos($search,"'") !== false){
					$type = explode("'",$search);
					foreach($type as $key=>$val){
						if($key==0)
							$text = $searchTypeVal.$val.'"';
						else
							$text .= ' AND '.$searchTypeVal.$val.'"';
					}
				}
				else
					$text = $searchTypeVal.$search.'"';
			}
		}
		return $text;
	}
	private function getEnrollmentMyProgramDetails($filter,$fieldList,$method,$from){
		// List of required fields to be set as NULL,
		// because Solr will not keep any null fields
			$baseList = array(
				'id'=>NULL,
				'registered_user_id'=>NULL,
				'overall_status'=>NULL,
				'reg_date'=>NULL,
				'cancel_date'=>NULL,
				'comp_date'=>NULL,
				'updated_on'=>NULL,
				'user_name'=>NULL,
				'full_name'=>NULL,
				'mandatory'=>NULL,
				'certifypath'=>NULL,
				'program_id'=>NULL,
				'module_title'=>NULL,
				'overall_status_name'=>NULL,
				'date'=>NULL,
				'exempted_sts'=>NULL,
				'exempted_overallstatus'=>NULL
			);
		/* if($countQry == true) {
			$data = 'fl=registered_user_id:UserId,overall_status:CurrentStatus,module_title:ModuleTitle';
			$data .= '&fq=BaseType:TP&fq=CourseId:'.$entityId.'&fq=CurrentModule:Y';
			$data .= '&indent=on&q=*:*&start=0&rows=1';
			$prgRec = $this->getData($this->collName,$data,'GET');
			$prgCnt = $prgRec['response']['numFound'];
		} */
		// Append field list
		$data = 'fl=' . implode(',',$fieldList);
	
		// Append filters
		$data .= '&fq=' . implode('&fq=',$filter);
		//Pagination
		if($from == 'autocomplete'){
			$rows = 1000;
			$start = 0;
		}else {
			$rows = 10;
			$rows = !empty($_REQUEST['rows']) ? $_REQUEST['rows'] : $rows;
			$start = ($_REQUEST['page']*$_REQUEST['rows'])-$rows;
			$start = $start < 0 ? 0 : $start;
		}
		$sidx = $_REQUEST['sidx'];
		$sord = $_REQUEST['sord'];
		if(!empty($sidx)) {
			if($sidx == 'user_name') {
				$sort = 'UsernameSrt+'.$sord;
			}
			if($sidx == 'full_name') {
				$sort = 'FullNameSrt+'.$sord;
			}
			else if($sidx == 'Date') {
				$sort = 'RegistrationStatusDate+'.$sord;
			}
			else if($sidx == 'Status') {
				$sort = 'CurrentStatusName+'.$sord;
			}
		} else {
			$sort = 'RegistrationStatusDate+desc';
		}
	    //Fullname,Username field throws error.So commented sort
		$data .= '&sort='.$sort.'&indent=on&q=*:*&start='.$start.'&rows='.$rows.'&facet=true&facet.field=CurrentStatusName';
		//$data .= '&indent=on&q=*:*&start='.$start.'&rows='.$rows.'&facet=true&facet.field=CurrentStatusName';
		try{
			$srcRst = $this->getData($this->collName,$data,'GET');
			$this->modifyResultSetForRoster($srcRst);
			$srcRst1 = $srcRst['response']['docs'];
			$srcRst2 = array();
			$srcRst2['count'] = $srcRst['response']['numFound'];
			$srcRst3 = $srcRst['facet_counts']['facet_fields']['CurrentStatusName'];
			$srcRst2['statusCount'] = $this->getStatusCount($srcRst3);
			foreach($srcRst1 as $doc){
				$tdoc  = array_merge($baseList, $doc);
				$this->modifyResultSetEnrollements($tdoc);
				$srcRst2['result'][] = $this->arrayToObject($tdoc);
			}
			expDebug::dPrint("Solr search result - Final tp enrollmens ".print_r($srcRst2,1),4);
			return $srcRst2;
		}catch(Exception $e){
			expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
		}
	}
	private function getUserDetailsOfEnrolled($filter,$fieldList,$method){
		$resBase = array(
				'User_id'=>NULL,
				'name'=>NULL
		);
		expDebug::dPrint("Sounn    filtererer autocommmm 7777777".print_r($fieldList,1),5);
		// Append field list
		$data = 'fl=' . implode(',',$fieldList);
		// Append filters
		$data .= '&fq=' . implode('&fq=',$filter);
		$rows = 1000;
		$start = 0;
		$data .= '&indent=on&q=*:*&start='.$start.'&rows='.$rows;
	
		try{
			$srcRst = $this->getData($this->collName,$data,$method);
			$srcRst1 = $srcRst['response']['docs'];
			$srcRst2 = array();
			$srcRst2['recCount'] = $srcRst['response']['numFound'];
			foreach($srcRst1 as $doc){
				$tdoc  = array_merge($resBase, $doc);
				$tdoc = $this->removeTilt($tdoc);
				$srcRst2['records'][] = $this->arrayToObject($tdoc);
			}
			expDebug::dPrint("tOTAL in Solr Catalog Search buncgh-- ".print_r($srcRst2,true),4);
			return $srcRst2;
		}catch(Exception $e){
			expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
		}
	}
	private function modifyResultSet(&$tdoc){
		// reset reg date
		$tdoc['reg_date'] = $this->modifyDate($tdoc['reg_date']);
		if($tdoc['comp_date'])
			$tdoc['comp_date'] = $this->modifyDate($tdoc['comp_date']);
	}
	private function modifyResultSetForRoster(&$srcRst){
		$obj = $this;
		if($this->responseType == 'json'){
			foreach($srcRst['response']['docs'] as $key=>&$doc){
				// To do
			}
		}else if($this->responseType == 'xml'){
			try{
			$obj = $this;
			$recs = $this->apiResponseHandler($srcRst,'class');
			$dom = new DOMDocument();
			$dom->loadXML($recs);
			$srcRst = $dom->saveXML();
			}catch(Exception $e){
				expDebug::dPrint("Error in parsing XML data -- ".print_r($e,true),1);
			}
		}

	}
	private function modifyResultSetEnrollements(&$tdoc){
		// reset reg date
		$tdoc['reg_date'] = $this->modifyDate($tdoc['reg_date']);
		// reset cancel date
		if($tdoc['cancel_date'])
			$tdoc['cancel_date'] = $this->modifyDate($tdoc['cancel_date']);
		// reset completed date
		if($tdoc['comp_date'])
			$tdoc['comp_date'] = $this->modifyDate($tdoc['comp_date']);
		// reset updated on date
		if($tdoc['updated_on'])
			$tdoc['updated_on'] = $this->modifyDate($tdoc['updated_on']);
		// reset date field
		$this->resetDate($tdoc);
	
	}
	private function resetDate(&$tdoc){
		$status = $tdoc['date'];
		if($status == 'lrn_tpm_ovr_enr')
			$date = $tdoc['reg_date'];
		else if($status == 'lrn_tpm_ovr_cln')
			$date = $tdoc['cancel_date'];
		else if($status == 'lrn_tpm_ovr_cmp')
			$date = $tdoc['comp_date'];
		else
			$date = $tdoc['updated_on'];
	
		$tdoc['date'] = $date;
	}
	
	private function modifyDate(&$date){
		$dt = $date;
		$dt = str_replace('T',' ',$dt);
		$dt = str_replace('Z','',$dt);
		expDebug::dPrint("Error in Solr Catalog Search -- ".$dt,1);
		return $dt;
	}
	
	/** Fetch All Enrollments of TP -- TP enrollments roster page sections ends**/
	
	
	/** Fetch All Enrollments of Class -- Class enrollments roster page sections begins**/
	
	public function classEnrollments($entityId,$entityType,$searchKeyword='',$from,$countQry){
		global $Solr_User;
			$fieldList = array(
				'id:EnrollId',
				'master_enr_id:MasterId',
				'delivery_type:Type',
				'registered_user_id:UserId',
				'full_name:FullName',
				'user_name:Username',
				'reg_status:RegistrationStatus',
				'comp_status:CompletionStatus',
				'status:CurrentStatusName',
				'score_id:Score',
				'reg_status_name:RegistrationStatusName',
				'comp_status_name:CompletionStatusName',
				'created_on:RegisteredOn',
				//	'updated_on:CompleteDays',
				'date:RegistrationStatusDate',
				'reg_status_date:RegistrationDate',
				'comp_date:CompletionDate',
				'waitlist_priority:WaitlistNo',
				'overall_status:EnrollmentPath',
				//	'uc_order_id:CurrentStatusName',
				'is_compliance:Compliance',
				'mandatory:Mandatory',
				'exempted_sts:ExemptedStatus',
				'exempted_overallstatus:ExemptedStatus',
				'sumEdit:1',
				'sumDelete:1',
		);
		// Setting required filters
		$filter = array();
		if(empty($Solr_User)){
			$userdetails = $this->getLoggedUserDetails();
		}else{
			$userdetails = $Solr_User;
		}
		$uid = $userdetails[0]->id;
		$manager_id = $userdetails[0]->manager_id;
		$ddfault = array(t('LBL181'),t('LBL036')." ".t('LBL107'),t('LBL036')." ".t('LBL137'),t('LBL036')." ".t('LBL133')." ".t('LBL107'),t('LBL036')." ".t('LBL173')
				,t('LBL036')." ".t('LBL134'),t('LBL1270'),t('LBL036')." ".t('LBL102'));
		$this->setClassEnrollmentAccessFilters($filter,$entityId);
		if($_REQUEST['apiname']) {
			$this->setDateFilter($filter);
		}
			if($searchKeyword && $searchKeyword!= '' && !in_array($searchKeyword, $ddfault)) {
				$searchType = ($_REQUEST['searhType']) ? $_REQUEST['searhType'] : $_REQUEST['search_type'];
				
				$text_filter = $this->setSearchFiler($searchKeyword);
				
				if($from != 'autocomplete') {
					array_push($filter,urlencode($text_filter));
				} else {
					$result = $this->setFetchUserListCSN($filter,$searchType,$text_filter);
					return $result;
				}
			} 
			$results = $this->getClassEnrollmentDetails($filter,$fieldList,'GET',$from);
		return $results;
		
	}
	
	private function setClassEnrollmentAccessFilters(&$filter,&$entityId){
		$str = 'ClassId:'.$entityId;
		array_push($filter, urlencode($str));
	}
	private function setDateFilter(&$filter){
		if ($_REQUEST ['date_from'] || $_REQUEST ['date_to']) {
			if ($_REQUEST ['date_from']) {
				$start_date = $_REQUEST ['date_from'];
				$start_date .= 'T00:00:00Z';
			}
			if ($_REQUEST ['date_from']) {
				$end_date = $_REQUEST ['date_to'];
				$end_date .= 'T00:00:00Z';
			}
			$date_range = '' . $start_date . ' TO ' . $end_date . '';
			$datefilter = 'RegistrationDate:[' . $date_range . ']';
			array_push ( $filter, urlencode ( $datefilter ) );
		}
			
	}
	private function setUserNameFilter(&$filter){
		if($_REQUEST['Username']) {
			$uname = $_REQUEST['Username'];
			$nameFilter = 'Username:*'.$uname.'*';
			array_push ( $filter, urlencode ( $nameFilter ) );
		}
	}
	
	private function setFetchUserListCSN(&$filter,$searchType,$text_filter){
		if($searchType == 'user'){
			$fieldList = array(
					'name:Username'
			);
		}
		if($searchType == 'fullname'){
			$fieldList = array(
					'name:FullName'
			);
		}
		if($searchType == 'org'){
			$fieldList = array(
					'name:Organization'
			);
		}
		if($searchType == 'jobrole'){
			$fieldList = array(
					'name:JobRole'
			);
		}
		if($searchType == 'usertype'){
			$fieldList = array(
					'name:UserType'
			);
		}
		if($searchType == 'manager'){
			$fieldList = array(
					'name:ManagerName'
			);
		}
		if($searchType == 'group'){
			$fieldList = array(
					'name:LearnerGrpName'
			);
		}if($searchType == 'status'){
			$fieldList = array(
					'name:CurrentStatusName'
			);
		}
		if($searchType == 'path'){
			$fieldList = array(
					'name:ModuleTitle'
			);
		}
		array_push($filter,urlencode($text_filter));
		$res = $this->getUserDetailsOfEnrolled($filter,$fieldList,'GET');
		return $res;
				
	}
	
	private function getClassEnrollmentDetails($filter,$fieldList,$method,$from){
		// List of required fields to be set as NULL,
		// because Solr will not keep any null fields
		
		$baseList = array(
				'id'=>NULL,
				'master_enr_id'=>NULL,
				'delivery_type'=>NULL,
				'registered_user_id'=>NULL,
				'full_name'=>NULL,
				'user_name'=>NULL,
				'status'=>NULL,
				'reg_status'=>NULL,
				'comp_status'=>NULL,
				'score_id'=>NULL,
				'date'=>NULL,
				'reg_status_name'=>NULL,
				'comp_status_name'=>NULL,
				'created_on'=>NULL,
				//	'updated_on:CompleteDays',
				'reg_status_date'=>NULL,
				'comp_date'=>NULL,
				'waitlist_priority'=>NULL,
				'overall_status'=>NULL,
				//	'uc_order_id:CurrentStatusName',
				'is_compliance'=>NULL,
				'mandatory'=>NULL,
				'exempted_sts'=>NULL,
				'exempted_overallstatus'=>NULL,
				'sumEdit'=>NULL,
				'sumDelete'=>NULL					
		);
		
		/* if($countQry == true) {
			expDebug::dPrint("set fetch user setFetchUserListCSN beeeee".$entityId, 5);
			$data = 'fl=registered_user_id:UserId,status:CurrentStatusName';
			$data .= '&fq=ClassId:'.$entityId;
			$data .= '&indent=on&q=*:*&start=0&rows=1';
			$cnrRec = $this->getData($this->collName,$data,'GET');
			$recCnt = $cnrRec['response']['numFound'];
			expDebug::dPrint("set fetch user setFetchUserListCSN countttt".$cnrRec, 5);
		} */
		// Append field list
		$data = 'fl=' . implode(',',$fieldList);
		// Append filters
		$data .= '&fq=' . implode('&fq=',$filter);
		//Pagination
		
		if($from == 'autocomplete'){
			$rows = 1000;
			$start = 0;
		}else {
			$rows = 10;
			$rows = !empty($_REQUEST['rows']) ? $_REQUEST['rows'] : $rows;
			$start = ($_REQUEST['page']*$_REQUEST['rows'])-$rows;
			$start = $start < 0 ? 0 : $start;
		}
		// Sort
		$sidx = $_REQUEST['sidx'];
		$sord = $_REQUEST['sord'];
		if(!empty($sidx)) {
			if($sidx == 'full_name') {
				$sort = 'FullNameSrt+'.$sord;
			}
			else if($sidx == 'user_name') {
				$sort = 'UsernameSrt+'.$sord;
			}
			else if($sidx == 'Date') {
				$sort = 'RegistrationStatusDate+'.$sord;
			}
			else if($sidx == 'Status') {
				$sort = 'CurrentStatusName+'.$sord;
			}
		} else {
			$sort = 'RegistrationStatusDate+desc';
		} 
 		//$data .= '&sort='.$sort.'&indent=on&q=*:*&start='.$start.'&rows='.$rows.'&facet=true&facet.field=CurrentStatusName';
		$data .= '&sort='.$sort.'&indent=on&q=*:*&start='.$start.'&rows='.$rows.'&facet=true&facet.field=CurrentStatusName';
		try{
			$srcRst = $this->getData($this->collName,$data,'GET');
			$this->modifyResultSetForRoster($srcRst);
			expDebug::dPrint('Solr Out put -- > '.print_r(($srcRst),true),4);	
			$srcRst1 = $srcRst['response']['docs'];	
			$srcRst2 = array();
			$srcRst2['count'] = $srcRst['response']['numFound'];
			$srcRst3 = $srcRst['facet_counts']['facet_fields']['CurrentStatusName'];
			$srcRst2['statusCount'] = $this->getStatusCount($srcRst3);

			foreach($srcRst1 as $doc){
				$tdoc  = array_merge($baseList, $doc);
				$this->resetScoreField($tdoc);
				expDebug::dPrint('Solr Out put tdocccc-- > '.print_r(($tdoc),true),4);
				$srcRst2['result'][] = $this->arrayToObject($tdoc);
			}
	
			expDebug::dPrint("Solr search result - Final class enrollment ".print_r($srcRst2,1),4);
			return $srcRst2;
		}catch(Exception $e){
			expDebug::dPrint("Error in Solr Class enrollment search -- ".$e->getMessage(),1);
		}
	}

	/** Fetch All Enrollments of Class -- Class enrollments roster page sections ends**/
  private function getStatusCount($srcRst) {
  	$srcRst4 = array();
  	for($i=0;$i<count($srcRst);$i++) {
  		if($srcRst[$i] == 'Enrolled')
  			$srcRst4[$srcRst[$i]] = $srcRst[$i+1];
  		else if($srcRst[$i] == 'Canceled')
  			$srcRst4[$srcRst[$i]] = $srcRst[$i+1];
  		else if($srcRst[$i] == 'Completed')
  			$srcRst4[$srcRst[$i]] = $srcRst[$i+1];
  		else if($srcRst[$i] == 'In progress')
  			$srcRst4[$srcRst[$i]] = $srcRst[$i+1];
  		else if($srcRst[$i] == 'Incomplete')
  			$srcRst4[$srcRst[$i]] = $srcRst[$i+1];
  		else if($srcRst[$i] == 'Pending')
  			$srcRst4[$srcRst[$i]] = $srcRst[$i+1];
  		else if($srcRst[$i] == 'Waitlist')
  			$srcRst4[$srcRst[$i]] = $srcRst[$i+1];
  		else if($srcRst[$i] == 'No Show')
  			$srcRst4[$srcRst[$i]] = $srcRst[$i+1];
  		else if($srcRst[$i] == 'Reserved')
  			$srcRst4[$srcRst[$i]] = $srcRst[$i+1];
  		else if($srcRst[$i] == 'Expired')
  			$srcRst4[$srcRst[$i]] = $srcRst[$i+1];
  		//	$srcRst3[$i] = $srcRst3[$i+1];
  	}
  	return $srcRst4;
  }
}
?>