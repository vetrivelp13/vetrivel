<?php

/**
 * LnrSearchMyteam
 *
 * Myteam(Manager View) Search, which is used to search the User and Assign training to the users.
 * Also have option to filter search using Report(Direct/InDirect), Country, Location and Username.
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 01-Nov-2011
 */
class LnrSearchMyteam{
  /**
   * An indentifier
   * @var string
   */
  private $select = null;
  private $user_role = null;
  public $limit;
  public $page;
  public $start = 0;
  public $total_pages;

  public $country_name_distinct = array();
  public $full_search = array();

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
  public function setParamVal($index_var){
  	try{
    return isset($_GET[$index_var]) ? $_GET[$index_var]:'';
    }catch (Exception $ex) {
    	watchdog_exception('setParamVal', $ex);
    	expertusErrorThrow($ex);
    }
  }

  function searchTitle($title) {
  	try{
    if (!empty($title)){
      $this->select->condition(db_and()->
      condition('per.full_name', '%' . db_like($title) . '%', 'LIKE'));
    }
    }catch (Exception $ex) {
    	watchdog_exception('searchTitle', $ex);
    	expertusErrorThrow($ex);
    }
  }

  function searchCountry($cy_type) {
  	try{
    if (!empty($cy_type)){
      $country = explode("|",$cy_type);
      $this->select->condition(db_and()->
      condition('per.country',$country,'IN')
      );
    }
    }catch (Exception $ex) {
    	watchdog_exception('searchCountry', $ex);
    	expertusErrorThrow($ex);
    }
  }

  function searchLocation($location) {
  	try{
    if (!empty($location)){
    $this->select->condition(db_and()->
        condition('per.city', '%' . db_like($location) . '%', 'LIKE'));
    }
    }catch (Exception $ex) {
    	watchdog_exception('searchLocation', $ex);
    	expertusErrorThrow($ex);
    }
  }

  function searchUsername($username) {
  	try{
    if (!empty($username)){
    $this->select->condition(db_or()->
        condition('per.user_name', '%' . db_like($username) . '%', 'LIKE')->condition('per.full_name', '%' . db_like($username) . '%', 'LIKE'));
    }
    }catch (Exception $ex) {
    	watchdog_exception('searchUsername', $ex);
    	expertusErrorThrow($ex);
    }
  }
  
  function searchReportType($reporttype) {
  	try{
// Fix for #0039846: Issue with Virtual and Direct report filters in Team view
  		$vLMSUserId = getSltpersonUserId();
    if (!empty($reporttype)){      
        if($reporttype == 'direct'){
          $this->select->condition('per.manager_id',$vLMSUserId);          
        }else if($reporttype == 'virtual'){
          //$this->select->where('FIND_IN_SET( '.$vLMSUserId.', per.dotted_mngr_id )');
          $this->select->condition('per_oth_mng.manager_id',$vLMSUserId);
        }else{
        	$this->select->condition(db_or()->condition('per.manager_id', $vLMSUserId)->condition('per_oth_mng.manager_id',$vLMSUserId));
        	//$this->select->condition(db_or()->condition('per.manager_id',$vLMSUserId)->where('FIND_IN_SET( '.$vLMSUserId.', per.dotted_mngr_id )'));
        }        
    }else{
    	$this->select->condition(db_or()->condition('per.manager_id', $vLMSUserId)->condition('per_oth_mng.manager_id',$vLMSUserId));
    	//$this->select->condition(db_or()->condition('per.manager_id',$vLMSUserId)->where('FIND_IN_SET( '.$vLMSUserId.', per.dotted_mngr_id )'));
    }
    }catch (Exception $ex) {
    	watchdog_exception('searchReportType', $ex);
    	expertusErrorThrow($ex);
    }
  }
  

  function setSearchParam($titleSrch=false){
  	try{
    if($titleSrch)
      $this->searchTitle($this->setParamVal('full_name'));    
    $this->searchCountry($this->setParamVal('cy_type'));
    $this->searchLocation($this->setParamVal('location'));
    $this->searchUsername($this->setParamVal('username'));
    $this->searchReportType($this->setParamVal('reporttype'));
    $this->searchUsername($this->setParamVal('searchusername'));
    }catch (Exception $ex) {
    	watchdog_exception('setSearchParam', $ex);
    	expertusErrorThrow($ex);
    }
  }

