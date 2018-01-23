<?php

/**
 * LnrSearchCatalog
 *
 * Catalog Search, which is used to search the Class,Certification and Curricula
 * and have option to find various types of Delivery Type, Language, Country and Location
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 22-Aug-2011
 */
class LnrSearchCatalog{
  /**
   * An indentifier
   * @var string
   */
  public $select = null;
  //public $user_role = null;
  public $limit;
  public $page;
  public $start = 0;
  public $total_pages;
  private $courseLangvar=0;
  private $baseQuery;
  private $iltFilterSet=0; //Added/changed by ganeshbabuv on Feb 13th 2016 for  date, country and location filters issues #0071712

/*  public $delivery_type_name_disrttinct = array();
  public $language_distinct = array();
  public $country_name_distinct = array();
  public $full_search = array();
  public $mro_distinct = array();
  public $count_search = array();*/


  function __construct() {
  	try{
    if(!empty($_GET['rows']))
    $this->limit     = $_GET['rows'];
    if(!empty($_GET['page']))
    $this->page      = $_GET['page'];
    //$this->user_role = '';
  	}catch (Exception $ex) {
	    watchdog_exception('__construct', $ex);
	    expertusErrorThrow($ex);
    }
  }


  /**
   * Funtion to return the value of $_GET method.
   *
   * @param string $index_var
   * $index_var- String value
   *
   * @return
   *   Results of $_GET method
   */
  public function setParamVal($index_var, $cleanInput = true){
    try{
    if (isset($_GET[$index_var])&& !(isset($_REQUEST['apiname'])))  {
    	$val= $cleanInput ? stripApostrophe($_GET[$index_var]) : $_GET[$index_var];	//change by ayyappans for 42444: In catalog location filter if the location contains special character
    	//return escape_string($val);
    	return $val;
    }
    if(isset($_REQUEST['apiname'])){
    	if (isset($_REQUEST[$index_var])) {
    		$val= $cleanInput ? stripApostrophe($_REQUEST[$index_var]) : $_REQUEST[$index_var];
    		return escape_string($val);
    	}
    }
    if ($index_var == 'lg_type' && (arg(1) != 'catalog-autocomplete') && !(stristr($_SERVER['HTTP_REFERER'],'search|'))) {
    	return getUserPreferredLanguage();
    }

    return '';
  }
  catch (Exception $ex) {
    watchdog_exception('setParamVal', $ex);
    expertusErrorThrow($ex);
    }
  }

  /**
   * Funtion to return the value of $_GET method.
   *
   * @param string $index_var
   * $index_var- String value
   *
   * @return
   *   Results of $_GET method
   */
  public function setParamTitleVal($index_var){
  	try{
  		
  		if (isset($_REQUEST[$index_var])) {
  			$val = str_replace("\'", "'", $_REQUEST[$index_var]);
  			$val = str_replace('\"', '"', $val);
  			return $val;
  		}
  		return '';
  	}
  	catch (Exception $ex) {
  		watchdog_exception('setParamTitleVal', $ex);
  		expertusErrorThrow($ex);
  	}
  }
  /* -------------------- Start Title Search ------------------*/
  function searchTitleClass($title) {
    try{
    global $catalog_reg;
    $Cls_Code = isset($_REQUEST['course_code']) ? $_REQUEST['course_code'] : '';
    if (!empty($title)){
    
      if($catalog_reg == 'Course'){
      	if($Cls_Code == ''){
	      $this->select->condition(db_and()->
	      			condition(db_or()->
	      					condition('cls.title', '%' . db_like($title) . '%', 'LIKE')->
	      					condition('cls.code', '%' . db_like($title) . '%', 'LIKE')->
	      					condition('td.tagname', '%' . db_like($title) . '%', 'LIKE')->
	      					condition('cls.short_description', '%' . db_like($title) . '%', 'LIKE')
	      			));
      }
      else{
      	$this->select->condition(db_and()->
      			condition(db_or()->
      					condition('cls.title', $title, 'LIKE')->
      					condition('cls.code', $Cls_Code , 'LIKE')));
      }
      }else{
      	   $this->select->condition(db_and()->
      				condition(db_or()->
      					condition('cls.title', '%' . db_like($title) . '%', 'LIKE')->
      					condition('cls.code', '%' . db_like($title) . '%', 'LIKE')->
      					condition('cls.short_description', '%' . db_like($title) . '%', 'LIKE')->
      					condition('td.tagname', '%' . db_like($title) . '%', 'LIKE')
      			));
      }
      //->condition('td.tagname', '%' . db_like($title) . '%', 'LIKE') -- Comment for performance
    }
    else{
    	if($Cls_Code != ''){
    		expDebug::dPrint(' $selectedFiltersfsfsfsfs = ' . print_r($this->searchRating, true) , 4);
    		$this->select->condition(db_and()->
    				condition('cls.code', $Cls_Code, 'LIKE'));
    	}
    }
    }catch (Exception $ex) {
    watchdog_exception('searchTitleClass', $ex);
    expertusErrorThrow($ex);
    }
  }

  function searchTitleCourse($title) {
  	try{
  		$Crs_Code = isset($_REQUEST['course_code']) ? $_REQUEST['course_code'] : '';
  		if (!empty($title)){
  			if($Crs_Code == ''){
  			$this->select->condition(db_and()->
  					condition(db_or()->
  							condition('crs.title', '%' . db_like($title) . '%', 'LIKE')->
  							condition('crs.code', '%' . db_like($title) . '%', 'LIKE')->
  							condition('crs.short_description', '%' . db_like($title) . '%', 'LIKE')
  							->condition('td.tagname', '%' . db_like($title) . '%', 'LIKE') //-- Comment for performance
  					));
  		}
  		else{
  			$this->select->condition(db_and()->
  					condition(db_and()->
  							condition('crs.title', $title, 'LIKE')->
  							condition('crs.code', $Crs_Code , 'LIKE')));
  		}
  		}
  		else{
  			if($Crs_Code != ''){
  				$this->select->condition(db_and()->
  						condition('crs.code', $Crs_Code, 'LIKE'));
  			}
  		}
  	}catch (Exception $ex) {
  		watchdog_exception('searchTitleClass', $ex);
  		expertusErrorThrow($ex);
  	}
  }

  function searchTitleTP($title) {
    try{
    	$Cls_Code = isset($_REQUEST['course_code']) ? $_REQUEST['course_code'] : '';
    if (!empty($title)){
    	if($Cls_Code != ''){
    		$this->select->condition(db_and()->
    				condition(db_and()->
    						condition('prm.title', $title, 'LIKE')->
    						condition('prm.code', $Cls_Code , 'LIKE')));
    	}
    	else{
      $this->select->condition(db_and()->
      condition(db_or()->
      condition('prm.title', '%' . db_like($title) . '%', 'LIKE')->
      condition('prm.code', '%' . db_like($title) . '%', 'LIKE')->
      condition('prm.short_desc', '%' . db_like($title) . '%', 'LIKE')
      ->condition('td.tagname', '%' . db_like($title) . '%', 'LIKE') //-- Comment for performance
      ));
    }
    }
    else{
    	if($Cls_Code != ''){
    		$this->select->condition(db_and()->
    				condition('prm.code', $Cls_Code, 'LIKE'));
    	}
    }
    }catch (Exception $ex) {
    watchdog_exception('searchTitleTP', $ex);
    expertusErrorThrow($ex);
    }
  }
  /* -------------------- End Title Search ------------------*/


  /* -------------------- Start Type Search ------------------*/
  function searchTypeClass($dl_type,$ob_type) {
    try{
    if (!empty($dl_type)) {
      $deliverytype = explode("|",$dl_type);
      $this->select->condition(db_and()->condition('cls.delivery_type',$deliverytype,'IN'));
    } else if (empty($ob_type) && empty($dl_type)){
      $this->select->condition(db_and()->condition('cls.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_wbt','lrn_cls_dty_vcl','lrn_cls_dty_vod'),'IN'));
    }
    }catch (Exception $ex) {
    watchdog_exception('searchTypeClass', $ex);
    expertusErrorThrow($ex);
    }
  }

function searchTypeTP($dl_type,$ob_type) {
try{
  if (!empty($ob_type)){
      $objecttype = explode("|",$ob_type);
      $this->select->condition(db_and()->condition('prm.object_type',$objecttype,'IN'));
    } else if (empty($ob_type) && empty($dl_type)){
      $this->select->condition(db_and()->condition('prm.object_type', array('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn'), 'IN'));
    }
  }catch (Exception $ex) {
    watchdog_exception('searchTypeTP', $ex);
    expertusErrorThrow($ex);
    }
}
/* -------------------- End Type Search ------------------*/


  /* -------------------- Start Language Search ------------------*/
  function searchLanguageClass($lg_type) {
    try{
    if (!empty($lg_type)){
      $languagetype = explode("|",$lg_type);
      $this->select->condition(db_and()->condition('cls.lang_code',$languagetype,'IN'));
    }
    }catch (Exception $ex) {
    watchdog_exception('searchLanguageClass', $ex);
    expertusErrorThrow($ex);
    }
  }
  function searchLanguageCourse($lg_type) {
  	try{
  		expDebug::dPrint("test lang".$lg_type." --- ".$this->courseLangvar);
  		if (!empty($lg_type)){
  			$languagetype = explode("|",$lg_type);
  			// Condition commented by Vincent for #0036993
        // Any other issue came because of this let consider the above.
  			//if($this->courseLangvar == 1){
  				$this->select->condition(db_and()->condition(db_or()->condition('clsRollup.lang_code',$languagetype,'IN')
  						->condition('crs.lang_code',$languagetype,'IN')));
  			/*}else{
  				$this->select->condition(db_and()->condition('clsRollup.lang_code',$languagetype,'IN'));
  			}*/
  		//	expDebug::dPrintDBAPI('select class lang course',$this->select);
  		}
  	}catch (Exception $ex) {
  		watchdog_exception('searchLanguageCourse', $ex);
  		expertusErrorThrow($ex);
  	}
  }

  function searchLanguageTP($lg_type) {
    try{
    if (!empty($lg_type)){
      $languagetype = explode("|",$lg_type);
      $this->select->condition(db_and()->condition('prm.lang_code',$languagetype,'IN'));
    }
    }catch (Exception $ex) {
    watchdog_exception('searchLanguageTP', $ex);
    expertusErrorThrow($ex);
    }
  }

  /* -------------------- End Language Search ------------------*/


  /* -------------------- Start Price Search ------------------*/

  function searchPriceClass($price) {
  	try{
    if (!empty($price) || $_REQUEST['price_filter']) {
    	$priceRpl 	 = str_replace("$", "", $price);
    	$priceExp 	 = explode("-", $priceRpl);
    	expDebug::dPrint('$price_start---->'.print_r($priceExp,1),4);
    	$price_start   = preg_replace("/[^0-9]/", '', $priceExp[0]);
    	$price_end 	 = preg_replace("/[^0-9]/", '', $priceExp[1]);
    	expDebug::dPrint('$price_start---->'.print_r($price_start,1),4);
    	expDebug::dPrint('$$price_end---->'.print_r($price_end,1),4);
    	$userId = getSltpersonUserId();

    	if($_REQUEST['apiname'])
    	{
    		$price = $_REQUEST['price_filter'];
    		$price = explode(',',$price);
    		$price_start = $price[0];
    		$price_end = $price[1];
    	}

    	$UserDefaultCurrencyArr = getUserDefaultCurrency();
    	$UserPrefCurrCode = $UserDefaultCurrencyArr['currency_code'];

    	$this->select->where('IF(cls.delivery_type IN (\'lrn_cls_dty_ilt\',\'lrn_cls_dty_wbt\',\'lrn_cls_dty_vcl\',\'lrn_cls_dty_vod\'),(ifnull((slf_convert_price(cls.price,cls.currency_type,\''.$UserPrefCurrCode.'\')),0) BETWEEN \''.$price_start.'\' and \''.$price_end.'\'),1=1)');
    }
  	}catch (Exception $ex) {
    watchdog_exception('searchPriceClass', $ex);
    expertusErrorThrow($ex);
  	}
  }

  function searchPriceCourse($price) {
  	try{
  		if (!empty($price) || $_REQUEST['price_filter']) {
  			$UserDefaultCurrencyArr = getUserDefaultCurrency();
  			$UserPrefCurrCode = $UserDefaultCurrencyArr['currency_code'];
  			$this->courseLangvar = 1;
  			$priceRng = getSliderPriceRange();
  			$startprice = $priceRng['startprice'];
  			$endprice = $priceRng['endprice'];
  			$priceRpl 	 = str_replace("$", "", $price);
  			$priceExp 	 = explode("-", $priceRpl);
  			$price_start   = preg_replace("/[^0-9]/", '', $priceExp[0]);
  			$price_end 	 = preg_replace("/[^0-9]/", '', $priceExp[1]);

  			if($_REQUEST['apiname'])
  			{
  				$price = $_REQUEST['price_filter'];
  				$price = explode(',',$price);
  				$price_start = $price[0];
  				$price_end = $price[1];
  			}
  			if($startprice != $price_start || $endprice!=$price_end){
  				$this->select->where('IF(clsRollup.delivery_type IN (\'lrn_cls_dty_ilt\',\'lrn_cls_dty_wbt\',\'lrn_cls_dty_vcl\',\'lrn_cls_dty_vod\'),
  						(ifnull(slf_convert_price(clsRollup.price,clsRollup.currency_type,\''.$UserPrefCurrCode.'\'),0) BETWEEN \''.$price_start.'\' and \''.$price_end.'\'),
  						if(clsRollup.delivery_type is null,1=2,1=1))');
  			}else{
  				$this->select->where('IF(clsRollup.delivery_type IN (\'lrn_cls_dty_ilt\',\'lrn_cls_dty_wbt\',\'lrn_cls_dty_vcl\',\'lrn_cls_dty_vod\'),
  						(ifnull(slf_convert_price(clsRollup.price,clsRollup.currency_type,\''.$UserPrefCurrCode.'\'),0) BETWEEN \''.$price_start.'\' and \''.$price_end.'\'),1=1)');
  			}
  		}
  	}catch (Exception $ex) {
  		watchdog_exception('searchPriceCourse', $ex);
  		expertusErrorThrow($ex);
  	}
  }

  function searchPriceTP($price) {
  	try{

  		if (!empty($price) || $_REQUEST['price_filter']) {
  			$UserDefaultCurrencyArr = getUserDefaultCurrency();
  			$UserPrefCurrCode = $UserDefaultCurrencyArr['currency_code'];

  			$priceRpl 	 = str_replace("$", "", $price);
  			$priceExp 	 = explode("-", $priceRpl);
  			$price_start   = preg_replace("/[^0-9]/", '', $priceExp[0]);
  			$price_end 	 = preg_replace("/[^0-9]/", '', $priceExp[1]);
  			if($_REQUEST['apiname'])
  			{
  				$price = $_REQUEST['price_filter'];
  				$price = explode(',',$price);
  				$price_start = $price[0];
  				$price_end = $price[1];
  			}
  			$this->select->where('IF(prm.object_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\'),(ifnull(slf_convert_price(prm.price,prm.currency_type,\''.$UserPrefCurrCode.'\'),0) BETWEEN \''.$price_start.'\' and \''.$price_end.'\'),1=1)');
  		}
  	}catch (Exception $ex) {
    watchdog_exception('searchPriceTP', $ex);
    expertusErrorThrow($ex);
  	}
  }
/* -------------------- End Price Search ------------------*/

  /*------------------- start job role search ---------------*/
  function searchJobrole($jr_type){
  	try{
  		expDebug::dPrint('SELECTED JOB ROLE AEE-->'.print_r($jr_type,1),4);
  		if(!empty($jr_type)){
	  		$queryCondition = db_or();
	  		if (strpos($jr_type,'|') !== false) {
			  		if (!empty($jr_type)){
			  			$jobrolearr = explode("|",$jr_type);
			  			expDebug::dPrint('SELECTED JOB ROLE AEE-->'.print_r($jobrolearr,1),4);
			  			$queryCondition->where('if(catacs.job_role=\'All\',\''.$jr_type .'\'!=\'\',\'\')');
			  			foreach ($jobrolearr as $key=>$value){
			  					$queryCondition->where('FIND_IN_SET(:job_role_'.$key.', catacs.job_role)',array(':job_role_'.$key => $value));
			  			}
			  		}
	  		}elseif (!empty($jr_type)){
	  			//where('FIND_IN_SET(:'.$job_role_var.', '. $aliasCatalogAccess . '.'.$value.$braket,array(':'.$job_role_var => $job_role));
	  			$queryCondition->where('if(catacs.job_role=\'All\',\''.$jr_type .'\'!=\'\',\'\')');
	  			$queryCondition->where('FIND_IN_SET(:job_role_val, catacs.job_role)',array(':job_role_val' => $jr_type));
	  			//$this->select->where('FIND_IN_SET("'.$jr_type.'", catacs.job_role)');
	  		}
	  		$this->select->condition($queryCondition);
	  		expDebug::dPrint('SELECTED JOB ROLE AEE-->$find_in'.print_r($find_in,1),4);
  		}
  	}catch (Exception $ex) {
  		watchdog_exception('searchJobrole', $ex);
  		expertusErrorThrow($ex);
  	}
  }


  /*------------------- start job role search ---------------*/


  /* -------------------- Start Country Search ------------------*/
  function searchCountry($cy_type) {
    try{
    if (!empty($cy_type)){
      $country = explode("|",$cy_type);
      $this->select->condition(db_and()->
      condition('cls.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl'),'IN')->
      condition('loc.country',$country,'IN')
      );
    }
    }catch (Exception $ex) {
    watchdog_exception('searchCountry', $ex);
    expertusErrorThrow($ex);
    }
  }
  function searchCourseCountry($cy_type) {
  	try{
  		if (!empty($cy_type)){
  			$this->courseLangvar = 1;
			$this->iltFilterSet=1; //Added/changed by ganeshbabuv on Feb 13th 2016 for  date, country and location filters issues #0071712
  			$country = explode("|",$cy_type);
  			$this->select->condition(db_and()->
  					condition('clsRollup.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl'),'IN')->
  					condition('loc.country',$country,'IN')
  			);
  		}
  	}catch (Exception $ex) {
  		watchdog_exception('searchCourseCountry', $ex);
  		expertusErrorThrow($ex);
  	}
  }

 /* Changes done by vetrivel - Added for #0066426 */
 function searchTPCountry($cy_type) {
  	try{
  		if (!empty($cy_type)){
  			$this->courseLangvar = 1;
			$this->iltFilterSet=1; //Added/changed by ganeshbabuv on Feb 13th 2016 for  date, country and location filters issues #0071712
  			$country = explode("|",$cy_type);
  			$this->select->condition(db_and()->
  					condition('clsRollup.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl'),'IN')-> 
  					condition('loc.country',$country,'IN')
					->condition('clsRollup.status',array('lrn_cls_sts_atv'),'IN')
					
  			);
  		}
  	}catch (Exception $ex) {
  		watchdog_exception('searchTPCountry', $ex);
  		expertusErrorThrow($ex);
  	}
  } 

  /* -------------------- End Country Search ------------------*/


  /* -------------------- Start Location Search ------------------*/
  
/* Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404 */

  function searchLocation($location) {
    try{
    if (!empty($location)){
			$loc_or_condition_obj=getLocationFilterQuery($location);
      $this->select->condition('cls.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl'),'IN');
		     $this->select->condition(db_and()->condition($loc_or_condition_obj)); 
    }
    }catch (Exception $ex) {
    watchdog_exception('searchLocation', $ex);
    expertusErrorThrow($ex);
    }
  }
 
 /* Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404 */

  function searchCourseLocation($location) {
  	try{
  		if (!empty($location)){
  			$this->courseLangvar = 1;
			$this->iltFilterSet=1; //Added/changed by ganeshbabuv on Feb 13th 2016 for  date, country and location filters issues #0071712
			$this->select->leftJoin('slt_country','loc_ctry','loc_ctry.country_code=loc.country');
			$this->select->leftJoin('slt_state','loc_stats','loc_stats.state_code = loc.state and loc_stats.country_code=loc.country'); 
			
 			$loc_or_condition_obj=getLocationFilterQuery($location);
			
  			$this->select->condition(db_and()->
  					condition('clsRollup.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl'),'IN')->
	  					condition($loc_or_condition_obj)
  			);
  		}
  	}catch (Exception $ex) {
  		watchdog_exception('searchCourseLocation', $ex);
  		expertusErrorThrow($ex);
  	}
  }

 /* Added for #0066426  - Function has added by vetrivel */
 
 function searchTPLocation($location) {
  	try{ 

  		if (!empty($location)){
  			$this->courseLangvar = 1;
			$this->iltFilterSet=1; //Added/changed by ganeshbabuv on Feb 13th 2016 for  date, country and location filters issues #0071712
			$this->select->leftJoin('slt_country','loc_ctry','loc_ctry.country_code=loc.country');
			$this->select->leftJoin('slt_state','loc_stats','loc_stats.state_code = loc.state and loc_stats.country_code=loc.country'); 

 			$loc_or_condition_obj=getLocationFilterQuery($location);
  			
  			$this->select->condition(db_and()->
  					condition('clsRollup.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl'),'IN')->
  					condition($loc_or_condition_obj)
					->condition('clsRollup.status',array('lrn_cls_sts_atv'),'IN')
  			); 
  			
  		}
  	}catch (Exception $ex) {
  		watchdog_exception('searchTPLocation', $ex);
  		expertusErrorThrow($ex);
  	}
  }

  /* -------------------- End Location Search ------------------*/


  /* -------------------- Start Tag Search ------------------*/
  function searchTag($tag) {
    try{
   	 expDebug::dPrint('TAGS VALUE---->'.$tag,4);
   	 expDebug::dPrint('TAGS VALUE---->'.rawurldecode($tag),4);
    if (!empty($tag)){
    	if(isset($_REQUEST['mobile_request'])){
			$tags = explode ( "|", $tag );
			$this->select->condition ( db_and ()->condition ( 'td.tagname', $tags, 'IN' ) );
    	}else{
      		//$this->select->condition(db_and()->condition('td.tagname', rawurldecode($tag), '='));
      		$this->select->where('td.tagname=:tagname', array(':tagname' => rawurldecode($tag)));
    	}
    }
    }catch (Exception $ex) {
    watchdog_exception('searchTag', $ex);
    expertusErrorThrow($ex);
    }
  }
  function searchCourseTag($tag) {
  	try{
  		if (!empty($tag)){
  			
  			if(isset($_REQUEST['mobile_request'])){
  				$tags = explode ( "|", $tag );
  				$this->select->condition ( db_and ()->condition ( 'td.tagname', $tags, 'IN' ) );
  			}else{
  				$this->courseLangvar = 1;
  				//$this->select->condition(db_and()->condition('td.tagname', rawurldecode($tag), '='));
  				$this->select->where('td.tagname=:tagname', array(':tagname' => rawurldecode($tag)));
  			}
  		}
  	}catch (Exception $ex) {
  		watchdog_exception('searchCourseTag', $ex);
  		expertusErrorThrow($ex);
  	}
  }
  /* -------------------- End Tag Search ------------------*/

  /* -------------------- Start Date Search ------------------*/

  function searchDate($startdate, $enddate) {
    try{
    $timeZone = date_default_timezone(false);
    $defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
    $sessionTZ = new DateTimeZone($defaultTimezone);
    $toDay = new DateTime(null,$sessionTZ) ;
    $toDayDT = $toDay->format('Y-m-d H:i');
    $toDay = $toDay->format('Y-m-d');

    $UserPHPTimezoneName		= getUserPHPTimezoneName();
    $vUserPHPTimezonename		= $UserPHPTimezoneName[0]->zone_name;
    if(!$UserPHPTimezoneName[0]->zone_name){
    	$vUserPHPTimezonename = date_default_timezone(FALSE);
    }
    if (!empty($startdate)){
      list($m,$d,$y) = explode("-",$startdate);
      $startdate = $y.'-'.$m.'-'.$d.' 00:00';
      /*commented for ticket #0033124*/
      //$startdate  = timeZoneConvert($startdate , $vUserPHPTimezonename,  $timeZone);
      //$startdate = $startdate->format('Y-m-d');
    }
    if (!empty($enddate)){
    	list($m,$d,$y) = explode("-",$enddate);
    	$enddate = $y.'-'.$m.'-'.$d.' 23:59';
    	/*commented for ticket #0033124*/
      	//$enddate  = timeZoneConvert($enddate , $vUserPHPTimezonename,  $timeZone);
      	//$enddate = $enddate->format('Y-m-d');
    }
    expDebug::dPrint('$vUserPHPTimezonename : '.$vUserPHPTimezonename, 4);
    $applyTimezone = (empty($vUserPHPTimezonename) ? $defaultTimezone : $vUserPHPTimezonename); // apply Learner Timezone #0048574
    if (!empty($startdate) && !empty($enddate) ){
      $this->select->condition(db_and()->
      condition('cls.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl'),'IN')->
    			where("'$startdate' <= CONVERT_TZ(CONCAT(DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.start_time),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone),'".$applyTimezone."')")->
    			where("'$enddate' >= CONVERT_TZ(CONCAT(DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.start_time),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone),'".$applyTimezone."')")->
    			where("if(DATE_FORMAT(CONVERT_TZ(CONCAT(ses.start_date,' ',ses.start_time),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone),'".$applyTimezone."'), _utf8'%Y-%m-%d')  = '$toDay',
      		 			'$toDayDT' <= CONVERT_TZ(CONCAT(DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.start_time),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone),'".$applyTimezone."'),
      		1=1)")
      );

  		} else if (!empty($startdate) && $enddate=='' ){
  			$this->select->condition(db_and()->
      condition('cls.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl'),'IN')->
    			where("'$startdate' <= CONVERT_TZ(CONCAT(DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.start_time) ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone), '".$applyTimezone."' )")->
    			where("if(DATE_FORMAT(CONVERT_TZ(CONCAT(ses.start_date,' ',ses.start_time),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone),'".$applyTimezone."'), _utf8'%Y-%m-%d')  = '$toDay',
    					'$toDayDT' <= CONVERT_TZ(CONCAT(DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.start_time),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone), '".$applyTimezone."' ),
      			1=1)")
  			);
  		} else if ($startdate=='' && !empty($enddate)){
  			$startdate = Date("Y-m-d");
  			$this->select->condition(db_and()->
      condition('cls.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl'),'IN')->
    			where("'$enddate' >= CONVERT_TZ(CONCAT(DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.start_time),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone),'".$applyTimezone."')")->
    			where("if(DATE_FORMAT(CONVERT_TZ(CONCAT(ses.start_date,' ',ses.start_time),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone),'".$applyTimezone."'), _utf8'%Y-%m-%d') = '$toDay',
    					'$toDayDT' <= CONVERT_TZ(CONCAT(DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.start_time) ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone), '".$applyTimezone."'),
      			1=1)")
  			);
  		}
  	}catch (Exception $ex) {
  		watchdog_exception('searchDate', $ex);
  		expertusErrorThrow($ex);
  	}
  }

function searchCourseDate($startdate, $enddate) {
  	try{
	  	$timeZone = date_default_timezone(false);
	    $defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
	    $sessionTZ = new DateTimeZone($defaultTimezone);
	    $toDay = new DateTime(null,$sessionTZ) ;
	    $toDayDT = $toDay->format('Y-m-d H:i');
	    $toDay = $toDay->format('Y-m-d');

	    $UserPHPTimezoneName		= getUserPHPTimezoneName();
	    $vUserPHPTimezonename		= $UserPHPTimezoneName[0]->zone_name;
	    if(!$UserPHPTimezoneName[0]->zone_name){
	    	$vUserPHPTimezonename = date_default_timezone(FALSE);
	    }
	    if (!empty($startdate)){
	      list($m,$d,$y) = explode("-",$startdate);
	      $startdate = $y.'-'.$m.'-'.$d.' 00:00';
	      /*commented for ticket #0033124*/
	      //$startdate  = timeZoneConvert($startdate , $vUserPHPTimezonename,  $timeZone);
	      //$startdate = $startdate->format('Y-m-d');
	    }
	    if (!empty($enddate)){
	    	list($m,$d,$y) = explode("-",$enddate);
	    	$enddate = $y.'-'.$m.'-'.$d.' 23:59';
	    	/*commented for ticket #0033124*/
	      //$enddate  = timeZoneConvert($enddate , $vUserPHPTimezonename,  $timeZone);
	      //$enddate = $enddate->format('Y-m-d');
	    }

        $applyTimezone = (empty($vUserPHPTimezonename) ? $defaultTimezone : $vUserPHPTimezonename); // apply Learner Timezone #0048574 

       	if (!empty($startdate) && !empty($enddate) ){
  			$this->courseLangvar = 1;
			$this->iltFilterSet=1; //Added/changed by ganeshbabuv on Feb 13th 2017 for  date, country and location filters issues #0071712

			//Added/changed by ganeshbabuv on Feb 15th 2016 for #0072216: Unable to find the ILT courses according to user timezone in Learner Catalog at Course Level
  			$this->select->condition(db_and()->
  					condition('clsRollup.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl'),'IN')->
  					condition('clsRollup.id', 1 , '>=')->
  					where("'$startdate' <= CONVERT_TZ(CONCAT(DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.start_time),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone),'".$applyTimezone."')")->
    			where("'$enddate' >= CONVERT_TZ(CONCAT(DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.start_time),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone),'".$applyTimezone."')")->
    			where("if(DATE_FORMAT(CONVERT_TZ(CONCAT(ses.start_date,' ',ses.start_time),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone),'".$applyTimezone."'), _utf8'%Y-%m-%d')  = '$toDay',
      		 			'$toDayDT' <= CONVERT_TZ(CONCAT(DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.start_time),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone),'".$applyTimezone."'),
      			1=1)") 
  			);

    } else if (!empty($startdate) && $enddate=='' ){
  			$this->courseLangvar = 1;
			$this->iltFilterSet=1; //Added/changed by ganeshbabuv on Feb 13th 2017 for  date, country and location filters issues #0071712
			
			//Added/changed by ganeshbabuv on Feb 15th 2016 for #0072216: Unable to find the ILT courses according to user timezone in Learner Catalog at Course Level
      $this->select->condition(db_and()->
  					condition('clsRollup.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl'),'IN')->
  					condition('clsRollup.id', 1 , '>=')->
    			where("'$startdate' <= CONVERT_TZ(CONCAT(DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.start_time) ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone), '".$applyTimezone."' )")->
    			where("if(DATE_FORMAT(CONVERT_TZ(CONCAT(ses.start_date,' ',ses.start_time),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone),'".$applyTimezone."'), _utf8'%Y-%m-%d')  = '$toDay',
    					'$toDayDT' <= CONVERT_TZ(CONCAT(DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.start_time),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone), '".$applyTimezone."' ),
      1=1)")
  		);
    }  else if ($startdate=='' && !empty($enddate)){
  			$this->courseLangvar = 1;
			$this->iltFilterSet=1; //Added/changed by ganeshbabuv on Feb 13th 2017 for  date, country and location filters issues #0071712
      $startdate = Date("Y-m-d");
			
			//Added/changed by ganeshbabuv on Feb 15th 2016 for #0072216: Unable to find the ILT courses according to user timezone in Learner Catalog at Course Level
      $this->select->condition(db_and()->
  					condition('clsRollup.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl'),'IN')->
  					condition('clsRollup.id', 1 , '>=')->
    			where("'$enddate' >= CONVERT_TZ(CONCAT(DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.start_time),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone),'".$applyTimezone."')")->
    			where("if(DATE_FORMAT(CONVERT_TZ(CONCAT(ses.start_date,' ',ses.start_time),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone),'".$applyTimezone."'), _utf8'%Y-%m-%d') = '$toDay',
    					'$toDayDT' <= CONVERT_TZ(CONCAT(DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.start_time) ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone), '".$applyTimezone."'),
      1=1)")
  		);
    }
    }catch (Exception $ex) {
  		watchdog_exception('searchCourseDate', $ex);
    expertusErrorThrow($ex);
    }
  }

 /* Added for #0066426  - Function has added by vetrivel */

