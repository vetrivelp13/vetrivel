<?php
/*
 * getOrgName() - Fetch an organization's name
 */
function getOrgName($orgId) {
	try {
  if (empty($orgId)) {
    return '';
  }
  
  // Select slt_organization table
  $select = db_select('slt_organization', 'org');
  
  // Get org id
  $select->addField('org', 'name', 'name');
  
  // Set the conditions
  $select->condition('org.id', $orgId, '=');

  expDebug::dPrintDBAPI(' $select obj = ' , $select);
  
  // Execure the query
  $result = $select->execute()->fetchAll();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  
  if (count($result) <= 0) {
    return ''; // Does not exist
  }
  
  return $result[0]->name;
  } catch (Exception $ex) {
  	watchdog_exception('getOrgName', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * getAllOrganizationSelectOptionsList() - 
 */
//change by ayyappans for 33432: Issue in Users
function getAllOrganizationSelectOptionsList($excludedOrgId, $codeRequired = 1, $fromAjax = false, $selectedOrgId) {
	try {
  expDebug::dPrint(' $excludedOrgId = '. print_r($excludedOrgId, true) , 4) ;
  $idAli	= $fromAjax ? 'opt' : 'id';
  $nameAli	= $fromAjax ? 'val' : 'name';
  $seleAli	= 'sel';
	
 // Select the table
  $select = db_select('slt_organization', 'org');
  
if ((!empty($excludedOrgId)) && $excludedOrgId > 0) {
    /*$excludeList = getDescendantOrgIds($excludedOrgId);
    $excludeList[] = $excludedOrgId; // Append array
    $select->condition('org.id', $excludeList, 'NOT IN');*/
	//fix for ticket 11298 - issue 11
	$select->condition('org.id', $excludedOrgId, '<>');
 }

  // Add fields to fetch
  $select->addExpression('org.id', $idAli);
  $select->addExpression('org.name', $nameAli);
  //$select->addExpression('org.number', 'number');
  
  // Add condition
  $select->condition('org.status', 'cre_org_sts_act');
  $select->condition('org.name', '', '!=');
  filterByUserAccessPrivileges($select,'org','',1,'cre_org','attach','org');
  if($fromAjax) {
  	$searchText	= $_GET['z'];
  	$page		= $_GET['page'];
  	$limit		= $_GET['rows'];
  	$start		= $limit * $page - $limit;
  	$select->addExpression('if(find_in_set(org.id, :select_id) > 0 ,1 ,0)', 'sel', array('select_id'=>$selectedOrgId));
  	if($searchText != '' && $searchText != null) {
  		$select->condition('org.name', '%' . db_like($searchText) . '%', 'LIKE');
  	}
  	$select->orderBy('sel','desc');
  	$select->orderBy('val');
  	expDebug::dPrintDBAPI("Access getOrganizationLst countQuery ",$select->countQuery());
  	$count = $select->countQuery()->execute()->fetchField();
  	
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
  	$select->range($start,$limit);
  	expDebug::dPrint('ayyappan bm $start and $end'.$start.' '.$end, 4);
  	expDebug::dPrintDBAPI("Access getOrganizationLst Query ",$select, 4);
  	$organization = $select->execute()->fetchAll();
  	$output = array();
  	$output['records'] = $count ;
  	$output['page'] = $page ;
  	$output['data'] = $organization;
  	expDebug::dPrint('$output'.print_r($output, 1));
  	return $output;
  }
  
  $select->orderBy('name');
  
  expDebug::dPrintDBAPI(' $select obj = ' , $select);
  
  
  //$orgsList = $select->execute()->fetchAllKeyed();
  $orgsList = $select->execute()->fetchAll();
  expDebug::dPrint(' $orgsList = '. print_r($orgsList, true) , 3);
  
  $orgOptions = array();
  foreach ($orgsList as $org) {
    /*if($codeRequired){
      $orgOptions[$org->id] = $org->name . ' ' . ' (' . $org->number . ')' ;
    } else {*/
      $orgOptions[$org->id] = $org->name ;
    //}	
  }
  
  return $orgOptions;
  } catch (Exception $ex) {
  	watchdog_exception('getAllOrganizationSelectOptionsList', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * getAllJobRoleSelectOptionsList() -
*/
function getAllJobRoleSelectOptionsList($excludedOrgId, $codeRequired = 1, $fromAjax = false, $selectedJobroleId) {
	try {
		expDebug::dPrint(' $excludedRoleId = '. print_r($excludedJobroleId, true) , 4) ;
		$idAli	= $fromAjax ? 'opt' : 'id';
		$nameAli	= $fromAjax ? 'val' : 'name';
		$seleAli	= 'sel';

		// Select the table
		$select = db_select('slt_profile_list_items', 'spli');

		if ((!empty($excludedJobroleId)) && $excludedJobroleId > 0) {
			$select->condition('spli.code', $excludedJobroleId, '<>');
		}

		// Add fields to fetch
		$select->addExpression('spli.code', $idAli);
		$select->addExpression('spli.name', $nameAli);
			// Add condition
		$select->condition('spli.is_active', 'Y','=');
		$select->condition('spli.code', '%cre_usr_jrl_%', 'LIKE');
		filterByUserAccessPrivileges($select, 'spli','',1,'cre_usr_jrl','','spli');
		if($fromAjax) {
			$searchText	= $_GET['z'];
			$page		= $_GET['page'];
			$limit		= $_GET['rows'];
			$start		= $limit * $page - $limit;
			$select->addExpression('if(find_in_set(spli.code, :select_id) > 0 ,1 ,0)', 'sel', array('select_id'=>$selectedJobroleId));
			if($searchText != '' && $searchText != null) {
				$select->condition('spli.name', '%' . db_like($searchText) . '%', 'LIKE');
			}
			$select->orderBy('sel','desc');
			$select->orderBy('val');
			expDebug::dPrintDBAPI("Access getJobRoleLst countQuery ",$select->countQuery());
			$count = $select->countQuery()->execute()->fetchField();
			 
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
			$select->range($start,$limit);
			expDebug::dPrint('yogaraja bm $start and $end'.$start.' '.$end, 4);
			expDebug::dPrintDBAPI("Access getOrganizationLst Query ",$select, 4);
			$jobrole = $select->execute()->fetchAll();
			$output = array();
			$output['records'] = $count ;
			$output['page'] = $page ;
			$output['data'] = $jobrole;
			expDebug::dPrint('$output'.print_r($output, 1));
			return $output;
		}
		$select->orderBy('name');
		expDebug::dPrintDBAPI(' $select obj = ' , $select);
		$orgsList = $select->execute()->fetchAll();
		expDebug::dPrint(' $$jobrole = '. print_r($$jobrole, true) , 3);

		$orgOptions = array();
		foreach ($jobrole as $role) {
			$jobRoleOptions[$role->code] = $role->name ;
		}

		return $jobRoleOptions;
	} catch (Exception $ex) {
		watchdog_exception('getAllJobRoleSelectOptionsList', $ex);
		expertusErrorThrow($ex);
	}
}
function getJobRoleAutoComplete($excludedOrgId, $codeRequired = 1, $fromAjax = false, $selectedJobroleId) {
	try {
		expDebug::dPrint(' $excludedRoleId = '. print_r($excludedJobroleId, true) , 4) ;
		$idAli	= $fromAjax ? 'opt' : 'id';
		$nameAli	= $fromAjax ? 'val' : 'name';
		$seleAli	= 'sel';

		// Select the table
		$select = db_select('slt_profile_list_items', 'spli');

		if ((!empty($excludedJobroleId)) && $excludedJobroleId > 0) {
			$select->condition('spli.code', $excludedJobroleId, '<>');
		}

		// Add fields to fetch
		$select->addExpression('spli.code', $idAli);
		$select->addExpression('spli.name', $nameAli);
		// Add condition
		$select->condition('spli.is_active', 'Y','=');
		$select->condition('spli.code', '%cre_usr_jrl_%', 'LIKE');
		expDebug::dPrint(' $fromAjax = '. print_r($fromAjax, true) , 4) ;
		$searchText	= $_GET['z'];
		$limit		= $_GET['limit'];
		if($searchText != '' && $searchText != null) {
			$select->condition('spli.name', '%' . db_like($searchText) . '%', 'LIKE');
		}
		filterByUserAccessPrivileges($select, 'spli','',1,'cre_usr_jrl','','spli');
		expDebug::dPrintDBAPI("Access getJobRoleLst countQuery ",$select->countQuery());
		$count = $select->countQuery()->execute()->fetchField();
		if ($limit > 0) {
			$select->range(0, $limit);
		}
		expDebug::dPrint('yogaraja bm $start and $end'.$start.' '.$end, 4);
		$select->orderBy('name');
		expDebug::dPrintDBAPI("Access getJOB ROLE Lst Query ",$select, 4);
		$jobrole = $select->execute()->fetchAll();
		foreach ($jobrole as $role) {
			expDebug::dPrint(' $role = ' . print_r($role, true),3);
			$choiceStr = '';
			$choiceStr .= $role->name;
			expDebug::dPrint(' $choiceStr = ' . print_r($choiceStr, true),4);
			print $choiceStr . "\n";
		}
		expDebug::dPrint(' $$jobrole = '. print_r($$jobrole, true) , 3);
	} catch (Exception $ex) {
		watchdog_exception('getJobRoleAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}



/*
 * getMatchingOrgsList() - Returns matching organization names with ids. Function called in user admin (org ac field) and org admin (parent org ac field).
 *                         $hiddenOrgId - This is the id set by autocomplete plugin. It corresponds to the organization selected by the user in the
 *                         autocomplete dropdown.
 *                         $pattern - The selected organization name or the string typed by user when nothing was selected from the dropdown.
 *                         $excludedOrgId - any organization and its descendants which are to be excluded from the search.
 */
function getMatchingOrgsList($pattern, $hiddenOrgId = null, $excludedOrgId = null) {
  try {
  // Get list of org ids which are excluded from the search
  $excludedOrgIdList = array();
  if (!empty($excludedOrgId) && $excludedOrgId > 0) {
    $excludedOrgIdList[] = $excludedOrgId;
    $excludedOrgIdList = array_merge($excludedOrgIdList, getDescendantOrgIds($excludedOrgId));
  }

  // If $hiddenOrgId is present and is active, get its corresponding name.
  // If $pattern matches its name, return this record
  if (!empty($hiddenOrgId) &&  !in_array($hiddenOrgId, $excludedOrgIdList)) {
    // Select the table
    $select = db_select('slt_organization', 'org');
    
    // Select the field
    $select->addField('org', 'id');
    $select->addField('org', 'name');
        
    // Add conditions
    $select->condition('org.id', $hiddenOrgId, '=');
    $select->condition('org.status', 'cre_org_sts_act', '=');
    filterByUserAccessPrivileges($select,'org','',1,'cre_org','attach','org');
    expDebug::dPrintDBAPI(' hidden org id $select SQL = ' , $select);
    
    // Fetch results
    $result = $select->execute()->fetchAll();
    expDebug::dPrint(' hidden org id $result = ' . print_r($result, true) , 3);
  }
  
  // If a record is fetched, and its name matches the $pattern return the record
  if (!empty($result) && $result[0]->name == $pattern) {
      expDebug::dPrint(' pattern matches hidden org_id\'s name');
      return $result;
  }
  
  // $hiddenOrgId is invalid, we need to search the pattern in name field
  // Select the table
  
  $select = db_select('slt_organization', 'org');
  
  // Select the field
    $select->addField('org', 'id');
    $select->addField('org', 'name');
  
  // Add condition
  //$select->condition('org.status', 'cre_org_sts_act', '=');
  //$select->condition('org.name', '%' . db_like($pattern) . '%', 'LIKE');
  $select->condition('org.name' ,$pattern , '=');
  
  if (!empty($excludedOrgId) && $excludedOrgId > 0) {
    $select->condition('org.id', $excludedOrgIdList, 'NOT IN');
  }
  filterByUserAccessPrivileges($select,'org','',1,'cre_org','attach','org');
  expDebug::dPrintDBAPI(' $select SQL = ' , $select);
   
  // Fetch results
  $result = $select->execute()->fetchAll();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
    
  return $result; 
  } catch (Exception $ex) {
  	watchdog_exception('getMatchingOrgsList', $ex);
  	expertusErrorThrow($ex);
  }
}
?>