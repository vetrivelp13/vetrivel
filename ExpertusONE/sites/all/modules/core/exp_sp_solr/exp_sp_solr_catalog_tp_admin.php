<?php 
require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solrclient.php';

class AdminCatalogTPSolrSearch extends SolrClient{
	function __construct(){
		// Get collection prefix from configuration
		$prefix = getConfigValue('collection_prefix');
	
		// Set exact collection name (should match with solr's collection)
		$this->collName = $prefix . '_' . SolrClient::CatalogCore;
	
		//Call parent constructor
		parent::__construct();
	}
	
	public function getTPList(){
		expDebug::dPrint("Solr search is triggered");
		$fieldList = array(
				'Id:EntityId',
				'title:Title',
				'code:Code',
				'short_desc:Description',
				'status_code:Status',
				'status:StatusName',
				'lang:LangName',
				'delivery_type_code:Type',
				'facility_name',
				'location_name',
				'price:Price',
				'object_type:TypeName',
				'Privilege',
				'CreatedBy',
				'UpdatedBy'
		);
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
			$apiFields = array(
					'id:EntityId',
					'shown_in_catalog:AddInfoShowCatalog',
					'shown_in_notification:AddInfoShowMail',
					'currency:CurrencyTypeCode',
					'currency_code:CurrencyCode',
					'currency_symbol:CurrencySymbol',
					'tagname:TagName',
					'additional_info:AdditionalInfo'
			);
			unset($fieldList['Id']);
			$fieldList = array_merge($fieldList,$apiFields);
		}
		// Setting required filters
		$filter = array();
		$this->setAdminAccessFilters($filter);
		$this->setSearchFilters($filter);
			
		$results = $this->getCatalogTPDetails($filter,$fieldList,'GET');
			
