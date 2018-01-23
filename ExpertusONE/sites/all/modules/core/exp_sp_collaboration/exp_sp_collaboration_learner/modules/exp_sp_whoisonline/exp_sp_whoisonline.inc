<?php

function fetch_online_users_list()
{
	try{
	 global $user;
	 drupal_save_session(FALSE);
	 $rs=fetch_online_users_list_rs($user->uid);
	 echo drupal_json_encode($rs);
	}catch(Exception $ex) {	
	  	watchdog_exception('fetch_online_users_list', $ex);
	  	expertusErrorThrow($ex);
  	}
}
/*
 * fetch_online_users_list() - function to fetch requested max count of online user's for a page in jqgrid
 */
function fetch_online_users_list_rs($uid, $callfromAPI = 0) {
  
  // Do not update the session table.
  
  
 
  
 expDebug::dPrint(' ' .
                        'page = "' . $_GET['page'] . '" ,' .
                        'rows = "' . $_GET['rows'] . '" ,' .
                        'sort(sidx) = "' . $_GET['sidx'] . '" ,' .
                        'order(sord) = "' . $_GET['sord'] . '" ,' .
                        'search = "' . $_GET['search'] . '" ,' .
                        'nd = "' . $_GET['nd'] . '" ,' .
                        'id = "' . $_GET['id'] . '" ,' .
                        'oper = "' . $_GET['oper'] . '" ,' .
                        'editoper = "' . $_GET['editoper'] . '" ,' .
                        'addoper = "' . $_GET['addoper'] . '" ,' .
                        'deloper = "' . $_GET['deloper'] . '" ,' .
                        'totalrows = "' . $_GET['totalrows'] . '" ,' .
                        'npage = "' . $_GET['npage'] ,4
  ); 
  try{
      global $theme_key;
	  $page = $_REQUEST['page']; // get the requested page
	  $limit = !empty($_REQUEST['rows']) ? $_REQUEST['rows'] : 5;
	  $sord = 'ASC'; // Sort is always ascending by full_name
	
	  // Count users active within the defined period.
	  $interval = REQUEST_TIME - variable_get('exp_sp_whoisonline_block_seconds_online', 900);
	  // Select sessions table
	  $select = db_select('sessions', 's');
	  
	  // Get count of active users since $interval
	  $select->addExpression('COUNT(DISTINCT s.uid)', 'count');
	  
	  // Set the conditions
	  $select->condition('s.timestamp', $interval, '>=');
	  $select->condition('s.uid', 0, '>');
	  $select->condition('s.uid', $uid, '!=');
	
	  expDebug::dPrintDBAPI(' $select obj = ' ,$select);
	  
	  // Execure the query
	  $onlineUsersCount = $select->execute()->fetchField();
	  expDebug::dPrint(' $onlineUsersCount = ' . print_r($onlineUsersCount, true),3);
	
	  if( $onlineUsersCount > 0 ) {
	    $total_pages = ceil($onlineUsersCount / $limit);
	  }
	  else {
	    $total_pages = 0;
	  }
	  
	  if ($page > $total_pages) {
	    $page = $total_pages;
	  }
	
	  $start = ($limit * $page) - $limit; // do not put $limit*($page - 1)
	  if ($start < 0) {
	    $start = 0;
	  } 
	  expDebug::dPrint(' $start = ' . print_r($start, true),4);
	  
	  // Select sessions table
	  $select = db_select('sessions', 's');
	      
	  // Left join the users, slt_person and slt_profile_list_items table to get the user name
	  $select->leftJoin('users', 'u', 's.uid = u.uid');
	  $select->leftJoin('slt_person', 'p', 'u.name = p.user_name');
	  $select->leftJoin('slt_profile_list_items', 'pli', 'p.job_title = pli.code');
	  $select->leftJoin('file_managed', 'pic', 'u.picture = pic.fid');
	  $select->leftjoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id = p.id ');
	  $select->leftjoin('slt_groups','grp',"grp.status = 'cre_sec_sts_atv' AND grp.is_admin=0 AND (((((if(grp.org_id='All',p.org_id is not null,grp.org_id is null OR FIND_IN_SET(p.org_id,grp.org_id)>0))
												AND (if(grp.user_type='All',p.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(p.user_type,''),grp.user_type)>0 ))
												AND (if(grp.employment_type='All',p.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(p.employment_type,''),grp.employment_type)>0))
												AND (if(grp.country='All',p.country is not null,grp.country is null OR FIND_IN_SET(ifnull(p.country,''),grp.country)>0))
  											AND (if(grp.state='All',p.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(p.country,'-',p.state),''),grp.state)>0))
  											AND (if(grp.department='All',p.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(p.dept_code,''),grp.department)>0))
	   										AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where p.id= jobmap.user_id)>0,grp.job_role IS NULL OR (FIND_IN_SET(ifnull(lpjm.job_role,''),ifnull(grp.job_role,''))>0)))
  											AND (if(grp.language='All',p.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(p.preferred_language,''),grp.language)>0))
 												AND (if(grp.is_manager='N',1=1,(ifnull(p.is_manager,'N') = grp.is_manager))
       									AND	if(grp.is_instructor='N',1=1,(ifnull(p.is_instructor,'N') = grp.is_instructor)))
	  									AND (grp.removed_users is null OR FIND_IN_SET(p.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(p.id,grp.added_users)>0))))");
	  $select->leftJoin('slt_group_attributes','grpatt','grpatt.group_id = grp.id AND if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,\'%Y-%m-%d\') <= DATE_FORMAT(p.hire_date,\'%Y-%m-%d\'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,\'%Y-%m-%d\') >= DATE_FORMAT(p.hire_date,\'%Y-%m-%d\'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(p.hire_date,\'%Y-%m-%d\') between DATE_FORMAT(grpatt.between_start_date,\'%Y-%m-%d\') AND DATE_FORMAT(grpatt.between_end_date,\'%Y-%m-%d\'), 1=0),1=1) ');
	  // Select fields/expression values to be fetched from the database.
	  $select->addField('s', 'uid', 'uid');
	  $select->addField('p', 'full_name', 'full_name');
	  $select->addField('pic', 'uri', 'uri');
	  $select->addField('pli', 'name', 'job_title');
	  
	  // Set the conditions
	  $select->condition('s.timestamp', $interval, '>=');
	  $select->condition('s.uid', 0, '>');
	  $select->condition('s.uid', $uid, '!=');
	  
	  // Group by uid in sessions table
	  $select->groupBy('s.uid');
	  
	  // Descending order by full_name
	  $select->orderBy('full_name', $sord);

	  // Fetching total records
	  $totalRecords = $select->countQuery()->execute()->fetchField();
	  
	  // Limit to $limit records
	  $select->range($start, $limit);
  	if($uid>0){
  		$loggedInUserId = getUserIdFromDrupalUid($uid);
			$grpId = getGroupId($loggedInUserId);
		}
	  
 		if($uid <= 0 || $loggedInUserId <= 0 || empty($grpId)){
			$select->condition('grp.id','','IS NULL');
		}else if(!empty($grpId)){
			$select->condition('grp.id','','IS NOT NULL');
			$select->where("FIND_IN_SET(grp.id,'$grpId')> 0");
			$select->groupBy("p.id");
		}
	  expDebug::dPrintDBAPI(' $select obj = ' , $select );
	  
	  
	  // Execure the query and fetch all the records
	  $onlineUsersList = $select->execute()->fetchAll();
	  expDebug::dPrint(' $items = ' . print_r($onlineUsersList, true),3);
	  if($callfromAPI){
	  	$onlineUsersList['totalrow'] = $totalRecords;
	    return $onlineUsersList;
	  }
	  // Prepare and send the response
	  $response->page = $page;
	  $response->total = $total_pages;
	  $response->records = $onlineUsersCount;
	  
	  $i=0;
	  foreach ($onlineUsersList as $onlineUser) {
	    expDebug::dPrint(' $onlineUser = '. print_r($onlineUser, true),3);
	
	    $response->rows[$i]['id']=$onlineUser->uid; 
	    $response->rows[$i]['cell']['id'] = $onlineUser->uid;
	    $response->rows[$i]['cell']['job_title'] = $onlineUser->job_title;
	    $response->rows[$i]['cell']['css'] = 'exp-whoisonline-user-rec';
	    
	    // Get screen_name from full_name
	  	$exp_ufn = explode(" ",$onlineUser->full_name);
	 	if(count($exp_ufn) > 2){
	 	  $response->rows[$i]['cell']['screen_name'] = $exp_ufn[0]." ".substr($exp_ufn[2], 0, 1);
	 	}
	 	else {
	 	  $response->rows[$i]['cell']['screen_name'] = $exp_ufn[0]." ".substr($exp_ufn[1], 0, 1);
	 	}
	 	
	 	// Create the image url from uri
	  if($theme_key == 'expertusoneV2'){ 
	 	if (!empty($onlineUser->uri)) {
	      $response->rows[$i]['cell']['image_url'] = file_create_url($onlineUser->uri);
	 	}
	 	else {
	      $response->rows[$i]['cell']['image_url'] = file_create_url('sites/default/files/pictures/expertusonev2_default_user.png');
	    }
	  }else{
	  if (!empty($onlineUser->uri)) {
	      $response->rows[$i]['cell']['image_url'] = file_create_url($onlineUser->uri);
	 	}
	 	else {
	      $response->rows[$i]['cell']['image_url'] = file_create_url('sites/default/files/pictures/default_user.png');
	    }
	  }
	    $i++;
	  }
	  
	  
	  if (count($onlineUsersList) > 0) {
	    $response->rows[0]['cell']['css'] .= ' exp-whoisonline-user-first-rec';
	    $response->rows[count($onlineUsersList) - 1]['cell']['css'] .= ' exp-who-is-online-last-rec';
	  }
	  expDebug::dPrint("response");
		 expDebug::dPrint($response,4);
	  expDebug::dPrint(' $response record before test snub = '. print_r($response, true),4);
	  //test_snub($response, $limit);
	  expDebug::dPrint(' $response record = '. print_r($response, true),4);
	   return $response;
  }catch(Exception $ex) {	
	  watchdog_exception('fetch_online_users_list_rs', $ex);
	  expertusErrorThrow($ex);
  }
}

