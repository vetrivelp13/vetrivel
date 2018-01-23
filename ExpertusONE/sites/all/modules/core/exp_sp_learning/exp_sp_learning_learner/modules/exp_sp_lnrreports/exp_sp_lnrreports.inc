<?php
global $next,$finalArray;

/*
 * learnerReportsSearchResults()- Returns the report list and action links in the search page.
 */
function learnerReportsSearchResults(){
	try {
		$adminAccess = reportAdminAccess();

		$reportDetails = getLearnerReportsDetails($adminAccess);
		$userId = getSltpersonUserId();
		expDebug::dPrint('User Access : '.user_access('administrator') , 4);
		$srch_result_response= new stdClass();
		$srch_result_response->total   = $reportDetails['total_pages'];
		$srch_result_response->records = $reportDetails['count'];
		$srch_result_response->page    = $reportDetails['page'];
		$managed_By = $reportDetails['manage_by_selector'];
		expDebug::dPrint('$manage_by_selector  : '.$filter_By, 4);
		$qry_search_results  = $reportDetails['result'];
		$_SESSION['jq_count'] = array();
		$i=0;
		$availableTypes = array();

		foreach ($qry_search_results as $rec_search_results) {

			$rec_search_results->row =  $i;
			$srch_result_response->rows[$i]['id']   = $rec_search_results->id;
			$rec_search_results->widgetId = 'lnr-reports-search';

			$srch_result_response->rows[$i]['cell'] = array(
					'details'             => theme('exp_sp_lnrreports-view-detail', array('results' => $rec_search_results, 'admin_access' => $adminAccess)),
					'action'              => theme('exp_sp_lnrreports-view-action', array('results' => $rec_search_results, 'admin_access' => $adminAccess))
			);
			$_SESSION['jq_modal'][$i] = $rec_search_results->id;
			$i++;
		}
		$is_mgr = is_manager($userId);
		$is_ins = is_instructor($userId);
		$utype = $is_mgr?($is_ins?"manager,instructor":"manager"):($is_ins?"instructor":'');
		expDebug::dPrint("utype : ".$utype);
		$grpIds = getGroupsDetails($userId,'','',$utype,0);
		foreach($grpIds as $key => $gId){
			$gIds[]= $gId->id;
		}
		$managedByOptions = getManagedByFilters();
		$selectType = db_select('slt_profile_list_items', 'profile');
		$selectType->addField('profile', 'code','code');
		$selectType->addField('profile', 'name','value');
		$selectType->condition('profile.code', 'cre_rpt_rpc_%','LIKE');
		$selectType->orderBy('profile.name', 'ASC');
		expDebug::dPrintDBAPI(' $selectType ', $selectType);
		$typeArray = $selectType->execute()->fetchAll();
		/*commented by ranjani -start*/
		/*$selectRole = db_select('slt_profile_list_items', 'profile');
		 $selectRole->addField('profile', 'code','code');
		$selectRole->addField('profile', 'name','value');
		$selectRole->condition('profile.code', 'cre_rpt_vis_%','LIKE');
		$roleArray = $selectRole->execute()->fetchAll();*/
		/*commented by ranjani -end*/
		/*added by ranjani -start*/
		 if(!user_access('administrator') && !is_data_admin()){
			$roleArray = $GLOBALS['user']->roles;
			$rNameArr = array();
			foreach($roleArray as $rid => $rname){
				$rNameArr[]= $rname;
			}

			$checkIfViewAll = db_select('slt_groups', 'grp');
			$checkIfViewAll->addField('grp', 'id', 'code');
			$checkIfViewAll->addField('grp', 'name', 'value');
			$checkIfViewAll->condition('grp.name', $rNameArr, 'IN');
			$checkIfViewAll->condition('grp.status', 'cre_sec_sts_atv', '=');
			$checkIfViewAll->orderBy('grp.name', 'ASC');
			$or = db_or();
			$or->where('FIND_IN_SET(\'View Report Perm\',grp.access_permissions) >= 1');
			$or->where('FIND_IN_SET(\'Create Report Perm\',grp.access_permissions) >= 1');
			$or->where('FIND_IN_SET(\'Assign Report Perm\',grp.access_permissions) >= 1');
			$checkIfViewAll->condition($or);
			expDebug::dPrintDBAPI(' $checkIfViewAll ', $checkIfViewAll);
			$secRoleRes = $checkIfViewAll->execute()->fetchAll();

			/*if($checkIfViewAllRes || is_manager($userId) || is_instructor($userId)){
				//report permission group user
				$secRoleQry = db_select('slt_groups', 'grp');
				$secRoleQry->addField('grp', 'id', 'code');
				$secRoleQry->addField('grp', 'name', 'value');
				$secRoleQry->condition('grp.status', 'cre_sec_sts_atv', '=');
				//$secRoleQry->condition('grp.id', $gIds, 'IN');
				$secRoleQry->condition('grp.name', $rNameArr, 'IN');
				expDebug::dPrintDBAPI(' $secRoleQry ', $secRoleQry);
				$secRoleRes = $secRoleQry->execute()->fetchAll();
			}
			else{
				$checkIfViewAss = db_select('slt_groups', 'grp');
				$checkIfViewAss->addField('grp', 'id', 'code');
				$checkIfViewAss->addField('grp', 'name', 'value');
				$checkIfViewAss->condition('grp.name', $rNameArr, 'IN');
				$checkIfViewAss->condition('grp.status', 'cre_sec_sts_atv', '=');
				$checkIfViewAss->where('FIND_IN_SET(\'Assign Report perm\',grp.access_permissions) >= 1');
				expDebug::dPrintDBAPI(' $checkIfViewAss ', $checkIfViewAss);
				$checkIfViewAssRes = $checkIfViewAss->execute()->fetchAll();
				if($checkIfViewAssRes){
					//Assign report perm group user
					$secRoleQry = db_select('slt_groups', 'grp');
					$secRoleQry->addField('grp', 'id', 'code');
					$secRoleQry->addField('grp', 'name', 'value');
					$secRoleQry->condition('grp.status', 'cre_sec_sts_atv', '=');
					$secRoleQry->condition('grp.id', $gIds, 'IN');
					expDebug::dPrintDBAPI(' $select ', $secRoleQry);
					$secRoleRes = $secRoleQry->execute()->fetchAll();
				}
			} */
		}else{
			//Admin role user
			$secRoleQry = db_select('slt_groups', 'grp');
			$secRoleQry->addField('grp', 'id', 'code');
			$secRoleQry->addField('grp', 'name', 'value');
			$secRoleQry->condition('grp.status', 'cre_sec_sts_atv', '=');
			$secRoleQry->condition('grp.code', 'grp_sup', '!=');
			$secRoleQry->orderBy('grp.name', 'ASC');
			expDebug::dPrintDBAPI('Narrow search Filter reports Roles = ' ,$secRoleQry);
			$secRoleRes = $secRoleQry->execute()->fetchAll();
		}
		/*added by ranjani -end*/
		$srch_result_response->filter = theme('exp_sp_lnrreports-view-filter', array('results' => '', 'access' => getAvailableReportTypes($adminAccess), 'types'  => $typeArray,
				'roles'=> array('default_role'=>$secRoleRes),'managedby'  => $managedByOptions, 'admin_access' => $adminAccess,'filter_By' =>$managed_By)); // filter_By is added for Checkbox Default selection To User
		expDebug::dPrint('$srch_result_response : '.print_r($srch_result_response, true), 4);
		echo drupal_json_output($srch_result_response);

	} catch (Exception $ex) {
		watchdog_exception('learnerReportsSearchResults', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * reportPrintExport() - To export the report list as a CSV
 */
function reportPrintExport($type = ''){
	try {
	if(user_is_logged_in() === false) {
	  drupal_access_denied();
	  exit();
	}
  $adminAccess = reportAdminAccess();
  getLearnerReportsDetails($adminAccess, $type);
  } catch (Exception $ex) {
  	watchdog_exception('reportPrintExport', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getLearnerReportsDetailsQuery() - To get Report Details
 */
function getLearnerReportsDetailsQuery($adminAccess, $type = '',$manageBy='',$from=''){
  
  try{
    
    //$access = getLearnerUserAccessPrivileges($adminAccess);
  // Vincent Added on 11 Aug, 2015 for # 0052759 item 4
    foreach($_GET as $name=>$val){
    	$_GET[$name] = reportXSSValidate($val,$name,0);
    }
    
    $page      = is_null($_GET['page'])? $_GET['page'] : orgValidate($_GET['page']); 
    $limit     = is_null($_GET['rows'])? $_GET['rows'] : orgValidate($_GET['rows']); 
    $sidx      = stripApostrophe($_GET['sidx']);
    $sord      = stripApostrophe($_GET['sord']);
    $tab       = $_GET['tab'];
    $sortby    = stripApostrophe($_GET['sortby']);
    $title      = $_GET['title']; 
    
    $reportStatus   = stripApostrophe($_GET['report_status']);
    $reportTypes    = stripApostrophe($_GET['report_types']);
    $reportName     = $_GET['report_name'];
    $userId = getSltpersonUserId();
    $is_mgr = is_manager($userId);
    $is_ins = is_instructor($userId);
    $utype = $is_mgr?($is_ins?"manager,instructor":"manager"):($is_ins?"instructor":'');
    $grpIds = getGroupsDetails($userId,'','',$utype,0);
    foreach($grpIds as $key => $gId){
    	$gIds[]= $gId->id;
    }
    $query = db_select('slt_report_details', 'rep');
    
    if($type != 'count'){
      $query->leftjoin('slt_profile_list_items', 'items', 'rep.status = items.code');
    }
    
    if($type == 'count'){
      
      $query->addExpression('count(rep.id)', 'total_count');
      
    } else if($type == 'roles'){
      
      $query->addField('rep','report_visibility');
      
    } else {
    
      $query->addField('rep', 'id', 'id');
      $query->addField('rep', 'title', 'title');
      $query->addField('rep', 'description', 'description');
      $query->addField('rep', 'report_visibility', 'report_visibility');
      $query->addField('rep', 'report_builder_type', 'report_type');
      $query->addField('rep', 'status', 'status');
      $query->addField('items', 'name', 'status_name');
      $query->addExpression('(select GROUP_CONCAT(name) from slt_groups where FIND_IN_SET(id, rep.report_visibility ) >=1 ) ', 'report_visibility_name');
      
    }
     if(!user_access('administrator') && !is_data_admin()){
		/*added by ranjani -start */
    	$listOfRolesForLoggedUser = $GLOBALS['user']->roles;
	    $secRoleQry = db_select('slt_groups', 'grp');
	    $secRoleQry->addField('grp', 'id', 'sec_role_id');
	    $rNameArr = array();
	  	foreach($listOfRolesForLoggedUser as $rid => $rname){
		    $rNameArr[]= $rname;   	
			}
	    $secRoleQry->condition('grp.name', $rNameArr, 'IN');
	    expDebug::dPrintDBAPI('Reports - $secRoleQryr',$secRoleQry );
	    $secRoleRes = $secRoleQry->execute()->fetchAll();
	    expDebug::dPrint('REPORT ROLE IDS-->'.print_r($secRoleRes,1),4);
	    $orQuery = db_or();
    	$checkIfViewAll = db_select('slt_groups', 'grp');
	    $checkIfViewAll->addField('grp', 'id', 'code');
	    $checkIfViewAll->addField('grp', 'name', 'value');    
	    $checkIfViewAll->condition('grp.name', $rNameArr, 'IN');
	    $or = db_or();
	    $or->where('FIND_IN_SET(\'View Report Perm\',grp.access_permissions) >= 1');
	    $or->where('FIND_IN_SET(\'Create Report Perm\',grp.access_permissions) >= 1');
	    $or->where('FIND_IN_SET(\'Assign Report Perm\',grp.access_permissions) >= 1');
	    $checkIfViewAll->condition($or);	    
	    $checkIfViewAllRes = $checkIfViewAll->execute()->fetchAll();
	    
	    expDebug::dPrint('REPORT ROLE IDS$checkIfViewAllRes-->'.print_r($checkIfViewAllRes,1),4);
	    //Modified by Priya for #0036482
	    /* if($checkIfViewAllRes || is_manager($userId) || is_instructor($userId)){	 
	    	$orQuery->where('1=1');
	    }  *//* else{	  
	    	
	    	$checkIfViewAss = db_select('slt_groups', 'grp');
		    $checkIfViewAss->addField('grp', 'id', 'code');
		    $checkIfViewAss->addField('grp', 'name', 'value');    
		    $checkIfViewAss->condition('grp.name', $rNameArr, 'IN');
		    $checkIfViewAss->where('FIND_IN_SET(\'Assign Report perm\',grp.access_permissions) >= 1');
		    $checkIfViewAssRes = $checkIfViewAss->execute()->fetchAll();
		 		 if($checkIfViewAssRes){
			    foreach($secRoleRes as $roleRecord){
			    	//$orQuery->where('FIND_IN_SET(\''.$roleRecord->sec_role_id.'\', rep.report_visibility) > 0');
			    	$orQuery->where('FIND_IN_SET('.$roleRecord->sec_role_id.',rep.report_visibility)');	 
		    	}
		    } */
		    /* else if(is_manager($userId) || is_instructor($userId)){
		    	 foreach($secRoleRes as $roleRecord){
		    		$orQuery->where('(FIND_IN_SET('.$roleRecord->sec_role_id.',rep.report_visibility ) )');
		    	} 
		    } */   	
	   // } 
	    /*added by ranjani -end*/ 
	    /*commented by ranjani -start*/ 
	    /*if($access['cre_rpt_vis_adm'] == TRUE){
	      $orQuery->where('(FIND_IN_SET(\'cre_rpt_vis_adm\',rep.report_visibility ) )');
	    } 
	    if($access['cre_rpt_vis_mgr'] == TRUE){
	      $orQuery->where('(FIND_IN_SET(\'cre_rpt_vis_mgr\',rep.report_visibility ) )');
	    } 
	    if($access['cre_rpt_vis_ins'] == TRUE){
	      $orQuery->where('(FIND_IN_SET(\'cre_rpt_vis_ins\',rep.report_visibility ) )');
	    }
	    if($access['cre_rpt_vis_usr'] == TRUE){
	      $orQuery->where('(FIND_IN_SET(\'cre_rpt_vis_usr\',rep.report_visibility ) )');
	    }*/
	    /*commented by ranjani -end*/ 
	   /*  $query->condition($orQuery);
	    expDebug::dPrintDBAPI('Reports - not administrator',$query );*/
	    if($checkIfViewAllRes){
	    //	$orQuery->where('1=1');
	    	$orQuery->where('rep.report_visibility is null or rep.created_by="'.$userId.'"');
	    }
	    if(!empty($secRoleRes) && empty($_GET['report_roles'])){
	    	foreach($secRoleRes as $roleRecord){
	    		expDebug::dPrint('$$$roleRecord  : '.print_r($roleRecord->sec_role_id,1), 4);
	    		//$orQuery->where('FIND_IN_SET(\''.$roleRecord->sec_role_id.'\', rep.report_visibility) > 0');
	    		$orQuery->where('FIND_IN_SET('.$roleRecord->sec_role_id.',rep.report_visibility)');
	    	}
	    	$query->condition($orQuery);
	    	expDebug::dPrint('$$orQuery  : '.print_r($orQuery,1), 4);
	    }
    } 
    if(user_access('Create Report Perm')){
    	if(!empty($reportStatus)){
    		$reportStatusArray = explode('|', $reportStatus);
    		$query->condition('rep.status', $reportStatusArray, 'IN');
    	}
    	}
    else {
      $or = db_or();
      $or->condition('rep.status', 'cre_rpt_rps_atv', '=');
      $or->condition('rep.status', 'cre_rpt_rps_dft', '=');
      $query->condition($or);
    }
    
    // Deleted entry should not be displayed
    $query->condition('rep.status', array('cre_rpt_rps_del'), 'NOT IN');
    if(!empty($from)){
    	$query->condition('rep.status', array('cre_rpt_rps_itv'), 'NOT IN');
    }
    //getUserListField()
    //Modified by Priya for #0036482
    expDebug::dPrint("asdasdasd".print_r($_GET,true),5);
    if(isset($_GET['report_roles'])){
    	if(!empty($_GET['report_roles'])){
	    	$reportRolesArray = explode('|',stripApostrophe($_GET['report_roles']));
	      $reportRolesSetArray = array();
	      foreach($reportRolesArray as $rpRole){
	        $reportRolesSetArray[] = 'FIND_IN_SET(\''.$rpRole.'\', rep.report_visibility)';
	      }
	      //$whereCondition = ($checkIfViewAllRes) ? '1=1' : implode(' or ',$reportRolesSetArray);
	      $whereCondition =implode(' or ',$reportRolesSetArray);
    	}else{
    		$whereCondition = '1=1';
    	}
	      $manageByFilterValArr = empty($_GET['report_manage'])? array() : explode('|', $_GET['report_manage']);
	      $numFilterValues = count($manageByFilterValArr);
	      if($numFilterValues == 0 || $numFilterValues == 2){
	      	$query->where('('. $whereCondition .')');
	      }elseif($manageByFilterValArr[0] == 'cre_sys_fop_me'){
	      	$query->where('(rep.created_by = '.$userId.' and ('.$whereCondition.' ))');
	      }else{
	      	//if($checkIfViewAllRes)
	      		$query->where('((rep.created_by != '.$userId.' or rep.created_by is null ) and ('.$whereCondition.' ))');
	      	//else
	      		//$query->where('( '. $whereCondition .') ');
	      }
    }else if(!empty($manageBy)){
    	expDebug::dPrint('$manageBy  : '.$manageBy, 4);
    	// if user is looking for Me or need a Count of the Person
    	if($manageBy == 'me' || $manageBy == 'count') {
	    	$query->where('(rep.created_by = '.$userId.')');
    	}
    }else{
    	$query->where('(rep.created_by = '.$userId.')');
    }
    
    if(!empty($title)){
      $query->condition('rep.title', '%'.db_like($title).'%', 'like');
    }
    
    if(!empty($reportTypes)){
      $reportTypesArray = explode('|', $reportTypes);
      $query->condition('rep.report_type', $reportTypesArray, 'IN');
    }
    
    if(!empty($reportName)){
      $query->condition('rep.title', '%'.db_like($reportName).'%', 'LIKE');
    }
    if($type == 'roles'){
      $query->groupby('rep.report_visibility');
    }
    
    // Sorting related
    $sord = $sord==''?'asc' : $sord;
    if($sortby == 'AZ'){
      $query->orderBy('rep.title', 'asc');
    } else if($sortby == 'ZA'){
      $query->orderBy('rep.title', 'desc');
    } else if($sortby == 'Time'){
      $query->orderby('rep.created_on', 'desc');
      $query->orderby('rep.updated_on', 'desc');
    } else {
      $query->orderBy('rep.title', 'asc');
    }

    expDebug::dPrintDBAPI('Reports - Query string : ',$query );
    
    return $query;
    
  } catch (Exception $ex) {
    
    watchdog_exception('learnerReportsSearchResults', $ex);
    expertusErrorThrow($ex);
    
  }    
    
}

/*
 * getLearnerReportsDetails() - To get Report detaiils with pagination
 */
function getLearnerReportsDetails($adminAccess, $type = ''){
  
  try {
  	expDebug::dPrint("testing the report details".print_r($_GET,1));
  	//expDebug::dPrint("func_get_args in getLearnerReportsDetails ".print_r(func_get_args(), 1));
  	// Vincent Added on 11 Aug, 2015 for # 0052759 item 4
    $page      = orgValidate(reportXSSValidate($_GET['page'])); 
    $limit     = orgValidate(reportXSSValidate($_GET['rows'])); 
    $sidx      = reportXSSValidate($_GET['sidx']);
    $sord      = reportXSSValidate($_GET['sord']);
    $tab       = reportXSSValidate($_GET['tab']);
    $sortby    = reportXSSValidate($_GET['sortby']);
    
    $count = 0;
    
    if ($type == "export") {
      $query = getLearnerReportsDetailsQuery($adminAccess, '');
  	  $colHeadersMap = array(t('LBL083') => 'title',
  	  						 t('LBL229') => 'description',
                             t('LBL102') => 'status_name',
                             t('Role') => 'report_visibility_name',
  	                         t('LBL036') => 'report_type',
                         );
                        
      $filterArray[] = array(t('Report').':'.textboxFiltersetValueToPrintableStr('title'));
      if ($adminAccess) { // This check is in getLearnerReportsDetailsQuery(), so checking here as well.
        $filterArray[] = array(t('LBL102').':'.lnrReportsCheckboxFiltersetSelectionsToString('report_status'));
      }
       $filterArray[] = array(t('Groups').':'.checkboxRoleFiltersetSelectionsToPrintableStr('report_roles'));
       $filterArray[] = array(t('LBL036').':'.checkboxFiltersetSelectionsToPrintableStr('report_types'));
       $filterArray[] = array(t('Managed By').':'.checkboxFiltersetSelectionsToPrintableStr('report_manage'));
      //$appliedFilters[t('LBL226')] = textboxFiltersetValueToPrintableStr('report_name');
      $filterArray[] = array(ucfirst(t('LBL011')).':'.getNarrowSearchSortTitle());
      SendNarrowSearchResultsAsCSVFile($query, $colHeadersMap, 'export', $filterArray);
      
    } else if($type == "print") {
      
      $query = getLearnerReportsDetailsQuery($adminAccess, '');
      // % width of the columns must total to 100%
      $colHeadersMap = array(t('LBL083') => array('title',25),
      					     t('LBL229') => array('description',25),
                             t('LBL102') => array('status_name',25),
      					     t('Role') => array('report_visibility_name',25),
                             t('LBL036') => array('report_type',25),
                          );
      
      $appliedFilters = array();
      $appliedFilters[t('Report')] = textboxFiltersetValueToPrintableStr('title');
      if ($adminAccess) { // This check is in getLearnerReportsDetailsQuery(), so checking here as well.
        $appliedFilters[t('LBL102')] = lnrReportsCheckboxFiltersetSelectionsToString('report_status');
      }
      $appliedFilters[t('Groups')] = checkboxRoleFiltersetSelectionsToPrintableStr('report_roles');
      $appliedFilters[t('LBL036')] = checkboxFiltersetSelectionsToPrintableStr('report_types');
      // manage by me is Fixed For this Ticket #0039575
      $appliedFilters[t('Managed By')] = checkboxFiltersetSelectionsToPrintableStr('report_manage');
      //$appliedFilters[t('LBL226')] = textboxFiltersetValueToPrintableStr('report_name');
      $appliedFilters[ucfirst(t('LBL011'))] = getNarrowSearchSortTitle();
  
      SendNarrowSearchResultsAsPDFFile($query, t('Report'), $appliedFilters, $colHeadersMap);
  
    } else {
    	//Following count query is commented as its not used anywhere here due to #0042384 ticket
    	/*
    	$fetchManageResults = getLearnerReportsDetailsQuery($adminAccess, 'count','count');
    	$countManageResults = $fetchManageResults->execute()->fetchField();
    	// if user Has no Records in Reports Show all the reports to him as Both
    	expDebug::dPrint('$countManageResults : '.$countManageResults, 4);
    	*/
    	
    	//0042384: In the reports search page the managed by filter should not be defaulted to any value.
 /* 	if ($countManageResults > 0) {
    		$manageBy = 'me';
    	}
    	else { */
    		$manageBy = 'both';
    	//}
     
      /*
      $countQuery = getLearnerReportsDetailsQuery($adminAccess, 'count',$manageBy);
      $count = $countQuery->execute()->fetchField();
      */
      
      $query = getLearnerReportsDetailsQuery($adminAccess, '',$manageBy);
      /* Paging Related */
      $start = $limit * $page - $limit;
	  $count = $limit;
      if(isset($_REQUEST['jqgrid_row_id'])) {
      	$start = 0;
      }
      $query->range($start, $limit);
      add_row_id_condition($query, 'rep');
      expDebug::dPrintDBAPI('select query for report fetching getLearnerReportsDetails : ',$query);
      // Execute query
      $queryResult = $query->execute();
      expDebug::dPrint('test query '.print_r($queryResult,true),5);
      if( $count >0 ){ 
        $total_pages = ceil($count/$limit); 
      }else{ 
        $total_pages = 0; 
      }	  
      if($page > $total_pages) 
      	$page = $total_pages;
      if($start < 0) {
      	$start = 0;
      }
    
      $pagination['result'] = $queryResult;
      $pagination['page'] = $page;
      $pagination['count'] = $count;
      $pagination['total_pages'] = $total_pages;
      expDebug::dPrint('$manage_by_selector : '.$manageBy, 4);
      $pagination['manage_by_selector'] = $manageBy;

      /*-- #0049113 - pagination issue fix with SAML --*/
      $crPage = $_COOKIE['current_page'];
      $updatePage='';
      $isFromSearch = true;
      if(!empty($page) && !empty($crPage)){
      	$level = explode('~',$crPage);
      	$isUpdate = explode('#',($isFromSearch==false)?$level[1]:$level[0]);
      	if($isUpdate[1]==1){
      		$page=$isUpdate[0];
      	}else{
      		$isUpdate[0]=$page;
      	}
      	$isUpdate[1]=0;
      	($isFromSearch==false)?$level[1]=implode('#',$isUpdate):$level[0]=implode('#',$isUpdate);
      	$updatePage = implode('~',$level);
      }else if(empty($crPage)){
      	$updatePage = $page.'#0~0#0~0#0';
      }
      setcookie("current_page",$updatePage,null,"/","","","");
      
      return $pagination;
      
    }
    
  } catch (Exception $ex) {
    
    watchdog_exception('getLearnerReportsDetails', $ex);
    expertusErrorThrow($ex);
    
  }

}

/*
 * function lnrReportsCheckboxFiltersetSelectionsToString() - Returns the selected checkbox filters pipe separated
 */
function lnrReportsCheckboxFiltersetSelectionsToString($filtersetCode) {
	try{
  $selectedFilters = getURLParam($filtersetCode);
  expDebug::dPrint(' $selectedFilters = ' . print_r($selectedFilters, true) , 4);
  $selectedFiltersAsString = '';
  if (!empty($selectedFilters)){
    $checkBoxFiltersCodeList = explode("|", $selectedFilters);
    $firstCode = true;
    foreach ($checkBoxFiltersCodeList as $code) {
      if ($code == 'cre_rpt_rps_atv') {
        $value = t('Published');
      }
      else if ($code == 'cre_rpt_rps_itv') {
        $value = t('Unpublished');
      }
      else {
        $value = getProfileListItemName($code);
      }   	
      if ($firstCode) {
        $selectedFiltersAsString .= $value;
        $firstCode = false;
      }
      else {
        $selectedFiltersAsString .= ' | ' . $value;
      }
    } // end foreach
  } // end if
  
  expDebug::dPrint(' $selectedFiltersAsString = ' . print_r($selectedFiltersAsString, true) , 4);
  
  return $selectedFiltersAsString;
  } catch (Exception $ex) {
  	watchdog_exception('lnrReportsCheckboxFiltersetSelectionsToString', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * function checkboxRoleFiltersSelectionsToString() - Returns the selected checkbox filters pipe separated
 *                                                  Assumed, all code is in 
 */
function checkboxRoleFiltersetSelectionsToPrintableStr($filtersetCode) {
	try{
  $selectedFilters = getURLParam($filtersetCode);
  expDebug::dPrint(' $selectedFilters = ' . print_r($selectedFilters, true) , 4);
  $selectedFiltersAsString = '';
  if (!empty($selectedFilters)){
    $checkBoxFiltersCodeList = explode("|", $selectedFilters);
    $firstCode = true;
    foreach ($checkBoxFiltersCodeList as $code) {
      $value = getRoleListItemName($code);
      if ($firstCode) {
        $selectedFiltersAsString .= $value;
        $firstCode = false;
      }
      else {
        $selectedFiltersAsString .= ' | ' . $value;
      }
    } // end foreach
  } // end if
  
  //$selectedFiltersAsString = (empty($selectedFiltersAsString))? 'All' : $selectedFiltersAsString;
  expDebug::dPrint(' $selectedFiltersAsString = ' . print_r($selectedFiltersAsString, true) , 4);
  
  return $selectedFiltersAsString;
  } catch (Exception $ex) {
  	watchdog_exception('checkboxRoleFiltersetSelectionsToPrintableStr', $ex);
  	expertusErrorThrow($ex);
  }
}

function getRoleListItemName($itemCode) {
  try{
   
  // Select table
  $selectStmt = db_select('slt_groups', 'grp');

  // Set conditions
  $selectStmt->condition('grp.id', $itemCode);

  // Select fields and/or add expressions
  $selectStmt->addField('grp', 'name', 'name');

  expDebug::dPrintDBAPI('$selectStmt = ', $selectStmt);

  // Execute query
  $result = $selectStmt->execute();

  if ($result->rowCount() <= 0) {
  	expDebug::dPrint('unknown item code');
    return null;
  }
  $name = $result->fetchField();
  expDebug::dPrint('$name = ' . $name);
  
  return $name;
  }catch (Exception $ex) {
    watchdog_exception('getRoleListItemName', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getReportColumnHeader() - To get report column header.
 */
function getReportColumnHeader($reportId, $aliasOnly = false){
  try {
    $select = db_select('slt_report_query_builder', 'rep');
    if (!$aliasOnly) {
      $select->addField('rep', 'column_sql');
    }
    $select->addField('rep', 'column_alias_sql');
    $select->condition('rep.report_details_id', $reportId);
    $select->condition('rep.union_query', '2', '!=');
    expDebug::dPrintDBAPI('$select', $select);
    
    $result = $select->execute()->fetchAssoc();
    expDebug::dPrint('$result = ' . print_r($result, true));
    
    return $result;
  }
  catch (Exception $ex) {
    watchdog_exception('getReportColumnHeader', $ex);
    
    if (isset($_GET['cron_key'])) {
      expertusLogError($ex);
      expDebug::dPrint('Throwing new ReportSchedulesWorkerException', 4);
      throw new ReportSchedulesWorkerException($ex, 'INFORM-OWNER');
    }
    
    expertusErrorThrow($ex);
  }
}

/**
 * Splits filter criteria values string
 */
function splitFilterCriteriaValuesString($inputNameValues) {
	try {
    $inputNameValuesResultArray = array();
	$criteriaSplitter = (isset($_GET['cron_key']) ? ',' : EXP_AC_SEPARATOR);
	
	$inputNameValuesArray = explode($criteriaSplitter, $inputNameValues);
    expDebug::dPrint('$inputNameValuesArray = ' . print_r($inputNameValuesArray, true), 4);
    foreach ($inputNameValuesArray as $values) {
      if (!empty($values)) {
        list($criteriaId, $criteriaValue) = explode('~~', $values);
        $inputNameValuesResultArray[$criteriaId] = $criteriaValue;
      }
    }
    expDebug::dPrint('$inputNameValuesResultArray: ' . print_r($inputNameValuesResultArray, true), 4);
    return $inputNameValuesResultArray;
	}
	catch (Exception $ex) {
    watchdog_exception('splitFilterCriteriaValuesString', $ex);
    
    if (isset($_GET['cron_key'])) {
      expertusLogError($ex);
      expDebug::dPrint('Throwing new ReportSchedulesWorkerException', 4);
      throw new ReportSchedulesWorkerException($ex, 'INFORM-OWNER');
    }
    
    // will come here only when it is not a scheduled report
    expertusErrorThrow($ex);
	}
}

/**
 * Fetches all criteria of a report with their details
 */
function reportGetCriteriaList($reportId) {
  try {
    $criteriaQuery = db_select('slt_report_criteria', 'criteria');
    
    $criteriaQuery->addField('criteria', 'id');
    $criteriaQuery->addField('criteria', 'name');
    $criteriaQuery->addField('criteria', 'column_name');
    $criteriaQuery->addField('criteria', 'column_alias_name');
    $criteriaQuery->addField('criteria', 'column_type');
    $criteriaQuery->addField('criteria', 'condition_type');
    $criteriaQuery->addField('criteria', 'use_user_details');
    $criteriaQuery->addField('criteria', 'mandatory');
    $criteriaQuery->addField('criteria', 'table_name');
    $criteriaQuery->condition('criteria.report_details_id', $reportId);
    
    expDebug::dPrintDBAPI('$criteriaQuery', $criteriaQuery);
    
    $criteriaList = $criteriaQuery->execute()->fetchAll();
    expDebug::dPrint('$criteriaList = ' . print_r($criteriaList, true));

    return $criteriaList;
  }
  catch (Exception $ex) {
    watchdog_exception('reportGetCriteriaList', $ex);
    
    if (isset($_GET['cron_key'])) {
      expertusLogError($ex);
      expDebug::dPrint('Throwing new ReportSchedulesWorkerException', 4);
      throw new ReportSchedulesWorkerException($ex, 'INFORM-OWNER');
    }
    
    // will come here only when it is not a scheduled report
    expertusErrorThrow($ex);
  }
}

/*
 * Generates the filter criteria sql for adding to the where clause
 */
function buildReportQueryConditions($reportId, $inputNameValues, $queryBuilderSQL, $reportEditable, $scheduleStartTimeObj) {
  try {
  	// Initialize
    $andPrefixedConditions = ''; //when present, starts with an AND
    $andSuffixedConditions = ''; //when present, ends with an AND (so it has to be placed immediately after WHERE in SQL query).
    $queryArguments = array(); // db_query() values for the placeholder arguments in the SQL query
    $filterArray = array(); // For listing the filters in csv export
    $filterPdfArray = array(); // For listing the filters in PDF export
    $orderByColsList = array('mandatory' => array(), 'optional' => array()); // for scheduled reports
    
    $unionQuery = trim($queryBuilderSQL['union_query']);
    expDebug::dPrint('$unionQuery = ' . $unionQuery, 4);
    
    $wherePosition = $queryBuilderSQL['where_position']; // value meaningful only for the non-UNION queries.
    expDebug::dPrint('$wherePosition = ' . $wherePosition, 4);
    
    
    if ($inputNameValues != '0') {
      // Get the filter criteria values enterered by the report user from report launch dialog
      $inputNameValuesResultArray = splitFilterCriteriaValuesString($inputNameValues);
      
      // Get all the filter criteria of this report and their attributes
      $criteriaList = reportGetCriteriaList($reportId);
      
      if (count($criteriaList) > 0) {
        // Initialize
        $placeholderId = 1; // id of the placeholder argument in sql query. Is incremented in formFieldCondition()
        $isFromDateCriteria = true; // To determine whether the date (or datetime) filter criteria is the From Date criteria or the To Date criteria.
                                    // (For date (or datetime) fields, two filter criteria records are created in slt_report_criteria table.
                                    // The lower id is always From Date criteria and the other is To Date criteria.)
        
        // Prepare conditions to be added to the SQL query based on criteria
        foreach ($criteriaList as $criteriaDBVal) {
          
          $criteriaId = $criteriaDBVal->id;
          
          // Get the input value for criteria entered by the user in launched report
          $inputValue = $inputNameValuesResultArray[$criteriaId];
          
          reportPreProcessCriteria($criteriaDBVal, $inputValue, $unionQuery, $reportEditable, $scheduleStartTimeObj);
          
          $columnName = $criteriaDBVal->column_name;
          $criteriaType = $criteriaDBVal->column_type;
          $criteriaCondition = $criteriaDBVal->condition_type;
          
          // If non empty input value, add the criteria to the SQL where clause
          if (!empty($inputValue)) {
            if ($unionQuery || $wherePosition == 0) {
              $retArr = formFieldCondition($columnName, $criteriaCondition, $inputValue, $criteriaType, $unionQuery, 'before', $placeholderId);
              $andPrefixedConditions .= $retArr['condition'];
              expDebug::dPrint("Query ConditionSQL Before : " . $andPrefixedConditions);
              expDebug::dPrint("Query ConditionSQL After : " . print_r($queryArguments, 1), 4);
              expDebug::dPrint("Query ConditionSQL After : " . print_r($retArr['argument'], 1), 4);
              $queryArguments = array_merge($queryArguments, $retArr['argument']);
              expDebug::dPrint('Query $queryArguments Before : ' . print_r($queryArguments, true), 5);
            }
            else {
              $retArr = formFieldCondition($columnName, $criteriaCondition, $inputValue, $criteriaType, $unionQuery, 'after', $placeholderId);
              $andSuffixedConditions .= $retArr['condition'];
              expDebug::dPrint("Query ConditionSQL After : " . $andSuffixedConditions, 4);
              $queryArguments = array_merge($queryArguments, $retArr['argument']);
              expDebug::dPrint('Query $queryArguments After : ' . print_r($queryArguments, true), 5);
            }
            
            // $orderByColList is currently used for scheduled reports
            if ((!empty($criteriaDBVal->mandatory) || !empty($criteriaDBVal->use_user_details)) &&
                                  !in_array($criteriaDBVal->column_alias_name, $orderByColsList['mandatory'])) {
              $orderByColsList['mandatory'][] = $criteriaDBVal->column_alias_name;
            }
            else if ($retArr['multval'] && !in_array($criteriaDBVal->column_alias_name, $orderByColsList['optional'])) {
              $orderByColsList['optional'][] = $criteriaDBVal->column_alias_name;
            }
          }
          
          // Save applied filter and value for showing in CSV and PDF
          // Determine the correct header for date time field.
          if ($criteriaType == 'datetime' || $criteriaType == 'date') {
            if ($isFromDateCriteria) {
              $columnHeader = $criteriaDBVal->name . ' From';
              $isFromDateCriteria = false;
            }
            else {
              $columnHeader = $criteriaDBVal->name . ' To';
              $isFromDateCriteria = true;
            }
          }
          else {
            $columnHeader = $criteriaDBVal->name;
            $isFromDateCriteria = true;
          }
          
          // Save the filter in $filterArray (for CSV) and $filterPdfArray (for pdf)
          if (isset($_GET['cron_key']) && !empty($inputValue)) { // scheduled reports can have multiple values in criteria fields
            $inputValue = implode(', ', explode('~||', $inputValue));
          }
          $filterArray[] = array($columnHeader . ': ' . $inputValue);
          if (!array_key_exists($columnHeader, $filterPdfArray)) {
            $filterPdfArray[$columnHeader] = $inputValue;
          }
        } //end foreach
      }
    }
    
    expDebug::dPrint('$andPrefixedConditions = ' . print_r($andPrefixedConditions, true), 4);
    expDebug::dPrint('$andSuffixedConditions = ' . print_r($andSuffixedConditions, true), 4);
    expDebug::dPrint('$queryArguments = ' . print_r($queryArguments, true), 4);
    expDebug::dPrint('$filterArray = ' . print_r($filterArray, true), 4);
    expDebug::dPrint('$filterPdfArray = ' . print_r($filterPdfArray, true), 4);
    expDebug::dPrint('$orderByColsList = ' . print_r($orderByColsList, true), 4);
    
    return array($andPrefixedConditions, $andSuffixedConditions, $queryArguments, $filterArray, $filterPdfArray, $orderByColsList);
  }
  catch (ReportSchedulesWorkerException $repSchWrkEx) {
    expDebug::dPrint('Rethrowing ReportSchedulesWorkerException', 4);
    throw($repSchWrkEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('buildReportQueryConditions', $ex);
    
    if (isset($_GET['cron_key'])) {
      expertusLogError($ex);
      expDebug::dPrint('Throwing new ReportSchedulesWorkerException', 4);
      throw new ReportSchedulesWorkerException($ex, 'INFORM-OWNER');
    }
    
    // will come here only when it is not a scheduled report
    expertusErrorThrow($ex);
  }
}

/*
 * Prepares the common part of the report SQL (common to user launched report, user csv export, user pdf export and scheduled report generation)
 */
function buildReportQueryCommonPart($reportId, $inputNameValues, $queryBuilderSQL, $reportEditable, $preview, $scheduleStartTimeObj) {
	try {
		// Generate SQL conditions clause from the filter criteria values
		list($andPrefixedConditions, $andSuffixedConditions, $queryArguments, $filterArray, $filterPdfArray, $orderByColsList) =
		buildReportQueryConditions($reportId, $inputNameValues, $queryBuilderSQL, $reportEditable, $scheduleStartTimeObj);

		$unionQuery = trim($queryBuilderSQL['union_query']);
		expDebug::dPrint('$unionQuery = ' . $unionQuery, 4);
		$wherePosition = $queryBuilderSQL['where_position']; // value meaningful only for the non-UNION queries.
		expDebug::dPrint('$wherePosition = ' . $wherePosition, 4);
		$queryFullSQL = trim($queryBuilderSQL['query_builder_sql']);
		expDebug::dPrint('$queryFullSQL = ' . $queryFullSQL, 4);
		$querySQLLen = strlen($queryFullSQL);
		expDebug::dPrint('$querySQLLen = ' . $querySQLLen, 4);
    
		
		
		// Problem happens with uneven spaces in group and order SQL
		$groupSQL = trim($queryBuilderSQL['group_sql']);
		$orderSQL = trim($queryBuilderSQL['order_sql']);
		$limitSQL = trim($queryBuilderSQL['limit_sql']);
		if($unionQuery){
			$queryFullSQL = "select tableunionalias.* from ( $queryFullSQL ) as tableunionalias ";
			if($reportEditable == 1 && $preview == 0){
				$querySQL = $queryFullSQL.' where 1=2 '.$andPrefixedConditions; // $andPrefixedConditions starts with an AND
			} else {
				$querySQL = $queryFullSQL.' where 1=1 '.$andPrefixedConditions; // $andPrefixedConditions starts with an AND
			}
		}
		else {
			if ($wherePosition == 0) {
				if(!empty($groupSQL)){
					$splitPosition = stripos($queryFullSQL, $groupSQL);
				} else if(!empty($orderSQL)){
					$splitPosition = stripos($queryFullSQL, $orderSQL);
				} else if(!empty($limitSQL)){
					$splitPosition = stripos($queryFullSQL, $limitSQL);
				} else {
					$splitPosition = strlen($queryFullSQL);
				}

				$querySQL = substr($queryFullSQL, 0, $splitPosition);

				$queryOtherSQL = substr($queryFullSQL, $splitPosition, $querySQLLen);
				if ($reportEditable == 1 && $preview == 0) { // Do not execute the query for admin unless preview is set to 1.
					$querySQL = $querySQL .
					' WHERE 1=2 ' .
					$andPrefixedConditions . ' ' . // $andPrefixedConditions starts with an AND
					$queryOtherSQL;
				}
				else {
					$querySQL = $querySQL .
					' WHERE 1=1 ' .
					$andPrefixedConditions.' ' . // $andPrefixedConditions starts with an AND
					$queryOtherSQL;
				}
			}
			else {
				$splitPosition = $wherePosition;
				$querySQL = substr($queryFullSQL, 0, $splitPosition - 1);
				$queryOtherSQL = substr($queryFullSQL, $splitPosition - 1, $querySQLLen);
				if ($reportEditable == 1 && $preview == 0) {
					$querySQL = $querySQL .
					' 1=2 and ' .
					$andSuffixedConditions . ' ' . //ends with an AND (so it has to be placed // immediately after WHERE in SQL query)
					$queryOtherSQL;
				}
				else {
					$querySQL = $querySQL . ' ' .
							$andSuffixedConditions . ' ' . //ends with an AND (so it has to be placed immediately after WHERE in SQL query)
							$queryOtherSQL;
				}
			}
		}

		// Remove Semicolon at end of the query
		$querySQL = trim($querySQL);
		$semiColonPosition = substr($querySQL, -1);
		if($semiColonPosition == ';'){
			$querySQL = substr($querySQL, 0, strlen($querySQL) - 1);
		}
 // 70573 -commented the line as results were empty if the search string contains double quotes
 //  $querySQL = str_replace('"', "'", $querySQL);

		expDebug::dPrint('$querySQL = ' . $querySQL, 4);
		return array($querySQL, $queryArguments, $filterArray, $filterPdfArray, $orderByColsList);
	}
	catch (ReportSchedulesWorkerException $repSchWrkEx) {
		expDebug::dPrint('Rethrowing ReportSchedulesWorkerException', 4);
		throw($repSchWrkEx); //rethrow
	}
	catch (Exception $ex) {
		watchdog_exception('buildReportQueryCommonPart', $ex);

		if (isset($_GET['cron_key'])) {
			expertusLogError($ex);
			expDebug::dPrint('Throwing new ReportSchedulesWorkerException', 4);
			throw new ReportSchedulesWorkerException($ex, 'INFORM-OWNER');
		}

		// will come here only when it is not a scheduled report
		expertusErrorThrow($ex);
	}
}

/**
 * Completes query for export to csv or pdf
 */
function completeQueryForExport($reportId, $listquery, $queryBuilderSQL, $sort, $sortOrder, $format) {
	try {
	$columnHeaders = getReportColumnHeader($reportId, true); // fetch column alias sql only
    expDebug::dPrint('$columnHeaders = ' . print_r($columnHeaders, true), 4);
    $columnHeadersArray = explode(',', $columnHeaders['column_alias_sql']);
    $columnHeadersArrayNew = array();
    if(arg(1) == 'report-result-content-export') {
      foreach($columnHeadersArray as $k=>$val){
    	if (strpos($val, ".") !== false) {
    		$val1 = explode(".",$val);
    		$columnHeadersArrayNew[] = $val1[1];
    	}
    	else {
    		$columnHeadersArrayNew[] = $val;
    	}
      }
    }
    else {
      $columnHeadersArrayNew = $columnHeadersArray;
    }
    if (isset($_GET['cron_key'])) {
    	$orderByColsList = $sort;
    	$orderByClause = ' order by ';
    	if (empty($orderByColsList['mandatory']) && empty($orderByColsList['optional'])) {
    		// Order by first column
    		$orderByClause .= '`' . $columnHeadersArrayNew[0] . '`';
    	}
    	else {
    		$first = true;
    		foreach ($orderByColsList['mandatory'] as $sortCol) {
    			if ($first) {
    				$orderByClause .= '`';
    				$first = false;
    			}
    			else {
    				$orderByClause .= ' , `';
    			}
    		  $orderByClause .= $sortCol;
    		  $orderByClause .= '`';
    		}
    	  foreach ($orderByColsList['optional'] as $sortCol) {
          if ($first) {
            $orderByClause .= '`';
            $first = false;
          }
          else {
            $orderByClause .= ' , `';
          }
          $orderByClause .= $sortCol;
          $orderByClause .= '`';
        }
    	}
      $orderSQL = trim($queryBuilderSQL['order_sql']);
      if (!empty($orderSQL)) {
        $strPositon = strripos($listquery, 'order by');
        $listquery  = substr($listquery, 0, $strPositon);
        $listquery  = $listquery . $orderSQL;
      }
      else {
        $listquery = $listquery . $orderByClause;
      }
    }
    else if (!empty($sort) && $sort != 'undefined') {
      $orderSQL = trim($queryBuilderSQL['order_sql']);
      if (!empty($orderSQL)) {
        $strPositon = strripos($listquery, 'order by');
        $listquery  = substr($listquery, 0, $strPositon);
        $listquery  = $listquery . ' order by `' . $sort . '` ' . $sortOrder;
      }
      else {
      	if(!is_array($sort))
        $listquery = $listquery . ' order by `' . $sort . '` ' . $sortOrder;
      }
    }
    expDebug::dPrint('$listquery = ' . $listquery, 4);

    // Report title
    $reportHeader = getReportHeader($reportId);
    expDebug::dPrint('$reportHeader = ' . print_r($reportHeader, true), 4);
    
    // Report column headings
    $columnHeadersLowerArray = explode(',', strtolower($columnHeaders['column_alias_sql']));  
    $columnHeadersLowerArray = array_map('trim', $columnHeadersLowerArray);
    $columnHeadersLowerArrayNew = array();
    if(arg(1) == 'report-result-content-export') {
      foreach($columnHeadersLowerArray as $k=>$val){
    	if (strpos($val, ".") !== false) {
    		$val1 = explode(".",$val);
    		$columnHeadersLowerArrayNew[] = $val1[1];
    	}
    	else {
    		$columnHeadersLowerArrayNew[] = $val;
    	}
      }
    }
    else {
    	$columnHeadersLowerArrayNew = $columnHeadersLowerArray;
    }
    if(strtoupper($format) == "CSV" || strtoupper($format) == "XLS") {
      $colHeadersMap = array_combine($columnHeadersArrayNew, $columnHeadersLowerArrayNew);        
    }
    else { // PDF 
    	$column_cont_msg = false;       
      foreach ($columnHeadersLowerArrayNew as $count => $cHeader) {
        if ($count > 13) {
        	$column_cont_msg = true;
          break;
        }
       $colHeadersMap[$columnHeadersArrayNew[$count]] = array($cHeader, 8);
      }
    }
    expDebug::dPrint('$colHeadersMap = ' . print_r($colHeadersMap, true), 4);
    
    return array($listquery, $reportHeader, $colHeadersMap, $column_cont_msg);
	}
  catch (ReportSchedulesWorkerException $repSchWrkEx) {
    expDebug::dPrint('Rethrowing ReportSchedulesWorkerException', 4);
    throw($repSchWrkEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('completeQueryForExport', $ex);
    
    if (isset($_GET['cron_key'])) {
      expertusLogError($ex);
      expDebug::dPrint('Throwing new ReportSchedulesWorkerException', 4);
      throw new ReportSchedulesWorkerException($ex, 'INFORM-OWNER');
    }
    
    expertusErrorThrow($ex);
  }
}

/**
 * Callback for report data fetch from UI for jqgrid, export to csv or pdf
 */
function loadLearnerReportResult($reportId, $reportEditable, $preview, $sort, $sortOrder, $export='', $scheduleStartTimeObj = null, $callFrom = '') {
	try {
		if(user_is_logged_in() === false) {
			drupal_access_denied();
			exit();
		}
		expDebug::dPrint('$reportId = ' . print_r($reportId, true), 5);
		expDebug::dPrint('$inputNameValues = ' . print_r($inputNameValues, true), 5);
		expDebug::dPrint('$reportEditable = ' . print_r($reportEditable, true), 5);
		expDebug::dPrint('$preview = ' . print_r($preview, true), 5);
		expDebug::dPrint('$sort = ' . print_r($sort, true), 5);
		expDebug::dPrint('$sortOrder = ' . print_r($sortOrder, true), 5);
		expDebug::dPrint('$export = ' . print_r($export, true), 5);
		expDebug::dPrint('$scheduleStartTimeObj = ' . print_r($scheduleStartTimeObj, true), 5);
		include_once  $_SERVER["DOCUMENT_ROOT"]."/sites/all/dao/AbstractDAO.php";		
		$inputNameValues = getURLParam('inputNameValues');	//43200: Error in exporting report
		// Initialize
		$expErrorMsg	 = '';
		$isTerminated = false; // flag to identify if the query is terminated because of taking longer time to execute.

		
	
		
		
		
		// Get SQL query details saved by the query builder
		$queryBuilderSQL = getQueryFromBuilder($reportId);
		expDebug::dPrint('$queryBuilderSQL = ' . print_r($queryBuilderSQL, true), 4);
		
		$userId = getIdOfLoggedInUser();
		require_once('sites/all/services/GlobalUtil.php');
		$gutil = new GlobalUtil();
		$expConfig = $gutil->getConfig();
		$slt_table_exten = trim($expConfig['slt_table_exten']);
		expDebug::dPrint("config variable-->".$slt_table_exten);
		if($slt_table_exten == 0){			
			list($table, $table1) = explode(',', $queryBuilderSQL['table_sql']);
			if(empty($table)){$table = $table1;}else{$table = $table;}		
			expDebug::dPrint("testete".$table);
			$tablelist = getalltables();			
			$tableExnt = 0;
			if(in_array(trim($table),$tablelist)){			
				$tableExnt = 1;		
			}		
			expDebug::dPrint("table extention-->".$tableExnt);	
			$table_name_exten = substr($table, 0, 4);
			if((trim($table_name_exten) == 'slt') || ($tableExnt == 1)){
				$expErrorMsg = t("ERR253");
				//$expErrorMsg .= $table.' does not exist.';
				$queryBuilderSQL = array();
				$isTerminated = true;
			}
		}
		// Build the common part which is common to user launched report, user csv export, user pdf export and scheduled report generation
		list($listquery, $queryArguments, $filterArray, $filterPdfArray, $orderByColsList) = // $orderByColsList is ignored as implemented only for scheduled reports
		buildReportQueryCommonPart($reportId, $inputNameValues, $queryBuilderSQL, $reportEditable, $preview, $scheduleStartTimeObj);
		//Added for #0070534
		$config = getConfig('exp_sp');
	  $maxExecutionTime = ($config['query_max_execution_time'] * 1000);
		if ($export == "CSV") {
			list($listquery, $reportHeader, $colHeadersMap,$column_cont_msg) = completeQueryForExport($reportId, $listquery, $queryBuilderSQL, $sort, $sortOrder, $export);
			try{
				$dao = new AbstractDAO('Report');
				$dao->connect();
				$dao->execute('SET SESSION MAX_EXECUTION_TIME='.$maxExecutionTime);
				$dao->execute('SET @report_uid = '.$userId);			
				$listqueryResult = $dao->query($listquery);
	
				//$listqueryResult = db_query("$listquery", $queryArguments, getReportDatabaseOption());
				expDebug::dPrintDBAPI('List Query CSV/PDF', $listqueryResult, $queryArguments);
				SendNarrowSearchResultsAsCSVFile($listqueryResult, $colHeadersMap, 'export', $filterArray, $reportHeader,'','y');
				$dao->closeconnect();
			}catch(PDOException $pex){ //Added for #0070534
				$expErrorMsg = $exception->getMessage();
				if (strpos($expErrorMsg, 'maximum statement execution time exceeded') > 0 ) {
					$expErrorMsg = t('ERR256');
				}
				expDebug::dPrint('$expErrorMsg = ' . print_r($expErrorMsg, true) , 4);
					
				$expDebugMsg  = 'Expertus ERROR : ';
				$expDebugMsg .= $exception->getMessage() . "\r\n\r\n";
				$expDebugMsg .= "Trace info :\r\n";
				$expDebugMsg .= $exception->getTraceAsString() . "\r\n\r\n";
					
				$traceInfo = array();
				$traceInfoList = debug_backtrace(false);
				if (sizeof($traceInfoList) > 1) {
					$traceInfo = $traceInfoList[1];
				}
				expDebug::dPrint($expDebugMsg, 1, $traceInfo);
				$queryResult = array();
				$isTerminated = true;
			}
		}
		else if ($export == "PDF") {
			list($listquery, $reportHeader, $colHeadersMap,$column_cont_msg) = completeQueryForExport($reportId, $listquery, $queryBuilderSQL, $sort, $sortOrder, $export);

			//$listqueryResult = db_query("$listquery", $queryArguments, getReportDatabaseOption());
			try{
				$dao = new AbstractDAO('Report');
				$dao->connect();
				$dao->execute('SET SESSION MAX_EXECUTION_TIME='.$maxExecutionTime);
				$dao->execute('SET @report_uid = '.$userId);			
				$listqueryResult = $dao->query($listquery);
				//$listqueryResult=$dao->fetchAllResults();
						
				expDebug::dPrintDBAPI('List Query CSV/PDF', $listqueryResult, $queryArguments);
				SendNarrowSearchResultsAsPDFFile($listqueryResult, $reportHeader, $filterPdfArray, $colHeadersMap, 'export', 6,'p','',$column_cont_msg,'y');
				$dao->closeconnect();
			}catch(PDOException $pex){ //Added for #0070534
				$expErrorMsg = $exception->getMessage();
				if (strpos($expErrorMsg, 'maximum statement execution time exceeded') > 0 ) {
					$expErrorMsg = t('ERR256');
				}
				expDebug::dPrint('$expErrorMsg = ' . print_r($expErrorMsg, true) , 4);
					
				$expDebugMsg  = 'Expertus ERROR : ';
				$expDebugMsg .= $exception->getMessage() . "\r\n\r\n";
				$expDebugMsg .= "Trace info :\r\n";
				$expDebugMsg .= $exception->getTraceAsString() . "\r\n\r\n";
					
				$traceInfo = array();
				$traceInfoList = debug_backtrace(false);
				if (sizeof($traceInfoList) > 1) {
					$traceInfo = $traceInfoList[1];
				}
				expDebug::dPrint($expDebugMsg, 1, $traceInfo);
				$queryResult = array();
				$isTerminated = true;
			}
		}else if($export == "PDFcount" || $export == "CSVcount"){
			$dao = new AbstractDAO('Report');
			$dao->connect();
			$dao->execute('SET SESSION MAX_EXECUTION_TIME='.$maxExecutionTime);
			$dao->execute('SET @report_uid = '.$userId);
			$listqueryResult = $dao->query($listquery);
			$pdfCount = $listqueryResult->rowCount();
			$dao->closeconnect();
			expDebug::dPrint("pdf count>>> ".$pdfCount);
			$config_max_records = ($export == "PDFcount") ? $expConfig['pdf_max_records'] : $expConfig['csv_max_records'];
			expDebug::dPrint('$$config_max_records = >>' . $config_max_records, 4);
			if($pdfCount > $config_max_records)
				echo "exceed_max_limit";
			else
				echo "limit";
		}
		else {
			if($callFrom == 'Internal'){//Added for #0070534
				$page          = 1;
				$limit         = 100;
			}else{
				// Vincent Added on 11 Aug, 2015 for # 0052759 item 4
				$page          = orgValidate(reportXSSValidate($_GET['page']));
				$limit         = orgValidate(reportXSSValidate($_GET['rows']));
				$sidx          = reportXSSValidate($_GET['sidx']);
				$sord          = reportXSSValidate($_GET['sord']);
			}
			expDebug::dPrint('$page = ' . $page, 4);
			expDebug::dPrint('$limit = ' . $limit, 4);
			expDebug::dPrint('$sidx = ' . $sidx, 4);
			expDebug::dPrint('$sord = ' . $sord, 4);

			$limitSQL = trim($queryBuilderSQL['limit_sql']);
			$limitVar = '';
			//change by ayyappans for 31039 and 39860 tickets
			$start = $limit * $page - $limit;
			// report performance changes
			$limit += 1;
			
			if (empty($limitSQL)) {
				/* Paging Related */
				$limitVar = " limit $start, $limit";
			}
			else {
				$limitValue = substr($limitSQL, strpos($limitSQL, " ") + 1);
				$listquery = preg_replace("/$limitSQL/i", '', $listquery);
				$end = min(($limitValue - $start), $limit);
				$limitVar = " limit $start, $end";
			}
			expDebug::dPrint('report select $limitVar = '.$limitVar, 4);
			$orderSQL = trim($queryBuilderSQL['order_sql']);

			$sortVar = '';
			if(!empty($orderSQL)){
			$strPositon = strripos($listquery,"order by");
			if( (!empty($strPositon) && empty($sidx)) || (empty($strPositon) && empty($sidx)) ) {
				$sortVar = '';
			} elseif(!empty($strPositon) && !empty($sidx)) {
				$sortVar = ',`'.$sidx.'` '.$sord;	
				// $strPositon = strripos($listquery,"order by");
				$strPositon = strripos($listquery,' ORDER BY `'.$sidx.'` '); // #0047034 added For this Ticket.
				if(!empty($strPositon)){
					$listquery  = substr($listquery,0,$strPositon);
				}
			}elseif(empty($strPositon) && !empty($sidx)) {
				$sortVar = ' ORDER BY `'.$sidx.'` '.$sord;
			}
		}else if(!empty($sidx)){
			$builderInfo = getWizardReportQueryBuilderDetails($reportId);
			$hasBuilderInfo = empty($builderInfo)? false : true;
			if ($hasBuilderInfo) {
				$columnSql = explode('~~', $builderInfo['column_sql']);
				$columnAliasSql = explode(',', $builderInfo['column_alias_sql']);
			}
			else{
				$columnSql = explode(",", $_REQUEST['column_sql']);
				$columnAliasSql = explode(",", $_REQUEST['column_alias_sql']);
			}
			$ColumnCount =array_count_values($columnAliasSql);
			if($ColumnCount[$sidx] > 1){
				// sort by colum name instead of alias name
				$OrdFieldArray = array_combine($columnSql,$columnAliasSql);
				$OrdField = array_search($sidx,$OrdFieldArray);
			$sortVar = ' ORDER BY '.$OrdField.' '.$sord;
			}else{
				$sortVar = ' ORDER BY `'.$sidx.'` '.$sord;
			}
		}
			expDebug::dPrint('Sending query to grid $sortVar : ' . $sortVar, 4);
			expDebug::dPrint('Sending query to grid $$limitVar : ' . $limitVar, 4);
			expDebug::dPrint('Sending query to grid $listquery : ' . $listquery, 4);
			expDebug::dPrint('Sending query to grid : '. $listquery . ' ' . $sortVar . ' ' . $limitVar, 4);

			$count = 0;
	  	$maxExecutionTime = ($config['query_launch_max_time'] * 1000); //Added for #0070534
			try {
				expDebug::dPrint("REPORT QUERY FOR FETCH DATA ::: " . "$listquery $sortVar $limitVar", 2);
// 				$listquery = preg_replace('/SELECT/i', 'SELECT 0C_FOUND_ROWS ', $listquery, 1);

				
				 
				$daoMaster = new AbstractDAO();
				$daoMaster->connect();
				
				$rqry = 'INSERT INTO slt_web_report_running_query (report_id, user_id, session_id, pid, started_on) 
									VALUES ( '.$reportId.' , '.$userId.' , "'.session_id().'" , CONNECTION_ID() , now())';
				//$arg = array(':repId'=>$reportId, ':userId'=>$userId, ':session'=>session_id());
				expDebug::dPrintDBAPI("TO CAPTURE LONG RUNNING QUERY  -- ",$rqry);
				$pid = $daoMaster->query($rqry);
				$daoMaster->closeconnect();
				
				$dao = new AbstractDAO('Report');
				$dao->connect();
				$dao->execute('SET SESSION MAX_EXECUTION_TIME='.$maxExecutionTime);	
				$dao->execute('SET @report_uid = '.$userId);
				$reportBuilderType = getReportBuilderType($reportId);
				
				//$listquery = 'select *from slt_groups;';
				//$vid = db_query("select *from slt_groups;")->fetchAll();
				//$select = db_select('slt_groups', 'grp');
				//$select->addField('grp','site_id');
				//$select->addField('grp','id');
				//$select->addField('grp','name');
				//$vid= $select->execute()->fetchAll(); 
				//expDebug::dPrint("result debug ".print_r($vid,true),5);
				$groupby = "";
				// Commented By Shobana N. Because of this condition design wizard throws error.
				/* if($reportBuilderType == 'design_wizard'){	
					if($queryBuilderSQL['table_sql'] != 'slv_enrollments,slv_master_enrollments'){	
						$qry1 = $dao->query('SHOW COLUMNS FROM '.$queryBuilderSQL['table_sql']);
						$resul1 = $dao->fetchAllResults();
						$coulmn = $resul1[0]->Field;
					    $groupby = ' GROUP BY '.$queryBuilderSQL["table_sql"].'.'.$coulmn;				
					}	   		
				} */
				expDebug::dPrintDBAPI('query builder ',$listquery.$groupby.$sortVar.$limitVar);
				$listqueryResult = $dao->query($listquery.$groupby.$sortVar.$limitVar,TRUE);
				
				$queryResult=$dao->fetchAllResults();
				//$listqueryResult = db_query("$listquery $sortVar $limitVar", $queryArguments, getReportDatabaseOption());
				//expDebug::dPrintDBAPI('List Query with limit', $listqueryResult, $queryArguments);
				 
				// Execute query
				//$queryResult = $listqueryResult->fetchAll();
				//expDebug::dPrint('$queryResult = ' . print_r($queryResult, true));
				expDebug::dPrint('$queryResult = ' . print_r($queryResult, true));
				//Found total records
				//$totRecord  = db_query('select FOUND_ROWS() as total_record', array(), getReportDatabaseOption());
			//	expDebug::dPrintDBAPI('$totRecord', $totRecord, array());
				/* $qry = $dao->query('select FOUND_ROWS() as total_record');
				$resul = $dao->fetchResult();
				if(isset($limitValue)) {
					$count = min($resul->total_record, $limitValue);
				}
				else {
					$count = $resul->total_record;
				} */
				
				$count = count($queryResult);
// 				$count = 402;
				expDebug::dPrint('Total records = ' . $count);
				$dao->closeconnect();	
				 
			}
			catch (Exception $exception) {
				$expErrorMsg = $exception->getMessage();
				//Added for #0070534
				if (strpos($expErrorMsg, 'maximum statement execution time exceeded') > 0 ) {
					$expErrorMsg = $reportEditable == 1 ? t('ERR255') : t('ERR256');
					expDebug::dPrint("Error in Query execution ".$expErrorMsg. " ---- ".$callFrom);
					if($callFrom == 'Internal')
						return FALSE;
				}else if (strpos($expErrorMsg, 'MySQL server has gone away') > 0 ) {
					$expErrorMsg = t('ERR170');
				}else if (!empty($exception->errorInfo[2])) {
					// Vincent modified on 11 Aug, 2015 for # 0052759 item 4
					$expErrorMsg = t('ERR167'); //$exception->errorInfo[2]; //SQL syntax error
				}else {
					$expErrorMsg = t('ERR154'); //System error, Contact Support Team
				}
				expDebug::dPrint('$expErrorMsg = ' . print_r($expErrorMsg, true) , 4);
					
				$expDebugMsg  = 'Expertus ERROR : ';
				$expDebugMsg .= $exception->getMessage() . "\r\n\r\n";
				$expDebugMsg .= "Trace info :\r\n";
				$expDebugMsg .= $exception->getTraceAsString() . "\r\n\r\n";
					
				$traceInfo = array();
				$traceInfoList = debug_backtrace(false);
				if (sizeof($traceInfoList) > 1) {
					$traceInfo = $traceInfoList[1];
				}
				expDebug::dPrint($expDebugMsg, 1, $traceInfo);
				$queryResult = array();
				$isTerminated = true;
			}
			if($count != 0) {
				$count = $start + $count;
			}
			
			if ($count > 0) {
				$total_pages = ceil($count/($limit - 1));
			}else{
				$total_pages = 0;
			}
			if($page > $total_pages)
				$page = $total_pages;
			if($start < 0) {
				$start = 0;
			}
			// page calculation
			$page = ($start / ($limit - 1)) + 1;
			expDebug::dPrint('$count = '.$count, 4);
			expDebug::dPrint('$start = '.$start, 4);
			expDebug::dPrint('$limit = '.$limit, 4);
			expDebug::dPrint('$page number here = '.$total_pages, 4);
			$srch_result_response_report->total   = $total_pages;
			$srch_result_response_report->records = $count;
			$srch_result_response_report->page    = $page;

			$_SESSION['jq_count'] = array();

			$columnAliasHeader = $queryBuilderSQL['column_alias_sql'];
			$columnAliasHeaderArray = explode(',', $columnAliasHeader);

			expDebug::dPrint('Query Result : '.print_r($queryResult,true),3);
			expDebug::dPrint('Column alias header: '.print_r($columnAliasHeaderArray, true) , 4);

			$i=0;
			foreach ($queryResult as $rec_search_results) {
				$srch_result_response_report->rows[$i]['id']   = $i;
				keysToLower($rec_search_results);
				
				$c = 0;
				foreach($columnAliasHeaderArray as $columnName){
					$headerAliasTmp = trim(strtolower($columnName));
					
					$columnNameGridIndex = str_replace(' ', '_', $columnName);
					$aliasname = explode('.', $headerAliasTmp);
					if (!empty($aliasname[1])) { // Issue id Fixed for this ticket #0031604
						$headerAliasTmp = $aliasname[1];
					}
					
					$srch_result_response_report->rows[$i]['cell'][$columnName] = strip_tags($rec_search_results->$headerAliasTmp);
					$c++;
				}
				$i++;
			}

			$srch_result_response_report->message = '';
			if($isTerminated == true) {
				$srch_result_response_report->message    = $expErrorMsg;
			}

			expDebug::dPrint('$srch_result_response_report = ' . print_r($srch_result_response_report, true), 5);
			//$dao->closeconnect();
			// Added for 0070534
			destroyQuery($reportId);
			if($callFrom == 'Internal')
				return TRUE;
			else
				echo drupal_json_encode($srch_result_response_report);
		}

	}
	catch (Exception $ex) {
		watchdog_exception('loadLearnerReportResult', $ex);
		expertusErrorThrow($ex);
	}
}

function destroyQuery($reportId){
	try{
		$reportId = empty($reportId)? $_REQUEST['reportId'] : $reportId;
		$config = getConfig('exp_sp');
		$maxExecutionTime = ($config['query_launch_max_time']);
		$userId = getIdOfLoggedInUser();
		$qry = 'SELECT pid FROM slt_web_report_running_query 
						WHERE report_id = :repId
						AND user_id = :userid
						AND session_id = :session
						AND started_on < DATE_ADD(started_on, INTERVAL :maxTime SECOND) ';
		expDebug::dPrintDBAPI("Delete long running query -- ",$qry,array(':repId'=>$reportId, ':userid'=>$userId, ':session'=>session_id(), ':maxTime'=>$maxExecutionTime));
		$pid = db_query($qry, array(':repId'=>$reportId, ':userid'=>$userId, ':session'=>session_id(), ':maxTime'=>$maxExecutionTime))->fetchAll();
		expDebug::dPrint("Selected pids - ".print_r($pid,true),3);
		foreach($pid as $p=>$id){
			try{
				db_query('KILL :pid ',array(':pid'=>$id->pid));
			}catch(Exception $ex){
				// noting to do
			}
			db_query('DELETE FROM slt_web_report_running_query WHERE pid = :pid ', array(':pid'=>$id->pid));
		}
		
		return true;
	}catch(Exception $e){
		expertusErrorThrow($e);
	}
}

function keysToLower(&$obj)
{
	if(is_object($obj))
	{
		$newobj = (object) array();
		foreach ($obj as $key => &$val)
			$newobj->{trim(strtolower($key))} = keysToLower($val);
		$obj=$newobj;
	}
	else if(is_array($obj))
		foreach($obj as &$value)
		keysToLower($value);
	return $obj;
}
/**
 * Preprocesses scheduled report's date value
 */
function preProcessScheduledReportDateValue($scheduleStartTimeObj, &$dateValue) {
	try {

		$dateBeforeCode = 'DB';
		$dateAfterCode = 'DA';
	if (empty($dateValue) || $dateValue == $dateBeforeCode || $dateValue == $dateAfterCode) {
		$dateValue='';
    	return;
    }    
    $scheduleStartClone = clone $scheduleStartTimeObj;
    if (substr_compare($dateValue, $dateBeforeCode, -strlen($dateBeforeCode), strlen($dateBeforeCode)) === 0) {
    	$numDays = substr($dateValue, 0, -strlen($dateBeforeCode));
    	$scheduleStartClone->sub(new DateInterval('P' . $numDays . 'D'));
      $dateValue = $scheduleStartClone->format('Y-m-d'); // Update $dateValue with the real date
    }
    else if (substr_compare($dateValue, $dateAfterCode, -strlen($dateAfterCode), strlen($dateAfterCode)) === 0) {
    	$numDays = substr($dateValue, 0, -strlen($dateAfterCode));
      $scheduleStartClone->add(new DateInterval('P' . $numDays . 'D'));
      $dateValue = $scheduleStartClone->format('Y-m-d'); // Update $dateValue with the real date
    }
	}
	catch (Exception $ex) {
    watchdog_exception('reportGetCriteriaList', $ex);
    
    if (isset($_GET['cron_key'])) {
      expertusLogError($ex);
      expDebug::dPrint('Throwing new ReportSchedulesWorkerException', 4);
      throw new ReportSchedulesWorkerException($ex, 'INFORM-OWNER');
    }
    
    // will come here only when it is not a scheduled report
    expertusErrorThrow($ex);		
	}
}

/**
 * Modifies the criteria object, criteria value and sql query to what finally needs to be added to the sql query.
 */
function reportPreProcessCriteria(&$criteriaObj, &$inputValue, $unionQuery, $reportEditable, $scheduleStartTimeObj) {
	try {
		expDebug::dPrint('$criteriaObj = ' . print_r($criteriaObj, true), 4);
		expDebug::dPrint('$inputValue = ' . $inputValue, 4);
		expDebug::dPrint('$unionQuery = ' . print_r($unionQuery, true), 4);
		expDebug::dPrint('$reportEditable = ' . $reportEditable, 4);
		expDebug::dPrint('$$scheduleStartTimeObj = ' . print_r($scheduleStartTimeObj,true), 4);
		//$scheduleStartTimeObj
		
		if (isset($_GET['cron_key']) && $criteriaObj->column_type == 'datetime' && $criteriaObj->column_type = 'date') {
			preProcessScheduledReportDateValue($scheduleStartTimeObj, $inputValue);
		}

	  // For union column alias has been changed to work will all union select queries
    if ($unionQuery) {
    	$criteriaObj->orig_column_name = $criteriaObj->column_name;
      $criteriaObj->column_name = 'tableunionalias.`' . $criteriaObj->column_alias_name . '`';
    }

    //If from admin report edit screen or if criteria does not have use_user_details set, no modification to be done
    if (isset($_GET['cron_key']) || $reportEditable || empty($criteriaObj->use_user_details)) {
    	expDebug::dPrint('no modification done', 4);
    	return;
    }

    // Criteria has use_user_details set
    
    $inputValue = reportGetCriteriaValueFromUserDetail($criteriaObj, $unionQuery);
    return;
	}
  catch (ReportSchedulesWorkerException $repSchWrkEx) {
    expDebug::dPrint('Rethrowing ReportSchedulesWorkerException', 4);
    throw($repSchWrkEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('reportPreProcessCriteria', $ex);
    
    if (isset($_GET['cron_key'])) {
      expertusLogError($ex);
      expDebug::dPrint('Throwing new ReportSchedulesWorkerException', 4);
      throw new ReportSchedulesWorkerException($ex, 'INFORM-OWNER');
    }
    
    // will come here only when it is not a scheduled report
    expertusErrorThrow($ex);    
  }
}

/*
 * Fetches information of the logged in user's details from SPLearnerInfo cookie for reports "use logged in user detail" feature.
 */
function getLoggedInUserDetailsForReport(){
  try{  
    $userDetails = array('userid' => '',
                         'username' => '',
                         'usermail' => '',
                         'userfirstname' => '',
                         'userlastname' => '',
                         'usersavedfullname' => '');
  
    require_once "./getLearnerInfo.php";
    $obj = new GetLearnerInfo();
    foreach ($userDetails as $detail => $value) {
      $userDetails[$detail] = $obj->getValue($detail);
    }
    expDebug::dPrint('$userDetails = ' . print_r($userDetails, true), 4);
    return $userDetails;
  }
  catch (Exception $ex) {
    watchdog_exception('getLoggedInUserDetailsForReport', $ex);
    
    if (isset($_GET['cron_key'])) {
      expertusLogError($ex);
      expDebug::dPrint('Throwing new ReportSchedulesWorkerException', 4);
      throw new ReportSchedulesWorkerException($ex, 'INFORM-OWNER');
    }
    
    // will come here only when it is not a scheduled report
    expertusErrorThrow($ex);    
  }
}

/**
 * Fetches user criteria value from user detail
 */
function reportGetCriteriaValueFromUserDetail($criteriaObj, $unionQuery) {
	try {
    expDebug::dPrint('$criteriaObj = ' . print_r($criteriaObj, true), 4);
    
    // Fetch logged in user details
    $userDetails = getLoggedInUserDetailsForReport();
    expDebug::dPrint('$userDetails = ' . print_r($userDetails, true), 4);
    
    // Based on field name decide which user details to return as criteria value
    $columnName = $unionQuery? $criteriaObj->orig_column_name : $criteriaObj->column_name;
    $splitColumnName = reportSplitColumnName($columnName);
    expDebug::dPrint('$splitColumnName '.print_r($splitColumnName, 1));
//     ["full_name", "user_name","last_name","first_name","survey_username","instructor_fullname","manager_user_name","enrolled_username","manager_fullname","managername", "managerfullname", "fullname", "username", "lastname", "firstname", "managerusername"];
    switch (strtolower($splitColumnName['field_name'])) {
    	case 'user_name':
    	case 'manager_user_name': 
    	case 'enrolled_username':
    	case 'manager_fullname':	
    	case 'managername':	
    	case 'survey_username':
    	case 'username':
    	case 'managerusername':
    	case 'userdottedmanagerusername':	
    		return $userDetails['username'];
    		
    	case 'first_name':
    	case 'firstname':
    	    return $userDetails['userfirstname'];
    	    
    	case 'last_name':
    	case 'lastname':
    	    return $userDetails['userlastname'];
    	    
    	case 'full_name':
    	case 'managerfullname':
    	case 'instructor_fullname':
    	case 'fullname':
    	case 'userdottedmanagername':
    		return $userDetails['usersavedfullname'];
    		
    	default: throw new Exception('User detail fetched for an unsupported field.');
    }
	}
  catch (ReportSchedulesWorkerException $repSchWrkEx) {
    expDebug::dPrint('Rethrowing ReportSchedulesWorkerException', 4);
    throw($repSchWrkEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('reportGetCriteriaValueFromUserDetail', $ex);
    
    if (isset($_GET['cron_key'])) {
      expertusLogError($ex);
      expDebug::dPrint('Throwing new ReportSchedulesWorkerException', 4);
      throw new ReportSchedulesWorkerException($ex, 'INFORM-OWNER');
    }
    
    // will come here only when it is not a scheduled report
    expertusErrorThrow($ex);    
  }
}

/**
 * Splits a column name into table alias and field name
 */
function reportSplitColumnName($colName) {
  try {
    $retArr = array('table_alias' => '',
                    'field_name' => '');
  	$colNameArray = explode('.', $colName);
  	$colNameArraySize = count($colNameArray);
	  if ($colNameArraySize > 2) {
	  	throw Exception('Dot in table alias or field name. This scenario now needs to be handled in code.');
	  }
	  
	  if ($colNameArraySize == 2) {
	  	$retArr['table_alias'] = $colNameArray[0];
	  	$retArr['field_name'] = $colNameArray[1];
	  }
	  else {
	  	$retArr['field_name'] = $colName;
	  }
	  
	  expDebug::dPrint('$retArr = ' . print_r($retArr, true), 5);
	  return $retArr;
  }
  catch (Exception $ex) {
    watchdog_exception('reportSplitColumnName', $ex);
    
    if (isset($_GET['cron_key'])) {
      expertusLogError($ex);
      expDebug::dPrint('Throwing new ReportSchedulesWorkerException', 4);
      throw new ReportSchedulesWorkerException($ex, 'INFORM-OWNER');
    }
    
    // will come here only when it is not a scheduled report
    expertusErrorThrow($ex);    
  }
}

/**
 * Terminate long running queries on the reports database server
 */
function terminateLongRunningQueriesOnReportsDBServer() {
	try {
		$config = getConfig('exp_sp');
	  $maxExecutionTime = $config['query_max_execution_time'];
		$processlistQry = db_query('SHOW PROCESSLIST', array(), getReportDatabaseOption());
		$processlist = $processlistQry->fetchAll();
		foreach ($processlist as $proc) {
			expDebug::dPrint('Reports database process obj = ' . print_r($proc, true), 4);
			if(strtolower($proc->command) == 'query' && $proc->time >= $maxExecutionTime &&
			       (strtolower($proc->state) == 'copying to tmp table' || strtolower($proc->state) == 'sorting result' ||
			           strtolower($proc->state) == 'sending data') && stripos(strtolower($proc->info), 'select') >= 0) {
				$killQuery = 'kill ' . $proc->id;
				expDebug::dPrint('Query for killing db process = ' . $killQuery, 2);
				$killResult = db_query($killQuery, array(), getReportDatabaseOption());
			}
		}
	}
	catch (Exception $ex) {
    watchdog_exception('terminateLongRunningQueriesOnReportsDBServer', $ex);    
    if (isset($_GET['cron_key'])) {
      expertusLogError($ex);
      throw $ex; // Rethrow
    }
    
    // will come here only when not a cron job
    expertusErrorThrow($ex);    
	}
}

/**
 * Fetches criteria details
 */
function getReportCriteriaDetails($criteriaId) {
	try{
		$selStmt = db_select('slt_report_criteria', 'cri');
		$selStmt->addField('cri', 'report_details_id');
		$selStmt->addField('cri', 'table_name');
		$selStmt->addField('cri', 'column_name');
		$selStmt->addField('cri', 'column_alias_name');
		$selStmt->addField('cri', 'criteria_query');
		$selStmt->addField('cri', 'criteria_union_query');
		$selStmt->addField('cri', 'condition_type');
		$selStmt->condition('cri.id', $criteriaId);
		expDebug::dPrintDBAPI('$selStmt', $selStmt);
		$criteriaDetails = $selStmt->execute()->fetchAssoc();
		expDebug::dPrint('$criteriaDetails = '. print_r($criteriaDetails, true));

		return $criteriaDetails;
	}
	catch (Exception $ex) {
		watchdog_exception('getReportCriteriaDetails', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * Fetches the profile code prefix used in a table column joined with slt_profile_list_items
 */
function getProfileCodePrefixInCol($joinTable, $joinTableCol) {
	try {
		$query = db_select($joinTable, 'tname');
		$query->addField('tname', $joinTableCol , 'profile_code');
		$query->isNotNull('tname.' . $joinTableCol);
		$query->condition('tname.' . $joinTableCol, '', '!=');
		$query->groupBy('tname.' . $joinTableCol);
		$query->range(0, 1);
		expDebug::dPrintDBAPI('$query', $query);
		$profileCode = $query->execute()->fetchField();
		expDebug::dPrint('$profileCode = ' . print_r($profileCode, true), 4);
		
		if (empty($profileCode)) {
			expDebug::dPrint('No profile code found in db from which prefix can be extracted', 4);
			//return false;
		}
		
		$strPositon = strripos($profileCode, '_');
		if($strPositon){
			$profileCodePrefix = substr($profileCode, 0, $strPositon + 1);
		}else{
			$profileCodePrefix = '';
		}
		expDebug::dPrint('$profileCodePrefix = ' . $profileCodePrefix, 4);
		
		return $profileCodePrefix;
	}
	catch (Exception $ex) {
		watchdog_exception('getProfileCodePrefixInCol', $ex);
		expertusLogError($ex);
		return false;
	}
}

/**
 * For criteria autocomplete fetches spli code prefix from the table to which slt_profile_list_items table is joined
 */
function getCriteriaSpliCodePrefixFromJoinTable($spliTableAliasName, $tablesList, $tableAliasesList, $tableJoinList) {
	try {
		$spliAliasIdx = array_search($spliTableAliasName, $tableAliasesList);
		if ($spliAliasIdx === false || empty($tableJoinList[$spliAliasIdx])) {
			expDebug::dPrint('Could not find join table info for spli table alias', 4);
			return false;
		}
		
	  $joinColArr  = explode('.', $tableJoinList[$spliAliasIdx]);
	  expDebug::dPrint('$joinColArr = ' . print_r($joinColArr, true), 4);
		$joinTableAlias = $joinColArr[0];
		$joinTableCol = $joinColArr[1];
		if (empty($joinTableAlias) || empty($joinTableCol)) {
			expDebug::dPrint('Either joined table alias or joined table column or both are unavialable', 4);
			return false;
		}
		
		$joinTableIdx = array_search($joinTableAlias, $tableAliasesList);
		if ($joinTableIdx === false || empty($tablesList[$joinTableIdx])) {
			expDebug::dPrint('Could not match joined table alias to its db table name', 4);
			return false;
		}

		$profileCodePrefix = getProfileCodePrefixInCol($tablesList[$joinTableIdx], $joinTableCol);		
		return $profileCodePrefix;
	}
	catch (Exception $ex) {
		watchdog_exception('getCriteriaSpliCodePrefixFromJoinTable', $ex);
		expertusLogError($ex);
		return false;
	}
}

/*
 * Generates db_select() statement for criteria autocomplete options from table
 */
function getCriteriaACSelStmt($searchText, $searchQuery, $condType,$unionQry = 0) {
	try {
	//	$selStmt = db_select($table, $tableAlias);
	//	if ($addDistinctClause) {
	//	  $selStmt->distinct();
	//	}
		//$selStmt->addExpression($column, $columnAlias);
	//	$selStmt->condition($column, '%' . db_like($searchText) . '%', 'LIKE');
		//expDebug::dPrintDBAPI('$selStmt', $selStmt);
		//$userId = getIdOfLoggedInUser();
		if($condType == 'cre_rpt_cnd_sta')
			$selStmt = (empty($unionQry)) ? ($searchQuery. " LIKE '".db_like($searchText)."%'") : ($searchQuery. " LIKE '$searchText%' UNION ".$unionQry . " LIKE '(".db_like($searchText)."%'");
		else if ($condType == 'cre_rpt_cnd_con')
			$selStmt = (empty($unionQry)) ? ($searchQuery. " LIKE '%".db_like($searchText)."%'") : ($searchQuery. " LIKE '%$searchText%' UNION ".$unionQry . " LIKE '%".db_like($searchText)."%'");
		else if($condType == 'cre_rpt_cnd_equ')
			$selStmt = (empty($unionQry)) ? ($searchQuery. " = '".escape_string($searchText)."'") : ($searchQuery. " = '".escape_string($searchText)."' UNION ".$unionQry . " = '".escape_string($searchText)."'");
		
		return $selStmt;
	}
	catch (Exception $ex) {
		watchdog_exception('getCriteriaACSelStmt', $ex);
		expertusLogError($ex);
		return false;
	}
}

/*
 * Generates db_select() statement for criteria autocomplete options from joined slt_profile_list_items table
*/
function getCriteriaACSelStmtSPLI($searchText, $narrowCodePrefixesList = array(),  $addDistinctClause = true) {
	try {
			$selStmt = db_select('slt_profile_list_items', 'spli');
			if ($addDistinctClause) {
			  $selStmt->distinct();
			}
			$selStmt->addField('spli', 'name', 'acval');
			$numCodePrefix = count($narrowCodePrefixesList);
		  if ($numCodePrefix == 1) {
		    $selStmt->condition('spli.code', $narrowCodePrefixesList[0] . '%', 'LIKE');
			}
			else if ($numCodePrefix > 1) {
			  $orClause = db_or();
				foreach($narrowCodePrefixesList as $codePrefix) {
				  $orClause->condition('spli.code', $codePrefix . '%', 'LIKE');
				}
				$selStmt->condition($orClause);
			}
			$selStmt->condition(db_or()->condition('spli.name', '%' . db_like($searchText) . '%', 'LIKE')
					                       ->condition('spli.code', '%' . db_like($searchText) . '%', 'LIKE'));
			expDebug::dPrintDBAPI('$selStmt', $selStmt);
			
			return $selStmt;
	}
	catch (Exception $ex) {
		watchdog_exception('getCriteriaACSelStmtSPLI', $ex);
		expertusLogError($ex);
		return false;
	}
}

/**
 * Fetches report type
 */
function getReportBuilderType($reportId) {
	try {
		$selectQuery = db_select('slt_report_details','rep');
		$selectQuery->addField('rep', 'report_builder_type');
		$selectQuery->condition('rep.id', $reportId);
		expDebug::dPrintDBAPI('$selectQuery', $selectQuery);
		
		$reportBuilderType = $selectQuery->execute()->fetchField();		
		expDebug::dPrint('$reportBuilderType = ' . $reportBuilderType);
		
		return $reportBuilderType;
	}
	catch (Exception $ex) {
	  watchdog_exception('getReportBuilderType', $ex);
		expertusErrorThrow($ex);
  }	
}

/**
 * Prepares db_select() orm object for criteria autocomplete options for a single non-union query
 */
function getCriteriaACSelStmtForSingleQuery($searchText, $criteriaDetails) {
	try {
		$selStmt = false;
		
		$splitCol = explode('.', $criteriaDetails['column_name']);
		$tableAlias = (count($splitCol) > 1 && !empty($splitCol['0']))? $splitCol['0'] : '';
		
		if (empty($tableAlias)) {
			$selStmt = getCriteriaACSelStmt($searchText, $criteriaDetails['criteria_query'], $criteriaDetails['condition_type']);
		}	 
		else if ($criteriaDetails['table_name'] != 'slt_profile_list_items') {
			// Get report type
			$reportBuilderType = getReportBuilderType($criteriaDetails['report_details_id']);
			
			if ($reportBuilderType != 'design_wizard' || strpos($criteriaDetails['table_name'], 'listitems') === false) {
			  $selStmt = getCriteriaACSelStmt($searchText, $criteriaDetails['criteria_query'], $criteriaDetails['condition_type']);
			}
			/* Commented as this logic is matching with incorrect column. See Additional Information section in ticket #0035195.
			 * else {
				// It is design_wizard query and column is slt_profile_list item table name. Get code prefix from join table and build query accordingly
				$colCriteriaSqlList = explode(',', $queryBuilder['column_criteria_sql']);
				expDebug::dPrint('$colCriteriaSqlList = ' . print_r($colCriteriaSqlList, true), 4);
				$colSqlList = explode('~~', $queryBuilder['column_sql']);
				expDebug::dPrint('$colSqlList = ' . print_r($colSqlList, true), 4);
				$colIdx = array_search($criteriaDetails['column_name'], $colCriteriaSqlList);
				expDebug::dPrint('$colIdx = ' . print_r($colIdx, true), 4);
				$joinCol = $colSqlList[$colIdx];
				expDebug::dPrint('$joinCol = ' . print_r($joinCol, true), 4);
				$splitJoinCol = explode('.', $joinCol);
				expDebug::dPrint('$splitJoinCol = ' . print_r($splitJoinCol, true), 4);
				if (count($splitJoinCol) == 2) { // just to be safe
					$profileCodePrefix = getProfileCodePrefixInCol($splitJoinCol[0], $splitJoinCol[1]);
					expDebug::dPrint('$profileCodePrefix = ' . print_r($profileCodePrefix, true), 4);
					if (!empty($profileCodePrefix)) {
					  $selStmt = getCriteriaACSelStmtSPLI($searchText, array($profileCodePrefix));
					}
				}
			} */
		}
		else { // criteria table name is slt_profile_list_items table
			$tablesList = explode(',', $queryBuilder['table_sql']);
			$tableAliasesList = explode(',', $queryBuilder['table_alias_sql']);
			$tableJoinList = explode(',', $queryBuilder['table_join_sql']);		
			$profileCodePrefix = getCriteriaSpliCodePrefixFromJoinTable($tableAlias, $tablesList, $tableAliasesList, $tableJoinList);
			
			//if (!empty($profileCodePrefix)) {
				$selStmt = getCriteriaACSelStmtSPLI($searchText, array($profileCodePrefix));
			//}
	  }

		return $selStmt;
	}
	catch (Exception $ex) {
		watchdog_exception('getCriteriaACSelStmtForSingleQuery', $ex);
		expertusLogError($ex);
		return false;
	}
}

/**
 * Prepares db_select() orm object for criteria autocomplete options for union query
 */
function getCriteriaACSelStmtForUnionQuery($searchText, $criteriaDetails, $unionQueryComponentsList) {
	try {
		$spliCodePrefixList = array();
		$stmtList = array();
		$stmtIdx = 0;
		
		// Column aliases are present only in the first union query component
	  /* $columnAliasesList = explode(',', $unionQueryComponentsList[0]->column_alias_sql);
	  expDebug::dPrint('$columnAliasesList = ' . print_r($columnAliasesList, true), 4);
	  
		// Loop through all union_query_info records
		foreach ($unionQueryComponentsList as $unionComponent) {
			//Explode column sql, column alias sql, table_sql, table_alias_sql into respective arrays
			$columnsList = explode('~~', $unionComponent->column_sql);
			expDebug::dPrint('$columnsList = ' . print_r($columnsList, true), 4);
			$tablesList = explode(',', $unionComponent->table_sql);
			expDebug::dPrint('$tablesList = ' . print_r($tablesList, true), 4);
			$tableAliasesList = explode(',', $unionComponent->table_alias_sql);
			expDebug::dPrint('$tableAliasesList = ' . print_r($tableAliasesList, true), 4);
			$tableJoinList = explode(',', $unionComponent->table_join_sql);
			expDebug::dPrint('$tableJoinList = ' . print_r($tableJoinList, true), 4);
			 
			//In column_alias_sql exploded array, find index of criteria column alias name
			$colIdx = array_search($criteriaDetails['column_alias_name'], $columnAliasesList);
			if ($colIdx === false || empty($columnsList[$colIdx])) {
				expDebug::dPrint('Could not find column for column alias ' . $criteriaDetails['column_alias_name'], 4);
				continue; // return false; @TODO: skip search in this component only or skip completely? Currently skipping search only this component.
			}

			// split the column name into table alias name and field name
			expDebug::dPrint('$columnsList[$colIdx] = ' . $columnsList[$colIdx], 4);
			$splitCol = explode('.', $columnsList[$colIdx]);
			$tableAlias = (count($splitCol) > 0 && !empty($splitCol['0']))? $splitCol['0'] : '';
      expDebug::dPrint('$tableAlias = ' . print_r($tableAlias, true), 4);
			// If table alias name is not present
			if (empty($tableAlias)) {
				if (empty($tablesList) || count($tablesList) > 1 || empty($tablesList[0])) {
				  continue; // return false; @TODO: skip search in this component only or skip completely? Currently skipping search only this component.
				}
				$stmtList[$stmtIdx] = getCriteriaACSelStmt($searchText, $tablesList[0], $columnsList[$colIdx], false);
				$stmtIdx++;
			}
			// Else if table alias name is present, find its index in exploded table_alias_sql
			else {
				//   Find the corresponding table name from exploded table_sql
				$tableIdx = array_search($tableAlias, $tableAliasesList);
				expDebug::dPrint('$tableIdx = ' . $tableIdx, 4);
				expDebug::dPrint('$tablesList[$tableIdx] = ' . $tablesList[$tableIdx], 4);
				if ((!empty($tableIdx) && $tableIdx === false) || empty($tablesList[$tableIdx])) {
					expDebug::dPrint('Could not find table for table alias', 4);
					continue; // return false; @TODO: skip search in this component only or skip completely? Currently skipping search only this component.
				}
				 
				//   If the table name is slt_profile_list_items,
				if ($tablesList[$tableIdx] == 'slt_profile_list_items') {
					// Get the profile code prefix from table column whose code is matched with prefix and save the prefix in slt_profile_list table
					$spliCodePrefix = getCriteriaSpliCodePrefixFromJoinTable($tableAlias, $tablesList, $tableAliasesList, $tableJoinList);
					if (!empty($spliCodePrefix) && !in_array($spliCodePrefix, $spliCodePrefixList)) {
					  $spliCodePrefixList[] = $spliCodePrefix;
					}
				}
				else {
					// Else build query using field name, table name
					$stmtList[$stmtIdx] = getCriteriaACSelStmt($searchText, $tablesList[$tableIdx], $columnsList[$colIdx], false, $tableAlias);
			    $stmtIdx++;
				}
			}
		}
		
		// Build profile list query
		if (!empty($spliCodePrefixList)) {
			$stmtList[$stmtIdx] = getCriteriaACSelStmtSPLI($searchText, $spliCodePrefixList, false);
			$stmtIdx++;
		}
		
		if ($stmtIdx == 0) {
			return false;
		}
		
		// If there is only one statement, add distinct to this query and return. Note that this is a possible scenario - slt_profile_list_item in both queries of the union.
		if ($stmtIdx == 1) {
			$stmtList[0]->distinct();
			expdebug::dPrintDBAPI('$stmtList[0]', $stmtList[0]);
			return $stmtList[0];
		}

    // Else union all queries and return unioned query
    for ($i = 1; $i >= $stmtIdx; $i++) {
    	$stmtList[0]->union($stmtList[$i]);
    }     */
		
		$selStmt = getCriteriaACSelStmt($searchText, $criteriaDetails['criteria_query'], $criteriaDetails['condition_type'],$criteriaDetails['criteria_union_query']);
		
    expdebug::dPrintDBAPI('$stmtList[0]', $selStmt[0]);
		return $selStmt;
	}
	catch (Exception $ex) {
		watchdog_exception('getCriteriaACSelStmtForUnionQuery', $ex);
		expertusLogError($ex);
		return false;
	}
}

/**
 * Generates matching reports criteria autocomplete options
 */
function getCriteriaAutoComplete($criteriaId) {
  try {
  	$criteriaACSelStmt = null;
  	$userId = getIdOfLoggedInUser();
  	
    $criteriaDetails = getReportCriteriaDetails($criteriaId);
    expDebug::dPrint('$criteriaDetails = '. print_r($criteriaDetails, true), 5);
    
    $queryBuilder = getQueryFromBuilder($criteriaDetails['report_details_id'], true);
    expDebug::dPrint('$queryBuilder = ' . print_r($queryBuilder, true), 5);
    
    expDebug::dPrint('Search text $_GET[z] = ' . $_GET['z'], 4);
  
    if(empty($criteriaDetails['criteria_query'])){
    	print '';
    	exit;
    }
    if ($queryBuilder['union_query'] == '0') {
      $criteriaACSelStmt = getCriteriaACSelStmtForSingleQuery($_GET['z'], $criteriaDetails);
    }
    else {
    	$criteriaACSelStmt = getCriteriaACSelStmtForUnionQuery($_GET['z'], $criteriaDetails, $queryBuilder['union_qry_info']);
    }

    if (empty($criteriaACSelStmt)) {
    	print '';
    	exit;    	
    }
    
     include_once  $_SERVER["DOCUMENT_ROOT"]."/sites/all/dao/AbstractDAO.php";
    $dao = new AbstractDAO('Report');
    $dao->connect();  
    $dao->execute('SET @report_uid = '.$userId);
    expDebug::dPrint('$criteriaACSelStmt '.$criteriaACSelStmt); 
    $res = $dao->query($criteriaACSelStmt);
    $matchingNames=$dao->fetchAllResults();      
    // Execute the criteria filter query and return result to ac field
    //$matchingNames = $criteriaACSelStmt->execute()->fetchAll();
    expDebug::dPrint('$matchingNames = ' . print_r($matchingNames, true), 4);
    foreach ($matchingNames as $name) {
    	print $name->acval . "\n";
    }
    $dao->closeconnect();
    exit;
  }
  catch (Exception $ex) {
    watchdog_exception('getCriteriaAutoComplete', $ex);
    expertusLogError($ex);
    print '';
    exit;
  }
}

/*
 * Compacts SQL query string
 * Note: Built from pre-existing code. May not be optimal code. @TODO: Reduce the number of lines of code
 */
function compactReportSQL($sql) {
	try{
		expDebug::dPrint('original $sql to be compacted = ' . $sql, 4);
	  $sql = preg_replace('/\s+/', ' ', $sql);
	  $sql = preg_replace('/\s\s+/', ' ', $sql);
	  while (preg_match('/\s{2}/', $sql)) {
	      $sql = preg_replace('/\s{2}/', ' ', $sql);
	  }
	  $sql = trim($sql);
		if(substr($sql, -1) == ';') {
	    $sql = rtrim($sql, ';');
	  }
	  expDebug::dPrint('compacted $sql = ' . $sql, 4);
	  return $sql;
  }
  catch (Exception $ex) {
  	watchdog_exception('compactReportSQL', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * formFieldCondition() - prepares sql filter criteria conditions for adding to sql query
 */
function formFieldCondition($columnName, $criteriaCondition, $inputValue, $criteriaType, $unionQuery, $appendAnd, &$placeholderId){
  try {
  	expDebug::dPrint('Outpurt val::'.print_r(func_get_args(),1));
    // Initialize $retArr
  	$retArr = array();
  	$retArr['condition'] = '';
    $retArr['argument'] = array();
    $retArr['multval'] = false;
    expDebug::dPrint("plceholder key value -->".$criteriaCondition);
    switch ($criteriaCondition) {
      
      case 'cre_rpt_cnd_grt': // Greater than
      	$placeHolderKey = ':arg' . $placeholderId++;
      	$retArr['argument'][$placeHolderKey] = $inputValue;
      	$date_format = dateFormatCheck($inputValue);
      	//change by ayyappans for 42308: Date filter not working for union query.
        /* if ($unionQuery) {
          $retArr['condition'] = 'STR_TO_DATE(' . $columnName . ', \'%b %d, %Y\') >= DATE(' . $placeHolderKey . ') ';
        }
        else {
          $retArr['condition'] = 'DATE(' . $columnName. ') >= STR_TO_DATE(' . $placeHolderKey. ', \'%Y-%m-%d\') ';
        } */
      	//$retArr['condition'] = ' DATE(' . $columnName . ') >= STR_TO_DATE(' . $placeHolderKey . ', \'%Y-%m-%d\') ';
      	//Modified by priya for #0045831
      	//$retArr['condition'] = 'IF(STR_TO_DATE(' . $columnName . ', \'%b %d, %Y\') is not null,STR_TO_DATE(' . $columnName . ', \'%b %d, %Y\'),DATE(' . $columnName . ')) >= DATE(' . $placeHolderKey . ') ';      	
      	$retArr['condition'] = 'IF(STR_TO_DATE(' . rawurldecode($columnName) . ', \'%b %d, %Y\') is not null,STR_TO_DATE(' . rawurldecode($columnName) . ', \'%b %d, %Y\'),DATE(' . rawurldecode($columnName) . ')) >= DATE("' . $retArr['argument'][$placeHolderKey] . '") ';
      	if(isset($_GET['cron_key'])) {
			unset($retArr['argument'][$placeHolderKey]);
		}
        break;
      
      case 'cre_rpt_cnd_les': // Less than
      	$placeHolderKey = ':arg' . $placeholderId++;
      	$retArr['argument'][$placeHolderKey] = $inputValue;
      	$date_format = dateFormatCheck($inputValue);
      	//change by ayyappans for 42308: Date filter not working for union query.
        /* if ($unionQuery) {
          $retArr['condition'] = 'STR_TO_DATE(' . $columnName . ', \'%b %d, %Y\') <= DATE(' . $placeHolderKey . ') ';
        }
        else {
          $retArr['condition'] = ' DATE(' . $columnName . ') <= STR_TO_DATE(' . $placeHolderKey . ', \'%Y-%m-%d\') ';
        } */
      	//$retArr['condition'] = ' DATE(' . $columnName . ') <= STR_TO_DATE(' . $placeHolderKey . ', \'%Y-%m-%d\') ';
      	//Modified by priya for #0045831
      	//$retArr['condition'] = 'IF(STR_TO_DATE(' . $columnName . ', \'%m/%d/%Y\') is not null,STR_TO_DATE(' . $columnName . ', \'%m/%d/%Y\'),DATE(' . $columnName . ')) <= IF(STR_TO_DATE(' . $placeHolderKey . ', \'%m/%d/%Y\') is not null,STR_TO_DATE(' . $placeHolderKey . ', \'%m/%d/%Y\'),DATE(' . $placeHolderKey . ')) ';
      	$retArr['condition'] = 'IF(STR_TO_DATE(' . rawurldecode($columnName) . ', \'%b %d, %Y\') is not null,STR_TO_DATE(' . rawurldecode($columnName) . ', \'%b %d, %Y\'),(IF(STR_TO_DATE(' . rawurldecode($columnName) . ', \'%m/%d/%Y\') is not null,STR_TO_DATE(' . rawurldecode($columnName) . ', \'%m/%d/%Y\'),DATE(' . rawurldecode($columnName) . ')))) <= IF(STR_TO_DATE("' . $retArr['argument'][$placeHolderKey] . '", \'%m/%d/%Y\') is not null,STR_TO_DATE("' . $retArr['argument'][$placeHolderKey] . '", \''.$date_format.'\'),DATE("' . $retArr['argument'][$placeHolderKey] . '")) ';
      	if(isset($_GET['cron_key'])) {
      		unset($retArr['argument'][$placeHolderKey]);
      	}
        break;
        
      case 'cre_rpt_cnd_con': // Contains
      	if (isset($_GET['cron_key'])) { // scheduled reports are only allowed to have multiple values in criteria fields
      		$containsCond = '(';
      		$valuesList = explode('~||', $inputValue);
      		if (count($valuesList) > 1) {
      			$retArr['multval'] = true;
      		}
      		$first = true;
      		foreach ($valuesList as $value) {
      		  if ($first) {
              $first = false;
            }
            else {
              $containsCond .= ' or ';
            }
      			$placeHolderKey = ':arg' . $placeholderId++;
      			$retArr['argument'][$placeHolderKey] = '%' . $value . '%';
      			$containsCond .= rawurldecode($columnName) . ' like ' . $placeHolderKey;
      		} // foreach
      		$containsCond .= ')';
      		$retArr['condition'] = ' ' . $containsCond . ' ';
      	}
      	else { //launched report
	      	$placeHolderKey = ':arg' . $placeholderId++;
	      	$retArr['argument'][$placeHolderKey] = '%' . $inputValue . '%';
	       // $retArr['condition'] = ' ' . $columnName . ' like ' . $placeHolderKey . ' ';
	      	$retArr['condition'] = ' ' . rawurldecode($columnName) . ' like "%' .db_like($inputValue) . '%" ';
	    }
        break;
        
      case 'cre_rpt_cnd_sta': // Starts with
        if (isset($_GET['cron_key'])) { // scheduled reports are only allowed to have multiple values in criteria fields
          $startsWithCond = '(';
          $valuesList = explode('~||', $inputValue);
          if (count($valuesList) > 1) {
            $retArr['multval'] = true;
          }
          $first = true;
          foreach ($valuesList as $value) {
            if ($first) {
              $first = false;
            }
            else {
              $startsWithCond .= ' or ';
            }
            $placeHolderKey = ':arg' . $placeholderId++;
            $retArr['argument'][$placeHolderKey] = $value . '%';
            $startsWithCond .= rawurldecode($columnName) . ' like ' . $placeHolderKey;
          } // foreach
          $startsWithCond .= ')';
          $retArr['condition'] = ' ' . $startsWithCond . ' ';
        }
        else { // launched report
	      	$placeHolderKey = ':arg' . $placeholderId++;
	      	$retArr['argument'][$placeHolderKey] = $inputValue . '%';
	        $retArr['condition'] = ' ' . rawurldecode($columnName) . ' like "' . $inputValue . '%" ';
        }
        break;
      
      case 'in':
      	$inputValueItemsList = explode(',', $inputValue);
      	$inputValueForQueryItemsList = array();
      	foreach ($inputValueItemsList as $inputValueItem) {
      		$placeHolderKey = ':arg' . $placeholderId++;
      		$retArr['argument'][$placeHolderKey] = $inputValueItem;
      		$inputValueForQueryItemsList[] = $placeHolderKey;
      	}
      	$inputValueForQuery = implode(',', $inputValueForQueryItemsList);
        $retArr['condition'] = ' ' . rawurldecode($columnName) . ' IN ( ' . $inputValueForQuery . ' ) ';
        break;
        
      case 'cre_rpt_cnd_equ': // Equal
        if ($criteriaType == 'datetime' || $criteriaType == 'date') {
        	$placeHolderKey = ':arg' . $placeholderId++;
          $retArr['argument'][$placeHolderKey] = $inputValue;
          $retArr['condition'] = ' DATE(' . rawurldecode($columnName) . ') = STR_TO_DATE("' . $retArr['argument'][$placeHolderKey] . '", \'%Y-%m-%d\') ';
        }
        else {
        	if (isset($_GET['cron_key'])) { // scheduled reports are only allowed to have multiple values in criteria fields
            $equalCond = '(';
            $valuesList = explode('~||', $inputValue);
        	  if (count($valuesList) > 1) {
              $retArr['multval'] = true;
            }
            $first = true;
            foreach ($valuesList as $value) {
              if ($first) {
                $first = false;
              }
              else {
                $equalCond .= ' or ';
              }
              $placeHolderKey = ':arg' . $placeholderId++;
              $retArr['argument'][$placeHolderKey] = $value;
              $equalCond .= rawurldecode($columnName) . ' = "' . $retArr['argument'][$placeHolderKey].'"';
            } // foreach
            $equalCond .= ')';
            $retArr['condition'] = ' ' . $equalCond . ' ';
          }
          else { //launched report
          	$placeHolderKey = ':arg' . $placeholderId++;
            $retArr['argument'][$placeHolderKey] = escape_string($inputValue);
            $retArr['condition'] = rawurldecode($columnName) . ' = "' . $retArr['argument'][$placeHolderKey] . '" ';
          }
        }
        break;
      
      default: // All
      	$placeHolderKey = ':arg' . $placeholderId++;
      	$retArr['argument'][$placeHolderKey] = $inputValue;
        $retArr['condition'] = rawurldecode($columnName). " = " . $placeHolderKey . " ";
        break;
    }
    
    if ($appendAnd == 'before') {
      $retArr['condition'] = ' and ' . $retArr['condition'];
    }
    else {
      $retArr['condition'] = $retArr['condition'] . ' and ';
    } 
  
    expDebug::dPrint('$retArr = ' . print_r($retArr, true), 4);
    return $retArr;
  
  }
  catch (Exception $ex) {
    watchdog_exception('formFieldCondition', $ex);
    
    if (isset($_GET['cron_key'])) {
      expertusLogError($ex);
      expDebug::dPrint('Throwing new ReportSchedulesWorkerException', 4);
      throw new ReportSchedulesWorkerException($ex, 'INFORM-OWNER');
    }
    
    // will come here only when it is not a scheduled report
    expertusErrorThrow($ex);    
  }
}

/*
 * getQueryFromBuilder() 
 */
function getQueryFromBuilder($reportId, $addUnionQryInfo = false){
  try { 
    $query = db_select('slt_report_query_builder', 'builder');
    $query->addField('builder', 'query_builder_sql');
    $query->addField('builder', 'where_position');
    $query->addField('builder', 'group_sql');
    $query->addField('builder', 'order_sql');
    $query->addField('builder', 'limit_sql');
    $query->addField('builder', 'union_query');
    $query->addField('builder', 'column_sql');
    $query->addField('builder', 'column_alias_sql');
    $query->addField('builder', 'column_criteria_sql');
    $query->addField('builder', 'table_sql');
    $query->addField('builder', 'table_alias_sql');
    $query->addField('builder', 'table_join_sql');
    $query->condition('builder.report_details_id', $reportId);
    $query->condition('builder.union_query', '2', '!=');
    expDebug::dPrintDBAPI('$query', $query);
    $result = $query->execute()->fetchAssoc();
    expDebug::dPrint('$result = ' . print_r($result, true));
    
    if ($addUnionQryInfo && !empty($result) && $result['union_query'] != '0') {
      $query = db_select('slt_report_query_builder', 'builder');
      $query->addField('builder', 'column_sql');
      $query->addField('builder', 'column_alias_sql');
      $query->addField('builder', 'table_sql');
      $query->addField('builder', 'table_alias_sql');
      $query->addField('builder', 'table_join_sql');
      $query->condition('builder.report_details_id', $reportId);
      expDebug::dPrintDBAPI('$query', $query);
      $result['union_qry_info'] = $query->execute()->fetchAll();
    }
    expDebug::dPrint('$result with union query info = ' . print_r($result, true));
    return $result;
  }
  catch (Exception $ex) {
    watchdog_exception('getQueryFromBuilder', $ex);
    
    if (isset($_GET['cron_key'])) {
      expertusLogError($ex);
      expDebug::dPrint('Throwing new ReportSchedulesWorkerException', 4);
      throw new ReportSchedulesWorkerException($ex, 'INFORM-OWNER');
    }
    
    // will come here only when it is not a scheduled report
    expertusErrorThrow($ex);
  }
  
}

/*
 * getLearnerReportCriteriaAutoComplete() 
 */
function getLearnerReportCriteriaAutoComplete(){
  
  try{
    
    $matchValue  = reportXSSValidate($_GET['z']);
    $criteriaId  = reportXSSValidate($_GET['criteria_id']);
    
    $getFieldQuery = db_select('slt_report_criteria', 'criteria');
    $getFieldQuery->addField('criteria', 'name');
    $getFieldQuery->condition('criteria.id', $criteriaId);
    $criteriaName = $getFieldQuery->execute()->fetchField();
    
    list($tableName, $fieldName) = explode('.', $criteriaName);
        
    $select = db_select($tableName, 'tbl');
    $select->addField('tbl', $fieldName);
    $select->condition('tbl.'.$fieldName, '%' . db_like($matchValue) . '%', 'LIKE');
    $qry_autocomplete = $select->execute()->fetchAll();
    foreach ($qry_autocomplete as $rec_autocomplete) {
      print $rec_autocomplete->$fieldName."\n";
    }
    
  } catch (Exception $ex) {
    
    watchdog_exception('getLearnerReportCriteriaAutoComplete', $ex);
    expertusErrorThrow($ex);
    
  }
     
}

/*
 * getLearnerReportnameAutoComplete() - Returns the matched reportname from slt_person table.
 */
function getLearnerReportnameAutoComplete() { // Needed for search by manager reportname.
  
  try{
    
    $searchText = escape_string($_GET['z']);
    $listOfRolesForLoggedUser = $GLOBALS['user']->roles;
    $secRoleQry = db_select('slt_groups', 'grp');
    $secRoleQry->addField('grp', 'id', 'sec_role_id');
    $rNameArr = array();
    foreach($listOfRolesForLoggedUser as $rid => $rname){
    	$rNameArr[]= $rname;
    }
    $secRoleQry->condition('grp.name', $rNameArr, 'IN');
    $secRoleRes = $secRoleQry->execute()->fetchAll();
    $orQuery = db_or();
    $select = db_select('slt_report_details', 'rep');
    $select->addField('rep', 'title', 'title');
    $select->condition('rep.title', '%' . ($searchText) . '%', 'LIKE');
    $select->condition('rep.status', 'cre_rpt_rps_del', '!=');
    if(user_access('administrator') || is_data_admin()){
    	$select->where('(1=1)');
    }
    else if(!empty($secRoleRes)){
    	foreach($secRoleRes as $roleRecord){
    		$orQuery->where('FIND_IN_SET('.$roleRecord->sec_role_id.',rep.report_visibility) ');
    	}
    	$select->condition($orQuery);
    }
    $select->orderBy('title', 'ASC');

    expDebug::DPrintDBAPI("report search query ",$select);
    $matchingReportNames = $select->execute()->fetchAll();
    expDebug::DPrint("report search query result >>> ".print_r($matchingReportNames,true),5);
  
    foreach ($matchingReportNames as $reportName) {
      print $reportName->title . "\n";
    }
    
  } catch (Exception $ex) {
    
    watchdog_exception('getLearnerReportnameAutoComplete', $ex);
    expertusErrorThrow($ex);
    
  }   
  
}

/*
 * getAvailableReportTypes() - For fetching report types.
 */
function getAvailableReportTypes($adminAccess){
  try{
    
  $query = getLearnerReportsDetailsQuery($adminAccess, 'roles');

  $access = getLearnerUserAccessPrivileges($adminAccess);
  
  $result = $query->execute()->fetchAll();
  
  $availableTypes = array();
  $multiReportTypeArray = array();
  foreach($result as $reportFor){
    $multiReportTypeArray = explode(',',$reportFor->report_visibility);
    foreach($multiReportTypeArray as $multiReport){
      $multiReport = trim($multiReport);
      if($access[$multiReport] == 1){
        $availableTypes[$multiReport] = $multiReport;
      }
    }
  }
  
  expDebug::dPrint('$access '.print_r($access, true) , 4);
  expDebug::dPrint('$availableTypes '.print_r($availableTypes, true) , 4);
  return $availableTypes;
  
  } catch (Exception $ex) {
    
    watchdog_exception('getAvailableReportTypes', $ex);
    expertusErrorThrow($ex);
    
  }   
}

/*
 * getLearnerUserAccessPrivileges() 
 */
function getLearnerUserAccessPrivileges($adminAccess){
  
  try{
    
    $userId = getSltpersonUserId();
    
    if($adminAccess){
      $userAccessArray['cre_rpt_vis_usr'] = true;
      $userAccessArray['cre_rpt_vis_ins'] = true;
      $userAccessArray['cre_rpt_vis_mgr'] = true;
      $userAccessArray['cre_rpt_vis_adm'] = true;
    } else {
      $userAccessArray['cre_rpt_vis_usr'] = true;
      $userAccessArray['cre_rpt_vis_ins'] = is_instructor($userId);
      $userAccessArray['cre_rpt_vis_mgr'] = is_manager($userId);
      $userAccessArray['cre_rpt_vis_adm'] = (user_access('Administration Perm') == 1) ? true : false;
    }
    
    expDebug::dPrint('report.inc : $userAccessArray : '.print_r($userAccessArray, true) , 4);
    return $userAccessArray;
    
  } catch (Exception $ex) {
    
    watchdog_exception('getLearnerUserAccessPrivileges', $ex);
    expertusErrorThrow($ex);
    
  }   
  
}

/*
 * deleteLearnerReport() - to delete the report from the search page
 */
function deleteLearnerReport($id){
  $txn = db_transaction();
  try{
      
    $userId = getSltpersonUserId();
    
    $fields['status'] = 'cre_rpt_rps_del';
    $fields['updated_by'] = $userId;
    $fields['updated_on'] = now();
    
    $update = db_update('slt_report_details');
    $update->fields($fields);
    $update->condition('id', $id);
    $update->execute();
    add_audit_trail_entry($userId, $id, 'cre_rpt_rss', 'exp_sp_lnrreports', 'deleteLearnerReport', 'Report Deleted');
    $schdSelect = db_select('slt_report_schedules', 'rep_sched');
    $schdSelect->addField('rep_sched', 'id');
    $schdSelect->addField('rep_sched', 'status');
    $schdSelect->condition('rep_sched.report_id', $id);
    expDebug::dPrintDBAPI('$schdSelect', $schdSelect);
    $schdPrevStatuses = $schdSelect->execute()->fetchAll();
    expDebug::dPrint('$schdPrevStatuses' . print_r($schdPrevStatuses, true));
      
    $schdFields = array();
    $schdFields['status'] = 'cre_rpt_rss_del';
    $schdFields['updated_by'] = $userId;
    $schdFields['updated_on'] = now();
    $schdUpdate = db_update('slt_report_schedules');
    $schdUpdate->fields($schdFields);
    $schdUpdate->condition('report_id', $id);
    expDebug::dPrintDBAPI('$schdUpdate', $schdUpdate);
    $schdUpdate->execute();

    foreach ($schdPrevStatuses as $schd) {
      $schdPrev = array($schd);
      make_audit_trail_entries($schdFields, $userId, $schdPrev, $schd->id, 'Updated', 'cre_rpt_rss', 'exp_sp_lnrreports',
                                         'deleteLearnerReport', array('status'));
    }
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('deleteLearnerReport', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * publishUnpublishLearnerReport()  - to publish and unpublish the reports from the search page
 */
function publishUnpublishLearnerReport($type, $id) {
	$txn = db_transaction();
  try {
    $userId = getSltpersonUserId();

    $status = ($type == 'publish') ? 'cre_rpt_rps_atv' : 'cre_rpt_rps_itv';
        
    $fields = array();
    $fields['status'] = $status;
    $fields['updated_by'] = $userId;
    $fields['updated_on'] = now();
    
    $update = db_update('slt_report_details');
    $update->fields($fields);
    $update->condition('id', $id);
    expDebug::dPrintDBAPI('$update', $update);
    $update->execute();
    if($status == 'cre_rpt_rps_atv'){
    	$value = 'cre_rpt_rps_itv';
    }else{
    	$value = 'cre_rpt_rps_atv';
    }
    add_audit_trail_entry($userId, $id, 'cre_rpt_rss', 'exp_sp_lnrreports', 'publishUnpublishLearnerReport', 'Updated status',$userId,$value,$status);    
    
    
    if ($status == 'cre_rpt_rps_itv') {
    	$schdSelect = db_select('slt_report_schedules', 'rep_sched');
    	$schdSelect->addField('rep_sched', 'id');
    	$schdSelect->addField('rep_sched', 'status');
    	$schdSelect->condition('rep_sched.report_id', $id);
    	$schdSelect->condition('rep_sched.status', 'cre_rpt_rss_atv', '=');
    	expDebug::dPrintDBAPI('$schdSelect', $schdSelect);
    	$schdPrevStatuses = $schdSelect->execute()->fetchAll();
    	expDebug::dPrint('$schdPrevStatuses' . print_r($schdPrevStatuses, true));
    	
    	$schdFields = array();
    	$schdFields['status'] = 'cre_rpt_rss_itv';
      $schdFields['updated_by'] = $userId;
      $schdFields['updated_on'] = now();
	    $schdUpdate = db_update('slt_report_schedules');
	    $schdUpdate->fields($schdFields);
	    $schdUpdate->condition('report_id', $id);
        $schdUpdate->condition('status', 'cre_rpt_rss_atv');
	    expDebug::dPrintDBAPI('$schdUpdate', $schdUpdate);
	    $schdUpdate->execute();

	    foreach ($schdPrevStatuses as $schd) {
	    	$schdPrev = array($schd);
	      make_audit_trail_entries($schdFields, $userId, $schdPrev, $schd->id, 'Updated', 'cre_rpt_rss', 'exp_sp_lnrreports',
                                         'publishUnpublishLearnerReport', array('status'));
	    }
    }
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('publishUnpublishLearnerReport', $ex);
    expertusErrorThrow($ex);
  }
}

function loadReportLayout($reportId){
  
  try{
    
    $themeExist = 0;
    $userId = getSltpersonUserId();
    
    // All themes
    $selectAllThemes = db_select('slt_report_common_themes', 'common_themes');
    $selectAllThemes->addField('common_themes', 'id');
    $selectAllThemes->addField('common_themes', 'theme_name');
    $resultAllThemes = $selectAllThemes->execute()->fetchAll();
    $themeExist = checkThemeExist($reportId);
    if(empty($themeExist) || $themeExist == 0){
      
      
      $reportThemeDetails = fetchThemeDetails(1);
      $reportThemeDetails['theme_name'] = 'ExpertusONE Theme';
      $reportThemeDetails['report_details_id'] = $reportId;
      $reportThemeDetails['created_by'] = $userId;
      $reportThemeDetails['created_on'] = now();
      $reportThemeDetails['updated_by'] = $userId;
      $reportThemeDetails['updated_on'] = now();
      
      $insertQuery = db_insert('slt_report_themes');
      $insertQuery->fields($reportThemeDetails);
      $insertQuery->execute();
      
    }
    
    // Criteria
    $criteriaArray = loadAddedReportCriteria($reportId);
    
    $resultReportDetails = fetchReportDetails($reportId);
    
    $result['report_details'] = $resultReportDetails;
    $result['all_themes']     = $resultAllThemes;
    $result['criteria']       = $criteriaArray;
    $result['admin_access']   = reportAdminAccess();
    
    expDebug::dPrint('$result = ' . print_r($result, true), 5);
    print json_encode($result);
    
  }
  catch (Exception $ex) {  
    watchdog_exception('loadReportLayout', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * loadAddedReportCriteria() 
 */
function loadAddedReportCriteria($reportId){
  
  try {
    
    $selectCriteria = db_select('slt_report_criteria', 'criteria');
    $selectCriteria->addField('criteria', 'id');
    $selectCriteria->addField('criteria', 'name'); 
    $selectCriteria->addField('criteria', 'column_name');
	  $selectCriteria->addField('criteria', 'column_alias_name');
    $selectCriteria->addField('criteria', 'column_type');
    $selectCriteria->addField('criteria', 'mandatory');
    $selectCriteria->addField('criteria', 'use_user_details');
    $selectCriteria->addField('criteria', 'condition_type');
    $selectCriteria->condition('criteria.report_details_id', $reportId);
    $criteriaArray = $selectCriteria->execute()->fetchAll();
    
    expDebug::dPrintDBAPI('$loadAddedReportCriteria query= ',$selectCriteria);
    
    foreach ($criteriaArray as $key=> $criteria){
	    if($criteria->use_user_details == 1){
	    	$criteriaArray[$key]->user_details = reportGetCriteriaValueFromUserDetail($criteria, false);
	    }
    }
    expDebug::dPrint('$loadAddedReportCriteria = ' . print_r($criteriaArray, true), 5);
    return $criteriaArray;
  
  } catch (Exception $ex) {
    
    watchdog_exception('loadAddedReportCriteria', $ex);
    expertusErrorThrow($ex);
    
  }
}

/*
 * checkThemeExist() - checking if theme exits
 */
function checkThemeExist($reportId){
  
  try {
    
    $select = db_select('slt_report_themes', 'theme');
    $select->addField('theme', 'id' );
    $select->condition('theme.report_details_id', $reportId);
    $result = $select->execute()->fetchField();
    return $result;
  
  } catch (Exception $ex) {
    
    watchdog_exception('checkThemeExist', $ex);
    expertusErrorThrow($ex);
    
  }
}


/*
 * getReportHeader() - 
 */
function getReportHeader($reportId) {
  try { 
    $select = db_select('slt_report_details', 'rep');
    $select->addField('rep', 'title', 'title');
    $select->condition('rep.id', $reportId , '=');
    expDebug::dPrintDBAPI('$select', $select);
    
    $result = $select->execute()->fetchField();
    expDebug::dPrint('$result = ' . print_r($result, true));
  
    return $result;
  }
  catch (Exception $ex) {
    watchdog_exception('getReportHeader', $ex);
    
    if (isset($_GET['cron_key'])) {
      expertusLogError($ex);
      expDebug::dPrint('Throwing new ReportSchedulesWorkerException', 4);
      throw new ReportSchedulesWorkerException($ex, 'INFORM-OWNER');
    }
    
    expertusErrorThrow($ex);
  }
}

/*
 * fetchReportDetails() - for fetching report details 
 */
function fetchReportDetails($reportId){
  
  try {
        
    $select = db_select('slt_report_details', 'report');
    $select->leftjoin('slt_report_query_builder', 'query_builder', 'report.id = query_builder.report_details_id AND query_builder.union_query != \'2\'');
    $select->leftjoin('slt_report_themes', 'theme', 'report.id = theme.report_details_id');
    $select->addField('report', 'report_header_text');
    $select->addField('report', 'report_footer_text');
    $select->addField('report', 'report_builder_type');
    $select->addField('report', 'title');
  
    $select->addField('theme', 'theme_name');
    $select->addField('theme', 'header_style');
    $select->addField('theme', 'header_style_tip');
    $select->addField('theme', 'footer_style');
    $select->addField('theme', 'footer_style_tip');
    $select->addField('theme', 'criteria_column_style');
    $select->addField('theme', 'criteria_column_style_tip');
    $select->addField('theme', 'grid_header_style');
    $select->addField('theme', 'grid_header_style_tip');
    $select->addField('theme', 'grid_header_font_style');
    $select->addField('theme', 'grid_row_border_style');
    $select->addField('theme', 'grid_row_style');
    $select->addField('theme', 'grid_alternate_row_style');
    $select->addField('theme', 'grid_row_style_tip');
    
    $select->addField('query_builder', 'column_sql');
    $select->addField('query_builder', 'column_alias_sql');
    $select->addField('query_builder', 'limit_sql');
    $select->addField('query_builder', 'table_sql');
    $select->addField('query_builder', 'table_alias_sql');
    $select->condition('report.id', $reportId);
    expDebug::dPrintDBAPI('$select', $select);
    $resultReportDetails = $select->execute()->fetchAssoc();
    /*--Fix for #34161: Issue in Reports --*/
    if(strlen($resultReportDetails['report_header_text']) > 57) {
     $resultReportDetails['report_header_text_short'] = titleController('EXP-SP-LNRREPORTS-REPORT-HEADER-TEXT',$resultReportDetails['report_header_text'], 57);
    } else {
     $resultReportDetails['report_header_text_short'] = '';
    }
    expDebug::dPrint('$resultReportDetails = ' . print_r($resultReportDetails, true));
    return $resultReportDetails; 
    
  } catch (Exception $ex) {
    
    watchdog_exception('fetchReportDetails', $ex);
    expertusErrorThrow($ex);
    
  }
}


/*
 * fetchThemeDetails() - for fetching theme details
 */
function fetchThemeDetails($themeId, $type = ''){
  
  try {
    
    if($type == 'individual'){
      $select = db_select('slt_report_themes', 'theme');
    } else {
      $select = db_select('slt_report_common_themes', 'theme');
    }
    $select->addField('theme', 'theme_name');
    $select->addField('theme', 'header_style');
    $select->addField('theme', 'header_style_tip');
    $select->addField('theme', 'footer_style');
    $select->addField('theme', 'footer_style_tip');
    $select->addField('theme', 'criteria_column_style');
    $select->addField('theme', 'criteria_column_style_tip');
    $select->addField('theme', 'grid_header_style');
    $select->addField('theme', 'grid_header_style_tip');
    $select->addField('theme', 'grid_header_font_style');
    $select->addField('theme', 'grid_row_border_style');
    $select->addField('theme', 'grid_row_style');
    $select->addField('theme', 'grid_alternate_row_style');
    $select->addField('theme', 'grid_row_style_tip');
    if($type == 'individual'){
      $select->condition('theme.report_details_id', $themeId);
    } else {
      $select->condition('theme.id', $themeId);
    }
    $resultThemeDetails = $select->execute()->fetchAssoc();
    return $resultThemeDetails; 
    
  } catch (Exception $ex) {
    
    watchdog_exception('fetchThemeDetails', $ex);
    expertusErrorThrow($ex);
    
  }
  
}

/*
 * updateReportHeaderFooter() - to update the reports header and footer.
 */
function updateReportHeaderFooter($reportId, $type, $value){
  
  try {
    $value = str_replace(EXP_AC_SEPARATOR, '/', $value);  
    $userId = getSltpersonUserId();
    
    if($type == 'header'){
      $fields['report_header_text'] = $value;
    } else {
      $fields['report_footer_text'] = $value;
    }
    $fields['updated_by'] = $userId;
    $fields['updated_on'] = now();
    
    $updateQuery = db_update('slt_report_details');
    $updateQuery->fields($fields);
    $updateQuery->condition('id', $reportId);
    $updateQuery->execute();
  
    print json_encode( array('result'=>'updated', 'type' => $type) );

  } catch (Exception $ex) {
    
    watchdog_exception('updateReportHeaderFooter', $ex);
    expertusErrorThrow($ex);
    
  }
  
}


/*
 * updateReportHeaderFooterStyle() - to update the reports header and footer CSS/Style.
 */
function updateReportHeaderFooterStyle($reportId, $type, $value){
  
  try {

    list($fontList, $fontSize, $fontStyle, $fontColor, $backgroundColor, $borderColor, $lineHeight, $altbackgroundColor) = explode('$$', $value);
    $fontColor = str_replace('*','#',$fontColor);
    
    $userId = getSltpersonUserId();
    
    $fields['theme_name'] = '';
    
    if($type == 'grid_header'){
      
      $backgroundColor = str_replace('*','#',$backgroundColor);
      $borderColor = str_replace('*','#',$borderColor);
      
      $style = 'font-family : '.$fontList.'; font-size : '.$fontSize.'; color : '.$fontColor.'; font-style: '.$fontStyle.';';
      $otherStyle = 'margin: 0px; padding: 0px; padding-left: 2px; background-color: '.$backgroundColor.'; line-height: '.$lineHeight.'px; border: 1px solid '.$borderColor.'';
      
      $styleTip = '{"backgroundcolor":"'.$backgroundColor.'","lineheight":"'.$lineHeight.'","border":"'.$borderColor.'","fontfamily":"'.$fontList.'","fontsize":"'.$fontSize.'","color":"'.$fontColor.'","fontstyle":"'.$fontStyle.'"}';
      
      $fields['grid_header_style'] = $otherStyle;
      $fields['grid_header_font_style'] = $style;
      
      $fields['grid_header_style_tip'] = $styleTip;
      
    } else if($type == 'grid_row'){
      
      $backgroundColor = str_replace('*','#',$backgroundColor);
      $altbackgroundColor = str_replace('*','#',$altbackgroundColor);
      $borderColor = str_replace('*','#',$borderColor);

      $style = 'margin: 0px; padding: 0px; padding-left: 4px; background-color: '.$backgroundColor.'; line-height: '.$lineHeight.'px; font-family : '.$fontList.' ; font-size : '.$fontSize.'; color : '.$fontColor.'; font-style: '.$fontStyle.'; border-bottom:1px solid '.$borderColor.';'; 
      $otherStyle = 'margin: 0px; padding: 0px; padding-left: 4px; background-color: '.$altbackgroundColor.'; line-height: '.$lineHeight.'px; font-family : '.$fontList.'; font-size : '.$fontSize.'; color : '.$fontColor.'; font-style: '.$fontStyle.'; border-bottom:1px solid '.$borderColor.';';
      $borderStyle = 'border-left:1px solid '.$borderColor.'; border-right:1px solid '.$borderColor.';';
            
      $styleTip = '{"backgroundcolor":"'.$backgroundColor.'","backgroundaltcolor":"'.$altbackgroundColor.'","lineheight":"'.$lineHeight.'","fontfamily":"'.$fontList.'","fontsize":"'.$fontSize.'","color":"'.$fontColor.'","fontstyle":"'.$fontStyle.'","border":"'.$borderColor.'"}';

      $fields['grid_row_style'] = $style;
      $fields['grid_alternate_row_style'] = $otherStyle;
      $fields['grid_row_border_style'] = $borderStyle;
      
      $fields['grid_row_style_tip'] = $styleTip;
      
    } else {

      $style = 'font-family : '.$fontList.'; font-size : '.$fontSize.'; color : '.$fontColor.'; font-style: '.$fontStyle.';';
      $styleTip = '{"fontfamily":"'.$fontList.'","fontsize":"'.$fontSize.'","color":"'.$fontColor.'","fontstyle":"'.$fontStyle.'"}';
      
      $fields[$type.'_style'] = $style;
      $fields[$type.'_style_tip'] = $styleTip;
    }
    $fields['updated_by'] = $userId;
    $fields['updated_on'] = now();
    
    $updateQuery = db_update('slt_report_themes');
    $updateQuery->fields($fields);
    $updateQuery->condition('report_details_id', $reportId);
    $updateQuery->execute();
    
    print json_encode( array('style1'=> $style, 'style2' => $otherStyle, 'style3' => $borderStyle) );

  } catch (Exception $ex) {
    
    watchdog_exception('updateReportHeaderFooter', $ex);
    expertusErrorThrow($ex);
    
  }
  
}

/*
 * loadStyleReport() 
 */
function loadStyleReport($reportId, $type){
  
  try{
      
    $select = db_select('slt_report_themes', 'theme');
    $select->addField('theme', $type.'_style_tip', 'style');
    $select->condition('theme.report_details_id', $reportId);
    $result = $select->execute()->fetchField();
    //print $result;
    /*
     * Temp fix for 0028282: French -- Issue in Reports
     * Todo: Clean up the slt_report_themes tables data and remove below code
     */
    if(strstr($result, '{')){
    	print str_replace(array('font-family','font-size','font-style'), array('fontfamily','fontsize','fontstyle'), $result);
    }
    else{
    	$resulrtArray = explode(';', $result);
    	$tempString = '';
    	foreach($resulrtArray as $key => $value ){
    		if($value){     
    			$resulrtListArray = explode(':', $value);
    			$resulrtListArray[0] = trim($resulrtListArray[0]);
    			$resulrtListArray[0] ='"'.str_replace(array('font-family','font-size','font-style'), array('fontfamily','fontsize','fontstyle'), $resulrtListArray[0]).'"';
    			$resulrtListArray[1] ='"'.trim($resulrtListArray[1]).'"';
    			$tempString .= $resulrtListArray[0] . ':' . $resulrtListArray[1] .',';    			
    		}
    	}
    	$result = "{".rtrim($tempString, ",")."}";
    	print $result; 
    }
  } catch (Exception $ex) {
    
    watchdog_exception('loadStyleReport', $ex);
    expertusErrorThrow($ex);
    
  }
  
}

/**
 * Callback for administration/report-search/update-column-alias/%/%/%
 * Called only for wizard created queries, for sql query builder queries column aliases are set through the query itself.
 */
function updateColumnAlias($reportId, $oldAlias, $newAlias){
  try{
  	  
    $selectQuery = db_select('slt_report_query_builder','builder');
    $selectQuery->leftjoin('slt_report_details', 'details', 'details.id = builder.report_details_id');
    $selectQuery->addField('details', 'report_builder_type');
    $selectQuery->addField('builder', 'column_alias_sql');
    $selectQuery->addField('builder', 'query_builder_sql');
    $selectQuery->condition('builder.report_details_id', $reportId);
    $columnResultSet = $selectQuery->execute()->fetchAssoc();
    
    $columnAliasList = $columnResultSet['column_alias_sql'];
    $queryBuilderSql = $columnResultSet['query_builder_sql'];
    $reportBuilderType = $columnResultSet['report_builder_type'];
    
    $columnAliasListArray = explode(',', $columnAliasList);
    
    $oldAlias = rawurldecode($oldAlias);
    $newAlias = rawurldecode($newAlias);
    
    if(in_array($newAlias, $columnAliasListArray)){
      
      print json_encode( array('result'=> 'already_exist', 'report_id' => $reportId) );
      
    } else {
    
      $key = array_search($oldAlias, $columnAliasListArray);
      $columnAliasListArray[$key] = $newAlias;
      
      $userId = getSltpersonUserId();
      
      $queryBuilderSql = str_replace('AS "'.$oldAlias.'"','AS "'.$newAlias.'"', $queryBuilderSql);

      expDebug::dPrint('$$queryBuilderSql : '.$queryBuilderSql , 4);
      
      if($reportBuilderType == 'design_wizard'){
        $wherePosition = strpos(strtolower($queryBuilderSql), 'where') + 6;
        $wherePosition = empty($wherePosition) ? 0 : $wherePosition;
      } else {
        include_once(drupal_get_path('module', 'exp_sp_lnrreports') .'/php_sql_parser/src/PHPSQLParser.php');
        $parser = new PHPSQLParser($queryBuilderSql, true);
        $parserArray = $parser->parsed;
        $unionArray = $parserArray['UNION'];
        $unionCount = count($unionArray);
        $unionQuery = 0;
        if($unionCount){
           $unionQuery = 1;
           $whereArray = $unionArray[$unionCount-1]['WHERE'];
        } else {
           $whereArray = $parserArray['WHERE'];
        }
        $wherePosition = 0;
        if(count($whereArray)){
           $wherePosition = $whereArray[0]['position'];
        }        
      }
      
      $columnAliasNew = implode(',', $columnAliasListArray);
      $fields['query_builder_sql'] = $queryBuilderSql;
      $fields['where_position'] = $wherePosition;
      $fields['column_alias_sql'] = $columnAliasNew;
      $fields['updated_by'] = $userId;
      $fields['updated_on'] = now();
      
      $updateQuery = db_update('slt_report_query_builder');
      $updateQuery->fields($fields);
      $updateQuery->condition('report_details_id', $reportId);
      $updateQuery->execute();
      
      print json_encode( array('result'=> 'updated', 'report_id' => $reportId, 'column_alias_sql' => $columnAliasNew) );
      
    }
    
  } catch (Exception $ex) {
    
    watchdog_exception('updateColumnAlias', $ex);
    expertusErrorThrow($ex);
    
  }
  
}

/*
 * updateCriteriaName() 
 */
function updateCriteriaName($criteriaId, $value){
  
  try{
    
    $userId = getSltpersonUserId();
    
    $fields['name'] = $value;
    $fields['updated_by'] = $userId;
    $fields['updated_on'] = now();
    
    $updateQuery = db_update('slt_report_criteria');
    $updateQuery->fields($fields);
    $updateQuery->condition('id', $criteriaId);
    $updateQuery->execute();
    
    print json_encode( array('result'=> 'updated', 'criteria_id' => $criteriaId) );
    
  } catch (Exception $ex) {
    
    watchdog_exception('updateCriteriaName', $ex);
    expertusErrorThrow($ex);
    
  }
  
}

/*
 * changeReportTheme() 
 */
function changeReportTheme($reportId, $themeId){
  
  try{
    
    $userId = getSltpersonUserId();
    
    $reportThemeDetails = fetchThemeDetails($themeId);
    $reportThemeDetails['updated_by'] = $userId;
    $reportThemeDetails['updated_on'] = now();
    
    $updateQuery = db_update('slt_report_themes');
    $updateQuery->fields($reportThemeDetails);
    $updateQuery->condition('report_details_id', $reportId);
    $updateQuery->execute();
    
    print json_encode( array('result'=> 'updated', 'report_details' => fetchReportDetails($reportId)) );
    
  } catch (Exception $ex) {
    
    watchdog_exception('changeReportTheme', $ex);
    expertusErrorThrow($ex);
    
  }
  
}

/**
 * saveReportTheme() - Saving the theme to reuse in other reports 
 */
function saveReportTheme($reportId, $newThemeName=''){
  
  try{
    $newThemeName = $_POST['newThemeName'];
    $themeNameExist = checkDuplicateThemeName($newThemeName);
    if(empty($themeNameExist)){
    
      $userId = getSltpersonUserId();
      
      $reportfields['theme_name'] = $newThemeName;
      $reportfields['updated_by'] = $userId;
      $reportfields['updated_on'] = now();
      
      $updateQuery = db_update('slt_report_themes');
      $updateQuery->fields($reportfields);
      $updateQuery->condition('report_details_id', $reportId);
      $updateQuery->execute();
      
      // Create a new common theme
      $reportThemeDetails = fetchThemeDetails($reportId, 'individual');
      $reportThemeDetails['theme_name'] = $newThemeName;
      $reportThemeDetails['created_by'] = $userId;
      $reportThemeDetails['created_on'] = now();
      $reportThemeDetails['updated_by'] = $userId;
      $reportThemeDetails['updated_on'] = now();
      
      $insertQuery = db_insert('slt_report_common_themes');
      $insertQuery->fields($reportThemeDetails);
      $insertQuery->execute();
       	
      // All themes
      $selectAllThemes = db_select('slt_report_common_themes', 'common_themes');
      $selectAllThemes->addField('common_themes', 'id');
      $selectAllThemes->addField('common_themes', 'theme_name');
      $resultAllThemes = $selectAllThemes->execute()->fetchAll();
      
      print json_encode( array('result'=> 'created', 'all_themes' => $resultAllThemes) );
    
    } else {
      print json_encode( array('result'=> 'duplicate') );
    }
  
  } catch (Exception $ex) {
    
    watchdog_exception('saveReportTheme', $ex);
    expertusErrorThrow($ex);
    
  }
  
}

/*
 * checkDuplicateThemeName() 
 */
function checkDuplicateThemeName($themeName){
	try{
  $select = db_select('slt_report_common_themes', 'theme');
  $select->addField('theme', 'id');
  $select->condition('theme.theme_name', $themeName);
  return $select->execute()->fetchField();
  } catch (Exception $ex) {
  	watchdog_exception('checkDuplicateThemeName', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * loadReportCriteria() 
 */
function loadReportCriteria($reportId){
  
  try{
    
    $select = db_select('slt_report_query_builder', 'query_builder');
    $select->addField('query_builder', 'column_criteria_sql');
    $select->addField('query_builder', 'column_criteria_alias_sql');
    $select->condition('query_builder.report_details_id', $reportId);
    $select->condition('query_builder.union_query', '2', '!=');
    $result = $select->execute()->fetchAssoc();
    
    print drupal_json_encode( array('result' => 'updated', 'criteria_details' => $result) );
    
  } catch (Exception $ex) {
    
    watchdog_exception('loadReportCriteria', $ex);
    expertusErrorThrow($ex);
    
  }
  
}

/*
 * addReportCriteria() 
 */
function addReportCriteria($reportId, $criteriaSQL, $criteriaAliasSQL){
 
  try{
  	global $databases,$finalArray;
  	$finalArray = array();
    // Begin - Get field type
    $criteriaSQLArray = explode('.', $criteriaSQL);
    
    $tableNameQuery = db_select('slt_report_query_builder', 'builder');
    $tableNameQuery->leftjoin('slt_report_details', 'details', 'builder.report_details_id = details.id');
    $tableNameQuery->addField('details', 'report_builder_type');
    $tableNameQuery->addField('builder', 'table_sql'); 
    $tableNameQuery->addField('builder', 'table_alias_sql');
    $tableNameQuery->addField('builder', 'column_sql');
    $tableNameQuery->addField('builder', 'union_query');
    $tableNameQuery->addField('builder', 'query_builder_sql');
    $tableNameQuery->condition('builder.report_details_id', $reportId);
    $tableNameQuery->condition('builder.union_query', '2', '!=');
    expDebug::dPrintDBAPI('$tableNameQuery', $tableNameQuery);
    $tableNameResult = $tableNameQuery->execute()->fetchAssoc();
    expDebug::dPrint('$tableNameResult = ' . print_r($tableNameResult, true));
    
    $tableSqlArray = explode(',',$tableNameResult['table_sql']);
    $tableAliasSqlArray = explode(',',$tableNameResult['table_alias_sql']);
    
    $reportType = $tableNameResult['report_builder_type'];    
    
    if(count($criteriaSQLArray) >= 1){
      if($reportType != 'query_builder'){
          $tableName = $criteriaSQLArray[0];         
          if(strstr($tableName, 'listitems')){
          	$tableName = 'slt_profile_list_items';
          }else{
          	$tableName = $criteriaSQLArray[0];
          }          
	      $criteriaFieldName = $criteriaSQLArray[1];
      } else {
      	if(count($criteriaSQLArray) > 1){

	      $indexCount = array_search($criteriaSQLArray[0], $tableAliasSqlArray);
	      $tableName = $tableSqlArray[$indexCount];
	      $criteriaFieldName = $criteriaSQLArray[1];
      	}else{
      		$tableName = $tableNameResult['table_sql'];

      		$criteriaFieldName = $criteriaSQLArray[0];
      	}
	      
	      $entViewsArr = array('slt_course_template' => 'slv_entities_course',
									      		'slt_course_class' => 'slv_entities_class',
									      		'slt_order' => 'slv_entities_order',
									      		'slt_person' => 'slv_entities_user',
									      		'slt_group' => 'slv_entities_group',
									      		'slt_program' => 'slv_entities_program',
									      		'slt_location' => 'slv_entities_location',
									      		'slt_survey' => 'slv_entities_survey_assessment',
									      		'slt_survey_questions' => 'slv_entities_surass_question',
									      		'slt_organization' => 'slv_entities_organization',
									      		'slt_discount' => 'slv_entities_discount',
									      		'slt_content' => 'slv_entities_content',
									      		'slt_announcement' => 'slv_entities_announcement',
									      		'slt_banner' => 'slv_entities_banner'
									      );
	      
	      $viewDesc[1] = db_query("SELECT VIEW_DEFINITION FROM INFORMATION_SCHEMA.VIEWS
       											WHERE TABLE_SCHEMA = '".$databases['default']['report']['database']."'
    												AND TABLE_NAME = '".$tableName."'")->fetchField();
	      
	      $inc = 1;
	      if(count($criteriaSQLArray) > 1)
	      	$colName[1] = $criteriaSQLArray[1];
	      else 
	      	$colName[1] = $criteriaSQLArray[0];
	      
	      if($tableNameResult['union_query'] == 1 && count($criteriaSQLArray) > 1){
	      	$unionTblQry = db_select('slt_report_query_builder', 'builder');
	      	$unionTblQry->innerjoin('slt_report_details', 'details', 'builder.report_details_id = details.id');
	      	$unionTblQry->addField('builder', 'column_sql');
	      	$unionTblQry->addField('builder', 'table_sql');
	      	$unionTblQry->condition('builder.report_details_id', $reportId);
	      	$unionTblQry->condition('builder.union_query', '1', '!=');
	      	expDebug::dPrintDBAPI('$unionTblQry', $unionTblQry);
	      	$unionTblRes = $unionTblQry->execute()->fetchAssoc();
	      	expDebug::dPrint('$unionTblRes = ' . print_r($unionTblRes, true));
	      	$union2TblRes = explode(',',$unionTblRes['table_sql']);
	      	$union1TblRes = explode(',',$tableNameResult['table_sql']);
	      	$tblcolumn_key = array_search($tableName,$union1TblRes);
	      	
	      	if(isset($tblcolumn_key)){
	      		$uniontblName = $union2TblRes[$tblcolumn_key];
      }
	      	
	      	if(count($criteriaSQLArray) > 1){
	      		$union2TblfldRes = explode('~~',$unionTblRes['column_sql']);
	      		$union1TblfldRes = explode('~~',$tableNameResult['column_sql']);
    } else {
	      		$union2TblfldRes = explode('~~',$unionTblRes['column_sql']);
	      		$union1TblfldRes = explode('~~',$tableNameResult['column_sql']);
	      	}
	      	
	      	
	      	$fldcolumn_key = array_search($criteriaSQL,$union1TblfldRes);
	      	expDebug::dPrint("Column key value".print_r($fldcolumn_key,1));
	      	if(isset($fldcolumn_key)){
	      		$fkeyName = explode('.',$union2TblfldRes[$fldcolumn_key]);
	      		$colName[2] = $fkeyName[1];
	      	}
	      	$inc = 2;
	      	$viewDesc[2] = db_query("SELECT VIEW_DEFINITION FROM INFORMATION_SCHEMA.VIEWS
       											WHERE TABLE_SCHEMA = '".$databases['default']['report']['database']."'
    												AND TABLE_NAME = '".$uniontblName."'")->fetchField();
	      	expDebug::dPrint("SWEGTWRhyt ae union2-->>".print_r($viewDesc,1));
	      }

	      for($i=1;$i<=$inc;$i++){
	      	expDebug::dPrint("DFSDFSDFSDRR".print_r($colName,1));
	      	$tblcon = '` AS `'.$colName[$i].'`';
	      	$orgTableName = explode(strtolower($tblcon), strtolower($viewDesc[$i]));

	      	$fieldName = explode(',',$orgTableName[0]);

	      	$tableAliasName = explode('.',$fieldName[count($fieldName) - 1]);

	      	 
	      	$sltTbl = ' '.str_replace('select ','',$tableAliasName[0]).' on';
	      	$otName = explode($sltTbl, $viewDesc[$i]);
	      	$fName = explode('.',$otName[0]);
	      // $oTbl = str_replace(str_replace('`','',$fName[count($fName) - 1]),'select ','');
	     //  $oTbl = str_replace('`','',$fName[count($fName) - 1]);
	     //  $oTblFld = str_replace('`','',$tableAliasName[1]);
	      	$oTbl = str_replace('`','',$fName[count($fName) - 1]);
	      	$oTblFld = str_replace('`','',$tableAliasName[1]);

	      	 if($tableName == 'slv_class_orders' || $tableName == 'slv_tp_orders' 
	      	 		|| $uniontblName == 'slv_class_orders' || $uniontblName == 'slv_tp_orders'){
	      	 	$criteriaQuery[] = "SELECT DISTINCT(a.".$oTblFld.") as acval FROM ".$oTblFld." a WHERE a.".$oTblFld;
	      	 	expDebug::dPrint(" Criteria Query to store in column".print_r($criteriaQuery,1));
	      	 }else{
	      	 	$criteriaQuery[] = "SELECT DISTINCT(b.".$oTblFld.") as acval FROM ".$entViewsArr[$oTbl]." a
	      		INNER JOIN ".$oTbl." b ON b.id = a.entity_id WHERE b.".$oTblFld;
	      	 	expDebug::dPrint(" Criteria Query to store in column".print_r($criteriaQuery,1));
	      	 }
	      	 
	      }
	      
	      expDebug::dPrint(" arry of tables".print_r($entViewsArr,1));
	      
	      
      }
    } else {
      $tableName = $tableSqlArray[0];
      $criteriaFieldName = $criteriaSQLArray[0];
    }

    expDebug::dPrint('$criteriaSQLArray : '.print_r($criteriaSQLArray, true) , 3);
    expDebug::dPrint('$tableNameResult : '.print_r($tableNameResult, true) , 3);
    expDebug::dPrint('$tableSqlArray : '.print_r($tableSqlArray, true) , 3);
    expDebug::dPrint('$tableAliasSqlArray : '.print_r($tableAliasSqlArray, true) , 3);
    expDebug::dPrint('$tableName : '.$tableName , 4);
    
    $columnType = '';
    if(!strstr($tableName, 'listitems')){
      $query = "SHOW COLUMNS IN ".$tableName." where Field = '".$criteriaFieldName."'";
      expDebug::dPrint('$query : '. $query , 4);
      
      $fieldResult = @db_query($query)->fetchAssoc();
      $columnType = $fieldResult['type'];
    }
    // End - Get field type
    expDebug::dPrintDBAPI(' $fieldResult : ', $query);
    expDebug::dPrint(' $fieldResult : '.print_r($fieldResult,true),3);
    expDebug::dPrint('$columnType : '.$columnType , 4);
    
    $userId = getSltpersonUserId();
      
    $fields = array();
    if($columnType == 'datetime' || $columnType == 'date'){
      $fields['name'] = trim($criteriaAliasSQL);
      $fields['condition_type'] = 'cre_rpt_cnd_grt';
    } else {
      $fields['name'] = trim($criteriaAliasSQL);
      $fields['condition_type'] = 'cre_rpt_cnd_con'; // Default contains search
    }
    $fields['table_name'] = trim($tableName);
/** #70379:: Added Stripslashes**/    
    $fields['column_name'] = stripslashes(trim($criteriaSQL));
    $fields['column_alias_name'] = trim($criteriaAliasSQL);
    $fields['column_type'] = $columnType;
//     $fields['criteria_query'] = trim($criteriaQuery[0]);
//     if(!empty($criteriaQuery[1]))
//     	$fields['criteria_union_query'] = trim($criteriaQuery[1]);
    //changes done following needs to be cleaned up
    $criteria_autocomplete = getReportCriteriaQueryFromViews($tableName, $criteriaSQL);
    expDebug::dPrint('$ $criteria_autocomplete = ' . print_r($criteria_autocomplete, true), 4);
    if(is_object($criteria_autocomplete)===FALSE && ($columnType != 'datetime' && $columnType != 'date')){
    	//TODO to add the autocomplete query for expressions and functions
    	if(count($criteriaSQLArray)<=2 || count($tableSqlArray)==1){
    		expDebug::dPrint('$ $criteria_autocomplete = ' . print_r($criteria_autocomplete, true), 4);
    		$column_query_builder_sql = "SELECT ".rawurldecode($criteriaSQL)." as '".$criteriaAliasSQL."'";
    		expDebug::dPrint('$ Field Select Query = ' . print_r($column_query_builder_sql, true), 4);
    		require_once(drupal_get_path('module', 'exp_sp_lnrreports') . '/php_sql_parser/src/PHPSQLParser.php');
    		$columnParser = new PHPSQLParser($column_query_builder_sql, true);
    		expDebug::dPrint('$ $parser->parsed = ' . print_r($columnParser->parsed, true), 4);
    		foreach ($columnParser->parsed['SELECT'] as $k => $v){
    			if(is_array($v['alias'])!==FALSE){
    				$alias = $v['alias']['no_quotes'];
    			}
    			elseif($v['sub_tree'][1]['expr_type']=='const'){
    				$alias = $v['sub_tree'][1]['base_expr'];
    			}
    			$alias = str_replace(array('\'', '"'), '', $alias);
    			expDebug::dPrint('$ $alias::$criteriaAliasSQL = '. print_r($alias, true) .'::'. print_r($criteriaAliasSQL, true), 4);
    			if($alias==$criteriaAliasSQL){
    				$next = false;
    				array_walk($v,'arrayCheckRef','colref');
    				expDebug::dPrint('$ $columnParser->parsed Column Array = ' . print_r($finalArray, true), 4);
    			}    			
    				unset($columnParser->parsed['SELECT'][$k]);
    		}
    		$finalArray = array_unique($finalArray);
    		if(count($finalArray)==1){
    			$columnKey = $finalArray[0];
    			$columnTokens = explode(".", $columnKey);
    			if(count($columnTokens)>1){
    				if(in_array($columnTokens[0], $tableAliasSqlArray)){
    					$aliasIndex=array_search($columnTokens[0],$tableAliasSqlArray);
    					$tableName=$tableSqlArray[$aliasIndex];
    					$criteria_autocomplete = getReportCriteriaQueryFromViews($tableName, $columnTokens[1]);
    				}
    			}elseif (count($columnTokens)==1){
    				$criteria_autocomplete = getReportCriteriaQueryFromViews($tableName, $columnKey);
    				$columnTokens[1]=$columnTokens[0];
    			}
    			$pattern = "/^(SELECT|select) (DISTINCT|distinct)?\s?([\(`a-zA-Z0-9._\)]*)\s?(as|AS)?\s?(acval|ACVAL)?/";
    			if(!empty($criteria_autocomplete->criteria_query)){
	    			preg_match($pattern,$criteria_autocomplete->criteria_query,$matches);
	    			expDebug::dPrint('$matching column name in autocomplete query = ' . print_r($matches, true), 4);
	    			$newCriteriaSQL = preg_replace("/$columnKey/", $matches[3], rawurldecode($criteriaSQL));
	    			expDebug::dPrint('$replacement Pattern = ' . print_r($newCriteriaSQL, true), 4);
	    			$replacePattern = $newCriteriaSQL;
	    			$tempQuery = preg_replace($matches[3], $replacePattern, $criteria_autocomplete->criteria_query);
	    			expDebug::dPrint('$new Criteria SQL = ' . print_r($tempQuery, true), 4);
	    			$criteria_autocomplete->criteria_query = $tempQuery;
    			}    			
    			if(!empty($criteria_autocomplete->criteria_union_query)){
	    			preg_match($pattern,$criteria_autocomplete->criteria_union_query,$matchesUnion);
	    			expDebug::dPrint('$matching column name in autocomplete union query = ' . print_r($matchesUnion, true), 4);
	    			$newCriteriaUnionSQL = preg_replace("/$columnKey/", $matchesUnion[3], rawurldecode($criteriaSQL));
	    			expDebug::dPrint('$replacement Pattern = ' . print_r($newCriteriaUnionSQL, true), 4);
	    			$replacePattern = $newCriteriaUnionSQL;
	    			$tempUnionQuery = preg_replace($matches[3], $replacePattern, $criteria_autocomplete->criteria_union_query);
	    			expDebug::dPrint('$new Criteria Union SQL = ' . print_r($tempUnionQuery, true), 4);
	    			$criteria_autocomplete->criteria_union_query = $tempUnionQuery;
    			} 
    			$columnType = '';
    			if(!strstr($tableName, 'listitems')){
    				$query = "SHOW COLUMNS IN ".$tableName." where Field = '".$columnTokens[1]."'";
    				expDebug::dPrint('$query : '. $query , 4);
    			
    				$fieldResult = @db_query($query)->fetchAssoc();
    				$columnType = $fieldResult['type'];
    			}
    			// End - Get field type
    			expDebug::dPrintDBAPI(' $fieldResult : ', $query);
    			expDebug::dPrint(' $fieldResult : '.print_r($fieldResult,true),3);
    			expDebug::dPrint('$columnType : '.$columnType , 4);
    			if($columnType == 'datetime' || $columnType == 'date'){
    				$fields['name'] = trim($criteriaAliasSQL);
    				$fields['condition_type'] = 'cre_rpt_cnd_grt';
    			} else {
    				$fields['name'] = trim($criteriaAliasSQL);
    				$fields['condition_type'] = 'cre_rpt_cnd_con'; // Default contains search
    			}
    			$fields['table_name'] = trim($tableName);
    			$fields['column_type'] = $columnType;
    		}//Single column expression or function
    	}    	
    }
    $fields['criteria_query'] = $criteria_autocomplete->criteria_query;
    $fields['criteria_union_query'] = $criteria_autocomplete->criteria_union_query;
    $fields['report_details_id'] = $reportId;
    $fields['created_by'] = $userId;
    $fields['created_on'] = now();
    $fields['updated_by'] = $userId;
    $fields['updated_on'] = now();
    
    $insert = db_insert('slt_report_criteria'); // shobanan
    $insert->fields($fields);
    expDebug::dPrintDBAPI('insert criteria ', $insert);
    $insert->execute();
    
    if($columnType == 'datetime' || $columnType == 'date'){
      $fields['name'] = trim($criteriaAliasSQL);
      $fields['condition_type'] = 'cre_rpt_cnd_les';
      $insert = db_insert('slt_report_criteria');
      $insert->fields($fields);
      $insert->execute();
    }
    
    print drupal_json_encode( array('result' => 'updated', 'criteria_details' => loadAddedReportCriteria($reportId)) );
  
  } catch (Exception $ex) {
    
    watchdog_exception('addReportCriteria', $ex);
    expertusErrorThrow($ex);
    
  }
  
}


/*
 * deleteReportCriteria() 
 */
function deleteReportCriteria($reportId, $criteriaId, $columnType, $coulmnAliasName){
  try{
    
    $delete = db_delete('slt_report_criteria');
    $delete->condition('report_details_id', $reportId);
    
    if($columnType != 'datetime'){
    	$delete->condition('id', $criteriaId);
    }else{
    	$delete->condition('column_alias_name', $coulmnAliasName);
    }
    $delete->execute();
    
    //Added for #0070534
    $pcheck = resetReportValidate($reportId);
    expDebug::dPrint("Filter validate -- ".print_r($pcheck,true),3);
  	if($pcheck == 'NON_SCALABLE_QUERY'){
  		$status = db_query('SELECT status FROM slt_report_details WHERE id = :repId ',array(':repId' => $reportId))->fetchField();
    	print drupal_json_encode( array('result' => 'NON_SCALABLE_QUERY', 'status' => $status, 'criteria_details' => loadAddedReportCriteria($reportId)) );
    }else{
    	print drupal_json_encode( array('result' => 'updated', 'criteria_details' => loadAddedReportCriteria($reportId)) );
    }
    
    
  } catch (Exception $ex) {
    
    watchdog_exception('deleteReportCriteria', $ex);
    expertusErrorThrow($ex);
    
  }
  
}

/*
 * UpdateReportCriteriaCondition() 
 */
function UpdateReportCriteriaCondition($reportId, $criteriaId, $criteriaCondition){
  try{
    
    $fields['condition_type'] = $criteriaCondition;
    
    $update = db_update('slt_report_criteria');
    $update->fields($fields);
    $update->condition('report_details_id', $reportId);
    $update->condition('id', $criteriaId);
    $update->execute();
    
    print drupal_json_encode( array('result' => 'updated', 'criteria_details' => loadAddedReportCriteria($reportId)) );
    
  } catch (Exception $ex) {
    
    watchdog_exception('UpdateReportCriteriaCondition', $ex);
    expertusErrorThrow($ex);
    
  }
  
}



/*
 * addDeleteMandatory() 
 */
function addDeleteMandatory($reportId, $criteriaId, $type){
  
  try{
    
    $userId = getSltpersonUserId();
    
    $fields['updated_by'] = $userId;
    $fields['updated_on'] = now();
    $fields['mandatory'] = ($type == 'add') ? 1 : 0;
    
    $criteriaIdArray = explode(',',$criteriaId);
    
    $update = db_update('slt_report_criteria');
    $update->fields($fields);
    $update->condition('id', $criteriaIdArray, 'IN');
    $update->execute();  
    
    //Added for #0070534
  	$pcheck = resetReportValidate($reportId);
    expDebug::dPrint("Filter validate -- ".print_r($pcheck,true),3);
  	if($pcheck == 'NON_SCALABLE_QUERY'){
  		$status = db_query('SELECT status FROM slt_report_details WHERE id = :repId ',array(':repId' => $reportId))->fetchField();
    	print drupal_json_encode( array('result' => 'NON_SCALABLE_QUERY', 'status' => $status, 'criteria_details' => loadAddedReportCriteria($reportId)) );
    }else{
    	print drupal_json_encode( array('result' => 'updated', 'criteria_details' => loadAddedReportCriteria($reportId)) );
    }
  } catch (Exception $ex) {
    
    watchdog_exception('addDeleteMandatory', $ex);
    expertusErrorThrow($ex);
    
  }
}

function updateCriteriaModifiers($reportId, $criteriaIdList, $isDate, $conditionType, $mandatory, $userfilter){
	try{
	
		$userId = getSltpersonUserId();
		$criteriaId = explode("|",$criteriaIdList);
		$fields['updated_by'] = $userId;
		$fields['updated_on'] = now();
		$fields['use_user_details'] = $userfilter;
		$fields['mandatory'] = $mandatory;
		if($isDate != 1){
		  $fields['condition_type'] = empty($conditionType) ? null :$conditionType;
		}
		$update = db_update('slt_report_criteria');
		$update->fields($fields);
		$update->condition('report_details_id', $reportId, '=');
		if($isDate == 1){
		    $update->condition('id', array($criteriaId[0],$criteriaId[1]), 'IN');
		}else{
			$update->condition('id', $criteriaId[0], '=');
		}
		expDebug::dPrintDBAPI(' $select obj = ' , $update);
		$update->execute();
		
		//Added for #0070534
		$pcheck = resetReportValidate($reportId);
    expDebug::dPrint("Filter validate -- ".print_r($pcheck,true),3);
  	if($pcheck == 'NON_SCALABLE_QUERY'){
  		$status = db_query('SELECT status FROM slt_report_details WHERE id = :repId ',array(':repId' => $reportId))->fetchField();
    	print drupal_json_encode( array('result' => 'NON_SCALABLE_QUERY', 'status' => $status, 'criteria_id' => $criteriaId[0]) );
    }else{
    	print drupal_json_encode( array('result' => 'updated', 'criteria_id' => $criteriaId[0]) );
    }
	
	} catch (Exception $ex) {
		watchdog_exception('updateCriteriaModifiers', $ex);
		expertusErrorThrow($ex);
	
	}	
}

/*
 * loadQueryBuilderContent()
 * 
 * To get the query content for the given report id
 */
function loadQueryBuilderContent($reportId){
  try{
  	// Fetch query builder content details
    $select = db_select('slt_report_query_builder', 'query');
    $select->leftjoin('slt_report_details', 'report', 'report.id = query.report_details_id');
    $select->addField('report', 'title');
    $select->addField('report', 'report_builder_type');
    $select->addField('query', 'query_builder_sql', 'report_query');
    $select->addField('query', 'query_builder_org_sql', 'report_org_query');
    $select->addField('query', 'table_sql');
    $select->addField('query', 'table_alias_sql');
    $select->addField('query', 'column_sql');
    $select->addField('query', 'column_alias_sql');
    $select->addField('query', 'where_sql');
    $select->addField('query', 'window_positions');
    $select->condition('query.report_details_id', $reportId);
    $select->condition('query.union_query', '2', '!=');
    expDebug::dPrintDBAPI('$select', $select);
    $result = $select->execute()->fetchAssoc();
    expDebug::dPrint('$result' . print_r($result, true));
    
    // Check whether there are any active schedules for the report
    $scheduleSelect = db_select('slt_report_schedules', 'rep_sch');
    $scheduleSelect->addExpression('COUNT(rep_sch.id)', 'count');
    $scheduleSelect->condition('rep_sch.report_id', $reportId, '=');
    $scheduleSelect->condition('rep_sch.status', 'cre_rpt_rss_atv', '=');
    expDebug::dPrintDBAPI('$scheduleSelect', $scheduleSelect);
    $hasActiveSchedule = $scheduleSelect->execute()->fetchField();
    expDebug::dPrint('$hasActiveSchedule' . $hasActiveSchedule);
    $activeScheduleMsg = array();
    if ($hasActiveSchedule) {
    	$activeScheduleMsg[] = t('ERR233');//There are active schedules setup for this report. Your change may interrupt the service
    }
    
    $returnArr = array('query_data' => $result['report_query'],
                       'query_org_data' => $result['report_org_query'],
                       'report_title' => $result['title'],
                       'report_details' => $result,
                       'has_active_schedule_msg' => $activeScheduleMsg);
    expDebug::dPrint('$returnArr = ' . print_r($returnArr, true), 5);
    
    print drupal_json_encode($returnArr);
  }
  catch (Exception $ex) {   
    watchdog_exception('loadQueryBuilderContent', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * loadReportDetails()
 * 
 * To get the report information from report details table,
 * for the given report id.
 */
 function loadReportDetails($reportId){
 	try{
    $select = db_select('slt_report_details', 'rep_details');
    $select->leftjoin('slt_report_themes', 'rep_theme','rep_theme.report_details_id = rep_details.id');
    $select->addField('rep_theme', 'theme_name');
    $select->addField('rep_details', 'title','report_title');
    $select->addField('rep_details', 'description','report_desc');
    $select->addField('rep_details', 'report_visibility','report_visibility');
    $select->addField('rep_details', 'report_type','report_type');
    $select->addField('rep_details', 'status', 'status');
    $select->condition('rep_details.id', $reportId);
    $resultReportDetails = $select->execute()->fetchAssoc();
    print drupal_json_encode($resultReportDetails); 
    } catch (Exception $ex) {
    	watchdog_exception('loadReportDetails', $ex);
    	expertusErrorThrow($ex);
    }
 }
 
 /*
 * loadReportListItems()
 * 
 * To get the report list items from profile list items table,
  */
 function loadReportListItems(){
   try{
    $select = db_select('slt_profile_list_items', 'profile');
    $select-> addField('profile', 'code','code');
    $select-> addField('profile', 'name','name');

    $select-> condition(db_or()->
      condition('profile.code', 'cre_rpt_rpc_%','LIKE')->
      //condition('profile.code', 'cre_rpt_vis_%','LIKE')->
      condition('profile.code', 'cre_rpt_cnd_%','LIKE')
    );     
      
    $resultReport = $select->execute()->fetchAll();
    
    $resultReportList = array();
    
    $count = 0;
    foreach($resultReport as $key => $value ){
      //$parentCode = substr($value->code, 0, -4);
      $parCode = splitReportCode($value->code,'_',3); 
      $resultReportList[$parCode[0]][$count]['code'] = $value->code;
      $resultReportList[$parCode[0]][$count]['name'] = $value->name;
      
      $count++;
    }
    /*added by ranjani - start */
    if(user_access('administrator') || is_data_admin()) {
    	$select = db_select('slt_groups', 'grp');
    	$select-> addField('grp', 'id','code');
    	$select-> addField('grp', 'name','name');
    	$select->condition('grp.status', 'cre_sec_sts_atv', '=');
    	$select->condition('grp.code', 'grp_sup', '!=');
    	//Added for 70085
    	$or = db_or();
    	$or->where('(FIND_IN_SET(\'View Report Perm\',grp.access_permissions) >= 1)');
    	$or->where('(FIND_IN_SET(\'Create Report Perm\',grp.access_permissions) >= 1)');
    	$or->where('(FIND_IN_SET(\'Assign Report Perm\',grp.access_permissions) >= 1)');
    	$select->condition($or);
        expDebug::dPrintDBAPI('select groups for report visbilty dropdown ', $select);
    	$resultRoles = $select->execute()->fetchAll();
    }
  	else {
	$roleArray = $GLOBALS['user']->roles;	
    $grpNameArr = array();
	foreach($roleArray as $rid => $rname){
		$grpNameArr[]= $rname;
	}
		$getGroupName = db_select('slt_groups', 'grp');
		$getGroupName->addField('grp', 'id', 'code');
		$getGroupName->addField('grp', 'name', 'name');
		$getGroupName->condition('grp.name', $grpNameArr, 'IN');
		$getGroupName->condition('grp.status', 'cre_sec_sts_atv', '=');
		$getGroupName->condition('grp.code', 'grp_sup', '!=');
		$or = db_or();
		$or->where('(FIND_IN_SET(\'View Report Perm\',grp.access_permissions) >= 1)');
		$or->where('(FIND_IN_SET(\'Create Report Perm\',grp.access_permissions) >= 1)');
		$or->where('(FIND_IN_SET(\'Assign Report Perm\',grp.access_permissions) >= 1)');
		$getGroupName->condition($or);
		$resultRoles = $getGroupName->execute()->fetchAll();
		expDebug::dPrintDBAPI('select groups for report visbilty dropdown ', $getGroupName);
	}
  	$count = 0;
    foreach($resultRoles as $key => $value ){
      $resultReportList['cre_rpt_vis'][$count]['code'] = $value->code;
      $resultReportList['cre_rpt_vis'][$count]['name'] = $value->name;
      
      $count++;
    }
    /*added by ranjani - end */
    
    sort($resultReportList['cre_rpt_rpc']);
    sort($resultReportList['cre_rpt_vis']);
    sort($resultReportList['cre_rpt_cnd']);
    
    print drupal_json_encode(array('category' => $resultReportList['cre_rpt_rpc'], 'visibility' => $resultReportList['cre_rpt_vis'], 'criteria_condition' => $resultReportList['cre_rpt_cnd'])); 
    } catch (Exception $ex) {
    	watchdog_exception('loadReportListItems', $ex);
    	expertusErrorThrow($ex);
    }
 }
 
function splitReportCode($valueCode,$udScore,$nth){
	try{
	$max = strlen($valueCode);
	$n = 0;
	for($i=0;$i<$max;$i++){
	    if($valueCode[$i]==$udScore){
	        $n++;
	        if($n>=$nth){
	            break;
	        }
	    }
	}
	$arr[] = substr($valueCode,0,$i);
	return $arr;
	} catch (Exception $ex) {
		watchdog_exception('splitReportCode', $ex);
		expertusErrorThrow($ex);
	}
}
 
function checkThemeEmpty($reportId){
	try{
  $select = db_select('slt_report_themes', 'theme');
  $select->addField('theme', 'theme_name' );
  $select->condition('theme.report_details_id', $reportId);
  $result = $select->execute()->fetchField();
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('checkThemeEmpty', $ex);
  	expertusErrorThrow($ex);
  }
}
  
/**
 * Checks for forbidden keywords in report query
 */
function forbiddenKeywordsInReportQuery($query) {
	try {
		$forbiddenKeywords = array('insert ', 'delete ', 'drop ', 'truncate ', 'update ');
    foreach ($forbiddenKeywords AS $keyword) {
      if (stristr($query, $keyword)) {
        return true; // has error, no need to check further
      }
    }
    return false; // no error
	}
	catch (Exception $ex) {
    watchdog_exception('forbiddenKeywordsInReportQuery', $ex);
    expertusErrorThrow($ex);		
	}
}

/**
 * Prepares group by sql clause from parsed array
 */
function prepareGroupBySqlClauseFromParsedArray($groupArray) {
	try {
	  $groupSql = '';
    if (count($groupArray) > 0) {
      $groupSqlArray = array();
      foreach ($groupArray as $group) {
        $groupSqlArray[] = $group['base_expr'];
      }
      $groupSql = ' GROUP BY ' . implode(', ', $groupSqlArray);
    }
    return $groupSql;
	}
  catch (Exception $ex) {
    watchdog_exception('prepareGroupBySqlClauseFromParsedArray', $ex);
    expertusErrorThrow($ex);    
  }
}

/**
 * Prepares order by sql clause from parsed array
 */
function prepareOrderBySqlClauseFromParsedArray($orderArray) {
  try {
    $orderSql = '';
    if (count($orderArray) > 0) {
      $orderSqlArray = array();
      foreach ($orderArray as $order) {
        $orderSqlArray[] = $order['base_expr'];  
      }
      $orderSql = ' ORDER BY '.implode(', ', $orderSqlArray);
    }
    return $orderSql;
  }
  catch (Exception $ex) {
    watchdog_exception('prepareOrderBySqlClauseFromParsedArray', $ex);
    expertusErrorThrow($ex);    
  }
}

/**
 * Prepares limit sql clause from parsed array
 */
function prepareLimitSqlClauseFromParsedArray($limitArray) {
  try {
    $limitSql = '';
    if (count($limitArray) > 0) {
      if (empty($limitArray['offset'])) {
        $limitSql = ' LIMIT ' . $limitArray['rowcount'];
      }
      else {
        $offset = $limitArray['offset'];
        $rowCount = $limitArray['rowcount']; 
        $limitSql = ' LIMIT ' . $offset . ',' . $rowCount;
      }
    }
        
    return $limitSql;
  }
  catch (Exception $ex) {
    watchdog_exception('prepareLimitSqlClauseFromParsedArray', $ex);
    expertusErrorThrow($ex);    
  }
}

/**
 * Validate parsed single query
 */
function isValidReportSingleQuery($compactSql, $groupSql, $orderSql, $limitSql, $wherePosition, &$errMsg) {
	try {
    $splitPosition = 0;
    $compactSqlLen = strlen($compactSql);
    if ($wherePosition == 0) {
      if (!empty($groupSql)) {
        $splitPosition = stripos($compactSql, $groupSql);
      }
      elseif (!empty($orderSql)) {
        $splitPosition = stripos($compactSql, $orderSql);
      }
      elseif (!empty($limitSql)) {
        $splitPosition = stripos($compactSql, $limitSql);
      }
      
      if (empty($splitPosition)) {
        $runSql = $compactSql . ' WHERE 1=2';
      }
      else {
        $runSql = substr($compactSql, 0, $splitPosition);
        $runSql .= ' WHERE 1=2 ';
        $runSql .= substr($compactSql, $splitPosition, $compactSqlLen); // suffix the part after the split position
      }
    }
    else { // query has a where clause
      $splitPosition = $wherePosition;
      $runSql = substr($compactSql, 0, $splitPosition - 1);
      $runSql .= ' 1=2 AND ';
      $runSql .= substr($compactSql, $splitPosition - 1, $compactSqlLen);  // suffix the part after the split position
    }   
    expDebug::dPrint('$runSql = ' . $runSql, 4);

    try {
      $result = db_query($runSql, array(), getReportDatabaseOption());
      //expDebug::dPrint('$result = ' . print_r($result->fetchAll(), true), 4);
    }
    catch (Exception $ex) {
      expDebug::dPrint('test run of report sql query threw an exception $ex = ' . print_r($ex, true), 4); 
      // Vincent modified on 11 Aug, 2015 for # 0052759 item 4
      $errMsg = t('ERR167'); //$ex->errorInfo[2];
      return false;
    }
    
    return true;
	}
  catch (Exception $ex) {
    watchdog_exception('isValidReportSingleQuery', $ex);
    expertusErrorThrow($ex);    
  }
}

/**
 * Generates various query builder values from parsed sql query
 */
function generateReportQueryBuilderValues($parsedQuery) {
	global $finalArray;
	try {
    expDebug::dPrint('$parsedQuery = ' . print_r($parsedQuery, true), 5);
    $columnArray = array();
    $columnAliasArray = array();
    $columnCriteriaArray = array();
    $columnCriteriaAliasArray = array();
    $tableNameArray = array();
    $tableNameAliasArray = array();
    $tableJoinInfoArray = array();
      
    // Table name parsing
    foreach ($parsedQuery['FROM'] as $from) {
      $parsedArray = parseSqlNameAlias('table', $from);
      $tableNameArray[] = $parsedArray[0];
      $tableNameAliasArray[] = $parsedArray[1];
      if(isset($parsedArray[3])){
      $tableJoinInfoArray[] = $parsedArray[3];
      }else{
      	$tableJoinInfoArray[] = $parsedArray['FROM'][0];      
      }
    }
    expDebug::dPrint('$tableNameArray = ' . print_r($tableNameArray, true), 4);
    expDebug::dPrint('$tableNameAliasArray = ' . print_r($tableNameAliasArray, true), 4);
    expDebug::dPrint('$tableJoinInfoArray = ' . print_r($tableJoinInfoArray, true), 4);

    // Column parsing
    foreach ($parsedQuery['SELECT'] as $select) {
      expDebug::dPrint('$select = ' . print_r($select, true), 4);
      expDebug::dPrint('count($tableNameAliasArray) =  ' . print_r(count($tableNameAliasArray), true), 4);
      // To handle * in field name
      if (strstr($select['base_expr'], '*')) {
        expDebug::dPrint('* in select base expression', 4);
        if (trim($select['base_expr']) == '*'  && count($tableNameAliasArray)==1) {
          expDebug::dPrint('select base expression is *', 4);
          $tableColumnQuery = db_query('SHOW COLUMNS FROM ' . $tableNameArray[0]);
          $tableColumnResult = $tableColumnQuery->fetchAll();
          foreach ($tableColumnResult as $tableColumn) {
            $columnArray[] = $tableColumn->field;
            $columnAliasArray[] = $tableColumn->field;
            $columnCriteriaArray[] = $tableColumn->field;
            $columnCriteriaAliasArray[] = $tableColumn->field;
          }           
        }
        //45880: PS - Print option does not export all the columns in the report.
        elseif (strstr($select['base_expr'], '*') && count($tableNameAliasArray)>1) {
        	foreach($tableNameAliasArray as $alias){
        		$columnTableName = $alias;
        		$columnFieldName = '*';
	        	expDebug::dPrint('$columnTableNamealias = ' . print_r($columnTableName, true), 4);
	        	expDebug::dPrint('$columnTableNamealias = ' . print_r($columnFieldName, true), 4);
	        	$orgColumnTableIndex = array_search($columnTableName, $tableNameAliasArray);
	        	expDebug::dPrint('$orgColumnTableIndex = ' . $orgColumnTableIndex, 4);
	        	$orgColumnTableName = $tableNameArray[$orgColumnTableIndex];
	        	expDebug::dPrint('$orgColumnTableName = ' . print_r($orgColumnTableName, true), 4);
	        	
	        	$tableColumnQuery = db_query('SHOW COLUMNS FROM ' . $orgColumnTableName);
	        	$tableColumnResult = $tableColumnQuery->fetchAll();
	        	foreach ($tableColumnResult as $tableColumn) {
	        		$columnArray[] = $tableColumn->field;
	        		$columnAliasArray[] = $tableColumn->field;
	        		$columnCriteriaArray[] = $tableColumn->field;
	        		$columnCriteriaAliasArray[] = $tableColumn->field;
	        	}
        	}
        }
        else { // To handle * with table name or alias name
          expDebug::dPrint('Base expression is table.* or tablealias.*', 4);
          list($columnTableName, $columnFieldName) = explode('.', trim($select['base_expr']));
          expDebug::dPrint('$columnTableName = ' . print_r($columnTableName, true), 4);
          expDebug::dPrint('$columnFieldName = ' . print_r($columnFieldName, true), 4);
          $orgColumnTableIndex = array_search($columnTableName, $tableNameAliasArray);
          expDebug::dPrint('$orgColumnTableIndex = ' . $orgColumnTableIndex, 4);
          $orgColumnTableName = $tableNameArray[$orgColumnTableIndex];
          expDebug::dPrint('$orgColumnTableName = ' . print_r($orgColumnTableName, true), 4);
            
          $tableColumnQuery = db_query('SHOW COLUMNS FROM ' . $orgColumnTableName);
          $tableColumnResult = $tableColumnQuery->fetchAll();
          foreach ($tableColumnResult as $tableColumn) {
            $columnArray[] = $tableColumn->field;
            $columnAliasArray[] = $tableColumn->field;
            $columnCriteriaArray[] = $tableColumn->field;
            $columnCriteriaAliasArray[] = $tableColumn->field;  
          }
        }
      }
      else {
        $parsedArray = parseSqlNameAlias('column', $select);
        expDebug::dPrint('$parsedArray = ' . print_r($parsedArray, true), 4);
        $columnArray[] = $parsedArray[0];
        $columnAliasArray[] = $parsedArray[1];
// && $parsedArray[2] != 'function'     
//0074364: Date fields not showing up in report filters   
        if (stripos(strtolower($parsedArray[0]), 'select')===FALSE && stripos(strtolower($parsedArray[0]), 'group_concat')===FALSE && $parsedArray[2] != 'const' && $parsedArray[2] != 'multi_column_expression') {
        	if($parsedArray[2] == 'aggregate_function'){
        		$exceptionFunctions = array('min','max');
        		$funcCount =0;
        		foreach ($exceptionFunctions as $exceptionVal){
        			$keyMatch = $exceptionVal."%28";
        			expDebug::dPrint('inside aggregate function = ' . print_r(stripos(strtolower($parsedArray[0]), $keyMatch), true), 4);
        			if(stripos(strtolower($parsedArray[0]), $keyMatch)!==FALSE){
        				//TODO #74364
        				$funcCount++;
        				if($funcCount>1)break;
        				expDebug::dPrint('inside aggregate function = initializing parser', 4);
        				require_once(drupal_get_path('module', 'exp_sp_lnrreports') . '/php_sql_parser/src/PHPSQLParser.php');
        				$fieldToModify = "SELECT ".urldecode($parsedArray[0]);
        				$parsedField = new PHPSQLParser($fieldToModify, true);
        				expDebug::dPrint('$ $parser->$parsedField = ' . print_r($parsedField->parsed['SELECT'], true), 4);
        				$value = $parsedField->parsed['SELECT'];
        				$finalArray=array();
        				array_walk($value,'arrayCheckRef','colref');
        				 
        				 
        			}
        		}
        		// To ensure min and max function are used in single column expression
        		if(count($finalArray)==1 && $funcCount==1){
        			expDebug::dPrint('$ $parser->$parsedField  $$finalArray after array walk search for colref = ' . print_r($finalArray, true), 4);
        			$columnCriteriaArray[] = $finalArray[0];
        			$columnCriteriaAliasArray[] = $parsedArray[1];
        		}
        	}else{
        		$columnCriteriaArray[] = $parsedArray[0];
        		$columnCriteriaAliasArray[] = $parsedArray[1];
        	}
        	 
        }
        
      }
    }
    expDebug::dPrint('$columnArray = ' . print_r($columnArray, true), 4);
    expDebug::dPrint('$columnAliasArray = ' . print_r($columnAliasArray, true), 4);
    expDebug::dPrint('$columnCriteriaArray = ' . print_r($columnCriteriaArray, true), 4);
    expDebug::dPrint('$columnCriteriaAliasArray = ' . print_r($columnCriteriaAliasArray, true), 4);

    $tableSql = implode(',', $tableNameArray);
    $tableAliasSql = implode(',', $tableNameAliasArray); 
    $tableJoinSql = implode(',', $tableJoinInfoArray);
    $columnSql = implode('~~', $columnArray);
    $columnAliasSql = implode(',', $columnAliasArray);
    $columnCriteriaSql = implode(',', $columnCriteriaArray);
    $columnCriteriaAliasSql = implode(',', $columnCriteriaAliasArray);
    expDebug::dPrint('$tableSql = ' . $tableSql , 4);
    expDebug::dPrint('$tableAliasSql = ' . $tableAliasSql , 4);
    expDebug::dPrint('$tableJoinSql = ' . $tableJoinSql , 4);
    expDebug::dPrint('$columnSql = ' . $columnSql , 4);
    expDebug::dPrint('$columnAliasSql = ' . $columnAliasSql , 4);
    expDebug::dPrint('$columnCriteriaSql = ' . $columnCriteriaSql , 4);
    expDebug::dPrint('$columnCriteriaAliasSql = ' . $columnCriteriaAliasSql , 4);
    
		return array($tableSql, $tableAliasSql, $tableJoinSql, $columnSql, $columnAliasSql, $columnCriteriaSql, $columnCriteriaAliasSql);
	}
  catch (Exception $ex) {
    watchdog_exception('generateReportQueryBuilderValues', $ex);
    expertusErrorThrow($ex);    
  }
}

/**
 * Saves a validated report query in DB
 */
function saveParsedReportQueryInDB(&$reportId, $originalSql, $parsedQuery, $compactSql, $groupSql, $orderSql, $limitSql, $wherePosition) {
  try {
  	list($table_sql, $table_alias_sql, $table_join_sql, $column_sql, $column_alias_sql, $columns_criteria_sql, $alias_criteria_sql)
  	  = generateReportQueryBuilderValues($parsedQuery);
  	
    $userId = getSltpersonUserId();
    //If edit report
    if (!empty($reportId)) { // edit exisiting report
      $updateSqlStmt = db_update('slt_report_query_builder');
      $fields = array(
        'column_sql' => $column_sql,
        'column_alias_sql' => $column_alias_sql, 
        'column_criteria_sql' => $columns_criteria_sql,
        'column_criteria_alias_sql' => $alias_criteria_sql,
        'table_sql' => $table_sql,
        'table_alias_sql' => $table_alias_sql,
        'table_join_sql' => $table_join_sql,
        'where_sql' => '',
        'where_position' => $wherePosition,
        'group_sql' => $groupSql,
        'order_sql' => $orderSql,
        'limit_sql' => $limitSql,
        'union_query' => 0,
        'query_builder_sql'  =>  $compactSql,
        'query_builder_org_sql'  =>  $originalSql,
        'updated_by' => $userId,
        'updated_on' => now()
      );              
      $updateSqlStmt->condition('report_details_id', $reportId);
      $updateSqlStmt->fields($fields);
      expDebug::dPrintDBAPI('$updateSqlStmt', $updateSqlStmt);
      $updateSqlStmt->execute();
    }
    else {
      $transaction = db_transaction();
      try {
        $insertStmt = db_insert('slt_report_details');
        $fields = array(
          'status' => 'cre_rpt_rps_dft',
          'report_builder_type' => 'query_builder',
          'description' => '',
          'created_by' => $userId,
          'created_on' => now(),
          'updated_by' => $userId,
          'updated_on' => now(),
        );
        $insertStmt->fields($fields);
        expDebug::dPrintDBAPI('$insertStmt', $insertStmt);
        $reportId = $insertStmt->execute();
        expDebug::dPrint('$reportId = ' . $reportId);
          
        $insertSqlStmt = db_insert('slt_report_query_builder');
        $fields = array(
          'report_details_id' => $reportId,
          'column_sql' => $column_sql,
          'column_alias_sql' => $column_alias_sql, 
          'column_criteria_sql' => $columns_criteria_sql,
          'column_criteria_alias_sql' => $alias_criteria_sql,
          'table_sql' => $table_sql,
          'table_alias_sql' => $table_alias_sql,
        	'table_join_sql' => $table_join_sql,
          'where_sql' => '',
          'where_position' => $wherePosition,
          'group_sql' => $groupSql,
          'order_sql' => $orderSql,
          'limit_sql' => $limitSql,
          'union_query' => 0,
          'query_builder_sql' => $compactSql,
          'query_builder_org_sql' => $originalSql,
          'created_by' => $userId,
          'created_on' => now(),
          'updated_by' => $userId,
          'updated_on' => now(),
        );              
        $insertSqlStmt->fields($fields);
        expDebug::dPrintDBAPI('$insertSqlStmt', $insertSqlStmt);
        $reportQueryBuilderId = $insertSqlStmt->execute();
        expDebug::dPrint('$reportQueryBuilderId = ' . $reportQueryBuilderId);
      }
      catch (Exception $e) {
        $transaction->rollback();
        unset($transaction);
        watchdog_exception('saveParsedReportQueryInDB_trxn', $e);
        expertusErrorThrow($e);   
      }
      unset($transaction);
    }
	}
  catch (Exception $ex) {
    watchdog_exception('saveParsedReportQueryInDB', $ex);
    expertusErrorThrow($ex);    
  }
}

/**
 * Validates and saves single query
 */
function validateAndSaveParsedReportsSingleQuery(&$reportId, $originalSql, $parsedQuery, $compactSql, &$errMsg) {
	try {
	  // Rebuild group by, order by and limit sql clauses from the parsed query
    $groupSql = prepareGroupBySqlClauseFromParsedArray($parsedQuery['GROUP']);
    $orderSql = prepareOrderBySqlClauseFromParsedArray($parsedQuery['ORDER']); //@TODO: currently does not consider order preference ASC or DESC?
    $limitSql = prepareLimitSqlClauseFromParsedArray($parsedQuery['LIMIT']);
    
	  $wherePosition = 0;
    if (!empty($parsedQuery['WHERE'])) {
      $wherePosition = $parsedQuery['WHERE'][0]['position'];
    }
    
    if (!isValidReportSingleQuery($compactSql, $groupSql, $orderSql, $limitSql, $wherePosition, $errMsg)) {
    	return false;
    }
    
	  // Is a valid query which can be saved in db
    saveParsedReportQueryInDB($reportId, $originalSql, $parsedQuery, $compactSql, $groupSql, $orderSql, $limitSql, $wherePosition);
    
    return true;
	}
  catch (Exception $ex) {
    watchdog_exception('validateAndSaveParsedReportsSingleQuery', $ex);
    expertusErrorThrow($ex);    
  }
}

/**
 * Validates union query
 */
function isValidReportUnionQuery($compactUnionSql, $parsedUnionQuery, &$errMsg) {
	try {
    // Replace each UNION, UNION ALL or UNION DISTINCT keyword with LIMIT 0 followed by UNION clause

    $unionValidationQuery = '(' . preg_replace("/ ?UNION (ALL |DISTINCT )?/i", " LIMIT 0 ) $0 ", $compactUnionSql);
    $unionValidationQuery = preg_replace("/ ?UNION (ALL |DISTINCT )?/i", "$0 ( ", $unionValidationQuery);
    $unionValidationQuery = preg_replace("/$/", " LIMIT 0 ) $0", $unionValidationQuery, 1);    
    expDebug::dPrint('$unionValidationQuery = ' . $unionValidationQuery, 4);
    try {
      $result = db_query($unionValidationQuery, array(), getReportDatabaseOption());
      expDebug::dPrint('$result = ' . print_r($result->rowCount(), true), 4);
    }
    catch (Exception $ex) {
      expDebug::dPrint('test run of report sql union query threw an exception $ex = ' . print_r($ex, true), 4); 
      // Vincent modified on 11 Aug, 2015 for # 0052759 item 4
			$errMsg = t('ERR167'); //$ex->errorInfo[2];
      return false;
    }
    
    $firstTime = true;
    foreach ($parsedUnionQuery as $unionKeyword => $unionedQueries) {
      expDebug::dPrint('$unionKeyword = ' . $unionKeyword, 4);
      expDebug::dPrint('$unionedQueries = ' . print_r($unionedQueries, true), 4);//    
      foreach ($unionedQueries as $selectQuery) {
      	$i = 0;
      	expDebug::dPrint('$selectQuery = ' . print_r($selectQuery, true), 4);
      	expDebug::dPrint('table Of the Union Query = ' . print_r($selectQuery['FROM'][0][table], true), 4);
      	foreach ($selectQuery['SELECT'] as $select) {
      		expDebug::dPrint('$select = ' . print_r($select, true), 4);
      		// To handle * in field name
      		if (strstr($select['base_expr'], '*')) {
      			expDebug::dPrint('* in select base expression', 4);
      			if (trim($select['base_expr']) == '*') {
      				expDebug::dPrint('select base expression is *', 4);
      				// $tableColumnQuery = db_query('SHOW COLUMNS FROM ' . $tableNameArray[0]); // Table is Not Given in this #0041727
      				$tableColumnQuery = db_query('SHOW COLUMNS FROM ' . $selectQuery['FROM'][0][table]); 
      				$tableColumnResult = $tableColumnQuery->fetchAll();
      				
      				foreach ($tableColumnResult as $tableColumn) {
      					expDebug::dPrint('$tableColumn = ' . print_r($tableColumn, true), 4);
      					if ($firstTime) {
      						$columnAliasArray[] = $tableColumn->field;
      					}
      					else {
      						if ($tableColumn->field != $columnAliasArray[$i]) {
      							expDebug::dPrint("Different column aliases in the unioned select statements1 : $tableColumn->field <> $columnAliasArray[$i]", 4);
      							$errMsg = t('Different column aliases in the unioned select statements.');
      							return false;
      						}
      					}
      					$i++;
      				} // foreach
      			}
      			else { // To handle * with table name or alias name
      				expDebug::dPrint('Base expression is table.* or tablealias.*', 4);
      				list($columnTableName, $columnFieldName) = explode('.', trim($select['base_expr']));
      				expDebug::dPrint('$columnTableName = ' . print_r($columnTableName, true), 4);
      				expDebug::dPrint('$columnFieldName = ' . print_r($columnFieldName, true), 4);
      				/* $orgColumnTableIndex = array_search($columnTableName, $tableNameAliasArray);
      				expDebug::dPrint('$orgColumnTableIndex = ' . $orgColumnTableIndex, 4);
      				$orgColumnTableName = $tableNameArray[$orgColumnTableIndex];
      				expDebug::dPrint('$orgColumnTableName = ' . print_r($orgColumnTableName, true), 4);
      				$tableColumnQuery = db_query('SHOW COLUMNS FROM ' . $orgColumnTableName); */ // Table is Not Given in this #0041727
      				
      				$tableColumnQuery = db_query('SHOW COLUMNS FROM ' . $selectQuery['FROM'][0][table]);
      				$tableColumnResult = $tableColumnQuery->fetchAll();
      				foreach ($tableColumnResult as $tableColumn) {
      					expDebug::dPrint('$tableColumn = ' . print_r($tableColumn, true), 4);
      					if ($firstTime) {
      						$columnAliasArray[] = $tableColumn->field;
      					}
      					else {
      						if ($tableColumn->field != $columnAliasArray[$i]) {
      							expDebug::dPrint("Different column aliases in the unioned select statements2 : $tableColumn->field <> $columnAliasArray[$i]", 4);
      							$errMsg = t('Different column aliases in the unioned select statements.');
      							return false;
      						}
      					}
      					$i++;
      				} // foreach
      			}
      		}
      		else {
      			$parsedArray = parseSqlNameAlias('column', $select);
      			expDebug::dPrint('$parsedArray = ' . print_r($parsedArray, true), 4);
      			if ($firstTime) {
      				$columnAliasArray[] = $parsedArray[1];
      			}
      			else {
      				if ($parsedArray[1] != $columnAliasArray[$i]) {
      					expDebug::dPrint("Different column aliases in the unioned select statements3 : $parsedArray[1] <> $columnAliasArray[$i]", 4);
      					$errMsg = t('Different column aliases in the unioned select statements.');
      					return false;
      				}
      			}
      			$i++;
      		}
      	}
        if ($firstTime) {
          $firstTime = false;
          expDebug::dPrint('first time $i = ' . $i, 4);
          expDebug::dPrint('first time count($columnAliasArray) = ' . count($columnAliasArray), 4);
          
        }
        else {
          expDebug::dPrint('for validation $i = ' . $i, 4);
          expDebug::dPrint('for validation count($columnAliasArray) = ' . count($columnAliasArray), 4);
        	if ($i != count($columnAliasArray)) {
	          expDebug::dPrint('Different column count in the unioned select statements.', 4); 
	          $errMsg = t('Different column count in the unioned select statements.');
	          return false;
        	}
        }
      }
    }
    
    return true;
	}
	catch (Exception $ex) {
    watchdog_exception('isValidReportUnionQuery', $ex);
    expertusErrorThrow($ex);
	}
}

/**
 * Saves a new union query in DB
 */
function saveNewUnionQueryInDB(&$reportId, $originalSql, $parsedQuery, $compactSql) {
  try {
    $userId = getSltpersonUserId();
    $trxn = db_transaction();
    try {
    	// Create new slt_report_details record and save report id in $reportId
      $insertStmt = db_insert('slt_report_details');
      $fields = array(
        'status' => 'cre_rpt_rps_dft',
        'report_builder_type' => 'query_builder',
        'description' => '',
        'created_by' => $userId,
        'created_on' => now(),
        'updated_by' => $userId,
        'updated_on' => now(),
      );
      $insertStmt->fields($fields);
      expDebug::dPrintDBAPI('$insertStmt', $insertStmt);
      $reportId = $insertStmt->execute();
      expDebug::dPrint('$reportId = ' . $reportId);
        
      // Loop through all unioned select queries and save in db (@TODO: save in reverse order?)
      $first = true;
      foreach ($parsedQuery as $unionKeyword => $unionedQueries) {
      	expDebug::dPrint('$unionKeyword = ' . $unionKeyword, 4);
      	expDebug::dPrint('$unionedQueries = ' . print_r($unionedQueries, true), 4);
	      foreach ($unionedQueries as $selectQuery) {
	      	expDebug::dPrint('$selectQuery = ' . print_r($selectQuery, true), 4);
	        list($tableSql, $tableAliasSql, $tableJoinSql, $columnSql, $columnAliasSql, $columnCriteriaSql, $columnCriteriaAliasSql)
	          = generateReportQueryBuilderValues($selectQuery);
	        $insertSqlStmt = db_insert('slt_report_query_builder');
	        $fields = array(
	          'report_details_id' => $reportId,
	          'column_sql' => $columnSql,
	          'column_alias_sql' => $first? $columnAliasSql : null, 
	          'column_criteria_sql' => $first? $columnCriteriaSql : null,
	          'column_criteria_alias_sql' => $first? $columnCriteriaAliasSql : null,
	          'table_sql' => $tableSql,
	          'table_alias_sql' => $tableAliasSql, 
	        	'table_join_sql' => $tableJoinSql,
	          'where_sql' => null,
	          'where_position' => null,
	          'group_sql' => null,
	          'order_sql' => null,
	          'limit_sql' => null,
	          'union_query' => $first? 1 : 2,
	          'query_builder_sql' => $first? $compactSql : null,
	          'query_builder_org_sql' => $first? $originalSql : null,
	          'created_by' => $userId,
	          'created_on' => now(),
	          'updated_by' => $userId,
	          'updated_on' => now(),
	        );
	        $insertSqlStmt->fields($fields);
	        expDebug::dPrintDBAPI('$insertSqlStmt', $insertSqlStmt);
	        $reportQueryBuilderId = $insertSqlStmt->execute();
	        expDebug::dPrint('$reportQueryBuilderId = ' . $reportQueryBuilderId);
	        $first = false;
	      } //end foreach
      } // end foreach
    }
    catch (Exception $ex) {
      $trxn->rollback();
      unset($trxn);
      watchdog_exception('saveNewUnionQueryInDB_trxn', $ex);
      expertusErrorThrow($ex);
    }
    unset($trxn);
	}
  catch (Exception $ex) {
    watchdog_exception('saveNewUnionQueryInDB', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Updates an existing union query in DB
 */
function updateUnionQueryInDB($reportId, $originalSql, $parsedQuery, $compactSql) {
  try {
  	$userId = getSltpersonUserId();
  	$trxn = db_transaction();
  	try {
      // Delete all records from slt_report_query_builder
      $delStmt = db_delete('slt_report_query_builder');
      $delStmt->condition('report_details_id', $reportId);
      expDebug::dPrintDBAPI('$delStmt', $delStmt);
      $delStmt->execute();

      // Loop through all unioned select queries and save in db (@TODO: save in reverse order?)
      $first = true;
      foreach ($parsedQuery as $unionKeyword => $unionedQueries) {
        expDebug::dPrint('$unionKeyword = ' . $unionKeyword, 4); // for debugging
        expDebug::dPrint('$unionedQueries = ' . print_r($unionedQueries, true), 4);
        foreach ($unionedQueries as $selectQuery) {
	      	expDebug::dPrint('$selectQuery = ' . print_r($selectQuery, true), 4);
	        list($tableSql, $tableAliasSql, $tableJoinSql, $columnSql, $columnAliasSql, $columnCriteriaSql, $columnCriteriaAliasSql)
	          = generateReportQueryBuilderValues($selectQuery);
	        $insertSqlStmt = db_insert('slt_report_query_builder');        
	        $fields = array(
	          'report_details_id' => $reportId,
	          'column_sql' => $columnSql,
	          'column_alias_sql' => $first? $columnAliasSql : null, 
	          'column_criteria_sql' => $first? $columnCriteriaSql : null,
	          'column_criteria_alias_sql' => $first? $columnCriteriaAliasSql :  null,
	          'table_sql' => $tableSql,
	          'table_alias_sql' => $tableAliasSql,
	        	'table_join_sql' => $tableJoinSql,
	          'where_sql' => null,
	          'where_position' => null,
	          'group_sql' => null,
	          'order_sql' => null,
	          'limit_sql' => null,
	          'union_query' => $first? 1 : 2,
	          'query_builder_sql' => $first? $compactSql : null,
	          'query_builder_org_sql' => $first? $originalSql : null,
	          'created_by' => $userId,
	          'created_on' => now(),
	          'updated_by' => $userId,
	          'updated_on' => now(),
	        );
	        $insertSqlStmt->fields($fields);
	        expDebug::dPrintDBAPI('$insertSqlStmt', $insertSqlStmt);
	        $reportQueryBuilderId = $insertSqlStmt->execute();
	        expDebug::dPrint('$reportQueryBuilderId = ' . $reportQueryBuilderId);
	        $first = false;
	      } // end foreach
      } // end foreach
    }
    catch (Exception $ex) {
      $trxn->rollback();
      unset($trxn);
      watchdog_exception('updateUnionQueryInDB_trxn', $ex);
      expertusErrorThrow($ex);
    }
    unset($trxn);
  }
  catch (Exception $ex) {
    watchdog_exception('updateUnionQueryInDB', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * 
 */
function validateAndSaveParsedReportsUnionQuery(&$reportId, $originalSql, $parsedQuery, $compactSql, &$errMsg) {
	try {
	  if (!isValidReportUnionQuery($compactSql, $parsedQuery, $errMsg)) {
      return false;
    }

    // Is a valid query which can be saved in db
    if (empty($reportId)) {
      saveNewUnionQueryInDB($reportId, $originalSql, $parsedQuery, $compactSql);
    }
    else {
      updateUnionQueryInDB($reportId, $originalSql, $parsedQuery, $compactSql);
    }
    
    return true;
	}
  catch (Exception $ex) {
    watchdog_exception('validateAndSaveParsedReportsUnionQuery', $ex);
    expertusErrorThrow($ex);    
  }	
}

/*
 * setQueryBuilder() - Split query into separate column 
 */
function setQueryBuilder($reportId = '') {
  try {
  	if ($reportId == 'undefined') {
  		$reportId = '';
  	}
    
    // Fetch the report sql query from $_REQUEST
    $originalSql = reportXSSValidate($_REQUEST['qryval']);
    expDebug::dPrint('$originalSql = ' . $originalSql , 4);    

    $compactSql = compactReportSQL($originalSql);
    expDebug::dPrint('$compactSql = ' . $compactSql, 4);
    
    // Do not proceed when there are forbidden keywords in the query
    if (forbiddenKeywordsInReportQuery($compactSql)) {
    	$errMsg = t('ERR167') . '. ' . t('ERR168') . '.';
      $retArr = array('Err' => $errMsg, 'report_id' => $reportId);
      expDebug::dPrint('$retArr = ' . print_r($retArr, true), 4);
      echo drupal_json_output($retArr);
      return;
    }
    
    $errMsg = '';
    
    //require_once(drupal_get_path('module', 'exp_sp_lnrreports') . '/PHP-SQL-Parser/src/PHPSQLParser.php');
    require_once(drupal_get_path('module', 'exp_sp_lnrreports') . '/php_sql_parser/src/PHPSQLParser.php');
    $parser = new PHPSQLParser($compactSql, true);
    expDebug::dPrint('$ $parser->parsed = ' . print_r($parser->parsed, true), 5);
    $parsedQuery = $parser->parsed;
       
    if (empty($parsedQuery['UNION']) && empty($parsedQuery['UNION ALL']) && empty($parsedQuery['UNION DISTINCT'])) {
      if (validateAndSaveParsedReportsSingleQuery($reportId, $originalSql, $parsedQuery, $compactSql, $errMsg) === false) {
        $retArr = array('Err' => $errMsg, 'report_id' => $reportId);
        expDebug::dPrint('$retArrsadas = ' . print_r($retArr, true), 4);
        echo drupal_json_output($retArr);
        return;
      }
    }
    else { // union query
      if (validateAndSaveParsedReportsUnionQuery($reportId, $originalSql, $parsedQuery, $compactSql, $errMsg) === false) {
        $retArr = array('Err' => $errMsg, 'report_id' => $reportId);
        expDebug::dPrint('$retArr = ' . print_r($retArr, true), 4);
        echo drupal_json_output($retArr);
        return;
      }
    }
    expDebug::dPrint("asdasdsad".print_r($parsedQuery['FROM'][0]['sub_tree']['UNION'][0],true),5);
    
    // Validate there is no direct table used in report when it is turned off
    require_once('sites/all/services/GlobalUtil.php');
    $gutil = new GlobalUtil();
    $expConfig = $gutil->getConfig();
    $slt_table_exten = trim($expConfig['slt_table_exten']);
    expDebug::dPrint("config variable-->".$slt_table_exten);
    expDebug::dPrint("Parsed Query  -->".print_r($parsedQuery,true),1);
    $tables = Array();
    $tablelist = getalltables();
    if($slt_table_exten == 0){
    	//list($a, $table) = explode('from', $originalSql);
    	getAllQueryTables($parsedQuery,$tables);
    	foreach($tables as $table){
    	expDebug::dPrint("table name 2222-->".$table);
    	$tableExnt = 0;
    	if(in_array(trim($table),$tablelist)){
    		$tableExnt = 1;
    	}
    	$table_name_exten = substr($table, 0, 4);
    	expDebug::dPrint("table extentionss-->".$tableExnt);
    	if((trim($table_name_exten) == 'slt') || ($tableExnt == 1)){
    		$errMsg = t("ERR253");
    		//$errMsg .= $table.' does not exist.';
    		$retArr = array('Err' => $errMsg, 'report_id' => $reportId);
    		expDebug::dPrint('$retArrsd = ' . print_r($retArr, true), 4);
    		echo drupal_json_output($retArr);
    		return;
    	}
    }
    
    } 
    
    //Added for #0070534
    $pcheck = '';
    $status = db_query('SELECT status FROM slt_report_details WHERE id = :repId ',array(':repId' => $reportId))->fetchField();
    if($status != 'cre_rpt_rps_dft'){
	  	$pcheck = resetReportValidate($reportId);
	  	$pcheck = ($pcheck == 0)? 'NON_SCALABLE_QUERY' : 'SCALABLE_QUERY';
	  }
    
    $retArr = array('Err' => 'Success', 'result' => $pcheck, 'report_id' => $reportId);
    expDebug::dPrint('$retArr = ' . print_r($retArr, true), 4);
    echo drupal_json_output($retArr);
  }
  catch (Exception $ex) {
  	watchdog_exception('setQueryBuilder', $ex);
  	expertusErrorThrow($ex);
  }
}


/* function getAllQueryTables($parsedQuery){
	$tablelist = Array();
	$tables = Array();
	/* if (empty($parsedQuery['UNION']) && empty($parsedQuery['UNION ALL']) && empty($parsedQuery['UNION DISTINCT'])) {
		//$table = $parsedQuery['FROM'][0]['table'];
		$tablelist = array_merge($tables,$parsedQuery['FROM']);
	}else{
		$tablelist = array_merge($tables,$parsedQuery['UNION'][0]['FROM']);;
	}
	if(empty($table)){
		if(isset($parsedQuery['FROM'][0]['sub_tree']['UNION'][0])){
			$tablelist = $parsedQuery['FROM'][0]['sub_tree']['UNION'][0]['FROM'][0]['table'];
		}
	} */
/*	traverseArray($parsedQuery,$tablelist);
	expDebug::dPrint("TABLE LIST FROM -- ".print_r($tablelist,1),1);
	foreach($tablelist as $tbl){
		$tables[] = $tbl['table'];
	}
	expDebug::dPrint("QUERY TABLES -- > ".print_r($tables,1),1);
	return $tables;
} */

function getAllQueryTables($parsedQuery, &$tables){
	foreach($parsedQuery as $key=>$value)
	{
		if(is_array($value))
		{
			getAllQueryTables($value,$tables);
		}else{
			if($key == 'table'){
				expDebug::dPrint("TESTING ".print_r($value,true),1);
				$tables[] = $value;
			}
		}
	}
}

/**
 * Parses the table name and alias
 * If table is slt_profile_list_item, also returns the corresponding table.column to which it is joined.
 */
function parseSQLTableNameAlias($value) {
	try {
		global $next,$finalArray;
		expDebug::dPrint('$value = ' . print_r($value, true), 4);
		$name = '';
		$alias = '';
		$spliJoinCol = '';
		if($value['expr_type']=='table'){
		$name = $value['table'];
		}
		elseif($value['expr_type']=='subquery'){
			$next = false;
			$finalArray = array();
			array_walk($value, 'arrayCheckRef','table');
			$tableNames = implode(",",array_unique($finalArray));
			$name = $tableNames;
		}
		if (count($value['alias']) > 1) {
			$alias = $value['alias']['name'];
		}
		else {
			$alias = $value['table'];
		}
		$name = str_replace(array('\'', '"'), '', $name);
		$alias = str_replace(array('\'', '"'), '', $alias);
		expDebug::dPrint('$name = ' . $name, 4);
		expDebug::dPrint('$alias = ' . $alias, 4);
		
		if ($name == 'slt_profile_list_items' && strtolower($value['ref_type']) == 'on' && count($value['ref_clause']) > 0) {
			// Find the index of spli alias.code
			$spliCodeIdx = -1;
			foreach($value['ref_clause'] as $idx => $token) {
				if (!empty($token['base_expr']) && $token['base_expr'] == $alias . '.code') {
					$spliCodeIdx = $idx;
					break;
				}
			}
			expDebug::dPrint('$spliCodeIdx = ' . $spliCodeIdx, 4);
			if ($spliCodeIdx >= 0) {
			  // If '=' sign suceeds, join column is at index + 2
				if (!empty($value['ref_clause'][$spliCodeIdx + 1]) && !empty($value['ref_clause'][$spliCodeIdx + 1]['base_expr']) &&
						$value['ref_clause'][$spliCodeIdx + 1]['base_expr'] == '=' &&
						!empty($value['ref_clause'][$spliCodeIdx + 2]) && !empty($value['ref_clause'][$spliCodeIdx + 2]['base_expr'])) {
					expDebug::dPrint('= sign succeeds spli code', 4);
			  	$spliJoinCol = $value['ref_clause'][$spliCodeIdx + 2]['base_expr'];
			  }
			  // Else if '=' sign preceeds, join column is as at index -2
			  else if (!empty($value['ref_clause'][$spliCodeIdx - 1]) && !empty($value['ref_clause'][$spliCodeIdx - 1]['base_expr']) &&
			  		    $value['ref_clause'][$spliCodeIdx - 1]['base_expr'] == '=' &&
			  		        !empty($value['ref_clause'][$spliCodeIdx - 2]) && !empty($value['ref_clause'][$spliCodeIdx - 2]['base_expr'])) {
			  	expDebug::dPrint('= sign preceeds spli code', 4);
			  	$spliJoinCol = $value['ref_clause'][$spliCodeIdx - 2]['base_expr'];
			  }
			}
		}
		
	  $returnArr = array($name, $alias, $spliJoinCol);
	  expDebug::dPrint('$returnArr = ' . print_r($returnArr, true), 4);
	  return $returnArr;
	}
	catch (Exception $ex) {
		watchdog_exception('parseSQLTableNameAlias', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Parses the column name and alias
 */
function parseSQLColumnNameAlias($value) {
	try {
	  if (count($value['alias']) > 1) {
//#70379 changes
/** Building expression using PHPSQLCreator class and using it in place of field name and alias instead of the commented code**/
	  	/* $name = $value['base_expr'];
	  	 $alias = $value['alias']['name']; */
 	  	$newSelectVal = array('SELECT' => array($value)); 
 	  	require_once(drupal_get_path('module', 'exp_sp_lnrreports') . '/php_sql_parser/src/PHPSQLCreator.php'); 	  	
	  	$creator = new PHPSQLCreator($newSelectVal);
	  	$creator->created = trim($creator->created); 
	  	expDebug::dPrint('$newly created field_string = ' . print_r($creator, true), 4);
	  	$selectPattern = "/^SELECT /i";
	  	$aliasPattern = "/".$value['alias']['base_expr'].$value['delim']."$/i";
	  	$delimPattern = "/".$value['delim']."$/i";
  		if(preg_match($selectPattern, $creator->created,$matches)=== 1){
  			$creator->created = preg_replace($selectPattern, '', $creator->created);
  		}
  		if(preg_match($aliasPattern, $creator->created,$matches)=== 1){
  			$exprArray[0] = preg_replace($aliasPattern, '', $creator->created);
  		}elseif (preg_match($delimPattern, $creator->created,$matches)=== 1){
  			$exprArray[0] = preg_replace($delimPattern, '', $creator->created);
  		}
  		$exprArray[1] = $value['alias']['no_quotes'];
	  	expDebug::dPrint('$created expr array for expression w/o alias' . print_r($exprArray, true), 4);	    
		$name = $exprArray[0];
		$alias = $exprArray[1];
			
	  }
	  else {
		  if (count($value['sub_tree']) > 1) {
			$alias = $value['sub_tree'][1]['base_expr'];
//#70379 changes
        	$aliasPattern = "/ ".$value['sub_tree'][1]['base_expr']."$/";
        	$delimPattern = "/".$value['delim']."$/";
        	if(preg_match($aliasPattern, $value['base_expr'])=== 1){
        		$name_array[0] = preg_replace($aliasPattern, '', $value['base_expr']);
        	}elseif (preg_match($delimPattern, $value['base_expr'])=== 1){
        		$name_array[0] = preg_replace($delimPattern, '', $value['base_expr']);
        	}
        	expDebug::dPrint('$newly created name array = ' . print_r($name_array, true), 4);
        	$name = $name_array[0];
		  }
		  else {
        	$name = $value['base_expr'];
        	$alias = $value['base_expr'];
		  }
	  }
//#70379
/**Instead of replacing quotes,Just using urlencode. It gets decoded while inserting into the table**/
	  if($value['expr_type']!='function' && $value['expr_type']!='expression' && $value['expr_type']!='const')
	  	$name = str_replace(array('\'', '"'), '', $name);
	  $alias = str_replace(array('\'', '"'), '', $alias);
	  $name = rawurlencode(trim($name));	  	  
	  //$alias = urlencode($alias);
	  
	  $returnArr = array($name, $alias);
	  expDebug::dPrint('$returnArr = ' . print_r($returnArr, true), 4);
	  return $returnArr;
	}
	catch (Exception $ex) {
		watchdog_exception('parseSQLColumnNameAlias', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Parses the column name and alias / table name and alias
 */
function parseSQLNameAlias($type, $value) {
	global $finalArray;
	$finalArray = array();
	try {
    expDebug::dPrint('$type = ' . print_r($type, true), 4);
	  expDebug::dPrint('$value = ' . print_r($value, true), 4);
	  $name = '';
	  $alias = '';
	  $spliJoinCol = '';
	  
	  if ($type == 'table') {
      list($name, $alias, $spliJoinCol) = parseSQLTableNameAlias($value);
	  }
	  else {
	  	list($name, $alias) = parseSQLColumnNameAlias($value);
	  }
	
	  // Fetch expression type
	  if (isset($value['expr_type'][0]['expr_type'])) {
	    $exprType = $value['expr_type'][0]['expr_type'];
	  }
	  else {
	    $exprType = $value['expr_type'];
	  }
	  if($exprType == 'expression' || $exprType == 'function'){
		  $next = false;
		  array_walk($value,'arrayCheckRef','aggregate_function');
		  expDebug::dPrint('$$finalArray after array walk search for aggregate_function = ' . print_r($finalArray, true), 4);
		  if(count($finalArray)>0)
		  	$exprType = 'aggregate_function';
//Added to figure if the expression contains more than one column. If so the will not be made available as a report criteria		  
		  $next = false;
		  $finalArray=array();
		  array_walk($value,'arrayCheckRef','colref');
		  expDebug::dPrint('$$finalArray after array walk search for colref = ' . print_r($finalArray, true), 4);
		  if(count(array_unique($finalArray))>1)
		  	$exprType = 'multi_column_expression';
		  
	  }
	  $returnArr = array($name, $alias, $exprType, $spliJoinCol);
	  expDebug::dPrint('$returnArr = ' . print_r($returnArr, true), 4);
	  return $returnArr;
	}
	catch (Exception $ex) {
		watchdog_exception('parseSQLNameAlias', $ex);
		expertusErrorThrow($ex);
	}
}

/* Loop through the substree */ 
function getSubTree($value){ // Currently not in use
  try{
  expDebug::dPrint(' getSubTree() - INPUT = ' . print_r($value, true) , 4);
  
  while(isset($value['sub_tree']) && count($value['sub_tree']) == 1){
	$value = $value['sub_tree'][0];
  }	
  
  expDebug::dPrint(' getSubTree() - OUTPUT = ' . print_r($value['base_expr'], true) , 4);
  
  return $value['base_expr'];
  } catch (Exception $ex) {
  	watchdog_exception('getSubTree', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Functon to XSS validate in report fields.
 * $field (String) - Field element to validate
 * $name (String) - Field element name
 * $Type (Integer) - Determine to throw error or striped data
 *   0 - return striped data, 1 - throw error
 */
function reportXSSValidate($field,$name='',$type=''){
	if(empty($field))
		return $field;
	
	$field1 = expertus_xss_validate_security($field);
	if($field !== $field1){
		if($type === 0){
			return $field1;
		}else{
			throw new Exception(t('ERR071')." ".t($name));
		}
	}else{
		return $field;
	}
}

function addReportPublishAndUnpublish() {
  $txn = db_transaction();
  
  try{
    $userId = getSltpersonUserId();
    // Added reportXSSValidate by Vincent for #0050212 on June 9, 2015
    $themeName    = reportXSSValidate($_REQUEST['set_mainquerform_Theme_Name'],'LBL802',0);
    // Added security fix to avoid SQL Fatal Error when inject through Fiddler by ganeshbabuv on june 26th 2015 1:00PM
    $reportId     = orgValidate(reportXSSValidate($_REQUEST['report_id'],'Id',1)); 
    $reportStatus = reportXSSValidate($_REQUEST['set_mainquerform_statusType'],'LBL102',1);
    $rTitle				=	reportXSSValidate($_REQUEST['set_mainquerform_ReportTitle'],'LBL799',0);
    $rDesc				=	reportXSSValidate($_REQUEST['set_mainquerform_ReportDescription'],'LBL229',0);
    $rVisibility	=	reportXSSValidate($_REQUEST['set_mainquerform_report_visibility_type'],'LBL800',1);
    $rbType				=	reportXSSValidate($_REQUEST['report_builder_type'],'LBL036',1);
    $rType				=	reportXSSValidate($_REQUEST['set_report_type'],'LBL801',1);
    
    //print $themeName;
    $fields = array(
    								'title' => @rawUrlDecode($rTitle),
    								'description' => @rawUrlDecode($rDesc),
                    'report_visibility' => $rVisibility,
                    'report_builder_type' => $rbType,
                    'report_type' => $rType,
    				        'status' => $reportStatus,
                    'created_by' => $userId,
                    'created_on' => now()
              );    

    // Below function added by Vincent on 30 Nov, 2016 for #0070534
    $isValidQuery = checkReportPerformance($reportId);
    if($isValidQuery === NON_SCALABLE_QUERY)
    	throw new Exception('PDO Exception: maximum statement execution time exceeded');
    
    $fields['is_validated'] = 1;
    
    $updateStmt = db_update('slt_report_details');
    $updateStmt->fields($fields);
    $updateStmt->condition('id', $reportId, '=');
    $updateStmt->execute();
    
    // Audit entry
    make_audit_trail_entries($fields, $userId, null ,$reportId, 'Updated','cre_rpt_rps','exp_sp_lnrreports','addReportPublishAndUnpublish',
                                        array('title', 'description', 'report_visibility', 'status' ,'report_header_text'));

    if ($reportStatus == 'cre_rpt_rps_itv') {
      $schdSelect = db_select('slt_report_schedules', 'rep_sched');
      $schdSelect->addField('rep_sched', 'id');
      $schdSelect->addField('rep_sched', 'status');
      $schdSelect->condition('rep_sched.report_id', $reportId);
      $schdSelect->condition('rep_sched.status', 'cre_rpt_rss_atv', '=');
      expDebug::dPrintDBAPI('$schdSelect', $schdSelect);
      $schdPrevStatuses = $schdSelect->execute()->fetchAll();
      expDebug::dPrint('$schdPrevStatuses' . print_r($schdPrevStatuses, true));
      
      $schdFields = array();
      $schdFields['status'] = 'cre_rpt_rss_itv';
      $schdFields['updated_by'] = $userId;
      $schdFields['updated_on'] = now();
      $schdUpdate = db_update('slt_report_schedules');
      $schdUpdate->fields($schdFields);
      $schdUpdate->condition('report_id', $reportId);
      $schdSelect->condition('status', 'cre_rpt_rss_atv');
      expDebug::dPrintDBAPI('$schdUpdate', $schdUpdate);
      $schdUpdate->execute();

      foreach ($schdPrevStatuses as $schd) {
        $schdPrev = array($schd);
        make_audit_trail_entries($schdFields, $userId, $schdPrev, $schd->id, 'Updated', 'cre_rpt_rss', 'exp_sp_lnrreports',
                                         'addReportPublishAndUnpublish', array('status'));
      }
    }
                                         
  }
  catch (Exception $ex) {
    $txn->rollback();
    //Added for #0070534
    $expErrorMsg = $ex->getMessage();
    if (strpos($expErrorMsg, 'maximum statement execution time exceeded') > 0 ) {
			print 'NON_SCALABLE_QUERY';
    }else{
	    watchdog_exception('addReportPublishAndUnpublish', $ex);
	    //throw $ex;
	    print $ex->getMessage();
    }
  }
  unset($txn);
}

/**
 * Check if the report performance check is done or not
 * This introduced for #0070534
 * @param $reportId
 * @return unknown_type
 */
function resetReportValidate($reportId){
	$filterDetail = db_query('SELECT id, mandatory FROM slt_report_criteria WHERE report_details_id= :repId ',array(':repId'=>$reportId))->fetchAll();
  $isMandatory = FALSE;
	foreach($filterDetail as $filter){
		if($filter->mandatory == 1){
			$isMandatory = TRUE;
		}
	}
	if($isMandatory === FALSE){
		db_query('update slt_report_details set is_validated = null where id = :repId ',array(':repId' => $reportId));
    
    return checkReportPerformance($reportId);
	}
	return SCALABLE_QUERY; 
}

/**
 * To find if the report query execute within the configured time or not
 * This function introduced for #0070534
 * @param $reportId
 * @return Booleon
 */
function checkReportPerformance($reportId){
	$reportValidated = db_query('SELECT is_validated FROM slt_report_details WHERE id= :repId ',array(':repId'=>$reportId))->fetchField();
	expDebug::dPrint("Checking query performance report validity ".print_r($reportValidated,true),3);
	if($reportValidated)
		return SCALABLE_QUERY;
		
	$filterDetail = db_query('SELECT id, mandatory FROM slt_report_criteria WHERE report_details_id= :repId ',array(':repId'=>$reportId))->fetchAll();
	expDebug::dPrint("Checking query performance report filters ".print_r($filterDetail,true),3);
		
	$isMandatory = FALSE;
	foreach($filterDetail as $filter){
		if($filter->mandatory == 1){
			$isMandatory = TRUE;
		}
	}
	expDebug::dPrint("Checking query performance report mandatory filters ".print_r($isMandatory,true),3);
	if($isMandatory == TRUE){
		return SCALABLE_QUERY;
	}else{
		try{
			$res = loadLearnerReportResult($reportId, 1, 1, '', '', '', null,'Internal');
			expDebug::dPrint("Checking query performance report execution ".print_r($res,true),3);
			if($res === TRUE)
				return SCALABLE_QUERY;
			return NON_SCALABLE_QUERY;
		}catch(Exception $e){
			expDebug::dPrint('Error in performance check '.print_r($e->getMessage(),true),1);
			return NON_SCALABLE_QUERY;
		}
	}
}

/**
 * Created new wizard report record in db
 */
function newWizardReport() {
	try {
		$insertStmt = db_insert('slt_report_details');
		$insertStmt->fields(array('status' => 'cre_rpt_rps_dft',
				                      'report_builder_type' => 'design_wizard'));
		expDebug::dPrintDBAPI('$insertStmt', $insertStmt);
		$reportDetailsId = $insertStmt->execute();
		expDebug::dPrint('$reportDetailsId = ' . $reportDetailsId);
		
		return $reportDetailsId;
	}
	catch (Exception $ex) {
		watchdog_exception('newWizardReport', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Creates new query_builder
 */
function getWizardReportQueryBuilderDetails($reportDetailsId) {
	try {
		$select = db_select('slt_report_query_builder', 'query_builder');
		$select->addField('query_builder', 'column_sql');
		$select->addField('query_builder', 'column_alias_sql');
		$select->condition('query_builder.report_details_id', $reportDetailsId, '=');
		$select->condition('query_builder.union_query', '0', '=');
		expDebug::dPrintDBAPI('$select', $select);
		
		$builderInfo = $select->execute()->fetchAssoc();
		expDebug::dPrint('$builderInfo = ' . print_r($builderInfo, true));
		
		return $builderInfo;
	}
	catch (Exception $ex) {
		watchdog_exception('getWizardReportQueryBuilderDetails', $ex);
		expertusErrorThrow($ex);
	}
}
/**
 * Add/edit Design Wizard report data
 */
function editReportData() {
	try {
    global $user;
    // Vincent Added on 11 Aug, 2015 for # 0052759 item 4
    foreach($_REQUEST as $name=>$val){
    	$_REQUEST[$name] = reportXSSValidate($val,$name,0);
    }
    
    $reportDetailsId  = $_REQUEST['report_details_id'];    
    // If this is a new report, create an id for it by adding a draft record in slt_report_details table
    if (empty($reportDetailsId) || $reportDetailsId == 'undefined') {
	    $reportDetailsId = newWizardReport();
    }
    
    // Get any existing query builder info
    $builderInfo = getWizardReportQueryBuilderDetails($reportDetailsId);		
		$hasBuilderInfo = empty($builderInfo)? false : true;
	  expDebug::dPrint('$hasBuilderInfo = ' . $hasBuilderInfo, 4);

	  $columnSqlDBArray = array();
	  $columnAliasSqlDBArray = array();
	  if ($hasBuilderInfo) {
	    $columnSqlDBArray = explode('~~', $builderInfo['column_sql']);
	    expDebug::dPrint('$columnSqlDBArray = ' . print_r($columnSqlDBArray, true), 4);
	    $columnAliasSqlDBArray = explode(',', $builderInfo['column_alias_sql']);
	    expDebug::dPrint('$columnAliasSqlDBArray = ' . print_r($columnAliasSqlDBArray, true), 4);
	  }
  
    // Rebuild wizard sql and compact it
	  $columnAliasSql = array();
    $columnMapTableArray = array();
    $spliCounter = 0;
    $columnMapFieldArray = array();
    $columnMapWhereArray = array();
    $columnSqlModifiedArray = array();
    $columnSqlString  = '';
    $columnSqlArray = explode(",", $_REQUEST['column_sql']);
    expDebug::dPrint('$columnSqlArray = ' . print_r($columnSqlArray, true), 4);
    $columnAliasSqlArray = explode(",", $_REQUEST['column_alias_sql']);
    expDebug::dPrint('$columnAliasSqlArray = ' . print_r($columnAliasSqlArray, true), 4);
    $columnMapListItems = explode(",", $_REQUEST['map_list_items']);
    expDebug::dPrint('$columnMapListItems = ' . print_r($columnMapListItems, true), 4);
    
    for ($i = 0; $i < count($columnSqlArray); $i++) {
      if ($columnMapListItems[$i] == 1) { // Is a column whose value is a code from slt_profile_list_items table
        $spliCounter++;
        $columnMapTableArray[] = 'slt_profile_list_items listitems' . $spliCounter;
        $columnMapWhereArray[] = 'listitems' . $spliCounter . '.code = ' . $columnSqlArray[$i];
        $columnSqlModifiedArray[] = 'listitems' . $spliCounter . '.name';
        if (in_array($columnSqlArray[$i], $columnSqlDBArray)) {
          $columnMapFieldArray[] = 'listitems' . $spliCounter . '.name' . ' AS "' .
                                         $columnAliasSqlDBArray[array_search($columnSqlArray[$i], $columnSqlDBArray)] . '"';
          $columnAliasSql[] = $columnAliasSqlDBArray[array_search($columnSqlArray[$i], $columnSqlDBArray)];
        }
        else {
          $columnMapFieldArray[] = 'listitems' . $spliCounter . '.name' . ' AS "' . $columnAliasSqlArray[$i] . '"';
          $columnAliasSql[] = $columnAliasSqlArray[$i];
        }
      }
      else {
        if (!empty($columnSqlString)) {
          $columnSqlString .= ', ';
        }
        if (in_array($columnSqlArray[$i], $columnSqlDBArray)) {
          $columnSqlString .= $columnSqlArray[$i] . ' AS "' . $columnAliasSqlDBArray[array_search($columnSqlArray[$i], $columnSqlDBArray)] . '"';
          $columnAliasSql[] = $columnAliasSqlDBArray[array_search($columnSqlArray[$i], $columnSqlDBArray)];
        }
        else {
          $columnSqlString .= $columnSqlArray[$i] . ' AS "' . $columnAliasSqlArray[$i] . '"';
          $columnAliasSql[] = $columnAliasSqlArray[$i];
        }
        $columnSqlModifiedArray[] = $columnSqlArray[$i];
      }
    } //end for
    expDebug::dPrint('$columnSqlModifiedArray = ' . print_r($columnSqlModifiedArray, true) , 4);
    expDebug::dPrint('$columnAliasSql = ' . print_r($columnAliasSql, true) , 4);
    expDebug::dPrint('$columnMapTableArray = ' . print_r($columnMapTableArray, true) , 4);
    expDebug::dPrint('$spliCounter = ' . print_r($spliCounter, true) , 4);
    expDebug::dPrint('$columnMapFieldArray = ' . print_r($columnMapFieldArray, true) , 4);
    expDebug::dPrint('$columnMapWhereArray = ' . print_r($columnMapWhereArray, true) , 4);
    expDebug::dPrint('$columnSqlModifiedArray = ' . print_r($columnSqlModifiedArray, true) , 4);
    expDebug::dPrint('$columnSqlString = ' . print_r($columnSqlString, true) , 4);
  
    if ($spliCounter > 0) {
      if (empty($columnSqlString)) {
        $columnMapField = implode(',', $columnMapFieldArray);
      }
      else {
        $columnMapField = ',' . implode(',', $columnMapFieldArray);
      }
      $columnMapTable = ',' . implode(',', $columnMapTableArray);
      $columnMapWhere = ' AND ' . implode(' AND ', $columnMapWhereArray);
    }
  
    //Query Builder
    $orgSQL = "SELECT ". $columnSqlString . $columnMapField. " FROM " . $_REQUEST['table_sql'] . $columnMapTable . " WHERE 1=1";
    
    expDebug::dPrint('$_REQUEST[rel_column] = ' . $_REQUEST['rel_column'], 4);
    if (!empty($_REQUEST['rel_column'])) {
      $relCol = str_replace(",", " AND ", $_REQUEST['rel_column']);
      $orgSQL .= " AND " . $relCol;
    }
    $orgSQL .= $columnMapWhere;
    expDebug::dPrint('$orgSQL = ' . $orgSQL, 4);

    $compactSQL = compactReportSQL($orgSQL);
    $wherePosition = strpos(strtolower($compactSQL), 'where');
    $wherePosition = empty($wherePosition)? 0 : $wherePosition + 6;

    if ($hasBuilderInfo) {
      $columnObj = new stdClass();
      $columnObj->reportId = $reportDetailsId;
      $columnObj->columnArray = explode(',', $_REQUEST['column_sql']);
      $columnObj->columnAliasArray = $columnAliasSql;
      
      $colOrder = reorderColumnProcessing($columnObj);
      
      $column_sql = implode('~~', $colOrder->newColumnSql);
      $column_alias_sql = implode(',', $colOrder->newColumnAliasSql);
        
    	$saveStmt = db_update('slt_report_query_builder');    	  
      $fields = array(
        'column_sql' => $column_sql,
        'column_alias_sql' => $column_alias_sql, 
        'column_criteria_sql' => implode(',', $columnSqlModifiedArray),
        'column_criteria_alias_sql' => implode(',', $columnAliasSql),
        'table_sql' => $_REQUEST['table_sql'],
        'table_alias_sql' => $_REQUEST['table_alias_sql'],   
        'where_sql' => $_REQUEST['rel_column'],
        'where_position' => $wherePosition,
        'group_sql' => '',
        'order_sql' => '',
        'limit_sql' => '',
        'window_positions' => $_REQUEST['window_positions'],
        'union_query' => 0,
        'query_builder_sql' => trim($compactSQL),
        'query_builder_org_sql' => $orgSQL,
        'updated_by' => $user->uid,
        'updated_on' => now()
      );
      $saveStmt->fields($fields);
      $saveStmt->condition('report_details_id', $reportDetailsId, '=');
    }
    else {
      $saveStmt = db_insert('slt_report_query_builder');
      $fields = array(
        'report_details_id' => $reportDetailsId,
        'column_sql' => str_replace(',', '~~', $_REQUEST['column_sql']),
        'column_alias_sql' => implode(',', $columnAliasSql), 
        'column_criteria_sql' => implode(',', $columnSqlModifiedArray),
        'column_criteria_alias_sql' => implode(',', $columnAliasSql),
        'table_sql' => $_REQUEST['table_sql'],
        'table_alias_sql' => $_REQUEST['table_alias_sql'],   
        'where_sql' => $_REQUEST['rel_column'],
        'where_position' => $wherePosition,
        'group_sql' => '',
        'order_sql' => '',
        'limit_sql' => '',
        'window_positions' => $_REQUEST['window_positions'],
        'union_query' => 0,
        'query_builder_sql' => trim($compactSQL),
        'query_builder_org_sql' => $orgSQL,
        'created_by' => $user->uid,
        'created_on' => now(),
        'updated_by' => $user->uid,
        'updated_on' => now(),
      );  	  		    
  	  $saveStmt->fields($fields);
    }
  	// Execute the update statement.
    expDebug::dPrintDBAPI('$saveStmt', $saveStmt);
  	$saveStmt->execute();
  	
	//Added for #0070534
		$pcheck = '';
		$status = db_query('SELECT status FROM slt_report_details WHERE id = :repId ',array(':repId' => $reportDetailsId))->fetchField();
    if($status != 'cre_rpt_rps_dft'){
	  	$pcheck = resetReportValidate($reportDetailsId);
	  	$pcheck = ($pcheck == 0)? 'NON_SCALABLE_QUERY' : 'SCALABLE_QUERY';
	  }
	  
  	print json_encode( array('report_id' => $reportDetailsId , 'result' => $pcheck));
  }
  catch (Exception $ex) {
    watchdog_exception('editReportData', $ex);
    expertusErrorThrow($ex);
  }
}


function launchReportForamt() {
  try{
  // Added reportXSSValidate by Vincent for #0050212 on June 9, 2015
  $id = reportXSSValidate($_REQUEST['report_id']);
  // Select slt_organization table
  $select = db_select('slt_report_criteria', 'rep');
  // Get org id
  $select->addField('rep', 'id', 'id');
  $select->addField('rep', 'report_details_id', 'report_details_id');
  $select->addField('rep', 'name', 'name');
  $select->addField('rep', 'title', 'title');
  $select->addField('rep', 'query_column_name', 'query_column_name');
  $select->addField('rep', 'criteria_type', 'criteria_type');
  $select->addField('rep', 'criteria_display', 'criteria_display');
  $select->addField('rep', 'expand_display', 'expand_display');
  $select->addField('rep', 'return_column', 'return_column');
  $select->addField('rep', 'display_column', 'display_column');
  $select->addField('rep', 'overview_column', 'overview_column');
  $select->addField('rep', 'match_column', 'match_column');
  $select->addField('rep', 'criteria_defaults', 'criteria_defaults');
  $select->addField('rep', 'criteria_list', 'criteria_list');
  $select->addField('rep', 'sequence', 'sequence');
  // Set the conditions
  $select->condition('rep.report_details_id', $id, '=');

  
  expDebug::dPrint('$select id = ' . $id , 4);
  expDebug::dPrintDBAPI(' $select obj = ' , $select);
  
  // Execure the query
  $result = $select->execute()->fetchAll();
  expDebug::dPrint('$result = ' . print_r($result, true) , 3);
  
  if (count($result) <= 0) {
    echo ''; // Does not exist
  } 
  
  echo drupal_json_output($result);
  } catch (Exception $ex) {
  	watchdog_exception('launchReportForamt', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * get the list tables - from database
 */

function getReportTables($tableName='') {
	try{
		$gutil = new GlobalUtil();
		$expConfig = $gutil->getConfig();
		$slt_table_exten = trim($expConfig['slt_table_exten']);
  $tableName = ($tableName == '0') ? '' : trim($tableName); 
  expDebug::dPrint(' $$tableName = ' . $tableName , 4);
  // To get the table and table alias details to display
  $tableQuery = db_select('slt_report_tables_alias', 'table_alias');
  $tableQuery->addField('table_alias', 'id', 'table_id');
  $tableQuery->addField('table_alias', 'table_name', 'table_name');
  $tableQuery->addField('table_alias', 'table_alias_name', 'table_alias');
  $tableQuery->addField('table_alias', 'table_description', 'table_description');
  if($slt_table_exten == 0)
  	$tableQuery->condition('table_alias.table_name', 'slv_%', 'like');
  else
  $tableQuery->condition('table_alias.table_name', 'slt_%', 'like');
  //	$tableQuery->condition(db_or()->condition('table_alias.table_name', 'slv_%', 'like')->condition('table_alias.table_name', 'slt_%', 'like'));
  
  $tableQuery->condition('table_alias.is_visible', '1');
  if(trim($tableName) != ""){
    $tableQuery->condition('table_alias.table_alias_name', '%' . db_like($tableName) . '%', 'like');
  }
  $tableQuery->orderBy('table_alias_name', 'ASC');
  expDebug::dPrintDBAPI("Query for report design wizard",$tableQuery);
  $resultTableQuery = $tableQuery->execute()->fetchAll();
  expDebug::dPrint('$resultTableQuery = '.print_r($resultTableQuery,true) , 3);
	
  if(trim($tableName) != "" && count($resultTableQuery)){
    $tableIdArray = array();
    foreach($resultTableQuery as $tableDetails){
      $tableIdArray[] = $tableDetails->table_id;
    }
	$tableIdVar = implode(',', $tableIdArray);
  	if($slt_table_exten == 0){
      $relatedTableQuery = db_query('select id as table_id, table_name as table_name,
					table_alias_name as table_alias, table_description as table_description from slt_report_tables_alias 
					where id in (select relation_table_id from slt_report_tables_relation where table_id in ('.$tableIdVar.')) and
					table_name like \'slv%\' and is_visible=\'1\'');
	}else{
		$relatedTableQuery = db_query('select id as table_id, table_name as table_name,
					table_alias_name as table_alias, table_description as table_description from slt_report_tables_alias 
					where id in (select relation_table_id from slt_report_tables_relation where table_id in ('.$tableIdVar.')) and
					table_name like \'slt%\' and is_visible=\'1\'');
	}
    
    $relatedTableResult  = $relatedTableQuery->fetchAll();
    $resultTableQuery = array_merge($resultTableQuery,$relatedTableResult);
  }
  $c1 = 0; // Count for tables
  $tableDetailsArray = array();
  if(count($resultTableQuery)){

    foreach($resultTableQuery as $tableDetails){
      
      $tableDetailsArray[$c1]["dbtable"] = $tableDetails->table_name;
      $tableDetailsArray[$c1]["dbtable_display"] = $tableDetails->table_alias;
      $tableDetailsArray[$c1]["dbtable_description"] = $tableDetails->table_description;
      $tableAliasId = $tableDetails->table_id;
      
      // To get the field and fields alias details to display
      $fieldQuery = db_select('slt_report_tables_fields_alias', 'field_alias');
      $fieldQuery->addField('field_alias', 'table_field_name', 'field_name');
      $fieldQuery->addField('field_alias', 'table_field_alias_name', 'field_alias');
      $fieldQuery->addField('field_alias', 'table_field_description', 'field_description');
      $fieldQuery->addField('field_alias', 'map_list_items', 'map_list_items');
      $fieldQuery->addField('field_alias', 'table_field_type', 'field_type');
      $fieldQuery->condition('field_alias.table_alias_id', $tableAliasId);
      $fieldQuery->condition('field_alias.is_visible', '1');
      $resultFieldQuery = $fieldQuery->execute()->fetchAll();
      
      $relatedTableDetails  = getRelatedTableNames($tableAliasId);
    
      $c2 = 0; // Count for table fields
      $fieldDetailsArray = array();
      $fieldDetailsDisplayArray = array();
      $fieldDetailsMapListArray = array();
      if(count($resultFieldQuery)){
        foreach($resultFieldQuery as $fieldDetails){
          $fieldDetailsArray[$c2] = $fieldDetails->field_name;
          $fieldDetailsDisplayArray[$c2] = $fieldDetails->field_alias;
          $fieldDetailsMapListArray[$c2] = $fieldDetails->map_list_items;
          $c2++;
        }
        
        if(count($fieldDetailsArray)){
          $tableDetailsArray[$c1]["dbcolumns"] = $fieldDetailsArray;
          $tableDetailsArray[$c1]["dbcolumns_display"] = $fieldDetailsDisplayArray;
          $tableDetailsArray[$c1]["dbcolumns_map_list"] = $fieldDetailsMapListArray;
          $tableDetailsArray[$c1]["related_table_names"] = $relatedTableDetails['table_names'];//'tbl_slt_attendance_details,tbl_slt_attendance_summary';
          $tableDetailsArray[$c1]["related_table_alias_names"] = $relatedTableDetails['table_alias_names'];//'tbl_Attendance Details,tbl_Attendance Summary';
          $tableDetailsArray[$c1]["related_table_column_relation"] = getRelatedTableColumnNames($tableAliasId);//'slt_announcement_master.title=slt_attendance_details.status,slt_announcement_master.title=slt_attendance_summary.total_attempts';
        } else {
          unset($tableDetailsArray[$c1]);
        }
        
      }
      $c1++;
    }

  }

  expDebug::dPrint('$tableDetailsArray = ' . print_r($tableDetailsArray, true) , 4);
  
  echo drupal_json_output($tableDetailsArray);
  } catch (Exception $ex) {
  	watchdog_exception('getReportTables', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * Fetch all column name from the selected tables.
 * The column names should match with the given text.
 * 
 */
function getReportTableColsAutoComplete(){
	try{
  $reportTables  = explode(",",reportXSSValidate($_GET['report_tables']));
  $tableString   = '';
  for($i=0;$i<count($reportTables);$i++)
    $tableString .= "'".$reportTables[$i]."'".",";
  $tableString  = rtrim($tableString,',');
  $colname = reportXSSValidate($_GET['z']);
  $util    = new GlobalUtil();
  $conf    = $util->getConfig();
  $db_url  = $conf["report_db_url"];
  $tmp1    = explode("@",$db_url);
  $dbname  = substr($tmp1[1],stripos($tmp1[1],"/")+1,strlen($tmp1[1]));
			
  $result = db_query("SHOW TABLES FROM ".$dbname." WHERE tables_in_".$dbname." IN (".$tableString.")");
  //expDebug::dPrint('$result = ' .print_r($result,true),3);
  $tablecolname = "tables_in_".$dbname;
  foreach ($result as $row) {
    $table = $row->$tablecolname;
    $result1 = db_query("SHOW COLUMNS FROM ".$table." FROM ".$dbname." where Field like '%".$colname."%'");
    foreach ($result1 as $row1) {
        print $table.'.'.$row1->field."\n";
    }
 }
 } catch (Exception $ex) {
 	watchdog_exception('getReportTableColsAutoComplete', $ex);
 	expertusErrorThrow($ex);
 }
}//End getReportTableColsAutoComplete()

/*
 * Fetch all column values from the selected tables.
 * The column values should match with the given text.
 * 
 */
function getReportCriteriaValuesAutoComplete(){
	try{
  $matchValue = reportXSSValidate($_GET['z']);
  $tableInfo  = explode(".",reportXSSValidate($_GET['criteria_id']));
  $select = db_select($tableInfo[0], 'tbl');
  $select->addField('tbl', $tableInfo[1]);
  $select->condition('tbl.'.$tableInfo[1], '%' . db_like($matchValue) . '%', 'LIKE');
  $qry_autocomplete = $select->execute()->fetchAll();
  foreach ($qry_autocomplete as $rec_autocomplete) {
    print $rec_autocomplete->$tableInfo[1]."\n";
  }
  } catch (Exception $ex) {
  	watchdog_exception('getReportCriteriaValuesAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}//End getReportCriteriaValuesAutoComplete()

/*
 * Fetch all report table alias names for auto complete
 * 
 */
function getReportTablesAutoComplete(){
	try{
  $matchValue = reportXSSValidate($_GET['z']);
  $gutil = new GlobalUtil();
  $expConfig = $gutil->getConfig();
  $slt_table_exten = trim($expConfig['slt_table_exten']);
  $tableQuery = db_select('slt_report_tables_alias', 'table_alias');
  $tableQuery->addField('table_alias', 'table_alias_name', 'table_alias');
  if($slt_table_exten == 0)
  	$tableQuery->condition('table_alias.table_name', 'slv_%', 'like');
  else
  $tableQuery->condition('table_alias.table_name', 'slt_%', 'like');
  	//$tableQuery->condition(db_or()->condition('table_alias.table_name', 'slv_%', 'like')->condition('table_alias.table_name', 'slt_%', 'like'));

  $tableQuery->condition('table_alias.is_visible', '1');
  $tableQuery->condition('table_alias.table_alias_name', '%' . db_like($matchValue) . '%', 'LIKE');
  $qry_autocomplete = $tableQuery->execute()->fetchAll();
  foreach ($qry_autocomplete as $rec_autocomplete) {
    print $rec_autocomplete->table_alias."\n";
  }
  } catch (Exception $ex) {
  	watchdog_exception('getReportTablesAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}//End getReportTablesAutoComplete()

 
function getRelatedTableNames($tableAliasId){
	try{
  $select = db_select('slt_report_tables_relation', 'table_relation');
  $select->innerJoin('slt_report_tables_alias', 'table_alias', 'table_relation.relation_table_id = table_alias.id');
  $select->addField('table_alias', 'table_name', 'table_name');
  $select->addField('table_alias', 'table_alias_name', 'table_alias_name');
  $select->condition('table_relation.table_id', $tableAliasId);
  $select->distinct();
  $resulQuery = $select->execute()->fetchAll();
  
  $tableDetailsArray = array();
  $tableNames = '';
  $tableAliasNames  = '';
  if(count($resulQuery)){
    foreach($resulQuery as $tableDetails){
      if($tableNames!='')
        $tableNames .= ',';
      if($tableAliasNames!='')
        $tableAliasNames .= ',';        
      
      $tableNames .= 'tbl_'.$tableDetails->table_name;  
      $tableAliasNames .= 'tbl_'.$tableDetails->table_alias_name;
    }
  }
  $tableDetailsArray['table_names']=$tableNames;
  $tableDetailsArray['table_alias_names']=$tableAliasNames;
  return $tableDetailsArray;
  } catch (Exception $ex) {
  	watchdog_exception('getRelatedTableNames', $ex);
  	expertusErrorThrow($ex);
  }
}//End of getTelatedTableNames

function getRelatedTableColumnNames($tableAliasId){
	try{
  $select = db_select('slt_report_tables_relation', 'table_relation');
  $select->innerJoin('slt_report_tables_alias', 'table_alias', 'table_relation.table_id = table_alias.id');
  $select->innerJoin('slt_report_tables_alias', 'rel_table_alias', 'table_relation.relation_table_id = rel_table_alias.id');
  $select->innerJoin('slt_report_tables_fields_alias', 'fields_alias', 'table_relation.table_field_id = fields_alias.id');
  $select->innerJoin('slt_report_tables_fields_alias', 'rel_fields_alias', 'table_relation.relation_table_field_id = rel_fields_alias.id');
  $select->addField('table_alias', 'table_name', 'table_name');
  $select->addField('rel_table_alias', 'table_name', 'rel_table_name');
  $select->addField('fields_alias', 'table_field_name', 'table_field_name');
  $select->addField('rel_fields_alias', 'table_field_name', 'rel_table_field_name');
  $select->condition('table_relation.table_id', $tableAliasId);
  $resulQuery = $select->execute()->fetchAll();
  
  $tableFieldRelations = '';
  if(count($resulQuery)){
    foreach($resulQuery as $tableDetails){
      if($tableFieldRelations!='')
        $tableFieldRelations .= ',';
      $tableFieldRelations .= $tableDetails->table_name.'.'.$tableDetails->table_field_name.'='.$tableDetails->rel_table_name.'.'.$tableDetails->rel_table_field_name;  
    }
  }
  return $tableFieldRelations;
  } catch (Exception $ex) {
  	watchdog_exception('getRelatedTableColumnNames', $ex);
  	expertusErrorThrow($ex);
  }
}//End of getRelatedTableColumnNames

/**
 * @name : getReportDatabaseOption
 * @desc : Function to get the report database. If separate database is configured
 * for report then it return the report database otherwise return the default database.
 * 
 * @param: 
 * @return array
 * 
 */
function getReportDatabaseOption(){
	try{
		$dbOption = array(
				'target'		  => 'report',
				'fetch' 		  => PDO::FETCH_OBJ,
				'return'		  => Database::RETURN_STATEMENT,
				'throw_exception' => TRUE,
		);
	return $dbOption;
	} catch (Exception $ex) {
		watchdog_exception('getReportDatabaseOption', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * reorderReportColumn() - reorder the column sql
 */
function reorderReportColumn() {
  try {
    $userId = getSltpersonUserId();
    // Added reportXSSValidate by Vincent for #0050212 on June 9, 2015
    $changedOrder = reportXSSValidate($_REQUEST['reorder']);
    $reportId = reportXSSValidate($_REQUEST['reportId']);
    $columnHeaders = getReportColumnHeader($reportId);            
    $columnAliasSql = array();
    $columnSqlDBArray = array();
    $columnAliasSqlDBArray = array();
    if (count($columnHeaders)) {
      $columnSqlDBArray = explode('~~', $columnHeaders['column_sql']);
      $columnAliasSqlDBArray = explode(',', $columnHeaders['column_alias_sql']);
    }
    $newColumnSql = array();
    $newColumnAliasSql = array();
    foreach($changedOrder as $id => $index) {
      $newColumnSql[$id] = $columnSqlDBArray[$index];
      $newColumnAliasSql[$id] =  $columnAliasSqlDBArray[$index];
    }
    $changedColumnSql = implode('~~', $newColumnSql);
    $changedColumnAliasSql = implode(',', $newColumnAliasSql);
    
    $updateQuery = db_update('slt_report_query_builder');
    $fields['column_sql'] = $changedColumnSql;
    $fields['column_alias_sql'] = $changedColumnAliasSql;
    $fields['updated_by'] = $userId;
    $fields['updated_on'] = now();
    $updateQuery->fields($fields);
    $updateQuery->condition('report_details_id', $reportId);
    $updateQuery->condition('union_query', '2', '!=');
    $updateQuery->execute();
    
    echo drupal_json_output(array('column_sql' => $changedColumnSql,
                                  'column_alias_sql' => $changedColumnAliasSql));
  }
  catch (Exception $ex) {
    watchdog_exception('reorderReportColumn', $ex);
    expertusErrorThrow($ex);  
  }
}

/**
 * Called only for Design Wizard reports
 */
function reorderColumnProcessing($columnObj) {
	try {
		expDebug::dPrint('$columnObj = ' . print_r($columnObj, true), 4);
    $reportId = $columnObj->reportId;
    $columnArray = $columnObj->columnArray;
    $columnAliasArray = $columnObj->columnAliasArray;
    if ($reportId) {
      $columnHeaders = getReportColumnHeader($reportId);            
      $columnSqlDBArray = array();
      $columnAliasSqlDBArray = array();
      if (count($columnHeaders) > 0) {
        $columnSqlDBArray = explode('~~', $columnHeaders['column_sql']);
        $columnAliasSqlDBArray = explode(',', $columnHeaders['column_alias_sql']);
      }
      $newColumnSql = array();
      $newColumnAliasSql = array();
      $cntColumnArray = count($columnArray);
      $cntColumnSqlDB = count($columnSqlDBArray);
      $cntColumn  = ($cntColumnArray > $cntColumnSqlDB) ? $cntColumnArray : $cntColumnSqlDB;
      $assignColumn = $cntColumn;
      for ($i = 0; $i < $cntColumn; $i++) {
        if (in_array($columnArray[$i], $columnSqlDBArray)) {
          $key = array_search($columnArray[$i], $columnSqlDBArray);
          $newColumnSql[$key] = $columnArray[$i];
          $newColumnAliasSql[$key] = $columnAliasArray[$i];     
        }
        else {
          $newColumnSql[$assignColumn] = $columnArray[$i];
          $newColumnAliasSql[$assignColumn] = $columnAliasArray[$i];
          $assignColumn++;
        }      
      }
      ksort($newColumnSql);
      $newColumnSql = array_values(array_filter($newColumnSql));
      ksort($newColumnAliasSql);        
      $newColumnAliasSql = array_values(array_filter($newColumnAliasSql));
      $returnCol = new stdClass();
      $returnCol->newColumnSql = $newColumnSql;
      $returnCol->newColumnAliasSql = $newColumnAliasSql;
      expDebug::dPrint('$returnCol = ' . print_r($returnCol, true), 4);
      return $returnCol;
    }
  } catch (Exception $ex) {
  	watchdog_exception('reorderColumnProcessing', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Define a custom exception class for hook_cron
 */
class ReportSchedulesException extends Exception {
  public function __construct(Exception $previous = null, $message = '', $code = 0) {
  	try{
    parent::__construct($message, $code, $previous);
    } catch (Exception $ex) {
    	watchdog_exception('ReportSchedulesException __construct', $ex);
    	expertusErrorThrow($ex);
    }
  }
}

/**
 * Make next run time string in system default timezone for saving in db.
 */
function reportScheduleMakeRunTime($year, $month, $day, $time, $timezone) {
  try {
  	$systemDefaultTimezone = date_default_timezone(FALSE);
  	expDebug::dPrint('$systemDefaultTimezone = ' . $systemDefaultTimezone, 4);

    if (empty($timezone)) {
    	$timezone = $systemDefaultTimezone;
    }
    expDebug::dPrint('$timezone = ' . $timezone, 4);

    //Initialize with 1st day of the month
    $nextRunDateTimeObj = new DateObject($year . '-' . $month . '-01 ' . $time, $timezone, 'Y-m-d H:i:s');
    expDebug::dPrint('Initialize next run date time in timezone using the first day of month = ' . date_format($nextRunDateTimeObj, 'Y-m-d H:i:s'), 4);
    // Get number of days in the month
    $lastDayOfMonth = $nextRunDateTimeObj->format('t');
    expDebug::dPrint('$lastDayOfMonth = ' . $lastDayOfMonth, 4);
    
    //Adjust day to the last day of month if it is outside the month
    if ($day > $lastDayOfMonth) {
      $day = $lastDayOfMonth;
    }
    
    // Reset date to the intended date
    $nextRunDateTimeObj->setDate($year, $month, $day);
    
    expDebug::dPrint('Next run date time in timezone = ' . date_format($nextRunDateTimeObj, 'Y-m-d H:i:s'), 4);
    
    // Get corresponding date time in the system default timezone, for saving in the DB
    if (!empty($timezone) && $timezone != $systemDefaultTimezone) {
    	expDebug::dPrint('Converting timezone of date object', 4);
    	$nextRunDateTimeObj->setTimezone(new DateTimeZone($systemDefaultTimezone));
    }

    $nextRunDateTime = date_format($nextRunDateTimeObj, 'Y-m-d H:i:s');
    expDebug::dPrint('Next run date time in system default timezone = ' . $nextRunDateTime, 4);
    
    return $nextRunDateTime;
  }
  catch (ReportSchedulesException $repSchEx) {
  	expDebug::dPrint('Rethrowing ReportSchedulesException', 4);
  	throw($repSchEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('reportScheduleMakeRunTime', $ex);
    expertusLogError($ex);
    expDebug::dPrint('Throwing new ReportSchedulesException', 4);
    throw new ReportSchedulesException($ex);
  }
}

/**
 * Splits a date time object into details such as year, month, day, day of week, hour, minute, second, number of days in the current month, etc.
 */
function reportScheduleSplitDateTimeObj($dateTimeObj)  {
  try {
    $splitDateTime = new stdClass();
    
    $dateTimeArr = $dateTimeObj->toArray();
    $splitDateTime->year = $dateTimeArr['year'];
    $splitDateTime->month = $dateTimeArr['month'];
    $splitDateTime->month_last_day = $dateTimeObj->format('t');
    $splitDateTime->day = $dateTimeArr['day'];
    $splitDateTime->weekday = date_day_of_week($dateTimeObj) + 1; // +1 to make the weekdays start from 1 instead of 0.
    $splitDateTime->hour = $dateTimeArr['hour'];
    $splitDateTime->minute = $dateTimeArr['minute'];
    $splitDateTime->second = $dateTimeArr['second'];
    $splitDateTime->time = date_format($dateTimeObj, 'H:i:s');
    $splitDateTime->time_for_compare = date_format($dateTimeObj, 'His');
    $splitDateTime->timezone = $dateTimeArr['timezone'];
    $splitDateTime->formatted = date_format($dateTimeObj, 'Y-m-d H:i:s');
    
    expDebug::dPrint('$splitDateTime = ' . print_r($splitDateTime, true), 4);
    return $splitDateTime;
  }
  catch (ReportSchedulesException $repSchEx) {
  	expDebug::dPrint('Rethrowing ReportSchedulesException', 4);
    throw($repSchEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('reportScheduleSplitDateTimeObj', $ex);
    expertusLogError($ex);
    expDebug::dPrint('Throwing new ReportSchedulesException', 4);
    throw new ReportSchedulesException($ex);
  }
}

/**
 *  Converts system default timezone date time to date time in the specified time zone. Return split date time details.
 */
function reportScheduleGetSplitTZDateTime($defaultTZDateTimeStr, $toTimezone) {
	try {

		$defaultTimezone =  date_default_timezone(FALSE);
	  $dateTimeObj = new DateObject($defaultTZDateTimeStr, $defaultTimezone, 'Y-m-d H:i:s');
	  
		
	  if (!empty($toTimezone) && $toTimezone != $defaultTimezone) {
	  	expDebug::dPrint('Converting timezone of date object', 4);
	  	$dateTimeObj->setTimezone(new DateTimeZone($toTimezone));
	  }

	  $splitDateTime = reportScheduleSplitDateTimeObj($dateTimeObj);
    $splitDateTime->datetime_obj = $dateTimeObj; // Will be used to advance by a day in daily report

    expDebug::dPrint('$splitDateTime = ' . print_r($splitDateTime, true), 4);
    return $splitDateTime;
	}
  catch (ReportSchedulesException $repSchEx) {
  	expDebug::dPrint('Rethrowing ReportSchedulesException', 4);
    throw($repSchEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('reportScheduleGetSplitTZDateTime', $ex);
    expertusLogError($ex);
    expDebug::dPrint('Throwing new ReportSchedulesException', 4);
    throw new ReportSchedulesException($ex);
  }	
}

/**
 *  Gets now date time details in the specified time zone
 */
function reportScheduleGetSplitTZNowDateTime($timezone) {
  try {
  	if (empty($timezone)) {
  		$timezone = date_default_timezone(FALSE); //Same as done in exp_sp_core.inc:now()
  	}
  	$splitDateTimeObj = date_now($timezone);
    $splitDateTime = reportScheduleSplitDateTimeObj($splitDateTimeObj);
    $splitDateTime->datetime_obj = $splitDateTimeObj; // Will be used to advance by a day in daily report
    
    expDebug::dPrint('$splitDateTime = ' . print_r($splitDateTime, true), 4);
    return $splitDateTime;
  }
  catch (ReportSchedulesException $repSchEx) {
  	expDebug::dPrint('Rethrowing ReportSchedulesException', 4);
    throw($repSchEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('reportScheduleGetSplitTZNowDateTime', $ex);
    expertusLogError($ex);
    expDebug::dPrint('Throwing new ReportSchedulesException', 4);
    throw new ReportSchedulesException($ex);
  } 
}

/**
 * Determines the first run time of a yearly schedule
 */
function getFirstRunTimeForYearlySchedule($scheduleMonth, $scheduleDay, $scheduleTime, $scheduleTZ) {
	try {
		// Get the now date time details in schedule's timezone
		$schedTZNowObj = reportScheduleGetSplitTZNowDateTime($scheduleTZ);

    // Compare months
    expDebug::dPrint("Comparing months: $scheduleMonth with $schedTZNowObj->month", 4);
    if ($scheduleMonth < $schedTZNowObj->month ) {
      expDebug::dPrint('Schedule month for this year is passed. Scheduling report in the next year', 4);
      $firstRunTime = reportScheduleMakeRunTime($schedTZNowObj->year + 1, $scheduleMonth, $scheduleDay, $scheduleTime, $scheduleTZ);
    }
    else if ($scheduleMonth == $schedTZNowObj->month) {
      expDebug::dPrint('Schedule month is same as this month.', 4);
      //Compare days
      // First, adjust schedule day based on the number of days in current month
      $adjustedScheduleDay = $scheduleDay;
      if ($adjustedScheduleDay > $schedTZNowObj->month_last_day) {
        $adjustedScheduleDay = $schedTZNowObj->month_last_day;
        expDebug::dPrint("Schedule day ($scheduleDay) adjusted to last day of current month ($schedTZNowObj->month_last_day) as beyond the month." . 4);
      }
      // Now compare
      expDebug::dPrint("Comparing days: $adjustedScheduleDay (adjusted) with $schedTZNowObj->day", 4);
      if ($adjustedScheduleDay < $schedTZNowObj->day) {
        expDebug::dPrint('Schedule day is passed. Scheduling report in the next year', 4);
        $firstRunTime = reportScheduleMakeRunTime($schedTZNowObj->year + 1, $scheduleMonth, $scheduleDay, $scheduleTime, $scheduleTZ);
      }
      else if ($adjustedScheduleDay == $schedTZNowObj->day) {
        expDebug::dPrint('Schedule day is same as today.', 4);
        // Compare times
        $schTimeForCompare = str_replace(':', '', $scheduleTime);
        expDebug::dPrint("Comparing $schTimeForCompare with $schedTZNowObj->time_for_compare", 4);
        if ($schTimeForCompare <= $schedTZNowObj->time_for_compare) {
          expDebug::dPrint('Schedule time is passed. Scheduling report in the next year', 4);
          $firstRunTime = reportScheduleMakeRunTime($schedTZNowObj->year + 1, $scheduleMonth, $scheduleDay, $scheduleTime, $scheduleTZ);
        }
        else { //$schTimeForCompare > $schedTZNowObj->time_for_comare
          expDebug::dPrint('Schedule time is in the future. Scheduling report for this year', 4);
          $firstRunTime = reportScheduleMakeRunTime($schedTZNowObj->year, $scheduleMonth, $adjustedScheduleDay, $scheduleTime, $scheduleTZ);
        }
      }
      else { // $adjustedScheduleDay > $schedTZNowObj->day
        expDebug::dPrint('Schedule day is in the future. Scheduling report for this year', 4);
        $firstRunTime = reportScheduleMakeRunTime($schedTZNowObj->year, $scheduleMonth, $scheduleDay, $scheduleTime, $scheduleTZ);
      }
    }
    else { //$scheduleMonth > $schedTZNowObj->month
      expDebug::dPrint('Schedule month is in the future. Scheduling report for this year', 4);
      $firstRunTime = reportScheduleMakeRunTime($schedTZNowObj->year, $scheduleMonth, $scheduleDay, $scheduleTime, $scheduleTZ);
    }
    
    expDebug::dPrint('$firstRunTime = ' . $firstRunTime, 4);
		return $firstRunTime;
	}
  catch (ReportSchedulesException $repSchEx) {
  	expDebug::dPrint('Rethrowing ReportSchedulesException', 4);
    throw($repSchEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('getFirstRunTimeForYearlySchedule', $ex);
    expertusLogError($ex);
    expDebug::dPrint('Throwing new ReportSchedulesException', 4);
    throw new ReportSchedulesException($ex);
  }
}

/**
 * Determines the next run time of a yearly schedule
 */
function getNextRunTimeForYearlySchedule($prevRunTimeStr, $scheduleMonth, $scheduleDay, $scheduleTime, $scheduleTZ) {
  try {
	  // Get the last run date time details in schedule's timezone.
	  $prevDTObj = reportScheduleGetSplitTZDateTime($prevRunTimeStr, $scheduleTZ); // $prevRunTimeStr is in system default timezone
	  // Advance the year by one
	  expDebug::dPrint('Advancing year to next year as per configured schedule', 4);
	  $nextRunTime = reportScheduleMakeRunTime($prevDTObj->year + 1, $scheduleMonth, $scheduleDay, $scheduleTime, $scheduleTZ);
  
    expDebug::dPrint('$nextRunTime = ' . $nextRunTime, 4);
    return $nextRunTime;
  }
  catch (ReportSchedulesException $repSchEx) {
  	expDebug::dPrint('Rethrowing ReportSchedulesException', 4);
    throw($repSchEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('getNextRunTimeForYearlySchedule', $ex);
    expertusLogError($ex);
    expDebug::dPrint('Throwing new ReportSchedulesException', 4);
    throw new ReportSchedulesException($ex);
  }
}

/**
 * Advances month to next month
 */
function reportScheduleAdvanceMonth($currentYear, $currentMonth) {
	try {
    if ($currentMonth == 12) {
    	$nextYear = $currentYear + 1;
      $nextMonth = 1;
    }
    else {
    	$nextYear = $currentYear;
      $nextMonth = $currentMonth + 1;
    }
		
    return array($nextYear, $nextMonth);
	}
  catch (ReportSchedulesException $repSchEx) {
  	expDebug::dPrint('Rethrowing ReportSchedulesException', 4);
    throw($repSchEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('reportScheduleAdvanceMonth', $ex);
    expertusLogError($ex);
    expDebug::dPrint('Throwing new ReportSchedulesException', 4);
    throw new ReportSchedulesException($ex);
  }
}

/**
 * Determines the first run time of a monthly schedule
 */
function getFirstRunTimeForMonthlySchedule($scheduleDay, $scheduleTime, $scheduleTZ) {
	try {
    // Get the now date time details in schedule's timezone
    $schedTZNowObj = reportScheduleGetSplitTZNowDateTime($scheduleTZ);

    //Compare days
    // First, adjust schedule day based on the number of days in current month
    $adjustedScheduleDay = $scheduleDay;
    if ($adjustedScheduleDay > $schedTZNowObj->month_last_day) {
      $adjustedScheduleDay = $schedTZNowObj->month_last_day;
      expDebug::dPrint("Schedule day ($scheduleDay) adjusted to last day of current month ($schedTZNowObj->month_last_day) as beyond the month.", 4);
    }
    // Now compare
    expDebug::dPrint("Comparing days: $adjustedScheduleDay (adjusted) with $schedTZNowObj->day", 4);
    if ($adjustedScheduleDay < $schedTZNowObj->day) {
      expDebug::dPrint('Schedule day is passed. Scheduling report in the next month', 4);
      list($nextYear, $nextMonth) = reportScheduleAdvanceMonth($schedTZNowObj->year, $schedTZNowObj->month);
      $firstRunTime = reportScheduleMakeRunTime($nextYear, $nextMonth, $scheduleDay, $scheduleTime, $scheduleTZ);
    }
    else if ($adjustedScheduleDay == $schedTZNowObj->day) {
      expDebug::dPrint('Schedule day is same as today.', 4);
      // Compare times
      $schTimeForCompare = str_replace(':', '', $scheduleTime);
      expDebug::dPrint("Comparing $schTimeForCompare with $schedTZNowObj->time_for_compare", 4);
      if ($schTimeForCompare <= $schedTZNowObj->time_for_compare) {
        expDebug::dPrint('Schedule time is passed. Scheduling report in the next month', 4);
        list($nextYear, $nextMonth) = reportScheduleAdvanceMonth($schedTZNowObj->year, $schedTZNowObj->month);       
        $firstRunTime = reportScheduleMakeRunTime($nextYear, $nextMonth, $scheduleDay, $scheduleTime, $scheduleTZ);
      }
      else { //$schTimeForCompare > $schedTZNowObj->time_for_comare
        expDebug::dPrint('Schedule time is in the future. Scheduling report for this month', 4);
        $firstRunTime = reportScheduleMakeRunTime($schedTZNowObj->year, $schedTZNowObj->month, $adjustedScheduleDay, $scheduleTime, $scheduleTZ);
      }
    }
    else { // $adjustedScheduleDay > $schedTZNowObj->day
      expDebug::dPrint('Schedule day is in the future. Scheduling report for this month', 4);
      $firstRunTime = reportScheduleMakeRunTime($schedTZNowObj->year, $schedTZNowObj->month, $adjustedScheduleDay, $scheduleTime, $scheduleTZ);
    }
    
    expDebug::dPrint('$firstRunTime = ' . $firstRunTime, 4);
    return $firstRunTime;
	}
	catch (ReportSchedulesException $repSchEx) {
		expDebug::dPrint('Rethrowing ReportSchedulesException', 4);
    throw($repSchEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('getFirstRunTimeForMonthlySchedule', $ex);
    expertusLogError($ex);
    expDebug::dPrint('Throwing new ReportSchedulesException', 4);
    throw new ReportSchedulesException($ex);
  }
}

/**
 * Determines the next run time of a monthly schedule
 */
function getNextRunTimeForMonthlySchedule($prevRunTimeStr, $scheduleDay, $scheduleTime, $scheduleTZ) {
  try {
    // Get the last run date time details in schedule's timezone.
    $prevDTObj = reportScheduleGetSplitTZDateTime($prevRunTimeStr, $scheduleTZ); // $prevRunTimeStr is in system default timezone
    // Advance the month by one
    list($nextYear, $nextMonth) = reportScheduleAdvanceMonth($prevDTObj->year, $prevDTObj->month);
    $nextRunTime = reportScheduleMakeRunTime($nextYear, $nextMonth, $scheduleDay, $scheduleTime, $scheduleTZ);
      
    expDebug::dPrint('$nextRunTime = ' . $nextRunTime, 4);
    return $nextRunTime;
  }
  catch (ReportSchedulesException $repSchEx) {
  	expDebug::dPrint('Rethrowing ReportSchedulesException', 4);
    throw($repSchEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('getNextRunTimeForMonthlySchedule', $ex);
    expertusLogError($ex);
    expDebug::dPrint('Throwing new ReportSchedulesException', 4);
    throw new ReportSchedulesException($ex);
  }
}

/**
 * Advances datetime to the given weekday. Returns year, month, day
 */
function reportScheduleAdvanceWeek($splitDateTimeObj, $schedWeekday) {
  try {
  	// Determine the number of days to advance by so that the report runs on the scheduled weekday
  	$currWeekday = $splitDateTimeObj->weekday;
	  if ($schedWeekday > $currWeekday) {
	    $advanceByDays = $schedWeekday - $currWeekday;
	  }
	  else { // $currWeekday >= $schedWeekday
	    $advanceByDays = 7 - ($currWeekday - $schedWeekday);
	  }
	  
	  expDebug::dPrint('$advanceByDays = ' . $advanceByDays, 4);
	  $dateTimeObj = $splitDateTimeObj->datetime_obj;
	  $dateTimeObj->add(new DateInterval('P' . $advanceByDays . 'D'));
  	
    $splitDateTime = reportScheduleSplitDateTimeObj($dateTimeObj);
    return array($splitDateTime->year, $splitDateTime->month, $splitDateTime->day);
  }
  catch (ReportSchedulesException $repSchEx) {
  	expDebug::dPrint('Rethrowing ReportSchedulesException', 4);
    throw($repSchEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('reportScheduleAdvanceWeek', $ex);
    expertusLogError($ex);
    expDebug::dPrint('Throwing new ReportSchedulesException', 4);
    throw new ReportSchedulesException($ex);
  }
}

/**
 * Determines the first run time of a weekly schedule
 */
function getFirstRunTimeForWeeklySchedule($scheduleDay, $scheduleTime, $scheduleTZ) {
  try {
    // Get the now date time details in schedule's timezone
    $schedTZNowObj = reportScheduleGetSplitTZNowDateTime($scheduleTZ);

    //Compare days
    expDebug::dPrint("Comparing days: $scheduleDay with $schedTZNowObj->weekday", 4);
    if ($scheduleDay < $schedTZNowObj->weekday) { //weekday
      expDebug::dPrint('Schedule day is passed. Scheduling report in the next week', 4);
      list($nextYear, $nextMonth, $nextDay) = reportScheduleAdvanceWeek($schedTZNowObj, $scheduleDay);
      $firstRunTime = reportScheduleMakeRunTime($nextYear, $nextMonth, $nextDay, $scheduleTime, $scheduleTZ);
    }
    else if ($scheduleDay == $schedTZNowObj->weekday) {
      expDebug::dPrint('Schedule day is same as today.', 4);
      // Compare times
      $schTimeForCompare = str_replace(':', '', $scheduleTime);
      expDebug::dPrint("Comparing $schTimeForCompare with $schedTZNowObj->time_for_compare", 4);
      if ($schTimeForCompare <= $schedTZNowObj->time_for_compare) {
        expDebug::dPrint('Schedule time is passed. Scheduling report in the next week', 4);
        list($nextYear, $nextMonth, $nextDay) = reportScheduleAdvanceWeek($schedTZNowObj, $scheduleDay);
        $firstRunTime = reportScheduleMakeRunTime($nextYear, $nextMonth, $nextDay, $scheduleTime, $scheduleTZ);
      }
      else { //$schTimeForCompare > $schedTZNowObj->time_for_comare
        expDebug::dPrint('Schedule time is in the future. Schedule day is today. Scheduling for today', 4);
        $firstRunTime = reportScheduleMakeRunTime($schedTZNowObj->year, $schedTZNowObj->month, $schedTZNowObj->day, $scheduleTime, $scheduleTZ);
      }
    }
    else { // $adjustedScheduleDay > $schedTZNowObj->day
      expDebug::dPrint('Schedule day is in the future. Scheduling report in this week', 4);
      list($nextYear, $nextMonth, $nextDay) = reportScheduleAdvanceWeek($schedTZNowObj, $scheduleDay);
      $firstRunTime = reportScheduleMakeRunTime($nextYear, $nextMonth, $nextDay, $scheduleTime, $scheduleTZ);
    }
    
    expDebug::dPrint('$firstRunTime = ' . $firstRunTime, 4);
    return $firstRunTime;
  }
  catch (ReportSchedulesException $repSchEx) {
  	expDebug::dPrint('Rethrowing ReportSchedulesException', 4);
    throw($repSchEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('getFirstRunTimeForWeeklySchedule', $ex);
    expertusLogError($ex);
    expDebug::dPrint('Throwing new ReportSchedulesException', 4);
    throw new ReportSchedulesException($ex);
  }
}

/**
 * Determines the next run time of a weekly schedule
 */
function getNextRunTimeForWeeklySchedule($prevRunTimeStr, $scheduleDay, $scheduleTime, $scheduleTZ) {
  try {
    // Get the last run date time details in schedule's timezone.
    $prevDTObj = reportScheduleGetSplitTZDateTime($prevRunTimeStr, $scheduleTZ); // $prevRunTimeStr is in system default timezone
    // Advance date by a week
    list($nextYear, $nextMonth, $nextDay) = reportScheduleAdvanceWeek($prevDTObj, $scheduleDay);
    $nextRunTime = reportScheduleMakeRunTime($nextYear, $nextMonth, $nextDay, $scheduleTime, $scheduleTZ);

    expDebug::dPrint('$nextRunTime = ' . $nextRunTime, 4);
    return $nextRunTime;
  }
  catch (ReportSchedulesException $repSchEx) {
  	expDebug::dPrint('Rethrowing ReportSchedulesException', 4);
    throw($repSchEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('getNextRunTimeForWeeklySchedule', $ex);
    expertusLogError($ex);
    expDebug::dPrint('Throwing new ReportSchedulesException', 4);
    throw new ReportSchedulesException($ex);
  }
}


/**
 * Advances datetime by a day, returns year, month, day
 */
function reportScheduleAdvanceDay($dateTimeObj) {
  try {
    $dateTimeObj->add(new DateInterval('P1D'));
    $splitDateTime = reportScheduleSplitDateTimeObj($dateTimeObj);
    return array($splitDateTime->year, $splitDateTime->month, $splitDateTime->day);
  }
  catch (ReportSchedulesException $repSchEx) {
  	expDebug::dPrint('Rethrowing ReportSchedulesException', 4);
    throw($repSchEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('reportScheduleAdvanceDay', $ex);
    expertusLogError($ex);
    expDebug::dPrint('Throwing new ReportSchedulesException', 4);
    throw new ReportSchedulesException($ex);
  }
}

/**
 * Determines the first run time of a daily schedule
 */
function getFirstRunTimeForDailySchedule($scheduleTime, $scheduleTZ) {
  try {
    // Get the now date time details in schedule's timezone
    $schedTZNowObj = reportScheduleGetSplitTZNowDateTime($scheduleTZ);

    // Compare times
    $schTimeForCompare = str_replace(':', '', $scheduleTime);
    expDebug::dPrint("Comparing $schTimeForCompare with $schedTZNowObj->time_for_compare", 4);
    if ($schTimeForCompare <= $schedTZNowObj->time_for_compare) {
      expDebug::dPrint('Schedule time is passed. Scheduling report in the next day', 4);
      list($nextYear, $nextMonth, $nextDay) = reportScheduleAdvanceDay($schedTZNowObj->datetime_obj);
      $firstRunTime = reportScheduleMakeRunTime($nextYear, $nextMonth, $nextDay, $scheduleTime, $scheduleTZ);
    }
    else { //$schTimeForCompare > $schedTZNowObj->time_for_comare
      expDebug::dPrint('Schedule time is in the future. Scheduling report in this day', 4);
      $firstRunTime = reportScheduleMakeRunTime($schedTZNowObj->year, $schedTZNowObj->month, $schedTZNowObj->day, $scheduleTime, $scheduleTZ);
    }
    
    expDebug::dPrint('$firstRunTime = ' . $firstRunTime, 4);
    return $firstRunTime;

  }
  catch (ReportSchedulesException $repSchEx) {
  	expDebug::dPrint('Rethrowing ReportSchedulesException', 4);
    throw($repSchEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('getFirstRunTimeForDailySchedule', $ex);
    expertusLogError($ex);
    expDebug::dPrint('Throwing new ReportSchedulesException', 4);
    throw new ReportSchedulesException($ex);
  }
}

/**
 * Determines the next run time of a daily schedule
 */
function getNextRunTimeForDailySchedule($prevRunTimeStr, $scheduleTime, $scheduleTZ) {
  try {
	  // Get the last run date time details in schedule's timezone.
	  $prevDTObj = reportScheduleGetSplitTZDateTime($prevRunTimeStr, $scheduleTZ); // $prevRunTimeStr is in system default timezone
	  // Advance the day by one
	  list($nextYear, $nextMonth, $nextDay) = reportScheduleAdvanceDay($prevDTObj->datetime_obj);
	  $nextRunTime = reportScheduleMakeRunTime($nextYear, $nextMonth, $nextDay, $scheduleTime, $scheduleTZ);

    expDebug::dPrint('$nextRunTime = ' . $nextRunTime, 4);
    return $nextRunTime;
  }
  catch (ReportSchedulesException $repSchEx) {
  	expDebug::dPrint('Rethrowing ReportSchedulesException', 4);
    throw($repSchEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('getNextRunTimeForDailySchedule', $ex);
    expertusLogError($ex);
    expDebug::dPrint('Throwing new ReportSchedulesException', 4);
    throw new ReportSchedulesException($ex);
  }
}

/**
 * Fetch the next run time for a report schedule
 */
function getReportScheduleNextRunTime($prevRunTimeStr, $scheduleFrequency, $scheduleMonth, $scheduleDay, $scheduleTime, $scheduleTZ) {
	try {
		// Just in case the next_run field was emptied and not set to null
	  if ($prevRunTimeStr == '0000-00-00 00:00:00') {
      $prevRunTimeStr = null;
    }

		do {
			// Initialize
			$nextRunTime = '';
			$nextRunTimeInPast = false;
			
			// Find next run time
			if (empty($prevRunTimeStr)) {
				switch ($scheduleFrequency) {
					case 'yearly':
						$nextRunTime = getFirstRunTimeForYearlySchedule($scheduleMonth, $scheduleDay, $scheduleTime, $scheduleTZ);
				    break;
				  
					case 'monthly':
						$nextRunTime = getFirstRunTimeForMonthlySchedule($scheduleDay, $scheduleTime, $scheduleTZ);
		        break;
				
					case 'weekly':
						$nextRunTime = getFirstRunTimeForWeeklySchedule($scheduleDay, $scheduleTime, $scheduleTZ);
		        break;
		
					case 'daily':
						$nextRunTime = getFirstRunTimeForDailySchedule($scheduleTime, $scheduleTZ);
		        break;
				}
			}
			else { //$prevRunTimeStr is present
			  switch ($scheduleFrequency) {
	        case 'yearly':
	          $nextRunTime = getNextRunTimeForYearlySchedule($prevRunTimeStr, $scheduleMonth, $scheduleDay, $scheduleTime, $scheduleTZ);
	          break;
	        
	        case 'monthly':
	          $nextRunTime = getNextRunTimeForMonthlySchedule($prevRunTimeStr, $scheduleDay, $scheduleTime, $scheduleTZ);
	          break;
	      
	        case 'weekly':
	          $nextRunTime = getNextRunTimeForWeeklySchedule($prevRunTimeStr, $scheduleDay, $scheduleTime, $scheduleTZ);
	          break;
	  
	        case 'daily':
	          $nextRunTime = getNextRunTimeForDailySchedule($prevRunTimeStr, $scheduleTime, $scheduleTZ);
	          break;
	      }
	      
			  // Next run time cannot be in the past
			  $nextRunDateTimeObj = new DateObject($nextRunTime, date_default_timezone(FALSE), 'Y-m-d H:i:s');
			  $nowDateTimeObj = date_now(date_default_timezone(FALSE));
        expDebug::dPrint("Comparing next run date time " . $nextRunTime . " with now time " . date_format($nowDateTimeObj, 'Y-m-d H:i:s'), 4);         
        if ($nextRunDateTimeObj <= $nowDateTimeObj) {
          $nextRunTimeInPast = true;
          expDebug::dPrint('Next run time is in past, finding the first future run time');
          $prevRunTimeStr = null; // Find the first future run datetime
        }
			}
		} while ($nextRunTimeInPast);
		
		expDebug::dPrint('$nextRunTime = ' . $nextRunTime, 4);
		return $nextRunTime;
	}
  catch (ReportSchedulesException $repSchEx) {
  	expDebug::dPrint('Rethrowing ReportSchedulesException', 4);
  	throw($repSchEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('getReportScheduleNextRunTime', $ex);
    expertusLogError($ex);
    expDebug::dPrint('Throwing new ReportSchedulesException', 4);
    throw new ReportSchedulesException($ex);
  }
}

/**
 * Returns a list of report schedules which are ready to run
 */
function getReadyToRunReportSchedules() {
	try {
		/* All fields:
		 * ('id', 'code', 'status', 'report_id', 'mail_subject', 'mail_body', 'mail_to',
     *           'attach_format', 'frequency', 'freq_month', 'freq_day', 'freq_time', 'freq_timezone', 'params', 'sort',
     *                     'last_run', 'last_run_status', 'run_time', 'next_run', 'notify_user', 'created_by', 'created_on',
     *                              'updated_by', 'updated_on', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4')
     */
		
    $select = db_select('slt_report_schedules', 'rep_sch');
    $select->leftJoin('slt_profile_list_items', 'spli_tz', 'rep_sch.freq_timezone = spli_tz.code && spli_tz.lang_code = \'cre_sys_lng_eng\'');
    $select->leftJoin('slt_report_details', 'rep_details', 'rep_sch.report_id = rep_details.id');
    
    $select->addField('rep_sch', 'id', 'id');
    $select->addField('rep_sch', 'report_id', 'report_id');  // for schedule run failure notification
    $select->addField('rep_sch', 'frequency', 'frequency');
    $select->addField('rep_sch', 'freq_month', 'freq_month');
    $select->addField('rep_sch', 'freq_day', 'freq_day');
    $select->addField('rep_sch', 'freq_time', 'freq_time');
    $select->addField('rep_sch', 'next_run', 'next_run');
    $select->addField('rep_sch', 'run_time', 'run_time');
    $select->addField('rep_sch', 'notify_user', 'notify_user');
    $select->addField('spli_tz', 'attr2', 'freq_tz_phpname');
    $select->addField('rep_sch', 'created_by', 'created_by'); // for schedule run failure notification
    $select->addField('rep_sch', 'updated_by', 'updated_by'); // for schedule run failure notification
    $select->addField('rep_sch', 'updated_on', 'schedule_last_updated_on');
    $select->addField('rep_sch', 'mail_to', 'mail_to'); // for schedule run failure notification
    $select->addField('rep_details', 'title', 'report_title'); // for schedule run failure notification    
    $select->addField('rep_sch', 'mail_subject', 'mail_subject');
    $select->addField('rep_sch', 'mail_body', 'mail_body');
    $select->addField('rep_sch', 'mail_to', 'mail_to');
    $select->addField('rep_sch', 'attach_format', 'attach_format');
    $select->addField('rep_sch', 'params', 'params');
    $select->addField('rep_sch', 'sort', 'sort');
    $select->addField('rep_sch', 'run_time', 'run_time');
    
    $select->condition('rep_sch.status', 'cre_rpt_rss_atv', '=');
    $select->condition('rep_sch.next_run', now(), '<=');
    $select->orderBy('FIELD(rep_sch.frequency, \'daily\', \'weekly\', \'monthly\', \'yearly\')', 'ASC');
    $select->orderBy('rep_sch.run_time', 'ASC');
    expDebug::dPrintDBAPI('$select', $select);
    
    $result = $select->execute()->fetchAll();
    expDebug::dPrint('$result = ' . print_r($result, true));
    
    return $result;
  }
  catch (Exception $ex) {
    watchdog_exception('getReadyToRunReportSchedules', $ex);
    expertusLogError($ex);
    expDebug::dPrint('Throwing new ReportSchedulesException', 4);
    throw new ReportSchedulesException($ex);
  }
}


/**
 * Returns a list of report schedules which are ready to run
 */
function updateReportScheduleNextRunTime($reportSchedule) {
  try {
	  expDebug::dPrint('$reportSchedule = ' . print_r($reportSchedule, true), 4);
	  $nextRunTime = getReportScheduleNextRunTime($reportSchedule->next_run,
	                                              $reportSchedule->frequency,
	                                              $reportSchedule->freq_month,
	                                              $reportSchedule->freq_day,
	                                              $reportSchedule->freq_time,
	                                              $reportSchedule->freq_tz_phpname);
    
	  $fields = array('next_run' => $nextRunTime);
	  $updateStmt = db_update('slt_report_schedules');
	  $updateStmt->condition('id', $reportSchedule->id);
	  $updateStmt->fields($fields);
	  expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
	  $updateStmt->execute();
  }
  catch (ReportSchedulesException $repSchEx) {
  	expDebug::dPrint('Rethrowing ReportSchedulesException', 4);
  	throw($repSchEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('updateReportScheduleNextRunTime', $ex);
    expertusLogError($ex);
    expDebug::dPrint('Throwing new ReportSchedulesException', 4);
    throw new ReportSchedulesException($ex);
  }
}

/**
 * Saves the schedule run status in slt_report_schedules and slt_report_schedules_history
 */
function updateReportScheduleStatus($scheduleObj, $status, $statusComment = '', $actualStartTime = null, $actualRunTime = null,
                                                $runTimeGetReportData = null, $runTimeGenerateAttachment = null, $runTimeSendMail = null) {
	$txn = db_transaction();
  try {
  	// Schedule actual start time
  	if (empty($actualStartTime)) {
  		$actualStartTime = now();
  	}
  	expDebug::dPrint('$actualStartTime = ' . $actualStartTime, 4);
  	
  	// Calculate average run time
  	$averageRunTime = $scheduleObj->run_time;
  	if (empty($averageRunTime)) {
  		$averageRunTime = 0;
  	}
  	if (empty($actualRunTime)) {
  		$actualRunTime = 0;
  	}
  	if (!empty($averageRunTime) && !empty($actualRunTime)) {
  		$averageRunTime = ($averageRunTime +  $actualRunTime) / 2;
  	}
  	else if (!empty($actualRunTime)) { // $averageRunTime is empty
  		$averageRunTime = $actualRunTime;
  	}
  	else if (empty($averageRunTime)) { // both $averageRunTime and $actualRunTime are empty
  		$averageRunTime = null;
  	}
  	expDebug::dPrint('$averageRunTime = ' . $averageRunTime, 4);
  	
  	// Prepare status comment if an exception
  	if (is_object($statusComment)) {
  		$ex = $statusComment;
  		if (get_class($ex) == 'ReportSchedulesException') {
  			$ex = $ex->getPrevious();
  		}
  		
  	  $statusComment = "Exception :\r\n";
      $statusComment .= $ex->getMessage() . "\r\n\r\n";
      $statusComment .= "Trace :\r\n";
      $statusComment .= $ex->getTraceAsString();
  	}
  	expDebug::dPrint('$statusComment = ' . $statusComment, 4);
  	
  	$updateFields = array(
  	  'last_run' => $actualStartTime,
  	  'last_run_status' => $status,
  	  'run_time' => $averageRunTime
    );
    $updateStmt = db_update('slt_report_schedules');
    $updateStmt->condition('id', $scheduleObj->id);
    $updateStmt->fields($updateFields);
    expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
    $updateStmt->execute();
    
    $insertFields = array(
      'schedule_id' => $scheduleObj->id,
      'scheduled_start_time' => $scheduleObj->next_run,
      'actual_start_time' => $actualStartTime,
      'run_time' => $actualRunTime,
      'data_time' => $runTimeGetReportData,
      'attachment_time' => $runTimeGenerateAttachment,
      'send_mail_time' => $runTimeSendMail,
      'status' => $status,
      'status_comment' => $statusComment,
      'created_by' => 0,
      'created_on' => now()
    );
    $insertStmt = db_insert('slt_report_schedules_history');
    $insertStmt->fields($insertFields);
    expDebug::dPrintDBAPI('$insertStmt', $insertStmt);
    $insertStmt->execute();
  }
  catch (Exception $ex) {
  	$txn->rollback();
    watchdog_exception('updateReportScheduleStatus', $ex);
    expertusLogError($ex);
    // Cannot die as is running from cron.php
  }
}

/**
 * 
 */
function informScheduleOwnerOfFailure($reportSchedule, $failureReason) { //$notificationCode, $objectId1,$notifyResult,$userId
	try {
		expDebug::dPrint('Informing schedule owner of failure ' , 4);
		expDebug::dPrint($reportSchedule, 4);
		
    $notificationCode = 'report_schedule_failure';
    
		// Get notification info
    $notificationTemplateInfo = getNotificationTemplateInfo($notificationCode, 'cre_sys_lng_eng');
    
    // Get notification status
    $notifyFlag = $notificationTemplateInfo['status'];
    if ($notifyFlag == 'cre_ntn_sts_atv') {
      $notifyFlag = 'Y';
    }

    // Get notification override info
    $overrideInfo = getEntityNotificationOverrideInfo($reportSchedule->report_id, 'cre_sys_obt_rpt', $notificationCode, 'Email');
    expDebug::dPrint('$overrideInfo = ', 4);
    expDebug::dPrint($overrideInfo, 4);

    // Get overriden notification status and content
    $notifyOverrideFlag = null;
    $overrideContent = '';
    if (!empty($overrideInfo)) {
      $notifyOverrideFlag = $overrideInfo['notify_flag'];
      // Both 'Y' and 'cre_ntn_sts_atv' are active statuses
      if ($notifyOverrideFlag == 'cre_ntn_sts_atv') {
        $notifyOverrideFlag = 'Y';
      }
      $overrideContent = $overrideInfo['override_content'];
    }

    // If the notification is disabled in template, do not send the notification
    if ($notifyFlag != 'Y') {
      expDebug::dPrint('Notification is disabled', 4);
      return;
    }

    //If the notification is overridden and disabled, do not send the notification
    if (!empty($notifyOverrideFlag) && $notifyOverrideFlag != 'Y') {
      expDebug::dPrint(' Notification is overridded and disabled', 4);
      return;
    }

    // Get created by / updated by user names
    $createdBy = '';
    if (!empty($reportSchedule->created_by)) {
      $result = getPersonDetails($reportSchedule->created_by, array('full_name'));
      $createdBy = $result['full_name'];
    }
    
    $updatedBy = '';
    if (!empty($reportSchedule->updated_by)) {
      $result = getPersonDetails($reportSchedule->updated_by, array('full_name'));
      $updatedBy = $result['full_name'];
    }
    
	  $notifyUserFirstName = '';
	  $notifyUserLastName = '';
	  $notifyUserFullName = '';
    if (!empty($reportSchedule->notify_user)) {
      $result = getPersonDetails($reportSchedule->notify_user, array('first_name', 'last_name', 'full_name'));
      $notifyUserFirstName = $result['first_name'];
      $notifyUserLastName = $result['last_name'];
      $notifyUserFullName = $result['full_name'];
    }
    
    // Get failed schedule's scheduled run time in schedule timezone
    $scheduledRunTime = $reportSchedule->next_run;
    $scheduleTZ = $reportSchedule->freq_tz_phpname;
    expDebug::dPrint('$scheduleTZ = ' . $scheduleTZ, 4);
    $systemDefaultTimezone = date_default_timezone(FALSE);
    expDebug::dPrint('$systemDefaultTimezone = ' . $systemDefaultTimezone, 4);
    
    if (!empty($scheduleTZ) && $scheduleTZ != $systemDefaultTimezone) {
      $scheduledRunTimeObj = new DateObject($scheduledRunTime, $systemDefaultTimezone, 'Y-m-d H:i:s');
      expDebug::dPrint('$scheduledRunTime = ' . date_format($scheduledRunTimeObj, 'Y-m-d H:i:s'), 4);
      
      // Get corresponding date time in the system default timezone
      expDebug::dPrint('Converting timezone of date object', 4);
      $scheduledRunTimeObj->setTimezone(new DateTimeZone($scheduleTZ));
      
      $scheduledRunTime = date_format($scheduledRunTimeObj, 'Y-m-d H:i:s');
      expDebug::dPrint('$scheduledRunTime in schedule timezone = ' . $scheduledRunTime, 4);
    }
    
    // Get notify user's preferred language for translating the failure reason
    $prefLang = 'en-us';
    $drupalUser = getDrupalUserForNotificationInsert($reportSchedule->notify_user, true);
    if (!empty($drupalUser) && !empty($drupalUser['drupal_lang_code'])) {
    	$prefLang = $drupalUser['drupal_lang_code'];
    }
    expDebug::dPrint('$prefLang = ' . $prefLang, 4);
    
    $notificationInfo = array();
    $notificationInfo['tokens_string'] = 'report_title>|' . $reportSchedule->report_title . '~|' .
                                         'run_time>|' . dateTimeStrToUnivShort($scheduledRunTime, true, true, true, false) . '~|' .
                                         'failure_reason>|' . t($failureReason, array(), array('langcode' => $prefLang)) . '~|' .
                                         'recipients>|' . $reportSchedule->mail_to . '~|' .
                                         'created_by>|' . $createdBy . '~|' .
                                         'updated_by>|' . $updatedBy . '~|' .
                                         'first_name>|' . $notifyUserFirstName . '~|' .
                                         'last_name>|' . $notifyUserLastName . '~|' .
                                         'full_name>|' . $notifyUserFullName;
      $notificationInfo['message_id'] = $notificationCode;
      $notificationInfo['message_type'] = 'Report Schedule Failure Notification';
      $notificationInfo['send_type'] = 'php mailer';
      $notificationInfo['lang_code'] = 'cre_sys_lng_eng';
      expDebug::dPrint('$notificationInfo = ' , 5);
      expDebug::dPrint($notificationInfo, 5);
      callCommonNotificationInsert($notificationCode, $notificationInfo, $notificationTemplateInfo, $reportSchedule->notify_user, $overrideContent);
	}
  catch (Exception $ex) {
    watchdog_exception('informScheduleOwnerOfFailure', $ex);
    expertusLogError($ex);
    // Cannot die as is running from cron.php
  }
}

/**
 * ReportsSchedule queue implementation.
 * // Code taken from default queue SystemQueue class in system.queue.inc
 * // SystemQueue class code was used instead of extending SystemQueue to add exception handling.
 */
class ReportsScheduleQueue implements DrupalReliableQueueInterface {
  /**
   * The name of the queue this instance is working with.
   *
   * @var string
   */

  public function __construct($name) {
  	// Nothing to do
  	// The table slt_report_schedules_queue is only for report_schedules queue. There is no need to save the queue name
  	try{
  		
  	} catch (Exception $ex) {
  			watchdog_exception('ReportsScheduleQueue __construct', $ex);
  			expertusErrorThrow($ex);
  		}
  }

  public function createItem($data) {
  	try {
      $insStmt = db_insert('slt_report_schedules_queue');
      $insStmt->fields(array(
        'data' => serialize($data),
        'created' => time(),
      ));
      expDebug::dPrintDBAPI('$insStmt', $insStmt);
      return (bool) $insStmt->execute();
  	}
  	catch (Exception $ex) {
  		watchdog_exception('ReportsScheduleQueue::createItem', $ex);
  		expertusLogError($ex);
  		expDebug::dPrint('Throwing new ReportSchedulesException', 4);
  		throw new ReportSchedulesException($ex);
  	}
  }

  public function numberOfItems() {
  	try {
      return db_query('SELECT COUNT(id) FROM {slt_report_schedules_queue}')->fetchField();
  	}
  	catch (Exception $ex) {
  		watchdog_exception('ReportsScheduleQueue::numberOfItems', $ex);
  		expertusLogError($ex);
  		expDebug::dPrint('Throwing new ReportSchedulesException', 4);
  		throw new ReportSchedulesException($ex);  		
  	}
  }

  public function claimItem($lease_time = 30) {
  	try {
	    // Claim an item by updating its expire fields. If claim is not successful
	    // another thread may have claimed the item in the meantime. Therefore loop
	    // until an item is successfully claimed or we are reasonably sure there
	    // are no unclaimed items left.
	    while (TRUE) {
	      $item = db_query_range('SELECT id, data FROM {slt_report_schedules_queue} q WHERE expire = 0 ORDER BY created ASC', 0, 1)->fetchObject();
	      expDebug::dPrint('$item = ' . print_r($item, true), 4);
	      if ($item) {
	        // Try to update the item. Only one thread can succeed in UPDATEing the
	        // same row. We cannot rely on REQUEST_TIME because items might be
	        // claimed by a single consumer which runs longer than 1 second. If we
	        // continue to use REQUEST_TIME instead of the current time(), we steal
	        // time from the lease, and will tend to reset items before the lease
	        // should really expire.
	        $procFields = array();
	        if (!empty($_SERVER['SCRIPT_NAME']) && basename($_SERVER['SCRIPT_NAME']) == 'clicron.php') {
            expDebug::dPrint('fetching process details', 5);
		        $currProc = getProcessDetails();
		        if (!isset($currProc->pstart) || !isset($currProc->pname)) {
		        	throw new Exception('unable to get process start time and/or name');
		        }
		        $procFields['pid'] = $currProc->pid;
		        $procFields['pstart'] = $currProc->pstart; // needed to make sure we kill the correct process when have to
		        $procFields['pname'] = $currProc->pname; // extra check to make sure we kill the correct process when have to
	        }
	        $updStmt = db_update('slt_report_schedules_queue')
	          ->fields(array('expire' => time() + $lease_time) + $procFields)
	          ->condition('id', $item->id)
	          ->condition('expire', 0);
	        expDebug::dPrintDBAPI('$updStmt', $updStmt);
	        // If there are affected rows, this update succeeded.
	        if ($updStmt->execute()) {
	        	$item->data = unserialize($item->data);
	          return $item;
	        }
	      }
	      else {
	        // No items currently available to claim.
	        return FALSE;
	      }
	    } // end while
  	}
  	catch (ReportSchedulesException $repSchEx) {
  		expDebug::dPrint('Rethrowing ReportSchedulesException', 4);
  		throw($repSchEx); //rethrow
  	}
    catch (Exception $ex) {
    	watchdog_exception('ReportsScheduleQueue::claimItem', $ex);
    	expertusLogError($ex);
    	expDebug::dPrint('Throwing new ReportSchedulesException', 4);
    	throw new ReportSchedulesException($ex);
    }
  }

  public function releaseItem($item) {
  	try {
	    $updStmt = db_update('{slt_report_schedules_queue');
	    $updStmt->fields(array(
				'pid' => null,
	      'pstart' => null,
	      'pname' => null,
	      'expire' => 0,
	    ));
	    $updStmt->condition('id', $item->id);
	    expDebug::dPrintDBAPI('$updStmt', $updStmt);
	    return $updStmt->execute();
  	}
  	catch (Exception $ex) {
  		watchdog_exception('ReportsScheduleQueue::releaseItem', $ex);
  		expertusLogError($ex);
  		expDebug::dPrint('Throwing new ReportSchedulesException', 4);
  		throw new ReportSchedulesException($ex);
  	}  	
  }

  public function deleteItem($item) {
  	try {
	    $delStmt = db_delete('{slt_report_schedules_queue');
	    $delStmt->condition('id', $item->id);
	    expDebug::dPrintDBAPI('$delStmt', $delStmt);
	    $delStmt->execute();
    }
    catch (Exception $ex) {
    	watchdog_exception('ReportsScheduleQueue::deleteItem', $ex);
    	expertusLogError($ex);
    	expDebug::dPrint('Throwing new ReportSchedulesException', 4);
    	throw new ReportSchedulesException($ex);
    }    
  }
  
  /**
   * Removes expired items from report schedules queue (slt_report_schedules_queue)
   * Called from exp_sp_lnrreports_cron (hook_cron)
   */
  public function removeExpiredItems() {
  	try {
  		$select = db_select('slt_report_schedules_queue', 'srsq');
  		$select->addfield('srsq', 'id', 'id');
  		$select->addfield('srsq', 'pid', 'pid');
  		$select->addfield('srsq', 'pname', 'pname');
  		$select->addfield('srsq', 'pstart', 'pstart');
  		$select->addfield('srsq', 'data', 'data');
  		$select->condition('srsq.expire', 0, '<>');
  		$select->condition('srsq.expire', REQUEST_TIME, '<');
  		expDebug::dPrintDBAPI('$select', $select);
  		$expiredItemsList = $select->execute()->fetchAll();
  		expDebug::dPrint('$expiredItemsList = '.print_r($expiredItemsList,true) ,4);

  		foreach ($expiredItemsList as $expiredItem) {
  			if (!empty($_SERVER['SCRIPT_NAME']) && basename($_SERVER['SCRIPT_NAME']) == 'clicron.php' && !empty($expiredItem->pid)) {
          expDebug::dPrint('fetching process details', 5);
  				$procDetails = getProcessDetails($expiredItem->pid);
  				if (isset($procDetails->pstart) && isset($procDetails->pname) &&
  						  $procDetails->pstart == $expiredItem->pstart && $procDetails->pname == $expiredItem->pname) { // Confirm before killing process
  					killProcess($expiredItem->pid);
  				}
  			}
  			
  			$schedError = 'ERR234';//Queue worker exceeded the item lease time.
        expDebug::dPrint('$schedError = ' . print_r($schedError, true), 4);
        $reportSchedule = unserialize($expiredItem->data);
        updateReportScheduleStatus($reportSchedule, 'failed', $schedError);
        expDebug::dPrint('Informing schedule owner', 4);
        informScheduleOwnerOfFailure($reportSchedule, $schedError);
  			

  			//Remove the item from the queue. the schedule will run at the new next_run time
  			$this->deleteItem($expiredItem);
  			
  		} // end foreach
  	}
  	catch (ReportSchedulesException $repSchEx) {
  		expDebug::dPrint('Rethrowing ReportSchedulesException', 4);
  		throw($repSchEx); //rethrow
  	}
  	catch (Exception $ex) {
  		watchdog_exception('ReportsScheduleQueue::removeExpiredItems', $ex);
  		expertusLogError($ex);
  		expDebug::dPrint('Throwing new ReportSchedulesException', 4);
  		throw new ReportSchedulesException($ex);
  	}
  }

  public function createQueue() {
    /* Nothing to do */
  	try{
  		
  		} catch (Exception $ex) {
  			watchdog_exception('createQueue', $ex);
  			expertusErrorThrow($ex);
  		}
  }

  public function deleteQueue() {
  	try {
	    $delStmt = db_delete('{slt_report_schedules_queue');
	    expDebug::dPrintDBAPI('$delStmt', $delStmt);
	    $delStmt->execute();
	  }
	  catch (Exception $ex) {
	  	watchdog_exception('ReportsScheduleQueue::deleteQueue', $ex);
	  	expertusLogError($ex);
	  	expDebug::dPrint('Throwing new ReportSchedulesException', 4);
	  	throw new ReportSchedulesException($ex);
	  }
  }
}

/**
 * Fetches current process pid, name & start time
 */
function getProcessDetails($pid = null) {
	try {
		if (empty($pid)) {
		  $pid = getmypid();
		}
		$procDetails = new stdClass();
		$procDetails->pid = $pid;
		$phpUname = strtolower(php_uname('s'));
		expDebug::dPrint('$phpUname = ' . print_r($phpUname, true), 4);
	  $output = array();
	  $status = 0;
	  if (stripos($phpUname, 'win') > -1) {
	    $winWmicCmd = 'WMIC.exe';
      require_once('sites/all/services/GlobalUtil.php');
      $gutil = new GlobalUtil();
      $expConfig = $gutil->getConfig();
      $configWinWmicCmd = trim($expConfig['win_wmic_cmd_fullpath']); // Windows: Full path of the windows wmic.exe command
                                                                     // Run 'where wmic' at the command prompt to know the full path
                                                                     // Default location: C:\Windows\System32\wbem\WMIC.exe
      if (!empty($configWinWmicCmd)) {
        $winWmicCmd = $configWinWmicCmd;
      }
      expDebug::dPrint('$winWmicCmd = ' . $winWmicCmd, 4);
	  	$winCmd = $winWmicCmd . " process where processid=$procDetails->pid GET name, creationdate /value";
	  	expDebug::dPrint('$winCmd = ' . $winCmd, 4);
	  	exec($winCmd, $output, $status);
	  	// If the process id does not exist, the wmic command returns "No Instance(s) available" and $status is 0.
	  	// If command does not exist, $status 1 is returned with empty output
	  	if ($status != 0) {
	  		throw new Exception('wmic command does not exist');
	  	}
	  }
	  else if ($phpUname == 'linux') {
	  	$linuxCatCmd = 'cat';
      require_once('sites/all/services/GlobalUtil.php');
      $gutil = new GlobalUtil();
      $expConfig = $gutil->getConfig();
      $configLinuxCatCmd = trim($expConfig['linux_cat_cmd_fullpath']); // Linux: Full path of the cat command
                                                                       //        Run 'which cat' in the terminal to know the full path
                                                                       // Default location: /bin/cat
      if (!empty($configLinuxCatCmd)) {
        $linuxCatCmd = $configLinuxCatCmd;
      }
      expDebug::dPrint('$linuxCatCmd = ' . $linuxCatCmd, 4);
	  	$linuxCmdCheckCat = $linuxCatCmd . ' /dev/null';
	  	passthru ($linuxCmdCheckCat, $status);
	  	// If command does not exist, $status 1 is returned
	  	if ($status != 0) {
	  		throw new Exception('cat command does not exist');
	  	}
	  	$linuxCmd = $linuxCatCmd . ' /proc/' . $procDetails->pid . '/stat | awk \'{printf("CreationDate=%s\nName=%s\n", $22, $2)}\'';
	  	expDebug::dPrint('$linuxCmd = ' . $linuxCmd, 4);
	    exec($linuxCmd, $output, $status);
	    // If awk command does not exist, $status 1 is returned
	    if ($status != 0) {
	    	throw new Exception('awk command does not exist');
	    }
	    // If the process id does not exist, the cat command fails with a message on stderr 'No such file or directory' but nothing is output on stdout.
	    // Since awk succeeds without input, an empty $output is returned with $status 0.
	  }
	  else {
	  	throw new Exception('Unsupported operating system');
	  }
	  
	  expDebug::dPrint('$output = ' . print_r($output, true), 4);
	  expDebug::dPrint('$status = ' . print_r($status, true), 4);
	  
	  // Process output
	  if ($status == 0 && !empty($output)) {
	  	foreach ($output as $line) {
	  		$line = trim($line);
	  		if (empty($line)) {
	  			expDebug::dPrint('empty line');
	  			continue;
	  		}
	  		
	  		expDebug::dPrint('$line = ' . $line, 4);
	  		$splitLine = explode('=', $line);
	  	  $key = trim($splitLine[0]);
	  	  $value = trim($splitLine[1]);
	  			
	  	  $value = trim($value);
	  	  if ($key == 'CreationDate') {
	  	    $procDetails->pstart = $value;
	  	  }
	  	  else if ($key == 'Name') {
	  	    $procDetails->pname = $value;
	  	  }
	  	}
	  }
	  
	  expDebug::dPrint('$procDetails = ' . print_r($procDetails, true), 4);
	  return $procDetails;
  }
  catch (Exception $ex) {
    watchdog_exception('getProcessDetails', $ex);
    expertusLogError($ex);
    expDebug::dPrint('Throwing new ReportSchedulesException', 4);
    throw new ReportSchedulesException($ex);
  }
}

/**
 * Hard kills a process
 * Source: https://gist.github.com/lsauer/6476476
 */
function killProcess($pid) {
	try {
		$status = 0;
		$phpUname = strtolower(php_uname('s'));
		expDebug::dPrint('$phpUname = ' . print_r($phpUname, true), 4);		
		if (stripos($phpUname, 'win') > -1) {
      $winTaskkillCmd = 'taskkill.exe';
      require_once('sites/all/services/GlobalUtil.php');
      $gutil = new GlobalUtil();
      $expConfig = $gutil->getConfig();
      $configWinTaskkillCmd = trim($expConfig['win_taskkill_cmd_fullpath']); // Windows: Full path of the windows taskkill.exe command
                                                                             // Run 'where taskkill' at the command prompt to know the full path
                                                                             // Default location: C:\Windows\System32\taskkill.exe
      if (!empty($configWinTaskkillCmd)) {
        $winTaskkillCmd = $configWinTaskkillCmd;
      }
      expDebug::dPrint('$winTaskkillCmd = ' . $winTaskkillCmd, 4);
			
			$killCmd = $winTaskkillCmd . " /F /PID $pid";
		}
		else {
			$linuxKillCmd = 'kill';
      require_once('sites/all/services/GlobalUtil.php');
      $gutil = new GlobalUtil();
      $expConfig = $gutil->getConfig();
      $configLinuxKillCmd = trim($expConfig['linux_kill_cmd_fullpath']); // Linux: Full path of the kill command
                                                                         //        Run 'which kill' in the terminal to know the full path
                                                                         // Default location: /bin/kill
      if (!empty($configLinuxKillCmd)) {
        $linuxKillCmd = $configLinuxKillCmd;
      }
      expDebug::dPrint('$linuxKillCmd = ' . $linuxKillCmd, 4);
			$killCmd = $linuxKillCmd . " -9 $pid"; 
		}
		expDebug::dPrint('$killCmd = ' . $killCmd, 4);
		exec($killCmd);

		// If pid was for an existing process, it should have been killed by above.
	}
	catch (Exception $ex) {
		watchdog_exception('killProcess', $ex);
		expertusLogError($ex);
		expDebug::dPrint('Throwing new ReportSchedulesException', 4);
		throw new ReportSchedulesException($ex);
	}
};

/**
 * Inserts ready to run schedules in report schedules queue
 */
function insertReadyToRunSchedulesInQueue($reportsQueue) {
	try {
	  $reportSchedulesList = getReadyToRunReportSchedules(); //throws ReportScheduleLogException
	  foreach ($reportSchedulesList as $reportSchedule) {
		  try {
			  // First step is to update the next_run time of the schedule
			  updateReportScheduleNextRunTime($reportSchedule); //throws ReportScheduleLogException
	
			  // Place schedule in the queue
			  $reportsQueue->createItem($reportSchedule); //throws ReportScheduleLogException
		  }
		  catch (ReportSchedulesException $repSchEx) {
			  watchdog_exception('exp_sp_lnrreports_cron schedule ' . $reportSchedule->id, $repSchEx);
			  updateReportScheduleStatus($reportSchedule, 'failed', $repSchEx);
			  expDebug::dPrint('Informing schedule owner', 4);
			  $msgForOwner = 'ERR231';//System was unable to generate the report due to an internal error.
			  informScheduleOwnerOfFailure($reportSchedule, $msgForOwner);
		  }
		  catch (Exception $ex) {
			  watchdog_exception('exp_sp_lnrreports_cron schedule ' . $reportSchedule->id, $ex);
			  expertusLogError($ex);
  		  updateReportScheduleStatus($reportSchedule, 'failed', $ex);
			  expDebug::dPrint('Informing schedule owner', 4);
			  $msgForOwner = 'ERR231';//System was unable to generate the report due to an internal error.
			  informScheduleOwnerOfFailure($reportSchedule, $msgForOwner);
		  }
	  } // end foreach
	}
	catch (ReportSchedulesException $repSchEx) {
    expDebug::dPrint('Rethrowing ReportSchedulesException', 4);
  	throw($repSchEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('insertReadyToRunSchedulesInQueue', $ex);
    expertusLogError($ex);
    expDebug::dPrint('Throwing new ReportSchedulesException', 4);
    throw new ReportSchedulesException($ex);
  }
}

/**
 * Define a custom exception class for queue worker
 */
class ReportSchedulesWorkerException extends Exception {
  private $informOwner;
  private $msgForOwner;
  public function __construct(Exception $previous = null, $informOwner = false, $msgForOwner = '', $code = 0) {
  	try{
    $this->informOwner = $informOwner;
    if ($this->informOwner === 'INFORM-OWNER') {
    	$this->informOwner = true;
    }
    $this->msgForOwner = $msgForOwner;
    parent::__construct('', $code, $previous);
    } catch (Exception $ex) {
    	watchdog_exception('ReportSchedulesWorkerException __construct', $ex);
    	expertusErrorThrow($ex);
    }
  }

  public function informOwner() {
  	try{
    return $this->informOwner;
    } catch (Exception $ex) {
    	watchdog_exception('informOwner', $ex);
    	expertusErrorThrow($ex);
    }
  }
  
  public function getMsgForOwner() {
  	try{
    return $this->msgForOwner;
    } catch (Exception $ex) {
    	watchdog_exception('getMsgForOwner', $ex);
    	expertusErrorThrow($ex);
    }
  }
}

/**
 * Converts narrow search result to pdf string
 */
function convertNarrowSearchResultsToPDFString(&$reportData, $colHeadersMap, $filterArray, $reportHeader) {
	try {
		
	  $mpdf = createMpdfObjectForNarrowSearchResults($reportData, $reportHeader, $filterArray, $colHeadersMap, 6);
	  $pdfContent = $mpdf->Output('test.pdf', 'S');
	  
    // Find byte length
    $pdfContentLen = strlen($pdfContent);
    expDebug::dPrint('$pdfContentLen with strlen = ' . $pdfContentLen, 4);
    
    return array($pdfContent, $pdfContentLen);
	}
  catch (ReportSchedulesWorkerException $repSchWrkEx) {
    expDebug::dPrint('Rethrowing ReportSchedulesWorkerException', 4);
    throw($repSchWrkEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('convertNarrowSearchResultsToPDFString', $ex);
    expertusLogError($ex);
    expDebug::dPrint('Throwing new ReportSchedulesWorkerException', 4);
    throw new ReportSchedulesWorkerException($ex, 'INFORM-OWNER');
  }
}

/**
 * Fetches the maximum permitted number of records in a report attachment
 */
function getAttachmentFormatMaxRecords($format) {
	try {
	  switch ($format) {
      case 'csv' :
        $recLimit = 5000; //default when not defined
        $expConfigVar = 'csv_max_records'; // exp_sp.ini configuration variable
        break;
      
      case 'pdf' :
        $recLimit = 500; //default when not defined
        $expConfigVar = 'pdf_max_records'; // exp_sp.ini configuration variable
      	break;
        
      default:
        throw new Exception('Unsupported scheduled report attachment format');
        break;
    }
    
    require_once('sites/all/services/GlobalUtil.php');
    $gutil = new GlobalUtil();
    $expConfig = $gutil->getConfig();
    $configRecLimit = trim($expConfig[$expConfigVar]) + 0;
  
    if (!empty($configRecLimit)) {
      $recLimit = $configRecLimit;
    }
    
    expDebug::dPrint('$recLimit = ' . $recLimit, 4);
    return $recLimit;
	}
  catch (Exception $ex) {
    watchdog_exception('getAttachmentFormatMaxRecords', $ex);
    expertusLogError($ex);
    expDebug::dPrint('Throwing new ReportSchedulesWorkerException', 4);
    throw new ReportSchedulesWorkerException($ex, 'INFORM-OWNER');
  }
}

/**
 * Prepares and adds to exp_email queue a scheduled report mail object
 */
function repSchQueueEmailObject($reportSchedule, $tmpFileFullPath, $attachmentNo, $numAttachments, $startTimeObj, $runTimeDbQuery, $startTimeObjGenAttach) {
	try {
    $attachmentSize = filesize($tmpFileFullPath);

    // Add an upper limit to attachment size, throw exception if exceeds
    $maxAttachmentSize = 10 * 1024 * 1024; // 10 MB default when max_email_attachment_size is not defined in exp_sp.ini
      
    // Allow override through exp_sp.ini
    require_once('sites/all/services/GlobalUtil.php');
    $gutil = new GlobalUtil();
    $expConfig = $gutil->getConfig();
    $configMaxAttachmentSize = trim($expConfig['max_email_attachment_size']); //in kilo bytes
    if (!empty($configMaxAttachmentSize)) {
      $maxAttachmentSize = $configMaxAttachmentSize * 1024;
    }
      
    if ($attachmentSize > $maxAttachmentSize) {
      unlink($tmpFileFullPath);
      $msgForOwner = 'ERR238'; //Report exceeds maximum permitted size.
      expDebug::dPrint($msgForOwner . " Report size: $attachmentSize bytes. Maximum permitted attachment size: $maxAttachmentSize bytes.", 1);
      throw new ReportSchedulesWorkerException(new Exception(), 'INFORM-OWNER', $msgForOwner);
    }

    $message = new StdClass();      
    // Save schedule details and attachment data & name in email message object
    $message->schedule_details = $reportSchedule;
    $message->attachment_data = $tmpFileFullPath;
    $message->attachment_name = createAttachmentFileName($reportSchedule->report_title, $attachmentNo, $numAttachments, $startTimeObj, $reportSchedule->freq_tz_phpname);
    $message->attachment_no = $attachmentNo;
    $message->total_attachments = $numAttachments;
    if ($attachmentNo == $numAttachments) {
      // Get worker end time and calculate the run time taken in seconds.
      $endTimeObj = date_now(date_default_timezone(FALSE));
      $message->total_run_time = $endTimeObj->getTimestamp() - $startTimeObj->getTimestamp(); // in seconds         
      // Save other attributes for logging on save
      $message->start_time = date_format($startTimeObj, 'Y-m-d H:i:s');
      $message->run_time_report_data = $runTimeDbQuery;
      $message->run_time_gen_attachment = $endTimeObj->getTimestamp() - $startTimeObjGenAttach->getTimestamp(); // in seconds
    }
    // Save send email function name in message
    $message->send = 'emailScheduledReport';      
    // Queue the message
    $mailQueue = DrupalQueue::get('exp_email');
    $mailQueue->createItem($message);
  }
  catch (ReportSchedulesWorkerException $repSchWrkEx) {
    expDebug::dPrint('Rethrowing ReportSchedulesWorkerException', 4);
    throw($repSchWrkEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('repSchQueueEmailObject', $ex);
    if (file_exists($tmpFileFullPath)) {
      unlink($tmpFileFullPath);
    }
    expertusLogError($ex);
    expDebug::dPrint('Throwing new ReportSchedulesWorkerException', 4);
    throw new ReportSchedulesWorkerException($ex, 'INFORM-OWNER');
  }
}

/**
 * Prepares and adds to exp_email queue email(s) with pdf attachments
 */
function reportScheduleEmailPDFRecords($data, $numRecords, $reportSchedule, $reportHeader, $colHeadersMap, $filterPdfArray,
                                                               $startTimeObj, $runTimeDbQuery, $offset=0, $recCount=0) {
  try {
    $startTimeObjGenAttach = date_now(date_default_timezone(FALSE));
    $tmpFolder = 'tmp';
    $maxRecords = getAttachmentFormatMaxRecords($reportSchedule->attach_format);
    expDebug::dPrint('$maxRecords = ' . $maxRecords, 4);
    $maxLimitRecords = getConfigValue('query_max_records_limit');
    if(empty($maxLimitRecords)){
    	$maxLimitRecords = 100000;
    }
    if($recCount!=0){
    	$numAttachments=ceil($recCount / $maxRecords);
    }else{
    	$numAttachments = ceil($numRecords / $maxRecords);
    }
    expDebug::dPrint('$numAttachments = ' . $numAttachments, 4);
    
    $count = 0;
    expDebug::dPrint('$$offset reportScheduleEmailCSVRecords = ' . $offset, 4);
    if($offset!=0){
    	$attachmentNo = ($offset / $maxRecords);
    }
    else{
    	$attachmentNo = 0;
    }
    expDebug::dPrint('$$$attachmentNo reportScheduleEmailCSVRecords = ' . $attachmentNo, 4);
    
    
    foreach ($data as $dataRow) {
      if ($count == 0) {
        $attachmentNo += 1; // Increment attachment number
        $attachmentHeader = $reportHeader;
        if ($numAttachments > 1) {
          $attachmentHeader .= ' (' . ($attachmentNo) . ' / ' . $numAttachments . ')';
        }
        $content = theme('search-results-pdf', array('title' => $attachmentHeader,
                                                     'applied_filters' => $filterPdfArray,
                                                     'header_field_map' => $colHeadersMap,
                                                     'font_size' => 6, //6pt
                                                     'only_header' => true));
        $mpdf = createMpdfObject('P', $reportHeader); // portrait
        $mpdf->WriteHTML($content, 2, true, false);
      }

      $mpdf->WriteHTML('<tr>', 2, false, false);
      foreach ($colHeadersMap as $hlabel => $hdetails) {
       //expDebug::dPrint("hlabel = $hlabel, hdetails = $hdetails, dataRow->hdetails[0] = " .($dataRow->$hdetails[0]), 4);
        if(property_exists($dataRow,$hdetails[0])===FALSE){
        	$vars = get_object_vars($dataRow);
        	foreach($vars as $key => $value) {
        		if($hdetails[0] == strtolower($key)) {
        			$hdetails[0] =$key;
        		}
        	}
        }
        $mpdf->WriteHTML('<td align="left" valign="top">' . $dataRow->$hdetails[0] . '</td>', 2, false, false);
      } // foreach
      $mpdf->WriteHTML('</tr>', 2, false, false);
      $count += 1;
      
      if ($count == $maxRecords) {
        $content = theme('search-results-pdf', array('only_footer' => true));
        $mpdf->WriteHTML($content, 2, false, true);
        unset($content);
        
        $tmpFileFullPath = $_SERVER['DOCUMENT_ROOT'] . '/' . $tmpFolder . '/' . uniqid('rep_sch_tmp_' . $attachmentNo . '_' . $numAttachments . '_', true) . '.pdf';
        expDebug::dPrint('$tmpFileFullPath = ' . $tmpFileFullPath, 4);     
        $mpdf->Output($tmpFileFullPath, 'F');
        unset($mpdf);
        
        // Generate email message object for exp_email queue and add to queue
        repSchQueueEmailObject($reportSchedule, $tmpFileFullPath, $attachmentNo, $numAttachments, $startTimeObj, $runTimeDbQuery, $startTimeObjGenAttach);

        // Reset count
        $count = 0;
      }
    } //end foreach
    
    if ($count > 0) { // Last attachment with less than max records
      $content = theme('search-results-pdf', array('only_footer' => true));
      $mpdf->WriteHTML($content, 2, false, true);
      unset($content);
        
      $tmpFileFullPath = $_SERVER['DOCUMENT_ROOT'] . '/' . $tmpFolder . '/' . uniqid('rep_sch_tmp_' . $attachmentNo . '_' . $numAttachments . '_', true) . '.pdf';
      expDebug::dPrint('$tmpFileFullPath = ' . $tmpFileFullPath, 4);     
      $mpdf->Output($tmpFileFullPath, 'F');
      unset($mpdf);
        
    	// Generate email message object for exp_email queue and queue it
      repSchQueueEmailObject($reportSchedule, $tmpFileFullPath, $attachmentNo, $numAttachments, $startTimeObj, $runTimeDbQuery, $startTimeObjGenAttach);
   }
  }
  catch (ReportSchedulesWorkerException $repSchWrkEx) {
    expDebug::dPrint('Rethrowing ReportSchedulesWorkerException', 4);
    throw($repSchWrkEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('reportScheduleEmailPDFRecords', $ex);
    expertusLogError($ex);
    if (!empty($tmpFileFullPath) && file_exists($tmpFileFullPath)) {
      unlink($tmpFileFullPath);
    }
    expDebug::dPrint('Throwing new ReportSchedulesWorkerException', 4);
    throw new ReportSchedulesWorkerException($ex, 'INFORM-OWNER');
  }
}

/**
 * Prepares and adds to exp_email queue email(s) with csv attachments
 */
function reportScheduleEmailCSVRecords($data, $numRecords, $reportSchedule, $reportHeader, $colHeadersMap, $filterArray, $startTimeObj, $runTimeDbQuery, $offset=0, $recCount=0) {
  try {
    $startTimeObjGenAttach = date_now(date_default_timezone(FALSE));
    $tmpFolder = 'tmp';
    $maxRecords = getAttachmentFormatMaxRecords($reportSchedule->attach_format);
    expDebug::dPrint('$maxRecords = ' . $maxRecords, 4);
    $maxLimitRecords = getConfigValue('query_max_records_limit');
    if(empty($maxLimitRecords)){
    	$maxLimitRecords = 100000;
    }
    if($recCount!=0){
    	$numAttachments=ceil($recCount / $maxRecords);
    }else{
    	$numAttachments = ceil($numRecords / $maxRecords);
    }
    expDebug::dPrint('$numAttachments = ' . $numAttachments, 4);
      
    $count = 0;
    expDebug::dPrint('$$offset reportScheduleEmailCSVRecords = ' . $offset, 4);
    if($offset!=0){
    	$attachmentNo = ($offset / $maxRecords);
    }
    else{
    	$attachmentNo = 0;
    }
    expDebug::dPrint('$$$attachmentNo reportScheduleEmailCSVRecords = ' . $attachmentNo, 4);
    foreach ($data as $dataRow) {
      if ($count == 0) {
        $attachmentNo += 1; // Increment attachment number
        $attachmentHeader = $reportHeader;
        if ($numAttachments > 1) {
          $attachmentHeader .= ' (' . ($attachmentNo) . ' / ' . $numAttachments . ')';
        }
        list($csvContent, $csvDataCols) = convertFiltersHeadersToCSVString($colHeadersMap, $filterArray, $attachmentHeader);
        $csvContent = chr(255) . chr(254) . mb_convert_encoding($csvContent, 'UTF-16LE', 'UTF-8');        
        $tmpFileFullPath = $_SERVER['DOCUMENT_ROOT'] . '/' . $tmpFolder . '/' . uniqid('rep_sch_tmp_' . $attachmentNo . '_' . $numAttachments . '_', true) . '.csv';
        expDebug::dPrint('$tmpFileFullPath = ' . $tmpFileFullPath, 4);
        $tmpFile = fopen($tmpFileFullPath, 'wb');
        fwrite($tmpFile, $csvContent);
      }
          
      $csvDataRow = array();            
      if (empty($csvDataCols)) {
        foreach ($dataRow as $fieldValue) {
          $csvDataRow[] = $fieldValue;
        } // foreach
      }
      else {
        // Prepare csv file row based on user specified data columns in $colHeadersMap
        foreach ($csvDataCols as $dataColIdx) {
        	if(property_exists($dataRow,$dataColIdx)===FALSE){
        		$vars = get_object_vars($dataRow);
	        	foreach($vars as $key => $value) {
    	    		if($dataColIdx == strtolower($key)) {
        				$dataColIdx =$key;
        			}
	        	}
        	}
          $csvDataRow[] = $dataRow->$dataColIdx;
        } // foreach
      }
      expDebug::dPrint('$csvDataRow = ' . print_r($csvDataRow, true), 4);
            
      // Generate the csv data row
      $csvContent = array_to_CSV($csvDataRow);
      $csvContent = mb_convert_encoding($csvContent, 'UTF-16LE', 'UTF-8');
      fwrite($tmpFile, $csvContent);
      $count += 1;
      
      if ($count == $maxRecords) {
        fclose($tmpFile);
        // Generate email message object for exp_email queue and queue it
        repSchQueueEmailObject($reportSchedule, $tmpFileFullPath, $attachmentNo, $numAttachments, $startTimeObj, $runTimeDbQuery, $startTimeObjGenAttach);

        // Reset count
        $count = 0;
      }
    } //end foreach
    
    if ($count > 0) { // Last attachment with less than max records
      fclose($tmpFile);
      // Generate email message object for exp_email queue
      repSchQueueEmailObject($reportSchedule, $tmpFileFullPath, $attachmentNo, $numAttachments, $startTimeObj, $runTimeDbQuery, $startTimeObjGenAttach);
    }
  }
  catch (ReportSchedulesWorkerException $repSchWrkEx) {
    expDebug::dPrint('Rethrowing ReportSchedulesWorkerException', 4);
    throw($repSchWrkEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('reportScheduleEmailCSVRecords', $ex);
    expertusLogError($ex);
    if (!empty($tmpFileFullPath) && file_exists($tmpFileFullPath)) {
      unlink($tmpFileFullPath);
    }
    expDebug::dPrint('Throwing new ReportSchedulesWorkerException', 4);
    throw new ReportSchedulesWorkerException($ex, 'INFORM-OWNER');
  }
}

/**
 * Prepares and adds to exp_email queue email with no records attachment (for both csv and pdf)
 */
function reportScheduleEmailNoRecords($reportSchedule, $reportHeader, $colHeadersMap, $filterArray, $filterPdfArray, $startTimeObj, $runTimeDbQuery) {
	try {
		$tmpFolder = 'tmp';
		$startTimeObjGenAttach = date_now(date_default_timezone(FALSE));
	  if ($reportSchedule->attach_format == 'csv') {
      list($csvContent, $csvDataCols) = convertFiltersHeadersToCSVString($colHeadersMap, $filterArray, $reportHeader);
      $csvContent .= t('MSG278', array(),  array('langcode' => 'en-us')) . "\r\n"; //There are no records.
      $csvContent = chr(255) . chr(254) . mb_convert_encoding($csvContent, 'UTF-16LE', 'UTF-8');      	
      $tmpFileFullPath = $_SERVER['DOCUMENT_ROOT'] . '/' . $tmpFolder . '/' . uniqid('rep_sch_tmp_', true) . '.csv';
      expDebug::dPrint('$tmpFileFullPath = ' . $tmpFileFullPath, 4);
      $tmpFile = fopen($tmpFileFullPath, 'wb');
      fwrite($tmpFile, $csvContent);
      fclose($tmpFile);        
	  } 
    else { // pdf
      $content = theme('search-results-pdf', array('title' => $title,
                                                   'applied_filters' => $filterPdfArray,
                                                   'header_field_map' => $colHeadersMap,
                                                   'font_size' => 6,
                                                   'only_header' => true));
      $mpdf = createMpdfObject($orientation, $title);
      $mpdf->WriteHTML($content, 2, true, false);
      $content =
        '<tr>' .
          '<td align="left" valign="top" colspan="' . count($colHeadersMap) . '" align="center">' . t('MSG278') . '</td>' . //There are no records.
        '</tr>';
      expDebug::dPrint('$content = ' . $content, 4);
      $mpdf->WriteHTML($content, 2, false, false);
      $content = theme('search-results-pdf', array('only_footer' => true));
      $mpdf->WriteHTML($content, 2, false, true);
      unset($content);
      $tmpFileFullPath = $_SERVER['DOCUMENT_ROOT'] . '/' . $tmpFolder . '/' . uniqid('rep_sch_tmp_', true) . '.pdf';
      expDebug::dPrint('$tmpFileFullPath = ' . $tmpFileFullPath, 4);     
      $mpdf->Output($tmpFileFullPath, 'F');
      unset($mpdf);
    }

    // Generate email message object for exp_email queue
    repSchQueueEmailObject($reportSchedule, $tmpFileFullPath, 1, 1, $startTimeObj, $runTimeDbQuery, $startTimeObjGenAttach);
  }
  catch (ReportSchedulesWorkerException $repSchWrkEx) {
    expDebug::dPrint('Rethrowing ReportSchedulesWorkerException', 4);
    throw($repSchWrkEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('reportScheduleEmailNoRecords', $ex);
    expertusLogError($ex);
    if (!empty($tmpFileFullPath) && file_exists($tmpFileFullPath)) {
      unlink($tmpFileFullPath);
    } 
    expDebug::dPrint('Throwing new ReportSchedulesWorkerException', 4);
    throw new ReportSchedulesWorkerException($ex, 'INFORM-OWNER');
  }
}

/**
 * Generates and queues emails with attachments
 */
function generateAndQueueAttachmentEmails($reportSchedule, $data, $reportHeader, $colHeadersMap, $filterArray, $filterPdfArray,
                                                    $startTimeObj, $runTimeDbQuery,$offset=0, $recCount=0) {
	try {
		require_once(drupal_get_path('module', 'exp_sp_administration') . '/exp_sp_administration.inc');
		
		// Determine the number of attachments
		$numRecords = count($data);
		expDebug::dPrint('$numRecords = ' . $numRecords, 4);
		expDebug::dPrint('$$offset generateAndQueueAttachmentEmails = ' . $offset, 4);
		expDebug::dPrint('$$recCount generateAndQueueAttachmentEmails = ' . $recCount, 4);
		if ($numRecords == 0) {
			reportScheduleEmailNoRecords($reportSchedule, $reportHeader, $colHeadersMap, $filterArray, $filterPdfArray, $startTimeObj, $runTimeDbQuery);
		}
		else if ($reportSchedule->attach_format == 'csv') {
			reportScheduleEmailCSVRecords($data, $numRecords, $reportSchedule, $reportHeader, $colHeadersMap, $filterArray, $startTimeObj, $runTimeDbQuery, $offset, $recCount);
		}
		else { // pdf
      reportScheduleEmailPDFRecords($data, $numRecords, $reportSchedule, $reportHeader, $colHeadersMap, $filterPdfArray, $startTimeObj, $runTimeDbQuery, $offset, $recCount);
		}
	}
  catch (ReportSchedulesWorkerException $repSchWrkEx) {
    expDebug::dPrint('Rethrowing ReportSchedulesWorkerException', 4);
    throw($repSchWrkEx); //rethrow
  }
  catch (Exception $ex) {
    watchdog_exception('generateAndQueueAttachmentEmails', $ex);
    expertusLogError($ex);
    expDebug::dPrint('Throwing new ReportSchedulesWorkerException', 4);
    throw new ReportSchedulesWorkerException($ex, 'INFORM-OWNER');
  }
}

/**
 * Sends mail with the attachment
 */
function emailScheduledReport($message) {
	try {
		$reportSchedule = $message->schedule_details;
		$attachmentNo = $message->attachment_no;
		$numAttachments = $message->total_attachments;
		
		// Capture start time
    $startTimeObj = date_now(date_default_timezone(FALSE));
		
    require_once('sites/all/commonlib/phpmailer/PHPMailerAutoload.php');
    $phpMailer = new PHPMailer();
    $phpMailer->CharSet = 'UTF-8';
    $phpMailer->setFrom(variable_get('site_mail', 'info@expertusone.com'), variable_get('site_name', 'ExpertusONE'));
    $phpMailer->addReplyTo($address);
    $phpMailer->WordWrap = 50;
    if ($numAttachments > 1) {
      $phpMailer->Subject  = $reportSchedule->mail_subject . ' (' . $attachmentNo . ' / ' . $numAttachments . ')';
    }
    else {
    	$phpMailer->Subject  = $reportSchedule->mail_subject;
    }
    $phpMailer->Body = $reportSchedule->mail_body;
    foreach (explode(',', $reportSchedule->mail_to) as $address) {
      $address = trim($address);
      if (!empty($address)) {
        $phpMailer->AddAddress($address);
      }
      else {
        throw new Exception ('Bad schedule email address list');
      }
    }
	  switch ($reportSchedule->attach_format) {
      case 'csv' :
      	$fileName = $message->attachment_name . '.csv';
        $phpMailer->addAttachment($message->attachment_data, $fileName, 'base64', 'application/vnd.ms-excel');
        break;
      
      case 'pdf' :
      	$fileName = $message->attachment_name . '.pdf';
        $phpMailer->addAttachment($message->attachment_data, $fileName, 'base64', 'application/pdf');
        break;
        
      default:
        throw new Exception('Unsupported scheduled report attachment format');
        break;
    }
    
    if(!$phpMailer->Send() && $attachmentNo == $numAttachments) { // Currently capturing result only for the last attachment
    	expDebug::dPrint('System could not email the report to recipient. ' . $phpMailer->ErrorInfo, 4);
      updateReportScheduleStatus($reportSchedule, 'failed', 'ERR235'); //System could not email the report to recipient.
      expDebug::dPrint('Informing schedule owner', 4);
      informScheduleOwnerOfFailure($reportSchedule, 'ERR235'); //System could not email the report to recipient.
    }
    else if ($attachmentNo == $numAttachments) { // Currently capturing result only after the last attachment is emailed
	    // Save status   
	    updateReportScheduleStatus($reportSchedule, 'success', '', $message->start_time, $message->total_run_time,
	                                                         $message->run_time_report_data, $message->run_time_gen_attachment);
    }
    if (file_exists($message->attachment_data)) {
      unlink($message->attachment_data);
    }
    
    // Calculate run time (for debugging only)
    $endTimeObj = date_now(date_default_timezone(FALSE));
    $emailTime = $endTimeObj->getTimestamp() - $startTimeObj->getTimestamp(); // in seconds
    expDebug::dPrint('$emailTime = ' . $emailTime, 4);
	}
  catch (Exception $ex) {
    watchdog_exception('emailScheduledReport', $ex);
    expertusLogError($ex);
    
    if (file_exists($message->attachment_data)) {
    	unlink($message->attachment_data);
    }
    
    if ($attachmentNo == $numAttachments) { // Currently capturing result only for the last attachment
	    $msgInDB = $ex;
	    $msgForOwner = 'ERR235'; //System could not email the report to recipient.
	    updateReportScheduleStatus($reportSchedule, 'failed', $msgInDB);
	    expDebug::dPrint('Informing schedule owner', 4);
	    informScheduleOwnerOfFailure($reportSchedule, $msgForOwner);
    }
  }
}

/**
 * Generates a filename for the attachmant file from the report name
 */
function createAttachmentFileName($reportTitle, $attachmentNumber, $totalAttachments, $scheduleRunTimeObj, $scheduleTZ) {
	try {
		$attachmentName = preg_replace("/[^\w\d ]/ui", '', $reportTitle); // Remove all non alpha numeric characters from the title
		$attachmentName = preg_replace("/ /ui", '_', $attachmentName); // replace space with underscore
		if ($totalAttachments > 1) {
		  $attachmentName .= '_' . $attachmentNumber . '_' . $totalAttachments;
		}

		// Convert schedule run time which is in system TZ to schedule TZ
		$scheduleRunTimeObjClone = clone $scheduleRunTimeObj; // cloning as the object is passed by reference since php5
		expDebug::dPrint('$scheduleRunTimeObjClone = ' . date_format($scheduleRunTimeObjClone, 'Y-m-d H:i:s'), 4);
    $defaultTimezone =  date_default_timezone(FALSE);
    expDebug::dPrint('$defaultTimezone = ' . print_r($defaultTimezone, true), 4);
    expDebug::dPrint('$scheduleTZ = ' . print_r($scheduleTZ, true), 4); 
		if (!empty($scheduleTZ) && $scheduleTZ != $defaultTimezone) {
      // Get corresponding date time in the schedule timzone
      $scheduleRunTimeObjClone->setTimezone(new DateTimeZone($scheduleTZ));
      expDebug::dPrint('$scheduleRunTimeObjClone after timezone conversion to scheduleTZ = ' . date_format($scheduleRunTimeObjClone, 'Y-m-d H:i:s'), 4);
    }
		
	  $attachmentName .= '_' . date_format($scheduleRunTimeObjClone, 'YmdHis');
		
		expDebug::dPrint('$attachmentName = ' . $attachmentName, 4);
		return $attachmentName;
	}
  catch (Exception $ex) {
    watchdog_exception('createAttachmentFileName', $ex);
    expertusLogError($ex);
    expDebug::dPrint('Throwing new ReportSchedulesWorkerException', 4);
    throw new ReportSchedulesWorkerException($ex, 'INFORM-OWNER');
  }
}
/*
 * get_query_string() - to get a query string with all placeholder arguments substituted with values
 */
function get_query_string($queryStr, $params) {
	$keys = array();
	$values = array();

	# build a regular expression for each parameter
	foreach ($params as $key => $value) {
		if (is_string($key)) {
			if (substr($key, 0, 1) == ':') {
				$keys[] = '/' . $key . '/';
			}
			else {
				$keys[] = '/:' . $key . '/';
			}
		}
		else {
			$keys[] = '/[?]/';
		}

		if (is_numeric($value)) {
			$values[] = intval($value);
		}
		else {
			$values[] = '"'.$value .'"';
		}
	}

	$queryStr = preg_replace($keys, $values, $queryStr, 1, $count);

	return $queryStr;
}

/**
 * Job worker for reports schedules queue
 */
function report_schedules_queue_worker($reportSchedule) {
  try {
  	# Registering shutdown function
	  register_shutdown_function('fatalErrorHandler', $reportSchedule);
    // Raise PHP max_execution_time
    // PHP set_time_limit() updates max_execution_time.
    // See http://stackoverflow.com/questions/8914257/difference-between-set-time-limit-and-ini-setmax-execution-time
    drupal_set_time_limit(4200); // 1 hour + 10 min (item lease time + cron semaphore lock max hold time + something extra to be on the safe side)
    # increase memory limit 
    ini_set('memory_limit', '512M');
    // Save worker start time
    $startTimeObj = date_now(date_default_timezone(FALSE));
    $userId=$reportSchedule->created_by;
    // createUserEntityFlatTable($userId, 23); // report sync up on every 23 hours
    expDebug::dPrint('$userId = ' . print_r($userId, true), 5);
    expDebug::dPrint('Processing $reportSchedule = ' . print_r($reportSchedule, true), 5);
    
    // Fetch the sort filter
    if (!empty($reportSchedule->sort)) {
      list($sortColumn, $sortDirection) = explode('~~', $reportSchedule->sort);
    }

    // Get SQL query details saved by the query builder
    $queryBuilderSQL = getQueryFromBuilder($reportSchedule->report_id);
    expDebug::dPrint('$queryBuilderSQL = ' . print_r($queryBuilderSQL, true), 4);
    
    // Build the common part which is common to user launched report, user csv export, user pdf export and scheduled report generation
    list($selectSQL, $queryArguments, $filterArray, $filterPdfArray, $orderByColsList) =
           buildReportQueryCommonPart($reportSchedule->report_id, $reportSchedule->params, $queryBuilderSQL, 0, 0, $startTimeObj);
    
    // Complete the query for export
    list($selectSQL, $reportHeader, $colHeadersMap,$column_cont_msg) =
           completeQueryForExport($reportSchedule->report_id, $selectSQL, $queryBuilderSQL, $orderByColsList, null, $reportSchedule->attach_format);
    
    $dao = new AbstractDAO('Report');
				$dao->connect();
				$dao->execute('SET @report_uid ='.$userId);	
				$query=get_query_string($selectSQL,$queryArguments);		
  //	$resp = $dao->query($query); 
				//$actualQuery = $resp->queryString;
				$countQuery =  "SELECT COUNT(1)  FROM (".$query.") query_alias";
				$dao->query($countQuery);
				$recCount= $dao->fetchColumn();
				expDebug::dPrint('$$RecoCount of the report query '.print_r($recCount,true), 4);
				//$maxRecords = 100000;
				$maxRecords = getConfigValue('query_max_records_limit');
				if(empty($maxRecords)){
					$maxRecords = 100000;
				}
				//getAttachmentFormatMaxRecords($reportSchedule->attach_format)
				if($recCount > $maxRecords){
					$iterations = ceil($recCount/$maxRecords);
					for($i=0;$i<$iterations;$i++){
						$offset = $i * $maxRecords;
						//TO-DO #77619, If the limit from the query is available the limitquery needs to handle that. 
						$limitQuery = $query. " LIMIT ".$maxRecords." OFFSET ". $offset;
						$resp = $dao->query($limitQuery);
						$select= $dao->fetchAllResults();
						
						
						// Prepare DatabaseStatementBase object using db_query(). Use slave db for reports
						//$select = db_query("$selectSQL", $queryArguments, getReportDatabaseOption());
						//expDebug::dPrintDBAPI('$select', $select, $queryArguments);
						
						// Calculate run time
						$endTimeObj = date_now(date_default_timezone(FALSE));
						$runTimeDbQuery = $endTimeObj->getTimestamp() - $startTimeObj->getTimestamp(); // in seconds
						expDebug::dPrint('$offset of the report query '.print_r($offset,true), 4);
						
						// Generate and queue emails with attachments
						generateAndQueueAttachmentEmails($reportSchedule, $select, $reportHeader, $colHeadersMap,
								$filterArray, $filterPdfArray, $startTimeObj, $runTimeDbQuery, $offset, $recCount);
						
					}
				}
				else{
					$resp = $dao->query($query);
					$select= $dao->fetchAllResults();
					
					
					// Prepare DatabaseStatementBase object using db_query(). Use slave db for reports
					//$select = db_query("$selectSQL", $queryArguments, getReportDatabaseOption());
					//expDebug::dPrintDBAPI('$select', $select, $queryArguments);
					
					// Calculate run time
					$endTimeObj = date_now(date_default_timezone(FALSE));
					$runTimeDbQuery = $endTimeObj->getTimestamp() - $startTimeObj->getTimestamp(); // in seconds
					
					// Generate and queue emails with attachments
					generateAndQueueAttachmentEmails($reportSchedule, $select, $reportHeader, $colHeadersMap,
							$filterArray, $filterPdfArray, $startTimeObj, $runTimeDbQuery);
					
				}
				$dao->closeconnect();

    // Restore PHP max_execution_time
    drupal_set_time_limit(240); // Restore to a lower value but give suffient time to cron.php to do any remaining tasks
    ini_set('memory_limit', '128M'); // Reset memory limit
  }
  catch (ReportSchedulesWorkerException $repSchWrkEx) {
    watchdog_exception('report_schedules_queue_worker', $repSchWrkEx);
    
    // Restore PHP max_execution_time
    drupal_set_time_limit(240); // Restore to a lower value but give suffient time to cron.php to do any remaining tasks
    ini_set('memory_limit', '128M'); // Reset memory limit
    // Exception logged in the exp_debug.txt file by the original source of exception.
    // Inform schedule owner if requested by the thrower
    if ($repSchWrkEx->informOwner()) {
    	$msgForOwner = $repSchWrkEx->getMsgForOwner();
    	if (empty($msgForOwner)) {
    		// Cause of error is unknown. Exception trace is saved in DB and the schedule owner is informed with a standard message.
    		$msgInDB = $repSchWrkEx->getPrevious();
    		$msgForOwner = 'ERR231';//System was unable to generate the report due to an internal error
    	}
    	else {	
    		$msgInDB = $msgForOwner;
    	}
    	
    	// Cause for error is known. Save the error in DB and also inform the schedule owner of the same
      updateReportScheduleStatus($reportSchedule, 'failed', $msgInDB);
      expDebug::dPrint('Informing schedule owner', 4);
      informScheduleOwnerOfFailure($reportSchedule, $msgForOwner);
    }
    
    // Cannot die as it will interrupt cron.php.
  }
  catch (Exception $ex) {
  	// A previously uncaught exception. Assuming the schedule was interrupted and schedule owner needs to be informed with the schedule status updated.
    watchdog_exception('report_schedules_queue_worker', $ex);
    
    // Restore PHP max_execution_time
    drupal_set_time_limit(240); // Restore to a lower value but give suffient time to cron.php to do any remaining tasks
    ini_set('memory_limit', '128M'); // Reset memory limit
    expertusLogError($ex);
    $msgInDB = $ex;
    $msgForOwner = 'ERR231';//System was unable to generate the report due to an internal error
    updateReportScheduleStatus($reportSchedule, 'failed', $msgInDB);
    expDebug::dPrint('Informing schedule owner', 4);
    informScheduleOwnerOfFailure($reportSchedule, $msgForOwner);

    // Cannot die as it will interrupt cron.php.
  }
}

/**
 * Fatal error handler for the report schedule worker
 */
function fatalErrorHandler($reportSchedule){
	try{
	# Getting last error
	$error = error_get_last();
  # Reset time limet
 	drupal_set_time_limit(240); // Restore to a lower value but give suffient time to cron.php to do any remaining tasks
 	ini_set('memory_limit', '128M'); // Reset memory limit
  # Checking if last error is a fatal error 
  if(($error['type'] === E_ERROR) || ($error['type'] === E_USER_ERROR)){
    # Here we handle the error, displaying HTML, logging, ...
    expDebug::dPrint($error,1);
    $msgForOwner = 'ERR231'; //System was unable to generate the report due to an internal error
    updateReportScheduleStatus($reportSchedule, 'failed', $error['message']);
    expDebug::dPrint('Informing schedule owner', 4);
    informScheduleOwnerOfFailure($reportSchedule, $msgForOwner);
    expertusLogError($error);
  }
	}catch(Exception $e){
		expDebug::dPrint("ERROR -- >",1);
		expDebug::dPrint($e,1);
		
	}
}

/**
 * Updates report schedule's next_run to now datetime so that the next cron.php executes it.
 */
function runReportSchedule($scheduleId) {
	try {
	// commented for 0049465
    $fields = array('next_run' => now());
    $updateStmt = db_update('slt_report_schedules');
    $updateStmt->condition('id', $scheduleId);
    $updateStmt->fields($fields);
    expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
    $updateStmt->execute();
    $reportid=db_query("select report_id from slt_report_schedules
						WHERE id = '".$scheduleId."'");
    
    $reportid = $reportid->fetchField();
    expDebug::dPrint('programdetails id >>> ' .print_r($reportid,true), 4);
    expertus_set_message(t('ERR227'));//Schedule is queued for execution
    refreshReportSchedulesPopup(null, array('report_id' => $reportid));
    $commands = array();
    $statusMsgHTML = theme('status_messages');
    expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true) , 4);
    if (!empty($statusMsgHTML)) {
    	$commands[] = ajax_command_html('#show_expertus_message', $statusMsgHTML);
    }
    print ajax_render($commands);
    exit;
  }
  catch (Exception $ex) {
    watchdog_exception('runReportSchedule', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Wrapper for Drupal ajax command showReportSchedulesPopup
 */
function showReportSchedulesPopup($html) {
	try{
  return array(
      'command' => 'showReportSchedulesPopup',
      'html' => $html
  );
  } catch (Exception $ex) {
  	watchdog_exception('getMsgForOwner', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Renders and opens a reports schedule popup
 */
function renderAndShowReportSchedulesPopup($reportId, $fromLaunch = false) {
	try{
		global $theme_key;
		expDebug::dPrint('$reportId = ' . print_r($reportId, true), 4);
		$popupId = 'create-schedule-qtip-'.$reportId;

		if (reportHasSchedules($reportId)) {
          $output = theme('schedule-list', array('report_id' => $reportId));
		}
		else {
			$output = theme('noitems-in-qtip', array('popup_id' => $popupId, // The id to be given to the popup
                                               'menu_path'=> 'ajax/administration/report-search/schedule-form/' . $reportId . '/0',
                                               'button_label'=> t('LBL1207'),//Schedule report
                                               'help_text' =>t('MSG694'),//There are no schedule jobs for report.Click on Create Schedule to Schedule the report
  												));
		}
		
		expDebug::dPrint('$output = ' . $output, 5);
		$output = theme('schedules-qtip', array('popup_id' => $popupId, // The id to be given to the popup
				                                     'popup_width' => '600',
				                                     'popup_height' =>($theme_key == 'expertusoneV2')? '380' : '367',
				                                     'class' => 'lnr-report-schedule-popup' . ($fromLaunch? ' from-launch' : ''), // Class(es) to be added to the popup for styling etc.
				                                     'display_html' => $output // The html to display inside the popup
		));
		
		$commands = array();
		$commands[] = showReportSchedulesPopup($output);
		
		expDebug::dPrint('$commands = ' . print_r($commands, true), 4);
		print ajax_render($commands);
		exit;
	}
	catch (Exception $ex) {
		watchdog_exception('renderAndShowReportSchedulesPopup', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Wrapper for Drupal ajax command replaceContentInReportSchedulesPopup
 */
function replaceContentInReportSchedulesPopup($html,$popupId){
	try{
  return array(
      'command' => 'replaceContentInReportSchedulesPopup',
      'html' => $html,
      'popup_id'=> $popupId,
  );
  } catch (Exception $ex) {
  	watchdog_exception('replaceContentInReportSchedulesPopup', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Wrapper for Drupal ajax command initializeVtip
 */
function initializeVtip(){
	try {
	return array(
			'command' => 'initializeVtip',
	);
	} catch (Exception $ex) {
		watchdog_exception('initializeVtip', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Builds and paints schedule addedit form in Report Schedules popup
 */
function buildAndPaintFormInReportSchedulesPopup($reportId, $scheduleId) {
	try {
		expDebug::dPrint('$reportId = ' . print_r($reportId, true), 4);
    expDebug::dPrint('$scheduleId = ' . print_r($scheduleId, true), 4);

    $form_state = array(
				'no_redirect'  => TRUE,
				'build_info' => array('args' => array($reportId, $scheduleId)),
		);
		$scheduleAddEditFormId = 'report_schedule_addedit_form';
		$builtScheduleAddEditForm = drupal_build_form($scheduleAddEditFormId, $form_state);
		$renderedScheduleAddEditForm = drupal_render($builtScheduleAddEditForm);

		$popupId = 'create-schedule-qtip-'.$reportId;
			
		$commands = array();
		$commands[] = replaceContentInReportSchedulesPopup($renderedScheduleAddEditForm, $popupId);
		expDebug::dPrint('$commands = ' . print_r($commands, true), 4);
		print ajax_render($commands);
		exit;
	}
	catch (Exception $ex) {
		watchdog_exception('buildAndPaintFormInReportSchedulesPopup', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Checks whether any active or inactive schedules exist for a report
 */
function reportHasSchedules($reportId) {
	try {
		global $user;
		if(gettype($reportId) == 'string'){
			$rep_id = explode('_',$reportId);
			$reportId = $rep_id[0];
		}
		$loggedUserName = $user->name;
		$loggedInUserId = getSltpersonUserId();
		$select = db_select('slt_report_schedules', 'sched');
		$select->addExpression('count(1)', 'cnt');
		$select->condition('sched.report_id', $reportId);
		$select->condition(db_or()->condition('sched.status','cre_rpt_rss_atv')
				                      ->condition('sched.status','cre_rpt_rss_itv'));
	  $adminUsersList = getadminUsers();
	  $adminUsersList = explode(',', $adminUsersList);
    $getUserStatus = in_array($loggedInUserId, $adminUsersList) ? true : false;
    /*if(!empty($loggedInUserId) && $getUserStatus == false && $loggedUserName != 'admin') {
    	$select->condition('sched.created_by', $loggedInUserId);
    }*/
		expDebug::dPrintDBAPI('$select = ', $select);
		$result = $select->execute()->fetchField();
		expDebug::dPrint('$result = '. $result);
		return (empty($result)? false : true);
	}
  catch (Exception $ex) {
		watchdog_exception('reportHasSchedules', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Refreshes the content of report schedules popup
 */
function refreshReportSchedulesPopup($form, $form_state) {
	try{
		$commands = array();
		$reportId = $form_state['report_id'];
		expDebug::dPrint('$reportId = ' . $reportId, 4);
		$popupId = 'create-schedule-qtip-' . $reportId;
		expDebug::dPrint('$popupId = ' . $popupId, 4);
		
		if (isset($form_state['storage']['form_validate'])) {
			expDebug::dPrint('Form validation failed. Repainting form.', 4);
			$commands[] = ajax_command_replace(NULL, drupal_render($form));
			$commands[] = initializeVtip();
		} 
		else {
			expDebug::dPrint('Schedule record updated in DB. Repainting popup', 4);
			if(reportHasSchedules($reportId)) {
				$output = theme('schedule-list' , array('report_id' => $reportId));
			}
			else {
				$output = theme('noitems-in-qtip', array('popup_id' => $popupId, // The id to be given to the popup
						'menu_path'=> 'ajax/administration/report-search/schedule-form/' . $reportId . '/0',
						'button_label'=> t('LBL1207'),//Schedule report,
						'help_text' => t('MSG694'),//There are no schedule jobs for report.Click on Create Schedule to Schedule the report,
				));
			}
			$commands[] = replaceContentInReportSchedulesPopup($output, $popupId);			
		}
		
		// Show any status messages
		$statusMsgHTML = theme('status_messages');
		expDebug::dPrint('$statusMsgHTML = ' . print_r($statusMsgHTML, true) , 4);
		if (!empty($statusMsgHTML)) {
			$commands[] = ajax_command_html('#show_expertus_message', $statusMsgHTML);
		}
		
		expDebug::dPrint('$commands = ' . print_r($commands, true), 4);
		print ajax_render($commands);
		exit;
	}
	catch (Exception $ex) {
		watchdog_exception('refreshReportSchedulesPopup', $ex);
		expertusErrorThrow($ex);
	}

}

/**
 * Builds the frequency dropdown options list
 */
function getFrequencyOptions(){
	try{
	return array('yearly' => t('LBL1195'),//Yearly
	             'monthly'=> t('LBL1196'),//Monthly
	             'weekly'=>t('LBL1197'),//Weekly
	             'daily'=>t('LBL1198'));//Daily
	}
	catch (Exception $ex) {
		watchdog_exception('getFrequencyOptions', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Builds the day dropdown options list
 */
function getDayOptions($frequency) {
	try{
		$dayOptions = array();

		switch ($frequency) {
			case 'yearly':
			case 'monthly':
				$dayOptions = array_combine(range(1, 31), range(1, 31));
		 	  break;
		 		
	    case 'weekly':
	  	  $dayOptions[1] = t('LBL1199');//Sunday
	  	  $dayOptions[2] = t('LBL1200');//Monday
	  	  $dayOptions[3] = t('LBL1201');//Tuesday
	  	  $dayOptions[4] = t('LBL1202');//Wednesday
	  	  $dayOptions[5] = t('LBL1203');//Thursday
	  	  $dayOptions[6] = t('LBL1204');//Friday
	  	  $dayOptions[7] = t('LBL1205');//Saturday
	  	  break;

	    case 'daily':
	  	  break;
		} //switch end
		expDebug::dPrint('$dayOptions = ' . print_r($dayOptions, true), 4);
		return $dayOptions;
	}
	catch (Exception $ex) {
		watchdog_exception('getDayOptions', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Creates report schedules addedti form
 */
function report_schedule_addedit_form($form, &$form_state, $reportId, $scheduleId) {
	try {
		expDebug::dPrint('$form_state[build_info] = ' . print_r($form_state['build_info'], true) , 5);
		expDebug::dPrint('$form_state[storage] = ' . print_r($form_state['storage'], true) , 5);
		expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true) , 5);
	  expDebug::dPrint('$reportId = ' . $reportId, 4);
	  expDebug::dPrint('$scheduleId = ' . $scheduleId, 4);
	
	  $loggedInUserId = getSltpersonUserId();
	  // Save repord id and schedule id in form state
		$form_state['report_id'] = $reportId;
		if (!empty($scheduleId)) {
		  $form_state['schedule_id'] = $scheduleId;
		}
		
		// Fetch schedule details when schedule id is present
		$scheduleDetails = array();
		if (empty($scheduleId)) {
			$reportTitle = getReportHeader($reportId);
			$reportTitle = empty($reportTitle)? '' : $reportTitle;
		}
		else {
			$scheduleDetails = getScheduleDetailsForForm($scheduleId);
			expDebug::dPrint('$scheduleDetails = ' . print_r($scheduleDetails, true), 4);
			
	    // Extract criteria filters default values from params
	    $criteriaDBValues = array(); // Initialize
	    $splitParamsStr = explode(',', $scheduleDetails->params);
	    foreach ($splitParamsStr as $paramStr) {
	    	 $splitParamStr = explode('~~', $paramStr);
	    	 $criteriaDBValues[$splitParamStr[0]] = empty($splitParamStr[1])? '' : $splitParamStr[1]; // Date type criteria default value will be
	    	                                                                                          // further processed for DB, DA, or Date
	    	                                                                                          // Other fields default value will be further
	    	                                                                                          // checked for multiple values
	    }
	    
	    // Add criteria filters default values to schedule details
	    $scheduleDetails->criteria_values = $criteriaDBValues;
	    expDebug::dPrint('$scheduleDetails with criteria default values = ' . print_r($scheduleDetails, true), 4);
	    
	    // Add sort by and sort order default values to schedule details
	    $scheduleDetails->sort_by = '';
	    $scheduleDetails->sort_order = '';
	    if (!empty($scheduleDetails->sort)) {
		    $splitSortStr = explode('~~', $scheduleDetails->sort);
		    $scheduleDetails->sort_by = $splitSortStr[0];
		    $scheduleDetails->sort_order = $splitSortStr[1];
	    }
		}
	
	  $formWrapperId = 'create-schedule-addedit-form';
	  $tabIndex = 1;
		$form = newAddEditFormWithCancelAndSave($formWrapperId);
	 
		$form['field-scroll-start'] = array(
				'#type' => 'markup',
				'#markup' => '<div id="field-scroll-wrapper">'
		);
		addHiddenFieldToAddEditForm($form, 'report_id', $form_state['report_id']);
		
		$defaultValue = empty($scheduleDetails)? '' : implode(', ', explode(',', $scheduleDetails->mail_to));
		addTextFieldToAddEditForm($form, 'mail_to', t('LBL440'), $defaultValue, $tabIndex++, true); //Send to
		$form['mail_to']['#attributes']['data-default-text'] = t('MSG698'); //Separate email addresses with commas
	
		$defaultValue =  empty($scheduleDetails)? $reportTitle : $scheduleDetails->mail_subject;
		addTextFieldToAddEditForm($form, 'mail_subject', t('LBL119'), $defaultValue, $tabIndex++, true); //Subject
	
		$defaultValue =  empty($scheduleDetails)? '' : $scheduleDetails->mail_body;
		addTextAreaFieldToAddEditForm($form, 'mail_body', t('LBL929'), $defaultValue, $tabIndex++, true); //Message
	
		addTwoColumnRowToAddEditForm($form, 'format_schedule');
	
		$defaultValue =  empty($scheduleDetails)? '' : $scheduleDetails->attach_format;
		$availableOptions = array('csv' => t('csv'),
		                          'pdf' => t('pdf'));
		addDropDownToAddEditForm($form, 'attach_format', t('LBL1206'), $availableOptions, $defaultValue, $tabIndex++, true); //Format
	
		$defaultFreqValue = empty($form_state['values'])? (empty($scheduleDetails)? '' : $scheduleDetails->frequency) : $form_state['values']['frequency'];
		$frequencyFieldAttributes = array(
				'id' => 'frequency',
				'title' => t('LBL1207'),//schedule
				'fetch_options' => getFrequencyOptions,
				'default_value' => $defaultFreqValue,
				'mandatory' => true,
				'freq_day_id' => 'freq_day',
				'freq_month_id' => 'freq_month',
				'form_wrapper_id' => $formWrapperId,
				'tab_index' => $tabIndex++,
					
		);
		expDebug::dPrint('$frequencyFieldAttributes = ' . print_r($frequencyFieldAttributes, true), 5);
		addFrequencyDropDownToAddEditForm($form, $frequencyFieldAttributes);
	
		endAddEditTwoColumnRow($form);
	
		addTwoColumnRowToAddEditForm($form, 'day_month');
		$form['field-month-start'] = array(
				'#type' => 'markup',
				'#markup' => '<div class="month-field-wrapper">'
		);
		$defaultMonthValue = empty($scheduleDetails)? '' : $scheduleDetails->freq_month;
		if (empty($defaultFreqValue) || $defaultFreqValue != 'yearly') {
			$monthReadOnly = true;
			$defaultMonthValue = '';
			$monthMandatory = false;
		}
		else {
			$monthReadOnly = false;
			$monthMandatory = true;
		}
		$availableOptions = array('1' => t('LBL1208'),//January
		                          '2' => t('LBL1209'),//February
		                          '3' => t('LBL1210'),//March
		                          '4' => t('LBL1211'),//April
		                          '5' => t('LBL1212'),//May
		                          '6' => t('LBL1213'),//June
		                          '7' => t('LBL1214'),//July
				                      '8' => t('LBL1215'),//August
				                      '9' => t('LBL1216'),//September
				                      '10' => t('LBL1217'),//October
				                      '11' => t('LBL1218'),//October
				                      '12' => t('LBL1219'));//December
		addDropDownToAddEditForm($form, 'freq_month', t('LBL913'), $availableOptions, $defaultMonthValue, $tabIndex++, $monthMandatory, $monthReadOnly); //Month
		$form['field-month-end'] = array(
				'#type' => 'markup',
				'#markup' => '</div>'
		);
		$defaultDayValue =  empty($scheduleDetails)? '' : $scheduleDetails->freq_day;
		if ($form_state['triggering_element']['#name'] == 'frequency' || $defaultFreqValue == 'daily') {
			$defaultDayValue = '';
		}
	
		$dayFieldAttributes = array(
				'id' => 'freq_day',
				'wrapper_id' => 'adddedit-day-wrapper',
				'title' => t('LBL910'),//day
				'fetch_options' => getDayOptions,
				'default_value' => $defaultDayValue,
				'affector_title' => t('LBL1207'),//Schedule
				'affector_default_value' => $defaultFreqValue,
				'tab_index' => $tabIndex++,
		);
		expDebug::dPrint('$dayFieldAttributes  = ' . print_r($dayFieldAttributes, true), 5);
	
		if (empty($defaultFreqValue) || $defaultFreqValue == 'daily') {
			$dayFieldAttributes['mandatory'] = false;
		}
		else {
			$dayFieldAttributes['mandatory'] = true;
		}
		addDayDropDownToAddEditForm($form, $dayFieldAttributes);
	
		endAddEditTwoColumnRow($form);
	
		addTwoColumnRowToAddEditForm($form, 'time_timezone');
		
		$defaultValue = empty($scheduleDetails)? '' : empty($scheduleDetails->freq_time)? '00:00' : $scheduleDetails->freq_time;
		if (!empty($defaultValue)) {
			$defaultValueArr = explode(':', $defaultValue);
			$defaultValue = $defaultValueArr[0] . ':' . $defaultValueArr[1];
		}
		expDebug::dPrint('freq_time $defaultValue = ' . $defaultValue, 4);
		$form['freq-time-start'] = array(
				'#type' => 'markup',
				'#markup' => '<div class="time-picker-image">'
		);
		addTimePickerFieldToAddEditForm($form, 'freq_time', t('LBL1220'), $defaultValue, $tabIndex++, true); //Time
		
		$form['freq-time-end'] = array(
				'#type' => 'markup',
				'#markup' => '<div class="icon-timer">&nbsp;</div></div>'
		);
		// Prepare default timezone
		$defaultValueTZ = empty($scheduleDetails)? '' : $scheduleDetails->freq_timezone;
		if (empty($defaultValueTZ)) {
			$defaultValueTZ = getPersonDetails($loggedInUserId, array('time_zone'));
		}
	  if (empty($defaultValueTZ)) {
	  	$defaultSiteTZ = defaultSiteTimeZone();
	    $defaultValueTZ = $defaultSiteTZ['time_zone'];
	    $defaultValueTZ = empty($defaultValueTZ)? '' : $defaultValueTZ;
		}
	
		// Prepare timezones list
		$timeZoneArr = getTimeZonesList();
		$timeZoneList = array();
		foreach ($timeZoneArr as $id => $row) {
		  $timeZoneList[$row->zonecode] = $row->zonename;
		}
		addDropDownToAddEditForm($form, 'freq_timezone', t('LBL297'), $timeZoneList, $defaultValueTZ, $tabIndex++, false);
	
		endAddEditTwoColumnRow($form);
	
	
		// Add Criteria fields
		$reportCriteria = loadAddedReportCriteria($form_state['report_id']);
		$form['report-param-seperator'] = array(
				'#type'    => 'markup',
				'#markup'  => '<div class="clearBoth"></div>' .
				'<div style="display:block; padding:11px 0px">' .
				'<div class="report-param-seperator"></div>' .
				'</div>'
		);
		if (count($reportCriteria)) {
			
			$form['report-param-text'] = array(
					'#type'    => 'markup',
					'#markup'  => '<div class="report-param-text-style">' . t('LBL1221') . ':</div>'//Report Input Parameters
			);
		} 
		else {
			$form['report-param-empty-text'] = array(
					'#type'    => 'markup',
					'#markup'  => '<div class="report-param-empty-text">' . t('MSG695') . '</div>'//Criteria not available
			);
		}
		$form_state['mandatory'] = array();
		$form_state['mandatory_datefields'] = array();
		$form_state['criteria_name'] = array();
		$criteriaDateFields = array();

		foreach ($reportCriteria as $key => $value) {
			expDebug::dPrint('Criteria = ' . print_r($value, true), 5);
			if($reportCriteria[$key]->column_type == 'datetime') {
		    $defaultValue = empty($scheduleDetails->criteria_values[$reportCriteria[$key]->id])?
                                        '' : $scheduleDetails->criteria_values[$reportCriteria[$key]->id];
				
				if (!array_key_exists($reportCriteria[$key]->column_name, $criteriaDateFields)) {
					$form['date-criteria-starts'.$reportCriteria[$key]->id] = array(
							'#type' => 'markup',
							'#markup' => '<div class="edit-datecriteria-dropdown">'
					);
					
					$inputId = 'criteriaparam_date_from_' . str_replace('.' , '_' , $reportCriteria[$key]->column_name) . '_' . $reportCriteria[$key]->id;
					addDateCriteriaFieldToAddEditForm($form, $inputId, $reportCriteria[$key]->name . ' - ' . t('LBL649'), $defaultValue, $tabIndex, $reportCriteria[$key]->mandatory || $reportCriteria[$key]->use_user_details);
					$tabIndex += 5;
					
					$form_state['criteria_name'][$inputId] = $reportCriteria[$key]->name;
					if ($reportCriteria[$key]->mandatory || $reportCriteria[$key]->use_user_details) {
						$form_state['mandatory_datefields'][$inputId] = $inputId;
					}
		
					$criteriaDateFields[$reportCriteria[$key]->column_name] = $reportCriteria[$key]->column_name;
				}
				else {
					
					$inputId = 'criteriaparam_date_to_' . str_replace('.', '_', $reportCriteria[$key]->column_name) . '_' . $reportCriteria[$key]->id;
					addDateCriteriaFieldToAddEditForm($form, $inputId, $reportCriteria[$key]->name . ' - ' . t('LBL621'), $defaultValue, $tabIndex, //To
					                                  $reportCriteria[$key]->mandatory || $reportCriteria[$key]->use_user_details);
					$tabIndex += 5;
					
					$form_state['criteria_name'][$inputId] = $reportCriteria[$key]->name;
					if ($reportCriteria[$key]->mandatory || $reportCriteria[$key]->use_user_details) {
						$form_state['mandatory_datefields'][$inputId] = $inputId;
					}
					
					$form['date-criteria-ends'.$reportCriteria[$key]->id] = array(
							'#type' => 'markup',
							'#markup' => '</div>'
					);
	
				}
				
			}
			else {
				// Determine whether field is read only.
				$useUserDetail = false;
				if ($reportCriteria[$key]->use_user_details == 1 && !user_access('Create Report Perm')) {
					$useUserDetail = true;
				}
				// Revise default value if new schedule and
				if (empty($scheduleDetails) && $useUserDetail) {
					$defaultValue = $reportCriteria[$key]->user_details;
				}
				else {
					$defaultValue = empty($scheduleDetails->criteria_values[$reportCriteria[$key]->id])?
					                          '' : implode(', ', explode('~||', $scheduleDetails->criteria_values[$reportCriteria[$key]->id]));
				}
				expDebug::dPrint('column_type in else part = ' . print_r($reportCriteria[$key]->column_type, true), 5);
				$fieldId = 'criteriaparam_'. str_replace('.' , '_' , $reportCriteria[$key]->column_name) . '_' . $reportCriteria[$key]->id;
	
				addCriteriaTextFieldToReportScheduleAddEditForm($form, $fieldId, $reportCriteria[$key]->name, $defaultValue,
				                                                $tabIndex++, $reportCriteria[$key]->mandatory || $reportCriteria[$key]->use_user_details, $useUserDetail);
				if($reportCriteria[$key]->mandatory || $reportCriteria[$key]->use_user_details){
					$form_state['mandatory'][$fieldId] = $fieldId;
				}
			}
		} //end foreach    
		expDebug::dPrint('loop ends', 4);
		
    /*// Sort  
    addTwoColumnRowToAddEditForm($form, 'sortby_sortorder');
    
    // sort by column
    $sortByList = getReportScheduleSortByOptions($reportId);
    $defaultValue = empty($scheduleDetails) || !array_key_exists($scheduleDetails->sort_by, $sortByList)? '' : $scheduleDetails->sort_by;
    expDebug::dPrint('sortcol $defaultValue = ' . $defaultValue, 4);
    addDropDownToAddEditForm($form, 'sort_by', t('LBL1236'), $sortByList, $defaultValue, $tabIndex++); //Order By
  
    // sort order
    $defaultValue =  empty($scheduleDetails)? '' : $scheduleDetails->sort_order;
    $availableOptions = array('ASC' => t('LBL1237'), //Ascending
                              'DESC' => t('LBL1238')); //Descending
    addSortOrderDropDownToReportScheduleAddEditForm($form, 'sort_order', ' ', $availableOptions, $defaultValue, $tabIndex++);

    endAddEditTwoColumnRow($form);*/

		$form['field-scroll-end'] = array(
				'#type' => 'markup',
				'#markup' => '</div>'
		);
		
		addEditFormWithScheduleCancelAndSaveClose($form, $formWrapperId, 'schedule-save-button', 'schedule-cancel-button', $scheduleDetails, $tabIndex);

		return $form;
	}
  catch (Exception $ex) {
    watchdog_exception('report_schedule_addedit_form', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * For bypassing the default validation and submit handlers of report_schedule_addedit_form
 */
function scheduleAddeditFormIgnore($form, &$form_state) {
	try{
	expDebug::dPrint('$form_state[storage] = ' . print_r($form_state['storage'], true), 4);
  unset($form_state['storage']['form_validate']);
  } catch (Exception $ex) {
  	watchdog_exception('scheduleAddeditFormIgnore', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Adds cancel, save, save and activate, detactivate and delete buttons to schedule addedit form
 */
function addEditFormWithScheduleCancelAndSaveClose(&$form, $wrapperId, $saveButtonId, $cancelButtonId, $scheduleDetails, $tabIndex) {
	try {
		global $theme_key;
		
    $setcls = ($theme_key == 'expertusoneV2')? '' : (empty($scheduleDetails->id) ? 'resize-save_btn' : 'resize-save_btn4');
    $saveCls = empty($scheduleDetails)? 'sched-save-add' : 'sched-save-edit';

		$form['cancel-and-save-actions-row-start'] = array(
		  '#type' => 'markup',
		  '#markup' => '<div class="addedit-form-cancel-and-save-actions-row"><div class="sched-repeat-top-bg"></div>' .
				             '<div class="addedit-form-cancel-container-actions ' . $setcls . ' ' . $saveCls . '">',
		);
	
		$ajaxCallback = 'refreshReportSchedulesPopup';
		
    // Add the Cancel button

		// We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
		// each time the form is re-built.
		$cancelAttributes['class'] = array('addedit-edit-' . $cancelButtonId);
	
		// For common styling of all addedit cancel buttons
		// $cancelAttributes['class'][] = 'addedit-edit-cancel';
		$cancelAttributes['class'][] = 'admin-action-button-middle-bg white-btn-bg-middle';
	
		// A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
		$cancelAttributes['class'][] = 'addedit-form-expertusone-throbber';
		// Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
    $cancelAttributes['data-wrapperid'] = array($wrapperId);
    		
		$cancelAttributes['tabindex'][] = $tabIndex + 1;
	
		// A common class for all addedit form cancel button, for styling to appear as a link.
		$cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';
		$form[$cancelButtonId] = array(
      '#type' => 'submit',
      '#prefix' => '<div class="white-btn-bg-left"></div>',
      '#suffix' => '<div class="white-btn-bg-right"></div>',
      '#value' => t('LBL109'), // Cancel
      '#attributes' => $cancelAttributes,
		  '#validate' => array('scheduleAddeditFormIgnore'),
		  '#submit' => array('scheduleAddeditFormIgnore'),
		  '#ajax' => array(
						'wrapper'  => $wrapperId,
						'callback' => $ajaxCallback,
						'method'   => 'replace',
						'effect'   => 'none', // 'fade',
						'event'    => 'click',
						'schedActionStatus' => '',
						'keypress' => true,
						'progress' => array(
								'type' => 'throbber',
								'message' => ''
						),
		  )
    );
	
		// Add the Save button
		// We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
		// each time the form is re-built.
		$saveAttributes['class'] = array('addedit-edit-' . $saveButtonId);
	
		// For common styling of all addedit save buttons
		//$saveAttributes['class'][] = 'addedit-edit-save';
		$saveAttributes['class'][] = 'admin-save-button-middle-bg';
		$saveAttributes['tabindex'][] = $tabIndex;
	
		// A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
		$saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
		// Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
		$saveAttributes['data-wrapperid'] = array($wrapperId);
  
		if (empty($scheduleDetails)) { // ADD MODE
      $form[$saveButtonId] = array(
        '#type' => 'submit',
        '#prefix' => '<div class="admin-save-button-container">' .
                       '<div class="admin-save-button-left-bg"></div>',
        '#suffix' =>   '<div class="admin-save-button-right-bg"></div>' .
                     '</div>' .
                     '<div class="clearBoth"></div>',
        '#value' => t('LBL141'), //Save
        '#name' => 'save',
        '#attributes' => $saveAttributes,
        '#ajax' => array(
          'wrapper' => $wrapperId,
          'callback' => $ajaxCallback,
          'method' => 'replace',
          'effect' => 'none', // 'fade',
          'event'=>'click',
          'keypress' => true,
          'progress' => array(
            'type' => 'throbber',
            'message' => ''
          ),
        )
      );  
		}
		else { // EDIT MODE
			$saveAttributes['class'][] = 'edit-catalog-course-save-publish';			
		  $setclass = ($theme_key == 'expertusoneV2')? 'admin-save-button-left-bg' : '';

			$form[$saveButtonId] = array(
			  '#type' => 'submit',
				'#value' => t('LBL141'), //Save
				'#name'	=> 'save',
				'#prefix' => '<div class="admin-save-pub-unpub-button-container">' .
                       '<div class="' . $setclass . '"></div>',
				'#suffix' =>   '<span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span>' .
                     '</div>',
				'#attributes' => $saveAttributes,
				'#ajax' => array(
				  'wrapper' => $wrapperId,
					'callback' => $ajaxCallback,
					'method' => 'replace',
					'effect' => 'none', // 'fade',
					'event' => 'click',
					'schedOp' => '',
					'keypress' => true,
					'progress' => array(
					  'type' => 'throbber',
						'message' => ''
					),
				)
			);

			expDebug::dPrint('save button added', 4);
			
			$form['addedit-button-group-ul-start'] = array(
		    '#type' => 'markup',
			  '#markup' => '<ul class="catalog-pub-add-list">',
			);
			
			expDebug::dPrint('ul start markup added', 4);
		  
			expDebug::dPrint('$scheduleDetails = ' . print_r($scheduleDetails, true), 4);
			if ($scheduleDetails->status == 'cre_rpt_rss_atv') {
				$activateDeactivateLabel  = t('LBL574'); //Deactivate
				$schedOp  = 'Deactivate';
			}
			else {
				$activateDeactivateLabel  = t('LBL641'); //Save and Activate
				$schedOp  = 'Save and Activate';
			}
			
			expDebug::dPrint('activate deactivate button label decided', 4);

      // Add the Save button
      $activateDeactivateButtonId = 'schedule-activate-deactivate-button';
      // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
      // each time the form is re-built.
      $activateDeactivateAttributes['class'] = array('addedit-edit-' . $activateDeactivateButtonId);
  
      // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
      $activateDeactivateAttributes['class'][] = 'addedit-form-expertusone-throbber';
      // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
      $activateDeactivateAttributes['data-wrapperid'] = array($wrapperId);
      
      expDebug::dPrint('adding activate/deactivate button', 4);

			$form[$activateDeactivateButtonId] = array(
			  '#type' => 'submit',
			  '#value' => $activateDeactivateLabel,
			  '#name'	=> 'sched-activate-deactivate',
				'#prefix' => '<li class="save-pub-unpub-sub-menu">',
				'#suffix' => '</li>',
				'#attributes' => $activateDeactivateAttributes,
				'#ajax' => array(
					'wrapper' => $wrapperId,
					'callback' => $ajaxCallback,
				  'method' => 'replace',
					'effect' => 'none', // 'fade',
					'event'=>'click',
					'schedOp' => $schedOp,
					'keypress' => true,
					'progress' => array(
					  'type' => 'throbber',
						'message' => ''
					),
			  )
			);
			
			expDebug::dPrint('activate/deactivate button added', 4);
				
		 	$deleteButtonId = 'schedule-delete-button';
      // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
      // each time the form is re-built.
      $deleteAttributes['class'] = array('addedit-edit-' . $deleteButtonId);
      // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
      $deleteAttributes['class'][] = 'addedit-form-expertusone-throbber';
      // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
      $deleteAttributes['data-wrapperid'] = array($wrapperId);
      
      $form[$deleteButtonId] = array(
        '#type' => 'submit',
        '#value' => t('LBL286'), // Delete
		 	  '#name' => 'delete',
        '#prefix' => '<li class="save-pub-unpub-sub-menu">',
		 	  '#suffix' => '</li>',
        '#attributes' => $deleteAttributes,
        '#validate' => array('scheduleAddeditFormIgnore'),
        '#submit' => array('deleteReportSchedule'),
        '#ajax' => array(
          'wrapper' => $wrapperId,
          'callback' => $ajaxCallback,
            'method' => 'replace',
            'effect' => 'none', // 'fade',
            'event'=>'click',
            'keypress' => true,
            'progress' => array(
              'type' => 'throbber',
              'message' => ''
            ),
          )
      );
		 	
      expDebug::dPrint('delete button added', 4);
      
	    $form['addedit-button-group-ul-end'] = array(
        '#type' => 'markup',
		    '#markup' => '<div class="clearBoth"></div>' .
		               '</ul>',
	    );  
		}
	
		$form['cancel-and-save-actions-row-end'] = array(
		  '#type' => 'markup',
			'#markup' => '</div>' .
				           '<div class="clearBoth"></div>' .
				         '</div>',
		);
	
		$form['addedit-left-col-end'] = array(
		  '#type' => 'markup',
			'#markup' => '</div>' .
			             '<div class="clearBoth"></div>',
		);
	}
  catch (Exception $ex) {
    watchdog_exception('addEditFormWithScheduleCancelAndSaveClose', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Default validate handler for report_schedule_addedit_form
 */
function report_schedule_addedit_form_validate($form, &$form_state) {
	try {
		expDebug::dPrint('$form_state[mandatory] = ' . print_r($form_state['mandatory'], true) , 4);
		expDebug::dPrint('$form_state[values] =  ' . print_r($form_state['values'], true) , 4);
		expDebug::dPrint('$form_state[criteria_name] = ' . print_r($form_state['criteria_name'], true) , 4);
		
		// Cleanup
		expDebug::dPrint('Unsetting $form_state[storage][form_validate]', 4);
		unset($form_state['storage']['form_validate']);
		
		if ($form_state['triggering_element']['#name'] == 'sched-activate-deactivate' &&
		                                       $form_state['triggering_element']['#ajax']['schedOp'] == 'Deactivate')  {
			return; // No validation required
		}
		
		// Check the mandatory fields after trimming and saving the values
		if ($form_state['values']['mail_to'] == t('MSG698')) { //Separate email addresses with commas
		  $form_state['values']['mail_to'] = '';
		}
	  foreach(array('mail_to','mail_subject', 'mail_body') as $field) {
	  	$form_state['values'][$field] = trim($form_state['values'][$field]);
	  }
	  // validating mandatory fields based on frequency
	  //addEditCheckMandatoryFields() can be called only once in form_validate (see issue #0030486)
	  if ($form_state['values']['frequency'] == 'yearly') {
	  	addEditCheckMandatoryFields($form, $form_state, array('mail_to', 'mail_subject', 'mail_body', 'attach_format', 'frequency', 'freq_month', 'freq_day', 'freq_time'));
	  }
	  else if ($form_state['values']['frequency'] == 'monthly' || $form_state['values']['frequency'] == 'weekly') {
	  	addEditCheckMandatoryFields($form, $form_state, array('mail_to', 'mail_subject', 'mail_body', 'attach_format', 'frequency', 'freq_day', 'freq_time'));
	  }
	  else {
	  	addEditCheckMandatoryFields($form, $form_state, array('mail_to', 'mail_subject', 'mail_body', 'attach_format', 'frequency', 'freq_time'));
	  }
	  // Check mandatory criteria params
	  foreach($form_state['values'] as $key => $value) {
	  	expDebug::dPrint('form field $key = ' . print_r($key, true) . ' value = ' . print_r($value, true) , 5);
	  	if (strpos($key, 'criteriaparam') !== false) {		
	  		//trim and save the values
	  		if (strpos ($key, 'criteriaparam_date_') !== false) {
	  			$form_state['values'][$key]['date-criteria-date'] = trim($form_state['values'][$key]['date-criteria-date']);
	  			if (empty($form_state['values'][$key]['date-criteria-date']) || $form_state['values'][$key]['date-criteria-date'] == 'mm-dd-yyyy') {
	  				  $form_state['values'][$key]['date-criteria-date'] = '';
	  			}
	  			else {
	  				$explodedDateField = explode('-',$form_state['values'][$key]['date-criteria-date']);
	  				$year = $explodedDateField[2];
	  				$explodedDateField[2] = $explodedDateField[1];
	  				$explodedDateField[1] = $explodedDateField[0];
	  				$explodedDateField[0] = $year;
	  				$form_state['values'][$key]['date-criteria-date'] = implode('-',$explodedDateField);
	  			}
	  			expDebug::dPrint('$form_state[values][$key][date-criteria-date]=  ' . print_r($form_state['values'][$key]['date-criteria-date'], true) , 4);
	  			$form_state['values'][$key]['date-criteria-days'] = trim($form_state['values'][$key]['date-criteria-days']);
	  			expDebug::dPrint('$form_state[values][$key][date-criteria-days]=  ' . print_r($form_state['values'][$key]['date-criteria-days'], true) , 4);
	  		}
	  		else {
	  			$form_state['values'][$key] = trim($form_state['values'][$key]);
	  		}
	  		//validate the mandatory fields
	  		if (!empty($form_state['mandatory'][$key]) && empty($form_state['values'][$key])){
	  			$form_state['storage']['form_validate'] = false;
	  			form_set_error($key, $form[$key]['#title'] . t('ERR101'));//is required.
	  		}

	  		if (!empty($form_state['mandatory_datefields'][$key])) {
	  			if ($form_state['values'][$key]['date-criteria-radio'] == 'date') {
	  				 $dateField = 'date-criteria-date';
	  			}
	  			else {
	  				$dateField = 'date-criteria-days';
	  			}
	  			expDebug::dPrint('$form_state[values][$key][$dateField]=  ' . print_r($form_state['values'][$key][$dateField], true) , 4);
	  			if(empty($form_state['values'][$key][$dateField])) {
	  				if (strpos ($key, 'criteriaparam_date_from_') !== false) {
	  					$form_state["storage"]["form_validate"] = false;
	  					form_set_error($key . '][' . $dateField , $form_state['criteria_name'][$key] . ' ' . t('LBL649')  . t('ERR101'));//is required.
	  				}
	  				else {
	  					$form_state["storage"]["form_validate"] = false;
	  					form_set_error($key . '][' . $dateField , $form_state['criteria_name'][$key]. ' ' . t('LBL621')  . t('ERR101'));
	  					expDebug::dPrint('$form_state[criteria_name][$key] = ' . $form_state['criteria_name'][$key], 5);
	  				}			
	  			}
	  		}
	  		
	  		// Trim and validate multiple values in non-date fields
	  		if (strpos ($key, 'criteriaparam_date_') === false && !empty($form_state['values'][$key])) {
	  	    $fieldValue = explode(',', $form_state['values'][$key]);
	  	    foreach ($fieldValue as $valKey => $value) {
	  	    	$fieldValue[$valKey] = trim($value);
	  	    	if (empty($fieldValue[$valKey])) {
              $form_state["storage"]["form_validate"] = false;
              form_set_error($key, t('LBL405'));//Enter a value.
	  	    	}
	  	    } // end foreach
	  	    $form_state['values'][$key] = implode('~||', $fieldValue); // Prepare the value string for saving in DB
	  		} // end if
	  	} // end if criteriaparam
	  } // end foreach
	  
	  // Validate tha mail_to field
		if (!empty($form_state['values']['mail_to'])) {
			$emailValue = explode(',', $form_state['values']['mail_to']);
			foreach ($emailValue as $key => $value) {
				$emailValue[$key] = trim($value);
				if (empty($emailValue[$key]) || !valid_email_address($emailValue[$key])) {
					$form_state["storage"]["form_validate"] = false;
			    form_set_error('mail_to', t('ERR002'));//Enter a valid email address.
			  }
		  }
		  $form_state['values']['mail_to'] = implode(',', $emailValue); // retain the trimmed values for saving in DB
		}
			
	  //Validate criteria date fields
		validateDateFields($form, $form_state);
		
		// When no validation error, unset $form_state['storage']['form_validate']
		if (isset($form_state['storage']['form_validate']) && $form_state['storage']['form_validate'] == true) {
			expDebug::dPrint('no validation error', 4);
			unset($form_state['storage']['form_validate']); // No validation error
		}
	}
	catch (Exception $ex) {
    watchdog_exception('report_schedule_addedit_form_validate', $ex);
    expertusErrorThrow($ex);
  }
}
	
/**
 * Validates the report params date fields
 */
function validateDateFields($form, &$form_state) {
	try {
		//loop through all the form state values
		foreach ($form_state['values'] as $key => $value) {			
		  //if field_id does not start with criteriaparams_date and its not a from datefield ignore
		  if(strpos ($key, 'criteriaparam_date_from_') === false) {
		  	expDebug::dPrint('Not a date_from field Skipping.', 4);
		  	continue;
		  }
		  	
		  //get common prefix for date fields and build to date field id.
		  $fromDateFieldId = $key;
		  $explodedFromDateId = explode('_', $fromDateFieldId);
		  $explodedFromDateId[2] = 'to';
		  $explodedLastElemIdx = count($explodedFromDateId) - 1;
		  $fromDateCriteriaId = $explodedFromDateId[$explodedLastElemIdx];
		  $toDateCriteriaId = $fromDateCriteriaId + 1;
		  $explodedFromDateId[$explodedLastElemIdx] = $toDateCriteriaId;
		  $toDateFieldId = implode('_', $explodedFromDateId);
      expDebug::dPrint('$fromDateFieldId = ' . $fromDateFieldId, 4);
      expDebug::dPrint('$toDateFieldId = ' . $toDateFieldId, 4);
		  
      //get from and to date field value
      if ($form_state['values'][$fromDateFieldId]['date-criteria-radio'] == 'date') {
      	$fromDateField = 'date-criteria-date';
      }
      else {
      	$fromDateField = 'date-criteria-days';
      }
      $fromVal = $form_state['values'][$fromDateFieldId][$fromDateField];
      
      if ($form_state['values'][$toDateFieldId]['date-criteria-radio'] == 'date') {
      	$toDateField = 'date-criteria-date';
      }
      else {
      	$toDateField = 'date-criteria-days';
      }
      $toVal = $form_state['values'][$toDateFieldId][$toDateField];
      expDebug::dPrint('$fromVal = ' . $fromVal, 4);
      expDebug::dPrint('$toVal = ' . $toVal, 4);
      
		  
		  //if both values are empty - no check required
		  if(empty($fromVal) && empty($toVal)) {
		    expDebug::dPrint('Both form date and to date fields are empty. Skipping.', 4);
		    continue;
		  }
		   
		  //if either of the date fields are empty- error
		  if(empty($fromVal) || empty($toVal)) {
		  	if (!empty($form_state['mandatory_datefields'][$fromDateFieldId])) {
		  		continue;
		  	}
		  	if(empty($fromVal)){
		    expDebug::dPrint('from date is empty. Error condition.', 4);
		   	$form_state["storage"]["form_validate"] = false;
  			form_set_error($fromDateFieldId . '][' . $fromDateField , t('ERR239'));//One of the date field cannot be left empty
		  	}
		  	else {
		  		expDebug::dPrint('to date is empty. Error condition.', 4);
		  		$form_state["storage"]["form_validate"] = false;
		  		form_set_error($toDateFieldId . '][' . $toDateField , t('ERR239'));//One of the date field cannot be left empty
		  	}
  			continue;
		  }

      //Modify the values to as they would be saved in DB
		  $fromDateFieldCriteriaValue = '';
		  if ($fromDateField == 'date-criteria-days') {
		  	$fromDateFieldCriteriaId = 'date-criteria-days-type';
		  	$fromDateFieldCriteriaValue = $form_state['values'][$fromDateFieldId][$fromDateFieldCriteriaId];
		  }
    
      $fromVal = $fromVal . $fromDateFieldCriteriaValue;
      expDebug::dPrint('$fromVal = ' . $fromVal, 5);
      
      $toDateFieldCriteriaValue = '';
      if ($toDateField == 'date-criteria-days') {
      	$toDateFieldCriteriaId = 'date-criteria-days-type';
      	$toDateFieldCriteriaValue = $form_state['values'][$toDateFieldId][$toDateFieldCriteriaId];
      }
     
      $toVal = $toVal . $toDateFieldCriteriaValue;
      expDebug::dPrint('$toVal = ' . $toVal, 5);

		  //Based of type of value check whether the string is in correct format
		  $nowTimeObj = date_now(date_default_timezone(FALSE));
		  $compareDates = true;
		  if (endswith($fromVal, 'DB') || endswith($fromVal, 'DA')) {
		    $numericPart = substr($fromVal, 0, -2);
		   	if (!is_numeric($numericPart)) {
		   		expDebug::dPrint('form date ends with DB or DA but not numeric. Error condition.', 4);
		   		$form_state["storage"]["form_validate"] = false;
		   		form_set_error($fromDateFieldId . '][' . $fromDateField , t('ERR097'). ' '.$form[$fromDateFieldId]['#title']);//Enter only numeric values in
		   		$compareDates = false;
		   	}
		   	else {
		   	  preProcessScheduledReportDateValue($nowTimeObj, $fromVal);
		   	  expDebug::dPrint('$fromVal = ' . $fromVal, 4);
		   	} 	
		  }
		  if (endswith($toVal, 'DB') || endswith($toVal, 'DA')) {
		   	$numericPart = substr($toVal, 0, -2);
		   	if (!is_numeric($numericPart)) {
		   		expDebug::dPrint('to date ends with DB or DA but not numeric. Error condition.', 4);
		   		$form_state["storage"]["form_validate"] = false;
		   		form_set_error($toDateFieldId . '][' . $toDateField , t('ERR097'). ' '.$form[$toDateFieldId]['#title']);//Enter only numeric values in
		   		$compareDates = false;
		   	}
		   	else {
		   		preProcessScheduledReportDateValue($nowTimeObj, $toVal);
		   		expDebug::dPrint('$toVal = ' . $toVal, 4);
		   	}
		  }
		  
		  //check if from date is greater than to date - error
		  if ($compareDates) {
		    expDebug::dPrint('Comparing dates.', 4);
			  $fromDateObj = new DateObject($fromVal, date_default_timezone(FALSE), 'Y-m-d');
			  $toDateObj = new DateObject($toVal, date_default_timezone(FALSE), 'Y-m-d');
			  if ($fromDateObj > $toDateObj) {
			    expDebug::dPrint('from date is past to date. Error condition.', 4);
			  	$form_state["storage"]["form_validate"] = false;
			  	form_set_error($fromDateFieldId . '][' . $fromDateField, t('ERR229'));//From date cannot be later than the to date
			  }
		  }
		}
	}
	catch (Exception $ex) {
		watchdog_exception('validateDateFields', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Submit handler for report_schedule_addedit_form
 */
function report_schedule_addedit_form_submit($form, &$form_state) {
	try {
		expDebug::dPrint('$form_state[storage] = ' . print_r($form_state['storage'], true), 4);
		expDebug::dPrint('$form_state[values]rwerwrewrewr =' . print_r($form_state['values'], true), 5);
		
		if ($form_state['triggering_element']['#name'] == 'sched-activate-deactivate' &&
		                                                 $form_state['triggering_element']['#ajax']['schedOp'] == 'Deactivate')  {
			deactivateReportSchedule($form_state['schedule_id']);
			return;
		}
		
		$loggedInUserId = getIdOfLoggedInUser();
		
		// Check whether add or edit
		if (empty($form_state['schedule_id'])) { // Add
			expDebug::dPrint('ADD PART', 4);
			$returnArr = addNewSchedule($form, $form_state, $loggedInUserId);
			expDebug::dPrint(' $returnArr = ' . print_r($returnArr, true) , 4);
			$newScheduleId = $returnArr[0]->id;
			expertus_set_message(t('MSG689'));//Details Updated.
		}
		else {
			expDebug::dPrint('UPDATE PART', 4);
			$updateStatus = $form_state['triggering_element']['#name'] == 'sched-activate-deactivate'? true : false;
			updateScheduleDetails($form, $form_state, $updateStatus, $loggedInUserId);
			expertus_set_message(t('MSG689'));//Details Updated.
		}
	}
	catch (Exception $ex) {
		watchdog_exception('report_schedule_addedit_form_submit', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Clears schedules month and day fields
 */
function clearScheduleMonthAndDayFields($form, &$form_state) {
	try {
		$dayFieldId = $form_state['triggering_element']['#ajax']['freq_day_id'];
		unset($form_state['values'][$dayFieldId]);
		unset($form_state['input'][$dayFieldId]);
		
		$monthFieldId = $form_state['triggering_element']['#ajax']['freq_month_id'];
		unset($form_state['values'][$monthFieldId]);
		unset($form_state['input'][$monthFieldId]);
	}
  catch (Exception $ex) {
    watchdog_exception('clearScheduleMonthAndDayFields', $ex);
    expertusErrorThrow($ex);
  }	
}

/**
 * Adds frequency dropdown field to schedules addedit form
 */
function addFrequencyDropDownToAddEditForm(&$form, $affector = array()) {
	try {
		expDebug::dPrint('$affector  = ' . print_r($affector, true) ,4);
		$affectorAttributes = array();
		$affectorAttributes['autocomplete'] = 'off'; // Prevent browser autocomplete
	
		// We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
		// each time the form is re-built.
		$affectorAttributes['class'] = array('addedit-edit-' . $affector['id']); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
		// Add classes for adding style
		$affectorAttributes['class'][] = 'addedit-edit-dropdown';
		$affectorAttributes['class'][] = 'addedit-dependant-dropdown-affector';
	
		// A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
		$affectorAttributes['class'][] = 'addedit-form-expertusone-throbber';
		// Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
		$affectorAttributes['data-wrapperid'] = array($affector['form_wrapper_id']);
	
		if (isset($affector['tab_index']) && $affector['tab_index'] > 0) {
			$affectorAttributes['tabindex'] = $affector['tab_index'];
		}
		expDebug::dPrint('$affectorAttributes  = ' . print_r($affectorAttributes, true) ,4);
	
		$affectorOptions = $affector['fetch_options']();
		if ($affector['mandatory']) {
			$nullOption[''] = t('LBL674');
		} else {
			$nullOption[''] = 'None Selected';
		}
		expDebug::dPrint('$affectorOptions = ' . print_r($affectorOptions, true) ,4);
		//$finalAffectorOptions = array_merge($nullOption, $affectorOptions);
		$finalAffectorOptions = $nullOption + $affectorOptions;
		expDebug::dPrint('$finalAffectorOptions = ' . print_r($finalAffectorOptions, true) , 4);
	  
		if (empty($form['addedit_tmp_storage']['two_column'])) {
			$prefix = addEditGetPrefix('FIELD', $affector['title'], $affector['mandatory']);
			$suffix = addEditGetSuffix('FIELD');
		}
		else {
			if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
				$prefix = addEditGetPrefix('TWOCOL_FIRSTCOL_FIELD', $affector['title'], $affector['mandatory']);
				$suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD');
				$form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
			} else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
				$prefix = addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $affector['title'], $affector['mandatory']);
				$suffix = addEditGetSuffix('TWOCOL_SECONDCOL_FIELD');
				$form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
			} else {
				throw new Exception('exp_sp_administration.inc : addAjaxAffectorDropDownToAddEditForm() : Attempt to add a third field to a two column row.');
			}
		}
		
		global $theme_key;
		$newuiPrefix = '';
    $newuiSuffix = '';		
		if($theme_key == 'expertusoneV2') {
			$newuiPrefix = '<div class="expertus-dropdown-bg">' .
			                 '<div class="expertus-dropdown-icon">';
			$newuiSuffix =   '</div>' .
			               '</div>';
		}
	
		$form[$affector['id']] = array(
	    '#type' => 'select',
		  '#prefix' => $prefix . $newuiPrefix,
		  '#suffix' => $newuiSuffix . $suffix,
		  '#title' => $affector['title'],
		  '#title_display' => 'invisible',
		  '#options' => $finalAffectorOptions,
		  '#default_value' => $affector['default_value'],
		  '#attributes' => $affectorAttributes,
		  '#validate' => array('clearScheduleMonthAndDayFields'),
		  '#ajax' => array(
			  'wrapper' => $affector['form_wrapper_id'],
				'callback' => repaintSchedulesForm,
				'freq_day_id' => $affector['freq_day_id'],
				'freq_month_id' => $affector['freq_month_id'],
				'method' => 'replace',
				'effect' => 'none', // 'fade',
				//'event'=>'click',
				'keypress' => true,
				'progress' => array(
					'type' => 'throbber',
					'message' => ''
				),
		  )
		);
	}
  catch (Exception $ex) {
    watchdog_exception('addFrequencyDropDownToAddEditForm', $ex);
    expertusErrorThrow($ex);
  } 
}

/**
 * Adds day dropdown field to schedules addedit form
 */
function addDayDropDownToAddEditForm(&$form, $affected = array()) {
	try {
		$affectedAttributes = array();
		$affectedAttributes['autocomplete'] = 'off'; // Prevent browser autocomplete
	
		// We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
		// each time the form is re-built.
		$affectedAttributes['class'] = array('addedit-edit-' . $affected['id']); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
		// Add classes for adding style
		$affectedAttributes['class'][] = 'addedit-edit-dropdown';
		$affectedAttributes['class'][] = 'addedit-dependant-dropdown-affected';
	
	
		if (isset($affected['tab_index']) && $affected['tab_index'] > 0) {
			$affectedAttributes['tabindex'] = $affected['tab_index'];
		}
	
		$affectedOptions = $affected['fetch_options']($affected['affector_default_value']);
	  
		$readOnly = false;
		$nullOption[''] = t('LBL674');
		if (empty($affected['affector_default_value']) || $affected['affector_default_value'] == 'daily') {
			$readOnly = true;
		}
		
		$finalAffectedOptions = $nullOption + $affectedOptions;
		expDebug::dPrint(' $finalAffectedOptions = ' . print_r($finalAffectedOptions, true) , 4);
	
		$prefix = '';
		$suffix = '';
		if (empty($form['addedit_tmp_storage']['two_column'])) {
			$prefix = addEditGetPrefix('FIELD', $affected['title'], $affected['mandatory'], ':', $affected['wrapper_id']);
			$suffix = addEditGetSuffix('FIELD');
		}
		else {
			if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
				$prefix = addEditGetPrefix('TWOCOL_FIRSTCOL_FIELD', $affected['title'], $affected['mandatory'], ':', $affected['wrapper_id']);
				$suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD');
				$form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
			} else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
				$prefix = addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $affected['title'], $affected['mandatory'], ':', $affected['wrapper_id']);
				$suffix = addEditGetSuffix('TWOCOL_SECONDCOL_FIELD');
				$form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
			} else {
				throw new Exception('exp_sp_administration.inc : addAjaxAffectorDropDownToAddEditForm() : Attempt to add a third field to a two column row.');
			}
		}
		global $theme_key;
		$newuiPrefix = '';
    $newuiSuffix = '';
		if($theme_key == 'expertusoneV2') {
			$newuiPrefix = '<div class="expertus-dropdown-bg">' .
			                 '<div class="expertus-dropdown-icon">';
			$newuiSuffix =   '</div>' .
			               '</div>';
		}
	
		$form[$affected['id']] = array(
				'#type' => 'select',
				'#prefix' => $prefix . $newuiPrefix,
				'#suffix' => $newuiSuffix . $suffix,
				'#title' => $affected['title'],
				'#title_display' => 'invisible',
				'#options' => $finalAffectedOptions,
				'#default_value' => $affected['default_value'],
				'#attributes' => $affectedAttributes,
				'#disabled' => $readOnly,
		);
  }
  catch (Exception $ex) {
    watchdog_exception('addDayDropDownToAddEditForm', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Repaints schedules form
 */
function repaintSchedulesForm($form, $form_state) {
   try{
   	  $builtForm = drupal_build_form('report_schedule_addedit_form', $form_state);
   	  $commands[] = ajax_command_replace(NULL, drupal_render($builtForm));
      $commands[] = ajax_command_invoke(NULL, "scheduleParametersCallVtip",array());
	  return array(
	  '#type' => 'ajax',
	  '#commands' => $commands
	  ); 
   	}
	catch (Exception $ex) {
	  watchdog_exception('repaintSchedulesForm', $ex);
	  expertusErrorThrow($ex);
	}
}
/**
 * Fetches schedule details for initializing schedule addedit form
 */
function getScheduleDetailsForForm($scheduleId) {
	try {
	  $fields = array('id', 'status', 'report_id', 'mail_subject', 'mail_body', 'mail_to', 'attach_format', 'frequency', 'freq_month', 'freq_day',
	                                  'freq_time', 'freq_timezone', 'params', 'sort');
		// Select table
		$selectStmt = db_select('slt_report_schedules', 'schedules');
		
		// Set conditions
		$selectStmt->condition('schedules.id', $scheduleId);

		// Select fields and/or add expressions
		foreach($fields as $fieldName) {
			$selectStmt->addField('schedules', $fieldName, $fieldName);
		}

		expDebug::dPrintDBAPI('$selectStmt', $selectStmt);

		// Execute query
		$result = $selectStmt->execute()->fetch();
		expDebug::dPrint('$result = ' . print_r($result, true));

		return $result;
	}
	catch (Exception $ex) {
		watchdog_exception('getScheduleDetailsForForm', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Builds the criteria param for saving in DB
 */
function buildCriteriaParams($form_state) {
	try {
		$crtCnt = 0;
		$paramValues = '';
		
		foreach($form_state['values'] as $key => $value) {
		expDebug::dPrint('form field $key = ' . print_r($key, true). ' $value = ' . print_r($value, true) , 5);
			if (strpos($key, 'criteriaparam') !== false) {
				$criteria = explode('_', $key);
				if ($criteria[1] == 'date') {
					if ($value['date-criteria-radio'] == 'date') {
					  $value = $value['date-criteria-date'];
					}
					else {
						if($value['date-criteria-days']!='')
						$value = $value['date-criteria-days'] . $value['date-criteria-days-type'];
						else
							$value='';
					}
					expDebug::dPrint(' $value for date fields = ' . print_r($value, true) , 5);
				}
				
				$paramValues .= ($crtCnt) ? ',' : '';
				$criteriaId = $criteria[count($criteria) - 1];
				expDebug::dPrint('criteria size values'.print_r($criteria, true), 5);
				
				$paramValues .= $criteriaId . '~~' . $value;
				$crtCnt = 1;
				expDebug::dPrint('built till now $paramValues = ' . print_r($paramValues, true), 5);
		 }
	  } // end foreach
	  expDebug::dPrint('returning $paramValues = ' . $paramValues, 4);
	  return $paramValues;
	}
	catch(Exception $ex) {
		watchdog_exception('buildCriteriaParams', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Saves a new schedule record in DB
 */
function addNewSchedule($form, &$form_state, $userId = "" ) {
	expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 5);
	
	$txn = db_transaction();	
	try {
		$paramValues = buildCriteriaParams($form_state);
		//$sortValue = buildReportScheduleSortValue($form_state['values']['sort_by'], $form_state['values']['sort_order']);		
	  if (empty($form_state['values']['freq_timezone'])) {
      $defaultSiteTZ = defaultSiteTimeZone();
      $form_state['values']['freq_timezone'] = empty($defaultSiteTZ['time_zone'])? null : $defaultSiteTZ['time_zone'];
    }
    
		$fields = array(
			'report_id'         => $form_state['values']['report_id'],
			'mail_to'           => trim($form_state['values']['mail_to']),
			'mail_subject'      => $form_state['values']['mail_subject'],
			'mail_body'         => $form_state['values']['mail_body'],
			'status'            => 'cre_rpt_rss_atv',
			'attach_format'     => $form_state['values']['attach_format'],
			'frequency'         => $form_state['values']['frequency'],
			'freq_month'        => empty($form_state['values']['freq_month'])? null : $form_state['values']['freq_month'],
			'freq_day'          => empty($form_state['values']['freq_day'])? null : $form_state['values']['freq_day'],
			'freq_time'         => $form_state['values']['freq_time'],
			'freq_timezone'     => $form_state['values']['freq_timezone'],
			'params'            => $paramValues,
		  //'sort'              => $sortValue,
			'notify_user'       => $userId,
			'created_by'        => $userId,
			'created_on'        => now(),
			'updated_by'        => $userId,
			'updated_on'        => now(),
		);
		//Build the next run time
	  $pli = getProfileListItemByCode($fields['freq_timezone']);
		$fields['next_run'] = getReportScheduleNextRunTime(null, $fields['frequency'], $fields['freq_month'],
		                                                                 $fields['freq_day'], $fields['freq_time'] . ':00', $pli[0]->attr2);
    // Select the table to insert the record in
    $insertStmt = db_insert('slt_report_schedules');		                                                                 
		$insertStmt->fields($fields);	
		expDebug::dPrintDBAPI('$insertStmt', $insertStmt);
		
		// Execute the insert statement. Fetch the new $scheduleId.
		$scheduleId = $insertStmt->execute();
		expDebug::dPrint('$scheduleId = ' . $scheduleId);
		
		//make audit trail entries		
		$formFieldsList = array('mail_to', 'mail_subject', 'mail_body', 'attach_format', 'frequency', 'freq_month', 'freq_day',
		                                                                                      'freq_time', 'freq_timezone', 'params'/*, 'sort' */);
		make_audit_trail_entries($fields, $userId, null, $scheduleId , 'Inserted', 'cre_rpt_rss', 'exp_sp_lnrreports',
		                                   'addNewSchedule', $formFieldsList);
		 
		expDebug::dPrint('$scheduleId before returning = ' . $scheduleId);
		return array((object)array('id'=>$scheduleId));
	}
	catch (Exception $ex) {
		$txn->rollback();
		watchdog_exception('addNewSchedule', $ex);
		expertusLogError($ex);
		return array((object)array('id' => 'failure'));
	}
	
}

/**
 * Updates a report schedule record in DB
 */
function updateScheduleDetails($form, &$form_state, $updateStatus, $userId = ""){
	expDebug::dPrint('updateScheduleDetails $form_state[values] = ' . print_r($form_state['values'], true), 5);
	
	$txn = db_transaction();

	try {
		 $scheduleId = $form_state['schedule_id'];		 
		 $formFieldsList = array('mail_to', 'mail_subject', 'mail_body', 'attach_format', 'frequency', 'freq_month', 'freq_day',
                                                                                         'freq_time', 'freq_timezone', 'params'/*, 'sort'*/);
	   if ($updateStatus == true) {
       $formFieldsList[] = 'status';
     }
     $schdSelect = db_select('slt_report_schedules', 'rep_sched');
     $schdSelect->fields('rep_sched', $formFieldsList);
     $schdSelect->condition('rep_sched.id', $scheduleId);
     expDebug::dPrintDBAPI('$schdSelect', $schdSelect);
     $schdPrevStatuses = $schdSelect->execute()->fetchAll();
     expDebug::dPrint('$schdPrevStatuses' . print_r($schdPrevStatuses, true));
		
		$paramValues = buildCriteriaParams($form_state);
		//$sortValue = buildReportScheduleSortValue($form_state['values']['sort_by'], $form_state['values']['sort_order']);
		if (empty($form_state['values']['freq_timezone'])) {
		  $defaultSiteTZ = defaultSiteTimeZone();
      $form_state['values']['freq_timezone'] = empty($defaultSiteTZ['time_zone'])? null : $defaultSiteTZ['time_zone'];
		}
		
		$fields = array(
				'mail_to'           => trim($form_state['values']['mail_to']),
				'mail_subject'      => $form_state['values']['mail_subject'],
				'mail_body'         => $form_state['values']['mail_body'],
				'attach_format'     => $form_state['values']['attach_format'],
				'frequency'         => $form_state['values']['frequency'],
				'freq_month'        => empty($form_state['values']['freq_month'])? null : $form_state['values']['freq_month'],
				'freq_day'          => empty($form_state['values']['freq_day'])? null : $form_state['values']['freq_day'],
				'freq_time'         => $form_state['values']['freq_time'],
				'freq_timezone'     => $form_state['values']['freq_timezone'],
				'params'            => $paramValues,
		    //'sort'              => $sortValue,
				'updated_by'        => $userId,
				'updated_on'        => now()
		);
		if ($updateStatus == true) {
			//Add status to the fields list
		  $fields['status'] = 'cre_rpt_rss_atv';
		}
		
		//Prepare and add the next run time to the fields list
    $pli = getProfileListItemByCode($fields['freq_timezone']);
   $fields['next_run'] = getReportScheduleNextRunTime(null, $fields['frequency'], $fields['freq_month'], $fields['freq_day'],
                                                                                                     $fields['freq_time'] . ':00', $pli[0]->attr2);
                                                                                                      
    $updateStmt = db_update('slt_report_schedules');
    $updateStmt->condition('id', $scheduleId);
		$updateStmt->fields($fields);
		expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
		
		// Execute query
		$numUpdated = $updateStmt->execute();
		expDebug::dPrint('$numUpdated = ' . print_r($numUpdated, true));
		
		make_audit_trail_entries($fields, $userId, $schdPrevStatuses, $scheduleId , 'Updated', 'cre_rpt_rss', 'exp_sp_lnrreports',
		                                                                                        'updateScheduleDetails', $formFieldsList);
		
		return array((object)array('id' => $scheduleId));
	}
	catch (Exception $ex) {
		$txn->rollback();
		watchdog_exception('updateScheduleDetails', $ex);
		expertusLogError($ex);
		return array((object)array('id' => 'failure'));
	}	
}

/**
 * Fetches report schedule details for displaying values in the grid
 */
function getReportSchedulesForGrid($reportId, $countRequired = false) {
  try {	
  	global $user;
  	$loggedUserName = $user->name;
		$loggedInUserId = getSltpersonUserId();
		$select = db_select('slt_report_schedules', 'sched_rep');
		$select->leftJoin('slt_profile_list_items', 'spli_tz', 'sched_rep.freq_timezone = spli_tz.code && spli_tz.lang_code = \'cre_sys_lng_eng\'');
		if ($countRequired) {
			$select->addExpression('count(sched_rep.id)', 'count');
		}
		else {
			$select->addField('sched_rep', 'id', 'id');
			$select->addField('sched_rep', 'status', 'status');
			$select->addField('sched_rep', 'frequency', 'frequency');
			$select->addField('sched_rep', 'freq_month', 'freq_month');
			$select->addField('sched_rep', 'freq_day', 'freq_day');
			$select->addField('sched_rep', 'freq_time', 'freq_time');
			$select->addField('spli_tz', 'attr2', 'freq_tz_phpname');
			$select->addField('spli_tz', 'name', 'freq_tz_name');
			$select->addField('sched_rep', 'next_run', 'next_run');
			$select->addField('sched_rep', 'last_run', 'last_run');
			$select->addField('sched_rep', 'last_run_status', 'last_run_status');
		}
    $select->condition('sched_rep.report_id', $reportId);
    
    $adminUsersList = getadminUsers();
    $adminUsersList = explode(',', $adminUsersList);
    $getUserStatus = in_array($loggedInUserId, $adminUsersList)? true : false;
    /*if(!empty($loggedInUserId) && $getUserStatus == false && $loggedUserName != 'admin') {
    	$select->condition('sched_rep.created_by', $loggedInUserId);
    }*/
		$select->condition(db_or()->condition('sched_rep.status','cre_rpt_rss_atv')
				                      ->condition('sched_rep.status','cre_rpt_rss_itv'));
/* 		if ($countRequired == false) {
		  //$orderByCondition = 'IF(sched_rep.updated_on IS NULL, sched_rep.created_on, sched_rep.updated_on)';
		  $select->orderBy('sched_rep.created_on', 'DESC');
		} */
	  expDebug::dPrintDBAPI('$select for getReportSchedulesForGrid = ', $select);
	  return $select;
	}
	catch (Exception $ex) {
		watchdog_exception('getReportSchedulesForGrid', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Prepares HTML for the schedule column in the schedules grid
 */
function fetchScheduleHtml($frequency) {
	try {
		$schedFrequencyMap = array('weekly' => t('LBL1197'),//Weekly
		    											 'yearly'  => t('LBL1195'),//Yearly
															 'monthly' => t('LBL1196'),//Monthly
															 'daily'   => t('LBL1198'),//Daily
															);
		
   return $schedFrequencyMap[$frequency];
	}
 catch (Exception $ex) {
		watchdog_exception('fetchScheduleHtml', $ex);
		expertusErrorThrow($ex);
 }	
}

/**
 * Get yearly schedule value to print inside next run column
 */
function getYearlyValueForNextRun ($month, $day) {
	try {
		$monthOptions = array('1'=> t('LBL1208'),//January
				'2' => t('LBL1209'),//February
				'3' => t('LBL1210'),//March
				'4' => t('LBL1211'),//April
				'5' => t('LBL1212'),//May
				'6' => t('LBL1213'),//June
				'7' => t('LBL1214'),//July
				'8' => t('LBL1215'),//August
				'9' => t('LBL1216'),//September
				'10'=> t('LBL1217'),//October
				'11'=> t('LBL1218'),//November
				'12'=> t('LBL1219')//December
		);
		$value = $monthOptions[$month] . ' ' . $day;
		return $value;
	}
	catch (Exception $ex) {
		watchdog_exception('getYearlyValueForNextRun', $ex);
		expertusErrorThrow($ex);
	}
}


/**
 * Get weekly schedule value to print inside next run column
 */
function getWeeklyValueForNextRun ($day) {
	try {
		$weekOptions = array( '1' => t('LBL1199'),//Sunday
				'2' => t('LBL1200'),//Monday
				'3' => t('LBL1201'),//Tuesday
				'4' => t('LBL1202'),//Wednesday
				'5' => t('LBL1203'),//Thursday
				'6' => t('LBL1204'),//Friday
				'7' => t('LBL1205'),//Saturday
		);
		expDebug::dPrint('$$day getWeeklyValueForNextRun ='.print_r($day,true), 4);
		expDebug::dPrint('$weekOptions ='.print_r($weekOptions,true), 4);
		$value = $weekOptions[$day];
		expDebug::dPrint('getWeeklyValueForNextRun -->' . $value, 4);
		return $value;
	
	}
	catch (Exception $ex) {
		watchdog_exception('getWeeklyValueForNextRun', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Prepares HTML for the next run status column in the schedules grid
 */
function fetchNextRunHtml($nextRun, $schedFreq, $schedTZ) {
	try {
		expDebug::dPrint('$nextRun = ' . $nextRun, 4);
		expDebug::dPrint('$schedTZ = ' . $schedTZ, 4);
		
		$defaultTimezone =  date_default_timezone(FALSE);
		expDebug::dPrint('$defaultTimezone = ' . print_r($defaultTimezone, true), 4);
		$dateTimeObj = new DateObject($nextRun, $defaultTimezone, 'Y-m-d H:i:s');
		expDebug::dPrint('$scheduledNextRunTime = ' . date_format($dateTimeObj, 'Y-m-d H:i:s'), 4);
		
		if (!empty($schedTZ) && $schedTZ != $defaultTimezone) {
			// Get corresponding date time in the schedule timzone
			expDebug::dPrint('Converting timezone of date object', 4);
			$dateTimeObj->setTimezone(new DateTimeZone($schedTZ));
		}
		
	  $splitDateTimeObj = reportScheduleSplitDateTimeObj($dateTimeObj);
		expDebug::dPrint('$dateObj = '. print_r($splitDateTimeObj, true), 4);
	  $timeStr = $dateTimeObj->format('H:i');
	  expDebug::dPrint('$timeStr = '. $timeStr);
	 
	   switch ($schedFreq) {
	  	case 'yearly':
	  		$yearlyValue = getYearlyValueForNextRun ($splitDateTimeObj->month, $splitDateTimeObj->day);
	  		$listValue = $yearlyValue . ', ' . $splitDateTimeObj->year . ' ' . t('LBL809') . ' ' . $timeStr;
	  		break;
	  
	  	case 'monthly':
	  		$monthlyValue = getYearlyValueForNextRun ($splitDateTimeObj->month, $splitDateTimeObj->day);
	  		$listValue = $monthlyValue . ', ' . $splitDateTimeObj->year . ' ' . t('LBL809') . ' ' . $timeStr;
	  		break;
	  
	  	case 'weekly':
	  		$weeklyValue = getWeeklyValueForNextRun ($splitDateTimeObj->weekday);
	  		$listValue = $weeklyValue . ' ' . t('LBL809') . ' ' . $timeStr;
	  		break;
	  
	  	case 'daily':
	  		$listValue = mb_convert_case(t('LBL809'), MB_CASE_TITLE, 'UTF-8') . ' ' . $timeStr;
	  		break;
	  } 
	  
	  return $listValue;
		
	}
	catch (Exception $ex) {
		watchdog_exception('fetchNextRunHtml', $ex);
		expertusErrorThrow($ex);
	}
}
/**
 * Prepares HTML for the last run status column in the schedules grid
 */
function fetchRunHtml($scheduleId, $runStatus, $runTime, $scheduleTZ, $export = false) {
	try {
		$statusComment = '';
		if (empty($runTime)) {
			$statusComment = t('ERR230');// Yet to run
		}
		// Build status comment and failure reason
		else if ($runStatus == 'failed') {
			$statusComment = t('ERR228');//Failed due to
			$select = db_select('slt_report_schedules_history', 'sched_hist');
			$select->addField('sched_hist','status_comment', 'stat_comm');
			$select->condition('sched_hist.schedule_id', $scheduleId, '=');
			$select->orderBy('sched_hist.id', 'DESC');
			$select->range(0, 1);
		
			expDebug::dPrintDBAPI('$select', $select);
			$result = $select->execute()->fetchField();
			if(!empty ($result)) {
				$statusComment .= ': ';
				if (strpos($result, 'Exception') !== false) {
				  $statusComment .= t('ERR231'); //System was unable to generate the report due to an internal error.
				}
				else {
					$statusComment .= t($result);
				}
			}
		}
		else {
			$scheduledRunTime = $runTime;
			// Convert run time into schedule timezone
			$systemDefaultTimezone = date_default_timezone(FALSE);
			expDebug::dPrint('$systemDefaultTimezone = ' . $systemDefaultTimezone, 4);
			
			if (!empty($scheduleTZ) && $scheduleTZ != $systemDefaultTimezone) {
				$scheduledRunTimeObj = new DateObject($runTime, $systemDefaultTimezone, 'Y-m-d H:i:s');
				expDebug::dPrint('$scheduledRunTime = ' . date_format($scheduledRunTimeObj, 'Y-m-d H:i:s'), 4);
			
				// Get corresponding date time in the schedule timzone
				expDebug::dPrint('Converting timezone of date object', 4);
				$scheduledRunTimeObj->setTimezone(new DateTimeZone($scheduleTZ));
			
				// Get run time string in schedule timezone
				$scheduledRunTime = date_format($scheduledRunTimeObj, 'Y-m-d H:i:s');
				expDebug::dPrint('$scheduledRunTime in schedule timezone = ' . $scheduledRunTime, 4);
			}
			
			$statusComment =  dateTimeStrToUnivShort($scheduledRunTime, true, true, true, false);//Successful on
		}
		if ($export) {
			return $statusComment;
		}
		$runHTML = '<span class="vtip" title="' . sanitize_data($statusComment) . '" >' . titleController('EXP-SP-LNRREPORTS-REPORT-STATUSCOMMENT', $statusComment, 28) . '</span>' ;
		return $runHTML;
	}
  catch (Exception $ex) {
		watchdog_exception('fetchRunHtml', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Deletes (soft-delete) a report schedule in DB
 */
function deleteReportSchedule($form, $form_state) {
	$txn = db_transaction();
	try {
		$userId = getSltpersonUserId();
	  $scheduleId =	$form_state['schedule_id'];
		
		$schdSelect = db_select('slt_report_schedules', 'rep_sched');
		$schdSelect->addField('rep_sched', 'status');
		$schdSelect->condition('rep_sched.id', $scheduleId);
		expDebug::dPrintDBAPI('$schdSelect', $schdSelect);
		$schdPrevStatuses = $schdSelect->execute()->fetchAll();
		expDebug::dPrint('$schdPrevStatuses' . print_r($schdPrevStatuses, true));
		
		$fields['status'] = 'cre_rpt_rss_del';
		$fields['updated_by'] = $userId;
		$fields['updated_on'] = now();
	
		$updateStmt = db_update('slt_report_schedules');
		$updateStmt->fields($fields);
		$updateStmt->condition('id', $scheduleId, '=');
		expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
		$numUpdated = $updateStmt->execute();
		expDebug::dPrint('$numUpdated = ' . $numUpdated, 4);
		
		make_audit_trail_entries($fields, $userId, $schdPrevStatuses, $scheduleId, 'Updated', 'cre_rpt_rss', 'exp_sp_lnrreports',
				'deleteReportSchedule', array('status'));
			
		}	
		catch (Exception $ex) {
		$txn->rollback();
		watchdog_exception('deleteReportSchedule', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Adds a dropdown field to report schedule addedit form
*/
function addDateDropDownToReportScheduleAddEditForm(&$form, $id, $title = '', $options = array(), $defaultOption = '',
                                                            $tabIndex = null, $mandatory = false, $readOnly = false,$attributes = array()) {
  try {
		//Using the below veriable we can find current loaded theme name
		global $theme_key;
	
		//$attributes = array();
		$attributes['autocomplete'] = 'off'; // Prevent browser autocomplete
	
		// We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
		// each time the form is re-built.
		if($attributes['class']){
			$attributes['class'] = array($attributes['class']);
		}
		if(is_array($attributes['class'])){
			$attributes['class'][] = 'addedit-edit-' . $id;
		}
		else{
			$attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
		}
		$attributes['onchange'][] = 'textfieldTitleChange(this);';
	
		// Add class to apply common style to all drop-downs
		$attributes['class'][] = 'addedit-edit-dropdown';
		$attributes['class'][] = 'addedit-edit-date-criteria-dropdown'; // for applying javascript behavior
	
		if ($readOnly) {
			$attributes['class'][] = 'addedit-readonly-dropdown';
		}
	
		if (isset($tabIndex) && $tabIndex > 0) {
			$attributes['tabindex'] = $tabIndex;
		}

		$finalOptions = $options;
		foreach($finalOptions as $key => $value){
			$finalOptions[$key] = t($value);
		}
	
		$newuiPrefix = '';
    $newuiSuffix = '';
		if($theme_key == 'expertusoneV2') {
			if($id == 'crs_language' || $id == 'lang_code') {
				$newuiPrefix = '<div class="expertus-dropdown-bg crs-lang-dropdown">' .
				                 '<div class="expertus-dropdown-icon">';
			}
			else {
				$newuiPrefix = '<div class="expertus-dropdown-bg">' .
				                 '<div class="expertus-dropdown-icon">';
			  $newuiSuffix =   '</div>' .
			                 '</div>';
			}
		}

    $prefix = '';
    $suffix = '';
		if (empty($form['addedit_tmp_storage']['two_column'])) {
			$prefix = addEditGetPrefix('FIELD', $title, $mandatory);
			$suffix = addEditGetSuffix('FIELD');
		}
		else {
			if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
				$prefix = addEditGetPrefix('TWOCOL_FIRSTCOL_FIELD', $title, $mandatory);
				$suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD');
				$form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
			}
			else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
				$prefix = addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $title, $mandatory);
				$suffix = addEditGetSuffix('TWOCOL_SECONDCOL_FIELD');
				$form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
			}
			else {
				throw new Exception('exp_sp_administration.inc : addDropDownToAddEditForm() : Attempt to add a third field to a two column row.');
			}
		}
		$form[$id] = array(
		  '#type' => 'select',
		  '#prefix' => $newuiPrefix,
			'#suffix' => $newuiSuffix,
			'#title' => $title,
			'#title_display' => 'invisible',
			'#options'=> $finalOptions,
			'#default_value' => $defaultOption,
			'#disabled' => $readOnly,
			'#attributes' => $attributes
		);
	
		$replacedId = str_replace('_', '-', $id);
		$form['load_select_drop_down_' . $id] = array (
		  '#markup' => '<img src=\'themes/seven/images/buttons.png\' width="0" height="0" style="display:none;" onload="textfieldTitleChangeField(\'edit-'.$replacedId.'\');" />',
		);
  }
	catch (Exception $ex) {
    watchdog_exception('addDateDropDownToReportScheduleAddEditForm', $ex);
    expertusErrorThrow($ex);
  }	
}

/**
 * Adds a reports criteria filter text field to the reports schedule addedit form
 */
function addCriteriaTextFieldToReportScheduleAddEditForm(&$form, $id, $title = '', $defaultValue = '', $tabIndex = null, $mandatory = false,
                                                                 $readOnly = false, $maxChars = 99999, $viewWidthInChars = 60, $description = false) {
  try {
		// Prepare the additional HTML attributes for the textfield
		$attributes = array();
		$attributes['autocomplete'] = 'off'; // Prevent browser autocomplete
	
		// We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
		// each time the form is re-built.
		$attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
	
		// Add class to apply common style to all text fields.
		$attributes['class'][] = 'addedit-edit-textfield';
	
		if ($readOnly) {
			$attributes['class'][] = 'addedit-readonly-textfield';
		}
	
		if (isset($tabIndex) && $tabIndex > 0) {
			$attributes['tabindex'] = $tabIndex;
		}
	
		// If the view width is larger than max allowed characters, make view width equal to the max allowed characters
		if ($viewWidthInChars > $maxChars) {
			$viewWidthInChars = $maxChars;
		}
		if($defaultValue == t('MSG447')){
			$attributes['class'][] = 'narrow-search-filterset-daterange-empty';
		}
		$prefix = '';
		$suffix = '';
		if (empty($form['addedit_tmp_storage']['two_column'])) {
			$prefix = addEditScheduleGetPrefix('FIELD', $title, $mandatory);
			$suffix = addEditGetSuffix('FIELD');
		}
		else {
			if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
				$prefix = addEditScheduleGetPrefix('TWOCOL_FIRSTCOL_FIELD', $title, $mandatory);
				$suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD');
				$form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
			} else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
				$prefix = addEditScheduleGetPrefix('TWOCOL_SECONDCOL_FIELD', $title, $mandatory);
				$suffix = addEditGetSuffix('TWOCOL_SECONDCOL_FIELD');
				$form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
			} else {
				throw new Exception('exp_sp_administration.inc : addTextFieldToAddEditForm() : Attempt to add a third field to a two column row.');
			}
		}
	
		// Create the form element
		$form[$id] = array(
			'#type' => 'textfield',
			'#prefix' => $prefix,
			'#suffix' => $suffix,
			'#title' => $title,
			'#title_display' => 'invisible',
			'#default_value' => $defaultValue,
			'#disabled' => $readOnly,
			'#attributes' => $attributes,
			'#size' => $viewWidthInChars,
			'#maxlength' => $maxChars,
			'#description' => $description
		);
	}
  catch (Exception $ex) {
    watchdog_exception('addCriteriaTextFieldToReportScheduleAddEditForm', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Prepares HTML prefix for schedule addedit form fields
 */
function addEditScheduleGetPrefix($type, $fieldTitle = '', $mandatory = false, $separatorSymbol = ':', $prefixId = ''){
  try {
		$mandatorySymbol = "<span class='addedit-mandatory'>*</span>";
	
		switch (strtoupper($type)){
			case 'FIELD':
				return '<div class="addedit-new-field" '.(!empty($prefixId) ? ('id="'.$prefixId.'"') : '').'>' .
						'<div class="addedit-new-field-title">'
						.'<span class="vtip" title="'.sanitize_data($fieldTitle).'" >'.titleController('EXP-SP-LNRREPORTS-REPORT-FIELDTITLE',$fieldTitle,12).'</span>'
						. $separatorSymbol
						. ($mandatory? $mandatorySymbol : '') .
						'</div>' .
						'<div class="addedit-new-field-value">';
						break;
			case 'SINGLEFIELD':
				return '<div class="addedit-new-field" '.(!empty($prefixId) ? ('id="'.$prefixId.'"') : '').'>' .
						'<div class="addedit-singlefield-title">' .
						($mandatory? $mandatorySymbol : '');
						break;
							
			case 'ACTION' :
				return '<div class="addedit-form-first-misc-action" '.(!empty($prefixId) ? ('id="'.$prefixId.'"') : '').'>' .
						'<span class="addedit-action-button">' .
						'<div class="addedit-form-cancel-and-save-actions-row">' .
						'<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>';
				break;
					
			case 'CHECKBOX_TITLE_SUFFIXED' :
				return '<div class="addedit-new-field" '.(!empty($prefixId) ? ('id="'.$prefixId.'"') : '').'>' .
						'<div class="addedit-new-field-title"></div>' .
						'<div class="addedit-new-field-value">';
				break;
					
			case 'DATEFIELD' :
				return '<div class="addedit-new-field" '.(!empty($prefixId) ? ('id="'.$prefixId.'"') : '').'>' .
						'<div class="addedit-new-field-title">' .
						$fieldTitle . $separatorSymbol . ($mandatory? $mandatorySymbol : '') .
						'</div>' .
						'<div class="addedit-new-field-value">' .
						'<div class="addedit-new-datefield-value">';
						break;
							
			case 'DOUBLEFIELD':
				return '<div class="addedit-new-field" '.(!empty($prefixId) ? ('id="'.$prefixId.'"') : '').'>' .
						'<div class="addedit-new-field-title">' .
						$fieldTitle . $separatorSymbol . ($mandatory? $mandatorySymbol : '') .
						'</div>' .
						'<div class="addedit-new-field-value">';
						break;
							
			case 'DOUBLEDROPDOWN':
				return  '<div class="addedit-new-field-title dropspecific-width" '.(!empty($prefixId) ? ('id="'.$prefixId.'"') : '').' >' .
						$fieldTitle . $separatorSymbol . ($mandatory? $mandatorySymbol : '') .
						'</div>' .
						'<div class="addedit-new-field-value">';
						break;
							
			case 'STATUSFIELD':
				return  '<div class="addedit-new-field-title textspecific-width" '.(!empty($prefixId) ? ('id="'.$prefixId.'"') : '').' >' .
						$fieldTitle . $separatorSymbol . ($mandatory? $mandatorySymbol : '') .
						'</div>' .
						'<div class="addedit-new-field-value addedit-textbox-daterange-date-container">';
						break;
							
			case 'DOUBLEDROPDOWNCURRENCY':
				return  '<div class="addedit-new-field-title curreny-type-label" '.(!empty($prefixId) ? ('id="'.$prefixId.'"') : '').' >' .
						$fieldTitle . $separatorSymbol . ($mandatory? $mandatorySymbol : '') .
						'</div>' .
						'<div class="addedit-new-field-value">';
						break;
							
			case 'TWOCOL_FIRSTCOL_FIELD':
				return '<div class= "addedit-twocol-firstcol" ' . (!empty($prefixId) ? ('id="'.$prefixId.'"') : '').'>' .
						'<div class="addedit-new-field-title">'
						.'<span class="vtip" title="'.sanitize_data($fieldTitle).'" >'.titleController('EXP-SP-LNRREPORTS-REPORT-NEW-FIELD-TITLE',$fieldTitle,8).'</span>'
						. $separatorSymbol . ($mandatory? $mandatorySymbol : '') .
						'</div>' .
						'<div class="addedit-firstcol-field-value">';
						break;
							
			case 'TWOCOL_SECONDCOL_FIELD':
				return  '<div class= "addedit-twocol-secondcol" ' . (!empty($prefixId) ? ('id="'.$prefixId.'"') : '').'>' .
						'<div class="addedit-new-field-title">' .
						$fieldTitle . $separatorSymbol . ($mandatory? $mandatorySymbol : '') .
						'</div>' .
						'<div class="addedit-secondcol-field-value">';
						break;
							
			case 'TWOCOL_FIRSTCOL_CHECKBOX_TITLE_SUFFIXED':
				return  '<div class="addedit-twocol-firstcol" '.(!empty($prefixId) ? ('id="'.$prefixId.'"') : '').'>' .
						'<div class="addedit-new-field-title"></div>' .
						'<div class="addedit-firstcol-field-value">';
				break;
					
			case 'TWOCOL_SECONDCOL_CHECKBOX_TITLE_SUFFIXED':
				return  '<div class="addedit-twocol-secondcol" '.(!empty($prefixId) ? ('id="'.$prefixId.'"') : '').'>' .
						//'<div class="addedit-new-field-title"></div>' .
				'<div class="addedit-secondcol-field-value">';
				break;
					
			case 'TWOCOL_FIRSTCOL_DATEFIELD' :
				return '<div class= "addedit-twocol-firstcol" ' . (!empty($prefixId) ? ('id="'.$prefixId.'"') : '').'>' .
						'<div class="addedit-new-field-title">' .
						$fieldTitle . $separatorSymbol . ($mandatory? $mandatorySymbol : '') .
						'</div>' .
						'<div class="addedit-firstcol-field-value">' .
						'<div class="addedit-new-datefield-value">';
						break;
							
			case 'TWOCOL_SECONDCOL_DATEFIELD':
				return  '<div class= "addedit-twocol-secondcol" ' . (!empty($prefixId) ? ('id="'.$prefixId.'"') : '').'>' .
						'<div class="addedit-new-field-title">' .
						$fieldTitle . $separatorSymbol . ($mandatory? $mandatorySymbol : '') .
						'</div>' .
						'<div class="addedit-secondcol-field-value">' .
						'<div class="addedit-new-datefield-value">';
						break;
		}
	}
  catch (Exception $ex) {
    watchdog_exception('addEditScheduleGetPrefix', $ex);
    expertusErrorThrow($ex);
  }
	
}

/**
 * exports schedules to CSV
 */
function exportSchedulesToCsv($reportId, $exportOption) {
	try {
		  $select = getReportSchedulesForGrid($reportId);

			$colHeadersMap = array(
					t('LBL1207')  => 'frequency',
					t('LBL1239')   => 'next_run',
					t('LBL1224')  => 'last_run',
			);
			SendNarrowSearchResultsAsCSVFile($select, $colHeadersMap, 'export', array(), '', 'schedules_list');
	}	
	catch (Exception $ex) {
		watchdog_exception('exportSchedulesToCsv', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Custom hook_csv_row_data_alter function
 * Called in SendNarrowSearchResultsAsCSVFile()
 */
function schedules_list_csv_row_data_alter($rowData) {
  try {  
    $rowData[0]->next_run = fetchNextRunHtml($rowData[0]->next_run, $rowData[0]->frequency, $rowData[0]->freq_tz_phpname);
    $rowData[0]->last_run = fetchRunHtml($rowData[0]->id, $rowData[0]->last_run_status, $rowData[0]->last_run, $rowData[0]->freq_tz_phpname, true);
  }
  catch (Exception $ex) {
    watchdog_exception('schedules_list_csv_row_data_alter', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Prepares data for the report schedules list jqGrid
 */
function loadReportSchedulesListDataGridType($mode, $type, $searchKeyword, $entityId, $entityType) {
	try {
		$page      = orgValidate(reportXSSValidate($_GET['page']));
		$limit     = orgValidate(reportXSSValidate($_GET['rows']));
		$sidx      = reportXSSValidate($_GET['sidx']);
		$sord      = reportXSSValidate($_GET['sord']);
		$tab       = reportXSSValidate($_GET['tab']);
		$uniqueId  =  "$type-$entityId-$entityType";
		$searchType = reportXSSValidate($_GET['searhType']);
		$countRequired = false;

		$fetchFields = array();
		
		$mode = empty($mode)? 'view' : $mode;
		$countAvailable = 0;
		$count = 0;
		$searchKeyword = str_replace('|', '/', $searchKeyword);
		$countQuery = getReportSchedulesForGrid($entityId, true);
		
	  $totalRecords = $countQuery->execute()->fetchField();
	  expDebug::dPrint('$totalRecords = '. $totalRecords, 4);
	  
	  expDebug::dPrint('Fetching records for ReportSchedulesList', 4);
	  $listquery = getReportSchedulesForGrid($entityId);
	  
	  $coloumn = "sched_rep.created_on";
	  if(!isset($sidx)){
	  	$coloumn = "sched_rep.created_on";
	  }
	  if(isset($sidx) && $sidx == "Schedule"){
	  	$coloumn = "sched_rep.frequency";
	  }
	  
	  $start = $limit * $page - $limit;
	  expDebug::dPrint('$start = ' . $start, 4);
	  expDebug::dPrint('$page = ' . $page, 4);
	  expDebug::dPrint('$limit = ' . $limit, 4);
	  $listquery->orderBy($coloumn, $sord);
 	  $listquery->range($start, $limit);
 	  expDebug::dPrintDBAPI('$listquery', $listquery);
   
    // Execute query
    $queryResult = $listquery->execute();
    expDebug::dPrint('$queryResult = ' . print_r($queryResult, true));
   
    if( $totalRecords > 0 ) {
      $total_pages = ceil($totalRecords/$limit);
    }
    else {
      $total_pages = 0;
    }	  
    if ($page > $total_pages)
  	  $page = $total_pages;
    if ($start < 0) {
  	  $start = 0;
    }
    
		expDebug::dPrint('building $srch_result_response_reportSchedule', 4);
		$srch_result_response_reportSchedule->total   = $total_pages;
		$srch_result_response_reportSchedule->records = $totalRecords;
		$srch_result_response_reportSchedule->page    = $page;
		$srch_result_response_reportSchedule->unique_id    = $uniqueId;
		$srch_result_response_reportSchedule->msg    = '';
		
		$_SESSION['jq_count'] = array();
		$i=0;
	
			foreach ($queryResult as $rec_search_results){
				expDebug::dPrint('$rec_search_results = ' . print_r($rec_search_results, true), 4);
				$srch_result_response_reportSchedule->rows[$i]['id']   = $rec_search_results->id;
				$srch_result_response_reportSchedule->rows[$i]['cell'] = array (
						'id'       => $rec_search_results->id,
						'Schedule' => fetchScheduleHtml($rec_search_results->frequency),
						'NextRun'  => ($rec_search_results->status == 'cre_rpt_rss_atv')?
				                       fetchNextRunHtml( $rec_search_results->next_run, $rec_search_results->frequency, $rec_search_results->freq_tz_phpname) : '',						                                           
						'LastRun'  => fetchRunHtml($rec_search_results->id,$rec_search_results->last_run_status,$rec_search_results->last_run,$rec_search_results->freq_tz_phpname),
						'Action'   => theme('schedule-actions', 	array('schedule_id' => $rec_search_results->id,
								                                            'report_id' => $entityId,
																														'status' => $rec_search_results->status))
		
				);
				$i++;
			
		}
		expDebug::dPrint('built $srch_result_response_reportSchedule' . print_r($srch_result_response_reportSchedule, true), 4);
		echo drupal_json_encode($srch_result_response_reportSchedule);
	}
	catch(Exception $ex) {
			watchdog_exception('loadReportSchedulesListDataGridType', $ex);
			expertusErrorThrow($ex);
		}
}

/**
 * Deletes the schedule and repaints the grid.
 */
function deleteScheduleAndRepaint($reportId, $scheduleId) {
	try {		
		deleteReportSchedule(null, array('schedule_id' => $scheduleId));
		expertus_set_message(t('LBL1225'), 'error');//Deleted
		refreshReportSchedulesPopup(null, array('report_id' => $reportId));		
	}
	catch(Exception $ex) {
		watchdog_exception('deleteScheduleAndRepaint', $ex);
		expertusErrorThrow($ex);
	}
	
}

/**
 * Deactivate the schedule
 */
function deactivateReportSchedule($scheduleId) {
	try {
		$userId = getSltpersonUserId();
		
		$fields['status'] = 'cre_rpt_rss_itv';
		$fields['updated_by'] = $userId;
		$fields['updated_on'] = now();
		
		$updateStmt = db_update('slt_report_schedules');
		$updateStmt->fields($fields);
		$updateStmt->condition('id', $scheduleId, '=');
		expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
		$numUpdated = $updateStmt->execute();
		expDebug::dPrint('$numUpdated = '. $numUpdated);
		
		$prevStatusObj = new stdClass();
		$prevStatusObj->status = 'cre_rpt_rss_atv';
		make_audit_trail_entries($fields, $userId, array($prevStatusObj), $scheduleId, 'Updated', 'cre_rpt_rss', 'exp_sp_lnrreports',
				'deactivateReportSchedule', array('status'));
	 
	}
	catch(Exception $ex) {
		watchdog_exception('deactivateReportSchedule', $ex);
		expertusErrorThrow($ex);
	}		
}

/**
 * Toggles status of a schedule in DB
 */
function activateOrDeactivateSchedule($reportId, $scheduleId, $statusFlag) {
  $userId = getSltpersonUserId();
  $txn = db_transaction();
  try {
    if ($statusFlag == 1) { // make active
    	$fields['status'] = 'cre_rpt_rss_atv';
    	
    	$select = db_select('slt_report_schedules', 'rep_sch');
    	$select->leftJoin('slt_profile_list_items', 'spli_tz', 'rep_sch.freq_timezone = spli_tz.code && spli_tz.lang_code = \'cre_sys_lng_eng\'');
    	$select->addField('rep_sch', 'frequency', 'frequency');
    	$select->addField('rep_sch', 'freq_month', 'freq_month');
    	$select->addField('rep_sch', 'freq_day', 'freq_day');
    	$select->addField('rep_sch', 'freq_time', 'freq_time');
    	$select->addField('spli_tz', 'attr2', 'freq_tz_phpname');
    	$select->condition('rep_sch.id', $scheduleId);
    	expDebug::dPrintDBAPI('$select' ,$select);
    	$result = $select->execute()->fetch();
    	expDebug::dPrint('$result = ' .print_r($result,true));
    	
    	//Build the next run time
    	$fields['next_run'] = getReportScheduleNextRunTime(null, $result->frequency, $result->freq_month,
    			 $result->freq_day,  $result->freq_time,  $result->freq_tz_phpname);
    }
    else { // make inactive
    	$fields['status'] = 'cre_rpt_rss_itv';
    }	
    $fields['updated_by'] = $userId;
    $fields['updated_on'] = now();
    
    $updateStmt = db_update('slt_report_schedules');
		$updateStmt->fields($fields);
		$updateStmt->condition('id', $scheduleId, '=');
		expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
		$numUpdated = $updateStmt->execute();
		expDebug::dPrint('$numUpdated = '. $numUpdated);
		
		$prevStatusObj = new stdClass();
		if($fields['status'] == 'cre_rpt_rss_atv'){$prevStatusObj->status = 'cre_rpt_rss_itv';}else{$prevStatusObj->status = 'cre_rpt_rss_atv';}
			make_audit_trail_entries($fields, $userId, array($prevStatusObj), $scheduleId, 'Updated', 'cre_rpt_rss', 'exp_sp_lnrreports',
				'activateOrDeactivateSchedule', array('status'));
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('activateOrDeactivateSchedule', $ex);
    expertusErrorThrow($ex);
  }
  unset($txn);
  expertus_set_message(t('MSG689')); // Details Updated.
  refreshReportSchedulesPopup(null, array('report_id' => $reportId));
}

/**
 * Fetches the status of a schedule from DB
 */
function getScheduleStatus($scheduleId) {
  try {
    $select = db_select('slt_report_schedules', 'schedules');
    $select->addField('schedules','status', 'status');
    $select->condition('schedules.id', $scheduleId);
  
    // Execute the query
    return $select->execute()->fetchField();
  }
  catch(Exception $ex) {
    watchdog_exception('getScheduleStatus', $ex);
    expertusErrorThrow($ex);
  }
}
/**
 *  to check admin privelege users
 */
function getadminUsers() {
	try {
		$select = db_select('slt_groups', 'grp');
		$select->addField( 'grp','userslist','userlist');
		$select->condition('grp.name', 'Administer','=');
		expDebug::dPrintDBAPI('$select getadminUsers =' ,$select);
		// Execute the query
		return $select->execute()->fetchField();
		
	}
	catch(Exception $ex) {
		watchdog_exception('getadminUsers', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Returns sortby column options for report schedule
 */
function getReportScheduleSortByOptions($reportId) {
  try {
    $select = db_select('slt_report_query_builder', 'rep');
    $select->addField('rep', 'column_alias_sql');
    $select->condition('rep.report_details_id', $reportId);
    $select->condition('rep.union_query', '2', '!=');
    expDebug::dPrintDBAPI('$select', $select);
    
    $result = $select->execute()->fetchField();
    expDebug::dPrint('$result = ' . print_r($result, true));
    
    $columnAliases = explode(',', $result);
    
    $optionsList = array_combine($columnAliases, $columnAliases);
    expDebug::dPrint('$optionsList = ' . print_r($optionsList, true), 4);
    
    return $optionsList;
  }
  catch (Exception $ex) {
    watchdog_exception('getReportScheduleSortByOptions', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Builds sort value string for saving in DB
 */
function buildReportScheduleSortValue($sortBy, $sortOrder) {
  try {
    $sortValue = null;
    if (!empty($sortBy)) {
      $sortValue = $sortBy . '~~';
      if (!empty($sortOrder)) {
        $sortValue .= $sortOrder;
      }
      else {
        $sortValue .= 'ASC';
      }
    }
    
    return $sortValue;
  }
  catch (Exception $ex) {
    watchdog_exception('buildReportScheduleSortValue', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Adds sort order dropdown field to report schedule addedit form
*/
function addSortOrderDropDownToReportScheduleAddEditForm(&$form, $id, $title = '', $options = array(), $defaultOption = '',
                                                            $tabIndex = null, $mandatory = false, $readOnly = false,$attributes = array()) {
  try {
    //Using the below veriable we can find current loaded theme name
    global $theme_key;
  
    //$attributes = array();
    $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete
  
    // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
    // each time the form is re-built.
    if($attributes['class']){
      $attributes['class'] = array($attributes['class']);
    }
    if(is_array($attributes['class'])){
      $attributes['class'][] = 'addedit-edit-' . $id;
    }
    else{
      $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
    }
    $attributes['onchange'][] = 'textfieldTitleChange(this);';
  
    // Add class to apply common style to all drop-downs
    $attributes['class'][] = 'addedit-edit-dropdown';
  
    if ($readOnly) {
      $attributes['class'][] = 'addedit-readonly-dropdown';
    }
  
    if (isset($tabIndex) && $tabIndex > 0) {
      $attributes['tabindex'] = $tabIndex;
    }

    $nullOption[] = t('LBL674');

    $finalOptions = $nullOption + $options;
    foreach($finalOptions as $key => $value) {
      $finalOptions[$key] = t($value);
    }
  
    $newuiPrefix = '';
    $newuiSuffix = '';
    if($theme_key == 'expertusoneV2') {
      if($id == 'crs_language' || $id == 'lang_code') {
        $newuiPrefix = '<div class="expertus-dropdown-bg crs-lang-dropdown">' .
                         '<div class="expertus-dropdown-icon">';
      }
      else {
        $newuiPrefix = '<div class="expertus-dropdown-bg">' .
                         '<div class="expertus-dropdown-icon">';
        $newuiSuffix =   '</div>' .
                       '</div>';
      }
    }

    if (!empty($form['addedit_tmp_storage']['two_column'])) {
      if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
        $form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
      }
      else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
        $form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
      }
      else {
        throw new Exception('exp_sp_administration.inc : addDropDownToAddEditForm() : Attempt to add a third field to a two column row.');
      }
    }
    $form[$id] = array(
      '#type' => 'select',
      '#prefix' => $newuiPrefix,
      '#suffix' => $newuiSuffix,
      '#title' => $title,
      '#title_display' => 'invisible',
      '#options'=> $finalOptions,
      '#default_value' => $defaultOption,
      '#disabled' => $readOnly,
      '#attributes' => $attributes
    );
  
    $replacedId = str_replace('_', '-', $id);
    $form['load_select_drop_down_' . $id] = array (
      '#markup' => '<img src=\'themes/seven/images/buttons.png\' width="0" height="0" style="display:none;" onload="textfieldTitleChangeField(\'edit-'.$replacedId.'\');" />',
    );
  }
  catch (Exception $ex) {
    watchdog_exception('addSortOrderDropDownToReportScheduleAddEditForm', $ex);
    expertusErrorThrow($ex);
  } 
}

/*
 * process function for custom 'date_criteria_radios' form element.
 * Derived from exp_sp_administration_question,inc process_asmt_dpn_radios()
 */
function process_date_criteria_radios($element) {
  try {
    expDebug::dPrint('$element = ' . print_r($element, true), 5);
    $value = is_array($element['#value'])? $element['#value'] : array();
    $tabIndex =  empty($element['#attributes']['tabindex'])? 0 : $element['#attributes']['tabindex'];

    $element += array('date-criteria-radio-date' => array());
    $dateRadioAttributes = array();
    $dateRadioAttributes['class'] = array('date-criteria-radio-container');
    if ($tabIndex > 0) {
      $dateRadioAttributes['tabindex'] = $tabIndex;
      $tabIndex++;
    }
    // Generate the parents as the autogenerator does, so we will have a
    // unique id for each radio button.
    //$parents_for_id = array_merge($element['#parents'], array('date-criteria-radio'), array($key));
    $element['date-criteria-radio-date'] += array(
      '#type' => 'radio',
      //'#title' => $choice,
      // The key is sanitized in drupal_attributes() during output from the
      // theme function.
      '#return_value' => 'date',
      '#default_value' => isset($value['date-criteria-radio']) ? $value['date-criteria-radio']  : NULL,
      '#attributes' => $dateRadioAttributes,
      '#parents' => array_merge($element['#parents'], array('date-criteria-radio')), // This gives the radio buttons a common name
      //'#id' => drupal_html_id('edit-' . implode('-', $parents_for_id)),
      '#ajax' => NULL,
    );
    
    // Maintain order of options as defined in #options, in case the element
    // defines custom option sub-elements, but does not define all option
    // sub-elements.
    $element += array('date-criteria-date' => array());   
    $defaultDate =  $defaultValue['date'];
    if($defaultDate!='') $defaultDate = date('m-d-Y', strtotime($defaultDate));
    $dateCriteriaTextAttributes = array();
    $dateCriteriaTextAttributes['autocomplete'] = 'off';
    $dateCriteriaTextAttributes['class'] = array('date-criteria-date-class');
    
    $dateCriteriaTextAttributes['data-datePickerDatesDisplayOption'] = "all";
    $dateCriteriaTextAttributes['data-emptyDateFieldText'] = 'mm-dd-yyyy';
    if ($tabIndex > 0) {
    	$dateCriteriaTextAttributes['tabindex'] = $tabIndex;
    	$tabIndex++;
    }
    $dateCriteriaTextAttributes['data-datePickerTooltip'] = '';
    $dateCriteriaTextAttributes['class'][] = (($readOnly) ? 'addedit-readonly-datefield' : 'addedit-edit-datefield');
    $dateCriteriaTextAttributes['class'][] = 'addedit-edit-ac-textfield';
    expDebug::dPrint('issetvalue ='.isset($value['date-criteria-date']),4);
    $element['date-criteria-date'] += array(
      '#type' => 'textfield',
      '#default_value' => isset($value['date-criteria-date']) ? $value['date-criteria-date'] : '',
      '#maxlength' => '500',
      '#size' => '15',
      '#attributes' => $dateCriteriaTextAttributes,
    );
    
    $element += array('date-criteria-radio-day' => array());
    $dateRadioAttributes = array();
    $dateRadioAttributes['class'] = array('date-criteria-radio-container');
    if ($tabIndex > 0) {
      $dateRadioAttributes['tabindex'] = $tabIndex;
      $tabIndex++;
    }
    // Generate the parents as the autogenerator does, so we will have a
    // unique id for each radio button.
    //$parents_for_id = array_merge($element['#parents'], array('date-criteria-radio'), array($key));
    $element['date-criteria-radio-day'] += array(
      '#type' => 'radio',
     // '#title' => $choice,
      // The key is sanitized in drupal_attributes() during output from the
      // theme function.
      '#return_value' => 'day',
      '#default_value' => isset($value['date-criteria-radio']) ? $value['date-criteria-radio']  : NULL,
      '#attributes' => $dateRadioAttributes,
      '#parents' => array_merge($element['#parents'], array('date-criteria-radio')), // This gives the radio buttons a common name
      //'#id' => drupal_html_id('edit-' . implode('-', $parents_for_id)),
      '#ajax' => NULL,
    );
    
    // Maintain order of options as defined in #options, in case the element
    // defines custom option sub-elements, but does not define all option
    // sub-elements.
    $element += array('date-criteria-days' => array());
    $dateCriteriaTextAttributes = array();
    $dateCriteriaTextAttributes['autocomplete'] = 'off';
    $dateCriteriaTextAttributes['class'] = array('date-criteria-days-class');
    if ($tabIndex > 0) {
      $dateCriteriaTextAttributes['tabindex'] = $tabIndex;
      $tabIndex++;
    }

    $element['date-criteria-days'] += array(
      '#type' => 'textfield',
      '#default_value' => isset($value['date-criteria-days']) ? $value['date-criteria-days'] : '',
      '#maxlength' => '500',
      '#size' => '15',
      '#attributes' => $dateCriteriaTextAttributes,
    );
    
    // Maintain order of options as defined in #options, in case the element
    // defines custom option sub-elements, but does not define all option
    // sub-elements.
    $element += array('date-criteria-days-type' => array());
    $dateCriteriaDaysTypeAttributes = array();
    $dateCriteriaDaysTypeAttributes['class'] = array('date-criteria-days-type');
    if ($tabIndex > 0) {
      $dateCriteriaDaysTypeAttributes['tabindex'] = $tabIndex;
      $tabIndex++;
    }
    
    $availableOptions = array('DB' => t('LBL1222'),//Days Before
                              'DA'=> t('LBL1223'),//Days After
                             );
    
    $element['date-criteria-days-type'] += array(
      '#type' => 'select',
      '#default_value' => isset($value['date-criteria-days-type']) ? $value['date-criteria-days-type'] : 'DB',
      '#attributes' => $dateCriteriaDaysTypeAttributes,
      '#options'=> $availableOptions,
    );

    expDebug::dPrint('returning $element = ' . print_r($element, true), 5);
    return $element;
  }
  catch (Exception $ex) {
    watchdog_exception('process_date_criteria_radios', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * theme function for date_criteria_radios
 * derived from asmt_dpn_radios()
 */
function theme_date_criteria_radios($variables) {
  try {
    expDebug::dPrint('$variables = ' . print_r($variables, true), 5);
  
    $element = $variables['element'];
  
    $element['#children'] .=    '<div class="date-criteria-radios-wrapper">'.
                                 '<div class="date-criteria-radio date-criteria-radio-date">' .
                                   theme('radio', array('element' => $element['date-criteria-radio-date'])) . '<span>&nbsp;</span>' .
                                 '</div>' .
                                 '<div class="date-criteria-textfield date-criteria-date">' .
                                   drupal_render($element['date-criteria-date']) . 
                                 '</div>' .
                                 '<div class="date-criteria-radio date-criteria-radio-day">' .
                                   theme('radio', array('element' => $element['date-criteria-radio-day'])) . '<span>&nbsp;</span>' .
                                 '</div>' .
                                 '<div class="date-criteria-textfield date-criteria-days">' .
                                   drupal_render($element['date-criteria-days']) . 
                                 '</div>' .
                                 '<div class="dropdown-style-container">'.
                                  '<div class="expertus-dropdown-bg">'.
                                   '<div class="expertus-dropdown-icon">'.
                                    '<div class="date-criteria-dropdown date-criteria-days-type">' .
                                     drupal_render($element['date-criteria-days-type']) . 
                                    '</div>' .
                                   '</div>' .
                                  '</div>'.
                                 '</div>'. 
                                 '<div class ="date-criteria-text">'.
                                  t('run date') .
                                 '</div>'.
                                '</div>';
    /*
     * if($theme_key == 'expertusoneV2') {
			$newuiPrefix = '<div class="expertus-dropdown-bg">' .
			                 '<div class="expertus-dropdown-icon">';
			$newuiSuffix =   '</div>' .
			               '</div>';
		}
     * */
     
    $attributes = array();
    if (isset($element['#id'])) {
      $attributes['id'] = $element['#id'];
    }
    $attributes['class'] = 'form-radios';
    if (!empty($element['#attributes']['class'])) {
      $attributes['class'] .= ' ' . implode(' ', $element['#attributes']['class']);
    }
  
    return '<div' . drupal_attributes($attributes) . '>' . (!empty($element['#children']) ? $element['#children'] : '') . '</div>';
  }
  catch (Exception $ex) {
    watchdog_exception('theme_date_criteria_radios', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * value callback for date-criteria-radios
 * Derived from asmt_dpn_radios_value_callback()
 */
function date_criteria_radios_value_callback($element, $input = FALSE) {
  try {
    expDebug::dPrint('$element = ' . print_r($element, true), 5);
    expDebug::dPrint('$input = ' . print_r($input, true), 5);
    $elementValue = array();
    if ($input === FALSE) {
      $element += array('#default_value' => NULL); // add a default value to the element if not present
        // Further explode the default value
        $defaultDateText = '';
        $defaultDayText = '';
        $defaultDateType = 'DB';
        $defaultOption = 'date';
        if (!empty($element['#default_value'])) {
          if (endswith($element['#default_value'], 'DB')) {
            $defaultDayText = substr($element['#default_value'], 0, -2);
            $defaultDateType = 'DB';
            $defaultOption = 'day';
          }
          else if (endswith($element['#default_value'], 'DA')) {
            $defaultDayText = substr($element['#default_value'], 0, -2);
            $defaultDateType = 'DA';
            $defaultOption = 'day';
          }
          else {
            $defaultDateText = $element['#default_value'];
            $explodedDateField = explode('-',$element['#default_value']);
            $year = $explodedDateField[0];
            $explodedDateField[0] = $explodedDateField[1];
            $explodedDateField[1] = $explodedDateField[2];
            $explodedDateField[2] = $year;
            $defaultDateText = implode('-',$explodedDateField);
          }
        }
        
        $elementValue['date-criteria-date'] = $defaultDateText;
        $elementValue['date-criteria-radio'] = $defaultOption;
        $elementValue['date-criteria-days'] = $defaultDayText;
        $elementValue['date-criteria-days-type'] = $defaultDateType;
        
    }
    elseif (is_array($input)) {
      $elementValue = $input;
    }
  
    expDebug::dPrint('$elementValue = ' . print_r($elementValue, true), 4);
    return $elementValue;
  }
  catch (Exception $ex) {
    watchdog_exception('date_criteria_radios_value_callback', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * Adds date criteria custom field to $form
 */
function addDateCriteriaFieldToAddEditForm(&$form, $id, $title, $defaultValue, $tabIndex, $mandatory) {
  try {
  	$attributes = array();
  	// We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  	// each time the form is re-built.
  	$attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  	$attributes['class'][] = 'addedit-date-criteria-container';
  	
  	if (isset($tabIndex) && $tabIndex > 0) {
  		$attributes['tabindex'] = $tabIndex;
  	}
  	if ($readOnly) {
  		$attributes['disabled'] = 'disabled';
  		$attributes['class'][] = 'addedit-readonly-textfield';
  	}	
  		//$attributes['class'][] = 'addedit-readonly-textfield'; // Add another class
 
    $prefix = '';
    $suffix = '';
    if (empty($form['addedit_tmp_storage']['two_column'])) {
      $prefix = addEditScheduleGetPrefix('FIELD', $title, $mandatory);
      $suffix = addEditGetSuffix('FIELD');
    }
    else {
      if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
        $prefix = addEditScheduleGetPrefix('TWOCOL_FIRSTCOL_FIELD', $title, $mandatory);
        $suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
      } else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
        $prefix = addEditScheduleGetPrefix('TWOCOL_SECONDCOL_FIELD', $title, $mandatory);
        $suffix = addEditGetSuffix('TWOCOL_SECONDCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
      } else {
        throw new Exception('exp_sp_administration.inc : addTextFieldToAddEditForm() : Attempt to add a third field to a two column row.');
      }
    }
    $form[$id] = array (
      '#type' => 'date_criteria_radios',
      '#prefix' => $prefix,
      '#suffix' => $suffix,
      '#title_display' => 'invisible',
      '#title' => $title,
      '#default_value' =>  $defaultValue,
      '#attributes' => $attributes,
    );
  }
  catch (Exception $ex) {
    watchdog_exception('addDateCriteriaFieldToAddEditForm', $ex);
    expertusErrorThrow($ex);
  }
}

function reportlaunchcount($reportId){
	try{
		expDebug::dPrint('Launching reportId = ' . print_r($reportId, true), 5);

		$selectQuery = db_select('slt_report_details','rep');
		$selectQuery->addField('rep', 'launch_count');
		$selectQuery->condition('rep.id', $reportId);
		expDebug::dPrintDBAPI('launch count selectQuery', $selectQuery);
		$launchcount = $selectQuery->execute()->fetchField();
		$launch_count=$launchcount+1;
		
		expDebug::dPrint('$launch_count = ' . $launch_count);

		$fields['launch_count'] = $launch_count;

		$updateQuery = db_update('slt_report_details');
		$updateQuery->fields($fields);
		$updateQuery->condition('id', $reportId);
		$updateQuery->execute();
		expDebug::dPrintDBAPI('launch count updateQuery', $updateQuery);


		//print $result;
	}catch(Exception $ex){
		watchdog_exception('reportlaunchcount', $ex);
	}
}

function dateFormatCheck($date) {
	try{
	if(preg_match("/^(\d{4})-(\d{2})-(\d{2})$/", $date, $matches))
	{
		if(checkdate($matches[2], $matches[3], $matches[1]))
		{
			return '%Y-%m-%d';
		}
	}
	else if(preg_match("/^(\d{2})\/(\d{2})\/(\d{4})$/", $date, $matches))
	{
		if(checkdate($matches[1], $matches[2], $matches[3]))
		{
			return '%m/%d/%Y';
		}else if(preg_match("/^(\d{2})\/(\d{2})\/(\d{4})$/", $date, $matches)){
			if(checkdate($matches[2], $matches[1], $matches[3]))
			{
				return '%d/%m/%Y';
			}
 
		}
	}

	else if(preg_match("/^(\d{2})-(\d{2})-(\d{4})$/", $date, $matches))
	{
		if(checkdate($matches[1], $matches[2], $matches[3]))
		{
			return '%m-%d-%Y';
		}
	}
	}catch(Exception $ex){
		watchdog_exception('dateFormatCheck', $ex);
	}
}
function getalltables(){
	try{
		global $databases;
		//expDebug::dPrint("inside tabel");		
		$tablelistQry = db_query('show tables from '.$databases['default']['report']['database']);
		$tablelist = $tablelistQry->fetchAll(PDO::FETCH_COLUMN);		
		$tablesin = "tables_in_".$databases['default']['report']['database'];
		foreach($tablelist as $tables){		
			if (strpos($tables, 'slv_') !== false){
   				 unset($tables);
			}	else{
				$tablearray[] = $tables;
			}		
		}
		//expDebug::dPrint("getalltables -->".print_r($tablearray,true),5);
		return $tablearray;
		
	}catch(Exception $ex){
		watchdog_exception('getalltables', $ex);
	}
}
function getReportCriteriaQueryFromViews($table_name, $field_name){
    try{
        $select = db_select('slt_report_autocomplete', 'aut');
        $select->addField('aut', 'criteria_query');
        $select->addField('aut', 'criteria_union_query');
        if(count(explode(".",$field_name))<=2)
        	$select->where('aut.field_name = SUBSTRING_INDEX(\''.$field_name.'\', \'.\' ,-1)');
        else 
        	$select->where('aut.field_name = \''.$field_name.'\'');
        $select->condition('aut.view_name', $table_name);
        expDebug::dPrintDBAPI('select report critria query', $select);
        $result = $select->execute()->fetch();
        expDebug::dPrint('report criteria '.print_r($result, 1));
        return $result;

    }catch(Exception $ex){
        watchdog_exception('getReportCriteriaQueryFromViews', $ex);
    }
}
/**
 * 
 * @param value of array $item
 * @param key of array $key
 * @param condition string to checked $cond
 * Array walk function used to check a string in the nested array
 * Builds an array in the global a variable
 */
function arrayCheckRef($item, $key,$cond) {
	global $next, $finalArray;
	//expDebug::dPrintDBAPI('Key inside array walk function', $key);
	if (is_array($item)) {
		array_walk($item, 'arrayCheckRef',$cond);
	}else{
		//expDebug::dPrintDBAPI('$item inside array walk function', $item);
	}
	if ($next) {
		array_push($finalArray, $item);
		//expDebug::dPrintDBAPI('Cond matched inside array walk function', $finalArray);
		$next = false;
	}

	if ($key == 'expr_type' && $item == $cond) {
		$next = true;
	}
}


?>