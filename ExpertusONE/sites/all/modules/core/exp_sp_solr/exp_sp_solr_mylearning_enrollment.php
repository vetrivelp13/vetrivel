<?php
require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solrclient.php';

class MyLearningEnrollmentSolrSearch extends SolrClient{

	function __construct(){
		// Get collection prefix from configuration
		$prefix = getConfigValue('collection_prefix');

		// Set exact collection name (should match with solr's collection)
		$this->collName = $prefix . '_' . SolrClient::EnrollCore;

		expDebug::dPrint("Core name -- ".$this->collName);

		//Call parent constructor
		parent::__construct();
	}

/** My Enrollment In My Learning Page starts here **/
	public function EnrollmentSolrSearch(){
		global $Solr_User;
		expDebug::dPrint("Solr search is triggered2");
		$fieldList = array(
			'id:EnrollId',
			'launchable:Launchable',
			'user_id:UserId',
			'master_enrollment_id:MasterId',
			'class_id:ClassId',
			'course_id:CourseId',
			'check_entity_id:ClassGroupMap',
			'check_entity_type:ClassGroupMapType',
			'mandatory:Mandatory',
			'is_compliance:Compliance',
			'compliance_complete_date:CompleteDate',
			'compliance_complate_days:CompleteDays',
			'compliance_validity_date:ValidityDate',
			'compliance_validity_days:ValidityDays',
			'compliance_completed:ComplianceCompleted',
			'recertify_path:EnrollmentPath',
			'reg_status:CurrentStatusName',
			'reg_status_code:CurrentStatus',
			'status_code:RegistrationStatus',
			'reg_date:RegDate',
			'comp_status:CompletionStatus',
			'comp_date:CompDate',
			'update_date:UpdatedOn',
			'reg_status_date:RegistrationStatusDate',
			'valid_from:ValidFrom',
			'valid_to:ValidTo',
			'score_id:Score',  // called modify result set for 0085112
			'grade:Grade',
			'progress:Progress',
			'updated_on:UpdatedOn',
			'updated_by:CompeteBy',
			'created_by:RegisteredBy',
			'waitlist_priority:WaitlistNo',
			'preassesment_completion_status:PreAssessCompStatus',
			'title:CourseTitle',
			'crs_code:CourseCode',
			'cls_title:Title',
			'code:Code',
			'cls_status:Status',
			'description:Description',
			'delivery_type_code:Type',
			'dedicated_class_flag:DedicatedClass',
			'delivery_type:TypeName',
			'basetype:BaseType',
			'courseid:CourseId',
			'classprice:Price',
			'currency_type:currency_type',
			'currency_symbol:currency_symbol',
			'currency:Currency',
			'currency_code:currency_code',
			'langcode:Language',
			'node_id:NodeId',
			'locationid:LocationId',
			'locationname:LocationName',
			'locationaddr1:LocationAddr1',
			'locationaddr2:LocationAddr2',
			'locationcity:LocationCity',
			'latitude:Latitude',
			'longitude:Longitude',
			'locationstate:LocationState',
			'statename:LocationState',
			'locationzip:LocationZip',
			'locationphone:LocationPhone',
			'locationcountry:LocationCountry',
			'language:LanguageName',
			'user_name:Username',
			'orderdatetime:OrderedON',
			'session_id:SessionId',
			'session_start:SessionStartDateDisp',
			'session_start_time:SessionStartTime',
			'session_end_time:SessionEndTime',
			'content_status:ContentStatus',
			'session_title:SessionTitle',
			'ascen:OrderByMRO',
			'max_attempts:ContentMaxAttempt',
			'total_attempts:TotalAttempts',
			'cmpl_expired:IsExpired',
			'managerid:UserManagerId',
			'hire_date:UserHireDate',
			'timezone:SessionTZName',
			'ptimezone:UserTZName',
			'completion_date:CompDateUserTZ',
			'registration_date:RegDateUserTZ',
			'mro:MROValue',
			'assigned_by:AssignedByRole',
			'duedate:DueDate',
			'sessiondatetime:SessionDateTime',
			'tagname:TagName',
			'survey_status:SurveyStatus',
			'assessment_status:AssessmentStatus',
			'preassessment_status:PreAssessStatus',
			'exempted_sts:ExemptedStatus',



		);
	//	if(isset($_REQUEST['apiname'])) {
		if(isset($_REQUEST['mobile_request'])){
			$mobileFields = array(
					'prg_end_date:EndDate',
					'class_id:ClassIdStr',
					'id:EnrollIdStr',
					'user_id:UserIdStr',
					'course_id:CourseIdStr',
					'percentage_complete:CompletionPercentage',
					'is_current:CurrentModule',
					'comp_status:CurrentStatus',
					'comp_date:CompletionDate',
					'prg_expires_in_value:ValidityDays',
					'prg_expires_in_unit:ValidityType',
					'completion_date:CompletionDate'


			);
			$keyStr = array('class_id:ClassId','id:EnrollId','user_id:UserId','course_id:CourseId');
			foreach($keyStr as $key=>$val) {
				$key = array_search($val, $fieldList);
				unset($fieldList[$key]);
			}
			$fieldList = array_merge($fieldList,$mobileFields);
		}
		 if(isset($_REQUEST['apiname']) && !isset($_REQUEST['mobile_request'])) {

			$apiFields = array(

					'class_id:ClassIdStr',
					'id:EnrollIdStr',
					'user_id:UserIdStr',
					'course_id:CourseIdStr'

			);
			$keyStr = array('class_id:ClassId','id:EnrollId','user_id:UserId','course_id:CourseId');
			foreach($keyStr as $key=>$val) {
				$key = array_search($val, $fieldList);
				unset($fieldList[$key]);
			}
			$fieldList = array_merge($fieldList,$apiFields);

		}
		// Setting required filters
		$filter = array();
		if(empty($Solr_User)){
			$userdetails = $this->getLoggedUserDetails();
		}else{
			$userdetails = $Solr_User;
		}
		$uid = $userdetails[0]->id;
		$manager_id = $userdetails[0]->manager_id;
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
			$uid = $_REQUEST['UserID'];
		}
		expDebug::dPrint('User details'.print_r($manager_id,true),4);
		$this->setEnrollmentAccessFilters($filter,$uid);
		$this->setSearchFilters($filter,$uid,$manager_id);