/*
 * test_snub() - Function to manipulate response data from fetch_online_users_list() to add dummy online users.
 *
function test_snub(&$response, $limit) {
  if ($response->records > 0) {
    $response->rows[0]['cell']['css'] = 'exp-whoisonline-user-rec';
    for ($i = 1; $i < $limit; $i++) {
      $response->rows[$i]['id'] = $response->rows[0]['id'] + $i;
      $response->rows[$i]['cell']['id'] = $response->rows[0]['cell']['id'] + $i;
      $response->rows[$i]['cell']['job_title'] = $response->rows[0]['cell']['job_title'];
      $response->rows[$i]['cell']['screen_name'] = $response->rows[0]['cell']['screen_name'];
      $response->rows[$i]['cell']['image_url'] = $response->rows[0]['cell']['image_url'];
      $response->rows[$i]['cell']['css'] = 'exp-whoisonline-user-rec';
    }
    
    $response->rows[0]['cell']['css'] .= ' exp-whoisonline-user-first-rec';
    $response->rows[$limit - 1]['cell']['css'] .= ' exp-who-is-online-last-rec';
    
    $response->total = 2;
    $response->records = $limit * 2;
  }
}
 */

function fetch_online_users_list_api($learnerid){
  try {
  	$drupalUid = getDrupalIdOfUser($learnerid);
  	return fetch_online_users_list_rs($drupalUid, 1);
  }catch(Exception $ex) {	
	  watchdog_exception('fetch_online_users_list_api', $ex);
	  expertusErrorThrow($ex);
  }
}

?>