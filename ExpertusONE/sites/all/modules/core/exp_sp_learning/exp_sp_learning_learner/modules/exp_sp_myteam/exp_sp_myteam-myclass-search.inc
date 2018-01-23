<?php

/**
 * MyteamMyclassSearch
 *
 * Catalog Search, which is used to search the Class,Certification and Curricula
 * and have option to find various types of Delivery Type, Language, Country and Location
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 07-Nov-2011
 */
class MyteamMyclassSearch{
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
  public $userId;

  public $full_search = array();
  public  $sidx='';
  public  $sord='';
  
  function __construct() {
  	try{
    if(!empty($_GET['rows']))
    $this->limit     = $_GET['rows'];
    if(!empty($_GET['page']))
    $this->page      = $_GET['page'];
    //$this->user_role = '';
    if(!empty($_GET['sidx']))
      $this->sidx      = $_GET['sidx']; 
    if(!empty($_GET['sord']))
      $this->sord      = $_GET['sord']; 
    expDebug::dPrint("sidx ==>".$this->sidx);
    expDebug::dPrint("sord ==>".$this->sord);
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
  	try{
    return isset($_GET[$index_var]) ? $_GET[$index_var]:'';
    } catch (Exception $ex) {
    	watchdog_exception('setParamVal', $ex);
    	expertusErrorThrow($ex);
    }
  }
    
  function searchTitle($title) {
  	try{
    if (!empty($title)){
      $this->select->condition(db_and()->
      condition('class.title', '%' . db_like($title) . '%', 'LIKE'));
    }
    } catch (Exception $ex) {
    	watchdog_exception('searchTitle', $ex);
    	expertusErrorThrow($ex);
    }
  }
  
  function setSearchParam($titleSrch=false){
  	try{
    if($titleSrch)
      $this->searchTitle($this->setParamVal('class_title'));   
    } catch (Exception $ex) {
    	watchdog_exception('setSearchParam', $ex);
    	expertusErrorThrow($ex);
    } 
  }

  function calculatePagination() {
  	try{
    $this->qry_search_count = 0;
    $this->qry_search_count = $this->qry_paging_count;
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
    } catch (Exception $ex) {
    	watchdog_exception('calculatePagination', $ex);
    	expertusErrorThrow($ex);
    }
  }


  function searchResultsQry($isAPI = '',$startDate = '',$endDate = ''){
  	try{
    $userId  = $this->userId;    
    $this->select = db_select('slt_enrollment', 'enr');
    $this->select -> join('slt_profile_list_items', 'profile_status', 'profile_status.code = enr.reg_status');
    $this->select -> join('slt_course_class', 'class', 'enr.course_id = class.course_id AND class.id = enr.class_id');
    $this->select -> join('slt_profile_list_items', 'profile_delivery', 'profile_delivery.code = class.delivery_type');
    $this->select -> leftJoin('slt_master_enrollment','master_enr','enr.master_enrollment_id = master_enr.id');     
    // Below change done by Vincent for #0066758 on July 23, 2016
			// TODO: This should be changed in future once the flat structure is implement for performance tuning
    // RAM : this is Changes for This Ticket #0037832, when Myteam Assign and View Learning status is Not Showing, when a Access is Set For Course level.
   /* $classsrQuery = 'if((select count(1) from slt_group_mapping grp_map where grp_map.entity_id = class.id and grp_map.entity_type = \'cre_sys_obt_cls\')>0, 
    (group_map.entity_id = class.id and group_map.entity_type = \'cre_sys_obt_cls\'),(group_map.entity_id = class.course_id and group_map.entity_type = \'cre_sys_obt_crs\'))';*/
    $classsrQuery = '(group_map.parent_id=concat(class.course_id, \'-\', IFNULL(class.id,0)) AND group_map.parent_type = \'learning\')';
    $this->select-> leftJoin('slt_group_mapping','group_map','('.$classsrQuery.') and group_map.group_type = 0 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)'); 
  	// $this->select -> leftJoin('slt_group_mapping','group_map','group_map.entity_id =class.id and group_map.entity_type = \'cre_sys_obt_cls\' and group_map.group_type = 0');
    $this->select -> leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status = \'cre_sec_sts_atv\'');
    $this->select->addField('enr', 'id', 'enrolled_id');    
    $this->select->addField('enr', 'class_id', 'class_id');
    $this->select->addField('enr', 'course_id', 'course_id');
    $this->select->addField('enr', 'user_id', 'user_id');
    $this->select->addField('enr', 'reg_date', 'enrolled_on');
    $this->select->addField('enr', 'comp_date', 'completed_on');
    $this->select->addField('enr', 'comp_date', 'canceled_on');
    $this->select->addField('enr', 'created_on', 'created_on');
    $this->select->addField('enr', 'updated_on', 'updated_on');
    $this->select->addField('enr', 'reg_status', 'reg_status');
    $this->select->addField('enr', 'comp_status', 'comp_status');
    /* Changes for mandatory to be displayed for classes assigned by the manager after checking "mandatory" checkbox in assign learning ticket 16261 */
    $this->select->addField('enr', 'mandatory', 'mand_status');
    $this->select->addField('profile_status', 'name', 'status');    
    $this->select->addField('profile_delivery', 'name', 'type'); 
    $this->select->addField('profile_delivery', 'code', 'delivery_type_code');
    
    //$this->select->addField('profile_delivery', 'is_active', 'is_active');   
    $this->select->addField('class', 'title', 'class_title');
    $this->select->addField('group_map', 'mro', 'mro_status');
    $this->select->addField('class', 'price', 'classprice');
    if(isset($_REQUEST['apiname']) && ($_REQUEST['apiname'] == 'userclassesunderManagerAPI')){ 
	$this->select -> leftJoin('slt_profile_list_items','spli','spli.code= \'cre_sys_brl_mmc\'');
	$this->select->addField('spli', 'is_active', 'mgrmarkcomp');
    $this->select->addField('class', 'short_description', 'description');
	}
    $this->select->addExpression('null', 'current_id');
    $this->select->addExpression('profile_delivery.attr1', 'base_type');
    $this->select->addExpression('\'Class\'', 'learning_type');    
	$this->select->addExpression('(CASE WHEN comp_status=\'lrn_crs_cmp_cmp\' THEN \'Completed\' WHEN comp_status=\'lrn_crs_cmp_enr\' THEN \'Enrolled\' WHEN comp_status=\'lrn_crs_cmp_inp\' THEN \'InProgress\' WHEN comp_status=\'lrn_crs_cmp_inc\' THEN \'InComplete\' WHEN comp_status=\'lrn_crs_reg_can\' OR comp_status=\'lrn_crs_cmp_nsw\' THEN \'Cancelled\' WHEN comp_status=\'lrn_crs_reg_ppm\' THEN \'Pending\' WHEN comp_status=\'lrn_crs_reg_wtl\' THEN \'Waitlisted\' END)','sort_comp_status');    
    $this->select->addExpression('enr.is_compliance', 'is_compliance'); 
    $this->select->addExpression('(SELECT CONCAT(exmp.exempted_status,\'|\',exmp.id) FROM slt_enrollment_exempted exmp WHERE exmp.enrollment_id = enr.id AND exmp.enroll_type = \'class\' ORDER BY id DESC LIMIT 1)', 'exempted_sts');
    $this->select->condition('enr.user_id',$userId);
    
    if($isAPI == 'api')
    {
    	if(!empty($startDate) && !empty($endDate))
    	{
    	$db_or = db_or();
    	$db_or->condition('enr.reg_date', array($startDate, $endDate), 'BETWEEN');
    	$db_or->condition('enr.comp_date', array($startDate, $endDate), 'BETWEEN');
    	$this->select->condition($db_or);
    	}
    	
    	
    		if (!empty($_REQUEST['delivery_type_code'])){
    			$delivery_type = explode("|",$_REQUEST['delivery_type_code']);
    			$this->select->condition('profile_delivery.code', $delivery_type, 'IN');
    		}
    	
    	if (!empty($_REQUEST['reg_status'])){
    		$reg_status = explode("|",$_REQUEST['reg_status']);
    		if (!empty($_REQUEST['mobile_request'])){
    		$db_or = db_or();
    		$db_or->condition('enr.comp_status', $reg_status, 'IN');
    		$db_or->condition('enr.reg_status', $reg_status, 'IN');
    		$this->select->condition($db_or);
    		}
			else{ 		
				$this->select->condition('enr.comp_status', $reg_status, 'IN');
			}
    		
    	}
    	
    	
    	expDebug::dPrint('checking if class title exists '.$_REQUEST['cls_title']);
    	
    	if(!empty($_REQUEST['cls_title']))
    	{
    		$db_or = db_or();
    		$db_or->condition('class.title','%' . $_REQUEST['cls_title'] . '%' ,'LIKE');
    		$db_or->condition('class.code', '%' . $_REQUEST['cls_title'] . '%' ,'LIKE');
    		$this->select->condition($db_or);
    	}
    }   	
    
   
    $this->select->condition('enr.user_id',$userId);
    $this->select->where('IF(enr.master_enrollment_id IS NULL,1=1,enr.reg_date < master_enr.reg_date)');
    $this->select->condition('enr.reg_status', array('lrn_crs_reg_cnf', 'lrn_crs_reg_ppm', 'lrn_crs_reg_ppv', 'lrn_crs_reg_wtl','lrn_crs_reg_can'), 'IN');
    $this->select->groupBy('enr.id');
    
    
    	
    } catch (Exception $ex) {
    	watchdog_exception('searchResultsQry', $ex);
    	expertusErrorThrow($ex);
    }
  }
  
function searchLpResultsQry($isAPI = '',$startDate = '',$endDate = ''){
	try{ 
    $userId  = $this->userId;
    $this->selectProg = db_select('slt_master_enrollment', 'mas_enr');
    $this->selectProg -> join('slt_program', 'prog', 'mas_enr.program_id = prog.id');   
    $this->selectProg -> join('slt_profile_list_items', 'mas_profile_status', 'mas_profile_status.code = mas_enr.overall_status');
    $this->selectProg -> join('slt_profile_list_items', 'mas_profile_delivery', 'mas_profile_delivery.code = prog.object_type');
    $this->selectProg -> leftJoin('slt_group_mapping','group_map','group_map.entity_id = prog.id and group_map.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\') and group_map.group_type = 0');
    $this->selectProg -> leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status = \'cre_sec_sts_atv\'');
    $this->selectProg->addField('mas_enr', 'id', 'enrolled_id');
    $this->selectProg->addField('mas_enr', 'program_id', 'class_id');
    $this->selectProg->addField('mas_enr','program_id','course_id');
    $this->selectProg->addField('mas_enr', 'user_id', 'user_id');
    $this->selectProg->addField('mas_enr', 'reg_date', 'enrolled_on');
    $this->selectProg->addField('mas_enr', 'comp_date', 'completed_on');
    $this->selectProg->addField('mas_enr', 'cancel_date', 'canceled_on');
    $this->selectProg->addField('mas_enr', 'created_on', 'created_on');
    $this->selectProg->addField('mas_enr', 'updated_on', 'updated_on');
    $this->selectProg->addField('mas_enr', 'overall_status', 'reg_status');  
    $this->selectProg->addField('mas_enr', 'overall_status', 'comp_status');
    /* Changes for mandatory to be displayed for classes assigned by the manager after checking "mandatory" checkbox in assign learning ticket 16261 */
    $this->selectProg->addField('mas_enr', 'mandatory', 'mand_status');  
    $this->selectProg->addField('mas_profile_status', 'name', 'status');    
    $this->selectProg->addField('mas_profile_delivery', 'name', 'type'); 
    $this->selectProg->addField('mas_profile_delivery', 'code', 'delivery_type_code');
    //$this->selectProg->addField('mas_profile_delivery', 'is_active', 'is_active');   
    $this->selectProg->addField('prog', 'title', 'class_title');
    $this->selectProg->addField('group_map', 'mro', 'mro_status');
    $this->selectProg->addField('prog', 'price', 'classprice');
    if(isset($_REQUEST['apiname']) && ($_REQUEST['apiname'] == 'userclassesunderManagerAPI')){ 
	$this->selectProg -> leftJoin('slt_profile_list_items','spli','spli.code= \'cre_sys_brl_mmc\'');
	$this->selectProg->addField('spli', 'is_active', 'mgrmarkcomp');
	$this->selectProg->addField('prog', 'short_desc', 'description');
	}
    $this->selectProg->addField('mas_enr', 'is_current', 'current_id');
    $this->selectProg->addExpression('\'tp\'', 'base_type'); 
    $this->selectProg->addExpression('\'Program\'', 'learning_type');     
	$this->selectProg->addExpression('(CASE WHEN overall_status=\'lrn_tpm_ovr_cmp\' THEN \'Completed\' WHEN overall_status=\'lrn_tpm_ovr_enr\' THEN \'Enrolled\' WHEN overall_status=\'lrn_tpm_ovr_inp\' THEN \'InProgress\' WHEN overall_status=\'lrn_tpm_ovr_inc\' THEN \'InComplete\'  WHEN overall_status=\'lrn_tpm_ovr_cln\' THEN \'Cancelled\' WHEN overall_status=\'lrn_tpm_ovr_ppv\' OR overall_status=\'lrn_tpm_ovr_ppm\' THEN \'Pending\' END)','sort_comp_status');
    $this->selectProg->addExpression('null', 'is_compliance');
    $this->selectProg->addExpression('(SELECT CONCAT(exmp.exempted_status,\'|\',exmp.id) FROM slt_enrollment_exempted exmp WHERE exmp.enrollment_id = mas_enr.program_id AND exmp.enroll_type = \'class\' ORDER BY id DESC LIMIT 1)', 'exempted_sts');
    
    $this->selectProg->condition('mas_enr.user_id',$userId);
    
    if($isAPI == 'api')
    {
    	if(!empty($startDate) && !empty($endDate))
    	{
    	$db_or = db_or();
    	$db_or->condition('mas_enr.reg_date', array($startDate, $endDate), 'BETWEEN');
    	$db_or->condition('mas_enr.comp_date', array($startDate, $endDate), 'BETWEEN');
    	$this->selectProg->condition($db_or);
    	}
   
    	if (!empty($_REQUEST['delivery_type_code'])){
    		$delivery_type = explode("|",$_REQUEST['delivery_type_code']);
    		$this->selectProg->condition('mas_profile_delivery.code', $delivery_type, 'IN');
    	}
    	
    	if (!empty($_REQUEST['reg_status'])){
    		$reg_status = explode("|",$_REQUEST['reg_status']);
    		$this->selectProg->condition('mas_enr.overall_status', $reg_status, 'IN');
    	}
    	
    	if(!empty($_REQUEST['cls_title']))
    	{
    		$db_or = db_or();
    		$db_or->condition('prog.title','%' . $_REQUEST['cls_title'] . '%' ,'LIKE');
    		$db_or->condition('prog.code', '%' . $_REQUEST['cls_title'] . '%' ,'LIKE');
    		$this->selectProg->condition($db_or);
    	}
    }
    
    $this->selectProg->condition('mas_enr.overall_status', array('lrn_tpm_ovr_cln', 'lrn_tpm_ovr_cmp', 'lrn_tpm_ovr_enr', 'lrn_tpm_ovr_inp','lrn_tpm_ovr_ppm', 'lrn_tpm_ovr_wtl', 'lrn_tpm_ovr_inc'), 'IN');
    $this->selectProg->groupBy('mas_enr.id');
    
    
	} catch (Exception $ex) {
    	watchdog_exception('searchLpResultsQry', $ex);
    	expertusErrorThrow($ex);
    }
  }
  
  function getSearchResults($isApi='',$startDate='',$endDate=''){
  	try{
    $this -> searchResultsQry($isApi,$startDate,$endDate);
    $this -> searchLpResultsQry($isApi,$startDate,$endDate); 
   // $this->select->orderBy("IF(current_id IS NOT NULL, current_id= 'Y',1=1)","DESC");   

    
    if($isApi == 'api')
    {
    	
    	$sortby =$_REQUEST['sortby'];
    	if($sortby == "AZ") {
    		$xorderby = "class_title";
    		$xorderbylist = "asc";
    	}else if ($sortby == "ZA") {
    		$xorderby = "class_title";
    		$xorderbylist="desc";
    	}else if ($sortby == "Time") {
    		$xorderby="updated_on";
    		$xorderbylist="desc";
    	}else if ($sortby == "dateNew") {
    		$xorderby="enrolled_on";
    		$xorderbylist="desc";
    	}else {
    		$xorderby="class_title";
    		$xorderbylist="asc";
    	}
    	$this->select->orderBy($xorderby,$xorderbylist);
    }
    else {
    	$this->select->orderBy("IF(current_id IS NOT NULL, current_id= 'Y',1=1)","DESC");
    }
    
    $this->selectProg->union($this->select,'UNION ALL');
    // Dont Enable The Debug In Profuction While Using In Union all Query -> if enabled Means It will show Only The Program.   
    // expDebug::dPrintDBAPI(' getSearchResults select obj = ' , $this->selectProg);  
    $this -> setSearchParam(true);
    $this -> setSearchFilter();
    $this -> calculatePagination();    
    return $this -> getSearchList();
    } catch (Exception $ex) {
    	watchdog_exception('getSearchResults', $ex);
    	expertusErrorThrow($ex);
    }
  }
  
  function setFullSearchResults(){
  	try{
    $this -> searchResultsQry();    
    $this -> setSearchParam();
    $this -> setSearchFilter();
    } catch (Exception $ex) {
    	watchdog_exception('setFullSearchResults', $ex);
    	expertusErrorThrow($ex);
    }
  }

  function getSearchResults_($userId) {
  	try{
    $this -> setSearchResults();
    return $this -> getSearchList();
    } catch (Exception $ex) {
    	watchdog_exception('getSearchResults_', $ex);
    	expertusErrorThrow($ex);
    }
  }

  function searchSortBy($sortby) {
    /*if ($sortby == "AZ") {
      $xorderby = "class_title";
      $xorderbylist = "asc";
    } else if ($sortby == "ZA") {
      $xorderby = "class_title";
      $xorderbylist="desc";
    } else if ($sortby == "Time") {
      $xorderby="created_on";
      $xorderbylist="desc";
    } else{
      $xorderby="enrolled_on";
      $xorderbylist="desc";
    }*/
  	try{
    $xorderby="enrolled_on";
    $xorderbylist="desc";
    if($this->sidx !=''){
      if($this->sidx == "class_title")
        $xorderby = 'class_title';
      else if($this->sidx == "enrolled_on")
        $xorderby = 'enrolled_on';
      else if($this->sidx == "completed_on")
        $xorderby = 'updated_on';
      else if($this->sidx == "type")
        $xorderby = 'type';
      else if($this->sidx == "status")
        $xorderby = 'sort_comp_status';
      $xorderbylist = $this->sord;
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
  	try{
    $this->searchSortBy($this->setParamVal('sortby'));
    $this->select->range($this->start, $this->limit);
    $result = $this->selectProg->execute()->fetchAll();
    expDebug::dPrintDBAPI('$result',$this->selectProg);
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
  function setSearchFilter(){
  	try{
    $prog_full_search = $this->selectProg->execute()->fetchAll();
    $this->qry_paging_count = count($prog_full_search);
    } catch (Exception $ex) {
    	watchdog_exception('setSearchFilter', $ex);
    	expertusErrorThrow($ex);
    }
  }
} //Class MyteamMyclassSearch End
/*
 * Function to cancel user registration from course-class(Manager View)
 */
function cancelUserFromCourseClass($LnrType,$enrolledId,$classId,$userId) {
	try{
  $updatedBy = getSltpersonUserId();
  if($LnrType == "Class"){
	  require_once(drupal_get_path('module', 'exp_sp_lnrenrollment').'/exp_sp_lnrenrollment.inc');
	    // For audit trail, get the audited slt_enrollment table fields before the cancel op
      $oldEnrDetails = getEnrollmentDetails($enrolledId, array('reg_status', 'comp_status', 'comp_date', 'grade', 'score'));    
      getDropEnrollment($updatedBy,'',$enrolledId,'','','',1);
      // For audit trail, get get the audited slt_enrollment table fields after the cancel op.
      // Also get the cancel date for class.
      $newEnrDetails = getEnrollmentDetails($enrolledId, array('updated_on', 'reg_status', 'comp_status', 'comp_date', 'grade', 'score'));
      add_audit_trail_entry($updatedBy, $classId, 'cre_sys_obt_cls', 'exp_sp_myteam', 'Reject',
                              'Manager cancelled learner\'s class enrollment', $userId,
                                $oldEnrDetails->reg_status . '|' . $oldEnrDetails->comp_status . '|' . $oldEnrDetails->comp_date . '|' . $oldEnrDetails->grade . '|' . $oldEnrDetails->score,
                                  $newEnrDetails->reg_status . '|' . $newEnrDetails->comp_status . '|' . $newEnrDetails->comp_date . '|' . $newEnrDetails->grade . '|' . $newEnrDetails->score,
                                    $enrolledId);
      $cancel_date = date('M d, Y',strtotime($newEnrDetails->updated_on));
      print $cancel_date.'|success';
  }else{
    if(myTeamDropTrainingProgram($enrolledId,$classId,$userId)){
      //get the cancel date for training plan
      $select = db_select('slt_master_enrollment', 'menr');
      $select->addField('menr','cancel_date');
      $select->condition('menr.id',$enrolledId,'=');
      $cancel_date  = $select->execute()->fetchField();
      $cancel_date = date('M d, Y',strtotime($cancel_date));
      print $cancel_date.'|success';
    }else{
      print ' |failure';   
    }
    
  }
  } catch (Exception $ex) {
  	watchdog_exception('cancelUserFromCourseClass', $ex);
  	expertusErrorThrow($ex);
  }
}

function myTeamDropTrainingProgram($masterEnrollId,$prgId,$user_id){
	try{
		$isUpdate = '';
	  $currUserId = getSltpersonUserId();
	  $updateStmt = db_update('slt_enrollment');  
	  $updateStmt->condition('user_id', $user_id);
	  $updateStmt->condition('master_enrollment_id', $masterEnrollId);
	  $updateStmt->condition(db_or()->condition('comp_status', array('lrn_crs_cmp_enr','lrn_crs_cmp_inp'))
	          		->isNull('comp_status'));
	  
	  //previous state for enrollorment  table
	  $select = db_select('slt_enrollment', 'enrst');
	  $select->addField('enrst','reg_status');
	  $select->condition('enrst.master_enrollment_id',$masterEnrollId,'=');
	  $enrollment_previous_state = $select->execute()->fetchField();
	  expDebug::dPrint("previous_state:". $previous_state.'Master_enrollment'.$masterEnrollId.'prgramid'.$prgId);
	  
	  //$updateStmt->condition('reg_from_prg', 'Y');
	  //$updateStmt->where('IFNULL(comp_status,null) NOT IN (\'lrn_crs_cmp_cmp\')');
	  $updateStmt->fields(array(
	                      'reg_status'      => 'lrn_crs_reg_can',                                                   
	                      'updated_on'    => now(),
	                      'updated_by'    => $currUserId,
	                      'comp_status'   => NULL,
	                      'comp_date'     => NULL
	                      ));
	  $updateStmt->execute();
	  
	  //previous state for master enrollorment  table
	  //$oldEnrDetails = getEnrollmentDetails($enrolledId, array('reg_status', 'comp_status', 'comp_date', 'grade', 'score'));  
	  $select = db_select('slt_master_enrollment', 'menrst');
	  $select->addField('menrst','overall_status');
	  $select->condition('menrst.id',$masterEnrollId,'=');
	  $select->condition('menrst.program_id',$prgId,'=');
	  $master_previous_state = $select->execute()->fetchField();
	  expDebug::dPrint("previous_state:". $master_previous_state.'Master_enrollment'.$masterEnrollId.'prgramid'.$prgId);
	
	  
	  //Update master enroll
	  $updateMasterEnr = db_update('slt_master_enrollment');  
	  $updateMasterEnr->condition('id', $masterEnrollId);
	  $updateMasterEnr->condition('user_id', $user_id);
	  //$updateMasterEnr->where('overall_status NOT IN (\'lrn_tpm_ovr_cmp\',\'lrn_tpm_ovr_cln\')');
	  $updateMasterEnr->condition('overall_status', array('lrn_tpm_ovr_cln','lrn_tpm_ovr_cmp','lrn_tpm_ovr_inc','lrn_tpm_ovr_rsv','lrn_tpm_ovr_exp'), 'NOT IN');
	  $updateMasterEnr->fields(array(
	                      'overall_status'      => 'lrn_tpm_ovr_cln',                                                   
	                      'updated_on'    => now(),
	                      'cancel_date'    => now(),
	  					  'updated_by'    => $currUserId
	                      ));
	  $isUpdate = $updateMasterEnr->execute();
	  
	  if($isUpdate == 0)
	  	return true;  //if nothing is get update then no need to do the below tasks
	                      
	  // audit log details for cancel program
	
	   add_audit_trail_entry($currUserId, $prgId, 'cre_sys_obt_trp', 'exp_sp_myteam', 'Reject',
	                              'Manager cancelled learner\'s enrollment to program', $user_id,
	                                $enrollment_previous_state . '|' . $master_previous_state,
	                                  'lrn_crs_reg_can' . '|' . 'lrn_tpm_ovr_cln',
	                                    $masterEnrollId);
	
	                                    
	  $prg_id = db_query("SELECT program_id FROM slt_master_enrollment WHERE id = :masterid", array(':masterid' =>$masterEnrollId))->fetchField();
	  $obj_role_id = db_query("SELECT id FROM slt_object_role_mapping WHERE object_id =:oid AND object_type = :objtype AND user_id = :userid",
	   array(":oid" =>$prg_id,":objtype" =>'cre_sys_obt_trp',":userid" =>$user_id))->fetchField();
	  
	   
	  //Delete in slt_object_role_mapping
	  $deleteStmt = db_delete('slt_object_role_mapping');  
	  $deleteStmt->condition('id', $obj_role_id);
	  $deleteStmt->execute();
	  
	   expDebug::dPrint("auditlog: user".$user_id.'enroll'.$masterEnrollId.'classid'.$prgId);
	  
	  // Certification/Curriculum/Learning Plan cancel implemented
	  //include "../../exp_sp_learning.inc";
	  $notificationType = "certCurrLPRejectionByManager";
	  $objectId1 = $masterEnrollId;
	  $objectId2 = $prgId;
	  $userId = $user_id;
	  $updateUserId = $currUserId;
	  insertNotification($notificationType, $objectId1, $objectId2, $userId, $updateUserId);  
	  syncSolrData('Enrollment');
	  syncSolrData('MasterEnrollment');
	  syncSolrData('User');
	  return true;
  } catch (Exception $ex) {
  	watchdog_exception('myTeamDropTrainingProgram', $ex);
  	expertusErrorThrow($ex);
  }
}
function markcompleteTPclassunderManagerAPI($user_Id,$userid,$Enrolled_id,$classId,$completionDate,$learning_type,$score)
{
	$response = array();
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	// For audit trail, get get the audited slt_enrollment table fields before the mark complete op
	if(strtolower($learning_type) == "class"){
		$oldEnrDetails = getEnrollmentDetails($enrolledId, array('reg_status', 'comp_status', 'comp_date', 'score'));
		$select = db_select('slt_course_class', 'coucla');
		$select->addExpression('COUNT(coucla.id)', 'ncount');
		$select->join('slt_enrollment','sltenr', 'coucla.course_id = sltenr.course_id');
		$select->condition('sltenr.id',$Enrolled_id, '=');
		$select->condition('coucla.id',$classId,'=');
		$select->condition('sltenr.user_id',$user_Id, '=');
		$select->addField('coucla','title');
		$select->addField('coucla','code');
		$select->addField('coucla','short_description');
		$select->addField('coucla','delivery_type');
		expDebug::dPrintDBAPI('$select check amaran = ' , $select);
		$result = $select->execute()->fetchObject();
		expDebug::dPrint('$result check amaran = '.print_r($result,true),4);
		if($result->ncount) {
			$detail = new stdClass();
			$result->status = markCompleteUserFromCourseClass($learning_type,$Enrolled_id,$classId,$user_Id,$completionDate,$score,'api');
			 
			$detail->title   		= $result->title;
			$detail->code  			= $result->code;
			$detail->description	= $result->short_description;
			$detail->type			= getProfileListItemName($result->delivery_type);
			$detail->status			= $result->status;
			 
			$response[] 			= $detail;
			 
			return $response;
		}
		else{
			$error -> isValidateError = true;
			$error -> errormsg = "Enrollment Id,Course Id ,Class Id,User Id-> Are not assoicated with prticaular Enrollement. Kindly Check it. ";
			return $error;
			break;
		}

	}
	else {
		$newMasterEnrDetails = getMasterEnrollmentDetails($enrolledId, array('overall_status', 'comp_date'));
		expDebug::dPrint('$newMasterEnrDetails check amaran = '.print_r($newMasterEnrDetails,true),4);
		$select = db_select('slt_master_enrollment', 'msenr');
		$select->addExpression('COUNT(msenr.id)', 'ncount');
		$select->join('slt_program','sltpro', 'msenr.program_id = sltpro.id');
		$select->condition('msenr.id',$Enrolled_id, '=');
		$select->addField('sltpro','title');
		$select->addField('sltpro','code');
		$select->addField('sltpro','short_desc');
		$select->addField('sltpro','object_type');
		expDebug::dPrintDBAPI('$select check amaran = ' , $select);
		$result = $select->execute()->fetchObject();
		expDebug::dPrint('$result check amaran = '.print_r($result,true),4);

		if($result->ncount) {
			$detail = new stdClass();
			$result->status = markCompleteUserFromCourseClass($learning_type,$Enrolled_id,$classId,$user_Id,$completionDate,$score,'api');
			$detail->title   		= $result->title;
			$detail->code  			= $result->code;
			$detail->description	= $result->short_desc;
			$detail->type			= getProfileListItemName($result->object_type);
			$detail->status			= $result->status;
			 
			$response[] 			= $detail;

			return $response;
		}
		else{
			$error -> isValidateError = true;
			$error -> errormsg = "Given Enrollment Id,Course Id ,Class Id,User Id-> Are not assoicated with prticaular Traning Plan Enrollement. Kindly Check it. ";
			return $error;
			break;
		}


	}

}
/*
 * Function to mark complete user registration from course-class(Manager View)
 */
function markCompleteUserFromCourseClass($LnrType,$enrolledId,$classId,$userId,$completionDate,$score,$isAPI='')
{
	try{

		$updatedBy = getSltpersonUserId();
		$valcompDate = date('Y-m-d H:i',strtotime(str_replace('-', '/', $completionDate) . ' 00:00:00'));
		$printcompDate = date('M d, Y',strtotime(str_replace('-', '/', $completionDate)));
		$valScore = ($score != 'nil') ? $score : '';
		if(strtolower($LnrType) == "class"){
			// For audit trail, get get the audited slt_enrollment table fields before the mark complete op
			$oldEnrDetails = getEnrollmentDetails($enrolledId, array('reg_status', 'comp_status', 'comp_date', 'score'));
			$select = db_select('slt_course_class', 'coucla');
			$select->addField('coucla','course_id');
			$select->condition('coucla.id',$classId,'=');
			$courseId  = $select->execute()->fetchField();
            
			$markcompleteUpdate = lp_enrollment_upd('','','crs_comp_by_manager',$valcompDate,'','','','',$valScore,$enrolledId,$updatedBy,$userId,'',$classId,$courseId);
			//$markcompleteUpdate = "call slp_enrollment_upd(NULL,NULL,'lrn_crs_cmp_cmp','".$valcompDate."',NULL,NULL,NULL,NULL,'".$valScore."',$enrolledId,$updatedBy,$userId,NULL,$classId,$courseId)";
			expDebug::dPrint("mark complete by manager >>> ".$markcompleteUpdate);
			$check = $markcompleteUpdate;
			expDebug::dPrint("Procedure out ".print_r($check,true),4);
			expDebug::dPrint("Procedure out ".print_r($check[0]->xdid_update,true),4);
			//if($check[0]->xdid_update > 0){
			if($check > 0){
				addICalToNotification();
				// For audit trail, get get the audited slt_enrollment table fields after the mark complete op
				$newEnrDetails = getEnrollmentDetails($enrolledId, array('reg_status', 'comp_status', 'comp_date', 'score'));
				insertUserPointsForEachActionsPerformed($userId,'complete_class_training');
				add_audit_trail_entry($updatedBy, $classId, 'cre_sys_obt_cls', 'exp_sp_myteam', 'Mark Complete',
						'Manager marked learner\'s enrollment to class as completed', $userId,
						$oldEnrDetails->reg_status . '|' . $oldEnrDetails->comp_status . '|' . $oldEnrDetails->comp_date .  '|' . $oldEnrDetails->score,
						$newEnrDetails->reg_status . '|' . $newEnrDetails->comp_status . '|' . $newEnrDetails->comp_date . '|' . $newEnrDetails->score,
						$enrolledId);
				if($isAPI == 'api')
				{
					$detail =  new stdClass();
					if($check)
						$detail->status= "success";
					else
						$detail->status = "failure";
					return $detail;
				}
				print $printcompDate.'|success';
			}else{
				if($isAPI == 'api'){
					$detail =  new stdClass();
					$detail->status = 'nochange';
					return $detail;
				}
				print $printcompDate."|nochange";
			}
		}else{
			$oldMasterEnrDetails = getMasterEnrollmentDetails($enrolledId, array('overall_status', 'comp_date'));
			expDebug::dPrint("Values by oldMasterEnrDetails check amaran>>> ".print_r($oldMasterEnrDetails,true),5);

			$markcompleteUpdate =lp_roster_upd_wrapper($classId,'','',$enrolledId,$updatedBy,'','','crs_comp_by_manager',$valcompDate,'','');
			//$markcompleteUpdate = "call slp_roster_upd_wrapper($classId,NULL,NULL,$enrolledId,$updatedBy,NULL,NULL,'crs_comp_by_manager','".$valcompDate."',NULL,NULL)";
			expDebug::dPrint("mark complete by manager >>> ".$markcompleteUpdate);
			$check =$markcompleteUpdate;
			//$check = db_query($markcompleteUpdate)->fetchAll();
			expDebug::dPrint("Procedure return value -- ".print_r($check,1),4);
			$newMasterEnrDetails = getMasterEnrollmentDetails($enrolledId, array('overall_status', 'comp_date'));
			expDebug::dPrint("Values by user >>> ".$enrolledId.'classidd'.$classId.'userid'.$userId);
			insertUserPointsForEachActionsPerformed($userId,'complete_class_training');
			add_audit_trail_entry($updatedBy, $classId, 'cre_sys_obt_cls', 'exp_sp_myteam', 'Mark Complete',
					'Manager marked learner\'s enrollment to class as completed', $userId,
					$oldMasterEnrDetails->overall_status . '|' .  $oldMasterEnrDetails->comp_date  ,
					$newMasterEnrDetails->overall_status . '|' .  $newMasterEnrDetails->comp_date  ,
					$enrolledId,'TP');

			// expDebug::dPrint("mark complete by manager >>> ".print_r($newtplDetails));
			if($check[0]->msg > 0){
				if($isAPI == 'api')
				{
					$detail =  new stdClass();
					if($check)
						$detail->status= "success";
					else
						$detail->status = "failure";
					return $detail;
				}
				print $printcompDate.'|success';
			}else{
				if($isAPI == 'api'){
					$detail =  new stdClass();
					$detail->status= "nochange";
					return $detail;
				}else{
					print $printcompDate.'|nochange';
				}
			}
		}
		 
	} catch (Exception $ex) {
		watchdog_exception('markCompleteUserFromCourseClass', $ex);
		expertusErrorThrow($ex);
	}
}


?>