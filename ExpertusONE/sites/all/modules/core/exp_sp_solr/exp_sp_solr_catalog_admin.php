<?php 
require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solrclient.php';
class AdminCatalogSolrSearch extends SolrClient{
	function __construct(){
		// Get collection prefix from configuration
		$prefix = getConfigValue('collection_prefix');
	
		// Set exact collection name (should match with solr's collection)
		$this->collName = $prefix . '_' . SolrClient::CatalogCore;
	
		//Call parent constructor
		parent::__construct();
	}
	
	public function getCourseList(){
	if($_REQUEST['catalogtype'] == 'Class'){
			$fieldList = array('id:CourseId',
								'class_id:EntityId', // modified for ticket 0084881: Most recent classes created to a course not showing up
			);
		}
		else{
			$fieldList = array('id:EntityId',
								'class_id'          // modified for ticket 0084881: Most recent classes created to a course not showing up
			);
		}
		$fieldList1 = array(
				'title:Title',
				'code:Code',
				'short_desc:Description',
				'status_code:Status',
				'status:StatusName',
				'lang:LangName',
				'delivery_type:TypeName',
				'delivery_type_code:Type',
				'facility_name',
				'location_name',
				'price:Price',
				'object_type:EntityTypeName',
				'Privilege',
				'CreatedBy',
				'UpdatedBy'
		);
		
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){		
			if($_REQUEST['catalogtype'] == 'Class'){
				$apiFields = array(
						'location_name:ClassLocationNames',
						'duration:duration',
						'author_vendor:authorVendor',
						'is_compliance:Compliance',
						'currency:CurrencyTypeCode',
						'currency_code:CurrencyCode',
						'currency_symbol:CurrencySymbol',
						'tagname:TagName',
						'additional_info:AdditionalInfo',
						'shown_in_catalog:AddInfoShowCatalog',
						'shown_in_notification:AddInfoShowMail',
				);
			
			}else{
				$apiFields = array(
				'location_name:ClassLocationNames',
				'tagname:TagName',
				'currency_type:CurrencyTypeCode',
				);
			}
			
			
			unset($fieldList1['location_name']); // Field names for API and WEB are different hence removeing for api
			$fieldList1 = array_merge($fieldList1,$apiFields);
		}
      	    $fieldList = array_merge($fieldList,$fieldList1);
		

		
		expDebug::dPrint("Solr search result - Final fieldlist".print_r($fieldList,1),4);
		// Setting required filters
		$filter = array();
		$this->setAdminAccessFilters($filter);
		$this->setSearchFilters($filter);
	
		$results = $this->getCatalogDetails($filter,$fieldList,'GET');
			
