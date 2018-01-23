<?php
/*
 * getNewCourses() - Get the top 4 most recently added courses.
 */
function getNewCourses() {
  try{
  // Select slt_course_template table
  $select = db_select('slt_course_template', 'sct');

  // Left join the slt_node_learning_activity table.
  $select->leftJoin('slt_node_learning_activity', 'snl', 'sct.id = %alias.entity_id AND %alias.entity_type = \'cre_sys_obt_crs\'');
  //$select->leftjoin('slt_catalog_access', 'catacs', 'catacs.entity_id=sct.id AND catacs.entity_type = \'cre_sys_obt_crs\' ');
//  $select->leftJoin('slt_group_mapping', 'group_map', 'group_map.entity_id=sct.id AND group_map.entity_type = \'cre_sys_obt_crs\'');
  //$select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');


  // Select these specific fields for the output.
  $select->addField('sct', 'id','id');
  $select->addField('sct', 'title','title');
  $select->addField('sct', 'short_description','short_description');
  $select->addField('snl', 'node_id','node_id');
  $select->addField('sct', 'created_on','created_on');
  $select->addField('snl', 'entity_type','entity_type');

  // Filter only active status.
  $select->condition('sct.status', 'lrn_crs_sts_atv');
  $select->condition('sct.is_blended', 0);
  $select->where('(IF(sct.compliance_completed = \'hire_days\' or sct.compliance_completed = \'date\',1=0,1=1))');

  //filterByUserAccessPrivileges($select, 'catacs','','group_map');
  filterByUserAccessPrivileges($select, 'sct','','','cre_sys_obt_crs');

  $select->groupBy('sct.id');

  return $select;
  }catch (Exception $ex) {
    watchdog_exception('getNewCourses', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getNewTrainingPlans() - Get the top 4 most recently added training plan.
 */
function getNewTrainingPlans() {
  try{
  // Select slt_program table
  $select = db_select('slt_program', 'sp');

  // Left join the slt_node_learning_activity table.
  $select->leftJoin('slt_node_learning_activity', 'snl', 'sp.id = %alias.entity_id AND %alias.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\') AND %alias.custom0 IS NULL');
  //$select->leftjoin('slt_catalog_access', 'catacs', 'catacs.entity_id=sp.id AND catacs.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\')');
 // $select->leftJoin('slt_group_mapping', 'group_map', 'group_map.entity_id=sp.id AND group_map.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\')');
 // $select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');


  // Select these specific fields for the output.
  $select->addField('sp', 'id','id');
  $select->addField('sp', 'title','title');
  $select->addField('sp', 'short_desc','short_description');
  $select->addField('snl', 'entity_id','node_id');
  $select->addField('sp', 'created_on','created_on');
  $select->addField('snl', 'entity_type','entity_type');

  // Filter only active status.
  $select->condition('sp.status', 'lrn_lpn_sts_atv');

  //filterByUserAccessPrivileges($select, 'catacs','','group_map');
  filterByUserAccessPrivileges($select, 'sp','','','','Program');

  $select->groupBy('sp.id');

  return $select;
  }catch (Exception $ex) {
    watchdog_exception('getNewTrainingPlans', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getNewTrainings() - Get the top 4 most recently added training plan /course.
*/

function getNewTrainings(){
  try{
  $exp_sp_new_course_block_no_of_display = variable_get('exp_sp_new_course_block_no_of_display', 4); // Fetch max 4 items, if variable is not set

  $select=getNewCourses();
  $selectTP=getNewTrainingPlans();
  $selectTP->orderBy('created_on','DESC');
  $selectTP->range(0, $exp_sp_new_course_block_no_of_display);
  $select_new=$select->union($selectTP,'UNION ALL');
  $selectInd = $select_new->extend("ExpertusSelectQueryExtender");
	$selectInd->setIndex(array('sct'=>'ksts','sp'=>'tpsts'));
	$retResult= $selectInd->execute()->fetchAll();

  expDebug::dPrintDBAPI(' $select_new = ' , $selectInd);
  expDebug::dPrint(' $retResult = ' . print_r($retResult, true) , 3);

  return $retResult;
  }catch (Exception $ex) {
    watchdog_exception('getNewTrainings', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getHighlyRateds() - Get the most recent 'HighlyRated' courses
 */
function getHighlyRateds() {
  try{
  // Select slt_promoted_highly_rated table
  $select = db_select('slt_promoted_highly_rated', 'shr');

  // Left join the slt_course_template table
  $select->leftJoin('slt_course_template', 'sct',
                        // Currently 'Course' is stored as entity_type for course in slt_promoted_highly_rated table.
                        // However, we are likely to switch to using the code.
  						'(shr.entity_type = \'cre_sys_obt_crs\' OR shr.entity_type = \'Course\') AND shr.activity_id = %alias.id');

  $select->leftjoin('slt_program','sp','(shr.entity_type = \'Curricula\' OR shr.entity_type = \'Certification\' OR shr.entity_type = \'Learning Plan\' OR shr.entity_type = \'Training Plan\') AND shr.activity_id = sp.id');
  //$select->leftjoin('slt_catalog_access', 'catacs', '((catacs.entity_id=sct.id AND catacs.entity_type = \'cre_sys_obt_crs\') OR catacs.entity_id=sp.id AND catacs.entity_type IN (\'cre_sys_obt_cur\',\'cre_sys_obt_crt\',\'cre_sys_trn\'))');
 //$select->leftJoin('slt_group_mapping', 'group_map', '((group_map.entity_id=sct.id AND group_map.entity_type = \'cre_sys_obt_crs\') OR group_map.entity_id=sp.id AND group_map.entity_type IN (\'cre_sys_obt_cur\',\'cre_sys_obt_crt\',\'cre_sys_trn\'))');
 //$select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');
  // Select these specific fields for the output.
  $select->addField('shr', 'updatedon');
  $select->addField('shr', 'status');
  $select->addField('shr', 'createdon');

  $select->addExpression('IF((shr.entity_type = \'cre_sys_obt_crs\' OR shr.entity_type = \'Course\'),shr.content_id,sp.id) ','node_id');

  $select->addExpression('IF((shr.entity_type = \'cre_sys_obt_crs\' OR shr.entity_type = \'Course\'),\'cre_sys_obt_crs\',sp.object_type) ','entity_type');

  $select->addExpression('IF((shr.entity_type = \'cre_sys_obt_crs\' OR shr.entity_type = \'Course\'),sct.id,sp.id) ','id');
  $select->addExpression('IF((shr.entity_type = \'cre_sys_obt_crs\' OR shr.entity_type = \'Course\'),sct.title,sp.title) ','title');
  $select->addExpression('IF((shr.entity_type = \'cre_sys_obt_crs\' OR shr.entity_type = \'Course\'),sct.short_description,sp.short_desc) ','short_description');
  $select->addExpression('IF((shr.entity_type = \'cre_sys_obt_crs\' OR shr.entity_type = \'Course\'),sct.code,sp.code) ','code');

  // Fetch only records for courses from slt_promoted_highly_rated table whose status is '0' or '1'. Status '2' is for deleted records
  $select->condition(db_or()->condition('shr.entity_type', 'Course') // Currently 'Course' is stored as entity_type for course in slt_promoted_highly_rated table.
                            ->condition('shr.entity_type', 'cre_sys_obt_crs') // Added in case we later switch to using the code in slt_promoted_highly_rated table.
                            ->condition('shr.entity_type', 'Certification')
                            ->condition('shr.entity_type', 'Curricula')
                            ->condition('shr.entity_type', 'Learning Plan')
                            ->condition('shr.entity_type', 'Training Plan')
                     );
  $select->condition(db_or()->condition('shr.status', '0')
                            ->condition('shr.status', '1')
                    );
  // and for only active courses
   $select->condition(db_or()->condition(db_and()->condition('sct.status', 'lrn_crs_sts_atv')
                                                 ->condition('sct.is_blended',0)
                                        )
                             ->condition('sp.status', 'lrn_lpn_sts_atv')
                     );

  // Further filter by role
  //filterByUserAccessPrivileges($select, 'catacs','','group_map');
  filterByUserAccessPrivileges($select, '','','','','Spotlight');

  // Descending order by created date.
  $select->orderBy('shr.status', 'DESC');
  $select->orderBy('IFNULL(shr.updatedon, shr.createdon)', 'DESC');
  // Fetch only as many records as have been configured by admin
  $exp_sp_highly_rated_block_no_of_display = variable_get('exp_sp_highly_rated_block_no_of_display', 4); // Fetch max 4 items, if variable is not set
  $select->range(0, $exp_sp_highly_rated_block_no_of_display);


  expDebug::dPrintDBAPI(' $select obj = ' , $select);

  // Return the result in object format.
  $retResult = $select->execute()->fetchAll();
  expDebug::dPrint(' $retResult = ' . print_r($retResult, true) , 3);
  return $retResult;
  }catch (Exception $ex) {
    watchdog_exception('getHighlyRateds', $ex);
    expertusErrorThrow($ex);
  }
}


/*
 * getHighlyRateds() - Get the most recent 'HighlyRated' courses
 */
function getListOfHighlyRatedByRestAPI(){
	try{
		$result = getHighlyRatedRating();
		$count = getHighlyRatedRating('COUNT');
		$result['totalrow'] = $count ;
		expDebug::dPrint(' $count = ' . print_r($count, true) , 3);
		return $result;
	}catch(Exception $ex) {
		watchdog_exception('getListOfHighlyRatedByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
	}
function getHighlyRatedRating($count = '') {

  try{
  	
  	include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules//core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrenrollment/exp_sp_lnrenrollment.inc');
  	 
  $select = db_select('slt_highly_rated_training', 'vote');
  //$select->addExpression('nod.node_id','node_id');
  if(!empty($count)){
  	$select->addExpression('count(1)','cnt');
  }else{
  $select->addField('vote','node_id','node_id');
  $select->addField('vote','vote_value','vote_value');
  $select->addField('vote','entity_type','entity_type');
  $select->addField('vote','id','id');
  $select->addField('vote','title','title');
  $select->addField('vote','short_description','short_description');
  $select->addField('vote','code','code');
  }
  $select->orderBy('vote_value','DESC');
  $select->orderBy('vote_count','DESC');
  $select->orderBy('vote.created_on','DESC');
  // Fetch only as many records as have been configured by admin
  $exp_sp_highly_rated_block_no_of_display = variable_get('exp_sp_highly_rated_block_no_of_display', 4); // Fetch max 4 items, if variable is not set
   $limit = (empty($_REQUEST['limit'])) ? $exp_sp_highly_rated_block_no_of_display : $_REQUEST['limit'];
   $select->range(0, $limit);
  expDebug::dPrintDBAPI(' $select slt_highly_rated_training = ' , $select);
  filterByUserAccessPrivileges($select, 'vote','','','cre_sys_obt_crs');
  expDebug::dPrintDBAPI('Checking Group Access' , $select);
  if(!empty($count))
  	$retResult = $select->execute()->fetchField();
  else
  $retResult = $select->execute()->fetchAll();
  
  if($_REQUEST['mobile_request']){
  foreach($retResult as $resClsDetials) {
  	$attachment_status = getAttachmentDetails($resClsDetials->id,$resClsDetials->entity_type,"");
  	$resClsDetials->attachment_status=$attachment_status;
  }
  }
  
  expDebug::dPrint(' $retResult = ' . print_r($retResult, true) , 3);
  return $retResult;
  }catch (Exception $ex) {
    watchdog_exception('getHighlyRatedRating', $ex);
    expertusErrorThrow($ex);
  }
}


/*
 * getMostPopularCourses() - Get the courses that have the most number of users enrolled
 */
function getMostPopularCourses($count = ''){
  try{
  $selectEnr = db_select('slt_enrollment', 'enrol');
  $selectEnr->addField('enrol', 'course_id');
  $selectEnr->addExpression('COUNT(enrol.user_id)','ucnt');
  $selectEnr->where('enrol.reg_status IN (\'lrn_crs_reg_wtl\', \'lrn_crs_reg_ppm\', \'lrn_crs_reg_cnf\')');
  $selectEnr->condition('enrol.master_enrollment_id', '', 'IS NULL');
  $selectEnr->groupBy('enrol.course_id');
  $selectEnr->orderBy('ucnt', 'DESC');
  // expDebug::dPrintDBAPI(' $select top enrollments = ' , $selectEnr);
  // Select slt_course_template table
  $select = db_select('slt_course_template', 'sct');

  // Left join the slt_node_learning_activity table.
  $select->join('slt_node_learning_activity', 'snl', 'sct.id = %alias.entity_id AND %alias.entity_type = \'cre_sys_obt_crs\'');

  // $select->join('slt_enrollment', 'enr', 'sct.id = %alias.course_id');
	$select->addJoin('INNER', $selectEnr, 'enr', 'enr.course_id = sct.id');
  //$select->leftjoin('slt_catalog_access', 'catacs', 'catacs.entity_id=sct.id AND catacs.entity_type = \'cre_sys_obt_crs\' ');
  //$select->leftJoin('slt_group_mapping', 'group_map', 'group_map.entity_id=sct.id AND group_map.entity_type = \'cre_sys_obt_crs\'');
  // $select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');
  if(!empty($count)){
  	  $select->addExpression('count(Distinct(enr.course_id))','cnt');
  }else{
	  // Select these specific fields for the output.
	  $select->addField('sct', 'id','id');
	  $select->addField('sct', 'title','title');
	  $select->addField('sct', 'short_description','short_description');
	  // $select->addexpression('COUNT(enr.user_id)','enr_count');
	  $select->addField('snl', 'node_id','node_id');
	  $select->addField('sct', 'code','code');
	  $select->addField('snl', 'entity_type','entity_type');
	  $select->addField('enr', 'ucnt', 'enr_count');
  }
  // Filter only active status.
  $select->condition('sct.status', 'lrn_crs_sts_atv');
  $select->condition('sct.is_blended', 0);
  // $select->condition('enr.reg_status', array('lrn_crs_reg_wtl','lrn_crs_reg_ppm','lrn_crs_reg_cnf'),'IN');
  // Further filter by role
  //filterByUserAccessPrivileges($select, 'catacs','','group_map');
  filterByUserAccessPrivileges($select, 'sct','','','cre_sys_obt_crs');
  // if(empty($count))
    // $select->groupBy('enr.course_id');
  expDebug::dPrintDBAPI('select most popular courses ' , $select);
  return $select;
  }catch (Exception $ex) {
    watchdog_exception('getMostPopularCourses', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getMostPopular() - Get the training plan that have the most number of users enrolled
 */
function getMostPopularTrainingPlans($count=''){
  try{
  
	$selectMasEnr = db_select('slt_master_enrollment', 'enrol');
	$selectMasEnr->addField('enrol', 'program_id');
	$selectMasEnr->addExpression('COUNT(enrol.user_id)','ucnt');
	$selectMasEnr->where('enrol.overall_status IN (\'lrn_tpm_ovr_cmp\', \'lrn_tpm_ovr_enr\', \'lrn_tpm_ovr_inp\', \'lrn_tpm_ovr_exp\', \'lrn_tpm_ovr_wtl\', \'lrn_tpm_ovr_inc\')');
	$selectMasEnr->groupBy('enrol.program_id');
	$selectMasEnr->orderBy('ucnt', 'DESC');
	// expDebug::dPrintDBAPI(' $select top tp enrollments = ' , $selectMasEnr);
  // Select slt_program table
  $select = db_select('slt_program', 'sp');

  // Left join the slt_node_learning_activity table.
  $select->join('slt_node_learning_activity', 'snl', 'sp.id = %alias.entity_id AND %alias.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\')');

  // $select->join('slt_master_enrollment', 'enr', 'sp.id = %alias.program_id');
  $select->addJoin('INNER', $selectMasEnr, 'enr', 'sp.id = enr.program_id');
  //$select->leftjoin('slt_catalog_access', 'catacs', 'catacs.entity_id=sp.id AND catacs.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\')');
  //$select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=sp.id AND group_map.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\')');
  //$select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');
  if(!empty($count)){
  	  $select->addExpression('count(Distinct(enr.program_id))','cnt');
  }else{
	  // Select these specific fields for the output.
	  $select->addField('sp', 'id','id');
	  $select->addField('sp', 'title','title');
	  $select->addField('sp', 'short_desc','short_description');
	  // $select->addexpression('COUNT(enr.user_id)','enr_count');
	  $select->addField('snl', 'node_id','node_id');
	  $select->addField('sp', 'code','code');
	  $select->addField('snl', 'entity_type','entity_type');
	  $select->addField('enr', 'ucnt', 'enr_count');
  }
  // Filter only active status.
  $select->condition('sp.status', 'lrn_lpn_sts_atv');
  // $select->condition('enr.overall_status', array('lrn_tpm_ovr_cmp','lrn_tpm_ovr_enr','lrn_tpm_ovr_inp','lrn_tpm_ovr_exp','lrn_tpm_ovr_wtl','lrn_tpm_ovr_inc'),'IN');

  // Further filter by role
  //filterByUserAccessPrivileges($select, 'catacs','','group_map');
  filterByUserAccessPrivileges($select, 'sp','','','','Program');
  // if(empty($count))
  	// $select->groupBy('enr.program_id');
  // expDebug::dPrintDBAPI('select most popular tps ',$select);
  return $select;
  }catch (Exception $ex) {
    watchdog_exception('getMostPopularTrainingPlans', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getMostPopular() - Get the courses/training plan that have the most number of users enrolled
 */
function getMostPopular($userId = 0){
	try{
  $exp_sp_most_popular_block_no_of_display = variable_get('exp_sp_most_popular_block_no_of_display', 4); // Fetch max 4 items, if variable is not set
  expDebug::dPrint('Api Name : '.$_REQUEST['apiname'], 4); // If request came from API --> API name
  if($_REQUEST['apiname'] == "GetMostPopularCoursesAPI") { // API Get learner Id
  	$learnerid = $_REQUEST['learnerid'];
  	expDebug::dPrint('Learner Id : '.$learnerid, 4); // Learner Id from API
  }
  if($userId && ( empty($_REQUEST['apiname']) || ((!empty($learnerid) || $learnerid != 0 ) && $_REQUEST['apiname'] == "GetMostPopularCoursesAPI" )) ) {
	  	if($_REQUEST['apiname'] == "GetMostPopularCoursesAPI") { // API Need this drupal_bootstrap to work On ExpertusSelectQueryExtender
	  		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	  		$limit = (empty($_REQUEST['limit'])) ? $exp_sp_most_popular_block_no_of_display : $_REQUEST['limit'];
	  	}else{
	  		$limit = $exp_sp_most_popular_block_no_of_display;
	  	}
	  	
	  	if($_REQUEST['mobile_request']){
	  		
	  		//File included for attachment in mobile
	  		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules//core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrenrollment/exp_sp_lnrenrollment.inc');
	  		
	  		 
	  		$select = db_select('slt_most_popular_training', 'sct');
	  		// Select these specific fields for the output.
	  		$select->addField('sct', 'id','id');
	  		$select->addField('sct', 'title','title');
	  		$select->addField('sct', 'short_description','short_description');
	  		$select->addField('sct', 'enr_count','enr_count');
	  		$select->addField('sct', 'node_id','node_id');
	  		$select->addField('sct', 'code','code');
	  		$select->addField('sct', 'entity_type','entity_type');
	  		$select->range(0, $exp_sp_most_popular_block_no_of_display);
	  		$retResult= $select->execute()->fetchAll();
	  		
	  		if($_REQUEST['mobile_request']){
	  		foreach($retResult as $resClsDetials) {
	  			$attachment_status = getAttachmentDetails($resClsDetials->id,$resClsDetials->entity_type,"");
	  			$resClsDetials->attachment_status=$attachment_status;
	  		}
	  		}
	  		
	  		expDebug::dPrintDBAPI(' $select_new = ' , $select);
	  		expDebug::dPrint(' $retResult = ' . print_r($retResult, true) , 3);
	  	}else{
	  	
     	$select=getMostPopularCourses();
	    $selectTP=getMostPopularTrainingPlans();
	    $selectTP->orderBy('enr_count','DESC');
	    $selectTP->range(0, $limit);
	    $select_new=$select->union($selectTP,'UNION ALL');
	    $selectInd = $select_new->extend("ExpertusSelectQueryExtender");
			$selectInd->setIndex(array('sct'=>'ksts','sp'=>'tpsts'));
	    $retResult= $selectInd->execute()->fetchAll();
	    expDebug::dPrintDBAPI(' $select_new = ' , $selectInd);
	    expDebug::dPrint(' $retResult = ' . print_r($retResult, true) , 3);
	    if ($_REQUEST['apiname'] == "GetMostPopularCoursesAPI") {
	    	$sel_crs_cnt = getMostPopularCourses('COUNT');
	    	$res_crs_cnt = $sel_crs_cnt->execute()->fetchField();
	    	$sel_TP_cnt = getMostPopularTrainingPlans('COUNT');
	    	$res_TP_cnt = $sel_TP_cnt->execute()->fetchField();
	    	$total_cnt = $res_crs_cnt + $res_TP_cnt ;
	    	// expDebug::dprint("total count ". $total_cnt,4);
	    	$retResult['totalrow'] = $total_cnt;
	    }
	    }
  	
  }else{
    $select = db_select('slt_most_popular_training', 'sct');
    // Select these specific fields for the output.
    $select->addField('sct', 'id','id');
    $select->addField('sct', 'title','title');
    $select->addField('sct', 'short_description','short_description');
    $select->addField('sct', 'enr_count','enr_count');
    $select->addField('sct', 'node_id','node_id');
    $select->addField('sct', 'code','code');
    $select->addField('sct', 'entity_type','entity_type');
    $select->range(0, $exp_sp_most_popular_block_no_of_display);
    $retResult= $select->execute()->fetchAll();
    expDebug::dPrintDBAPI(' $select_new = ' , $select);
    expDebug::dPrint(' $retResult = ' . print_r($retResult, true) , 3);
  }
  return $retResult;
  }catch (Exception $ex) {
  	watchdog_exception('getMostPopular', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * getMostActiveUser() - Fetch the most active users
 * SELECT
 *		u.uid AS uid,
 *		p.full_name AS full_name,
 *		f.uri AS uri,
 *		pli.name AS job_title,
 *		COUNT(e.user_id) AS count_user,
 *		CONCAT(p.first_name, SPACE(1), SUBSTR(p.last_name, 1, 1)) AS user_name
 * FROM
 *		slt_enrollment e
 *		LEFT JOIN slt_person p ON e.user_id = p.id
 *		LEFT JOIN slt_profile_list_items pli ON p.job_title = pli.code
 *		LEFT JOIN users u ON p.user_name = u.name
 *		LEFT JOIN file_managed f ON u.picture = f.fid
 * GROUP BY
 * 		e.user_id
 * ORDER BY
 * 		count_user DESC
 * LIMIT 4 OFFSET 0
 */
function getMostActiveUser() {
   try{
   	$select = db_select('slt_most_active_users', 'e');
   	$select->addField('e', 'uid', 'uid');
   	$select->addField('e', 'full_name', 'full_name');
   	$select->addField('e', 'uri', 'uri');
   	$select->addField('e', 'job_title', 'job_title');
   	$select->addField('e', 'count_user', 'count_user');
   	$select->addField('e', 'user_name', 'user_name');
   	expDebug::dPrintDBAPI(' $select SQL = ' , $select);
   	$retResult = $select->execute()->fetchAll();
   	return $retResult;
  }catch (Exception $ex) {
  	watchdog_exception('getMostActiveUser', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 *  get_sort_details() - Used for Qtip, to show the course details, while mouseover on course title
 */
function get_sort_details($entityId, $tpTitle) {
	try {
  $select = db_select('slt_course_template', 'sct');
  // Join the slt_node_learning_activity table.
  $select->join('slt_node_learning_activity', 'snl', 'sct.id = %alias.entity_id');
  // Select these specific fields for the output.
  $select->addField('sct', 'id');
  $select->addField('sct', 'code');
  $select->addField('sct', 'title');
  $select->addField('sct', 'short_description');
  // Filter only active status.
  $select->condition('snl.node_id', $entityId);
  $row =  $select->execute()->fetchAssoc();
  $row->short_description = (!empty($row->short_description) ? $row->short_description : '');
  print json_encode($row);
  exit;
  }catch (Exception $ex) {
  	watchdog_exception('get_sort_details', $ex);
  	expertusErrorThrow($ex);
  }
}

?>