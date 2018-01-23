<?php

function fetch_catalog_users_list($username)
{
	try{
	 global $user;
	 drupal_save_session(FALSE);
	 $rs=fetch_catalog_users_list_rs($user->uid,$username);
	 echo drupal_json_encode($rs);
	}catch(Exception $ex) {	
	  	watchdog_exception('fetch_catalog_users_list', $ex);
	  	expertusErrorThrow($ex);
  	}
}
/*
 * fetch_catalog_users_list() - function to fetch requested max count of online user's for a page in jqgrid
 */
function fetch_catalog_users_list_rs($uid, $username,$callfromAPI = 0) {
  
  try{
      global $theme_key;
	  $page = $_REQUEST['page']; // get the requested page
	  $limit = $_REQUEST['rows'];
	  $sord = 'ASC'; // Sort is always ascending by full_name
	
	  // Count users active within the defined period.
	  $interval = REQUEST_TIME - variable_get('exp_sp_user_catalog_block_seconds_online', 900);
	  $loggedInUserId = getIdOfLoggedInUser();
  	if($loggedInUserId != 1){
			$grpId = getGroupId($loggedInUserId);
		}
	  // Select sessions table
	  /*$select = db_select('users', 'u');
	  $select->leftJoin('slt_person', 'p', 'u.name = p.user_name');
	  // and u.status = 1 and p.status='cre_usr_sts_atv'
	   // Set the conditions
	  $select->condition('u.uid', array(0,$uid), 'NOT IN');
	  $select->condition('u.status', 1, '=');
	  $select->condition('p.status', 'cre_usr_sts_atv', '=');
	  $select->condition('p.user_name', array('admin', 'guest'), 'NOT IN');
	  $select->leftjoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id = p.id ');
	  if(!empty($grpId)){
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
	  }
		
		
		$select1 = db_select('slt_person','p1');
  		$select1->leftJoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id = p1.id');
  		$select1->innerJoin('slt_groups','grp',"grp.status = 'cre_sec_sts_atv' AND grp.is_admin=0 AND (((((if(grp.org_id='All',p1.org_id is not null,grp.org_id is null OR FIND_IN_SET(p1.org_id,grp.org_id)>0))
												AND (if(grp.user_type='All',p1.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(p1.user_type,''),grp.user_type)>0 ))
												AND (if(grp.employment_type='All',p1.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(p1.employment_type,''),grp.employment_type)>0))
												AND (if(grp.country='All',p1.country is not null,grp.country is null OR FIND_IN_SET(ifnull(p1.country,''),grp.country)>0))
  											AND (if(grp.state='All',p1.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(p1.country,'-',p1.state),''),grp.state)>0))
  											AND (if(grp.department='All',p1.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(p1.dept_code,''),grp.department)>0))
	   										AND (if(grp.job_role='All',lpjm.id is not null,grp.job_role IS NULL OR (FIND_IN_SET(ifnull(lpjm.job_role,''),ifnull(grp.job_role,''))>0)))
  											AND (if(grp.language='All',p1.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(p1.preferred_language,''),grp.language)>0))
 												AND (if(grp.is_manager='N',1=1,(ifnull(p1.is_manager,'N') = grp.is_manager))
       									AND	if(grp.is_instructor='N',1=1,(ifnull(p1.is_instructor,'N') = grp.is_instructor)))
												AND (grp.removed_users is null OR FIND_IN_SET(p1.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(p1.id,grp.added_users)>0))))");
  		$select1->addField('p1','id');
  		$select1->condition('p1.status','cre_usr_sts_atv','=');
  		$select1->condition('p1.id', $loggedInUserId, '!=');
  		
  		$select->leftJoin($select1,'gu','gu.id = p.id');
  		
		if($loggedInUserId <= 0 || empty($grpId)){
			$select->where('FIND_IN_SET(ifnull(gu.id,\'\'),p.id)<=0');
		}else if(!empty($grpId)){
			$select->where("(FIND_IN_SET(grp.id,'$grpId')>0 OR (FIND_IN_SET(IFNULL(gu.id,''),p.id)<=0))");
		}
	  // Select a specific user
	  if($username != "null" && $username != t('LBL181')){
	  	$select->condition(db_or()
  	                     ->condition('p.user_name', '%' . db_like($username) . '%', 'LIKE')
  	                     ->condition('p.full_name', '%' . db_like($username) . '%', 'LIKE')
  	                  );
	  }
	  
	  // Get count of active users since $interval
	  $select->addExpression('COUNT(DISTINCT u.uid)', 'count');*/
		
		$select = getUserCatalogQuery($uid, $username,$callfromAPI,$loggedInUserId,$grpId,1);
		
		expDebug::dPrintDBAPI(' Fetch users list query 2 = ' , $select );
	  
		$onlineUsersCount = $select->countQuery()->execute()->fetchField();
		expDebug::dPrintDBAPI(' Fetch users list query 2 = ' , $select );
		expDebug::dPrint("total count of user----->".$onlineUsersCount);
		
	  // Execure the query
	 // $onlineUsersCount = $select->execute()->fetchField();
	  //expDebug::dPrint(' $onlineUsersCount = ' . print_r($onlineUsersCount, true),3);
	 //  expDebug::dPrintDBAPI(' Fetch users list query 1 = ' ,$select);
	
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
	  //expDebug::dPrint(' $start = ' . print_r($start, true),4);
	  
	  $select = getUserCatalogQuery($uid, $username,$callfromAPI,$loggedInUserId,$grpId,0);
	  
	  // Limit to $limit records
	  $select->range($start, $limit); 
	  
	  // Execure the query and fetch all the records
	  $onlineUsersList = $select->execute()->fetchAll();
	  expDebug::dPrintDBAPI(' Fetch users list query 2 = ' , $select );
	  expDebug::dPrint(' $items = ' . print_r($onlineUsersList, true),3);
	  if($callfromAPI){
	    return $onlineUsersList;
	  }
	  // Prepare and send the response
	  $response= new stdClass();
	  $response->page = $page;
	  $response->total = $total_pages;
	  $response->records = $onlineUsersCount;
	  
	  $i=0;
	  foreach ($onlineUsersList as $onlineUser) {
	    //expDebug::dPrint(' $onlineUser = '. print_r($onlineUser, true),3);
	
	    $response->rows[$i]['id']=$onlineUser->uid; 
	    $response->rows[$i]['personid']=$onlineUser->personid; 
	    $response->rows[$i]['cell']['id'] = $onlineUser->uid;
	    $response->rows[$i]['cell']['personid'] = core_encrypt($onlineUser->personid);
	    $response->rows[$i]['cell']['job_title'] = sanitize_data($onlineUser->job_title); 
	    $response->rows[$i]['cell']['css'] = 'exp-usercatalog-user-rec';
	    
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
	 	} else{
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
	    $response->rows[0]['cell']['css'] .= ' exp-catalogusers-first-rec';
	    $response->rows[count($onlineUsersList) - 1]['cell']['css'] .= ' exp-catalogusers-last-rec';
	  }
	   return $response;
  }catch(Exception $ex) {	
	  watchdog_exception('fetch_catalog_users_list_rs', $ex);
	  expertusErrorThrow($ex);
  }
}