		return $results;
	}
	
	public function getCatalogAutoComplete(){
		$fieldAutoComplete = array(
				'title:Title',
				'type:EntityTypeName'
		);
		$filter = array();
		$this->setAdminAccessFilters($filter);
		$this->setAutoCompleteFilters($filter);
		$results = $this->getCatalogDetailsAutoComplete($filter,$fieldAutoComplete,'GET');
		return $results;
	}
	
	private function getCatalogDetails($filter,$fieldList,$method){
		global $Solr_User;
		// List of required fields to be set as NULL,
		// because Solr will not keep any null fields
		$baseList = array(
				'id'=>null,
				'title'=>null,
				'code'=>null,
				'short_desc'=>null,
				'status_code'=>null,
				'status'=>null,
				'lang'=>null,
				'delivery_type'=>null,
				'class_id'=>null,
				'delivery_type_code'=>null,
				'facility_name'=>null,
				'location_name'=>null,
				'price'=>null,
				'object_type'=>null,
				'sumedit'=>null,
				'sumdelete'=>null
		);
		
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
			if($_REQUEST['catalogtype'] == 'Class'){
				$apiFields = array(
						'location_name'=>null,
						'duration'=>null,
						'author_vendor'=>null,
						'is_compliance'=>null,
						'currency'=>null,
						'currency_code'=>null,
						'currency_symbol'=>null,
						'tagname'=>null,
						'additional_info'=>nulll
				);
			
			}else{
				$apiFields = array(
				'location_name'=>null,
				'tagname'=>null,
				);
			}
		
			unset($baseList['location_name']); // Field names for API and WEB are different hence removeing for api
			unset($baseList['sumedit']);
			unset($baseList['sumdelete']);
			$baseList = array_merge($baseList,$apiFields);
			$dispCols=explode(',',$_REQUEST['display_cols']);
			foreach($fieldList as $field=>$val){
				$acFld = explode(':',$val);
				if(!in_array($acFld[0],$dispCols)){
					unset($fieldList[$field]);
					unset($baseList[$acFld[0]]);
				}
			}
		}
		// Append field list
		$data = 'fl=' . implode(',',$fieldList);
	
		// Append filters
		$data .= '&fq=' . implode('&fq=',$filter);
		//Pagination
		$rows = 10;
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
			$start = ($_REQUEST['rows']*$_REQUEST['limit'])-$_REQUEST['limit'];
			$rows = !empty($_REQUEST['limit']) ? $_REQUEST['limit'] : $rows;
		}else{
		    $start = ($_REQUEST['page']*$_REQUEST['rows'])-$_REQUEST['rows'];
		    $rows = !empty($_REQUEST['rows']) ? $_REQUEST['rows'] : $rows;
		}
		
		
		if($_REQUEST['sortby'] && $_REQUEST['sortby']!='NewlyListed'){
			$sortby = $_REQUEST['sortby'] == 'AZ'?'TitleSrt+asc':'TitleSrt+desc';
		}
		else
			$sortby = 'CreatedOn+desc';
		expDebug::dPrint('Sort by value '.print_r($sortby,true),4);
		$start = $start < 0 ? 0 : $start;
		$data .= '&sort='.$sortby.'&indent=on&q=*:*&start='.$start.'&rows='.$rows;
	
		try{
			$srcRst = $this->getData($this->collName,$data,'GET');
	
			if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
				return $this->apiResponseHandler($srcRst,'catalog',$baseList);
			}
				
			$srcRst1 = $srcRst['response']['docs'];
			$srcRst2 = array();
			$srcRst2['recCount'] = $srcRst['response']['numFound'];
			// Logged in users details
			if(empty($Solr_User)){
				$results = $this->getLoggedUserDetails();
			}else{
				$results = $Solr_User;
			}
			foreach($srcRst1 as $doc){
				$tdoc  = array_merge($baseList, $doc);
				$this->modifyResultSet($tdoc,$results);
				$tdoc = $this->removeTilt($tdoc);
				$srcRst2['records'][0][] = $this->arrayToObject($tdoc);
			}
			$srcRst2['records'][1] = $this->getManageByValueCourse('course','coursemanage');
			$srcRst2['records'][2] = $this->getManageByValueCourse('class','classmanage');
			expDebug::dPrint("Solr search result - Final ".print_r($srcRst2,1),4);
			expDebug::dPrint("Solr search result - Final Count".print_r($srcRst['response']['numFound'],1),4);
			return $srcRst2;
		}catch(Exception $e){
			expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
		}
	
	}
	
	private function getCatalogDetailsAutoComplete($filter,$fieldList,$method){
		global $Solr_User;
		$baseList = array(
				 'title'=>null,
				'type'=>null
		);
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
			
			// Logged in users details
			if(empty($Solr_User)){
				$results = $this->getLoggedUserDetails();
			}else{
				$results = $Solr_User;
			}
			foreach($srcRst1 as $doc){
				$tdoc  = array_merge($baseList, $doc);
				$this->modifyResultSet($tdoc,$results);
				$tdoc = $this->removeTilt($tdoc);
				$srcRst2['records'][0][] = $this->arrayToObject($tdoc);
			}
			expDebug::dPrint("Solr search result - Final ".print_r($srcRst2,1),4);
			expDebug::dPrint("Solr search result - Final Count".print_r($srcRst['response']['numFound'],1),4);
			return $srcRst2;
		}catch(Exception $e){
			expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
		}
	
	}
	
	private function getManageByValueCourse($page,$filtername){
		$isPageLoad = ($page == 'class')? empty($_REQUEST['clspgopened']) : !isset($_REQUEST[$filtername]);
		$manageByFilterValArr = empty($_REQUEST[$filtername])? array() : explode('|', $_REQUEST[$filtername]);
		$numFilterValues = count($manageByFilterValArr);
		if($isPageLoad)
			$manage_filter = 'me';
		else{
			if ($numFilterValues == 0 || $numFilterValues == 2)
				$manage_filter = 'both';
			else{
				if ($manageByFilterValArr[0] == 'cre_sys_fop_me')
					$manage_filter = 'me';
				else
					$manage_filter = 'others';
			}
		}
		return $manage_filter;
	}
	
	private function setSearchFilters(&$filter){
		$this->setDefaultFilter($filter);
		$this->setStatusFilter($filter);
		$this->setLangFilter($filter);
		if(empty($_REQUEST['apiname'])){
		$this->setManageFilter($filter);
		}
		$this->setTagFilter($filter);
		$this->setDeliveryTypeFilter($filter);
		$this->setPriceFilter($filter);
		$this->setCurrencyFilter($filter);
		$this->setDateFilter($filter);
		$this->setContentFilter($filter);
		$this->setSurveyFilter($filter);
		$this->setAssessmentFilter($filter);
		$this->setInstructorFilter($filter);
		$this->setLocFilter($filter);
		$this->setTextSearch($filter);
		$this->searchByEntity($filter);
		
		expDebug::dPrint('Registration level check'.print_r($filter,true),4);
	}
	
	private function setDefaultFilter(&$filter){
		if($_REQUEST['catalogtype'] == 'Class')
			$type = 'EntityType:cre_sys_obt_cls';
		else
			$type = 'EntityType:cre_sys_obt_crs';
		array_push($filter, urlencode($type));
	}
	
	private function setStatusFilter(&$filter){
		$statusname = ($_REQUEST['catalogtype'] == 'Class')? 'catalogclassstatus' : 'catalogcoursestatus';
			if ($_REQUEST [$statusname]) {
				expDebug::dPrint ( "Solr search result - Final123 " . print_r ( $_REQUEST [$statusname], 1 ), 4 );
				$status = explode ( '|', $_REQUEST [$statusname] );
				expDebug::dPrint ( "Solr search result - Final321 " . print_r ( $status, 1 ), 4 );
				foreach ( $status as $key => $val ) {
					if ($key == 0)
						$statusfilter = 'Status:(*' . trim ( $val ) . '*';
					else
						$statusfilter .= ' OR *' . trim ( $val ) . '*';
				}
				$statusfilter .= ')';
				array_push ( $filter, urlencode ( $statusfilter ) );
			}
		}
	
	private function setLangFilter(&$filter){
		$langName = ($_REQUEST['catalogtype'] == 'Class')? 'classlangtype' : 'courselangtype';
		
			if ($_REQUEST [$langName]) {
				$lang = explode ( '|', $_REQUEST [$langName] );
				foreach ( $lang as $key => $val ) {
					if ($key == 0)
						$langfilter = 'LangCode:(*' . trim ( $val ) . '*';
					else
						$langfilter .= ' OR *' . trim ( $val ) . '*';
				}
				$langfilter .= ')';
				array_push ( $filter, urlencode ( $langfilter ) );
			}
	  }
	
	private function setManageFilter(&$filter){
		global $Solr_User;
		//Manage -- While logging in, manage option should be me. Managed by the logged in user
		if(empty($Solr_User)){
			// Setting group filters
			require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user.php';
			$userObj = new UserSolrSearch();
			$userDetails = $userObj->getLoggedUserDetails();
		}else{
			$userDetails = $Solr_User;
		}
		$uid = $userDetails[0]->id;
		$manageby = ($_REQUEST['catalogtype'] == 'Class')? 'classmanage' : 'coursemanage';
		if(!isset($_REQUEST[$manageby]))
			$manage = 'CreatedBy:'.$uid.' OR UpdatedBy:'.$uid.'';
		else{
			expDebug::dPrint('manage filter set yes',4);
			if($_REQUEST[$manageby]){
				$filtervalues = explode('|',$_REQUEST[$manageby]);
				expDebug::dPrint('manage filter set yes'.print_r($filtervalues,true),4);
				if(count($filtervalues) == 1){
					if($filtervalues[0] == 'cre_sys_fop_me')
						$manage = 'CreatedBy:'.$uid.' OR UpdatedBy:'.$uid.'';
					else if($filtervalues[0] == 'cre_sys_fop_oth')
						$manage = '-CreatedBy:'.$uid.' AND -CreatedBy:'.$uid.'';
				}
			}
		}
		expDebug::dPrint('Manage filter:'.print_r($manage,true),4);
		if($manage)
			array_push($filter, urlencode($manage));
	}
	
	private function setTagFilter(&$filter){
		$tagName = ($_REQUEST['catalogtype'] == 'Class')? 'catalogclasstag' : 'catalogcoursetag';
		$tagName = (isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])) ? 'tagname' : $tagName;
			if ($_REQUEST [$tagName]) {
				$tg =  urldecode($_REQUEST[$tagName]);
				$tagfilter = 'TagSearch:"*' . str_replace ( ' ', '`', $tg ) . '*"';
				expDebug::dPrint ( 'Tagfilter:::' . print_r ( $tg, true ), 4 );
				array_push ( $filter, urlencode ( $tagfilter ) );
			}
		}
	
	private function setDeliveryTypeFilter(&$filter){
		if($_REQUEST['deliverytype'] && $_REQUEST['catalogtype'] == 'Class'){
			$type = explode('|',$_REQUEST['deliverytype']);
			foreach($type as $key=>$val){
				if($key==0)
					$typefilter = 'Type:(*'.trim($val).'*';
				else
					$typefilter .= ' OR *'.trim($val).'*';
			}
			$typefilter .= ')';
			array_push($filter, urlencode($typefilter));
		}
	}
	
	private function setPriceFilter(&$filter){
		
		$price = ($_REQUEST['catalogtype'] == 'Class')? 'classPrice' : 'coursePrice';
		if($_REQUEST[$price]){
			expDebug::dPrint('PRice entered is:'.print_r($_REQUEST[$price],true),4);
			$price = str_replace('|',' TO ',$_REQUEST[$price]);
			expDebug::dPrint('check of price'.print_r($price,1),4);
			$pricefilter = 'Price:['.$price.']';
			if($price[0] == 0) // Handles when Price value comes as null from table.This will be changed/removed when Course level pricing is introduced.
				$pricefilter .= ' OR *';
			array_push($filter, urlencode($pricefilter));
		}
		
	}
	
	private function setCurrencyFilter(&$filter){
		$cur = ($_REQUEST['catalogtype'] == 'Class')? 'classcurrency' : 'coursecurrency';
		if($_REQUEST[$cur]){
			$type = explode('|',$_REQUEST[$cur]);
			foreach($type as $key=>$val){
				if($key==0)
					$typefilter = 'CurrencyTypeCode:(*'.trim($val).'*';
				else
					$typefilter .= ' OR *'.trim($val).'*';
			}
			$typefilter .= ')';
			array_push($filter, urlencode($typefilter));
		}
		
	}
	
	private function setDateFilter(&$filter){
		if($_REQUEST['classdaterange'] && $_REQUEST['catalogtype'] == 'Class'){
			$date =  explode('|',$_REQUEST['classdaterange']);
			if($date[0]){
				expDebug::dPrint('date entered'.print_r($date,true),4);
				$start_date = date_format(date_create_from_format('m-d-Y', $date[0]), 'Y-m-d');
				$start_date .='T00:00:00Z';
				if($date[1]){
					$end_date = date_format(date_create_from_format('m-d-Y', $date[1]), 'Y-m-d');
					$end_date .='T00:00:00Z';
				}
				else
					$end_date = '*';
				$date_range = ''.$start_date.' TO '.$end_date.'';
				$datefilter = 'SessionStart:['.$date_range.']';
				expDebug::dPrint('Date range filter entered is'.print_r($datefilter,true),4);
				array_push($filter, urlencode($datefilter));
			}
		}
		
	}
	
	private function setContentFilter(&$filter){
		if($_REQUEST['classContent'] && $_REQUEST['catalogtype'] == 'Class'){
			$content = 'Content:"'.$_REQUEST['classContent'].'"';
			array_push($filter, urlencode($content));
		}
		
	}
	
	private function setSurveyFilter(&$filter){
		if($_REQUEST['classSurvey'] && $_REQUEST['catalogtype'] == 'Class'){
			expDebug::dPrint('Surveyfilter:::'.print_r($_REQUEST['classSurvey'],true),4);
			if(strpos($_REQUEST['classSurvey'],' ') !== false){
			$type = explode(' ',$_REQUEST['classSurvey']);
			foreach($type as $key=>$val){
				if($key==0)
					$typefilter = 'Survey:*'.$val.'*';
				else
					$typefilter .= ' AND Survey:*'.$val.'*';
			}
			}
			else {
				$typefilter = 'Survey:*'.$_REQUEST['classSurvey'].'*';
			}
			array_push($filter, urlencode($typefilter));
			expDebug::dPrint('Surveyfilter:::'.print_r($typefilter,true),4);
			}
		
	}
	
	private function setAssessmentFilter(&$filter){
	if($_REQUEST['classAssessment'] && $_REQUEST['catalogtype'] == 'Class'){
		if(strpos($_REQUEST['classAssessment'],' ') !== false){
		$type = explode(' ',$_REQUEST['classAssessment']);
			foreach($type as $key=>$val){
				if($key==0)
					$typefilter = 'Assessment:*'.$val.'*';
				else
					$typefilter .= ' AND Assessment:*'.$val.'*';
			}
		}
		else {
			$typefilter = 'Assessment:*'.$_REQUEST['classAssessment'].'*';
		}
			array_push($filter, urlencode($typefilter));
	}
	}
	
	private function setInstructorFilter(&$filter){
		if($_REQUEST['classInstructor'] && $_REQUEST['catalogtype'] == 'Class'){
			if(strpos($_REQUEST['classInstructor'],' ') !== false){
			$type = explode(' ',$_REQUEST['classInstructor']);
			foreach($type as $key=>$val){
				if($key==0)
					$typefilter = 'Instructor:*'.$val.'*';
				else
					$typefilter .= ' AND Instructor:*'.$val.'*';
			}
			}
			else {
				$typefilter = 'Instructor:*'.$_REQUEST['classInstructor'].'*';
			}
			array_push($filter, urlencode($typefilter));
		}
	}
	
	private function setLocFilter(&$filter){
		if($_REQUEST['classLocation'] && $_REQUEST['catalogtype'] == 'Class'){
					$location   = preg_replace("/[^a-zA-z]/", ' ', $_REQUEST['classLocation']);
					expDebug::dPrint ( 'location filter entered ---- >' . print_r ( $location, true ), 4 );
					$type = explode(' ',$location);
					foreach($type as $key=>$val){
						if(strlen($val)>=3){
							if($i == 0){
								$loc = 'LocationAddress:*'.$val.'*';
								$i++;
							}
							else {
								$loc .= ' AND LocationAddress:*'.$val.'*';
							}
						}
					}
					array_push($filter, urlencode($loc));
					}
	}
	
	private function setTextSearch(&$filter){
		if($_REQUEST['textfilter']){
			$_REQUEST['textfilter'] = str_replace('"', '\"', $_REQUEST['textfilter']);
			$textfilter = 'TextSearch:"'.$_REQUEST['textfilter'].'" OR WordSearch:"'.$_REQUEST['textfilter'].'"';
			array_push($filter, urlencode($textfilter));
			}
		}
	
	private function searchByEntity(&$filter){
		if($_REQUEST['jqgrid_row_id']){
			$textfilter = 'EntityId:"'.$_REQUEST['jqgrid_row_id'].'"';
			array_push($filter,urlencode($textfilter));
			if($_REQUEST['catalogtype'] == 'Class')
				$textfilter = 'EntityType:"cre_sys_obt_cls"';
			if($_REQUEST['catalogtype'] == 'Course')
				$textfilter = 'EntityType:"cre_sys_obt_crs"';
			array_push($filter,urlencode($textfilter));
		}
	}
	
	
	private function setAutoCompleteFilters(&$filter){
		$searchCurText = str_replace('"', '\"', $_REQUEST['z']);
		$textfilter = 'TextSearch:"'.$searchCurText.'" OR WordSearch:"'.$searchCurText.'"';
		array_push($filter, urlencode($textfilter));
		
		//Entity filter
		$entity_filter = 'EntityTypeName:Course OR Class';
		array_push($filter, urlencode($entity_filter));
		// status filter
		$status = 'Status:(lrn_cls_sts_atv OR lrn_crs_sts_atv OR lrn_cls_sts_itv OR  lrn_crs_sts_itv)';
		array_push($filter, urlencode($status));
		
	}
	
	private function modifyResultSet(&$tdoc,$results){
		// Reset id
		//$this->resetId($tdoc);
		// Set Edit & Delete privilege
		$this->setPrivileges($tdoc,$results,'cre_sys_obt_crs');
	}
	
	private function resetId(&$tdoc){
		$tdoc['id'] = $tdoc['Id'];
		unset($tdoc['Id']);
	}

}
?>