  function calculatePagination() {
  	try{
    $this->qry_search_count = 0;
    $this->qry_search_count = count($this->full_search);
    if ( $this->qry_search_count >0 ) {
      $this->total_pages = ceil($this->qry_search_count/$this->limit);
    } else {
      $this->total_pages = 0;
    }
    if ($this->page > $this->total_pages) $this->page=$this->total_pages;
    $this->start = $this->limit*$this->page - $this->limit;
    if ($this->start < 0) {
      $this->start = 0;
    }
    }catch (Exception $ex) {
    	watchdog_exception('calculatePagination', $ex);
    	expertusErrorThrow($ex);
    }
  }

  function searchResultsQry(){
  	try {
    $vLMSUserId = getSltpersonUserId();
    $this->select = db_select('slt_person', 'per');
    $this->select->distinct();
    $this->select->leftJoin('slt_person_other_manager', 'per_oth_mng', 'per_oth_mng.user_id=per.id and per_oth_mng.is_direct=\'N\'');
    $this->select->leftJoin('users', 'u', 'per.user_name = u.name');
    $this->select->leftJoin('file_managed', 'f', 'u.picture = f.fid');
    $this->select->leftJoin('slt_profile_list_items', 'pli', 'per.job_title = pli.code');
    $this->select->leftjoin('slt_country', 'sc', 'sc.country_code= per.country');
    $this->select->leftjoin('slt_state', 'st', 'st.country_code= per.country AND st.state_code=per.state');
    //$this->select->addExpression('IF( FIND_IN_SET('.$vLMSUserId.',per.dotted_mngr_id) > 0, 1, 0)', 'dotted_manager');
    $this->select->addExpression('IF((select count(*) from slt_person_other_manager as spom1 where spom1.manager_id = '.$vLMSUserId.' and spom1.is_direct=\'N\' and per.id = spom1.user_id ) > 0, 1, 0)', 'dotted_manager');
    $this->select->addField('per', 'id', 'user_id');
    $this->select->addField('per', 'user_name', 'username');
    $this->select->addField('per','first_name', 'first_name');
    $this->select->addField('per','last_name', 'last_name');
    $this->select->addField('per','middle_name', 'middle_name');
    $this->select->addField('per','full_name', 'full_name');
    $this->select->addField('per','city', 'user_city');
    //$this->select->addField('per','state', 'user_state');
    $this->select->addField('st','state_name', 'user_state');
    $this->select->addField('f', 'uri', 'user_picture');
    $this->select->addField('pli', 'name', 'job_title');
    $this->select->addField('sc','country_name', 'country_name');
    $this->select->addField('sc','country_code', 'country_code');
    $this->select->addField('u', 'uid', 'drup_user_id');
    //$this->select->addField('enr', 'cls_title', 'class_title');
    //$this->select->addExpression('CONCAT(per.location_name, SPACE(1), per.city))', 'user_location');
    $this->select->condition('per.status','cre_usr_sts_atv');
    //$this->select->condition(db_or()->condition('per.manager_id',$vLMSUserId)->where('FIND_IN_SET( '.$vLMSUserId.', per.dotted_mngr_id )'));
    //$this->select->condition('per.manager_id',$vLMSUserId);
    //$this->select->groupBy('enr.id');
    }catch (Exception $ex) {
    	watchdog_exception('searchResultsQry', $ex);
    	expertusErrorThrow($ex);
    }
  }
  function setSearchResults(){
  	try{
    $this -> searchResultsQry();
    $this -> setSearchParam(true);
    $this -> setSearchFilter();
    $this -> calculatePagination();
    }catch (Exception $ex) {
    	watchdog_exception('setSearchResults', $ex);
    	expertusErrorThrow($ex);
    }
  }
  
  function setFullSearchResults(){
  	try{
    $this -> searchResultsQry();    
    $this -> setSearchParam();
    $this -> setSearchFilter();
    }catch (Exception $ex) {
    	watchdog_exception('setFullSearchResults', $ex);
    	expertusErrorThrow($ex);
    }
  }

  function getSearchResults() {
  	try{
    $this -> setSearchResults();
    return $this -> getSearchList();
    }catch (Exception $ex) {
    	watchdog_exception('getSearchResults', $ex);
    	expertusErrorThrow($ex);
    }
  }