function getUserCatalogQuery($uid, $username,$callfromAPI = 0,$loggedInUserId,$grpId,$countQuery=0){
	try{
			// Select sessions table
			$select = db_select('users', 'u');			 
			$select->innerJoin('slt_person', 'p', 'u.name = p.user_name');			
			if($countQuery==0){
				$select->leftJoin('slt_profile_list_items', 'pli', 'p.job_title = pli.code');
				$select->leftjoin('file_managed', 'pic', 'u.picture = pic.fid');
				$select->distinct();
				$select->addField('u', 'uid', 'uid');
				$select->addField('p', 'id', 'personid');
				$select->addField('p', 'full_name', 'full_name');
				$select->addField('pic', 'uri', 'uri');
				$select->addField('pli', 'name', 'job_title');
			}else{
				$select->addExpression('u.uid', 'uid');
			}
			// Set the conditions
			$select->condition('u.uid', array(0,$uid), 'NOT IN');
			$select->condition('u.status', 1, '=');
			$select->condition('p.status', 'cre_usr_sts_atv', '=');
			$select->condition('p.user_name', array('admin', 'guest'), 'NOT IN');
			if($loggedInUserId != 1){
				//$select->leftjoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id = p.id ');
				if(!empty($grpId)){
					//$select->leftJoin('slt_group_user_mapping', 'grp','grp.group_status = \'cre_sec_sts_atv\' and grp.user_id=p.id AND grp.group_id IN ('.$grpId.')');
					$select->leftJoin('slt_group_user_mapping', 'grp','grp.group_status = \'cre_sec_sts_atv\' and grp.user_type != \'R\' and grp.user_id=p.id and grp.group_type=0');
					$select->where('grp.group_id IN ('.$grpId.') OR grp.group_id IS NULL');
				}
// 				$select1 = db_select('slt_person','p1');
// 				$select1->where('p1.id NOT IN(select user_id from slt_group_user_mapping)');
// 				$select1->addField('p1','id');
// 				$select1->condition('p1.status','cre_usr_sts_atv','=');
// 				$select1->condition('p1.id', $loggedInUserId, '!=');
// 				$select->leftJoin($select1,'gu','gu.id = p.id');
			}
		//Select a specific user
		if($username != "null" && $username != t('LBL181')){
			$select->condition(db_or()
					->condition('p.user_name', '%' . db_like($username) . '%', 'LIKE')
					->condition('p.full_name', '%' . db_like($username) . '%', 'LIKE')
			);
		}
		if(!empty($grpId) && ($loggedInUserId != 1)){
			$select->groupBy("p.id");
		}
		// Descending order by full_name
		$select->orderBy('full_name', $sord);
		return $select;
	}catch(Exception $ex) {	
	  watchdog_exception('fetch_catalog_users_list_rs', $ex);
	  expertusErrorThrow($ex);
  	}									
}

