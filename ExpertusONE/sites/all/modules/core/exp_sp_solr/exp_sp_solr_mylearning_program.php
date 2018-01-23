<?php
require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solrclient.php';

class MyLearningProgramSolrSearch extends SolrClient{

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
	public function ProgramSolrSearch(){
		expDebug::dPrint("Solr search is triggered2");
		$fieldList = array(
				'id:EnrollId',
				'order_id:OrderID',
				'program_id:CourseId',
				'user_id:UserId',
				'overall_status:CurrentStatus',
			//	'pre_status:',//
				'mandatory:Mandatory',
				'reg_date:RegistrationDate',
				'recertify_path:EnrollmentPath',
				'is_current:CurrentModule',
				'cancel_date:CancelledON',
				'comp_date:CompletionDate',
				'score_id:Score',
				'pre_score:PreAssScore',
				'grade:Grade',
				'percentage_complete:CompletionPercentage',
				'overall_progress:Progress',
				'content_status:ContentStatus',
		//		'browser_session:',//
				'created_by:RegisteredBy',
				'created_on:RegisteredOn',
				'updated_by:UpdatedBy',
				'updated_on:UpdatedOn',
		//		'custom0:',//
		//		'custom1:',//
		//		'custom2:',//
		//		'custom3:',//
		//		'custom4:',//
				'custom_dataload:DataloadCustom',
				'dataload_by:DataloadBy',
				'custom_status:CustomStatus',
				'launchable:Launchable',
				'programid:CourseId',
				'basetype:BaseType',
				'prg_title:Title',
				'prg_code:Code',
				'prg_desc:Description',
				'prg_shortdesc:Description',
				'prg_fulldescription:Description',
				'prg_start_date:StartDate',
				'prg_end_date:EndDate',
				'prg_status:Status',
				'prg_object_type:Type',
				'prg_expires_in_value:ValidityDays',
				'prg_expires_in_unit:ValidityType',
				'master_mandatory:MasterMandatory',
				'masterenroll_overall_status:CurrentStatus',
				'node_id:NodeId',
				'attr2:Attr2',
				'due_date:DueDate',
				'mro:MRO',
				'ascen:MROValue',
				'exempted_sts:ExemptedStatus',
				'exempted_sts_2:ExemptedStatus'
		);

		// Setting required filters
		$filter = array();
		$solruserDet = $this->getSolrUserDetails();
		$uid = $solruserDet[0]->id;
		$manager_id = $solruserDet[0]->manager_id;
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
			$uid = $_REQUEST['UserID'];
		}
		expDebug::dPrint('User details--->'.print_r($solruserDet,true),4);
		$this->setProgramAccessFilters($filter,$uid);
		$this->setSearchFilters($filter,$uid,$manager_id);
			
		$results = $this->getMyProgramDetails($filter,$fieldList,'GET');
		