  function searchSortBy($sortby) {
  	try{
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
    }catch (Exception $ex) {
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
  	try{
    $this->searchSortBy($this->setParamVal('sortby'));
    $this->select->range($this->start, $this->limit);
    return $this->select->execute()->fetchAll();
    }catch (Exception $ex) {
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
  function setSearchFilter(){
  	try{
    $country_name=array();
    $full_Select = $this->select;
    $this->full_search = $full_Select->execute()->fetchAll();
    foreach($this->full_search as $key => $val){
      if(trim($val->country_name)!=""){
        $country_name[$val->country_code] = $val->country_name;
      }
    }
    $this->country_name_distinct = array_unique($country_name);
    }catch (Exception $ex) {
    	watchdog_exception('setSearchFilter', $ex);
    	expertusErrorThrow($ex);
    }
  }
  
} //Class LnrSearchCatalog End

function getMyteamClassResults($userId){
	try{
  /*$select = db_select('slv_myenrollment', 'enr');
  $select->addField('enr', 'cls_title', 'class_title');
  $select->addField('enr', 'reg_date', 'enrolled_on');
  $select->addField('enr', 'comp_date', 'completed_on');
  $select->addField('enr', 'reg_status', 'status');
  $select->addField('enr', 'delivery_type', 'type');
  $select->condition('enr.user_id',$userId);
  $select->groupBy('enr.id');
  $qry_search_results = $select->execute()->fetchAll();*/
	$query = "SELECT enr.cls_title AS class_title, enr.reg_date AS enrolled_on, enr.comp_date AS completed_on, ";
	$query .= " enr.reg_status AS status, enr.delivery_type AS type FROM ( ";
  $query .= getEnrollmentQuery($userId);
  $query .= " ) enr ";
  $query .= " GROUP BY enr.id ";
  expDebug::dPrint("My Team getMyteamClassResults Query :-");
  expDebug::dPrintDBAPI("My Team getMyteamClassResults Query :-", $query);
  $select = db_query($query);
  $qry_search_results = $select->fetchField();
  expDebug::dPrint($qry_search_results , 3);
  $srch_result_response = theme('exp_sp_myteam-class-detail', array('results' => $qry_search_results));
  echo drupal_json_output($srch_result_response);
  }catch (Exception $ex) {
  	watchdog_exception('getMyteamClassResults', $ex);
  	expertusErrorThrow($ex);
  }
}
/**
 * Get results for Location autocomplete.
 *
 * @param string ''
  *
 * @return
 *   Auto Complete Location Results
 */
function getMyteamLocationAutoComplete(){
	try{
  $userId = getSltpersonUserId();
  $location = $_GET['z'];
  $select = db_select('slt_person', 'per');
  $select->leftJoin('slt_person_other_manager', 'per_oth_mng', 'per_oth_mng.user_id=per.id and per_oth_mng.is_direct=\'N\'');
  $select->addField('per', 'city');
  $select->condition('per.city', '%' . db_like($location) . '%', 'LIKE');
  //$select->condition('per.manager_id',$userId,'=');
  $select->condition(db_or()
                          ->condition('per.manager_id',$userId,'=')
                          ->condition('per_oth_mng.manager_id',$userId,'=')                       
                    );
  $select->groupBy('per.city');
  $qry_loc_autocomplete = $select->execute()->fetchAll();
  foreach ($qry_loc_autocomplete as $rec_loc_autocomplete) {
    print $rec_loc_autocomplete->city."\n";
  }
  }catch (Exception $ex) {
  	watchdog_exception('getMyteamLocationAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}
/**
 * Get results for Username autocomplete.
 *
 * @param string ''
  *
 * @return
 *   Auto Complete Username Results
 */
function getMyteamUsernameAutoComplete(){
	try{
	$userId = getSltpersonUserId();
  $username = $_GET['z'];
  $select = db_select('slt_person', 'per');
  $select->leftJoin('slt_person_other_manager', 'per_oth_mng', 'per_oth_mng.user_id=per.id and per_oth_mng.is_direct=\'N\'');
  $select->addField('per', 'full_name');
  $select->addField('per', 'manager_id');
  //$select->addField('per', 'dotted_mngr_id');
  $select->addExpression('per_oth_mng.manager_id', 'dotted_mngr_id');
  $select->condition(db_or()->condition('per.user_name', '%' . db_like($username) . '%', 'LIKE')->condition('per.full_name', '%' . db_like($username) . '%', 'LIKE'));
  $select->condition(db_or()
                          ->condition('per.manager_id',$userId,'=')
                          ->condition('per_oth_mng.manager_id',$userId)
                    );
  $select->groupBy('per.id');
  expDebug::dPrintDBAPI("Get User id:-",$select);
  $qry_user_autocomplete = $select->execute()->fetchAll();
  foreach ($qry_user_autocomplete as $rec_user_autocomplete) {
    print $rec_user_autocomplete->full_name."\n";
  }
  }catch (Exception $ex) {
  	watchdog_exception('getMyteamUsernameAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}

function getMyteamApprovalUsernameAutoComplete(){
	try{
		$userId = getSltpersonUserId();
		$username = $_GET['z'];
		$select = db_select('slt_skill_set', 'skill');
		$select->innerJoin('slt_person', 'per', 'per.user_name = skill.user_name');
		$select->leftJoin('slt_person', 'persn', 'persn.id = skill.user_id');
		$select->addField('persn', 'full_name');
		$select->condition('per.id',$userId,'=');
		$select->condition('persn.full_name', '%' . db_like($username) . '%', 'LIKE');
		$select->groupBy('per.id');
		expDebug::dPrintDBAPI("Get User id:-",$select);
		$qry_user_autocomplete = $select->execute()->fetchAll();
		foreach ($qry_user_autocomplete as $rec_user_autocomplete) {
			print $rec_user_autocomplete->full_name."\n";
		}
	}catch (Exception $ex) {
		watchdog_exception('getMyteamApprovalUsernameAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}
/**
 * @method CheckMyteamPrequiste() - Get Prequiste details For Both Tp and Class.
 * @author ayyappans
 * @param $userId - leaner id
 * @param $catalogId - course/tp id to get prereq
 * @param $tpType - class/course/tp
 * @param $fromPage - MyTeam
 */
function CheckMyteamPrequiste($userId,$catalogId,$tpType,$fromPage){
	try{
		// Include Files For Prequiste
		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_prerequisite/exp_sp_prerequisite.module');
		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_prerequisite/exp_sp_prerequisite.inc');
		// Get the Basic Prequiste From System Catalog
		$result = getCatalogTpPrerequisites($catalogId,($tpType == "class") ? 'initClassPrereq' : 'initPrereq','','',$fromPage,$userId);
		return drupal_json_output($result);
	}catch (Exception $ex) {
		watchdog_exception('CheckMyteamPrequiste', $ex);
		expertusErrorThrow($ex);
	}
}


/**
 * @method fetchPrequisteList() - Fetch results for Prequiste For Both Tp and Class.
 * @param $userId - learner id
 * @param $type - class/course/tp
 * @param $catalogId - course/tp id to get prereq
 * @param $preLevel - l1/l2/l3
 * @param $fromPage - MyTeam
 * @param $widgetId - l1/l2/l3
 */
function fetchPrequisteList($userId, $type, $catalogId, $preLevel, $fromPage, $widgetId){
	try{
		// Include Files For Prequiste
		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_prerequisite/exp_sp_prerequisite.inc');
		$result = getCatalogTpPrerequisites($catalogId, $type, $widgetId, $preLevel, $fromPage,$userId);
		return drupal_json_output($result);
	}catch (Exception $ex) {
		watchdog_exception('CheckMyteamPrequiste', $ex);
		expertusErrorThrow($ex);
	}
}
 




function ManagerVerifyCetificate($certId,$status){
	try{
		include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_my_skill/exp_sp_my_skill.inc";
		expDebug::dPrint('getClassDelType $$delType: sandhyaaaa '.print_r($certId, true) , 3);
		expDebug::dPrint('getClassDelType $$delType: sandhyaaaa '.print_r($status, true) , 3);
		
		$cert_expiry_reminder = isset($config["Certificate_verification_expiry"]) ? $config["Certificate_verification_expiry"] : 10;
		
		$select = db_select('slt_skill_set','skill');
		$select->addField('skill','id');
		$select->condition('skill.verification_status',"Verification pending",'=');
		$select->condition('skill.id',$certId,'=');
		$select->isNull('skill.is_deleted');
		$select->where("DATEDIFF(DATE_ADD(skill.created_on, INTERVAL $cert_expiry_reminder DAY),NOW()) >=0");
		$rowCount = $select->execute()->fetchField();
		expDebug::dPrintDBAPI(' query for default language' ,$select);
		expDebug::dPrint('add_new_skill_details sanhdd: '.print_r($rowCount,true) , 5);
		if($rowCount > 0){
		$numUpdated = db_update('slt_skill_set')
		->fields(array(
				'verification_status' => $status,
		        'updated_on' => NOW()))
		->condition('id', $certId)
		->execute();
		expDebug::dPrint('$numUpdated = ', $numUpdated);
		
		$verifier_id = getSltpersonUserId();
		
		$qry = "select user_id from slt_skill_set where id= $certId order by id desc";
		expDebug::dPrint('add_new_skill_details aaaaaaaaaaaaaaaaaaaaaaaa: '.print_r($qry,true) , 5);
		$userId = db_query($qry)->fetchField();
		
		insertNotification('CertificateVerificationStatus', $certId, $verifier_id, $userId, $userId);
		}
		
		
	}catch (Exception $ex) {
		watchdog_exception('CheckMyteamPrequiste', $ex);
		expertusErrorThrow($ex);
	}
}

?>