function searchTPDate($startdate, $enddate) {
  	try{
	  	$timeZone = date_default_timezone(false);
	    $defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
	    $sessionTZ = new DateTimeZone($defaultTimezone);
	    $toDay = new DateTime(null,$sessionTZ) ;
	    $toDayDT = $toDay->format('Y-m-d H:i');
	    $toDay = $toDay->format('Y-m-d');

	    $UserPHPTimezoneName		= getUserPHPTimezoneName();
	    $vUserPHPTimezonename		= $UserPHPTimezoneName[0]->zone_name;
	    if(!$UserPHPTimezoneName[0]->zone_name){
	    	$vUserPHPTimezonename = date_default_timezone(FALSE);
	    }
	    if (!empty($startdate)){
	      list($m,$d,$y) = explode("-",$startdate);
	      $startdate = $y.'-'.$m.'-'.$d.' 00:00';
	      /*commented for ticket #0033124*/
	      //$startdate  = timeZoneConvert($startdate , $vUserPHPTimezonename,  $timeZone);
	      //$startdate = $startdate->format('Y-m-d');
	    }
	    if (!empty($enddate)){
	    	list($m,$d,$y) = explode("-",$enddate);
	    	$enddate = $y.'-'.$m.'-'.$d.' 23:59';
	    	/*commented for ticket #0033124*/
	      //$enddate  = timeZoneConvert($enddate , $vUserPHPTimezonename,  $timeZone);
	      //$enddate = $enddate->format('Y-m-d');
	    }

		$this->select->leftJoin('slt_module_crs_mapping','module','module.program_id=prm.id');
		$this->select->leftJoin('slt_course_class','clsRollup','clsRollup.course_id=module.course_id');
		$this->select->leftJoin('slt_location','loc','loc.id = clsRollup.location_id');
		$this->select->leftJoin('slt_course_class_session','ses','ses.class_id = clsRollup.id');
		
		expDebug::dPrint('$vUserPHPTimezonename : '.$vUserPHPTimezonename, 4);
        $applyTimezone = (empty($vUserPHPTimezonename) ? $defaultTimezone : $vUserPHPTimezonename); // apply Learner Timezone #0048574
       // expDebug::dPrint('$applyTimezone : '.$applyTimezone, 5);
		
  		if (!empty($startdate) && !empty($enddate) ){
  			$this->courseLangvar = 1;
			$this->iltFilterSet=1; //Added/changed by ganeshbabuv on Feb 13th 2017 for  date, country and location filters issues #0071712
  			
  			//Added/changed by ganeshbabuv on Feb 15th 2017 for #0072216: Unable to find the ILT courses according to user timezone in Learner Catalog at Course Level
  			$this->select->condition(db_and()->
  					condition('clsRollup.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl'),'IN')->
  					condition('clsRollup.id', 1 , '>=')->
  					where("CAST('$startdate' AS DATETIME) <= CONVERT_TZ(CONCAT(DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.start_time),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone),'".$applyTimezone."')")->
    			where("CAST('$enddate' AS DATETIME) >= CONVERT_TZ(CONCAT(DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.start_time),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone),'".$applyTimezone."')")->
    			where("if(DATE_FORMAT(CONVERT_TZ(CONCAT(ses.start_date,' ',ses.start_time),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone),'".$applyTimezone."'), _utf8'%Y-%m-%d')  = '$toDay',
      		 			CAST('$toDayDT' AS DATETIME) <= CONVERT_TZ(CONCAT(DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.start_time),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone),'".$applyTimezone."'),
      			1=1)")
				->condition('clsRollup.status',array('lrn_cls_sts_atv'),'IN')
  			);

  		} else if (!empty($startdate) && $enddate=='' ){
  			$this->courseLangvar = 1;
			$this->iltFilterSet=1; //Added/changed by ganeshbabuv on Feb 13th 2017 for  date, country and location filters issues #0071712
  			//Added/changed by ganeshbabuv on Feb 15th 2017 for #0072216: Unable to find the ILT courses according to user timezone in Learner Catalog at Course Level
  			$this->select->condition(db_and()->
  					condition('clsRollup.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl'),'IN')->
  					condition('clsRollup.id', 1 , '>=')->
  					where("CAST('$startdate' AS DATETIME) <= CONVERT_TZ(CONCAT(DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.start_time) ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone), '".$applyTimezone."' )")->
    			where("if(DATE_FORMAT(CONVERT_TZ(CONCAT(ses.start_date,' ',ses.start_time),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone),'".$applyTimezone."'), _utf8'%Y-%m-%d')  = '$toDay',
    					CAST('$toDayDT' AS DATETIME) <= CONVERT_TZ(CONCAT(DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.start_time),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone), '".$applyTimezone."' ),
      			1=1)")
      			->condition('clsRollup.status',array('lrn_cls_sts_atv'),'IN')
  			);
  		} else if ($startdate=='' && !empty($enddate)){
  			$this->courseLangvar = 1;
			$this->iltFilterSet=1; //Added/changed by ganeshbabuv on Feb 13th 2016 for  date, country and location filters issues #0071712
  			$startdate = Date("Y-m-d");
  			//Added/changed by ganeshbabuv on Feb 15th 2017 for #0072216: Unable to find the ILT courses according to user timezone in Learner Catalog at Course Level
  			$this->select->condition(db_and()->
  					condition('clsRollup.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl'),'IN')->
  					condition('clsRollup.id', 1 , '>=')->
      			where("'$enddate' >= CONVERT_TZ(CONCAT(DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.start_time),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone),'".$applyTimezone."')")->
    			where("if(DATE_FORMAT(CONVERT_TZ(CONCAT(ses.start_date,' ',ses.start_time),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone),'".$applyTimezone."'), _utf8'%Y-%m-%d') = '$toDay',
    					CAST('$toDayDT' AS DATETIME) <= CONVERT_TZ(CONCAT(DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.start_time) ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone), '".$applyTimezone."'),
      			1=1)")
				->condition('clsRollup.status',array('lrn_cls_sts_atv'),'IN')
  			);
  		}
  	}catch (Exception $ex) {
  		watchdog_exception('searchTPDate', $ex);
  		expertusErrorThrow($ex);
  	}
  }


  /* -------------------- End Date Search ------------------*/


  /* -------------------- Start MRO Search ------------------*/

	function setTrainingType($mro_type,$isLogin = false,$alias = ''){
	  try{
		if (!empty($mro_type)){
			global $catalog_reg;
      if($catalog_reg == 'Course') 
      	$clsTblName = 'clsRollup';
      else
      	$clsTblName = 'cls';
      
      if(empty($alias))
      	$alias = 'usrmap';
      
	      $mro = explode("|",$mro_type);
	      if(in_array('cre_sys_inv_com',$mro)){
	      	// Modified by Vincent on June 24, 2015 for #0050568
	      		$this->select->where('(IF('.$alias.'.id is null,(if('.$alias.'.mro is not null, '.$alias.'.mro, group_map.mro ) IN (\''.implode('\',\'',$mro).'\')
	      	    AND (catacs.status = \'cre_sec_sts_atv\')), '.$alias.'.mro IN (\''.implode('\',\'',$mro).'\')))
	      	    OR ('.$clsTblName.'.is_compliance = 1 and ( (IF('.$alias.'.id is null,(
             if((select count(1) from slt_groups gp,slt_group_mapping gm where gm.group_id=gp.id and (gm.entity_type = \'cre_sys_obt_crs\' OR gm.entity_type = \'cre_sys_obt_cls\') and gm.entity_id = '.$clsTblName.'.id and gp.status = \'cre_sec_sts_atv\')>0,
             catacs.id is not null, (catacs.id is null))) ,'.$alias.'.id is not null) )
             ))');
	      }
	      else{
	        if($isLogin){
	          $this->select->condition(db_and()->where('if('.$alias.'.mro is not null, '.$alias.'.mro, group_map.mro ) IN (\''.implode('\',\'',$mro).'\')'));
	          // Added by Vincent on June 24, 2015 for #0050568
	          $this->select->condition(db_and()->where('if('.$alias.'.mro is not null,\'cre_sec_sts_atv\', catacs.status) = "cre_sec_sts_atv"'));
	        }
	        else{
  	          // Modified by Vincent on June 24, 2015 for #0050568
  	          $this->select->where('(if('.$alias.'.mro is not null, '.$alias.'.mro, group_map.mro ) IN (\''.implode('\',\'',$mro).'\')
	      	    AND (if('.$alias.'.mro is not null, 1=1, catacs.status = \'cre_sec_sts_atv\'))) ');
	        }
	      }
		//expDebug::dPrintDBAPI("testing >>> ",$this->select);
	    }
      }catch (Exception $ex) {
          watchdog_exception('setTrainingType', $ex);
          expertusErrorThrow($ex);
          }
	}

	function setTrainingTypeTP($mro_type,$isLogin = false){
	  try{
		if (!empty($mro_type)){
	      $mro = explode("|",$mro_type);
	      if($isLogin){
	        $this->select->where('if(usrmap.mro is not null, usrmap.mro, group_map.mro ) IN (\''.implode('\',\'',$mro).'\')');
	        // Added by Vincent on June 24, 2015 for #0050568
	        $this->select->condition(db_and()->where('if(usrmap.mro is not null,"cre_sec_sts_atv", catacs.status) = "cre_sec_sts_atv"'));
	      }
	      else{
  	        // Modified by Vincent on June 24, 2015 for #0050568
            $this->select->where('(group_map.mro IN (\''.implode('\',\'',$mro).'\')
	      	    AND (catacs.status = \'cre_sec_sts_atv\'))');
	      }
	    }
      }catch (Exception $ex) {
          watchdog_exception('setTrainingTypeTP', $ex);
          expertusErrorThrow($ex);
          }
	}
	/* -------------------- End MRO Search ------------------*/

	/* -------------------- Start Rating Search ------------------*/

  function searchRating($rating_type,$type) {
    try{
    if (!empty($rating_type)){

	     $voteValue = explode("|",$rating_type);
	     $length = sizeof($voteValue);
    		 if($length == 1) {
    		      if($type == 'Class') {
    		         $this->select->condition(db_and()->
        		        condition('cls.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl','lrn_cls_dty_wbt','lrn_cls_dty_vod'),'IN')->
    		         		where("vote.value BETWEEN  :minval and :maxval",array(':minval'=>(min($voteValue) - 19),':maxval'=>(max($voteValue))))
        	         );
    		      }else{
    		        $this->select->condition(db_and()->
        		        condition('prm.object_type', array('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn'), 'IN') ->
    		        		where("vote.value BETWEEN  :minval and :maxval",array(':minval'=>(min($voteValue) - 19),':maxval'=>(max($voteValue))))
        	         );
    		      }


    	    }else{
    	    	$str = '';
    	    	for($i=0; $i < $length; $i++){
    	    		$str .= "(vote.value BETWEEN ".($voteValue[$i]-19)." and ".($voteValue[$i]).")";
    	    		if($i != $length-1) {
    	    			$str .= ' OR ';
    	    		}
    	    	}

    	    	if($type == 'Class') {
    		     $this->select->condition(db_and()->
    		        condition('cls.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl','lrn_cls_dty_wbt','lrn_cls_dty_vod'),'IN')->
    		      where("$str")
    		     );
    	    	}else{
    	    	  $this->select->condition(db_and()->
    		        condition('prm.object_type', array('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn'), 'IN') ->
    		      where("$str")
    		     );
    	    	}
    	    }
  	}
  	}catch (Exception $ex) {
          watchdog_exception('searchRating', $ex);
          expertusErrorThrow($ex);
    }
  }
  function searchCourseRating($rating_type,$type) {
  	try{
  		if (!empty($rating_type)){
  			$voteValue = explode("|",$rating_type);
  			$length = sizeof($voteValue);
  			if($length == 1) {
  				//$minV = min($voteValue) - 15;
  				//$maxV = max($voteValue) + 5;

  				$this->select->condition(db_and()->
  						condition('clsRollup.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl','lrn_cls_dty_wbt','lrn_cls_dty_vod'),'IN')

  				);
    		    $this->select->having("avgvote BETWEEN  :minval and :maxval",array(':minval'=>(min($voteValue) - 19),':maxval'=>(max($voteValue))));
  			}else{
  				//$str = 'db_or()->';
  				$str = '';
  				//$val = '';
  				for($i=0; $i < $length; $i++){
  					$str .= "(avgvote BETWEEN ".($voteValue[$i]-19)." and ".($voteValue[$i]).")";
  					/*  $str1 = "(avgvote BETWEEN  :minval$i and :maxval$i)";
  					$val = "':minval$i'=>($voteValue[$i]-15),':maxval$i'=>($voteValue[$i]+5)";
  					$str .= "condition(".$str1." ,array(".$val."))";  */
  					if($i != $length-1) {
  						$str .= ' OR ';
  						//$str .= '->';
  						//$val .= ',';
  					}
  				}
  					//$str = "(".$str." ,array(".$val."))";
  					expDebug::dPrint('araay string rate-->>'.$str,4);
  					$this->select->condition(db_and()->
  							condition('clsRollup.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl','lrn_cls_dty_wbt','lrn_cls_dty_vod'),'IN')
  					);
  					$this->select->having("$str");

  			}
  		}
  	}catch (Exception $ex) {
  		watchdog_exception('searchCourseRating', $ex);
  		expertusErrorThrow($ex);
  	}
  }

  /* -------------------- End Rating Search ------------------*/

  function setSearchParamClass($titleSrch=false,$compliance=''){
    try{
    if($titleSrch) {
      $this->searchTitleClass($this->setParamTitleVal('title'));
    }
    $this->searchTypeClass($this->setParamVal('dl_type'),$this->setParamVal('ob_type'));
    if($compliance !='auto_register'){
     $this->searchLanguageClass($this->setParamVal('lg_type'));
    }
    if(module_exists('exp_sp_commerce')) {
      $this->searchPriceClass($this->setParamVal('price'));
    }
    $this->searchTag($_REQUEST['tag']);
    $this->setTrainingType($this->setParamVal('mro_type'));
    $this->searchJobrole($this->setParamVal('jr_type'));
    $this->searchCountry($this->setParamVal('cy_type'));
    $this->searchLocation($this->setParamVal('location', false));
    $this->searchDate($this->setParamVal('startdate'), $this->setParamVal('enddate'));
    if(!empty($_SESSION['availableFunctionalities']->exp_sp_fivestar)) {
  		$this->searchRating($this->setParamVal('rating_type'),'Class');
  		expDebug::dPrint(' $selectedFilters = ' . print_r($this->searchRating, true) , 4);
    }
    }catch (Exception $ex) {
          watchdog_exception('setSearchParamClass', $ex);
          expertusErrorThrow($ex);
    }
  }
  function setSearchParamCourse($titleSrch=false){
  	global $catalog_reg;
  	try{
  		if($titleSrch) {
  			$this->searchTitleCourse($this->setParamTitleVal('title'));
  		}
  		$this->searchCourseTag($_REQUEST['tag']);
  		//$this->searchJobrole($this->setParamVal('jr_type'));
  		$this->searchCourseLocation($this->setParamVal('location', false));
  		$this->setTrainingType($this->setParamVal('mro_type'),false,'umap');
  		$this->searchCourseDate($this->setParamVal('startdate'), $this->setParamVal('enddate'));

  		if(module_exists('exp_sp_commerce')) {
  			$this->searchPriceCourse($this->setParamVal('price'));
  		}
  		//$this->setTrainingType($this->setParamVal('mro_type'));
  		$this->searchJobrole($this->setParamVal('jr_type'));
  		$this->searchCourseCountry($this->setParamVal('cy_type'));
  		$this->searchLanguageCourse($this->setParamVal('lg_type'));
  	}catch (Exception $ex) {
  		watchdog_exception('setSearchParamCourse', $ex);
  		expertusErrorThrow($ex);
  	}
  }

  function setSearchParamTP($titleSrch=false){
    try{
    if($titleSrch) {
      $this->searchTitleTP($this->setParamTitleVal('title'));
    }
    $this->searchTypeTP($this->setParamVal('dl_type'),$this->setParamVal('ob_type'));
    $this->searchLanguageTP($this->setParamVal('lg_type'));
    if(module_exists('exp_sp_commerce')) {
      $this->searchPriceTP($this->setParamVal('price'));
    }
    $this->searchTag($_REQUEST['tag']);
    $this->searchJobrole($this->setParamVal('jr_type'));
    $this->setTrainingTypeTP($this->setParamVal('mro_type'));
	
	//Added for #0066426 - Changes done by vetrivel
    $this->searchTPCountry($this->setParamVal('cy_type'));
    $this->searchTPDate($this->setParamVal('startdate'), $this->setParamVal('enddate'));
    $this->searchTPLocation($this->setParamVal('location', false));
    //End of #0066426
	
	
	
    if(!empty($_SESSION['availableFunctionalities']->exp_sp_fivestar)) {
    	$this->searchRating($this->setParamVal('rating_type'),'LP');
    }
    }catch (Exception $ex) {
          watchdog_exception('setSearchParamTP', $ex);
          expertusErrorThrow($ex);
    }
  }

  function catalogSearchQueryPageCount(){
    try{
      global $catalog_reg;

     // expDebug::dPrint('variable check form api course or class'.$catalog_reg);
      if($catalog_reg == 'Course') {
        $this->select = catalogCourseSearchQuery();
        $this->setSearchParamCourse(true);
        $this->select->condition('crs.status','lrn_crs_sts_atv','=');
       // $this->select->groupby('crs.id');
        if(!empty($_SESSION['availableFunctionalities']->exp_sp_fivestar)) {
        	$this->searchCourseRating($this->setParamVal('rating_type'),'Course');
        }
        /* $courseQuery = 'gmap.entity_id=crs.id AND gmap.entity_type = \'cre_sys_obt_crs\'';
        $classQuery = 'gmap.entity_id=cls.id AND gmap.entity_type = \'cre_sys_obt_cls\''; */
        /* if($GLOBALS["user"]->uid != 1){
        		$this->select->where('if(group_map.id is not null,group_map.id = (select gmap.id from slt_group_mapping gmap where ('.$courseQuery.') and gmap.group_type=0 order by mro asc limit 1)
        		or group_map.id = (select gmap.id from slt_group_mapping gmap where ('.$classQuery.') and gmap.group_type=0 order by mro asc limit 1),1=1)');
        } */
        $alisename = 'crs';
        $entity_type = 'cre_sys_obt_crs';
      } else {
        $this->select = catalogClassSearchQuery();
        $this->setSearchParamClass(true);
        $this->select->condition('cls.status','lrn_cls_sts_atv','=');
       // $this->select->groupby('cls.id');
        $this->select->where('cls.dedicated_class_flag = \'N\' or cls.dedicated_class_flag is null');
        /* $classsr_Query = 'if((select count(1) from slt_group_mapping grp_mapp where grp_mapp.entity_id = cls.id AND grp_mapp.entity_type = \'cre_sys_obt_cls\')>0,(gmap.entity_id=cls.id
        AND gmap.entity_type = \'cre_sys_obt_cls\'),(gmap.entity_id=cls.course_id AND gmap.entity_type = \'cre_sys_obt_crs\'))';
        if($GLOBALS["user"]->uid != 1){
        	$this->select->where('if(group_map.id is not null,group_map.id = (select gmap.id from slt_group_mapping gmap where ('.$classsr_Query.') and gmap.group_type=0 order by mro asc limit 1),1=1)');
        } */
        $alisename = 'cls';
        $entity_type = 'cre_sys_obt_cls';
      }
    filterByUserAccessPrivileges($this->select, $alisename,'','',$entity_type,$catalog_reg);



    $clsQuery=$this->select;

    $this->select = catalogTPSearchQuery();
    $this->setSearchParamTP(true);
    $this->select->condition('prm.status','lrn_lpn_sts_atv','=');
   // $this->select->groupby('prm.id');
    filterByUserAccessPrivileges($this->select, 'prm','','','','Program');
   /*  if($GLOBALS["user"]->uid != 1){
    	$this->select->where('if(group_map.id is not null,group_map.id = (select gmap.id from slt_group_mapping gmap where gmap.entity_id=prm.id and gmap.group_type=0 and gmap.entity_type IN(\'cre_sys_obt_trn\',\'cre_sys_obt_crt\',\'cre_sys_obt_cur\') order by mro asc limit 1),1=1)');
    } */
    $tpQuery=$this->select;

    $resultQuery=$clsQuery->union($tpQuery,'UNION ALL');
    $resultQuery->groupBy('cls_code');

    return $resultQuery;
    }catch (Exception $ex) {
          watchdog_exception('catalogSearchQueryPageCount', $ex);
          expertusErrorThrow($ex);
    }

  }

function catalogSearchQuery(){
  try{
    global $catalog_reg;
    $userDetails 		= getPersonDetails(getIdOfLoggedInUser(), array('hire_date'));
    $usrHireDate		= $userDetails['hire_date'];
    $anonymousUser  = (count($userDetails)) > 0 ? 0 : 1;
    
    $userId  = getIdOfLoggedInUser();
		$userId = ($_REQUEST['apiname'] == 'CatalogSearchAPI' && $_REQUEST['userid']) ? $_REQUEST['userid'] :  $userId;
    
    if($catalog_reg == 'Course') {
      $this->select = catalogCourseSearchQuery();
      $this->setSearchParamCourse(true);
      $this->select->condition('crs.status','lrn_crs_sts_atv','=');
     // $this->select->groupby('crs.id');
      if(!empty($_SESSION['availableFunctionalities']->exp_sp_fivestar)) {
      	$this->searchCourseRating($this->setParamVal('rating_type'),'Course');
      }
      
       $this->select->where('(IF(umap.id is not null and group_map.id is null, umap.user_id = '.$userId.', (if(group_map.id is null,
      		(select count(1) from slt_user_access_mapping um where (um.parent_id=concat(crs.id, \'-\', IFNULL(clsRollup.id,0)) AND um.parent_type = \'learning\') and um.access_type = 0)=0,
		      (if(group_map.entity_type = \'cre_sys_obt_crs\',
       catacs.id is not null OR (select count(1) from slt_course_class cs
                                 left join slt_group_mapping gm on cs.id=gm.entity_id and gm.entity_type = \'cre_sys_obt_cls\'  
                                 where cs.course_id=group_map.entity_id and gm.id is null
                                 )> 0,  
       if(group_map.entity_type = \'cre_sys_obt_cls\',
         catacs.id is not null ,
         (catacs.id is null and umap.id is null))) )))))
       	 and (clsRollup.dedicated_class_flag = \'N\' or clsRollup.dedicated_class_flag is null)');
      // Conditon "OR (select count(1) from slt_group_mapping where entity_id=clsRollup.course_id and entity_type= \'cre_sys_obt_crs\') = 0"
      // removed by Vincent on 06 Apr, 2017 - for in course level registration if the course doesnot have a access and it is a mandatory course
      // in caralog it shows mandatory but while click register the class not shown (in case of multiple classes under the course)
      
      $alisename = 'crs';
      $entity_type = 'cre_sys_obt_crs';
      if($anonymousUser)
      	$this->select->where('(IF(crs.compliance_completed = \'hire_days\' or crs.compliance_completed = \'date\',1=0,1=1))');
 	  else
      $this->select->where('(IF(crs.compliance_completed = \'hire_days\' AND "'.$usrHireDate.'" != "",(DATE_ADD(DATE_FORMAT("'.$usrHireDate.'", "%Y-%m-%d"), INTERVAL crs.complete_days DAY) >= CURDATE()),IF(crs.complete_date IS NOT NULL AND crs.complete_date != "",crs.complete_date >= CURDATE(),1=1)))');
    }else {
      $this->select = catalogClassSearchQuery('',false,'catalogquery');
      $this->setSearchParamClass(true);
    //  $this->select->groupby('cls.id');
      $this->select->condition('cls.status','lrn_cls_sts_atv','=');
      $this->select->where('cls.dedicated_class_flag = \'N\' or cls.dedicated_class_flag is null');
      /* $classsr_Query = 'if((select count(1) from slt_group_mapping grp_mapp where grp_mapp.entity_id = cls.id  AND grp_mapp.entity_type = \'cre_sys_obt_cls\')>0,(gmap.entity_id=cls.id
      AND gmap.entity_type = \'cre_sys_obt_cls\'),(gmap.entity_id=cls.course_id AND gmap.entity_type = \'cre_sys_obt_crs\'))';
      if($GLOBALS["user"]->uid != 1){
      	$this->select->where('if(group_map.id is not null,group_map.id = (select gmap.id from slt_group_mapping gmap where ('.$classsr_Query.') and gmap.group_type=0 order by mro asc limit 1),1=1)');
      } */
      $alisename = 'cls';
      $entity_type = 'cre_sys_obt_cls';
      if($anonymousUser)
      	$this->select->where('(IF(templ.compliance_completed = \'hire_days\' or templ.compliance_completed = \'date\',1=0,1=1))');
      else 
      $this->select->where('(IF(templ.compliance_completed = \'hire_days\' AND "'.$usrHireDate.'" != "",(DATE_ADD(DATE_FORMAT("'.$usrHireDate.'", "%Y-%m-%d"), INTERVAL templ.complete_days DAY) >= CURDATE()),IF(templ.complete_date IS NOT NULL AND templ.complete_date != "",DATE_FORMAT(templ.complete_date,"%Y-%m-%d") >= CURDATE(),1=1)))');
    
    }
    if((isset($_REQUEST['apiname']) && ($_REQUEST['apiname'] == 'CatalogSearchAPI') && $catalog_reg == 'Course') || $catalog_reg != 'Course'){
    filterByUserAccessPrivileges($this->select, $alisename,'','',$entity_type,$catalog_reg);
    }
   
    $clsQuery=$this->select;

	$this->select = catalogTPSearchQuery('','catalogquery');  
   
    $this->setSearchParamTP(true); 
    $this->select->condition('prm.status','lrn_lpn_sts_atv','='); 
   // $this->select->groupby('prm.id');
 
   if($this->iltFilterSet=='1'){ //Added/changed by ganeshbabuv on Feb 13th 2016 for  date, country and location filters issues #0071712

  		  catalogTPCourseAccessQuery($this->select,'');    
    	  $this->select->where('(((map.id is null or map.id=\'\') and (grp.id is null or grp.id=\'\')) or (map.id!=\'\' and grp.id!=\'\'))'); 

		    $timeZone = date_default_timezone(false);
		    $defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
		    $sessionTZ = new DateTimeZone($defaultTimezone);
		    $toDay = new DateTime(null,$sessionTZ) ;
		    $toDayDT = $toDay->format('Y-m-d H:i'); 

	        $cls_aliase = 'clsRollup' ;			 

		    $this->select->leftJoin('slt_course_class_session','ses_past','ses_past.class_id='.$cls_aliase.'.id and ses_past.course_id='.$cls_aliase.'.course_id 
		    and CONVERT_TZ(CONCAT(DATE_FORMAT(ses_past.start_date, _utf8\'%Y-%m-%d\'),\' \',ses_past.start_time),(select pro_past.attr2 from slt_profile_list_items pro_past where pro_past.code= ses_past.timezone),\''.$defaultTimezone.'\')<=\''.$toDayDT.'\'');

			$this->select->where('ses_past.id is null or ses_past.id =\'\'');  
  }

  filterByUserAccessPrivileges($this->select, 'prm','','','','Program');
 /*  if($GLOBALS["user"]->uid != 1){
    $this->select->where('if(group_map.id is not null,group_map.id = (select gmap.id from slt_group_mapping gmap where gmap.entity_id=prm.id and gmap.group_type=0 and gmap.entity_type IN(\'cre_sys_obt_trn\',\'cre_sys_obt_crt\',\'cre_sys_obt_cur\') order by mro asc limit 1),1=1)');
  } */
    //$this->searchSortBy($this->setParamVal('sortby'));
    $this->select->orderBy('ascen','asc');

    $tpQuery=$this->select;

    $resultQuery=$clsQuery->union($tpQuery,'UNION ALL');
    $rating = isset($_GET['rating_type']) ? $_GET['rating_type'] : '';
    if(isset($_REQUEST['apiname']) ){
    	$rating = $_REQUEST['rating_type'];
    }
    if($catalog_reg == 'Course' && !empty($rating))
    	$resultQuery->groupBy('cls_code');
    return $resultQuery;
    }catch (Exception $ex) {
          watchdog_exception('catalogSearchQuery', $ex);
          expertusErrorThrow($ex);
    }

  }
function searchResultsQry(){
   try{

    $delType = $this->setParamVal('dl_type');
    $obType = $this->setParamVal('ob_type');

    $contrytype = $this->setParamVal('cy_type');
    $locationtype = $this->setParamVal('location'); //$this->setParamVal('location', false);

    $startdate = $this->setParamVal('startdate');
    $enddate = $this->setParamVal('enddate');
    //(!isset($_GET['dl_type'])) ||
    $courseId = '';
    if(isset($_REQUEST['apiname']) && ($_REQUEST['apiname'] == 'CatalogSearchAPI')){
      $courseId = escape_string($_REQUEST['course_id']);
      if(empty($courseId) && !empty($_REQUEST['course_code'])){
        $selectCrsId = db_select('slt_course_template', 'crt');
        $selectCrsId -> addField('crt', 'id');
        $selectCrsId -> condition('crt.code',escape_string($_REQUEST['course_code']));
        $selectCrsId -> range(0,1);
        $courseId = $selectCrsId -> execute() -> fetchField();
      }
    }
    expDebug::dPrint('searchResultsQry course id:' . $courseId);
	
	/* Added & Modified for #0066426 - By vetrivel and ganesh */
	
	$delType=trim($delType);
	$obType=trim($obType);
	
	if($delType!='' && $obType==''){
		$search_mode='deliverytype';
	}else if($delType=='' && $obType!=''){
		$search_mode='objecttype';
	}else{
		$search_mode='false';
	}
	
   if(!empty($courseId)){
     // get courseid from coursecode
     $this->catalogSearchQueryClass();
      if($_REQUEST['object_type']=='Course')
      	$this->select->condition('crs.id',$courseId);
      else
      $this->select->condition('cls.course_id',$courseId);
      $this->baseQuery = $this->select;
      $this->select = $this->baseQuery;
      //$this->searchSortBy($this->setParamVal('sortby'));
      $this->select->orderBy('ascen','asc');
     // $this->select->range($this->start, $this->limit);
      //Embed widget related work
  	 }else if(($search_mode=='deliverytype') || ($search_mode=='deliverytype' && !empty($contrytype)) || ($search_mode=='deliverytype' && !empty($locationtype)) || ($search_mode=='deliverytype' && !empty($startdate)) || ($search_mode=='deliverytype' && !empty($enddate)) || ((isset($_GET['ent_type']) && ($_GET['ent_type'] == 'cre_sys_obt_cls' || $_GET['ent_type'] == 'cre_sys_obt_crs')) && isset($_GET['ent_id']))){    		
      $this->catalogSearchQueryClass();
      $this->baseQuery = $this->select;
      $this->select = $this->baseQuery;
     // $this->searchSortBy($this->setParamVal('sortby'));
      $this->select->orderBy('ascen','asc');
      //$this->select->range($this->start, $this->limit);
      //Embed widget related work
   	 }else if(($search_mode=='objecttype') || ($search_mode=='objecttype' && !empty($contrytype)) || ($search_mode=='objecttype' && !empty($locationtype)) || ($search_mode=='objecttype' && !empty($startdate)) || ($search_mode=='objecttype' && !empty($enddate)) || ((isset($_GET['ent_type']) && $_GET['ent_type'] == 'cre_sys_obt_trp') && isset($_GET['ent_id']))){
      $this->catalogSearchQueryTP();
      $this->baseQuery = $this->select;
      $this->select = $this->baseQuery;
    //  $this->searchSortBy($this->setParamVal('sortby'));
      $this->select->orderBy('ascen','asc');
     // $this->select->range($this->start, $this->limit);
    }else{
      $this->select = $this->catalogSearchQuery();
      expDebug::dPrintDBAPI('Count for Catalog Search ',$this);
    }
    }catch (Exception $ex) {
          watchdog_exception('searchResultsQry', $ex);
          expertusErrorThrow($ex);
    }
  }

  function catalogSearchQueryClass($login='',$compliance=''){
    try{
      global $catalog_reg;
      if($catalog_reg == 'Course') {
        $this->select = catalogCourseSearchQuery();
        // Condition enabled by Vincent for #0036993 (dont know why this line commented earlier)
        // Any other issue came because of this let consider the above.
        $this->select->condition('crs.status','lrn_crs_sts_atv','=');
        //Embed widget related work
        if(isset($_GET['ent_type']) && isset($_GET['ent_id']))
        		$this->searchByEntity();
        $this->setSearchParamCourse(true);
        //$this->select->groupby('crs.id'); Commented for the Catalog -> course filter Issue
        if(!empty($_SESSION['availableFunctionalities']->exp_sp_fivestar)) {
        	$this->searchCourseRating($this->setParamVal('rating_type'),'Course');
        }
       $this->select->where('if(group_map.entity_type = \'cre_sys_obt_crs\',
       catacs.id is not null OR (select count(1) from slt_course_class cs
                                 left join slt_group_mapping gm on cs.id=gm.entity_id and gm.entity_type = \'cre_sys_obt_cls\'  
                                 where cs.course_id=group_map.entity_id and gm.id is null
                                 )> 0,  
       if(group_map.entity_type = \'cre_sys_obt_cls\',
         catacs.id is not null OR (select count(1) from slt_group_mapping where entity_id=clsRollup.course_id and entity_type= \'cre_sys_obt_crs\') = 0,
         1=1)) AND (clsRollup.dedicated_class_flag = \'N\' or clsRollup.dedicated_class_flag is null)');
        $alisename = 'crs';
        $entity_type = 'cre_sys_obt_crs';
      }else {
        $this->select = catalogClassSearchQuery('',false,'catalogquery');
       $this->setSearchParamClass(true,$compliance);
        if($login == 'login'){
        	$this->select->groupby('cls.course_id');
        }else{
        	//$this->select->groupby('cls.id');
        }
        $this->select->condition('cls.status','lrn_cls_sts_atv','=');
        //Embed widget related work
        if(isset($_GET['ent_type']) && isset($_GET['ent_id']))
        		$this->searchByEntity();
        $this->select->where('cls.dedicated_class_flag = \'N\' or cls.dedicated_class_flag is null');
        /* $classsr_Query = 'if((select count(1) from slt_group_mapping grp_mapp where grp_mapp.entity_id = cls.id AND grp_mapp.entity_type = \'cre_sys_obt_cls\')>0,(gmap.entity_id=cls.id
        AND gmap.entity_type = \'cre_sys_obt_cls\'),(gmap.entity_id=cls.course_id AND gmap.entity_type = \'cre_sys_obt_crs\'))';
        if($GLOBALS["user"]->uid != 1){
        	$this->select->where('if(group_map.id is not null,group_map.id = (select gmap.id from slt_group_mapping gmap where ('.$classsr_Query.') and gmap.group_type=0  order by mro asc limit 1),1=1)');
        } */
        $alisename = 'cls';
        $entity_type = 'cre_sys_obt_cls';
      }
      if((isset($_REQUEST['apiname']) && ($_REQUEST['apiname'] == 'CatalogSearchAPI') && $catalog_reg == 'Course') || $catalog_reg != 'Course'){
          filterByUserAccessPrivileges($this->select, $alisename,'','',$entity_type,$catalog_reg,'','','','',$login);
      }
          
          $rating = isset($_GET['rating_type']) ? $_GET['rating_type'] : '';
          if(isset($_REQUEST['apiname']) ){
          	$rating = $_REQUEST['rating_type'];
          }
          if($catalog_reg == 'Course' && !empty($rating))
          	$this->select->groupBy('cls_code'); 

      //and if(group_map.id is not null,group_map.id = (select gmap.id from slt_group_mapping gmap where gmap.entity_id=cls.id and gmap.group_type=0 and gmap.entity_type = 'cre_sys_obt_cls' order by mro asc limit 1),1=1)

    }catch (Exception $ex) {
          watchdog_exception('catalogSearchQueryClass', $ex);
          expertusErrorThrow($ex);
    }
  }
  function catalogSearchQueryClassCron($update='',$compliance=''){
  	try{
  		global $catalog_reg;
  		if($catalog_reg == 'Course') {
  			$this->select = catalogCourseSearchQueryCron();
  			//$this->select->condition('crs.status','lrn_crs_sts_atv','=');
  			$this->setSearchParamCourse(true);
  			$this->select->groupby('crs.id')
  			             ->groupby('person.id');
  			// Added for #0049456
	        $this->select->groupby('catacs.id');
  			if(!empty($_SESSION['availableFunctionalities']->exp_sp_fivestar)) {
  				$this->searchCourseRating($this->setParamVal('rating_type'),'Course');
  			}
  			$this->select->condition('person.status','cre_usr_sts_atv','=');
  			$this->select->condition('person.id',1,'!=');
  			$this->select->condition('person.id',2,'!=');
  			if($update == ''){
  			  $this->select->condition('enroll.id','','is null');
  			  $this->select->condition('temp.id','','is null');
  			}else{
  				$this->select->condition(db_and()->condition('enroll.id','','is not null')
  						->condition('enroll.is_compliance','','is null'));
  			}

  			/*  $courseQuery = 'gmap.entity_id=crs.id AND gmap.entity_type = \'cre_sys_obt_crs\'';
  			 $classQuery = 'gmap.entity_id=cls.id AND gmap.entity_type = \'cre_sys_obt_cls\'';
  			if($GLOBALS["user"]->uid != 1){
  			$this->select->where('if(group_map.id is not null,group_map.id = (select gmap.id from slt_group_mapping gmap where ('.$courseQuery.') and gmap.group_type=0  order by mro asc limit 1)
  					or group_map.id = (select gmap.id from slt_group_mapping gmap where ('.$classQuery.') and gmap.group_type=0  order by mro asc limit 1),1=1)');
  			} */
  			$alisename = 'crs';
  			$entity_type = 'cre_sys_obt_crs';
  			$this->select->where('IF(crs.complete_date IS NOT NULL AND crs.complete_date != "",DATE_FORMAT(crs.complete_date,"%Y-%m-%d") >= CURDATE(),1=1)');
  			$this->select->where('IF(crs.validity_date IS NOT NULL AND crs.validity_date != "",DATE_FORMAT(crs.validity_date,"%Y-%m-%d") >= CURDATE(),1=1 )');
  		}else {
  			$this->select = catalogClassSearchQueryCron();
  			$this->setSearchParamClass(true,$compliance);
  				$this->select->groupby('cls.id');
  				// Added for #0049456
  				$this->select->groupby('catacs.id');

  				             //->groupby('person.id');
  			$this->select->condition('cls.status','lrn_cls_sts_atv','=');
  			//$this->select->condition('person.status','cre_usr_sts_atv','=');
  			//$this->select->condition('person.id',1,'!=');
  		//	$this->select->condition('person.id',2,'!=');
  		  if($update == ''){
  			 // $this->select->condition('enroll.id','','is null');
  			 // $this->select->condition('temp.id','','is null');
  			}else{
  				//$this->select->condition(db_and()->condition('enroll.id','','is not null')
  					//	->condition('enroll.is_compliance','','is null'));

  			}
  			$this->select->where('cls.dedicated_class_flag = \'N\' or cls.dedicated_class_flag is null');
  			/* $classsr_Query = 'if((select count(1) from slt_group_mapping grp_mapp where grp_mapp.entity_id = cls.id AND grp_mapp.entity_type = \'cre_sys_obt_cls\')>0,(gmap.entity_id=cls.id
  			 AND gmap.entity_type = \'cre_sys_obt_cls\'),(gmap.entity_id=cls.course_id AND gmap.entity_type = \'cre_sys_obt_crs\'))';
  			if($GLOBALS["user"]->uid != 1){
  			$this->select->where('if(group_map.id is not null,group_map.id = (select gmap.id from slt_group_mapping gmap where ('.$classsr_Query.') and gmap.group_type=0  order by mro asc limit 1),1=1)');
  			} */
  			$alisename = 'cls';
  			$entity_type = 'cre_sys_obt_cls';
  			$this->select->where('IF(templ.complete_date IS NOT NULL AND templ.complete_date != "",DATE_FORMAT(templ.complete_date,"%Y-%m-%d") >= CURDATE(),1=1)');
  			$this->select->where('IF(templ.validity_date IS NOT NULL AND templ.validity_date != "",DATE_FORMAT(templ.validity_date,"%Y-%m-%d") >= CURDATE(),1=1 )');
  		}

  		filterByUserAccessPrivilegesCron($this->select, $alisename,'','',$entity_type,$catalog_reg,'','','cron',$update);

  		//and if(group_map.id is not null,group_map.id = (select gmap.id from slt_group_mapping gmap where gmap.entity_id=cls.id and gmap.group_type=0 and gmap.entity_type = 'cre_sys_obt_cls' order by mro asc limit 1),1=1)

  	}catch (Exception $ex) {
  		watchdog_exception('catalogSearchQueryClass', $ex);
  		expertusErrorThrow($ex);
  	}
  }
  function catalogSearchQueryTP(){
    try{
      $this->select = catalogTPSearchQuery('','catalogquery');
      //Embed widget related work
      if(isset($_GET['ent_type']) && isset($_GET['ent_id']))
      	$this->searchByEntity();
      $this->setSearchParamTP(true);
      $this->select->condition('prm.status','lrn_lpn_sts_atv','=');
      //$this->select->groupby('prm.id');
	  
	    /* Added/changed by ganeshbabuv on Feb 13th 2016 for  date, country and location filters issues #0071712 */
  		 if($this->iltFilterSet=='1'){ 
  		   catalogTPCourseAccessQuery($this->select,'');    
    	   $this->select->where('(((map.id is null or map.id=\'\') and (grp.id is null or grp.id=\'\')) or (map.id!=\'\' and grp.id!=\'\'))');
			     
			    $timeZone = date_default_timezone(false);
			    $defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
			    $sessionTZ = new DateTimeZone($defaultTimezone);
			    $toDay = new DateTime(null,$sessionTZ) ;
			    $toDayDT = $toDay->format('Y-m-d H:i');
				
		        $cls_aliase = 'clsRollup' ;			 
				
   			    $this->select->leftJoin('slt_course_class_session','ses_past','ses_past.class_id='.$cls_aliase.'.id and ses_past.course_id='.$cls_aliase.'.course_id 
		   		 and CONVERT_TZ(CONCAT(DATE_FORMAT(ses_past.start_date, _utf8\'%Y-%m-%d\'),\' \',ses_past.start_time),(select pro_past.attr2 from slt_profile_list_items pro_past where pro_past.code= ses_past.timezone),\''.$defaultTimezone.'\')<=\''.$toDayDT.'\'');
				
				$this->select->where('ses_past.id is null or ses_past.id =\'\'');  
        }

      filterByUserAccessPrivileges($this->select, 'prm','','','','Program');
      /* if($GLOBALS["user"]->uid != 1){
      	$this->select->where('if(group_map.id is not null,group_map.id = (select gmap.id from slt_group_mapping gmap where gmap.entity_id=prm.id and gmap.group_type=0 and gmap.entity_type IN(\'cre_sys_obt_trn\',\'cre_sys_obt_crt\',\'cre_sys_obt_cur\') order by mro asc limit 1),1=1)');
      } */
    }catch (Exception $ex) {
          watchdog_exception('catalogSearchQueryTP', $ex);
          expertusErrorThrow($ex);
    }
  }

  function catalogSearchQueryTPCron(){
  	try{
  		$this->select = catalogTPSearchQueryCron();
  		$this->setSearchParamTP(true);
  		$this->select->condition('prm.status','lrn_lpn_sts_atv','=');
  		//$this->select->condition('person.status','cre_usr_sts_atv','=');
  		//$this->select->condition('person.id',1,'!=');
  	//	$this->select->condition('person.id',2,'!=');
  	//	$this->select->condition('master_enroll.id','','is null');
  		//$this->select->condition('temp.id','','is null');
  		$this->select->groupby('prm.id');
  		//Added for #0049456
  		$this->select->groupby('catacs.id');
  		             //->groupby('person.id');
  		filterByUserAccessPrivilegesCron($this->select, 'prm','','','','Program','','','cron');
  		/* if($GLOBALS["user"]->uid != 1){
  		 $this->select->where('if(group_map.id is not null,group_map.id = (select gmap.id from slt_group_mapping gmap where gmap.entity_id=prm.id and gmap.group_type=0 and gmap.entity_type IN(\'cre_sys_obt_trn\',\'cre_sys_obt_crt\',\'cre_sys_obt_cur\') order by mro asc limit 1),1=1)');
  		} */
  	}catch (Exception $ex) {
  		watchdog_exception('catalogSearchQueryTP', $ex);
  		expertusErrorThrow($ex);
  	}
  }



  function calculatePagination($skipAccess='') {
    try{
    	global $catalog_reg;
    	expDebug::dPrint('checking the agrument 1'. arg(1));

    $this->select = db_select($this->select,'acc');
    $this->select->addField('acc','cls_id');
    $this->select->addField('acc','cls_code');
    $this->select->addField('acc','cls_title');
    $this->select->addField('acc','cls_short_description');
    $this->select->addField('acc','price');
    $this->select->addField('acc','currency_type');
    $this->select->addField('acc','registration_end_on');
    $this->select->addField('acc','created_on');
    $this->select->addField('acc','published_on');
    if($skipAccess!=1)
    $this->select->addField('acc','mro_id');
    $this->select->addField('acc','start_date');
    $this->select->addField('acc','delivery_type_name');
    $this->select->addField('acc','export_compliance');
    $this->select->addField('acc','expires_in_value');
    $this->select->addField('acc','expires_in_unit');
    $this->select->addField('acc','prm_created_on');
    $this->select->addField('acc','object_type');
    $this->select->addField('acc','is_compliance');
    $this->select->addField('acc','avgvote');

    //0066800->condition added for mobile api request.
    if($catalog_reg == 'Class' || arg(1) == 'courselevel-search' || arg(1) == 'lp-change-class' || arg(1) == 'order' || $_REQUEST['apiname'] == 'getAvailableClassesAPI'){
    	$this->select->addField('acc','language_code');
    	$this->select->addField('acc','language');
    	$this->select->addField('acc','location_id');
    	$this->select->addField('acc','crs_id');
    	$this->select->addField('acc','delivery_type_code');
    	$this->select->groupBy('acc.cls_id');
    	$this->select->groupBy('acc.crs_id');
    }else{
    	$this->select->addExpression('GROUP_CONCAT(DISTINCT(acc.language_code))', 'language_code');
    	$this->select->addExpression('GROUP_CONCAT(DISTINCT(acc.language))', 'language');
    	$this->select->addExpression('GROUP_CONCAT(acc.location_id)', 'location_id');
    	$this->select->addExpression('GROUP_CONCAT(DISTINCT(acc.crs_id))', 'crs_id');
    	$this->select->addExpression('COUNT(DISTINCT(acc.cls_count))', 'cls_count');
    	$this->select->addExpression('GROUP_CONCAT(DISTINCT(acc.delivery_type_code))', 'delivery_type_code');
    	$this->select->groupBy('acc.cls_id');
    	$this->select->groupBy('acc.object_type');
    }



    $exeQuery = $this->select;
    $exeQuery = $exeQuery->extend("ExpertusSelectQueryExtender");
		//$exeQuery->setIndex(array('cls'=>'clssts','prm'=>'tpsts'));
		$list= $exeQuery->execute()->fetchCol();

    $this->qry_search_count = 0;
    $this->qry_search_count = count($list);

    if ( $this->qry_search_count > 0 ) {
      $this->total_pages = ceil($this->qry_search_count/$this->limit);
    } else {
      $this->total_pages = 0;
    }
    if($_GET['apiname'] != 'CatalogSearchAPI'){
      if ($this->page > $this->total_pages) $this->page=$this->total_pages;
    }
    $this->start = $this->limit*$this->page - $this->limit;
    if ($this->start < 0) {
      $this->start = 0;
    }
    }catch (Exception $ex) {
          watchdog_exception('calculatePagination', $ex);
          expertusErrorThrow($ex);
    }
  }

  function getSearchResults($callFrom) {
    try{
    	//Below check added by Vincent on Dec 2, 2013 for catalog search from API
    	// If the catalog search is set to Course it should be reset to Class if the call is from API
     	// TODO: This check should be removed once the Course search is implemented in Mobile app.
    	//global $catalog_reg;
    	//if($callFrom=='API')
  			//$catalog_reg = 'Course';
    	global $catalog_reg;

    	if($callFrom)
    		$UserDefaultCurrencyArr = getUserDefaultCurrency($callFrom);
    	else
    		$UserDefaultCurrencyArr = getUserDefaultCurrency();
  		$UserPrefCurrCode = $UserDefaultCurrencyArr['currency_code'];
  		$UserPrefCurrSym = $UserDefaultCurrencyArr['currency_sym'];
  		$UserPrefCurrLongCode = $UserDefaultCurrencyArr['currency_long_code'];

	    $this -> searchResultsQry();
	    $this->select = db_select($this->select,'acc');
	    $this->select->addField('acc','cls_id');
	    $this->select->addField('acc','cls_code');
	    $this->select->addField('acc','cls_title');
	    $this->select->addField('acc','cls_short_description');
	
	    $this->select->addExpression('if(acc.price is not null and acc.price > 0, slf_convert_price(acc.price,acc.currency_type,\''.$UserPrefCurrCode.'\'), acc.price)','price');
			$this->select->addExpression('\''.$UserPrefCurrSym.'\'','currency_symbol');
			$this->select->addExpression('\''.$UserPrefCurrCode.'\'','currency_type');
	    $this->select->addField('acc','price','base_price');
	   	$this->select->addField('acc','currency_type','base_currency_type');
	   	if($callFrom){
	   	$this->select->addField('acc','currency_symbol', 'base_currency_symbol');
	   		$this->select->addField('acc','delivery_type_name');
	   	}
	    $this->select->addField('acc','registration_end_on');
	    $this->select->addField('acc','created_on');
	    $this->select->addField('acc','published_on');
	    $this->select->addField('acc','mro_id');
	    //$this->select->addField('acc','language_code');
//	    $this->select->addField('acc','language');
	    $this->select->addField('acc','start_date');
//	    $this->select->addField('acc','location_id');
	    //$this->select->addField('acc','crs_id');
//	    $this->select->addField('acc','delivery_type_code');
	    $this->select->addField('acc','export_compliance');
	    $this->select->addField('acc','expires_in_value');
	    $this->select->addField('acc','expires_in_unit');
	    $this->select->addField('acc','prm_created_on');
	    $this->select->addField('acc','object_type');
	    $this->select->addField('acc','is_compliance');
	    $this->select->addField('acc','avgvote');
	    if($catalog_reg == 'Class'){
	    	if($callFrom){
	    	$this->select->addField('acc','language_code');
	    	$this->select->addField('acc','language');
	    	$this->select->addField('acc','location_id');
	    	$this->select->addField('acc','crs_id');
	    	$this->select->addField('acc','delivery_type_code');
	    	$this->select->addExpression('acc.tagname','tagname');
	    	}
	    	
	    	$this->select->groupBy('acc.cls_id');
	    	$this->select->groupBy('acc.crs_id');

	    	if(isset($_REQUEST['mobile_request'])){
	    		$this->select->addField('acc','additional_info');
	    		$this->select->addField('acc','addn_catalog_show');
	    	}
	    	
	    }else{
	    	if($callFrom){
	    	$this->select->addExpression('GROUP_CONCAT(DISTINCT(acc.language_code))', 'language_code');
	    	$this->select->addExpression('GROUP_CONCAT(DISTINCT(acc.language))', 'language');
	    	$this->select->addExpression('GROUP_CONCAT(acc.location_id)', 'location_id');
	    	$this->select->addExpression('GROUP_CONCAT(DISTINCT(acc.crs_id))', 'crs_id');
	    	  $this->select->addExpression('COUNT(DISTINCT(acc.crs_id))', 'cls_count');
	    	$this->select->addExpression('GROUP_CONCAT(DISTINCT(acc.delivery_type_code))', 'delivery_type_code');
	    	$this->select->addExpression('acc.tagname','tagname');
	    	}
	    	
	    	$this->select->groupBy('acc.cls_id');
	    	$this->select->groupBy('acc.object_type');
	    	

	    	if(isset($_REQUEST['mobile_request'])){
	    		$this->select->addField('acc','additional_info');
	    		$this->select->addField('acc','addn_catalog_show');
	    	}
	    	
	    }
	    
	    $this->searchSortBy($this->setParamVal('sortby'));
	    //#0062052: In catalog pagination, while entering the page number which is more than the actual page.It is navigating to last page as empty.
	  /*  $cntResults=$this->select->countQuery()->execute()->fetchField();	    
	    $this->qry_search_count = $cntResults;	     
	    if ( $this->qry_search_count > 0 ) {
	    	$this->total_pages = ceil($this->qry_search_count/$this->limit);
	    } else {
	    	$this->total_pages = 0;
	    }
	    if($_GET['apiname'] != 'CatalogSearchAPI'){
	    	if ($this->page > $this->total_pages) $this->page=$this->total_pages;
	    }*/
	    $this->start = $this->limit*$this->page - $this->limit;
	    if ($this->start < 0) {
	    	$this->start = 0;
	    }
	    $this->select->range($this->start, $this->limit);
	    if($callFrom == 'API') {
	    $selectInd = $this->select->extend("ExpertusSelectQueryExtender");
			//$selectInd->setIndex(array('cls'=>'clssts','prm'=>'tpsts'));
	    $catQryRes = $selectInd->catalogExecute()->fetchAll();
				expDebug::dPrintDBAPI('CATALOG SEARCH QUERY with ExpertusSelectQueryExtender--->', $selectInd);
	    expDebug::dPrint("Executed string of query ".print_r($catQryRes,true),4);
	    $totRecQry  = db_query('select found_rows() as tot_records');
	    $count  = $totRecQry->fetchField();
	    expDebug::dPrint("Result of count from catalog".$count,3);
	    $this->qry_search_count = 0;
	    $this->qry_search_count = $count;
	    
	    if ( $this->qry_search_count > 0 ) {
	    	$this->total_pages = ceil($this->qry_search_count/$this->limit);
	    } else {
	    	$this->total_pages = 0;
	    }
	    if($_GET['apiname'] != 'CatalogSearchAPI'){
					if($this->page > $this->total_pages)
						$this->page = $this->total_pages;
				}
			} else {
				//expDebug::dPrintDBAPI('CATALOG SEARCH QUERY without ExpertusSelectQueryExtender--->', $this->select);
				expDebug::dPrint("Executed string of query " . print_r($catQryRes, true), 4);
				$catQryRes = $this->select->execute()->fetchAll();
				expDebug::dPrintDBAPI('$this->select ', $this->select);/*Viswanathan added for #82096*/
	    }
	     
	    return $catQryRes;
    }catch (Exception $ex) {
          watchdog_exception('getSearchResults', $ex);
          expertusErrorThrow($ex);
    }
  }

  /**
   * Funtion to return the Search Results.
   *
   * @param string ''
   *
   * @return
   *   Results of Search Data
   */
/*  function getSearchList(){
   // $this->select->range($this->start, $this->limit);
    expDebug::dPrint("CATALOG SEARCH QUERY : ".$this->select->__toString());
	//print $this->select;

    $countSelect = $this->select;
    $catalogResult = $countSelect->execute()->fetchCol();

	$catalogResult = $this->select->execute()->fetchAll();
    return $catalogResult;
  }*/

  /**
   * Funtion to set the Filter Critria
   *
   * @param string ''
   *
   * @return
   * Set Filter Values
   */

/*  function setSearchFilter(){

    $full_Select = $this->select;
    $this->full_search = $full_Select->execute()->fetchAll();
    $delivery_type_name=array();
    $language=array();
    $country_name=array();
    $mro_flags = array();
    foreach($this->full_search as $key => $val){
      $delivery_type_name[$val->delivery_type_code] = $val->delivery_type_name;
      $language[$val->language_code] = $val->language;
      if($val->country_code !=''){
      $country_name[$val->country_code] = $val->country_name;
      }
    	if($val->mro_id !=''){
      $mro_flags[$val->mro_id] = $val->mro;
      }
    }

    $this->delivery_type_name_distinct = array_unique($delivery_type_name);
    $this->language_distinct = array_unique($language);
    $this->country_name_distinct = array_unique($country_name);
    $this->mro_distinct = array_unique($mro_flags);
  }*/



  function searchSortBy($sortby) {
    try{
    if ($sortby == "AZ") {
      $xorderby = "cls_title";
      $xorderbylist = "asc";
    } else if ($sortby == "ZA") {
      $xorderby = "cls_title";
      $xorderbylist="desc";
    } else if ($sortby == "Time") {
      $xorderby="published_on";
      $xorderbylist="desc";
    } else if ($sortby == "ClassStartDate") {
      $xorderby="ifnull(start_date,'4000-12-30') , start_date";
      $xorderbylist="asc";
    } else if ($sortby == "Mandatory") {
      $xorderby="is_compliance";
      $xorderbylist="desc";
    }/*Viswanathan added for 73586*/
    elseif($sortby == 'dateOld'){
    	$xorderby="created_on";
    	$xorderbylist="asc";
    }
    elseif($sortby == 'dateNew'){
    	$xorderby="created_on";
    	$xorderbylist="desc";
    }
    elseif($sortby == 'type'){
      $xorderby="delivery_type_code,delivery_type_name";
      $xorderbylist="asc";
    }else {
      $xorderby="cls_title";
      $xorderbylist="asc";
    }

    $this->select->orderBy($xorderby,$xorderbylist);
    }catch (Exception $ex) {
    watchdog_exception('searchSortBy', $ex);
    expertusErrorThrow($ex);
    }
  }
  //Embed widget related work ( search based on entity type and entity id)
  function searchByEntity(){
  	try{
  		$ent_type = $this->setParamVal('ent_type');
  		$ent_id = $this->setParamVal('ent_id');
  		if($ent_type =='cre_sys_obt_cls')
  			$this->select->condition('cls.id',$ent_id,'=');
  		elseif($ent_type =='cre_sys_obt_crs')
  			$this->select->condition('crs.id',$ent_id,'=');
  		elseif($ent_type =='cre_sys_obt_trp')
  			$this->select->condition('prm.id',$ent_id,'=');
  	}
  	catch (Exception $ex) {
  		watchdog_exception('searchByEntity', $ex);
  		expertusErrorThrow($ex);
  	}
  }

} //-------------------------Class LnrSearchCatalog End--------------------------


/**
 * Funtion to return the list of criteria like Delivery Type etc.
 *
 * @param
 *
 * @return
 *   Results of Delivery types
 */
function getDeliverTypes(){
  try{
  global $catalog_reg;

  $select = db_select('slt_profile_list_items','splt');
  $select->addField('splt','id', 'id');
  $select->addField('splt','name', 'c_name');
  $select->addField('splt','code', 'c_code');
  if($catalog_reg == 'Course') {
  	$select->condition('splt.code', 'lrn_crs', '=');
  }else{
  	$select->condition('splt.code', db_like('lrn_cls_dty_') . '%', 'LIKE');
  }
  $select->condition(db_and()->condition('splt.lang_code','cre_sys_lng_eng','='));
  $select->condition(db_and()->condition('splt.is_active','Y','='));
  expDebug::dPrintDBAPI('narrow search sql query-->>',$select);
  $result = $select->execute()->fetchAll();
  return $result;
  }catch (Exception $ex) {
    watchdog_exception('getDeliverTypes', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Funtion to return the list of Learning Objects criteria like Cert,curricula and learning plan etc.
 *
 * @param
 *
 * @return
 *   Results of Learning Object Types
 */
function getLearningObjectType(){
  try{
  $select = db_select('slt_profile_list_items','splt');
  $select->addField('splt','id', 'id');
  $select->addField('splt','name', 'name');
  $select->addField('splt','code', 'code');

  $select->condition(db_and()->
      condition(db_or()->
      condition('splt.code', 'cre_sys_obt_crt' , '=')->
      condition('splt.code', 'cre_sys_obt_cur' , '=')->
      condition('splt.code', 'cre_sys_obt_trn' , '=')));

  $select->condition(db_and()->condition('splt.lang_code','cre_sys_lng_eng','='));

  $select->condition(db_and()->condition('splt.is_active','Y','='));

  $result = $select->execute()->fetchAll();
  return $result;
  }catch (Exception $ex) {
    watchdog_exception('getLearningObjectType', $ex);
    expertusErrorThrow($ex);
  }
}
/**
 * Funtion to return the list of criteria like MRO etc.
 *
 * @param
 *
 * @return
 *   Results of MRO
 */
/*function getTrainingTypes($mro_set){
	$mrotype = explode("|",$mro_set);
	$in_set = '';
	foreach($mrotype as $key => $val){
		$in_set .= "'".$val."',";
	}
	$infilter = substr($in_set,0,-1);
	$select = db_query('SELECT code,name FROM slt_profile_list_items WHERE code IN ('.$infilter.') AND is_active = \'Y\' ORDER BY FIELD(CODE,\'cre_sys_inv_man\',\'cre_sys_inv_rec\',\'cre_sys_inv_opt\')');
  $result = $select->fetchAll();
  $mro_types = array();
  foreach($result as $key => $val){
  	if($val->code != ''){
    	$mro_types[$val->code] = $val->name;
  	}
  }
  return $mro_types;
}*/


/**
 * Funtion to return the list of criteria like MRO etc.
 *
 * @return
 *   Results of MRO
 */
function getStaticTrainingTypes($items){
  try{
  	$in_set = '';
  	foreach($items as $key => $val){
  		$in_set .= "'".$val."',";
  	}
  	$infilter = substr($in_set,0,-1);
  	$select = db_query('SELECT code,name FROM slt_profile_list_items WHERE code IN ('.$infilter.') AND is_active = \'Y\' ORDER BY FIELD(CODE,'.$infilter.')');
  	$result = $select->fetchAll();
  	expDebug::dPrintDBAPI(' getStaticTrainingTypes ram $select ', $select);
  	return $result;
  }catch (Exception $ex) {
    watchdog_exception('getStaticTrainingTypes', $ex);
    expertusErrorThrow($ex);
  }
}

function getTrainingCompliance(){
	try{
		$select = db_query('select count(is_compliance) from slt_course_template where is_compliance=1;');
		$result = $select->fetchField();
		expDebug::dPrintDBAPI('$select ramiscomplainces', $select);
		return ($result >= 1) ? 'cre_sys_inv_com' : '';
	}catch (Exception $ex) {
		watchdog_exception('getTrainingCompliance', $ex);
		expertusErrorThrow($ex);
	}
}

function getMandOrRecTrainningCount($flag){
	try{
		$selectStmt = db_select('slt_group_mapping', 'mromap');
		$selectStmt->condition('mromap.mro', $flag);
		$selectStmt->addExpression('COUNT(mromap.id)', 'mromapcount');
		$result = $selectStmt->execute()->fetchField();
		expDebug::dPrintDBAPI(' getMandOrRecTrainningCount ram ', $selectStmt);
		return ($result >= 1) ? $flag : '';
	}catch (Exception $ex) {
		watchdog_exception('getMandOrRecTrainningCount', $ex);
		expertusErrorThrow($ex);
	}
}


/**
 * Funtion to return the Minimum & Maximum Price.
 *
 * @param
 *
 * @return
 *   Results of Min,Max Prices
 */
/*function getPriceRangeDetails(){

    $select = srchCatalogQuery();

    $select->addExpression('MIN(ifnull(cl.price,0))', 'cls_minprice');
    $select->addExpression('MAX(ifnull(cl.price,0))', 'cls_maxprice');
    $select->addExpression('MIN(ifnull(prm.price,0))', 'prm_minprice');
    $select->addExpression('MAX(ifnull(prm.price,0))', 'prm_maxprice');

	if(!empty($_SESSION['availableFunctionalities']->exp_sp_learning_plan_detail)) {
    	$select->condition('ms.type', array('Class','cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn'), 'IN');
    }else{
    	$select->condition('ms.type', 'Class', '=');
    }

    $select->where('IF(ms.type=\'Class\',
    ( splt_deliverytype.lang_code=\'cre_sys_lng_eng\'
    and splt_status.lang_code=\'cre_sys_lng_eng\'
    and splt_language.lang_code=\'cre_sys_lng_eng\'
    and nl.entity_type=\'cre_sys_obt_cls\'
    and ms.status in (\'lrn_cls_sts_atv\',\'lrn_crs_sts_atv\')  and crs.status = \'lrn_crs_sts_atv\'),1=1 )'
    );

     $select->where('IF(ms.type!=\'Class\',
    (
    ms.status in (\'lrn_lpn_sts_atv\') AND map.program_id=ms.pk_id ),1=1 )' //AND map.program_id=ms.pk_id
    );

    //addFilterForLoggedInUserRole($select, 'sepm');
    filterByUserAccessPrivileges($select, 'catacs');

    $startdate = Date("Y-m-d");
    $time = Date("H:i:s");
    $toDay = Date("Y-m-d");

    $select->where('IF(ms.type=\'Class\',
      (
      ( ms.delivery_type=\'lrn_cls_dty_ilt\' and cs.start_date >= \''.$startdate.'\' and
      	IF(cs.start_date =\''.$toDay.'\', cs.start_time >= \''.$time.'\',1=1) )
      or
      ( ms.delivery_type=\'lrn_cls_dty_vcl\' and cs.start_date >= \''.$startdate.'\' and
      	IF(cs.start_date =\''.$toDay.'\', cs.start_time >= \''.$time.'\',1=1)  )
      or
      ( ms.delivery_type=\'lrn_cls_dty_wbt\' and content.content_id IS NOT NULL )
      )
      or
      ( ms.delivery_type=\'lrn_cls_dty_vod\' and content.content_id IS NOT NULL ),
      1=1)');

   $result = $select->execute()->fetchAssoc();

  // print $select;
   return $result;


}  */

/**
 * Funtion to return the Minimum & Maximum Price.
 *
 * @return
 *   Results of Min,Max Prices
 */
function getStaticPriceRangeDetails(){
  try{
  $query = "select group_concat(pri.minprice) as minprice,group_concat(pri.maxprice) as maxpri from ";
  $query .= "(select min(cls.price) minprice,max(cls.price) maxprice from slt_course_class cls where cls.status='lrn_cls_sts_atv' ";
  $query .= " union all select min(prg.price) minprice,max(prg.price) maxprice from slt_program prg where prg.status='lrn_lpn_sts_atv') pri ";
  $select = db_query($query);
  $result = $select->fetchAssoc();
  return $result;
  }catch (Exception $ex) {
    watchdog_exception('getStaticPriceRangeDetails', $ex);
    expertusErrorThrow($ex);
  }
}


/**
 * Function to return the list of Languages .
 *
 * @param1 - List of language code
 *
 * @return
 *   Results of Language types
 */
/*function getAllLanguageTypes($lg_type){
    $languagetype = explode("|",$lg_type);
    $select = db_select('slt_profile_list_items','slpt_li');
    $select->addField('slpt_li','name', 'language');
    $select->addField('slpt_li','code', 'language_code');
    $select->condition(db_and()->condition('slpt_li.code',$languagetype,'IN'));
    $result = $select->execute()->fetchAll();
    $language = array();
    foreach($result as $key => $val){
      $language[$val->language_code] = $val->language;
    }
    return $language;
}*/

/**
 * Function to return the list of Languages .
 *
 * @return
 *   Results of Language types
 */
function getAllStaticLanguageTypes(){

  // fetch language from slt_course_class and program
/*    $query = "select distinct lang.language language, lang.language_code language_code from (
              select slpt_li.name as language, slpt_li.code as language_code
              from slt_course_class cls
              left outer join slt_profile_list_items slpt_li on slpt_li.code=cls.lang_code
              where slpt_li.is_active = 'Y'
              group by cls.lang_code
              union all
              select slpt_li.name as language, slpt_li.code as language_code
              from slt_program prm
              left outer join slt_profile_list_items slpt_li on slpt_li.code=prm.lang_code
              where slpt_li.is_active = 'Y'
              group by prm.lang_code) lang;";
	$select = db_query($query);

	$result = $select->fetchAll();*/

  //fetch language from List Values
  try{
  $select = db_select('slt_profile_list_items','splt');
  $select->addField('splt','id', 'id');
  $select->addField('splt','name', 'language');
  $select->addField('splt','code', 'language_code');
  $select->condition('splt.code', db_like('cre_sys_lng_') . '%', 'LIKE');
  $select->condition(db_and()->condition('splt.lang_code','cre_sys_lng_eng','='));
  $select->condition(db_and()->condition('splt.is_active','Y','='));
  $select->orderBy('language','asc');
  expDebug::dPrintDBAPI('$select to fetch languages', $select);
  $result = $select->execute()->fetchAll();
  return $result;
  }catch (Exception $ex) {
    watchdog_exception('getAllStaticLanguageTypes', $ex);
    expertusErrorThrow($ex);
  }

}


/**
 * Function to return the list of countries .
 *
 * @param1 - List of country code
 *
 * @return
 *   Results of Countries
 */
/*function getAllCountry($cy_type){
    $countrytype = explode("|",$cy_type);
    $select = db_select('slt_country','sc');
    $select->addField('sc','country_name');
    $select->addField('sc','country_code');
    $select->condition(db_and()->condition('sc.country_code',$countrytype,'IN'));
    $result = $select->execute()->fetchAll();
    $country = array();
    foreach($result as $key => $val){
      $country[$val->country_code] = $val->country_name;
    }
    return $country;
}*/


    /**
 * Function to return the list of countries .
 *
 *
 * @return
 *   Results of Countries
 */
function getAllStaticCountry(){
  try{
    $select  = db_select('slt_location','loc');
    $select->innerJoin('slt_country', 'country', 'country.country_code = loc.country');
    $select->addField('country','country_code');
    $select->addField('country','country_name');
    $select->distinct('loc.country');
    $select->condition('loc.status', 'lrn_res_loc_atv', '=');
    $select->orderBy('country_name','asc');
    $result = $select->execute()->fetchAll();
    expDebug::dPrintDBAPI("getAllStaticCountry = ",$select);
    return $result;
  }catch (Exception $ex) {
    watchdog_exception('getAllStaticCountry', $ex);
    expertusErrorThrow($ex);
  }

}

function getLocationNameDts($locationId) {
   try{
  // Select table
    $select = db_select('slt_location', 'loc');

    // Select fields to fetch
    $select->addField('loc', 'name', 'location_name');

    // Set the condition(s)
    $select->condition('loc.id', $locationId);

    // Execute query and fetch the single values
    $locationName = $select->execute()->fetchField();

    return $locationName;
  }catch (Exception $ex) {
    watchdog_exception('getLocationNameDts', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Function get the node id
 *
 * @return nodeId
 */
function getNodeIdFromEntity($entity_id,$entity_type){
	try{
		$select = db_select('slt_node_learning_activity', 'nla');
		$select->addField('nla',	'node_id', 'node_id');
		$select->condition('nla.entity_id',	 $entity_id);
		$select->condition('nla.entity_type', $entity_type);
		$select->range(0, 1);
		$nodeId  = $select->execute()->fetchField();
		return $nodeId;
	}catch (Exception $ex) {
		watchdog_exception('getStarRatingInfo', $ex);
		expertusErrorThrow($ex);
	}
}
/**
 * Function get the class ids with comma separator
 *
 * @return avgvote
 */
function getclasses($courseID){
	try{
		$selectQuery = db_select('slt_course_template','crs');
		$selectQuery->leftJoin('slt_course_class', 'cls', 'cls.course_id=crs.id');

		$selectQuery->addExpression('GROUP_CONCAT(DISTINCT(cls.id))', 'cls_id');
		$selectQuery->addExpression('count(cls.id)', 'cls_count');
		$selectQuery->condition('crs.id', $courseID, '=');
		$selectQuery->condition('cls.status', 'lrn_cls_sts_atv', '=');
		expDebug::dPrintDBAPI(' Vote sqlssss = ' , $selectQuery);
		$resultset  = $selectQuery->execute()->fetchAll();
		return $resultset;
	}
	catch (Exception $ex) {
		watchdog_exception('getclasses', $ex);
		expertusErrorThrow($ex);
	}
}


function getCourseRating($classIDs){
	try{
		$classIds = explode(',',$classIDs);
		$select = db_select('votingapi_cache', 'vote');
		$select->leftJoin('slt_node_learning_activity', 'snla', 'snla.node_id = vote.entity_id');
		$select->addField('vote',	'value', 'vote_averag');
		$select->condition('snla.entity_id', $classIds, 'IN');
		$select->condition('vote.function', 'average', '=');
		$select->condition('vote.entity_type','Class', '=');
		expDebug::dPrintDBAPI(' Vote sql = ' , $select);
		$result  = $select->execute()->fetchAll();
		$vote_sum = 0;
		$course_vote = '';
		foreach ($result as $vote){
			$vote_sum = $vote_sum + $vote->vote_averag;
		}
		if($vote_sum>0){
			$course_vote = $vote_sum/count($result);
		}
		expDebug::dPrint('COURSE VOTES = TOTLAL'.print_r($course_vote,true),4);
		expDebug::dPrint('COURSE VOTES = '.print_r($result,true),4);
		return $course_vote;
	}catch (Exception $ex) {
		watchdog_exception('getCourseRating', $ex);
		expertusErrorThrow($ex);
	}
}
/**
 * Function to return the list of catalog.
 *
 * @return
 *   Results of Catalogs
 */
function getCatalogSearchWrapperAPI($paramsobj,$user_id){
  try{
  	global $catalog_reg;
  $paramsobj=array_to_object($paramsobj);

  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc');
  include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_prerequisite/exp_sp_prerequisite.inc');
  include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrenrollment/exp_sp_launch.inc');
  include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules//core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrenrollment/exp_sp_lnrenrollment.inc');

  	//Below function will set currency details in $_REQUEST
		$currencyDetails = splitCurrencyInputAndSetInRequest();
                $updUserInfo = getPersonDetails($user_id,array('preferred_currency'));
                $updUserCurrency = $updUserInfo['preferred_currency'];
                $isCommerceEnabled =  module_exists('exp_sp_commerce');
                $currecyChangeRestrict = restrict_user_currency_select();
                $_REQUEST['currency_type'] = (empty($_REQUEST['currency_type']) || $_REQUEST['currency_type']=='') ? $updUserCurrency : $_REQUEST['currency_type'];
		if(empty($currencyDetails))
		{
                    $errobj=new stdClass();
                    $errobj->isValidateError = 1;
                    $errobj->errcode = 'L_012';
                    $errobj->field = 'currency_type';
                    $errobj->errormsg = "The given Currency type is invalid or inactive status";
                    return $errobj;
		}
		$api_object_type = strtolower($paramsobj->object_type);
		
		if(($_REQUEST['apiname'] == "CatalogSearchAPI")&&($paramsobj->object_type != '') && ($api_object_type != 'course' && $api_object_type != 'class')){
			expDebug::dPrint('object_type testtt = ' .$_REQUEST['object_type']);
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_005';
			$errobj->field = 'object_type';
			$errobj->errormsg = "object type value is invalid";
			return $errobj;
		}
                if($isCommerceEnabled && $currecyChangeRestrict == TRUE){	
                if(!empty($updUserCurrency) && ($updUserCurrency != $_REQUEST['currency_type'])){
                        $errobj=new stdClass();
                        $errobj->isValidateError = 1;
                        $errobj->errcode = 'L_012';
                        $errobj->field = 'currency_type';
                        $errobj->errormsg = "Currency code should be same as user's profile currency.";
                        return $errobj;
                        }
                }
		
  expDebug::dPrint('currency_type = ' .$_REQUEST['currency_type']);
  expDebug::dPrint('currency_sym  = ' .$_REQUEST['currency_sym']);
  
  if ($_REQUEST['currency_type'] && $_REQUEST['currency_sym']) {
  	$_SESSION['shop_cart_currency_code'] = $_REQUEST['currency_type'];
  	$_SESSION['user_preferred_currency_code'] = $_REQUEST['currency_type'];
  	$_SESSION['user_preferred_currency_sym'] = $_REQUEST['currency_sym'];
  } else if ($_REQUEST['currency_type']) {
  	$getDefaultCurrency = getDefaultCurrency();
  	$UserPrefCurrCode = $getDefaultCurrency['preferred_currency'];
  	$UserPrefCurrSym = $getDefaultCurrency['attr2'];
  		
  	$_SESSION['shop_cart_currency_code'] = $UserPrefCurrCode;
  	$_SESSION['user_preferred_currency_code'] = $UserPrefCurrCode;
  	$_SESSION['user_preferred_currency_sym'] = $UserPrefCurrSym;
  } else {
  	$getDefaultCurrency = getDefaultCurrency();
  	$UserPrefCurrCode = $getDefaultCurrency['preferred_currency'];
  	$UserPrefCurrSym = $getDefaultCurrency['attr2'];
  
  	$_SESSION['shop_cart_currency_code'] = $UserPrefCurrCode;
  	$_SESSION['user_preferred_currency_code'] = $UserPrefCurrCode;
  	$_SESSION['user_preferred_currency_sym'] = $UserPrefCurrSym;
  }
  
  $_GET['page']=$paramsobj->page;
  $_GET['rows']=$paramsobj->limit;
  $_REQUEST["dl_type"]=str_replace(",","|",$_REQUEST["dl_type"]);
  if($_REQUEST['apiname'] == "CatalogSearchAPI" && !empty($_REQUEST['currency_code']) && strpos($_REQUEST['currency_code'],'cre_') !== false)
    {
  	$selectQuery = db_select('slt_profile_list_items', 'spli');
  	$selectQuery->addField('spli','attr1');
  	$selectQuery->condition('spli.is_active', 'Y', '=');
  	$selectQuery->condition('spli.attr3', 'Y', '=');
  	$selectQuery->condition('spli.code', $_REQUEST['currency_code'], '=');

  	$currency_type = $selectQuery->execute()->fetchField();
  	if(empty($currency_type))
  	{
  		$errobj=new stdClass();
  		$errobj->isValidateError = 1;
  		$errobj->errcode = 'L_012';
  		$errobj->field = 'currency_type';
  		$errobj->errormsg = "The given Currency type is invalid or inactive status";
  		return $errobj;
  	}
  }
  if($paramsobj->action == 'autoregister')
  {

  	$selectStmt = db_select('slt_person', 'per');
  	$selectStmt->condition('per.id', $_GET["user_id"]);
  	$selectStmt->addField('per', 'user_name');
  	$result = $selectStmt->execute()->fetchField();
  	expDebug::dPrint('username= ' . print_r($result,true), 4);
  	exp_mobile_login_success($_REQUEST["user_id"],$result);
  }

  if($paramsobj->action == 'team')
  {
  	$rtnType = $_REQUEST['returntype'];
		$_REQUEST['returntype'] = 'json';
  	$result = getMyteamCatalogResults($paramsobj->user_id);
		$_REQUEST['returntype'] = $rtnType;
		unset($_REQUEST['SolrImpl']); // TODO: Instead of unset this, implement below functionalities in solr srarch
  	return $result;
  }
  else {
  if(!empty($_GET["price_start"]) && !empty($_GET["price_end"])){
    $_GET["price"]= "".$_GET["price_start"]."-".$_GET["price_end"]; // 0056924 - default $ removal
  }
  
  // SOLR check starts
  $srcSource = getConfigValue('solr_search');
	if($srcSource == 0){
  $catalog_reg = $paramsobj->object_type;
  $lnrSrch = new LnrSearchCatalog();

  $results = $lnrSrch->getSearchResults('API');
  expDebug::dPrint('enrollment id = ' . print_r($results,true), 4);
  if($lnrSrch->qry_search_count && count($results)){
    $results['totalrow'] = $lnrSrch->qry_search_count;
  }
	}else{
		require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_catalog.php';
		try{
			expDebug::dPrint("Solr Search Testing",1);
			$rtnType = $_REQUEST['returntype'];
			$_REQUEST['returntype'] = 'json';
			$solrObj = new CatalogSolrSearch();
			$search_results = $solrObj->getSolrSearchResult();
			$results = $search_results['records'];
			expDebug::dPrint('Result set from solr'.print_r($results,true),5);
			$results['totalrow'] = $search_results['recCount'];
			$_REQUEST['returntype'] = $rtnType;
			unset($_REQUEST['SolrImpl']); // TODO: Instead of unset this, implement below functionalities in solr srarch
		}catch(Exception $e){
			expDebug::dPrint("Error in Solr Search -- ".print_r($e->getMessage(),true),1);
		}
	}

  foreach($results as $resClsDetials) {
  	if($catalog_reg =='Course'){
  		$classId     		= $resClsDetials->crs_id;
  		$courseId    		= $resClsDetials->cls_id;
  		if($srcSource == 1){
  			$courseId = (array)$courseId;
  			$courseId = $courseId[0];
  			$classId = (array)$classId;
  			$classId = $classId[0];
  		}
  	}else{
  		$classId     		= $resClsDetials->cls_id;
  		$courseId    		= $resClsDetials->crs_id;
  	}
    $object_type 		= $resClsDetials->object_type;
    $object_type_code	= ($object_type =='Class'||$object_type =='Course')? 'cre_sys_obt_cls' : $object_type;
    //  Ticket #53811: List of Catalog API-Avg vote column doesn't fetch any value
    if($object_type=='Course'){
    	$resClsDetials->avgvote = getCourseRating($classId);
    }

    //$getNodeId   = getNodeIdFromEntity($classId,$object_type_code);
    expDebug::dPrint('registration_end_on Date : '.print_r($resClsDetials->registration_end_on,true),4);
    //  Ticket #0033039 For

    $createDate = new DateTime($resClsDetials->registration_end_on);
    $registration_end_on = $createDate->format('Y-m-d');
    expDebug::dPrint('The registration_end_on Date strtotime For api = '.print_r($registration_end_on,true),4);

    $presentDate=date('Y-m-d');
    expDebug::dPrint('PresentTime Date strtotime For api  = '.print_r(strtotime($presentDate),true),4);

    if(strtotime($registration_end_on) >= strtotime($presentDate) || empty($createDate)){
      $resClsDetials->registration_ended= "false";
    }
    else{
      $resClsDetials->registration_ended= "true";
    }

    //$resClsDetials->waitlist_status = getWaitlistCatalogInfo($classId);
    //$resClsDetials->sessioninfo = getSessionDetails($courseId,$classId,$resClsDetials->delivery_type_code,"enroll");
    if($object_type !='Course' && $object_type !='Class' ){
    	if($catalog_reg =='Course'){
    		$resClsDetials->prerequisites = getCatalogTpPrerequisite($courseId,$paramsobj->user_id);
    	}else{
    		$resClsDetials->prerequisites = getCatalogTpPrerequisite($classId,$paramsobj->user_id);
    	}
    	
    }else{
    $resClsDetials->prerequisites = getCatalogPrerequisite($courseId,$paramsobj->user_id);
    }
    
    
    //$resClsDetials->available_seats = getAvailableSeats($resClsDetials->cls_id);
    //$resClsDetials->waitlist_seats  = $resClsDetials->waitlist_status;
   // $resClsDetials->rating = getEntityRatingInfo($getNodeId,$object_type);

    if(($catalog_reg =='Course' && $object_type !='Course') || ($catalog_reg !='Course') || ($object_type =='Course' && $resClsDetials->cls_count == 1)){
    	if(($resClsDetials->delivery_type_code == "lrn_cls_dty_ilt" || $resClsDetials->delivery_type_code == "lrn_cls_dty_vcl")) {
    		$resClsDetials->sessioninfo = getSessionDetails($courseId,$classId,$resClsDetials->delivery_type_code);
    		$resClsDetials->available_seats = getAvailableSeats($classId);
    		$resClsDetials->waitlist_status  = getWaitlistCatalogInfo($classId);
    		$resClsDetials->waitlist_seats  = $resClsDetials->waitlist_status;
    		$pastDateCount                 = checkPastDateOrNot($classId,$resClsDetials->delivery_type_code);
    	}else{
    		$resClsDetials->sessioninfo = array();
    		$resClsDetials->available_seats = '';
    		$resClsDetials->waitlist_status  = '';
    		$resClsDetials->waitlist_seats  = '';
    		$pastDateCount                 = 0;
    	}
        if($catalog_reg =='Course' && $object_type_code!='cre_sys_obt_cls'){
           $getNodeId   = getNodeIdFromEntity($courseId,$object_type_code);
           $value = getEntityRatingInfo($getNodeId,$object_type);   
    	}
    	elseif($catalog_reg =='Course'){
    		$classIDs= getclasses($courseId);
    		$value = getCourseRating($classIDs[0]->cls_id);
    	}
    	   else {
    	$getNodeId   = getNodeIdFromEntity($classId,$object_type_code);
    	     $value = getEntityRatingInfo($getNodeId,$object_type);
        }
        $ratingVal = ''; 
        if($value >= 81 && $value <= 100){
        	$ratingVal = 100;
        }elseif($value >= 61 && $value <= 80){
        	$ratingVal = 80;
        }elseif($value >= 41 && $value <= 60){
        	$ratingVal = 60;
        }elseif($value >= 21 && $value <= 40){
        	$ratingVal = 40;
        }elseif($value >= 1 && $value <= 20){
        	$ratingVal = 20;
        }
        $resClsDetials->rating = $ratingVal;
        $resClsDetials->avgvote = $ratingVal;
    }
    $widgetId 			= (($catalog_reg =='Course')) ? 'catalog-search' : '';

    /* for getting TP registered status*/
    if($object_type !='Course' && $object_type !='Class' ){


     if($catalog_reg =='Course'){
        $resClsDetials->enrolled_id = getObjectRegisteredOrNot($courseId );
		if($resClsDetials->object_type == "cre_sys_obt_crt"){
		  $resClsDetials->enrolled_id->module_id = getModuleId($paramsobj->user_id,$courseId );
		  $nxt_mdl = getModuleIdFromObjectId($courseId ,($resClsDetials->enrolled_id->module_id[0]->recertify_path)+1);
		  $resClsDetials->enrolled_id->recertify_before_certify_expired = ($resClsDetials->enrolled_id->module_id[0]->comp_date  && $nxt_mdl > 0 ) ? ($resClsDetials->enrolled_id->module_id[0]->object_type == 'cre_sys_obt_crt') ? getRecertifyBeforeCertifyExpired($resClsDetials->enrolled_id->module_id[0]->comp_date,$resClsDetials->enrolled_id->module_id[0]->expires_in_value,$resClsDetials->enrolled_id->module_id[0]->expires_in_unit) : 0 : 0;
		 }
		
		
    	$enrollId = $resClsDetials->enrolled_id->master_enrolled_id;
    	if(isset($_REQUEST['mobile_version'])){
     	$tp_courses = lp_fetch_module_list($courseId,$user_id,'N','',1);
	 	$classIdArr = attachedCourseClassDetailsAPI($courseId);
    	}
	 	$availableSeats = getObjectAvailableSeats($courseId);
	 	$waitlist_status = getProgramWaitlistStatus($courseId,$paramsobj->user_id);
	 	$resClsDetials->available_seats = $availableSeats;
	 	$resClsDetials->waitlist_status  = $waitlist_status;
	 	$resClsDetials->waitlist_seats  = $waitlist_status;
	 	 
     }
     else{
          $resClsDetials->enrolled_id = getObjectRegisteredOrNot($classId );
		  if($resClsDetials->object_type == "cre_sys_obt_crt"){
		  $resClsDetials->enrolled_id->module_id = getModuleId($paramsobj->user_id,$classId );
		  $nxt_mdl = getModuleIdFromObjectId($classId ,($resClsDetials->enrolled_id->module_id[0]->recertify_path)+1);
		  $resClsDetials->enrolled_id->recertify_before_certify_expired = ($resClsDetials->enrolled_id->module_id[0]->comp_date  && $nxt_mdl > 0 ) ? ($resClsDetials->enrolled_id->module_id[0]->object_type == 'cre_sys_obt_crt') ? getRecertifyBeforeCertifyExpired($resClsDetials->enrolled_id->module_id[0]->comp_date,$resClsDetials->enrolled_id->module_id[0]->expires_in_value,$resClsDetials->enrolled_id->module_id[0]->expires_in_unit) : 0 : 0;
		  }
          $enrollId = $resClsDetials->enrolled_id->master_enrolled_id;
          if(isset($_REQUEST['mobile_version'])){
      	  $tp_courses = lp_fetch_module_list($classId,$user_id,'N','',1);
         $classIdArr = attachedCourseClassDetailsAPI($classId);
          }
         $availableSeats = getObjectAvailableSeats($classId);
         $waitlist_status = getProgramWaitlistStatus($classId,$paramsobj->user_id);
         $resClsDetials->available_seats = $availableSeats;
         $resClsDetials->waitlist_status  = $waitlist_status;
         $resClsDetials->waitlist_seats  = $waitlist_status;
          

     }
     $resClsDetials->tp_courses = $tp_courses;
     $resClsDetials->class_list = $classIdArr;

    }
    else{

    $resClsDetials->enrolled_id = getRegisteredOrNot($courseId,$classId,$paramsobj->user_id,$widgetId);

     $enrollId = $resClsDetials->enrolled_id->enrolled_id;
     $resClsDetials->launchdetails = getLaunchDetail( $enrollId);
    }
    /* if(($catalog_reg =='Course' && $object_type =='Course') && $resClsDetials->cls_count == 1){
    	$resClsDetials->enrolled_id = getClassEnrollmentIdForCourseLevel($courseId,$classId);
    	$enrollId = $resClsDetials->enrolled_id;
    }else{ */
    expDebug::dPrint('print the enrolled details from api'.print_r($resClsDetials->enrolled_id,true));

    //}
    $class_status = '';
    $resClsDetials->multi_register = is_multiregistration_allowed($classId);
    $registerEndDateCheck = 0;

    $startdate = Date("Y-m-d");
    if($resClsDetials->registration_end_on !='' || $resClsDetials->registration_end_on != null) {
      if (strtotime($resClsDetials->registration_end_on) < strtotime($startdate)) {
        $registerEndDateCheck = 1;
      }
    }
    //$pastDateCount  = checkPastDateOrNot($classId,$resClsDetials->delivery_type_code);
    if(($enrollId != '' || $enrollId != null || $enrollId != 0) && $resClsDetials->multi_register == 0 ) {
	  $class_status = ((($resClsDetials->enrolled_id->comp_status == 'lrn_crs_cmp_cmp')||( $resClsDetials->enrolled_id->master_enrolled_status == 'lrn_tpm_ovr_cmp' ) ) ? t('Completed') : ((($resClsDetials->enrolled_id->enrolled_status == 'lrn_crs_reg_wtl') || ($resClsDetials->enrolled_id->waitlist_flag == 'lrn_crs_reg_wtl') || ($resClsDetials->enrolled_id->master_enrolled_status == 'lrn_tpm_ovr_wtl')) ? t('LBL190') : t('Registered')));
    } else if(($resClsDetials->delivery_type_code == "lrn_cls_dty_ilt" || $resClsDetials->delivery_type_code == "lrn_cls_dty_vcl") && $resClsDetials->available_seats == 0 && $resClsDetials->waitlist_seats == 0) {
      $class_status = t('LBL046');
    } else if(($resClsDetials->delivery_type_code == "lrn_cls_dty_ilt" || $resClsDetials->delivery_type_code == "lrn_cls_dty_vcl") && $pastDateCount > 0) {
      $class_status = t('LBL105');
    } else if($registerEndDateCheck <> 0) {
      $class_status = t('LBL047');
    }
    $resClsDetials->class_status  = $class_status;

    expDebug::dPrint('enrollment id = tp_courses '.print_r($resClsDetials->tp_courses), 4);
    if($object_type !='Course' && $object_type !='Class' ){
        if($catalog_reg =='Course'){
            $node_id = getNodeIdResults($courseId,$object_type);
        }
        else{
             $node_id = getNodeIdResults($classId,$object_type);
        }
    }
    else{
    $node_id = getNodeIdResults($classId,'cre_sys_obt_cls');
    }

    $userid = getDrupalIdOfUser($user_id);
    $resClsDetials->is_cart_added = isCartAdded($node_id,$userid);

    	$select = db_select('slt_survey_mapping','sc');
		$select->addField('sc','pre_status');
		$select->condition('sc.object_id',$classId);
		$select->condition('sc.object_type','cre_sys_obt_cls');
		expDebug::dPrintDBAPI('query for assessment',$select);
		$AssessmentStatus = $select->execute()->fetchAll();

    $resClsDetials->AssessmentStatus = $AssessmentStatus;
    
    if(!empty($resClsDetials->enrolled_id->enrolled_id)){
    	 
    	$enrollmetId =  $resClsDetials->enrolled_id->enrolled_id;
    	$select_weived = "SELECT CONCAT(exmp.exempted_status) as exempted_sts FROM slt_enrollment_exempted exmp WHERE exmp.enrollment_id = $enrollmetId  AND exmp.enroll_type = 'class' ORDER BY id DESC LIMIT 1";
    	expDebug::dPrintDBAPI('getRegisteredOrNot88998899 ', $select_weived);
    	$qry_res  = db_query($select_weived);
    	$sessionsList = $qry_res->fetchAll();
    	$resClsDetials->waived_status = $sessionsList[0]->exempted_sts;
    	 
    }
    
    
    if(isset($_REQUEST['mobile_request'])){
    
    	$object_type_code = "cre_sys_obt_crs";
    	if ($object_type == "Class")
    	{
    		$object_type_code = "cre_sys_obt_cls";
    	}elseif ($object_type == 'cre_sys_obt_crt' || $object_type == 'cre_sys_obt_cur' || $object_type == 'cre_sys_obt_trn')
    	{
    		$object_type_code = "cre_sys_obt_trp";
    	}
    	$attachment_status = getAttachmentDetails($resClsDetials->cls_id,$object_type_code,"");
    	$count = count($attachment_status);
    
    	expDebug::dPrint("getAttachmentDe:tails Results".print_r($attachment_status,true),5);
    	$resClsDetials->attachment_status=$attachment_status;
    	
    	
    	expDebug::dPrint("allcatalogresultsssss".print_r($resClsDetials,true),5);
    	if($resClsDetials->object_type != 'Class' && $resClsDetials->object_type != 'Course'){
    		if($resClsDetials->enrolled_id->master_enrolled_id){
    		$user_detail=db_query("select overall_progress from slt_master_enrollment where id=".$resClsDetials->enrolled_id->master_enrolled_id);
    		expDebug::dPrintDBAPI("query12345tptptptptptptptptpt ", $user_detail);
    		$userDetail = $user_detail->fetchAll();
    		$resClsDetials->class_progress = $userDetail[0]->overall_progress;
    }
    
    	}else{
    	if($resClsDetials->enrolled_id->enrolled_id){
    		$user_detail=db_query("select progress from slt_enrollment where id=".$resClsDetials->enrolled_id->enrolled_id);
    		expDebug::dPrintDBAPI("query12345rchangese3edede ", $user_detail);
    		$userDetail = $user_detail->fetchAll();
    		$resClsDetials->class_progress = $userDetail[0]->progress;
    	}
    	}

    	if($object_type =='Class'){
    		 
    		if($resClsDetials->crs_id != ''){
    			$equivalanceclass = checkEquivalence($user_id, $resClsDetials->crs_id, $resClsDetials->cls_id,$adminEnrollCall=false, $fromCart = false);
    			expDebug::dPrint(' $equivalanceclass9999 = ' . print_r($equivalanceclass,true) , 4 );
    			$equiArray = array();
    			if(count($equivalanceclass)>0){
    				for($i=0;$i<count($equivalanceclass);$i++){
    					if($equivalanceclass[$i]->equv_id != ""){
    						$equi_class = $equivalanceclass[$i]->equv_title;
    						expDebug::dPrint(' $equivalanceclass123445555 = ' . print_r($equi_class,true) , 4 );
    						$equi_title = array('equi_title'=>$equi_class);
    						array_push($equiArray,$equi_title);
    					}
    				}
    				expDebug::dPrint(' $equivalanceclass1234 = ' . print_r($equiArray,true) , 4 );
    				$resClsDetials->equi_class = $equiArray;
    				 
    			}
    	
    			$courseDetails_query = "select title,complete_date,validity_days from slt_course_template  where id = ".$resClsDetials->crs_id;
    			$qry_res  = db_query($courseDetails_query);
    			$courseDetails = $qry_res->fetchAll();
    			$resClsDetials->course_details = $courseDetails;
    		}
    		 
    	}
    	
    	
  }
    

  }
  return $results;
  }
  }catch (Exception $ex) {
    watchdog_exception('getCatalogSearchWrapperAPI', $ex);
    expertusErrorThrow($ex);
  }
}
/**
 *  Callback of the class Search Details in course level registration.
 *  It will display the list of records available within select class popup in Catalog page.
 *  @param $userId
 *    - Id of logged in user
 *  $courseId
 *    - Course id for selecting a class
 */
function getCourseLevelCatalogClassResults($userId,$courseId){
  //module_load_include('inc', 'exp_sp_lnrsearch', 'exp_sp_lnrsearch');
  //global $catalog_reg;
  try{
  $lnrSrch = new LnrSearchCatalog();
  $lnrSrch->select = catalogClassSearchQuery($userId,false,'selectclass');
  $lnrSrch->searchTitleClass($lnrSrch->setParamTitleVal('title'));
  $lnrSrch->searchTypeClass($lnrSrch->setParamVal('dl_type'),'');
  $lnrSrch->select->condition('cls.course_id',$courseId);
  $lnrSrch->select->condition('cls.status','lrn_cls_sts_atv','=');
  $lnrSrch->select->where('cls.dedicated_class_flag = \'N\' or cls.dedicated_class_flag is null');
  //$lnrSrch->select->groupby('cls.id');
  filterByUserAccessPrivileges($lnrSrch->select, 'cls','','','','Class');
  //$lnrSrch->select->condition('cls.id',$changesClassId,'!=');
  /*-- Fix for #38386 : order by moved to the end of the query --*/
  $lnrSrch->calculatePagination();
  $lnrSrch->searchSortBy($lnrSrch->setParamVal('sortby'));
  $lnrSrch->select->orderBy('mro_id','asc');

  $lnrSrch->select->range($lnrSrch->start, $lnrSrch->limit);
  expDebug::dPrintDBAPI('getCourseLevelCatalogClassResults query:', $lnrSrch->select);
  $qry_search_results = $lnrSrch->select->execute()->fetchAll();

  $srch_result_response->header = theme('exp_sp_lnrsearch-catalogclass-search-view-header', array('userId' => $userId, 'courseId' =>$courseId));
  $srch_result_response->total   = $lnrSrch->total_pages;
  $srch_result_response->records = $lnrSrch->qry_search_count;
  $srch_result_response->page    = $lnrSrch->page;
  $srch_result_response->userId  = $userId;
  $UserDefaultCurrencyArr = getUserDefaultCurrency();
  $UserPrefCurrCode = $UserDefaultCurrencyArr['currency_code'];
  $UserPrefCurrSym = $UserDefaultCurrencyArr['currency_sym'];

  $_SESSION['jq_count'] = array();
  $i=0;
  $detail = array();
	foreach ($qry_search_results as $rec_search_results) {
		$detail['Course']['cls_id'][]=$rec_search_results->cls_id;
		$detail['Course']['crs_id'][]=$rec_search_results->crs_id;
		$detail['Course']['dl_type'][]=$rec_search_results->delivery_type_code;
	}
	$subQuery = '';
	$subTpQuery = '';
	$usertimezone = getUserPHPTimezoneName();
	$usertimezone = $usertimezone[0]->zone_name;
	$timeZone = date_default_timezone(false);
	$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
	$usertimezone = empty($usertimezone)? $defaultTimezone : $usertimezone;
  if(count($detail['Course']['cls_id'])>0){
		$clsids = implode(',',$detail['Course']['cls_id']);
		$subQuery = "
					select  
					cls.id as cls_id
					,crs.id as crs_id
					,'Class' as ob_type
					,nod1.node_id as cls_nid
					,null as crs_nid
					,loc.name as loc_name
					,ses1.capacity_max
					,ses1.waitlist_count
					,ses1.id as session_id
					,if(brul.entity_id is null, 0, 1) as is_multi_reg
					,enroll.reg_count
					,enroll.wtl_count
					,ses1tz.attr2 as session_timezone
					-- Getting Start time
					-- -- -- Without converted to TZ
					,date_format(concat(date_format(ses1.start_date,'%Y-%m-%d'),' ',ses1.start_time),'%b %d, %Y') as ilt_session_start_date_format
					,date_format(concat(date_format(ses1.start_date,'%Y-%m-%d'),' ',ses1.start_time),'%h:%i') as ilt_session_start_time_format
					,date_format(concat(date_format(ses1.start_date,'%Y-%m-%d'),' ',ses1.start_time),'%p') as ilt_session_start_time_form
					-- -- -- Converted to TZ
					,date_format(CONVERT_TZ(concat(date_format(ses1.start_date,'%Y-%m-%d'),' ',ses1.start_time),ses1tz.attr2,'$usertimezone'),'%b %d, %Y') as vc_session_start_date_format
					,date_format(CONVERT_TZ(concat(date_format(ses1.start_date,'%Y-%m-%d'),' ',ses1.start_time),ses1tz.attr2,'$usertimezone'),'%h:%i') as vc_session_start_time_format
					,date_format(CONVERT_TZ(concat(date_format(ses1.start_date,'%Y-%m-%d'),' ',ses1.start_time),ses1tz.attr2,'$usertimezone'),'%p') as vc_session_start_time_form
					-- Getting End time 
					-- -- -- Without converted to TZ
					,date_format(concat(date_format(ses2.start_date,'%Y-%m-%d'),' ',ses2.end_time),'%b %d, %Y') as ilt_session_start_end_format
					,date_format(concat(date_format(ses2.start_date,'%Y-%m-%d'),' ',ses2.end_time),'%h:%i') as ilt_session_end_time_format
					,date_format(concat(date_format(ses2.start_date,'%Y-%m-%d'),' ',ses2.end_time),'%p') as ilt_session_end_time_form
					-- -- -- Converted to TZ
					,date_format(CONVERT_TZ(concat(date_format(ses2.start_date,'%Y-%m-%d'),' ',ses2.end_time),ses2tz.attr2,'$usertimezone'),'%b %d, %Y') as vc_session_start_end_format
					,date_format(CONVERT_TZ(concat(date_format(ses2.start_date,'%Y-%m-%d'),' ',ses2.end_time),ses2tz.attr2,'$usertimezone'),'%h:%i') as vc_session_end_time_format
					,date_format(CONVERT_TZ(concat(date_format(ses2.start_date,'%Y-%m-%d'),' ',ses2.end_time),ses2tz.attr2,'$usertimezone'),'%p') as vc_session_end_time_form
					,if(ses1.id = ses2.id , 0, 1) as is_multi_session
					,ses2tz.attr1 as attr1
					,ses2tz.attr4 as attr4
					,null as crs_count
				from
				slt_course_class cls
				inner join slt_course_template crs on crs.id = cls.course_id
				inner join slt_node_learning_activity nod1 on nod1.entity_id = cls.id and nod1.entity_type = 'cre_sys_obt_cls' 
				left outer join slt_location loc on loc.id = cls.location_id
				left outer join slt_course_class_session ses1 on ses1.class_id=cls.id 
				  and ses1.id = (select id from slt_course_class_session where class_id=cls.id order by start_date,start_time limit 1)
				left outer join slt_course_class_session ses2 on ses2.class_id=cls.id 
				  and ses2.id = (select id from slt_course_class_session where class_id=cls.id order by start_date desc ,start_time desc limit 1)
				left join slt_profile_list_items ses1tz on ses1tz.code = ses1.timezone
				left join slt_profile_list_items ses2tz on ses2tz.code = ses2.timezone
				-- Multi-registration check
				left join (
				    select br.entity_id as entity_id from slt_business_rule_mapping br
				    inner join slt_profile_list_items brpf on brpf.code = br.business_rule_code and brpf.is_active = 'Y'
				    where br.entity_type = 'cre_sys_obt_cls' and br.business_rule_code = 'cre_sys_brl_rra'
				  ) brul on brul.entity_id = cls.id
				-- Get enrollment count for validate seat availability
				left join (
				    select enr.class_id, count(if(enr.reg_status != 'lrn_crs_reg_wtl',1,null)) as reg_count, count(if(enr.reg_status = 'lrn_crs_reg_wtl',1,null)) as wtl_count
				    from slt_enrollment enr
				    inner join slt_course_class cl on cl.id=enr.class_id  and cl.delivery_type in ('lrn_cls_dty_ilt','lrn_cls_dty_vcl')
				    where enr.reg_status IN ('lrn_crs_reg_cnf', 'lrn_crs_reg_ppm', 'lrn_crs_reg_rsv', 'lrn_crs_reg_wtl')
				    and enr.class_id in (".$clsids.")
				    and IF(enr.reg_status = 'lrn_crs_reg_cnf', enr.comp_status IN('lrn_crs_cmp_cmp','lrn_crs_cmp_inp','lrn_crs_cmp_enr','lrn_crs_cmp_att'),1=1)
				    group by enr.class_id
				  ) enroll on enroll.class_id = cls.id
				where cls.id in (".$clsids.")
		";
	}
  if(!empty($subQuery)){ 
		$sRes = db_query($subQuery)->fetchAll();
		expDebug::dPrint("Sub Query results ".print_r($sRes,true),3);
		$subResult = array();
		foreach($sRes as $srs){
			$subResult[$srs->ob_type.'_'.$srs->cls_id] = $srs;
		}
	}
  foreach ($qry_search_results as $rec_search_results) {
  	$subRst = $subResult[$rec_search_results->object_type.'_'.$rec_search_results->cls_id];
    //if($rec_search_results->currency_type==null || $rec_search_results->currency_type=='' || $rec_search_results->currency_type=="cre_sys_crn_usd") {
      $rec_search_results->currency_type=$UserPrefCurrSym;
    /* }else{
      $rec_search_results->currency_type='$';
    } */

    if($rec_search_results->export_compliance == null)
      $rec_search_results->export_compliance = 'N';

    if($rec_search_results->price == null || $rec_search_results->price == '')
      $rec_search_results->price = '0.00';

    $rec_search_results->row =  $i;
    $srch_result_response->rows[$i]['id']   = $rec_search_results->cls_id;

   /* if($rec_search_results->object_type == 'Class') {
      $rec_search_results->available_seats = getAvailableSeats($rec_search_results->cls_id);
      $rec_search_results->node_id = getNodeIdResults($rec_search_results->cls_id,'cre_sys_obt_cls');
    }*/
    $rec_search_results->node_id = $subRst->cls_nid;
  	if($rec_search_results->delivery_type_code == 'lrn_cls_dty_ilt' || $rec_search_results->delivery_type_code == 'lrn_cls_dty_vcl' ){
			// Seat availability calculation
			if ($subRst->capacity_max != 0) {
		    $no_of_available_seats = $subRst->capacity_max - $subRst->reg_count;
		  }
		
		  if ($no_of_available_seats < 0 ) {
		    $no_of_available_seats = 0;
		  }
		  // Watilist calculation
			if ($subRst->capacity_max != 0) {
		    if($subRst->capacity_max <= ($subRst->reg_count+$subRst->wtl_count)) {
		      $no_of_waitlist_seats = ($subRst->capacity_max + $subRst->waitlist_count) - ($subRst->reg_count+$subRst->wtl_count);
		    } else {
		      $no_of_waitlist_seats = 0;
		    }
		  }
		  if ($no_of_waitlist_seats < 0 ) {
		    $no_of_waitlist_seats = 0;
    }
      $rec_search_results->available_seats = $no_of_available_seats;
      $rec_search_results->waitlist_seats  = $no_of_waitlist_seats;
      $rec_search_results->session_details = $subRst;
  	}
  	$rec_search_results->location = $subRst->loc_name;
    //$rec_search_results->location = getLocationNameDts($rec_search_results->location_id);
    //$rec_search_results->session_details = getSessionDetails($rec_search_results->crs_id,$rec_search_results->cls_id,$rec_search_results->delivery_type_code);
    //$rec_search_results->multi_register = is_multiregistration_allowed($rec_search_results->cls_id);
    $rec_search_results->multi_register = !empty($subRst->is_multi_reg)? true : false;
	  $rec_search_results->registered_class = !empty($subRst->is_multi_reg)? explode(',',$subRst->is_multi_reg) : NULL ;
	  $rec_search_results->nodeIdFiveStar = explode(',',$subRst->cls_nid);

    $rec_search_results->widgetId = 'lnr-catalog-search';
    $srch_result_response->rows[$i]['cell'] = array(
           	'image'               => theme('exp_sp_lnrsearch-view-image', array('results' => $rec_search_results)),
           	'details'             => theme('exp_sp_lnrsearch-view-detail', array('results' => $rec_search_results,'userId' => $userId)),
           	'action'              => theme('exp_sp_lnrsearch-catalog-class-view-action', array('results' => $rec_search_results,'userId' => $userId))
    );
    $_SESSION['jq_modal'][$i] = $rec_search_results->cls_id;
    $i++;
  }
  echo drupal_json_output($srch_result_response);
  }catch (Exception $ex) {
  	watchdog_exception('getCourseLevelCatalogClassResults', $ex);
  	expertusErrorThrow($ex);
  }
}


/**
 * Get results for Class title autocomplete.
 *
 * @return
 *   Auto Complete Course-Class title Results
 */
function getSelectClsAutoComplete($delType,$userId,$courseId){
	try{
  //module_load_include('inc', 'exp_sp_lnrsearch', 'exp_sp_lnrsearch');
  global $catalog_reg;
  $delType = ($delType == t('LBL428'))? '' : $delType;
  $lnrSrch = new LnrSearchCatalog();
  $lnrSrch->select = catalogClassSearchQuery($userId);
  $lnrSrch->searchTitleClass($_GET['z']);
  $lnrSrch->searchTypeClass($delType,'');
  $lnrSrch->select->condition('cls.status','lrn_cls_sts_atv','=');
  $lnrSrch->select->groupby('cls.id');
  filterByUserAccessPrivileges($lnrSrch->select, 'cls','','','cre_sys_obt_cls','');
  $lnrSrch->select->condition('cls.course_id',$courseId);
  //$lnrSrch->select->condition('cls.id', $changeClassId, '!=');
  expDebug::dPrintDBAPI('getSelectClsAutoComplete query:', $lnrSrch->select);
  $qry_search_results = $lnrSrch->select->execute()->fetchAll();
  foreach ($qry_search_results as $rec_srch_autocomplete) {
    print $rec_srch_autocomplete->cls_title."\n";
  }
  }catch (Exception $ex) {
  	watchdog_exception('getSelectClsAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}

function getCompletedClassId($courseId) {
	try{
  $userId = getSltpersonUserId();

  $selectCls = db_select('slt_node_learning_activity', 'slt_node');
  $selectCls->leftJoin('slt_enrollment','slt_enr','slt_enr.class_id = slt_node.entity_id');
  $selectCls->addField('slt_node', 'node_id');
  $selectCls->condition('slt_node.entity_type','cre_sys_obt_cls','=');
  $selectCls->condition('slt_enr.course_id',$courseId,'=');
  $selectCls->condition('slt_enr.reg_status','lrn_crs_reg_cnf','=');
  $selectCls->condition('slt_enr.comp_status','lrn_crs_cmp_cmp','=');
  $selectCls->condition('slt_enr.user_id',$userId,'=');
  $result = $selectCls->execute()->fetchField();
  expDebug::dPrintDBAPI('getCompletedClassId query:', $selectCls);

  return $result;
  }catch (Exception $ex) {
  	watchdog_exception('getCompletedClassId', $ex);
  	expertusErrorThrow($ex);
  }
}

function addClassToCartAPI($user_id,$courseId,$classId,$deliveryType){
	try{
			drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
			include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc');

			$pastClass = checkPastDateOrNot($classId,$deliveryType);
			expDebug::dPrint('pastClasspastClass ' . $pastClass, 4);
			if($pastClass != 1)
				{
					$nodeId = getNodeIdResults($classId,'cre_sys_obt_cls');
					$_SESSION['uc_referer_uri'] = 'addClassToCartAPI';
					$res = callAddToCart('add',$nodeId,$classId,$courseId,'','api',$user_id);
					return array((object)array('status' => $res));
				}
			else
				{
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "Class has been delivered";
					return $errobj;
				}

	}catch (Exception $ex) {
		watchdog_exception('addClassToCartAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function getCheckAddressAPI(){
	try{
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);


	$selectStmt = db_select('uc_countries', 'ct');
  	// Set conditions
  	$selectStmt->condition('ct.country_id', variable_get('uc_check_mailing_country', 840));
 	 // Select state name field for returning
 	 $selectStmt->addField('ct', 'country_name', 'name');
  	expDebug::dPrintDBAPI(' $selectStmt = ' ,$selectStmt);
 	 // Execute query
 	 $country_name = $selectStmt->execute()->fetchField();

  	return (object)array('mailing_name' =>  variable_get('uc_check_mailing_name', ''),'mailing_company' => variable_get('uc_check_mailing_company', ''),'mailing_street1' => variable_get('uc_check_mailing_street1', ''),'mailing_street2' => variable_get('uc_check_mailing_street2', ''),'mailing_city' => variable_get('uc_check_mailing_city', ''),'mailing_zone' => variable_get('uc_check_mailing_zone', ''),'mailing_postal_code' => variable_get('uc_check_mailing_postal_code', ''),'mailing_country' => $country_name);
	}catch (Exception $ex) {
		watchdog_exception('getCheckAddressAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function getBillingAddressAPI($user_id){
	try{
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

			$uid = getDrupalIdOfUser($user_id);

 			$select = db_select('uc_orders','uc');
			$select->addField('uc', 'billing_first_name' , 'first_name');
			$select->addField('uc', 'billing_last_name', 'last_name');
			$select->addField('uc', 'billing_phone', 'phone');
			$select->addField('uc', 'billing_company', 'company');
			$select->addField('uc', 'billing_street1', 'street1');
			$select->addField('uc', 'billing_street2', 'street2');
			$select->addField('uc', 'billing_city', 'city');
			$select->addField('uc', 'billing_zone', 'zone');
			$select->addField('uc', 'billing_postal_code', 'postal_code');
			$select->addField('uc', 'billing_country', 'country');
			$select->addField('uc', 'order_id', 'order_id');
			$select->condition('uc.uid', $uid);
			$select->groupBy('uc.billing_first_name');
			$select->groupBy('uc.billing_last_name');
			$select->groupBy('uc.billing_company');
			$select->groupBy('uc.billing_street1');
			$select->groupBy('uc.billing_street2');
			$select->groupBy('uc.billing_city');
			$select->groupBy('uc.billing_zone');
			$select->groupBy('uc.billing_postal_code');
			$select->groupBy('uc.billing_country');

			$result = $select->execute();

			$addresses = array();
		  	while ($address = $result->fetchAssoc()) {
		  	if (!empty($address['street1']) || !empty($address['postal_code'])) {
		  	$selectStmt = db_select('uc_countries', 'ct');
		  	// Set conditions
		  	$selectStmt->condition('ct.country_id',$address['country']);
		 	 // Select state name field for returning
		 	 $selectStmt->addField('ct', 'country_name', 'name');
		  		expDebug::dPrintDBAPI(' $selectStmt = ' ,$selectStmt);
		 	 // Execute query
		 	 $country_name = $selectStmt->execute()->fetchField();

		 	 $selectzone = db_select('uc_zones', 'zn');
		  	// Set conditions
		  	$selectzone->condition('zn.zone_id',$address['zone']);
		 	 // Select state name field for returning
		 	 $selectzone->addField('zn', 'zone_name', 'name');
		  	expDebug::dPrintDBAPI(' $selectzone = ' ,$selectzone);
		 	 // Execute query
		 	 $zone_name = $selectzone->execute()->fetchField();
		 	 expDebug::dPrint('Sho checking --->1'.print_r($addresses,true));
			expDebug::dPrint('okokokok'.$addresses[0]['country']);
			$address['country'] = $country_name;
			$address['zone'] = $zone_name;
		  	$addresses[] = $address;
  	}
  }

expDebug::dPrint('country_name country_name'.print_r($country_name,true));
  return $addresses;
	}catch (Exception $ex) {
		watchdog_exception('getBillingAddressAPI', $ex);
		expertusErrorThrow($ex);
	}
}


function createUcOrderIdAPI($user_id,$learnerid,$amount){
	try{
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$uid = getDrupalIdOfUser($user_id);
		include_once($_SERVER["DOCUMENT_ROOT"]. 'sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc');
		
	//Below function will set currency details in $_REQUEST
		$currencyDetails = splitCurrencyInputAndSetInRequest();
		
		if(empty($currencyDetails))
		{
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->field = 'currency_type';
		$errobj->errormsg = "The given Currency type is invalid or inactive status";
		return $errobj;
		}
		expDebug::dPrint('currency_type = ' .$_REQUEST['currency_type']);
		expDebug::dPrint('currency_sym  = ' .$_REQUEST['currency_sym']);
		// Create  uc_orders order_id
		$order = uc_order_new($uid);
		expDebug::dPrint('calling after 1'.print_r($order,true),5);
		$fields = array(
				'order_total' => $amount,
				'currency_type' => $_REQUEST['currency_type'],
		);
		$updStmt = db_update('uc_orders');
		$updStmt->fields($fields);
		$updStmt->condition('order_id', $order->order_id);
		expDebug::dPrintDBAPI('$updStmt', $updStmt);
		$updStmt->execute();

return $order;
	}catch (Exception $ex) {
		watchdog_exception('createUcOrderIdAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function addNewBillingAddressAPI($user_id,$userid,$order_ID,$first_name,$last_name,$phone,$company,$street1,$street2,$city,$zone,$postal_code,$country,$slt_orderID,$product_count,$old_order_id,$action){
	try{
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		include_once($_SERVER["DOCUMENT_ROOT"]. 'sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc');
		$uid = getDrupalIdOfUser($user_id);
		expDebug::dPrint('order_ID after 1'.print_r($order_ID,true),5);
		
	//Below function will set currency details in $_REQUEST
		$currencyDetails = splitCurrencyInputAndSetInRequest();
		
		if(empty($currencyDetails))
		{
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->field = 'currency_type';
		$errobj->errormsg = "The given Currency type is invalid or inactive status";
		return $errobj;
		}
		expDebug::dPrint('currency_type = ' .$_REQUEST['currency_type']);
		expDebug::dPrint('currency_sym  = ' .$_REQUEST['currency_sym']);

		if($action == 'existingaddress')
		{
		$order_ID = $old_order_id;
		}
		$selectStmt = db_select('uc_countries', 'ct');
  	// Set conditions
  	$selectStmt->condition('ct.country_name',$country);
 	 // Select state name field for returning
 	 $selectStmt->addField('ct', 'country_id', 'id');
  	expDebug::dPrintDBAPI(' $selectStmt = ' ,$selectStmt);
 	 // Execute query
 	 $country_id = $selectStmt->execute()->fetchField();

 	 $selectStmt = db_select('uc_zones', 'zo');
  	// Set conditions
  	$selectStmt->condition('zo.zone_name',$zone);
 	 // Select state name field for returning
 	 $selectStmt->addField('zo', 'zone_id', 'id');
  	expDebug::dPrintDBAPI(' $selectStmt = ' ,$selectStmt);
 	 // Execute query
 	 $zone_id = $selectStmt->execute()->fetchField();
		// uc_orders id update
		$fields = array(
				'billing_first_name' => $first_name,
				'billing_last_name' => $last_name,
				'billing_phone' => $phone,
				'billing_company' => $company,
				'billing_street1' => $street1,
				'billing_street2' => $street2,
				'billing_city' => $city,
				'billing_zone' => $zone_id,
				'billing_postal_code' => $postal_code,
				'billing_country' => $country_id,
				'product_count'=> $product_count,
				'currency_type' => $_REQUEST['currency_type']
		);
		$updStmt = db_update('uc_orders');
		$updStmt->fields($fields);
		$updStmt->condition('order_id', $order_ID);
		expDebug::dPrintDBAPI('$updStmt', $updStmt);
		$ucUpdNum = $updStmt->execute();

		// slt_order id update
		$ord_fields = array(
				'uc_order_id' => $order_ID,
				'user_id' => $user_id,
		);
		$ord_ins = db_update('slt_order');
		$ord_ins->fields($ord_fields);
		$ord_ins->condition('id', $slt_orderID);
		expDebug::dPrintDBAPI('$ord_ins', $ord_ins);
		$sltUpdNum = $ord_ins->execute();

		expDebug::dPrint('$ucUpdNum', $ucUpdNum);
		expDebug::dPrint(' $sltUpdNum = '.$sltUpdNum);


 $result-> status =  'success';

 return $result;

	}catch (Exception $ex) {
		watchdog_exception('addNewBillingAddressAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function listUsersUnderMangerAPI($user_id,$userid,$userName,$location,$reporttype){

try{
// drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_sitesetup/exp_sp_administration_module_info/exp_sp_admin_userpoints/exp_sp_admin_userpoints.inc";

if(!is_numeric($user_id))
{
	$errobj=new stdClass();
	$errobj->isValidateError = 1;
	$errobj->field = "user_id";
	$errobj->errormsg = 'Invalid User ID';
	return $errobj;
}




// $select = db_select('slt_person', 'ct');
// $select->leftJoin('users', 'u', 'ct.user_name = u.name');
// $select->addExpression('COUNT(ct.id)', 'id');
// $select_or_condition = db_or();
// $select_or_condition->condition('ct.manager_id',$user_id);
// $select_or_condition->where('FIND_IN_SET('.$user_id.', ct.dotted_mngr_id)');
// $select->condition($select_or_condition);

// $row_count = $select->execute()->fetchField();
// expDebug::dPrintDBAPI('$row_count = ', $select);
// expDebug::dPrintDBAPI('$row_count = '.$row_count);

// if($row_count <= 0)
// {
// 	$errobj=new stdClass();
// 	$errobj->isValidateError = 1;
// 	$errobj->field = "user_id";
// 	$errobj->errormsg = 'The given user is not a manager';
// 	return $errobj;
// }

$selectStmt = db_select('slt_person', 'ct');
$selectStmt->leftJoin('users', 'u', 'ct.user_name = u.name');
$selectStmt->leftJoin('file_managed', 'f', 'u.picture = f.fid');
$selectStmt->leftJoin('slt_profile_list_items', 'pro', 'ct.job_title = pro.code');
$selectStmt->leftJoin('slt_country', 'sc', 'sc.country_code = ct.country');
$selectStmt->leftjoin('slt_state', 'st', 'st.country_code= ct.country AND st.state_code=ct.state');
$selectStmt->leftJoin('slt_person_jobrole_mapping', 'prm', 'prm.user_id = ct.id');
$selectStmt->leftJoin('slt_profile_list_items', 'prof', 'prm.job_role = prof.code');
$selectStmt->leftJoin('slt_person_other_manager', 'spom', 'spom.user_id = ct.id and spom.is_direct=\'N\'');
$selectStmt->addExpression('IF((select count(*) from slt_person_other_manager as spom1 where spom1.manager_id = '.$user_id.' and spom1.is_direct=\'N\' and ct.id = spom1.user_id ) > 0, 1, 0)', 'dotted_manager');
//$selectStmt->addExpression('IF( FIND_IN_SET('.$user_id.',ct.dotted_mngr_id) > 0, 1, 0)', 'dotted_manager');
// Select state name field for returning
    $db_or = db_or();
$db_or->condition('ct.user_name', '%' . db_like($userName) . '%' ,'LIKE');
$db_or->condition('ct.full_name', '%' . db_like($userName) . '%' ,'LIKE');
$selectStmt->condition($db_or);


	if (!empty($reporttype))
	{
		if($reporttype == 'direct'){
			$selectStmt->condition('ct.manager_id',$user_id);
		}else if($reporttype == 'virtual'){
			$selectStmt->condition('spom.manager_id',$user_id);
			//$selectStmt->where('FIND_IN_SET('.$user_id.', ct.dotted_mngr_id)');
		}
		else
		{
			$db_or_condition = db_or();
			$db_or_condition->condition('ct.manager_id',$user_id);
// 			$db_or_condition->condition(FIND_IN_SET('.$user_id.',ct.dotted_mngr_id));
			$db_or_condition->condition('spom.manager_id',$user_id);
			$selectStmt->condition($db_or_condition);
		}
	}
	else
	{
			$db_or_condition = db_or();
			$db_or_condition->condition('ct.manager_id',$user_id);
			$db_or_condition->condition('spom.manager_id',$user_id);
			$selectStmt->condition($db_or_condition);
	}

    if (!empty($location)){
    $selectStmt->condition(db_and()->condition('ct.city', '%' . db_like($location) . '%', 'LIKE'));
    }
    if (!empty($_REQUEST['country_code'])){ 
    	/* $select = db_select('uc_countries', 'uct');
    	// Set conditions
    	$select->condition('uct.country_id',$_REQUEST['country_code']);
    	// Select state name field for returning
    	$select->addField('uct', 'country_iso_code_2');
    	expDebug::dPrintDBAPI(' $selectStmt = ' ,$select);
    	// Execute query
    	$country_id = $select->execute()->fetchField(); */
   		$selectStmt->condition('ct.country',$_REQUEST['country_code']);
    if (!empty($_REQUEST['state_code'])){
        /*
    	$select = db_select('uc_zones', 'uc_zo');
		$select->leftjoin('slt_state','stc','stc.state_name = uc_zo.zone_name');
    	// Set conditions
    	$select->condition('uc_zo.zone_id',$_REQUEST['state_code']);
    	// Select state name field for returning
    	$select->addField('stc', 'state_code');
    	expDebug::dPrintDBAPI(' $selectStmt = ' ,$select);
    	// Execute query

    	$zone_id = $select->execute()->fetchField();
        */
        $selectStmt->condition('st.state_code',$_REQUEST['state_code']);
    }
    }

    if (!empty($_REQUEST['job_role']) || !empty($_REQUEST['job_title'])){
    	$select_or = db_or();
    	if(!empty($_REQUEST['job_role']))
    	{
    		$job_role = explode("|",$_REQUEST['job_role']);
    		$select_or->condition('prm.job_role', $job_role, 'IN');
    	}

    	if(!empty($_REQUEST['job_title'])){
    		$job_title = explode("|",$_REQUEST['job_title']);
    		$select_or->condition('ct.job_title', $job_title, 'IN');
    	}
    	$selectStmt->condition($select_or);
    }

    $selectStmt->distinct();
    $selectStmt->addField('ct', 'id', 'id');
		$selectStmt->addField('ct', 'first_name', 'first_name');
		$selectStmt->addField('ct', 'last_name', 'last_name');
		$selectStmt->addField('ct', 'full_name', 'full_name');
		$selectStmt->addField('ct', 'user_name', 'user_name');
		$selectStmt->addField('pro', 'name', 'job_title');
		$selectStmt->addExpression('GROUP_CONCAT(DISTINCT(prof.name))', 'job_role');
		$selectStmt->addField('ct', 'phone_no', 'phone_no');
		$selectStmt->addField('ct', 'mobile_no', 'mobile_no');
		$selectStmt->addField('ct', 'email', 'email');
		$selectStmt->addField('ct', 'manager_id', 'manager_id');
		$selectStmt->addExpression('GROUP_CONCAT(DISTINCT(spom.manager_id))', 'report_type');
		//$selectStmt->addField('ct', 'dotted_mngr_id', 'report_type');
    // Select Picture URI From Drupal Table
    $selectStmt->addField('f', 'uri', 'user_picture');
    $sortby =$_REQUEST['sortby'];
if($sortby == "AZ") {
      $xorderby = "ct.full_name";
      $xorderbylist = "asc";
    }else if ($sortby == "ZA") {
      $xorderby = "ct.full_name";
      $xorderbylist="desc";
    }else if ($sortby == "Time") {
      $xorderby="ct.updated_on";
      $xorderbylist="desc";
    }else {
      $xorderby="ct.full_name";
      $xorderbylist="asc";
    }
    $selectStmt->orderBy($xorderby,$xorderbylist);
    $selectStmt->groupBy('ct.id');
    expDebug::dPrintDBAPI('$selectStmt = ' ,$selectStmt);
// Execute query
$result = $selectStmt->execute()->fetchAll();

for($i=0;$i<count($result);$i++)
 	    {
  $totalPoints	  =   getTotalPointsGainedByUser($result[$i]->id);
  $totalActionPoints  = getActionPointsForUser($result[$i]->id);
  $userBadgesView     = BadgesCount($totalActionPoints);
  $result[$i]->points = $totalPoints;
  $result[$i]->badges = $userBadgesView;
  $select = db_select('slt_enrollment', 'en');
  $select->leftJoin('slt_profile_list_items', 'itm', 'itm.code = en.comp_status');
  $select->condition('en.user_id',$result[$i]->id);
  $select->condition('en.reg_status','lrn_crs_reg_cnf');
  $select->addField('itm', 'name', 'status_text');
  $select->addExpression('COUNT(en.comp_status)', 'enroll_count');
  //$select->addField('en', 'comp_status', 'comp_status');
  $select->groupBy('en.comp_status');
  expDebug::dPrintDBAPI('select$select = ' ,$select);
  $enroll_count = $select->execute()->fetchAllKeyed();
  expDebug::dPrint('$enroll_countenroll_countenroll_countenroll_count ='.print_r($enroll_count,true));
  $result[$i]->enroll_count = $enroll_count;
  }

  expDebug::dPrint('resullllttttttttttttt'.print_r($result,true));

    return $result;
}catch (Exception $ex) {
watchdog_exception('listUsersUnderMangerAPI', $ex);
expertusErrorThrow($ex);
}
}


function getJobDetailsAPI(){
// 	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_setup/exp_sp_administration_setup.inc');
	//expDebug::dPrint('InsideInsideInsideInside');
	$result = getListValueSetUpDetailList($_REQUEST['entitytype'],'list');
	return $result;
}


function getAiccStatusAPI($user_id,$userid,$id){
	try{
		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrenrollment/exp_sp_launch.inc');

		$selectStmt = db_select('slt_enrollment', 'ct');
		// Set conditions
		$selectStmt->condition('ct.id',$id);
		$selectStmt->condition('ct.user_id',$user_id);
		// Select state name field for returning
		$selectStmt->addField('ct', 'comp_status', 'comp_status');
		$selectStmt->addField('ct', 'score', 'score');
		expDebug::dPrintDBAPI(' $testtttt = ' ,$selectStmt);
		// Execute query
		$result = $selectStmt->execute()->fetchAll();

		$launch   = getWBTLaunchDetail($id,'lrn_cls_dty_wbt');
		$launcher=array();
		foreach ($launch as $key=>$words) {
			$launcher[$key]['id'] .=$words['id'];
			$launcher[$key]['contentid'] .=$words['contentid'];
			$launcher[$key]['versionid'] .=$words['versionid'];
			$launcher[$key]['lesscore'] .=$words['lesscore'];
			$launcher[$key]['status'] .=$words['status'];
			$launcher[$key]['maxattempt'] .=$words['maxattempt'];
			$launcher[$key]['attempts'] .=$words['attempts'];
		}
		$result[0] -> launch=$launcher;

		if(!empty($_REQUEST['aiccId']))
		{
			$select = db_select('slt_aicc_interaction', 'sa');
			$select->condition('sa.aicc_id',$_REQUEST['aiccId']);
			$select->addField('sa', 'session_id', 'session_id');
			expDebug::dPrintDBAPI(' $testtttt = ' ,$select);
			$session = $select->execute()->fetchField();
			$result[0] -> session=$session;
		}

		if($_REQUEST['mobile_request']){
				
			$user_detail=db_query("select progress from slt_enrollment where id=".$id);
			expDebug::dPrintDBAPI("query12345rashutoshhhh ", $user_detail);
			$userDetail = $user_detail->fetchAll();
			$result[0]->overallProgress = $userDetail[0]->progress;
				
		}
		
		expDebug::dPrint('$result123w3 ='.print_r($result,true));
		expDebug::dPrint('$aiccIdaiccIdaiccId ='.print_r($_REQUEST['aiccId'],true));
		return $result;
			
	}catch (Exception $ex) {
		watchdog_exception('getAiccStatusAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function updateUcOrderProductAPI($user_id,$userid,$order_id,$class_id,$class_title,$dl_name,$price){
	try{
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		include_once($_SERVER["DOCUMENT_ROOT"]. 'sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc');
		
	//Below function will set currency details in $_REQUEST
		$currencyDetails = splitCurrencyInputAndSetInRequest();
		
		if(empty($currencyDetails))
		{
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->field = 'currency_type';
		$errobj->errormsg = "The given Currency type is invalid or inactive status";
		return $errobj;
		}
		expDebug::dPrint('currency_type = ' .$_REQUEST['currency_type']);
		expDebug::dPrint('currency_sym  = ' .$_REQUEST['currency_sym']);
		$classid = explode("-~",$class_id);
		expDebug::dPrint('$class_id$class_id ='.print_r($classid,true));
		$classtitle = explode("-~",$class_title);
		$dlname = explode("-~",$dl_name);
		$amount = explode("-~",$price);
			expDebug::dPrint("isalreadyexistsisalreadyexistsisalreadyexists ");

		$selectStmt = db_select('uc_order_products','ct');
		$selectStmt->addField('ct', 'title');
        $selectStmt->condition('ct.order_id',$order_id);
        expDebug::dPrintDBAPI('query for checking',$selectStmt);
        $isalreadyexists = $selectStmt->execute()->fetchAll();
				expDebug::dPrint("isalreadyexistsisalreadyexistsisalreadyexists ".$isalreadyexists,4);
		if(count($isalreadyexists) > 0){
		expDebug::dPrint("isalreadyexists true");

		$result-> status ='Already exists';
			return $result;

		}else{

		expDebug::dPrint("isalreadyexists false");




		for($i=0; $i<count($classid);$i++)
		{
			$entity_type_Array = explode("-",$dlname[$i]);
			$entity_type = $entity_type_Array[0];
			if($entity_type != 'cre_sys_obt_crt' && $entity_type != 'cre_sys_obt_cur' && $entity_type != 'cre_sys_obt_trn')
			{
			$entity_type = 'cre_sys_obt_cls';
			}
		$select = db_select('slt_node_learning_activity','sc');
    $select->addField('sc','node_id');
		    $select->condition('sc.entity_id',$classid[$i]);
		    $select->condition('sc.entity_type',$entity_type);
    $node_id = $select->execute()->fetchField();




$insLog  = db_insert('uc_order_products');
				$insLog->fields(array(
						'order_id' => $order_id,
						'nid' => $node_id,
						'title' => $classtitle[$i],
						'model' => $dlname[$i],
						'qty' => '1',
						'cost' => $amount[$i],
						'price' => $amount[$i],
						'weight' => '0',
						'currency_type' => $_REQUEST['currency_type']

				));
				$logId = $insLog->execute();

				expDebug::dPrintDBAPI('select class lang course',$insLog);
				}

				$status = ( !empty($logId) )?'success':'failure';

	$result-> status =$status;
return $result;
}
	}catch (Exception $ex) {
		watchdog_exception('updateUcOrderProductAPI', $ex);
		expertusErrorThrow($ex);
	}
}


function deleteItemsFromCartAPI($lnrUserId, $adminUserId, $classid, $sltOrderId,$action,$deliverytypename){
	try{
        drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$uid=getDrupalIdOfUser($lnrUserId);
		$orderId_select=db_select('slt_order','ord');
		$orderId_select->addField('ord','id','ord_id');
		$orderId_select->condition('ord.user_id',$lnrUserId,'=');
		$orderId_select->condition('ord.order_status','cme_pmt_sts_rsv','=');
		$orderId=$orderId_select->execute()->fetchField();
	
if((empty($orderId) && !empty($classid)) || $action == 'delete'){

			if($deliverytypename == 'cre_sys_obt_crt' || $deliverytypename == 'cre_sys_obt_cur' || $deliverytypename == 'cre_sys_obt_trn' ){
			$object_type = $deliverytypename;
			}else{
			$object_type ='cre_sys_obt_cls';
			}
			expDebug::dPrint("Enroll order update in cart module when User update cart: ".$deliverytypename,4);


				$select = db_select('slt_node_learning_activity','sc');
			    $select->addField('sc','node_id');
			    $select->condition('sc.entity_id',$classid);
			    $select->condition('sc.entity_type',$object_type);
			    expDebug::dPrintDBAPI('query for nodeid',$select);
			    $node_id = $select->execute()->fetchField();

				$delete = db_delete('uc_cart_products');
				$delete->condition('nid',$node_id,'=');
				$delete->condition('cart_id',$uid,'=');
				expDebug::dPrintDBAPI('query for deleting',$delete);
				$delete->execute();

				//update slt_enrollment
				$select = db_select('slt_enrollment','se');
			    $select->addField('se','id');
			    $select->condition('se.class_id',$classid);
				$select->condition('se.order_id',$sltOrderId);
				expDebug::dPrintDBAPI('$select',$select);
			    $id = $select->execute()->fetchField();

			    if($deliverytypename == 'cre_sys_obt_crt' || $deliverytypename == 'cre_sys_obt_cur' || $deliverytypename == 'cre_sys_obt_trn' ){
						$masterEnrollId  = db_query("SELECT id FROM slt_master_enrollment WHERE program_id='".$classid."' AND overall_status='lrn_tpm_ovr_rsv' AND user_id='".$lnrUserId."' limit 1")->fetchField();
		                 expDebug::dPrint("masterEnrollIdmasterEnrollIdmasterEnrollIdmasterEnrollId".$masterEnrollId,4);
		                 //Update slt_master_enroll
		                $updateEnr = db_update('slt_master_enrollment');
		                $updateEnr->fields(array(
		                              'overall_status'=> 'lrn_tpm_ovr_rsc',
		                              'comp_date'		=> NULL,
		                              'updated_by'    => $lnrUserId,
		                			  'updated_on'=>now(),
		                              ));
		                $updateEnr->condition('id', $masterEnrollId);
		                $updateEnr->execute();


		                $select = db_select('slt_enrollment','se');
					    $select->addField('se','id');
						$select->condition('se.master_enrollment_id',$masterEnrollId);
						$select->condition('se.reg_status','lrn_crs_reg_rsv');
						$select->condition('se.user_id',$lnrUserId);
						expDebug::dPrintDBAPI('$select',$select);
					    $result = $select->execute()->fetchAll();


			          	foreach ($result as $row) {
			              $cartUpdate = "call slp_enrollment_del('".$row->id."', 'lrn_crs_reg_rsc','".$lnrUserId."')";
			          	  expDebug::dPrint("Enroll order update to reseved cancel in cart module when User update cart: ".$cartUpdate,4);
			              db_query($cartUpdate);
			          	}
				}else{
				    $cartUpdate = "call slp_enrollment_del('".$id."', 'lrn_crs_reg_rsc','".$lnrUserId."')";
					expDebug::dPrint("Enroll order update in cart module when User update cart: ".$cartUpdate,4);
					db_query($cartUpdate);
				}
				
				$result = 'success';
			
			}
			elseif (empty($classid))
			{
				if(!isset($_REQUEST['mobile_request'])){
				$delete = db_delete('uc_cart_products');
				$delete->condition('cart_id',$uid,'=');
				$delete->execute();
				//update slt_enrollment

				}
				foreach($sltOrderId as $key => $val)
				{
					$slt_orderid = $val->orderid;

						
				$updateEnr = db_update('slt_master_enrollment');
		        $updateEnr->fields(array(
		                              'overall_status'=> 'lrn_tpm_ovr_rsc',
		                              'comp_date'		=> NULL,
		                              'updated_by'    => $lnrUserId,
		        					  'updated_on'=>now(),
		                              ));
		          $updateEnr->condition('order_id',$slt_orderid);

		          
		          $updateEnr->execute();

		          

				$select = db_select('slt_enrollment','se');
			    $select->addField('se','id');
				$select->condition('se.order_id',$slt_orderid);
				expDebug::dPrintDBAPI('$select',$select);
			    $result = $select->execute()->fetchAll();
			    foreach($result as $key => $val)
			    	{
	     				 $id = $val->id;
	     				 $cartUpdate = "call slp_enrollment_del('".$id."', 'lrn_crs_reg_rsc','".$lnrUserId."')";
							expDebug::dPrint("Enroll order update in cart module when User update cart: ".$cartUpdate,4);
						db_query($cartUpdate);
	    			}
			}
			$result = 'success';
			}
		
		else
		{
			
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Cart cannot be modified now as checkout is in progress. If required, remove all items from cart and try again.";
				return $errobj;
			
		}
			/*if(!empty($sltOrderId))
				db_query("update slt_order set order_status='cme_pmt_sts_rcl' where id = '$sltOrderId'");
*/

syncSolrData('MasterEnrollment');
				//$result->status = 'success';
				return $result;
	}
	catch (Exception $ex) {
		watchdog_exception('deleteItemsFromCartAPI', $ex);
		expertusErrorThrow($ex);
	}
}


function calculateDiscountsAPI($user_id, $userid, $orderid,$discount_code){
	try{
		include_once ($_SERVER ["DOCUMENT_ROOT"] . '/sites/all/modules/core/exp_sp_commerce/modules/ubercart/contrib/uc_discounts_alt/uc_discounts/uc_discounts.module');

		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		include_once($_SERVER["DOCUMENT_ROOT"]. 'sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc');
		
	//Below function will set currency details in $_REQUEST
		$currencyDetails = splitCurrencyInputAndSetInRequest();
		
		if(empty($currencyDetails))
		{
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->field = 'currency_type';
		$errobj->errormsg = "The given Currency type is invalid or inactive status";
		return $errobj;
		}
		expDebug::dPrint('currency_type = ' .$_REQUEST['currency_type']);
		expDebug::dPrint('currency_sym  = ' .$_REQUEST['currency_sym']);
		
		if ($_REQUEST['currency_type'] && $_REQUEST['currency_sym']) {
			$_SESSION['shop_cart_currency_code'] = $_REQUEST['currency_type'];
			$_SESSION['user_preferred_currency_code'] = $_REQUEST['currency_type'];
			$_SESSION['user_preferred_currency_sym'] = $_REQUEST['currency_sym'];
		} else if ($_REQUEST['currency_type']) {
			$getDefaultCurrency = getDefaultCurrency();
			$UserPrefCurrCode = $getDefaultCurrency['preferred_currency'];
			$UserPrefCurrSym = $getDefaultCurrency['attr2'];

			$_SESSION['shop_cart_currency_code'] = $UserPrefCurrCode;
			$_SESSION['user_preferred_currency_code'] = $UserPrefCurrCode;
			$_SESSION['user_preferred_currency_sym'] = $UserPrefCurrSym;
		} else {
			$getDefaultCurrency = getDefaultCurrency();
			$UserPrefCurrCode = $getDefaultCurrency['preferred_currency'];
			$UserPrefCurrSym = $getDefaultCurrency['attr2'];

			$_SESSION['shop_cart_currency_code'] = $UserPrefCurrCode;
			$_SESSION['user_preferred_currency_code'] = $UserPrefCurrCode;
			$_SESSION['user_preferred_currency_sym'] = $UserPrefCurrSym;
		}


// 		$discount_code = stripslashes( $discount_code );
		
	$discount_details = 	uc_discounts_js_calculate( $userid, $user_id, 'api', $orderid, $discount_code, false );
	if ($discount_details['total_discount']>0){
		$discount_report->status=  'success';
		$discount_report->discount_value  = $discount_details['total_discount'];
		$discount_report->new_order_total = $discount_details['total'];
	}
	
	
	if($discount_details['total']==0)
	{
		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_commerce/modules/ubercart/uc_cart/uc_cart.pages.inc');
		$orderComplete=purchaseClassAPI($user_id,$userid,$orderid,'Zero Cost','','','','','','');
		if($orderComplete->status=='success')
		{
			$discount_report->order_status='completed';
			$discount_report->comments='Your Order has been completed, because the order total amount is Zero Cost';
			expDebug::dPrint('SUBU2'.print_r($discount_details,true),5);
				
		}
	}
	
	$discount_report-> orderid = $discount_details['orderid'];
	$discount_report-> errors = $discount_details['errors'];
	$discount_report-> messages = $discount_details['messages'];
	return 	$discount_report;
	
	

}
	catch (Exception $ex) {
		watchdog_exception('calculateDiscountsAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function calculateTaxAPI($user_id, $userid, $orderid,$sltOrderId){
try{
	include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_commerce/modules/ubercart/contrib/uc_discounts_alt/uc_discounts/uc_discounts.module');
	global $user;
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	include_once($_SERVER["DOCUMENT_ROOT"]. 'sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc');
	
//Below function will set currency details in $_REQUEST
		$currencyDetails = splitCurrencyInputAndSetInRequest();
		
		if(empty($currencyDetails))
		{
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->field = 'currency_type';
		$errobj->errormsg = "The given Currency type is invalid or inactive status";
		return $errobj;
		}
	expDebug::dPrint('currency_type = ' .$_REQUEST['currency_type']);
	expDebug::dPrint('currency_sym  = ' .$_REQUEST['currency_sym']);

	if ($_REQUEST['currency_type'] && $_REQUEST['currency_sym']) {
		$_SESSION['shop_cart_currency_code'] = $_REQUEST['currency_type'];
		$_SESSION['user_preferred_currency_code'] = $_REQUEST['currency_type'];
		$_SESSION['user_preferred_currency_sym'] = $_REQUEST['currency_sym'];
	} else if ($_REQUEST['currency_type']) {
		$getDefaultCurrency = getDefaultCurrency();
		$UserPrefCurrCode = $getDefaultCurrency['preferred_currency'];
		$UserPrefCurrSym = $getDefaultCurrency['attr2'];

		$_SESSION['shop_cart_currency_code'] = $UserPrefCurrCode;
		$_SESSION['user_preferred_currency_code'] = $UserPrefCurrCode;
		$_SESSION['user_preferred_currency_sym'] = $UserPrefCurrSym;
	} else {
		$getDefaultCurrency = getDefaultCurrency();
		$UserPrefCurrCode = $getDefaultCurrency['preferred_currency'];
		$UserPrefCurrSym = $getDefaultCurrency['attr2'];

		$_SESSION['shop_cart_currency_code'] = $UserPrefCurrCode;
		$_SESSION['user_preferred_currency_code'] = $UserPrefCurrCode;
		$_SESSION['user_preferred_currency_sym'] = $UserPrefCurrSym;
	}

	$_SESSION['slt_order_id'] = $sltOrderId;
	expDebug::dPrint(" $orderid $orderid $orderid". $orderid,5);
 	 drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	$uid = getDrupalIdOfUser($user_id);
 	 $user->uid = $uid;
 	  $order = uc_order_load($orderid);
 	  db_delete('uc_order_products')
    	->condition('order_id', $order->order_id)
    	->execute();
  		 $order->products = uc_cart_get_contents($user->uid,'rebuild');
  		 $order->order_total = uc_order_get_total($order, TRUE);
 		 $order->line_items = uc_order_load_line_items($order, TRUE);
 		 $order->line_items = array_merge($order->line_items, uc_order_load_line_items($order, FALSE));
  		 usort($order->line_items, 'uc_weight_sort');
 		 uc_order_save($order);
 		 
 		 $query = "select sum(acc.tax) as tax from
 		 (select tax_amount as tax from slt_order_items
 		 		where order_id = '".$sltOrderId."' and program_id IS NOT NULL
 		 		group by program_id
 		 		UNION ALL
 		 		select tax_amount as tax from slt_order_items
 		 		where order_id = '".$sltOrderId."' and program_id IS NULL ) as acc";
 		 $select = db_query($query);
 		 $tax = $select->fetchField();
 		 expDebug::dPrint("taxtaxtaxtaxtax: ".$tax,4);
 		 $result->tax = $tax;
		 return $result;
	}
catch (Exception $ex) {
	watchdog_exception('calculateTaxAPI', $ex);
	expertusErrorThrow($ex);
	}
}


function getTPDetailsAPI($user_id, $userid, $programID,$object_type){
try{
	
	
    drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	include_once($_SERVER["DOCUMENT_ROOT"]. 'sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc');
	
	
	if($_REQUEST['action'] == "gethistory"){
		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrlearningplan/exp_sp_lnrlearningplan.inc');
		$modulelist = getCertificationModules($programID,1);
		expDebug::dPrint('modulelist'.print_r($modulelist,true));
		$result->modulelist = $modulelist;
		return $result;
	}
	
	
	//$node_id = getNodeIdResults($programID,$object_type);
	$isCart=0;
	$isAdminSide='N';
	//expDebug::dPrint('$node_id$node_id$node_id'.$node_id, 4);
	//Modified code for ticket #0046830
	//$result->course_data = lp_fetch_module_list($programID,$user_id,'N',1);
	$fetchresult = lp_fetch_module_list($programID,$user_id,'N','',1);
	expDebug::dPrint(' $fetchresult '. print_r($fetchresult,true),5);
	$result->course_data->module_details = $fetchresult[0]['module_details'];
	$result->course_data->course_details = $fetchresult[0]['course_details'];
	//$result = lp_singleclass_or_multiclass_cnt_for_course($programID,$node_id,0,'N',$userListIds = '','catalogpopup-N');
	expDebug::dPrint('$result$result$result$result$result'.print_r($result,true),5);
	return $result;


	}
catch (Exception $ex) {
	watchdog_exception('getTPDetailsAPI', $ex);
	expertusErrorThrow($ex);
	}
}

function getTPCourseClassDetailsAPI($user_id, $userid, $courseID,$object_type){
try{
    drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
    include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrlearningplan/exp_sp_lnrlearningplan.inc');
	include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc');
	include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_survey/exp_sp_survey_learner/exp_sp_surveylearner.inc');
	include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_learning_plan_detail/exp_sp_learning_plan_detail.inc');
	if($_REQUEST['action'] == "details"){
		if(isset($_REQUEST['mobile_request'])){
		$master_enrollment_id = $_REQUEST['master_enrollment_id'];
		$recertify_path = $_REQUEST['recertify_path'];
		$overall_status = $_REQUEST['overall_status'];

		$result->enrolled_classes = getLPClasses($master_enrollment_id,$courseID,$recertify_path,$object_type,$overall_status,$user_id,1);
		expDebug::dPrint('enrolled_classesenrolled_classesenrolled_classesenrolled_classes'.print_r($result->enrolled_classes,true),5);
		for($i=0;$i<count($result->enrolled_classes);$i++){
			$result->enrolled_classes[$i]['pre_assessment'] = getAssignedSurveys ( $userid, $result->enrolled_classes[$i]['class_id'], 'cre_sys_obt_cls', 'assessment', 1, $result->enrolled_classes[$i]['id'], 'yes' );
			$result->enrolled_classes[$i]['post_assessment'] = getAssignedSurveys ( $userid, $result->enrolled_classes[$i]['class_id'], 'cre_sys_obt_cls', 'assessment', 0, $result->enrolled_classes[$i]['id'], 'yes' );
			$result->enrolled_classes[$i]['survey'] = getAssignedSurveys ( $userid, $result->enrolled_classes[$i]['class_id'], 'cre_sys_obt_cls', 'survey', '1', $result->enrolled_classes[$i]['id'], 'yes' );
			
			//$result->enrolled_classes[$i]['pre_assessment'][0]->progress = 80;
			if($result->enrolled_classes[$i]['pre_assessment'][0]->surveyid){
				$class_pre_progress = db_query("select progress from slt_attendance_summary where assessment_id = ".$result->enrolled_classes[$i]['pre_assessment'][0]->surveyid." and enrollment_id = ".$result->enrolled_classes[$i]['id']);
				$progress_pre = $class_pre_progress->fetchAll();
				$result->enrolled_classes[$i]['pre_assessment'][0]->progress = $progress_pre[0]->progress;
			}
			if($result->enrolled_classes[$i]['post_assessment'][0]->surveyid){
				$class_post_progress = db_query("select progress from slt_attendance_summary where assessment_id = ".$result->enrolled_classes[$i]['post_assessment'][0]->surveyid." and enrollment_id = ".$result->enrolled_classes[$i]['id']);
				$progress_post = $class_post_progress->fetchAll();
				$result->enrolled_classes[$i]['post_assessment'][0]->progress = $progress_post[0]->progress;
			}
			if($result->enrolled_classes[$i]['survey'][0]->surveyid){
				for($s=0;$s<count($result->enrolled_classes[$i]['survey']);$s++){
					$surveyId = $result->enrolled_classes[$i]['survey'][$s]->surveyid;
					$class_survey_progress = db_query("select progress from slt_attendance_summary where assessment_id = ".$surveyId." and enrollment_id = ".$result->enrolled_classes[$i]['id']);
					$progress_survey = $class_survey_progress->fetchAll();
					$result->enrolled_classes[$i]['survey'][$s]->progress = $progress_survey[0]->progress;
				}
			}
			
			expDebug::dPrint('dededfrrccc33334444444445555'.print_r($result->enrolled_classes[$i]['delivery_type'],true),5);
			
			if($result->enrolled_classes[$i]['delivery_type'] == 'Classroom' || $result->enrolled_classes[$i]['delivery_type'] == 'Virtual Class'){
				$session_detail = getClassSessionsDateTime($result->enrolled_classes[$i].['class_id']);
				$sccs_date_and_time = implode(' ', array($session_detail[0][date],$session_detail[0][end_time]));
				$curr_date_and_time =  date('Y-m-d H:i',strtotime("-30 minutes"));
				
				
				
				if (strtotime($curr_date_and_time) >= strtotime($sccs_date_and_time)) {
					$result->enrolled_classes[$i]['ilt_sessionCheck'] = 'Session_completed';
					expDebug::dPrint('Session_completed');
				}else{
					$result->enrolled_classes[$i]['ilt_sessionCheck'] = 'Session_not_completed';
					expDebug::dPrint('Session_not_completed');
				}
				
				//expDebug::dPrint('dededfrrccc3333444444444'.print_r($result->enrolled_classes[$i]->delivery_type,true),5);
			
			}
			
			
			expDebug::dPrint('$result$result$result$result$result1111'.print_r($result,true),5);
		}
		$tp_courses = lp_fetch_module_list($courseID,$user_id,'N','',1);
		$result->tp_courses = $tp_courses;
		$result->pre_assessment = getAssignedSurveys ( $userid, $courseID, $object_type, 'assessment', 1, $master_enrollment_id, 'yes' );
		$result->post_assessment = getAssignedSurveys ( $userid, $courseID, $object_type, 'assessment', 0, $master_enrollment_id, 'yes' );
		$result->survey = getAssignedSurveys ( $userid, $courseID, $object_type, 'survey', '1', $master_enrollment_id, 'yes' );
		
		$class_cnt = count($result->enrolled_classes) - 1;
		
		$last_enrollId = $result->enrolled_classes[$class_cnt]['id'];
		
		
		if($result->pre_assessment[0]->surveyid){
			$preassessmentId = $result->pre_assessment[0]->surveyid;
			$Tp_pre_progress = db_query("select progress from slt_attendance_summary where assessment_id = ".$preassessmentId." and enrollment_id = ".$last_enrollId);
			$progress_pre = $Tp_pre_progress->fetchAll();
			$result->pre_assessment[0]->progress = $progress_pre[0]->progress;
		
		}
		if($result->post_assessment[0]->surveyid){
			$postassessmentId = $result->post_assessment[0]->surveyid;
			$Tp_post_progress = db_query("select progress from slt_attendance_summary where assessment_id = ".$postassessmentId." and enrollment_id = ".$last_enrollId);
			$progress_post = $Tp_post_progress->fetchAll();
			$result->post_assessment[0]->progress = $progress_post[0]->progress;
		
		}
		
		if($result->survey[0]->surveyid){
			for($s=0;$s<count($result->survey);$s++){
					
				$surveyId = $result->survey[$s]->surveyid;
					
				$Tp_survey_progress = db_query("select progress from slt_attendance_summary where assessment_id = ".$surveyId." and enrollment_id = ".$last_enrollId);
				expDebug::dPrintDBAPI("queryashutoshmishra12343 ", $Tp_survey_progress);
				$progress_survey = $Tp_survey_progress->fetchAll();
				$result->survey[$s]->progress = $progress_survey[0]->progress;
					
			}
		}
		
		$user_detail=db_query("select overall_progress from slt_master_enrollment where id =".$master_enrollment_id);
		expDebug::dPrintDBAPI("query12345rchangese3edede222 ", $user_detail);
		$userDetail = $user_detail->fetchAll();
		$result->percentage_completion = $userDetail[0]->overall_progress;
		
		
		return $result;
		}
	}else{
	 $result->class_list = attachedCourseClassDetailsAPI($courseID);
	 $enrolled_id = getObjectRegisteredOrNot($courseID,$user_id );
	 $master_enrollment_id = $enrolled_id->master_enrolled_id;
	 $module_id = getModuleId($user_id,$courseID );	 
	 $tp_courses = lp_fetch_module_list($courseID,$user_id,'N','',1);
	 $result->tp_courses = $tp_courses;
	 $result->details = getLearningDetails($courseID);
	 $result->details['module_id'] =  $module_id;
	 if(!empty($master_enrollment_id)){
	 $result->enrolled_classes = getLPClasses($master_enrollment_id,$courseID,'',$object_type,$_REQUEST['overall_status'],$user_id,1);
	 }
	 else{
	 $result->enrolled_classes='';
	 }
	 for($i=0;$i<count($result->class_list);$i++){
		// expDebug::dPrint('$result$result$result$result$result'.print_r($result->class_list[$i]->class_id,true),4);
		 $enrolledid= getRegisteredOrNot($result->class_list[$i]->course_id,$result->class_list[$i]->class_id,$user_id,'');
		 $result->class_list[$i]->enrolledid=$enrolledid;
		// expDebug::dPrint('$result$result$result$result$result'.print_r($enrolledid,true),4);
		 }
	 expDebug::dPrint('$result$result$result$result$result'.print_r($result,true),5);
	 return $result;
	}
	}
catch (Exception $ex) {
	watchdog_exception('getTPCourseClassDetailsAPI', $ex);
	expertusErrorThrow($ex);
	}
}

function addTPToCartAPI($user_id,$tpId,$classIds,$object_type,$regType){
	try{
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc');
		/* global $user;
		$uid = getDrupalIdOfUser($user_id);
 	    $user->uid = $uid; */
		$nodeId = getNodeIdResults($tpId,$object_type);
		  //$_SERVER['HTTP_REFERER'] = 'addClassToCartAPI';
		$_SESSION['uc_referer_uri'] = 'addTPToCartAPI';
	      //	$res = callAddToCartTP('add',$nodeId,$tpId,$courseId,$classIds,'api',$user_id);

		expDebug::dPrint('MultiRegister call add  '.$regType, 4);
		if($regType == 'MultiRegister'){
			expDebug::dPrint('MultiRegister call add  ', 4);
		    $res = callAddToCartTP('add',$nodeId,$tpId,$courseId,$classIds,'api',$user_id);

		    expDebug::dPrint('MultiRegister call add  final'.$res, 4);
		     return array((object)array('status'=>$res));
		}
		else{
			$res=lp_singleclass_or_multiclass_cnt_for_course($tpId,$nodeId,true,'N',$user_id,'',false,true,$regDate ='');
			if($res =='singleRegister'){
			$res = callAddToCartTP('add',$nodeId,$objectId,'',$classIds,'api',$user_id);
			}

			expDebug::dPrint('MultiRegister  '.$res, 4);
			$classIdArr = attachedCourseClassDetailsAPI($tpId,$user_id);
			for($i=0;$i<count($classIdArr);$i++){
		                         expDebug::dPrint('$result$result$result$result$result'.print_r($classIdArr[$i]->class_id,true),4);
							     $enrolledid= getRegisteredOrNot($classIdArr[$i]->course_id,$classIdArr[$i]->class_id,$userIdList,'');
							     $classIdArr[$i]->enrolledid=$enrolledid;
		  }
		         expDebug::dPrint('$result$result$result$result$result'.print_r($classIdArr,true),5);

		         $tp_courses = lp_fetch_module_list($tpId, $user_id,'N','',1);
		         return array((object)array('class_list'=>$classIdArr,'status'=>$res,'tp_courses'=> $tp_courses));
	}

		return array((object)array('status' => $res));
	}catch (Exception $ex) {
		watchdog_exception('addTPToCartAPI', $ex);
		expertusErrorThrow($ex);
	}
}


function setPushNotificationServiceAPI() {
	try{

		// 		$name =  $_REQUEST['full_name'];
		// 		$email = $_REQUEST['email_id'];
		// 		$gcm_regid = $_REQUEST['reg_id'];

		// 		$result = mysql_query("INSERT INTO gcm_users(name, email, gcm_regid, created_at) VALUES('$name', '$email', '$gcm_regid', NOW())");

		expDebug::dPrint('innnnnnnnn ');

		$query = db_merge('slt_mobiledevice_tokens');

	$query->key(array(
			'gcm_regid' => $_REQUEST['reg_id'],
	));
	$query->fields(array(
			'name' => $_REQUEST['full_name'],
				'email' => $_REQUEST['email_id'],
				'user_id' => $_REQUEST['user_id'],
				'gcm_regid' => $_REQUEST['reg_id'],
	));

	expDebug::dPrintDBAPI('Manage notification -->>',$query);
		$result = $query->execute();

		$result-> status ='success';
		return $result;

	}
	catch (Exception $ex) {
		watchdog_exception('setPushNotificationServiceAPI', $ex);
		expertusErrorThrow($ex);
	}
}


function configurePushNotificationDetailsAPI()
{
	try{
		if($_REQUEST['action'] == 'get')
		{
			$select = db_select('slt_manage_pushnotification','notif');
			$select->addField('notif','weekend', 'weekend');
			$select->addField('notif','certification', 'certification');
			$select->addField('notif','mylearning', 'mylearning');
			$select->addField('notif','myclasses', 'myclasses');
			$select->addField('notif','myteam', 'myteam');
			$select->addField('notif','receive_notification', 'receive_notification');
			$select->addField('notif','from_time', 'from_time');
			$select->addField('notif','to_time', 'to_time');
			$select->condition('notif.user_id',$_REQUEST['userid']);
			expDebug::dPrintDBAPI('narrow search sql query-->>',$select);
			$result = $select->execute()->fetchAll();

			expDebug::dPrint(' Resultttttt = ' . print_r($result, true),4);
			return $result;
		}
		else
		{
	$query = db_merge('slt_manage_pushnotification');
	$query->key(array(
			'user_id' => $_REQUEST['userid'],
	));
	$query->fields(array(
			'from_time' => $_REQUEST['from_time'],
			'to_time' => $_REQUEST['to_time'],
			'weekend' => $_REQUEST['weekend'],
			'certification' => $_REQUEST['certification'],
			'mylearning' => $_REQUEST['mylearning'],
			'myclasses' => $_REQUEST['myclasses'],
			'myteam' => $_REQUEST['myteam'],
			'receive_notification' => $_REQUEST['receive_notification'],
	));

	expDebug::dPrintDBAPI('Manage notification -->>',$query);
	$result = $query->execute();
	return $result->result = "success";
		}
	}
	catch (Exception $ex) {
		watchdog_exception('configurePushNotificationDetailsAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function getPushNotificationDetailsAPI($userId) {
	try{
		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_manage/exp_sp_administration_announcement/exp_sp_administration_announcement.inc');
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

		if($_REQUEST['action'] == 'get')
		{
				
			if($_REQUEST['notification_id'])
			{
				$new_Notiid = $_REQUEST['notification_id'];
				$check_Noti_table = substr($new_Notiid, -1);

				$new_notiId = '';
				$noti_type = '';
				if($check_Noti_table == 1){
		$select = db_select('slt_notification','notif');
					$new_notiId = substr($_REQUEST['notification_id'], 0, -1);
					$noti_type = 'normal';
				}else if($check_Noti_table == 0){
					$select = db_select('slt_bulk_notification','notif');
					$new_notiId = substr($_REQUEST['notification_id'], 0, -1);
					$noti_type = 'bulk';
				}
					
					
				$select->condition('notif.id',$new_notiId, '=');
					

		$select->leftJoin('slt_notification_info', 'info', 'notif.msg_id = info.notification_code');
		$select->leftJoin('slt_notification_frame', 'frame', 'info.id = frame.notification_id');
		$select->addField('notif','token_str', 'token_str');
		$select->addField('notif','detail_value', 'action_detail');
		$select->addField('frame', 'notification_template', 'notification_template');
		$select->addField('frame','notification_subject', 'notification_subject');
		$select->addField('notif','msg_id', 'msg_id');
		$select->addField('notif','msg_type', 'msg_type');
		$select->addField('notif','id', 'notification_id');
		$select->addField('notif','viewed', 'viewed');
		$select->addField('notif','soft_delete', 'soft_delete');
		$select->addField('info','notification_title', 'notification_title');
		$select->addField('notif','created_on', 'updated_on');
				$select->addExpression("'$noti_type'", 'noti_type');
		//$select->addField('',"\'now()\'",'db_time');
		$select->condition('notif.mobile',1,'=');
		$select->condition('notif.user_id',$userId);
		$select->condition('notif.soft_delete',1,'<>');
		$select->condition('frame.lang_code','cre_sys_lng_eng', '=');


		//$select->orderBy('notif.created_on','desc');
		$select->orderBy('viewed', 'ASC');
		$select->orderBy('updated_on', 'DESC');

			}else{


				$select1 = db_select('slt_bulk_notification','notif');
				$select1->leftJoin('slt_notification_info', 'info', 'notif.msg_id = info.notification_code');
				$select1->leftJoin('slt_notification_frame', 'frame', 'info.id = frame.notification_id');
			    $select1->addField('frame','notification_subject', 'notification_subject');
				$select1->addField('notif','msg_id', 'msg_id');
				$select1->addField('notif','created_on', 'updated_on');
				$select1->addField('notif','id', 'notification_id');
				$select1->addField('info','notification_title', 'notification_title');
				$select1->addField('notif','detail_value', 'action_detail');
		        $select1->addField('notif','token_str', 'token_str');
				$select1->addField('notif','soft_delete', 'soft_delete');
				$select1->addField('frame', 'notification_template', 'notification_template');
				$select1->addField('notif','msg_type', 'msg_type');
				$select1->addField('notif','viewed', 'viewed');	
				$select1->addExpression('\'notification\'', 'type');
				$select1->addExpression("'bulk'", 'noti_type');	
				//$select->addField('',"\'now()\'",'db_time');
				$select1->condition('notif.mobile',1,'=');
				$select1->condition('notif.user_id',$userId);
				$select1->condition('notif.soft_delete',1,'<>');
				$select1->condition('frame.lang_code','cre_sys_lng_eng', '=');
				
				$select2 = db_select('slt_notification','notif');
				$select2->leftJoin('slt_notification_info', 'info', 'notif.msg_id = info.notification_code');
				$select2->leftJoin('slt_notification_frame', 'frame', 'info.id = frame.notification_id');
			    $select2->addField('frame','notification_subject', 'notification_subject');
				$select2->addField('notif','msg_id', 'msg_id');
				$select2->addField('notif','created_on', 'updated_on');
				$select2->addField('notif','id', 'notification_id');
				$select2->addField('info','notification_title', 'notification_title');
				$select2->addField('notif','detail_value', 'action_detail');
		        $select2->addField('notif','token_str', 'token_str');
				$select2->addField('notif','soft_delete', 'soft_delete');
				$select2->addField('frame', 'notification_template', 'notification_template');
				$select2->addField('notif','msg_type', 'msg_type');
				$select2->addField('notif','viewed', 'viewed');	
				$select2->addExpression('\'notification\'', 'type');
				$select2->addExpression("'normal'", 'noti_type');	
				//$select->addField('',"\'now()\'",'db_time');
				$select2->condition('notif.mobile',1,'=');
				$select2->condition('notif.user_id',$userId);
				$select2->condition('notif.soft_delete',1,'<>');
				$select2->condition('frame.lang_code','cre_sys_lng_eng', '=');
				
				
				
				
				
					
				// $select1 = db_select('slt_bulk_notification','notif');
				// $select1->leftJoin('slt_notification_info', 'info', 'notif.msg_id = info.notification_code');
				// $select1->leftJoin('slt_notification_frame', 'frame', 'info.id = frame.notification_id');
				// $select1->addField('notif','token_str', 'token_str');
				// $select1->addField('notif','detail_value', 'action_detail');
				// $select1->addField('frame', 'notification_template', 'notification_template');
				// $select1->addField('frame','notification_subject', 'notification_subject');
				// $select1->addField('notif','msg_id', 'msg_id');
				// $select1->addField('notif','msg_type', 'msg_type');
				// $select1->addField('notif','id', 'notification_id');
				// $select1->addField('notif','viewed', 'viewed');
				// $select1->addField('notif','soft_delete', 'soft_delete');
				// $select1->addField('info','notification_title', 'notification_title');
				// $select1->addField('notif','created_on', 'updated_on');
				// $select1->addExpression("'bulk'", 'noti_type');
				//$select1->addField('',"\'now()\'",'db_time');
				// $select1->condition('notif.mobile',1,'=');
				// $select1->condition('notif.user_id',$userId);
				// $select1->condition('notif.soft_delete',1,'<>');
				// $select1->condition('frame.lang_code','cre_sys_lng_eng', '=');
					
				// $select2 = db_select('slt_notification','notif');
				// $select2->leftJoin('slt_notification_info', 'info', 'notif.msg_id = info.notification_code');
				// $select2->leftJoin('slt_notification_frame', 'frame', 'info.id = frame.notification_id');
				// $select2->addField('notif','token_str', 'token_str');
				// $select2->addField('notif','detail_value', 'action_detail');
				// $select2->addField('frame', 'notification_template', 'notification_template');
				// $select2->addField('frame','notification_subject', 'notification_subject');
				// $select2->addField('notif','msg_id', 'msg_id');
				// $select2->addField('notif','msg_type', 'msg_type');
				// $select2->addField('notif','id', 'notification_id');
				// $select2->addField('notif','viewed', 'viewed');
				// $select2->addField('notif','soft_delete', 'soft_delete');
				// $select2->addField('info','notification_title', 'notification_title');
				// $select2->addField('notif','created_on', 'updated_on');
				// $select2->addExpression("'normal'", 'noti_type');
				// //$select2->addField('',"\'now()\'",'db_time');
				// $select2->condition('notif.mobile',1,'=');
				// $select2->condition('notif.user_id',$userId);
				// $select2->condition('notif.soft_delete',1,'<>');
				// $select2->condition('frame.lang_code','cre_sys_lng_eng', '=');
					
					
				$selectunion = $select2->union($select1,'UNION ALL');

			}
			
			
			if ($_REQUEST ['mobile_version'] && $_REQUEST ['type']) {
				$announcementquery = getAnnouncementResults ( $userId );
				$notificationQuery = $selectunion;
				
				expDebug::dPrintDBAPI('Push notification details3332 = ',$notificationQuery);
				
				
				$page = $_REQUEST ['page'];
				$limit = $_REQUEST ['limit'];
				$start = $limit * $page - $limit;
				
				$select_final =$announcementquery->union ( $notificationQuery, 'UNION ALL' );
								expDebug::dPrintDBAPI('1212132131eeeee ',$select_final);
// 				
				// $select_final->orderBy ( 'updated_on', 'DESC' );
				// $select_final->range ( $start, $limit );
				
				// $select->orderBy('notif.created_on','desc');
				// $final->orderBy('viewed', 'ASC');
				// $final->orderBy('updated_on', 'DESC');
				
				$select= db_select($select_final,'notif');
				$select->fields('notif');
				$select->orderBy('viewed', 'ASC');
				$select->orderBy('updated_on', 'DESC');
				$select->range ( $start, $limit );
				expDebug::dPrintDBAPI('Push notification details query112222 = ',$select);
				
				
				$result = $select->execute()->fetchAll();
				
				expDebug::dPrint('rffrvvvvgfgtt45555555'.print_r($result, true) , 4);
				
				
				// $result = $announcementquery->execute ()->fetchAll ();
			}else{
				
				$page = $_REQUEST ['page'];
				$limit = $_REQUEST ['limit'];
				$start = $limit * $page - $limit;
				if(!$_REQUEST['notification_id'])
				{
					$select= db_select($selectunion,'notif');
					$select->fields('notif');
					$select->orderBy('viewed', 'ASC');
					$select->orderBy('updated_on', 'DESC');
				}
				expDebug::dPrintDBAPI('Push notification details query11 = ',$select);
				$select->range ( $start, $limit );
				
				$result = $select->execute()->fetchAll();
				//expDebug::dPrint('rffrvvvvgfgtt45555555'.print_r($result, true) , 4);
				
			}
		

		// expDebug::dPrintDBAPI('Push notification details query = ',$select);
		// $result = $select->execute()->fetchAll();
		
		
		
		
		
		if(count($result) > 0)
		{
		  include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc');


		$selectTime = db_select('slt_person','per');
	$selectTime ->leftJoin('slt_profile_list_items', 'item', 'per.time_zone = item.code');
	$selectTime->addField('item','attr2', 'time_zone');
	$selectTime->condition('per.id',$userId);
	expDebug::dPrintDBAPI('Timezone query = ',$selectTime);
	$time_zone = $selectTime->execute()->fetchField();
	expDebug::dPrint('Timezone = '.$time_zone);

		  for($i=0;$i<count($result);$i++)
		  {
		  	// 		  	expDebug::dprint(' displayNotificationDetailsAPI $output value  = '.$result[$i],4);

		  	$date = new DateTime($result[$i]->updated_on, new DateTimeZone($time_zone));

		  	$result[$i]->updated_on = $date->format('Y-m-d H:i:s');
		  	$output = base64_encode(generateNotificationDetailsFromToken($result[$i]->token_str,$result[$i]->notification_template));
		  	$result[$i]->notification_template = $output;
		  	expDebug::dprint(' displayNotificationDetailsAPI $output = '.$output,4);

		  	expDebug::dprint(' displayNotificationDetailsAPI2323232323 = '.$result[$i]->msg_id ,4);
		  	 

		  	if($result[$i]->msg_id == "cert_curr_lp_expire_reminder")
		  	{
		  		$list = explode('~|', $result[$i]->action_detail);

		  		$arr = explode(">|", $list[0]);

		  		expDebug::dPrint(' action detail valueeee = ' . print_r($arr[1], true),4);

		  		$selectRecertify = db_select('slt_master_enrollment','master');
		  		$selectRecertify->addField('master','recertify_path', 'recertify_path');
		  		$selectRecertify->condition('master.user_id',$userId);
		  		$selectRecertify->condition('master.program_id',$arr[1]);
		  		$selectRecertify->condition('master.recertify_path',1);
		  		$selectRecertify->groupby('master.program_id');

		  		expDebug::dPrintDBAPI('Recertify query = ',$selectRecertify);
		  		$Recertify = $selectRecertify-> execute() ->fetchField();
		  		expDebug::dprint('Recertify output = '.$Recertify);
		  	}else if($result[$i]->msg_id == "open_survey_mobile"){
		  		$list = explode('~|', $result[$i]->action_detail);
		  		$arr = explode(">|", $list[0]);
		  		expDebug::dPrint(' action detail valueeee = ' . print_r($arr, true),4);
		  		$classInfo = getClassInfoForNotifications($arr[1]);
		  		$exists = enrollmentExistsCheck($userId, $arr[1], 'Class');
		  		
		  	}
		  	else if ($result[$i]->msg_id == "compliance_expired"){
		  		$list = explode('~|', $result[$i]->token_str);
		  		$arr = explode(">|", $list[2]);
		  		$classInfo=['delivery_type_name'=>$arr[1]];


		  	}else if($result[$i]->msg_id == "compliance_expiry_remainder"){
		  		$list = explode('~|', $result[$i]->token_str);
		  		$arr = explode(">|", $list[2]);
		  		$classInfo=['delivery_type_name'=>$arr[1]];

		  	}
		  	else
		  	{
		  		$list = explode('~|', $result[$i]->action_detail);
		  		$arr = explode(">|", $list[1]);
		  		$classInfo = getClassInfoForNotifications($arr[1]);
		  		$exists = enrollmentExistsCheck($userId, $arr[1], 'Class');
		  	}

		  	$result[$i]->recertify_path = $Recertify;
		  	$result[$i]->enrolled_status = $exists;

		  	$subject = getNotificationSubject($result[$i]->token_str,$result[$i]->notification_subject);
		  	$result[$i]->notification_subject = $subject;
		  	$result[$i]->cls_status = $classInfo['status'];
		  	$result[$i]->delivery_type_name = $classInfo['delivery_type_name'];


		  	expDebug::dprint(' 545665tytyt454545 = ' .$result[$i]->noti_type,4);
		  	 
		  	if($result[$i]->noti_type == 'normal'){
		  		$result[$i]->notification_id = $result[$i]->notification_id . 1;
		  	}else if($result[$i]->noti_type == 'bulk'){
		  		$result[$i]->notification_id = $result[$i]->notification_id . 0;
		  	}
		  	 
		  	expDebug::dprint(' displayNotificationDetailsAPI subject = ' .$subject,4);

		  }

		  	
		  $select2 = db_select('slt_notification', 'notif');
		  $select2->addField('notif', 'id', 'id');
		  //   $select->addExpression('COUNT(notif.id)');
		  $select2->condition('notif.user_id', $userId);
		  $select2->condition('notif.mobile', 1);
		  $select2->condition('notif.viewed', 1 , '<>');
		  $select2->condition('notif.soft_delete', 1,'<>');
		  	
		  $select1 = db_select('slt_bulk_notification', 'notif');
		  $select1->addField('notif', 'id', 'id');
		  //   $select->addExpression('COUNT(notif.id)');
		  $select1->condition('notif.user_id', $userId);
		  $select1->condition('notif.mobile', 1);
		  $select1->condition('notif.viewed', 1 , '<>');
		  $select1->condition('notif.soft_delete', 1,'<>');
		  $selectunion = $select1->union($select2,'UNION ALL');
		  $select = db_select($selectunion,'noti');
		  $select->addExpression('COUNT(noti.id)');

		  	
				$notViewedNotif =  $select->execute()->fetchField();
			//	$result[0]->notification_notviewed = $notViewedNotif;
			
			if($_REQUEST['mobile_version']){
				$announcementListCount = fetchAnnouncementUserWidget('count',$userid);
				$result[0]->notification_notviewed = $notViewedNotif+$announcementListCount;
			}else{
				$result[0]->notification_notviewed = $notViewedNotif;
			}

			

				$db_time_query = db_query('select now() from slt_notification limit 1');
				$db_time = $db_time_query->fetchField();
				expDebug::dPrintDBAPI('db_time_query output = ', $db_time);
				$result[0]->db_time = $db_time;

				expDebug::dPrint(
		' Resultttttt = ' . print_r($result, true),4);


		}
		return $result;
		}
		else {
				
				
			expDebug::dPrint(' Notification viwed = ' .$_REQUEST['viewed']);


			$notification_id = explode("|",$_REQUEST['notification_id']);
			for($k=0;$k<count($notification_id);$k++){
			$new_Notiid = $notification_id[$k];
			$check_Noti_table = substr($new_Notiid, -1);
			expDebug::dprint(' 61616197979797 = '.$check_Noti_table ,4);

			if ($check_Noti_table == 1){
			$updStmt = db_update('slt_notification');

			}else if($check_Noti_table == 0){
				$updStmt = db_update('slt_bulk_notification');
			}
				
			$notification_id[$k] = substr($notification_id[$k], 0, -1);

			}

			if($_REQUEST['viewed'] == 1)
			{
			$updStmt->fields(array(
					'viewed' => $_REQUEST['viewed'],
			));
			}
			else
			{

				$updStmt->fields(array(
						'soft_delete' => $_REQUEST['soft_delete'],
				));
			}
 		//	$updStmt->condition('id', $_REQUEST['notification_id']);
			$updStmt->condition('id',$notification_id,'IN');
			expDebug::dPrintDBAPI('set viewed or not viewed query =  ' ,$updStmt);
				$updStmt->execute();

				$result = array();


			$select2 = db_select('slt_notification', 'notif');
				$select2->addField('notif', 'id', 'id');
			//   $select->addExpression('COUNT(notif.id)');
			$select2->condition('notif.user_id', $userId);
			$select2->condition('notif.mobile', 1);
			$select2->condition('notif.viewed', 1 , '<>');
			$select2->condition('notif.soft_delete', 1,'<>');
				
			$select1 = db_select('slt_bulk_notification', 'notif');
			$select1->addField('notif', 'id', 'id');
			//   $select->addExpression('COUNT(notif.id)');
			$select1->condition('notif.user_id', $userId);
			$select1->condition('notif.mobile', 1);
			$select1->condition('notif.viewed', 1 , '<>');
			$select1->condition('notif.soft_delete', 1,'<>');
			$selectunion = $select1->union($select2,'UNION ALL');
			$select = db_select($selectunion,'noti');
			$select->addExpression('COUNT(noti.id)');


				$notViewedNotif =  $select->execute()->fetchField();
				$result[0]->notification_notviewed = $notViewedNotif;
				return $result;
				}

				}
				catch (Exception $ex) {
				watchdog_exception(
			'getPushNotificationDetailsAPI', $ex);
		expertusErrorThrow($ex);
	}
}


function exp_mobile_login_success($userId,$username) {
try{

expDebug::dPrint('login test login test', 5);

  	$httpReferer = explode("&",$_SERVER['HTTP_REFERER']);
  	expDebug::dPrint($_SERVER['HTTP_REFERER'] , 4);
  	$drupal_user_id = getDrupalIdOfUser($userId);
  	$user_info      = user_load($drupal_user_id);
  	$is_admin_perm  = user_access('Administration Perm', $user_info);
  	  global $catalog_reg;
     $catalog_reg = 'Class';


		global $user;
		$user->name = $username;
		expDebug::dPrint(' $user->name = ' . print_r($user->name, true),4);

		expDebug::dPrint('login test login test2222', 5);

	//Mandatory class and program auto_registration start
  		if(variable_get('exp_sp_mandatory_auto_registration_allowed', FALSE) && $userId  ){

  		expDebug::dPrint('login test login test33333', 5);
  		$solrSource = getConfigValue('solr_search');
  		if($solrSource == 1){
  			require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_catalog.php';
  			try{
  				expDebug::dPrint ( "Solr Search Testing", 1 );
  				$rtnType = $_REQUEST['returntype'];
  				$_REQUEST['returntype'] = 'json';
  				$solrObj = new CatalogSolrSearch ();
  				$search_results = $solrObj->getSolrSearchResult ('mobilelogin',$userId);
  				$resultSet = $search_results ['records'];
  				expDebug::dPrint ( 'Result set from solrlogin file' . print_r ( $search_results, true ), 5 );
  				$count = $search_results ['recCount'];
  				expDebug::dPrint ( 'Result set count' . print_r ( $count, true ), 5 );
  				$_REQUEST['returntype'] = $rtnType;
  				unset($_REQUEST['SolrImpl']);
  			}catch(Exception $e){
  				expDebug::dPrint("Error in Solr Search -- ".print_r($e->getMessage(),true),1);
  			}
  			expDebug::dPrint("Solr search result - Final ".print_r($qry_search_results,1),4);
  		}
  		else {
  		  	  	  // Include the search file.
 		  module_load_include('inc', 'exp_sp_lnrsearch', 'exp_sp_lnrsearch');
  		  $lnrSrch = new LnrSearchCatalog();
  		  //$lnrSrch -> searchResultsQry();
  		  //$lnrSrch->select = $lnrSrch->catalogSearchQuery();
  		  expDebug::dPrint('login test login test44444', 5);

  		  $lnrSrch->catalogSearchQueryClass('login');
  		  expDebug::dPrint('login test login test55555', 5);
  		  if(module_exists('exp_sp_commerce')){
  		    $lnrSrch->searchPriceClass('$0-$0');
  		  }
  		  expDebug::dPrint('login test login test55555', 5);
  		  //$lnrSrch->searchDate('', '');
    	  $lnrSrch->setTrainingType('cre_sys_inv_com|cre_sys_inv_man',true);
    	  $classQuery=$lnrSrch->select;
    	   expDebug::dPrint('login test login test66666', 5);
    	  $lnrSrch->catalogSearchQueryTP();
  		  if(module_exists('exp_sp_commerce')){
  		 	$lnrSrch->searchPriceTP('$0-$0');
  		  }
  		  expDebug::dPrint('login test login test777777', 5);
  		  //$lnrSrch->searchDate('', '');
    	  $lnrSrch->setTrainingTypeTP('cre_sys_inv_man',true);
    	  $TPQuery=$lnrSrch->select;
			expDebug::dPrint('login test login test77777', 5);
    	  $classQuery->union($TPQuery,'UNION ALL');
   		  $resultSet = $classQuery->execute()->fetchAll();
   		  expDebug::dPrintDBAPI('select class lang course',$classQuery);
  		}
   		  expDebug::dPrint('auto reg resultset '. print_r($resultSet,true) , 4);

  		  	foreach ($resultSet as $result) {
  		    $object_type  	= $result->object_type;
   		    $crs_id	    	= $result->crs_id;
   		    $cls_id 		= $result->cls_id;
   		    $node_id 		= $result->node_id;
   		    $mro_id 		= $result->mro_id;
   		    $is_compliance  = $result->is_compliance;
   		    $published_on = $result->published_on;
   		      if (($mro_id == 'cre_sys_inv_man' || ($is_compliance == 1 && chkCrsCompleteByDate($crs_id))) && $userId !=1) {
   			    if($object_type == 'Class'){
   			      $maxCount = getIfSingleClass($crs_id,$userId,$is_compliance);
                  if($maxCount[0]->maxcnt == 1) {
       			      $getRegister = getRegisteredOrNot($crs_id,$cls_id,$userId,'','','login');
       			        if(empty($getRegister->enrolled_id)){
       			  	    $delivery_type_code = $result->delivery_type_code;
       			  	    $available_seats = 1;
       			  	    if($delivery_type_code == 'lrn_cls_dty_ilt' || $delivery_type_code == 'lrn_cls_dty_vcl'){
       			          $available_seats = getAvailableSeats($cls_id);
       			  	    }
       			  	    $registerEndDateCheck = registerEndDateCheck($crs_id,$cls_id);
       			  	    $preRequisite = checkCatalogPrerequisite($crs_id,'cre_sys_obt_crs',$userId); //if there is pre-requestie set for this course it should not register directly.
    				    expDebug::dPrint('class register call.' . $available_seats .' end date ' .$registerEndDateCheck . 'enroll' .$getRegister->enrolled_id , 4);
    					if(count($preRequisite) == 0) {
    					    if($available_seats >= 1 && $registerEndDateCheck == 0){
    					      expDebug::dPrint("class register call.");
    					      $waitlist = false;
    					      $isAdminSide = 'N';
    					      register_user_to_course_class($userId, $crs_id, $cls_id,$waitlist,$isAdminSide,false);
    					    }
    					}else{
    						$published = explode(" ",$published_on);
    						$getVal = getFrequencyDataValue('man_comp_multiple_class_register',$published[0]);
    						if($getVal == 1)	{
    							insertNotification("MultipleComplianceClassNotification", $crs_id, $cls_id, $userId, $updateUserId);
    						}
    					}
       			      }else{
       			      	/*#	0032377 : compliance class update when jobrole or ORG or UserType change*/
       			      	$updateStmt = db_update('slt_enrollment');
       			      	// Set conditions
       			      	$updateStmt->condition('id',$getRegister->enrolled_id,'=');
       			      	// Set new values of fields
       			      	$updateStmt->fields(array(
       			      			'is_compliance' => $is_compliance,
       			      			'updated_on'    => now(),
       			      			'updated_by'    => $userId
       			      	));
       			      	expDebug::dPrintDBAPI(' $updateStmt = ', $updateStmt);
       			      	$updateStmt->execute();
       			      }
                  }/*else{
                  	if($is_compliance == 1 || $mro_id == 'cre_sys_inv_man'){
                  		if($is_compliance == 1){$label = 'MultipleComplianceClassNotification';}else{$label='MultipleMandatoryClassNotification';}
                  		$getRegister = getRegisteredOrNot($crs_id,$cls_id,$userId);
                  		if(empty($getRegister->enrolled_id)){
                  			$published = explode(" ",$published_on);
                  			$getVal = getFrequencyDataValue('man_comp_multiple_class_register',$published[0]);
                  			expDebug::dPrint("Remainder to Register Mutiple Class Noification ".print_r($getVal,true),4);
                  			if($getVal == 1)	{
                  				insertNotification($label, $crs_id, $cls_id, $userId, $updateUserId);
                  			}
                  		}
                  	}
                  } */

   			    } else {
   			  	  $resultsMasterEnroll = getObjectRegisteredOrNot($cls_id,$userId);
   			  	  if(empty($resultsMasterEnroll->master_enrolled_id)){
				    $prm_available_seats = getObjectAvailableSeats($cls_id,$userId);
				    if(($prm_available_seats > 0 || $prm_available_seats == -1)){
				  	  $maxClassCnt = getCourseClassCountForLP($cls_id,$userId,'N');
				  	  $preRequisite = checkCatalogPrerequisite($cls_id,'cre_sys_obt_trp'); //if there is pre-requestie set for this Training Plan it should not register directly.
				  	  if($maxClassCnt[0]->cnt == 1 && count($preRequisite) == 0  ) {
	   	 			    $classId = $maxClassCnt[0]->classids;
	   	 			    register_user_to_training_plan($userId,$cls_id, "$classId",false,'N',false);
				  	  }/*else{
							 		$prm_id 		= $result->cls_id;
							 		$published_on = $result->published_on;
							 		$published = explode(" ",$published_on);
							 		$getVal = getFrequencyDataValue('mandatory_tp_multiple_register',$published[0]);
							 		if($getVal == 1)	{
							 				insertNotification("MultipleMantadoryTPNotification", $prm_id, null, $userId, $updateUserId);
							 		}
								}*/
				    }
   			      }
   			    }
   		      }
   		  }





   		  }

  		//Mandatory class and program auto_registration end


expDebug::dPrint('updateUserIdupdateUserId'.updateUserId, 5);
syncSolrData('Enrollment');
syncSolrData('User');
}catch (Exception $ex) {
    watchdog_exception('exp_mobile_login_success', $ex);
    expertusErrorThrow($ex);
}


}

function getUserComparisonResultsAPI()
{
	try{

		include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_sitesetup/exp_sp_administration_module_info/exp_sp_admin_userpoints/exp_sp_admin_userpoints.inc";

		$user_name = explode('~_', $_REQUEST['input_command']);
			
		$selectQuery = db_select('slt_person','per');
		$selectQuery->leftJoin('users', 'u', 'per.user_name = u.name');
		$selectQuery->leftJoin('slt_person_other_manager', 'per_oth_mng', 'per_oth_mng.user_id=per.id and per_oth_mng.is_direct=\'N\'');
		// 		$selectQuery->leftJoin('slt_track_reminder_call', 'src', 'src.subordinate_id = per.id AND src.manager_id = '.$_REQUEST['user_id'].'');
		$selectQuery->leftJoin('file_managed', 'file', 'u.picture = file.fid');
		$selectQuery->addField('per','id', 'user_id');
		$selectQuery->addField('per','full_name', 'full_name');
		$selectQuery->addField('file', 'uri', 'profile_pic');
		$selectQuery->addField('per', 'email', 'email_id');
		$selectQuery->addField('per', 'employee_no', 'employee_no');
		// 		$selectQuery->addExpression('COUNT(src.reminder)', 'count_reminder');
		// 		$selectQuery->addExpression('COUNT(src.calls)', 'count_calls');
		$db_or = db_or();
		$db_or->condition('per.manager_id',$_REQUEST['user_id']);
		//$db_or->where('FIND_IN_SET('.$_REQUEST['user_id'].', per.dotted_mngr_id)');
		$db_or->condition('per_oth_mng.manager_id',$_REQUEST['user_id']);
		$selectQuery->condition($db_or);
		$selectQuery->condition(db_or()->
				condition('per.full_name', '%' . db_like($user_name[0]) . '%', 'LIKE')->
				condition('per.full_name', '%' . db_like($user_name[1]) . '%', 'LIKE')
				);

		expDebug::dPrintDBAPI('Ajay fetching data input = ',$selectQuery);
		$output = $selectQuery->execute()->fetchAll();
		expDebug::dPrint(' Resultttttt = ' . print_r($output, true),4);

		if(empty($output))
		{
			$result[0]->status = "Failure";
			return $result;
		}
		else
		{
			$idInArray = array();
			$reminder_overall_count = 0;
			$calls_overall_count = 0;
				
			for($k = 0;$k<count($output);$k++)
			{
				$totalPoints	  =   getTotalPointsGainedByUser($output[$k]->user_id);
				$totalActionPoints  = getActionPointsForUser($output[$k]->user_id);
				$selectQuerySrc = db_select('slt_track_reminder_call','src');
				$selectQuerySrc->addExpression('COUNT(src.reminder)', 'count_reminder');
				$selectQuerySrc->condition('src.manager_id',$_REQUEST['user_id']);
				$selectQuerySrc->condition('src.subordinate_id',$output[$k]->user_id);
				$selectQuerySrc->condition('src.reminder',1);

				expDebug::dPrintDBAPI('Ajay fetching data input new = ',$selectQuerySrc);
				$count_reminder = $selectQuerySrc->execute()->fetchField();

				$reminder_overall_count = $reminder_overall_count + $count_reminder;

				expDebug::dPrint(' Resultttttt count_reminder = ' .$count_reminder);

				$selectQuerySrc = db_select('slt_track_reminder_call','src');
				$selectQuerySrc->addExpression('COUNT(src.calls)', 'count_calls');
				$selectQuerySrc->condition('src.manager_id',$_REQUEST['user_id']);
				$selectQuerySrc->condition('src.subordinate_id',$output[$k]->user_id);
				$selectQuerySrc->condition('src.calls',1);

				expDebug::dPrintDBAPI('Ajay fetching data input new = ',$selectQuerySrc);
				$count_calls = $selectQuerySrc->execute()->fetchField();
				expDebug::dPrint(' Resultttttt count_calls = ' .$count_calls);
				$calls_overall_count = $calls_overall_count + $count_calls;

				$userBadgesView     = BadgesCount($totalActionPoints);
				$output[$k]->points = $totalPoints;
				$output[$k]->badges = $userBadgesView;
				$output[$k]->count_reminder = $count_reminder;
				$output[$k]->count_calls = $count_calls;
				$output[$k]->reminder_overall_count = $reminder_overall_count;
				$output[$k]->calls_overall_count = $calls_overall_count;
				array_push($idInArray,$output[$k]->user_id);
			}
		}

		$startDate = $_REQUEST['startDate'];
		$endDate = $_REQUEST['endDate'];
		$startDate = str_replace('-', '/', $startDate); //2014/11/25;
		$endDate = str_replace('-', '/', $endDate); //2014/11/25;
		$startDate = $startDate.' 00:00:00';
		$endDate = $endDate.' 23:59:59';

		$select = db_select('slt_enrollment','enroll');
		$select->addExpression('COUNT(enroll.comp_status)', 'count');
		$select->addField('enroll','comp_status', 'comp_status');
		$select->addField('enroll','user_id', 'user_id');
		$select->addField('enroll','is_compliance', 'object_type');
		$select->condition('enroll.reg_status','lrn_crs_reg_cnf');
		$select->condition('enroll.user_id',$idInArray, 'IN');
		$select->condition('enroll.updated_on', array($startDate, $endDate), 'BETWEEN');
		$select->isNull('enroll.master_enrollment_id');
		$select->groupby('enroll.user_id');
		$select->groupby('enroll.comp_status');
		$select->groupby('enroll.is_compliance');
		$selectProgram = db_select('slt_master_enrollment','mast');
		$selectProgram->leftJoin('slt_program', 'prog', 'prog.id = mast.program_id');
		$selectProgram->addExpression('COUNT(mast.overall_status)', 'count');
		$selectProgram->addField('mast','overall_status', 'comp_status');
		$selectProgram->addField('mast','user_id', 'user_id');
		$selectProgram->addField('prog','object_type', 'object_type');
		$selectProgram->condition('mast.overall_status','lrn_tpm_ovr_cln', '!=');
		$selectProgram->condition('mast.user_id',$idInArray, 'IN');
		$selectProgram->condition('mast.updated_on', array($startDate, $endDate), 'BETWEEN');
		$selectProgram->groupby('mast.user_id');
		$selectProgram->groupby('mast.overall_status');
		$selectProgram->groupby('prog.object_type');
		$select->union($selectProgram, 'UNION ALL');
		expDebug::dPrintDBAPI('Ajay fetching data = ',$select);
		$result = $select->execute()->fetchAll();

		// Query for fetching compliance and mandatory count
		$selectCompliance = db_select('slt_enrollment', 'enr');
		$selectCompliance->leftJoin('slt_person', 'per','enr.user_id = per.id');
		$selectCompliance->addExpression('if(find_in_set(\'lrn_crs_cmp_cmp\',group_concat(enr.comp_status)) > 0,\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_enr\')','comp_status');
		$selectCompliance->addExpression('if(find_in_set(\'lrn_crs_cmp_cmp\',group_concat(enr.comp_status)) > 0,1,1=1)','status_count');
		$selectCompliance->addField('per', 'id', 'user_id');
		$selectCompliance->addField('enr', 'course_id', 'crs_id');
		$selectCompliance->addField('enr', 'is_compliance', 'is_compliance');
		$selectCompliance->addField('enr', 'mandatory', 'mandatory');
		$selectCompliance->condition('enr.reg_status', 'lrn_crs_reg_cnf');
		$selectCompliance->condition('enr.user_id',$idInArray, 'IN');
		$or = db_or();
		$or->condition('enr.is_compliance', '1');
		$or->condition('enr.mandatory', 'Y');
		$selectCompliance->condition($or);
		$selectCompliance->isNull('enr.master_enrollment_id');
		$selectCompliance->groupBy('per.id');
		$selectCompliance->groupBy('enr.course_id');
		$subQuery =  db_select($selectCompliance,'acs');
		$subQuery->addField('acs', 'user_id', 'user_id');
		$subQuery->addField('acs', 'comp_status', 'comp_status');
		$subQuery->addField('acs', 'is_compliance', 'is_compliance');
		$subQuery->addField('acs', 'mandatory', 'mandatory');
		$subQuery->addExpression('COUNT(acs.comp_status)', 'status_count');
		$subQuery->groupBy('comp_status');
		$subQuery->groupBy('user_id');
		$subQuery->groupBy('is_compliance');
		$subQuery->groupBy('mandatory');

		expDebug::dPrintDBAPI("select compliance query for user comparison :",$subQuery);

		$complianceResult =  $subQuery->execute()->fetchAll();

		expDebug::dPrint('Result of compliance details for user comparison ='.print_r($complianceResult,true) , 4);

		$output[0]->status = "Success";
		$output[0]->data_available = true;

		if(empty($result))
		{
			$output[0]->data_available = false;
			return 	$output;
		}

		for($i=0;$i<count($output);$i++)
		{
			$overallRegCount = 0;
			$overallCertCount = 0;
			$overallProgCount = 0;
			$output[$i]->completed = 0;
			$output[$i]->enrolled = 0;
			$output[$i]->incomplete = 0;
			$output[$i]->program_enrolled = 0;
			$output[$i]->program_completed = 0;
			$output[$i]->program_incomplete = 0;
			$output[$i]->certification_completed = 0;
			$output[$i]->certification_expired = 0;
			$output[$i]->compliance_completed = 0;
			$output[$i]->compliance_pending = 0;
			$output[$i]->mandatory_completed = 0;
			$output[$i]->mandatory_pending = 0;

				
			for($p = 0;$p<count($complianceResult);$p++)
			{
					
				if($output[$i]->user_id == $complianceResult[$p]->user_id)
				{
					if($complianceResult[$p]->is_compliance)
					{
						if($complianceResult[$p]->comp_status == 'lrn_crs_cmp_cmp')
						{
							$output[$i]->compliance_completed = $complianceResult[$p]->status_count;
						}
						else
						{
							$output[$i]->compliance_pending = $complianceResult[$p]->status_count;
						}
					}
					else
					{
						if($complianceResult[$p]->comp_status == 'lrn_crs_cmp_cmp')
						{
							$output[$i]->mandatory_completed = $complianceResult[$p]->status_count;
						}
						else
						{
							$output[$i]->mandatory_pending = $complianceResult[$p]->status_count;
						}
					}
				}
					
			}
				
				
			for($j=0;$j<count($result);$j++)
			{
				if($output[$i]->user_id == $result[$j]->user_id)
				{
					if($result[$j]->comp_status == 'lrn_crs_cmp_cmp')
					{
						$output[$i]->completed = $output[$i]->completed + $result[$j]->count;
						$overallRegCount = $overallRegCount + $result[$j]->count;
					}
					else if($result[$j]->comp_status == 'lrn_crs_cmp_enr' || $result[$j]->comp_status == 'lrn_crs_cmp_inp')
					{
						$output[$i]->enrolled = $output[$i]->enrolled + $result[$j]->count;
						$overallRegCount = $overallRegCount + $result[$j]->count;
					}
					else if($result[$j]->comp_status == 'lrn_crs_cmp_inc')
					{
						$output[$i]->incomplete = $output[$i]->incomplete + $result[$j]->count;
						$overallRegCount = $overallRegCount + $result[$j]->count;
					}
					else if($result[$j]->comp_status == 'lrn_crs_cmp_exp')
					{
						$output[$i]->expired = $output[$i]->expired + $result[$j]->count;
						$overallRegCount = $overallRegCount + $result[$j]->count;
					}
					else if($result[$j]->comp_status == 'lrn_tpm_ovr_enr' || $result[$j]->comp_status == 'lrn_tpm_ovr_inp')
					{
						$output[$i]->program_enrolled = $output[$i]->program_enrolled + $result[$j]->count;
						$overallProgCount = $overallProgCount + $result[$j]->count;
					}
					else if($result[$j]->comp_status == 'lrn_tpm_ovr_cmp')
					{
						$output[$i]->program_completed = $output[$i]->program_completed + $result[$j]->count;
						$overallProgCount = $overallProgCount + $result[$j]->count;
						if($result[$j]->object_type == 'cre_sys_obt_crt')
						{
							$output[$i]->certification_completed = $result[$j]->count;
							$overallCertCount = $overallCertCount + $output[$i]->certification_completed;
						}
					}
					else if($result[$j]->comp_status == 'lrn_tpm_ovr_inc')
					{
						$output[$i]->program_incomplete = $output[$i]->program_incomplete + $result[$j]->count;
						$overallProgCount = $overallProgCount + $result[$j]->count;
					}
					else if($result[$j]->comp_status == 'lrn_tpm_ovr_exp')
					{
						$output[$i]->program_expired = $output[$i]->program_expired + $result[$j]->count;
						$overallProgCount = $overallProgCount + $result[$j]->count;

						if($result[$j]->object_type == 'cre_sys_obt_crt')
						{
							$output[$i]->certification_expired = $result[$j]->count;
							$overallCertCount = $overallCertCount + $output[$i]->certification_expired;
						}
					}

				}
			}
			$output[$i]->overall_class_count = $overallRegCount;
			$output[$i]->overall_program_count = $overallProgCount;
			$output[$i]->overall_certification_count = $overallCertCount;
		}
			
		expDebug::dPrint(' Resultttttt = ' . print_r($result, true),4);
		return 	$output;
	}
	catch (Exception $ex) {
		watchdog_exception('configurePushNotificationDetailsAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function UpdateReminderCallAPI(){
	try{

		$insLog  = db_insert('slt_track_reminder_call');
		$insLog->fields(array(
				'manager_id' => $_REQUEST['userid'],
				'subordinate_id' => $_REQUEST['subordinate_id'],
				'calls' => $_REQUEST['call'],
				'reminder' => $_REQUEST['reminder'],
		));
		$logId = $insLog->execute();

		$status = ( !empty($logId) )?'success':'failure';

		$result-> status =$status;

		return $result;

	}
	catch (Exception $ex) {
		watchdog_exception('UpdateReminderCallAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function getCatalogClassListAPI(){
	try{

		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_core/exp_sp_core.inc');


		$select = db_select('slt_course_class','cls');
		$select->addField('cls','id', 'cls_id');
		$select->addField('cls','course_id', 'crs_id');
		$select->addField('cls','title', 'cls_title');
		$select->addField('cls','title', 'cls_title');
		$select->addField('cls','delivery_type', 'dl_type');
		if(isset($_REQUEST['mobile_request'])){
		$select->addField('cls','registration_end_on', 'registration_end_on');
		}
		//   $alisename = 'cls';
		//   $entity_type = 'cre_sys_obt_cls';

		$select->condition('cls.status','lrn_cls_sts_atv','=');
		$select->condition('cls.price','0.00','=');

		$select->condition('cls.title', '%' . db_like($_REQUEST['filter_text']) . '%', 'LIKE');

		filterByUserAccessPrivileges($select, 'cls',$_REQUEST['user_id'],'','','Class');

		expDebug::dPrintDBAPI('narrow search sql query-->>',$select);
		$result = $select->execute()->fetchAll();

		if(isset($_REQUEST['mobile_request'])){
			
			foreach ($result as $rec_search_results) {
			
				$createDate = new DateTime($rec_search_results->registration_end_on);
				$registration_end_on = $createDate->format('Y-m-d');
				$presentDate=date('Y-m-d');
			
				if(strtotime($registration_end_on) >= strtotime($presentDate) || empty($createDate)){
					$rec_search_results->registration_ended= "false";
				}
				else{
					$rec_search_results->registration_ended="true";
				}
			
			}
				
		}
		
		return  $result;

	}
	catch (Exception $ex) {
		watchdog_exception('getCatalogClassListAPI', $ex);
		expertusErrorThrow($ex);
	}
}



function getAnnouncementResults($userid) {
	try{
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		
		$userPreferredLang = getUserPreferredLanguage();
		expDebug::dPrint('get $userPreferred Language'.$userPreferredLang,4);
			$userTimezone = getUserPHPTimezoneName($userid);
			expDebug::dPrint('get $userPreferred Language111');
			$timeZone = date_default_timezone(false);
			expDebug::dPrint('get $userPreferred Language222'.$userTimezone[0]->zone_name.$timeZone.$userid);
			$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
			$select = db_select('slt_site_notice', 'snote');
			$select->leftJoin('slt_site_notice_dismiss', 'snd', 'snd.nid = snote.id and snd.user_id = '.$userid);
			$select->leftJoin('slt_person', 'p', 'p.id = snote.user_id');
			$select->leftJoin('users', 'u', 'u.name = p.user_name');
			$select->leftJoin('file_managed', 'pic', 'u.picture = pic.fid');
			$select->isNull('snd.id');
			$select->where("CONCAT( DATE_FORMAT(snote.from_date, _utf8'%Y-%m-%d'),' ',snote.from_time,':00') < IFNULL(CONVERT_TZ(now(), '".$defaultTimezone."' ,'".$userTimezone[0]->zone_name."'),NOW())");
			$select->where("CONCAT( DATE_FORMAT(snote.to_date, _utf8'%Y-%m-%d'),' ',snote.to_time,':00') > IFNULL(CONVERT_TZ(now(), '".$defaultTimezone."' ,'".$userTimezone[0]->zone_name."'),NOW()) or to_date is null");
			$select->condition('snote.status', 'cre_sys_obt_not_atv', '=');
			$select->condition('snote.lang_code', $userPreferredLang , '=');
			filterByUserAccessPrivileges($select, 'snote',$userid,'','cre_sys_obt_not');
			$select->distinct();
			$select->addField('snote', 'description', 'notification_subject');
			$select->addField('snote', 'user_id', 'msg_id');
			$select->addField('snote', 'from_date', 'updated_on');
			$select->addField('snote', 'id', 'notification_id');
			$select->addField('p', 'full_name', 'notification_title');
			$select->addField('pic', 'uri', 'action_detail');
			$select->addExpression('null', 'token_str');
			$select->addExpression('null', 'soft_delete');
			$select->addExpression('null', 'notification_template');
			$select->addExpression('null', 'msg_type');
			$select->addExpression('FIND_IN_SET('.$userid.', snote.viewed)', 'viewed');
			$select->addExpression('\'announcement\'', 'type');
			$select->addExpression('\'announcement\'', 'noti_type');
				
			$selectInd = $select->extend("ExpertusSelectQueryExtender");
			$selectInd->setIndex(array('snote'=>'sli_siteann_lan'));
			expDebug::dPrintDBAPI('Fetch announcement query LIST = ' , $selectInd);
						$announcementList = $select->execute()->fetchAll();
							expDebug::dPrint(' $$announcementList = ' . print_r($announcementList, true) , 4);
					
			return $select;
	}catch(Exception $ex) {
		expDebug::dPrint('fetchAnnouncementAll'.$ex,4);
		watchdog_exception('fetchAnnouncementAll', $ex);
		expertusErrorThrow($ex);
	}
}
/**
 * 
 * @param unknown $mode
 * @param unknown $name
 * @param unknown $value
 */
function get_set_user_preferences_menu($mode) {
	try {
		$user_id = getSltpersonUserId();
// 		expDebug::dPrint('$_POST '.print_r($_POST , 1), 1);
		$catalog_refine = $_POST['catalog_refine'];
			if($user_id>0){
			$pref = get_set_user_preferences($user_id, array('catalog_refine' => (int)$catalog_refine), $mode);
			expDebug::dPrint('$user_id--------> '.print_r($pref , 1), 1);
			echo drupal_json_output($pref);
		}
		else{
			$_SESSION['guest_user_preferences']['catalog_refine'] = $_POST['catalog_refine'];
			if((isset($_SESSION['guest_user_preferences']['catalog_refine']) && $_SESSION['guest_user_preferences']['catalog_refine'] == 1)) {
				$hide_filter = 1;
			} else {
				$hide_filter = 0;
			}
			$_SESSION['guest_user_preferences']['catalog_criteria_class'] = (($hide_filter) ? 'searchcriteria-div-unpinned' : '');
			$_SESSION['guest_user_preferences']['catalog_content_class'] = (($hide_filter) ? 'catalog-extended' : '');
			drupal_add_js(array('user_preferences' => array('catalog_refine' => $_SESSION['guest_user_preferences']['catalog_refine'])), 'setting');
			drupal_add_js(array('user_preferences' => array('catalog_criteria_class' => $_SESSION['guest_user_preferences']['catalog_criteria_class'])), 'setting');
			drupal_add_js(array('user_preferences' => array('catalog_content_class' => $_SESSION['guest_user_preferences']['catalog_content_class'])), 'setting');
			echo drupal_json_output(array('guest_user_preferences'=>$_SESSION['guest_user_preferences']));
		}
		
	} catch (Exception $ex) {
		expDebug::dPrint('get_set_user_preferences_menu '.$ex->getMessage(), 2);
		watchdog_exception('get_set_user_preferences_menu', $ex);
		expertusErrorThrow($ex);
	}
}

function getLanguageAutoComplete(){
	try{
		$search = $_GET['z'];
		expDebug::dPrint(' $$announcementList = ' . print_r($search, true) , 4);
		$select = db_select('slt_profile_list_items','prof');
		$select->condition('prof.parent_id','116','=');
		$select->condition('prof.is_active','Y','=');
		$select->condition('prof.name', '%' . db_like($search) . '%', 'LIKE');
		$select->addField('prof','name','lang');
		$lang = $select->execute()->fetchField();
		expDebug::dPrintDBAPI('Fetch language query LIST = ' , $select);
		expDebug::dPrint('language = ' . print_r($lang, true) , 4);
		print $lang;
	}
	catch(Exception $ex) {
		expDebug::dPrint('getLanguageAutoComplete'.$ex,4);
		watchdog_exception('getLanguageAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}
function getLanguageAutoCompleteFetch(){
	try{
		$search = $_GET['z'];
		expDebug::dPrint(' $$announcementList = ' . print_r($search, true) , 4);
		$select = db_select('slt_profile_list_items','prof');
		$select->condition('prof.parent_id','116','=');
		$select->condition('prof.is_active','Y','=');
		$select->condition('prof.name', '%' . db_like($search) . '%', 'LIKE');
		$select->addField('prof','name','lang');
		$lang = $select->execute()->fetchField();
		expDebug::dPrintDBAPI('Fetch language query LIST = ' , $select);
		expDebug::dPrint('language = ' . print_r($lang, true) , 4);
		echo drupal_json_encode($lang);
	}
	catch(Exception $ex) {
		expDebug::dPrint('getLanguageAutoCompleteFetch'.$ex,4);
		watchdog_exception('getLanguageAutoCompleteFetch', $ex);
		expertusErrorThrow($ex);
	}
}

function AddItemsToShoppingCartAPI($learner_id,$entity_type,$entity_id,$delivery_type,$class_ids)
{
	try{
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$orderId_select=db_select('slt_order','ord');
		$orderId_select->addField('ord','id','ord_id');
		$orderId_select->condition('ord.user_id',$learner_id,'=');
		$orderId_select->condition('ord.order_status','cme_pmt_sts_rsv','=');
		$orderId=$orderId_select->execute()->fetchField();
		expDebug::dPrint(' $orderId = ' . print_r($orderId, true) , 4);

		if (empty ($orderId))
		{
			$is_enabled_select=db_select('system','sys');
			$is_enabled_select->addField('sys','status','status');
			$is_enabled_select->condition('sys.name','exp_sp_commerce','=');
			$is_enabled_select->condition('sys.type','module','=');
			$is_enabled=$is_enabled_select->execute()->fetchField();
			expDebug::dPrintDBAPI(' $is_enabled = ' ,$is_enabled_select);
			expDebug::dPrint(' $is_enabled = ' . print_r($is_enabled, true) , 4);
	
			if(!$is_enabled)
			{
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Commerce is disabled. Contact your Site Administrator.";
				return $errobj;
			}
			if(!is_numeric($learner_id))
			{
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "<learner id> accepts only numerical values";
				return $errobj;
			}
			if(!is_numeric($entity_id))
			{
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "<Entity id> accepts only numerical values";
				return $errobj;
			}
			include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_core/exp_sp_core.inc');
			$learner_status=getPersonDetails($learner_id,array('id','status'));
			expDebug::dPrint(' $learner_status = ' . print_r($learner_status, true) , 4);
			 
			if(empty($learner_status) || $learner_status['status']=='cre_usr_sts_del')
			{
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Invalid Learner ID";
				return $errobj;
			}
			if(!empty($learner_status) && $learner_status['status']!='cre_usr_sts_atv')
			{
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Learner is suspended";
				return $errobj;
			}
			if(!in_array($entity_type,array('Class','Training Plan'))){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Invalid Entity Type";
				return $errobj;
			}
			if($entity_type=='Class')
			{
				if(!in_array($delivery_type, array('Classroom','Web-based','VirtualClass','Video'))){
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "Invalid Delivery Type";
					return $errobj;
				}
			}
			if($entity_type=='Training Plan')
			{
				$regstatus_select=db_select('slt_master_enrollment','mas');
				$regstatus_select->addField('mas','overall_status','status');
				$regstatus_select->condition('mas.user_id',$learner_id,'=');
				$regstatus_select->condition('mas.program_id',$entity_id,'=');
				$regstatus=$regstatus_select->execute()->fetchField();
				expDebug::dPrint('yuvan order_id ' . print_r($regstatus, true) , 4);
				if(!in_array($delivery_type, array('LearningPlan','Curricula','Certification'))){
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "Invalid Delivery Type";
					return $errobj;
				}
			
				if (!empty($regstatus)){
			
			
					if(!in_array($regstatus, array('lrn_tpm_ovr_rsc','lrn_tpm_ovr_can','lrn_tpm_ovr_inc'))){
			
						$errobj=new stdClass();
						$errobj->isValidateError = 1;
						$errobj->errcode = 'L_012';
						$errobj->errormsg = "You have already registered for this Training Plan";
						return $errobj;
			
					}
			
			
				}
			}
			if($entity_type=='Class')
			{
				$entity_select = db_select('slt_course_class', 'cls');
				$entity_select->addField('cls','id','classid');
				$entity_select->addField('cls','course_id','courseid');
				$entity_select->addField('cls','is_compliance','compliance');
				$entity_select->addField('cls','delivery_type','delivery_type');
				$entity_select->addField('cls','price','price');
				$entity_select->addField('cls','registration_end_on','registration_end_on');
				$entity_select->addField('cls','status','status');
				$entity_select->condition('cls.id',$entity_id,'=');
				$is_entity_valid=$entity_select->execute()->rowCount();
				
				if($is_entity_valid==0)
				{
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "Invalid Entity ID";
					return $errobj;
				}
				$is_entity_valid=$entity_select->execute()->fetchAssoc();
				expDebug::dPrint(' $is_entity_valid = ' . print_r($is_entity_valid, true) , 4);
				if($is_entity_valid['status']!='lrn_cls_sts_atv')
				{
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "Invalid Class Status";
					return $errobj;
				}
				if($is_entity_valid['price']<=0)
				{
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "Entered entity is not a paid Class";
					return $errobj;
				}
				if(!empty($is_entity_valid['registration_end_on']))
				{
					$currentdate=date('Y-m-d H:i:s');
					expDebug::dPrint(' $$currentdate = ' . print_r($currentdate, true) , 4);
					expDebug::dPrint(' $is_entity_valid registration_end_on =' . print_r($is_entity_valid['registration_end_on'], true) , 4);
						
					if($currentdate>$is_entity_valid['registration_end_on'])
					{
						$errobj=new stdClass();
						$errobj->isValidateError = 1;
						$errobj->errcode = 'L_012';
						$errobj->errormsg = "Registration is closed.";
						return $errobj;
					}
				}
	
				$entity_sel = db_select('slt_course_template', 'crs');
				$entity_sel->addField('crs','id','id');
				$entity_sel->addField('crs','complete_days','complete_days');
				$entity_sel->addField('crs','complete_date','complete_date');
				$entity_sel->addField('crs','validity_date','validity_date');
				$entity_sel->addField('crs','compliance_completed','compliance_completed');
				$entity_sel->condition('crs.id',$is_entity_valid['courseid'],'=');
				$entity_compliance=$entity_sel->execute()->fetchAssoc();
				expDebug::dPrint(' $is_$compliance_check' . print_r($entity_compliance, true) , 4);
	
				$hire_date=getPersonDetails($learner_id,array('id','hire_date'));
	
				if($entity_compliance['compliance_completed']=='hire_days' && !empty($hire_date['hire_date']))
				{
					$complete_by_date=date('Y-m-d H:i:s', strtotime($hire_date['hire_date']. ' + '. $entity_compliance['complete_days'].' days'));
					$current_date=date('Y-m-d H:i:s');
					expDebug::dPrint(' $is_$compliance_check' . print_r($complete_by_date, true) , 4);
					expDebug::dPrint(' $is_$compliance_check' . print_r($current_date, true) , 4);
	
					if($current_date>$complete_by_date)
					{
						$errobj=new stdClass();
						$errobj->isValidateError = 1;
						$errobj->errcode = 'L_012';
						$errobj->errormsg = "Course is expired for the user.";
						return $errobj;
					}
				}
				else
				{
					if($entity_compliance['compliance_completed']=='date' && !empty($entity_compliance['complete_date']))
					{
						$current_date=date('Y-m-d H:i:s');
						expDebug::dPrint(' $is_$compliance_check' . print_r($current_date, true) , 4);
						expDebug::dPrint(' $is_$compliance_check' . print_r($entity_compliance['complete_date'], true) , 4);
						
						if($current_date>$entity_compliance['complete_date'])
						{
							$errobj=new stdClass();
							$errobj->isValidateError = 1;
							$errobj->errcode = 'L_012';
							$errobj->errormsg = "Course is expired for the user.";
							return $errobj;
						}
					}
				}
				if(!empty($entity_compliance['validity_date']))
				{
					$current_date=date('Y-m-d H:i:s');
						
					expDebug::dPrint(' subu' . print_r($entity_compliance['validity_date'], true) , 4);
					expDebug::dPrint(' subu 1' . print_r($current_date, true) , 4);
						
					if($current_date>$entity_compliance['validity_date'])
					{
						$errobj=new stdClass();
						$errobj->isValidateError = 1;
						$errobj->errcode = 'L_012';
						$errobj->errormsg = "Course is expired for the user.";
						return $errobj;
					}
				}
	
	
	
				expDebug::dPrint(' $is_entity_valid = ' . print_r($is_entity_valid, true) , 4);
				
				$select = db_select('slt_course_class', 'cls');
				$select->addField('cls','id','cls_id');
				filterByUserAccessPrivileges($select,'cls',$learner_id,'','cre_sys_obt_cls','Class','','','','','');
				$select->condition('cls.id',$entity_id,'=');
				$catalogaccess=$select->execute()->fetchField();
				expDebug::dPrint(' $catalogaccess = ' . print_r($catalogaccess, true) , 4);
				if(empty($catalogaccess))
				{
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "You do not have the necessary privileges to register for this Class.";
					return $errobj;
				}
				$is_class_dedicated=is_dedicated($entity_id);
				expDebug::dPrint(' $$is_class_dedicated = ' . print_r($is_class_dedicated, true) , 4);


				if(!empty($is_class_dedicated))
				{
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "This class is dedicated for a Training Plan. Enrollments can only be done from the Training Plan.";
					return $errobj;
				}
				$prerequisite_select=db_select('slt_common_mapping','scm');
				$prerequisite_select->addField('scm','id1','entityId');
				$prerequisite_select->addField('scm','prereq_type','prereq_type');
				$prerequisite_select->addExpression('count(scm.id2)','prereq_cnt');
				$prerequisite_select->addExpression('group_concat(scm.id2)','prereq_id');
				$prerequisite_select->condition('scm.id1',$is_entity_valid['courseid'],'=');
				$prerequisite_select->condition('scm.object_type','cre_sys_obt_crs','=');
				$prerequisite_select->condition('scm.type',5,'=');
				$prerequisite_select->groupby('scm.prereq_type');

				$prerequisite_id=$prerequisite_select->execute()->fetchAssoc();
				expDebug::dPrint(' $$prerequisite_id = ' . print_r($is_entity_valid['courseid'], true) , 4);

				expDebug::dPrint(' $$prerequisite_id = ' . print_r($prerequisite_id, true) , 4);
				expDebug::dPrintDBAPI(' $prequisite_check = ', $prerequisite_select);

				if(!empty($prerequisite_id) && $prerequisite_id['prereq_type']=='cre_sys_pre_crs')
				{
					$prerequisite_status_select=db_query("select count(distinct(course_id)) as comp_cnt,user_id from slt_enrollment where find_in_set(course_id,'".$prerequisite_id['prereq_id']."')>0 and comp_status='lrn_crs_cmp_cmp' and master_enrollment_id is null and user_id=".$learner_id);
					$prerequisite_status=$prerequisite_status_select->fetchAssoc();
					expDebug::dPrint(' $prerequisite_status = ' . print_r($prerequisite_status, true) , 4);
					expDebug::dPrintDBAPI(' $$prerequisite_status = ',$prerequisite_status_select);
					 
					if($prerequisite_status['comp_cnt']!=$prerequisite_id['prereq_cnt'])
					{
						$errobj=new stdClass();
						$errobj->isValidateError = 1;
						$errobj->errcode = 'L_012';
						$errobj->errormsg = "You need to complete the prerequisite before adding the Class to the cart.";
						return $errobj;
					}
					 
				}
			}
			if($entity_type=='Training Plan')
			{
				$entity_select = db_select('slt_program', 'pgm');
				$entity_select->addField('pgm','id','programid');
				$entity_select->addField('pgm','price','price');
				$entity_select->addField('pgm','object_type','objecttype');
				$entity_select->addField('pgm','status','status');
				$entity_select->condition('pgm.id',$entity_id,'=');
				$entity_valid=$entity_select->execute()->rowCount();
				expDebug::dPrint(' $entity_valid = ' . print_r($entity_valid, true) , 4);
				if($entity_valid==0)
				{
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "Invalid Entity ID";
					return $errobj;
				}
        if($entity_valid==1 && $delivery_type == 'LearningPlan'){
          $entity_sel = db_select('slt_program', 'prg');
          $entity_sel->addField('prg','end_date','end_date');
          $entity_sel->condition('prg.id',$entity_id,'=');
          $entity_completeby=$entity_sel->execute()->fetchAssoc();
          expDebug::dPrint('Learning plan complete by date ' . print_r($entity_completeby, true) , 4);
            
            if(!empty($entity_completeby['end_date'])){
            $complete_by_date=$entity_completeby['end_date'];
            $current_date=date('Y-m-d H:i:s');
            expDebug::dPrint(' Learning plan complete by date ' . print_r($complete_by_date, true) , 4);
            expDebug::dPrint(' Learning plan complete by date ' . print_r($current_date, true) , 4);
            if($current_date>$complete_by_date)
            {
              $errobj=new stdClass();
              $errobj->isValidateError = 1;
              $errobj->errcode = 'L_012';
              $errobj->errormsg = "Training Plan is expired for the user.";
              return $errobj;
            }
          }
        }
				$entity_valid=$entity_select->execute()->fetchAssoc();
				expDebug::dPrint(' $entity_valid = ' . print_r($entity_valid, true) , 4);
				if($entity_valid['status']!='lrn_lpn_sts_atv')
				{
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "Invalid Training Plan Status";
					return $errobj;
				}
				if($entity_valid['price']<=0)
				{
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "Entered entity is not a paid Training Plan";
					return $errobj;
				}

				$tp_map_select=db_select('slt_module_crs_mapping','map');
				$tp_map_select->addExpression('count(map.id)','tp_mapping_count');
				$tp_map_select->addExpression('GROUP_CONCAT(map.course_id)','course_id');
				$tp_map_select->condition('map.program_id',$entity_id,'=');
				$tp_map_count=$tp_map_select->execute()->fetchAssoc();
				expDebug::dPrint(' $$tp_map_count = ' . print_r($tp_map_count, true) , 4);

				$tp_course_cls_select=db_select('slt_module_crs_mapping','map');
				$tp_course_cls_select->innerJoin('slt_course_class','cls',"map.course_id=cls.course_id and map.program_id=".$entity_id);
				$tp_course_cls_select->addExpression('count(cls.id)','tp_course_cls_count');
        $tp_course_cls_select->condition('cls.status',array('lrn_cls_sts_del','lrn_cls_sts_dld'),'NOT IN');
        expDebug::dPrintDBAPI(' $$tp_course_cls_select = ' ,$tp_course_cls_select);
				$tp_course_cls_count=$tp_course_cls_select->execute()->fetchField();
				expDebug::dPrint(' $$tp_course_cls_count = ' . print_r($tp_course_cls_count, true) , 4);

				// if(empty($class_ids) && $tp_map_count['tp_mapping_count']<$tp_course_cls_count)
				// {
				// 	$errobj=new stdClass();
				// 	$errobj->isValidateError = 1;
				// 	$errobj->errcode = 'L_012';
				// 	$errobj->errormsg = "TP associated Course contains more than one class, Provide class ID of a class listed under the each course that is associated with the Training Plan";
				// 	return $errobj;
				// }
				// else{
				// 	if(!empty($class_ids) && $tp_map_count['tp_mapping_count']<=$tp_course_cls_count){
				// 		$classIdsCount=explode(',',$class_ids);
				// 		if(count($classIdsCount)!=$tp_map_count['tp_mapping_count'])
				// 		{
				// 			$errobj=new stdClass();
				// 			$errobj->isValidateError = 1;
				// 			$errobj->errcode = 'L_012';
				// 			$errobj->errormsg = "Invalid Class_ids";
				// 			return $errobj;
				// 		}
						 
				// 		$tp_class_select=db_query("select count(distinct(cls.course_id)) from slt_course_class cls where cls.status='lrn_cls_sts_atv' and FIND_IN_SET(cls.id,'".$class_ids."')>0 and FIND_IN_SET(cls.course_id,'".$tp_map_count['course_id']."')>0");
				// 		$tp_class_count=$tp_class_select->fetchField();

				// 		expDebug::dPrint(' $$tp_class_count = ' . print_r($tp_class_count, true) , 4);
				// 		expDebug::dPrintDBAPI(' $tp_class_count = ' ,$tp_class_select);

				// 		if($tp_map_count['tp_mapping_count']!=$tp_class_count){
				// 			$errobj=new stdClass();
				// 			$errobj->isValidateError = 1;
				// 			$errobj->errcode = 'L_012';
				// 			$errobj->errormsg = "Invalid Class_ids";
				// 			return $errobj;
				// 		}
				// 	}
				// }
				$prerequisite_select=db_select('slt_common_mapping','scm');
				$prerequisite_select->addField('scm','id1','entityId');
				$prerequisite_select->addField('scm','prereq_type','prereq_type');
				$prerequisite_select->addExpression('count(scm.id2)','prereq_cnt');
				$prerequisite_select->addExpression('group_concat(scm.id2)','prereq_id');
				$prerequisite_select->condition('scm.id1',$entity_valid['programid'],'=');
				$prerequisite_select->condition('scm.object_type','cre_sys_obt_trp','=');
				$prerequisite_select->condition('scm.type',5,'=');
				$prerequisite_select->groupby('scm.prereq_type');
				 
				$prerequisite_id=$prerequisite_select->execute()->fetchAll();
				expDebug::dPrint(' $$prerequisite_id = ' . print_r($entity_valid['programid'], true) , 4);
				 
				expDebug::dPrint(' $$prerequisite_id = ' . print_r($prerequisite_id, true) , 4);
				expDebug::dPrintDBAPI(' $prequisite_check = ', $prerequisite_select);
				 
				if(!empty($prerequisite_id))
				{
					for($i=0;$i<count($prerequisite_id);$i++)
					{
						if($prerequisite_id[$i]->prereq_type=='cre_sys_pre_crs')
						{
							$prerequisite_status_select=db_query("select count(distinct(course_id)) as comp_cnt,user_id from slt_enrollment where find_in_set(course_id,'".$prerequisite_id[$i]->prereq_id."')>0 and comp_status='lrn_crs_cmp_cmp' and master_enrollment_id is null and user_id=".$learner_id);
							$prerequisite_status=$prerequisite_status_select->fetchAssoc();
							expDebug::dPrint(' $prerequisite_status = ' . print_r($prerequisite_status, true) , 4);
							expDebug::dPrintDBAPI(' $$prerequisite_status = ',$prerequisite_status_select);
							 
							if($prerequisite_status['comp_cnt']!=$prerequisite_id[$i]->prereq_cnt)
							{
								$errobj=new stdClass();
								$errobj->isValidateError = 1;
								$errobj->errcode = 'L_012';
								$errobj->errormsg = "You need to complete the prerequisite before adding the Training Plan to the cart.";
								return $errobj;
							}

						}
						 
						else
						{
							if($prerequisite_id[$i]->prereq_type=='cre_sys_pre_trp')
							{
								$prerequisite_status_select=db_query("select count(distinct(program_id)) as comp_cnt,user_id from slt_master_enrollment where find_in_set(program_id,'".$prerequisite_id[$i]->prereq_id."')>0 and overall_status='lrn_tpm_ovr_cmp' and user_id=".$learner_id);
								$prerequisite_status=$prerequisite_status_select->fetchAssoc();
								expDebug::dPrint(' $prerequisite_status = ' . print_r($prerequisite_status, true) , 4);
								expDebug::dPrintDBAPI(' $$prerequisite_status = ',$prerequisite_status_select);

								if($prerequisite_status['comp_cnt']!=$prerequisite_id[$i]->prereq_cnt)
								{
									$errobj=new stdClass();
									$errobj->isValidateError = 1;
									$errobj->errcode = 'L_012';
									$errobj->errormsg = "The Training Plan can't be added to the cart. Since the prerequisite needs to be completed";
									return $errobj;
								}

							}
						}
					}
				}
			}
			/*
			if($entity_type=='Class' && !empty($class_ids)){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = " class_ids parameter is not required for entity type Class. It is applicable only for entity type Training Plan";
				return $errobj;
					
				}
				*/
			switch ($delivery_type) {
					
				case "Classroom":
					$deliveryType='lrn_cls_dty_ilt';
					if($is_entity_valid['delivery_type']!=$deliveryType)
					{
						$errobj=new stdClass();
						$errobj->isValidateError = 1;
						$errobj->errcode = 'L_012';
						$errobj->errormsg = " Delivery Type does not match with entered Entity";
						return $errobj;
					}
					break;
				case "VirtualClass":
					$deliveryType='lrn_cls_dty_vcl';
					if($is_entity_valid['delivery_type']!=$deliveryType)
					{
						$errobj=new stdClass();
						$errobj->isValidateError = 1;
						$errobj->errcode = 'L_012';
						$errobj->errormsg = " Delivery Type does not match with entered Entity";
						return $errobj;
					}
					break;
				case "Web-based":
					$deliveryType='lrn_cls_dty_wbt';
					if($is_entity_valid['delivery_type']!=$deliveryType)
					{
						$errobj=new stdClass();
						$errobj->isValidateError = 1;
						$errobj->errcode = 'L_012';
						$errobj->errormsg = " Delivery Type does not match with entered Entity";
						return $errobj;
					}
					break;
				case "Video":
					$deliveryType='lrn_cls_dty_vod';
					if($is_entity_valid['delivery_type']!=$deliveryType)
					{
						$errobj=new stdClass();
						$errobj->isValidateError = 1;
						$errobj->errcode = 'L_012';
						$errobj->errormsg = " Delivery Type does not match with entered Entity";
						return $errobj;
					}
					break;
				case "Certification":
					$deliveryType='cre_sys_obt_crt';
					if($entity_valid['objecttype']!=$deliveryType)
					{
						$errobj=new stdClass();
						$errobj->isValidateError = 1;
						$errobj->errcode = 'L_012';
						$errobj->errormsg = " Delivery Type does not match with entered Entity";
						return $errobj;
					}
					break;
				case "Curricula":
					$deliveryType='cre_sys_obt_cur';
					if($entity_valid['objecttype']!=$deliveryType)
					{
						$errobj=new stdClass();
						$errobj->isValidateError = 1;
						$errobj->errcode = 'L_012';
						$errobj->errormsg = " Delivery Type does not match with entered Entity";
						return $errobj;
					}
					break;
				case "LearningPlan":
					$deliveryType='cre_sys_obt_trn';
					if($entity_valid['objecttype']!=$deliveryType)
					{
						$errobj=new stdClass();
						$errobj->isValidateError = 1;
						$errobj->errcode = 'L_012';
						$errobj->errormsg = " Delivery Type does not match with entered Entity";
						return $errobj;
					}
					break;
				default:
					$deliveryType='';
						}
			expDebug::dPrint(' $deliveryType = ' . print_r($deliveryType, true) , 4);

			if($entity_type=='Class'){
				$class_details=$entity_select->execute()->fetchAssoc();
				$class_ids='';
				expDebug::dPrint(' $class_details = ' . print_r($class_details, true) , 4);

				$addtocart_status=addClassToCartAPI($learner_id,$class_details['courseid'],$class_details['classid'],$deliveryType);

				expDebug::dPrint(' $addtocart_status = ' . print_r($addtocart_status, true) , 4);
				return $addtocart_status;
			}
			if($entity_type=='Training Plan'){
				$select_pgm = db_select('slt_program','pgm');
				$select_pgm->addField('pgm','id','pgm_id');
				filterByUserAccessPrivileges($select_pgm,'pgm',$learner_id,'',$deliveryType,'Program','','','','','');
				$select_pgm->condition('pgm.id',$entity_id,'=');
				$catalogaccess=$select_pgm->execute()->fetchField();
				expDebug::dPrint(' $catalogaccess = ' . print_r($catalogaccess, true) , 4);
				if(empty($catalogaccess))
				{
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "You do not have the necessary privileges to register for this Training Plan.";
					return $errobj;
				}
				$tp_details=$entity_select->execute()->fetchAssoc();
				$reg_type='';
				if(!empty($class_ids))
				{
					$reg_type='MultiRegister';
				}
				expDebug::dPrint(' $tp_details = ' . print_r($tp_details, true) , 4);
				$addtocart_status=addTPToCartAPI($learner_id,$tp_details['programid'],$class_ids,$tp_details['objecttype'],$reg_type,'');
				expDebug::dPrint(' $addtocart_status = ' . print_r($addtocart_status, true) , 4);
			}
		}

		else{
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "A new Item cannot be added to Cart as a checkout is already in progress";
			return $errobj;
		}

		return $addtocart_status;

	}
	catch(Exception $ex) {
		expDebug::dPrint('AddItemsToShoppingCartAPI'.$ex,4);
		watchdog_exception('AddItemsToShoppingCartAPI', $ex);
		expertusErrorThrow($ex);
	}

}

function ListItemsFromShoppingCartAPI($learner_id,$entity_id,$entity_type)
{
	try{

	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	if(!is_numeric($learner_id))
	{
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->errormsg = "<learner id> accepts only numerical values";
		return $errobj;
	}
	include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_core/exp_sp_core.inc');
	$learner_status=getPersonDetails($learner_id,array('id','status'));
	expDebug::dPrint(' $learner_status = ' . print_r($learner_status, true) , 4);
	if(empty($learner_status))
	{
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->errormsg = "Invalid Learner ID";
		return $errobj;
	}
	
	if(!in_array($entity_type,array('Class','Training Plan')) && !empty($entity_type)){
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->errormsg = "Invalid Entity Type";
		return $errobj;
	}
	if(!empty($entity_type) && empty($entity_id)) {
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->errormsg = "Entity ID is required when Entity Type is provided";
		return $errobj;
	}
	if(!empty($entity_id) && empty($entity_type)) {
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->errormsg = "Entity Type is required when Entity ID is provided";
		return $errobj;
	}
	if($entity_type=='Class')
	{
		$entity_select = db_select('slt_course_class', 'cls');
		$entity_select->addField('cls','id','classid');
		$entity_select->addField('cls','course_id','courseid');
		$entity_select->condition('cls.id',$entity_id,'=');
		$is_entity_valid=$entity_select->execute()->rowCount();
		expDebug::dPrint(' $is_entity_valid = ' . print_r($is_entity_valid, true) , 4);
		if($is_entity_valid==0)
		{
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Invalid Entity ID";
			return $errobj;
		}
		
		
	}
	if($entity_type=='Training Plan')
	{
		$entity_select = db_select('slt_program', 'pgm');
		$entity_select->addField('pgm','id','programid');
		$entity_select->addField('pgm','object_type','objecttype');
		$entity_select->condition('pgm.id',$entity_id,'=');
		$entity_valid=$entity_select->execute()->rowCount();
		expDebug::dPrint(' $entity_valid = ' . print_r($entity_valid, true) , 4);
		if($entity_valid==0)
		{
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Invalid Entity ID";
			return $errobj;
		}
	}
	$uid = getDrupalIdOfUser($learner_id);
	
	expDebug::dPrint(' $uid = ' . print_r($uid, true) , 4);
	$cart_select=db_select('uc_cart_products','uc');
	$cart_select->addField('uc','cart_item_id','cart_item_id');
	$cart_select->condition('uc.cart_id',$uid,'=');
	$cartItem=$cart_select->execute()->fetchAll();
	
	if(empty($cartItem))
	{
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->errormsg = "Your Shopping Cart is Empty";
		return $errobj;
	}
	 
	if($entity_type=='Class' || empty($entity_type))
	{
		$selectStmt = db_select('uc_cart_products', 'c');
		$selectStmt->innerJoin('node', 'n', "n.nid = c.nid AND c.cart_id =".$uid);
		$selectStmt->innerJoin('slt_node_learning_activity', 'nl', 'nl.node_id = n.nid and nl.entity_type = \'cre_sys_obt_cls\'');
		if(!empty($entity_type))
				{
		$selectStmt->innerJoin('slt_course_class', 'cls', "cls.id = nl.entity_id and nl.entity_type=cls.entity_type and cls.id=".$entity_id);
		}
		else {
			$selectStmt->innerJoin('slt_course_class', 'cls', 'cls.id = nl.entity_id and nl.entity_type=cls.entity_type');
		}
		$selectStmt->leftJoin('slt_location', 'loc', 'loc.id = cls.location_id');
		$selectStmt->innerJoin('slt_profile_list_items', 'prof', 'prof.code = cls.delivery_type');
		$selectStmt->innerJoin('users', 'u', 'u.uid = c.cart_id');
		$selectStmt->innerJoin('slt_person', 'p', 'p.user_name = u.name');
		$selectStmt->addField('cls','id','entity_id');
		$selectStmt->addField('prof','name','delivery_type');
		$selectStmt->addField('cls','price','price');
		$selectStmt->addField('cls','currency_type','currency_type');
		$selectStmt->addField('p','id','learner_id');
		$selectStmt->addField('loc','name','location');
		$selectStmt->addExpression("'Class'",'entity_type');
		$totalRecords = $selectStmt->execute()->fetchAll();
		expDebug::dPrintDBAPI(' $$selectStmt = ',$selectStmt);
		
		
	}
	 
	if($entity_type=='Training Plan' || empty($entity_type))
	{
		$selectStmt1 = db_select('uc_cart_products', 'c');
		$selectStmt1->innerJoin('node', 'n', "n.nid = c.nid AND c.cart_id =".$uid);
		$selectStmt1->innerJoin('slt_node_learning_activity', 'nl', 'nl.node_id = n.nid and nl.entity_type in (\'cre_sys_obt_cur\',\'cre_sys_obt_crt\',\'cre_sys_obt_trn\')');
		if(!empty($entity_type))
		{
		$selectStmt1->innerJoin('slt_program', 'pgm', "pgm.id = nl.entity_id and nl.entity_type=pgm.object_type and pgm.id=".$entity_id);
		}
		else 
		{
			$selectStmt1->innerJoin('slt_program', 'pgm', 'pgm.id = nl.entity_id and nl.entity_type=pgm.object_type');
		}
		$selectStmt1->innerJoin('slt_profile_list_items', 'prof', 'prof.code = pgm.object_type');
		$selectStmt1->innerJoin('users', 'u', 'u.uid = c.cart_id');
		$selectStmt1->innerJoin('slt_person', 'p', 'p.user_name = u.name');
		$selectStmt1->addField('pgm','id','entity_id');
		$selectStmt1->addField('prof','name','delivery_type');
		$selectStmt1->addField('pgm','price','price');
		$selectStmt1->addField('pgm','currency_type','currency_type');
		$selectStmt1->addField('p','id','learner_id');
		$selectStmt1->addExpression("''",'location');
		$selectStmt1->addExpression("'Training Plan'",'entity_type');
		$totalRecords = $selectStmt1->execute()->fetchAll();
		expDebug::dPrintDBAPI(' $$selectStmt1 = ',$selectStmt1);
		
	}
	 
	if(empty($entity_type))
	{
		$selectStmt2=$selectStmt->union($selectStmt1,'UNION ALL');
		$selectStmt3=db_select($selectStmt2,'res');
		$selectStmt3->addField('res','entity_type');
		$selectStmt3->addField('res','entity_id');
		$selectStmt3->addField('res','delivery_type');
		$selectStmt3->addField('res','price');
		$selectStmt3->addField('res','currency_type');
		$selectStmt3->addField('res','learner_id');
		$selectStmt3->addField('res','location');
		
		$totalRecords = $selectStmt3->execute()->fetchAll();
		expDebug::dPrintDBAPI(' $$selectStmt = ',$selectStmt3);
		
		  
	}
	 
	$cnt=count($totalRecords);
	$amount=0;
	for($i=0;$i<$cnt;$i++)
	{
		$amount=$totalRecords[$i]->price+$amount;
	
		expDebug::dPrint(' $amount = ' . print_r($amount, true) , 4);
	
	} 
	$totalRecord[0]->total_price=number_format($amount,2);
	$totalRecord[0]->items=$totalRecords;
	
	//$node_id = $selectStmt->execute()->fetchAll();
	return $totalRecord;
	}
	catch(Exception $ex) {
		expDebug::dPrint('ListItemsFromShoppingCartAPI'.$ex,4);
		watchdog_exception('ListItemsFromShoppingCartAPI', $ex);
		expertusErrorThrow($ex);
	}
	//    return array($totalRecords);
}

function RemoveItemsFromShoppingCartAPI($learner_id,$entity_type,$entity_id,$userid)
{
try{
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	$uid = getDrupalIdOfUser($learner_id);
	
	expDebug::dPrint(' $learner id in the function= ' . print_r($learner_id, true) , 4);
	if(!is_numeric($learner_id))
	{
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->errormsg = "<learner id> accepts only numerical values";
		return $errobj;
	}
	include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_core/exp_sp_core.inc');
	$learner_status=getPersonDetails($learner_id,array('id','status'));
	expDebug::dPrint(' $learner_status = ' . print_r($learner_status, true) , 4);
	if(empty($learner_status))
	{
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->errormsg = "Invalid Learner ID";
		return $errobj;
	}
	
	if(!in_array($entity_type,array('Class','Training Plan')) && !empty($entity_type)){
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->errormsg = "Invalid Entity Type";
		return $errobj;
	}
	if(!empty($entity_type) && empty($entity_id)) {
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->errormsg = "Entity ID is required when Entity Type is provided";
		return $errobj;
	}
	if(!empty($entity_id) && empty($entity_type)) {
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->errormsg = "Entity Type is required when Entity ID is provided";
		return $errobj;
	}
	$cart_select=db_select('uc_cart_products','uc');
	$cart_select->addField('uc','cart_item_id','cart_item_id');
	$cart_select->condition('uc.cart_id',$uid,'=');
	if($entity_type=='Class')
	{
		$entity_select = db_select('slt_course_class', 'cls');
		$entity_select->addField('cls','id','classid');
		$entity_select->addField('cls','course_id','courseid');
		$entity_select->condition('cls.id',$entity_id,'=');
		$is_entity_valid=$entity_select->execute()->rowCount();
		expDebug::dPrint(' $is_entity_valid = ' . print_r($is_entity_valid, true) , 4);
		if($is_entity_valid==0)
		{
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Invalid Entity ID";
			return $errobj;
		}
		$order_select = db_select('slt_enrollment', 'enr');
		$order_select->addField('enr','order_id','orderid');
		$order_select->condition('enr.class_id',$entity_id,'=');
		$order_select->condition('enr.user_id',$learner_id,'=');
		$order_select->condition('enr.reg_status','lrn_crs_reg_rsv','=');
		$sltOrderId=$order_select->execute()->fetchAssoc();
		expDebug::dPrint(' $$sltOrderId = ' . print_r($sltOrderId, true) , 4);
		
		
		$cartItem=$cart_select->execute()->fetchAll();
		
		if(empty($cartItem))
		{
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Your Shopping Cart is Empty";
			return $errobj;
		}
		
		$action='';
		$deliverytypename='';
		expDebug::dPrint(' $uid = ' . print_r($uid, true) , 4);
		$status=deleteItemsFromCartAPI($learner_id, $userid, $entity_id,$sltOrderId['orderid'],$action,$deliverytypename);
		expDebug::dPrint(' $status = ' . print_r($status, true) , 4);
		
	}
	if($entity_type=='Training Plan')
	{
		$entity_select = db_select('slt_program', 'pgm');
		$entity_select->addField('pgm','id','programid');
		$entity_select->addField('pgm','object_type','objecttype');
		$entity_select->condition('pgm.id',$entity_id,'=');
		$entity_valid=$entity_select->execute()->rowCount();
		expDebug::dPrint(' $entity_valid = ' . print_r($entity_valid, true) , 4);
		if($entity_valid==0)
		{
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Invalid Entity ID";
			return $errobj;
		}
		$entity_valid=$entity_select->execute()->fetchAssoc();
		expDebug::dPrint(' $sdubu = ' . print_r($entity_valid, true) , 4);
		
		$order_select = db_select('slt_master_enrollment','mas');
		$order_select->addField('mas','order_id','orderid');
		$order_select->condition('mas.program_id',$entity_id,'=');
		$order_select->condition('mas.user_id',$learner_id,'=');
		$order_select->condition('mas.overall_status','lrn_tpm_ovr_rsv','=');
		$sltOrderId=$order_select->execute()->fetchAssoc();
		expDebug::dPrint(' $$sltOrderId = ' . print_r($sltOrderId, true) , 4);
		
		
		$cartItem=$cart_select->execute()->fetchAll();
		
		if(empty($cartItem))
		{
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Your Shopping Cart is Empty";
			return $errobj;
		}
		
		$action='';
		expDebug::dPrint(' $uid = ' . print_r($uid, true) , 4);
		$status=deleteItemsFromCartAPI($learner_id, $userid, $entity_id, $sltOrderId['orderid'],$action,$entity_valid['objecttype']);
		expDebug::dPrint(' $status = ' . print_r($status, true) , 4);
	}
	/*
	$order_select = db_select('slt_order', 'ord');
	$order_select->innerJoin('slt_enrollment','enr','enr.order_id=ord.id and enr.reg_status = \'lrn_crs_reg_rsv\' and enr.user_id='.$learner_id);
	$order_select->leftJoin('slt_master_enrollment','mas','mas.order_id=ord.id and mas.overall_status = \'lrn_tpm_ovr_rsv\' and mas.user_id='.$learner_id);
	$order_select->addField('ord','id','orderid');
	$totalRecords = $forder_select->execute()->fetchAssoc();*/
	if(empty($entity_type))
	{
		$details=ListItemsFromShoppingCartAPI($learner_id,$entity_id,$entity_type);
		expDebug::dPrint(' $details = ' . print_r($details, true) , 4);
		
		$cartItem=$cart_select->execute()->fetchAll();
		
		if(empty($cartItem))
		{
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Your Shopping Cart is Empty";
			return $errobj;
		}
	$order_select=db_query("SELECT ord.id AS orderid
	FROM
	slt_order ord
	INNER JOIN slt_enrollment enr ON (enr.order_id=ord.id and enr.reg_status = 'lrn_crs_reg_rsv' and enr.user_id=".$learner_id.") 
	LEFT JOIN slt_master_enrollment mas ON (mas.order_id=ord.id and  mas.overall_status = 'lrn_tpm_ovr_rsv' and mas.user_id=".$learner_id.") group by ord.id");
    $order_id= $order_select->fetchAll();
	
	expDebug::dPrintDBAPI(' $order_select = ',$order_select);
	
	expDebug::dPrint(' $order_id = ' . print_r($order_id, true) , 4);
	
	$action='';
	$deliverytypename='';

	$status=deleteItemsFromCartAPI($learner_id, $userid, $entity_id, $order_id,$action,$deliverytypename);
	expDebug::dPrint(' $status = ' . print_r(count($details[0]->items), true) , 4);
	}
	expDebug::dPrint(' $Subu = ' . print_r($status, true) , 4);
	
	if($status=='success' && !empty($entity_id) && !empty($entity_type))
	{
		$result->status=$status;
		$result->items->entity_id=$entity_id;
		$result->items->entity_type=$entity_type;
		$result->learner_id=$learner_id;
		expDebug::dPrint(' $result = ' . print_r($result, true) , 4);
		
		
		
	}
	else 
	{
		if($status=='success'){
		$result->status=$status;
		for($i=0;$i<count($details[0]->items);$i++)
		{
			$Items[$i]->entity_id=$details[0]->items[$i]->entity_id;
			$Items[$i]->entity_type=$details[0]->items[$i]->entity_type;
				
		}
		$result->learner_id=$learner_id;
		$result->items=$Items;
		expDebug::dPrint(' $result = ' . print_r($result, true) , 4);
		

		
	}
	}
	return $result;
	
}
catch(Exception $ex) {
	expDebug::dPrint('RemoveItemsFromShoppingCartAPI'.$ex,4);
	watchdog_exception('RemoveItemsFromShoppingCartAPI', $ex);
	expertusErrorThrow($ex);
}
}


function CheckOutAPI($learner_id,$userid)
{
	try{
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$drupal_user_id = getDrupalIdOfUser($learner_id);
		
		$orderId_select=db_select('slt_order','ord');
		$orderId_select->addField('ord','id','ord_id');
		$orderId_select->condition('ord.user_id',$learner_id,'=');
		$orderId_select->condition('ord.order_status','cme_pmt_sts_rsv','=');
		$orderId=$orderId_select->execute()->fetchField();
		expDebug::dPrint(' $orderId = ' . print_r($orderId, true) , 4);
		
		$cart_select=db_select('uc_cart_products','uc');
		$cart_select->addField('uc','cart_item_id','cartid');
		$cart_select->condition('uc.cart_id',$drupal_user_id,'=');
		$cartId=$cart_select->execute()->fetchField();
		expDebug::dPrint(' $cartId = ' . print_r($cartId, true) , 4);
		
		
		
		if(empty($cartId))
		{
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Your shopping cart is empty";
			return $errobj;
		}
	if(!is_numeric($learner_id))
	{
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->errormsg = "<learner id> accepts only numerical values";
		return $errobj;
	}
	include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_core/exp_sp_core.inc');
	$learner_status=getPersonDetails($learner_id,array('id','status'));
	expDebug::dPrint(' $learner_status = ' . print_r($learner_status, true) , 4);
	if(empty($learner_status))
	{
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->errormsg = "Invalid Learner ID";
		return $errobj;
	}
	if(!empty($learner_status) && $learner_status['status']!='cre_usr_sts_atv')
	{
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->errormsg = "Learner is suspended";
		return $errobj;
	}
	if(empty($orderId))
	{
	$action='get';
	$order_details=getListOfclassesinCartAPI($learner_id,$userid,$action);
	expDebug::dPrint(' $order_details = ' . print_r($order_details, true) , 4);
	/*$select_amount= db_select('slt_order_items', 'ord');
		$select_amount->addExpression('sum(ord.item_price)','amount');
		$select_amount->condition('ord.order_id',$order_details[0]->order_id,'=');
		//$select_amount->select->groupby('ord.program_id');
		*/
	$amount=0;
	$class_id='';
	$class_title='';
	$dl_name='';
	$price='';
	for($i=0;$i<$order_details['totalrow'];$i++)
	{
		$amount=$order_details[$i]->price+$amount;
		empty($class_id)?$class_id=$order_details[$i]->classid:$class_id=$class_id."-~".$order_details[$i]->classid;
		empty($class_title)?$class_title=$order_details[$i]->classtitle:$class_title=$class_title."-~".$order_details[$i]->classtitle;
        empty($dl_name)?$dl_name=$order_details[$i]->deliverytypename."-Code-".$order_details[$i]->class_code:$dl_name=$dl_name."-~".$order_details[$i]->deliverytypename."-Code-".$order_details[$i]->class_code;
		empty($price)?$price=$order_details[$i]->price:$price=$price."-~".$order_details[$i]->price;
        
		expDebug::dPrint(' $amount = ' . print_r($amount, true) , 4);
		
	}
	expDebug::dPrint(' $$amount = ' . print_r($amount, true) , 4);
	
	//$amount=$select_amount->execute()->fetchField();
	$uc_order_details=createUcOrderIdAPI($learner_id,$user_id,$amount);
	expDebug::dPrint(' $uc_order_details = ' . print_r($uc_order_details, true) , 4);
	//$class_id='';
	//$class_title='';
	//$dl_name='';
	//$price=$amount;

	$status=updateUcOrderProductAPI($learner_id,$userid,$uc_order_details->order_id,$class_id,$class_title,$dl_name,$price);
	
	}
	else{
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->errormsg = "A checkout is already in progress";
		return $errobj;
	}
	
		$results->learner_id=$learner_id;
		$results->order_id=$uc_order_details->order_id;
		$results->order_sub_total=number_format($amount,2);
		$results->status=$status;
	
	return $results;
	}
	catch(Exception $ex) {
		expDebug::dPrint('CheckOutAPI'.$ex,4);
	watchdog_exception('CheckOutAPI', $ex);
	expertusErrorThrow($ex);
	}
}


function PurchaseAPI($learner_id,$order_id,$payment_method,$userid,$po_number,$cc_number,$cc_exp_month,$cc_exp_year,$cc_cvv,$currency_type,$order_comments)
{
	try{
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	$drupal_user_id = getDrupalIdOfUser($learner_id);
	include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_core/exp_sp_core.inc');
	$learner_status=getPersonDetails($learner_id,array('id','status'));
	expDebug::dPrint(' $learner_status = ' . print_r($learner_status, true) , 4);
	
	
	
	if(empty($learner_status))
	{
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->errormsg = "Invalid Learner ID";
		return $errobj;
	}
	if(!empty($learner_status) && $learner_status['status']!='cre_usr_sts_atv')
	{
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->errormsg = "Learner is suspended";
		return $errobj;
	}
	if(!in_array($payment_method,array('creditcard','po','check','invoice')) || empty($payment_method)){
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->errormsg = "Invalid Payment Method";
		return $errobj;
	}
	if(!empty($order_id))
	{
		$orderId_select=db_select('uc_orders','ord');
		$orderId_select->addField('ord','order_id','ord_id');
		$orderId_select->condition('ord.order_id',$order_id,'=');
		$order_details=$orderId_select->execute()->fetchField();
		if(empty($order_details))
		{
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Invalid Order ID.";
			return $errobj;
		}
	
	}


	if(!empty($drupal_user_id) && !empty($order_id))
	{
		$orderId_select=db_select('uc_orders','ord');
		$orderId_select->addField('ord','order_id','ord_id');
		$orderId_select->addField('ord','order_status','order_status');
		$orderId_select->condition('ord.uid',$drupal_user_id,'=');
		$orderId_select->condition('ord.order_id',$order_id,'=');
		$orderId=$orderId_select->execute()->fetchAssoc();
		if(empty($orderId['ord_id']))
		{
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->errormsg = "Order ID does not match with entered Learner ID";
		return $errobj;
		}
		if($orderId['order_status']!='in_checkout')
		{
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Invalid order status";
			return $errobj;
		}
		$order_billing_select=db_query("select * from uc_orders where ((billing_city is null or billing_city='') or (billing_country is null or billing_country='')
or(billing_zone is null or billing_zone='') or (billing_postal_code is null or billing_postal_code='')) and (order_id=".$order_id.")");
		$order_billing_details=$order_billing_select->fetchField();
		if(!empty($order_billing_details))
		{
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Purchase cannot be completed as the Billing Address has not been entered";
			return $errobj;
		}
	}
	if($payment_method=='creditcard')
	{
		$payment_method='credit';
		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_commerce/modules/ubercart/uc_cart/uc_cart.pages.inc');
	    $result=purchaseClassAPI($learner_id,$userid,$order_id,$payment_method,$po_number,$cc_number,$cc_exp_month,$cc_exp_year,$cc_cvv,$currency_type);
			
	
	}
	elseif($payment_method=='po')
	{
		if(empty($po_number)){
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "PO Number is Mandatory for the Purchase Order payment method";
			return $errobj;
		}
		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_commerce/modules/ubercart/uc_cart/uc_cart.pages.inc');
		$result=purchaseClassAPI($learner_id,$userid,$order_id,$payment_method,$po_number,$cc_number,$cc_exp_month,$cc_exp_year,$cc_cvv);
		
	}
	elseif($payment_method=='check')
	{
	    include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_commerce/modules/ubercart/uc_cart/uc_cart.pages.inc');
		$result=purchaseClassAPI($learner_id,$userid,$order_id,$payment_method,$po_number,$cc_number,$cc_exp_month,$cc_exp_year,$cc_cvv);
	
	}
	elseif($payment_method=='invoice')
	{
		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_commerce/modules/ubercart/uc_cart/uc_cart.pages.inc');
		$result=purchaseClassAPI($learner_id,$userid,$order_id,$payment_method,$po_number,$cc_number,$cc_exp_month,$cc_exp_year,$cc_cvv);
	
	}
	expDebug::dPrint(' $result = ' . print_r($order_comments, true) , 4);
	expDebug::dPrint(' $subu1 = ' . print_r($result, true) , 4);
	
	if($result->status=='success' && !empty($order_comments))
	{
		expDebug::dPrint(' $order_comments = ' . print_r($result, true) , 4);
		
		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_commerce/modules/ubercart/uc_order/uc_order.module');
		
		uc_order_comment_save($order_id, $drupal_user_id, $order_comments,'order', 'pending','TRUE');
		
	}
	
//$result->status = 'success';
$result->order_id = $order_id;
//$result = $total;

return $result;
}
catch(Exception $ex) {
	expDebug::dPrint('CheckOutAPI'.$ex,4);
	watchdog_exception('CheckOutAPI', $ex);
	expertusErrorThrow($ex);
}
}

//yuvanesh- created this function for the cart API UR
function addBillingAddressAPI(){
	try{
			
		include_once($_SERVER["DOCUMENT_ROOT"]. 'sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc');
		
		expDebug::dPrint('check request '.print_r($_REQUEST[first_name],true),5);
		$country =$_REQUEST['country'];
		$zone  = $_REQUEST['state'];
		$order_ID = $_REQUEST['order_id'];
		$user_id= $_REQUEST['learner_id'];
		$uid = getDrupalIdOfUser($user_id);
		
		if(!is_numeric($user_id))
    {
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "<learner id> accepts only numerical values";
    	return $errobj;
    }

$learner_status=getPersonDetails($user_id,array('id','status'));


if(empty($learner_status))
                                        {
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "Invalid Learner ID";
					return $errobj;
                                        }
										
if ($order_ID == 0){
											
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->field = 'orderid';
				$errobj->errormsg = "Invalid Order ID";
				return $errobj;	
											
										}
										
if (!empty ($order_ID)){
			  $selectQuery = db_select('uc_orders', 'ud');
			  $selectQuery->addField('ud', 'order_id','id');
			  $selectQuery->condition('ud.order_id', $order_ID, '=');
			  $res = $selectQuery->execute()->fetchAll();
		if (empty ($res)){
			  $errobj->isValidateError = 1;
			  $errobj->errcode = 'L_012';
			  $errobj->field = 'orderid';
		      $errobj->errormsg = "Invalid Order ID";
			  return $errobj;
					
				}	
				
			
		}

		if(!empty($uid) && !empty($order_ID))
		{
			$orderId_select=db_select('uc_orders','ord');
			$orderId_select->addField('ord','order_id','ord_id');
			$orderId_select->condition('ord.uid',$uid,'=');
			$orderId_select->condition('ord.order_id',$order_ID,'=');
			$orderId=$orderId_select->execute()->fetchField();
			if(empty($orderId))
			{
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Order ID does not match with entered Learner ID";
				return $errobj;
			}
		}
	$selectStmt = db_select('uc_countries', 'ct');
  	// Set conditions
  	$selectStmt->condition('ct.country_name',$country);
 	 // Select state name field for returning
 	 $selectStmt->addField('ct', 'country_id', 'id');
  	expDebug::dPrintDBAPI(' $selectStmt = ' ,$selectStmt);
 	 // Execute query
 	 $country_id = $selectStmt->execute()->fetchField();
if(!empty($country)) {
			
			if(empty($country_id))
			{
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->field = 'learnerid';
				$errobj->errormsg = "Invalid Country code";
				return $errobj;
			}
		}	
 	 $selectStmt = db_select('uc_zones', 'zo');
  	// Set conditions
  	$selectStmt->condition('zo.zone_name',$zone);
 	 // Select state name field for returning
 	 $selectStmt->addField('zo', 'zone_id', 'id');
  	expDebug::dPrintDBAPI(' $selectStmt = ' ,$selectStmt);
 	 // Execute query
 	 $zone_id = $selectStmt->execute()->fetchField();
	 
	 if(!empty($zone)) {
			
			if(empty($zone_id))
			{
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->field = 'learnerid';
				$errobj->errormsg = "Invalid State code";
				return $errobj;
			}
		}	
	 
	 
	 
		// uc_orders id update
		$fields = array(
				'billing_first_name' => $_REQUEST[first_name],
				'billing_last_name' => $_REQUEST[last_name],
				'billing_phone' => $_REQUEST[phone_no],
				'billing_street1' => $_REQUEST[address_line1],
				'billing_street2' => $_REQUEST[address_line2],
				'billing_city' => $_REQUEST[city],
				'billing_zone' => $zone_id,
				'billing_postal_code' => $_REQUEST[zip_code],
				'billing_country' => $country_id,
				
				
		);
		$updStmt = db_update('uc_orders');
		$updStmt->fields($fields);
		$updStmt->condition('order_id', $order_ID);
		$updStmt->condition('uid', $uid);
		expDebug::dPrintDBAPI('$updStmt', $updStmt);
		$ucUpdNum = $updStmt->execute();

		/* slt_order id update
		$ord_fields = array(
				'uc_order_id' => $order_ID,
				'user_id' => $user_id,
		);
		$ord_ins = db_update('slt_order');
		$ord_ins->fields($ord_fields);
		$ord_ins->condition('id', $slt_orderID);
		expDebug::dPrintDBAPI('$ord_ins', $ord_ins);
		$sltUpdNum = $ord_ins->execute();

		expDebug::dPrint('$ucUpdNum', $ucUpdNum);
		expDebug::dPrint(' $sltUpdNum = '.$sltUpdNum);*/


 $result-> status =  'success';

 return $result;

	}catch (Exception $ex) {
		watchdog_exception('addBillingAddressAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function applyDiscountsAPI($learner_id, $userid, $orderid,$discount_code){
	try{
		$user_id = $_REQUEST['learner_id'];
	$userid = $_REQUEST['userid'];
	$orderid = $_REQUEST['order_id'];
	$drupal_user_id = getDrupalIdOfUser($learner_id);
	$discount_code = $_REQUEST['discount_code'];
	
	if(!is_numeric($user_id))
    {
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "<learner id> accepts only numerical values";
    	return $errobj;
    }

$learner_status=getPersonDetails($user_id,array('id','status'));


if(empty($learner_status))
                                        {
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "Invalid Learner ID";
					return $errobj;
                                        }
										
if ($orderid == 0){
											
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->field = 'orderid';
				$errobj->errormsg = "Invalid Order ID";
				return $errobj;	
											
										}
										
if (!empty ($orderid)){
			  $selectQuery = db_select('uc_orders', 'ud');
			  $selectQuery->addField('ud', 'order_id','id');
			  $selectQuery->condition('ud.order_id', $orderid, '=');
			  $res = $selectQuery->execute()->fetchAll();
		if (empty ($res)){
			  $errobj->isValidateError = 1;
			  $errobj->errcode = 'L_012';
			  $errobj->field = 'orderid';
		      $errobj->errormsg = "Invalid Order ID";
			  return $errobj;
					
				}	
				
			
		}


 if(!is_numeric($orderid))
    {
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "< Order id> accepts only numerical values.";
    	return $errobj;
    }

if(!empty($drupal_user_id) && !empty($orderid))
	{
		$orderId_select=db_select('uc_orders','ord');
		$orderId_select->addField('ord','order_id','ord_id');
		$orderId_select->condition('ord.uid',$drupal_user_id,'=');
		$orderId_select->condition('ord.order_id',$orderid,'=');
		$orderId=$orderId_select->execute()->fetchField();
		if(empty($orderId))
		{
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->errormsg = "Order ID does not match with entered Learner ID";
		return $errobj;
		}
	}
	
		if (!empty($orderid)){
		$select = db_select('uc_order_line_items', 'uli');
      $select->addField('uli', 'order_id','order_id');
	$select->condition('uli.type','uc_discounts' , '=');	
	$select->condition('uli.order_id',$orderid , '=');
	expDebug::dPrintDBAPI(' slt_order_id from pretax amount ' , $select);
		$discount_id = $select->execute()->fetchField();
	
	
	// if (!empty ($discount_id))	{
		
		
	// 		$errobj=new stdClass();
 //    	$errobj->isValidateError = 1;
 //    	$errobj->errcode = 'L_012';
 //    	$errobj->errormsg = "Discount already Applied for the given order id";
 //    	return $errobj;
		


		
		
		
	// }
	}


		
		
		
$results = calculateDiscountsAPI($user_id, $userid, $orderid,$discount_code);
expDebug::dPrint('$calculate yuvanesh in lnrsearch' . print_r($results, true), 5);
 $result->status = $results;
 expDebug::dPrint('$calculate yuvanesh in lnrsearch' . print_r($result, true), 5);
 return $result;
 
	}
	catch (Exception $ex) {
		watchdog_exception('applyDiscountsAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function applyTaxAPI(){
try{
	
	include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_commerce/modules/ubercart/contrib/uc_discounts_alt/uc_discounts/uc_discounts.module');
	
	$user_id = $_REQUEST['learner_id'];
	$userid = $_REQUEST['userid'];
	$orderid = $_REQUEST['order_id'];
	$drupal_user_id = getDrupalIdOfUser($user_id);
	
	if(!is_numeric($user_id))
    {
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "<learner id> accepts only numerical values";
    	return $errobj;
    }

$learner_status=getPersonDetails($user_id,array('id','status'));


if(empty($learner_status))
                                        {
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "Invalid Learner ID";
					return $errobj;
                                        }
										
if ($orderid == 0){
											
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->field = 'orderid';
				$errobj->errormsg = "Invalid Order ID";
				return $errobj;	
											
										}
										
if (!empty ($orderid)){
			  $selectQuery = db_select('uc_orders', 'ud');
			  $selectQuery->addField('ud', 'order_id','id');
			  $selectQuery->condition('ud.order_id', $orderid, '=');
			  $res = $selectQuery->execute()->fetchAll();
		if (empty ($res)){
			  $errobj->isValidateError = 1;
			  $errobj->errcode = 'L_012';
			  $errobj->field = 'orderid';
		      $errobj->errormsg = "Invalid Order ID";
			  return $errobj;
					
				}	
				
			
		}


 if(!is_numeric($orderid))
    {
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "< Order id> accepts only numerical values.";
    	return $errobj;
    }
    
    if(!empty($drupal_user_id) && !empty($orderid))
    {
    	$orderId_select=db_select('uc_orders','ord');
    	$orderId_select->addField('ord','order_id','ord_id');
    	$orderId_select->addField('ord', 'order_total','order_total');
    	$orderId_select->condition('ord.uid',$drupal_user_id,'=');
    	$orderId_select->condition('ord.order_id',$orderid,'=');
    	$orderId=$orderId_select->execute()->fetchAssoc();
    	if(empty($orderId['ord_id']))
    	{
    		$errobj=new stdClass();
    		$errobj->isValidateError = 1;
    		$errobj->errcode = 'L_012';
    		$errobj->errormsg = "Order ID does not match with entered Learner ID";
    		return $errobj;
    	}
    }
	
	if (!empty($orderid)){
		$select = db_select('uc_order_line_items', 'uod');
	
	$select->addField('uod', 'order_id','id');
	$select->condition('uod.order_id',$orderid , '=');
	$select->condition('uod.title','Tax' , '=');		
	expDebug::dPrintDBAPI(' slt_order_id from pretax amount ' , $select);
		$tax_details = $select->execute()->fetchField();
	
	
	if (!empty ($tax_details))	{
		
		
			$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Tax has already been calculated for this Order ID";
    	return $errobj;
		
		
		
	}
	}
	$sltOrderId_select=db_select('slt_order','ord');
	$sltOrderId_select->addField('ord','id','id');
	$sltOrderId_select->condition('ord.uc_order_id',$orderid ,'=');
	$sltOrderId=$sltOrderId_select->execute()->fetchAssoc();
	
	$pre_tax_amount = $orderId['order_total'];
	expDebug::dPrint(' slt_order_itax amount ' . print_r($pre_tax_amount, true) , 4);
	
	expDebug::dPrintDBAPI(' slt_order_id from pretax amount ' , $select);
//Below function will set currency details in $_REQUEST
	 calculateTaxAPI($user_id, $userid, $orderid,$sltOrderId['id']);
	expDebug::dPrint(' slt_order_idyuvan from pretax amount ' . print_r($tax_amount, true) , 4);
    $select = db_select('uc_orders', 'uod');
	$select->leftJoin('uc_order_line_items', 'udl', "udl.order_id = uod.order_id and udl.title = 'Tax'" );
	$select->addField('udl', 'amount','amount');
	$select->condition('uod.order_id',$orderid , '=');	
	expDebug::dPrintDBAPI(' slt_order_id from pretax amount ' , $select);
	$tax_amount = $select->execute()->fetchField();
	$post_tax_amount =$pre_tax_amount+$tax_amount;
	
	expDebug::dPrint(' $tax amount ' . print_r($tax_amount, true) , 4);
	expDebug::dPrint('$post_tax_amount ' . print_r($post_tax_amount, true) , 4);
	if(!empty($tax_amount))
	{
	$result->tax_amount = $tax_amount;
	$result->status = 'Success';
	$result->post_tax_amount = 	$post_tax_amount;
	$result->pre_tax_amount  = $pre_tax_amount;
	}
		 return $result;
	}
catch (Exception $ex) {
	watchdog_exception('applyTaxAPI', $ex);
	expertusErrorThrow($ex);
	}
}

function getOrderStatusApi(){
	
	try 
	
	{
		
		
		
	$learner_id  = 	$_REQUEST['learner_id'];
	$user_id = 	$_REQUEST['userid'];
	$orderid = $_REQUEST['order_id'];
	$uid = getDrupalIdOfUser($learner_id);
	
	
	 if(!is_numeric($learner_id))
    {
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "<learner id> accepts only numerical values";
    	return $errobj;
    }

$learner_status=getPersonDetails($learner_id,array('id','status'));


if(empty($learner_status))
                                        {
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "Invalid Learner ID";
					return $errobj;
                                        }
										
if ($orderid == 0){
											
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->field = 'orderid';
				$errobj->errormsg = "Invalid Order ID";
				return $errobj;	
											
										}
										
if (!empty ($orderid)){
			  $selectQuery = db_select('uc_orders', 'ud');
			  $selectQuery->addField('ud', 'order_id','id');
			  $selectQuery->condition('ud.order_id', $orderid, '=');
			  $res = $selectQuery->execute()->fetchAll();
		if (empty ($res)){
			  $errobj->isValidateError = 1;
			  $errobj->errcode = 'L_012';
			  $errobj->field = 'orderid';
		      $errobj->errormsg = "Invalid Order ID";
			  return $errobj;
					
				}	
				
			
		}

 if(!is_numeric($orderid))
    {
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "< Order id> accepts only numerical values.";
    	return $errobj;
    }
    
    if(!empty($uid) && !empty($orderid))
    {
    	$orderId_select=db_select('uc_orders','ord');
    	$orderId_select->addField('ord','order_id','ord_id');
    	$orderId_select->condition('ord.uid',$uid,'=');
    	$orderId_select->condition('ord.order_id',$orderid,'=');
    	$orderId=$orderId_select->execute()->fetchField();
    	if(empty($orderId))
    	{
    		$errobj=new stdClass();
    		$errobj->isValidateError = 1;
    		$errobj->errcode = 'L_012';
    		$errobj->errormsg = "Order ID does not match with entered Learner ID";
    		return $errobj;
    	}
    }

	
	$select = db_select('uc_orders', 'uod');
	$select->addField('uod', 'order_status','order_status'); 
	$select->addField('uod', 'order_id','order_id');
	$select->addField('uod', 'uid','learner_id');
	$select->condition('uod.uid',$uid , '=');	
	$select->condition('uod.order_id',$orderid , '=');	
	$orderRes = $select->execute()->fetchAll();
	
	expDebug::dPrint(' $totalRecords 1 = ' . print_r($announceRes[0]->order_status, true) , 4);
	
	$result->status = $orderRes[0]->order_status;
	$result->order_id = $orderRes[0]->order_id;
	$result->learner_id = $learner_id;
	
	return $result;
	}
	catch (Exception $ex) {
	watchdog_exception('getOrderStatusApi', $ex);
	expertusErrorThrow($ex);
	}
	
	
}

function getOrderDetailsApi(){
	
	
	try{
	$learner_id  = 	$_REQUEST['learner_id'];
	$user_id = 	$_REQUEST['userid'];
	$orderid = $_REQUEST['order_id'];
	$uid = getDrupalIdOfUser($learner_id);
	
	 if(!is_numeric($learner_id))
    {
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "<learner id> accepts only numerical values";
    	return $errobj;
    }

$learner_status=getPersonDetails($learner_id,array('id','status'));

if(empty($learner_status))
                                        {
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "Invalid Learner ID";
					return $errobj;
                                        }
										
if ($orderid == 0){
											
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->field = 'orderid';
				$errobj->errormsg = "Invalid Order ID";
				return $errobj;	
											
										}
										
if (!empty ($orderid)){
			  $selectQuery = db_select('uc_orders', 'ud');
			  $selectQuery->addField('ud', 'order_id','id');
			  $selectQuery->condition('ud.order_id', $orderid, '=');
			  $res = $selectQuery->execute()->fetchAll();
		if (empty ($res)){
			  $errobj->isValidateError = 1;
			  $errobj->errcode = 'L_012';
			  $errobj->field = 'orderid';
		      $errobj->errormsg = "Invalid Order ID";
			  return $errobj;
					
				}	
				
			
		}

 if(!is_numeric($orderid))
    {
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "<Order id> accepts only numerical values.";
    	return $errobj;
    }
    
    if(!empty($uid) && !empty($orderid))
    {
    	$orderId_select=db_select('uc_orders','ord');
    	$orderId_select->addField('ord','order_id','ord_id');
    	$orderId_select->condition('ord.uid',$uid,'=');
    	$orderId_select->condition('ord.order_id',$orderid,'=');
    	$orderId=$orderId_select->execute()->fetchField();
    	if(empty($orderId))
    	{
    		$errobj=new stdClass();
    		$errobj->isValidateError = 1;
    		$errobj->errcode = 'L_012';
    		$errobj->errormsg = "Order ID does not match with entered Learner ID";
    		return $errobj;
    	}
    }
	
    $order_details_select=db_select('uc_orders','uod');
    $order_details_select->leftJoin('uc_countries', 'con', 'con.country_id = uod.billing_country');
    $order_details_select->leftJoin('uc_zones', 'zon', 'zon.zone_id = uod.billing_zone');
    $order_details_select->leftJoin('uc_order_line_items', 'uli', 'uli.order_id = uod.order_id and uli.type=\'uc_discounts\'');
    $order_details_select->leftJoin('uc_order_line_items', 'uol', 'uod.order_id = uol.order_id and uol.type=\'cybersource_tax\'');
    $order_details_select->addField('uod', 'order_status','order_status');
    $order_details_select->addExpression('format(uod.order_total,2)','order_total');
    $order_details_select->addField('uod', 'billing_first_name','first_name');
    $order_details_select->addField('uod', 'billing_last_name','last_name');
    $order_details_select->addField('uod', 'billing_street1','address_line1');
    $order_details_select->addField('uod', 'billing_street2','address_line2');
    $order_details_select->addField('uod', 'billing_city','city');
    $order_details_select->addField('zon', 'zone_name','state');
    $order_details_select->addField('uod', 'billing_postal_code','zip_code');
    $order_details_select->addField('con', 'country_name','country');
    $order_details_select->addField('uod', 'billing_phone','phone_no');
    $order_details_select->addExpression('format(replace(sum(uli.amount),\'-\',\'\'),2)','discount_amount');
    $order_details_select->addExpression('format(uol.amount,2)','tax_amount');
    $order_details_select->condition('uod.order_id',$orderid ,'=');
    $order_details_select->condition('uod.uid',$uid ,'=');
    $order_details_select->groupby('uli.order_id');
    $orderDetails=$order_details_select->execute()->fetchAll();
    expDebug::dPrintDBAPI('$order_details_select',$order_details_select);
    expDebug::dPrint('yuvnannnnnnn' . print_r($orderDetails, true) , 4);
    
    $enrollment_select=db_select('uc_orders','uod');
    $enrollment_select->innerJoin('slt_order', 'sod', 'sod.uc_order_id = uod.order_id');
    $enrollment_select->innerJoin('slt_enrollment', 'enr', 'enr.order_id = sod.id and enr.master_enrollment_id is null');
    $enrollment_select->innerJoin('slt_course_class', 'cls', 'cls.id = enr.class_id');
    $enrollment_select->innerJoin('slt_profile_list_items', 'prof', 'prof.code = cls.entity_type');
    $enrollment_select->innerJoin('slt_profile_list_items', 'prof1', 'prof1.code = cls.delivery_type');
    $enrollment_select->addField('enr','class_id','entity_id');
    $enrollment_select->addField('enr','id','enrollment_id');
    $enrollment_select->addField('prof','name','entity_type');
    $enrollment_select->addField('prof1','name','delivery_type');
    $enrollment_select->addField('cls','title','title');
    $enrollment_select->addField('cls','code','entity_code');
    $enrollment_select->addField('cls','short_description','description');
    $enrollment_select->addField('cls','price','price');
    $enrollment_select->addField('cls','currency_type','currency_type');
    $enrollment_select->condition('uod.order_id',$orderid ,'=');
    $enrollment_select->condition('uod.uid',$uid ,'=');
  //  $orderDetails_class=$enrollment_select->execute()->fetchAll();
   // expDebug::dPrintDBAPI('$$enrollment_select',$enrollment_select);
    //expDebug::dPrint('yuvnannnnnnn' . print_r($orderDetails_class, true) , 4);
   
    
    $master_enrollment_select=db_select('uc_orders','uod');
    $master_enrollment_select->innerJoin('slt_order', 'sod', 'sod.uc_order_id = uod.order_id');
    $master_enrollment_select->innerJoin('slt_master_enrollment', 'mas', 'mas.order_id = sod.id');
    $master_enrollment_select->innerJoin('slt_program', 'pgm', 'pgm.id = mas.program_id');
    $master_enrollment_select->innerJoin('slt_profile_list_items', 'prof', 'prof.code = pgm.object_type');
    $master_enrollment_select->addField('mas','program_id','entity_id');
    $master_enrollment_select->addField('mas','id','enrollment_id');
    $master_enrollment_select->addField('pgm','type','entity_type');
    $master_enrollment_select->addField('prof','name','delivery_type');
    $master_enrollment_select->addField('pgm','title','title');
    $master_enrollment_select->addField('pgm','code','entity_code');
    $master_enrollment_select->addField('pgm','short_desc','description');
    $master_enrollment_select->addField('pgm','price','price');
    $master_enrollment_select->addField('pgm','currency_type','currency_type');
    $master_enrollment_select->condition('uod.order_id',$orderid ,'=');
    $master_enrollment_select->condition('uod.uid',$uid ,'=');
   // $orderDetails_program=$master_enrollment_select->execute()->fetchAll();
    //expDebug::dPrintDBAPI('$$master_enrollment_select',$master_enrollment_select);
    //expDebug::dPrint('yuvnannnnnnn' . print_r($orderDetails_program, true) , 4);
    
      	$order_items_select=$enrollment_select->union($master_enrollment_select,'UNION ALL');
    	$order_items_sel=db_select($order_items_select,'res');
    	$order_items_sel->addField('res','entity_type');
    	$order_items_sel->addField('res','entity_id');
    	$order_items_sel->addField('res','delivery_type');
    	$order_items_sel->addField('res','price');
    	$order_items_sel->addField('res','currency_type');
    	$order_items_sel->addField('res','entity_code');
    	$order_items_sel->addField('res','title');
    	$order_items_sel->addField('res','description');
    	$order_items_sel->addField('res','enrollment_id');
      	$totalRecords = $order_items_sel->execute()->fetchAll();
      	$total_items=$order_items_sel->execute()->rowCount();
    	expDebug::dPrintDBAPI(' $$selectStmt = ',$order_items_sel);
    
    $result[0]->order_id=$orderid;
    $result[0]->learner_id=$learner_id;
    $result[0]->order_details=$orderDetails;
    $result[0]->total_items=$total_items;
    $result[0]->order_items=$totalRecords;
    
    
	return $result;
	
	}
	catch (Exception $ex) {
	watchdog_exception('getOrderDetailsApi', $ex);
	expertusErrorThrow($ex);
	}
	
	
}

?>