/*
 * fetch users list for autocomplete
 */
function getUserCatalogAutoComplete(){
  try{
  	global $user;
  	$loggedInUserId = getIdOfLoggedInUser();
  	if($loggedInUserId != 1){
			$grpId = getGroupId($loggedInUserId);
		}
  	$select = db_select('users', 'u');
	      
	  $select->leftJoin('slt_person', 'p', 'u.name = p.user_name');
	 // $select->leftJoin('slt_profile_list_items', 'pli', 'p.job_title = pli.code');
	  
	  $select->addField('u', 'name', 'username');
	
	  // Set the conditions
	  $select->condition('u.uid', array(0,$user->uid), 'NOT IN');
	  $select->condition('u.status', 1, '=');
	  $select->condition('p.status', array('cre_usr_sts_atv'), 'IN');
	  $select->condition('p.user_name', array('admin', 'guest'), 'NOT IN');
	  if($loggedInUserId != 1){
		  $select->leftjoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id = p.id ');
		  if(!empty($grpId)){
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
		  	$select->leftjoin('slt_group_attributes','grpatt','grpatt.group_id = grp.id AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,\'%Y-%m-%d\') <= DATE_FORMAT(p.hire_date,\'%Y-%m-%d\'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,\'%Y-%m-%d\') >= DATE_FORMAT(p.hire_date,\'%Y-%m-%d\'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(p.hire_date,\'%Y-%m-%d\') between DATE_FORMAT(grpatt.between_start_date,\'%Y-%m-%d\') AND DATE_FORMAT(grpatt.between_end_date,\'%Y-%m-%d\'), 1=0),1=1)
		  	)');
		  }
			
	  $select1 = db_select('slt_person','p1');
	  		$select1->leftJoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id = p1.id');
	  		$select1->innerJoin('slt_groups','grp',"grp.status = 'cre_sec_sts_atv' AND grp.is_admin=0 AND (((((if(grp.org_id='All',p1.org_id is not null,grp.org_id is null OR FIND_IN_SET(p1.org_id,grp.org_id)>0))
													AND (if(grp.user_type='All',p1.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(p1.user_type,''),grp.user_type)>0 ))
													AND (if(grp.employment_type='All',p1.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(p1.employment_type,''),grp.employment_type)>0))
													AND (if(grp.country='All',p1.country is not null,grp.country is null OR FIND_IN_SET(ifnull(p1.country,''),grp.country)>0))
	  											AND (if(grp.state='All',p1.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(p1.country,'-',p1.state),''),grp.state)>0))
	  											AND (if(grp.department='All',p1.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(p1.dept_code,''),grp.department)>0))
		   										AND (if(grp.job_role='All',lpjm.id is not null,grp.job_role IS NULL OR (FIND_IN_SET(ifnull(lpjm.job_role,''),ifnull(grp.job_role,''))>0)))
	  											AND (if(grp.language='All',p1.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(p1.preferred_language,''),grp.language)>0))
	 												AND (if(grp.is_manager='N',1=1,(ifnull(p1.is_manager,'N') = grp.is_manager))
	       									AND	if(grp.is_instructor='N',1=1,(ifnull(p1.is_instructor,'N') = grp.is_instructor)))
	  										AND (grp.removed_users is null OR FIND_IN_SET(p1.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(p1.id,grp.added_users)>0))))");
	  		$select1->leftJoin('slt_group_attributes','grpatt','grpatt.group_id = grp.id AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,\'%Y-%m-%d\') <= DATE_FORMAT(p1.hire_date,\'%Y-%m-%d\'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,\'%Y-%m-%d\') >= DATE_FORMAT(p1.hire_date,\'%Y-%m-%d\'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(p1.hire_date,\'%Y-%m-%d\') between DATE_FORMAT(grpatt.between_start_date,\'%Y-%m-%d\') AND DATE_FORMAT(grpatt.between_end_date,\'%Y-%m-%d\'), 1=0),1=1))');
	  		$select1->addField('p1','id');
	  		$select1->condition('p1.status','cre_usr_sts_atv','=');
	  		$select1->condition('p1.id', $loggedInUserId, '!=');
	  		
	  		$select->leftJoin($select1,'gu','gu.id = p.id');
	  		
	  	if(empty($grpId)){
	  		
				$select->where('FIND_IN_SET(ifnull(gu.id,\'\'),p.id)<=0');
			}else if(!empty($grpId)){
			//	$select->condition('grp.id','','IS NOT NULL');
				//$select->where("(FIND_IN_SET(grp.id,'$grpId')>0 OR (FIND_IN_SET(IFNULL(gu.id,''),p.id)<=0))");
				$select->groupBy("p.id");
			}
	  }
	  
  	
		$username = $_GET['z'];
	  //Select a specific user
	  if($username != "null" && $username != t('LBL181')){
	  	$select->condition(db_or()
  	                     ->condition('p.user_name', '%' . db_like($username) . '%', 'LIKE')
  	                     ->condition('p.full_name', '%' . db_like($username) . '%', 'LIKE')
  	                  );
	  }
	  
	  // Descending order by full_name
	  $select->orderBy('full_name', $sord);
	  
	  // Execure the query and fetch all the records
	  $onlineUsersList = $select->execute()->fetchAll();
	  
	  expDebug::dPrintDBAPI(' Fetch users list for autocomplete ' , $select );
	  
	  foreach ($onlineUsersList as $onlineUser) {
		print $onlineUser->username."\n";
	  }
  }catch (Exception $ex) {
    watchdog_exception('getUserCatalogAutoComplete', $ex);
    expertusErrorThrow($ex);
  } 
}
function getUserProfileDetails($drupal_user_id){
  try{
  // Select drupal users table
  $select =  db_select('users', 'u');

  // Left join the file_managed table to get the picture name and picture uri
  $select->leftjoin('file_managed','fm', 'u.picture = fm.fid');
  
  // Left join the slt_person table
  $select->leftJoin('slt_person','p','u.name = p.user_name');

  // Left join the slt_profile_list_items, slt_person table (to get manager's name), slt_organization to slt_person table
  $select->leftJoin('slt_profile_list_items', 'pli', ' p.time_zone = pli.code');
  $select->leftJoin('slt_person', 'mgr', 'p.manager_id = mgr.id');
  $select->leftJoin('slt_organization','org', 'p.org_id = org.id');
  $select->leftJoin('slt_location','loc', 'p.preferred_loc_id = loc.id');

  // Fetch these fields from the users table
  $select->addField('u','uid');

  // Fetch these fields from the slt_person table
  $select->addField('p','id');
  $select->addField('p','full_name');
  $select->addField('p','user_name');
  $select->addField('loc','name','location_name');
  $select->addField('p','email');
  
  // Fetch these fields from the slt_organization table
  $select->addField('org','name');
  
  // Fetch these fields from the slt_profile_list_items table
  $select->addField('pli','name','timezonename');
  
  // Fetch these fields from the file_managed table
  $select->addField('fm','uri');
  
  // Set the condition
  $select->condition('u.uid', $drupal_user_id);
  expDebug::dPrintDBAPI(' $select obj = ' , $select);
  
  // Execute the query and fetch the result
  $result = $select->execute()->fetchObject();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  
  if(!empty($result->uri)) {
    $result->filename = file_create_url($result->uri);     
  }
  else {
    $result->filename = file_create_url('sites/default/files/pictures/default_user.png');
  }
  unset($result->uri); // Remove uri from the result as not needed anymore.

  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  return $result;
  }catch (Exception $ex) {
    watchdog_exception('getUserProfileDetails', $ex);
    expertusErrorThrow($ex);
  } 
}

function getUserJobtitleDetails($user_id) {
  try{
  $select = db_select('users', 'u');
  // Left join the slt_person table.
  $select->leftJoin('slt_person', 'p', 'u.name= p.user_name');
  // Left join slt_profile_list_items table to get job title name
  $select->leftJoin('slt_profile_list_items', 'pli', 'p.job_title = pli.code');
  // Select fields/expression values to be fetched from the database.
  $select->addField('pli', 'name', 'job_title');
  $select->condition('u.uid', $user_id);
  $result = $select->execute()->fetchObject();
  return $result->job_title;
  }catch (Exception $ex) {
    watchdog_exception('getUserJobtitle', $ex);
    expertusErrorThrow($ex);
  } 
  
}



?>