		$results = $this->getMyEnrollmentDetails($filter,$fieldList,'GET');

		return $results;
	}

	public function EnrollmentSolrSearchAutoComplete(){
		global $Solr_User;
		expDebug::dPrint("Solr search is triggered2");
		$fieldList = array(
				'clsid:ClassId',
				'coursetitle:CourseTitle',
				'coursecode:CourseCode',
				'classtitle:Title',
				'classcode:Code',
				);
		$filter = array();
		if(empty($Solr_User)){
			$userdetails = $this->getLoggedUserDetails();
		}else{
			$userdetails = $Solr_User;
		}
		$uid = $userdetails[0]->id;
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
			$uid = $_REQUEST['UserID'];
		}
		$this->setEnrollmentAccessFilters($filter,$uid);
		$this->setSearchFilterAutoComplete($filter);
		$results = $this->getMyEnrollmentDetailsAutoComplete($filter,$fieldList,'GET');
		return $results;
	}

	private function getMyEnrollmentDetails($filter,$fieldList,$method){
		// List of required fields to be set as NULL,
		// because Solr will not keep any null fields
		$baseList = array(
			'master_enrollment_id'=>NULL,
			'class_id'=>NULL,
			'course_id' =>NULL,
			'check_entity_id' =>NULL,
			'check_entity_type' =>NULL,
			'mandatory' =>NULL,
			'is_compliance' =>NULL,
			'compliance_complete_date' =>NULL,
			'compliance_complate_days' =>NULL,
			'compliance_validity_date' =>NULL,
			'compliance_validity_days' =>NULL,
			'recertify_path' =>NULL,
			'reg_status' =>NULL,
			'reg_status_code' =>NULL,
			'status_code' =>NULL,
			'reg_date' =>NULL,
			'comp_status' =>NULL,
			'comp_date' =>NULL,
			'update_date' =>NULL,
			'reg_status_date' =>NULL,
			'valid_from' =>NULL,
			'valid_to' =>NULL,
			'score_id' =>NULL,
			'grade' =>NULL,
			'progress' =>NULL,
			'updated_on' =>NULL,
			'updated_by' =>NULL,
			'created_by' =>NULL,
			'waitlist_priority' =>NULL,
			'preassesment_completion_status' =>NULL,
			'delivery_type_code' =>NULL,
			'dedicated_class_flag' =>NULL,
			'delivery_type' =>NULL,
			'basetype' =>NULL,
			'courseid'=>NULL,
			'classprice' =>NULL,
			'currency_type' => '$' ,
			'currency_symbol' => '$',
			'currency' =>NULL,
			'currency_code'=> '',
			'langcode'=>NULL,
			'node_id' =>NULL,
			'locationid' =>NULL,
			'locationname' =>NULL,
			'locationaddr1' =>NULL,
			'locationaddr2' =>NULL,
			'locationcity' =>NULL,
			'latitude' =>NULL,
			'longitude' =>NULL,
			'locationstate' =>NULL,
			'statename' =>NULL,
			'locationzip' =>NULL,
			'locationphone' =>NULL,
			'locationcountry' =>NULL,
			'orderdatetime' =>NULL,
			'session_id' =>NULL,
			'session_start' =>NULL,
			'session_start_time' =>NULL,
			'session_end_time' =>NULL,
			'content_status' =>NULL,
			'session_title' =>NULL,
			'ascen' =>NULL,
			'max_attempts' =>NULL,
			'total_attempts' =>NULL,
			'cmpl_expired' =>NULL,
			'managerid' =>NULL,
			'timezone' =>NULL,
			'ptimezone' =>NULL,
			'completion_date'=>NULL,
			'registration_date'=>NULL,
			'duedate' =>NULL,
			'sessiondatetime' =>NULL,
			'tagname' =>NULL,
			'mro' =>NULL,
			'assigned_by' =>NULL,
			'survey_status' =>NULL,
			'assessment_status' =>NULL,
			'preassessment_status' =>NULL,
			'exempted_sts' =>NULL,
			'percentage_complete'=>NULL,
			'prg_expires_on'=>NULL,
			'prg_expires_in_value'=>NULL,
			'prg_expires_in_unit'=>NULL,
			'prg_start_date'=>NULL,
			'prg_end_date'=>NULL,
			'enforce_sequence'=>NULL,
			'is_current'=>NULL
		);
		if(isset($_REQUEST['mobile_request']) || isset($_REQUEST['apiname'])){
			$mobileFields = array(
					'id'=>NULL,
					'user_id'=>NULL,
			);
			$baseList = array_merge($baseList,$mobileFields);
		}

		// Append field list
		$data = 'fl=' . implode(',',$fieldList);

		// Append filters
		$data .= '&fq=' . implode('&fq=',$filter);
		//Pagination
		$rows = $_REQUEST['rows'];
		$start = ($_REQUEST['page']*$_REQUEST['rows'])-$_REQUEST['rows'];

		// Sort
		$start = ($start < 0 ) ? 0 : $start;
		$sortby = 'TitleSrt+asc';

		if($_REQUEST['sortBy'] && $_REQUEST['sortBy'] != 'undefined'){
			if($_REQUEST['sortBy'] == 'ZA')
				$sortby = 'TitleSrt+desc';
			else if($_REQUEST['sortBy'] == 'dateNew')
				$sortby = 'EnrollId+asc';
			else if($_REQUEST['sortBy'] == 'dateOld')
				$sortby = 'EnrollId+desc';
			else if($_REQUEST['sortBy'] == 'type')
				$sortby = 'Type+asc';
			else if($_REQUEST['sortBy'] == 'startdate')
				$sortby = 'SessionStartDateSort+asc';
			else if($_REQUEST['sortBy'] == 'orderbystatus')
				$sortby = 'CurrentStatus+asc';
			else if($_REQUEST['sortBy'] == 'Mandatory')
				$sortby = 'Mandatory+desc,Compliance+desc';
			}


		//	$sortby .= ',EnrollId+asc' ;
		expDebug::dPrint('Sort by value is'.$sortby,4);
		$data .= '&sort='.$sortby.'&indent=on&q=*:*&start='.$start.'&rows='.$rows;


		try{
			$srcRst = $this->getData($this->collName,$data,'GET');

			//$srcRst = drupal_json_decode($srcRst);

			expDebug::dPrint('Solr Out put -- > '.print_r(($srcRst),true),4);

			$srcRst1 = $srcRst['response']['docs'];
			expDebug::dPrint("Solr result count ".print_r($srcRst['response']['numFound'],1),4);
			$srcRst2 = array();
			$srcRst2['count'] = $srcRst['response']['numFound'];
		foreach($srcRst1 as $doc){
				$tdoc  = array_merge($baseList, $doc);
				$this->modifyResultSet($tdoc);
				expDebug::dPrint("Solr result each doc ".print_r($tdoc,1),4);
				$srcRst2['result'][] = $this->arrayToObject($tdoc);
			}

			expDebug::dPrint("Solr search result - Final ".print_r($srcRst2,1),4);
			expDebug::dPrint("Solr search result - Final Count".print_r($srcRst['response']['numFound'],1),4);
			return $srcRst2;
		}catch(Exception $e){
			expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
		}

	}

	private function getMyEnrollmentDetailsAutoComplete($filter,$fieldList,$method){
		// List of required fields to be set as NULL,
		// because Solr will not keep any null fields
		$baseList = array(
				'clsid'=> NULL,
				'coursetitle'=> NULL,
				'coursecode'=> NULL,
				'classtitle'=> NULL,
				'classcode'=> NULL,
		);
		// Append field list
		$data = 'fl=' . implode(',',$fieldList);
		// Append filters
		$data .= '&fq=' . implode('&fq=',$filter);

		$data .= '&indent=on&q=*:*';

		$data .= '&rows=1000';

		try{
			$srcRst = $this->getData($this->collName,$data,'GET');

			expDebug::dPrint('Solr Out put -- > '.print_r(($srcRst),true),4);

			$srcRst1 = $srcRst['response']['docs'];
			expDebug::dPrint("Solr result count ".print_r($srcRst['response']['numFound'],1),4);
			$srcRst2 = array();
			$srcRst2['count'] = $srcRst['response']['numFound'];
			foreach($srcRst1 as $doc){
				$tdoc  = array_merge($baseList, $doc);
				$this->modifyResultSet($tdoc);
				expDebug::dPrint("Solr result each doc ".print_r($tdoc,1),4);
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
		expDebug::dPrint('Registration status check12333::'.print_r($regstatus,true),4);
		$regstatus  	= stripApostrophe($regstatus);
		$statusArray = explode('|',$regstatus);
		expDebug::dPrint('Registration status checkssdss::'.print_r(count($statusArray),true),4);
		$i = 0;
		$enrSts = '';
		// Completed with attempts left
		if (in_array('lrn_crs_cmp_cmp_atl',$statusArray)){
			expDebug::dPrint('Status filter check1');
			$enrSts .= '(CurrentStatus:lrn_crs_cmp_cmp AND Launchable:1)';
			$i++;
		}
	// completed and need re-register
		if (in_array('lrn_crs_cmp_cmp_rereg',$statusArray)){
				$notificationTemplateInfo = getNotificationTemplateInfo('compliance_expiry_remainder', 'cre_sys_lng_eng');
				$reregister_remainder_days 	= $notificationTemplateInfo['notify_before'];
				$enrSts .= ($i == 0) ? '' : ' OR ';
				$reReg = 'ExpDate:[NOW-'.$reregister_remainder_days.'DAY TO NOW+'.$reregister_remainder_days.'DAY]';
				expDebug::dPrint('Completed need reregister check-->'.$reReg,4);
				$enrSts .= '((IsExpired:1  AND Compliance:1) OR (CurrentStatus:lrn_crs_cmp_cmp AND Compliance:1 AND ('.$reReg.')))';
				$i++;
			}
			if(isset($_REQUEST['mobile_request'])) {
				if (in_array('lrn_tpm_ovr_cmp_atl',$statusArray)){
					expDebug::dPrint('Status filter check1');
					$enrSts .= ($i == 0) ? '' : ' OR ';
					$enrSts .= '(CurrentStatus:lrn_tpm_ovr_cmp AND Launchable:1)';
					$i++;
				}
				// For completed and need reregister filter
				$config = getConfig("exp_sp");
				$cert_days_expire = isset($config["cert_expire_reminder_days"]) ? $config["cert_expire_reminder_days"] : 10;

				expDebug::dPrint('cert expire day check::'.$cert_days_expire,4);
				if (in_array('lrn_tpm_ovr_cmp_rereg', $statusArray)){
					$enrSts .= ($i == 0) ? '' : ' OR ';
					$reReg = 'ExpDate:[NOW-'.$cert_days_expire.'DAY TO NOW+'.$cert_days_expire.'DAY]';
					expDebug::dPrint('Completed need reregister check-->'.$reReg,4);
					$enrSts .= "((CurrentStatus:lrn_tpm_ovr_exp ) OR ( $reReg AND CurrentStatus:lrn_tpm_ovr_cmp))";
					$i++;
				}
				if (($key = array_search('lrn_crs_cmp_cmp_atl', $statusArray)) !== false) {
					unset($statusArray[$key]);
				}
				if (($key = array_search('lrn_crs_cmp_cmp_rereg', $statusArray)) !== false) {
					unset($statusArray[$key]);
				}
				if (($key = array_search('lrn_tpm_ovr_cmp_atl', $statusArray)) !== false) {
					unset($statusArray[$key]);
				}
				if (($key = array_search('lrn_tpm_ovr_cmp_rereg', $statusArray)) !== false) {
					unset($statusArray[$key]);
				}
				foreach($statusArray as $key=>$val){
					$enrSts .= ($i == 0) ? '' : ' OR ';
					$enrSts .= 'CurrentStatus:*'.trim($val).'*';
					$i++;
				}
			}else {
				if((in_array('lrn_crs_cmp_enr', $statusArray))||(in_array('lrn_crs_cmp_cmp', $statusArray))||(in_array('lrn_crs_reg_can', $statusArray))
					||(in_array('lrn_crs_reg_ppm', $statusArray))||(in_array('lrn_crs_cmp_inp', $statusArray))||(in_array('lrn_crs_cmp_att', $statusArray))||
					(in_array('lrn_crs_reg_wtl', $statusArray))||(in_array('lrn_crs_cmp_inc', $statusArray)) || (in_array('lrn_crs_cmp_exp', $statusArray))){
				if (($key = array_search('lrn_crs_cmp_cmp_atl', $statusArray)) !== false) {
					unset($statusArray[$key]);
				}
				if (($key = array_search('lrn_crs_cmp_cmp_rereg', $statusArray)) !== false) {
					unset($statusArray[$key]);
				}
				foreach($statusArray as $key=>$val){
					$enrSts .= ($i == 0) ? '' : ' OR ';
					$enrSts .= 'CurrentStatus:*'.trim($val).'*';
					$i++;
				}
			}
		}

		expDebug::dPrint('Status filter check22222222::'.$enrSts,4);
		$filter_menu = $enrSts;

		if(!isset($_REQUEST['enr_id']))                                            // Condition added for 0085079
			array_push($filter, urlencode($filter_menu));


		$this->setPriceFilter($filter);
		$this->setScheduleFilter($filter);
		$this->setDueFilter($filter);
		$this->setRegDateFilter($filter);
		$this->setAssignedByFilter($filter,$uid,$manager_id);
		$this->setLocFilter($filter);
		$this->setTPtypeFilter($filter);
		$this->setDelTypeFilter($filter);
		$this->setTextFilter($filter);
		$this->setEnrolledFilter($filter);  // Condition added for 0085079

		expDebug::dPrint('Registration level check'.print_r($filter,true),4);
	}



	private function setPriceFilter(&$filter){
		if($_REQUEST['price']){
			if($_REQUEST['price'] == 'priced')
				$pricefilter = '-Price:0.0';
			else if($_REQUEST['price'] == 'free')
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
			$duedatefilter = 'RegistrationDate:[NOW-'.$_REQUEST['reg'].'DAYS TO NOW]';
			expDebug::dPrint('Due date filter'.print_r($duedatefilter,true),4);
			array_push($filter, urlencode($duedatefilter));
		}
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
			if ($_REQUEST ['date_from'] || $_REQUEST ['date_to']) {
				if($_REQUEST ['date_from']){
				$date_from = $_REQUEST ['date_from'];
				$date_from .= 'T00:00:00Z';
				}
				else
					$date_from = '*';
				if($_REQUEST ['date_to']){
				$date_to = $_REQUEST ['date_to'];
				$date_to .= 'T00:00:00Z';
				}
				else
					$date_to = '*';
				$regDatefilter = 'RegistrationDate:['.$date_from.' TO '.$date_to.']';
				expDebug::dPrint ( 'Due date filter' . print_r ( $regDatefilter, true ), 4 );
				array_push ($filter,urlencode($regDatefilter));
			}
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
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname']) && !isset($_REQUEST['mobile_request'])){
			$_REQUEST['del_type'] = $_REQUEST['delivery_type_code'];
		}
		if($_REQUEST['del_type']){
			$deltype = explode('|',$_REQUEST['del_type']);
			foreach($deltype as $key=>$val){
				if(empty($val)) continue;
				if($key==0)
					$deltypefilter = 'Type:(*'.trim($val).'*';
				else
					$deltypefilter .= ' OR *'.trim($val).'*';
			}
			$deltypefilter .= ')';
			array_push($filter, urlencode($deltypefilter));
			expDebug::dPrint("Solr search is deltypefilter");
		}
	}

	private function setTPtypeFilter(&$filter){
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
			if($_REQUEST['compliance_mandatory'] == 1){
				$_REQUEST['tra_type'] = 'cre_sys_inv_man|cre_sys_inv_com';
			}
			else {
				$_REQUEST['tra_type'] = $_REQUEST['compliance_mandatory'];
			}
		}
		if($_REQUEST['tra_type']) {
			$trainingType = stripRegStatus(stripApostrophe($_REQUEST['tra_type']));
			$mrovalue= explode('|',$trainingType);
			$i = 0;
			if (in_array('cre_sys_inv_man', $mrovalue)){
				$trainingfilter = 'Mandatory:Y';
				$i++;
			}
			if (in_array('cre_sys_inv_opt', $mrovalue)){
				$trainingfilter .= ($i == 0) ? '' : ' OR';
				$trainingfilter .= ' MROValue:Optional##cre_sys_inv_opt';
				$i++;
			}
			if (in_array('cre_sys_inv_rec', $mrovalue)){
				$trainingfilter .= ($i == 0) ? '' : ' OR';
				$trainingfilter .= ' MROValue:Recommended##cre_sys_inv_rec';
				$i++;
			}
			if (in_array('cre_sys_inv_com', $mrovalue)){
				$trainingfilter .= ($i == 0) ? '' : ' OR';
				$trainingfilter .= ' Compliance:1';
				$i++;
			}
			if(isset($_REQUEST['mobile_request'])) {
				foreach($mrovalue as $key=>$val){
					if($key==0)
						$trainingfilter .= ' OR MRO:*'.$val.'*';
					else
						$trainingfilter .= ' OR MRO:*'.$val.'*';
				}
			}
			expDebug::dPrint('Training filter value'.print_r($trainingfilter,true),4);
			array_push($filter, urlencode($trainingfilter));
		}
	}

	private function setLocFilter(&$filter){
		if($_REQUEST['selectedLocID'] && $_REQUEST['selectedLocID']!=0){
			$locfilter = 'LocationId:'.$_REQUEST['selectedLocID'].'';
		}
		else if($_REQUEST['location']){
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
					$locfilter = 'LocationAddress:*'.$val.'*';
				else
					$locfilter .= ' AND LocationAddress:*'.$val.'*';
			}
		}
		if($locfilter)
			array_push($filter, urlencode($locfilter));
	}

	private function setTextFilter(&$filter){
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname']) && !isset($_REQUEST['mobile_request'])){
			$_REQUEST['searchText'] = $_REQUEST['ClassTitle'];
		}
		if(!empty($_REQUEST['searchText'])){
			$text = explode(' ',$_REQUEST['searchText']);
			foreach($text as $key=>$val){
				if($key==0)
					$textfilter = 'TextSearch:*'.$val.'*';
				else
					$textfilter .= ' AND TextSearch:*'.$val.'*';
			}
			expDebug::dPrint('Registration level check'.print_r($textfilter,true),4);
			array_push($filter, urlencode($textfilter));
		}
	}

	private function setEnrolledFilter(&$filter){
		if(isset($_REQUEST['enr_id']) && $_REQUEST['enr_id'] != '' && !empty($_REQUEST['enr_id'])){
			$enr = 'EnrollId:'.$_REQUEST['enr_id'].'';
			array_push($filter, urlencode($enr));
		}
	}

	private function setEnrollmentAccessFilters(&$filter,&$uid){
		$grpstr = 'UserId:'.$uid.'';
		array_push($filter, urlencode($grpstr));

		if(!isset($_REQUEST['apiname']) || empty($_REQUEST['apiname'])){
			$class = '-BaseType:TP';
			array_push($filter, urlencode($class));
			$grpstr1 = '-MasterId:*'; // In web master enrollments are not fetched, In api List of user's enrollments-- master enrollment classes are also fetched
			array_push($filter, urlencode($grpstr1));
		} else {
			$grpstr1 = '-MasterId:*';
			array_push($filter, urlencode($grpstr1));
		}
	}

	private function setSearchFilterAutoComplete(&$filter){
		$search = $_REQUEST['z'];
		$text = explode(' ',$search);
		foreach($text as $key=>$val){
			if($key==0)
				$textfilter = 'TextSearch:*'.$val.'*';
			else
				$textfilter .= ' AND TextSearch:*'.$val.'*';
		}
		expDebug::dPrint('Registration level check'.print_r($textfilter,true),4);
		if($textfilter)
			array_push($filter, urlencode($textfilter));

	}

	private function modifyResultSet(&$tdoc){
		$obj = $this;
		if($this->responseType == 'json'){
			foreach($srcRst['response']['docs'] as $key=>&$doc){

			}
		}elseif($this->responseType == 'xml'){
			try{
				$obj = $this;
				$recs = $this->apiResponseHandler($srcRst,'myenroll');
				$dom = new DOMDocument();
				$dom->loadXML($recs);
			/*	$res = $dom->getElementsByTagName('mro_id');
				foreach($res as $rs){
					if(!empty($rs->nodeValue))
						$rs->nodeValue = $obj->resetMRO($rs->nodeValue);
				}*/
				$srcRst = $dom->saveXML();
			}catch(Exception $e){
				expDebug::dPrint("Error in parsing XML data -- ".print_r($e,true),1);
			}
		}
		/* Reset id
		 // $this->resetId($tdoc);
		 //$this->resetRegDate($tdoc);
		 //$this->resetSessionStart($tdoc);*/
		//Reset Location id
		$this->resetLocationId($tdoc);
		$this->resetScoreField($tdoc);
		if(isset($_REQUEST['mobile_request'])){
			$this->resetEndDateForMobileApi($tdoc);
		}
	}
	private function resetEndDateForMobileApi(&$tdoc){
		$prgEndDate = $tdoc ['prg_end_date'];
		$prgEndDate = str_replace ( 'T', ' ', $prgEndDate );
		$prgEndDate = str_replace ( 'Z', '', $prgEndDate );
		$tdoc ['prg_end_date'] = $prgEndDate;
	}
	private function resetId(&$tdoc){
		$tdoc['id'] = $tdoc['enroll_id'];
		unset($tdoc['enroll_id']);
	}

	private function resetRegDate(&$tdoc){
		$months = array (1=>'Jan',2=>'Feb',3=>'Mar',4=>'Apr',5=>'May',6=>'Jun',7=>'Jul',8=>'Aug',9=>'Sep',10=>'Oct',11=>'Nov',12=>'Dec');
		// reg_date
		$reg = $tdoc['reg_date'];
		$reg = explode('T',$reg);
		$reg = $reg[0];
		$reg = explode('-',$reg);
		$regDate = ''.$reg[2].'-'.$months[(int)$reg[1]].'-'.$reg[0].'';
		$tdoc['reg_date'] = $regDate;

		// comp_date
		if($tdoc['comp_date']!=NULL || $tdoc['comp_date'] != '' || !empty($tdoc['comp_date'])){
			$reg = $tdoc ['comp_date'];
			$reg = explode ( 'T', $reg );
			$reg = $reg [0];
			$reg = explode ( '-', $reg );
			$regDate = '' . $reg [2] . '-' . $months [( int ) $reg [1]] . '-' . $reg [0] . '';
			$tdoc ['comp_date'] = $regDate;
		}

		// update_date
		$reg1 = $tdoc['update_date'];
		$reg1 = explode('T',$reg1);
		$reg1 = $reg1[0];
		$reg1 = explode('-',$reg1);
		$regDate1 = ''.$reg1[2].'-'.$months[(int)$reg1[1]].'-'.$reg1[0].'';
		$tdoc['update_date'] = $regDate1;

		// reg_status_date
		$reg_status = $tdoc['reg_status_date'];
		$reg_status = explode('T',$reg_status);
		$reg_status = $reg_status[0];
		$reg_status = explode('-',$reg_status);
		$reg_status_date = ''.$reg_status[2].'-'.$months[(int)$reg_status[1]].'-'.$reg_status[0].'';
		$tdoc['reg_status_date'] = $reg_status_date;


		// compliance_complete_date
		if($tdoc['compliance_complete_date']!=NULL && $tdoc['compliance_complete_date'] != '' && !empty($tdoc['compliance_complete_date'])){
			$compliance_date = $tdoc ['compliance_complete_date'];
			$compliance_date = str_replace ( 'T', ' ', $compliance_date );
			$compliance_date = str_replace ( 'Z', '', $compliance_date );
			$tdoc ['compliance_complete_date'] = $compliance_date;
		}

		// compliance_complete_date
		if($tdoc['compliance_validity_date']!=NULL && $tdoc['compliance_validity_date'] != '' && !empty($tdoc['compliance_validity_date'])){
			$compliance_date = $tdoc ['compliance_validity_date'];
			$compliance_date = str_replace ( 'T', ' ', $compliance_date );
			$compliance_date = str_replace ( 'Z', '', $compliance_date );
			$tdoc ['compliance_complete_date'] = $compliance_date;
		}

		// validfrom
		if($tdoc['valid_from']!=NULL && $tdoc['valid_from'] != '' && !empty($tdoc['valid_from'])){
			$hire_date = $tdoc ['valid_from'];
			$hire_date = str_replace ( 'T', ' ', $hire_date );
			$hire_date = str_replace ( 'Z', '', $hire_date );
			$tdoc ['valid_from'] = $hire_date;
		}
		// validTO
		if($tdoc['valid_to']!=NULL && $tdoc['valid_to'] != '' && !empty($tdoc['valid_to'])){
			$hire_date = $tdoc ['valid_to'];
			$hire_date = str_replace ( 'T', ' ', $hire_date );
			$hire_date = str_replace ( 'Z', '', $hire_date );
			$tdoc ['valid_to'] = $hire_date;
		}
	}
	private function resetLocationId(&$tdoc){
		$locid = (array)$tdoc['locationid'];
		$tdoc['locationid'] = $locid[0];
	}
	private function resetSessionStart(&$tdoc){
		$months = array (1=>'Jan',2=>'Feb',3=>'Mar',4=>'Apr',5=>'May',6=>'Jun',7=>'Jul',8=>'Aug',9=>'Sep',10=>'Oct',11=>'Nov',12=>'Dec');
		$locid = (array)$tdoc['session_start'];
		$sess_start = $locid[0];
		$reg_status = explode('T',$sess_start);
		$reg_status = $reg_status[0];
		$reg_status = explode('-',$reg_status);
		$reg_status_date = ''.$reg_status[2].'-'.$months[(int)$reg_status[1]].'-'.$reg_status[0].'';
		$tdoc['session_start'] = $reg_status_date;
	}



