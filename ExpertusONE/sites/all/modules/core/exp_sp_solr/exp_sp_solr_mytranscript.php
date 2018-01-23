<?php
require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solrclient.php';
class MyTranscriptSolrSearch extends SolrClient{

	function __construct(){
		// Get collection prefix from configuration
		$prefix = getConfigValue('collection_prefix');

		// Set exact collection name (should match with solr's collection)
		$this->collName = $prefix . '_' . SolrClient::EnrollCore;

		expDebug::dPrint("Core name -- ".$this->collName);

		//Call parent constructor
		parent::__construct();
	}
	
	public function TrasncriptSolrSearch(){
		global $Solr_User;
		expDebug::dPrint("Solr search is triggered2");
		$fieldList = array(
				'title:Title',
				'enroll_id:EnrollId',
				'comp_date:CompletionDate',
				'class_id:ClassId',
				'course_id:CourseId',
				'object_type_name:TypeName',
				'delivery_type:Type',
				'score_id:Score',// called modify result set for 0085112
				'grade:Grade',
				'type:BaseType', // called modify result set
				'program_id:ProgramId'
				);
		
		$filter = array();
		if(empty($Solr_User)){
			$userdetails = $this->getLoggedUserDetails();
		}else{
			$userdetails = $Solr_User;
		}
		
		$uid = $userdetails[0]->id;
		expDebug::dPrint('User details'.print_r($uid,true),4);
		
		$this->setDefaultFilters($filter,$uid);
		$results = $this->getTranscriptDetails($filter,$fieldList,'GET');
			
		return $results;
		
	}
	
	private function setDefaultFilters(&$filter,&$uid){
		/* Enrollments starts here*/
		$enroll_filter = '(UserId:'.$uid.''; //Get logged in user's data
		$enroll_filter .= ' AND -BaseType:TP'; // Get Enrollments of user
		$enroll_filter .= ' AND CompletionStatus:lrn_crs_cmp_cmp'; // Get Completed records
		$enroll_filter .= ' AND RegistrationStatus:lrn_crs_reg_cnf'; // Get registration status 
		$enroll_filter .= ' AND -MasterId:*)'; // Remove master enrollments
		/* Enrollments ends here*/
		/* Master Enrollments starts here*/
		$enroll_filter .= ' OR (UserId:'.$uid.''; //Get logged in user's data
		$enroll_filter .= ' AND BaseType:TP'; // Get master Enrollments of user
		$enroll_filter .= ' AND CurrentStatus:lrn_tpm_ovr_cmp'; // Get Completed records
		$enroll_filter .= ' AND CurrentModule:Y)'; // Remove master enrollments
		/* Master Enrollments ends here*/
		expDebug::dPrint(' Filter entered issss---->>'.$enroll_filter,4);
		array_push($filter, urlencode($enroll_filter));
		
	}
	
	private function getTranscriptDetails($filter,$fieldList,$method){
		// List of required fields to be set as NULL,
		// because Solr will not keep any null fields
		
		$baseList = array(
				'title'=>NULL,
				'enroll_id'=>NULL,
				'comp_date'=>NULL,
				'class_id'=>NULL,
				'course_id'=>NULL,
				'object_type_name'=>NULL,
				'delivery_type'=>NULL,
				'score_id'=>NULL,
				'grade'=>NULL,
				'type'=>NULL // called modify result set
				);
		
		// Append field list
		$data = 'fl=' . implode(',',$fieldList);
		// Append filters
		$data .= '&fq=' . implode('&fq=',$filter);
		//Pagination
		$rows = $_GET['rows'];
		$start = ($_GET['page']*$_GET['rows'])-$_GET['rows'];
		// Sort
		$sortby = 'CompletionDate+desc';
		
		$data .= '&sort='.$sortby.'&indent=on&q=*:*&start='.$start.'&rows='.$rows;
		
		try{
			$srcRst = $this->getData($this->collName,$data,'GET');
			expDebug::dPrint('Solr Out put -- > '.print_r(($srcRst),true),4);
				
			$srcRst1 = $srcRst['response']['docs'];
			expDebug::dPrint("Solr result count ".print_r($srcRst1,1),4);
			
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
	
	private function modifyResultSet(&$tdoc){
		// Reset type field
		$this->resetType($tdoc);
		// Reset name for score field
		$this->resetScoreField($tdoc);
	}
	
	private function resetType(&$tdoc){
		// reset value of type as LP for training plan and Class for classes
		if($tdoc['type'] == 'TP'){
			$tdoc['type'] = 'LP';
		}
		else{ 
			$tdoc['type'] = 'Class';
		}
		// Pass prgramid as classid and courseid for TP
		if($tdoc['class_id'] == '' && $tdoc['course_id'] == '' && $tdoc['program_id'] != ''){
			$tdoc['class_id'] = $tdoc['program_id'];
			$tdoc['course_id'] = $tdoc['program_id'];
			unset($tdoc['program_id']);
			}
		}
	
	
}
?>