		return $results;
	}
	
	public function getCatalogTPAutoComplete(){
		expDebug::dPrint("Solr search is triggered");
		$fieldList = array('title:Title');
		$filter = array();
		$this->setAdminAccessFilters($filter);
		$this->setAutoCompleteFilters($filter);
			
		$results = $this->getCatalogTPDetailsAutoComplete($filter,$fieldList,'GET');
			
		return $results;
	}
	
	private function getCatalogTPDetails($filter,$fieldList,$method){
		global $Solr_User;
		// List of required fields to be set as NULL,
		// because Solr will not keep any null fields
		$baseList = array(
				'Id'=>null,
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
			$apiFields = array(
					'id'=>null,
					'shown_in_catalog'=>null,
					'shown_in_notification'=>null,
					'currency'=>null,
					'currency_code'=>null,
					'currency_symbol'=>null,
					'tagname'=>null,
					'additional_info'=>null
			);
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
		
	expDebug::dPrint('Sort by value is'.print_r($sortby,1),4);
		
		$start = $start < 0 ? 0 : $start;
		
		$data .= '&sort='.$sortby.'&indent=on&q=*:*&start='.$start.'&rows='.$rows;
	
		try{
			$srcRst = $this->getData($this->collName,$data,'GET');
	
			if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
				return $this->apiResponseHandler($srcRst,'program',$baseList);
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
			$srcRst2['records'][1] = $this->getManageByValueTP('tp','tpmanage');
			expDebug::dPrint("Solr search result - Final ".print_r($srcRst2,1),4);
			expDebug::dPrint("Solr search result - Final Count".print_r($srcRst['response']['numFound'],1),4);
			return $srcRst2;
		}catch(Exception $e){
			expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
		}
	
	}
	
	private function getCatalogTPDetailsAutoComplete($filter,$fieldList,$method){
		global $Solr_User;
		// List of required fields to be set as NULL,
		// because Solr will not keep any null fields
		$baseList = array('title'=>null);
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
	
	private function getManageByValueTP($page,$filtername){
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
	
	private function modifyResultSet(&$tdoc,$results){
		// Reset id
		$this->resetId($tdoc);
		// Set Edit & Delete privilege
		$this->setPrivileges($tdoc,$results,'cre_sys_obt_trp');
	}
	
	private function resetId(&$tdoc){
		$tdoc['id'] = $tdoc['Id'];
		unset($tdoc['Id']);
	}
	
	
	private function setSearchFilters(&$filter){
		
		$this->setDefaultFilter($filter);
		$this->setTypeFilter($filter);
		$this->setStatusFilter($filter);
		$this->setLanguageFilter($filter);
		if(empty($_REQUEST['apiname'])){
		$this->setManageFilter($filter);
		}
		$this->setPriceFilter($filter);
		$this->setCurrencyFilter($filter);
		$this->setCourseFilter($filter);
		$this->setSurveyFilter($filter);
		$this->setAssmntFilter($filter);
		$this->setTagFilter($filter);
		$this->setTextFilter($filter);
		$this->searchByEntity($filter);
		expDebug::dPrint('Registration level check'.print_r($filter,true),4);
	}
	
	
	private function setDefaultFilter(&$filter){
		$type = 'ProgramTypeCode:cre_sys_obt_trp';
		array_push($filter, urlencode($type));
		}
	
	private function setTypeFilter(&$filter){
		if($_REQUEST['deliverytype']){
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
	
	private function setStatusFilter(&$filter){
		if($_REQUEST['prgstatus']){
			$status = explode('|',$_REQUEST['prgstatus']);
			foreach($status as $key=>$val){
				if($key==0)
					$statusfilter = 'Status:(*'.trim($val).'*';
				else
					$statusfilter .= ' OR *'.trim($val).'*';
			}
			$statusfilter .= ')';
			array_push($filter, urlencode($statusfilter));
		}
	}
	
	private function setLanguageFilter(&$filter){
		if ($_REQUEST ['langtype']) {
			$lang = explode ( '|', $_REQUEST ['langtype'] );
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
		if(empty($Solr_User)){
			// Setting group filters
			require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user.php';
			$userObj = new UserSolrSearch();
			$userDetails = $userObj->getLoggedUserDetails();
		}else{
			$userDetails = $Solr_User;
		}
		$uid = $userDetails[0]->id;
		if(!isset($_REQUEST['tpmanage']))
			$manage = 'CreatedBy:'.$uid.' OR UpdatedBy:'.$uid.'';
		else{
			if($_REQUEST['tpmanage']){
				$filtervalues = explode('|',$_REQUEST['tpmanage']);
				expDebug::dPrint('manage filter set yes'.print_r($filtervalues,true),4);
				if(count($filtervalues) == 1){
					if($filtervalues[0] == 'cre_sys_fop_me')
						$manage = 'CreatedBy:'.$uid.' OR UpdatedBy:'.$uid.'';
					else if($filtervalues[0] == 'cre_sys_fop_oth')
						$manage = '-CreatedBy:'.$uid.' AND -CreatedBy:'.$uid.'';
				}
			}
		}
		if($manage)
			array_push($filter, urlencode($manage));
		
	}
	
	private function setPriceFilter(&$filter){
		if($_REQUEST['progPrice']){
			$price = str_replace('|',' TO ',$_REQUEST['progPrice']);
			$pricefilter = 'Price:['.$price.']';
			if($price[0] == 0) // Handles when Price value comes as null from table.This will be changed/removed when Course level pricing is introduced.
				$pricefilter .= ' OR *';
			array_push($filter, urlencode($pricefilter));
		}
	}
	
	private function setCurrencyFilter(&$filter){
		if($_REQUEST['tpcurrency']){
			$cur = explode('|',$_REQUEST['tpcurrency']);
			foreach($cur as $key=>$val){
				if($key==0)
					$statusfilter = 'CurrencyTypeCode:(*'.trim($val).'*';
				else
					$statusfilter .= ' OR *'.trim($val).'*';
			}
			$statusfilter .= ')';
			array_push($filter, urlencode($statusfilter));
		}
	}
	
	private function setCourseFilter(&$filter){
		if($_REQUEST['courseName']){
			$type = explode(' ',$_REQUEST['courseName']);
			foreach($type as $key=>$val){
				if($key==0)
					$typefilter = 'CourseTitle:*'.$val.'*';
				else
					$typefilter .= ' AND CourseTitle:*'.$val.'*';
			}
			array_push($filter, urlencode($typefilter));
		}
	}
	
	private function setSurveyFilter(&$filter){
		if($_REQUEST['surveyName']){
			expDebug::dPrint('Filter value entered is::'.print_r($_REQUEST['surveyName'],true),4);
			if(strpos($_REQUEST['surveyName'],' ') !== false){
			$type = explode(' ',$_REQUEST['surveyName']);
			foreach($type as $key=>$val){
				if($key==0)
					$typefilter = 'Survey:*'.$val.'*';
				else
					$typefilter .= ' AND Survey:*'.$val.'*';
			}
			}
			else {
				$typefilter = ' Survey:*'.$_REQUEST['surveyName'].'*';
			}
			array_push($filter, urlencode($typefilter));
		}
	}
	
	private function setAssmntFilter(&$filter){
		if($_REQUEST['assesmentName']){
			expDebug::dPrint('Filter value entered is::'.print_r($_REQUEST['assesmentName'],true),4);
			if(strpos($_REQUEST['assesmentName'],' ') !== false){
			$type = explode(' ',$_REQUEST['assesmentName']);
			foreach($type as $key=>$val){
				if($key==0)
					$typefilter = 'Assessment:*'.$val.'*';
				else
					$typefilter .= ' AND Assessment:*'.$val.'*';
			}
			}
			else {
				$typefilter = ' AND Assessment:*'.$_REQUEST['assesmentName'].'*';
			}
			array_push($filter, urlencode($typefilter));
		}
	}
	
	private function setTagFilter(&$filter){
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
		    $tag_name = 'tagname';
		}else {
			$tag_name = 'tagName';
		}
		if($_REQUEST[$tag_name]){
			$tg =  urldecode($_REQUEST[$tagName]);
			expDebug::dPrint('Filter value entered is::'.print_r($_REQUEST[$tag_name],true),4);
			$tagfilter = 'TagSearch:"*'.str_replace(' ', '`', $tg).'*"';
			array_push($filter, urlencode($tagfilter));
		}
	}
	private function setTextFilter(&$filter){
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
			$textfilter = 'ProgramTypeCode:"cre_sys_obt_trp"';
			array_push($filter,urlencode($textfilter));
		}
	}
	
	private function setAutoCompleteFilters(&$filter){
		$searchCurText = str_replace('"', '\"', $_REQUEST['z']);
		$textfilter = 'TextSearch:"'.$searchCurText.'" OR WordSearch:"'.$searchCurText.'"';
		array_push($filter, urlencode($textfilter));
		
		
		//Entity filter
		$entity_filter = 'ProgramTypeCode:cre_sys_obt_trp';
		array_push($filter, urlencode($entity_filter));
		// status filter
		$status = 'Status:(lrn_lpn_sts_atv OR lrn_lpn_sts_itv)';
		array_push($filter, urlencode($status));
	}
	
}

?>