/** My Enrollment In My Learning Page ends here **/

/** My Calendar sectin starts here **/

	public function getEnrollmentsForCalendar($startDate,$endDate,$uid){
		$fieldList = array(
				'class_id:ClassId',
				'reg_status:RegistrationStatus',
				'comp_status:CompletionStatus',
			);
		$filter = array();
		$this->setDefaultFilterForUser($filter,$startDate,$endDate,$uid);

		$results = $this->getCalendarDetailsEnrollmentsOfUser($filter,$fieldList,'GET');

		return $results;
	}
	private function setDefaultFilterForUser(&$filter,$startDate,$endDate,$uid){
		// Fetch enrollments of user
		$enroll_user = 'UserId:'.$uid.'';
		array_push($filter, urlencode($enroll_user));
		// Fetch session date range filter
		$startDate = str_replace(' ','T',$startDate);
		$startDate.='Z';
		$endDate = str_replace(' ','T',$endDate);
		$endDate.='Z';
		$sessionRange = 'SessionStartDate:['.$startDate.' TO '.$endDate.']';
		array_push($filter, urlencode($sessionRange));
		// Fetch enrollments which is confirmed
		$status = '(RegistrationStatus:(*lrn_crs_reg_cnf*) OR (*lrn_crs_reg_ppm*)) OR (CompletionStatus:(*lrn_crs_cmp_cmp*))';
		array_push($filter, urlencode($status));
	}
	private function getCalendarDetailsEnrollmentsOfUser($filter,$fieldList,$method){
		// List of required fields to be set as NULL,
		// because Solr will not keep any null fields
		$baseList = array(
				'class_id'=>NULL,
				'reg_status'=>NULL,
				'comp_status'=>NULL,
		);

		// Append field list
		$data = 'fl=' . implode(',',$fieldList);
		// Append filters
		$data .= '&fq=' . implode('&fq=',$filter);
		//Pagination
		$start = 0;
		$rows = 1000;
		// Sort
		$data .= '&indent=on&q=*:*&start='.$start.'&rows='.$rows.'';

		try{
			$srcRst = $this->getData($this->collName,$data,'GET');
			expDebug::dPrint('Solr Out put -- > '.print_r(($srcRst),true),4);

			$srcRst1 = $srcRst['response']['docs'];
			expDebug::dPrint("Solr result count ".print_r($srcRst1,1),4);

			$srcRst2 = array();
			$srcRst2['count'] = $srcRst['response']['numFound'];

			foreach($srcRst1 as $doc){
				$tdoc  = array_merge($baseList, $doc);
				$srcRst2['result'][] = $this->arrayToObject($tdoc);
			}

			expDebug::dPrint("Solr search result - Final ".print_r($srcRst2,1),4);
			return $srcRst2;
		}catch(Exception $e){
			expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
		}
	}
/** My Calendar sectin ends here **/
	}


?>