		expDebug::dPrint("sesssion checkkkk".print_r($prgSts,1),4);
		return $results;
	}
	
	public function myProgramSolrSearchAutoComplete(){
		$fieldList = array('prg_title:Title');
		$filter = array();
		$solruserDet = $this->getSolrUserDetails();
		$uid = $solruserDet[0]->id;
		$this->setProgramAccessFilters($filter,$uid);
		$this->setSearchAutocompleteFilters($filter);
		$results = $this->getMyProgramDetailsAutoComplete($filter,$fieldList,'GET');			
		return $results;
	}
	private function getMyProgramDetailsAutoComplete ($filter,$fieldList,$method){
		$resBase = array('prg_title' => NULL);
	
		// Append field list
		$data = 'fl=' . implode(',',$fieldList);
		// Append filters
		$data .= '&fq=' . implode('&fq=',$filter);
	
		$data .= '&indent=on&q=*:*';
	
		$data .= '&rows=1000';
	
		try{
			$srcRst = $this->getData($this->collName,$data,'GET');
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
	private function getMyProgramDetails($filter,$fieldList,$method){
		// List of required fields to be set as NULL,
		// because Solr will not keep any null fields
		$prgSts = getStatusFilters('myprograms');
		$sortbyStatusArray = explode("|",$prgSts);
		expDebug::dPrint("session value check---->".print_r($sortbyStatusArray,1),4);
		
		$baseList = array(
				'id'=>NULL,
				'order_id'=>NULL,
				'program_id'=>NULL,
				'user_id'=>NULL,
				'overall_status'=>NULL,
				'pre_status'=>NULL,
				'mandatory'=>NULL,
				'reg_date'=>NULL,
				'recertify_path'=>NULL,
				'is_current'=>NULL,
				'cancel_date'=>NULL,
				'comp_date'=>NULL,
				'score_id'=>NULL,
				'pre_score'=>NULL,
				'grade'=>NULL,
				'percentage_complete'=>NULL,
				'overall_progress'=>NULL,
				'content_status'=>NULL,
				'browser_session'=>NULL,
				'created_by'=>NULL,
				'created_on'=>NULL,
				'updated_by'=>NULL,
				'updated_on'=>NULL,
				'custom0'=>NULL,
				'custom1'=>NULL,
				'custom2'=>NULL,
				'custom3'=>NULL,
				'custom4'=>NULL,
				'custom_dataload'=>NULL,
				'dataload_by'=>NULL,
				'custom_status'=>NULL,
				'launchable'=>NULL,
				'programid'=>NULL,
				'basetype'=>NULL,
				'prg_title'=>NULL,
				'prg_code'=>NULL,
				'prg_desc'=>NULL,
				'prg_shortdesc'=>NULL,
				'prg_start_date'=>NULL,
				'prg_end_date'=>NULL,
				'prg_status'=>NULL,
				'prg_object_type'=>NULL,
				'prg_expires_in_value'=>NULL,
				'prg_expires_in_unit'=>NULL,
				'master_mandatory'=>NULL,
				'masterenroll_overall_status'=>NULL,
				'node_id'=>NULL,
				'attr2'=>NULL,
				'due_date'=>NULL,
				'mro'=>NULL,
				'ascen'=>NULL,
				'exempted_sts'=>NULL,
				'exempted_sts_2'=>NULL,
				'prg_fulldescription'=>NULL
						
		);
		// Append field list
		$data = 'fl=' . implode(',',$fieldList);

		// Append filters
		$data .= '&fq=' . implode('&fq=',$filter);
		//Pagination
		$rows = !empty($_REQUEST['rows']) ? $_REQUEST['rows'] : 10;
		$start = ($_REQUEST['page']*$_REQUEST['rows'])-$rows;
		$start = ($start < 0 ) ? 0 : $start;
		if($_REQUEST['sortBy'] && $_REQUEST['sortBy'] != 'undefined'){
			if($_REQUEST['sortBy'] == 'AZ')
				$sort = 'TitleSrt+asc';
			else if($_REQUEST['sortBy'] == 'ZA')
				$sort = 'TitleSrt+desc';
			else if($_REQUEST['sortBy'] == 'dateNew')
				$sort = 'EnrollId+asc';
			else if($_REQUEST['sortBy'] == 'dateOld')
				$sort = 'EnrollId+desc';
			else if($_REQUEST['sortBy'] == 'Mandatory')
				$sort = 'MRO+asc';
			else if($_REQUEST['sortBy'] == 'type')
				$sort = 'Type+asc';
			else if($_REQUEST['sortBy'] == 'orderbystatus') { 
				$sort = 'CurrentStatus+desc';
			}
		}
		else {
			$sort = 'TitleSrt+asc';
		}
		expDebug::dPrint('Sort by value is'.$sort,1);
		$data .= '&sort='.$sort.'&indent=on&q=*:*&start='.$start.'&rows='.$rows;


		try{
			$srcRst = $this->getData($this->collName,$data,'GET');

			//$srcRst = drupal_json_decode($srcRst);
				
			expDebug::dPrint('Solr Out put -- > '.print_r(($srcRst),true),4);
				
			$srcRst1 = $srcRst['response']['docs'];
			expDebug::dPrint("Solr result count ".print_r($srcRst['response']['numFound'],1),1);
			$srcRst2 = array();
			$srcRst2['count'] = $srcRst['response']['numFound'];
			foreach($srcRst1 as $doc){
				$tdoc  = array_merge($baseList, $doc);
				$this->resetScoreField($tdoc);
				expDebug::dPrint("Solr result each doc ".print_r($tdoc,1),1);
				$srcRst2['result'][] = $this->arrayToObject($tdoc);
			}
				
			expDebug::dPrint("Solr search result - Final ".print_r($srcRst2,1),4);
			expDebug::dPrint("Solr search result - Final Count".print_r($srcRst['response']['numFound'],1),4);
			return $srcRst2;
		}catch(Exception $e){
			expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
		}

	}

	private function setSearchFilters(&$filter,&$uid,&$manager_id){
		// First only Enrolled courses should be displayed
		$regstatus  	= $_REQUEST['regstatuschk'];
		if(!empty($regstatus)) {
		$regstatus  	= stripApostrophe($regstatus);
		$statusArray = explode('|',$regstatus);
		$i = 0;
		$prgSts = getStatusFilters('myprograms');
		$sortbyStatusArray = explode("|",$prgSts);
		expDebug::dPrint('TP Registration status check::'.print_r($sortbyStatusArray,1),4);
		$sts = '';
		if (in_array('lrn_tpm_ovr_cmp_atl',$statusArray)){
			expDebug::dPrint('Status filter check1');
			$sts .= '(CurrentStatus:lrn_tpm_ovr_cmp AND Launchable:1)';  
			$i++;
		}
		// For completed and need reregister filter
		$config = getConfig("exp_sp");
		$cert_days_expire = isset($config["cert_expire_reminder_days"]) ? $config["cert_expire_reminder_days"] : 10;
	
		expDebug::dPrint('cert expire day check::'.$cert_days_expire,4);
		if (in_array('lrn_tpm_ovr_cmp_rereg', $statusArray)){ 
			$sts .= ($i == 0) ? '' : ' OR ';
			$reReg = 'ExpDate:[NOW-'.$cert_days_expire.'DAY TO NOW+'.$cert_days_expire.'DAY]';
			expDebug::dPrint('Completed need reregister check-->'.$reReg,4);
			$sts .= "(CurrentStatus:lrn_tpm_ovr_exp ) OR ( $reReg AND CurrentStatus:lrn_tpm_ovr_cmp)";
			$i++;
		}
		if((in_array('lrn_tpm_ovr_enr', $statusArray))||(in_array('lrn_tpm_ovr_inp', $statusArray))||(in_array('lrn_tpm_ovr_cmp', $statusArray))
				||(in_array('lrn_tpm_ovr_cln', $statusArray))||(in_array('lrn_tpm_ovr_inc', $statusArray))||(in_array('lrn_tpm_ovr_ppm', $statusArray))||
				(in_array('lrn_tpm_ovr_ppv', $statusArray))||(in_array('lrn_tpm_ovr_wtl', $statusArray))){
			if (($key = array_search('lrn_tpm_ovr_cmp_atl', $statusArray)) !== false) {
				unset($statusArray[$key]);
			}
			if (($key = array_search('lrn_tpm_ovr_cmp_rereg', $statusArray)) !== false) {
				unset($statusArray[$key]);
			}
			foreach($statusArray as $key=>$val){
			$sts .= ($i == 0) ? '' : ' OR ';
				expDebug::dPrint('Status filter check22222222 ketttt::'.$key,4);
				$sts .= 'CurrentStatus:*'.trim($val).'*';
				$i++;
			}
		}
		$status = $sts;
		array_push($filter, urlencode($status));
		expDebug::dPrint('Registration status array check after::'.print_r($filter,true),4);
		}		
		$this->setPriceFilter($filter);
		$this->setScheduleFilter($filter);
		$this->setDueFilter($filter);
		$this->setRegDateFilter($filter);
		$this->setAssignedByFilter($filter,$uid,$manager_id);
		$this->setLocFilter($filter);
		$this->setTPtypeFilter($filter);
		$this->setDelTypeFilter($filter);
		$this->setTextFilter($filter);
	}
	private function setPriceFilter(&$filter){
		if($_REQUEST['price']){
			if($_REQUEST['price'] == 'priced')
				$pricefilter = '-Price:0.0';
			else
				$pricefilter = 'Price:0.0';
			array_push($filter, urlencode($pricefilter));
		}
	}

	private function setScheduleFilter(&$filter){
		if($_REQUEST['scheduled']){
			$scheduleddate = 'SessionStartDate:[NOW TO NOW+'.$_REQUEST['scheduled'].'DAYS]';
			expDebug::dPrint('Scheduled date filter'.print_r($scheduleddate,true),4);
			array_push($filter, urlencode($scheduleddate));
		}
	}

	private function setDueFilter(&$filter){
		if($_REQUEST['due']){
			$duedatefilter = 'DueDate:[NOW TO NOW+'.$_REQUEST['due'].'DAYS]';
			expDebug::dPrint('Due date filter'.print_r($duedatefilter,true),4);
			array_push($filter, urlencode($duedatefilter));
		}
	}

	private function setRegDateFilter(&$filter){
		if($_REQUEST['reg']){
			$regdatefilter = 'RegistrationDate:[NOW-'.$_REQUEST['reg'].'DAYS TO NOW]';
			expDebug::dPrint('Reg date filter'.print_r($regdatefilter,true),4);
			array_push($filter, urlencode($regdatefilter));
		}
	}

	private function setAssignedByFilter(&$filter,&$uid,&$manager_id){
	if(!empty($_REQUEST['assignedby'])){
			if($_REQUEST['assignedby'] == 'cre_sys_fop_me') {
				$assignedby = 'RegisteredBy:'.$uid.'';
			}
			else if($_REQUEST['assignedby'] == 'cre_sys_fop_mgr') {
				$assignedby = 'RegisteredBy:'.$manager_id.'';
			}
			else {
				$assignedby = '-RegisteredBy:'.$uid;
				if(!empty($manager_id))
					$assignedby .= ' AND -RegisteredBy:'.$manager_id;
			}
			array_push($filter, urlencode($assignedby));
		}
	}
	 
	private function setDelTypeFilter(&$filter){
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

	private function setTPtypeFilter(&$filter){
		if($_REQUEST['tra_type'] && !empty($_REQUEST['tra_type'])) {
			$trainingType = stripRegStatus(stripApostrophe($_REQUEST['tra_type']));
			$mrovalue= explode('|',$trainingType);
			foreach($mrovalue as $key=>$val){
				if($key==0)
					$trainingfilter = 'MRO:*'.$val.'*';
				else
					$trainingfilter .= ' OR MRO:*'.$val.'*';
			}
			array_push($filter, urlencode($trainingfilter));
			expDebug::dPrint('Training filter value'.print_r($trainingfilter,true),4);
		}
	}

	private function setLocFilter(&$filter){
		if($_REQUEST['selectedLocID']){
			$locFilter = 'LocationId:'.$_REQUEST['selectedLocID'].'';
		}
		else if($_REQUEST['location']) {
			if(isset($_REQUEST['mobile_request'])){
				$location = str_replace('~',' ',$_REQUEST['location']);
			}
			else {
				$location = $_REQUEST['location'];
				$location   = preg_replace("/[^a-zA-z]/", ' ', $location);
				$location = preg_replace('!\s+!', ' ', $location);
			}
			$text = explode(' ',$location);
			foreach($text as $key=>$val){
				if($key==0)
					$locFilter = 'LocationAddress:*'.$val.'*';
				else
					$locFilter .= ' AND LocationAddress:*'.$val.'*';
			}
		}
		if($locFilter)
			array_push($filter, urlencode($locFilter));
	}

	private function setTextFilter(&$filter){
		$textsearch = $_REQUEST['searchText'];
	    if(!empty($textsearch)) {
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
	private function setSearchAutocompleteFilters(&$filter){
		$searchCurText =  str_replace('"', '\"', $_REQUEST['z']);
		if(!empty($searchCurText)) {
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
	}

	private function setProgramAccessFilters(&$filter,&$uid){
		$grpstr = 'UserId:'.$uid.' AND BaseType:TP';
		array_push($filter, urlencode($grpstr));
		expDebug::dPrint('TP Registration level check'.print_r($filter,true),4);
	}

	
	
	
}
?>