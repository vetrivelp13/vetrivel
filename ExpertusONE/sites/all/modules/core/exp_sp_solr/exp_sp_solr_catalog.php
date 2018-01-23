<?php 
require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solrclient.php';

class CatalogSolrSearch extends SolrClient{
	function __construct(){
		// Get collection prefix from configuration
		$prefix = getConfigValue('collection_prefix');
	
		// Set exact collection name (should match with solr's collection)
		$this->collName = $prefix . '_' . SolrClient::CatalogCore;
	
		//Call parent constructor
		parent::__construct();
	}
    
	/**
	 * Fetch catalog list from Solr. Main method of catalog search.
	 * @return (Object) Result set
	 */

	
	public function getSolrSearchResult($page='',$userId=''){
		global $catalog_reg;
		// Set field mapping for class level search
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
			$catalog_reg = $_REQUEST['object_type'];
		}
		if($catalog_reg == 'Class'){
			$fieldList = array(
					'cls_id:EntityId',
					'cls_code:Code',
					'cls_title:Title',
					'cls_short_description:Description',
					'base_price:Price',
					'base_currency_type:CurrencyTypeCode',
					'registration_end_on:RecEndDate',
					'created_on:CreatedOn',
					'published_on:PublishedOn',
					'mro_id:mro',
					'user_mro_id:AcessUsermro',
					'course_id:CourseId',
					'start_date:ValidFrom',
					'export_compliance:Custom4',
					'expires_in_value:ValidityDays',
					'expires_in_unit:ValidityUnit',
					'prm_created_on:CreatedOn',
					'object_type:EntityTypeName',
					'prg_object_type:EntityType',
					'is_compliance:Compliance',
					'avgvote:Rating',
					'price:Price',
					'currency_symbol:CurrencyTypeCode',
					'currency_type:CurrencyTypeCode',
					'ses_start_date:SessionStart',
					
					
			);
			
			if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
				$apiFields = array(
					'base_currency_symbol:CurrencySymbol',
					'delivery_type_name:TypeName',
					'language_code:LangCode',
					'language:LangName',
					'location_id:LoctionId',
					'crs_id:CourseId',
					'delivery_type_code:Type',
					'tagname:TagName',
					'coursetagname:CourseTagName'
					
				);
				$fieldList = array_merge($fieldList,$apiFields);
			}
		}else{ // Setting field mapping for course level search
			$fieldList = array(
					'cls_id:EntityId',
					'cls_code:Code',
					'cls_title:Title',
					'cls_short_description:Description',
					'base_price:Price',
					'base_currency_type:CurrencyTypeCode',
					'registration_end_on:RegEndDate',
					'created_on:CreatedOn',
					'published_on:PublishedOn',
					'mro_id:mro',
					'user_mro_id:AcessUsermro',
					'course_id:CourseId',
					'start_date:ValidFrom',
					'export_compliance:Custom4',
					'expires_in_value:ValidityDays',
					'expires_in_unit:ValidityUnit',
					'prm_created_on:CreatedOn',
					'object_type:EntityTypeName',
					'prg_object_type:EntityType',
					'is_compliance:Compliance',
					'avgvote:Rating',
					'price:Price',
					'currency_symbol:CurrencyTypeCode',
					'currency_type:CurrencyTypeCode',
					
			);
			if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
				$apiFields = array(
					'language_code:LangCode',
					'language:LangName',
					'delivery_type_name:TypeName',
					'location_id:ClassLocationIds',
					'crs_id:ClassIds',
					'delivery_type_code:ClassTypeCodes',
					'cls_count:ClsCountNoDedi',
					'tagname:TagName'
				);
				$fieldList = array_merge($fieldList,$apiFields);
			}
			if(isset($_REQUEST['mobile_request'])){
				$mobileFields = array(
						'additional_info:AdditionalInfo',
						'addn_catalog_show:AddInfoShowCatalog'
				);
				$fieldList = array_merge($fieldList,$mobileFields);
			}
		}
	
	
		// Setting required filters
		$filter = array();
		$this->setAccessFilters($filter,$userId);
		$this->setSearchFilters($filter,$catalog_reg,$page);
		$results = $this->getCatalogDetails($filter,$fieldList,'GET',$page,$userId);			
		return $results;
	}

	public function CatalogSolrSearchAutoComplete(){
		global $catalog_reg;
		$fieldList = array('cls_title:Title');
		$filter = array();
		$this->setAccessFilters($filter);
		$this->setDefaultFilter($filter,$catalog_reg);
		$this->setSearchAutocompleteFilters($filter);		
		$results = $this->getCatalogDetailsAutoComplete($filter,$fieldList,'GET');			
		return $results;
	}

	private function setAccessFilters(&$filter,$userId){
		$filter = array();
		if($userId != '' || !empty($userId)){
			require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user.php';
			$userObj = new UserSolrSearch();
			$userDetails = $userObj->getLoggedUserDetails($userId);
		}
		else {
			if (! isset ( $_SESSION ['Solr_User'] ) || empty ( $_SESSION ['Solr_User'] )) {
				// Setting group filters
				require_once $_SERVER ['DOCUMENT_ROOT'] . '/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user.php';
				$userObj = new UserSolrSearch ();
				$userDetails = $userObj->getLoggedUserDetails ();
			} else {
				$userDetails = $_SESSION ['Solr_User'];
			}
		}
		$grpStr = '';
		$uid = $userDetails[0]->id;
		$user_hire = $userDetails[0]->HireDate;
		$user_name = $userDetails[0]->username;
		expDebug::dPrint ( 'Learner group for userss' . print_r ( $userDetails, true ), 4 );
		if($userDetails[0]->learner_groups != ''){
		$userAdminGrp = explode(',',$userDetails[0]->learner_groups);
		expDebug::dPrint ( 'Learner group for userss' . print_r ( $userDetails, true ), 4 );
		foreach($userAdminGrp as $key=>$val){
				if($key==0)
						$grpStr = 'LnrGroupName:"*'.trim(str_replace(' ','?',$val)).'*"';
				else
						$grpStr .= ' OR LnrGroupName:"*'.trim(str_replace(' ','?',$val)).'*"';
				}
				$grpStr .= ' OR (LnrGroupName:"*Open_Entity*" AND LnrAccessUserName:"*Open_Entity*")';
		}	
			
		else
			$grpStr = '(LnrGroupName:"*Open_Entity*" AND LnrAccessUserName:"*Open_Entity*")';
		
		if($user_name)
			$grpStr .= ' OR LnrAccessUserName:"*`'.$user_name.'`*"';
		$filter[] = urlencode($grpStr);
		if($user_hire){
			$hire_date_filter = $this->setHireDateFilter($user_hire);
			array_push($filter, urlencode($hire_date_filter));
		}
	}
	
	private function setHireDateFilter(&$user_hire){
		$hire_date_filter = '';
		$datetime1 = date_create($user_hire);
		$datetime2 = date_create(now());
		$interval = (date_diff($datetime1, $datetime2)->format("%R%a days"));
		if(strpos($interval,'+') !== false){
			preg_match_all('!\d+!', $interval, $days);
		$days_check = $days[0][0]-1;
		$hire_date_filter = '-HireDays:[1 TO '.$days_check.']';
		}
		return $hire_date_filter;
	}
    
	private function setSearchFilters(&$filter,&$catalog_reg,$page){
		// Setting required filters
		$this->setDefaultFilter($filter,$catalog_reg);
		$this->setTypeFilter($filter,$catalog_reg);
		$this->setMroFilter($filter,$page);
		$this->setCompExpiry($filter,$page);
	    if($page!='login'){
            $this->setLanguageFilter($filter);
        }
		$this->setCountryFilter($filter);
		$this->setPriceFilter($filter,$page);
		$this->setDateFilter($filter);
		$this->setTagFilter($filter,$catalog_reg); 
		$this->setLocationFilter($filter);
		$this->setRatingFilter($filter);
		$this->setJobRoleFilter($filter);
	//	$this->setCurrencyFilter($filter); // Commented due currency code is passed as parameter in API which is not needed for solr
		$this->setILTstatus($filter);
		$this->setTextSearchFilter($filter);
		$this->setCourseFilter($filter);
		$this->searchByEntity($filter);
		expDebug::dPrint('Registration level check'.print_r($filter,true),4);
	}
	
	
	
	private function setDefaultFilter(&$filter,&$catalog_reg){
		$entity_type = 'EntityTypeName:'.$catalog_reg.''; 
		$entity_type .= ' OR ProgramTypeCode:cre_sys_obt_trp';
		array_push($filter, urlencode($entity_type));
		if($catalog_reg == 'Class') 
			$status = 'lrn_cls_sts_atv';
		else
			$status = 'lrn_crs_sts_atv';
		$statusfilter = 'Status:(lrn_lpn_sts_atv OR '.$status.')';
		array_push($filter, urlencode($statusfilter));
		// Do not display class dedicated to TP
		$dedicated_class = '-DedicatedClass:Y';
		array_push($filter, urlencode($dedicated_class));
		}
		
	private function setDefaultPriceFilter(&$filter){
		    $nonpriced = '-Price:0';
		    array_push($filter, urlencode($nonpriced));
		}
	
	private function setTypeFilter(&$filter,$catalog_reg){
		$typefilter = '';
		$dlType = !empty($_REQUEST['dl_type']) ? explode('|',$_REQUEST['dl_type']) : array();
		$obType = !empty($_REQUEST['ob_type']) ? explode('|',$_REQUEST['ob_type']) : array();
		$type = array_merge($dlType,$obType);
		if($type){
			foreach ( $type as $key => $val ) {
				if ($key == 0){
					/* if($catalog_reg == 'Course' && isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname']))
						$typefilter = 'Type:(*' . trim ( $val ) . '*';
					else */
					$typefilter = 'Type:(*' . trim ( $val ) . '*';
				}else{
					$typefilter .= ' OR *' . trim ( $val ) . '*';
			}
			}
			$typefilter .= ')';
			array_push ( $filter, urlencode ( $typefilter ) );
		}
	}
	
	private function setMroFilter(&$filter,$page){
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname']) && $_REQUEST['compliance_mandatory'] == 1){
			$mro_id = 'cre_sys_inv_com|cre_sys_inv_man|'.$_REQUEST ['mro_type'].'';
		}
		else 
			$mro_id = $_REQUEST ['mro_type'];
		$mros = ($page == 'login'|| $page == 'mobilelogin') ? 'cre_sys_inv_com|cre_sys_inv_man' : $mro_id ;
		if ($mros) {
			$grpStr = '';
			$checkOR = 0;
			$mro_type = explode ( '|', $mros );
			if (in_array ( 'cre_sys_inv_com', $mro_type )) {
				$grpStr = 'Compliance:1';
				$checkOR = 1;
			}
			if (! isset ( $_SESSION ['Solr_User'] ) || empty ( $_SESSION ['Solr_User'] )) {
				require_once $_SERVER ['DOCUMENT_ROOT'] . '/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user.php';
				$userObj = new UserSolrSearch ();
				$userDetails = $userObj->getLoggedUserDetails ();
			} else {
				$userDetails = $_SESSION ['Solr_User'];
			}
			$username = $userDetails [0]->username;
			foreach ( $mro_type as $mro ){
				if ($mro != 'cre_sys_inv_com'){
					if ($key == 0 && $checkOR == 0){
						$grpStr .= ' AcessUsermro:"*`'.str_replace ( ' ', '?', $username ). '-' . $mro . '`*"';
						$checkOR = 1;
					}
					else 
						$grpStr .= ' OR AcessUsermro:"*`'.str_replace ( ' ', '?', $username ). '-' . $mro . '`*"';
				}
			}
			
			if($userDetails [0]->learner_groups != ''){
				$userAdminGrp = explode ( ',', $userDetails [0]->learner_groups );
				foreach ( $userAdminGrp as $key => $val ) {
					$val = trim ( substr ( $val, 0, - 1 ) );
					foreach ( $mro_type as $mro ) {
						if ($mro != 'cre_sys_inv_com') {
							$grpStr .= ' OR mro:*' . str_replace ( ' ', '?', $val ) . '-' . $mro . '`*';
						}
					}
				}
			}
		/*	else if($checkOR != 1)
				$grpStr .= '-mro:*';
	        */
	        array_push ( $filter, urlencode ($grpStr));
			expDebug::dPrint ( 'mro filter entered ---- >' . print_r ( $grpStr, true ), 4 );
		}
	}
	private function setCompExpiry(&$filter,$page){
		if($page== 'login' || $page == 'mobilelogin'){
			$PastDatefilter = '-CompleteDate:[* TO NOW-1DAY] OR -ValidityDate:[* TO NOW-1DAY]';
			array_push($filter, urlencode($PastDatefilter));
		}
	}
	
	
	private function setPriceFilter(&$filter,$page){
		if($_REQUEST['price']){
			/*$price = str_replace('$,','',$_REQUEST['price']);
			$price = str_replace('$','',$price);
			$price = str_replace('-',' TO ',$price);*/
			$priceExp 	 = explode("-", $_REQUEST['price']);
    	$price_start   = preg_replace("/[^0-9]/", '', $priceExp[0]);
    	$price_end 	 = preg_replace("/[^0-9]/", '', $priceExp[1]);
    	$price = $price_start . " TO " . $price_end;
			$pricefilter = 'Price:['.$price.']';
			array_push($filter, urlencode($pricefilter));
		}
		if($_REQUEST['price_filter']){
			$price = str_replace(',',' TO ',$_REQUEST['price_filter']);
			$pricefilter = 'Price:['.$price.']';
			array_push($filter, urlencode($pricefilter));
		}
		if($page == 'login' || $page == 'mobilelogin'){
			$price = 'Price:0';
			array_push($filter, urlencode($price));
		}
	}
	
	private function setDateFilter(&$filter){
		if ($_REQUEST ['startdate'] || $_REQUEST ['enddate']) {
			if ($_REQUEST ['startdate']) {
				$start_date = date_format ( date_create_from_format ( 'm-d-Y', $_REQUEST ['startdate'] ), 'Y-m-d' );
				$start_date .= 'T00:00:00Z';
			} else
				$start_date = '*';
			if ($_REQUEST ['enddate']) {
				$end_date = date_format ( date_create_from_format ( 'm-d-Y', $_REQUEST ['enddate'] ), 'Y-m-d' );
				$end_date .= 'T00:00:00Z';
			} else
				$end_date = '*';
			$date_range = '' . $start_date . ' TO ' . $end_date . '';
			$datefilter = 'SessionStart:[' . $date_range . '] OR SessionStarts:[' . $date_range . ']';
			array_push ( $filter, urlencode ( $datefilter ) );
		}
	}
	
	private function setLocationFilter(&$filter){
				if($_REQUEST['location']){
					$location   = preg_replace("/[^a-zA-z]/", ' ', $_REQUEST['location']);
					expDebug::dPrint ( 'location filter entered ---- >' . print_r ( $location, true ), 4 );
					$location = preg_replace('!\s+!', ' ', $location);
					expDebug::dPrint ( 'location filter entered ---- >:::' . print_r ( $location, true ), 4 );
					$type = explode(' ',$location);
					$i = 0;
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
	
	private function setRatingFilter(&$filter){
		if($_REQUEST['rating_type']){
			$rating = explode('|',$_REQUEST['rating_type']);
			foreach($rating as $key=>$val){
				$val = ($val == 20) ? '[1 TO 20]' :(($val == 40) ? '[21 TO 40]' : (($val == 60) ? '[41 TO 60]' : (($val == 80) ? '[61 TO 80]' : (($val == 100)? '[81 TO 100]': ''))));
				if($key==0)
					$ratingfilter = 'Rating:'.$val.'';
				else
					$ratingfilter .= ' OR Rating:'.$val.'';
			}
			array_push($filter, urlencode($ratingfilter));
		}
	}
	
	private function setJobRoleFilter(&$filter){
		if($_REQUEST['jr_type']){
			$jobrole = explode('|',$_REQUEST['jr_type']);
			foreach($jobrole as $key=>$val){
				expDebug::dPrint("Solr search result - Final1233 ".print_r($val,1),4);
				if($key==0)
					$jobrole = 'LnrGroupJobRole:"*'.$val.'*"';
				else
					$jobrole .= ' OR LnrGroupJobRole:"*'.$val.'*"';
			}
			array_push($filter, urlencode($jobrole));
		}
	}
	
	private function setCurrencyFilter(&$filter){
		if($_REQUEST['currency_code']){
			$currency_filter = 'CurrencyCode:*'.$_REQUEST['currency_code'].'*';
			array_push($filter, urlencode($currency_filter));
		}
	}
	
	private function setILTstatus(&$filter){
		if($_REQUEST['cls_status']){
			$date = str_replace(' ', 'T', now());
			$date .= 'Z';
			expDebug::dPrint("Solr search result - Final1233 ".print_r($date,1),4);
			if($_REQUEST['cls_status'] == 'Scheduled')
				$session = 'SessionStart:['.$date.' TO *]';
			else if($_REQUEST['cls_status'] == 'Delivered')
				$session = 'SessionStart:[* TO '.$date.']';
			array_push($filter, urlencode($session));
		}
	}
	private function setCourseFilter(&$filter){
		expDebug::dPrint("Solr search result - Final1233 ".print_r($_REQUEST['course_id'],1),4);
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname']) && $_REQUEST['course_id']){
			$catalog = $_REQUEST['object_type'];
			if( $catalog == 'Class')
				$crs_filter = 'CourseId:'.$_REQUEST['course_id'].'';
			else 
				$crs_filter = 'EntityId:'.$_REQUEST['course_id'].'';
			array_push($filter, urlencode($crs_filter));
		}
	}
	private function setTextSearchFilter(&$filter){
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname']) && $_REQUEST['course_code']){
			$_REQUEST['course_code'] = str_replace('"', '\"', $_REQUEST['course_code']);
			$codefilter = 'TextSearch:"'.$_REQUEST['course_code'].'" OR WordSearch:"'.$_REQUEST['course_code'].'"';
			array_push($filter, urlencode($codefilter));
		}
		if($_REQUEST['title']){
		$textsearch = str_replace('"', '\"', $_REQUEST['title']);
		$textfilter = 'TextSearch:"'.$textsearch.'" OR WordSearch:"'.$textsearch.'"';
		array_push($filter, urlencode($textfilter));

		}
	}
	
	private function searchByEntity(&$filter){
		if($_REQUEST['ent_type']){
			$textfilter = 'EntityType:"'.$_REQUEST['ent_type'].'"';
			array_push($filter,urlencode($textfilter));
		}
		if($_REQUEST['ent_id']){
			$textfilter = 'EntityId:"'.$_REQUEST['ent_id'].'"';
			array_push($filter,urlencode($textfilter));
		}
	}
	
	private function setLanguageFilter(&$filter){
		global $Solr_User;
		if(empty($Solr_User)){
			$results = $this->getLoggedUserDetails();
		}else{
			$results = $Solr_User;
		}
		$language = (!isset($_REQUEST['lg_type']) && (!isset($_REQUEST['apiname']) || empty($_REQUEST['apiname'])))?$results[0]->preferredlanguagecode:''; // Deafult language
		if(!empty($language)) {
			$lng = 'LangCode:'.$language;
			array_push($filter, urlencode($lng));
		}
		if($_REQUEST['lg_type']){
			$lang = explode('|',$_REQUEST['lg_type']);
			foreach($lang as $key=>$val){
				if($key==0)
					$langfilter = 'LangCode:(*'.trim($val).'*';
				else
					$langfilter .= ' OR *'.trim($val).'*';
			}
			$langfilter .= ')';
			array_push($filter, urlencode($langfilter));
		}
	}
	
	private function setCountryFilter(&$filter){
		if($_REQUEST['cy_type']){
			$country = explode('|',$_REQUEST['cy_type']);
			foreach($country as $key=>$val){
				if($key==0)
					$countryfilter = 'LocationCountryCode:(*'.trim($val).'*';
				else
					$countryfilter .= ' OR *'.trim($val).'*';
			}
			$countryfilter .= ')';
			array_push($filter, urlencode($countryfilter));
			
		}
	}
   private function setTagFilter(&$filter ,&$catalog_reg){
   		expDebug::dPrint("tag filter entered".$_REQUEST['tag']);
   		if($_REQUEST['tag']){
   			$tg = (isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])) ? $_REQUEST['tag'] : urldecode($_REQUEST['tag']);
   			if($tg != ''){
   				$tag = 'TagSearch:"*'.str_replace(' ','`',$tg).'*"';
   				if($catalog_reg == 'Class')
   					$tag .= ' OR CourseTagName:"*'.str_replace(' ','`',$tg).'*"';
   		array_push($filter,urlencode($tag));
   	}
   	}
   }
	
	private function setSearchAutocompleteFilters(&$filter,&$catalog_reg){
	    if($_REQUEST['z']){
		$searchCurText =  str_replace('"', '\"', $_REQUEST['z']);
		$textfilter = 'TextSearch:"'.$searchCurText.'" OR WordSearch:"'.$searchCurText.'"';

		array_push($filter, urlencode($textfilter));
	    }
	}
	private function getCatalogDetailsAutoComplete ($filter,$fieldList,$method){
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
			expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
		}
	}
	private function getCatalogDetails($filter,$fieldList,$method,$page,$userId){
		// List of required fields to be set as NULL,
		// because Solr will not keep any null fields
		$resBase = array(
				'cls_id' => NULL,
				'cls_code' => NULL,
				'cls_title' => NULL,
				'cls_short_description' => NULL,
				'base_price' => NULL,
				'base_currency_type' => NULL,
				'registration_end_on' => NULL,
				'created_on' => NULL,
				'published_on' => NULL,
				'mro_id' => NULL,
				'course_id' => NULL,
				'start_date' => NULL,
				'export_compliance' => NULL,
				'expires_in_value' => NULL,
				'expires_in_unit' => NULL,
				'prm_created_on' => NULL,
				'object_type' => NULL,
				'is_compliance' => NULL,
				'avgvote' => NULL,
				'price' => NULL,
				'currency_symbol' => NULL,
				'currency_type' => NULL,
				'ses_start_date'=> NULL,
				'prg_object_type'=> NULL,
				'user_mro_id'=>NULL
		);
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
			$apiFields = array(
				'base_currency_symbol'=> NULL,
				'delivery_type_name'=> NULL,
				'language_code'=> NULL,
				'language'=> NULL,
				'location_id'=> NULL,
				'crs_id'=> NULL,
				'delivery_type_code'=> NULL,
				'tagname'=> NULL,
				'coursetagname'=> NULL
			);
			$resBase = array_merge($resBase,$apiFields);
			if(isset($_REQUEST['mobile_request'])){
				$mobileFields = array(
						'additional_info'=> NULL,
						'addn_catalog_show'=> NULL
				);
				$resBase = array_merge($resBase,$mobileFields);
			}
			$dispCols=explode(',',$_REQUEST['display_cols']);
			array_push($dispCols,"prg_object_type");
			if(!in_array('mro_id',$dispCols)){
				array_push($dispCols,"mro_id");
			}
			array_push($dispCols,"user_mro_id");
			foreach($fieldList as $field=>$val){
				$acFld = explode(':',$val);
				if(!in_array($acFld[0],$dispCols) && $acFld[0]!= "coursetagname"  ){
					unset($fieldList[$field]);
					unset($resBase[$acFld[0]]);
				}
			}
		}
		
		// Append field list
		$data = 'fl=' . implode(',',$fieldList);
		// Append filters
		$data .= '&fq=' . implode('&fq=',$filter);
		//Pagination
		$start = ($_REQUEST['page']*$_REQUEST['rows'])-10;
		
		//Sort by
		if($_REQUEST['sortby'] && $_REQUEST['sortby'] != 'undefined'){
			if($_REQUEST['sortby'] == 'ZA')
				$sort = 'TitleSrt+desc';
			if($_REQUEST['sortby'] == 'AZ')
				$sort = 'TitleSrt+asc';
			else if($_REQUEST['sortby'] == 'Time')
				$sort='UpdatedOn+desc';
			else if($_REQUEST['sortby'] == 'ClassStartDate')
				$sort = 'SessionStart+asc';
			else if($_REQUEST['sortby'] == 'Mandatory')
				$sort = 'mro+asc';
		}
		else
			$sort = 'TitleSrt+asc';
	
		$sort .= empty($sort) ? 'EntityId+asc' : ',EntityId+asc' ;  
		$rows = 10;
		if($page == 'login' || $page == 'mobilelogin')
			$rows = 1000;
		
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
			$start = ($_REQUEST['page']*$_REQUEST['limit'])-$_REQUEST['limit'];
			$rows = (!empty($_REQUEST['limit']) && $page != 'mobilelogin' ) ? $_REQUEST['limit'] : $rows;
		}// Default, needs to be append with given sort
		
		$start = $start < 0 ? 0 : $start;
		$data .= '&sort='.$sort.'&indent=on&q=*:*&start='.$start.'&rows='.$rows;
		
		try{
			$srcRst = $this->getData($this->collName,$data,'GET');
			$this->modifyResultSet($srcRst,$userId);
			//$srcRst = drupal_json_decode($srcRst);
			$srcRst1 = $srcRst['response']['docs'];
			$srcRst2 = array();
			$srcRst2['recCount'] = $srcRst['response']['numFound'];
			foreach($srcRst1 as $doc){
				$tdoc  = array_merge($resBase, $doc);
				$tdoc = $this->removeTilt($tdoc);
				$srcRst2['records'][] = $this->arrayToObject($tdoc);
			}
	
			expDebug::dPrint("Solr search result - Final ".print_r($srcRst2,1),4);
			expDebug::dPrint("Solr search result - Final Count".print_r($srcRst['response']['numFound'],1),4);
			return $srcRst2;
		}catch(Exception $e){
			expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
		}
	
	}

	private function modifyResultSet(&$srcRst,$userId,$page){
		global $Solr_User;
		//$this->setMRO($srcRst);
		//$this->userCurrency($srcRst);
		if($userId != '' || !empty($userId)){
			require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user.php';
			$userObj = new UserSolrSearch();
			$user = $userObj->getLoggedUserDetails($userId);
		}
		else if(empty($Solr_User)){
			$user = $this->getLoggedUserDetails();
		} else {
			$user = $Solr_User;
		}
		$obj = $this;
		if($this->responseType == 'json'){
			foreach($srcRst['response']['docs'] as $key=>&$doc){
				$obj->SwapPrgCode($doc);
				$obj->setMRO($doc,$user);
				$obj->userCurrency($doc,$page);
				$obj->regEndDate($doc);
				$obj->SetClassTag($doc);
				if($_REQUEST['object_type'] == 'Course'){
					$obj->resetMultiFields($doc);
			}
			}
		}elseif($this->responseType == 'xml'){
			try{
			$obj = $this;
			$recs = $this->apiResponseHandler($srcRst,'lnrcatalog');
			$dom = new DOMDocument();
			$dom->loadXML($recs);
			$res = $dom->getElementsByTagName('mro_id');
			foreach($res as $rs){
				if(!empty($rs->nodeValue))
					$rs->nodeValue = $obj->resetMRO($rs->nodeValue,$userId);
			}
			$srcRst = $dom->saveXML();
			}catch(Exception $e){
				expDebug::dPrint("Error in parsing XML data -- ".print_r($e,true),1);
			}
		}
	}
	
	private function resetMultiFields(&$doc){
		$crsId = $doc['crs_id'][0];
		unset($doc['crs_id'][0]);
		$doc['crs_id']=$crsId;
		
		$dlCode = $doc['delivery_type_code'][0];
		unset($doc['delivery_type_code'][0]);
		$doc['delivery_type_code'] = $dlCode;
	}
	
	private function setMRO(&$doc,$user){
		if($this->responseType == 'json'){
			if(isset($doc['user_mro_id']) && !empty($doc['user_mro_id'])){
				$user_mro = $this->resetUserMRO($doc['user_mro_id'],$user);
				if($user_mro != '')
					$doc['mro_id'] = $user_mro;
				else {
					if(isset($doc['mro_id']) && !empty($doc['mro_id'])){
						$doc['mro_id'] = $this->resetMRO($doc['mro_id'],$user);
					}
				}
			}
			else {
				if(isset($doc['mro_id']) && !empty($doc['mro_id'])){
					$doc['mro_id'] = $this->resetMRO($doc['mro_id'],$user);
				}
			}
		}
	}
	private function SwapPrgCode(&$doc){
	if($doc['object_type']=='Curricula' || $doc['object_type']=='Learning Plan' || $doc['object_type']=='Certification' ){
					$prgType = $doc['object_type'];
					$doc['object_type'] = $doc['prg_object_type'];
					unset($doc['prg_object_type']);
				}
	}
	private function resetUserMRO($usermro,$user){
		$mroArr = explode(',',$usermro);
		$morRst = array();
		$access = '';
		foreach($mroArr as $mro){
			$mro = trim($mro,'`');
			$user_access = explode('-',$mro);
			if($user[0]->username == $user_access[0])
				$access = $user_access[1];
			
		}
		expDebug::dPrint("Final accessss 1233 ".print_r($access,true),3);
		return $access;
	}
	private function resetMRO($mrostr,$user){
		$mroArr = explode(',',$mrostr);
		$morRst = array();
		foreach($mroArr as $mro){
			$mroidArr = explode('-',$mro);
			$m1 = end($mroidArr);
			unset($mroidArr[sizeOf($mroidArr)-1]);
			$g1 = implode('-',$mroidArr);
			if(stripos($user[0]->learner_groups,$g1)!==false)
				array_push($morRst,str_replace('`','',$m1));
		}
		if(in_array('cre_sys_inv_man',$morRst)) return 'cre_sys_inv_man';
		if(in_array('cre_sys_inv_rec',$morRst)) return 'cre_sys_inv_rec';
		if(in_array('cre_sys_inv_opt',$morRst)) return 'cre_sys_inv_opt';
	}

	private function userCurrency(&$doc,$page=''){
			//if(empty($_SESSION['SolrData']['UserCurrency'])){
		if($page == 'order' && (isset($_SESSION['admin_shop_cart_currency_code']) && !empty($_SESSION['admin_shop_cart_currency_code']))){
			$UserPrefCurrCode = $_SESSION['admin_shop_cart_currency_code'];
			$UserPrefCurrSym = $_SESSION['admin_shop_cart_currency_sym'];
		} else {
				$UserDefaultCurrencyArr = (isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname']))? getUserDefaultCurrency(1): getUserDefaultCurrency();
			$UserPrefCurrCode = $UserDefaultCurrencyArr['currency_code'];
			$UserPrefCurrSym = $UserDefaultCurrencyArr['currency_sym'];
		}
			if(isset($doc['price']) && !empty($doc['price'])){
				
				$price = db_query('SELECT slf_convert_price(:price,:type,:code) AS price',
							array(':price'=>$doc['price'],':type'=>$doc['currency_type'],':code'=>$UserPrefCurrCode))->fetchField();
		  	
		  		$UserPrefCurrLongCode = $UserDefaultCurrencyArr['currency_long_code'];
		  		
		  		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
		  			$doc['price'] = $price;
		  		}
		  		else {
		  			$doc['price'] = $price;
		  			$doc['base_price'] = $price;
		  		}
		  		$doc['currency_symbol'] = $UserPrefCurrSym;
		  		$doc['currency_type'] = $UserPrefCurrCode;
	    }else{
				if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
				 $doc['price'] = '0.00';
				 $doc['base_price'] = '0.00';
				 $doc['currency_symbol'] = $UserPrefCurrSym;
				 $doc['currency_type'] = $UserPrefCurrCode;
				}
			}
	}
	
	private function regEndDate(&$doc) {
		$reg_end_date = str_replace('T',' ',$doc['registration_end_on']);
		$reg_end_date = str_replace('Z','',$reg_end_date);
		$doc['registration_end_on'] = $reg_end_date;
	}
	
	/** Attach course **/
	
	public function getSolrSearchResultTPAttachCourse($excludedCourseIdArray){
	    global $catalog_reg;
	    $fieldList = array(
	            'id:EntityId',
	            'title:Title',
	            'code:Code',
	            'Privilege',
	            'CreatedBy',
	            'UpdatedBy'
	    );
	    expDebug::dPrint('excludedCourseIdArray keyvaluesssss getSolrSearchResultTPAttachCourse'.print_r($excludedCourseIdArray,true),4);
	    // Setting required filters
	    $filter = array();
	    $this->setAdminAccessFilters($filter);
	    $this->setSearchFiltersTPAttachCourse($filter,$excludedCourseIdArray);
	    $results = $this->getCatalogDetailsTPAttachCourse($filter,$fieldList,'GET');
	    return $results;
	}
	
	private function setSearchFiltersTPAttachCourse(&$filter,$excludedCourseIdArray){
	    // Setting required filters
	    $this->setDefaultFilterTPAttachCourse($filter,$excludedCourseIdArray);
	    $this->setTextSearchFilterTP($filter);
	}
	
	private function setDefaultFilterTPAttachCourse(&$filter,$excludedCourseIdArray){
	    $entity_type = 'EntityTypeName:'.Course.'';
	    array_push($filter, urlencode($entity_type));
	    $statusfilter = 'Status:lrn_crs_sts_atv';
	    array_push($filter, urlencode($statusfilter));
	    $excludedIds = explode(",",$excludedCourseIdArray);
	    if($excludedIds != 0 && !empty($excludedIds)) {
		    foreach($excludedIds as $key => $value)
		    {
		        if($key == 0){
		            $excludedcourse = '-EntityId: '.$value.'';
		        }
		        else {
		            $excludedcourse .= ' AND -EntityId: '.$value.'';
		        }
		    }
		    array_push($filter, urlencode($excludedcourse));
	    }
	}
	
	private function setTextSearchFilterTP(&$filter){
		$reqData = $_REQUEST['q'];
	    if($reqData){
	        expDebug::dPrint('q filter value'.print_r($_REQUEST['q'],true),4);
	        $data = explode('/',$_REQUEST['q']);
	
	        expDebug::dPrint('data filter value'.print_r($data,true),4);
	        $textfiltervalue = $data[4];
	        expDebug::dPrint('text filter value'.print_r($textfiltervalue,true),4);
	        $default_text_title = t('LBL088').' '.t('LBL083').' '.t('LBL644').' '.t('LBL096');
	        if(!empty($textfiltervalue) && $textfiltervalue!='' && $textfiltervalue!=$default_text_title){
	                    $textfilter = 'TextSearch:"'.$textfiltervalue.'" OR WordSearch:"'.$textfiltervalue.'"';
	                array_push($filter, urlencode($textfilter));
	        }
	    }
	}
	public function attachCourseAutoComplete($excludedCourseId){
		$fieldList = array('title:Title');
		$filter = array();
    	$this->setAdminAccessFilters($filter);
		$this->setSearchFiltersTPAttachCourse($filter,$excludedCourseId);
		$this->setSearchAutocompleteFiltersTP($filter);
		$results = $this->getAttachCourseDetailsAutoComplete($filter,$fieldList,'GET');
		return $results;
	}
	private function getAttachCourseDetailsAutoComplete ($filter,$fieldList,$method){
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
				$this->modifyResultSetTPAttachCourse($tdoc);
				$srcRst2['records'][] = $this->arrayToObject($tdoc);
			}
	
			expDebug::dPrint("Solr search result - Final ".print_r($srcRst2,1),4);
			expDebug::dPrint("Solr search result - Final Count".print_r($srcRst['response']['numFound'],1),4);
			return $srcRst2;
		}catch(Exception $e){
			expDebug::dPrint("Error in TP Attach course autocomplete Search -- ".$e->getMessage(),1);
		}
	}
	private function setSearchAutocompleteFiltersTP(&$filter,&$catalog_reg){
	    $searchCurText =  str_replace('"', '\"', $_REQUEST['z']);
	    if(!empty($searchCurText)){
	            $textfilter = 'TextSearch:"'.$searchCurText.'" OR WordSearch:"'.$searchCurText.'"';
	        array_push($filter, urlencode($textfilter));
	    }
	}
	private function getCatalogDetailsTPAttachCourse($filter,$fieldList,$method){
		global $Solr_User;
	    // List of required fields to be set as NULL,
	    // because Solr will not keep any null fields
	    $resBase = array(
	            'id' => NULL,
	            'code' => NULL,
	            'title' => NULL,
	            'sumedit' => NULL,
	            'sumdelete' => NULL
	    );
	
	    // Append field list
	    $data = 'fl=' . implode(',',$fieldList);
	    // Append filters
	    $data .= '&fq=' . implode('&fq=',$filter);
	    //Pagination
	     
	     
	    $rows = ($_REQUEST['rows']) ? $_REQUEST['rows'] : 10;
	    $start = ($_REQUEST['page']*$_REQUEST['rows'])-$rows;
	    $start = $start > 0 ? $start : 0;
	    expDebug::dPrint("no of rows".print_r($_REQUEST['rows'],1),4);
	    expDebug::dPrint("page number ".print_r($_REQUEST['page'],1),4);
	    expDebug::dPrint("start value ".print_r($start,1),4);
	    $sidx = $_REQUEST['sidx'];
	    $sord = $_REQUEST['sord'];
	    if(!empty($sidx) && $sidx != 'undefined') {
	    	if($sidx == 'title') {
	    		$sort = 'TitleSrt+'.$sord;
	    	}
	    	if($sidx == 'code') {
	    		$sort = 'CodeSrt+'.$sord;
	    	}
	    	
	    }else {
			$sort = 'TitleSrt+desc';
		} 
	    expDebug::dPrint("Sort detail12345 ".print_r($sort,1),4);
	     
	    $data .= '&sort='.$sort.'&indent=on&q=*:*&start='.$start.'&rows='.$rows;
	    expDebug::dPrint("data detail12345 ".print_r($data,1),4);
	
	    // Logged in users details
	    if(empty($Solr_User)){
	        $results = $this->getLoggedUserDetails();
	    }else{
	        $results = $Solr_User;
	    }
	    try{
	        $srcRst = $this->getData($this->collName,$data,'GET');
	        $srcRst1 = $srcRst['response']['docs'];
	        $srcRst2 = array();
	        $srcRst2['recCount'] = $srcRst['response']['numFound'];
	        foreach($srcRst1 as $doc){
	            $tdoc  = array_merge($resBase, $doc);
	            $this->modifyResultSetTPAttachCourse($tdoc,$results);
	            $srcRst2['records'][] = $this->arrayToObject($tdoc);
	        }
	
	        expDebug::dPrint("Solr search result - Final ".print_r($srcRst2,1),4);
	        expDebug::dPrint("Solr search result - Final Count".print_r($srcRst['response']['numFound'],1),4);
	        return $srcRst2;
	    }catch(Exception $e){
	        expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
	    }
	
	}
	private function modifyResultSetTPAttachCourse(&$tdoc,$results){
		
	    // Set Edit & Delete privilege
	    expDebug::dPrint('Call to priviledge function'.print_r($results,true),4);
	    $this->setPrivileges($tdoc,$results,'cre_sys_obt_crs');
	}
	/** Attach course end **/
	
	
	/** Order add trainings starts here **/
	
	public function getSolrSearchResultMyOrder($user_id){
	    global $catalog_reg;
	    $fieldList = array(
	            'cls_id:EntityId',
	            'cls_code:Code',
	            'cls_title:Title',
	            'cls_short_description:Description',
	            'price:Price',
	            'currency_type:CurrencyTypeCode',
	            'registration_end_on:RecEndDate',
	            'created_on:CreatedOn',
	            'published_on:PublishedOn',
	            'currency_code:CurrencyCode',
	            'currency_symbol:CurrencySymbol',
	            'currency_long_code:CurrencyCode',
	            'language_code:LangCode',
	            'language:LangName',
	            'start_date:SessionStartDate',
	            'location_id:LoctionId',
	            'crs_id:CourseId',
	            'delivery_type_code:Type',
	            'delivery_type_name:TypeName',
	            'export_compliance:Custom4',
	            'expires_in_value:ValidityDays',
	            'expires_in_unit:ValidityUnit',
	            'prm_created_on:CreatedOn',
	            'object_type:Type',
	            'is_compliance:Compliance',
	            'avgvote:Rating',
	    );
	
	    // Setting required filters
	    $filter = array();
	   // $this->setAccessFiltersMyOrder($filter,$user_id);
	    $this->setAdminAccessFilters($filter);
	    $this->setSearchFiltersMyOrder($filter,'Class');
	    $results = $this->getCatalogDetailsMyOrder($filter,$fieldList,'GET');
	    return $results;
	}
	
	private function setAccessFiltersMyOrder(&$filter,&$uid){
	    $filter = array();
	    // Setting group filters
	    require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user.php';
	    $userObj = new UserSolrSearch();
	    $userDetails = $userObj->getLoggedUserDetails($uid);
	    $grpStr = '';
	    $user_hire = $userDetails[0]->HireDate;
	    $user_name = $userDetails[0]->username;
	    expDebug::dPrint ( 'Learner group for userss' . print_r ( $userDetails, true ), 4 );
	
	    if($userDetails[0]->learner_groups != ''){
	        $userAdminGrp = explode(',',$userDetails[0]->learner_groups);
	        expDebug::dPrint ( 'Learner group for userss' . print_r ( $userDetails, true ), 4 );
	        foreach($userAdminGrp as $key=>$val){
	            if($key==0)
	                $grpStr = 'LnrGroupName:"*'.trim(str_replace(' ','?',$val)).'*"';
	            else
	                $grpStr .= ' OR LnrGroupName:"*'.trim(str_replace(' ','?',$val)).'*"';
	        }
	        $grpStr .= ' OR (LnrGroupName:"*Open_Entity*" AND LnrAccessUserName:"*Open_Entity*")';
	    }
	    else
	        $grpStr = '(LnrGroupName:"*Open_Entity*" AND LnrAccessUserName:"*Open_Entity*")';
	
	    if($user_name)
	        $grpStr .= ' OR LnrAccessUserName:"*`'.$user_name.'`*"';
	    $filter[] = urlencode($grpStr);
	    if($user_hire){
	        $hire_date_filter = $this->setHireDateFilter($user_hire);
	        array_push($filter, urlencode($hire_date_filter));
	    }
	}
	
	private function setSearchFiltersMyOrder(&$filter,$catalog_reg){
	    // Setting required filters
	    $this->setDefaultFilter($filter,$catalog_reg);
	    $this->setDefaultPriceFilter($filter);
	    $this->setTypeFilter($filter,$catalog_reg);
	    $this->setTextSearchFilter($filter);
	    $this->setSearchAutocompleteFilters($filter);
	    expDebug::dPrint('Registration level check'.print_r($filter,true),4);
	}
	
	private function getCatalogDetailsMyOrder($filter,$fieldList,$method){
	    // List of required fields to be set as NULL,
	    // because Solr will not keep any null fields
	    $resBase = array(
	            'cls_id' => NULL,
	            'cls_code' => NULL,
	            'cls_title' => NULL,
	            'cls_short_description' => NULL,
	            'price' => NULL,
	            'currency_type' => NULL,
	            'registration_end_on' => NULL,
	            'created_on' => NULL,
	            'published_on' => NULL,
	            'currency_code' => NULL,
	            'currency_symbol' => NULL,
	            'currency_long_code' => NULL,
	            'language_code' => NULL,
	            'language' => NULL,
	            'location_id' => NULL,
	            'crs_id' => NULL,
	            'delivery_type_code' => NULL,
	            'delivery_type_name' => NULL,
	            'start_date' => NULL,
	            'export_compliance' => NULL,
	            'expires_in_value' => NULL,
	            'expires_in_unit' => NULL,
	            'prm_created_on' => NULL,
	            'object_type' => NULL,
	            'is_compliance' => NULL,
	            'avgvote' => NULL
	
	    );
	
	    // Append field list
	    $data = 'fl=' . implode(',',$fieldList);
	    // Append filters
	    $data .= '&fq=' . implode('&fq=',$filter);
	    //Pagination
	    $rows = !empty($_REQUEST['rows']) ? $_REQUEST['rows'] : 10;
	    $start = ($_REQUEST['page']*$_REQUEST['rows'])-$rows;
	
	    //Sort by
	    if($_REQUEST['sord'] && $_REQUEST['sord'] != 'undefined'){
	        if($_REQUEST['sord'] == 'asc')
	            $sort = 'TitleSrt+asc';
	        if($_REQUEST['sord'] == 'desc')
	            $sort = 'TitleSrt+desc';
	    }
	
	    expDebug::dPrint("Sort detail ".print_r($sort,1),4);
	    $sort .= empty($sort) ? 'EntityId+asc' : ',EntityId+asc' ;
	
	    expDebug::dPrint("Sort detail12345 ".print_r($sort,1),4);
	    if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
	        $start = ($_REQUEST['page']*$_REQUEST['limit'])-$_REQUEST['limit'];
	        $rows = !empty($_REQUEST['limit']) ? $_REQUEST['limit'] : $rows;
	
	    }// Default, needs to be append with given sort
	    $start = $start < 0 ? 0 : $start;
	    $data .= '&sort='.$sort.'&indent=on&q=*:*&start='.$start.'&rows='.$rows;
	    expDebug::dPrint("data detail12345 ".print_r($data,1),4);
	    try{
	        $srcRst = $this->getData($this->collName,$data,'GET');
	        expDebug::dPrint("Solr search result - Final ".print_r($srcRst,1),4);
	
	        $this->modifyResultSet($srcRst,'','order');
	        //$srcRst = drupal_json_decode($srcRst);
	
	        $srcRst1 = $srcRst['response']['docs'];
	        $srcRst2 = array();
	        $srcRst2['recCount'] = $srcRst['response']['numFound'];
	        foreach($srcRst1 as $doc){
	            $tdoc  = array_merge($resBase, $doc);
	            $tdoc = $this->removeTilt($tdoc);
	            $srcRst2['records'][] = $this->arrayToObject($tdoc);
	        }
	
	        expDebug::dPrint("Solr search result - Final ".print_r($srcRst2,1),4);
	        expDebug::dPrint("Solr search result - Final Count".print_r($srcRst['response']['numFound'],1),4);
	        return $srcRst2;
	    }catch(Exception $e){
	        expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
	    }
	
	}
	/** Order add trainings ends here **/
	
	
	/** My team assign learning starts here **/
	
	public function getSolrSearchResultMyteam($user_id){
	    global $catalog_reg;
	    $fieldList = array(
	            'object_type:Type',
	            'crs_id:CourseId',
	            'cls_id:EntityId',
	            'cls_code:Code',
	            'cls_title:Title',
	            'cls_short_description:Description',
	            'delivery_type_code:Type',
	            'currency_type:CurrencyTypeCode',
	            'export_compliance:Custom4',
	            'delivery_type_name:TypeName',
	            'status:StatusName',
	            'language:LangName',
	            'location:ClassLocationNameList', // need to modify in result set since locationaddress comes as array for TP -- latha
	            'node_id:NodeId',
	            'price:Price',
	            'session_id:SessionId',
	            'sess_start_date:SessionStartDate',
	            'sess_start_time:SessionStartTime',
	            'sess_end_time:SessionEndTime',
	            'sess_end_date:SessionEndDate',
	            'country_name:LocationCountryName',
	            'language_code:LangCode',
	            'country_code:LocationCountryCode',
	            'registration_end_on:RegistrationEndDate',
	            'expires_in_value:ValidityDays',
	            'expires_in_unit:ValidityUnit',
	            'prm_price:Price',
	            'prm_currency_type:CurrencyTypeCode',
	            'prm_end_date:ValidTo',
	            'prm_created_on:CreatedOn',
	            'mro_id:mro',
	            'is_compliance:Compliance',
	
	    );
	
	    // Setting required filters
	    $filter = array();
	    $this->setAccessFiltersMyTeam($filter,$user_id);
	    $this->setSearchFiltersMyTeam($filter,'Class');
	    	
	    $results = $this->getCatalogDetailsMyTeam($filter,$fieldList,'GET');
	    	
	    return $results;
	
	}
	
	private function setAccessFiltersMyTeam(&$filter,&$uid){
	    $filter = array();
	    // Setting group filters
	    require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user.php';
	    $userObj = new UserSolrSearch();
	    $userDetails = $userObj->getLoggedUserDetails($uid);
	    $grpStr = '';
	    $user_hire = $userDetails[0]->HireDate;
	    $user_name = $userDetails[0]->username;
	    expDebug::dPrint ( 'Learner group for userss' . print_r ( $userDetails, true ), 4 );
	
	    if($userDetails[0]->learner_groups != ''){
	        $userAdminGrp = explode(',',$userDetails[0]->learner_groups);
	        expDebug::dPrint ( 'Learner group for userss' . print_r ( $userDetails, true ), 4 );
	        foreach($userAdminGrp as $key=>$val){
	            if($key==0)
	                $grpStr = 'LnrGroupName:"*'.trim(str_replace(' ','?',$val)).'*"';
	            else
	                $grpStr .= ' OR LnrGroupName:"*'.trim(str_replace(' ','?',$val)).'*"';
	        }
	        $grpStr .= ' OR (LnrGroupName:"*Open_Entity*" AND LnrAccessUserName:"*Open_Entity*")';
	    }
	    else
	        $grpStr = '(LnrGroupName:"*Open_Entity*" AND LnrAccessUserName:"*Open_Entity*")';
	
	    if($user_name)
	        $grpStr .= ' OR LnrAccessUserName:"*`'.$user_name.'`*"';
	    $filter[] = urlencode($grpStr);
	    if($user_hire){
	        $hire_date_filter = $this->setHireDateFilter($user_hire);
	        array_push($filter, urlencode($hire_date_filter));
	    }
	}
	
	private function setSearchFiltersMyTeam(&$filter,$catalog_reg){
	    // Setting required filters
	    $this->setDefaultFilter($filter,$catalog_reg);
	    $this->setTypeFilter($filter,$catalog_reg);
	    $this->setTextSearchFilter($filter);
	    $this->setPriceMyteam($filter);
	    expDebug::dPrint('Registration level check'.print_r($filter,true),4);
	}
	private function setPriceMyteam(&$filter){
	    $price = 'Price:0';
	    array_push($filter, urlencode($price));
	}
	private function getCatalogDetailsMyTeam($filter,$fieldList,$method){
	    // List of required fields to be set as NULL,
	    // because Solr will not keep any null fields
	    $resBase = array(
	            'object_type'=> NULL,
	            'cls_id'=> NULL,
	            'crs_id' => NULL,
	            'cls_code'=> NULL,
	            'cls_title'=> NULL,
	            'cls_short_description'=> NULL,
	            'delivery_type_code'=> NULL,
	            'currency_type'=> NULL,
	            'export_compliance'=> NULL,
	            'delivery_type_name'=> NULL,
	            'status'=> NULL,
	            'language'=> NULL,
	            'location'=> NULL,
	            'node_id'=> NULL,
	            'price'=> NULL,
	            'session_id'=> NULL,
	            'sess_start_date'=> NULL,
	            'sess_start_time'=> NULL,
	            'sess_end_time'=> NULL,
	            'sess_end_date'=> NULL,
	            'country_name'=> NULL,
	            'language_code'=> NULL,
	            'country_code'=> NULL,
	            'registration_end_on'=> NULL,
	            'expires_in_value'=> NULL,
	            'expires_in_unit'=> NULL,
	            'prm_price'=> NULL,
	            'prm_currency_type'=> NULL,
	            'prm_end_date'=> NULL,
	            'prm_created_on'=> NULL,
	            'mro_id'=> NULL,
	            'is_compliance'=> NULL
	
	    );
	
	    // Append field list
	    $data = 'fl=' . implode(',',$fieldList);
	    // Append filters
	    $data .= '&fq=' . implode('&fq=',$filter);
	    //Pagination
	    $rows = !empty($_REQUEST['rows']) ? $_REQUEST['rows'] : 10;
	    $start = ($_REQUEST['page']*$_REQUEST['rows'])-$rows;
	
	    //Sort by
	    if($_REQUEST['sortby'] && $_REQUEST['sortby'] != 'undefined'){
	        if($_REQUEST['sortby'] == 'AZ')
	            $sort = 'TitleSrt+asc';
	       else if($_REQUEST['sortby'] == 'ZA')
	            $sort = 'TitleSrt+desc';
	        else if($_REQUEST['sortby'] == 'Time')
	            $sort='UpdatedOn+desc';
	        else if($_REQUEST['sortby'] == 'ClassStartDate')
	            $sort = 'SessionStart+asc';
	        else if($_REQUEST['sortby'] == 'Mandatory')
	            $sort = 'mro+asc';
	    }
	    else
	        $sort = 'TitleSrt+asc';
	
	    $sort .= empty($sort) ? 'EntityId+asc' : ',EntityId+asc' ;
	
	    if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
	        $start = ($_REQUEST['page']*$_REQUEST['limit'])-$_REQUEST['limit'];
	        $rows = !empty($_REQUEST['limit']) ? $_REQUEST['limit'] : $rows;
	
	    }// Default, needs to be append with given sort
	    $start = $start < 0 ? 0 : $start;
	    $data .= '&sort='.$sort.'&indent=on&q=*:*&start='.$start.'&rows='.$rows;
	
	    try{
	        $srcRst = $this->getData($this->collName,$data,'GET');
	        expDebug::dPrint("Solr search result - Final ".print_r($srcRst,1),4);
	
	        $this->modifyResultSet($srcRst);
	        //$srcRst = drupal_json_decode($srcRst);
	
	        $srcRst1 = $srcRst['response']['docs'];
	        $srcRst2 = array();
	        $srcRst2['recCount'] = $srcRst['response']['numFound'];
	        foreach($srcRst1 as $doc){
	            $tdoc  = array_merge($resBase, $doc);
	            $this->modifyResultSetMyTeam($tdoc);
	            $srcRst2['records'][] = $this->arrayToObject($tdoc);
	        }
	
	        expDebug::dPrint("Solr search result - Final ".print_r($srcRst2,1),4);
	        expDebug::dPrint("Solr search result - Final Count".print_r($srcRst['response']['numFound'],1),4);
	        return $srcRst2;
	    }catch(Exception $e){
	        expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
	    }
	
	}
	
	 /** Auto complete **/
	public function getSolrSearchResultMyTeamAutoComplete($user_id){
	    global $catalog_reg;
	    $fieldList = array(
	            'cls_title:Title',
	    );
	
	    // Setting required filters
	    $filter = array();
	    $this->setAccessFiltersMyTeam($filter,$user_id);
	    $this->setSearchFiltersMyTeamAutoComplete($filter,'Class');
	    $results = $this->getCatalogDetailsMyTeamAutoComplete($filter,$fieldList,'GET');
	
	
	    return $results;
	}
	
	private function setSearchFiltersMyTeamAutoComplete(&$filter,$catalog_reg){
	    // Setting required filters
	    $this->setDefaultFilter($filter,$catalog_reg);
	    $this->setTypeFilter($filter,$catalog_reg);
	    $this->setPriceMyteam($filter);
	    $this->setSearchAutocompleteFilters($filter);
	    expDebug::dPrint('Registration level check'.print_r($filter,true),4);
	}
	
	
	private function getCatalogDetailsMyTeamAutoComplete($filter,$fieldList,$method){
	    // List of required fields to be set as NULL,
	    // because Solr will not keep any null fields
	    $resBase = array(
	            'cls_title' => NULL,
	    );
	
	    // Append field list
	    $data = 'fl=' . implode(',',$fieldList);
	    // Append filters
	    $data .= '&fq=' . implode('&fq=',$filter);
	    //Pagination
	    $rows = !empty($_REQUEST['rows']) ? $_REQUEST['rows'] : 10;
	    $start = ($_REQUEST['page']*$_REQUEST['rows'])-$rows;
	
	    //Sort by
	    if($_REQUEST['sord'] && $_REQUEST['sord'] != 'undefined'){
	        if($_REQUEST['sord'] == 'asc')
	            $sort = 'TitleSrt+asc';
	        if($_REQUEST['sord'] == 'desc')
	            $sort = 'TitleSrt+desc';
	    }
	
	    expDebug::dPrint("Sort detail ".print_r($sort,1),4);
	    $sort .= empty($sort) ? 'EntityId+asc' : ',EntityId+asc' ;
	
	    expDebug::dPrint("Sort detail12345 ".print_r($sort,1),4);
	    if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
	        $start = ($_REQUEST['page']*$_REQUEST['limit'])-$_REQUEST['limit'];
	        $rows = !empty($_REQUEST['limit']) ? $_REQUEST['limit'] : $rows;
	
	    }// Default, needs to be append with given sort
	    $start = $start < 0 ? 0 : $start;
	    $data .= '&sort='.$sort.'&indent=on&q=*:*&start='.$start.'&rows='.$rows;
	    expDebug::dPrint("data detail12345 ".print_r($data,1),4);
	    try{
	        $srcRst = $this->getData($this->collName,$data,'GET');
	        expDebug::dPrint("Solr search result - Final ".print_r($srcRst,1),4);
	
	        $this->modifyResultSet($srcRst);
	        //$srcRst = drupal_json_decode($srcRst);
	
	        $srcRst1 = $srcRst['response']['docs'];
	        $srcRst2 = array();
	        $srcRst2['recCount'] = $srcRst['response']['numFound'];
	        foreach($srcRst1 as $doc){
	            $tdoc  = array_merge($resBase, $doc);
	            $this->modifyResultSetMyTeam($tdoc);
	            $srcRst2['records'][] = $this->arrayToObject($tdoc);
	        }
	
	        expDebug::dPrint("Solr search result - Final ".print_r($srcRst2,1),4);
	        expDebug::dPrint("Solr search result - Final Count".print_r($srcRst['response']['numFound'],1),4);
	        return $srcRst2;
	    }catch(Exception $e){
	        expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
	    }
	
	}
	private function modifyResultSetMyTeam(&$tdoc){
	    // Logged in users details
	    $location = (array)$tdoc['location'];
	    expDebug::dPrint("Error in Solr Catalog Search rfffffffffff-- ".print_r($location,true),1);
	    $tdoc['location'] = $location[0];
	    
	    // modify object type for 0085103
	    $type = $tdoc['object_type'];
	    if($type == 'lrn_cls_dty_wbt' || $type == 'lrn_cls_dty_ilt'|| $type == 'lrn_cls_dty_vod' || $type == 'lrn_cls_dty_vcl') 
	    	$type = 'Class';
		$tdoc['object_type'] = $type;
	}
	
	/** My team assign learning ends here **/
	
	private function SetClassTag(&$tdoc){
		// Logged in users details
		$classtag = $tdoc['tagname'];
		$coursetag = $tdoc['coursetagname'];
		expDebug::dPrint("Error in Solr Catalog Search coursetag-- ".print_r($coursetag,true),1);
		expDebug::dPrint("Error in Solr Catalog Search classtag-- ".print_r($classtag,true),1);
		if(empty($coursetag)){
			$tdoc['tagname'] = $classtag;
		 }else if(empty($classtag)){
		 	$tdoc['tagname'] = $coursetag;
		 }else{
		 	$tdoc['tagname'] = $classtag.','.$coursetag;
		} 
		expDebug::dPrint("Error in Solr Catalog Search tdoc-- ".print_r($tdoc['tagname'],true),1);
	}
	
}
?>