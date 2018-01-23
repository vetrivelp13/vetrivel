<?php
/**
 * classSearchPagination
 *
 * classSearchPagination which is used in add/edit course section
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2012, ExpertUs
 * @since 17-Aug-2012
 */
class classSearchPagination{
  /**
   * An indentifier
   * @var string
   */
  private $select = null;
  public $limit;
  public $page;
  public $start = 0;
  public $total_pages;

  public $country_name_distinct = array();
  public $full_search = array();

  function __construct() {
  	try {
    if(!empty($_GET['rows']))
    $this->limit     = $_GET['rows'];
    if(!empty($_GET['page']))
    $this->page      = $_GET['page'];
    //$this->user_role = '';
    } catch (Exception $ex) {
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
  public function setParamVal($index_var){
  	try {
    return isset($_GET[$index_var]) ? $_GET[$index_var]:'';
    } catch (Exception $ex) {
    	watchdog_exception('setParamVal', $ex);
    	expertusErrorThrow($ex);
    }
  }

  function calculatePagination() {
  	try {
  	$full_Select = $this->select;
    $this->qry_search_count = 0;
    $this->qry_search_count = $full_Select->execute()->fetchField();
    if ( $this->qry_search_count >0 ) {
      $this->total_pages = ceil($this->qry_search_count/$this->limit);
    } else {
      $this->total_pages = 0;
    }
    if ($this->page > $this->total_pages) $this->page=$this->total_pages;
    //$this->start = $this->limit*$this->page - $this->limit;
    // Added by Vincent on 02, Jan 2014 for #0029687: Refresh on Admin Enrollments page
    $this->start = getOffsetOfFirstRecordInPage($this->page, $this->total_pages, $this->limit, $this->qry_search_count,false);
    if ($this->start < 0) {
      $this->start = 0;
    }
    } catch (Exception $ex) {
    	watchdog_exception('calculatePagination', $ex);
    	expertusErrorThrow($ex);
    }
  }
function getAdministrationClassSearchDetails($courseId,$classId,$cnt) {
  try {
  		$this->select = db_select("slt_course_class","cls");
	    $this->select->innerJoin('slt_profile_list_items','prof','prof.code = cls.status ');
	    $this->select->innerJoin('slt_profile_list_items','prof1','prof1.code = cls.delivery_type ');
	    $this->select->innerJoin('slt_profile_list_items','prof2','prof2.code = cls.lang_code ');
	    $this->select->leftJoin('slt_course_class_session', 'ses', 'cls.id=ses.class_id and ses.id=(SELECT id FROM  slt_course_class_session WHERE class_id=cls.id ORDER BY start_date LIMIT 1)');
	    $this->select->leftJoin('slt_facility', 'fac', 'ses.facility_id= fac.id');
	    $this->select->leftJoin('slt_tag_entity','te','((cls.id=te.entity_id and te.entity_type=\'Class\') OR (cls.course_id=te.entity_id and te.entity_type=\'Course\'))');
	    $this->select->leftJoin('slt_tagdefn','td','te.tagid= td.id');
  	if($cnt=='count'){
  		$this->select->addExpression('COUNT(Distinct(cls.id))','clscount');
  		//$this->select->addExpression('count(1)','clscount');
  		$this->select->condition('cls.course_id',$courseId,'=');	
		  $this->select->condition('cls.status', 'lrn_cls_sts_del' ,'!=');
		  //$this->select->addField('td','tagname', 'tagname');
  	}else{
	    $this->select->addField('cls','id','cls_id');
	    $this->select->addField('cls','title','cls_title');
	    $this->select->addField('prof','name','cls_status');
	    $this->select->addField('prof1','name','cls_delivery_type');
	    $this->select->addField('cls','code','cls_code');
	    $this->select->addField('prof2', 'name','cls_language');
	    $this->select->addField('fac', 'name','facility_name');
	    $this->select->addField('cls','delivery_type','cls_delivery_type_code');
	    $this->select->addField('cls','created_on','created_on');
	    //$this->select->addField('td','tagname', 'tagname');
	    $this->select->condition('cls.course_id',$courseId,'=');	
		  $this->select->condition('cls.status', 'lrn_cls_sts_del' ,'!=');
		  if($this->start==0 && !empty($classId) && $classId!=0){
		  	$this->select->condition('cls.id',$classId,'=');
		  }else{
		  	$this->select->condition('cls.id',$classId,'!=');
		  }
  	}
  	filterByUserAccessPrivileges($this->select, 'cls','',1,'cre_sys_obt_cls','Courseedit','cls'); // Added $createByAdmin parameter 'cls' for # 0050368
  	if($_GET['class_name'] == t('LBL766') || $_GET['class_name'] == '' || $_GET['class_name'] == t('LBL036').' '.t('Class').' '.t('LBL102')  || $_GET['class_name'] == t('LBL036').' '.t('LBL084')){
  	  
  	}
  	elseif($_GET['search_Type'] == 'clstit' && !empty($_GET['class_name'])) {
  	  //$this->select->condition('cls.title','%'.$_GET['class_name'].'%','like');
  	  $this->select->condition(db_or()->condition('cls.title', '%' . db_like($_GET['class_name']) . '%' ,'like')
  	                             ->condition('cls.code', '%' . db_like($_GET['class_name']) . '%' ,'like')
  	  							 ->condition('td.tagname', '%' . db_like($_GET['class_name']) . '%', 'like')
  	  		); 
  	}
  	elseif ($_GET['search_Type'] == 'deltype' && !empty($_GET['class_name'])) {
  	  $allDeliveryTypes = getProfileItemNamesKeyed('lrn_cls_dty_');
  	  expDebug::dPrint('$allDeliveryTypes = ' . print_r($allDeliveryTypes, true), 4);
  	  $searchDeliveryTypesList = array();
  	  foreach ($allDeliveryTypes as $code => $name) {
  	  	if (mb_stripos($name, $_GET['class_name'], null, 'UTF-8') !== false) {
  	  		$searchDeliveryTypesList[] = $code;
  	  	}
  	  }
  	  expDebug::dPrint('$searchDeliveryTypesList = ' . print_r($searchDeliveryTypesList, true), 4);
  	  if (!empty($searchDeliveryTypesList)) {
        $this->select->condition('cls.delivery_type', $searchDeliveryTypesList, 'IN');
  	  }
  	  else {
  	  	$this->select->where('1=0');
  	  }
  	}
  	elseif ($_GET['search_Type'] == 'clsstatus' && !empty($_GET['class_name'])) {
  		$allClassStatus = getProfileItemNamesKeyed('lrn_cls_sts_');
      $searchStatusList = array();
      foreach ($allClassStatus as $code => $name) {
        if (mb_stripos($name, $_GET['class_name'], null, 'UTF-8') !== false) {
          $searchStatusList[] = $code;
        }
      }
      expDebug::dPrint('$searchStatusList = ' . print_r($searchStatusList, true), 4);
      if (!empty($searchStatusList)) {
        $this->select->condition('cls.status', $searchStatusList, 'IN');
      }
      else {
        $this->select->where('1=0');
      } 	  
  	}
  	/*if(isset($_GET['class_name']) && !empty($_GET['class_name'])){
   	  $this->select->condition('cls.title','%'.$_GET['class_name'].'%','like');
  	}*/
    //$crsList = $select->execute()->fetchAll();
    //return $crsList;
  } catch (Exception $ex) {
    watchdog_exception('getAdministrationClassSearchDetails', $ex);
    expertusErrorThrow($ex);
  } 
 }

 function getAdministrationClassSearchDetailsUnion($courseId,$classId,$cnt){
 	try{
 		$select1 = db_select("slt_course_class","cls");
    $select1->innerJoin('slt_profile_list_items','prof','prof.code = cls.status ');
    $select1->innerJoin('slt_profile_list_items','prof1','prof1.code = cls.delivery_type ');
    $select1->innerJoin('slt_profile_list_items','prof2','prof2.code = cls.lang_code ');
    $select1->leftJoin('slt_course_class_session', 'ses', 'cls.id=ses.class_id and ses.id=(SELECT id FROM  slt_course_class_session WHERE class_id=cls.id ORDER BY start_date LIMIT 1)');
    $select1->leftJoin('slt_facility', 'fac', 'ses.facility_id= fac.id');
    $select1->leftJoin('slt_tag_entity','te','((cls.id=te.entity_id and te.entity_type=\'Class\') OR (cls.course_id=te.entity_id and te.entity_type=\'Course\'))');
    $select1->leftJoin('slt_tagdefn','td','te.tagid= td.id');
    $select1->addField('cls','id','cls_id');
    $select1->addField('cls','title','cls_title');
    $select1->addField('prof','name','cls_status');
    $select1->addField('prof1','name','cls_delivery_type');
    $select1->addField('cls','code','cls_code');
    $select1->addField('prof2', 'name','cls_language');
    $select1->addField('fac', 'name','facility_name');
    $select1->addField('cls','delivery_type','cls_delivery_type_code');
    $select1->addField('cls', 'created_on','created_on');
   // $select1->addField('td','tagname', 'tagname');
    $select1->condition('cls.course_id',$courseId,'=');	
  	$select1->condition('cls.status', 'lrn_cls_sts_del' ,'!=');
  	$select1->condition('cls.id',$classId,'!=');
  	//Modified for #0047523
  	filterByUserAccessPrivileges($select1, 'cls','',1,'cre_sys_obt_cls','Courseedit','cls');
  	$select1->groupBy('cls.id');
  	return $select1;
  	} catch (Exception $ex) {
  		watchdog_exception('getAdministrationClassSearchDetailsUnion', $ex);
  		expertusErrorThrow($ex);
  	}
 }
  function setSearchResults($courseId,$classId){
  	try {
    $this -> getAdministrationClassSearchDetails($courseId,$classId);
    //$this -> setSearchParam(true);
    //$this -> setSearchFilter();
    $this -> calculatePagination();
    } catch (Exception $ex) {
    	watchdog_exception('setSearchResults', $ex);
    	expertusErrorThrow($ex);
    }
  }
  
  function setFullSearchResults(){
  	try {
    $this -> getAdministrationClassSearchDetails();    
    //$this -> setSearchParam();
    $this -> setSearchFilter();
    } catch (Exception $ex) {
    	watchdog_exception('setFullSearchResults', $ex);
    	expertusErrorThrow($ex);
    }
  }

  function getSearchResults($courseId,$classId) {
  	try {
  	$this -> getAdministrationClassSearchDetails($courseId,$classId,'count');
    $this -> calculatePagination();
    expDebug::dPrint('classid -->>'.$classId.'started-->>'.$this->start);
    if($this->start==0 && !empty($classId) && $classId!=0){
    	$select2 = $this -> getAdministrationClassSearchDetailsUnion($courseId,$classId,'count');
    	$this -> getAdministrationClassSearchDetails($courseId,$classId,'');
   		//$select2->orderBy('created_on','desc');
    	$select2->range($this->start, $this->limit);
    	$this->select->union($select2,'UNION ALL');
    	$this->select->groupBy('cls.id'); // Modified for #0047523
    }else{
    	$this -> getAdministrationClassSearchDetails($courseId,$classId,'');
    	$this->select->groupBy('cls.id');
    	$this->select->orderBy('cls.created_on','desc');
    	$this->select->range($this->start, $this->limit);
    	
    }
    
    
    return $this -> getSearchList();
    } catch (Exception $ex) {
    	watchdog_exception('getSearchResults', $ex);
    	expertusErrorThrow($ex);
    }
  }

  function searchSortBy($sortby) {
  	try {
    if ($sortby == "AZ") {
      $xorderby = "per.last_name, per.first_name";
      $xorderbylist = "asc";
    }else if ($sortby == "ZA") {
      $xorderby = "per.last_name desc, per.first_name desc";
      $xorderbylist="";
    }else if ($sortby == "Time") {
      $xorderby="per.updated_on";
      $xorderbylist="desc";
    }else {
      $xorderby="per.last_name, per.first_name";
      $xorderbylist="asc";
    }
    $this->select->orderBy($xorderby,$xorderbylist);
    } catch (Exception $ex) {
    	watchdog_exception('searchSortBy', $ex);
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
  function getSearchList(){
  	try {
    //$this->searchSortBy($this->setParamVal('sortby'));
    $result =  $this->select->execute()->fetchAll();
    expDebug::dPrintDBAPI("Class Search Query --->>> ",$this->select);
    return $result;
    } catch (Exception $ex) {
    	watchdog_exception('getSearchList', $ex);
    	expertusErrorThrow($ex);
    }
  }
  /**
   * Funtion to set the Filter Critria
   *
   * @param string ''
   *
   * @return
   * Set Filter Values
   */
  /*function setSearchFilter(){
    $country_name=array();
    $full_Select = $this->select;
    $this->full_search = $full_Select->execute()->fetchAll();
    foreach($this->full_search as $key => $val){
      if(trim($val->country_name)!=""){
        $country_name[$val->country_code] = $val->country_name;
      }
    }
    $this->country_name_distinct = array_unique($country_name);
  }*/
  
} //Class classSearchPagination End
?>